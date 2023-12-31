// XTPFlowGraphConnectionPoints.h: interface for the CXTPFlowGraphConnectionPoints class.
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
#if !defined(__XTPFLOWGRAPHCONNECTIONPOINTS_H__)
#define __XTPFLOWGRAPHCONNECTIONPOINTS_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPFlowGraphNode;
class CXTPFlowGraphConnectionPoint;

// ------------------------------------------------------------------
//
// Summary:
//     Represents a collection of Connection Points for a Node.
// Remarks:
//     Connection points are added to a Node and can no input points,
//     \only input or output points, or can have both.
// ------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPFlowGraphConnectionPoints : public CXTPCmdTarget
{
protected:
	// ------------------------------------------------------
	// Summary:
	//     Constructs a CXTPFlowGraphConnectionPoints object.
	// ------------------------------------------------------
	CXTPFlowGraphConnectionPoints(CXTPFlowGraphNode* pNode);
	// --------------------------------------------------------------------
	// Summary:
	//     Destroys a CXTPFlowGraphConnectionPoints object, handles cleanup
	//     and deallocation.
	// --------------------------------------------------------------------
	virtual ~CXTPFlowGraphConnectionPoints();

public:
	// -----------------------------------------------------------------
	// Summary:
	//     Adds a connection point to the Node.
	// Parameters:
	//     pConnectionPoint :  Reference to the connection point to add.
	//
	// Returns:
	//     A reference to the connection point that was just added.
	//
	//
	// -----------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* AddConnectionPoint(CXTPFlowGraphConnectionPoint* pConnectionPoint, int nIndex = -1);

	// ------------------------------------------------
	// Summary:
	//     Removes all connection points from the node.
	// ------------------------------------------------
	void RemoveAll();

	// --------------------------------------------------------------------
	// Summary:
	//     Removes the specified connection point.
	// Parameters:
	//     pConnectionPoint :  Reference to the connection point to remove.
	//     bRemovePairs:       Indicates whether to remove the other connection
	//                         point with the same name (but different type (IN/OUT)
	// --------------------------------------------------------------------
	void Remove(CXTPFlowGraphConnectionPoint* pConnectionPoint, BOOL bRemovePairs = TRUE);
	// ---------------------------------------------------------------------
	// Summary:
	//     Removes the specified connection point from the node's collection
	//     \of connection points.
	// Parameters:
	//     nIndex :  Index of the connection point to remove.
	// ---------------------------------------------------------------------
	void RemoveAt(int nIndex, BOOL bRemovePairs = TRUE, BOOL bSkipUndoTask = FALSE);

	// -----------------------------------------------------------------
	// Summary:
	//     \Returns the total number of connections points for the node.
	// Returns:
	//     The total number of connections points for the node.
	// -----------------------------------------------------------------
	int GetCount() const;
	// ---------------------------------------------------------------
	// Summary:
	//     Get the connection point at the specified index.
	// Parameters:
	//     nIndex :  Index of the desired connection point to return.
	//
	// Returns:
	//     Reference to the connection point at the specified index.
	// ---------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* GetAt(int nIndex) const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Finds and returns the specified connection point by searching for
	//     the connection point's name. There can be 2 connection points with the same
	//     name but different type (in & out), so sometimes you should provide a type
	// Parameters:
	//     lpszName :  name of the connection point to find.
	//
	// Returns:
	//     \Returns a reference to the FlowGraphConnectionPoint if found,
	//     \otherwise it will return Nothing\\Null.
	// ---------------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* FindConnectionPoint(LPCTSTR lpszName, XTPFlowGraphConnectionPointType type = xtpFlowGraphPointNone) const;
	// ------------------------------------------------------------------
	// Summary:
	//     Finds and returns the specified connection point.
	// Parameters:
	//     nId :  ID of the connection point to find.
	// Returns:
	//     \Returns a reference to the FlowGraphConnectionPoint if found,
	//     \otherwise it will return Nothing\\Null.
	// ------------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* FindConnectionPoint(int nId) const;

	// ---------------------------------------------------------------------
	// Summary:
	//     Finds and returns the specified connection point by searching for
	//     the connection point's caption. There can be 2 connection points with the same
	//     caption but different type (in & out), so sometimes you should provide a type
	// Parameters:
	//     lpszCaption :  caption of the connection point to find.
	//
	// Returns:
	//     \Returns a reference to the FlowGraphConnectionPoint if found,
	//     \otherwise it will return Nothing\\Null.
	// ---------------------------------------------------------------------
	CXTPFlowGraphConnectionPoint* FindConnectionPointByCaption(LPCTSTR lpszCaption, XTPFlowGraphConnectionPointType type = xtpFlowGraphPointNone) const;

public:
	// ------------------------------------------------------------------------
	// Summary:
	//     Call this member function to Store/Load a collection of connection
	//     points using the specified data object.
	// Parameters:
	//     pPX :  Source or destination CXTPPropExchange data object reference.
	// Remarks:
	//     This member function is used to store or load a collection
	//     connection points data to or from a storage.
	// ------------------------------------------------------------------------
	void DoPropExchange(CXTPPropExchange* pPX);


protected:
	CXTPFlowGraphNode* m_pNode;  // Reference to the node that uses the connection points from this collection.

	CArray<CXTPFlowGraphConnectionPoint*, CXTPFlowGraphConnectionPoint*> m_arrConnectionPoints; // Collection of connection points.

	friend class CXTPFlowGraphNode;

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPFlowGraphConnectionPoints)
	DECLARE_ENUM_VARIANT(CXTPFlowGraphConnectionPoints)

	afx_msg int OleGetItemCount();
	afx_msg LPDISPATCH OleGetItem(int nIndex);
	afx_msg LPDISPATCH OleFindConnectionPoint(int nId);
	afx_msg LPDISPATCH OleFindConnectionPointByCaption(LPCTSTR lpszCaption, const VARIANT& vtType);

	afx_msg LPDISPATCH OleFindConnectionPointByName(LPCTSTR lpszName, const VARIANT& vtType);

//}}AFX_CODEJOCK_PRIVATE
#endif
};



AFX_INLINE int CXTPFlowGraphConnectionPoints::GetCount() const {
	return (int)m_arrConnectionPoints.GetSize();
}
AFX_INLINE CXTPFlowGraphConnectionPoint* CXTPFlowGraphConnectionPoints::GetAt(int nIndex) const {
	return nIndex >= 0 && nIndex < m_arrConnectionPoints.GetSize() ? m_arrConnectionPoints[nIndex] : NULL;
}

#endif //#if !defined(__XTPFLOWGRAPHCONNECTIONPOINTS_H__)
