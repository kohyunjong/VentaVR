// ConfOutputsChildOne.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfOutputsChildOne.h"
#include "afxdialogex.h"
#include "FolderDlg.h"

// CConfOutputsChildOne ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfOutputsChildOne, CDialogEx)

CConfOutputsChildOne::CConfOutputsChildOne(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfOutputsChildOne::~CConfOutputsChildOne()
{
}

void CConfOutputsChildOne::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_1_1_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BTN_CONFIG_1_BROW, m_btnOutputBrowser);
	DDX_Control(pDX, IDC_CFOUT_FPS, m_editFPS);

	DDX_Control(pDX, IDC_CFOUT_VB, m_editVideoBit);
	DDX_Control(pDX, IDC_CFOUT_GOP, m_editGOP);
	DDX_Control(pDX, IDC_CBO_CFOUT_CDEC, m_cboCodec);
	DDX_Control(pDX, IDC_CBO_CFOUT_PS, m_cboPreset);
}


BEGIN_MESSAGE_MAP(CConfOutputsChildOne, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_1_1_SAVE, &CConfOutputsChildOne::OnBnClickedBtnConfig11Save)
	ON_BN_CLICKED(IDC_BTN_CONFIG_1_BROW, &CConfOutputsChildOne::OnBnClickedBtnConfig1Brow)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_CBO_CFOUT_CDEC, &CConfOutputsChildOne::OnCbnSelchangeCboCfoutCdec)
	ON_EN_CHANGE(IDC_CFOUT_FPS, &CConfOutputsChildOne::OnEnChangeCfoutFps)
	ON_EN_CHANGE(IDC_CFOUT_VB, &CConfOutputsChildOne::OnEnChangeCfoutVb)
	ON_EN_CHANGE(IDC_CFOUT_GOP, &CConfOutputsChildOne::OnEnChangeCfoutGop)
END_MESSAGE_MAP()


// CConfOutputsChildOne �޽��� ó�����Դϴ�.


void CConfOutputsChildOne::OnBnClickedBtnConfig11Save()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemText(IDC_CFOUT_GOP, GOP);
	SetDlgItemText(IDC_CFOUT_VB, VB);
	SetDlgItemText(IDC_CFOUT_FPS, FPS);

	this->GetValue();
	((CConfiguChildView1*)(GetParent()->GetParent()))->m_dlgsubRTMPConf.GetValue();
	((CConfiguChildView1*)(GetParent()->GetParent()))->m_dlgsubBlackMConf.GetValue();

	theApp.m_PrjAllCfg.SaveConfigValue();
}


BOOL CConfOutputsChildOne::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	m_btnOutputBrowser.LoadBitmaps(IDBB_BTN_OPENFILE_NORMAL, 0, 0, IDBB_BTN_OPENFILE_DISABLE);
	m_btnOutputBrowser.SizeToContent();

	//text ���ڼ� ����
	((CEdit *)GetDlgItem(IDC_CFOUT_VB))->SetLimitText(6);
	((CEdit *)GetDlgItem(IDC_CFOUT_GOP))->SetLimitText(3);

	SetFontObj();
	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CConfOutputsChildOne::OnBnClickedBtnConfig1Brow()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	WCHAR Path[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, Path);
	CString currentPath = _T("");
	LPCTSTR lpszTitle = _T("Select the root folder for the browse dialog:");
	UINT	uFlags = BIF_RETURNONLYFSDIRS | BIF_USENEWUI;

	currentPath = Path;

	CFolderDialog dlgRoot(lpszTitle, currentPath, this, uFlags);

	if (dlgRoot.DoModal() == IDOK)
	{
		//currentPath = dlgRoot.GetFolderPath();
		GetDlgItem(IDC_CFOUT_DIR)->SetWindowText(dlgRoot.GetFolderPath());
		UpdateData(FALSE);
	}

}

