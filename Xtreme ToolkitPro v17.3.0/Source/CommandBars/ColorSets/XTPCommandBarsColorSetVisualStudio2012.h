// XTPCommandBarsColorSetVisualStudio2012.h
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
#if !defined(__XTPCOMMANDBARSCOLORSETVISUALSTUDIO2012_H__)
#define __XTPCOMMANDBARSCOLORSETVISUALSTUDIO2012_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetVisualStudio2012

class _XTP_EXT_CLASS CXTPCommandBarsColorSetVisualStudio2012 : public CXTPCommandBarsColorSet
{
public:

	CXTPCommandBarsColorSetVisualStudio2012();

	virtual void RefreshColors(const CXTPResourceImages *pImages);
};

//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetVisualStudio2012Light

class _XTP_EXT_CLASS CXTPCommandBarsColorSetVisualStudio2012Light : public CXTPCommandBarsColorSet
{
public:

	CXTPCommandBarsColorSetVisualStudio2012Light();

	virtual void RefreshColors(const CXTPResourceImages *pImages);
};


//////////////////////////////////////////////////////////////////////////
// CXTPCommandBarsColorSetVisualStudio2012Dark

class _XTP_EXT_CLASS CXTPCommandBarsColorSetVisualStudio2012Dark : public CXTPCommandBarsColorSet
{
public:

	CXTPCommandBarsColorSetVisualStudio2012Dark();

	virtual void RefreshColors(const CXTPResourceImages *pImages);
};

#endif // !defined(__XTPCOMMANDBARSCOLORSETVISUALSTUDIO2012_H__)
