#ifndef __MICAUDIO_CAPTURE_QURAM_H__
#define __MICAUDIO_CAPTURE_QURAM_H__

#define USE_DIRECTSOUND

#ifdef USE_DIRECTSOUND
#else
#define USE_WINAPI
#endif

#include "AudioCapture.h"
#include <stdio.h>

#ifdef USE_DIRECTSOUND
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

#include <dsound.h>
#include "WaveFile.h"
#include <tchar.h>
#define NUM_REC_NOTIFICATIONS 16
#else
#ifdef USE_WINAPI
#include <MMSystem.h>
#include <fstream>
#include <tchar.h>
#include "WaveFile.h"
#define NUM_INPUT_BUFFERS 4
#else
#include "XICommon.h"
#endif
#endif

#define AUDIO_SAMPLESPERSEC 48000 // 48000	//44100
#define BUFFER_SIZE 7680//16384

class MICAudioCapture : public AudioCapture {
public:
	MICAudioCapture();

	static MICAudioCapture* create_AudioCapture(int iCam);
	static BOOL destroy_AudioCapture(MICAudioCapture* instance);

	BOOL init_AudioCapture(int iCam);
#ifdef USE_DIRECTSOUND
	BOOL reset_AudioCapture();
#endif

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
#ifdef USE_DIRECTSOUND
	HRESULT RecordCapturedData(MICAudioCapture *info);
	static DWORD WINAPI onInputError(LPVOID handle);
	static DWORD WINAPI run(LPVOID handle);
#else
#ifdef USE_WINAPI
	static void CALLBACK waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2);
#else
	static void onCapture(const BYTE * pbFrame, int cbFrame, void * pvParam, UINT64 u64TimeStamp);
#endif
#endif

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
	
#ifdef USE_WINAPI
	WAVEFORMATEX m_waveInFormatEx;
	HWAVEIN m_hWaveIn;
	int m_nWaveInHdrCount;
	WAVEHDR* m_pWaveInHdr;
	int m_nWaveInHdrPrepareCount;
	int m_iWaveInHdr;
#endif

#ifdef USE_DIRECTSOUND
	LPDIRECTSOUNDCAPTURE       m_pDSCapture = NULL;
	LPDIRECTSOUNDCAPTUREBUFFER m_pDSBCapture = NULL;
	LPDIRECTSOUNDNOTIFY        m_pDSNotify = NULL;
	HINSTANCE                  m_hInst = NULL;
	GUID                       m_guidCaptureDevice = GUID_NULL;
	BOOL                       m_bRecording;
	WAVEFORMATEX               m_wfxInput;
	DSBPOSITIONNOTIFY          m_aPosNotify[NUM_REC_NOTIFICATIONS + 1];
	HANDLE                     m_hNotificationEvent;
	BOOL                       m_abInputFormatSupported[20];
	DWORD                      m_dwCaptureBufferSize;
	DWORD                      m_dwNextCaptureOffset;
	DWORD                      m_dwNotifySize;
#endif
};

#endif // __MICAUDIO_CAPTURE_QURAM_H__