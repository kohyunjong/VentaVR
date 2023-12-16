#include "EngineManager.h"
#include "QLicense.h"

#include <fstream>
#include <iostream>
#include <time.h>

using namespace std;

int EngineManager::Create(int numCamera, int inputWidth, int inputHeight, int outputWidth, int outputHeight, int mode)
{
	//oldTime = QClock();

#if (LICENSE_MACADDRESS != 0)
	QLicense m_pQLicense = QLicense();
	if (m_pQLicense.checkMacAddress(LICENSE_MACADDRESS) == QVS_FALSE)
		return QVS_FALSE;
#endif

	output_width = outputWidth;
	output_height = outputHeight;


	for (int i = 0; i < numCamera; i++) {
		texId[i] = 0;
	}

	roiTexId[0] = 0;

#ifdef SDI_OUTPUT_TEST
	outputBuffers = (void**)malloc(sizeof(void*)*NUM_OUTPUT_BUFFERS);
	for (int i = 0; i < NUM_OUTPUT_BUFFERS; i++) {
		outputBuffers[i] = malloc(inputWidth*inputHeight * 3);
	}
	outputIndex = 0;
#endif
	roi_buffer = (unsigned char*)malloc(outputWidth * outputHeight * 4);
	memset(roi_buffer, 0x00, outputWidth * outputHeight * 4);
	mROIMode = QVS_Engine::ROIMaskMode_Off;
	is_draw_ROI = 0;
	is_Redraw = 1;

	nadir_param.f = RECTLINEAR;
	nadir_param.out_f = EQUIRECTANGULAR;
	nadir_param.out_fov = 360;

	nadir_param.a = 0;
	nadir_param.b = 0;
	nadir_param.c = 0;

	nadir_param.y = 0;
	nadir_param.p = -90;
	nadir_param.r = 0;

	nadir_param.v = 50;
	nadir_param.w = 1024;
	nadir_param.h = 1024;

	nadir_param.top = 0;
	nadir_param.bottom = 1024;
	nadir_param.left = 0;
	nadir_param.right = 1024;

	mBlendingMode = MULTIBAND_BLEND;
	mSeamMode = FIND_SEAM_FROM_DIST;
	mGainMode = 0;
	mAutoGainMode = 0;//1;
	mNadirMode = INPUT_STATE_NO_DRAW;
	mNadirPosition = NADIR_BOTTOM;
	mNadirSize = 17;
	sprintf(mNadirPath, "%s", "./Nadir/nadir.png");
	mRGBIndex = 0;
	for (int i = 0; i < CAMERA_MAX; i++)
	{
		mRed[i] = 0;
		mGreen[i] = 0;
		mBlue[i] = 0;
	}

	mCaptureLensType = FULL_FRAME_FISHEYE;
	mCaptureFPS = 30.f;
	mNumCamera = numCamera;
	mNumVerticalLine = min(2, (mNumCamera - 1) % 3 + (mNumCamera - 1) / 3);
	mNumHorizontalLine = (mNumCamera - 1) / 3;
	mCaptureWidth = inputWidth;
	mCaptureHeight = inputHeight;
	mMaskCaptureWidth = inputWidth / 2;
	mMaskCaptureHeight = inputHeight / 2;
	mMaskUpdateRect = cv::Rect(0, 0, mMaskCaptureWidth, mMaskCaptureHeight);
	mCaptureFov = 120.0f;

	for (int i = 0; i < CAMERA_MAX; i++) {
		selectTexId[i] = 0;
		selectedTexId[i] = 0;
		mManual_RoiTexId[i] = 0;
		mRoi_Manual_buffer[i] = (unsigned char*)malloc(mMaskCaptureWidth * mMaskCaptureHeight * 4);
		memset(mRoi_Manual_buffer[i], 0x00, mMaskCaptureWidth * mMaskCaptureHeight * 4);
	}

	mManual_calib_mask_left_index = 0;
	mManual_calib_mask_right_index = 1;
	mRotateManualMask_Left_angle = 0;
	mRotateManualMask_Right_angle = 0;

	for (int i = 0; i < CAMERA_MAX; i++)
	{
		mCalibrationIndex[i] = 1;
		mRegionalWarpingIndex[i] = 0;
		mSaveMapIndex[i] = false;
		mRegionalWarpingMapIndex[i] = 0;
		mRegionalWarpingUndoMapIndex[i] = 0;
		mRegionalWarpingBeforeMapZeros[i] = 0;
	}
	mBMOutputDevice = 0;
	mBMOutputDisplay = 0;
	mGainChannelIndex = 0;
	mGainRGBIndex = 0;
	mGainPointIndex = 0;
	for (int i = 0; i < CAMERA_MAX; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (k == 0) {
					mGainPointX[i][j][k] = 0.0f;
					mGainPointY[i][j][k] = 0.0f;
				}
				else if (k == 1) {
					mGainPointX[i][j][k] = 127.0f;
					mGainPointY[i][j][k] = 127.0f;
				}
				else if (k == 2) {
					mGainPointX[i][j][k] = 255.0f;
					mGainPointY[i][j][k] = 255.0f;
				}
			}
		}
	}
	mGainCurveData = (void**)malloc(sizeof(void*)*mNumCamera);
	for (int i = 0; i < mNumCamera; i++)
	{
		mGainCurveData[i] = NULL;
	}
	mOutputCurveData = (void*)malloc(sizeof(void*));
	mOutputCurveData = NULL;

	mAudioDevice = -1;
	mPanoramaSize = 0;
	mIsMultiStreaming = 0;

	input_manager = new InputManager(numCamera, NUM_CHANNEL, NUM_BUFFERS);
	if (input_manager == NULL)
		return QVS_MSG_INVALID_INSTANCE_ERROR;

	//calibrator = new CalibratorMulti(numCamera, NUM_BUFFERS, mCaptureWidth, mCaptureHeight, outputWidth, outputHeight, 3);
	//if (calibrator == NULL)
	//	return QVS_MSG_INVALID_INSTANCE_ERROR;

	input_tempData = new InputTempData(numCamera, NUM_BUFFERS, mCaptureWidth, mCaptureHeight, 3);
	if (input_tempData == NULL)
		return QVS_MSG_INVALID_INSTANCE_ERROR;

	/*int __ret = getVersion();
	printf("QCalibration Version  ::> %d\n", __ret);
	__ret = CreateCalibrator(numCamera, NUM_BUFFERS, mCaptureWidth, mCaptureHeight, outputWidth, outputHeight, 3);
	if (!__ret) return QVS_MSG_INVALID_INSTANCE_ERROR;*/

	stitcher = new Stitcher();
	if (stitcher == NULL)
		return QVS_MSG_INVALID_INSTANCE_ERROR;


	stitcher->create(outputWidth, outputHeight, mNumCamera, inputWidth, inputHeight);
	
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();
	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();

	stitchingMode = STEREO_STITCHING_LR;
	((StitchComposer*)stitcher)->setStereoMode(STEREO_STITCHING_LR);
	((StitchComposer*)stitcher)->stitchingMode = STEREO_STITCHING_LR;
	mOutputType = STEREO;
	mOutputFov = 180;
	nadir_param.out_fov = 180;

	global_param->out_f = mOutputType;
	global_param->out_fov = mOutputFov;

	for (int i = 0; i < mNumCamera; i++)
	{
		image_param[i].f = global_param->f;
		image_param[i].out_f = global_param->out_f;
		image_param[i].out_fov = mOutputFov;
	}


	sprintf(snapshot_path, ".\\");
	//PTS_path[0] = NULL;

	mUseViewSize = 1;

	is_Init = 0;
	is_load = 0;
	is_VideoCaptureInit = 0;
	is_GenTextures = 0;
	//is_buffering = 1;
	is_thread_seam = 0;
	is_request_change_param = 0;
	is_request_change_fps = 0;
	is_request_change_status = 0;

	is_audio_header_hdd = 0;
	is_audio_header_rtmp = 0;

	is_AudioPlayerInit = 0;

	//mAudioPlayer = NULL;

	mAudioController = AudioController::create_AudioController(stitcher->composer);

	temp_videoCapture_state = (int**)malloc(CAMERA_MAX * sizeof(int*));

	for (int i = 0; i < CAMERA_MAX; i++)
	{
		mVideoCaptureList[i].mVideoCapture = NULL;
		mVideoCaptureList[i].mVideoCapture_CPU = NULL;

		mVideoCaptureList[i].mCaptureDeviceIndex = -1;

		temp_videoCapture_state[i] = &mVideoCaptureList[i].mCaptureDeviceIndex;
		printf("TEST, temp_videoCapture_state[i] %d, *temp_videoCapture_state[i] %d \n", temp_videoCapture_state[i], *temp_videoCapture_state[i]);
		sprintf(mInputImagePath[i], "");

		// Edit Input Animation
		mSourceTextures[i] = NULL;

		mVideoCaptureList[i].mAspectX = -1;
		mVideoCaptureList[i].mAspectY = -1;
	}
	audio_capture = NULL;

	mNumOtherInput = 0;

	mSelectSize = 100;
	printf("mSelectSize : %d\n", mSelectSize);
	for (int i = 0; i < 8; i++)
	{
		char path[MAX_PATH];
		if (select_buffer[i] == NULL)
		{
			select_buffer[i] = (unsigned char*)malloc(mSelectSize * mSelectSize * 4);

			sprintf(path, "./Resources/select_%d.png", i);
			cv::Mat select_img = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);
			//printf("bigheadk, ch1 = %d", select_img.channels());
			cv::resize(select_img, select_img, cv::Size(mSelectSize, mSelectSize));

			if (select_img.rows != 0) {
				cv::cvtColor(select_img, select_img, CV_BGRA2RGBA);
				memcpy(select_buffer[i], select_img.data, mSelectSize * mSelectSize * 4);
			}

			select_img.release();
		}

		if (selected_buffer[i] == NULL)
		{
			selected_buffer[i] = (unsigned char*)malloc(mSelectSize * mSelectSize * 4);

			sprintf(path, "./Resources/selected_%d.png", i);
			cv::Mat selected_img = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);
			//printf("bigheadk, ch2 = %d", select_img.channels());
			cv::resize(selected_img, selected_img, cv::Size(mSelectSize, mSelectSize));

			if (selected_img.rows != 0) {
				cv::cvtColor(selected_img, selected_img, CV_BGRA2RGBA);
				memcpy(selected_buffer[i], selected_img.data, mSelectSize * mSelectSize * 4);
			}

			selected_img.release();
		}
	}
	mSelectMode = QVS_Engine::InputSelectMode_Off;
	mCalibrationMode = QVS_Engine::CalibrationMode_MakeTemplate;
	mRegionalWarpingViewMode = QVS_Engine::RegionalWarpingViewMode_Off;

	is_need_save = false;
	is_editmode = false;
	calculateInteractiveCoordinates();
	calculateInteractiveCoordinatesForWidePanorama();

	mCaptureDoneCounter = 0;
	mIsSeamThread = false;
	mIsGainThread = false;
	mIsCalibrationThread = false;
	mlsStereoCalibrationThread = false;
	//Overlay init

	// Edit Input Animation
	is_SourceViewInit = 0;
	mEditInputMode = QVS_Engine::EditInputMode_Off;
	mEditInputTexture = NULL;

	//mCurOverlayIndex = -1;

	setPanoramaSize(mPanoramaSize);

	stitcher->setCallback_setOutputLog(onOutputLog, this);

	printf("\n##### Quram 360 Engine Create Done #####\n\n");
	return QVS_SUCCESS;
}

int EngineManager::Destroy()
{
	//mOverlayTemplateList
	//Overlay destroy
	checkWarpingMap();

	TerminateThread(mSeamThread, 0);
	TerminateThread(mGainThread, 0);
	TerminateThread(mCalibrationThread, 0);
	//	TerminateThread(mWarpingMapThread, 0);
	TerminateThread(mEditOutputThread, 0);
	TerminateThread(mManualCurveThread, 0);
	while (1){
		DWORD dwExitCode = NULL;
		GetExitCodeThread(mSeamThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			break;
		Sleep(WAIT_WARP_TIME);

		printf("bigheadk, wait 0 \n");
	}

	while (1){
		DWORD dwExitCode = NULL;
		GetExitCodeThread(mGainThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			break;
		Sleep(WAIT_WARP_TIME);

		printf("bigheadk, wait 1 \n");
	}

	while (1){
		DWORD dwExitCode = NULL;
		GetExitCodeThread(mCalibrationThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			break;
		Sleep(WAIT_WARP_TIME);

		printf("bigheadk, wait 2 \n");
	}

	while (1){
		DWORD dwExitCode = NULL;
		GetExitCodeThread(mEditOutputThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			break;
		Sleep(WAIT_WARP_TIME);

		printf("bigheadk, wait 4 \n");
	}

	while (1){
		DWORD dwExitCode = NULL;
		GetExitCodeThread(mManualCurveThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			break;
		Sleep(WAIT_WARP_TIME);

		printf("bigheadk, wait 5 \n");
	}


	for (int i = 0; i < CAMERA_MAX; i++)
	{
		destroyVideoCapture(i);
	}


	destroyAudioDevice();
	input_manager->destroy();
	Sleep(30);
	printf("bigheadk, delete 1\n");
	if (stitcher)
		stitcher->destroy();
	delete stitcher;
	stitcher = NULL;
	printf("bigheadk, delete 2\n");
	Sleep(30);
	destroyInteractiveCoordinates();
	printf("bigheadk, delete 3\n");

	AudioController::destroy_AudioController(mAudioController);

	DeleteTextures();

	printf("bigheadk, delete 5\n");
	return QVS_SUCCESS;
}

int EngineManager::Init()
{
	return 1;
}

int EngineManager::GenTextures()
{
	glGenTextures(mNumCamera, texId);

	for (int i = 0; i < mNumCamera; i++) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, texId[i]);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGB,
			mCaptureWidth, mCaptureHeight, 0,
			GL_RGB, GL_UNSIGNED_BYTE, 
			0
			);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	glGenTextures(1, roiTexId);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, roiTexId[0]);

	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGBA,
		output_width, output_height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE,
		0
		);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glGenTextures(CAMERA_MAX, mManual_RoiTexId);

	for (int i = 0; i < CAMERA_MAX; i++) {
		//glEnable(GL_BLEND);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, mManual_RoiTexId[i]);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			mMaskCaptureWidth, mMaskCaptureHeight, 0,
			GL_RGBA, GL_UNSIGNED_BYTE,
			0

			);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	glGenTextures(CAMERA_MAX, selectTexId);

	for (int i = 0; i < CAMERA_MAX; i++) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, selectTexId[i]);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_RGBA,
			mSelectSize, mSelectSize, 0,
			GL_RGBA, GL_UNSIGNED_BYTE,
			0
			);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	glGenTextures(CAMERA_MAX, maskTexId);

	for (int i = 0; i < CAMERA_MAX; i++) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_TEXTURE_2D);

		glBindTexture(GL_TEXTURE_2D, maskTexId[i]);

		glTexImage2D(
			GL_TEXTURE_2D, 0, GL_ALPHA,
			output_width / 8, output_height / 8, 0,
			GL_ALPHA, GL_UNSIGNED_BYTE,
			0
			);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	}

	return 1;
}

int EngineManager::DeleteTextures()
{
	glDeleteTextures(mNumCamera, texId);
	glDeleteTextures(1, roiTexId);
	glDeleteTextures(CAMERA_MAX, mManual_RoiTexId);
	glDeleteTextures(CAMERA_MAX, selectTexId);
	glDeleteTextures(CAMERA_MAX, maskTexId);
	return 1;
}

BOOL CALLBACK EnumCallback(LPGUID guid, char* descr, char* modname, LPVOID ctx)
{
	std::vector<char*> *names = (std::vector<char*>*)ctx;
	names->push_back(descr);
	return TRUE;
}

int EngineManager::AudioCaptureUpdate()
{
	for (int i = mAudioCaptureDeviceList.size() - 1; i > 0; i--)
	{
		if (strstr(mAudioCaptureDeviceList[i].mCaptureDeviceName, "High Definition Audio") ||
			strstr(mAudioCaptureDeviceList[i].mCaptureDeviceName, "Microphone") ||
			strstr(mAudioCaptureDeviceList[i].mCaptureDeviceName, "Realtek"))
		{
			mAudioCaptureDeviceList.pop_back();
		}
	}

	std::vector<char*> names;
	if (!FAILED(DirectSoundCaptureEnumerateA((LPDSENUMCALLBACKA)EnumCallback, &names)))
	{
		// do stuff
	}

	for (int i = 0; i < names.size(); i++)
	{
		if ((strstr(names[i], "High Definition Audio") && !strstr(names[i], "PDI")) ||
			(strstr(names[i], "Microphone")) ||
			(strstr(names[i], "Realtek"))) {
			char *temp;
			temp = (char*)malloc(MAX_PATH);
			//sprintf(temp, "%s(%d)", videoInfo.szBoardSerialNo, videoInfo.byChannelIndex);
			sprintf(temp, "%s", names[i]);

			CaptureDevice captureDevice;
			captureDevice.mCaptureDeviceName = temp;
			captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_MIC;

			mAudioCaptureDeviceList.push_back(captureDevice);

			break;
		}
	}

	return 1;
}

int EngineManager::AudioCaptureInit()
{
	mAudioCaptureDeviceList.clear();

	int device_index = 0;

	int mw_device_index = 0;
	while (true)
	{
		MWCAP_CHANNEL_INFO videoInfo = { 0 };
		TCHAR szDevicePath[MAX_PATH];
		if (MW_SUCCEEDED != MWGetDevicePath(mw_device_index, szDevicePath))
		{
			//printf("Engine ERROR: Can't get Device Path!\n");
			break;
		}

		HCHANNEL hchannel = MWOpenChannelByPath(szDevicePath);
		if (hchannel == NULL)
		{
			//printf("Engine ERROR: Open channel %s:%d error!\n", szDevicePath, mw_device_index);
			break;
		}

		if (MW_SUCCEEDED != MWGetChannelInfo(hchannel, &videoInfo))
		{
			//printf("Engine ERROR: Can't get channel info!\n");
			break;
		}

		char *temp;
		temp = (char*)malloc(MAX_PATH);
		//sprintf(temp, "%s(%d)", videoInfo.szBoardSerialNo, videoInfo.byChannelIndex);
		sprintf(temp, "%s (%d) - %d", videoInfo.szProductName, videoInfo.byChannelIndex, device_index);
		//sprintf(temp, "Magewell (%d) - %d", videoInfo.byChannelIndex, device_index);

		CaptureDevice captureDevice;
		captureDevice.mCaptureDeviceName = temp;
		captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Magewell;
		captureDevice.mDeviceNumber = mw_device_index;

		mAudioCaptureDeviceList.push_back(captureDevice);

		mw_device_index++;
		device_index++;
	}

	int xi_device_index = 0;
	while (true)
	{
		int cVideoCaptures = XIS_GetVideoCaptureCount();
		if (cVideoCaptures <= 0) {
			printf("ERROR: Can't find capture card!\n");
			break;
		}

		VIDEO_CAPTURE_INFO vidCapInfo;
		int iUSBDevice = 0;
		bool isFind = false;
		for (int i = 0; i < cVideoCaptures; i++) {
			if (!XIS_GetVideoCaptureInfo(i, &vidCapInfo)) {
				//VIDEO_CAPTURE_INFO_EX vidCapInfo;
				//if (!XIS_GetVideoCaptureInfoEx(mCameraIndex, &vidCapInfo)) {
				printf("ERROR: Can't get channel info!\n");
				break;
			}

			if (_tcsstr(vidCapInfo.szDShowID, _T("usb")) || _tcsstr(vidCapInfo.szName, _T("USB"))) {
				if (iUSBDevice == xi_device_index) {
					isFind = true;
					break;
				}
				else
					iUSBDevice++;
			}
		}
		if (isFind == false) {
			printf("ERROR: Can't find USB device!\n");
			break;
		}

		HANDLE hChannel = XIS_OpenVideoCapture(vidCapInfo.szDShowID);
		if (NULL == hChannel) {
			printf("ERROR: Open channel %s:%d error!\n", vidCapInfo.szDShowID, xi_device_index);
			break;
		}

		char *temp;
		temp = (char*)malloc(MAX_PATH);
		//sprintf(mChannelID, "%s(%d)", vidCapInfo.szName, mCameraIndex);
		sprintf(temp, "USB - %d", device_index);

		CaptureDevice captureDevice;
		captureDevice.mCaptureDeviceName = temp;
		captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Magewell_USB;
		captureDevice.mDeviceNumber = xi_device_index;

		mAudioCaptureDeviceList.push_back(captureDevice);

		xi_device_index++;
		device_index++;
	}

	int bm_device_index = 0;
	while (true)
	{
		char displayName[MAX_PATH] = "";
		char modelName[MAX_PATH] = "";

		if (getDeviceList(bm_device_index + 1, displayName, modelName) == false) {
			if (strcmp(displayName, "") == 0)
				break;

			bm_device_index++;
			continue;
		}

		char *temp;
		temp = (char*)malloc(MAX_PATH);
		sprintf(temp, "%s - %d", displayName, device_index);

		CaptureDevice captureDevice;
		captureDevice.mCaptureDeviceName = temp;
		captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Blackmagic;
		captureDevice.mDeviceNumber = bm_device_index;

		mAudioCaptureDeviceList.push_back(captureDevice);

		bm_device_index++;
		device_index++;
	}

	int mic_device_index = 0;
	std::vector<char*> names;
	if (!FAILED(DirectSoundCaptureEnumerateA((LPDSENUMCALLBACKA)EnumCallback, &names)))
	{
		// do stuff
	}

	for (int i = 0; i < names.size(); i++)
	{
		if ((strstr(names[i], "High Definition Audio") && !strstr(names[i], "PDI")) ||
			(strstr(names[i], "Microphone")) ||
			(strstr(names[i], "Realtek"))) {
			char *temp;
			temp = (char*)malloc(MAX_PATH);
			//sprintf(temp, "%s(%d)", videoInfo.szBoardSerialNo, videoInfo.byChannelIndex);
			sprintf(temp, "%s", names[i]);

			CaptureDevice captureDevice;
			captureDevice.mCaptureDeviceName = temp;
			captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_MIC;

			mAudioCaptureDeviceList.push_back(captureDevice);

			mic_device_index++;
			device_index++;

			break;
		}
	}

	return 1;
}

int EngineManager::VideoCaptureInit()
{
	mCaptureDeviceList.clear();

	is_VideoCaptureInit = 1;

	int device_index = 0;

	if (!MWCaptureInitInstance())
	{
		printf("Engine ERROR: MWCaptureInitInstance return fail!\n");
	}
	else
	{
		int mw_device_index = 0;
		while (true)
		{
			MWCAP_CHANNEL_INFO videoInfo = { 0 };
			TCHAR szDevicePath[MAX_PATH];
			if (MW_SUCCEEDED != MWGetDevicePath(mw_device_index, szDevicePath))
			{
				//printf("Engine ERROR: Can't get Device Path!\n");
				break;
			}

			HCHANNEL hchannel = MWOpenChannelByPath(szDevicePath);
			if (hchannel == NULL)
			{
				//printf("Engine ERROR: Open channel %s:%d error!\n", szDevicePath, mw_device_index);
				break;
			}

			if (MW_SUCCEEDED != MWGetChannelInfo(hchannel, &videoInfo))
			{
				//printf("Engine ERROR: Can't get channel info!\n");
				break;
			}

			char *temp;
			temp = (char*)malloc(MAX_PATH);
			//sprintf(temp, "%s(%d)", videoInfo.szBoardSerialNo, videoInfo.byChannelIndex);
			sprintf(temp, "%s (%d) - %d", videoInfo.szProductName, videoInfo.byChannelIndex, device_index);
			//sprintf(temp, "Magewell (%d) - %d", videoInfo.byChannelIndex, device_index);

			CaptureDevice captureDevice;
			captureDevice.mCaptureDeviceName = temp;
			captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Magewell;
			captureDevice.mDeviceNumber = mw_device_index;

			mCaptureDeviceList.push_back(captureDevice);

			mw_device_index++;
			device_index++;
		}
	}

	if (!XIS_Initialize()) {
		printf("ERROR: XIS_Initialize return fail!\n");
	}
	else
	{
		int xi_device_index = 0;
		while (true)
		{
			int cVideoCaptures = XIS_GetVideoCaptureCount();
			if (cVideoCaptures <= 0) {
				printf("ERROR: Can't find capture card!\n");
				break;
			}

			VIDEO_CAPTURE_INFO vidCapInfo;
			int iUSBDevice = 0;
			bool isFind = false;
			for (int i = 0; i < cVideoCaptures; i++) {
				if (!XIS_GetVideoCaptureInfo(i, &vidCapInfo)) {
					//VIDEO_CAPTURE_INFO_EX vidCapInfo;
					//if (!XIS_GetVideoCaptureInfoEx(mCameraIndex, &vidCapInfo)) {
					printf("ERROR: Can't get channel info!\n");
					break;
				}

				if (_tcsstr(vidCapInfo.szDShowID, _T("usb")) || _tcsstr(vidCapInfo.szName, _T("USB"))) {
					if (iUSBDevice == xi_device_index) {
						isFind = true;
						break;
					}
					else
						iUSBDevice++;
				}
			}
			if (isFind == false) {
				printf("ERROR: Can't find USB device!\n");
				break;
			}

			HANDLE hChannel = XIS_OpenVideoCapture(vidCapInfo.szDShowID);
			if (NULL == hChannel) {
				printf("ERROR: Open channel %s:%d error!\n", vidCapInfo.szDShowID, xi_device_index);
				break;
			}

			char *temp;
			temp = (char*)malloc(MAX_PATH);
			//sprintf(mChannelID, "%s(%d)", vidCapInfo.szName, mCameraIndex);
			sprintf(temp, "USB - %d", device_index);

			CaptureDevice captureDevice;
			captureDevice.mCaptureDeviceName = temp;
			captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Magewell_USB;
			captureDevice.mDeviceNumber = xi_device_index;

			mCaptureDeviceList.push_back(captureDevice);

			xi_device_index++;
			device_index++;
		}
	}

	int bm_device_index = 0;
	while (true)
	{
		char displayName[MAX_PATH] = "";
		char modelName[MAX_PATH] = "";

		if (getDeviceList(bm_device_index + 1, displayName, modelName) == false) {
			if (strcmp(displayName, "") == 0)
				break;

			bm_device_index++;
			continue;
		}

		char *temp;
		temp = (char*)malloc(MAX_PATH);
		sprintf(temp, "%s - %d", displayName, device_index);

		CaptureDevice captureDevice;
		captureDevice.mCaptureDeviceName = temp;
		captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Blackmagic;
		captureDevice.mDeviceNumber = bm_device_index;

		mCaptureDeviceList.push_back(captureDevice);

		bm_device_index++;
		device_index++;
	}

	char *temp;
	temp = (char*)malloc(MAX_PATH);
	sprintf(temp, "Snapshot");

	CaptureDevice captureDevice;
	captureDevice.mCaptureDeviceName = temp;
	captureDevice.mCaptureDeviceType = QVS_Engine::InputModuleType_Image;

	mCaptureDeviceList.push_back(captureDevice);

	device_index++;

	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mCaptureDeviceIndex != -1)
		{
			createVideoCapture(i, mVideoCaptureList[i].mCaptureDeviceIndex, mCaptureDeviceList[mVideoCaptureList[i].mCaptureDeviceIndex].mCaptureDeviceType);
		}
	}

	return 1;
}

