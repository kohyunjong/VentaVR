//
//  GLProg.cpp
//  LaplacianBlending
//
//  Created by Hyun Joon Shin on 5/19/16.
//  Copyright © 2016 Hyun Joon Shin. All rights reserved.
//

#include <stdio.h>
#include "GLProg.h"

LaplacianMergeProg	laplacianMergeProg;
LaplacianProg		laplacianProg;
FastLaplacianProg	fastlaplacianProg;
FastAlphaedProg		fastalphaedProg;
NegateProg			negateProg;
LaplacianVisProg	laplacianVisProg;
NormalizeProg		normalizeProg;
NormalizeA0Prog		normalizeA0Prog;
AlphaedProg			alphaedProg;
AlphaedMProg		alphaedMProg;
UndistortProg		undistortProg;
WarpProg			warpProg;
Alpha2ColorProg		alpha2ColorProg;
WarpUnalphaedProg	warpUnalphaedProg;
WarpUnalphaedWarapedMaskProg warpUnalphaedWarpedMaskProg;
FullframeFisheyeWarpUnalphaedWarapedMaskProg fullframeFisheyeWarpUnalphaedWarapedMaskProg;
FullframeFisheyeWarpProg fullframeFisheyeWarpProg;
FullframeFisheyeStereoProg fullframeFisheyeStereoProg;
FullframeFisheyeCylindricalWarpProg fullframeFisheyeCylindricalWarpProg;
//FullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg fullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg;
RectlinearWarpUnalphaedPanoramaWarapedMaskProg rectlinearWarpUnalphaedPanoramaWarapedMaskProg;
RectlinearPanoramaWarpProg rectlinearPanoramaWarpProg;
RectlinearWarpProg rectlinearWarpProg;

SimpleProg		simpleProg;

MultiBandPaddingProg		multiBandPaddingProg;

const char* vShader =
"void main() {\n"
"	gl_Position = gl_ModelViewMatrix*gl_Vertex;\n"
"	gl_TexCoord[0] = gl_MultiTexCoord0;\n"
"}\n\n";

void SingleTexProg::use(GLuint tex) {
	GLProg::use();
	if( texPos == 0 )
		texPos = glGetUniformLocation(prog, "tex");
	
	glUniform1i(texPos, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
}
void TwoTexProg::use(GLuint tex1, GLuint tex2) {
	GLProg::use();
	if( tex1Pos == 0 ) {
		tex1Pos = glGetUniformLocation(prog, "tex1");
		tex2Pos = glGetUniformLocation(prog, "tex2");
	}
	glUniform1i(tex1Pos, 0);
	glUniform1i(tex2Pos, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
}

void FiveTexProg::use(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5) {
	GLProg::use();
	if (tex1Pos == 0) {
		tex1Pos = glGetUniformLocation(prog, "tex1");
		tex2Pos = glGetUniformLocation(prog, "tex2");
		tex3Pos = glGetUniformLocation(prog, "tex3");
		tex4Pos = glGetUniformLocation(prog, "tex4");
		tex5Pos = glGetUniformLocation(prog, "tex5");
	}
	glUniform1i(tex1Pos, 0);
	glUniform1i(tex2Pos, 1);
	glUniform1i(tex3Pos, 2);
	glUniform1i(tex4Pos, 3);
	glUniform1i(tex5Pos, 4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_1D, tex5);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, tex4);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, tex3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
}

void SixTexProg::use(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6) {
	GLProg::use();
	if (tex1Pos == 0) {
		tex1Pos = glGetUniformLocation(prog, "tex1");
		tex2Pos = glGetUniformLocation(prog, "tex2");
		tex3Pos = glGetUniformLocation(prog, "tex3");
		tex4Pos = glGetUniformLocation(prog, "tex4");
		tex5Pos = glGetUniformLocation(prog, "tex5");
		tex6Pos = glGetUniformLocation(prog, "tex6");
	}
	glUniform1i(tex1Pos, 0);
	glUniform1i(tex2Pos, 1);
	glUniform1i(tex3Pos, 2);
	glUniform1i(tex4Pos, 3);
	glUniform1i(tex5Pos, 4);
	glUniform1i(tex6Pos, 5);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, tex6);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_1D, tex5);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, tex4);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, tex3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
}

void SevenTexProg::use(GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6, GLuint tex7) {
	GLProg::use();
	if (tex1Pos == 0) {
		tex1Pos = glGetUniformLocation(prog, "tex1");
		tex2Pos = glGetUniformLocation(prog, "tex2");
		tex3Pos = glGetUniformLocation(prog, "tex3");
		tex4Pos = glGetUniformLocation(prog, "tex4");
		tex5Pos = glGetUniformLocation(prog, "tex5");
		tex6Pos = glGetUniformLocation(prog, "tex6");
		tex7Pos = glGetUniformLocation(prog, "tex7");
	}
	glUniform1i(tex1Pos, 0);
	glUniform1i(tex2Pos, 1);
	glUniform1i(tex3Pos, 2);
	glUniform1i(tex4Pos, 3);
	glUniform1i(tex5Pos, 4);
	glUniform1i(tex6Pos, 5);
	glUniform1i(tex7Pos, 6);
	glActiveTexture(GL_TEXTURE6);
	glBindTexture(GL_TEXTURE_2D, tex7);
	glActiveTexture(GL_TEXTURE5);
	glBindTexture(GL_TEXTURE_2D, tex6);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_1D, tex5);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_1D, tex4);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_1D, tex3);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, tex2);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex1);
}


void FastAlphaedProg::use(int num_input, GLuint tex0, GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6, GLuint tex7)
{
	GLProg::use();
	if (tex0Pos == 0) {
		tex0Pos = glGetUniformLocation(prog, "tex0");
		tex1Pos = glGetUniformLocation(prog, "tex1");
		tex2Pos = glGetUniformLocation(prog, "tex2");
		tex3Pos = glGetUniformLocation(prog, "tex3");
		tex4Pos = glGetUniformLocation(prog, "tex4");
		tex5Pos = glGetUniformLocation(prog, "tex5");
		tex6Pos = glGetUniformLocation(prog, "tex6");
		tex7Pos = glGetUniformLocation(prog, "tex7");

		numInputPos = glGetUniformLocation(prog, "num_input");
	}
	if (num_input > 0)
	{
		glUniform1i(tex0Pos, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
	}

	if (num_input > 1)
	{
		glUniform1i(tex1Pos, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
	}

	if (num_input > 2)
	{
		glUniform1i(tex2Pos, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex2);
	}
	if (num_input > 3)
	{
		glUniform1i(tex3Pos, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex3);
	}

	if (num_input > 4)
	{
		glUniform1i(tex4Pos, 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tex4);
	}

	if (num_input > 5)
	{
		glUniform1i(tex5Pos, 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, tex5);
	}

	if (num_input > 6)
	{
		glUniform1i(tex6Pos, 6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, tex6);
	}

	if (num_input > 7)
	{
		glUniform1i(tex7Pos, 7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, tex7);
	}

	glUniform1i(numInputPos, num_input);
}

void FastLaplacianProg::use(int num_input, float contrast, GLuint tex0, GLuint tex1, GLuint tex2, GLuint tex3, GLuint tex4, GLuint tex5, GLuint tex6, GLuint tex7,
	GLuint tex8, GLuint tex9, GLuint tex10, GLuint tex11, GLuint tex12, GLuint tex13, GLuint tex14, GLuint tex15)
{
	GLProg::use();
	if (tex0Pos == 0) {
		tex0Pos = glGetUniformLocation(prog, "tex0");
		tex1Pos = glGetUniformLocation(prog, "tex1");
		tex2Pos = glGetUniformLocation(prog, "tex2");
		tex3Pos = glGetUniformLocation(prog, "tex3");
		tex4Pos = glGetUniformLocation(prog, "tex4");
		tex5Pos = glGetUniformLocation(prog, "tex5");
		tex6Pos = glGetUniformLocation(prog, "tex6");
		tex7Pos = glGetUniformLocation(prog, "tex7");

		tex8Pos = glGetUniformLocation(prog, "tex8");
		tex9Pos = glGetUniformLocation(prog, "tex9");
		tex10Pos = glGetUniformLocation(prog, "tex10");
		tex11Pos = glGetUniformLocation(prog, "tex11");
		tex12Pos = glGetUniformLocation(prog, "tex12");
		tex13Pos = glGetUniformLocation(prog, "tex13");
		tex14Pos = glGetUniformLocation(prog, "tex14");
		tex15Pos = glGetUniformLocation(prog, "tex15");

		numInputPos = glGetUniformLocation(prog, "num_input");
		contrastPos = glGetUniformLocation(prog, "contrast");
	}
	if (num_input > 0)
	{
		glUniform1i(tex0Pos, 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex0);
		glUniform1i(tex1Pos, 1);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex1);
	}

	if (num_input > 1)
	{
		glUniform1i(tex2Pos, 2);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, tex2);
		glUniform1i(tex3Pos, 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, tex3);
	}

	if (num_input > 2)
	{
		glUniform1i(tex4Pos, 4);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D, tex4);
		glUniform1i(tex5Pos, 5);
		glActiveTexture(GL_TEXTURE5);
		glBindTexture(GL_TEXTURE_2D, tex5);
	}
	if (num_input > 3)
	{
		glUniform1i(tex6Pos, 6);
		glActiveTexture(GL_TEXTURE6);
		glBindTexture(GL_TEXTURE_2D, tex6);
		glUniform1i(tex7Pos, 7);
		glActiveTexture(GL_TEXTURE7);
		glBindTexture(GL_TEXTURE_2D, tex7);
	}

	if (num_input > 4)
	{
		glUniform1i(tex8Pos, 8);
		glActiveTexture(GL_TEXTURE8);
		glBindTexture(GL_TEXTURE_2D, tex8);
		glUniform1i(tex9Pos, 9);
		glActiveTexture(GL_TEXTURE9);
		glBindTexture(GL_TEXTURE_2D, tex9);
	}

	if (num_input > 5)
	{
		glUniform1i(tex10Pos, 10);
		glActiveTexture(GL_TEXTURE10);
		glBindTexture(GL_TEXTURE_2D, tex10);
		glUniform1i(tex11Pos, 11);
		glActiveTexture(GL_TEXTURE11);
		glBindTexture(GL_TEXTURE_2D, tex11);
	}

	if (num_input > 6)
	{
		glUniform1i(tex12Pos, 12);
		glActiveTexture(GL_TEXTURE12);
		glBindTexture(GL_TEXTURE_2D, tex12);
		glUniform1i(tex13Pos, 13);
		glActiveTexture(GL_TEXTURE13);
		glBindTexture(GL_TEXTURE_2D, tex13);
	}

	if (num_input > 7)
	{
		glUniform1i(tex14Pos, 14);
		glActiveTexture(GL_TEXTURE14);
		glBindTexture(GL_TEXTURE_2D, tex14);
		glUniform1i(tex15Pos, 15);
		glActiveTexture(GL_TEXTURE15);
		glBindTexture(GL_TEXTURE_2D, tex15);
	}

	glUniform1i(numInputPos, num_input);
	glUniform1f(contrastPos, contrast);
}

