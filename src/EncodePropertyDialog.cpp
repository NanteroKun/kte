// EncodePropertyDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "EncodePropertyDialog.h"


// CEncodePropertyDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CEncodePropertyDialog, CSizeDialog)


BEGIN_MESSAGE_MAP(CEncodePropertyDialog, CSizeDialog)
	ON_CBN_SELCHANGE(IDC_VIDEO_ENCODER_COMBO, &CEncodePropertyDialog::OnCbnSelchangeVideoEncoderCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_ENCODER_COMBO, &CEncodePropertyDialog::OnCbnSelchangeAudioEncoderCombo)
	ON_WM_DESTROY()
	ON_CBN_SELCHANGE(IDC_AUDIO_ENCODER_BITRATE_COMBO, &CEncodePropertyDialog::OnCbnSelchangeAudioEncoderBitrateCombo)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_SIZE_AUTO_RADIO, &CEncodePropertyDialog::OnBnClickedSizeAutoRadio)
	ON_BN_CLICKED(IDC_SIZE_MANUAL_RADIO, &CEncodePropertyDialog::OnBnClickedSizeAutoRadio)
	ON_EN_CHANGE(IDC_VIDEO_QUALITY_EDIT, &CEncodePropertyDialog::OnEnChangeVideoQualityEdit)
	ON_EN_CHANGE(IDC_VIDEO_FRAMERATE_EDIT, &CEncodePropertyDialog::OnEnChangeVideoFramerateEdit)
	//ON_WM_SIZE()
	//ON_WM_VSCROLL()
	ON_CBN_SELCHANGE(IDC_AUDIO_ENCODER_SAMPLESPERSEC_COMBO, &CEncodePropertyDialog::OnCbnSelchangeAudioEncoderSamplespersecCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_ENCODER_BITSPERSAMPLE_COMBO, &CEncodePropertyDialog::OnCbnSelchangeAudioEncoderBitspersampleCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_ENCODER_CHANNELS_COMBO, &CEncodePropertyDialog::OnCbnSelchangeAudioEncoderChannelsCombo)
	ON_CBN_SELCHANGE(IDC_AUDIO_ENCODER_AVSYNCHRONIZATION, &CEncodePropertyDialog::OnCbnSelchangeAudioEncoderAvsynchronization)
END_MESSAGE_MAP()


CEncodePropertyDialog::CEncodePropertyDialog(CWnd* pParent /*=NULL*/)
	: CSizeDialog(CEncodePropertyDialog::IDD, pParent)
{
	m_bFirstFlag = FALSE; 
	//m_strAudioBitrate = _T("");
	m_dwBitrate = 0;
	m_nSamplesPerSec = 0;
	m_wBitsPerSample = 0;
	m_nChannels = 0;
	m_bAVSynchronization = FALSE;
}

CEncodePropertyDialog::~CEncodePropertyDialog()
{
}

void CEncodePropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}




// CEncodePropertyDialog ���b�Z�[�W �n���h��

////////////////////////////////////////
void CEncodePropertyDialog::OnOK()
{
	//CDialog::OnOK();
}

////////////////////////////////////////
void CEncodePropertyDialog::OnCancel()
{
	//CDialog::OnCancel();
}

