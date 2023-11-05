#include "ExposureCompensator.h"

#include "CamInput/CamInputArray.h"
#include "Composer/ComposeHelper.h"

#include "time.h"

ExposureCompensator::ExposureCompensator(int wdt, int hgt)
{
	width = wdt;
	height = hgt;
}
bool ExposureCompensator::estimate(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx, int gainMode, int stereoIdx)
{
	switch (gainMode)
	{
	case NO_GAIN_COMPENSATION:
		printf("bigheadk, NO_GAIN_COMPENSATION\n");
		return getDefaultColorTransformInformations(input_tempData, input, pts_param, helpers, buffer_idx);
	case GAIN_CURVE_AUTO_RGB_COMPENSATION:
	{
		printf("bigheadk, GAIN_CURVE_AUTO_RGB_COMPENSATION\n");
											 time_t st = clock();
											 bool rt = getNewColorTransformInformations(input_tempData, input, pts_param, helpers, buffer_idx, stereoIdx);
											 time_t et = clock();
											 printf("khkim : curve time : %f s\n", (float)(et - st) / (float)CLOCKS_PER_SEC);
											 return rt;
	}
	default:
	{
		printf("bigheadk, default\n");
		time_t st = clock();
		bool rt = calculatePixelAvg(input_tempData, input, pts_param, helpers, buffer_idx, gainMode);
		time_t et = clock();
		printf("khkim : curve time : %f s\n", (float)(et - st) / (float)CLOCKS_PER_SEC);
		return rt;
	}
	}
}

double ExposureCompensator::round(double value, int pos)
{
	double temp;
	temp = value * pow(10, pos);  // 원하는 소수점 자리수만큼 10의 누승을 함
	temp = floor(temp + 0.5);          // 0.5를 더한후 버림하면 반올림이 됨
	temp *= pow(10, -pos);           // 다시 원래 소수점 자리수로
	return temp;
}

void ExposureCompensator::calculateDiff(GainInfo *g) {
	std::vector<double> diffList;
	std::vector<double> diffRGBList[3];
	//cv::vector<int> *countList;

	diffList.assign(g->numImg, 0);
	diffRGBList[0].assign(g->numImg, 0);
	diffRGBList[1].assign(g->numImg, 0);
	diffRGBList[2].assign(g->numImg, 0);

	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j) {
				g->overlapInfo[i].diff[0] = (g->overlapInfo[i].avg[0] - g->overlapInfo[i].avg[1]) / 2;//diff/2

				g->overlapInfo[i].diffRGB[0][0] = (g->overlapInfo[i].avgRGB[0][0] - g->overlapInfo[i].avgRGB[1][0]) / 2;
				g->overlapInfo[i].diffRGB[0][1] = (g->overlapInfo[i].avgRGB[0][1] - g->overlapInfo[i].avgRGB[1][1]) / 2;
				g->overlapInfo[i].diffRGB[0][2] = (g->overlapInfo[i].avgRGB[0][2] - g->overlapInfo[i].avgRGB[1][2]) / 2;

				//printf("calculateDiff[0-1]:index%d: g->overlapInfo[i].num=(%d, %d)\n", j, g->overlapInfo[i].num[0], g->overlapInfo[i].num[1]);
			}

			if (g->overlapInfo[i].num[1] == j) {
				g->overlapInfo[i].diff[1] = (g->overlapInfo[i].avg[1] - g->overlapInfo[i].avg[0]) / 2;//diff/2

				g->overlapInfo[i].diffRGB[1][0] = (g->overlapInfo[i].avgRGB[1][0] - g->overlapInfo[i].avgRGB[0][0]) / 2;
				g->overlapInfo[i].diffRGB[1][1] = (g->overlapInfo[i].avgRGB[1][1] - g->overlapInfo[i].avgRGB[0][1]) / 2;
				g->overlapInfo[i].diffRGB[1][2] = (g->overlapInfo[i].avgRGB[1][2] - g->overlapInfo[i].avgRGB[0][2]) / 2;

				//printf("calculateDiff[1-0]:index%d: g->overlapInfo[i].num=(%d, %d)\n", j, g->overlapInfo[i].num[0], g->overlapInfo[i].num[1]);
			}
		}
	}

	/*[apply diff by pixel count ratio]*/
	//countList = new cv::vector<int>[g->numImg];
	std::vector<int> sumList;
	sumList.assign(g->numImg, 0);
	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j) {
				//countList[j].push_back(g->overlapInfo[i].avgCount);
				sumList.at(j) += g->overlapInfo[i].avgCount;
			}
			if (g->overlapInfo[i].num[1] == j) {
				//countList[j].push_back(g->overlapInfo[i].avgCount);
				sumList.at(j) += g->overlapInfo[i].avgCount;
			}
		}
	}

	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j) {
				float ratio = ((float)(g->overlapInfo[i].avgCount) / sumList.at(j));//[apply diff by pixel count ratio]

				diffList.at(j) += (g->overlapInfo[i].diff[0] * ratio);

				diffRGBList[0].at(j) += (g->overlapInfo[i].diffRGB[0][0] * ratio);
				diffRGBList[1].at(j) += (g->overlapInfo[i].diffRGB[0][1] * ratio);
				diffRGBList[2].at(j) += (g->overlapInfo[i].diffRGB[0][2] * ratio);

			}
			if (g->overlapInfo[i].num[1] == j) {
				float ratio = ((float)(g->overlapInfo[i].avgCount) / sumList.at(j));

				diffList.at(j) += (g->overlapInfo[i].diff[1] * ratio);

				diffRGBList[0].at(j) += (g->overlapInfo[i].diffRGB[1][0] * ratio);
				diffRGBList[1].at(j) += (g->overlapInfo[i].diffRGB[1][1] * ratio);
				diffRGBList[2].at(j) += (g->overlapInfo[i].diffRGB[1][2] * ratio);
			}

		}
	}
	/*[apply diff by pixel count 1:1 unconditionally] end*/

	for (int j = 0; j < g->numImg; j++) {
		g->imgInfo[j].finalDiff += diffList.at(j);
		g->imgInfo[j].finalDiffRGB[0] += diffRGBList[0].at(j);
		g->imgInfo[j].finalDiffRGB[1] += diffRGBList[1].at(j);
		g->imgInfo[j].finalDiffRGB[2] += diffRGBList[2].at(j);
	}

	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j) {
				g->overlapInfo[i].avg[0] -= diffList.at(j);

				g->overlapInfo[i].avgRGB[0][0] -= diffRGBList[0].at(j);
				g->overlapInfo[i].avgRGB[0][1] -= diffRGBList[1].at(j);
				g->overlapInfo[i].avgRGB[0][2] -= diffRGBList[2].at(j);
			}
			if (g->overlapInfo[i].num[1] == j) {
				g->overlapInfo[i].avg[1] -= diffList.at(j);

				g->overlapInfo[i].avgRGB[1][0] -= diffRGBList[0].at(j);
				g->overlapInfo[i].avgRGB[1][1] -= diffRGBList[1].at(j);
				g->overlapInfo[i].avgRGB[1][2] -= diffRGBList[2].at(j);
			}
		}
	}


	diffList.clear();
	diffRGBList[0].clear();
	diffRGBList[1].clear();
	diffRGBList[2].clear();
}

void ExposureCompensator::optiCurveRGB(GainInfo *g, int stage)
{
	double **curveInfo, **curveInfoR, **curveInfoG, **curveInfoB;

	curveInfoR = (double**)malloc(g->numImg * sizeof(double));
	curveInfoG = (double**)malloc(g->numImg * sizeof(double));
	curveInfoB = (double**)malloc(g->numImg * sizeof(double));
	for (int i = 0; i < g->numImg; i++)
	{
		curveInfoR[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
		curveInfoG[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
		curveInfoB[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
		for (int j = 0; j < VALUE_COUNT; j++)
		{
			curveInfoR[i][j] = 0;
			curveInfoG[i][j] = 0;
			curveInfoB[i][j] = 0;
		}
	}

	//This code is more efficient.
	/*[apply diff by pixel count ratio]*/	
	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j) {
				float ratio = ((float)(g->overlapInfo[i].avgCount) / g->imgInfo[j].pixelcnt);// [apply diff by pixel count ratio]
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					curveInfoR[j][k] += (g->overlapInfo[i].curveInfoR[0][k] * ratio);
					curveInfoG[j][k] += (g->overlapInfo[i].curveInfoG[0][k] * ratio);
					curveInfoB[j][k] += (g->overlapInfo[i].curveInfoB[0][k] * ratio);
				}
			}
			if (g->overlapInfo[i].num[1] == j) {
				float ratio = ((float)(g->overlapInfo[i].avgCount) / g->imgInfo[j].pixelcnt);
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					curveInfoR[j][k] += (g->overlapInfo[i].curveInfoR[1][k] * ratio);
					curveInfoG[j][k] += (g->overlapInfo[i].curveInfoG[1][k] * ratio);
					curveInfoB[j][k] += (g->overlapInfo[i].curveInfoB[1][k] * ratio);
				}
			}
		}
	}

	//Rounding
	for (int j = 0; j < g->numImg; j++) {
		for (int k = 0; k < VALUE_COUNT; k++)
		{
			if (stage > 0)
			{
				int thisIndex = (int)(g->thisCurveInfoR[stage - 1][j][k]);
				g->thisCurveInfoR[stage][j][k] = cv::saturate_cast<uchar>(curveInfoR[j][thisIndex] + 0.5);//rounding

				thisIndex = (int)(g->thisCurveInfoG[stage - 1][j][k]);
				g->thisCurveInfoG[stage][j][k] = cv::saturate_cast<uchar>(curveInfoG[j][thisIndex] + 0.5);//rounding

				thisIndex = (int)(g->thisCurveInfoB[stage - 1][j][k]);
				g->thisCurveInfoB[stage][j][k] = cv::saturate_cast<uchar>(curveInfoB[j][thisIndex] + 0.5);//rounding
			}
			else
			{
				g->thisCurveInfoR[stage][j][k] = cv::saturate_cast<uchar>(curveInfoR[j][k] + 0.5);
				g->thisCurveInfoG[stage][j][k] = cv::saturate_cast<uchar>(curveInfoG[j][k] + 0.5);
				g->thisCurveInfoB[stage][j][k] = cv::saturate_cast<uchar>(curveInfoB[j][k] + 0.5);
			}
		}
	}

	if (stage == g->numImg - 1)
	{
		for (int j = 0; j < g->numImg; j++) {
			for (int k = 0; k < VALUE_COUNT; k++)
			{
				g->imgInfo[j].finalCurveInfoR[k] = g->thisCurveInfoR[stage][j][k];
				g->imgInfo[j].finalCurveInfoG[k] = g->thisCurveInfoG[stage][j][k];
				g->imgInfo[j].finalCurveInfoB[k] = g->thisCurveInfoB[stage][j][k];
			}
		}
		for (int i = 0; i < g->numImg; i++) {
			for (int j = 0; j < g->numImg; j++) {
				free(g->thisCurveInfoR[i][j]);
				free(g->thisCurveInfoG[i][j]);
				free(g->thisCurveInfoB[i][j]);
			}
			free(g->thisCurveInfoR[i]);
			free(g->thisCurveInfoG[i]);
			free(g->thisCurveInfoB[i]);
		}
		free(g->thisCurveInfoR);
		free(g->thisCurveInfoG);
		free(g->thisCurveInfoB);
	}
	else
	{
		std::vector<std::vector<int>> tempHistR0, tempHistG0, tempHistB0, tempHistR1, tempHistG1, tempHistB1;
		for (int i = 0; i < g->matchCount; i++)
		{
			if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
				continue;
			std::vector<int> list;
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				list.push_back(0);
			}
			tempHistR0.push_back(list);
			tempHistG0.push_back(list);
			tempHistB0.push_back(list);
			tempHistR1.push_back(list);
			tempHistG1.push_back(list);
			tempHistB1.push_back(list);
		}
		for (int j = 0; j < g->numImg; j++) {
			for (int i = 0; i < g->matchCount; i++) {
				if (g->overlapInfo[i].num[0] == j) {
					for (int k = 0; k < VALUE_COUNT; k++)
					{
						int count = g->overlapInfo[i].HistR0[k];

						uchar newSrc = cv::saturate_cast<uchar>(curveInfoR[j][k] + 0.5);
						tempHistR0[i][newSrc] += count;

						count = g->overlapInfo[i].HistG0[k];

						newSrc = cv::saturate_cast<uchar>(curveInfoG[j][k] + 0.5);
						tempHistG0[i][newSrc] += count;

						count = g->overlapInfo[i].HistB0[k];

						newSrc = cv::saturate_cast<uchar>(curveInfoB[j][k] + 0.5);
						tempHistB0[i][newSrc] += count;
					}
				}
				if (g->overlapInfo[i].num[1] == j) {
					for (int k = 0; k < VALUE_COUNT; k++)
					{
						int count = g->overlapInfo[i].HistR1[k];

						uchar newSrc = cv::saturate_cast<uchar>(curveInfoR[j][k] + 0.5);
						tempHistR1[i][newSrc] += count;

						count = g->overlapInfo[i].HistG1[k];

						newSrc = cv::saturate_cast<uchar>(curveInfoG[j][k] + 0.5);
						tempHistG1[i][newSrc] += count;

						count = g->overlapInfo[i].HistB1[k];

						newSrc = cv::saturate_cast<uchar>(curveInfoB[j][k] + 0.5);
						tempHistB1[i][newSrc] += count;
					}
				}
			}
		}

		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
				continue;
			for (int k = 0; k < VALUE_COUNT; k++)
			{
				g->overlapInfo[i].HistR0[k] = tempHistR0[i][k];
				g->overlapInfo[i].HistG0[k] = tempHistG0[i][k];
				g->overlapInfo[i].HistB0[k] = tempHistB0[i][k];
				g->overlapInfo[i].HistR1[k] = tempHistR1[i][k];
				g->overlapInfo[i].HistG1[k] = tempHistG1[i][k];
				g->overlapInfo[i].HistB1[k] = tempHistB1[i][k];
			}
		}
	}


	for (int j = 0; j < g->numImg; j++) {
		free(curveInfoR[j]);
		free(curveInfoG[j]);
		free(curveInfoB[j]);
	}
	free(curveInfoR);
	free(curveInfoG);
	free(curveInfoB);
	
}

void ExposureCompensator::calculateCurveDiff(GainInfo *g, int idx, int gainMode) {

	double **curveInfo, **curveInfoR, **curveInfoG, **curveInfoB;
	//cv::vector<int> *countList;
	int diff[2];
	diff[0] = 0;
	diff[1] = 0;

	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
	{
		curveInfo = (double**)malloc(g->numImg * sizeof(double));
		for (int i = 0; i < g->numImg; i++)
		{
			curveInfo[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				curveInfo[i][j] = 0;
			}
		}
	}
	else
	{
		curveInfoR = (double**)malloc(g->numImg * sizeof(double));
		curveInfoG = (double**)malloc(g->numImg * sizeof(double));
		curveInfoB = (double**)malloc(g->numImg * sizeof(double));
		for (int i = 0; i < g->numImg; i++)
		{
			curveInfoR[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
			curveInfoG[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
			curveInfoB[i] = (double*)malloc(VALUE_COUNT * sizeof(double));
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				curveInfoR[i][j] = 0;
				curveInfoG[i][j] = 0;
				curveInfoB[i][j] = 0;
			}
		}
	}

#if 1 //This code is more efficient.
	/*[apply diff by pixel count ratio]*/
	std::vector<int> sumList;
	sumList.assign(g->numImg, 0);
	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			for (int k = 0; k < 2; k++)
			{
				if (g->overlapInfo[i].num[k] == j)
				{
					sumList.at(j) += g->overlapInfo[i].avgCount;
				}
			}
		}
	}
#else 
	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j)
			{
				g->imgInfo[j].matchCount++;
			}
			if (g->overlapInfo[i].num[1] == j)
			{
				g->imgInfo[j].matchCount++;
			}
		}
	}
	/*[apply diff by pixel count ratio]*/
	std::vector<int> sumList;
	sumList.assign(g->numImg, 0);
	for (int j = 0; j < g->numImg; j++) {
		for (int i = 0; i < g->matchCount; i++) {
			if (g->overlapInfo[i].num[0] == j) {
				//countList[j].push_back(g->overlapInfo[i].avgCount);
				sumList.at(j) += g->overlapInfo[i].avgCount;
			}
			if (g->overlapInfo[i].num[1] == j) {
				//countList[j].push_back(g->overlapInfo[i].avgCount);
				sumList.at(j) += g->overlapInfo[i].avgCount;
			}
		}
	}
	/*[apply diff by pixel count ratio] end*/
