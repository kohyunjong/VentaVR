#ifndef __INPUT_MANAGER_QURAM_H__
#define __INPUT_MANAGER_QURAM_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// modify input module structure... 161226~
#define CAMERA_MAX 16

#define INIT			0x00
#define EMPTY			0x01
#define CAPTURED		0x02
#define BUFFERING		1

struct DATA_STATE
{
	int* captured;
	int* rendered;

	long long* time_stamp;

	int	in_idx;
	int out_idx;
};

struct InputManager 
{
	InputManager(int num_camera, int num_channels, int num_buffers)
	{
		numCamera = num_camera;
		numChannels = num_channels;
		numBuffers = num_buffers;

		globalInIdx = 0;
		globalOutIdx = 0;

		checkBuffers = 0;
		initBuffering = 0;
		bufferCount = 0;

		initFirstBuffering = 0;
		mOutDelay = 0;

		data_state = (DATA_STATE*) malloc(numCamera*numChannels*sizeof(DATA_STATE));
		outIdx = (int**)malloc(numChannels*sizeof(int*));
		for (int i = 0; i < numCamera*numChannels; i++)
		{
			data_state[i].captured = (int*) malloc(sizeof(int) * numBuffers);
			data_state[i].rendered = (int*) malloc(sizeof(int) * numBuffers);
			data_state[i].time_stamp = (long long*)malloc(sizeof(long long)* numBuffers);

			data_state[i].in_idx = 0;
			data_state[i].out_idx = 0;

			memset(data_state[i].captured, 0, sizeof(int) * numBuffers);
			memset(data_state[i].rendered, 0, sizeof(int) * numBuffers);
			memset(data_state[i].time_stamp, 0, sizeof(long long)* numBuffers);
		}
		for (int i = 0; i < numChannels; i++)
		{
			outIdx[i] = (int*)malloc(sizeof(int)*numCamera);
		}
		//memset(outIdx, 0, sizeof(int)*numCamera*numChannels);
	}

	void	finishCaptured(int camera_idx, int channel_idx, long long time_stamp);
	int		getInIndex(int camera_idx, int channel_idx);
	void	destroy();
	int*		getOutIndex(int channel);
	int		updateIndex(int idx);
	long long		getTimeStamp(int idx, int channel);
	int		isFinishBuffering();
	int		isFirstBuffering();

	int		setBufferoffset(int offset);

	DATA_STATE* data_state;

	int numCamera;
	int numChannels;
	int numBuffers;

	int checkBuffers;

	int globalInIdx;
	int globalOutIdx;

	int**	outIdx;	

	int		initBuffering;
	int		bufferCount;

	int		initFirstBuffering;
	int		mOutDelay;
};

#endif