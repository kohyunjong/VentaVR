// ConfOutputsChildTwo.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "T360VidStitch.h"
#include "ConfOutputsChildTwo.h"
#include "afxdialogex.h"


// CConfOutputsChildTwo ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CConfOutputsChildTwo, CDialogEx)

CConfOutputsChildTwo::CConfOutputsChildTwo(CWnd* pParent /*=NULL*/)
: CTaskPanelDlg(pParent)
{

}

CConfOutputsChildTwo::~CConfOutputsChildTwo()
{
}

void CConfOutputsChildTwo::DoDataExchange(CDataExchange* pDX)
{
	CTaskPanelDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_CONFIG_1_2_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_CFOUT2_FPS, m_editFPS);

	DDX_Control(pDX, IDC_CFOUT2_VB, m_editVB);
	DDX_Control(pDX, IDC_CFOUT2_GOP, m_editGOP);
	DDX_Control(pDX, IDC_CBO_CFOUT2_CDEC, m_cboCodec);
	DDX_Control(pDX, IDC_CBO_CFOUT2_PS, m_cboPreset);
}


BEGIN_MESSAGE_MAP(CConfOutputsChildTwo, CTaskPanelDlg)
	ON_BN_CLICKED(IDC_BTN_CONFIG_1_2_SAVE, &CConfOutputsChildTwo::OnBnClickedBtnConfig12Save)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
	ON_CBN_SELCHANGE(IDC_CBO_CFOUT2_CDEC, &CConfOutputsChildTwo::OnCbnSelchangeCboCfout2Cdec)
	ON_EN_CHANGE(IDC_CFOUT2_FPS, &CConfOutputsChildTwo::OnEnChangeCfout2Fps)
	ON_EN_CHANGE(IDC_CFOUT2_VB, &CConfOutputsChildTwo::OnEnChangeCfout2Vb)
	ON_EN_CHANGE(IDC_CFOUT2_GOP, &CConfOutputsChildTwo::OnEnChangeCfout2Gop)
END_MESSAGE_MAP()


// CConfOutputsChildTwo �޽��� ó�����Դϴ�.


void CConfOutputsChildTwo::OnBnClickedBtnConfig12Save()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	SetDlgItemText(IDC_CFOUT2_GOP, GOP);
	SetDlgItemText(IDC_CFOUT2_VB, VB);
	SetDlgItemText(IDC_CFOUT2_FPS, FPS);
	
	this->GetValue();
	((CConfiguChildView1*)(GetParent()->GetParent()))->m_dlgsubHDDConf.GetValue();
	((CConfiguChildView1*)(GetParent()->GetParent()))->m_dlgsubBlackMConf.GetValue();

	theApp.m_PrjAllCfg.SaveConfigValue();
}


