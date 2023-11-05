//
// GdipButtonExt.h : Version 1.0 - see article at CodeProject.com
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

#pragma once

// GdipButtonExt.h : header file
//

class CGdiPlusBitmapResource;
/////////////////////////////////////////////////////////////////////////////
// CGdipButtonExt window

class CGdipButtonExt : public CButton
{
public:

	CGdipButtonExt();
	virtual ~CGdipButtonExt();

	// image types
	enum	{
				STD_TYPE	= 0,
				ALT_TYPE,
				DIS_TYPE
			};

	// sets the image type
	void SetImage(int type);

	BOOL LoadAltImage(UINT id, LPCTSTR pType);
	BOOL LoadStdImage(UINT id, LPCTSTR pType);
	BOOL LoadOvrImage(UINT id, LPCTSTR pType);
	BOOL LoadPressImage(UINT id, LPCTSTR pType);
	BOOL LoadImages(UINT stdid, UINT altid, UINT ovrid, UINT prsid, LPCTSTR pType);
	
	BOOL LoadAltImage(LPCWSTR pFile, LPCTSTR pType);
	BOOL LoadStdImage(LPCWSTR pFile, LPCTSTR pType);
	BOOL LoadOvrImage(LPCWSTR pFile, LPCTSTR pType);
	BOOL LoadPressImage(LPCWSTR pFile, LPCTSTR pType);
	BOOL LoadImages(LPCWSTR stdpFile, UINT altid, UINT ovrid, UINT prsid, LPCTSTR pType);

	// if false, disables the press state and uses grayscale image if it exists
	void EnableButton(BOOL bEnable = TRUE) { m_bIsDisabled = !bEnable; }

	// in toggle mode each press toggles between std and alt images
	void EnableToggle(BOOL bEnable = TRUE);

	// return the enable/disable state
	BOOL IsDisabled(void) {return (m_bIsDisabled == TRUE); }

	void SetBkGnd(CDC* pDC);

	void SetToolTipText(CString spText, BOOL bActivate = TRUE);
	void SetToolTipText(UINT nId, BOOL bActivate = TRUE);
	void SetHorizontal(bool ImagesAreLaidOutHorizontally = FALSE);
	void DeleteToolTip();
	void SizeToContent();

protected:

	void PaintBk(CDC* pDC);
	void PaintBtn(CDC* pDC);

	BOOL	m_bLoadFromFile;
	BOOL	m_bHaveAltImage;
	BOOL	m_bHaveBitmaps;

	BOOL	m_bIsDisabled;
	BOOL	m_bIsToggle;
	BOOL	m_bIsHovering;
	BOOL	m_bIsTracking;

	int		m_nCurType;

	CGdiPlusBitmapResource* m_pAltImage;
	CGdiPlusBitmapResource* m_pOverImage;
	CGdiPlusBitmapResource* m_pStdImage;
	CGdiPlusBitmapResource* m_pPressImage;

	CString			m_tooltext;
	CToolTipCtrl*	m_pToolTip;
	
	void	InitToolTip();
	void	ReLoadImages();

	virtual void PreSubclassWindow();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//{{AFX_MSG(CGdipButtonExt)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnMouseHover(WPARAM wparam, LPARAM lparam) ;
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:

	CDC		m_dcBk;			// button background
	
	CDC		m_dcStd;		// standard button
	CDC		m_dcStdP;		// standard button pressed
	CDC		m_dcStdH;		// standard button hot

	CDC		m_dcAlt;		// alternate button
	CDC		m_dcAltP;		// alternate button pressed
	CDC		m_dcAltH;		// alternate button hot

	CDC		m_dcGS;			// grayscale button (does not have a hot or pressed state)

	CDC*	m_pCurBtn;		// current pointer to one of the above

};