#endif

	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
	{
		for (int j = 0; j < g->numImg; j++) {
			for (int i = 0; i < g->matchCount; i++) {
				if (g->overlapInfo[i].num[0] == j) {
					//float ratio = 1;// [apply diff by pixel count 1:1 unconditionally]
					float ratio = ((float)(g->overlapInfo[i].avgCount) / sumList.at(j));// [apply diff by pixel count ratio]
					for (int k = 0; k < VALUE_COUNT; k++)
					{
						curveInfo[j][k] += (g->overlapInfo[i].curveInfo[0][k] * ratio);
					}
				}
				if (g->overlapInfo[i].num[1] == j) {
					//float ratio = 1;
					float ratio = ((float)(g->overlapInfo[i].avgCount) / sumList.at(j));
					for (int k = 0; k < VALUE_COUNT; k++)
					{
						curveInfo[j][k] += (g->overlapInfo[i].curveInfo[1][k] * ratio);
					}
				}
			}
		}

		//Rounding
		for (int j = 0; j < g->numImg; j++) {
			for (int k = 0; k < VALUE_COUNT; k++)
			{
				if (idx > 0)//curve update except for first loop
				{
					int thisIndex = (int)(g->thisCurveInfo[idx - 1][j][k]);
					g->thisCurveInfo[idx][j][k] = cv::saturate_cast<uchar>(curveInfo[j][thisIndex]+0.5);

				}
				else//curve update for first loop
				{
					g->thisCurveInfo[idx][j][k] = cv::saturate_cast<uchar>(curveInfo[j][k] + 0.5);
				}

				//if (g->thisCurveInfo[idx][j][k] > 255)
				//	g->thisCurveInfo[idx][j][k] = 255;
				//if (g->thisCurveInfo[idx][j][k] < 0)
				//	g->thisCurveInfo[idx][j][k] = 0;
			}
		}

		if (idx == g->numImg - 1)//last loop
		{
			for (int j = 0; j < g->numImg; j++) {
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					g->imgInfo[j].finalCurveInfo[k] = g->thisCurveInfo[idx][j][k];
				}
			}
			for (int i = 0; i < g->numImg; i++) {
				for (int j = 0; j < g->numImg; j++) {
					free(g->thisCurveInfo[i][j]);
				}
				free(g->thisCurveInfo[i]);
			}
			free(g->thisCurveInfo);
		}
		else//update histogram 
		{
			std::vector<std::vector<int>> tempHist0, tempHist1;
			for (int i = 0; i < g->matchCount; i++)
			{
				if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
					continue;

				std::vector<int> list;
				for (int j = 0; j < VALUE_COUNT; j++)
				{
					list.push_back(0);
				}
				tempHist0.push_back(list);
				tempHist1.push_back(list);
			}
			for (int j = 0; j < g->numImg; j++) {
				for (int i = 0; i < g->matchCount; i++) {
					if (g->overlapInfo[i].num[0] == j) {
						for (int k = 0; k < VALUE_COUNT; k++)
						{
							int count = g->overlapInfo[i].HistAVG0[k];
							int newSrc = cv::saturate_cast<uchar>(curveInfo[j][k] + 0.5);
							tempHist0[i][newSrc] += count;

						}
					}
					if (g->overlapInfo[i].num[1] == j) {
						for (int k = 0; k < VALUE_COUNT; k++)
						{
							int count = g->overlapInfo[i].HistAVG1[k];
							int newSrc = cv::saturate_cast<uchar>(curveInfo[j][k] + 0.5);
							tempHist1[i][newSrc] += count;
						}
					}
				}
			}

			for (int i = 0; i < g->matchCount; i++) {
				if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
					continue;
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					g->overlapInfo[i].HistAVG0[k] = tempHist0[i][k];
					g->overlapInfo[i].HistAVG1[k] = tempHist1[i][k];
				}
			}
		}

		for (int j = 0; j < g->numImg; j++) {
			free(curveInfo[j]);
		}
		free(curveInfo);
	}
	else
	{
		for (int j = 0; j < g->numImg; j++) {
			for (int i = 0; i < g->matchCount; i++) {
				if (g->overlapInfo[i].num[0] == j) {
					//float ratio = 1;// [apply diff by pixel count 1:1 unconditionally]
					float ratio = ((float)(g->overlapInfo[i].avgCount) / sumList.at(j));// [apply diff by pixel count ratio]
					//printf("calculateCurveDiff11: ratio=%f\n", ratio);
					for (int k = 0; k < VALUE_COUNT; k++)
					{
						curveInfoR[j][k] += (g->overlapInfo[i].curveInfoR[0][k] * ratio);
						curveInfoG[j][k] += (g->overlapInfo[i].curveInfoG[0][k] * ratio);
						curveInfoB[j][k] += (g->overlapInfo[i].curveInfoB[0][k] * ratio);
					}
				}
				if (g->overlapInfo[i].num[1] == j) {
					//float ratio = 1;
					float ratio = ((float)(g->overlapInfo[i].avgCount) / sumList.at(j));
					//printf("calculateCurveDiff22: ratio=%f\n", ratio);
					for (int k = 0; k < VALUE_COUNT; k++)
					{
						curveInfoR[j][k] += (g->overlapInfo[i].curveInfoR[1][k] * ratio);
						curveInfoG[j][k] += (g->overlapInfo[i].curveInfoG[1][k] * ratio);
						curveInfoB[j][k] += (g->overlapInfo[i].curveInfoB[1][k] * ratio);
					}
				}

			}
		}

		//Rounding
		for (int j = 0; j < g->numImg; j++) {
			for (int k = 0; k < VALUE_COUNT; k++)
			{
				if (idx > 0)
				{
					int thisIndex = (int)(g->thisCurveInfoR[idx - 1][j][k]);
					g->thisCurveInfoR[idx][j][k] = cv::saturate_cast<uchar>(curveInfoR[j][thisIndex] + 0.5);//rounding

					thisIndex = (int)(g->thisCurveInfoG[idx - 1][j][k]);
					g->thisCurveInfoG[idx][j][k] = cv::saturate_cast<uchar>(curveInfoG[j][thisIndex] + 0.5);//rounding

					thisIndex = (int)(g->thisCurveInfoB[idx - 1][j][k]);
					g->thisCurveInfoB[idx][j][k] = cv::saturate_cast<uchar>(curveInfoB[j][thisIndex] + 0.5);//rounding

				}
				else
				{
					g->thisCurveInfoR[idx][j][k] = cv::saturate_cast<uchar>(curveInfoR[j][k] + 0.5);
					g->thisCurveInfoG[idx][j][k] = cv::saturate_cast<uchar>(curveInfoG[j][k] + 0.5);
					g->thisCurveInfoB[idx][j][k] = cv::saturate_cast<uchar>(curveInfoB[j][k] + 0.5);
				}

				//if (g->thisCurveInfoR[idx][j][k] > 255)
				//	g->thisCurveInfoR[idx][j][k] = 255;
				//if (g->thisCurveInfoR[idx][j][k] < 0)
				//	g->thisCurveInfoR[idx][j][k] = 0;
				//if (g->thisCurveInfoG[idx][j][k] > 255)
				//	g->thisCurveInfoG[idx][j][k] = 255;
				//if (g->thisCurveInfoG[idx][j][k] < 0)
				//	g->thisCurveInfoG[idx][j][k] = 0;
				//if (g->thisCurveInfoB[idx][j][k] > 255)
				//	g->thisCurveInfoB[idx][j][k] = 255;
				//if (g->thisCurveInfoB[idx][j][k] < 0)
				//	g->thisCurveInfoB[idx][j][k] = 0;
			}
		}

		if (idx == g->numImg - 1)
			//if (idx == 2)
		{
			for (int j = 0; j < g->numImg; j++) {
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					g->imgInfo[j].finalCurveInfoR[k] = g->thisCurveInfoR[idx][j][k];
					g->imgInfo[j].finalCurveInfoG[k] = g->thisCurveInfoG[idx][j][k];
					g->imgInfo[j].finalCurveInfoB[k] = g->thisCurveInfoB[idx][j][k];
				}
			}
			for (int i = 0; i < g->numImg; i++) {
				for (int j = 0; j < g->numImg; j++) {
					free(g->thisCurveInfoR[i][j]);
					free(g->thisCurveInfoG[i][j]);
					free(g->thisCurveInfoB[i][j]);
				}
				free(g->thisCurveInfoR[i]);
				free(g->thisCurveInfoG[i]);
				free(g->thisCurveInfoB[i]);
			}
			free(g->thisCurveInfoR);
			free(g->thisCurveInfoG);
			free(g->thisCurveInfoB);
		}
		else
		{
			std::vector<std::vector<int>> tempHistR0, tempHistG0, tempHistB0, tempHistR1, tempHistG1, tempHistB1;
			for (int i = 0; i < g->matchCount; i++)
			{
				if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
					continue;
				std::vector<int> list;
				for (int j = 0; j < VALUE_COUNT; j++)
				{
					list.push_back(0);
				}
				tempHistR0.push_back(list);
				tempHistG0.push_back(list);
				tempHistB0.push_back(list);
				tempHistR1.push_back(list);
				tempHistG1.push_back(list);
				tempHistB1.push_back(list);
			}
			for (int j = 0; j < g->numImg; j++) {
				for (int i = 0; i < g->matchCount; i++) {
					if (g->overlapInfo[i].num[0] == j) {
						for (int k = 0; k < VALUE_COUNT; k++)
						{
							int count = g->overlapInfo[i].HistR0[k];

							uchar newSrc = cv::saturate_cast<uchar>(curveInfoR[j][k] + 0.5);
							tempHistR0[i][newSrc] += count;

							count = g->overlapInfo[i].HistG0[k];

							newSrc = cv::saturate_cast<uchar>(curveInfoG[j][k] + 0.5);
							tempHistG0[i][newSrc] += count;

							count = g->overlapInfo[i].HistB0[k];

							newSrc = cv::saturate_cast<uchar>(curveInfoB[j][k] + 0.5);
							tempHistB0[i][newSrc] += count;
						}
					}
					if (g->overlapInfo[i].num[1] == j) {
						for (int k = 0; k < VALUE_COUNT; k++)
						{
							int count = g->overlapInfo[i].HistR1[k];

							uchar newSrc = cv::saturate_cast<uchar>(curveInfoR[j][k] + 0.5);
							tempHistR1[i][newSrc] += count;

							count = g->overlapInfo[i].HistG1[k];

							newSrc = cv::saturate_cast<uchar>(curveInfoG[j][k] + 0.5);
							tempHistG1[i][newSrc] += count;

							count = g->overlapInfo[i].HistB1[k];

							newSrc = cv::saturate_cast<uchar>(curveInfoB[j][k] + 0.5);
							tempHistB1[i][newSrc] += count;
						}
					}
				}
			}

			for (int i = 0; i < g->matchCount; i++) {
				if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
					continue;
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					g->overlapInfo[i].HistR0[k] = tempHistR0[i][k];
					g->overlapInfo[i].HistG0[k] = tempHistG0[i][k];
					g->overlapInfo[i].HistB0[k] = tempHistB0[i][k];
					g->overlapInfo[i].HistR1[k] = tempHistR1[i][k];
					g->overlapInfo[i].HistG1[k] = tempHistG1[i][k];
					g->overlapInfo[i].HistB1[k] = tempHistB1[i][k];
				}
			}
		}
		//for (int k = 0; k < VALUE_COUNT; k++)
		//	printf("JW calculateCurveDiff after: g->imgInfo[2].finalCurveInfo[%d]=%d\n", k, g->imgInfo[2].finalCurveInfo[k]);

		//printf("calculate curve rgb error done\n");

		for (int j = 0; j < g->numImg; j++) {
			free(curveInfoR[j]);
			free(curveInfoG[j]);
			free(curveInfoB[j]);
		}
		free(curveInfoR);
		free(curveInfoG);
		free(curveInfoB);
	}
	//printf("JW calculateCurveDiff end\n");
}

void ExposureCompensator::setOptGainInfo(GainInfo *g, int matchCount, int gainMode) {
	//g->matchCount = matchCount;

	for (int i = 0; i < g->matchCount; i++) {
		if (g->overlapInfo[i].num[0] < -1 || g->overlapInfo[i].num[1] < -1)
			continue;
		g->overlapInfo[i].avg[0] = ((g->overlapInfo[i].avgRGB[0][0] + g->overlapInfo[i].avgRGB[0][1] + g->overlapInfo[i].avgRGB[0][2]) / 3) / g->overlapInfo[i].avgCount;
		g->overlapInfo[i].avg[1] = ((g->overlapInfo[i].avgRGB[1][0] + g->overlapInfo[i].avgRGB[1][1] + g->overlapInfo[i].avgRGB[1][2]) / 3) / g->overlapInfo[i].avgCount;

		g->overlapInfo[i].avgRGB[0][0] /= g->overlapInfo[i].avgCount;
		g->overlapInfo[i].avgRGB[0][1] /= g->overlapInfo[i].avgCount;
		g->overlapInfo[i].avgRGB[0][2] /= g->overlapInfo[i].avgCount;
		g->overlapInfo[i].avgRGB[1][0] /= g->overlapInfo[i].avgCount;
		g->overlapInfo[i].avgRGB[1][1] /= g->overlapInfo[i].avgCount;
		g->overlapInfo[i].avgRGB[1][2] /= g->overlapInfo[i].avgCount;
	}

	for (int i = 0; i < g->numImg; i++) {
		calculateDiff(g);
	}

	for (int i = 0; i < g->numImg; i++)
	{
		//printf("g->imgInfo[i].finalDiff = %lf\n", g->imgInfo[i].finalDiff);
		for (int k = 0; k < VALUE_COUNT; k++)
		{
			int diff = 0;;
			if (gainMode == GAIN_AVERAGE_COMPENSATION_FULL)
			{
				if (i == 0)
				{
					diff = k - g->imgInfo[g->numImg - 1].finalDiff;
				}
				else
				{
					diff = k - g->imgInfo[i - 1].finalDiff;
				}
				if (diff < 0)
					diff = 0;
				if (diff > 255)
					diff = 255;

				mCurveInfoR[i][k] = diff;
				mCurveInfoG[i][k] = diff;
				mCurveInfoB[i][k] = diff;
				//if (i == 0)
				//	printf("mCurveInfoR[0][%d]=%d\n", k, mCurveInfoR[i][k]);
			}
			else if (gainMode == GAIN_RGB_COMPENSATION_FULL)
			{
				if (i == 0)
				{
					diff = k - g->imgInfo[g->numImg - 1].finalDiffRGB[0];
				}
				else
				{
					diff = k - g->imgInfo[i - 1].finalDiffRGB[0];
				}
				if (diff < 0)
					diff = 0;
				if (diff > 255)
					diff = 255;
				mCurveInfoR[i][k] = diff;

				if (i == 0)
				{
					diff = k - g->imgInfo[g->numImg - 1].finalDiffRGB[1];
				}
				else
				{
					diff = k - g->imgInfo[i - 1].finalDiffRGB[1];
				}
				if (diff < 0)
					diff = 0;
				if (diff > 255)
					diff = 255;
				mCurveInfoG[i][k] = diff;

				if (i == 0)
				{
					diff = k - g->imgInfo[g->numImg - 1].finalDiffRGB[2];
				}
				else
				{
					diff = k - g->imgInfo[i - 1].finalDiffRGB[2];
				}
				if (diff < 0)
					diff = 0;
				if (diff > 255)
					diff = 255;
				mCurveInfoB[i][k] = diff;
			}
			else//GainOff
			{
				mCurveInfoR[i][k] = k;
				mCurveInfoG[i][k] = k;
				mCurveInfoB[i][k] = k;
			}
		}
	}
}
void ExposureCompensator::initCurveInfo(int inputCount){
	mInputCount = inputCount;
	if (mCurveInfoR == NULL)
	{
		mCurveInfoR = (unsigned char**)malloc(inputCount * sizeof(unsigned char*));
		for (int i = 0; i < inputCount; i++)
		{
			mCurveInfoR[i] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				mCurveInfoR[i][j] = j;
			}
		}
	}
	if (mCurveInfoG == NULL)
	{
		mCurveInfoG = (unsigned char**)malloc(inputCount * sizeof(unsigned char*));
		for (int i = 0; i < inputCount; i++)
		{
			mCurveInfoG[i] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				mCurveInfoG[i][j] = j;
			}
		}
	}
	if (mCurveInfoB == NULL)
	{
		mCurveInfoB = (unsigned char**)malloc(inputCount * sizeof(unsigned char*));
		for (int i = 0; i < inputCount; i++)
		{
			mCurveInfoB[i] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
			for (int j = 0; j < VALUE_COUNT; j++)
			{
				mCurveInfoB[i][j] = j;
			}
		}
	}
}
void ExposureCompensator::initTempCurveInfo(GainInfo *g, int inputCount, int gainMode)
{
	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
	{
		g->thisCurveInfo = (unsigned char***)malloc(inputCount * sizeof(unsigned char**));
		for (int i = 0; i < inputCount; i++)
		{
			g->thisCurveInfo[i] = (unsigned char**)malloc(inputCount* sizeof(unsigned char*));
			for (int j = 0; j < inputCount; j++)
			{
				g->thisCurveInfo[i][j] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
				for (int k = 0; k < VALUE_COUNT; k++)
				{
					g->thisCurveInfo[i][j][k] = 0;
				}
			}

		}
	}
	else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
	{
		g->thisCurveInfoR = (unsigned char***)malloc(inputCount* sizeof(unsigned char**));
		g->thisCurveInfoG = (unsigned char***)malloc(inputCount * sizeof(unsigned char**));
		g->thisCurveInfoB = (unsigned char***)malloc(inputCount * sizeof(unsigned char**));

		for (int i = 0; i < inputCount; i++)
		{
			g->thisCurveInfoR[i] = (unsigned char**)malloc(inputCount * sizeof(unsigned char*));
			g->thisCurveInfoG[i] = (unsigned char**)malloc(inputCount * sizeof(unsigned char*));
			g->thisCurveInfoB[i] = (unsigned char**)malloc(inputCount * sizeof(unsigned char*));

			for (int j = 0; j < inputCount; j++)
			{
				g->thisCurveInfoR[i][j] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
				g->thisCurveInfoG[i][j] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
				g->thisCurveInfoB[i][j] = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));

				for (int k = 0; k < VALUE_COUNT; k++)
				{
					g->thisCurveInfoR[i][j][k] = 0;
					g->thisCurveInfoG[i][j][k] = 0;
					g->thisCurveInfoB[i][j][k] = 0;
				}
			}
		}
	}
}
void ExposureCompensator::initImgInfo(ImageInfo *imgInfo, int gainMode) {
	imgInfo->finalDiff = 0;
	imgInfo->finalDiffRGB[0] = 0;
	imgInfo->finalDiffRGB[1] = 0;
	imgInfo->finalDiffRGB[2] = 0;
	imgInfo->pixelcnt = 0;
	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
	{
		imgInfo->finalCurveInfo = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
		for (int i = 0; i < VALUE_COUNT; i++)
		{
			imgInfo->finalCurveInfo[i] = 0;
		}
	}
	else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
	{
		imgInfo->finalCurveInfoR = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
		imgInfo->finalCurveInfoG = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
		imgInfo->finalCurveInfoB = (unsigned char*)malloc(VALUE_COUNT * sizeof(unsigned char));
		for (int i = 0; i < VALUE_COUNT; i++)
		{
			imgInfo->finalCurveInfoR[i] = 0;
			imgInfo->finalCurveInfoG[i] = 0;
			imgInfo->finalCurveInfoB[i] = 0;
		}
	}
	//printf("initImgInfo: finalCurveInfo malloc\n");
}
void ExposureCompensator::initMatchInfo(OverlapInfo *matchInfo, int gainMode) {
	matchInfo->avg[0] = 0;
	matchInfo->avg[1] = 0;
	matchInfo->avgCount = 0;
	matchInfo->avgRGB[0][0] = 0;
	matchInfo->avgRGB[0][1] = 0;
	matchInfo->avgRGB[0][2] = 0;
	matchInfo->avgRGB[1][0] = 0;
	matchInfo->avgRGB[1][1] = 0;
	matchInfo->avgRGB[1][2] = 0;
	matchInfo->diff[0] = 0;
	matchInfo->diff[1] = 0;
	matchInfo->diffRGB[0][0] = 0;
	matchInfo->diffRGB[0][1] = 0;
	matchInfo->diffRGB[0][2] = 0;
	matchInfo->diffRGB[1][0] = 0;
	matchInfo->diffRGB[1][1] = 0;
	matchInfo->diffRGB[1][2] = 0;
	matchInfo->num[0] = -1;
	matchInfo->num[1] = -1;

	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
	{
		matchInfo->curvePointMin[0] = 0;
		matchInfo->curvePointMid[0] = 0;
		matchInfo->curvePointMax[0] = 0;
		matchInfo->curvePointMin[1] = 0;
		matchInfo->curvePointMid[1] = 0;
		matchInfo->curvePointMax[1] = 0;
		matchInfo->HistAVG0 = (int*)malloc(VALUE_COUNT * sizeof(int));
		matchInfo->HistAVG1 = (int*)malloc(VALUE_COUNT * sizeof(int));

		matchInfo->curveInfo = (double**)malloc(2 * sizeof(double*));
		matchInfo->curveInfo[0] = (double*)malloc(VALUE_COUNT * sizeof(double));
		matchInfo->curveInfo[1] = (double*)malloc(VALUE_COUNT * sizeof(double));

		for (int i = 0; i < VALUE_COUNT; i++)
		{
			matchInfo->HistAVG0[i] = 0;
			matchInfo->HistAVG1[i] = 0;

			matchInfo->curveInfo[0][i] = 0;
			matchInfo->curveInfo[1][i] = 0;
		}
	}
	else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
	{
		matchInfo->curvePointMinR[0] = 0;
		matchInfo->curvePointMidR[0] = 0;
		matchInfo->curvePointMaxR[0] = 0;
		matchInfo->curvePointMinR[1] = 0;
		matchInfo->curvePointMidR[1] = 0;
		matchInfo->curvePointMaxR[1] = 0;

		matchInfo->curvePointMinG[0] = 0;
		matchInfo->curvePointMidG[0] = 0;
		matchInfo->curvePointMaxG[0] = 0;
		matchInfo->curvePointMinG[1] = 0;
		matchInfo->curvePointMidG[1] = 0;
		matchInfo->curvePointMaxG[1] = 0;

		matchInfo->curvePointMinB[0] = 0;
		matchInfo->curvePointMidB[0] = 0;
		matchInfo->curvePointMaxB[0] = 0;
		matchInfo->curvePointMinB[1] = 0;
		matchInfo->curvePointMidB[1] = 0;
		matchInfo->curvePointMaxB[1] = 0;

		matchInfo->HistR0 = (int*)malloc(VALUE_COUNT * sizeof(int));
		matchInfo->HistG0 = (int*)malloc(VALUE_COUNT * sizeof(int));
		matchInfo->HistB0 = (int*)malloc(VALUE_COUNT * sizeof(int));
		matchInfo->HistR1 = (int*)malloc(VALUE_COUNT * sizeof(int));
		matchInfo->HistG1 = (int*)malloc(VALUE_COUNT * sizeof(int));
		matchInfo->HistB1 = (int*)malloc(VALUE_COUNT * sizeof(int));

		matchInfo->curveInfoR = (double**)malloc(2 * sizeof(double*));
		matchInfo->curveInfoR[0] = (double*)malloc(VALUE_COUNT * sizeof(double));
		matchInfo->curveInfoR[1] = (double*)malloc(VALUE_COUNT * sizeof(double));
		matchInfo->curveInfoG = (double**)malloc(2 * sizeof(double*));
		matchInfo->curveInfoG[0] = (double*)malloc(VALUE_COUNT * sizeof(double));
		matchInfo->curveInfoG[1] = (double*)malloc(VALUE_COUNT * sizeof(double));
		matchInfo->curveInfoB = (double**)malloc(2 * sizeof(double*));
		matchInfo->curveInfoB[0] = (double*)malloc(VALUE_COUNT * sizeof(double));
		matchInfo->curveInfoB[1] = (double*)malloc(VALUE_COUNT * sizeof(double));

		for (int i = 0; i < VALUE_COUNT; i++)
		{
			matchInfo->HistR0[i] = 0;
			matchInfo->HistG0[i] = 0;
			matchInfo->HistB0[i] = 0;
			matchInfo->HistR1[i] = 0;
			matchInfo->HistG1[i] = 0;
			matchInfo->HistB1[i] = 0;

			matchInfo->curveInfoR[0][i] = 0;
			matchInfo->curveInfoR[1][i] = 0;
			matchInfo->curveInfoG[0][i] = 0;
			matchInfo->curveInfoG[1][i] = 0;
			matchInfo->curveInfoB[0][i] = 0;
			matchInfo->curveInfoB[1][i] = 0;
		}
	}
}
double ExposureCompensator::h00(double t)
{
	return 2 * t*t*t - 3 * t*t + 1;
}
double ExposureCompensator::h10(double t)
{
	return t*(1 - t)*(1 - t);
}
double ExposureCompensator::h01(double t)
{
	return t*t*(3 - 2 * t);
}
double ExposureCompensator::h11(double t)
{
	return t*t*(t - 1);
}

double getPt(double n1, double n2, double perc)
{
	double diff = n2 - n1;

	return n1 + (diff * perc);
}

