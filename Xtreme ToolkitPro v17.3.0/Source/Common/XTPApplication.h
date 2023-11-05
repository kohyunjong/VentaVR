// XTPApplication.h
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
#if !defined(__XTPAPPLICATION_H__)
#define __XTPAPPLICATION_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CXTPApplication;

//-----------------------------------------------------------------------
// Summary:
//      Defines a callback interface for XTP application event sink.
//-----------------------------------------------------------------------
struct IXTPApplicationEvents
{
	//-----------------------------------------------------------------------
	// Summary:
	//      Is called by framework when application before application shutting down.
	// Parameters:
	//      pApplication - Event sending application pointer.
	//-----------------------------------------------------------------------
	virtual void OnBeforeApplicationShutdown(CXTPApplication* pApplication)
	{
		UNREFERENCED_PARAMETER(pApplication);
	}

	//-----------------------------------------------------------------------
	// Summary:
	//      Is called by framework when application is being shutdown before
	//      the actual shutting down.
	// Parameters:
	//      pApplication - Event sending application pointer.
	//-----------------------------------------------------------------------
	virtual void OnApplicationShutdown(CXTPApplication* pApplication)
	{
		UNREFERENCED_PARAMETER(pApplication);
	}
};

//-----------------------------------------------------------------------
// Summary:
//      Defines a global XTP application object interface. The object is 
//      kept alive until a call to XTPShutdown. At any moment there is
//      one and only one XTP application instance per loaded dynamic or
//      executable module which links XTP framework.
// See also:
//      XTPGetApplication, XTPShutdown
//-----------------------------------------------------------------------
class _XTP_EXT_CLASS CXTPApplication
	: public CObject
	, public CXTPObservable<CXTPApplication, IXTPApplicationEvents>
	, public CXTPSynchronized
{
	friend class CXTPSingleton<CXTPApplication>;
	friend _XTP_EXT_CLASS CXTPApplication* AFX_CDECL XTPGetApplication();
	friend _XTP_EXT_CLASS void AFX_CDECL XTPShutdown();

	DECLARE_DYNAMIC(CXTPApplication);

private:
	CXTPApplication();
public:
	//{{AFX_CODEJOCK_PRIVATE
	~CXTPApplication();
	//}}AFX_CODEJOCK_PRIVATE
	
private:
	static CXTPApplication& GetIstance();

	void Shutdown();

	BOOL m_bShutdown;
};

//-----------------------------------------------------------------------
// Summary:
//      Obtains the only XTP application instance pointer. Must not be called
//      after XTPShutdown call.
// Returns:
//      The only XTP application instance pointer.
// See also:
//      XTPShutdown
//-----------------------------------------------------------------------
_XTP_EXT_CLASS CXTPApplication* AFX_CDECL XTPGetApplication();

//-----------------------------------------------------------------------
// Summary:
//      Performs XTP application shutting down with releasing all internal 
//      XTP pointers and resources but not terminating the calling thread.
//      The call to XTPShutdown must happen as late as possible at runtime
//      but no later than returning from WinMain or DllMain.
//      For backward compatibility it still is called by framework automatically
//      on static storage destruction but the order of this operation 
//      is not guaranteed.
// See also:
//      XTPGetApplication
//-----------------------------------------------------------------------
_XTP_EXT_CLASS void AFX_CDECL XTPShutdown();

#endif // !defined(__XTPAPPLICATION_H__)