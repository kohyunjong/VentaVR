//
//  glPyramid.cpp
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/19/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#include "glPyramid.h"
#include "glsl.h"
#include "GLProg.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

GLPyramid::GLPyramid(int width, int height, int depth, GLuint inttype, bool initGL_)
: w(width),h(height),type(inttype) {
	DEPTH=depth;
	gaussianFBO=new GLuint[DEPTH];
	gaussianTex=new GLuint[DEPTH];
	gaussianTex[0] = 0;
	if( initGL_) initGL();
}
GLPyramid::~GLPyramid() {
	glDeleteFramebuffers(DEPTH,gaussianFBO);
	glDeleteTextures(DEPTH,gaussianTex);
	delete gaussianFBO;
	delete gaussianTex;
}

void GLPyramid::deleteGL()
{
	glDeleteTextures(DEPTH, gaussianTex);
	glDeleteFramebuffers(DEPTH, gaussianFBO);
	delete gaussianFBO;
	delete gaussianTex;
}
	
void GLPyramid::initGL() {
	if( gaussianTex[0]==0 ) {
		glGenFramebuffers(DEPTH, gaussianFBO);
		glGenTextures(DEPTH,gaussianTex);
		for(size_t i=0; i<DEPTH; i++) {
			setupTexture(gaussianTex[i], (w+(1<<i)-1)>>i, (h+(1<<i)-1)>>i, type, NULL);
			glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
			glBindTexture(GL_TEXTURE_2D, gaussianTex[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gaussianTex[i], 0);
		}
	}
}

void GLPyramid::setDepth(int d) {
	DEPTH = d;
}

void GLPyramid::buildPyramid() {
	glDisable(GL_BLEND);
	for(size_t i=1; i<DEPTH; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
		glViewport(0,0,(w+(1<<i)-1)>>i,(h+(1<<i)-1)>>i);
		glClearColor(0,0,0,0);
		glClear(GL_COLOR_BUFFER_BIT);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D,gaussianTex[i-1]);
		drawRect();
	}
}

void GLPyramid::addLaplacian(int num_input, GLPyramid** p, float contrast_step)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	for (size_t i = 0; i<DEPTH - 1; i++) {
		glViewport(0, 0, (w + (1 << i) - 1) >> i, (h + (1 << i) - 1) >> i);
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
		switch (num_input)
		{
		case 1:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1]);
			break;
		case 2:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1]);
			break;
		case 3:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1], p[2]->gaussianTex[i], p[2]->gaussianTex[i + 1]);
			break;
		case 4:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1], p[2]->gaussianTex[i], p[2]->gaussianTex[i + 1],
				p[3]->gaussianTex[i], p[3]->gaussianTex[i + 1]);
			break;
		case 5:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1], p[2]->gaussianTex[i], p[2]->gaussianTex[i + 1],
				p[3]->gaussianTex[i], p[3]->gaussianTex[i + 1], p[4]->gaussianTex[i], p[4]->gaussianTex[i + 1]);
			break;
		case 6:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1], p[2]->gaussianTex[i], p[2]->gaussianTex[i + 1],
				p[3]->gaussianTex[i], p[3]->gaussianTex[i + 1], p[4]->gaussianTex[i], p[4]->gaussianTex[i + 1], p[5]->gaussianTex[i], p[5]->gaussianTex[i + 1]);
			break;
		case 7:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1], p[2]->gaussianTex[i], p[2]->gaussianTex[i + 1],
				p[3]->gaussianTex[i], p[3]->gaussianTex[i + 1], p[4]->gaussianTex[i], p[4]->gaussianTex[i + 1], p[5]->gaussianTex[i], p[5]->gaussianTex[i + 1], 
				p[6]->gaussianTex[i], p[6]->gaussianTex[i + 1]);
			break;
		case 8:
			fastlaplacianProg.use(num_input, contrast_step, p[0]->gaussianTex[i], p[0]->gaussianTex[i + 1], p[1]->gaussianTex[i], p[1]->gaussianTex[i + 1], p[2]->gaussianTex[i], p[2]->gaussianTex[i + 1],
				p[3]->gaussianTex[i], p[3]->gaussianTex[i + 1], p[4]->gaussianTex[i], p[4]->gaussianTex[i + 1], p[5]->gaussianTex[i], p[5]->gaussianTex[i + 1],
				p[6]->gaussianTex[i], p[6]->gaussianTex[i + 1], p[7]->gaussianTex[i], p[7]->gaussianTex[i + 1]);
			break;
		}
		drawRect();
	}
	fastlaplacianProg.unuse();

	int i = DEPTH - 1;
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
	switch (num_input)
	{
	case 1:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i]);
		break;
	case 2:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i]);
		break;
	case 3:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i], p[2]->gaussianTex[i]);
		break;
	case 4:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i], p[2]->gaussianTex[i], p[3]->gaussianTex[i]);
		break;
	case 5:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i], p[2]->gaussianTex[i], p[3]->gaussianTex[i],
			p[4]->gaussianTex[i]);
		break;
	case 6:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i], p[2]->gaussianTex[i], p[3]->gaussianTex[i],
			p[4]->gaussianTex[i], p[5]->gaussianTex[i]);
		break;
	case 7:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i], p[2]->gaussianTex[i], p[3]->gaussianTex[i],
			p[4]->gaussianTex[i], p[5]->gaussianTex[i], p[6]->gaussianTex[i]);
		break;
	case 8:
		fastalphaedProg.use(num_input, p[0]->gaussianTex[i], p[1]->gaussianTex[i], p[2]->gaussianTex[i], p[3]->gaussianTex[i],
			p[4]->gaussianTex[i], p[5]->gaussianTex[i], p[6]->gaussianTex[i], p[7]->gaussianTex[i]);
		break;
	}
	glViewport(0, 0, (w + (1 << i) - 1) >> i, (h + (1 << i) - 1) >> i);
	drawRect();
	fastlaplacianProg.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLPyramid& GLPyramid::operator += (const GLPyramid& p) {
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
	for(size_t i=0; i<DEPTH-1; i++) {
		glViewport(0,0,(w+(1<<i)-1)>>i,(h+(1<<i)-1)>>i);
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
		laplacianProg.use(p[i], p[i+1]);
		drawRect();
	}
	laplacianProg.unuse();
	
	int i=DEPTH-1;
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
	alphaedProg.use(p[i]);
	glViewport(0,0,(w+(1<<i)-1)>>i,(h+(1<<i)-1)>>i);
	drawRect();
	alphaedProg.unuse();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
	return *this;
}
void GLPyramid::clear() {
	glClearColor(0,0,0,0);
	for(size_t i=0; i<DEPTH; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
		glViewport(0,0,(w+(1<<i)-1)>>i,(h+(1<<i)-1)>>i);
		glClear(GL_COLOR_BUFFER_BIT);
	}
}
void GLPyramid::merge(const GLPyramid& p, GLuint texid) {
	glClearColor(0,0,0,0);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE,GL_ONE);
