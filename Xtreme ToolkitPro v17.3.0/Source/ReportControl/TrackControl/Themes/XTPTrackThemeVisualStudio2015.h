// XTPTrackThemeVisualStudio2015.h: interface for the CXTPTrackThemeVisualStudio2015 class.
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
#if !defined(__XTPTRACKTHEMEVISUALSTUDIO2015_H__)
#define __XTPTRACKTHEMEVISUALSTUDIO2015_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary:
//     CXTPTrackThemeVisualStudio2015 is a CXTPTrackThemeOffice2013 derived
//     class.  This represents an Office 2013 style theme.
//===========================================================================
class _XTP_EXT_CLASS CXTPTrackThemeVisualStudio2015 : public CXTPTrackThemeOffice2013
{
public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Default paint manager constructor, handles properties initialization.
	//-----------------------------------------------------------------------
	CXTPTrackThemeVisualStudio2015();

	//-------------------------------------------------------------------------
	// Summary:
	//     This method is called to refresh the visual metrics of the
	//     report control.
	//-------------------------------------------------------------------------
	virtual void RefreshMetrics();
};

#endif //#if !defined(__XTPTRACKTHEMEVISUALSTUDIO2015_H__)
