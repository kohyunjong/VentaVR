#ifndef __EXPOSURE_COMPENSATOR_QURAM_H__
#define __EXPOSURE_COMPENSATOR_QURAM_H__

#include <stdio.h>

#include "InputTempData.h"
#include "WarpInfo.h"
#include "StitchingMode.h"
#include "CamInput\CamInputArray.h"
#include "CamInput\CamInput.h"
#include "Composer\ComposeHelper.h"

#define USE_SPLINE_CURVE 1
#define SEAM_SCALE 8
#define VALUE_COUNT 256


struct OverlapInfo {
	int  num[2];							// Indices of Images 

	double diff[2];
	double diffRGB[2][3];

	double avg[2];
	double avgRGB[2][3];
	int avgCount;

	int *HistAVG0, *HistAVG1, *HistR0, *HistG0, *HistB0, *HistR1, *HistG1, *HistB1;
	int curvePointMin[2], curvePointMid[2], curvePointMax[2],
		curvePointMinR[2], curvePointMidR[2], curvePointMaxR[2],
		curvePointMinG[2], curvePointMidG[2], curvePointMaxG[2],
		curvePointMinB[2], curvePointMidB[2], curvePointMaxB[2];//[0] = x in curve, [1] = y in curve
	double **curveInfo = NULL, **curveInfoR = NULL, **curveInfoG = NULL, **curveInfoB = NULL;
};
typedef struct OverlapInfo OverlapInfo;

typedef struct _OverlapChannelInfo{
	int ich;
	int dist;
}OverlapChannelInfo;

typedef struct _ColorTransformInfo
{
private:
	int nsize = 0;
	std::vector<cv::Vec3b> firstcolors;
	std::vector<cv::Vec3b> secondcolors;
	int fch = -1;
	int sch = -1;

public:
	std::vector<float> deltaR[256];
	std::vector<float> deltaG[256];
	std::vector<float> deltaB[256];

	int size()
	{
		return nsize;
	}
	
	void setchInfo(int _fch, int _sch)
	{
		if (fch == -1)
			fch = _fch;
		if (sch == -1)
			sch = _sch;
	}
	int getfch()
	{
		return fch;
	}
	int getsch()
	{
		return sch;
	}
	void push(cv::Vec3b fcolor, cv::Vec3b scolor)
	{
		firstcolors.push_back(fcolor);
		secondcolors.push_back(scolor);

		cv::Vec3f delta;
		delta[0] = (float)fcolor[0] - (float)scolor[0];
		delta[1] = (float)fcolor[1] - (float)scolor[1];
		delta[2] = (float)fcolor[2] - (float)scolor[2];

		deltaR[fcolor[0]].push_back(delta[0]);
		deltaG[fcolor[1]].push_back(delta[1]);
		deltaB[fcolor[2]].push_back(delta[2]);

		nsize++;
	}
	cv::Vec3b get1stColor(int i)
	{
		if (i >= nsize)
			return cv::Vec3b(0, 0, 0);

		return firstcolors.at(i);
	}
	cv::Vec3b get2ndColor(int i)
	{
		if (i >= nsize)
			return cv::Vec3b(0, 0, 0);
		return secondcolors.at(i);
	}

	void clear()
	{
		nsize = 0;
		firstcolors.clear();
		secondcolors.clear();

		for (int i = 0; i < 256; i++)
		{
			deltaR[i].clear();
			deltaG[i].clear();
			deltaB[i].clear();
		}
	}

	cv::Vec3f error(int i)
	{
		cv::Vec3f err((float)INT_MAX, (float)INT_MAX, (float)INT_MAX);

		if (i >= nsize)
			return err;

		err = firstcolors[i] - secondcolors[i];

		return err;
	}
}ColorTransformInfo;

struct ImageInfo {
	double finalDiff;
	double finalDiffRGB[3];
	unsigned char *finalCurveInfo, *finalCurveInfoR, *finalCurveInfoG, *finalCurveInfoB;
	int pixelcnt;
};
typedef struct ImageInfo ImageInfo;

