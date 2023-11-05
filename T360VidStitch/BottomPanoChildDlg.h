#pragma once
#include "afxwin.h"
#include "CurvePaneDlg.h"
#include "ColorSettingDlg.h"
#include "afxcmn.h"
// CBottomPanoChildDlg ��ȭ �����Դϴ�.

class CBottomPanoChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CBottomPanoChildDlg)

public:
	CBottomPanoChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBottomPanoChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_BOTTOMPANODLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ctlTopBar;
	CCurvePaneDlg*	m_pCurveControlDlg;
	CColorSettingDlg* m_pColorSetlDlg;

	BOOL m_bisShowPanoPreview;
	BOOL m_bisCapConf;
	CGdipButton m_btnCurves1;	// Ȱ��ȭ ���� 
	CGdipButton m_btnColor;
	CGdipButton m_btnNadirOnOff;
	CGdipButton m_btnGainOnOff;
	CGdipButton m_btnBrow;
	CComboBox m_cboNadirPos;
	CSliderCtrl m_sldNadirHeight;

	int m_nSeam;
	int m_nBlending;

	CComboBox m_cboAGainMode;
	BOOL	m_bGainActive;
	int m_nAGainEffect;

	void UpdateDialogItemValue();
	void EnableNadirButton(BOOL bEnable);
	void EnableGainButton(BOOL bEnable);
	void DoSaveTempShapShot();
	void DeleteTempShapShot();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedBtnCurves();
	afx_msg void OnBnClickedBtnColorset();
	afx_msg void OnBnClickedBtnGain();
	afx_msg void OnBnClickedBtnFileBrow();
	afx_msg void OnBnClickedBtnNadirSet();
	afx_msg void OnCbnSelchangeComboNadirpos();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void SetRadioSeamStatus(UINT value);
	afx_msg void SetRadioBlendingStatus(UINT value);
	afx_msg void SetRadioGainStatus(UINT value);
	afx_msg void OnCbnSelchangeComboAgainmide();
	BOOL MakeThumbFrameImage(LPCTSTR InSourceImageFileName, LPCTSTR InDestImageFileName,
		const CRect& InTagetRect, REFGUID InImageFormatToMake);
	afx_msg LRESULT OnCloseColorDlg(WPARAM wParam, LPARAM lParam);
	CSliderCtrl m_sldDsAudio;
	CSliderCtrl m_sldDsVideo;
	int m_oldAudioPos;
	int m_oldVudioPos;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SetFontObj();
	CGdipButton m_btnCurves2;		// ��Ȱ��ȭ ��ư
	void OnToggleCurveBtn();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnGrayscaleSet();
	CGdipButton m_btnGrayOnOff;
	CSliderCtrl m_sldBrightness;
	CSliderCtrl m_sldContrast;
	int setGrayscale = GRAYSCALE_OFF;
};
