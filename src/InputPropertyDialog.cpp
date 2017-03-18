// InputPropertyDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "InputPropertyDialog.h"
#include "InputPropertyConfigDialog.h"
#include "MainFrm.h"

// CInputPropertyDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CInputPropertyDialog, CSizeDialog)


BEGIN_MESSAGE_MAP(CInputPropertyDialog, CSizeDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_VIDEO_SETTING_COMBO, &CInputPropertyDialog::OnCbnSelchangeVideoSettingCombo)
	ON_CBN_SELCHANGE(IDC_VIDEO_RESOLUTION_COMBO, &CInputPropertyDialog::OnCbnSelchangeVideoResolutionCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_SETTING_COMBO, &CInputPropertyDialog::OnCbnSelchangeAudioSettingCombo)
	ON_BN_CLICKED(IDC_VIDEO_SETTING_BUTTON, &CInputPropertyDialog::OnBnClickedVideoSettingButton)
	ON_BN_CLICKED(IDC_AUDIO_SETTING_BUTTON, &CInputPropertyDialog::OnBnClickedAudioSettingButton)
	ON_BN_CLICKED(IDC_VIDEO_RESOLUTION_BUTTON, &CInputPropertyDialog::OnBnClickedVideoResolutionButton)
	//ON_MESSAGE(WM_DEVICECHANGE, &CInputPropertyDialog::OnDeviceChange)
	//ON_WM_SIZE()
	//ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_AUDIO_RESOLUTION_BUTTON, &CInputPropertyDialog::OnBnClickedAudioResolutionButton)
	ON_CBN_SELCHANGE(IDC_AUDIO_RESOLUTION_COMBO, &CInputPropertyDialog::OnCbnSelchangeAudioResolutionCombo)
	ON_BN_CLICKED(IDC_INPUT_CONFIG_BUTTON, &CInputPropertyDialog::OnBnClickedInputConfigButton)
	ON_COMMAND(ID_WDM_DEVICE, &CInputPropertyDialog::OnWdmDevice)
	ON_COMMAND(ID_WDM_CROSSBAR1, &CInputPropertyDialog::OnWdmCrossbar1)
	ON_COMMAND(ID_WDM_CROSSBAR2, &CInputPropertyDialog::OnWdmCrossbar2)
	ON_COMMAND(ID_WDM_TVAUDIO, &CInputPropertyDialog::OnWdmTvaudio)
	ON_COMMAND(ID_WDM_TVTUNER, &CInputPropertyDialog::OnWdmTvtuner)
END_MESSAGE_MAP()


CInputPropertyDialog::CInputPropertyDialog(CWnd* pParent /*=NULL*/)
	: CSizeDialog(CInputPropertyDialog::IDD, pParent)
{
	m_bFirstFlag = TRUE;
}

CInputPropertyDialog::~CInputPropertyDialog()
{
}

void CInputPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}



//////////////////////////////////////////////////
//����������
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::OnInitDialog()
{
	CSizeDialog::OnInitDialog();
	m_bFirstFlag = TRUE;	//�����ݒ�J�n

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_VIDEO_SETTING_COMBO),IDC_VIDEO_SETTING_COMBO);
	m_ToolTip.AddTool((CButton *)GetDlgItem(IDC_VIDEO_SETTING_BUTTON),IDC_VIDEO_SETTING_BUTTON);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO),IDC_VIDEO_RESOLUTION_COMBO);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_VIDEO_RESOLUTION_BUTTON),IDC_VIDEO_RESOLUTION_BUTTON);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_AUDIO_SETTING_COMBO),IDC_AUDIO_SETTING_COMBO);
	m_ToolTip.AddTool((CButton *)GetDlgItem(IDC_AUDIO_SETTING_BUTTON),IDC_AUDIO_SETTING_BUTTON);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_AUDIO_RESOLUTION_COMBO),IDC_AUDIO_RESOLUTION_COMBO);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_AUDIO_RESOLUTION_BUTTON),IDC_AUDIO_RESOLUTION_BUTTON);
	m_ToolTip.AddTool((CComboBox *)GetDlgItem(IDC_INPUT_CONFIG_BUTTON),IDC_INPUT_CONFIG_BUTTON);

	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	CComboBox* combo;
	int nInsertIndex;
	//�r�f�I�L���v�`���f�o�C�X�̃R���{�{�b�N�X���쐬
	combo = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO); assert(combo);
	if (pDSA->m_strVideoNameList.size() != 0)
	{
		for (int i=0;i<(int)pDSA->m_strVideoNameList.size();i++)
		{
			combo->AddString(pDSA->m_strVideoNameList[i].c_str());
		}
	} else
	{
		VideoDeviceFailed(0);
		CEdit* edit = (CEdit*)GetDlgItem(IDC_INPUT_ERROR_EDIT); assert(edit);
		edit->SetWindowText(_T("�r�f�I�L���v�`���f�o�C�X�����o����܂���ł���\r\n�G���R�[�h�͊J�n�ł��܂���\r\n"));
	}

	//�I�[�f�B�I�L���v�`���f�o�C�X�̃R���{�{�b�N�X���쐬
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_SETTING_COMBO); assert(combo);
	if (pDSA->m_strAudioNameList.size() != 0)
	{
		for (int i=0;i<(int)pDSA->m_strAudioNameList.size();i++)
		{
			nInsertIndex = combo->AddString(pDSA->m_strAudioNameList[i].c_str());
			combo->SetItemData(nInsertIndex,(DWORD_PTR)i);
		}
	} else
	{
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
		CWnd* wnd = (CWnd*)GetDlgItem(IDC_AUDIO_SETTING_BUTTON); assert(wnd);
		wnd->EnableWindow(FALSE);
		//�G���[�\��
		CEdit* edit = (CEdit*)GetDlgItem(IDC_INPUT_ERROR_EDIT); assert(edit);
		CString strErr;
		edit->GetWindowText(strErr);
		strErr += _T("�I�[�f�B�I�L���v�`���f�o�C�X�����o����܂���ł���\r\n�G���R�[�h�͊J�n�ł��܂���"); 
		edit->SetWindowText(strErr);
	}

	//�v���t�@�C���̃��[�h
	LoadProfileSetting();

	m_bFirstFlag = FALSE;	//�����ݒ�t���O�̏I��

	return TRUE;
}


