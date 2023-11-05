#ifndef CTHINPLATESPLINE_H_
#define CTHINPLATESPLINE_H_
#include <iostream>
#include <opencv2/opencv.hpp>
//#include <opencv2\opencv.hpp>
using namespace cv;





class CThinPlateSpline {
public:

	CThinPlateSpline();
	CThinPlateSpline(const std::vector<Point2f>& pS, const std::vector<Point2f>& pD);
	~CThinPlateSpline();

	Point2f interpolate(const Point2f& p);

	void warpImage(const Mat& src,
		Mat& dst,
		const int interpolation = INTER_CUBIC,
		Mat* result_mapx = NULL,
		Mat* result_mapy = NULL);

	void warpImage(int w, int h,
		const int interpolation = INTER_CUBIC,
		Mat* result_mapx = NULL,
		Mat* result_mapy = NULL);

	void computeMaps(const Size& inputSize,
		const Size& dstSize,
		Mat_<double>& mapx,
		Mat_<double>& mapy);

private:


	double fktU(const Point2f& p1, const Point2f& p2);

	void computeSplineCoeffs(std::vector<Point2f>& iP,
		std::vector<Point2f>& iiP);




	Mat_<float> cMatrix;
	Mat_<double> mapx;
	Mat_<double> mapy;
	std::vector<Point2f> pSrc;
	std::vector<Point2f> pDst;



};

#endif /* CTHINPLATESPLINE_H_ */