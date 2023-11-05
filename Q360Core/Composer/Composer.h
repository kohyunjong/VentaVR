

#ifndef __COMPOSER_QURAM_H__
#define __COMPOSER_QURAM_H__

#include "../Quram360Engine.h"
#include "CamInput/CamInputArray.h"
#include "ComposeHelper.h"

#include <opencv2/core/core.hpp>
#include "../StitchingMode.h"

//#include "../Calibrator.h"
#include "../InputTempData.h"
#include "GLBase/glPyramid.h"

/* NVENC */

#include <cuda_runtime.h>
#include <cuda_gl_interop.h>
#include <process.h>

#include "Capture/BMOutput.h"
#include "../WarpInfo.h"

#include "../ExposureCompensation.h"
#include "../ExposureCompensator.h"


//#include "Optimizer/optimize_main.h"


#define INIT_REQUEST_RESTREAMING		0x40
#define NO_REQUEST_RESTREAMING			0x41
#define REQUEST_RESTREAMING				0x42

#define NUM_ENCODE_BUFFER	4
/* NVENC end*/

#define MAX_RESTARTSTREAMINGCOUNT	120

#define PYR_DEPTH 8

#define MULTIBAND_PADDING	1.25

struct OverlayCamInput
{
	CamInput	*caminput;
	ComposeHelper* helper;
	ComposeHelper* right_helper;	// for stereo
	int			id;
	int			state;
	int			templateNum;

	float		opacity;

	PTSParam	ptsparam;
};

struct Composer {
	bool glInited;
	int width, height;
	//int output_width, output_height;
	CamInputArray *input;
	CamInput	*nadir;
	//std::vector<OverlayTemplate*>	overlayTemplates;
	std::vector<OverlayCamInput*>	overlayInputs;
	std::vector<ComposeHelper*> helpers;
	ComposeHelper* nadir_helper;
	//std::vector<ComposeHelper*> overlayHelpers;


	ExposureCompensator* exposureCompensator;
	const char* blendType = "";

	//void audio_push(unsigned char *data, int size, int output_mode);

	Composer(int w, int h) : glInited(false), width(w), height(h) {
		nadir_helper = 0;
	}

	virtual GLuint getResTex() = 0;
	virtual void initGL() = 0;
	virtual void refresh(int** input_state, float animation_time, int stereoIndex, int editMode = STITCHING_MODE, int stitching_mode = false) = 0;
	//virtual void refresh() = 0;
	virtual void addOverlayInput(int width, int height, int depth, int id, int tplNum, PTSParam* param = NULL, int outputType = EQUIRECTANGULAR) = 0;
	virtual void addOverlayInput(int width, int height, int depth, int id, void* ptr, int tplNum, PTSParam* param = NULL, int outputType = EQUIRECTANGULAR) = 0;
	virtual void modifyOverlayInput(int width, int height, int depth, int id, int targetID, int tplNum) = 0;
	virtual void modifyOverlayInput(int width, int height, int depth, int id, int targetID, void* ptr, int tplNum) = 0;
	virtual void destroyOverlayInput(int id) = 0;
	virtual int selectOverlay(int x, int y, int tplNum) = 0;
	virtual void updateOverlayInput(int id) = 0;
	virtual void setOverlayState(int state, int id) = 0;
	virtual void createHelpers(int editMode = STITCHING_MODE) = 0;
	virtual void createNadirHelpers() = 0;
	virtual void getOverlayCenter(int id, float* cx, float* cy) = 0;

	void setInput(CamInputArray* inp) {
		input = inp;
		createHelpers();
	}
	void createNadir(PTSParam param, const char* nadir_path, int input_type)
	{
		nadir = CamInput::createNadir(param, nadir_path, NADIR_WIDTH, NADIR_HEIGHT, input_type);
		createNadirHelpers();
	}
	void updateNadir(PTSParam param)
	{
		nadir->setPTSParam(param);
	}
	int updateNadirImage(const char* nadir_path)
	{
		return ((CamInputNadir*)nadir)->updateNadir(nadir_path);
	}
	inline bool isGLInited(void) { return glInited; }

	void* getOverlayPBOAddress(int idx);

#ifdef USE_XML
	static Composer* create(const pugi::xml_node& node);
#else
	static Composer* create(int numCamera, int outputWidth, int outputHeight);
#endif
};

struct WarpComposer : public Composer {
	WarpComposer(int w, int h) :Composer(w, h){}
	ComposeHelper& helper(size_t i)
	{
		while (i >= helpers.size());

		return *((ComposeHelper*)helpers[i]);
	}

	virtual void createHelpers(int editMode = STITCHING_MODE);
	virtual void createNadirHelpers();

