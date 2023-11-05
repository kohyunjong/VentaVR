#ifndef __ADVANCED_PARAM_QURAM_H__
#define __ADVANCED_PARAM_QURAM_H__
#include <vector>
#include <opencv2\core.hpp>

struct QAdvancedParam {
	QAdvancedParam();
	QAdvancedParam(const QAdvancedParam& other);
	const QAdvancedParam& operator =(const QAdvancedParam& other);
	void QDestroy();

	unsigned char** output_mask;
	std::vector<int> selectIndex;
	std::vector<cv::Rect> mask_rois;
	int step;
};

#endif // __ADVANCED_PARAM_QURAM_H__