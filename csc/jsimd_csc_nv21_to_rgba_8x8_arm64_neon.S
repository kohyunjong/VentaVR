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
 * jsimd_ycc_nv21_extrgb_180_8x8_convert_neon
 * jsimd_ycc_nv21_extbgr_180_8x8_convert_neon
 * jsimd_ycc_nv21_extrgbx_180_8x8_convert_neon
 * jsimd_ycc_nv21_extbgrx_180_8x8_convert_neon
 * jsimd_ycc_nv21_extxbgr_180_8x8_convert_neon
 * jsimd_ycc_nv21_extxrgb_180_8x8_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    ld1  {v5.b}[0], [UV]
    ld1  {v5.b}[1], [UV], 1
    ld1  {v4.b}[0], [UV]
    ld1  {v4.b}[1], [UV], 1
    ld1  {v5.b}[2], [UV]
    ld1  {v5.b}[3], [UV], 1
    ld1  {v4.b}[2], [UV]
    ld1  {v4.b}[3], [UV], 1
    ld1  {v5.b}[4], [UV]
    ld1  {v5.b}[5], [UV], 1
    ld1  {v4.b}[4], [UV]
    ld1  {v4.b}[5], [UV], 1
    ld1  {v5.b}[6], [UV]
    ld1  {v5.b}[7], [UV], 1
    ld1  {v4.b}[6], [UV]
    ld1  {v4.b}[7], [UV], 1

    ld1  {v19.8b}, [Y], 8
    ld1  {v18.8b}, [Y2], 8
    prfm pldl1keep, [UV, #64]
    prfm pldl1keep, [Y, #64]
	prfm pldl1keep, [Y2, #64]
.endm

.macro do_store bpp, size
    st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB2], 32
.endm

.macro generate_jsimd_ycc_rgb_180_8x8_convert_neon_nv21 colorid, bpp, r_offs, rsize, g_offs, gsize, b_offs, bsize, r_offs2, g_offs2, b_offs2, defsize

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

    sqxtun       v1\g_offs\defsize, v20.8h
	sqxtun       v1\g_offs2\defsize, v22.8h
    sqxtun       v1\r_offs\defsize, v24.8h
	sqxtun       v1\r_offs2\defsize, v26.8h
    sqxtun       v1\b_offs\defsize, v28.8h
	sqxtun       v1\b_offs2\defsize, v30.8h

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
    .short			1634,  -833,   -400,   2066, 1634,  -833,   -400,   2066
    .short          -128,  -128,   -128,   -128
    .short          -128,  -128,   -128,   -128

asm_function jsimd_ycc_\colorid\()_180_8x8_convert_neon
    INPUT_WIDTH     .req x0
    INPUT_BUF       .req x1
    OUTPUT_BUF      .req x2
	ODD             .req x3

    RGB             .req x5
    RGB2            .req x6

    Y_N             .req x10
    UV_N            .req x4
    RGB_N           .req x11

    Y               .req x7
    Y2              .req x8
    UV              .req x9

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
    ldr             UV, [INPUT_BUF, 8]
    ldr             Y2, [INPUT_BUF, 24]
    //.unreq          INPUT_BUF

    /* Initially set v10, v11.4h, v12.8b, d13 to 0xFF */
    movi            v13.16b, #255

    /* Outer loop over scanlines */
	ldr             Y, [Y]
	ldr             UV, [UV]
	ldr             Y2, [Y2]

    mov             N, INPUT_WIDTH

    ldr             RGB, [OUTPUT_BUF]//, #8
    ldr             RGB2, [OUTPUT_BUF, 8]//, #8

	add             Y_N, N, N
	add             RGB_N, Y_N, Y_N
	add             RGB_N, RGB_N, RGB_N

    sub             Y_N, Y_N, #8
    sub             UV_N, N, #8

	mov             N, ODD
	add             UV_N, UV_N, N

    add             RGB_N, RGB_N, #32
	
    sub             INPUT_WIDTH, INPUT_WIDTH, #7
	mov             N, #0

0:

    do_load         8
	
    do_yuv_to_rgb_stage1

    do_yuv_to_rgb_stage2

    //do_store        \bpp, 8
	
	// R_1
	rev16           v10.8b, v10.8b
	rev32           v10.4h, v10.4h
	rev64           v10.2s, v10.2s

	// G_1
	rev16           v11.8b, v11.8b
	rev32           v11.4h, v11.4h
	rev64           v11.2s, v11.2s

	// B_1
	rev16           v12.8b, v12.8b
	rev32           v12.4h, v12.4h
	rev64           v12.2s, v12.2s

	// R_2
	rev16           v14.8b, v14.8b
	rev32           v14.4h, v14.4h
	rev64           v14.2s, v14.2s
	
	// G_2
	rev16           v15.8b, v15.8b
	rev32           v15.4h, v15.4h
	rev64           v15.2s, v15.2s

	// B_2
	rev16           v16.8b, v16.8b
	rev32           v16.4h, v16.4h
	rev64           v16.2s, v16.2s
    
	st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB], 32
	

	// 2
	add Y, Y, Y_N
	add UV, UV, UV_N
	add Y2, Y2, Y_N

	sub RGB, RGB, RGB_N
	sub RGB2, RGB2, RGB_N

	do_load         8
    do_yuv_to_rgb_stage1

    do_yuv_to_rgb_stage2

    //do_store        \bpp, 8
	// R_1
	rev16           v10.8b, v10.8b
	rev32           v10.4h, v10.4h
	rev64           v10.2s, v10.2s

	// G_1
	rev16           v11.8b, v11.8b
	rev32           v11.4h, v11.4h
	rev64           v11.2s, v11.2s

	// B_1
	rev16           v12.8b, v12.8b
	rev32           v12.4h, v12.4h
	rev64           v12.2s, v12.2s

	// R_2
	rev16           v14.8b, v14.8b
	rev32           v14.4h, v14.4h
	rev64           v14.2s, v14.2s
	
	// G_2
	rev16           v15.8b, v15.8b
	rev32           v15.4h, v15.4h
	rev64           v15.2s, v15.2s

	// B_2
	rev16           v16.8b, v16.8b
	rev32           v16.4h, v16.4h
	rev64           v16.2s, v16.2s
    
	st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB], 32


	// 3
	add Y, Y, Y_N
	add UV, UV, UV_N
	add Y2, Y2, Y_N

	sub RGB, RGB, RGB_N
	sub RGB2, RGB2, RGB_N

	do_load         8
    do_yuv_to_rgb_stage1

    do_yuv_to_rgb_stage2

    //do_store        \bpp, 8
	// R_1
	rev16           v10.8b, v10.8b
	rev32           v10.4h, v10.4h
	rev64           v10.2s, v10.2s

	// G_1
	rev16           v11.8b, v11.8b
	rev32           v11.4h, v11.4h
	rev64           v11.2s, v11.2s

	// B_1
	rev16           v12.8b, v12.8b
	rev32           v12.4h, v12.4h
	rev64           v12.2s, v12.2s

	// R_2
	rev16           v14.8b, v14.8b
	rev32           v14.4h, v14.4h
	rev64           v14.2s, v14.2s
	
	// G_2
	rev16           v15.8b, v15.8b
	rev32           v15.4h, v15.4h
	rev64           v15.2s, v15.2s

	// B_2
	rev16           v16.8b, v16.8b
	rev32           v16.4h, v16.4h
	rev64           v16.2s, v16.2s
    
	st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB], 32

	// 4
	add Y, Y, Y_N
	add UV, UV, UV_N
	add Y2, Y2, Y_N

	sub RGB, RGB, RGB_N
	sub RGB2, RGB2, RGB_N

	do_load         8
    do_yuv_to_rgb_stage1

    do_yuv_to_rgb_stage2

    //do_store        \bpp, 8
	// R_1
	rev16           v10.8b, v10.8b
	rev32           v10.4h, v10.4h
	rev64           v10.2s, v10.2s

	// G_1
	rev16           v11.8b, v11.8b
	rev32           v11.4h, v11.4h
	rev64           v11.2s, v11.2s

	// B_1
	rev16           v12.8b, v12.8b
	rev32           v12.4h, v12.4h
	rev64           v12.2s, v12.2s

	// R_2
	rev16           v14.8b, v14.8b
	rev32           v14.4h, v14.4h
	rev64           v14.2s, v14.2s
	
	// G_2
	rev16           v15.8b, v15.8b
	rev32           v15.4h, v15.4h
	rev64           v15.2s, v15.2s

	// B_2
	rev16           v16.8b, v16.8b
	rev32           v16.4h, v16.4h
	rev64           v16.2s, v16.2s
    
	st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB], 32






	// landscape full
	add             N, N, #8

    ldr             Y, [INPUT_BUF]
    ldr             UV, [INPUT_BUF, 8]
    ldr             Y2, [INPUT_BUF, 24]

	ldr             Y, [Y]
	ldr             UV, [UV]
	ldr             Y2, [Y2]

    ldr             RGB, [OUTPUT_BUF]//, #8
    ldr             RGB2, [OUTPUT_BUF, 8]//, #8

	add             Y, Y, N
	add             UV, UV, N
	add             Y2, Y2, N

	sub             RGB, RGB, N
	sub             RGB2, RGB2, N

	sub             RGB, RGB, N
	sub             RGB2, RGB2, N

	sub             RGB, RGB, N
	sub             RGB2, RGB2, N

	sub             RGB, RGB, N
	sub             RGB2, RGB2, N

	cmp             N, INPUT_WIDTH
    b.lt            0b
	
	



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

	.unreq          INPUT_BUF
    .unreq          INPUT_WIDTH
    .unreq          OUTPUT_BUF
	.unreq          ODD
    .unreq          RGB
    .unreq          RGB2
	.unreq          Y_N
	.unreq          UV_N
	.unreq          RGB_N
    .unreq          Y
    .unreq          Y2
    .unreq          UV
    .unreq          N

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.endm

