#ifndef __EXPOSURE_COMPENSATION_QURAM_H__
#define __EXPOSURE_COMPENSATION_QURAM_H__

#include <stdio.h>
#include <math.h>

#define EXPOSURE_ANIMATION_DURATION	1000
#define NUM_EXPOSURE_BUFFER			10

struct ExposureData {
	float exposureDiff[4];
	unsigned char exposureCurve[3][256];
	long long time_stamp = 0;
};

struct ExposureCircularQueue {
	ExposureData* ExposureDataList;
	int head, tail, max_index;
	ExposureCircularQueue(int k);
	void enqueue(ExposureData data);
	ExposureData dequeue();
	int size();
	bool isEmpty();
	bool isFull();
};

struct ExposureCompensation
{
	ExposureCircularQueue mExposureQueue = ExposureCircularQueue(NUM_EXPOSURE_BUFFER);

	float exposureDiffR = 0.f, exposureDiffG = 0.f, exposureDiffB = 0.f, exposureDiffY = 0.f;
	float manualR = 0.f, manualG = 0.f, manualB = 0.f;

	void setExposureData(double diffR, double diffG, double diffB, unsigned char* tableR, unsigned char* tableG, unsigned char* tableB, long long time_stamp);
	void setExposureCurveRGB(unsigned char* tableR, unsigned char* tableG, unsigned char* tableB, long long time_stamp);
	void setExposureDiffRGB(double diffR, double diffG, double diffB, long long time_stamp);
	void setExposureDiffY(double diffY, long long time_stamp);
	void setManualRGB(int R, int G, int B);
	ExposureData getExposureDiffForAnimation(long long time_stamp);
	ExposureData getExposureDataForAnimation(long long time_stamp);
};

#endif