#include "Composer.h"
#include "GLbase/glProg.h"
#include "clock.h"

#define SEAM_SCALE 8
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../SeamFinder.h"

#include "GLBase/glsl.h"

#include <opencv2/stitching/detail/blenders.hpp>

//	NVENC start

#define WRITE_LOG	0
#define SHOW_LOG	0

extern "C" void cu_RGBtoYUV444(unsigned char *yuv, unsigned char *pData, int width, int height);
extern "C" void cu_RGBtoYUV420Pitch(unsigned char *bufY, unsigned char *bufU, unsigned char *bufV, unsigned char *pData, int stride, int width, int height);
extern "C" void cu_UVtoNV12(unsigned char *pOut, unsigned char *bufU, unsigned char *bufV, int stride, int width, int height);
extern "C" void cu_customCudaMemcpy(unsigned char *yuv, unsigned char *pData, int width, int height, int bytes);
extern "C" void cu_RGBtoNV12(unsigned char *pDest, unsigned char *pSrc, int stride, int width, int height);

COutputManager *m_outputManager;
BMOutput *m_BMOutput;

//cudaArray_t g_RGBBuffer[NUM_ENCODE_BUFFER];
void *g_RGBBuffer[NUM_ENCODE_BUFFER];
void *s_cuColorBuffer;
bool cudaCheckError(cudaError_t e);

//unsigned int __stdcall nvEncodeFrameThreadFunc(void* param);
//unsigned int __stdcall nvStreamingThreadFunc(void* param);

//	NVENC end
boolean end_writing_container;

void fillRect(unsigned char* b, int w, int h, int off, int val) {
	for (int y = off; y<h - off; y++) for (int x = off; x<w - off; x++) b[y*w + x] = val;
}

void drawROIRect(const ComposeHelper& hlp, int w) {
	int rx = hlp.roix, rw = (int)(hlp.roiw + 0.999999f);
	int ry = hlp.roiy, rh = (int)(hlp.roih + 0.999999f);
	if (rx >= 0 && rx + rw <= w) {
		glScissor(rx, ry, rw, rh);
		drawRect();
	}
	if (rx<0) {
		glScissor(rx + w, ry, w - rx, rh);
		drawRect();
		glScissor(0, ry, rw + rx, rh);
		drawRect();
	}
	// Todo following case need more test
	if (rx + rw > w) {
		glScissor(rx, ry, w - rx, rh);
		drawRect();
		glScissor(0, ry, rw + rx - w, rh);
		drawRect();
	}
}

bool containROI(int w, int h, cv::Rect roi, cv::Point p) {
	int rx = roi.x;
	int rw = roi.width;

	int ry = roi.y;
	int rh = roi.height;

	if (rx >= 0 && rx + rw < w) {
		if (roi.contains(p))
			return true;
	}
	if (rx<0) {
		cv::Rect t = cv::Rect(rx + w, ry, -rx, rh);
		if (t.contains(p))
			return true;
		t.x = 0;
		t.y = ry;
		t.width = rw + rx;
		t.height = rh;

		if (t.contains(p))
			return true;
	}
	// Todo following case need more test
	if (rx + rw >= w) {

		cv::Rect t = cv::Rect(rx, ry, w - rx, rh);
		if (t.contains(p))
			return true;
		t.x = 0;
		t.y = ry;
		t.width = rw + rx - w;
		t.height = rh;

		if (t.contains(p))
			return true;
	}
	return false;
}

#ifdef USE_XML
Composer* Composer::create(const pugi::xml_node& node){
	Composer* cmpr = 0;
	const char* type = node.attribute("type").value();
	cmpr = new StitchComposer(3840, 1920);
	cmpr->blendType = type;
	return cmpr;
}
#else
Composer* Composer::create(int numCamera, int outputWidth, int outputHeight){
	Composer* cmpr = 0;
	cmpr = new StitchComposer(outputWidth, outputHeight);
	cmpr->blendType = "blend";

	((StitchComposer*)cmpr)->mNumCamera = numCamera;
	printf("bigheadk, create Composer\n");
	cmpr->exposureCompensator = new ExposureCompensator(outputWidth, outputHeight);
	cmpr->exposureCompensator->initCurveInfo(numCamera);

	return cmpr;
}
#endif

void WarpComposer::createHelpers(int editMode) {
	for (size_t i = 0; i<helpers.size(); i++) if (helpers[i]) delete helpers[i];
	helpers.clear();
	int mode = COMPOSEHELPER_ROI;
	if (editMode == STITCHING_EDIT_MODE)
	{
		mode = COMPOSEHELPER_EDIT;
	}
	for (int i = 0; i<(int)input->count(); i++) {
		ComposeHelper* helper = new ComposeHelper((*input->camIn(i)), width, height, mode);
		helpers.push_back(helper);
	}
}

void WarpComposer::createNadirHelpers() {
	if (nadir_helper)
		delete nadir_helper;
	nadir_helper = 0;

	printf("set Nadir\n" );
	nadir_helper = new ComposeHelper(*nadir, width, height, COMPOSEHELPER_ROI);
}

void WarpComposer::modifyOverlayInput(int wdt, int hgt, int depth, int id, int targetID, int tplNum) {
	PTSParam param, temp_param;
	for (int i = 0; i < overlayInputs.size(); i++){
		if (overlayInputs[i] != NULL && overlayInputs[i]->id == targetID)
			temp_param = overlayInputs[i]->ptsparam;
	}
	param.f = temp_param.f;

	param.a = temp_param.a;
	param.b = temp_param.b;
	param.c = temp_param.c;

	param.y = temp_param.y;
	param.p = temp_param.p;
	param.r = temp_param.r;

	param.diff_pitch = temp_param.diff_pitch;
	param.diff_yaw = temp_param.diff_yaw;

	param.v = temp_param.v;
	param.w = wdt;
	param.h = hgt;

	param.left = 0;
	param.right = wdt;
	param.top = 0;
	param.bottom = hgt;
	param.out_f = temp_param.out_f;
	param.out_fov = temp_param.out_fov;

	CamInput* cam = CamInput::create(param, wdt, hgt, depth, NOT_FOR_360_INPUT, (int)0);
	OverlayCamInput* overlayinput = new OverlayCamInput();
	overlayinput->caminput = cam;
	overlayinput->id = id;

	overlayinput->ptsparam = param;
	overlayinput->helper = NULL;
	overlayinput->right_helper = NULL;
	overlayinput->templateNum = tplNum;
	overlayinput->opacity = 0.0f;
	overlayInputs.push_back(overlayinput);

	cam->dirty = true;
	cam->state = INPUT_STATE_NO_DRAW;
	cam->initGL();
	cam->connect();
}

void WarpComposer::modifyOverlayInput(int wdt, int hgt, int depth, int id, int targetID, void* ptr, int tplNum) {
	PTSParam param, temp_param;
	for (int i = 0; i < overlayInputs.size(); i++){
		if (overlayInputs[i] != NULL && overlayInputs[i]->id == targetID)
			temp_param = overlayInputs[i]->ptsparam;
	}
	param.f = temp_param.f;

	param.a = temp_param.a;
	param.b = temp_param.b;
	param.c = temp_param.c;

	param.y = temp_param.y;
	param.p = temp_param.p;
	param.r = temp_param.r;

	param.diff_pitch = temp_param.diff_pitch;
	param.diff_yaw = temp_param.diff_yaw;

	param.v = temp_param.v;
	param.w = wdt;
	param.h = hgt;

	param.left = 0;
	param.right = wdt;
	param.top = 0;
	param.bottom = hgt;
	param.out_f = temp_param.out_f;
	param.out_fov = temp_param.out_fov;

	CamInput* cam = CamInput::create(param, wdt, hgt, depth, NOT_FOR_360_INPUT, ptr);
	OverlayCamInput* overlayinput = new OverlayCamInput();
	overlayinput->caminput = cam;
	overlayinput->id = id;

	overlayinput->ptsparam = param;
	overlayinput->helper = NULL;
	overlayinput->right_helper = NULL;
	overlayinput->templateNum = tplNum;
	overlayinput->opacity = 0.0f;
	overlayInputs.push_back(overlayinput);

	cam->dirty = true;
	cam->state = INPUT_STATE_NO_DRAW;
}

void WarpComposer::addOverlayInput(int wdt, int hgt, int depth, int id, int tplNum, PTSParam* param, int outputType) {
	//PTSParam param;
	PTSParam tempParam;
	if (param != NULL)
	{
		param->f = RECTLINEAR;
		param->out_f = outputType;
		if (outputType == WIDE_PANORAMA || outputType == STEREO)
			param->out_fov = 180.f;
		else
			param->out_fov = 360.f;
		param->w = wdt;
		param->h = hgt;

		param->left = 0;
		param->right = wdt;
		param->top = 0;
		param->bottom = hgt;
	}
	else
	{
		tempParam.f = RECTLINEAR;
		tempParam.out_f = outputType;
		if (outputType == WIDE_PANORAMA) {
			tempParam.out_fov = 180.f;
			tempParam.v = 30.f;
		}
		else if (outputType == STEREO){
			tempParam.out_fov = 180.f;
			tempParam.v = 60.f;
		}
		else {
			tempParam.out_fov = 360.f;
			tempParam.v = 60.f;
		}

		tempParam.a = 0;
		tempParam.b = 0;
		tempParam.c = 0;

		tempParam.y = 0;
		tempParam.p = 0;
		tempParam.r = 0;

		tempParam.diff_pitch = 0;
		tempParam.diff_yaw = 0;

		tempParam.v = 60;
		/*if (depth == 4){ param.v = wdt / 12; }
		else{ param.v = 60; }*/
		tempParam.w = wdt;
		tempParam.h = hgt;

		tempParam.left = 0;
		tempParam.right = wdt;
		tempParam.top = 0;
		tempParam.bottom = hgt;
	}

	CamInput* cam;

	if (param != NULL)
		cam = CamInput::create(*param, wdt, hgt, depth, NOT_FOR_360_INPUT, (int)0);
	else
		cam = CamInput::create(tempParam, wdt, hgt, depth, NOT_FOR_360_INPUT, (int)0);

	OverlayCamInput* overlayinput = new OverlayCamInput();
	overlayinput->caminput = cam;
	overlayinput->id = id;

	if (param != NULL)
		overlayinput->ptsparam = *param;
	else
		overlayinput->ptsparam = tempParam;

	overlayinput->helper = NULL;
	overlayinput->right_helper = NULL;
	overlayinput->templateNum = tplNum;
	overlayinput->opacity = 0.0f;
	overlayInputs.push_back(overlayinput);

	cam->dirty = true;
	cam->state = INPUT_STATE_NO_DRAW;
	cam->initGL();
	cam->connect();
}

