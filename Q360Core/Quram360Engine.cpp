// Quram360Engine.cpp : DLL 응용 프로그램을 위해 내보낸 함수를 정의합니다.
//
#include "Quram360Engine.h"
#include "EngineManager.h"
//#include "stdafx.h"
#include "stdio.h"

EngineManager *mQuramEngineManager = NULL;

bool QVS_Engine::QVS_InitEngineManager()
{
	bool ret = FALSE;
	mQuramEngineManager = new EngineManager();
	if (mQuramEngineManager != NULL)
		ret = TRUE;

	return ret;
}

int QVS_Engine::QVS_Create(int numCamera, int inputWidth, int inputHeight, int outputWidth, int outputHeight, int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->Create(numCamera, inputWidth, inputHeight, outputWidth, outputHeight, mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetSavePath(const char *path)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setSavePath(path);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SaveParam(int is_exit)
{
	if (mQuramEngineManager != NULL && !mQuramEngineManager->getIsDoingAnimation())
		return mQuramEngineManager->SaveParam(is_exit);
	return QVS_FAIL;
}
int QVS_Engine::QVS_LoadParam(const char *path)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->LoadParam(path);
	return QVS_FAIL;
}
int QVS_Engine::QVS_IsNeedSave()
{
	if (mQuramEngineManager != NULL)
		return QVS_TRUE;
	return QVS_FALSE;
}
void QVS_Engine::QVS_SetManualRGB(int idx, int R, int G, int B)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setManualRGB(idx, R, G, B);
}
int QVS_Engine::QVS_DoManualGainMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->doManualGainMode();
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetQuramCalibMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setQuramCalibMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SnapShot(long long delay)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->SnapShot(delay);
	return QVS_FAIL;	
}
void QVS_Engine::QVS_SetSnapshotPath(const char *path)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setSnapshotPath(path);
}
int QVS_Engine::QVS_StartOutput()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->startOutput();
	return QVS_FAIL;
}
int QVS_Engine::QVS_FinishOutput()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->finishOutput();
	return QVS_FAIL;
}

int QVS_Engine::QVS_SetExposureCompensationMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setBlendingMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetSeamMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setSeamMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetGainMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setGainMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetAutoGainMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setAutoGainMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetNadirCompensationMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setNadirCompensationMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetNadirCompensationParam(int position, int size)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setNadirCompensationParam(position, size);
	return QVS_FAIL;
}
int QVS_Engine::QVS_UpdateNadirImage()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->updateNadirImage();
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetAudioBufferOffset(int offset)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setAudioBufferOffset(offset);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetVideoBufferOffset(int offset)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setVideoBufferOffset(offset);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetOutputConfiguration(Configuration *config)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setOutputConfiguration(*config);
	return QVS_FAIL;
}
int QVS_Engine::QVS_SetInputConfiguration(float fps, int width, int height, int lens_type, float fov)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setInputConfiguration(fps, width, height, lens_type, fov);
	return QVS_FAIL;
}
int QVS_Engine::QVS_ParsingPTS(const char *path)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->ParsingPTS(path);
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetRGBIndex(int index)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setRGBIndex(index);
}
int QVS_Engine::QVS_GetRGBIndex()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getRGBIndex();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetRed()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getRed();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetGreen()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGreen();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetBlue()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getBlue();
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetGainChannelIndex(int index)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainChannelIndex(index);
}
void QVS_Engine::QVS_SetGainRGBIndex(int index)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainRGBIndex(index);
}
void QVS_Engine::QVS_SetGainPointIndex(int index)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointIndex(index);
}
int QVS_Engine::QVS_GetNumInput()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getNumInput();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetNumCameara()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getNumCameara();
	return QVS_FAIL;
}
float QVS_Engine::QVS_GetCapureFPS()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCapureFPS();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetCapureWidth()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCapureWidth();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetCapureHeight()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCapureHeight();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetExposureMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getExposureMode();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetSeamMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getSeamMode();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetGainMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainMode();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetNadirMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getNadirMode();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetNadirPosition()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getNadirPosition();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetNadirSize()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getNadirSize();
	return QVS_FAIL;
}
char* QVS_Engine::QVS_GetNadirPath()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getNadirPath();
	return NULL;
}
char* QVS_Engine::QVS_GetSnapshot_path()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getSnapshot_path();
	return NULL;
}
int QVS_Engine::QVS_IsEncoding()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->isEncoding();
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetGainPointX1(double x)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointX1(x);
}
void QVS_Engine::QVS_SetGainPointX2(double x)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointX2(x);
}
void QVS_Engine::QVS_SetGainPointX3(double x)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointX3(x);
}
void QVS_Engine::QVS_SetGainPointY1(double y)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointY1(y);
}
void QVS_Engine::QVS_SetGainPointY2(double y)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointY2(y);
}
void QVS_Engine::QVS_SetGainPointY3(double y)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainPointY3(y);
}
double QVS_Engine::QVS_GetGainPointX1()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainPointX1();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetGainPointX2()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainPointX2();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetGainPointX3()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainPointX3();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetGainPointY1()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainPointY1();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetGainPointY2()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainPointY2();
	return QVS_FAIL;
}
double QVS_Engine::QVS_GetGainPointY3()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainPointY3();
	return QVS_FAIL;
}
void* QVS_Engine::QVS_GetGainCurveData()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getGainCurveData();
	return NULL;
}
void QVS_Engine::QVS_SetGainCurveData(void* data)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGainCurveData(data);
}
int QVS_Engine::QVS_Stitching(int w, int h, int panorama_preview_mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->Stitching(w, h, panorama_preview_mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_Drawing(int w, int h)
{
	if (mQuramEngineManager != NULL)
	{
		mQuramEngineManager->Stitching(w, h, 0);
		return mQuramEngineManager->Drawing(w, h);
	}
	return QVS_FAIL;
}
int QVS_Engine::QVS_DrawingManualCalib(int w, int h)
{
	if (mQuramEngineManager != NULL)
	{
		mQuramEngineManager->Stitching(w, h, 0);
		return mQuramEngineManager->DrawingManualCalib(w, h);
	}
	return QVS_FAIL;
}
int QVS_Engine::QVS_DrawingInteractive(int w, int h)
{
	if (mQuramEngineManager != NULL)
	{
		mQuramEngineManager->Stitching(w, h, 0);
		return mQuramEngineManager->DrawingInteractive(w, h);
	}
	return QVS_FAIL;
}
bool QVS_Engine::QVS_GetDeviceList(int n, char* displayName, char* modelName)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getDeviceList(n, displayName, modelName);
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetPanoramaSize(int panoramaSize)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setPanoramaSize(panoramaSize);
}
int QVS_Engine::QVS_GetPanoramaSize()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getPanoramaSize();
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetAudioDevice(int audioDevice)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setAudioDevice(audioDevice);
}
int QVS_Engine::QVS_GetAudioDevice()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getAudioDevice();
	return QVS_FAIL;
}

