#pragma once

struct EngineManager;

#define CAMERA_MAX	16

#define QVS_SUCCESS					0x01
#define QVS_FAIL					0x00
#define QVS_TRUE					0x01
#define QVS_FALSE					0x00
#define QVS_MSG_INVALID_INSTANCE_ERROR	0x101
#define QVS_MSG_FILE_CORRUP_ERROR		0x102
#define QVS_MSG_PARSING_NOT_FOUND_ERROR	0x103
#define QVS_MSG_OUTOFRANGE_ERROR		0x111
#define QVS_MSG_OUTOFMEMORY_ERROR		0x112
#define QVS_MSG_CUDA_SUCCESS			0x201
#define QVS_MSG_CUDA_FAIL				0x202
#define	QVS_MSG_DECKLINK_ERROR			0x203

//added callback MSG 170124
#define QVS_MSG_SNAPSHOT_DONE			0x500
#define QVS_MSG_MANUAL_GAIN_MODE_DONE	0x510
#define QVS_MSG_QURAM_CALIB_MODE_DONE	0x520
#define QVS_MSG_GAIN_DONE				0x530
#define QVS_MSG_SEAM_DONE				0x540
#define QVS_MSG_AUTO_GAIN_DONE			0x550
#define QVS_MSG_EDIT_OUTPUT_DONE		0x560

#define QVS_MSG_MAKE_START_WARPMAP		0x570
#define QVS_MSG_MAKE_DONE_WARPMAP		0x571

#define QVS_MSG_LOADING_WARPMAP			0x580
#define QVS_MSG_LOADING_END_WARPMAP		0x581

#define QVS_MSG_DO_LOAD					0x590
#define QVS_MSG_DONE_LOAD				0x591
#define QVS_MSG_DONE_INPUT_LOAD			0x592

//#define QVS_MSG_OUTPUT_DONE				0x560
//#define QVS_MSG_ENCODING_DONE			0x570
//#define QVS_MSG_STREAMING_DONE			0x580
//#define QVS_MSG_AUDIO_BUF_OFFSET_DONE	0x590
//#define QVS_MSG_VIDEO_BUF_OFFSET_DONE	0x600

#define QVS_MSG_MANUAL_GAIN_MODE_FAIL	0x511
#define QVS_MSG_EDIT_OUTPUT_FAIL		0x561

#define QVS_MSG_CALIBRATION_START		0x600
#define QVS_MSG_CALIBRATION_DONE		0x601
#define QVS_MSG_CALIBRATION_FAIL		0x602
#define QVS_MSG_CALIBRATION_TIMEOUT		0x603
#define QVS_MSG_CALIBRATION_NOT_RUN		0x604
#define QVS_MSG_CALIBRATION_FOV_FAIL	0x605
#define QVS_MSG_CALIBRATION_FOV_WARNING	0x606

#define QVS_MSG_STRAIGHTEN_START		0x607
#define QVS_MSG_STRAIGHTEN_DONE			0x608
#define QVS_MSG_UNDOREDO_START			0x609
#define QVS_MSG_UNDOREDO_DONE			0x610

#define QVS_MSG_HISTORY_ENQUEUE			0x612
#define QVS_MSG_HISTORY_UNDO			0x613
#define QVS_MSG_HISTORY_REDO			0x614

// Regional Warping
#define QVS_MSG_REGIONAL_WARPING_START		0x700
#define QVS_MSG_REGIONAL_WARPING_DONE		0x701
#define QVS_MSG_REGIONAL_WARPING_FAIL		0x702
#define QVS_MSG_REGIONAL_WARPING_INVALID_POINT		0x703
#define QVS_MSG_REGIONAL_WARPING_INVALID_AREA		0x704
#define QVS_MSG_REGIONAL_WARPING_BIG_AREA			0x705
#define QVS_MSG_REGIONAL_WARPING_LONG_DISTANCE		0x706

