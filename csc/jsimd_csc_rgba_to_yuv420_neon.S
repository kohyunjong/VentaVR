
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


/*****************************************************************************/

/*
 * qjsimd_extrgb_ycc_h2v1_convert_neon
 * qjsimd_extbgr_ycc_h2v1_convert_neon
 * qjsimd_extrgbx_ycc_h2v1_convert_neon
 * qjsimd_extbgrx_ycc_h2v1_convert_neon
 * qjsimd_extxbgr_ycc_h2v1_convert_neon
 * qjsimd_extxrgb_ycc_h2v1_convert_neon
 *
 * Colorspace conversion RGB -> YCbCr
 */

.macro do_store size
    .if \size == 8
        vst1.8  {d20}, [Y]!
        vst1.8  {d21}, [U]!
        vst1.8  {d22}, [V]!
    .elseif \size == 4
        vst1.8  {d20[0]}, [Y]!
        vst1.8  {d20[1]}, [Y]!
        vst1.8  {d20[2]}, [Y]!
        vst1.8  {d20[3]}, [Y]!
        vst1.8  {d21[0]}, [U]!
        vst1.8  {d21[1]}, [U]!
        vst1.8  {d21[2]}, [U]!
        vst1.8  {d21[3]}, [U]!
        vst1.8  {d22[0]}, [V]!
        vst1.8  {d22[1]}, [V]!
        vst1.8  {d22[2]}, [V]!
        vst1.8  {d22[3]}, [V]!
    .elseif \size == 2
        vst1.8  {d20[4]}, [Y]!
        vst1.8  {d20[5]}, [Y]!
        vst1.8  {d21[4]}, [U]!
        vst1.8  {d21[5]}, [U]!
        vst1.8  {d22[4]}, [V]!
        vst1.8  {d22[5]}, [V]!
    .elseif \size == 1
        vst1.8  {d20[6]}, [Y]!
        vst1.8  {d21[6]}, [U]!
        vst1.8  {d22[6]}, [V]!
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

.macro generate_qjsimd_rgb_ycc_h2v1_convert_neon colorid, bpp, r_offs, g_offs, b_offs

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
	vpaddl.u16	q11,	q11
	vrshr.u32	q11,	q11,	#1	/* rounding and shift */
    vmovn.u16   d21, q11      /* d21 = u */
    vpaddl.u16	q12,	q12
	vrshr.u32	q12,	q12,	#1	/* rounding and shift */
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
	vmovn.u16   d20, q10		/* d20 = y */
	vmovl.u8    q2, d1\r_offs	/* r = { d4, d5 } */
	/*vmovn.u16   d22, q11*/	/* d21 = u */
	vmovl.u8    q3, d1\g_offs	/* g = { d6, d7 } */
	/*vmovn.u16   d24, q12*/	/* d22 = v */
	vmovl.u8    q4, d1\b_offs	/* b = { d8, d9 } */
	vmull.u16   q7, d4, d0[0]
	vmlal.u16   q7, d6, d0[1]
	vmlal.u16   q7, d8, d0[2]
	vst1.8      {d20}, [Y]!
	vmull.u16   q8, d5, d0[0]
	vmlal.u16   q8, d7, d0[1]
	vmlal.u16   q8, d9, d0[2]
	vmlsl.u16   q9,  d4, d0[3]
	vmlsl.u16   q9,  d6, d1[0]
	vmlal.u16   q9,  d8, d1[1]
	
	vpaddl.u16	q11,	q11
	vrshr.u32	q11,	q11,	#1	/* rounding and shift */
	vmovn.u16   d21,	q11			/* 32 to 16 */
	vst1.8      {d21}, [U]!
	
	vmlsl.u16   q13, d5, d0[3]
	vmlsl.u16   q13, d7, d1[0]
	vmlal.u16   q13, d9, d1[1]
	vrev64.32   q14, q1
	vrev64.32   q15, q1
	vmlal.u16   q14, d4, d1[1]
	vmlsl.u16   q14, d6, d1[2]
	vmlsl.u16   q14, d8, d1[3]
	

	vpaddl.u16	q12,	q12
	vrshr.u32	q12,	q12,	#1	/* rounding and shift */
	vmovn.u16   d22,	q12			/* 32 to 16 */
	vst1.8      {d22}, [V]!
	
	
	vmlal.u16   q15, d5, d1[1]
	vmlsl.u16   q15, d7, d1[2]
	vmlsl.u16   q15, d9, d1[3]
	
