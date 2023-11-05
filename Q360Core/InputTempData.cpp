#include "InputTempData.h"

InputTempData::InputTempData(int numCamera, int numBuffers, int width, int height, int channel)
{
	mNum_camera = numCamera;

	inputs = (Calibrator_Input*)malloc(sizeof(Calibrator_Input)* mNum_camera);
	seam_finder = new qSeamFinder();

	for (int i = 0; i < mNum_camera; i++)
	{
		inputs[i].init(numBuffers, width, height, channel);
	}
	// test
	init_masks = 0;
	updated_mask = 0;
}
void* InputTempData::getBufferAddress(int camera_idx, int buffer_idx)
{
	return inputs[camera_idx].getBufferAddress(buffer_idx);
}