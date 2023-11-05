
#if defined(__linux__) && defined(__ELF__)
.section .note.GNU-stack,"",%progbits /* mark stack as non-executable */
#endif

.text
.fpu neon
.arch armv7a
.object_arch armv4
.arm


#define RESPECT_STRICT_ALIGNMENT 1

/*****************************************************************************/

/* Supplementary macro for setting function attributes */
.macro asm_function fname
#ifdef __APPLE__
    .func _\fname
    .globl _\fname
_\fname:
#else
    .func \fname
    .global \fname
#ifdef __ELF__
    .hidden \fname
    .type \fname, %function
#endif
\fname:
#endif
.endm

/* Transpose a block of 4x4 coefficients in four 64-bit registers */
.macro transpose_4x4 x0, x1, x2, x3
    vtrn.16 \x0, \x1
    vtrn.16 \x2, \x3
    vtrn.32 \x0, \x2
    vtrn.32 \x1, \x3
.endm

#define CENTERJSAMPLE 128



/*****************************************************************************/

/*
 * jsimd_ycc_nv12_extrgb_convert_neon
 * jsimd_ycc_nv12_extbgr_convert_neon
 * jsimd_ycc_nv12_extrgbx_convert_neon
 * jsimd_ycc_nv12_extbgrx_convert_neon
 * jsimd_ycc_nv12_extxbgr_convert_neon
 * jsimd_ycc_nv12_extxrgb_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    .if \size == 8
        vld1.8  {d4[0]}, [U]
        vld1.8  {d4[1]}, [U]!
        vld1.8  {d5[0]}, [U]
        vld1.8  {d5[1]}, [U]!
        vld1.8  {d4[2]}, [U]
        vld1.8  {d4[3]}, [U]!
        vld1.8  {d5[2]}, [U]
        vld1.8  {d5[3]}, [U]!
        vld1.8  {d4[4]}, [U]
        vld1.8  {d4[5]}, [U]!
        vld1.8  {d5[4]}, [U]
        vld1.8  {d5[5]}, [U]!
        vld1.8  {d4[6]}, [U]
        vld1.8  {d4[7]}, [U]!
        vld1.8  {d5[6]}, [U]
        vld1.8  {d5[7]}, [U]!
        vld1.8  {d19}, [Y, :64]!
        vld1.8  {d18}, [Y2, :64]!
        pld     [U, #32]
        pld     [U, #32]
        pld     [Y, #64]
        pld     [Y2, #64]
    .elseif \size == 4
        vld1.8  {d4[0]}, [U]
        vld1.8  {d4[1]}, [U]!
        vld1.8  {d5[0]}, [U]
        vld1.8  {d5[1]}, [U]!
        vld1.8  {d4[2]}, [U]
        vld1.8  {d4[3]}, [U]!
        vld1.8  {d5[2]}, [U]
        vld1.8  {d5[3]}, [U]!
        vld1.8  {d19[0]}, [Y]!
        vld1.8  {d19[1]}, [Y]!
        vld1.8  {d19[2]}, [Y]!
        vld1.8  {d19[3]}, [Y]!
        vld1.8  {d18[0]}, [Y2]!
        vld1.8  {d18[1]}, [Y2]!
        vld1.8  {d18[2]}, [Y2]!
        vld1.8  {d18[3]}, [Y2]!
    .elseif \size == 2
        vld1.8  {d4[4]}, [U]
        vld1.8  {d4[5]}, [U]!
        vld1.8  {d5[4]}, [U]
        vld1.8  {d5[5]}, [U]!
        vld1.8  {d19[4]}, [Y]!
        vld1.8  {d19[5]}, [Y]!
        vld1.8  {d18[4]}, [Y2]!
        vld1.8  {d18[5]}, [Y2]!
    .elseif \size == 1
        vld1.8  {d4[6]}, [U]!
        vld1.8  {d5[6]}, [U]!
        vld1.8  {d19[6]}, [Y]!
        vld1.8  {d18[6]}, [Y2]!
    .else
        .error unsupported macroblock size
    .endif
.endm

.macro do_store bpp, size
    .if \bpp == 24
        .if \size == 8
            vst3.8  {d10, d11, d12}, [RGB]!
            vst3.8  {d13, d14, d15}, [RGB2]!
        .elseif \size == 4
            vst3.8  {d10[0], d11[0], d12[0]}, [RGB]!
            vst3.8  {d10[1], d11[1], d12[1]}, [RGB]!
            vst3.8  {d10[2], d11[2], d12[2]}, [RGB]!
            vst3.8  {d10[3], d11[3], d12[3]}, [RGB]!
            vst3.8  {d13[0], d14[0], d15[0]}, [RGB2]!
            vst3.8  {d13[1], d14[1], d15[1]}, [RGB2]!
            vst3.8  {d13[2], d14[2], d15[2]}, [RGB2]!
            vst3.8  {d13[3], d14[3], d15[3]}, [RGB2]!
        .elseif \size == 2
            vst3.8  {d10[4], d11[4], d12[4]}, [RGB]!
            vst3.8  {d10[5], d11[5], d12[5]}, [RGB]!
            vst3.8  {d13[4], d14[4], d15[4]}, [RGB2]!
            vst3.8  {d13[5], d14[5], d15[5]}, [RGB2]!
        .elseif \size == 1
            vst3.8  {d10[6], d11[6], d12[6]}, [RGB]!
            vst3.8  {d13[6], d14[6], d15[6]}, [RGB2]!
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp == 32
        .if \size == 8
            vst4.8  {d10, d11, d12, d13}, [RGB]!
            vst4.8  {d14, d15, d16, d17}, [RGB2]!
        .elseif \size == 4
            vst4.8  {d10[0], d11[0], d12[0], d13[0]}, [RGB]!
            vst4.8  {d10[1], d11[1], d12[1], d13[1]}, [RGB]!
            vst4.8  {d10[2], d11[2], d12[2], d13[2]}, [RGB]!
            vst4.8  {d10[3], d11[3], d12[3], d13[3]}, [RGB]!
            vst4.8  {d14[0], d15[0], d16[0], d17[0]}, [RGB2]!
            vst4.8  {d14[1], d15[1], d16[1], d17[1]}, [RGB2]!
            vst4.8  {d14[2], d15[2], d16[2], d17[2]}, [RGB2]!
            vst4.8  {d14[3], d15[3], d16[3], d17[3]}, [RGB2]!
        .elseif \size == 2
            vst4.8  {d10[4], d11[4], d12[4], d13[4]}, [RGB]!
            vst4.8  {d10[5], d11[5], d12[5], d13[5]}, [RGB]!
            vst4.8  {d14[4], d15[4], d16[4], d17[4]}, [RGB2]!
            vst4.8  {d14[5], d15[5], d16[5], d17[5]}, [RGB2]!
        .elseif \size == 1
            vst4.8  {d10[6], d11[6], d12[6], d13[6]}, [RGB]!
            vst4.8  {d14[6], d15[6], d16[6], d17[6]}, [RGB2]!
        .else
            .error unsupported macroblock size
        .endif
    .else
        .error unsupported bpp
    .endif
.endm

.macro generate_jsimd_ycc_rgb_convert_neon_nv12 colorid, bpp, r_offs, g_offs, b_offs, r_offs2, g_offs2, b_offs2

/*
 * 2 stage pipelined YCbCr->RGB conversion
 */

