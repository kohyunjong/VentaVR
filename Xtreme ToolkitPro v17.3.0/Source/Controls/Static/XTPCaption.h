// XTPCaption.h : interface for the CXTPCaption class.
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
#if !defined(__XTPCAPTION_H__)
#define __XTPCAPTION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CXTPCaption;
class CXTPCaptionPopupWnd;
class CXTPCaptionTheme;

#define XTP_IDC_BTN_CLOSE                100

// --------------------------------------------------------------------
// Summary:
//     CXTPCaptionButton is a CXTPButton derived class. It is used
//     by the CXTPCaption class to activate a CXTPCaptionPopupWnd window.
// --------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPCaptionButton : public CXTPButton
{
public:
	//-----------------------------------------------------------------------
	// Summary      : Constructs a CXTPCaptionButton object
	//-----------------------------------------------------------------------
	CXTPCaptionButton();

	//-----------------------------------------------------------------------
	// Summary      : This member function is called to return a pointer to the
	//                parent caption.
	// Returns      : A pointer to a CXTPCaption object.
	//-----------------------------------------------------------------------
	CXTPCaption* GetCaption() {
		return m_pCaption;
	}
	//-----------------------------------------------------------------------
	// Summary      : This member function is called to set the pointer to the
	//                parent caption.
	// Parameters   : pCaption - A pointer to a CXTPCaption object.
	//-----------------------------------------------------------------------
	void SetCaption(CXTPCaption* pCaption) {
		m_pCaption = pCaption;
	}
	virtual BOOL SetTheme(XTPControlTheme eTheme);

protected:
	CXTPCaption* m_pCaption; // A pointer to the parent CXTPCaption.
};

const DWORD CPWS_EX_GROOVE_EDGE    = 0x0001;  //<ALIAS CXTPCaption::Create@CWnd*@LPCTSTR@DWORD@DWORD@const CRect&@UINT>
const DWORD CPWS_EX_RAISED_EDGE    = 0x0002;  //<ALIAS CXTPCaption::Create@CWnd*@LPCTSTR@DWORD@DWORD@const CRect&@UINT>
const DWORD CPWS_EX_CLOSEBUTTON    = 0x0004;  //<ALIAS CXTPCaption::Create@CWnd*@LPCTSTR@DWORD@DWORD@const CRect&@UINT>