////////////////////////////////////////
//�_�C�A���O������
////////////////////////////////////////
BOOL CEncodePropertyDialog::OnInitDialog()
{
	CSizeDialog::OnInitDialog();
	m_bFirstFlag = TRUE;	//�������t���O�L��

	//GetWindowRect(m_rect);
	//m_nScrollPos = 0;


	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_ENCODER_COMBO),IDC_VIDEO_ENCODER_COMBO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_BITRATE_EDIT),IDC_VIDEO_BITRATE_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT),IDC_VIDEO_FRAMERATE_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_SIZE_AUTO_RADIO),IDC_SIZE_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_SIZE_MANUAL_RADIO),IDC_SIZE_MANUAL_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_WIDTH_EDIT),IDC_VIDEO_WIDTH_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_HEIGHT_EDIT),IDC_VIDEO_HEIGHT_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_QUALITY_EDIT),IDC_VIDEO_QUALITY_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_BUFFER_WINDOW_EDIT),IDC_BUFFER_WINDOW_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_ENCODER_COMBO),IDC_AUDIO_ENCODER_COMBO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_ENCODER_BITRATE_COMBO),IDC_AUDIO_ENCODER_BITRATE_COMBO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_ENCODER_SAMPLESPERSEC_COMBO),IDC_AUDIO_ENCODER_SAMPLESPERSEC_COMBO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_ENCODER_BITSPERSAMPLE_COMBO),IDC_AUDIO_ENCODER_BITSPERSAMPLE_COMBO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_ENCODER_CHANNELS_COMBO),IDC_AUDIO_ENCODER_CHANNELS_COMBO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_ENCODER_AVSYNCHRONIZATION),IDC_AUDIO_ENCODER_AVSYNCHRONIZATION);

	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	CDirectShowAccess *pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	CString strNum;

	//�r�f�I�G���R�[�h�R���{�{�b�N�X�̐ݒ�
	CComboBox* combo = (CComboBox*)this->GetDlgItem(IDC_VIDEO_ENCODER_COMBO); assert(combo);
	if (pDSA->m_VideoCodecList.size() != 0)
	{
		for (int i=0;i<(int)pDSA->m_VideoCodecList.size();i++) {			//�G���R�[�h�A�C�e�����̒ǉ�
			combo->AddString(pDSA->m_VideoCodecList[i].strName.c_str());
		}
		if (!pProfile->m_strVideoEncodeName.IsEmpty())	//INI�̐ݒ�L��
		{
			for (int i=0;i<(int)pDSA->m_VideoCodecList.size();i++)
			{
				if (pProfile->m_strVideoEncodeName.Compare(pDSA->m_VideoCodecList[i].strName.c_str()) == 0)
				{
					combo->SetCurSel(i);
					break;
				}
			}
		}
		if (combo->GetCurSel() == CB_ERR){
			combo->SetCurSel(0);	//0�Ԗڂ̑I�������Ƃ�
		}
		//�r�f�I�G���R�[�_�[�̐ݒ��ύX����
		OnCbnSelchangeVideoEncoderCombo();
	} else {	//�G���R�[�_�[�̐ݒ肪�Ȃ�(�ƂĂ��H)
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
		AfxMessageBox(_T("�C���X�g�[������Ă���r�f�I�G���R�[�_�[��������܂���ł���"),MB_OK|MB_ICONINFORMATION);
	}

	//�r�f�I�r�b�g���[�g�̐ݒ�(�P�ʂ�Kbps)
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_BITRATE_SPIN); assert(spin);
	spin->SetRange32(4,135000);	//���͐ݒ�̍ő��135Mbps
	spin->SetPos32(pProfile->m_nVideoEnocdeBitrate);
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_BITRATE_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEnocdeBitrate);
	edit->SetWindowText(strNum);

	//�t���[�����[�g�̐ݒ�
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN); assert(spin);
	spin->SetRange(1,(int)pProfile->m_dVideoEncodeMaxFramerateSetting);
	spin->SetPos32((int)pProfile->m_dVideoEncodeFramerate);		//�X�s���{�^������ݒ肷��
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT); assert(edit);
	if (((int)(pProfile->m_dVideoEncodeFramerate*100)) % 10 != 0)
		strNum.Format(_T("%.2f"),pProfile->m_dVideoEncodeFramerate);
	else if (((int)(pProfile->m_dVideoEncodeFramerate*10)) % 10 != 0)
		strNum.Format(_T("%.1f"),pProfile->m_dVideoEncodeFramerate);
	else
		strNum.Format(_T("%.0f"),pProfile->m_dVideoEncodeFramerate);
	edit->SetWindowText(strNum);
	/*
	CSliderCtrl* slider = (CSliderCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SLIDER); assert(slider);
	slider->SetTic(10);
	slider->SetTicFreq(200);
	slider->SetPageSize(100);
	slider->SetRange(1,(int)(pIni->m_dVideoEncodeMaxFramerateSetting*100));
	slider->SetPos((int)(pIni->m_dVideoEncodeFramerate*100));
	*/

	//�T�C�Y�̕ύX
	CButton* button1 = (CButton*)this->GetDlgItem(IDC_SIZE_AUTO_RADIO); assert(button1);
	CButton* button2 = (CButton*)this->GetDlgItem(IDC_SIZE_MANUAL_RADIO); assert(button2);
	if (pProfile->m_bVideoEncodeSize)	//��������(�T�C�Y�ύX�L��)
	{
		button1->SetCheck(BST_UNCHECKED);
		button2->SetCheck(BST_CHECKED);
	} else {	//��������(�T�C�Y�ύX����)
		button1->SetCheck(BST_CHECKED);
		button2->SetCheck(BST_UNCHECKED);
	}
	OnBnClickedSizeAutoRadio();

	//�r�f�I�̕��ƍ���
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_WIDTH_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEncodeWidth);
	edit->SetWindowText(strNum);
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_HEIGHT_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEncodeHeight);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_WIDTH_SPIN); assert(spin);
	spin->SetRange32(1,9999);
	spin->SetPos32(pProfile->m_nVideoEncodeWidth);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_HEIGHT_SPIN); assert(spin);
	spin->SetRange32(1,9999);
	spin->SetPos32(pProfile->m_nVideoEncodeHeight);

	//�r�f�I�̊��炩��
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_QUALITY_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEnocdeQuality);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_QUALITY_SPIN); assert(spin);
	spin->SetRange32(0,100);
	OnEnChangeVideoQualityEdit();	//�X�^�e�B�b�N�e�L�X�g��ύX
	/*
	slider = (CSliderCtrl*)this->GetDlgItem(IDC_VIDEO_QUALITY_SLIDER); assert(slider);
	slider->SetTic(10);
	slider->SetTicFreq(10);
	slider->SetPageSize(10);
	slider->SetRange(0,100);
	slider->SetPos(pIni->m_nVideoEnocdeQuality);
	*/

	//�o�b�t�@�T�C�Y
	edit = (CEdit*)this->GetDlgItem(IDC_BUFFER_WINDOW_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nBufferWindow/1000);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_BUFFER_WINDOW_SPIN); assert(spin);
	spin->SetRange32(1,30);
	spin->SetPos32((pProfile->m_nBufferWindow)/1000);

	//�I�[�f�B�I�G���R�[�h�R���{�{�b�N�X�̐ݒ�
	combo = (CComboBox*)this->GetDlgItem(IDC_AUDIO_ENCODER_COMBO); assert(combo);
	if (pDSA->m_AudioCodecList.size() != 0)
	{
		for (int i=0;i<(int)pDSA->m_AudioCodecList.size();i++) {
			combo->AddString(pDSA->m_AudioCodecList[i].strName.c_str());
		}
		if (!pProfile->m_strAudioEncodeName.IsEmpty()) {
			for (int i=0;i<(int)pDSA->m_AudioCodecList.size();i++)
			{
				if (pProfile->m_strAudioEncodeName.Compare(pDSA->m_AudioCodecList[i].strName.c_str()) == 0)
				{
					combo->SetCurSel(i);
					break;
				}
			}
		}
		if (combo->GetCurSel() == CB_ERR){
			combo->SetCurSel(0);	//0�Ԗڂ̑I�������Ƃ�
		}
		//�I�[�f�B�I�G���R�[�_�[�̏ڍׂ̓ǂݍ���
		m_dwBitrate = pProfile->m_nAudioEncodeBitrate;
		m_nSamplesPerSec = pProfile->m_nAudioEncodeSamplesPerSec;
		m_wBitsPerSample = pProfile->m_nAudioEncodeBitsPerSample;
		m_nChannels = pProfile->m_nAudioEncodeChannels;
		m_bAVSynchronization = pProfile->m_bAudioEncodeAVSynchronization;

		//�I�[�f�B�I�G���R�[�_�[�ݒ�R���{�{�b�N�X��ύX
		OnCbnSelchangeAudioEncoderCombo();
	} else {	//�G���R�[�_�[�̐ݒ肪�Ȃ�(�ƂĂ��H)
		combo->AddString(_T("[�Ȃ�]"));
		combo->SetCurSel(0);
		combo->EnableWindow(FALSE);
		AfxMessageBox(_T("�C���X�g�[������Ă���I�[�f�B�I�G���R�[�_�[��������܂���ł���"),MB_OK|MB_ICONINFORMATION);
	}

	m_bFirstFlag = FALSE;	//�������t���O�I��
	return TRUE;
}
////////////////////////////////////////
//�_�C�A���O���I�������Ƃ��ɌĂ΂��C�x���g�n���h��
////////////////////////////////////////
void CEncodePropertyDialog::OnDestroy()
{
	SaveProfileSetting();
	CDialog::OnDestroy();
}
////////////////////////////////////////
//�r�f�I�G���R�[�_�[�̕ύX
////////////////////////////////////////
void CEncodePropertyDialog::OnCbnSelchangeVideoEncoderCombo()
{
	CDirectShowAccess *pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CComboBox* combo = (CComboBox*)this->GetDlgItem(IDC_VIDEO_ENCODER_COMBO); assert(combo);
	int nIndex = combo->GetCurSel();
	if (!pDSA->SelectVideoCodecIndex(nIndex)) {
		AfxMessageBox(_T("�r�f�I�G���R�[�_�[�̐ݒ�ύX�Ɏ��s���܂���"));
		return;
	}
}

