
#include "csc_converter.h"

#if HAVE_NEON
#include <arm_neon.h>
#endif

#include "../WINK_Includes/QuramThreadpool.h"

#ifndef ANDROID
#include <stdio.h>
#include <string.h>
#define jstat_printf
#else
#include <android/log.h>
#define jstat_printf(x...)  __android_log_print(ANDROID_LOG_INFO,"CSC_DEBUG",x)
#endif

#define QURAM_USE_THREADPOOL	1

#if 0
#define		YCBCR_TO_RED(y, cb, cr)		(y) + ((91881 * (cr))>>16)
#define		YCBCR_TO_GREEN(y, cb, cr)	(y) + ((-22554 * (cb) + -46802 *(cr))>>16)
#define		YCBCR_TO_BLUE(y, cb, cr)	(y) + ((116130 * (cb))>>16)
#else
// bt.601
#define		YCBCR_TO_RED(y, cb, cr)		( (1192*((y)-16) + (cr*1634)) >> 10  )
#define		YCBCR_TO_GREEN(y, cb, cr)	( (1192*((y)-16) - (cb*400) - (cr*833)) >> 10)
#define		YCBCR_TO_BLUE(y, cb, cr)	( (1192*((y)-16) + (cb*2066)) >> 10)

// bt.709
#define		YCBCR_TO_RED_709(y, cb, cr)		( (1192*((y)-16) + (cr*1836)) >> 10  )
#define		YCBCR_TO_GREEN_709(y, cb, cr)	( (1192*((y)-16) - (cb*218) - (cr*545)) >> 10)
#define		YCBCR_TO_BLUE_709(y, cb, cr)	( (1192*((y)-16) + (cb*2162)) >> 10)
#endif
int clip( int x )
{
	if( x < 0 ) return 0;
	else if( x > 255 ) return 255;
	else return x;
}

#if QURAM_USE_THREADPOOL
typedef struct _quram_csc_info_to_yuv
{
	int		width;
	int		height;
	unsigned char*	input_buf[2];
	unsigned char***	output_buf;
} quram_csc_info_to_yuv;

typedef struct _quram_csc_info_to_rgba
{
	int		width;
	int		height;
	unsigned char*	output_buf[2];
	unsigned char***	input_buf;
} quram_csc_info_to_rgba;

typedef struct _quram_csc_info_to_rgb
{
	int		width;
	int		height;
	unsigned char*	output_buf;
	unsigned char*	input_buf;
} quram_csc_info_to_rgb;

#define CSC_TP_UNIT_HEIGHT		32
#define CSC_TP_THREAD_COUNT		4
#endif

void _rgba8888_to_yuv420_sg(int width, int height, int *input_cs, int *output_cs);
void _rgba8888_to_nv12_sg(int width, int height, int *input_cs, int *output_cs);
void _rgba8888_to_nv21_sg(int width, int height, int *input_cs, int *output_cs);
void _nv12_to_rgba8888_sg(int width, int height, int *input_cs, int *output_cs);
void _nv12_to_rgba8888_709_sg(int width, int height, int *input_cs, int *output_cs);
void _nv12_to_bgra8888_sg(int width, int height, int *input_cs, int *output_cs);
void _y_uv_nv12_to_rgba8888_sg(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs);
void _nv12_to_rgb888_sg(int width, int height, int *input_cs, int *output_cs);
void _nv21_to_rgba8888_sg(int width, int height, int *input_cs, int *output_cs);
void _nv21_to_rgba8888_709_sg(int width, int height, int *input_cs, int *output_cs);
void _nv21_to_bgra8888_sg(int width, int height, int *input_cs, int *output_cs);
void _y_uv_nv21_to_rgba8888_sg(int width, int height, int *input_cs, int *input_uv_cs, int *output_cs);
void _nv21_to_rgb888_sg(int width, int height, int *input_cs, int *output_cs);
void _yuv420_to_rgba8888_sg(int width, int height, unsigned char* input_y, unsigned char* input_u, unsigned char* input_v, int *output_cs);
void _yuv420_to_rgba8888_709_sg(int width, int height, int *input_cs, int *output_cs);
void _yuv420_to_bgra8888_sg(int width, int height, int *input_cs, int *output_cs);
void _yuv_nv_reverse_sg(int width, int height, int *input_cs, int *output_cs);
void _yuv420_to_nv12_sg(int width, int height, int *input_cs, int *output_cs);
void _yuv420_to_nv21_sg(int width, int height, int *input_cs, int *output_cs);

#if (HAVE_NEON || HAVE_NEON_64)
extern void qjsimd_extrgbx_ycc_h2v1_convert_neon ( unsigned int img_width,
              unsigned char** input_buf,  unsigned char*** output_buf,
             unsigned int output_row, int num_rows);

extern void qjsimd_ycc_h2v1_h2v2_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char*** output_buf,
             unsigned int output_row, int num_rows);

extern void jsimd_extrgbx_yccnv12_convert_neon( unsigned int img_width,
              unsigned char** input_buf,  unsigned char*** output_buf);

extern void jsimd_extrgbx_yccnv12_nouv_convert_neon( unsigned int img_width,
              unsigned char** input_buf,  unsigned char*** output_buf);

extern void jsimd_extrgbx_yccnv21_convert_neon( unsigned int img_width,
              unsigned char** input_buf,  unsigned char*** output_buf);

extern void jsimd_extrgbx_yccnv21_nouv_convert_neon( unsigned int img_width,
              unsigned char** input_buf,  unsigned char*** output_buf);

extern void jsimd_ycc_nv12_extrgbx_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv12_extrgbx_single_line_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv12_extrgbx_709_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv12_extrgbx_single_line_709_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv12_extrgb_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv12_extrgb_single_line_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv21_extrgbx_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv21_extrgbx_single_line_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv21_extrgbx_709_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv21_extrgbx_single_line_709_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv21_extrgb_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_nv21_extrgb_single_line_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_h2v2_extrgbx_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_h2v1_extrgbx_convert_neon( unsigned int img_width,
              unsigned char*** input_buf, unsigned int input_row, unsigned char** output_buf, unsigned int nom_row);

extern void jsimd_ycc_h2v2_extrgbx_709_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_h2v1_extrgbx_709_convert_neon( unsigned int img_width,
              unsigned char*** input_buf, unsigned int input_row, unsigned char** output_buf, unsigned int nom_row);

extern void jsimd_ycc_h2v2_extbgrx_convert_neon( unsigned int img_width,
              unsigned char*** input_buf,  unsigned char** output_buf);

extern void jsimd_ycc_h2v1_extbgrx_convert_neon( unsigned int img_width,
              unsigned char*** input_buf, unsigned int input_row, unsigned char** output_buf, unsigned int nom_row);

extern void* memcpy_neon(void* dst, void* src, unsigned int size);

#endif

void rgba8888_to_yuv420(int width, int height, int *input_cs, int *output_cs)
{
	{
		_rgba8888_to_yuv420_sg( width, height, input_cs, output_cs );
	}
}

void _rgba8888_to_yuv420_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0, j = 0;
	unsigned char *y = (unsigned char*)output_cs;
	unsigned char *u;
	unsigned char *v;

	unsigned char *input_buf[2] = {0, };