//////////////////////////////////////////////////
//�_�C�A���O�I����
//////////////////////////////////////////////////
void CInputPropertyDialog::OnDestroy()
{
	//�v���t�@�C���̃Z�[�u
	SaveProfileSetting();

	CDialog::OnDestroy();
}
//////////////////////////////////////////////////
//�c�[���`�b�v�̕\��
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE: 
			m_ToolTip.RelayEvent(pMsg);
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////
//�x���R���g���[���ɂ��Ẵu���V�F�ύX
//////////////////////////////////////////////////
HBRUSH CInputPropertyDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	if (nCtlColor == CTLCOLOR_STATIC) {
		if (pWnd->GetDlgCtrlID() == IDC_INPUT_ERROR_EDIT)
			pDC->SetTextColor(RGB(255,0,0));
	}
	return hbr;
}
//////////////////////////////////////////////////
//�r�f�I�L���v�`���f�o�C�X(�R���{�{�b�N�X)��
//�I�������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CInputPropertyDialog::OnCbnSelchangeVideoSettingCombo()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO);
	CComboBox* combo_resolution = (CComboBox*)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO);
	if (!combo || !combo_resolution)
		return;
	int nIndex = combo->GetCurSel();
	if (nIndex == CB_ERR) return;

	//�t�B���^�̈ꎞ��~
	BOOL bPreview = pDSA->IsRunFilter();
	if (m_bFirstFlag == FALSE && bPreview)
		pDSA->StopFilter();

	//�r�f�I��ύX����
	if (!(pDSA->SelectVideoIndex(nIndex)))
	{
		AfxMessageBox(_T("�r�f�I�L���v�`���f�o�C�X�̊m�ۂɎ��s���܂���"));
		VideoDeviceFailed(1);
		return;
	}

	//�\���{�^�����L���ɂȂ�
	CWnd* wnd = GetDlgItem(IDC_VIDEO_SETTING_BUTTON); assert(wnd);
	if (wnd->IsWindowEnabled() == FALSE)
		wnd->EnableWindow(TRUE);

	//�𑜓x�ꗗ���擾����
	vector<wstring> video_resolution = ((CKTEApp*)AfxGetApp())->GetDSA()->m_strVideoSettingList;
	if (video_resolution.size() == 0) {
		//�𑜓x�ꗗ�ݒ肪�S�������ꍇ
		VideoDeviceFailed(2);
		return;
	}

	//�r�f�I�L���v�`���f�o�C�X�̐ݒ�
	if (combo_resolution->IsWindowEnabled() == FALSE)
		combo_resolution->EnableWindow(TRUE);
	combo_resolution->ResetContent();
	int nInsertIndex;
	for (int i=0;i<(int)video_resolution.size();i++)
	{
		//��FORMAT_VideoInfo2�̃^�C�v�ɂ��Ă͋L�ڂ��Ȃ�
		if (pDSA->m_VideoMediaTypeList[i].formattype == FORMAT_VideoInfo)
		{
			nInsertIndex = combo_resolution->AddString(video_resolution[i].c_str());
			combo_resolution->SetItemData(nInsertIndex,i);	//�C���f�b�N�X�𖄂ߍ���
		}
	}
	//����N�����݂̂̃f�t�H���g�l��ݒ肷��
	if (m_bFirstFlag)
	{
		if (pProfile->m_nVideoSettingIndex >= 0 && pProfile->m_nVideoSettingIndex < (int)video_resolution.size())
		{
			if (pProfile->m_strVideoSettingName.Compare(video_resolution[pProfile->m_nVideoSettingIndex].c_str()) == 0)
				combo_resolution->SetCurSel(pProfile->m_nVideoSettingIndex);	//n�Ԗڂ̃r�f�I��I��
		}
	}
	if (combo_resolution->GetCurSel() == CB_ERR)
		combo_resolution->SetCurSel(0);	//0�Ԗڂ̃r�f�I�𑜓x��I��

	//�r�f�I�𑜓x�R���{�{�b�N�X��I���������W���[�����Ăяo��
	OnSelectVideoSettingIndex(NULL);
	//�I�[�f�B�I�s���̗L�����m�F���A�I�[�f�B�I�L���v�`�����X�g�ɒǉ�����
	ProcVideoAudioInputPin();
	return;
}
//////////////////////////////////////////////////
//�r�f�I�L���v�`���f�o�C�X(�R���{�{�b�N�X)�̉𑜓x�ݒ��
//�I�������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CInputPropertyDialog::OnCbnSelchangeVideoResolutionCombo()
{
	OnSelectVideoSettingIndex(NULL);
}
//////////////////////////////////////////////////
//�r�f�I�L���v�`���f�o�C�X(�R���{�{�b�N�X)�̉𑜓x�ݒ��
//�I�������Ƃ��ɌĂ΂��C�x���g�n���h���̒��g
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::OnSelectVideoSettingIndex(AM_MEDIA_TYPE* pamt)
{
	CComboBox*  combo = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO);
	if (!combo) return FALSE;
	if (combo->IsWindowEnabled() == FALSE) return FALSE;
	if (combo->GetCurSel() == CB_ERR) return FALSE;
	CComboBox* combo_resolution = (CComboBox*)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO);
	if (!combo_resolution) return FALSE;
	if (combo_resolution->IsWindowEnabled() == FALSE) return FALSE;
	if (combo_resolution->GetCurSel() == CB_ERR) return FALSE;

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	//�t�B���^�̈ꎞ��~
	BOOL bPreview = pDSA->IsRunFilter();
	if (bPreview)
		pDSA->StopFilter();

	//���̉𑜓x�ύX���t�B���^�č\�z�̕��@
	int nIndex = (int)combo_resolution->GetItemData(combo_resolution->GetCurSel());

	if (!pDSA->SelectVideoSettingIndex(nIndex,pamt))
	{
		AfxMessageBox(_T("�r�f�I�𑜓x�̕ύX�Ɏ��s���܂���"));
		VideoDeviceFailed(3);
		return FALSE;
	}

	//�f�o�C�X�̃L���p�V�e�B�]�[����ݒ�(���ƍ����ƃt���[�����[�g)
	VideoDeviceCapacity(nIndex);

	//////////////////////////////////////////////////
	//�v���r���[�̍ĊJ
	//�J��������L����Ă���ꍇ�̓^�C�~���O�Ŏ��s����
	if (m_bFirstFlag == FALSE && !pDSA->IsRunFilter() && pDSA->IsCanPreview())
	{
		//���̃^�C�~���O�ŉ𑜓x���ύX���ꂽ�|���v���r���[�E�B���h�E�ɒʒm����
		((CMainFrame*)AfxGetMainWnd())->GetPreviewWnd()->SetPreviewWnd();
		((CMainFrame*)AfxGetMainWnd())->GetPreviewWnd()->SetPreviewSize(m_bFirstFlag?FALSE:TRUE);
		BOOL bRet = pDSA->RunFilter();
		if (bRet == FALSE)
		{
			VideoDeviceFailed(1);
		}
	}
	return TRUE;
}


