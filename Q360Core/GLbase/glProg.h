//
//  Header.h
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/19/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#ifndef Header_h
#define Header_h

#include "glsl.h"

extern const char* vShader;

struct GLProg {
	GLuint prog;
	virtual std::string getVShader() const =0;
	virtual std::string getFShader() const =0;
	GLProg():prog(0){}
	void glInit() { prog = loadProgram(getVShader(),getFShader()); }
	void unuse() const {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);
	}
protected:
	void use() {
		if( prog == 0 ) glInit();
		glUseProgram(prog);
	}
};
struct SingleTexProg: GLProg {
	GLuint texPos;
	SingleTexProg(): GLProg(), texPos(0){}
	void use(GLuint tex);
};
struct TwoTexProg: GLProg {
	GLuint tex1Pos, tex2Pos;
	TwoTexProg(): GLProg(), tex1Pos(0), tex2Pos(0){}
	void use(GLuint tex1, GLuint tex2);
};
/*struct ThreeTexProg : GLProg {
	GLuint tex1Pos, tex2Pos, tex3Pos;
	ThreeTexProg() : GLProg(), tex1Pos(0), tex2Pos(0), tex3Pos(0){}
	void use(GLuint tex1, GLuint tex2, GLuint tex3);
};*/
struct FiveTexProg : GLProg {
	GLuint tex1Pos, tex2Pos, tex3Pos, tex4Pos, tex5Pos;
	FiveTexProg() : GLProg(), tex1Pos(0), tex2Pos(0), tex3Pos(0), tex4Pos(0), tex5Pos(0){}
	void use(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5);
};
struct SixTexProg : GLProg {
	GLuint tex1Pos, tex2Pos, tex3Pos, tex4Pos, tex5Pos, tex6Pos, tex7Pos;
	SixTexProg() : GLProg(), tex1Pos(0), tex2Pos(0), tex3Pos(0), tex4Pos(0), tex5Pos(0), tex6Pos(0){}
	void use(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6);
};
struct SevenTexProg : GLProg {
	GLuint tex1Pos, tex2Pos, tex3Pos, tex4Pos, tex5Pos, tex6Pos, tex7Pos;
	SevenTexProg() : GLProg(), tex1Pos(0), tex2Pos(0), tex3Pos(0), tex4Pos(0), tex5Pos(0), tex6Pos(0), tex7Pos(0){}
	void use(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6, GLuint tex7);
};
struct LaplacianProg: TwoTexProg {
	LaplacianProg(): TwoTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct LaplacianMergeProg : TwoTexProg {
	LaplacianMergeProg() : TwoTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};

struct FastLaplacianProg : GLProg{
	FastLaplacianProg() : GLProg(){}

