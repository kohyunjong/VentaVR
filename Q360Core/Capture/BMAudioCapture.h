#ifndef __BMAUDIO_CAPTURE_QURAM_H__
#define __BMAUDIO_CAPTURE_QURAM_H__

#include "AudioCapture.h"
#include "platform.h"

#include "BMVideoCapture.h"

#define AUDIO_SAMPLESPERSEC 48000 // 48000	//44100

class BMAudioCapture : public AudioCapture {
public:
	BMAudioCapture();

	static BMAudioCapture* create_AudioCapture(BMVideoCapture* videoCapture);
	static BOOL destroy_AudioCapture(BMAudioCapture* instance);

	BOOL init_AudioCapture(BMVideoCapture* videoCapture);

	void setHeaderBuffer(unsigned char* headerBuffer);
	void setHeaderBufferSize(int headerBufferSize);

	void setwFormatTag(WORD wFormatTag);
	void setnChannels(WORD nChannels);
	void setnSamplesPerSec(DWORD nSamplesPerSec);
	void setnAvgBytesPerSec(DWORD nAvgBytesPerSec);
	void setnBlockAlign(WORD nBlockAlign);
	void setwBitsPerSample(WORD wBitsPerSample);

	int getCameraIndex();
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
	static void onCapture(void* handle, IDeckLinkAudioInputPacket* audioPacket);
	static void onChange(void* handle, IDeckLinkDisplayMode *newDisplayMode, BMDPixelFormat pixelFormat);

private:
	int mCameraIndex;
	unsigned char* mHeaderBuffer;
	int mHeaderBufferSize;

	CWaveFile file;
	WORD mwFormatTag;
	WORD mnChannels;
	DWORD mnSamplesPerSec;
	DWORD mnAvgBytesPerSec;
	WORD mnBlockAlign;
	WORD mwBitsPerSample;

	//IDeckLinkIterator*			deckLinkIterator = NULL;
	//IDeckLink*					deckLink = NULL;
	IDeckLinkInput*             deckLinkInput = NULL;
	NotificationCallback*  notificationCallback = NULL;

	callback_onAudioCaptureDone mpCaptureDoneCallback;
	void* mpCaptureDoneCallbackData;
};

#endif // __BMAUDIO_CAPTURE_QURAM_H__