BOOL CConfOutputsChildTwo::OnInitDialog()
{
	CTaskPanelDlg::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	// 	m_btnSave.LoadImages(IDB_BTN_SAVE_NORMAL, 0, 0, IDB_BTN_SAVE_PRESSED, _T("PNG"));
	// 	m_btnSave.SetToolTipText(_T("Show Source VIew"));
	// 	m_btnSave.SizeToContent();
	// 
	// 	m_btnOutputBrowser.LoadImages(IDB_BTN_OPENFILE_NORMAL, 0, 0, 0, _T("PNG"));
	// 	m_btnOutputBrowser.SetToolTipText(_T("Show Source VIew"));
	// 	m_btnOutputBrowser.SizeToContent();

	m_btnSave.LoadBitmaps(IDBB_BTN_CONFIG_SAVE_NORMAL, IDBB_BTN_CONFIG_SAVE_PRESSED, 0, IDBB_BTN_CONFIG_SAVE_DISABLE);
	m_btnSave.SizeToContent();

	((CStatic*)GetDlgItem(IDC_STATIC_12_39))->ShowWindow(FALSE);
	((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_DYNAMICFPS))->ShowWindow(FALSE);

	//text ���ڼ� ����
	((CEdit *)GetDlgItem(IDC_CFOUT2_VB))->SetLimitText(6);
	((CEdit *)GetDlgItem(IDC_CFOUT2_GOP))->SetLimitText(3);

	SetFontObj();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


HBRUSH CConfOutputsChildTwo::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CTaskPanelDlg::OnCtlColor(pDC, pWnd, nCtlColor);
	static HBRUSH hbrBack = CreateSolidBrush(COLOR_DIALOG_NOMAL_BK);
	static HBRUSH hbrNomalBack = CreateSolidBrush(COLOR_CONFIG_NOMAL_BK);
	// TODO:  ���⼭ DC�� Ư���� �����մϴ�.
	switch (pWnd->GetDlgCtrlID())
	{
		case IDC_STATIC_12_27:
		case IDC_STATIC_12_28:
		case IDC_STATIC_12_29:
		case IDC_STATIC_12_30:
		case IDC_STATIC_12_31:
		case IDC_STATIC_12_32:
		case IDC_STATIC_12_33:
		case IDC_STATIC_12_34:
		case IDC_STATIC_12_35:
		case IDC_STATIC_12_36:
		case IDC_STATIC_12_37:
		case IDC_STATIC_12_38:
		case IDC_STATIC_12_39:
		{
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkMode(TRANSPARENT);
			hbr = (HBRUSH)GetStockObject(NULL_BRUSH);
		}
		break;
		case IDC_CFOUT2_HEIGHT:
		case IDC_CFOUT2_WIDTH:
		{
			pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
			hbr = hbrBack;
		}
		break;
		case IDC_CFOUT2_VB:
		case IDC_CFOUT2_GOP:
		case IDC_CFOUT2_FPS:
		{
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
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
// 		case IDC_CBO_CFOUT2_RCMODE:
// 		{
// 			pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT); // @KEYWORD@THEME@
// 			pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
// 			hbr = hbrBack;
// 		}
// 			break;
		default:
			pDC->SetTextColor(COLOR_DIALOG_TEXT); // @KEYWORD@THEME@
			pDC->SetBkColor(COLOR_CONFIG_NOMAL_BK);
			hbr = hbrNomalBack;
			break;
		}
		break;
	case CTLCOLOR_LISTBOX: // Listbox
		switch (pWnd->GetDlgCtrlID())
		{
// 		case IDC_CBO_CFOUT2_RCMODE:
// 		{
// 			pDC->SetTextColor(COLOR_DIALOG_GRAY_TEXT); // @KEYWORD@THEME@
// 			pDC->SetBkColor(COLOR_DIALOG_GRAY_BK);
// 			hbr = hbrBack;
// 		}
// 		break;
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


BOOL CConfOutputsChildTwo::OnEraseBkgnd(CDC* pDC)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	CRect rc;
	GetClientRect(&rc);

	pDC->FillSolidRect(&rc, COLOR_DIALOG_NOMAL_BK);
	return TRUE;
}


void CConfOutputsChildTwo::SetValues()
{
	//theApp.m_PrjAllCfg.m_RtmpOption
	//m_cboCodec.EnableWindow(TRUE);
	//m_cboPreset.EnableWindow(TRUE);

	//CString szTemp;
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_width = (theApp.m_PrjAllCfg.m_RtmpOption.width == 0 ? 3840 : theApp.m_PrjAllCfg.m_RtmpOption.width);
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_height = (theApp.m_PrjAllCfg.m_RtmpOption.height == 0 ? 1920 : theApp.m_PrjAllCfg.m_RtmpOption.height);

	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_Configuration.rtmp_width);
	//GetDlgItem(IDC_CFOUT2_WIDTH)->SetWindowText(szTemp);
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_Configuration.rtmp_height);
	//GetDlgItem(IDC_CFOUT2_HEIGHT)->SetWindowText(szTemp);

	//strcpy(theApp.m_PrjAllCfg.m_Configuration.rtmp_path, theApp.m_PrjAllCfg.m_RtmpOption.outPath);
	//szTemp.Format(_T("%s"), CString(theApp.m_PrjAllCfg.m_RtmpOption.outPath));
	//GetDlgItem(IDC_CFOUT2_SURL)->SetWindowText(szTemp.GetLength() > 0 ? szTemp : _T("rtmp://"));

	////rcMode 2 = CBR, 1 = VBR
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_bitrate_mode = (theApp.m_PrjAllCfg.m_RtmpOption.rcMode == 2 ? 0 : 1);
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_RCMODE))->SetCurSel(theApp.m_PrjAllCfg.m_RtmpOption.rcMode == 2 ? 0 : 1);

	////Video encoding bitrate 
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_bitrate = theApp.m_PrjAllCfg.m_RtmpOption.avgBitrate;
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_RtmpOption.avgBitrate);
	//GetDlgItem(IDC_CFOUT2_VB)->SetWindowText(szTemp);
	//
	//// GOP
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_gop = theApp.m_PrjAllCfg.m_RtmpOption.gop;
	//szTemp.Format(_T("%d"), theApp.m_PrjAllCfg.m_RtmpOption.gop);
	//GetDlgItem(IDC_CFOUT2_GOP)->SetWindowText(szTemp);

	////Audio encoding bitrate (kbits/s). 0�̸� 128K, 1�̸� 192K
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_audio_bitrate = theApp.m_PrjAllCfg.m_RtmpOption.audioBitrate;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_AB))->SetCurSel(theApp.m_PrjAllCfg.m_RtmpOption.audioBitrate);

	////Encoding color format. 0�̸� YUV444, 1�̸� NV12�� ����
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_color_space = theApp.m_PrjAllCfg.m_RtmpOption.colorSpace;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_CS))->SetCurSel(theApp.m_PrjAllCfg.m_RtmpOption.colorSpace);

	////preset: Encoding ���� �켱 ����. 0�̸� ȭ�� �켱, 1�̸� �ӵ� �켱���� ����.
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_preset = theApp.m_PrjAllCfg.m_RtmpOption.preset;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_PS))->SetCurSel(theApp.m_PrjAllCfg.m_Configuration.rtmp_preset);

	////fps
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_fps = theApp.m_PrjAllCfg.m_RtmpOption.fps;
	//szTemp.Format(_T("%.2lf"), theApp.m_PrjAllCfg.m_RtmpOption.fps);
	//GetDlgItem(IDC_CFOUT2_FPS)->SetWindowText(szTemp);

	////codec: 0�̸� H264, 1�̸� HEVC �ڵ����� ����.
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_codec = theApp.m_PrjAllCfg.m_RtmpOption.codec;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_CDEC))->SetCurSel(theApp.m_PrjAllCfg.m_RtmpOption.codec);
	//
	////Repeat : 0�̸� Once, 1�̸� Repeat
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_repeat = theApp.m_PrjAllCfg.m_RtmpOption.repeat;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_REPEAT))->SetCurSel(theApp.m_PrjAllCfg.m_RtmpOption.repeat == 1 ? 1 : 0);

	////Dynamic FPS : 0�̸� Off, 1�̸� On
	//theApp.m_PrjAllCfg.m_Configuration.rtmp_dynamic_fps = theApp.m_PrjAllCfg.m_RtmpOption.dynamicFPS;
	//((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_DYNAMICFPS))->SetCurSel(theApp.m_PrjAllCfg.m_RtmpOption.dynamicFPS == 1 ? 1 : 0);

	////Output�� 8K�϶� Codec�� Preset����
	//if (theApp.m_PrjAllCfg.m_nDstWidth == 7680)
	//{
	//	m_cboCodec.SetCurSel(1);
	//	m_cboCodec.EnableWindow(FALSE);
	//	m_cboPreset.SetCurSel(2);
	//	m_cboPreset.EnableWindow(FALSE);
	//}

	//GetDlgItem(IDC_CBO_CFOUT2_CS)->EnableWindow(theApp.m_PrjAllCfg.m_RtmpOption.codec == 0 ? TRUE : FALSE);


}

