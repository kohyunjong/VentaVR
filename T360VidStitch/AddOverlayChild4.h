#pragma once
#include "afxwin.h"
#include "AddOverlayPreviewDlg.h"


// CAddOverlayChild4 대화 상자입니다.

class CAddOverlayChild4 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayChild4)

public:
	CAddOverlayChild4(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddOverlayChild4();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDOVERLAY_CHILD4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	AddOverlayPreviewDlg* m_pdlgAddOverlayPreview;
	bool isEnableDone = false;

	afx_msg void OnEnChangeEcOverlayText();
	afx_msg void OnCbnSelchangeCboFont();
	afx_msg void OnCbnSelchangeCboFontstyle();
	afx_msg void OnStnClickedCrTextcolor();
	afx_msg void OnStnClickedCrTextbgcolor();
	afx_msg void OnStnClickedCrTextbgtrans();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnDeleteItem(int nIDCtl, LPDELETEITEMSTRUCT lpDeleteItemStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void DrawTextBitmap(Bitmap* bt);
	void GetTextRect(Graphics* graphics, CString text, Gdiplus::Font* font);
	void SetFontStyle(int nType);
	int BitmapToByteBuffer(Bitmap* bmp);
	void SetTextOverlayInfo(int id);
	void GetTextOverlayInfo(int id);
	void SetAddOverlayDlgInfo();
	int SetFont(CString fontName);
	
	CEdit m_editTextInput;
	CString m_PreText;
	CString m_CulText;

	CComboBox m_cboFont;
	CString m_Font = _T("Arial");

	CComboBox m_cboFontStyle;
	FontStyle m_FontStyle = FontStyleRegular;
	int fontstyle = 0;

	CStatic m_TextColorPicker;
	COLORREF txtColor = (0x323232), bgColor = (0x00000000);
	RectF m_TextRectSize;

	CStatic m_TextBgColorPicker;
	CStatic m_TextBgTrans;
	int TextColor_R = 50, TextColor_G = 50, TextColor_B = 50;
	int TextBgColor_R = 50, TextBgColor_G = 50, TextBgColor_B = 50, TextBgColor_A = 0;
};
