/*
* CThinPlateSpline.cpp
*
*  Created on: 24.01.2010
*      Author: schmiedm
*/
#include <vector>
#include <opencv\cv.h>
#include <opencv\cxcore.h>
#include "CThinPlateSpline.h"
using namespace cv;

float last, current;
CThinPlateSpline::CThinPlateSpline() {

}

CThinPlateSpline::CThinPlateSpline(const std::vector<Point2f>& pS, const std::vector<Point2f>& pD)
{
	if (pS.size() == pS.size())
	{
		pSrc = pS;
		pDst = pD;
	}
}

CThinPlateSpline::~CThinPlateSpline() {
}



double CThinPlateSpline::fktU(const Point2f& p1, const Point2f& p2)
{
	float dx = p1.x - p2.x, dy = p1.y - p2.y;
	double r2 = dx*dx + dy*dy;
	double r;
	if (r2 == 0)
		return 0.0;
	else
	{
		r = sqrt(r2);
		return (r2 * log(r));
	}
}

void CThinPlateSpline::computeSplineCoeffs(std::vector<Point2f>& iPIn, std::vector<Point2f>& iiPIn)
{
	std::vector<Point2f>* iPt = NULL;
	std::vector<Point2f>*	iiPt = NULL;

	iPt = &iiPIn;
	iiPt = &iPIn;

	int dim = 2;
	int n = iPt->size();

	Mat_<float> V(dim, n, 0.0);
	Mat_<float> K(n, n, 0.0);

	std::vector<Point2f>::iterator itY;
	std::vector<Point2f>::iterator itX;

	int x = 0;
	for (itX = iPt->begin(); itX != iPt->end(); ++itX, x++) {
		int y = 0;
		for (itY = iPt->begin(); itY != iPt->end(); ++itY, y++) {
			if (y != x) {
				K(x, y) = (float)fktU(*itX, *itY);
			}
		}
	}

	std::vector<Point2f>::iterator it;
	int u = 0;
	for (it = iiPt->begin(); it != iiPt->end(); ++it)
	{
		V(0, u) = (float)it->x;
		V(1, u) = (float)it->y;
		u++;
	}


	Mat Vt;
	transpose(V, Vt);
	cMatrix = Mat_<float>(n, dim, 0.0);

	Mat invK;
	invert(K, invK, DECOMP_SVD);

	cMatrix = invK * Vt;
}





Point2f CThinPlateSpline::interpolate(const Point2f& p)
{

	Point2f interP;
	std::vector<Point2f>* pList = &pDst;
	std::vector<Point2f>::iterator iitY;
	std::vector<Point2f>::iterator iitX;
	int n = pDst.size();
	//Mat_<float> K(n,n,0.0);

	double  cTmp = 0, uTmp = 0, tmp_ii = 0;

	for (int i = 0; i < 2; i++) {
		tmp_ii = 0;
		for (int j = 0; j < (int)pSrc.size(); j++) {
			cTmp = cMatrix(j, i);
			uTmp = fktU((*pList)[j], p);
			tmp_ii = tmp_ii + (cTmp * uTmp);
		}

		if (i == 1) {
			interP.y = (float)(tmp_ii);
		}
		if (i == 0) {
			interP.x = (float)(tmp_ii);
		}
	}

	return interP;
}

void CThinPlateSpline::warpImage(const Mat& src, Mat& dst, const int interpolation, Mat* result_mapx, Mat* result_mapy)
{
	Size size = src.size();
	dst = Mat(size, src.type());

	Size sizeSm(size.width / 1, size.height / 1);

	computeSplineCoeffs(pSrc, pDst);
	computeMaps(size, sizeSm, mapx, mapy);
	resize(mapx, mapx, size, 0, 0, CV_INTER_LINEAR);
	resize(mapy, mapy, size, 0, 0, CV_INTER_LINEAR);

	if (result_mapx != NULL && result_mapy != NULL) {
		*result_mapx = mapx.clone();
		*result_mapy = mapy.clone();
	}

	Mat_<float> xx;
	Mat_<float> yy;

	mapx.convertTo(xx, CV_32F);
	mapy.convertTo(yy, CV_32F);

	remap(src, dst, xx, yy, interpolation);
}

void CThinPlateSpline::warpImage(int w, int h, const int interpolation, Mat* result_mapx, Mat* result_mapy)
{
	Size size(w, h);
	Size sizeSm(w / 1, h / 1);

	computeSplineCoeffs(pSrc, pDst);
	computeMaps(size, sizeSm, mapx, mapy);
	resize(mapx, mapx, size, 0, 0, CV_INTER_LINEAR);
	resize(mapy, mapy, size, 0, 0, CV_INTER_LINEAR);

	if (result_mapx != NULL && result_mapy != NULL) {
		*result_mapx = mapx.clone();
		*result_mapy = mapy.clone();
	}
}



void CThinPlateSpline::computeMaps(const Size& inputSize, const Size& dstSize, Mat_<double>& mx, Mat_<double>& my)
{
	mx = Mat_<float>(dstSize);
	my = Mat_<float>(dstSize);

	Point2f p(0.f, 0.f);
	Point_<float> intP(0.f, 0.f);
	float fx = inputSize.width / (float)dstSize.width, fy = inputSize.height / (float)dstSize.height;
	for (int row = 0; row < dstSize.height; row++) {
		for (int col = 0; col < dstSize.width; col++) {
			p = Point2f(col*fx, row*fy);
			intP = interpolate(p);
			mx(row, col) = intP.x;
			my(row, col) = intP.y;
		}
	}
}
