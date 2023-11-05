#include "IMGVideoCapture.h"

// public functions //

IMGVideoCapture::IMGVideoCapture() {

}

IMGVideoCapture* IMGVideoCapture::create_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, char* imagePath) {
	IMGVideoCapture* instance = new IMGVideoCapture();

	if (instance->init_VideoCapture(iCam, iChannel, captureWidth, captureHeight, capturFPS, imagePath) == false)
		return NULL;

	return instance;
}

BOOL IMGVideoCapture::init_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, char* imagePath) {
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

		printf("Initialize IMGVideoCapture\n");
		return true;
	} while (false);

	return false;
}

BOOL IMGVideoCapture::destroy_VideoCapture(IMGVideoCapture* instance) {
	if (instance) {
		instance->mIsClear = true;
		DWORD result = WaitForSingleObject(instance->mhCaptureThread, 3000);
		if (result == WAIT_OBJECT_0)
			printf("Destroy capture thread\n");
		else if (result == WAIT_TIMEOUT)
			printf("ERROR: Can't destroy capture thread!\n");

		instance->mImg.release();
		delete instance;

		printf("Destroy IMGVideoCapture\n");
		return true;
	}

	printf("ERROR: IMGVideoCapture instance : NULL!\n");
	return false;
}

void IMGVideoCapture::setCaptureWidth(int captureWidth) {
	mCaptureWidth = captureWidth;
}

void IMGVideoCapture::setCaptureHeight(int captureHeight) {
	mCaptureHeight = captureHeight;
}

void IMGVideoCapture::setCapturFPS(float capturFPS) {
	mCapturFPS = capturFPS;
}

void IMGVideoCapture::setStride(DWORD stride) {
	mStride = stride;
}

void IMGVideoCapture::setFrameSize(DWORD frameSize) {
	mFrameSize = frameSize;
}

int IMGVideoCapture::getChannelIndex() {
	return mChannelIndex;
}
int IMGVideoCapture::getCameraIndex() {
	return mCameraIndex;
}

int IMGVideoCapture::getCaptureWidth() {
	return mCaptureWidth;
}

int IMGVideoCapture::getCaptureHeight() {
	return mCaptureHeight;
}

float IMGVideoCapture::getCapturFPS() {
	return mCapturFPS;
}

DWORD IMGVideoCapture::getStride() {
	return mStride;
}

DWORD IMGVideoCapture::getFrameSize() {
	return mFrameSize;
}

char* IMGVideoCapture::getChannelID() {
	return mChannelID;
}

void IMGVideoCapture::setCallback_getAddress(callback_getAddress cb, void* data) {
	mpGetAddressCallback = cb;
	mpGetAddressCallbackData = data;
}

void IMGVideoCapture::setCallback_onCaptureDone(callback_onCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

void IMGVideoCapture::setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data) {
	mpSnapShotDoneCallback = cb;
	mpSnapShotDoneCallbackData = data;
}

void IMGVideoCapture::SnapShot(LONGLONG delay) {
	mSnapShotBuffer = new BYTE[mFrameSize];

	mIsSnapShot = true;
}

// private functions //

bool IMGVideoCapture::isClear() {
	return mIsClear;
}

bool IMGVideoCapture::isSnapShot() {
	return mIsSnapShot;
}

LONGLONG IMGVideoCapture::getSnapShotTime() {
	return mSnapShotTime;
}

BYTE* IMGVideoCapture::getSnapShotBuffer() {
	return mSnapShotBuffer;
}

void IMGVideoCapture::requestChangeCameraIndex(int index, bool enable) {
	mNewCameraIndex = index;
	mNewEnable = enable;
	mIsRequestChangeCameraIndex = 1;
}

void IMGVideoCapture::requestCopyInputToCPU() {
	mIsRequestCopyInputToCPU = 1;
}

bool IMGVideoCapture::isDoneCopyInputToCPU() {
	return mIsRequestCopyInputToCPU;
}

DWORD WINAPI IMGVideoCapture::run(LPVOID handle) {
	IMGVideoCapture *info = (IMGVideoCapture *)handle;

	int width = info->getCaptureWidth();
	int height = info->getCaptureHeight();

	DWORD stride = info->getStride();
	DWORD frameSize = info->getFrameSize();

	while (true) {
		if (info->isClear()) {
			printf("IMGVideoCapture Clear.\n");
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

		memcpy(ptr0, info->mImg.data, frameSize);

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

BYTE* IMGVideoCapture::getAddress(void* handle, void* data, int captureIndex) {
	if (mpGetAddressCallback)
		return (BYTE*)mpGetAddressCallback(handle, data, captureIndex);
	else {
		printf("ERROR: getAddress callback : NULL.\n");
		return NULL;
	}
}