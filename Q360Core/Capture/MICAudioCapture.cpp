#include "MICAudioCapture.h"

// public functions //

MICAudioCapture::MICAudioCapture() {

}

MICAudioCapture* MICAudioCapture::create_AudioCapture(int iCam) {
	MICAudioCapture* instance = new MICAudioCapture();

	instance->setClass(MIC_AUDIO);
	instance->init_AudioCapture(iCam);

	return instance;
}

#ifdef USE_DIRECTSOUND
BOOL MICAudioCapture::init_AudioCapture(int iCam) {
	mIsClear = false;
	mhCaptureThread = NULL;
	mpCaptureDoneCallback = NULL;
	mpCaptureDoneCallbackData = NULL;

	mCameraIndex = iCam;
	m_hNotificationEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	do {
		file.Init(_T("test.wav"), AUDIO_SAMPLESPERSEC);

		setHeaderBuffer(file.getHeader());
		setHeaderBufferSize(file.getHeaderSize());

		setwFormatTag(file.m_wfHeader.pcmFormat.wf.wFormatTag);
		setnChannels(file.m_wfHeader.pcmFormat.wf.nChannels);
		setnSamplesPerSec(file.m_wfHeader.pcmFormat.wf.nSamplesPerSec);
		setnAvgBytesPerSec(file.m_wfHeader.pcmFormat.wf.nAvgBytesPerSec);
		setnBlockAlign(file.m_wfHeader.pcmFormat.wf.nBlockAlign);
		setwBitsPerSample(file.m_wfHeader.pcmFormat.wBitsPerSample);

		MMRESULT mRes;

		memset(&m_wfxInput, 0, sizeof(m_wfxInput));
		m_wfxInput.wFormatTag = WAVE_FORMAT_PCM;
		m_wfxInput.nChannels = file.m_wfHeader.pcmFormat.wf.nChannels;
		m_wfxInput.nSamplesPerSec = file.m_wfHeader.pcmFormat.wf.nSamplesPerSec;
		m_wfxInput.wBitsPerSample = file.m_wfHeader.pcmFormat.wBitsPerSample;
		m_wfxInput.nBlockAlign = m_wfxInput.nChannels*m_wfxInput.wBitsPerSample / 8;
		m_wfxInput.nAvgBytesPerSec = m_wfxInput.nSamplesPerSec*m_wfxInput.nBlockAlign;
		m_wfxInput.cbSize = 0;

		//printf("%d %d %d %d %d %d\n", m_waveInFormatEx.wFormatTag, m_waveInFormatEx.nChannels, m_waveInFormatEx.nSamplesPerSec, m_waveInFormatEx.wBitsPerSample, m_waveInFormatEx.nBlockAlign, m_waveInFormatEx.nAvgBytesPerSec);

		HRESULT hr;



		//// InitDirectSound ////
		ZeroMemory(&m_aPosNotify, sizeof(DSBPOSITIONNOTIFY)*
			(NUM_REC_NOTIFICATIONS + 1));
		m_dwCaptureBufferSize = 0;
		m_dwNotifySize = 0;

		// Initialize COM
		if (FAILED(hr = CoInitialize(NULL))) {
			printf("MICAudioCapture ERROR: CoInitialize fail! (%d)\n", hr);
			break;
		}

		// Create IDirectSoundCapture using the preferred capture device
		if (FAILED(hr = DirectSoundCaptureCreate(&m_guidCaptureDevice, &m_pDSCapture, NULL))) {
			printf("MICAudioCapture ERROR: DirectSoundCaptureCreate fail! (%d)\n", hr);
			break;
		}



		//// CreateCaptureBuffer ////
		DSCBUFFERDESC dscbd;

		// Set the notification size
		m_dwNotifySize = BUFFER_SIZE;// MAX(1024, m_wfxInput.nAvgBytesPerSec / 8);
		m_dwNotifySize -= m_dwNotifySize % m_wfxInput.nBlockAlign;

		// Set the buffer sizes 
		m_dwCaptureBufferSize = m_dwNotifySize * NUM_REC_NOTIFICATIONS;

		// Create the capture buffer
		ZeroMemory(&dscbd, sizeof(dscbd));
		dscbd.dwSize = sizeof(dscbd);
		dscbd.dwBufferBytes = m_dwCaptureBufferSize;
		dscbd.lpwfxFormat = &m_wfxInput; // Set the format during creatation

		if (FAILED(hr = m_pDSCapture->CreateCaptureBuffer(&dscbd,
			&m_pDSBCapture,
			NULL))) {
			printf("MICAudioCapture ERROR: CreateCaptureBuffer fail! (%d)\n", hr);
			break;
		}

		m_dwNextCaptureOffset = 0;



		//// InitNotifications ////
		if (NULL == m_pDSBCapture) {
			printf("MICAudioCapture ERROR: m_pDSBCapture is NULL!\n");
			break;
		}

		// Create a notification event, for when the sound stops playing
		if (FAILED(hr = m_pDSBCapture->QueryInterface(IID_IDirectSoundNotify,
			(VOID**)&m_pDSNotify))) {
			printf("MICAudioCapture ERROR: QueryInterface fail! (%d)\n", hr);
			break;
		}

		// Setup the notification positions
		for (INT i = 0; i < NUM_REC_NOTIFICATIONS; i++)
		{
			m_aPosNotify[i].dwOffset = (m_dwNotifySize * i) + m_dwNotifySize - 1;
			m_aPosNotify[i].hEventNotify = m_hNotificationEvent;
		}

		// Tell DirectSound when to notify us. the notification will come in the from 
		// of signaled events that are handled in WinMain()
		if (FAILED(hr = m_pDSNotify->SetNotificationPositions(NUM_REC_NOTIFICATIONS,
			m_aPosNotify))) {
			printf("MICAudioCapture ERROR: SetNotificationPositions fail! (%d)\n", hr);
			break;
		}



		//// StartRecord ////
		if (FAILED(hr = m_pDSBCapture->Start(DSCBSTART_LOOPING))) {
			printf("MICAudioCapture ERROR: Start fail! (%d)\n", hr);
			break;
		}

		mhCaptureThread = CreateThread(NULL, 0, run, this, 0, NULL);

		printf("Initialize MICAudioCapture\n");
		return TRUE;
	} while (false);

	// Stop the capture and read any data that 
	// was not caught by a notification
	if (NULL != m_pDSBCapture) {
		m_pDSBCapture->Stop();
	}

	// Release DirectSound interfaces
	if (m_pDSNotify != NULL) {
		m_pDSNotify->Release();
		m_pDSNotify = NULL;
	}
	if (m_pDSBCapture != NULL) {
		m_pDSBCapture->Release();
		m_pDSBCapture = NULL;
	}
	if (m_pDSCapture != NULL) {
		m_pDSCapture->Release();
		m_pDSCapture = NULL;
	}

	// Release COM
	CoUninitialize();

	if (m_hNotificationEvent) {
		CloseHandle(m_hNotificationEvent);
		m_hNotificationEvent = NULL;
	}

	return reset_AudioCapture();
}
BOOL MICAudioCapture::reset_AudioCapture() {
	m_hNotificationEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	do {
		HRESULT hr;



		//// InitDirectSound ////
		ZeroMemory(&m_aPosNotify, sizeof(DSBPOSITIONNOTIFY)*
			(NUM_REC_NOTIFICATIONS + 1));
		m_dwCaptureBufferSize = 0;
		m_dwNotifySize = 0;

		// Initialize COM
		if (FAILED(hr = CoInitialize(NULL))) {
			printf("MICAudioCapture ERROR: CoInitialize fail! (%d)\n", hr);
			break;
		}

		// Create IDirectSoundCapture using the preferred capture device
		if (FAILED(hr = DirectSoundCaptureCreate(&m_guidCaptureDevice, &m_pDSCapture, NULL))) {
			printf("MICAudioCapture ERROR: DirectSoundCaptureCreate fail! (%d)\n", hr);
			break;
		}



		//// CreateCaptureBuffer ////
		DSCBUFFERDESC dscbd;

		// Set the notification size
		m_dwNotifySize = BUFFER_SIZE;// MAX(1024, m_wfxInput.nAvgBytesPerSec / 8);
		m_dwNotifySize -= m_dwNotifySize % m_wfxInput.nBlockAlign;

		// Set the buffer sizes 
		m_dwCaptureBufferSize = m_dwNotifySize * NUM_REC_NOTIFICATIONS;

		// Create the capture buffer
		ZeroMemory(&dscbd, sizeof(dscbd));
		dscbd.dwSize = sizeof(dscbd);
		dscbd.dwBufferBytes = m_dwCaptureBufferSize;
		dscbd.lpwfxFormat = &m_wfxInput; // Set the format during creatation

		if (FAILED(hr = m_pDSCapture->CreateCaptureBuffer(&dscbd,
			&m_pDSBCapture,
			NULL))) {
			printf("MICAudioCapture ERROR: CreateCaptureBuffer fail! (%d)\n", hr);
			break;
		}

		m_dwNextCaptureOffset = 0;



		//// InitNotifications ////
		if (NULL == m_pDSBCapture) {
			printf("MICAudioCapture ERROR: m_pDSBCapture is NULL!\n");
			break;
		}

		// Create a notification event, for when the sound stops playing
		if (FAILED(hr = m_pDSBCapture->QueryInterface(IID_IDirectSoundNotify,
			(VOID**)&m_pDSNotify))) {
			printf("MICAudioCapture ERROR: QueryInterface fail! (%d)\n", hr);
			break;
		}

		// Setup the notification positions
		for (INT i = 0; i < NUM_REC_NOTIFICATIONS; i++)
		{
			m_aPosNotify[i].dwOffset = (m_dwNotifySize * i) + m_dwNotifySize - 1;
			m_aPosNotify[i].hEventNotify = m_hNotificationEvent;
		}

		// Tell DirectSound when to notify us. the notification will come in the from 
		// of signaled events that are handled in WinMain()
		if (FAILED(hr = m_pDSNotify->SetNotificationPositions(NUM_REC_NOTIFICATIONS,
			m_aPosNotify))) {
			printf("MICAudioCapture ERROR: SetNotificationPositions fail! (%d)\n", hr);
			break;
		}



		//// StartRecord ////
		if (FAILED(hr = m_pDSBCapture->Start(DSCBSTART_LOOPING))) {
			printf("MICAudioCapture ERROR: Start fail! (%d)\n", hr);
			break;
		}

		mhCaptureThread = CreateThread(NULL, 0, run, this, 0, NULL);

		printf("Initialize MICAudioCapture\n");
		return TRUE;
	} while (false);

	return FALSE;
}
#else
#ifdef USE_WINAPI
BOOL MICAudioCapture::init_AudioCapture(int iCam) {
	mIsClear = false;
	mhChannel = NULL;
	mpCaptureDoneCallback = NULL;
	m_hWaveIn = NULL;

	mCameraIndex = iCam;

	do {
		file.Init(_T("test.wav"), AUDIO_SAMPLESPERSEC);

		setHeaderBuffer(file.getHeader());
		setHeaderBufferSize(file.getHeaderSize());

		setwFormatTag(file.m_wfHeader.pcmFormat.wf.wFormatTag);
		setnChannels(file.m_wfHeader.pcmFormat.wf.nChannels);
		setnSamplesPerSec(file.m_wfHeader.pcmFormat.wf.nSamplesPerSec);
		setnAvgBytesPerSec(file.m_wfHeader.pcmFormat.wf.nAvgBytesPerSec);
		setnBlockAlign(file.m_wfHeader.pcmFormat.wf.nBlockAlign);
		setwBitsPerSample(file.m_wfHeader.pcmFormat.wBitsPerSample);

		MMRESULT mRes;

		memset(&m_waveInFormatEx, 0, sizeof(m_waveInFormatEx));
		m_waveInFormatEx.wFormatTag = WAVE_FORMAT_PCM;
		m_waveInFormatEx.nChannels = file.m_wfHeader.pcmFormat.wf.nChannels;
		m_waveInFormatEx.nSamplesPerSec = file.m_wfHeader.pcmFormat.wf.nSamplesPerSec;
		m_waveInFormatEx.wBitsPerSample = file.m_wfHeader.pcmFormat.wBitsPerSample;
		m_waveInFormatEx.nBlockAlign = m_waveInFormatEx.nChannels*m_waveInFormatEx.wBitsPerSample / 8;
		m_waveInFormatEx.nAvgBytesPerSec = m_waveInFormatEx.nSamplesPerSec*m_waveInFormatEx.nBlockAlign;
		m_waveInFormatEx.cbSize = 0;

		//printf("%d %d %d %d %d %d\n", m_waveInFormatEx.wFormatTag, m_waveInFormatEx.nChannels, m_waveInFormatEx.nSamplesPerSec, m_waveInFormatEx.wBitsPerSample, m_waveInFormatEx.nBlockAlign, m_waveInFormatEx.nAvgBytesPerSec);

		int n = waveInGetNumDevs();
		int deviceID = -1;

		for (int i = 0; i < n; i++) {
			WAVEINCAPS wic;
			waveInGetDevCaps(i, &wic, sizeof(wic));

			if (_tcsstr(wic.szPname, _T("High Definition Audio"))) {
				deviceID = i;
				break;
			}
		}

		if (deviceID == -1) {
			printf("Device Disconnected!\n");
			continue;
		}

		mRes = waveInOpen(&m_hWaveIn, deviceID, &m_waveInFormatEx,
			(DWORD_PTR)waveInProc, (DWORD_PTR)this, CALLBACK_FUNCTION);

		if (mRes != MMSYSERR_NOERROR) {
			printf("waveInOpen : %d\n", mRes);
			continue;
		}

		m_nWaveInHdrCount = NUM_INPUT_BUFFERS;
		m_pWaveInHdr = new WAVEHDR[m_nWaveInHdrCount];

		for (int i = 0; i<m_nWaveInHdrCount; i++)
		{
			memset(&m_pWaveInHdr[i], 0, sizeof(WAVEHDR));
			m_pWaveInHdr[i].lpData = reinterpret_cast <LPSTR> (malloc(BUFFER_SIZE));
			m_pWaveInHdr[i].dwBufferLength = BUFFER_SIZE;
			m_pWaveInHdr[i].dwUser = i;

			mRes = waveInPrepareHeader(m_hWaveIn, &m_pWaveInHdr[i], sizeof(WAVEHDR));
			if (mRes != MMSYSERR_NOERROR) {
				printf("waveInPrepareHeader : %d\n", mRes);
				return FALSE;
			}

			mRes = waveInAddBuffer(m_hWaveIn, &m_pWaveInHdr[i], sizeof(WAVEHDR));
			if (mRes != MMSYSERR_NOERROR) {
				printf("waveInAddBuffer : %d\n", mRes);
				return FALSE;
			}
		}

		mRes = waveInStart(m_hWaveIn);
		if (mRes != MMSYSERR_NOERROR) {
			printf("waveInStart : %d\n", mRes);
			continue;
		}

		printf("Initialize MICAudioCapture\n");
		return TRUE;
	} while (false);

	return FALSE;
}
#else
BOOL MICAudioCapture::init_AudioCapture(int iCam) {
	mIsClear = false;
	mhChannel = NULL;
	mpCaptureDoneCallback = NULL;

	mCameraIndex = iCam;

	do {
		if (!XIS_Initialize()) {
			printf("MICAudioCapture ERROR: XICaptureInitInstance return fail!\n");
			break;
		}

		int cAudioCaptures = XIS_GetAudioCaptureCount();
		if (cAudioCaptures <= 0) {
			printf("MICAudioCapture ERROR: Can't find capture card!\n");
			break;
		}

		AUDIO_CAPTURE_INFO_EX audioInfo;
		int iMICDevice = 0;
		bool isFind = false;
		for (int i = 0; i < cAudioCaptures; i++) {
			if (!XIS_GetAudioCaptureInfoEx(i, &audioInfo)) {
				printf("MICAudioCapture ERROR: Can't get channel info!\n");
				break;
			}

			//if (_tcsstr(audioInfo.szID, _T("usb")) || _tcsstr(audioInfo.szName, _T("USB"))) {
			if (_tcsstr(audioInfo.szName, _T("High Definition Audio"))) {
				if (iMICDevice == mCameraIndex) {
					isFind = true;
					break;
				}
				else
					iMICDevice++;
			}
		}
		if (isFind == false) {
			printf("MICAudioCapture ERROR: Can't find USB device!\n");
			break;
		}

		mhChannel = XIS_OpenAudioCaptureEx(audioInfo.szID);
		if (mhChannel == NULL) {
			printf("MICAudioCapture ERROR: Open channel %s:%d error!\n", audioInfo.szID, mCameraIndex);
			break;
		}

		int cSamplesPerFrame = AUDIO_SAMPLESPERSEC / 25;
		if (!XIS_SetAudioCaptureFormat(mhChannel, AUDIO_SAMPLESPERSEC, 2, cSamplesPerFrame, 10)) {
			printf("MICAudioCapture ERROR: XIS_SetAudioCaptureFormat return fail!\n");
			break;
		}

		if (!XIS_SetAudioCaptureCallbackEx(mhChannel, onCapture, this)) {
			printf("MICAudioCapture ERROR: XIS_SetAudioCaptureCallbackEx return fail!\n");
			break;
		}

		if (!XIS_StartAudioCapture(mhChannel)) {
			printf("MICAudioCapture ERROR: Start audio capture fail!\n");
			break;
		}

		file.Init(_T("test.wav"), AUDIO_SAMPLESPERSEC);

		setHeaderBuffer(file.getHeader());
		setHeaderBufferSize(file.getHeaderSize());

		setwFormatTag(file.m_wfHeader.pcmFormat.wf.wFormatTag);
		setnChannels(file.m_wfHeader.pcmFormat.wf.nChannels);
		setnSamplesPerSec(file.m_wfHeader.pcmFormat.wf.nSamplesPerSec);
		setnAvgBytesPerSec(file.m_wfHeader.pcmFormat.wf.nAvgBytesPerSec);
		setnBlockAlign(file.m_wfHeader.pcmFormat.wf.nBlockAlign);
		setwBitsPerSample(file.m_wfHeader.pcmFormat.wBitsPerSample);

		printf("Initialize XICapture SDK for MICAudioCapture\n");
		return TRUE;
	} while (false);

	return FALSE;
}
#endif
#endif