////////////////////////////////////////
//�I�[�f�B�I�G���R�[�_�[�̕ύX
////////////////////////////////////////
void CEncodePropertyDialog::OnCbnSelchangeAudioEncoderCombo()
{
	//�r�b�g���[�g�ݒ�̍��ڂ�ҏW����
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CComboBox* combo = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_COMBO); assert(combo);
	int nIndex = combo->GetCurSel();	//�C���f�b�N�X
	if (nIndex == CB_ERR)
		return;
	assert(nIndex >= 0 && nIndex < (int)pDSA->m_AudioCodecList.size());
	if (!pDSA->SelectAudioCodecIndex(nIndex))
	{
		AfxMessageBox(_T("�I�[�f�B�I�R�[�f�b�N�̕ύX�Ɏ��s���܂����B"));
		return;
	}

	//�t�H�[�}�b�g�̈ꗗ���擾
	vector<DWORD> nSamplesPerSecList;
	vector<WORD> wBitsPerSampleList;
	vector<WORD> nChannelsList;
	vector<BOOL> bAVSynchronizationList;
	for (int i=0;i<(int)pDSA->m_AudioFormatList.size();i++)
	{
		//���g��
		BOOL bFind = FALSE;
		for (int j=0;j<(int)nSamplesPerSecList.size();j++){
			if (nSamplesPerSecList[j] == pDSA->m_AudioFormatList[i].nSamplesPerSec)
				bFind = TRUE;
		}
		if (!bFind)
			nSamplesPerSecList.push_back(pDSA->m_AudioFormatList[i].nSamplesPerSec);
		//����\
		bFind = FALSE;
		for (int j=0;j<(int)wBitsPerSampleList.size();j++){
			if (wBitsPerSampleList[j] == pDSA->m_AudioFormatList[i].wBitsPerSample)
				bFind = TRUE;
		}
		if (!bFind)
			wBitsPerSampleList.push_back(pDSA->m_AudioFormatList[i].wBitsPerSample);
		//�`�����l��
		bFind = FALSE;
		for (int j=0;j<(int)nChannelsList.size();j++){
			if (nChannelsList[j] == pDSA->m_AudioFormatList[i].nChannels)
				bFind = TRUE;
		}
		if (!bFind)
			nChannelsList.push_back(pDSA->m_AudioFormatList[i].nChannels);
		//A/V����
		bFind = FALSE;
		for (int j=0;j<(int)bAVSynchronizationList.size();j++){
			if (bAVSynchronizationList[j] == pDSA->m_AudioFormatList[i].bAVSynchronization)
				bFind = TRUE;
		}
		if (!bFind)
			bAVSynchronizationList.push_back(pDSA->m_AudioFormatList[i].bAVSynchronization);
	}

	//�t�H�[�}�b�g�������������ǋL
	CComboBox* combo_samples = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_SAMPLESPERSEC_COMBO); assert(combo_samples);
	CComboBox* combo_bits = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_BITSPERSAMPLE_COMBO); assert(combo_bits);
	CComboBox* combo_channels = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_CHANNELS_COMBO); assert(combo_channels);
	CComboBox* combo_av = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_AVSYNCHRONIZATION); assert(combo_av);
	combo_samples->ResetContent();
	combo_bits->ResetContent();
	combo_channels->ResetContent();
	combo_av->ResetContent();
	if (!combo_samples->IsWindowEnabled()) combo_samples->EnableWindow(TRUE);
	if (!combo_bits->IsWindowEnabled()) combo_bits->EnableWindow(TRUE);
	if (!combo_channels->IsWindowEnabled()) combo_channels->EnableWindow(TRUE);
	if (!combo_av->IsWindowEnabled()) combo_av->EnableWindow(TRUE);

	CString strNum;
	int nInsertIndex;
	if (nSamplesPerSecList.size() != 0)
	{
		for (int i=0;i<(int)nSamplesPerSecList.size();i++)
		{
			//strNum.Format(_T("%.3f"),((float)nSamplesPerSecList[i]/1000.0));
			strNum.Format(_T("%d"),nSamplesPerSecList[i]);
			nInsertIndex = combo_samples->AddString(strNum);
			combo_samples->SetItemData(nInsertIndex,(DWORD_PTR)nSamplesPerSecList[i]);
			if (m_nSamplesPerSec == nSamplesPerSecList[i])
				combo_samples->SetCurSel(nInsertIndex);
		}
		if (combo_samples->GetCurSel() == CB_ERR)
			combo_samples->SetCurSel(0);
	} else {
		combo_samples->AddString(_T("�Ȃ�"));
		combo_samples->SetCurSel(0);
		combo_samples->EnableWindow(FALSE);
	}
	if (wBitsPerSampleList.size() != 0)
	{
		for (int i=0;i<(int)wBitsPerSampleList.size();i++)
		{
			strNum.Format(_T("%d"),wBitsPerSampleList[i]);
			nInsertIndex = combo_bits->AddString(strNum);
			combo_bits->SetItemData(nInsertIndex,(DWORD_PTR)wBitsPerSampleList[i]);
			if (m_wBitsPerSample == wBitsPerSampleList[i])
				combo_bits->SetCurSel(nInsertIndex);
		}
		if (combo_bits->GetCurSel() == CB_ERR)
			combo_bits->SetCurSel(0);
	} else {
		combo_bits->AddString(_T("�Ȃ�"));
		combo_bits->SetCurSel(0);
		combo_bits->EnableWindow(FALSE);
	}
	if (nChannelsList.size() != 0)
	{
		for (int i=0;i<(int)nChannelsList.size();i++)
		{
			if (nChannelsList[i] == 6)
				strNum = _T("5.1");
			else if (nChannelsList[i] == 8)
				strNum = _T("7.1");
			else
				strNum.Format(_T("%d"),nChannelsList[i]);
			nInsertIndex = combo_channels->AddString(strNum);
			combo_channels->SetItemData(nInsertIndex,(DWORD_PTR)nChannelsList[i]);
			if (m_nChannels == nChannelsList[i])
				combo_channels->SetCurSel(nInsertIndex);
		}
		if (combo_channels->GetCurSel() == CB_ERR)
			combo_channels->SetCurSel(0);
	} else {
		combo_channels->AddString(_T("�Ȃ�"));
		combo_channels->SetCurSel(0);
		combo_channels->EnableWindow(FALSE);
	}

	if (bAVSynchronizationList.size() != 0)
	{
		for (int i=0;i<(int)bAVSynchronizationList.size();i++)
		{
			nInsertIndex = combo_av->AddString(bAVSynchronizationList[i]?_T("�L��"):_T("����"));
			combo_av->SetItemData(nInsertIndex,(DWORD_PTR)bAVSynchronizationList[i]);
			if (m_bAVSynchronization == bAVSynchronizationList[i])
				combo_av->SetCurSel(nInsertIndex);
		}
		if (combo_av->GetCurSel() == CB_ERR)
			combo_av->SetCurSel(0);
	} else {
		combo_av->AddString(_T("�Ȃ�"));
		combo_av->SetCurSel(0);
		combo_av->EnableWindow(FALSE);
	}

	//�r�b�g���[�g�̍��ڂ�ҏW
	AudioEncoderSettingChange();
}

