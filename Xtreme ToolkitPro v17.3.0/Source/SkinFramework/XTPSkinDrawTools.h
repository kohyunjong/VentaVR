// XTPSkinDrawTools.h
//
// (c)1998-2016 Codejock Software, All Rights Reserved.
//
// THIS SOURCE FILE IS THE PROPERTY OF CODEJOCK SOFTWARE AND IS NOT TO BE
// RE-DISTRIBUTED BY ANY MEANS WHATSOEVER WITHOUT THE EXPRESSED WRITTEN
// CONSENT OF CODEJOCK SOFTWARE.
//
// THIS SOURCE CODE CAN ONLY BE USED UNDER THE TERMS AND CONDITIONS OUTLINED
// IN THE XTREME TOOLKIT PRO LICENSE AGREEMENT. CODEJOCK SOFTWARE GRANTS TO
// YOU (ONE SOFTWARE DEVELOPER) THE LIMITED RIGHT TO USE THIS SOFTWARE ON A
// SINGLE COMPUTER.
//
// CONTACT INFORMATION:
// support@codejock.com
// http://www.codejock.com
//
/////////////////////////////////////////////////////////////////////////////

//{{AFX_CODEJOCK_PRIVATE
#if !defined(__XTPSKINDRAWTOOLS_H__)
#define __XTPSKINDRAWTOOLS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//{{AFX_CODEJOCK_PRIVATE

// Private function

_XTP_EXT_CLASS BOOL WINAPI XTRSkinFrameworkDrawEdge(HDC hdc, LPRECT lprc, UINT edge, UINT flags);
_XTP_EXT_CLASS BOOL WINAPI XTRSkinFrameworkDrawFrameControl(HDC hdc, LPRECT lprc,  UINT uType,  UINT uState);
_XTP_EXT_CLASS int AFX_CDECL XTPGetParentDCClipBox(HWND   pwnd, HDC    hdc, LPRECT lprc);
_XTP_EXT_CLASS void AFX_CDECL XTPSkinFrameworkGetIconSize(HICON hImage, int* pcx, int* pcy);
_XTP_EXT_CLASS HBITMAP AFX_CDECL XTPSkinFrameworkLoadBitmap(HMODULE hModule, LPCTSTR lpszResourceName, BOOL& bAlpha);
_XTP_EXT_CLASS void AFX_CDECL XTPSkinFrameworkDrawFrame(HDC hdc, LPRECT lprc, int nSize, COLORREF clr);
_XTP_EXT_CLASS void AFX_CDECL XTPSkinFrameworkDrawFrame(HDC hdc, LPRECT lprc, int nSize, HBRUSH hBrush);

//}}AFX_CODEJOCK_PRIVATE

#endif // !defined(__XTPSKINDRAWTOOLS_H__)
