#include "BMOutput.h"

// public functions //

BMOutput::BMOutput() {

}

BMOutput* BMOutput::create_BMOutput(int width, int height, int outputDevice, int outputDisplay) {
	BMOutput* instance = new BMOutput();

	if (instance && instance->init_BMOutput(width, height, outputDevice, outputDisplay) == S_FALSE) {
		delete instance;
		return NULL;
	}

	return instance;
}

BOOL BMOutput::destroy_BMOutput(BMOutput* instance) {
	instance->StopPlayback();

	// Release the video input interface
	if (instance->deckLinkOutput != NULL)
		instance->deckLinkOutput->Release();

	// Release the Decklink object
	if (instance->deckLink != NULL)
		instance->deckLink->Release();

	// Release the DeckLink iterator
	if (instance->deckLinkIterator != NULL)
		instance->deckLinkIterator->Release();

	// Release the videoframe object
	if (instance->videoFrame != NULL)
		instance->videoFrame->Release();

	// Release the outputCallback callback object
	if (instance->outputCallback)
		delete instance->outputCallback;

	// Free the output buffer
	if (instance->mOutputBuffers)
		free(instance->mOutputBuffers);

	delete instance;

	return S_OK;
}

BOOL BMOutput::init_BMOutput(int width, int height, int outputDevice, int outputDisplay) {
	mOutputDevice = outputDevice;
	mOutputDisplay = outputDisplay;
	mOutputWidth = width;
	mOutputHeight = height;
	mIsStop = false;
	mOutputBuffers = NULL;
	mOutputIndex = 0;
	mTime = QClock();

#ifdef SDI_OUTPUT_TEST
	mpGetOutputAddressCallback = NULL;
	mpGetOutputAddressCallbackData = NULL;
#endif

	do {
		HRESULT result;

		if (outputDevice == 0)
		{
			fprintf(stderr, "Selection No Device.\n");
			return S_FALSE;
		}

		Initialize();

		// Create an IDeckLinkIterator object to enumerate all DeckLink cards in the system
		result = GetDeckLinkIterator(&deckLinkIterator);
		if (result != S_OK)
		{
			fprintf(stderr, "A DeckLink iterator could not be created.  The DeckLink drivers may not be installed.\n");
			break;
		}

		// Create an IDeckLinkVideoConversion object
		result = CoCreateInstance(CLSID_CDeckLinkVideoConversion, NULL, CLSCTX_ALL, IID_IDeckLinkVideoConversion, (void**)&deckLinkVideoConversion);
		if (result != S_OK)
		{
			fprintf(stderr, "A IDeckLinkVideoConversion could not be created.\n");
			break;
		}

		// Obtain the first DeckLink device
		for (int i = 0; i < mOutputDevice; i++) {
			result = deckLinkIterator->Next(&deckLink);
			if (result != S_OK)
			{
				fprintf(stderr, "Could not find DeckLink device - result = %08x\n", result);
				break;
			}
		}

		// Obtain the output interface for the DeckLink device
		result = deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&deckLinkOutput);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not obtain the IDeckLinkInput interface - result = %08x\n", result);
			break;
		}

		// Create an instance of output callback
		outputCallback = new OutputCallback(deckLinkOutput, onSchedule, this);
		if (outputCallback == NULL)
		{
			fprintf(stderr, "Could not create output callback object\n");
			break;
		}

		// Set the callback object to the DeckLink device's output interface
		result = deckLinkOutput->SetScheduledFrameCompletionCallback(outputCallback);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not set callback - result = %08x\n", result);
			break;
		}

		mOutputBuffers = (void**)malloc(sizeof(void*) * NUM_OUTPUT_BUFFERS);
		for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++) {
			mOutputBuffers[i] = malloc(mOutputWidth * mOutputHeight * 3);
			memset(mOutputBuffers[i], 0, mOutputWidth * mOutputHeight * 3);
		}
		if (mOutputBuffers == NULL)
		{
			fprintf(stderr, "Could not malloc output buffer\n");
			break;
		}

		result = StartPlayback();
		if (result != S_OK)
		{
			fprintf(stderr, "Could not start playback\n", result);
			break;
		}

		return S_OK;
	} while (false);

	// Release the video input interface
	if (deckLinkOutput != NULL) {
		deckLinkOutput->Release();
		deckLinkOutput = NULL;
	}

	// Release the Decklink object
	if (deckLink != NULL) {
		deckLink->Release();
		deckLink = NULL;
	}

	// Release the DeckLink iterator
	if (deckLinkIterator != NULL) {
		deckLinkIterator->Release();
		deckLinkIterator = NULL;
	}

	// Release the videoframe object
	if (videoFrame != NULL) {
		videoFrame->Release();
		videoFrame = NULL;
	}

	// Release the outputCallback callback object
	if (outputCallback) {
		delete outputCallback;
		outputCallback = NULL;
	}

	// Free the output buffer
	if (mOutputBuffers) {
		free(mOutputBuffers);
		mOutputBuffers = NULL;
	}

	return S_FALSE;
}

