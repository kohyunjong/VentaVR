#pragma once
#include "afxwin.h"


// CAddOverlayChild2 ��ȭ �����Դϴ�.

class CAddOverlayChild2 : public CDialogEx
{
	DECLARE_DYNAMIC(CAddOverlayChild2)

public:
	CAddOverlayChild2(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CAddOverlayChild2();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ADDOVERLAY_CHILD2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	bool isEnableDone = false;

	char* m_StreamingOverlayUrl = NULL;

	CEdit m_editURL;
	afx_msg void OnEnChangeEcOverlayUrl();
};
