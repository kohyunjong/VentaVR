
// CurvePaneDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "CurvePaneDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.


// CCurvePaneDlg ��ȭ ����



CCurvePaneDlg::CCurvePaneDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurvePaneDlg::IDD, pParent)
{

	m_covCtrl.m_bReverse = FALSE;
	m_iComboChannel = 0;
	m_Input = 0;
	m_Output = 0;
	m_nActiveIdx = -1;
	m_pointRed_1.x = 0;
	m_pointRed_2.x = 127.5;
	m_pointRed_3.x = 255;
	m_pointGreen_1.x = 0;
	m_pointGreen_2.x = 0;
	m_pointGreen_3.x = 0;
	m_pointBlue_1.x = 0;
	m_pointBlue_2.x = 0;
	m_pointBlue_3.x = 0;

	m_pointRed_1.y = 0;
	m_pointRed_2.y = 127.5;
	m_pointRed_3.y = 255;
	m_pointGreen_1.y = 0;
	m_pointGreen_2.y = 0;
	m_pointGreen_3.y = 0;
	m_pointBlue_1.y = 0;
	m_pointBlue_2.y = 0;
	m_pointBlue_3.y = 0;

	m_nOldBtnIdx = -1;
	m_bCompareButton = FALSE;
}

void CCurvePaneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CURVCTRL, m_covCtrl);
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_comboChannelList);
	DDX_Control(pDX, IDC_EDIT_INPUT, m_editInput);
	DDX_Control(pDX, IDC_EDIT_OUTPUT, m_editOutput);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_1, m_btnThumb1);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_2, m_btnThumb2);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_3, m_btnThumb3);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_4, m_btnThumb4);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_5, m_btnThumb5);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_6, m_btnThumb6);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_7, m_btnThumb7);
	DDX_Control(pDX, IDC_BUTTON_IMAGE_8, m_btnThumb8);
}

BEGIN_MESSAGE_MAP(CCurvePaneDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CCurvePaneDlg::OnBnClickedButtonSave)
	ON_CBN_SELCHANGE(IDC_COMBO_CHANNEL, &CCurvePaneDlg::OnCbnSelchangeComboChannel)
	ON_EN_CHANGE(IDC_EDIT_INPUT, &CCurvePaneDlg::OnEnChangeEditInput)
	ON_EN_CHANGE(IDC_EDIT_OUTPUT, &CCurvePaneDlg::OnEnChangeEditOutput)
	ON_WM_SHOWWINDOW()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_1, &CCurvePaneDlg::OnBnClickedButtonImage1)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_2, &CCurvePaneDlg::OnBnClickedButtonImage2)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_3, &CCurvePaneDlg::OnBnClickedButtonImage3)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_4, &CCurvePaneDlg::OnBnClickedButtonImage4)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_5, &CCurvePaneDlg::OnBnClickedButtonImage5)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_6, &CCurvePaneDlg::OnBnClickedButtonImage6)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_7, &CCurvePaneDlg::OnBnClickedButtonImage7)
	ON_BN_CLICKED(IDC_BUTTON_IMAGE_8, &CCurvePaneDlg::OnBnClickedButtonImage8)
END_MESSAGE_MAP()

// CCurvePaneDlg �޽��� ó����

BOOL CCurvePaneDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	CXTPSkinManager* pSkinManager = XTPSkinManager();

	CXTPSkinManagerSchema* pSchema = pSkinManager->CreateSchema(theApp.GetAppDir() + SUBSKIN);
	pSkinManager->SetWindowTheme(GetSafeHwnd(), pSchema);
	CMDTARGET_RELEASE(pSchema);

	m_comboChannelList.AddString(_T("Red"));
	m_comboChannelList.AddString(_T("Green"));
	m_comboChannelList.AddString(_T("Blue"));

	m_covCtrl.InitGraphData();

	InitCurveGraph();

	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CCurvePaneDlg::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetControlValue();

	
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(FALSE);
	QVS_Engine::QVS_DoManualGainMode();
	
// 	while (!theApp.m_ScreenManager.GetGainManCurveDone())
// 		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

	theApp.m_ScreenManager.SetGainManCurveStatus(FALSE);

	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(TRUE);
}

