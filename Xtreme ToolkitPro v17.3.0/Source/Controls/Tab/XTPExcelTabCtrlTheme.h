// XTPExcelTabCtrlTheme.h: interface for the CXTPExcelTabCtrlTheme class.
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
#if !defined(__XTPEXCELTABCTRLTHEME_H__)
#define __XTPEXCELTABCTRLTHEME_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPTcbItem;
class CXTPExcelTabCtrl;
class CXTPExcelTabCtrlButtonState;

//===========================================================================
// Summary:
//     CXTPExcelTabCtrlTheme is used to draw the CXTPExcelTabCtrl object.  All themes
//     used for CXTPExcelTabCtrl should inherit from this base class.
//===========================================================================
class _XTP_EXT_CLASS CXTPExcelTabCtrlTheme : public CXTPControlTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPExcelTabCtrlTheme object.
	//-------------------------------------------------------------------------
	CXTPExcelTabCtrlTheme();

	//-------------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPExcelTabCtrlTheme object, handles cleanup and deallocation.
	//-------------------------------------------------------------------------
	~CXTPExcelTabCtrlTheme();

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Call this function to refresh the flat tab control colors,
	//     position any scroll bars to their default positions, and then
	//     reset the control to the system defaults.
	// Parameters:
	//     pTab - Points to the CXTPExcelTabCtrl object.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPExcelTabCtrl* pTab);

public:

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function will draw a tab to the device context specified
	//     by 'pDC'.
	// Parameters:
	//     pDC          - A CDC pointer that represents the current device
	//                    context.
	//     pTabCtrl     - A pointer to a CXTPExcelTabCtrl object.
	//     pt           - A CPoint object that specifies the position of
	//                    the XY location of the top left corner of the tab to draw.
	//     bSelected    - True if the tab is currently selected.
	//     pTcbItem     - Tab pointer to render.
	// Returns:
	//     The x position of the next tab to be drawn if successful, otherwise
	//     returns -1;.
	//-------------------------------------------------------------------------
	virtual int DrawTab(CDC* pDC, CXTPExcelTabCtrl* pTabCtrl, const CPoint& pt, bool bSelected, CXTPTcbItem* pTcbItem);

	//-------------------------------------------------------------------------
	// Summary:
	//     This member function is used by the flat tab control to draw an arrow
	//     button to the device context specified by 'pDC'.
	// Parameters:
	//     pDC          - A CDC pointer that represents the current device
	//                    context.
	//     pTabCtrl     - A pointer to a CXTPExcelTabCtrl object.
	//     state - XY location of the top left corner of the tab to draw.
	//-------------------------------------------------------------------------
	virtual void DrawButton(CDC* pDC, CXTPExcelTabCtrl* pTabCtrl, CXTPExcelTabCtrlButtonState& state);

	//-----------------------------------------------------------------------
	// Summary: Call this member function to draw the background of the button.
	// Parameters:
	//      pDC   - A pointer to a valid device context.
	//      rect  - A CRect object that contains the location and the dimensions of the button.
	//      state - A CXTPExcelTabCtrlButtonState that contains the current state of the button.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBack(CDC* pDC, CRect& rect, CXTPExcelTabCtrlButtonState& state);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the background
	//     color of the tab.
	// Parameters:
	//     pTcbItem - The tab pointer to get background color for.
	// Returns:
	//     An RGB value that represents the tab background color if successful,
	//     otherwise returns COLORREF_NULL.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTabBackColor(CXTPTcbItem* pTcbItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the text color
	//     of the tab.
	// Parameters:
	//     pTcbItem - The tab pointer to get text color for.
	// Returns:
	//     An RGB value that represents the tab text color if successful,
	//     otherwise returns COLORREF_NULL.
	//-----------------------------------------------------------------------
	virtual COLORREF GetTabTextColor(CXTPTcbItem* pTcbItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the background
	//     color for selected tabs.
	// Parameters:
	//     pTcbItem - The tab pointer to get selected background color for.
	// Returns:
	//     An RGB value that represents the selected tabs background color if
	//     successful, otherwise returns COLORREF_NULL.
	//-----------------------------------------------------------------------
	virtual COLORREF GetSelTabBackColor(CXTPTcbItem* pTcbItem) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function gets an RGB value that represents the text color
	//     for selected tabs.
	// Parameters:
	//     pTcbItem - The tab pointer to get selected text color for.
	// Returns:
	//     An RGB value that represents the selected tab text color if
	//     successful, otherwise returns COLORREF_NULL.
	//-----------------------------------------------------------------------
	virtual COLORREF GetSelTabTextColor(CXTPTcbItem* pTcbItem) const;

	CXTPPaintManagerColor m_clr3DFace;         // RGB value that represents the normal tab face color.
	CXTPPaintManagerColor m_clr3DFacePushed;   // RGB value for pushed background color.
	CXTPPaintManagerColor m_clr3DFaceHilite;   // RGB value for highlighted background color.
	CXTPPaintManagerColor m_clrBtnText;        // RGB value that represents the tab outline color.
	CXTPPaintManagerColor m_clrBtnTextGray;    // RGB value for disabled text color.
	CXTPPaintManagerColor m_clrBtnTextPushed;  // RGB value for highlighted text color.
	CXTPPaintManagerColor m_clrBtnTextHilite;  // RGB value for pushed text color.
	CXTPPaintManagerColor m_clrWindow;         // RGB value that represents the selected tab face color.
	CXTPPaintManagerColor m_clrWindowText;     // RGB value that represents the tab text color.
	CXTPPaintManagerColor m_clr3DHilight;      // RGB value that represents the tab highlight color.
	CXTPPaintManagerColor m_clr3DShadow;       // RGB value that represents the tab shadow color.
	CXTPPaintManagerColor m_clrBorder3DHilite; // RGB value for 3D border highlight color.
	CXTPPaintManagerColor m_clrBorder3DShadow; // RGB value for 3D border shadow color.

	int m_cx; // Width for each arrow button.
	int m_cy; // Height for each arrow button
};

