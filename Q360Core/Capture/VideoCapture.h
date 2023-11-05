#ifndef __VIDEO_CAPTURE_QURAM_H__
#define __VIDEO_CAPTURE_QURAM_H__

#include "Capture.h"

typedef void*(*callback_getAddress)(void* handle, void* data, int captureIndex);
typedef void(*callback_onCaptureDone)(void* handle, void* data, long long timeStamp, int captureIndex);
typedef void(*callback_onSnapShotDone)(void* handle, void* data, BYTE* snapShotBuffer, int bufferSize, long long timeStamp);

class VideoCapture : public Capture {
public:
	VideoCapture();

	virtual int getCameraIndex() = 0;
	virtual int getChannelIndex() = 0;
	virtual char* getChannelID() = 0;

	virtual void setCallback_getAddress(callback_getAddress cb, void* data) = 0;
	virtual void setCallback_onCaptureDone(callback_onCaptureDone cb, void* data) = 0;
	virtual void setCallback_onSnapShotDone(callback_onSnapShotDone cb, void* data) = 0;

	virtual void SnapShot(LONGLONG delay = 0) = 0;
	virtual void requestChangeCameraIndex(int index, bool enable) = 0;
	virtual void requestCopyInputToCPU() = 0;
	virtual bool isDoneCopyInputToCPU() = 0;

	int getCaptureType();
	void setCaptureType(int captureType);

private:
	int mCaptureType;
};
#endif // __VIDEO_CAPTURE_QURAM_H__