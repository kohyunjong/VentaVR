#ifndef __WARPINFO_H__
#define __WARPINFO_H__

#include "opencv2/highgui/highgui.hpp"

#define INIT_DATA	0x00
#define INVALID_DATA 0x01
#define VALID_DATA 0x02

#define WAIT_WARP_TIME 10

struct WarpInfo {
	int			*warpMap = 0;
	cv::Rect	roi;
	int			data_state;
	cv::Point	center_point;

	int			*inv_warpMap = 0;
};

#endif	//__WARPINFO_H__