#ifndef __XIVIDEO_CAPTURE_QURAM_H__
#define __XIVIDEO_CAPTURE_QURAM_H__

#include "VideoCapture.h"
#include "XICommon.h"

class XIVideoCapture : public VideoCapture {
public:
	XIVideoCapture();

	static XIVideoCapture* create_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, cv::Rect cropRect = cv::Rect(0,0,0,0));
	static BOOL destroy_VideoCapture(XIVideoCapture* instance);

	BOOL init_VideoCapture(int iCam, int iDevice, int iChannel, int captureWidth, int captureHeight, float capturFPS, DWORD colorSpace, cv::Rect cropRect);

	void setCaptureWidth(int captureWidth);
	void setCaptureHeight(int captureHeight);
	void setCapturFPS(float capturFPS);
	void setColorSpace(DWORD colorSpace);
	void setStride(DWORD stride);
	void setFrameSize(DWORD frameSize);
	void setHChannel(HANDLE hChannel);

	int getCaptureWidth();
	int getCaptureHeight();
	int getCameraIndex();
	int getChannelIndex();
	float getCapturFPS();
	DWORD getColorSpace();
	DWORD getStride();
	DWORD getFrameSize();
	HANDLE getHChannel();
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
	static void onCapture(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp);
	BYTE* getAddress(void* handle, void* data, int captureIndex);

private:
	int mCameraIndex;
	int mChannelIndex;
	int mCaptureWidth;
	int mCaptureHeight;
	float mCapturFPS;
	DWORD mColorSpace;
	DWORD mStride;
	DWORD mFrameSize;
	HANDLE mhChannel;
	bool mIsClear;
	UINT64 mLastFrameTime;
	long long mSnapShotTime;
	bool mIsSnapShot;
	BYTE* mSnapShotBuffer;
	char mChannelID[MAX_PATH];
	int mNewCameraIndex;
	int mNewEnable;
	bool mIsRequestChangeCameraIndex;
	bool mIsRequestCopyInputToCPU;

	cv::Mat mRGBInput;
	cv::Mat mBGRInput;
	cv::Mat mYUVInput;

	cv::Rect mCropRect;

#ifdef IMAGE_TO_BUFFER
	cv::Mat mImg;
#endif

	callback_getAddress mpGetAddressCallback;
	void* mpGetAddressCallbackData;
	callback_onCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
	callback_onSnapShotDone mpSnapShotDoneCallback;
	void* mpSnapShotDoneCallbackData;
};
#endif // __XIVIDEO_CAPTURE_QURAM_H__