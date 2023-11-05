#include "BMVideoCapture.h"

// public functions //

BMVideoCapture::BMVideoCapture() {

}

BMVideoCapture* BMVideoCapture::create_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect) {
	BMVideoCapture* instance = new BMVideoCapture();

	instance->init_VideoCapture(iCam, iChannel, captureWidth, captureHeight, capturFPS, cropRect);

	return instance;
}

BMVideoCapture* BMVideoCapture::create_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect) {
	BMVideoCapture* instance = new BMVideoCapture();

	instance->init_VideoCapture(iCam, iDevice, iChannel, captureWidth, captureHeight, capturFPS, cropRect);

	return instance;
}


BOOL BMVideoCapture::init_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect) {
	mIsClear = false;
	mIsSnapShot = false;
	mIsRequestCopyInputToCPU = 0;
	mResizeRGBInput = cv::Mat(cv::Size(captureWidth, captureHeight), CV_8UC3);
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

	mCropRect = cropRect;

	if (mCropRect.width > 0 && mCropRect.height > 0)
	{
		mFrameSize = mCropRect.width*mCropRect.height * 3;
	}
	else
	{
		mFrameSize = mCaptureWidth * mCaptureHeight * 3;
	}

	do {
		HRESULT result;
		BOOL supported;

		Initialize();

		// Create an IDeckLinkIterator object to enumerate all DeckLink cards in the system
		if (GetDeckLinkIterator(&deckLinkIterator) != S_OK)
		{
			fprintf(stderr, "A DeckLink iterator could not be created.  The DeckLink drivers may not be installed.\n");
			break;
		}

		// Create an IDeckLinkVideoConversion object
		result = CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&deckLinkVideoConversion);
		if (result != S_OK)
		{
			fprintf(stderr, "A IDeckLinkVideoConversion could not be created.\n");
			break;
		}

		// Obtain the first DeckLink device
		for (int i = 0; i <= mCameraIndex; i++) {
			result = deckLinkIterator->Next(&deckLink);
			if (result != S_OK)
			{
				//fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
				break;
			}
		}
		if (result != S_OK)
		{
			fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
			break;
		}

		BSTR displayNameW;
		char temp[MAX_PATH];
		deckLink->GetDisplayName(&displayNameW);
		int strSize = WideCharToMultiByte(CP_ACP, 0, displayNameW, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, displayNameW, -1, temp, strSize, 0, 0);

		sprintf(mChannelID, "%s (%d)", temp, mCameraIndex);

		// Obtain the Attributes interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkAttributes, (void**)&deckLinkAttributes);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkAttributes interface - result = %08x\n", result);
			break;
		}

		// Determine whether the DeckLink device supports input format detection
		result = deckLinkAttributes->GetFlag(BMDDeckLinkSupportsInputFormatDetection, &supported);
		if ((result != S_OK) || (supported == false))
		{
			fprintf(stderr, "Device does not support automatic mode detection\n");
			break;
		}

		// Obtain the input interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkInput interface - result = %08x\n", result);
			break;
		}

		// Obtain the output interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&deckLinkOutput);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkOutput interface - result = %08x\n", result);
			break;
		}

		// Create an instance of notification callback
		notificationCallback = new NotificationCallback(deckLinkInput, onCapture, this, onChange, this);
		if (notificationCallback == NULL)
		{
			fprintf(stderr, "Could not create notification callback object\n");
			break;
		}

		// Set the callback object to the DeckLink device's input interface
		result = deckLinkInput->SetCallback(notificationCallback);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not set callback - result = %08x\n", result);
			break;
		}

		// Enable video input with a default video mode and the automatic format detection feature enabled
		//result = deckLinkInput->EnableVideoInput(bmdModeNTSC, bmdFormat10BitYUV, bmdVideoInputEnableFormatDetection);
		result = deckLinkInput->EnableVideoInput(bmdModeHD1080p2997, bmdFormat8BitYUV, bmdVideoInputEnableFormatDetection);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not enable video input - result = %08x\n", result);
			break;
		}

		mPixelFormat = bmdFormat8BitYUV;

		// Start capture
		result = deckLinkInput->StartStreams();
		if (result != S_OK)
		{
			fprintf(stderr, "Could not start capture - result = %08x\n", result);
			break;
		}

		printf("Initialize BMCapture SDK\n");
		return S_OK;
	} while (false);

	// Release the attributes interface
	if (deckLinkAttributes != NULL)
		deckLinkAttributes->Release();

	// Release the video input interface
	if (deckLinkInput != NULL)
		deckLinkInput->Release();

	// Release the Decklink object
	if (deckLink != NULL)
		deckLink->Release();

	// Release the DeckLink iterator
	if (deckLinkIterator != NULL)
		deckLinkIterator->Release();

	// Release the notification callback object
	if (notificationCallback)
		delete notificationCallback;

	return S_FALSE;
}

