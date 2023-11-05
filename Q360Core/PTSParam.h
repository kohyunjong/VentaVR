#ifndef __PTS_PARAM_QURAM_H__
#define __PTS_PARAM_QURAM_H__

#include "opencv2/highgui/highgui.hpp"

struct PTSParam {
	PTSParam();
	PTSParam(const PTSParam& other);
	const PTSParam& operator =(const PTSParam& other);
	void PTSDistroy();
	void PTSInit();

	int f;
	int out_f;

	double y;
	double p;
	double r;

	double v;
	double a;
	double b;
	double c;

	double out_fov;

	//PBO
	int w;
	int h;

	int top;
	int bottom;
	int left;
	int right;

	//Main Memory
	int cali_w;
	int cali_h;

	int cali_top;
	int cali_bottom;
	int cali_left;
	int cali_right;

	int scale;

	cv::Mat sourcemask;
	char* source_buffer = NULL;
	int sourcesize;
	bool isValued;
	//unsigned char** output_mask;

	char* regional_warping_raw_buffer = NULL;
	char* regional_warping_buffer = NULL;
	int regional_warping_size;
	int regional_warping_level;
	bool is_clear;
	bool is_dirty;

	cv::Mat warp_img;
	cv::Mat warp_mask;
	cv::Rect warp_rect;
	std::vector<int> mask_index;

	//smart
	unsigned char* mapBuffer;
	int mapW;
	int mapH;

        int stitching_mode;	
        // for stereo overlay
	double diff_yaw = 0.0;
	double diff_pitch = 0.0;
};

#endif // __PTS_PARAM_QURAM_H__