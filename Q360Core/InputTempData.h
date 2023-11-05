#ifndef __QURAM_INPUT_TEMP_DATA_H__
#define __QURAM_INPUT_TEMP_DATA_H__

#pragma once
#include "clock.h"
#include "Calibrator_Input.h"
#include "qSeamFinder.h"

#define SUCCESS 1
#define FAIL 0

struct InputTempData
{
public:
	void**						seamMasks;
	int							mw, mh;
	int							init_masks;
	long long					updated_mask;
	int							mNum_camera;
	struct Calibrator_Input*	inputs;
	struct qSeamFinder*			seam_finder;

	int output_width;
	int output_height;

	InputTempData(int numCamera, int numBuffers, int width, int height, int channel);
	void*	getBufferAddress(int camera_idx, int buffer_idx);
};
#endif   //__QURAM_INPUT_TEMP_DATA_H__