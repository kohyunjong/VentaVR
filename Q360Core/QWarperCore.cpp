#include "QWarperCore.h"

void QWarperCore::matrix_matrix_mult(double mat1[3][3], double mat2[3][3], double mat_ret[3][3])
{
	int i, k;

	for (i = 0; i<3; i++)
	{
		for (k = 0; k<3; k++)
		{
			mat_ret[i][k] = mat1[i][0] * mat2[0][k] + mat1[i][1] * mat2[1][k] + mat1[i][2] * mat2[2][k];
		}
	}
}

void QWarperCore::matrix_mult(double mat[3][3], double vec[3])
{
	int i;
	double v0 = vec[0];
	double v1 = vec[1];
	double v2 = vec[2];


	for (i = 0; i<3; i++)
	{
		vec[i] = mat[i][0] * v0 + mat[i][1] * v1 + mat[i][2] * v2;
	}
}

void QWarperCore::matrix_inv_mult(double mat[3][3], double vec[3])
{
	int i;
	double v0 = vec[0];
	double v1 = vec[1];
	double v2 = vec[2];

	for (i = 0; i<3; i++)
	{
		vec[i] = mat[0][i] * v0 + mat[1][i] * v1 + mat[2][i] * v2;
	}
}

void QWarperCore::SetMatrix(double pitch, double yaw, double roll, double m[3][3], int cl)
{
	double mat_x[3][3], mat_y[3][3], mat_z[3][3], mat_temp[3][3];

	mat_x[0][0] = 1.0;                                mat_x[0][1] = 0.0;                                mat_x[0][2] = 0.0;
	mat_x[1][0] = 0.0;                                mat_x[1][1] = cos(pitch);							mat_x[1][2] = sin(pitch);
	mat_x[2][0] = 0.0;                                mat_x[2][1] = -mat_x[1][2];						mat_x[2][2] = mat_x[1][1];

	mat_y[0][0] = cos(yaw);                           mat_y[0][1] = 0.0;                                mat_y[0][2] = -sin(yaw);
	mat_y[1][0] = 0.0;                                mat_y[1][1] = 1.0;                                mat_y[1][2] = 0.0;
	mat_y[2][0] = -mat_y[0][2];						  mat_y[2][1] = 0.0;                                mat_y[2][2] = mat_y[0][0];

	mat_z[0][0] = cos(roll);						  mat_z[0][1] = sin(roll);							mat_z[0][2] = 0.0;
	mat_z[1][0] = -mat_z[0][1];						  mat_z[1][1] = mat_z[0][0];						mat_z[1][2] = 0.0;
	mat_z[2][0] = 0.0;                                mat_z[2][1] = 0.0;                                mat_z[2][2] = 1.0;

	if (cl)
		matrix_matrix_mult(mat_z, mat_x, mat_temp);
	else
		matrix_matrix_mult(mat_x, mat_z, mat_temp);
	matrix_matrix_mult(mat_temp, mat_y, m);
}

void QWarperCore::lens_distortion(double dst_x, double dst_y, double* src_x, double* src_y, void* lens_params)
{
	double r, scale;

	r = (sqrt(dst_x*dst_x + dst_y*dst_y)) / ((double*)lens_params)[4];
	scale = ((((double*)lens_params)[3] * r + ((double*)lens_params)[2]) * r + ((double*)lens_params)[1]) * r + ((double*)lens_params)[0];

	*src_x = scale * dst_x;
	*src_y = scale * dst_y;
}

void QWarperCore::spherical_equirect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam)
{
	double phi, theta, r;
	double vec[3];

	phi = dst_x / distanceparam;
	theta = -dst_y / distanceparam + PI / 2;

	if (theta < 0)
	{
		theta = -theta;
		phi += PI;
	}
	if (theta > PI)
	{
		theta = PI - (theta - PI);
		phi += PI;
	}

	vec[0] = sin(theta) * sin(phi);
	vec[1] = cos(theta);

	r = sqrt(vec[1] * vec[1] + vec[0] * vec[0]);

	theta = distanceparam * atan2(r, sin(theta) * cos(phi));

	*src_x = theta * vec[0] / r;
	*src_y = theta * vec[1] / r;
}

