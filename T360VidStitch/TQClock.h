

#ifndef __TQCLOCK_QURAM_H__
#define __TQCLOCK_QURAM_H__

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
inline double TQClock() {
	static LARGE_INTEGER freq = { 0, 0 };
	/*if( freq.QuadPart==0 )*/ QueryPerformanceFrequency(&freq);
	LARGE_INTEGER cur;
	QueryPerformanceCounter(&cur);
	return cur.QuadPart / (double)freq.QuadPart * 1000;
}
#undef WIN32_LEAN_AND_MEAN
#endif

#endif