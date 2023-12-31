// XTPReportDefines.h
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
#if !defined(__XTPREPORTDEFINES_H__)
#define __XTPREPORTDEFINES_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef _XTP_ACTIVEX
enum XTPEnumReportCustomDraw
{
	xtpCustomDrawItem        = 0x01,
	xtpCustomMeasureRow      = 0x02,
	xtpCustomBeforeDrawRow   = 0x04,
	xtpCustomDrawPreviewItem = 0x08,
	xtpCustomMeasurePreviewItem = 0x10,
};
#endif

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the shell icons.
//-----------------------------------------------------------------------
enum XTPReportShellIcon
{
	xtpReportShellIconLock = 47
};


//-----------------------------------------------------------------------
// Summary:
//      This enum defines the index of the glyphs.
//-----------------------------------------------------------------------
enum XTPReportGlyph
{
	xtpReportGlyphInvalid            = -1,
	xtpReportGlyphRowCollapse        =  0,
	xtpReportGlyphRowExpand          =  1,
	xtpReportGlyphEnabledUnchecked   =  2,
	xtpReportGlyphEnabledChecked     =  3,
	xtpReportGlyphDisabledUnchecked  =  4,
	xtpReportGlyphDisabledChecked    =  5,
	xtpReportGlyphColumnExpand       =  6,
	xtpReportGlyphColumnRestore      =  7,
	xtpReportGlyphEnabledTristate    =  8,
	xtpReportGlyphItemLocked         =  9,
	xtpReportGlyphItemUnlocked       = 10,
	xtpReportGlyphCount
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines the possible checkbox states.
//-----------------------------------------------------------------------
enum XTPReportCheckState
{
	xtpReportCheckStateInvalid        = -1,   // The checkbox state is invalid
	xtpReportCheckStateUnchecked      =  0,   // The checkbox is in an unchecked state
	xtpReportCheckStateChecked        =  1,   // The checkbox is in an checked state
	xtpReportCheckStateIndeterminate  =  2,   // The checkbox is in an indeterminate state
	xtpReportCheckStateCount          =  3    // Number of checkbox states
};

//-----------------------------------------------------------------------
// Summary:
//      This enum defines body, header, and footer rows.
// See Also:
//      CXTPReportRow::m_nRowType
//-----------------------------------------------------------------------
enum XTPReportRowType
{
	xtpRowTypeInvalid   = -1,    // Indicates an invalid row type
	xtpRowTypeBody      =  0,    // The row is in body rows collection.
	xtpRowTypeHeader    =  1,    // The row is in Headers rows collection.
	xtpRowTypeFooter    =  2     // The row is in Footers rows collection.
};

//-----------------------------------------------------------------------
// Summary:
//     Enumeration of orientations.
// See Also: SetGridStyle
//-----------------------------------------------------------------------
enum XTPReportOrientation
{
	xtpReportOrientationHorizontal = 0,
	xtpReportOrientationVertical   = 1,
	xtpReportOrientationAll        = 2
};

//===========================================================================
// Summary:
//     Represents predefined grid line styles:
// Example:
//     <code>m_wndReport.SetGridStyle(FALSE, xtpReportGridLargeDots);</code>
// See Also: CXTPReportControl::SetGridStyle
//
// <KEYWORDS xtpReportGridNoLines, xtpReportGridSmallDots, xtpReportGridLargeDots, xtpReportGridDashes, xtpReportGridSolid>
//===========================================================================
enum XTPReportLineStyle
{
	xtpReportLineStyleNone      = 0,     // No line
	xtpReportLineStyleDotSmall  = 1,     // Line is drawn with small dots
	xtpReportLineStyleDotLarge  = 2,     // Line is drawn with large dots
	xtpReportLineStyleDash      = 3,     // Line is drawn with dashes
	xtpReportLineStyleSolid     = 4,     // Solid line
	xtpReportLineStyleCount     = 5,     // Number of line styles