//===========================================================================
// Summary:
//     CXTPExcelTabCtrlThemeOfficeXP is used to draw the CXTPExcelTabCtrl object.
//===========================================================================
class _XTP_EXT_CLASS CXTPExcelTabCtrlThemeOfficeXP : public CXTPExcelTabCtrlTheme
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPExcelTabCtrlThemeOfficeXP object.
	//-------------------------------------------------------------------------
	CXTPExcelTabCtrlThemeOfficeXP();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	//     pTab - Points to the CXTPExcelTabCtrl object.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPExcelTabCtrl* pTab);

	///-----------------------------------------------------------------------
	// Summary: Call this member function to draw the background of the button.
	// Parameters:
	//      pDC   - A pointer to a valid device context.
	//      rect  - A CRect object that contains the location and the dimensions of the button.
	//      state - A CXTPExcelTabCtrlButtonState that contains the current state of the button.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBack(CDC* pDC, CRect& rect, CXTPExcelTabCtrlButtonState& state);
};

//===========================================================================
// Summary:
//     CXTPExcelTabCtrlThemeOffice2003 is used to draw the CXTPExcelTabCtrl object.
//===========================================================================
class _XTP_EXT_CLASS CXTPExcelTabCtrlThemeOffice2003 : public CXTPExcelTabCtrlThemeOfficeXP
{
public:

	//-------------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPExcelTabCtrlThemeOffice2003 object.
	//-------------------------------------------------------------------------
	CXTPExcelTabCtrlThemeOffice2003();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called by the theme manager to refresh
	//     the visual styles used by each components theme.
	//     pTab - Points to the CXTPExcelTabCtrl object.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics(CXTPExcelTabCtrl* pTab);

	//-----------------------------------------------------------------------
	// Summary: Call this member function to draw the background of the button.
	// Parameters:
	//      pDC   - A pointer to a valid device context.
	//      rect  - A CRect object that contains the location and the dimensions of the button.
	//      state - A CXTPExcelTabCtrlButtonState that contains the current state of the button.
	//-----------------------------------------------------------------------
	virtual void DrawButtonBack(CDC* pDC, CRect& rect, CXTPExcelTabCtrlButtonState& state);
};

/////////////////////////////////////////////////////////////////////////////

#endif // !defined(__XTPEXCELTABCTRLTHEME_H__)
