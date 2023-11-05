#include "QWarper.h"
int QWarper::makeWarpingData(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h, int input_type, int output_type)
{
	/*switch (_mode)
	{
	case CIRCULAR_FISH_TO_EQUIRECT:
	case FULL_FRAME_FISHEYE_TO_EQUIRECT:
		SetDataEquirect(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, intput_h, output_w, output_h);
		break;
	case CIRCULAR_FISH_TO_STEREOGRAPHIC:
	case FULL_FRAME_FISHEYE_TO_STEREOGRAPHIC:
		SetDataStereographic(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, intput_h, output_w, output_h);
		break;
	case RECTLINEAR_TO_EQUIRECT:
	case RECTLINEAR_TO_PANORAMA:
		SetDataRectLinear(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, intput_h, output_w, output_h);
	break;
	default:
		break;
	}*/
	if (input_type == RECTLINEAR) {
		if (output_type == WIDE_PANORAMA || output_type == EQUIRECTANGULAR)
			SetDataRectLinear(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, intput_h, output_w, output_h);
	}
	else if (input_type == FULL_FRAME_FISHEYE || input_type == CIRCULAR_FISHEYE) {
		//if (output_type == EQUIRECTANGULAR)
			SetDataEquirect(_fov, _out_fov, _yaw, _pitch, _roll, _lens_b, input_w, intput_h, output_w, output_h);
	}
	
	return WARP_SUCCESS;
}
int QWarper::changeDataMode(int input_w, int intput_h, int output_w, int output_h, int input_type, int output_type)
{
	/*switch (mode)
	{
	case CIRCULAR_FISH_TO_EQUIRECT:
	case FULL_FRAME_FISHEYE_TO_EQUIRECT:
		SetDataEquirect(fov, out_fov, yaw, pitch, roll, lens_b, input_w, intput_h, output_w, output_h);
		break;
	case CIRCULAR_FISH_TO_STEREOGRAPHIC:
	case FULL_FRAME_FISHEYE_TO_STEREOGRAPHIC:
		SetDataStereographic(fov, out_fov, yaw, pitch, roll, lens_b, input_w, intput_h, output_w, output_h);
		break;
	case RECTLINEAR_TO_EQUIRECT:
	case RECTLINEAR_TO_PANORAMA:
		SetDataRectLinear(fov, out_fov, yaw, pitch, roll, lens_b, input_w, intput_h, output_w, output_h);
	break;
	default:
		break;
	}*/
	if (input_type == RECTLINEAR) {
		if (output_type == WIDE_PANORAMA || output_type == EQUIRECTANGULAR)
			SetDataRectLinear(fov, out_fov, yaw, pitch, roll, lens_b, input_w, intput_h, output_w, output_h);
	}
	else if (input_type == FULL_FRAME_FISHEYE || input_type == CIRCULAR_FISHEYE) {
		//if (output_type == EQUIRECTANGULAR)
			SetDataEquirect(fov, out_fov, yaw, pitch, roll, lens_b, input_w, intput_h, output_w, output_h);
	}

	return WARP_SUCCESS;
}
int QWarper::SetDataEquirect(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h)
{
	int i;
	double a, b;
	//common
	a = DEG_TO_RAD(_fov);	// field of view in rad
	b = DEG_TO_RAD(_out_fov);

	rad[3] = 0;
	rad[2] = _lens_b;
	rad[1] = 0;
	rad[0] = 1.0 - rad[1] - rad[2] - rad[3];
	rad[4] = ((double)(input_w < intput_h ? input_w : intput_h)) / 2.0;
	rad[5] = rad[4];

	//
	SetMatrix(-DEG_TO_RAD(_pitch),
		0.0,
		-DEG_TO_RAD(_roll),
		m,
		0);

	dist = (double)output_w / b;
	scale[0] = (double)input_w / a / dist;
	scale[1] = scale[0];

	rot[0] = dist * PI;
	rot[1] = -_yaw *  dist * PI / 180.0;

	perspect[0] = (void*)(m);
	perspect[1] = (void*)&(dist);

	//invers
	SetMatrix(DEG_TO_RAD(_pitch),
		0.0,
		DEG_TO_RAD(_roll),
		inv_m,
		1);

	inv_dist = (double)output_w / b;
	inv_scale[0] = (double)input_w / a / inv_dist;
	inv_scale[0] = 1.0 / inv_scale[0];
	inv_scale[1] = inv_scale[0];

	inv_rot[0] = inv_dist * PI;
	inv_rot[1] = _yaw *  inv_dist * PI / 180.0;

	inv_perspect[0] = (void*)(inv_m);
	inv_perspect[1] = (void*)&(inv_dist);

	return WARP_SUCCESS;
}
int QWarper::SetDataStereographic(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h)
{
	int i;
	double a, b;
	//common
	a = DEG_TO_RAD(_fov);	// field of view in rad
	b = DEG_TO_RAD(_out_fov);

	rad[3] = 0;
	rad[2] = _lens_b;
	rad[1] = 0;
	rad[0] = 1.0 - rad[1] - rad[2] - rad[3];
	rad[4] = ((double)(input_w < intput_h ? input_w : intput_h)) / 2.0;
	rad[5] = rad[4];

	//
	SetMatrix(-DEG_TO_RAD(_pitch),
		0.0,
		-DEG_TO_RAD(_roll),
		m,
		0);

	double tpara = 1;
	double      tx, ty;
	stereographic_erect(b / 2.0, 0.0, &tx, &ty, tpara);
	dist = output_w / (2.0*tx);

	scale[0] = (double)input_w / a / dist;
	scale[1] = scale[0];
	
	rot[0] = dist * PI;								// 180 in screenpoints
	rot[1] = -_yaw *  dist * PI / 180.0; 			//    rotation angle in screenpoints

	perspect[0] = (void*)(m);
	perspect[1] = (void*)&(dist);

	//invers
	SetMatrix(DEG_TO_RAD(_pitch),
		0.0,
		DEG_TO_RAD(_roll),
		inv_m,
		1);

	tpara = 1;
	stereographic_erect(b / 2.0, 0.0, &tx, &ty, tpara);
	inv_dist = output_w / (2.0*tx);

	inv_scale[0] = 1.0 / ((double)input_w / a / inv_dist);
	inv_scale[1] = inv_scale[0];

	inv_rot[0] = inv_dist * PI;								// 180 in screenpoints
	inv_rot[1] = _yaw *  inv_dist * PI / 180.0; 			//    rotation angle in screenpoints

	inv_perspect[0] = (void*)(inv_m);
	inv_perspect[1] = (void*)&(inv_dist);

	return WARP_SUCCESS;
}
int QWarper::SetDataRectLinear(double _fov, double _out_fov, double _yaw, double _pitch, double _roll, double _lens_b, int input_w, int intput_h, int output_w, int output_h)
{
	int i;
	double a, b;
	//common
	double out_fov = 180;
	a = DEG_TO_RAD(_fov);	// field of view in rad
	b = DEG_TO_RAD(out_fov);

	rad[3] = 0;
	rad[2] = _lens_b;
	rad[1] = 0;
	rad[0] = 1.0 - rad[1] - rad[2] - rad[3];
	rad[4] = ((double)(input_w < intput_h ? input_w : intput_h)) / 2.0;
	rad[5] = rad[4];

	//
	SetMatrix(-DEG_TO_RAD(_pitch),
		0.0,
		-DEG_TO_RAD(_roll),
		m,
		0);

	dist = (double)output_w / b;
	scale[0] = (double)input_w / (2.0 * tan(a / 2.0)) / dist;
	scale[1] = scale[0];

	rot[0] = dist * PI;
	rot[1] = -_yaw *  dist * PI / 180.0;

	perspect[0] = (void*)(m);
	perspect[1] = (void*)&(dist);

	//invers
	SetMatrix(DEG_TO_RAD(_pitch),
		0.0,
		DEG_TO_RAD(_roll),
		inv_m,
		1);

	inv_dist = (double)output_w / b;
	inv_scale[0] = (double)input_w / (2.0 * tan(a / 2.0)) / inv_dist;
	inv_scale[0] = 1.0 / inv_scale[0];
	inv_scale[1] = inv_scale[0];

	inv_rot[0] = inv_dist * PI;
	inv_rot[1] = _yaw *  inv_dist * PI / 180.0;

	inv_perspect[0] = (void*)(inv_m);
	inv_perspect[1] = (void*)&(inv_dist);

	return WARP_SUCCESS;
}
//======================================================================================================================================
int QWarper::Warping_Inverse(double dst_x, double dst_y, double* src_x, double* src_y)
{
	double xd = dst_x;
	double yd = dst_y;
	double ret_x = 0, ret_y = 0;

	if (input_type == RECTLINEAR) {
		if (output_type == WIDE_PANORAMA) {
		inverse_lens_distortion(xd, yd, &ret_x, &ret_y, rad);
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_resize(xd, yd, &ret_x, &ret_y, inv_scale[0], inv_scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			inverse_rect_warp(xd, yd, &ret_x, &ret_y, inv_dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_warp(xd, yd, &ret_x, &ret_y, inv_perspect);					// Perspective Control spherical Image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_spherical(xd, yd, &ret_x, &ret_y, inv_dist);				// Convert spherical image to equirect.
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, inv_rot[1]);				// Rotate equirect. image horizontally
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			pano_erect(xd, yd, &ret_x, &ret_y, dist);
		}
		else if (output_type == EQUIRECTANGULAR) {
		inverse_lens_distortion(xd, yd, &ret_x, &ret_y, rad);
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_resize(xd, yd, &ret_x, &ret_y, inv_scale[0], inv_scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			inverse_rect_warp(xd, yd, &ret_x, &ret_y, inv_dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_warp(xd, yd, &ret_x, &ret_y, inv_perspect);					// Perspective Control spherical Image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_spherical(xd, yd, &ret_x, &ret_y, inv_dist);				// Convert spherical image to equirect.
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, inv_rot[1]);				// Rotate equirect. image horizontally
		}
	}
	else if (input_type == FULL_FRAME_FISHEYE || input_type == CIRCULAR_FISHEYE) {
		if (output_type == EQUIRECTANGULAR) {
			inverse_lens_distortion(xd, yd, &ret_x, &ret_y, rad);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			equirect_resize(xd, yd, &ret_x, &ret_y, inv_scale[0], inv_scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			spherical_warp(xd, yd, &ret_x, &ret_y, inv_perspect);					// Perspective Control spherical Image
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			equirect_spherical(xd, yd, &ret_x, &ret_y, inv_dist);				// Convert spherical image to equirect.
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, inv_rot[1]);				// Rotate equirect. image horizontally
		}
		else if (output_type == WIDE_PANORAMA) {
		inverse_lens_distortion(xd, yd, &ret_x, &ret_y, rad);
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_resize(xd, yd, &ret_x, &ret_y, inv_scale[0], inv_scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			/*inverse_rect_warp(xd, yd, &ret_x, &ret_y, inv_dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;*/
		spherical_warp(xd, yd, &ret_x, &ret_y, inv_perspect);					// Perspective Control spherical Image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_spherical(xd, yd, &ret_x, &ret_y, inv_dist);				// Convert spherical image to equirect.
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, inv_rot[1]);				// Rotate equirect. image horizontally
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			pano_erect(xd, yd, &ret_x, &ret_y, dist);
		}
	}
	*src_x = ret_x; *src_y = ret_y;
	return WARP_SUCCESS;
}
int QWarper::Warping_(double dst_x, double dst_y, double* src_x, double* src_y)
{
	double xd = dst_x;
	double yd = dst_y;
	double ret_x = 0, ret_y = 0;

	if (input_type == RECTLINEAR) {
		if (output_type == WIDE_PANORAMA) {
			erect_pano(xd, yd, &ret_x, &ret_y, dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, rot[1]);				// Rotate equirect. image horizontally
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_equirect(xd, yd, &ret_x, &ret_y, dist);				// Convert spherical image to equirect.
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_warp(xd, yd, &ret_x, &ret_y, perspect);					// Perspective Control spherical Image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			rect_warp_sphere(xd, yd, &ret_x, &ret_y, dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_resize(xd, yd, &ret_x, &ret_y, scale[0], scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		lens_distortion(xd, yd, &ret_x, &ret_y, rad);
		}
		else if (output_type == EQUIRECTANGULAR) {
			yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, rot[1]);				// Rotate equirect. image horizontally
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			spherical_equirect(xd, yd, &ret_x, &ret_y, dist);				// Convert spherical image to equirect.
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			spherical_warp(xd, yd, &ret_x, &ret_y, perspect);					// Perspective Control spherical Image
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			rect_warp_sphere(xd, yd, &ret_x, &ret_y, dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			equirect_resize(xd, yd, &ret_x, &ret_y, scale[0], scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			lens_distortion(xd, yd, &ret_x, &ret_y, rad);
		}
	}
	else if (input_type == FULL_FRAME_FISHEYE || input_type == CIRCULAR_FISHEYE) {
		if (output_type == EQUIRECTANGULAR) {
		yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, rot[1]);				// Rotate equirect. image horizontally
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_equirect(xd, yd, &ret_x, &ret_y, dist);				// Convert spherical image to equirect.
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_warp(xd, yd, &ret_x, &ret_y, perspect);					// Perspective Control spherical Image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		equirect_resize(xd, yd, &ret_x, &ret_y, scale[0], scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		lens_distortion(xd, yd, &ret_x, &ret_y, rad);
		}
		else if (output_type == WIDE_PANORAMA) {
			erect_pano(xd, yd, &ret_x, &ret_y, dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, rot[1]);				// Rotate equirect. image horizontally
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_equirect(xd, yd, &ret_x, &ret_y, dist);				// Convert spherical image to equirect.
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		spherical_warp(xd, yd, &ret_x, &ret_y, perspect);					// Perspective Control spherical Image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
			/*rect_warp_sphere(xd, yd, &ret_x, &ret_y, dist);
			xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;*/
		equirect_resize(xd, yd, &ret_x, &ret_y, scale[0], scale[1]);		// Scale image
		xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
		lens_distortion(xd, yd, &ret_x, &ret_y, rad);
	}
	}

	*src_x = ret_x; *src_y = ret_y;

	return WARP_SUCCESS;
}

#if 0
//Image <--> Equirect
int QWarper::Warping_ImageToEquirect(double dst_x, double dst_y, double* src_x, double* src_y)
{
	double xd = dst_x;
	double yd = dst_y;
	double ret_x = 0, ret_y = 0;

	inverse_lens_distortion(xd, yd, &ret_x, &ret_y, rad);
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	equirect_resize(xd, yd, &ret_x, &ret_y, inv_scale[0], inv_scale[1]);		// Scale image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	spherical_warp(xd, yd, &ret_x, &ret_y, inv_perspect);					// Perspective Control spherical Image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	equirect_spherical(xd, yd, &ret_x, &ret_y, inv_dist);				// Convert spherical image to equirect.
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, inv_rot[1]);				// Rotate equirect. image horizontally

	*src_x = ret_x; *src_y = ret_y;

	return WARP_SUCCESS;
}
int QWarper::Warping_EquirectToImage(double src_x, double src_y, double* dst_x, double* dst_y)
{
	double xd = src_x;
	double yd = src_y;
	double ret_x = 0, ret_y = 0;

	yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, rot[1]);				// Rotate equirect. image horizontally
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	spherical_equirect(xd, yd, &ret_x, &ret_y, dist);				// Convert spherical image to equirect.
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	spherical_warp(xd, yd, &ret_x, &ret_y, perspect);					// Perspective Control spherical Image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	equirect_resize(xd, yd, &ret_x, &ret_y, scale[0], scale[1]);		// Scale image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	lens_distortion(xd, yd, &ret_x, &ret_y, rad);

	*dst_x = ret_x; *dst_y = ret_y;
	return WARP_SUCCESS;
}
//Image <--> Stereo
int QWarper::Warping_ImageToStereo(double dst_x, double dst_y, double* src_x, double* src_y)
{
	double xd = dst_x;
	double yd = dst_y;
	double ret_x = 0, ret_y = 0;
	
	inverse_lens_distortion(xd, yd, &ret_x, &ret_y, rad);
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	equirect_resize(xd, yd, &ret_x, &ret_y, inv_scale[0], inv_scale[1]);		// Scale image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	spherical_warp(xd, yd, &ret_x, &ret_y, inv_perspect);					// Perspective Control spherical Image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	equirect_spherical(xd, yd, &ret_x, &ret_y, inv_dist);				// Convert spherical image to equirect.
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, inv_rot[1]);				// Rotate equirect. image horizontally
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	stereographic_erect(xd, yd, &ret_x, &ret_y, inv_dist);

	*src_x = ret_x; *src_y = ret_y;
	return WARP_SUCCESS;
}

