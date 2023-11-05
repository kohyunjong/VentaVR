#pragma once


// CPanoPreViewDlg 대화 상자입니다.

class CPanoPreViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPanoPreViewDlg)

public:
	CPanoPreViewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPanoPreViewDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PANORAMAPREVIEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CView* m_pPreView;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