BOOL MICAudioCapture::destroy_AudioCapture(MICAudioCapture* instance) {
	if (instance) {
		instance->mIsClear = true;

#ifdef USE_DIRECTSOUND
		DWORD result = WaitForSingleObject(instance->mhCaptureThread, 3000);
		if (result == WAIT_OBJECT_0)
			printf("Destroy capture thread\n");
		else if (result == WAIT_TIMEOUT)
			printf("ERROR: Can't destroy capture thread!\n");

		if (instance->m_hNotificationEvent) {
			CloseHandle(instance->m_hNotificationEvent);
			instance->m_hNotificationEvent = NULL;
		}
#else
#ifdef USE_WINAPI
		if (instance->m_hWaveIn) {
			waveInStop(instance->m_hWaveIn);
			waveInReset(instance->m_hWaveIn);

			if (instance->m_pWaveInHdr != NULL) {
				for (int i = 0; i < NUM_INPUT_BUFFERS; i++)
				{
					waveInUnprepareHeader(instance->m_hWaveIn, &(instance->m_pWaveInHdr[i]), sizeof(WAVEHDR));
				}
				instance->m_pWaveInHdr = NULL;
			}

			waveInClose(instance->m_hWaveIn);

			instance->m_hWaveIn = NULL;
		}
#else
		if (instance->mhChannel != NULL) {
			XIS_StopAudioCapture(instance->mhChannel);
			XIS_CloseAudioCapture(instance->mhChannel);
			instance->mhChannel = NULL;
		}
#endif
#endif

		delete instance;

		printf("Destroy MICAudioCapture\n");
		return TRUE;
	}

	printf("ERROR: MICAudioCapture instance : NULL!\n");
	return FALSE;
}