	virtual void addOverlayInput(int width, int height, int depth, int id, int tplNum, PTSParam* param = NULL, int outputType = EQUIRECTANGULAR);
	virtual void addOverlayInput(int width, int height, int depth, int id, void* ptr, int tplNum, PTSParam* param = NULL, int outputType = EQUIRECTANGULAR);
	virtual void modifyOverlayInput(int width, int height, int depth, int id, int target_id, int tplNum);
	virtual void modifyOverlayInput(int width, int height, int depth, int id, int target_id, void* ptr, int tplNum);
	virtual void destroyOverlayInput(int id);
	virtual void updateOverlayInput(int id);
	virtual int selectOverlay(int x, int y, int tplNum);
	virtual void setOverlayState(int state, int id);
	virtual void getOverlayCenter(int id, float* cx, float* cy);

	void minDistMask(size_t i, size_t w, size_t h, unsigned char* buf);
	void dilate(size_t w, size_t h, const unsigned char* buf, unsigned char* dst);
	void blur(size_t w, size_t h, const unsigned char* buf, unsigned char* dst);
	virtual void add(size_t k, long long timeStamp, float animation_time, bool isEditMode = false, int stitching_mode = false) = 0;
	virtual void refresh(int** input_state, float animation_time, int stereoIndex, int editMode = STITCHING_MODE, int stitching_mode = false);
	void updateTexure(InputTempData* input_tempData, int** input_state);

	int stitchingMode = 0;
	bool is360 = true;
};



struct StitchComposer : public WarpComposer {
#if LICENSE_PERSONAL_KEY
	GLuint watermarkTex;
	unsigned char* watermark_buffer;
#endif
	GLuint tmpFBO, tmpTex, resFBO, resTex, warpFBO, warpTex;
	GLuint outFBO, outTex;
	GLPyramid cum, merger;		// for Laplacian
	GLPyramid **buf;

	GLuint gridTex;

	GLuint BMOutPBO[4];		// for Black Magic Output
	void* BMPtr[4];

	int BMReadindex = 0;
	int BMWriteindex = 0;

	int BMWdt = 0;
	int BMHgt = 0;

	int BMResolutionMode = 0;

	int gainRenderingMode = 0;

	int outputWidth = 0, outputHeight = 0;
	int mNumCamera;

	float contrast_step = 1.0;
	float brightness_step = 0;
	int grayscale_mode = GRAYSCALE_OFF;

	char* pano_file_path;
	//GLuint testPBO[4];
	//void* testPtr[4];
	//int testReadindex = 0;
	//int testWriteindex = 0;
	float *colors;

	bool mPrepareMode = false;

	StitchComposer(int w, int h) : WarpComposer(w, h),
		cum(w*MULTIBAND_PADDING, h, PYR_DEPTH, GL_RGBA16F), merger(w*MULTIBAND_PADDING, h, PYR_DEPTH, GL_RGBA), buf(0), tmpFBO(0), resFBO(0), resTex(0), outFBO(0), outTex(0), gridTex(0) {
		blendMode = NORMAL_BLEND;
		//blendMode = SIMPLE_BLEND;
		seamMode = FIND_SEAM_FROM_IMAGE;
		grayscale_mode = GRAYSCALE_OFF;
		//seamMode = FIND_SEAM_FROM_DIST;
		//warpMode = FULL_FRAME_TO_EQUIRECT;
		//nadirMode = NADIR_LOG_OFF;

		gainMode = NO_GAIN_COMPENSATION;
		//gainDiffMode = GAIN_AVERAGE_COMPENSATION_HALF;//GAIN_AVERAGE_COMPENSATION_FULL;
		availableSound = NO_AVAILABLE_SOUND;
		stitching_resolution_mode = STITCHING_2BY1;
		isSave = false;
		mPrepareMode = false;
		pano_file_path = NULL;

		outputWidth = w;
		outputHeight = h;

		colors = (float*)malloc(sizeof(float)* 4);

		SetThreadAffinityMask(GetCurrentThread(), 1);
		//SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_HIGHEST);

		mpCallback = NULL;
		mpCallbackData = NULL;
	}

	int blendMode;
	int seamMode;
	//int warpMode;
	//int nadirMode;
	int gainMode;
	int autoMode = 0;
	//int gainDiffMode;

	//CFileStreaming *m_fileStreming;
	//std::function<void()> pOnVodStreamingFinishedCallback;
	//void SetCallback_onVodStreamingFinished(std::function<void()> func);
	void onStreamingFinished_FS();


	int isEncoding;
	double fps_encoder;

	bool isSetAudioHeader_streaming;
	int isStreaming;
	double fps_streaming;

	int stitchingCount;
	int restartTimer;

