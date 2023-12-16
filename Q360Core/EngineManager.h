#pragma once

// Release Number . Major Number . Minor Number
#define QURAM_360_ENGINE_VERSION "2.4.8"

#include "CamInput/CamInputArray.h"
#include "Composer/Composer.h"
#include "EditPanorama.h"

//#include <FL/Fl.h>
//#include <FL/x.H>
#ifdef WIN32
//#pragma comment (lib, "fltkdll.lib")

#include <GL/Glew.h>
#include <GL/wGlew.h>
//#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")
#pragma comment (lib, "glew.lib")
#endif
#include <PugiXML/pugixml.hpp>
#ifdef WIN32
//#pragma comment (lib, "pugixmlsd.lib")
#endif

//#include <FL/Fl_Gl_Window.H>
#include "CamInput/CamInputArray.h"
#include "Composer/Composer.h"
#include "GLBase/glsl.h"
#include "GLBase/glProg.h"
#include <FL/gl.h>
#include "clock.h"
#include "opencv2/highgui/highgui.hpp"
#include "stitcher.h"
#include "InputManager.h"
#include "Capture/XIVideoCapture.h"
#include "Capture/XIAudioCapture.h"
#include "Capture/MWVideoCapture.h"
#include "Capture/MWAudioCapture.h"
#include "Capture/BMVideoCapture.h"
#include "Capture/BMAudioCapture.h"
#include "Capture/IMGVideoCapture.h"
#include "Capture/MICAudioCapture.h"
#include "Quram360Engine.h"
#include "Base64ImageDecoder.h"
//#include "Capture/BMOutput.h"

#include "QWarper.h"
#include "WarpInfo.h"
#include "StitchingTemplate.h"

//#include "Calibrator_multi.h"
//#include "QCalibrator/QCameraParameter/QCameraParams.h"
//#include "QCalibrator\QUtils\qutils.h"
#include "Calibrator_Input.h"
#include "QAdvencedParam.h"
#include "PTSParam.h"
#include "snapshot.h"
#include "AudioPlayer.h"

#include "Draw/Draw.h"
#include "Draw/Texture.h"

#include "RegionalWarping/SingleRegionalWarping.h"
#include "RegionalWarping/MultiRegionalWarping.h"

#include <vector>

//#include <d3d11.h>
#include <fstream>
#include <iostream>
#include "glUtils.h"
#include "mask_util.h"

#include "AudioController.h"

#include "InputTempData.h"
//#pragma comment (lib, "d3d11.lib")
//#pragma comment (lib, "QCalibration.lib")

#define NUM_CHANNEL	2
#define CAPTURE_COLORSPACE MWFOURCC_RGB24

#define STRAIGHTEN_ANIM_TIMER	33

#define MAX_OVERLAY_FOV		180
#define MIN_OVERLAY_FOV		10

//#define MAX_OVERLAY_TEMPLATE 10
//#define MAX_OVERLAY_SHOW_ALL 40
//#define MAX_OVERLAY_SHOW_MOV 2

#define TEST_OVERLAYLOAD_THREAD 1

#ifdef IMAGE_TO_BUFFER
#define NUM_BUFFERS					16
#else
#define NUM_BUFFERS					4
#endif

#ifdef SDI_OUTPUT_TEST
#define NUM_OUTPUT_BUFFERS 10
#endif

struct PerfectCalibrationInfo {
	std::vector<float> mSrcX, mSrcY, mDstX, mDstY;
	std::vector<float> mPFSrcX, mPFSrcY, mPFDstX, mPFDstY;

	int mIndex;
};

struct VideoCaptureList {
	VideoCapture* mVideoCapture;
	VideoCapture* mVideoCapture_CPU;

	int mCaptureDeviceIndex;
	int mAspectX;
	int mAspectY;
};

struct CaptureDevice {
	char* mCaptureDeviceName;
	int mCaptureDeviceType;
	int mDeviceNumber;

	CaptureDevice() { mDeviceNumber = -1; }
};

struct EngineManager {

