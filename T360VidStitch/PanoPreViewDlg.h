#pragma once


// CPanoPreViewDlg ��ȭ �����Դϴ�.

class CPanoPreViewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPanoPreViewDlg)

public:
	CPanoPreViewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPanoPreViewDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PANORAMAPREVIEW_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CView* m_pPreView;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