void WarpComposer::addOverlayInput(int wdt, int hgt, int depth, int id, void* ptr, int tplNum, PTSParam* param, int outputType) {
	//PTSParam param;
	PTSParam tempParam;
	if (param != NULL)
	{
		param->f = RECTLINEAR;
		param->out_f = outputType;
		if (outputType == WIDE_PANORAMA || outputType == STEREO)
			param->out_fov = 180.f;
		else
			param->out_fov = 360.f;
		param->w = wdt;
		param->h = hgt;

		param->left = 0;
		param->right = wdt;
		param->top = 0;
		param->bottom = hgt;
	}
	else
	{
		tempParam.f = RECTLINEAR;
		tempParam.out_f = outputType;
		if (outputType == WIDE_PANORAMA) {
			tempParam.out_fov = 180.f;
			tempParam.v = 30.f;
		}
		else if (outputType == STEREO){
			tempParam.out_fov = 180.f;
			tempParam.v = 60.f;
		}
		else {
			tempParam.out_fov = 360.f;
			tempParam.v = 60.f;
		}
		tempParam.a = 0;
		tempParam.b = 0;
		tempParam.c = 0;

		tempParam.y = 0;
		tempParam.p = 0;
		tempParam.r = 0;

		tempParam.diff_pitch = 0;
		tempParam.diff_yaw = 0;

		tempParam.v = 60;
		/*if (depth == 4){ param.v = wdt / 12; }
		else{ param.v = 60; }*/
		tempParam.w = wdt;
		tempParam.h = hgt;

		tempParam.left = 0;
		tempParam.right = wdt;
		tempParam.top = 0;
		tempParam.bottom = hgt;
	}

	CamInput* cam;

	if (param != NULL)
		cam = CamInput::create(*param, wdt, hgt, depth, NOT_FOR_360_INPUT, ptr);
	else
		cam = CamInput::create(tempParam, wdt, hgt, depth, NOT_FOR_360_INPUT, ptr);

	OverlayCamInput* overlayinput = new OverlayCamInput();
	overlayinput->caminput = cam;
	overlayinput->id = id;

	if (param != NULL)
		overlayinput->ptsparam = *param;
	else
		overlayinput->ptsparam = tempParam;

	overlayinput->helper = NULL;
	overlayinput->right_helper = NULL;
	overlayinput->templateNum = tplNum;
	overlayinput->opacity = 0.0f;
	overlayInputs.push_back(overlayinput);

	cam->dirty = true;
	cam->state = INPUT_STATE_NO_DRAW;
}
void WarpComposer::getOverlayCenter(int id, float* cx, float* cy)
{
	*cx = -1.0f;
	*cy = -1.0f;
	for (int i = 0; i < overlayInputs.size(); i++)
	{
		if (overlayInputs[i]->id == id)
		{
			cv::Rect crop_rect(0, 0, overlayInputs[i]->caminput->width, overlayInputs[i]->caminput->height);
			overlayInputs[i]->helper->getOutputCoord(overlayInputs[i]->caminput->width, overlayInputs[i]->caminput->height, width, height, overlayInputs[i]->caminput->width / 2, overlayInputs[i]->caminput->height / 2, cx, cy, crop_rect);
			//*cx = overlayInputs[i]->helper->roix + overlayInputs[i]->helper->roiw / 2.0f;
			//*cy = overlayInputs[i]->helper->roiy + overlayInputs[i]->helper->roih / 2.0f;
			break;
		}
	}
}

void WarpComposer::setOverlayState(int state, int id)
{
	printf("setOverlayState %d, %d\n", state, id);
	for (int i = 0; i < overlayInputs.size(); i++)
	{
		printf("overlayInputs[i]->id %d, %d\n", overlayInputs[i]->id, id);
		if (overlayInputs[i]->id == id)
		{
			CamInput* p = (CamInput*)overlayInputs[i]->caminput;
			p->state = state;
			break;
		}
	}
}

int WarpComposer::selectOverlay(int x, int y, int tplNum)
{
	if (stitchingMode == STEREO_STITCHING_TB)
		y *= 2;
	else if (stitchingMode == STEREO_STITCHING_LR)
		x *= 2;

	for (int i = overlayInputs.size() - 1; i >= 0; i--)
	{
		int dist = overlayInputs[i]->helper->CalcDist(overlayInputs[i]->caminput->width, overlayInputs[i]->caminput->height, x, y);
		if (dist != INT_MAX && overlayInputs[i]->templateNum == tplNum)
			return overlayInputs[i]->id;
	}
}

void WarpComposer::updateOverlayInput(int id) {
	for (int i = 0; i < overlayInputs.size(); i++)
	{
		if (overlayInputs[i]->id == id)
		{
			overlayInputs[i]->caminput->setPTSParam(overlayInputs[i]->ptsparam);
			overlayInputs[i]->caminput->dirty = true;
			break;
		}
	}
}

void WarpComposer::destroyOverlayInput(int id)
{
	for (int i = 0; i < overlayInputs.size(); i++)
	{
		if (overlayInputs[i]->id == id)
		{
			overlayInputs[i]->caminput->clear();

			if (overlayInputs[i]->helper)
				delete overlayInputs[i]->helper;

			if (overlayInputs[i]->right_helper)
				delete overlayInputs[i]->right_helper;

			overlayInputs[i]->helper = NULL;
			overlayInputs[i]->right_helper = NULL;

			overlayInputs.erase(overlayInputs.begin() + i);
			break;
		}
	}
}

void WarpComposer::dilate(size_t w, size_t h, const unsigned char*buf, unsigned char* dst) {
	for (unsigned long y = 1; y<h - 1; y++)for (unsigned long x = 1; x<w - 1; x++) {
		int v = buf[y*w + x] + buf[y*w + x + 1] + buf[y*w + x - 1] + buf[(y + 1)*w + x] + buf[(y + 1)*w + x - 1] + buf[(y + 1)*w + x + 1]
			+ buf[(y - 1)*w + x] + buf[(y - 1)*w + x - 1] + buf[(y - 1)*w + x + 1];
		if (v>1) dst[y*w + x] = 255;
		else dst[y*w + x] = 0;
	}
	{ unsigned long y = 0; for (unsigned long x = 1; x<w - 1; x++) {
		int v = buf[y*w + x] + buf[y*w + x + 1] + buf[y*w + x - 1] + buf[(y + 1)*w + x] + buf[(y + 1)*w + x - 1] + buf[(y + 1)*w + x + 1];
		if (v>1) dst[y*w + x] = 255;
		else dst[y*w + x] = 0;
	}}
	{ unsigned long y = h - 1; for (unsigned long x = 1; x<w - 1; x++) {
		int v = buf[y*w + x] + buf[y*w + x + 1] + buf[y*w + x - 1] + buf[(y - 1)*w + x] + buf[(y - 1)*w + x - 1] + buf[(y - 1)*w + x + 1];
		if (v>1) dst[y*w + x] = 255;
		else dst[y*w + x] = 0;
	}}
	for (unsigned long y = 1; y<h - 1; y++) {
		unsigned long x = 0;
		int v = buf[y*w + x] + buf[y*w + x + 1] + buf[(y + 1)*w + x] + buf[(y + 1)*w + x + 1]
			+ buf[(y - 1)*w + x] + buf[(y - 1)*w + x + 1];
		if (v>1) dst[y*w + x] = 255;
		else dst[y*w + x] = 0;
	}
	for (unsigned long y = 1; y<h - 1; y++) {
		unsigned long x = w - 1;
		int v = buf[y*w + x] + buf[y*w + x - 1] + buf[(y + 1)*w + x] + buf[(y + 1)*w + x - 1]
			+ buf[(y - 1)*w + x] + buf[(y - 1)*w + x - 1];
		if (v>1) dst[y*w + x] = 255;
		else dst[y*w + x] = 0;
	}
}
void WarpComposer::blur(size_t w, size_t h, const unsigned char*buf, unsigned char* dst) {
	for (unsigned long y = 1; y<h - 1; y++)for (unsigned long x = 1; x<w - 1; x++) {
		int v = buf[y*w + x] + buf[y*w + x + 1] + buf[y*w + x - 1] + buf[(y + 1)*w + x] + buf[(y + 1)*w + x - 1] + buf[(y + 1)*w + x + 1]
			+ buf[(y - 1)*w + x] + buf[(y - 1)*w + x - 1] + buf[(y - 1)*w + x + 1];
		dst[y*w + x] = v / 9;
	}
}

void WarpComposer::refresh(int** input_state, float animation_time, int stereoIndex, int editMode, int stitching_mode) {
	if (!input) return;
	if (stitching_mode != STITCHED_INPUT)
	{
		if (input->isDirty()) {
			createHelpers(editMode);
			input->setUndirty();
		}
	}
	if (nadir->dirty)
	{
		createNadirHelpers();
		nadir->dirty = false;
	}
	//printf("refresh %d\n", overlayInputs.size());
	

	if (overlayInputs.size() > 0)
	{
		int mode = COMPOSEHELPER_REGION;
		if (editMode == STITCHING_EDIT_MODE)
		{
			mode = COMPOSEHELPER_EDIT;
		}

		for (int i = 0; i < overlayInputs.size(); i++)
		{
			if (overlayInputs[i]->caminput->dirty)
			{
				
				if (overlayInputs[i]->helper == NULL)
				{
					ComposeHelper* helper = new ComposeHelper(*overlayInputs[i]->caminput, width, height, mode);
					overlayInputs[i]->helper = helper;
				}
				else
				{
					overlayInputs[i]->helper->update(*overlayInputs[i]->caminput, width, height, mode);
				}

	            if (overlayInputs[i]->right_helper == NULL)
				{
					ComposeHelper* right_helper = new ComposeHelper(*overlayInputs[i]->caminput, width, height, mode, true);
					overlayInputs[i]->right_helper = right_helper;
				}
				else
				{
					overlayInputs[i]->right_helper->update(*overlayInputs[i]->caminput, width, height, mode, true);
				}

				overlayInputs[i]->caminput->dirty = false;
				//overlayInputs[i]->caminput->state = INPUT_STATE_DRAW;
			}
		}
	}
}

void WarpComposer::updateTexure(InputTempData* input_tempData, int** input_state)
{

	if (input_tempData->updated_mask != 0 && input_tempData->init_masks == 1)
	{
		printf("bigheadk, updateTexture calibrator->init_masks = %d\n", input_tempData->init_masks);
		{
			for (int i = 0; i < input_tempData->mNum_camera; i++) {
				//if (input_state != 0)
				//{
				//	if (*input_state[i] == -1)
				//		continue;
				//}
				unsigned char* dst = (unsigned char*)input_tempData->seamMasks[i];
				GLTexMask& tex = input->camIn(i)->getTexMask();
				tex.setMask(dst, input_tempData->mw, input_tempData->mh);
			}
			input_tempData->updated_mask = 0;
		}
	}
}

int StitchComposer::startOutput(int outputDevice, int outputDisplay)
{
	if (isOutput == QVS_MSG_NO_BMOUTPUT)
	{
		m_BMOutput = BMOutput::create_BMOutput(outputWidth, outputWidth / 16 * 9, outputDevice, outputDisplay);
		isOutput = QVS_MSG_READY_BMOUTPUT;
		return QVS_SUCCESS;
	}
	return isOutput;
}

int StitchComposer::finishOutput()
{
	if (isOutput == QVS_MSG_BMOUTPUT)
	{
		isOutput = QVS_MSG_FINISH_BMOUTPUT;
		return QVS_SUCCESS;
	}
	return isOutput;
}

void StitchComposer::initOutput(int *isEncoding, int *isStreaming, int stitching_resolution_mode)
{
	int wdt = outputWidth, hgt = outputHeight;

	if (stitchingMode == STEREO_STITCHING_TB)
		hgt *= 2;
	else  if (stitchingMode == STEREO_STITCHING_LR)
		wdt *= 2;

	if (stitching_resolution_mode == STITCHING_16BY9)
	{
		hgt = wdt / 16 * 9;
	}

	m_outputManager = new COutputManager(wdt, hgt, isEncoding, isStreaming);
	if (stitchingMode == STEREO_STITCHING_TB || stitchingMode == STEREO_STITCHING_LR)
	{
		m_outputManager->setStereo(stitchingMode);
	}
	else
	{
		m_outputManager->setStereo(0);
	}
	
	//m_outputManager->init(wdt, hgt, isEncoding, isStreaming);

	m_BMOutput = NULL;
	isOutput = QVS_MSG_NO_BMOUTPUT;

	m_outputManager->setResolutionMode(stitching_resolution_mode);

	int rgbBufSize = wdt * hgt * 3;
	for (int i = 0; i < NUM_ENCODE_BUFFER; i++)
	{
		cudaCheckError(cudaMalloc(&g_RGBBuffer[i], rgbBufSize));
	}

	if (m_outputManager)
		m_outputManager->setCallback_setOutputLog(mpCallback, mpCallbackData);
}

