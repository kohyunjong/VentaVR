// XTPFrameThemeOffice2013.h : interface for the CXTPFrameThemeOffice2013 class.
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
#if !defined(__XTPFRAMETHEMEOFFICE2013_H__)
#define __XTPFRAMETHEMEOFFICE2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPPaintManager;
class CXTPCommandBarsFrameHook;

class _XTP_EXT_CLASS CXTPFrameThemeOffice2013 : public CXTPFrameThemeModernUI
{
public:

	CXTPFrameThemeOffice2013(CXTPPaintManager *pPaintManager);

	virtual ~CXTPFrameThemeOffice2013();

	virtual CSize GetCaptionButtonSize(UINT nId, HWND hwnd) const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame caption bar.
	// Parameters:
	//     pDC - Pointer to device context
	//     rcCaptionBar - Caption bar rectangle
	//     pSite - Frame to draw
	//     bActive - TRUE if frame is active
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBar(CDC *pDC, CRect rcCaptionBar, CWnd *pSite, BOOL bActive);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the manager.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	virtual LPCTSTR GetStyleResourceName() const;

	virtual CRect CalcFrameBorder(const CXTPCommandBarsFrameHook *pFrameHook) const;

	virtual CXTPFrameShadow* CreateShadow(CWnd* pFrame);

	CXTPPaintManagerColor m_clrFrameCaptionBorder;
	CSize m_szMDIClildWindowCaptionButtonSize;
};

#endif // !defined(__XTPFRAMETHEMEOFFICE2013_H__)