std::string LaplacianProg::getFShader() const { return
	"uniform sampler2D tex1, tex2;\n"
	"void main() {\n"
	"	vec4 c =texture2D(tex1, gl_TexCoord[0].st);\n"
	"	gl_FragColor=vec4((c-texture2D(tex2, gl_TexCoord[0].st)).rgb*c.a,c.a);\n"
	"}\n";
}

std::string LaplacianMergeProg::getFShader() const {
	return
		"uniform sampler2D tex1, tex2;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex1, gl_TexCoord[0].st);\n"
		"	vec4 c1 =texture2D(tex2, gl_TexCoord[0].st);\n"
		//"	c1.r = c1.r - 0.5;\n"
		//"	c1.g = c1.g - 0.5;\n"
		//"	c1.b = c1.b - 0.5;\n"

		"	gl_FragColor=vec4((c0+c1).rgb,c0.a);\n"
		"}\n";
}

std::string FastLaplacianProg::getFShader() const {
	return
		"uniform sampler2D tex0, tex1, tex2, tex3, tex4, tex5, tex6, tex7;\n"
		"uniform sampler2D tex8, tex9, tex10, tex11, tex12, tex13, tex14, tex15;\n"
		"uniform float contrast;"
		"uniform int num_input;"
		"void main() {\n"
		"	vec4 color;\n"
		"	vec4 ret;\n"
		"	vec4 c0 =texture2D(tex0, gl_TexCoord[0].st);\n"
		"	vec4 c1 =texture2D(tex1, gl_TexCoord[0].st);\n"
		"	color = vec4(c0-c1);\n"
		//"	color.r = color.r + 0.5;\n"
		//"	color.g = color.g + 0.5;\n"
		//"	color.b = color.b + 0.5;\n"
		"	ret = vec4(color.rgb*c0.a*contrast, c0.a);\n"

		"	if(num_input>1) {\n"
		"		c0 =texture2D(tex2, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex3, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"

		"	if(num_input>2) {\n"
		"		c0 =texture2D(tex4, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex5, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"

		"	if(num_input>3) {\n"
		"		c0 =texture2D(tex6, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex7, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"

		"	if(num_input>4) {\n"
		"		c0 =texture2D(tex8, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex9, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"

		"	if(num_input>5) {\n"
		"		c0 =texture2D(tex10, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex11, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"

		"	if(num_input>6) {\n"
		"		c0 =texture2D(tex12, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex13, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"

		"	if(num_input>7) {\n"
		"		c0 =texture2D(tex14, gl_TexCoord[0].st);\n"
		"		c1 =texture2D(tex15, gl_TexCoord[0].st);\n"
		"		color = vec4(c0-c1);\n"
		//"		color.r = color.r + 0.5;\n"
		//"		color.g = color.g + 0.5;\n"
		//"		color.b = color.b + 0.5;\n"
		"		ret = ret + vec4(color.rgb*c0.a*contrast, c0.a);\n"
		"	}\n"
		
		"	gl_FragColor = ret / ret.a;\n"
		"}\n";
}

std::string NegateProg::getFShader() const { return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex, gl_TexCoord[0].st);\n"
		"	gl_FragColor=vec4(-(c0.rgb),c0.a);\n"
		"}\n";
	}

std::string LaplacianVisProg::getFShader() const { return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex, gl_TexCoord[0].st);\n"
		"	float fac = 1./c0.a;\n"
		"	gl_FragColor = vec4(c0.rgb*fac*4.+vec3(.5,.5,.5),c0.a);\n"
		"}\n";
	}

std::string NormalizeProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex, gl_TexCoord[0].st);\n"
		"	if( c0.a<=0.001 ) gl_FragColor=vec4(0,0,0,0);\n"
		"	else		gl_FragColor = c0/c0.a;\n"
		"}\n";
}

std::string SimpleProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex, gl_TexCoord[0].st);\n"
		//"	c0.a = 1.0;\n"
		"	gl_FragColor = c0;\n"
		//"	if( c0.a<=0.001 ) gl_FragColor=vec4(0,0,0,0);\n"

		//"	else		gl_FragColor = c0/c0.a;\n"
		"}\n";
}


std::string MultiBandPaddingProg::getFShader() const { return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"
		// MultiBand blending padding
		"	float fac = 1.0/1.25;\n"
		"	float mar = (1.0-fac)/2.0;\n"

		"   v.x = (v.x*fac)+mar;\n"
		// MultiBand blending padding

		"	vec4 c0 =texture2D(tex, v);\n"

		/*"	float a = 0.0; \n"
		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"		a =1.0;\n"
		"	gl_FragColor = vec4(c0.rgb, a); \n"*/
		"	if( c0.a == 0 ) gl_FragColor=vec4(0,0,0,0);\n"
		"	else		gl_FragColor = c0;\n"
		//"	gl_FragColor = c0;\n"
		"}\n";
	}

std::string NormalizeA0Prog::getFShader() const { return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex, gl_TexCoord[0].st);\n"
		"	gl_FragColor = vec4(c0.rgb,0);\n"
		"}\n";
	}

std::string AlphaedProg::getFShader() const { return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	vec4 c0 =texture2D(tex, gl_TexCoord[0].st);\n"
		"	gl_FragColor = vec4(c0.rgb*c0.a,c0.a);\n"
		"}\n";
	}

std::string FastAlphaedProg::getFShader() const {
	return
		"uniform sampler2D tex0, tex1, tex2, tex3;\n"
		"uniform sampler2D tex4, tex5, tex6, tex7;\n"
		"uniform int num_input;\n"
		"void main() {\n"
		"	vec4 color;\n"
		"	vec4 c0 =texture2D(tex0, gl_TexCoord[0].st);\n"
		"	color = vec4(c0.rgb*c0.a,c0.a);\n"

		"	if(num_input > 1) {\n"
		"		c0 =texture2D(tex1, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	if(num_input > 2) {\n"
		"		c0 =texture2D(tex2, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	if(num_input > 3) {\n"
		"		c0 =texture2D(tex3, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	if(num_input > 4) {\n"
		"		c0 =texture2D(tex4, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	if(num_input > 5) {\n"
		"		c0 =texture2D(tex5, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	if(num_input > 6) {\n"
		"		c0 =texture2D(tex6, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	if(num_input > 7) {\n"
		"		c0 =texture2D(tex7, gl_TexCoord[0].st);\n"
		"		color = color + vec4(c0.rgb*c0.a,c0.a);\n"
		"	}\n"

		"	gl_FragColor = color/color.a;\n"
		"}\n";
}

std::string Alpha2ColorProg::getFShader() const { return
		"uniform sampler2D tex;\n"
		"void main() {\n"
		"	float a = texture2D(tex, gl_TexCoord[0].st).a;\n"
		"	gl_FragColor = vec4(a/2.,a/2.,a/2.,1);\n"
		"}\n";
	}

std::string AlphaedMProg::getFShader() const { return
	"uniform sampler2D tex1, tex2;\n"
	"void main() {\n"
	"	float a =texture2D(tex2, gl_TexCoord[0].st).a;\n"
	"	gl_FragColor = vec4((texture2D(tex1, gl_TexCoord[0].st)*a).rgb,a);\n"
	"}\n";
}

void UndistortProg::use(GLuint tex,float aspect,float a,float b,float c,float d) {
	SingleTexProg::use(tex);
	if(aPos==0) {
		aspectPos = glGetUniformLocation(prog, "aspect");
		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");
	}
	glUniform1f(aspectPos, aspect);
	glUniform1f(aPos, a);
	glUniform1f(bPos, b);
	glUniform1f(cPos, c);
	glUniform1f(dPos, d);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex);
}

#define ERECT_PANO \
	"vec2 erect_pano(vec2 p, float dist){\n" \
	"	vec2 sp = vec2(p.x, p.y);\n" \
	"	sp.y = dist * atan(p.y / dist);\n" \
	"	return sp;\n" \
	"}\n"

#define ERECT_RECT \
	"vec2 erect_rect(vec2 p, float dist){\n" \
	"	p.x = dist * atan(p.x , dist);\n"\
	"	p.y = dist * atan(p.y / sqrt(dist * dist + p.x * p.x));\n"\
	"	return vec2(p.x, p.y);\n" \
	"}\n"


