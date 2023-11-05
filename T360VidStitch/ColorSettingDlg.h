#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CColorSettingDlg 대화 상자입니다.

class CColorSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CColorSettingDlg)

public:
	CColorSettingDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CColorSettingDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_COLORSET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CSliderCtrl m_sliderRed;
	CSliderCtrl m_sliderGreen;
	CSliderCtrl m_sliderBlue;
	
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	CString m_szRedValue;
	CString m_szGreenValue;
	CString m_szBlueValue;
	int m_nActiveIdx;

	afx_msg void OnBnClickedBtnColorSave();
	CGdipButton m_btnSave;
	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void LoadButtonImage(BOOL bShow);
	CGdipButtonExt m_btnThumb1;
	CGdipButtonExt m_btnThumb2;
	CGdipButtonExt m_btnThumb3;
	CGdipButtonExt m_btnThumb4;
	CGdipButtonExt m_btnThumb5;
	CGdipButtonExt m_btnThumb6;
	CGdipButtonExt m_btnThumb7;
	CGdipButtonExt m_btnThumb8;
	afx_msg void OnClose();
	void ClickButtons(int i);
	void SetControlValue();
	afx_msg void OnBnClickedButtonImage1();
	afx_msg void OnBnClickedButtonImage2();
	afx_msg void OnBnClickedButtonImage3();
	afx_msg void OnBnClickedButtonImage4();
	afx_msg void OnBnClickedButtonImage5();
	afx_msg void OnBnClickedButtonImage6();
	afx_msg void OnBnClickedButtonImage7();
	afx_msg void OnBnClickedButtonImage8();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
