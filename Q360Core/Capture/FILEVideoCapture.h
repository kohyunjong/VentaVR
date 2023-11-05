#ifndef __FILEVIDEO_CAPTURE_QURAM_H__
#define __FILEVIDEO_CAPTURE_QURAM_H__

#include "VideoCapture.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

class FILEVideoCapture : public VideoCapture {
public:
	FILEVideoCapture();

	static FILEVideoCapture* create_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, char* videoPath);
	static BOOL destroy_VideoCapture(FILEVideoCapture* instance);

	BOOL init_VideoCapture(int iCam, int iChannel, int captureWidth, int captureHeight, float capturFPS, char* videoPath);

	void setCaptureWidth(int captureWidth);
	void setCaptureHeight(int captureHeight);
	void setCapturFPS(float capturFPS);
	void setStride(DWORD stride);
	void setFrameSize(DWORD frameSize);

	int getCaptureWidth();
	int getCaptureHeight();
	int getCameraIndex();
	int getChannelIndex();
	float getCapturFPS();
	DWORD getStride();
	DWORD getFrameSize();
	char* getChannelID();

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
	static DWORD WINAPI run(LPVOID handle);
	BYTE* getAddress(void* handle, void* data, int captureIndex);

private:
	int mCameraIndex;
	int mChannelIndex;
	int mCaptureWidth;
	int mCaptureHeight;
	float mCapturFPS;
	DWORD mStride;
	DWORD mFrameSize;
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

	callback_getAddress mpGetAddressCallback;
	void* mpGetAddressCallbackData;
	callback_onCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
	callback_onSnapShotDone mpSnapShotDoneCallback;
	void* mpSnapShotDoneCallbackData;
};
#endif // __FILEVIDEO_CAPTURE_QURAM_H__