	EngineManager() {
		printf("\n##### Quram 360 Engine Version : %s #####\n\n", QURAM_360_ENGINE_VERSION);

		//calibrator = NULL;
		base64ImageDecoder = NULL;
	};

	double oldTime;

	Stitcher *stitcher;
	InputManager *input_manager;
	//Calibrator *calibrator;
	InputTempData *input_tempData;
	VideoCapture** video_capture;
	AudioCapture* audio_capture;

	void getEncoderReport_Hdd(double *bitrate, int *secs, float *fps);
	void getEncoderReport_Streaming(double *bitrate, int *secs, float *fps);

	int startVodStreaming(char* filepath, char* url, bool isRepeat);

	Base64ImageDecoder* base64ImageDecoder;

	VideoCaptureList mVideoCaptureList[CAMERA_MAX];
	char mInputImagePath[CAMERA_MAX][MAX_PATH];

	int** temp_videoCapture_state;

	std::vector<CaptureDevice> mAudioCaptureDeviceList;
	std::vector<CaptureDevice> mCaptureDeviceList;



	int output_width = 0, output_height = 0;

#ifdef SDI_OUTPUT_TEST
	BMOutput* video_output;
#endif

#ifdef SDI_OUTPUT_TEST
	void** outputBuffers;
	int outputIndex;
#endif
	char* dllTest();
	int Create(int numCamera, int inputWidth, int inputHeight, int outputWidth, int outputHeight, int mode);
	int Destroy();
	int Init();
	int GenTextures();
	int DeleteTextures();
	int AudioCaptureUpdate();
	int AudioCaptureInit();
	int VideoCaptureInit();

	//added for engine API 170105
	int createVideoCapture(int cameraIndex, int deviceIndex, int cameraType);
	int destroyVideoCapture(int cameraIndex);
	int swapVideoCapture(int cameraIndex1, int cameraIndex2);
	char* getAudioCaptureDeviceName(int index);
	int getAudioCaptureDeviceSize();
	char* getCaptureDeviceName(int index);
	int getCaptureDeviceSize();
	int setCaptureDeviceIndex(int index, int value);
	int getCaptureDeviceIndex(int index);
	int setInputImagePath(int index, char* path);
	char* getInputImagePath(int index);
	//added for engine API 170105

	int Stitching(int w, int h, int panorama_preview_mode);
	unsigned char* setSelectPosition(int i, int bound_x, int bound_y, int bound_w, int bound_h);
	int Drawing(int w, int h);
	int DrawingManualCalib(int w, int h);
	int DrawingInteractive(int w, int h);
	int setSavePath(const char *path);
	int IsNeedSave();
	int SaveParam(int is_exit = 0);
	int LoadParam(const char *path);
	int setSnapshotPath(const char *path);
	int SnapShot(LONGLONG delay = 0);
	int ParsingPTS(const char *path);

	int startOutput();
	int finishOutput();

	int setBlendingMode(int mode);
	int setSeamMode(int mode);
	int setGainMode(int mode);
	int setAutoGainMode(int mode);
	int doManualGainMode();
	int setQuramCalibMode(int mode);
	void* detectAllSeamArea();
	void* detectSeamArea(int index1, int index2);
	int getStep(int mode);
	void setStep(int mode, int step);
	void setManualRGB(int idx, int R, int G, int B);
	//void setOutputConfiguration(int hdd_width, int hdd_height, char* hdd_path, int hdd_bitrate_mode, int hdd_bitrate, int hdd_gop, int hdd_audio_bitrate, int hdd_color_space, int hdd_preset, float hdd_fps, int hdd_codec,
	//	int rtmp_width, int rtmp_height, char* rtmp_path, int rtmp_bitrate_mode, int rtmp_bitrate, int rtmp_gop, int rtmp_audio_bitrate, int rtmp_color_space, int rtmp_preset, float rtmp_fps, int rtmp_codec, int bmoutput_device, int bmoutput_display);
	int setOutputConfiguration(QVS_Engine::Configuration config);
	int setInputConfiguration(float fps, int width, int height, int lens_type, float fov);
	void setAudioDevice(int audioDevice);

