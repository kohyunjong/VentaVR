#ifndef __AUDIO_CAPTURE_QURAM_H__
#define __AUDIO_CAPTURE_QURAM_H__

#include "Capture.h"

typedef void(*callback_onAudioCaptureDone)(void *handle, void *data, unsigned char* buffer, int size, long long timeStamp);
typedef void(*callback_onInputError)(void *handle, void *data, int msg, long long timeStamp);

class AudioCapture : public Capture {
public:
	AudioCapture();

	virtual unsigned char* getHeaderBuffer() = 0;
	virtual int getHeaderBufferSize() = 0;

	virtual WORD getwFormatTag() = 0;
	virtual WORD getnChannels() = 0;
	virtual DWORD getnSamplesPerSec() = 0;
	virtual DWORD getnAvgBytesPerSec() = 0;
	virtual WORD getnBlockAlign() = 0;
	virtual WORD getwBitsPerSample() = 0;

	virtual void setCallback_onCaptureDone(callback_onAudioCaptureDone cb, void* data) = 0;

	int getCaptureType();
	void setCaptureType(int captureType);
	void setCallback_onInputError(callback_onInputError cb, void* data);

private:
	int mCaptureType;

protected:
	callback_onInputError mpInputErrorCallback;
	void* mpInputErrorCallbackData;
};

#endif // __AUDIO_CAPTURE_QURAM_H__