int EngineManager::createVideoCapture(int cameraIndex, int deviceIndex, int cameraType)
{
	if (cameraType == QVS_Engine::InputModuleType_Magewell)
	{
		int offset = 0;
		for (int i = 0; i < mCaptureDeviceList.size(); i++)
		{
			if (mCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
				break;
			else
				offset++;
		}

		mVideoCaptureList[cameraIndex].mVideoCapture = MWVideoCapture::create_VideoCapture(cameraIndex, deviceIndex - offset, 0, -1, mCaptureWidth, mCaptureHeight, mCaptureFPS, CAPTURE_COLORSPACE);
		if (mVideoCaptureList[cameraIndex].mVideoCapture == NULL)
			return 0;

		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_getAddress(getAddress, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onCaptureDone(onCaptureDone, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onSnapShotDone(onSnapShotDone, this);

		mVideoCaptureList[cameraIndex].mVideoCapture_CPU = MWVideoCapture::create_VideoCapture(cameraIndex, deviceIndex - offset, 1, -1, mCaptureWidth, mCaptureHeight, mCaptureFPS, CAPTURE_COLORSPACE);
		if (mVideoCaptureList[cameraIndex].mVideoCapture_CPU == NULL)
			return 0;

		mVideoCaptureList[cameraIndex].mVideoCapture_CPU->setCallback_getAddress(getAddress, this);
		mVideoCaptureList[cameraIndex].mVideoCapture_CPU->setCallback_onCaptureDone(onCaptureDone, this);
		mVideoCaptureList[cameraIndex].mVideoCapture_CPU->setCallback_onSnapShotDone(onSnapShotDone, this);

		mVideoCaptureList[cameraIndex].mVideoCapture->setCaptureType(QVS_Engine::InputModuleType_Magewell);

		mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = deviceIndex;

		if (mVideoCaptureList[cameraIndex].mAspectX < 0)
			mVideoCaptureList[cameraIndex].mAspectX = 16;
		if (mVideoCaptureList[cameraIndex].mAspectY < 0)
			mVideoCaptureList[cameraIndex].mAspectY = 9;

		setAspectX(cameraIndex, mVideoCaptureList[cameraIndex].mAspectX);
		setAspectY(cameraIndex, mVideoCaptureList[cameraIndex].mAspectY);
	}
	else if (cameraType == QVS_Engine::InputModuleType_Blackmagic)
	{
		//int offset = 0;
		//for (int i = 0; i < mCaptureDeviceList.size(); i++)
		//{
		//	if (mCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_Blackmagic)
		//		break;
		//	else
		//		offset++;
		//}

		mVideoCaptureList[cameraIndex].mVideoCapture = BMVideoCapture::create_VideoCapture(cameraIndex, mCaptureDeviceList[deviceIndex].mDeviceNumber/*deviceIndex - offset*/, 0, mCaptureWidth, mCaptureHeight, mCaptureFPS);
		if (mVideoCaptureList[cameraIndex].mVideoCapture == NULL)
			return 0;

		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_getAddress(getAddress, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onCaptureDone(onCaptureDone, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onSnapShotDone(onSnapShotDone, this);

		mVideoCaptureList[cameraIndex].mVideoCapture->setCaptureType(QVS_Engine::InputModuleType_Blackmagic);

		mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = deviceIndex;
	}
	else if (cameraType == QVS_Engine::InputModuleType_Magewell_USB)
	{
		//int offset = 0;
		//for (int i = 0; i < mCaptureDeviceList.size(); i++)
		//{
		//	if (mCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell_USB)
		//		break;
		//	else
		//		offset++;
		//}

		mVideoCaptureList[cameraIndex].mVideoCapture = XIVideoCapture::create_VideoCapture(cameraIndex, mCaptureDeviceList[deviceIndex].mDeviceNumber/*deviceIndex - offset*/, 0, mCaptureWidth, mCaptureHeight, mCaptureFPS, XI_COLOR_YUYV);
		if (mVideoCaptureList[cameraIndex].mVideoCapture == NULL)
			return 0;

		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_getAddress(getAddress, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onCaptureDone(onCaptureDone, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onSnapShotDone(onSnapShotDone, this);

		mVideoCaptureList[cameraIndex].mVideoCapture->setCaptureType(QVS_Engine::InputModuleType_Magewell_USB);

		mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = deviceIndex;
	}
	else if (cameraType == QVS_Engine::InputModuleType_Image)
	{
		mVideoCaptureList[cameraIndex].mVideoCapture = IMGVideoCapture::create_VideoCapture(cameraIndex, 0, mCaptureWidth, mCaptureHeight, mCaptureFPS, mInputImagePath[cameraIndex]);
		if (mVideoCaptureList[cameraIndex].mVideoCapture == NULL)
			return 0;

		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_getAddress(getAddress, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onCaptureDone(onCaptureDone, this);
		mVideoCaptureList[cameraIndex].mVideoCapture->setCallback_onSnapShotDone(onSnapShotDone, this);

		mVideoCaptureList[cameraIndex].mVideoCapture->setCaptureType(QVS_Engine::InputModuleType_Image);

		mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = deviceIndex;
	}

	return 1;
}

int EngineManager::destroyVideoCapture(int cameraIndex)
{
	if (mVideoCaptureList[cameraIndex].mVideoCapture)
	{
		if (mVideoCaptureList[cameraIndex].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
		{
			MWVideoCapture::destroy_VideoCapture((MWVideoCapture*)mVideoCaptureList[cameraIndex].mVideoCapture);
			mVideoCaptureList[cameraIndex].mVideoCapture = NULL;
			MWVideoCapture::destroy_VideoCapture((MWVideoCapture*)mVideoCaptureList[cameraIndex].mVideoCapture_CPU);
			mVideoCaptureList[cameraIndex].mVideoCapture_CPU = NULL;

			mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = -1;
		}
		else if (mVideoCaptureList[cameraIndex].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Blackmagic)
		{
			BMVideoCapture::destroy_VideoCapture((BMVideoCapture*)mVideoCaptureList[cameraIndex].mVideoCapture);
			mVideoCaptureList[cameraIndex].mVideoCapture = NULL;

			mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = -1;
		}
		else if (mVideoCaptureList[cameraIndex].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell_USB)
		{
			XIVideoCapture::destroy_VideoCapture((XIVideoCapture*)mVideoCaptureList[cameraIndex].mVideoCapture);
			mVideoCaptureList[cameraIndex].mVideoCapture = NULL;

			mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = -1;
		}
		else if (mVideoCaptureList[cameraIndex].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Image)
		{
			IMGVideoCapture::destroy_VideoCapture((IMGVideoCapture*)mVideoCaptureList[cameraIndex].mVideoCapture);
			mVideoCaptureList[cameraIndex].mVideoCapture = NULL;

			mVideoCaptureList[cameraIndex].mCaptureDeviceIndex = -1;
		}
	}

	return 1;
}

int EngineManager::swapVideoCapture(int cameraIndex1, int cameraIndex2)
{
	VideoCapture* tempVideoCapture = mVideoCaptureList[cameraIndex1].mVideoCapture;
	VideoCapture* tempVideoCapture_CPU = mVideoCaptureList[cameraIndex1].mVideoCapture_CPU;
	int tempValue = mVideoCaptureList[cameraIndex1].mCaptureDeviceIndex;
	int tempAspectX = mVideoCaptureList[cameraIndex1].mAspectX;
	int tempAspectY = mVideoCaptureList[cameraIndex1].mAspectY;

	mVideoCaptureList[cameraIndex2].mVideoCapture->requestChangeCameraIndex(cameraIndex1, true);
	mVideoCaptureList[cameraIndex1].mVideoCapture = mVideoCaptureList[cameraIndex2].mVideoCapture;
	if (mCaptureDeviceList[mVideoCaptureList[cameraIndex2].mCaptureDeviceIndex].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
	{
		mVideoCaptureList[cameraIndex2].mVideoCapture_CPU->requestChangeCameraIndex(cameraIndex1, true);
	}
	mVideoCaptureList[cameraIndex1].mVideoCapture_CPU = mVideoCaptureList[cameraIndex2].mVideoCapture_CPU;

	mVideoCaptureList[cameraIndex1].mCaptureDeviceIndex = mVideoCaptureList[cameraIndex2].mCaptureDeviceIndex;
	mVideoCaptureList[cameraIndex1].mAspectX = mVideoCaptureList[cameraIndex2].mAspectX;
	mVideoCaptureList[cameraIndex1].mAspectY = mVideoCaptureList[cameraIndex2].mAspectY;

	if (tempValue != -1)
	{
		tempVideoCapture->requestChangeCameraIndex(cameraIndex2, true);
		mVideoCaptureList[cameraIndex2].mVideoCapture = tempVideoCapture;
		if (mCaptureDeviceList[tempValue].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
		{
			tempVideoCapture_CPU->requestChangeCameraIndex(cameraIndex2, true);
		}
		mVideoCaptureList[cameraIndex2].mVideoCapture_CPU = tempVideoCapture_CPU;


		mVideoCaptureList[cameraIndex2].mCaptureDeviceIndex = tempValue;
		mVideoCaptureList[cameraIndex2].mAspectX = tempAspectX;
		mVideoCaptureList[cameraIndex2].mAspectY = tempAspectY;
	}
	else
	{
		mVideoCaptureList[cameraIndex2].mVideoCapture = tempVideoCapture;
		mVideoCaptureList[cameraIndex2].mVideoCapture_CPU = tempVideoCapture_CPU;

		mVideoCaptureList[cameraIndex2].mCaptureDeviceIndex = tempValue;
		mVideoCaptureList[cameraIndex2].mAspectX = tempAspectX;
		mVideoCaptureList[cameraIndex2].mAspectY = tempAspectY;
	}
	printf("bigheadk, swapVideoCapture!\n");
	is_need_save = true;

	return 1;
}

char* EngineManager::getAudioCaptureDeviceName(int index)
{
	if (index < mAudioCaptureDeviceList.size() && index >= 0)
	{
		return mAudioCaptureDeviceList[index].mCaptureDeviceName;
	}
	else
	{
		return NULL;
	}
}

int EngineManager::getAudioCaptureDeviceSize()
{
	return mAudioCaptureDeviceList.size();
}

char* EngineManager::getCaptureDeviceName(int index)
{
	if (index < mCaptureDeviceList.size() && index >= 0)
	{
		return mCaptureDeviceList[index].mCaptureDeviceName;
	}
	else
	{
		return NULL;
	}
}

int EngineManager::getCaptureDeviceSize()
{
	return mCaptureDeviceList.size();
}

int EngineManager::setCaptureDeviceIndex(int index, int value)
{
	int preIndex = -1;
	if (mCaptureDeviceList[value].mCaptureDeviceType != QVS_Engine::InputModuleType_Image)
	{
		for (int i = 0; i < CAMERA_MAX; i++)
		{
			if (mVideoCaptureList[i].mCaptureDeviceIndex == value)
			{
				preIndex = i;
				break;
			}
		}
	}

	if (preIndex != -1)
	{
		if (mCaptureDeviceList[mVideoCaptureList[preIndex].mCaptureDeviceIndex].mCaptureDeviceType == QVS_Engine::InputModuleType_Image || mCaptureDeviceList[mVideoCaptureList[index].mCaptureDeviceIndex].mCaptureDeviceType == QVS_Engine::InputModuleType_Image)
		{
			int aspectX = mVideoCaptureList[preIndex].mAspectX, aspectY = mVideoCaptureList[preIndex].mAspectY;
			if (mVideoCaptureList[index].mVideoCapture != NULL)
			{
				destroyVideoCapture(index);
				mVideoCaptureList[index].mAspectX = -1;
				mVideoCaptureList[index].mAspectY = -1;
			}
			if (mVideoCaptureList[preIndex].mVideoCapture != NULL)
			{
				destroyVideoCapture(preIndex);
				mVideoCaptureList[preIndex].mAspectX = -1;
				mVideoCaptureList[preIndex].mAspectY = -1;
			}
			mVideoCaptureList[index].mAspectX = aspectX;
			mVideoCaptureList[index].mAspectY = aspectY;
			createVideoCapture(index, value, mCaptureDeviceList[value].mCaptureDeviceType);
		}
		else
		{
			swapVideoCapture(index, preIndex);

			setAspectX(preIndex, mVideoCaptureList[preIndex].mAspectX);
			setAspectY(preIndex, mVideoCaptureList[preIndex].mAspectY);

			setAspectX(index, mVideoCaptureList[index].mAspectX);
			setAspectY(index, mVideoCaptureList[index].mAspectY);
		}
	}
	else
	{
		int aspectX = mVideoCaptureList[index].mAspectX, aspectY = mVideoCaptureList[index].mAspectY;
		if (mVideoCaptureList[index].mVideoCapture != NULL)
		{
			destroyVideoCapture(index);
			mVideoCaptureList[index].mAspectX = -1;
			mVideoCaptureList[index].mAspectY = -1;
		}
		mVideoCaptureList[index].mAspectX = aspectX;
		mVideoCaptureList[index].mAspectY = aspectY;
		createVideoCapture(index, value, mCaptureDeviceList[value].mCaptureDeviceType);
	}

	is_need_save = true;
	return 1;
}

int EngineManager::getCaptureDeviceIndex(int index)
{
	return mVideoCaptureList[index].mCaptureDeviceIndex;
}

int EngineManager::setInputImagePath(int index, char* path)
{
	//wchar_t strUnicode[256] = { 0, };
	//char strMultiByte[256] = { 0, };
	//MultiByteToWideChar(CP_UTF8, 0, path, strlen(path), strUnicode, 256);
	//WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

	sprintf(mInputImagePath[index], path);
	//sprintf(mInputImagePath[index], strMultiByte);
	is_need_save = true;
	return 1;
}

char* EngineManager::getInputImagePath(int index)
{
	return mInputImagePath[index];
}

int EngineManager::Stitching(int w, int h, int panorama_preview_mode)
{
	//check fps
	double req_time = (double)1000 / renderFPS;

	if (checkTimer == 0.0)
	{
		checkTimer = QClock();
	}

	while (req_time > abs(QClock() - checkTimer))
	{

	}
	checkTimer = QClock() + (abs(QClock() - checkTimer) - req_time);
	//printf("chk timer QClock() %lf, checkTimer = %lf\n", QClock(), checkTimer);

	int editMode = STITCHING_MODE;
	bool isStereoMode = false;
	

	if (is_Init == 0 && is_load != 0)
	{
#ifdef USE_XML
		stitcher->init();
#else
		stitcher->init(nadir_param, mNadirPath, mNumCamera, mCaptureWidth, mCaptureHeight, stitchingMode);
#endif
		is_request_change_status = 1;
		is_request_change_param = STITCHING_INIT;
		is_Init = 1;
	}
	else if (input_manager->isFinishBuffering())
	{
		int* idx = input_manager->getOutIndex(0);
		if (idx == 0)
			return QVS_FAIL;

		if (is_doing_exposure || is_doing_seam)
		{
		}
		else if (is_request_change_param == STITCHING_INIT)
		{
			is_request_change_param = 0;
			stitcher->updateStitchingTemplate();

			mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);
			if (!mAutoGainMode)
				mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);

			mWarpingMapThread = CreateThread(NULL, 0, run_warpMap, this, 0, NULL);

			SingleRegionalWarping* warper = mRegionalWarping;
			for (int num = 0; num < mNumCamera; num++)
			{
				GLTexMask& tex = stitcher->inputArray->camIn(num)->getTexMask();
				if (tex.isMapSet())
					tex.setBeforeMap(warper->getMapBuffer(), warper->getWidth(), warper->getHeight());
				else
					tex.setBeforeMap(warper->getMapZeros(), warper->getWidth(), warper->getHeight());
			}
		}
		else if (is_request_change_param == STITCHING_TEMPLATE_REFRESH)
		{
			stitcher->updateStitchingTemplate();

			mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);
			if (!mAutoGainMode)
				mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);

			mWarpingMapThread = CreateThread(NULL, 0, run_warpMap, this, 0, NULL);
			is_request_change_param = 0;

			if (calibration_done_callback != NULL)
				calibration_done_callback(QVS_MSG_CALIBRATION_DONE);

			isDoingAnimation = false;
		}
		else if (is_request_change_param == UNDO_REDO_TRY)
		{
			stitcher->updateStitchingTemplate();

			mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);
			if (!mAutoGainMode)
				mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);

			is_request_change_param = 0;

			if (calibration_done_callback != NULL)
				calibration_done_callback(QVS_MSG_UNDOREDO_DONE);

			isDoingAnimation = false;
		}
		else if (is_request_change_param == UNDO_REDO_MASK_DONE)
		{
			stitcher->stitchingTemplate->mCircularQueue->pre_enqueue();
			stitcher->stitchingTemplate->mCameraParams->mIsRegionalWarping = false;
			stitcher->stitchingTemplate->mCircularQueue->enqueue(stitcher->stitchingTemplate->mCameraParams);
			history_callback(QVS_MSG_HISTORY_ENQUEUE);

			mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);
			if (!mAutoGainMode)
				mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);

			is_request_change_param = 0;
		}
		else if (is_request_change_param == ROTATE_MODE)
		{
			editMode = STITCHING_EDIT_MODE;
			float input_yaw = cal_ro_x;
			float input_pitch = cal_ro_y;
			float input_roll = cal_ro_z;

			cal_ro_x = 0;
			cal_ro_y = 0;
			cal_ro_z = 0;

			PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
			PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();
			/*		for (int i = 0; i < 6; i++)
			{
			printf("is_request_change_param i=%d, y=%f, p=%f, r=%f\n", i, image_param[i].y, image_param[i].p, image_param[i].r);

			}*/
			cv::Vec3b input_theta(input_roll, input_pitch, input_yaw);
			rotation_Image(input_theta, mNumCamera, image_param);

			//printf("KHJ, test mOutputType=%d, isDoingEvent=%d, mRotateScale=%f \n", mOutputType, isDoingEvent, mRotateScale);

			if (isStraighten)
			{
				isStraighten = false;
				if (calibration_done_callback != NULL)
					calibration_done_callback(QVS_MSG_STRAIGHTEN_START);

				setAfterImageParam(image_param);
				is_request_change_param = ANIMATION_MODE;
			}
			else
				stitcher->updateStitchingTemplate();
		}
		else if (is_request_change_param == STEREO_STITCHING_TB || is_request_change_param == STEREO_STITCHING_LR)
		{
			//setSeamMode(MASK_FOR_EDIT);
			//mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);
			((StitchComposer*)stitcher)->setStereoMode(stitchingMode);
			isStereoMode = true;
			//is_request_change_param = 0;
		}
		if (isStereoMode || stitchingMode == STITCHED_INPUT)
		{
			stitcher->stitching(w, h, idx, temp_videoCapture_state, input_tempData, panorama_preview_mode, mCurrentScale, mCurrentX, mCurrentY, warping_animTime, stereoIndex, editMode, stitchingMode);
			if (is_request_change_status == 1)
			{
				is_request_change_status = 0;
				setNadirCompensationMode(mNadirMode);
				setNadirCompensationParam(mNadirPosition, mNadirSize);
			}
		}
		else
		{
			if (is_request_change_status == 1)
			{
				is_request_change_status = 0;

				setBlendingMode(mBlendingMode);
				setSeamMode(mSeamMode);
				setGainMode(mGainMode);
				setNadirCompensationMode(mNadirMode);
				setNadirCompensationParam(mNadirPosition, mNadirSize);
			}
			//double tick = QClock();
			stitcher->stitching(w, h, idx, temp_videoCapture_state, input_tempData, panorama_preview_mode, mCurrentScale, mCurrentX, mCurrentY, warping_animTime, stereoIndex, editMode, stitchingMode);
			
		}
	}
	else{
		glClearColor(0, 0, 0, 1);
		glClear(GL_COLOR_BUFFER_BIT);
		glFlush();
		glFinish();
	}


	if (!is_VideoCaptureInit && is_load != 0)
	{
		VideoCaptureInit();
		AudioCaptureInit();

		if (load_callback != NULL)
			load_callback(QVS_MSG_DONE_INPUT_LOAD);
	}


	return QVS_SUCCESS;
}

unsigned char* EngineManager::setSelectPosition(int i, int bound_x, int bound_y, int bound_w, int bound_h)
{
	if (bound_w == 0 || bound_h == 0)
		return NULL;

	unsigned char* mask = (unsigned char*)malloc(input_tempData->mw * input_tempData->mh);
	memcpy(mask, input_tempData->seamMasks[i], input_tempData->mw * input_tempData->mh);

	int nCount = 0;
	int nSumX = 0;
	int nSumY = 0;
	for (int y = bound_y; y < bound_h; y++)
	{
		int index = y * input_tempData->mw;
		for (int x = 0; x < input_tempData->mw; x++)
		{
			if (mask[index + x] == 255)
			{
				if (x >= bound_x && x < bound_x + bound_w) {
					nCount++;
					nSumX += x;
					nSumY += y;
				}
				mask[index + x] = 0x3F;
			}
		}
	}

	if (nCount != 0)
	{
		mCenter_x[i] = 8.f * (float)nSumX / (float)nCount;
		mCenter_y[i] = 8.f * (float)nSumY / (float)nCount;
	}
	else
	{
		mCenter_x[i] = mCenter_y[i] = 0;
	}

	if (mCenter_x[i] < 0) mCenter_x[i] += output_width;
	if (mCenter_y[i] < 0) mCenter_y[i] += output_height;
	if (mCenter_x[i] > output_width) mCenter_x[i] -= output_width;
	if (mCenter_y[i] > output_height) mCenter_y[i] -= output_height;

	int cx = mCenter_x[i] / 8;
	int cy = mCenter_y[i] / 8;
	if (mask[cx + cy * input_tempData->mw] == 0) {
		free(mask);

		if (cx < bound_x + bound_w / 2)
			return setSelectPosition(i, bound_x, bound_y, bound_w / 2, bound_h);
		else
			return setSelectPosition(i, bound_x + bound_w / 2, 0, bound_w / 2, bound_h);
	}

	cv::Mat blurMask(input_tempData->mh, input_tempData->mw, CV_8UC1, mask);
	cv::GaussianBlur(blurMask, blurMask, cv::Size(5, 5), 0, 0, cv::BORDER_REFLECT);

	return mask;
}

int EngineManager::Drawing(int w, int h)
{
	if (!is_GenTextures)
	{
		GenTextures();
		is_GenTextures = 1;
	}

	if (is_Init == 0)
	{

	}
	else if (input_manager->isFinishBuffering())
	{
		int* idx = input_manager->getOutIndex(0);
		if (idx == 0)
			return QVS_FAIL;

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(1, 1, 1, 1);

		if (mEditInputMode == QVS_Engine::EditInputMode_Off)
		{
			for (int i = 0; i < mNumCamera; i++) {
				if (*temp_videoCapture_state[i] == -1)
					continue;

				float ratio = min(((float)w / (mNumVerticalLine + 1)) / (float)mCaptureWidth, ((float)h / (mNumHorizontalLine + 1)) / (float)mCaptureHeight);
				float viewportRatioWidth = (float)mCaptureWidth / w;
				float viewportRatioHeight = (float)mCaptureHeight / h;

				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glEnable(GL_TEXTURE_2D);
				glViewport(0, 0, w, h);
				glPushMatrix();

				glColor4f(1, 1, 1, 1);

				glTranslatef(
					2.0f * (i % (mNumVerticalLine + 1)) / (mNumVerticalLine + 1) + 2.0f / (mNumVerticalLine + 1) / 2 - 1,
					-(2.0f * (i / (mNumVerticalLine + 1)) / (mNumHorizontalLine + 1) + 2.0f / (mNumHorizontalLine + 1) / 2 - 1),
					0);

				glScalef(viewportRatioWidth * ratio, -(viewportRatioHeight * ratio), 1);

				GLuint* pbo = ((CamInputPBO*)(stitcher->inputArray->cams[i]))->getPBOID();

				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[idx[i]]);
				glBindTexture(GL_TEXTURE_2D, texId[i]);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mCaptureWidth, mCaptureHeight, GL_RGB, GL_UNSIGNED_BYTE, 0);
				glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
				drawRect();

				glDisable(GL_TEXTURE_2D);
				glPopMatrix();
			}
		}
		else if (mEditInputMode == QVS_Engine::EditInputMode_On)
		{
			if (is_SourceViewInit == 0) {
				for (int i = 0; i < mNumCamera; i++) {
					float viewW = w / (float)(mNumVerticalLine + 1);
					float viewH = h / (float)(mNumHorizontalLine + 1);

					GLuint* pbo = ((CamInputPBO*)(stitcher->inputArray->cams[i]))->getPBOID();
					mSourceTextures[i] = new Texture(i % (mNumVerticalLine + 1) * viewW, ((mNumHorizontalLine)-i / (mNumVerticalLine + 1)) * viewH, round(viewW), round(viewH), mCaptureWidth, mCaptureHeight, texId[i]);
				}

				is_SourceViewInit = 1;
			}

			for (int i = 0; i < mNumCamera; i++) {
				if (mEditInputTexture != mSourceTextures[i])
				{
					GLuint* pbo = ((CamInputPBO*)(stitcher->inputArray->cams[i]))->getPBOID();
					mSourceTextures[i]->draw(pbo[idx[i]]);
				}
			}
		}

		glViewport(0, 0, w, h);

		for (int i = 1; i <= mNumVerticalLine; i++)
		{
			glPushMatrix();
			glTranslatef(i * 2.0f / (mNumVerticalLine + 1) - 1, 0, 0);
			glColor3f(0.5f, 0.5f, 0.5f);
			drawLine_Center();
			glPopMatrix();
		}
		for (int i = 1; i <= mNumHorizontalLine; i++)
		{
			glPushMatrix();
			glTranslatef(0, i * 2.0f / (mNumHorizontalLine + 1) - 1, 0);
			glRotatef(90, 0, 0, 1);
			glColor3f(0.5f, 0.5f, 0.5f);
			drawLine_Center();
			glPopMatrix();
		}

		glFlush(); glFinish();

		glColor3f(1.0f, 1.0f, 1.0f);

		if (mEditInputMode == QVS_Engine::EditInputMode_On)
		{
			if (mEditInputTexture != NULL)
			{
				GLuint* pbo = ((CamInputPBO*)(stitcher->inputArray->cams[mEditInputTextureIndex]))->getPBOID();
				mEditInputTexture->draw(pbo[idx[mEditInputTextureIndex]]);
			}
		}
	}

	if (!is_VideoCaptureInit && is_load != 0)
	{
		VideoCaptureInit();
		AudioCaptureInit();

		if (load_callback != NULL)
			load_callback(QVS_MSG_DONE_INPUT_LOAD);
	}

	return QVS_SUCCESS;
}
int EngineManager::DrawingManualCalib(int w, int h)
{
	mIsRanderingStart = true;
	if (!is_GenTextures)
	{
		GenTextures();
		is_GenTextures = 1;
	}
	else if (input_manager->isFinishBuffering())
	{
		int* idx = input_manager->getOutIndex(0);
		if (idx == 0)
			return QVS_FAIL;

		int InputWidth = mCaptureWidth;
		int InputHeight = mCaptureHeight;
		glViewport(0, 0, w, h);
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		beginOrtho(w, h, InputWidth, InputHeight);
		double imageRatio = (double)InputWidth / (double)InputHeight;
		double viewRatio = (double)w / (double)h;
		double drawRatio;

		int imgWdt = InputWidth;
		int imgHgt = InputHeight;

		for (int i = 0; i < 2; i++) {
			int cur_index = (i == 0) ? mManual_calib_mask_left_index : mManual_calib_mask_right_index;

			if (*temp_videoCapture_state[cur_index] == -1)
				continue;
			glEnable(GL_TEXTURE_2D);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();

			gluLookAt(0.0f, 0.0f, -1.0f, 0.0f, 0.f, 0.0f, 0.0f, -1.0f, 0.0f);
			bool isHorizon;
			int rotateManualMask_angle;

			if (i == 0){
				glTranslated(-0.5, 0, 0);
				rotateManualMask_angle = mRotateManualMask_Left_angle;
			}
			else {
				glTranslated(0.5, 0, 0);
				rotateManualMask_angle = mRotateManualMask_Right_angle;
			}

			if (rotateManualMask_angle == 0 || rotateManualMask_angle == 180){
				isHorizon = true;
				imageRatio = (double)InputWidth / (double)InputHeight;

				imgWdt = InputWidth;
				imgHgt = InputHeight;
			}
			else{
				isHorizon = false;
				imageRatio = (double)InputHeight / (double)InputWidth;

				imgWdt = InputHeight;
				imgHgt = InputWidth;
			}

			double scale;

			int newWdt;
			int newHgt;

			if (imageRatio > viewRatio){
				scale = (viewRatio / imageRatio);
				glScaled(1, scale, 0);

				newWdt = w;
				newHgt = h*scale;
			}
			else{
				scale = (imageRatio / viewRatio);
				glScaled(scale, 1, 0);

				newWdt = w*scale;
				newHgt = h;
			}

			int rectWdt = w / 2;
			int rectHgt = h;

			double rectRatio = (double)rectWdt / (double)rectHgt;
			double newImgRatio = (double)newWdt / (double)newHgt;

			double scale_resize = 1.0;

			if (newImgRatio > rectRatio){
				scale_resize = (double)rectWdt / (double)newWdt;
			}
			else{
				scale_resize = (double)rectHgt / (double)newHgt;
			}
			glScaled(scale_resize, scale_resize, 0);

			glRotated(rotateManualMask_angle, 0, 0, 1);

			glGetDoublev(GL_MODELVIEW_MATRIX, mModelview_Mat[i]);
			/*for (int j = 0; j < 16; j++){
			std::cout << mModelview_Mat[i][j] << " ";
			}
			std::cout << std::endl;*/

			//image buffer
			GLuint* pbo = ((CamInputPBO*)(stitcher->inputArray->cams[cur_index]))->getPBOID();
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[idx[cur_index]]);
			glBindTexture(GL_TEXTURE_2D, texId[cur_index]);
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, InputWidth, InputHeight, GL_RGB, GL_UNSIGNED_BYTE, 0);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
			drawRect(); //drawRect_Vertex3f(0);

			//mask buffer
			{
				//QLOGD("RECT", "rect = %d, %d, %d, %d", mMaskUpdateRect.x, mMaskUpdateRect.y, mMaskUpdateRect.width, mMaskUpdateRect.height);
				glBindTexture(GL_TEXTURE_2D, mManual_RoiTexId[cur_index]);
#if 0
				if (mMaskUpdateRect.x < 0) mMaskUpdateRect.x = 0;
				if (mMaskUpdateRect.y < 0) mMaskUpdateRect.y = 0;
				if (mMaskUpdateRect.x + mMaskUpdateRect.width > mMaskCaptureWidth) mMaskUpdateRect.width = mMaskCaptureWidth - mMaskUpdateRect.x;
				if (mMaskUpdateRect.y + mMaskUpdateRect.height > mMaskCaptureHeight) mMaskUpdateRect.height = mMaskCaptureHeight - mMaskUpdateRect.y;
				glPixelStorei(GL_UNPACK_ROW_LENGTH, mMaskCaptureWidth);
				glTexSubImage2D(GL_TEXTURE_2D, 0, mMaskUpdateRect.x, mMaskUpdateRect.y, mMaskUpdateRect.width, mMaskUpdateRect.height,
					GL_BGRA, GL_UNSIGNED_BYTE, mRoi_Manual_buffer[cur_index] + ((mMaskUpdateRect.x + (mMaskCaptureWidth* mMaskUpdateRect.y)) * 4));
				glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
#else
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mMaskCaptureWidth, mMaskCaptureHeight, GL_BGRA, GL_UNSIGNED_BYTE, mRoi_Manual_buffer[cur_index]);
#endif
				drawRect(); //drawRect_Vertex3f(0);
				glDisable(GL_TEXTURE_2D);
			}
			glPopMatrix();
		}
		glColor3f(1, 1, 1);
		drawLine_Center();
		glGetDoublev(GL_PROJECTION_MATRIX, mProjection_Mat);

		//glGetIntegerv(GL_VIEWPORT, mViewPort_Mat);

		if (mROIMode == QVS_Engine::ROIMaskMode_Manual_On)
		{
			//beginOrtho(w, h, InputWidth, InputHeight);
			glMatrixMode(GL_MODELVIEW);
			glPushMatrix();
			glLoadIdentity();
			glViewport(0, 0, w, h);
			gluLookAt(0.0f, 0.0f, -1.0f, 0.0f, 0.f, 0.0f, 0.0f, -1.0f, 0.0f);
			//QLOGD("TEST", "%d, %d", mPoint_x, mPoint_y);

			glTranslated((double)(mPoint_x - (w / 2))*2.f / w, (double)((mPoint_y - h) + (h / 2))*2.f / h, 0.f);
			int isLeft = checkLeftMask(mPoint_x, mPoint_y, w, h);
			int rotateManualMask_angle;
			bool isHorizon;
			double rad_x, rad_y;
			if (isLeft){
				rotateManualMask_angle = mRotateManualMask_Left_angle;
			}
			else {
				rotateManualMask_angle = mRotateManualMask_Right_angle;
			}
			if (rotateManualMask_angle == 0 || rotateManualMask_angle == 180){
				isHorizon = true;
				imageRatio = (double)mMaskCaptureWidth / (double)mMaskCaptureHeight;

				imgWdt = mMaskCaptureWidth;
				imgHgt = mMaskCaptureHeight;
			}
			else{
				isHorizon = false;
				imageRatio = (double)mMaskCaptureHeight / (double)mMaskCaptureWidth;

				imgWdt = mMaskCaptureHeight;
				imgHgt = mMaskCaptureWidth;
			}
			double scale;

			int newWdt;
			int newHgt;

			if (imageRatio > viewRatio){
				scale = (viewRatio / imageRatio);
				rad_x = ((double)mSize);
				rad_y = ((double)mSize * scale);  //glScaled(1, scale, 0);

				newWdt = w;
				newHgt = h*scale;
			}
			else{
				scale = (imageRatio / viewRatio);
				rad_x = ((double)mSize * scale);  //glScaled(scale, 1, 0);
				rad_y = ((double)mSize);

				newWdt = w*scale;
				newHgt = h;
			}

			int rectWdt = w / 2;
			int rectHgt = h;

			double rectRatio = (double)rectWdt / (double)rectHgt;
			double newImgRatio = (double)newWdt / (double)newHgt;

			double scale_resize = 1.0;

			if (newImgRatio > rectRatio){
				scale_resize = (double)rectWdt / (double)newWdt;
			}
			else{
				scale_resize = (double)rectHgt / (double)newHgt;
			}
			rad_x = ((double)rad_x * scale_resize);
			rad_y = ((double)rad_y * scale_resize);

			rad_x *= ((double)2.f / imgWdt);
			rad_y *= ((double)2.f / imgHgt);

			drawCircle_Vertex3f(rad_x, rad_y);
			glPopMatrix();
			//if (mIsManualMaskLeft){
			//	//QLOGD("test", "mPoint(%d,%d) mPoint2(%d,%d)", mPoint_x, mPoint_y, mPoint_x2, mPoint_y2);
			//	glMatrixMode(GL_MODELVIEW);
			//	glPushMatrix();
			//	glViewport(0, 0, w, h);
			//	glLoadIdentity();
			//	gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.f, 0.0f, 0.0f, 1.0f, 0.0f);
			//	double xx, yy;
			//	if (mIsManualMaskLeft == 1)
			//		xx = (double)((MANUAL_PIXEL_WIDTH + MANUAL_PIXEL_CENTER_MARGIN + MANUAL_PIXEL_LEFT_MARGIN + (mPoint_x2 * MANUAL_PIXEL_WIDTH / InputHeight)));
			//	else if (mIsManualMaskLeft == 2)
			//		xx = (double)((MANUAL_PIXEL_LEFT_MARGIN + (mPoint_x2 * MANUAL_PIXEL_WIDTH / InputHeight)));
			//	yy = (double)(mPoint_y2 * MANUAL_PIXEL_HEIGHT / InputWidth);
			//	glTranslated((double)(xx - (w / 2))*2.f / w * aspectRatio, (double)((h - yy) - (h / 2))*2.f / h, 0.f);
			//	draw_circle(10);
			//	glPopMatrix();
			//}
			/*endOrtho();
			glFlush(); glFinish();*/
		}
		endOrtho();
		glFlush(); glFinish();
	}
	mIsRanderingStart = false;
	return QVS_SUCCESS;
}
int EngineManager::DrawingInteractive(int w, int h)
{
	if (is_Init == 0)
	{

	}
	else if (input_manager->isFinishBuffering())
	{
		int* idx = input_manager->getOutIndex(0);
		if (idx == 0)
			return QVS_FAIL;

		GLuint texid = stitcher->composer->getResTex();
		if (((StitchComposer*)stitcher->composer)->stitching_resolution_mode == STITCHING_16BY9)
			texid = ((StitchComposer*)stitcher->composer)->outTex;

		//glFinish();
		//double tick = QClock();
		display(w, h, texid);
		//glFinish();
		//printf("JW timeChk = %f\n", QClock() - tick);
	}

	if (!is_VideoCaptureInit && is_load != 0)
	{
		VideoCaptureInit();
		AudioCaptureInit();

		if (load_callback != NULL)
			load_callback(QVS_MSG_DONE_INPUT_LOAD);
	}

	return QVS_SUCCESS;
}