// ----------------------------------------------------------------------
// Summary:
//     CXTPCaption is a CStatic derived class. It is used to create
//     caption or info bars similar to those in Microsoft(r) Outlook.
// ----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPCaption : public CStatic
{
	DECLARE_DYNAMIC(CXTPCaption)

public:

	// ------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPCaption object, handles cleanup and
	//     deallocation
	// ------------------------------------------------------------
	virtual ~CXTPCaption();

	// -----------------------------------------
	// Summary:
	//     Constructs a CXTPCaption object
	// -----------------------------------------
	CXTPCaption();

public:

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member to switch the visual theme of the control.
	// Parameters:
	//     nTheme - New visual theme. Can be any of the values listed in the Remarks section.
	// Remarks:
	//     nStyle can be one of the following:
	//     * <b>xtpControlThemeDefault</b> Use default theme.
	//     * <b>xtpControlThemeOfficeXP</b> Use Office XP theme.
	//     * <b>xtpControlThemeOffice2003</b> Use Office 2003 theme.
	//-----------------------------------------------------------------------
	BOOL SetTheme(XTPControlTheme eTheme);
	BOOL SetTheme(CXTPCaptionTheme* pTheme);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a pointer to the currently seleted theme.
	// Returns:
	//     A pointer to a CXTPCaptionTheme object representing the currently selected theme.
	//-----------------------------------------------------------------------
	CXTPCaptionTheme* GetTheme();

	//-----------------------------------------------------------------------
	// Summary      : This member function is called to determine if the caption
	//                is configured to be used as the application's primary caption.
	// Returns      : true if the caption is configured to be used as the application's
	//                primary caption, otherwise false.
	//-----------------------------------------------------------------------
	bool IsAppCaption() const {
		return m_bAppCaption;
	}
	//-----------------------------------------------------------------------
	// Summary      : This member function is called to set the flag that indicates
	//                if the caption is the application's primary caption.
	// Parameters   : bAppCaption - true if the caption is configured to be used as the
	//                applications primary caption.
	//-----------------------------------------------------------------------
	void SetAppCaption(bool bAppCaption) {
		m_bAppCaption = bAppCaption;
	}

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to get a reference to the caption's close
	//     / popup button.
	// Returns:
	//     A reference to a CXTPCaptionButton object.
	//-----------------------------------------------------------------------
	CXTPCaptionButton& GetCaptionButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function will set the caption bar border, background,
	//     and font colors.
	// Parameters:
	//     clrBorder - An RGB value that represents the new border color.
	//     clrFace - An RGB value that represents the new background color.
	//     clrText - An RGB value that represents the new font color.
	//-----------------------------------------------------------------------
	virtual void SetCaptionColors(COLORREF clrBorder, COLORREF clrFace, COLORREF clrText);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to enable Office 2003 visualizations.
	// Parameters:
	//     b2003Colors - True to enable Office 2003 visualizations, false
	//                   to disable.
	//-----------------------------------------------------------------------
	void SetOffice2003Colors(bool b2003Colors = true);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to return the current style settings
	//     for the caption.
	// Returns:
	//     A DWORD value that represents the current style set for the caption.
	//-----------------------------------------------------------------------
	DWORD GetCaptionStyle() const;

	// -------------------------------------------------------------------
	// Summary:
	//     Modifies the caption title and appearance.
	// Parameters:
	//     nBorderSize -   Specifies size in pixels of the banner border.
	//     pFont -         Specifies the new caption font.
	//     lpszWindText -  NULL terminated string specifying the new caption
	//                     text.
	//     hIcon -         Handle of the icon to be drawn in the caption.
	// Remarks:
	//     You can use this member function to set the border size that is
	//     drawn around the caption banner, the font that the caption will
	//     use, and the caption text and icon to be displayed.
	// -------------------------------------------------------------------
	virtual void ModifyCaptionStyle(int nBorderSize, CFont* pFont = NULL, LPCTSTR lpszWindText = NULL, HICON hIcon = NULL);

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to set the child and notification windows
	//     for the caption bar.
	// Parameters:
	//     pChild     - A CWnd pointer that represents the child window to be
	//                  displayed in the popup window.
	//     pNotifyWnd - A CWnd pointer that represents the window to receive
	//                  notification messages.
	//-----------------------------------------------------------------------
	virtual void SetChildWindow(CWnd* pChild, CWnd* pNotifyWnd);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to destroy the popup window and
	//     associated children.
	//-----------------------------------------------------------------------
	virtual void KillChildWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to display the popup window and
	//     associated children.
	// Returns:
	//     TRUE if the popup window is successfully displayed, otherwise FALSE.
	//-----------------------------------------------------------------------
	virtual BOOL ShowPopupWindow();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to update the text and icon for the
	//     caption bar.
	// Parameters:
	//     lpszWindowText - NULL terminated string to display in the caption bar.
	//     hIcon          - Handle of the icon to display in the caption
	//                      bar. It can be NULL.
	//-----------------------------------------------------------------------
	virtual void UpdateCaption(LPCTSTR lpszWindowText, HICON hIcon);

	// ---------------------------------------------------------------------------------
	// Summary:
	//     Creates and initializes the child window associated with the
	//     CXTPCaption object.
	// Parameters:
	//     pParentWnd -      Specifies the parent window.
	// lpszWindowName -  Points to a NULL terminated character string that contains
	//                       the window name. This will be displayed in the caption
	//                       area.
	// dwExStyle -       Specifies caption bar style. See the Remarks section below
	//                       for a complete list of available styles.
	// dwStyle -         Specifies the control window style. Apply any combination
	//                       of caption bar styles to the control.
	// rect -            Specifies the position and size of the caption bar. It can
	//                       be either a <i>RECT</i> structure or a <i>CRect</i> object.
	// nID -             Specifies the caption bar control ID.
	// Remarks:
	//     This method creates a caption bar window and attaches it to the
	//     CXTPCaption object. You construct a caption bar in two steps.
	//     First, call the constructor, which constructs the CXTPCaption
	//     \object. Then call Create, which creates the Window's child window
	//     and attaches it to CXTPCaption. Create initializes the window class
	//     name and window name and registers values for its style, parent,
	//     and ID.<p/>
	// <p/>
	// Styles to be added or removed can be combined by using the bitwise
	//     OR (|) operator. It can be one or more of the following:
	//     * <b>CPWS_EX_GROOVE_EDGE</b> Show the caption with a sunken
	//           border.
	//     * <b>CPWS_EX_RAISED_EDGE</b> Show the caption with a raised 3D
	//           border.
	//     * <b>CPWS_EX_CLOSEBUTTON</b> Caption has a close button.
	// Returns:
	//     TRUE if successful, otherwise returns FALSE.
	// ---------------------------------------------------------------------------------
	virtual BOOL Create(CWnd* pParentWnd, LPCTSTR lpszWindowName, DWORD dwExStyle = CPWS_EX_RAISED_EDGE, DWORD dwStyle = WS_VISIBLE | SS_CENTER | SS_CENTERIMAGE, const CRect& rect = CRect(0, 0, 0, 0), UINT nID = 0xffff);

	// ----------------------------------------------------------------
	// Summary:
	//     This member function is called to refresh the caption button
	//     style.
	// ----------------------------------------------------------------
	void RefreshButton();

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function retrieves the current caption title.
	// Returns:
	//     A reference to a CString object that contains the caption title.
	//-----------------------------------------------------------------------
	const CString& GetCaptionText();

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to see if the caption has a close button.
	// Returns:
	//     TRUE if the caption contains a close button, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL HasCloseButton() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine the size and location
	//     of the text associated with the caption.
	// Returns:
	//     The size and location of the caption text or an empty rect if no text
	//     is defined.
	//-----------------------------------------------------------------------
	virtual CRect GetTextRect() const;

protected:

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to draw the caption background.
	// Parameters:
	//     pDC    - A CDC pointer that represents the current device context.
	//     rcItem - A CRect reference that represents the size of the area to paint.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionBack(CDC* pDC, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to paint the caption text.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionText(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to paint the caption icon.
	// Parameters:
	//     pDC    - A CDC pointer that represents the current device context.
	//     rcItem - A CRect reference that represents the size of the area to paint.
	//-----------------------------------------------------------------------
	virtual void DrawCaptionIcon(CDC* pDC, CRect& rcItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to update the caption text.
	// Parameters:
	//     pDC - A CDC pointer that represents the current device context.
	//-----------------------------------------------------------------------
	virtual void UpdateCaptionText(CDC* pDC);

	//-----------------------------------------------------------------------
	// Summary:
	//     This member function is called to determine the size and location
	//     of the button associated with the caption.
	// Returns:
	//     The size and location of the caption button or an empty rect
	//     if no button is defined.
	//-----------------------------------------------------------------------
	virtual CRect GetButtonRect() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this member function to refresh theme colors and redraw the control.
	//-----------------------------------------------------------------------
	virtual void RefreshMetrics();

//{{AFX_CODEJOCK_PRIVATE

	//{{AFX_VIRTUAL(CXTPCaption)
	virtual BOOL OnWndMsg(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CXTPCaption)
	afx_msg void OnPaint();
	afx_msg LRESULT OnPrintClient(WPARAM wParam, LPARAM /*lParam*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnSysColorChange();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnCaptButton();
	afx_msg void OnPushPinButton();
	afx_msg void OnPushPinCancel();
	//}}AFX_MSG
	afx_msg LRESULT OnSetTheme(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
//}}AFX_CODEJOCK_PRIVATE

private:
	BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = 0xffff);

public:
	int                 m_nBorder;      // Size in pixels for the caption border.
	COLORREF            m_clrBorder;    // An RGB value that represents the user defined border color.
	COLORREF            m_clrFace;      // An RGB value that represents the user defined background color.
	COLORREF            m_clrText;      // An RGB value that represents the user defined font color.

protected:

	int                  m_nOffset;      // Size in pixels that the child window should leave for its caption area.
	bool                 m_bUserColors;  // true if the user has specified caption colors other than the default.
	bool                 m_bAppCaption;  // true if the caption is used as the application's primary caption.
	CWnd*                m_pChildWnd;    // A CWnd pointer that represents the child window displayed in the popup window.
	CWnd*                m_pParentView;  // A CWnd pointer that represents the child window's parent view.
	CWnd*                m_pSplitterWnd; // A CWnd pointer that represents the splitter window. It is used to track size changes.
	CSize                m_sizeIcon;     // Width and height of the caption icon area.
	CRect                m_rcChild;      // Size of the child window displayed in the popup.
	CRect                m_rcParent;     // Size of the child's parent view.
	CRect                m_rcSplitter;   // Size of the splitter window.
	DWORD                m_dwExStyle;    // Border style bits, either CPWS_EX_GROOVE_EDGE | CPWS_EX_RAISED_EDGE.
	HICON                m_hIcon;        // User defined icon handle. The default value is NULL.
	CString              m_strCaption;   // Text that will be displayed in the caption.
	CImageList           m_ilButton;     // Image list used to create the close button icon.
	CXTPCaptionButton    m_btnCaption;   // Button used to 'tack' the popup window back in place.
	CXTPCaptionPopupWnd* m_pPopupWnd;    // Points to the popup window.
	CXTPCaptionTheme*    m_pTheme;       // Pointer to the current theme object.
	BOOL                 m_bSubclassed;  // TRUE if the window was sub-classed.

	friend class CXTPCaptionButton;
	friend class CXTPCaptionButtonTheme;
	friend class CXTPCaptionTheme;
};

//////////////////////////////////////////////////////////////////////

AFX_INLINE CXTPCaptionButton& CXTPCaption::GetCaptionButton() {
	ASSERT(::IsWindow(m_btnCaption.m_hWnd)); return m_btnCaption;
}
AFX_INLINE DWORD CXTPCaption::GetCaptionStyle() const {
	return m_dwExStyle;
}
AFX_INLINE const CString& CXTPCaption::GetCaptionText() {
	return m_strCaption;
}
AFX_INLINE BOOL CXTPCaption::HasCloseButton() const {
	return ((m_dwExStyle & CPWS_EX_CLOSEBUTTON) == CPWS_EX_CLOSEBUTTON);
}
AFX_INLINE BOOL CXTPCaption::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) {
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}
AFX_INLINE BOOL CXTPCaption::Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) {
	return CStatic::Create(lpszText, dwStyle, rect, pParentWnd, nID);
}
AFX_INLINE CXTPCaptionTheme* CXTPCaption::GetTheme() {
	return m_pTheme;
}

#endif // #if !defined(__XTPCAPTION_H__)
