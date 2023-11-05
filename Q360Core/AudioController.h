#ifndef __AUDIO_CONTROLLER_QURAM_H__
#define __AUDIO_CONTROLLER_QURAM_H__

#include "Composer/Composer.h"
#include "AudioPlayer.h"
#include "Capture/AudioCapture.h"

#include <queue>

class AudioController {
public:
	typedef struct struct_AUDIO_CONTROLLER_HEADER {
		DWORD	dwRiffFlag;			// 'RIFF'
		DWORD	dwFileSize;
		DWORD	dwWaveFlag;			// 'WAVE'
		DWORD	dwFmtFlag;			// 'fmt'
		DWORD	dwFmtSize;
		WORD	wFormatTag;			/* format type */
		WORD	nChannels;			/* number of channels (i.e. mono, stereo, etc.) */
		DWORD	nSamplesPerSec;		/* sample rate */
		DWORD	nAvgBytesPerSec;	/* for buffer estimation */
		WORD	nBlockAlign;		/* block size of data */
		WORD	wBitsPerSample;
		DWORD	dwDataFlag;			// 'data'
		DWORD	dwDataSize;
	} AUDIO_CONTROLLER_HEADER, *PAUDIO_CONTROLLER_HEADER;

public:
	AudioController();

	static AudioController* create_AudioController(Composer* composer);
	static int destroy_AudioController(AudioController* instance);

	int init_AudioController(Composer* composer);

	bool isClear();
	int push(void* buffer, int size);
	int reset();
	int setOffset(int offset);
	int getOffset();

private:
	//static DWORD WINAPI run(LPVOID handle);
	static void onPush(void *handle, void *data, unsigned char* buffer, int size, long long timeStamp);

public:
	AUDIO_CONTROLLER_HEADER m_wfHeader;
	unsigned char* mAudioHeader;
	int mAudioHeaderSize;

	Composer* mComposer;
	AudioPlayer* mAudioPlayer;
	HANDLE mhOutputThread;

	bool mIsClear;

	int mIsAudioHeaderHDD;
	int mIsAudioHeaderRTMP;

	int mIsNeedReset;

	std::queue<unsigned char*> mPushBuffer;
	int mPushBufferSize;
};

#endif // __AUDIO_CONTROLLER_QURAM_H__