	//added for engine API 170105
	void destroyAudioDevice();
	//added for engine API 170105

	void setPanoramaSize(int panoramaSize);
	int getPanoramaSize();
	void setMultiStreaming(int isMultiStreaming);

	//added for engine API 170106
	int setROIMode(int mode);
	int drawManualPosition(int x, int y, int size);
	int saveMask(int mode);
	int drawManualROI(int x, int y, int view_w, int view_h, int size, int mode, int imgIndex);
	int checkLeftMask(int x, int y, int view_w, int view_h);
	int calcScreenPosition(int x, int y, int &pos_x, int &pos_y, int isLeft);
	int drawCircleMask(int centerX, int centerY, int r, int mode, int imgIndex, int imgColor);
	int calcManualMaskPoint2(bool isLeft, int x, int y);
	int InitDrawManualROI();
	int drawROI(int x, int y, int size, int mode);
	int drawROIFinish();
	int clearROI();
	int clearManualMaskROI(bool isAll);
	int setSelectMode(int mode);
	int selectROI(int x, int y, int &isSelected);
	int selectAllROI();
	//added for engine API 170106

	//added for engine API 170214
	int moveOutput(float x, float y, int isRoll, int isReverseX, int isReverseY);
	bool editOutput();
	void saveOutput();
	void setIsRedraw(int isRedraw);
	int getIsRedraw();

	int zoomOverlay(float scale);
	int moveOverlay(float x, float y);
	int rollOverlay(float x1, float y1, float x2, float y2);

	//added for engine API 170315
	int savePanorama(char* file_path);

	const char* getFPS();
	int setAudioBufferOffset(int offset);
	int setVideoBufferOffset(int offset);

	//added for engine API 170330
	int getLensTypeCount();
	char* getLensTypeName(int index);

	//added for engine API 170406
	void editOff();
	bool isEdit();

	//added for engine API 170417
	bool undo();
	bool redo();

	//added for engine API 170615
	bool undoEnabled();
	bool redoEnabled();

	//added for engine API 171011
	// Edit Input Animation
	int setEditInputMode(int mode);
	int onSize(int w, int h);
	int onDown(int x, int y);
	int onUp(int x, int y);
	int onScroll(int dx, int dy);

	// 190129 for stereo
	int stereoStitching();
	bool getStereoMode();

	int getStereoPair();
	void setStereoPair(int index);

	void getImageParam(int idx, float &yaw, float &pitch, float &roll);
	void setImageParam(int idx, float &yaw, float &pitch, float &roll);

	//191002 for aspect ratio
	int setAspectX(int index, int aspectX);
	int setAspectY(int index, int aspectY);
	int getAspectX(int index);
	int getAspectY(int index);

	//200102 for rendering fps
	double renderFPS = 60.0;
	double getRenderFPS();
	void setRenderFPS(double fps);
	double checkTimer = 0.0;

	// for VideoCapture
	static void onSnapShotDone(void *handle, void *data, BYTE* buffer, int size, long long timeStamp);
	static void onInputError(void *handle, void *data, int msg, long long timeStamp);
	static void onAudioCaptureDone(void *handle, void *data, unsigned char* buffer, int size, long long timeStamp);
	static void onCaptureDone(void *handle, void *data, long long timeStamp, int captureIndex);
	static void* getAddress(void *handle, void* data, int captureIndex);
#ifdef SDI_OUTPUT_TEST
	static void* getOutputAddress(void *handle, void* data);
#endif
	static BOOL getDeviceList(int n, char* displayName, char* modelName);

	static DWORD WINAPI onTimer_calibration(LPVOID handle);

	static DWORD WINAPI run_seam(LPVOID handle);
	static DWORD WINAPI run_gain(LPVOID handle);
	static DWORD WINAPI run_manual_curve(LPVOID handle);
	static DWORD WINAPI run_regional_warping(LPVOID handle);
	static DWORD WINAPI run_warpMap(LPVOID handle);
	static DWORD WINAPI	run_edit_mode(LPVOID handle);
	static DWORD WINAPI	run_undo(LPVOID handle);
	static DWORD WINAPI	run_redo(LPVOID handle);
	static DWORD WINAPI	run_load_param(LPVOID handle);
	static DWORD WINAPI run_stereo_calibration(LPVOID handle);
	