void CConfOutputsChildTwo::GetValue()
{
	CString szTemp;
	GetDlgItem(IDC_CFOUT2_WIDTH)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.rtmp_width = _ttoi(szTemp);

	GetDlgItem(IDC_CFOUT2_HEIGHT)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.rtmp_height = _ttoi(szTemp);

	GetDlgItem(IDC_CFOUT2_SURL)->GetWindowText(szTemp);
	char* szpPath = CString2Char(szTemp);
	strcpy(theApp.m_PrjAllCfg.m_Configuration.rtmp_path, szpPath);
	free(szpPath);
	//rcMode 2 = CBR, 32 = VBR
	theApp.m_PrjAllCfg.m_Configuration.rtmp_bitrate_mode = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_RCMODE))->GetCurSel();

	//Video encoding bitrate 
	GetDlgItem(IDC_CFOUT2_VB)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.rtmp_bitrate = _ttoi(szTemp);

	// GOP
	GetDlgItem(IDC_CFOUT2_GOP)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.rtmp_gop = _ttoi(szTemp);

	//Audio encoding bitrate (kbits/s). 0�̸� 128K, 1�̸� 192K
	theApp.m_PrjAllCfg.m_Configuration.rtmp_audio_bitrate = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_AB))->GetCurSel();

	//Encoding color format. 0�̸� YUV444, 1�̸� NV12�� ����
	theApp.m_PrjAllCfg.m_Configuration.rtmp_color_space = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_CS))->GetCurSel();

	//preset: Encoding ���� �켱 ����. 0�̸� ȭ�� �켱, 1�̸� �ӵ� �켱���� ����.
	theApp.m_PrjAllCfg.m_Configuration.rtmp_preset = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_PS))->GetCurSel();

	//fps
	GetDlgItem(IDC_CFOUT2_FPS)->GetWindowText(szTemp);
	theApp.m_PrjAllCfg.m_Configuration.rtmp_fps = _ttoi(szTemp);

	//codec: 0�̸� H264, 1�̸� HEVC �ڵ����� ����.
	theApp.m_PrjAllCfg.m_Configuration.rtmp_codec = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_CDEC))->GetCurSel();

	//Repeat : 0�̸� Once, 1�̸� Repeat
	theApp.m_PrjAllCfg.m_Configuration.rtmp_repeat = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_REPEAT))->GetCurSel();

	//Dynamic FPS : 0�̸� Off, 1�̸� On
	theApp.m_PrjAllCfg.m_Configuration.rtmp_dynamic_fps = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_DYNAMICFPS))->GetCurSel();



}