BOOL BMVideoCapture::init_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect) {
	mIsClear = false;
	mIsSnapShot = false;
	mIsRequestCopyInputToCPU = 0;
	mResizeRGBInput = cv::Mat(cv::Size(captureWidth, captureHeight), CV_8UC3);
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

	mFrameSize = captureWidth * captureHeight * 3;

	do {
		HRESULT result;
		BOOL supported;

		Initialize();

		// Create an IDeckLinkIterator object to enumerate all DeckLink cards in the system
		if (GetDeckLinkIterator(&deckLinkIterator) != S_OK)
		{
			fprintf(stderr, "A DeckLink iterator could not be created.  The DeckLink drivers may not be installed.\n");
			break;
		}

		// Create an IDeckLinkVideoConversion object
		result = CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&deckLinkVideoConversion);
		if (result != S_OK)
		{
			fprintf(stderr, "A IDeckLinkVideoConversion could not be created.\n");
			break;
		}

		// Obtain the first DeckLink device
		for (int i = 0; i <= iDevice; i++) {
			result = deckLinkIterator->Next(&deckLink);
			if (result != S_OK)
			{
				//fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
				break;
			}
		}
		if (result != S_OK)
		{
			fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
			break;
		}

		BSTR displayNameW;
		char temp[MAX_PATH];
		deckLink->GetDisplayName(&displayNameW);
		int strSize = WideCharToMultiByte(CP_ACP, 0, displayNameW, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, displayNameW, -1, temp, strSize, 0, 0);

		sprintf(mChannelID, "%s (%d)", temp, iDevice);

		// Obtain the Attributes interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkAttributes, (void**)&deckLinkAttributes);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkAttributes interface - result = %08x\n", result);
			break;
		}

		// Determine whether the DeckLink device supports input format detection
		result = deckLinkAttributes->GetFlag(BMDDeckLinkSupportsInputFormatDetection, &supported);
		if ((result != S_OK) || (supported == false))
		{
			fprintf(stderr, "Device does not support automatic mode detection\n");
			break;
		}

		// Obtain the input interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkInput interface - result = %08x\n", result);
			break;
		}

		// Obtain the output interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&deckLinkOutput);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkOutput interface - result = %08x\n", result);
			break;
		}

		// Create an instance of notification callback
		notificationCallback = new NotificationCallback(deckLinkInput, onCapture, this, onChange, this);
		if (notificationCallback == NULL)
		{
			fprintf(stderr, "Could not create notification callback object\n");
			break;
		}

		// Set the callback object to the DeckLink device's input interface
		result = deckLinkInput->SetCallback(notificationCallback);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not set callback - result = %08x\n", result);
			break;
		}

		// Enable video input with a default video mode and the automatic format detection feature enabled
		//result = deckLinkInput->EnableVideoInput(bmdModeNTSC, bmdFormat10BitYUV, bmdVideoInputEnableFormatDetection);
		result = deckLinkInput->EnableVideoInput(bmdModeHD1080p2997, bmdFormat8BitYUV, bmdVideoInputEnableFormatDetection);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not enable video input - result = %08x\n", result);
			break;
		}

		mPixelFormat = bmdFormat8BitYUV;

		// Start capture
		result = deckLinkInput->StartStreams();
		if (result != S_OK)
		{
			fprintf(stderr, "Could not start capture - result = %08x\n", result);
			break;
		}

		printf("Initialize BMCapture SDK\n");
		return S_OK;
	} while (false);

	// Release the attributes interface
	if (deckLinkAttributes != NULL)
		deckLinkAttributes->Release();

	// Release the video input interface
	if (deckLinkInput != NULL)
		deckLinkInput->Release();

	// Release the Decklink object
	if (deckLink != NULL)
		deckLink->Release();

	// Release the DeckLink iterator
	if (deckLinkIterator != NULL)
		deckLinkIterator->Release();

	// Release the notification callback object
	if (notificationCallback)
		delete notificationCallback;

	return S_FALSE;
}