#define ROTATE_EQUIRECT \
	"vec2 rotate_eqrect(vec2 p, float rot0, float rot1){\n" \
	"	vec2 sp = vec2(p.x, p.y);\n" \
	"	sp.x = sp.x + rot1;\n" \
	"	return sp;\n" \
	"}\n"

	/*
	"	while (sp.x < -rot0)\n" \
	"	{\n" \
	"		sp.x = sp.x + (2.0*rot0);\n" \
	"	}\n" \
	"	while (sp.x > rot0)\n" \
	"	{\n" \
	"		sp.x = sp.x - (2.0*rot0);\n" \
	"	}\n" \
	*/

#define PERSPECTIVE_CONTROL_SPHERICAL_IMAGE \
	"vec2 persp_sphere(vec2 p, mat3 m, float dist) {\n"\
	"	float theta, s, r;\n"\
	"	r = sqrt(p.x * p.x + p.y * p.y);\n"\
	"	theta = r / dist;\n"\
	"	if (r == 0.0){\n"\
	"		s = 0.0;\n"\
	"	}\n"\
	"	else {\n"\
	"		s = sin(theta) / r;\n"\
	"	}\n"\
	"	vec3 v = m*vec3(s * p.x, s * p.y, cos(theta));\n" \
	"	r = sqrt(v.x * v.x + v.y * v.y);\n"\
	"	if (r == 0.0)\n"\
	"		theta = 0.0;\n"\
	"	else\n"\
	"		theta = dist * atan(r, v.z) / r;\n"\
	"	return vec2(theta*v.x, theta*v.y);\n"\
	"}\n"

#define CORRECT_LENSE_DIST_BIGHEADK \
	"vec2 radial(vec2 p, float mid)	{\n"\
	"	float r, scale;		\n"\
	"	r = (sqrt(p.x*p.x + p.y*p.y)) / mid;		\n"\
	"	scale = ((a * r + b) * r + c) * r + d;		\n"\
	"	return vec2(p.x*scale, p.y*scale);\n"\
	"}\n"

#define SPHERICAL_IMAGE_TO_EQUIRECT \
	"vec2 sphere_tp_erect(vec2 p, float dist) {\n" \
	"	float phi, theta, r, s;\n" \
	"	vec3 v;\n" \
	"	const float PI=3.14159265358979;\n" \
	"	phi = p.x / dist;\n" \
	"	theta = -p.y / dist + PI / 2.0;\n" \
	"	if (theta < 0.0)\n" \
	"	{\n" \
	"		theta = -theta;\n" \
	"		phi += PI;\n" \
	"	}\n" \
	"if (theta > PI)\n" \
	"	{\n" \
	"		theta = PI - (theta - PI);\n" \
	"		phi += PI;\n" \
	"	}\n" \
	"	s = sin(theta);\n" \
	"	v.x = s * sin(phi);\n" \
	"	v.y = cos(theta);\n" \
	"	r = sqrt(v.x * v.x + v.y * v.y);\n" \
	"	theta = dist * atan(r, s * cos(phi));\n" \
	"	return vec2(theta * v.x / r, theta * v.y / r);\n" \
	"}\n"

#define PERSPECTIVE_RECT_SPHERICAL \
	"vec2 rect_sphere(vec2 p, float dist) {\n"\
	"	float theta, rho, r;\n"\
	"	r = sqrt(p.x * p.x + p.y * p.y);\n"\
	"	theta = r / dist;\n"\
	"	if(theta == 0.0){\n"\
	"		rho = 1.0;\n"\
	"	}\n"\
	"	else{\n"\
	"		rho = tan(theta) / theta;\n"\
	"	}\n"\
	"	return vec2(p.x*rho, p.y*rho);\n"\
	"}\n"

#define ERECT_CYLINDRICAL \
	"float erect_millercylindrical(vec2 p, float dist) {\n"\
	"	float y;\n"\
	"	const float PI=3.14159265358979;\n" \
	"	y = p.y / dist;\n"\
	"	y = 1.25 * atan(sinh(4 * y / 5.0));\n"\
	"	if(abs(y) > PI*0.5){\n"\
	"		p.x = 0.0;\n"\
	"		p.y = 0.0;\n"\
	"		return 0.00;\n"\
	"	}\n"\
	"	p.y = dist * y;\n"\
	"	return 1.0;\n"\
	"}\n"

#define CYLINDRICAL_ERECT \
	"float millercylindrical_erect(vec2 p, float dist) {\n"\
	"	float phi, tanPhi;\n"\
	"	const float PI=3.14159265358979;\n" \
	"	phi = p.y / dist;\n"\
	"	tanPhi = tan(PI / 4 + 0.4 * phi);\n"\
	"	if(tanPhi < 0.0){\n"\
	"		p.x = 0.0;\n"\
	"		p.y = 0.0;\n"\
	"		return 0.0;\n"\
	"	}\n"\
	"	p.y = dist * log(tanPhi) / 0.8;\n"\
	"	return 1.0;\n"\
	"}\n"

#define RECT_WARP_SPHERE \
	"vec2 rect_warp_sphere(vec2 p, float dist) {\n"\
	"	float theta, rho;\n"\
	"	const float PI=3.14159265358979;\n" \
	"	theta = sqrt(p.x * p.x + p.y * p.y) / dist;\n" \
	"	if(theta >= PI / 2.0){\n"\
	"		rho = 1.6e16;\n"\
	"	}\n"\
	"   else if (theta == 0.0) {\n" \
	"		rho = 1.0;\n"\
	"   }\n"\
	"   else {\n" \
	"		rho = tan(theta) / theta;\n"\
	"   }\n"\
	"	p.x = rho * p.x;\n"\
	"	p.y = rho * p.y;\n"\
	"   return vec2(p.x, p.y);\n"\
	"}\n"

#define EQUIRECT_TO_SPHERICAL \
	"vec2 equirect_spherical(vec2 p, float dist) {\n"\
	"	float theta, radial, scale;\n"\
	"	float s, r;\n"\
	"	radial = sqrt(p.x * p.x + p.y * p.y);\n" \
	"	theta = radial / dist;\n" \
	"	if(theta == 0.0){\n"\
	"		scale = 1.0 / dist;\n"\
	"	}\n"\
	"   else {\n" \
	"		scale = sin(theta) / radial;\n"\
	"   }\n"\
	"	s = scale * p.x;\n"\
	"	r = cos(theta);\n"\
	"	p.x = dist * atan(s , r);\n"\
	"	p.y = dist * atan(scale * p.y / sqrt(r * r + s * s));\n"\
	"   return vec2(p.x, p.y);\n"\
	"}\n"

//#define RECT_TRANSLATE \

void UndistortProg::use(GLuint tex,float aspect,const float f[]) {
	use(tex,aspect,f[0],f[1],f[2],f[3]);
}

void FullframeFisheyeWarpProg::setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], float opacity_mode, int cam_idx)
{
	SingleTexProg::use(tex);

	if (aPos == 0) {
		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");

		mPos = glGetUniformLocation(prog, "m");

		rot0Pos = glGetUniformLocation(prog, "rot0");
		rot1Pos = glGetUniformLocation(prog, "rot1");

		scale0Pos = glGetUniformLocation(prog, "scale0");
		scale1Pos = glGetUniformLocation(prog, "scale1");

		distPos = glGetUniformLocation(prog, "dist");

		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");

		opacity_Pos = glGetUniformLocation(prog, "opacity_mode");
		cam_indexPos = glGetUniformLocation(prog, "cam_index");
	}

	glUniform1f(aPos, lens_dist[0]);
	glUniform1f(bPos, lens_dist[1]);
	glUniform1f(cPos, lens_dist[2]);
	glUniform1f(dPos, lens_dist[3]);

	glUniformMatrix3fv(mPos, 1, 0, m);

	glUniform1f(rot0Pos, rot0);
	glUniform1f(rot1Pos, rot1);

	glUniform1f(scale0Pos, scale0);
	glUniform1f(scale1Pos, scale1);

	glUniform1f(distPos, distance);

	glUniform1f(aspect_dwPos, aspect_dw);
	glUniform1f(aspect_swPos, aspect_sw);

	glUniform1f(opacity_Pos, opacity_mode);
	glUniform1i(cam_indexPos, cam_idx);
}

void RectlinearPanoramaWarpProg::setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int opacity_mode, int cam_idx)
{
	SingleTexProg::use(tex);

	if (aPos == 0) {
		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");

		mPos = glGetUniformLocation(prog, "m");

		rot0Pos = glGetUniformLocation(prog, "rot0");
		rot1Pos = glGetUniformLocation(prog, "rot1");

		scale0Pos = glGetUniformLocation(prog, "scale0");
		scale1Pos = glGetUniformLocation(prog, "scale1");

		distPos = glGetUniformLocation(prog, "dist");

		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");

		opacity_Pos = glGetUniformLocation(prog, "opacity_mode");
		cam_indexPos = glGetUniformLocation(prog, "cam_index");
	}

	glUniform1f(aPos, lens_dist[0]);
	glUniform1f(bPos, lens_dist[1]);
	glUniform1f(cPos, lens_dist[2]);
	glUniform1f(dPos, lens_dist[3]);

	glUniformMatrix3fv(mPos, 1, 0, m);

	glUniform1f(rot0Pos, rot0);
	glUniform1f(rot1Pos, rot1);

	glUniform1f(scale0Pos, scale0);
	glUniform1f(scale1Pos, scale1);

	glUniform1f(distPos, distance);

	glUniform1f(aspect_dwPos, aspect_dw);
	glUniform1f(aspect_swPos, aspect_sw);

	glUniform1i(opacity_Pos, opacity_mode);
	glUniform1i(cam_indexPos, cam_idx);
}

