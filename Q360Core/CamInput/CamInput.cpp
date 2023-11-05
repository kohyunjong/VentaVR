
#include "CamInput.h"

CamInput* CamInput::create(PTSParam param, int input_width, int input_height, int depth, int input_type, int stitching_mode){
	CamInput* camIn = 0;
	//if (stitching_mode == STITCHED_INPUT)
//		camIn = new CamInputStitchedBuffer();
	//else
		camIn = new CamInputImageFile();

	if (camIn == 0) return 0;

	camIn->width = input_width;
	camIn->height = input_height;
	camIn->depth = depth;

	camIn->diff_tr[0] = 0.f;
	camIn->diff_tr[1] = 0.f;

	camIn->input_type = input_type;
	camIn->setPTSParam(param);

	return camIn;
}

void CamInput::update(PTSParam param, int input_width, int input_height, int depth, int input_type){
	width = input_width;
	height = input_height;
	depth = depth;

	input_type = input_type;

	setPTSParam(param);
}

CamInput* CamInput::create(PTSParam param, int input_width, int input_height, int input_type, int depth, void* ptr){
	CamInput* camIn = 0;
	camIn = new CamInputImage(input_width, input_height, depth, ptr);

	if (camIn == 0) return 0;

	camIn->input_type = input_type;

	camIn->diff_tr[0] = 0.f;
	camIn->diff_tr[1] = 0.f;

	camIn->setPTSParam(param);

	return camIn;
}

CamInput* CamInput::createNadir(PTSParam param, std::string fn, int input_width, int input_height, int input_type){
	CamInput* camIn = 0;
	camIn = new CamInputNadir(fn, input_width, input_height);

	if (camIn == 0) return 0;

	camIn->input_type = input_type;

	camIn->setPTSParam(param);

	return camIn;
}

void	CamInput::setMask(const void* buf, int w, int h) {
	if( maskBuf ) delete maskBuf; maskBuf=NULL;
	maskW=w; maskH=h;
	maskBuf = new unsigned char[maskW*maskH];
	memcpy( maskBuf, buf, maskW*maskH );
	if( glInited ) tex.setMask( maskBuf, maskW, maskH );
	maskSet = true;
}
void	CamInput::setImage(void* buf, int w, int h) {
	if (glInited) tex.setImage(buf, w, h);
}
void	CamInput::setImageRGBA(void* buf, int w, int h) {
	if (glInited) tex.setImageRGBA(buf, w, h);
}

void	CamInput::initGL() {
	tex.initGL(); glInited=true;
	if( maskSet && maskBuf && !tex.isMaskSet() )
		tex.setMask(maskBuf,maskW,maskH);
}

void CamInput::clear() {
	disconnect();
	tex.clear(); glInited=false;
	if( maskBuf ) delete maskBuf; maskBuf=NULL; maskW=maskH=0; maskSet=false;
	width = height = 0;
	format = UNDEF;
	tr[0]=tr[1]=tr[2]=tr[3]=tr[4]=tr[5]=0;
	fov = 60;
	dist[0]=dist[1]=dist[2]=dist[3]=0;
}

void	CamInputImageFile::setFilename( const std::string& fn ) {
	filename=fn;
	printf("Filename: %s\n", filename.c_str());
}
int		CamInputImageFile::connect() {
	//width = input_width, height = input_height; 
	dirty = true; // dirty for aspect ratio change	bigheadk
	printf("size: %d x %d, depth : %d \n", width, height, depth );	
	if( width>0 && height>0 ) {
		if (depth == 4)
			format = InFormat::RGBA8; 
		else 
			format = InFormat::RGB8;

		connected = true;

		if(!connected) return 0;
		if (!glInited) return 0;
		if (!pbo[0]) {
			connectPBO(width, height, depth);
			if (depth == 4)
				tex.setImageRGBA(NULL, width, height);
			else
				tex.setImage(NULL, width, height);
		}
		return format;
	}
	else return UNDEF;
}
int		CamInputImageFile::refresh(int pbo_idx) {
	if( !connected ) return 0;
	if( !glInited ) return 0;
	if(!pbo[0]) {
		connectPBO(width,height, depth);
		if (depth == 4)
			tex.setImageRGBA(NULL, width, height);
		else
			tex.setImage(NULL,width,height);
	}
//	tex.setImage( img.buf, img.w, img.h );
	//void* ptr = getAddress();
	//memcpy(ptr,img.buf,img.w*img.h*3);
	finishUpdate(tex.tex, width, height, depth, pbo_idx);
	return true;
}

int		CamInputImageFile::refresh(int pbo_idx, GLuint texid) {
	return true;
}

int		CamInputImageFile::disconnect() {
	width = 0;
	height = 0;
	connected = false;
	return true;
}