int EngineManager::setSavePath(const char *path)
{
	strcpy(save_path, path);

	return QVS_SUCCESS;
}

int EngineManager::IsNeedSave()
{
	printf("IsNeedSave() is_need_save=%d \n", is_need_save);
	if (is_need_save)
		return QVS_TRUE;
	else
		return QVS_FALSE;
}
#if(TEST_OVERLAYLOAD_THREAD)
DWORD WINAPI EngineManager::run_load_param(LPVOID handle)
{
	EngineManager *manager = (EngineManager *)handle;

	float fps;

	printf("bigheadk, LoadParam!!!\n\n");
#if 0 //FLTK
	wchar_t strUnicode[256] = { 0, };
	char strMultiByte[256] = { 0, };
	MultiByteToWideChar(CP_UTF8, 0, save_path, strlen(save_path), strUnicode, 256);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

	FILE* fp = fopen(strMultiByte, "rt");
#else //MFC
	FILE* fp = fopen(manager->save_path, "rt");
#endif

	if (fp == NULL)
	{
		printf("Load ERROR: PTS file open fail!\n");
		return QVS_MSG_FILE_CORRUP_ERROR;
	}

	if (manager->load_callback != NULL)
		manager->load_callback(QVS_MSG_DO_LOAD);

	char temp_s[MAX_PATH];
	char temp_w[MAX_PATH];
	char* temp_o = NULL;
	int temp_i;
	temp_w[0] = 0;

	int cmp;

	// Check Quram 360 Engine Version //
	int release_ver;
	int major_ver;
	int minor_ver;

	do
	{
		fgets(temp_s, MAX_PATH, fp);
		cmp = strcmp(temp_s, "# version:\n");
		if (cmp == 0)
			break;
	} while (!feof(fp));

	if (cmp != 0)
	{
		release_ver = 1;
		major_ver = 1;
		minor_ver = 1;
	}
	else
	{
		fscanf(fp, "v%d.%d.%d", &(release_ver), &(major_ver), &(minor_ver));
	}
	// Check Quram 360 Engine Version //

	do
	{
		fgets(temp_s, MAX_PATH, fp);
		cmp = strcmp(temp_s, "# application:\n");
		printf("%s", temp_s);
		if (cmp == 0)
			break;
	} while (!feof(fp));

	if (cmp != 0)
	{
		printf("Load ERROR: Not found \"# application:\"!\n");
		fclose(fp);
	}
	else
	{
		bool isMore;

		// input
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-input") == 0)
			{
				// skip one line
				fgets(temp_s, MAX_PATH, fp);

				for (int i = 0; i < strlen(temp_s) - 1; i++)
					temp_s[i] = temp_s[i + 1];
				temp_s[strlen(temp_s) - 1] = '/0';

				if (strlen(temp_s) > 0)
					sscanf(temp_s, "f %f", &(manager->mCaptureFPS));

				if (manager->mCaptureFPS <= 0)
					manager->mCaptureFPS = 30.f;
				//

				for (int i = 0; i < CAMERA_MAX; i++)
				{
					fgets(temp_s, MAX_PATH, fp);
					sscanf(temp_s, "%d ", &(manager->mVideoCaptureList[i].mCaptureDeviceIndex));

					int index = 0;
					for (int i = 0; i < strlen(temp_s); i++)
					{
						if (temp_s[i] == ' ')
						{
							index = i + 1;
							break;
						}
					}

					std::string tpath(temp_s + index);
					int lastidx = tpath.find_last_of("\n");
					std::string trimpath = tpath.substr(0, lastidx);
					memcpy(manager->mInputImagePath[i], trimpath.c_str(), trimpath.size());
					manager->mInputImagePath[i][trimpath.size()] = '\0';
					printf("bigheadk, input %s\n", manager->mInputImagePath[i]);
				}
				break;
			}
		}

		// status
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-status") == 0)
			{
				//fscanf(fp, "%s", mNadirPath);

				fgets(temp_s, MAX_PATH, fp);
				std::string tpath(temp_s + 1);
				int lastidx = tpath.find_last_of("\n");
				std::string trimpath = tpath.substr(0, lastidx);
				memcpy(manager->mNadirPath, trimpath.c_str(), trimpath.size());
				manager->mNadirPath[trimpath.size()] = '\0';

				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'e':
				sscanf(temp_w, "e %d", &(manager->mBlendingMode));
				break;
			case 's':
				sscanf(temp_w, "s %d", &(manager->mSeamMode));
				break;
			case 'g':
				sscanf(temp_w, "g %d", &(manager->mGainMode));
				break;
			case 'n':
				sscanf(temp_w, "n %d", &(manager->mNadirMode));
				break;
			case 'i':
				sscanf(temp_w, "i %d", &(manager->mNadirSize));
				break;
			case 'p':
				sscanf(temp_w, "p %d", &(manager->mNadirPosition));
				isMore = false;
				break;
				/*case 'm':
				sscanf(temp_w, "m %d", &(manager->is_StereoMode));
				break;*/
			default:
				break;
			}
		}
		// bmoutput
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-bmoutput") == 0)
			{
				//fscanf(fp, "%s", snapshot_path);

				fgets(temp_s, MAX_PATH, fp);
				std::string tpath(temp_s + 1);
				int lastidx = tpath.find_last_of("\n");
				std::string trimpath = tpath.substr(0, lastidx);
				memcpy(manager->snapshot_path, trimpath.c_str(), trimpath.size());
				manager->snapshot_path[trimpath.size()] = '\0';

				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'd':
				sscanf(temp_w, "d %d", &(manager->mBMOutputDevice));
				break;
			case 'p':
				sscanf(temp_w, "p %d", &(manager->mBMOutputDisplay));
				isMore = false;
				break;
			default:
				break;
			}
		}

		// panorama
		isMore = false;
		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "settings:") == 0) //# Panorama settings:
			{
				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'r':
				sscanf(temp_w, "r %lf", &(manager->renderFPS));
				break;
			case 's':
				sscanf(temp_w, "s %d", &(manager->mPanoramaSize));
				manager->setPanoramaSize(manager->mPanoramaSize);
				//((StitchComposer*)manager->stitcher->composer)->setStitchingResolutionMode(manager->mPanoramaSize);
				break;
			case 'f':
				sscanf(temp_w, "f %d", &(manager->mOutputType));
				break;
			case 'v':
				sscanf(temp_w, "v %f", &(manager->mOutputFov));
				isMore = false;
				break;
			default:
				break;
			}
		}

		if (fp) fclose(fp);
	}

	if (temp_o != NULL)
	{
		free(temp_o);
		temp_o = NULL;
	}

	manager->ParsingPTS(manager->save_path);

	//overlaytest_param check
	/*for (int i = 0; i < manager->mOverlayTemplateList.size(); i++)
	{
	OverlayTemplate* overlayTemplate = manager->mOverlayTemplateList.at(i);
	for (int k = 0; k < overlayTemplate->mOverlayList.size(); k++)
	{
	OverlayCapture* overlay = overlayTemplate->mOverlayList.at(k);
	PTSParam overlay_param = overlay->getParam();
	printf("OvermayTemplate[%d], id[%d] ooooo f%d y%lf r%lf p%lf v%lf a%lf b%lf c%lf C%d,%d,%d,%d\n", i, overlay->getId(), overlay_param.f, overlay_param.y, overlay_param.r, overlay_param.p, overlay_param.v, overlay_param.a, overlay_param.b, overlay_param.c, overlay_param.left, overlay_param.right, overlay_param.top, overlay_param.bottom);
	}
	}*/

	if (manager->load_callback != NULL)
		manager->load_callback(QVS_MSG_DONE_LOAD);

	manager->is_load = 1;

	return 0;
}
#endif
int EngineManager::SaveParam(int is_exit)
{
	printf("!!SaveParam start!!\n");

	is_load = 1;

	double startTime = QClock();
	wchar_t strUnicode[256] = { 0, };
	char strMultiByte[256] = { 0, };
	//MultiByteToWideChar(CP_UTF8, 0, save_path, strlen(save_path), strUnicode, 256);
	//WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();

	FILE* fp = fopen(save_path, "wt");
	if (fp == NULL)
	{
		printf("Save ERROR: File write fail!\n");
		return QVS_MSG_FILE_CORRUP_ERROR;
	}

	fprintf(fp, "# version:\n");
	fprintf(fp, "v%s\n\n", QURAM_360_ENGINE_VERSION);
	fprintf(fp, "# application:\n");
	fprintf(fp, "#-new\n");
	//fprintf(fp, "o n%d w%d h%d\n", mNumCamera, mCaptureWidth, mCaptureHeight);
	int mode = 0;
	if (stitchingMode == STEREO_STITCHING_TB)
		mode = (int)QVS_Engine::StitchingMode_Stereo;
	else if (stitchingMode == STEREO_STITCHING_LR)
		mode = (int)QVS_Engine::StitchingMode_Stereo_Side;
	else if (stitchingMode == STITCHED_INPUT)
		mode = (int)QVS_Engine::StitchingMode_StitchedInput;
	else if (stitchingMode == WIDE_STITCHING)
		mode = (int)QVS_Engine::StitchingMode_Wide;

	fprintf(fp, "o n%d w%d h%d x%d y%d m%d\n", mNumCamera, mCaptureWidth, mCaptureHeight, output_width, output_height, mode);
	fprintf(fp, "o n%d w%d h%d x%d y%d m%d\n", mNumCamera, mCaptureWidth, mCaptureHeight, output_width, output_height, mode);
	fprintf(fp, "o n%d w%d h%d x%d y%d m%d\n", mNumCamera, mCaptureWidth, mCaptureHeight, output_width, output_height, mode);
	fprintf(fp, "#-input f%lf\n", mCaptureFPS);
	for (int i = 0; i < CAMERA_MAX; i++)
	{
		fprintf(fp, "%d %s\n", mVideoCaptureList[i].mCaptureDeviceIndex, mInputImagePath[i]);
	}
	fprintf(fp, "#-status %s\n", mNadirPath);
	fprintf(fp, "o e%d s%d g%d n%d i%d p%d\n", mBlendingMode, mSeamMode, mGainMode, mNadirMode, mNadirSize, mNadirPosition);
	
	fprintf(fp, "#-bmoutput %s\n", snapshot_path);
	fprintf(fp, "o d%d p%d\n\n", mBMOutputDevice, mBMOutputDisplay);
	fprintf(fp, "# Panorama settings:\n");
	fprintf(fp, "o r%lf s%d f%d v%lf\n", renderFPS, mPanoramaSize, global_param->out_f, global_param->out_fov);

	fprintf(fp, "\n# input images:\n");
	fprintf(fp, "#-dummyimage\n");
	fprintf(fp, "# The following line contains a 'dummy image' containing some global parameters for the project\n");
	//fprintf(fp, "o w1 h1 y%lf r%lf p%lf v%lf a%lf b%lf c%lf f%d d0 e0 g0 t0\n", global_param.y, global_param.r, global_param.p, global_param.v, global_param.a, global_param.b, global_param.c, global_param.f);
	fprintf(fp, "o w1 h1 y%lf r%lf p%lf v%lf a%lf b%lf c%lf f%d d0 e0 g0 t0\n", global_param->y, global_param->r, global_param->p, global_param->v, global_param->a, global_param->b, global_param->c, global_param->f);
	for (int i = 0; i < mNumCamera; i++)
	{
		fprintf(fp, "#-imgfile %d %d\n", image_param[i].w, image_param[i].h);
		fprintf(fp, "#-metadata -1 -1 -1 0000-00-00T00:00:00 3*8 0 -1 -1 -1 * * * curve -1 * T * 13112775108774188\n");
		fprintf(fp, "#-viewpoint 0 0 0 0 0\n");
		fprintf(fp, "#-exposureparams 0 0 0 0\n");
		fprintf(fp, "#-qaspect %d %d\n", getAspectX(i), getAspectY(i));
		if (image_param[i].sourcesize > 0)
		{
			//fprintf(fp, "#-sourcemask %s\n", image_param[i].source_buffer);
			fprintf(fp, "#-sourcemask ");
			fwrite(image_param[i].source_buffer, image_param[i].sourcesize, 1, fp);
			fprintf(fp, "\n");
		}
		if (mSaveMapIndex[i] == true)
		{
			fprintf(fp, "#-roimap %d %d %d ", image_param[i].regional_warping_level, image_param[i].mapW, image_param[i].mapH);
			fwrite(image_param[i].regional_warping_buffer, strlen(image_param[i].regional_warping_buffer), 1, fp);
			fprintf(fp, "\n");
		}
		fprintf(fp, "o f%d y%lf r%lf p%lf v%lf a%lf b%lf c%lf d=0 e=0 g=0 t=0 C%d,%d,%d,%d\n", image_param[i].f, image_param[i].y, image_param[i].r, image_param[i].p, image_param[i].v, image_param[i].a, image_param[i].b, image_param[i].c, image_param[i].left, image_param[i].right, image_param[i].top, image_param[i].bottom);
	}
	is_need_save = false;

	if (fp) fclose(fp);

	printf("!!SaveParam end!!\n");

	double endtime = QClock();
	printf("!!SaveParam process : %lf!!\n", (endtime - startTime) / 1000.0);
	return QVS_SUCCESS;
}

int EngineManager::LoadParam(const char *path)
{
	setSavePath(path);

#if(TEST_OVERLAYLOAD_THREAD)
	mOverlayLoadThread = CreateThread(NULL, 0, run_load_param, this, 0, NULL);
	return QVS_SUCCESS;

#else
	float fps;

	printf("bigheadk, LoadParam!!!\n\n");
#if 0 //FLTK
	wchar_t strUnicode[256] = { 0, };
	char strMultiByte[256] = { 0, };
	MultiByteToWideChar(CP_UTF8, 0, save_path, strlen(save_path), strUnicode, 256);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

	FILE* fp = fopen(strMultiByte, "rt");
#else //MFC
	FILE* fp = fopen(save_path, "rt");
#endif

	if (fp == NULL)
	{
		printf("Load ERROR: PTS file open fail!\n");
		return QVS_MSG_FILE_CORRUP_ERROR;
	}

	char temp_s[MAX_PATH];
	char temp_w[MAX_PATH];
	int temp_i;
	temp_w[0] = 0;

	int cmp;

	// Check Quram 360 Engine Version //
	int release_ver;
	int major_ver;
	int minor_ver;

	do
	{
		fgets(temp_s, MAX_PATH, fp);
		cmp = strcmp(temp_s, "# version:\n");
		if (cmp == 0)
			break;
	} while (!feof(fp));

	if (cmp != 0)
	{
		release_ver = 1;
		major_ver = 1;
		minor_ver = 1;
	}
	else
	{
		fscanf(fp, "v%d.%d.%d", &(release_ver), &(major_ver), &(minor_ver));
	}
	// Check Quram 360 Engine Version //

	do
	{
		fgets(temp_s, MAX_PATH, fp);
		cmp = strcmp(temp_s, "# application:\n");
		printf("%s", temp_s);
		if (cmp == 0)
			break;
	} while (!feof(fp));

	if (cmp != 0)
	{
		printf("Load ERROR: Not found \"# application:\"!\n");
		fclose(fp);
	}
	else
	{
		bool isMore;

		// input
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-input") == 0)
			{
				// skip one line
				fgets(temp_s, MAX_PATH, fp);
				//

				for (int i = 0; i < CAMERA_MAX; i++)
				{
					fgets(temp_s, MAX_PATH, fp);
					sscanf(temp_s, "%d ", &(mVideoCaptureList[i].mCaptureDeviceIndex));
					std::string tpath(temp_s + 2);
					int lastidx = tpath.find_last_of("\n");
					std::string trimpath = tpath.substr(0, lastidx);
					memcpy(mInputImagePath[i], trimpath.c_str(), trimpath.size());
					mInputImagePath[i][trimpath.size()] = '\0';
					printf("bigheadk, input %s\n", mInputImagePath[i]);
				}
				break;
			}
		}

		// status
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-status") == 0)
			{
				//fscanf(fp, "%s", mNadirPath);

				fgets(temp_s, MAX_PATH, fp);
				std::string tpath(temp_s + 1);
				int lastidx = tpath.find_last_of("\n");
				std::string trimpath = tpath.substr(0, lastidx);
				memcpy(mNadirPath, trimpath.c_str(), trimpath.size());
				mNadirPath[trimpath.size()] = '\0';

				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'e':
				sscanf(temp_w, "e %d", &(mBlendingMode));
				break;
			case 's':
				sscanf(temp_w, "s %d", &(mSeamMode));
				break;
			case 'g':
				sscanf(temp_w, "g %d", &(mGainMode));
				break;
			case 'n':
				sscanf(temp_w, "n %d", &(mNadirMode));
				break;
			case 'i':
				sscanf(temp_w, "i %d", &(mNadirSize));
				break;
			case 'p':
				sscanf(temp_w, "p %d", &(mNadirPosition));
				isMore = false;
				break;
			default:
				break;
			}
		}

		// hdd
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-hdd") == 0)
			{
				fscanf(fp, "%s", hdd_opts.outPath);
				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'w':
				sscanf(temp_w, "w %d", &(hdd_opts.width));
				break;
			case 'h':
				sscanf(temp_w, "h %d", &(hdd_opts.height));
				break;
			case 'm':
				sscanf(temp_w, "m %d", &temp_i);
				hdd_opts.rcMode = (temp_i == 0) ? 2 : 32;
				break;
			case 'b':
				sscanf(temp_w, "b %d", &(hdd_opts.avgBitrate));
				break;
			case 'g':
				sscanf(temp_w, "g %d", &(hdd_opts.gop));
				break;
			case 'a':
				sscanf(temp_w, "a %d", &(hdd_opts.audioBitrate));
				break;
			case 'c':
				sscanf(temp_w, "c %d", &(hdd_opts.colorSapce));
				break;
			case 'p':
				sscanf(temp_w, "p %d", &temp_i);
				switch (temp_i)
				{
				case QVS_Engine::Preset_HIGH_QUALITY:
					sprintf(hdd_opts.preset, "%s", "hq");
					break;
				case QVS_Engine::Preset_HIGH_PERFORMANCE:
					sprintf(hdd_opts.preset, "%s", "hp");
					break;
				case QVS_Engine::Preset_LOW_LATENCY_HIGH_PERFORMANCE:
					sprintf(hdd_opts.preset, "%s", "lowLatencyHP");
					break;
				case QVS_Engine::Preset_LOW_LATENCY_HIGH_QUALITY:
					sprintf(hdd_opts.preset, "%s", "lowLatencyHQ");
					break;
				}
				//sprintf(hdd_opts.preset, "%s", (temp_i == 0) ? "hq" : "hp");
				break;
			case 'f':
				sscanf(temp_w, "f %f", &(fps));
				hdd_opts.fps = fps;
				break;
			case 'd':
				sscanf(temp_w, "d %d", &(hdd_opts.codec));
				isMore = false;
				break;
			default:
				break;
			}
		}

		// rtmp
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-rtmp") == 0)
			{
				fscanf(fp, "%s", rtmp_opts.outPath);
				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'w':
				sscanf(temp_w, "w %d", &(rtmp_opts.width));
				break;
			case 'h':
				sscanf(temp_w, "h %d", &(rtmp_opts.height));
				break;
			case 'm':
				sscanf(temp_w, "m %d", &temp_i);
				rtmp_opts.rcMode = (temp_i == 0) ? 2 : 32;
				break;
			case 'b':
				sscanf(temp_w, "b %d", &(rtmp_opts.avgBitrate));
				break;
			case 'g':
				sscanf(temp_w, "g %d", &(rtmp_opts.gop));
				break;
			case 'a':
				sscanf(temp_w, "a %d", &(rtmp_opts.audioBitrate));
				break;
			case 'c':
				sscanf(temp_w, "c %d", &(rtmp_opts.colorSapce));
				break;
			case 'p':
				sscanf(temp_w, "p %d", &temp_i);
				switch (temp_i)
				{
				case QVS_Engine::Preset_HIGH_QUALITY:
					sprintf(rtmp_opts.preset, "%s", "hq");
					break;
				case QVS_Engine::Preset_HIGH_PERFORMANCE:
					sprintf(rtmp_opts.preset, "%s", "hp");
					break;
				case QVS_Engine::Preset_LOW_LATENCY_HIGH_PERFORMANCE:
					sprintf(rtmp_opts.preset, "%s", "lowLatencyHP");
					break;
				case QVS_Engine::Preset_LOW_LATENCY_HIGH_QUALITY:
					sprintf(rtmp_opts.preset, "%s", "lowLatencyHQ");
					break;
				}
				break;
			case 'f':
				sscanf(temp_w, "f %f", &(fps));
				rtmp_opts.fps = fps;
				break;
			case 'd':
				sscanf(temp_w, "d %d", &(rtmp_opts.codec));
				break;
			case 'r':
				sscanf(temp_w, "r %d", &(rtmp_opts.isAutoRestartStreaming));
				isMore = false;
				break;
			case 'y':
				//sscanf(temp_w, "y %d", &(rtmp_opts.isDynamicFpsEnable));
				//isMore = false;
				break;
			default:
				break;
			}
		}


		// bmoutput
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-bmoutput") == 0)
			{
				//fscanf(fp, "%s", snapshot_path);

				fgets(temp_s, MAX_PATH, fp);
				std::string tpath(temp_s + 1);
				int lastidx = tpath.find_last_of("\n");
				std::string trimpath = tpath.substr(0, lastidx);
				memcpy(snapshot_path, trimpath.c_str(), trimpath.size());
				snapshot_path[trimpath.size()] = '\0';

				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 'd':
				sscanf(temp_w, "d %d", &(mBMOutputDevice));
				break;
			case 'p':
				sscanf(temp_w, "p %d", &(mBMOutputDisplay));
				isMore = false;
				break;
			default:
				break;
			}
		}

		// panorama
		isMore = false;

		while (fscanf(fp, "%s", temp_w) != EOF)
		{
			if (strcmp(temp_w, "#-panorama") == 0)
			{
				//fscanf(fp, "%s", snapshot_path);
				isMore = true;
				break;
			}
		}

		while (isMore)
		{
			if (fscanf(fp, "%s", temp_w) == EOF)
			{
				printf("Load ERROR: Not found configuration value!\n");
				fclose(fp);
				return QVS_MSG_PARSING_NOT_FOUND_ERROR;
			}

			switch (temp_w[0])
			{
			case 's':
				sscanf(temp_w, "s %d", &(mPanoramaSize));
				((StitchComposer*)stitcher->composer)->setStitchingResolutionMode(mPanoramaSize);
				isMore = false;
				break;
			default:
				break;
			}
		}

		rtmp_opts.width = hdd_opts.width;
		rtmp_opts.height = hdd_opts.height;

		if (hdd_opts.width == 7680 /*Output이 8K일때*/)
		{
			sprintf(hdd_opts.preset, "%s", "lowLatencyHP");
			hdd_opts.codec = 1;
		}
		if (rtmp_opts.width == 7680 /*Output이 8K일때*/)
		{
			sprintf(rtmp_opts.preset, "%s", "lowLatencyHP");
			rtmp_opts.codec = 1;
		}

		if (fp) fclose(fp);
	}

	return ParsingPTS(path);