void RectlinearWarpProg::setting(GLuint tex, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int opacity_mode, int cam_idx, float yaw, float pitch, float roll)
{
	SingleTexProg::use(tex);

	if (aPos == 0) {
		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");

		mPos = glGetUniformLocation(prog, "m");

		rot0Pos = glGetUniformLocation(prog, "rot0");
		rot1Pos = glGetUniformLocation(prog, "rot1");

		scale0Pos = glGetUniformLocation(prog, "scale0");
		scale1Pos = glGetUniformLocation(prog, "scale1");

		distPos = glGetUniformLocation(prog, "dist");

		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");

		opacity_Pos = glGetUniformLocation(prog, "opacity_mode");
		cam_indexPos = glGetUniformLocation(prog, "cam_index");

		yawPos = glGetUniformLocation(prog, "yaw");
		pitchPos = glGetUniformLocation(prog, "pitch");
		rollPos = glGetUniformLocation(prog, "roll");
	}

	glUniform1f(aPos, lens_dist[0]);
	glUniform1f(bPos, lens_dist[1]);
	glUniform1f(cPos, lens_dist[2]);
	glUniform1f(dPos, lens_dist[3]);

	glUniformMatrix3fv(mPos, 1, 0, m);

	glUniform1f(rot0Pos, rot0);
	glUniform1f(rot1Pos, rot1);

	glUniform1f(scale0Pos, scale0);
	glUniform1f(scale1Pos, scale1);

	glUniform1f(distPos, distance);

	glUniform1f(aspect_dwPos, aspect_dw);
	glUniform1f(aspect_swPos, aspect_sw);

	glUniform1i(opacity_Pos, opacity_mode);
	glUniform1i(cam_indexPos, cam_idx);

	glUniform1i(yawPos, yaw);
	glUniform1i(pitchPos, pitch);
	glUniform1i(rollPos, roll);
}

void FullframeFisheyeStereoProg::setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode,
	float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int gray_mode, float brightness_step, float animation_time, int cam_index)
{
	//TwoTexProg::use(tex, mask);
	//ThreeTexProg::use(tex, mask, lut);
	//FiveTexProg::use(tex, mask, lut0, lut1, lut2);
	//SixTexProg::use(tex, mask, lut0, lut1, lut2, map);
	SevenTexProg::use(tex, mask, lut0, lut1, lut2, map, before_map);

	if (aPos == 0) {
		//isMapSet = glGetUniformLocation(prog, "isMapSet");
		//isMapAnimation = glGetUniformLocation(prog, "isMapAnimation");
		//mapWidth = glGetUniformLocation(prog, "mapWidth");
		//mapHeight = glGetUniformLocation(prog, "mapHeight");

		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");

		mPos = glGetUniformLocation(prog, "m");

		rot0Pos = glGetUniformLocation(prog, "rot0");
		rot1Pos = glGetUniformLocation(prog, "rot1");

		scale0Pos = glGetUniformLocation(prog, "scale0");
		scale1Pos = glGetUniformLocation(prog, "scale1");

		distPos = glGetUniformLocation(prog, "dist");

		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");

		laplacian_modePos = glGetUniformLocation(prog, "laplacian_mode");

		r_gainPos = glGetUniformLocation(prog, "r_gain");
		g_gainPos = glGetUniformLocation(prog, "g_gain");
		b_gainPos = glGetUniformLocation(prog, "b_gain");

		gain_modePos = glGetUniformLocation(prog, "gain_mode");
		gray_modePos = glGetUniformLocation(prog, "gray_mode");

		brightness_stepPos = glGetUniformLocation(prog, "brightness_step");
		animation_timePos = glGetUniformLocation(prog, "animation_time");

		crop_ratio_wPos = glGetUniformLocation(prog, "crop_ratio_w");
		crop_ratio_hPos = glGetUniformLocation(prog, "crop_ratio_h");
		crop_sxPos = glGetUniformLocation(prog, "crop_sx");
		crop_syPos = glGetUniformLocation(prog, "crop_sy");
		cam_indexPos = glGetUniformLocation(prog, "cam_index");
	}
	//glUniform1i(isMapSet, mapSet);
	//glUniform1i(isMapAnimation, resetMapAnimation);
	//glUniform1f(mapWidth, mapw);
	//glUniform1f(mapHeight, maph);

	glUniform1f(aPos, lens_dist[0]);
	glUniform1f(bPos, lens_dist[1]);
	glUniform1f(cPos, lens_dist[2]);
	glUniform1f(dPos, lens_dist[3]);

	glUniformMatrix3fv(mPos, 1, 0, m);

	glUniform1f(rot0Pos, rot0);
	glUniform1f(rot1Pos, rot1);

	glUniform1f(scale0Pos, scale0);
	glUniform1f(scale1Pos, scale1);

	glUniform1f(distPos, distance);

	glUniform1f(aspect_dwPos, aspect_dw);
	glUniform1f(aspect_swPos, aspect_sw);

	glUniform1i(laplacian_modePos, laplacian_mode);

	glUniform1f(r_gainPos, r_gain);
	glUniform1f(g_gainPos, g_gain);
	glUniform1f(b_gainPos, b_gain);

	glUniform1i(gain_modePos, gain_mode);
	glUniform1i(gray_modePos, gray_mode);

	glUniform1f(brightness_stepPos, brightness_step);
	glUniform1f(animation_timePos, animation_time);

	glUniform1f(crop_ratio_wPos, crop_ratio_w);
	glUniform1f(crop_ratio_hPos, crop_ratio_h);
	glUniform1f(crop_sxPos, crop_sx);
	glUniform1f(crop_syPos, crop_sy);
	glUniform1i(cam_indexPos, cam_index);
}

void RectlinearWarpUnalphaedPanoramaWarapedMaskProg::setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode,
	float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int resetMapAnimation, int lens_type)
{
	//TwoTexProg::use(tex, mask);
	//ThreeTexProg::use(tex, mask, lut);
	//FiveTexProg::use(tex, mask, lut0, lut1, lut2);
	//SixTexProg::use(tex, mask, lut0, lut1, lut2, map);
	SevenTexProg::use(tex, mask, lut0, lut1, lut2, map, before_map);
	
	if (aPos == 0) {
		isMapSet = glGetUniformLocation(prog, "isMapSet");
		isMapAnimation = glGetUniformLocation(prog, "isMapAnimation");
		mapWidth = glGetUniformLocation(prog, "mapWidth");
		mapHeight = glGetUniformLocation(prog, "mapHeight");

		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");

		mPos = glGetUniformLocation(prog, "m");

		rot0Pos = glGetUniformLocation(prog, "rot0");
		rot1Pos = glGetUniformLocation(prog, "rot1");

		scale0Pos = glGetUniformLocation(prog, "scale0");
		scale1Pos = glGetUniformLocation(prog, "scale1");

		distPos = glGetUniformLocation(prog, "dist");

		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");

		laplacian_modePos = glGetUniformLocation(prog, "laplacian_mode");

		r_gainPos = glGetUniformLocation(prog, "r_gain");
		g_gainPos = glGetUniformLocation(prog, "g_gain");
		b_gainPos = glGetUniformLocation(prog, "b_gain");

		gain_modePos = glGetUniformLocation(prog, "gain_mode");
		gray_modePos = glGetUniformLocation(prog, "gray_mode");

		brightness_stepPos = glGetUniformLocation(prog, "brightness_step");
		animation_timePos = glGetUniformLocation(prog, "animation_time");

		crop_ratio_wPos = glGetUniformLocation(prog, "crop_ratio_w");
		crop_ratio_hPos = glGetUniformLocation(prog, "crop_ratio_h");
		crop_sxPos = glGetUniformLocation(prog, "crop_sx");
		crop_syPos = glGetUniformLocation(prog, "crop_sy");
		lensType = glGetUniformLocation(prog, "lens_type");
	}
	glUniform1i(isMapSet, mapSet);
	glUniform1i(isMapAnimation, resetMapAnimation);
	glUniform1f(mapWidth, mapw);
	glUniform1f(mapHeight, maph);

	glUniform1f(aPos, lens_dist[0]);
	glUniform1f(bPos, lens_dist[1]);
	glUniform1f(cPos, lens_dist[2]);
	glUniform1f(dPos, lens_dist[3]);

	glUniformMatrix3fv(mPos, 1, 0, m);

	glUniform1f(rot0Pos, rot0);
	glUniform1f(rot1Pos, rot1);

	glUniform1f(scale0Pos, scale0);
	glUniform1f(scale1Pos, scale1);

	glUniform1f(distPos, distance);

	glUniform1f(aspect_dwPos, aspect_dw);
	glUniform1f(aspect_swPos, aspect_sw);

	glUniform1i(laplacian_modePos, laplacian_mode);

	glUniform1f(r_gainPos, r_gain);
	glUniform1f(g_gainPos, g_gain);
	glUniform1f(b_gainPos, b_gain);

	glUniform1i(gain_modePos, gain_mode);
	glUniform1i(gray_modePos, gray_mode);

	glUniform1f(brightness_stepPos, brightness_step);
	glUniform1f(animation_timePos, animation_time);

	glUniform1f(crop_ratio_wPos, crop_ratio_w);
	glUniform1f(crop_ratio_hPos, crop_ratio_h);
	glUniform1f(crop_sxPos, crop_sx);
	glUniform1f(crop_syPos, crop_sy);

	glUniform1i(lensType, lens_type);
}

