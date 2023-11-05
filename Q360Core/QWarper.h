#ifndef __QWARPER_H__
#define __QWARPER_H__

#include "QWarperCore.h"
#include <opencv2\core.hpp>
#include "WarpInfo.h"
#include "StitchingMode.h"
#include "CamInput/CamInput.h"
#include "CameraParameter\QCameraParams.h"
class QWarper : public QWarperCore
{
protected:
	int mode;
	double scale[2];
	double inv_scale[2];
	double shear[2];
	double rot[2];
	double inv_rot[2];
	double rad[6];
	double dist, inv_dist;
	void *perspect[2];
	void *inv_perspect[2];
	double m[3][3];
	double inv_m[3][3];

	double yaw, pitch, roll;
	double fov, lens_b;
	double out_fov;
	int input_type, output_type;
public:
	QWarper(){};
	QWarper(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int input_h, int output_w, int output_h, int _input_type, int _output_type)
	{
		fov = _fov; out_fov = _out_fov; yaw = _yaw; pitch = _pitch; roll = _roll; lens_b = _lens_b; input_type = _input_type; output_type = _output_type;
		makeWarpingData(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, input_h, output_w, output_h, _input_type, _output_type);
	};
	~QWarper(){};
	int setWarpData(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int input_h, int output_w, int output_h, int _input_type, int _output_type){
		fov = _fov; out_fov = _out_fov; yaw = _yaw; pitch = _pitch; roll = _roll; lens_b = _lens_b; input_type = _input_type; output_type = _output_type;
		makeWarpingData(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, input_h, output_w, output_h, _input_type, _output_type);
		return WARP_SUCCESS;
	}
	
	int makeWarpingData(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h, int input_type, int output_type);
	int SetDataEquirect(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h);
	int SetDataStereographic(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h);
	int SetDataRectLinear(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h);
	int SetDataCylindrical(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h);

	int changeDataMode(int input_w, int intput_h, int output_w, int output_h, int input_type, int output_type);
	int Warping_Inverse(double dst_x, double dst_y, double* src_x, double* src_y);
	int Warping_(double dst_x, double dst_y, double* src_x, double* src_y);
};

class QWarpingHelper : public QWarper
{
	int inputWidth, inputHeight;
	int outWidth, outHeight;

public:
	QWarpingHelper() {};
	QWarpingHelper(QCameraParams cam, double _out_fov, int input_w, int input_h, int input_type = FULL_FRAME_FISHEYE, int output_type = EQUIRECTANGULAR) :
		QWarper(cam.fov, _out_fov, cam.yaw, cam.pitch, cam.roll, cam.lens_b, input_w, input_h, cam.out_width, cam.out_height, input_type, output_type)
	{
		inputWidth = input_w; inputHeight = input_h;
		outWidth = cam.out_width; outHeight = cam.out_height;
	};
	QWarpingHelper(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, 
		int input_w, int input_h, int output_w, int output_h, int input_type = FULL_FRAME_FISHEYE, int output_type = EQUIRECTANGULAR) :
		QWarper(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, input_h, output_w, output_h, input_type, output_type)
	{
		inputWidth = input_w; inputHeight = input_h;
		outWidth = output_w; outHeight = output_h;
	};

	int Compute_ROI(cv::Rect &ret_Roi);
	int Warping_Image(cv::Mat Img, QCameraParams& cam);
	int Warping_Image_withMap(cv::Mat Img, QCameraParams& cam, int outWidth, int outHeight, WarpInfo warp_info);
};
#endif	//__QWARPER_H__