/*--------------------------------- id ---------------     bpp R  rsize  G  gsize  B  bsize                     defsize */
generate_jsimd_ycc_rgb_180_8x8_convert_neon_nv21 nv21_extrgbx, 32, 0, .4h,   1, .4h,   2, .4h,     4,    5,    6,    .8b   
generate_jsimd_ycc_rgb_180_8x8_convert_neon_nv21 nv21_extbgrx, 32, 2, .4h,   1, .4h,   0, .4h,     6,    5,    4,    .8b   
.purgem do_load
.purgem do_store


/*****************************************************************************************/














/*****************************************************************************/

/*
 * jsimd_ycc_nv21_extrgb_270_8x8_convert_neon
 * jsimd_ycc_nv21_extbgr_270_8x8_convert_neon
 * jsimd_ycc_nv21_extrgbx_270_8x8_convert_neon
 * jsimd_ycc_nv21_extbgrx_270_8x8_convert_neon
 * jsimd_ycc_nv21_extxbgr_270_8x8_convert_neon
 * jsimd_ycc_nv21_extxrgb_270_8x8_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    ld1  {v5.b}[0], [UV]
    ld1  {v5.b}[1], [UV], 1
    ld1  {v4.b}[0], [UV]
    ld1  {v4.b}[1], [UV], 1
    ld1  {v5.b}[2], [UV]
    ld1  {v5.b}[3], [UV], 1
    ld1  {v4.b}[2], [UV]
    ld1  {v4.b}[3], [UV], 1
    ld1  {v5.b}[4], [UV]
    ld1  {v5.b}[5], [UV], 1
    ld1  {v4.b}[4], [UV]
    ld1  {v4.b}[5], [UV], 1
    ld1  {v5.b}[6], [UV]
    ld1  {v5.b}[7], [UV], 1
    ld1  {v4.b}[6], [UV]
    ld1  {v4.b}[7], [UV], 1

    ld1  {v19.8b}, [Y], 8
    ld1  {v18.8b}, [Y2], 8
    prfm pldl1keep, [UV, #64]
    prfm pldl1keep, [Y, #64]
	prfm pldl1keep, [Y2, #64]
.endm

.macro do_store bpp, size
    st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB2], 32
.endm

.macro generate_jsimd_ycc_rgb_270_8x8_convert_neon_nv21 colorid, bpp, r_offs, rsize, g_offs, gsize, b_offs, bsize, r_offs2, g_offs2, b_offs2, defsize

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

    sqxtun       v1\g_offs\defsize, v20.8h
	sqxtun       v1\g_offs2\defsize, v22.8h
    sqxtun       v1\r_offs\defsize, v24.8h
	sqxtun       v1\r_offs2\defsize, v26.8h
    sqxtun       v1\b_offs\defsize, v28.8h
	sqxtun       v1\b_offs2\defsize, v30.8h

.endm

.macro do_yuv_to_rgb
    do_yuv_to_rgb_stage1
    do_yuv_to_rgb_stage2
.endm

/* Apple gas crashes on adrl, work around that by using adr.
 * But this requires a copy of these constants for each function.
 */

