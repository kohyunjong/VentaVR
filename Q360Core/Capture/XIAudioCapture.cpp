#include "XIAudioCapture.h"

// public functions //

XIAudioCapture::XIAudioCapture() {

}

XIAudioCapture* XIAudioCapture::create_AudioCapture(int iCam) {
	XIAudioCapture* instance = new XIAudioCapture();

	instance->init_AudioCapture(iCam);

	return instance;
}

BOOL XIAudioCapture::init_AudioCapture(int iCam) {
	mIsClear = false;
	mhChannel = NULL;
	mpCaptureDoneCallback = NULL;

	mCameraIndex = iCam;

	do {
		if (!XIS_Initialize()) {
			printf("XIAudioCapture ERROR: XICaptureInitInstance return fail!\n");
			break;
		}

		int cAudioCaptures = XIS_GetAudioCaptureCount();
		if (cAudioCaptures <= 0) {
			printf("XIAudioCapture ERROR: Can't find capture card!\n");
			break;
		}

		AUDIO_CAPTURE_INFO_EX audioInfo;
		int iUSBDevice = 0;
		bool isFind = false;
		for (int i = 0; i < cAudioCaptures; i++) {
			if (!XIS_GetAudioCaptureInfoEx(i, &audioInfo)) {
				printf("XIAudioCapture ERROR: Can't get channel info!\n");
				break;
			}

			if (_tcsstr(audioInfo.szID, _T("usb")) || _tcsstr(audioInfo.szName, _T("USB"))) {
			//if (_tcsstr(audioInfo.szName, _T("High Definition Audio"))) {
				if (iUSBDevice == mCameraIndex) {
					isFind = true;
					break;
				}
				else
					iUSBDevice++;
			}
		}
		if (isFind == false) {
			printf("XIAudioCapture ERROR: Can't find USB device!\n");
			break;
		}

		mhChannel = XIS_OpenAudioCaptureEx(audioInfo.szID);
		if (mhChannel == NULL) {
			printf("XIAudioCapture ERROR: Open channel %s:%d error!\n", audioInfo.szID, mCameraIndex);
			break;
		}

		int cSamplesPerFrame = AUDIO_SAMPLESPERSEC / 250;
		if (!XIS_SetAudioCaptureFormat(mhChannel, AUDIO_SAMPLESPERSEC, 2, cSamplesPerFrame, 0)) {
			printf("XIAudioCapture ERROR: XIS_SetAudioCaptureFormat return fail!\n");
			break;
		}

		if (!XIS_SetAudioCaptureCallbackEx(mhChannel, onCapture, this)) {
			printf("XIAudioCapture ERROR: XIS_SetAudioCaptureCallbackEx return fail!\n");
			break;
		}

		if (!XIS_StartAudioCapture(mhChannel)) {
			printf("XIAudioCapture ERROR: Start audio capture fail!\n");
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

		printf("Initialize XICapture SDK for XIAudioCapture\n");
		return TRUE;
	} while (false);

	return FALSE;
}

BOOL XIAudioCapture::destroy_AudioCapture(XIAudioCapture* instance) {
	if (instance) {
		instance->mIsClear = true;

		if (instance->mhChannel != NULL) {
			XIS_StopAudioCapture(instance->mhChannel);
			XIS_CloseAudioCapture(instance->mhChannel);
			instance->mhChannel = NULL;
		}

		delete instance;

		printf("Destroy XICapture SDK\n");
		return TRUE;
	}

	printf("ERROR: XIAudioCapture instance : NULL!\n");
	return FALSE;
}

void XIAudioCapture::setHChannel(HANDLE hChannel) {
	mhChannel = hChannel;
}

void XIAudioCapture::setHeaderBuffer(unsigned char* headerBuffer) {
	mHeaderBuffer = headerBuffer;
}

void XIAudioCapture::setHeaderBufferSize(int headerBufferSize) {
	mHeaderBufferSize = headerBufferSize;
}

void XIAudioCapture::setwFormatTag(WORD wFormatTag) {
	mwFormatTag = wFormatTag;
}

void XIAudioCapture::setnChannels(WORD nChannels) {
	mnChannels = nChannels;
}

void XIAudioCapture::setnSamplesPerSec(DWORD nSamplesPerSec) {
	mnSamplesPerSec = nSamplesPerSec;
}

void XIAudioCapture::setnAvgBytesPerSec(DWORD nAvgBytesPerSec) {
	mnAvgBytesPerSec = nAvgBytesPerSec;
}

void XIAudioCapture::setnBlockAlign(WORD nBlockAlign) {
	mnBlockAlign = nBlockAlign;
}

void XIAudioCapture::setwBitsPerSample(WORD wBitsPerSample) {
	mwBitsPerSample = wBitsPerSample;
}

int XIAudioCapture::getCameraIndex() {
	return mCameraIndex;
}

HANDLE XIAudioCapture::getHChannel() {
	return mhChannel;
}

unsigned char* XIAudioCapture::getHeaderBuffer() {
	return mHeaderBuffer;
}

int XIAudioCapture::getHeaderBufferSize() {
	return mHeaderBufferSize;
}

WORD XIAudioCapture::getwFormatTag() {
	return mwFormatTag;
}

WORD XIAudioCapture::getnChannels() {
	return mnChannels;
}

DWORD XIAudioCapture::getnSamplesPerSec() {
	return mnSamplesPerSec;
}

DWORD XIAudioCapture::getnAvgBytesPerSec() {
	return mnAvgBytesPerSec;
}

WORD XIAudioCapture::getnBlockAlign() {
	return mnBlockAlign;
}

WORD XIAudioCapture::getwBitsPerSample() {
	return mwBitsPerSample;
}

void XIAudioCapture::setCallback_onCaptureDone(callback_onAudioCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

// private functions //

bool XIAudioCapture::isClear() {
	return mIsClear;
}

void XIAudioCapture::onCapture(const BYTE * pbFrame, int cbFrame, void * pvParam, UINT64 u64TimeStamp) {
	XIAudioCapture *info = (XIAudioCapture*)pvParam;
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