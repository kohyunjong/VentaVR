#pragma once
#include "./Src/ImgDialogBase.h"
#include "Label.h"
// CTransparentDlgType1 ��ȭ �����Դϴ�.


class CTransparentDlgType : public CDialog
{
	DECLARE_DYNAMIC(CTransparentDlgType)

public:
	CTransparentDlgType(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTransparentDlgType();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_TRANSPARENT_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	CLabel	m_szText1;
	CString	m_strText;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void MoveTextPos(int x, int y, int width, int height);

	//CTime m_tstart;
	//CTime m_tend;

	int type;

	CWinThread* m_pfnTimerThread;
	HANDLE		m_hTimerfnThExitEvt;
	BOOL		m_bTimerfnContinue;
	void		DoStartATimer(int nType);
	void		KillTimerfnThread();

	int frame;
	CWinThread* m_pLoadingfnThread;
	HANDLE		m_hLoadingfnThExitEvt;
	BOOL		m_bLoadingfnContinue;
	void		DoStartLoading();
	void		KillLoadingfnThread();

	void		SetFontObj();
protected:
	static UINT	TimerfnThreadProc(LPVOID pParam);
	static UINT	LoadingfnThreadProc(LPVOID pParam);
public:
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	typedef BOOL(WINAPI *SetLayer)(HWND hWnd, COLORREF crKey, BYTE bAlpha, DWORD dwFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CBitmap		m_BGBitmap;
	void ToastString(CString str);
};
