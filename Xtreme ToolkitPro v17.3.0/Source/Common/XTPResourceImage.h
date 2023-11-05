// XTPResourceImage.h: interface for the CXTPResourceImage class.
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
#if !defined(__XTPRESOURCEIMAGE_H__)
#define __XTPRESOURCEIMAGE_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define XTP_DI_COMPOSITE 1

class CXTPResourceImages;
class CXTPResourceImageList;
class CXTPNotifyConnection;
class CXTPBitmapDC;

class _XTP_EXT_CLASS CXTPResourceImage : public CXTPCmdTarget
{
public:
	CXTPResourceImage(CXTPResourceImages* pImages);
	virtual ~CXTPResourceImage();

public:
	BOOL LoadFile(HMODULE hModule, LPCTSTR lpszBitmapFileName);
	void SetBitmap(HBITMAP hBitmap, BOOL bAlpha = FALSE);

	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, const CRect& rcSizingMargins, COLORREF clrTransparent, DWORD dwFlags);
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, const CRect& rcSizingMargins, COLORREF clrTransparent);
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc, CRect rcSizingMargins);
	void DrawImage(CDC* pDC, const CRect& rcDest, CRect rcSrc);

	int GetHeight() const;
	int GetWidth() const;
	CSize GetExtent() const;

	BOOL IsAlphaImage() const;

	void PreserveOrientation() {
		m_bImageOrientaionPreserved = TRUE;
	}

	CRect GetSource(int nState = 0, int nCount = 1) const;

	BOOL GetBitmapPixel(CPoint pt, COLORREF& clrPixel);

	void Premultiply();

	void DrawImageFlipped(BOOL bFlip);


protected:
	BOOL DrawImagePart(CDC* pDCDest, CRect rcDest,
		CDC* pDCSrc, CRect rcSrc) const;

	BOOL DrawImageTile(CDC* pDCDest, const CRect& rcDest,
		CDC* pDCSrc, const CRect& rcSrc, BOOL bTile) const;

	void InvertBitmap();
	BOOL ConvertToBits(HBITMAP hBitmap);

protected:
	LPVOID m_pBits;
	BOOL m_bAlpha;
	BOOL m_bMultiply;
	BOOL m_bImageOrientaionPreserved;
	BOOL m_bInvert;
	CSize m_szExtent;
	BOOL m_bFlipped;

	CXTPResourceImages* m_pImages;

	friend class CXTPResourceImages;
	friend class CXTPResourceImageList;
};

class _XTP_EXT_CLASS CXTPResourceImages : public CXTPCmdTarget
{
	friend class CXTPSingleton<CXTPResourceImages>;

public:
	CXTPResourceImages();
	virtual ~CXTPResourceImages();

public:
	void RemoveAll();

	CXTPResourceImage* LoadFile(LPCTSTR lpszImageFile);
	CXTPResourceImage* LoadImage(LPCTSTR strImageFile);

	HMODULE GetHandle() const;

	BOOL SetHandle(LPCTSTR lpszDllFileName, LPCTSTR lpszIniFileName = NULL);
	BOOL SetHandle(HMODULE hResource, LPCTSTR lpszIniFileName = NULL, BOOL bFreeOnRelease = FALSE);


	//-----------------------------------------------------------------------
	// Summary:
	//     Reads or writes options from or to an archive.
	// Parameters:
	//     lpszThemeFileName - Name of initialization file of the theme
	//     lpszAppFileName   - Name of initialization file of the application
	//-----------------------------------------------------------------------
	BOOL SetHandle(HMODULE hResource, LPCTSTR lpszThemeFileName, LPCTSTR lpszAppFileName, BOOL bFreeOnRelease = FALSE);

	void SetIniData(LPCSTR lpIniData);

	BOOL InitResourceHandle(LPCTSTR lpszTestImageFile, LPCTSTR lpResType = RT_BITMAP);