#endif
}

int EngineManager::setSnapshotPath(const char *path)
{
	strcpy(snapshot_path, path);

	printf("bigheadk, setSnapshotPath!\n");
	is_need_save = true;
	return QVS_SUCCESS;
}

int EngineManager::SnapShot(LONGLONG delay)
{
	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mVideoCapture)
			mVideoCaptureList[i].mVideoCapture->SnapShot(delay);
	}

	return 1;
}

int EngineManager::ParsingPTS(const char *path)
{
#if 0 //FLTK
	wchar_t strUnicode[256] = { 0, };
	char strMultiByte[256] = { 0, };
	MultiByteToWideChar(CP_UTF8, 0, path, strlen(path), strUnicode, 256);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();

	FILE* fp = fopen(strMultiByte, "rt");
#else //MFC
	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();

	global_param->out_f = mOutputType;
	global_param->out_fov = mOutputFov;
	FILE* fp = fopen(path, "rt");
#endif
	if (fp == NULL)
	{
		printf("PTS Parsing ERROR: PTS file open fail!\n");
		return QVS_MSG_FILE_CORRUP_ERROR;
	}

	char temp_s[MAX_PATH];
	char temp_w[1000000];
	temp_w[0] = 0;
	int cmp;

	do
	{
		fgets(temp_s, MAX_PATH, fp);
		//cmp = strcmp(temp_s, "#PanoramaSettings:\n");
		cmp = strcmp(temp_s, "# Panorama settings:\n");
		if (cmp == 0)
			break;
	} while (!feof(fp));

	//if (cmp != 0)
	//{
	//	printf("PTS Parsing ERROR: Not found \"# Panorama settings:\"!\n");
	//	fclose(fp);
	//	return QVS_MSG_PARSING_NOT_FOUND_ERROR;
	//}

	bool isMore = true;
	while (isMore)
	{
		if (fscanf(fp, "%s", temp_w) == EOF)
		{
			printf("Load ERROR1: Not found configuration value!\n");
			fclose(fp);
			return QVS_MSG_PARSING_NOT_FOUND_ERROR;
		}

		switch (temp_w[0])
		{
			/*case 'e':
			sscanf(temp_w, "e %d", &(manager->mBlendingMode));
			break;
			case 's':
			sscanf(temp_w, "s %d", &(manager->mSeamMode));
			break;
			case 'g':
			sscanf(temp_w, "g %d", &(manager->mGainMode));
			break;
			case 'i':
			sscanf(temp_w, "i %d", &(manager->mNadirSize));
			break;*/
		case 'f':
			sscanf(temp_w, "f %d", &(global_param->out_f));
			break;
		case 'v':
			sscanf(temp_w, "v %lf", &(global_param->out_fov));
			isMore = false;
			break;
		default:
			break;
		}
	}

	//int cmp;

	do
	{
		fgets(temp_s, MAX_PATH, fp);
		cmp = strcmp(temp_s, "# input images:\n");
		if (cmp == 0)
			break;
	} while (!feof(fp));

	if (cmp != 0)
	{
		printf("PTS Parsing ERROR: Not found \"# input images:\"!\n");
		fclose(fp);
		return QVS_MSG_PARSING_NOT_FOUND_ERROR;
	}

	// skip two line
	fgets(temp_s, MAX_PATH, fp);
	fgets(temp_s, MAX_PATH, fp);
	//

	// global param
	global_param->w = 0;
	global_param->h = 0;

	while (isMore)
	{
		if (fscanf(fp, "%s", temp_w) == EOF)
		{
			printf("PTS Parsing ERROR2: Not found configuration value!\n");
			fclose(fp);
			return QVS_MSG_PARSING_NOT_FOUND_ERROR;
		}

		switch (temp_w[0])
		{
		case 'y':
			sscanf(temp_w, "y %lf", &(global_param->y));
			break;
		case 'r':
			sscanf(temp_w, "r %lf", &(global_param->r));
			break;
		case 'p':
			sscanf(temp_w, "p %lf", &(global_param->p));
			break;
		case 'v':
			sscanf(temp_w, "v %lf", &(global_param->v));
			break;
		case 'a':
			sscanf(temp_w, "a %lf", &(global_param->a));
			break;
		case 'b':
			sscanf(temp_w, "b %lf", &(global_param->b));
			break;
		case 'c':
			sscanf(temp_w, "c %lf", &(global_param->c));
			isMore = false;
			break;
		default:
			break;
		}
	}
	// reset sourcemask
	for (int i = 0; i < mNumCamera; i++) {
		if (!image_param[i].sourcemask.empty())
		{
			if (image_param[i].sourcemask.rows > 0 && image_param[i].sourcemask.cols > 0)
			{
				image_param[i].sourcemask.release();
			}
		}
		if (image_param[i].source_buffer != 0)
		{
			free(image_param[i].source_buffer);
			image_param[i].source_buffer = NULL;
		}
		image_param[i].sourcesize = 0;
		switch (global_param->out_f)
		{
		case 0:
			image_param[i].out_f = RECTLINEAR;
			break;
		case 1:
			image_param[i].out_f = WIDE_PANORAMA;
			break;
		case 5:
			image_param[i].out_f = STEREO;
			break;
		default:
			image_param[i].out_f = EQUIRECTANGULAR;
			break;
		}

		image_param[i].out_fov = global_param->out_fov;
	}

	// reset sourcemask
	for (int i = 0; i < mNumCamera; i++) {
		image_param[i].is_clear = true;
		image_param[i].is_dirty = true;
	}

	// imgfile parameters
	int img_count = -1;
	bool isCPS = false;
	while (fscanf(fp, "%s", temp_w) != EOF)
	{
		if (strcmp(temp_w, "Control") == 0)
		{
			// input images end
			isCPS = true;
			break;
		}

		switch (temp_w[0])
		{
		case 'f':
			sscanf(temp_w, "f %d", &(image_param[img_count].f));
			break;
		case 'y':
			sscanf(temp_w, "y %lf", &(image_param[img_count].y));
			break;
		case 'r':
			sscanf(temp_w, "r %lf", &(image_param[img_count].r));
			break;
		case 'p':
			sscanf(temp_w, "p %lf", &(image_param[img_count].p));
			break;
		case 'v':
			if (temp_w[1] == '=')
				image_param[img_count].v = global_param->v;
			else
				sscanf(temp_w, "v %lf", &(image_param[img_count].v));
			break;
		case 'a':
			if (temp_w[1] == '=')
				image_param[img_count].a = global_param->a;
			else
				sscanf(temp_w, "a %lf", &(image_param[img_count].a));
			break;
		case 'b':
			if (temp_w[1] == '=')
				image_param[img_count].b = global_param->b;
			else
				sscanf(temp_w, "b %lf", &(image_param[img_count].b));
			break;
		case 'c':
			if (temp_w[1] == '=')
				image_param[img_count].c = global_param->c;
			else
				sscanf(temp_w, "c %lf", &(image_param[img_count].c));
			break;
		case 'C':
			sscanf(temp_w, "C %d,%d,%d,%d", &(image_param[img_count].left), &(image_param[img_count].right), &(image_param[img_count].top), &(image_param[img_count].bottom));
			//image_param[img_count].left = 0;
			//image_param[img_count].right = 1920;
			//image_param[img_count].top = -420;
			//image_param[img_count].bottom = 1500;
			break;
		default:
			if (strcmp(temp_w, "#-imgfile") == 0)
			{
				img_count++;

				fscanf(fp, "%d %d %s", &(image_param[img_count].w), &(image_param[img_count].h), temp_w);

				//image_param[img_count].top = 0;
				//image_param[img_count].bottom = image_param[img_count].h;
				//image_param[img_count].left = 0;
				//image_param[img_count].right = image_param[img_count].w;

				//char *img_name = strtok(temp_w, "\"");
				//strcpy(image_param[img_count].name, img_name);
			}
			else if (strcmp(temp_w, "#-qaspect") == 0)
			{
				int aspectX, aspectY;
				fscanf(fp, "%d %d", &aspectX, &aspectY);
				mVideoCaptureList[img_count].mAspectX = aspectX;
				mVideoCaptureList[img_count].mAspectY = aspectY;
			}
			else if (strcmp(temp_w, "#-sourcemask") == 0)
			{
				fscanf(fp, "%s", temp_w);
				int size = strlen(temp_w);
				if (base64ImageDecoder == NULL)
				{
					base64ImageDecoder = new Base64ImageDecoder();
				}

				image_param[img_count].sourcemask = base64ImageDecoder->base64_decode(temp_w, size);

				image_param[img_count].source_buffer = (char*)malloc(size);
				image_param[img_count].sourcesize = size;
				memset(image_param[img_count].source_buffer, 0, size);
				memcpy(image_param[img_count].source_buffer, temp_w, size);
				//image_param[img_count].source_buffer[size] = '/0';

				if (!image_param[img_count].sourcemask.empty() && image_param[img_count].sourcemask.data != NULL){
					int index = 0;
					cv::Mat tempMat;
					cv::resize(image_param[img_count].sourcemask, tempMat, cv::Size(image_param[img_count].sourcemask.cols / 2, image_param[img_count].sourcemask.rows / 2));
					if (mRoi_Manual_buffer[img_count] == NULL)
						mRoi_Manual_buffer[img_count] = (unsigned char*)malloc(image_param[img_count].sourcemask.cols / 2 * image_param[img_count].sourcemask.rows / 2 * 4);
					for (int row = 0; row < tempMat.rows; row++){
						for (int col = 0; col < tempMat.cols; col++){
							bool check = false;
							for (int k = 0; k < 3; k++){
								uchar temp = mRoi_Manual_buffer[img_count][index++] = tempMat.at<cv::Vec3b>(row, col)[k];
								if (temp > 250) check = true;
							}
							if (check) mRoi_Manual_buffer[img_count][index++] = 0x3F;
							else mRoi_Manual_buffer[img_count][index++] = 0x00;
						}
					}
				}
			}
			else if (strcmp(temp_w, "#-roimap") == 0)
			{
				fscanf(fp, "%d", &(image_param[img_count].regional_warping_level));
				fscanf(fp, "%d", &(image_param[img_count].mapW));
				fscanf(fp, "%d", &(image_param[img_count].mapH));

				//char* decoded_buffer = (char*)mRegionalWarping->getFirstMapBuffer(img_count);
				char* decoded_buffer = (char*)malloc(mRegionalWarping->getWidth() * mRegionalWarping->getHeight() * 4);
				fscanf(fp, "%s", decoded_buffer);
				int size = strlen(decoded_buffer);

				char* source_buffer = (char*)malloc(size + 1);
				memcpy(source_buffer, decoded_buffer, size + 1);

				if (base64ImageDecoder == NULL)
				{
					base64ImageDecoder = new Base64ImageDecoder();
				}

				cv::Mat decoded_mat = base64ImageDecoder->base64_decode_for_ROIMapSave(decoded_buffer, size);

				//memset(decoded_buffer, 0, mCaptureWidth * mCaptureHeight * 4);
				memcpy(decoded_buffer, decoded_mat.data, mCaptureWidth * mCaptureHeight * 4);
				free(decoded_buffer);
				//mRegionalWarping->setFirstMapBuffer(img_count);
				//mRegionalWarping->setMapZeros(img_count);

				image_param[img_count].regional_warping_raw_buffer = (char*)malloc(mRegionalWarping->getWidth() * mRegionalWarping->getHeight() * 4);
				memcpy(image_param[img_count].regional_warping_raw_buffer, decoded_mat.data, mRegionalWarping->getWidth() * mRegionalWarping->getHeight() * 4);

				decoded_mat.release();

				image_param[img_count].regional_warping_size = strlen(source_buffer) + 1;
				image_param[img_count].regional_warping_buffer = source_buffer;

				image_param[img_count].is_clear = false;
				image_param[img_count].is_dirty = true;
				mRegionalWarpingMapIndex[img_count] = 1;
				is_RegionWarping = 1;
			}
			break;
		}
	}
	/*
	char* script = NULL;
	script = LoadScriptFP(fp);
	if (script == NULL) QLOGD("QParser", "script null!!!");
	//QLOGD("QParser", "readControlPoints start!! %d", cps.size());
	//readControlPoints(script, cps);
	fclose(fp);
	free(script);
	*/

	fclose(fp);

	is_request_change_status = 1;
	is_request_change_param = STITCHING_INIT;

	return QVS_SUCCESS;
}

const char* EngineManager::getFPS()
{
	return stitcher->ttt;
}

int EngineManager::setAudioBufferOffset(int offset)
{
	// 170627 START Audio Buffering
	if (mAudioController != NULL)
		return mAudioController->setOffset(offset);
	else
		return 0;//value 0

	//if (mAudioPlayer != NULL)
	//	return mAudioPlayer->SetBufferOffset(offset);
	//else
	//	return 0;//value 0
	// 170627 END
}
int EngineManager::setVideoBufferOffset(int offset)
{
	return input_manager->setBufferoffset(offset);
}

void EngineManager::onSnapShotDone(void *handle, void *data, BYTE* buffer, int size, long long timeStamp)
{
	VideoCapture *info = (VideoCapture *)handle;
	EngineManager *manager = (EngineManager *)data;

	int bpp = size / (manager->getCaptureWidth() * manager->getCaptureHeight());
	int type = CV_8UC3;
	if (bpp == 1)
		type = CV_8UC1;
	else if (bpp == 3)
		type = CV_8UC3;
	else if (bpp == 4)
		type = CV_8UC4;

	char temp[1024];
	sprintf(temp, "SnapShotDebug handle : %p, data : %p, buffer : %p, size : %d, bpp : %d, type : %d", handle, data, buffer, size, bpp, type);
	printf("%s\n", temp);

	QVS_Engine::QVS_writeLog(temp, 1024);

	cv::Mat mat = cv::Mat(manager->mCaptureHeight, manager->mCaptureWidth, CV_8UC3, buffer);
	cv::cvtColor(mat, mat, CV_BGR2RGB);

	char path[MAX_PATH];

	sprintf(path, "%s\\snapshot_%d.jpg", manager->snapshot_path, info->getCameraIndex());
	std::string strPath = std::string(path);
	cv::imwrite(strPath, mat);

	printf("EngineManager::onSnapShotDone!!\n");
	manager->snapshot_done_callback(QVS_MSG_SNAPSHOT_DONE);

	/*
	char path[MAX_PATH];
	sprintf(path, "snapshot_%d.jpg", info->getCameraIndex());
	saveJPEG(path, info->getCaptureWidth(), info->getCaptureHeight(), buffer);
	*/
}

void EngineManager::onInputError(void *handle, void *data, int msg, long long timeStamp)
{
	Capture *info = (Capture *)handle;
	EngineManager *manager = (EngineManager *)data;

	switch (info->getClass())
	{
	case MW_VIDEO:
		break;
	case BM_VIDEO:
		break;
	case USB_VIDEO:
		break;
	case IMG_VIDEO:
		break;
	case MIC_AUDIO:
		manager->audioError_callback(msg);
		break;
	case MW_AUDIO:
		break;
	case BM_AUDIO:
		break;
	case USB_AUDIO:
		break;
	default:
		break;
	}
}

void EngineManager::onAudioCaptureDone(void *handle, void *data, unsigned char* buffer, int size, long long timeStamp)
{
	AudioCapture *info = (AudioCapture *)handle;
	EngineManager *manager = (EngineManager *)data;

	// 170627 START Audio Buffering
	if (manager->mAudioController != NULL)
		manager->mAudioController->push(buffer, size);

	//if (manager->is_AudioPlayerInit == 1)
	//{
	//	if (manager->mCaptureDeviceList[manager->mAudioDevice].mCaptureDeviceType != QVS_Engine::InputModuleType_Blackmagic)
	//	{
	//		if (manager->mAudioPlayer != NULL)
	//			manager->mAudioPlayer->PushBuffer(buffer, size);

	//		
	//	}
	//}
	//else if (manager->is_AudioPlayerInit == 0)
	//{
	//	if (manager->mCaptureDeviceList[manager->mAudioDevice].mCaptureDeviceType != QVS_Engine::InputModuleType_Blackmagic) {
	//		manager->mAudioPlayer = new AudioPlayer();
	//		manager->mAudioPlayer->LoadWaveData(info->getwFormatTag(), info->getnChannels(), info->getnSamplesPerSec(), info->getnAvgBytesPerSec(), info->getnBlockAlign(), info->getwBitsPerSample(), buffer, size);
	//		manager->mAudioPlayer->Play();
	//	}

	//	manager->is_AudioPlayerInit = 1;
	//	((StitchComposer*)manager->stitcher->composer)->availableSound = AVAILABLE_SOUND;
	//}

	//if (((StitchComposer*)manager->stitcher->composer)->isEncoding == QVS_MSG_ENCODING)
	//{
	//	if (manager->is_audio_header_hdd == 0)
	//	{
	//		//printf("push 1 START\n");
	//		manager->stitcher->composer->audio_push(manager->audio_capture->getHeaderBuffer(), manager->audio_capture->getHeaderBufferSize(), QVS_MSG_ENCODING);
	//		manager->stitcher->composer->audio_push(buffer, size, QVS_MSG_ENCODING);
	//		//printf("push 1 END\n");
	//		manager->is_audio_header_hdd = 1;
	//	}
	//	else
	//	{
	//		//printf("push 2 START\n");
	//		manager->stitcher->composer->audio_push(buffer, size, QVS_MSG_ENCODING);
	//		//printf("push 2 END\n");
	//	}
	//}
	//if (((StitchComposer*)manager->stitcher->composer)->isStreaming == QVS_MSG_STREAMING)
	//{
	//	if (manager->is_audio_header_rtmp == 0)
	//	{
	//		//printf("push 1 START\n");
	//		manager->stitcher->composer->audio_push(manager->audio_capture->getHeaderBuffer(), manager->audio_capture->getHeaderBufferSize(), QVS_MSG_STREAMING);
	//		manager->stitcher->composer->audio_push(buffer, size, QVS_MSG_STREAMING);
	//		//printf("push 1 END\n");
	//		manager->is_audio_header_rtmp = 1;
	//	}
	//	else
	//	{
	//		//printf("push 2 START\n");
	//		manager->stitcher->composer->audio_push(buffer, size, QVS_MSG_STREAMING);
	//		//printf("push 2 END\n");
	//	}
	//}
	// 170627 END
}

void EngineManager::onCaptureDone(void *handle, void *data, long long timeStamp, int captureIndex)
{
	VideoCapture *info = (VideoCapture *)handle;
	EngineManager *manager = (EngineManager *)data;

	int camera_idx = info->getCameraIndex();
	//int channel_idx = info->getChannelIndex();
	int channel_idx = captureIndex;

	//if (channel_idx == 0 && camera_idx == 0) {
	//	double nowTime = QClock();
	//	double diff = nowTime - manager->oldTime;
	//	manager->oldTime = nowTime;
	//	printf("%lf\n", diff);
	//}

#ifdef SDI_OUTPUT_TEST
	// sdi output test code
	if (camera_idx == 0)
	{
		manager->outputIndex++;
		if (manager->outputIndex == NUM_OUTPUT_BUFFERS)
			manager->outputIndex = 0;
	}
#endif
#if(0)
	// test code
	if (manager->stitcher->composer->temp_idx[camera_idx] == 1)
	{
		manager->stitcher->composer->temp_idx[camera_idx] = 2;
	}
	else
#endif
		manager->input_manager->finishCaptured(camera_idx, channel_idx, timeStamp);

	if (manager->stitchingMode != STITCHED_INPUT)
	{

		if (manager->input_manager->isFirstBuffering() && manager->is_thread_seam == 0 && manager->is_Init == 1)
		{
			manager->is_thread_seam = 1;
			manager->mSeamThread = CreateThread(NULL, 0, run_seam, manager, 0, NULL);
		}

		// copy input data to cpu
		if (channel_idx == 1)
		{
			if (manager->mIsSeamThread || manager->mIsGainThread || manager->mIsCalibrationThread)
			{
				manager->mCaptureDoneCounter++;
			}

			if (manager->mCaptureDoneCounter == manager->mNumCamera)
			{
				if (manager->mIsSeamThread)
				{
					manager->mIsSeamThread = false;
					manager->mSeamThread = CreateThread(NULL, 0, run_seam, manager, 0, NULL);
				}

				if (manager->mIsGainThread)
				{
					manager->mIsGainThread = false;
					if (manager->mGainMode == GAIN_CURVE_MANUAL_AVERAGE_COMPENSATION)
					{
						//TerminateThread(mGainThread, 0);
						manager->mGainThread = NULL;
						manager->doManualGainMode();

					}
					else
					{
						//TerminateThread(mManualCurveThread, 0);
						manager->mManualCurveThread = NULL;

						if (manager->mGainThread == NULL) {
							manager->mGainThread = CreateThread(NULL, 0, run_gain, manager, 0, NULL);
						}
						else {
							//ResumeThread(mGainThread);
							if (manager->mAutoGainMode)
							{
								ResumeThread(manager->mGainThread);
							}
							else
							{
								manager->mGainThread = NULL;
								manager->mGainThread = CreateThread(NULL, 0, run_gain, manager, 0, NULL);
							}
						}

					}
				}
				if (manager->stitchingMode == STEREO_STITCHING_TB || manager->stitchingMode == STEREO_STITCHING_LR) {
					if (manager->mlsStereoCalibrationThread){
						manager->mlsStereoCalibrationThread = false;
						manager->mStereoCalibrationThread = CreateThread(NULL, 0, run_stereo_calibration, manager, 0, NULL);
					}
				}
			}
		}
	}
}

void* EngineManager::getAddress(void *handle, void* data, int captureIndex)
{
	VideoCapture *info = (VideoCapture *)handle;
	EngineManager *manager = (EngineManager *)data;

	int camera_idx = info->getCameraIndex();
	//int channel_idx = info->getChannelIndex();
	int channel_idx = captureIndex;

#if(0)
	// test code
	if (manager->stitcher->composer->temp_idx[camera_idx] == 0)
	{
		manager->stitcher->composer->temp_idx[camera_idx] = 1;
		return manager->stitcher->composer->tempBuff[camera_idx];
	}
#endif

#ifdef SDI_OUTPUT_TEST
	if (camera_idx == 0)
	{
		//printf("INPUT manager->outputIndex : %d\n", manager->outputIndex);
		return manager->outputBuffers[manager->outputIndex];
	}
#endif

	int idx = manager->input_manager->getInIndex(camera_idx, channel_idx);

	if (channel_idx == 0)
	{
		return manager->stitcher->getPBOAddress(camera_idx, idx);
	}
	else if (channel_idx == 1)
	{
		return manager->input_tempData->getBufferAddress(camera_idx, idx);
	}

	return 0;
}

#ifdef SDI_OUTPUT_TEST
void* EngineManager::getOutputAddress(void *handle, void* data)
{
	EngineManager *manager = (EngineManager *)data;

	//printf("OUTPUT manager->outputIndex : %d -> %d\n", manager->outputIndex, ((manager->outputIndex - 1) + NUM_OUTPUT_BUFFERS) % NUM_OUTPUT_BUFFERS);

	//return manager->outputBuffers[((manager->outputIndex - 1) + NUM_OUTPUT_BUFFERS) % NUM_OUTPUT_BUFFERS];

	uchar* temp = (uchar*)malloc(1920 * 1080 * 3);
	memset(temp, 128, 1920 * 1080 * 3);

	return temp;
}
#endif

BOOL EngineManager::getDeviceList(int n, char* displayName, char* modelName)
{
	return BMVideoCapture::getDeviceList(n, displayName, modelName);
}

DWORD WINAPI EngineManager::onTimer_calibration(LPVOID handle)
{
	EngineManager *manager = (EngineManager *)handle;

	Sleep(3000);

	if (manager->mIsCalibrationThread == true)
	{
		manager->mIsCalibrationThread = false;
		manager->calibration_done_callback(QVS_MSG_CALIBRATION_TIMEOUT);
	}

	return 0;
}

DWORD WINAPI EngineManager::run_seam(LPVOID handle) {
	EngineManager *manager = (EngineManager *)handle;
	//printf("bigheadk, CreateThread\n");

	//while (1)
	{
		Sleep(500);

		if (!manager->isDoingAnimation && !manager->is_doing_seam)
		{
			int* idx = manager->input_manager->getOutIndex(1);
			manager->is_doing_seam = true;
			manager->makeSeamMask(manager->input_manager->getTimeStamp(idx[0], 1), idx, MASK_FOR_SEAM);
			manager->is_doing_seam = false;
			manager->seamMode_done_callback(QVS_MSG_SEAM_DONE);
			manager->isUndoRedo = false;
		}
	}
	return 0;
}
DWORD WINAPI EngineManager::run_gain(LPVOID handle) {
	EngineManager *manager = (EngineManager *)handle;
	printf("jw, run_gain CreateThread\n");

	//while (1)
	{
		Sleep(300);

		if (!manager->isDoingAnimation && !manager->is_doing_exposure)
		{
			if (manager->mAutoGainMode)
			{
				while (manager->mAutoGainMode)
				{
					//= abs(QClock() - timer)
					Sleep(2000);
					if (manager->is_request_change_param == ROTATE_MODE || manager->isDoingCalibration || manager->is_request_change_param == ANIMATION_MODE)
						continue;

					manager->is_doing_exposure = true;
					// Copy input data to CPU memory
					for (int i = 0; i < manager->mNumCamera; i++)
					{
						if (manager->mVideoCaptureList[i].mVideoCapture)
						{
							if (manager->mIsMultiStreaming == 0 || manager->mVideoCaptureList[i].mVideoCapture->getCaptureType() != QVS_Engine::InputModuleType_Magewell)
								manager->mVideoCaptureList[i].mVideoCapture->requestCopyInputToCPU();
						}
					}

					Sleep(300);
					int* idx = manager->input_manager->getOutIndex(1);
					//long long timestamp = (long long)QClock();
					//printf("run_gain: timestamp=%lld\n", timestamp);
					//manager->calculatePixelAvg(manager->input_manager->getTimeStamp(idx[0], 1), idx);
					manager->estimateExposureCompensation(idx);
					manager->is_doing_exposure = false;
					manager->autoGainMode_done_callback(QVS_MSG_AUTO_GAIN_DONE);
				}
			}
			else {
				Sleep(100);

				int* idx = manager->input_manager->getOutIndex(1);
				manager->is_doing_exposure = true;
				manager->estimateExposureCompensation(idx);
				manager->is_doing_exposure = false;
				manager->gainMode_done_callback(QVS_MSG_GAIN_DONE);
			}
		}
	}
	printf("jw, run_gain done CreateThread\n");
	return 0;
}
DWORD WINAPI EngineManager::run_manual_curve(LPVOID handle) {
	EngineManager *manager = (EngineManager *)handle;

	std::vector<std::vector<std::vector<cv::Point2d>>> curvePoints;// curvePoints.at(index)[RGB][MIN/MID/MAX]
	std::vector<cv::Mat> resultCurves;


	for (int i = 0; i < manager->mNumCamera; i++) {
		std::vector<std::vector<cv::Point2d>> rgbs;
		for (int j = 0; j < 3; j++) {
			std::vector<cv::Point2d> points;
			for (int k = 0; k < 3; k++) {
				cv::Point2d gain_curve_point(manager->mGainPointX[i][j][k], manager->mGainPointY[i][j][k]);
				points.push_back(gain_curve_point);
			}
			rgbs.push_back(points);
		}
		curvePoints.push_back(rgbs);
	}

	//Sleep(300);
	int* idx = manager->input_manager->getOutIndex(1);
	resultCurves = manager->getCurve((long long)QClock(), curvePoints);
	//printf("JW run_manual_curve: resultCurves.size()=%d\n", resultCurves.size());
	if (resultCurves.size() == manager->mNumCamera)
	{
		//draw in layout
		int i = manager->mGainChannelIndex;

		cv::cvtColor(resultCurves.at(i), resultCurves.at(i), CV_BGR2RGB);
		manager->mGainCurveData[i] = (void*)malloc(resultCurves.at(i).rows * resultCurves.at(i).cols * resultCurves.at(i).channels());
		memcpy(manager->mGainCurveData[i], resultCurves.at(i).data, resultCurves.at(i).rows * resultCurves.at(i).cols * resultCurves.at(i).channels());
		//printf("run_manual_curve done!!\n");

		manager->doManualGainMode_done_callback(QVS_MSG_MANUAL_GAIN_MODE_DONE);
	}
	else
	{
		//popup notify!!
		//printf("The Input points for Curve are incorrect!!\n");
		manager->doManualGainMode_done_callback(QVS_MSG_MANUAL_GAIN_MODE_FAIL);
	}

	return 0;
}

