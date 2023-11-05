
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

/*****************************************************************************/

/*
 * jsimd_extrgb_yccnv21_convert_neon
 * jsimd_extbgr_yccnv21_convert_neon
 * jsimd_extrgbx_yccnv21_convert_neon
 * jsimd_extbgrx_yccnv21_convert_neon
 * jsimd_extxbgr_yccnv21_convert_neon
 * jsimd_extxrgb_yccnv21_convert_neon
 *
 * Colorspace conversion RGB -> YCbCr
 */

.macro do_store size
    .if \size == 8
        vst1.8  {d20}, [Y]!
        vst1.8  {d22[0]}, [U]!
        vst1.8  {d21[0]}, [U]!
        vst1.8  {d22[2]}, [U]!
        vst1.8  {d21[2]}, [U]!
        vst1.8  {d22[4]}, [U]!
        vst1.8  {d21[4]}, [U]!
        vst1.8  {d22[6]}, [U]!
        vst1.8  {d21[6]}, [U]!
    .elseif \size == 4
        vst1.8  {d20[0]}, [Y]!
        vst1.8  {d20[1]}, [Y]!
        vst1.8  {d20[2]}, [Y]!
        vst1.8  {d20[3]}, [Y]!
        vst1.8  {d22[0]}, [U]!
        vst1.8  {d21[0]}, [U]!
        vst1.8  {d22[2]}, [U]!
        vst1.8  {d21[2]}, [U]!
     .elseif \size == 2
        vst1.8  {d20[4]}, [Y]!
        vst1.8  {d20[5]}, [Y]!
        vst1.8  {d22[4]}, [U]!
        vst1.8  {d21[4]}, [U]!
    .elseif \size == 1
        vst1.8  {d20[6]}, [Y]!
        vst1.8  {d22[6]}, [U]!
        vst1.8  {d21[6]}, [U]!
    .else
        .error unsupported macroblock size
    .endif
.endm

