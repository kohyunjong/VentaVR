#ifndef __MULTI_REGIONAL_WARPING_QURAM_H__
#define __MULTI_REGIONAL_WARPING_QURAM_H__

#include "RegionalWarping.h"

#define SCALE_FACTOR 2

using std::vector;

struct MultiRegionalWarpingInfo {
	MultiRegionalWarpingInfo(std::vector<float> srcX, std::vector<float> srcY, std::vector<float> dstX, std::vector<float> dstY, int index) {
		mSrcX = srcX;
		mSrcY = srcY;
		mDstX = dstX;
		mDstY = dstY;
		mIndex = index;
	}

	// input
	std::vector<float> mSrcX, mSrcY, mDstX, mDstY;
	int mIndex;

	// output
	cv::Mat mResultX;
	cv::Mat mResultY;
	int x, y, w, h;
};

class MultiRegionalWarping : public RegionalWarping {
public:
	MultiRegionalWarping();

	static MultiRegionalWarping* create_MultiRegionalWarping(int w, int h, int n);
	static int destroy_MultiRegionalWarping(MultiRegionalWarping* instance);

	int init_MultiRegionalWarping(int w, int h, int n);

	void* getMapBuffer(int index);

	void addPoints(std::vector<float> srcX, std::vector<float> srcY, std::vector<float> dstX, std::vector<float> dstY, int index);
	void resetPoint();

	int warping();

private:
	int mNumCamera;

	void** mMapBuffers;

	std::vector<MultiRegionalWarpingInfo*> mInfos;
};

#endif // __MULTI_REGIONAL_WARPING_QURAM_H__