	int is_thread_seam;
	int is_Init;
	int is_load;
	//int is_buffering;

	//// Regional Warping
	std::vector<float> mPFSrcX, mPFSrcY, mPFDstX, mPFDstY;
	std::vector<float> mSrcX, mSrcY, mDstX, mDstY;
	std::vector<float> mOutputSrcX, mOutputSrcY, mOutputDstX, mOutputDstY;
	int is_RegionWarping;
	int is_resetRegionWarping;
	int is_undoRegionWarping;
	int isDoingUndoRegionWarping;
	int isDoingEvent;
	SingleRegionalWarping* mRegionalWarping;
	int mUseViewSize;
	int mViewW, mViewH;
	int mImageW, mImageH;
	float mCurrentScale;
	float mRotateScale;
	float mCurrentX, mCurrentY;
	float mFocusX, mFocusY;
	float mBoundLeft, mBoundRight, mBoundTop, mBoundBottom;
	int mCursorX, mCursorY;


	GLuint texId[CAMERA_MAX];
	int is_GenTextures;
	int is_VideoCaptureInit;

	//char PTS_path[MAX_PATH];
	char snapshot_path[MAX_PATH];
	char save_path[MAX_PATH];

	PTSParam nadir_param;

	int mBlendingMode;
	int mSeamMode;
	int mGainMode;
	int mAutoGainMode;
	int mNadirMode;
	int mNadirPosition;
	int mNadirSize;
	char mNadirPath[MAX_PATH];
	int mRGBIndex;
	int mRed[CAMERA_MAX], mGreen[CAMERA_MAX], mBlue[CAMERA_MAX];
	char mStmap_L_Path[MAX_PATH];
	char mStmap_R_Path[MAX_PATH];

	bool is_doing_exposure = false;
	bool is_doing_seam = false;

	bool is_need_save;

	float mCaptureFPS;
	float mCaptureFov;
	int mNumCamera;
	int mNumVerticalLine;
	int mNumHorizontalLine;
	int mCaptureWidth;
	int mCaptureHeight;
	int mMaskCaptureWidth;
	int mMaskCaptureHeight;
	cv::Rect mMaskUpdateRect;
	bool mIsRanderingStart;
	int mCaptureLensType;
	int mCalibrationIndex[CAMERA_MAX];
	int mRegionalWarpingIndex[CAMERA_MAX]; // Reiognal Warping
	int mRegionalWarpingMapIndex[CAMERA_MAX];
	int mRegionalWarpingUndoMapIndex[CAMERA_MAX];
	int mRegionalWarpingBeforeMapZeros[CAMERA_MAX];
	bool mSaveMapIndex[CAMERA_MAX];
	int mBMOutputDevice;
	int mBMOutputDisplay;
	int mGainChannelIndex;
	int mGainRGBIndex;
	int mGainPointIndex;
	double mGainPointX[CAMERA_MAX][3][3], mGainPointY[CAMERA_MAX][3][3];
	void** mGainCurveData;
	double mOutputCurvePointX[3], mOutputCurvePointY[3];
	void* mOutputCurveData;
	int mAudioDevice;
	int mPanoramaSize;
	int mIsMultiStreaming;

	int mIsManualMaskLeft;
	int mManual_calib_mask_left_index;
	int mManual_calib_mask_right_index;
	int mRotateManualMask_Left_angle;
	int mRotateManualMask_Right_angle;
	GLdouble mModelview_Mat[2][16];
	GLdouble mProjection_Mat[16];
	int mManual_calib_mask_color;
	int mPoint_x, mPoint_y, mSize;
	int mPoint_x2, mPoint_y2;

	int is_request_change_param;
	int is_request_change_fps;
	int is_request_change_status;

	int is_audio_header_hdd;
	int is_audio_header_rtmp;

	int is_AudioPlayerInit;

