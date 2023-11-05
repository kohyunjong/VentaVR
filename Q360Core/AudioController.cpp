#include "AudioController.h"

#include <stdio.h>

unsigned int __stdcall run_thread(void* param);

AudioController::AudioController() {

}

AudioController* AudioController::create_AudioController(Composer* composer) {
	AudioController* instance = new AudioController();

	if (instance->init_AudioController(composer) == 0) {
		delete instance;
		return NULL;
	}

	return instance;
}

int AudioController::destroy_AudioController(AudioController* instance) {
	if (instance) {
		instance->mIsClear = true;

		if (instance->mAudioPlayer) {
			delete instance->mAudioPlayer;
			instance->mAudioPlayer = NULL;
		}

		for (int i = 0; i < instance->mPushBuffer.size(); i++) {
			unsigned char* buffer = instance->mPushBuffer.front();
			free(buffer);
			instance->mPushBuffer.pop();
		}

		if (instance->mAudioHeader != NULL)
		{
			free(instance->mAudioHeader);
			instance->mAudioHeader = NULL;
		}

		delete instance;
	}

	return 1;
}

int AudioController::init_AudioController(Composer* composer) {
	mComposer = composer;
	mIsClear = false;
	mIsAudioHeaderHDD = 0;
	mIsAudioHeaderRTMP = 0;
	mIsNeedReset = 1;
	mhOutputThread = NULL;

	m_wfHeader.dwRiffFlag = 'FFIR';
	m_wfHeader.dwFileSize = sizeof(AUDIO_CONTROLLER_HEADER)-12;

	m_wfHeader.dwWaveFlag = 'EVAW';
	m_wfHeader.dwFmtFlag = ' tmf';
	m_wfHeader.dwFmtSize = sizeof(WORD)+sizeof(WORD)+sizeof(DWORD)+sizeof(DWORD)+sizeof(WORD)+sizeof(WORD);

	m_wfHeader.wBitsPerSample = 16;
	m_wfHeader.wFormatTag = WAVE_FORMAT_PCM;
	m_wfHeader.nChannels = 2;
	m_wfHeader.nSamplesPerSec = 48000;
	m_wfHeader.nBlockAlign = m_wfHeader.wBitsPerSample * 2 / 8;
	m_wfHeader.nAvgBytesPerSec = m_wfHeader.nBlockAlign * 48000;

	m_wfHeader.dwDataFlag = 'atad';
	m_wfHeader.dwDataSize = 0;

	mAudioHeader = (unsigned char*)malloc(sizeof(AUDIO_CONTROLLER_HEADER));
	memcpy(mAudioHeader, &m_wfHeader, sizeof(AUDIO_CONTROLLER_HEADER));

	mAudioHeaderSize = sizeof(AUDIO_CONTROLLER_HEADER);

	mAudioPlayer = new AudioPlayer();
	mAudioPlayer->setCallback_onPush(onPush, this);
	mAudioPlayer->LoadWaveData(
		m_wfHeader.wFormatTag,
		m_wfHeader.nChannels,
		m_wfHeader.nSamplesPerSec,
		m_wfHeader.nAvgBytesPerSec,
		m_wfHeader.nBlockAlign,
		m_wfHeader.wBitsPerSample);
	mAudioPlayer->Play();

	((StitchComposer*)mComposer)->availableSound = AVAILABLE_SOUND;

	unsigned int threadId;
	unsigned long threadPtr;
	//mhOutputThread = CreateThread(NULL, 0, run, this, 0, NULL);
	threadPtr = _beginthreadex(NULL, 0, &run_thread, (void*)(this), 0, &threadId);
	SetThreadPriority(OpenThread(THREAD_ALL_ACCESS, NULL, threadId), THREAD_PRIORITY_HIGHEST);//THREAD_PRIORITY_HIGHEST
	if (mhOutputThread == NULL) {
		printf("ERROR: Can't create output thread!\n");
	}

	return 1;
}

bool AudioController::isClear() {
	return mIsClear;
}