HBRUSH CConfOutputsChildOne::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_GRAY_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_12_16:
		case IDC_STATIC_12_17:
		case IDC_STATIC_12_18:
		case IDC_STATIC_12_19:
		case IDC_STATIC_12_20:
		case IDC_STATIC_12_21:
		case IDC_STATIC_12_22:
		case IDC_STATIC_12_23:
		case IDC_STATIC_12_24:
		case IDC_STATIC_12_25:
		case IDC_STATIC_12_26:
		{
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		break;
		case IDC_CFOUT_DIR:
		case IDC_CFOUT_HEIGHT:
		case IDC_CFOUT_WIDTH:
		{
			pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT);
			pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
			hbr = hbrBack;
		}
		break;
		case IDC_CFOUT_VB:
		case IDC_CFOUT_GOP:
		case IDC_CFOUT_FPS:
		{
			pDC->SetTextColor(COLOR_DIALOG_TEXT);
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;

		}
		break;
	}

	switch (nCtlColor)
	{
	case CTLCOLOR_EDIT: // Edit box
		switch (pWnd->GetDlgCtrlID())
		{
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT);
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	case CTLCOLOR_LISTBOX: // Listbox
		switch (pWnd->GetDlgCtrlID())
		{
		case IDC_CBO_CFOUT_RCMODE:
			pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT);
			pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
			hbr = hbrNomalBack;
			break;
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	}	

	// TODO:  �⺻���� �������� ������ �ٸ� �귯�ø� ��ȯ�մϴ�.
	return hbr;
}


BOOL CConfOutputsChildOne::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
}

