/*
 * ARMv8 NEON optimizations for QJPEG
 *
 * Copyright (C) 2009 by Quram Co., Ltd.
 * All rights reserved
 *
 */

#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits /* mark stack as non-executable */
#endif

.text
//.arch armv8-a+fp+simd


#define RESPECT_STRICT_ALIGNMENT 1

/*****************************************************************************/

/* Supplementary macro for setting function attributes */
.macro asm_function fname
#ifdef __APPLE__
    .globl _\fname
_\fname:
#else
    .global \fname
#ifdef __ELF__
    .hidden \fname
    .type \fname, %function
#endif
\fname:
#endif
.endm






/*****************************************************************************/

/*
 * jsimd_ycc_nv12_709_extrgb_convert_neon
 * jsimd_ycc_nv12_709_extbgr_convert_neon
 * jsimd_ycc_nv12_709_extrgbx_convert_neon
 * jsimd_ycc_nv12_709_extbgrx_convert_neon
 * jsimd_ycc_nv12_709_extxbgr_convert_neon
 * jsimd_ycc_nv12_709_extxrgb_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    .if \size == 8
        ld1  {v4.b}[0], [U]
        ld1  {v4.b}[1], [U], 1
        ld1  {v5.b}[0], [U]
        ld1  {v5.b}[1], [U], 1
        ld1  {v4.b}[2], [U]
        ld1  {v4.b}[3], [U], 1
        ld1  {v5.b}[2], [U]
        ld1  {v5.b}[3], [U], 1
        ld1  {v4.b}[4], [U]
        ld1  {v4.b}[5], [U], 1
        ld1  {v5.b}[4], [U]
        ld1  {v5.b}[5], [U], 1
        ld1  {v4.b}[6], [U]
        ld1  {v4.b}[7], [U], 1
        ld1  {v5.b}[6], [U]
        ld1  {v5.b}[7], [U], 1
        
        ld1  {v19.8b}, [Y], 8
        ld1  {v18.8b}, [Y2], 8
        prfm pldl1keep, [U, #64]
        prfm pldl1keep, [U, #64]
        prfm pldl1keep, [Y, #64]
		prfm pldl1keep, [Y2, #64]
    .elseif \size == 4
        ld1  {v4.b}[0], [U]
        ld1  {v4.b}[1], [U], 1
        ld1  {v5.b}[0], [U]
        ld1  {v5.b}[1], [U], 1
        ld1  {v4.b}[2], [U]
        ld1  {v4.b}[3], [U], 1
        ld1  {v5.b}[2], [U]
        ld1  {v5.b}[3], [U], 1
        ld1  {v19.b}[0], [Y], 1
        ld1  {v19.b}[1], [Y], 1
        ld1  {v19.b}[2], [Y], 1
        ld1  {v19.b}[3], [Y], 1
        ld1  {v18.b}[0], [Y2], 1
        ld1  {v18.b}[1], [Y2], 1
        ld1  {v18.b}[2], [Y2], 1
        ld1  {v18.b}[3], [Y2], 1
    .elseif \size == 2
        ld1  {v4.b}[4], [U]
        ld1  {v4.b}[5], [U], 1
        ld1  {v5.b}[4], [U]
        ld1  {v5.b}[5], [U], 1
        ld1  {v19.b}[4], [Y], 1
        ld1  {v19.b}[5], [Y], 1
        ld1  {v18.b}[4], [Y2], 1
        ld1  {v18.b}[5], [Y2], 1
    .elseif \size == 1
        ld1  {v4.b}[6], [U], 1
        ld1  {v5.b}[6], [U], 1
        ld1  {v19.b}[6], [Y], 1
        ld1  {v18.b}[6], [Y2], 1
    .else
        .error unsupported macroblock size
    .endif
.endm

.macro do_store bpp, size
    .if \bpp == 24
        .if \size == 8
            st3  {v10.8b, v11.8b, v12.8b}, [RGB], 24
            st3  {v13.8b, v14.8b, v15.8b}, [RGB2], 24
        .elseif \size == 4
            st3  {v10.b, v11.b, v12.b}[0], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[1], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[2], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[3], [RGB], 3
            st3  {v13.b, v14.b, v15.b}[0], [RGB2], 3
            st3  {v13.b, v14.b, v15.b}[1], [RGB2], 3
            st3  {v13.b, v14.b, v15.b}[2], [RGB2], 3
            st3  {v13.b, v14.b, v15.b}[3], [RGB2], 3
        .elseif \size == 2
            st3  {v10.b, v11.b, v12.b}[4], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[5], [RGB], 3
            st3  {v13.b, v14.b, v15.b}[4], [RGB2], 3
            st3  {v13.b, v14.b, v15.b}[5], [RGB2], 3
        .elseif \size == 1
            st3  {v10.b, v11.b, v12.b}[6], [RGB], 3
            st3  {v13.b, v14.b, v15.b}[6], [RGB2], 3
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp == 32
        .if \size == 8
            st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
            st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB2], 32
        .elseif \size == 4
            st4  {v10.b, v11.b, v12.b, v13.b}[0], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[1], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[2], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[3], [RGB], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[0], [RGB2], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[1], [RGB2], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[2], [RGB2], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[3], [RGB2], 4
        .elseif \size == 2
            st4  {v10.b, v11.b, v12.b, v13.b}[4], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[5], [RGB], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[4], [RGB2], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[5], [RGB2], 4
        .elseif \size == 1
            st4  {v10.b, v11.b, v12.b, v13.b}[6], [RGB], 4
            st4  {v14.b, v15.b, v16.b, v17.b}[6], [RGB2], 4
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp==16
        .if \size == 8
            st1  {v25.8h}, [RGB],16
        .elseif \size == 4
            st1  {v25.4h}, [RGB],8
        .elseif \size == 2
            st1  {v25.h}[4], [RGB],2
            st1  {v25.h}[5], [RGB],2
        .elseif \size == 1
            st1  {v25.h}[6], [RGB],2
        .else
            .error unsupported macroblock size
        .endif
     .else
        .error unsupported bpp
    .endif
.endm

.macro generate_jsimd_ycc_rgb_709_convert_neon_nv12 colorid, bpp, r_offs, rsize, g_offs, gsize, b_offs, bsize, r_offs2, g_offs2, b_offs2, defsize

/*
 * 2-stage pipelined YCbCr->RGB conversion
 */

