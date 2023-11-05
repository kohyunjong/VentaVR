//
// GdipButtonExt.cpp : Version 1.0 - see article at CodeProject.com
//
// Author:  Darren Sessions
//          
//
// Description:
//     GdipButtonExt is a CButton derived control that uses GDI+ 
//     to support alternate image formats
//
// History
//     Version 1.0 - 2008 June 10
//     - Initial public release
//
// License:
//     This software is released under the Code Project Open License (CPOL),
//     which may be found here:  http://www.codeproject.com/info/eula.aspx
//     You are free to use this software in any way you like, except that you 
//     may not sell this source code.
//
//     This software is provided "as is" with no expressed or implied warranty.
//     I accept no liability for any damage or loss of business that this 
//     software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GdipButtonExt.h"

#include "CGdiPlusBitmap.h"
#include "MemDC.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGdipButtonExt

CGdipButtonExt::CGdipButtonExt()
{
	m_pStdImage = NULL;
	m_pOverImage = NULL;
	m_pAltImage = NULL;
	m_pPressImage = NULL;

	m_bLoadFromFile = FALSE;
	m_bHaveBitmaps = FALSE;
	m_bHaveAltImage = FALSE;

	m_pCurBtn = NULL;

	m_bIsDisabled = FALSE;
	m_bIsToggle = FALSE;

	m_bIsHovering = FALSE;
	m_bIsTracking = FALSE;

	m_nCurType = STD_TYPE;

	m_pToolTip = NULL;

}

CGdipButtonExt::~CGdipButtonExt()
{
	if(m_pStdImage) delete m_pStdImage;
	if(m_pAltImage) delete m_pAltImage;
	if (m_pOverImage) delete m_pOverImage;
	if (m_pPressImage) delete m_pPressImage;

	if(m_pToolTip)	delete m_pToolTip;
}


BEGIN_MESSAGE_MAP(CGdipButtonExt, CButton)
	//{{AFX_MSG_MAP(CGdipButtonExt)
	ON_WM_DRAWITEM()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



//=============================================================================
//
// LoadStdImage()
//
// Purpose:     The LoadStdImage() Loads the image for the button.  This 
//				function must be called at a minimum or the button wont do 
//				anything.
//
// Parameters:  
//		[IN]	id
//				resource id, one of the resources already imported with the 
//				resource editor, usually begins with IDR_  
//
//		[IN]	pType
//				pointer to string describing the resource type
//				
// Returns:     BOOL
//				Non zero if successful, otherwise zero
//
//=============================================================================
BOOL CGdipButtonExt::LoadStdImage(UINT id, LPCTSTR pType)
{
	m_bLoadFromFile = FALSE;
	if (m_pStdImage)
		delete m_pStdImage;

	m_pStdImage = new CGdiPlusBitmapResource;
	return m_pStdImage->Load(id, pType);
}

BOOL CGdipButtonExt::LoadOvrImage(UINT id, LPCTSTR pType)
{
	m_pOverImage = new CGdiPlusBitmapResource;
	return m_pOverImage->Load(id, pType);
}

BOOL CGdipButtonExt::LoadPressImage(UINT id, LPCTSTR pType)
{
	m_pPressImage = new CGdiPlusBitmapResource;
	return m_pPressImage->Load(id, pType);
}

BOOL CGdipButtonExt::LoadImages(UINT stdid, UINT altid, UINT ovrid, UINT prsid, LPCTSTR pType)
{
	BOOL bRtn = FALSE;

	if (stdid != 0)
		bRtn = LoadStdImage(stdid, pType);
	if (altid != 0)
		bRtn = LoadAltImage(altid, pType);
	if (ovrid != 0)
		bRtn = LoadOvrImage(ovrid, pType);
	if (prsid != 0)
		bRtn = LoadPressImage(prsid, pType);

	return bRtn;
}