	COLORREF GetImageColor(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, COLORREF clrDefault = (COLORREF)-1) const;
	int GetImageInt(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, int nDefault = -1) const;
	CRect GetImageRect(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CRect rcDefault = CRect(0, 0, 0, 0)) const;
	CSize GetImageSize(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, CSize szDefault = CSize(0, 0)) const;

	CString GetImageString(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName, LPCTSTR lpszDef = NULL) const;
	CString GetImageValue(LPCTSTR lpszSectionName, LPCTSTR lpszKeyName) const;
	BOOL IsValid() const;
	void AssertValid();

	BOOL LoadBitmap(CBitmap* pBitmap, LPCTSTR lpszResourceName);

	CXTPNotifyConnection* GetConnection();

	CString m_strDllFileName;
	CString m_strIniFileName;

	BOOL m_bLoadedNothing;

protected:
	CString _ImageNameToResourceName(LPCTSTR strImageFile);

	void ClearProperties();
	void LoadProperties(LPSTR lpTextFile, LPSTR lpTextFileEnd, BOOL bAllowOverwrite);

private:
	BOOL ReadString(CString& str, LPSTR& lpTextFile, LPSTR lpTextFileEnd);

	BOOL LoadResources(LPCTSTR lpszIniFileName);
	BOOL LoadResources(HRSRC hRsrc, BOOL bAllowOverwrite);

	static BOOL CALLBACK EnumResNameProc(HMODULE hModule,
		LPCTSTR lpszType, LPTSTR lpszName, LONG_PTR lParam);
	HRSRC FindResourceIniFile(HMODULE hResource);

	UINT GetKeyHash(LPCTSTR lpszSection, LPCTSTR lpszKeyName) const;
	HBITMAP AllocateLayerBitmap(UINT cx, UINT cy, LPDWORD* lpBits = NULL, SIZE* pSize = NULL);


protected:
	HMODULE m_hResource;
	BOOL m_bFreeOnRelease;
	CRITICAL_SECTION m_cs;
	BOOL m_bPremultiplyImages;

	CXTPBitmapDC *m_pdc;
	HBITMAP m_hbmLayer;
	LPDWORD m_pLayerBits;

	CXTPNotifyConnection* m_pConnection;

	CMap<UINT, UINT, CString, CString> m_mapPropeties;

protected:
	CMapStringToPtr m_mapImages;


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	DECLARE_OLETYPELIB_EX(CXTPResourceImages);

	BOOL OleLoadFromFile(LPCTSTR lpszPath, LPCTSTR lpszIniFileName);
	virtual void OnFinalRelease();
	BSTR OleGetIniFileName();
	BSTR OleGetDllFileName();
	OLE_COLOR OleGetGetColor(LPCTSTR lpszSection, LPCTSTR lpszEntry);

	//}}AFX_CODEJOCK_PRIVATE
#endif

	friend class CXTPResourceImage;

};

//-----------------------------------------------------------------------
// Summary: Call to access members of the CXTPResourceImages class.
// Returns: Returns the a pointer to the single instance of the
//          CXTPResourceImages class.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPResourceImages* AFX_CDECL XTPResourceImages();

//-----------------------------------------------------------------------
// Summary:    Call to access members of the CXTPResourceImage class.
// Parameters: lpszImageFile - Name of the image file to load specified in the theme INI.
// Returns:    Returns the a pointer to the single instance of the
//             CXTPResourceImage class.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPResourceImage* AFX_CDECL XTPLoadImage(LPCTSTR lpszImageFile);

//-----------------------------------------------------------------------
// Summary:    Call to get the color value specified by lpszKey from the
//             current theme.
// Parameters: lpszSection - Name of the section in the theme INI file.
//             lpszKey     - Name of the value that defines the specified value.
//             clrDef      - Default value returned if lpszKey was not found.
// Returns:    A value represting the color defined by lpszKey.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS COLORREF AFX_CDECL XTPIniColor(LPCTSTR lpszSection, LPCTSTR lpszKey, COLORREF clrDef);

