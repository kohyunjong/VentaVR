#include "ComposeHelper.h"

int ComposeHelper::set(const CamInput& inp, int w, int h, int ComposeHelperMode, bool stereo_right) {
	dist = inp.dist; rt = inp.tr; fov = inp.fov;  diff_rt = inp.diff_tr;
	width = w; height = h;

	int input_wdt = inp.crop_right - inp.crop_left;
	int input_hgt = inp.crop_bottom - inp.crop_top;

	cv::Rect crop_rect;
	crop_rect.x = inp.crop_left;
	crop_rect.y = inp.crop_top;
	crop_rect.width = input_wdt;
	crop_rect.height = input_hgt;

	aspect = input_hgt / (float)input_wdt;
	focal = input_hgt / 2 / tan(fov / 2 * PI / 180) / input_wdt;

	aspect_dw = (double)width / (double)height;
	aspect_sw = (double)input_wdt / (double)input_hgt;

	crop_ratio_w = (double)input_wdt / (double)inp.width;
	crop_ratio_h = (double)input_hgt / (double)inp.height;

	crop_sx = (double)-inp.crop_left / (double)input_wdt;
	crop_sy = (double)-inp.crop_top / (double)input_hgt;

	yaw = rt[0];
	pitch = rt[1];
	roll = rt[2];

	if (stereo_right)
	{
		//printf("bigheadk, ComposeHelper::set, inp.diff_tr[0] = %lf\n", diff_rt[0]);
		yaw += diff_rt[0];
		pitch += diff_rt[1];
	}

	output_type = inp.output_type;
	output_fov = (float)inp.output_fov;
	
	//
	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);
	double mm[3][3];

	SetMatrix(-DEG_TO_RAD(pitch),
		0.0,
		-DEG_TO_RAD(roll),
		mm,
		0);

	for (int i = 0; i < 9; i++)
	{
		rotatem[i] = (float)mm[i / 3][i % 3];
	}

	distance = (double)aspect_dw / b;
	if (inp.lens_type == RECTLINEAR)
	{
		//printf("bigheadk, LENS Type is RECTLINEAR\n");
		scale0 = (double)aspect_sw / (2.0*tan(a / 2.0)) / distance;
	}
	else
	{
		scale0 = (double)aspect_sw / a / distance;
	}

	scale1 = scale0;

	rot0 = distance * PI;
	rot1 = -yaw * distance * PI / 180.0;

	lens_dist[0] = dist[0];
	lens_dist[1] = dist[1];
	lens_dist[2] = dist[2];
	lens_dist[3] = 1.0 - lens_dist[0] - lens_dist[1] - lens_dist[2];
	//
	lens_type = inp.lens_type;
	switch (ComposeHelperMode)
	{
	case COMPOSEHELPER_ROI:
		computeRoi(inp.width, inp.height, width, height, fov, yaw, pitch, roll, lens_dist, crop_rect);
		break;
	case COMPOSEHELPER_REGION:
		computeRegion(inp.width, inp.height, width, height, fov, yaw, pitch, roll, lens_dist, crop_rect);
		break;
	case COMPOSEHELPER_EDIT:
		break;
	default:
		break;
	}
	return WARP_SUCCESS;
}
int ComposeHelper::getOutputCoord(int inputWidth, int inputHeight, int outWidth, int outHeight, int x, int y, float* ret_x, float* ret_y, cv::Rect crop_rect)
{
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;

	int srcWidth = crop_rect.width;
	int srcHeight = crop_rect.height;

	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)srcWidth / 2.0 - 0.5;
	double sh2 = (double)srcHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double invDist = (double)outWidth / b;
	double invScale0;
	if (lens_type == RECTLINEAR)
	{
		invScale0 = 1.0 / ((double)srcWidth / (2.0 * tan(a / 2.0)) / invDist);
	}
	else
	{
		invScale0 = 1.0 / ((double)srcWidth / a / invDist);
	}
	double invScale1 = invScale0;

	double invMt[3][3];

	SetMatrix(DEG_TO_RAD(pitch),
		0.0,
		DEG_TO_RAD(roll),
		invMt,
		1);

	double invRot0 = invDist * PI;                // 180 in screenpoints
	double invRot1 = yaw *  invDist * PI / 180.0;       //    rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = invMt;
	perspect[1] = (void*)&(invDist);

	double params[5];
	params[0] = lens_dist[3];
	params[1] = lens_dist[2];
	params[2] = lens_dist[1];
	params[3] = lens_dist[0];

	params[4] = ((double)(srcWidth < srcHeight ? srcWidth : srcHeight)) / 2.0;

	x_d = (double)(x - crop_rect.x) - sw2;
	y_d = (double)(y - crop_rect.y) - sh2;

	inverse_lens_distortion(x_d, y_d, &Dx, &Dy, params);

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	equirect_resize(x_d, y_d, &Dx, &Dy, invScale0, invScale1);

	if (lens_type == RECTLINEAR)
	{
		//printf("bigheadk, inverse LENS Type is RECTLINEAR\n");
		x_d = Dx; y_d = Dy;
		Dx = 0; Dy = 0;
		inverse_rect_warp(x_d, y_d, &Dx, &Dy, invDist);
	}

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	spherical_warp(x_d, y_d, &Dx, &Dy, perspect);

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	equirect_spherical(x_d, y_d, &Dx, &Dy, invDist);

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	yaw_rotate_equirect(x_d, y_d, &Dx, &Dy, invRot1);

	if (output_type == WIDE_PANORAMA) {
		x_d = Dx; y_d = Dy;
		Dx = 0; Dy = 0;
		pano_erect(x_d, y_d, &Dx, &Dy, invDist);
	}

	Dx += w2;
	Dy += h2;

	*ret_x = Dx;
	*ret_y = Dy;

	return WARP_SUCCESS;
}
int ComposeHelper::computeRoi(int inputWidth, int inputHeight, int outWidth, int outHeight, double fov, double yaw, double pitch, double roll, float* dist, cv::Rect crop_rect) {
	int x, y, x_jump;
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;
	
	int srcWidth = crop_rect.width;
	int srcHeight = crop_rect.height;
	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)srcWidth / 2.0 - 0.5;
	double sh2 = (double)srcHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double invDist = (double)outWidth / b;
	double invScale0;
	if (lens_type == RECTLINEAR)
	{
		printf("bigheadk, inverse LENS Type is RECTLINEAR\n");
		invScale0 = 1.0 / ((double)srcWidth / (2.0 * tan(a / 2.0)) / invDist);
	}
	else
	{
		invScale0 = 1.0 / ((double)srcWidth / a / invDist);
	}
	double invScale1 = invScale0;

	double invMt[3][3];

	SetMatrix(DEG_TO_RAD(pitch),
		0.0,
		DEG_TO_RAD(roll),
		invMt,
		1);

	double invRot0 = invDist * PI;                // 180 in screenpoints
	double invRot1 = yaw *  invDist * PI / 180.0;       //    rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = invMt;
	perspect[1] = (void*)&(invDist);

	double params[5];
	params[0] = dist[3];
	params[1] = dist[2];
	params[2] = dist[1];
	params[3] = dist[0];

	params[4] = ((double)(srcWidth < srcHeight ? srcWidth : srcHeight)) / 2.0;
	
	for (y = 0; y <= inputHeight; y += 1) {
		x_jump = (y == 0 || y == inputHeight || abs(y - inputHeight / 2) <= 5) ? 1 : inputWidth / 2;
		double margin_x = 0.25f;
		double margin_y = 1.25f;
		if (lens_type == RECTLINEAR){
			margin_x = 1.f; margin_y = 1.f;
		}
		for (x = -inputWidth *margin_x; x <= inputWidth*margin_y; x += x_jump) {
			x_d = (double)(x - crop_rect.x) - sw2;
			y_d = (double)(y - crop_rect.y) - sh2;

			inverse_lens_distortion(x_d, y_d, &Dx, &Dy, params);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_resize(x_d, y_d, &Dx, &Dy, invScale0, invScale1);

			if (lens_type == RECTLINEAR)
			{
				//printf("bigheadk, inverse LENS Type is RECTLINEAR\n");
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				inverse_rect_warp(x_d, y_d, &Dx, &Dy, invDist);
			}

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			spherical_warp(x_d, y_d, &Dx, &Dy, perspect);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_spherical(x_d, y_d, &Dx, &Dy, invDist);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			yaw_rotate_equirect(x_d, y_d, &Dx, &Dy, invRot1);

			if (output_type == WIDE_PANORAMA)
			{
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				pano_erect(x_d, y_d, &Dx, &Dy, invDist);
			}

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
	//printf("Dx(%d), Dy(%d), srcWidth(%d), srcHeight(%d)\n", Dx, Dy, srcWidth, srcHeight);
	//printf("1 compute ROI: minX=%d, minY=%d, maxX=%d, maxY=%d \n", minX, minY, maxX, maxY);
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

	//printf("2 compute ROI: minX=%d, minY=%d, maxX=%d, maxY=%d \n", minX, minY, maxX, maxY);
	roix = minX, roiy = minY, roiw = maxX - minX, roih = maxY - minY;
#if(0)
	float minTheta = PI * 2, maxTheta = 0, minRho = PI / 2, maxRho = -PI / 2;

	vec3 prevAngle = toMercator(-.5, -.5);
	minTheta = maxTheta = prevAngle.y;
	for (float dy = -.5f; dy<.55f; dy += 0.01f) {
		float lineMin = PI * 2, lineMax = 0;
		for (float dx = -.5f; dx<.55f; dx += 0.01f) {
			vec3 angle = toMercator(dx, dy);
			angleAlign(prevAngle.y, angle.y);

			lineMin = MIN(lineMin, angle.y), lineMax = MAX(lineMax, angle.y);
			minRho = MIN(minRho, angle.x), maxRho = MAX(maxRho, angle.x);
			prevAngle = angle;
		}
		angleAlign(minTheta, lineMin); angleAlign(maxTheta, lineMax);
		minTheta = MIN(minTheta, lineMin), maxTheta = MAX(maxTheta, lineMax);
	}
	int minX = (int)(minTheta / (PI * 2)*width), maxX = (int)(maxTheta / (PI * 2)*width + 0.99999f);
	int minY = MAX(0, (int)((minRho / PI + .5f)*height)), maxY = MIN(height, (int)((maxRho / PI + .5f)*height + 0.99999f));
	if (maxTheta - minTheta >= PI * 2) minX = 0, maxX = width;
	roix = minX, roiy = minY, roiw = maxX - minX, roih = maxY - minY;
#endif
	return WARP_SUCCESS;
}
int ComposeHelper::computeRegion(int inputWidth, int inputHeight, int outWidth, int outHeight, double fov, double yaw, double pitch, double roll, float* dist, cv::Rect crop_rect) {
	int x, y, x_jump;
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;

	int srcWidth = crop_rect.width;
	int srcHeight = crop_rect.height;
	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)srcWidth / 2.0 - 0.5;
	double sh2 = (double)srcHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double invDist = (double)outWidth / b;
	double invScale0;
	if (lens_type == RECTLINEAR){
		invScale0 = 1.0 / ((double)srcWidth / (2.0 * tan(a / 2.0)) / invDist);
	}
	else {
		invScale0 = 1.0 / ((double)srcWidth / a / invDist);
	}
	double invScale1 = invScale0;

	double invMt[3][3];

	SetMatrix(DEG_TO_RAD(pitch),
		0.0,
		DEG_TO_RAD(roll),
		invMt,
		1);

	double invRot0 = invDist * PI;                // 180 in screenpoints
	double invRot1 = yaw *  invDist * PI / 180.0;       //    rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = invMt;
	perspect[1] = (void*)&(invDist);

	double params[5];
	params[0] = dist[3];
	params[1] = dist[2];
	params[2] = dist[1];
	params[3] = dist[0];

	params[4] = ((double)(srcWidth < srcHeight ? srcWidth : srcHeight)) / 2.0;

	for (int i = 0; i < REGION_HEIGHT; i++){
		for (int j = 0; j < REGION_WIDTH; j++){
			regionMap[i][j] = false;
		}
	}
	for (y = 0; y <= inputHeight; y += 1) {
		x_jump = (y == 0 || y == inputHeight || abs(y - inputHeight / 2) <= 5) ? 1 : inputWidth / 2;
		//printf("x_jump(%d), y(%d), H(%d), W(%d)\n", x_jump, y, inputHeight, inputWidth);
		for (x = 0; x <= inputWidth; x += x_jump) {
			x_d = (double)(x - crop_rect.x) - sw2;
			y_d = (double)(y - crop_rect.y) - sh2;

			inverse_lens_distortion(x_d, y_d, &Dx, &Dy, params);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_resize(x_d, y_d, &Dx, &Dy, invScale0, invScale1);

			if (lens_type == RECTLINEAR)
			{
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				inverse_rect_warp(x_d, y_d, &Dx, &Dy, invDist);
			}

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			spherical_warp(x_d, y_d, &Dx, &Dy, perspect);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_spherical(x_d, y_d, &Dx, &Dy, invDist);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			yaw_rotate_equirect(x_d, y_d, &Dx, &Dy, invRot1);

			if (output_type == WIDE_PANORAMA)
			{
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				pano_erect(x_d, y_d, &Dx, &Dy, invDist);
			}

			Dx += w2;
			Dy += h2;
			
			if (output_type == STEREO)
			{
				if (Dx < -outWidth * 2 + 1)
					Dx = Dx + outWidth * 2;
				if (Dx < 0)
					continue;
				if (Dx > outWidth * 2 - 1)
					Dx = Dx - outWidth * 2;
				if (Dx > outWidth - 1)
					continue;

				if (Dy < -outHeight * 2 + 1)
					Dy = Dy + outHeight * 2;
				if (Dy < 0)
					continue;
				if (Dy > outHeight * 2 - 1)
					Dy = Dy - outHeight * 2;
				if (Dy > outHeight - 1)
					continue;
			}
			else
			{
				if (Dx < 0.f){
					Dx = Dx + outWidth;
				}
				if (Dx > outWidth - 1){
					Dx = outWidth - Dx;
				}
				if (Dy < 0){
					Dy = 0;
				}
				if (Dy > outHeight - 1){
					Dy = outHeight - 1;
				}
			}

			double regionRectW = outWidth / REGION_WIDTH;
			double regionRectH = outHeight / REGION_HEIGHT;
			int indexW = Dx / regionRectW;
			int indexH = Dy / regionRectH;
			//printf("regionRectW(%lf), regionRectH(%lf), indexW(%d), indexH(%d)\n", regionRectW, regionRectH, indexW, indexH);
			regionMap[indexH][indexW] = true;
		}
	}
	/*for (int i = 0; i < REGION_HEIGHT; i++){
		for (int j = 0; j < REGION_WIDTH; j++){
			if (regionMap[i][j]) printf("===> test [%d][%d]\n", i, j);
		}
	}*/
	return WARP_SUCCESS;
}
int ComposeHelper::smallWarp(int input_width, int input_height, unsigned char* img_buf, unsigned char* buf, unsigned char* msk, cv::Rect crop_rect, int scale) {
	//printf("img_w %\d, %d, %d\n", img_w, img_h, scale);
	//printf("width %d, %d\n", width, height, scale);
	int rw = smallRoiW(scale), rh = smallRoiH(scale);
	int rx = smallRoiX(scale), ry = smallRoiY(scale);

	//printf("smallWarp %d, %d, %d, %d\n", rw, rh, rx, ry);

	cv::Mat test_out = cv::Mat(rh, rw, CV_8UC3, buf);
	cv::Mat test_org = cv::Mat(input_height, input_width, CV_8UC3, img_buf);

	int img_w = crop_rect.width;
	int img_h = crop_rect.height;

	float crop_scale = 0.8f;
	if (input_height >= img_h)
		crop_scale = 1.0f;

	float yaw = rt[0];
	float pitch = rt[1];
	float roll = rt[2];

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double mt[3][3];

	SetMatrix(-DEG_TO_RAD(pitch),
		0.0,
		-DEG_TO_RAD(roll),
		mt,
		0);

	double mpdist = (double)width / scale / b;
	double mpscale0 = (double)img_w / scale / a / mpdist;

	if (lens_type == RECTLINEAR)
	{
		mpscale0 = (double)img_w / scale / (2.0 * tan(a / 2.0)) / mpdist;
	}

	double mpscale1 = mpscale0;

	double mprot1 = -yaw * mpdist * PI / 180.0;

	void* perspect[2];
	perspect[0] = mt;
	perspect[1] = (void*)&(mpdist);
	double	mprad[6];

	double t = ((double)((img_w / scale) < (img_h / scale) ? (img_w / scale) : (img_h / scale))) / 2.0;
	double lens[5] = { lens_dist[3], lens_dist[2], lens_dist[1], lens_dist[0], t };

	cv::Vec<uchar, 3> *dstData = (cv::Vec<uchar, 3> *)test_out.data;

	for (int y = 0; y<rh; y++) {
		unsigned char *mb = msk + rw*y;
		for (int x = 0; x<rw; x++) {

			int nx = x + rx;
			int ny = y + ry;

			double 			w2 = (double)width / scale / 2.0 - 0.5;
			double 			h2 = (double)height / scale / 2.0 - 0.5;
			double 			sw2 = (double)img_w / scale / 2.0 - 0.5;
			double 			sh2 = (double)img_h / scale / 2.0 - 0.5;

			double sx = nx - w2, sy = ny - h2;
			double dx = nx - w2, dy = ny - h2;

			if (output_type == WIDE_PANORAMA)
			{
				erect_pano(dx, dy, &sx, &sy, mpdist);
				dx = sx, dy = sy;
				sx = 0, sy = 0;
			}

			yaw_rotate_equirect(dx, dy, &sx, &sy, mprot1);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			spherical_equirect(dx, dy, &sx, &sy, mpdist);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			spherical_warp(dx, dy, &sx, &sy, perspect);

			if (lens_type == RECTLINEAR)
			{
				dx = sx, dy = sy;
				sx = 0, sy = 0;
				rect_warp_sphere(dx, dy, &sx, &sy, mpdist);
			}

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			equirect_resize(dx, dy, &sx, &sy, mpscale0, mpscale1);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			lens_distortion(dx, dy, &sx, &sy, lens);

			sx += sw2;
			sy += sh2;

			sx += crop_rect.x / scale;
			sy += crop_rect.y / scale;

			if (sx >= 0 && sx < (input_width / scale) && sy >= 0 && sy < (input_height / scale))
			{
				if (lens_type == CIRCULAR_FISHEYE)
				{
					int midx = (crop_rect.width / 2 + crop_rect.x) / scale;
					int midy = (crop_rect.height / 2 + crop_rect.y) / scale;

					if (sqrt((midx - sx) * (midx - sx) + (midy - sy) * (midy - sy)) <= (crop_rect.width*crop_scale / 2) / scale)
					{
						dstData[y*rw + x] = test_org.at<cv::Vec<uchar, 3>>(sy*scale, sx*scale);
						*mb++ = 255;
					}
					else
					{
						*mb++ = 0;
					}
				}
				else
				{
					dstData[y*rw + x] = test_org.at<cv::Vec<uchar, 3>>(sy*scale, sx*scale);
					*mb++ = 255;
				}
			}
			else
			{
				*mb++ = 0;
			}
		}
	}
	return WARP_SUCCESS;
}
int ComposeHelper::computeCenterPoint(int inputWidth, int inputHeight, int outWidth, int outHeight, cv::Rect crop_rect, cv::Rect& ret_rect, cv::Point& center_point) {
	int x, y, x_jump;
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;

	int srcWidth = crop_rect.width;
	int srcHeight = crop_rect.height;

	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)srcWidth / 2.0 - 0.5;
	double sh2 = (double)srcHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double invDist = (double)outWidth / b;
	double invScale0 = 1.0f;
	if (lens_type == RECTLINEAR)
		invScale0 = 1.0 / ((double)srcWidth / (2.0 * tan(a / 2.0)) / invDist);
	else
		invScale0 = 1.0 / ((double)srcWidth / a / invDist);
	double invScale1 = invScale0;

	double invMt[3][3];

	SetMatrix(DEG_TO_RAD(pitch),
		0.0,
		DEG_TO_RAD(roll),
		invMt,
		1);

	double invRot0 = invDist * PI;                // 180 in screenpoints
	double invRot1 = yaw *  invDist * PI / 180.0;       //    rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = invMt;
	perspect[1] = (void*)&(invDist);

	double params[5];
	params[0] = dist[3];
	params[1] = dist[2];
	params[2] = dist[1];
	params[3] = dist[0];

	params[4] = ((double)(srcWidth < srcHeight ? srcWidth : srcHeight)) / 2.0;

	x_d = 0;//(double)(x - crop_rect.x) - sw2;
	y_d = 0;// (double)(y - crop_rect.y) - sh2;
	inverse_lens_distortion(x_d, y_d, &Dx, &Dy, params);

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	equirect_resize(x_d, y_d, &Dx, &Dy, invScale0, invScale1);

	if (lens_type == RECTLINEAR) {
		x_d = Dx; y_d = Dy;
		Dx = 0; Dy = 0;
		inverse_rect_warp(x_d, y_d, &Dx, &Dy, invDist);
	}

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	spherical_warp(x_d, y_d, &Dx, &Dy, perspect);

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	equirect_spherical(x_d, y_d, &Dx, &Dy, invDist);

	x_d = Dx; y_d = Dy;
	Dx = 0; Dy = 0;
	yaw_rotate_equirect(x_d, y_d, &Dx, &Dy, invRot1);

	if (output_type == WIDE_PANORAMA) {
		x_d = Dx; y_d = Dy;
		Dx = 0; Dy = 0;
		pano_erect(x_d, y_d, &Dx, &Dy, invDist);
	}

	Dx += w2;
	Dy += h2;

	//printf("Dx : %f, Dy : %f\n", Dx, Dy);

	if (Dx <= 100 || Dx >= 3740) center_point.x = (ret_rect.x + ret_rect.width / 2.0f);
	else center_point.x = Dx;
	if (Dy <= 100 || Dy >= 1820) center_point.y = (ret_rect.y + ret_rect.height / 2.0f);
	else center_point.y = Dy;
	return WARP_SUCCESS;
}