	GLuint tex0Pos, tex1Pos, tex2Pos, tex3Pos, tex4Pos, tex5Pos, tex6Pos, tex7Pos;
	GLuint tex8Pos, tex9Pos, tex10Pos, tex11Pos, tex12Pos, tex13Pos, tex14Pos, tex15Pos;
	GLuint numInputPos;
	GLuint contrastPos;
	void use(int num_input, float contrast, GLuint tex0, GLuint tex1, GLuint tex2 = 0, GLuint tex3 = 0, GLuint tex4 = 0, GLuint tex5 = 0, GLuint tex6 = 0, GLuint tex7 = 0,
		GLuint tex8 = 0, GLuint tex9 = 0, GLuint tex10 = 0, GLuint tex11 = 0, GLuint tex12 = 0, GLuint tex13 = 0, GLuint tex14 = 0, GLuint tex15 = 0);

	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct FastAlphaedProg : GLProg {
	FastAlphaedProg() : GLProg(){}

	GLuint tex0Pos, tex1Pos, tex2Pos, tex3Pos;
	GLuint tex4Pos, tex5Pos, tex6Pos, tex7Pos;
	GLuint numInputPos;
	void use(int num_input, GLuint tex0, GLuint tex1 = 0, GLuint tex2 = 0, GLuint tex3 = 0, GLuint tex4 = 0, GLuint tex5 = 0, GLuint tex6 = 0, GLuint tex7 = 0);

	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};

struct NegateProg: SingleTexProg {
	NegateProg(): SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct LaplacianVisProg: SingleTexProg {
	LaplacianVisProg(): SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct MultiBandPaddingProg : SingleTexProg {
	MultiBandPaddingProg() : SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct NormalizeProg: SingleTexProg {
	NormalizeProg(): SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct SimpleProg : SingleTexProg {
	SimpleProg() : SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};

struct NormalizeA0Prog: SingleTexProg {
	NormalizeA0Prog(): SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct AlphaedProg: SingleTexProg {
	AlphaedProg(): SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct AlphaedMProg: TwoTexProg {
	AlphaedMProg(): TwoTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};
struct UndistortProg: SingleTexProg {
	GLuint aspectPos,aPos,bPos,cPos,dPos;
	UndistortProg(): SingleTexProg(), aPos(0) {}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
	void use(GLuint tex, float aspect, float a, float b, float c,float d);
	void use(GLuint tex, float aspect, const float f[]);
};
/*struct WarpProg: TwoTexProg {
	GLuint aspectPos,aPos,bPos,cPos,dPos;
	GLuint mPos, focalPos;
	WarpProg(): TwoTexProg(){}
	std::string getVShader() const { return vShader; }
	virtual std::string getFShader() const;
	virtual void use(GLuint tex, GLuint mask, const float m[], float focal, float aspec, const float dist[]);
};*/
/*struct WarpProg : ThreeTexProg {
	GLuint aspectPos, aPos, bPos, cPos, dPos;
	GLuint mPos, focalPos;
	WarpProg() : ThreeTexProg(){}
	std::string getVShader() const { return vShader; }
	virtual std::string getFShader() const;
	virtual void use(GLuint tex, GLuint mask, const float m[], float focal, float aspec, const float dist[]);
};*/
/*struct WarpProg : SixTexProg {
	GLuint aspectPos, aPos, bPos, cPos, dPos;
	GLuint mPos, focalPos;
	WarpProg() : SixTexProg(){}
	std::string getVShader() const { return vShader; }
	virtual std::string getFShader() const;
	virtual void use(GLuint tex, GLuint mask, const float m[], float focal, float aspec, const float dist[]);
};*/
struct WarpProg : SevenTexProg {
	GLuint aspectPos, aPos, bPos, cPos, dPos;
	GLuint mPos, focalPos;
	WarpProg() : SevenTexProg(){}
	std::string getVShader() const { return vShader; }
	virtual std::string getFShader() const;
	virtual void use(GLuint tex, GLuint mask, const float m[], float focal, float aspec, const float dist[]);
};
struct WarpSingleProg : SingleTexProg {
	GLuint aspectPos, aPos, bPos, cPos, dPos;
	GLuint mPos, focalPos;
	WarpSingleProg() : SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	virtual std::string getFShader() const;
	virtual void use(GLuint tex, const float m[], float focal, float aspec, const float dist[]);
};

struct WarpUnalphaedProg: WarpProg {
	GLuint aspectPos,aPos,bPos,cPos,dPos;
	GLuint mPos, focalPos;
	WarpUnalphaedProg(): WarpProg(){}
	virtual std::string getFShader() const;
};

struct WarpUnalphaedWarapedMaskProg: WarpProg {
	GLuint aspectPos,aPos,bPos,cPos,dPos;
	GLuint mPos, focalPos;
	WarpUnalphaedWarapedMaskProg(): WarpProg(){}
	virtual std::string getFShader() const;
};

struct FullframeFisheyeStereoProg : WarpProg {
	GLuint isMapSet;
	GLuint isMapAnimation;
	GLuint isEdited;
	GLuint mapWidth;
	GLuint mapHeight;
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint laplacian_modePos;
	GLuint r_gainPos, g_gainPos, b_gainPos;		// modified, r,g,b
	GLuint gain_modePos;
	GLuint gray_modePos;
	GLuint brightness_stepPos;
	GLuint animation_timePos;
	GLuint crop_ratio_wPos, crop_ratio_hPos, crop_sxPos, crop_syPos;
	GLuint cam_indexPos;
	FullframeFisheyeStereoProg() : WarpProg(){}
	virtual std::string getFShader() const;

	void setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode, float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int gray_mode, float brightness_step, float animation_time, int cam_index);
		
};

struct FullframeFisheyeStereoWithMapProg : WarpProg {
	GLuint isMapSet;
	GLuint isMapAnimation;
	GLuint isEdited;
	GLuint mapWidth;
	GLuint mapHeight;
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint laplacian_modePos;
	GLuint r_gainPos, g_gainPos, b_gainPos;		// modified, r,g,b
	GLuint gain_modePos;
	GLuint gray_modePos;
	GLuint brightness_stepPos;
	GLuint animation_timePos;
	GLuint crop_ratio_wPos, crop_ratio_hPos, crop_sxPos, crop_syPos;
	GLuint cam_indexPos;
	FullframeFisheyeStereoWithMapProg() : WarpProg(){}
	virtual std::string getFShader() const;

	void setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode, float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int cam_index);

};

struct FullframeFisheyeWarpProg : WarpSingleProg {
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint opacity_Pos;					// Transparency effect
	GLuint cam_indexPos;
	FullframeFisheyeWarpProg() : WarpSingleProg() {}
	virtual std::string getFShader() const;

	void setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1,
		float distance, float aspect_dw, float aspect_sw, float lens_dist[], float opacity_mode, int camera_idx);
};

struct RectlinearPanoramaWarpProg : WarpSingleProg {
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint opacity_Pos;					// Transparency effect
	GLuint cam_indexPos;
	RectlinearPanoramaWarpProg() : WarpSingleProg() {}
	virtual std::string getFShader() const;

	void setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1,
		float distance, float aspect_dw, float aspect_sw, float lens_dist[], int opacity_mode, int camera_idx);
};

struct RectlinearWarpProg : WarpSingleProg {
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint opacity_Pos;					// Transparency effect
	GLuint cam_indexPos;
	GLuint yawPos, pitchPos, rollPos;
	RectlinearWarpProg() : WarpSingleProg() {}
	virtual std::string getFShader() const;

	void setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1,
		float distance, float aspect_dw, float aspect_sw, float lens_dist[], int opacity_mode, int camera_idx, float yaw, float pitch, float roll);
};

struct FullframeFisheyeCylindricalWarpProg : WarpSingleProg {
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint opacity_Pos;					// Transparency effect
	FullframeFisheyeCylindricalWarpProg() : WarpSingleProg() {}
	virtual std::string getFShader() const;