//void FullframeFisheyeWarpUnalphaedCylindricalWarapedMaskProg::setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode,
//	float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int resetMapAnimation)
//{
//	//TwoTexProg::use(tex, mask);
//	//ThreeTexProg::use(tex, mask, lut);
//	//FiveTexProg::use(tex, mask, lut0, lut1, lut2);
//	//SixTexProg::use(tex, mask, lut0, lut1, lut2, map);
//	SevenTexProg::use(tex, mask, lut0, lut1, lut2, map, before_map);
//
//	if (aPos == 0) {
//		isMapSet = glGetUniformLocation(prog, "isMapSet");
//		isMapAnimation = glGetUniformLocation(prog, "isMapAnimation");
//		mapWidth = glGetUniformLocation(prog, "mapWidth");
//		mapHeight = glGetUniformLocation(prog, "mapHeight");
//
//		aPos = glGetUniformLocation(prog, "a");
//		bPos = glGetUniformLocation(prog, "b");
//		cPos = glGetUniformLocation(prog, "c");
//		dPos = glGetUniformLocation(prog, "d");
//
//		mPos = glGetUniformLocation(prog, "m");
//
//		rot0Pos = glGetUniformLocation(prog, "rot0");
//		rot1Pos = glGetUniformLocation(prog, "rot1");
//
//		scale0Pos = glGetUniformLocation(prog, "scale0");
//		scale1Pos = glGetUniformLocation(prog, "scale1");
//
//		distPos = glGetUniformLocation(prog, "dist");
//
//		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
//		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");
//
//		laplacian_modePos = glGetUniformLocation(prog, "laplacian_mode");
//
//		r_gainPos = glGetUniformLocation(prog, "r_gain");
//		g_gainPos = glGetUniformLocation(prog, "g_gain");
//		b_gainPos = glGetUniformLocation(prog, "b_gain");
//
//		gain_modePos = glGetUniformLocation(prog, "gain_mode");
//		gray_modePos = glGetUniformLocation(prog, "gray_mode");
//
//		brightness_stepPos = glGetUniformLocation(prog, "brightness_step");
//		animation_timePos = glGetUniformLocation(prog, "animation_time");
//
//		crop_ratio_wPos = glGetUniformLocation(prog, "crop_ratio_w");
//		crop_ratio_hPos = glGetUniformLocation(prog, "crop_ratio_h");
//		crop_sxPos = glGetUniformLocation(prog, "crop_sx");
//		crop_syPos = glGetUniformLocation(prog, "crop_sy");
//	}
//	glUniform1i(isMapSet, mapSet);
//	glUniform1i(isMapAnimation, resetMapAnimation);
//	glUniform1f(mapWidth, mapw);
//	glUniform1f(mapHeight, maph);
//
//	glUniform1f(aPos, lens_dist[0]);
//	glUniform1f(bPos, lens_dist[1]);
//	glUniform1f(cPos, lens_dist[2]);
//	glUniform1f(dPos, lens_dist[3]);
//
//	glUniformMatrix3fv(mPos, 1, 0, m);
//
//	glUniform1f(rot0Pos, rot0);
//	glUniform1f(rot1Pos, rot1);
//
//	glUniform1f(scale0Pos, scale0);
//	glUniform1f(scale1Pos, scale1);
//
//	glUniform1f(distPos, distance);
//
//	glUniform1f(aspect_dwPos, aspect_dw);
//	glUniform1f(aspect_swPos, aspect_sw);
//
//	glUniform1i(laplacian_modePos, laplacian_mode);
//
//	glUniform1f(r_gainPos, r_gain);
//	glUniform1f(g_gainPos, g_gain);
//	glUniform1f(b_gainPos, b_gain);
//
//	glUniform1i(gain_modePos, gain_mode);
//	glUniform1i(gray_modePos, gray_mode);
//
//	glUniform1f(brightness_stepPos, brightness_step);
//	glUniform1f(animation_timePos, animation_time);
//
//	glUniform1f(crop_ratio_wPos, crop_ratio_w);
//	glUniform1f(crop_ratio_hPos, crop_ratio_h);
//	glUniform1f(crop_sxPos, crop_sx);
//	glUniform1f(crop_syPos, crop_sy);
//}

void FullframeFisheyeWarpUnalphaedWarapedMaskProg::setting(GLuint tex, GLuint mask, GLuint lut0, GLuint lut1, GLuint lut2, GLuint map, GLuint before_map, float m[], float rot0, float rot1, float scale0, float scale1, float distance, float aspect_dw, float aspect_sw, float lens_dist[], int laplacian_mode,
	float r_gain, float g_gain, float b_gain, int gain_mode, float crop_ratio_w, float crop_ratio_h, float crop_sx, float crop_sy, int mapSet, float mapw, float maph, int gray_mode, float brightness_step, float animation_time, int resetMapAnimation)
{
	//TwoTexProg::use(tex, mask);
	//ThreeTexProg::use(tex, mask, lut);
	//FiveTexProg::use(tex, mask, lut0, lut1, lut2);
	//SixTexProg::use(tex, mask, lut0, lut1, lut2, map);
	SevenTexProg::use(tex, mask, lut0, lut1, lut2, map, before_map);

	if (aPos == 0) {
		isMapSet = glGetUniformLocation(prog, "isMapSet");
		isMapAnimation = glGetUniformLocation(prog, "isMapAnimation");
		mapWidth = glGetUniformLocation(prog, "mapWidth");
		mapHeight = glGetUniformLocation(prog, "mapHeight");

		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");

		mPos = glGetUniformLocation(prog, "m");

		rot0Pos = glGetUniformLocation(prog, "rot0");
		rot1Pos = glGetUniformLocation(prog, "rot1");

		scale0Pos = glGetUniformLocation(prog, "scale0");
		scale1Pos = glGetUniformLocation(prog, "scale1");

		distPos = glGetUniformLocation(prog, "dist");

		aspect_dwPos = glGetUniformLocation(prog, "aspect_dw");
		aspect_swPos = glGetUniformLocation(prog, "aspect_sw");

		laplacian_modePos = glGetUniformLocation(prog, "laplacian_mode");

		r_gainPos = glGetUniformLocation(prog, "r_gain");
		g_gainPos = glGetUniformLocation(prog, "g_gain");
		b_gainPos = glGetUniformLocation(prog, "b_gain");

		gain_modePos = glGetUniformLocation(prog, "gain_mode");
		gray_modePos = glGetUniformLocation(prog, "gray_mode");
		
		brightness_stepPos = glGetUniformLocation(prog, "brightness_step");
		animation_timePos = glGetUniformLocation(prog, "animation_time");

		crop_ratio_wPos = glGetUniformLocation(prog, "crop_ratio_w");
		crop_ratio_hPos = glGetUniformLocation(prog, "crop_ratio_h");
		crop_sxPos = glGetUniformLocation(prog, "crop_sx");
		crop_syPos = glGetUniformLocation(prog, "crop_sy");
	}
	glUniform1i(isMapSet, mapSet);
	glUniform1i(isMapAnimation, resetMapAnimation);
	glUniform1f(mapWidth, mapw);
	glUniform1f(mapHeight, maph);

	glUniform1f(aPos, lens_dist[0]);
	glUniform1f(bPos, lens_dist[1]);
	glUniform1f(cPos, lens_dist[2]);
	glUniform1f(dPos, lens_dist[3]);

	glUniformMatrix3fv(mPos, 1, 0, m);

	glUniform1f(rot0Pos, rot0);
	glUniform1f(rot1Pos, rot1);

	glUniform1f(scale0Pos, scale0);
	glUniform1f(scale1Pos, scale1);

	glUniform1f(distPos, distance);

	glUniform1f(aspect_dwPos, aspect_dw);
	glUniform1f(aspect_swPos, aspect_sw);

	glUniform1i(laplacian_modePos, laplacian_mode);

	glUniform1f(r_gainPos, r_gain);
	glUniform1f(g_gainPos, g_gain);
	glUniform1f(b_gainPos, b_gain);

	glUniform1i(gain_modePos, gain_mode);
	glUniform1i(gray_modePos, gray_mode);

	glUniform1f(brightness_stepPos, brightness_step);
	glUniform1f(animation_timePos, animation_time);

	glUniform1f(crop_ratio_wPos, crop_ratio_w);
	glUniform1f(crop_ratio_hPos, crop_ratio_h);
	glUniform1f(crop_sxPos, crop_sx);
	glUniform1f(crop_syPos, crop_sy);
}

#define CORRECT_LENSE_DIST \
	"vec2 correctLensDist(vec2 p){\n" \
	"	p = vec2(p.x,p.y*aspect);\n" \
	"	float rn = dot(p,p), r=sqrt(rn);\n" \
	"	rn = (a*rn+b*r)*rn+c*rn+(1.+d)*r;\n" \
	"	if( r>0. ) p=p*r/rn; else p=vec2(0.0);\n" \
	"	return vec2(p.x,p.y/aspect);\n" \
	"}\n"

std::string UndistortProg::getFShader() const { return
	"uniform sampler2D tex;\n"
	"uniform float a, b, c, d;\n"
	"uniform float aspect;\n"
	CORRECT_LENSE_DIST
	"void main() {\n"
	"	vec2 p =correctLensDist(gl_TexCoord[0].st-vec2(.5,.5))+vec2(.5,.5);\n"
	"	vec4 c0=vec4(0,0,0,0);\n"
	"	if(p.x>=0. && p.x<=1. && p.y>=0. && p.y<=1.) c0 =texture2D(tex, p);\n"
	"	gl_FragColor = vec4(c0.rgb*c0.a,c0.a);\n"
	"}\n";
}

void WarpProg::use(GLuint tex1, GLuint tex2, const float m[], float focal, float aspect, const float dist[]) {
	//TwoTexProg::use(tex1,tex2);
	//ThreeTexProg::use(tex1, tex2, NULL);
	//FiveTexProg::use(tex1, tex2, NULL, NULL, NULL);
	//SixTexProg::use(tex1, tex2, NULL, NULL, NULL, NULL);
	SevenTexProg::use(tex1, tex2, NULL, NULL, NULL, NULL, NULL);
	if(aPos==0) {
		focalPos = glGetUniformLocation(prog, "focal");
		mPos = glGetUniformLocation(prog, "m");
		aspectPos = glGetUniformLocation(prog, "aspect");
		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");
	}
	glUniformMatrix3fv(mPos, 1, 0, m);
	glUniform1f(focalPos, focal);
	glUniform1f(aspectPos, aspect);
	glUniform1f(aPos, dist[0]);
	glUniform1f(bPos, dist[1]);
	glUniform1f(cPos, dist[2]);
	glUniform1f(dPos, dist[3]);
}