#if HAVE_NEON
	unsigned char ***output_buf;
	unsigned char ***output_samp_buf;

	unsigned char* uv_buf = (unsigned char*)memalign(64, width*2*2);

	int uv_wdt = (width+1)>>1;
	int uv_hgt = (height+1)>>1;

	unsigned char* out_u = y + width * height;
	unsigned char* out_v = out_u + (uv_wdt*uv_hgt);

	unsigned char *org_u;
	unsigned char *org_v;

	int pad = width % 128;

	u = uv_buf;
	v = u + (width * 2);

	output_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	output_samp_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	input_buf[0] = (unsigned char*)input_cs;

	for(i=0; i<4; i++ )
	{
		output_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		output_samp_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	output_buf[0][0] = y;
	output_buf[1][0] = u;
	output_buf[2][0] = v;
	output_buf[3][0] = 0;

	for(i=0; i< height/2 ; i++ )
	{
		// 1st line
		qjsimd_extrgbx_ycc_h2v1_convert_neon( width, input_buf, output_buf, 0, 1 );

		input_buf[0] += (width<<2);

		output_buf[0][0] += width;
		output_buf[1][0] += width;
		output_buf[2][0] += width;

		// 2nd line
		qjsimd_extrgbx_ycc_h2v1_convert_neon( width, input_buf, output_buf, 0, 1 );

		input_buf[0] += (width<<2);
		
		output_buf[0][0] += width;

		output_buf[1][0] = u;
		output_buf[2][0] = v;

		if(pad)
		{
			for(j=0; j< (width+1)>>1; j++)
			{
				*(out_u + uv_wdt*(i) + j) = (*(output_buf[1][0]+j*2) + *(output_buf[1][0]+j*2 + width) + 1) / 2;
				*(out_v + uv_wdt*(i) + j) = (*(output_buf[2][0]+j*2) + *(output_buf[2][0]+j*2 + width) + 1) / 2;
			}
		}
		else
		{
			// u sampling 
			output_samp_buf[0][0] = out_u + uv_wdt*(i); 
			output_samp_buf[1][0] = u;
			output_samp_buf[1][1] = u + width;

			qjsimd_ycc_h2v1_h2v2_convert_neon(width, output_samp_buf, output_samp_buf, 0, 1);

			// v sampling 
			output_samp_buf[0][0] = out_v + uv_wdt*(i); 
			output_samp_buf[1][0] = v;
			output_samp_buf[1][1] = v + width;

			qjsimd_ycc_h2v1_h2v2_convert_neon(width, output_samp_buf, output_samp_buf, 0, 1);
		}
	}
	
	if(height%2)
	{
		output_buf[1][0] = u;
		output_buf[2][0] = v;

		qjsimd_extrgbx_ycc_h2v1_convert_neon( width, input_buf, output_buf, 0, 1 );

		// sampling
		for(j=0; j< (width+1)>>1; j++)
		{
			*(out_u + uv_wdt*(i) + j) = *(output_buf[1][0]+j*2);
			*(out_v + uv_wdt*(i) + j) = *(output_buf[2][0]+j*2);
		}
	}

	free(uv_buf);

	for(i=0; i<4; i++ )
	{
		free(output_buf[i]);
		free(output_samp_buf[i]);
	}

	free(output_buf);
	free(output_samp_buf);
	
#else
	unsigned char *outY = (unsigned char*)output_cs;
	unsigned char *outU = outY + (width * height);
	unsigned char *outV = outU + ((width+1)>>1) * ((height+1)>>1);
	unsigned char *rgba = (unsigned char*)input_cs;

	int r, g, b;

	unsigned char *inptr = rgba;

	u = y + width * height;
	v = u + ((width+1)>>1) * ((height+1)>>1);

	for (i = 0; i < height;)
	{
		unsigned char *outYptr = outY + width * i;
		unsigned char *outUptr = outU + ((width+1)>>1) * i/2;
		unsigned char *outVptr = outV + ((width+1)>>1) * i/2;

		for(j=0;j<(width>>1); j++) {

			r = (*inptr++);
			g = (*inptr++);
			b = (*inptr++);
			inptr++;

			*(outYptr++) = ( ( (1225*r) + (2404*g) + (467*b) )>>12 );
			*(outUptr++) = ( ( (-691*r) - (1357*g) + (b<<11) )>>12 ) +128;	// u
			*(outVptr++) = ( ( (r<<11) - (1715*g) - (333*b) )>>12 ) + 128;	// v

			r = (*inptr++);
			g = (*inptr++);
			b = (*inptr++);
			inptr++;

			*(outYptr++) = ( ( (1225*r) + (2404*g) + (467*b) )>>12 );
		}

		if(width & 1)
		{
			r	= (*inptr++);
			g	= (*inptr++);
			b	= (*inptr++);
			inptr++;

			*(outYptr++) = ( ( (1225*r) + (2404*g) + (467*b) )>>12 );
			*(outUptr++) = ( ( (-691*r) - (1357*g) + (b<<11) )>>12 ) +128;	// u
			*(outVptr++) = ( ( (r<<11) - (1715*g) - (333*b) )>>12 ) + 128;	// v
		}

		i++;

		if(i < height)
		{
			outYptr	= outY + width * i;

			for(j=0;j<(width); j++) {
				r	= (*inptr++);
				g	= (*inptr++);
				b	= (*inptr++);
				inptr++;

				*(outYptr++) = ( ( (1225*r) + (2404*g) + (467*b) )>>12 );
			}
		}
		i++;
	}
#endif
}

//#if QURAM_USE_THREADPOOL
void _rgba8888_to_nv12_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_yuv* arg = (quram_csc_info_to_yuv*)argv;

	int alignWdt = ((arg->width+1)>>1)<<1;

#if HAVE_NEON
	for(i=0; i< arg->height - 1 ; i+=2 )
	{
		jsimd_extrgbx_yccnv12_convert_neon( arg->width, arg->input_buf, arg->output_buf);

		arg->output_buf[0][0] += arg->width;
		arg->output_buf[1][0] += (alignWdt);
		arg->input_buf[0] += (arg->width<<2);

		jsimd_extrgbx_yccnv12_nouv_convert_neon( arg->width, arg->input_buf, arg->output_buf);

		arg->output_buf[0][0] += arg->width;
		arg->input_buf[0] += (arg->width<<2);
	}
	if( arg->height - i == 1 )
		jsimd_extrgbx_yccnv12_convert_neon( arg->width, arg->input_buf, arg->output_buf);
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* input = arg->input_buf[0];
	unsigned char* y = arg->output_buf[0][0];
	unsigned char *u = arg->output_buf[1][0];//y + width * height;

	int j = 0;

	int red, green, blue;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			red		= (*input++);
			green	= (*input++);
			blue	= (*input++);
			(input++);

			*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;

			red		= (*input++);
			green	= (*input++);
			blue	= (*input++);
			(input++);

			*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
		}

		if(width & 1)
		{
			red		= (*input++);
			green	= (*input++);
			blue	= (*input++);
			(input++);

			*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				red		= (*input++);
				green	= (*input++);
				blue	= (*input++);
				(input++);

				*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );

				red		= (*input++);
				green	= (*input++);
				blue	= (*input++);
				(input++);

				*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}

			if(width & 1)
			{
				red		= (*input++);
				green	= (*input++);
				blue	= (*input++);
				(input++);

				*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}
		}
	}
#endif
}

void _rgba8888_to_nv12_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)output_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	unsigned char*	input_cs_offset = (unsigned char*)input_cs;
	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_yuv**	arg = (quram_csc_info_to_yuv**)malloc( sizeof(quram_csc_info_to_yuv*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_rgba8888_to_nv12_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_yuv*)malloc( sizeof(quram_csc_info_to_yuv) );
		arg[i]->output_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->output_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->input_buf[0]		= input_cs_offset + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->output_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->output_buf[2][0]	= v;
		arg[i]->output_buf[3][0]	= 0;

		quram_threadpool_add_task( threadpool, _rgba8888_to_nv12_tp, arg[i], 0);
		//_rgba8888_to_nv12_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_yuv*)malloc( sizeof(quram_csc_info_to_yuv) );
	arg[i]->output_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->output_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->input_buf[0]		= input_cs_offset + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->input_buf[1]		= input_cs_offset + (1 * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->output_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->output_buf[2][0]	= v;
	arg[i]->output_buf[3][0]	= 0;

	quram_threadpool_add_task( threadpool, _rgba8888_to_nv12_tp, arg[i], 0);
	//_rgba8888_to_nv12_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->output_buf[j] );
		}
		free( arg[i]->output_buf );
		free( arg[i] );
	}
	free( arg );
}
//#else
void _rgba8888_to_nv12_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0, j = 0;
	unsigned char *y0 = (unsigned char*)output_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *input_buf[2] = {0, };

#if HAVE_NEON
	unsigned char ***output_buf;

	output_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	input_buf[0] = (unsigned char*)input_cs;
	input_buf[1] = input_buf[0] + width * 4;

	for(i=0; i<4; i++ )
	{
		output_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	output_buf[0][0] = y0;
	output_buf[1][0] = u;
	output_buf[2][0] = v;
	output_buf[3][0] = 0;

	for(i=0; i< height-1 ; i+=2 )
	{
		jsimd_extrgbx_yccnv12_convert_neon( width, input_buf, output_buf);

		output_buf[0][0] += width;
		output_buf[1][0] += (alignWdt);
		input_buf[0] += (width<<2);

		jsimd_extrgbx_yccnv12_nouv_convert_neon( width, input_buf, output_buf);

		output_buf[0][0] += width;
		input_buf[0] += (width<<2);
	}

	if( height - i == 1 )
	{
		jsimd_extrgbx_yccnv12_convert_neon( width, input_buf, output_buf);
	}

	for(i=0; i<4; i++ )
	{
		free(output_buf[i]);
	}

	free(output_buf);
#else
	int red, green, blue;

	input_buf[0] = (unsigned char*)input_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			red		= (*input_buf[0]++);
			green	= (*input_buf[0]++);
			blue	= (*input_buf[0]++);
			(input_buf[0]++);

			*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;	// u
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;	// v

			red		= (*input_buf[0]++);
			green	= (*input_buf[0]++);
			blue	= (*input_buf[0]++);
			(input_buf[0]++);

			*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
		}

		if(width & 1)
		{
			red		= (*input_buf[0]++);
			green	= (*input_buf[0]++);
			blue	= (*input_buf[0]++);
			(input_buf[0]++);

			*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;	// u
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;	// v
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				red		= (*input_buf[0]++);
				green	= (*input_buf[0]++);
				blue	= (*input_buf[0]++);
				(input_buf[0]++);

				*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );

				red		= (*input_buf[0]++);
				green	= (*input_buf[0]++);
				blue	= (*input_buf[0]++);
				(input_buf[0]++);

				*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}

			if(width & 1)
			{
				red		= (*input_buf[0]++);
				green	= (*input_buf[0]++);
				blue	= (*input_buf[0]++);
				(input_buf[0]++);

				*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}
		}
	}
#endif
}
//#endif

void rgba8888_to_nv12(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_rgba8888_to_nv12_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_rgba8888_to_nv12_sg( width, height, input_cs, output_cs );
		_rgba8888_to_nv12_tps( width, height, input_cs, output_cs );
	}
}

//#if QURAM_USE_THREADPOOL
void _rgba8888_to_nv21_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_yuv* arg = (quram_csc_info_to_yuv*)argv;

	int alignWdt = ((arg->width+1)>>1)<<1;