//-----------------------------------------------------------------------
// Summary:    Call to get the int value specified by lpszKey from the
//             current theme.
// Parameters: lpszSection - Name of the section in the theme INI file.
//             lpszKey     - Name of the value that defines the specified value.
//             nDef        - Default value returned if lpszKey was not found.
// Returns:    A value represting the int defined by lpszKey.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS int AFX_CDECL XTPIniInt(LPCTSTR lpszSection, LPCTSTR lpszKey, int nDef);

//-----------------------------------------------------------------------
// Summary:    Call to get the CRect value specified by lpszKey from the
//             current theme.
// Parameters: lpszSection - Name of the section in the theme INI file.
//             lpszKey     - Name of the value that defines the specified value.
//             rcDef       - Default value returned if lpszKey was not found.
// Returns:    A value represting the CRect defined by lpszKey.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CRect AFX_CDECL XTPIniRect(LPCTSTR lpszSection, LPCTSTR lpszKey, CRect rcDef);

//-----------------------------------------------------------------------
// Summary:    Call to get the CSize value specified by lpszKey from the
//             current theme.
// Parameters: lpszSection - Name of the section in the theme INI file.
//             lpszKey     - Name of the value that defines the specified value.
//             szDef       - Default value returned if lpszKey was not found.
// Returns:    A value represting the CSize defined by lpszKey.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CSize AFX_CDECL XTPIniSize(LPCTSTR lpszSection, LPCTSTR lpszKey, CSize szDef);

//-----------------------------------------------------------------------
// Summary:    Call to get the CString value specified by lpszKey from the
//             current theme.
// Parameters: lpszSection - Name of the section in the theme INI file.
//             lpszKey     - Name of the value that defines the specified value.
//             lpszDef     - Default value returned if lpszKey was not found.
// Returns:    A value represting the CString defined by lpszKey.
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CString AFX_CDECL XTPIniString(LPCTSTR lpszSection, LPCTSTR lpszKey, LPCTSTR lpszDef);

// -----------------------------------------
// Use with CXTPResourceImages::SetHandle().
// -----------------------------------------

// Office2007.dll
#define xtpIniOffice2007Aqua                _T("OFFICE2007AQUA_INI")                // Specifies Office 2007 Aqua settings file.
#define xtpIniOffice2007Blue                _T("OFFICE2007BLUE_INI")                // Specifies Office 2007 Blue settings file. 
#define xtpIniOffice2007Black               _T("OFFICE2007BLACK_INI")               // Specifies Office 2007 Black settings file.
#define xtpIniOffice2007Silver              _T("OFFICE2007SILVER_INI")              // Specifies Office 2007 Silver settings file.

// Office2010.dll
#define xtpIniOffice2010Blue                _T("OFFICE2010BLUE_INI")                // Specifies Office 2010 Blue settings file. 
#define xtpIniOffice2010Black               _T("OFFICE2010BLACK_INI")               // Specifies Office 2010 Black settings file.
#define xtpIniOffice2010Silver              _T("OFFICE2010SILVER_INI")              // Specifies Office 2010 Silver settings file.