#ifdef SDI_OUTPUT_TEST
void BMOutput::setCallback_getOutputAddress(callback_getOutputAddress cb, void* data) {
	mpGetOutputAddressCallback = cb;
	mpGetOutputAddressCallbackData = data;
}
#endif

void* BMOutput::getBuffer() {
	return mOutputBuffers[mOutputIndex];
}

void* BMOutput::getLastBuffer() {
	return mOutputBuffers[((mOutputIndex - 1) + NUM_OUTPUT_BUFFERS) % NUM_OUTPUT_BUFFERS];
}

void BMOutput::putBuffer(void* buffer) {
	memcpy(mOutputBuffers[mOutputIndex], buffer, mOutputWidth * mOutputHeight * 3);
	mOutputIndex++;
	if (mOutputIndex == NUM_OUTPUT_BUFFERS)
		mOutputIndex = 0;
}

void BMOutput::finishBufferCopy() {
	mOutputIndex++;
	if (mOutputIndex == NUM_OUTPUT_BUFFERS)
		mOutputIndex = 0;
}

HRESULT BMOutput::StartPlayback() {
	do {
		mIsStop = false;

		BMDDisplayModeSupport supported;
		HRESULT result;

		IDeckLinkDisplayModeIterator* temp;
		deckLinkOutput->GetDisplayModeIterator(&temp);

		// Check the selected display mode/pixelformat can be output 
		result = deckLinkOutput->DoesSupportVideoMode(kDisplayMode, kPixelFormat, kOutputFlags, &supported, &gDisplayMode);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not determine whether video mode is supported - result = %08x\n", result);
			break;
		}

		if (supported == bmdDisplayModeNotSupported)
		{
			fprintf(stderr, "Video mode is not supported\n");
			break;
		}

		// Store the frame duration and timesale for later use
		gDisplayMode->GetFrameRate(&gFrameDuration, &gTimeScale);

		// Enable video output
		result = deckLinkOutput->EnableVideoOutput(kDisplayMode, kOutputFlags);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not enable video output - result = %08x\n", result);
			break;
		}

		// Start
		result = deckLinkOutput->StartScheduledPlayback(0, gTimeScale, 1.0);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not start - result = %08x\n", result);
			break;
		}

		CreateThread(NULL, 0, run, this, 0, NULL);

		printf("Start Playback\n");

		return S_OK;
	} while (false);

	return S_FALSE;
}

HRESULT BMOutput::StopPlayback() {
	do {
		mIsStop = true;

		HRESULT result;

		// Stop capture
		result = deckLinkOutput->StopScheduledPlayback(0, NULL, 0);

		// Disable the video input interface
		result = deckLinkOutput->DisableVideoOutput();

		gTotalFramesScheduled = 0;

		printf("Stop Playback\n");

		return S_OK;
	} while (false);

	return S_FALSE;
}

// private functions //

bool BMOutput::isStop() {
	return mIsStop;
}

