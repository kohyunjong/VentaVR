#ifndef __BMOUTPUT_QURAM_H__
#define __BMOUTPUT_QURAM_H__

#include "platform.h"
#include "time.h"
#include "clock.h"

//#define SDI_OUTPUT_TEST
#define NUM_OUTPUT_BUFFERS 4

typedef void*(*callback_getOutputAddress)(void* handle, void* data);

class BMOutput {
public:
	BMOutput();

	static BMOutput* create_BMOutput(int width, int height, int outputDevice, int outputDisplay);
	static BOOL destroy_BMOutput(BMOutput* instance);

	BOOL init_BMOutput(int width, int height, int outputDevice, int outputDisplay);

#ifdef SDI_OUTPUT_TEST
	void setCallback_getOutputAddress(callback_getOutputAddress cb, void* data);
#endif

	void* getBuffer();
	void* getLastBuffer();
	void putBuffer(void* buffer);

	void finishBufferCopy();

	HRESULT StartPlayback();
	HRESULT StopPlayback();

private:
	bool isStop();
	static DWORD WINAPI run(LPVOID handle);
	static HRESULT onSchedule(void* handle, IDeckLinkVideoFrame* videoFrame);
#ifdef SDI_OUTPUT_TEST
	BYTE* getOutputAddress(void* handle, void* data);
#endif

private:
	int mOutputDevice;
	int mOutputDisplay;
	int mOutputWidth;
	int mOutputHeight;
	bool mIsStop;
	void** mOutputBuffers;
	int mOutputIndex;
	double mTime;

	// Video mode parameters
	const BMDDisplayMode      kDisplayMode = bmdMode4K2160p30;
	const BMDVideoOutputFlags kOutputFlags = bmdVideoOutputFlagDefault;
	const BMDPixelFormat      kPixelFormat = bmdFormat8BitYUV;

	// Frame parameters
	const INT32_UNSIGNED kRowBytes = 7680; // 8 : (Width * 16 / 8), 10 : ((Width + 47) / 48) * 128, BGRA : (Width * 32 / 8)
	BMDTimeValue         gFrameDuration = 0;
	BMDTimeScale         gTimeScale = 0;

	// The display mode object corresponding to kDisplayMode
	IDeckLinkDisplayMode* gDisplayMode = NULL;

	// Keep track of the number of scheduled frames
	INT32_UNSIGNED gTotalFramesScheduled = 0;

	IDeckLinkIterator*      deckLinkIterator = NULL;
	IDeckLinkVideoConversion*	deckLinkVideoConversion = NULL;
	IDeckLink*              deckLink = NULL;
	IDeckLinkOutput*        deckLinkOutput = NULL;
	OutputCallback*         outputCallback = NULL;
	IDeckLinkVideoFrame*    videoFrame = NULL;

#ifdef SDI_OUTPUT_TEST
	callback_getOutputAddress mpGetOutputAddressCallback;
	void* mpGetOutputAddressCallbackData;
#endif
};
#endif // __BMOUTPUT_QURAM_H__