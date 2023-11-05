//
//  simpleTex.cpp
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/20/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#include "GLTexMask.h"
#include "glsl.h"
#include "glProg.h"
#include "../stitchingMode.h"

void GLTexMask::initGL() {
	if( tex == 0 ) {
		glGenTextures(1,&tex);
		glBindTexture(GL_TEXTURE_2D,tex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	if( mask == 0 ) {
		glGenTextures(1,&mask);
		glBindTexture(GL_TEXTURE_2D,mask);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	if (lut0 == 0) {
		glGenTextures(1, &lut0);
		//glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_1D, lut0);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//lutset = true;
	}
	if (lut1 == 0) {
		glGenTextures(1, &lut1);
		//glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_1D, lut1);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	if (lut2 == 0) {
		glGenTextures(1, &lut2);
		//glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_1D, lut2);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	if (map == 0) {
		glGenTextures(1, &map);
		glBindTexture(GL_TEXTURE_2D, map);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	if (before_map == 0) {
		glGenTextures(1, &before_map);
		glBindTexture(GL_TEXTURE_2D, before_map);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
}
void GLTexMask::setMap(void* b, int ww, int hh, int level) {
	if (map == 0) return;
	glBindTexture(GL_TEXTURE_2D, map);
	
	//if (mapSet == false) {
		mapw = ww; maph = hh;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mapw / level, maph / level, 0, GL_RGBA, GL_UNSIGNED_BYTE, b);
		mapSet = true;
	//}
	//else if (mapw == ww && maph == hh)
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, mapw / level, maph / level, GL_RGBA, GL_UNSIGNED_BYTE, b);
	//else std::cerr << "Texture and input size mismatch\n";
	glBindTexture(GL_TEXTURE_2D, 0);
}
void GLTexMask::setBeforeMap(void* b, int ww, int hh, int level)
{
	if (before_map == 0) return;
	glBindTexture(GL_TEXTURE_2D, before_map);

	//if (!beforeSet)
	//{
		bmapw = ww; bmaph = hh;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, bmapw / level, bmaph / level, 0, GL_RGBA, GL_UNSIGNED_BYTE, b);
		beforeSet = true;
	//}
	//else if (bmapw == ww && bmaph == hh)
	//	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, bmapw / level, bmaph / level, GL_RGBA, GL_UNSIGNED_BYTE, b);
	//else std::cerr << "Texture and input size mismatch\n";
	
	resetMapAnimation = false;
	glBindTexture(GL_TEXTURE_2D, 0);
}
void GLTexMask::setLut(void* r, void* g, void* b, int ww, int gainMode)
{
	//printf("JW, setLut start (lut0, lut1, lut2) = (%d, %d, %d)\n", lut0, lut1, lut2);
	if (lut0 == 0 || lut1 == 0 || lut2 == 0) return;

	if (!lutset) {
		/*glBindTexture(GL_TEXTURE_1D, lut);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);*/

		//glBindTexture(GL_TEXTURE_1D, lut0);
		//glActiveTexture(GL_TEXTURE2);
		
		//glTexImage1D(GL_TEXTURE_1D, 0, GL_ALPHA, ww, 0, GL_ALPHA, GL_UNSIGNED_BYTE, b);
		lutset = true;
		printf("JW, setLut success!!\n");
	}

	//printf("JW, setLut end\n");
}
void GLTexMask::setImage(void* b,int ww, int hh) {
	if( tex==0 ) return;
	glBindTexture(GL_TEXTURE_2D,tex);
	if( !imageSet ) {
		w = ww; h=hh;
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,w,h,0,GL_RGB,GL_UNSIGNED_BYTE,b);
		imageSet = true;
	}
	else if( w==ww && h==hh )
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,w,h,GL_RGB,GL_UNSIGNED_BYTE,b);
	else std::cerr<<"Texture and input size mismatch\n";
	glBindTexture(GL_TEXTURE_2D,0);
}
void GLTexMask::setImageRGBA(void* b, int ww, int hh) {
	if (tex == 0) return;
	glBindTexture(GL_TEXTURE_2D, tex);
	if (!imageSet) {
		w = ww; h = hh;
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, b);
		imageSet = true;
	}
	else if (w == ww && h == hh)
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, b);
	else{ std::cerr << "Texture and input size mismatch\n"; }
	glBindTexture(GL_TEXTURE_2D, 0);
}
void GLTexMask::setMask(void* b, int ww, int hh) {
	if (mask == 0) return;
	glBindTexture(GL_TEXTURE_2D,mask);
	if( !maskSet ) {
		mw = ww; mh=hh;
		glTexImage2D(GL_TEXTURE_2D,0,GL_ALPHA,mw,mh,0,GL_ALPHA,GL_UNSIGNED_BYTE,b);
		maskSet = true;
	}
	else if( mw==ww && mh==hh )
		glTexSubImage2D(GL_TEXTURE_2D,0,0,0,mw,mh,GL_ALPHA,GL_UNSIGNED_BYTE,b);
	else std::cerr<<"Mask and input size mismatch\n";
	glBindTexture(GL_TEXTURE_2D,0);
}
void GLTexMask::subMask(void* b, int cx, int cy, int ww, int hh) {
	printf("mask %d, maskSet %d,\n", mask, maskSet);
	if( mask==0 ) return;
	if( !maskSet ) return;
	glBindTexture(GL_TEXTURE_2D,mask);
	glTexSubImage2D(GL_TEXTURE_2D,0,cx,cy,ww,hh,GL_ALPHA,GL_UNSIGNED_BYTE,b);
	glBindTexture(GL_TEXTURE_2D,0);
}

void GLTexMask::clear() {
	if (map) glDeleteTextures(1, &map); map = 0;
	if (before_map) glDeleteTextures(1, &before_map); before_map = 0;
	if( tex ) glDeleteTextures(1,&tex ); tex = 0;
	if( mask ) glDeleteTextures(1,&mask ); mask = 0;
	if (lut0) glDeleteTextures(1, &lut0); lut0 = 0;
	if (lut1) glDeleteTextures(1, &lut1); lut1 = 0;
	if (lut2) glDeleteTextures(1, &lut2); lut2 = 0;
	w = 0; h = 0; imageSet = false, maskSet = false, mapSet = false, beforeSet = false;
}