void CCurvePaneDlg::OnCbnSelchangeComboChannel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�..
	if (!m_covCtrl.m_bFirstImageSelect)
		return;
	m_iComboChannel = m_comboChannelList.GetCurSel();
	m_bCompareButton = FALSE;
	GetControlValueAll();
	m_bCompareButton = TRUE;
	if (m_iComboChannel == COLOR_RED)
	{
		m_covCtrl.m_LineColor = LINE_COLOR_RED;
		m_covCtrl.m_iChannel = COLOR_RED;
	}
	if (m_iComboChannel == COLOR_GREEN)
	{
		m_covCtrl.m_LineColor = LINE_COLOR_GREEN;
		m_covCtrl.m_iChannel = COLOR_GREEN;
	}
	if (m_iComboChannel == COLOR_BLUE)
	{
		m_covCtrl.m_LineColor = LINE_COLOR_BLUE;
		m_covCtrl.m_iChannel = COLOR_BLUE;
	}
	m_covCtrl.m_iChannel = m_iComboChannel;
	m_covCtrl.UpdateControl();
}


void CCurvePaneDlg::OnEnChangeEditInput()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString editValue;
	if (!m_covCtrl.m_bFirstImageSelect)
		return;
	GetDlgItemText(IDC_EDIT_INPUT, editValue);
	double channelValue = OnChangeStrToDouble(editValue, 0);

	if (channelValue > 255)
	{
		SetDlgItemText(IDC_EDIT_INPUT, _T("255"));
		channelValue = 255;
	}
	m_Input = channelValue;
	if (m_covCtrl.m_bSelectPoint)
	{
		if (m_iComboChannel == 0)			// Red
		{
			switch (m_covCtrl.m_PointIndex)
			{
				case 1:	m_pointRed_1.x = channelValue; break;
				case 2: m_pointRed_2.x = channelValue; break;
				case 3: m_pointRed_3.x = channelValue; break;
			}
		}

		else if (m_iComboChannel == 1)		//Green
		{
			switch (m_covCtrl.m_PointIndex)
			{
				case 1:	m_pointGreen_1.x = channelValue; break;
				case 2: m_pointGreen_2.x = channelValue; break;
				case 3: m_pointGreen_3.x = channelValue; break;
			}
		}

		else if (m_iComboChannel == 2)		// Blue
		{
			switch (m_covCtrl.m_PointIndex)
			{
				case 1:	m_pointBlue_1.x = channelValue; break;
				case 2: m_pointBlue_2.x = channelValue; break;
				case 3: m_pointBlue_3.x = channelValue; break;
			}
		}
	}

	UpdateData();
	m_covCtrl.IOUpdate(m_Input, m_Output);
}


void CCurvePaneDlg::OnEnChangeEditOutput()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CDialogEx::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	if (!m_covCtrl.m_bFirstImageSelect)
		return;

	CString editValue;

	GetDlgItemText(IDC_EDIT_OUTPUT, editValue);
	double channelValue = OnChangeStrToDouble(editValue, 1);
	

	if (channelValue > 255)
	{
		SetDlgItemText(IDC_EDIT_OUTPUT, _T("255"));
		channelValue = 255;
	}
	m_Output = channelValue;

	if (m_covCtrl.m_bSelectPoint)
	{
		if (m_iComboChannel == 0)			// Red
		{
			switch (m_covCtrl.m_PointIndex)
			{
				case 1:	m_pointRed_1.y = channelValue; break;
				case 2: m_pointRed_2.y = channelValue; break;
				case 3: m_pointRed_3.y = channelValue; break;
			}
		}

		else if (m_iComboChannel == 1)		//Green
		{
			switch (m_covCtrl.m_PointIndex)
			{
				case 1:	m_pointGreen_1.y = channelValue; break;
				case 2: m_pointGreen_2.y = channelValue; break;
				case 3: m_pointGreen_3.y = channelValue; break;
			}
		}
		 
		else if (m_iComboChannel == 2)		// Blue
		{
			switch (m_covCtrl.m_PointIndex)
			{
				case 1:	m_pointBlue_1.y = channelValue; break;
				case 2: m_pointBlue_2.y = channelValue; break;
				case 3: m_pointBlue_3.y = channelValue; break;
			}
		}
	}

	UpdateData();
	m_covCtrl.IOUpdate(m_Input, m_Output);
}