.macro do_yuv_to_rgb_stage1
    vaddw.u8        q3, q1, d4     /* q3 = u - 128 */
    vaddw.u8        q4, q1, d5     /* q2 = v - 128 */
    vmull.s16       q10, d6, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q10, d8, d1[1] /* multiply by -400 */
    vmull.s16       q11, d7, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q11, d9, d1[1] /* multiply by -400 */
    vmull.s16       q12, d8, d1[0] /* multiply by 1634 */	/* red */
    vmull.s16       q13, d9, d1[0] /* multiply by 1634 */
    vmull.s16       q14, d6, d1[3] /* multiply by 2066 */	/* blue */
    vmull.s16       q15, d7, d1[3] /* multiply by 2066 */
.endm

.macro do_yuv_to_rgb_stage2
    vrshrn.s32      d20, q10, #10
    vrshrn.s32      d21, q11, #10
    vrshrn.s32      d24, q12, #10
    vrshrn.s32      d25, q13, #10
    vrshrn.s32      d28, q14, #10
    vrshrn.s32      d29, q15, #10

	// amouse fix color conversion polynomial
	vmov.s8			d26, #16
	vsubl.u8		q11, d18, d26
	vmull.s16		q15, d0, d22
	vshrn.s32		d22, q15, #10
	vmull.s16		q15, d0, d23
	vshrn.s32		d23, q15, #10

	vsubl.u8		q13, d19, d26
	vmull.s16		q15, d0, d26
	vshrn.s32		d18, q15, #10
	vmull.s16		q15, d0, d27
	vshrn.s32		d19, q15, #10

	vadd.s16        q13, q12, q11
	vadd.s16   	    q15, q14, q11
	vadd.s16 	    q11, q10, q11

	vadd.s16        q10, q10, q9
    vadd.s16        q12, q12, q9
    vadd.s16        q14, q14, q9
	//////////////////////////////////////////////////

    vqmovun.s16     d1\g_offs, q10
    vqmovun.s16     d1\g_offs2, q11
    vqmovun.s16     d1\r_offs, q12
    vqmovun.s16     d1\r_offs2, q13
    vqmovun.s16     d1\b_offs, q14
    vqmovun.s16     d1\b_offs2, q15