BOOL BMVideoCapture::destroy_VideoCapture(BMVideoCapture* instance) {
	if (instance) {
		instance->mIsClear = true;

		// Stop synchronized video and audio capture
		if (instance->deckLinkInput != NULL)
			instance->deckLinkInput->StopStreams();

		// Release the attributes interface
		if (instance->deckLinkAttributes != NULL)
			instance->deckLinkAttributes->Release();

		// Release the video input interface
		if (instance->deckLinkInput != NULL)
			instance->deckLinkInput->Release();

		// Release the Decklink object
		if (instance->deckLink != NULL)
			instance->deckLink->Release();

		// Release the DeckLink iterator
		if (instance->deckLinkIterator != NULL)
			instance->deckLinkIterator->Release();

		// Release the notification callback object
		if (instance->notificationCallback)
			delete instance->notificationCallback;

		delete instance;
		printf("Destroy BMVideoCapture\n");
		return S_OK;
	}

	printf("ERROR: BMVideoCapture instance : NULL!\n");
	return S_FALSE;
}

int BMVideoCapture::getCameraIndex() {
	return mCameraIndex;
}

int BMVideoCapture::getChannelIndex() {
	return mChannelIndex;
}

char* BMVideoCapture::getChannelID() {
	return mChannelID;
}

IDeckLinkInput* BMVideoCapture::getIDeckLinkInput() {
	return deckLinkInput;
}

NotificationCallback* BMVideoCapture::getNotificationCallback() {
	return notificationCallback;
}

void BMVideoCapture::setCallback_getAddress(callback_getAddress cb, void* data) {
	mpGetAddressCallback = cb;
	mpGetAddressCallbackData = data;
}

void BMVideoCapture::setCallback_onCaptureDone(callback_onCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

void BMVideoCapture::setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data) {
	mpSnapShotDoneCallback = cb;
	mpSnapShotDoneCallbackData = data;
}

void BMVideoCapture::SnapShot(LONGLONG delay) {
	mSnapShotBuffer = new BYTE[mFrameSize];

	mSnapShotTime = mLastFrameTime + delay;
	mIsSnapShot = true;
}

void BMVideoCapture::requestChangeCameraIndex(int index, bool enable) {
	mNewCameraIndex = index;
	mNewEnable = enable;
	mIsRequestChangeCameraIndex = 1;
}

void BMVideoCapture::requestCopyInputToCPU() {
	mIsRequestCopyInputToCPU = 1;
}

bool BMVideoCapture::isDoneCopyInputToCPU() {
	return mIsRequestCopyInputToCPU;
}

// private functions //

bool BMVideoCapture::isClear() {
	return mIsClear;
}

bool BMVideoCapture::isSnapShot() {
	return mIsSnapShot;
}

LONGLONG BMVideoCapture::getSnapShotTime() {
	return mSnapShotTime;
}

BYTE* BMVideoCapture::getSnapShotBuffer() {
	return mSnapShotBuffer;
}

