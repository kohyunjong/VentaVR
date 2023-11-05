#include "SingleRegionalWarping.h"

#include "RegionalWarping/TPSWarping/CThinPlateSpline.h"
#include "RegionalWarping/MLSWarping/imgwarp_mls_rigid.h"
#include "RegionalWarping/MLSWarping/imgwarp_mls_similarity.h"

// public functions //

SingleRegionalWarping::SingleRegionalWarping() {

}

SingleRegionalWarping* SingleRegionalWarping::create_SingleRegionalWarping(int w, int h) {
	printf("SingleRegionalWarping w : %d, h : %d\n", w, h);

	SingleRegionalWarping* instance = new SingleRegionalWarping();

	if (instance->init_SingleRegionalWarping(w, h) == 0) {
		delete instance;
		return NULL;
	}

	return instance;
}

int SingleRegionalWarping::destroy_SingleRegionalWarping(SingleRegionalWarping* instance) {
	if (instance) {
		if (instance->mMapBuffer) {
			free(instance->mMapBuffer);
			instance->mMapBuffer = NULL;
		}
		if (instance->mMapZeros) {
			free(instance->mMapZeros);
			instance->mMapZeros = NULL;
		}

		delete instance;

		printf("Destroy SingleRegionalWarping\n");
		return true;
	}

	printf("ERROR: SingleRegionalWarping instance : NULL!\n");
	return false;
}

int SingleRegionalWarping::init_SingleRegionalWarping(int w, int h) {
	mImageWidth = w;
	mImageHeight = h;

	mWarpingWidth = w / SCALE_FACTOR;
	mWarpingHeight = h / SCALE_FACTOR;

	mMapBuffer = malloc(w * h * 4);
	memset(mMapBuffer, 0, w * h * 4);

	mMapZeros = malloc(w * h * 4);
	memset(mMapZeros, 0, w * h * 4);

	mMode = 0;
	mSize = 3;
	mLength = 0;
	mAutoMode = 0;

	return 1;
}

void* SingleRegionalWarping::getMapBuffer() {
	return mMapBuffer;
}

void SingleRegionalWarping::setMapBuffer(void* buffer) {
	if (buffer == NULL)
		memset(mMapBuffer, 0, mImageWidth * mImageHeight * 4);
	else
		memcpy(mMapBuffer, buffer, mImageWidth * mImageHeight * 4);
}

void SingleRegionalWarping::addSrcPoint(float srcX, float srcY) {
	mSrcX.push_back(srcX);
	mSrcY.push_back(srcY);
}

void SingleRegionalWarping::addDstPoint(float dstX, float dstY) {
	mDstX.push_back(dstX);
	mDstY.push_back(dstY);
}

void SingleRegionalWarping::resetPoint() {
	mSrcX.clear();
	mSrcY.clear();
	mDstX.clear();
	mDstY.clear();
}

