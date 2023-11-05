#ifndef __BMVIDEO_CAPTURE_QURAM_H__
#define __BMVIDEO_CAPTURE_QURAM_H__

#include "VideoCapture.h"
#include "platform.h"

class BMVideoCapture : public VideoCapture {
public:
	BMVideoCapture();

	static BMVideoCapture* create_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect = cv::Rect(0, 0, 0, 0));
	static BMVideoCapture* create_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect = cv::Rect(0, 0, 0, 0));
	static BOOL destroy_VideoCapture(BMVideoCapture* instance);

	static BOOL getDeviceList(int n, char* displayName, char* modelName);

	BOOL init_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect);
	BOOL init_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, cv::Rect cropRect);

	int getCameraIndex();
	int getChannelIndex();
	char* getChannelID();
	IDeckLinkInput* getIDeckLinkInput();
	NotificationCallback* getNotificationCallback();

	void setCallback_getAddress(callback_getAddress cb, void* data);
	void setCallback_onCaptureDone(callback_onCaptureDone cb, void* data);
	void setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data);

	void SnapShot(LONGLONG delay = 0);
	void requestChangeCameraIndex(int index, bool enable);
	void requestCopyInputToCPU();
	bool isDoneCopyInputToCPU();

private:
	bool isClear();
	bool isSnapShot();
	LONGLONG getSnapShotTime();
	BYTE* getSnapShotBuffer();
	static void onCapture(void* handle, IDeckLinkVideoInputFrame* videoFrame);
	static void onChange(void* handle, IDeckLinkDisplayMode *newDisplayMode, BMDPixelFormat pixelFormat);
	BYTE* getAddress(void* handle, void* data, int captureIndex);

private:
	int mCameraIndex;
	int mChannelIndex;
	bool mIsEnable;
	int mCaptureWidth;
	int mCaptureHeight;
	float mCapturFPS;
	DWORD mFrameSize;
	bool mIsClear;
	long long mLastFrameTime;
	long long mSnapShotTime;
	bool mIsSnapShot;
	BYTE* mSnapShotBuffer;
	char mChannelID[MAX_PATH];
	int mNewCameraIndex;
	int mNewEnable;
	bool mIsRequestChangeCameraIndex;
	bool mIsRequestCopyInputToCPU;

	cv::Mat mResizeRGBInput;

	cv::Rect mCropRect;

	IDeckLinkIterator*			deckLinkIterator = NULL;
	IDeckLinkVideoConversion*	deckLinkVideoConversion = NULL;
	IDeckLinkAttributes*        deckLinkAttributes = NULL;
	IDeckLink*					deckLink = NULL;
	IDeckLinkInput*             deckLinkInput = NULL;
	IDeckLinkOutput*            deckLinkOutput = NULL;
	NotificationCallback*       notificationCallback = NULL;

	IDeckLinkDisplayMode* mDisplayMode;
	BMDPixelFormat mPixelFormat;

	callback_getAddress mpGetAddressCallback;
	void* mpGetAddressCallbackData;
	callback_onCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
	callback_onSnapShotDone mpSnapShotDoneCallback;
	void* mpSnapShotDoneCallbackData;
};
#endif // __BMVIDEO_CAPTURE_QURAM_H__