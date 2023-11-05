#include "AudioCapture.h"

// public functions //

AudioCapture::AudioCapture() {
	mpInputErrorCallback = NULL;
	mpInputErrorCallbackData = NULL;
}
int AudioCapture::getCaptureType() {
	return mCaptureType;
}

void AudioCapture::setCaptureType(int captureType) {
	mCaptureType = captureType;
}

void AudioCapture::setCallback_onInputError(callback_onInputError cb, void* data) {
	mpInputErrorCallback = cb;
	mpInputErrorCallbackData = data;
}