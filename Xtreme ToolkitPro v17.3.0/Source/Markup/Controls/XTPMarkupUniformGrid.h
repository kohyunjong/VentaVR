// XTPMarkupUniformGrid.h: interface for the CXTPMarkupUniformGrid class.
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
#if !defined(__XTPMARKUPUNIFORMGRID_H__)
#define __XTPMARKUPUNIFORMGRID_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPMarkupDependencyProperty;
class CXTPMarkupDrawingContext;

//===========================================================================
// Summary: CXTPMarkupUniformGrid is CXTPMarkupPanel derived class. It implements UniformGrid XAML Tag
//===========================================================================
class _XTP_EXT_CLASS CXTPMarkupUniformGrid : public CXTPMarkupPanel
{
	DECLARE_MARKUPCLASS(CXTPMarkupUniformGrid)
protected:
	//-----------------------------------------------------------------------
	// Summary:
	//     Constructs a CXTPMarkupUniformGrid object
	//-----------------------------------------------------------------------
	CXTPMarkupUniformGrid();

	//-----------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPMarkupUniformGrid object, handles cleanup and deallocation
	//-----------------------------------------------------------------------
	virtual ~CXTPMarkupUniformGrid();

public:
	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set columns number of the Grid
	// Parameters:
	//     nColumns - Column count to set
	//-----------------------------------------------------------------------
	void SetColumns(int nColumns);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines number of columns of the Grid
	// Returns:
	//     Number of columns of the Grid
	//-----------------------------------------------------------------------
	int GetColumns() const;

	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set rows number of the Grid
	// Parameters:
	//     nRows - rows count to set
	//-----------------------------------------------------------------------
	void SetRows(int nRows);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines number of rows of the Grid
	// Returns:
	//     Number of rows of the Grid
	//-----------------------------------------------------------------------
	int GetRows() const;


	//-----------------------------------------------------------------------
	// Summary:
	//     Call this method to set first column for first row of the grid
	// Parameters:
	//     nFirstColumn - First column position of first row
	//-----------------------------------------------------------------------
	void SetFirstColumn(int nFirstColumn);

	//-----------------------------------------------------------------------
	// Summary:
	//     Determines position of first column of first row
	// Parameters:
	//     First column position of first row
	//-----------------------------------------------------------------------
	int GetFirstColumn() const;

protected:
//{{AFX_CODEJOCK_PRIVATE
	// Implementation
	virtual CSize MeasureOverride(CXTPMarkupDrawingContext* pDC, CSize szAvailableSize);
	virtual CSize ArrangeOverride(CSize szFinalSize);

private:
	void UpdateComputedValues();

private:
	int m_nColumns;
	int m_nRows;
	int m_nFirstColumn;

public:
	static CXTPMarkupDependencyProperty* m_pFirstColumnProperty;
	static CXTPMarkupDependencyProperty* m_pColumnsProperty;
	static CXTPMarkupDependencyProperty* m_pRowsProperty;
//}}AFX_CODEJOCK_PRIVATE

	//{{AFX_CODEJOCK_PRIVATE
public:
	DECLARE_DISPATCH_MAP()
#ifdef _XTP_ACTIVEX
	DECLARE_INTERFACE_MAP()
	DECLARE_OLETYPELIB_EX(CXTPMarkupUniformGrid);
#endif

	afx_msg long OleGetColumns();
	afx_msg long OleGetRows();
	afx_msg long OleGetFirstColumn();
	afx_msg void OleSetColumns(long);
	afx_msg void OleSetRows(long);
	afx_msg void OleSetFirstColumn(long);
	//}}AFX_CODEJOCK_PRIVATE
};

#endif // !defined(__XTPMARKUPUNIFORMGRID_H__)
