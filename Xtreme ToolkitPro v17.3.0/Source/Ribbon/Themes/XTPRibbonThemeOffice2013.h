// XTPRibbonThemeOffice2013.h: interface for the CXTPRibbonOffice2013Theme class.
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
#if !defined(__XTPRIBBONTHEMEOFFICE2013_H__)
#define __XTPRIBBONTHEMEOFFICE2013_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     Struct used to define context tab colors for the ribbon control.
//===========================================================================
struct XTPRIBBON_HEADER_COLOR
{
	COLORREF clrEdge; // RGB value representing context tab border color.
	COLORREF clrBack; // RGB value representing context tab background color.
	COLORREF clrText; // RGB value representing context tab text color.
};

//===========================================================================
// Summary:
//     CXTPRibbonOffice2013Theme is a CXTPRibbonMarkupTheme derived class
//     used to represent a Office 2013 ribbon theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPRibbonOffice2013Theme : public CXTPRibbonMarkupTheme
{
	DECLARE_DYNAMIC(CXTPRibbonOffice2013Theme)

public:

	// --------------------------------------------------------
	// Summary:
	//     Constructs a CXTPRibbonOffice2013Theme object
	// Parameters:
	//     pPaintManager :  \Returns a CXTPPaintManager object.
	// --------------------------------------------------------
	CXTPRibbonOffice2013Theme(CXTPPaintManager *pPaintManager);

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPRibbonOffice2013Theme object, handles cleanup and deallocation.
	//-----------------------------------------------------------------------
	virtual ~CXTPRibbonOffice2013Theme();

	//-------------------------------------------------------------------------
	// Summary:
	//      This method is called to refresh the visual metrics of manager.
	// Remarks:
	//      Refreshes all of the colors in the Ribbon Bar.  Many of the colors
	//      used are set with the GROUP_COLOR structure.
	// See Also:
	//     GROUP_COLOR
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();

	//-----------------------------------------------------------------------
	// Summary:
	//     Calculates ribbon caption height
	//-----------------------------------------------------------------------
	virtual int GetRibbonCaptionHeight(const CXTPRibbonBar *pRibbonBar) const;

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw single tab of ribbon bar
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pItem - Ribbon tab to draw
	//-----------------------------------------------------------------------
	virtual void DrawRibbonTab(CDC *pDC, CXTPRibbonTab *pItem);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw ribbon context headers
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pContextHeaders - Collection of context header
	//     pRibbonBar - Parent RibbonBar pointer
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameContextHeaders(CDC *pDC, CXTPRibbonBar *pRibbonBar, CXTPRibbonTabContextHeaders *pContextHeaders);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to fill background of Ribbon Bar control.
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pRibbonBar - Points to a CXTPRibbonBar object
	//-----------------------------------------------------------------------
	virtual void FillRibbonBar(CDC *pDC, CXTPRibbonBar *pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//     This method is called to draw Ribbon caption
	// Parameters:
	//     pDC - Pointer to a valid device context
	//     pRibbonBar - Ribbon Bar to draw
	//     bActive - TRUE if ribbon frame is active
	//-----------------------------------------------------------------------
	virtual void DrawRibbonFrameCaption(CDC *pDC, CXTPRibbonBar *pRibbonBar, BOOL bActive);

	virtual void SetBackImage(int nBackImage);

protected:

	virtual CString GetBackImageName();

	virtual void DrawBackImage(CDC *pDC, CXTPRibbonBar *pRibbonBar);

	//-----------------------------------------------------------------------
	// Summary:
	//		Helper class used to define variables to be used by XAML Markup code.
	//-----------------------------------------------------------------------
	class CThemeStaticExtension : public CXTPMarkupStaticExtension
	{
	public:
		//-------------------------------------------------------------------
		// Summary:
		//		Default constructor, initializes CXTPMarkupStaticExtension
		//      variables.
		//-------------------------------------------------------------------
		CThemeStaticExtension();
	};

	XTPRIBBON_HEADER_COLOR m_clrHeader[24]; // Array of color values used for rendering the Ribbon's context tabs.

private:

	int m_nBackImage;
	int m_nCaptionHeight;
	BOOL IsGdiplusInitialized() const;
	ULONG_PTR m_GdiPlusToken;
};

#endif // !defined(__XTPRIBBONTHEMEOFFICE2013_H__)