int QVS_Engine::QVS_GetBmOutputDevice()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->mBMOutputDevice;
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetBmOutputDisplay()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->mBMOutputDisplay;
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetNadirPath(const char* path)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setNadirPath(path);
}

void QVS_Engine::QVS_SetZoom(float a)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setZoom(a);
}
void QVS_Engine::QVS_SetMultiStreaming(int isMultiStreaming)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setMultiStreaming(isMultiStreaming);
}
void QVS_Engine::QVS_SetPan(float a)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setPan(a);
}
void QVS_Engine::QVS_SetPitch(float a)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setPitch(a);
}
void QVS_Engine::QVS_MulPitch(float a)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->mulPitch(a);
}
int QVS_Engine::QVS_GetAutoGainMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getAutoGainMode();
	return QVS_FAIL;
}
int QVS_Engine::QVS_createVideoCapture(int cameraIndex, int deviceIndex, int cameraType)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->createVideoCapture(cameraIndex, deviceIndex, cameraType);
	return QVS_FAIL;
}
int QVS_Engine::QVS_destroyVideoCapture(int cameraIndex)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->destroyVideoCapture(cameraIndex);
	return QVS_FAIL;
}
int QVS_Engine::QVS_swapVideoCapture(int cameraIndex1, int cameraIndex2)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->swapVideoCapture(cameraIndex1, cameraIndex2);
	return QVS_FAIL;
}
int QVS_Engine::QVS_AudioCaptureUpdate()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->AudioCaptureUpdate();
	return QVS_FAIL;
}
char* QVS_Engine::QVS_getAudioCaptureDeviceName(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getAudioCaptureDeviceName(index);
	return NULL;
}
int QVS_Engine::QVS_getAudioCaptureDeviceSize()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getAudioCaptureDeviceSize();
	return QVS_FAIL;
}
char* QVS_Engine::QVS_getCaptureDeviceName(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCaptureDeviceName(index);
	return NULL;
}
int QVS_Engine::QVS_getCaptureDeviceSize()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCaptureDeviceSize();
	return QVS_FAIL;
}
int QVS_Engine::QVS_setCaptureDeviceIndex(int index, int value)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setCaptureDeviceIndex(index, value);
	return QVS_FAIL;
}
int QVS_Engine::QVS_getCaptureDeviceIndex(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCaptureDeviceIndex(index);
	return QVS_FAIL;
}
int QVS_Engine::QVS_setInputImagePath(int index, char* path)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setInputImagePath(index, path);
	return QVS_FAIL;
}
char* QVS_Engine::QVS_getInputImagePath(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getInputImagePath(index);
	return NULL;
}
void QVS_Engine::QVS_destroyAudioDevice()
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->destroyAudioDevice();
}
int QVS_Engine::QVS_setROIMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setROIMode(mode);
}
int QVS_Engine::QVS_drawROI(int x, int y, int size, int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->drawROI(x, y, size, mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_clacManualMaskPoint2(bool isLeft, int x, int y)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->calcManualMaskPoint2(isLeft, x, y);
	return QVS_FAIL;
}
int QVS_Engine::QVS_drawManualROI(int x, int y, int view_w, int view_h, int size, int mode, int isLeft)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->drawManualROI(x, y, view_w, view_h, size, mode, isLeft);
	return QVS_FAIL;
}
//int QVS_Engine::QVS_drawManualRightROI(int x, int y, int size, int mode, int imgIndex)
//{
//	return mQuramEngineManager->drawManualROI(x, y, 0,0,size, mode, 0);
//}
int QVS_Engine::QVS_InitDrawManualROI()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->InitDrawManualROI();
	return QVS_FAIL;
}