// Stereo Calibration
#define QVS_MSG_STEREO_CALIBRATION_START		0x800
#define QVS_MSG_STEREO_CALIBRATION_DONE		0x801
#define QVS_MSG_STEREO_CALIBRATION_FAIL		0x802

//Encoding
#define QVS_MSG_NO_ENCODING		0x14
#define QVS_MSG_WRITING			0x13
#define QVS_MSG_FINISH_ENCODING	0x12
#define	QVS_MSG_ENCODING		0x11
#define QVS_MSG_READY_ENCODING	0x10

//Streaming
#define QVS_MSG_NO_STREAMING			0x23
#define QVS_MSG_FINISH_STREAMING		0x22
#define	QVS_MSG_STREAMING				0x21
#define QVS_MSG_READY_STREAMING			0x20

//Blackmagic Output
#define QVS_MSG_NO_BMOUTPUT			0x33
#define QVS_MSG_FINISH_BMOUTPUT		0x32
#define	QVS_MSG_BMOUTPUT			0x31
#define QVS_MSG_READY_BMOUTPUT		0x30

#define PANORAMA_PREVIEW_FULL		0x01
#define PANORAMA_PREVIEW_UP			0x02
#define PANORAMA_PREVIEW_MID		0x03
#define PANORAMA_PREVIEW_DOWN		0x04

// for line
#define FIND_SEAM_FROM_IMAGE	0x01
#define FIND_SEAM_FROM_DIST		0x02

// for EDIT
#define MASK_FOR_SEAM		0x00
#define MASK_FOR_EDIT		0x03

// for blend
#define MULTIBAND_BLEND			0x02
#define NORMAL_BLEND			0X03

// for nadir compensation
#define NADIR_LOG_OFF			0x01
#define NADIR_LOG_ON			0x02

#define NADIR_TOP				0x01
#define NADIR_BOTTOM			0x02  

// for grayscale editing
#define GRAYSCALE_OFF			0x00
#define GRAYSCALE_ON			0x01

#define INPUT_IMAGE				0x50
#define INPUT_VIDEO				0x51

#define MAX_LOGVECTOR			800

//180718 for license
#define LICENSE_PERSONAL_KEY	0
#define QVS_PLANTYPE_DEMO		0
#define QVS_PLANTYPE_4K			1
#define QVS_PLANTYPE_8K			2
#define LICENSE_MACADDRESS		0 //refer to enum on QMacAddress.h
#define LICENSE_POWERED_BY_LG   (LICENSE_MACADDRESS == 1 ? 1 : 0)
#define LICENSE_AVM             (LICENSE_MACADDRESS == 4 ? 1 : 0)

#define MAX_OVERLAY_TEMPLATE 30

typedef unsigned long       DWORD;
typedef DWORD   COLORREF;

//Callback
typedef void(*callback_snapshot)(int msg);
typedef void(*callback_doManualGainMode)(int msg);
typedef void(*callback_quramCalibMode)(int msg);
typedef void(*callback_gainMode)(int msg);
typedef void(*callback_seamMode)(int msg);
typedef void(*callback_autoGainMode)(int msg);
typedef void(*callback_editOutput)(int msg);
typedef void(*callback_calibration)(int msg);
typedef void(*callback_doMakeWarpmap)(int msg);
typedef void(*callback_history)(int msg);
typedef void(*callback_audioError)(int msg);
typedef void(*callback_regionWarping)(int msg);
typedef void(*callback_stereoCalibration)(int msg);
typedef void(*callback_load)(int msg);

//typedef void(*callback_startOutput)(int msg);
//typedef void(*callback_startEncoding)(int msg);
//typedef void(*callback_startStreaming)(int msg);
//typedef void(*callback_audioBufferOffset)(int msg);
//typedef void(*callback_videoBufferOffset)(int msg);

class QVS_Engine {
public:

	struct Configuration{
		Configuration(){

		};

