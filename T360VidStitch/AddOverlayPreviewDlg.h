#pragma once


// AddOverlayPreviewDlg 대화 상자입니다.

class AddOverlayPreviewDlg : public CDialogEx
{
	DECLARE_DYNAMIC(AddOverlayPreviewDlg)

public:
	AddOverlayPreviewDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AddOverlayPreviewDlg();

// 대화 상자 데이터입니다.
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