int QWarper::Warping_StereoToImage(double dst_x, double dst_y, double* src_x, double* src_y)
{
	double xd = dst_x;
	double yd = dst_y;
	double ret_x = 0, ret_y = 0;

	erect_stereographic(xd, yd, &ret_x, &ret_y, dist);				
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	yaw_rotate_equirect(xd, yd, &ret_x, &ret_y, rot[1]);				// Rotate equirect. image horizontally
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	spherical_equirect(xd, yd, &ret_x, &ret_y, dist);				// Convert spherical image to equirect.
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	spherical_warp(xd, yd, &ret_x, &ret_y, perspect);					// Perspective Control spherical Image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	equirect_resize(xd, yd, &ret_x, &ret_y, scale[0], scale[1]);		// Scale image
	xd = ret_x, yd = ret_y; ret_x = 0; ret_y = 0;
	lens_distortion(xd, yd, &ret_x, &ret_y, rad);
	
	*src_x = ret_x; *src_y = ret_y;
	return WARP_SUCCESS;
}
#endif
//=======================================================================================================================================
int QWarpingHelper::Compute_ROI(cv::Rect& ret_Roi)
{
	int x, y, x_jump;
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;

	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)inputWidth / 2.0 - 0.5;
	double sh2 = (double)inputHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	for (y = 0; y <= inputHeight; y += 1) {
		x_jump = (y == 0 || y == inputHeight || abs(y - inputHeight / 2) <= 5) ? 1 : inputWidth / 2;
		for (x = 0; x <= inputWidth; x += x_jump) {
			x_d = (double)x - sw2;
			y_d = (double)y - sh2;
			Warping_Inverse(x_d, y_d, &Dx, &Dy);
			Dx += w2;
			Dy += h2;
			if (!isnan(Dx)) {
				if ((int)Dx < minX) minX = (int)(Dx + 0.5);
				if ((int)Dx > maxX) maxX = (int)(Dx + 0.5);
			}
			if (!isnan(Dy)) {
				if ((int)Dy < minY) minY = (int)(Dy + 0.5);
				if ((int)Dy > maxY) maxY = (int)(Dy + 0.5);
			}
		}
	}
	if ((maxX - minX) > (outWidth * 0.90))
	{
		minX = 0;
		maxX = outWidth;
	}
	if ((maxY - minY) > (outHeight * 0.90))
	{
		minY = 0;
		maxY = outHeight;
	}
	if (minY < outHeight * 0.2)
	{
		minY = 0;
	}

	if (maxY > outHeight * 0.90)
	{
		maxY = outHeight;
	}
	//cv::Rect Roi;
	ret_Roi.x = minX, ret_Roi.y = minY, ret_Roi.width = maxX - minX, ret_Roi.height = maxY - minY;

	return WARP_SUCCESS;
}
int QWarpingHelper::Warping_Image(cv::Mat Img, QCameraParams& cam)
{
	char tempFileName[1024];
	int roix = 0, roiy = 0, roiw = 0, roih = 0;
	int scale = cam.scale;
	int Img_width = Img.cols;
	int Img_height = Img.rows;
	//outWidth = outWidth / scale;
	//outHeight = outHeight / scale;
	double dist[5];
	cv::Rect Roi;
	Compute_ROI(Roi);
	roix = Roi.x;
	roiy = Roi.y;
	roiw = Roi.width;
	roih = Roi.height;
	cam.imgROI = Roi;
	if (roix < 0){ roix += outWidth; }
	int ch = Img.channels();

	std::vector<unsigned char *> dstList;
	cv::Mat dstImage = cv::Mat::zeros(roih, roiw, CV_8UC1);
	cv::Mat maskImage = cv::Mat::zeros(roih, roiw, CV_8UC1);
	cv::Vec<uchar, 1> *dstData = (cv::Vec<uchar, 1> *)dstImage.data;
	cv::Vec<uchar, 1> *maskData = (cv::Vec<uchar, 1> *)maskImage.data;
	int mask_term = 8 / cam.scale;

	for (int h = roiy; h < roih + roiy; h++) {
		for (int w = roix; w < roix + roiw; w++) {

			double 			w2 = (double)outWidth / 2.0 - 0.5;  // Steve's L
			double 			h2 = (double)outHeight / 2.0 - 0.5;
			double 			sw2 = (double)Img_width / 2.0 - 0.5;
			double 			sh2 = (double)Img_height / 2.0 - 0.5;

			double			sx = w - w2, sy = h - h2;
			double			dx = w - w2, dy = h - h2;

			Warping_((double)dx, (double)dy, &sx, &sy);

			sx += sw2;
			sy += sh2;

			if (sx >= 1 && sx < Img_width - 1 && sy >= 1 && sy < Img_height - 1) {
				if (w < 0)
					w = w + outWidth;
				dstData[(h - roiy)*roiw + (w - roix)] = Img.at<cv::Vec<uchar, 1>>(sy, sx);
				if (sx >= mask_term && sx < Img_width - mask_term && sy >= mask_term && sy < Img_height - mask_term) {
					maskData[(h - roiy)*roiw + (w - roix)] = 255;
				}
			}
		}
	}
	cam.warp_Image = dstImage;
	cam.mask = maskImage;
#if WRITE_TEST_IMAGE
	sprintf(tempFileName, "%s/feature/warp_%d.jpg", IMAGE_PATH, cam.index);
	imwrite(tempFileName, dstImage);
	sprintf(tempFileName, "%s/feature/mask_%d.jpg", IMAGE_PATH, cam.index);
	imwrite(tempFileName, maskImage);
#endif
	return WARP_SUCCESS;
}
int QWarpingHelper::Warping_Image_withMap(cv::Mat Img, QCameraParams& cam, int outWidth, int outHeight, WarpInfo warp_info)
{
	char tempFileName[1024];
	int roix = 0, roiy = 0, roiw = 0, roih = 0;
	cv::Rect Roi = cam.imgROI;
	int Img_width = Img.cols;
	int Img_height = Img.rows;

	roix = Roi.x;
	roiy = Roi.y;
	roiw = Roi.width;
	roih = Roi.height;
	
	if (roix < 0){ roix += outWidth; }
	int ch = Img.channels();

	std::vector<unsigned char *> dstList;
	cv::Mat dstImage = cv::Mat::zeros(roih, roiw, CV_8UC1);
	cv::Mat maskImage = cv::Mat::zeros(roih, roiw, CV_8UC1);

	cv::Vec<uchar, 1> *dstData = (cv::Vec<uchar, 1> *)dstImage.data;
	cv::Vec<uchar, 1> *maskData = (cv::Vec<uchar, 1> *)maskImage.data;

	double		w2 = 0, h2 = 0;
	int mask_term = 8 / cam.scale;
	for (int h = roiy; h < roih + roiy; h++) {
		for (int w = roix; w < roix + roiw; w++) {

			int sx, sy;

			int nx = w;
			int ny = h;

			int xx = w*cam.scale;
			int yy = h*cam.scale;

			int wdt = outWidth*cam.scale;

			if (xx > wdt)
				xx = xx - wdt;
			else if (xx < 0)
				xx = xx + wdt;
			if (yy > outHeight*cam.scale)
				yy = outHeight*cam.scale - 1;
			else if (yy < 0)
				yy = 0;

			sx = warp_info.warpMap[(yy*wdt + xx) * 2];
			sy = warp_info.warpMap[(yy*wdt + xx) * 2 + 1];

			sx = sx / cam.scale;
			sy = sy / cam.scale;

			if (sx >= 1 && sx < Img_width - 1 && sy >= 1 && sy < Img_height - 1) {

				if (nx < 0)
					nx = nx + outWidth;

				dstData[(ny - roiy)*roiw + (nx - roix)] = Img.at<cv::Vec<uchar, 1>>(sy, sx);
				if (sx >= mask_term && sx < Img_width - mask_term && sy >= mask_term && sy < Img_height - mask_term) {
					maskData[(ny - roiy)*roiw + (nx - roix)] = 255;
				}
			}
		}
	}

	cam.warp_Image = dstImage;
	cam.mask = maskImage;
#if WRITE_TEST_IMAGE
	sprintf(tempFileName, "%s/warp/result_%d.jpg", IMAGE_PATH, cam.index);
	imwrite(tempFileName, dstImage);
	sprintf(tempFileName, "%s/warp/mask_%d.jpg", IMAGE_PATH, cam.index);
	imwrite(tempFileName, maskImage);
#endif
	return WARP_SUCCESS;
}

