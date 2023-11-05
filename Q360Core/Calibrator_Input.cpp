#include "Calibrator_Input.h"

void* Calibrator_Input::getBufferAddress(int buffer_idx)
{
	return buffers[buffer_idx];
}

void Calibrator_Input::init(int num, int w, int h, int ch)
{
	num_buffers = num;
	width = w;
	height = h;
	channel = ch;

	buffers = (void**)malloc(sizeof(void*) * num_buffers);

	for (int i = 0; i < num_buffers; i++)
	{
		buffers[i] = malloc(width*height*ch);
	}
}

void Calibrator_Input::destroy()
{
	for (int i = 0; i < num_buffers; i++)
	{
		free(buffers[i]);
	}
	free(buffers);
}