void QWarperCore::spherical_rect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	// params: double distanceparam

	register double  theta, r;

#if 0	
	theta = atan(sqrt(x_dest*x_dest + y_dest*y_dest) / *((double*)params));
	phi = atan2(y_dest, x_dest);

	/**x_src = *((double*)params) * theta * cos(phi);
	*y_src = *((double*)params) * theta * sin(phi);*/
	*x_src = distanceparam * theta * cos(phi);
	*y_src = distanceparam * theta * sin(phi);
#endif
	r = sqrt(x_dest*x_dest + y_dest*y_dest) / distanceparam;
	if (r == 0.0)
		theta = 1.0;
	else
		theta = atan(r) / r;

	*x_src = theta * x_dest;
	*y_src = theta * y_dest;
}

int QWarperCore::rect_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	// params: double distanceparam

	register double  phi, theta;

	phi = x_dest / distanceparam;
	theta = -y_dest / distanceparam + PI / 2.0;
	if (theta < 0)
	{
		theta = -theta;
		phi += PI;
	}
	if (theta > PI)
	{
		theta = PI - (theta - PI);
		phi += PI;
	}

#if 0
	v[2] = distanceparam * sin(theta) * cos(phi);   //  x' -> z
	v[0] = distanceparam * sin(theta) * sin(phi);	//  y' -> x
	v[1] = distanceparam * cos(theta);				//  z' -> y

	phi = atan2(v[1], v[0]);
	//  old:	
	//	theta = atan2( sqrt( v[0]*v[0] + v[1]*v[1] ) , v[2] );
	//	rho = *((double*)params) * tan( theta );
	//  new:
	rho = *((double*)params) * sqrt(v[0] * v[0] + v[1] * v[1]) / v[2];
	*x_src = rho * cos(phi);
	*y_src = rho * sin(phi);
#endif
#if 1
	* x_src = distanceparam * tan(phi);
	*y_src = distanceparam / (tan(theta) * cos(phi));
#endif
	// normalize phi to be in the -PI, PI range
	while (phi <= -PI)
		phi += 2 * PI;
	while (phi > PI)
		phi -= 2 * PI;

	// check if the point is "in front" of the camera
	if (phi < -PI / 2.0 || phi > PI / 2.0) {
		// behind, transform considered invalid
		return 0;
	}
	else
		return 1;
	// normalize phi to be in the -PI, PI range
	while (phi <= -PI)
		phi += 2 * PI;
	while (phi > PI)
		phi -= 2 * PI;

	// check if the point is "in front" of the camera
	if (phi < -PI / 2.0 || phi > PI / 2.0) {
		// behind, transform considered invalid
		return 0;
	}
	else
		return 1;

}

int QWarperCore::erect_rect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	// params: double distanceparam
#if 0
	theta = atan(sqrt(x_dest*x_dest + y_dest*y_dest) / distanceparam);
	phi = atan2(y_dest, x_dest);


	v[1] = distanceparam * sin(theta) * cos(phi);   //  x' -> y
	v[2] = distanceparam * sin(theta) * sin(phi);	//  y' -> z
	v[0] = distanceparam * cos(theta);				//  z' -> x

	theta = atan2(sqrt(v[0] * v[0] + v[1] * v[1]), v[2]);
	phi = atan2(v[1], v[0]);

	*x_src = distanceparam * phi;
	*y_src = distanceparam * (-theta + PI / 2.0);
#endif

	*x_src = distanceparam * atan2(x_dest, distanceparam);
	*y_src = distanceparam * atan2(y_dest, sqrt(distanceparam*distanceparam + x_dest*x_dest));

	return 1;
}