void CCurvePaneDlg::UpdateIOEditCtrl(INT Index)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CString  strInput, strOutput;
	double input , output;
	if (m_iComboChannel == 0)			// Red
	{
		switch (Index)
		{
			case 1: output = m_pointRed_1.y; input = m_pointRed_1.x; break;
			case 2: output = m_pointRed_2.y; input = m_pointRed_2.x; break;
			case 3: output = m_pointRed_3.y; input = m_pointRed_3.x; break;
		}
	}

	else if (m_iComboChannel == 1)		//Green
	{
		switch (Index)
		{
			case 1: output = m_pointGreen_1.y; input = m_pointGreen_1.x; break;
			case 2: output = m_pointGreen_2.y; input = m_pointGreen_2.x; break;
			case 3: output = m_pointGreen_3.y; input = m_pointGreen_3.x; break;
		}
	}

	else if (m_iComboChannel == 2)		// Blue
	{
		switch (Index)
		{
			case 1: output = m_pointBlue_1.y; input = m_pointBlue_1.x; break;
			case 2: output = m_pointBlue_2.y; input = m_pointBlue_2.x; break;
			case 3: output = m_pointBlue_3.y; input = m_pointBlue_3.x; break;
		}
	}
	
	if (Index != 0 && Index != 4)
	{
		strInput.Format(_T("%.2f"), input);
		strOutput.Format(_T("%.2f"), output);

		SetDlgItemText(IDC_EDIT_INPUT, strInput);
		SetDlgItemText(IDC_EDIT_OUTPUT, strOutput);
	}
}


void CCurvePaneDlg::OnBnClickedButtonImage1()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_1);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage2()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_2);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage3()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_3);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage4()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_4);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage5()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_5);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage6()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_6);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage7()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_7);
	GetControlValueAll();
}


void CCurvePaneDlg::OnBnClickedButtonImage8()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	ClickButtons(IDC_BUTTON_IMAGE_8);
	GetControlValueAll();
}

void CCurvePaneDlg::SetControlValue()
{
	//////////////////////////////////////////////////////////////////////////////
	if (m_nActiveIdx == -1)
		return;
	QVS_Engine::QVS_SetGainChannelIndex(m_nActiveIdx);

	QVS_Engine::QVS_SetGainRGBIndex(0);		////// Red
	QVS_Engine::QVS_SetGainPointIndex(0);
	QVS_Engine::QVS_SetGainPointX1(m_pointRed_1.x);
	QVS_Engine::QVS_SetGainPointY1(m_pointRed_1.y);

	QVS_Engine::QVS_SetGainPointIndex(1);
	QVS_Engine::QVS_SetGainPointX2(m_pointRed_2.x);
	QVS_Engine::QVS_SetGainPointY2(m_pointRed_2.y);

	QVS_Engine::QVS_SetGainPointIndex(2);
	QVS_Engine::QVS_SetGainPointX3(m_pointRed_3.x);
	QVS_Engine::QVS_SetGainPointY3(m_pointRed_3.y);


	/////////////////////////////////////////////////////////////////////////
	QVS_Engine::QVS_SetGainRGBIndex(1);		///// Green
	QVS_Engine::QVS_SetGainPointIndex(0);
	QVS_Engine::QVS_SetGainPointX1(m_pointGreen_1.x);
	QVS_Engine::QVS_SetGainPointY1(m_pointGreen_1.y);

	QVS_Engine::QVS_SetGainPointIndex(1);
	QVS_Engine::QVS_SetGainPointX2(m_pointGreen_2.x);
	QVS_Engine::QVS_SetGainPointY2(m_pointGreen_2.y);

	QVS_Engine::QVS_SetGainPointIndex(2);
	QVS_Engine::QVS_SetGainPointX3(m_pointGreen_3.x);
	QVS_Engine::QVS_SetGainPointY3(m_pointGreen_3.y);


	/////////////////////////////////////////////////////////////////////////
	QVS_Engine::QVS_SetGainRGBIndex(2);		///// Blue
	QVS_Engine::QVS_SetGainPointIndex(0);
	QVS_Engine::QVS_SetGainPointX1(m_pointBlue_1.x);
	QVS_Engine::QVS_SetGainPointY1(m_pointBlue_1.y);

	QVS_Engine::QVS_SetGainPointIndex(1);
	QVS_Engine::QVS_SetGainPointX2(m_pointBlue_2.x);
	QVS_Engine::QVS_SetGainPointY2(m_pointBlue_2.y);

	QVS_Engine::QVS_SetGainPointIndex(2);
	QVS_Engine::QVS_SetGainPointX3(m_pointBlue_3.x);
	QVS_Engine::QVS_SetGainPointY3(m_pointBlue_3.y);
	
}