	int mNumOtherInput;

	GLuint roiTexId[3];
	unsigned char* roi_buffer;
	GLuint mManual_RoiTexId[CAMERA_MAX];
	unsigned char* mRoi_Manual_buffer[CAMERA_MAX];

	int roi_x, roi_y;
	int mROIMode;
	int is_draw_ROI;
	int is_Redraw;
	bool is_editmode;

	int mSelectSize;
	GLuint selectTexId[CAMERA_MAX];
	GLuint selectedTexId[CAMERA_MAX];
	GLuint maskTexId[CAMERA_MAX];
	unsigned char* select_buffer[CAMERA_MAX];
	unsigned char* selected_buffer[CAMERA_MAX];
	int mSelectMode;
	int mCalibrationMode;
	int mRegionalWarpingViewMode; // Regional Warping
	int mStep, mStep_update, mStep_align;

	//animation
	bool mImageLeftSelectAnimationStart, mImageRightSelectAnimationStart;
	double mImageLeftSelectTime, mImageRightSelectTime;

	float mCenter_x[CAMERA_MAX];
	float mCenter_y[CAMERA_MAX];

	double timer;

	HANDLE mSeamThread;
	HANDLE mGainThread;
	HANDLE	mCalibrationThread;
	HANDLE	mPerfectCalibrationThread;
	HANDLE	mRegionalWarpingThread; // Regional Warping
	HANDLE	mStereoCalibrationThread; // Regional Warping
	HANDLE	mWarpingMapThread;
	HANDLE	mEditOutputThread;
	HANDLE mManualCurveThread;
	HANDLE mUndoThread;
	HANDLE mRedoThread;
	HANDLE mOverlayLoadThread;

	long long gain_timestamp = 0;

	//interactive coordinates
	float *texCoordS, *texCoordT, *texCoordT2;
	float *coordY1, *coordY2;
	float **coordX1, **coordX2, **coordZ1, **coordZ2;

	float *texCoordS_side, *texCoordT_side, *texCoordT2_side;
	float *texCoordS_circle, *texCoordT_circle, *texCoordT2_circle;
	float *coordX1_side, *coordX2_side, *coordY1_side, *coordY2_side, *coordZ1_side, *coordZ2_side;
	float *coordX1_circle, *coordX2_circle, *coordY1_circle, *coordY2_circle, *coordZ1_circle, *coordZ2_circle;

	bool isDoingCalibration = false;

	int mCaptureDoneCounter;
	bool mIsSeamThread;
	bool mIsGainThread;
	bool mIsCalibrationThread;
	bool mIsPerfectCalibrationThread;
	bool mlsStereoCalibrationThread;

	// Edit Input Animation
	int is_SourceViewInit;
	int mEditInputMode;
	Texture* mSourceTextures[CAMERA_MAX];
	Texture* mEditInputTexture;
	int mEditInputTextureIndex;
	//int mViewW, mViewH;

	//Stereo
	int is_doing_stereoCalibration;
	//bool is_StereoMode = false;
	int stitchingMode = 0;
	//bool is_StitchedMode = false;
        int stereoIndex = 0;

	//Wide Panorama
	int mOutputType = EQUIRECTANGULAR; //Equirectangular
	float mOutputFov = 360;

	//added callback for App
	callback_snapshot snapshot_done_callback;
	callback_doManualGainMode doManualGainMode_done_callback;
	callback_quramCalibMode quramCalibMode_done_callback;
	callback_gainMode gainMode_done_callback;
	callback_seamMode seamMode_done_callback;
	callback_autoGainMode autoGainMode_done_callback;
	callback_editOutput editOutput_done_callback;
	callback_calibration calibration_done_callback;
	callback_doMakeWarpmap makeWapmap_callback;
	callback_history history_callback;
	callback_audioError audioError_callback;
	callback_regionWarping regionWarping_done_callback;
	callback_stereoCalibration stereoCalibration_done_callback;
	callback_load load_callback;

