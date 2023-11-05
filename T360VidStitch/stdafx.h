
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars


#include <XTToolkitPro.h>   // Codejock Software Components
#include "global.h"

#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;


#include "GdipButton.h"
#include "GdipButtonext.h"
/////////////////////////////////////////////////////////////////////////////

#ifndef		CREATEPROCESS_MANIFEST_RESOURCE_ID
#define CREATEPROCESS_MANIFEST_RESOURCE_ID		1 
#endif

#ifndef		RT_MANIFEST
#define RT_MANIFEST								24
#endif

#ifndef		CS_DROPSHADOW
#define CS_DROPSHADOW							0x00020000
#endif

/////////////////////////////////////////////////////////////////////////////

#ifndef		SAFE_DELETE
#define SAFE_DELETE( p )	{ if( p ){ delete p; p = NULL; } }
#endif

#ifndef		SAFE_FREE
#define SAFE_FREE( p )		{ if( p ){ free( (LPVOID)p ); p = NULL; } }
#endif


#include "gl/gl.h"
#include "gl/glu.h"
#include "Quram360Engine.h"
#include "QLicense.h"

#if (!LICENSE_PERSONAL_KEY && LICENSE_MACADDRESS == 0)//def _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#endif
#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


