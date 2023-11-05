#include "AudioPlayer.h"

void AudioPlayer::Play()
{
	if (FALSE == m_bPlaying)
	{
		if (nullptr == m_pData || 0 == m_nDataLen)
			return;

		DSBUFFERDESC bufdesc;
		ZeroMemory(&bufdesc, sizeof(DSBUFFERDESC));
		bufdesc.dwSize = sizeof(DSBUFFERDESC);

		bufdesc.dwFlags = DSBCAPS_STATIC | DSBCAPS_GLOBALFOCUS | DSBCAPS_CTRLPOSITIONNOTIFY;
		bufdesc.dwBufferBytes = m_wfx.nAvgBytesPerSec / NUM_DIV * Num_of_buffers;
		bufdesc.lpwfxFormat = &m_wfx;

		CComPtr<IDirectSoundBuffer> pBuffer;
		HRESULT hr = m_lpDSound->CreateSoundBuffer(&bufdesc, &pBuffer, NULL);
		if (FAILED(hr))
		{
			return;
		}

		m_lpDSoundBuffer8.Release();
		GUID IID_IDSoundBuffer = { 0x6825a449, 0x7524, 0x4d82, 0x92, 0x0f, 0x50, 0xe3, 0x6a, 0xb3, 0xab, 0x1e };
		pBuffer->QueryInterface(IID_IDSoundBuffer, (LPVOID*)&m_lpDSoundBuffer8);

		void *write1 = 0, *write2 = 0;
		unsigned long length1 = 0, length2 = 0;
		hr = m_lpDSoundBuffer8->Lock(0, bufdesc.dwBufferBytes, &write1, &length1, &write2, &length2, 0);
		if (FAILED(hr))
		{
			return;
		}

		if (0 < length1)
		{
			memset(write1, 0x00, length1);
			memcpy_s(write1, length1, m_pData.get(), length1 >= m_nDataLen ? m_nDataLen : length1);
			m_nDataPos = length1 >= m_nDataLen ? m_nDataLen : length1;
		}

		hr = m_lpDSoundBuffer8->Unlock(write1, length1, write2, length2);

		GUID IID_IDSoundNotify = { 0xb0210783, 0x89cd, 0x11d0, 0xaf, 0x8, 0x0, 0xa0, 0xc9, 0x25, 0xcd, 0x16 };

		// Make Notification Point.
		CComPtr<IDirectSoundNotify8> lpDsNotify;
		DSBPOSITIONNOTIFY PositionNotify[Num_of_buffers];

		hr = m_lpDSoundBuffer8->QueryInterface(IID_IDSoundNotify, (LPVOID*)&lpDsNotify);
		if (SUCCEEDED(hr))
		{
			for (int i = 0; i < Num_of_buffers; i++)
			{
				PositionNotify[i].dwOffset = m_wfx.nAvgBytesPerSec / NUM_DIV * (i + 1) - 1;
				PositionNotify[i].hEventNotify = m_hEvents[i];
			}
			hr = lpDsNotify->SetNotificationPositions(Num_of_buffers, PositionNotify);

			m_timerID = timeSetEvent(5, 5, TimerProcess, (DWORD_PTR)this, TIME_PERIODIC | TIME_CALLBACK_FUNCTION);

			m_bPlaying = TRUE;
			//GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("Stop"));
			m_nDataPos = 0;

			m_lpDSoundBuffer8->SetCurrentPosition(0);
			m_lpDSoundBuffer8->Play(0, 0, DSBPLAY_LOOPING);
			printf("Audio Streaming Start!\n");
		}
	}
	else
	{
		timeKillEvent(m_timerID);

		m_lpDSoundBuffer8->Stop();

		m_bPlaying = FALSE;

		printf("Audio Streaming End!\n");
		//GetDlgItem(IDC_BTN_PLAY)->SetWindowText(_T("Play"));
	}
}