	/*callback_startOutput startOutput_done_callback;
	callback_startEncoding startEncoding_done_callback;
	callback_startStreaming startStreaming_done_callback;
	callback_audioBufferOffset audioBufferOffset_done_callback;
	callback_videoBufferOffset videoBufferOffset_done_callback;*/


	//added setCallback for App
	void setCallbackSnapshot(callback_snapshot cb);
	void setCallbackDoManaulGainMode(callback_doManualGainMode cb);
	void setCallbackQuramCalibMode(callback_quramCalibMode cb);
	void setCallbackGainMode(callback_gainMode cb);
	void setCallbackSeamMode(callback_seamMode cb);
	void setCallbackAutoGainMode(callback_autoGainMode cb);
	void setCallbackEditOutput(callback_autoGainMode cb);
	void setCallbackCalibration(callback_calibration cb);
	void setCallbackDoMakeWarpmap(callback_doMakeWarpmap cb);
	void setCallbackHistory(callback_history cb);
	void setCallbackAudioError(callback_audioError cb);
	void setCallbackRegionWarping(callback_regionWarping cb);
	void setCallbackStereoCalibration(callback_stereoCalibration cb);
	void setCallbackLoad(callback_load cb);

	bool doingLoad = false;
	void setDoingLoad(bool status);
	bool getDoingLoad();


	void makeSeamMask(long long timeStame, int* buffer_idx, int edit_mode);
	void makeStereoInputData(long long timeStamp, int* buffer_idx);
	//void calculatePixelAvg(int* buffer_idx);
	void estimateExposureCompensation(int* buffer_idx);
	std::vector<cv::Mat> getCurve(long long timeStamp, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints);
	int setNadirCompensationMode(int mode);
	int setNadirCompensationParam(int position, int size);
	bool startAutoCrop(std::vector<cv::Mat>& imgs, PTSParam* input_image_param, PTSParam *global_image_param, PTSParam* output_image_param, WarpInfo* warp_info);
	void circles_detector(cv::Mat &image, std::vector<cv::Vec3f> &circles, cv::Rect& boundingRect, int index, double canny_threshold = 100.0, double circle_threshold = 0.23, int numIterations = 50);
	int makeCircularMask(cv::Mat& Img, cv::Mat& mask, std::vector<cv::Vec3f> circles, cv::Rect& boundingRect, int margin);

	int updateNadirImage();
	int updateStmap(int index);

	//added for engine API
	void setRGBIndex(int index);
	int getRGBIndex();
	double getRed();
	double getGreen();
	double getBlue();
	void setGainChannelIndex(int index);
	void setGainRGBIndex(int index);
	void setGainPointIndex(int index);
	int getCaptureType();
	int getNumCameara();
	int getNumInput();
	float getCapureFPS();
	int getCapureWidth();
	int getCapureHeight();
	int getOutputWidth();
	int getOutputHeight();
	int getCaptureWidth();
	int getCaptureHeight();
	void setDrawMaskLeftIndex(int index);
	void setDrawMaskRightIndex(int index);
	int getDrawMaskLeftIndex();
	int getDrawMaskRightIndex();
	void setDrawMaskColor(int index);
	void rotateManualMaskLeft(int direction);
	void rotateManualMaskRight(int direction);
	int getExposureMode();
	int getSeamMode();
	int getGainMode();
	int getNadirMode();
	int getNadirPosition();
	int getNadirSize();
	char* getNadirPath();
	void setNadirPath(const char* path);
	void setStmapPath(int index, const char* path);
	char* getSnapshot_path();
	int isEncoding();
	void setGainPointX1(double x);
	void setGainPointX2(double x);
	void setGainPointX3(double x);
	void setGainPointY1(double y);
	void setGainPointY2(double y);
	void setGainPointY3(double y);
	double getGainPointX1();
	double getGainPointX2();
	double getGainPointX3();
	double getGainPointY1();
	double getGainPointY2();
	double getGainPointY3();

	void* getGainCurveData();
	void setGainCurveData(void* data);
	int getAudioDevice();
	int getLensType();
	float getFOV();

	int getBmOutputDevice();
	int getBmOutputDisplay();

	int getAutoGainMode();

