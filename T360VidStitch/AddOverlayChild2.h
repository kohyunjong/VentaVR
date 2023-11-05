#pragma once
#include "afxwin.h"


// CAddOverlayChild2 대화 상자입니다.

class CAddOverlayChild2 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayChild2)

public:
	CAddOverlayChild2(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAddOverlayChild2();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ADDOVERLAY_CHILD2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	bool isEnableDone = false;

	char* m_StreamingOverlayUrl = NULL;

	CEdit m_editURL;
	afx_msg void OnEnChangeEcOverlayUrl();
};
