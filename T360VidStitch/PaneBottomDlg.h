#pragma once


// CPaneBottomDlg ��ȭ �����Դϴ�.

class CPaneBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneBottomDlg)

public:
	CPaneBottomDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPaneBottomDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PANEBOTTOMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
};
