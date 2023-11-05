#include "StitchingTemplate.h"
#include "StitchingMode.h"

StitchingTemplate::StitchingTemplate(int outWidth, int outHeight, int inputWidth, int inputHeight, int num_cam)
{
	mCameraParams = new CameraParams(outWidth, outHeight, inputWidth, inputHeight, num_cam);
	mCircularQueue = new HistoryCircularQueue(10, outWidth, outHeight, inputWidth, inputHeight, num_cam);
}

void StitchingTemplate::destroy()
{
	printf("bigheadk, StitchingTemplate delete 0\n");
	if (mCameraParams)
	{
		mCameraParams->destroy();
		delete mCameraParams;
	}
	mCameraParams = NULL;
	printf("bigheadk, StitchingTemplate delete 1\n");
	if (mCircularQueue)
	{
		mCircularQueue->destory();
		delete mCircularQueue;
	}
	mCircularQueue = NULL;
	printf("bigheadk, StitchingTemplate delete 2\n");
}

PTSParam* StitchingTemplate::getImageParams()
{
	return mCameraParams->image_param;
}

PTSParam* StitchingTemplate::getIGlobalParam()
{
	return &mCameraParams->global_param;
}
void CameraParams::destroy()
{
	for (int i = 0; i < mNumCamera; i++) {
		printf("bigheadk, CameraParams delete 0\n");
		if ( !image_param[i].sourcemask.empty() )
		{
			if (image_param[i].sourcemask.rows > 0 && image_param[i].sourcemask.cols > 0)
			{
				image_param[i].sourcemask.release();
			}
		}
		printf("bigheadk, CameraParams delete 1\n");
		if (image_param[i].source_buffer != NULL)
		{
			free(image_param[i].source_buffer);
			image_param[i].source_buffer = 0;
		}
		printf("bigheadk, CameraParams delete 2\n");
		image_param[i].sourcesize = 0;
		if (warp_info[i].warpMap != NULL)
		{
			free(warp_info[i].warpMap);
			warp_info[i].warpMap = NULL;
		}
		printf("bigheadk, CameraParams delete 3\n");
		if (warp_info[i].inv_warpMap != NULL)
		{
			free(warp_info[i].inv_warpMap);
			warp_info[i].inv_warpMap = NULL;
		}
		if (image_param[i].regional_warping_buffer != NULL)
		{
			free(image_param[i].regional_warping_buffer);
			image_param[i].regional_warping_buffer = NULL;
		}
	}
	if (warp_info != NULL){
		free(warp_info);
		warp_info = NULL;
	}
}

