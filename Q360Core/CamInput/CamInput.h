
#ifndef __CAM_INPUT_QURAM_H__
#define __CAM_INPUT_QURAM_H__

#ifdef WIN32
#include <GL\glew.h>
#pragma comment(lib, "glew.lib")
#endif
#include <string>
#include "GLBase/glTexMask.h"
#include "../PTSParam.h"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "../Quram360Engine.h"

#include "../StitchingMode.h"
#include "../ExposureCompensation.h"

//#define USE_XML
#ifndef USE_XML
//#define NUM_CAMERA 6
#endif

#define NADIR_WIDTH		1024
#define NADIR_HEIGHT	1024

#define IN_BUFFER_CNT 4

struct CamInput {
	enum InFormat {
		UNDEF	= 0,
		RGB8	= 1,
		RGBA8	= 2,
		YUV420	= 3,
		YUV444	= 4
	};
	bool connected, glInited, maskSet, dirty;
	int width, height;
	int		depth;
	int		data_step;
	int		elemSize;
	int		lens_type;
	int		input_type;
	int		state;
	int		output_type = EQUIRECTANGULAR;
	int		output_fov = 360;
	InFormat format;
	void* maskBuf; int maskW, maskH;

	float tr[6];		// Translation rotation
	float fov;			// Field of View
	float dist[4];		// Distortion parameter

	float diff_tr[2];

	int crop_left, crop_right, crop_top, crop_bottom;
	
	ExposureCompensation exposureCompensation;
	
	virtual int connect() = 0;
	virtual int refresh(int pbo_idx)=0;
	virtual int refresh(int pbo_idx, GLuint texid)=0;
	virtual int disconnect()=0;
	virtual void* getPBOAddress(int pbo_idx) = 0;

	virtual void clear();
	virtual void initGL();

	int		getWidth() { return width; }
	int		getHeight() { return height; }
	int		getDepth() { return depth; }
	CamInput()
		: connected(false), glInited(false), depth(3), width(0), height(0), format(UNDEF), maskBuf(0), maskW(0), lens_type(FULL_FRAME_FISHEYE), input_type(FOR_360_INPUT), state(INPUT_STATE_DRAW){
	}
	void	update(PTSParam param, int input_width, int input_height, int depth, int input_type);

	void	setTransform(float trf[6]);
	void	setTransform(float yaw, float pitch, float roll, float tx, float ty, float tz);
	void	setFov(float FOV);
	void	setDistParam(float distParam[4]);
	void	setDistParam(float a, float b, float c, float d);
	//void	setPTSParam(PTSParam param);
	void	setPTSParam(PTSParam& param);

	void	setState(int value);
	int		getState();

	void	setMask(const void* buf, int w, int h);
	void	setImage(void* buf, int w, int h);
	void	setImageRGBA(void* buf, int w, int h);
	void	setLut(void* buf, int w);
	virtual GLuint getImageTex() { return tex.tex; }
	virtual GLuint getMaskTex() { return tex.mask; }
	virtual GLuint getLUT0() { return tex.lut0; }
	virtual GLuint getLUT1() { return tex.lut1; }
	virtual GLuint getLUT2() { return tex.lut2; }
	virtual GLuint getMap() { return tex.map; }
	virtual GLuint getBeforeMap() { return tex.before_map; }
	virtual GLTexMask& getTexMask() { return tex; }

	virtual int isMapSet() { return tex.isMapSet(); }
	virtual int isMapAnimation() { return tex.isMapAnimation(); }
	virtual int getMapW() { return tex.getMapW(); }
	virtual int getMapH() { return tex.getMapH(); }

	static CamInput* create(PTSParam param, int input_width, int input_height, int depth, int input_type, int stitching_mode);
	static CamInput* create(PTSParam param, int input_width, int input_height, int input_type, int depth, void* ptr);
	static CamInput* createNadir(PTSParam param, std::string fn, int input_width, int input_height, int input_type);
protected:
	GLTexMask tex;
};


struct CamInputImage : public CamInput {
	int width;
	int height;
	int channel;

	CamInputImage(int wdt, int hgt, int depth, void* ptr)
	{

		width = wdt; 
		height = hgt; 
		channel = depth;

		initGL();
		setImageRGBA(ptr, width, height);
	}

	int update(int wdt, int hgt, void* ptr)
	{
		return QVS_SUCCESS;
	}
	
	void destroy()
	{
		clear();
	}

	int		connect();
	int		refresh(int pbo_idx);
	int		refresh(int pbo_idx, GLuint texid);
	int		disconnect();
	void*	getPBOAddress(int pbo_idx);
};

struct CamInputNadir : public CamInput {
	void* buffer;

	std::string filePath;
	int width;// = NADIR_WIDTH;
	int height;// = NADIR_HEIGHT;

