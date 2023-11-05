#include "EditPanorama.h"

void rotation_Image(cv::Vec3b input, int num_cam, PTSParam* result_param)
{
	cv::Mat intput_R = eulerAnglesToRotationMatrix(input);

	for (int i = 0; i < num_cam; i++){
		float yaw = (float)(result_param[i].y * PI) / 180;
		float pitch = (float)(result_param[i].p * PI) / 180;
		float roll = (float)(result_param[i].r * PI) / 180;
		cv::Vec3b theta(roll, pitch, yaw);
		cv::Mat cur_R = intput_R*eulerAnglesToRotationMatrix(theta);
		cv::Vec3b r = rotationMatrixToEulerAngles(cur_R);

		result_param[i].y = (r[2] * 180) / PI;
		result_param[i].p = (r[1] * 180) / PI;
		result_param[i].r = (r[0] * 180) / PI;
	}
}

cv::Vec3b straighten_Image(PTSParam* cams, int num_cam)
{
	printf("straighten_Image start\n");
	int center_index = 0, first_index = 0, second_index = 0;
	float center_yaw = 360.f, center_roll = 0.f, sum_roll = 0.f, avr_roll = 0.f, sum_pitch = 0.f, avr_pitch = 0.f;
	float second_yaw = 360.f, second_roll = 0.f;
	bool isReverse = false;
	for (int i = 0; i < num_cam; i++)
	{
		//printf("cams[%d] y=%f, p=%f \n", i, cams[i].y, cams[i].p);
		if (center_yaw > abs(cams[i].y))
		{	
			first_index = i;
			center_yaw = abs(cams[i].y);
		}
		else if (second_yaw > abs(cams[i].y))
		{
			second_index = i;
			second_yaw = abs(cams[i].y);
		}
		sum_roll += cams[i].r;
		if (cams[i].p < 0)
			sum_pitch += (cams[i].p + 180);
		else
			sum_pitch += cams[i].p;
	}

	avr_pitch = sum_pitch / (num_cam );
	//printf("avr_pitch=%f, cams[%d].p =%f, cams[%d].p=%f\n", avr_pitch, first_index, cams[first_index].p, second_index,cams[second_index].p);
	float first_pitch, second_pitch;
	if (cams[first_index].p < 0)
		first_pitch = cams[first_index].p + 180;
	else
		first_pitch = cams[first_index].p;
	if (cams[second_index].p < 0)
		second_pitch = cams[second_index].p + 180;
	else
		second_pitch = cams[second_index].p;
	if (abs(first_pitch - avr_pitch) <= abs(second_pitch - avr_pitch))
		center_index = first_index;
	else
		center_index = second_index;
	//printf("cams[center_index].y=%f, cams[center_index].p=%f, cams[center_index].r=%f\n", cams[center_index].y, cams[center_index].p, cams[center_index].r);
	avr_roll = sum_roll / num_cam;

	if (cams[center_index].r < 0)
		isReverse = true;
	float iterator = 1.f;
	float pix_roll = 0.5f;
	int pitch_count = 0;
	int roll_count = 0;
	float c_yaw, c_pitch, c_roll, prev_roll, prev_pitch;

	cv::Mat c_r, c_t, m, r, pix_r;
	cv::Vec3b v;
	float best_error = 1000.f;
	float best_yaw, best_pitch, best_roll, best_yaw2, best_pitch2, best_roll2;
	while (1)
	{
		pitch_count++;
		roll_count = pitch_count / (180 / iterator);
		if (roll_count > (180 / pix_roll))
			break;
		c_yaw = (float)(cams[center_index].y * PI) / 180;
		c_pitch = (float)(cams[center_index].p * PI) / 180;
		c_roll = (float)(cams[center_index].r * PI) / 180;
		c_r = eulerAnglesToRotationMatrix(cv::Vec3b(c_roll, c_pitch, c_yaw));

		prev_roll = pix_roll * roll_count;
		//if (prev_roll > 90.f)
		//	prev_roll -= 180.f;
		if (isReverse) 
			prev_roll -= 180;
		
		prev_pitch = cams[center_index].p + iterator;

		if (prev_pitch > 90.f)
			prev_pitch -= 180.f;
		

		c_roll = (float)(prev_roll * PI) / 180;
		c_pitch = (float)(prev_pitch * PI) / 180;
		c_t = eulerAnglesToRotationMatrix(cv::Vec3b(c_roll, c_pitch, c_yaw));
		float yaw, pitch, roll;
		m = c_t * c_r.inv();
		int good_count = 0;
		float sum_error = 0.f;
		for (int i = 0; i < num_cam; i++)
		{
			yaw = (float)(cams[i].y * PI) / 180;
			pitch = (float)(cams[i].p * PI) / 180;
			roll = (float)(cams[i].r * PI) / 180;

			r = eulerAnglesToRotationMatrix(cv::Vec3b(roll, pitch, yaw));
			pix_r = m * r;
			v = rotationMatrixToEulerAngles(pix_r);

			cams[i].y = (v[2] * 180) / PI;
			cams[i].p = (v[1] * 180) / PI;
			cams[i].r = (v[0] * 180) / PI;
			float roll_t = abs(cams[i].r);
			if (i == center_index) {
				continue;
			}

			//if (cams[i].isValued)
			sum_error += abs(abs(prev_roll) - roll_t);
		}
		if (sum_error < best_error && (abs(cams[center_index].p) < 20)) {
		//if (sum_error < best_error ) {
			best_error = sum_error;
			best_yaw = cams[center_index].y;
			best_pitch = cams[center_index].p;
			best_roll = cams[center_index].r;
		}
		//printf("cams[center_index].r=%f,  sum_error=%f\n", cams[center_index].r, sum_error);
	}

	c_yaw = (float)(cams[center_index].y * PI) / 180;
	c_pitch = (float)(cams[center_index].p * PI) / 180;
	c_roll = (float)(cams[center_index].r * PI) / 180;
	c_r = eulerAnglesToRotationMatrix(cv::Vec3b(c_roll, c_pitch, c_yaw));

	c_yaw = (float)(best_yaw * PI) / 180;
	c_pitch = (float)(best_pitch * PI) / 180;
	c_roll = (float)(best_roll * PI) / 180;
	//printf("center_index=%d, best_roll=%f\n", center_index, best_roll);
	c_t = eulerAnglesToRotationMatrix(cv::Vec3b(c_roll, c_pitch, c_yaw));
	m = c_t * c_r.inv();

	cv::Vec3b degree = rotationMatrixToEulerAngles(m);

	printf("straighten_Image end\n");
	return degree;
}