//////////////////////////////////////////////////
//�I�[�f�B�I�G���R�[�_�[�̐ݒ��ύX�����ۂɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CEncodePropertyDialog::AudioEncoderSettingChange()
{
	//���݂̃R���{�{�b�N�X�̒l���擾
	DWORD nSamplesPerSec = 0;
	WORD wBitsPerSample = 0;
	WORD nChannels = 0;
	BOOL bAVSynchronization = TRUE;
	CComboBox* combo_samples = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_SAMPLESPERSEC_COMBO); assert(combo_samples);
	CComboBox* combo_bits = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_BITSPERSAMPLE_COMBO); assert(combo_bits);
	CComboBox* combo_channels = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_CHANNELS_COMBO); assert(combo_channels);
	CComboBox* combo_av = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_AVSYNCHRONIZATION); assert(combo_av);
	if (combo_samples->IsWindowEnabled() && combo_samples->GetCurSel() != CB_ERR)
		nSamplesPerSec = (DWORD)combo_samples->GetItemData(combo_samples->GetCurSel());
	if (combo_bits->IsWindowEnabled() && combo_bits->GetCurSel() != CB_ERR)
		wBitsPerSample = (WORD)combo_bits->GetItemData(combo_bits->GetCurSel());
	if (combo_channels->IsWindowEnabled() && combo_channels->GetCurSel() != CB_ERR)
		nChannels = (WORD)combo_channels->GetItemData(combo_channels->GetCurSel());
	if (combo_av->IsWindowEnabled() && combo_av->GetCurSel() != CB_ERR)
		bAVSynchronization = (BOOL)combo_av->GetItemData(combo_av->GetCurSel());

	//�ڍאݒ�ƓK������r�b�g���[�g�ꗗ�̗�
	CComboBox* combo_bitrate = (CComboBox*)GetDlgItem(IDC_AUDIO_ENCODER_BITRATE_COMBO); assert(combo_bitrate);
	combo_bitrate->ResetContent();
	if (!combo_bitrate->IsWindowEnabled())
		combo_bitrate->EnableWindow(TRUE);
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	int nInsertIndex;
	CString strNum;
	int nDiff = INT_MAX;
	for (int i=0;i<(int)pDSA->m_AudioFormatList.size();i++)
	{
		if (pDSA->m_AudioFormatList[i].nSamplesPerSec != nSamplesPerSec)
			continue;
		if (pDSA->m_AudioFormatList[i].wBitsPerSample != wBitsPerSample)
			continue;
		if (pDSA->m_AudioFormatList[i].nChannels != nChannels)
			continue;
		if (pDSA->m_AudioFormatList[i].bAVSynchronization != bAVSynchronization)
			continue;
		//strNum.Format(_T("%0.2f"),((float)pDSA->m_AudioFormatList[i].dwBitrate/1000));
		int nBitrate = (int) (((float)pDSA->m_AudioFormatList[i].dwBitrate/((float)1000.0))+(0.5) );
		strNum.Format(_T("%d"),nBitrate);
		nInsertIndex = combo_bitrate->AddString(strNum);
		combo_bitrate->SetItemData(nInsertIndex,(DWORD_PTR)i);	//�ݒ�C���f�b�N�X���d����

		if (nDiff > abs((int)(m_dwBitrate - pDSA->m_AudioFormatList[i].dwBitrate)) )
		{
			combo_bitrate->SetCurSel(nInsertIndex);
			nDiff = abs((int)(m_dwBitrate - pDSA->m_AudioFormatList[i].dwBitrate));

		}
	}

	if (combo_bitrate->GetCount() != 0)
	{
		if (combo_bitrate->GetCurSel() == CB_ERR)
			combo_bitrate->SetCurSel(0);

	} else {	//�Y������r�b�g���[�g�����������ꍇ
		combo_bitrate->AddString(_T("[�Ȃ�]"));
		combo_bitrate->SetCurSel(0);
		combo_bitrate->SetItemData(0,(DWORD_PTR)-1);
		combo_bitrate->EnableWindow(FALSE);
	}
	//�r�b�g���[�g�ύX�̓K�p
	OnCbnSelchangeAudioEncoderBitrateCombo();
	return;
}

