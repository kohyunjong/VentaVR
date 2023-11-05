#include "QCameraParams.h"
QCameraParams::QCameraParams() {}
QCameraParams::QCameraParams(const QCameraParams &other) { *this = other; }

const QCameraParams& QCameraParams::operator =(const QCameraParams &other)
{
	img_width		= other.img_width;
	img_height		= other.img_height;
	out_width		= other.out_width;
	out_height		= other.out_height;
	s_width			= other.s_width;
	s_height		= other.s_height;

	index			= other.index;
	fov				= other.fov;
	yaw				= other.yaw;
	pitch			= other.pitch;
	roll			= other.roll;
	lens_b			= other.lens_b;

	move_optimize_image = other.move_optimize_image;
	img_lens_type		= other.img_lens_type;

	imgROI		= other.imgROI;
	cropROI		= other.cropROI;
	mask		= other.mask.clone();
	org_mask	= other.org_mask.clone();
	s_Image		= other.s_Image.clone();
	warp_Image	= other.warp_Image.clone();
	org_Image   = other.org_Image.clone();
	scale		= other.scale;
	kps			= other.kps;
	descriptors = other.descriptors.clone();
	out_mask	= other.out_mask;
	mask_rois	= other.mask_rois;
	IsValued	= other.IsValued;
	Isdetect	= other.Isdetect;
	IsCrop		= other.IsCrop;
	IsWarp		= other.IsWarp;
	step		= other.step;
	for (int i = 0; i < other.match_index.size(); i++) match_index.push_back(other.match_index[i]);
	for (int i = 0; i < other.match_rect.size(); i++) match_rect.push_back(other.match_rect[i]);
	for (int i = 0; i < other.mask_index.size(); i++) mask_index.push_back(other.mask_index[i]);
	match_list = other.match_list;

	//smart
	mapBuffer = other.mapBuffer;
	seam_img = other.seam_img;
	stitching_mode = other.stitching_mode;
	output_type = other.output_type;

	return *this;
}