int QVS_Engine::QVS_drawManualPosition(int x, int y, int size)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->drawManualPosition(x, y, size);
	return QVS_FAIL;
}
int QVS_Engine::QVS_saveMask(int imgIndex)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->saveMask(imgIndex);
	return QVS_FAIL;
}
int QVS_Engine::QVS_drawROIFinish()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->drawROIFinish();
	return QVS_FAIL;
}

int QVS_Engine::QVS_clearROI()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->clearROI();
	return QVS_FAIL;
}
int QVS_Engine::QVS_clearManualMaskROI(bool isAll)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->clearManualMaskROI(isAll);
	return QVS_FAIL;
}
int QVS_Engine::QVS_setSelectMode(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setSelectMode(mode);
	return QVS_FAIL;
}
int QVS_Engine::QVS_selectROI(int x, int y, int &isSeleted)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->selectROI(x, y, isSeleted);
	return QVS_FAIL;
}
int QVS_Engine::QVS_selectAllROI()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->selectAllROI();
	return QVS_FAIL;
}
int QVS_Engine::QVS_moveOutput(float x, float y, int isRoll, int isReverseX, int isReverseY)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->moveOutput(x, y, isRoll, isReverseX, isReverseY);
	return QVS_FAIL;
}
bool QVS_Engine::QVS_editOutput()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->editOutput();
	return QVS_FAIL;
}
void QVS_Engine::QVS_saveOutput()
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->saveOutput();
}
int QVS_Engine::QVS_isRedraw()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getIsRedraw();
	return QVS_FAIL;
}

void QVS_Engine::QVS_setCallback_snapshot(callback_snapshot cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackSnapshot(cb);
}
void QVS_Engine::QVS_setCallback_doManualGainMode(callback_doManualGainMode cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackDoManaulGainMode(cb);
}
void QVS_Engine::QVS_setCallback_quramCalibMode(callback_quramCalibMode cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackQuramCalibMode(cb);
}
void QVS_Engine::QVS_setCallback_gainMode(callback_gainMode cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackGainMode(cb);
}
void QVS_Engine::QVS_setCallback_seamMode(callback_seamMode cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackSeamMode(cb);
}
void QVS_Engine::QVS_setCallback_autoGainMode(callback_autoGainMode cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackAutoGainMode(cb);
}
void QVS_Engine::QVS_setCallback_editOutput(callback_autoGainMode cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackEditOutput(cb);
}
void QVS_Engine::QVS_setCallback_calibration(callback_calibration cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackCalibration(cb);
}
void QVS_Engine::QVS_setCallback_doMakeWarpmap(callback_doMakeWarpmap cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackDoMakeWarpmap(cb);
}
void QVS_Engine::QVS_setCallback_history(callback_history cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackHistory(cb);
}

void QVS_Engine::QVS_setCallback_audioError(callback_audioError cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackAudioError(cb);
}

void QVS_Engine::QVS_setCallback_regionWarping(callback_regionWarping cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackRegionWarping(cb);
}
void QVS_Engine::QVS_setCallback_stereoCalibration(callback_stereoCalibration cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackStereoCalibration(cb);
}


