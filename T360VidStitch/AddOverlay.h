#pragma once
#include "afxwin.h"
#include "AddOverlayPreviewDlg.h"

//#define NUM_OVERLAY_TYPE	3
#define NUM_TOTAL_OVERLAY_MAX_COUNT 20
#define NUM_IMAGE_OVERLAY_MAX_COUNT 10
#define NUM_TEXT_OVERLAY_MAX_COUNT 10
//#define NUM_VIDEO_OVERLAY_MAX_COUNT 2
#define NUM_FFMPEG_OVERLAY_MAX_COUNT 6
//#define NUM_CAMERA_OVERLAY_MAX_COUNT 1

#define USE_CAMERAOVERLAY_CROP 1

// AddOverlay 대화 상자입니다.
class AddOverlay : public CDialogEx
{
	DECLARE_DYNAMIC(AddOverlay)

public:
	AddOverlay(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~AddOverlay();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDOVERLAYDLG };

	enum OverlayType
	{
		Overlay_None,
		Overlay_Image,
		Overlay_Video,
		Overlay_Streaming,
		Overlay_Camera,
		Overlay_Text,
		Overlay_Gif
	};

	OverlayType selectedType = Overlay_None;
	//char*		overlaypath;
	char* m_ImageOverlayPath = NULL;
	char* m_GifOverlayPath = NULL;
	char* m_VedioOverlayPath = NULL;
	char* m_StreamingOverlayUrl = NULL;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnDone;
	CGdipButton m_btnBrowse;	
	AddOverlayPreviewDlg* m_pdlgAddOverlayPreview;

	CEdit m_editTextInput;
	CString m_PreText;
	CString m_CulText;
	CString m_Font = _T("Arial");
	FontStyle m_FontStyle = FontStyleRegular;
	int fontstyle = 0;
	COLORREF txtColor = (0x323232), bgColor = (0x00000000);
	RectF m_TextRectSize;
	CStatic m_TextColorPicker;
	int TextColor_R = 50, TextColor_G = 50, TextColor_B = 50;
	CStatic m_TextBgColorPicker;
	CStatic m_TextBgTrans;
	int TextBgColor_R = 50, TextBgColor_G = 50, TextBgColor_B = 50, TextBgColor_A = 0;
	CEdit m_editLeft;
	CEdit m_editTop;
	CEdit m_editRight;
	CEdit m_editBottom;

	void DrawTextBitmap(Bitmap* bt);
	void GetTextRect(Graphics* graphics, CString text, Gdiplus::Font* font);
	int BitmapToByteBuffer(Bitmap* bmp);
	void SetFontStyle(int nType);
	void SetTextOverlayInfo(int id);
	void GetTextOverlayInfo(int id);
	void SetAddOverlayDlgInfo();
	int SetFont(CString fontName);
	int GetEncoderClsid(const WCHAR *format, CLSID *pClsid);

	afx_msg void OnCbnSelchangeOverlayType();
	afx_msg void OnBnClickedBtnBrowse();
	afx_msg void OnBnClickedBtnDone();

	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnEnChangeEcOverlayText();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnCbnSelchangeCboFont();
	afx_msg void OnStnClickedCrTextcolor();
	afx_msg void OnCbnSelchangeCboFontstyle();
	afx_msg void OnClose();
	afx_msg void OnStnClickedCrTextbgcolor();
	afx_msg void OnStnClickedCrTextbgtrans();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnCbnSelchangeCboOverlayCam();
	afx_msg void OnEnChangeEcOverlayUrl();
	afx_msg void OnBnClickedCheckCrop();
	CStatic m_szLeft;
	CStatic m_szTop;
	CStatic m_szRight;
	CStatic m_szBottom;
};