////////////////////////////////////////
//�I�[�f�B�I�̃r�b�g���[�g�̕ύX�R���{�{�b�N�X��ύX�������ɌĂ΂��C�x���g�n���h��
//�����ŃI�[�f�B�I�R�[�f�b�N�̃t�H�[�}�b�g�̉��ς���
////////////////////////////////////////
void CEncodePropertyDialog::OnCbnSelchangeAudioEncoderBitrateCombo()
{
	CDirectShowAccess *pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CComboBox* combo = (CComboBox*)this->GetDlgItem(IDC_AUDIO_ENCODER_BITRATE_COMBO); assert(combo);
	int nIndexData = combo->GetCurSel();
	if (nIndexData == CB_ERR)
		return;
	int nItemData = (int)combo->GetItemData(nIndexData);

	/*
	assert(nItemData >= 0 && nItemData < (int)pDSA->m_AudioFormatList.size());
	if (nItemData < 0 || nItemData >= (int)pDSA->m_AudioFormatList.size())
		return;
	*/
	if (pDSA->SelectAudioCodecFormatIndex(nItemData))
	{
		//�ݒ�l�̋L��
		m_dwBitrate = pDSA->m_AudioFormatList[nItemData].dwBitrate;
		m_nSamplesPerSec = pDSA->m_AudioFormatList[nItemData].nSamplesPerSec;
		m_wBitsPerSample = pDSA->m_AudioFormatList[nItemData].wBitsPerSample;
		m_nChannels = pDSA->m_AudioFormatList[nItemData].nChannels;
		m_bAVSynchronization = pDSA->m_AudioFormatList[nItemData].bAVSynchronization;
	} else {
		//AfxMessageBox(_T("�I�[�f�B�I�R�[�f�b�N�̃t�H�[�}�b�g�ύX�Ɏ��s���܂����B"));
	}
	return;
}



////////////////////////////////////////
//�X���C�_�[�̈ړ�
////////////////////////////////////////
void CEncodePropertyDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	/*
	CSliderCtrl* slider = (CSliderCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SLIDER); assert(slider);
	if (pScrollBar->GetSafeHwnd() == slider->GetSafeHwnd())
	{
		//�G�f�B�b�g�{�b�N�X���X�V
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT);
		assert(edit);
		double dFPS = 0;
		dFPS = ((double)slider->GetPos()) / ((double)100);
		CString strNum;
		if (((int)(dFPS*100)) % 10 != 0)
			strNum.Format(_T("%.2f"),dFPS);
		else if (((int)(dFPS*10)) % 10 != 0)
			strNum.Format(_T("%.1f"),dFPS);
		else
			strNum.Format(_T("%.0f"),dFPS);
		edit->SetWindowText(strNum);
		//�{���͂�����Spin��ύX����R�[�h���K�v
		return;
	}
	slider = (CSliderCtrl*)this->GetDlgItem(IDC_VIDEO_QUALITY_SLIDER);
	assert(slider);
	if (pScrollBar->GetSafeHwnd() == slider->GetSafeHwnd())
	{
		//�G�f�B�b�g�{�b�N�X���X�V
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_QUALITY_EDIT);
		assert(edit);
		CString strNum;
		strNum.Format(_T("%d"),slider->GetPos());
		edit->SetWindowText(strNum);
		OnEnChangeVideoQualityEdit();
		//�{���͂�����Spin��ύX����R�[�h���K�v
		return;
	}
	*/
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}
////////////////////////////////////////
//�t���[�����[�g �e�L�X�g�{�b�N�X�̕ҏW
////////////////////////////////////////
void CEncodePropertyDialog::OnEnChangeVideoFramerateEdit()
{
	/*
	CSliderCtrl* slider = (CSliderCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SLIDER);
	if (!slider) return;
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT); assert(edit);
	CString strNum;
	edit->GetWindowText(strNum);
	//�l���ύX���ꂽ�|���X���C�_�[�R���g���[���ɒʒm����
	int nPos = (int)(_tstof(strNum) * 100);
	slider->SetPos(nPos);
	*/
	return;
}
////////////////////////////////////////
//�r�f�I�T�C�Y�̕ύX�{�^��
////////////////////////////////////////
void CEncodePropertyDialog::OnBnClickedSizeAutoRadio()
{
	CButton* button1 = (CButton*)this->GetDlgItem(IDC_SIZE_AUTO_RADIO); assert(button1);
	CEdit* edit1 = (CEdit*)this->GetDlgItem(IDC_VIDEO_WIDTH_EDIT); assert(edit1);
	CEdit* edit2 = (CEdit*)this->GetDlgItem(IDC_VIDEO_HEIGHT_EDIT); assert(edit2);
	CSpinButtonCtrl* spin1 = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_WIDTH_SPIN); assert(spin1);
	CSpinButtonCtrl* spin2 = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_HEIGHT_SPIN); assert(spin1);

	if (button1->GetCheck() & BST_CHECKED)
	{
		edit1->EnableWindow(FALSE);
		edit2->EnableWindow(FALSE);
		spin1->EnableWindow(FALSE);
		spin2->EnableWindow(FALSE);
	} else {
		edit1->EnableWindow(TRUE);
		edit2->EnableWindow(TRUE);
		spin1->EnableWindow(TRUE);
		spin2->EnableWindow(TRUE);
	}
}
////////////////////////////////////////
//�r�f�I�̊��炩���e�L�X�g�{�b�N�X�̕ҏW
////////////////////////////////////////
void CEncodePropertyDialog::OnEnChangeVideoQualityEdit()
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_QUALITY_EDIT); assert(edit);
	CString strNum;
	edit->GetWindowText(strNum);
	int nVideoEncodeQuality = _tstoi(strNum);
	if (nVideoEncodeQuality >= 80) {
		this->SetDlgItemText( IDC_VIDEO_QUALITY_STATIC, _T("(�ł��N��)"));
	} else if (nVideoEncodeQuality >= 60) {
		this->SetDlgItemText( IDC_VIDEO_QUALITY_STATIC, _T("(���N��)"));
	} else if (nVideoEncodeQuality >= 40) {
		this->SetDlgItemText( IDC_VIDEO_QUALITY_STATIC, _T("(���U)"));
	} else if (nVideoEncodeQuality >= 20) {
		this->SetDlgItemText( IDC_VIDEO_QUALITY_STATIC, _T("(��芊�炩)"));
	} else {
		this->SetDlgItemText( IDC_VIDEO_QUALITY_STATIC, _T("(�ł����炩)"));
	}
}

