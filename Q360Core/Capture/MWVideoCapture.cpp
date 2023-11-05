#include "MWVideoCapture.h"

// public functions //

MWVideoCapture::MWVideoCapture() {

}

MWVideoCapture* MWVideoCapture::create_VideoCapture(int iCam, int iDevice, int iChannel, int iID, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, bool isCamOverlay, cv::Rect cropRect) {
	MWVideoCapture* instance = new MWVideoCapture();

	instance->init_VideoCapture(iCam, iDevice, iChannel, iID, captureWidth, captureHeight, capturFPS, colorSpace, cropRect);
	instance->isCam = isCamOverlay;
	instance->mAspectX = 16;
	instance->mAspectY = 9;

	return instance;
}

BOOL MWVideoCapture::init_VideoCapture(int iCam, int iDevice, int iChannel, int iID, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, cv::Rect cropRect) {
	mIsClear = false;
	mIsSnapShot = false;
	mhChannel = NULL;
	mIsRequestChangeCameraIndex = 0;
	mIsRequestCopyInputToCPU = 0;
	mIsRequestChangeFPS = 0;
	mIsMultiStreaming = 0;
	mpGetAddressCallback = NULL;
	mpGetAddressCallbackData = NULL;
	mpCaptureDoneCallback = NULL;
	mpCaptureDoneCallbackData = NULL;

	mCameraIndex = iCam;
	mChannelIndex = iChannel;
	mIsEnable = true;
	mCaptureWidth = captureWidth;
	mCaptureHeight = captureHeight;
	mCapturFPS = capturFPS;
	mColorSpace = colorSpace;
	mTime = QClock();
	mID = iID;

	mCropRect = cropRect;

	do {
		/*
		if (!MWCaptureInitInstance()) {
			printf("ERROR: MWCaptureInitInstance return fail!\n");
			break;
		}
		*/

		MWCAP_CHANNEL_INFO videoInfo = { 0 };
		TCHAR szDevicePath[MAX_PATH];
		if (MW_SUCCEEDED != MWGetDevicePath(iDevice, szDevicePath)) {
			printf("ERROR: Can't get Device Path!\n");
			break;
		}

		mhChannel = MWOpenChannelByPath(szDevicePath);
		if (mhChannel == NULL) {
			printf("ERROR: Open channel %s:%d error!\n", szDevicePath, iDevice);
			break;
		}

		if (MW_SUCCEEDED != MWGetChannelInfo(mhChannel, &videoInfo)) {
			printf("ERROR: Can't get channel info!\n");
			break;
		}

		sprintf(mChannelID, "%s(%d)", videoInfo.szBoardSerialNo, videoInfo.byChannelIndex);

		mStride = FOURCC_CalcMinStride(mColorSpace, mCaptureWidth, 2);

		if (mCropRect.width > 0 && mCropRect.height > 0)
		{
			//mFrameSize = mCropRect.width*mCropRect.height * 3;
			mFrameSize = FOURCC_CalcImageSize(mColorSpace, mCaptureWidth, mCaptureHeight, mStride);
			buffer = (unsigned char*)malloc(mFrameSize);
		}
		else
		{
			mFrameSize = FOURCC_CalcImageSize(mColorSpace, mCaptureWidth, mCaptureHeight, mStride);
		}

		mhCaptureThread = CreateThread(NULL, 0, run, this, 0, NULL);
		if (mhCaptureThread == NULL) {
			printf("ERROR: Can't create video capture thread!\n");
			break;
		}

		printf("Initialize MWCapture SDK\n");
		return MW_SUCCEEDED;
	} while (false);

	return MW_FAILED;
}

BOOL MWVideoCapture::destroy_VideoCapture(MWVideoCapture* instance) {
	if (instance) {
		instance->mIsClear = true;

		DWORD result = WaitForSingleObject(instance->mhCaptureThread, 3000);
		if (result == WAIT_OBJECT_0)
			printf("Destroy capture thread\n");
		else if (result == WAIT_TIMEOUT)
			printf("ERROR: Can't destroy capture thread!\n");

		delete instance;

		printf("Destroy MWCapture SDK\n");
		return MW_SUCCEEDED;
	}

	printf("ERROR: MWVideoCapture instance : NULL!\n");
	return MW_FAILED;
}

void MWVideoCapture::setCaptureWidth(int captureWidth) {
	mCaptureWidth = captureWidth;
}

void MWVideoCapture::setCaptureHeight(int captureHeight) {
	mCaptureHeight = captureHeight;
}

void MWVideoCapture::setCapturFPS(float capturFPS) {
	mCapturFPS = capturFPS;
	mIsRequestChangeFPS = 1;
}

void MWVideoCapture::setColorSpace(DWORD colorSpace) {
	mColorSpace = colorSpace;
}

void MWVideoCapture::setStride(DWORD stride) {
	mStride = stride;
}

void MWVideoCapture::setFrameSize(DWORD frameSize) {
	mFrameSize = frameSize;
}

void MWVideoCapture::setHChannel(HCHANNEL hChannel) {
	mhChannel = hChannel;
}

