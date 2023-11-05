
#include "ExposureCompensation.h"


ExposureCircularQueue::ExposureCircularQueue(int k) :head(0), tail(0), max_index(k) {
	ExposureDataList = new ExposureData[max_index];
	for (int i = 0; i < max_index; i++)
	{
		ExposureDataList[i].exposureDiff[0] = 0.0f;
		ExposureDataList[i].exposureDiff[1] = 0.0f;
		ExposureDataList[i].exposureDiff[2] = 0.0f;
		ExposureDataList[i].exposureDiff[3] = 0.0f;

		for (int j = 0; j < 256; j++)
		{
			ExposureDataList[i].exposureCurve[0][j] = j;
			ExposureDataList[i].exposureCurve[1][j] = j;
			ExposureDataList[i].exposureCurve[2][j] = j;
		}


		ExposureDataList[i].time_stamp = 0;
	}
}

void ExposureCircularQueue::enqueue(ExposureData data) {

	ExposureDataList[tail].exposureDiff[0] = data.exposureDiff[0];
	ExposureDataList[tail].exposureDiff[1] = data.exposureDiff[1];
	ExposureDataList[tail].exposureDiff[2] = data.exposureDiff[2];
	ExposureDataList[tail].exposureDiff[3] = data.exposureDiff[3];

	for (int j = 0; j < 256; j++)
	{
		ExposureDataList[tail].exposureCurve[0][j] = data.exposureCurve[0][j];
		ExposureDataList[tail].exposureCurve[1][j] = data.exposureCurve[1][j];
		ExposureDataList[tail].exposureCurve[2][j] = data.exposureCurve[2][j];
	}

	ExposureDataList[tail].time_stamp = data.time_stamp;
	tail = (tail + 1) % max_index;

}
ExposureData ExposureCircularQueue::dequeue(){
	ExposureData temp;
	temp = ExposureDataList[head];
	head = (head + 1) % max_index;
	return temp;
}
int ExposureCircularQueue::size(){
	return (tail - head);
}
bool ExposureCircularQueue::isEmpty(){
	if (abs(head == tail)){
		return true;
	}
	else{
		return false;
	}
}
bool ExposureCircularQueue::isFull(){
	if (head == (tail + 1) % max_index){
		return true;
	}
	else{
		return false;
	}
}


void ExposureCompensation::setExposureData(double diffR, double diffG, double diffB, unsigned char* tableR, unsigned char* tableG, unsigned char* tableB, long long time_stamp) {
	exposureDiffR = (float)diffR / 255.f;
	exposureDiffG = (float)diffG / 255.f;
	exposureDiffB = (float)diffB / 255.f;

	ExposureData exposureData;
	exposureData.exposureDiff[0] = exposureDiffR;
	exposureData.exposureDiff[1] = exposureDiffG;
	exposureData.exposureDiff[2] = exposureDiffB;
	exposureData.exposureDiff[3] = 0;

	for (int i = 0; i < 256; i++)
	{
		exposureData.exposureCurve[0][i] = tableR[i];
		exposureData.exposureCurve[1][i] = tableG[i];
		exposureData.exposureCurve[2][i] = tableB[i];
	}

	exposureData.time_stamp = time_stamp;

	mExposureQueue.enqueue(exposureData);
}
void ExposureCompensation::setExposureCurveRGB(unsigned char* tableR, unsigned char* tableG, unsigned char* tableB, long long time_stamp) {

	ExposureData exposureData;

	for (int i = 0; i < 256; i++)
	{
		exposureData.exposureCurve[0][i] = tableR[i];
		exposureData.exposureCurve[1][i] = tableG[i];
		exposureData.exposureCurve[2][i] = tableB[i];
	}

	exposureData.time_stamp = time_stamp;

	mExposureQueue.enqueue(exposureData);
}
void ExposureCompensation::setExposureDiffRGB(double diffR, double diffG, double diffB, long long time_stamp) {
	exposureDiffR = (float)diffR / 255.f;
	exposureDiffG = (float)diffG / 255.f;
	exposureDiffB = (float)diffB / 255.f;

	ExposureData exposureData;
	exposureData.exposureDiff[0] = exposureDiffR;
	exposureData.exposureDiff[1] = exposureDiffG;
	exposureData.exposureDiff[2] = exposureDiffB;
	exposureData.exposureDiff[3] = 0;

	exposureData.time_stamp = time_stamp;

	mExposureQueue.enqueue(exposureData);
}
void ExposureCompensation::setExposureDiffY(double diffY, long long time_stamp) {
	exposureDiffY = diffY / 255.f;

	ExposureData exposureData;
	exposureData.exposureDiff[0] = 0;
	exposureData.exposureDiff[1] = 0;
	exposureData.exposureDiff[2] = 0;
	exposureData.exposureDiff[3] = exposureDiffY;

	exposureData.time_stamp = time_stamp;

	mExposureQueue.enqueue(exposureData);
}

void ExposureCompensation::setManualRGB(int R, int G, int B)
{
	manualR = R / 255.f;
	manualG = G / 255.f;
	manualB = B / 255.f;
}

