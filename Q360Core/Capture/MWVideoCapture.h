#ifndef __MWVIDEO_CAPTURE_QURAM_H__
#define __MWVIDEO_CAPTURE_QURAM_H__

#include "VideoCapture.h"
#include "MWCommon.h"

class MWVideoCapture : public VideoCapture {
public:
	MWVideoCapture();

	static MWVideoCapture* create_VideoCapture(int iCam, int iDevice, int iChannel, int iID, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, bool isCamOverlay = false, cv::Rect cropRect = cv::Rect(0, 0, 0 ,0));
	static BOOL destroy_VideoCapture(MWVideoCapture* instance);

	BOOL init_VideoCapture(int iCam, int iDevice, int iChannel, int iID, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, cv::Rect cropRect);

	void setCaptureWidth(int captureWidth);
	void setCaptureHeight(int captureHeight);
	void setCapturFPS(float capturFPS);
	void setColorSpace(DWORD colorSpace);
	void setStride(DWORD stride);
	void setFrameSize(DWORD frameSize);
	void setHChannel(HCHANNEL hChannel);
	void setAspectX(int aspectX);
	void setAspectY(int aspectY);

	int getCaptureWidth();
	int getCaptureHeight();
	int getCameraIndex();
	int getChannelIndex();
	float getCapturFPS();
	DWORD getColorSpace();
	DWORD getStride();
	DWORD getFrameSize();
	HCHANNEL getHChannel();
	char* getChannelID();
	int getAspectX();
	int getAspectY();

	unsigned char* buffer;

	void setCallback_getAddress(callback_getAddress cb, void* data);
	void setCallback_onCaptureDone(callback_onCaptureDone cb, void* data);
	void setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data);

	void SnapShot(LONGLONG delay = 0);
	int getStatus();
	void requestChangeCameraIndex(int index, bool enable);
	void requestCopyInputToCPU();
	bool isDoneCopyInputToCPU();
	void setMultiStreaming(int isMultiStreaming);

private:
	bool isEnable();
	bool isClear();
	bool isSnapShot();
	LONGLONG getSnapShotTime();
	BYTE* getSnapShotBuffer();
	static DWORD WINAPI run(LPVOID handle);
	BYTE* getAddress(void* handle, void* data, int captureIndex);

	cv::Rect mCropRect;

private:
	int mCameraIndex;
	int mChannelIndex;
	bool mIsEnable;
	int mCaptureWidth;
	int mCaptureHeight;
	float mCapturFPS;
	DWORD mColorSpace;
	DWORD mStride;
	DWORD mFrameSize;
	HCHANNEL mhChannel;
	bool mIsClear;
	HANDLE mhCaptureThread;
	long long mSnapShotTime;
	bool mIsSnapShot;
	BYTE* mSnapShotBuffer;
	char mChannelID[MAX_PATH];
	int mNewCameraIndex;
	int mNewEnable;
	bool mIsRequestChangeCameraIndex;
	bool mIsRequestCopyInputToCPU;
	bool mIsRequestChangeFPS;
	bool mIsMultiStreaming;
	int mID;
	double mTime;

	bool isCam = false;

	int mAspectX;
	int mAspectY;

	callback_getAddress mpGetAddressCallback;
	void* mpGetAddressCallbackData;
	callback_onCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
	callback_onSnapShotDone mpSnapShotDoneCallback;
	void* mpSnapShotDoneCallbackData;
};
#endif // __MWVIDEO_CAPTURE_QURAM_H__