void StitchComposer::destroyOutput()
{
	for (int i = 0; i < NUM_ENCODE_BUFFER; i++)
	{
		if (g_RGBBuffer[i])
			cudaFree(g_RGBBuffer[i]);
		g_RGBBuffer[i] = NULL;
	}

	if (m_outputManager != NULL)
	{
		delete m_outputManager;
		m_outputManager = NULL;
	}
}

void StitchComposer::initGL() {
	if (resFBO == 0) {
#if LICENSE_PERSONAL_KEY
		watermark_buffer = (unsigned char*)malloc(width * height * 4);
		cv::Mat WaterMark = cv::imread("vrue360.dat", CV_LOAD_IMAGE_UNCHANGED);
		cv::cvtColor(WaterMark, WaterMark, CV_BGRA2RGBA);
		cv::resize(WaterMark, WaterMark, cv::Size(width, height));
		memcpy(watermark_buffer, WaterMark.data, width * height * 4);
		WaterMark.release();
#endif

		// for laplacian
		printf("bigheadk, initGL malloc\n");
		buf = (GLPyramid**)malloc(sizeof(GLPyramid*)*mNumCamera);
		printf("bigheadk, initGL malloc done\n");
		for (int i = 0; i < mNumCamera; i++)
		{
			buf[i] = new GLPyramid(outputWidth*MULTIBAND_PADDING, outputHeight, PYR_DEPTH, GL_RGBA);
			buf[i]->initGL();
		}
		printf("bigheadk, initGL new GLPyramid done \n");
		cum.initGL();
		merger.initGL();
		// 
		if (blendMode == NORMAL_BLEND)
		{
			for (int i = 0; i < mNumCamera; i++)
			{
				buf[i]->setDepth(5);
			}
			printf("bigheadk, initGL new GLPyramid done \n");
			cum.setDepth(5);
			merger.setDepth(5);
		}
		glGenFramebuffers(1, &tmpFBO);
		glGenFramebuffers(1, &resFBO);
		glGenFramebuffers(1, &warpFBO);
		glGenFramebuffers(1, &outFBO);
		glGenTextures(1, &tmpTex);
		glGenTextures(1, &resTex);
		glGenTextures(1, &warpTex);
		glGenTextures(1, &outTex);;

		glGenTextures(1, &gridTex);
#if LICENSE_PERSONAL_KEY
		glGenTextures(1, &watermarkTex);

		glBindTexture(GL_TEXTURE_2D, watermarkTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, watermark_buffer);
		//setupTexture(watermarkTex, width, height, GL_RGBA, watermark_buffer);
#endif
		setupTexture(tmpTex, width*MULTIBAND_PADDING, height, GL_RGBA, NULL);
		if (stitchingMode == STEREO_STITCHING_TB)
		{
			setupTexture(resTex, width, height * 2, GL_RGBA, NULL);
			setupTexture(outTex, outputWidth, outputWidth / 16 * 9, GL_RGBA, NULL);
		}
		else if (stitchingMode == STEREO_STITCHING_LR)
		{
			setupTexture(resTex, width * 2, height, GL_RGBA, NULL);
			setupTexture(outTex, outputWidth * 2, outputWidth / 8 * 9, GL_RGBA, NULL);
		}
		else
		{
			setupTexture(resTex, width, height, GL_RGBA, NULL);
			setupTexture(outTex, outputWidth, outputWidth / 16 * 9, GL_RGBA, NULL);
		}
		setupTexture(warpTex, width, height, GL_RGBA32F_ARB, NULL);

		{
			int stereo_hgt = height * 2;
			cv::Mat img;
			img = cv::imread("./nadir/grid.png", CV_LOAD_IMAGE_UNCHANGED);

			int depth = img.channels();
			void* buffer = malloc(width*stereo_hgt * 4);

			if (!img.empty())
			{
				cv::cvtColor(img, img, CV_BGRA2RGBA);
				
				if (img.cols != width || img.rows != stereo_hgt)
				{
					cv::resize(img, img, cv::Size(width, stereo_hgt));
				}
				memcpy(buffer, img.data, width*stereo_hgt * 4);
			}
			else{
				memset(buffer, 0, width*stereo_hgt * 4);
			}

			setupTexture(gridTex, width, stereo_hgt, GL_RGBA, 0);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, stereo_hgt, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
			glFinish();

			free(buffer);
			img.release();
		}
		

		glBindFramebuffer(GL_FRAMEBUFFER, tmpFBO);
		glBindTexture(GL_TEXTURE_2D, tmpTex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tmpTex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
		glBindTexture(GL_TEXTURE_2D, resTex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resTex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, outFBO);
		glBindTexture(GL_TEXTURE_2D, outTex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outTex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, warpFBO);
		glBindTexture(GL_TEXTURE_2D, warpTex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, warpTex, 0);

		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		initOutput(&isEncoding, &isStreaming, stitching_resolution_mode);

		//m_fileStreming = new CFileStreaming();
	}
	glInited = true;

	isEncoding = QVS_MSG_NO_ENCODING;
	isStreaming = QVS_MSG_NO_STREAMING;
	isRequestReStreaming = INIT_REQUEST_RESTREAMING;
}

GLuint StitchComposer::getResTex() {
	return resTex;
}

GLuint StitchComposer::getOutTex() {
	return outTex;
}

void StitchComposer::setDepth(int d) {
	for (int i = 0; i < mNumCamera; i++)
	{
		buf[i]->setDepth(d);
	}
	cum.setDepth(d);
	merger.setDepth(d);
}
void StitchComposer::setRenderingInfo(int k, long long timeStamp){
	CamInput& p = *(input->camIn(k));
	//WarpHelper& hlp = helper(k);

	//float *colors;
	//colors = (float*)malloc(sizeof(float)* 4);

	//float colorR = 0.f, colorG = 0.f, colorB = 0.f, colorY = 0.f;
	//if (mCurveInfoR != NULL && mCurveInfoG != NULL && mCurveInfoB != NULL)
	{
		ExposureData exposure = p.exposureCompensation.getExposureDataForAnimation(timeStamp);
		colors[0] = p.exposureCompensation.manualR;
		colors[1] = p.exposureCompensation.manualG;
		colors[2] = p.exposureCompensation.manualB;
		colors[3] = exposure.exposureDiff[3];//channel Y	

		//unsigned char* curveR = exposure.exposureCurve[0];// +(unsigned char)colorR;
		//unsigned char* curveG = exposure.exposureCurve[1];// +(unsigned char)colorG;
		//unsigned char* curveB = exposure.exposureCurve[2];// +(unsigned char)colorB;

		glActiveTexture(GL_TEXTURE2);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, 256, 0, GL_ALPHA, GL_UNSIGNED_BYTE, exposure.exposureCurve[0]);
		glActiveTexture(GL_TEXTURE3);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, 256, 0, GL_ALPHA, GL_UNSIGNED_BYTE, exposure.exposureCurve[1]);
		glActiveTexture(GL_TEXTURE4);
		glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, 256, 0, GL_ALPHA, GL_UNSIGNED_BYTE, exposure.exposureCurve[2]);
	}

	//colors[0] = colorR;
	//colors[1] = colorG;
	//colors[2] = colorB;
	//colors[3] = colorY;

	//return colors;
}
void StitchComposer::add(size_t k, long long timeStamp, float animation_time, bool isEditMode, int stitching_mode) {
	CamInput& stereoP = *(input->camIn(k%2));
	CamInput& p = *(input->camIn(k));

	//ComposeHelper& stereoHlp = helper(k % 2);
	ComposeHelper& stereoHlp = helper(k);
	ComposeHelper& hlp = helper(k);

	int is_laplacian_mode = (blendMode == MULTIBAND_BLEND ? 1 : (blendMode == NORMAL_BLEND ? 1 : 0));

	//float* colors;

	//float colorR = 0.f, colorG = 0.f, colorB = 0.f, colorY = 0.f;

	//colors = setRenderingInfo((int)k, timeStamp);
	setRenderingInfo((int)k, timeStamp);

	//colorR = colors[0];
	//colorG = colors[1];
	//colorB = colors[2];
	//colorY = colors[3];
	if (blendMode == MULTIBAND_BLEND || blendMode == NORMAL_BLEND)
	{
		//warping
		//glFinish();
		//double tick = QClock();

		if (stitching_mode == STEREO_STITCHING_TB)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
			glViewport(0, 0, width, height*2);
			fullframeFisheyeStereoProg.setting(p.getImageTex(), p.getMaskTex(), p.getLUT0(), p.getLUT1(), p.getLUT2(), p.getMap(), p.getBeforeMap(), (float*)stereoHlp.rotatem, (float)stereoHlp.rot0, (float)stereoHlp.rot1, (float)stereoHlp.scale0, (float)stereoHlp.scale1,
				(float)stereoHlp.distance, (float)stereoHlp.aspect_dw, (float)stereoHlp.aspect_sw, (float*)stereoHlp.lens_dist, is_laplacian_mode, colors[0], colors[1], colors[2], gainRenderingMode, (float)stereoHlp.crop_ratio_w, (float)stereoHlp.crop_ratio_h, (float)stereoHlp.crop_sx, (float)stereoHlp.crop_sy, grayscale_mode, brightness_step, animation_time, (int)k % 2);
		}
		else if (stitching_mode == STEREO_STITCHING_LR)
		{
			
			glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
			glViewport(0, 0, width * 2, height);
			if (p.isMapSet()) {
				fullframeFisheyeStereoWithMapProg.setting(p.getImageTex(), p.getMaskTex(), p.getLUT0(), p.getLUT1(), p.getLUT2(), p.getMap(), p.getBeforeMap(), (float*)stereoHlp.rotatem, (float)stereoHlp.rot0, (float)stereoHlp.rot1, (float)stereoHlp.scale0, (float)stereoHlp.scale1,
					(float)stereoHlp.distance, (float)stereoHlp.aspect_dw, (float)stereoHlp.aspect_sw, (float*)stereoHlp.lens_dist, is_laplacian_mode, colors[0], colors[1], colors[2], gainRenderingMode, (float)stereoHlp.crop_ratio_w, (float)stereoHlp.crop_ratio_h, (float)stereoHlp.crop_sx, (float)stereoHlp.crop_sy, p.isMapSet(), (float)p.getMapW(), (float)p.getMapH(), grayscale_mode, brightness_step, animation_time, (int)k % 2 + 2);
			}
			else {
				fullframeFisheyeStereoProg.setting(p.getImageTex(), p.getMaskTex(), p.getLUT0(), p.getLUT1(), p.getLUT2(), p.getMap(), p.getBeforeMap(), (float*)stereoHlp.rotatem, (float)stereoHlp.rot0, (float)stereoHlp.rot1, (float)stereoHlp.scale0, (float)stereoHlp.scale1,
					(float)stereoHlp.distance, (float)stereoHlp.aspect_dw, (float)stereoHlp.aspect_sw, (float*)stereoHlp.lens_dist, is_laplacian_mode, colors[0], colors[1], colors[2], gainRenderingMode, (float)stereoHlp.crop_ratio_w, (float)stereoHlp.crop_ratio_h, (float)stereoHlp.crop_sx, (float)stereoHlp.crop_sy, grayscale_mode, brightness_step, animation_time, (int)k % 2 + 2);
			}
		}
		else if (stitching_mode == WIDE_STITCHING){
			buf[k]->clear();
			glDisable(GL_BLEND);
			glBindFramebuffer(GL_FRAMEBUFFER, buf[k]->gaussianFBO[0]);
			glViewport(0, 0, width*MULTIBAND_PADDING, height);
			rectlinearWarpUnalphaedPanoramaWarapedMaskProg.setting(p.getImageTex(), p.getMaskTex(), p.getLUT0(), p.getLUT1(), p.getLUT2(), p.getMap(), p.getBeforeMap(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
				(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, is_laplacian_mode, colors[0], colors[1], colors[2], gainRenderingMode, (float)hlp.crop_ratio_w, (float)hlp.crop_ratio_h, (float)hlp.crop_sx, (float)hlp.crop_sy, p.isMapSet(), (float)p.getMapW(), (float)p.getMapH(), grayscale_mode, brightness_step, animation_time, p.isMapAnimation(), p.lens_type);
			/*rectlinearPanoramaWarpProg.setting(p.getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
				(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, 0, -1);*/
		}
		else
		{
			buf[k]->clear();
			glDisable(GL_BLEND);
			glBindFramebuffer(GL_FRAMEBUFFER, buf[k]->gaussianFBO[0]);
			glViewport(0, 0, width*MULTIBAND_PADDING, height);
			fullframeFisheyeWarpUnalphaedWarapedMaskProg.setting(p.getImageTex(), p.getMaskTex(), p.getLUT0(), p.getLUT1(), p.getLUT2(), p.getMap(), p.getBeforeMap(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
				(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, is_laplacian_mode, colors[0], colors[1], colors[2], gainRenderingMode, (float)hlp.crop_ratio_w, (float)hlp.crop_ratio_h, (float)hlp.crop_sx, (float)hlp.crop_sy, p.isMapSet(), (float)p.getMapW(), (float)p.getMapH(), grayscale_mode, brightness_step, animation_time, p.isMapAnimation());
		}
		

		isEditMode = true;
		if (isEditMode)
		{
			if (stitching_mode == STEREO_STITCHING_TB) {
				
				if ((int)k % 2 == 0)
					glScissor(0, 0, width, height );
				else if ((int)k % 2 == 1)
					glScissor(0, height, width, height);
			}
			else if (stitching_mode == STEREO_STITCHING_LR) {

				if ((int)k % 2 == 0)
					glScissor(0, 0, width, height);
				else if ((int)k % 2 == 1)
					glScissor(width, 0, width, height);
			}
			//glScissor(0, 0, width, height);
			drawRect();
			warpProg.unuse();
		}
		else
		{
			glEnable(GL_SCISSOR_TEST);

			int wdt = width*MULTIBAND_PADDING;
			int marX = (wdt - width) / 2;

			int rx = hlp.roix, rw = (int)(hlp.roiw + 0.999999f);
			int ry = hlp.roiy, rh = (int)(hlp.roih + 0.999999f);


			if (rx >= marX && rx + rw <= width - marX) {
				glScissor(rx + marX, ry, rw, rh);
				drawRect();
			}
			if (rx < marX) {
				if (rx >= 0)
				{
					glScissor(rx + marX, ry, rw, rh);
					drawRect();

					glScissor(wdt - (marX - rx), ry, (marX - rx), rh);
					drawRect();
				}
				else  // rx<0
				{
					if (rx < -marX)
					{
						glScissor(0, ry, rw + (rx + marX), rh);
						drawRect();

						glScissor(wdt - (marX * 2) + (rx + marX), ry, rw, rh);	// chk rw
						drawRect();
					}
					else
					{
						glScissor(rx + marX, ry, rw, rh);
						drawRect();

						glScissor(wdt - (marX * 2) + (rx + marX), ry, rw, rh);
						drawRect();
					}
				}
			}
			if (rx + rw > width - marX) {
				if (rx < width - marX)
				{
					glScissor(rx + marX, ry, rw, rh);
					drawRect();

					glScissor(0, ry, marX - (width - (rx + rw)), rh);
					drawRect();
				}
				else
				{
					if (rx + rw < width)
					{
						glScissor(rx + marX, ry, rw, rh);
						drawRect();

						glScissor(width - (rx + rw), ry, rw, rh);
						drawRect();
					}
					else if (rx + rw < width + marX)
					{
						glScissor(rx + marX, ry, rw, rh);
						drawRect();

						glScissor(rx - (width - marX), ry, rw, rh);
						drawRect();
					}
					else
					{
						glScissor(rx + marX, ry, rw, rh);
						drawRect();

						glScissor(rx - (width - marX), ry, rw - (rx + rw - (width + marX)), rh);
						drawRect();
					}
				}
			}

			warpProg.unuse();
			glDisable(GL_SCISSOR_TEST);

		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		if (stitching_mode != STEREO_STITCHING_TB && stitching_mode != STEREO_STITCHING_LR && blendMode != NORMAL_BLEND) {
			buf[k]->buildPyramid();
		}
	}
	else if (blendMode == SIMPLE_BLEND)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_ONE, GL_ONE);
		//glBlendFunc(GL_ONE_MINUS_DST_ALPHA, GL_ONE);
		glBindFramebuffer(GL_FRAMEBUFFER, warpFBO);
		glViewport(0, 0, width, height);

		fullframeFisheyeWarpUnalphaedWarapedMaskProg.setting(p.getImageTex(), p.getMaskTex(), NULL, NULL, NULL, NULL, NULL, (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
			(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, is_laplacian_mode, colors[0], colors[1], colors[2], gainRenderingMode, (float)hlp.crop_ratio_w, (float)hlp.crop_ratio_h, (float)hlp.crop_sx, (float)hlp.crop_sy, 0, 0, 0, grayscale_mode, brightness_step, animation_time, 0);

		glEnable(GL_SCISSOR_TEST);
		int rx = hlp.roix, rw = (int)(hlp.roiw + 0.999999f);
		int ry = hlp.roiy, rh = (int)(hlp.roih + 0.999999f);
		if (rx >= 0 && rx + rw <= width) {
			glScissor(rx, ry, rw, rh);
			drawRect();
		}
		if (rx<0) {
			glScissor(rx + width, ry, width - rx, rh);
			drawRect();
			glScissor(0, ry, rw + rx, rh);
			drawRect();
		}
		if (rx + rw > width) {
			glScissor(rx, ry, width - rx, rh);
			drawRect();
			glScissor(0, ry, rw + rx - width, rh);
			drawRect();
		}
		warpProg.unuse();
		glDisable(GL_SCISSOR_TEST);
		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

bool StitchComposer::estimateExposureCompensation(InputTempData *input_tempData, int* buffer_idx, PTSParam* pts_param, int stereoIdx)
{
	return exposureCompensator->estimate(input_tempData, input, pts_param, helpers, buffer_idx, gainMode, stereoIdx);
}
std::vector<cv::Mat> StitchComposer::getCurve(InputTempData *input_tempData, long long timeStamp, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints)
{
	return exposureCompensator->getCurve(input_tempData, input, timeStamp, manualCurvePoints, gainMode);
}

void StitchComposer::refresh(int** input_state, float animation_time, int stereoIndex, int editMode, int stitching_mode) {
	if (!input) return;

	WarpComposer::refresh(input_state, animation_time, stereoIndex, editMode, stitching_mode);
		
		if (stitching_mode == STITCHED_INPUT)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
			glClearColor(0, 0, 0, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			glViewport(0, 0, width, height);

			CamInput& p = *(input->camIn(0));
			simpleProg.use(p.getImageTex());		

			drawRect();

			simpleProg.unuse();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		else
		{
			if (blendMode == MULTIBAND_BLEND || blendMode == NORMAL_BLEND)
			{
				if (stitching_mode == STEREO_STITCHING_TB || stitching_mode == STEREO_STITCHING_LR) {
					glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
					glClearColor(0.0, 0.0, 0.0, 1.0);
					glClear(GL_COLOR_BUFFER_BIT);
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}
				cum.clear();
			}
			else if (blendMode == SIMPLE_BLEND)
			{
				glBindFramebuffer(GL_FRAMEBUFFER, warpFBO);
				glClearColor(0, 0, 0, 0);
				glClear(GL_COLOR_BUFFER_BIT);
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
			}

			if (stitching_mode == STEREO_STITCHING_TB || stitching_mode == STEREO_STITCHING_LR) {
				glEnable(GL_SCISSOR_TEST);
				glEnable(GL_TEXTURE_2D);
				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			}
			long long timeStamp = QClock();
	
			if (stitching_mode == STEREO_STITCHING_TB || stitching_mode == STEREO_STITCHING_LR) {
				for (size_t i = stereoIndex * 2; i <= stereoIndex*2 + 1; i++)
				{
					if (input->camIn(i) && input_state != 0 && *input_state[i] != -1)
					{
						add(i, timeStamp, animation_time, editMode, stitching_mode);
					}
				}

				glDisable(GL_TEXTURE_2D);
				glDisable(GL_BLEND);
				glDisable(GL_SCISSOR_TEST);
				glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
			}
			else {
				for (size_t i = 0; i < input->count(); i++)
				{
					if (input->camIn(i) && input_state != 0 && *input_state[i] != -1)
					{
						add(i, timeStamp, animation_time, editMode, stitching_mode);
					}
				}

				if (blendMode == MULTIBAND_BLEND )// || blendMode == NORMAL_BLEND)
				{
					cum.addLaplacian(mNumCamera, buf, contrast_step);
					//cum.fastadd(buf0, buf1, buf2, buf3, mNumCamera);
					merger.merge(cum, tmpFBO);

					glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
					glViewport(0, 0, width, height);
					multiBandPaddingProg.use(tmpTex);
					drawRect();
					multiBandPaddingProg.unuse();
				}
				else if (blendMode == NORMAL_BLEND)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
					glViewport(0, 0, width, height);
					multiBandPaddingProg.use(buf[0]->gaussianTex[0]);
					drawRect();
					multiBandPaddingProg.unuse();

				}
				else if (blendMode == SIMPLE_BLEND)
				{
					/*
					glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
					glViewport(0, 0, width, height);
					normalizeProg.use(warpTex);
					drawRect();
					normalizeProg.unuse();
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
					*/
				}

				//GLuint fboId = resFBO;

			
			glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
			glViewport(0, 0, width, height);
			multiBandPaddingProg.use(tmpTex);
			drawRect();
			multiBandPaddingProg.unuse();
			}
		}

		if (stitching_mode == STEREO_STITCHING_TB) {
			for (int k = 0; k < 2; k++)
			{
				CamInput& p = *(nadir);

				if (p.getState() == INPUT_STATE_DRAW)
				{
					const ComposeHelper& hlp = *(ComposeHelper*)nadir_helper;


					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
					glViewport(0, 0, width, height * 2);

					fullframeFisheyeWarpProg.setting(p.getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
						(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, 1.0, k);
					glEnable(GL_SCISSOR_TEST);

					int rx = hlp.roix, rw = (int)(hlp.roiw + 0.999999f);
					int ry = hlp.roiy, rh = (int)(hlp.roih + 0.999999f);
					if (rx >= 0 && rx + rw <= width) {
						glScissor(rx, ry, rw, rh);
						drawRect();
					}
					if (rx<0) {
						glScissor(rx + width, ry + (k*height), width - rx, rh);
						drawRect();
						glScissor(0, ry + (k*height), rw + rx, rh);
						drawRect();
					}
					if (rx + rw > width) {
						glScissor(rx, ry + (k*height), width - rx, rh);
						drawRect();
						glScissor(0, ry + (k*height), rw + rx - width, rh);
						drawRect();
					}

					warpProg.unuse();
					glDisable(GL_BLEND);
					glDisable(GL_SCISSOR_TEST);
				}

				if (overlayInputs.size() > 0)
				{
					for (int num = 0; num < MAX_OVERLAY_TEMPLATE; num++)
						for (int i = 0; i < overlayInputs.size(); i++)
						{
							if (overlayInputs[i]->templateNum == num)
							{
								CamInput* p = (CamInput*)overlayInputs[i]->caminput;
								if (p->state == INPUT_STATE_DRAW || p->state == FADE_IN_OVERLAY || p->state == FADE_OUT_OVERLAY || p->state == UNSELECTED_OVERLAY || p->state == SELECTED_OVERLAY)
								{
									p->refresh(-1);
									const ComposeHelper& hlp = *(ComposeHelper*)overlayInputs[i]->helper;
									const ComposeHelper& r_hlp = *(ComposeHelper*)overlayInputs[i]->right_helper;
	
									glEnable(GL_BLEND);
									glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

									glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
									glViewport(0, 0, width, height * 2);

									switch (p->state)
									{
									case INPUT_STATE_DRAW:
									case SELECTED_OVERLAY:
										overlayInputs[i]->opacity = 1.0;
										break;
									case UNSELECTED_OVERLAY:
										overlayInputs[i]->opacity = 0.3;
										break;
									case FADE_IN_OVERLAY:
										if (overlayInputs[i]->opacity < 1.0)
											overlayInputs[i]->opacity += 0.1;
										else
											p->state = INPUT_STATE_DRAW;
										break;
									case FADE_OUT_OVERLAY:
										if (overlayInputs[i]->opacity > 0.0)
											overlayInputs[i]->opacity -= 0.1;
										else
										{
											p->state = INPUT_STATE_NO_DRAW;
											overlayInputs[i]->opacity = 0.0f;
										}
										break;
									}

								/*if (stitching_mode == WIDE_STITCHING) {
									rectlinearPanoramaWarpProg.setting(p->getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
										(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, overlayInputs[i]->opacity, k);
										}
								else {
									fullframeFisheyeWarpProg.setting(p->getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
										(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, overlayInputs[i]->opacity, k);
									}*/

									if (k == 0)
									{
										fullframeFisheyeWarpProg.setting(p->getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
											(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, overlayInputs[i]->opacity, k);

										glEnable(GL_SCISSOR_TEST);
										int regionW = width / REGION_WIDTH;
										int regionH = height / REGION_HEIGHT;

										for (int i = 0; i < REGION_HEIGHT; i++){
											for (int j = 0; j < REGION_WIDTH; j++){
												if (hlp.regionMap[i][j]){
													int rx = regionW*j, rw = regionW;
													int ry = regionH*i, rh = regionH;
													glScissor(rx, ry + (k*height), rw, rh);
													drawRect();
												}
											}
										}
										drawRect();
										warpProg.unuse();
										glDisable(GL_BLEND);
										glDisable(GL_SCISSOR_TEST);
									}
									else if (k == 1)
									{
										fullframeFisheyeWarpProg.setting(p->getImageTex(), (float*)r_hlp.rotatem, (float)r_hlp.rot0, (float)r_hlp.rot1, (float)r_hlp.scale0, (float)r_hlp.scale1,
											(float)r_hlp.distance, (float)r_hlp.aspect_dw, (float)r_hlp.aspect_sw, (float*)r_hlp.lens_dist, overlayInputs[i]->opacity, k);

										glEnable(GL_SCISSOR_TEST);
										int regionW = width / REGION_WIDTH;
										int regionH = height / REGION_HEIGHT;

										for (int i = 0; i < REGION_HEIGHT; i++){
											for (int j = 0; j < REGION_WIDTH; j++){
												if (r_hlp.regionMap[i][j]){
													int rx = regionW*j, rw = regionW;
													int ry = regionH*i, rh = regionH;
													glScissor(rx, ry + (k*height), rw, rh);
													drawRect();
												}
											}
										}
										drawRect();
										warpProg.unuse();
										glDisable(GL_BLEND);
										glDisable(GL_SCISSOR_TEST);
									}
								}
							}
						}
				}
			}
		}
		else if (stitching_mode == STEREO_STITCHING_LR) {
			for (int k = 0; k < 2; k++)
			{
				CamInput& p = *(nadir);

				if (p.getState() == INPUT_STATE_DRAW)
				{
					const ComposeHelper& hlp = *(ComposeHelper*)nadir_helper;


					glEnable(GL_BLEND);
					glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

					glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
					glViewport(0, 0, width * 2, height);

					fullframeFisheyeWarpProg.setting(p.getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
						(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, 1.0, k + 2);
					glEnable(GL_SCISSOR_TEST);

					int rx = hlp.roix, rw = (int)(hlp.roiw + 0.999999f);
					int ry = hlp.roiy, rh = (int)(hlp.roih + 0.999999f);
					if (rx >= 0 && rx + rw <= width) {
						glScissor(rx + (k*width), ry, rw, rh);
						drawRect();
					}
					if (rx<0) {
						glScissor(rx + (k*width), ry, width - rx, rh);
						drawRect();
						glScissor(k*width, ry, rw + rx, rh);
						drawRect();
					}
					if (rx + rw > width) {
						glScissor(rx + (k*width), ry, width - rx, rh);
						drawRect();
						glScissor(k*width, ry, rw + rx - width, rh);
						drawRect();
					}

					warpProg.unuse();
					glDisable(GL_BLEND);
					glDisable(GL_SCISSOR_TEST);
				}

				if (overlayInputs.size() > 0)
				{
					for (int num = 0; num < MAX_OVERLAY_TEMPLATE; num++)
						for (int i = 0; i < overlayInputs.size(); i++)
						{
							if (overlayInputs[i]->templateNum == num)
							{
								CamInput* p = (CamInput*)overlayInputs[i]->caminput;
								if (p->state == INPUT_STATE_DRAW || p->state == FADE_IN_OVERLAY || p->state == FADE_OUT_OVERLAY || p->state == UNSELECTED_OVERLAY || p->state == SELECTED_OVERLAY)
								{
									p->refresh(-1);
									const ComposeHelper& hlp = *(ComposeHelper*)overlayInputs[i]->helper;
									const ComposeHelper& b_hlp = *(ComposeHelper*)overlayInputs[i]->right_helper;
									glEnable(GL_BLEND);
									glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

									glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
									glViewport(0, 0, width * 2, height);

									switch (p->state)
									{
									case INPUT_STATE_DRAW:
									case SELECTED_OVERLAY:
										overlayInputs[i]->opacity = 1.0;
										break;
									case UNSELECTED_OVERLAY:
										overlayInputs[i]->opacity = 0.3;
										break;
									case FADE_IN_OVERLAY:
										if (overlayInputs[i]->opacity < 1.0)
											overlayInputs[i]->opacity += 0.1;
										else
											p->state = INPUT_STATE_DRAW;
										break;
									case FADE_OUT_OVERLAY:
										if (overlayInputs[i]->opacity > 0.0)
											overlayInputs[i]->opacity -= 0.1;
										else
										{
											p->state = INPUT_STATE_NO_DRAW;
											overlayInputs[i]->opacity = 0.0f;
										}
										break;
									}

									if (k == 0)
									{
										fullframeFisheyeWarpProg.setting(p->getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
											(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, overlayInputs[i]->opacity, k + 2);

										glEnable(GL_SCISSOR_TEST);
										int regionW = width / REGION_WIDTH;
										int regionH = height / REGION_HEIGHT;

										for (int i = 0; i < REGION_HEIGHT; i++){
											for (int j = 0; j < REGION_WIDTH; j++){
												if (hlp.regionMap[i][j]){
													int rx = regionW*j, rw = regionW;
													int ry = regionH*i, rh = regionH;
													glScissor(rx + (k*width), ry, rw, rh);
													drawRect();
												}
											}
										}
										drawRect();
										warpProg.unuse();
										glDisable(GL_BLEND);
										glDisable(GL_SCISSOR_TEST);
									}
									else if (k == 1)
									{
										fullframeFisheyeWarpProg.setting(p->getImageTex(), (float*)b_hlp.rotatem, (float)b_hlp.rot0, (float)b_hlp.rot1, (float)b_hlp.scale0, (float)b_hlp.scale1,
											(float)b_hlp.distance, (float)b_hlp.aspect_dw, (float)b_hlp.aspect_sw, (float*)b_hlp.lens_dist, overlayInputs[i]->opacity, k + 2);

										glEnable(GL_SCISSOR_TEST);
										int regionW = width / REGION_WIDTH;
										int regionH = height / REGION_HEIGHT;

										for (int i = 0; i < REGION_HEIGHT; i++){
											for (int j = 0; j < REGION_WIDTH; j++){
												if (b_hlp.regionMap[i][j]){
													int rx = regionW*j, rw = regionW;
													int ry = regionH*i, rh = regionH;
													glScissor(rx + (k*width), ry, rw, rh);
													drawRect();
												}
											}
										}
										drawRect();
										warpProg.unuse();
										glDisable(GL_BLEND);
										glDisable(GL_SCISSOR_TEST);
									}
								}
							}
						}
				}
			}
		}
		else
		{
			CamInput& p = *(nadir);

			if (p.getState() == INPUT_STATE_DRAW)
			{
				const ComposeHelper& hlp = *(ComposeHelper*)nadir_helper;


				glEnable(GL_BLEND);
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

				glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
				glViewport(0, 0, width, height);

				fullframeFisheyeWarpProg.setting(p.getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
					(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, 1.0, -1);
				glEnable(GL_SCISSOR_TEST);
				int rx = hlp.roix, rw = (int)(hlp.roiw + 0.999999f);
				int ry = hlp.roiy, rh = (int)(hlp.roih + 0.999999f);
				if (rx >= 0 && rx + rw <= width) {
					glScissor(rx, ry, rw, rh);
					drawRect();
				}
				if (rx<0) {
					glScissor(rx + width, ry, width - rx, rh);
					drawRect();
					glScissor(0, ry, rw + rx, rh);
					drawRect();
				}
				if (rx + rw > width) {
					glScissor(rx, ry, width - rx, rh);
					drawRect();
					glScissor(0, ry, rw + rx - width, rh);
					drawRect();
				}

				warpProg.unuse();
				glDisable(GL_BLEND);
				glDisable(GL_SCISSOR_TEST);
			}

			if (overlayInputs.size() > 0)
			{
				for (int num = 0; num < MAX_OVERLAY_TEMPLATE; num++)
					for (int i = 0; i < overlayInputs.size(); i++)
					{
						if (overlayInputs[i]->templateNum == num)
						{
							CamInput* p = (CamInput*)overlayInputs[i]->caminput;
							if (p->state == INPUT_STATE_DRAW || p->state == FADE_IN_OVERLAY || p->state == FADE_OUT_OVERLAY || p->state == UNSELECTED_OVERLAY || p->state == SELECTED_OVERLAY)
							{
								p->refresh(-1);
								const ComposeHelper& hlp = *(ComposeHelper*)overlayInputs[i]->helper;
								glEnable(GL_BLEND);
								glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

								glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
								glViewport(0, 0, width, height);

								switch (p->state)
								{
								case INPUT_STATE_DRAW:
								case SELECTED_OVERLAY:
									overlayInputs[i]->opacity = 1.0;
									break;
								case UNSELECTED_OVERLAY:
									overlayInputs[i]->opacity = 0.3;
									break;
								case FADE_IN_OVERLAY:
									if (overlayInputs[i]->opacity < 1.0)
										overlayInputs[i]->opacity += 0.1;
									else
										p->state = INPUT_STATE_DRAW;
									break;
								case FADE_OUT_OVERLAY:
									if (overlayInputs[i]->opacity > 0.0)
										overlayInputs[i]->opacity -= 0.1;
									else
									{
										p->state = INPUT_STATE_NO_DRAW;
										overlayInputs[i]->opacity = 0.0f;
									}
									break;
								}

							if (stitching_mode == WIDE_STITCHING) {
								rectlinearPanoramaWarpProg.setting(p->getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
									(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, overlayInputs[i]->opacity, -1);
								}
							else {
								fullframeFisheyeWarpProg.setting(p->getImageTex(), (float*)hlp.rotatem, (float)hlp.rot0, (float)hlp.rot1, (float)hlp.scale0, (float)hlp.scale1,
									(float)hlp.distance, (float)hlp.aspect_dw, (float)hlp.aspect_sw, (float*)hlp.lens_dist, overlayInputs[i]->opacity, -1);
							}

								int regionW = width / REGION_WIDTH;
								int regionH = height / REGION_HEIGHT;
							glEnable(GL_SCISSOR_TEST);
								for (int i = 0; i < REGION_HEIGHT; i++){
									for (int j = 0; j < REGION_WIDTH; j++){
										if (hlp.regionMap[i][j]){
											int rx = regionW*j, rw = regionW;
											int ry = regionH*i, rh = regionH;
											glScissor(rx, ry, rw, rh);
											drawRect();
										}
									}
								}
								drawRect();

								warpProg.unuse();
								glDisable(GL_BLEND);
								glDisable(GL_SCISSOR_TEST);
							}
						}
					}
			}
		}

		//watermark
	#if LICENSE_PERSONAL_KEY
		if (QVS_Engine::QVS_getPlanType() == QVS_PLANTYPE_DEMO)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
			glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
			glViewport(0, 0, width, height);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, watermarkTex);
			//glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, watermark_buffer);
			drawRect();
			glDisable(GL_TEXTURE_2D);
			glDisable(GL_BLEND);
		}
	#endif

		// resize
	if (stitching_resolution_mode == STITCHING_16BY9)
	{
		int wdt = outputWidth;
		if (stitching_mode == STEREO_STITCHING_LR)
			wdt *= 2;

		glDisable(GL_BLEND);
		glBindFramebuffer(GL_FRAMEBUFFER, outFBO);
		glViewport(0, 0, wdt, wdt / 16 * 9);
		glClearColor(0, 0, 0, 0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, resTex);
		drawRect();
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	if (isSave) {
		int wdt = outputWidth, hgt = outputHeight;
		if (stitching_mode == STEREO_STITCHING_TB)
			hgt *= 2;
		else if (stitching_mode == STEREO_STITCHING_LR)
			wdt *= 2;
		if (stitching_resolution_mode == STITCHING_16BY9)
			hgt = wdt / 16 * 9;

		unsigned char* buff = (unsigned char*)malloc(wdt * hgt * 3);
		glReadPixels(0, 0, wdt, hgt, GL_RGB, GL_UNSIGNED_BYTE, buff);
		cv::Mat mat1 = cv::Mat(hgt, wdt, CV_8UC3, buff);

		cv::cvtColor(mat1, mat1, CV_RGB2BGR);

		//cv::imwrite("output.png", mat1);
		if (pano_file_path != NULL) cv::imwrite(pano_file_path, mat1);
		free(buff);
		isSave = false;
		pano_file_path = NULL;
	}

	if (isOutput == QVS_MSG_READY_BMOUTPUT)
	{
		BMWdt = outputWidth;
		BMHgt = outputHeight;
		BMResolutionMode = STITCHING_2BY1;
		if (stitching_resolution_mode == STITCHING_16BY9)
		{
			BMHgt = BMWdt / 16 * 9;
			BMResolutionMode = STITCHING_16BY9;
		}
		glGenBuffersARB(NUM_ENCODE_BUFFER, BMOutPBO);
		for (int i = 0; i < NUM_ENCODE_BUFFER; i++)
		{
			glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, BMOutPBO[i]);
			glBufferDataARB(GL_PIXEL_PACK_BUFFER_ARB, BMWdt * BMHgt * 3, 0, GL_STREAM_READ_ARB);
			//BMPtr[i] = glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);
			//glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
		}

		glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);

		isOutput = QVS_MSG_BMOUTPUT;

		BMReadindex = -1;
		BMWriteindex = 0;

	}
	else if (isOutput == QVS_MSG_BMOUTPUT)
	{
		if (BMResolutionMode == stitching_resolution_mode)
		{
			if (m_BMOutput)
			{
				void* ptr = m_BMOutput->getBuffer();

				glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, BMOutPBO[BMWriteindex]);
				glReadPixels(0, 0, BMWdt, BMHgt, GL_RGB, GL_UNSIGNED_BYTE, 0);

				if (BMReadindex >= 0)
				{
					glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, BMOutPBO[BMReadindex]);
					BMPtr[BMReadindex] = glMapBufferARB(GL_PIXEL_PACK_BUFFER_ARB, GL_READ_ONLY_ARB);

					if (BMPtr[BMReadindex])
					{
						memcpy(ptr, BMPtr[BMReadindex], BMWdt * BMHgt * 3);
						glUnmapBufferARB(GL_PIXEL_PACK_BUFFER_ARB);
					}
				}
				BMReadindex = BMWriteindex;

				BMWriteindex = (BMWriteindex + 1) % NUM_ENCODE_BUFFER;
#if(1)
				// TEST CODE
				//glReadPixels(0, 0, outputWidth, outputHeight, GL_RGB, GL_UNSIGNED_BYTE, ptr);

				//static char color = 0;
				//memset(ptr, color++, 1920 * 1080 * 3);
#endif

				m_BMOutput->finishBufferCopy();
				glBindBufferARB(GL_PIXEL_PACK_BUFFER_ARB, 0);

			}
		}
		else
		{
			printf("ERROR, Different Resolution!!\n");
		}

	}
	else if (isOutput == QVS_MSG_FINISH_BMOUTPUT)
	{
		if (m_BMOutput)
		{
			BMOutput::destroy_BMOutput(m_BMOutput);
			m_BMOutput = NULL;

			//glDeleteBuffersARB(1, &BMOutPBO);
			glDeleteBuffersARB(NUM_ENCODE_BUFFER, BMOutPBO);


		}
		isOutput = QVS_MSG_NO_BMOUTPUT;

	}

	if (mPrepareMode)
	{
		if (stitching_resolution_mode == STITCHING_16BY9)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindFramebuffer(GL_FRAMEBUFFER, outFBO);

			glViewport(0, 0, 0, 0);
			if (stitching_mode == STEREO_STITCHING_TB)
				glViewport(0, 0, width, width / 16 * 9);
			else if (stitching_mode == STEREO_STITCHING_LR)
				glViewport(0, 0, width * 2, width / 8 * 9);

			simpleProg.use(gridTex);

			drawRect();

			simpleProg.unuse();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_BLEND);
		}
		else
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			glBindFramebuffer(GL_FRAMEBUFFER, resFBO);

			glViewport(0, 0, 0, 0);
			if (stitching_mode == STEREO_STITCHING_TB)
				glViewport(0, 0, width, height * 2);
			else if (stitching_mode == STEREO_STITCHING_LR)
				glViewport(0, 0, width * 2, height);

			simpleProg.use(gridTex);

			drawRect();

			simpleProg.unuse();
			glBindFramebuffer(GL_FRAMEBUFFER, 0);

			glDisable(GL_BLEND);
		}
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

bool StitchComposer::makeSeamMask(PTSParam* pts_param, InputTempData *input_tempData, int* buffer_idx, long long time_stamp, int edit_mode)
{
	printf("====> makeSeamMask START\n");
	float t = QClock();

	if (edit_mode == MASK_FOR_EDIT) {
		const size_t NUM_INPUT = input_tempData->mNum_camera;

		if (input_tempData->init_masks == 0)
		{
			input_tempData->mw = width / SEAM_SCALE, input_tempData->mh = height / SEAM_SCALE;
			input_tempData->seamMasks = (void**)malloc(sizeof(void*)* input_tempData->mNum_camera);

			for (size_t i = 0; i < input_tempData->mNum_camera; i++)
			{
				input_tempData->seamMasks[i] = malloc(input_tempData->mw*input_tempData->mh);
			}
			input_tempData->init_masks = 1;
		}
		for (size_t i = 0; i<input_tempData->mNum_camera; i++) {
			ComposeHelper& h = helper(i);
			unsigned char* dst = (unsigned char*)input_tempData->seamMasks[i];

			int mw = width / SEAM_SCALE, mh = height / SEAM_SCALE;

			memset(dst, 255, mw*mh);
		}
	}
	else if (seamMode == FIND_SEAM_FROM_IMAGE) {
		std::vector<cv::Mat> imgs;
		std::vector<cv::Mat> masks;
		std::vector<cv::Point2i> corners;

		cv::Rect crop_rect;

		for (size_t i = 0; i<input_tempData->mNum_camera; i++) {
			ComposeHelper& h = helper(i);
			int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
			cv::Mat msk = cv::Mat::zeros(rh, rw, CV_8UC1);
			cv::Mat m1 = cv::Mat::zeros(rh, rw, CV_8UC3);

			int wdt = input_tempData->inputs[i].width;
			int hgt = input_tempData->inputs[i].height;

			crop_rect.x = ((CamInputImageFile*)(input->camIn(i)))->crop_left;
			crop_rect.y = ((CamInputImageFile*)(input->camIn(i)))->crop_top;

			crop_rect.width = ((CamInputImageFile*)(input->camIn(i)))->crop_right - crop_rect.x;
			crop_rect.height = ((CamInputImageFile*)(input->camIn(i)))->crop_bottom - crop_rect.y;
			unsigned char* buf = (unsigned char*)input_tempData->inputs[i].buffers[buffer_idx[i]];

			cv::Mat org = cv::Mat(hgt, wdt, CV_8UC3, buf).clone();
			h.smallWarp(wdt, hgt, org.data, m1.data, msk.data, crop_rect, SEAM_SCALE);
			imgs.push_back(m1);
			masks.push_back(msk);
			corners.push_back(cv::Point2i(rx, ry));
		}
		for (size_t i = 0; i < input_tempData->mNum_camera - 1; i++)
		{
			for (size_t j = i + 1; j < input_tempData->mNum_camera; j++)
			{
				findSeamInPair(width / SEAM_SCALE, imgs[i], masks[i], corners[i], imgs[j], masks[j], corners[j]);
			}

		}
		if (input_tempData->init_masks == 0)
		{
			input_tempData->mw = width / SEAM_SCALE, input_tempData->mh = height / SEAM_SCALE;
			input_tempData->seamMasks = (void**)malloc(sizeof(void*)* input_tempData->mNum_camera);

			for (size_t i = 0; i < input_tempData->mNum_camera; i++)
			{
				input_tempData->seamMasks[i] = malloc(input_tempData->mw*input_tempData->mh);
			}
			input_tempData->init_masks = 1;
		}

		for (size_t i = 0; i<input_tempData->mNum_camera; i++) {
			ComposeHelper& h = helper(i);

			cv::dilate(masks[i], masks[i], cv::Mat());
			cv::dilate(masks[i], masks[i], cv::Mat());
			cv::dilate(masks[i], masks[i], cv::Mat());
			unsigned char* dst = (unsigned char*)input_tempData->seamMasks[i];

			int mw = width / SEAM_SCALE, mh = height / SEAM_SCALE;

			memset(dst, 0, mw*mh);

			int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);

			if (rx<0) rx += mw;
			if (rx + rw>mw) {
				int w1 = (mw - rx), w2 = (rx + rw - mw);
				unsigned char* b1 = new unsigned char[w1*rh];
				unsigned char* b2 = new unsigned char[w2*rh];
				for (int y = 0; y<rh; y++) {
					memcpy(b1 + y*w1, masks[i].data + y*rw, w1);
					memcpy(b2 + y*w2, masks[i].data + y*rw + w1, w2);
				}
				for (int yy = ry; yy < ry + rh; yy++)
				{
					for (int xx = rx; xx < rx + w1; xx++)
					{
						dst[yy*mw + xx] = b1[(yy - ry)*w1 + (xx - rx)];
					}
				}
				for (int yy = ry; yy < ry + rh; yy++)
				{
					for (int xx = 0; xx < w2; xx++)
					{
						dst[yy*mw + xx] = b2[(yy - ry)*w2 + (xx - 0)];
					}
				}
				delete b1;
				delete b2;
			}
			else
			{
				for (int yy = ry; yy < ry + rh; yy++)
				{
					for (int xx = rx; xx < rx + rw; xx++)
					{
						dst[yy*mw + xx] = masks[i].data[(yy - ry)*rw + (xx - rx)];
					}
				}
			}
		}
	}
	else if (seamMode == FIND_SEAM_FROM_DIST) {
		std::vector<cv::Mat> imgs;
		std::vector<cv::Mat> masks2;
		std::vector<cv::Point2i> corners;
		std::vector<cv::Rect> rects;

		const size_t NUM_INPUT = input_tempData->mNum_camera;
		bool has_mask[CAMERA_MAX];// = true;
		cv::Mat input_mask[CAMERA_MAX];

		for (int i = 0; i < NUM_INPUT; i++)
		{
			if (pts_param[i].sourcemask.rows <= 0 || pts_param[i].sourcemask.cols <= 0)
			{
				has_mask[i] = false;
			}
			else
			{
				input_mask[i] = pts_param[i].sourcemask.clone();
				cv::resize(input_mask[i], input_mask[i], cv::Size(input_mask[i].cols / SEAM_SCALE, input_mask[i].rows / SEAM_SCALE));
				has_mask[i] = true;
			}
		}
		for (size_t i = 0; i<input_tempData->mNum_camera; i++) {
			ComposeHelper& h = helper(i);
			int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);
			cv::Mat msk2 = cv::Mat::zeros(rh, rw, CV_8UC1);
			masks2.push_back(msk2);
			corners.push_back(cv::Point2i(rx, ry));
			rects.push_back(cv::Rect(rx, ry, rw, rh));
		}
		cv::Rect crop_rect;

		for (int y = 0; y < height / SEAM_SCALE; y++) {
			for (int x = 0; x < width / SEAM_SCALE; x++) {
				int minDist = 1000000, minDistIndex = -1, minX = 0, minY = 0, dist = 0, rx, ry;
				for (size_t i = 0; i < input->count(); i++) {
					cv::Rect  tRect(corners[i].x, corners[i].y, rects[i].width, rects[i].height);
					if (containROI(width / SEAM_SCALE, height / SEAM_SCALE, tRect, cv::Point(x, y))) {
						ComposeHelper& h = helper(i);
						crop_rect.x = ((CamInputImageFile*)(input->camIn(i)))->crop_left;
						crop_rect.y = ((CamInputImageFile*)(input->camIn(i)))->crop_top;
						crop_rect.width = ((CamInputImageFile*)(input->camIn(i)))->crop_right - crop_rect.x;
						crop_rect.height = ((CamInputImageFile*)(input->camIn(i)))->crop_bottom - crop_rect.y;

						int wdt = ((CamInputImageFile*)(input->camIn(i)))->width;
						int hgt = ((CamInputImageFile*)(input->camIn(i)))->height;
						if (has_mask[i])
						{
							dist = h.CalcDist(input_mask[i], wdt, hgt, x, y, crop_rect, SEAM_SCALE);
						}
						else
						{
							dist = h.CalcDist(wdt, hgt, x, y, crop_rect, SEAM_SCALE);
						}
						if (dist < minDist) {
							minDist = dist;
							minDistIndex = i;
						}
					}
				}
				if (minDistIndex >= 0) {
					int dx = x - corners[minDistIndex].x;
					int dy = y - corners[minDistIndex].y;
					if (dx < 0)
						dx += (width / SEAM_SCALE);
					if (dx >= (width / SEAM_SCALE))
						dx -= (width / SEAM_SCALE);

					if (NUM_INPUT == 1)
					{
					float margin = crop_rect.width / 2 / SEAM_SCALE * 0.15;
					
					if (sqrt(minDist) < crop_rect.width / 2 / SEAM_SCALE  * 0.85)
					masks2[minDistIndex].at<unsigned char>(dy, dx) = 255;
					else
					{
						//printf("bigheadk, margin = %f, minDist %d, ratio = %f\n", margin, minDist, (((crop_rect.width / 2 / SEAM_SCALE) - sqrt(minDist)) / margin));
						float ratio = (((crop_rect.width / 2 / SEAM_SCALE) - sqrt(minDist)) / margin);
						if (ratio < 0.0)
							ratio = 0.0f;
						masks2[minDistIndex].at<unsigned char>(dy, dx) = 255 * ratio;
					}
				}
					else
					{
						masks2[minDistIndex].at<unsigned char>(dy, dx) = 255;
					}
				}
			}
		}

		if (input_tempData->init_masks == 0)
		{
			input_tempData->mw = width / SEAM_SCALE, input_tempData->mh = height / SEAM_SCALE;
			input_tempData->seamMasks = (void**)malloc(sizeof(void*)* input_tempData->mNum_camera);

			for (size_t i = 0; i < input_tempData->mNum_camera; i++)
			{
				input_tempData->seamMasks[i] = malloc(input_tempData->mw*input_tempData->mh);
			}
			input_tempData->init_masks = 1;
		}
		for (size_t i = 0; i<input_tempData->mNum_camera; i++) {
			ComposeHelper& h = helper(i);

			cv::dilate(masks2[i], masks2[i], cv::Mat());
			unsigned char* dst = (unsigned char*)input_tempData->seamMasks[i];

			int mw = width / SEAM_SCALE, mh = height / SEAM_SCALE;

			memset(dst, 0, mw*mh);
			int rx = h.smallRoiX(SEAM_SCALE), ry = h.smallRoiY(SEAM_SCALE), rw = h.smallRoiW(SEAM_SCALE), rh = h.smallRoiH(SEAM_SCALE);

			if (rx<0) rx += mw;
			if (rx + rw>mw) {
				int w1 = (mw - rx), w2 = (rx + rw - mw);
				unsigned char* b1 = new unsigned char[w1*rh];
				unsigned char* b2 = new unsigned char[w2*rh];
				for (int y = 0; y<rh; y++) {
					memcpy(b1 + y*w1, masks2[i].data + y*rw, w1);
					memcpy(b2 + y*w2, masks2[i].data + y*rw + w1, w2);
				}
				for (int yy = ry; yy < ry + rh; yy++)
				{
					for (int xx = rx; xx < rx + w1; xx++)
					{
						dst[yy*mw + xx] = b1[(yy - ry)*w1 + (xx - rx)];
					}
				}
				for (int yy = ry; yy < ry + rh; yy++)
				{
					for (int xx = 0; xx < w2; xx++)
					{
						dst[yy*mw + xx] = b2[(yy - ry)*w2 + (xx - 0)];
					}
				}
				delete b1;
				delete b2;
			}
			else
			{
				for (int yy = ry; yy < ry + rh; yy++)
				{
					for (int xx = rx; xx < rx + rw; xx++)
					{
						dst[yy*mw + xx] = masks2[i].data[(yy - ry)*rw + (xx - rx)];
					}
				}

				//tex.subMask(masks[i].data, rx, ry, rw, rh);
			}

			//checking temporary mask 
			//cv::Mat mat1 = cv::Mat(mh, mw, CV_8UC1, dst);
			//std::stringstream ss;
			//int index = i;
			//ss << index;
			//std::string name = ss.str() + "mask.png";
			//cv::imwrite(name, mat1);
			//cv::imshow("mask", mat1);
			//cv::waitKey();
		}
		//multiband blend test
		//blendImages(corners, imgs, masks2);


		//}
	}
	printf("====> makeSeamMask END\n");
	input_tempData->updated_mask = 1;
	printf("Mask took : %f\n", QClock() - t);
	return true;
}

void StitchComposer::setGainMode(int mode)
{
	gainMode = mode;
	//curveUpdated = false;
}

void StitchComposer::setSeamMode(int mode)
{
	seamMode = mode;
}

void StitchComposer::setNadirMode(int mode)
{
	nadir->setState(mode);
	//nadirMode = mode;
}

void StitchComposer::setManualRGB(int idx, int R, int G, int B)
{
	CamInput& p = *(input->camIn(idx));

	p.exposureCompensation.setManualRGB(R, G, B);
}

void StitchComposer::setStitchingResolutionMode(int mode)
{
	stitching_resolution_mode = mode;
}

void StitchComposer::setCallback_setOutputLog(void* cb, void* data)
{
	mpCallback = cb;
	mpCallbackData = data;
}

int COutputManager::getResolutionMode()
{
	return resolution_mode;
}

void COutputManager::setResolutionMode(int mode)
{
	resolution_mode = mode;
}

void COutputManager::setStereo(int mode)
{
	m_isStereo = mode;
}

COutputManager::COutputManager(int width, int height, int *isEncoding, int *isStreaming)
{
	m_width = width;
	m_height = height;
	m_isEncoding = isEncoding;
	m_isStreaming = isStreaming;

	int rgbBufSize = width * height * 3;

	m_buffer_in_idx = 0;
	m_buffer_out_idx = -1;

	glGenTextures(1, &nv_glColor);
	glBindTexture(GL_TEXTURE_2D, nv_glColor);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenBuffers(1, &nv_glColorBuffer);
	glBindBuffer(GL_PIXEL_PACK_BUFFER, nv_glColorBuffer);
	glBufferData(GL_PIXEL_PACK_BUFFER, rgbBufSize * sizeof(unsigned char), 0, GL_DYNAMIC_DRAW);

	//cudaGraphicsGLRegisterBuffer(&nv_cuColorRes, nv_glColorBuffer, CU_GRAPHICS_MAP_RESOURCE_FLAGS_NONE);
	//glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

	//mFileStreaming = new CFileStreaming();
}

COutputManager::~COutputManager()
{
	glDeleteBuffers(1, &nv_glColorBuffer);
	glDeleteTextures(1, &nv_glColor);
}


void COutputManager::setCallback_setOutputLog(void* cb, void* data)
{
	mpSetOutputLogCallback = (callback_setOutputLog)cb;
	mpSetOutputLogCallbackData = data;
}

//unsigned int __stdcall nvEncodeFrameThreadFunc(void* param)
//{
//	CNvEncManager *pEncoder = (CNvEncManager *)param;
//	encOpts eo = pEncoder->m_encOpts;
//
//	double interval = (double)1000 / eo.fps;
//	double actual_interval = interval;
//	int numframe = 0;
//	double start_time = 0;
//
//	end_writing_container = false;
//
//	while (!pEncoder->bEncodeThreadFinish)
//	{
//		while (abs(pEncoder->m_timer - QClock()) < interval)
//		{
//		}
//
//		pEncoder->m_timer = QClock();
//
//		if (pEncoder->m_bufIndex >= 0)
//		{
//			pEncoder->startEncodeFrameThread();
//
//			if (numframe == 0)
//			{
//				start_time = QClock();
//			}
//			else
//			{
//				actual_interval = interval - ((abs(start_time - QClock()) / (double)numframe) - interval) * numframe*0.5;
//			}
//			numframe++;
//		}
//	}
//
//	pEncoder->finish(OUTPUT_HDD);
//	end_writing_container = true;
//	return 0;
//}
//
//unsigned int __stdcall nvStreamingThreadFunc(void* param)
//{
//	CNvEncManager *pEncoder = (CNvEncManager *)param;
//	encOpts eo = pEncoder->m_encOpts;
//
//	double interval = (double)1000 / eo.fps;
//	double actual_interval = interval;
//	int numframe = 0;
//	double start_time = 0;
//	int level = 2;
//
//	char* log = (char*)malloc(sizeof(char) * 32);
//	int size = sizeof(char) * 32;
//
//	/*
//	double kill_thread_check_timer;
//	const double KILL_TIME = 2000;
//	*/
//	while (!pEncoder->bEncodeThreadFinish)
//	{
//		while (abs(pEncoder->m_timer - QClock()) < interval)
//		{
//		}
//
//		pEncoder->m_timer = QClock();
//
//		if (*pEncoder->m_isStreaming == QVS_MSG_FINISH_STREAMING)
//			continue;
//
//		if (!pEncoder->isPushAble(level))
//		{
//			/*
//			if (level == 2)
//			{
//				kill_thread_check_timer = QClock();
//			}
//			else
//			{
//				if (abs(kill_thread_check_timer - QClock()) > KILL_TIME)
//				{
//					*pEncoder->m_isStreaming = QVS_MSG_FINISH_STREAMING;
//					continue;
//				}
//			}
//			*/
//			level = 3;
//			printf("bigheadk, drop frame 3\n");
//
//			if (pEncoder->m_nvEncoder->mpSetOutputLogCallback)
//			{
//				double duration = QClock() - pEncoder->m_nvEncoder->m_startTime;
//
//				int sec = (int)duration / 1000;
//				int min = sec / 60;
//				int hour = min / 60;
//
//				sprintf(log, "Frame drop!! %02d:%02d:%02d", hour, min % 60, sec % 60);
//				pEncoder->m_nvEncoder->mpSetOutputLogCallback(log, size, false);
//			}
//			continue;
//		}
//		else if (level == 3)
//		{
//			double duration = QClock() - pEncoder->m_nvEncoder->m_startTime;
//
//			int sec = (int)duration / 1000;
//			int min = sec / 60;
//			int hour = min / 60;
//
//			sprintf(log, "frame drop end!! change level from 3 to 2!! %02d:%02d:%02d", hour, min % 60, sec % 60);
//			pEncoder->m_nvEncoder->mpSetOutputLogCallback(log, size, false);
//
//			level = 2;
//		}
//
//		if (pEncoder->m_bufIndex >= 0)
//		{
//			pEncoder->startEncodeFrameThread();
//
//			if (numframe == 0)
//			{
//				start_time = QClock();
//			}
//			else
//			{
//				actual_interval = interval - ((abs(start_time - QClock()) / (double)numframe) - interval) * numframe*0.5;
//			}
//			numframe++;
//		}
//	}
//	printf("nvStreamingThreadFunc died!!!!!!!!!!!!!!!!!!!!!!!!\n");
//	pEncoder->finish(OUTPUT_STREAMING);
//
//	return 0;
//}

bool cudaCheckError(cudaError_t e)
{
	if (e != cudaSuccess)
	{
		printf("Cuda failure %s:%d: '%s'\n", __FILE__, __LINE__, cudaGetErrorString(e));
		return false;
	}
	else
	{
		return true;
	}
}

// bigheadk for Calibration
#define USE_MULTI_WARPMAP 1
#if USE_MULTI_WARPMAP
#include <thread>

typedef struct _warpmapInfo
{
	int wdt, hgt;
	int owdt, ohgt;
	ComposeHelper* p_helper;
	CamInput* p_cinp;
	cv::Rect* p_wInfoRoi;
	cv::Point* p_wInfoCenter;
	int* p_wInfoWarpMap;
	int* p_wInfoInvWarpMap;
	int idx;
}warpmapInfo;

void warpmap(warpmapInfo& w_info)
{
	ComposeHelper* p_helper = w_info.p_helper;
	CamInput& inp = *(w_info.p_cinp);

	int input_wdt = inp.crop_right - inp.crop_left;
	int input_hgt = inp.crop_bottom - inp.crop_top;

	cv::Rect crop_rect;
	crop_rect.x = inp.crop_left;
	crop_rect.y = inp.crop_top;
	crop_rect.width = input_wdt;
	crop_rect.height = input_hgt;
	p_helper->makeWarpMap(inp.width, inp.height, w_info.p_wInfoWarpMap, crop_rect, w_info.owdt, w_info.ohgt);
	//p_helper->computeRoiFit(inp.width, inp.height, w_info.wdt, w_info.hgt, crop_rect, *w_info.p_wInfoRoi, *w_info.p_wInfoCenter);
	p_helper->computeRoiFit_makeInvWarpMap(inp.width, inp.height, w_info.wdt, w_info.hgt, crop_rect, *w_info.p_wInfoRoi, *w_info.p_wInfoCenter, w_info.p_wInfoInvWarpMap);

	p_helper->computeCenterPoint(inp.width, inp.height, w_info.wdt, w_info.hgt, crop_rect, *w_info.p_wInfoRoi, *w_info.p_wInfoCenter);
}
#endif
void StitchComposer::updateWarpMap(WarpInfo* warp_info) {
#if USE_MULTI_WARPMAP
	std::vector<std::thread> thlist;
#endif

	for (int i = 0; i < input->count(); i++)
	{
#if USE_MULTI_WARPMAP
		warpmapInfo w_info;
		w_info.wdt = width;
		w_info.hgt = height;
		w_info.owdt = outputWidth;
		w_info.ohgt = outputHeight;
		w_info.p_helper = (ComposeHelper*)helpers[i];
		w_info.p_cinp = input->camIn(i);
		w_info.p_wInfoRoi = &(warp_info[i].roi);
		w_info.p_wInfoCenter = &(warp_info[i].center_point);
		w_info.p_wInfoWarpMap = warp_info[i].warpMap;
		w_info.p_wInfoInvWarpMap = warp_info[i].inv_warpMap;
		w_info.idx = i;

		thlist.push_back(std::thread(warpmap, w_info));
#else
		CamInput& inp = *(input->camIn(i));

		int input_wdt = inp.crop_right - inp.crop_left;
		int input_hgt = inp.crop_bottom - inp.crop_top;

		cv::Rect crop_rect;
		crop_rect.x = inp.crop_left;
		crop_rect.y = inp.crop_top;
		crop_rect.width = input_wdt;
		crop_rect.height = input_hgt;

		helper(i).makeWarpMap(inp.width, inp.height, warp_info[i].warpMap, crop_rect, outputWidth, outputHeight);
		helper(i).computeRoiFit(inp.width, inp.height, width, height, crop_rect, warp_info[i].roi, warp_info[i].center_point);
		helper(i).computeCenterPoint(inp.width, inp.height, width, height, crop_rect, warp_info[i].roi, warp_info[i].center_point);
#endif
	}

#if USE_MULTI_WARPMAP
	for (int i = 0; i < thlist.size(); i++){
		thlist[i].join();
	}
	thlist.clear();
#endif
}
void* Composer::getOverlayPBOAddress(int id)
{
	for (int i = 0; i < overlayInputs.size(); i++)
	{
		if (overlayInputs[i]->id == id)
		{
			CamInputImageFile* cam = (CamInputImageFile*)overlayInputs[i]->caminput;
			return cam->getPBOAddress(-1);
		}
	}
	return 0;

}
void StitchComposer::savePanoramaImage(char* path)
{
	isSave = true;
	pano_file_path = path;
}
void StitchComposer::destroyInputColors()
{
	free(colors);
}

void StitchComposer::destroy()
{
	input = NULL;

	if (nadir)
	{
		((CamInputNadir*)nadir)->destroy();
		nadir->clear();
		delete nadir;
	}
	nadir = NULL;

	for (int i = 0; i < overlayInputs.size(); i++)
	{
		overlayInputs[i]->caminput->clear();

		if (overlayInputs[i]->helper)
			delete overlayInputs[i]->helper;

		overlayInputs[i]->helper = NULL;

		if (overlayInputs[i]->right_helper)
			delete overlayInputs[i]->right_helper;

		overlayInputs[i]->right_helper = NULL;
		overlayInputs.erase(overlayInputs.begin() + i);
	}

	overlayInputs.clear();
	for (size_t i = 0; i < helpers.size(); i++)
	{
		if (helpers[i])
		{
			delete helpers[i];
		}
	}
	helpers.clear();
	if (nadir_helper)
	{
		delete nadir_helper;
	}
	nadir_helper = NULL;

	if (exposureCompensator)
	{
		exposureCompensator->destroy();
		delete exposureCompensator;
	}

	exposureCompensator = NULL;

	destroyInputColors();
	destroyOutput();

#if LICENSE_PERSONAL_KEY
	if (watermark_buffer != NULL)
		free(watermark_buffer);
	watermark_buffer = NULL;

	glDeleteTextures(1, &watermarkTex);
#endif

	for (int i = 0; i < mNumCamera; i++)
	{
		buf[i]->deleteGL();
	}
	cum.deleteGL();
	merger.deleteGL();

	glDeleteTextures(1, &resTex);
	glDeleteTextures(1, &warpTex);
	glDeleteTextures(1, &outTex);
	glDeleteFramebuffers(1, &resFBO);
	glDeleteFramebuffers(1, &warpFBO);
	glDeleteFramebuffers(1, &outFBO);
}

void StitchComposer::setGrayscaleMode(int mode)
{
	grayscale_mode = mode;
}
void StitchComposer::setBrightnessStep(int step)
{
	brightness_step = step*BRIGHTNESS_FACTOR;
}
void StitchComposer::setContrastStep(int step)
{
	if (step >= 0)
		contrast_step = 1.0 + step*CONTRAST_FACTOR;
}
void StitchComposer::setStereoMode(int mode)
{
	stitchingMode = mode;

	//if (m_outputManager)
	//	m_outputManager->setStereo(mode);
}
void StitchComposer::setPrepareMode(bool mode)
{
	mPrepareMode = mode;
}