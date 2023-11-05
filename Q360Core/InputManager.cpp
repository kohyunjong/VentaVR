#include <InputManager.h>

void InputManager::destroy()
{
	for (int i = 0; i < numCamera*numChannels; i++)
	{
		free(data_state[i].captured);
		free(data_state[i].rendered);
		free(data_state[i].time_stamp);
	}
	free(data_state);
}

int InputManager::getInIndex(int camera_idx, int channel_idx)
{
	int idx = (channel_idx*numCamera) + camera_idx;
	int inidx = data_state[idx].in_idx;

	if (channel_idx == 1 && inidx > 0 && initFirstBuffering == 0)
	{
		initFirstBuffering = 1;
	}
	data_state[idx].captured[inidx] = EMPTY;
	return inidx;
}

long long InputManager::getTimeStamp(int idx, int channel)
{
	for (int i = 0; i < numCamera; i++)
	{
		if (data_state[channel*numCamera + i].captured[idx] == CAPTURED)
		{
			return data_state[channel*numCamera + i].time_stamp[idx];
		}
	}
}

int* InputManager::getOutIndex(int channel)
{
	int* ret = outIdx[channel];
	int back_idx = 0;
	if (channel == 0)
	{
		back_idx = BUFFERING + mOutDelay;
	}
	else if (channel == 1)
	{
		back_idx = 1;
	}

	for (int i = 0; i < numCamera; i++)
	{
		int idx = (channel*numCamera) + i;

		ret[i] = (data_state[idx].in_idx+numBuffers-back_idx) % numBuffers;
	}
	return ret;
}

void InputManager::finishCaptured(int camera_idx, int channel_idx, long long time_stamp)
{
	int idx = (channel_idx*numCamera) + camera_idx;
	int inidx = data_state[idx].in_idx;

	data_state[idx].captured[inidx] = CAPTURED;
	data_state[idx].time_stamp[inidx] = time_stamp;
	data_state[idx].in_idx = updateIndex(data_state[idx].in_idx);

	if (!initBuffering)
	{
		if (bufferCount < data_state[idx].in_idx)
			bufferCount = data_state[idx].in_idx;
		if (bufferCount > BUFFERING)
		{
			printf("isFinishBuffering\n");
			initBuffering = 1;
		}
	}
}

int	InputManager::isFinishBuffering()
{
	return initBuffering;
}

int InputManager::updateIndex(int idx)
{
	return (idx + 1) % numBuffers;
}

int	InputManager::isFirstBuffering()
{
	return initFirstBuffering;
}

int InputManager::setBufferoffset(int offset)
{
	mOutDelay -= offset;

	if (mOutDelay < 0)
		mOutDelay = 0;
	if (mOutDelay > numBuffers - 7)
		mOutDelay = numBuffers - 7;

	return mOutDelay;
}