void CCurvePaneDlg::GetControlValue(int index)
{
	if (m_nActiveIdx == -1)
		return;

	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT_OUTPUT)->EnableWindow(TRUE);

	if (index != -1)
		UpdateIOEditCtrl(index);
}

void CCurvePaneDlg::GetControlValueAll()
{
	if (m_nActiveIdx == -1)
		return;

	if (m_bCompareButton == TRUE)
	{
		m_covCtrl.UpdateControl();
		UpdateIOEditCtrl(m_covCtrl.m_PointIndex);
		return;
	}
		

//	m_covCtrl.m_bFirstImageSelect = TRUE;

	QVS_Engine::QVS_SetGainChannelIndex(m_nActiveIdx);
	QVS_Engine::QVS_SetGainRGBIndex(0);

	QVS_Engine::QVS_SetGainPointIndex(0);
	m_pointRed_1.x = QVS_Engine::QVS_GetGainPointX1();
	m_pointRed_1.y = QVS_Engine::QVS_GetGainPointY1();

	QVS_Engine::QVS_SetGainPointIndex(1);
	m_pointRed_2.x = QVS_Engine::QVS_GetGainPointX2();
	m_pointRed_2.y = QVS_Engine::QVS_GetGainPointY2();

	QVS_Engine::QVS_SetGainPointIndex(2);
	m_pointRed_3.x = QVS_Engine::QVS_GetGainPointX3();
	m_pointRed_3.y = QVS_Engine::QVS_GetGainPointY3();
	
	////////////////////////////////////////////

	QVS_Engine::QVS_SetGainRGBIndex(1);

	QVS_Engine::QVS_SetGainPointIndex(0);
	m_pointGreen_1.x = QVS_Engine::QVS_GetGainPointX1();
	m_pointGreen_1.y = QVS_Engine::QVS_GetGainPointY1();

	QVS_Engine::QVS_SetGainPointIndex(1);
	m_pointGreen_2.x = QVS_Engine::QVS_GetGainPointX2();
	m_pointGreen_2.y = QVS_Engine::QVS_GetGainPointY2();

	QVS_Engine::QVS_SetGainPointIndex(2);
	m_pointGreen_3.x = QVS_Engine::QVS_GetGainPointX3();
	m_pointGreen_3.y = QVS_Engine::QVS_GetGainPointY3();

	////////////////////////////////////////////////

	QVS_Engine::QVS_SetGainRGBIndex(2);

	QVS_Engine::QVS_SetGainPointIndex(0);
	m_pointBlue_1.x = QVS_Engine::QVS_GetGainPointX1();
	m_pointBlue_1.y = QVS_Engine::QVS_GetGainPointY1();

	QVS_Engine::QVS_SetGainPointIndex(1);
	m_pointBlue_2.x = QVS_Engine::QVS_GetGainPointX2();
	m_pointBlue_2.y = QVS_Engine::QVS_GetGainPointY2();

	QVS_Engine::QVS_SetGainPointIndex(2);
	m_pointBlue_3.x = QVS_Engine::QVS_GetGainPointX3();
	m_pointBlue_3.y = QVS_Engine::QVS_GetGainPointY3();

	int index = 1;
	double input = 0, output = 0;

	m_covCtrl.IOUpdate(-3, -3, 0);
	m_covCtrl.IOUpdate(258, 258, 4);
	for (index=1; index<4;index++)
	{
		if (m_iComboChannel == 0)			// Red
		{
			switch (index)
			{
				case 1: input = m_pointRed_1.x; output = m_pointRed_1.y; break;
				case 2: input = m_pointRed_2.x; output = m_pointRed_2.y; break;
				case 3: input = m_pointRed_3.x; output = m_pointRed_3.y; break;
			}
		}

		else if (m_iComboChannel == 1)		//Green
		{
			switch (index)
			{
				case 1: input = m_pointGreen_1.x; output = m_pointGreen_1.y; break;
				case 2: input = m_pointGreen_2.x; output = m_pointGreen_2.y; break;
				case 3: input = m_pointGreen_3.x; output = m_pointGreen_3.y; break;
			}
		}
		else if (m_iComboChannel == 2)		// Blue
		{
			switch (index)
			{
				case 1: input = m_pointBlue_1.x; output = m_pointBlue_1.y; break;
				case 2: input = m_pointBlue_2.x; output = m_pointBlue_2.y; break;
				case 3: input = m_pointBlue_3.x; output = m_pointBlue_3.y; break;
			}
		}
		m_covCtrl.IOUpdate(input, output,index);
	}	

	if (m_covCtrl.m_PointIndex != -1)
	{
		m_covCtrl.m_PointIndex = 2;
		UpdateIOEditCtrl(m_covCtrl.m_PointIndex);
	}
		
	else
	{
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_OUTPUT)->EnableWindow(FALSE);
		SetDlgItemText(IDC_EDIT_INPUT, _T(""));
		SetDlgItemText(IDC_EDIT_OUTPUT, _T(""));
	}

}



void CCurvePaneDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	if (bShow)
	{
		m_btnSave.LoadImages(IDB_BTN_SAVE_NORMAL, 0, 0, IDB_BTN_SAVE_PRESSED, 0, 0, _T("PNG"));
		m_btnSave.SizeToContent();
	}
	LoadButtonImage(bShow);
}

void CCurvePaneDlg::LoadButtonImage(BOOL bShow)
{
	CString temp, szTempPath, strFilePath;
	//GetTempPath(_MAX_PATH, temp.GetBuffer(MAX_PATH));
	temp = theApp.GetAppDir();	//�ӽ� ���� ����...���α׷�...���丮������..
	szTempPath.Format(_T("%s"), temp);

	// Temp������ �ٸ�����?? ���߿� ���� �غ���.
	szTempPath.Format(_T("%s"), szTempPath.Right(1));
	if (szTempPath.CompareNoCase(_T("\\")) != 0)
		szTempPath.Format(_T("%s%s"), temp, _T("\\_Q360_\\"));
	else
		szTempPath.Format(_T("%s%s"), temp, _T("_Q360_\\"));

	if (bShow)
	{
		CGdipButtonExt* pGdiBtn = NULL;
		m_nActiveIdx = -1; //���ʱ�ȭ

		m_btnThumb1.ShowWindow(SW_HIDE);
		m_btnThumb2.ShowWindow(SW_HIDE);
		m_btnThumb3.ShowWindow(SW_HIDE);
		m_btnThumb4.ShowWindow(SW_HIDE);
		m_btnThumb5.ShowWindow(SW_HIDE);
		m_btnThumb6.ShowWindow(SW_HIDE);
		m_btnThumb7.ShowWindow(SW_HIDE);
		m_btnThumb8.ShowWindow(SW_HIDE);
		
		while (theApp.m_ScreenManager.GetSnapShotDone())
			((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);

		for (int i = 0; i < QVS_Engine::QVS_GetNumCameara(); i++)
		{
			switch (i)
			{
			case 0: pGdiBtn = &m_btnThumb1; break;
			case 1: pGdiBtn = &m_btnThumb2; break;
			case 2: pGdiBtn = &m_btnThumb3; break;
			case 3: pGdiBtn = &m_btnThumb4; break;
			case 4: pGdiBtn = &m_btnThumb5; break;
			case 5: pGdiBtn = &m_btnThumb6; break;
			case 6: pGdiBtn = &m_btnThumb7; break;
			case 7: pGdiBtn = &m_btnThumb8; break;
			}
			strFilePath.Format(_T("snapshot_%d.jpg"), i);
			pGdiBtn->LoadImages(szTempPath + strFilePath, IDB_POPUP_SRCTHB1+i, 0, 0, _T("PNG"));
			pGdiBtn->ShowWindow(SW_SHOW);
			pGdiBtn->EnableButton(TRUE);
			pGdiBtn->SetImage(CGdipButtonExt::STD_TYPE);
		}
		((CT360VidStitchDlg*)theApp.m_pMainWnd)->Wait(15);
	}
	else
	{
		m_btnThumb1.LoadImages(IDB_POPUP_SRCTHB1, IDB_POPUP_SRCTHB1, 0, 0, _T("PNG"));
		m_btnThumb2.LoadImages(IDB_POPUP_SRCTHB2, IDB_POPUP_SRCTHB2, 0, 0, _T("PNG"));
		m_btnThumb3.LoadImages(IDB_POPUP_SRCTHB3, IDB_POPUP_SRCTHB3, 0, 0, _T("PNG"));
		m_btnThumb4.LoadImages(IDB_POPUP_SRCTHB4, IDB_POPUP_SRCTHB4, 0, 0, _T("PNG"));
		m_btnThumb5.LoadImages(IDB_POPUP_SRCTHB5, IDB_POPUP_SRCTHB5, 0, 0, _T("PNG"));
		m_btnThumb6.LoadImages(IDB_POPUP_SRCTHB6, IDB_POPUP_SRCTHB6, 0, 0, _T("PNG"));
		m_btnThumb7.LoadImages(IDB_POPUP_SRCTHB7, IDB_POPUP_SRCTHB7, 0, 0, _T("PNG"));
		m_btnThumb8.LoadImages(IDB_POPUP_SRCTHB8, IDB_POPUP_SRCTHB8, 0, 0, _T("PNG"));
	}

}

void CCurvePaneDlg::OnClose()
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
//	LoadButtonImage(FALSE);
//	InitCurveGraph();
//	ShowWindow(SW_HIDE);
	
	theApp.m_ScreenManager.m_pBottomPanoChild->PostMessage(WM_CLOSECOLORDLG, 1, 0);
	CDialogEx::OnClose();
}

