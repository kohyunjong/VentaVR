// SourceEditRect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "SourceEditRect.h"



// CSourceEditRect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSourceEditRect, CImgDialogBase)

CSourceEditRect::CSourceEditRect(CWnd* pParent /*=NULL*/)
: CImgDialogBase(CTransparentDlgType::IDD
, IDB_PNG_SOURCE_DRAG
, _T("PNG")
, AfxGetResourceHandle()
, pParent)
{

}

CSourceEditRect::~CSourceEditRect()
{
}

void CSourceEditRect::DoDataExchange(CDataExchange* pDX)
{
	CImgDialogBase::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSourceEditRect, CImgDialogBase)
END_MESSAGE_MAP()


// CSourceEditRect 메시지 처리기입니다.