BOOL CGdipButtonExt::LoadAltImage(LPCWSTR pFile, LPCTSTR pType)
{
	m_bHaveAltImage = TRUE;
	m_pAltImage = new CGdiPlusBitmapResource;
	return m_pAltImage->Load(pFile);
}
BOOL CGdipButtonExt::LoadStdImage(LPCWSTR pFile, LPCTSTR pType)
{
	m_bLoadFromFile = TRUE;
	m_pStdImage = new CGdiPlusBitmapResource;
	return m_pStdImage->Load(pFile);
}
BOOL CGdipButtonExt::LoadOvrImage(LPCWSTR pFile, LPCTSTR pType)
{
	m_pOverImage = new CGdiPlusBitmapResource;
	return m_pOverImage->Load(pFile);
}
BOOL CGdipButtonExt::LoadPressImage(LPCWSTR pFile, LPCTSTR pType)
{
	m_pPressImage = new CGdiPlusBitmapResource;
	return m_pPressImage->Load(pFile);
}

BOOL CGdipButtonExt::LoadImages(LPCWSTR stdpFile, UINT altid, UINT ovrid, UINT prsid, LPCTSTR pType)
{
	BOOL bRtn = FALSE;
	ReLoadImages();
	if (stdpFile)
		bRtn = LoadStdImage(stdpFile, pType);
	if (altid != 0)
		bRtn = LoadAltImage(altid, pType);
	if (ovrid != 0)
		bRtn = LoadOvrImage(ovrid, pType);
	if (prsid != 0)
		bRtn = LoadPressImage(prsid, pType);

	return bRtn;
}
//=============================================================================
//
// LoadAltImage()
//
// Purpose:     The LoadAltImage() Loads the altername image for the button.  
//				This function call is optional
// Parameters:  
//		[IN]	id
//				resource id, one of the resources already imported with the 
//				resource editor, usually begins with IDR_  
//
//		[IN]	pType
//				pointer to string describing the resource type
//				
// Returns:     BOOL
//				Non zero if successful, otherwise zero
//
//=============================================================================
BOOL CGdipButtonExt::LoadAltImage(UINT id, LPCTSTR pType)
{
	m_bHaveAltImage = TRUE;
	m_pAltImage = new CGdiPlusBitmapResource;
	return (m_pAltImage->Load(id, pType));
}