		int hdd_width;
		int hdd_height;
		char hdd_path[256];
		int hdd_bitrate_mode;
		int hdd_bitrate;
		int hdd_gop;
		int hdd_audio_bitrate;
		int hdd_color_space;
		int hdd_preset;
		float hdd_fps;
		int hdd_codec;

		int rtmp_width;
		int rtmp_height;
		char rtmp_path[256];
		int rtmp_bitrate_mode;
		int rtmp_bitrate;
		int rtmp_gop;
		int rtmp_audio_bitrate;
		int rtmp_color_space;
		int rtmp_preset;
		float rtmp_fps;
		int rtmp_codec;
		int rtmp_repeat;
		int rtmp_dynamic_fps;

		int bmoutput_device;
		int bmoutput_display;
	};

	typedef
	enum _InputModuleType
	{
		InputModuleType_Magewell = 0,
		InputModuleType_Blackmagic = 1,
		InputModuleType_Magewell_USB = 2,
		InputModuleType_Image = 3,
		InputModuleType_MIC = 4
	} 	InputModuleType;

	typedef
	enum _ROIMaskMode
	{
		ROIMaskMode_Manual_On = 2,
		ROIMaskMode_On = 1,
		ROIMaskMode_Off = 0
	} 	ROIMaskMode;

	typedef
	enum _ROIMaskDrawMode
	{
		ROIMaskDrawMode_Draw = 1,
		ROIMaskDrawMode_Erase = 0
	} 	ROIMaskDrawMode;

	typedef
	enum _ROIMaskColor
	{
		ROIMaskColor_Masking = 0x3F00FF00,
		ROIMaskColor_No = 0x00000000
	} 	ROIMaskColor;

	typedef
	enum _ROIManualMaskColor
	{
		ROIManualMaskColor_Red_Masking = 0x0000FF3F, //0x3FFF0000,
		ROIManualMaskColor_Green_Masking = 0x00FF003F, //0x3F00FF00,
		ROIManualMaskColor_No = 0x00000000
	} 	ROIManualMaskColor;

	typedef
	enum _EditInputMode
	{
		EditInputMode_On = 1,
		EditInputMode_Off = 0
	} 	EditInputMode;

	typedef
	enum _InputSelectMode
	{
		InputSelectMode_On = 1,
		InputSelectMode_Off = 0
	} 	InputSelectMode;

	typedef
	enum _RegionalWarpingViewMode
	{
		RegionalWarpingViewMode_Edit = 2,
		RegionalWarpingViewMode_Select = 1,
		RegionalWarpingViewMode_Off = 0
	} 	RegionalWarpingViewMode;

	typedef
	enum _CalibrationMode
	{
		CalibrationMode_Stereo = 5,
		CalibrationMode_Advanced_Align = 4,
		CalibrationMode_Detail_All = 3,
		CalibrationMode_MakeTemplate = 2,
		CalibrationMode_Advanced = 1,
		CalibrationMode_FAST_All = 0 //Fast code  (will be erased)
	} 	CalibrationMode;

	typedef
	enum _EncodingPresetMode
	{
		Preset_HIGH_QUALITY = 0,
		Preset_HIGH_PERFORMANCE = 1,
		Preset_LOW_LATENCY_HIGH_PERFORMANCE = 2,
		Preset_LOW_LATENCY_HIGH_QUALITY = 3
	} 	EncodingPresetMode;

	typedef
	enum _StitchingMode
	{
		StitchingMode_Normal = 0,
		StitchingMode_Stereo = 1,
		StitchingMode_StitchedInput = 2,
		StitchingMode_Stereo_Side = 3,
		StitchingMode_Wide = 4
	} 	StitchingMode;

	typedef
	enum _PrepareMode
	{
		StitchingMode_Hide = 0,
		StitchingMode_Show = 1
	} 	PrepareMode;

	static bool QVS_InitEngineManager();
	static int QVS_Create(int numCamera, int inputWidth, int inputHeight, int outputWidth, int outputHeight, int mode);
	static int QVS_SetSavePath(const char *path);
	static int QVS_SaveParam(int is_exit = 0);
	static int QVS_LoadParam(const char *path);
	static int QVS_IsNeedSave();

