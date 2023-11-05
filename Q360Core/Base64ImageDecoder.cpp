#include "Base64ImageDecoder.h"

Base64ImageDecoder::Base64ImageDecoder()
{
	decoding_table = NULL;
	if (decoding_table == NULL) build_decoding_table();
}

void Base64ImageDecoder::build_decoding_table() {
	decoding_table = (char*)malloc(256);
	for (int i = 0; i < 64; i++)
		decoding_table[(unsigned char)encoding_table[i]] = i;
}
int Base64ImageDecoder::base64_encode(const char *data, size_t input_length, char* encoded_data, size_t *output_length) {

	//*output_length = 4 * ((input_length + 2) / 3) + 1;
	//if (encoded_data != NULL){
	//	free(encoded_data); encoded_data = NULL;
	//}
	////printf("(%d) TEST---->2\n", *output_length);
	//encoded_data = (char *)malloc(*output_length);
	//memset(encoded_data, 0, *output_length);
	if (encoded_data == NULL) return NULL;
	int i, j;
	for (i = 0, j = 0; (unsigned int)i < input_length;) {

		unsigned int octet_a = (unsigned int)i < input_length ? (unsigned char)data[i++] : 0;
		unsigned int octet_b = (unsigned int)i < input_length ? (unsigned char)data[i++] : 0;
		unsigned int octet_c = (unsigned int)i < input_length ? (unsigned char)data[i++] : 0;

		unsigned int triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

		encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
		encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
	}
	for (i = 0; i < mod_table[input_length % 3]; i++)
		encoded_data[*output_length - 2 - i] = '=';
	return 1;
}

cv::Mat Base64ImageDecoder::base64_decode(const char *data,	size_t input_length) {
	cv::Mat img;
	//printf("bigheadk, base64_decode input_length %d\n", input_length);
	if (decoding_table == NULL) build_decoding_table();
	if (input_length % 4 != 0) return img;
	
	int output_length = 0;
	output_length = input_length / 4 * 3;
	if (data[input_length - 1] == '=') (output_length)--;
	if (data[input_length - 2] == '=') (output_length)--;
	//printf("bigheadk, base64_decode output_length %d\n", output_length);
	unsigned char *decoded_data = (unsigned char *)malloc(output_length);
	if (decoded_data == NULL) return img;
	for (int i = 0, j = 0; i < input_length;) {
		unsigned int sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int triple = (sextet_a << 3 * 6)
			+ (sextet_b << 2 * 6)
			+ (sextet_c << 1 * 6)
			+ (sextet_d << 0 * 6);
		if (j < output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
		if (j < output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
		if (j < output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
	}

	cv::Mat rawData = cv::Mat(1, output_length, CV_8UC1, decoded_data);
	img = cv::imdecode(rawData, cv::IMREAD_COLOR);
	free(decoded_data);

	//printf("bigheadk, mask detected imgrows = %d, ch = %d\n", img.rows, img.channels());

	return img;
}

cv::Mat Base64ImageDecoder::base64_decode_for_ROIMapSave(const char *data, size_t input_length) {
	cv::Mat img;
	//printf("bigheadk, base64_decode input_length %d\n", input_length);
	if (decoding_table == NULL) build_decoding_table();
	if (input_length % 4 != 0) return img;

	int output_length = 0;
	output_length = input_length / 4 * 3;
	if (data[input_length - 1] == '=') (output_length)--;
	if (data[input_length - 2] == '=') (output_length)--;
	//printf("bigheadk, base64_decode output_length %d\n", output_length);
	unsigned char *decoded_data = (unsigned char *)malloc(output_length);
	if (decoded_data == NULL) return img;
	for (int i = 0, j = 0; i < input_length;) {
		unsigned int sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
		unsigned int triple = (sextet_a << 3 * 6)
			+ (sextet_b << 2 * 6)
			+ (sextet_c << 1 * 6)
			+ (sextet_d << 0 * 6);
		if (j < output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
		if (j < output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
		if (j < output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
	}

	cv::Mat rawData = cv::Mat(1, output_length, CV_8UC1, decoded_data);
	img = cv::imdecode(rawData, cv::IMREAD_UNCHANGED);
	free(decoded_data);

	//printf("bigheadk, mask detected imgrows = %d, ch = %d\n", img.rows, img.channels());

	return img;
}

void Base64ImageDecoder::Destroy()
{
	if (decoding_table != NULL)
	{
		free(decoding_table);
		decoding_table = NULL;
	}
}