asm_function jsimd_ycc_\colorid\()_270_8x8_convert_neon
    INPUT_WIDTH     .req x0
    INPUT_BUF       .req x1
    OUTPUT_WIDTH    .req x2
    OUTPUT_BUF      .req x3
    
    RGB             .req x5
    RGB2            .req x6

    Y_N             .req x10
    UV_N            .req x4
    RGB_N           .req x11
	RGB_N2          .req x12
	RGB_N3          .req x13

    Y               .req x7
    Y2              .req x8
    UV              .req x9
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
    ldr             UV, [INPUT_BUF, 8]
    ldr             Y2, [INPUT_BUF, 24]
    //.unreq          INPUT_BUF

    /* Outer loop over scanlines */
	ldr             Y, [Y]
	ldr             UV, [UV]
	ldr             Y2, [Y2]

    mov             N, INPUT_WIDTH

    //ldr             RGB, [OUTPUT_BUF], #8
    //ldr             RGB2, [OUTPUT_BUF], #8
    ldr             RGB, [OUTPUT_BUF]//, #8
    ldr             RGB2, [OUTPUT_BUF, 8]//, #8

	add             Y_N, N, N
    sub             Y_N, Y_N, #8
    sub             UV_N, N, #8

    and             N, INPUT_WIDTH, #1
	add             UV_N, UV_N, N

    mov             N, OUTPUT_WIDTH
    add             N, N, N

    add             RGB_N, N, N
    add             RGB_N, RGB_N, RGB_N
    sub             RGB_N, RGB_N, #32

	sub             INPUT_WIDTH, INPUT_WIDTH, #7
	mov             N, #0

	mov             RGB_N2, OUTPUT_WIDTH
	add             RGB_N2, RGB_N2, RGB_N2
	add             RGB_N2, RGB_N2, RGB_N2 // 1


