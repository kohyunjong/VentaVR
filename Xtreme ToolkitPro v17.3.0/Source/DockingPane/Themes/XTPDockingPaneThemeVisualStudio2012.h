// XTPDockingPaneThemeVisualStudio2012.h
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
#if !defined(__XTPDOCKINGPANETHEMEVISUALSTUDIO2012_H__)
#define __XTPDOCKINGPANETHEMEVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPResourceImages;

//===========================================================================
// Summary: CXTPDockingPaneVisualStudio2012Theme is used to define drawing routines
//          specific to a Visual Studio 2012 docking pane theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPDockingPaneVisualStudio2012Theme : public CXTPDockingPaneDefaultTheme
{
public:
	//-------------------------------------------------------------------------
	// Summary: Constructs a CXTPDockingPaneVisualStudio2012Theme object.
	//-------------------------------------------------------------------------
	CXTPDockingPaneVisualStudio2012Theme();

	//-------------------------------------------------------------------------
	// Summary: Destroys a CXTPDockingPaneVisualStudio2012Theme object, handles 
	//          cleanup and deallocation.
	//-------------------------------------------------------------------------
	virtual ~CXTPDockingPaneVisualStudio2012Theme();

	//-----------------------------------------------------------------------
    // Summary: This member function is used to initialize default colors 
	//          for a theme from an INI resource.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:    Call this method to get pane's caption rectangle.
	// Parameters: pPane - Side Panel container to get caption CRECT.
	// Returns:    Bounding rectangle of the side panel's caption.
	//-----------------------------------------------------------------------
	virtual CRect GetPaneCaptionRect(const CXTPDockingPaneSidePanel *pPane);

	//-----------------------------------------------------------------------
	// Summary:    verride this member to change the client rectangle of the 
	//             child docking pane.
	// Parameters: pPane  : Pointer to the tabbed container or side panel.
	//             rc     : Client rectangle to be changed.
	//             bApply : TRUE to update tabs inside client area.
	//-----------------------------------------------------------------------
	virtual void AdjustClientRect(CXTPDockingPaneTabbedContainer* pPane, CRect& rc, BOOL bApply);

	//-----------------------------------------------------------------------
	// Summary:    Override this member to change the caption rectangle of the 
	//             child docking pane.
	// Parameters: pPane : Pointer to a CXTPDockingPaneTabbedContainer object 
	//                     or CXTPDockingPaneSidePanel object.
	//             rc    : Caption rectangle to be changed.
	//-----------------------------------------------------------------------
	virtual void AdjustCaptionRect(const CXTPDockingPaneTabbedContainer* pPane, CRect& rc);

	//-----------------------------------------------------------------------
	// Summary:    Override this method to draw splitter.
	// Parameters: pDC       : Pointer to a valid device context
	//             pSplitter : Pointer to splitter window
	//-----------------------------------------------------------------------
	virtual void DrawSplitter(CDC *pDC, CXTPDockingPaneSplitterWnd *pSplitter);

	//-----------------------------------------------------------------------
	// Summary:    Override this member function to draw a caption.
	// Parameters: pDC   : Reference to the device context in which to draw.
	//             pPane : Pointer to the tabbed container.
	//             rc    : Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	virtual void DrawCaption(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:    Call this method to draw frame of floating window
	// Parameters: pDC   : Pointer to a valid device context
	//             pPane : Floating window pointer
	//             rc    : Client rectangle of floating frame
	//-----------------------------------------------------------------------
	virtual void DrawFloatingFrame(CDC *pDC, CXTPDockingPaneMiniWnd *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:    Override this member function to draw a tabbed container.
	// Parameters: pDC   : Reference to the device context in which to draw.
	//             pPane : Pointer to the tabbed container.
	//             rc    : Client rectangle of the tabbed container.
	//-----------------------------------------------------------------------
	virtual void DrawPane(CDC *pDC, CXTPDockingPaneTabbedContainer *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:    Override this member function to draw the side panel.
	// Parameters: pDC   : Reference to the device context in which to draw.
	//             pPane : Pointer to the side panel.
	//             rc    : Client rectangle of the side panel.
	//-----------------------------------------------------------------------
	virtual void DrawSidePanel(CDC *pDC, CXTPDockingPaneSidePanel *pPane, CRect rc);

	//-----------------------------------------------------------------------
	// Summary:    Override this member function to draw button of tabbed caption.
	// Parameters: pDC       : Pointer to a valid device context
	//             pButton   : Button pointer need to draw.
	//             clrButton : Color of button text.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionButton(CDC *pDC, CXTPDockingPaneCaptionButton *pButton, COLORREF clrButton);

	//-----------------------------------------------------------------------
	// Summary:    This mmethod is called by the framework to draw the docking pane
	//             auto hide panel background and splitter.
	// Parameters: pDC       : Pointer to the auto hide panes device context.
	//             rc        : Size of the area to draw.
	//             direction : Docking direction either xtpPaneDockLeft, xtpPaneDockRight,
	//                         xtpPaneDockTop or xtpPaneDockBottom.
	//-----------------------------------------------------------------------
	virtual void DrawAutoHidePanel(CDC* pDC, CRect rc, XTPDockingPaneDirection direction);

protected:
    //-----------------------------------------------------------------------
    // Summary:    This member function is called to draw a titel bar for the
	//             dockign pane.
    // Parameters: pDC   : Pointer to a valid device context.
	//             pPane : Pointer to a valid CXTPDockingPaneBase object.
	//             rc    : Size of the title bar to draw.
	//             bVertical : TRUE it the title bar should be drawn vertical.
    //-----------------------------------------------------------------------
	virtual void DrawTitleBar(CDC *pDC, CXTPDockingPaneBase *pPane, CRect rc, BOOL bVertical);

    //-----------------------------------------------------------------------
    // Summary:    This member function is called to draw a gripper for the 
	//             docking pane.
    // Parameters: pDC       : Pointer to a valid device context.
	//             rcGripper : Size of the gripper to draw.
	//             clr       : Specifies the color of the gripper.
	//             bVertical : TRUE it the gripper should be drawn vertical.
    //-----------------------------------------------------------------------
	void DrawGripper(CDC *pDC, CRect rcGripper, COLORREF clr, BOOL bVertical);

    //-----------------------------------------------------------------------
    // Summary:    This member function is called to draw a dotted line.
    // Parameters: pDC       : Pointer to a valid device context.
	//             x         : Specifies the left position of the dotted line.
	//             y         : Specifies the top position of the dotted line.
	//             nSize     : Specifies the length of the dotted line.
	//             clr       : Specifies the color of the dotted line.
	//             bVertical : TRUE if the line to be drawn is vertical.
    //-----------------------------------------------------------------------
	void DrawDottedLine(CDC *pDC, int x, int y, int nSize, COLORREF clr, BOOL bVertical);

    //-----------------------------------------------------------------------
    // Summary:    Call this member function to retrieve the active or inactive
	//             color set for the Docking Pane using the state specified by bActive.
	// Parameters: bActive : TRUE to retrieve the active color set.
    // Returns:    A reference to a CXTPDockingPaneColorSet object representing
	//             the docking pane state specified by bActive.
    //-----------------------------------------------------------------------
	virtual CXTPDockingPaneColorSet* GetColorSet(BOOL bActive);

    //-----------------------------------------------------------------------
    // Summary:    Called to initialize the active and inactive colors sets for
	//             the docking pane theme.
    // Parameters: pcsActive   - Pointer to a CXTPDockingPaneColorSet representing 
	//                           the active pane color set.
	//             pcsInactive - Pointer to a CXTPDockingPaneColorSet representing
	//                           the inactive pane color set.
    //-----------------------------------------------------------------------
	void SetPaneColorSets(CXTPDockingPaneColorSet* pcsActive, CXTPDockingPaneColorSet* pcsInactive);

    //-----------------------------------------------------------------------
    // Summary: Called to free the memory allocated for the docking pane 
	//          active and inactive color sets.
    //-----------------------------------------------------------------------
	void DeletePaneColorSets();

	CXTPDockingPaneColorSet* m_pcsActive;   // Active state docking pane colors.
	CXTPDockingPaneColorSet* m_pcsInactive; // Inactive state docking pane colors.
	CString                  m_strFontFace; // Font face name
	int                      m_nFontSize;   // Font size in points
};

AFX_INLINE CXTPDockingPaneColorSet* CXTPDockingPaneVisualStudio2012Theme::GetColorSet(BOOL bActive) {
	return bActive ? m_pcsActive : m_pcsInactive;
}

#endif // !defined(__XTPDOCKINGPANETHEMEVISUALSTUDIO2012_H__)