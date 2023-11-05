#pragma once


// AddOverlayPreviewDlg ��ȭ �����Դϴ�.

class AddOverlayPreviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddOverlayPreviewDlg)

public:
	AddOverlayPreviewDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~AddOverlayPreviewDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_OVERLAYPREVIEWDLG };

	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CachedBitmap *cBitmap = NULL;

	CString m_TextPreview;
	CString m_FontPreview;
	FontStyle m_FontStylePreview;
	Color m_TextColorPreview;
	Color m_TextBgColorPreview;

	int dlgWidth = 385, dlgHeight = 170;

	void UpdatePreviewScreen();
	bool OnShowFirst = false;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