#if 1
	int i=DEPTH-1;
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
	glViewport(0,0,(w+(1<<i)-1)>>i,(h+(1<<i)-1)>>i);
	glClear(GL_COLOR_BUFFER_BIT);
	normalizeProg.use(p[i]);
	drawRect();
	for(int i=(int)DEPTH-2; i>=1; i--) {
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[i]);
		glViewport(0,0,(w+(1<<i)-1)>>i,(h+(1<<i)-1)>>i);
		glClear(GL_COLOR_BUFFER_BIT);
		laplacianMergeProg.use(gaussianTex[i + 1], p[i]);
		drawRect();

	}
	int idx = 0;
	if(texid == 0)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[idx]);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, texid);
	}
	glViewport(0, 0, w, h);
	glClear(GL_COLOR_BUFFER_BIT);
	laplacianMergeProg.use(gaussianTex[idx + 1], p[idx]);
	drawRect();
	//normalizeA0Prog.use(gaussianTex[idx + 1]);
	//drawRect();
	//normalizeA0Prog.unuse();
#else
	for(int i=DEPTH-2; i>=0; i--) {
		glBindFramebuffer(GL_FRAMEBUFFER, p.gaussianFBO[i]);
		glViewport(0,0,w>>i,h>>i);
		if( i==0 || i==DEPTH-2 )	normalizeProg.use(p.gaussianTex[i+1]);
		else						normalizeA0Prog.use(p.gaussianTex[i+1]);
		drawRect();
		if( i==0 )	normalizeProg.unuse();
		else		normalizeA0Prog.unuse();
	}

#endif
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}


void RectMaskPyramid::setImage(void* buf, int w, int h, float sx, float sy, float sw, float sh){
	glBindTexture(GL_TEXTURE_2D,gaussianTex[0]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, buf);
	glBindFramebuffer(GL_FRAMEBUFFER, gaussianFBO[0]);
	glColorMask(false, false, false, true);
	glClearColor(0,0,0,0);
	glClear(GL_COLOR_BUFFER_BIT);
	glEnable(GL_SCISSOR_TEST);
	glScissor((int)(sx*w),(int)(sy*h),(int)((sw)*w),(int)((sh)*h));
	glClearColor(0,0,0,1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_SCISSOR_TEST);
	glColorMask(true,true,true,true);
	buildPyramid();
}