#if HAVE_NEON
	for(i=0; i< arg->height-1 ; i+=2 )
	{
		jsimd_extrgbx_yccnv21_convert_neon( arg->width, arg->input_buf, arg->output_buf);
		//jsimd_extrgbx_yccnv21_convert_neon( arg->width, arg->input_buf, arg->output_buf, 0, 1);

		arg->output_buf[0][0] += arg->width;
		arg->output_buf[1][0] += (alignWdt);
		arg->input_buf[0] += (arg->width<<2);

		jsimd_extrgbx_yccnv21_nouv_convert_neon( arg->width, arg->input_buf, arg->output_buf);
		//jsimd_extrgbx_yccnv21_nouv_convert_neon( arg->width, arg->input_buf, arg->output_buf, 0, 1);

		arg->output_buf[0][0] += arg->width;
		arg->input_buf[0] += (arg->width<<2);
	}

	if( arg->height  - i == 1 )
		jsimd_extrgbx_yccnv21_convert_neon( arg->width, arg->input_buf, arg->output_buf);
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* input = arg->input_buf[0];
	unsigned char* y = arg->output_buf[0][0];
	unsigned char *u = arg->output_buf[1][0];

	int j = 0;

	int red, green, blue;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			red		= (*input++);
			green	= (*input++);
			blue	= (*input++);
			(input++);

			*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;	// v
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;	// u

			red		= (*input++);
			green	= (*input++);
			blue	= (*input++);
			(input++);

			*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
		}

		if(width & 1)
		{
			red		= (*input++);
			green	= (*input++);
			blue	= (*input++);
			(input++);

			*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;	// v
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;	// u

		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				red		= (*input++);
				green	= (*input++);
				blue	= (*input++);
				(input++);

				*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );

				red		= (*input++);
				green	= (*input++);
				blue	= (*input++);
				(input++);

				*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}

			if(width & 1)
			{
				red		= (*input++);
				green	= (*input++);
				blue	= (*input++);
				(input++);

				*(y++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}
		}
	}
#endif
}

void _rgba8888_to_nv21_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)output_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	unsigned char*	input_cs_offset = (unsigned char*)input_cs;
	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_yuv**	arg = (quram_csc_info_to_yuv**)malloc( sizeof(quram_csc_info_to_yuv*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_rgba8888_to_nv21_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_yuv*)malloc( sizeof(quram_csc_info_to_yuv) );
		arg[i]->output_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->output_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->input_buf[0]		= input_cs_offset + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->output_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->output_buf[2][0]	= v;
		arg[i]->output_buf[3][0]	= 0;

		quram_threadpool_add_task( threadpool, _rgba8888_to_nv21_tp, arg[i], 0);
		//_rgba8888_to_nv21_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_yuv*)malloc( sizeof(quram_csc_info_to_yuv) );
	arg[i]->output_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->output_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->input_buf[0]		= input_cs_offset + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->input_buf[1]		= input_cs_offset + (1 * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->output_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->output_buf[2][0]	= v;
	arg[i]->output_buf[3][0]	= 0;

	quram_threadpool_add_task( threadpool, _rgba8888_to_nv21_tp, arg[i], 0);
	//_rgba8888_to_nv21_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->output_buf[j] );
		}
		free( arg[i]->output_buf );
		free( arg[i] );
	}
	free( arg );
}
//#else
void _rgba8888_to_nv21_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0, j = 0;
	unsigned char *y0 = (unsigned char*)output_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *input_buf[2] = {0, };

#if HAVE_NEON
	unsigned char ***output_buf;

	output_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	input_buf[0] = (unsigned char*)input_cs;
	input_buf[1] = input_buf[0] + width * 4;

	for(i=0; i<4; i++ )
	{
		output_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	output_buf[0][0] = y0;
	output_buf[1][0] = u;
	output_buf[2][0] = v;
	output_buf[3][0] = 0;

	for(i=0; i< height -1 ; i+=2 )
	{
		jsimd_extrgbx_yccnv21_convert_neon( width, input_buf, output_buf);
		
		output_buf[0][0] += width;
		output_buf[1][0] += (alignWdt);
		input_buf[0] += (width<<2);
		
		jsimd_extrgbx_yccnv21_nouv_convert_neon( width, input_buf, output_buf);

		output_buf[0][0] += width;
		input_buf[0] += (width<<2);
	}

	if( height - i == 1 )
	{
		jsimd_extrgbx_yccnv21_convert_neon( width, input_buf, output_buf);
	}
#else
	int red, green, blue;

	input_buf[0] = (unsigned char*)input_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			red		= (*input_buf[0]++);
			green	= (*input_buf[0]++);
			blue	= (*input_buf[0]++);
			(input_buf[0]++);

			*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;	// v
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;	// u

			red		= (*input_buf[0]++);
			green	= (*input_buf[0]++);
			blue	= (*input_buf[0]++);
			(input_buf[0]++);

			*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
		}

		if(width & 1)
		{
			red		= (*input_buf[0]++);
			green	= (*input_buf[0]++);
			blue	= (*input_buf[0]++);
			(input_buf[0]++);

			*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			*(u++)	= ( ( (red<<11) - (1715*green) - (333*blue) )>>12 ) + 128;	// v
			*(u++)	= ( ( (-691*red) - (1357*green) + (blue<<11) )>>12 ) +128;	// u
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				red		= (*input_buf[0]++);
				green	= (*input_buf[0]++);
				blue	= (*input_buf[0]++);
				(input_buf[0]++);

				*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );

				red		= (*input_buf[0]++);
				green	= (*input_buf[0]++);
				blue	= (*input_buf[0]++);
				(input_buf[0]++);

				*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}

			if(width & 1)
			{
				red		= (*input_buf[0]++);
				green	= (*input_buf[0]++);
				blue	= (*input_buf[0]++);
				(input_buf[0]++);

				*(y0++) =  ( ( (1225*red) + (2404*green) + (467*blue) )>>12 );
			}
		}
	}
#endif
}
//#endif

void rgba8888_to_nv21(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_rgba8888_to_nv21_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_rgba8888_to_nv21_sg( width, height, input_cs, output_cs );
		_rgba8888_to_nv21_tps( width, height, input_cs, output_cs );
	}
}

void _nv12_to_rgba8888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if HAVE_NEON

	if(arg->width & 1)
	{
		int width = arg->width;
		int height = arg->height;
		
		unsigned char* y0 = arg->input_buf[0][0];
		unsigned char* u = arg->input_buf[1][0];
		//unsigned char *v = arg->input_buf[2][0];

		int j = 0;

		int yVal, cbVal, crVal;
		int red, green, blue;

		//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
		unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		for(i=0; i< arg->height-1 ; i+=2 )
		{
			jsimd_ycc_nv12_extrgbx_convert_neon( arg->width, arg->input_buf, arg->output_buf);

			arg->input_buf[0][0] += arg->width<<1;
			arg->input_buf[1][0] += arg->width;
			arg->input_buf[2][0] += arg->width;
			arg->input_buf[3][0] += arg->width<<1;
			arg->output_buf[0] += (arg->width<<2) <<1;
			arg->output_buf[1] += (arg->width<<2) <<1;
		}

		if( arg->height - i ==  1 )
			jsimd_ycc_nv12_extrgbx_single_line_convert_neon( arg->width, arg->input_buf, arg->output_buf);
	}

#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv12_to_rgba8888_709_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if HAVE_NEON

	if(arg->width & 1)
	{
		int width = arg->width;
		int height = arg->height;
		
		unsigned char* y0 = arg->input_buf[0][0];
		unsigned char* u = arg->input_buf[1][0];
		//unsigned char *v = arg->input_buf[2][0];

		int j = 0;

		int yVal, cbVal, crVal;
		int red, green, blue;

		//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
		unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		for(i=0; i< arg->height-1 ; i+=2 )
		{
			jsimd_ycc_nv12_extrgbx_709_convert_neon( arg->width, arg->input_buf, arg->output_buf);

			arg->input_buf[0][0] += arg->width<<1;
			arg->input_buf[1][0] += arg->width;
			arg->input_buf[2][0] += arg->width;
			arg->input_buf[3][0] += arg->width<<1;
			arg->output_buf[0] += (arg->width<<2) <<1;
			arg->output_buf[1] += (arg->width<<2) <<1;
		}

		if( arg->height - i ==  1 )
			jsimd_ycc_nv12_extrgbx_single_line_709_convert_neon( arg->width, arg->input_buf, arg->output_buf);
	}

#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv12_to_bgra8888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if 0//HAVE_NEON

	if(arg->width & 1)
	{
		int width = arg->width;
		int height = arg->height;
		
		unsigned char* y0 = arg->input_buf[0][0];
		unsigned char* u = arg->input_buf[1][0];
		//unsigned char *v = arg->input_buf[2][0];

		int j = 0;

		int yVal, cbVal, crVal;
		int red, green, blue;

		//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
		unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}
			}
		}
	}
	else
	{
		for(i=0; i< arg->height-1 ; i+=2 )
		{
			jsimd_ycc_nv12_extbgrx_convert_neon( arg->width, arg->input_buf, arg->output_buf);

			arg->input_buf[0][0] += arg->width<<1;
			arg->input_buf[1][0] += arg->width;
			arg->input_buf[2][0] += arg->width;
			arg->input_buf[3][0] += arg->width<<1;
			arg->output_buf[0] += (arg->width<<2) <<1;
			arg->output_buf[1] += (arg->width<<2) <<1;
		}

		if( arg->height - i ==  1 )
			jsimd_ycc_nv12_extbgrx_single_line_convert_neon( arg->width, arg->input_buf, 0, arg->output_buf, 1 );
	}

#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}
		}
	}
#endif
}

