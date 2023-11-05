#ifndef __XIAUDIO_CAPTURE_QURAM_H__
#define __XIAUDIO_CAPTURE_QURAM_H__

#include "AudioCapture.h"
#include "XICommon.h"

#define AUDIO_SAMPLESPERSEC 48000 // 48000	//44100

class XIAudioCapture : public AudioCapture {
public:
	XIAudioCapture();

	static XIAudioCapture* create_AudioCapture(int iCam);
	static BOOL destroy_AudioCapture(XIAudioCapture* instance);

	BOOL init_AudioCapture(int iCam);

	void setHChannel(HANDLE hChannel);
	void setHeaderBuffer(unsigned char* headerBuffer);
	void setHeaderBufferSize(int headerBufferSize);

	void setwFormatTag(WORD wFormatTag);
	void setnChannels(WORD nChannels);
	void setnSamplesPerSec(DWORD nSamplesPerSec);
	void setnAvgBytesPerSec(DWORD nAvgBytesPerSec);
	void setnBlockAlign(WORD nBlockAlign);
	void setwBitsPerSample(WORD wBitsPerSample);

	int getCameraIndex();
	HANDLE getHChannel();
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
	static void onCapture(const BYTE * pbFrame, int cbFrame, void * pvParam, UINT64 u64TimeStamp);

private:
	int mCameraIndex;
	HANDLE mhChannel;
	bool mIsClear;
	HANDLE mhCaptureThread;
	unsigned char* mHeaderBuffer;
	int mHeaderBufferSize;
	LONGLONG mSynchronizeTime;

	CWaveFile file;
	WORD mwFormatTag;
	WORD mnChannels;
	DWORD mnSamplesPerSec;
	DWORD mnAvgBytesPerSec;
	WORD mnBlockAlign;
	WORD mwBitsPerSample;

	callback_onAudioCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
};

#endif // __XIAUDIO_CAPTURE_QURAM_H__