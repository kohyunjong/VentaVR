#pragma once

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct Calibrator_Input
{
	void**	buffers;
	int		num_buffers;
	int		width, height;
	int		channel;	

	Calibrator_Input()
	{
		width = 0, height = 0;
	}

	void* getBufferAddress(int buffer_idx);
	void init(int num, int w, int h, int ch);
	void destroy();

};