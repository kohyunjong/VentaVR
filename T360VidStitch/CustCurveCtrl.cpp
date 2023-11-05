// CustCurveCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "CustCurveCtrl.h"
#include "CurvePaneDlg.h"
//#include "CustomCtlTestDlg.h"

// CustCurveCtrl

IMPLEMENT_DYNAMIC(CustCurveCtrl, CWnd)

CustCurveCtrl::CustCurveCtrl()
{
	RegisterWindowClass();
	EnableAutomation();
	m_PointIndex = -1;
	m_bReverse = FALSE;
	m_LineColor = RGB(0, 0, 0);
	m_ControlSizeX = 0;
	m_ControlSizeY = 0;
	m_bSelectPoint = TRUE;
	m_bFirstImageSelect = FALSE;
}

CustCurveCtrl::~CustCurveCtrl()
{
}

void CustCurveCtrl::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CWnd::OnFinalRelease();
}

// Register the window class if it has not already been registered.
BOOL CustCurveCtrl::RegisterWindowClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, CUSTCURVECTL_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = CUSTCURVECTL_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(CustCurveCtrl, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(CustCurveCtrl, CWnd)
END_DISPATCH_MAP()

// Note: we add support for IID_IustCurveCtrl to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {B392768A-F3BE-4870-8AE7-00738EB20EC7}
static const IID IID_IustCurveCtrl =
{ 0xB392768A, 0xF3BE, 0x4870, { 0x8A, 0xE7, 0x0, 0x73, 0x8E, 0xB2, 0xE, 0xC7 } };

BEGIN_INTERFACE_MAP(CustCurveCtrl, CWnd)
	INTERFACE_PART(CustCurveCtrl, IID_IustCurveCtrl, Dispatch)
END_INTERFACE_MAP()


// CustCurveCtrl message handlers

BOOL CustCurveCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default
	//Test Code..
	//InitGraphData();
	//SetReverseGraph();
	UpdateDialog(pDC->m_hDC);
	return TRUE;//CWnd::OnEraseBkgnd(pDC);
}


void CustCurveCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages
}



BOOL CustCurveCtrl::Create(CWnd* pParentWnd, const RECT& rect, UINT nID, DWORD dwStyle /*=WS_VISIBLE*/)
{
	return CWnd::Create(CUSTCURVECTL_CLASSNAME, _T(""), dwStyle, rect, pParentWnd, nID);
}
void CustCurveCtrl::UpdateCurvePoint(CPoint point)
{
	int index = IsInsideControlPoint(point);
	CCurvePaneDlg * pDlg = (CCurvePaneDlg*)GetParent();
	m_PointIndex = index;		// -1 일때는 선택 안됨
	if (index >= 1 && index < 4)
	{
		//we are moving a control point around
		pDlg->GetControlValue(index);


		m_bSelectPoint = TRUE;
		//		m_InputValue = (255.0 / m_ControlSizeX) * point.x;
		//		m_OutputValue = (255.0 / m_ControlSizeY) * point.y;

		pDlg->UpdateIOEditCtrl(index);
	}
	else
	{
		m_bSelectPoint = FALSE;
		(pDlg->GetDlgItem(IDC_EDIT_INPUT))->EnableWindow(FALSE);
		(pDlg->GetDlgItem(IDC_EDIT_OUTPUT))->EnableWindow(FALSE);
	}
	pDlg->UpdateData();
}



void CustCurveCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//check if the mouse left clicking into one of the control points
	UpdateCurvePoint(point);
	UpdateControl();
	CWnd::OnLButtonDown(nFlags, point);
}

void CustCurveCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CWnd::OnMouseMove(nFlags, point);
}

void CustCurveCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
//	m_MoveIndex = -1;
	CWnd::OnLButtonUp(nFlags, point);
}

void CustCurveCtrl::SetReverseGraph()
{
	int count = m_ControlPoints.GetSize();
	CPoint point1, point3;
	int nTempy;
	if (count > 0)
	{
		m_bReverse = !m_bReverse;
		point1 = m_ControlPoints[0];
		point3 = m_ControlPoints[2];

		nTempy = point1.y;
		point1.y = point3.y;
		point3.y = nTempy;

		SetPoint(0, point1);
		SetPoint(2, point3);
	}
}