//////////////////////////////////////////////////
//�r�f�I�f�o�C�X�̃L���p�V�e�B�]�[����ݒ�(�r�f�I�̕��E�����E�t���[�����[�g�E�K�p�{�^��)
//int nIndex �ݒ�󋵂��擾����AM_MEDIA_TYPE�̃C���f�b�N�X
//TODO 64�r�b�g�J�������ɂĂ��̏����ɖ�肠��(�����p�����[�^���擾�ł��Ȃ����)
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::VideoDeviceCapacity(int nIndex)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (nIndex < 0 || nIndex >= (int)pDSA->m_VideoCapsList.size()) return FALSE;

	//���݂̃r�f�I�ݒ�̃L���p�V�e�B�ő�E�ŏ��l���擾
	VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)pDSA->m_VideoFirstMediaType.pbFormat;					//���݂̐ݒ�l
	VIDEO_STREAM_CONFIG_CAPS* pvscc = (VIDEO_STREAM_CONFIG_CAPS*)&(pDSA->m_VideoCapsList[nIndex]);	//�L���v�`���r�f�I�f�o�C�X�̐��\

	if (pvih == NULL) {
		pvih = (VIDEOINFOHEADER*)pDSA->m_VideoMediaTypeList[nIndex].pbFormat;			//�K�p�����ݒ�l
	}

	//�r�f�I�̕�
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_WIDTH_SPIN); assert(spin);
	spin->SetRange32(pvscc->MinOutputSize.cx,pvscc->MaxOutputSize.cx);
	spin->SetPos32(pvih->bmiHeader.biWidth);
	if (pvscc->MinOutputSize.cx != pvscc->MaxOutputSize.cx) {
		spin->EnableWindow(TRUE);
		GetDlgItem(IDC_VIDEO_WIDTH_EDIT)->EnableWindow(TRUE);
	} else {
		spin->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_WIDTH_EDIT)->EnableWindow(FALSE);
	}

	//�r�f�I�̍���
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_HEIGHT_SPIN); assert(spin);
	spin->SetRange32(pvscc->MinOutputSize.cy,pvscc->MaxOutputSize.cy);
	spin->SetPos32(pvih->bmiHeader.biHeight);
	if (pvscc->MinOutputSize.cy != pvscc->MaxOutputSize.cy) {
		spin->EnableWindow(TRUE);
		GetDlgItem(IDC_VIDEO_HEIGHT_EDIT)->EnableWindow(TRUE);
	} else {
		spin->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_HEIGHT_EDIT)->EnableWindow(FALSE);
	}
	//�r�f�I�̃t���[�����[�g
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN); assert(spin);
	double dCurrentFPS,dMinFPS,dMaxFPS;
	if (pvih->AvgTimePerFrame != 0)
		dCurrentFPS = (double)((double)10000000.0/(double)pvih->AvgTimePerFrame);
	else
		dCurrentFPS = 0.0;
	if (pvscc->MaxFrameInterval != 0)
		dMinFPS = (double)((double)10000000.0/(double)pvscc->MaxFrameInterval);
	else
		dMinFPS = 0.0;
	if (pvscc->MinFrameInterval != 0)
		dMaxFPS = (double)((double)10000000.0/(double)pvscc->MinFrameInterval);
	else
		dMaxFPS = 0.0;
	//spin->SetRange32((int)(dMinFPS*100),(int)(dMaxFPS*100));
	//spin->SetPos32((int)(dCurrentFPS*100));
	spin->SetRange32((int)(dMinFPS),(int)(dMaxFPS));
	spin->SetPos32((int)dCurrentFPS);
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT); assert(edit);
	CString strNum;
	if (((int)(dCurrentFPS*100)) % 10 != 0)
		strNum.Format(_T("%.2f"),dCurrentFPS);
	else if (((int)(dCurrentFPS*10)) % 10 != 0)
		strNum.Format(_T("%.1f"),dCurrentFPS);
	else
		strNum.Format(_T("%.0f"),dCurrentFPS);
	edit->SetWindowText(strNum);

	if (dMinFPS != dMaxFPS) {
		spin->EnableWindow(TRUE);
		edit->EnableWindow(TRUE);
	} else {
		spin->EnableWindow(FALSE);
		edit->EnableWindow(FALSE);
	}

	//�K�p�{�^���̗L���E����
	if (!GetDlgItem(IDC_VIDEO_WIDTH_EDIT)->IsWindowEnabled() && 
		!GetDlgItem(IDC_VIDEO_HEIGHT_EDIT)->IsWindowEnabled() &&
		!GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT)->IsWindowEnabled())
		GetDlgItem(IDC_VIDEO_RESOLUTION_BUTTON)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_VIDEO_RESOLUTION_BUTTON)->EnableWindow(TRUE);

	//�c�[���`�b�v�̔z�u
	strNum.Format(_T("���̍ő�l:%d\r\n���̍ŏ��l:%d\r\n���̑���:%d"),
		pvscc->MaxOutputSize.cx,pvscc->MinOutputSize.cx,pvscc->OutputGranularityX);
	m_ToolTip.DelTool(GetDlgItem(IDC_VIDEO_WIDTH_EDIT));
	m_ToolTip.AddTool(GetDlgItem(IDC_VIDEO_WIDTH_EDIT),strNum);
	strNum.Format(_T("�����̍ő�l:%d\r\n�����̍ŏ��l:%d\r\n�����̑���:%d"),
		pvscc->MaxOutputSize.cy,pvscc->MinOutputSize.cy,pvscc->OutputGranularityY);
	m_ToolTip.DelTool(GetDlgItem(IDC_VIDEO_HEIGHT_EDIT));
	m_ToolTip.AddTool(GetDlgItem(IDC_VIDEO_HEIGHT_EDIT),strNum);
	strNum.Format(_T("�t���[�����[�g�̍ő�l:%.2f\r\n�t���[�����[�g�̍ŏ��l:%.2f"),dMaxFPS,dMinFPS);
	m_ToolTip.DelTool(GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT));
	m_ToolTip.AddTool(GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT),strNum);

	return TRUE;
}

