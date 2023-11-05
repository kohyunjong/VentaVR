#pragma once


// CPaneLeftDlg 대화 상자입니다.

class CPaneLeftDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneLeftDlg)

public:
	CPaneLeftDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaneLeftDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PANELEFTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