void MWVideoCapture::setAspectX(int aspectX) {
	mAspectX = aspectX;
}

void MWVideoCapture::setAspectY(int aspectY) {
	mAspectY = aspectY;
}

int MWVideoCapture::getChannelIndex() {
	return mChannelIndex;
}
int MWVideoCapture::getCameraIndex() {
	return mCameraIndex;
}

int MWVideoCapture::getCaptureWidth() {
	return mCaptureWidth;
}

int MWVideoCapture::getCaptureHeight() {
	return mCaptureHeight;
}

float MWVideoCapture::getCapturFPS() {
	return mCapturFPS;
}

DWORD MWVideoCapture::getColorSpace() {
	return mColorSpace;
}

DWORD MWVideoCapture::getStride() {
	return mStride;
}

DWORD MWVideoCapture::getFrameSize() {
	return mFrameSize;
}

HCHANNEL MWVideoCapture::getHChannel() {
	return mhChannel;
}

char* MWVideoCapture::getChannelID() {
	return mChannelID;
}

int MWVideoCapture::getAspectX() {
	return mAspectX;
}

int MWVideoCapture::getAspectY() {
	return mAspectY;
}

void MWVideoCapture::setCallback_getAddress(callback_getAddress cb, void* data) {
	mpGetAddressCallback = cb;
	mpGetAddressCallbackData = data;
}

void MWVideoCapture::setCallback_onCaptureDone(callback_onCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

void MWVideoCapture::setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data) {
	mpSnapShotDoneCallback = cb;
	mpSnapShotDoneCallbackData = data;
}

void MWVideoCapture::SnapShot(LONGLONG delay) {
	mSnapShotBuffer = new BYTE[mFrameSize];

	LONGLONG timeStamp = 0LL;
	MWGetDeviceTime(mhChannel, &timeStamp);

	mSnapShotTime = timeStamp + delay;

	mIsSnapShot = true;
}

// private functions //

bool MWVideoCapture::isEnable() {
	return mIsEnable;
}

bool MWVideoCapture::isClear() {
	return mIsClear;
}

bool MWVideoCapture::isSnapShot() {
	return mIsSnapShot;
}

LONGLONG MWVideoCapture::getSnapShotTime() {
	return mSnapShotTime;
}

BYTE* MWVideoCapture::getSnapShotBuffer() {
	return mSnapShotBuffer;
}

int MWVideoCapture::getStatus() {
	MWCAP_VIDEO_SIGNAL_STATUS videoSignalStatus;
	MWGetVideoSignalStatus(mhChannel, &videoSignalStatus);
	return videoSignalStatus.state;
}

void MWVideoCapture::requestChangeCameraIndex(int index, bool enable) {
	mNewCameraIndex = index;
	mNewEnable = enable;
	mIsRequestChangeCameraIndex = 1;
}

void MWVideoCapture::requestCopyInputToCPU() {
	mIsRequestCopyInputToCPU = 1;
}

bool MWVideoCapture::isDoneCopyInputToCPU() {
	return mIsRequestCopyInputToCPU;
}

void MWVideoCapture::setMultiStreaming(int isMultiStreaming) {
	mIsMultiStreaming = isMultiStreaming;
}