.endm

.balign 16
qjsimd_\colorid\()_ycc_h2v1_neon_consts:
	.short          19595, 38470, 7471,  11059
	.short          21709, 32768, 27439, 5329
	.short          32767, 128,   32767, 128
	.short          32767, 128,   32767, 128

asm_function qjsimd_\colorid\()_ycc_h2v1_convert_neon
	OUTPUT_WIDTH    .req r0
	INPUT_BUF       .req r1
	OUTPUT_BUF      .req r2
	OUTPUT_ROW      .req r3
	NUM_ROWS        .req r4

	OUTPUT_BUF0     .req r5
	OUTPUT_BUF1     .req r6
	OUTPUT_BUF2     .req OUTPUT_BUF

	RGB             .req r7
	Y               .req r8
	U               .req r9
	V               .req r10
	N               .req ip

	/* Load constants to d0, d1, d2, d3 */
	adr             ip, qjsimd_\colorid\()_ycc_h2v1_neon_consts
	vld1.16         {d0, d1, d2, d3}, [ip, :128]

	/* Save ARM registers and handle input arguments */
	push            {r4, r5, r6, r7, r8, r9, r10, lr}
	ldr             NUM_ROWS, [sp, #(4 * 8)]
	ldr             OUTPUT_BUF0, [OUTPUT_BUF]
	ldr             OUTPUT_BUF1, [OUTPUT_BUF, #4]
	ldr             OUTPUT_BUF2, [OUTPUT_BUF, #8]
	.unreq          OUTPUT_BUF

	/* Save NEON registers */
	vpush           {d8-d15}

	/* Outer loop over scanlines */
	cmp             NUM_ROWS, #1
	blt             9f
0:
	ldr             Y, [OUTPUT_BUF0, OUTPUT_ROW, lsl #2]
	ldr             U, [OUTPUT_BUF1, OUTPUT_ROW, lsl #2]
	mov             N, OUTPUT_WIDTH
	ldr             V, [OUTPUT_BUF2, OUTPUT_ROW, lsl #2]
	add             OUTPUT_ROW, OUTPUT_ROW, #1
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
    subs            NUM_ROWS, NUM_ROWS, #1
    bgt             0b
9:
	/* Restore all registers and return */
	vpop            {d8-d15}
	pop             {r4, r5, r6, r7, r8, r9, r10, pc}

	.unreq          OUTPUT_WIDTH
	.unreq          OUTPUT_ROW
	.unreq          INPUT_BUF
	.unreq          NUM_ROWS
	.unreq          OUTPUT_BUF0
	.unreq          OUTPUT_BUF1
	.unreq          OUTPUT_BUF2
	.unreq          RGB
	.unreq          Y
	.unreq          U
	.unreq          V
	.unreq          N

.endfunc	

.purgem do_rgb_to_yuv
.purgem do_rgb_to_yuv_stage1
.purgem do_rgb_to_yuv_stage2
.purgem do_rgb_to_yuv_stage2_store_load_stage1

.endm

/*--------------------------------- id ----- bpp R  G  B */
generate_qjsimd_rgb_ycc_h2v1_convert_neon extrgb,  24, 0, 1, 2
generate_qjsimd_rgb_ycc_h2v1_convert_neon extbgr,  24, 2, 1, 0
generate_qjsimd_rgb_ycc_h2v1_convert_neon extrgbx, 32, 0, 1, 2
generate_qjsimd_rgb_ycc_h2v1_convert_neon extbgrx, 32, 2, 1, 0
generate_qjsimd_rgb_ycc_h2v1_convert_neon extxbgr, 32, 3, 2, 1
generate_qjsimd_rgb_ycc_h2v1_convert_neon extxrgb, 32, 1, 2, 3

.purgem do_load
.purgem do_store

/*****************************************************************************/




/*****************************************************************************/

asm_function qjsimd_ycc_h2v1_h2v2_convert_neon

	OUTPUT_WIDTH	.req r0
	INPUT_BUF		.req r1
	OUTPUT_BUF		.req r2
	OUTPUT_ROW		.req r3
	NUM_ROWS		.req r4

	OUTPUT_BUF1		.req r5
	OUTPUT_BUF2		.req OUTPUT_BUF
	
	INPUT_BUF1		.req r6
	INPUT_BUF1_2	.req r7
	INPUT_BUF2		.req r8
	INPUT_BUF2_2	.req INPUT_BUF

	IN_UV1			.req r9
	IN_UV2			.req r10
	OUT_UV			.req r11
	N				.req ip

	/* Save ARM registers and handle input arguments */
	push            {r4, r5, r6, r7, r8, r9, r10, r11, lr}
	ldr             NUM_ROWS, [sp, #(4 * 9)]
	ldr             OUTPUT_BUF1, [OUTPUT_BUF]
	ldr             OUTPUT_BUF2, [OUTPUT_BUF, #8]
	.unreq          OUTPUT_BUF

	ldr             INPUT_BUF1_2, [INPUT_BUF, #4]
	ldr             INPUT_BUF1, [INPUT_BUF, #4]
	ldr             INPUT_BUF2, [INPUT_BUF, #8]
	.unreq          INPUT_BUF

	/* Save NEON registers */
	vpush           {d8-d15}

	/* Outer loop over scanlines */
	cmp				NUM_ROWS, #1
	blt				9f

	mov				N, OUTPUT_WIDTH

	ldr				OUT_UV, [OUTPUT_BUF1, OUTPUT_ROW, lsl #2]
	ldr				IN_UV1, [INPUT_BUF1, OUTPUT_ROW, lsl #2]			/* 1st line */
	add             OUTPUT_ROW, OUTPUT_ROW, #1
	ldr				IN_UV2, [INPUT_BUF1_2, OUTPUT_ROW, lsl #2]			/* 2nd line */
	sub             OUTPUT_ROW, OUTPUT_ROW, #1
	
	
	/* load 1st line */
1:	
	vld1.16         {d0, d1, d2, d3}, [IN_UV1, :128]!

	/*add             IN_U, IN_U, #3600*/
	vld1.16         {d4, d5, d6, d7}, [IN_UV2, :128]!
	
	vadd.u16		q0,	q0,	q2
	vadd.u16		q1,	q1,	q3
	
	vrshr.u32		q0,	q0,	#1	/* rounding and shift */
	vrshr.u32		q1,	q1,	#1	/* rounding and shift */
	
	vmovn.u16		d0,	q0
	vmovn.u16		d1,	q1

	/*vmov.u8         d0, #3*/
	vst1.8          {d0, d1}, [OUT_UV]!
	
	subs            N, N, #32
	bge             1b
	



9:
	/* Restore all registers and return */
	vpop            {d8-d15}
	pop             {r4, r5, r6, r7, r8, r9, r10, r11, pc}

	.unreq          OUTPUT_WIDTH
	.unreq          OUTPUT_ROW
	.unreq          NUM_ROWS
	.unreq			INPUT_BUF1
	.unreq			INPUT_BUF1_2
	.unreq			INPUT_BUF2
	.unreq			INPUT_BUF2_2
	.unreq          OUTPUT_BUF1
	.unreq          OUTPUT_BUF2
	.unreq          IN_UV1
	.unreq          IN_UV2
	.unreq          OUT_UV
	.unreq          N
	
.endfunc
/*****************************************************************************/
