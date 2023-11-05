#include "QAdvencedParam.h"

QAdvancedParam::QAdvancedParam() {}
QAdvancedParam::QAdvancedParam(const QAdvancedParam &other) { *this = other; }

const QAdvancedParam& QAdvancedParam::operator =(const QAdvancedParam &other)
{
	output_mask = other.output_mask;
	selectIndex = other.selectIndex;
	mask_rois	= other.mask_rois;
	return *this;
}
void QAdvancedParam::QDestroy()
{
	output_mask = NULL;
	selectIndex.clear();
	mask_rois.clear();
}