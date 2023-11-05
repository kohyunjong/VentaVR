#include "stdafx.h"
#include "ProjAllConfig.h"


CProjAllConfig::CProjAllConfig()
{
	m_bInitQVS_EngineDone = FALSE;
	m_szProjectFullPath = _T("");
	m_ModuleType = QVS_Engine::InputModuleType_Magewell;
	m_bLoadConfig = FALSE;

	m_bFirstUpdateBtmPano = FALSE;
	m_bFirstUpdateBtmSource = FALSE;
//		PLM #17129 :
//	m_szPTSFullPath = _T("./0712.pts");
	m_nNeedSave = 0;
	m_restart = false;

	m_newType = NEWTYPE::None;
	m_cboNumOfCam = 0;
	m_cboInputSize = 0;
	m_cboOutputSize = 0;
}


CProjAllConfig::~CProjAllConfig()
{
}

void CProjAllConfig::SaveConfigValue()
{
	int rtn = QVS_Engine::QVS_SetOutputConfiguration(&m_Configuration);
	if(rtn == QVS_FAIL)
	{
		TRACE(_T("Save Failed Config... Check your streaming address\n"));
	}
#ifdef _DEBUG
	if (rtn == QVS_SUCCESS)
		TRACE(_T("Save Success Config.\n"));
#endif
}