#ifndef __AUDIO_PLAYER_QURAM_H__
#define __AUDIO_PLAYER_QURAM_H__

#pragma once

#include <memory>
#include <Windows.h>
#include <MMSystem.h>
#include <dsound.h>
#include <atlcomcli.h>

#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "dsound.lib")

#define BITRATE				192000
#define NUM_DIV				25
#define UNIT_BUFFER			BITRATE / NUM_DIV
#define NUM_BUFFER			3 // second

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam);

typedef void(*callback_onPush)(void *handle, void *data, unsigned char* buffer, int size, long long timeStamp);

class AudioPlayer {
	struct ArrayDeleter
	{
		template <class T>
		void operator()(T* p)
		{
			delete[] p;
		}
	};

public:
	AudioPlayer() {
		m_bPlaying = FALSE;
		mBufferCounter = 0;
		mBufferOffset = 0;
		mIsRequestReset = false;

		mpPushCallback = NULL;
		mpPushCallbackData = NULL;

		m_pTemp = NULL;

		memset(m_hEvents, 0x00, sizeof(HANDLE)* Num_of_buffers);
		m_timerID = MMSYSERR_NOERROR;
		memset(&m_wfx, 0x00, sizeof(WAVEFORMATEX));

		HRESULT hr = DirectSoundCreate8(NULL, &m_lpDSound, NULL);
		if (FAILED(hr))
		{
			printf("DirectSoundCreate8 Fail.\n");
		}
		else
		{
			WNDCLASS WndClass = { 0 };
			hInstance = GetModuleHandle(NULL);

			WndClass.hInstance = hInstance;
			WndClass.lpfnWndProc = WndProc;
			WndClass.lpszClassName = lpszClass;
			RegisterClass(&WndClass);

			m_hWnd = CreateWindow(lpszClass, lpszClass, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, (HMENU)NULL, hInstance, NULL);

			hr = m_lpDSound->SetCooperativeLevel(m_hWnd, DSSCL_PRIORITY);

			for (int i = 0; i < Num_of_buffers; i++)
				m_hEvents[i] = CreateEvent(NULL, FALSE, NULL, NULL);
		}
	}
	~AudioPlayer() {
		if (m_bPlaying == TRUE) {
			timeKillEvent(m_timerID);

			m_lpDSoundBuffer8->Stop();

			m_bPlaying = FALSE;

			printf("Audio Streaming End!\n");
		}
	}

	void Play();
	//int LoadWaveData(LPTSTR szFileName);
	int LoadWaveData(WORD formatTag, WORD channels, DWORD samplesPerSec, DWORD avgBytesPerSec, WORD blockAlign, WORD bitsPerSample, void* buffer = NULL, int size = 0);
	int PushBuffer(void* buffer, int size);
	int ResetBufferPosition();
	int SetBufferOffset(int offset);
	int GetBufferOffset();

	int getBufferSize();
	unsigned char* getBuffer();
	void setCallback_onPush(callback_onPush cb, void* data);

protected:
	static void CALLBACK TimerProcess(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);

private:
	enum { Num_of_buffers = 4 };
	BOOL m_bPlaying;
	std::shared_ptr<char> m_pData;
	DWORD m_Size;
	DWORD m_prePosition;
	DWORD m_nDataLen;
	DWORD m_nDataPos;

	char* m_pTemp;

	CComPtr<IDirectSound8> m_lpDSound;
	CComPtr<IDirectSoundBuffer8> m_lpDSoundBuffer8;

	HANDLE m_hEvents[Num_of_buffers];
	MMRESULT m_timerID;
	WAVEFORMATEX m_wfx;

	HWND m_hWnd;
	HINSTANCE hInstance;
	const TCHAR* lpszClass = _T("AUDIOPLAYER");

	int mBufferCounter;
	int mBufferOffset;

	bool mIsRequestReset;

	callback_onPush mpPushCallback;
	void* mpPushCallbackData;
};

#endif // __AUDIO_PLAYER_QURAM_H__