DWORD WINAPI EngineManager::run_stereo_calibration(LPVOID handle) {
	EngineManager* manager = (EngineManager *)handle;
	{
		//Sleep(300);
		int* idx = manager->input_manager->getOutIndex(1);
		manager->makeStereoInputData(manager->input_manager->getTimeStamp(idx[0], 1), idx);
	}
	return 0;
}

void EngineManager::makeStereoInputData(long long timeStamp, int* buffer_idx)
{
	
	if (is_doing_stereoCalibration == true)
	{
		return;
	}

	is_doing_stereoCalibration = true;

	if (stereoCalibration_done_callback != NULL)
		stereoCalibration_done_callback(QVS_MSG_STEREO_CALIBRATION_START);

	PTSParam					*image_param = stitcher->stitchingTemplate->getImageParams();
	PTSParam					*global_param = stitcher->stitchingTemplate->getIGlobalParam();

	int ret = 0;

	setBeforeImageParam(image_param);
	//double startTime = QClock();

	PTSParam output_image_param[CAMERA_MAX];

	for (int i = 0; i < mNumCamera; i++){
		output_image_param[i] = image_param[i];
	}
	printf("KHJ, makeStereoInputData START input_tempData->mNum_camera=%d\n", input_tempData->mNum_camera);
	std::vector<cv::Mat> imgs(input_tempData->mNum_camera);
	for (int i = 0; i < input_tempData->mNum_camera; i++){
		int w = image_param[i].cali_w;
		int h = image_param[i].cali_h;
		imgs[i] = cv::Mat(h, w, CV_8UC3, input_tempData->inputs[i].buffers[buffer_idx[i]]);
#if 1
		char tempFileName[1024];
		sprintf(tempFileName, "%s/circle/test_img_%d.jpg", "sample", i);
		imwrite(tempFileName, imgs[i]);
#endif
	}
	//ret = startAutoCrop(imgs, image_param, global_param, output_image_param, stitcher->stitchingTemplate->mCameraParams->warp_info);
	if (ret > 0){
		is_request_change_param = STEREO_STITCHING_TB;
		is_doing_stereoCalibration = false;

		for (int i = 0; i < mNumCamera; i++)
		{
			stitcher->inputArray->cams[i]->setPTSParam(image_param[i]);
		}

		global_param->v = image_param[0].v;

		if (stereoCalibration_done_callback != NULL)
			stereoCalibration_done_callback(QVS_MSG_STEREO_CALIBRATION_DONE);
	}
	else{
		switch (ret)
		{
			if (calibration_done_callback != NULL)
				calibration_done_callback(QVS_MSG_CALIBRATION_FAIL);
			is_doing_stereoCalibration = false;
			break;
		}
	}
}

bool EngineManager::startAutoCrop(std::vector<cv::Mat>& imgs, PTSParam* input_image_param, PTSParam *global_image_param, PTSParam* output_image_param, WarpInfo* warp_info)
{
	printf("KHJ, startAutoCrop 1\n");
	QCameraParams* cam;
	int w = input_image_param->cali_w;
	int h = input_image_param->cali_h;
	PTSParam* ptsp = input_image_param;
	printf("KHJ, startAutoCrop 1.5 ptsp->f=%d\n", ptsp->f);
	cam->img_lens_type = ptsp->f;

	int scale = 1;

	//int minside = MIN(w, h);
	//if (minside > DEFAULT_INPUT_WIDTH)
	//	scale = minside / DEFAULT_INPUT_WIDTH;

	//cv::resize(img, img, cv::Size(w / scale, h / scale));
	
	printf("KHJ, startAutoCrop 2\n");
	cam->org_Image = imgs[0].clone();
	printf("KHJ, startAutoCrop 3\n");

	cv::Mat img = imgs[0];
	printf("KHJ, startAutoCrop 4\n");
	cv::cvtColor(img, img, CV_RGB2GRAY);
	printf("KHJ, startAutoCrop 5\n");
	cam->index = index;
	cam->scale = scale;
	cam->out_width = output_width;
	cam->out_height = output_height;
	cam->img_width = ptsp->w;
	cam->img_height = ptsp->h;
	cam->s_width = cam->img_width / cam->scale;
	cam->s_height = cam->img_height / cam->scale;

	//cam->fov					= 120;
	cam->yaw = 0;
	cam->pitch = 0;
	cam->roll = 0;
	cam->lens_b = 0.f;
	cam->cropROI.x = 0;
	cam->cropROI.y = 0;
	cam->move_optimize_image = true;
	cam->Isdetect = true;
	cam->IsValued = false;
	cam->IsCrop = false;
	cam->IsWarp = false;

	cv::Mat sub_image;
	printf("KHJ, startAutoCrop 2 ptsp->f=%d\n", ptsp->f);
	if (ptsp->f == 0x02) {
		std::vector<cv::Vec3f> circles;
		cv::Rect boundingRect;
		circles_detector(imgs[0], circles, boundingRect, index);
		
		int margin = 0;
		//int margin = 0;
		makeCircularMask(imgs[0], cam->mask, circles, boundingRect, margin);
		printf("KHJ, startAutoCrop 5\n");
		if (circles.size() > 0)
		{
			cam->IsCrop = true;
			cam->cropROI.x = boundingRect.x;
			cam->cropROI.y = boundingRect.y;
			cam->cropROI.width = boundingRect.width;
			cam->cropROI.height = boundingRect.height;
			cam->img_width = cam->cropROI.width;
			cam->img_height = cam->cropROI.height;
		}
	};
	cam->s_Image = imgs[0];
	return true;
}

void EngineManager::circles_detector(cv::Mat &image, std::vector<cv::Vec3f> &circles, cv::Rect& boundingRect, int index, double canny_threshold, double circle_threshold, int numIterations)
{
	CV_Assert(image.type() == CV_8UC1 || image.type() == CV_8UC3);
	int minside = MIN(image.cols, image.rows);
	bool isVertical = image.cols < image.rows ? true : false;
	const int TSIZE = 256;
	cv::Mat smallimg;
	cv::Size tsize;
	double ratio = 1.0f;
	if (minside > TSIZE)
	{
		if (isVertical)
		{
			ratio = (double)TSIZE / (double)image.cols;
			tsize.width = TSIZE;
			tsize.height = cvRound(image.rows*ratio);
		}
		else{
			ratio = (double)TSIZE / (double)image.rows;
			tsize.width = cvRound(image.cols*ratio);
			tsize.height = TSIZE;
		}
		resize(image, smallimg, tsize);
	}
	else{
		smallimg = image.clone();
	}
	cv::Mat gray;
	if (smallimg.channels() == 3)
		cvtColor(smallimg, gray, CV_BGR2GRAY);
	else
		gray = smallimg.clone();
	circles.clear();
	// Edge Detection
	gray = gray - 16;
	gray.convertTo(gray, CV_8UC1, 16.0, -256);

	cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	cv::dilate(gray, gray, kernel, cv::Point(-1, -1), 5);

	cv::Mat edges;
	cv::Canny(gray, edges, MAX(canny_threshold / 2, 1), canny_threshold, 3);
	cv::dilate(edges, edges, kernel, cv::Point(-1, -1), 5);

	// Create point set from Canny Output
	std::vector<cv::Point2d> rawpoints;
	for (int r = 0; r < edges.rows; r++)
	{
		for (int c = 0; c < edges.cols / 2; c++)
		{
			if (edges.at<unsigned char>(r, c) == 255)
			{
				rawpoints.push_back(cv::Point2d(c, r));
				break;
			}
		}
		for (int c = edges.cols - 1; c >= edges.cols / 2; c--)
		{
			if (edges.at<unsigned char>(r, c) == 255)
			{
				rawpoints.push_back(cv::Point2d(c, r));
				break;
			}
		}
	}

	int sx = edges.cols*0.5*0.9;
	int ex = edges.cols*0.5*1.1;
	int sy = edges.rows*0.5*0.9;
	int ey = edges.rows*0.5*1.1;

	std::vector<double> dists;
	std::vector<double> sorteddists;
	int maxCnt = 0;
	int fixCX = sx;
	int fixCY = sy;
	float fixDist = 0;
#if 0
	for (int cy = sy; cy < ey; cy++)
	for (int cx = sx; cx < ex; cx++)
#else
	int cx = edges.cols*0.5;
	int cy = edges.rows*0.5;
#endif
	{
		cv::Point2d center(cx, cy);
		float mindist = (float)INT_MAX;
		float maxdist = (float)INT_MIN;
		for (int i = 0; i < rawpoints.size(); i++)
		{
			float dist = cv::norm(cv::Mat(rawpoints.at(i)), cv::Mat(center));
			dists.push_back(dist);
			mindist = min(mindist, dist);
			maxdist = max(maxdist, dist);
		}

		float err_th = min((maxdist - mindist)*0.05f, 2.828f);
		err_th = max(err_th, 1.414f);
		int cnt = 0;
		for (int i = 0; i < dists.size(); i++)
		{
			for (int j = 0; j < dists.size(); j++)
			{
				if (std::abs(dists.at(i) - dists.at(j)) < err_th)
				{
					cnt++;
				}
			}

			if (maxCnt <= cnt)
			{
				fixDist = dists.at(i);
				maxCnt = cnt;
				fixCX = cx;
				fixCY = cy;
			}

			cnt = 0;
		}
		dists.clear();
	}

	circles.push_back(cv::Vec3f(fixCX, fixCY, fixDist));

	cv::Vec3f maxcircle(0, 0, 0);

	for (int i = 0; i < (int)circles.size(); i++)
	{
		circles.at(i) *= (1.0f / ratio);
		cv::Vec3f circle = circles.at(i);
		double cx = circle[0];
		double cy = circle[1];
		double r = circle[2];
		if (r > maxcircle[2])
		{
			maxcircle[0] = cx; maxcircle[1] = cy; maxcircle[2] = r;
		}

		boundingRect.x = maxcircle[0] - maxcircle[2];
		boundingRect.y = maxcircle[1] - maxcircle[2];
		boundingRect.width = maxcircle[2] * 2;
		boundingRect.height = maxcircle[2] * 2;
	}

	boundingRect.x = MAX(0, boundingRect.x);
	boundingRect.y = MAX(0, boundingRect.y);
	boundingRect.width = MIN(image.cols - boundingRect.x, boundingRect.width);
	boundingRect.height = MIN(image.rows - boundingRect.y, boundingRect.height);

#if DEBUG_CIRCLES_DETECTOR
	{
		std::string outname("circle_detector_debug");
		outname = outname + std::to_string(cd_g_output_idx) + ".jpg";
		cv::Mat debug_image = smallimg.clone();
		cv::cvtColor(debug_image, debug_image, CV_GRAY2RGB);
		cv::Scalar red(0, 0, 255);
		cv::Scalar green(0, 255, 0);
		cv::Scalar blue(255, 0, 0);
		cv::Rect r(boundingRect);
		r.x *= ratio;
		r.y *= ratio;
		r.width *= ratio;
		r.height *= ratio;
		cv::rectangle(debug_image, r, green); // center
		//for (int i = 0; i < (int)rawpoints.size(); i++)
		//{
		//	cv::circle(debug_image, rawpoints[i], 1, green);
		//}
		maxcircle *= ratio;
		cv::circle(debug_image, cv::Point(maxcircle[0], maxcircle[1]), maxcircle[2], red, 1);
		cv::circle(debug_image, cv::Point(maxcircle[0], maxcircle[1]), 3, blue, -1);
		cv::imwrite(outname, debug_image);
		cd_g_output_idx++;
	}
#endif

	return;
}

int EngineManager::makeCircularMask(cv::Mat& Img, cv::Mat& mask, std::vector<cv::Vec3f> circles, cv::Rect& boundingRect, int margin)
{
	cv::Mat inputImage, dstImage, maskImage;
	inputImage = Img;
	maskImage = cv::Mat::zeros(inputImage.rows, inputImage.cols, CV_8UC1);
	if (circles.size() > 0) {
		cv::Point center(circles[0][0], circles[0][1]);
		int radius = circles[0][2] - margin;
		cv::circle(maskImage, center, radius, 255, CV_FILLED);
		radius += margin;
		boundingRect.x = center.x - radius;
		boundingRect.y = center.y - radius;
		boundingRect.width = radius * 2;
		boundingRect.height = radius * 2;
	}
	else {
		//maskImage = cv::Mat::ones(inputImage.rows, inputImage.cols, CV_8UC1);
		cv::GaussianBlur(inputImage, dstImage, cv::Size(0, 0), 4);
		cv::threshold(dstImage, maskImage, 20, 255, cv::THRESH_BINARY);
	}

	mask = maskImage;
	return 1;
}

void EngineManager::makeSeamMask(long long timeStamp, int* buffer_idx, int edit_mode)
{
	stitcher->makeSeamMask(input_tempData, timeStamp, buffer_idx, edit_mode);
}
/*void EngineManager::calculatePixelAvg(int* buffer_idx)
{
stitcher->calculatePixelAvg(input_tempData, buffer_idx);
}*/
void EngineManager::estimateExposureCompensation(int* buffer_idx)
{
	if (stitchingMode == STEREO_STITCHING_TB || stitchingMode == STEREO_STITCHING_LR)
	{
		int numLoop = mNumCamera / 2;
		for (int i = 0; i < numLoop; i++)
		{
			stitcher->estimateExposureCompensation(input_tempData, buffer_idx, i);
		}
	}
	else
	{
		stitcher->estimateExposureCompensation(input_tempData, buffer_idx);
	}
}
std::vector<cv::Mat> EngineManager::getCurve(long long timeStamp, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints)
{
	std::vector<cv::Mat> result = stitcher->getCurve(input_tempData, timeStamp, manualCurvePoints);

	return result;
}

int EngineManager::startOutput()
{
	//video_output->StartPlayback();

	return stitcher->startOutput(mBMOutputDevice, mBMOutputDisplay);
}
int EngineManager::finishOutput()
{
	//video_output->StopPlayback();

	return stitcher->finishOutput();
}

int EngineManager::setBlendingMode(int mode)
{
	stitcher->setBlendingMode(mode);
	mBlendingMode = mode;

	printf("bigheadk, setBlendingMode!\n");
	is_need_save = true;

	return QVS_SUCCESS;
}

int EngineManager::setSeamMode(int mode)
{
	mCaptureDoneCounter = 0;
	mIsSeamThread = true;

	// Copy input data to CPU memory
	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mVideoCapture)
		{
			if (mIsMultiStreaming == 0 || mVideoCaptureList[i].mVideoCapture->getCaptureType() != QVS_Engine::InputModuleType_Magewell)
				mVideoCaptureList[i].mVideoCapture->requestCopyInputToCPU();
		}
	}

	stitcher->setSeamMode(mode);

	// thread is created in onCaptureDone...
	//mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);

	mSeamMode = mode;

	printf("bigheadk, setSeamMode!\n");
	is_need_save = true;

	return QVS_SUCCESS;
}

int EngineManager::setGainMode(int mode)
{
	mCaptureDoneCounter = 0;
	mIsGainThread = true;

	int* idx = input_manager->getOutIndex(1);
	//printf("setGainMode: manager->input_manager->getTimeStamp(idx[0], 1)=%lld\n", input_manager->getTimeStamp(idx[0], 1));

	// Copy input data to CPU memory
	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mVideoCapture)
		{
			if (mIsMultiStreaming == 0 || mVideoCaptureList[i].mVideoCapture->getCaptureType() != QVS_Engine::InputModuleType_Magewell)
				mVideoCaptureList[i].mVideoCapture->requestCopyInputToCPU();
		}
	}

	// Do Set Gain Mode Code...
	stitcher->setGainMode(mode);

	// thread is created in onCaptureDone...
	////if (mode == GAIN_CURVE_MANUAL_RGB_COMPENSATION || mode == GAIN_CURVE_MANUAL_AVERAGE_COMPENSATION)
	//if (mode == GAIN_CURVE_MANUAL_AVERAGE_COMPENSATION)
	//{
	//	//TerminateThread(mGainThread, 0);
	//	mGainThread = NULL;
	//	doManualGainMode();
	//	
	//}
	//else
	//{
	//	//TerminateThread(mManualCurveThread, 0);
	//	mManualCurveThread = NULL;

	//	if (mGainThread == NULL) {
	//		mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);
	//	}
	//	else {
	//		//ResumeThread(mGainThread);
	//		if (mAutoGainMode)
	//		{
	//			ResumeThread(mGainThread);
	//		}
	//		else
	//		{
	//			mGainThread = NULL;
	//			mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);
	//		}
	//	}
	//	
	//}

	mGainMode = mode;

	printf("bigheadk, setGainMode!\n");
	is_need_save = true;
	//printf("JW EngineManager::setGainMode = %d\n", mGainMode);

	return QVS_SUCCESS;
}
int EngineManager::doManualGainMode()
{
	if (mManualCurveThread == NULL)
		mManualCurveThread = CreateThread(NULL, 0, run_manual_curve, this, 0, NULL);
	else {
		mManualCurveThread = NULL;
		mManualCurveThread = CreateThread(NULL, 0, run_manual_curve, this, 0, NULL);
		/*if (mAutoGainMode)
		{
		ResumeThread(mManualCurveThread);
		}
		else
		{
		mManualCurveThread = NULL;
		mManualCurveThread = CreateThread(NULL, 0, run_manual_curve, this, 0, NULL);
		}*/
	}
	//printf("JW Apply clicked!! doManualGainMode\n");

	return QVS_SUCCESS;
}
int EngineManager::setAutoGainMode(int mode)
{
	//if (mGainMode == GAIN_CURVE_MANUAL_RGB_COMPENSATION || mGainMode == GAIN_CURVE_MANUAL_AVERAGE_COMPENSATION)
	if (mGainMode == GAIN_CURVE_MANUAL_AVERAGE_COMPENSATION)
	{
		if (mode) {
			TerminateThread(mGainThread, 0);
			mGainThread = NULL;

			if (mManualCurveThread == NULL)
				mManualCurveThread = CreateThread(NULL, 0, run_manual_curve, this, 0, NULL);
			else {
				mManualCurveThread = NULL;
				mManualCurveThread = CreateThread(NULL, 0, run_manual_curve, this, 0, NULL);
			}
		}
		else {
			//mManualCurveThread = NULL;

			autoGainMode_done_callback(QVS_MSG_AUTO_GAIN_DONE);
		}
	}
	else
	{
		if (mode) {
			TerminateThread(mManualCurveThread, 0);
			mManualCurveThread = NULL;

			if (mGainThread == NULL)
				mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);
			else {
				mGainThread = NULL;
				mGainThread = CreateThread(NULL, 0, run_gain, this, 0, NULL);
			}
		}
		else {
			//mGainThread = NULL;

			autoGainMode_done_callback(QVS_MSG_AUTO_GAIN_DONE);
		}
	}

	mAutoGainMode = mode;

	printf("bigheadk, setAutoGainMode!\n");
	is_need_save = true;

	return QVS_SUCCESS;
}
int EngineManager::getAutoGainMode()
{
	return mAutoGainMode;
}

int EngineManager::setQuramCalibMode(int mode)
{
	mCaptureDoneCounter = 0;
	mIsCalibrationThread = true;

	isDoingCalibration = true;
	if (calibration_done_callback != NULL)
		calibration_done_callback(QVS_MSG_CALIBRATION_START);
	// Copy input data to CPU memory
	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mCaptureDeviceIndex == -1)
		{
			isDoingCalibration = false;
			if (calibration_done_callback != NULL)
				calibration_done_callback(QVS_MSG_CALIBRATION_FAIL);
			return QVS_FAIL;
		}

		if (mVideoCaptureList[i].mVideoCapture)
		{
			if (mIsMultiStreaming == 0 || mVideoCaptureList[i].mVideoCapture->getCaptureType() != QVS_Engine::InputModuleType_Magewell)
				mVideoCaptureList[i].mVideoCapture->requestCopyInputToCPU();
		}
	}

	mCalibrationMode = mode;

	// thread is created in onCaptureDone...
	//mCalibrationThread = CreateThread(NULL, 0, run_quram_calib, this, 0, NULL);

	CreateThread(NULL, 0, onTimer_calibration, this, 0, NULL);

	return QVS_SUCCESS;
}

void* EngineManager::detectAllSeamArea() {
	unsigned char* img[CAMERA_MAX];
	cv::Mat mat[CAMERA_MAX];

	for (int i = 0; i < mNumCamera; i++)
	{
		img[i] = (unsigned char*)malloc(input_tempData->mw * input_tempData->mh);
		memcpy(img[i], input_tempData->seamMasks[i], input_tempData->mw * input_tempData->mh);

		mat[i] = cv::Mat(input_tempData->mh, input_tempData->mw, CV_8UC1, img[i]);

		mat[i] = mat[i] / mNumCamera;

		if (i != 0)
			cv::add(mat[0], mat[i], mat[0]);
	}

	cv::threshold(mat[0], mat[0], 255 / mNumCamera * 1.5, 255, cv::THRESH_BINARY);

	int size = 20;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * size + 1, 2 * size + 1), cv::Point(size, size));
	cv::dilate(mat[0], mat[0], element);

	//cv::imwrite("all.png", mat[0]);

	unsigned char* result = (unsigned char*)malloc(input_tempData->mw * input_tempData->mh * 4);
	memset(result, 0, input_tempData->mw * input_tempData->mh * 4);

	cv::Mat result_mat = cv::Mat(input_tempData->mh, input_tempData->mw, CV_8UC4, result);

	std::vector<cv::Mat> planes;
	cv::split(result_mat, planes);
	mat[0].copyTo(planes[1]);
	planes[3] = cv::Scalar(0x3F);

	cv::merge(planes, result_mat);

	cv::resize(result_mat, result_mat, cv::Size(input_tempData->mw * 8, input_tempData->mh * 8), 0.0, 0.0, 1);

	void* result2 = malloc(input_tempData->mw * 8 * input_tempData->mh * 8 * 4);
	memcpy(result2, result_mat.data, input_tempData->mw * 8 * input_tempData->mh * 8 * 4);

	for (int i = 0; i < mNumCamera; i++)
	{
		free(img[i]);
	}
	free(result);

	return result2;
}

void* EngineManager::detectSeamArea(int index1, int index2) {
	unsigned char* img1 = (unsigned char*)malloc(input_tempData->mw * input_tempData->mh);
	memcpy(img1, input_tempData->seamMasks[index1], input_tempData->mw * input_tempData->mh);

	unsigned char* img2 = (unsigned char*)malloc(input_tempData->mw * input_tempData->mh);
	memcpy(img2, input_tempData->seamMasks[index2], input_tempData->mw * input_tempData->mh);

	cv::Mat mat1 = cv::Mat(input_tempData->mh, input_tempData->mw, CV_8UC1, img1);
	cv::Mat mat2 = cv::Mat(input_tempData->mh, input_tempData->mw, CV_8UC1, img2);

	cv::Mat pano1 = cv::Mat(input_tempData->mh, input_tempData->mw * 3, CV_8UC1);
	cv::Mat pano2 = cv::Mat(input_tempData->mh, input_tempData->mw * 3, CV_8UC1);

	cv::Mat sub1_left = pano1(cv::Rect(0, 0, input_tempData->mw, input_tempData->mh));
	cv::Mat sub1_center = pano1(cv::Rect(input_tempData->mw, 0, input_tempData->mw, input_tempData->mh));
	cv::Mat sub1_right = pano1(cv::Rect(input_tempData->mw * 2, 0, input_tempData->mw, input_tempData->mh));
	cv::Mat sub2_left = pano2(cv::Rect(0, 0, input_tempData->mw, input_tempData->mh));
	cv::Mat sub2_center = pano2(cv::Rect(input_tempData->mw, 0, input_tempData->mw, input_tempData->mh));
	cv::Mat sub2_right = pano2(cv::Rect(input_tempData->mw * 2, 0, input_tempData->mw, input_tempData->mh));

	mat1.copyTo(sub1_left);
	mat1.copyTo(sub1_center);
	mat1.copyTo(sub1_right);
	mat2.copyTo(sub2_left);
	mat2.copyTo(sub2_center);
	mat2.copyTo(sub2_right);

	int size = 15;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * size + 1, 2 * size + 1), cv::Point(size, size));
	cv::dilate(pano1, pano1, element);
	cv::dilate(pano2, pano2, element);

	cv::Mat result_mat;
	cv::bitwise_and(pano1, pano2, result_mat);

	cv::Mat result_sub = result_mat(cv::Rect(input_tempData->mw, 0, input_tempData->mw, input_tempData->mh));

	cv::threshold(result_sub, result_sub, 200, 255, cv::THRESH_BINARY);

	//char temp[256];
	//sprintf(temp, "result\\seam_%d_%d.png", index1, index2);
	//cv::imwrite(temp, result_sub);

	void* result = malloc(input_tempData->mw * input_tempData->mh);
	memcpy(result, result_sub.clone().data, input_tempData->mw * input_tempData->mh);

	free(img1);
	free(img2);

	return result;
}

int EngineManager::getStep(int mode)
{
	if (mode == QVS_Engine::CalibrationMode_Advanced)
		return mStep_update;

	if (mode == QVS_Engine::CalibrationMode_MakeTemplate)
		return mStep_align;

	//return mStep;
}

void EngineManager::setStep(int mode, int step)
{
	if (mode == QVS_Engine::CalibrationMode_Advanced)
		mStep_update = step;

	if (mode == QVS_Engine::CalibrationMode_MakeTemplate)
		mStep_align = step;

	//mStep = step;
}

int EngineManager::setNadirCompensationMode(int mode)
{
	stitcher->setNadirCompensationMode(mode);
	mNadirMode = mode;

	return QVS_SUCCESS;
}

int EngineManager::setNadirCompensationParam(int position, int size)
{
	if (position == NADIR_BOTTOM)
	{
		nadir_param.p = -90;
		mNadirPosition = position;
	}
	else if (position == NADIR_TOP)
	{
		nadir_param.p = 90;
		mNadirPosition = position;
	}

	if (size != -1)
	{
		nadir_param.v = 50 + (size - 17) * 1.5;
		mNadirSize = size;
	}

	stitcher->updateNadirParam(nadir_param);
	//stitcher->setNadirCompensationMode(mode);

	return QVS_SUCCESS;
}

void EngineManager::setManualRGB(int idx, int R, int G, int B)
{
	stitcher->setManualRGB(idx, R, G, B);
	mRed[idx] = R;
	mGreen[idx] = G;
	mBlue[idx] = B;

	printf("bigheadk, setManualRGB!\n");
	is_need_save = true;
}

