// XTPSystemMetrics.h
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
#if !defined(__XTPSYSTEMMETRICS_H__)
#define __XTPSYSTEMMETRICS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class _XTP_EXT_CLASS CXTPSystemMetrics
{
	friend class CXTPSingleton<CXTPSystemMetrics>;

public:
	CXTPSystemMetrics();

	void RefreshMetrics();

public:

	int m_nBorderY;    // The height of a window border, in pixels.
	int m_nCaptionY;   // The height of a caption area, in pixels.
	int m_nSizeFrameY; // The thickness of the sizing border around the perimeter of a window that can be resized, in pixels.

	// Frame icon
	int m_nSmallIconX; // SM_CYSMICON
	int m_nSmallIconY; // SM_CXSMICON

};


_XTP_EXT_CLASS CXTPSystemMetrics* AFX_CDECL XTPSystemMetrics();


class _XTP_EXT_CLASS CXTPDeviceCaps
{
	friend class CXTPSingleton<CXTPDeviceCaps>;

public:
	CXTPDeviceCaps();

	void RefreshMetrics();

public:
	// Log pixels
	int m_nLogPixelsX; // LOGPIXELSX
	int m_nLogPixelsY; // LOGPIXELSY
};

_XTP_EXT_CLASS CXTPDeviceCaps* AFX_CDECL XTPDeviceCaps();


#endif // !defined(__XTPSYSTEMMETRICS_H__)