void WarpSingleProg::use(GLuint tex1, const float m[], float focal, float aspect, const float dist[]) {
	SingleTexProg::use(tex1);
	if (aPos == 0) {
		focalPos = glGetUniformLocation(prog, "focal");
		mPos = glGetUniformLocation(prog, "m");
		aspectPos = glGetUniformLocation(prog, "aspect");
		aPos = glGetUniformLocation(prog, "a");
		bPos = glGetUniformLocation(prog, "b");
		cPos = glGetUniformLocation(prog, "c");
		dPos = glGetUniformLocation(prog, "d");
	}
	glUniformMatrix3fv(mPos, 1, 0, m);
	glUniform1f(focalPos, focal);
	glUniform1f(aspectPos, aspect);
	glUniform1f(aPos, dist[0]);
	glUniform1f(bPos, dist[1]);
	glUniform1f(cPos, dist[2]);
	glUniform1f(dPos, dist[3]);
}

#define SCREEN_TO_IMAGE \
	"vec3 screenToImage(vec2 p) {\n" \
	"	const float PI=3.14159265358979;\n" \
	"	float rho = (p.y-.5)*PI, theta=p.x*PI*2., cosRho=cos(rho);\n" \
	"	vec3 v = m*vec3(cosRho*sin(theta),sin(rho),cos(theta)*cosRho);\n" \
	"	float fac = focal/v.z, tx=v.x*fac, ty=-v.y*fac/aspect;\n" \
	"	return vec3(tx+.5,ty+.5,v.z);\n" \
	"}\n"


std::string WarpProg::getFShader() const { return
	"uniform sampler2D tex1, tex2;\n"
	"uniform mat3 m;\n"
	"uniform float a, b, c, d;\n"
	"uniform float focal, aspect;\n"
	CORRECT_LENSE_DIST
	SCREEN_TO_IMAGE
	"void main() {\n"
	"	vec3 v = screenToImage(gl_TexCoord[0].st);\n"
	"	vec4 c0=vec4(0,0,0,0);\n"
	"	float a=0.0;\n"
	"	if(v.z<0. && v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) {\n"
	"		a =texture2D(tex2, v.xy).a;\n"
	"		v.xy =correctLensDist(v.xy-vec2(.5,.5))+vec2(.5,.5);\n"
	"	}\n"
	"	c0 =texture2D(tex1, v.xy);\n"
	"	gl_FragColor = vec4(c0.rgb*a,a);\n"
"}\n";
}

std::string WarpSingleProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"uniform mat3 m;\n"
		"uniform float a, b, c, d;\n"
		"uniform float focal, aspect;\n"
		CORRECT_LENSE_DIST
		SCREEN_TO_IMAGE
		"void main() {\n"
		"	vec3 v = screenToImage(gl_TexCoord[0].st);\n"
		"	vec4 c0=vec4(0,0,0,0);\n"
		"	if(v.z<0. && v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) {\n"
		"		v.xy =correctLensDist(v.xy-vec2(.5,.5))+vec2(.5,.5);\n"
		"	}\n"
		"	c0 =texture2D(tex1, v.xy);\n"
		"	gl_FragColor = vec4(c0);\n"
		"}\n";
}

std::string WarpUnalphaedProg::getFShader() const { return
	"uniform sampler2D tex1, tex2;\n"
	"uniform mat3 m;\n"
	"uniform float a, b, c, d;\n"
	"uniform float focal, aspect;\n"
	CORRECT_LENSE_DIST
	SCREEN_TO_IMAGE
	"void main() {\n"
	"	vec3 v = screenToImage(gl_TexCoord[0].st);\n"
	"	vec4 c0=vec4(0,0,0,0);\n"
	"	float a=0.0;\n"
	"	if(v.z<0. && v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
	"		a =texture2D(tex2, v.xy).a;\n"
	"	v.xy =correctLensDist(v.xy-vec2(.5,.5))+vec2(.5,.5);\n"
	"	c0 =texture2D(tex1, v.xy);\n"
	"	gl_FragColor = vec4(c0.rgb,a);\n"
"}\n";
}

std::string WarpUnalphaedWarapedMaskProg::getFShader() const { return
	"uniform sampler2D tex1, tex2;\n"
	"uniform mat3 m;\n"
	"uniform float a, b, c, d;\n"
	"uniform float focal, aspect;\n"
	CORRECT_LENSE_DIST
	SCREEN_TO_IMAGE
	"void main() {\n"
	"	vec3 v = screenToImage(gl_TexCoord[0].st);\n"
	"	vec4 c0=vec4(0,0,0,0);\n"
	"	float a=0.0;\n"
	"	if(v.z<0. && v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
	"		a =texture2D(tex2, gl_TexCoord[0].st).a;\n"
	"	v.xy =correctLensDist(v.xy-vec2(.5,.5))+vec2(.5,.5);\n"
	"	c0 =texture2D(tex1, v.xy);\n"
	"	gl_FragColor = vec4(c0.rgb,a);\n"
"}\n";
}

std::string FullframeFisheyeStereoProg::getFShader() const {
	return
		"uniform sampler2D tex1, tex2;\n"
		"uniform sampler1D tex3, tex4, tex5;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform float r_gain;\n"
		"uniform float g_gain;\n"
		"uniform float b_gain;\n"
		"uniform int gain_mode;\n"
		"uniform int gray_mode;\n"
		"uniform float brightness_step;\n"
		"uniform float animation_time;\n"
		"uniform float crop_ratio_w, crop_ratio_h, crop_sx, crop_sy;\n"
		"uniform int cam_index;\n"

		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		"	if(cam_index == 0)\n"
		"		v.y = (v.y)*2.0;\n"	
		"	else if(cam_index == 1)\n"
		"		v.y = (v.y-0.5)*2.0;\n"
		"	else if(cam_index == 2)\n"
		"		v.x = (v.x)*2.0;\n"
		"	else if(cam_index == 3)\n"
		"		v.x = (v.x-0.5)*2.0;\n"
 
		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"

		"	vec4 c0=vec4(0,0,0,0);\n"
		"	float a=0.0;\n"

		"	v.x = (v.x-crop_sx)*crop_ratio_w;\n"
		"	v.y = (v.y-crop_sy)*crop_ratio_h;\n"

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"	{\n"
		"		if( sqrt( (v.x-0.5)*(v.x-0.5) + ( (v.y-0.5)*0.5625 )*((v.y-0.5)*0.5625) ) < (crop_ratio_w / 2.0)) \n"
		"			a =1.0;\n"
		"	}\n"
		//"		a =texture2D(tex2, tv).a;\n"


		"	c0 =texture2D(tex1, v.xy);\n"

		"	if(gain_mode == 0){\n"	//all of mode 
		"		c0.r = texture1D(tex3, c0.r + r_gain).a;\n"
		"		c0.g = texture1D(tex4, c0.g + g_gain).a;\n"
		"		c0.b = texture1D(tex5, c0.b + b_gain).a;\n"
		"	}\n"

		"	if(brightness_step != 0.0) {\n"
		"		c0.r = c0.r + brightness_step;\n"
		"		c0.g = c0.g + brightness_step;\n"
		"		c0.b = c0.b + brightness_step;\n"
		"	}\n"

		"	if(gray_mode == 1) {\n"
		"		float intensity = (c0.r*0.299 + c0.g*0.587 + c0.b*0.114);\n"	// BT.601
		//"	float intensity = (c0.r*0.3 + c0.g*0.59 + c0.b*0.11);\n" // photoshop
		//"	float intensity = (c0.r*0.2126 + c0.g*0.7152 + c0.b*0.0722);\n"	// Luma

		"		c0.r = intensity;\n"
		"		c0.g = intensity;\n"
		"		c0.b = intensity;\n"
		"	}\n"


		"	if(c0.r > 1.0)\n"
		"		c0.r = 1.0;\n"
		"	if(c0.g > 1.0)\n"
		"		c0.g = 1.0;\n"
		"	if(c0.b > 1.0)\n"
		"		c0.b = 1.0;\n"

		"	if(c0.r < 0.0)\n"
		"		c0.r = 0.0;\n"
		"	if(c0.g < 0.0)\n"
		"		c0.g = 0.0;\n"
		"	if(c0.b < 0.0)\n"
		"		c0.b = 0.0;\n"

		"	if(laplacian_mode == 1)\n"
		"		gl_FragColor = vec4(c0.rgb*a,a);\n"
		"	else\n"
		"		gl_FragColor = vec4(c0.rgb*a,a);\n"
		"}\n";
}