void QVS_Engine::QVS_setCallback_load(callback_load cb)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setCallbackLoad(cb);
}

void QVS_Engine::QVS_setDoingLoad(bool b)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setDoingLoad(b);
}

bool QVS_Engine::QVS_getDoingLoad()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getDoingLoad();
}

int QVS_Engine::QVS_VideoCaptureInit()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->VideoCaptureInit();
	return QVS_FAIL;
}

int QVS_Engine::QVS_savePanorama(char* save_path)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->savePanorama(save_path);
	return QVS_FAIL;
}


int QVS_Engine::QVS_getLensType()
{
	if (mQuramEngineManager != NULL)
	{
		int ret = 0, lens_type = mQuramEngineManager->getLensType();
		return lens_type;
	}
	return QVS_FAIL;
}
float QVS_Engine::QVS_getFOV()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getFOV();
	return QVS_FAIL;
}

int QVS_Engine::QVS_saveLensType(int lens_type)
{
	return lens_type;
}

int QVS_Engine::QVS_getLensTypeCount()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getLensTypeCount();
	return QVS_FAIL;
}

char* QVS_Engine::QVS_getLensTypeName(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getLensTypeName(index);
	return NULL;
}

void QVS_Engine::QVS_editOff()
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->editOff();
}
bool QVS_Engine::QVS_isEdit()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->isEdit();
	return QVS_FAIL;
}

int QVS_Engine::QVS_GetOutputWidth()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getOutputWidth();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetOutputHeight()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getOutputHeight();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetCaptureWidth()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCaptureWidth();
	return QVS_FAIL;
}
int QVS_Engine::QVS_GetCaptureHeight()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCaptureHeight();
	return QVS_FAIL;
}
void QVS_Engine::QVS_SetDrawMaskLeftIndex(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setDrawMaskLeftIndex(index);
}
void QVS_Engine::QVS_SetDrawMaskRightIndex(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setDrawMaskRightIndex(index);
}
int QVS_Engine::QVS_GetDrawMaskLeftIndex()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getDrawMaskLeftIndex();
}
int QVS_Engine::QVS_GetDrawMaskRightIndex()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getDrawMaskRightIndex();
}
void QVS_Engine::QVS_SetDrawMaskColor(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setDrawMaskColor(index);
}
void QVS_Engine::QVS_RotateManualMaskLeft(int direction)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->rotateManualMaskLeft(direction);
}
void QVS_Engine::QVS_RotateManualMaskRight(int direction)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->rotateManualMaskRight(direction);
}

void QVS_Engine::QVS_undo()
{
	if (mQuramEngineManager != NULL && !mQuramEngineManager->getIsDoingAnimation() && !mQuramEngineManager->getIsUndoRedo())
		mQuramEngineManager->undo();
}
void QVS_Engine::QVS_redo()
{
	if (mQuramEngineManager != NULL && !mQuramEngineManager->getIsDoingAnimation() && !mQuramEngineManager->getIsUndoRedo())
		mQuramEngineManager->redo();
}
bool QVS_Engine::QVS_undoEnabled()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->undoEnabled();
	return QVS_FAIL;
}
bool QVS_Engine::QVS_redoEnabled()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->redoEnabled();
	return QVS_FAIL;
}

int QVS_Engine::QVS_destroy()
{
	if (mQuramEngineManager != NULL)
	{
		int ret = mQuramEngineManager->Destroy();
		if (ret == QVS_SUCCESS)
		{
			delete mQuramEngineManager;
			mQuramEngineManager = NULL;
		}
		return ret;
	}
	return QVS_FAIL;
}

int QVS_Engine::QVS_getStep(int mode)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getStep(mode);
	return QVS_FAIL;
}

void QVS_Engine::QVS_setStep(int mode, int step)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setStep(mode, step);
}

int QVS_Engine::QVS_getSelectedImage()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getSelectedImage();
	return QVS_FAIL;
}

int QVS_Engine::QVS_FileWrite(int w, int h, int channel, unsigned char* buffer)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->FileWriter(w, h, channel, buffer);

	return QVS_TRUE;
}

int QVS_Engine::QVS_CheckInitParam(int &check)
{
	if (mQuramEngineManager != NULL)
		check = mQuramEngineManager->checkInitParam();
	return QVS_TRUE;
}

int QVS_Engine::QVS_setEditInputMode(int mode)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setEditInputMode(mode);

	return QVS_TRUE;
}