CameraParams::CameraParams(int outWidht, int outHeight, int inputWidth, int inputHeight, int num_cam = CAMERA_MAX)
{
	mNumCamera = num_cam;

	mOutputWidth = outWidht;
	mOutputHeight = outHeight;

	warp_info = (WarpInfo*)malloc(sizeof(WarpInfo)* mNumCamera);
	for (int i = 0; i < mNumCamera; i++)
	{
		warp_info[i].warpMap = (int*)malloc(sizeof(int)* outWidht * outHeight * 2);
		warp_info[i].inv_warpMap = (int*)malloc(sizeof(int)* inputWidth * inputHeight * 2);
		warp_info[i].roi = cv::Rect();
		warp_info[i].data_state = INIT_DATA;
	}

	global_param.f = FULL_FRAME_FISHEYE;   //default lens_type is FullFrame_Fisheye_Lens..
	global_param.y = 0.0f;
	global_param.p = 0.0f;
	global_param.r = 0.0f;

	global_param.v = 120.0f;
	global_param.a = 0.0f;
	global_param.b = 0.0f;
	global_param.c = 0.0f;

	global_param.w = inputWidth;
	global_param.h = inputHeight;

	global_param.top = 0;
	global_param.bottom = inputHeight;
	global_param.left = 0;
	global_param.right = inputWidth;

	global_param.out_f = EQUIRECTANGULAR;
	global_param.out_fov = 360;

	for (int i = 0; i < mNumCamera; i++) {
		image_param[i].f = FULL_FRAME_FISHEYE;
		image_param[i].out_f = EQUIRECTANGULAR;
		image_param[i].out_fov = 360;

		image_param[i].y = 0.0f;
		image_param[i].p = 0.0f;
		image_param[i].r = 0.0f;

		image_param[i].v = 120.0f;
		image_param[i].a = 0.0f;
		image_param[i].b = 0.0f;
		image_param[i].c = 0.0f;

		image_param[i].w = inputWidth;
		image_param[i].h = inputHeight;

		image_param[i].top = 0;
		image_param[i].bottom = inputHeight;
		image_param[i].left = 0;
		image_param[i].right = inputWidth;

		//main memory
		image_param[i].cali_w = image_param[i].w;
		image_param[i].cali_h = image_param[i].h;

		image_param[i].cali_top = image_param[i].top;
		image_param[i].cali_bottom = image_param[i].bottom;
		image_param[i].cali_left = image_param[i].left;
		image_param[i].cali_right = image_param[i].right;

		image_param[i].scale = 1;
		image_param[i].sourcemask = cv::Mat();
		image_param[i].source_buffer = NULL;
		image_param[i].sourcesize = 0;
		image_param[i].isValued = true;
		//image_param[i].output_mask = &roi_buffer;

		image_param[i].regional_warping_buffer = NULL;
		image_param[i].regional_warping_size = 0;
		image_param[i].regional_warping_level = 1;
		image_param[i].is_clear = false;
		image_param[i].is_dirty = false;
	}
	mIsRegionalWarping = false;
}

