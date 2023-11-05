#pragma once

#include "CamInput/CamInputArray.h"
#include "Composer/Composer.h"

//#include <FL/Fl.h>
//#include <FL/x.H>
#ifdef WIN32
//#pragma comment (lib, "fltkdll.lib")

#include <GL/Glew.h>
#include <GL/wGlew.h>
#pragma comment (lib, "opengl32.lib")
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
//#include <FL/gl.h>
#include "clock.h"
#include <opencv2/highgui/highgui.hpp>

//#include "Calibrator.h"
#include "StitchingTemplate.h"


struct Stitcher {
	bool glewInited = false;

	Stitcher() {
		sprintf_s(ttt, 1024, "% 5.1f FPS % 5.1f ms", 0, 0);
	};

	CamInputArray* inputArray = NULL;
	Composer* composer = NULL;
	const int SAMPLE = 0;
	int numCamera = 0;

	char ttt[1024];

	StitchingTemplate *stitchingTemplate;

	void createStitchingTemplate(int outWdt, int outHgt, int inputWdt, int inputHgt);
	void deleteStitchingTemplate();

	void create(int wdt, int hgt, int numCamera, int inputWdt, int inputHgt);
	void stitching(int w, int h, int* pbo_idx, int** input_state, InputTempData* input_tempData, int panorama_preview_mode, float currentScale, float currentX, float currentY, float animation_time, int stereoIndex, int editMode = STITCHING_MODE, int stitching_mode = 0);

	int startOutput(int outputDevice, int outputDisplay);
	int finishOutput();

	int updateNadirImage(const char* nadir_path);

	void updateStitchingTemplate(bool isDirty=true);
	void updateOverlay(int id);

	void updateDirty(bool isDirty);

#ifdef USE_XML
	void init();
#else
	void init(PTSParam nadir_param, const char* nadir_path, int num_camera, int input_width, int input_height, int stitching_mode);
#endif
	void* getPBOAddress(int camera_idx, int buffer_idx);
	void makeSeamMask(InputTempData *input_tempData, long long timeStame, int* buffer_idx, int edit_mode);
	//void calculatePixelAvg(Calibrator *calibrator, int* buffer_idx);
	void estimateExposureCompensation(InputTempData *input_tempData, int* buffer_idx, int stereoIdx = -1);
	std::vector<cv::Mat> getCurve(InputTempData *input_tempData, long long timeStamp, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints);
	void setBlendingMode(int mode);
	void setSeamMode(int mode);
	void setGainMode(int mode);
	void setNadirCompensationMode(int mode);
	void updateNadirParam(PTSParam nadir_param);
	void setManualRGB(int idx, int R, int G, int B);

	// bigheadk for calibration
	void updateWarpMap(WarpInfo* warp_info);

	void getOverlayCenter(int id, float* cx, float* cy);

	void addOverlayInput(int width, int height, int depth, int id, int tplNum, PTSParam* param = NULL, int outputType = EQUIRECTANGULAR);
	void addOverlayInput(int width, int height, int depth, int id, void* ptr, int tplNum, PTSParam* param = NULL, int outputType = EQUIRECTANGULAR);
	void modifyOverlayInput(int width, int height, int depth, int id, int targetID, int tplNum);
	void modifyOverlayInput(int width, int height, int depth, int id, int targetID, void* ptr, int tplNum);
	void destroyOverlayInput(int id);
	void savePanoramaImage(char* path);
	void* getOverlayPBOAddress(int idx);

	void destroy();
	int selectOverlay(int x, int y, int tplNum);
	void setOverlayState(int state, int id);

	void setGrayscaleMode(int mode);
	void setBrightnessStep(int step);
	void setContrastStep(int step);
	void setStereoMode(int mode);
	void setPrepareMode(bool mode);

	void setCallback_setOutputLog(void* cb, void* data);
};