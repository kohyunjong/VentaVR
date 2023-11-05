#ifndef __WARP_HELPER__QURAM_H__
#define __WARP_HELPER__QURAM_H__

#include "QWarperCore.h"
#include "CamInput/CamInput.h"
#include "clock.h"

#define REGION_WIDTH 8
#define REGION_HEIGHT 4

class ComposeHelper : public QWarperCore
{
public:
	const float *dist, *rt;
	const float *diff_rt;
	float fov, focal, aspect;
	float yaw, roll, pitch;
	int width, height;
	float rotatem[9];
	double rot0, rot1, scale0, scale1, distance, aspect_dw, aspect_sw;
	double crop_sx, crop_sy, crop_ratio_w, crop_ratio_h;
	float lens_dist[4];
	int lens_type;
	int output_type;
	float output_fov;
	int roix, roiy, roiw, roih; 
	bool regionMap[REGION_HEIGHT][REGION_WIDTH];

	ComposeHelper(const CamInput& inp, int w, int h, int ComposeHelperMode = COMPOSEHELPER_ROI, bool stereo_right = false){
		set(inp, w, h, ComposeHelperMode, stereo_right);
	};
	void update(const CamInput& inp, int w, int h, int ComposeHelperMode = COMPOSEHELPER_ROI, bool stereo_right = false) {
		set(inp, w, h, ComposeHelperMode, stereo_right);
	}

	int set(const CamInput& inp, int w, int h, int ComposeHelperMode, bool stereo_right = false);
	int smallRoiW(int scale = 32) { return (roix + roiw + scale - 1) / scale - smallRoiX(scale); }
	int smallRoiH(int scale = 32) { return (roiy + roih + scale - 1) / scale - smallRoiY(scale); }
	int smallRoiX(int scale = 32) { return (roix) / scale; }
	int smallRoiY(int scale = 32) { return (roiy) / scale; }

	int getOutputCoord(int inputWidth, int inputHeight, int outWidth, int outHeight, int x, int y, float* ret_x, float* ret_y, cv::Rect crop_rect);
	int computeRoi(int inputWidth, int inputHeight, int outWidth, int outHeight, double fov, double yaw, double pitch, double roll, float* dist, cv::Rect crop_rect);
	int computeRegion(int inputWidth, int inputHeight, int outWidth, int outHeight, double fov, double yaw, double pitch, double roll, float* dist, cv::Rect crop_rect);
	int smallWarp(int input_width, int input_height, unsigned char* img_buf, unsigned char* buf, unsigned char* msk, cv::Rect crop_rect, int scale = 32);
	int computeCenterPoint(int inputWidth, int inputHeight, int outWidth, int outHeight, cv::Rect crop_rect, cv::Rect& ret_rect, cv::Point& center_point);
	int computeRoiFit(int inputWidth, int inputHeight, int outWidth, int outHeight, cv::Rect crop_rect, cv::Rect& ret_rect, cv::Point& center_point);
	int makeWarpMap(int input_width, int input_height, int* warp_mask, cv::Rect crop_rect, int outputWidth, int outputHeight);
	int CalcDist(cv::Mat& input_mask, int input_width, int input_height, int x, int y, cv::Rect crop_rect, int scale = 32);
	int CalcDist(int input_width, int input_height, int x, int y, cv::Rect crop_rect, int scale = 32);
	int CalcDist(int input_width, int input_height, int x, int y);
	int getSourceCoord(int input_width, int input_height, int x, int y, cv::Rect crop_rect, double *ret_x, double *ret_y, int scale = 32);

	int computeRoiFit_makeInvWarpMap(int inputWidth, int inputHeight, int outWidth, int outHeight, cv::Rect crop_rect, cv::Rect& ret_rect, cv::Point& center_point, int* inv_warp_mask);
};
#endif	//__WARP_HELPER__QURAM_H__