bool ExposureCompensator::bezier_curve(int time_limit, const std::vector<double> x_src, const std::vector<double> y_src, std::vector<double> &destX, std::vector<double> &destY)
{
	destX.clear();
	destY.clear();

	
	if (x_src.size() == 5)
	{
		for (int i = 0; i < time_limit; i++)
		{
			double percents = (double)i / (double)time_limit;
			// The Green Lines
			//double xa = getPt(x_src[0], x_src[1], percents);
			double ya = getPt(y_src[0], y_src[1], percents);
			//double xb = getPt(x_src[1], x_src[2], percents);
			double yb = getPt(y_src[1], y_src[2], percents);
			//double xc = getPt(x_src[2], x_src[3], percents);
			double yc = getPt(y_src[2], y_src[3], percents);
			//double xd = getPt(x_src[3], x_src[4], percents);
			double yd = getPt(y_src[3], y_src[4], percents);

			// The Blue Line
			//double xm = getPt(xa, xb, percents);
			double ym = getPt(ya, yb, percents);
			//double xn = getPt(xb, xc, percents);
			double yn = getPt(yb, yc, percents);
			//double xo = getPt(xc, xd, percents);
			double yo = getPt(yc, yd, percents);


			// The Black Dot
			//double xp = getPt(xm, xn, percents);
			double yp = getPt(ym, yn, percents);
			//double xq = getPt(xn, xo, percents);
			double yq = getPt(yn, yo, percents);

			// The Black Dot
			//double x = getPt(xp, xq, percents);
			double y = getPt(yp, yq, percents);

			destX.push_back(i);
			destY.push_back(y);
		}
	}
	else if (x_src.size() == 4)
	{
		for (int i = 0; i < time_limit; i++)
		{
			double percents = (double)i / (double)time_limit;
			// The Green Lines
			//double xa = getPt(x_src[0], x_src[1], percents);
			double ya = getPt(y_src[0], y_src[1], percents);
			//double xb = getPt(x_src[1], x_src[2], percents);
			double yb = getPt(y_src[1], y_src[2], percents);
			//double xc = getPt(x_src[2], x_src[3], percents);
			double yc = getPt(y_src[2], y_src[3], percents);

			// The Blue Line
			//double xm = getPt(xa, xb, percents);
			double ym = getPt(ya, yb, percents);
			//double xn = getPt(xb, xc, percents);
			double yn = getPt(yb, yc, percents);


			// The Black Dot
			//double x = getPt(xm, xn, percents);
			double y = getPt(ym, yn, percents);

			destX.push_back(i);
			destY.push_back(y);
		}
	}


	return true;
}
bool ExposureCompensator::monotonic_cubic_Hermite_spline(int time_limit, const std::vector<double> x_src, const std::vector<double> y_src, std::vector<double> &destX, std::vector<double> &destY)
{
	destX.clear();
	destY.clear();
	//destX->clear();
	//destY->clear();

	double eps = 1e-10;

	// 0-based index 사용.
	int n = (int)x_src.size();
	int k = 0;
	double *m = new double[n];
	m[0] = (y_src[1] - y_src[0]) / (x_src[1] - x_src[0]);
	m[n - 1] = (y_src[n - 1] - y_src[n - 2]) / (x_src[n - 1] - x_src[n - 2]);

	for (k = 1; k<n - 1; k++){
		m[k] = (y_src[k] - y_src[k - 1]) / (2 * (x_src[k] - x_src[k - 1])) + (y_src[k + 1] - y_src[k]) / (2 * (x_src[k + 1] - x_src[k]));
	}
	for (k = 0; k<n - 1; k++){
		double delta_k = (y_src[k + 1] - y_src[k]) / (x_src[k + 1] - x_src[k]);
		if (fabs(delta_k) <= eps){
			m[k] = m[k + 1] = 0;
		}
		else{
			double ak = m[k] / delta_k;
			double bk = m[k + 1] / delta_k;
			if (ak*ak + bk*bk > 9){
				m[k] = 3 / (sqrt(ak*ak + bk*bk))*ak*delta_k;
				m[k + 1] = 3 / (sqrt(ak*ak + bk*bk))*bk*delta_k;
			}
		}
	}


	for (k = 0; k<n - 1; k++){
		double cur_x = (double)((int)(0.5 + x_src[k]));
		double next_x = (double)((int)(x_src[k + 1]));
		double cur_y = y_src[k];
		double next_y = y_src[k + 1];
		double h = next_x - cur_x;
		double x = 0;

		double inc = 1;
		//double inc = (next_x - cur_x)*0.1;

		for (x = cur_x; x<next_x; x += inc){
			if (x > time_limit) break;
			double t = (x - cur_x) / h;
			//printf("monotonic_cubic_Hermite_spline: x=%lf\n", x);
			destX.push_back(x);
			//if (destX != NULL){
			//	destX->push_back(x);
			//}
			double y = cur_y*h00(t) + h*m[k] * h10(t) + next_y*h01(t) + h*m[k + 1] * h11(t);
			destY.push_back(y);
			//destY->push_back(y);
			//printf("monotonic_cubic_Hermite_spline: y=%lf\n", y);
		}
		if (k == n - 2)//last point
		{
			destX.push_back(next_x);

			double t = (next_x - cur_x) / h;
			double y = cur_y*h00(t) + h*m[k] * h10(t) + next_y*h01(t) + h*m[k + 1] * h11(t);
			destY.push_back(y);
		}
	}

	double maxval = destY.at(destY.size() - 1);
	for (int i = 0; i < destY.size(); i++)
	{
		if (maxval<destY.at(i))
		{
			maxval = destY.at(i);
		}
	}
	if (maxval > 255.0)
	{
		double r = 255.0 / maxval;
		for (int i = 0; i < destY.size(); i++)
		{
			destY.at(i) *= r;
		}
	}
	delete m;

	return true;
}
bool ExposureCompensator::makeManualCurve(std::vector<cv::Mat> &resultCurve, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints, GainInfo *g, int inputCount, int gainMode){
	//printf("makeCurve!!!\n");
	std::vector<double> x_src, y_src;
	std::vector<double> destX, destY;
	//std::vector<cv::Mat> resultCurve;
	cv::Scalar pointColor = cv::Scalar(255, 255, 255);
	int circle_radius = 3;
	bool curveAvailable = true;
	//printf("makeCurve: curve manual111\n");
	if (gainMode == GAIN_CURVE_MANUAL_AVERAGE_COMPENSATION)//manualCurvePoints.at(index)[0] = AVG VALUE
	{
		for (int k = 0; k < inputCount; k++)
		{
			if (manualCurvePoints.size() == 0)
			{
				//printf("makeCurve: curve manual111\n");
				x_src.push_back(0.f);
				y_src.push_back(0.f);

				x_src.push_back(127.f);
				y_src.push_back(127.f);

				x_src.push_back(255.f);
				y_src.push_back(255.f);
				//printf("makeCurve: curve manual222\n");
			}
			else
			{
				if (manualCurvePoints.at(k)[0].at(CURVE_POINT_MIN).x > 0)
				{
					x_src.push_back(0.f);
					y_src.push_back(0.f);
				}

				x_src.push_back(manualCurvePoints.at(k)[0].at(CURVE_POINT_MIN).x);
				y_src.push_back(manualCurvePoints.at(k)[0].at(CURVE_POINT_MIN).y);

				x_src.push_back(manualCurvePoints.at(k)[0].at(CURVE_POINT_MID).x);
				y_src.push_back(manualCurvePoints.at(k)[0].at(CURVE_POINT_MID).y);

				x_src.push_back(manualCurvePoints.at(k)[0].at(CURVE_POINT_MAX).x);
				y_src.push_back(manualCurvePoints.at(k)[0].at(CURVE_POINT_MAX).y);


				if (manualCurvePoints.at(k)[0].at(CURVE_POINT_MAX).x < 255)
				{
					x_src.push_back(255.f);
					y_src.push_back(255.f);
				}
			}

			monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
			//printf("makeCurve: curve manual333\n"); 

			for (int i = 0; i < VALUE_COUNT; i++)
			{
				if (destY.at(i) >= 0 || destY.at(i) <= 255)
				{
					//printf("destY.at(i) = %lf\n", destY.at(i));
				}
				else
				{
					curveAvailable = false;
					printf("This  Histogram from manual points can't make curve!!\n");
					return curveAvailable;
				}
				if (k == 0)
				{
					mCurveInfoR[inputCount - 1][i] = (unsigned char)(destY.at(i));
					mCurveInfoG[inputCount - 1][i] = (unsigned char)(destY.at(i));
					mCurveInfoB[inputCount - 1][i] = (unsigned char)(destY.at(i));
				}
				else
				{
					mCurveInfoR[k - 1][i] = (unsigned char)(destY.at(i));
					mCurveInfoG[k - 1][i] = (unsigned char)(destY.at(i));
					mCurveInfoB[k - 1][i] = (unsigned char)(destY.at(i));
				}

			}
			cv::Mat test = cv::Mat::zeros(256, 256, CV_8UC3);
			//cv::Mat test = cv::Mat::zeros(3, 3, CV_8UC3);
			//for (int i = 0; i < destX->size()-1; i++)
			for (int i = 0; i < destX.size() - 1; i++)
			{
				cv::line(test, cv::Point2f(destX.at(i), 255 - destX.at(i)), cv::Point2f(destX.at(i + 1), 255 - destX.at(i + 1)), cv::Scalar(64, 64, 64), 1);//default
				cv::line(test, cv::Point2f(destX.at(i), 255 - destY.at(i)), cv::Point2f(destX.at(i + 1), 255 - destY.at(i + 1)), cv::Scalar(0, 0, 255), 1);//modified
			}
			cv::circle(test, cv::Point2f(manualCurvePoints.at(k)[0].at(CURVE_POINT_MIN).x, 255 - manualCurvePoints.at(k)[0].at(CURVE_POINT_MIN).y), circle_radius, pointColor, CV_FILLED);
			cv::circle(test, cv::Point2f(manualCurvePoints.at(k)[0].at(CURVE_POINT_MID).x, 255 - manualCurvePoints.at(k)[0].at(CURVE_POINT_MID).y), circle_radius, pointColor, CV_FILLED);
			cv::circle(test, cv::Point2f(manualCurvePoints.at(k)[0].at(CURVE_POINT_MAX).x, 255 - manualCurvePoints.at(k)[0].at(CURVE_POINT_MAX).y), circle_radius, pointColor, CV_FILLED);
			//printf("monotonic_cubic_Hermite_spline: destX->size()=%d\n", destX.size());
			//printf("monotonic_cubic_Hermite_spline: destY->size()=%d\n", destY.size());
			resultCurve.push_back(test);
			//printf("makeCurve: curve manual444\n");
			//cv::imshow("curve", test);
			//cv::waitKey(0);


			destX.clear();
			destY.clear();
			x_src.clear();
			y_src.clear();
		}
	}
	else//GAIN_CURVE_MANUAL_RGB_COMPENSATION//manualCurvePoints.at(index)[0] = R, manualCurvePoints.at(index)[1] = G, manualCurvePoints.at(index)[2] = B
	{
		for (int k = 0; k < inputCount; k++)
		{
			cv::Mat test = cv::Mat::zeros(256, 256, CV_8UC3);
			for (int j = 0; j < 3; j++)//RGB(012) loop
			{
				if (manualCurvePoints.size() == 0)
				{
					/*for test
					if (k == 0)
					{
					if (j == 0)
					{
					x_src.push_back(0.f);
					y_src.push_back(55.f);

					x_src.push_back(127.f);
					y_src.push_back(155.f);

					x_src.push_back(255.f);
					y_src.push_back(222.f);
					}
					else if (j == 1)
					{
					x_src.push_back(0.f);
					y_src.push_back(15.f);

					x_src.push_back(127.f);
					y_src.push_back(105.f);

					x_src.push_back(255.f);
					y_src.push_back(200.f);
					}
					else
					{
					x_src.push_back(0.f);
					y_src.push_back(99.f);

					x_src.push_back(127.f);
					y_src.push_back(199.f);

					x_src.push_back(255.f);
					y_src.push_back(155.f);
					}

					}
					else*/
					{
						x_src.push_back(0.f);
						y_src.push_back(0.f);

						x_src.push_back(127.f);
						y_src.push_back(127.f);

						x_src.push_back(255.f);
						y_src.push_back(255.f);
					}

				}
				else
				{
					if (manualCurvePoints.at(k)[j].at(CURVE_POINT_MIN).x > 0)
					{
						x_src.push_back(0.f);
						y_src.push_back(0.f);
					}
					//printf("makeCurve: curve manual111\n");
					x_src.push_back(manualCurvePoints.at(k)[j].at(CURVE_POINT_MIN).x);
					y_src.push_back(manualCurvePoints.at(k)[j].at(CURVE_POINT_MIN).y);

					x_src.push_back(manualCurvePoints.at(k)[j].at(CURVE_POINT_MID).x);
					y_src.push_back(manualCurvePoints.at(k)[j].at(CURVE_POINT_MID).y);

					x_src.push_back(manualCurvePoints.at(k)[j].at(CURVE_POINT_MAX).x);
					y_src.push_back(manualCurvePoints.at(k)[j].at(CURVE_POINT_MAX).y);

					//printf("makeCurve: curve manual222\n");
					if (manualCurvePoints.at(k)[j].at(CURVE_POINT_MAX).x < 255)
					{
						x_src.push_back(255.f);
						y_src.push_back(255.f);
					}
				}

				monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
				//printf("makeCurve: curve manual333\n");

				for (int i = 0; i < VALUE_COUNT; i++)
				{
					if (destY.at(i) >= 0 || destY.at(i) <= 255)
					{
						//printf("destY.at(i) = %lf\n", destY.at(i));
					}
					else
					{
						curveAvailable = false;
						printf("This  Histogram from manual points can't make curve!!\n");
						return curveAvailable;
					}

					if (k == inputCount - 1)
					{
						if (j == 0)//R
							mCurveInfoR[0][i] = (unsigned char)(destY.at(i));
						else if (j == 1)//G
							mCurveInfoG[0][i] = (unsigned char)(destY.at(i));
						else//B
							mCurveInfoB[0][i] = (unsigned char)(destY.at(i));
					}
					else
					{
						if (j == 0)//R
							mCurveInfoR[k + 1][i] = (unsigned char)(destY.at(i));
						else if (j == 1)//G
							mCurveInfoG[k + 1][i] = (unsigned char)(destY.at(i));
						else//B
							mCurveInfoB[k + 1][i] = (unsigned char)(destY.at(i));
					}
				}

				for (int i = 0; i < destX.size() - 1; i++)
				{
					cv::Scalar color;
					if (j == 0)
					{
						cv::line(test, cv::Point2f(destX.at(i), 255 - destX.at(i)), cv::Point2f(destX.at(i + 1), 255 - destX.at(i + 1)), cv::Scalar(64, 64, 64), 1);//default
						color = cv::Scalar(0, 0, 255);

					}
					else if (j == 1)
					{
						color = cv::Scalar(0, 255, 0);
					}
					else
					{
						color = cv::Scalar(255, 0, 0);
					}
					cv::line(test, cv::Point2f(destX.at(i), 255 - destY.at(i)), cv::Point2f(destX.at(i + 1), 255 - destY.at(i + 1)), color, 1);//modified

				}
				cv::circle(test, cv::Point2f(manualCurvePoints.at(k)[j].at(CURVE_POINT_MIN).x, 255 - manualCurvePoints.at(k)[j].at(CURVE_POINT_MIN).y), circle_radius, pointColor, CV_FILLED);
				cv::circle(test, cv::Point2f(manualCurvePoints.at(k)[j].at(CURVE_POINT_MID).x, 255 - manualCurvePoints.at(k)[j].at(CURVE_POINT_MID).y), circle_radius, pointColor, CV_FILLED);
				cv::circle(test, cv::Point2f(manualCurvePoints.at(k)[j].at(CURVE_POINT_MAX).x, 255 - manualCurvePoints.at(k)[j].at(CURVE_POINT_MAX).y), circle_radius, pointColor, CV_FILLED);
				//printf("monotonic_cubic_Hermite_spline: destX->size()=%d\n", destX.size());
				//printf("monotonic_cubic_Hermite_spline: destY->size()=%d\n", destY.size());			

				destX.clear();
				destY.clear();
				x_src.clear();
				y_src.clear();
			}
			resultCurve.push_back(test);
			//cv::imshow("curve", test);
			//cv::waitKey(0);
		}
	}

	//printf("makeCurve: curve manual222\n");
	return curveAvailable;
}
bool ExposureCompensator::makeOutputCurve(cv::Mat &resultCurve, std::vector<cv::Point2d> curvePoints, GainInfo *g){
	//printf("makeCurve!!!\n");
	std::vector<double> x_src, y_src;
	std::vector<double> destX, destY;
	//std::vector<cv::Mat> resultCurve;
	cv::Scalar pointColor = cv::Scalar(255, 255, 255);
	int circle_radius = 3;
	bool curveAvailable = true;
	//printf("makeCurve: curve manual111\n");


	if (curvePoints.size() == 0)
	{
		//printf("makeCurve: curve manual111\n");
		x_src.push_back(0.f);
		y_src.push_back(0.f);

		x_src.push_back(127.f);
		y_src.push_back(127.f);

		x_src.push_back(255.f);
		y_src.push_back(255.f);
		//printf("makeCurve: curve manual222\n");
	}
	else
	{
		if (curvePoints.at(CURVE_POINT_MIN).x > 0)
		{
			x_src.push_back(0.f);
			y_src.push_back(0.f);
		}

		x_src.push_back(curvePoints.at(CURVE_POINT_MIN).x);
		y_src.push_back(curvePoints.at(CURVE_POINT_MIN).y);

		x_src.push_back(curvePoints.at(CURVE_POINT_MID).x);
		y_src.push_back(curvePoints.at(CURVE_POINT_MID).y);

		x_src.push_back(curvePoints.at(CURVE_POINT_MAX).x);
		y_src.push_back(curvePoints.at(CURVE_POINT_MAX).y);


		if (curvePoints.at(CURVE_POINT_MAX).x < 255)
		{
			x_src.push_back(255.f);
			y_src.push_back(255.f);
		}
	}

	monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
	//printf("makeCurve: curve manual333\n");

	for (int i = 0; i < VALUE_COUNT; i++)
	{
		if (destY.at(i) >= 0 || destY.at(i) <= 255)
		{
			//printf("destY.at(i) = %lf\n", destY.at(i));
		}
		else
		{
			curveAvailable = false;
			printf("This  Histogram from manual points can't make curve!!\n");
			return curveAvailable;
		}

	}
	cv::Mat test = cv::Mat::zeros(256, 256, CV_8UC3);
	//cv::Mat test = cv::Mat::zeros(3, 3, CV_8UC3);
	//for (int i = 0; i < destX->size()-1; i++)
	for (int i = 0; i < destX.size() - 1; i++)
	{
		cv::line(test, cv::Point2f(destX.at(i), 255 - destX.at(i)), cv::Point2f(destX.at(i + 1), 255 - destX.at(i + 1)), cv::Scalar(64, 64, 64), 1);//default
		cv::line(test, cv::Point2f(destX.at(i), 255 - destY.at(i)), cv::Point2f(destX.at(i + 1), 255 - destY.at(i + 1)), cv::Scalar(0, 0, 255), 1);//modified
	}
	cv::circle(test, cv::Point2f(curvePoints.at(CURVE_POINT_MIN).x, 255 - curvePoints.at(CURVE_POINT_MIN).y), circle_radius, pointColor, CV_FILLED);
	cv::circle(test, cv::Point2f(curvePoints.at(CURVE_POINT_MID).x, 255 - curvePoints.at(CURVE_POINT_MID).y), circle_radius, pointColor, CV_FILLED);
	cv::circle(test, cv::Point2f(curvePoints.at(CURVE_POINT_MAX).x, 255 - curvePoints.at(CURVE_POINT_MAX).y), circle_radius, pointColor, CV_FILLED);
	//printf("monotonic_cubic_Hermite_spline: destX->size()=%d\n", destX.size());
	//printf("monotonic_cubic_Hermite_spline: destY->size()=%d\n", destY.size());
	//resultCurve.push_back(test);
	test.copyTo(resultCurve);
	//printf("makeCurve: curve manual444\n");
	//cv::imshow("curve", test);
	//cv::waitKey(0);


	destX.clear();
	destY.clear();
	x_src.clear();
	y_src.clear();


	//printf("makeCurve: curve manual222\n");
	return curveAvailable;
}

bool ExposureCompensator::genCurveRGB(GainInfo *g, int stage){
	std::vector<double> x_src, y_src;
	std::vector<double> destX, destY;
	bool curveAvailable = true;

	for (int j = 0; j < g->matchCount; j++)
	{
		if (g->overlapInfo[j].num[0] < 0 || g->overlapInfo[j].num[1] < 0)
			continue;

		for (int ch = 0; ch < 3; ch++)
		{
			int* curvePointMin = NULL;
			int* curvePointMid = NULL;
			int* curvePointMax = NULL;
			double** curveInfo = NULL;

			switch (ch)
			{
			case 0:
				curvePointMin = g->overlapInfo[j].curvePointMinR;
				curvePointMid = g->overlapInfo[j].curvePointMidR;
				curvePointMax = g->overlapInfo[j].curvePointMaxR;
				curveInfo = g->overlapInfo[j].curveInfoR;
				break;
			case 1:
				curvePointMin = g->overlapInfo[j].curvePointMinG;
				curvePointMid = g->overlapInfo[j].curvePointMidG;
				curvePointMax = g->overlapInfo[j].curvePointMaxG;
				curveInfo = g->overlapInfo[j].curveInfoG;
				break;
			case 2:
				curvePointMin = g->overlapInfo[j].curvePointMinB;
				curvePointMid = g->overlapInfo[j].curvePointMidB;
				curvePointMax = g->overlapInfo[j].curvePointMaxB;
				curveInfo = g->overlapInfo[j].curveInfoB;
				break;
			}

			double minavg = (curvePointMin[0] + curvePointMin[1]) *0.5;
			double midavg = (curvePointMid[0] + curvePointMid[1]) *0.5;
			double maxavg = (curvePointMax[0] + curvePointMax[1]) *0.5;

			if (midavg <= minavg)
			{
				curveAvailable = false;
				printf("This Histogram can't make curve: midavg <= minavg\n");

				destX.clear();
				destY.clear();
				x_src.clear();
				y_src.clear();

				return curveAvailable;
			}
			if (maxavg <= midavg)
			{
				curveAvailable = false;
				printf("This Histogram can't make curve: maxavg <= midavg\n");

				destX.clear();
				destY.clear();
				x_src.clear();
				y_src.clear();

				return curveAvailable;
			}

			for (int k = 0; k < 2; k++)
			{
				if (curvePointMin[k] > 0)
				{
					x_src.push_back(0.f);
					y_src.push_back(0.f);
				}

				x_src.push_back((double)(curvePointMin[k]));
				y_src.push_back(minavg);

				x_src.push_back((double)(curvePointMid[k]));
				y_src.push_back(midavg);

				x_src.push_back((double)(curvePointMax[k]));
				y_src.push_back(maxavg);

				if (curvePointMax[k] < 255)
				{
					x_src.push_back(255.f);
					y_src.push_back(255.f);
				}

#if USE_SPLINE_CURVE
				monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
#else
				bezier_curve(VALUE_COUNT, x_src, y_src, destX, destY);
#endif
				for (int i = 0; i < VALUE_COUNT; i++)
				{
					if (destY.at(i) < 0 || destY.at(i) > 255)
					{
						curveAvailable = false;
						printf("This Histogram can't make curve: destY.at(%d) = %lf\n", i, destY.at(i));

						destX.clear();
						destY.clear();
						x_src.clear();
						y_src.clear();

						return curveAvailable;
					}

					curveInfo[k][i] = destY.at(i);
				}

				destX.clear();
				destY.clear();
				x_src.clear();
				y_src.clear();
			}

			for (int k = 0; k < 2; k++)
			{
				curvePointMin[k] = 0;
				curvePointMid[k] = 0;
				curvePointMax[k] = 0;
			}
		}			
	}

	return curveAvailable;
}

