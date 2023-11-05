#include "VideoCapture.h"

// public functions //

VideoCapture::VideoCapture() {

}

int VideoCapture::getCaptureType() {
	return mCaptureType;
}

void VideoCapture::setCaptureType(int captureType) {
	mCaptureType = captureType;
}