// XTPMarkupDefinitionBase.h: interface for the CXTPMarkupDefinitionBase class.
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
#if !defined(__XTPMARKUPDEFINITIONBASE_H__)
#define __XTPMARKUPDEFINITIONBASE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupGrid;
class CXTPMarkupObject;
class CXTPMarkupPropertyChangedEventArgs;
class CXTPMarkupGridLength;

//===========================================================================
// Summary: CXTPMarkupDefinitionBase is base class for CXTPMarkupColumnDefinition and CXTPMarkupRowDefinition
//          that defines constrains and definitions for Grid rows and columns
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupDefinitionBase : public CXTPMarkupFrameworkContentElement
{
	DECLARE_MARKUPCLASS(CXTPMarkupDefinitionBase);
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupDefinitionBase object
	// Parameters:
	//     bIsColumnDefinition - TRUE for CXTPMarkupColumnDefinition, FALSE for CXTPMarkupRowDefinition
	//-----------------------------------------------------------------------
	CXTPMarkupDefinitionBase(BOOL bIsColumnDefinition = FALSE);

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Returns minimum user defined size
	// Returns:
	//     Minimum constrain for column or row
	//-----------------------------------------------------------------------
	int GetUserMinSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns maximum user defined size
	// Returns:
	//     Maximum constrain for column or row
	//-----------------------------------------------------------------------
	int GetUserMaxSize() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Returns user defined size
	// Returns:
	//     User defined size for column or row
	//-----------------------------------------------------------------------
	CXTPMarkupGridLength* GetUserSize() const;

//{{AFX_CODEJOCK_PRIVATE
	// Implementation
private:
	void OnBeforeLayout();
	void UpdateMinSize(int nMinSize);
	double GetPreferredSize() const;

protected:
	static void AFX_CDECL OnDefinitionPropertyChanged(CXTPMarkupObject* d, CXTPMarkupPropertyChangedEventArgs* /*e*/);

protected:
	BOOL m_bIsColumnDefinition;
	int m_nMinSize;
	int m_nSizeType;
	double m_nMeasureSize;
	double m_nSizeCache;
	int m_nFinalOffset;

	friend class CXTPMarkupGrid;
//}}AFX_CODEJOCK_PRIVATE

//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
#ifdef _XTP_ACTIVEX
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupDefinitionBase);
#endif

//}}AFX_CODEJOCK_PRIVATE
};

#endif // !defined(__XTPMARKUPDEFINITIONBASE_H__)