void _nv12_to_rgb888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if HAVE_NEON
	for(i=0; i< arg->height-1 ; i+=2 )
	{
		jsimd_ycc_nv12_extrgb_convert_neon( arg->width, arg->input_buf, arg->output_buf);

		arg->input_buf[0][0] += arg->width<<1;
		arg->input_buf[1][0] += arg->width;
		arg->input_buf[2][0] += arg->width;
		arg->input_buf[3][0] += arg->width<<1;
		arg->output_buf[0] += (arg->width*3) <<1;
		arg->output_buf[1] += (arg->width*3) <<1;
	}

	if( arg->height - i ==  1 )
		jsimd_ycc_nv12_extrgb_single_line_convert_neon( arg->width, arg->input_buf, arg->output_buf);
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;

	unsigned char* outBuf = (unsigned char*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				*(outBuf++)		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv12_to_rgba8888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv12_to_rgba8888_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv12_to_rgba8888_tp, arg[i], 0);
		//_nv12_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv12_to_rgba8888_tp, arg[i], 0);
	//_nv12_to_rgba8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv12_to_rgba8888_709_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv12_to_rgba8888_709_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv12_to_rgba8888_709_tp, arg[i], 0);
		//_nv12_to_rgba8888_709_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv12_to_rgba8888_709_tp, arg[i], 0);
	//_nv12_to_rgba8888_709_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv12_to_bgra8888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv12_to_bgra8888_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv12_to_bgra8888_tp, arg[i], 0);
		//_nv12_to_bgra8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv12_to_bgra8888_tp, arg[i], 0);
	//_nv12_to_bgra8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _y_uv_nv12_to_rgba8888_tps(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_y_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = (unsigned char*)input_uv_cs;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_y_uv_nv12_to_rgba8888_sg( width, height, input_y_cs, input_uv_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv12_to_rgba8888_tp, arg[i], 0);
		//_nv12_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv12_to_rgba8888_tp, arg[i], 0);
	//_nv12_to_rgba8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv12_to_rgb888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv12_to_rgb888_sg( width, height, input_cs, output_cs );;
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 3);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 3;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv12_to_rgb888_tp, arg[i], 0);
		//_nv12_to_rgb888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 3);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 3;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv12_to_rgb888_tp, arg[i], 0);
	//_nv12_to_rgb888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv12_to_rgba8888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
#if HAVE_NEON

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv12_extrgbx_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv12_extrgbx_single_line_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv12_to_rgba8888_709_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
#if HAVE_NEON

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv12_extrgbx_709_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv12_extrgbx_single_line_709_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv12_to_bgra8888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
#if 0//HAVE_NEON

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv12_extbgrx_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv12_extbgrx_single_line_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}
		}
	}
#endif
}

void _y_uv_nv12_to_rgba8888_sg(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_y_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = (unsigned char*)input_uv_cs;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

#if HAVE_NEON

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u+j)-128;
				crVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					cbVal = *(u++)-128;
					crVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv12_extrgbx_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv12_extrgbx_single_line_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv12_to_rgb888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

#if HAVE_NEON
	unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	unsigned char *output_buf[2] = {0, };

	for(i=0; i<4; i++ )
	{
		input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	input_buf[0][0] = y0;
	input_buf[1][0] = u;
	input_buf[2][0] = v;
	input_buf[3][0] = y1;

	output_buf[0] = (unsigned char*)output_cs;
	output_buf[1] = output_buf[0] + width * 3;

	for(i=0; i< height - 1 ; i+=2 )
	{
		jsimd_ycc_nv12_extrgb_convert_neon( width, input_buf, output_buf);
		input_buf[0][0] += width*2;
		input_buf[1][0] += width;
		input_buf[2][0] += width;
		input_buf[3][0] += width*2;
		output_buf[0] += (width * 3) * 2;
		output_buf[1] += (width * 3) * 2;
	}

	if( height - i ==  1 )
		jsimd_ycc_nv12_extrgb_single_line_convert_neon( width, input_buf, output_buf);

	for(i=0; i<4; i++ )
	{
		free(input_buf[i]);
	}

	free(input_buf);
#else
	int j;

	int yVal, cbVal, crVal;
	//int red, green, blue;

	//unsigned int* outBuf = (unsigned int*)output_cs;
	unsigned char* outBuf = (unsigned char*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(u+j+1)-128;

			*(outBuf++) = (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void nv12_to_rgba8888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv12_to_rgba8888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv12_to_rgba8888_sg( width, height, input_cs, output_cs );
		_nv12_to_rgba8888_tps( width, height, input_cs, output_cs );
	}
}

void nv12_to_rgba8888_709(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv12_to_rgba8888_709_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv12_to_rgba8888_sg( width, height, input_cs, output_cs );
		_nv12_to_rgba8888_709_tps( width, height, input_cs, output_cs );
	}
}

void nv12_to_bgra8888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv12_to_bgra8888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv12_to_rgba8888_sg( width, height, input_cs, output_cs );
		_nv12_to_bgra8888_tps( width, height, input_cs, output_cs );
	}
}

void y_uv_nv12_to_rgba8888(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_y_uv_nv12_to_rgba8888_sg( width, height, input_y_cs, input_uv_cs, output_cs );
	}
	else
	{
		//_nv12_to_rgba8888_sg( width, height, input_cs, output_cs );
		_y_uv_nv12_to_rgba8888_tps( width, height, input_y_cs, input_uv_cs, output_cs );
	}
}

void nv12_to_rgb888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv12_to_rgb888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv12_to_rgb888_sg( width, height, input_cs, output_cs );
		_nv12_to_rgb888_tps( width, height, input_cs, output_cs );
	}
}

void _nv21_to_rgba8888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if HAVE_NEON
	if(arg->width & 1)
	{
		int width = arg->width;
		int height = arg->height;
		
		unsigned char* y0 = arg->input_buf[0][0];
		unsigned char* u = arg->input_buf[1][0];
		//unsigned char *v = arg->input_buf[2][0];

		int j = 0;

		int yVal, cbVal, crVal;
		int red, green, blue;

		//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
		unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		for(i=0; i< arg->height-1 ; i+=2 )
		{
			jsimd_ycc_nv21_extrgbx_convert_neon( arg->width, arg->input_buf, arg->output_buf);

			arg->input_buf[0][0] += arg->width<<1;
			arg->input_buf[1][0] += arg->width;
			arg->input_buf[2][0] += arg->width;
			arg->input_buf[3][0] += arg->width<<1;
			arg->output_buf[0] += (arg->width<<2) <<1;
			arg->output_buf[1] += (arg->width<<2) <<1;
		}

		if( arg->height - i ==  1 )
			jsimd_ycc_nv21_extrgbx_single_line_convert_neon( arg->width, arg->input_buf, arg->output_buf);
	}
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv21_to_rgba8888_709_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if (HAVE_NEON || HAVE_NEON_64)
	if(arg->width & 1)
	{
		int width = arg->width;
		int height = arg->height;
		
		unsigned char* y0 = arg->input_buf[0][0];
		unsigned char* u = arg->input_buf[1][0];
		//unsigned char *v = arg->input_buf[2][0];

		int j = 0;

		int yVal, cbVal, crVal;
		int red, green, blue;

		//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
		unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		for(i=0; i< arg->height-1 ; i+=2 )
		{
			jsimd_ycc_nv21_extrgbx_709_convert_neon( arg->width, arg->input_buf, arg->output_buf);

			arg->input_buf[0][0] += arg->width<<1;
			arg->input_buf[1][0] += arg->width;
			arg->input_buf[2][0] += arg->width;
			arg->input_buf[3][0] += arg->width<<1;
			arg->output_buf[0] += (arg->width<<2) <<1;
			arg->output_buf[1] += (arg->width<<2) <<1;
		}

		if( arg->height - i ==  1 )
			jsimd_ycc_nv21_extrgbx_single_line_709_convert_neon( arg->width, arg->input_buf, arg->output_buf);
	}
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv21_to_bgra8888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if 0//HAVE_NEON
	if(arg->width & 1)
	{
		int width = arg->width;
		int height = arg->height;
		
		unsigned char* y0 = arg->input_buf[0][0];
		unsigned char* u = arg->input_buf[1][0];
		//unsigned char *v = arg->input_buf[2][0];

		int j = 0;

		int yVal, cbVal, crVal;
		int red, green, blue;

		//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
		unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}
			}
		}
	}
	else
	{
		for(i=0; i< arg->height-1 ; i+=2 )
		{
			jsimd_ycc_nv21_extbgrx_convert_neon( arg->width, arg->input_buf, arg->output_buf);

			arg->input_buf[0][0] += arg->width<<1;
			arg->input_buf[1][0] += arg->width;
			arg->input_buf[2][0] += arg->width;
			arg->input_buf[3][0] += arg->width<<1;
			arg->output_buf[0] += (arg->width<<2) <<1;
			arg->output_buf[1] += (arg->width<<2) <<1;
		}

		if( arg->height - i ==  1 )
			jsimd_ycc_nv21_extbgrx_single_line_convert_neon( arg->width, arg->input_buf, 0, arg->output_buf, 1 );
	}
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}
		}
	}
#endif
}

void _nv21_to_rgb888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if HAVE_NEON
	for(i=0; i< arg->height-1 ; i+=2 )
	{
		jsimd_ycc_nv21_extrgb_convert_neon( arg->width, arg->input_buf, arg->output_buf);

		arg->input_buf[0][0] += arg->width<<1;
		arg->input_buf[1][0] += arg->width;
		arg->input_buf[2][0] += arg->width;
		arg->input_buf[3][0] += arg->width<<1;
		arg->output_buf[0] += (arg->width*3) <<1;
		arg->output_buf[1] += (arg->width*3) <<1;
	}

	if( arg->height - i ==  1 )
		jsimd_ycc_nv21_extrgb_single_line_convert_neon( arg->width, arg->input_buf, arg->output_buf);
#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];
	//unsigned char *v = arg->input_buf[2][0];

	int j = 0;

	int yVal, cbVal, crVal;
	//int red, green, blue;

	//unsigned int* outBuf = (unsigned char*)arg->output_buf[0];
	unsigned char* outBuf = (unsigned char*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				*(outBuf++)		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}