//=============================================================================
//
//	The framework calls this member function when a child control is about to 
//	be drawn.  All the bitmaps are created here on the first call. Every thing
//	is done with a memory DC except the background, which get's it's information 
//	from the parent. The background is needed for transparent portions of PNG 
//	images. An always on top app (such as Task Manager) that is in the way can 
//	cause it to get an incorrect background.  To avoid this, the parent should 
//	call the SetBkGnd function with a memory DC when it creates the background.
//				
//=============================================================================
HBRUSH CGdipButtonExt::CtlColor(CDC* pScreenDC, UINT nCtlColor) 
{
	if(!m_bHaveBitmaps && !m_bIsDisabled)
	{
		if(!m_pStdImage)
		{
			return NULL; // Load the standard image with LoadStdImage()
		}

		CBitmap bmp, *pOldBitmap;

		CRect rect;
		GetClientRect(rect);

		// do everything with mem dc
		CMemoryDC pDC(pScreenDC, rect);

		Gdiplus::Graphics graphics(pDC->m_hDC);

		// background
		if (m_dcBk.m_hDC == NULL)
		{

			CRect rect1;
			CClientDC clDC(GetParent());
			GetWindowRect(rect1);
			GetParent()->ScreenToClient(rect1);

			m_dcBk.CreateCompatibleDC(&clDC);
			bmp.CreateCompatibleBitmap(&clDC, rect.Width(), rect.Height());
			pOldBitmap = m_dcBk.SelectObject(&bmp);
			m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), &clDC, rect1.left, rect1.top, SRCCOPY);
			bmp.DeleteObject();
		}

		// standard image
		if (m_dcStd.m_hDC == NULL)
		{
			PaintBk(pDC);

			if (m_bLoadFromFile)
			{
				RectF grect; grect.X = 0, grect.Y = 0; grect.Width = rect.Width(); grect.Height = rect.Height();
				graphics.DrawImage(*m_pStdImage, grect, 0, 0, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel);
			}
			else
				graphics.DrawImage(*m_pStdImage, 0, 0);
		
			m_dcStd.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
			pOldBitmap = m_dcStd.SelectObject(&bmp);
			m_dcStd.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
			bmp.DeleteObject();

			// standard image pressed
			if (m_dcStdP.m_hDC == NULL)
			{
				PaintBk(pDC);
				if (m_pPressImage)
					graphics.DrawImage(*m_pPressImage, 1, 1);
				else
				{
					if (m_bLoadFromFile)
					{
						ImageAttributes ia;

						ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
							0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
							0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
							0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
							0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

						ia.SetColorMatrix(&HotMat);

						RectF grect; grect.X = 0, grect.Y = 0; grect.Width = rect.Width(); grect.Height = rect.Height();
						graphics.DrawImage(*m_pStdImage, grect, 0, 0, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
						graphics.DrawImage(*m_pAltImage, grect, 0, 0, m_pAltImage->m_pBitmap->GetWidth(), m_pAltImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
					}
					else
						graphics.DrawImage(*m_pStdImage, 1, 1);
				}

				m_dcStdP.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdP.SelectObject(&bmp);
				m_dcStdP.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// standard image hot
			if(m_dcStdH.m_hDC == NULL)
			{
				ImageAttributes ia;

				PaintBk(pDC);

				ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
					0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
					0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

				ia.SetColorMatrix(&HotMat);

				float width = (float)m_pStdImage->m_pBitmap->GetWidth();
				float height = (float)m_pStdImage->m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;

				if (m_pOverImage != NULL)
					graphics.DrawImage(*m_pOverImage, grect, 0, 0, width, height, UnitPixel);
				else
				{
					if (m_bLoadFromFile)
					{
						RectF cgrect; cgrect.X = 0, cgrect.Y = 0; cgrect.Width = rect.Width(); cgrect.Height = rect.Height();
						graphics.DrawImage(*m_pStdImage, cgrect, 0, 0, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
						graphics.DrawImage(*m_pAltImage, cgrect, 0, 0, m_pAltImage->m_pBitmap->GetWidth(), m_pAltImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
					}
					else
						graphics.DrawImage(*m_pStdImage, grect, 0, 0, width, height, UnitPixel, &ia);
				}
					
				m_dcStdH.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcStdH.SelectObject(&bmp);
				m_dcStdH.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// grayscale image
			if(m_dcGS.m_hDC == NULL)
			{
				PaintBk(pDC);

				ColorMatrix GrayMat = {	0.30f, 0.30f, 0.30f, 0.00f, 0.00f,
										0.59f, 0.59f, 0.59f, 0.00f, 0.00f,
										0.11f, 0.11f, 0.11f, 0.00f, 0.00f,
										0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
										0.00f, 0.00f, 0.00f, 0.00f, 1.00f	};

				ImageAttributes ia;
				ia.SetColorMatrix(&GrayMat);

				float width = (float)m_pStdImage->m_pBitmap->GetWidth();
				float height = (float)m_pStdImage->m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;
				if (m_bLoadFromFile)
				{
					RectF cgrect; cgrect.X = 0, cgrect.Y = 0; cgrect.Width = rect.Width(); cgrect.Height = rect.Height();
					graphics.DrawImage(*m_pStdImage, cgrect, 0, 0, width, height, UnitPixel, &ia);
				}
				else
					graphics.DrawImage(*m_pStdImage, grect, 0, 0, width, height, UnitPixel, &ia);

				m_dcGS.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcGS.SelectObject(&bmp);
				m_dcGS.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

				bmp.DeleteObject();
			}
		}

		// alternate image
		if( (m_dcAlt.m_hDC == NULL) && m_bHaveAltImage )
		{
			PaintBk(pDC);
			if (m_bLoadFromFile)
			{
				ImageAttributes ia;
				ia.SetColorKey(Color(255, 255, 255), Color(255, 255, 255), ColorAdjustTypeBitmap);
				ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
					0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
					0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

				ia.SetColorMatrix(&HotMat);

				RectF cgrect; cgrect.X = 0, cgrect.Y = 0; cgrect.Width = rect.Width(); cgrect.Height = rect.Height();
				graphics.DrawImage(*m_pStdImage, cgrect, 0, 0, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel);
				graphics.DrawImage(*m_pAltImage, cgrect, 0, 0, m_pAltImage->m_pBitmap->GetWidth(), m_pAltImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
			}
			else
				graphics.DrawImage(*m_pAltImage, 0, 0);
		
			m_dcAlt.CreateCompatibleDC(pDC);
			bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
			pOldBitmap = m_dcAlt.SelectObject(&bmp);
			m_dcAlt.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
			bmp.DeleteObject();

			// alternate image pressed
			if( (m_dcAltP.m_hDC == NULL) && m_bHaveAltImage )
			{
				ImageAttributes ia;
				ia.SetColorKey(Color(255, 255, 255), Color(255, 255, 255), ColorAdjustTypeBitmap);
				PaintBk(pDC);

				ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
					0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
					0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
					0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
					0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

				ia.SetColorMatrix(&HotMat);

				if (m_pPressImage)
				{
					RectF cgrect; cgrect.X = 0, cgrect.Y = 0; cgrect.Width = rect.Width(); cgrect.Height = rect.Height();
					graphics.DrawImage(*m_pStdImage, cgrect, 1, 1, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
					graphics.DrawImage(*m_pPressImage, cgrect, 1, 1, m_pPressImage->m_pBitmap->GetWidth(), m_pPressImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
				}
				else
				{
					if (m_bLoadFromFile)
					{
						RectF cgrect; cgrect.X = 0, cgrect.Y = 0; cgrect.Width = rect.Width(); cgrect.Height = rect.Height();
						graphics.DrawImage(*m_pStdImage, cgrect, 1, 1, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
						graphics.DrawImage(*m_pAltImage, cgrect, 1, 1, m_pAltImage->m_pBitmap->GetWidth(), m_pAltImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
					}
					else
						graphics.DrawImage(*m_pAltImage, 1, 1);
				}
			
				m_dcAltP.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcAltP.SelectObject(&bmp);
				m_dcAltP.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}

			// alternate image hot
			if (m_dcAltH.m_hDC == NULL)
			{
				ImageAttributes ia;
				ia.SetColorKey(Color(255, 255, 255), Color(255, 255, 255), ColorAdjustTypeBitmap);
				PaintBk(pDC);
					ColorMatrix HotMat = { 1.05f, 0.00f, 0.00f, 0.00f, 0.00f,
						0.00f, 1.05f, 0.00f, 0.00f, 0.00f,
						0.00f, 0.00f, 1.05f, 0.00f, 0.00f,
						0.00f, 0.00f, 0.00f, 1.00f, 0.00f,
						0.05f, 0.05f, 0.05f, 0.00f, 1.00f };

					ia.SetColorMatrix(&HotMat);

				float width = (float)m_pStdImage->m_pBitmap->GetWidth();
				float height = (float)m_pStdImage->m_pBitmap->GetHeight();

				RectF grect; grect.X=0, grect.Y=0; grect.Width = width; grect.Height = height;
				if (m_pOverImage != NULL)
					graphics.DrawImage(*m_pOverImage, grect, 0, 0, width, height, UnitPixel, &ia);
				else
				{
					if (m_bLoadFromFile)
					{
						RectF cgrect; cgrect.X = 0, cgrect.Y = 0; cgrect.Width = rect.Width() ; cgrect.Height = rect.Height() ;
						graphics.DrawImage(*m_pStdImage, cgrect, 0, 0, m_pStdImage->m_pBitmap->GetWidth(), m_pStdImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
						graphics.DrawImage(*m_pAltImage, cgrect, 0, 0, m_pAltImage->m_pBitmap->GetWidth(), m_pAltImage->m_pBitmap->GetHeight(), UnitPixel, &ia);
					}
					else
						graphics.DrawImage(*m_pAltImage, grect, 0, 0, width, height, UnitPixel, &ia);
				}
				m_dcAltH.CreateCompatibleDC(pDC);
				bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
				pOldBitmap = m_dcAltH.SelectObject(&bmp);
				m_dcAltH.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);
				bmp.DeleteObject();
			}
		}

		if(m_pCurBtn == NULL)
		{
			m_pCurBtn = &m_dcStd;
		}

		m_bHaveBitmaps = TRUE;
	}

	return NULL;
}

//=============================================================================
// paint the background
//=============================================================================
void CGdipButtonExt::PaintBk(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &m_dcBk, 0, 0, SRCCOPY);
}

//=============================================================================
// paint the bitmap currently pointed to with m_pCurBtn
//=============================================================================
void CGdipButtonExt::PaintBtn(CDC *pDC)
{
	CRect rect;
	GetClientRect(rect);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), m_pCurBtn, 0, 0, SRCCOPY);
}

//=============================================================================
// enables the toggle mode
// returns if it doesn't have the alternate image
//=============================================================================
void CGdipButtonExt::EnableToggle(BOOL bEnable)
{
	if(!m_bHaveAltImage) return;

	m_bIsToggle = bEnable; 

	// this actually makes it start in the std state since toggle is called before paint
	if(bEnable)	m_pCurBtn = &m_dcAlt;
	else		m_pCurBtn = &m_dcStd;

}

//=============================================================================
// sets the image type and disabled state then repaints
//=============================================================================
void CGdipButtonExt::SetImage(int type)
{
	m_nCurType = type;

	(type == DIS_TYPE) ? m_bIsDisabled = TRUE : m_bIsDisabled = FALSE;

	Invalidate();
}

//=============================================================================
// set the control to owner draw
//=============================================================================
void CGdipButtonExt::PreSubclassWindow()
{
	// Set control to owner draw
	ModifyStyle(0, BS_OWNERDRAW, SWP_FRAMECHANGED);

	CButton::PreSubclassWindow();
}

//=============================================================================
// disable double click 
//=============================================================================
BOOL CGdipButtonExt::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_LBUTTONDBLCLK)
		pMsg->message = WM_LBUTTONDOWN;

	if (m_pToolTip != NULL)
	{
		if (::IsWindow(m_pToolTip->m_hWnd))
		{
			m_pToolTip->RelayEvent(pMsg);		
		}
	}

	return CButton::PreTranslateMessage(pMsg);
}

void CGdipButtonExt::ReLoadImages()
{
	m_dcBk.DeleteDC();			// button background
	m_dcBk.m_hDC = NULL;
	m_dcStd.DeleteDC();		// standard button
	m_dcStd.m_hDC = NULL;
	m_dcStdP.DeleteDC();	// standard button pressed
	m_dcStdP.m_hDC = NULL;
	m_dcStdH.DeleteDC();	// standard button hot
	m_dcStdH.m_hDC = NULL;
	m_dcAlt.DeleteDC();		// alternate button
	m_dcAlt.m_hDC = NULL;
	m_dcAltP.DeleteDC();		// alternate button pressed
	m_dcAltP.m_hDC = NULL;
	m_dcAltH.DeleteDC();		// alternate button hot
	m_dcAltH.m_hDC = NULL;

	m_dcGS.DeleteDC();			// grayscale button (does not have a hot or pressed state)
	m_dcGS.m_hDC = NULL;
	m_bHaveBitmaps = FALSE;
}
//=============================================================================
// overide the erase function
//=============================================================================
BOOL CGdipButtonExt::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

//=============================================================================
// Paint the button depending on the state of the mouse
//=============================================================================
void CGdipButtonExt::DrawItem(LPDRAWITEMSTRUCT lpDIS) 
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);

	// handle disabled state
	if(m_bIsDisabled)
	{
		m_pCurBtn = &m_dcGS;
		PaintBtn(pDC);
		return;
	}

	BOOL bIsPressed = (lpDIS->itemState & ODS_SELECTED);

	// handle toggle button
	if(m_bIsToggle && bIsPressed)
	{
		(m_nCurType == STD_TYPE) ? m_nCurType = ALT_TYPE : m_nCurType = STD_TYPE;
	}

	if(bIsPressed)
	{
		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStdP;
		else
			m_pCurBtn = &m_dcAltP;
	}
	else if(m_bIsHovering)
	{

		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStdH;
		else
			m_pCurBtn = &m_dcAltH;
	}
	else
	{
		if(m_nCurType == STD_TYPE)
			m_pCurBtn = &m_dcStd;
		else
			m_pCurBtn = &m_dcAlt;
	}

	// paint the button
	PaintBtn(pDC);
}

//=============================================================================
LRESULT CGdipButtonExt::OnMouseHover(WPARAM wparam, LPARAM lparam) 
//=============================================================================
{
	m_bIsHovering = TRUE;
	if (m_bIsDisabled)
		return 0;
	Invalidate();
	DeleteToolTip();

	// Create a new Tooltip with new Button Size and Location
	SetToolTipText(m_tooltext);

	if (m_pToolTip != NULL)
	{
		if (::IsWindow(m_pToolTip->m_hWnd))
		{
			//Display ToolTip
			m_pToolTip->Update();
		}
	}

	return 0;
}


//=============================================================================
LRESULT CGdipButtonExt::OnMouseLeave(WPARAM wparam, LPARAM lparam)
//=============================================================================
{
	m_bIsTracking = FALSE;
	m_bIsHovering = FALSE;
	if (!m_bIsDisabled)
		Invalidate();
	return 0;
}

//=============================================================================
void CGdipButtonExt::OnMouseMove(UINT nFlags, CPoint point) 
//=============================================================================
{
	if (!m_bIsTracking && !m_bIsDisabled)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bIsTracking = _TrackMouseEvent(&tme);
	}
	
	CButton::OnMouseMove(nFlags, point);
}

//=============================================================================
//	
//	Call this member function with a memory DC from the code that paints 
//	the parents background.  Passing the screen DC defeats the purpose of 
//  using this function.
//
//=============================================================================
void CGdipButtonExt::SetBkGnd(CDC* pDC)
{
	CRect rect, rectS;
	CBitmap bmp, *pOldBitmap;

	GetClientRect(rect);
	GetWindowRect(rectS);
	GetParent()->ScreenToClient(rectS);

	m_dcBk.DeleteDC();

	m_dcBk.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = m_dcBk.SelectObject(&bmp);
	m_dcBk.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, rectS.left, rectS.top, SRCCOPY);
	bmp.DeleteObject();
}