	// Old names
	xtpReportGridNoLines        = xtpReportLineStyleNone,
	xtpReportGridSmallDots      = xtpReportLineStyleDotSmall,
	xtpReportGridLargeDots      = xtpReportLineStyleDotLarge,
	xtpReportGridDashes         = xtpReportLineStyleDash,
	xtpReportGridSolid          = xtpReportLineStyleSolid
};

typedef XTPReportLineStyle XTPReportGridStyle;

//===========================================================================
// Summary:
//     Represents predefined freeze column line styles:
//===========================================================================
enum XTPReportFreezeColsDividerStyle
{
	xtpReportFreezeColsDividerThin      = 0x01,  // thin line style
	xtpReportFreezeColsDividerBold      = 0x02,  // bold line style
	xtpReportFreezeColsDividerHeader    = 0x04,  // header line style
	xtpReportFreezeColsDividerShade     = 0x08,  // shade line style
	xtpReportFreezeColsDividerShowAlways = 0x10,  // show event if AutoColumnSizing is On
};

//{{AFX_CODEJOCK_PRIVATE

// deprecated
#define xtpGridNoLines      xtpReportGridNoLines
#define xtpGridSmallDots    xtpReportGridSmallDots
#define xtpGridLargeDots    xtpReportGridLargeDots
#define xtpGridDashes       xtpReportGridDashes
#define xtpGridSolid        xtpReportGridSolid

//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary:
//     Represents predefined column styles.
// Example:
//     <code>m_wndReport.GetPaintManager()->m_columnStyle = xtpReportColumnFlat;</code>
// See Also: CXTPReportPaintManager::m_columnStyle, CXTPReportPaintManager
//
// <KEYWORDS xtpReportColumnShaded, xtpReportColumnFlat>
//===========================================================================
enum XTPReportColumnStyle
{
	xtpReportColumnShaded,      // Columns are gray shaded.
	xtpReportColumnFlat,        // Flat style for drawing columns.
	xtpReportColumnExplorer,    // Explorer column style
	xtpReportColumnOffice2003,  // Gradient column style
	xtpReportColumnResource     // Office 2007/2010 column style
};

//{{AFX_CODEJOCK_PRIVATE

// deprecated
#define xtpColumnShaded xtpReportColumnShaded
#define xtpColumnFlat   xtpReportColumnFlat
#define xtpReportColumnOffice2007 xtpReportColumnResource

//}}AFX_CODEJOCK_PRIVATE

//-----------------------------------------------------------------------
// Summary:
//     Represent the style of lines used to draw a hierarchical tree structure.
// Example:
//     <code>m_wndReport.GetPaintManager()->m_treeStructureStyle = xtpReportTreeStructureSolid;</code>
// See Also: CXTPReportPaintManager::m_treeStructureStyle, CXTPReportPaintManager
//
// <KEYWORDS xtpReportTreeStructureNone, xtpReportTreeStructureSolid>
//-----------------------------------------------------------------------
enum XTPReportTreeStructureStyle
{
	xtpReportTreeStructureNone,     // No lines will be drawn from the parent node to each child node.
	xtpReportTreeStructureSolid,    // Lines will be drawn from the parent node to each child node at the next level.
	xtpReportTreeStructureDots      // Dots will be drawn from the parent node to each child node at the next level.
};

//-----------------------------------------------------------------------
// Summary:
//     Represent the Header/Footer rows divider style.
//-----------------------------------------------------------------------
enum XTPReportSectionDividerStyle
{
	xtpReportSectionDividerStyleNone    = 0x00, // no line
	xtpReportSectionDividerStyleThin    = 0x01, // thin style
	xtpReportSectionDividerStyleBold    = 0x02, // bold style
	xtpReportSectionDividerStyleShade   = 0x04, // shaded line style
	xtpReportSectionDividerStyleOutlook = 0x08,  // outlook line style

//{{AFX_CODEJOCK_PRIVATE
	// Deprecated
	xtpReportFixedRowsDividerNone       = xtpReportSectionDividerStyleNone,
	xtpReportFixedRowsDividerThin       = xtpReportSectionDividerStyleThin,
	xtpReportFixedRowsDividerBold       = xtpReportSectionDividerStyleBold,
	xtpReportFixedRowsDividerShade      = xtpReportSectionDividerStyleShade,
	xtpReportFixedRowsDividerOutlook    = xtpReportSectionDividerStyleOutlook
//}}AFX_CODEJOCK_PRIVATE
};

//{{AFX_CODEJOCK_PRIVATE
// Old enum name
typedef XTPReportSectionDividerStyle XTPReportFixedRowsDividerStyle;
//}}AFX_CODEJOCK_PRIVATE


//-----------------------------------------------------------------------
// Summary:
//     Section divider position.
//-----------------------------------------------------------------------
enum XTPReportSectionDividerPosition
{
	xtpReportSectionDividerPositionTop    = 0,   // Divider above the section
	xtpReportSectionDividerPositionBottom = 1    // Divider below the section
};

//-----------------------------------------------------------------------
// Summary:
//     Represent the Report Draw Sort Triangle logic
//-----------------------------------------------------------------------
enum XTPReportDrawSortTriangleStyle
{
	xtpReportDrawSortTriangleDefault = 0,   // The triangle will be removed when the column size is too small to display
	                                        // the full caption and triangle
	xtpReportDrawSortTriangleAlways  = 1,   // The sort triangle displayed in column header when a column is sorted and
	                                        // the caption will be cut with ellipsis
	xtpReportDrawSortTriangleNever   = 2    // Never draw the triangle.
};

//===========================================================================
// Summary:
//     Enumeration of hyperlink text decoration.
//
// <KEYWORDS xtpReportTextDecorationNone, xtpReportTextDecorationUnderline>
//===========================================================================
enum XTPReportTextDecoration
{
	xtpReportTextDecorationNone     = 0,    // Text has no decoration
	xtpReportTextDecorationUnderline    = 1     // Text is underlined
};

//-----------------------------------------------------------------------
// Summary:
//     Visual theme enumeration
// Example:
//     <code> m_wndReportControl.SetTheme(xtpReportThemeOffice2013); </code>
// See Also: CXTPReportControl::SetTheme, CXTPReportControl::GetCurrentTheme
//
// <KEYWORDS xtpReportThemeDefault, xtpReportThemeOfficeXP, xtpReportThemeOffice2003, xtpReportThemeOffice2003Luna, xtpReportThemeOffice2013, xtpReportThemeVisual2012StudioLight, xtpReportThemeVisual2012StudioDark, xtpReportThemeVisual2012StudioBlue, xtpReportThemeExplorer, xtpReportThemeResource>
//-----------------------------------------------------------------------
enum XTPReportPaintTheme
{
	xtpReportThemeDefault,                  // Default theme.
	xtpReportThemeOfficeXP,                 // Office XP style theme.
	xtpReportThemeOffice2003,               // Office 2003 style theme.
	xtpReportThemeOffice2003Luna,           // Office 2003 style theme with luna colors.
	xtpReportThemeOffice2013,               // Office 2013 style theme.
	xtpReportThemeVisualStudio2012Light,    // Visual Studio 2012 Light style theme.
	xtpReportThemeVisualStudio2012Dark,     // Visual Studio 2012 Dark style theme.
	xtpReportThemeVisualStudio2012Blue,     // Visual Studio 2012 Blue style theme.
	xtpReportThemeVisualStudio2015,         // Visual Studio 2015 style theme.
	xtpReportThemeExplorer,                 // Windows Explorer theme.
	xtpReportThemeResource,                 // Resource DLL style theme.
};


//{{AFX_CODEJOCK_PRIVATE

XTP_DECLARE_HEAP_ALLOCATOR_(CXTPReportDataAllocator, _XTP_EXT_CLASS)
XTP_DECLARE_HEAP_ALLOCATOR_(CXTPReportRowAllocator,  _XTP_EXT_CLASS)

// to allocate in the app default heap
XTP_DECLARE_HEAP_ALLOCATOR_(CXTPReportAllocatorDefault, _XTP_EXT_CLASS)

XTP_DECLARE_BATCH_ALLOC_OBJ_DATA_(CXTPReportRow_BatchData, _XTP_EXT_CLASS);
XTP_DECLARE_BATCH_ALLOC_OBJ_DATA_(CXTPReportGroupRow_BatchData, _XTP_EXT_CLASS);

//}}AFX_CODEJOCK_PRIVATE


//-----------------------------------------------------------------------
// Summary:
//     NULL terminated string used by CXTPReportControl which specifies
//     the window class name for the report control.
// Example:
// <code>
// BOOL CXTPReportControl::RegisterWindowClass()
// {
//     WNDCLASS wndcls;
//     HINSTANCE hInst = XTPGetInstanceHandle();
//
//     if (!(::GetClassInfo(hInst, XTPREPORTCTRL_CLASSNAME, &wndcls)))
//     {
//         // otherwise we need to register a new class
//         wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
//         wndcls.lpfnWndProc      = ::DefWindowProc;
//         wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
//         wndcls.hInstance        = hInst;
//         wndcls.hIcon            = NULL;
//         wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
//         wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
//         wndcls.lpszMenuName     = NULL;
//         wndcls.lpszClassName    = XTPREPORTCTRL_CLASSNAME;
//
//         if (!AfxRegisterClass(&wndcls))
//         {
//             AfxThrowResourceException();
//             return FALSE;
//         }
//     }
//
//     return TRUE;
// }
// </code>
// See Also:
//     CXTPReportControl::RegisterWindowClass
//-----------------------------------------------------------------------
const TCHAR XTPREPORTCTRL_CLASSNAME[] = _T("XTPReport");
const TCHAR XTPTRACKCTRL_CLASSNAME[] = _T("XTPTrackControl");

//===========================================================================
// Summary:
//     Report control records clipboard format name.
//===========================================================================
static const LPCTSTR XTPREPORTCTRL_CF_RECORDS = _T("XTPReport_CF_Records");


//-----------------------------------------------------------------------
// Summary:
//     This constant indicates that a COLORREF is not defined.
// Example:
// <code>
// void CXTPReportRecordItem::GetItemMetrics(
//     XTP_REPORTRECORDITEM_DRAWARGS* pDrawArgs,
//     XTP_REPORTRECORDITEM_METRICS* pItemMetrics)
// {
//     if (m_clrBackground != XTP_REPORT_COLOR_DEFAULT)
//         pItemMetrics->clrBackground = m_clrBackground;
//
//     if (m_clrText != XTP_REPORT_COLOR_DEFAULT)
//         pItemMetrics->clrForeground = m_clrText;
//
//     if (m_pFontCaption != NULL)
//         pItemMetrics->pFont = m_pFontCaption;
//
//     else if (m_bBoldText)
//         pItemMetrics->pFont =
//             &pDrawArgs->pControl->GetPaintManager()->m_fontBoldText;
// }
// </code>
// See Also:
//    CXTPReportRecordItem, CXTPReportRow
//-----------------------------------------------------------------------
const COLORREF XTP_REPORT_COLOR_DEFAULT = (COLORREF)-1;

//-----------------------------------------------------------------------
// Summary:
//     This constant indicates that an icon ID is not defined.
// Example:
// <code>
// m_wndReport.AddColumn(new CXTPReportColumn(COLUMN_PRICE, _T("Price"),
//     80, TRUE, XTP_REPORT_NOICON, TRUE, FALSE));
// </code>
// See Also:
//    CXTPReportColumn
//-----------------------------------------------------------------------
const int XTP_REPORT_NOICON = (int)-1;

//-----------------------------------------------------------------------
// Summary:

//     Defines message for header context menu
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_HEADER_RCLICK notification message is sent to inform
//     the owner window that the user has right clicked column of report control
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_HEADER_RCLICK, XTP_ID_REPORT_CONTROL, OnReportColumnRClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportColumnRClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*)pNotifyStruct;
//     ASSERT(pItemNotify->pColumn);
//     CPoint ptClick = pItemNotify->pt;
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_HEADER_RCLICK         (NM_FIRST-51)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling rows selection changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_SELCHANGED notification message is sent to inform
//     the owner window that the user select row of report control
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_SELCHANGED, XTP_ID_REPORT_CONTROL, OnReportSelChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportSelChanged(NMHDR* pNMHDR, LRESULT* /*result*/)
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_SELCHANGED            (NM_FIRST-52)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling row checking event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_CHECKED notification message is sent to inform
//     the owner window that the user click check box  of item of report control
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_CHECKED, XTP_ID_REPORT_CONTROL, OnReportCheckItem)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportCheckItem(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_CHECKED               (NM_FIRST-53)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling hyperlink click event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_HYPERLINK notification message is sent to inform
//     the owner window that the user click hyperlink of item.
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_HYPERLINK, XTP_ID_REPORT_CONTROL, OnReportHyperlinkClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportHyperlinkClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//
//     if (!pItemNotify->pRow || !pItemNotify->pColumn)
//         return;
//
//     // if click on Hyperlink in Item
//     if (pItemNotify->nHyperlink >= 0)
//     {
//         TRACE(_T("Hyperlink Click : \n row %d \n col %d \n Hyperlink %d.\n"),
//             pItemNotify->pRow->GetIndex(),
//             pItemNotify->pColumn->GetItemIndex(),
//             pItemNotify->nHyperlink);
//     }
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_HYPERLINK             (NM_FIRST-54)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for column changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_COLUMNORDERCHANGED notification message is sent to inform
//     the owner window that the user change the order of columns of report control.
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_ID_REPORT_CONTROL, OnReportColumnOrderChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportColumnOrderChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTCOLUMNORDERCHANGED* pItemNotify = (XTP_NM_REPORTCOLUMNORDERCHANGED*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_GROUPORDERCHANGED, XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_COLUMNORDERCHANGED    (NM_FIRST-55)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for sort changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_SORTORDERCHANGED notification message is sent to inform
//     the owner window that the user change click header of report control and change sort order.
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_SORTORDERCHANGED, XTP_ID_REPORT_CONTROL, OnReportSortOrderChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportSortOrderChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_SORTORDERCHANGED      (NM_FIRST-56)

//-----------------------------------------------------------------------
// Summary:
//     Defines message when a column header has been clicked and the sort order
//      has changed, but CXTPReportControl::Populate has NOT yet been called.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_PRESORTORDERCHANGED notification message is sent to inform
//     the owner window that the user change click header of report control the sort order will
//     change. XTP_NM_REPORT_PRESORTORDERCHANGED is send BEFORE CXTPReportControl::Populate has been
//     called, so the order of the rows has not yet been visibly changed.  After CXTPReportControl::Populate is called,
//     the XTP_NM_REPORT_SORTORDERCHANGED notification is send to inform that the order has actually
//     changed.
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_VALUECHANGED, XTP_NM_REPORT_SORTORDERCHANGED
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_PRESORTORDERCHANGED   (NM_FIRST-67)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for value changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_VALUECHANGED notification message is sent to inform
//     the owner window that the user change value of report cell
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// <code>
// BEGIN_MESSAGE_MAP(CPropertiesView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_VALUECHANGED, ID_REPORT_CONTROL, OnReportValueChanged)
// END_MESSAGE_MAP()
//
// void CPropertiesView::OnReportValueChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     switch (pItemNotify->pItem->GetItemData())
//     {
//         case ID_PROPERTY_MULTIPLESELECTION:
//             GetTargetReport()->SetMultipleSelection(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_PREVIEWMODE:
//             GetTargetReport()->EnablePreviewMode(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             GetTargetReport()->Populate();
//             break;
//         case ID_PROPERTY_GROUPBOXVISIBLE:
//             GetTargetReport()->ShowGroupBy(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_FOCUSSUBITEMS:
//             GetTargetReport()->FocusSubItems(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNREMOVE:
//             GetTargetReport()->GetReportHeader()->AllowColumnRemove(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNREORDER:
//             GetTargetReport()->GetReportHeader()->AllowColumnReorder(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_ALLOWCOLUMNRESIZE:
//             GetTargetReport()->GetReportHeader()->AllowColumnResize(
//                 CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_FLATHEADER:
//             GetTargetReport()->GetPaintManager()->m_columnStyle =
//                 (CRecordPropertyBool::GetValue(pItemNotify)) ? xtpReportColumnFlat : xtpReportColumnShaded;
//             break;
//         case ID_PROPERTY_HIDESELECTION:
//             GetTargetReport()->GetPaintManager()->m_bHideSelection =
//                 (CRecordPropertyBool::GetValue(pItemNotify));
//             break;
//         case ID_PROPERTY_TREEINDENT:
//             GetTargetReport()->GetPaintManager()->m_nTreeIndent =
//                 CRecordPropertyInt::GetValue(pItemNotify);
//             GetTargetReport()->RedrawControl();
//             break;
//     }
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_VALUECHANGED          (NM_FIRST-57)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for in-place button click event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_INPLACEBUTTONDOWN notification message is sent to inform
//     the owner window that the user click in-place button of report control
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Returns:
//     TRUE if message was processed; FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_ID_REPORT_CONTROL, OnReportButtonClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportButtonClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTINPLACEBUTTON* pItemNotify = (XTP_NM_REPORTINPLACEBUTTON*) pNotifyStruct;
//     ASSERT(pItemNotify->pButton);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_INPLACEBUTTONDOWN     (NM_FIRST-58)


//-----------------------------------------------------------------------
// Summary:
//     Defines message for row expand event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_ROWEXPANDED notification message is sent to inform
//     the owner window that the user expand or collapse the row of report control .
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_ROWEXPANDED, XTP_ID_REPORT_CONTROL, OnReportRowExpandChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportRowExpandChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_ROWEXPANDED           (NM_FIRST-59)


//-----------------------------------------------------------------------
// Summary:
//     Defines message for left mouse button click.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_LBUTTONDOWN notification message is sent to inform
//     the owner window that the user press left button on the row.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_LBUTTONDOWN, XTP_ID_REPORT_CONTROL, OnReportLButtonDown)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportLButtonDown(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTRECORDITEM* pItemNotify = (XTP_NM_REPORTRECORDITEM*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_LBUTTONDOWN           (NM_FIRST-60)
#define XTP_NM_REPORT_LBUTTONUP             (NM_FIRST-89)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportControl::GetItemMetrics().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_GETITEMMETRICS notification message is sent to allow
//     the owner window customize item drawing.
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_GETITEMMETRICS, XTP_ID_REPORT_CONTROL, OnReportGetItemMetrics)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportGetItemMetrics(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTITEMMETRICS* pItemNotify = (XTP_NM_REPORTITEMMETRICS*)pNotifyStruct;
//
//     ASSERT(pItemNotify->pDrawArgs);
//     ASSERT(pItemNotify->pDrawArgs->pControl);
//     ASSERT(pItemNotify->pDrawArgs->pRow);
//
//     //     pItemNotify->pDrawArgs->pColumn   - may be NULL (for a group row)
//     //     pItemNotify->pDrawArgs->pItem     - may be NULL (for a group row)
//
//     ASSERT(pItemNotify->pItemMetrics);
//
//     // TODO: customize members of pItemNotify->pItemMetrics.
// }
// </code>
// See Also:
//     CXTPReportControl, CXTPReportControl::GetItemMetrics()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_GETITEMMETRICS        (NM_FIRST-61)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportControl::OnRequestEdit().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_REQUESTEDIT notification message is sent to notify
//     that an item has entered edit mode.  This is send when an editable
//     item starts to be edited or when a check box item is checked\unchecked.
// See Also:
//     CXTPReportControl, CXTPReportControl::OnRequestEdit()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_REQUESTEDIT           (NM_FIRST-62)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportControl::OnBeforeCopyToText().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_BEFORE_COPY_TOTEXT notification message is sent to allow
//     the owner window customize copy/paste operations.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_BEFORE_COPY_TOTEXT, XTP_ID_REPORT_CONTROL, OnBeforeCopyToText)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnBeforeCopyToText(NMHDR*  pNotifyStruct, LRESULT* result)
// {
//  ASSERT(pNotifyStruct);
//
//  XTP_NM_REPORT_BEFORE_COPYPASTE* pnmCopyPaste = (XTP_NM_REPORT_BEFORE_COPYPASTE*)pNotifyStruct;
//
//  if (!pnmCopyPaste || !pnmCopyPaste->ppRecord || !*pnmCopyPaste->ppRecord ||
//      !pnmCopyPaste->parStrings)
//  {
//      ASSERT(FALSE);
//      return;
//  }
//
//  // customize data provided by pnmCopyPaste->parStrings
//  // you can use source record object: (*pnmCopyPaste->ppRecord)->Member()
//
//  // set result to not zero to cancel operation:
//  // *result = (LRESULT)TRUE;
// }
// </code>
// See Also:
//     CXTPReportControl, CXTPReportControl::OnBeforeCopyToText()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_BEFORE_COPY_TOTEXT    (NM_FIRST-63)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportControl::OnBeforePasteFromText().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_BEFORE_PASTE_FROMTEXT notification message is sent
//     to allow the owner window customize copy/paste operations.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_BEFORE_PASTE_FROMTEXT, XTP_ID_REPORT_CONTROL, OnReportBeforePasteFromText)
// END_MESSAGE_MAP()
//
//  void CReportSampleView::OnReportBeforePasteFromText(NMHDR*  pNotifyStruct, LRESULT* result)
//  {
//      ASSERT(pNotifyStruct);
//
//      XTP_NM_REPORT_BEFORE_COPYPASTE* pnmCopyPaste = (XTP_NM_REPORT_BEFORE_COPYPASTE*)pNotifyStruct;
//
//      if (!pnmCopyPaste || !pnmCopyPaste->ppRecord || !pnmCopyPaste->parStrings) {
//          ASSERT(FALSE);
//          return;
//      }
//
//      CMessageRecord* pRecord = new CMessageRecord();
//      if (!pRecord) {
//          return;
//      }
//
//      *pnmCopyPaste->ppRecord = pRecord;
//
//      CXTPReportColumns* pColumns = GetReportCtrl().GetColumns();
//      ASSERT(pColumns);
//      if (!pColumns) {
//          return;
//      }
//
//      int nDataCount = pnmCopyPaste->parStrings->GetSize();
//
//      const int nColumnCount = pColumns->GetVisibleColumnsCount();
//      for (int nCol = 0; nCol < nColumnCount; nCol++)
//      {
//          CXTPReportColumn* pColumn = pColumns->GetVisibleAt(nCol);
//          CXTPReportRecordItem* pItem = pRecord->GetItem(pColumn);
//          ASSERT(pItem);
//          if (NULL == pItem)
//              continue;
//
//          if (nCol < nDataCount)
//          {
//              CString strItem = pnmCopyPaste->parStrings->GetAt(nCol);
//              pItem->SetCaption(strItem);
//          }
//      }
//      // set result to not zero to cancel operation:
//      // *result = (LRESULT)TRUE;
//  }
// </code>
// See Also:
//     CXTPReportControl, CXTPReportControl::OnBeforePasteFromText()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_BEFORE_PASTE_FROMTEXT (NM_FIRST-64)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportControl::OnBeforePaste().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_BEFORE_PASTE notification message is sent to allow
//     the owner window customize copy/paste operations.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_BEFORE_PASTE, XTP_ID_REPORT_CONTROL, OnBeforePaste)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnBeforePaste(NMHDR*  pNotifyStruct, LRESULT* result)
// {
//  ASSERT(pNotifyStruct);
//
//  XTP_NM_REPORT_BEFORE_COPYPASTE* pnmCopyPaste = (XTP_NM_REPORT_BEFORE_COPYPASTE*)pNotifyStruct;
//  ASSERT(pnmCopyPaste && pnmCopyPaste->parStrings == NULL);
//
//  if (!pnmCopyPaste || !pnmCopyPaste->ppRecord || !*pnmCopyPaste->ppRecord) {
//      ASSERT(FALSE);
//      return;
//  }
//
//  // you can create new or change provided record using pnmCopyPaste->ppRecord
//
//  // set result to not zero to cancel operation:
//  // *result = (LRESULT)TRUE;
// }
// </code>
// See Also:
//     CXTPReportControl, CXTPReportControl::OnBeforePaste()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_BEFORE_PASTE          (NM_FIRST-65)

//{{AFX_CODEJOCK_PRIVATE
// Internal Trace operation
#define XTP_TRACE
//}}AFX_CODEJOCK_PRIVATE


#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
#define XTP_NM_REPORT_VALUECHANGING         (NM_FIRST-66)
//}}AFX_CODEJOCK_PRIVATE
#endif

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportHeader::AdjustColumnsWidth().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_COLUMNWIDTHCHANGED notification message is sent
//     when the width of a column has changed.  This can happen when the user
//     changes the width or when the Report Control automatically changes the
//     width.
// See Also:
//     CXTPReportHeader, CXTPReportHeader::AdjustColumnsWidth
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_COLUMNWIDTHCHANGED    (NM_FIRST-68)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling CXTPReportControl::OnPreviewKeyDown().
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     This notification is sent to Main window in a WM_NOTIFY message before
//     processing OnKeyDown event.
// See Also:
//     CXTPReportControl::OnPreviewKeyDown, XTP_NM_REPORTPREVIEWKEYDOWN,
//     CXTPReportControl::OnKeyDown, CWnd::OnKeyDown.
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_PREVIEWKEYDOWN        (NM_FIRST-69)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the user start dragging records.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTDRAGDROP structure passed with this message
// See Also:
//     XTP_NM_REPORTDRAGDROP, XTP_NM_REPORT_BEGINDRAG, XTP_NM_REPORT_DROP,
//     XTP_NM_REPORT_RECORDS_DROPPED, XTP_NM_REPORT_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_BEGINDRAG             (NM_FIRST-70)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the user dropped records to report control.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTDRAGDROP structure passed with this message
// See Also:
//     XTP_NM_REPORTDRAGDROP, XTP_NM_REPORT_BEGINDRAG,
//     XTP_NM_REPORT_RECORDS_DROPPED, XTP_NM_REPORT_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_DROP                  (NM_FIRST-71)
#define XTP_NM_REPORT_HASVALIDDROPTYPE      (NM_FIRST-85)
#define XTP_NM_REPORT_VALIDDROPTARGET       (NM_FIRST-86)
//-----------------------------------------------------------------------
// Summary:
//     Defines message for in-place editing is canceled and value is not
//     changed event.
// Parameters:
//     id        -  Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTRECORDITEM structure passed with this message
// See Also:
//     XTP_NM_REPORTRECORDITEM, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportRecordItem::OnEditCanceled
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_EDIT_CANCELED         (NM_FIRST-72)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the user changed a selected item in the combobox.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// See Also:
//     CXTPReportControl::OnConstraintSelecting()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_CONSTRAINT_SELECTING          (NM_FIRST-73)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the tooltip will be drawn.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// See Also:
//     CXTPReportControl::OnGetToolTipInfo()
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_GETTOOLTIPINFO          (NM_FIRST-74)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the records were dropped to report control.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTDRAGDROP structure passed with this message
// See Also:
//     XTP_NM_REPORTDRAGDROP, XTP_NM_REPORT_BEGINDRAG, XTP_NM_REPORT_DROP,
//     XTP_NM_REPORT_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_RECORDS_DROPPED          (NM_FIRST-75)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for handling focus changing event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_FOCUS_CHANGING notification message is sent to inform
//     the owner window that the focused row or column is about to be changed
//     The owner window of the report control receives this notification
//     through the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_FOCUS_CHANGING, XTP_ID_REPORT_CONTROL, OnReportFocusChanging)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportFocusChanging(NMHDR* pNMHDR, LRESULT* /*result*/)
// {
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN,
//     XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_FOCUS_CHANGED
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_FOCUS_CHANGING         (NM_FIRST-76)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the user completed drag&drop operation.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTDRAGDROP structure passed with this message
// See Also:
//     XTP_NM_REPORTDRAGDROP, XTP_NM_REPORT_BEGINDRAG, XTP_NM_REPORT_DROP,
//     XTP_NM_REPORT_RECORDS_DROPPED
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_DRAGDROP_COMPLETED     (NM_FIRST-77)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for in-place editing has just started.
//     Send after XTP_NM_REPORT_REQUESTEDIT and when inplace editor
//     has been created.
// Parameters:
//     id        -  Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTRECORDITEM structure passed with this message
// See Also:
//     XTP_NM_REPORTRECORDITEM, XTP_NM_REPORT_VALUECHANGED,
//     XTP_NM_REPORT_REQUESTEDIT,
//     CXTPReportRecordItem::OnEditCanceled
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_BEGINEDIT              (NM_FIRST-78)

#ifdef _XTP_ACTIVEX
//{{AFX_CODEJOCK_PRIVATE
#define XTP_NM_REPORT_EDIT_CHANGING          (NM_FIRST-79)
//}}AFX_CODEJOCK_PRIVATE
#endif

//-----------------------------------------------------------------------
// Summary:
//     Defines message for selected rows collection changing. Send for
//     each row when it added/removed.
// Parameters:
//     id        -  Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_SELECTION_CHANGING structure passed with this message
// See Also:
//     XTP_NM_SELECTION_CHANGING
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_SELCHANGING           (NM_FIRST-80)

//-----------------------------------------------------------------------
// Summary:
//     Indicates the user is dragging records over a control.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     XTP_NM_REPORTDRAGDROP structure passed with this message
// See Also:
//     XTP_NM_REPORTDRAGDROP, XTP_NM_REPORT_BEGINDRAG, XTP_NM_REPORT_DROP,
//     XTP_NM_REPORT_RECORDS_DROPPED, XTP_NM_REPORT_DRAGDROP_COMPLETED
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_DRAGOVER               (NM_FIRST-81)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for column group order changed event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_GROUPORDERCHANGED notification message is sent to inform
//     the owner window that the user change the columns group order of report control.
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_GROUPORDERCHANGED, XTP_ID_REPORT_CONTROL, OnReportGroupOrderChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportGroupOrderChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTCOLUMNORDERCHANGED* pItemNotify = (XTP_NM_REPORTCOLUMNORDERCHANGED*) pNotifyStruct;
//     ASSERT(pItemNotify != NULL);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORT_CHECKED, XTP_NM_REPORT_HEADER_RCLICK,
//     XTP_NM_REPORT_HYPERLINK, XTP_NM_REPORT_INPLACEBUTTONDOWN, XTP_NM_REPORT_SELCHANGED,
//     XTP_NM_REPORT_COLUMNORDERCHANGED, XTP_NM_REPORT_SORTORDERCHANGED, XTP_NM_REPORT_VALUECHANGED,
//     CXTPReportControl, CXTPReportHeader
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_GROUPORDERCHANGED      (NM_FIRST-82)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for item button click event.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_ITEMBUTTONCLICK notification message is sent to inform
//     the owner window that the user click item button of report control
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Returns:
//     TRUE if message was processed; FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_ITEMBUTTONCLICK, XTP_ID_REPORT_CONTROL, OnReportItemButtonClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportItemButtonClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
//     ASSERT(pItemNotify->pItemControl);
//
//     // TODO: Handle command.
// }
// </code>
// See Also:
//     XTP_NM_REPORTITEMCONTROL
//     CXTPReportRecordItemControl, CXTPReportRecordItemButton
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_ITEMBUTTONCLICK     (NM_FIRST-83)

//-----------------------------------------------------------------------
// Summary:
//     Defines message for header column click on PlusMinus icon.
// Parameters:
//     id -         Resource ID for the control.
//     memberFxn -  Name of member function to handle the message.
// Remarks:
//     The XTP_NM_REPORT_PLUSMINUSCLICK notification message is sent to inform
//     the owner window that the user click on header column PlusMinus icon of report control
//     The owner window of the report control receives this notification
//     threw the WM_COMMAND message.
// Returns:
//     TRUE if message was processed; FALSE for default process.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_PLUSMINUSCLICK, XTP_ID_REPORT_CONTROL, OnReportPlusMinusClick)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportPlusMinusClick(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTITEMCONTROL* pItemNotify = (XTP_NM_REPORTITEMCONTROL*) pNotifyStruct;
//     ASSERT(pItemNotify->pItemControl);
//     ASSERT(pItemNotify->pColumn);
//     CPoint ptClick = pItemNotify->pt;
//
//     // TODO: Handle command.
// }
// </code>
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_PLUSMINUSCLICK     (NM_FIRST-84)

//-----------------------------------------------------------------------
// Summary:
//     This message is sent by the report control when the state of an item
//     or range of items has changed. It is sent in the form of a WM_NOTIFY message.
// Parameters:
//     lpStateChange - Long pointer to an XTP_NM_REPORTSTATECHANGED structure
//     that contains data about the item or items that have changed.
// Remarks:
//     The XTP_NM_REPORT_STATECHANGED notification message is sent to inform
//     the owner window that the state of an item or range of items has changed.
// Returns:
//     The application receiving this message must return zero.
// Example:
//     Here is an example of how an application would handle this message.
// <code>
// BEGIN_MESSAGE_MAP(CReportSampleView, CXTPReportView)
//     ON_NOTIFY(XTP_NM_REPORT_STATECHANGED, XTP_ID_REPORT_CONTROL, OnReportStateChanged)
// END_MESSAGE_MAP()
//
// void CReportSampleView::OnReportStateChanged(NMHDR*  pNotifyStruct, LRESULT* /*result*/)
// {
//     XTP_NM_REPORTSTATECHANGED* pItemNotify = (XTP_NM_REPORTSTATECHANGED*)pNotifyStruct;
//     int nBegin = nm.nBegin; // first item in the range
//     int nEnd = nm.nEnd; // last item in the range
//
//     // TODO: Handle command.
// }
// </code>
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_STATECHANGED (NM_FIRST-88)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_REPORT_HSCROLL message is sent when the horizontal
//     scrolling position has changed.
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_HSCROLL (NM_FIRST-90)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_REPORT_VSCROLL message is sent when the vertical
//     scrolling position has changed.
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_VSCROLL (NM_FIRST-91)

//-----------------------------------------------------------------------
// Summary:
//     The XTP_NM_REPORT_MOUSEMOVE message is sent when the
//     mouse is moved over the control.
//-----------------------------------------------------------------------
#define XTP_NM_REPORT_MOUSEMOVE (NM_FIRST-92)


#define XTP_NM_REPORT_GIVEFEEDBACK (NM_FIRST-93)

//-----------------------------------------------------------------------
// Summary:
//     Enables deprecated report control methods
//-----------------------------------------------------------------------
#define XTP_REPORT_DEPRECATED()    (1)

class CXTPReportRow;

//-----------------------------------------------------------------------
// Summary:
//     Define a function pointer for comparing events.
// Remarks:
//     This function pointer is used in the SortEx method.
// See Also:
//     Sort, SortEx, CXTPReportControl::SetRowsCompareFunc
//-----------------------------------------------------------------------
typedef int (_cdecl* XTPReportRowsCompareFunc)(const CXTPReportRow** pRow1, const CXTPReportRow** pRow2);


#define XTP_DECLARE_PROPERTY(property, type)\
	afx_msg type OleGet##property();\
	afx_msg void OleSet##property(type);

#define XTP_DISP_PROPERTY_EX_ID(theClass, szExternalName, dispid, vtPropType)\
	DISP_PROPERTY_EX_ID(theClass, #szExternalName, dispid, OleGet##szExternalName, OleSet##szExternalName, vtPropType)

#define XTP_IMPLEMENT_PROPERTY(theClass, theProperty, theMember, theType)\
	theType theClass::OleGet##theProperty()\
	{\
		return theMember;\
	}\
	void theClass::OleSet##theProperty(theType set)\
	{\
		theMember = set;\
	}

#define XTP_IMPLEMENT_PROPERTY_COLOR(theClass, theProperty, theMember)\
	COLORREF theClass::OleGet##theProperty()\
	{\
		return theMember;\
	}\
	void theClass::OleSet##theProperty(COLORREF oleColor)\
	{\
		theMember = AxTranslateColor(oleColor);\
	}


#endif //#if !defined(__XTPREPORTDEFINES_H__)