int AudioController::push(void* buffer, int size) {
	if (mIsNeedReset == 1) {
		mAudioPlayer->ResetBufferPosition();

		mIsNeedReset = 0;
	}

	mAudioPlayer->PushBuffer(buffer, size);

	//if (((StitchComposer*)mComposer)->isEncoding == QVS_MSG_ENCODING)
	//{
	//	if (mIsAudioHeaderHDD == 0)
	//	{
	//		//printf("push 1 START\n");
	//		mComposer->audio_push(mAudioHeader, mAudioHeaderSize, QVS_MSG_ENCODING);
	//		//mComposer->audio_push(audioCapture->getHeaderBuffer(), audioCapture->getHeaderBufferSize(), QVS_MSG_ENCODING);
	//		mComposer->audio_push(mAudioPlayer->getBuffer(), mAudioPlayer->getBufferSize(), QVS_MSG_ENCODING);
	//		//mComposer->audio_push((unsigned char*)buffer, size, QVS_MSG_ENCODING);
	//		//printf("push 1 END\n");
	//		mIsAudioHeaderHDD = 1;
	//	}
	//	else
	//	{
	//		//printf("push 2 START\n");
	//		mComposer->audio_push(mAudioPlayer->getBuffer(), mAudioPlayer->getBufferSize(), QVS_MSG_ENCODING);
	//		//mComposer->audio_push((unsigned char*)buffer, size, QVS_MSG_ENCODING);
	//		//printf("push 2 END\n");
	//	}
	//}
	//if (((StitchComposer*)mComposer)->isStreaming == QVS_MSG_STREAMING)
	//{
	//	if (mIsAudioHeaderRTMP == 0)
	//	{
	//		//printf("push 1 START\n");
	//		mComposer->audio_push(mAudioHeader, mAudioHeaderSize, QVS_MSG_STREAMING);
	//		mComposer->audio_push(mAudioPlayer->getBuffer(), mAudioPlayer->getBufferSize(), QVS_MSG_STREAMING);
	//		//printf("push 1 END\n");
	//		mIsAudioHeaderRTMP = 1;
	//	}
	//	else
	//	{
	//		//printf("push 2 START\n");
	//		mComposer->audio_push(mAudioPlayer->getBuffer(), mAudioPlayer->getBufferSize(), QVS_MSG_STREAMING);
	//		//printf("push 2 END\n");
	//	}
	//}

	return 1;
}

void AudioController::onPush(void *handle, void *data, unsigned char* buffer, int size, long long timeStamp) {
	AudioCapture *info = (AudioCapture *)handle;
	AudioController *audioController = (AudioController *)data;

	audioController->mPushBuffer.push(buffer);
	audioController->mPushBufferSize = size;

	//if (((StitchComposer*)audioController->mComposer)->isEncoding == QVS_MSG_ENCODING)
	//{
	//	if (audioController->mIsAudioHeaderHDD == 0)
	//	{
	//		//printf("push 1 START\n");
	//		audioController->mComposer->audio_push(audioController->mAudioHeader, audioController->mAudioHeaderSize, QVS_MSG_ENCODING);
	//		//mComposer->audio_push(audioCapture->getHeaderBuffer(), audioCapture->getHeaderBufferSize(), QVS_MSG_ENCODING);
	//		//audioController->mComposer->audio_push(audioController->mAudioPlayer->getBuffer(), audioController->mAudioPlayer->getBufferSize(), QVS_MSG_ENCODING);
	//		audioController->mComposer->audio_push((unsigned char*)buffer, size, QVS_MSG_ENCODING);
	//		//printf("push 1 END\n");
	//		audioController->mIsAudioHeaderHDD = 1;
	//	}
	//	else
	//	{
	//		//printf("push 2 START\n");
	//		//audioController->mComposer->audio_push(audioController->mAudioPlayer->getBuffer(), audioController->mAudioPlayer->getBufferSize(), QVS_MSG_ENCODING);
	//		audioController->mComposer->audio_push((unsigned char*)buffer, size, QVS_MSG_ENCODING);
	//		//printf("push 2 END\n");
	//	}
	//}
	//if (((StitchComposer*)audioController->mComposer)->isStreaming == QVS_MSG_STREAMING)
	//{
	//	if (audioController->mIsAudioHeaderRTMP == 0)
	//	{
	//		//printf("push 1 START\n");
	//		audioController->mComposer->audio_push(audioController->mAudioHeader, audioController->mAudioHeaderSize, QVS_MSG_STREAMING);
	//		audioController->mComposer->audio_push((unsigned char*)buffer, size, QVS_MSG_STREAMING);
	//		//printf("push 1 END\n");
	//		audioController->mIsAudioHeaderRTMP = 1;
	//	}
	//	else
	//	{
	//		//printf("push 2 START\n");
	//		audioController->mComposer->audio_push((unsigned char*)buffer, size, QVS_MSG_STREAMING);
	//		//printf("push 2 END\n");
	//	}
	//}
}


unsigned int __stdcall run_thread(void* param)
{

//}

//DWORD WINAPI AudioController::run(LPVOID handle) {
	AudioController *audioController = (AudioController *)param;

	while (1) {
		if (audioController->isClear()) {
			printf("AudioController Clear.\n");
			break;
		}
		if (audioController->mPushBuffer.empty() == false) {
			unsigned char* buffer = audioController->mPushBuffer.front();
			audioController->mPushBuffer.pop();

			int size = audioController->mPushBufferSize;

		}
	}

	return 0;
}

int AudioController::reset() {
	mIsNeedReset = 1;

	return 1;
}

int AudioController::setOffset(int offset) {
	return mAudioPlayer->SetBufferOffset(offset);
}

int AudioController::getOffset() {
	return mAudioPlayer->GetBufferOffset();
}