void CConfOutputsChildTwo::OnCbnSelchangeCboCfout2Cdec()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	theApp.m_PrjAllCfg.m_Configuration.rtmp_codec = ((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_CDEC))->GetCurSel();
	if (theApp.m_PrjAllCfg.m_Configuration.rtmp_codec == HEVC && GetDlgItem(IDC_CBO_CFOUT2_CS)->IsWindowEnabled())
	{
		((CComboBox*)GetDlgItem(IDC_CBO_CFOUT2_CS))->SetCurSel(1);
		GetDlgItem(IDC_CBO_CFOUT2_CS)->EnableWindow(FALSE);
	}
	
	if (theApp.m_PrjAllCfg.m_Configuration.rtmp_codec == H264)
		GetDlgItem(IDC_CBO_CFOUT2_CS)->EnableWindow(TRUE);
}
void CConfOutputsChildTwo::SetFontObj()
{
	/////////////////////////////////////////////////////////////////
	CFont* aria12 = ((CT360VidStitchDlg*)theApp.m_pMainWnd)->GetFontObj(Aria12);

	GetDlgItem(IDC_CFOUT2_WIDTH)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT2_HEIGHT)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT2_SURL)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_RCMODE)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT2_VB)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT2_GOP)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_AB)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_CS)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_PS)->SetFont(aria12);
	GetDlgItem(IDC_CFOUT2_FPS)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_CDEC)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_REPEAT)->SetFont(aria12);
	GetDlgItem(IDC_CBO_CFOUT2_DYNAMICFPS)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_27)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_28)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_29)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_30)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_31)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_32)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_33)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_34)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_35)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_36)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_37)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_38)->SetFont(aria12);
	GetDlgItem(IDC_STATIC_12_39)->SetFont(aria12);

}

void CConfOutputsChildTwo::OnEnChangeCfout2Fps()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	double fps;
	CString editValue;
	GetDlgItemText(IDC_CFOUT2_FPS, editValue);
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


double CConfOutputsChildTwo::OnChangeStrToDouble(CString strValue)
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

			SetDlgItemText(IDC_CFOUT2_FPS, strValue);
			m_editFPS.SetSel(-1, -1);
		}
	}
	return _wtof(strValue);
}

void CConfOutputsChildTwo::OnEnChangeCfout2Vb()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strValue;
	int bitrate;
	GetDlgItemText(IDC_CFOUT2_VB, strValue);
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



void CConfOutputsChildTwo::OnEnChangeCfout2Gop()
{
	// TODO:  RICHEDIT ��Ʈ���� ���, �� ��Ʈ����
	// CTaskPanelDlg::OnInitDialog() �Լ��� ������ 
	//�ϰ� ����ũ�� OR �����Ͽ� ������ ENM_CHANGE �÷��׸� �����Ͽ� CRichEditCtrl().SetEventMask()�� ȣ������ ������
	// �� �˸� �޽����� ������ �ʽ��ϴ�.

	// TODO:  ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	CString strValue;
	int gop;
	GetDlgItemText(IDC_CFOUT2_GOP, strValue);
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