int QVS_Engine::QVS_onSize(int w, int h)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->onSize(w, h);

	return QVS_TRUE;
}

int QVS_Engine::QVS_onDown(int x, int y)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->onDown(x, y);

	return QVS_TRUE;
}

int QVS_Engine::QVS_onUp(int x, int y)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->onUp(x, y);

	return QVS_TRUE;
}

int QVS_Engine::QVS_onScroll(int dx, int dy)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->onScroll(dx, dy);

	return QVS_TRUE;
}
int QVS_Engine::QVS_ZoomIn(float s, int x, int y)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->zoomIn(s, x, y);
}
int QVS_Engine::QVS_ZoomOut(float s, int x, int y)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->zoomOut(s, x, y);
}
int QVS_Engine::QVS_ScaleOutput(float s, int x, int y)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->ScaleOutput(s, x, y);
}
int QVS_Engine::QVS_ResetToFullView()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->resetToFullView();
}
int QVS_Engine::QVS_Scroll(int x, int y, int w, int h)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->scroll(x, y, w, h);
}
int QVS_Engine::QVS_SetViewSize(int w, int h)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setViewSize(w, h);
}
int QVS_Engine::QVS_setCursor(int x, int y)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setCursor(x, y);
}
int QVS_Engine::QVS_getCursorX()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCursorX();
}
int QVS_Engine::QVS_getCursorY()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getCursorY();
}
int QVS_Engine::QVS_getRegionalWarpingGroupCurrentIndex()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getRegionalWarpingGroupCurrentIndex();
}
int QVS_Engine::QVS_getRegionalWarpingGroupSize()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getRegionalWarpingGroupSize();
}

// 171101 for Filter
void QVS_Engine::QVS_setGrayscaleMode(int mode)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setGrayscaleMode(mode);
}
void QVS_Engine::QVS_setBrightnessStep(int step)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setBrightnessStep(step);
}
void QVS_Engine::QVS_setContrastStep(int step)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setContrastStep(step);
}

int QVS_Engine::QVS_getPrepareMode()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getPrepareMode();
}

void QVS_Engine::QVS_setPrepareMode(int mode)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setPrepareMode(mode);
}

int QVS_Engine::QVS_showPrepareView()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->showPrepareView();
}

int QVS_Engine::QVS_hidePrepareView()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->hidePrepareView();
}

void QVS_Engine::QVS_writeLog(char* log, int size)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->writeLog(log, size);
}
void QVS_Engine::QVS_saveLogFile()
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->saveLogFile();
}
void QVS_Engine::QVS_clearLog()
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->clearLog();
}
char* QVS_Engine::QVS_viewLog(bool isFirst)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->viewLog(isFirst);
}

bool QVS_Engine::QVS_getIsDoingAnimation()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getIsDoingAnimation();
}

void QVS_Engine::QVS_setPlanType(int type)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setPlanType(type);
}

int QVS_Engine::QVS_getPlanType()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getPlanType();
}

// 190129 for Stereo
void QVS_Engine::QVS_stereoStitching()
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->stereoStitching();
}

bool QVS_Engine::QVS_getStereoMode()
{
	bool ret = false;
	if (mQuramEngineManager != NULL)
		ret = mQuramEngineManager->getStereoMode();
	return ret;
}

int QVS_Engine::QVS_getStereoPair()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getStereoPair();
}

void QVS_Engine::QVS_setStereoPair(int index)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setStereoPair(index);
}

void QVS_Engine::QVS_getImageParam(int idx, float &yaw, float &pitch, float &roll)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->getImageParam(idx, yaw, pitch, roll);
}

void QVS_Engine::QVS_setImageParam(int idx, float &yaw, float &pitch, float &roll)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setImageParam(idx, yaw, pitch, roll);
}

//191002 for aspect ratio
int QVS_Engine::QVS_setAspectX(int index, int aspectX)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setAspectX(index, aspectX);
	else
		return -1;
}

int QVS_Engine::QVS_setAspectY(int index, int aspectY)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->setAspectY(index, aspectY);
	else
		return -1;
}

int QVS_Engine::QVS_getAspectX(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getAspectX(index);
	else
		return -1;
}

int QVS_Engine::QVS_getAspectY(int index)
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getAspectY(index);
	else
		return -1;
}

double QVS_Engine::QVS_getRenderFPS()
{
	if (mQuramEngineManager != NULL)
		return mQuramEngineManager->getRenderFPS();
	else
		return -1;
}

void QVS_Engine::QVS_setRenderFPS(double fps)
{
	if (mQuramEngineManager != NULL)
		mQuramEngineManager->setRenderFPS(fps);

	return;
}