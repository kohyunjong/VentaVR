
#ifndef __CAM_INPUT_ARRAY_QURAM_H__
#define __CAM_INPUT_ARRAY_QURAM_H__

#include "CamInput.h"
#include <vector>
#include <string>

struct CamInputArray {
	std::vector<CamInput*> cams;
	bool dirty, glInited, connected;
	CamInputArray() {
	}
#ifdef USE_XML
	inline CamInputArray(const pugi::xml_node& node) :dirty(true), glInited(false), connected(false) {
		std::string mainPath = node.attribute("path").value();
		//int i = 0;
		for (pugi::xml_node camNode = node.child("CamInput"); camNode; camNode = camNode.next_sibling("CamInput")) {
			CamInput* cam = CamInput::create(camNode,mainPath);
			cams.push_back(cam);
			//if (i > 4)
		//		break;
			//i++;
		}
	}
#else
	inline CamInputArray(PTSParam *params, int num_camera, int input_width, int input_height, int input_type, int stitching_mode) :dirty(true), glInited(false), connected(false) {
		for (int i = 0; i < num_camera; i++) {
			CamInput* cam = CamInput::create(params[i], input_width, input_height, 3, input_type, stitching_mode);
			cams.push_back(cam);
		}
	}
#endif
	inline void  clear() {
		for(size_t i=0; i<cams.size(); i++ ) if( cams[i] ) { cams[i]->clear(); delete cams[i]; }
		cams.clear();
		connected = false;
		dirty=false;
		glInited=false;
	}
	inline void connect(int i=-1) {
		if( i<0 ) for(size_t k=0; k<cams.size(); k++ ){ if(!cams[k]->connected) cams[k]->connect(); }
		else if( i<(int)cams.size() ) if(!cams[i]->connected) cams[i]->connect();
		connected = true;
	}
	inline void refresh(int* pbo_idx, int i=-1) {
		if (i<0) for (size_t k = 0; k<cams.size(); k++) cams[k]->refresh(pbo_idx[k]);
		else if (i<(int)cams.size()) cams[i]->refresh(pbo_idx[i]);
	}

	inline void refresh_stereo(int* pbo_idx, int stereo_idx) {
		int i = stereo_idx * 2;
		cams[i]->refresh(pbo_idx[i]);
		cams[i+1]->refresh(pbo_idx[i+1]);
	}

	inline void refreshStitchedBuffer(int* pbo_idx, GLuint texid, int i = -1) {
		if (i<0) for (size_t k = 0; k<cams.size(); k++) cams[k]->refresh(pbo_idx[k], texid);
		else if (i<(int)cams.size()) cams[i]->refresh(pbo_idx[i], texid);
	}
	inline void initGL(int i=-1) {
		if( i<0 ) for(size_t k=0; k<cams.size(); k++ ){ if(!cams[k]->glInited ) cams[k]->initGL(); }
		else if( i<(int)cams.size() ) if(!cams[i]->glInited) cams[i]->initGL();
		glInited=true;
	}
	inline void disconnect(int i=-1) {
		if( i<0 ) for(size_t k=0; k<cams.size(); k++ ) cams[k]->disconnect();
		else if( i<(int)cams.size() ) cams[i]->disconnect();
	}
	inline void clear(int i=-1) {
		if (i<0) for (size_t k = 0; k<cams.size(); k++) ((CamInputPBO*)cams[k])->destroy();
		else if (i<(int)cams.size()) ((CamInputPBO*)cams[i])->destroy();

		if( i<0 ) for(size_t k=0; k<cams.size(); k++ ) cams[k]->clear();
		else if( i<(int)cams.size() ) cams[i]->clear();
	}
	inline bool isGLInited() {
		if(!glInited) return false;
		bool ret = true; for(size_t i=0; i<cams.size(); i++ ) ret&=cams[i]->glInited;
		return ret;
	}
	inline bool isDirty() {
		if(dirty) return true; bool ret =false; for(size_t i=0; i<cams.size(); i++ ) ret|=cams[i]->dirty;
		return ret;
	}
	inline void setUndirty() {
		dirty=false;
		for(size_t i=0; i<cams.size(); i++ ) cams[i]->dirty=false;
	}
	inline void* getPBOAddress(int camera_idx, int buffer_idx)
	{
		if (cams[camera_idx])
		{
			return cams[camera_idx]->getPBOAddress(buffer_idx);
		}
		return 0;
	}
	inline GLuint tex(size_t k) { if( k>=0 && k<cams.size() ) return cams[k]->getImageTex(); return 0; }
	inline GLuint mask(size_t k) { if( k>=0 && k<cams.size() ) return cams[k]->getMaskTex(); return 0; }
	inline GLuint width(size_t k) { if( k>=0 && k<cams.size() ) return cams[k]->width; return 0; }
	inline GLuint height(size_t k) { if( k>=0 && k<cams.size() ) return cams[k]->height; return 0; }
	inline size_t count() { return cams.size(); }
	inline GLTexMask* texMask(size_t k) { if( k>=0 && k<cams.size() ) return &(cams[k]->getTexMask()); return 0; }
	inline CamInput* camIn(size_t k) { if( k>=0 && k<cams.size() ) return cams[k]; return 0; }
};




#endif
