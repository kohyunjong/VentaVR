#include "RegionalWarping.h"

// public functions //

int RegionalWarping::getWidth() {
	return mImageWidth;
}

int RegionalWarping::getHeight() {
	return mImageHeight;
}

int RegionalWarping::getWarpingWidth() {
	return mWarpingWidth;
}

int RegionalWarping::getWarpingHeight() {
	return mWarpingHeight;
}

int RegionalWarping::getMode() {
	return mMode;
}

int RegionalWarping::getSize() {
	return mSize;
}

int RegionalWarping::getLength() {
	return mLength;
}

int RegionalWarping::getAutoMode() {
	return mAutoMode;
}

void RegionalWarping::setMode(int mode) {
	mMode = mode;
}

void RegionalWarping::setSize(int size) {
	mSize = size;
}

void RegionalWarping::setLength(int length) {
	mLength = length;
}

void RegionalWarping::setAutoMode(int autoMode) {
	mAutoMode = autoMode;
}