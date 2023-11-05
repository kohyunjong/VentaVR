#include "stitcher.h"

struct RingBuf {
	float* v;
	int cur, last, K;
	RingBuf(int k) :cur(0), last(0), K(k) { v = new float[K]; }
	void push(float x) { v[cur] = x; cur = (cur + 1) % K; last++; last = last>K ? K : last; }
	float avg() { float sum = 0; for (int i = 0; i<last; i++) sum += v[i]; return sum / last; }
};

RingBuf fps(60);

void Stitcher::stitching(int w, int h, int* pbo_idx, int** input_state, InputTempData* input_tempData, int panorama_preview_mode, float currentScale, float currentX, float currentY, float animation_time, int stereoIndex, int editMode, int stitching_mode) {
	
	if (!glewInited) {
		glewInit(); glewInited = true;
	}

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	//glFinish();
	double tick = QClock();
	if (inputArray) {
		//if (stitching_mode != STITCHED_INPUT)
		{
			if (!inputArray->isGLInited()) inputArray->initGL();
			inputArray->refresh(pbo_idx);
		}
		//else
		{
			//inputArray->refreshStitchedBuffer(pbo_idx, composer->getResTex());
		}
		
	}
	
	//glFlush(); glFinish();
	//printf("JW GPU Upload = %f\n", QClock() - tick);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);

	
	if (composer) {
		
		if (!composer->isGLInited()) {
			composer->initGL();
		}
		if (stitching_mode != STITCHED_INPUT)
		{
			((WarpComposer*)composer)->updateTexure(input_tempData, input_state);
		}

		composer->refresh(input_state, animation_time, stereoIndex, editMode, stitching_mode);
	}
	


	if (((StitchComposer*)composer)->stitchingMode == WIDE_STITCHING)
		panorama_preview_mode = PANORAMA_PREVIEW_MID;
	
	if (panorama_preview_mode != 0) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		int x = 0;
		int y = 0;

		int wdt = w;
		int hgt = h;

		switch (panorama_preview_mode)
		{
		case PANORAMA_PREVIEW_FULL:
			break;
		case PANORAMA_PREVIEW_UP:
			if (wdt / 2 > hgt)
			{
				x += (wdt - (hgt * 2));
				wdt = hgt * 2;
			}
			else if (wdt / 2 < hgt)
			{
				y += (hgt - (wdt * 0.5));
				hgt = wdt / 2;
			}
			break;
		case PANORAMA_PREVIEW_MID:
			if (wdt / 2 > hgt)
			{
				x += (wdt - (hgt * 2)) / 2;
				wdt = hgt * 2;
			}
			else if (wdt / 2 < hgt)
			{
				y += (hgt - (wdt * 0.5)) / 2;
				hgt = wdt / 2;
			}
			break;
		case PANORAMA_PREVIEW_DOWN:
			if (wdt / 2 > hgt)
			{
				wdt = hgt * 2;
			}
			else if (wdt / 2 < hgt)
			{
				hgt = wdt / 2;
			}
			break;
		}
		glViewport(x, y, wdt, hgt);

		glPushMatrix();

		glPushMatrix();
		int output_w = ((StitchComposer*)composer)->outputWidth;
		int output_h = ((StitchComposer*)composer)->outputHeight;
		if(((StitchComposer*)composer)->stitchingMode == WIDE_STITCHING)
			glScalef(currentScale, output_h / (float)output_w * -2 * currentScale, 1);
		else
			glScalef(currentScale, -currentScale, 1);
		//glScalef(currentScale, -currentScale, 1);

		glTranslatef(-2 * (currentX - w / 2.0f) / (float)w, -2 * (currentY - h / 2.0f) / (float)h * (output_w / (float)output_h / 2), 0);

		//glScalef(currentScale, -currentScale, 1);
		//glTranslatef(-2 * (currentX - w / 2.0f) / (float)w, -2 * (currentY - h / 2.0f) / (float)h, 0);

		glColor4f(1, 1, 1, 1);
		glEnable(GL_TEXTURE_2D);
		if (composer)
		{
			if (((StitchComposer*)composer)->stitching_resolution_mode == STITCHING_2BY1)
				glBindTexture(GL_TEXTURE_2D, ((StitchComposer*)composer)->resTex);
			else if (((StitchComposer*)composer)->stitching_resolution_mode == STITCHING_16BY9)
				glBindTexture(GL_TEXTURE_2D, ((StitchComposer*)composer)->outTex);
		}
		drawRect();
	}
	

	alpha2ColorProg.unuse();


	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
	glFlush(); glFinish();
	//gl_font(FL_HELVETICA, 13);
	double dt = QClock() - tick;
	fps.push((float)dt);
	ttt[1024]; sprintf_s(ttt, 1024, "% 5.1f FPS % 5.1f ms", 1000 / fps.avg(), fps.avg());
			//printf("%s\n",ttt);
	//gl_draw(ttt, -1, -1);
}