std::string RectlinearWarpUnalphaedPanoramaWarapedMaskProg::getFShader() const {
	return
		"uniform sampler2D tex1, tex2;\n"
		"uniform sampler1D tex3, tex4, tex5;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform float r_gain;\n"
		"uniform float g_gain;\n"
		"uniform float b_gain;\n"
		"uniform int gain_mode;\n"
		"uniform int gray_mode;\n"
		"uniform float brightness_step;\n"
		"uniform float animation_time;\n"
		"uniform float crop_ratio_w, crop_ratio_h, crop_sx, crop_sy;\n"
		"uniform int lens_type;\n"

		// warping code START
		"uniform sampler2D tex6;\n"
		"uniform sampler2D tex7;\n"
		"uniform int isMapSet;\n"
		"uniform int isMapAnimation;\n"
		"uniform float mapWidth;\n"
		"uniform float mapHeight;\n"
		// warping code END

		ERECT_PANO
		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		RECT_WARP_SPHERE
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		// MultiBand blending padding
		"   v.x = v.x*1.25;\n"
		"   v.x = v.x-0.125;\n"

		"	if(v.x > 1.0)\n"
		"		v.x = v.x-1.0;\n"
		"	if(v.x < 0.0)\n"
		"		v.x = v.x+1.0;\n"

		"	vec2 tv = v;\n"
		// MultiBand blending padding

		// warping code START
		"	if (isMapSet == 1){\n"
		"		float mapx1 = texture2D(tex6, v.xy).r;\n"
		"		float mapx2 = texture2D(tex6, v.xy).g;\n"
		"		float mapy1 = texture2D(tex6, v.xy).b;\n"
		"		float mapy2 = texture2D(tex6, v.xy).a;\n"

		"		mapx1 = mapx1 * 255.0;\n"
		"		mapy1 = mapy1 * 255.0;\n"

		"		if(mapx2 > 0.5){\n"
		"			mapx1 = -mapx1;\n"
		"			mapx2 = -(mapx2 - 0.5);\n"
		"		}\n"
		"		if(mapy2 > 0.5){\n"
		"			mapy1 = -mapy1;\n"
		"			mapy2 = -(mapy2 - 0.5);\n"
		"		}\n"

		"		float mapx = (mapx1 + mapx2 * 2.0) / mapWidth;\n"
		"		float mapy = (mapy1 + mapy2 * 2.0) / mapHeight;\n"

		"		float before_mapx1 = texture2D(tex7, v.xy).r;\n"
		"		float before_mapx2 = texture2D(tex7, v.xy).g;\n"
		"		float before_mapy1 = texture2D(tex7, v.xy).b;\n"
		"		float before_mapy2 = texture2D(tex7, v.xy).a;\n"

		"		before_mapx1 = before_mapx1 * 255.0;\n"
		"		before_mapy1 = before_mapy1 * 255.0;\n"

		"		if(before_mapx2 > 0.5){\n"
		"			before_mapx1 = -before_mapx1;\n"
		"			before_mapx2 = -(before_mapx2 - 0.5); \n"
		"		}\n"
		"		if(before_mapy2 > 0.5){\n"
		"			before_mapy1 = -before_mapy1;\n"
		"			before_mapy2 = -(before_mapy2 - 0.5); \n"
		"		}\n"

		"		float before_mapx = (before_mapx1 + before_mapx2 * 2.0) / mapWidth;\n"
		"		float before_mapy = (before_mapy1 + before_mapy2 * 2.0) / mapHeight;\n"

		"		float diff_mapx = mapx - before_mapx;\n"
		"		float diff_mapy = mapy - before_mapy;\n"

		"		if(animation_time <1.0f){\n"
		"			v.x += before_mapx + diff_mapx * animation_time;\n"
		"			v.y += before_mapy + diff_mapy * animation_time;\n"
		"		}\n"
		"		else{\n"
		"			v.x += mapx;\n"
		"			v.y += mapy;\n"
		"		}\n"
		"	}\n"
		"	else if (isMapAnimation == 1){\n"
		"		float before_mapx1 = texture2D(tex7, v.xy).r;\n"
		"		float before_mapx2 = texture2D(tex7, v.xy).g;\n"
		"		float before_mapy1 = texture2D(tex7, v.xy).b;\n"
		"		float before_mapy2 = texture2D(tex7, v.xy).a;\n"

		"		before_mapx1 = before_mapx1 * 255.0;\n"
		"		before_mapy1 = before_mapy1 * 255.0;\n"

		"		if(before_mapx2 > 0.5){\n"
		"			before_mapx1 = -before_mapx1;\n"
		"			before_mapx2 = -(before_mapx2 - 0.5); \n"
		"		}\n"
		"		if(before_mapy2 > 0.5){\n"
		"			before_mapy1 = -before_mapy1;\n"
		"			before_mapy2 = -(before_mapy2 - 0.5); \n"
		"		}\n"

		"		float before_mapx = (before_mapx1 + before_mapx2 * 2.0) / mapWidth;\n"
		"		float before_mapy = (before_mapy1 + before_mapy2 * 2.0) / mapHeight;\n"

		"		v.x += before_mapx * (1 - animation_time);\n"
		"		v.y += before_mapy * (1 - animation_time);\n"
		"	}\n"
//		 warping code END

		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		"	v = erect_pano(v, dist);\n"
		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"   if(lens_type == 0)\n"
		"		v = rect_warp_sphere(v, dist);\n"

		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"


		"	vec4 c0=vec4(0,0,0,0);\n"
		"	float a=0.0;\n"

		/*"	v.x = (v.x-crop_sx)*crop_ratio_w;\n"
		"	v.y = (v.y-crop_sy)*crop_ratio_h;\n"*/

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) { \n"
		"		if(texture2D(tex2, tv).a > 0.99) \n"
		"			a =texture2D(tex2, tv).a;\n"
		"		c0 =texture2D(tex1, v.xy);}\n"
		//"   else \n"
		//"		a = 0;\n"
		

		"	if(gain_mode == 0){\n"	//all of mode 
		"		c0.r = texture1D(tex3, c0.r + r_gain).a;\n"
		"		c0.g = texture1D(tex4, c0.g + g_gain).a;\n"
		"		c0.b = texture1D(tex5, c0.b + b_gain).a;\n"
		"	}\n"

		"	if(brightness_step != 0.0) {\n"
		"		c0.r = c0.r + brightness_step;\n"
		"		c0.g = c0.g + brightness_step;\n"
		"		c0.b = c0.b + brightness_step;\n"
		"	}\n"

		"	if(gray_mode == 1) {\n"
		"		float intensity = (c0.r*0.299 + c0.g*0.587 + c0.b*0.114);\n"	// BT.601
		//"	float intensity = (c0.r*0.3 + c0.g*0.59 + c0.b*0.11);\n" // photoshop
		//"	float intensity = (c0.r*0.2126 + c0.g*0.7152 + c0.b*0.0722);\n"	// Luma
		"		c0.r = intensity;\n"
		"		c0.g = intensity;\n"
		"		c0.b = intensity;\n"
		"	}\n"


		"	if(c0.r > 1.0)\n"
		"		c0.r = 1.0;\n"
		"	if(c0.g > 1.0)\n"
		"		c0.g = 1.0;\n"
		"	if(c0.b > 1.0)\n"
		"		c0.b = 1.0;\n"

		"	if(c0.r < 0.0)\n"
		"		c0.r = 0.0;\n"
		"	if(c0.g < 0.0)\n"
		"		c0.g = 0.0;\n"
		"	if(c0.b < 0.0)\n"
		"		c0.b = 0.0;\n"

		"	if(laplacian_mode == 1)\n"
		"		gl_FragColor = vec4(c0.rgb,a);\n"
		"	else\n"
		"		gl_FragColor = vec4(c0.rgb*a,a);\n"
		"}\n";
}