	void* getOutputCurveData();
	void setOutputCurveData(void* data);

	//Temporary Gain Curve Data
	double *tempX0, *tempX1, *tempX2;
	double *tempY0, *tempY1, *tempY2;
	std::vector<std::vector<std::vector<cv::Point2d>>> mCurvePoints;

	// bigheadk for calibration
	void updateWarpMap();

	//interacitve coordinates
	void calculateInteractiveCoordinates();
	void destroyInteractiveCoordinates();
	void calculateInteractiveCoordinatesForWidePanorama();

	void destroyInputColors();

	// AudioPlayer
	//AudioPlayer* mAudioPlayer;
	AudioController* mAudioController;

	//for advanced calibration
	int getSelectedImage();

	void checkWarpingMap(bool showProgToast = true);
	int checkInitParam();
	void FileWriter(int w, int h, int channel, unsigned char* buffer);

	// Regional Warping
	int getRegionalWarpingViewMode();
	int setRegionalWarpingViewMode(int mode);
	int selectWarpingPoint(int x, int y, int w, int h, int mode);
	void convertInput2Output(int num, int x, int y, double point[2]);
	void convertOutput2Input(int num, int x, int y, double point[2]);
	int doRegionalWarping();
	int resetRegionalWarping();
	bool isDeleteRegionalWarping();
	int deleteRegionalWarping();
	int getRegionalWarpingMode();
	int getRegionalWarpingSize();
	int getRegionalWarpingLength();
	int getRegionalWarpingAutoMode();
	int setRegionalWarpingMode(int mode);
	int setRegionalWarpingSize(int size);
	int setRegionalWarpingLength(int length);
	int setRegionalWarpingAutoMode(int autoMode);
	int zoomIn(float s, int x, int y);
	int zoomOut(float s, int x, int y);
	int ScaleOutput(float s, int x, int y);
	int resetToFullView();
	int scroll(int dx, int dy, int w, int h);
	int setViewSize(int viewW, int viewH);
	int setCursor(int x, int y);
	int getCursorX();
	int getCursorY();
	int getRegionalWarpingGroupCurrentIndex();
	int getRegionalWarpingGroupSize();

	//Filter
	void setGrayscaleMode(int mode);
	void setBrightnessStep(int step);
	void setContrastStep(int step);

	//PrepareMode
	int prepareMode = 0;
	int getPrepareMode();
	void setPrepareMode(int mode);
	int showPrepareView();
	int hidePrepareView();

    //Log
	std::vector<char*> mLog;
	int index = 0;
	//int memory = 0;
	int newCount = 0;
	bool isNewLog = false;
	void writeLog(char* log, int size);
	void saveLogFile();
	void clearLog();
	char* viewLog(bool isFirst);

	static void onOutputLog(void* data, char* log, int size, bool isSave);

	// GL
	//int W = 1268;
	//int H = 1268;
	int W = 1024;
	int H = 768;

	float ro_x = 0.0f;
	float ro_y = 0.0f;
	float fov = 90.0f;

	double gain_timer;

	float cal_ro_x = 0.0f;
	float cal_ro_y = 0.0f;
	float cal_ro_z = 0.0f;
	float cal_fov = 0.0f;

	float edit_pos_fov = 0.0f;
	float edit_pos_yaw = 0.0f;
	float edit_pos_roll = 0.0f;
	float edit_pos_pitch = 0.0f;

	bool change_stereo_diff = false;
	bool change_overlay_pos = false;

	float cal_ro_x_anim;
	float cal_ro_y_anim;
	float cal_ro_z_anim;

	int anim_cal_mode = 0;


	PTSParam before_param[8];
	PTSParam after_param[8];
	void setBeforeImageParam(PTSParam *param);
	void setAfterImageParam(PTSParam *param);
	float warping_animTime = 1.0f;
	int countTime = 0;
	int animationTime = 30;
	bool isStraighten = false;
	bool isUndoRedo = false;
	bool getIsUndoRedo() { return isUndoRedo; };
	bool isDoingAnimation = false;
	bool getIsDoingAnimation() { return isDoingAnimation; };
	void setIsDoingAnimation(bool state) { isDoingAnimation = state; };
	int is_beforeRegionWarping = 0;
	int is_setMapZeros = 0;