void CConfOutputsChildOne::SetValues()
{
	//theApp.m_PrjAllCfg.m_HddOptions.
	//printf("bigheadk, theApp.m_PrjAllCfg.m_HddOptions.rcMode %d, theApp.m_PrjAllCfg.m_Configuration.hdd_preset %d\n", theApp.m_PrjAllCfg.m_HddOptions.rcMode, theApp.m_PrjAllCfg.m_Configuration.hdd_preset);
	//m_cboCodec.EnableWindow(TRUE);
	//m_cboPreset.EnableWindow(TRUE);

	//CString szTemp;
	//theApp.m_PrjAllCfg.m_Configuration.hdd_width = (theApp.m_PrjAllCfg.m_HddOptions.width == 0 ? 3840 : theApp.m_PrjAllCfg.m_HddOptions.width);
	//theApp.m_PrjAllCfg.m_Configuration.hdd_height = (theApp.m_PrjAllCfg.m_HddOptions.height == 0 ? 1920 : theApp.m_PrjAllCfg.m_HddOptions.height);
	//
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_Configuration.hdd_width);
	//GetDlgItem(IDC_CFOUT_WIDTH)->SetWindowText(szTemp);
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_Configuration.hdd_height);
	//GetDlgItem(IDC_CFOUT_HEIGHT)->SetWindowText(szTemp);

	//strcpy(theApp.m_PrjAllCfg.m_Configuration.hdd_path, theApp.m_PrjAllCfg.m_HddOptions.outPath);
	//szTemp.Format(_T("%s"), CString(theApp.m_PrjAllCfg.m_Configuration.hdd_path));
	//GetDlgItem(IDC_CFOUT_DIR)->SetWindowText(szTemp.GetLength()>0 ? szTemp:_T("./"));
	//
	////rcMode 2 = CBR, 1 = VBR
	//theApp.m_PrjAllCfg.m_Configuration.hdd_bitrate_mode = (theApp.m_PrjAllCfg.m_HddOptions.rcMode == 2 ? 0 : 1);
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_RCMODE))->SetCurSel(theApp.m_PrjAllCfg.m_HddOptions.rcMode == 2 ? 0 : 1);
	//
	////Video encoding bitrate 
	//theApp.m_PrjAllCfg.m_Configuration.hdd_bitrate = theApp.m_PrjAllCfg.m_HddOptions.avgBitrate;
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_HddOptions.avgBitrate);
	//GetDlgItem(IDC_CFOUT_VB)->SetWindowText(szTemp);

	//// GOP
	//theApp.m_PrjAllCfg.m_Configuration.hdd_gop = theApp.m_PrjAllCfg.m_HddOptions.gop;
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_HddOptions.gop);
	//GetDlgItem(IDC_CFOUT_GOP)->SetWindowText(szTemp);

	////Audio encoding bitrate (kbits/s). 0�̸� 128K, 1�̸� 192K
	//theApp.m_PrjAllCfg.m_Configuration.hdd_audio_bitrate = theApp.m_PrjAllCfg.m_HddOptions.audioBitrate;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_AB))->SetCurSel(theApp.m_PrjAllCfg.m_HddOptions.audioBitrate);

	////Encoding color format. 0�̸� YUV444, 1�̸� NV12�� ����
	//theApp.m_PrjAllCfg.m_Configuration.hdd_color_space = theApp.m_PrjAllCfg.m_HddOptions.colorSpace;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_CS))->SetCurSel(theApp.m_PrjAllCfg.m_HddOptions.colorSpace);

	////preset: Encoding ���� �켱 ����. 0�̸� ȭ�� �켱, 1�̸� �ӵ� �켱���� ����.
	//theApp.m_PrjAllCfg.m_Configuration.hdd_preset = theApp.m_PrjAllCfg.m_HddOptions.preset;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_PS))->SetCurSel(theApp.m_PrjAllCfg.m_Configuration.hdd_preset);

	////fps
	//theApp.m_PrjAllCfg.m_Configuration.hdd_fps = theApp.m_PrjAllCfg.m_HddOptions.fps;
	//szTemp.Format(_T("%.2lf"), theApp.m_PrjAllCfg.m_HddOptions.fps); 
	//GetDlgItem(IDC_CFOUT_FPS)->SetWindowText(szTemp);

	////codec: 0�̸� H264, 1�̸� HEVC �ڵ����� ����.
	//theApp.m_PrjAllCfg.m_Configuration.hdd_codec = theApp.m_PrjAllCfg.m_HddOptions.codec;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_CDEC))->SetCurSel(theApp.m_PrjAllCfg.m_HddOptions.codec);

	////Output�� 8K�϶� Codec�� Preset����
	//if (theApp.m_PrjAllCfg.m_nDstWidth == 7680)
	//{
	//	m_cboCodec.SetCurSel(1);
	//	m_cboCodec.EnableWindow(FALSE);
	//	m_cboPreset.SetCurSel(2);
	//	m_cboPreset.EnableWindow(FALSE);
	//}

	//GetDlgItem(IDC_CBO_CFOUT_CS)->EnableWindow(theApp.m_PrjAllCfg.m_HddOptions.codec==0 ? TRUE :FALSE);
}

void CConfOutputsChildOne::GetValue()
{
	CString szTemp;
	GetDlgItem(IDC_CFOUT_WIDTH)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.hdd_width = _ttoi(szTemp);

	GetDlgItem(IDC_CFOUT_HEIGHT)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.hdd_height = _ttoi(szTemp);

	GetDlgItem(IDC_CFOUT_DIR)->GetWindowText(szTemp);
	char* szpPath = CString2Char(szTemp);
	strcpy(theApp.m_PrjAllCfg.m_Configuration.hdd_path, szpPath);
	free(szpPath);
	//rcMode 2 = CBR, 1 = VBR
	theApp.m_PrjAllCfg.m_Configuration.hdd_bitrate_mode = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_RCMODE))->GetCurSel();

	//Video encoding bitrate 
	GetDlgItem(IDC_CFOUT_VB)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.hdd_bitrate = _ttoi(szTemp);
		
	// GOP
	GetDlgItem(IDC_CFOUT_GOP)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.hdd_gop = _ttoi(szTemp);

	//Audio encoding bitrate (kbits/s). 0�̸� 128K, 1�̸� 192K
	theApp.m_PrjAllCfg.m_Configuration.hdd_audio_bitrate = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_AB))->GetCurSel();

	//Encoding color format. 0�̸� YUV444, 1�̸� NV12�� ����
	theApp.m_PrjAllCfg.m_Configuration.hdd_color_space = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_CS))->GetCurSel();
	
	//preset: Encoding ���� �켱 ����. 0�̸� ȭ�� �켱, 1�̸� �ӵ� �켱���� ����.
	theApp.m_PrjAllCfg.m_Configuration.hdd_preset = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_PS))->GetCurSel();
	
	//fps
	GetDlgItem(IDC_CFOUT_FPS)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.hdd_fps = _ttoi(szTemp);

	//codec: 0�̸� H264, 1�̸� HEVC �ڵ����� ����.
	theApp.m_PrjAllCfg.m_Configuration.hdd_codec = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_CDEC))->GetCurSel();
}


