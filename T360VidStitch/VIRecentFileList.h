#pragma once
#include "afxadv.h"


class VIRecentFileList : public CRecentFileList {
public:
	VIRecentFileList(UINT nStart, LPCTSTR lpszSection,
		LPCTSTR lpszEntryFormat, int nSize,
		int nMaxDispLen = AFX_ABBREV_FILENAME_LEN)
		: CRecentFileList(nStart, lpszSection, lpszEntryFormat, nSize,
		nMaxDispLen) {}
	virtual void UpdateMenu(CCmdUI* pCmdUI);
};  // class VIRecentFileList

