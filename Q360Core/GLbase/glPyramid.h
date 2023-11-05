//
//  glPyramid.h
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/19/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#ifndef glPyramid_h
#define glPyramid_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glew.lib" )
#endif

struct GLPyramid {
	int w, h;
	GLuint type, DEPTH;
	GLuint *gaussianFBO, *gaussianTex;
	const GLuint& operator[] (size_t i) const { return gaussianTex[i]; }
	GLuint& operator[] (size_t i) { return gaussianTex[i]; }
	GLPyramid(int width, int height, int depth, GLuint intType=GL_RGBA32F_ARB, bool initGL=false);
	void initGL();
	void deleteGL();
	~GLPyramid();

	GLPyramid& operator += (const GLPyramid& p);
	void clear();
	void merge(const GLPyramid& p, GLuint texid);
	void buildPyramid();
	void addLaplacian(int num_input, GLPyramid** p, float contrast_step);
	void setDepth(int d);
protected:
};

struct RectMaskPyramid: GLPyramid {
	RectMaskPyramid(int width, int height, int intType=GL_RGBA, bool initGL=true)
	: GLPyramid(width,height,intType,initGL){}
	void setImage(void* buf,int w,int h,
				  float sx=0,float sy=0,float sw=-1,float sh=-1);
};

#endif /* glPyramid_h */