void MICAudioCapture::setHChannel(HANDLE hChannel) {
	mhChannel = hChannel;
}

void MICAudioCapture::setHeaderBuffer(unsigned char* headerBuffer) {
	mHeaderBuffer = headerBuffer;
}

void MICAudioCapture::setHeaderBufferSize(int headerBufferSize) {
	mHeaderBufferSize = headerBufferSize;
}

void MICAudioCapture::setwFormatTag(WORD wFormatTag) {
	mwFormatTag = wFormatTag;
}

void MICAudioCapture::setnChannels(WORD nChannels) {
	mnChannels = nChannels;
}

void MICAudioCapture::setnSamplesPerSec(DWORD nSamplesPerSec) {
	mnSamplesPerSec = nSamplesPerSec;
}

void MICAudioCapture::setnAvgBytesPerSec(DWORD nAvgBytesPerSec) {
	mnAvgBytesPerSec = nAvgBytesPerSec;
}

void MICAudioCapture::setnBlockAlign(WORD nBlockAlign) {
	mnBlockAlign = nBlockAlign;
}

void MICAudioCapture::setwBitsPerSample(WORD wBitsPerSample) {
	mwBitsPerSample = wBitsPerSample;
}

int MICAudioCapture::getCameraIndex() {
	return mCameraIndex;
}