int EngineManager::updateNadirImage()
{
#if 0 //FLTK
	wchar_t strUnicode[256] = { 0, };
	char strMultiByte[256] = { 0, };
	MultiByteToWideChar(CP_UTF8, 0, mNadirPath, strlen(mNadirPath), strUnicode, 256);
	WideCharToMultiByte(CP_ACP, 0, strUnicode, -1, strMultiByte, 256, NULL, NULL);

	return stitcher->updateNadirImage(strMultiByte);
#else
	return stitcher->updateNadirImage(mNadirPath);
#endif

	//return 1;
}

int EngineManager::updateStmap_L()
{
	cv::Mat img;
	img = cv::imread(mStmap_L_Path, CV_LOAD_IMAGE_UNCHANGED);
	if (img.data == NULL)
	{
		return 0;
	}
	else
	{
		if (img.channels() == 3)
		{
			cv::cvtColor(img, img, CV_BGR2RGBA);
		}
		else
		{
			cv::cvtColor(img, img, CV_BGRA2RGBA);
		}

		if (img.cols != output_width || img.rows != output_height)
		{
			cv::resize(img, img, cv::Size(output_width, output_height));
		}

		memcpy(buffer, img.data, output_width * output_height * 4);
		setImageRGBA(buffer, width, height);

		img.release();
		return QVS_SUCCESS;
	}
}

void EngineManager::updateStmap_R()
{

}

int EngineManager::setOutputConfiguration(QVS_Engine::Configuration config)
{

	if (strstr(config.rtmp_path, "rtmp") != NULL)
	{

	}
	else if (strstr(config.rtmp_path, "rtsp") != NULL)
	{

	}
	else if (strstr(config.rtmp_path, "udp") != NULL)
	{

	}
	else if (strstr(config.rtmp_path, "srt") != NULL)
	{

	}
	else
	{
		return QVS_FAIL;
	}

	mBMOutputDevice = config.bmoutput_device;
	mBMOutputDisplay = config.bmoutput_display;

	is_need_save = true;

	return QVS_SUCCESS;
}

int EngineManager::setInputConfiguration(float fps, int width, int height, int lens_type, float fov)
{
	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();

	mCaptureFPS = fps;
	mCaptureWidth = width;
	mCaptureHeight = height;
	global_param->f = lens_type;
	global_param->v = fov;
	for (int i = 0; i < mNumCamera; i++){
		image_param[i].f = lens_type;
		image_param[i].v = fov;
	}

	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mVideoCapture)
		{
			if (mVideoCaptureList[i].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
				((MWVideoCapture*)(mVideoCaptureList[i].mVideoCapture))->setCapturFPS(mCaptureFPS);
		}
	}

	printf("setInputConfiguration global_param.f=%d, fov=%f\n", global_param->f, global_param->v);

	is_need_save = true;

	return QVS_SUCCESS;
}

