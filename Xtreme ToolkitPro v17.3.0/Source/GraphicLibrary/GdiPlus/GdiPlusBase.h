/**************************************************************************\
*
* Copyright (c) 1998-2001, Microsoft Corp.  All Rights Reserved.
*
* Module Name:
*
*   GdiplusBase.h
*
* Abstract:
*
*   GDI+ base memory allocation class
*
\**************************************************************************/

#ifndef _GDIPLUSBASE_H
#define _GDIPLUSBASE_H

#ifdef _DEBUG
typedef class GdiplusBaseDbg
// Changing class name is necessary to make sure linker won't pickup standard GDI+
// destructors in static configuration.
#else
class GdiplusBase
#endif
{
#ifdef _DEBUG

	class AfxMemTracker
	{
		enum { Signature = 'AFX' };

		int m_Signature;
		void* m_gdiPtr;
		void* m_afxPtr;

		AfxMemTracker() {}

		void Init(LPCSTR lpszFileName, int nLine)
		{
			ASSERT(NULL != lpszFileName);

			m_Signature = Signature;
			m_gdiPtr = this + 1;
			m_afxPtr = AfxAllocMemoryDebug(1, FALSE, lpszFileName, nLine);
		}

		bool Verify() const
		{
			return (!::IsBadReadPtr(this, sizeof(*this))
				&& m_Signature == Signature
				&& AfxIsMemoryBlock(m_afxPtr, 1));
		}

		void Destroy()
		{
			AfxFreeMemoryDebug(m_afxPtr, FALSE);
			DllExports::GdipFree(this);
		}

	public:
		static void* Allocate(size_t nSize, LPCSTR lpszFileName, int nLine)
		{
			void* gdiPtr = NULL;

			AfxMemTracker* pMT = reinterpret_cast<AfxMemTracker*>(DllExports::GdipAlloc(sizeof(AfxMemTracker) + nSize));
			if (NULL != pMT)
			{
				pMT->Init(lpszFileName, nLine);
				gdiPtr = pMT->m_gdiPtr;
			}

			return gdiPtr;
		}

		static void Deallocate(void* ptr)
		{
			AfxMemTracker* pMT = reinterpret_cast<AfxMemTracker*>(
				reinterpret_cast<char*>(ptr) - sizeof(AfxMemTracker));
			if (pMT->Verify())
			{
				pMT->Destroy();
			}
			else
			{
				DllExports::GdipFree(ptr);
			}
		}
	};

#endif /*_DEBUG*/

public:
	void(operator delete)(void* in_pVoid)
	{
#ifdef _DEBUG
		AfxMemTracker::Deallocate(in_pVoid);
#else
		DllExports::GdipFree(in_pVoid);
#endif
	}
	void* (operator new)(size_t in_size)
	{
#ifdef _DEBUG
		return AfxMemTracker::Allocate(in_size, __FILE__, __LINE__);
#else
		return DllExports::GdipAlloc(in_size);
#endif
	}
	void(operator delete[])(void* in_pVoid)
	{
#ifdef _DEBUG
		AfxMemTracker::Deallocate(in_pVoid);
#else
		DllExports::GdipFree(in_pVoid);
#endif
	}
	void* (operator new[])(size_t in_size)
	{
#ifdef _DEBUG
		return AfxMemTracker::Allocate(in_size, __FILE__, __LINE__);
#else
		return DllExports::GdipAlloc(in_size);
#endif
	}

#ifdef _DEBUG

	void* operator new(size_t nSize, LPCSTR lpszFileName, int nLine)
	{
		return AfxMemTracker::Allocate(nSize, lpszFileName, nLine);
	}
	void operator delete(void* p, LPCSTR lpszFileName, int nLine)
	{
		UNREFERENCED_PARAMETER(lpszFileName);
		UNREFERENCED_PARAMETER(nLine);

		AfxMemTracker::Deallocate(p);
	}
	void* operator new[](size_t nSize, LPCSTR lpszFileName, int nLine)
	{
		return AfxMemTracker::Allocate(nSize, lpszFileName, nLine);
	}
	void operator delete[](void* p, LPCSTR lpszFileName, int nLine)
	{
		UNREFERENCED_PARAMETER(lpszFileName);
		UNREFERENCED_PARAMETER(nLine);

		AfxMemTracker::Deallocate(p);
	}

#endif /*_DEBUG*/
#ifdef _DEBUG
} GdiplusBase;
#else
};
#endif

#endif /*_GDIPLUSBASE_H*/