	void setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1,
		float distance, float aspect_dw, float aspect_sw, float lens_dist[], int opacity_mode);
};

struct RectlinearWarpUnalphaedPanoramaWarapedMaskProg : WarpProg {
	GLuint isMapSet;
	GLuint isMapAnimation;
	GLuint isEdited;
	GLuint mapWidth;
	GLuint mapHeight;
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint laplacian_modePos;
	GLuint r_gainPos, g_gainPos, b_gainPos;		// modified, r,g,b
	GLuint gain_modePos;
	GLuint gray_modePos;
	GLuint brightness_stepPos;
	GLuint animation_timePos;
	GLuint crop_ratio_wPos, crop_ratio_hPos, crop_sxPos, crop_syPos;
	GLuint lensType;
	RectlinearWarpUnalphaedPanoramaWarapedMaskProg() : WarpProg() {}
	virtual std::string getFShader() const;

	void setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode, float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int resetMapAnimation, int lens_type);
	void setLUT(GLuint tex);
};

//struct FullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg : WarpProg {
//	GLuint isMapSet;
//	GLuint isMapAnimation;
//	GLuint isEdited;
//	GLuint mapWidth;
//	GLuint mapHeight;
//	GLuint aPos, bPos, cPos, dPos;		// lens coeff
//	GLuint mPos;						// matrix rotate(pitch, roll)
//	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
//	GLuint scale0Pos, scale1Pos;		// ratio
//	GLuint distPos;						// horizontal pixels per degree
//	GLuint aspect_dwPos, aspect_swPos;
//	GLuint laplacian_modePos;
//	GLuint r_gainPos, g_gainPos, b_gainPos;		// modified, r,g,b
//	GLuint gain_modePos;
//	GLuint gray_modePos;
//	GLuint brightness_stepPos;
//	GLuint animation_timePos;
//	GLuint crop_ratio_wPos, crop_ratio_hPos, crop_sxPos, crop_syPos;
//	FullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg() : WarpProg() {}
//	virtual std::string getFShader() const;
//
//	void setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode, float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int resetMapAnimation);
//	void setLUT(GLuint tex);
//};

struct FullframeFisheyeWarpUnalphaedWarapedMaskProg : WarpProg {
	GLuint isMapSet;
	GLuint isMapAnimation;
	GLuint isEdited;
	GLuint mapWidth;
	GLuint mapHeight;
	GLuint aPos, bPos, cPos, dPos;		// lens coeff
	GLuint mPos;						// matrix rotate(pitch, roll)
	GLuint rot0Pos, rot1Pos;			// rotate(yaw)
	GLuint scale0Pos, scale1Pos;		// ratio
	GLuint distPos;						// horizontal pixels per degree
	GLuint aspect_dwPos, aspect_swPos;
	GLuint laplacian_modePos;
	GLuint r_gainPos, g_gainPos, b_gainPos;		// modified, r,g,b
	GLuint gain_modePos;
	GLuint gray_modePos;
	GLuint brightness_stepPos;
	GLuint animation_timePos;
	GLuint crop_ratio_wPos, crop_ratio_hPos, crop_sxPos, crop_syPos;
	FullframeFisheyeWarpUnalphaedWarapedMaskProg() : WarpProg() {}
	virtual std::string getFShader() const;