void _nv21_to_rgba8888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv21_to_rgba8888_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv21_to_rgba8888_tp, arg[i], 0);
		//_nv21_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv21_to_rgba8888_tp, arg[i], 0);
	//_nv21_to_rgba8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv21_to_rgba8888_709_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv21_to_rgba8888_709_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv21_to_rgba8888_709_tp, arg[i], 0);
		//_nv21_to_rgba8888_709_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv21_to_rgba8888_709_tp, arg[i], 0);
	//_nv21_to_rgba8888_709_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv21_to_bgra8888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv21_to_bgra8888_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv21_to_bgra8888_tp, arg[i], 0);
		//_nv21_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv21_to_bgra8888_tp, arg[i], 0);
	//_nv21_to_bgra8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _y_uv_nv21_to_rgba8888_tps(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_y_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = (unsigned char*)input_uv_cs;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	int alignWdt = ((width+1)>>1)<<1;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_y_uv_nv21_to_rgba8888_sg( width, height, input_y_cs, input_uv_cs, output_cs );;
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv21_to_rgba8888_tp, arg[i], 0);
		//_nv21_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * alignWdt );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv21_to_rgba8888_tp, arg[i], 0);
	//_nv21_to_rgba8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}


void _nv21_to_rgb888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_nv21_to_rgb888_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 3);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 3;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _nv21_to_rgb888_tp, arg[i], 0);
		//_nv21_to_rgb888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 3);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 3;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * width );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _nv21_to_rgb888_tp, arg[i], 0);
	//_nv21_to_rgb888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _nv21_to_rgba8888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
#if (HAVE_NEON || HAVE_NEON_64)

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv21_extrgbx_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv21_extrgbx_single_line_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv21_to_rgba8888_709_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
#if (HAVE_NEON || HAVE_NEON_64)

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv21_extrgbx_709_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv21_extrgbx_single_line_709_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv21_to_bgra8888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
#if 0//HAVE_NEON

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

					yVal = *(y0++);
					
					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv21_extbgrx_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv21_extbgrx_single_line_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}
		}
	}
#endif
}

void _y_uv_nv21_to_rgba8888_sg(int width, int height, int *input_cs, int *input_uv_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = (unsigned char*)input_uv_cs;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

#if HAVE_NEON

	if(width & 1)
	{
		int j;

		int yVal, cbVal, crVal;
		int red, green, blue;

		unsigned int* outBuf = (unsigned int*)output_cs;

		for(i=0; i<height; i++)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u+j)-128;
				cbVal = *(u+j+1)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			i++;

			if(i < height)
			{
				for(j=0; j<width-1; j+=2)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

					yVal = *(y0++);

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}

				if(width & 1)
				{
					yVal = *(y0++);
					crVal = *(u++)-128;
					cbVal = *(u++)-128;

					red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
					green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
					blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

					*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
				}
			}
		}
	}
	else
	{
		unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		unsigned char *output_buf[2] = {0, };

		for(i=0; i<4; i++ )
		{
			input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		output_buf[0] = (unsigned char*)output_cs;
		output_buf[1] = output_buf[0] + width * 4;

		input_buf[0][0] = y0;
		input_buf[1][0] = u;
		input_buf[2][0] = v;
		input_buf[3][0] = y1;

		for(i=0; i< height - 1 ; i+=2 )
		{
			jsimd_ycc_nv21_extrgbx_convert_neon( width, input_buf, output_buf);
			input_buf[0][0] += width*2;
			input_buf[1][0] += width;
			input_buf[2][0] += width;
			input_buf[3][0] += width*2;
			output_buf[0] += (width<<2) * 2;
			output_buf[1] += (width<<2) * 2;
		}

		if( height - i ==  1 )
			jsimd_ycc_nv21_extrgbx_single_line_convert_neon( width, input_buf, output_buf);

		for(i=0; i<4; i++ )
		{
			free(input_buf[i]);
		}

		free(input_buf);
	}
#else
	int j;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _nv21_to_rgb888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

#if HAVE_NEON
	unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	unsigned char *output_buf[2] = {0, };

	for(i=0; i<4; i++ )
	{
		input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	output_buf[0] = (unsigned char*)output_cs;
	output_buf[1] = output_buf[0] + width * 3;

	input_buf[0][0] = y0;
	input_buf[1][0] = u;
	input_buf[2][0] = v;
	input_buf[3][0] = y1;

	for(i=0; i< height - 1; i+=2 )
	{
		jsimd_ycc_nv21_extrgb_convert_neon( width, input_buf, output_buf);
		input_buf[0][0] += width*2;
		input_buf[1][0] += width;
		input_buf[2][0] += width;
		input_buf[3][0] += width*2;
		output_buf[0] += (width*3) * 2;
		output_buf[1] += (width*3) * 2;
	}

	if( height - i ==  1 )
		jsimd_ycc_nv21_extrgb_single_line_convert_neon( width, input_buf, output_buf);

	for(i=0; i<4; i++ )
	{
		free(input_buf[i]);
	}

	free(input_buf);
#else
	int j;

	int yVal, cbVal, crVal;
	//int red, green, blue;

	//unsigned int* outBuf = (unsigned int*)output_cs;
	unsigned char* outBuf = (unsigned char*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<width-1; j+=2)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			crVal = *(u+j)-128;
			cbVal = *(u+j+1)-128;

			*(outBuf++) = (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
			*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
			*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

			//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<width-1; j+=2)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				crVal = *(u++)-128;
				cbVal = *(u++)-128;

				*(outBuf++)	= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));	// r
				*(outBuf++)	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));	// g
				*(outBuf++)	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));	// b

				//*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void nv21_to_rgba8888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv21_to_rgba8888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv21_to_rgba8888_sg( width, height, input_cs, output_cs );
		_nv21_to_rgba8888_tps( width, height, input_cs, output_cs );
	}
}

void nv21_to_rgba8888_709(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv21_to_rgba8888_709_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv21_to_rgba8888_sg( width, height, input_cs, output_cs );
		_nv21_to_rgba8888_709_tps( width, height, input_cs, output_cs );
	}
}

void nv21_to_bgra8888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv21_to_bgra8888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv21_to_bgra8888_sg( width, height, input_cs, output_cs );
		_nv21_to_bgra8888_tps( width, height, input_cs, output_cs );
	}
}

void y_uv_nv21_to_rgba8888(int width, int height, int *input_y_cs, int *input_uv_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_y_uv_nv21_to_rgba8888_sg( width, height, input_y_cs, input_uv_cs, output_cs );
	}
	else
	{
		_y_uv_nv21_to_rgba8888_tps( width, height, input_y_cs, input_uv_cs, output_cs );
	}
}

void nv21_to_rgb888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_nv21_to_rgb888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_nv21_to_rgb888_sg( width, height, input_cs, output_cs );
		_nv21_to_rgb888_tps( width, height, input_cs, output_cs );
	}
}

//#if QURAM_USE_THREADPOOL
void _yuv420_to_rgba8888_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

	int width = arg->width;
	int height = arg->height;

#if (HAVE_NEON || HAVE_NEON_64)

	for(i=0; i<height;)
	{
		jsimd_ycc_h2v1_extrgbx_convert_neon( width, arg->input_buf, 0, arg->output_buf, 1 );

		arg->input_buf[0][0] += width;
		arg->input_buf[3][0] += width;
		arg->output_buf[0] += (width<<2);
		arg->output_buf[1] += (width<<2);

		i++;

		if(i < height)
		{
			jsimd_ycc_h2v1_extrgbx_convert_neon(width, arg->input_buf, 0, arg->output_buf, 1);

			arg->input_buf[0][0] += width;
			arg->input_buf[1][0] += ((width+1)>>1);
			arg->input_buf[2][0] += ((width+1)>>1);
			arg->input_buf[3][0] += width;
			arg->output_buf[0] += (width << 2);
			arg->output_buf[1] += (width << 2);

			i++;
		}
	}

#else
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];
	unsigned char *v = arg->input_buf[2][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _yuv420_to_rgba8888_709_tp(void* argv)	//int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

	int width = arg->width;
	int height = arg->height;

#if (HAVE_NEON || HAVE_NEON_64)

	for(i=0; i<height;)
	{
		jsimd_ycc_h2v1_extrgbx_709_convert_neon( width, arg->input_buf, 0, arg->output_buf, 1 );

		arg->input_buf[0][0] += width;
		arg->input_buf[3][0] += width;
		arg->output_buf[0] += (width<<2);
		arg->output_buf[1] += (width<<2);

		i++;

		if(i < height)
		{
			jsimd_ycc_h2v1_extrgbx_709_convert_neon(width, arg->input_buf, 0, arg->output_buf, 1);

			arg->input_buf[0][0] += width;
			arg->input_buf[1][0] += ((width + 1) >> 1);
			arg->input_buf[2][0] += ((width + 1) >> 1);
			arg->input_buf[3][0] += width;
			arg->output_buf[0] += (width << 2);
			arg->output_buf[1] += (width << 2);

			i++;
		}
	}

#else

	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];
	unsigned char *v = arg->input_buf[2][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}
#endif
}

void _yuv420_to_bgra8888_tp(void* argv)	
{
	int i = 0;
	quram_csc_info_to_rgba* arg = (quram_csc_info_to_rgba*)argv;

#if 0//HAVE_NEON

	for(i=0; i<height;)
	{
		jsimd_ycc_h2v1_extbgrx_convert_neon( width, arg->input_buf, 0, arg->output_buf, 1 );

		arg->input_buf[0][0] += width;
		arg->input_buf[3][0] += width;
		arg->output_buf[0] += (width<<2);
		arg->output_buf[1] += (width<<2);

		i++;

		if(i < height)
		{
			jsimd_ycc_h2v1_extbgrx_convert_neon(width, arg->input_buf, 0, arg->output_buf, 1);

			arg->input_buf[0][0] += width;
			arg->input_buf[1][0] += ((width + 1) >> 1);
			arg->input_buf[2][0] += ((width + 1) >> 1);
			arg->input_buf[3][0] += width;
			arg->output_buf[0] += (width << 2);
			arg->output_buf[1] += (width << 2);

			i++;
		}
	}

#else
	int width = arg->width;
	int height = arg->height;
	
	unsigned char* y0 = arg->input_buf[0][0];
	unsigned char* u = arg->input_buf[1][0];
	unsigned char *v = arg->input_buf[2][0];

	int j = 0;

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)arg->output_buf[0];

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}
		}
	}