0:

    do_load         8

    do_yuv_to_rgb_stage1

    //do_yuv_to_rgb_stage2
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

    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v14.16b, v6.16b, v8.16b

    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v31.16b, v6.16b, v8.16b

    sqxtun      v6.8b, v20.8h // g_1
    sqxtun      v8.8b, v22.8h // g_2
    zip1         v23.16b, v6.16b, v8.16b

    //do_store        \bpp, 8

    // 2
    add Y, Y, Y_N
    add UV, UV, UV_N
    add Y2, Y2, Y_N

    do_load         8
    do_yuv_to_rgb_stage1

    //do_yuv_to_rgb_stage2
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


    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v15.16b, v6.16b, v8.16b
	
    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v7.16b, v6.16b, v8.16b
	
    sqxtun       v6.8b, v20.8h // g_1
    sqxtun       v8.8b, v22.8h // g_2
    zip1         v25.16b, v6.16b, v8.16b

    //do_store        \bpp, 8
 
    // 3
    add Y, Y, Y_N
    add UV, UV, UV_N
    add Y2, Y2, Y_N

    do_load         8
    do_yuv_to_rgb_stage1

    //do_yuv_to_rgb_stage2
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

    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v16.16b, v6.16b, v8.16b

    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v9.16b, v6.16b, v8.16b

    sqxtun       v6.8b, v20.8h // g_1
    sqxtun       v8.8b, v22.8h // g_2
    zip1         v27.16b, v6.16b, v8.16b

    //do_store        \bpp, 8
 

    // 4
    add Y, Y, Y_N
    add UV, UV, UV_N
    add Y2, Y2, Y_N

    do_load         8
    do_yuv_to_rgb_stage1

    //do_yuv_to_rgb_stage2
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

    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v17.16b, v6.16b, v8.16b

    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v21.16b, v6.16b, v8.16b

    sqxtun       v6.8b, v20.8h // g_1
    sqxtun       v8.8b, v22.8h // g_2
    zip1         v29.16b, v6.16b, v8.16b

    //do_store        \bpp, 8

    // transpose

    // v14, v15, v16, v17 - R
    //1
    zip1            v18.8h,       v14.8h, v15.8h
    zip2            v20.8h,       v14.8h, v15.8h
    zip1            v24.8h,       v16.8h, v17.8h
    zip2            v28.8h,       v16.8h, v17.8h

    //2				          
    zip1            v14.4s,       v18.4s, v20.4s
    zip2            v15.4s,       v18.4s, v20.4s
    zip1            v16.4s,       v24.4s, v28.4s
    zip2            v17.4s,       v24.4s, v28.4s

    //3				          
    zip1            v18.2d,       v14.2d, v16.2d
    zip2            v20.2d,       v14.2d, v16.2d
    zip1            v24.2d,       v15.2d, v17.2d
    zip2            v28.2d,       v15.2d, v17.2d

    //4
    mov             v14.2d, v18.2d
    mov             v15.2d, v20.2d
    mov             v16.2d, v24.2d
    mov             v17.2d, v28.2d

    ins             v13.s[0], v14.s[1]
    ins             v14.s[1], v14.s[2]
    ins             v14.s[2], v13.s[0]

    ins             v13.s[0], v15.s[1]
    ins             v15.s[1], v15.s[2]
    ins             v15.s[2], v13.s[0]

    ins             v13.s[0], v16.s[1]
    ins             v16.s[1], v16.s[2]
    ins             v16.s[2], v13.s[0]

    ins             v13.s[0], v17.s[1]
    ins             v17.s[1], v17.s[2]
    ins             v17.s[2], v13.s[0]

    // 
    // v31, v7, v9, v21 - G
    //1
    zip1            v18.8h,       v31.8h, v7.8h
    zip2            v20.8h,       v31.8h, v7.8h
    zip1            v24.8h,       v9.8h, v21.8h
    zip2            v28.8h,       v9.8h, v21.8h

    //2				          
    zip1            v31.4s,       v18.4s, v20.4s
    zip2            v7.4s,       v18.4s, v20.4s
    zip1            v9.4s,       v24.4s, v28.4s
    zip2            v21.4s,       v24.4s, v28.4s

    //3				          
    zip1            v18.2d,       v31.2d, v9.2d
    zip2            v20.2d,       v31.2d, v9.2d
    zip1            v24.2d,       v7.2d, v21.2d
    zip2            v28.2d,       v7.2d, v21.2d

    //4
    mov             v31.2d, v18.2d
    mov             v7.2d, v20.2d
    mov             v9.2d, v24.2d
    mov             v21.2d, v28.2d

    ins             v13.s[0], v31.s[1]
    ins             v31.s[1], v31.s[2]
    ins             v31.s[2], v13.s[0]

    ins             v13.s[0], v7.s[1]
    ins             v7.s[1], v7.s[2]
    ins             v7.s[2], v13.s[0]

    ins             v13.s[0], v9.s[1]
    ins             v9.s[1], v9.s[2]
    ins             v9.s[2], v13.s[0]

    ins             v13.s[0], v21.s[1]
    ins             v21.s[1], v21.s[2]
    ins             v21.s[2], v13.s[0]

    // v23, v25, v27, v29 - R
    //1
    zip1            v18.8h,       v23.8h, v25.8h
    zip2            v20.8h,       v23.8h, v25.8h
    zip1            v24.8h,       v27.8h, v29.8h
    zip2            v28.8h,       v27.8h, v29.8h

    //2				          
    zip1            v23.4s,       v18.4s, v20.4s
    zip2            v25.4s,       v18.4s, v20.4s
    zip1            v27.4s,       v24.4s, v28.4s
    zip2            v29.4s,       v24.4s, v28.4s

    //3				          
    zip1            v18.2d,       v23.2d, v27.2d
    zip2            v20.2d,       v23.2d, v27.2d
    zip1            v24.2d,       v25.2d, v29.2d
    zip2            v28.2d,       v25.2d, v29.2d

    //4
    mov             v23.2d, v18.2d
    mov             v25.2d, v20.2d
    mov             v27.2d, v24.2d
    mov             v29.2d, v28.2d

    ins             v13.s[0], v23.s[1]
    ins             v23.s[1], v23.s[2]
    ins             v23.s[2], v13.s[0]

    ins             v13.s[0], v25.s[1]
    ins             v25.s[1], v25.s[2]
    ins             v25.s[2], v13.s[0]

    ins             v13.s[0], v27.s[1]
    ins             v27.s[1], v27.s[2]
    ins             v27.s[2], v13.s[0]

    ins             v13.s[0], v29.s[1]
    ins             v29.s[1], v29.s[2]
    ins             v29.s[2], v13.s[0]

    // 1
    mov             v10.d[0], v17.d[1] // r
    mov				v11.d[0], v29.d[1] // g
    mov				v12.d[0], v21.d[1] // b
    movi            v13.16b, #255      // a

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32

    // 2
    mov             v10.d[0], v16.d[1] // r
    mov				v11.d[0], v27.d[1] // g
    mov				v12.d[0], v9.d[1] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N

    // 3
    mov             v10.d[0], v15.d[1] // r
    mov				v11.d[0], v25.d[1] // g
    mov				v12.d[0], v7.d[1] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32

    // 4
    mov             v10.d[0], v14.d[1] // r
    mov				v11.d[0], v23.d[1] // g
    mov				v12.d[0], v31.d[1] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32


    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N

    // 5
    mov             v10.d[0], v17.d[0] // r
    mov				v11.d[0], v29.d[0] // g
    mov				v12.d[0], v21.d[0] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32

    // 6
    mov             v10.d[0], v16.d[0] // r
    mov				v11.d[0], v27.d[0] // g
    mov				v12.d[0], v9.d[0] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
 	
    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N

    // 7
    mov             v10.d[0], v15.d[0] // r
    mov				v11.d[0], v25.d[0] // g
    mov				v12.d[0], v7.d[0] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32

    // 8
    mov             v10.d[0], v14.d[0] // r
    mov				v11.d[0], v23.d[0] // g
    mov				v12.d[0], v31.d[0] // b

    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32


	// landscape full
	add             N, N, #8

    ldr             Y, [INPUT_BUF]
    ldr             UV, [INPUT_BUF, 8]
    ldr             Y2, [INPUT_BUF, 24]

	ldr             Y, [Y]
	ldr             UV, [UV]
	ldr             Y2, [Y2]

    ldr             RGB, [OUTPUT_BUF]//, #8
    ldr             RGB2, [OUTPUT_BUF, 8]//, #8

	add             Y, Y, N
	add             UV, UV, N
	add             Y2, Y2, N

	mul             RGB_N3, RGB_N2, N

	sub             RGB, RGB, RGB_N3
	sub             RGB2, RGB2, RGB_N3

	cmp             N, INPUT_WIDTH
    b.lt            0b


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

    .unreq          INPUT_WIDTH
	.unreq          INPUT_BUF
    .unreq          OUTPUT_WIDTH
    .unreq          OUTPUT_BUF
    .unreq          RGB
    .unreq          RGB2
    .unreq          Y_N
    .unreq          UV_N
    .unreq          RGB_N
	.unreq          RGB_N2
	.unreq          RGB_N3
    .unreq          Y
    .unreq          Y2
    .unreq          UV
    .unreq          N

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.endm

