#ifndef __STITCHING_TEMPLATE_QURAM_H__
#define __STITCHING_TEMPLATE_QURAM_H__

#include "PTSParam.h"
#include "InputManager.h"
#include "WarpInfo.h"
#include "RegionalWarping\RegionalWarping.h"

struct CameraParams
{
	CameraParams(int outWidth, int outHeight, int inputWidth, int inputHeight, int num_cam);
	void destroy();
	void setData(CameraParams* camera_params);
	PTSParam global_param;
	PTSParam image_param[CAMERA_MAX];
	WarpInfo *warp_info;
	int mNumCamera;

	int mOutputWidth;
	int mOutputHeight;

	bool mIsRegionalWarping;
};

struct HistoryCircularQueue {
	CameraParams** HistoryDataList;
	int head, tail, max_index, cur_index;
	int input_width, input_height;
	int output_width, output_height;
	int num_camera;
	HistoryCircularQueue(int k, int outWidth, int outHeight, int inputWidth, int inputHeight, int num_cam) :head(0), tail(0), max_index(k), cur_index(0){
		input_width = inputWidth;
		input_height = inputHeight;
		output_width = outWidth;
		output_height = outHeight;
		num_camera = num_cam;
		HistoryDataList = (CameraParams**)malloc(sizeof(CameraParams*)*max_index);
		for (int i = 0; i < max_index; i++)
			HistoryDataList[i] = new CameraParams(output_width, output_height, input_width, input_height, num_camera);
	}
	int size();
	bool isEmpty();
	bool isFull();
	void pre_enqueue();
	void enqueue(CameraParams* data);
	//void enqueue(PTSParam* global, PTSParam* image);
	void destory();
	//CameraParams dequeue();
	bool undoEnabled();
	bool redoEnabled();
	CameraParams* undo();
	CameraParams* redo();
};

struct StitchingTemplate
{
	StitchingTemplate(int outWdt, int outHgt, int inputWidth, int inputHeight, int num_cam);
	void destroy();

	PTSParam* getImageParams();
	PTSParam* getIGlobalParam();
	CameraParams* mCameraParams;
	HistoryCircularQueue* mCircularQueue;
};


#endif