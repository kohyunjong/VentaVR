#include "qSeamFinder.h"
#include "clock.h"

#define SEAM_SCALE 8
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int qSeamFinder::makeSeamMask()
{	
#if	(0)
	double t = QClock();
	std::vector<cv::Mat> imgs;
	std::vector<cv::Mat> masks;
	std::vector<cv::Point2i> corners;

	for (size_t i = 0; i<input->count(); i++) {
		WarpHelper& h = helper(i);
		int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
		cv::Mat m(rh, rw, CV_8UC3), msk(rh, rw, CV_8UC1);
		//jimage::Image& iii = ((CamInputImageFile*)(input->camIn(i)))->img;
		jimage::Image iii;
		CamInputPBO* caminputpbo = ((CamInputPBO*)input->camIn(i));
		iii.cols = 1440;
		iii.rows = 1080;
		iii.buf = tempBuff[i];

		cv::Mat dst, dst2;
		cv::pyrDown(cv::Mat(iii.rows, iii.cols, CV_8UC3, iii.data), dst);
		cv::pyrDown(dst, dst2);
		jimage::Image iig(dst2.cols, dst2.rows, 3); iig.buf = dst2.data;
		h.smallWarp(iii, m.data, msk.data, SEAM_SCALE);
		imgs.push_back(m);
		masks.push_back(msk);
		corners.push_back(cv::Point2i(rx, ry));
	}
	for (size_t i = 0; i<input->count() - 1; i++)
		for (size_t j = i + 1; j<input->count(); j++)
			findSeamInPair(width / SEAM_SCALE, imgs[i], masks[i], corners[i], imgs[j], masks[j], corners[j]);
	for (size_t i = 0; i<input->count(); i++) {
		WarpHelper& h = helper(i);
		GLTexMask& tex = input->camIn(i)->getTexMask();
		cv::dilate(masks[i], masks[i], cv::Mat());

		int mw = width / SEAM_SCALE, mh = height / SEAM_SCALE;
		tex.setMask(0, mw, mh);
		int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
		if (rx<0) rx += mw;
		if (rx + rw>mw) {
			int w1 = (mw - rx), w2 = (rx + rw - mw);
			unsigned char* b1 = new unsigned char[w1*rh];
			unsigned char* b2 = new unsigned char[w2*rh];
			for (int y = 0; y<rh; y++) {
				memcpy(b1 + y*w1, masks[i].data + y*rw, w1);
				memcpy(b2 + y*w2, masks[i].data + y*rw + w1, w2);
			}
			tex.subMask(b1, rx, ry, w1, rh);
			tex.subMask(b2, 0, ry, w2, rh);
			delete b1;
			delete b2;
		}
		else
			tex.subMask(masks[i].data, rx, ry, rw, rh);

		/*
		char fn[1024];
		sprintf(fn,"C:\\Users\\joony\\Desktop\\mask%ld.png",i);
		imwrite(fn,um2);
		*/
	}
	printf("Mask took : %f\n", QClock() - t);
#endif
	return true;
}