cv::Mat eulerAnglesToRotationMatrix(cv::Vec3b theta)
{
	cv::Mat roll = (cv::Mat_<double>(3, 3) <<
		1, 0, 0,
		0, cos(theta[0]), -sin(theta[0]),
		0, sin(theta[0]), cos(theta[0])
		);

	cv::Mat pitch = (cv::Mat_<double>(3, 3) <<
		cos(theta[1]), 0, sin(theta[1]),
		0, 1, 0,
		-sin(theta[1]), 0, cos(theta[1])
		);

	cv::Mat yaw = (cv::Mat_<double>(3, 3) <<
		cos(theta[2]), -sin(theta[2]), 0,
		sin(theta[2]), cos(theta[2]), 0,
		0, 0, 1);

	cv::Mat R = yaw * pitch * roll;

	return R;
}
cv::Vec3b rotationMatrixToEulerAngles(cv::Mat &R)
{

	//assert(isRotationMatrix(R));

	float sy = sqrt(R.at<double>(0, 0) * R.at<double>(0, 0) + R.at<double>(1, 0) * R.at<double>(1, 0));

	bool singular = sy < 1e-6; // If

	float roll, pitch, yaw;
	if (!singular)
	{
		roll = atan2(R.at<double>(2, 1), R.at<double>(2, 2));
		pitch = atan2(-R.at<double>(2, 0), sy);
		yaw = atan2(R.at<double>(1, 0), R.at<double>(0, 0));
	}
	else
	{
		roll = atan2(-R.at<double>(1, 2), R.at<double>(1, 1));
		pitch = atan2(-R.at<double>(2, 0), sy);
		yaw = 0;
	}
	return cv::Vec3b(roll, pitch, yaw);
}