#endif
}

void _yuv420_to_rgba8888_tps(int width, int height, unsigned char *input_y, unsigned char *input_u, unsigned char *input_v, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = input_y;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = input_u;
	unsigned char *v = input_v;

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_yuv420_to_rgba8888_sg(width, height, input_y, input_u, input_v, output_cs);
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _yuv420_to_rgba8888_tp, arg[i], 0);
		//_yuv420_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _yuv420_to_rgba8888_tp, arg[i], 0);
	//_yuv420_to_rgba8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _yuv420_to_rgba8888_709_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_yuv420_to_rgba8888_709_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _yuv420_to_rgba8888_709_tp, arg[i], 0);
		//_yuv420_to_rgba8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _yuv420_to_rgba8888_709_tp, arg[i], 0);
	//_yuv420_to_rgba8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}

void _yuv420_to_bgra8888_tps(int width, int height, int *input_cs, int *output_cs)
{
	int i=0;
	int j=0;

	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *out = (unsigned char*)output_cs;

	int queue_size = (height + (CSC_TP_UNIT_HEIGHT>>1))/CSC_TP_UNIT_HEIGHT;

	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgba**	arg = (quram_csc_info_to_rgba**)malloc( sizeof(quram_csc_info_to_rgba*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_yuv420_to_bgra8888_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
		arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
		for(j=0; j<4; j++ )
		{
			arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
		}

		arg[i]->width			= width;
		arg[i]->height			= CSC_TP_UNIT_HEIGHT;
		arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

		arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
		arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
		arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
		arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

		quram_threadpool_add_task( threadpool, _yuv420_to_bgra8888_tp, arg[i], 0);
		//_yuv420_to_bgra8888_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgba*)malloc( sizeof(quram_csc_info_to_rgba) );
	arg[i]->input_buf = (unsigned char*** )calloc( sizeof(char**), 4);
	for(j=0; j<4; j++ )
	{
		arg[i]->input_buf[j] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	arg[i]->width			= width;
	arg[i]->height			= height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->output_buf[0]		= out + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf[1]		= arg[i]->output_buf[0] + width * 4;

	arg[i]->input_buf[0][0]	= y0 + (i * CSC_TP_UNIT_HEIGHT * width );
	arg[i]->input_buf[1][0]	= u +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
	arg[i]->input_buf[2][0]	= v +  (i * (CSC_TP_UNIT_HEIGHT>>1) * ((width+1)>>1) );
	arg[i]->input_buf[3][0]	= y1 + (i * CSC_TP_UNIT_HEIGHT * width );

	quram_threadpool_add_task( threadpool, _yuv420_to_bgra8888_tp, arg[i], 0);
	//_yuv420_to_bgra8888_tp(arg[i]);

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		for(j=0 ; j<4 ; j++ )
		{
			free( arg[i]->input_buf[j] );
		}
		free( arg[i]->input_buf );
		free( arg[i] );
	}
	free( arg );
}
//#else

void _yuv420_to_rgba8888_sg(int width, int height, unsigned char* input_y, unsigned char* input_u, unsigned char* input_v, int *output_cs)
{
	int i = 0;
	int j = 0;
	unsigned char *y0 = input_y;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = input_u;
	unsigned char *v = input_v;
	
	int uv_width = ((width+1)>>1);

	unsigned char *output_buf[2] = {0, };

#if (HAVE_NEON || HAVE_NEON_64)
	
	unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	for(i=0; i<4; i++ )
	{
		input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	input_buf[0][0] = y0;
	input_buf[1][0] = u;
	input_buf[2][0] = v;
	input_buf[3][0] = y1;

	output_buf[0] = (unsigned char*)output_cs;
	output_buf[1] = output_buf[0] + width * 4;

	for(i=0; i< height; )
	{
		jsimd_ycc_h2v1_extrgbx_convert_neon( width, input_buf, 0, output_buf, 1 );

		input_buf[0][0] += width;
		input_buf[3][0] += width;
		output_buf[0] += (width<<2);
		output_buf[1] += (width<<2);

		i++;

		if(i<height)
		{
			jsimd_ycc_h2v1_extrgbx_convert_neon( width, input_buf, 0, output_buf, 1 );

			input_buf[0][0] += width;
			input_buf[1][0] += ((width+1)>>1);
			input_buf[2][0] += ((width+1)>>1);
			input_buf[3][0] += width;
			output_buf[0] += (width<<2);
			output_buf[1] += (width<<2);

			i++;
		}
	}

	for(i=0; i<4; i++ )
	{
		free(input_buf[i]);
	}

	free(input_buf);
#else

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}

#endif
}

void _yuv420_to_rgba8888_709_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	int j = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
	int uv_width = ((width+1)>>1);

	unsigned char *output_buf[2] = {0, };

#if (HAVE_NEON || HAVE_NEON_64)
	
	unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	for(i=0; i<4; i++ )
	{
		input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	input_buf[0][0] = y0;
	input_buf[1][0] = u;
	input_buf[2][0] = v;
	input_buf[3][0] = y1;

	output_buf[0] = (unsigned char*)output_cs;
	output_buf[1] = output_buf[0] + width * 4;

	for(i=0; i< height; )
	{
		jsimd_ycc_h2v1_extrgbx_709_convert_neon( width, input_buf, 0, output_buf, 1 );

		input_buf[0][0] += width;
		input_buf[3][0] += width;
		output_buf[0] += (width<<2);
		output_buf[1] += (width<<2);

		i++;

		if(i<height)
		{
			jsimd_ycc_h2v1_extrgbx_709_convert_neon( width, input_buf, 0, output_buf, 1 );

			input_buf[0][0] += width;
			input_buf[1][0] += ((width+1)>>1);
			input_buf[2][0] += ((width+1)>>1);
			input_buf[3][0] += width;
			output_buf[0] += (width<<2);
			output_buf[1] += (width<<2);

			i++;
		}
	}

	for(i=0; i<4; i++ )
	{
		free(input_buf[i]);
	}

	free(input_buf);

#else

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED_709(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN_709(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE_709(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (blue<<16) | (green<<8) | (red);
			}
		}
	}

#endif
}

void _yuv420_to_bgra8888_sg(int width, int height, int *input_cs, int *output_cs)
{
	int i = 0;
	int j = 0;
	unsigned char *y0 = (unsigned char*)input_cs;
	unsigned char *y1 = y0 + width ;
	unsigned char *u = y0 + width * height;
	unsigned char *v = u + ((width+1)>>1) * ((height+1)>>1);
	
	unsigned char *output_buf[2] = {0, };

#if (HAVE_NEON || HAVE_NEON_64)
	
	unsigned char ***input_buf = (unsigned char*** )calloc( sizeof(char**), 4);

	for(i=0; i<4; i++ )
	{
		input_buf[i] = (unsigned char**)calloc( sizeof(char*), 1 );
	}

	input_buf[0][0] = y0;
	input_buf[1][0] = u;
	input_buf[2][0] = v;
	input_buf[3][0] = y1;

	output_buf[0] = (unsigned char*)output_cs;
	output_buf[1] = output_buf[0] + width * 4;

	for (i = 0; i< height;)
	{
		jsimd_ycc_h2v1_extbgrx_convert_neon(width, input_buf, 0, output_buf, 1);

		input_buf[0][0] += width;
		input_buf[3][0] += width;
		output_buf[0] += (width<<2);
		output_buf[1] += (width<<2);

		i++;

		if(i<height)
		{
			jsimd_ycc_h2v1_extbgrx_convert_neon( width, input_buf, 0, output_buf, 1 );

			input_buf[0][0] += width;
			input_buf[1][0] += ((width+1)>>1);
			input_buf[2][0] += ((width+1)>>1);
			input_buf[3][0] += width;
			output_buf[0] += (width<<2);
			output_buf[1] += (width<<2);

			i++;
		}
	}
	
	for(i=0; i<4; i++ )
	{
		free(input_buf[i]);
	}

	free(input_buf);
#else

	int yVal, cbVal, crVal;
	int red, green, blue;

	unsigned int* outBuf = (unsigned int*)output_cs;

	for(i=0; i<height; i++)
	{
		for(j=0; j<(width>>1); j++)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

			yVal = *(y0++);
			
			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		if(width & 1)
		{
			yVal = *(y0++);
			cbVal = *(u+j)-128;
			crVal = *(v+j)-128;

			red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
			green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
			blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

			*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
		}

		i++;

		if(i < height)
		{
			for(j=0; j<(width>>1); j++)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);

				yVal = *(y0++);
				
				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}

			if(width & 1)
			{
				yVal = *(y0++);
				cbVal = *(u++)-128;
				crVal = *(v++)-128;

				red		= (clip( YCBCR_TO_RED(yVal, cbVal, crVal)  ));
				green	= (clip( YCBCR_TO_GREEN(yVal, cbVal, crVal)));
				blue	= (clip( YCBCR_TO_BLUE(yVal, cbVal, crVal) ));

				*(outBuf++) = 0xFF000000 | (red<<16) | (green<<8) | (blue);
			}
		}
	}

#endif
}
//#endif

void yuv420_to_rgba8888(int width, int height, unsigned char* input_y, unsigned char* input_u, unsigned char* input_v, int *output_cs)
{
	if( height < 1200 )
	{
		_yuv420_to_rgba8888_sg(width, height, input_y, input_u, input_v, output_cs);
	}
	else
	{
		//_yuv420_to_rgba8888_sg( width, height, input_cs, output_cs );
		_yuv420_to_rgba8888_tps(width, height, input_y, input_u, input_v, output_cs);
	}
}

