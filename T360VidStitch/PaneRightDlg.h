#pragma once


// CPaneRightDlg ��ȭ �����Դϴ�.

class CPaneRightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneRightDlg)

public:
	CPaneRightDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPaneRightDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PANERIGHTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
