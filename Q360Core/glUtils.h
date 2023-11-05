#ifndef __QGLUTILS_H__
#define __QGLUTILS_H__

inline void beginOrtho(int w, int h, int img_w, int img_h) {
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	double aspectRatio = 0.f;
	
	/*if (w > h){
		aspectRatio = (double)w / (double)h;
		glOrtho(-1.0f, 1.0f, -1.0f* aspectRatio, 1.0f* aspectRatio, -1.0f, 1.0f);
	}
	else{
		aspectRatio = (double)w / (double)h;
		glOrtho(-1.0f, 1.0f, -1.0f* aspectRatio, 1.0f* aspectRatio, -1.0f, 1.0f);
	}*/
	//float fH = tanf(90 / 360.f*PI)*0.001, fW = fH*aspectRatio; glFrustum(-fW, fW, -fH, fH, 0.001, 10);
	//gluPerspective(90, (double)w / (double)h, 0.001, 10);
	/*if (w > h) glOrtho(-1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	else glOrtho(-1.0f, 1.0f, -1.0f * aspectRatio, 1.0f * aspectRatio, -1.0f, 1.0f);*/

	//glOrtho(-1.0f* aspectRatio, 1.0f* aspectRatio, -1.0f, 1.0f, -1.0f, 1.0f);
	glOrtho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);
}
inline void endOrtho() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

inline void beginPerspective(double fov, int w, int h) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	double aspectRatio = (double)w / (double)h;
	//float fH = tanf(fov / 360.f*PI)*0.001, fW = fH*aspectRatio; glFrustum(-fW, fW, -fH, fH, 0.001, 10);
	gluPerspective(fov, (double)w / (double)h, 0.001, 10);
	glPushMatrix();
}
inline void endPerspective() {
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}
#endif //__QGLUTILS_H__