int SingleRegionalWarping::warping() {
	if (mAutoMode == 0 && mSrcX.size() == 1 && mSrcY.size() == 1 && mDstX.size() == 1 && mDstY.size() == 1) {
		float x1 = mSrcX[0];
		float y1 = mSrcY[0];
		float x2 = mDstX[0];
		float y2 = mDstY[0];

		float dx = x2 - x1;
		float dy = y2 - y1;

		float m = dy / dx;
		float mp = -1 / m;

		int length = (mLength * 10) / 2;

		if (mp < 1) {
			for (int i = -length; i < length; i++) {
				if (i != 0) {
					int newSrcX = x1 + i;
					int newSrcY = mp * newSrcX + (y1 - mp * x1);
					int newDstX = newSrcX + dx;
					int newDstY = newSrcY + dy;

					mSrcX.push_back(newSrcX);
					mSrcY.push_back(newSrcY);
					mDstX.push_back(newDstX);
					mDstY.push_back(newDstY);
				}
			}
		}
		else {
			for (int i = -length; i < length; i++) {
				if (i != 0) {
					int newSrcY = y1 + i;
					int newSrcX = (1 / mp) * newSrcY + (x1 - (1 / mp) * y1);
					int newDstY = newSrcY + dy;
					int newDstX = newSrcX + dx;

					mSrcX.push_back(newSrcX);
					mSrcY.push_back(newSrcY);
					mDstX.push_back(newDstX);
					mDstY.push_back(newDstY);
				}
			}
		}
	}

	int pointSize = mSrcX.size();

	if ((mSrcX.size() == 0 || mSrcY.size() == 0 || mDstX.size() == 0 || mDstY.size() == 0)
		|| (mSrcX.size() != pointSize || mSrcY.size() != pointSize || mDstX.size() != pointSize || mDstY.size() != pointSize)) {
		printf("ERROR: src size (%d,%d), dst size (%d,%d)\n", mSrcX.size(), mSrcY.size(), mDstX.size(), mDstY.size());
		return REIONGAL_WARPING_ERROR_INVALID_POINT;
	}

	// set source & destination points
	vector<Point2f> srcWarping, dstWarping;
	for (int i = 0; i < pointSize; i++) {
		srcWarping.push_back(Point(mSrcX[i] / SCALE_FACTOR, mSrcY[i] / SCALE_FACTOR));
		dstWarping.push_back(Point(mDstX[i] / SCALE_FACTOR, mDstY[i] / SCALE_FACTOR));
	}
	//

	// calculate distance between source and destination
	vector<double> dist;
	vector<Rect> fixedRects;
	for (int i = 0; i < pointSize; i++) {
#ifdef DIST_STATIC
		dist.push_back(max(20, (int)((sqrt(pow(mSrcX[i] - mDstX[i], 2) + pow(mSrcY[i] - mDstY[i], 2))))));
#else
		dist.push_back(sqrt(pow(srcWarping[i].x - dstWarping[i].x, 2) + pow(srcWarping[i].y - dstWarping[i].y, 2)));
#endif

		int fixed_size = mSize * WARPING_SIZE_FACTOR;

		fixedRects.push_back(Rect(
			mSrcX[i] - fixed_size * dist[i],
			mSrcY[i] - fixed_size * dist[i],
			fixed_size * 2 * dist[i],
			fixed_size * 2 * dist[i]));
	}
	//

	// set fixed rect
	int maxLeft, maxTop, maxRight, maxBottom;

	maxLeft = fixedRects[0].x / SCALE_FACTOR;
	maxTop = fixedRects[0].y / SCALE_FACTOR;
	maxRight = fixedRects[0].x / SCALE_FACTOR + fixedRects[0].width / SCALE_FACTOR;
	maxBottom = fixedRects[0].y / SCALE_FACTOR + fixedRects[0].height / SCALE_FACTOR;

	for (int i = 1; i < fixedRects.size(); i++) {
		int left = fixedRects[i].x / SCALE_FACTOR;
		int top = fixedRects[i].y / SCALE_FACTOR;
		int right = fixedRects[i].x / SCALE_FACTOR + fixedRects[i].width / SCALE_FACTOR;
		int bottom = fixedRects[i].y / SCALE_FACTOR + fixedRects[i].height / SCALE_FACTOR;

		if (left < maxLeft)
			maxLeft = left;
		if (top < maxTop)
			maxTop = top;
		if (right > maxRight)
			maxRight = right;
		if (bottom > maxBottom)
			maxBottom = bottom;
	}

	Rect fixedRect = Rect(maxLeft, maxTop, maxRight - maxLeft, maxBottom - maxTop);
	//

	int cropLeft, cropTop, cropRight, cropBottom;
	if (fixedRect.x < 0)
		cropLeft = 0;
	else
		cropLeft = fixedRect.x;
	if (fixedRect.y < 0)
		cropTop = 0;
	else
		cropTop = fixedRect.y;
	if (fixedRect.x + fixedRect.width > mWarpingWidth - 1)
		cropRight = mWarpingWidth - 1;
	else
		cropRight = fixedRect.x + fixedRect.width;
	if (fixedRect.y + fixedRect.height > mWarpingHeight - 1)
		cropBottom = mWarpingHeight - 1;
	else
		cropBottom = fixedRect.y + fixedRect.height;

	Rect cropRect = Rect(cropLeft, cropTop, cropRight - cropLeft, cropBottom - cropTop);
	int warpingMax = max(mWarpingWidth, mWarpingHeight);

	if (cropRect.width <= 0 || cropRect.height <= 0) {
		printf("ERROR: Warping area is invalid! cropRect WH(%d,%d)\n", cropRect.width, cropRect.height);
		return REIONGAL_WARPING_ERROR_INVALID_AREA;
	}
	else if (cropRect.width > warpingMax || cropRect.height > warpingMax) {
		printf("ERROR: Warping area is too big! cropRect WH(%d,%d) > %d\n", cropRect.width, cropRect.height, warpingMax);
		return REIONGAL_WARPING_ERROR_BIG_AREA;
	}

	// set fixed points
	vector<Point2f> fixedPts;
	fixedPts.push_back(Point(0, 0));
	fixedPts.push_back(Point(cropRect.width, 0));
	fixedPts.push_back(Point(cropRect.width, cropRect.height));
	fixedPts.push_back(Point(0, cropRect.height));

	// x
	for (int i = 80 / SCALE_FACTOR; i < cropRect.width - 40 / SCALE_FACTOR; i += 80 / SCALE_FACTOR) {
		fixedPts.push_back(Point(i, 0));
		fixedPts.push_back(Point(i, cropRect.height));
	}

	// y
	for (int i = 80 / SCALE_FACTOR; i < cropRect.height - 40 / SCALE_FACTOR; i += 80 / SCALE_FACTOR) {
		fixedPts.push_back(Point(0, i));
		fixedPts.push_back(Point(cropRect.width, i));
	}
	//

	// set all input points
	vector<Point2f> srcPts, dstPts;
	for (int i = 0; i < srcWarping.size(); i++) {
		srcPts.push_back(Point(srcWarping[i].x - cropRect.x, srcWarping[i].y - cropRect.y));
		dstPts.push_back(Point(dstWarping[i].x - cropRect.x, dstWarping[i].y - cropRect.y));
	}
	for (int i = 0; i < fixedPts.size(); i++) {
		srcPts.push_back(fixedPts[i]);
		dstPts.push_back(fixedPts[i]);
	}
	//

	int warping_mode = mMode;

	Mat result_mapx, result_mapy;

	if (warping_mode == 0) {
		CThinPlateSpline tps(srcPts, dstPts);
#if 0
		// TEST
		Mat original_Img = cv::imread("D:\\SKT360\\8K_ป๙วร\\snapshot_0.jpg");
		Mat warping_Img;

		Rect cropRect = Rect(cropLeft, cropTop, cropRight - cropLeft, cropBottom - cropTop);
		Mat original_Img2 = original_Img.clone();
		Mat crop_Img = original_Img2(cropRect);

		tps.warpImage(crop_Img, warping_Img, INTER_LINEAR, &result_mapx, &result_mapy);

		Mat original_Img_clone = original_Img.clone();
		Mat warping_Img_clone = warping_Img.clone();

		Mat original_Img_sub = original_Img_clone(cropRect);
		warping_Img_clone.copyTo(original_Img_sub);

		cv::imwrite("warping_original_input.jpg", original_Img);
		cv::imwrite("warping_result_input.jpg", original_Img_clone);

		cv::imwrite("warping_original_sub_input.jpg", crop_Img);
		cv::imwrite("warping_result_sub_input.jpg", warping_Img);

		for (int i = 0; i < srcWarping.size(); i++) {
			circle(original_Img, srcWarping[i], 2, Scalar(0, 0, 255), 2);
			circle(original_Img, dstWarping[i], 2, Scalar(255, 0, 0), 2);

			circle(original_Img_clone, srcWarping[i], 2, Scalar(0, 0, 255), 2);
			circle(original_Img_clone, dstWarping[i], 2, Scalar(255, 0, 0), 2);
		}

		//cv::imshow("img", original_Img_clone);
		//cv::waitKey();
		cv::imwrite("warping_original_input_point.jpg", original_Img);
		cv::imwrite("warping_result_input_point.jpg", original_Img_clone);

		//cv::imwrite("warping_original_sub_input_point.jpg", original_Img);
		//cv::imwrite("warping_result_sub_input_point.jpg", original_Img_clone);
		// TEST
#else if
		tps.warpImage(cropRect.width, cropRect.height, INTER_LINEAR, &result_mapx, &result_mapy);
#endif

		for (int i = 0; i < result_mapx.rows; i++) {
			for (int j = 0; j < result_mapx.cols; j++) {
				result_mapx.at<double>(i, j) -= j;
				result_mapy.at<double>(i, j) -= i;
			}
		}
	}
	else if (warping_mode == 1 || warping_mode == 2) {
		ImgWarp_MLS *imgTrans = NULL;
		if (warping_mode == 1)
			imgTrans = new ImgWarp_MLS_Similarity();
		else if (warping_mode == 2)
			imgTrans = new ImgWarp_MLS_Rigid();

		vector<Point_<int>> _srcPts, _dstPts;
		for (int i = 0; i < srcPts.size(); i++) {
			_srcPts.push_back(srcPts[i]);
			_dstPts.push_back(dstPts[i]);
		}

		imgTrans->setAllAndGenerate(
			cropRect.width, cropRect.height,
			_srcPts,
			_dstPts,
			cropRect.width, cropRect.height, 1,
			&result_mapx,
			&result_mapy);
	}

	int smooth_size_w = cropRect.width * 0.05;
	int smooth_size_h = cropRect.height * 0.05;

	int w = result_mapx.cols;
	int h = result_mapx.rows;

	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++) {
			if (j < smooth_size_w) {
				result_mapx.at<double >(i, j) = j * result_mapx.at<double>(i, smooth_size_w) / (double)smooth_size_w;
				result_mapy.at<double>(i, j) = j * result_mapy.at<double>(i, smooth_size_w) / (double)smooth_size_w;
			}
			else if (j > w - 1 - smooth_size_w) {
				result_mapx.at<double>(i, j) = (w - 1 - j) * result_mapx.at<double>(i, w - 1 - smooth_size_w) / (double)smooth_size_w;
				result_mapy.at<double>(i, j) = (w - 1 - j) * result_mapy.at<double>(i, w - 1 - smooth_size_w) / (double)smooth_size_w;
			}

			if (i < smooth_size_h) {
				result_mapx.at<double>(i, j) = i * result_mapx.at<double>(smooth_size_h, j) / (double)smooth_size_h;
				result_mapy.at<double>(i, j) = i * result_mapy.at<double>(smooth_size_h, j) / (double)smooth_size_h;
			}
			else if (i > h - 1 - smooth_size_h) {
				result_mapx.at<double>(i, j) = (h - 1 - i) * result_mapx.at<double>(h - 1 - smooth_size_h, j) / (double)smooth_size_h;
				result_mapy.at<double>(i, j) = (h - 1 - i) * result_mapy.at<double>(h - 1 - smooth_size_h, j) / (double)smooth_size_h;
			}
		}
	}

	for (int i = 0; i < result_mapx.rows; i++) {
		for (int j = 0; j < result_mapx.cols; j++) {
			int x1 = result_mapx.at<double>(i, j) / 1;
			int y1 = result_mapy.at<double>(i, j) / 1;

			if (abs(x1) > 255 || abs(y1) > 255) {
				printf("ERROR: Warping Distance is too long xy(%d,%d) dist(%d,%d) > 255\n", j, i, x1, y1);

				return REIONGAL_WARPING_ERROR_LONG_DISTANCE;
			}
		}
	}

	result_mapx *= SCALE_FACTOR;
	result_mapy *= SCALE_FACTOR;

	resize(result_mapx, result_mapx, Size(w * SCALE_FACTOR, h * SCALE_FACTOR));
	resize(result_mapy, result_mapy, Size(w * SCALE_FACTOR, h * SCALE_FACTOR));

	merge(result_mapx, result_mapy, cropRect.x * SCALE_FACTOR, cropRect.y * SCALE_FACTOR, cropRect.width * SCALE_FACTOR, cropRect.height * SCALE_FACTOR);

	return REIONGAL_WARPING_OK;
}

