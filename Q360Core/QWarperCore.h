#ifndef __QWARPER_CORE_H__
#define __QWARPER_CORE_H__

#include <math.h>

#define EPSLN	1.0e-10
#define R_EPS	1.0e-6   
#define PI 3.14159265358979323846
#define HALF_PI (PI*0.5)
#define NUM_MAX_ITERATION 100
#define DEG_TO_RAD( x )		( (x) * 2.0 * PI / 360.0 )
#define RAD_TO_DEG( x )		( (x) * 360.0 / ( 2.0 * PI ) )
#define WARP_SUCCESS 1
#define WARP_FAIL 0

class QWarperCore {
public:
	QWarperCore(){};
	~QWarperCore(){};

	void matrix_matrix_mult(double mat1[3][3], double mat2[3][3], double mat_ret[3][3]);
	void matrix_mult(double mat[3][3], double vec[3]);
	void matrix_inv_mult(double mat[3][3], double vec[3]);
	void SetMatrix(double pitch, double yaw, double roll, double m[3][3], int cl);

	void lens_distortion(double dst_x, double dst_y, double* src_x, double* src_y, void* lens_params);
	void spherical_equirect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	void yaw_rotate_equirect(double dst_x, double dst_y, double* src_x, double* src_y, double rot);
	void equirect_spherical(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	void spherical_rect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	int rect_erect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	int erect_rect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	void inverse_rect_warp(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	void rect_warp_sphere(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	void sphere_warp_rect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam);
	void spherical_warp(double dst_x, double  dst_y, double* src_x, double* src_y, void* params);
	void equirect_resize(double dst_x, double dst_y, double* src_x, double* src_y, double scale_x, double scale_y);
	void inverse_lens_distortion(double dst_x, double dst_y, double* src_x, double* src_y, void* params);
	int horiz(double x_dest, double y_dest, double* x_src, double* y_src, void* params);
	int vert(double x_dest, double y_dest, double* x_src, double* y_src, void* params);
	int stereographic_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam);
	int erect_stereographic(double x_dest, double  y_dest, double* lon, double* lat, double distanceparam);
	int erect_millercylindrical(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam);
	int millercylindrical_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam);
	int pano_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam);
	int erect_pano(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam);
};

#endif	//__QWARPER_CORE_H__