void yuv420_to_rgba8888_709(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_yuv420_to_rgba8888_709_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_yuv420_to_rgba8888_sg( width, height, input_cs, output_cs );
		_yuv420_to_rgba8888_709_tps( width, height, input_cs, output_cs );
	}
}

void yuv420_to_bgra8888(int width, int height, int *input_cs, int *output_cs)
{
	if( height < 1200 )
	{
		_yuv420_to_bgra8888_sg( width, height, input_cs, output_cs );
	}
	else
	{
		//_yuv420_to_rgba8888_sg( width, height, input_cs, output_cs );
		_yuv420_to_bgra8888_tps( width, height, input_cs, output_cs );
	}
}

//void swap_uv(unsigned char *pOut, unsigned char *pIn0, int size)
typedef struct _quram_csc_info_swap
{
	int size;
	unsigned char* output_buf;
	unsigned char* input_buf;
} quram_csc_info_swap;



void _swap_uv( void* argv )
{
#if HAVE_NEON
	quram_csc_info_swap* arg = (quram_csc_info_swap*)argv;
	unsigned short *pSout = 0;
	unsigned char *outb = arg->output_buf;
	unsigned char *inb = arg->input_buf;
	int i = 0;
	int size_4 = arg->size/4;

	uint8x8_t v_input;
	for(i=0; i < size_4; i++)
	{
		v_input = vld1_u8(inb); inb+=8;
		vst1_u8(outb, vrev16_u8(v_input)); outb+=8;
	}

	pSout = (unsigned short *)outb;
	i*= 4;
	for(; i< arg->size; i++)
	{
		*pSout++ = (*inb << 8) | (*(inb+1) );
		inb++;
		inb++;
	}
#else
	quram_csc_info_swap* arg = (quram_csc_info_swap*)argv;
	unsigned short *pSout = 0;
	unsigned char *outb = arg->output_buf;
	unsigned char *inb = arg->input_buf;
	int i = 0;
	int size_4 = arg->size/4;

	pSout = (unsigned short *)outb;

	for(; i< arg->size; i++)
	{
		*pSout++ = (*inb << 8) | (*(inb+1) );
		inb++;
		inb++;
	}

#endif
}

//#define CSC_TP_UNIT_SIZE	256*1024
void _yuv_nv_reverse_tps(int width, int height, int *input_cs, int *output_cs)
{
	unsigned char *pInY = (unsigned char*)input_cs;
	unsigned char *pInU = pInY + width * height;
	unsigned char *pInV = pInU + 1;

	unsigned char *pOutY = (unsigned char*)output_cs;
	unsigned char *pOutV = pOutY + width * height;
	unsigned char *pOutU = pOutV + 1;

	int uv_size = ((width+1)>>1) * ((height+1)>>1);

	int CSC_TP_UNIT_SIZE = ((width+1)>>1) * 512;

	int i=0;
	int queue_size = (uv_size + (CSC_TP_UNIT_SIZE>>1))/CSC_TP_UNIT_SIZE;

	quram_csc_info_swap**	arg = (quram_csc_info_swap**)malloc( sizeof(quram_csc_info_swap*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_yuv_nv_reverse_sg( width, height, input_cs, output_cs );
		return;
	}
	
	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_swap*)malloc( sizeof(quram_csc_info_swap) );

		arg[i]->size			= CSC_TP_UNIT_SIZE;
		arg[i]->input_buf		= pInU + (i * CSC_TP_UNIT_SIZE*2);
		arg[i]->output_buf		= pOutV + (i * CSC_TP_UNIT_SIZE*2);

		quram_threadpool_add_task( threadpool, _swap_uv, arg[i], 0);
		//_swap_uv(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_swap*)malloc( sizeof(quram_csc_info_swap) );

	arg[i]->size			= uv_size - (i * CSC_TP_UNIT_SIZE);
	arg[i]->input_buf		= pInU + (i * CSC_TP_UNIT_SIZE*2);
	arg[i]->output_buf		= pOutV + (i * CSC_TP_UNIT_SIZE*2);

	quram_threadpool_add_task( threadpool, _swap_uv, arg[i], 0);
	//_swap_uv(arg[i]);

	// Y copy here
#if HAVE_NEON
	memcpy_neon(pOutY, pInY, width*height);
#else
	memcpy(pOutY, pInY, width*height);
#endif

	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		free( arg[i] );
	}
	free( arg );
}

void _yuv_nv_reverse_sg(int width, int height, int *input_cs, int *output_cs)
{
	unsigned char *pInY = (unsigned char*)input_cs;
	unsigned char *pInU = pInY + width * height;
	unsigned char *pInV = pInU + 1;

	unsigned char *pOutY = (unsigned char*)output_cs;
	unsigned char *pOutV = pOutY + width * height;
	unsigned char *pOutU = pOutV + 1;
	quram_csc_info_swap	arg;

	int uv_size = ((width+1)>>1) * ((height+1)>>1);
#if HAVE_NEON
	memcpy_neon(pOutY, pInY, width*height);
#else
	memcpy(pOutY, pInY, width*height);
#endif
	arg.size = uv_size;
	arg.output_buf	= pOutV;
	arg.input_buf	= pInU;
	_swap_uv( &arg );
	//swap_uv(pOutV, pInU, uv_size);
}

void yuv_nv_reverse(int width, int height, int *input_cs, int *output_cs)
{
	if( height * width > 4*1024*1024)
	{
		//_yuv_nv_reverse_sg(width, height, input_cs, output_cs);
		_yuv_nv_reverse_tps(width, height, input_cs, output_cs);
	}
	else
	{
		_yuv_nv_reverse_sg(width, height, input_cs, output_cs);
	}
}

typedef struct _quram_csc_info_interleave
{
	int size;

	unsigned char* output_buf;
	unsigned char* input_u;
	unsigned char* input_v;
} quram_csc_info_interleave;

void _interleave_uv( void* argv )
{
#if HAVE_NEON
	quram_csc_info_interleave* arg = (quram_csc_info_interleave*)argv;
	unsigned short *pSout = 0;
	unsigned char *outb = arg->output_buf;
	unsigned char *inb_u = arg->input_u;
	unsigned char *inb_v = arg->input_v;
	int i = 0;
	int size_8 = arg->size/8;

	uint8x8_t u_input;
	uint8x8_t v_input;
	uint8x8x2_t uv_output;

	for(i=0; i < size_8; i++)
	{
		u_input = vld1_u8(inb_u); inb_u+=8;
		v_input = vld1_u8(inb_v); inb_v+=8;
		uv_output = vzip_u8(u_input, v_input);
		vst1_u8(outb, uv_output.val[0]); outb+=8;
		vst1_u8(outb, uv_output.val[1]); outb+=8;
	}

	pSout = (unsigned short *)outb;
	i*= 8;
	for(; i< arg->size; i++)
	{
		*pSout++ = (*inb_v << 8) | (*(inb_u) );
		inb_u++;
		inb_v++;
	}
#else
	quram_csc_info_interleave* arg = (quram_csc_info_interleave*)argv;
	unsigned short *pSout = 0;
	unsigned char *outb = arg->output_buf;
	unsigned char *inb_u = arg->input_u;
	unsigned char *inb_v = arg->input_v;
	int i = 0;
	int size_8 = arg->size/8;

	pSout = (unsigned short *)outb;

	for(; i< arg->size; i++)
	{
		*pSout++ = (*inb_v << 8) | (*(inb_u) );
		inb_u++;
		inb_v++;
	}
#endif
}

//#define CSC_TP_UNIT_SIZE	256*1024
void _yuv420_to_nv12_tps(int width, int height, int *input_cs, int *output_cs)
{
	unsigned char *pInY = (unsigned char*)input_cs;
	unsigned char *pInU = pInY + width * height;
	unsigned char *pInV = pInU + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *pOutY = (unsigned char*)output_cs;
	unsigned char *pOutUV = pOutY + width * height;

	int uv_size = ((width+1)>>1) * ((height+1)>>1);

	int CSC_TP_UNIT_SIZE = ((width+1)>>1) * 512;

	int i=0;
	int queue_size = (uv_size + (CSC_TP_UNIT_SIZE>>1))/CSC_TP_UNIT_SIZE;

	quram_csc_info_interleave**	arg = (quram_csc_info_interleave**)malloc( sizeof(quram_csc_info_interleave*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );

	if(threadpool == 0)
	{
		_yuv420_to_nv12_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_interleave*)malloc( sizeof(quram_csc_info_interleave) );
		
		arg[i]->size			= CSC_TP_UNIT_SIZE;
		arg[i]->input_u			= pInU + (i * CSC_TP_UNIT_SIZE);
		arg[i]->input_v			= pInV + (i * CSC_TP_UNIT_SIZE);
		arg[i]->output_buf		= pOutUV + (i * CSC_TP_UNIT_SIZE*2);

		quram_threadpool_add_task( threadpool, _interleave_uv, arg[i], 0);
		//_interleave_uv(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_interleave*)malloc( sizeof(quram_csc_info_interleave) );

	arg[i]->size			= uv_size - (i * CSC_TP_UNIT_SIZE);
	arg[i]->input_u			= pInU + (i * CSC_TP_UNIT_SIZE);
	arg[i]->input_v			= pInV + (i * CSC_TP_UNIT_SIZE);
	arg[i]->output_buf		= pOutUV + (i * CSC_TP_UNIT_SIZE*2);

	//jstat_printf("arg[%d]->size  : %d\n", i, arg[i]->size);

	quram_threadpool_add_task( threadpool, _interleave_uv, arg[i], 0);
	//_interleave_uv(arg[i]);

	// Y copy here
#if HAVE_NEON
	memcpy_neon(pOutY, pInY, width*height);
#else
	memcpy(pOutY, pInY, width*height);
#endif
	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		free( arg[i] );
	}
	free( arg );
}


