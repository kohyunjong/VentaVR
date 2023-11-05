#include <cuda_runtime.h>
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>


extern "C"{
	void cu_RGBtoYUV444(unsigned char *yuv, unsigned char *pData, int width, int height);
	void cu_RGBtoYUV420Pitch(unsigned char *bufY, unsigned char *bufU, unsigned char *bufV, unsigned char *pData, int stride, int width, int height);
	void cu_UVtoNV12(unsigned char *pOut, unsigned char *bufU, unsigned char *bufV, int stride, int width, int height);
	void cu_customCudaMemcpy(unsigned char *yuv, unsigned char *pData, int width, int height, int bytes);
	void cu_RGBtoNV12(unsigned char *pDest, unsigned char *pSrc, int stride, int width, int height);
}
__global__ void convertRGBtoYUV444p(unsigned char *input, unsigned char *output, int dst_width, int dst_height);
__global__ void convertRGBtoYUV420pPitch(unsigned char *input, unsigned char *bufY, unsigned char *bufU, unsigned char *bufV, int stride, int dst_width, int dst_height);
__global__ void convertUVtoNV12(unsigned char *pOut, unsigned char *bufU, unsigned char *bufV, int stride, int width, int height);
__global__ void customCudaMemcpy(unsigned char *input, unsigned char *output, int dst_width, int dst_height, int bytes);
__global__ void convertRGBtoNV12(unsigned char *output, unsigned char *input, int stride, int dst_width, int dst_height);

void cu_RGBtoYUV444(unsigned char *yuv, unsigned char *pData, int width, int height)
{
	dim3 dimblock(8, 8);
	dim3 dimgrid(
		(width + dimblock.x - 1) / dimblock.x,
		(height + dimblock.y - 1) / dimblock.y);

	convertRGBtoYUV444p << <dimgrid, dimblock >> >(pData, yuv, width, height);
}

__global__ void convertRGBtoYUV444p(unsigned char *input, unsigned char *output, int dst_width, int dst_height)
{
	int RGBcomponentCount = 3;
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
	int dst_size = dst_width * dst_height;

	unsigned int R = input[(x + y * dst_width) * RGBcomponentCount];
	unsigned int G = input[(x + y * dst_width) * RGBcomponentCount + 1];
	unsigned int B = input[(x + y * dst_width) * RGBcomponentCount + 2];

	float Y = ((0.299 * R + 0.587 * G + 0.114 * B) * 219 / 255) + 16; if (Y >= 255) Y = 255;
	float U = ((-0.299 * R - 0.587 * G + 0.886 * B) * 224 / 1.772 / 255) + 128; if (U >= 255) U = 255;
	float V = ((0.701 * R - 0.587 * G - 0.114 * B) * 224 / 1.402 / 255) + 128; if (V >= 255) V = 255;

	output[x + y * dst_width] = (unsigned char)Y;
	output[dst_size + (x + y * dst_width)] = (unsigned char)U;
	output[(dst_size * 2) + (x + y * dst_width)] = (unsigned char)V;
}

void cu_customCudaMemcpy(unsigned char *yuv, unsigned char *pData, int width, int height, int bytes)
{
	dim3 dimblock(8, 8);
	dim3 dimgrid(
		(width + dimblock.x - 1) / dimblock.x,
		(height + dimblock.y - 1) / dimblock.y);

	customCudaMemcpy << <dimgrid, dimblock >> >(pData, yuv, width, height, bytes);
}

__global__ void customCudaMemcpy(unsigned char *input, unsigned char *output, int dst_width, int dst_height, int bytes)
{
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

	output[(x + y * dst_width) * bytes] = input[(x + y * dst_width) * bytes];
	output[(x + y * dst_width) * bytes + 1] = input[(x + y * dst_width) * bytes + 1];
	output[(x + y * dst_width) * bytes + 2] = input[(x + y * dst_width) * bytes + 2];
}

