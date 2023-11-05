#pragma once



// CFormView1 form view

class CFormView1 : public CFormView
{
	DECLARE_DYNCREATE(CFormView1)

protected:
	CFormView1();           // protected constructor used by dynamic creation
	virtual ~CFormView1();

public:
	enum { IDD = IDD_PANORAMAPREVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CPalette    m_cPalette;
	CPalette    *m_pOldPalette;
	CRect       m_oldRect;
	float       m_fRadius;
	CClientDC   *m_pDC;
	HGLRC       m_hrc;
	// Operations
	int STITCHING_MODE;
public:
	void Init();
	void CreateRGBPalette(void);
	BOOL bSetupPixelFormat(void);
	unsigned char ComponentFromIndex(int i, UINT nbits, UINT shift);
	void draw_output();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual void OnDraw(CDC* /*pDC*/);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL m_play;
	afx_msg void OnDestroy();
	virtual void OnInitialUpdate();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	void GLResize(int cx, int cy);
	void GLRenderSceneTest();
};