void _yuv420_to_nv12_sg(int width, int height, int *input_cs, int *output_cs)
{
	unsigned char *pInY = (unsigned char*)input_cs;
	unsigned char *pInU = pInY + width * height;
	unsigned char *pInV = pInU + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *pOutY = (unsigned char*)output_cs;
	unsigned char *pOutUV = pOutY + width * height;

	quram_csc_info_interleave	arg;

	int uv_size = ((width+1)>>1) * ((height+1)>>1);
#if HAVE_NEON
	memcpy_neon(pOutY, pInY, width*height);
#else
	memcpy(pOutY, pInY, width*height);
#endif
	arg.size = uv_size;
	arg.output_buf	= pOutUV;
	arg.input_u	= pInU;
	arg.input_v	= pInV;
	_interleave_uv( &arg );
	//swap_uv(pOutV, pInU, uv_size);
}

void yuv420_to_nv12(int width, int height, int *input_cs, int *output_cs)
{
	if( height * width > 4*1024*1024)
	{
		_yuv420_to_nv12_tps(width, height, input_cs, output_cs);
		//_yuv420_to_nv12_sg(width, height, input_cs, output_cs);
	}
	else
	{
		_yuv420_to_nv12_sg(width, height, input_cs, output_cs);
	}
}

//#define CSC_TP_UNIT_SIZE	256*1024
void _yuv420_to_nv21_tps(int width, int height, int *input_cs, int *output_cs)
{
	unsigned char *pInY = (unsigned char*)input_cs;
	unsigned char *pInU = pInY + width * height;
	unsigned char *pInV = pInU + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *pOutY = (unsigned char*)output_cs;
	unsigned char *pOutUV = pOutY + width * height;

	int uv_size = ((width+1)>>1) * ((height+1)>>1);

	int CSC_TP_UNIT_SIZE = ((width+1)>>1) * 512;

	int i=0;
	int queue_size = (uv_size + (CSC_TP_UNIT_SIZE>>1))/CSC_TP_UNIT_SIZE;

	quram_csc_info_interleave**	arg = (quram_csc_info_interleave**)malloc( sizeof(quram_csc_info_interleave*) * queue_size );

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init( CSC_TP_THREAD_COUNT );
	
	if(threadpool == 0)
	{
		_yuv420_to_nv21_sg( width, height, input_cs, output_cs );
		return;
	}

	for(i=0 ; i<queue_size-1 ; i++)
	{
		arg[i] = (quram_csc_info_interleave*)malloc( sizeof(quram_csc_info_interleave) );
		
		arg[i]->size			= CSC_TP_UNIT_SIZE;
		arg[i]->input_u			= pInV + (i * CSC_TP_UNIT_SIZE);
		arg[i]->input_v			= pInU + (i * CSC_TP_UNIT_SIZE);
		arg[i]->output_buf		= pOutUV + (i * CSC_TP_UNIT_SIZE*2);

		quram_threadpool_add_task( threadpool, _interleave_uv, arg[i], 0);
		//_interleave_uv(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_interleave*)malloc( sizeof(quram_csc_info_interleave) );

	arg[i]->size			= uv_size - (i * CSC_TP_UNIT_SIZE);
	arg[i]->input_u			= pInV + (i * CSC_TP_UNIT_SIZE);
	arg[i]->input_v			= pInU + (i * CSC_TP_UNIT_SIZE);
	arg[i]->output_buf		= pOutUV + (i * CSC_TP_UNIT_SIZE*2);
	
	quram_threadpool_add_task( threadpool, _interleave_uv, arg[i], 0);
	//_interleave_uv(arg[i]);

	// Y copy here
#if HAVE_NEON
	memcpy_neon(pOutY, pInY, width*height);
#else
	memcpy(pOutY, pInY, width*height);
#endif
	// process last thread
	quram_threadpool_free( threadpool, 1 );

	for(i=0 ; i<queue_size ; i++ )
	{
		free( arg[i] );
	}
	free( arg );
}


void _yuv420_to_nv21_sg(int width, int height, int *input_cs, int *output_cs)
{
	unsigned char *pInY = (unsigned char*)input_cs;
	unsigned char *pInU = pInY + width * height;
	unsigned char *pInV = pInU + ((width+1)>>1) * ((height+1)>>1);

	unsigned char *pOutY = (unsigned char*)output_cs;
	unsigned char *pOutUV = pOutY + width * height;

	quram_csc_info_interleave	arg;

	int uv_size = ((width+1)>>1) * ((height+1)>>1);
#if HAVE_NEON
	memcpy_neon(pOutY, pInY, width*height);
#else
	memcpy(pOutY, pInY, width*height);
#endif
	arg.size = uv_size;
	arg.output_buf	= pOutUV;
	arg.input_u	= pInV;
	arg.input_v	= pInU;
	_interleave_uv( &arg );
	//swap_uv(pOutV, pInU, uv_size);
}

void yuv420_to_nv21(int width, int height, int *input_cs, int *output_cs)
{
	if( height * width > 4*1024*1024)
	{
		_yuv420_to_nv21_tps(width, height, input_cs, output_cs);
	}
	else
	{
		_yuv420_to_nv21_sg(width, height, input_cs, output_cs);
	}
}

void _rgba8888_to_rgb565_sg(int width, int height, int *input_cs, unsigned short *output_cs)
{
	int i = 0, j = 0;

	int red, green, blue;

	int rgba_val = 0;

	int* pInBuff = input_cs;
	unsigned short* pOutBuff = (unsigned short*)output_cs;
	
	for (i = 0; i<height; i++)
	{
		pInBuff = input_cs + width*i;
		pOutBuff = output_cs + width*i;

		for (j = 0; j<width; j++)
		{
			rgba_val = *(pInBuff++) & 0x00ffffff;

			red = rgba_val & 0xff;
			blue = rgba_val >> 16;
			green = (rgba_val & 0xff00) >> 8;

			*(pOutBuff++) = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
		}
	}
}

void _rgba8888_to_rgb565_tp(void* argv)
{
	int i = 0, j = 0;
	quram_csc_info_to_rgb* arg = (quram_csc_info_to_rgb*)argv;

	int width = arg->width;
	int height = arg->height;
	
	unsigned int* input_cs = (unsigned int*)arg->input_buf;
	unsigned short* output_cs = (unsigned short*)arg->output_buf;

	int* pInBuff = input_cs;
	unsigned short* pOutBuff = (unsigned short*)output_cs;

	int red, green, blue;

	int rgba_val = 0;

	for (i = 0; i<height; i++)
	{
		pInBuff = input_cs + width*i;
		pOutBuff = output_cs + width*i;

		for (j = 0; j<width; j++)
		{
			rgba_val = *(pInBuff++) & 0x00ffffff;

			red = rgba_val & 0xff;
			blue = rgba_val >> 16;
			green = (rgba_val & 0xff00) >> 8;

			*(pOutBuff++) = ((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3);
		}
	}
}

void _rgba8888_to_rgb565_tps(int width, int height, int *input_cs, unsigned short *output_cs)
{
	int i = 0;
	int j = 0;

	unsigned short* pOutBuff = (unsigned short*)output_cs;

	int queue_size = (height) / CSC_TP_UNIT_HEIGHT;

	unsigned char*	input_cs_offset = (unsigned char*)input_cs;
	int ret = 0;
	//clock_t st, et;

	quram_csc_info_to_rgb**	arg = (quram_csc_info_to_rgb**)malloc(sizeof(quram_csc_info_to_rgb*)* queue_size);

	struct quram_threadpool* threadpool = 0;

	threadpool = quram_threadpool_init(CSC_TP_THREAD_COUNT);

	if (threadpool == 0)
	{
		_rgba8888_to_rgb565_sg(width, height, input_cs, output_cs);
		return;
	}

	for (i = 0; i<queue_size - 1; i++)
	{
		arg[i] = (quram_csc_info_to_rgb*)malloc(sizeof(quram_csc_info_to_rgb));
		
		arg[i]->width = width;
		arg[i]->height = CSC_TP_UNIT_HEIGHT;
		arg[i]->input_buf = input_cs_offset + (i * CSC_TP_UNIT_HEIGHT * width * 4);
		arg[i]->output_buf = pOutBuff + (i * CSC_TP_UNIT_HEIGHT * width);

		quram_threadpool_add_task(threadpool, _rgba8888_to_rgb565_tp, arg[i], 0);
		//_rgba8888_to_rgb565_tp(arg[i]);
	}

	// process last thread
	arg[i] = (quram_csc_info_to_rgb*)malloc(sizeof(quram_csc_info_to_rgb));

	arg[i]->width = width;
	arg[i]->height = height - (CSC_TP_UNIT_HEIGHT*i);
	arg[i]->input_buf = input_cs_offset + (i * CSC_TP_UNIT_HEIGHT * width * 4);
	arg[i]->output_buf = pOutBuff + (i * CSC_TP_UNIT_HEIGHT * width);

	quram_threadpool_add_task(threadpool, _rgba8888_to_rgb565_tp, arg[i], 0);
	//_rgba8888_to_rgb565_tp(arg[i]);

	// process last thread
	quram_threadpool_free(threadpool, 1);

	for (i = 0; i<queue_size; i++)
	{
		free(arg[i]);
	}
	free(arg);
}

void rgba8888_to_rgb565(int width, int height, int *input_cs, unsigned short *output_cs)
{
	if (height < 1200)
	{
		_rgba8888_to_rgb565_sg(width, height, input_cs, output_cs);
	}
	else
	{
		//_rgba8888_to_rgb565_sg(width, height, input_cs, output_cs);
		_rgba8888_to_rgb565_tps(width, height, input_cs, output_cs);
	}
}