#pragma once
#include "afxwin.h"


// CAddOverlayChild1 대화 상자입니다.

class CAddOverlayChild1 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayChild1)

public:
	CAddOverlayChild1(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddOverlayChild1();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDOVERLAY_CHILD1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	bool isEnableDone = false;

	CGdipButton m_btnBrowse;
	CStatic m_staticPath;

	char* m_ImageOverlayPath = NULL;
	char* m_GifOverlayPath = NULL;
	char* m_VedioOverlayPath = NULL;
	void setPath(int type);

	afx_msg void OnBnClickedBtnOverlayBrowse();
};
