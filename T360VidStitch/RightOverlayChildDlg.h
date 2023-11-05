#pragma once
#include "afxwin.h"
#include "OverlayEditPositionDlg.h"


// RightOverlayChildDlg ��ȭ �����Դϴ�.

class RightOverlayChildDlg : public CDialogEx
{
	DECLARE_DYNAMIC(RightOverlayChildDlg)

public:
	RightOverlayChildDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~RightOverlayChildDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RIGHTOVERLAYCHILDDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CGdipButton m_btnAddOverlay;
	CGdipButton m_btnDelOverlay;
	CGdipButton m_btnEditOverlay;
	CGdipButton m_btnEditOverlayText;
	CGdipButton m_btnBack;
	CGdipButton m_btnNameEdit;
	CGdipButton m_btnNameSave;
	CGdipButton m_btnNameCancel;

	CEdit m_editName;

	CString m_culName, m_preName;

	COverlayEditPositionDlg* m_dlgOverlayEditPos;

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//void OnPaint();

	afx_msg void OnBnClickedBtnaddoverlay();
	afx_msg void OnBnClickedBtndeleteoverlay();
	afx_msg void OnBnClickedBtneditoverlay();
	afx_msg void OnBnClickedBtnEditText();
	afx_msg void OnBnClickedBtnback();

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	void setEditoverlay(bool isEdit);
	void setEditoverlayText(bool isEditText);
	afx_msg void OnBnClickedBtnNameEdit();
	afx_msg void OnBnClickedBtnNameSave();
	afx_msg void OnBnClickedBtnNameCancel();
	afx_msg void OnEnChangeEditName();

	void dlgUpdate();
};