//int AudioPlayer::LoadWaveData(LPTSTR szFileName)
//{
//	if (NULL == szFileName)
//		return -1;
//
//	HMMIO hmmio = mmioOpen(szFileName, NULL, MMIO_ALLOCBUF | MMIO_READ);
//	if (NULL == hmmio)
//		return -2;
//
//	MMCKINFO mmckParent;
//	mmckParent.fccType = mmioFOURCC('W', 'A', 'V', 'E');
//
//	MMRESULT mmResult = mmioDescend(hmmio, &mmckParent, NULL, MMIO_FINDRIFF);
//	if (MMSYSERR_NOERROR != mmResult)
//	{
//		mmioClose(hmmio, 0);
//		return -3;
//	}
//
//	MMCKINFO mmckChild;
//	mmckChild.ckid = mmioFOURCC('f', 'm', 't', ' ');
//
//	mmResult = mmioDescend(hmmio, &mmckChild, &mmckParent, MMIO_FINDCHUNK);
//	if (MMSYSERR_NOERROR != mmResult)
//	{
//		mmioClose(hmmio, 0);
//		return -3;
//	}
//
//	LONG nFormatLen = mmckChild.cksize;
//	if (mmioRead(hmmio, (HPSTR)&m_wfx, nFormatLen) != nFormatLen)
//	{
//		mmioClose(hmmio, 0);
//		return -4;
//	}
//
//	mmResult = mmioAscend(hmmio, &mmckChild, 0);
//	if (MMSYSERR_NOERROR != mmResult)
//	{
//		mmioClose(hmmio, 0);
//		return -5;
//	}
//
//	mmckChild.ckid = mmioFOURCC('d', 'a', 't', 'a');
//
//	mmResult = mmioDescend(hmmio, &mmckChild, &mmckParent, MMIO_FINDCHUNK);
//	if (MMSYSERR_NOERROR != mmResult)
//	{
//		mmioClose(hmmio, 0);
//		return -3;
//	}
//
//	m_nDataLen = mmckChild.cksize;
//
//	char* pTemp = NULL;
//
//	//try
//	{
//		pTemp = new char[m_nDataLen];
//	}
//	//catch (CMemoryException* /*e*/){}
//	//catch (CException* /*e*/){}
//
//	if (NULL == pTemp)
//	{
//		mmioClose(hmmio, 0);
//		return -6;
//	}
//
//	if (m_nDataLen != mmioRead(hmmio, (HPSTR)pTemp, m_nDataLen))
//	{
//		delete[] pTemp;
//		mmioClose(hmmio, 0);
//		return -4;
//	}
//	m_pData.reset(pTemp, ArrayDeleter());
//	mmioClose(hmmio, 0);
//
//	return 0;
//}

int AudioPlayer::LoadWaveData(WORD formatTag, WORD channels, DWORD samplesPerSec, DWORD avgBytesPerSec, WORD blockAlign, WORD bitsPerSample, void* buffer, int size)
{
	printf("Audio Header Info : %d, %d, %d, %d, %d, %d, %p, %d\n", formatTag, channels, samplesPerSec, avgBytesPerSec, blockAlign, bitsPerSample, buffer, size);
	m_wfx.wFormatTag = formatTag;
	m_wfx.nChannels = channels;
	m_wfx.nSamplesPerSec = samplesPerSec;
	m_wfx.nAvgBytesPerSec = avgBytesPerSec;
	m_wfx.nBlockAlign = blockAlign;
	m_wfx.wBitsPerSample = bitsPerSample;

	m_nDataLen = m_wfx.nAvgBytesPerSec * NUM_BUFFER * 2;
	m_pTemp = new char[m_nDataLen];
	memset(m_pTemp, 0, m_nDataLen);
	m_pData.reset((char*)m_pTemp, ArrayDeleter());

	return 0;
}

int AudioPlayer::PushBuffer(void* buffer, int size)
{
	if (mIsRequestReset == true) {
		return 1;
	}

	m_Size = size;
	m_prePosition = mBufferCounter;

	char* pTemp = m_pData.get();
	if (pTemp == NULL)
		return -1;
	//printf("START mBufferCounter : %d, m_nDataLen : %d, pTemp : %p\n", mBufferCounter, m_nDataLen, pTemp);
	memcpy(pTemp + mBufferCounter, buffer, size);
	//printf("END\n");

	//printf("PUSH mBufferCounter : %d, m_nDataPos : %d, diff : %d\n", mBufferCounter, m_nDataPos, mBufferCounter - m_nDataPos);

	mBufferCounter += size;
	if ((int)m_nDataLen <= mBufferCounter) {
		mBufferCounter = 0;
		return 1;
	}

	if ((int)mBufferCounter - (int)m_nDataPos > UNIT_BUFFER * 4) {
		//printf("AudioPlayer ERROR: diff : %d, m_prePosition : %d, mBufferCounter : %d, size : %d, m_nDataPos : %d, m_nDataLen : %d, UNIT_BUFFER : %d\n", mBufferCounter - m_nDataPos, m_prePosition, mBufferCounter, size, m_nDataPos, m_nDataLen, UNIT_BUFFER);
		ResetBufferPosition();
	}

	return 0;
}

//int AudioPlayer::PushBuffer(void* buffer, int size)
//{
//	mBufferCounter += size;
//	if ((int)m_nDataLen <= mBufferCounter) {
//		mBufferCounter = 0;
//	}
//
//	char* pTemp = m_pData.get();
//	if (pTemp == NULL)
//		return -1;
//	//printf("START mBufferCounter : %d, m_nDataLen : %d, pTemp : %p\n", mBufferCounter, m_nDataLen, pTemp);
//	memcpy(pTemp + mBufferCounter, buffer, size);
//	//int clearPosition = mBufferCounter - (int)(m_wfx.nAvgBytesPerSec) * NUM_BUFFER;
//	//if (clearPosition < 0)
//	//	clearPosition += m_nDataLen;
//	//memset(pTemp + clearPosition, 0, size);
//	//printf("END\n");
//
//	return 0;
//}