int SingleRegionalWarping::merge(Mat result_mapx, Mat result_mapy, int x, int y, int w, int h) {
	Mat_<double> mapx = Mat_<double>(Size(mImageWidth, mImageHeight));
	Mat_<double> mapy = Mat_<double>(Size(mImageWidth, mImageHeight));

	int groupCount = 0;

	Rect cropRect = Rect(x, y, w, h);
	Mat_<double> mapx2 = Mat_<double>(Size(mImageWidth, mImageHeight));
	Mat_<double> mapy2 = Mat_<double>(Size(mImageWidth, mImageHeight));
	Mat_<double> mapx_sub = mapx2(cropRect);
	Mat_<double> mapy_sub = mapy2(cropRect);
	result_mapx.copyTo(mapx_sub);
	result_mapy.copyTo(mapy_sub);

	add(mapx, mapx2, mapx);
	add(mapy, mapy2, mapy);

	unsigned char* map = (unsigned char*)mMapBuffer;
	for (int i = 0; i < mImageWidth * mImageHeight; i++) {
		int fx1 = map[i * 4];
		int fy1 = map[i * 4 + 2];
		double fx2 = map[i * 4 + 1] * 2.0 / 255.0 - 1;
		double fy2 = map[i * 4 + 3] * 2.0 / 255.0 - 1;

		if (fx2 < 0.0)
			fx1 = -fx1;
		if (fy2 < 0.0)
			fy1 = -fy1;

		double fx = fx1 + fx2;
		double fy = fy1 + fy2;

		int ix = i % mImageWidth;
		int iy = i / mImageWidth;

		double sumx = mapx.at<double>(iy, ix) + fx;
		double sumy = mapy.at<double>(iy, ix) + fy;

		int x1 = sumx / 1;
		int x2 = (sumx - x1 + 1) / 2.0 * 255.0;

		int y1 = sumy / 1;
		int y2 = (sumy - y1 + 1) / 2.0 * 255.0;

		map[i * 4] = abs(x1);
		map[i * 4 + 1] = x2;
		map[i * 4 + 2] = abs(y1);
		map[i * 4 + 3] = y2;
	}

	return 0;
}

void* SingleRegionalWarping::getMapZeros() {
	return mMapZeros;
}