/*--------------------------------- id ---------------     bpp R  rsize  G  gsize  B  bsize                     defsize */
generate_jsimd_ycc_rgb_270_8x8_convert_neon_nv21 nv21_extrgbx, 32, 0, .4h,   1, .4h,   2, .4h,     4,    5,    6,    .8b   
generate_jsimd_ycc_rgb_270_8x8_convert_neon_nv21 nv21_extbgrx, 32, 2, .4h,   1, .4h,   0, .4h,     6,    5,    4,    .8b   
.purgem do_load
.purgem do_store


/*****************************************************************************************/



















/*****************************************************************************/

/*
 * jsimd_ycc_nv21_extrgb_90_8x8_convert_neon
 * jsimd_ycc_nv21_extbgr_90_8x8_convert_neon
 * jsimd_ycc_nv21_extrgbx_90_8x8_convert_neon
 * jsimd_ycc_nv21_extbgrx_90_8x8_convert_neon
 * jsimd_ycc_nv21_extxbgr_90_8x8_convert_neon
 * jsimd_ycc_nv21_extxrgb_90_8x8_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    ld1  {v5.b}[0], [UV]
    ld1  {v5.b}[1], [UV], 1
    ld1  {v4.b}[0], [UV]
    ld1  {v4.b}[1], [UV], 1
    ld1  {v5.b}[2], [UV]
    ld1  {v5.b}[3], [UV], 1
    ld1  {v4.b}[2], [UV]
    ld1  {v4.b}[3], [UV], 1
    ld1  {v5.b}[4], [UV]
    ld1  {v5.b}[5], [UV], 1
    ld1  {v4.b}[4], [UV]
    ld1  {v4.b}[5], [UV], 1
    ld1  {v5.b}[6], [UV]
    ld1  {v5.b}[7], [UV], 1
    ld1  {v4.b}[6], [UV]
    ld1  {v4.b}[7], [UV], 1

    ld1  {v19.8b}, [Y], 8
    ld1  {v18.8b}, [Y2], 8
    prfm pldl1keep, [UV, #64]
    prfm pldl1keep, [Y, #64]
	prfm pldl1keep, [Y2, #64]
.endm

.macro do_store bpp, size
    st4  {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
    st4  {v14.8b, v15.8b, v16.8b, v17.8b}, [RGB2], 32
.endm

.macro generate_jsimd_ycc_rgb_90_8x8_convert_neon_nv21 colorid, bpp, r_offs, rsize, g_offs, gsize, b_offs, bsize, r_offs2, g_offs2, b_offs2, defsize

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

    sqxtun       v1\g_offs\defsize, v20.8h
	sqxtun       v1\g_offs2\defsize, v22.8h
    sqxtun       v1\r_offs\defsize, v24.8h
	sqxtun       v1\r_offs2\defsize, v26.8h
    sqxtun       v1\b_offs\defsize, v28.8h
	sqxtun       v1\b_offs2\defsize, v30.8h

.endm

.macro do_yuv_to_rgb
    do_yuv_to_rgb_stage1
    do_yuv_to_rgb_stage2
.endm

/* Apple gas crashes on adrl, work around that by using adr.
 * But this requires a copy of these constants for each function.
 */