void BMVideoCapture::onCapture(void* handle, IDeckLinkVideoInputFrame* videoFrame) {
	BMVideoCapture *info = (BMVideoCapture *)handle;
	if (info == NULL)
		return;

	if (info->isClear()) {
		printf("BMVideoCapture Clear.\n");
		return;
	}

	HRESULT result;
	BMDTimeValue frameTime;
	BMDTimeValue frameDuration;
	videoFrame->GetStreamTime(&frameTime, &frameDuration, 1000);
	//printf("frameTime : %lld,frameDuration : %lld\n", frameTime, frameDuration);

	info->mLastFrameTime = frameTime;

	long w = videoFrame->GetWidth();
	long h = videoFrame->GetHeight();
	long rb = videoFrame->GetRowBytes();
	BMDPixelFormat format = videoFrame->GetPixelFormat();
	//printf("w : %ld, h : %ld, rb : %ld, format : %x\n", w, h, rb, format);

	cv::Mat YUVInput = cv::Mat(cv::Size(w, h), CV_8UC2);
	cv::Mat BGRAInput = cv::Mat(cv::Size(w, h), CV_8UC4);
	cv::Mat RGBInput = cv::Mat(cv::Size(w, h), CV_8UC3);
	cv::Mat resizeRGBInput;

#if 0 // Use deckLinkVideoConversion method
	IDeckLinkMutableVideoFrame* frame = NULL;
	result = info->deckLinkOutput->CreateVideoFrame(w, h, w * 4, bmdFormat8BitBGRA, bmdFrameFlagDefault, &frame);
	result = info->deckLinkVideoConversion->ConvertFrame(videoFrame, frame);

	void* rowBytes;
	result = frame->GetBytes(&rowBytes);
	info->mYUVInput.data = (uchar*)rowBytes;

	if (info->mIsRequestChangeCameraIndex == 1) {
		info->mCameraIndex = info->mNewCameraIndex;
		info->mIsEnable = info->mNewEnable;
		info->mIsRequestChangeCameraIndex = 0;
	}

	void* ptr0 = info->getAddress(info, info->mpGetAddressCallbackData, 0);
	if (ptr0 == NULL)
		return;

	info->mRGBInput.data = (uchar*)ptr0;

	cv::cvtColor(info->mYUVInput, info->mRGBInput, CV_BGRA2RGB);
#else
	void* rowBytes;
	result = videoFrame->GetBytes(&rowBytes);

	if (info->mPixelFormat == bmdFormat8BitYUV) {
		YUVInput.data = (uchar*)rowBytes;
		cv::cvtColor(YUVInput, RGBInput, CV_YUV2RGB_Y422);
	}
	else if (info->mPixelFormat == bmdFormat8BitBGRA) {
		BGRAInput.data = (uchar*)rowBytes;
		cv::cvtColor(BGRAInput, RGBInput, CV_BGRA2RGB);
	}

	float ratio = info->mCaptureHeight / (float)h;
	int resizeWidth = w * ratio;
	if (info->mCaptureHeight != h) {
		cv::resize(RGBInput, resizeRGBInput, cv::Size(resizeWidth, info->mCaptureHeight), 0, 0, CV_INTER_NN);
		RGBInput = resizeRGBInput;
	}
	if (info->mCaptureWidth != resizeWidth) {
		if (info->mCaptureWidth > resizeWidth) {
			// margin
			cv::Rect marginRect = cv::Rect((info->mCaptureWidth - resizeWidth) / 2, 0, resizeWidth, info->mCaptureHeight);
			RGBInput.copyTo(info->mResizeRGBInput(marginRect));
		}
		else {
			// crop
			cv::Rect cropRect = cv::Rect((resizeWidth - info->mCaptureWidth) / 2, 0, info->mCaptureWidth, info->mCaptureHeight);
			info->mResizeRGBInput = RGBInput(cropRect).clone();
		}
	}
	else {
		info->mResizeRGBInput = RGBInput;
	}

	if (info->mIsRequestChangeCameraIndex == 1) {
		info->mCameraIndex = info->mNewCameraIndex;
		info->mIsEnable = info->mNewEnable;
		info->mIsRequestChangeCameraIndex = 0;
	}

	void* ptr0 = info->getAddress(info, info->mpGetAddressCallbackData, info->mChannelIndex);
	if (ptr0 == NULL)
		return;
	if (info->mCropRect.width > 0 && info->mCropRect.height > 0)
	{
		memcpy(ptr0, info->mResizeRGBInput.data + ((info->mCropRect.y*info->mCropRect.width + info->mCropRect.x) * 3), info->mCropRect.width*info->mCropRect.height * 3);
	}
	else
	{
		memcpy(ptr0, info->mResizeRGBInput.data, info->mCaptureWidth * info->mCaptureHeight * 3);
	}
#endif
	//void* ptr1 = info->getAddress(info, info->mpGetAddressCallbackData, 1);
	//if (ptr1) {
		//memcpy(ptr1, info->mRGBInput.data, info->mCaptureWidth * info->mCaptureHeight * 3);
	//}

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
		if (frameTime > info->getSnapShotTime() && snapShotBuffer != NULL) {
			memcpy(snapShotBuffer, ptr0, info->mFrameSize);
			info->mIsSnapShot = false;
			info->mpSnapShotDoneCallback(info, info->mpSnapShotDoneCallbackData, snapShotBuffer, info->mFrameSize, frameTime);
		}
	}

	if (info->mpCaptureDoneCallback)
		info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, frameTime, 0);
	else
		printf("ERROR: onCaptureDone callback : NULL.\n");

	//if (info->mpCaptureDoneCallback)
		//info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, frameTime, 1);
	//else
		//printf("ERROR: onCaptureDone callback : NULL.\n");
}

