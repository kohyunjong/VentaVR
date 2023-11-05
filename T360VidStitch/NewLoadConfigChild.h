#pragma once

//#include "NewLoadConfigChild_0.h"
#include "NewLoadConfigChild_1.h"
#include "NewLoadConfigChild_2.h"
#include "NewLoadConfigChild_3.h"
#include "NewLoadConfigChild_4.h"
#include "NewLoadConfigChild_5.h"
#include "NewLoadConfigChild_6.h"
#include "NewLoadConfigChild_7.h"
// CNewLoadConfigChild 대화 상자입니다.

class CNewLoadConfigChild : public CDialogEx
{
	DECLARE_DYNAMIC(CNewLoadConfigChild)

public:
	CNewLoadConfigChild(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewLoadConfigChild();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWLOAD_CONFIG_CHILD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	CXTPTaskPanel m_wndTaskPanel;

	//CNewLoadConfigChild_0 m_pNewLoadConfigChild_0;
	CNewLoadConfigChild_1 m_pNewLoadConfigChild_1;
	CNewLoadConfigChild_2 m_pNewLoadConfigChild_2;
	CNewLoadConfigChild_3 m_pNewLoadConfigChild_3;
	CNewLoadConfigChild_4 m_pNewLoadConfigChild_4;
	CNewLoadConfigChild_5 m_pNewLoadConfigChild_5;
	CNewLoadConfigChild_6 m_pNewLoadConfigChild_6;
	CNewLoadConfigChild_7 m_pNewLoadConfigChild_7;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnTaskPanelNotify(WPARAM wParam, LPARAM lParam);

	bool changeGroupUI(int id);
	void resetUI();
	void setConfigNumOfCam(int type);
	void setConfigInputSize(int type);
	void getInputSize(int type, int &width, int &height);
	void setConfigOutputSize(int type);
	void getOutputSize(int type, int &width, int &height);

	const static int DST_1K_WIDTH = 1024, DST_1K_HEIGHT = 512;
	const static int DST_2K_WIDTH = 1920, DST_2K_HEIGHT = 960;
	const static int DST_4K_WIDTH = 3840, DST_4K_HEIGHT = 1920;
	const static int DST_8K_WIDTH = 7680, DST_8K_HEIGHT = 3840;
	const static int DST_STEREO_4K_WIDTH = 3840, DST_STEREO_4K_HEIGHT = 1920;
	const static int DST_STEREO_SIDE_WIDTH = 2048, DST_STEREO_SIDE_HEIGHT = 2048;
};