typedef struct Gain{
	int startIdx;
	int numImg;
	int matchCount;
	OverlapInfo *overlapInfo;
	ImageInfo *imgInfo;
	unsigned char ***thisCurveInfo, ***thisCurveInfoR, ***thisCurveInfoG, ***thisCurveInfoB;
}GainInfo;
typedef GainInfo* GainP;


struct ExposureCompensator {
	ExposureCompensator(int wdt, int hgt);
	int width;
	int height;
	int mInputCount;

	bool estimate(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx, int gainMode, int stereoIdx = -1);
	void initCurveInfo(int inputCount);

	std::vector<cv::Mat> getCurve(InputTempData* input_tempData, CamInputArray *input, long long time_stamp, std::vector<std::vector<std::vector<cv::Point2d>>> curvePoints, int gainMode);

	void destroy();
private:
	//Gain Curve
	unsigned char **mCurveInfoR = NULL, **mCurveInfoG = NULL, **mCurveInfoB = NULL;// , **mDefaultCurveInfoR = NULL, **mDefaultCurveInfoG = NULL, **mDefaultCurveInfoB = NULL;
	const int CURVE_POINT_MIN = 0;
	const int CURVE_POINT_MID = 1;
	const int CURVE_POINT_MAX = 2;
	const float MATCHING_THRESHOLD = 0.15f;
	const float CURVEPOINT_THRESHOLD = 0.3f;// 3f;//0.7f;

	bool calculatePixelAvg(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx, int gainMode);
	bool getDefaultColorTransformInformations(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx);
	bool getColorTransformInformations(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx);
	bool getNewColorTransformInformations(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx, int stereoIdx=-1);

	//bool calculatePixelAvg(Calibrator* calibrator, long long time_stamp, int* buffer_idx, int gainMode);
	void setOptGainInfo(GainInfo *g, int matchCount, int gainMode);
	void calculateDiff(GainInfo *g);
	void calculateCurveDiff(GainInfo *g, int idx, int gainMode);
	void setGainDiff(GainInfo *g, CamInputArray *input, int gainMode);
	void setGainCurve(CamInputArray *input, int inputCount, long long time_stamp);
	void initImgInfo(ImageInfo *info, int gainMode);
	void initMatchInfo(OverlapInfo *matchInfo, int gainMode);

	void initTempCurveInfo(GainInfo *g, int inputCount, int gainMode);
	bool makeManualCurve(std::vector<cv::Mat> &resultCurve, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints, GainInfo *g, int inputCount, int gainMode);
	bool makeOutputCurve(cv::Mat &resultCurve, std::vector<cv::Point2d> curvePoints, GainInfo *g);
	bool makeAutoCurve(GainInfo *g, int inputCount, int gainMode);
	bool genCurveRGB(GainInfo *g, int stage);
	void optiCurveRGB(GainInfo *g, int stage);
	
	double h00(double t);
	double h01(double t);
	double h10(double t);
	double h11(double t);
	bool monotonic_cubic_Hermite_spline(int time_limit, const std::vector<double> x_src, const std::vector<double> y_src, std::vector<double> &destX, std::vector<double> &destY);
	bool bezier_curve(int time_limit, const std::vector<double> x_src, const std::vector<double> y_src, std::vector<double> &destX, std::vector<double> &destY);
	bool cubic_spline(std::vector<double>* x_series, std::vector<double>* y_series, std::vector<double> *destX, std::vector<double>* destY);
	void calculateCurvePoint(GainInfo *g, int w, int h, int gainMode);
	bool curveInitialPointRGB(GainInfo *g, int w, int h, int stage);
	void setLUT(CamInputArray *input, int inputCount, int gainMode, int startIdx = 0);
	void destroyCurveData(GainInfo *g, int gainMode);

	void resetCurveInfo(int count);
	//void refineCurveTable();
	double round(double value, int pos);
	bool containROI(int w, int h, cv::Rect roi, cv::Point p);

};
#endif