void CustCurveCtrl::InitGraphData()
{
	CDC* pDC = GetDC();
	RECT rect;
	CPoint point1, point2, point3 ,point4,point5;
	int count = m_ControlPoints.GetSize();

	GetClientRect(&rect);
#ifdef _DEBUG
	TRACE("%d, %d, width %d, hight %d\n", rect.left, rect.top, rect.right, rect.bottom);
#endif

	if (!m_bReverse)
	{
// 		point1.x = rect.left + GRAPH_GAP ;
// 		point1.y = rect.bottom - GRAPH_GAP ;
// 		point2.x = rect.right / 2;
// 		point2.y = rect.bottom / 2;
// 		point3.x = rect.right - GRAPH_GAP ;
// 		point3.y = rect.top + GRAPH_GAP ;
	}
	else
	{
		point1.x = rect.left + GRAPH_GAP;
		point1.y = rect.top + GRAPH_GAP;
		point2.x = rect.right / 2;
		point2.y = rect.bottom / 2;
		point3.x = rect.right - GRAPH_GAP;
		point3.y = rect.bottom - GRAPH_GAP;
	}

	if (count < 4)
	{
		AddCurvePoint(point1);
		AddCurvePoint(point2);
		AddCurvePoint(point3);
		AddCurvePoint(point4);
		AddCurvePoint(point5);
	}

	UpdateDialog(pDC->m_hDC);

	ReleaseDC(pDC);
}

void CustCurveCtrl::AddCurvePoint(CPoint point)
{
	m_ControlPoints.Add(point);
	if (m_ControlPoints.GetSize()>1)
	{
		//create a spline object
		Spline spline(m_ControlPoints.GetData(), m_ControlPoints.GetSize());
		//generate a curve
		spline.Generate();
		//get the curve points number
		m_CurvePoints.SetSize(spline.GetCurveCount());
		//get the points number
		int PointCount = 0;
		spline.GetCurve(m_CurvePoints.GetData(), PointCount);
	}
}

void CustCurveCtrl::UpdateControl()
{
	CDC* pDC = GetDC();
	UpdateDialog(pDC->m_hDC);
}

void CustCurveCtrl::UpdateDialog(HDC& hDC)
{
	//brush the DC
	RECT rect;
	HPEN hPen, oldPen;
	CDC *pDC = CDC::FromHandle(hDC);

	GetClientRect(&rect);

	DrawGrid(pDC, GRID_COLOR);
	//draw the control points


	for (int i = 0; i<m_ControlPoints.GetSize(); i++)
		DrawTracker(m_ControlPoints[i], SPOT_COLOR, 3, hDC);

	if (m_bSelectPoint && m_PointIndex != -1)
		DrawTracker(m_ControlPoints[m_PointIndex], SELECT_SPOT_COLOR, 3, hDC);


	//draw the curve
	hPen = CreatePen(PS_SOLID, 1, m_LineColor);
	oldPen = (HPEN)SelectObject(hDC, hPen);

	if (m_CurvePoints.GetSize()>1)
		Polyline(hDC, m_CurvePoints.GetData(), m_CurvePoints.GetSize());

	SelectObject(hDC, oldPen);
	DeleteObject(hPen);
}

void CustCurveCtrl::SetMiddlePoint(CPoint point)
{
	SetPoint(1, point);
}

void CustCurveCtrl::SetPoint(int nIndex, CPoint point)
{
	int count = m_ControlPoints.GetSize();

	if (count>2)
	{
		m_ControlPoints[nIndex] = point;

		//create a spline object
		Spline spline(m_ControlPoints.GetData(), m_ControlPoints.GetSize());
		//generate a curve
		spline.Generate();
		//get the curve points number
		m_CurvePoints.SetSize(spline.GetCurveCount());
		//get the points number
		int PointCount = 0;
		spline.GetCurve(m_CurvePoints.GetData(), PointCount);
	}
}

int CustCurveCtrl::IsInsideControlPoint(POINT& point)
{
	int count = m_ControlPoints.GetSize();

	if (count>0)
	{
		for (int i = 0; i<count; i++)
		{
			if (Distance(m_ControlPoints[i], point)<5)
				return i;
		}
	}
	return -1;
}


double CustCurveCtrl::Distance(const POINT& p1, const POINT& p2)
{
	int dx = abs(p1.x - p2.x);
	int dy = abs(p1.y - p2.y);

	return sqrt((double)(dx*dx + dy*dy));
}

void CustCurveCtrl::DrawTracker(POINT point, COLORREF rgbColor, int extent, HDC hDC)			// 포인터를 그림
{
	CBrush backBrush;
	CDC *pDC = CDC::FromHandle(hDC);

	CBrush* pOldBrush;
	backBrush.CreateSolidBrush(rgbColor);
	pOldBrush = pDC->SelectObject(&backBrush);

	pDC->PatBlt(point.x - extent, point.y - extent, extent * 2 + 1, extent * 2 + 1, PATCOPY /*DSTINVERT*/ );

	pDC->SelectObject(pOldBrush);
}