	static void QVS_SetManualRGB(int idx, int R, int G, int B);
	static int QVS_DoManualGainMode();/*change to int*/
	static void QVS_SetRGBIndex(int index);
	static int QVS_GetRGBIndex();
	static double QVS_GetRed();
	static double QVS_GetGreen();
	static double QVS_GetBlue();
	static void QVS_SetGainChannelIndex(int index);
	static void QVS_SetGainRGBIndex(int index);
	static void QVS_SetGainPointIndex(int index);
	static void QVS_SetGainPointX1(double x);
	static void QVS_SetGainPointX2(double x);
	static void QVS_SetGainPointX3(double x);
	static void QVS_SetGainPointY1(double y);
	static void QVS_SetGainPointY2(double y);
	static void QVS_SetGainPointY3(double y);
	static double QVS_GetGainPointX1();
	static double QVS_GetGainPointX2();
	static double QVS_GetGainPointX3();
	static double QVS_GetGainPointY1();
	static double QVS_GetGainPointY2();
	static double QVS_GetGainPointY3();


	static int QVS_SetQuramCalibMode(int mode);
	static int QVS_SnapShot(long long delay = 0);
	static void QVS_SetSnapshotPath(const char *path);/*change to void*/

	static int QVS_StartOutput();
	static int QVS_FinishOutput();

	static int QVS_SetExposureCompensationMode(int mode);
	static int QVS_SetSeamMode(int mode);
	static int QVS_SetGainMode(int mode);
	static int QVS_SetAutoGainMode(int mode);
	static int QVS_SetNadirCompensationMode(int mode);
	static int QVS_SetNadirCompensationParam(int position, int size);
	static int QVS_UpdateNadirImage();

	static int QVS_SetAudioBufferOffset(int offset);
	static int QVS_SetVideoBufferOffset(int offset);

	static int QVS_SetOutputConfiguration(Configuration *config);
	static int QVS_SetInputConfiguration(float fps, int width, int height, int lens_type, float fov);
	static int QVS_ParsingPTS(const char *path);


	static int QVS_GetNumInput();
	static int QVS_GetNumCameara();
	static float QVS_GetCapureFPS();
	static int QVS_GetCapureWidth();
	static int QVS_GetCapureHeight();
	static int QVS_GetExposureMode();
	static int QVS_GetSeamMode();
	static int QVS_GetGainMode();
	static int QVS_GetNadirMode();
	static int QVS_GetNadirPosition();
	static int QVS_GetNadirSize();
	static char* QVS_GetNadirPath();
	static char* QVS_GetSnapshot_path();
	static int QVS_IsEncoding();
	static bool QVS_GetDeviceList(int n, char* displayName, char* modelName);


	static int QVS_Stitching(int w, int h, int panorama_preview_mode);
	static int QVS_Drawing(int w, int h);
	static int QVS_DrawingManualCalib(int w, int h);
	static int QVS_DrawingInteractive(int w, int h);

	//161227 added
	static void* QVS_GetGainCurveData();
	static void QVS_SetGainCurveData(void* data);
	static void QVS_SetPanoramaSize(int panoramaSize);
	static int QVS_GetPanoramaSize();
	static void QVS_SetAudioDevice(int audioDevice);
	static int QVS_GetAudioDevice();

	static int QVS_GetBmOutputDevice();
	static int QVS_GetBmOutputDisplay();
	static void QVS_SetNadirPath(const char* path);

	static void QVS_SetZoom(float a);
	static void QVS_SetMultiStreaming(int isMultiStreaming);
	static void QVS_SetPan(float a);
	static void QVS_SetPitch(float a);
	static void QVS_MulPitch(float a);
	static int QVS_GetAutoGainMode();

