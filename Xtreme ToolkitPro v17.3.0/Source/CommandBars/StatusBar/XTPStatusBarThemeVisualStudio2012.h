// XTPStatusBarThemeVisualStudio2012.h
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
#if !defined(__XTPSTATUSBARTHEMEVISUALSTUDIO2012_H__)
#define __XTPSTATUSBARTHEMEVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class _XTP_EXT_CLASS CXTPStatusBarThemeVisualStudio2012 : public CXTPStatusBarPaintManager
{
public:
	CXTPStatusBarThemeVisualStudio2012(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill a status bar.
	// Parameters:
	//     pDC  - Pointer to a valid device context
	//     pBar - Points to a CXTPStatusBar object
	//-----------------------------------------------------------------------
	virtual void FillStatusBar(CDC *pDC, CXTPStatusBar *pBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw frame of single cell of status bar.
	// Parameters:
	//     pDC - Points to a valid device context.
	//     rc - CRect object specifying size of area.
	//     pPane - The status bar pane need to draw
	//     bGripperPane - TRUE if pane is last cell of status bar
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarPaneBorder(CDC *pDC, CRect rc, CXTPStatusBarPane *pPane, BOOL bGripperPane);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw the status bar's gripper.
	// Parameters:
	//     pDC      - Pointer to a valid device context
	//     rcClient - Client rectangle of the status bar.
	//-----------------------------------------------------------------------
	virtual void DrawStatusBarGripper(CDC *pDC, CRect rcClient);

	//-------------------------------------------------------------------------
	// Summary:
	//     Handles updates to paint manager colors and metrics.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#endif // !defined(__XTPSTATUSBARTHEMEVISUALSTUDIO2012_H__)