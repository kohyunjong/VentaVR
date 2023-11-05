#include "snapshot.h"

#include <tchar.h>
#include <ole2.h>
#include <gdiplus.h>
#pragma comment(lib, "Gdiplus.lib")
using namespace Gdiplus;

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

int GetEncoderClsid(const WCHAR* format, CLSID* pClsid)
{
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize(&num, &size);
	if (size == 0)
		return -1;  // Failure

	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));
	if (pImageCodecInfo == NULL)
		return -1;  // Failure

	GetImageEncoders(num, size, pImageCodecInfo);

	for (UINT j = 0; j < num; ++j)
	{
		if (wcscmp(pImageCodecInfo[j].MimeType, format) == 0)
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free(pImageCodecInfo);
			return j;  // Success
		}
	}

	free(pImageCodecInfo);
	return -1;  // Failure
}

void saveJPEG(const char* path, int width, int height, unsigned char* data) {
	// Initialize GDI+
	GdiplusStartupInput gdiplusStartupInput;

	ULONG_PTR gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);


	Bitmap bitmap(width, height, PixelFormat24bppRGB);
	Rect rect(0, 0, width, height);
	BitmapData bitmapData;

	Status status = bitmap.LockBits(
		&rect,
		ImageLockModeWrite,
		PixelFormat24bppRGB,
		&bitmapData
		);


	cv::Mat temp = cv::Mat(width, height, CV_8UC3, data);
	cv::cvtColor(temp, temp, CV_BGR2RGB);

	memcpy(bitmapData.Scan0, temp.data, width * height * 3);
	free(data);

	bitmap.UnlockBits(&bitmapData);

	TCHAR szUniCode[256] = { 0, };
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, path, strlen(path), szUniCode, 256);

	CLSID pngClsid;
	GetEncoderClsid(_T("image/jpeg"), &pngClsid);
	bitmap.Save(szUniCode, &pngClsid, NULL);
}