asm_function jsimd_ycc_\colorid\()_90_8x8_convert_neon
    INPUT_WIDTH     .req x0
    INPUT_BUF       .req x1
	OUTPUT_WIDTH    .req x2
    OUTPUT_BUF      .req x3
    
    RGB             .req x5
    RGB2            .req x6

    Y_N             .req x10
    UV_N            .req x4
    RGB_N           .req x11

    Y               .req x7
    Y2              .req x8
    UV              .req x9
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
    ldr             UV, [INPUT_BUF, 8]
    ldr             Y2, [INPUT_BUF, 24]
    //.unreq          INPUT_BUF

    /* Initially set v10, v11.4h, v12.8b, d13 to 0xFF */

    /* Outer loop over scanlines */
	ldr             Y, [Y]
	ldr             UV, [UV]
	ldr             Y2, [Y2]

    mov             N, INPUT_WIDTH

    ldr             RGB, [OUTPUT_BUF], #8
    ldr             RGB2, [OUTPUT_BUF], #8

	add             Y_N, N, N

    sub             Y_N, Y_N, #8
    sub             UV_N, N, #8

    and             N, INPUT_WIDTH, #1
	add             UV_N, UV_N, N
	
	mov             N, OUTPUT_WIDTH
	add             N, N, N

	add             RGB_N, N, N
	add             RGB_N, RGB_N, RGB_N

    sub             RGB_N, RGB_N, #32
/*
    ld1  {v31.8b}, [Y], 8
	ld1  {v7.8b}, [Y], 8
	ld1  {v9.8b}, [Y], 8
	ld1  {v21.8b}, [Y], 8
	ld1  {v23.8b}, [Y], 8
	ld1  {v25.8b}, [Y], 8
	ld1  {v27.8b}, [Y], 8
	ld1  {v29.8b}, [Y], 8

	zip1         v14.16b, v7.16b, v31.16b
	zip1         v15.16b, v21.16b, v9.16b
	zip1         v16.16b, v25.16b, v23.16b
	zip1         v17.16b, v29.16b, v27.16b
*/

    sub             INPUT_WIDTH, INPUT_WIDTH, #7
	mov             N, #0