void CCurvePaneDlg::ClickButtons(int i)
{
	if (i == -1)
		InitCurveGraph();
	else
		m_covCtrl.m_bFirstImageSelect = TRUE;

	if (m_covCtrl.m_PointIndex == -1)
	{
		m_covCtrl.m_PointIndex = 2;
		m_covCtrl.m_bSelectPoint = TRUE;
	}

	if (i == IDC_BUTTON_IMAGE_1)
		m_btnThumb1.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb1.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_2)
		m_btnThumb2.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb2.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_3)
		m_btnThumb3.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb3.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_4)
		m_btnThumb4.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb4.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_5)
		m_btnThumb5.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb5.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_6)
		m_btnThumb6.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb6.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_7)
		m_btnThumb7.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb7.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_8)
		m_btnThumb8.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb8.SetImage(CGdipButtonExt::STD_TYPE);

	if (i == IDC_BUTTON_IMAGE_1)
		m_btnThumb1.SetImage(CGdipButtonExt::ALT_TYPE);
	else
		m_btnThumb1.SetImage(CGdipButtonExt::STD_TYPE);

	m_nOldBtnIdx = m_nActiveIdx;
	m_nActiveIdx = i - (IDC_BUTTON_IMAGE_1 );

	if (m_nOldBtnIdx == m_nActiveIdx)
		m_bCompareButton = TRUE;
	else
		m_bCompareButton = FALSE;
	
	if (m_covCtrl.m_bFirstImageSelect == TRUE)
	{
		GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_OUTPUT)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_CHANNEL)->EnableWindow(TRUE);
	}
}

double CCurvePaneDlg::OnChangeStrToDouble(CString strValue ,int nIdx) 
{

	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((   48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57  ) || '.' == strValue.GetAt(i))  )	//���� ,  '.' ���� Ȯ��
		{
			//  0~ 9 , '.'  �̿� �ٸ����� �Է�
			if (strValue.GetLength() > 10)	// ���ڰ� �ѹ��� ���� ������ �ʱ�ȭ 
				strValue = _T("");	
			else
				strValue.Delete(i, 1);
			if (nIdx == 0)		// input
			{
				SetDlgItemText(IDC_EDIT_INPUT, strValue);
				m_editInput.SetSel(-1, -1);
			}
				
			if (nIdx == 1)		// output
			{
				SetDlgItemText(IDC_EDIT_OUTPUT, strValue);
				m_editOutput.SetSel(-1, -1);
			}
				
		}
	}
	return _wtof(strValue);
}

void CCurvePaneDlg::InitCurveGraph()
{
	m_comboChannelList.SetCurSel(0);
	m_covCtrl.m_LineColor = LINE_COLOR_RED;
	m_covCtrl.m_iChannel = m_iComboChannel;
	m_covCtrl.m_bFirstImageSelect = FALSE;

	SetDlgItemTextW(IDC_EDIT_OUTPUT, _T(""));
	SetDlgItemTextW(IDC_EDIT_INPUT, _T(""));

	m_covCtrl.IOUpdate(-3, -3, 0);
	m_covCtrl.IOUpdate(257, 257, 4);
	m_covCtrl.IOUpdate(0, 0, 1);
	m_covCtrl.IOUpdate(127, 127, 2);
	m_covCtrl.IOUpdate(255, 255, 3);

	GetDlgItem(IDC_EDIT_INPUT)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_OUTPUT)->EnableWindow(FALSE);
}

BOOL CCurvePaneDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	switch (pMsg->wParam)
	{
	case VK_ESCAPE:
		OnClose();
	case VK_RETURN:
		return TRUE;

	default:
		break;
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
