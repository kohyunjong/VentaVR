#include "PTSParam.h"

PTSParam::PTSParam() : f(0), y(0), p(0), r(0), source_buffer(NULL), regional_warping_level(1){ sourcemask = cv::Mat(); }
PTSParam::PTSParam(const PTSParam &other) { *this = other; }

const PTSParam& PTSParam::operator =(const PTSParam &other)
{
	f = other.f;
	out_f = other.out_f;

	y = other.y;
	p = other.p;
	r = other.r;

	v = other.v;
	a = other.a;
	b = other.b;
	c = other.c;

	out_fov = other.out_fov;

	//PBO
	w = other.w;
	h = other.h;

	top = other.top;
	bottom = other.bottom;
	left = other.left;
	right = other.right;

	//Main Memory
	cali_w = other.cali_w;
	cali_h = other.cali_h;

	cali_top = other.cali_top;
	cali_bottom = other.cali_bottom;
	cali_left = other.cali_left;
	cali_right = other.cali_right;
	scale = other.scale;

	stitching_mode = other.stitching_mode;

	//mask
	if (other.source_buffer != NULL){
		if (source_buffer != NULL){
			free(source_buffer);
			source_buffer = NULL;
		}
		source_buffer = (char*)malloc(sizeof(char)*other.sourcesize);
		memcpy(source_buffer, other.source_buffer, other.sourcesize);
		sourcesize = other.sourcesize;
	}
	else{
		if (source_buffer != NULL){
			free(source_buffer);
			source_buffer = NULL;
			sourcesize = 0;
		}
	}
	if (!sourcemask.empty()){
		sourcemask.release();
	}
	if (!other.sourcemask.empty() && other.sourcemask.rows > 0 && other.sourcemask.cols > 0){
		other.sourcemask.copyTo(sourcemask);
	}
	//output_mask = other.output_mask;

	// rw
	regional_warping_raw_buffer = other.regional_warping_raw_buffer;
	if (other.regional_warping_buffer != NULL){
		if (regional_warping_buffer != NULL){
			free(regional_warping_buffer);
			regional_warping_buffer = NULL;
		}
		regional_warping_buffer = (char*)malloc(sizeof(char)*other.regional_warping_size);
		memcpy(regional_warping_buffer, other.regional_warping_buffer, other.regional_warping_size);
		regional_warping_size = other.regional_warping_size;
	}
	else{
		if (regional_warping_buffer != NULL){
			free(regional_warping_buffer);
			regional_warping_buffer = NULL;
			regional_warping_size = 0;
		}
	}
	mapW = other.mapW;
	mapH = other.mapH;
	regional_warping_level = other.regional_warping_level;
	is_clear = other.is_clear;
	is_dirty = other.is_dirty;

	if (!other.warp_img.empty() && other.warp_img.rows > 0 && other.warp_img.cols > 0){
		other.warp_img.copyTo(warp_img);
		other.warp_mask.copyTo(warp_mask);
		warp_rect = other.warp_rect;
		mask_index = other.mask_index;
	}

	return *this;
}

void PTSParam::PTSInit()
{
	sourcemask = cv::Mat();
	source_buffer = NULL;
}
void PTSParam::PTSDistroy()
{
	if (!sourcemask.empty())
		sourcemask.release();
	if (source_buffer != NULL){
		free(source_buffer);
		source_buffer = NULL;
	}
}