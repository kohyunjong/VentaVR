// XTPMarkupGdiPlusDeviceContext.h: declaration of CXTPMarkupGdiPlusDeviceContext class.
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
#if !defined(__XTPMARKUPGDIPLUSDEVICECONTEXT_H__)
#define __XTPMARKUPGDIPLUSDEVICECONTEXT_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

struct IXTPMarkupDeviceDependentImageSite;
class CXTPMarkupTransformationMatrix;
class CXTPMarkupDeviceDependentImage;
class CXTPMarkupDedicatedDC;
class CXTPMarkupStrokeStyle;
class CXTPMarkupBrush;
class CXTPMarkupThickness;
class CXTPImageManagerIcon;
class CXTPMarkupFont;

class _XTP_EXT_CLASS CXTPMarkupGdiPlusDeviceContext : public CXTPMarkupDeviceContext
{
	class DedicatedDC;

public:
	CXTPMarkupGdiPlusDeviceContext(
		Gdiplus::GpGraphics* pGpGraphics,
		BOOL bSharedAccess = TRUE);
	virtual ~CXTPMarkupGdiPlusDeviceContext();

public:
	virtual void GetClipBox(LPRECT lpRect);
	virtual void IntersectClipRect(const RECT& rcLayoutClip);
	virtual HRGN SaveClipRegion();
	virtual void RestoreClipRegion(HRGN hRgn);
	virtual void OffsetViewport(const POINT& ptViewortOrg);
	virtual XTPMarkupSmoothingMode SetSmoothingMode(XTPMarkupSmoothingMode smoothingMode, BOOL bUseTextSmooting);
	virtual CXTPMarkupTransformationMatrix* GetTransformationMatrix() const;
	virtual void SetTransformationMatrix(CXTPMarkupTransformationMatrix* pMatrix);

	virtual CXTPMarkupDedicatedDC* GetDedicatedDC(const RECT& rect);
	virtual CXTPMarkupDeviceDependentImage* CreateDeviceDependentImage(IXTPMarkupDeviceDependentImageSite* pSite) const;

public:
	virtual void Ellipse(CRect rc, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);
	virtual void DrawRectangle(CRect rc, CXTPMarkupBrush* pBrush, CXTPMarkupThickness* pThickness);
	virtual void FillRectangle(CRect rc, CXTPMarkupBrush* pBrush);
	virtual void DrawLine(int x1, int y1, int x2, int y2, CXTPMarkupStrokeStyle* pStrokeStyle);
	virtual void Polyline(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle);
	virtual void Polygon(const POINT* points, int nCount, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);
	virtual void DrawPath(CXTPMarkupPathData *pData, CXTPMarkupStrokeStyle* pStrokeStyle, CXTPMarkupBrush* pFillBrush);

	virtual void DrawImage(CXTPImageManagerIcon* pIcon, CRect rc);
	virtual void DrawImage(CXTPMarkupDeviceDependentImage* pImage, CRect rc);

	virtual void SetTextFont(CXTPMarkupFont* pFont);
	virtual void SetTextColor(CXTPMarkupBrush* pBrush);
	virtual SIZE MeasureString(LPCWSTR lpszText, int nCount) const;
	virtual void DrawString(LPCWSTR lpszString, UINT nCount, LPCRECT lpRect);

	HDC GetDC();
	void ReleaseDC(HDC hDC);

public:
	static CXTPGdiPlus* GetGdiPlus();
	static void Register(BOOL bInit);

	static Gdiplus::GpGraphics* CreateGraphics(HDC hDC, HDC hAttribDC);
	Gdiplus::GpGraphics* GetGraphics() const;

protected:
	void Init(BOOL bReInit = FALSE);
	BOOL IsRTL() const;
	Gdiplus::Status GdipCreateBitmapFromHICON_Fixed(HICON hicon, Gdiplus::GpBitmap** bitmap) const;
	Gdiplus::GpFont* AccessFont();
	const Gdiplus::GpFont* AccessFont() const;
	void ApplySmoothingMode();
	Gdiplus::GpBrush* CreateGpBrush(CXTPMarkupBrush* pBrush, LPCRECT lpRect) const;
	Gdiplus::GpPen* CreateGpPen(CXTPMarkupStrokeStyle* pStrokeStyle, LPCRECT lpRect) const;
	CRect GetPointsBoundRect(const POINT* points, int nCount) const;
	BOOL IsGDIBrush(CXTPMarkupBrush* pBrush) const;
	Gdiplus::GpBitmap *IconToGdiplusBitmap(CXTPImageManagerIcon* pIcon) const;

protected:
	Gdiplus::PixelOffsetMode m_defaultPixelOffsetMode;
	Gdiplus::SmoothingMode m_gdipSmoothingMode;
	Gdiplus::InterpolationMode m_gdipInterpolationMode;
	Gdiplus::TextRenderingHint m_gdipTextRenderingHint;
	CXTPGdiPlus* m_pGdiPlus;
	BOOL m_bGpGraphicsSharedAccess;
	Gdiplus::GpGraphics* m_pGraphics;
	Gdiplus::GpFont* m_pGpFont;
	Gdiplus::GpBrush* m_pGpTextBrush;
	Gdiplus::GpStringFormat* m_pGpStringFormat;
	BOOL m_bRTL;
};

#endif // !defined(__XTPMARKUPGDIPLUSDEVICECONTEXT_H__)