void CConfOutputsChildOne::OnCbnSelchangeCboCfoutCdec()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_PrjAllCfg.m_Configuration.hdd_codec = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_CDEC))->GetCurSel();
	if (theApp.m_PrjAllCfg.m_Configuration.hdd_codec == HEVC && GetDlgItem(IDC_CBO_CFOUT_CS)->IsWindowEnabled())
	{
		((CComboBox*)GetDlgItem(IDC_CBO_CFOUT_CS))->SetCurSel(1);
		GetDlgItem(IDC_CBO_CFOUT_CS)->EnableWindow(FALSE);
	}
		
	if (theApp.m_PrjAllCfg.m_Configuration.hdd_codec == H264)
		GetDlgItem(IDC_CBO_CFOUT_CS)->EnableWindow(TRUE);

}

void CConfOutputsChildOne::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CFOUT_WIDTH)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT_HEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT_DIR)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT_RCMODE)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT_VB)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT_GOP)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT_AB)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT_CS)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT_PS)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT_FPS)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT_CDEC)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_16)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_17)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_18)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_19)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_20)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_21)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_22)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_23)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_24)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_25)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_26)->SetFont(aria12);

}

void CConfOutputsChildOne::OnEnChangeCfoutFps()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	double fps;
	CString editValue;
	GetDlgItemText(IDC_CFOUT_FPS, editValue);
	fps = OnChangeStrToDouble(editValue);		//���߿� ���ϰ��� FPS �ɹ������� ������
	
	if (fps > 60.00){
		fps = 60.00;
	}
	else if (fps < 15.00){
		fps = 15.00;
	}

	editValue.Format(_T("%.2lf"), fps);
	FPS = editValue;

	UpdateData();
}



double CConfOutputsChildOne::OnChangeStrToDouble(CString strValue)
{
	for (int i = 0; i < strValue.GetLength(); i++)
	{
		if (!((48 <= strValue.GetAt(i) && strValue.GetAt(i) <= 57) || '.' == strValue.GetAt(i)))	//���� ,  '.' ���� Ȯ��
		{
			//  0~ 9 , '.'  �̿� �ٸ����� �Է�
			if (strValue.GetLength() > 10)	// ���ڰ� �ѹ��� ���� ������ �ʱ�ȭ 
				strValue = _T("");
			else
				strValue.Delete(i, 1);

			SetDlgItemText(IDC_CFOUT_FPS, strValue);
			m_editFPS.SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}

void CConfOutputsChildOne::OnEnChangeCfoutVb()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strValue;
	int bitrate;
	GetDlgItemText(IDC_CFOUT_VB, strValue);
	bitrate = _ttoi(strValue);

	if (bitrate > 100000){
		bitrate = 100000;
	}
	else if (bitrate < 512){
		bitrate = 512;
	}
	else{
	}

	strValue.Format(_T("%d"), bitrate);
	VB = strValue;

	UpdateData();
}



void CConfOutputsChildOne::OnEnChangeCfoutGop()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strValue;
	int gop;
	GetDlgItemText(IDC_CFOUT_GOP, strValue);
	gop = _ttoi(strValue);

	if (gop > 120){
		gop = 120;
	}
	else if (gop < 15){
		gop = 15;
	}
	else{
	}

	strValue.Format(_T("%d"), gop);
	GOP = strValue;

	UpdateData();
}
