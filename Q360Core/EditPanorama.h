#pragma once
#include <opencv/cv.hpp>
#include <PTSParam.h>


void rotation_Image(cv::Vec3b input, int num_cam, PTSParam* result_param);
cv::Vec3b straighten_Image(PTSParam* cams, int num_cam);
cv::Mat eulerAnglesToRotationMatrix(cv::Vec3b theta);
cv::Vec3b rotationMatrixToEulerAngles(cv::Mat &R);

#define PI 3.141592654f