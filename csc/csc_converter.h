
#ifndef _QURAM_CSC_CONVERTER_
#define _QURAM_CSC_CONVERTER_

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

void nv12_to_rgba8888(int width, int height, int *input_cs, int *output_cs);
void nv12_to_rgba8888_709(int width, int height, int *input_cs, int *output_cs);
void nv12_to_bgra8888(int width, int height, int *input_cs, int *output_cs);

void nv21_to_rgba8888(int width, int height, int *input_cs, int *output_cs);
void nv21_to_rgba8888_709(int width, int height, int *input_cs, int *output_cs);
void nv21_to_bgra8888(int width, int height, int *input_cs, int *output_cs);

void nv12_to_rgb888(int width, int height, int *input_cs, int *output_cs);
void nv21_to_rgb888(int width, int height, int *input_cs, int *output_cs);

void y_uv_nv21_to_rgba8888(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs);
void y_uv_nv12_to_rgba8888(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs);

void rgba8888_to_nv12(int width, int height, int *input_cs, int *output_cs);
void rgba8888_to_nv21(int width, int height, int *input_cs, int *output_cs);
void rgba8888_to_yuv420(int width, int height, int *input_cs, int *output_cs);
void rgba8888_to_rgb565(int width, int height, int *input_cs, unsigned short *output_cs);

void yuv_nv_reverse(int width, int height, int *input_cs, int *output_cs);

void yuv420_to_nv12(int width, int height, int *input_cs, int *output_cs);
void yuv420_to_nv21(int width, int height, int *input_cs, int *output_cs);
void yuv420_to_rgba8888(int width, int height, int *input_cs, int *output_cs);
void yuv420_to_rgba8888_709(int width, int height, int *input_cs, int *output_cs);
void yuv420_to_bgra8888(int width, int height, int *input_cs, int *output_cs);

#ifdef __cplusplus
}
#endif  /* __cplusplus */


#endif /*_QURAM_CSC_CONVERTER_*/