DWORD WINAPI MWVideoCapture::run(LPVOID handle) {
	MWVideoCapture *info = (MWVideoCapture *)handle;

	int width = info->getCaptureWidth();
	int height = info->getCaptureHeight();

	DWORD stride = info->getStride();
	DWORD frameSize = info->getFrameSize();

	HCHANNEL hChannel = info->getHChannel();

	HANDLE hCaptureEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	MW_RESULT xr;

	if (info->isCam)
		Sleep(1000);

	//printf("bigheadk, thread start!!\n");

	do {
		xr = MWStartVideoCapture(hChannel, hCaptureEvent);
		if (xr != MW_SUCCEEDED) {
			printf("ERROR: Open Video Capture error!\n");
			break;
		}

		LONGLONG llBegin = 0LL;
		xr = MWGetDeviceTime(hChannel, &llBegin);
		if (xr != MW_SUCCEEDED)
			break;

		LONGLONG llExpireTime = llBegin;
		double dwFrameDuration = (DWORD)(1000 / info->getCapturFPS());
		info->mTime = QClock();

		while (true) {
			//printf("bigheadk, thread 1!!\n");
			if (info->isClear()) {
				printf("MWCaptureSDK Clear.\n");
				break;
			}

			if (info->mIsRequestChangeFPS == 1) {
				dwFrameDuration = (DWORD)(1000 / info->getCapturFPS());
				info->mIsRequestChangeFPS = 0;
			}
			
			while (abs(info->mTime - QClock()) < dwFrameDuration)
			{
				Sleep(1);
			}

			info->mTime = QClock() + (abs(QClock() - info->mTime) - dwFrameDuration);
			
			/*
			while (1)
			{
				MWCAP_VIDEO_CAPTURE_STATUS status;
				MWGetVideoCaptureStatus(hChannel, &status);

				if (status.bFrameCompleted)
					break;
				printf("bigheadk, capture status = %d\n", status.cyCompleted);
				printf("bigheadk, capture status = %d\n", status.iFrame);
				Sleep(1);
			}

			*/

			

			if (info->mIsRequestChangeCameraIndex == 1) {
				info->mCameraIndex = info->mNewCameraIndex;
				info->mIsEnable = info->mNewEnable;
				info->mIsRequestChangeCameraIndex = 0;
			}

			//printf("bigheadk, thread 2!!\n");
			if (info->isEnable() == false) {
				continue;
			}

			//printf("bigheadk, thread 2-1!!\n");
			if (info->mChannelIndex == 1 && info->mIsMultiStreaming == false) {
				continue;
			}
			//printf("bigheadk, thread 2-2!!\n");
			if (info->mpGetAddressCallback == NULL || info->mpGetAddressCallbackData == NULL) {
				continue;
			}
			//printf("bigheadk, thread 2-3!!\n");
			BYTE* ptr0;
			if (info->getCameraIndex() == -1)
				ptr0 = info->getAddress(info, info->mpGetAddressCallbackData, info->mID);
			else
				ptr0 = info->getAddress(info, info->mpGetAddressCallbackData, info->mChannelIndex);
			if (ptr0 == NULL)
				continue;

			//printf("bigheadk, thread 2-4!!\n");
			LONGLONG timeStamp = 0LL;
			//printf("bigheadk, thread 2-5!!\n");
			MWGetDeviceTime(info->mhChannel, &timeStamp);

			//printf("bigheadk, thread 2-6!!\n");
			int CAPTURE_PART_LINE = 0;
			//printf("bigheadk, thread 3!!\n");
			if (info->mCropRect.width > 0 && info->mCropRect.height > 0)
			{
				MWCaptureVideoFrameToVirtualAddressEx(hChannel, MWCAP_VIDEO_FRAME_ID_NEWEST_BUFFERED,
					(BYTE*)info->buffer, frameSize, stride, FALSE, NULL, info->getColorSpace(), width, height,
					0, CAPTURE_PART_LINE, NULL, NULL, 0, 100, 0, 100, 0, MWCAP_VIDEO_DEINTERLACE_BLEND, MWCAP_VIDEO_ASPECT_RATIO_CROPPING, NULL, NULL, MAX(info->getAspectX(), 0), MAX(info->getAspectY(), 0), MWCAP_VIDEO_COLOR_FORMAT_UNKNOWN, MWCAP_VIDEO_QUANTIZATION_UNKNOWN, MWCAP_VIDEO_SATURATION_UNKNOWN);

				if (info->mCropRect.width < width)
				{
					for (int i = 0; i < info->mCropRect.height; i++)
					{
						memcpy(ptr0 + ((i*info->mCropRect.width * 3)), info->buffer + (((i + info->mCropRect.y)*width + info->mCropRect.x) * 3), info->mCropRect.width * 3);
					}
				}
				else
					memcpy(ptr0, info->buffer + ((info->mCropRect.y*info->mCropRect.width + info->mCropRect.x) * 3), info->mCropRect.width*info->mCropRect.height * 3);
			}
			else
			{
				MWCaptureVideoFrameToVirtualAddressEx(hChannel, MWCAP_VIDEO_FRAME_ID_NEWEST_BUFFERED,
					ptr0, frameSize, stride, FALSE, NULL, info->getColorSpace(), width, height,
					0, CAPTURE_PART_LINE, NULL, NULL, 0, 100, 0, 100, 0, MWCAP_VIDEO_DEINTERLACE_BLEND, MWCAP_VIDEO_ASPECT_RATIO_CROPPING, NULL, NULL, MAX(info->getAspectX(), 0), MAX(info->getAspectY(), 0), MWCAP_VIDEO_COLOR_FORMAT_UNKNOWN, MWCAP_VIDEO_QUANTIZATION_UNKNOWN, MWCAP_VIDEO_SATURATION_UNKNOWN);
			}

			WaitForSingleObject(hCaptureEvent, 100);

			//printf("bigheadk, thread 4!!\n");
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
				if (timeStamp > info->getSnapShotTime() && snapShotBuffer != NULL) {
					memcpy(snapShotBuffer, ptr0, info->getFrameSize());
					info->mIsSnapShot = false;
					info->mpSnapShotDoneCallback(info, info->mpSnapShotDoneCallbackData, snapShotBuffer, info->getFrameSize(), timeStamp);
				}
			}


			if (info->mpCaptureDoneCallback)
				info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, timeStamp / 10000, info->mChannelIndex);
			else
				printf("ERROR: onCaptureDone callback : NULL.\n");
		}

		xr = MWStopVideoCapture(hChannel);

	} while (false);

	CloseHandle(hCaptureEvent);

	return 0;
}

BYTE* MWVideoCapture::getAddress(void* handle, void* data, int captureIndex) {
	if (mpGetAddressCallback)
		return (BYTE*)mpGetAddressCallback(handle, data, captureIndex);
	else {
		printf("ERROR: getAddress callback : NULL.\n");
		return NULL;
	}
}