HANDLE MICAudioCapture::getHChannel() {
	return mhChannel;
}

unsigned char* MICAudioCapture::getHeaderBuffer() {
	return mHeaderBuffer;
}

int MICAudioCapture::getHeaderBufferSize() {
	return mHeaderBufferSize;
}

WORD MICAudioCapture::getwFormatTag() {
	return mwFormatTag;
}

WORD MICAudioCapture::getnChannels() {
	return mnChannels;
}

DWORD MICAudioCapture::getnSamplesPerSec() {
	return mnSamplesPerSec;
}

DWORD MICAudioCapture::getnAvgBytesPerSec() {
	return mnAvgBytesPerSec;
}

WORD MICAudioCapture::getnBlockAlign() {
	return mnBlockAlign;
}

WORD MICAudioCapture::getwBitsPerSample() {
	return mwBitsPerSample;
}

void MICAudioCapture::setCallback_onCaptureDone(callback_onAudioCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

// private functions //

bool MICAudioCapture::isClear() {
	return mIsClear;
}

#ifdef USE_DIRECTSOUND
HRESULT MICAudioCapture::RecordCapturedData(MICAudioCapture *info) {
	HRESULT hr;
	VOID*   pbCaptureData = NULL;
	DWORD   dwCaptureLength;
	VOID*   pbCaptureData2 = NULL;
	DWORD   dwCaptureLength2;
	UINT    dwDataWrote;
	DWORD   dwReadPos;
	DWORD   dwCapturePos;
	LONG    lLockSize;

	if (NULL == m_pDSBCapture) {
		printf("MICAudioCapture ERROR: m_pDSBCapture is NULL!\n");
		return E_FAIL;
	}

	if (FAILED(hr = m_pDSBCapture->GetCurrentPosition(&dwCapturePos, &dwReadPos))) {
		printf("MICAudioCapture ERROR: GetCurrentPosition fail! (%d)\n", hr);
		return E_FAIL;
	}

	lLockSize = dwReadPos - m_dwNextCaptureOffset;
	if (lLockSize < 0)
		lLockSize += m_dwCaptureBufferSize;

	// Block align lock size so that we are always write on a boundary
	lLockSize -= (lLockSize % m_dwNotifySize);

	if (lLockSize == 0) {
		printf("MICAudioCapture ERROR: lLockSize is zero!\n");
		return S_OK;
	}

	// Lock the capture buffer down
	if (FAILED(hr = m_pDSBCapture->Lock(m_dwNextCaptureOffset, lLockSize,
		&pbCaptureData, &dwCaptureLength,
		&pbCaptureData2, &dwCaptureLength2, 0L))) {
		printf("MICAudioCapture ERROR: Lock fail! (%d)\n", hr);
		return E_FAIL;
	}

	//// Write the data into the wav file
	//g_file.Write((BYTE*)pbCaptureData, dwCaptureLength);

	// Move the capture offset along
	m_dwNextCaptureOffset += dwCaptureLength;
	m_dwNextCaptureOffset %= m_dwCaptureBufferSize; // Circular buffer

	if (pbCaptureData2 != NULL)
	{
		// Write the data into the wav file
		//g_file.Write((BYTE*)pbCaptureData2, dwCaptureLength2);

		// Move the capture offset along
		m_dwNextCaptureOffset += dwCaptureLength2;
		m_dwNextCaptureOffset %= m_dwCaptureBufferSize; // Circular buffer
	}

	// Unlock the capture buffer
	m_pDSBCapture->Unlock(pbCaptureData, dwCaptureLength,
		pbCaptureData2, dwCaptureLength2);

	if (info->mpCaptureDoneCallback) {
		info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, (BYTE*)pbCaptureData, BUFFER_SIZE, 0);
	}
	else
		printf("MICAudioCapture ERROR: onCaptureDone callback : NULL (buffer 1)\n");

	if (pbCaptureData2 != NULL)
	{
		if (info->mpCaptureDoneCallback) {
			info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, (BYTE*)pbCaptureData2, BUFFER_SIZE, 0);
		}
		else
			printf("MICAudioCapture ERROR: onCaptureDone callback : NULL (buffer 2)\n");
	}

	return S_OK;
}