// Office2013.dll
#define xtpIniOffice2013Access              _T("OFFICE2013ACCESS_INI")              // Specifies Office 2013 Access settings file.
#define xtpIniOffice2013AccessGrayDark      _T("OFFICE2013ACCESSGRAYDARK_INI")      // Specifies Office 2013 Access Dark Gray settings file. 
#define xtpIniOffice2013AccessGrayLight     _T("OFFICE2013ACCESSGRAYLIGHT_INI")     // Specifies Office 2013 Access Light Gray settings file.
#define xtpIniOffice2013Excel               _T("OFFICE2013EXCEL_INI")               // Specifies Office 2013 Excel settings file.
#define xtpIniOffice2013ExcelGrayDark       _T("OFFICE2013EXCELGRAYDARK_INI")       // Specifies Office 2013 Excel Dark Gray settings file. 
#define xtpIniOffice2013ExcelGrayLight      _T("OFFICE2013EXCELGRAYLIGHT_INI")      // Specifies Office 2013 Excel Light Gray settings file.
#define xtpIniOffice2013OneNote             _T("OFFICE2013ONENOTE_INI")             // Specifies Office 2013 OneNote settings file.
#define xtpIniOffice2013OneNoteGrayDark     _T("OFFICE2013ONENOTEGRAYDARK_INI")     // Specifies Office 2013 OneNote Dark Gray settings file. 
#define xtpIniOffice2013OneNoteGrayLight    _T("OFFICE2013ONENOTEGRAYLIGHT_INI")    // Specifies Office 2013 OneNote Light Gray settings file.
#define xtpIniOffice2013Outlook             _T("OFFICE2013OUTLOOK_INI")             // Specifies Office 2013 Outlook settings file.
#define xtpIniOffice2013OutlookGrayDark     _T("OFFICE2013OUTLOOKGRAYDARK_INI")     // Specifies Office 2013 Outlook Dark Gray settings file. 
#define xtpIniOffice2013OutlookGrayLight    _T("OFFICE2013OUTLOOKGRAYLIGHT_INI")    // Specifies Office 2013 Outlook Light Gray settings file.
#define xtpIniOffice2013Publisher           _T("OFFICE2013PUBLISHER_INI")           // Specifies Office 2013 Publisher settings file.
#define xtpIniOffice2013PublisherGrayDark   _T("OFFICE2013PUBLISHERGRAYDARK_INI")   // Specifies Office 2013 Publisher Dark Gray settings file. 
#define xtpIniOffice2013PublisherGrayLight  _T("OFFICE2013PUBLISHERGRAYLIGHT_INI")  // Specifies Office 2013 Publisher Light Gray settings file.
#define xtpIniOffice2013PowerPoint          _T("OFFICE2013POWERPOINT_INI")          // Specifies Office 2013 PowerPoint settings file.
#define xtpIniOffice2013PowerPointGrayDark  _T("OFFICE2013POWERPOINTGRAYDARK_INI")  // Specifies Office 2013 PowerPoint Dark Gray settings file. 
#define xtpIniOffice2013PowerPointGrayLight _T("OFFICE2013POWERPOINTGRAYLIGHT_INI") // Specifies Office 2013 PowerPoint Light Gray settings file.
#define xtpIniOffice2013Word                _T("OFFICE2013WORD_INI")                // Specifies Office 2013 Word settings file.
#define xtpIniOffice2013WordGrayDark        _T("OFFICE2013WORDGRAYDARK_INI")        // Specifies Office 2013 Word Dark Gray settings file. 
#define xtpIniOffice2013WordGrayLight       _T("OFFICE2013WORDGRAYLIGHT_INI")       // Specifies Office 2013 Word Light Gray settings file.

// VisualStudio2012.dll
#define xtpIniVisualStudio2012Dark          _T("VISUALSTUDIO2012DARK_INI")          // Specifies Visual Studio 2012 Dark settings file.
#define xtpIniVisualStudio2012Light         _T("VISUALSTUDIO2012LIGHT_INI")         // Specifies Visual Studio 2012 Light settings file.

// VisualStudio2015.dll
#define xtpIniVisualStudio2015Blue          _T("VISUALSTUDIO2015BLUE_INI")          // Specifies Visual Studio 2015 Blue settings file.
#define xtpIniVisualStudio2015Dark          _T("VISUALSTUDIO2015DARK_INI")          // Specifies Visual Studio 2015 Dark settings file.
#define xtpIniVisualStudio2015Light         _T("VISUALSTUDIO2015LIGHT_INI")         // Specifies Visual Studio 2015 Light settings file.

// Windows7.dll
#define xtpIniWindows7Blue                  _T("WINDOWS7BLUE_INI")                  // Specifies Windows 7 Blue settings file.

#endif // !defined(__XTPRESOURCEIMAGE_H__)