void EngineManager::setAudioDevice(int audioDevice)
{
	if (audioDevice < -1 || audioDevice >= (int)(mAudioCaptureDeviceList.size()))
	{
		printf("ERROR Audio Device Index : %d\n", audioDevice);
		return;
	}
	if (audioDevice == -1)
	{
		if (audio_capture != NULL)
		{
			if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
			{
				MWAudioCapture::destroy_AudioCapture((MWAudioCapture*)audio_capture);
			}
			else if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell_USB)
			{
				XIAudioCapture::destroy_AudioCapture((XIAudioCapture*)audio_capture);
			}
			else if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Blackmagic)
			{
				BMAudioCapture::destroy_AudioCapture((BMAudioCapture*)audio_capture);
			}
			else if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_MIC)
			{
				MICAudioCapture::destroy_AudioCapture((MICAudioCapture*)audio_capture);
			}
			audio_capture = NULL;
			mAudioDevice = -1;
		}

		// 170627 START Audio Buffering
		mAudioController->reset();

		//if (mAudioPlayer != NULL)
		//{
		//	delete mAudioPlayer;
		//	mAudioPlayer = NULL;
		//	is_AudioPlayerInit = 0;
		//}
		// 160627 END
	}
	else if (mAudioCaptureDeviceList[audioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
	{
		mAudioDevice = audioDevice;

		if (mAudioDevice > -1)
		{
			int offset = 0;
			for (int i = 0; i < mAudioCaptureDeviceList.size(); i++)
			{
				if (mAudioCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
					break;
				else
					offset++;

			}
			audio_capture = MWAudioCapture::create_AudioCapture(mAudioDevice - offset);
			audio_capture->setCaptureType(QVS_Engine::InputModuleType_Magewell);
			audio_capture->setCallback_onCaptureDone(onAudioCaptureDone, this);
		}
	}
	else if (mAudioCaptureDeviceList[audioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell_USB)
	{
		mAudioDevice = audioDevice;

		if (mAudioDevice > -1)
		{
			int offset = 0;
			for (int i = 0; i < mAudioCaptureDeviceList.size(); i++)
			{
				if (mAudioCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell_USB)
					break;
				else
					offset++;

			}
			audio_capture = XIAudioCapture::create_AudioCapture(mAudioDevice - offset);
			audio_capture->setCaptureType(QVS_Engine::InputModuleType_Magewell_USB);
			audio_capture->setCallback_onCaptureDone(onAudioCaptureDone, this);
		}
	}
	else if (mAudioCaptureDeviceList[audioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Blackmagic)
	{
		mAudioDevice = audioDevice;

		if (mAudioDevice > -1)
		{
			int offset = 0;
			for (int i = 0; i < mAudioCaptureDeviceList.size(); i++)
			{
				if (mAudioCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_Blackmagic)
					break;
				else
					offset++;

			}
			//audio_capture = BMAudioCapture::create_AudioCapture((BMVideoCapture*)video_capture[mAudioDevice - offset]);
			//audio_capture->setCaptureType(QVS_Engine::InputModuleType_Blackmagic);
			//audio_capture->setCallback_onCaptureDone(onAudioCaptureDone, this);
		}
	}
	else if (mAudioCaptureDeviceList[audioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_MIC)
	{
		mAudioDevice = audioDevice;

		if (mAudioDevice > -1)
		{
			int offset = 0;
			for (int i = 0; i < mAudioCaptureDeviceList.size(); i++)
			{
				if (mAudioCaptureDeviceList[i].mCaptureDeviceType == QVS_Engine::InputModuleType_MIC)
					break;
				else
					offset++;

			}
			audio_capture = MICAudioCapture::create_AudioCapture(mAudioDevice - offset);
			audio_capture->setCaptureType(QVS_Engine::InputModuleType_MIC);
			audio_capture->setCallback_onCaptureDone(onAudioCaptureDone, this);
			audio_capture->setCallback_onInputError(onInputError, this);
		}
	}

	printf("bigheadk, setAudioDevice! %d\n", audioDevice);
	is_need_save = true;
}

void EngineManager::destroyAudioDevice() {
	if (mAudioDevice < -1 || mAudioDevice >= (int)(mAudioCaptureDeviceList.size()))
	{
		printf("ERROR Audio Device Index : %d\n", mAudioDevice);
		return;
	}

	if (mAudioDevice == -1)
		return;

	if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell)
	{
		if (audio_capture != NULL)
		{
			MWAudioCapture::destroy_AudioCapture((MWAudioCapture*)audio_capture);
			audio_capture = NULL;
			mAudioDevice = -1;
		}

		// 170627 START Audio Buffering
		mAudioController->reset();

		//if (mAudioPlayer != NULL)
		//{
		//	delete mAudioPlayer;
		//	mAudioPlayer = NULL;
		//	is_AudioPlayerInit = 0;
		//}
		// 160627 END
	}
	else if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Magewell_USB)
	{
		if (audio_capture != NULL)
		{
			XIAudioCapture::destroy_AudioCapture((XIAudioCapture*)audio_capture);
			audio_capture = NULL;
			mAudioDevice = -1;
		}

		// 170627 START Audio Buffering
		mAudioController->reset();

		//if (mAudioPlayer != NULL)
		//{
		//	delete mAudioPlayer;
		//	mAudioPlayer = NULL;
		//	is_AudioPlayerInit = 0;
		//}
		// 160627 END
	}
	else if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_Blackmagic)
	{
		if (audio_capture != NULL)
		{
			BMAudioCapture::destroy_AudioCapture((BMAudioCapture*)audio_capture);
			audio_capture = NULL;
			mAudioDevice = -1;
		}

		// 170627 START Audio Buffering
		mAudioController->reset();

		//if (mAudioPlayer != NULL)
		//{
		//	delete mAudioPlayer;
		//	mAudioPlayer = NULL;
		//	is_AudioPlayerInit = 0;
		//}
		// 160627 END
	}
	else if (mAudioCaptureDeviceList[mAudioDevice].mCaptureDeviceType == QVS_Engine::InputModuleType_MIC)
	{
		if (audio_capture != NULL)
		{
			MICAudioCapture::destroy_AudioCapture((MICAudioCapture*)audio_capture);
			audio_capture = NULL;
			mAudioDevice = -1;
		}

		// 170627 START Audio Buffering
		mAudioController->reset();

		//if (mAudioPlayer != NULL)
		//{
		//	delete mAudioPlayer;
		//	mAudioPlayer = NULL;
		//	is_AudioPlayerInit = 0;
		//}
		// 160627 END
	}
}

void EngineManager::setPanoramaSize(int panoramaSize)
{
	printf("bigheadk, setPanoramaSize!\n");
	mPanoramaSize = panoramaSize;

	is_need_save = true;

	((StitchComposer*)stitcher->composer)->setStitchingResolutionMode(mPanoramaSize);
}

// bigheadk for calibration
DWORD WINAPI EngineManager::run_warpMap(LPVOID handle) {
	printf("updateWarpMap\n");
	double start_time = QClock();
	EngineManager *manager = (EngineManager *)handle;
	if (manager->makeWapmap_callback != NULL)
		manager->makeWapmap_callback(QVS_MSG_MAKE_START_WARPMAP);
	for (int i = 0; i < manager->mNumCamera; i++)
	{
		manager->stitcher->stitchingTemplate->mCameraParams->warp_info[i].data_state = INVALID_DATA;
	}
	manager->stitcher->stitchingTemplate->mCircularQueue->pre_enqueue();
	manager->history_callback(QVS_MSG_HISTORY_ENQUEUE);
	Sleep(300);
	manager->updateWarpMap();

	manager->stitcher->stitchingTemplate->mCameraParams->mIsRegionalWarping = false;
	manager->stitcher->stitchingTemplate->mCircularQueue->enqueue(manager->stitcher->stitchingTemplate->mCameraParams);
	for (int i = 0; i < manager->mNumCamera; i++)
	{
		manager->stitcher->stitchingTemplate->mCameraParams->warp_info[i].data_state = VALID_DATA;
	}

	if (manager->makeWapmap_callback != NULL)
		manager->makeWapmap_callback(QVS_MSG_MAKE_DONE_WARPMAP);
	printf("bigheadk, run_warpMap time = %lf\n", (QClock() - start_time));
	printf("updateWarpMap Done\n");
	return 0;
}

DWORD WINAPI EngineManager::run_edit_mode(LPVOID handle) {
	EngineManager *manager = (EngineManager *)handle;
	manager->checkWarpingMap();

	int* idx = manager->input_manager->getOutIndex(1);
	manager->makeSeamMask(manager->input_manager->getTimeStamp(idx[0], 1), idx, MASK_FOR_EDIT);

	manager->is_editmode = true;
	manager->is_request_change_param = ROTATE_MODE;
	return 0;
}

DWORD WINAPI EngineManager::run_undo(LPVOID handle) {
	EngineManager *manager = (EngineManager *)handle;
	manager->isDoingAnimation = true;
	manager->checkWarpingMap(false);

	CameraParams* temp = manager->stitcher->stitchingTemplate->mCircularQueue->undo();
	if (temp == NULL || temp == 0)
	{
		manager->isDoingAnimation = false;
		return false;
	}

	if (manager->calibration_done_callback != NULL)
		manager->calibration_done_callback(QVS_MSG_UNDOREDO_START);

	// rw
	if (manager->stitcher->stitchingTemplate->mCameraParams->mIsRegionalWarping == true)
	{
		for (int i = 0; i < manager->mNumCamera; i++)
		{
			if (manager->stitcher->stitchingTemplate->getImageParams()[i].is_dirty == true)
			{
				if (temp->image_param[i].regional_warping_buffer == NULL)
				{
					//GLTexMask& tex = manager->stitcher->inputArray->camIn(i)->getTexMask();
					//if (tex.isMapSet() == 1)
					//	tex.doResetAnimation();

					//tex.resetMap();
					//manager->mSaveMapIndex[i] = false;

					//manager->warping_animTime = 0.0f;
					manager->mRegionalWarpingBeforeMapZeros[i] = 1;
				}
				else
				{
					manager->mRegionalWarpingUndoMapIndex[i] = 1;

					//PTSParam* image_param = manager->stitcher->stitchingTemplate->getImageParams();

					int size = strlen(temp->image_param[i].regional_warping_buffer);

					if (manager->base64ImageDecoder == NULL)
					{
						manager->base64ImageDecoder = new Base64ImageDecoder();
					}

					cv::Mat decoded_mat = manager->base64ImageDecoder->base64_decode_for_ROIMapSave(temp->image_param[i].regional_warping_buffer, size);

					temp->image_param[i].regional_warping_raw_buffer = (char*)malloc(temp->image_param[i].mapW * temp->image_param[i].mapH * 4);
					memcpy(temp->image_param[i].regional_warping_raw_buffer, decoded_mat.data, temp->image_param[i].mapW * temp->image_param[i].mapH * 4);

					decoded_mat.release();
				}
			}
			else
			{
				// nothing...
			}
		}

		manager->stitcher->stitchingTemplate->mCameraParams->setData(temp);
		manager->is_undoRegionWarping = 2;
		manager->history_callback(QVS_MSG_HISTORY_UNDO);

		manager->isDoingUndoRegionWarping = 0;

		if (manager->calibration_done_callback != NULL)
			manager->calibration_done_callback(QVS_MSG_UNDOREDO_DONE);

		return true;
	}
	//

	//manager->isDoingAnimation = true;
	manager->isUndoRedo = true;
	manager->clearROI();						  //AdvancedUpdate ROI Clear
	for (int i = 0; i < manager->mNumCamera; i++) //AdvancedUpdate ROI Clear
		manager->mCalibrationIndex[i] = 1;

	/*if (manager->calibration_done_callback != NULL)
	manager->calibration_done_callback(QVS_MSG_UNDOREDO_START);*/

	int* idx = manager->input_manager->getOutIndex(1);
	manager->makeSeamMask(manager->input_manager->getTimeStamp(idx[0], 1), idx, MASK_FOR_EDIT);
	manager->setBeforeImageParam(manager->stitcher->stitchingTemplate->getImageParams());
	manager->setAfterImageParam(temp->image_param);

	printf("bigheadk, run undo1\n");
	manager->stitcher->stitchingTemplate->mCameraParams->setData(temp);
	printf("bigheadk, run undo2\n");
	//manager->is_request_change_param = UNDO_REDO_TRY;
	manager->is_request_change_param = ANIMATION_MODE;
	manager->history_callback(QVS_MSG_HISTORY_UNDO);
	manager->isDoingUndoRegionWarping = 0;
	printf("bigheadk, run undo end\n");
	return 0;
}

DWORD WINAPI EngineManager::run_redo(LPVOID handle) {
	printf("bigheadk, run run_redo\n");
	EngineManager *manager = (EngineManager *)handle;
	manager->isDoingAnimation = true;
	manager->checkWarpingMap(false);

	CameraParams* temp = manager->stitcher->stitchingTemplate->mCircularQueue->redo();
	if (temp == NULL || temp == 0)
	{
		manager->isDoingAnimation = false;
		return false;
	}

	if (manager->calibration_done_callback != NULL)
		manager->calibration_done_callback(QVS_MSG_UNDOREDO_START);

	// rw
	if (temp->mIsRegionalWarping == true)
	{
		for (int i = 0; i < manager->mNumCamera; i++)
		{
			if (temp->image_param[i].is_dirty == true)
			{
				if (temp->image_param[i].regional_warping_buffer == NULL)
				{
					//GLTexMask& tex = manager->stitcher->inputArray->camIn(i)->getTexMask();
					//if (tex.isMapSet() == 1)
					//	tex.doResetAnimation();

					//tex.resetMap();
					//manager->mSaveMapIndex[i] = false;

					//manager->warping_animTime = 0.0f;
					manager->mRegionalWarpingBeforeMapZeros[i] = 1;
				}
				else
				{
					manager->mRegionalWarpingUndoMapIndex[i] = 1;

					//PTSParam* image_param = manager->stitcher->stitchingTemplate->getImageParams();

					int size = strlen(temp->image_param[i].regional_warping_buffer);

					if (manager->base64ImageDecoder == NULL)
					{
						manager->base64ImageDecoder = new Base64ImageDecoder();
					}

					cv::Mat decoded_mat = manager->base64ImageDecoder->base64_decode_for_ROIMapSave(temp->image_param[i].regional_warping_buffer, size);

					temp->image_param[i].regional_warping_raw_buffer = (char*)malloc(temp->image_param[i].mapW * temp->image_param[i].mapH * 4);
					memcpy(temp->image_param[i].regional_warping_raw_buffer, decoded_mat.data, temp->image_param[i].mapW * temp->image_param[i].mapH * 4);

					decoded_mat.release();
				}
			}
			else
			{
				// nothing...
			}
		}

		manager->stitcher->stitchingTemplate->mCameraParams->setData(temp);
		manager->is_undoRegionWarping = 2;
		manager->history_callback(QVS_MSG_HISTORY_UNDO);

		manager->isDoingUndoRegionWarping = 0;

		if (manager->calibration_done_callback != NULL)
			manager->calibration_done_callback(QVS_MSG_UNDOREDO_DONE);

		return true;
	}
	//

	//manager->isDoingAnimation = true;
	manager->isUndoRedo = true;
	manager->clearROI();						  //AdvancedUpdate ROI Clear
	for (int i = 0; i < manager->mNumCamera; i++) //AdvancedUpdate ROI Clear
		manager->mCalibrationIndex[i] = 1;

	/*if (manager->calibration_done_callback != NULL)
	manager->calibration_done_callback(QVS_MSG_UNDOREDO_START);*/

	int* idx = manager->input_manager->getOutIndex(1);
	manager->makeSeamMask(manager->input_manager->getTimeStamp(idx[0], 1), idx, MASK_FOR_EDIT);
	manager->setBeforeImageParam(manager->stitcher->stitchingTemplate->getImageParams());
	manager->setAfterImageParam(temp->image_param);

	manager->stitcher->stitchingTemplate->mCameraParams->setData(temp);
	//manager->is_request_change_param = UNDO_REDO_TRY;
	manager->is_request_change_param = ANIMATION_MODE;
	manager->history_callback(QVS_MSG_HISTORY_UNDO);
	manager->isDoingUndoRegionWarping = 0;
	printf("bigheadk, run run_redo end\n");
	return 0;
}

void EngineManager::updateWarpMap()
{
	stitcher->updateWarpMap(stitcher->stitchingTemplate->mCameraParams->warp_info);

	printf("finish!!!!!!!!!!!!!!!!!!!!!!! updateWarpMap\n");
}

void EngineManager::setMultiStreaming(int isMultiStreaming)
{
	mIsMultiStreaming = isMultiStreaming;

	for (int i = 0; i < CAMERA_MAX; i++)
	{
		if (mVideoCaptureList[i].mVideoCapture_CPU)
		{
			if (mVideoCaptureList[i].mVideoCapture_CPU->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
				((MWVideoCapture*)mVideoCaptureList[i].mVideoCapture_CPU)->setMultiStreaming(mIsMultiStreaming);
		}
	}

	printf("bigheadk, setMultiStreaming!\n");

	is_need_save = true;

	printf("Magewell Multi-Streaming : %d\n", isMultiStreaming);
}

//added for engine
void EngineManager::setRGBIndex(int index)
{
	is_need_save = true;
	mRGBIndex = index;
}
int EngineManager::getRGBIndex()
{
	return mRGBIndex;
}
double EngineManager::getRed()
{
	return mRed[mRGBIndex];
}
double EngineManager::getGreen()
{
	return mGreen[mRGBIndex];
}
double EngineManager::getBlue()
{
	return mBlue[mRGBIndex];
}
void EngineManager::setGainChannelIndex(int index)
{
	printf("bigheadk, setGainChannelIndex!\n");
	is_need_save = true;
	mGainChannelIndex = index;
}
void EngineManager::setGainRGBIndex(int index)
{
	is_need_save = true;
	mGainRGBIndex = index;
}
void EngineManager::setGainPointIndex(int index)
{
	is_need_save = true;
	mGainPointIndex = index;
}
int EngineManager::getNumCameara()
{
	return mNumCamera;
}
int EngineManager::getNumInput()
{
	int ret = 0;
	for (int i = 0; i < mNumCamera; i++)
	{
		if (mVideoCaptureList[i].mVideoCapture != NULL)
		{
			ret++;
		}
	}
	return ret;
}
float EngineManager::getCapureFPS()
{
	return mCaptureFPS;
}
int EngineManager::getCapureWidth()
{
	return mCaptureWidth;
}
int EngineManager::getCapureHeight()
{
	return mCaptureHeight;
}

int EngineManager::getExposureMode()
{
	return mBlendingMode;
}
int EngineManager::getSeamMode()
{
	return mSeamMode;
}
int EngineManager::getGainMode()
{
	return mGainMode;
}
int EngineManager::getNadirMode()
{
	return mNadirMode;
}
int EngineManager::getNadirPosition()
{
	return mNadirPosition;
}
int EngineManager::getNadirSize()
{
	return mNadirSize;
}
char* EngineManager::getNadirPath()
{
	return mNadirPath;
}
void EngineManager::setNadirPath(const char* path)
{
	is_need_save = true;
	sprintf(mNadirPath, "%s", path);
}
void EngineManager::setStmap_L_Path(const char* path)
{
	is_need_save = true;
	sprintf(mStmap_L_Path, "%s", path);
}
void EngineManager::setStmap_R_Path(const char* path)
{
	is_need_save = true;
	sprintf(mStmap_R_Path, "%s", path);
}
char* EngineManager::getSnapshot_path()
{
	return snapshot_path;
}
int EngineManager::isEncoding()
{
	return ((StitchComposer*)(stitcher->composer))->isEncoding;
}
void EngineManager::setGainPointX1(double x)
{
	is_need_save = true;

	mGainPointX[mGainChannelIndex][mGainRGBIndex][0] = x;
}
void EngineManager::setGainPointX2(double x)
{
	is_need_save = true;

	mGainPointX[mGainChannelIndex][mGainRGBIndex][1] = x;
}
void EngineManager::setGainPointX3(double x)
{
	is_need_save = true;

	mGainPointX[mGainChannelIndex][mGainRGBIndex][2] = x;
}
void EngineManager::setGainPointY1(double y)
{
	is_need_save = true;

	mGainPointY[mGainChannelIndex][mGainRGBIndex][0] = y;
}
void EngineManager::setGainPointY2(double y)
{
	is_need_save = true;

	mGainPointY[mGainChannelIndex][mGainRGBIndex][1] = y;
}
void EngineManager::setGainPointY3(double y)
{
	is_need_save = true;

	mGainPointY[mGainChannelIndex][mGainRGBIndex][2] = y;
}
double EngineManager::getGainPointX1()
{
	return mGainPointX[mGainChannelIndex][mGainRGBIndex][0];
}
double EngineManager::getGainPointX2()
{
	return mGainPointX[mGainChannelIndex][mGainRGBIndex][1];
}
double EngineManager::getGainPointX3()
{
	return mGainPointX[mGainChannelIndex][mGainRGBIndex][2];
}
double EngineManager::getGainPointY1()
{
	return mGainPointY[mGainChannelIndex][mGainRGBIndex][0];
}
double EngineManager::getGainPointY2()
{
	return mGainPointY[mGainChannelIndex][mGainRGBIndex][1];
}
double EngineManager::getGainPointY3()
{
	return mGainPointY[mGainChannelIndex][mGainRGBIndex][2];
}
void* EngineManager::getGainCurveData()
{
	return mGainCurveData[mGainChannelIndex];
}
void EngineManager::setGainCurveData(void* data)
{
	is_need_save = true;

	mGainCurveData[mGainChannelIndex] = data;
}
int EngineManager::getAudioDevice()
{
	return mAudioDevice;
}
int EngineManager::getLensType()
{
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();
	return global_param->f;
}
float EngineManager::getFOV()
{
	PTSParam *global_param = stitcher->stitchingTemplate->getIGlobalParam();
	return global_param->v;
}

int EngineManager::getPanoramaSize()
{
	return mPanoramaSize;
}
int EngineManager::getBmOutputDevice()
{
	return mBMOutputDevice;
}
int EngineManager::getBmOutputDisplay()
{
	return mBMOutputDisplay;
}
char* EngineManager::dllTest()
{
	printf("EngineManager::dllTest!!\n");
	return "Quram Video360 Engine Linked Successfully";
}

int EngineManager::setROIMode(int mode)
{
	mROIMode = mode;
	//if (mROIMode == QVS_Engine::ROIMaskMode_Manual_On){
	//	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
	//	for (int i = 0; i < mNumCamera; i++){
	//			if (!image_param[i].sourcemask.empty() && image_param[i].sourcemask.data != NULL){
	//				int index = 0;
	//				cv::Mat tempMat;
	//				cv::resize(image_param[i].sourcemask, tempMat, cv::Size(image_param[i].sourcemask.cols / 2, image_param[i].sourcemask.rows / 2));
	//				//mRoi_Manual_buffer[i] = (unsigned char*)malloc(mCaptureWidth * mCaptureHeight * 4);
	//				for (int row = 0; row < tempMat.rows; row++){
	//					for (int col = 0; col < tempMat.cols; col++){
	//						bool check = false;
	//						for (int k = 0; k < 3; k++){
	//							uchar temp = mRoi_Manual_buffer[i][index++] = tempMat.at<cv::Vec3b>(row, col)[k];
	//							if (temp != 0) check = true;
	//						}
	//						if (check) mRoi_Manual_buffer[i][index++] = 0x3F;
	//						else mRoi_Manual_buffer[i][index++] = 0x00;
	//					}
	//				}
	//			}
	//	}
	//}
	return QVS_SUCCESS;
}
int EngineManager::drawManualPosition(int x, int y, int size)
{

	if (x >= 0) mPoint_x = x;
	if (y >= 0) mPoint_y = y;
	if (size >= 0) mSize = size;
	return QVS_SUCCESS;
}
int EngineManager::saveMask(int imgIndex)
{
	int repeat = mNumCamera;
	if (imgIndex >= 0){
		repeat = 1;
	}
	int index = 0;
	cv::Mat temp;
	std::vector<uchar> buff;
	std::vector<int> param = std::vector<int>(2);
	if (base64ImageDecoder == NULL)
	{
		base64ImageDecoder = new Base64ImageDecoder();
	}

	for (int i = 0; i < repeat; i++){
		if (imgIndex >= 0) index = imgIndex;
		else index = i;
		temp = cv::Mat(mMaskCaptureHeight, mMaskCaptureWidth, CV_8UC4);
		memcpy(temp.data, mRoi_Manual_buffer[index], mMaskCaptureHeight* mMaskCaptureWidth * 4);
		PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
		cv::resize(temp, temp, cv::Size(mCaptureWidth, mCaptureHeight));
		cv::cvtColor(temp, image_param[index].sourcemask, CV_BGRA2BGR);

		param[0] = CV_IMWRITE_PNG_COMPRESSION;
		param[1] = 4;
		cv::imencode(".png", temp, buff, param);

		size_t out_length = 4 * ((buff.size() + 2) / 3) + 1;
		if (image_param[i].source_buffer != NULL){
			free(image_param[i].source_buffer); image_param[i].source_buffer = NULL;
		}
		image_param[i].source_buffer = (char *)malloc(out_length);
		memset(image_param[i].source_buffer, 0x00, out_length);

		base64ImageDecoder->base64_encode((char*)(buff.data()), buff.size(), image_param[index].source_buffer, &out_length);
		image_param[index].sourcesize = out_length;
	}
	is_request_change_param = UNDO_REDO_MASK_DONE;

	param.clear();
	buff.clear();
	temp.release();

	/*char finename[1024];
	sprintf(finename, "test_mask_%d.jpg", imgIndex);
	cv::imwrite(finename, image_param[imgIndex].sourcemask);*/
	return QVS_SUCCESS;
}
int EngineManager::InitDrawManualROI()
{
	roi_x = 0; roi_y = 0;
	is_draw_ROI = 0;
	mIsManualMaskLeft = 0;

	return QVS_SUCCESS;
}
int EngineManager::calcManualMaskPoint2(bool isLeft, int x, int y)
{
	int imgIndex1, imgIndex2;
	PTSParam *image_param = stitcher->stitchingTemplate->getImageParams();
	mIsManualMaskLeft = isLeft ? 1 : 2;
	if (isLeft){
		imgIndex1 = mManual_calib_mask_left_index;
		imgIndex2 = mManual_calib_mask_right_index;
	}
	else{
		imgIndex1 = mManual_calib_mask_right_index;
		imgIndex2 = mManual_calib_mask_left_index;
	}
	double fov1 = image_param[imgIndex1].v;
	double fov2 = image_param[imgIndex2].v;
	double lensb1 = image_param[imgIndex1].b;
	double lensb2 = image_param[imgIndex2].b;
	double yaw1 = image_param[imgIndex1].y;
	double yaw2 = image_param[imgIndex2].y;
	double pitch1 = image_param[imgIndex1].p;
	double pitch2 = image_param[imgIndex2].p;
	double roll1 = image_param[imgIndex1].r;
	double roll2 = image_param[imgIndex2].r;
	double ret_x, ret_y;

	double type = image_param[imgIndex1].f;
	double out_type = image_param[imgIndex1].out_f;

	int pos_x, pos_y;
	calcScreenPosition(x, y, pos_x, pos_y, isLeft);

	if (pos_y <= 0 || pos_y >= mCaptureHeight || pos_x <= 0 || pos_x >= mCaptureWidth){
		InitDrawManualROI();
		return 0;
	}

	double ret_x2, ret_y2;
	QWarper warper0(fov1, 360, yaw1, pitch1, roll1, lensb1, mCaptureWidth, mCaptureHeight, output_width, output_height, type, out_type);
	QWarper warper1(fov2, 360, yaw2, pitch2, roll2, lensb2, mCaptureWidth, mCaptureHeight, output_width, output_height, type, out_type);

	double h2 = (double)mCaptureHeight / 2.0 - 0.5;
	double w2 = (double)mCaptureWidth / 2.0 - 0.5;
	warper0.Warping_Inverse((double)pos_x - w2, (double)pos_y - h2, &ret_x, &ret_y);  //warper0.Warping_ImageToEquirect((double)pos_x - w2, (double)pos_y - h2, &ret_x, &ret_y);
	h2 = (double)mCaptureHeight / 2.0 - 0.5;
	w2 = (double)mCaptureWidth / 2.0 - 0.5;
	warper1.Warping_((double)ret_x, (double)ret_y, &ret_x2, &ret_y2);  //warper1.Warping_EquirectToImage((double)ret_x, (double)ret_y, &ret_x2, &ret_y2);
	ret_x2 += w2;
	ret_y2 += h2;

	mPoint_x2 = mCaptureHeight - ret_y; mPoint_y2 = ret_x;
	if (pos_y <= 0 || pos_y >= mCaptureHeight || pos_x <= 0 || pos_x >= mCaptureWidth){

	}

	//QLOGD("POINT", "1 y(%lf), p(%lf) r(%lf)   2 y(%lf), p(%lf) r(%lf)", yaw1, pitch1, roll1, yaw2, pitch2, roll2);
	//QLOGD("POINT", "img(%d,%d), point(%d, %d) --> point(%d, %d)", imgIndex1, imgIndex2, _x, _y, mPoint_x2, mPoint_y2);
	return QVS_SUCCESS;
}
int EngineManager::checkLeftMask(int x, int y, int view_w, int view_h)
{
	if (x < view_w / 2) return 1; //left
	else return 0; //right
}
int EngineManager::calcScreenPosition(int x, int y, int &pos_x, int &pos_y, int isLeft){
	GLdouble modelview[16];
	GLdouble _pos_x, _pos_y, _pos_z;
	GLint viewPort[4];
	GLdouble projection[16];
	GLdouble __modelv[16];
	glGetIntegerv(GL_VIEWPORT, viewPort);

	for (int i = 0; i < 16; i++){
		modelview[i] = isLeft ? mModelview_Mat[0][i] : mModelview_Mat[1][i];
		if (i == 10) modelview[i] = 1;
	}
	GLdouble zCursor;
	glReadPixels((int)x, (int)viewPort[3] - y, 1, 1, GL_DEPTH_COMPONENT, GL_DOUBLE, &zCursor);
	if (gluUnProject((GLdouble)x, (GLdouble)viewPort[3] - (GLdouble)y, (GLdouble)zCursor, modelview, mProjection_Mat, viewPort, &_pos_x, &_pos_y, &_pos_z) == GLU_FALSE){

	}
	pos_x = (int)((_pos_x + 1.0)*(mMaskCaptureWidth / 2));
	pos_y = (int)((_pos_y + 1.0)*(mMaskCaptureHeight / 2));
	return QVS_SUCCESS;
}
int EngineManager::drawCircleMask(int centerX, int centerY, int r, int mode, int imgIndex, int imgColor)
{
	for (int yy = 0; yy <= r; yy++)
	{
		for (int xx = 0; xx <= r; xx++)
		{
			if (sqrt(xx*xx + yy*yy) < r)
			{
				int* buffer = (int*)mRoi_Manual_buffer[imgIndex];

				int rx = centerX + xx;
				int lx = centerX - xx;
				int ty = centerY + yy;
				int by = centerY - yy;
				if (rx < mMaskCaptureWidth)
				{
					if (ty < mMaskCaptureHeight)
					{
						buffer[rx + mMaskCaptureWidth * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? imgColor : QVS_Engine::ROIManualMaskColor_No;
					}
					if (by >= 0)
					{
						buffer[rx + mMaskCaptureWidth * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? imgColor : QVS_Engine::ROIManualMaskColor_No;
					}
				}
				if (lx >= 0)
				{
					if (ty < mMaskCaptureHeight)
					{
						buffer[lx + mMaskCaptureWidth * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? imgColor : QVS_Engine::ROIManualMaskColor_No;
					}
					if (by >= 0)
					{
						buffer[lx + mMaskCaptureWidth * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? imgColor : QVS_Engine::ROIManualMaskColor_No;
					}
				}
			}
		}
	}
	return QVS_SUCCESS;
}
int EngineManager::drawManualROI(int x, int y, int view_w, int view_h, int size, int mode, int isLeft)
{
	isLeft = checkLeftMask(x, y, view_w, view_h);

	int pos_x, pos_y;
	calcScreenPosition(x, y, pos_x, pos_y, isLeft);

	int imgIndex = 0, imgColor;
	cv::Scalar cv_imgColor;
	if (mManual_calib_mask_color == 0 && mode)
	{
		imgColor = QVS_Engine::ROIManualMaskColor_Red_Masking;
		cv_imgColor = cv::Scalar(0, 0, 255, 0x3F);
	}
	else if (mManual_calib_mask_color == 1 && mode){
		imgColor = QVS_Engine::ROIManualMaskColor_Green_Masking;
		cv_imgColor = cv::Scalar(0, 255, 0, 0x3F);
	}
	else if (mManual_calib_mask_color == 2 || !mode){
		imgColor = QVS_Engine::ROIManualMaskColor_No;
		cv_imgColor = cv::Scalar(0, 0, 0, 0);
	}
	if (isLeft) imgIndex = mManual_calib_mask_left_index;
	else imgIndex = mManual_calib_mask_right_index;

	if (mROIMode == QVS_Engine::ROIMaskMode_On)
		return 0;

	if (pos_y <= 0 || pos_y >= mMaskCaptureHeight || pos_x <= 0 || pos_x >= mMaskCaptureWidth){
		//QLOGD("TEST0", "y(%d), x(%d), pos_y(%d), pos_x(%d), mMaskCaptureHeight(%d), mMaskCaptureWidth(%d)", y, x,pos_y, pos_x, mMaskCaptureHeight, mMaskCaptureWidth);
		InitDrawManualROI();
		return 0;
	}
	cv::Mat src(mMaskCaptureHeight, mMaskCaptureWidth, CV_8UC4);
	src.data = mRoi_Manual_buffer[imgIndex];
	int r = size;

	double start_time = QClock();
	if (is_draw_ROI == 0)
	{
		is_draw_ROI = 1;
		roi_x = pos_x;
		roi_y = pos_y;
		/*cv::Rect rect(pos_x - r, pos_y - r, 2*r, 2*r);
		mMaskUpdateRect = rect;*/

		//QLOGD("RECT", "rect = %d, %d, %d, %d", mMaskUpdateRect.x, mMaskUpdateRect.y, mMaskUpdateRect.width, mMaskUpdateRect.height);
		cv::Point center(pos_x, pos_y);
		cv::circle(src, center, r, cv_imgColor, CV_FILLED); //drawCircleMask(centerX, centerY, r, mode, imgIndex, imgColor);
	}
	else
	{
		int dx = pos_x - roi_x;
		int dy = pos_y - roi_y;
		int x = roi_x;
		int y = roi_y;
		double degree = atan2((double)dy, (double)dx) * 180 / PI;
		cv::Point s_center(roi_x, roi_y);
		cv::Point e_center(pos_x, pos_y);
		cv::circle(src, s_center, r, cv_imgColor, CV_FILLED); //drawCircleMask(roi_x, roi_y, r, mode, imgIndex, imgColor);
		cv::circle(src, e_center, r, cv_imgColor, CV_FILLED); //drawCircleMask(pos_x, pos_y, r, mode, imgIndex, imgColor);
		double xx, yy;
		cv::Point rook_points[1][4];
		double dist_x, dist_y;
		if ((degree < 0 && degree > -90) || (degree > 90 && degree < 180)){
			dist_x = abs((r * sin(degree*PI / 180)));
			dist_y = abs((r * cos(degree*PI / 180)));

			xx = roi_x - dist_x;
			yy = roi_y - dist_y;
			rook_points[0][0] = cv::Point(xx, yy);
			xx = roi_x + dist_x;
			yy = roi_y + dist_y;
			rook_points[0][1] = cv::Point(xx, yy);
			xx = pos_x + dist_x;
			yy = pos_y + dist_y;
			rook_points[0][2] = cv::Point(xx, yy);
			xx = pos_x - dist_x;
			yy = pos_y - dist_y;
		}
		else{
			degree = 180 - degree;
			if (degree > 180) degree = degree -= 360;
			dist_x = abs((r * sin(degree*PI / 180)));
			dist_y = abs((r * cos(degree*PI / 180)));

			xx = roi_x + dist_x;
			yy = roi_y - dist_y;
			rook_points[0][0] = cv::Point(xx, yy);
			xx = roi_x - dist_x;
			yy = roi_y + dist_y;
			rook_points[0][1] = cv::Point(xx, yy);
			xx = pos_x - dist_x;
			yy = pos_y + dist_y;
			rook_points[0][2] = cv::Point(xx, yy);
			xx = pos_x + dist_x;
			yy = pos_y - dist_y;
			rook_points[0][3] = cv::Point(xx, yy);
		}
		rook_points[0][3] = cv::Point(xx, yy);
		const cv::Point* ppt[1] = { rook_points[0] };
		int npt[] = { 4 };
		//std::cout << rook_points[0][0] << " " << rook_points[0][1] << " " << rook_points[0][2] << " " << rook_points[0][3] <<  "dist " << dist_x << " " << dist_y << std::endl;
		cv::fillPoly(src, ppt, npt, 1, cv_imgColor, 8);
		src.release();

		roi_x = (int)pos_x;
		roi_y = (int)pos_y;
	}
	//std::cout << "====>drawManualROI time : " << QClock() - start_time << std::endl;
	return QVS_SUCCESS;
}
int EngineManager::drawROI(int x, int y, int size, int mode)
{
	if (mROIMode == QVS_Engine::ROIMaskMode_Off)
		return 0;

	if (is_draw_ROI == 0)
	{
		is_draw_ROI = 1;

		roi_x = x;
		roi_y = y;

		int centerX = x;
		int centerY = y;

		int r = size;
		for (int yy = 0; yy <= r; yy++)
		{
			for (int xx = 0; xx <= r; xx++)
			{
				if (sqrt(xx*xx + yy*yy) < r)
				{
					int* buffer = (int*)roi_buffer;

					int rx = centerX + xx;
					int lx = centerX - xx;
					int ty = centerY + yy;
					int by = centerY - yy;

					if (rx < output_width)
					{
						if (ty < output_height)
						{
							buffer[rx + output_width * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
						}
						if (by >= 0)
						{
							buffer[rx + output_width * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
						}
					}
					if (lx >= 0)
					{
						if (ty < output_height)
						{
							buffer[lx + output_width * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
						}
						if (by >= 0)
						{
							buffer[lx + output_width * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
						}
					}
				}
			}
		}
	}
	else
	{
		int addX, addY;
		int counter = 0;

		int dx = x - roi_x;
		int dy = y - roi_y;

		// X Direction : -
		if (dx < 0) {
			addX = -1;
			dx = -dx;
		}
		// X Direction : +
		else {
			addX = 1;
		}

		// Y Direction : -
		if (dy < 0) {
			addY = -1;
			dy = -dy;
		}
		// Y Direction : +
		else {
			addY = 1;
		}

		int x = roi_x;
		int y = roi_y;

		if (dx >= dy) {
			for (int i = 0; i < dx; i++) {
				x += addX;
				counter += dy;

				if (counter >= dx) {
					y += addY;
					counter -= dx;
				}

				int centerX = x;
				int centerY = y;

				int r = size;

				for (int yy = 0; yy <= r; yy++)
				{
					for (int xx = 0; xx <= r; xx++)
					{
						if (sqrt(xx*xx + yy*yy) < r)
						{
							int* buffer = (int*)roi_buffer;

							int rx = centerX + xx;
							int lx = centerX - xx;
							int ty = centerY + yy;
							int by = centerY - yy;

							if (rx < output_width)
							{
								if (ty < output_height)
								{
									buffer[rx + output_width * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
								if (by >= 0)
								{
									buffer[rx + output_width * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
							}
							if (lx >= 0)
							{
								if (ty < output_height)
								{
									buffer[lx + output_width * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
								if (by >= 0)
								{
									buffer[lx + output_width * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
							}
						}
					}
				}
			}
		}
		else {
			for (int i = 0; i < dy; i++) {
				y += addY;
				counter += dx;

				if (counter >= dy) {
					x += addX;
					counter -= dy;
				}

				int centerX = x;
				int centerY = y;

				int r = size;

				for (int yy = 0; yy <= r; yy++)
				{
					for (int xx = 0; xx <= r; xx++)
					{
						if (sqrt(xx*xx + yy*yy) < r)
						{
							int* buffer = (int*)roi_buffer;

							int rx = centerX + xx;
							int lx = centerX - xx;
							int ty = centerY + yy;
							int by = centerY - yy;

							if (rx < output_width)
							{
								if (ty < output_height)
								{
									buffer[rx + output_width * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
								if (by >= 0)
								{
									buffer[rx + output_width * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
							}
							if (lx >= 0)
							{
								if (ty < output_height)
								{
									buffer[lx + output_width * ty] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
								if (by >= 0)
								{
									buffer[lx + output_width * by] = mode == QVS_Engine::ROIMaskDrawMode_Draw ? QVS_Engine::ROIMaskColor_Masking : QVS_Engine::ROIMaskColor_No;
								}
							}
						}
					}
				}
			}
		}

		roi_x = x;
		roi_y = y;
	}
	return QVS_SUCCESS;
}

int EngineManager::drawROIFinish()
{
	is_draw_ROI = 0;

	return QVS_SUCCESS;
}

int EngineManager::clearROI()
{
	//roi_buffer = (unsigned char*)malloc(output_width * output_height * 4);
	if (roi_buffer != NULL)
		memset(roi_buffer, 0x00, output_width * output_height * 4);
	return 1;
}
int EngineManager::clearManualMaskROI(bool isAll)
{
	if (isAll){
		for (int i = 0; i < mNumCamera; i++){
			if (mRoi_Manual_buffer[i] != NULL)
				memset(mRoi_Manual_buffer[i], 0x00, mMaskCaptureWidth * mMaskCaptureHeight * 4);
		}
		mMaskUpdateRect.x = 0; mMaskUpdateRect.y = 0; mMaskUpdateRect.width = mMaskCaptureWidth; mMaskUpdateRect.height = mMaskCaptureHeight;
	}
	else{
		if (mRoi_Manual_buffer[mManual_calib_mask_left_index] != NULL)
			memset(mRoi_Manual_buffer[mManual_calib_mask_left_index], 0x00, mMaskCaptureWidth * mMaskCaptureHeight * 4);
		if (mRoi_Manual_buffer[mManual_calib_mask_right_index] != NULL)
			memset(mRoi_Manual_buffer[mManual_calib_mask_right_index], 0x00, mMaskCaptureWidth * mMaskCaptureHeight * 4);
		mMaskUpdateRect.x = 0; mMaskUpdateRect.y = 0; mMaskUpdateRect.width = mMaskCaptureWidth; mMaskUpdateRect.height = mMaskCaptureHeight;
	}

	return 1;
}
int EngineManager::setSelectMode(int mode)
{
	mSelectMode = mode;

	return 1;
}

int EngineManager::selectROI(int x, int y, int &isSelected)
{
	for (int i = 0; i < mNumCamera; i++)
	{
		//float center_x = (warp_info[i].roi.x + warp_info[i].roi.width / 2.0f);
		//float center_y = (warp_info[i].roi.y + warp_info[i].roi.height / 2.0f);
		float center_x = mCenter_x[i];// (warp_info[i].center_point.x);
		float center_y = mCenter_y[i];// (warp_info[i].center_point.y);
		if (center_x < 0)
			center_x += output_width;
		if (center_y < 0)
			center_y += output_height;

		if (x > center_x - mSelectSize / 2 && x < center_x + mSelectSize / 2 && y > center_y - mSelectSize / 2 && y < center_y + mSelectSize / 2)
		{
			isSelected = 1;
			if (mCalibrationIndex[i] == 0)
				mCalibrationIndex[i] = 1;
			else if (mCalibrationIndex[i] == 1)
				mCalibrationIndex[i] = 0;
		}
	}

	return 1;
}

int EngineManager::selectAllROI()
{
	int count = 0;
	for (int i = 0; i < mNumCamera; i++)
	{
		if (mCalibrationIndex[i] == 1)
			count++;
	}

	if (count == mNumCamera)
	{
		for (int i = 0; i < mNumCamera; i++)
		{
			mCalibrationIndex[i] = 0;
		}
	}
	else
	{
		for (int i = 0; i < mNumCamera; i++)
		{
			mCalibrationIndex[i] = 1;
		}
	}

	return 1;
}

int EngineManager::moveOutput(float x, float y, int isRoll, int isReverseX, int isReverseY)
{
	if (isRoll) {
		cal_ro_x = 0;
		cal_ro_y = 0;
		float roll_x = (x*fov * PI) / 180;
		float roll_y = -(y*fov * PI) / 180;
		if (isReverseY)
			roll_x = -roll_x;
		if (isReverseX)
			roll_y = -roll_y;
		float z = (roll_x + roll_y) / 2;
		cal_ro_z = (float)(z*fov * PI) / 180;
	}
	else {
		cal_ro_x = (float)(x*fov * PI) / 180;
		cal_ro_y = -(float)(y*fov * PI) / 180;
		cal_ro_z = 0.f;
	}
	cal_ro_x = cal_ro_x * 4;
	cal_ro_y = cal_ro_y * 4;
	cal_ro_z = cal_ro_z * 2;
	//printf("moveOutput fov=%f, cal_ro_x=%f, cal_ro_y=%f, cal_ro_z=%f\n", fov, cal_ro_x, cal_ro_y, cal_ro_z);

	return 1;
}
int EngineManager::zoomOverlay(float scale)
{
	cal_fov = scale;
	return QVS_SUCCESS;
}
bool EngineManager::editOutput()
{
	//checkWarpingMap();
	mEditOutputThread = CreateThread(NULL, 0, run_edit_mode, this, 0, NULL);
	return true;
}

void EngineManager::editOff()
{
	is_editmode = false;
}
bool EngineManager::isEdit()
{
	return is_editmode;
}

void EngineManager::saveOutput()
{
	is_request_change_param = STITCHING_TEMPLATE_REFRESH;
}
void EngineManager::setIsRedraw(int isRedraw)
{
	is_Redraw = isRedraw;
}
int EngineManager::getIsRedraw()
{
	return is_Redraw;
}
void* EngineManager::getOutputCurveData()
{
	return mOutputCurveData;
}
void EngineManager::setOutputCurveData(void* data)
{
	mOutputCurveData = data;
}

int EngineManager::savePanorama(char* file_path)
{
	//straighten_Image(image_param, mNumCamera);
	stitcher->savePanoramaImage(file_path);
	return 1;
}
int EngineManager::getLensTypeCount()
{
	return LENS_TYPE_THE_NUMBER;
}
char* EngineManager::getLensTypeName(int index)
{
	char *lens_name;
	lens_name = (char*)malloc(MAX_PATH);
	if (index == 0)
		sprintf(lens_name, "Rectilinear");
	else if (index == 1)
		sprintf(lens_name, "Cylindrical panorama");
	else if (index == 2)
		sprintf(lens_name, "Circular Fisheye");
	else if (index == 3)
		sprintf(lens_name, "FullFrame Fisheye");
	return lens_name;
}
bool EngineManager::undo()
{
	if (isDoingUndoRegionWarping == 1)
		return false;

	isDoingUndoRegionWarping = 1;
	mUndoThread = CreateThread(NULL, 0, run_undo, this, 0, NULL);
	return true;
}
bool EngineManager::redo()
{
	if (isDoingUndoRegionWarping == 1)
		return false;

	isDoingUndoRegionWarping = 1;
	mRedoThread = CreateThread(NULL, 0, run_redo, this, 0, NULL);
	return true;
}
bool EngineManager::undoEnabled()
{
	return stitcher->stitchingTemplate->mCircularQueue->undoEnabled();
}
bool EngineManager::redoEnabled()
{
	return stitcher->stitchingTemplate->mCircularQueue->redoEnabled();
}

int EngineManager::getOutputWidth()
{
	return output_width;
}
int EngineManager::getOutputHeight()
{
	return output_height;
}
int EngineManager::getCaptureWidth()
{
	return mCaptureWidth;
}
int EngineManager::getCaptureHeight()
{
	return mCaptureHeight;
}
void EngineManager::setDrawMaskLeftIndex(int index)
{
	//mImageLeftSelectAnimationStart = true;
	mManual_calib_mask_left_index = index;
}
void EngineManager::setDrawMaskRightIndex(int index)
{
	//mImageRightSelectAnimationStart = true;
	mManual_calib_mask_right_index = index;
}
int EngineManager::getDrawMaskLeftIndex()
{
	return mManual_calib_mask_left_index;
}
int EngineManager::getDrawMaskRightIndex()
{
	return mManual_calib_mask_right_index;
}
void EngineManager::setDrawMaskColor(int index)
{
	mManual_calib_mask_color = index;
}
void EngineManager::rotateManualMaskLeft(int direction)
{
	mRotateManualMask_Left_angle += 90;
	if (mRotateManualMask_Left_angle >= 360) mRotateManualMask_Left_angle = 0;
}
void EngineManager::rotateManualMaskRight(int direction)
{
	mRotateManualMask_Right_angle += 90;
	if (mRotateManualMask_Right_angle >= 360) mRotateManualMask_Right_angle = 0;
}
int EngineManager::getSelectedImage()
{
	int ret = 0;
	for (int i = 0; i < mNumCamera; i++) {
		if (mCalibrationIndex[i] == 1) {
			ret++;
		}
	}
	return ret;
}
void EngineManager::setCallbackSnapshot(callback_snapshot cb)
{
	snapshot_done_callback = cb;
}
void EngineManager::setCallbackDoManaulGainMode(callback_doManualGainMode cb)
{
	doManualGainMode_done_callback = cb;
}
void EngineManager::setCallbackQuramCalibMode(callback_quramCalibMode cb)
{
	quramCalibMode_done_callback = cb;
}
void EngineManager::setCallbackGainMode(callback_gainMode cb)
{
	gainMode_done_callback = cb;
}
void EngineManager::setCallbackSeamMode(callback_seamMode cb)
{
	seamMode_done_callback = cb;
}
void EngineManager::setCallbackAutoGainMode(callback_autoGainMode cb)
{
	autoGainMode_done_callback = cb;
}
void EngineManager::setCallbackEditOutput(callback_autoGainMode cb)
{
	editOutput_done_callback = cb;
}
void EngineManager::setCallbackCalibration(callback_calibration cb)
{
	calibration_done_callback = cb;
}
void EngineManager::setCallbackDoMakeWarpmap(callback_doMakeWarpmap cb)
{
	makeWapmap_callback = cb;
}
void EngineManager::setCallbackHistory(callback_history cb)
{
	history_callback = cb;
}
void EngineManager::setCallbackAudioError(callback_audioError cb)
{
	audioError_callback = cb;
}
void EngineManager::setCallbackRegionWarping(callback_regionWarping cb)
{
	regionWarping_done_callback = cb;
}
void EngineManager::setCallbackStereoCalibration(callback_stereoCalibration cb)
{
	stereoCalibration_done_callback = cb;
}
void EngineManager::setCallbackLoad(callback_load cb)
{
	load_callback = cb;
}

void EngineManager::calculateInteractiveCoordinates()
{
	const int longitude = 180, latitude = 361;

	texCoordS = (float*)malloc(latitude * sizeof(float));
	texCoordT = (float*)malloc(longitude * sizeof(float));
	texCoordT2 = (float*)malloc(longitude * sizeof(float));

	coordY1 = (float*)malloc(longitude * sizeof(float));
	coordY2 = (float*)malloc(longitude * sizeof(float));

	memset(texCoordS, 0, latitude);
	memset(texCoordT, 0, longitude);
	memset(texCoordT2, 0, longitude);

	memset(coordY1, 0, longitude);
	memset(coordY2, 0, longitude);

	coordX1 = (float**)malloc(longitude * sizeof(float*));
	coordX2 = (float**)malloc(longitude * sizeof(float*));
	coordZ1 = (float**)malloc(longitude * sizeof(float*));
	coordZ2 = (float**)malloc(longitude * sizeof(float*));
	for (int i = 0; i < longitude; i++)
	{
		coordX1[i] = (float*)malloc(latitude * sizeof(float));
		coordX2[i] = (float*)malloc(latitude * sizeof(float));
		coordZ1[i] = (float*)malloc(latitude * sizeof(float));
		coordZ2[i] = (float*)malloc(latitude * sizeof(float));

		memset(coordX1[i], 0, latitude);
		memset(coordX2[i], 0, latitude);
		memset(coordZ1[i], 0, latitude);
		memset(coordZ2[i], 0, latitude);
	}

	float R = 1;
	bool texCoordSDone = false;

	for (int lon = -90; lon < 90; lon += 3) {
		float r1 = R*cos(lon / 180.0*3.14159265358979);
		float r2 = R*cos((lon + 3) / 180.0*3.14159265358979);
		coordY1[lon + 90] = R*sin(lon / 180.0*3.14159265358979);
		coordY2[lon + 90] = R*sin((lon + 3) / 180.0*3.14159265358979);

		texCoordT[lon + 90] = (lon + 90) / 180.0;
		texCoordT2[lon + 90] = (lon + 93) / 180.0;

		for (int lat = 0; lat <= 360; lat += 3) {
			coordZ1[lon + 90][lat] = r1*cos(lat / 180.0*3.14159265358979);
			coordX1[lon + 90][lat] = r1*sin(lat / 180.0*3.14159265358979);
			coordZ2[lon + 90][lat] = r2*cos(lat / 180.0*3.14159265358979);
			coordX2[lon + 90][lat] = r2*sin(lat / 180.0*3.14159265358979);

			if (!texCoordSDone)
				texCoordS[lat] = lat / 360.0;

		}
		texCoordSDone = true;
	}
}
void EngineManager::calculateInteractiveCoordinatesForWidePanorama()
{
	const int m_rings = 360;
	float pi = 3.141592654f;
	float angleIncrement = (1.0f * pi) / static_cast<float>(m_rings);
	float textureCoordinateIncrement = 1.0f / static_cast<float>(m_rings);

	texCoordS_side = (float*)malloc(m_rings * sizeof(float));
	texCoordT_side = (float*)malloc(m_rings * sizeof(float));
	texCoordT2_side = (float*)malloc(m_rings * sizeof(float));

	coordX1_side = (float*)malloc(m_rings * sizeof(float));
	coordX2_side = (float*)malloc(m_rings * sizeof(float));
	coordY1_side = (float*)malloc(m_rings * sizeof(float));
	coordY2_side = (float*)malloc(m_rings * sizeof(float));
	coordZ1_side = (float*)malloc(m_rings * sizeof(float));
	coordZ2_side = (float*)malloc(m_rings * sizeof(float));

	memset(texCoordS_side, 0, m_rings);
	memset(texCoordT_side, 0, m_rings);
	memset(texCoordT2_side, 0, m_rings);

	memset(coordX1_side, 0, m_rings);
	memset(coordX2_side, 0, m_rings);
	memset(coordY1_side, 0, m_rings);
	memset(coordY2_side, 0, m_rings);
	memset(coordZ1_side, 0, m_rings);
	memset(coordZ2_side, 0, m_rings);

	for (int j = 0; j <= m_rings; j += 3) {
		float c = cos(angleIncrement * j);
		float s = sin(angleIncrement * j);

		coordX1_side[j] = c;
		coordX2_side[j] = c;
		coordY1_side[j] = 0.5f;
		coordY2_side[j] = -0.5f;
		coordZ1_side[j] = s;
		coordZ2_side[j] = s;

		texCoordS_side[j] = textureCoordinateIncrement * j;
		texCoordT_side[j] = 0;
		texCoordT2_side[j] = 1.0f;
	}

}
void EngineManager::destroyInteractiveCoordinates()
{
	//printf("JW destroy InteractiveCoordinates111\n");
	if (texCoordS != NULL)
		free(texCoordS);
	if (texCoordT != NULL)
		free(texCoordT);
	if (texCoordT2 != NULL)
		free(texCoordT2);
	if (coordY1 != NULL)
		free(coordY1);
	if (coordY2 != NULL)
		free(coordY2);

	texCoordS = NULL;
	texCoordT = NULL;
	texCoordT2 = NULL;
	coordY1 = NULL;
	coordY2 = NULL;

	if (coordX1 != NULL && coordZ1 != NULL && coordX2 != NULL && coordZ2 != NULL)
	{
		for (int i = 0; i < 180; i++)
		{
			free(coordX1[i]);
			free(coordZ1[i]);
			free(coordX2[i]);
			free(coordZ2[i]);
		}
		free(coordX1);
		free(coordZ1);
		free(coordX2);
		free(coordZ2);
	}

	coordX1 = NULL;
	coordZ1 = NULL;
	coordX2 = NULL;
	coordZ2 = NULL;

	//printf("JW destroy InteractiveCoordinates done\n");
}
void EngineManager::destroyInputColors()
{
	((StitchComposer*)stitcher->composer)->destroyInputColors();
}
void EngineManager::checkWarpingMap(bool showProgToast)
{
	if (makeWapmap_callback != NULL)
		makeWapmap_callback(QVS_MSG_LOADING_WARPMAP);

	for (int i = 0; i < mNumCamera; i++)
	{
		if (stitcher->stitchingTemplate->mCameraParams->warp_info[i].data_state == INVALID_DATA)
		{
			while (1){
				if (stitcher->stitchingTemplate->mCameraParams->warp_info[i].data_state == INVALID_DATA) Sleep(WAIT_WARP_TIME);
				else break;
			}
			//return QVS_FAIL;
		}
	}

	while (is_doing_exposure || is_doing_seam)
	{
		Sleep(WAIT_WARP_TIME);
	}

	if (showProgToast && makeWapmap_callback != NULL)
		makeWapmap_callback(QVS_MSG_LOADING_END_WARPMAP);
}
int EngineManager::checkInitParam(){
	int check = 0;
	for (int i = 0; i < mNumCamera; i++)
	{
		if (stitcher->stitchingTemplate->mCameraParams->image_param[i].y == 0 &&
			stitcher->stitchingTemplate->mCameraParams->image_param[i].p == 0 &&
			stitcher->stitchingTemplate->mCameraParams->image_param[i].r == 0) check = 1;
		else return FAIL;
	}
	return check;
}
void EngineManager::FileWriter(int w, int h, int channel, unsigned char* buffer)
{
	cv::Mat temp;
	if (channel == 4)
		temp = cv::Mat(h, w, CV_8UC4, buffer);
	else if (channel == 3)
		temp = cv::Mat(h, w, CV_8UC3, buffer);

	//cv::cvtColor(temp, temp, CV_RGBA2BGRA);// CV_RGBA2BGRA);
	flip(temp, temp, 0);
	cv::imwrite("00_OverlayText.png", temp);
}


int EngineManager::setEditInputMode(int mode)
{
	mEditInputMode = mode;

	return QVS_SUCCESS;
}

int EngineManager::onSize(int w, int h)
{
	mViewW = w;
	mViewH = h;

	float viewW = w / (float)(mNumVerticalLine + 1);
	float viewH = h / (float)(mNumHorizontalLine + 1);

	for (int i = 0; i < mNumCamera; i++)
	{
		if (mSourceTextures[i] != NULL)
		{
			mSourceTextures[i]->setViewOffset(i % (mNumVerticalLine + 1) * viewW, ((mNumHorizontalLine)-i / (mNumVerticalLine + 1)) * viewH);
			mSourceTextures[i]->setViewSize(round(viewW), round(viewH));
		}
	}

	setViewSize(w, h);

	return QVS_SUCCESS;
}

int EngineManager::onDown(int x, int y)
{
	if (mEditInputMode == QVS_Engine::EditInputMode_On)
	{
		for (int i = 0; i < mNumCamera; i++)
		{
			int viewX = mSourceTextures[i]->getViewX();
			int viewY = mSourceTextures[i]->getViewY();
			int viewW = mSourceTextures[i]->getViewW();
			int viewH = mSourceTextures[i]->getViewH();
			if (x > viewX && x < viewX + viewW && mViewH - y > viewY && mViewH - y < viewY + viewH)
			{
				mEditInputTexture = mSourceTextures[i];
				mEditInputTextureIndex = i;

				float oldScaleMin = mEditInputTexture->getMinimalScale(mEditInputTexture->getImageW(), mEditInputTexture->getImageH());
				int oldViewX = mEditInputTexture->getViewX();
				int oldViewY = mEditInputTexture->getViewY();
				int oldViewW = mEditInputTexture->getViewW();
				int oldViewH = mEditInputTexture->getViewH();

				mEditInputTexture->setViewOffset(0, 0);
				mEditInputTexture->setViewSize(mViewW, mViewH);

				float curScaleMin = mEditInputTexture->getMinimalScale(mEditInputTexture->getImageW(), mEditInputTexture->getImageH());
				float ratio = oldScaleMin / curScaleMin;

				mEditInputTexture->setCurrentScale(mEditInputTexture->getCurrentScale() * ratio);

				float setX = mEditInputTexture->getCurrentX() + (mViewW / 2.0f - (oldViewX + oldViewW / 2.0f)) / mEditInputTexture->getCurrentScale();
				float setY = mEditInputTexture->getCurrentY() - (mViewH / 2.0f - (oldViewY + oldViewH / 2.0f)) / mEditInputTexture->getCurrentScale();

				mEditInputTexture->setCurrentX(setX);
				mEditInputTexture->setCurrentY(setY);

				mEditInputTexture->setAlpha(0.7);

				return QVS_SUCCESS;
			}
		}
	}

	return QVS_FAIL;
}

int EngineManager::onUp(int x, int y)
{
	if (mEditInputMode == QVS_Engine::EditInputMode_On)
	{
		float viewW = mViewW / (float)(mNumVerticalLine + 1);
		float viewH = mViewH / (float)(mNumHorizontalLine + 1);

		int fromIndex = mEditInputTextureIndex;
		int toIndex = mEditInputTextureIndex;
		for (int i = 0; i < mNumCamera; i++)
		{
			int viewX = mSourceTextures[i]->getViewX();
			int viewY = mSourceTextures[i]->getViewY();
			int viewW = mSourceTextures[i]->getViewW();
			int viewH = mSourceTextures[i]->getViewH();
			if (x > viewX && x < viewX + viewW && mViewH - y > viewY && mViewH - y < viewY + viewH && mEditInputTextureIndex != i)
			{
				toIndex = i;
			}
		}

		if (mEditInputTexture != NULL)
		{
			mEditInputTexture->setViewOffset(mEditInputTextureIndex % (mNumVerticalLine + 1) * viewW, ((mNumHorizontalLine)-mEditInputTextureIndex / (mNumVerticalLine + 1)) * viewH);
			mEditInputTexture->setViewSize(round(viewW), round(viewH));

			mEditInputTexture->setAlpha(1.0);

			mEditInputTexture = NULL;
		}

		if (mEditInputTextureIndex != toIndex)
		{
			printf("%d <-> %d\n", mEditInputTextureIndex, toIndex);
			setCaptureDeviceIndex(mEditInputTextureIndex, mVideoCaptureList[toIndex].mCaptureDeviceIndex);
		}
	}

	return QVS_SUCCESS;
}

int EngineManager::onScroll(int dx, int dy)
{
	if (mEditInputMode == QVS_Engine::EditInputMode_On)
	{
		if (mEditInputTexture != NULL)
		{
			mEditInputTexture->startScroll(dx, dy);
		}
	}

	return QVS_SUCCESS;
}

void EngineManager::convertInput2Output(int num, int x, int y, double point[2]) {
	PTSParam *image_params = stitcher->stitchingTemplate->getImageParams();
	//PTSParam image_param = image_params[num];

	int left = image_params[num].left;
	int top = image_params[num].top;
	int right = image_params[num].right;
	int bottom = image_params[num].bottom;

	double	w2, h2;
	//struct 	QMakeParams		mp;
	//struct  QfDesc        	stack[15];
	double	sphere_fov = 360.0;

	double xx = x - left;
	double yy = y - top;

	int sphere_width = output_width;
	int sphere_height = output_height;

	CamInput* input = stitcher->inputArray->cams[num];

	int inputWidth = right - left;// input->width;
	int inputHeight = bottom - top;// input->height;
	double _fov, _lensb, _yaw, _pitch, _roll;

	_fov = input->fov;
	_lensb = input->dist[1];
	_yaw = input->tr[0];
	_pitch = input->tr[1];
	_roll = input->tr[2];

	//QSetInvMakeParams(stack, &mp, _fov, _yaw, _pitch, _roll, _lensb, inputWidth, inputHeight, sphere_width, sphere_height);
	QWarper warper0(_fov, 360, _yaw, _pitch, _roll, _lensb, inputWidth, inputHeight, sphere_width, sphere_height, input->lens_type, input->output_type);
	h2 = (double)inputHeight / 2.0 - 0.5;
	w2 = (double)inputWidth / 2.0 - 0.5;
	//Qexecute_stack_new((double)xx - w2, (double)yy - h2, &point[0], &point[1], stack);
	warper0.Warping_Inverse((double)xx - w2, (double)yy - h2, &point[0], &point[1]);
	point[0] += (double)sphere_width / 2.0 - 0.5;
	point[1] += (double)sphere_height / 2.0 - 0.5;
}

void EngineManager::convertOutput2Input(int num, int x, int y, double point[2])
{
	PTSParam *image_params = stitcher->stitchingTemplate->getImageParams();
	//PTSParam image_param = image_params[num];

	int left = image_params[num].left;
	int top = image_params[num].top;
	int right = image_params[num].right;
	int bottom = image_params[num].bottom;

	double	w2, h2;
	//struct 	QMakeParams		mp;
	//struct  QfDesc        	stack[15];
	double	sphere_fov = 360.0;

	double xx = x;
	double yy = y;

	int sphere_width = output_width;
	int sphere_height = output_height;

	CamInput* input = stitcher->inputArray->cams[num];

	int inputWidth = right - left;// input->width;
	int inputHeight = bottom - top;// input->height;
	double _fov, _lensb, _yaw, _pitch, _roll;

	_fov = input->fov;
	_lensb = input->dist[1];
	_yaw = input->tr[0];
	_pitch = input->tr[1];
	_roll = input->tr[2];

	//QSetMakeParams(stack, &mp, _fov, _yaw, _pitch, _roll, _lensb, inputWidth, inputHeight, sphere_width, sphere_height);
	QWarper warper1(_fov, 360, _yaw, _pitch, _roll, _lensb, inputWidth, inputHeight, sphere_width, sphere_height, input->lens_type, input->output_type);
	h2 = (double)sphere_height / 2.0 - 0.5;
	w2 = (double)sphere_width / 2.0 - 0.5;
	//Qexecute_stack_new(xx - w2, yy - h2, &point[0], &point[1], stack);
	warper1.Warping_(xx - w2, yy - h2, &point[0], &point[1]);
	point[0] += (double)inputWidth / 2.0 - 0.5;
	point[1] += (double)inputHeight / 2.0 - 0.5;

	point[0] += left;
	point[1] += top;
}

int EngineManager::zoomIn(float s, int x, int y)
{
	//if (mCurrentScale + s <= 3 && mCurrentScale + s >= 1)
	//{
	float scale_fix = output_width / output_height / 2.0f;
	float margin_fix = output_height - (output_height / scale_fix);

	float preX = mCurrentX;
	float preY = mCurrentY;

	mFocusX = mCurrentX + (x - mViewW / 2.0f) / mCurrentScale;
	mFocusY = mCurrentY + (y - mViewH / 2.0f) / mCurrentScale;

	mCurrentScale *= s;

	if (mCurrentScale >= 10)
		mCurrentScale = 10;
	if (mCurrentScale <= 1)
		mCurrentScale = 1;

	float tempX = mFocusX - (x - mViewW / 2.0f) / mCurrentScale;
	float tempY = mFocusY - (y - mViewH / 2.0f) / mCurrentScale;

	mBoundLeft = mViewW / (2.0f * mCurrentScale);
	mBoundRight = mViewW - mBoundLeft;
	mBoundTop = mViewH / (2.0f * mCurrentScale);
	mBoundBottom = mViewH - mBoundTop;

	// If the scaled height is smaller than the view height,
	// force it to be in the center.
	if (mViewH * mCurrentScale <= mViewH) {
		mBoundTop = mBoundBottom = mViewH / 2.0f;
	}

	// Same for width
	if (mViewW * mCurrentScale <= mViewW) {
		mBoundLeft = mBoundRight = mViewW / 2.0f;
	}

	if (tempX < mBoundLeft)
	{
		tempX = mBoundLeft;
	}
	else if (tempX > mBoundRight)
	{
		tempX = mBoundRight;
	}

	if (tempY < mBoundTop)
	{
		tempY = mBoundTop;
	}
	else if (tempY > mBoundBottom)
	{
		tempY = mBoundBottom;
	}

	mCurrentX = tempX;
	mCurrentY = tempY;
	isDoingEvent = 1;

	return QVS_SUCCESS;
	//}
	//else
	//{
	//	return QVS_FAIL;
	//}
}

int EngineManager::zoomOut(float s, int x, int y)
{
	if (mCurrentScale > 1)
	{
		//mZoomX = x;
		//mZoomY = y;
		mCurrentX = mViewW / 2.0f;
		mCurrentY = mViewH / 2.0f;
		mCurrentScale -= 2;
		return QVS_SUCCESS;
	}
	else
	{
		return QVS_FAIL;
	}
}

int EngineManager::ScaleOutput(float s, int x, int y)
{
	mRotateScale = s;
	isDoingEvent = 1;

	return QVS_SUCCESS;
}

int EngineManager::resetToFullView()
{
	mCurrentX = mViewW / 2.0f;
	mCurrentY = mViewH / 2.0f;
	mCurrentScale = 1;

	return QVS_SUCCESS;
}

int EngineManager::scroll(int dx, int dy, int w, int h)
{
	float x = mCurrentX + dx / mCurrentScale;
	float y = mCurrentY + dy / mCurrentScale;

	mBoundLeft = w / (2.0f * mCurrentScale);
	mBoundRight = w - mBoundLeft;
	mBoundTop = h / (2.0f * mCurrentScale);
	mBoundBottom = h - mBoundTop;

	//printf("LRTB(%f,%f,%f,%f) xy(%f,%f)\n", mBoundLeft, mBoundRight, mBoundTop, mBoundBottom, x, y);

	// If the scaled height is smaller than the view height,
	// force it to be in the center.
	if (h * mCurrentScale <= h) {
		mBoundTop = mBoundBottom = h / 2.0f;
	}

	// Same for width
	if (w * mCurrentScale <= w) {
		mBoundLeft = mBoundRight = w / 2.0f;
	}

	if (x < mBoundLeft)
	{
		x = mBoundLeft;
	}
	else if (x > mBoundRight)
	{
		x = mBoundRight;
	}

	if (y < mBoundTop)
	{
		y = mBoundTop;
	}
	else if (y > mBoundBottom)
	{
		y = mBoundBottom;
	}

	mCurrentX = x;
	mCurrentY = y;

	return QVS_SUCCESS;
}

int EngineManager::setViewSize(int viewW, int viewH)
{
	mViewW = viewW;
	mViewH = viewH;

	mImageW = viewW;
	mImageH = viewH;

	if (mUseViewSize == 1)
	{
		mImageW = viewW;
		mImageH = viewH;
		mCurrentX = mImageW / 2.0f;
		mCurrentY = mImageH / 2.0f;

		mCurrentScale = 1.0f;

		mUseViewSize = 0;
	}
	else
	{
		if (mImageW * mCurrentScale > mViewW) {
			if (mCurrentX * mCurrentScale < mViewW / 2.0f) {
				mCurrentX = ((mViewW / 2.0f) / mCurrentScale);
			}
			if (mCurrentX * mCurrentScale >(mImageW * mCurrentScale) - (mViewW / 2.0f)) {
				mCurrentX = (((mImageW * mCurrentScale) - (mViewW / 2.0f)) / mCurrentScale);
			}
		}
		else {
			mCurrentX = mImageW / 2.0f;
		}
		if (mImageH * mCurrentScale > mViewH) {
			if (mCurrentY * mCurrentScale < mViewH / 2.0f) {
				mCurrentY = ((mViewH / 2.0f) / mCurrentScale);
			}
			if (mCurrentY * mCurrentScale >(mImageH * mCurrentScale) - (mViewH / 2.0f)) {
				mCurrentY = (((mImageH * mCurrentScale) - (mViewH / 2.0f)) / mCurrentScale);
			}
		}
		else {
			mCurrentY = mImageH / 2.0f;
		}
	}

	return QVS_SUCCESS;
}

int EngineManager::setCursor(int x, int y)
{
	mCursorX = x;
	mCursorY = y;

	return QVS_SUCCESS;
}

int EngineManager::getCursorX()
{
	return mCursorX;
}

int EngineManager::getCursorY()
{
	return mCursorY;
}

int EngineManager::getRegionalWarpingGroupCurrentIndex()
{
	//return mRegionalWarping->getRegionalWarpingGroupCurrentIndex();
	return 0;
}

int EngineManager::getRegionalWarpingGroupSize()
{
	//return mRegionalWarping->getWarpingSize();
	return 0;
}

void EngineManager::setGrayscaleMode(int mode)
{
	stitcher->setGrayscaleMode(mode);
}
void EngineManager::setBrightnessStep(int step)
{
	stitcher->setBrightnessStep(step);
}
void EngineManager::setContrastStep(int step)
{
	stitcher->setContrastStep(step);
}

int EngineManager::getPrepareMode()
{
	return prepareMode;
}

void EngineManager::setPrepareMode(int mode)
{
	printf("setPrepareMode::%d\n", mode);
	prepareMode = mode;

	stitcher->setPrepareMode(bool(prepareMode));
}

int EngineManager::showPrepareView()
{
	if (prepareMode == 0)
	{
		return QVS_SUCCESS;
	}
	else
		return QVS_FAIL;
}

int EngineManager::hidePrepareView()
{
	if (prepareMode == 1)
	{
		return QVS_SUCCESS;
	}
	else
		return QVS_FAIL;
}

void EngineManager::writeLog(char* log, int size)
{
	//printf("==========> writeLogFile_start\n");

	int logSize = mLog.size();
	if (logSize > MAX_LOGVECTOR)
	{
		void* ptr = mLog[0];
		free(ptr);
		ptr = NULL;

		mLog.erase(mLog.begin());

		isNewLog = true;
		newCount++;
	}

	char* temp = (char*)malloc(sizeof(char*)* size);
	memcpy(temp, log, size);
	mLog.push_back(temp);


	/*memory = 0;
	cout << "=====> logLine : " << logSize << endl;
	for (int i = 0; i < logSize; i++)
	{
	memory += sizeof(char*)* size;
	}
	cout << "=====> memory : " << memory << endl;*/

	//cout << "=====> vector : " << mLog.front() << endl;
	//printf("==========> writeLogFile_end\n");
}

void EngineManager::saveLogFile()
{
	//printf("==========> saveLogFile_start\n");
	/*ofstream logFile;
	logFile.open("00_logFile.txt");
	logFile.clear();
	logFile.close();

	logFile.open("00_logFile.txt", ios::app);
	int logSize = mLog.size();
	for (int i = 0; i < logSize; i++)
	{
	logFile << mLog[i] << endl;
	//logFile.put('\n');
	}
	logFile.close();*/
	//cout << "==========>" << mLog[0] << endl;
	_wmkdir(_T("./Logs"));

	time_t now = time(0);
	struct tm tstruct;
	char buffer[80];
	tstruct = *localtime(&now);
	strftime(buffer, sizeof(buffer), "./Logs/LOG_%Y-%m-%d.txt", &tstruct);
	char buffer2[80];
	strftime(buffer2, sizeof(buffer2), " %Y/%m/%d     %X ", &tstruct);

	ofstream logFile;
	logFile.open(buffer, ios::app);
	//logFile.clear();
	int logSize = mLog.size();

	logFile << "\n[" << buffer2 << "]\n" << endl;

	for (int i = 0; i < logSize; i++)
	{
		logFile << mLog[i] << endl;
	}

	for (int k = 0; k < 5; k++)
	{
		logFile << endl;
	}
	logFile.close();

	//printf("==========> saveLogFile_end\n");
}

void EngineManager::clearLog()
{
	//printf("==========> eraseLogFile_start\n");
	int logSize = mLog.size();
	for (int i = 0; i < logSize; i++)
	{
		void* ptr = mLog[i];
		free(ptr);
		ptr = NULL;
	}
	mLog.clear();

	/*ofstream logFile;
	logFile.open("00_logFile.txt");
	logFile.clear();
	logFile.close();*/
	//printf("==========> eraseLogFile_end\n");
}

char* EngineManager::viewLog(bool isFirst)
{
	//printf("==========> getLogFile_start(%d)\n", mLog.size());
	int logSize = mLog.size();
	char output[2048 * sizeof(mLog)] = "";

	if (index > MAX_LOGVECTOR && isNewLog)
	{
		index = MAX_LOGVECTOR - newCount + 1;
		isNewLog = false;
		newCount = 0;
	}
	else if (index > MAX_LOGVECTOR)
		index = logSize;

	if (isFirst)
		index = 0;

	while (index < logSize)
	{
		//printf("=====> vector%d : %s\r\n", i, mLog[i]);
		sprintf(output, "%s%s\r\n", output, mLog[index]);
		index++;
	}


	//index = logSize;
	//printf("==========> getLogFile_end\n");
	return output;
}

void EngineManager::onOutputLog(void* data, char* log, int size, bool isSave)
{
	EngineManager *manager = (EngineManager *)data;
	if (manager != NULL)
	{
		manager->writeLog(log, size);
		if (isSave)
			manager->saveLogFile();
	}
}

void EngineManager::setBeforeImageParam(PTSParam *param)
{
	for (int num = 0; num < mNumCamera; num++)
	{
		before_param[num].v = param[num].v;
		before_param[num].b = param[num].b;
		before_param[num].y = param[num].y;
		before_param[num].p = param[num].p;
		before_param[num].r = param[num].r;

		//printf(">>>>> before_param : %3.3f, %3.3f, %3.3f, %3.3f, %3.3f <<<<<\n", before_param[num].v, before_param[num].b, before_param[num].y, before_param[num].p, before_param[num].r);
	}
}

void EngineManager::setAfterImageParam(PTSParam *param)
{
	for (int num = 0; num < mNumCamera; num++)
	{
		after_param[num].v = param[num].v;
		after_param[num].b = param[num].b;
		after_param[num].y = param[num].y;
		after_param[num].p = param[num].p;
		after_param[num].r = param[num].r;

		//printf(">>>>> after_param : %3.3f, %3.3f, %3.3f, %3.3f, %3.3f <<<<<\n", after_param[num].v, after_param[num].b, after_param[num].y, after_param[num].p, after_param[num].r);
	}
}

void EngineManager::setPlanType(int type)
{
	planType = type;
}

int EngineManager::getPlanType()
{
	return planType;
}

void EngineManager::setDoingLoad(bool status)
{
	doingLoad = status;
}
bool EngineManager::getDoingLoad()
{
	return doingLoad;
}


int EngineManager::stereoStitching()
{
	//mStereoCalibrationThread = CreateThread(NULL, 0, run_stereo_calibration, this, 0, NULL);
	//mSeamThread = CreateThread(NULL, 0, run_seam, this, 0, NULL);


	mlsStereoCalibrationThread = true;


	for (int i = 0; i < mNumCamera; i++)
	{
		//if (mVideoCaptureList[i].mCaptureDeviceIndex == -1)
		//{
		//	isDoingCalibration = false;
		//	if (calibration_done_callback != NULL)
		//		calibration_done_callback(QVS_MSG_CALIBRATION_FAIL);
		//	return QVS_FAIL;
		//}

		if (mVideoCaptureList[i].mVideoCapture)
		{
			if (mIsMultiStreaming == 0 || mVideoCaptureList[i].mVideoCapture->getCaptureType() != QVS_Engine::InputModuleType_Magewell)
				mVideoCaptureList[i].mVideoCapture->requestCopyInputToCPU();
		}
	}

	return QVS_SUCCESS;
}

bool EngineManager::getStereoMode()
{
	if (stitchingMode == STEREO_STITCHING_TB)
		return true;
	else if (stitchingMode == STEREO_STITCHING_LR)
		return true;
	return false;
}

int EngineManager::getStereoPair()
{
	printf("getStereoPair : %d\n", stereoIndex);
	return stereoIndex;
}

void EngineManager::setStereoPair(int index)
{
	stereoIndex = index;
	printf("setStereoPair : %d\n", stereoIndex);
}

void EngineManager::getImageParam(int idx, float &yaw, float &pitch, float &roll)
{
	PTSParam* image_param = stitcher->stitchingTemplate->getImageParams();
	yaw = image_param[idx].y;
	pitch = image_param[idx].p;
	roll = image_param[idx].r;
}

void EngineManager::setImageParam(int idx, float &yaw, float &pitch, float &roll)
{
	PTSParam* image_param = stitcher->stitchingTemplate->getImageParams();
	image_param[idx].y = yaw;
	image_param[idx].p = pitch;
	image_param[idx].r = roll;
	stitcher->updateStitchingTemplate();
}

int EngineManager::setAspectX(int index, int aspectX)
{
	if (mVideoCaptureList[index].mVideoCapture != NULL && mVideoCaptureList[index].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
	{
		mVideoCaptureList[index].mAspectX = aspectX;
		((MWVideoCapture*)mVideoCaptureList[index].mVideoCapture)->setAspectX(aspectX);
		return 1;
	}
	else
		return -1;
}

int EngineManager::setAspectY(int index, int aspectY)
{
	if (mVideoCaptureList[index].mVideoCapture != NULL && mVideoCaptureList[index].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
	{
		mVideoCaptureList[index].mAspectY = aspectY;
		((MWVideoCapture*)mVideoCaptureList[index].mVideoCapture)->setAspectY(aspectY);
		return 1;
	}
	else
		return -1;
}

int EngineManager::getAspectX(int index)
{
	if (mVideoCaptureList[index].mVideoCapture != NULL && mVideoCaptureList[index].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
	{
		return ((MWVideoCapture*)mVideoCaptureList[index].mVideoCapture)->getAspectX();
	}
	else
		return -1;
}

int EngineManager::getAspectY(int index)
{
	if (mVideoCaptureList[index].mVideoCapture != NULL && mVideoCaptureList[index].mVideoCapture->getCaptureType() == QVS_Engine::InputModuleType_Magewell)
	{
		return ((MWVideoCapture*)mVideoCaptureList[index].mVideoCapture)->getAspectY();
	}
	else
		return -1;
}

double EngineManager::getRenderFPS()
{
	printf(">>>>> getRenderFPS renderFPS %lf\n", renderFPS);
	return renderFPS;
}

void EngineManager::setRenderFPS(double fps)
{
	renderFPS = fps;
	checkTimer = QClock();
	printf(">>>>> setRenderFPS renderFPS %lf\n", renderFPS);
}

