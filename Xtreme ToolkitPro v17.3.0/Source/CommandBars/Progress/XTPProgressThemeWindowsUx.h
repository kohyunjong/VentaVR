// XTPProgressThemeWindowsUx.h
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
#if !defined(__XTPPROGRESSTHEMEWINDOWSUX_H__)
#define __XTPPROGRESSTHEMEWINDOWSUX_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPProgressThemeWindowsUx : public CXTPProgressPaintManager
{
public:
	CXTPProgressThemeWindowsUx(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPProgressThemeWindowsUx object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPProgressThemeWindowsUx();

public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Recalculates metrics
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	virtual void DrawProgress(CDC *pDC, CXTPProgressBase *pGallery);

protected:
	CXTPWinThemeWrapper *m_themeProgress; // Theme wrapper
};

#endif // !defined(__XTPPROGRESSTHEMEWINDOWSUX_H__)