bool ExposureCompensator::makeAutoCurve(GainInfo *g, int inputCount, int gainMode){
	//printf("makeCurve!!!\n");
	std::vector<double> x_src, y_src;
	std::vector<double> destX, destY;
	bool curveAvailable = true;

	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION || gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
	{
		for (int j = 0; j < g->matchCount; j++)
		{
			if (g->overlapInfo[j].num[0] < 0 || g->overlapInfo[j].num[1] < 0)
				continue;

			if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
			{
				for (int k = 0; k < 2; k++)//matched couple
				{
					if (k == 0)
					{
						if (g->overlapInfo[j].curvePointMin[0] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMin[0] + g->overlapInfo[j].curvePointMin[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMin[0]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMid[0] + g->overlapInfo[j].curvePointMid[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMid[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMid[0]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMax[0] + g->overlapInfo[j].curvePointMax[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMax[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMax[0]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMax[0] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[0]), (double)(g->overlapInfo[j].curvePointMin[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[0]), (double)(g->overlapInfo[j].curvePointMid[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[0]), (double)(g->overlapInfo[j].curvePointMax[1]));*/

					}
					else
					{
						if (g->overlapInfo[j].curvePointMin[1] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMin[0] + g->overlapInfo[j].curvePointMin[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMin[1]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMid[0] + g->overlapInfo[j].curvePointMid[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMid[1]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMid[1]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMax[0] + g->overlapInfo[j].curvePointMax[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMax[1]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMax[1]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMax[1] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[1]), (double)(g->overlapInfo[j].curvePointMin[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[1]), (double)(g->overlapInfo[j].curvePointMid[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[1]), (double)(g->overlapInfo[j].curvePointMax[0]));*/
					}
#if USE_SPLINE_CURVE
					monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
#else
					bezier_curve(VALUE_COUNT, x_src, y_src, destX, destY);
#endif

					for (int i = 0; i < VALUE_COUNT; i++)
					{
						if (destY.at(i) >= 0 && destY.at(i) <= 255)
						{
							//printf("destY.at(i) = %lf\n", destY.at(i));	
						}
						else
						{
							curveAvailable = false;
							printf("This Histogram can't make curve!!\n");
							return curveAvailable;
						}
						g->overlapInfo[j].curveInfo[k][i] = destY.at(i);
					}

					destX.clear();
					destY.clear();
					x_src.clear();
					y_src.clear();
				}

				for (int k = 0; k < 2; k++)
				{
					g->overlapInfo[j].curvePointMin[k] = 0;
					g->overlapInfo[j].curvePointMid[k] = 0;
					g->overlapInfo[j].curvePointMax[k] = 0;
				}
			}
			else// GAIN_CURVE_AUTO_RGB_COMPENSATION
			{

				//R
				for (int k = 0; k < 2; k++)
				{
					//printf("curve R: g->overlapInfo[j].curvePointMinR[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMinR[0], g->overlapInfo[j].curvePointMinR[1]);
					//printf("curve R: g->overlapInfo[j].curvePointMidR[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMidR[0], g->overlapInfo[j].curvePointMidR[1]);
					//printf("curve R: g->overlapInfo[j].curvePointMaxR[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMaxR[0], g->overlapInfo[j].curvePointMaxR[1]);
					if (k == 0)
					{
						if (g->overlapInfo[j].curvePointMinR[0] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMinR[0] + g->overlapInfo[j].curvePointMinR[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMinR[0]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMidR[0] + g->overlapInfo[j].curvePointMidR[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMidR[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMidR[0]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMaxR[0] + g->overlapInfo[j].curvePointMaxR[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMaxR[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMaxR[0]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMaxR[0] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[0]), (double)(g->overlapInfo[j].curvePointMin[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[0]), (double)(g->overlapInfo[j].curvePointMid[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[0]), (double)(g->overlapInfo[j].curvePointMax[1]));*/

					}
					else
					{
						if (g->overlapInfo[j].curvePointMinR[1] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMinR[0] + g->overlapInfo[j].curvePointMinR[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMinR[1]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMidR[0] + g->overlapInfo[j].curvePointMidR[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMidR[1]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMidR[1]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMaxR[0] + g->overlapInfo[j].curvePointMaxR[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMaxR[1]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMaxR[1]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMaxR[1] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[1]), (double)(g->overlapInfo[j].curvePointMin[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[1]), (double)(g->overlapInfo[j].curvePointMid[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[1]), (double)(g->overlapInfo[j].curvePointMax[0]));*/
					}

#if USE_SPLINE_CURVE
					monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
#else
					bezier_curve(VALUE_COUNT, x_src, y_src, destX, destY);
#endif
					//printf("curve R x_src.size(), x_src.size() = (%d, %d)\n", x_src.size(), y_src.size());
					//if (x_src.size() < 5)
					//{
					//	printf("curve R x_src=(%lf, %lf, %lf, %lf, %lf)\n", x_src[0], x_src[1], x_src[2], x_src[3], x_src[4]);
					//	printf("curve R y_src=(%lf, %lf, %lf, %lf, %lf)\n", y_src[0], y_src[1], y_src[2], y_src[3], y_src[4]);
					//}

					//printf("curve R\n");
					for (int i = 0; i < VALUE_COUNT; i++)
					{
						if (destY.at(i) >= 0 && destY.at(i) <= 255)
						{
							//printf("destY.at(%d) = %lf\n", i, destY.at(i));
						}
						else
						{
							curveAvailable = false;
							printf("This Histogram can't make curve: destY.at(%d) = %lf\n", i, destY.at(i));
							//for (int k = 0; k < VALUE_COUNT; k++)
							//{
							//	printf("destY.at(%d) = %lf\n", k, destY.at(k));
							//}
							return curveAvailable;
						}
						g->overlapInfo[j].curveInfoR[k][i] = destY.at(i);
					}

					destX.clear();
					destY.clear();
					x_src.clear();
					y_src.clear();
				}

				for (int k = 0; k < 2; k++)
				{
					g->overlapInfo[j].curvePointMinR[k] = 0;
					g->overlapInfo[j].curvePointMidR[k] = 0;
					g->overlapInfo[j].curvePointMaxR[k] = 0;
				}

				//G
				for (int k = 0; k < 2; k++)
				{
					//printf("curve G: g->overlapInfo[j].curvePointMinG[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMinG[0], g->overlapInfo[j].curvePointMinG[1]);
					//printf("curve G: g->overlapInfo[j].curvePointMidG[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMidG[0], g->overlapInfo[j].curvePointMidG[1]);
					//printf("curve G: g->overlapInfo[j].curvePointMaxG[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMaxG[0], g->overlapInfo[j].curvePointMaxG[1]);
					if (k == 0)
					{
						if (g->overlapInfo[j].curvePointMinG[0] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMinG[0] + g->overlapInfo[j].curvePointMinG[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMinG[0]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMidG[0] + g->overlapInfo[j].curvePointMidG[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMidG[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMidG[0]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMaxG[0] + g->overlapInfo[j].curvePointMaxG[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMaxG[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMaxG[0]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMaxG[0] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[0]), (double)(g->overlapInfo[j].curvePointMin[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[0]), (double)(g->overlapInfo[j].curvePointMid[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[0]), (double)(g->overlapInfo[j].curvePointMax[1]));*/

					}
					else
					{
						if (g->overlapInfo[j].curvePointMinG[1] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMinG[0] + g->overlapInfo[j].curvePointMinG[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMinG[1]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMidG[0] + g->overlapInfo[j].curvePointMidG[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMidG[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMidG[1]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMaxG[0] + g->overlapInfo[j].curvePointMaxG[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMaxG[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMaxG[1]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMaxG[1] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[1]), (double)(g->overlapInfo[j].curvePointMin[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[1]), (double)(g->overlapInfo[j].curvePointMid[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[1]), (double)(g->overlapInfo[j].curvePointMax[0]));*/
					}

#if USE_SPLINE_CURVE
					monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
#else
					bezier_curve(VALUE_COUNT, x_src, y_src, destX, destY);
#endif
					//printf("curve G x_src.size(), x_src.size() = (%d, %d)\n", x_src.size(), y_src.size());
					//if (x_src.size() < 5)
					//{
					//	printf("curve G x_src=(%lf, %lf, %lf, %lf, %lf)\n", x_src[0], x_src[1], x_src[2], x_src[3], x_src[4]);
					//	printf("curve G y_src=(%lf, %lf, %lf, %lf, %lf)\n", y_src[0], y_src[1], y_src[2], y_src[3], y_src[4]);
					//}

					//printf("curve G\n");
					for (int i = 0; i < VALUE_COUNT; i++)
					{
						if (destY.at(i) >= 0 && destY.at(i) <= 255)
						{
							//printf("destY.at(%d) = %lf\n", i, destY.at(i));
						}
						else
						{
							curveAvailable = false;
							printf("This Histogram can't make curve: destY.at(%d) = %lf\n", i, destY.at(i));
							//for (int k = 0; k < VALUE_COUNT; k++)
							//{
							//	printf("destY.at(%d) = %lf\n", k, destY.at(k));
							//}
							return curveAvailable;
						}
						g->overlapInfo[j].curveInfoG[k][i] = destY.at(i);
					}

					destX.clear();
					destY.clear();
					x_src.clear();
					y_src.clear();

				}
				for (int k = 0; k < 2; k++)
				{
					g->overlapInfo[j].curvePointMinG[k] = 0;
					g->overlapInfo[j].curvePointMidG[k] = 0;
					g->overlapInfo[j].curvePointMaxG[k] = 0;
				}

				//B
				for (int k = 0; k < 2; k++)
				{
					//printf("curve B: g->overlapInfo[j].curvePointMinB[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMinB[0], g->overlapInfo[j].curvePointMinB[1]);
					//printf("curve B: g->overlapInfo[j].curvePointMidB[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMidB[0], g->overlapInfo[j].curvePointMidB[1]);
					//printf("curve B: g->overlapInfo[j].curvePointMaxB[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMaxB[0], g->overlapInfo[j].curvePointMaxB[1]);
					if (k == 0)
					{
						if (g->overlapInfo[j].curvePointMinB[0] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMinB[0] + g->overlapInfo[j].curvePointMinB[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMinB[0]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMidB[0] + g->overlapInfo[j].curvePointMidB[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMidB[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMidB[0]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMaxB[0] + g->overlapInfo[j].curvePointMaxB[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMaxB[0]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMaxB[0]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMaxB[0] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
						}

						/*printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[0]), (double)(g->overlapInfo[j].curvePointMin[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[0]), (double)(g->overlapInfo[j].curvePointMid[1]));
						printf("JW makeCurve0: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[0]), (double)(g->overlapInfo[j].curvePointMax[1]));*/

					}
					else
					{
						//printf("curve B: g->overlapInfo[j].curvePointMinB[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMinB[0], g->overlapInfo[j].curvePointMinB[1]);
						//printf("curve B: g->overlapInfo[j].curvePointMidB[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMidB[0], g->overlapInfo[j].curvePointMidB[1]);
						//printf("curve B: g->overlapInfo[j].curvePointMaxB[0], [1] = (%d, %d)\n", g->overlapInfo[j].curvePointMaxB[0], g->overlapInfo[j].curvePointMaxB[1]);
						if (g->overlapInfo[j].curvePointMinB[1] > 0)
						{
							x_src.push_back(0.f);
							y_src.push_back(0.f);
						}
						double mid = (g->overlapInfo[j].curvePointMinB[0] + g->overlapInfo[j].curvePointMinB[1]) / 2.f;
						x_src.push_back((double)(g->overlapInfo[j].curvePointMinB[1]));
						y_src.push_back(mid);

						mid = (g->overlapInfo[j].curvePointMidB[0] + g->overlapInfo[j].curvePointMidB[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMidB[1]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMidB[1]));
							y_src.push_back(mid);
						}

						mid = (g->overlapInfo[j].curvePointMaxB[0] + g->overlapInfo[j].curvePointMaxB[1]) / 2.f;
						//if (x_src.at(x_src.size() - 1) != (double)(g->overlapInfo[j].curvePointMaxB[1]) && y_src.at(y_src.size() - 1) != mid)
						{
							x_src.push_back((double)(g->overlapInfo[j].curvePointMaxB[1]));
							y_src.push_back(mid);
						}

						if (g->overlapInfo[j].curvePointMaxB[1] < 255)
						{
							x_src.push_back(255.f);
							y_src.push_back(255.f);
							//printf("curve B 255 pushed\n");
						}
						/*printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMin)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMin[1]), (double)(g->overlapInfo[j].curvePointMin[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMid)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMid[1]), (double)(g->overlapInfo[j].curvePointMid[0]));
						printf("JW makeCurve1: (double)(g->overlapInfo[j].curvePointMax)=(%lf, %lf)\n", (double)(g->overlapInfo[j].curvePointMax[1]), (double)(g->overlapInfo[j].curvePointMax[0]));*/
					}

#if USE_SPLINE_CURVE
					monotonic_cubic_Hermite_spline(VALUE_COUNT, x_src, y_src, destX, destY);
#else
					bezier_curve(VALUE_COUNT, x_src, y_src, destX, destY);
#endif
					//printf("curve B x_src.size(), x_src.size() = (%d, %d)\n", x_src.size(), y_src.size());
					//if (x_src.size() < 5)
					//{
					//	printf("curve B x_src=(%lf, %lf, %lf, %lf, %lf)\n", x_src[0], x_src[1], x_src[2], x_src[3], x_src[4]);
					//	printf("curve B y_src=(%lf, %lf, %lf, %lf, %lf)\n", y_src[0], y_src[1], y_src[2], y_src[3], y_src[4]);
					//}

					for (int i = 0; i < VALUE_COUNT; i++)
					{
						if (destY.at(i) >= 0 && destY.at(i) <= 255)
						{
							//printf("destY.at(%d) = %lf\n", i, destY.at(i));
						}
						else
						{
							curveAvailable = false;
							printf("This Histogram can't make curve\n");
							//for (int k = 0; k < VALUE_COUNT; k++)
							//{
							//	printf("destY.at(%d) = %lf\n", k, destY.at(k));
							//}
							return curveAvailable;
						}
						g->overlapInfo[j].curveInfoB[k][i] = destY.at(i);
					}

					destX.clear();
					destY.clear();
					x_src.clear();
					y_src.clear();

				}
				for (int k = 0; k < 2; k++)
				{
					g->overlapInfo[j].curvePointMinB[k] = 0;
					g->overlapInfo[j].curvePointMidB[k] = 0;
					g->overlapInfo[j].curvePointMaxB[k] = 0;
				}

			}

		}

	}

	//for (int i = 0; i < 256; i++)
	//	mCurveInfo[i] = (unsigned char)128;// / (float)255;

	//for (int i = 0; i <= 200; i++)
	//	printf("bigheadk, %d\n", mCurveInfo[i]);// = (uchar)destY.at(i);// / (float)255;
	//printf("JW makeCurve end\n");

	return curveAvailable;
}
bool ExposureCompensator::getDefaultColorTransformInformations(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx)
{
	//printf("JW 0000\n");
	if (!input->connected) return false;
	float t = QClock();
	//printf("calculatePixelAvg: time = %f\n", t);

	GainP g = new GainInfo;
	int currentMatch = -1, maxMatch = 0;
	int tempMatchCount = input->count() * 3;

	g->numImg = (int)(input->count());
	g->imgInfo = (ImageInfo*)malloc(g->numImg * sizeof(ImageInfo));
	g->overlapInfo = (OverlapInfo*)malloc(tempMatchCount * sizeof(OverlapInfo));
	g->matchCount = tempMatchCount;
	g->startIdx = 0;
	for (int i = 0; i < g->numImg; i++) {
		initImgInfo(&(g->imgInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}
	for (int i = 0; i < tempMatchCount; i++) {
		initMatchInfo(&(g->overlapInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}
	
	for (int i = 0; i < g->numImg; i++)
	{
		//printf("g->imgInfo[i].finalDiff = %lf\n", g->imgInfo[i].finalDiff);
		for (int k = 0; k < VALUE_COUNT; k++)
		{
			mCurveInfoR[i][k] = k;
			mCurveInfoG[i][k] = k;
			mCurveInfoB[i][k] = k;
		}
	}


	setGainDiff(g, input, GAIN_CURVE_AUTO_RGB_COMPENSATION);

	setLUT(input, g->numImg, GAIN_CURVE_AUTO_RGB_COMPENSATION);
}

bool ExposureCompensator::getNewColorTransformInformations(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx, int stereoIdx)
{
	if (!input->connected) return false;
	float t = QClock();
	//printf("calculatePixelAvg: time = %f\n", t);
	std::vector<cv::Mat> imgs;
	std::vector<cv::Mat> masks;
	std::vector<cv::Point2i> corners;
	std::vector<cv::Rect> rects;
	std::vector<cv::Mat> orgImgs, testImgs;

	GainP g = new GainInfo;

	int startIdx = 0;
	int numInput = input->count();

	if (stereoIdx >= 0)
	{
		startIdx = stereoIdx * 2;
		numInput = 2;
	}
	g->startIdx = startIdx;

	int currentMatch = -1, maxMatch = 0;
	int tempMatchCount = numInput * 3;

	g->numImg = (int)numInput;
	g->imgInfo = (ImageInfo*)malloc(numInput * sizeof(ImageInfo));
	g->overlapInfo = (OverlapInfo*)malloc(tempMatchCount * sizeof(OverlapInfo));
	g->matchCount = tempMatchCount;
	for (int i = 0; i < g->numImg; i++) {
		initImgInfo(&(g->imgInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}
	for (int i = 0; i < tempMatchCount; i++) {
		initMatchInfo(&(g->overlapInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}
	cv::Rect crop_rect;
	for (size_t i = 0; i < g->numImg; i++) {
		int idxIdx = i + startIdx;
		ComposeHelper& h = *((ComposeHelper*)helpers[idxIdx]);
		int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
		
		cv::Mat msk(rh, rw, CV_8UC1);
		cv::Mat m1 = cv::Mat::zeros(rh, rw, CV_8UC3);
		crop_rect.x = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_left;
		crop_rect.y = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_top;
		crop_rect.width = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_right - crop_rect.x;
		crop_rect.height = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_bottom - crop_rect.y;

		int wdt = input_tempData->inputs[idxIdx].width;
		int hgt = input_tempData->inputs[idxIdx].height;
		unsigned char* buf = (unsigned char*)input_tempData->inputs[idxIdx].buffers[buffer_idx[idxIdx]];
		cv::Mat org = cv::Mat(hgt, wdt, CV_8UC3, buf).clone();
		orgImgs.push_back(org);
		h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, crop_rect, SEAM_SCALE);
		//h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, SEAM_SCALE);

		int ksize = std::min(rw, rh)*0.1f;
		cv::GaussianBlur(m1, m1, cv::Size((ksize << 1) + 1, (ksize << 1) + 1), 0);
		imgs.push_back(m1);
		masks.push_back(msk);
		corners.push_back(cv::Point2i(rx, ry));
		rects.push_back(cv::Rect(rx, ry, rw, rh));
	}
	const size_t NUM_INPUT = g->numImg;
	bool has_mask[CAMERA_MAX];// = true;
	cv::Mat input_mask[CAMERA_MAX];

	for (int i = 0; i < NUM_INPUT; i++)
	{
		int idxIdx = i + startIdx;

		if (pts_param[idxIdx].sourcemask.rows <= 0 || pts_param[idxIdx].sourcemask.cols <= 0)
		{
			has_mask[idxIdx] = false;
		}
		else
		{
			input_mask[idxIdx] = pts_param[idxIdx].sourcemask.clone();
			cv::resize(input_mask[idxIdx], input_mask[idxIdx], cv::Size(input_mask[idxIdx].cols / SEAM_SCALE, input_mask[idxIdx].rows / SEAM_SCALE));
			has_mask[idxIdx] = true;
		}
	}
	//const int nCameras = input->count();
	const int tWDT = width / SEAM_SCALE;
	const int tHGT = height / SEAM_SCALE;

#define DEBUG_EC_CURVE 0
#if DEBUG_EC_CURVE
	std::vector<ColorTransformInfo> cTInfos(nCameras*nCameras);
#endif // 0

	for (int y = 0; y < tHGT; y++)
	{
		for (int x = 0; x < tWDT; x++)
		{
			int minDist = INT_MAX, minDistIndex = -1, minX = 0, minY = 0, dist = 0, rx, ry;

			std::vector<OverlapChannelInfo> overlapChannels;
			//calculate each overlap avg color for Gain Compensation
			for (size_t i = 0; i < NUM_INPUT; i++)
			{
				int idxIdx = i + startIdx;
				cv::Rect  tRect(corners[i].x, corners[i].y, rects[i].width, rects[i].height);
				if (containROI(tWDT, tHGT, tRect, cv::Point(x, y)))
				{
					ComposeHelper& h = *((ComposeHelper*)helpers[idxIdx]);
					crop_rect.x = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_left;
					crop_rect.y = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_top;
					crop_rect.width = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_right - crop_rect.x;
					crop_rect.height = ((CamInputImageFile*)(input->camIn(idxIdx)))->crop_bottom - crop_rect.y;

					int wdt = ((CamInputImageFile*)(input->camIn(idxIdx)))->width;
					int hgt = ((CamInputImageFile*)(input->camIn(idxIdx)))->height;

					if (has_mask[idxIdx])
					{
						dist = h.CalcDist(input_mask[idxIdx], wdt, hgt, x, y, crop_rect, SEAM_SCALE);
					}
					else
					{
						dist = h.CalcDist(wdt, hgt, x, y, crop_rect, SEAM_SCALE);
					}

					if (dist != INT_MAX)
					{
						OverlapChannelInfo info;
						info.ich = i;
						info.dist = dist;
						overlapChannels.push_back(info);
					}
				}
			}

			const int nOverlap = overlapChannels.size();

			if (nOverlap >= 2)
			{
				for (int f = 0; f < nOverlap; f++)
				{
					const int fch = overlapChannels[f].ich;
					for (int s = f + 1; s < nOverlap; s++)
					{
						const int sch = overlapChannels[s].ich;

						int fx = x - corners[fch].x;
						int fy = y - corners[fch].y;
						if (fx < 0)
							fx += tWDT;
						if (fx >= tWDT)
							fx -= tWDT;

						int sx = x - corners[sch].x;
						int sy = y - corners[sch].y;
						if (sx < 0)
							sx += tWDT;
						if (sx >= tWDT)
							sx -= tWDT;

						if ((masks.at(fch)).at<uchar>(fy, fx) == 0 || (masks.at(sch)).at<uchar>(sy, sx) == 0)
						{
							continue;
						}

						cv::Vec3b fcolor = (imgs.at(fch)).at<cv::Vec3b>(fy, fx);

						int tsx = sx, tsy = sy;
						cv::Vec3b scolor = (imgs.at(sch)).at<cv::Vec3b>(sy, sx);

#if 1			
						float deltaR = ((float)fcolor[0] - (float)scolor[0]);
						float deltaG = ((float)fcolor[1] - (float)scolor[1]);
						float deltaB = ((float)fcolor[2] - (float)scolor[2]);

						float delta = sqrt(deltaR*deltaR + deltaG*deltaG + deltaB*deltaB);
						if (delta > 221 || abs(deltaR - deltaG)>32 || abs(deltaG - deltaB)>32 || abs(deltaR - deltaB)>32) continue;
#endif // 0						
						if (currentMatch >= 0)//not first
						{
							for (int m = 0; m <= maxMatch; m++) {
								if ((g->overlapInfo[m].num[0] == fch && g->overlapInfo[m].num[1] == sch) || (g->overlapInfo[m].num[0] == sch && g->overlapInfo[m].num[1] == fch))
								{
									currentMatch = m;
									break;
								}
								else if (m == maxMatch) {
									maxMatch++;
									currentMatch = maxMatch;
									break;
								}

							}
						}
						else//once at first
							currentMatch = 0;

#if DEBUG_EC_CURVE
						cTInfos.at(currentMatch).push(fcolor, scolor);
						cTInfos.at(currentMatch).setchInfo(fch, sch);
#endif
						g->overlapInfo[currentMatch].HistR0[fcolor[0]]++;//R
						g->overlapInfo[currentMatch].HistG0[fcolor[1]]++;//G
						g->overlapInfo[currentMatch].HistB0[fcolor[2]]++;//B

						g->overlapInfo[currentMatch].HistR1[scolor[0]]++;//R'
						g->overlapInfo[currentMatch].HistG1[scolor[1]]++;//G'
						g->overlapInfo[currentMatch].HistB1[scolor[2]]++;//B'
						
						g->overlapInfo[currentMatch].avgCount++;
						g->overlapInfo[currentMatch].num[0] = fch;
						g->overlapInfo[currentMatch].num[1] = sch;
					}
				}
			}
		}
	}
#if DEBUG_EC_CURVE
	for (int kkk = 0; kkk < cTInfos.size(); kkk++)
	{
		cv::Mat graph = cv::Mat::zeros(cv::Size(1024, 1024), CV_8UC3);

		ColorTransformInfo ctinfo = cTInfos.at(kkk);
		for (int i = 0; i < ctinfo.size(); i++)
		{
			for (int c = 0; c < 256; c++)
			{
				for (int i = 0; i < ctinfo.deltaR[c].size(); i++)
				{
					int dR = 256 + ctinfo.deltaR[c].at(i);
					cv::circle(graph, cv::Point(i * 4, dR * 2), 3, CV_RGB(255, 0, 0), 1);
				}
				for (int i = 0; i < ctinfo.deltaG[c].size(); i++)
				{
					int dG = 256 + ctinfo.deltaG[c].at(i);
					cv::circle(graph, cv::Point(i * 4, dG * 2), 3, CV_RGB(0, 255, 0), 1);
				}
				for (int i = 0; i < ctinfo.deltaB[c].size(); i++)
				{
					int dB = 256 + ctinfo.deltaB[c].at(i);
					cv::circle(graph, cv::Point(i * 4, dB * 2), 3, CV_RGB(0, 0, 255), 1);
				}
			}
		}

		cv::line(graph, cv::Point(0, 24), cv::Point(1024, 24), CV_RGB(0, 255, 0), 1);
		cv::line(graph, cv::Point(1000, 0), cv::Point(1000, 1024), CV_RGB(0, 255, 0), 1);

		for (int i = 0; i < 512; i++)
		{
			if (i % 10 == 0)
			{
				if (i < 256)
				{
					std::string texth = std::to_string(i);
					cv::putText(graph, texth, cv::Point(i * 4 + 2, 12), CV_FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 255, 0));
				}
				std::string textv = std::to_string(i + 6 - 256);
				cv::putText(graph, textv, cv::Point(970, 1024 - (i + 6) * 2), CV_FONT_HERSHEY_SIMPLEX, 0.3, CV_RGB(0, 255, 0));
			}

		}

#if 1
		cv::Mat fchimg, schimg;
		cv::cvtColor(imgs.at(ctinfo.getfch()), fchimg, CV_RGB2BGR);
		cv::cvtColor(imgs.at(ctinfo.getsch()), schimg, CV_RGB2BGR);

		std::string idx = std::to_string(kkk) + "_";
		std::string path("../result/");
		std::string ext(".jpg");

		std::string s_fch("fch_"); s_fch = path + idx + s_fch + ext;
		std::string s_sch("sch_"); s_sch = path + idx + s_sch + ext;

		std::string s_fmsk("fmsk_"); s_fmsk = path + idx + s_fmsk + ext;
		std::string s_smsk("smsk_"); s_smsk = path + idx + s_smsk + ext;

		std::string s_graph("graph_"); s_graph = path + idx + s_graph + ext;

		cv::imwrite(s_fch, fchimg);
		cv::imwrite(s_sch, schimg);

		cv::imwrite(s_fmsk, masks.at(ctinfo.getfch()));
		cv::imwrite(s_smsk, masks.at(ctinfo.getsch()));

		cv::imwrite(s_graph, graph);
#endif // 1

	}
#endif // 0
	
	g->matchCount = maxMatch + 1;
	int sum = 0;
	for (int i = 0; i < g->matchCount; i++) {
		sum += g->overlapInfo[i].avgCount;
	}
	int avg = sum / g->matchCount;

	int resetCount = 0;
	int h_range = (int)(0.3f*VALUE_COUNT*0.5f);
	for (int i = 0; i < g->matchCount; i++) 
	{
		if (g->overlapInfo[i].avgCount < avg * MATCHING_THRESHOLD)
		{
			initMatchInfo(&(g->overlapInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
			resetCount++;
			printf("JW matchInfo removed!!\n");
		}
		else{
#if 1//blur histogram
			int *p_histo[3][2];
			p_histo[0][0] = g->overlapInfo[i].HistR0;
			p_histo[1][0] = g->overlapInfo[i].HistG0;
			p_histo[2][0] = g->overlapInfo[i].HistB0;

			p_histo[0][1] = g->overlapInfo[i].HistR1;
			p_histo[1][1] = g->overlapInfo[i].HistG1;
			p_histo[2][1] = g->overlapInfo[i].HistB1;

			//int cntsum = 0;

			for (int ii = 0; ii < 2; ii++)
			{
				for (int jj = 0; jj < 3; jj++)
				{
					int T_histo[VALUE_COUNT];
					memcpy(T_histo, p_histo[jj][ii], VALUE_COUNT*sizeof(int));

					//histogram interpolation
					for (int pp = 0; pp < VALUE_COUNT; pp++)
					{
						int avg = 0;
						int sr = pp - h_range;
						int er = pp + h_range;
						sr = std::max(0, sr);
						er = std::min(er, VALUE_COUNT);
						float cnt = er - sr;
						float wsum = 0.0f;
						for (int rr = sr; rr < er; rr++)
						{
							float weight = (1.0f - std::abs(pp - rr) / (float)(h_range));
							avg += T_histo[rr] * weight;
							wsum += weight;
						}
						avg = ((avg / wsum) + 0.5f);
						p_histo[jj][ii][pp] = avg;
					}
				}
			}
#endif // 1
			g->imgInfo[g->overlapInfo[i].num[0]].pixelcnt += g->overlapInfo[i].avgCount;
			g->imgInfo[g->overlapInfo[i].num[1]].pixelcnt += g->overlapInfo[i].avgCount;
		}		
	}
	printf("[calculate pixel] took : %f\n", QClock() - t);

	bool curveAvailable = true;
	initTempCurveInfo(g, g->numImg, GAIN_CURVE_AUTO_RGB_COMPENSATION);

#define WRITE_EC_CURVE 0


	for (int i = 0; i < g->numImg; i++)
	{
		if (!(curveInitialPointRGB(g, width / SEAM_SCALE, height / SEAM_SCALE, i) && genCurveRGB(g, i)))
			return false;

		optiCurveRGB(g, i);

#if WRITE_EC_CURVE
		for (int j= 0; j< g->numImg; j++)
		{
			int jj = (j == g->numImg - 1 ? 0 : j + 1);
			std::string idx = std::to_string(jj) + "_";
			std::string idx2 = "_"+std::to_string(i);
			std::string path("../result/");
			std::string ext(".png");
			std::string curvestr("RGB_Curve"); curvestr = path + idx + curvestr + idx2 + ext;
			cv::Mat curveimg = cv::Mat::zeros(cv::Size(640, 640), CV_8UC3);
			curveimg.setTo(cv::Scalar::all(128));

			uchar** curveInfoR = g->thisCurveInfoR[i];
			uchar** curveInfoG = g->thisCurveInfoG[i];
			uchar** curveInfoB = g->thisCurveInfoB[i];

			for (int k = 0; k < VALUE_COUNT; k++)
			{
				const int colorval = 192;
				int r = curveInfoR[j][k];
				int g = curveInfoG[j][k];
				int b = curveInfoB[j][k];

				cv::circle(curveimg, cv::Point(k * 2 + 5, r * 2 + 5), 1, CV_RGB(colorval, colorval * (r == g), colorval * (r == b)), -1);
				cv::circle(curveimg, cv::Point(k * 2 + 5, g * 2 + 5), 1, CV_RGB(colorval*(r == g), colorval, 255 * (g == b)), -1);
				cv::circle(curveimg, cv::Point(k * 2 + 5, b * 2 + 5), 1, CV_RGB(colorval*(r == b), colorval * (g == b), colorval), -1);
			}

			cv::flip(curveimg, curveimg, 0);

			imwrite(curvestr, curveimg);
		}
#endif
	}
	//printf("JW 3333: gainMode = %d\n", gainMode);
	for (int i = 0; i < g->numImg; i++)
	{
#if WRITE_EC_CURVE
		std::string idx = std::to_string(i) + "_";
		std::string path("../result/");
		std::string ext(".png");

		std::string imgstr("image"); imgstr = path + idx + imgstr + ext;
		std::string imgECstr("imageEC"); imgECstr = path + idx + imgECstr + ext;
		std::string curvestr("RGB_Curve"); curvestr = path + idx + curvestr + ext;

		cv::Mat curveimg = cv::Mat::zeros(cv::Size(640, 640), CV_8UC3);
		curveimg.setTo(cv::Scalar::all(128));

#endif

		for (int k = 0; k < VALUE_COUNT; k++)
		{
			if (i == 0)
			{
				mCurveInfoR[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoR[k];
				mCurveInfoG[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoG[k];
				mCurveInfoB[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoB[k];
			}
			else
			{
				mCurveInfoR[i][k] = g->imgInfo[i - 1].finalCurveInfoR[k];
				mCurveInfoG[i][k] = g->imgInfo[i - 1].finalCurveInfoG[k];
				mCurveInfoB[i][k] = g->imgInfo[i - 1].finalCurveInfoB[k];
			}

#if WRITE_EC_CURVE
			const int colorval = 192;
			int r = mCurveInfoR[i][k];
			int g = mCurveInfoG[i][k];
			int b = mCurveInfoB[i][k];

			cv::circle(curveimg, cv::Point(k * 2 + 5, r * 2 + 5), 1, CV_RGB(colorval, colorval * (r == g), colorval * (r == b)), -1);
			cv::circle(curveimg, cv::Point(k * 2 + 5, g * 2 + 5), 1, CV_RGB(colorval*(r == g), colorval, 255 * (g == b)), -1);
			cv::circle(curveimg, cv::Point(k * 2 + 5, b * 2 + 5), 1, CV_RGB(colorval*(r == b), colorval * (g == b), colorval), -1);
#endif
		}

#if WRITE_EC_CURVE
		cv::flip(curveimg, curveimg, 0);

		cv::Mat outimg;
		int imgidx = i;
		if (i == 0) imgidx = g->numImg - 1;
		else imgidx = i - 1;
		cv::cvtColor(orgImgs.at(imgidx), outimg, CV_RGB2BGR);
		cv::Mat outimgEC = outimg.clone();

		for (int row = 0; row < outimgEC.rows; row++)
		{
			for (int col = 0; col < outimgEC.cols; col++)
			{
				cv::Vec3b bgr = outimgEC.at<cv::Vec3b>(row, col);
				outimgEC.at<cv::Vec3b>(row, col) = cv::Vec3b(mCurveInfoB[i][bgr[0]], mCurveInfoG[i][bgr[1]], mCurveInfoR[i][bgr[2]]);
			}
		}

		imwrite(imgstr, outimg);
		imwrite(imgECstr, outimgEC);
		imwrite(curvestr, curveimg);
#endif
	}
	
	destroyCurveData(g, GAIN_CURVE_AUTO_RGB_COMPENSATION);

	setGainDiff(g, input, GAIN_CURVE_AUTO_RGB_COMPENSATION);

	setLUT(input, g->numImg, GAIN_CURVE_AUTO_RGB_COMPENSATION);
	printf("Gain took : %f, gainMode =  %d\n", QClock() - t, GAIN_CURVE_AUTO_RGB_COMPENSATION);
	return true;
}
bool ExposureCompensator::getColorTransformInformations(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx)
{
	//printf("JW 0000\n");
	if (!input->connected) return false;
	float t = QClock();
	//printf("calculatePixelAvg: time = %f\n", t);
	
	std::vector<cv::Mat> imgs;
	std::vector<cv::Mat> masks;
	std::vector<cv::Point2i> corners;
	std::vector<cv::Rect> rects;
	std::vector<cv::Mat> orgImgs, testImgs;

	GainP g = new GainInfo;
	int currentMatch = -1, maxMatch = 0;
	int tempMatchCount = input->count() * 3;

	g->numImg = (int)(input->count());
	g->imgInfo = (ImageInfo*)malloc(g->numImg * sizeof(ImageInfo));
	g->overlapInfo = (OverlapInfo*)malloc(tempMatchCount * sizeof(OverlapInfo));
	g->matchCount = tempMatchCount;
	for (int i = 0; i < g->numImg; i++) {
		initImgInfo(&(g->imgInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}
	for (int i = 0; i < tempMatchCount; i++) {
		initMatchInfo(&(g->overlapInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}

	cv::Rect crop_rect;
	for (size_t i = 0; i<input->count(); i++) {
		ComposeHelper& h = *((ComposeHelper*)helpers[i]);
		int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
		
		cv::Mat msk(rh, rw, CV_8UC1);
		cv::Mat m1 = cv::Mat::zeros(rh, rw, CV_8UC3);

		crop_rect.x = ((CamInputImageFile*)(input->camIn(i)))->crop_left;
		crop_rect.y = ((CamInputImageFile*)(input->camIn(i)))->crop_top;
		crop_rect.width = ((CamInputImageFile*)(input->camIn(i)))->crop_right - crop_rect.x;
		crop_rect.height = ((CamInputImageFile*)(input->camIn(i)))->crop_bottom - crop_rect.y;

		int wdt = input_tempData->inputs[i].width;
		int hgt = input_tempData->inputs[i].height;
		unsigned char* buf = (unsigned char*)input_tempData->inputs[i].buffers[buffer_idx[i]];

		cv::Mat org = cv::Mat(hgt, wdt, CV_8UC3, buf).clone();
		h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, crop_rect, SEAM_SCALE);
		
		//h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, SEAM_SCALE);
#if 1
		int minside = std::min(m1.cols, m1.rows);
		int ksize = ((int)(minside*0.15)) * 2 + 1;
		ksize = std::max(3, ksize);
		cv::GaussianBlur(m1, m1, cv::Size(ksize, ksize), 0, 0);

		//cv::imshow("m1", m1);
		//cv::waitKey(0);
#endif // 1

		imgs.push_back(m1);
		masks.push_back(msk);
		corners.push_back(cv::Point2i(rx, ry));
		rects.push_back(cv::Rect(rx, ry, rw, rh));
	}

	const size_t NUM_INPUT = input_tempData->mNum_camera;
	bool has_mask[CAMERA_MAX];// = true;
	cv::Mat input_mask[CAMERA_MAX];

	for (int i = 0; i < NUM_INPUT; i++)
	{
		if (pts_param[i].sourcemask.rows <= 0 || pts_param[i].sourcemask.cols <= 0)
		{
			has_mask[i] = false;
		}
		else
		{
			input_mask[i] = pts_param[i].sourcemask.clone();
			cv::resize(input_mask[i], input_mask[i], cv::Size(input_mask[i].cols / SEAM_SCALE, input_mask[i].rows / SEAM_SCALE));
			has_mask[i] = true;
		}
	}

	const int nCameras = input->count();
	const int tWDT = width / SEAM_SCALE;
	const int tHGT = height / SEAM_SCALE;

	typedef struct _overlapInfo{
		int ich;
		int dist;
	}overlapInfo;

	for (int y = 0; y < tHGT; y++) 
	{
		for (int x = 0; x < tWDT; x++) 
		{
			int minDist = INT_MAX, minDistIndex = -1, minX = 0, minY = 0, dist = 0, rx, ry;

			std::vector<overlapInfo> overlapChannels;
			//calculate each overlap avg color for Gain Compensation
			for (size_t i = 0; i < nCameras; i++)
			{
				cv::Rect  tRect(corners[i].x, corners[i].y, rects[i].width, rects[i].height);
				if (containROI(tWDT, tHGT, tRect, cv::Point(x, y)))
				{
					ComposeHelper& h = *((ComposeHelper*)helpers[i]);
					crop_rect.x = ((CamInputImageFile*)(input->camIn(i)))->crop_left;
					crop_rect.y = ((CamInputImageFile*)(input->camIn(i)))->crop_top;
					crop_rect.width = ((CamInputImageFile*)(input->camIn(i)))->crop_right - crop_rect.x;
					crop_rect.height = ((CamInputImageFile*)(input->camIn(i)))->crop_bottom - crop_rect.y;

					int wdt = ((CamInputImageFile*)(input->camIn(i)))->width;
					int hgt = ((CamInputImageFile*)(input->camIn(i)))->height;

					if (has_mask[i])
					{
						dist = h.CalcDist(input_mask[i], wdt, hgt, x, y, crop_rect, SEAM_SCALE);
					}
					else
					{
						dist = h.CalcDist(wdt, hgt, x, y, crop_rect, SEAM_SCALE);
					}

					if (dist != INT_MAX)
					{
						overlapInfo info;
						info.ich = i;
						info.dist = dist;
						overlapChannels.push_back(info);
					}
				}
			}

			const int nOverlap = overlapChannels.size();

			if (nOverlap >= 2)
			{
				for (int f = 0; f < nOverlap; f++)
				{
					const int fch = overlapChannels[f].ich;
					for (int s = f+1; s < nOverlap; s++)
					{
						const int sch = overlapChannels[s].ich;

						int fx = x - corners[fch].x;
						int fy = y - corners[fch].y;
						if (fx < 0)
							fx += tWDT;
						if (fx >= tWDT)
							fx -= tWDT;

						int sx = x - corners[sch].x;
						int sy = y - corners[sch].y;
						if (sx < 0)
							sx += tWDT;
						if (sx >= tWDT)
							sx -= tWDT;

						if ((masks.at(fch)).at<uchar>(fy, fx) == 0 || (masks.at(sch)).at<uchar>(sy, sx) == 0)
						{
							continue;
						}

						cv::Vec3b fcolor = (imgs.at(fch)).at<cv::Vec3b>(fy, fx);
						
						int tsx = sx, tsy = sy;
						cv::Vec3b scolor = (imgs.at(sch)).at<cv::Vec3b>(sy, sx);

#if 0
						cv::Vec3d delta;
						cv::absdiff(scolor, fcolor, delta);
						
						cv::Vec3d deltaratio;

						deltaratio[0] = (delta[0] + 0.1) / (delta[1] + 0.1);
						deltaratio[1] = (delta[1] + 0.1) / (delta[1] + 0.1);
						deltaratio[2] = (delta[2] + 0.1) / (delta[1] + 0.1);

						for (int ch = 0; ch < 3; ch++)
						{
							if (scolor[ch] < fcolor[ch])
								scolor[ch] = fcolor[ch] - delta[1]*log2(deltaratio[ch]+1.0);
							else
								scolor[ch] = fcolor[ch] + delta[1]*log2(deltaratio[ch]+1.0);
						}
#endif // 0						
						if (currentMatch >= 0)//not first
						{
							for (int m = 0; m <= maxMatch; m++) {
								if ((g->overlapInfo[m].num[0] == fch && g->overlapInfo[m].num[1] == sch) || (g->overlapInfo[m].num[0] == sch && g->overlapInfo[m].num[1] == fch))
								{
									currentMatch = m;
									break;
								}
								else if (m == maxMatch) {
									maxMatch++;
									currentMatch = maxMatch;
									break;
								}

							}
						}
						else//once at first
							currentMatch = 0;

						g->overlapInfo[currentMatch].HistR0[fcolor[0]]++;
						g->overlapInfo[currentMatch].HistG0[fcolor[1]]++;
						g->overlapInfo[currentMatch].HistB0[fcolor[2]]++;

						g->overlapInfo[currentMatch].num[0] = fch;


						g->overlapInfo[currentMatch].HistR1[scolor[0]]++;
						g->overlapInfo[currentMatch].HistG1[scolor[1]]++;
						g->overlapInfo[currentMatch].HistB1[scolor[2]]++;

						g->overlapInfo[currentMatch].avgCount++;
						g->overlapInfo[currentMatch].num[1] = sch;
					}
				}
			}		
		}
	}
	g->matchCount = maxMatch+1;
	int sum = 0;
	for (int i = 0; i < g->matchCount; i++) {
		sum += g->overlapInfo[i].avgCount;
	}
	int avg = sum / g->matchCount;

	int resetCount = 0;
	for (int i = 0; i < g->matchCount; i++) {
		if (g->overlapInfo[i].avgCount < avg * MATCHING_THRESHOLD)
		{
			initMatchInfo(&(g->overlapInfo[i]), GAIN_CURVE_AUTO_RGB_COMPENSATION);
			resetCount++;
			printf("JW matchInfo removed!!\n");
		}
	}
	printf("[calculate pixel] took : %f\n", QClock() - t);
	
	bool curveAvailable = true;
	initTempCurveInfo(g, g->numImg, GAIN_CURVE_AUTO_RGB_COMPENSATION);

	for (int i = 0; i < g->numImg; i++)
	{
		calculateCurvePoint(g, width / SEAM_SCALE, height / SEAM_SCALE, GAIN_CURVE_AUTO_RGB_COMPENSATION);
		curveAvailable = makeAutoCurve(g, g->numImg, GAIN_CURVE_AUTO_RGB_COMPENSATION);
		if (!curveAvailable)
			return false;
		calculateCurveDiff(g, i, GAIN_CURVE_AUTO_RGB_COMPENSATION);
	}
	//printf("JW 3333: gainMode = %d\n", gainMode);
	for (int i = 0; i < g->numImg; i++)
	{
		for (int k = 0; k < VALUE_COUNT; k++)
		{
			if (i == 0)
			{
				mCurveInfoR[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoR[k];
				mCurveInfoG[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoG[k];
				mCurveInfoB[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoB[k];
			}
			else
			{
				mCurveInfoR[i][k] = g->imgInfo[i - 1].finalCurveInfoR[k];
				mCurveInfoG[i][k] = g->imgInfo[i - 1].finalCurveInfoG[k];
				mCurveInfoB[i][k] = g->imgInfo[i - 1].finalCurveInfoB[k];
			}
		}
	}
	destroyCurveData(g, GAIN_CURVE_AUTO_RGB_COMPENSATION);

	setGainDiff(g, input, GAIN_CURVE_AUTO_RGB_COMPENSATION);

	setLUT(input, g->numImg, GAIN_CURVE_AUTO_RGB_COMPENSATION);
	printf("Gain took : %f, gainMode =  %d\n", QClock() - t, GAIN_CURVE_AUTO_RGB_COMPENSATION);
	return true;
}
bool ExposureCompensator::calculatePixelAvg(InputTempData* input_tempData, CamInputArray *input, PTSParam* pts_param, std::vector<ComposeHelper*> helpers, int* buffer_idx, int gainMode) {
	//printf("JW 0000\n");
	if (!input->connected) return false;
	float t = QClock();
	//printf("calculatePixelAvg: time = %f\n", t);

	//start init
	std::vector<cv::Mat> imgs;
	std::vector<cv::Mat> masks;
	std::vector<cv::Point2i> corners;
	std::vector<cv::Rect> rects;
	std::vector<cv::Mat> orgImgs, testImgs;

	GainP g = new GainInfo;
	int currentMatch = -1, maxMatch = 0;
	int tempMatchCount = input->count() * 3;

	g->numImg = (int)(input->count());
	g->imgInfo = (ImageInfo*)malloc(g->numImg * sizeof(ImageInfo));
	g->overlapInfo = (OverlapInfo*)malloc(tempMatchCount * sizeof(OverlapInfo));
	g->matchCount = tempMatchCount;
	for (int i = 0; i < g->numImg; i++) {
		initImgInfo(&(g->imgInfo[i]), gainMode);
	}
	for (int i = 0; i < tempMatchCount; i++) {
		initMatchInfo(&(g->overlapInfo[i]), gainMode);
	}
	//finish init

	cv::Rect crop_rect;
	for (size_t i = 0; i<input->count(); i++) {
		ComposeHelper& h = *((ComposeHelper*)helpers[i]);
		int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
		cv::Mat msk(rh, rw, CV_8UC1);
		cv::Mat m1 = cv::Mat::zeros(rh, rw, CV_8UC3);

		crop_rect.x = ((CamInputImageFile*)(input->camIn(i)))->crop_left;
		crop_rect.y = ((CamInputImageFile*)(input->camIn(i)))->crop_top;
		crop_rect.width = ((CamInputImageFile*)(input->camIn(i)))->crop_right - crop_rect.x;
		crop_rect.height = ((CamInputImageFile*)(input->camIn(i)))->crop_bottom - crop_rect.y;

		int wdt = input_tempData->inputs[i].width;
		int hgt = input_tempData->inputs[i].height;
		unsigned char* buf = (unsigned char*)input_tempData->inputs[i].buffers[buffer_idx[i]];

		cv::Mat org = cv::Mat(hgt, wdt, CV_8UC3, buf).clone();
		h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, crop_rect, SEAM_SCALE);
		//h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, SEAM_SCALE);

		if (gainMode == GAIN_YUV_COMPENSATION_HALF || gainMode == GAIN_YUV_COMPENSATION_FULL) {
			cv::cvtColor(m1, m1, CV_RGB2YUV);
		}
		imgs.push_back(m1);
		masks.push_back(msk);
		corners.push_back(cv::Point2i(rx, ry));
		rects.push_back(cv::Rect(rx, ry, rw, rh));
	}

	const size_t NUM_INPUT = input_tempData->mNum_camera;
	bool has_mask[CAMERA_MAX];// = true;
	cv::Mat input_mask[CAMERA_MAX];

	for (int i = 0; i < NUM_INPUT; i++)
	{
		if (pts_param[i].sourcemask.rows <= 0 || pts_param[i].sourcemask.cols <= 0)
		{
			has_mask[i] = false;
		}
		else
		{
			input_mask[i] = pts_param[i].sourcemask.clone();
			cv::resize(input_mask[i], input_mask[i], cv::Size(input_mask[i].cols / SEAM_SCALE, input_mask[i].rows / SEAM_SCALE));
			has_mask[i] = true;
		}
	}

	for (int y = 0; y < height / SEAM_SCALE; y++) {
		for (int x = 0; x < width / SEAM_SCALE; x++) {
			int minDist = INT_MAX, minDistIndex = -1, minX = 0, minY = 0, dist = 0, rx, ry;

			//calculate each overlap avg color for Gain Compensation
			for (size_t i = 0; i < input->count(); i++) {
				cv::Rect  tRect(corners[i].x, corners[i].y, rects[i].width, rects[i].height);
				if (containROI(width / SEAM_SCALE, height / SEAM_SCALE, tRect, cv::Point(x, y))) {
					ComposeHelper& h = *((ComposeHelper*)helpers[i]);

					crop_rect.x = ((CamInputImageFile*)(input->camIn(i)))->crop_left;
					crop_rect.y = ((CamInputImageFile*)(input->camIn(i)))->crop_top;
					crop_rect.width = ((CamInputImageFile*)(input->camIn(i)))->crop_right - crop_rect.x;
					crop_rect.height = ((CamInputImageFile*)(input->camIn(i)))->crop_bottom - crop_rect.y;

					int wdt = ((CamInputImageFile*)(input->camIn(i)))->width;
					int hgt = ((CamInputImageFile*)(input->camIn(i)))->height;

					if (has_mask[i])
					{
						dist = h.CalcDist(input_mask[i], wdt, hgt, x, y, crop_rect, SEAM_SCALE);
					}
					else
					{
						dist = h.CalcDist(wdt, hgt, x, y, crop_rect, SEAM_SCALE);
					}

					int value = -1;
					if (dist < minDist) {
						if (minDist != INT_MAX) {//intersect!!
							int dx = x - corners[i].x;
							int dy = y - corners[i].y;
							if (dx < 0)
								dx += (width / SEAM_SCALE);
							if (dx >= (width / SEAM_SCALE))
								dx -= (width / SEAM_SCALE);

							//any Image
							{
								//cv::Mat image = imgs.at(i);
								//cv::Mat mask = masks.at(i);

								if ((masks.at(i)).at<unsigned char>(dy, dx) == 0)
								{
									//printf("skip!!!\n");
									continue;
								}

								if (currentMatch >= 0)//not first
								{
									for (int m = 0; m <= maxMatch; m++) {
										if (g->overlapInfo[m].num[0] == i && g->overlapInfo[m].num[1] == minDistIndex)
										{
											currentMatch = m;
											break;
										}
										else if (m == maxMatch) {
											maxMatch++;
											currentMatch = maxMatch;
											break;
										}

									}
								}
								else//once at first
									currentMatch = 0;

								if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION || gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
								{
									int valueR = (imgs.at(i)).at<cv::Vec3b>(dy, dx)[0];
									int valueG = (imgs.at(i)).at<cv::Vec3b>(dy, dx)[1];
									int valueB = (imgs.at(i)).at<cv::Vec3b>(dy, dx)[2];
									if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
									{
										value = (valueR + valueG + valueB) / 3;
										g->overlapInfo[currentMatch].HistAVG0[value]++;
									}
									else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
									{
										g->overlapInfo[currentMatch].HistR0[valueR]++;
										g->overlapInfo[currentMatch].HistG0[valueG]++;
										g->overlapInfo[currentMatch].HistB0[valueB]++;
									}
								}
								else
								{
									g->overlapInfo[currentMatch].avgRGB[0][0] += (double)((imgs.at(i)).at<cv::Vec3b>(dy, dx)[0]);
									g->overlapInfo[currentMatch].avgRGB[0][1] += (double)((imgs.at(i)).at<cv::Vec3b>(dy, dx)[1]);
									g->overlapInfo[currentMatch].avgRGB[0][2] += (double)((imgs.at(i)).at<cv::Vec3b>(dy, dx)[2]);

								}
								g->overlapInfo[currentMatch].num[0] = i;

								//test
								//imgs.at(i).at<cv::Vec3b>(dy, dx) = 255;

								//cv::Mat previousImage = imgs.at(minDistIndex);

								int prev_dx = x - corners[minDistIndex].x;
								int prev_dy = y - corners[minDistIndex].y;
								if (prev_dx < 0)
									prev_dx += (width / SEAM_SCALE);
								if (prev_dx >= (width / SEAM_SCALE))
									prev_dx -= (width / SEAM_SCALE);

								if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION || gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
								{
									int valueR = (imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[0];
									int valueG = (imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[1];
									int valueB = (imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[2];
									if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
									{
										value = (valueR + valueG + valueB) / 3;
										g->overlapInfo[currentMatch].HistAVG1[value]++;
									}
									else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
									{
										g->overlapInfo[currentMatch].HistR1[valueR]++;
										g->overlapInfo[currentMatch].HistG1[valueG]++;
										g->overlapInfo[currentMatch].HistB1[valueB]++;
									}
								}
								else
								{
									g->overlapInfo[currentMatch].avgRGB[1][0] += (double)((imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[0]);
									g->overlapInfo[currentMatch].avgRGB[1][1] += (double)((imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[1]);
									g->overlapInfo[currentMatch].avgRGB[1][2] += (double)((imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[2]);

								}
								g->overlapInfo[currentMatch].avgCount++;
								g->overlapInfo[currentMatch].num[1] = minDistIndex;

								//test
								//imgs.at(minDistIndex).at<cv::Vec3b>(prev_dy, prev_dx) = 255;
							}
						}
						minDist = dist;
						minDistIndex = i;
					}
					else {
						if (dist != INT_MAX) {//intersect!!
							int dx = x - corners[i].x;
							int dy = y - corners[i].y;
							if (dx < 0)
								dx += (width / SEAM_SCALE);
							if (dx >= (width / SEAM_SCALE))
								dx -= (width / SEAM_SCALE);
							//any Image
							{
								//cv::Mat image = imgs.at(i);
								//cv::Mat mask = masks.at(i);

								if ((masks.at(i)).at<unsigned char>(dy, dx) == 0)
								{
									//printf("skip!!!\n");
									continue;
								}

								if (currentMatch >= 0)//not first
								{
									for (int m = 0; m <= maxMatch; m++) {
										if (g->overlapInfo[m].num[0] == i && g->overlapInfo[m].num[1] == minDistIndex)
										{
											currentMatch = m;
											break;
										}
										else if (m == maxMatch) {
											maxMatch++;
											currentMatch = maxMatch;
											break;
										}

									}
								}
								else//once at first
									currentMatch = 0;

								if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION || gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
								{
									int valueR = (imgs.at(i)).at<cv::Vec3b>(dy, dx)[0];
									int valueG = (imgs.at(i)).at<cv::Vec3b>(dy, dx)[1];
									int valueB = (imgs.at(i)).at<cv::Vec3b>(dy, dx)[2];
									if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
									{
										value = (valueR + valueG + valueB) / 3;
										g->overlapInfo[currentMatch].HistAVG0[value]++;
									}
									else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
									{
										g->overlapInfo[currentMatch].HistR0[valueR]++;
										g->overlapInfo[currentMatch].HistG0[valueG]++;
										g->overlapInfo[currentMatch].HistB0[valueB]++;
									}
								}
								else
								{
									g->overlapInfo[currentMatch].avgRGB[0][0] += (double)((imgs.at(i)).at<cv::Vec3b>(dy, dx)[0]);
									g->overlapInfo[currentMatch].avgRGB[0][1] += (double)((imgs.at(i)).at<cv::Vec3b>(dy, dx)[1]);
									g->overlapInfo[currentMatch].avgRGB[0][2] += (double)((imgs.at(i)).at<cv::Vec3b>(dy, dx)[2]);

								}
								g->overlapInfo[currentMatch].num[0] = i;

								//test
								//imgs.at(i).at<cv::Vec3b>(dy, dx) = 255;

								//cv::Mat previousImage = imgs.at(minDistIndex);

								int prev_dx = x - corners[minDistIndex].x;
								int prev_dy = y - corners[minDistIndex].y;
								if (prev_dx < 0)
									prev_dx += (width / SEAM_SCALE);
								if (prev_dx >= (width / SEAM_SCALE))
									prev_dx -= (width / SEAM_SCALE);

								if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION || gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
								{
									int valueR = (imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[0];
									int valueG = (imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[1];
									int valueB = (imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[2];
									if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
									{
										value = (valueR + valueG + valueB) / 3;
										g->overlapInfo[currentMatch].HistAVG1[value]++;
									}
									else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
									{
										g->overlapInfo[currentMatch].HistR1[valueR]++;
										g->overlapInfo[currentMatch].HistG1[valueG]++;
										g->overlapInfo[currentMatch].HistB1[valueB]++;
									}
								}
								else
								{
									g->overlapInfo[currentMatch].avgRGB[1][0] += (double)((imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[0]);
									g->overlapInfo[currentMatch].avgRGB[1][1] += (double)((imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[1]);
									g->overlapInfo[currentMatch].avgRGB[1][2] += (double)((imgs.at(minDistIndex)).at<cv::Vec3b>(prev_dy, prev_dx)[2]);

								}
								g->overlapInfo[currentMatch].avgCount++;
								g->overlapInfo[currentMatch].num[1] = minDistIndex;

							}
						}
					}
				}
			}
		}
	}
	int sum = 0;
	g->matchCount = maxMatch + 1;

	for (int i = 0; i <= g->matchCount; i++) {
		sum += g->overlapInfo[i].avgCount;
	}
	int avg = sum / (g->matchCount);

	int resetCount = 0;
	for (int i = 0; i < g->matchCount; i++) {
		if (g->overlapInfo[i].avgCount < avg * MATCHING_THRESHOLD)
		{
			initMatchInfo(&(g->overlapInfo[i]), gainMode);
			resetCount++;
			printf("JW matchInfo removed!!\n");
		}
	}

	printf("[calculate pixel] took : %f\n", QClock() - t);
	//curveMode = GAIN_CURVE_AUTO;//temp
	if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION || gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
	{
		bool curveAvailable = true;
		initTempCurveInfo(g, g->numImg, gainMode);

		for (int i = 0; i < g->numImg; i++)
		{
			calculateCurvePoint(g, width / SEAM_SCALE, height / SEAM_SCALE, gainMode);
			curveAvailable = makeAutoCurve(g, g->numImg, gainMode);
			if (!curveAvailable)
				return false;
			calculateCurveDiff(g, i, gainMode);
		}
		//printf("JW 3333: gainMode = %d\n", gainMode);
		for (int i = 0; i < g->numImg; i++)
		{
			for (int k = 0; k < VALUE_COUNT; k++)
			{
				if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
				{
					if (i == 0)
					{
						mCurveInfoR[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfo[k];
						mCurveInfoG[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfo[k];
						mCurveInfoB[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfo[k];
					}
					else
					{
						mCurveInfoR[i][k] = g->imgInfo[i - 1].finalCurveInfo[k];
						mCurveInfoG[i][k] = g->imgInfo[i - 1].finalCurveInfo[k];
						mCurveInfoB[i][k] = g->imgInfo[i - 1].finalCurveInfo[k];
					}


				}
				else
				{
					if (i == 0)
					{
						mCurveInfoR[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoR[k];
						mCurveInfoG[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoG[k];
						mCurveInfoB[i][k] = g->imgInfo[g->numImg - 1].finalCurveInfoB[k];
					}
					else
					{
						mCurveInfoR[i][k] = g->imgInfo[i - 1].finalCurveInfoR[k];
						mCurveInfoG[i][k] = g->imgInfo[i - 1].finalCurveInfoG[k];
						mCurveInfoB[i][k] = g->imgInfo[i - 1].finalCurveInfoB[k];
					}

				}
			}
		}
		destroyCurveData(g, gainMode);
	}
	else
	{
		setOptGainInfo(g, maxMatch, gainMode);

		//RunOptimizer_Data(g);
		//printf("calculatePixelAvg: animation_t = %lld\n", (long long)t);		
	}
	setGainDiff(g, input, gainMode);

	setLUT(input, g->numImg, gainMode);
	printf("Gain took : %f, gainMode =  %d\n", QClock() - t, gainMode);
	return true;

}
void ExposureCompensator::resetCurveInfo(int count)
{
	for (int i = 0; i < count; i++)
	{
		for (int k = 0; k < VALUE_COUNT; k++)
		{
			mCurveInfoR[i][k] = k;
			mCurveInfoG[i][k] = k;
			mCurveInfoB[i][k] = k;
		}
	}
}

std::vector<cv::Mat> ExposureCompensator::getCurve(InputTempData* input_tempData, CamInputArray *input, long long time_stamp, std::vector<std::vector<std::vector<cv::Point2d>>> curvePoints, int gainMode) {
	//printf("JW getCurve\n");
	std::vector<cv::Mat> result;
	int inputCount = (int)(input->count());

	initCurveInfo(inputCount);
	bool curveAvailable = makeManualCurve(result, curvePoints, NULL, inputCount, gainMode);
	if (!curveAvailable)
		return result;

	for (int i = 0; i < inputCount; i++)
	{
		CamInput& p = *(input->camIn(i));
		p.exposureCompensation.setExposureData(0, 0, 0, mCurveInfoR[i], mCurveInfoG[i], mCurveInfoB[i], time_stamp);
	}
	setLUT(input, inputCount, gainMode);//set lut
	return result;
}

void ExposureCompensator::destroyCurveData(GainInfo *g, int gainMode){
	for (int i = 0; i < g->matchCount; i++)
	{
		if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
		{
			free(g->overlapInfo[i].HistAVG0);
			free(g->overlapInfo[i].HistAVG1);

			free(g->overlapInfo[i].curveInfo[0]);
			free(g->overlapInfo[i].curveInfo[1]);

			free(g->overlapInfo[i].curveInfo);
		}
		else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
		{
			free(g->overlapInfo[i].HistR0);
			free(g->overlapInfo[i].HistG0);
			free(g->overlapInfo[i].HistB0);
			free(g->overlapInfo[i].HistR1);
			free(g->overlapInfo[i].HistG1);
			free(g->overlapInfo[i].HistB1);

			free(g->overlapInfo[i].curveInfoR[0]);
			free(g->overlapInfo[i].curveInfoR[1]);
			free(g->overlapInfo[i].curveInfoR);
			free(g->overlapInfo[i].curveInfoG[0]);
			free(g->overlapInfo[i].curveInfoG[1]);
			free(g->overlapInfo[i].curveInfoG);
			free(g->overlapInfo[i].curveInfoB[0]);
			free(g->overlapInfo[i].curveInfoB[1]);
			free(g->overlapInfo[i].curveInfoB);
		}
	}

	for (int i = 0; i < g->numImg; i++)
	{
		if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
			free(g->imgInfo[i].finalCurveInfo);
		else if (gainMode == GAIN_CURVE_AUTO_RGB_COMPENSATION)
		{
			free(g->imgInfo[i].finalCurveInfoR);
			free(g->imgInfo[i].finalCurveInfoG);
			free(g->imgInfo[i].finalCurveInfoB);
		}
	}
}
void ExposureCompensator::setLUT(CamInputArray *input, int inputCount, int gainMode, int startIdx){
	for (int k = 0; k < inputCount; k++)
	{
		int idx = startIdx + k;
		CamInput& p = *(input->camIn(idx));

		GLTexMask& tex = p.getTexMask();
		tex.setLut(mCurveInfoR[k], mCurveInfoG[k], mCurveInfoB[k], VALUE_COUNT, gainMode);
	}
}

bool ExposureCompensator::curveInitialPointRGB(GainInfo *g, int w, int h, int stage)
{
	//only AVG mode
	for (int j = 0; j < g->matchCount; j++)
	{
		if (g->overlapInfo[j].num[0] < 0 || g->overlapInfo[j].num[1] < 0)
			continue;

		int *histo[3][2];
		histo[0][0] = g->overlapInfo[j].HistR0;
		histo[0][1] = g->overlapInfo[j].HistR1;
		histo[1][0] = g->overlapInfo[j].HistG0;
		histo[1][1] = g->overlapInfo[j].HistG1;
		histo[2][0] = g->overlapInfo[j].HistB0;
		histo[2][1] = g->overlapInfo[j].HistB1;

		int *curvePTMin[3], *curvePTMid[3], *curvePTMax[3];
		curvePTMin[0] = g->overlapInfo[j].curvePointMinR;
		curvePTMid[0] = g->overlapInfo[j].curvePointMidR;
		curvePTMax[0] = g->overlapInfo[j].curvePointMaxR;

		curvePTMin[1] = g->overlapInfo[j].curvePointMinG;
		curvePTMid[1] = g->overlapInfo[j].curvePointMidG;
		curvePTMax[1] = g->overlapInfo[j].curvePointMaxG;

		curvePTMin[2] = g->overlapInfo[j].curvePointMinB;
		curvePTMid[2] = g->overlapInfo[j].curvePointMidB;
		curvePTMax[2] = g->overlapInfo[j].curvePointMaxB;

		int totalCnt = g->overlapInfo[j].avgCount;

		for (int ch = 0; ch < 3; ch++)
		{
			for (int k = 0; k < 2; k++)
			{
				int accusum = 0;

				totalCnt = 0;
				for (int i = 0; i < VALUE_COUNT; i++)
				{
					totalCnt += histo[ch][k][i];
				}
				
				float startOffset = (float)totalCnt * 0.3f;
				float midOffset = (float)totalCnt * 0.5f;
				float endOffset = (float)totalCnt * 0.7f;

				for (int i = 0; i < VALUE_COUNT; i++)
				{
					accusum += histo[ch][k][i];

					if (accusum <= startOffset && histo[ch][k][i] > 0)
					{
						curvePTMin[ch][k] = i;
					}

					if (accusum <= midOffset && histo[ch][k][i] > 0)
					{
						curvePTMid[ch][k] = i;
					}

					if (accusum <= endOffset && histo[ch][k][i] > 0)
					{
						curvePTMax[ch][k] = i;
					}
				}

				if (curvePTMid[ch][k] == curvePTMin[ch][k])
				{
					if (curvePTMax[ch][k] - curvePTMin[ch][k] >= 2)
					{
						int tidx = ((curvePTMin[ch][k] + curvePTMax[ch][k]) >> 1);

						while (histo[ch][k][tidx] == 0 && tidx < curvePTMax[ch][k])
						{
							tidx++;
						}
						curvePTMid[ch][k] = tidx;
					}
					else{
						int tidx = std::max(1, curvePTMin[ch][k]-1);

						while (histo[ch][k][tidx] == 0 && tidx > 0)
						{
							tidx--;
						}
						curvePTMid[ch][k] = tidx;
					}
				}
				
				if (curvePTMid[ch][k] == curvePTMax[ch][k])
				{
					if (curvePTMax[ch][k] - curvePTMin[ch][k] >= 2)
					{
						int tidx = ((curvePTMin[ch][k] + curvePTMax[ch][k]) >> 1);
						while (histo[ch][k][tidx] == 0 && tidx > curvePTMin[ch][k])
						{
							tidx--;
						}
						curvePTMid[ch][k] = tidx;
					}else{
						int tidx = std::min(254, curvePTMin[ch][k]+1);

						while (histo[ch][k][tidx] == 0 && tidx < 255)
						{
							tidx++;
						}
						curvePTMid[ch][k] = tidx;
					}
				}

				if(curvePTMid[ch][k] <= curvePTMin[ch][k] || curvePTMid[ch][k] >= curvePTMax[ch][k])
					return false;			

			}
		}
	}

	return true;
}
void ExposureCompensator::calculateCurvePoint(GainInfo *g, int w, int h, int gainMode)
{
	//only AVG mode
	for (int j = 0; j < g->matchCount; j++)
	{
		//cv::Mat test0 = cv::Mat::zeros(VALUE_COUNT * 2, VALUE_COUNT * 2, CV_8UC3);
		//cv::Mat test1 = cv::Mat::zeros(VALUE_COUNT * 2, VALUE_COUNT * 2, CV_8UC3);
		if (g->overlapInfo[j].num[0] < 0 || g->overlapInfo[j].num[1] < 0)
			continue;
		int sum0 = 0, sum1 = 0, sumR0 = 0, sumG0 = 0, sumB0 = 0, sumR1 = 0, sumG1 = 0, sumB1 = 0, midCount0 = 0, midCount1 = 0,
			midCountR0 = 0, midCountG0 = 0, midCountB0 = 0, midCountR1 = 0, midCountG1 = 0, midCountB1 = 0;
		for (int i = 0; i < VALUE_COUNT; i++)
		{
			if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
			{
				sum0 += g->overlapInfo[j].HistAVG0[i];
				sum1 += g->overlapInfo[j].HistAVG1[i];
			}
			else
			{
				sumR0 += g->overlapInfo[j].HistR0[i];
				sumG0 += g->overlapInfo[j].HistG0[i];
				sumB0 += g->overlapInfo[j].HistB0[i];
				sumR1 += g->overlapInfo[j].HistR1[i];
				sumG1 += g->overlapInfo[j].HistG1[i];
				sumB1 += g->overlapInfo[j].HistB1[i];
			}

		}
		//cv::imshow("hist0", test0);
		//cv::imshow("hist1", test1);
		if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
		{
			midCount0 = sum0 / 2;
			midCount1 = sum1 / 2;

			sum0 = 0;
			sum1 = 0;
		}
		else
		{
			midCountR0 = sumR0 / 2;
			midCountG0 = sumG0 / 2;
			midCountB0 = sumB0 / 2;
			midCountR1 = sumR1 / 2;
			midCountG1 = sumG1 / 2;
			midCountB1 = sumB1 / 2;

			sumR0 = 0;
			sumG0 = 0;
			sumB0 = 0;
			sumR1 = 0;
			sumG1 = 0;
			sumB1 = 0;
		}
		//printf("JW calculateCurvePoint: g->overlapInfo[j].num=(%d, %d)\n", g->overlapInfo[j].num[0], g->overlapInfo[j].num[1]);

		for (int i = 0; i < VALUE_COUNT; i++)
		{
			if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
			{
				sum0 += g->overlapInfo[j].HistAVG0[i];
				sum1 += g->overlapInfo[j].HistAVG1[i];

				//RGB avg
				if (g->overlapInfo[j].curvePointMin[0] == 0 && g->overlapInfo[j].HistAVG0[i] > 0)
					g->overlapInfo[j].curvePointMin[0] = i;
				if (g->overlapInfo[j].curvePointMin[1] == 0 && g->overlapInfo[j].HistAVG1[i] > 0)
					g->overlapInfo[j].curvePointMin[1] = i;

				if (g->overlapInfo[j].HistAVG0[i] > 0)
					g->overlapInfo[j].curvePointMax[0] = i;
				if (g->overlapInfo[j].HistAVG1[i] > 0)
					g->overlapInfo[j].curvePointMax[1] = i;

				//find index of histogram near the midPoint
				if (g->overlapInfo[j].HistAVG0[i] > 0 && ((sum0 - g->overlapInfo[j].HistAVG0[i]) <= midCount0 && midCount0 < sum0) || ((sum0 - g->overlapInfo[j].HistAVG0[i]) < midCount0 && midCount0 <= sum0))
				{
					g->overlapInfo[j].curvePointMid[0] = i;
				}

				if (g->overlapInfo[j].HistAVG1[i] > 0 && ((sum1 - g->overlapInfo[j].HistAVG1[i]) <= midCount1 && midCount1 < sum1) || ((sum1 - g->overlapInfo[j].HistAVG1[i]) < midCount1 && midCount1 <= sum1))
				{
					g->overlapInfo[j].curvePointMid[1] = i;
				}

			}
			else
			{
				sumR0 += g->overlapInfo[j].HistR0[i];
				sumG0 += g->overlapInfo[j].HistG0[i];
				sumB0 += g->overlapInfo[j].HistB0[i];

				sumR1 += g->overlapInfo[j].HistR1[i];
				sumG1 += g->overlapInfo[j].HistG1[i];
				sumB1 += g->overlapInfo[j].HistB1[i];

				//channel R
				if (g->overlapInfo[j].curvePointMinR[0] == 0 && g->overlapInfo[j].HistR0[i] > 0)
					g->overlapInfo[j].curvePointMinR[0] = i;
				if (g->overlapInfo[j].curvePointMinR[1] == 0 && g->overlapInfo[j].HistR1[i] > 0)
					g->overlapInfo[j].curvePointMinR[1] = i;

				if (g->overlapInfo[j].HistR0[i] > 0)
					g->overlapInfo[j].curvePointMaxR[0] = i;
				if (g->overlapInfo[j].HistR1[i] > 0)
					g->overlapInfo[j].curvePointMaxR[1] = i;

				if (g->overlapInfo[j].HistR0[i] > 0 && g->overlapInfo[j].curvePointMidR[0] == 0 && (((sumR0 - g->overlapInfo[j].HistR0[i]) <= midCountR0 && midCountR0 < sumR0) || ((sumR0 - g->overlapInfo[j].HistR0[i]) < midCountR0 && midCountR0 <= sumR0)))
				{
					g->overlapInfo[j].curvePointMidR[0] = i;
				}

				if (g->overlapInfo[j].HistR1[i] > 0 && g->overlapInfo[j].curvePointMidR[1] == 0 && (((sumR1 - g->overlapInfo[j].HistR1[i]) <= midCountR1 && midCountR1 < sumR1) || ((sumR1 - g->overlapInfo[j].HistR1[i]) < midCountR1 && midCountR1 <= sumR1)))
				{
					g->overlapInfo[j].curvePointMidR[1] = i;
				}

				//channel G
				if (g->overlapInfo[j].curvePointMinG[0] == 0 && g->overlapInfo[j].HistG0[i] > 0)
					g->overlapInfo[j].curvePointMinG[0] = i;
				if (g->overlapInfo[j].curvePointMinG[1] == 0 && g->overlapInfo[j].HistG1[i] > 0)
					g->overlapInfo[j].curvePointMinG[1] = i;

				if (g->overlapInfo[j].HistG0[i] > 0)
					g->overlapInfo[j].curvePointMaxG[0] = i;
				if (g->overlapInfo[j].HistG1[i] > 0)
					g->overlapInfo[j].curvePointMaxG[1] = i;

				if (g->overlapInfo[j].HistG0[i] > 0 && g->overlapInfo[j].curvePointMidG[0] == 0 && (((sumG0 - g->overlapInfo[j].HistG0[i]) <= midCountG0 && midCountG0 < sumG0) || ((sumG0 - g->overlapInfo[j].HistG0[i]) < midCountG0 && midCountG0 <= sumG0)))
				{
					g->overlapInfo[j].curvePointMidG[0] = i;
				}

				if (g->overlapInfo[j].HistG1[i] > 0 && g->overlapInfo[j].curvePointMidG[1] == 0 && (((sumG1 - g->overlapInfo[j].HistG1[i]) <= midCountG1 && midCountG1 < sumG1) || ((sumG1 - g->overlapInfo[j].HistG1[i]) < midCountG1 && midCountG1 <= sumG1)))
				{
					g->overlapInfo[j].curvePointMidG[1] = i;
				}


				//channel B
				if (g->overlapInfo[j].curvePointMinB[0] == 0 && g->overlapInfo[j].HistB0[i] > 0)
					g->overlapInfo[j].curvePointMinB[0] = i;
				if (g->overlapInfo[j].curvePointMinB[1] == 0 && g->overlapInfo[j].HistB1[i] > 0)
					g->overlapInfo[j].curvePointMinB[1] = i;

				if (g->overlapInfo[j].HistB0[i] > 0)
					g->overlapInfo[j].curvePointMaxB[0] = i;
				if (g->overlapInfo[j].HistB1[i] > 0)
					g->overlapInfo[j].curvePointMaxB[1] = i;

				if (g->overlapInfo[j].HistB0[i] > 0 && g->overlapInfo[j].curvePointMidB[0] == 0 && (((sumB0 - g->overlapInfo[j].HistB0[i]) <= midCountB0 && midCountB0 < sumB0) || ((sumB0 - g->overlapInfo[j].HistB0[i]) < midCountB0 && midCountB0 <= sumB0)))
				{
					g->overlapInfo[j].curvePointMidB[0] = i;
				}

				if (g->overlapInfo[j].HistB1[i] > 0 && g->overlapInfo[j].curvePointMidB[1] == 0 && (((sumB1 - g->overlapInfo[j].HistB1[i]) <= midCountB1 && midCountB1 < sumB1) || ((sumB1 - g->overlapInfo[j].HistB1[i]) < midCountB1 && midCountB1 <= sumB1)))
				{
					g->overlapInfo[j].curvePointMidB[1] = i;
				}
			}
		}
		//printf("JW calculateCurvePoint: point=(%d, %d), (%d, %d), (%d, %d) of img(%d, %d)\n", g->overlapInfo[j].curvePointMin[0], g->overlapInfo[j].curvePointMin[1], g->overlapInfo[j].curvePointMid[0], g->overlapInfo[j].curvePointMid[1], g->overlapInfo[j].curvePointMax[0], g->overlapInfo[j].curvePointMax[1], g->overlapInfo[j].num[0], g->overlapInfo[j].num[1]);

		//Exception Case: mid == min or mid == max in case that the histogram is one side.
		int tempIdx = -1;
		if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
		{
			if (g->overlapInfo[j].curvePointMid[0] == g->overlapInfo[j].curvePointMin[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMid[0] = (g->overlapInfo[j].curvePointMax[0] + g->overlapInfo[j].curvePointMin[0]) / 2;
				while (g->overlapInfo[j].HistAVG0[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;

					tempIdx++;
				}
				g->overlapInfo[j].curvePointMid[0] = tempIdx;
				printf("curve point AVG adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMid[0] == g->overlapInfo[j].curvePointMax[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMid[0] = (g->overlapInfo[j].curvePointMax[0] + g->overlapInfo[j].curvePointMin[0]) / 2;
				while (g->overlapInfo[j].HistAVG0[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMid[0] = tempIdx;
				printf("curve point AVG adjusted\n");
			}

			if (g->overlapInfo[j].curvePointMid[1] == g->overlapInfo[j].curvePointMin[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMid[1] = (g->overlapInfo[j].curvePointMax[1] + g->overlapInfo[j].curvePointMin[1]) / 2;
				while (g->overlapInfo[j].HistAVG1[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMid[1] = tempIdx;
				printf("curve point AVG adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMid[1] == g->overlapInfo[j].curvePointMax[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMid[1] = (g->overlapInfo[j].curvePointMax[1] + g->overlapInfo[j].curvePointMin[1]) / 2;
				while (g->overlapInfo[j].HistAVG1[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMid[1] = tempIdx;
				printf("curve point AVG adjusted\n");
			}
		}
		else
		{
			if (g->overlapInfo[j].curvePointMidR[0] == g->overlapInfo[j].curvePointMinR[0])
			{
				//printf("original minR[0], midR[0], maxR[0] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinR[0], g->overlapInfo[j].curvePointMidR[0], g->overlapInfo[j].curvePointMaxR[0]);
				tempIdx = g->overlapInfo[j].curvePointMidR[0] = (g->overlapInfo[j].curvePointMaxR[0] + g->overlapInfo[j].curvePointMinR[0]) / 2;
				while (g->overlapInfo[j].HistR0[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMidR[0] = tempIdx;
				printf("curve point R adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMidR[0] == g->overlapInfo[j].curvePointMaxR[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMidR[0] = (g->overlapInfo[j].curvePointMaxR[0] + g->overlapInfo[j].curvePointMinR[0]) / 2;
				while (g->overlapInfo[j].HistR0[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMidR[0] = tempIdx;
				printf("curve point R adjusted\n");
			}
			if (g->overlapInfo[j].curvePointMidR[1] == g->overlapInfo[j].curvePointMinR[1])
			{
				//printf("original minR[0], midR[0], maxR[0] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinR[0], g->overlapInfo[j].curvePointMidR[0], g->overlapInfo[j].curvePointMaxR[0]);
				tempIdx = g->overlapInfo[j].curvePointMidR[1] = (g->overlapInfo[j].curvePointMaxR[1] + g->overlapInfo[j].curvePointMinR[1]) / 2;
				while (g->overlapInfo[j].HistR1[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMidR[1] = tempIdx;
				printf("curve point R adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMidR[1] == g->overlapInfo[j].curvePointMaxR[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMidR[1] = (g->overlapInfo[j].curvePointMaxR[1] + g->overlapInfo[j].curvePointMinR[1]) / 2;
				while (g->overlapInfo[j].HistR1[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMidR[1] = tempIdx;
				printf("curve point R adjusted\n");
			}

			if (g->overlapInfo[j].curvePointMidG[0] == g->overlapInfo[j].curvePointMinG[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMidG[0] = (g->overlapInfo[j].curvePointMaxG[0] + g->overlapInfo[j].curvePointMinG[0]) / 2;
				while (g->overlapInfo[j].HistG0[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMidG[0] = tempIdx;
				printf("curve point G adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMidG[0] == g->overlapInfo[j].curvePointMaxG[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMidG[0] = (g->overlapInfo[j].curvePointMaxG[0] + g->overlapInfo[j].curvePointMinG[0]) / 2;
				while (g->overlapInfo[j].HistG0[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMidG[0] = tempIdx;
				printf("curve point G adjusted\n");
			}
			if (g->overlapInfo[j].curvePointMidG[1] == g->overlapInfo[j].curvePointMinG[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMidG[1] = (g->overlapInfo[j].curvePointMaxG[1] + g->overlapInfo[j].curvePointMinG[1]) / 2;
				while (g->overlapInfo[j].HistG1[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMidG[1] = tempIdx;
				printf("curve point G adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMidG[1] == g->overlapInfo[j].curvePointMaxG[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMidG[1] = (g->overlapInfo[j].curvePointMaxG[1] + g->overlapInfo[j].curvePointMinG[1]) / 2;
				while (g->overlapInfo[j].HistG1[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMidG[1] = tempIdx;
				printf("curve point G adjusted\n");
			}

			if (g->overlapInfo[j].curvePointMidB[0] == g->overlapInfo[j].curvePointMinB[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMidB[0] = (g->overlapInfo[j].curvePointMaxB[0] + g->overlapInfo[j].curvePointMinB[0]) / 2;
				while (g->overlapInfo[j].HistB0[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMidB[0] = tempIdx;
				printf("curve point B adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMidB[0] == g->overlapInfo[j].curvePointMaxB[0])
			{
				tempIdx = g->overlapInfo[j].curvePointMidB[0] = (g->overlapInfo[j].curvePointMaxB[0] + g->overlapInfo[j].curvePointMinB[0]) / 2;
				while (g->overlapInfo[j].HistB0[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMidB[0] = tempIdx;
				printf("curve point B adjusted\n");
			}
			if (g->overlapInfo[j].curvePointMidB[1] == g->overlapInfo[j].curvePointMinB[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMidB[1] = (g->overlapInfo[j].curvePointMaxB[1] + g->overlapInfo[j].curvePointMinB[1]) / 2;
				while (g->overlapInfo[j].HistB1[tempIdx] == 0)
				{
					if (tempIdx == 255)
						break;
					tempIdx++;
				}
				g->overlapInfo[j].curvePointMidB[1] = tempIdx;
				printf("curve point B adjusted\n");
			}
			else if (g->overlapInfo[j].curvePointMidB[1] == g->overlapInfo[j].curvePointMaxB[1])
			{
				tempIdx = g->overlapInfo[j].curvePointMidB[1] = (g->overlapInfo[j].curvePointMaxB[1] + g->overlapInfo[j].curvePointMinB[1]) / 2;
				while (g->overlapInfo[j].HistB1[tempIdx] == 0)
				{
					if (tempIdx == 0)
						break;
					tempIdx--;
				}
				g->overlapInfo[j].curvePointMidB[1] = tempIdx;
				printf("curve point B adjusted\n");
			}
			//printf("after minR[0], midR[0], maxR[0] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinR[0], g->overlapInfo[j].curvePointMidR[0], g->overlapInfo[j].curvePointMaxR[0]);
			//printf("after minR[1], midR[1], maxR[1] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinR[1], g->overlapInfo[j].curvePointMidR[1], g->overlapInfo[j].curvePointMaxR[1]);
		}


		//pointMin and pointMax will be move to inside of histogram area by CURVEPOINT_THRESHOLD.
		if (gainMode == GAIN_CURVE_AUTO_AVERAGE_COMPENSATION)
		{

			float lowMargin = sum0 * CURVEPOINT_THRESHOLD;
			float highMargin = sum0 * (1 - CURVEPOINT_THRESHOLD);
			int minIdx0 = -1, maxIdx0 = -1, minIdx1 = -1, maxIdx1 = -1;
			sum0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sum0 += g->overlapInfo[j].HistAVG0[i];
				//find index of histogram near the lowMargin
				if ((sum0 - g->overlapInfo[j].HistAVG0[i]) <= lowMargin && lowMargin <= sum0)
				{
					//g->overlapInfo[j].curvePointMin[0] = i;
					minIdx0 = i;
				}
				if ((sum0 - g->overlapInfo[j].HistAVG0[i]) <= highMargin && highMargin <= sum0)
				{
					//g->overlapInfo[j].curvePointMax[0] = i;
					maxIdx0 = i;
				}
			}

			lowMargin = sum1 * CURVEPOINT_THRESHOLD;
			highMargin = sum1 * (1 - CURVEPOINT_THRESHOLD);
			sum1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sum1 += g->overlapInfo[j].HistAVG1[i];
				//find index of histogram near the highMargin
				if ((sum1 - g->overlapInfo[j].HistAVG1[i]) <= lowMargin && lowMargin <= sum1)
				{
					//g->overlapInfo[j].curvePointMin[1] = i;
					minIdx1 = i;
				}
				if ((sum1 - g->overlapInfo[j].HistAVG1[i]) <= highMargin && highMargin <= sum1)
				{
					//g->overlapInfo[j].curvePointMax[1] = i;
					maxIdx1 = i;
				}
			}

			if (minIdx0 == g->overlapInfo[j].curvePointMid[0] || minIdx1 == g->overlapInfo[j].curvePointMid[1])
			{
				//printf("same src at Min for curve!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMin[0] = minIdx0;
				g->overlapInfo[j].curvePointMin[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMid[0] || maxIdx1 == g->overlapInfo[j].curvePointMid[1])
			{
				//printf("same src at Max for curve!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMax[0] = maxIdx0;
				g->overlapInfo[j].curvePointMax[1] = maxIdx1;
			}
		}
		else
		{
			//channel R
			float lowMargin = sumR0 * CURVEPOINT_THRESHOLD;
			float highMargin = sumR0 * (1 - CURVEPOINT_THRESHOLD);
			int minIdx0 = -1, maxIdx0 = -1, minIdx1 = -1, maxIdx1 = -1;
			sumR0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sumR0 += g->overlapInfo[j].HistR0[i];

				//find index of histogram near the lowMargin
				if ((sumR0 - g->overlapInfo[j].HistR0[i]) <= lowMargin && lowMargin <= sumR0)
				{
					//g->overlapInfo[j].curvePointMinR[0] = i;
					minIdx0 = i;
				}
				if ((sumR0 - g->overlapInfo[j].HistR0[i]) <= highMargin && highMargin <= sumR0)
				{
					//g->overlapInfo[j].curvePointMaxR[0] = i;
					maxIdx0 = i;
				}
			}

			lowMargin = sumR1 * CURVEPOINT_THRESHOLD;
			highMargin = sumR1 * (1 - CURVEPOINT_THRESHOLD);
			sumR1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sumR1 += g->overlapInfo[j].HistR1[i];

				//find index of histogram near the highMargin
				if ((sumR1 - g->overlapInfo[j].HistR1[i]) <= lowMargin && lowMargin <= sumR1)
				{
					//g->overlapInfo[j].curvePointMinR[1] = i;
					minIdx1 = i;
				}
				if ((sumR1 - g->overlapInfo[j].HistR1[i]) <= highMargin && highMargin <= sumR1)
				{
					//g->overlapInfo[j].curvePointMaxR[1] = i;
					maxIdx1 = i;
				}
			}
			if (minIdx0 == g->overlapInfo[j].curvePointMidR[0] || minIdx1 == g->overlapInfo[j].curvePointMidR[1])
			{
				//printf("same src at Min for curve R!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMinR[0] = minIdx0;
				g->overlapInfo[j].curvePointMinR[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMidR[0] || maxIdx1 == g->overlapInfo[j].curvePointMidR[1])
			{
				//printf("same src at Max for curve R!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMaxR[0] = maxIdx0;
				g->overlapInfo[j].curvePointMaxR[1] = maxIdx1;
			}

			/*if (g->overlapInfo[j].curvePointMidR[0] == g->overlapInfo[j].curvePointMinR[0] || g->overlapInfo[j].curvePointMidR[0] == g->overlapInfo[j].curvePointMaxR[0]
			|| g->overlapInfo[j].curvePointMidR[1] == g->overlapInfo[j].curvePointMinR[1] || g->overlapInfo[j].curvePointMidR[1] == g->overlapInfo[j].curvePointMaxR[1])
			{
			printf("curve R impossible!!!\n");
			float second_curvepoint_threshold = CURVEPOINT_THRESHOLD / 2;
			float lowMargin = sumR0 * second_curvepoint_threshold;
			float highMargin = sumR0 * (1 - second_curvepoint_threshold);
			int minIdx0 = -1, maxIdx0 = -1, minIdx1 = -1, maxIdx1 = -1;

			sumR0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
			sumR0 += g->overlapInfo[j].HistR0[i];
			//if (test)
			//	printf("g->overlapInfo[j].HistR0[%d] = %d\n", i, g->overlapInfo[j].HistR0[i]);

			//find index of histogram near the lowMargin
			if ((sumR0 - g->overlapInfo[j].HistR0[i]) <= lowMargin && lowMargin <= sumR0)
			{
			//g->overlapInfo[j].curvePointMinR[0] = i;
			minIdx0 = i;
			}
			if ((sumR0 - g->overlapInfo[j].HistR0[i]) <= highMargin && highMargin <= sumR0)
			{
			//g->overlapInfo[j].curvePointMaxR[0] = i;
			maxIdx0 = i;
			}
			}

			lowMargin = sumR1 * second_curvepoint_threshold;
			highMargin = sumR1 * (1 - second_curvepoint_threshold);

			sumR1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
			sumR1 += g->overlapInfo[j].HistR1[i];

			//find index of histogram near the highMargin
			if ((sumR1 - g->overlapInfo[j].HistR1[i]) <= lowMargin && lowMargin <= sumR1)
			{
			//g->overlapInfo[j].curvePointMinR[1] = i;
			minIdx1 = i;
			}
			if ((sumR1 - g->overlapInfo[j].HistR1[i]) <= highMargin && highMargin <= sumR1)
			{
			//g->overlapInfo[j].curvePointMaxR[1] = i;
			maxIdx1 = i;
			}
			}
			if (minIdx0 == g->overlapInfo[j].curvePointMidR[0] || minIdx1 == g->overlapInfo[j].curvePointMidR[1])
			{
			printf("same src at Min for curve R again!!!\n");
			}
			else
			{
			g->overlapInfo[j].curvePointMinR[0] = minIdx0;
			g->overlapInfo[j].curvePointMinR[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMidR[0] || maxIdx1 == g->overlapInfo[j].curvePointMidR[1])
			{
			printf("same src at Max for curve R again!!!\n");
			}
			else
			{
			g->overlapInfo[j].curvePointMaxR[0] = maxIdx0;
			g->overlapInfo[j].curvePointMaxR[1] = maxIdx1;
			}
			}*/

			//channel G
			lowMargin = sumG0 * CURVEPOINT_THRESHOLD;
			highMargin = sumG0 * (1 - CURVEPOINT_THRESHOLD);
			sumG0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sumG0 += g->overlapInfo[j].HistG0[i];
				//find index of histogram near the lowMargin
				if ((sumG0 - g->overlapInfo[j].HistG0[i]) <= lowMargin && lowMargin <= sumG0)
				{
					//g->overlapInfo[j].curvePointMinG[0] = i;
					minIdx0 = i;
				}
				if ((sumG0 - g->overlapInfo[j].HistG0[i]) <= highMargin && highMargin <= sumG0)
				{
					//g->overlapInfo[j].curvePointMaxG[0] = i;
					maxIdx0 = i;
				}
			}

			lowMargin = sumG1 * CURVEPOINT_THRESHOLD;
			highMargin = sumG1 * (1 - CURVEPOINT_THRESHOLD);
			sumG1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sumG1 += g->overlapInfo[j].HistG1[i];
				//find index of histogram near the highMargin
				if ((sumG1 - g->overlapInfo[j].HistG1[i]) <= lowMargin && lowMargin <= sumG1)
				{
					//g->overlapInfo[j].curvePointMinG[1] = i;
					minIdx1 = i;
				}
				if ((sumG1 - g->overlapInfo[j].HistG1[i]) <= highMargin && highMargin <= sumG1)
				{
					//g->overlapInfo[j].curvePointMaxG[1] = i;
					maxIdx1 = i;
				}
			}
			if (minIdx0 == g->overlapInfo[j].curvePointMidG[0] || minIdx1 == g->overlapInfo[j].curvePointMidG[1])
			{
				//printf("same src at Min for curve G!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMinG[0] = minIdx0;
				g->overlapInfo[j].curvePointMinG[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMidG[0] || maxIdx1 == g->overlapInfo[j].curvePointMidG[1])
			{
				//printf("same src at Max for curve G!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMaxG[0] = maxIdx0;
				g->overlapInfo[j].curvePointMaxG[1] = maxIdx1;
			}
			//printf("minG[0], midG[0], maxG[0] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinG[0], g->overlapInfo[j].curvePointMidG[0], g->overlapInfo[j].curvePointMaxG[0]);
			//printf("minG[1], midG[1], maxG[1] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinG[1], g->overlapInfo[j].curvePointMidG[1], g->overlapInfo[j].curvePointMaxG[1]);
			/*if (g->overlapInfo[j].curvePointMidG[0] == g->overlapInfo[j].curvePointMinG[0] || g->overlapInfo[j].curvePointMidG[0] == g->overlapInfo[j].curvePointMaxG[0]
			|| g->overlapInfo[j].curvePointMidG[1] == g->overlapInfo[j].curvePointMinG[1] || g->overlapInfo[j].curvePointMidG[1] == g->overlapInfo[j].curvePointMaxG[1])
			{
			printf("curve G impossible!!!\n");
			float second_curvepoint_threshold = CURVEPOINT_THRESHOLD / 2;
			lowMargin = sumG0 * second_curvepoint_threshold;
			highMargin = sumG0 * (1 - second_curvepoint_threshold);
			sumG0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
			sumG0 += g->overlapInfo[j].HistG0[i];
			//find index of histogram near the lowMargin
			if ((sumG0 - g->overlapInfo[j].HistG0[i]) <= lowMargin && lowMargin <= sumG0)
			{
			//g->overlapInfo[j].curvePointMinG[0] = i;
			minIdx0 = i;
			}
			if ((sumG0 - g->overlapInfo[j].HistG0[i]) <= highMargin && highMargin <= sumG0)
			{
			//g->overlapInfo[j].curvePointMaxG[0] = i;
			maxIdx0 = i;
			}
			}

			lowMargin = sumG1 * second_curvepoint_threshold;
			highMargin = sumG1 * (1 - second_curvepoint_threshold);
			sumG1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
			sumG1 += g->overlapInfo[j].HistG1[i];
			//find index of histogram near the highMargin
			if ((sumG1 - g->overlapInfo[j].HistG1[i]) <= lowMargin && lowMargin <= sumG1)
			{
			//g->overlapInfo[j].curvePointMinG[1] = i;
			minIdx1 = i;
			}
			if ((sumG1 - g->overlapInfo[j].HistG1[i]) <= highMargin && highMargin <= sumG1)
			{
			//g->overlapInfo[j].curvePointMaxG[1] = i;
			maxIdx1 = i;
			}
			}
			if (minIdx0 == g->overlapInfo[j].curvePointMidG[0] || minIdx1 == g->overlapInfo[j].curvePointMidG[1])
			{
			printf("same src at Min for curve G again!!!\n");
			}
			else
			{
			g->overlapInfo[j].curvePointMinG[0] = minIdx0;
			g->overlapInfo[j].curvePointMinG[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMidG[0] || maxIdx1 == g->overlapInfo[j].curvePointMidG[1])
			{
			printf("same src at Max for curve G again!!!\n");
			}
			else
			{
			g->overlapInfo[j].curvePointMaxG[0] = maxIdx0;
			g->overlapInfo[j].curvePointMaxG[1] = maxIdx1;
			}
			}*/

			//channel B
			lowMargin = sumB0 * CURVEPOINT_THRESHOLD;
			highMargin = sumB0 * (1 - CURVEPOINT_THRESHOLD);
			sumB0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sumB0 += g->overlapInfo[j].HistB0[i];
				//find index of histogram near the lowMargin
				if ((sumB0 - g->overlapInfo[j].HistB0[i]) <= lowMargin && lowMargin <= sumB0)
				{
					//g->overlapInfo[j].curvePointMinB[0] = i;
					minIdx0 = i;
				}
				if ((sumB0 - g->overlapInfo[j].HistB0[i]) <= highMargin && highMargin <= sumB0)
				{
					//g->overlapInfo[j].curvePointMaxB[0] = i;
					maxIdx0 = i;
				}
			}

			lowMargin = sumB1 * CURVEPOINT_THRESHOLD;
			highMargin = sumB1 * (1 - CURVEPOINT_THRESHOLD);
			sumB1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
				sumB1 += g->overlapInfo[j].HistB1[i];
				//find index of histogram near the highMargin
				if ((sumB1 - g->overlapInfo[j].HistB1[i]) <= lowMargin && lowMargin <= sumB1)
				{
					//g->overlapInfo[j].curvePointMinB[1] = i;
					minIdx1 = i;
				}
				if ((sumB1 - g->overlapInfo[j].HistB1[i]) <= highMargin && highMargin <= sumB1)
				{
					//g->overlapInfo[j].curvePointMaxB[1] = i;
					maxIdx1 = i;
				}
			}
			if (minIdx0 == g->overlapInfo[j].curvePointMidB[0] || minIdx1 == g->overlapInfo[j].curvePointMidB[1])
			{
				//printf("same src at Min for curve B!!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMinB[0] = minIdx0;
				g->overlapInfo[j].curvePointMinB[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMidB[0] || maxIdx1 == g->overlapInfo[j].curvePointMidB[1])
			{
				//printf("same src at Max for curve B !!!\n");
			}
			else
			{
				g->overlapInfo[j].curvePointMaxB[0] = maxIdx0;
				g->overlapInfo[j].curvePointMaxB[1] = maxIdx1;
			}
			//printf("minB[0], midB[0], maxB[0] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinB[0], g->overlapInfo[j].curvePointMidB[0], g->overlapInfo[j].curvePointMaxB[0]);
			//printf("minB[1], midB[1], maxB[1] = (%d, %d, %d)\n", g->overlapInfo[j].curvePointMinB[1], g->overlapInfo[j].curvePointMidB[1], g->overlapInfo[j].curvePointMaxB[1]);
			/*if (g->overlapInfo[j].curvePointMidB[0] == g->overlapInfo[j].curvePointMinB[0] || g->overlapInfo[j].curvePointMidB[0] == g->overlapInfo[j].curvePointMaxB[0]
			|| g->overlapInfo[j].curvePointMidB[1] == g->overlapInfo[j].curvePointMinB[1] || g->overlapInfo[j].curvePointMidB[1] == g->overlapInfo[j].curvePointMaxB[1])
			{
			printf("curve B impossible!!!\n");
			float second_curvepoint_threshold = CURVEPOINT_THRESHOLD / 2;
			lowMargin = sumB0 * second_curvepoint_threshold;
			highMargin = sumB0 * (1 - second_curvepoint_threshold);
			sumB0 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
			sumB0 += g->overlapInfo[j].HistB0[i];
			//find index of histogram near the lowMargin
			if ((sumB0 - g->overlapInfo[j].HistB0[i]) <= lowMargin && lowMargin <= sumB0)
			{
			//g->overlapInfo[j].curvePointMinB[0] = i;
			minIdx0 = i;
			}
			if ((sumB0 - g->overlapInfo[j].HistB0[i]) <= highMargin && highMargin <= sumB0)
			{
			//g->overlapInfo[j].curvePointMaxB[0] = i;
			maxIdx0 = i;
			}
			}

			lowMargin = sumB1 * second_curvepoint_threshold;
			highMargin = sumB1 * (1 - second_curvepoint_threshold);
			sumB1 = 0;
			for (int i = 0; i < VALUE_COUNT; i++)
			{
			sumB1 += g->overlapInfo[j].HistB1[i];
			//find index of histogram near the highMargin
			if ((sumB1 - g->overlapInfo[j].HistB1[i]) <= lowMargin && lowMargin <= sumB1)
			{
			//g->overlapInfo[j].curvePointMinB[1] = i;
			minIdx1 = i;
			}
			if ((sumB1 - g->overlapInfo[j].HistB1[i]) <= highMargin && highMargin <= sumB1)
			{
			//g->overlapInfo[j].curvePointMaxB[1] = i;
			maxIdx1 = i;
			}
			}
			if (minIdx0 == g->overlapInfo[j].curvePointMidB[0] || minIdx1 == g->overlapInfo[j].curvePointMidB[1])
			{
			printf("same src at Min for curve B again!!!\n");
			}
			else
			{
			g->overlapInfo[j].curvePointMinB[0] = minIdx0;
			g->overlapInfo[j].curvePointMinB[1] = minIdx1;
			}
			if (maxIdx0 == g->overlapInfo[j].curvePointMidB[0] || maxIdx1 == g->overlapInfo[j].curvePointMidB[1])
			{
			printf("same src at Max for curve B again!!!\n");
			}
			else
			{
			g->overlapInfo[j].curvePointMaxB[0] = maxIdx0;
			g->overlapInfo[j].curvePointMaxB[1] = maxIdx1;
			}
			}*/
		}
		//printf("JW calculateCurvePoint: marginPoint=(%d, %d), (%d, %d), (%d, %d) of img(%d, %d)\n", g->overlapInfo[j].curvePointMin[0], g->overlapInfo[j].curvePointMin[1], g->overlapInfo[j].curvePointMid[0], g->overlapInfo[j].curvePointMid[1], g->overlapInfo[j].curvePointMax[0], g->overlapInfo[j].curvePointMax[1], g->overlapInfo[j].num[0], g->overlapInfo[j].num[1]);
	}
	//printf("JW calculateCurvePoint end\n");
}
void ExposureCompensator::setGainCurve(CamInputArray *input, int inputCount, long long time_stamp)
{
	for (int i = 0; i < inputCount; i++) {
		CamInput& p = *(input->camIn(i));
		p.exposureCompensation.setExposureCurveRGB(mCurveInfoR[i], mCurveInfoG[i], mCurveInfoB[i], time_stamp);
	}
}
void ExposureCompensator::setGainDiff(GainInfo *g, CamInputArray *input, int gainMode)
{
	long long time_stamp = (long long)QClock();
	printf("bigheadk, setGainDiff g->numImg = %d\n", g->numImg);
	for (int i = 0; i < g->numImg; i++) {		
		CamInput& p = *(input->camIn(i + g->startIdx));

		p.exposureCompensation.setExposureData(0, 0, 0, mCurveInfoR[i], mCurveInfoG[i], mCurveInfoB[i], time_stamp);
	}
	free(g->imgInfo);
	free(g->overlapInfo);
}

bool ExposureCompensator::containROI(int w, int h, cv::Rect roi, cv::Point p) {
	int rx = roi.x;
	int rw = roi.width;

	int ry = roi.y;
	int rh = roi.height;

	if (rx >= 0 && rx + rw < w) {
		if (roi.contains(p))
			return true;
	}
	if (rx<0) {
		cv::Rect t = cv::Rect(rx + w, ry, -rx, rh);
		if (t.contains(p))
			return true;
		t.x = 0;
		t.y = ry;
		t.width = rw + rx;
		t.height = rh;

		if (t.contains(p))
			return true;
	}
	// Todo following case need more test
	if (rx + rw >= w) {

		cv::Rect t = cv::Rect(rx, ry, w - rx, rh);
		if (t.contains(p))
			return true;
		t.x = 0;
		t.y = ry;
		t.width = rw + rx - w;
		t.height = rh;

		if (t.contains(p))
			return true;
	}
	return false;
}

void ExposureCompensator::destroy()
{
	if (mCurveInfoR == NULL)
	{
		for (int i = 0; i < mInputCount; i++)
		{
			free(mCurveInfoR[i]);
		}
	}
	free(mCurveInfoR);

	if (mCurveInfoG == NULL)
	{
		for (int i = 0; i < mInputCount; i++)
		{
			free(mCurveInfoG[i]);
		}
	}
	free(mCurveInfoG);

	if (mCurveInfoB == NULL)
	{
		for (int i = 0; i < mInputCount; i++)
		{
			free(mCurveInfoB[i]);
		}
	}
	free(mCurveInfoB);
}