////////////////////////////////////////
//�c�[���`�b�v�̕\��
////////////////////////////////////////
BOOL CEncodePropertyDialog::PreTranslateMessage(MSG* pMsg)
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
////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
////////////////////////////////////////
BOOL CEncodePropertyDialog::ChangeProfileNotify()
{
	m_bFirstFlag = TRUE;	//�������t���O�L��
	LoadProfileSetting();
	m_bFirstFlag = FALSE;	//�������t���O����
	return TRUE;
}
////////////////////////////////////////
//�v���t�@�C������̃��[�h
////////////////////////////////////////
BOOL CEncodePropertyDialog::LoadProfileSetting()
{
	CString strNum;
	CDirectShowAccess *pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	//�r�f�I�G���R�[�h�R���{�{�b�N�X�̐ݒ�
	CComboBox* combo = (CComboBox*)this->GetDlgItem(IDC_VIDEO_ENCODER_COMBO); assert(combo);
	if (!pProfile->m_strVideoEncodeName.IsEmpty())	//INI�̐ݒ�L��
	{
		for (int i=0;i<(int)pDSA->m_VideoCodecList.size();i++)
		{
			if (pProfile->m_strVideoEncodeName.Compare(pDSA->m_VideoCodecList[i].strName.c_str()) == 0)
			{
				combo->SetCurSel(i);
				break;
			}
		}
		if (combo->GetCurSel() == CB_ERR){
			combo->SetCurSel(0);	//0�Ԗڂ̑I�������Ƃ�
		}
	} else {						//INI�̐ݒ肪����
		combo->SetCurSel(0);		//0�Ԗڂ̑I�������Ƃ�
	}
	//�r�f�I�r�b�g���[�g�̐ݒ�(�P�ʂ�Kbps)
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_BITRATE_SPIN); assert(spin);
	spin->SetPos32(pProfile->m_nVideoEnocdeBitrate);
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_BITRATE_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEnocdeBitrate);
	edit->SetWindowText(strNum);

	//�t���[�����[�g�̐ݒ�
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN); assert(spin);
	spin->SetRange(1,(int)pProfile->m_dVideoEncodeMaxFramerateSetting);
	spin->SetPos32((int)pProfile->m_dVideoEncodeFramerate);		//�X�s���{�^������ݒ肷��
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT); assert(edit);
	if (((int)(pProfile->m_dVideoEncodeFramerate*100)) % 10 != 0)
		strNum.Format(_T("%.2f"),pProfile->m_dVideoEncodeFramerate);
	else if (((int)(pProfile->m_dVideoEncodeFramerate*10)) % 10 != 0)
		strNum.Format(_T("%.1f"),pProfile->m_dVideoEncodeFramerate);
	else
		strNum.Format(_T("%.0f"),pProfile->m_dVideoEncodeFramerate);
	edit->SetWindowText(strNum);
	/*
	CSliderCtrl* slider = (CSliderCtrl*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_SLIDER); assert(slider);
	slider->SetPos((int)(pIni->m_dVideoEncodeFramerate*100));
	*/

	//�T�C�Y�̕ύX
	CButton* button1 = (CButton*)this->GetDlgItem(IDC_SIZE_AUTO_RADIO); assert(button1);
	CButton* button2 = (CButton*)this->GetDlgItem(IDC_SIZE_MANUAL_RADIO); assert(button2);
	if (pProfile->m_bVideoEncodeSize)	//��������(�T�C�Y�ύX�L��)
	{
		button1->SetCheck(BST_UNCHECKED);
		button2->SetCheck(BST_CHECKED);
	} else {	//��������(�T�C�Y�ύX����)
		button1->SetCheck(BST_CHECKED);
		button2->SetCheck(BST_UNCHECKED);
	}
	OnBnClickedSizeAutoRadio();

	//�r�f�I�̕��ƍ���
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_WIDTH_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEncodeWidth);
	edit->SetWindowText(strNum);
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_HEIGHT_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEncodeHeight);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_WIDTH_SPIN); assert(spin);
	spin->SetPos32(pProfile->m_nVideoEncodeWidth);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_HEIGHT_SPIN); assert(spin);
	spin->SetPos32(pProfile->m_nVideoEncodeHeight);

	//�r�f�I�̊��炩��
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_QUALITY_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nVideoEnocdeQuality);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_VIDEO_QUALITY_SPIN); assert(spin);
	OnEnChangeVideoQualityEdit();	//�X�^�e�B�b�N�e�L�X�g��ύX

	//�o�b�t�@�T�C�Y
	edit = (CEdit*)this->GetDlgItem(IDC_BUFFER_WINDOW_EDIT); assert(edit);
	strNum.Format(_T("%d"),pProfile->m_nBufferWindow/1000);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_BUFFER_WINDOW_SPIN); assert(spin);
	spin->SetPos32((pProfile->m_nBufferWindow)/1000);

	//�I�[�f�B�I�G���R�[�h�R���{�{�b�N�X�̐ݒ�
	combo = (CComboBox*)this->GetDlgItem(IDC_AUDIO_ENCODER_COMBO); assert(combo);
	if (!pProfile->m_strAudioEncodeName.IsEmpty())	//INI�̐ݒ�L��
	{
		for (int i=0;i<(int)pDSA->m_AudioCodecList.size();i++)
		{
			if (pProfile->m_strAudioEncodeName.Compare(pDSA->m_AudioCodecList[i].strName.c_str()) == 0)
			{
				combo->SetCurSel(i);
				break;
			}
		}
		if (combo->GetCurSel() == CB_ERR){
			combo->SetCurSel(0);	//0�Ԗڂ̑I�������Ƃ�
		}

		//�I�[�f�B�I�G���R�[�_�[�̏ڍׂ̓ǂݍ���
		m_dwBitrate = pProfile->m_nAudioEncodeBitrate;
		m_nSamplesPerSec = pProfile->m_nAudioEncodeSamplesPerSec;
		m_wBitsPerSample = pProfile->m_nAudioEncodeBitsPerSample;
		m_nChannels = pProfile->m_nAudioEncodeChannels;
		m_bAVSynchronization = pProfile->m_bAudioEncodeAVSynchronization;

		//�I�[�f�B�I�G���R�[�_�[�ݒ�R���{�{�b�N�X��ύX
		OnCbnSelchangeAudioEncoderCombo();
	} else {						//INI�̐ݒ肪����
		combo->SetCurSel(0);		//0�Ԗڂ̑I�������Ƃ�
	}
	return TRUE;
}
////////////////////////////////////////
//�v���t�@�C���ւ̃Z�[�u
////////////////////////////////////////
BOOL CEncodePropertyDialog::SaveProfileSetting()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	//�r�f�I�G���R�[�_�[�ݒ�̕ۑ�
	CComboBox* combo = (CComboBox*)this->GetDlgItem(IDC_VIDEO_ENCODER_COMBO); assert(combo);
	combo->GetLBText(combo->GetCurSel(),pProfile->m_strVideoEncodeName);
	//�r�b�g���[�g�ݒ�̕ۑ�
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_BITRATE_EDIT); assert(edit);
	CString strNum;
	edit->GetWindowText(strNum);
	pProfile->m_nVideoEnocdeBitrate = _tstoi(strNum);
	//�r�f�I�̃t���[�����[�g�̕ۑ�
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT); assert(edit);
	edit->GetWindowText(strNum);
	pProfile->m_dVideoEncodeFramerate = (double)_tstof(strNum);
	//�r�f�I�G���R�[�h�̃T�C�Y�ύX�̕ۑ�
	CButton* button1 = (CButton*)this->GetDlgItem(IDC_SIZE_AUTO_RADIO); assert(button1);
	if (button1->GetCheck() & BST_CHECKED)
		pProfile->m_bVideoEncodeSize = FALSE;
	else 
		pProfile->m_bVideoEncodeSize = TRUE;
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_WIDTH_EDIT); assert(edit);
	edit->GetWindowText(strNum);
	pProfile->m_nVideoEncodeWidth = _tstoi(strNum);
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_HEIGHT_EDIT); assert(edit);
	edit->GetWindowText(strNum);
	pProfile->m_nVideoEncodeHeight = _tstoi(strNum);
	//�r�f�I�G���R�[�h�̊��炩���̕ۑ�
	edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_QUALITY_EDIT); assert(edit);
	edit->GetWindowText(strNum);
	pProfile->m_nVideoEnocdeQuality = _tstoi(strNum);
	//�o�b�t�@�T�C�Y�̕ۑ�
	edit = (CEdit*)this->GetDlgItem(IDC_BUFFER_WINDOW_EDIT);
	assert(edit);
	edit->GetWindowText(strNum);
	pProfile->m_nBufferWindow = _tstoi(strNum)*1000;

	//�I�[�f�B�I�G���R�[�_�[�̕ۑ�
	combo = (CComboBox*)this->GetDlgItem(IDC_AUDIO_ENCODER_COMBO); assert(combo);
	if (combo->IsWindowEnabled())
		combo->GetLBText(combo->GetCurSel(),pProfile->m_strAudioEncodeName);
	else
		pProfile->m_strAudioEncodeName = _T("");
	//�I�[�f�B�I�G���R�[�_�[�̏ڍׂ̕ۑ�
	if (m_dwBitrate != 0 && m_dwBitrate != -1)
		pProfile->m_nAudioEncodeBitrate = m_dwBitrate;
	if (m_nSamplesPerSec != 0 && m_nSamplesPerSec != -1)
		pProfile->m_nAudioEncodeSamplesPerSec = m_nSamplesPerSec;
	if (m_wBitsPerSample != 0 && m_wBitsPerSample != -1)
		pProfile->m_nAudioEncodeBitsPerSample = m_wBitsPerSample;
	if (m_nChannels != 0 && m_nChannels != -1)
		pProfile->m_nAudioEncodeChannels = m_nChannels;
	pProfile->m_bAudioEncodeAVSynchronization = m_bAVSynchronization;

	return TRUE;
}

