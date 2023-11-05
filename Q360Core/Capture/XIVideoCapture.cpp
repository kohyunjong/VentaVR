#include "XIVideoCapture.h"

// public functions //

XIVideoCapture::XIVideoCapture() {

}

XIVideoCapture* XIVideoCapture::create_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, cv::Rect cropRect) {
	XIVideoCapture* instance = new XIVideoCapture();

	instance->init_VideoCapture(iCam, iDevice, iChannel, captureWidth, captureHeight, capturFPS, colorSpace, cropRect);

	return instance;
}

BOOL XIVideoCapture::init_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, cv::Rect cropRect) {
	mIsClear = false;
	mIsSnapShot = false;
	mhChannel = NULL;
	mIsRequestCopyInputToCPU = 0;
	mRGBInput = cv::Mat(cv::Size(captureWidth, captureHeight), CV_8UC3);
	//mBGRInput = cv::Mat(cv::Size(captureWidth, captureHeight), CV_8UC3);
	mYUVInput = cv::Mat(cv::Size(captureWidth, captureHeight), CV_8UC2);
	mpGetAddressCallback = NULL;
	mpGetAddressCallbackData = NULL;
	mpCaptureDoneCallback = NULL;
	mpCaptureDoneCallbackData = NULL;

	mCameraIndex = iCam;
	mChannelIndex = iChannel;
	mCaptureWidth = captureWidth;
	mCaptureHeight = captureHeight;
	mCapturFPS = capturFPS;
	mColorSpace = colorSpace;

	mCropRect = cropRect;

	do {
		//if (!XIS_Initialize()) {
		//	printf("ERROR: XIS_Initialize return fail!\n");
		//	break;
		//}

		int cVideoCaptures = XIS_GetVideoCaptureCount();
		if (cVideoCaptures <= 0) {
			printf("ERROR: Can't find capture card!\n");
			break;
		}

		VIDEO_CAPTURE_INFO vidCapInfo;
		int iUSBDevice = 0;
		bool isFind = false;
		for (int i = 0; i < cVideoCaptures; i++) {
			if (!XIS_GetVideoCaptureInfo(i, &vidCapInfo)) {
				//VIDEO_CAPTURE_INFO_EX vidCapInfo;
				//if (!XIS_GetVideoCaptureInfoEx(mCameraIndex, &vidCapInfo)) {
				printf("ERROR: Can't get channel info!\n");
				break;
			}

			if (_tcsstr(vidCapInfo.szDShowID, _T("usb")) || _tcsstr(vidCapInfo.szName, _T("USB"))) {
				if (iUSBDevice == iDevice) {
					isFind = true;
					break;
				}
				else
					iUSBDevice++;
			}
		}
		if (isFind == false) {
			printf("ERROR: Can't find USB device!\n");
			break;
		}

		mhChannel = XIS_OpenVideoCapture(vidCapInfo.szDShowID);
		if (NULL == mhChannel) {
			printf("ERROR: Open channel %s:%d error!\n", vidCapInfo.szDShowID, iDevice);
			break;
		}

		//HANDLE m_hProperty = XIS_OpenVideoCapturePropertyHandle(mhChannel);
		//HRESULT result123 = XIPHD_SetFlipMode(m_hProperty, TRUE, TRUE);
		//printf("result123 : %d\n", result123);

		if (!XIS_SetVideoCaptureFormat(mhChannel, (XI_COLOR_FORMAT)mColorSpace, mCaptureWidth, mCaptureHeight, (int)(10000000.f / mCapturFPS))) {
			printf("ERROR: XIS_SetVideoCaptureFormat return fail!\n");
			break;
		}

		if (!XIS_SetVideoCaptureCallbackEx(mhChannel, onCapture, this, FALSE)) {
			printf("ERROR: XIS_SetVideoCaptureCallbackEx return fail!\n");
			break;
		}

		sprintf(mChannelID, "%s(%d)", vidCapInfo.szName, iDevice);

		if (!XIS_StartVideoCapture(mhChannel)) {
			printf("ERROR: Start video capture fail!\n");
			break;
		}

		mStride = mCaptureWidth * 3;
		if (mCropRect.width > 0 && mCropRect.height > 0)
		{
			mFrameSize = mCropRect.width*mCropRect.height * 3;
		}
		else
		{
			mFrameSize = mCaptureWidth * mCaptureHeight * 3;
		}

		printf("Initialize XICapture SDK WH(%d,%d) F(%f)\n", mCaptureWidth, mCaptureHeight, mCapturFPS);
		return TRUE;
	} while (false);

	return FALSE;
}

BOOL XIVideoCapture::destroy_VideoCapture(XIVideoCapture* instance) {
	if (instance) {
		instance->mIsClear = true;

		if (instance->mhChannel) {
			XIS_StopVideoCapture(instance->mhChannel);
			XIS_CloseVideoCapture(instance->mhChannel);
			instance->mhChannel = NULL;
		}

		delete instance;

		printf("Destroy XICapture SDK\n");
		return TRUE;
	}

	printf("ERROR: XIVideoCapture instance : NULL!\n");
	return FALSE;
}

void XIVideoCapture::setCaptureWidth(int captureWidth) {
	mCaptureWidth = captureWidth;
}

void XIVideoCapture::setCaptureHeight(int captureHeight) {
	mCaptureHeight = captureHeight;
}

void XIVideoCapture::setCapturFPS(float capturFPS) {
	mCapturFPS = capturFPS;
}

