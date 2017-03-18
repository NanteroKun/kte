// InputPropertyConfigDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "InputPropertyConfigDialog.h"
#include "afxdialogex.h"


// CInputPropertyConfigDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CInputPropertyConfigDialog, CDialog)

BEGIN_MESSAGE_MAP(CInputPropertyConfigDialog, CDialog)
	ON_BN_CLICKED(IDC_AUDIO_BUFFER_AUTO_RADIO, &CInputPropertyConfigDialog::OnBnClickedAudioBufferAutoRadio)
	ON_BN_CLICKED(IDC_AUDIO_BUFFER_MANUAL_RADIO, &CInputPropertyConfigDialog::OnBnClickedAudioBufferManualRadio)
	ON_BN_CLICKED(IDC_CLOCK_DISABLE_RADIO, &CInputPropertyConfigDialog::OnBnClickedClockDisableRadio)
	ON_BN_CLICKED(IDC_CLOCK_ENABLE_RADIO, &CInputPropertyConfigDialog::OnBnClickedClockEnableRadio)
	ON_BN_CLICKED(IDC_INPUT_SETTING_RESET_BUTTON, &CInputPropertyConfigDialog::OnBnClickedInputSettingResetButton)
END_MESSAGE_MAP()

CInputPropertyConfigDialog::CInputPropertyConfigDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputPropertyConfigDialog::IDD, pParent)
{

}

CInputPropertyConfigDialog::~CInputPropertyConfigDialog()
{
	m_bChange = FALSE;
	m_bAudioBufferChange = FALSE;
}

void CInputPropertyConfigDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


// CInputPropertyConfigDialog ���b�Z�[�W �n���h���[

