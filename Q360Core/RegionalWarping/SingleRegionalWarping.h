#ifndef __SINGLE_REGIONAL_WARPING_QURAM_H__
#define __SINGLE_REGIONAL_WARPING_QURAM_H__

#include "RegionalWarping.h"

#define SCALE_FACTOR 2

class SingleRegionalWarping : public RegionalWarping {
public:
	SingleRegionalWarping();

	static SingleRegionalWarping* create_SingleRegionalWarping(int w, int h);
	static int destroy_SingleRegionalWarping(SingleRegionalWarping* instance);

	int init_SingleRegionalWarping(int w, int h);

	void* getMapBuffer();
	void setMapBuffer(void* buffer);

	void addSrcPoint(float srcX, float srcY);
	void addDstPoint(float dstX, float dstY);
	void resetPoint();

	int warping();

	int merge(cv::Mat result_mapx, cv::Mat result_mapy, int x, int y, int w, int h);

	void* getMapZeros();

private:
	void* mMapBuffer;
	void* mMapZeros;

	std::vector<float> mSrcX;
	std::vector<float> mSrcY;
	std::vector<float> mDstX;
	std::vector<float> mDstY;
};

#endif // __SINGLE_REGIONAL_WARPING_QURAM_H__