int ComposeHelper::computeRoiFit_makeInvWarpMap(int inputWidth, int inputHeight, int outWidth, int outHeight, cv::Rect crop_rect, cv::Rect& ret_rect, cv::Point& center_point, int* inv_warp_mask) {
	int x, y;
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;

	int srcWidth = crop_rect.width;
	int srcHeight = crop_rect.height;

	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)srcWidth / 2.0 - 0.5;
	double sh2 = (double)srcHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double invDist = (double)outWidth / b;
	double invScale0 = 1.0f;
	if (lens_type == RECTLINEAR)
		invScale0 = 1.0 / ((double)srcWidth / (2.0 * tan(a / 2.0)) / invDist);
	else 
		invScale0 = 1.0 / ((double)srcWidth / a / invDist);

	double invScale1 = invScale0;
	double invMt[3][3];

	SetMatrix(DEG_TO_RAD(pitch),
		0.0,
		DEG_TO_RAD(roll),
		invMt,
		1);

	double invRot0 = invDist * PI;                // 180 in screenpoints
	double invRot1 = yaw *  invDist * PI / 180.0;       //    rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = invMt;
	perspect[1] = (void*)&(invDist);

	double params[5];
	params[0] = dist[3];
	params[1] = dist[2];
	params[2] = dist[1];
	params[3] = dist[0];

	params[4] = ((double)(srcWidth < srcHeight ? srcWidth : srcHeight)) / 2.0;
	int max = 0;
	if (inv_warp_mask == NULL)
		inv_warp_mask = (int*)malloc(sizeof(int)* inputWidth * inputHeight * 2);
	//inv_warp_mask = (int*)malloc(sizeof(int)* inputWidth * inputHeight * 2);
	//memset(inv_warp_mask, -1, inputWidth * inputHeight * 2);
	for (y = 0; y < inputHeight; y += 1) {
		for (x = 0; x < inputWidth; x += 1) {
			x_d = (double)(x - crop_rect.x) - sw2;
			y_d = (double)(y - crop_rect.y) - sh2;
			inverse_lens_distortion(x_d, y_d, &Dx, &Dy, params);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_resize(x_d, y_d, &Dx, &Dy, invScale0, invScale1);

			if (lens_type == RECTLINEAR) {
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				inverse_rect_warp(x_d, y_d, &Dx, &Dy, invDist);
			}

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			spherical_warp(x_d, y_d, &Dx, &Dy, perspect);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_spherical(x_d, y_d, &Dx, &Dy, invDist);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			yaw_rotate_equirect(x_d, y_d, &Dx, &Dy, invRot1);

			if (output_type == WIDE_PANORAMA) {
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				pano_erect(x_d, y_d, &Dx, &Dy, invDist);
			}

			Dx += w2;
			Dy += h2;

			inv_warp_mask[(y*inputWidth + x) * 2] = (int)(Dx + 0.5);
			inv_warp_mask[(y*inputWidth + x) * 2 + 1] = (int)(Dy + 0.5);

			if (!isnan(Dx)) {
				if ((int)Dx < minX) minX = (int)(Dx + 0.5);
				if ((int)Dx > maxX) maxX = (int)(Dx + 0.5);
			}
			if (!isnan(Dy)) {
				if ((int)Dy < minY) minY = (int)(Dy + 0.5);
				if ((int)Dy > maxY) maxY = (int)(Dy + 0.5);
			}
			if ((y*inputWidth + x) * 2 + 1 > max)
				max = (y*inputWidth + x) * 2 + 1;
		}
	}

	
	if ((maxX - minX) > (outWidth * 0.95))
	{
		minX = 0;
		maxX = outWidth;
	}
	if ((maxY - minY) > (outHeight * 0.95))
	{
		minY = 0;
		maxY = outHeight;
	}
	if (minY < outHeight * 0.1)
	{
		minY = 0;
	}

	//printf("==> compute ROI: minX=%d, minY=%d, maxX=%d, maxY=%d \n", minX, minY, maxX, maxY);
	ret_rect.x = minX, ret_rect.y = minY, ret_rect.width = maxX - minX, ret_rect.height = maxY - minY;
	return WARP_SUCCESS;
}