.macro do_yuv_to_rgb_stage1
    uaddw        v6.8h, v2.8h, v4.8b     /* q3 = u - 128 */
    uaddw        v8.8h, v2.8h, v5.8b     /* q2 = v - 128 */
    smull        v20.4s, v6.4h, v1.h[2] /* multiply by -833 */	/* green */
    smlal        v20.4s, v8.4h, v1.h[1] /* multiply by -400 */
    smull2       v22.4s, v6.8h, v1.h[2] /* multiply by -833 */	/* green */
    smlal2       v22.4s, v8.8h, v1.h[1] /* multiply by -400 */
    smull        v24.4s, v8.4h, v1.h[0] /* multiply by 1634 */	/* red */
    smull2       v26.4s, v8.8h, v1.h[0] /* multiply by 1634 */
    smull        v28.4s, v6.4h, v1.h[3] /* multiply by 2066 */	/* blue */
    smull2       v30.4s, v6.8h, v1.h[3] /* multiply by 2066 */
.endm

.macro do_yuv_to_rgb_stage2
    rshrn        v20.4h, v20.4s, #10
    rshrn2       v20.8h, v22.4s, #10
    rshrn        v24.4h, v24.4s, #10
    rshrn2       v24.8h, v26.4s, #10
    rshrn        v28.4h, v28.4s, #10
    rshrn2       v28.8h, v30.4s, #10

    movi         v26.8b, #16
    usubl        v26.8h, v18.8b, v26.8b
    smull        v30.4s, v0.4h, v26.4h
    rshrn        v22.4h, v30.4s, #10
    smull2       v30.4s, v0.8h, v26.8h
    rshrn2       v22.8h, v30.4s, #10

    movi         v26.8b, #16
    usubl        v26.8h, v19.8b, v26.8b
    smull        v30.4s, v0.4h, v26.4h
    rshrn        v18.4h, v30.4s, #10
    smull2       v30.4s, v0.8h, v26.8h
    rshrn2       v18.8h, v30.4s, #10

    add          v26.8h, v24.8h, v22.8h
    add          v30.8h, v28.8h, v22.8h
    add          v22.8h, v20.8h, v22.8h

    add          v20.8h, v20.8h, v18.8h
    add          v24.8h, v24.8h, v18.8h
    add          v28.8h, v28.8h, v18.8h

.if \bpp != 16
    sqxtun       v1\g_offs\defsize, v20.8h
	sqxtun       v1\g_offs2\defsize, v22.8h
    sqxtun       v1\r_offs\defsize, v24.8h
	sqxtun       v1\r_offs2\defsize, v26.8h
    sqxtun       v1\b_offs\defsize, v28.8h
	sqxtun       v1\b_offs2\defsize, v30.8h
