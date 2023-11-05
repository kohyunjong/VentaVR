#include "MWAudioCapture.h"

// public functions //

MWAudioCapture::MWAudioCapture() {

}

MWAudioCapture* MWAudioCapture::create_AudioCapture(int iCam) {
	MWAudioCapture* instance = new MWAudioCapture();

	instance->init_AudioCapture(iCam);

	return instance;
}

BOOL MWAudioCapture::init_AudioCapture(int iCam) {
	mIsClear = false;
	mhChannel = NULL;
	mpCaptureDoneCallback = NULL;

	mCameraIndex = iCam;
	mTime = QClock();

	do {
		if (!MWCaptureInitInstance()) {
			printf("MWAudioCapture ERROR: MWCaptureInitInstance return fail!\n");
			break;
		}

		MWCAP_CHANNEL_INFO videoInfo = { 0 };
		TCHAR szDevicePath[MAX_PATH];
		if (MW_SUCCEEDED != MWGetDevicePath(iCam, szDevicePath)) {
			printf("MWAudioCapture ERROR: Can't get Device Path!\n");
			break;
		}

		mhChannel = MWOpenChannelByPath(szDevicePath);
		if (mhChannel == NULL) {
			printf("MWAudioCapture ERROR: Open channel %s:%d error!\n", szDevicePath, iCam);
			break;
		}

		if (MW_SUCCEEDED != MWGetChannelInfo(mhChannel, &videoInfo)) {
			printf("MWAudioCapture ERROR: Can't get channel info!\n");
			break;
		}

		mhCaptureThread = CreateThread(NULL, 0, run, this, 0, NULL);
		if (mhCaptureThread == NULL) {
			printf("MWAudioCapture ERROR: Can't create video capture thread!\n");
			break;
		}

		printf("Initialize MWCapture SDK for MWAudioCapture\n");
		return MW_SUCCEEDED;
	} while (false);

	return MW_FAILED;
}

BOOL MWAudioCapture::destroy_AudioCapture(MWAudioCapture* instance) {
	if (instance) {
		instance->mIsClear = true;

		DWORD result = WaitForSingleObject(instance->mhCaptureThread, 3000);
		if (result == WAIT_OBJECT_0)
			printf("Destroy audio capture thread\n");
		else if (result == WAIT_TIMEOUT)
			printf("MWAudioCapture ERROR: Can't destroy audio capture thread!\n");

		delete instance;

		printf("Destroy MWAudioCapture\n");
		return MW_SUCCEEDED;
	}

	printf("MWAudioCapture ERROR: MWAudioCapture instance : NULL!\n");
	return MW_FAILED;
}

void MWAudioCapture::setHChannel(HCHANNEL hChannel) {
	mhChannel = hChannel;
}

void MWAudioCapture::setHeaderBuffer(unsigned char* headerBuffer) {
	mHeaderBuffer = headerBuffer;
}

void MWAudioCapture::setHeaderBufferSize(int headerBufferSize) {
	mHeaderBufferSize = headerBufferSize;
}

void MWAudioCapture::setwFormatTag(WORD wFormatTag) {
	mwFormatTag = wFormatTag;
}

void MWAudioCapture::setnChannels(WORD nChannels) {
	mnChannels = nChannels;
}

void MWAudioCapture::setnSamplesPerSec(DWORD nSamplesPerSec) {
	mnSamplesPerSec = nSamplesPerSec;
}

void MWAudioCapture::setnAvgBytesPerSec(DWORD nAvgBytesPerSec) {
	mnAvgBytesPerSec = nAvgBytesPerSec;
}

void MWAudioCapture::setnBlockAlign(WORD nBlockAlign) {
	mnBlockAlign = nBlockAlign;
}

void MWAudioCapture::setwBitsPerSample(WORD wBitsPerSample) {
	mwBitsPerSample = wBitsPerSample;
}

int MWAudioCapture::getCameraIndex() {
	return mCameraIndex;
}

HCHANNEL MWAudioCapture::getHChannel() {
	return mhChannel;
}