0:

    do_load         8
 
    do_yuv_to_rgb_stage1
 
    //do_yuv_to_rgb_stage2
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
 
    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v14.16b, v8.16b, v6.16b
 
    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v31.16b, v8.16b, v6.16b
 
    sqxtun      v6.8b, v20.8h // g_1
    sqxtun      v8.8b, v22.8h // g_2
    zip1         v23.16b, v8.16b, v6.16b
 
    //do_store        \bpp, 8
 
    // 2
    add Y, Y, Y_N
    add UV, UV, UV_N
    add Y2, Y2, Y_N
 
    do_load         8
    do_yuv_to_rgb_stage1
 
    //do_yuv_to_rgb_stage2
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
 
 
    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v15.16b, v8.16b, v6.16b

    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v7.16b, v8.16b, v6.16b

    sqxtun       v6.8b, v20.8h // g_1
    sqxtun       v8.8b, v22.8h // g_2
    zip1         v25.16b, v8.16b, v6.16b
 
    //do_store        \bpp, 8
 
    // 3
    add Y, Y, Y_N
    add UV, UV, UV_N
    add Y2, Y2, Y_N
 
    do_load         8
    do_yuv_to_rgb_stage1
 
    //do_yuv_to_rgb_stage2
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
 
    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v16.16b, v8.16b, v6.16b
 
    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v9.16b, v8.16b, v6.16b
 
    sqxtun       v6.8b, v20.8h // g_1
    sqxtun       v8.8b, v22.8h // g_2
    zip1         v27.16b, v8.16b, v6.16b
 
    //do_store        \bpp, 8
 
  
    // 4
    add Y, Y, Y_N
    add UV, UV, UV_N
    add Y2, Y2, Y_N
 
    do_load         8
    do_yuv_to_rgb_stage1
 
    //do_yuv_to_rgb_stage2
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
 
    sqxtun       v6.8b, v24.8h // r_1
    sqxtun       v8.8b, v26.8h // r_2
    zip1         v17.16b, v8.16b, v6.16b
 
    sqxtun       v6.8b, v28.8h // b_1
    sqxtun       v8.8b, v30.8h // b_2
    zip1         v21.16b, v8.16b, v6.16b
 
    sqxtun       v6.8b, v20.8h // g_1
    sqxtun       v8.8b, v22.8h // g_2
    zip1         v29.16b, v8.16b, v6.16b

    //do_store        \bpp, 8

    // transpose

    // v14, v15, v16, v17 - R
    //1
    zip1            v18.8h,       v15.8h, v14.8h
    zip2            v20.8h,       v15.8h, v14.8h
    zip1            v24.8h,       v17.8h, v16.8h
    zip2            v28.8h,       v17.8h, v16.8h
 
    //2				          
    zip1            v14.4s,       v20.4s, v18.4s
    zip2            v15.4s,       v20.4s, v18.4s
    zip1            v16.4s,       v28.4s, v24.4s
    zip2            v17.4s,       v28.4s, v24.4s
 
    //3				          
    zip1            v18.2d,       v16.2d, v14.2d
    zip2            v20.2d,       v16.2d, v14.2d
    zip1            v24.2d,       v17.2d, v15.2d
    zip2            v28.2d,       v17.2d, v15.2d
 
    //4
    mov             v14.2d, v18.2d
    mov             v15.2d, v20.2d
    mov             v16.2d, v24.2d
    mov             v17.2d, v28.2d

    ins             v13.s[0], v14.s[1]
    ins             v14.s[1], v14.s[2]
    ins             v14.s[2], v13.s[0]
 
    ins             v13.s[0], v15.s[1]
    ins             v15.s[1], v15.s[2]
    ins             v15.s[2], v13.s[0]
 
    ins             v13.s[0], v16.s[1]
    ins             v16.s[1], v16.s[2]
    ins             v16.s[2], v13.s[0]
 
    ins             v13.s[0], v17.s[1]
    ins             v17.s[1], v17.s[2]
    ins             v17.s[2], v13.s[0]

    // 
    // v31, v7, v9, v21 - G
    //1
    zip1            v18.8h,       v7.8h, v31.8h
    zip2            v20.8h,       v7.8h, v31.8h
    zip1            v24.8h,       v21.8h, v9.8h
    zip2            v28.8h,       v21.8h, v9.8h
 
    //2				          
    zip1            v31.4s,       v20.4s, v18.4s
    zip2            v7.4s,        v20.4s, v18.4s
    zip1            v9.4s,        v28.4s, v24.4s
    zip2            v21.4s,       v28.4s, v24.4s
 
    //3				          
    zip1            v18.2d,       v9.2d, v31.2d
    zip2            v20.2d,       v9.2d, v31.2d
    zip1            v24.2d,       v21.2d, v7.2d
    zip2            v28.2d,       v21.2d, v7.2d
 
    //4
    mov             v31.2d, v18.2d
    mov             v7.2d, v20.2d
    mov             v9.2d, v24.2d
    mov             v21.2d, v28.2d
 
    ins             v13.s[0], v31.s[1]
    ins             v31.s[1], v31.s[2]
    ins             v31.s[2], v13.s[0]
 
    ins             v13.s[0], v7.s[1]
    ins             v7.s[1], v7.s[2]
    ins             v7.s[2], v13.s[0]
 
    ins             v13.s[0], v9.s[1]
    ins             v9.s[1], v9.s[2]
    ins             v9.s[2], v13.s[0]
 
    ins             v13.s[0], v21.s[1]
    ins             v21.s[1], v21.s[2]
    ins             v21.s[2], v13.s[0]
 
    // v23, v25, v27, v29 - B
    //1
    zip1            v18.8h,       v25.8h, v23.8h
    zip2            v20.8h,       v25.8h, v23.8h
    zip1            v24.8h,       v29.8h, v27.8h
    zip2            v28.8h,       v29.8h, v27.8h
 
    //2				          
    zip1            v23.4s,       v20.4s, v18.4s
    zip2            v25.4s,       v20.4s, v18.4s
    zip1            v27.4s,       v28.4s, v24.4s
    zip2            v29.4s,       v28.4s, v24.4s
 
    //3				          
    zip1            v18.2d,       v27.2d, v23.2d
    zip2            v20.2d,       v27.2d, v23.2d
    zip1            v24.2d,       v29.2d, v25.2d
    zip2            v28.2d,       v29.2d, v25.2d
 
    //4
    mov             v23.2d, v18.2d
    mov             v25.2d, v20.2d
    mov             v27.2d, v24.2d
    mov             v29.2d, v28.2d

    ins             v13.s[0], v23.s[1]
    ins             v23.s[1], v23.s[2]
    ins             v23.s[2], v13.s[0]
 
    ins             v13.s[0], v25.s[1]
    ins             v25.s[1], v25.s[2]
    ins             v25.s[2], v13.s[0]
 
    ins             v13.s[0], v27.s[1]
    ins             v27.s[1], v27.s[2]
    ins             v27.s[2], v13.s[0]
 
    ins             v13.s[0], v29.s[1]
    ins             v29.s[1], v29.s[2]
    ins             v29.s[2], v13.s[0]