void CameraParams::setData(CameraParams* camera_params)
{
	for (int i = 0; i < mNumCamera; i++)
	{
		printf("bigheadk, setData 0\n");
		if (warp_info[i].warpMap == NULL)
			warp_info[i].warpMap = (int*)malloc(sizeof(int)* mOutputWidth * mOutputHeight * 2);
		printf("bigheadk, setData 1\n");
		memcpy((void*)warp_info[i].warpMap, (void*)camera_params->warp_info[i].warpMap, sizeof(int)*mOutputWidth * mOutputHeight * 2);
		printf("bigheadk, setData 2\n");
		if (warp_info[i].inv_warpMap == NULL)
			warp_info[i].inv_warpMap = (int*)malloc(sizeof(int)* image_param[i].w * image_param[i].h * 2);
		printf("bigheadk, setData 1\n");
		memcpy((void*)warp_info[i].inv_warpMap, (void*)camera_params->warp_info[i].inv_warpMap, sizeof(int)* image_param[i].w * image_param[i].h * 2);
		warp_info[i].roi = camera_params->warp_info[i].roi;
		printf("bigheadk, setData 3\n");
		warp_info[i].data_state = VALID_DATA;
	}
	printf("bigheadk, setData 4\n");
	global_param.f = camera_params->global_param.f;   //default lens_type is FullFrame_Fisheye_Lens..
	global_param.y = camera_params->global_param.y;
	global_param.p = camera_params->global_param.p;
	global_param.r = camera_params->global_param.r;

	global_param.v = camera_params->global_param.v;
	global_param.a = camera_params->global_param.a;
	global_param.b = camera_params->global_param.b;
	global_param.c = camera_params->global_param.c;

	global_param.w = camera_params->global_param.w;
	global_param.h = camera_params->global_param.h;

	global_param.top = camera_params->global_param.top;
	global_param.bottom = camera_params->global_param.bottom;
	global_param.left = camera_params->global_param.left;
	global_param.right = camera_params->global_param.right;
	printf("bigheadk, setData 5\n");
	for (int i = 0; i < mNumCamera; i++) {
		image_param[i].f = camera_params->image_param[i].f;

		image_param[i].y = camera_params->image_param[i].y;
		image_param[i].p = camera_params->image_param[i].p;
		image_param[i].r = camera_params->image_param[i].r;

		image_param[i].v = camera_params->image_param[i].v;
		image_param[i].a = camera_params->image_param[i].a;
		image_param[i].b = camera_params->image_param[i].b;
		image_param[i].c = camera_params->image_param[i].c;

		image_param[i].w = camera_params->image_param[i].w;
		image_param[i].h = camera_params->image_param[i].h;

		image_param[i].top = camera_params->image_param[i].top;
		image_param[i].bottom = camera_params->image_param[i].bottom;
		image_param[i].left = camera_params->image_param[i].left;
		image_param[i].right = camera_params->image_param[i].right;

		//main memory
		image_param[i].cali_w = camera_params->image_param[i].w;
		image_param[i].cali_h = camera_params->image_param[i].h;

		image_param[i].cali_top = camera_params->image_param[i].top;
		image_param[i].cali_bottom = camera_params->image_param[i].bottom;
		image_param[i].cali_left = camera_params->image_param[i].left;
		image_param[i].cali_right = camera_params->image_param[i].right;

		image_param[i].scale = camera_params->image_param[i].scale;
		image_param[i].isValued = camera_params->image_param[i].isValued;
		//image_param[i].output_mask = camera_params->&roi_buffer;

		//mask
		if (camera_params->image_param[i].source_buffer != NULL){
			if (image_param[i].source_buffer != NULL){
				free(image_param[i].source_buffer);
				image_param[i].source_buffer = NULL;
			}
			image_param[i].source_buffer = (char*)malloc(sizeof(char)*camera_params->image_param[i].sourcesize);
			memcpy(image_param[i].source_buffer, camera_params->image_param[i].source_buffer, camera_params->image_param[i].sourcesize);
			image_param[i].sourcesize = camera_params->image_param[i].sourcesize;
		}
		else{
			if (image_param[i].source_buffer != NULL){
				free(image_param[i].source_buffer);
				image_param[i].source_buffer = NULL;
				image_param[i].sourcesize = 0;
			}
		}
		if (!image_param[i].sourcemask.empty()) {
			image_param[i].sourcemask.release();
		}
		if (!camera_params->image_param[i].sourcemask.empty() && camera_params->image_param[i].sourcemask.rows > 0 && camera_params->image_param[i].sourcemask.cols > 0)
			camera_params->image_param[i].sourcemask.copyTo(image_param[i].sourcemask);

		//rw
		image_param[i].regional_warping_raw_buffer = camera_params->image_param[i].regional_warping_raw_buffer;
		if (camera_params->image_param[i].regional_warping_buffer != NULL){
			if (image_param[i].regional_warping_buffer != NULL){
				free(image_param[i].regional_warping_buffer);
				image_param[i].regional_warping_buffer = NULL;
			}
			image_param[i].regional_warping_buffer = (char*)malloc(sizeof(char)*camera_params->image_param[i].regional_warping_size);
			memcpy(image_param[i].regional_warping_buffer, camera_params->image_param[i].regional_warping_buffer, camera_params->image_param[i].regional_warping_size);
			image_param[i].regional_warping_size = camera_params->image_param[i].regional_warping_size;
		}
		else{
			if (image_param[i].regional_warping_buffer != NULL){
				free(image_param[i].regional_warping_buffer);
				image_param[i].regional_warping_buffer = NULL;
				image_param[i].regional_warping_size = 0;
			}
		}
		image_param[i].mapW = camera_params->image_param[i].mapW;
		image_param[i].mapH = camera_params->image_param[i].mapH;
		image_param[i].regional_warping_level = camera_params->image_param[i].regional_warping_level;
		image_param[i].is_clear = camera_params->image_param[i].is_clear;
		image_param[i].is_dirty = camera_params->image_param[i].is_dirty;
	}
	mIsRegionalWarping = camera_params->mIsRegionalWarping;
}

int HistoryCircularQueue::size()
{
	return (tail - head);
}