////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
////////////////////////////////////////
BOOL CEncodePropertyDialog::Encode(BOOL bStart)
{
	if (bStart)
	{
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
		//�ݒ�l�ꗗ
		DWORD dwBitrate = 0;		//�r�b�g���[�g
		DWORD msBufferWindow = 0;	//�o�b�t�@����
		double dFPS = 0.0;			//FPS
		CRect rcSource(0,0,0,0);	//�]������Rect
		CRect rcTarget(0,0,0,0);	//�]�����Rect
		DWORD dwQuality = 0;		//�Ȃ߂炩��
		int nMaxKeyFrameSpacing = 0;//�L�[�t���[���̊Ԋu

		//�r�b�g���[�g
		CString strNum;
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_BITRATE_EDIT);
		edit->GetWindowText(strNum);
		dwBitrate = _tstoi(strNum);
		//�o�b�t�@�E�B���h�E
		edit = (CEdit*)this->GetDlgItem(IDC_BUFFER_WINDOW_EDIT);
		edit->GetWindowText(strNum);
		msBufferWindow = _tstoi(strNum) * 1000;
		if (msBufferWindow == -1 || msBufferWindow == 0)	//3000���W��
			msBufferWindow = 3000;
		//�t���[�����[�g
		edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT);
		edit->GetWindowText(strNum);
		dFPS = (double)_tstof(strNum);
		//RECT�\���̂�ҏW����
		//TODO ������RECT�\���̃T�C�Y���{���̃T�C�Y��ς���Ă��܂��Ă���ꍇ������
		//�Ⴆ�΁ASCFH DSF�v���p�e�B�ŉ摜�T�C�Y��ҏW���Ă���ƁA���̃T�C�Y���������Ȃ�
		int nSrcWidth = 0;
		int nSrcHeight = 0;
		pDSA->GetCurrentVideoSize(nSrcWidth,nSrcHeight);
		rcSource.right = nSrcWidth;		//�����̕��ƍ����̎w�肨�������H
		rcSource.bottom = nSrcHeight;	//�����̕��ƍ����̎w�肪���������H
		assert(rcSource.right != 0 && rcSource.bottom != 0);
		CButton* button = (CButton*)this->GetDlgItem(IDC_SIZE_AUTO_RADIO);
		if (button->GetCheck() & BST_CHECKED)
			rcTarget = rcSource;
		else  {
			edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_WIDTH_EDIT);
			edit->GetWindowText(strNum);
			rcTarget.right= _tstoi(strNum);
			edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_HEIGHT_EDIT);
			edit->GetWindowText(strNum);
			rcTarget.bottom = _tstoi(strNum);
		}

		//���炩��
		edit = (CEdit*)this->GetDlgItem(IDC_VIDEO_QUALITY_EDIT);
		edit->GetWindowText(strNum);
		dwQuality = _tstoi(strNum);
		//�L�[�t���[���̊Ԋu
		BOOL bMaxKeyFrameSpacing = pProfile->m_bMaxKeyFrameSpacing;
		if (bMaxKeyFrameSpacing || nMaxKeyFrameSpacing == -1 || nMaxKeyFrameSpacing == 0)
		{
			if (dwBitrate <= 300)
				nMaxKeyFrameSpacing = 8000;
			else if (dwBitrate <= 600)
				nMaxKeyFrameSpacing = 6000;
			else if (dwBitrate <= 2000)
				nMaxKeyFrameSpacing = 4000;
			else
				nMaxKeyFrameSpacing = 3000;
		}
		//�r�f�I�̃p�t�H�[�}���X
		WORD wComplexity = (WORD)pProfile->m_nVideoComplexity;
		BOOL bComplexity = pProfile->m_bVideoComplexity;
		if (bComplexity == FALSE)
		{
			CComboBox* combo = (CComboBox*)this->GetDlgItem(IDC_VIDEO_ENCODER_COMBO); assert(combo);
			int nCodecIndex = combo->GetCurSel();
			if (nCodecIndex >= 0 && nCodecIndex < (int)pDSA->m_VideoCodecList.size())
			{
				WORD wMaxComplexity = (WORD)pDSA->m_VideoCodecList[nCodecIndex].dwComplexityMax;
				if (wComplexity > wMaxComplexity)
				{
					CString strMessage;
					strMessage.Format(_T("�R�[�f�b�N(%s)�̃r�f�I�p�t�H�[�}���X�͈̔͂�0����%d�܂ł��B\n�r�f�I�p�t�H�[�}���X�ݒ�͖����ɂȂ�A�R�[�f�b�N�̐����l���g���܂�"),
						pDSA->m_VideoCodecList[nCodecIndex].strName.c_str(),wMaxComplexity);
					AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
					bComplexity = TRUE;
				}
			} else {
				AfxMessageBox(_T("�R�[�f�b�N�̑I�����s���ł�"),MB_OK|MB_ICONINFORMATION);
				bComplexity = TRUE;
			}
		}

		BOOL bRet;
		//�ݒ��K�p����
		bRet = pDSA->SetVideoCodecSetting(dwBitrate*1000,msBufferWindow,dFPS,rcSource,rcTarget);
		if (bRet == FALSE) return FALSE;
		bRet = pDSA->SetVideoCodecSetting2(dwQuality,nMaxKeyFrameSpacing);
		if (bRet == FALSE) return FALSE;
		if (!bComplexity) {
			bRet = pDSA->SetVideoComplexity(wComplexity);
			if (bRet == FALSE) return FALSE;
		}
	}

	//�_�C�A���O�̖����ƗL��
	((CWnd*)GetDlgItem(IDC_VIDEO_ENCODER_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_VIDEO_BITRATE_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_VIDEO_BITRATE_SPIN))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_VIDEO_FRAMERATE_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_VIDEO_FRAMERATE_SPIN))->EnableWindow(bStart ? FALSE : TRUE);
	//((CWnd*)GetDlgItem(IDC_VIDEO_FRAMERATE_SLIDER))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_SIZE_AUTO_RADIO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_SIZE_MANUAL_RADIO))->EnableWindow(bStart ? FALSE : TRUE);
	CButton* button = (CButton*)this->GetDlgItem(IDC_SIZE_MANUAL_RADIO);
	if (button->GetCheck() & BST_CHECKED) {
		((CWnd*)GetDlgItem(IDC_VIDEO_WIDTH_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
		((CWnd*)GetDlgItem(IDC_VIDEO_WIDTH_SPIN))->EnableWindow(bStart ? FALSE : TRUE);
		((CWnd*)GetDlgItem(IDC_VIDEO_HEIGHT_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
		((CWnd*)GetDlgItem(IDC_VIDEO_HEIGHT_SPIN))->EnableWindow(bStart ? FALSE : TRUE);
	}
	((CWnd*)GetDlgItem(IDC_VIDEO_QUALITY_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_VIDEO_QUALITY_SPIN))->EnableWindow(bStart ? FALSE : TRUE);
	//((CWnd*)GetDlgItem(IDC_VIDEO_QUALITY_SLIDER))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_BUFFER_WINDOW_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_BUFFER_WINDOW_SPIN))->EnableWindow(bStart ? FALSE : TRUE);

	((CWnd*)GetDlgItem(IDC_AUDIO_ENCODER_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_AUDIO_ENCODER_BITRATE_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_AUDIO_ENCODER_SAMPLESPERSEC_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_AUDIO_ENCODER_BITSPERSAMPLE_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_AUDIO_ENCODER_CHANNELS_COMBO))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_AUDIO_ENCODER_AVSYNCHRONIZATION))->EnableWindow(bStart ? FALSE : TRUE);

	return TRUE;
}