	//170105 added
	static int QVS_createVideoCapture(int cameraIndex, int deviceIndex, int cameraType);
	static int QVS_destroyVideoCapture(int cameraIndex);
	static int QVS_swapVideoCapture(int cameraIndex1, int cameraIndex2);
	static char* QVS_getCaptureDeviceName(int index);
	static int QVS_getCaptureDeviceSize();
	static int QVS_setCaptureDeviceIndex(int index, int value);
	static int QVS_getCaptureDeviceIndex(int index);
	static int QVS_setInputImagePath(int index, char* path);
	static char* QVS_getInputImagePath(int index);
	static void QVS_destroyAudioDevice();

	//170106 added
	static int QVS_setROIMode(int mode);
	static int QVS_saveMask(int imgIndex);
	static int QVS_drawROI(int x, int y, int size, int mode);
	static int QVS_clacManualMaskPoint2(bool isLeft,int x, int y);
	static int QVS_drawManualPosition(int x, int y, int size);
	static int QVS_drawManualROI(int x, int y, int view_w, int view_h, int size, int mode, int isLeft);
	//static int QVS_drawManualRightROI(int x, int y, int size, int mode, int imgIndex);
	static int QVS_InitDrawManualROI();
	static int QVS_drawROIFinish();
	static int QVS_clearROI();
	static int QVS_clearManualMaskROI(bool isAll);
	static int QVS_setSelectMode(int mode);
	static int QVS_selectROI(int x, int y, int &isSelected);
	static int QVS_selectAllROI();

	//170124 added
	static void QVS_setCallback_snapshot(callback_snapshot cb);
	static void QVS_setCallback_doManualGainMode(callback_doManualGainMode cb);//??
	static void QVS_setCallback_quramCalibMode(callback_quramCalibMode cb);
	static void QVS_setCallback_gainMode(callback_gainMode cb);//??
	static void QVS_setCallback_seamMode(callback_seamMode cb);
	static void QVS_setCallback_autoGainMode(callback_autoGainMode cb);

	//170202 added
	static void QVS_setCallback_editOutput(callback_editOutput cb);

	//170213 added
	static void QVS_setCallback_calibration(callback_calibration cb);
	static void QVS_setCallback_doMakeWarpmap(callback_doMakeWarpmap cb);
	static void QVS_setCallback_history(callback_history cb);

	//170214 added
	static int QVS_moveOutput(float x, float y, int isRoll, int isReverseX, int isReverseY);
	static bool QVS_editOutput();
	static void QVS_saveOutput();
	static int QVS_isRedraw();
	//static void QVS_setCallback_startOutput(callback_startOutput cb);
	//static void QVS_setCallback_startEncoding(callback_startEncoding cb);
	//static void QVS_setCallback_startStreaming(callback_startStreaming cb);
	//static void QVS_setCallback_audioBufferOffset(callback_audioBufferOffset cb);
	//static void QVS_setCallback_videoBufferOffset(callback_videoBufferOffset cb);

	//170221 added
	static int QVS_VideoCaptureInit();

	//170315 added
	static int QVS_savePanorama(char* save_path);

	//170321
	static int QVS_getLensType();
	static float QVS_getFOV();
	static int QVS_saveLensType(int lens_type);

	//170330
	static int QVS_getLensTypeCount();
	static char* QVS_getLensTypeName(int index);

	//170406
	static void QVS_editOff();
	static bool QVS_isEdit();

	//170414
	static int QVS_GetOutputWidth();
	static int QVS_GetOutputHeight();

	//170616
	static int QVS_GetCaptureWidth();
	static int QVS_GetCaptureHeight();
	static void QVS_SetDrawMaskLeftIndex(int index);
	static void QVS_SetDrawMaskRightIndex(int index);
	static int QVS_GetDrawMaskLeftIndex();
	static int QVS_GetDrawMaskRightIndex();
	static void QVS_SetDrawMaskColor(int index);
	static void QVS_RotateManualMaskLeft(int direction);
	static void QVS_RotateManualMaskRight(int direction);