void* Stitcher::getPBOAddress(int camera_idx, int buffer_idx)
{
	return inputArray->getPBOAddress(camera_idx, buffer_idx);
}

void* Stitcher::getOverlayPBOAddress(int idx)
{
	return composer->getOverlayPBOAddress(idx);
}

void Stitcher::create(int wdt, int hgt, int num_camera, int inputWdt, int inputHgt)
{
	numCamera = num_camera;

	composer = Composer::create(numCamera, wdt, hgt);
	createStitchingTemplate(wdt, hgt, inputWdt, inputHgt);

#ifdef WIN32
	glewInit();
#endif

}

void Stitcher::destroy()
{
	printf("bigheadk, stitching delete 0\n");	

	if (inputArray) {
		inputArray->clear(-1);
		delete inputArray;
	}		
	inputArray = NULL;
	printf("bigheadk, stitching delete 1\n");

	if (composer)
	{
		((StitchComposer*)composer)->destroy();
		delete composer;
	}
	composer = NULL;
	printf("bigheadk, stitching delete 2\n");

	stitchingTemplate->destroy();
	printf("bigheadk, stitching delete 3\n");
}

#ifdef USE_XML
void Stitcher::init()
#else
void Stitcher::init(PTSParam nadir_param, const char* nadir_path, int num_camera, int input_width, int input_height, int stitching_mode)
#endif
{

	if (!glewInited) {
		glewInit(); glewInited = true;
		//glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	}

#ifdef USE_XML
	if (inputArray) {
		if (!inputArray->isGLInited()) inputArray->initGL();
		inputArray->connect();
	}
#else
	if (inputArray)
		delete inputArray;

	inputArray = new CamInputArray(stitchingTemplate->getImageParams(), num_camera, input_width, input_height, FOR_360_INPUT, stitching_mode);
	//if (stitching_mode != STITCHED_INPUT)
	{
		if (!inputArray->isGLInited()) inputArray->initGL();
	}

	inputArray->connect();
#endif


	if (composer) {

		((StitchComposer*)composer)->setStereoMode(stitching_mode);
		composer->setInput(inputArray);
		composer->createNadir(nadir_param, nadir_path, NOT_FOR_360_INPUT);
		if (!composer->isGLInited()) {
			composer->initGL();
		}

		//composer->refresh();
		composer->refresh(0, 0, 0);
	}
	glFlush(); glFinish();
}

void Stitcher::createStitchingTemplate(int outWdt, int outHgt, int inputWdt, int inputHgt)
{
	stitchingTemplate = new StitchingTemplate(outWdt, outHgt, inputWdt, inputHgt, numCamera);
}

void Stitcher::deleteStitchingTemplate()
{

}

void Stitcher::updateStitchingTemplate(bool isDirty)
{
	PTSParam* image_params = stitchingTemplate->getImageParams();
	PTSParam* global_param = stitchingTemplate->getIGlobalParam();
	for (int i = 0; i < numCamera; i++)
	{
		//printf("updateStitchingTemplate i=%d, y=%f, p=%f, r=%f\n",i, image_params[i].y, image_params[i].p, image_params[i].r);
		image_params[i].out_f = global_param->out_f;
		inputArray->cams[i]->setPTSParam(image_params[i]);
		inputArray->cams[i]->dirty = isDirty;
	}
}

void Stitcher::updateDirty(bool isDirty)
{
	for (int i = 0; i < numCamera; i++)
	{
		//printf("updateStitchingTemplate i=%d, y=%f, p=%f, r=%f\n",i, image_params[i].y, image_params[i].p, image_params[i].r);
		inputArray->cams[i]->dirty = isDirty;
	}
}

void Stitcher::updateOverlay(int id)
{
	composer->updateOverlayInput(id);
}

void Stitcher::makeSeamMask(InputTempData* input_tempData, long long timeStame, int* buffer_idx, int edit_mode)
{
	PTSParam* image_params = stitchingTemplate->getImageParams();
	((StitchComposer*)composer)->makeSeamMask(image_params, input_tempData, buffer_idx, timeStame, edit_mode);
}
/*void Stitcher::calculatePixelAvg(InputTempData* input_tempData, int* buffer_idx)
{
	((StitchComposer*)composer)->calculatePixelAvg(input_tempData, buffer_idx);
}*/
void Stitcher::estimateExposureCompensation(InputTempData* input_tempData, int* buffer_idx, int stereoIdx)
{
	PTSParam* image_params = stitchingTemplate->getImageParams();
	((StitchComposer*)composer)->estimateExposureCompensation(input_tempData, buffer_idx, image_params, stereoIdx);
}
std::vector<cv::Mat> Stitcher::getCurve(InputTempData* input_tempData, long long timeStamp, std::vector<std::vector<std::vector<cv::Point2d>>> manualCurvePoints)
{
	std::vector<cv::Mat> result = ((StitchComposer*)composer)->getCurve(input_tempData, timeStamp, manualCurvePoints);

	return result;
}

int Stitcher::startOutput(int outputDevice, int outputDisplay)
{
	return ((StitchComposer*)composer)->startOutput(outputDevice, outputDisplay);
}

int Stitcher::finishOutput()
{
	return ((StitchComposer*)composer)->finishOutput();
}

void Stitcher::setBlendingMode(int mode)
{
	((StitchComposer*)composer)->blendMode = mode;
	if (mode == MULTIBAND_BLEND)
	{
		//((StitchComposer*)composer)->setDepth(5);
		((StitchComposer*)composer)->setDepth(8);
	}
	else if (mode == NORMAL_BLEND)
	{
		((StitchComposer*)composer)->setDepth(5);
	}
}

void Stitcher::setSeamMode(int mode)
{
	((StitchComposer*)composer)->setSeamMode(mode);
}

void Stitcher::setGainMode(int mode)
{
	((StitchComposer*)composer)->setGainMode(mode);
}

void Stitcher::setNadirCompensationMode(int mode)
{
	((StitchComposer*)composer)->setNadirMode(mode);
}

void Stitcher::updateNadirParam(PTSParam nadir_param)
{
	composer->updateNadir(nadir_param);
}

int Stitcher::updateNadirImage(const char* image_path)
{
	return composer->updateNadirImage(image_path);
}

void Stitcher::setManualRGB(int idx, int R, int G, int B)
{
	((StitchComposer*)composer)->setManualRGB(idx, R, G, B);
}

// bigheadk for calibration
void Stitcher::updateWarpMap(WarpInfo* warp_info)
{
	((StitchComposer*)composer)->updateWarpMap(warp_info);
}

int Stitcher::selectOverlay(int x, int y, int tplNum)
{
	return composer->selectOverlay(x, y, tplNum);
}

void Stitcher::addOverlayInput(int width, int height, int depth, int id, int tplNum, PTSParam* param, int outputType)
{
	composer->addOverlayInput(width, height, depth, id, tplNum, param, outputType);
}

void Stitcher::addOverlayInput(int width, int height, int depth, int id, void* ptr, int tplNum, PTSParam* param, int outputType)
{
	composer->addOverlayInput(width, height, depth, id, ptr, tplNum, param, outputType);
}

void Stitcher::modifyOverlayInput(int width, int height, int depth, int id, int targetID, int tplNum)
{
	composer->modifyOverlayInput(width, height, depth, id, targetID, tplNum);
}

void Stitcher::modifyOverlayInput(int width, int height, int depth, int id, int targetID, void* ptr, int tplNum)
{
	composer->modifyOverlayInput(width, height, depth, id, targetID, ptr, tplNum);
}

void Stitcher::destroyOverlayInput(int id)
{
	composer->destroyOverlayInput(id);
}

void Stitcher::getOverlayCenter(int id, float* cx, float* cy)
{
	composer->getOverlayCenter(id, cx, cy);
}

void Stitcher::savePanoramaImage(char* path)
{
	((StitchComposer*)composer)->savePanoramaImage(path);
}

void Stitcher::setOverlayState(int state, int id)
{
	composer->setOverlayState(state, id);
}

void Stitcher::setGrayscaleMode(int mode)
{
	((StitchComposer *)composer)->setGrayscaleMode(mode);
}
void Stitcher::setBrightnessStep(int step)
{
	((StitchComposer *)composer)->setBrightnessStep(step);
}
void Stitcher::setContrastStep(int step)
{
	((StitchComposer *)composer)->setContrastStep(step);
}
void Stitcher::setStereoMode(int mode)
{
	((StitchComposer*)composer)->setStereoMode(mode);
}
void Stitcher::setPrepareMode(bool mode)
{
	((StitchComposer*)composer)->setPrepareMode(mode);
}
void Stitcher::setCallback_setOutputLog(void* cb, void* data)
{
	((StitchComposer*)composer)->setCallback_setOutputLog(cb, data);
}