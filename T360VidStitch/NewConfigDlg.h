#pragma once
#include "afxwin.h"


// CNewConfigDlg 대화 상자입니다.

class CNewConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CNewConfigDlg)

public:
	CNewConfigDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CNewConfigDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_NEWCONFIG_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	void OnDestroy();
	afx_msg void OnBnClickedBtnbrowose();
	afx_msg void OnBnClickedOk();
	CComboBox m_cboInputMoudleType;
	CComboBox m_cboNumofCam;
	CComboBox m_cboInputSize;
	CComboBox m_cboDstSize;
	CEdit m_txtNumOfCam;
	CEdit m_txtSourceWidth;
	CEdit m_txtSourceHeight;
	CEdit m_txtPrjBasePath;
	CEdit m_txtPrjName;
	CGdipButton m_btnBrower;
	CGdipButton m_btnOK;
//	CButton m_btnOK;

	const static int DST_1K_WIDTH = 1024, DST_1K_HEIGHT = 512;
	const static int DST_2K_WIDTH = 1920, DST_2K_HEIGHT = 960;
	const static int DST_4K_WIDTH = 3840, DST_4K_HEIGHT = 1920;
	const static int DST_8K_WIDTH = 7680, DST_8K_HEIGHT = 3840;
	const static int DST_STEREO_4K_WIDTH = 3840, DST_STEREO_4K_HEIGHT = 1920;
	const static int DST_STEREO_SIDE_WIDTH = 2048, DST_STEREO_SIDE_HEIGHT = 2048;

	afx_msg void OnEnUpdateEdtprjname();
	CString m_szPrjName;
	bool m_bPrjNameOverlap = false;
	void SetFontObj();
	afx_msg void OnCbnSelchangeComboDstsize();
};
