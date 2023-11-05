#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CRightInteracChildDlg 대화 상자입니다.

class CRightInteracChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightInteracChildDlg)

public:
	CRightInteracChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CRightInteracChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_RIGHTINTERACCHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	CGdipButton m_btnPlay;
	CSliderCtrl m_sldSpeed;
	CStatic m_szSpeed;
	CEdit m_txtSpeed;
	afx_msg void OnBnClickedBtnplay();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void PlayState(int mode);
	float PlaySpeed(int mode, float speed);
	float m_speed;
};