void XIVideoCapture::setColorSpace(DWORD colorSpace) {
	mColorSpace = colorSpace;
}

void XIVideoCapture::setStride(DWORD stride) {
	mStride = stride;
}

void XIVideoCapture::setFrameSize(DWORD frameSize) {
	mFrameSize = frameSize;
}

void XIVideoCapture::setHChannel(HANDLE hChannel) {
	mhChannel = hChannel;
}

int XIVideoCapture::getChannelIndex() {
	return mChannelIndex;
}
int XIVideoCapture::getCameraIndex() {
	return mCameraIndex;
}

int XIVideoCapture::getCaptureWidth() {
	return mCaptureWidth;
}

int XIVideoCapture::getCaptureHeight() {
	return mCaptureHeight;
}

float XIVideoCapture::getCapturFPS() {
	return mCapturFPS;
}

DWORD XIVideoCapture::getColorSpace() {
	return mColorSpace;
}

DWORD XIVideoCapture::getStride() {
	return mStride;
}

DWORD XIVideoCapture::getFrameSize() {
	return mFrameSize;
}

HANDLE XIVideoCapture::getHChannel() {
	return mhChannel;
}

char* XIVideoCapture::getChannelID() {
	return mChannelID;
}

void XIVideoCapture::setCallback_getAddress(callback_getAddress cb, void* data) {
	mpGetAddressCallback = cb;
	mpGetAddressCallbackData = data;
}

void XIVideoCapture::setCallback_onCaptureDone(callback_onCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

void XIVideoCapture::setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data) {
	mpSnapShotDoneCallback = cb;
	mpSnapShotDoneCallbackData = data;
}

void XIVideoCapture::SnapShot(LONGLONG delay) {
	mSnapShotBuffer = new BYTE[mFrameSize];

	mSnapShotTime = mLastFrameTime + delay;
	mIsSnapShot = true;
}

// private functions //

bool XIVideoCapture::isClear() {
	return mIsClear;
}

bool XIVideoCapture::isSnapShot() {
	return mIsSnapShot;
}

LONGLONG XIVideoCapture::getSnapShotTime() {
	return mSnapShotTime;
}

BYTE* XIVideoCapture::getSnapShotBuffer() {
	return mSnapShotBuffer;
}

void XIVideoCapture::requestChangeCameraIndex(int index, bool enable) {
	mNewCameraIndex = index;
	mNewEnable = enable;
	mIsRequestChangeCameraIndex = 1;
}

void XIVideoCapture::requestCopyInputToCPU() {
	mIsRequestCopyInputToCPU = 1;
}

bool XIVideoCapture::isDoneCopyInputToCPU() {
	return mIsRequestCopyInputToCPU;
}

#include "clock.h"

void XIVideoCapture::onCapture(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp) {
	XIVideoCapture *info = (XIVideoCapture*)pvParam;
	if (info == NULL)
		return;

	if (info->isClear()) {
		printf("XICaptureSDK Clear.\n");
		return;
	}

	void* ptr0 = info->getAddress(info, info->mpGetAddressCallbackData, info->mChannelIndex);
	if (ptr0 == NULL)
		return;

	if (info->mCropRect.width > 0 && info->mCropRect.height > 0)
	{
		info->mYUVInput.data = (uchar*)pbyImage;
		cv::cvtColor(info->mYUVInput, info->mRGBInput, CV_YUV2RGB_YUYV);
		memcpy(ptr0, info->mRGBInput.data + ((info->mCropRect.y*info->mCropRect.width + info->mCropRect.x) * 3), info->mCropRect.width*info->mCropRect.height * 3);
	}
	else
	{
		info->mYUVInput.data = (uchar*)pbyImage;
		info->mRGBInput.data = (uchar*)ptr0;
		cv::cvtColor(info->mYUVInput, info->mRGBInput, CV_YUV2RGB_YUYV);
	}

	if (info->mIsRequestCopyInputToCPU == 1) {

		BYTE* ptr1 = info->getAddress(info, info->mpGetAddressCallbackData, 1);
		if (ptr1 == NULL) {
			// do nothing...
			printf("ERROR: Get CPU Memory Address Fail!\n");
		}
		else {
			memcpy(ptr1, ptr0, info->mFrameSize);
			printf("Copy %d Input Data To CPU\n", info->getCameraIndex());

			if (info->mpCaptureDoneCallback)
				info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, info->mFrameSize, 1);
		}
		info->mIsRequestCopyInputToCPU = 0;
	}

	if (info->isSnapShot()) {
		BYTE* snapShotBuffer = info->getSnapShotBuffer();
		if (u64TimeStamp > info->getSnapShotTime() && snapShotBuffer != NULL) {
			memcpy(snapShotBuffer, ptr0, info->mFrameSize);
			info->mIsSnapShot = false;
			info->mpSnapShotDoneCallback(info, info->mpSnapShotDoneCallbackData, snapShotBuffer, info->mFrameSize, u64TimeStamp);
		}
	}

	if (info->mpCaptureDoneCallback)
		info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, u64TimeStamp, 0);
	else
		printf("ERROR: onCaptureDone callback : NULL.\n");

	info->mLastFrameTime = u64TimeStamp;
}

BYTE* XIVideoCapture::getAddress(void* handle, void* data, int captureIndex) {
	if (mpGetAddressCallback)
		return (BYTE*)mpGetAddressCallback(handle, data, captureIndex);
	else {
		printf("ERROR: getAddress callback : NULL.\n");
		return NULL;
	}
}