void BMVideoCapture::onChange(void* handle, IDeckLinkDisplayMode *newDisplayMode, BMDPixelFormat pixelFormat) {
	BMVideoCapture *info = (BMVideoCapture *)handle;

	info->mDisplayMode = newDisplayMode;
	info->mPixelFormat = pixelFormat;
}

BYTE* BMVideoCapture::getAddress(void* handle, void* data, int captureIndex) {
	if (mpGetAddressCallback)
		return (BYTE*)mpGetAddressCallback(handle, data, captureIndex);
	else {
		printf("ERROR: getAddress callback : NULL.\n");
		return NULL;
	}
}

BOOL BMVideoCapture::getDeviceList(int n, char* displayName, char* modelName) {
	IDeckLinkIterator*      deckLinkIterator = NULL;
	IDeckLink*              deckLink = NULL;
	IDeckLinkInput*         deckLinkInput = NULL;
	HRESULT result;

	do {
		Initialize();

		printf("getDeviceList n : %d\n", n);

		// Create an IDeckLinkIterator object to enumerate all DeckLink cards in the system
		result = GetDeckLinkIterator(&deckLinkIterator);
		if (result != S_OK)
		{
			fprintf(stderr, "A DeckLink iterator could not be created.  The DeckLink drivers may not be installed.\n");
			break;
		}

		// Obtain the first DeckLink device
		for (int i = 0; i < n; i++) {
			if (deckLink != NULL) {
				deckLink->Release();
				deckLink = NULL;
			}

			result = deckLinkIterator->Next(&deckLink);
			if (result != S_OK)
			{
				//fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
				break;
			}
		}
		if (result != S_OK)
		{
			fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
			break;
		}

		BSTR displayNameW;
		BSTR modelNameW;
		deckLink->GetDisplayName(&displayNameW);
		deckLink->GetModelName(&modelNameW);

		int strSize = WideCharToMultiByte(CP_ACP, 0, displayNameW, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, displayNameW, -1, displayName, strSize, 0, 0);

		strSize = WideCharToMultiByte(CP_ACP, 0, modelNameW, -1, NULL, 0, NULL, NULL);
		WideCharToMultiByte(CP_ACP, 0, modelNameW, -1, modelName, strSize, 0, 0);

		// Obtain the input interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&deckLinkInput);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkInput interface - result = %08x\n", result);
			break;
		}

		// Enable video input with a default video mode and the automatic format detection feature enabled
		result = deckLinkInput->EnableVideoInput(bmdModeHD1080p2997, bmdFormat8BitYUV, bmdVideoInputEnableFormatDetection);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not enable video input - result = %08x\n", result);
			break;
		}

		deckLinkInput->DisableVideoInput();
	} while (false);

	// Release the video input interface
	if (deckLinkInput != NULL)
		deckLinkInput->Release();

	// Release the Decklink object
	if (deckLink != NULL)
		deckLink->Release();

	// Release the DeckLink iterator
	if (deckLinkIterator != NULL)
		deckLinkIterator->Release();

	return result == S_OK ? true : false;
}