//=============================================================================
// Set the tooltip with a string resource
//=============================================================================
void CGdipButtonExt::SetToolTipText(UINT nId, BOOL bActivate)
{
	// load string resource
	m_tooltext.LoadString(nId);

	// If string resource is not empty
	if (m_tooltext.IsEmpty() == FALSE)
	{
		SetToolTipText(m_tooltext, bActivate);
	}

}

//=============================================================================
// Set the tooltip with a CString
//=============================================================================
void CGdipButtonExt::SetToolTipText(CString spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText.IsEmpty()) return;

	// Initialize ToolTip
	InitToolTip();
	m_tooltext = spText;

	// If there is no tooltip defined then add it
	if (m_pToolTip->GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_pToolTip->AddTool(this, m_tooltext, rectBtn, 1);
	}

	// Set text for tooltip
	m_pToolTip->UpdateTipText(m_tooltext, this, 1);
	m_pToolTip->SetDelayTime(2000);
	m_pToolTip->Activate(bActivate);
}

//=============================================================================
void CGdipButtonExt::InitToolTip()
//=============================================================================
{
	if (m_pToolTip == NULL)
	{
		m_pToolTip = new CToolTipCtrl;
		// Create ToolTip control
		m_pToolTip->Create(this);
		m_pToolTip->Activate(TRUE);
	}
} 

//=============================================================================
void CGdipButtonExt::DeleteToolTip()
//=============================================================================
{
	// Destroy Tooltip incase the size of the button has changed.
	if (m_pToolTip != NULL)
	{
		delete m_pToolTip;
		m_pToolTip = NULL;
	}
}

void CGdipButtonExt::SizeToContent()
{
	CRect rect;
	GetClientRect(&rect);
	if (m_pStdImage)
	{
		float width = (float)m_pStdImage->m_pBitmap->GetWidth();
		float height = (float)m_pStdImage->m_pBitmap->GetHeight();
		SetWindowPos(NULL, 0, 0, width, height,SWP_NOMOVE | SWP_SHOWWINDOW);
	}
}