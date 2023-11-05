//
//  glsl.h
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/19/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#ifndef glsl_h
#define glsl_h

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/glew.h>
#include <GL/gl.h>
#pragma comment( lib, "opengl32.lib" )
#pragma comment( lib, "glew.lib" )
#endif
#include <string>
#include <iostream>
#define MAX_LENGTH 16656
#define PI 3.14159265358979323846
inline void __printProgramLog(GLuint obj) {
	int infologLength = 0; int maxLength;
	glGetProgramiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
#ifdef _WIN32
	char infoLog[MAX_LENGTH];
#else
	char infoLog[maxLength];
#endif
	glGetProgramInfoLog(obj, maxLength, &infologLength, infoLog);
	if( infologLength > 0 ) std::cerr<<infoLog;
}

inline void __printShaderLog(GLuint obj) {
	int infologLength = 0; int maxLength;
	glGetShaderiv(obj,GL_INFO_LOG_LENGTH,&maxLength);
#ifdef _WIN32
	char infoLog[MAX_LENGTH];
#else
	char infoLog[maxLength];
#endif
	glGetShaderInfoLog(obj, maxLength, &infologLength, infoLog);
	if( infologLength > 0 ) std::cerr<<infoLog;
}

inline GLuint loadVertShader    (const std::string& str,bool log=true) {
	int len = (int)str.length();
	const char* ptr = str.c_str();
	GLuint shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shader, 1, &ptr, &len);
	glCompileShader(shader);
	if( log ) __printShaderLog(shader);
	return shader;
}

inline GLuint loadFragShader    (const std::string& str,bool log=true) {
	int len = (int)str.length();
	const char* ptr = str.c_str();
	GLuint shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shader, 1, &ptr, &len);
	glCompileShader(shader);
	if( log ) __printShaderLog(shader);
	return shader;
}

inline GLuint makeProgram(GLuint vshader, GLuint fshader,bool log=true) {
	GLuint program = glCreateProgram();
	glAttachShader(program, vshader);
	glAttachShader(program, fshader);
	glLinkProgram(program);
	if(log) __printProgramLog(program);
	return program;
}

inline GLuint loadProgram(const std::string& v, const std::string& f,bool log=true) {
	GLuint vshader = loadVertShader(v,log);
	GLuint fshader = loadFragShader(f,log);
	return makeProgram(vshader,fshader,log);
}

inline void setupTexture(GLuint texid, int w, int h, GLuint fmt, void* data=NULL) {
	glBindTexture(GL_TEXTURE_2D,texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	GLuint format = GL_RGBA;
	GLuint type = GL_UNSIGNED_BYTE;
	
	switch(fmt) {
		case GL_RGBA		: type = GL_UNSIGNED_BYTE; format=GL_RGBA; break;
		case GL_RGB			: type = GL_UNSIGNED_BYTE; format=GL_RGB; break;
		case GL_RGBA32F_ARB : type = GL_FLOAT; format=GL_RGBA; break;
		case GL_RGB32F_ARB	: type = GL_FLOAT; format=GL_RGB; break;
		case GL_RGBA16F_ARB	: type = GL_HALF_FLOAT; format=GL_RGBA; break;
		case GL_RGB16F_ARB	: type = GL_HALF_FLOAT; format=GL_RGB; break;
	}
	glTexImage2D(GL_TEXTURE_2D, 0, fmt, w, h, 0, format, type, NULL);
	if( data!=NULL )
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, w, h, GL_RGB, GL_UNSIGNED_BYTE, data);
}

inline void drawRect() {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex2f(-1,1);
	glTexCoord2f(0, 0); glVertex2f(-1,-1);
	glTexCoord2f(1, 0); glVertex2f(1,-1);
	glTexCoord2f(1, 1); glVertex2f(1,1);
	glEnd();
}

inline void drawRect_Vertex3f(double z) {
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-1, -1, 0);
	glTexCoord2f(1, 0); glVertex3f(1, -1, 0);
	glTexCoord2f(1, 1); glVertex3f(1, 1, 0);
	glTexCoord2f(0, 1); glVertex3f(-1, 1, 0);
	glEnd();
}
inline void drawCircle_Vertex3f(double rad_x, double rad_y) {
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < 360; i++) {
		double angle, x, y;
		angle = i*PI / 180;
		x = rad_x*cos(angle);
		y = rad_y*sin(angle);
		glVertex3f(x, y, 0);
	}
	glEnd();
	glColor3f(1.0f, 1.0f, 1.0f);
}
inline void drawLine_Center() {
	glBegin(GL_LINE_LOOP);
	glTexCoord2f(0.5, 0); glVertex3f(0, -1, 0);
	glTexCoord2f(0.5, 1); glVertex3f(0, 1, 0);
	glEnd();
}

inline bool checkFBO(int i) {
	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch(status) {
		case GL_FRAMEBUFFER_COMPLETE: return true;
		default:
			std::cerr<<"something is wrong in FBO\n";
			return false;
	}
}




#endif /* glsl_h */