//////////////////////////////////////////////////
//�r�f�I�𑜓x�̓K�p�{�^��
//////////////////////////////////////////////////
void CInputPropertyDialog::OnBnClickedVideoResolutionButton()
{
	//�K�p�{�^�����L�����ǂ����`�F�b�N����
	CComboBox*  combo = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO);
	if (!combo) return;
	if (combo->IsWindowEnabled() == FALSE) return;
	if (combo->GetCurSel() == CB_ERR) return;
	CComboBox* combo_resolution = (CComboBox*)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO);
	if (!combo_resolution) return;
	if (combo_resolution->IsWindowEnabled() == FALSE) return;
	if (combo_resolution->GetCurSel() == CB_ERR) return;

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	//�ݒ�̎擾
	int nWidth , nHeight;
	double dFPS;
	GetVideoCurrentInput(nWidth,nHeight,dFPS);

	//�t�B���^�̈ꎞ��~
	BOOL bPreview = pDSA->IsRunFilter();
	if (bPreview == TRUE)
		pDSA->StopFilter();

	//���f�B�A�^�C�v�̉���
	AM_MEDIA_TYPE amt;
	ZeroMemory(&amt,sizeof(AM_MEDIA_TYPE));
	amt = pDSA->m_VideoFirstMediaType;
	VIDEOINFOHEADER* pvih = (VIDEOINFOHEADER*)amt.pbFormat;
	if (pvih)
	{
		pvih->bmiHeader.biWidth = nWidth;
		pvih->bmiHeader.biHeight = nHeight;
		if (dFPS != 0.0)
			pvih->AvgTimePerFrame = (REFERENCE_TIME)(((double)10000000)/dFPS);
		//�T���v���T�C�Y��ύX
		ULONG ISampleSize = (nWidth*nHeight*pvih->bmiHeader.biBitCount)/8;
		amt.lSampleSize = ISampleSize;
		pvih->bmiHeader.biSizeImage = ISampleSize;

		//�ݒ�̓K�p
		if (!pDSA->SelectVideoSettingIndex(pDSA->GetVideoSettingIndex(),&amt)) {
			AfxMessageBox(_T("�r�f�I�ݒ�̓K�p�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
			return;
		}
	}

	//�r�f�I�ݒ�l�̎擾
	int nIndex = (int)combo_resolution->GetItemData(combo_resolution->GetCurSel());
	VideoDeviceCapacity(nIndex);

	//�v���r���[�̍ĊJ
	if (m_bFirstFlag == FALSE && pDSA->IsRunFilter() == FALSE && pDSA->IsCanPreview() == TRUE)
	{
		//���̃^�C�~���O�ŉ𑜓x���ύX���ꂽ�|���v���r���[�E�B���h�E�ɒʒm����
		((CMainFrame*)AfxGetMainWnd())->GetPreviewWnd()->SetPreviewWnd();
		((CMainFrame*)AfxGetMainWnd())->GetPreviewWnd()->SetPreviewSize(m_bFirstFlag?FALSE:TRUE);
		BOOL bRet = pDSA->RunFilter();
	}
	return;
}



//////////////////////////////////////////////////
//�I�[�f�B�I�R���{�{�b�N�X��I�������ۂɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CInputPropertyDialog::OnCbnSelchangeAudioSettingCombo()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_SETTING_COMBO); assert(combo);
	if (!combo) return;
	if (combo->IsWindowEnabled() == FALSE) return;
	if (combo->GetCurSel() == CB_ERR) return;

	int nIndex = (int)combo->GetItemData(combo->GetCurSel());

	if (nIndex == SHARE_VIDEO_AUDIO) {
		if (!pDSA->IsVidoFindAudioPin()) {
			AfxMessageBox(_T("�r�f�I�L���v�`���f�o�C�X��ɃI�[�f�B�I��������܂���ł���"));
			//TODO �\���{�^������������Ȃ����邩�A
			//�O�ɑI��ł�����Ԃɖ߂�������
			return;
		}
	}

	//�t�B���^�̈ꎞ��~
	BOOL bPreview = pDSA->IsRunFilter();
	if (bPreview)
		pDSA->StopFilter();

	//
	if (nIndex >= 0 && nIndex<(int)pDSA->m_strAudioNameList.size()){
		//���ʂ̃I�[�f�B�I�L���v�`���J�[�h���I�����ꂽ�ꍇ
		if (!pDSA->SelectAudioIndex(nIndex,FALSE))
		{
			AfxMessageBox(_T("�I�[�f�B�I�L���v�`���f�o�C�X�̊m�ۂɎ��s���܂���"));
		}
	} else if (nIndex == SHARE_VIDEO_AUDIO){
		//�r�f�I�t�B���^�[���L���Ă���I�[�f�B�I�L���v�`���J�[�h���g�p����ꍇ
		//�����𒼂���
		int nAudioIndex = pDSA->GetAudioIndex();
		if (nAudioIndex == -1)
			nAudioIndex = 0;
		if (!pDSA->SelectAudioIndex(nAudioIndex,TRUE)){
			AfxMessageBox(_T("�r�f�I�L���v�`���f�o�C�X��̃I�[�f�B�I�L���v�`���f�o�C�X�̊m�ۂɎ��s���܂���"));
		}
	}

	//�ڍ׃R���{�{�b�N�X�̕ҏW
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_RESOLUTION_COMBO); assert(combo);
	combo->ResetContent();
	if (!combo->IsWindowEnabled()) combo->EnableWindow(TRUE);
	int nInsertIndex;
	for (int i=0;i<(int)pDSA->m_strAudioSettingList.size();i++)
	{
		nInsertIndex = combo->AddString(pDSA->m_strAudioSettingList[i].c_str());
		combo->SetItemData(nInsertIndex,(DWORD_PTR)i);
	}
	if (combo->GetCount() == 0)
	{
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
	}

	//����N�����݂̂̃f�t�H���g�l��ݒ肷��
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	if (m_bFirstFlag)
	{
		if (pProfile->m_nAudioSettingIndex >= 0 && pProfile->m_nAudioSettingIndex < (int)pDSA->m_strAudioSettingList.size())
		{
			if (pProfile->m_strAudioSettingName.Compare(pDSA->m_strAudioSettingList[pProfile->m_nAudioSettingIndex].c_str()) == 0)
				combo->SetCurSel(pProfile->m_nAudioSettingIndex);	//n�Ԗڂ̐ݒ��I��
		}
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);	//0�Ԗڂ̐ݒ��I��

	//�I�[�f�B�I�R���{�{�b�N�X��ύX
	OnCbnSelchangeAudioResolutionCombo();

}
//////////////////////////////////////////////////
//�I�[�f�B�I�̏ڍאݒ�R���{�{�b�N�X��ύX�����ۂɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CInputPropertyDialog::OnCbnSelchangeAudioResolutionCombo()
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_RESOLUTION_COMBO); assert(combo);
	if (!combo)
		return;
	if (combo->IsWindowEnabled() == FALSE)
		return;
	if (combo->GetCurSel() == CB_ERR)
		return;

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	
	//�t�B���^�̈ꎞ��~
	BOOL bPreview = pDSA->IsRunFilter();
	if (bPreview)
		pDSA->StopFilter();

	//�I�[�f�B�I�ݒ�̕ύX���s��
	int nIndex = (int)combo->GetItemData(combo->GetCurSel());
	if (!pDSA->SelectAudioSettingIndex(nIndex,NULL))
	{
		AfxMessageBox(_T("�I�[�f�B�I�ݒ�̕ύX�Ɏ��s���܂���"));
		return;
	}
	//�I�[�f�B�I�f�o�C�X�̃L���p�V�e�B�]�[����ݒ�
	AudioDeviceCapacity(nIndex);

	//�v���r���[�̍ĊJ
	if (!m_bFirstFlag && !pDSA->IsRunFilter() && pDSA->IsCanPreview())
	{
		pDSA->RunFilter();
	}
	return;
}