//////////////////////////////////////////////////
//�_�C�A���O������
//////////////////////////////////////////////////
BOOL CInputPropertyConfigDialog::OnInitDialog()
{
	CDialog::OnInitDialog();


	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);

	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_CLOCK_DISABLE_RADIO),IDC_CLOCK_DISABLE_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_CLOCK_ENABLE_RADIO),IDC_CLOCK_ENABLE_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_CLOCK_ENABLE_AUTO_RADIO),IDC_CLOCK_ENABLE_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_CLOCK_ENABLE_SYSTEM_RADIO),IDC_CLOCK_ENABLE_SYSTEM_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_CLOCK_ENABLE_DSOUND_RADIO),IDC_CLOCK_ENABLE_DSOUND_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_SYNC_DISABLE_RADIO),IDC_SYNC_DISABLE_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_SYNC_ENABLE_RADIO),IDC_SYNC_ENABLE_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_BUFFER_AUTO_RADIO),IDC_AUDIO_BUFFER_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_BUFFER_MANUAL_RADIO),IDC_AUDIO_BUFFER_MANUAL_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_BUFFER_TIME_EDIT),IDC_AUDIO_BUFFER_TIME_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_BUFFER_TIME_SPIN),IDC_AUDIO_BUFFER_TIME_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_BUFFER_COUNT_EDIT),IDC_AUDIO_BUFFER_COUNT_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_AUDIO_BUFFER_COUNT_SPIN),IDC_AUDIO_BUFFER_COUNT_EDIT);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);



	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	CButton* button;
	CEdit* edit;

	m_bChange = FALSE;
	m_bAudioBufferChange = FALSE;

	//��N���b�N�̐ݒ�
	if (pProfile->m_bGraphClockEnable) {
		button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
	} else {
		button = (CButton*)GetDlgItem(IDC_CLOCK_DISABLE_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
		OnBnClickedClockEnableRadio(FALSE);
	}
	switch (pProfile->m_nGraphClockSetting)
	{
	case 0:
		button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_AUTO_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
		break;
	case 1:
		button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_SYSTEM_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
		break;
	case 2:
		button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_DSOUND_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
		break;
	}
	//�����Ɋւ���ݒ�
	if (pProfile->m_bGraphSyncEnable) {
		button = (CButton*)GetDlgItem(IDC_SYNC_ENABLE_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
	} else {
		button = (CButton*)GetDlgItem(IDC_SYNC_DISABLE_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
	}

	//�I�[�f�B�I�L���v�`���o�b�t�@
	if (pProfile->m_bAudioBufferEnable){
		button = (CButton*)GetDlgItem(IDC_AUDIO_BUFFER_MANUAL_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
	} else {
		button = (CButton*)GetDlgItem(IDC_AUDIO_BUFFER_AUTO_RADIO); assert(button);
		button->SetCheck(BST_CHECKED);
		OnBnClickedAudioBufferRadio(FALSE);
	}

	CString strNum;
	strNum.Format(_T("%d"),pProfile->m_nAudioBufferTime);
	edit = (CEdit*)GetDlgItem(IDC_AUDIO_BUFFER_TIME_EDIT); assert(edit);
	edit->SetWindowText(strNum);

	CSpinButtonCtrl* spin;
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_AUDIO_BUFFER_TIME_SPIN); assert(spin);
	spin->SetRange32(10,3000);
	spin->SetPos32(pProfile->m_nAudioBufferTime);

	strNum.Format(_T("%d"),pProfile->m_nAudioBufferCount);
	edit = (CEdit*)GetDlgItem(IDC_AUDIO_BUFFER_COUNT_EDIT); assert(edit);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_AUDIO_BUFFER_COUNT_SPIN); assert(spin);
	spin->SetRange32(-1,20);
	spin->SetPos32(pProfile->m_nAudioBufferCount);

	//���݂̃A���P�[�^�̃o�b�t�@�T�C�Y�ƌ��̎擾
	int nAudioAllocTime = -1;	//�A���P�[�^�̎���(�~���b)
	int nAudioAllocCount = -1;	//�A���P�[�^�̌�
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	long lBufferSize = pDSA->m_AudioGetAllocator.cbBuffer;	//�o�b�t�@�̃T�C�Y
	nAudioAllocCount = pDSA->m_AudioGetAllocator.cBuffers;	//�A���P�[�^�̌�
	//���݂̃t�H�[�}�b�g�ݒ���o�b�t�@�T�C�Y���Z�肷��
	if (lBufferSize != -1)
	{
		if (pDSA->m_AudioFirstMediaType.formattype == FORMAT_WaveFormatEx)
		{
			WAVEFORMATEX* WaveFormatEx = (WAVEFORMATEX*)pDSA->m_AudioFirstMediaType.pbFormat;
			if (WaveFormatEx->nAvgBytesPerSec != 0)
				nAudioAllocTime = (1000 * lBufferSize) / WaveFormatEx->nAvgBytesPerSec;
		}
	}
	strNum.Format(_T("���݂̃o�b�t�@�T�C�Y %d �~���b\r\n���݂̃o�b�t�@��     %d ��"),
		nAudioAllocTime,nAudioAllocCount);
	CWnd* wnd = (CWnd*)GetDlgItem(IDC_AUDIO_BUFFER_NOW_STATIC); assert(wnd);
	wnd->SetWindowText(strNum);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

//////////////////////////////////////////////////
//OK�{�^��
//////////////////////////////////////////////////
void CInputPropertyConfigDialog::OnOK()
{
	CButton* button;
	CEdit* edit;
	CString strNum;

	//�ݒ荀��
	BOOL bGraphClockEnable;
	int	nGraphClockSetting;
	BOOL bGraphSyncEnable;
	BOOL bAudioBufferEnable;
	int nAudioBufferTime;
	int nAudioBufferCount;

	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_RADIO); assert(button);
	if (button->GetCheck() & BST_CHECKED) {
		bGraphClockEnable = TRUE;
	} else {
		bGraphClockEnable = FALSE;
	}

	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_AUTO_RADIO); assert(button);
	if (button->GetCheck() & BST_CHECKED) {
		nGraphClockSetting = 0; }
	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_SYSTEM_RADIO); assert(button);
	if (button->GetCheck() & BST_CHECKED) {
		nGraphClockSetting = 1; }
	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_DSOUND_RADIO); assert(button);
	if (button->GetCheck() & BST_CHECKED) {
		nGraphClockSetting = 2;}

	button = (CButton*)GetDlgItem(IDC_SYNC_ENABLE_RADIO); assert(button);
	if (button->GetCheck() & BST_CHECKED)
		bGraphSyncEnable = TRUE;
	else
		bGraphSyncEnable = FALSE;

	button = (CButton*)GetDlgItem(IDC_AUDIO_BUFFER_MANUAL_RADIO); assert(button);
	if (button->GetCheck() & BST_CHECKED)
		bAudioBufferEnable = TRUE;
	else
		bAudioBufferEnable = FALSE;

	edit = (CEdit*)GetDlgItem(IDC_AUDIO_BUFFER_TIME_EDIT); assert(edit);
	edit->GetWindowText(strNum);
	nAudioBufferTime = _tstoi(strNum);

	edit = (CEdit*)GetDlgItem(IDC_AUDIO_BUFFER_COUNT_EDIT); assert(edit);
	edit->GetWindowText(strNum);
	nAudioBufferCount = _tstoi(strNum);


	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	if (pProfile->m_bGraphClockEnable != bGraphClockEnable)
		m_bChange = TRUE;
	if (bGraphClockEnable == TRUE && (pProfile->m_nGraphClockSetting != nGraphClockSetting))
		m_bChange = TRUE;
	if (pProfile->m_bGraphSyncEnable != bGraphSyncEnable)
		m_bChange = TRUE;

	if (pProfile->m_bAudioBufferEnable != bAudioBufferEnable)
		m_bAudioBufferChange = TRUE;
	if (bAudioBufferEnable == TRUE && (pProfile->m_nAudioBufferTime != nAudioBufferTime || pProfile->m_nAudioBufferCount != nAudioBufferCount) )
		m_bAudioBufferChange = TRUE;
	//�ݒ�̓K�p
	pProfile->m_bGraphClockEnable  = bGraphClockEnable;
	pProfile->m_nGraphClockSetting = nGraphClockSetting;
	pProfile->m_bGraphSyncEnable   = bGraphSyncEnable;
	pProfile->m_bAudioBufferEnable = bAudioBufferEnable;
	pProfile->m_nAudioBufferTime   = nAudioBufferTime;
	pProfile->m_nAudioBufferCount  = nAudioBufferCount;

	if (m_bAudioBufferChange)
		m_bChange = TRUE;

	if (m_bChange)
	{
		//�t�B���^�̍č\�z�������ōs��
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		BOOL bRunFilter = pDSA->IsRunFilter();
		if (bRunFilter)
			pDSA->StopFilter();

		if (m_bAudioBufferChange)
		{
			if (pDSA->m_AudioFirstMediaType.formattype != GUID_NULL) {
				if (!pDSA->SelectAudioSettingIndex(pDSA->GetAudioSettingIndex(),&pDSA->m_AudioFirstMediaType))
				{
					AfxMessageBox(_T("�I�[�f�B�I�o�b�t�@�̐ݒ�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
					return;
				}
			}
		}
		if (pDSA->IsCanPreview())
			pDSA->RunFilter();
		//�����Ńt�B���^���\�z�ł��Ȃ��ꍇ������̂ŁA�G���[���͂��悤�ɂ���
	}
	CDialog::OnOK();
}

//////////////////////////////////////////////////
//�L�����Z���{�^��
//////////////////////////////////////////////////
void CInputPropertyConfigDialog::OnCancel()
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	CDialog::OnCancel();
}
////////////////////////////////////////
//�����l�ɖ߂�
////////////////////////////////////////
void CInputPropertyConfigDialog::OnBnClickedInputSettingResetButton()
{
	CButton* button1;
	CButton* button2;
	//��N���b�N
	button1 = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_RADIO); assert(button1);
	button1->SetCheck(BST_CHECKED);
	button2 = (CButton*)GetDlgItem(IDC_CLOCK_DISABLE_RADIO); assert(button2);
	button2->SetCheck(BST_UNCHECKED);
	OnBnClickedClockEnableRadio(TRUE);

	button1 = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_AUTO_RADIO); assert(button1);
	button1->SetCheck(BST_CHECKED);
	button1 = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_SYSTEM_RADIO); assert(button1);
	button1->SetCheck(BST_UNCHECKED);
	button1 = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_DSOUND_RADIO); assert(button1);
	button1->SetCheck(BST_UNCHECKED);

	//����
	button1 = (CButton*)GetDlgItem(IDC_SYNC_ENABLE_RADIO); assert(button1);
	button1->SetCheck(BST_UNCHECKED);
	button2 = (CButton*)GetDlgItem(IDC_SYNC_DISABLE_RADIO); assert(button2);
	button2->SetCheck(BST_CHECKED);

	//�I�[�f�B�I�o�b�t�@
	button1 = (CButton*)GetDlgItem(IDC_AUDIO_BUFFER_AUTO_RADIO); assert(button1);
	button1->SetCheck(BST_UNCHECKED);
	button2 = (CButton*)GetDlgItem(IDC_AUDIO_BUFFER_MANUAL_RADIO); assert(button2);
	button2->SetCheck(BST_CHECKED);
	OnBnClickedAudioBufferRadio(TRUE);

	CSpinButtonCtrl* spin;
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_AUDIO_BUFFER_TIME_SPIN); assert(spin);
	spin->SetPos32(50);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_AUDIO_BUFFER_COUNT_SPIN); assert(spin);
	spin->SetPos32(-1);
}

