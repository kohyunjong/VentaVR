// XTPSingleton.h: CXTPSingleton template class definition.
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
#if !defined(__XTPSINGLETON_H__)
#define __XTPSINGLETON_H__
//}}AFX_CODEJOCK_PRIVATE

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//===========================================================================
// Summary: Implements Singleton design pattern for a pointer of unspecified
//          type that is to be allocated, created and destroyed by a client.
//          Respects thread safety for instance allocation.
//===========================================================================
class _XTP_EXT_CLASS CXTPSingletonPointer : IXTPApplicationEvents
{
public:
	// Creator callback type.
	typedef BOOL (AFX_CDECL* Creator)(void*);

	// Destroyer callback type.
	typedef void (AFX_CDECL* Destroyer)(void*);

	//===========================================================================
	// Summary: Handles object construction.
	// Parameters:
	//          cbObjectSize - Singleton object size in bytes.
	//          pfnCreator - Creater callback function pointer.
	//          pfnDestroyer - Destroyer callback function pointer.
	//===========================================================================
	CXTPSingletonPointer(
		SIZE_T cbObjectSize,
		Creator pfnCreator,
		Destroyer pfnDestroyer);

	//===========================================================================
	// Summary: Handles object destruction.
	//===========================================================================
	~CXTPSingletonPointer();

	//===========================================================================
	// Summary: Obtains encapsulated singleton instance pointer. The instance is 
	//          allocated and created during the first call. The function guaranties
	//          returning a valid pointer value. If it cannot return it an exception
	//          will be issued.
	// Returns: A pointer to singleton instance.
	//===========================================================================
	void* GetInstance();

	//===========================================================================
	// Summary: Destroys a previously created and allocated instance and
	//          invalidate the instance pointer.
	//===========================================================================
	void Destroy();

	//===========================================================================
	// Summary: Obtains reference to singleton access critical secton.
	// Returns: A reference to singleton access critical secton.
	//===========================================================================
	::CRITICAL_SECTION& GetAccess() const { return m_access; }

private:
	virtual void OnApplicationShutdown(CXTPApplication* pApplication);

private:
	void* m_pInstance;
	mutable ::CRITICAL_SECTION m_access;
	const SIZE_T m_cbObjectSize;
	const Creator m_pfnCreator;
	const Destroyer m_pfnDestroyer;
	BOOL m_bDestroyed;
};

//===========================================================================
// Summary: Implements Singleton design pattern. Can be used both as an 
//          adaptor or a base class for a class the is not derived from 
//          any other class. Respects thread safety for instance allocation.
// Parameters:
//          T - Type name of singleton instance.
//===========================================================================
template<class T>
class CXTPSingleton
{
	static BOOL AFX_CDECL Create(void* ptr)
	{
		new (ptr) T();
		return TRUE;
	}

	static void AFX_CDECL Destroy(void* ptr)
	{
		reinterpret_cast<T*>(ptr)->T::~T();
	}

public:

	//===========================================================================
	// Summary: Obtains encapsulated singleton instance reference. The instance is 
	//          allocated and created during the first call.
	// Returns: A reference to singleton instance.
	//===========================================================================
	static T& Instance()
	{
		void* pInstance = InstancePointer().GetInstance();
		return *reinterpret_cast<T*>(pInstance);
	}

protected:
	//===========================================================================
	// Summary: Handles object construction.
	//===========================================================================
	CXTPSingleton() { }

	//===========================================================================
	// Summary: Obtains reference to singleton access critical secton.
	// Returns: A reference to singleton access critical secton.
	//===========================================================================
	static ::CRITICAL_SECTION& GetInstanceAccess()
	{
		return InstancePointer().GetAccess();
	}

	static CXTPSingletonPointer& InstancePointer()
	{
		static CXTPSingletonPointer instanceHolder(sizeof(T), Create, Destroy);
		return instanceHolder;
	}
};

//{{AFX_CODEJOCK_PRIVATE
template<class Parent> class CXTPDerive : public Parent {};
template<> class CXTPDerive<void> {};
//}}AFX_CODEJOCK_PRIVATE

//===========================================================================
// Summary: Implements Singleton design pattern for a derived class with
//          possibility of providing a parent class.
// Parameters:
//          Derived - Type name of singleton instance.
//          Parent - Type of a parent class, can be 'void' which means
//                   no parent class is used.
//===========================================================================
template<class Derived, class Parent>
class CXTPSingletonBase 
	: public CXTPDerive<Parent>
	, public CXTPSingleton<Derived>
{
protected:
	//===========================================================================
	// Summary: Handles object construction.
	//===========================================================================
	CXTPSingletonBase() { }
};

#endif /*__XTPSINGLETON_H__*/
