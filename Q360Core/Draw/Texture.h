#ifndef __TEXTURE_QURAM_H__
#define __TEXTURE_QURAM_H__

#include <stdio.h>
#include <algorithm>

#include <GL/Glew.h>
#include <GL/wGlew.h>

class Texture {
public:
	static const float SCALE_LIMIT;

	Texture(int viewX, int viewY, int viewW, int viewH, int imageW, int imageH, int id);

	float getAlpha();
	int getViewX();
	int getViewY();
	int getViewW();
	int getViewH();
	int getImageW();
	int getImageH();
	float getCurrentX();
	float getCurrentY();
	float getCurrentScale();

	void setAlpha(float alpha);
	void setViewX(int x);
	void setViewY(int y);
	void setViewW(int w);
	void setViewH(int h);
	void setImageW(int w);
	void setImageH(int h);
	void setCurrentX(float x);
	void setCurrentY(float y);
	void setCurrentScale(float scale);

	static float translate(float value, float size, float newSize, float ratio);

	void calculateStableBound(float scale);
	void startScroll(float dx, float dy);
	void beginScale(float focusX, float focusY);
	void zoomIn(float tapX, float tapY, float targetScale);

	void setImageSize(int width, int height);
	void setViewOffset(int x, int y);
	void setViewSize(int viewW, int viewH);
	float getMinimalScale(int w, int h);

	void draw(int pbo);

private:
	int clamp(int x, int min, int max);
	float clamp(float x, float min, float max);

private:
	int mID;
	float mAlpha;

	float mScaleMin;
	float mScaleMax;
	float mScaleThreshold;

	int mViewX, mViewY;
	int mViewW, mViewH;
	int mImageW, mImageH;

	float mCurrentX, mCurrentY;
	float mCurrentScale;
	bool mUseViewSize;
	float mFocusBitmapX;
	float mFocusBitmapY;
	bool mWasMinScale;

	float mBoundLeft, mBoundRight, mBoundTop, mBoundBottom;
};

#endif // __TEXTURE_QURAM_H__