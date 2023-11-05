#pragma once


// CPaneRightDlg 대화 상자입니다.

class CPaneRightDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneRightDlg)

public:
	CPaneRightDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaneRightDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PANERIGHTVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
