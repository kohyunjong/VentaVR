#pragma once
#include "afxcmn.h"
#include "afxwin.h"

// CRightInteracChildDlg ��ȭ �����Դϴ�.

class CRightInteracChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRightInteracChildDlg)

public:
	CRightInteracChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CRightInteracChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RIGHTINTERACCHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
