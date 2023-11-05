#include "Texture.h"

using namespace std;

const float Texture::SCALE_LIMIT = 7.0f;

// public functions //

Texture::Texture(int viewX, int viewY, int viewW, int viewH, int imageW, int imageH, int id) {
	mUseViewSize = true;
	mScaleMax = SCALE_LIMIT;
	mScaleThreshold = 1.0f;
	mAlpha = 1.0f;

	mID = id;

	setViewOffset(viewX, viewY);
	setViewSize(viewW, viewH);
	setImageSize(imageW, imageH);
}

float Texture::getAlpha() {
	return mAlpha;
}

int Texture::getViewX() {
	return mViewX;
}

int Texture::getViewY() {
	return mViewY;
}

int Texture::getViewW() {
	return mViewW;
}

int Texture::getViewH() {
	return mViewH;
}

int Texture::getImageW() {
	return mImageW;
}

int Texture::getImageH() {
	return mImageH;
}

float Texture::getCurrentX() {
	return mCurrentX;
}

float Texture::getCurrentY() {
	return mCurrentY;
}

float Texture::getCurrentScale() {
	return mCurrentScale;
}

void Texture::setAlpha(float alpha) {
	mAlpha = alpha;
}

void Texture::setViewX(int x) {
	mViewX = x;
}

void Texture::setViewY(int y) {
	mViewY = y;
}

void Texture::setViewW(int w) {
	mViewW = w;
}

void Texture::setViewH(int h) {
	mViewH = h;
}

void Texture::setImageW(int w) {
	mImageW = w;
}

void Texture::setImageH(int h) {
	mImageH = h;
}
void Texture::setCurrentX(float x) {
	mCurrentX = x;
}

void Texture::setCurrentY(float y) {
	mCurrentY = y;
}

void Texture::setCurrentScale(float scale) {
	mCurrentScale = scale;
}

float Texture::translate(float value, float size, float newSize, float ratio) {
	return (newSize / 2.0f + (value - size / 2.0f) / ratio);
}

void Texture::calculateStableBound(float scale)
{
	// The number of pixels between the center of the view
	// and the edge when the edge is aligned.
	mBoundLeft = mViewW / (2.0f * scale);
	mBoundRight = mImageW - mBoundLeft;
	mBoundTop = mViewH / (2.0f * scale);
	mBoundBottom = mImageH - mBoundTop;

	// If the scaled height is smaller than the view height,
	// force it to be in the center.
	if (floor(mImageH * scale) <= mViewH) {
		mBoundTop = mBoundBottom = mImageH / 2.0f;
	}

	// Same for width
	if (floor(mImageW * scale) <= mViewW) {
		mBoundLeft = mBoundRight = mImageW / 2.0f;
	}
}

void Texture::startScroll(float dx, float dy) {
	float x = mCurrentX + (dx / mCurrentScale);
	float y = mCurrentY + (dy / mCurrentScale);

	calculateStableBound(mCurrentScale);

	mCurrentX = x;// clamp(x, mBoundLeft, mBoundRight);
	mCurrentY = y;// clamp(y, mBoundTop, mBoundBottom);
}

void Texture::beginScale(float focusX, float focusY) {
	mFocusBitmapX = (mCurrentX + (focusX - mViewW / 2.0f) / mCurrentScale);
	mFocusBitmapY = (mCurrentY + (focusY - mViewH / 2.0f) / mCurrentScale);

	//First Time calculate in zoom_start_ ->is it need to keep value of mCurrentScale and use that?
}

void Texture::zoomIn(float tapX, float tapY, float targetScale)
{
	mWasMinScale = false;

	if (targetScale > mScaleMax)
		targetScale = mScaleMax;

	beginScale(tapX, tapY);
	// Convert the tap position to image coordinate
	float tempX = mFocusBitmapX - (tapX - mViewW / 2.0f) / targetScale;
	float tempY = mFocusBitmapY - (tapY - mViewH / 2.0f) / targetScale;

	calculateStableBound(targetScale);
	mCurrentX = clamp(tempX, mBoundLeft, mBoundRight);
	mCurrentY = clamp(tempY, mBoundTop, mBoundBottom);
	mCurrentScale = targetScale;
}

