#pragma once
#include "afx.h"
#include "Quram360Engine.h"

class CProjAllConfig :public CObject
{
public:
	CProjAllConfig();
	virtual ~CProjAllConfig();

	enum NEWTYPE{
		None = 0,
		Q360_4K = 1,
		Q360_8K = 2,
		Stereo_TB = 3,
		Stereo_LR = 4,
		Extern_Input = 5,
		Wide_Panorama = 6
	};
private:
	int								m_newType;
	int								m_cboNumOfCam;
	int								m_cboInputSize;
	int								m_cboOutputSize;
public:

	BOOL							m_bLoadConfig;
	BOOL							m_bInitQVS_EngineDone;
//	CString							m_szPTSFullPath;	//		PLM #17129 :
	CString							m_szProjectFullPath;
	CString							m_szProjectName;
	int								m_nScrWidth;
	int								m_nScrHeight;
	int								m_nWidth;
	int								m_nHeight;
	int								m_nDstWidth;
	int								m_nDstHeight;
	float							m_nFov;
	int								m_nMaxCamra;

	int								m_nNeedSave;
	int								m_nStitchingMode;

	QVS_Engine::_InputModuleType	m_ModuleType;
	QVS_Engine::Configuration		m_Configuration;

	BOOL							m_bFirstUpdateBtmSource;
	BOOL							m_bFirstUpdateBtmPano;

	BOOL							m_restart;

	BOOL isQVSEngineInit()	{ return m_bInitQVS_EngineDone; }
	BOOL isReadQVSConfig()	{ return m_bLoadConfig; }
	void SaveConfigValue();
	int isNeedSave()	{ return m_nNeedSave; }

	void setNewType(int type) { m_newType = type; }
	int getNewType() { return m_newType; }
	void setCboNumOfCam(int type) { m_cboNumOfCam = type; }
	int getCboNumOfCam() { return m_cboNumOfCam; }
	void setCboInputSize(int type) { m_cboInputSize = type; }
	int getCboInputSize() { return m_cboInputSize; }
	void setCboOutputSize(int type) { m_cboOutputSize = type; }
	int getCboOutputSize() { return m_cboOutputSize; }
};