void cu_RGBtoNV12(unsigned char *pDest, unsigned char *pSrc, int stride, int width, int height)
{
	dim3 dimblock(8, 8);
	dim3 dimgrid(
		(width + dimblock.x - 1) / dimblock.x,
		(height + dimblock.y - 1) / dimblock.y);

	convertRGBtoNV12 << <dimgrid, dimblock >> >(pDest, pSrc, stride, width, height);
}

__global__ void convertRGBtoNV12(unsigned char *output, unsigned char *input, int stride, int dst_width, int dst_height)
{
	int RGBcomponentCount = 3;
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
	int dst_size = stride * dst_height;

	// load RGB values
	unsigned int R = input[(x + y * dst_width) * RGBcomponentCount];
	unsigned int G = input[(x + y * dst_width) * RGBcomponentCount + 1];
	unsigned int B = input[(x + y * dst_width) * RGBcomponentCount + 2];

	float Y = ((0.299 * R + 0.587 * G + 0.114 * B) * 219 / 255) + 16; if (Y >= 255) Y = 255;
	float U = ((-0.299 * R - 0.587 * G + 0.886 * B) * 224 / 1.772 / 255) + 128; if (U >= 255) U = 255;
	float V = ((0.701 * R - 0.587 * G - 0.114 * B) * 224 / 1.402 / 255) + 128; if (V >= 255) V = 255;

	output[x + y * stride] = Y;
	output[((x / 2) * 2) + ((y / 2) * stride) + (stride * dst_height)] = U;
	output[((x / 2) * 2) + 1 + ((y / 2) * stride) + (stride * dst_height)] = V;
}

void cu_RGBtoYUV420Pitch(unsigned char *bufY, unsigned char *bufU, unsigned char *bufV, unsigned char *pData, int stride, int width, int height)
{
	dim3 dimblock(8, 8);
	dim3 dimgrid(
		(width + dimblock.x - 1) / dimblock.x,
		(height + dimblock.y - 1) / dimblock.y);

	convertRGBtoYUV420pPitch << <dimgrid, dimblock >> >(pData, bufY, bufU, bufV, stride, width, height);
}

__global__ void convertRGBtoYUV420pPitch(unsigned char *input, unsigned char *bufY, unsigned char *bufU, unsigned char *bufV, int stride, int dst_width, int dst_height)
{
	int RGBcomponentCount = 3;
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;
	int dst_size = stride * dst_height;

	// load RGB values
	unsigned int R = input[(x + y * dst_width) * RGBcomponentCount];
	unsigned int G = input[(x + y * dst_width) * RGBcomponentCount + 1];
	unsigned int B = input[(x + y * dst_width) * RGBcomponentCount + 2];

	float Y = ((0.299 * R + 0.587 * G + 0.114 * B) * 219 / 255) + 16; if (Y >= 255) Y = 255;
	float U = ((-0.299 * R - 0.587 * G + 0.886 * B) * 224 / 1.772 / 255) + 128; if (U >= 255) U = 255;
	float V = ((0.701 * R - 0.587 * G - 0.114 * B) * 224 / 1.402 / 255) + 128; if (V >= 255) V = 255;

	bufY[x + y * stride] = Y;
	bufU[(x / 2) + ((y / 2) * (dst_width / 2))] = U;
	bufV[(x / 2) + ((y / 2) * (dst_width / 2))] = V;
}


void cu_UVtoNV12(unsigned char *pOut, unsigned char *bufU, unsigned char *bufV, int stride, int width, int height)
{
	dim3 dimblock(8, 8);
	dim3 dimgrid(
		(width + dimblock.x - 1) / dimblock.x,
		(height + dimblock.y - 1) / dimblock.y);

	convertUVtoNV12 << <dimgrid, dimblock >> >(pOut, bufU, bufV, stride, width, height);
}

__global__ void convertUVtoNV12(unsigned char *pOut, unsigned char *bufU, unsigned char *bufV, int stride, int width, int height)
{
	unsigned int x = blockIdx.x * blockDim.x + threadIdx.x;
	unsigned int y = blockIdx.y * blockDim.y + threadIdx.y;

	pOut[(x * 2) + y * stride * 2] = bufU[x + y * width];
	pOut[((x * 2) + 1) + y * stride * 2] = bufV[x + y * width];
}