.else
    sqshlu       v21.8h, v20.8h, #8
    sqshlu       v25.8h, v24.8h, #8
    sqshlu       v29.8h, v28.8h, #8
    sri          v25.8h, v21.8h, #5
    sri          v25.8h, v29.8h, #11
.endif

.endm

.macro do_yuv_to_rgb_stage2_store_load_stage1
	rshrn        v20.4h, v20.4s, #10
	rshrn        v24.4h, v24.4s, #10
	rshrn        v28.4h, v28.4s, #10

	ld1          {v4.b}[0], [U]
	ld1          {v4.b}[1], [U], 1
	ld1          {v5.b}[0], [U]
	ld1          {v5.b}[1], [U], 1
	ld1          {v4.b}[2], [U]
	ld1          {v4.b}[3], [U], 1
	ld1          {v5.b}[2], [U]
	ld1          {v5.b}[3], [U], 1
	ld1          {v4.b}[4], [U]
	ld1          {v4.b}[5], [U], 1
	ld1          {v5.b}[4], [U]
	ld1          {v5.b}[5], [U], 1
	ld1          {v4.b}[6], [U]
	ld1          {v4.b}[7], [U], 1
	ld1          {v5.b}[6], [U]
	ld1          {v5.b}[7], [U], 1

	rshrn2       v20.8h, v22.4s, #10
	rshrn2       v24.8h, v26.4s, #10
	rshrn2       v28.8h, v30.4s, #10

	movi         v26.8b, #16
	usubl        v26.8h, v18.8b, v26.8b
	smull        v30.4s, v0.4h, v26.4h
	rshrn        v22.4h, v30.4s, #10
	smull2       v30.4s, v0.8h, v26.8h
	rshrn2       v22.8h, v30.4s, #10

	movi         v26.8b, #16
	usubl        v26.8h, v19.8b, v26.8b
	smull        v30.4s, v0.4h, v26.4h
	rshrn        v18.4h, v30.4s, #10
	smull2       v30.4s, v0.8h, v26.8h
	rshrn2       v18.8h, v30.4s, #10

    add          v26.8h, v24.8h, v22.8h
    add          v30.8h, v28.8h, v22.8h
    add          v22.8h, v20.8h, v22.8h

    add          v20.8h, v20.8h, v18.8h
    add          v24.8h, v24.8h, v18.8h
    add          v28.8h, v28.8h, v18.8h
	
    sqxtun       v1\g_offs\defsize, v20.8h
	sqxtun       v1\g_offs2\defsize, v22.8h
    ld1          {v19.8b}, [Y], 8
	ld1          {v18.8b}, [Y2], 8
    sqxtun       v1\r_offs\defsize, v24.8h
	sqxtun       v1\r_offs2\defsize, v26.8h
    prfm         pldl1keep, [U, #64]
    prfm         pldl1keep, [U, #64]
    prfm         pldl1keep, [Y, #64]
	prfm         pldl1keep, [Y2, #64]
    sqxtun       v1\b_offs\defsize, v28.8h
	sqxtun       v1\b_offs2\defsize, v30.8h
    uaddw        v6.8h, v2.8h, v4.8b     /* v6.16b = u - 128 */
    uaddw        v8.8h, v2.8h, v5.8b     /* q2 = v - 128 */
    do_store     \bpp, 8
    smull        v20.4s, v6.4h, v1.h[2] /* multiply by -11277 */
    smlal        v20.4s, v8.4h, v1.h[1] /* multiply by -23401 */
    smull2       v22.4s, v6.8h, v1.h[2] /* multiply by -11277 */
    smlal2       v22.4s, v8.8h, v1.h[1] /* multiply by -23401 */
    smull        v24.4s, v8.4h, v1.h[0] /* multiply by 22971 */
    smull2       v26.4s, v8.8h, v1.h[0] /* multiply by 22971 */
    smull        v28.4s, v6.4h, v1.h[3] /* multiply by 29033 */
    smull2       v30.4s, v6.8h, v1.h[3] /* multiply by 29033 */
.endm

.macro do_yuv_to_rgb
    do_yuv_to_rgb_stage1
    do_yuv_to_rgb_stage2
.endm

/* Apple gas crashes on adrl, work around that by using adr.
 * But this requires a copy of these constants for each function.
 */

.balign 16
Ljsimd_ycc_\colorid\()_neon_consts:
    .short			1192,  1192,   1192,   1192, 1192,  1192,   1192,   1192
    .short			1836,  -545,   -218,   2162, 1836,  -545,   -218,   2162
    .short          -128,  -128,   -128,   -128
    .short          -128,  -128,   -128,   -128

asm_function jsimd_ycc_\colorid\()_709_convert_neon
    OUTPUT_WIDTH    .req x0
    INPUT_BUF       .req x1
    OUTPUT_BUF      .req x2

    RGB             .req x5
    RGB2            .req x6
    
    Y               .req x7
    Y2              .req x8
    U               .req x9
    V               .req x10
    N               .req x15

    sub             sp, sp, 336
    str             x15, [sp], 16
    /* Load constants to d1, d2, d3 (v0.4h is just used for padding) */
    adr             x15, Ljsimd_ycc_\colorid\()_neon_consts
    /* Save NEON registers */
    st1             {v0.8b, v1.8b, v2.8b, v3.8b}, [sp], 32
    st1             {v4.8b, v5.8b, v6.8b, v7.8b}, [sp], 32
    st1             {v8.8b, v9.8b, v10.8b, v11.8b}, [sp], 32
    st1             {v12.8b, v13.8b, v14.8b, v15.8b}, [sp], 32
    st1             {v16.8b, v17.8b, v18.8b, v19.8b}, [sp], 32
    st1             {v20.8b, v21.8b, v22.8b, v23.8b}, [sp], 32
    st1             {v24.8b, v25.8b, v26.8b, v27.8b}, [sp], 32
    st1             {v28.8b, v29.8b, v30.8b, v31.8b}, [sp], 32
    ld1             {v0.8h, v1.8h}, [x15], 32
    ld1             {v2.8h}, [x15]

    /* Save ARM registers and handle input arguments */
    /* push            {x4, x5, x6, x7, x8, x9, x10, x30} */
    stp             x4, x5, [sp], 16
    stp             x6, x7, [sp], 16
    stp             x8, x9, [sp], 16
    stp             x10, x30, [sp], 16
    ldr             Y, [INPUT_BUF]
    ldr             U, [INPUT_BUF, 8]
    ldr             V, [INPUT_BUF, 16]
    ldr             Y2, [INPUT_BUF, 24]
    .unreq          INPUT_BUF

    /* Initially set v10, v11.4h, v12.8b, d13 to 0xFF */
    movi            v10.16b, #255
    movi            v13.16b, #255
    movi            v14.16b, #255
    movi            v17.16b, #255

    /* Outer loop over scanlines */
0:
    ldr             Y, [Y]
    ldr             Y2, [Y2]
    ldr             U, [U]
    ldr             V, [V]

    mov             N, OUTPUT_WIDTH

    ldr             RGB, [OUTPUT_BUF], #8
    ldr             RGB2, [OUTPUT_BUF], #8

    /* Inner loop over pixels */
    do_load         8
	
    do_yuv_to_rgb_stage1
    subs            N, N, #8
    b.lt             2f
1:
    do_yuv_to_rgb_stage2_store_load_stage1
    subs            N, N, #8
    b.ge             1b
2:
    do_yuv_to_rgb_stage2
    do_store        \bpp, 8
    tst             N, #7
    b.eq             8f
3:
    tst             N, #4
    b.eq             3f
    do_load         4
3:
    tst             N, #2
    b.eq             4f
    do_load         2
4:
    tst             N, #1
    b.eq             5f
    do_load         1
5:
    do_yuv_to_rgb
    tst             N, #4
    b.eq             6f
    do_store        \bpp, 4
6:
    tst             N, #2
    b.eq             7f
    do_store        \bpp, 2
7:
    tst             N, #1
    b.eq             8f
    do_store        \bpp, 1
8:
    /* Restore all registers and return */
    sub             sp, sp, #336
    ldr             x15, [sp], 16
    ld1             {v0.8b, v1.8b, v2.8b, v3.8b}, [sp], 32
    ld1             {v4.8b, v5.8b, v6.8b, v7.8b}, [sp], 32
    ld1             {v8.8b, v9.8b, v10.8b, v11.8b}, [sp], 32
    ld1             {v12.8b, v13.8b, v14.8b, v15.8b}, [sp], 32
    ld1             {v16.8b, v17.8b, v18.8b, v19.8b}, [sp], 32
    ld1             {v20.8b, v21.8b, v22.8b, v23.8b}, [sp], 32
    ld1             {v24.8b, v25.8b, v26.8b, v27.8b}, [sp], 32
    ld1             {v28.8b, v29.8b, v30.8b, v31.8b}, [sp], 32
    /* pop             {r4, r5, r6, r7, r8, r9, r10, pc} */
    ldp             x4, x5, [sp], 16
    ldp             x6, x7, [sp], 16
    ldp             x8, x9, [sp], 16
    ldp             x10, x30, [sp], 16
    br              x30
    .unreq          OUTPUT_WIDTH
    .unreq          OUTPUT_BUF
    .unreq          RGB
    .unreq          RGB2
    .unreq          Y
    .unreq          Y2
    .unreq          U
    .unreq          V
    .unreq          N

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.purgem do_yuv_to_rgb_stage2_store_load_stage1
.endm

/*--------------------------------- id --------------- bpp R  rsize  G  gsize  B  bsize                     defsize */
generate_jsimd_ycc_rgb_709_convert_neon_nv12 nv12_extrgbx, 32, 0, .4h,   1, .4h,   2, .4h,     4,    5,    6,    .8b   
generate_jsimd_ycc_rgb_709_convert_neon_nv12 nv12_extbgrx, 32, 2, .4h,   1, .4h,   0, .4h,     6,    5,    4,    .8b   
.purgem do_load
.purgem do_store


/*****************************************************************************************/











/*****************************************************************************/

/*
 * jsimd_ycc_nv12_extrgb_709_convert_neon
 * jsimd_ycc_nv12_extbgr_709_convert_neon
 * jsimd_ycc_nv12_extrgbx_709_convert_neon
 * jsimd_ycc_nv12_extbgrx_709_convert_neon
 * jsimd_ycc_nv12_extxbgr_709_convert_neon
 * jsimd_ycc_nv12_extxrgb_709_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    .if \size == 8
        ld1  {v4.b}[0], [U]
        ld1  {v4.b}[1], [U], 1
        ld1  {v5.b}[0], [U]
        ld1  {v5.b}[1], [U], 1
        ld1  {v4.b}[2], [U]
        ld1  {v4.b}[3], [U], 1
        ld1  {v5.b}[2], [U]
        ld1  {v5.b}[3], [U], 1
        ld1  {v4.b}[4], [U]
        ld1  {v4.b}[5], [U], 1
        ld1  {v5.b}[4], [U]
        ld1  {v5.b}[5], [U], 1
        ld1  {v4.b}[6], [U]
        ld1  {v4.b}[7], [U], 1
        ld1  {v5.b}[6], [U]
        ld1  {v5.b}[7], [U], 1
                
        ld1  {v19.8b}, [Y], 8
        prfm pldl1keep, [U, #64]
        prfm pldl1keep, [U, #64]
        prfm pldl1keep, [Y, #64]
    .elseif \size == 4
        ld1  {v4.b}[0], [U]
        ld1  {v4.b}[1], [U], 1
        ld1  {v5.b}[0], [U]
        ld1  {v5.b}[1], [U], 1
        ld1  {v4.b}[2], [U]
        ld1  {v4.b}[3], [U], 1
        ld1  {v5.b}[2], [U]
        ld1  {v5.b}[3], [U], 1
        ld1  {v19.b}[0], [Y], 1
        ld1  {v19.b}[1], [Y], 1
        ld1  {v19.b}[2], [Y], 1
        ld1  {v19.b}[3], [Y], 1
    .elseif \size == 2
        ld1  {v4.b}[4], [U]
        ld1  {v4.b}[5], [U], 1
        ld1  {v5.b}[4], [U]
        ld1  {v5.b}[5], [U], 1
        ld1  {v19.b}[4], [Y], 1
        ld1  {v19.b}[5], [Y], 1
    .elseif \size == 1
        ld1  {v4.b}[6], [U], 1
        ld1  {v5.b}[6], [U], 1
        ld1  {v19.b}[6], [Y], 1
    .else
        .error unsupported macroblock size
    .endif
.endm

.macro do_store bpp, size
    .if \bpp == 24
        .if \size == 8
            st3  {v10.8b, v11.8b, v12.8b}, [RGB], 24
        .elseif \size == 4
            st3  {v10.b, v11.b, v12.b}[0], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[1], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[2], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[3], [RGB], 3
        .elseif \size == 2
            st3  {v10.b, v11.b, v12.b}[4], [RGB], 3
            st3  {v10.b, v11.b, v12.b}[5], [RGB], 3
        .elseif \size == 1
            st3  {v10.b, v11.b, v12.b}[6], [RGB], 3
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp == 32
        .if \size == 8
            st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
        .elseif \size == 4
            st4  {v10.b, v11.b, v12.b, v13.b}[0], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[1], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[2], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[3], [RGB], 4
        .elseif \size == 2
            st4  {v10.b, v11.b, v12.b, v13.b}[4], [RGB], 4
            st4  {v10.b, v11.b, v12.b, v13.b}[5], [RGB], 4
        .elseif \size == 1
            st4  {v10.b, v11.b, v12.b, v13.b}[6], [RGB], 4
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp==16
        .if \size == 8
            st1  {v25.8h}, [RGB],16
        .elseif \size == 4
            st1  {v25.4h}, [RGB],8
        .elseif \size == 2
            st1  {v25.h}[4], [RGB],2
            st1  {v25.h}[5], [RGB],2
        .elseif \size == 1
            st1  {v25.h}[6], [RGB],2
        .else
            .error unsupported macroblock size
        .endif
     .else
        .error unsupported bpp
    .endif
.endm

.macro generate_jsimd_ycc_rgb_709_convert_neon_nv12_single_line colorid, bpp, r_offs, rsize, g_offs, gsize, b_offs, bsize, defsize

/*
 * 2-stage pipelined YCbCr->RGB conversion
 */

.macro do_yuv_to_rgb_stage1
    uaddw        v6.8h, v2.8h, v4.8b     /* q3 = u - 128 */
    uaddw        v8.8h, v2.8h, v5.8b     /* q2 = v - 128 */
    smull        v20.4s, v6.4h, v1.h[2] /* multiply by -833 */	/* green */
    smlal        v20.4s, v8.4h, v1.h[1] /* multiply by -400 */
    smull2       v22.4s, v6.8h, v1.h[2] /* multiply by -833 */	/* green */
    smlal2       v22.4s, v8.8h, v1.h[1] /* multiply by -400 */
    smull        v24.4s, v8.4h, v1.h[0] /* multiply by 1634 */	/* red */
    smull2       v26.4s, v8.8h, v1.h[0] /* multiply by 1634 */
    smull        v28.4s, v6.4h, v1.h[3] /* multiply by 2066 */	/* blue */
    smull2       v30.4s, v6.8h, v1.h[3] /* multiply by 2066 */
.endm

.macro do_yuv_to_rgb_stage2
    rshrn        v20.4h, v20.4s, #10
    rshrn2       v20.8h, v22.4s, #10
    rshrn        v24.4h, v24.4s, #10
    rshrn2       v24.8h, v26.4s, #10
    rshrn        v28.4h, v28.4s, #13
    rshrn2       v28.8h, v30.4s, #13

	movi         v26.8b, #16
	usubl        v26.8h, v19.8b, v26.8b
	smull        v30.4s, v0.4h, v26.4h
	rshrn        v18.4h, v30.4s, #10
	smull2        v30.4s, v0.8h, v26.8h
	rshrn2       v18.8h, v30.4s, #10

    add          v20.8h, v20.8h, v18.8h
    add          v24.8h, v24.8h, v18.8h
    add          v28.8h, v28.8h, v18.8h

.if \bpp != 16
    sqxtun       v1\g_offs\defsize, v20.8h
    sqxtun       v1\r_offs\defsize, v24.8h
    sqxtun       v1\b_offs\defsize, v28.8h
.else
    sqshlu       v21.8h, v20.8h, #8
    sqshlu       v25.8h, v24.8h, #8
    sqshlu       v29.8h, v28.8h, #8
    sri          v25.8h, v21.8h, #5
    sri          v25.8h, v29.8h, #11
.endif

.endm

.macro do_yuv_to_rgb_stage2_store_load_stage1
	rshrn        v20.4h, v20.4s, #10
	rshrn        v24.4h, v24.4s, #10
	rshrn        v28.4h, v28.4s, #10

	ld1          {v4.b}[0], [U]
	ld1          {v4.b}[1], [U], 1
	ld1          {v5.b}[0], [U]
	ld1          {v5.b}[1], [U], 1
	ld1          {v4.b}[2], [U]
	ld1          {v4.b}[3], [U], 1
	ld1          {v5.b}[2], [U]
	ld1          {v5.b}[3], [U], 1
	ld1          {v4.b}[4], [U]
	ld1          {v4.b}[5], [U], 1
	ld1          {v5.b}[4], [U]
	ld1          {v5.b}[5], [U], 1
	ld1          {v4.b}[6], [U]
	ld1          {v4.b}[7], [U], 1
	ld1          {v5.b}[6], [U]
	ld1          {v5.b}[7], [U], 1

	rshrn2       v20.8h, v22.4s, #10
	rshrn2       v24.8h, v26.4s, #10
	rshrn2       v28.8h, v30.4s, #10

	movi         v26.8b, #16
	usubl        v26.8h, v19.8b, v26.8b
	smull        v30.4s, v0.4h, v26.4h
	rshrn        v18.4h, v30.4s, #10
	smull2        v30.4s, v0.8h, v26.8h
	rshrn2       v18.8h, v30.4s, #10

    add          v20.8h, v20.8h, v18.8h
    add          v24.8h, v24.8h, v18.8h
    add          v28.8h, v28.8h, v18.8h


.if \bpp != 16 /**************** rgb24/rgb32 *********************************/
    sqxtun       v1\g_offs\defsize, v20.8h
    ld1          {v19.8b}, [Y], 8
    sqxtun       v1\r_offs\defsize, v24.8h
    prfm         pldl1keep, [U, #64]
    prfm         pldl1keep, [U, #64]
    prfm         pldl1keep, [Y, #64]
    sqxtun       v1\b_offs\defsize, v28.8h
    uaddw        v6.8h, v2.8h, v4.8b     /* v6.16b = u - 128 */
    uaddw        v8.8h, v2.8h, v5.8b     /* q2 = v - 128 */
    smull        v20.4s, v6.4h, v1.h[2] /* multiply by -11277 */
    smlal        v20.4s, v8.4h, v1.h[1] /* multiply by -23401 */
    smull2       v22.4s, v6.8h, v1.h[2] /* multiply by -11277 */
    smlal2       v22.4s, v8.8h, v1.h[1] /* multiply by -23401 */
    smull        v24.4s, v8.4h, v1.h[0] /* multiply by 22971 */
    smull2       v26.4s, v8.8h, v1.h[0] /* multiply by 22971 */
.else /**************************** rgb565 ***********************************/
    sqshlu       v21.8h, v20.8h, #8
    sqshlu       v25.8h, v24.8h, #8
    sqshlu       v29.8h, v28.8h, #8
    uaddw        v6.8h, v2.8h, v4.8b     /* v6.16b = u - 128 */
    uaddw        v8.8h, v2.8h, v5.8b     /* q2 = v - 128 */
    ld1          {v0.8b}, [Y], 8
    smull        v20.4s, v6.4h, v1.h[1] /* multiply by -11277 */
    smlal        v20.4s, v8.4h, v1.h[2] /* multiply by -23401 */
    smull2       v22.4s, v6.8h, v1.h[1] /* multiply by -11277 */
    smlal2       v22.4s, v8.8h, v1.h[2] /* multiply by -23401 */
    sri          v25.8h, v21.8h, #5
    smull        v24.4s, v8.4h, v1.h[0] /* multiply by 22971 */
    smull2       v26.4s, v8.8h, v1.h[0] /* multiply by 22971 */
    prfm         pldl1keep, [U, #64]
    prfm         pldl1keep, [V, #64]
    prfm         pldl1keep, [Y, #64]
    sri          v25.8h, v29.8h, #11
.endif
    do_store     \bpp, 8
    smull        v28.4s, v6.4h, v1.h[3] /* multiply by 29033 */
    smull2       v30.4s, v6.8h, v1.h[3] /* multiply by 29033 */
.endm

.macro do_yuv_to_rgb
    do_yuv_to_rgb_stage1
    do_yuv_to_rgb_stage2
.endm

/* Apple gas crashes on adrl, work around that by using adr.
 * But this requires a copy of these constants for each function.
 */

.balign 16
Ljsimd_ycc_\colorid\()_neon_consts:
    .short			1192,  1192,   1192,   1192, 1192,  1192,   1192,   1192
    .short			1836,  -545,   -218,   2162, 1836,  -545,   -218,   2162
    .short          -128,  -128,   -128,   -128
    .short          -128,  -128,   -128,   -128

asm_function jsimd_ycc_\colorid\()_709_convert_neon
    OUTPUT_WIDTH    .req x0
    INPUT_BUF       .req x1
    OUTPUT_BUF      .req x2

    RGB             .req x5
    Y               .req x7
    U               .req x9
    V               .req x10
    N               .req x15

    sub             sp, sp, 336
    str             x15, [sp], 16
    /* Load constants to d1, d2, d3 (v0.4h is just used for padding) */
    adr             x15, Ljsimd_ycc_\colorid\()_neon_consts
    /* Save NEON registers */
    st1             {v0.8b, v1.8b, v2.8b, v3.8b}, [sp], 32
    st1             {v4.8b, v5.8b, v6.8b, v7.8b}, [sp], 32
    st1             {v8.8b, v9.8b, v10.8b, v11.8b}, [sp], 32
    st1             {v12.8b, v13.8b, v14.8b, v15.8b}, [sp], 32
    st1             {v16.8b, v17.8b, v18.8b, v19.8b}, [sp], 32
    st1             {v20.8b, v21.8b, v22.8b, v23.8b}, [sp], 32
    st1             {v24.8b, v25.8b, v26.8b, v27.8b}, [sp], 32
    st1             {v28.8b, v29.8b, v30.8b, v31.8b}, [sp], 32
    ld1             {v0.8h, v1.8h}, [x15], 32
    ld1             {v2.8h}, [x15]

    /* Save ARM registers and handle input arguments */
    /* push            {x4, x5, x6, x7, x8, x9, x10, x30} */
    stp             x4, x5, [sp], 16
    stp             x6, x7, [sp], 16
    stp             x8, x9, [sp], 16
    stp             x10, x30, [sp], 16
    ldr             Y, [INPUT_BUF]
    ldr             U, [INPUT_BUF, 8]
    ldr             V, [INPUT_BUF, 16]
    .unreq          INPUT_BUF

    /* Initially set v10, v11.4h, v12.8b, d13 to 0xFF */
    movi            v10.16b, #255
    movi            v13.16b, #255

    /* Outer loop over scanlines */
0:
    ldr             Y, [Y, x16]
    ldr             U, [U, x16]
    ldr             V, [V, x16]
    ldr             RGB, [OUTPUT_BUF], #8

    /* Inner loop over pixels */
    subs            N, N, #8
    b.lt             3f
    do_load         8
	
    do_yuv_to_rgb_stage1
    subs            N, N, #8
    b.lt             2f
1:
    do_yuv_to_rgb_stage2_store_load_stage1
    subs            N, N, #8
    b.ge             1b
2:
    do_yuv_to_rgb_stage2
    do_store        \bpp, 8
    tst             N, #7
    b.eq             8f
3:
    tst             N, #4
    b.eq             3f
    do_load         4
3:
    tst             N, #2
    b.eq             4f
    do_load         2
4:
    tst             N, #1
    b.eq             5f
    do_load         1
5:
    do_yuv_to_rgb
    tst             N, #4
    b.eq             6f
    do_store        \bpp, 4
6:
    tst             N, #2
    b.eq             7f
    do_store        \bpp, 2
7:
    tst             N, #1
    b.eq             8f
    do_store        \bpp, 1
8:
    /* Restore all registers and return */
    sub             sp, sp, #336
    ldr             x15, [sp], 16
    ld1             {v0.8b, v1.8b, v2.8b, v3.8b}, [sp], 32
    ld1             {v4.8b, v5.8b, v6.8b, v7.8b}, [sp], 32
    ld1             {v8.8b, v9.8b, v10.8b, v11.8b}, [sp], 32
    ld1             {v12.8b, v13.8b, v14.8b, v15.8b}, [sp], 32
    ld1             {v16.8b, v17.8b, v18.8b, v19.8b}, [sp], 32
    ld1             {v20.8b, v21.8b, v22.8b, v23.8b}, [sp], 32
    ld1             {v24.8b, v25.8b, v26.8b, v27.8b}, [sp], 32
    ld1             {v28.8b, v29.8b, v30.8b, v31.8b}, [sp], 32
    /* pop             {r4, r5, r6, r7, r8, r9, r10, pc} */
    ldp             x4, x5, [sp], 16
    ldp             x6, x7, [sp], 16
    ldp             x8, x9, [sp], 16
    ldp             x10, x30, [sp], 16
    br              x30
    .unreq          OUTPUT_WIDTH
    .unreq          OUTPUT_BUF
    .unreq          RGB
    .unreq          Y
    .unreq          U
    .unreq          V
    .unreq          N

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.purgem do_yuv_to_rgb_stage2_store_load_stage1
.endm

/*--------------------------------- id ----- bpp R  rsize  G  gsize  B  bsize  defsize   */
generate_jsimd_ycc_rgb_709_convert_neon_nv12_single_line nv12_extrgb_single_line,  24, 0, .4h,   1, .4h,   2, .4h,   .8b
generate_jsimd_ycc_rgb_709_convert_neon_nv12_single_line nv12_extrgbx_single_line, 32, 0, .4h,   1, .4h,   2, .4h,   .8b
generate_jsimd_ycc_rgb_709_convert_neon_nv12_single_line nv12_extbgrx_single_line, 32, 2, .4h,   1, .4h,   0, .4h,   .8b
.purgem do_load
.purgem do_store


/*****************************************************************************************/