.macro do_load bpp, size
    .if \bpp == 24
        .if \size == 8
            vld3.8  {d10, d11, d12}, [RGB]!
            pld     [RGB, #128]
        .elseif \size == 4
            vld3.8  {d10[0], d11[0], d12[0]}, [RGB]!
            vld3.8  {d10[1], d11[1], d12[1]}, [RGB]!
            vld3.8  {d10[2], d11[2], d12[2]}, [RGB]!
            vld3.8  {d10[3], d11[3], d12[3]}, [RGB]!
        .elseif \size == 2
            vld3.8  {d10[4], d11[4], d12[4]}, [RGB]!
            vld3.8  {d10[5], d11[5], d12[5]}, [RGB]!
        .elseif \size == 1
            vld3.8  {d10[6], d11[6], d12[6]}, [RGB]!
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp == 32
        .if \size == 8
            vld4.8  {d10, d11, d12, d13}, [RGB]!
            pld     [RGB, #128]
        .elseif \size == 4
            vld4.8  {d10[0], d11[0], d12[0], d13[0]}, [RGB]!
            vld4.8  {d10[1], d11[1], d12[1], d13[1]}, [RGB]!
            vld4.8  {d10[2], d11[2], d12[2], d13[2]}, [RGB]!
            vld4.8  {d10[3], d11[3], d12[3], d13[3]}, [RGB]!
        .elseif \size == 2
            vld4.8  {d10[4], d11[4], d12[4], d13[4]}, [RGB]!
            vld4.8  {d10[5], d11[5], d12[5], d13[5]}, [RGB]!
        .elseif \size == 1
            vld4.8  {d10[6], d11[6], d12[6], d13[6]}, [RGB]!
        .else
            .error unsupported macroblock size
        .endif
    .else
        .error unsupported bpp
    .endif
.endm

.macro generate_jsimd_rgb_yccnv21_convert_neon colorid, bpp, r_offs, g_offs, b_offs

/*
 * 2 stage pipelined RGB->YCbCr conversion
 */

.macro do_rgb_to_yuv_stage1
    vmovl.u8    q2, d1\r_offs /* r = { d4, d5 } */
    vmovl.u8    q3, d1\g_offs /* g = { d6, d7 } */
    vmovl.u8    q4, d1\b_offs /* b = { d8, d9 } */
    vmull.u16   q7, d4, d0[0]
    vmlal.u16   q7, d6, d0[1]
    vmlal.u16   q7, d8, d0[2]
    vmull.u16   q8, d5, d0[0]
    vmlal.u16   q8, d7, d0[1]
    vmlal.u16   q8, d9, d0[2]
    vrev64.32   q9,  q1
    vrev64.32   q13, q1
    vmlsl.u16   q9,  d4, d0[3]
    vmlsl.u16   q9,  d6, d1[0]
    vmlal.u16   q9,  d8, d1[1]
    vmlsl.u16   q13, d5, d0[3]
    vmlsl.u16   q13, d7, d1[0]
    vmlal.u16   q13, d9, d1[1]
    vrev64.32   q14, q1
    vrev64.32   q15, q1
    vmlal.u16   q14, d4, d1[1]
    vmlsl.u16   q14, d6, d1[2]
    vmlsl.u16   q14, d8, d1[3]
    vmlal.u16   q15, d5, d1[1]
    vmlsl.u16   q15, d7, d1[2]
    vmlsl.u16   q15, d9, d1[3]
.endm

.macro do_rgb_to_yuv_stage2
    vrshrn.u32  d20, q7,  #16
    vrshrn.u32  d21, q8,  #16
    vshrn.u32   d22, q9,  #16
    vshrn.u32   d23, q13, #16
    vshrn.u32   d24, q14, #16
    vshrn.u32   d25, q15, #16
    vmovn.u16   d20, q10      /* d20 = y */
    // amouse add for add 16 in y
    vmov.u8	d10, #16
    vadd.u8		d20, d20, d10
    /////////////////////////////////
    vmovn.u16   d21, q11      /* d21 = u */
    vmovn.u16   d22, q12      /* d22 = v */
.endm

.macro do_rgb_to_yuv
    do_rgb_to_yuv_stage1
    do_rgb_to_yuv_stage2
.endm

.macro do_rgb_to_yuv_stage2_store_load_stage1
      vrshrn.u32  d20, q7,  #16
      vrshrn.u32  d21, q8,  #16
      vshrn.u32   d22, q9,  #16
    vrev64.32   q9,  q1
      vshrn.u32   d23, q13, #16
    vrev64.32   q13, q1
      vshrn.u32   d24, q14, #16
      vshrn.u32   d25, q15, #16
    do_load     \bpp, 8
      vmovn.u16   d20, q10      /* d20 = y */
    vmovl.u8    q2, d1\r_offs   /* r = { d4, d5 } */
      vmovn.u16   d21, q11      /* d21 = u */
    vmovl.u8    q3, d1\g_offs   /* g = { d6, d7 } */
      vmovn.u16   d22, q12      /* d22 = v */
    vmovl.u8    q4, d1\b_offs   /* b = { d8, d9 } */
    vmull.u16   q7, d4, d0[0]
    vmlal.u16   q7, d6, d0[1]
    vmlal.u16   q7, d8, d0[2]
    // amouse add for add 16 in y
    vmov.u8	d10, #16
    vadd.u8		d20, d20, d10
    /////////////////////////////////
      vst1.8      {d20}, [Y]!
    vmull.u16   q8, d5, d0[0]
    vmlal.u16   q8, d7, d0[1]
    vmlal.u16   q8, d9, d0[2]
    vmlsl.u16   q9,  d4, d0[3]
    vmlsl.u16   q9,  d6, d1[0]
    vmlal.u16   q9,  d8, d1[1]
    
    vmlsl.u16   q13, d5, d0[3]
    vmlsl.u16   q13, d7, d1[0]
    vmlal.u16   q13, d9, d1[1]
    vrev64.32   q14, q1
    vrev64.32   q15, q1
    vmlal.u16   q14, d4, d1[1]
    vmlsl.u16   q14, d6, d1[2]
    vmlsl.u16   q14, d8, d1[3]
      
    vmlal.u16   q15, d5, d1[1]
    vmlsl.u16   q15, d7, d1[2]
    vmlsl.u16   q15, d9, d1[3]
    
  //  vst1.8      {d21}, [U]!
  //  vst1.8      {d22}, [V]!
    vst1.8  {d22[0]}, [U]!
    vst1.8  {d21[0]}, [U]!
    vst1.8  {d22[2]}, [U]!
    vst1.8  {d21[2]}, [U]!
    vst1.8  {d22[4]}, [U]!
    vst1.8  {d21[4]}, [U]!
    vst1.8  {d22[6]}, [U]!
    vst1.8  {d21[6]}, [U]!
.endm

.balign 16
jsimd_\colorid\()_yccnv21_neon_consts:
/*
    .short          19595, 38470, 7471,  11059
    .short          21709, 32768, 27439, 5329
    .short          32767, 128,   32767, 128
    .short          32767, 128,   32767, 128
*/
	.short          16828, 33038, 6416,  9713
    .short          19070, 28784, 24103, 4680
    .short          32767, 128,   32767, 128
    .short          32767, 128,   32767, 128

asm_function jsimd_\colorid\()_yccnv21_convert_neon
    OUTPUT_WIDTH    .req r0
    INPUT_BUF       .req r1
    OUTPUT_BUF      .req r2
//    OUTPUT_ROW      .req r3
//    NUM_ROWS        .req r4

    OUTPUT_BUF0     .req r5
    OUTPUT_BUF1     .req r6
    OUTPUT_BUF2     .req OUTPUT_BUF

    RGB             .req r7
    Y               .req r8
    U               .req r9
//    V               .req r10
    N               .req ip

    /* Load constants to d0, d1, d2, d3 */
    adr             ip, jsimd_\colorid\()_yccnv21_neon_consts
    vld1.16         {d0, d1, d2, d3}, [ip, :128]

    /* Save ARM registers and handle input arguments */
    push            {/*r4,*/ r5, r6, r7, r8, r9, r10, lr}
//    ldr             NUM_ROWS, [sp, #(4 * 8)]
    ldr             OUTPUT_BUF0, [OUTPUT_BUF]
    ldr             OUTPUT_BUF1, [OUTPUT_BUF, #4]
    ldr             OUTPUT_BUF2, [OUTPUT_BUF, #8]
    .unreq          OUTPUT_BUF

    /* Save NEON registers */
    vpush           {d8-d15}

    /* Outer loop over scanlines */
//    cmp             NUM_ROWS, #1
//    blt             9f
0:
    //ldr             Y, [OUTPUT_BUF0, #0/*OUTPUT_ROW*/, lsl #2]
    //ldr             U, [OUTPUT_BUF1, #0/*OUTPUT_ROW*/, lsl #2]
    ldr             Y, [OUTPUT_BUF0]
    ldr             U, [OUTPUT_BUF1]
    mov             N, OUTPUT_WIDTH
//    ldr             V, [OUTPUT_BUF2]
//    add             OUTPUT_ROW, OUTPUT_ROW, #1
    ldr             RGB, [INPUT_BUF], #4

    /* Inner loop over pixels */
    subs            N, N, #8
    blt             3f
    do_load         \bpp, 8
    do_rgb_to_yuv_stage1
    subs            N, N, #8
    blt             2f
1:
    do_rgb_to_yuv_stage2_store_load_stage1
    subs            N, N, #8
    bge             1b
2:
    do_rgb_to_yuv_stage2
    do_store        8
    tst             N, #7
    beq             8f
3:
    tst             N, #4
    beq             3f
    do_load         \bpp, 4
3:
    tst             N, #2
    beq             4f
    do_load         \bpp, 2
4:
    tst             N, #1
    beq             5f
    do_load         \bpp, 1
5:
    do_rgb_to_yuv
    tst             N, #4
    beq             6f
    do_store        4
6:
    tst             N, #2
    beq             7f
    do_store        2
7:
    tst             N, #1
    beq             8f
    do_store        1
8:
//    subs            NUM_ROWS, NUM_ROWS, #1
//    bgt             0b
9:
    /* Restore all registers and return */
    vpop            {d8-d15}
    pop             {/*r4,*/ r5, r6, r7, r8, r9, r10, pc}

    .unreq          OUTPUT_WIDTH
//    .unreq          OUTPUT_ROW
    .unreq          INPUT_BUF
//    .unreq          NUM_ROWS
    .unreq          OUTPUT_BUF0
    .unreq          OUTPUT_BUF1
    .unreq          OUTPUT_BUF2
    .unreq          RGB
    .unreq          Y
    .unreq          U
//    .unreq          V
    .unreq          N
.endfunc

.purgem do_rgb_to_yuv
.purgem do_rgb_to_yuv_stage1
.purgem do_rgb_to_yuv_stage2
.purgem do_rgb_to_yuv_stage2_store_load_stage1

.endm

/*--------------------------------- id ----- bpp R  G  B */
//generate_jsimd_rgb_yccnv21_convert_neon extrgb,  24, 0, 1, 2
//generate_jsimd_rgb_yccnv21_convert_neon extbgr,  24, 2, 1, 0
generate_jsimd_rgb_yccnv21_convert_neon extrgbx, 32, 0, 1, 2
//generate_jsimd_rgb_yccnv21_convert_neon extbgrx, 32, 2, 1, 0
//generate_jsimd_rgb_yccnv21_convert_neon extxbgr, 32, 3, 2, 1
//generate_jsimd_rgb_yccnv21_convert_neon extxrgb, 32, 1, 2, 3

.purgem do_load
.purgem do_store





/*****************************************************************************/

/*
 * jsimd_extrgb_yccnv21_nouv_convert_neon
 * jsimd_extbgr_yccnv21_nouv_convert_neon
 * jsimd_extrgbx_yccnv21_nouv_convert_neon
 * jsimd_extbgrx_yccnv21_nouv_convert_neon
 * jsimd_extxbgr_yccnv21_nouv_convert_neon
 * jsimd_extxrgb_yccnv21_nouv_convert_neon
 *
 * Colorspace conversion RGB -> YCbCr
 */

.macro do_store size
    .if \size == 8
        vst1.8  {d20}, [Y]!
     .elseif \size == 4
        vst1.8  {d20[0]}, [Y]!
        vst1.8  {d20[1]}, [Y]!
        vst1.8  {d20[2]}, [Y]!
        vst1.8  {d20[3]}, [Y]!
      .elseif \size == 2
        vst1.8  {d20[4]}, [Y]!
        vst1.8  {d20[5]}, [Y]!
     .elseif \size == 1
        vst1.8  {d20[6]}, [Y]!
      .else
        .error unsupported macroblock size
    .endif
.endm

.macro do_load bpp, size
    .if \bpp == 24
        .if \size == 8
            vld3.8  {d10, d11, d12}, [RGB]!
            pld     [RGB, #128]
        .elseif \size == 4
            vld3.8  {d10[0], d11[0], d12[0]}, [RGB]!
            vld3.8  {d10[1], d11[1], d12[1]}, [RGB]!
            vld3.8  {d10[2], d11[2], d12[2]}, [RGB]!
            vld3.8  {d10[3], d11[3], d12[3]}, [RGB]!
        .elseif \size == 2
            vld3.8  {d10[4], d11[4], d12[4]}, [RGB]!
            vld3.8  {d10[5], d11[5], d12[5]}, [RGB]!
        .elseif \size == 1
            vld3.8  {d10[6], d11[6], d12[6]}, [RGB]!
        .else
            .error unsupported macroblock size
        .endif
    .elseif \bpp == 32
        .if \size == 8
            vld4.8  {d10, d11, d12, d13}, [RGB]!
            pld     [RGB, #128]
        .elseif \size == 4
            vld4.8  {d10[0], d11[0], d12[0], d13[0]}, [RGB]!
            vld4.8  {d10[1], d11[1], d12[1], d13[1]}, [RGB]!
            vld4.8  {d10[2], d11[2], d12[2], d13[2]}, [RGB]!
            vld4.8  {d10[3], d11[3], d12[3], d13[3]}, [RGB]!
        .elseif \size == 2
            vld4.8  {d10[4], d11[4], d12[4], d13[4]}, [RGB]!
            vld4.8  {d10[5], d11[5], d12[5], d13[5]}, [RGB]!
        .elseif \size == 1
            vld4.8  {d10[6], d11[6], d12[6], d13[6]}, [RGB]!
        .else
            .error unsupported macroblock size
        .endif
    .else
        .error unsupported bpp
    .endif
.endm

.macro generate_jsimd_rgb_yccnv21_nouv_convert_neon colorid, bpp, r_offs, g_offs, b_offs

/*
 * 2 stage pipelined RGB->YCbCr conversion
 */

.macro do_rgb_to_yuv_stage1
    vmovl.u8    q2, d1\r_offs /* r = { d4, d5 } */
    vmovl.u8    q3, d1\g_offs /* g = { d6, d7 } */
    vmovl.u8    q4, d1\b_offs /* b = { d8, d9 } */
    vmull.u16   q7, d4, d0[0]
    vmlal.u16   q7, d6, d0[1]
    vmlal.u16   q7, d8, d0[2]
    vmull.u16   q8, d5, d0[0]
    vmlal.u16   q8, d7, d0[1]
    vmlal.u16   q8, d9, d0[2]
    vrev64.32   q9,  q1
    vrev64.32   q13, q1
    vmlsl.u16   q9,  d4, d0[3]
    vmlsl.u16   q9,  d6, d1[0]
    vmlal.u16   q9,  d8, d1[1]
    vmlsl.u16   q13, d5, d0[3]
    vmlsl.u16   q13, d7, d1[0]
    vmlal.u16   q13, d9, d1[1]
    vrev64.32   q14, q1
    vrev64.32   q15, q1
    vmlal.u16   q14, d4, d1[1]
    vmlsl.u16   q14, d6, d1[2]
    vmlsl.u16   q14, d8, d1[3]
    vmlal.u16   q15, d5, d1[1]
    vmlsl.u16   q15, d7, d1[2]
    vmlsl.u16   q15, d9, d1[3]
.endm

.macro do_rgb_to_yuv_stage2
    vrshrn.u32  d20, q7,  #16
    vrshrn.u32  d21, q8,  #16
    vshrn.u32   d22, q9,  #16
    vshrn.u32   d23, q13, #16
    vshrn.u32   d24, q14, #16
    vshrn.u32   d25, q15, #16
    vmovn.u16   d20, q10      /* d20 = y */
    // amouse add for add 16 in y
    vmov.u8	d10, #16
    vadd.u8		d20, d20, d10
    /////////////////////////////////
//    vmovn.u16   d21, q11      /* d21 = u */
//    vmovn.u16   d22, q12      /* d22 = v */
.endm

.macro do_rgb_to_yuv
    do_rgb_to_yuv_stage1
    do_rgb_to_yuv_stage2
.endm

.macro do_rgb_to_yuv_stage2_store_load_stage1
      vrshrn.u32  d20, q7,  #16
      vrshrn.u32  d21, q8,  #16
      vshrn.u32   d22, q9,  #16
    vrev64.32   q9,  q1
      vshrn.u32   d23, q13, #16
    vrev64.32   q13, q1
      vshrn.u32   d24, q14, #16
      vshrn.u32   d25, q15, #16
    do_load     \bpp, 8
      vmovn.u16   d20, q10      /* d20 = y */
    vmovl.u8    q2, d1\r_offs   /* r = { d4, d5 } */
    //  vmovn.u16   d21, q11      /* d21 = u */
    vmovl.u8    q3, d1\g_offs   /* g = { d6, d7 } */
    //  vmovn.u16   d22, q12      /* d22 = v */
    vmovl.u8    q4, d1\b_offs   /* b = { d8, d9 } */
    vmull.u16   q7, d4, d0[0]
    vmlal.u16   q7, d6, d0[1]
    vmlal.u16   q7, d8, d0[2]
  	// amouse add for add 16 in y
    vmov.u8	d10, #16
    vadd.u8		d20, d20, d10
    /////////////////////////////////
      vst1.8      {d20}, [Y]!
    vmull.u16   q8, d5, d0[0]
    vmlal.u16   q8, d7, d0[1]
    vmlal.u16   q8, d9, d0[2]
    vmlsl.u16   q9,  d4, d0[3]
    vmlsl.u16   q9,  d6, d1[0]
    vmlal.u16   q9,  d8, d1[1]
    //  vst1.8      {d21}, [U]!
    vmlsl.u16   q13, d5, d0[3]
    vmlsl.u16   q13, d7, d1[0]
    vmlal.u16   q13, d9, d1[1]
    vrev64.32   q14, q1
    vrev64.32   q15, q1
    vmlal.u16   q14, d4, d1[1]
    vmlsl.u16   q14, d6, d1[2]
    vmlsl.u16   q14, d8, d1[3]
    //  vst1.8      {d22}, [V]!
    vmlal.u16   q15, d5, d1[1]
    vmlsl.u16   q15, d7, d1[2]
    vmlsl.u16   q15, d9, d1[3]
.endm

.balign 16
jsimd_\colorid\()_yccnv21_nouv_neon_consts:
/*
    .short          19595, 38470, 7471,  11059
    .short          21709, 32768, 27439, 5329
    .short          32767, 128,   32767, 128
    .short          32767, 128,   32767, 128
*/
	.short          16828, 33038, 6416,  9713
    .short          19070, 28784, 24103, 4680
    .short          32767, 128,   32767, 128
    .short          32767, 128,   32767, 128
asm_function jsimd_\colorid\()_yccnv21_nouv_convert_neon
    OUTPUT_WIDTH    .req r0
    INPUT_BUF       .req r1
    OUTPUT_BUF      .req r2
//    OUTPUT_ROW      .req r3
//    NUM_ROWS        .req r4

    OUTPUT_BUF0     .req r5
    OUTPUT_BUF1     .req r6
    OUTPUT_BUF2     .req OUTPUT_BUF

    RGB             .req r7
    Y               .req r8
//    U               .req r9
//    V               .req r10
    N               .req ip

    /* Load constants to d0, d1, d2, d3 */
    adr             ip, jsimd_\colorid\()_yccnv21_nouv_neon_consts
    vld1.16         {d0, d1, d2, d3}, [ip, :128]

    /* Save ARM registers and handle input arguments */
    push            {/*r4,*/ r5, r6, r7, r8, /*r9, r10,*/ lr}
//    ldr             NUM_ROWS, [sp, #(4 * 8)]
    ldr             OUTPUT_BUF0, [OUTPUT_BUF]
    ldr             OUTPUT_BUF1, [OUTPUT_BUF, #4]
    ldr             OUTPUT_BUF2, [OUTPUT_BUF, #8]
    .unreq          OUTPUT_BUF

    /* Save NEON registers */
    vpush           {d8-d15}

    /* Outer loop over scanlines */
//    cmp             NUM_ROWS, #1
//    blt             9f
0:
//    ldr             Y, [OUTPUT_BUF0, OUTPUT_ROW, lsl #2]
	ldr             Y, [OUTPUT_BUF0]
//    ldr             U, [OUTPUT_BUF1, OUTPUT_ROW, lsl #2]
    mov             N, OUTPUT_WIDTH
//    ldr             V, [OUTPUT_BUF2, OUTPUT_ROW, lsl #2]
//    add             OUTPUT_ROW, OUTPUT_ROW, #1
    ldr             RGB, [INPUT_BUF], #4

    /* Inner loop over pixels */
    subs            N, N, #8
    blt             3f
    do_load         \bpp, 8
    do_rgb_to_yuv_stage1
    subs            N, N, #8
    blt             2f
1:
    do_rgb_to_yuv_stage2_store_load_stage1
    subs            N, N, #8
    bge             1b
2:
    do_rgb_to_yuv_stage2
    do_store        8
    tst             N, #7
    beq             8f
3:
    tst             N, #4
    beq             3f
    do_load         \bpp, 4
3:
    tst             N, #2
    beq             4f
    do_load         \bpp, 2
4:
    tst             N, #1
    beq             5f
    do_load         \bpp, 1
5:
    do_rgb_to_yuv
    tst             N, #4
    beq             6f
    do_store        4
6:
    tst             N, #2
    beq             7f
    do_store        2
7:
    tst             N, #1
    beq             8f
    do_store        1
8:
//    subs            NUM_ROWS, NUM_ROWS, #1
//    bgt             0b
9:
    /* Restore all registers and return */
    vpop            {d8-d15}
    pop             {/*r4,*/ r5, r6, r7, r8, /*r9, r10,*/ pc}

    .unreq          OUTPUT_WIDTH
//    .unreq          OUTPUT_ROW
    .unreq          INPUT_BUF
//    .unreq          NUM_ROWS
    .unreq          OUTPUT_BUF0
    .unreq          OUTPUT_BUF1
    .unreq          OUTPUT_BUF2
    .unreq          RGB
    .unreq          Y
//    .unreq          U
//    .unreq          V
    .unreq          N
.endfunc

.purgem do_rgb_to_yuv
.purgem do_rgb_to_yuv_stage1
.purgem do_rgb_to_yuv_stage2
.purgem do_rgb_to_yuv_stage2_store_load_stage1

.endm

/*--------------------------------- id ----- bpp R  G  B */
//generate_jsimd_rgb_yccnv21_nouv_convert_neon extrgb,  24, 0, 1, 2
//generate_jsimd_rgb_yccnv21_nouv_convert_neon extbgr,  24, 2, 1, 0
generate_jsimd_rgb_yccnv21_nouv_convert_neon extrgbx, 32, 0, 1, 2
//generate_jsimd_rgb_yccnv21_nouv_convert_neon extbgrx, 32, 2, 1, 0
//generate_jsimd_rgb_yccnv21_nouv_convert_neon extxbgr, 32, 3, 2, 1
//generate_jsimd_rgb_yccnv21_nouv_convert_neon extxrgb, 32, 1, 2, 3

.purgem do_load
.purgem do_store