	//170417
	static void QVS_undo();
	static void QVS_redo();
	static bool QVS_undoEnabled();
	static bool QVS_redoEnabled();

	static int QVS_getMaxNumOverlay();

	//170428
	static int QVS_destroy();

	//170612
	static int QVS_getStep(int mode);
	static void QVS_setStep(int mode, int step);

	//170620
	static int QVS_getSelectedImage();

	//170707
	static void QVS_setCallback_audioError(callback_audioError cb);
	static char* QVS_getAudioCaptureDeviceName(int index);
	static int QVS_AudioCaptureUpdate();
	static int QVS_getAudioCaptureDeviceSize();

	static int QVS_FileWrite(int w, int h, int channel, unsigned char* buffer);

	static int QVS_CheckInitParam(int &check);

	static void QVS_setTextOverlay(char* text, char* font, int fontstyle, COLORREF txtColor, int bgAlpha, COLORREF bgColor, int id);
	static char* QVS_getTextOverlayText(int id);
	static char* QVS_getTextOverlayFont(int id);
	static int QVS_getTextOverlayFontStyle(int id);
	static COLORREF QVS_getTextOverlayTextColor(int id);
	static int QVS_getTextOverlayTextBgAlpha(int id);
	static COLORREF QVS_getTextOverlayTextBgColor(int id);

	//171011
	static int QVS_setEditInputMode(int mode);
	static int QVS_onSize(int w, int h);
	static int QVS_onDown(int x, int y);
	static int QVS_onUp(int x, int y);
	static int QVS_onScroll(int dx, int dy);

	//171012
	static int QVS_ZoomIn(float s, int x, int y);
	static int QVS_ZoomOut(float s, int x, int y);
	static int QVS_ScaleOutput(float s, int x, int y);
	static int QVS_ResetToFullView();
	static int QVS_Scroll(int x, int y, int w, int h);
	static int QVS_SetViewSize(int w, int h);
	static void QVS_setCallback_regionWarping(callback_regionWarping cb);
	static void QVS_setCallback_stereoCalibration(callback_stereoCalibration cb);
	static int QVS_setCursor(int x, int y);
	static int QVS_getCursorX();
	static int QVS_getCursorY();
	static int QVS_getRegionalWarpingGroupCurrentIndex();
	static int QVS_getRegionalWarpingGroupSize();

	// 171101 for Filter
	static void QVS_setGrayscaleMode(int mode);
	static void QVS_setBrightnessStep(int step);
	static void QVS_setContrastStep(int step);
	
	//190718 for prepareMode
	static int QVS_getPrepareMode();
	static void QVS_setPrepareMode(int mode);
	static int QVS_showPrepareView();
	static int QVS_hidePrepareView();

	//171106 for logview
	static void QVS_writeLog(char* log, int size);
	static void QVS_saveLogFile();
	static void QVS_clearLog();
	static char* QVS_viewLog(bool isFirst);

	static bool QVS_getIsDoingAnimation();

	//180718 for license
	static void QVS_setPlanType(int type);
	static int QVS_getPlanType();

	static void QVS_setCallback_load(callback_load cb);
	static void QVS_setDoingLoad(bool b);
	static bool QVS_getDoingLoad();

	//190129 for Stereo
	static void QVS_stereoStitching();
	static bool QVS_getStereoMode();

	static void QVS_setStereoPair(int index);
	static int QVS_getStereoPair();

	static void QVS_getImageParam(int idx, float &yaw, float &pitch, float &roll);
	static void QVS_setImageParam(int idx, float &yaw, float &pitch, float &roll);

	//191002 for aspect ratio
	static int QVS_setAspectX(int index, int aspectX);
	static int QVS_setAspectY(int index, int aspectY);
	static int QVS_getAspectX(int index);
	static int QVS_getAspectY(int index);

	//200102 for rendering fps
	static double QVS_getRenderFPS();
	static void QVS_setRenderFPS(double fps);
};