//////////////////////////////////////////////////
//�I�[�f�B�I�̃L���p�V�e�B�]�[����ݒ�(���g���E����\�E�`�����l���E�K�p�{�^��)
//int nIndex �ݒ�󋵂��擾����AM_MEDIA_TYPE�̃C���f�b�N�X
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::AudioDeviceCapacity(int nIndex)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (nIndex < 0 || nIndex >= (int)pDSA->m_AudioCapsList.size()) return FALSE;

	WAVEFORMATEX* pWaveFormatEx = (WAVEFORMATEX*)pDSA->m_AudioFirstMediaType.pbFormat;
	AUDIO_STREAM_CONFIG_CAPS* pascc = &(pDSA->m_AudioCapsList[nIndex]);

	int nInsertIndex;
	CString strNum;
	//���g��
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_FREQ_COMBO); assert(combo); 
	combo->ResetContent();
	if (!combo->IsWindowEnabled())
		combo->EnableWindow(TRUE);

	ULONG i = pascc->MinimumSampleFrequency;
	do {
		strNum.Format(_T("%d"),i);
		nInsertIndex = combo->AddString(strNum);
		combo->SetItemData(nInsertIndex,(DWORD_PTR)i);
		if (i == pWaveFormatEx->nSamplesPerSec)
			combo->SetCurSel(nInsertIndex);
		i += pascc->SampleFrequencyGranularity;
	} while (i <= pascc->MaximumSampleFrequency && pascc->SampleFrequencyGranularity != 0);
	if (combo->GetCount() == 0)
	{
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);
	if (pascc->MinimumSampleFrequency == pascc->MaximumSampleFrequency)	//�ݒ肪�ύX�ł��Ȃ�
		combo->EnableWindow(FALSE);

	//����\
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_BITS_COMBO); assert(combo); 
	combo->ResetContent();
	if (!combo->IsWindowEnabled())
		combo->EnableWindow(TRUE);
	i = pascc->MinimumBitsPerSample;
	do {
		strNum.Format(_T("%d"),i);
		nInsertIndex = combo->AddString(strNum);
		combo->SetItemData(nInsertIndex,(DWORD_PTR)i);
		if (i == pWaveFormatEx->wBitsPerSample)
			combo->SetCurSel(nInsertIndex);
		i += pascc->BitsPerSampleGranularity;
	} while (i <= pascc->MaximumBitsPerSample && pascc->BitsPerSampleGranularity != 0);
	if (combo->GetCount() == 0)
	{
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);
	if (pascc->MinimumBitsPerSample == pascc->MaximumBitsPerSample)	//�ݒ肪�ύX�ł��Ȃ�
		combo->EnableWindow(FALSE);

	//�`�����l��
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_CHANNELS_COMBO); assert(combo); 
	combo->ResetContent();
	if (!combo->IsWindowEnabled())
		combo->EnableWindow(TRUE);
	i = pascc->MinimumChannels;
	do {
		strNum.Format(_T("%d"),i);
		nInsertIndex = combo->AddString(strNum);
		combo->SetItemData(nInsertIndex,(DWORD_PTR)i);
		if (i == pWaveFormatEx->nChannels)
			combo->SetCurSel(nInsertIndex);
		i += pascc->ChannelsGranularity;
	} while (i <= pascc->MaximumChannels && pascc->ChannelsGranularity != 0);
	if (combo->GetCount() == 0)
	{
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);
	if (pascc->MinimumChannels == pascc->MaximumChannels)	//�ݒ肪�ύX�ł��Ȃ�
		combo->EnableWindow(FALSE);

	//�K�p�{�^���̗L���E����
	if (!GetDlgItem(IDC_AUDIO_FREQ_COMBO)->IsWindowEnabled() && 
		!GetDlgItem(IDC_AUDIO_BITS_COMBO)->IsWindowEnabled() &&
		!GetDlgItem(IDC_AUDIO_CHANNELS_COMBO)->IsWindowEnabled())
		GetDlgItem(IDC_AUDIO_RESOLUTION_BUTTON)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_AUDIO_RESOLUTION_BUTTON)->EnableWindow(TRUE);

	//�c�[���`�b�v�̔z�u
	strNum.Format(_T("���g���̍ő�l:%d\r\n���g���̍ŏ��l:%d\r\n���g���̑���:%d"),
		pascc->MaximumSampleFrequency,pascc->MinimumSampleFrequency,pascc->SampleFrequencyGranularity);
	m_ToolTip.DelTool(GetDlgItem(IDC_AUDIO_FREQ_COMBO));
	m_ToolTip.AddTool(GetDlgItem(IDC_AUDIO_FREQ_COMBO),strNum);
	strNum.Format(_T("����\�̍ő�l:%d\r\n����\�̍ŏ��l:%d\r\n����\�̑���:%d"),
		pascc->MaximumBitsPerSample,pascc->MinimumBitsPerSample,pascc->BitsPerSampleGranularity);
	m_ToolTip.DelTool(GetDlgItem(IDC_AUDIO_BITS_COMBO));
	m_ToolTip.AddTool(GetDlgItem(IDC_AUDIO_BITS_COMBO),strNum);
	strNum.Format(_T("�`�����l���̍ő�l:%d\r\n�`�����l���̍ŏ��l:%d\r\n�`�����l���̑���:%d"),
		pascc->MaximumChannels,pascc->MinimumChannels,pascc->ChannelsGranularity);
	m_ToolTip.DelTool(GetDlgItem(IDC_AUDIO_CHANNELS_COMBO));
	m_ToolTip.AddTool(GetDlgItem(IDC_AUDIO_CHANNELS_COMBO),strNum);	

	return TRUE;
}

//////////////////////////////////////////////////
//�I�[�f�B�I�R���{�{�b�N�X�̓K�p�{�^��
//////////////////////////////////////////////////
void CInputPropertyDialog::OnBnClickedAudioResolutionButton()
{
	//�K�p�{�^�����L�����ǂ����`�F�b�N����
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_SETTING_COMBO); assert(combo);
	if (!combo)return;
	if (!combo->IsWindowEnabled()) return;
	if (combo->GetCurSel() == CB_ERR) return;
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_RESOLUTION_COMBO); assert(combo);
	if (!combo)return;
	if (!combo->IsWindowEnabled()) return;
	if (combo->GetCurSel() == CB_ERR) return;

	//�ݒ�̎擾
	DWORD nSamplesPerSec;
	WORD wBitsPerSample;
	WORD nChannels;
	GetAudioCurrentInput(nSamplesPerSec,wBitsPerSample,nChannels);

	//�t�B���^�̈ꎞ��~
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	BOOL bPreview = pDSA->IsRunFilter();
	if (bPreview == TRUE)
		pDSA->StopFilter();

	//���f�B�A�^�C�v������
	AM_MEDIA_TYPE amt;
	ZeroMemory(&amt,sizeof(AM_MEDIA_TYPE));
	amt = pDSA->m_AudioFirstMediaType;
	WAVEFORMATEX* pWaveFormatEx = (WAVEFORMATEX*)amt.pbFormat;
	pWaveFormatEx->nSamplesPerSec = nSamplesPerSec;
	pWaveFormatEx->wBitsPerSample = wBitsPerSample;
	pWaveFormatEx->nChannels = nChannels;
	pWaveFormatEx->nBlockAlign = (nChannels * wBitsPerSample) / 8;
	pWaveFormatEx->nAvgBytesPerSec = nSamplesPerSec * pWaveFormatEx->nBlockAlign;

	//�ݒ�̓K�p
	if (!pDSA->SelectAudioSettingIndex(pDSA->GetAudioSettingIndex(),&amt)) {
		AfxMessageBox(_T("�I�[�f�B�I�ݒ�̓K�p�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return;
	}

	//�I�[�f�B�I�ݒ�l�̎擾
	//int nIndex = (int)combo->GetItemData(combo->GetCurSel());
	//AudioDeviceCapacity(nIndex);

	//�v���r���[�̍ĊJ
	if (!m_bFirstFlag && !pDSA->IsRunFilter() && pDSA->IsCanPreview())
	{
		pDSA->RunFilter();
	}
	return;
}


//////////////////////////////////////////////////
//�r�f�I�\���{�^�� (�_�C�A���O��\������)
//////////////////////////////////////////////////
void CInputPropertyDialog::OnBnClickedVideoSettingButton()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	if (pDSA->GetWDMFilterID(WDM_FILTER_CROSS_BAR1) == -1 && 
		pDSA->GetWDMFilterID(WDM_FILTER_CROSS_BAR2) == -1 &&
		pDSA->GetWDMFilterID(WDM_FILTER_TV_AUDIO) == -1 &&
		pDSA->GetWDMFilterID(WDM_FILTER_TV_TUNER) == -1) {
			pDSA->ShowPropertySetting(TRUE,WDM_FILTER_NONE,this->GetSafeHwnd());
			return;
	}

	//WDM�f�o�C�X�t�B���^�[�����݂���̂ŁA�ʂ̕��@(���j���[�\���^)�ŕ\��
	CMenu cMenu;
	cMenu.LoadMenu(IDR_WDM_POPUP);
	CMenu* pPopup = cMenu.GetSubMenu(0);
	assert(pPopup);
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow();

	if (pDSA->GetWDMFilterID(WDM_FILTER_CROSS_BAR1) == -1)
		pPopup->EnableMenuItem(ID_WDM_CROSSBAR1,MF_GRAYED | MF_BYCOMMAND);
	if (pDSA->GetWDMFilterID(WDM_FILTER_CROSS_BAR2) == -1)
		pPopup->EnableMenuItem(ID_WDM_CROSSBAR2,MF_GRAYED | MF_BYCOMMAND);
	if (pDSA->GetWDMFilterID(WDM_FILTER_TV_AUDIO) == -1)
		pPopup->EnableMenuItem(ID_WDM_TVAUDIO,MF_GRAYED | MF_BYCOMMAND);
	if (pDSA->GetWDMFilterID(WDM_FILTER_TV_TUNER) == -1)
		pPopup->EnableMenuItem(ID_WDM_TVTUNER,MF_GRAYED | MF_BYCOMMAND);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	cMenu.DestroyMenu();
}
//////////////////////////////////////////////////
//�r�f�I�\�����j���[ (�_�C�A���O��\������)
//////////////////////////////////////////////////
void CInputPropertyDialog::OnWDMVideoSettingMenu(ENUM_WDM_FILTER e)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	pDSA->ShowPropertySetting(TRUE,e,this->GetSafeHwnd());
}

