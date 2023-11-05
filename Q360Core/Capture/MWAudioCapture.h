#ifndef __MWAUDIO_CAPTURE_QURAM_H__
#define __MWAUDIO_CAPTURE_QURAM_H__

#include "AudioCapture.h"
#include "MWCommon.h"

class MWAudioCapture : public AudioCapture {
public:
	MWAudioCapture();

	static MWAudioCapture* create_AudioCapture(int iCam);
	static BOOL destroy_AudioCapture(MWAudioCapture* instance);

	BOOL init_AudioCapture(int iCam);

	void setHChannel(HCHANNEL hChannel);
	void setHeaderBuffer(unsigned char* headerBuffer);
	void setHeaderBufferSize(int headerBufferSize);

	void setwFormatTag(WORD wFormatTag);
	void setnChannels(WORD nChannels);
	void setnSamplesPerSec(DWORD nSamplesPerSec);
	void setnAvgBytesPerSec(DWORD nAvgBytesPerSec);
	void setnBlockAlign(WORD nBlockAlign);
	void setwBitsPerSample(WORD wBitsPerSample);

	int getCameraIndex();
	HCHANNEL getHChannel();
	unsigned char* getHeaderBuffer();
	int getHeaderBufferSize();

	WORD getwFormatTag();
	WORD getnChannels();
	DWORD getnSamplesPerSec();
	DWORD getnAvgBytesPerSec();
	WORD getnBlockAlign();
	WORD getwBitsPerSample();

	void setCallback_onCaptureDone(callback_onAudioCaptureDone cb, void* data);

private:
	bool isClear();
	static DWORD WINAPI run(LPVOID handle);

private:
	int mCameraIndex;
	HCHANNEL mhChannel;
	bool mIsClear;
	HANDLE mhCaptureThread;
	unsigned char* mHeaderBuffer;
	int mHeaderBufferSize;
	double mTime;

	WORD mwFormatTag;
	WORD mnChannels;
	DWORD mnSamplesPerSec;
	DWORD mnAvgBytesPerSec;
	WORD mnBlockAlign;
	WORD mwBitsPerSample;

	callback_onAudioCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
};

#endif // __MWAUDIO_CAPTURE_QURAM_H__