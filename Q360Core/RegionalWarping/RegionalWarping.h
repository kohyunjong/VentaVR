#ifndef __REGIONAL_WARPING_QURAM_H__
#define __REGIONAL_WARPING_QURAM_H__

#include <vector>
#include <opencv2/opencv.hpp>

#define DIST_STATIC

#define WARPING_SIZE_FACTOR 2

#define REIONGAL_WARPING_OK 1
#define REIONGAL_WARPING_FAIL 0
#define REIONGAL_WARPING_ERROR_INVALID_POINT -1
#define REIONGAL_WARPING_ERROR_INVALID_AREA -2
#define REIONGAL_WARPING_ERROR_BIG_AREA -3
#define REIONGAL_WARPING_ERROR_LONG_DISTANCE -4

class RegionalWarping {
public:
	int getWidth();
	int getHeight();

	int getWarpingWidth();
	int getWarpingHeight();

	int getMode();
	int getSize();
	int getLength();
	int getAutoMode();

	void setMode(int mode);
	void setSize(int size);
	void setLength(int length);
	void setAutoMode(int autoMode);

	virtual int warping() = 0;
	int merge(int index, cv::Mat result_mapx, cv::Mat result_mapy, int x, int y, int w, int h);

protected:
	int mImageWidth;
	int mImageHeight;

	int mWarpingWidth;
	int mWarpingHeight;

	int mMode;
	int mSize;
	int mLength;
	int mAutoMode;
};

#endif // __REGIONAL_WARPING_QURAM_H__