//	
    movi            v13.16b, #255      // a
 
    // 1
    mov             v10.d[0], v14.d[1] // r
    mov				v11.d[0], v23.d[1] // g
    mov				v12.d[0], v31.d[1] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
 
    // 2
    mov             v10.d[0], v15.d[1] // r
    mov				v11.d[0], v25.d[1] // g
    mov				v12.d[0], v7.d[1] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
 
    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N
 
    // 3
    mov             v10.d[0], v16.d[1] // r
    mov				v11.d[0], v27.d[1] // g
    mov				v12.d[0], v9.d[1] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
 
    // 4
    mov             v10.d[0], v17.d[1] // r
    mov				v11.d[0], v29.d[1] // g
    mov				v12.d[0], v21.d[1] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32

    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N
 
    // 5
    mov             v10.d[0], v14.d[0] // r
    mov				v11.d[0], v23.d[0] // g
    mov				v12.d[0], v31.d[0] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
 
    // 6
    mov             v10.d[0], v15.d[0] // r
    mov				v11.d[0], v25.d[0] // g
    mov				v12.d[0], v7.d[0] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32
 	
    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N
 
    // 7
    mov             v10.d[0], v16.d[0] // r
    mov				v11.d[0], v27.d[0] // g
    mov				v12.d[0], v9.d[0] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB], 32
 
    // 8
    mov             v10.d[0], v17.d[0] // r
    mov				v11.d[0], v29.d[0] // g
    mov				v12.d[0], v21.d[0] // b
 
    st4             {v10.8b, v11.8b, v12.8b, v13.8b}, [RGB2], 32





	// landscape full
    add             N, N, #8

    ldr             Y, [INPUT_BUF]
    ldr             UV, [INPUT_BUF, 8]
    ldr             Y2, [INPUT_BUF, 24]

	ldr             Y, [Y]
	ldr             UV, [UV]
	ldr             Y2, [Y2]

	add             Y, Y, N
	add             UV, UV, N
	add             Y2, Y2, N

    add RGB, RGB, RGB_N
    add RGB2, RGB2, RGB_N

	cmp             N, INPUT_WIDTH
    b.lt            0b






//	st1  {v14.16b}, [RGB], 16
//	st1  {v15.16b}, [RGB], 16
//
//	st1  {v16.16b}, [RGB2], 16
//	st1  {v17.16b}, [RGB2], 16

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

    .unreq          INPUT_WIDTH
	.unreq          INPUT_BUF
    .unreq          OUTPUT_BUF
	.unreq          OUTPUT_WIDTH
    .unreq          RGB
    .unreq          RGB2
    .unreq          Y_N
    .unreq          UV_N
    .unreq          RGB_N
    .unreq          Y
    .unreq          Y2
    .unreq          UV
    .unreq          N

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.endm

/*--------------------------------- id ---------------     bpp R  rsize  G  gsize  B  bsize                     defsize */
generate_jsimd_ycc_rgb_90_8x8_convert_neon_nv21 nv21_extrgbx, 32, 0, .4h,   1, .4h,   2, .4h,     4,    5,    6,    .8b   
generate_jsimd_ycc_rgb_90_8x8_convert_neon_nv21 nv21_extbgrx, 32, 2, .4h,   1, .4h,   0, .4h,     6,    5,    4,    .8b   
.purgem do_load
.purgem do_store


/*****************************************************************************************/