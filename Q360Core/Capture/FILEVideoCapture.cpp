#include "FILEVideoCapture.h"

// public functions //

FILEVideoCapture::FILEVideoCapture() {

}

FILEVideoCapture* FILEVideoCapture::create_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, char* videoPath) {
	FILEVideoCapture* instance = new FILEVideoCapture();

	if (instance->init_VideoCapture(iCam, iChannel, captureWidth, captureHeight, capturFPS, videoPath) == false)
		return NULL;

	return instance;
}

BOOL FILEVideoCapture::init_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, char* videoPath) {
	mIsClear = false;
	mIsSnapShot = false;
	mIsRequestChangeCameraIndex = 0;
	mIsRequestCopyInputToCPU = 0;
	mpGetAddressCallback = NULL;
	mpGetAddressCallbackData = NULL;
	mpCaptureDoneCallback = NULL;
	mpCaptureDoneCallbackData = NULL;

	mCameraIndex = iCam;
	mChannelIndex = iChannel;
	mCaptureWidth = captureWidth;
	mCaptureHeight = captureHeight;
	mCapturFPS = capturFPS;

	do {
		/*
		printf("imgPath %s\n", imagePath);
		mImg = cv::imread(imagePath);

		sprintf(mChannelID, "IMAGE %d", mCameraIndex);

		if (mImg.rows == 0) {
			printf("ERROR: Image loading fail!\n");
			uchar* data = (uchar*)malloc(mCaptureWidth * mCaptureHeight * 3);
			memset(data, 0, mCaptureWidth * mCaptureHeight * 3);
			mImg = cv::Mat(mCaptureHeight, mCaptureWidth, CV_8UC3, data);
			break;
		}
		else if (mImg.cols != mCaptureWidth || mImg.rows != mCaptureHeight) {
			cv::resize(mImg, mImg, cv::Size(mCaptureWidth, mCaptureHeight), 0, 0, CV_INTER_LINEAR);
			cv::cvtColor(mImg, mImg, CV_BGR2RGB);
		}
		else
		{
			printf("GOOD: Image loading succ!\n");
			cv::cvtColor(mImg, mImg, CV_BGR2RGB);
		}

		mStride = mCaptureWidth * 3;
		mFrameSize = mStride * mCaptureHeight;

		mhCaptureThread = CreateThread(NULL, 0, run, this, 0, NULL);
		if (mhCaptureThread == NULL) {
			printf("ERROR: Can't create video capture thread!\n");
			break;
		}
		*/
		
		break;

		printf("Initialize FILEVideoCapture\n");
		return true;
	} while (false);

	return false;
}

BOOL FILEVideoCapture::destroy_VideoCapture(FILEVideoCapture* instance) {
	if (instance) {
		instance->mIsClear = true;
		delete instance;

		printf("Destroy FILEVideoCapture\n");
		return true;
	}

	printf("ERROR: FILEVideoCapture instance : NULL!\n");
	return false;
}

void FILEVideoCapture::setCaptureWidth(int captureWidth) {
	mCaptureWidth = captureWidth;
}

void FILEVideoCapture::setCaptureHeight(int captureHeight) {
	mCaptureHeight = captureHeight;
}

void FILEVideoCapture::setCapturFPS(float capturFPS) {
	mCapturFPS = capturFPS;
}

void FILEVideoCapture::setStride(DWORD stride) {
	mStride = stride;
}

void FILEVideoCapture::setFrameSize(DWORD frameSize) {
	mFrameSize = frameSize;
}

int FILEVideoCapture::getChannelIndex() {
	return mChannelIndex;
}
int FILEVideoCapture::getCameraIndex() {
	return mCameraIndex;
}

int FILEVideoCapture::getCaptureWidth() {
	return mCaptureWidth;
}

int FILEVideoCapture::getCaptureHeight() {
	return mCaptureHeight;
}

float FILEVideoCapture::getCapturFPS() {
	return mCapturFPS;
}

DWORD FILEVideoCapture::getStride() {
	return mStride;
}

