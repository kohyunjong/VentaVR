#pragma once
#include "./Src/ImgDialogBase.h"

// CSourceEditRect ��ȭ �����Դϴ�.

class CSourceEditRect : public CImgDialogBase
{
	DECLARE_DYNAMIC(CSourceEditRect)

public:
	CSourceEditRect(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CSourceEditRect();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_SOURCEEDITRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