	int isRequestReStreaming;

	int isOutput;
	int availableSound;

	int stitching_resolution_mode;
	bool isSave;
	//int isStereo = 0;

	virtual GLuint getResTex();
	virtual void add(size_t k, long long time, float animation_time, bool isEditMode = false, int stitching_mode = false);
	virtual void refresh(int** input_state, float animation_time, int stereoIndex, int editMode = STITCHING_MODE, int stitching_mode = false);
	//virtual void add(size_t k);
	//virtual void refresh();
	virtual void initGL();

	GLuint getOutTex();

	void setDepth(int d);		// exposure compensation
	void setSeamMode(int mode);
	void setGainMode(int mode);
	bool makeSeamMask(PTSParam* pts_param, InputTempData* input_tempData, int* buffer_idx, long long time_stamp, int mode);
	//void blendImages(std::vector<cv::Point> corners, std::vector<cv::Mat> images, std::vector<cv::Mat> masks);
	//int startEncoding(double fps);
	//int startEncoding(encOpts eo);
	//int finishEncoding();

	////int startStreaming(double fps);
	//int startStreaming(encOpts eo);
	//int finishStreaming();

	//int restartStreaming();

	int startOutput(int outputDevice, int outputDisplay);
	int finishOutput();

	void destroyEncoding();
	void initEncoder(double fps);

	void destroyStreaming();
	void initStreaming(double fps);

	void initOutput(int *isEncoding, int *isStreaming, int stitching_resolution_mode);
	void destroyOutput();

	void setNadirMode(int mode);

	void setStitchingResolutionMode(int mode);

	bool estimateExposureCompensation(InputTempData *input_tempData, int* buffer_idx, PTSParam* pts_param, int stereoIdx = -1);
	std::vector<cv::Mat> getCurve(InputTempData *input_tempData, long long timeStamp, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints);

	//
	//float* setRenderingInfo(int k, long long timeStamp);
	void setRenderingInfo(int k, long long timeStamp);

	void setManualRGB(int idx, int R, int G, int B);

	void blendImages(std::vector<cv::Point> corners, std::vector<cv::Mat> images, std::vector<cv::Mat> masks);

	// bigheadk for Calibration
	void updateWarpMap(WarpInfo* warp_info);

	void savePanoramaImage(char* path);

	void destroyInputColors();
	void destroy();

	void setGrayscaleMode(int mode);
	void setBrightnessStep(int step);
	void setContrastStep(int step);
	void setStereoMode(int mode);

	void setPrepareMode(bool mode);

	void setCallback_setOutputLog(void* cb, void* data);
	void* mpCallback;
	void* mpCallbackData;
};

//class CNvEncManager
//{
//public:
//	void init(encOpts eo, int mode);		// mode = HDD, RTMP...
//	void setEncBufIndex(int idx);
//	void startEncodeFrameThread();
//	void finish(int mode);
//
//	bool isPushAble(int level);
//
//	encOpts m_encOpts;
//	CNvEncoderCudaInterop *m_nvEncoder;
//
//	bool bEncodeThreadFinish;
//
//	double m_timer;
//	int m_bufIndex;
//
//	int m_nv_encode_format;
//
//	unsigned long m_nvEncThread;
//	unsigned int m_nvEncThreadId;
//
//	static void setOutputLog(char* log, int size, bool isSave);
//
//	int *m_isEncoding;
//	int *m_isStreaming;
//private:
//
//	int m_stride;
//
//	void *m_yuv420StrideBuffer_Y;
//	void *m_yuv420StrideBuffer_U;
//	void *m_yuv420StrideBuffer_V;
//
//};

class COutputManager
{
public:

	COutputManager(int width, int height, int *isEncoding, int *isStreaming);
	~COutputManager();
	//void init(int width, int height, int *isEncoding, int *isStreaming);		// mode = HDD, RTMP...

	int getResolutionMode();
	void setResolutionMode(int mode);
	void setStereo(int mode);

	typedef void*(*callback_setOutputLog)(void* data, char* log, int size, bool isSave);
	callback_setOutputLog mpSetOutputLogCallback;
	void* mpSetOutputLogCallbackData;
	void setCallback_setOutputLog(void* cb, void* data);

private:
	int m_width;
	int m_height;

	int m_buffer_in_idx;
	int m_buffer_out_idx;

	int *m_isEncoding;
	int *m_isStreaming;
	int m_isStereo;

	GLuint nv_glColor;
	GLuint nv_glColorBuffer;
	struct cudaGraphicsResource *nv_cuColorRes;
	//void *s_cuColorBuffer;

	int resolution_mode;
	bool resourceMapped = FALSE;
};

#endif
