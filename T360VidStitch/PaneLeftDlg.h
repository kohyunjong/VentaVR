#pragma once


// CPaneLeftDlg ��ȭ �����Դϴ�.

class CPaneLeftDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneLeftDlg)

public:
	CPaneLeftDlg(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CPaneLeftDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_PANELEFTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};
