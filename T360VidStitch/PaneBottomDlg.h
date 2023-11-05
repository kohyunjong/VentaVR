#pragma once


// CPaneBottomDlg 대화 상자입니다.

class CPaneBottomDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPaneBottomDlg)

public:
	CPaneBottomDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPaneBottomDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PANEBOTTOMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
};
