#ifndef __QURAM_CAMERA_PARAMS_H__
#define __QURAM_CAMERA_PARAMS_H__

#include "opencv2/core/core.hpp"
//#include "../QUtils/qmath.h"
		struct QCameraParams
		{
			QCameraParams();
			QCameraParams(const QCameraParams& other);
			const QCameraParams& operator =(const QCameraParams& other);

			int index;
			int img_width, img_height;
			int s_width, s_height;
			int out_width, out_height;

			double fov;
			double lens_b;
			double yaw;
			double pitch;
			double roll;
			
			//optimize data
			bool move_optimize_image;
			int img_lens_type;
			cv::Rect imgROI;
			cv::Rect cropROI;
			cv::Mat s_Image, warp_Image;
			//cv::Mat f_Image;
			cv::Mat org_Image;
			cv::Mat org_mask;
			cv::Mat mask;
			int scale;

			std::vector<cv::KeyPoint> kps;
			cv::Mat descriptors;

			cv::Mat match_list;
			std::vector<int> match_index;
			std::vector<cv::Rect> match_rect;
			std::vector<int> mask_index;
			//cv::Mat out_mask;
			unsigned char ** out_mask;
			unsigned char * seam_img;
			std::vector<cv::Rect>* mask_rois;
			bool	IsValued;
			bool	Isdetect;
			bool	IsCrop;
			bool	IsWarp;
			int		step;
			int     num_cams;
			int		stitching_mode;
			int		output_type;
			
			//smart calib
			unsigned char* mapBuffer;
		};

#endif   //__QURAM_CAMERA_PARAMS_H__