void QWarperCore::yaw_rotate_equirect(double dst_x, double dst_y, double* src_x, double* src_y, double rot)
{
	*src_x = dst_x + rot;
	*src_y = dst_y;
}

void QWarperCore::equirect_spherical(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam)
{
	double  theta, radial, scale;
	double	vec[3];

	radial = sqrt(dst_x * dst_x + dst_y * dst_y);
	theta = radial / distanceparam;
	if (theta == 0.0)
	{
		scale = 1.0 / distanceparam;
	}
	else
	{
		scale = sin(theta) / radial;
	}

	vec[1] = scale * dst_x;
	vec[0] = cos(theta);


	*src_x = distanceparam * atan2(vec[1], vec[0]);
	*src_y = distanceparam * atan(scale * dst_y / sqrt(vec[0] * vec[0] + vec[1] * vec[1]));
}

void QWarperCore::inverse_rect_warp(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam)
{
	double  theta, r;

	r = sqrt(dst_x*dst_x + dst_y*dst_y) / distanceparam;
	if (r == 0.0)
		theta = 1.0;
	else
		theta = atan(r) / r;

	*src_x = theta * dst_x;
	*src_y = theta * dst_y;
}

void QWarperCore::rect_warp_sphere(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam)
{
	double  theta, rho;

	theta = sqrt(dst_x * dst_x + dst_y * dst_y) / distanceparam;

	if (theta >= PI / 2.0)
		rho = 1.6e16;
	else if (theta == 0.0)
		rho = 1.0;
	else
		rho = tan(theta) / theta;
	*src_x = rho * dst_x;
	*src_y = rho * dst_y;
}

void QWarperCore::sphere_warp_rect(double dst_x, double  dst_y, double* src_x, double* src_y, double distanceparam)
{
	double  theta, r;

	theta = sqrt(dst_x * dst_x + dst_y * dst_y) / distanceparam;

	if (r == 0.0)
		theta = 1.0;
	else
		theta = atan(r) / r;
	*src_x = theta * dst_x;
	*src_y = theta * dst_y;
}

void QWarperCore::spherical_warp(double dst_x, double  dst_y, double* src_x, double* src_y, void* params)
{
	double theta, scale, radial;
	double vec[3];

	radial = sqrt(dst_x * dst_x + dst_y * dst_y);
	theta = radial / *((double*)((void**)params)[1]);
	if (radial == 0.0)
	{
		scale = 0.0;
	}
	else
	{
		scale = sin(theta) / radial;
	}

	vec[0] = scale * dst_x;
	vec[1] = scale * dst_y;
	vec[2] = cos(theta);

	matrix_inv_mult((double(*)[3]) ((void**)params)[0], vec);

	radial = sqrt(vec[0] * vec[0] + vec[1] * vec[1]);
	if (radial == 0.0)
	{
		theta = 0.0;
	}
	else
	{
		theta = *((double*)((void**)params)[1]) * atan2(radial, vec[2]) / radial;
	}

	*src_x = theta * vec[0];
	*src_y = theta * vec[1];
}

void QWarperCore::equirect_resize(double dst_x, double dst_y, double* src_x, double* src_y, double scale_x, double scale_y)
{
	*src_x = dst_x * scale_x;
	*src_y = dst_y * scale_y;
}

void QWarperCore::inverse_lens_distortion(double dst_x, double dst_y, double* src_x, double* src_y, void* params)
{
	double radial_src, radial_dst, f, scale;
	int iter = 0;

	radial_dst = (sqrt(dst_x*dst_x + dst_y*dst_y)) / ((double*)params)[4];

	radial_src = radial_dst;
	f = (((((double*)params)[3] * radial_src + ((double*)params)[2]) * radial_src +
		((double*)params)[1]) * radial_src + ((double*)params)[0]) * radial_src;

	while (abs(f - radial_dst) > R_EPS && iter++ < NUM_MAX_ITERATION)
	{
		radial_src = radial_src - (f - radial_dst) / (((4 * ((double*)params)[3] * radial_src + 3 * ((double*)params)[2]) * radial_src +
			2 * ((double*)params)[1]) * radial_src + ((double*)params)[0]);

		f = (((((double*)params)[3] * radial_src + ((double*)params)[2]) * radial_src +
			((double*)params)[1]) * radial_src + ((double*)params)[0]) * radial_src;
	}

	scale = (radial_dst != 0.0) ? radial_src / radial_dst : 1.0f;
	*src_x = dst_x * scale;
	*src_y = dst_y * scale;
}

