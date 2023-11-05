// XTPFontManager.h: interface for the CXTPFontManager class.
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
#if !defined(__XTPFONTMANAGER_H__)
#define __XTPFONTMANAGER_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary: Singleton class to manage fonts used by library windows and 
//          controls.  You can access members of this class by using the
//          function XTPFontManager().
//===========================================================================
class _XTP_EXT_CLASS CXTPFontManager
{
    //-----------------------------------------------------------------------
    // Summary: Private singleton constructor.
    //-----------------------------------------------------------------------
	CXTPFontManager();

    //-----------------------------------------------------------------------
	// Summary: Used to access the CXTPFontManager object.
    //-----------------------------------------------------------------------
	friend _XTP_EXT_CLASS CXTPFontManager* AFX_CDECL XTPFontManager();

    //-----------------------------------------------------------------------
	// Summary: Base class singleton template.
    //-----------------------------------------------------------------------
	friend class CXTPSingleton<CXTPFontManager>;

public:
	//-----------------------------------------------------------------------
	// Summary:    Call this member function to determine if the specified
	//             font exists.
	// Parameters: lpszFaceName - A NULL terminated string that represents the
	//                            font face name.
	// Returns:    TRUE if the font was found, otherwise FALSE.
	//-----------------------------------------------------------------------
	BOOL FontExists(LPCTSTR lpszFaceName);

    //-----------------------------------------------------------------------
    // Summary:    Call this member to create system fonts using the specified
	//             LOGFONT structure.
    // Parameters: lf - Font structure used to initialize all system fonts.
    //-----------------------------------------------------------------------
	void CreateFonts(LOGFONT lf);

    //-----------------------------------------------------------------------
    // Summary: Retrieves a reference to the standard library font managed
	//          by CXTPFontManager.
    // Returns: A reference to a CFont object representing the library 
	//          standard font.
    //-----------------------------------------------------------------------
	CFont& GetFont();

    //-----------------------------------------------------------------------
    // Summary: Retrieves a reference to the bold library font managed
	//          by CXTPFontManager.
    // Returns: A reference to a CFont object representing the library 
	//          bold font.
    //-----------------------------------------------------------------------
	CFont& GetFontBold();

    //-----------------------------------------------------------------------
    // Summary: Retrieves a reference to the italic library font managed
	//          by CXTPFontManager.
    // Returns: A reference to a CFont object representing the library 
	//          italic font.
    //-----------------------------------------------------------------------
	CFont& GetFontItalic();

    //-----------------------------------------------------------------------
    // Summary: Retrieves a reference to the italic standard library font 
	//          managed by CXTPFontManager.
    // Returns: A reference to a CFont object representing the library 
	//          italic standard font.
    //-----------------------------------------------------------------------
	CFont& GetFontItalicBold();

    //-----------------------------------------------------------------------
    // Summary: Retrieves a reference to the vertical library font managed
	//          by CXTPFontManager.
    // Returns: A reference to a CFont object representing the library 
	//          vertical font.
    //-----------------------------------------------------------------------
	CFont& GetFontVertical();

    //-----------------------------------------------------------------------
    // Summary: Retrieves a reference to the vertical bold library font 
	//          managed by CXTPFontManager.
    // Returns: A reference to a CFont object representing the library 
	//          vertical bold font.
    //-----------------------------------------------------------------------
	CFont& GetFontVerticalBold();

protected:
	CFont m_font;             // Default font.
	CFont m_fontBold;         // Default bold font.
	CFont m_fontItalic;       // Default italic font.
	CFont m_fontItalicBold;   // Default italic bold font.
	CFont m_fontVertical;     // Default vertical font.
	CFont m_fontVerticalBold; // Default vertical bold font.
};

AFX_INLINE CFont& CXTPFontManager::GetFont() {
	return m_font;
}
AFX_INLINE CFont& CXTPFontManager::GetFontBold() {
	return m_fontBold;
}
AFX_INLINE CFont& CXTPFontManager::GetFontItalic() {
	return m_fontItalic;
}
AFX_INLINE CFont& CXTPFontManager::GetFontItalicBold() {
	return m_fontItalicBold;
}
AFX_INLINE CFont& CXTPFontManager::GetFontVertical() {
	return m_fontVertical;
}
AFX_INLINE CFont& CXTPFontManager::GetFontVerticalBold() {
	return m_fontVerticalBold;
}

//---------------------------------------------------------------------------
// Summary: Call this function to access CXTPFontManager members. Since this 
//          class is designed as a single instance object you can only access 
//          its members through this method. You <b>cannot</b> directly 
//          instantiate an object of type CXTPFontManager.
// Example: <code>SetFont(&CXTPFontManager()->GetNormalFont());</code>
//---------------------------------------------------------------------------
_XTP_EXT_CLASS CXTPFontManager* AFX_CDECL XTPFontManager();

#endif // !defined(__XTPFONTMANAGER_H__)
