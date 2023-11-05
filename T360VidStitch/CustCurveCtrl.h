#pragma once

#include <Afxtempl.h>
#include "curvegraph.h"
#include "resource.h" 



#define CUSTCURVECTL_CLASSNAME    _T("MFCCustCurveCtrl")  // Window class name
// CustCurveCtrl

#define LINE_COLOR			RGB(255,0,0)
#define SPOT_COLOR			RGB(0,0,124)
#define SELECT_SPOT_COLOR	RGB(255,80,125)
#define GRID_COLOR			RGB(211,211,211)

#define COLOR_RED		0
#define COLOR_GREEN		1
#define COLOR_BLUE		2

#define BG_COLOR	RGB(250,250,250)
#define GRAPH_GAP	0



class CustCurveCtrl : public CWnd
{
	DECLARE_DYNAMIC(CustCurveCtrl)

public:
	CustCurveCtrl();
	virtual ~CustCurveCtrl();

	virtual void OnFinalRelease();
	// Operations

public:
	int m_PointIndex;				// 1,2,3 각각 왼쪽 중간 오른쪽 포인터
	BOOL m_bReverse;
	COLORREF m_LineColor;
	int m_iChannel;					// 0,1,2 각각 Red ,Green , Blue

public:
	CArray<POINT, POINT> m_ControlPoints;
	CArray<POINT, POINT> m_CurvePoints;

public:
	BOOL Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle = WS_VISIBLE);
	void UpdateControl();
	int IsInsideControlPoint(POINT& point);
	void InitGraphData();
	void SetMiddlePoint(CPoint point);
	void SetReverseGraph();
	double m_ControlSizeX;		//커브 컨트롤러의 크기 좌표 X
	double m_ControlSizeY;		//커브 컨트롤러의 크기 좌표 Y
	double m_InputValue;
	double m_OutputValue;
	BOOL m_bFirstImageSelect;
	BOOL m_bSelectPoint;		// 현재 선택된 포인트

protected:
	BOOL RegisterWindowClass();
	void UpdateDialog(HDC&   hDC);
	double Distance(const POINT& p1, const POINT& p2);
	void DrawTracker(POINT point, COLORREF rgbColor, int extent, HDC hDC);
	void AddCurvePoint(CPoint point);
	void DrawGrid(CDC* pDC, COLORREF rgbColor);
	void SetPoint(int nIndex, CPoint point);
protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	void UpdateCurvePoint(CPoint point);
	void IOUpdate(double Input ,double Output);
	void IOUpdate(double Input, double Output, int pointIndex);
	void CurveSpline( CPoint point);

};