DWORD WINAPI BMOutput::run(LPVOID handle) {
	BMOutput* info = (BMOutput*)handle;

	printf("BMOutput RUN (170622)\n");

	HRESULT result;

	HANDLE hTimerEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	double sleeptime = 1000.0f * info->gFrameDuration / info->gTimeScale;

	double interval = sleeptime;
	double actual_interval = interval;
	int numframe = 0;
	double start_time = 0;

	while (true) {
		if (info->isStop()) {
			break;
		}

		// Release the videoframe object
		if (info->videoFrame != NULL) {
			info->videoFrame->Release();
			info->videoFrame = NULL;
		}

		// Create a frame with defined format
		//videoFrameBlue = CreateFrame(deckLinkOutput);
		IDeckLinkMutableVideoFrame* frame = NULL;
		result = info->deckLinkOutput->CreateVideoFrame((int)info->gDisplayMode->GetWidth(), (int)info->gDisplayMode->GetHeight(), info->kRowBytes, info->kPixelFormat, bmdFrameFlagDefault, &frame);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not create a video frame - result = %08x\n", result);
			continue;
		}

#ifdef SDI_OUTPUT_TEST
		void* ptr0 = info->getOutputAddress(info, info->mpGetOutputAddressCallbackData);
		if (ptr0 == NULL)
			return S_FALSE;
#endif

		cv::Mat RGBInput = cv::Mat(cv::Size(info->mOutputWidth, info->mOutputHeight), CV_8UC3);
		RGBInput.data = (uchar*)info->getLastBuffer();

		//cv::imshow("RGB", RGBInput);
		//cv::waitKey();

		IDeckLinkMutableVideoFrame* frameBGRA = NULL;
		result = info->deckLinkOutput->CreateVideoFrame(info->mOutputWidth, info->mOutputHeight, info->mOutputWidth * 4, bmdFormat8BitBGRA, bmdFrameFlagDefault, &frameBGRA);

		void* bytes;
		frameBGRA->GetBytes(&bytes);

		cv::Mat BGRAInput = cv::Mat(cv::Size(info->mOutputWidth, info->mOutputHeight), CV_8UC4);
		BGRAInput.data = (uchar*)bytes;

		cv::cvtColor(RGBInput, BGRAInput, CV_RGB2BGRA);

		result = info->deckLinkVideoConversion->ConvertFrame(frameBGRA, frame);

		if (frameBGRA != NULL) {
			frameBGRA->Release();
			frameBGRA = NULL;
		}

		info->videoFrame = frame;

		result = info->outputCallback->scheduleNextFrame(info->videoFrame);
		if (result != S_OK)
		{
			fprintf(stderr, "Could not schedule video frame - result = %08x %p\n", result, info->videoFrame);
			continue;
		}

		while (abs(info->mTime - QClock()) < actual_interval)
		{

		}

		info->mTime = QClock();

		if (numframe == 0)
		{
			start_time = QClock();
		}
		else
		{
			actual_interval = interval - ((abs(start_time - QClock()) / (double)numframe) - interval) * numframe*0.5;
		}
	}

	return 1;
}

HRESULT BMOutput::onSchedule(void* handle, IDeckLinkVideoFrame* videoFrame) {
	BMOutput *info = (BMOutput *)handle;

	HRESULT result;

	result = info->deckLinkOutput->ScheduleVideoFrame(videoFrame, info->gTotalFramesScheduled * info->gFrameDuration, info->gFrameDuration, info->gTimeScale);
	if (result != S_OK)
		return result;

	info->gTotalFramesScheduled++;

	return result;
}

#ifdef SDI_OUTPUT_TEST
BYTE* BMOutput::getOutputAddress(void* handle, void* data) {
	if (mpGetOutputAddressCallback)
		return (BYTE*)mpGetOutputAddressCallback(handle, data);
	else {
		printf("ERROR: getOutputAddress callback : NULL.\n");
		return NULL;
	}
}
#endif