int ComposeHelper::computeRoiFit(int inputWidth, int inputHeight, int outWidth, int outHeight, cv::Rect crop_rect, cv::Rect& ret_rect, cv::Point& center_point) {
	int x, y, x_jump;
	double x_d, y_d;
	double Dx, Dy;
	double Dx2, Dy2;

	int srcWidth = crop_rect.width;
	int srcHeight = crop_rect.height;

	double w2 = (double)outWidth / 2.0 - 0.5;
	double h2 = (double)outHeight / 2.0 - 0.5;
	double sw2 = (double)srcWidth / 2.0 - 0.5;
	double sh2 = (double)srcHeight / 2.0 - 0.5;

	int minX = outWidth - 1;
	int maxX = 0;
	int minY = outHeight - 1;
	int maxY = 0;

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	double invDist = (double)outWidth / b;

	double invScale0 = 1.0f;
	if (lens_type == RECTLINEAR)
		invScale0 = 1.0 / ((double)srcWidth / (2.0 * tan(a / 2.0)) / invDist);
	else
		invScale0 = 1.0 / ((double)srcWidth / a / invDist);
	double invScale1 = invScale0;

	double invMt[3][3];

	SetMatrix(DEG_TO_RAD(pitch),
		0.0,
		DEG_TO_RAD(roll),
		invMt,
		1);

	double invRot0 = invDist * PI;                // 180 in screenpoints
	double invRot1 = yaw *  invDist * PI / 180.0;       //    rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = invMt;
	perspect[1] = (void*)&(invDist);

	double params[5];
	params[0] = dist[3];
	params[1] = dist[2];
	params[2] = dist[1];
	params[3] = dist[0];

	params[4] = ((double)(srcWidth < srcHeight ? srcWidth : srcHeight)) / 2.0;
	for (y = 0; y <= inputHeight; y += 1) {
		x_jump = (y == 0 || y == inputHeight || abs(y - inputHeight / 2) <= 5) ? 1 : inputWidth / 2;
		for (x = 0; x <= inputWidth; x += x_jump) {
			x_d = (double)(x - crop_rect.x) - sw2;
			y_d = (double)(y - crop_rect.y) - sh2;
			inverse_lens_distortion(x_d, y_d, &Dx, &Dy, params);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_resize(x_d, y_d, &Dx, &Dy, invScale0, invScale1);

			if (lens_type == RECTLINEAR) {
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				inverse_rect_warp(x_d, y_d, &Dx, &Dy, invDist);
			}

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			spherical_warp(x_d, y_d, &Dx, &Dy, perspect);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			equirect_spherical(x_d, y_d, &Dx, &Dy, invDist);

			x_d = Dx; y_d = Dy;
			Dx = 0; Dy = 0;
			yaw_rotate_equirect(x_d, y_d, &Dx, &Dy, invRot1);

			if (output_type == WIDE_PANORAMA) {
				x_d = Dx; y_d = Dy;
				Dx = 0; Dy = 0;
				pano_erect(x_d, y_d, &Dx, &Dy, invDist);
			}

			Dx += w2;
			Dy += h2;
			//	inverse_warp_mask[(y*inputWidth + x) * 2] = (int)(Dx+0.5);
			//	inverse_warp_mask[(y*inputWidth + x) * 2 + 1] = (int)(Dy+0.5);

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

	if ((maxX - minX) > (outWidth * 0.95))
	{
		minX = 0;
		maxX = outWidth;
	}
	if ((maxY - minY) > (outHeight * 0.95))
	{
		minY = 0;
		maxY = outHeight;
	}
	if (minY < outHeight * 0.1)
	{
		minY = 0;
	}

	//printf("==> compute ROI: minX=%d, minY=%d, maxX=%d, maxY=%d \n", minX, minY, maxX, maxY);
	ret_rect.x = minX, ret_rect.y = minY, ret_rect.width = maxX - minX, ret_rect.height = maxY - minY;
	return WARP_SUCCESS;
}
#if 1
int ComposeHelper::makeWarpMap(int input_width, int input_height, int* warp_mask, cv::Rect crop_rect, int outputWidth, int outputHeight) {
	int rw = roiw, rh = roih;
	int rx = roix, ry = roiy;

	int img_w = crop_rect.width;
	int img_h = crop_rect.height;

	float yaw = rt[0];
	float pitch = rt[1];
	float roll = rt[2];

	float a = DEG_TO_RAD(fov);
	float b = DEG_TO_RAD(output_fov);

	float mt[3][3];

	//SetMatrix
	{
		float ptc = -DEG_TO_RAD(pitch);
		float yaw = 0.0;
		float rol = -DEG_TO_RAD(roll);
		int cl = 0;
		double mat_x[3][3], mat_y[3][3], mat_z[3][3], mat_temp[3][3];

		mat_x[0][0] = 1.0;                                mat_x[0][1] = 0.0;                                mat_x[0][2] = 0.0;
		mat_x[1][0] = 0.0;                                mat_x[1][1] = cos(ptc);							mat_x[1][2] = sin(ptc);
		mat_x[2][0] = 0.0;                                mat_x[2][1] = -mat_x[1][2];						mat_x[2][2] = mat_x[1][1];

		mat_y[0][0] = cos(yaw);                           mat_y[0][1] = 0.0;                                mat_y[0][2] = -sin(yaw);
		mat_y[1][0] = 0.0;                                mat_y[1][1] = 1.0;                                mat_y[1][2] = 0.0;
		mat_y[2][0] = -mat_y[0][2];						  mat_y[2][1] = 0.0;                                mat_y[2][2] = mat_y[0][0];

		mat_z[0][0] = cos(rol);							  mat_z[0][1] = sin(rol);							mat_z[0][2] = 0.0;
		mat_z[1][0] = -mat_z[0][1];						  mat_z[1][1] = mat_z[0][0];						mat_z[1][2] = 0.0;
		mat_z[2][0] = 0.0;                                mat_z[2][1] = 0.0;                                mat_z[2][2] = 1.0;

		for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			mt[i][j] = (mat_x[i][0] * mat_z[0][0] + mat_x[i][1] * mat_z[1][0] + mat_x[i][2] * mat_z[2][0]) * mat_y[0][j] +
				(mat_x[i][0] * mat_z[0][1] + mat_x[i][1] * mat_z[1][1] + mat_x[i][2] * mat_z[2][1]) * mat_y[1][j] +
				(mat_x[i][0] * mat_z[0][2] + mat_x[i][1] * mat_z[1][2] + mat_x[i][2] * mat_z[2][2]) * mat_y[2][j];
		}
	}
	float mpdist = (float)width / b;
	float mpscale0 = 1.0f;
	if (lens_type == RECTLINEAR)
		mpscale0 = (float)img_w / (2.0 * tan(a/2.0)) / mpdist;
	else
		mpscale0 = (float)img_w / a / mpdist;

	float mpscale1 = mpscale0;

	float mprot1 = -yaw * mpdist * PI / 180.0;

	float	mprad[6];

	float t = ((float)((img_w) < (img_h) ? (img_w) : (img_h))) / 2.0;
	float lens[5] = { lens_dist[3], lens_dist[2], lens_dist[1], lens_dist[0], t };

	memset(warp_mask, -1, outputWidth * outputHeight * 2 * 4);

	int nx, ny;
	float w2 = (float)width / 2.0 - 0.5;
	float h2 = (float)height / 2.0 - 0.5;
	float sw2 = (float)img_w / 2.0 - 0.5;
	float sh2 = (float)img_h / 2.0 - 0.5;
	float sx, sy, dx, dy;
	float phi, theta, r, s, v0, v1, v2;

	for (int y = 0; y < rh; y++) {
		ny = y + ry;
		for (int x = 0; x < rw; x++) {
			nx = x + rx;

			//erect_pano
			if (output_type == WIDE_PANORAMA) {
				dy = mpdist * atan(dy / mpdist);
			}

			//yaw_rotate_equirect
			dx = nx - w2 + mprot1;
			dy = ny - h2;


			//spherical_equirect
			phi = dx / mpdist;
			theta = -dy / mpdist + PI / 2;

			if (theta < 0)
			{
				theta = -theta;
				phi += PI;
			}
			if (theta > PI)
			{
				theta = 2 * PI - theta;
				phi += PI;
			}

			float sintheta = sin(theta);
			float sinphi = sin(phi);
			float costheta = cos(theta);
			float cosphi = cos(phi);

			v0 = sintheta * sinphi;
			v1 = costheta;
			r = sqrt(v1 * v1 + v0 * v0);
			theta = (mpdist * atan2(r, sintheta * cosphi)) / r;
			dx = theta * v0;
			dy = theta * v1;


			//spherical_warp
			r = sqrt(dx * dx + dy * dy) + FLT_MIN;
			theta = r / mpdist;
			s = sin(theta) / r;

			float tv0 = s * dx;
			float tv1 = s * dy;
			float tv2 = cos(theta);

			//matrix_inv_mult
			v0 = mt[0][0] * tv0 + mt[1][0] * tv1 + mt[2][0] * tv2;
			v1 = mt[0][1] * tv0 + mt[1][1] * tv1 + mt[2][1] * tv2;
			v2 = mt[0][2] * tv0 + mt[1][2] * tv1 + mt[2][2] * tv2;

			r = sqrt(v0 * v0 + v1 * v1) + FLT_MIN;
			theta = mpdist * atan2(r, v2) / r;
			dx = theta * v0;
			dy = theta * v1;


			//rect_warp_sphere
			if (lens_type == RECTLINEAR) {

				double  temp, rho;
				temp = sqrt(dx * dx + dy * dy) / mpdist;

				if (temp >= PI / 2.0)
					rho = 1.6e16;
				else if (temp == 0.0)
					rho = 1.0;
				else
					rho = tan(temp) / temp;
				dx = rho * dx;
				dy = rho * dy;
			}

			//equirect_resize
			dx = dx * mpscale0;
			dy = dy * mpscale1;

			//lens_distortion
			r = (sqrt(dx*dx + dy*dy)) / (lens[4]);
			s = ((lens[3] * r + lens[2]) * r + lens[1]) * r + lens[0];

			sx = s * dx;
			sy = s * dy;

			// final store warp_mask
			sx += sw2 + crop_rect.x;
			sy += sh2 + crop_rect.y;

			if (sx < 0 || sy < 0 || sx >= input_width || sy >= input_height)
				continue;

			if (lens_type == CIRCULAR_FISHEYE)
			{
				int midx = (crop_rect.width * 0.5 + crop_rect.x) - sx;
				int midy = (crop_rect.height * 0.5 + crop_rect.y) - sy;

				if (sqrt(midx*midx + midy*midy) >(crop_rect.width * 0.5))
					continue;
			}

			nx = (nx > width ? (nx - width) : (nx<0 ? (nx + width) : nx));
			ny = MAX(0, MIN(height - 1, ny));

			warp_mask[(ny*width + nx) * 2] = sx;
			warp_mask[(ny*width + nx) * 2 + 1] = sy;
		}
	}

	return WARP_SUCCESS;
}
#else
int QStitchHelper::makeWarpMap(int input_width, int input_height, int* warp_mask, cv::Rect crop_rect, int outputWidth, int outputHeight) {
	NICAEA_LOG(makeWarpMap start ...);

	int rw = roiw, rh = roih;
	int rx = roix, ry = roiy;

	int img_w = crop_rect.width;
	int img_h = crop_rect.height;

	float yaw = rt[0];
	float pitch = rt[1];
	float roll = rt[2];

	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(360);

	double mt[3][3];

	SetMatrix(-DEG_TO_RAD(pitch),
		0.0,
		-DEG_TO_RAD(roll),
		mt,
		0);

	double mpdist = (double)width / b;
	double mpscale0 = (double)img_w / a / mpdist;

	double mpscale1 = mpscale0;

	double mprot1 = -yaw * mpdist * PI / 180.0;

	void* perspect[2];
	perspect[0] = mt;
	perspect[1] = (void*)&(mpdist);
	double	mprad[6];

	double t = ((double)((img_w) < (img_h) ? (img_w) : (img_h))) / 2.0;
	double lens[5] = { lens_dist[3], lens_dist[2], lens_dist[1], lens_dist[0], t };

	memset(warp_mask, -1, outputWidth * outputHeight * 2 * 4);

	for (int y = 0; y<rh; y++) {
		for (int x = 0; x<rw; x++) {
			int nx = x + rx;
			int ny = y + ry;

			double 			w2 = (double)width / 2.0 - 0.5;
			double 			h2 = (double)height / 2.0 - 0.5;
			double 			sw2 = (double)img_w / 2.0 - 0.5;
			double 			sh2 = (double)img_h / 2.0 - 0.5;

			double sx = nx - w2, sy = ny - h2;
			double dx = nx - w2, dy = ny - h2;

			yaw_rotate_equirect(dx, dy, &sx, &sy, mprot1);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			spherical_equirect(dx, dy, &sx, &sy, mpdist);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			spherical_warp(dx, dy, &sx, &sy, perspect);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			equirect_resize(dx, dy, &sx, &sy, mpscale0, mpscale1);

			dx = sx, dy = sy;
			sx = 0, sy = 0;
			lens_distortion(dx, dy, &sx, &sy, lens);

			sx += sw2;
			sy += sh2;

			sx += crop_rect.x;
			sy += crop_rect.y;

			if (sx >= 0 && sx < (input_width) && sy >= 0 && sy < (input_height))
			{
				if (lens_type == CIRCULAR_FISH_TO_EQUIRECT)
				{
					int midx = (crop_rect.width / 2 + crop_rect.x);
					int midy = (crop_rect.height / 2 + crop_rect.y);

					if (sqrt((midx - sx) * (midx - sx) + (midy - sy) * (midy - sy)) <= (crop_rect.width / 2))
					{
						if (nx > width)
							nx = nx - width;
						else if (nx < 0)
							nx = nx + width;

						if (ny >= height)
							ny = height - 1;
						else if (ny < 0)
							ny = 0;

						warp_mask[(ny*width + nx) * 2] = sx;
						warp_mask[(ny*width + nx) * 2 + 1] = sy;
					}
				}
				else
				{
					if (nx > width)
						nx = nx - width;
					else if (nx < 0)
						nx = nx + width;

					if (ny >= height)
						ny = height - 1;
					else if (ny < 0)
						ny = 0;

					warp_mask[(ny*width + nx) * 2] = sx;
					warp_mask[(ny*width + nx) * 2 + 1] = sy;
				}
			}
		}
	}
	return WARP_SUCCESS;
}
#endif
int ComposeHelper::CalcDist(cv::Mat& input_mask, int input_width, int input_height, int x, int y, cv::Rect crop_rect, int scale) {
	double sx, sy;
	getSourceCoord(input_width, input_height, x, y, crop_rect, &sx, &sy, scale);

	int dist = ((sx - input_width / scale / 2)*(sx - input_width / scale / 2)) + ((sy - input_height / scale / 2)*(sy - input_height / scale / 2));

	if (sx >= 0 && sx < (input_width / scale) && sy >= 0 && sy < (input_height / scale))
	{
		cv::Vec<uchar, 3> pixel = input_mask.at<cv::Vec<uchar, 3>>(cv::Point(sx, sy));
		if (pixel[2] >(uchar)128)
		{
			return INT_MAX;
		}
		else if (pixel[1] >(uchar)128)
		{
			return 0;
		}
		else
		{
			if (lens_type == CIRCULAR_FISHEYE)
			{
				int midx = (crop_rect.width / 2 + crop_rect.x) / scale;
				int midy = (crop_rect.height / 2 + crop_rect.y) / scale;

				if (sqrt((midx - sx) * (midx - sx) + (midy - sy) * (midy - sy)) <= (crop_rect.width / 2) / scale)
				{
					return dist;
				}
				else
				{
					return INT_MAX;
				}
			}
			else
			{
				return dist;
			}
		}
	}
	else
		return INT_MAX;

}
int ComposeHelper::CalcDist(int input_width, int input_height, int x, int y, cv::Rect crop_rect, int scale) {

	double sx, sy;
	getSourceCoord(input_width, input_height, x, y, crop_rect, &sx, &sy, scale);

	int dist = ((sx - input_width / scale / 2)*(sx - input_width / scale / 2)) + ((sy - input_height / scale / 2)*(sy - input_height / scale / 2));

	if (sx >= 0 && sx < (input_width / scale) && sy >= 0 && sy < (input_height / scale))
	{
		//return dist;
		if (lens_type == CIRCULAR_FISHEYE)
		{
			int midx = (crop_rect.width / 2 + crop_rect.x) / scale;
			int midy = (crop_rect.height / 2 + crop_rect.y) / scale;

			if (sqrt((midx - sx) * (midx - sx) + (midy - sy) * (midy - sy)) <= (crop_rect.width / 2) / scale)
			{
				return dist;
			}
			else
			{
				return INT_MAX;
			}
		}
		else
		{
			return dist;
		}
	}
	else
		return INT_MAX;
}
int ComposeHelper::CalcDist(int input_width, int input_height, int x, int y) {
	double sx, sy;
	cv::Rect crop_rect(0, 0, input_width, input_height);
	getSourceCoord(input_width, input_height, x, y, crop_rect, &sx, &sy, 1.0);

	int dist = ((sx - input_width / 2)*(sx - input_width / 2)) + ((sy - input_height / 2)*(sy - input_height / 2));

	if (sx >= 0 && sx < (input_width) && sy >= 0 && sy < (input_height))
	{
		if (lens_type == CIRCULAR_FISHEYE)
		{
			int midx = (crop_rect.width / 2 + crop_rect.x);
			int midy = (crop_rect.height / 2 + crop_rect.y);

			if (sqrt((midx - sx) * (midx - sx) + (midy - sy) * (midy - sy)) <= (crop_rect.width / 2))
			{
				return dist;
			}
			else
			{
				return INT_MAX;
			}
		}
		else
		{
			return dist;
		}
		//return dist;
	}
	else
		return INT_MAX;
}
int ComposeHelper::getSourceCoord(int input_width, int input_height, int x, int y, cv::Rect crop_rect, double *ret_x, double *ret_y, int scale)
{
	float yaw = rt[0];
	float pitch = rt[1];
	float roll = rt[2];
	//printf("yaw=%f, pitch=%f, roll=%f \n", yaw, pitch, roll);
	double a = DEG_TO_RAD(fov);
	double b = DEG_TO_RAD(output_fov);

	int img_w = crop_rect.width;
	int img_h = crop_rect.height;

	double mt[3][3];

	SetMatrix(-DEG_TO_RAD(pitch),
		0.0,
		-DEG_TO_RAD(roll),
		mt,
		0);

	double mpdist = (double)width / scale / b;  // horizontal pixels per degree
	double mpscale0 = (double)img_w / scale / a / mpdist;	// calculate distance for this projection

	if (lens_type == RECTLINEAR)
	{
		mpscale0 = (double)img_w / scale / (2.0 * tan(a / 2.0)) / mpdist;
	}

	double mpscale1 = mpscale0;

	double mprot1 = -yaw * mpdist * PI / 180.0;		// rotation angle in screenpoints

	void* perspect[2];
	perspect[0] = mt;
	perspect[1] = (void*)&(mpdist);
	double	mprad[6];

	double t = ((double)((img_w / scale) < (img_h / scale) ? (img_w / scale) : (img_h / scale))) / 2.0;
	double lens[5] = { lens_dist[3], lens_dist[2], lens_dist[1], lens_dist[0], t };

	int nx = x;
	int ny = y;

	double 			w2 = (double)width / scale / 2.0 - 0.5;  // Steve's L
	double 			h2 = (double)height / scale / 2.0 - 0.5;
	double 			sw2 = (double)img_w / scale / 2.0 - 0.5;
	double 			sh2 = (double)img_h / scale / 2.0 - 0.5;

	//printf("width %d, height %d, sW %d, sh %d\n", width, height, img_w, img_h);
	double sx = nx - w2, sy = ny - h2;
	double dx = nx - w2, dy = ny - h2;

	if (output_type == WIDE_PANORAMA)
	{
		erect_pano(dx, dy, &sx, &sy, mpdist);
		dx = sx, dy = sy;
		sx = 0, sy = 0;
	}

	yaw_rotate_equirect(dx, dy, &sx, &sy, mprot1);

	dx = sx, dy = sy;
	sx = 0, sy = 0;
	spherical_equirect(dx, dy, &sx, &sy, mpdist);

	dx = sx, dy = sy;
	sx = 0, sy = 0;
	spherical_warp(dx, dy, &sx, &sy, perspect);

	if (lens_type == RECTLINEAR)
	{
		dx = sx, dy = sy;
		sx = 0, sy = 0;
		rect_warp_sphere(dx, dy, &sx, &sy, mpdist);
	}

	dx = sx, dy = sy;
	sx = 0, sy = 0;
	equirect_resize(dx, dy, &sx, &sy, mpscale0, mpscale1);

	dx = sx, dy = sy;
	sx = 0, sy = 0;
	lens_distortion(dx, dy, &sx, &sy, lens);

	sx += sw2;
	sy += sh2;

	sx += crop_rect.x / scale;
	sy += crop_rect.y / scale;

	*ret_x = sx;
	*ret_y = sy;

	return WARP_SUCCESS;
}