bool HistoryCircularQueue::isEmpty()
{
	if (abs(head == tail)){
		return true;
	}
	else{
		return false;
	}
}

bool HistoryCircularQueue::isFull()
{
	if (head == (tail + 1) % max_index){
		return true;
	}
	else{
		return false;
	}
}

void HistoryCircularQueue::pre_enqueue()
{
	if (cur_index != tail)
		tail = cur_index;

	tail++;
	cur_index = tail;

	if (tail > max_index)
	{
		if (head < tail - max_index)
		{
			head = tail - max_index;
		}
	}
}

void HistoryCircularQueue::enqueue(CameraParams* data)
{
	HistoryDataList[(tail-1) % max_index]->setData(data);	
}

void HistoryCircularQueue::destory()
{
	if (HistoryDataList == NULL)
		return;
	for (int i = 0; i < max_index; i++) {
		CameraParams* cam = HistoryDataList[i];
		if (cam != NULL)
		{
			for (int j = 0; j < cam->mNumCamera; j++) {
				//cam->destroy();
				if (!cam->image_param[j].sourcemask.empty())
				{
					if (cam->image_param[j].sourcemask.rows > 0 && cam->image_param[j].sourcemask.cols > 0)
					{
						cam->image_param[j].sourcemask.release();
					}
				}
				printf("bigheadk, HistoryCircularQueue delete 0\n");
				if (cam->image_param[j].source_buffer != 0)
				{
					free(cam->image_param[j].source_buffer);
					cam->image_param[j].source_buffer = NULL;
				}
				printf("bigheadk, HistoryCircularQueue delete 1\n");
				cam->image_param[j].sourcesize = 0;
				printf("bigheadk, HistoryCircularQueue delete 2\n");
				if (cam->image_param[j].regional_warping_buffer != 0)
				{
					free(cam->image_param[j].regional_warping_buffer);
					cam->image_param[j].regional_warping_buffer = NULL;
				}
				cam->image_param[j].regional_warping_size = 0;
				printf("bigheadk, cam[j].warp_info[i] = %p\n", cam->warp_info[j]);
				printf("bigheadk, cam[j].warp_info[i].warpMap = %p\n", cam->warp_info[j].warpMap);
				if (cam->warp_info[j].warpMap != NULL)
				{
					free(cam->warp_info[j].warpMap);
					cam->warp_info[j].warpMap = NULL;
				}

				if (cam->warp_info[j].inv_warpMap != NULL)
				{
					free(cam->warp_info[j].inv_warpMap);
					cam->warp_info[j].inv_warpMap = NULL;
				}
				printf("bigheadk, HistoryCircularQueue delete 3\n");
			}
			printf("bigheadk, HistoryCircularQueue delete 5\n");
			delete cam;
			HistoryDataList[i] = NULL;
			printf("bigheadk, HistoryCircularQueue delete 6\n");
		}
	}
	cur_index = head;
	tail = head;
	free(HistoryDataList);
	HistoryDataList = NULL;
}

bool HistoryCircularQueue::undoEnabled()
{
	bool ret = false;
	printf("undoEnalbed cur_index=%d, head=%d\n", cur_index, head);
	if (cur_index - 1 != head)
		ret =  true;
	return ret;
}

bool HistoryCircularQueue::redoEnabled()
{
	bool ret = false;
	printf("redoEnabled cur_index=%d, tail=%d\n", cur_index, tail);
	if (cur_index != tail)
		ret = true;
	return ret;
}

CameraParams* HistoryCircularQueue::undo()
{
	if (isEmpty())
		return 0;
	if (cur_index - 1 != head)
		cur_index--;

	CameraParams* temp = HistoryDataList[(cur_index - 1)% max_index];
	return temp;
}

CameraParams* HistoryCircularQueue::redo()
{
	if (isEmpty())
		return 0;
	if (cur_index != tail)
		cur_index++;

	CameraParams* temp = HistoryDataList[(cur_index - 1) % max_index];
	return temp;
}