int QWarperCore::horiz(double x_dest, double y_dest, double* x_src, double* y_src, void* params)
{
	// params: double horizontal shift

	*x_src = x_dest + (*((double*)params));
	*y_src = y_dest;
	return 1;
}

int QWarperCore::vert(double x_dest, double y_dest, double* x_src, double* y_src, void* params)
{
	// params: double vertical shift

	*x_src = x_dest;
	*y_src = y_dest + (*((double*)params));
	return 1;
}

int QWarperCore::stereographic_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	double lon, lat;
	double sinphi, cosphi, coslon;
	double g, ksp;
	lon = x_dest / distanceparam;
	lat = y_dest / distanceparam;

	sinphi = sin(lat);
	cosphi = cos(lat);
	coslon = cos(lon);

	g = cosphi * coslon;

	// point projects to infinity:
	//    if (fabs(g + 1.0) <= EPSLN)

	ksp = distanceparam * 2.0 / (1.0 + g);
	*x_src = ksp * cosphi * sin(lon);
	*y_src = ksp * sinphi;

	return 1;
}

int QWarperCore::erect_stereographic(double x_dest, double  y_dest, double* lon, double* lat, double distanceparam)
{
	double rh;		/* height above sphere*/
	double c;		/* angle					*/
	double sinc, cosc;	/* sin of c and cos of c			*/

	/* Inverse equations
	-----------------*/
	double x = x_dest / distanceparam;
	double y = y_dest / distanceparam;
	rh = sqrt(x * x + y * y);
	c = 2.0 * atan(rh / (2.0 * 1));
	sinc = sin(c);
	cosc = cos(c);
	*lon = 0;
	if (fabs(rh) <= EPSLN)
	{
		*lat = 0;
		return 0;
	}
	else
	{
		*lat = asin((y * sinc) / rh) * distanceparam;

		if ((fabs(cosc) < EPSLN) && (fabs(x) < EPSLN))
			return 0;
		else
			*lon = atan2((x * sinc), (cosc * rh)) * distanceparam;
	}
	return 1;
}
/** convert from miller cylindrical to erect */
int QWarperCore::erect_millercylindrical(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	double y;

	*x_src = x_dest;
	y = y_dest / distanceparam;
	y = 1.25 * atan(sinh(4 * y / 5.0));
	if (fabs(y) > HALF_PI) {
		*x_src = 0;
		*y_src = 0;
		return 0;
	};
	*y_src = distanceparam * y;
	return 1;
}

/** convert from erect to miller */
int QWarperCore::millercylindrical_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	// params: distanceparam
	double phi, tanPhi;

	*x_src = x_dest;
	phi = y_dest / distanceparam;
	tanPhi = tan(PI / 4 + 0.4 * phi);
	if (tanPhi < 0) {
		*x_src = 0;
		*y_src = 0;
		return 0;
	};
	*y_src = distanceparam*log(tanPhi) / 0.8;
	return 1;
}

// This is the cylindrical projection
int QWarperCore::pano_erect(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	// params: double distanceparam

	*x_src = x_dest;
	*y_src = distanceparam * tan(y_dest / distanceparam);
	return 1;
}

int QWarperCore::erect_pano(double x_dest, double  y_dest, double* x_src, double* y_src, double distanceparam)
{
	// params: double distanceparam

	*x_src = x_dest;
	*y_src = distanceparam * atan(y_dest / distanceparam);
	return 1;
}