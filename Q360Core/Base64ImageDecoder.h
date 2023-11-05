#ifndef __BASE_64_IMAGE_DECODER__
#define __BASE_64_IMAGE_DECODER__

#include <stdio.h>
#include "opencv2/highgui/highgui.hpp"

static char encoding_table[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
'w', 'x', 'y', 'z', '0', '1', '2', '3',
'4', '5', '6', '7', '8', '9', '+', '/' };

static int mod_table[] = { 0, 2, 1 };

struct Base64ImageDecoder {
	Base64ImageDecoder();

	char *decoding_table;

	void build_decoding_table();

	int base64_encode(const char *data, size_t input_length, char* encoded_data, size_t *output_length);
	cv::Mat base64_decode(const char *data, size_t input_length);
	cv::Mat Base64ImageDecoder::base64_decode_for_ROIMapSave(const char *data, size_t input_length);

	void Destroy();
};

#endif