void Texture::setImageSize(int width, int height) {
	// If no image available, use view size.
	if (width == 0 || height == 0) {
		mUseViewSize = true;
		mImageW = mViewW;
		mImageH = mViewH;
		mCurrentX = mImageW / 2.0f;
		mCurrentY = mImageH / 2.0f;
		mCurrentScale = 1;
		mScaleMin = 1;

		//mViewer->setPosition(mCurrentX, mCurrentY, mCurrentScale);

		return;
	}

	mUseViewSize = false;

	float ratio = min((float)mImageW / width, (float)mImageH / height);

	if (ratio == 0)
		ratio = 1.0f;

	// See the comment above translate() for details.
	mCurrentX = translate(mCurrentX, mImageW, width, ratio);
	mCurrentY = translate(mCurrentY, mImageH, height, ratio);
	mCurrentScale = mCurrentScale * ratio;

	//mFromX = translate(mFromX, mImageW, width, ratio);
	//mFromY = translate(mFromY, mImageH, height, ratio);
	//mFromScale = mFromScale * ratio;

	//mToX = translate(mToX, mImageW, width, ratio);
	//mToY = translate(mToY, mImageH, height, ratio);
	//mToScale = mToScale * ratio;

	mFocusBitmapX = translate(mFocusBitmapX, mImageW, width, ratio);
	mFocusBitmapY = translate(mFocusBitmapY, mImageH, height, ratio);

	mImageW = width;
	mImageH = height;

	mScaleMin = getMinimalScale(mImageW, mImageH);

	mCurrentScale = clamp(mCurrentScale, mScaleMin, mScaleMax);

	//mViewer->setPosition(mCurrentX, mCurrentY, mCurrentScale);
}

void Texture::setViewOffset(int x, int y) {
	mViewX = x;
	mViewY = y;
}

void Texture::setViewSize(int viewW, int viewH) {
	bool needLayout = mViewW == 0 || mViewH == 0;

	mViewW = viewW;
	mViewH = viewH;

	if (mUseViewSize) {
		mImageW = viewW;
		mImageH = viewH;
		mCurrentX = mImageW / 2.0f;
		mCurrentY = mImageH / 2.0f;

		mCurrentScale = 1;
		mScaleMin = 1;
		//mViewer->setPosition(mCurrentX, mCurrentY, mCurrentScale);

		return;
	}

	if (mCurrentScale == mScaleMin)
		mWasMinScale = true;
	mScaleMin = getMinimalScale(mImageW, mImageH);

	if (needLayout || mCurrentScale < mScaleMin || mWasMinScale) {
		mCurrentX = mImageW / 2.0f;
		mCurrentY = mImageH / 2.0f;
		mCurrentScale = mScaleMin;
		mWasMinScale = true;
		//mViewer->setPosition(mCurrentX, mCurrentY, mCurrentScale);
	}
	else {
		if (mImageW * mCurrentScale > mViewW) {
			if (mCurrentX * mCurrentScale < mViewW / 2.0f) {
				mCurrentX = ((mViewW / 2.0f) / mCurrentScale);
			}
			if (mCurrentX * mCurrentScale
			>(mImageW * mCurrentScale) - (mViewW / 2.0f)) {
				mCurrentX = (((mImageW * mCurrentScale) - (mViewW / 2.0f))
					/ mCurrentScale);
			}
		}
		else {
			mCurrentX = mImageW / 2.0f;
		}
		if (mImageH * mCurrentScale > mViewH) {
			if (mCurrentY * mCurrentScale < mViewH / 2.0f) {
				mCurrentY = ((mViewH / 2.0f) / mCurrentScale);
			}
			if (mCurrentY * mCurrentScale
			>(mImageH * mCurrentScale) - (mViewH / 2.0f)) {
				mCurrentY = (((mImageH * mCurrentScale) - (mViewH / 2.0f))
					/ mCurrentScale);
			}
		}
		else {
			mCurrentY = mImageH / 2.0f;
		}

		//mViewer->setPosition(mCurrentX, mCurrentY, mCurrentScale);
	}
}

float Texture::getMinimalScale(int w, int h) {
	//SCALE_LIMIT
	return min(SCALE_LIMIT, min((float)mViewW / w, (float)mViewH / h));
}

void Texture::draw(int pbo) {
	float ratio = min((float)mViewW / (float)mImageW, (float)mViewH / (float)mImageH);
	float viewportRatioWidth = (float)mImageW / mViewW;
	float viewportRatioHeight = (float)mImageH / mViewH;

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
	glViewport(mViewX, mViewY, mViewW, mViewH);
	glPushMatrix();

	glScalef(viewportRatioWidth * ratio * mCurrentScale / mScaleMin, -viewportRatioHeight * ratio * mCurrentScale / mScaleMin, 1);
	glTranslatef(-2 * (mCurrentX - mImageW / 2.0f) / (float)mImageW, -2 * (mCurrentY - mImageH / 2.0f) / (float)mImageH, 0);

	glColor4f(1, 1, 1, mAlpha);

	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo);
	glBindTexture(GL_TEXTURE_2D, mID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mImageW, mImageH, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);

	//drawRect();
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(-1, 1);
	glTexCoord2f(0, 0); glVertex2f(-1, -1);
	glTexCoord2f(1, 0); glVertex2f(1, -1);
	glTexCoord2f(1, 1); glVertex2f(1, 1);
	glEnd();

	glDisable(GL_BLEND);

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

// private functions //

int Texture::clamp(int x, int min, int max) {
	if (x > max)
		return max;
	if (x < min)
		return min;
	return x;
}

float Texture::clamp(float x, float min, float max) {
	if (x > max)
		return max;
	if (x < min)
		return min;
	return x;
}