.endm

.macro do_yuv_to_rgb_stage2_store_load_stage1
    vld1.8  {d4[0]}, [U]
    vld1.8  {d4[1]}, [U]!
    vld1.8  {d5[0]}, [U]
    vld1.8  {d5[1]}, [U]!
    vld1.8  {d4[2]}, [U]
    vld1.8  {d4[3]}, [U]!
    vld1.8  {d5[2]}, [U]
    vld1.8  {d5[3]}, [U]!
    vld1.8  {d4[4]}, [U]
    vld1.8  {d4[5]}, [U]!
    vld1.8  {d5[4]}, [U]
    vld1.8  {d5[5]}, [U]!
    vld1.8  {d4[6]}, [U]
    vld1.8  {d4[7]}, [U]!
    vld1.8  {d5[6]}, [U]
    vld1.8  {d5[7]}, [U]!
    vrshrn.s32      d20, q10, #10
    vrshrn.s32      d21, q11, #10
    vrshrn.s32      d24, q12, #10
    vrshrn.s32      d25, q13, #10
    vrshrn.s32      d28, q14, #10
    vrshrn.s32      d29, q15, #10

	// amouse fix color conversion polynomial
	vmov.s8			d26, #16
	vsubl.u8		q11, d18, d26
	vmull.s16		q15, d0, d22
	vshrn.s32		d22, q15, #10
	vmull.s16		q15, d0, d23
	vshrn.s32		d23, q15, #10

	vsubl.u8		q13, d19, d26
	vmull.s16		q15, d0, d26
	vshrn.s32		d18, q15, #10
	vmull.s16		q15, d0, d27
	vshrn.s32		d19, q15, #10

	vadd.s16        q13, q12, q11
	vadd.s16   	    q15, q14, q11
	vadd.s16 	    q11, q10, q11

	vadd.s16        q10, q10, q9
    vadd.s16        q12, q12, q9
    vadd.s16        q14, q14, q9
	//////////////////////////////////////////////////

      vqmovun.s16     d1\g_offs, q10
      vqmovun.s16     d1\g_offs2, q11
    vld1.8          {d19}, [Y, :64]!
    vld1.8          {d18}, [Y2, :64]!
      vqmovun.s16     d1\r_offs, q12
      vqmovun.s16     d1\r_offs2, q13
    pld             [U, #32]
    pld             [U, #32]
    pld             [Y, #64]
      vqmovun.s16     d1\b_offs, q14
      vqmovun.s16     d1\b_offs2, q15
    vaddw.u8        q3, q1, d4     /* q3 = u - 128 */
    vaddw.u8        q4, q1, d5     /* q2 = v - 128 */
      do_store        \bpp, 8
    vmull.s16       q10, d6, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q10, d8, d1[1] /* multiply by -400 */
    vmull.s16       q11, d7, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q11, d9, d1[1] /* multiply by -400 */
    vmull.s16       q12, d8, d1[0] /* multiply by 1634 */	/* red */
    vmull.s16       q13, d9, d1[0] /* multiply by 1634 */
    vmull.s16       q14, d6, d1[3] /* multiply by 2066 */	/* blue */
    vmull.s16       q15, d7, d1[3] /* multiply by 2066 */
.endm

.macro do_yuv_to_rgb
    do_yuv_to_rgb_stage1
    do_yuv_to_rgb_stage2
.endm

/* Apple gas crashes on adrl, work around that by using adr.
 * But this requires a copy of these constants for each function.
 */

//nv12
.balign 16
jsimd_ycc_\colorid\()_neon_consts:
    .short			1192, 1192, 1192, 1192
    .short			1634, -833, -400, 2066
    .short          -128,  -128,   -128,   -128
    .short          -128,  -128,   -128,   -128

asm_function jsimd_ycc_\colorid\()_convert_neon
    OUTPUT_WIDTH    .req r0
    INPUT_BUF       .req r1
    OUTPUT_BUF      .req r2

	RGB             .req r5
    RGB2            .req r6
    Y               .req r7
    Y2              .req r8
    U               .req r9
    V               .req r10
    N               .req ip

    /* Load constants to d1, d2, d3 (d0 is just used for padding) */
    adr             ip, jsimd_ycc_\colorid\()_neon_consts
    vld1.16         {d0, d1, d2, d3}, [ip, :128]

    /* Save ARM registers and handle input arguments */
    push            {r4, r5, r6, r7, r8, r9, r10, lr}
    //ldr             NUM_ROWS, [sp, #(4 * 8)]
    ldr             Y, [INPUT_BUF]
    ldr             U, [INPUT_BUF, #4]
    ldr             V, [INPUT_BUF, #8]
    ldr             Y2, [INPUT_BUF, #12]
    .unreq          INPUT_BUF

    /* Save NEON registers */
    vpush           {d8-d15}

    /* Initially set d10, d11, d12, d13 to 0xFF */
    vmov.u8         q5, #255
    vmov.u8         q6, #255
    vmov.u8         q7, #255
    vmov.u8         q8, #255

    /* Outer loop over scanlines */
0:
    ldr             Y,  [Y]
    ldr             Y2, [Y2]
    ldr             U,  [U]
    ldr             V,  [V]

    mov             N, OUTPUT_WIDTH
    //   add             INPUT_ROW, INPUT_ROW, #1
    ldr             RGB, [OUTPUT_BUF], #4
    ldr             RGB2, [OUTPUT_BUF], #4

    // Inner loop over pixels
    subs            N, N, #8
    blt             3f
    do_load         8
    do_yuv_to_rgb_stage1
    subs            N, N, #8
    blt             2f
1:
    do_yuv_to_rgb_stage2_store_load_stage1
    subs            N, N, #8
    bge             1b
2:
    do_yuv_to_rgb_stage2
    do_store        \bpp, 8
    tst             N, #7
    beq             8f
3:
    tst             N, #4
    beq             3f
    do_load         4
3:
    tst             N, #2
    beq             4f
    do_load         2
4:
    tst             N, #1
    beq             5f
    do_load         1
5:
    do_yuv_to_rgb
    tst             N, #4
    beq             6f
    do_store        \bpp, 4
6:
    tst             N, #2
    beq             7f
    do_store        \bpp, 2
7:
    tst             N, #1
    beq             8f
    do_store        \bpp, 1
8:

    /* Restore all registers and return */
    vpop            {d8-d15}
    pop             {r4, r5, r6, r7, r8, r9, r10, pc}

    .unreq          OUTPUT_WIDTH
    .unreq          OUTPUT_BUF
    .unreq          RGB
    .unreq          RGB2
    .unreq          Y
    .unreq          Y2
    .unreq          U
    .unreq          V
    .unreq          N
.endfunc

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.purgem do_yuv_to_rgb_stage2_store_load_stage1

.endm

/*--------------------------------- id ----- bpp R  G  B */
generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extrgb,  24, 0, 1, 2, 3, 4, 5
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extbgr,  24, 2, 1, 0, 5, 4, 3
generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extrgbx, 32, 0, 1, 2, 4, 5, 6
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extbgrx, 32, 2, 1, 0, 6, 5, 4
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extxbgr, 32, 3, 2, 1, 7, 6, 5
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extxrgb, 32, 1, 2, 3, 5, 6, 7

.purgem do_load
.purgem do_store


/*****************************************************************************/

/*
 * jsimd_ycc_nv12_extrgb_convert_neon
 * jsimd_ycc_nv12_extbgr_convert_neon
 * jsimd_ycc_nv12_extrgbx_convert_neon
 * jsimd_ycc_nv12_extbgrx_convert_neon
 * jsimd_ycc_nv12_extxbgr_convert_neon
 * jsimd_ycc_nv12_extxrgb_convert_neon
 *
 * Colorspace conversion YCbCr -> RGB
 */


.macro do_load size
    .if \size == 8
        vld1.8  {d4[0]}, [U]
        vld1.8  {d4[1]}, [U]!
        vld1.8  {d5[0]}, [U]
        vld1.8  {d5[1]}, [U]!
        vld1.8  {d4[2]}, [U]
        vld1.8  {d4[3]}, [U]!
        vld1.8  {d5[2]}, [U]
        vld1.8  {d5[3]}, [U]!
        vld1.8  {d4[4]}, [U]
        vld1.8  {d4[5]}, [U]!
        vld1.8  {d5[4]}, [U]
        vld1.8  {d5[5]}, [U]!
        vld1.8  {d4[6]}, [U]
        vld1.8  {d4[7]}, [U]!
        vld1.8  {d5[6]}, [U]
        vld1.8  {d5[7]}, [U]!
        vld1.8  {d19}, [Y, :64]!
        pld     [U, #32]
        pld     [U, #32]
        pld     [Y, #64]
    .elseif \size == 4
        vld1.8  {d4[0]}, [U]
        vld1.8  {d4[1]}, [U]!
        vld1.8  {d5[0]}, [U]
        vld1.8  {d5[1]}, [U]!
        vld1.8  {d4[2]}, [U]
        vld1.8  {d4[3]}, [U]!
        vld1.8  {d5[2]}, [U]
        vld1.8  {d5[3]}, [U]!
        vld1.8  {d19[0]}, [Y]!
        vld1.8  {d19[1]}, [Y]!
        vld1.8  {d19[2]}, [Y]!
        vld1.8  {d19[3]}, [Y]!
    .elseif \size == 2
        vld1.8  {d4[4]}, [U]
        vld1.8  {d4[5]}, [U]!
        vld1.8  {d5[4]}, [U]
        vld1.8  {d5[5]}, [U]!
        vld1.8  {d19[4]}, [Y]!
        vld1.8  {d19[5]}, [Y]!
    .elseif \size == 1
        vld1.8  {d4[6]}, [U]!
        vld1.8  {d5[6]}, [U]!
        vld1.8  {d19[6]}, [Y]!
    .else
        .error unsupported macroblock size
    .endif
.endm

.macro do_store bpp, size
    .if \bpp == 24
        .if \size == 8
            vst3.8  {d10, d11, d12}, [RGB]!
        .elseif \size == 4
            vst3.8  {d10[0], d11[0], d12[0]}, [RGB]!
            vst3.8  {d10[1], d11[1], d12[1]}, [RGB]!
            vst3.8  {d10[2], d11[2], d12[2]}, [RGB]!
            vst3.8  {d10[3], d11[3], d12[3]}, [RGB]!
        .elseif \size == 2
            vst3.8  {d10[4], d11[4], d12[4]}, [RGB]!
            vst3.8  {d10[5], d11[5], d12[5]}, [RGB]!
        .elseif \size == 1
            vst3.8  {d10[6], d11[6], d12[6]}, [RGB]!
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp == 32
        .if \size == 8
            vst4.8  {d10, d11, d12, d13}, [RGB]!
        .elseif \size == 4
            vst4.8  {d10[0], d11[0], d12[0], d13[0]}, [RGB]!
            vst4.8  {d10[1], d11[1], d12[1], d13[1]}, [RGB]!
            vst4.8  {d10[2], d11[2], d12[2], d13[2]}, [RGB]!
            vst4.8  {d10[3], d11[3], d12[3], d13[3]}, [RGB]!
        .elseif \size == 2
            vst4.8  {d10[4], d11[4], d12[4], d13[4]}, [RGB]!
            vst4.8  {d10[5], d11[5], d12[5], d13[5]}, [RGB]!
        .elseif \size == 1
            vst4.8  {d10[6], d11[6], d12[6], d13[6]}, [RGB]!
        .else
            .error unsupported macroblock size
        .endif
    .else
        .error unsupported bpp
    .endif
.endm

.macro generate_jsimd_ycc_rgb_convert_neon_nv12_single_line colorid, bpp, r_offs, g_offs, b_offs

/*
 * 2 stage pipelined YCbCr->RGB conversion
 */

.macro do_yuv_to_rgb_stage1
    vaddw.u8        q3, q1, d4     /* q3 = u - 128 */
    vaddw.u8        q4, q1, d5     /* q2 = v - 128 */
    vmull.s16       q10, d6, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q10, d8, d1[1] /* multiply by -400 */
    vmull.s16       q11, d7, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q11, d9, d1[1] /* multiply by -400 */
    vmull.s16       q12, d8, d1[0] /* multiply by 1634 */	/* red */
    vmull.s16       q13, d9, d1[0] /* multiply by 1634 */
    vmull.s16       q14, d6, d1[3] /* multiply by 2066 */	/* blue */
    vmull.s16       q15, d7, d1[3] /* multiply by 2066 */
.endm

.macro do_yuv_to_rgb_stage2
    vrshrn.s32      d20, q10, #10
    vrshrn.s32      d21, q11, #10
    vrshrn.s32      d24, q12, #10
    vrshrn.s32      d25, q13, #10
    // shift 14 to 13
    vrshrn.s32      d28, q14, #13
    // shift 14 to 13
    vrshrn.s32      d29, q15, #13

    // d30, q11 q13, q15
	// amouse fix color conversion polynomial
	vsubl.u8		q13, d19, d26
	vmull.s16		q15, d0, d26
	vshrn.s32		d18, q15, #10
	vmull.s16		q15, d0, d27
	vshrn.s32		d19, q15, #10

	vadd.s16        q10, q10, q9
    vadd.s16        q12, q12, q9
    vadd.s16        q14, q14, q9
	//////////////////////////////////////////////////

    vqmovun.s16     d1\g_offs, q10
    vqmovun.s16     d1\r_offs, q12
    vqmovun.s16     d1\b_offs, q14
.endm

.macro do_yuv_to_rgb_stage2_store_load_stage1
    vld1.8  {d4[0]}, [U]
    vld1.8  {d4[1]}, [U]!
    vld1.8  {d5[0]}, [U]
    vld1.8  {d5[1]}, [U]!
    vld1.8  {d4[2]}, [U]
    vld1.8  {d4[3]}, [U]!
    vld1.8  {d5[2]}, [U]
    vld1.8  {d5[3]}, [U]!
    vld1.8  {d4[4]}, [U]
    vld1.8  {d4[5]}, [U]!
    vld1.8  {d5[4]}, [U]
    vld1.8  {d5[5]}, [U]!
    vld1.8  {d4[6]}, [U]
    vld1.8  {d4[7]}, [U]!
    vld1.8  {d5[6]}, [U]
    vld1.8  {d5[7]}, [U]!
    vrshrn.s32      d20, q10, #10
    vrshrn.s32      d21, q11, #10
    vrshrn.s32      d24, q12, #10
    vrshrn.s32      d25, q13, #10
    vrshrn.s32      d28, q14, #10
    vrshrn.s32      d29, q15, #10

      // d30, q11 q13, q15
	// amouse fix color conversion polynomial
	vmov.s8			d26, #16
	vsubl.u8		q13, d19, d26
	vmull.s16		q15, d0, d26
	vshrn.s32		d18, q15, #10
	vmull.s16		q15, d0, d27
	vshrn.s32		d19, q15, #10

	vadd.s16        q10, q10, q9
    vadd.s16        q12, q12, q9
    vadd.s16        q14, q14, q9
	//////////////////////////////////////////////////

      vqmovun.s16     d1\g_offs, q10
    vld1.8          {d19}, [Y, :64]!
      vqmovun.s16     d1\r_offs, q12
    pld             [U, #32]
    pld             [U, #32]
    pld             [Y, #64]
      vqmovun.s16     d1\b_offs, q14
    vaddw.u8        q3, q1, d4     /* q3 = u - 128 */
    vaddw.u8        q4, q1, d5     /* q2 = v - 128 */
      do_store        \bpp, 8
    vmull.s16       q10, d6, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q10, d8, d1[1] /* multiply by -400 */
    vmull.s16       q11, d7, d1[2] /* multiply by -833 */	/* green */
    vmlal.s16       q11, d9, d1[1] /* multiply by -400 */
    vmull.s16       q12, d8, d1[0] /* multiply by 1634 */	/* red */
    vmull.s16       q13, d9, d1[0] /* multiply by 1634 */
    vmull.s16       q14, d6, d1[3] /* multiply by 2066 */	/* blue */
    vmull.s16       q15, d7, d1[3] /* multiply by 2066 */
.endm

.macro do_yuv_to_rgb
    do_yuv_to_rgb_stage1
    do_yuv_to_rgb_stage2
.endm

/* Apple gas crashes on adrl, work around that by using adr.
 * But this requires a copy of these constants for each function.
 */

//nv12

.balign 16
jsimd_ycc_\colorid\()_neon_consts:
    .short			1192, 1192, 1192, 1192
    .short			1634, -833, -400, 2066
    .short          -128,  -128,   -128,   -128
    .short          -128,  -128,   -128,   -128

asm_function jsimd_ycc_\colorid\()_convert_neon
    OUTPUT_WIDTH    .req r0
    INPUT_BUF       .req r1
    OUTPUT_BUF      .req r2

	RGB             .req r5
    Y               .req r7
    U               .req r9
    V               .req r10
    N               .req ip

    /* Load constants to d1, d2, d3 (d0 is just used for padding) */
    adr             ip, jsimd_ycc_\colorid\()_neon_consts
    vld1.16         {d0, d1, d2, d3}, [ip, :128]

    /* Save ARM registers and handle input arguments */
    push            {r4, r5, r6, r7, r8, r9, r10, lr}
    //ldr             NUM_ROWS, [sp, #(4 * 8)]
    ldr             Y, [INPUT_BUF]
    ldr             U, [INPUT_BUF, #4]
    ldr             V, [INPUT_BUF, #8]
    .unreq          INPUT_BUF

    /* Save NEON registers */
    vpush           {d8-d15}

    /* Initially set d10, d11, d12, d13 to 0xFF */
    vmov.u8         q5, #255
    vmov.u8         q6, #255
    vmov.u8         q7, #255
    vmov.u8         q8, #255

    /* Outer loop over scanlines */
0:
    ldr             Y,  [Y]
    ldr             U,  [U]
    ldr             V,  [V]

    mov             N, OUTPUT_WIDTH
    //   add             INPUT_ROW, INPUT_ROW, #1
    ldr             RGB, [OUTPUT_BUF], #4

    // Inner loop over pixels
    subs            N, N, #8
    blt             3f
    do_load         8
    do_yuv_to_rgb_stage1
    subs            N, N, #8
    blt             2f
1:
    do_yuv_to_rgb_stage2_store_load_stage1
    subs            N, N, #8
    bge             1b
2:
    do_yuv_to_rgb_stage2
    do_store        \bpp, 8
    tst             N, #7
    beq             8f
3:
    tst             N, #4
    beq             3f
    do_load         4
3:
    tst             N, #2
    beq             4f
    do_load         2
4:
    tst             N, #1
    beq             5f
    do_load         1
5:
    do_yuv_to_rgb
    tst             N, #4
    beq             6f
    do_store        \bpp, 4
6:
    tst             N, #2
    beq             7f
    do_store        \bpp, 2
7:
    tst             N, #1
    beq             8f
    do_store        \bpp, 1
8:
    /* Restore all registers and return */
    vpop            {d8-d15}
    pop             {r4, r5, r6, r7, r8, r9, r10, pc}

    .unreq          OUTPUT_WIDTH
    .unreq          OUTPUT_BUF
    .unreq          RGB
    .unreq          Y
    .unreq          U
    .unreq          V
    .unreq          N
.endfunc

.purgem do_yuv_to_rgb
.purgem do_yuv_to_rgb_stage1
.purgem do_yuv_to_rgb_stage2
.purgem do_yuv_to_rgb_stage2_store_load_stage1

.endm

/*--------------------------------- id ----- bpp R  G  B */
generate_jsimd_ycc_rgb_convert_neon_nv12_single_line nv12_extrgb_single_line,  24, 0, 1, 2
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extbgr,  24, 2, 1, 0, 5, 4, 3
generate_jsimd_ycc_rgb_convert_neon_nv12_single_line nv12_extrgbx_single_line, 32, 0, 1, 2
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extbgrx, 32, 2, 1, 0, 6, 5, 4
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extxbgr, 32, 3, 2, 1, 7, 6, 5
//generate_jsimd_ycc_rgb_convert_neon_nv12 nv12_extxrgb, 32, 1, 2, 3, 5, 6, 7

.purgem do_load
.purgem do_store
