

// THIS CLASSES ARE UNDER DEVELOPMENT
// ....

#ifndef FRAME_OUTPUT_QURAM_H__
#define FRAME_OUTPUT_QURAM_H__
#include <GLbase/glTexMask.h>
#include <GLbase/glsl.h>
class FrameOutput {
public:
	bool glInited;
	int w, h;
	FrameOutput(int ww=0, int hh=0):glInited(false), w(ww), h(hh){}
	virtual void initGL() = 0;
	bool glInited(){ return glInited; }
	virtual unsigned int getFBO() =0;
	virtual unsigned int startVis() = 0;
	virtual void         endVis() = 0;
};

class GLTexFrameOutput : public FrameOutput {
public:
	GLuint resFBO, resTex;
	GLTexFrameOutput(int ww=0, int hh=0): FrameOutput(ww,hh),resFBO(0),resTex(0) {}
	virtual void initGL(){
		glGenFramebuffers(1, &resFBO);
		glGenTextures(1,&resTex);
		setupTexture(resTex, w, h, GL_RGBA, NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
		glBindTexture(GL_TEXTURE_2D, resTex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resTex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	virtual unsigned int getFBO() { return resFBO; }
	virtual unsigned int startVis(){ return resTex; }
	virtual void         endVis(){ }
};

class DXTexFrameOutput : public FrameOutput{
	GLuint resFBO, resTex;
	DXTexFrameOutput(int ww=0, int hh=0): FrameOutput(ww,hh),resFBO(0),resTex(0) {}
	virtual void initGL(){
		glGenFramebuffers(1, &resFBO);
		glGenTextures(1,&resTex);
		setupTexture(resTex, w, h, GL_RGBA, NULL);
		glBindFramebuffer(GL_FRAMEBUFFER, resFBO);
		glBindTexture(GL_TEXTURE_2D, resTex);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, resTex, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	virtual unsigned int getFBO() { return resFBO; }
	virtual unsigned int startVis(){ return resTex; }
	virtual void         endVis(){ }
};

#endif
