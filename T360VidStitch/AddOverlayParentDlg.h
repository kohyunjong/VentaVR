#pragma once
#include "afxcmn.h"

#include "AddOverlayChild1.h"
#include "AddOverlayChild2.h"
#include "AddOverlayChild3.h"
#include "AddOverlayChild4.h"
#include "afxwin.h"
// CAddOverlayParentDlg 대화 상자입니다.

//#define NUM_OVERLAY_TYPE	3
#define NUM_TOTAL_OVERLAY_MAX_COUNT 20
#define NUM_IMAGE_OVERLAY_MAX_COUNT 10
#define NUM_TEXT_OVERLAY_MAX_COUNT 10
//#define NUM_VIDEO_OVERLAY_MAX_COUNT 2
#define NUM_FFMPEG_OVERLAY_MAX_COUNT 6
//#define NUM_CAMERA_OVERLAY_MAX_COUNT 1

class CAddOverlayParentDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayParentDlg)

public:
	CAddOverlayParentDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddOverlayParentDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDOVERLAY_PARENTDLG };

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
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	void GetTextOverlayInfo(int id);

	CAddOverlayChild1* m_pdlgAddOverlayChild1;
	CAddOverlayChild2* m_pdlgAddOverlayChild2;
	CAddOverlayChild3* m_pdlgAddOverlayChild3;
	CAddOverlayChild4* m_pdlgAddOverlayChild4;
	
	CGdipButton m_btnImage;
	CGdipButton m_btnVideo;
	CGdipButton m_btnStreaming;
	CGdipButton m_btnCamera;
	CGdipButton m_btnText;
	CGdipButton m_btnGif;
	CGdipButton m_btnDone;

	void setDoneState(bool state);

	void setOverlayType(int type);
	afx_msg void OnBnClickedBtnOverlayImage();
	afx_msg void OnBnClickedBtnOverlayVideo();
	afx_msg void OnBnClickedBtnOverlayStreaming();
	afx_msg void OnBnClickedBtnOverlayCamera();
	afx_msg void OnBnClickedBtnOverlayText();
	afx_msg void OnBnClickedBtnOverlayGif();
	afx_msg void OnBnClickedBtnDone();
};
