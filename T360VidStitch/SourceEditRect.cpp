// SourceEditRect.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "SourceEditRect.h"



// CSourceEditRect ��ȭ �����Դϴ�.

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


// CSourceEditRect �޽��� ó�����Դϴ�.
