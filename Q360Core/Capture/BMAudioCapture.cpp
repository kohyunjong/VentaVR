#include "BMAudioCapture.h"

// public functions //

BMAudioCapture::BMAudioCapture() {

}

BMAudioCapture* BMAudioCapture::create_AudioCapture(BMVideoCapture* videoCapture) {
	BMAudioCapture* instance = new BMAudioCapture();

	instance->init_AudioCapture(videoCapture);

	return instance;
}

BOOL BMAudioCapture::init_AudioCapture(BMVideoCapture* videoCapture) {
	mpCaptureDoneCallback = NULL;

	do {
		HRESULT result;

		if (videoCapture == NULL)
		{
			fprintf(stderr, "BMAudioCapture : Video Capture Instance is NULL\n");
			break;
		}

		deckLinkInput = videoCapture->getIDeckLinkInput();
		if (deckLinkInput == NULL)
		{
			fprintf(stderr, "BMAudioCapture : IDeckLinkInput interface is NULL\n");
			break;
		}

		notificationCallback = videoCapture->getNotificationCallback();
		if (notificationCallback == NULL)
		{
			fprintf(stderr, "BMAudioCapture : notification callback is NULL\n");
			break;
		}

		notificationCallback->setCallback_onAudioCapture(onCapture, this);

		// Set the callback object to the DeckLink device's input interface
		result = deckLinkInput->SetCallback(notificationCallback);
		if (result != S_OK)
		{
			fprintf(stderr, "BMAudioCapture : Could not set callback - result = %08x\n", result);
			break;
		}

		// Pause video capture
		deckLinkInput->PauseStreams();

		// Enable audio input
		result = deckLinkInput->EnableAudioInput(bmdAudioSampleRate48kHz, bmdAudioSampleType16bitInteger, 2);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not enable audio input - result = %08x\n", result);
			break;
		}

		// Flush any queued video frames
		deckLinkInput->FlushStreams();

		// Start video capture
		deckLinkInput->StartStreams();
		
		file.Init(_T("test.wav"), bmdAudioSampleRate48kHz);

		setHeaderBuffer(file.getHeader());
		setHeaderBufferSize(file.getHeaderSize());

		setwFormatTag(file.m_wfHeader.pcmFormat.wf.wFormatTag);
		setnChannels(file.m_wfHeader.pcmFormat.wf.nChannels);
		setnSamplesPerSec(file.m_wfHeader.pcmFormat.wf.nSamplesPerSec);
		setnAvgBytesPerSec(file.m_wfHeader.pcmFormat.wf.nAvgBytesPerSec);
		setnBlockAlign(file.m_wfHeader.pcmFormat.wf.nBlockAlign);
		setwBitsPerSample(file.m_wfHeader.pcmFormat.wBitsPerSample);

		printf("Initialize BMCapture SDK for BMAudioCapture\n");
		return S_OK;
	} while (false);

	if (deckLinkInput != NULL)
		deckLinkInput = NULL;

	if (notificationCallback)
		notificationCallback = NULL;

	return S_FALSE;
}

BOOL BMAudioCapture::destroy_AudioCapture(BMAudioCapture* instance) {
	delete instance;
	return S_OK;
}

void BMAudioCapture::setHeaderBuffer(unsigned char* headerBuffer) {
	mHeaderBuffer = headerBuffer;
}

void BMAudioCapture::setHeaderBufferSize(int headerBufferSize) {
	mHeaderBufferSize = headerBufferSize;
}

void BMAudioCapture::setwFormatTag(WORD wFormatTag) {
	mwFormatTag = wFormatTag;
}

void BMAudioCapture::setnChannels(WORD nChannels) {
	mnChannels = nChannels;
}

void BMAudioCapture::setnSamplesPerSec(DWORD nSamplesPerSec) {
	mnSamplesPerSec = nSamplesPerSec;
}

void BMAudioCapture::setnAvgBytesPerSec(DWORD nAvgBytesPerSec) {
	mnAvgBytesPerSec = nAvgBytesPerSec;
}

void BMAudioCapture::setnBlockAlign(WORD nBlockAlign) {
	mnBlockAlign = nBlockAlign;
}

void BMAudioCapture::setwBitsPerSample(WORD wBitsPerSample) {
	mwBitsPerSample = wBitsPerSample;
}

int BMAudioCapture::getCameraIndex() {
	return mCameraIndex;
}

unsigned char* BMAudioCapture::getHeaderBuffer() {
	return mHeaderBuffer;
}

int BMAudioCapture::getHeaderBufferSize() {
	return mHeaderBufferSize;
}

WORD BMAudioCapture::getwFormatTag() {
	return mwFormatTag;
}

WORD BMAudioCapture::getnChannels() {
	return mnChannels;
}

DWORD BMAudioCapture::getnSamplesPerSec() {
	return mnSamplesPerSec;
}

DWORD BMAudioCapture::getnAvgBytesPerSec() {
	return mnAvgBytesPerSec;
}

WORD BMAudioCapture::getnBlockAlign() {
	return mnBlockAlign;
}

WORD BMAudioCapture::getwBitsPerSample() {
	return mwBitsPerSample;
}

void BMAudioCapture::setCallback_onCaptureDone(callback_onAudioCaptureDone cb, void* data) {
	mpCaptureDoneCallback = cb;
	mpCaptureDoneCallbackData = data;
}

// private functions //

void BMAudioCapture::onCapture(void* handle, IDeckLinkAudioInputPacket* audioPacket) {
	BMAudioCapture *info = (BMAudioCapture*)handle;
	if (info == NULL)
		return;

	HRESULT result;
	BMDTimeValue packetTime;
	audioPacket->GetPacketTime(&packetTime, 1000);

	long packetSize;
	packetSize = audioPacket->GetSampleFrameCount();

	void* buffer;
	result = audioPacket->GetBytes(&buffer);

	if (info->mpCaptureDoneCallback != NULL && info->mpCaptureDoneCallbackData != NULL) {
		info->mpCaptureDoneCallback(info, info->mpCaptureDoneCallbackData, (BYTE*)buffer, packetSize * 4, packetTime);
	}
	else
		printf("ERROR: onCaptureDone callback : NULL.\n");
}

void BMAudioCapture::onChange(void* handle, IDeckLinkDisplayMode *newDisplayMode, BMDPixelFormat pixelFormat) {
	BMAudioCapture *info = (BMAudioCapture *)handle;
}