int AudioPlayer::ResetBufferPosition()
{
	//int temp = m_nDataPos + UNIT_BUFFER;
	//if (m_nDataLen <= temp)
	//	temp = 0;

	//mBufferCounter = temp;

	mIsRequestReset = true;

	return 1;
}

int AudioPlayer::SetBufferOffset(int offset)
{
	const int size = UNIT_BUFFER;

	int temp_offset = mBufferOffset + (offset*size);

	if (temp_offset > 0)
		temp_offset = 0;
//	else if (temp_offset < ((size*NUM_TEMP_BUFFER) - (int)m_nDataLen))
//		temp_offset = ((size*NUM_TEMP_BUFFER) - (int)m_nDataLen);
	else if (temp_offset < -(int)(m_wfx.nAvgBytesPerSec) * NUM_BUFFER)
		temp_offset = -(int)(m_wfx.nAvgBytesPerSec) * NUM_BUFFER;

	mBufferOffset = temp_offset;
	return mBufferOffset;
}

int AudioPlayer::GetBufferOffset()
{
	return mBufferOffset;
}

int AudioPlayer::getBufferSize()
{
	return m_Size;
}

unsigned char* AudioPlayer::getBuffer()
{
	int bufferpos = (int)m_prePosition + mBufferOffset;
	if (bufferpos < 0)
		bufferpos += (int)m_nDataLen;

	return (unsigned char*)(m_pData.get() + bufferpos);
}

void AudioPlayer::setCallback_onPush(callback_onPush cb, void* data) {
	mpPushCallback = cb;
	mpPushCallbackData = data;
}

void CALLBACK AudioPlayer::TimerProcess(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	AudioPlayer* pDDS = (AudioPlayer*)dwUser;
	HRESULT hr;

	void *write1 = 0, *write2 = 0;
	unsigned long length1, length2;

	DWORD dwResult = WaitForMultipleObjects(Num_of_buffers, pDDS->m_hEvents, FALSE, 0);

	if ((dwResult - WAIT_OBJECT_0) < Num_of_buffers)
	{
		//TRACE(_T("[LOG] Wait : %d\n"), dwResult);
		DWORD nIndex = dwResult - WAIT_OBJECT_0;

		hr = pDDS->m_lpDSoundBuffer8->Lock((pDDS->m_wfx.nAvgBytesPerSec / NUM_DIV) * nIndex, pDDS->m_wfx.nAvgBytesPerSec / NUM_DIV, &write1, &length1, &write2, &length2, 0);

		if (0 < length1)
		{
			BYTE* pTemp = (BYTE*)pDDS->m_pData.get();

			int nSize = (length1 > pDDS->m_nDataLen - pDDS->m_nDataPos) ? pDDS->m_nDataLen - pDDS->m_nDataPos : length1;
			int bufferpos = (int)pDDS->m_nDataPos + pDDS->mBufferOffset;
			int encpos = bufferpos + (UNIT_BUFFER * -15);
			if (bufferpos < 0)
				bufferpos += (int)pDDS->m_nDataLen;
			if (encpos < 0)
				encpos += (int)pDDS->m_nDataLen;

			//memset(write1, 0x00, length1);
			//memcpy_s(write1, nSize, pDDS->m_pData.get() + pDDS->m_nDataPos, nSize);
			memcpy_s(write1, nSize, pTemp + bufferpos, nSize);

			//printf("PLAY mBufferCounter : %d, m_nDataPos : %d, diff : %d\n", pDDS->mBufferCounter, pDDS->m_nDataPos, pDDS->mBufferCounter - pDDS->m_nDataPos);

			int clearPosition = pDDS->m_nDataPos - (int)(pDDS->m_wfx.nAvgBytesPerSec) * 3;
			if (clearPosition < 0)
				clearPosition += pDDS->m_nDataLen;
			memset(pTemp + clearPosition, 0, length1);

			pDDS->m_nDataPos += nSize;
			if (pDDS->m_nDataLen <= pDDS->m_nDataPos)
				pDDS->m_nDataPos = 0;

			if (pDDS->mpPushCallback) {
				pDDS->mpPushCallback(pDDS, pDDS->mpPushCallbackData, pTemp + encpos, nSize, 0);
			}

			if (pDDS->mIsRequestReset == true) {
				int temp = pDDS->m_nDataPos + UNIT_BUFFER;
				if (pDDS->m_nDataLen <= temp)
					temp = 0;

				pDDS->mBufferCounter = temp;

				pDDS->mIsRequestReset = false;
			}
		}

		pDDS->m_lpDSoundBuffer8->Unlock(write1, length1, write2, length2);
	}
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	return (DefWindowProc(hWnd, iMsg, wParam, lParam));
}