DWORD FILEVideoCapture::getFrameSize() {
	return mFrameSize;
}

char* FILEVideoCapture::getChannelID() {
	return mChannelID;
}

void FILEVideoCapture::setCallback_getAddress(callback_getAddress cb, void* data) {
	mpGetAddressCallback = cb;
	mpGetAddressCallbackData = data;
}

void FILEVideoCapture::setCallback_onCaptureDone(callback_onCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

void FILEVideoCapture::setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data) {
	mpSnapShotDoneCallback = cb;
	mpSnapShotDoneCallbackData = data;
}

void FILEVideoCapture::SnapShot(LONGLONG delay) {
	mSnapShotBuffer = new BYTE[mFrameSize];

	mIsSnapShot = true;
}

// private functions //

bool FILEVideoCapture::isClear() {
	return mIsClear;
}

bool FILEVideoCapture::isSnapShot() {
	return mIsSnapShot;
}

LONGLONG FILEVideoCapture::getSnapShotTime() {
	return mSnapShotTime;
}

BYTE* FILEVideoCapture::getSnapShotBuffer() {
	return mSnapShotBuffer;
}

void FILEVideoCapture::requestChangeCameraIndex(int index, bool enable) {
	mNewCameraIndex = index;
	mNewEnable = enable;
	mIsRequestChangeCameraIndex = 1;
}

void FILEVideoCapture::requestCopyInputToCPU() {
	mIsRequestCopyInputToCPU = 1;
}

bool FILEVideoCapture::isDoneCopyInputToCPU() {
	return mIsRequestCopyInputToCPU;
}

DWORD WINAPI FILEVideoCapture::run(LPVOID handle) {
	FILEVideoCapture *info = (FILEVideoCapture *)handle;

	int width = info->getCaptureWidth();
	int height = info->getCaptureHeight();

	DWORD stride = info->getStride();
	DWORD frameSize = info->getFrameSize();

	while (true) {
		if (info->isClear()) {
			printf("FILEVideoCapture Clear.\n");
			break;
		}

		static LONGLONG timeStamp = 0;
		DWORD sleeptime = (DWORD)(1000 / info->getCapturFPS());
		Sleep(sleeptime);
		timeStamp += sleeptime;

		if (info->mIsRequestChangeCameraIndex == 1) {
			info->mCameraIndex = info->mNewCameraIndex;
			info->mIsRequestChangeCameraIndex = 0;
		}

		BYTE* ptr0 = info->getAddress(info, info->mpGetAddressCallbackData, info->mChannelIndex);
		if (ptr0 == NULL)
			continue;

		if (info->mIsRequestCopyInputToCPU == 1) {
			
			BYTE* ptr1 = info->getAddress(info, info->mpGetAddressCallbackData, 1);
			if (ptr1 == NULL) {
				// do nothing...
				printf("ERROR: Get CPU Memory Address Fail!\n");
			}
			else {
				memcpy(ptr1, ptr0, info->getFrameSize());
				printf("Copy %d Input Data To CPU\n", info->getCameraIndex());

				if (info->mpCaptureDoneCallback)
					info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, timeStamp / 10000, 1);
			}
			info->mIsRequestCopyInputToCPU = 0;
		}

		if (info->isSnapShot()) {
			BYTE* snapShotBuffer = info->getSnapShotBuffer();
			if (snapShotBuffer != NULL) {
				memcpy(snapShotBuffer, ptr0, info->getFrameSize());
				info->mIsSnapShot = false;
				info->mpSnapShotDoneCallback(info, info->mpSnapShotDoneCallbackData, snapShotBuffer, info->getFrameSize(), timeStamp);
			}
		}

		if (info->mpCaptureDoneCallback)
			info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, timeStamp, info->mChannelIndex);
	}

	return 0;
}

BYTE* FILEVideoCapture::getAddress(void* handle, void* data, int captureIndex) {
	if (mpGetAddressCallback)
		return (BYTE*)mpGetAddressCallback(handle, data, captureIndex);
	else {
		printf("ERROR: getAddress callback : NULL.\n");
		return NULL;
	}
}