	CamInputNadir(std::string fn, int wdt, int hgt)
	{
		filePath = fn;
		cv::Mat img;
		img = cv::imread(fn, CV_LOAD_IMAGE_UNCHANGED);

		width = wdt; height = hgt; depth = img.channels();
		buffer = malloc(width*height * 4);

		if (!img.empty())
		{
			if (depth == 3)
			{
				cv::cvtColor(img, img, CV_BGR2RGBA);
			}
			else
			{
				cv::cvtColor(img, img, CV_BGRA2RGBA);
			}

			if (img.cols != width || img.rows != height)
			{
				cv::resize(img, img, cv::Size(width, height));
			}
			memcpy(buffer, img.data, width*height * 4);
		}
		else{
			fprintf(stdout, "\nNadir image loading fail : %s...\n", fn.c_str());
			memset(buffer, 0, width*height * 4);
		}

		initGL();
		setImageRGBA(buffer, width, height);

		img.release();
	}
	int update(std::string fn, int wdt, int hgt)
	{
		return QVS_SUCCESS;
	}
	int updateNadir(const char* path)
	{
		filePath = path;
		cv::Mat img;
		img = cv::imread(path, CV_LOAD_IMAGE_UNCHANGED);
		if (img.data == NULL)
		{
			return QVS_MSG_FILE_CORRUP_ERROR;
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

			if (img.cols != width || img.rows != height)
			{
				cv::resize(img, img, cv::Size(width, height));
			}

			memcpy(buffer, img.data, width*height * 4);
			setImageRGBA(buffer, width, height);

			img.release();
			return QVS_SUCCESS;
		}
	}
	void destroy()
	{
		if (buffer != NULL)
		{
			free(buffer);
			buffer = NULL;
		}
	}

	int		connect();
	int		refresh(int pbo_idx);
	int		refresh(int pbo_idx, GLuint texid);
	int		disconnect();
	void*	getPBOAddress(int pbo_idx);
};

struct CamInputPBO: public CamInput {
	GLuint pbo[IN_BUFFER_CNT];
	int inBuf, outBuf;
	void** ptr;
	CamInputPBO(): CamInput(){pbo[0]=0; }
	virtual void connectPBO(int w, int h, int depth) {

		printf("bigheadk, w, h = %d, %d\n", w, h);
		glGenBuffers(IN_BUFFER_CNT,pbo);
		ptr = (void**)malloc(sizeof(void*)*IN_BUFFER_CNT);

		for(int i=0; i<IN_BUFFER_CNT; i++) { 
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[i]);
			glBufferDataARB(GL_PIXEL_UNPACK_BUFFER_ARB, w*h * depth, NULL, GL_STREAM_DRAW_ARB);
			ptr[i] = glMapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
			glUnmapBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB);
			glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
		}

		inBuf = 0;
		outBuf = -1;
	}
	virtual void* getAddress() {
		return ptr;
	}
	virtual void finishUpdate(GLuint texId, int w, int h, int depth, int pbo_idx) {
		if (pbo_idx < 0 && outBuf != -1)
		{
			pbo_idx = outBuf;
		}		
		if (pbo_idx < 0)
			return;

		//Upload to GPU
		//glFinish();
		//double tick = QClock();
		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, pbo[pbo_idx]);
		glMapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB, GL_WRITE_ONLY_ARB);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER_ARB);
		glBindTexture(GL_TEXTURE_2D, texId);
		if (depth == 4)
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, 0);
		else
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, 0);

		glBindBufferARB(GL_PIXEL_UNPACK_BUFFER_ARB, 0);
	    //glFinish();
		//printf("JW Upload to GPU = %f\n", QClock() - tick);
		//Upload to GPU end
		// NOTICE: if the reader is slow, reader will skip frames
	}
	virtual void* getAddress(int buffer_idx)
	{
		if (buffer_idx < 0)
		{
			buffer_idx = inBuf;
		}
		return ptr[buffer_idx];
	}
	GLuint* getPBOID() {
		return pbo;
	}
	void destroy()
	{
		glDeleteBuffers(IN_BUFFER_CNT, pbo);
	}
};


struct CamInputCaptureBoard: public CamInput  {
};

struct CamInputUSB: public CamInput {
};

struct CamInputImageFile: public CamInputPBO {
	std::string filename;
	CamInputImageFile() {}

	void	setFilename(const std::string& fn );
	int		connect();
	int		refresh(int pbo_idx);
	int		refresh(int pbo_idx, GLuint texid);
	int		disconnect();
	void*	getPBOAddress(int pbo_idx);
};

struct CamInputStitchedBuffer : public CamInputPBO {
	CamInputStitchedBuffer() {}

	int		connect();
	int		refresh(int pbo_idx, GLuint texid);
	int		refresh(int pbo_idx);
	int		disconnect();
	void*	getPBOAddress(int pbo_idx);
};


#endif // __CAM_INPUT_QURAM_H__
