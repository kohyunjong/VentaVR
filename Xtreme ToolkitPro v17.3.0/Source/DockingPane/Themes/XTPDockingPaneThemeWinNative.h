// XTPDockingPaneThemeWinNative.h
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
#if !defined(__XTPDOCKINGPANETHEMEWINNATIVE_H__)
#define __XTPDOCKINGPANETHEMEWINNATIVE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPDockingPaneWinNativeTheme is CXTPDockingPaneVisualStudio2003Theme derived class, represents
//     Win XP theme for docking panes.
// Remarks:
//     Call CXTPDockingPaneManager::SetTheme(xtpPaneThemeWinNative); to set this theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneWinNativeTheme : public CXTPDockingPaneVisualStudio2003Theme
{
public:
	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPDockingPaneWinNativeTheme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneWinNativeTheme();

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the pane.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw a caption.
	// Parameters:
	//     pDC - Reference to the device context in which to draw.
	//     pPane - Pointer to the tabbed container.
	//     rc - Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	virtual void DrawCaption(CDC* pDC, CXTPDockingPaneTabbedContainer* pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to draw frame of floating window
	// Parameters:
	//     pDC    - Pointer to a valid device context
	//     pPane - Floating window pointer
	//     rc    - CLient rectangle of floating frame
	//-----------------------------------------------------------------------
	void DrawFloatingFrame(CDC* pDC, CXTPDockingPaneMiniWnd* pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:
	//     Override this member function to draw the side panel.
	// Parameters:
	//     pDC    - Reference to the device context in which to draw.
	//     pPane - Pointer to the side panel.
	//     rc    - Client rectangle of the side panel.
	//-----------------------------------------------------------------------
	void DrawSidePanel(CDC* pDC, CXTPDockingPaneSidePanel* pPane, CRect rc);

protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Draw common part of tabbed and floating caption.
	// Parameters:
	//     pDC        - Reference to the device context in which to draw.
	//     pPane     - Container which caption need to draw.
	//     rcCaption - Caption bounding rectangle
	//     strTitle  - Caption text.
	//     bActive   - TRUE if caption is active.
	//     bVertical - TRUE to draw caption vertically
	//-----------------------------------------------------------------------
	virtual void DrawCaptionPart(CDC* pDC, CXTPDockingPaneBase* pPane, CRect rcCaption, const CString& strTitle, BOOL bActive, BOOL bVertical);

protected:
};

#endif // !defined(__XTPDOCKINGPANETHEMEWINNATIVE_H__)