std::string FullframeFisheyeWarpUnalphaedWarapedMaskProg::getFShader() const {
	return
		"uniform sampler2D tex1, tex2;\n"
		"uniform sampler1D tex3, tex4, tex5;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform float r_gain;\n"
		"uniform float g_gain;\n"
		"uniform float b_gain;\n"
		"uniform int gain_mode;\n"
		"uniform int gray_mode;\n"
		"uniform float brightness_step;\n"
		"uniform float animation_time;\n"
		"uniform float crop_ratio_w, crop_ratio_h, crop_sx, crop_sy;\n"

		// warping code START
		"uniform sampler2D tex6;\n"
		"uniform sampler2D tex7;\n"
		"uniform int isMapSet;\n"
		"uniform int isMapAnimation;\n"
		"uniform float mapWidth;\n"
		"uniform float mapHeight;\n"
		// warping code END
		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		// MultiBand blending padding
		"   v.x = v.x*1.25;\n"
		"   v.x = v.x-0.125;\n"

		"	if(v.x > 1.0)\n"
		"		v.x = v.x-1.0;\n"
		"	if(v.x < 0.0)\n"
		"		v.x = v.x+1.0;\n"

		"	vec2 tv = v;\n"
		// MultiBand blending padding

		// warping code START
		"	if (isMapSet == 1){\n"
		"		float mapx1 = texture2D(tex6, v.xy).r;\n"
		"		float mapx2 = texture2D(tex6, v.xy).g;\n"
		"		float mapy1 = texture2D(tex6, v.xy).b;\n"
		"		float mapy2 = texture2D(tex6, v.xy).a;\n"

		"		mapx1 = mapx1 * 255.0;\n"
		"		mapy1 = mapy1 * 255.0;\n"

		"		if(mapx2 > 0.5){\n"
		"			mapx1 = -mapx1;\n"
		"			mapx2 = -(mapx2 - 0.5);\n"
		"		}\n"
		"		if(mapy2 > 0.5){\n"
		"			mapy1 = -mapy1;\n"
		"			mapy2 = -(mapy2 - 0.5);\n"
		"		}\n"

		"		float mapx = (mapx1 + mapx2 * 2.0) / mapWidth;\n"
		"		float mapy = (mapy1 + mapy2 * 2.0) / mapHeight;\n"

		"		float before_mapx1 = texture2D(tex7, v.xy).r;\n"
		"		float before_mapx2 = texture2D(tex7, v.xy).g;\n"
		"		float before_mapy1 = texture2D(tex7, v.xy).b;\n"
		"		float before_mapy2 = texture2D(tex7, v.xy).a;\n"

		"		before_mapx1 = before_mapx1 * 255.0;\n"
		"		before_mapy1 = before_mapy1 * 255.0;\n"

		"		if(before_mapx2 > 0.5){\n"
		"			before_mapx1 = -before_mapx1;\n"
		"			before_mapx2 = -(before_mapx2 - 0.5); \n"
		"		}\n"
		"		if(before_mapy2 > 0.5){\n"
		"			before_mapy1 = -before_mapy1;\n"
		"			before_mapy2 = -(before_mapy2 - 0.5); \n"
		"		}\n"

		"		float before_mapx = (before_mapx1 + before_mapx2 * 2.0) / mapWidth;\n"
		"		float before_mapy = (before_mapy1 + before_mapy2 * 2.0) / mapHeight;\n"

		"		float diff_mapx = mapx - before_mapx;\n"
		"		float diff_mapy = mapy - before_mapy;\n"

		"		if(animation_time <1.0f){\n"
		"			v.x += before_mapx + diff_mapx * animation_time;\n"
		"			v.y += before_mapy + diff_mapy * animation_time;\n"
		"		}\n"
		"		else{\n"
		"			v.x += mapx;\n"
		"			v.y += mapy;\n"
		"		}\n"
		"	}\n"
		"	else if (isMapAnimation == 1){\n"
		"		float before_mapx1 = texture2D(tex7, v.xy).r;\n"
		"		float before_mapx2 = texture2D(tex7, v.xy).g;\n"
		"		float before_mapy1 = texture2D(tex7, v.xy).b;\n"
		"		float before_mapy2 = texture2D(tex7, v.xy).a;\n"

		"		before_mapx1 = before_mapx1 * 255.0;\n"
		"		before_mapy1 = before_mapy1 * 255.0;\n"

		"		if(before_mapx2 > 0.5){\n"
		"			before_mapx1 = -before_mapx1;\n"
		"			before_mapx2 = -(before_mapx2 - 0.5); \n"
		"		}\n"
		"		if(before_mapy2 > 0.5){\n"
		"			before_mapy1 = -before_mapy1;\n"
		"			before_mapy2 = -(before_mapy2 - 0.5); \n"
		"		}\n"

		"		float before_mapx = (before_mapx1 + before_mapx2 * 2.0) / mapWidth;\n"
		"		float before_mapy = (before_mapy1 + before_mapy2 * 2.0) / mapHeight;\n"

		"		v.x += before_mapx * (1 - animation_time);\n"
		"		v.y += before_mapy * (1 - animation_time);\n"
		"	}\n"
		// warping code END

		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"

		"	vec4 c0=vec4(0,0,0,0);\n"
		"	float a=0.0;\n"

		"	v.x = (v.x-crop_sx)*crop_ratio_w;\n"
		"	v.y = (v.y-crop_sy)*crop_ratio_h;\n"

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"		a =texture2D(tex2, tv).a;\n"

		"	c0 =texture2D(tex1, v.xy);\n"

		"	if(gain_mode == 0){\n"	//all of mode 
		"		c0.r = texture1D(tex3, c0.r + r_gain).a;\n"
		"		c0.g = texture1D(tex4, c0.g + g_gain).a;\n"
		"		c0.b = texture1D(tex5, c0.b + b_gain).a;\n"
		"	}\n"

		"	if(brightness_step != 0.0) {\n"
		"		c0.r = c0.r + brightness_step;\n"
		"		c0.g = c0.g + brightness_step;\n"
		"		c0.b = c0.b + brightness_step;\n"
		"	}\n"

		"	if(gray_mode == 1) {\n"
		"		float intensity = (c0.r*0.299 + c0.g*0.587 + c0.b*0.114);\n"	// BT.601
		//"	float intensity = (c0.r*0.3 + c0.g*0.59 + c0.b*0.11);\n" // photoshop
		//"	float intensity = (c0.r*0.2126 + c0.g*0.7152 + c0.b*0.0722);\n"	// Luma

		"		c0.r = intensity;\n"
		"		c0.g = intensity;\n"
		"		c0.b = intensity;\n"
		"	}\n"
			

		"	if(c0.r > 1.0)\n"
		"		c0.r = 1.0;\n"
		"	if(c0.g > 1.0)\n"
		"		c0.g = 1.0;\n"
		"	if(c0.b > 1.0)\n"
		"		c0.b = 1.0;\n"

		"	if(c0.r < 0.0)\n"
		"		c0.r = 0.0;\n"
		"	if(c0.g < 0.0)\n"
		"		c0.g = 0.0;\n"
		"	if(c0.b < 0.0)\n"
		"		c0.b = 0.0;\n"

		"	if(laplacian_mode == 1)\n"
		"		gl_FragColor = vec4(c0.rgb,a);\n"
		"	else\n"
		"		gl_FragColor = vec4(c0.rgb*a,a);\n"
		"}\n";
}

std::string FullframeFisheyeWarpProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform float opacity_mode;\n"
		"uniform int cam_index;\n"
		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		PERSPECTIVE_RECT_SPHERICAL
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		"	if(cam_index == 0)\n"
		"		v.y = (v.y)*2.0;\n"
		"	else if(cam_index == 1)\n"
		"		v.y = (v.y-0.5)*2.0;\n"
		"	else if(cam_index == 2)\n"
		"		v.x = (v.x)*2.0;\n"
		"	else if(cam_index == 3)\n"
		"		v.x = (v.x-0.5)*2.0;\n"

		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"	v = rect_sphere(v, dist);\n"
		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"

		"	vec4 c0=vec4(0,0,0,0);\n"

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"	{\n"
		"		c0 =texture2D(tex, v.xy);\n"
		"		if(opacity_mode < 1.0)\n"
		"		c0.w *= opacity_mode;\n"
		"	}\n"
		"	gl_FragColor = vec4(c0);\n"

		"}\n";
}
std::string RectlinearPanoramaWarpProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform int opacity_mode;\n"
		"uniform int cam_index;\n"

		ERECT_PANO
		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		RECT_WARP_SPHERE
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		//"	if(cam_index == 0)\n"
		//"		v.y = (v.y)*2.0;\n"
		//"	else if(cam_index == 1)\n"
		//"		v.y = (v.y-0.5)*2.0;\n"


		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		"	v = erect_pano(v, dist);\n"
		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"	v = rect_warp_sphere(v, dist);\n"

		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"

		"	vec4 c0=vec4(0,0,0,0);\n"

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"	{\n"
		"		c0 =texture2D(tex, v.xy);\n"
		//"		if(opacity_mode == 1)\n"
		//"		c0.w = 0.3;\n"
		"	}\n"
		"	gl_FragColor = vec4(c0);\n"

		"}\n";
}

std::string RectlinearWarpProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform int opacity_mode;\n"
		"uniform int cam_index;\n"
		"uniform float yaw, pitch, roll;\n"

		ERECT_RECT
		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		RECT_WARP_SPHERE
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		//"	if(cam_index == 0)\n"
		//"		v.y = (v.y)*2.0;\n"
		//"	else if(cam_index == 1)\n"
		//"		v.y = (v.y-0.5)*2.0;\n"


		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		/*"	v = erect_rect(v, dist);\n"
		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"	v = rect_warp_sphere(v, dist);\n"*/

		"	v.x = v.x+yaw;\n"
		"	v.y = v.y+pitch;\n"

		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"

		"	vec4 c0=vec4(0,0,0,0);\n"

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"	{\n"
		"		c0 =texture2D(tex, v.xy);\n"
		//"		if(opacity_mode == 1)\n"
		//"		c0.w = 0.3;\n"
		"	}\n"
		"	gl_FragColor = vec4(c0);\n"

		"}\n";
}

std::string FullframeFisheyeCylindricalWarpProg::getFShader() const {
	return
		"uniform sampler2D tex;\n"
		"uniform float a, b, c, d;\n"
		"uniform mat3 m;\n"
		"uniform float rot0, rot1;\n"
		"uniform float scale0, scale1;\n"
		"uniform float dist;\n"
		"uniform float aspect_dw, aspect_sw;\n"
		"uniform int laplacian_mode;\n"
		"uniform int opacity_mode;\n"
		ERECT_CYLINDRICAL
		ROTATE_EQUIRECT
		SPHERICAL_IMAGE_TO_EQUIRECT
		PERSPECTIVE_CONTROL_SPHERICAL_IMAGE
		PERSPECTIVE_RECT_SPHERICAL
		CORRECT_LENSE_DIST_BIGHEADK

		"void main() {\n"
		"	vec2 v = gl_TexCoord[0].st;\n"

		"	float w2 = aspect_dw/2.0;\n"
		"	float h2 = 1.0/2.0;\n"
		"	float sw2 = aspect_sw/2.0;\n"
		"	float sh2 = 1.0/2.0;\n"

		"	float mid = sw2;\n"
		"	if(sh2 < sw2)\n"
		"		mid = sh2;\n"

		"	v.x = (v.x*aspect_dw)-w2;\n"
		"	v.y = v.y-h2;\n"

		"	v = erect_millercylindrical(v, dist);\n"
		"	v = rotate_eqrect(v, rot0, rot1);\n"
		"	v = sphere_tp_erect(v, dist);\n"
		"	v = persp_sphere(v, m, dist);\n"
		"	v = rect_sphere(v, dist);\n"
		"	v.x = v.x*scale0;\n"
		"	v.y = v.y*scale1;\n"
		"	v = radial(v, mid);\n"

		"   v.x = (v.x+sw2) / aspect_sw;"
		"   v.y = v.y+sh2;"

		"	vec4 c0=vec4(0,0,0,0);\n"

		"	if(v.x>=0. && v.x<=1. && v.y>=0. && v.y<=1.) \n"
		"	{\n"
		"		c0 =texture2D(tex, v.xy);\n"
		"		if(opacity_mode == 1)\n"
		"		c0.w = 0.3;\n"
		"	}\n"
		"	gl_FragColor = vec4(c0);\n"

		"}\n";
}