//////////////////////////////////////////////////
//��N���b�N�̃��W�I�{�^��
//////////////////////////////////////////////////
void CInputPropertyConfigDialog::OnBnClickedClockEnableRadio(BOOL bEnable)
{
	CButton* button;
	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_AUTO_RADIO); assert(button);
	button->EnableWindow(bEnable);
	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_SYSTEM_RADIO); assert(button);
	button->EnableWindow(bEnable);
	button = (CButton*)GetDlgItem(IDC_CLOCK_ENABLE_DSOUND_RADIO); assert(button);
	button->EnableWindow(bEnable);
}
//////////////////////////////////////////////////
//�o�b�t�@�T�C�Y�̃��W�I�{�^��
//////////////////////////////////////////////////
void CInputPropertyConfigDialog::OnBnClickedAudioBufferRadio(BOOL bEnable)
{
	CEdit* edit;
	edit = (CEdit*)GetDlgItem(IDC_AUDIO_BUFFER_TIME_EDIT); assert(edit);
	edit->EnableWindow(bEnable);
	edit = (CEdit*)GetDlgItem(IDC_AUDIO_BUFFER_COUNT_EDIT); assert(edit);
	edit->EnableWindow(bEnable);
	CSpinButtonCtrl* spin;
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_AUDIO_BUFFER_TIME_SPIN); assert(spin);
	spin->EnableWindow(bEnable);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_AUDIO_BUFFER_COUNT_SPIN); assert(spin);
	spin->EnableWindow(bEnable);
}
////////////////////////////////////////
//�c�[���`�b�v�̕\��
////////////////////////////////////////
BOOL CInputPropertyConfigDialog::PreTranslateMessage(MSG* pMsg)
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