//////////////////////////////////////////////////
//�I�[�f�B�I�\���{�^�� (�_�C�A���O��\������)
//////////////////////////////////////////////////
void CInputPropertyDialog::OnBnClickedAudioSettingButton()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	pDSA->ShowPropertySetting(FALSE,WDM_FILTER_NONE,this->GetSafeHwnd());
}

//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::ChangeProfileNotify()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	//�t�B���^�̈ꎞ��~
	pDSA->StopFilter();

	m_bFirstFlag = TRUE;	//�����ݒ�J�n
	LoadProfileSetting();
	m_bFirstFlag = FALSE;	//�����ݒ�I��

	//�t�B���^�̍ĊJ(����́APreviewWindow���s�������ł���)
	//pDSA->RunFilter();
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C������̃��[�h
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::LoadProfileSetting()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	CComboBox* combo;

	//�r�f�I�L���v�`���f�o�C�X
	combo = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO); assert(combo);
	if (combo->IsWindowEnabled())
	{
		if (!pProfile->m_strVideoName.IsEmpty())
		{
			for (int i=0;i<(int)pDSA->m_strVideoNameList.size();i++)
			{
				if (pProfile->m_strVideoName.Compare(pDSA->m_strVideoNameList[i].c_str()) == 0)
				{
					combo->SetCurSel(i);
					break;
				}
			}
		}
		if (combo->GetCurSel() == CB_ERR)
			combo->SetCurSel(0);

		//�r�f�I�R���{�{�b�N�X��I�����W���[�����Ăяo��
		OnCbnSelchangeVideoSettingCombo();

		//�O��̃r�f�I���E�����E�t���[�����[�g�̒l�𕜌�
		if (!pProfile->m_strVideoName.IsEmpty() && pProfile->m_nVideoSettingIndex != -1 && 
			pProfile->m_nVideoSettingWidth != 0 && pProfile->m_nVideoSettingHeight != 0 && pProfile->m_dVideoSettingFPS != 0.0)
		{
			SetVideoCurrentInput(pProfile->m_nVideoSettingWidth,
				pProfile->m_nVideoSettingHeight,pProfile->m_dVideoSettingFPS);
			//�r�f�I�K�p�R���{�{�b�N�X��I�����W���[�����Ăяo��
			OnBnClickedVideoResolutionButton();
		}
	}

	//�I�[�f�B�I�L���v�`���f�o�C�X
	//TODO ���L���[�h�̍ۂɂ����Ƀo�O�����邪�A�Ƃ肠������񂵂ő��̎�����D��
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_SETTING_COMBO); assert(combo);
	if (combo->IsWindowEnabled())
	{
		if (!pProfile->m_strAudioName.IsEmpty())
		{
			for (int i=0;i<(int)pDSA->m_strAudioNameList.size();i++)
			{
				if (pProfile->m_strAudioName.Compare(pDSA->m_strAudioNameList[i].c_str()) == 0)
				{
					combo->SetCurSel(i);
					break;
				}
			}
		}
		//���L���[�h
		if (pProfile->m_bAudioInputVideoPin) {
			combo->SetCurSel(combo->GetCount() -1);	//�Ō����I��
		}
		if (combo->GetCurSel() == CB_ERR)
			combo->SetCurSel(0);

		//�I�[�f�B�I�R���{�{�b�N�X��I�����W���[�����Ăяo��
		OnCbnSelchangeAudioSettingCombo();

		//�O��̒l�𕜌�(���g���E����\�E�`�����l��)
		if (!pProfile->m_strAudioName.IsEmpty() && pProfile->m_nAudioSettingIndex != -1 && 
			pProfile->m_nSamplesPerSec > 0 && pProfile->m_wBitsPerSample > 0 && pProfile->m_nChannels > 0)
		{
			SetAudioCurrentInput(pProfile->m_nSamplesPerSec,
				pProfile->m_wBitsPerSample,pProfile->m_nChannels);
			//�I�[�f�B�I�K�p�R���{�{�b�N�X��I�����W���[�����Ăяo��
			OnBnClickedAudioResolutionButton();
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���ւ̃Z�[�u
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::SaveProfileSetting()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	CComboBox *combo1,*combo2;

	//�r�f�I�L���v�`���f�o�C�X
	combo1 = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO);
	combo2 = (CComboBox*)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO);
	if (combo1->IsWindowEnabled() && combo2->IsWindowEnabled() && combo1->GetCurSel() != CB_ERR && combo2->GetCurSel() != CB_ERR) {
		combo1->GetLBText(combo1->GetCurSel(),pProfile->m_strVideoName);
		combo2->GetLBText(combo2->GetCurSel(),pProfile->m_strVideoSettingName);
		pProfile->m_nVideoSettingIndex = combo2->GetCurSel();
	} else {
		pProfile->m_strVideoName = _T("SCFH DSF");
		pProfile->m_strVideoSettingName = _T("");
		pProfile->m_nVideoSettingIndex = -1;
	}
	//�r�f�I�𑜓x(���E�����E�t���[�����[�g)
	int nWidth , nHeight;
	double dFPS;
	GetVideoCurrentInput(nWidth,nHeight,dFPS);
	if (nWidth != 0 && nHeight != 0 && dFPS != 0.0) {
		pProfile->m_nVideoSettingWidth = nWidth;
		pProfile->m_nVideoSettingHeight = nHeight;
		pProfile->m_dVideoSettingFPS = dFPS;
	} else {
		pProfile->m_nVideoSettingWidth = -1;
		pProfile->m_nVideoSettingHeight = -1;
		pProfile->m_dVideoSettingFPS = 0.0;
	}

	//�I�[�f�B�I�L���v�`���f�o�C�X
	combo1 = (CComboBox*)GetDlgItem(IDC_AUDIO_SETTING_COMBO);
	combo2 = (CComboBox*)GetDlgItem(IDC_AUDIO_RESOLUTION_COMBO);
	if (combo1->IsWindowEnabled() && combo2->IsWindowEnabled()) {
		combo1->GetLBText(combo1->GetCurSel(),pProfile->m_strAudioName);
		combo2->GetLBText(combo2->GetCurSel(),pProfile->m_strAudioSettingName);
		pProfile->m_nAudioSettingIndex = combo2->GetCurSel();
		if (combo1->GetItemData((combo1->GetCurSel())) == SHARE_VIDEO_AUDIO)
			pProfile->m_bAudioInputVideoPin = TRUE;
		else 
			pProfile->m_bAudioInputVideoPin = FALSE;
	} else {
		pProfile->m_strAudioName = _T("");
		pProfile->m_strAudioSettingName = _T("");
		pProfile->m_nAudioSettingIndex = -1;
		pProfile->m_bAudioInputVideoPin = FALSE;
	}
	//�I�[�f�B�I�ݒ�(���g���E����\�E�`�����l��)
	DWORD nSamplesPerSec;
	WORD wBitsPerSample,nChannels;
	GetAudioCurrentInput(nSamplesPerSec,wBitsPerSample,nChannels);
	if (nSamplesPerSec != 0 && wBitsPerSample != 0 && nChannels != 0)
	{
		pProfile->m_nSamplesPerSec = nSamplesPerSec;
		pProfile->m_wBitsPerSample = wBitsPerSample;
		pProfile->m_nChannels = nChannels;
	} else {
		pProfile->m_nSamplesPerSec = 44100;
		pProfile->m_wBitsPerSample = 16;
		pProfile->m_nChannels = 2;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::Encode(BOOL bStart)
{
	//�r�f�I�𑜓x�����On/Off
	((CWnd*)GetDlgItem(IDC_VIDEO_SETTING_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	if (bStart)
	{
		GetDlgItem(IDC_VIDEO_WIDTH_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_WIDTH_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_HEIGHT_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_HEIGHT_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN)->EnableWindow(FALSE);
		GetDlgItem(IDC_VIDEO_RESOLUTION_BUTTON)->EnableWindow(FALSE);
	} else
	{
		CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_WIDTH_SPIN); assert(spin);
		int nMin,nMax;
		spin->GetRange32(nMin,nMax);
		if (nMin != nMax) {
			GetDlgItem(IDC_VIDEO_WIDTH_EDIT)->EnableWindow(TRUE);
			spin->EnableWindow(TRUE);
		}
		spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_HEIGHT_SPIN); assert(spin);
		spin->GetRange32(nMin,nMax);
		if (nMin != nMax) {
			GetDlgItem(IDC_VIDEO_HEIGHT_EDIT)->EnableWindow(TRUE);
			spin->EnableWindow(TRUE);
		}
		spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN); assert(spin);
		spin->GetRange32(nMin,nMax);
		if (nMin != nMax) {
			GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT)->EnableWindow(TRUE);
			spin->EnableWindow(TRUE);
		}
		if (GetDlgItem(IDC_VIDEO_WIDTH_EDIT)->IsWindowEnabled() ||
			GetDlgItem(IDC_VIDEO_HEIGHT_EDIT)->IsWindowEnabled() ||
			GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT)->IsWindowEnabled())
			GetDlgItem(IDC_VIDEO_RESOLUTION_BUTTON)->EnableWindow(TRUE);
	}

	//�I�[�f�B�I�����On/Off
	((CWnd*)GetDlgItem(IDC_AUDIO_SETTING_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_AUDIO_RESOLUTION_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	if (bStart)
	{
		GetDlgItem(IDC_AUDIO_FREQ_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUDIO_BITS_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUDIO_CHANNELS_COMBO)->EnableWindow(FALSE);
		GetDlgItem(IDC_AUDIO_RESOLUTION_BUTTON)->EnableWindow(FALSE);
	} else {
		//�ݒ肪�ЂƂ����Ȃ��ꍇ��OFF�̂܂܂ł���
		CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_FREQ_COMBO); assert(combo);
		if (combo->GetCount() > 1)
			combo->EnableWindow(TRUE);
		combo = (CComboBox*)GetDlgItem(IDC_AUDIO_BITS_COMBO); assert(combo);
		if (combo->GetCount() > 1)
			combo->EnableWindow(TRUE);
		combo = (CComboBox*)GetDlgItem(IDC_AUDIO_CHANNELS_COMBO); assert(combo);
		if (combo->GetCount() > 1)
			combo->EnableWindow(TRUE);

		if (GetDlgItem(IDC_AUDIO_FREQ_COMBO)->IsWindowEnabled() ||
			GetDlgItem(IDC_AUDIO_BITS_COMBO)->IsWindowEnabled() ||
			GetDlgItem(IDC_AUDIO_CHANNELS_COMBO)->IsWindowEnabled())
			GetDlgItem(IDC_AUDIO_RESOLUTION_BUTTON)->EnableWindow(TRUE);
	}


	((CWnd*)GetDlgItem(IDC_INPUT_CONFIG_BUTTON))->EnableWindow(bStart ? FALSE : TRUE);

	return TRUE;
}