unsigned char* MWAudioCapture::getHeaderBuffer() {
	return mHeaderBuffer;
}

int MWAudioCapture::getHeaderBufferSize() {
	return mHeaderBufferSize;
}

WORD MWAudioCapture::getwFormatTag() {
	return mwFormatTag;
}

WORD MWAudioCapture::getnChannels() {
	return mnChannels;
}

DWORD MWAudioCapture::getnSamplesPerSec() {
	return mnSamplesPerSec;
}

DWORD MWAudioCapture::getnAvgBytesPerSec() {
	return mnAvgBytesPerSec;
}

WORD MWAudioCapture::getnBlockAlign() {
	return mnBlockAlign;
}

WORD MWAudioCapture::getwBitsPerSample() {
	return mwBitsPerSample;
}

void MWAudioCapture::setCallback_onCaptureDone(callback_onAudioCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

bool MWAudioCapture::isClear() {
	return mIsClear;
}

DWORD WINAPI MWAudioCapture::run(LPVOID handle) {
	MWAudioCapture *info = (MWAudioCapture *)handle;

	HCHANNEL hChannel = info->getHChannel();

	MWCAP_AUDIO_SIGNAL_STATUS audioSignalStatus;
	MWGetAudioSignalStatus(hChannel, &audioSignalStatus);

	HANDLE hCaptureEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	MW_RESULT xr;

	do {
		xr = MWStartAudioCapture(hChannel);
		if (xr != MW_SUCCEEDED) {
			printf("ERROR: Open Audio Capture error!\n");
			break;
		}

		CWaveFile file;
		file.Init(_T("test.wav"), audioSignalStatus.dwSampleRate);

		info->setHeaderBuffer(file.getHeader());
		info->setHeaderBufferSize(file.getHeaderSize());

		info->setwFormatTag(file.m_wfHeader.pcmFormat.wf.wFormatTag);
		info->setnChannels(file.m_wfHeader.pcmFormat.wf.nChannels);
		info->setnSamplesPerSec(file.m_wfHeader.pcmFormat.wf.nSamplesPerSec);
		info->setnAvgBytesPerSec(file.m_wfHeader.pcmFormat.wf.nAvgBytesPerSec);
		info->setnBlockAlign(file.m_wfHeader.pcmFormat.wf.nBlockAlign);
		info->setwBitsPerSample(file.m_wfHeader.pcmFormat.wBitsPerSample);

		int n = 0;
		while (true) {
			if (info->isClear()) {
				printf("MWAudioCapture Clear.\n");
				break;
			}

			while (abs(info->mTime - QClock()) < 4) // 4 ms
			{

			}

			info->mTime = QClock();

			LONGLONG timeStamp = 0LL;
			MWGetDeviceTime(info->mhChannel, &timeStamp);

			short asAudioSamples[MWCAP_AUDIO_SAMPLES_PER_FRAME * 2];

			do {
				MWCAP_AUDIO_CAPTURE_FRAME audioFrame;
				xr = MWCaptureAudioFrame(hChannel, &audioFrame);

				if (xr == MW_SUCCEEDED) {
					for (int i = 0; i < MWCAP_AUDIO_SAMPLES_PER_FRAME; i++) {
						short sLeft = (short)(audioFrame.adwSamples[i * 8] >> 16);
						short sRight = (short)(audioFrame.adwSamples[i * 8 + 4] >> 16);

						asAudioSamples[i * 2] = sLeft;
						asAudioSamples[i * 2 + 1] = sRight;
					}

					if (info->mpCaptureDoneCallback) {
						info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, (BYTE*)asAudioSamples, sizeof(asAudioSamples), timeStamp / 10000);
					}
					else
						printf("ERROR: onCaptureDone callback : NULL.\n");
				}
			} while (xr == MW_SUCCEEDED);
		}
		file.Exit();

		MWStopAudioCapture(hChannel);

		if (hChannel != NULL)
			MWCloseChannel(hChannel);

		printf("MWAudioCapture Thread End.\n");

	} while (false);

	return 0;
}