void CustCurveCtrl::DrawGrid(CDC* pDC, COLORREF rgbColor)
{
	CRect rect;
	GetClientRect(&rect);
	FillRect(pDC->GetSafeHdc(), &rect, (HBRUSH)GetStockObject(BG_COLOR));

	// Center lines
	CPen penDash;
	penDash.CreatePen(PS_SOLID, 1, rgbColor);
	CPen* pOldPen = pDC->SelectObject(&penDash);

	// Major unit lines
	CPen penDot;
	penDot.CreatePen(PS_SOLID, 1, rgbColor);
	pDC->SelectObject(&penDot);

	m_ControlSizeX = rect.right;		// X축 눈금자를 9등분하기 위한 변수
	m_ControlSizeY = rect.bottom;		// Y축 눈금자를 9등분하기 위한 변수

	int GirdSizeX = m_ControlSizeX / 4;
	int GridSizeY = m_ControlSizeY / 4;

	for (int x = rect.left / GirdSizeX * GirdSizeX; x < rect.right; x += GirdSizeX)
	{
		if (x != 0)
		{
			pDC->MoveTo(x, rect.top);
			pDC->LineTo(x, rect.bottom);
		}
	}

	for (int y = rect.top / GridSizeY * GridSizeY; y < rect.bottom; y += GridSizeY)
	{
		if (y != 0)
		{
			pDC->MoveTo(rect.left, y);
			pDC->LineTo(rect.right, y);
		}
	}
	// Outlines
	CPen penSolid;
	penSolid.CreatePen(PS_SOLID, 1, rgbColor);
	pDC->SelectObject(&penSolid);
	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right - 1, rect.top);
	pDC->LineTo(rect.right - 1, rect.bottom - 1);
	pDC->LineTo(rect.left, rect.bottom - 1);
	pDC->LineTo(rect.left, rect.top);

	pDC->SelectObject(pOldPen);
}

void CustCurveCtrl::IOUpdate(double Input, double Output)		// 선택된 포인트만 업데이트
{
	if (m_bSelectPoint && m_PointIndex != -1)
	{
		m_InputValue = Input;
		m_OutputValue = Output;

		m_ControlPoints[m_PointIndex].x = m_InputValue / (255.0 / m_ControlSizeX);
		m_ControlPoints[m_PointIndex].y = m_OutputValue / (255.0 / m_ControlSizeY);
		m_ControlPoints[m_PointIndex].y = 255 / (255.0 / m_ControlSizeY) - m_ControlPoints[m_PointIndex].y;		//  Y축 반전 
		
		CurveSpline(m_ControlPoints[m_PointIndex]);
		UpdateControl();
	}
}

void CustCurveCtrl::IOUpdate(double Input, double Output, int pointIndex)		// 포인트 한번에 업데이트 할때 사용
{

	m_InputValue = Input;
	m_OutputValue = Output;

	m_ControlPoints[pointIndex].x = m_InputValue / (255.0 / m_ControlSizeX);
	m_ControlPoints[pointIndex].y = m_OutputValue / (255.0 / m_ControlSizeY);
	m_ControlPoints[pointIndex].y = 255 / (255.0 / m_ControlSizeY) - m_ControlPoints[pointIndex].y;			//  Y축 반전 
	m_PointIndex = pointIndex;
	CurveSpline(m_ControlPoints[pointIndex]);
	UpdateControl();

}



void CustCurveCtrl::CurveSpline(CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	
	int MoveState = 0;			//

	if (m_PointIndex == 1)			// 왼쪽 포인터 클릭
	{
	//		m_ControlPoints[m_MoveIndex] = point;

		if (m_ControlPoints.GetSize() > 1)
		{

			Spline spline(m_ControlPoints.GetData(), m_ControlPoints.GetSize());
			spline.Generate();

			m_CurvePoints.SetSize(spline.GetCurveCount());
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints.GetData(), PointCount);
		}
		CDC* pDC = GetDC();
		UpdateDialog(pDC->m_hDC);
		ReleaseDC(pDC);
	}
	if (m_PointIndex == 3)					// 오른쪽 포인터 클릭
	{
	//		m_ControlPoints[m_MoveIndex] = point;

		if (m_ControlPoints.GetSize() > 1)
		{
			Spline spline(m_ControlPoints.GetData(), m_ControlPoints.GetSize());
			spline.Generate();

			m_CurvePoints.SetSize(spline.GetCurveCount());
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints.GetData(), PointCount);
		}
		CDC* pDC = GetDC();
		UpdateDialog(pDC->m_hDC);
		ReleaseDC(pDC);
	}
	if (m_PointIndex == 2)				// 가운데 포인터 클릭
	{
	m_ControlPoints[m_PointIndex] = point;

		if (m_ControlPoints.GetSize()>1)
		{
			Spline spline(m_ControlPoints.GetData(), m_ControlPoints.GetSize());
			spline.Generate();

			m_CurvePoints.SetSize(spline.GetCurveCount());
			int PointCount = 0;
			spline.GetCurve(m_CurvePoints.GetData(), PointCount);
		}
		CDC* pDC = GetDC();
		UpdateDialog(pDC->m_hDC);
		ReleaseDC(pDC);
	}
}