//////////////////////////////////////////////////
//�r�f�I�f�o�C�X�̑���Ɏ��s�����ꍇ�̃T�u���[�`��
//int nLevel	0:�r�f�I�f�o�C�X�̌��o�Ɏ��s(���������Ȃ�����)
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::VideoDeviceFailed(int nLevel)
{
	CComboBox* combo;
	CWnd* wnd;
	if (nLevel <= 0) {
		combo = (CComboBox*)GetDlgItem(IDC_VIDEO_SETTING_COMBO); assert(combo);
		combo->ResetContent();
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
	}
	if (nLevel <= 1) {
		wnd = GetDlgItem(IDC_VIDEO_SETTING_BUTTON); assert(wnd);
		wnd->EnableWindow(FALSE);
	}
	if (nLevel <= 2) {
		combo = (CComboBox*)GetDlgItem(IDC_VIDEO_RESOLUTION_COMBO); assert(combo);
		combo->ResetContent();
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
	}
	if (nLevel <= 3) {
		wnd = GetDlgItem(IDC_VIDEO_WIDTH_EDIT); assert(wnd);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_WIDTH_SPIN); assert(wnd);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_HEIGHT_EDIT); assert(wnd);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_HEIGHT_SPIN); assert(wnd);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT); assert(wnd);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN); assert(wnd);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_RESOLUTION_BUTTON); assert(wnd);
		wnd->EnableWindow(FALSE);
	}
	return TRUE;
}

//////////////////////////////////////////////////
//���݂̃r�f�I���͒l(���E�����E�t���[�����[�g)�𓾂�T�u���[�`��
//////////////////////////////////////////////////
void CInputPropertyDialog::GetVideoCurrentInput(int &nWidth,int &nHeight,double &dFPS)
{
	//�r�f�I�𑜓x(���E�����E�t���[�����[�g)
	nWidth = 0;
	nHeight = 0;
	dFPS = 0.0;
	CString strNum;
	CEdit* edit = (CEdit*)GetDlgItem(IDC_VIDEO_WIDTH_EDIT);
	if (!edit) return;
	edit->GetWindowText(strNum);
	nWidth = _tstoi(strNum);
	edit = (CEdit*)GetDlgItem(IDC_VIDEO_HEIGHT_EDIT);
	if (!edit) return;
	edit->GetWindowText(strNum);
	nHeight = _tstoi(strNum);
	edit = (CEdit*)GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT);
	if (!edit) return;
	edit->GetWindowText(strNum);
	dFPS = _tstof(strNum);
	return;
}

//////////////////////////////////////////////////
//�r�f�I���͒l��ݒ� (�l��\�����邾���œK�p�͂��Ȃ�)
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::SetVideoCurrentInput(int nWidth, int nHeight, double dFPS)
{
	CString strNum;
	CEdit* edit = (CEdit*)GetDlgItem(IDC_VIDEO_WIDTH_EDIT);
	if (!edit) return FALSE;
	strNum.Format(_T("%d"),nWidth);
	edit->SetWindowText(strNum);
	edit = (CEdit*)GetDlgItem(IDC_VIDEO_HEIGHT_EDIT);
	if (!edit) return FALSE;
	strNum.Format(_T("%d"),nHeight);
	edit->SetWindowText(strNum);
	edit = (CEdit*)GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT);
	if (!edit) return FALSE;
	strNum.Format(_T("%fd"),dFPS);
	edit->SetWindowText(strNum);
	return TRUE;
}