	int planType = QVS_PLANTYPE_DEMO;
	void setPlanType(int type);
	int getPlanType();



	void reshape(int w, int h) {
		//glViewport((w - W) / 2, (h - H) / 2, W, H);
		glViewport(0, 0, w, h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		/*
		gluPerspective(fov, 1, 0.01, 10);
		*/
		if (fov >90) {
			gluPerspective(90, (double)w / (double)h, 0.001, 10);
			glTranslatef(0, 0, -(fov - 90) / 100);
		}
		else
			gluPerspective(fov, (double)w / (double)h, 0.001, 10);

		//printf("reshape: fov = %f\n", fov);
		//printf("reshape: (w, h) = (%d, %d)\n", w, h);
	}

	void display(int w, int h, GLuint tex) {
		reshape(w, h);

		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glColor4f(1, 1, 1, 1);

		glEnable(GL_TEXTURE_2D);

		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_DEPTH_TEST);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glPushMatrix();

		glRotatef(ro_y, 1, 0, 0);
		glRotatef(ro_x, 0, 1, 0);

		float R = 1;

		glBindTexture(GL_TEXTURE_2D, tex);
		float radius = 0.5f;

		if (stitchingMode == WIDE_STITCHING) {
			glBegin(GL_QUAD_STRIP);
			for (int j = 0; j < 360; j += 3) {
				glTexCoord2f(texCoordS_side[j], texCoordT_side[j]);
				glNormal3f(-radius*coordX1_side[j], radius*coordY1_side[j], -radius*coordZ1_side[j]);
				glVertex3f(-radius*coordX1_side[j], radius*coordY1_side[j], -radius*coordZ1_side[j]);

				glTexCoord2f(texCoordS_side[j], texCoordT2_side[j]);
				glNormal3f(-radius*coordX2_side[j], radius*coordY2_side[j], -radius*coordZ2_side[j]);
				glVertex3f(-radius*coordX2_side[j], radius*coordY2_side[j], -radius*coordZ2_side[j]);

			}
			glEnd();
		}
		else
		{
			for (int lon = 0; lon < 180; lon += 3) {
				glBegin(GL_TRIANGLE_STRIP);
				for (int lat = 0; lat <= 360; lat += 3) {

					glTexCoord2f(texCoordS[lat], texCoordT[lon]);
					glNormal3f(-coordX1[lon][lat], -coordY1[lon], coordZ1[lon][lat]);
					glVertex3f(-coordX1[lon][lat], -coordY1[lon], coordZ1[lon][lat]);

					glTexCoord2f(texCoordS[lat], texCoordT2[lon]);
					glNormal3f(-coordX2[lon][lat], -coordY2[lon], coordZ2[lon][lat]);
					glVertex3f(-coordX2[lon][lat], -coordY2[lon], coordZ2[lon][lat]);
				}
				glEnd();
			}
		}

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);
		glPopMatrix();
		glFlush();
		glFinish();
	}

	void getKey(int key) {
		if (key == 'q') {
			fov += 0.5;

			if (fov > 120)
				fov = 120;
		}
		else if (key == 'e') {
			fov -= 0.5;

			if (fov < 10)
				fov = 10;
		}
		else if (key == 'w') {
			ro_y -= 2;
		}
		else if (key == 's') {
			ro_y += 2;
		}
		else if (key == 'a') {
			ro_x -= 2;
		}
		else if (key == 'd') {
			ro_x += 2;
		}
	}

	void setPitch(float a)
	{
		ro_y += (a*fov);
	}
	void setPan(float a)
	{
		ro_x += (a*fov);
	}
	void setZoom(float a)
	{
		fov += a;
		if (fov>175) fov = 175;
		//if (fov<60) fov = 60;
		if (fov<20) fov = 20;
	}
	void mulPitch(float a)
	{
		ro_y = ro_y*a;
	}
};