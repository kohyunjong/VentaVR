#ifndef __CAPTURE_QURAM_H__
#define __CAPTURE_QURAM_H__

#include <Windows.h>
#include "opencv2/highgui/highgui.hpp"

#define MW_VIDEO 0
#define BM_VIDEO 1
#define USB_VIDEO 2
#define IMG_VIDEO 3
#define MIC_AUDIO 4
#define MW_AUDIO 5
#define BM_AUDIO 6
#define USB_AUDIO 7

class Capture {
public:
	Capture();
	virtual int getClass();

protected:
	virtual void setClass(int myClass);

private:
	int mMyClass;
};

#endif // __CAPTURE_QURAM_H__