void* CamInputImageFile::getPBOAddress(int pbo_idx)
{
	if (!connected) return 0;
	if (!glInited) return 0;
	if (!pbo[0]) {
		return 0;
	}
	if (pbo_idx < 0)
	{
		outBuf = inBuf;
		inBuf = (inBuf + 1) % IN_BUFFER_CNT;
		pbo_idx = inBuf;
	}
	return ptr[pbo_idx];
}
// bigheadk
int		CamInputStitchedBuffer::connect() {
	//width = input_width, height = input_height; 
	dirty = true; // dirty for aspect ratio change	bigheadk
	printf("size: %d x %d, depth : %d \n", width, height, depth);
	if (width>0 && height>0) {
		if (depth == 4)
			format = InFormat::RGBA8;
		else
			format = InFormat::RGB8;

		connected = true;

		if (!connected) return 0;
		if (!glInited) return 0;
		if (!pbo[0]) {
			connectPBO(width, height, depth);
		}
		return format;
	}
	else return UNDEF;
}
int		CamInputStitchedBuffer::refresh(int pbo_idx)
{
	return true;
}
int		CamInputStitchedBuffer::refresh(int pbo_idx, GLuint texid) {
	printf("bigheadk, refresh StitchedBuffer000\n");
	if (!connected) return 0;
	if (!glInited) return 0;
	if (!pbo[0]) {
		connectPBO(width, height, depth);
	}
	printf("bigheadk, refresh StitchedBuffer\n");
	//	tex.setImage( img.buf, img.w, img.h );
	//void* ptr = getAddress();
	//memcpy(ptr,img.buf,img.w*img.h*3);
	finishUpdate(texid, width, height, depth, pbo_idx);
	return true;
}
int		CamInputStitchedBuffer::disconnect() {
	width = 0;
	height = 0;
	connected = false;
	return true;
}

void* CamInputStitchedBuffer::getPBOAddress(int pbo_idx)
{
	if (!connected) return 0;
	if (!glInited) return 0;
	if (!pbo[0]) {
		return 0;
	}
	if (pbo_idx < 0)
	{
		outBuf = inBuf;
		inBuf = (inBuf + 1) % IN_BUFFER_CNT;
		pbo_idx = inBuf;
	}
	return ptr[pbo_idx];
}
// bigheadk

int		CamInputNadir::connect() {
	return UNDEF;
}
int		CamInputNadir::refresh(int pbo_idx) {
	return true;
}
int		CamInputNadir::refresh(int pbo_idx, GLuint texid) {
	return true;
}
int		CamInputNadir::disconnect() {
	return true;
}

void* CamInputNadir::getPBOAddress(int pbo_idx)
{
	return 0;
}


int		CamInputImage::connect() {
	return UNDEF;
}
int		CamInputImage::refresh(int pbo_idx) {
	return true;
}
int		CamInputImage::refresh(int pbo_idx, GLuint texid) {
	return true;
}
int		CamInputImage::disconnect() {
	return true;
}

void* CamInputImage::getPBOAddress(int pbo_idx)
{
	return 0;
}

void CamInput::setState(int value)
{
	state = value;
}

int CamInput::getState()
{
	return state;
}

void	CamInput::setTransform(float trf[6]) {
	tr[0] = trf[0]; tr[1] = trf[1]; tr[2] = trf[2]; tr[3] = trf[3]; tr[4] = trf[4]; tr[5] = trf[5]; dirty = true;
}
void	CamInput::setTransform(float yaw, float pitch, float roll, float tx, float ty, float tz) {
	tr[0] = yaw; tr[1] = pitch; tr[2] = roll; tr[3] = tx; tr[4] = ty; tr[5] = tz;  dirty = true;
}
void	CamInput::setFov(float FOV) {
	fov = FOV;  dirty = true; 
}
void	CamInput::setDistParam(float distParam[4]) {
	dist[0] = distParam[0]; dist[1] = distParam[1]; dist[2] = distParam[2]; dist[3] = distParam[3];  dirty = true;
}
void	CamInput::setDistParam(float a, float b, float c, float d) {
	dist[0] = a; dist[1] = b; dist[2] = c; dist[3] = d; dirty = true;
}
void	CamInput::setPTSParam(PTSParam& param) {
	tr[0] = param.y;
	tr[1] = param.p;
	tr[2] = param.r;

	diff_tr[0] = param.diff_yaw;
	diff_tr[1] = param.diff_pitch;

	fov = param.v;

	dist[0] = param.a;
	dist[1] = param.b;
	dist[2] = param.c;
	dist[3] = 1 - (dist[0] + dist[1] + dist[2]);

	width = param.w;
	height = param.h;

	crop_left = param.left;
	crop_right = param.right;
	crop_top = param.top;
	crop_bottom = param.bottom;

	lens_type = param.f;
	output_type = param.out_f;
	output_fov = param.out_fov;

	dirty = true;
}