DWORD WINAPI MICAudioCapture::onInputError(LPVOID handle) {
	MICAudioCapture *info = (MICAudioCapture *)handle;

	if (info->mpInputErrorCallback) {
		info->mpInputErrorCallback(info, info->mpInputErrorCallbackData, 0, 0);
	}

	return 0;
}

DWORD WINAPI MICAudioCapture::run(LPVOID handle) {
	MICAudioCapture *info = (MICAudioCapture *)handle;

	printf("MICAudioCapture run()\n");

	bool bDone = FALSE;
	DWORD dwResult;
	while (!bDone)
	{
		if (info->isClear()) {
			printf("MICAudioCapture Clear.\n");
			break;
		}

		dwResult = MsgWaitForMultipleObjects(1, &(info->m_hNotificationEvent),
			FALSE, 1000, QS_ALLEVENTS);

		//if (dwResult != WAIT_OBJECT_0 + 0)
		//{
		//	// Stop the capture and read any data that 
		//	// was not caught by a notification
		//	if (NULL != info->m_pDSBCapture) {
		//		info->m_pDSBCapture->Stop();
		//	}

		//	// Release DirectSound interfaces
		//	if (info->m_pDSNotify != NULL) {
		//		info->m_pDSNotify->Release();
		//		info->m_pDSNotify = NULL;
		//	}
		//	if (info->m_pDSBCapture != NULL) {
		//		info->m_pDSBCapture->Release();
		//		info->m_pDSBCapture = NULL;
		//	}
		//	if (info->m_pDSCapture != NULL) {
		//		info->m_pDSCapture->Release();
		//		info->m_pDSCapture = NULL;
		//	}

		//	// Release COM
		//	CoUninitialize();

		//	if (info->m_hNotificationEvent) {
		//		CloseHandle(info->m_hNotificationEvent);
		//		info->m_hNotificationEvent = NULL;
		//	}

		//	info->reset_AudioCapture();
		//	return 1;
		//}


		switch (dwResult)
		{
		case WAIT_OBJECT_0 + 0:
			// g_hNotificationEvents[0] is signaled

			// This means that DirectSound just finished playing 
			// a piece of the buffer, so we need to fill the circular 
			// buffer with new sound from the wav file

			if (info->RecordCapturedData(info) != S_OK)
			{
				printf("MICAudioCapture ERROR: RecordCapturedData fail!\n");
				bDone = TRUE;
				info->mIsClear = true;

				CreateThread(NULL, 0, onInputError, info, 0, NULL);
			}

			break;
		case WAIT_OBJECT_0 + 1:
			// do nothing...
			break;
		case STATUS_TIMEOUT:
			printf("MICAudioCapture ERROR: TIMEOUT!\n");
			bDone = TRUE;
			info->mIsClear = true;

			CreateThread(NULL, 0, onInputError, info, 0, NULL);

			break;
		}
	}

	// Stop the capture and read any data that 
	// was not caught by a notification
	if (NULL != info->m_pDSBCapture) {
		info->m_pDSBCapture->Stop();
	}

	// Release DirectSound interfaces
	if (info->m_pDSNotify != NULL) {
		info->m_pDSNotify->Release();
		info->m_pDSNotify = NULL;
	}
	if (info->m_pDSBCapture != NULL) {
		info->m_pDSBCapture->Release();
		info->m_pDSBCapture = NULL;
	}
	if (info->m_pDSCapture != NULL) {
		info->m_pDSCapture->Release();
		info->m_pDSCapture = NULL;
	}

	// Release COM
	CoUninitialize();

	printf("MICAudioCapture Capture thread finish!\n");

	return 0;
}
#else
#ifdef USE_WINAPI
void CALLBACK MICAudioCapture::waveInProc(HWAVEIN hwi, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	MICAudioCapture *info = (MICAudioCapture*)dwInstance;
	WAVEHDR *pHdr;
	PWAVEHDR wavedata = NULL;

	switch (uMsg)
	{
	case WIM_OPEN:
		info->m_iWaveInHdr = 0;

		break;
	case WIM_CLOSE:
		printf("MICAudioCapture Clear.\n");

		break;
	case WIM_DATA:
		//printf("WIM_DATA : %p, %p\n", dwParam1, dwParam2);

		if (info->isClear()) {
			//printf("MICAudioCapture Clear.\n");
			return;
		}

		wavedata = (PWAVEHDR)dwParam1;

		//printf("WIN_DATA wavedata : %p, %ld, %ld, %ld, %ld, %ld, %p, %p, %ld\n", wavedata, wavedata->dwBufferLength, wavedata->dwBytesRecorded, wavedata->dwFlags, wavedata->dwLoops, wavedata->dwUser, wavedata->lpData, wavedata->lpNext, wavedata->reserved);

		if (info->mpCaptureDoneCallback) {
			info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, (BYTE*)wavedata->lpData, BUFFER_SIZE, 0);
		}
		else
			printf("ERROR: onCaptureDone callback : NULL.\n");

		//info->file.Write((BYTE*)dwParam1, BUFFER_SIZE);

		MMRESULT mRes;

		mRes = waveInPrepareHeader(info->m_hWaveIn, &info->m_pWaveInHdr[info->m_iWaveInHdr], sizeof(WAVEHDR));
		if (mRes != MMSYSERR_NOERROR) {
			printf("waveInPrepareHeader : %d\n", mRes);
		}

		mRes = waveInAddBuffer(info->m_hWaveIn, &info->m_pWaveInHdr[info->m_iWaveInHdr], sizeof(WAVEHDR));
		if (mRes != MMSYSERR_NOERROR) {
			printf("waveInAddBuffer : %d\n", mRes);
		}

		info->m_iWaveInHdr++;
		if (info->m_iWaveInHdr == NUM_INPUT_BUFFERS)
			info->m_iWaveInHdr = 0;

		break;
	default:
		break;
	}
}
#else
void MICAudioCapture::onCapture(const BYTE * pbFrame, int cbFrame, void * pvParam, UINT64 u64TimeStamp) {
	MICAudioCapture *info = (MICAudioCapture*)pvParam;
	if (info == NULL)
		return;

	if (info->isClear()) {
		printf("XICaptureSDK Clear.\n");
		return;
	}

	if (info->mpCaptureDoneCallback) {
		info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, (BYTE*)pbFrame, cbFrame, u64TimeStamp);
	}
	else
		printf("ERROR: onCaptureDone callback : NULL.\n");
}
#endif
#endif