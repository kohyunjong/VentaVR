#pragma once
#include "afxwin.h"
#include "PanoPreViewDlg.h"


#define FULL_FRAME_FISHEYE_TO_EQUIRECT			0x03
#define CIRCULAR_FISH_TO_EQUIRECT				0x02
#define CYLINDRICAL_PANORAMA_TO_EQUIRECT		0x01
#define RECTLINEAR_TO_EQUIRECT					0x00

// CBottomSourceChildDlg 대화 상자입니다.

class CBottomSourceChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBottomSourceChildDlg)

public:
	CBottomSourceChildDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CBottomSourceChildDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_BOTTOMSOURCEDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctlTopBar;
	CPanoPreViewDlg* m_dlgPreviewDlg;

	BOOL m_bisShowPanoPreview;
	BOOL m_bisCapConf;

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnPreviepano();
	afx_msg void OnBnClickedBtnCapconf();
	CGdipButton m_btnCaptureConfig;
	// On/Off 토글버튼
	CGdipButton m_btnPanoPrevie;
	CGdipButton m_btnCameraSel;
	CGdipButton m_btnEdit;

	bool isCParamEdit = false, isCameraSel = false;

	void UpdateDialogItemValue();
	CComboBox m_cbpAudioInput;
	CComboBox m_cbpLensType;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetFontObj();
	afx_msg void OnCbnSelchangeComboAudioinput();
	afx_msg void OnCbnDropdownComboAudioinput();
	afx_msg void OnBnClickedBtnEditinput();
	afx_msg void OnBnClickedBtncipEdit();
	afx_msg void OnDestroy();
	afx_msg LRESULT OnClosePreviewDlg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboLens();

	void CameraSelBtnState(bool btnState);
	CString setLensType();
	CEdit m_editLenstype;
};