//////////////////////////////////////////////////
//�I�[�f�B�I���͒l���擾
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::GetAudioCurrentInput(DWORD &nSamplesPerSec,WORD &wBitsPerSample,WORD &nChannels)
{
	nSamplesPerSec = 0;
	wBitsPerSample = 0;
	nChannels = 0;
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_FREQ_COMBO); assert(combo);
	if (combo->GetCurSel() != CB_ERR)
		nSamplesPerSec = (DWORD)combo->GetItemData(combo->GetCurSel());
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_BITS_COMBO); assert(combo);
	if (combo->GetCurSel() != CB_ERR)
		wBitsPerSample = (WORD)combo->GetItemData(combo->GetCurSel());
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_CHANNELS_COMBO); assert(combo);
	if (combo->GetCurSel() != CB_ERR)
		nChannels = (WORD)combo->GetItemData(combo->GetCurSel());
	return TRUE;
}

//////////////////////////////////////////////////
//�I�[�f�B�I���͒l��ݒ� (�l��\�����邾���œK�p�͂��Ȃ�)
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::SetAudioCurrentInput(DWORD nSamplesPerSec,WORD wBitsPerSample,WORD nChannels)
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_FREQ_COMBO);
	if (!combo) return FALSE;
	for (int i=0;i<combo->GetCount();i++) {
		if ((DWORD)combo->GetItemData(i) == nSamplesPerSec) {
			combo->SetCurSel(i);
			break;
		}
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_BITS_COMBO);
	if (!combo) return FALSE;
	for (int i=0;i<combo->GetCount();i++) {
		if ((WORD)combo->GetItemData(i) == wBitsPerSample) {
			combo->SetCurSel(i);
			break;
		}
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);
	combo = (CComboBox*)GetDlgItem(IDC_AUDIO_CHANNELS_COMBO);
	if (!combo) return FALSE;
	for (int i=0;i<combo->GetCount();i++) {
		if ((WORD)combo->GetItemData(i) == nChannels) {
			combo->SetCurSel(i);
			break;
		}
	}
	if (combo->GetCurSel() == CB_ERR)
		combo->SetCurSel(0);
	return TRUE;
}

//////////////////////////////////////////////////
//���͂̏ڍאݒ�{�^�� (���͂̏ڍאݒ�_�C�A���O��\��)
//////////////////////////////////////////////////
void CInputPropertyDialog::OnBnClickedInputConfigButton()
{
	CInputPropertyConfigDialog dlg;
	dlg.DoModal();
}

//////////////////////////////////////////////////
//�r�f�I�t�B���^�[�̃I�[�f�B�I�s���̓��̗͂L���Ɋւ���T�u���[�`��
//////////////////////////////////////////////////
void CInputPropertyDialog::ProcVideoAudioInputPin()
{
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_SETTING_COMBO); assert(combo);

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	//�r�f�I�s���̗L��
	BOOL bFindAudioPin = pDSA->IsVidoFindAudioPin();

	//���݂̃J�[�\���ʒu�C���f�b�N�X���擾
	int nNowComboIndex = 0;
	if (combo->GetCurSel() != CB_ERR)
		nNowComboIndex = (int)combo->GetItemData(combo->GetCurSel());

	if (nNowComboIndex == SHARE_VIDEO_AUDIO) {	//�I�[�f�B�I�ݒ���͂����L���[�h�ɂȂ��Ă���
		if (bFindAudioPin) {
			//���݂̃I�[�f�B�I�L���v�`���ŁA���L���[�h�ɂăt�B���^�č\�z���s��
			OnCbnSelchangeAudioSettingCombo();
		} else {
			//�񋤗L���[�h�ɂȂ�(�I���I�[�f�B�I�f�o�C�X�͂Ƃ肠����0�Ԗ�)
			combo->DeleteString(combo->GetCount() -1);
			if (combo->GetCurSel() == CB_ERR)
				combo->SetCurSel(0);
			//0�Ԗڂ̃I�[�f�B�I�f�o�C�X��I������
			OnCbnSelchangeAudioSettingCombo();
		}
	} else {		//�I�[�f�B�I�ݒ���͂����L���[�h�ɂȂ��Ă���
		BOOL bInsertComboIndex = FALSE;
		for (int i=0;i<combo->GetCount();i++) {
			if ( (int)combo->GetItemData(i) == SHARE_VIDEO_AUDIO )
				bInsertComboIndex = TRUE;
		}
		//�R���{�{�b�N�X�̒���ҏW����݂̂ɗ��߂�
		if (bFindAudioPin) {
			if (!bInsertComboIndex) {
				int nInsertIndex = combo->AddString(_T("�r�f�I�L���v�`���f�o�C�X�̃I�[�f�B�I"));
				combo->SetItemData(nInsertIndex,(DWORD_PTR)SHARE_VIDEO_AUDIO);
			}
		} else {
			if (bInsertComboIndex) {
				combo->DeleteString(combo->GetCount() -1);
			}
		}
	}
	return;
}


//////////////////////////////////////////////////
//�f�o�C�X�ʒm�̃R�[���o�b�N���b�Z�[�W
//TODO ��������ŏ���
//////////////////////////////////////////////////
BOOL CInputPropertyDialog::DeviceChange()
{
	/*
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	BOOL bRet = TRUE;		//�߂�l������ł��������ǂ���
	BOOL bChange = FALSE;	//�ύX�����������ǂ����̗L��
	vector<wstring> strNameList;
	bRet = pDSA->ReScanDevice(TRUE,bChange,strNameList);
	if (bChange) {
		TRACE0("�r�f�I�f�o�C�X�����o����܂���\n");
		for (int i=0;i<(int)strNameList.size();i++) {
			TRACE2("Video[%d]:%s\n",i,strNameList[i].c_str());
		}
	}
	bRet = pDSA->ReScanDevice(FALSE,bChange,strNameList);
	if (bChange) {
		TRACE0("�I�[�f�B�I�f�o�C�X�����o����܂���\n");
		for (int i=0;i<(int)strNameList.size();i++) {
			TRACE2("Audio[%d]:%s\n",i,strNameList[i].c_str());
		}
	}
	*/
	/*
	�f�o�C�X�̍Č��o
	���o�O�̃f�o�C�X�ꗗ�Ɣ�r���č������邩�ǂ������ׂ�
	�����������ꍇ(�@�ƇA�ɕ������)
	�@�f�o�C�X���ǉ����ꂽ�ꍇ
		���̂܂ܒǋL�ŃR���{�{�b�N�X���X�g������������
		�R���{�{�b�N�X�̃C���f�b�N�X����������邽�߂ɒ��ӂ��K�v
	�A�f�o�C�X���폜���ꂽ�ꍇ
		�ꍇ�ɂ���Ă͍��g���Ă�f�o�C�X���폜�����ꍇ������(���̏ꍇ�̓G���[���o����)
	���ƁA�G���R�[�h���̏ꍇ�ɂ́A����Ƀf�o�C�X�̌��o�͌�񂵂ɂ���d�g�݂ɂ��悤
	*/
	return TRUE;
}


