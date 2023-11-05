#pragma once
#include "afxwin.h"


// RightPanoraChildDlg 대화 상자입니다.
enum WriteOutputLogType {
	HDD_START,
	HDD_END,
	STREAMING_START,
	STREAMING_END
};

class CRightPanoraChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightPanoraChildDlg)

public:
	CRightPanoraChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRightPanoraChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHTPANORACHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedBtncalibration();
	afx_msg void OnBnClickedBtnoutput();

	CGdipButton m_btnCailbration;
	CGdipButton m_btnHdd;
	CGdipButton m_btnStream;
	CGdipButton m_btnOutput;
	CGdipButton m_btnHdd2;
	CGdipButton m_btnStream2;
	CGdipButton m_btnOutput2;
	//CGdipButton m_btnEditDisable, m_btnEditOff, m_btnEditOn;
	CGdipButton m_btnStraighten;
	CGdipButton m_btnPanoSave;
	CGdipButton m_btnOverlay;
	CGdipButton m_btnVOD;
	CGdipButton m_btnVOD2;
	CGdipButton m_btnStereo;

	int		m_nFlgClibration;
	int		m_nFlgHddMode;
	int		m_nFlgStreamMode;
	int		m_nFlgVODStreamMode;
	int		m_nFlgOutputMode;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	char* vod_filepath = "";
	char* vod_url = "";
	bool vod_isRepeat = false;

	BOOL	m_bFirstUpdate;
	void	CheckClosetoRecodeing();
	//afx_msg void OnBnClickedBtnediton();
	//afx_msg void OnBnClickedBtneditoff();
	//afx_msg void OnBnClickedBtnstraighten();
	afx_msg void OnBnClickedBtnpanosave();
	afx_msg void OnBnClickedBtnstraighten2();
	afx_msg void OnBnClickedBtnpanorama();
	afx_msg void OnBnClickedBtnvod();

	void writeLog(int type);
};
