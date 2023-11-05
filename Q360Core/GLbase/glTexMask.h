//
//  simpleTex.hpp
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/20/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#ifndef glTexMask_hpp
#define glTexMask_hpp

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glew.lib" )
#endif

struct GLTexMask {
	GLuint tex, mask, lut0, lut1, lut2, map, before_map;
	int w, h, mw, mh, mapw, maph, bmapw, bmaph;
	GLTexMask(void* buf=0, int ww=0, int hh=0)
		:w(ww), h(hh), tex(0), mask(0), lut0(0), lut1(0), lut2(0), map(0), before_map(0), imageSet(false), maskSet(false), lutset(false), mapSet(false), beforeSet(false), resetMapAnimation(false){
		//if( ww>0 && hh>0 ) setImage(buf,w,h);
	}
	void setImage(void* b,int ww, int hh);
	void setImageRGBA(void* b, int ww, int hh);
	void setMask(void* b, int ww, int hh);
	void subMask(void* b, int cx, int cy, int ww, int hh);
	void setLut(void* r, void* g, void* b, int ww, int gainMode);
	void setMap(void* b, int ww, int hh, int level = 1);
	void setBeforeMap(void* b, int ww, int hh, int level = 1);
	void initGL();
	bool isLutSet(void) { return lutset; }
	bool isMapSet(void) { return mapSet; }
	bool isMapAnimation(void) { return resetMapAnimation; }
	bool isMaskSet( void ) { return maskSet; }
	bool isImageSet( void ) { return imageSet; }
	bool isGLInited( void ) { return tex!=0; }
	int getMapW() { return mapw; }
	int getMapH() { return maph; }
	void clear();
	void resetMap() { mapSet = false; };
	//void resetAnimMap() { mapSet = 2; };
	void doResetAnimation() { resetMapAnimation = true; }
	void resetBeforeMap() { beforeSet = false; };
	virtual ~GLTexMask() { clear(); }
protected:
	bool imageSet, maskSet, lutset, beforeSet, mapSet, resetMapAnimation;
};

#endif /* simpleTex_hpp */