	void setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode, float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int resetMapAnimation);
	void setLUT(GLuint tex);
};

struct Alpha2ColorProg: SingleTexProg {
	Alpha2ColorProg(): SingleTexProg(){}
	std::string getVShader() const { return vShader; }
	std::string getFShader() const;
};


extern	LaplacianMergeProg	laplacianMergeProg;
extern  FastLaplacianProg	fastlaplacianProg;
extern	FastAlphaedProg		fastalphaedProg;
extern  LaplacianProg		laplacianProg;
extern  NegateProg			negateProg;
extern  LaplacianVisProg	laplacianVisProg;
extern  NormalizeProg		normalizeProg;
extern  NormalizeA0Prog		normalizeA0Prog;
extern  AlphaedProg			alphaedProg;
extern  AlphaedMProg		alphaedMProg;
extern  UndistortProg		undistortProg;
extern	WarpProg			warpProg;
extern	WarpUnalphaedProg	warpUnalphaedProg;
extern WarpUnalphaedWarapedMaskProg warpUnalphaedWarpedMaskProg;
extern	Alpha2ColorProg		alpha2ColorProg;
extern FullframeFisheyeWarpUnalphaedWarapedMaskProg fullframeFisheyeWarpUnalphaedWarapedMaskProg;
extern FullframeFisheyeWarpProg fullframeFisheyeWarpProg;
extern FullframeFisheyeStereoProg fullframeFisheyeStereoProg;
extern FullframeFisheyeStereoWithMapProg fullframeFisheyeStereoWithMapProg;
extern FullframeFisheyeCylindricalWarpProg fullframeFisheyeCylindricalWarpProg;
//extern FullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg fullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg;
extern RectlinearWarpUnalphaedPanoramaWarapedMaskProg rectlinearWarpUnalphaedPanoramaWarapedMaskProg;
extern RectlinearPanoramaWarpProg rectlinearPanoramaWarpProg;
extern RectlinearWarpProg rectlinearWarpProg;
extern  SimpleProg		simpleProg;

extern  MultiBandPaddingProg		multiBandPaddingProg;

#endif /* Header_h */