ExposureData ExposureCompensation::getExposureDiffForAnimation(long long time_stamp)
{
	long long tot_time = 0, frame_time = 0, pre_time = 0, cur_time = 0, next_time = 0;
	int cur_index, pre_index;
	float frame_ratio = 0.f;
	ExposureData ret;
	//if (!mExposureQueue.isEmpty()) 
	{
		for (int i = 0; i < 9; i++) {
			int idx1 = mExposureQueue.tail - i - 1;
			if (idx1 < 0)
				idx1 += NUM_EXPOSURE_BUFFER;
			int idx2 = mExposureQueue.tail - i - 2;
			if (idx2 < 0)
				idx2 += NUM_EXPOSURE_BUFFER;

			cur_time = mExposureQueue.ExposureDataList[idx1].time_stamp;
			pre_time = mExposureQueue.ExposureDataList[idx2].time_stamp;

			if (time_stamp >= cur_time) {
				float duration = (float)(cur_time - pre_time);
				if (duration > EXPOSURE_ANIMATION_DURATION)
					duration = EXPOSURE_ANIMATION_DURATION;

				if (duration == 0)
					continue;
				frame_ratio = (time_stamp - cur_time) / duration;
				if (frame_ratio > 1)
					frame_ratio = 1.f;
				else if (frame_ratio < 0)
					frame_ratio = 0.f;

				ret.exposureDiff[0] = mExposureQueue.ExposureDataList[idx2].exposureDiff[0] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[0] - mExposureQueue.ExposureDataList[idx2].exposureDiff[0]) * frame_ratio;
				ret.exposureDiff[1] = mExposureQueue.ExposureDataList[idx2].exposureDiff[1] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[1] - mExposureQueue.ExposureDataList[idx2].exposureDiff[1]) * frame_ratio;
				ret.exposureDiff[2] = mExposureQueue.ExposureDataList[idx2].exposureDiff[2] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[2] - mExposureQueue.ExposureDataList[idx2].exposureDiff[2]) * frame_ratio;
				ret.exposureDiff[3] = mExposureQueue.ExposureDataList[idx2].exposureDiff[3] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[3] - mExposureQueue.ExposureDataList[idx2].exposureDiff[3]) * frame_ratio;

				return ret;
			}
		}
		ret.exposureDiff[0] = 0;
		ret.exposureDiff[1] = 0;
		ret.exposureDiff[2] = 0;
		ret.exposureDiff[3] = 0;
	}
	return ret;
}

ExposureData ExposureCompensation::getExposureDataForAnimation(long long time_stamp)
{
	long long tot_time = 0, frame_time = 0, pre_time = 0, cur_time = 0, next_time = 0;
	int cur_index, pre_index;
	float frame_ratio = 0.f;
	ExposureData ret;
	//if (!mExposureQueue.isEmpty()) 
	{
		for (int i = 0; i < 9; i++) {
			int idx1 = mExposureQueue.tail - i - 1;
			if (idx1 < 0)
				idx1 += NUM_EXPOSURE_BUFFER;
			int idx2 = mExposureQueue.tail - i - 2;
			if (idx2 < 0)
				idx2 += NUM_EXPOSURE_BUFFER;

			cur_time = mExposureQueue.ExposureDataList[idx1].time_stamp;
			pre_time = mExposureQueue.ExposureDataList[idx2].time_stamp;

			if (time_stamp >= cur_time) {
				float duration = (float)(cur_time - pre_time);
				if (duration > EXPOSURE_ANIMATION_DURATION)
					duration = EXPOSURE_ANIMATION_DURATION;

				if (duration == 0)
					continue;
				frame_ratio = (time_stamp - cur_time) / duration;
				if (frame_ratio > 1)
					frame_ratio = 1.f;
				else if (frame_ratio < 0)
					frame_ratio = 0.f;

				ret.exposureDiff[0] = mExposureQueue.ExposureDataList[idx2].exposureDiff[0] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[0] - mExposureQueue.ExposureDataList[idx2].exposureDiff[0]) * frame_ratio;
				ret.exposureDiff[1] = mExposureQueue.ExposureDataList[idx2].exposureDiff[1] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[1] - mExposureQueue.ExposureDataList[idx2].exposureDiff[1]) * frame_ratio;
				ret.exposureDiff[2] = mExposureQueue.ExposureDataList[idx2].exposureDiff[2] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[2] - mExposureQueue.ExposureDataList[idx2].exposureDiff[2]) * frame_ratio;
				ret.exposureDiff[3] = mExposureQueue.ExposureDataList[idx2].exposureDiff[3] + (mExposureQueue.ExposureDataList[idx1].exposureDiff[3] - mExposureQueue.ExposureDataList[idx2].exposureDiff[3]) * frame_ratio;

				for (int j = 0; j < 256; j++)
				{
					ret.exposureCurve[0][j] = mExposureQueue.ExposureDataList[idx2].exposureCurve[0][j] + (mExposureQueue.ExposureDataList[idx1].exposureCurve[0][j] - mExposureQueue.ExposureDataList[idx2].exposureCurve[0][j]) * frame_ratio;
					ret.exposureCurve[1][j] = mExposureQueue.ExposureDataList[idx2].exposureCurve[1][j] + (mExposureQueue.ExposureDataList[idx1].exposureCurve[1][j] - mExposureQueue.ExposureDataList[idx2].exposureCurve[1][j]) * frame_ratio;
					ret.exposureCurve[2][j] = mExposureQueue.ExposureDataList[idx2].exposureCurve[2][j] + (mExposureQueue.ExposureDataList[idx1].exposureCurve[2][j] - mExposureQueue.ExposureDataList[idx2].exposureCurve[2][j]) * frame_ratio;
				}

				return ret;
			}
		}
		ret.exposureDiff[0] = 0;
		ret.exposureDiff[1] = 0;
		ret.exposureDiff[2] = 0;
		ret.exposureDiff[3] = 0;
		for (int j = 0; j < 256; j++)
		{
			ret.exposureCurve[0][j] = j;
			ret.exposureCurve[1][j] = j;
			ret.exposureCurve[2][j] = j;
		}
	}

	return ret;
}