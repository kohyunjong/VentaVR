#pragma once
#include "./Src/ImgDialogBase.h"

// CSourceEditRect 대화 상자입니다.

class CSourceEditRect : public CImgDialogBase
{
	DECLARE_DYNAMIC(CSourceEditRect)

public:
	CSourceEditRect(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSourceEditRect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SOURCEEDITRECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
