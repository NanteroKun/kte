// SettingDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "SettingDialog.h"


// CSettingDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CSettingDialog, CDialog)

CSettingDialog::CSettingDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSettingDialog::IDD, pParent)
{
}

CSettingDialog::~CSettingDialog()
{
}

void CSettingDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CSettingDialog, CDialog)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDOK, &CSettingDialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSettingDialog::OnBnClickedCancel)
	ON_BN_CLICKED(IDAPPLY, &CSettingDialog::OnBnClickedApply)
	ON_BN_CLICKED(IDC_VIDEO_PLAYER_BUTTON, &CSettingDialog::OnBnClickedVideoPlayerButton)
	ON_BN_CLICKED(IDC_KEYFRAME_AUTO_RADIO, &CSettingDialog::OnBnClickedKeyframeAutoRadio)
	ON_BN_CLICKED(IDC_KEYFRAME_MANUAL_RADIO, &CSettingDialog::OnBnClickedKeyframeManualRadio)
	ON_BN_CLICKED(IDC_COMPLEXITY_AUTO_RADIO, &CSettingDialog::OnBnClickedComplexityAutoRadio)
	ON_BN_CLICKED(IDC_COMPLEXITY_MANUAL_RADIO, &CSettingDialog::OnBnClickedComplexityManualRadio)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_FORCE_PREVIEW_OFF_CHECK, &CSettingDialog::OnBnClickedForcePreviewOffCheck)
END_MESSAGE_MAP()


// CSettingDialog ���b�Z�[�W �n���h��

BOOL CSettingDialog::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	return CDialog::PreCreateWindow(cs);
}

int CSettingDialog::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  �����ɓ���ȍ쐬�R�[�h��ǉ����Ă��������B
	return 0;
}
//////////////////////////////////////////////////
//�_�C�A���O�̏�����
//////////////////////////////////////////////////
BOOL CSettingDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_MINI_TASKBAR_CHECK),IDC_MINI_TASKBAR_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_CREATEMUTEX_CHECK),IDC_CREATEMUTEX_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_SHOWTOPMOST_CHECK),IDC_SHOWTOPMOST_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_KEYFRAME_AUTO_RADIO),IDC_KEYFRAME_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_KEYFRAME_MANUAL_RADIO),IDC_KEYFRAME_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_KEYFRAME_EDIT),IDC_KEYFRAME_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_COMPLEXITY_AUTO_RADIO),IDC_COMPLEXITY_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_COMPLEXITY_MANUAL_RADIO),IDC_COMPLEXITY_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_COMPLEXITY_SLIDER),IDC_COMPLEXITY_SLIDER);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VIDEO_PLAYER_EDIT),IDC_VIDEO_PLAYER_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_GLOBALIP_CHECK),IDC_GLOBALIP_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_GLOBALIPURL_EDIT),IDC_GLOBALIPURL_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_FORCE_DISCONNECT_CHECK),IDC_FORCE_DISCONNECT_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_FORCE_PREVIEW_OFF_CHECK),IDC_FORCE_PREVIEW_OFF_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VMR_AUTO_RADIO),IDC_VMR_AUTO_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VMR_0_RADIO),IDC_VMR_0_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VMR_7_OFF_RADIO),IDC_VMR_7_OFF_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VMR_7_ON_RADIO),IDC_VMR_7_ON_RADIO);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_VMR_9_RADIO),IDC_VMR_9_RADIO);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	if (pSetting->m_bMiniTaskBar) {
		((CButton*)GetDlgItem(IDC_MINI_TASKBAR_CHECK))->SetCheck(BST_CHECKED);
	}
	if (pSetting->m_bCreateMutex) {
		((CButton*)GetDlgItem(IDC_CREATEMUTEX_CHECK))->SetCheck(BST_CHECKED);
	}
	if (pSetting->m_bShowTopMost) {
		((CButton*)GetDlgItem(IDC_SHOWTOPMOST_CHECK))->SetCheck(BST_CHECKED);
	}

	CEdit* edit;
	CButton *button1,*button2;

	//�r�f�I�v���C���[�̃p�X
	edit = (CEdit*)GetDlgItem(IDC_VIDEO_PLAYER_EDIT);
	assert(edit);
	edit->SetWindowText(pSetting->m_strVideoPlayerPath);

	//�O���[�o��IP�擾��URL
	button1 = (CButton*)GetDlgItem(IDC_GLOBALIP_CHECK);
	assert(button1);
	if (pSetting->m_bGetGlobalIPCheck)
	{
		button1->SetCheck(BST_CHECKED);
	} else {
		button1->SetCheck(BST_UNCHECKED);
	}
	edit = (CEdit*)GetDlgItem(IDC_GLOBALIPURL_EDIT);
	assert(edit);
	edit->SetWindowText(pSetting->m_strGetGlobalIPURL);

	//�G���R�[�h�I�����Ƀl�b�g���[�N�ڑ��������ؒf����
	button1 = (CButton*)GetDlgItem(IDC_FORCE_DISCONNECT_CHECK); assert(button1);
	if (pProfile->m_bOutputForceDisconnect)
	{
		button1->SetCheck(BST_CHECKED);
	} else {
		button1->SetCheck(BST_UNCHECKED);
	}

	//�v���r���[�֌W
	switch (pSetting->m_nPreviewVMR)
	{
	case 0:
		((CButton*)GetDlgItem(IDC_VMR_0_RADIO))->SetCheck(BST_CHECKED);
		break;
	case 1:
		((CButton*)GetDlgItem(IDC_VMR_AUTO_RADIO))->SetCheck(BST_CHECKED);
		break;
	case 7:
		if (pSetting->m_bPreviewVMROverlay)
			((CButton*)GetDlgItem(IDC_VMR_7_ON_RADIO))->SetCheck(BST_CHECKED);
		else
			((CButton*)GetDlgItem(IDC_VMR_7_OFF_RADIO))->SetCheck(BST_CHECKED);
		break;
	case 9:
		((CButton*)GetDlgItem(IDC_VMR_9_RADIO))->SetCheck(BST_CHECKED);
		break;
	}
	//���S�Ƀv���r���[���~����
	if (pSetting->m_bPreviewForceOff) {
		((CButton*)GetDlgItem(IDC_FORCE_PREVIEW_OFF_CHECK))->SetCheck(BST_CHECKED);
	}
	OnBnClickedForcePreviewOffCheck();

	//�L�[�t���[���̊Ԋu
	BOOL bMaxKeyFrameSpacing = pProfile->m_bMaxKeyFrameSpacing;
	button1 = (CButton*)GetDlgItem(IDC_KEYFRAME_AUTO_RADIO);
	button2 = (CButton*)GetDlgItem(IDC_KEYFRAME_MANUAL_RADIO);
	assert(button1 && button2);
	if (bMaxKeyFrameSpacing)
	{
		button1->SetCheck(BST_CHECKED);
		button2->SetCheck(BST_UNCHECKED);
	} else {
		button1->SetCheck(BST_UNCHECKED);
		button2->SetCheck(BST_CHECKED);
	}
	CheckKeyFrameButton(bMaxKeyFrameSpacing?FALSE:TRUE);

	edit = (CEdit*)GetDlgItem(IDC_KEYFRAME_EDIT); assert(edit);
	CString strNum;
	strNum.Format(_T("%d"),pProfile->m_nMaxKeyFrameSpacing/1000);
	edit->SetWindowText(strNum);
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_KEYFRAME_SPIN); assert(spin);
	spin->SetRange32(1,30);
	spin->SetPos32(pProfile->m_nMaxKeyFrameSpacing/1000);

	//�r�f�I�p�t�H�[�}���X(�R�[�f�b�N�̕��G��)
	BOOL bVideoComplexity = pProfile->m_bVideoComplexity;
	button1 = (CButton*)GetDlgItem(IDC_COMPLEXITY_AUTO_RADIO);
	button2 = (CButton*)GetDlgItem(IDC_COMPLEXITY_MANUAL_RADIO);
	assert(button1 && button2);
	if (bVideoComplexity)
	{
		button1->SetCheck(BST_CHECKED);
		button2->SetCheck(BST_UNCHECKED);
	} else {
		button1->SetCheck(BST_UNCHECKED);
		button2->SetCheck(BST_CHECKED);
	}
	CheckComplexityButton(bVideoComplexity?FALSE:TRUE);

	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_COMPLEXITY_SLIDER); assert(slider);
	slider->SetRange(0,5);
	if (pProfile->m_nVideoComplexity >= 0 && (pProfile->m_nVideoComplexity <= 5))
		slider->SetPos(pProfile->m_nVideoComplexity);
	else 
		slider->SetPos(0);
	//this->OnHScroll(0,slider->GetPos(),(CScrollBar*)slider);

	return TRUE;
}

//////////////////////////////////////////////////
//OK�{�^��
//////////////////////////////////////////////////
void CSettingDialog::OnBnClickedOk()
{
	OnBnClickedApply();	//�K�p�{�^��
	OnOK();
}

//////////////////////////////////////////////////
//�L�����Z���{�^��
//////////////////////////////////////////////////
void CSettingDialog::OnBnClickedCancel()
{
	OnCancel();
}

//////////////////////////////////////////////////
//�K�p�{�^��(�ݒ�l��ۑ�)
//////////////////////////////////////////////////
void CSettingDialog::OnBnClickedApply()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	pSetting->m_bMiniTaskBar = ((CButton*)GetDlgItem(IDC_MINI_TASKBAR_CHECK))->GetCheck() & BST_CHECKED;
	pSetting->m_bCreateMutex = ((CButton*)GetDlgItem(IDC_CREATEMUTEX_CHECK))->GetCheck() & BST_CHECKED;
	pSetting->m_bShowTopMost = ((CButton*)GetDlgItem(IDC_SHOWTOPMOST_CHECK))->GetCheck() & BST_CHECKED;

	CEdit* edit;
	CButton* button1;

	//�r�f�I�v���C���[�̃p�X
	edit = (CEdit*)GetDlgItem(IDC_VIDEO_PLAYER_EDIT); assert(edit);
	edit->GetWindowText(pSetting->m_strVideoPlayerPath);

	//�O���[�o��IP�擾��URL
	pSetting->m_bGetGlobalIPCheck = ((CButton*)GetDlgItem(IDC_GLOBALIP_CHECK))->GetCheck() & BST_CHECKED;

	edit = (CEdit*)GetDlgItem(IDC_GLOBALIPURL_EDIT); assert(edit);
	edit->GetWindowText(pSetting->m_strGetGlobalIPURL);

	//�G���R�[�h�I�����Ƀl�b�g���[�N�ڑ��������ؒf����
	pProfile->m_bOutputForceDisconnect = ((CButton*)GetDlgItem(IDC_FORCE_DISCONNECT_CHECK))->GetCheck() & BST_CHECKED;

	//�v���r���[�֘A
	int nPeviewVMR = 1;
	int bPreviewVMROverlay = FALSE;
	if (((CButton*)this->GetDlgItem(IDC_VMR_AUTO_RADIO))->GetCheck() & BST_CHECKED) {
		nPeviewVMR = 1;
		bPreviewVMROverlay = TRUE;
	} else if (((CButton*)this->GetDlgItem(IDC_VMR_0_RADIO))->GetCheck() & BST_CHECKED) {
		nPeviewVMR = 0;
	} else if (((CButton*)this->GetDlgItem(IDC_VMR_7_OFF_RADIO))->GetCheck() & BST_CHECKED) {
		nPeviewVMR = 7;
	} else if (((CButton*)this->GetDlgItem(IDC_VMR_7_ON_RADIO))->GetCheck() & BST_CHECKED) {
		nPeviewVMR = 7;
		bPreviewVMROverlay = TRUE;
	} else if (((CButton*)this->GetDlgItem(IDC_VMR_9_RADIO))->GetCheck() & BST_CHECKED) {
		nPeviewVMR = 9;
	}
	pSetting->m_nPreviewVMR        = nPeviewVMR;
	pSetting->m_bPreviewVMROverlay = bPreviewVMROverlay;
	//���S�Ƀv���r���[���~����
	pSetting->m_bPreviewForceOff = ((CButton*)GetDlgItem(IDC_FORCE_PREVIEW_OFF_CHECK))->GetCheck() & BST_CHECKED;

	//�L�[�t���[���̊Ԋu
	button1 = (CButton*)this->GetDlgItem(IDC_KEYFRAME_AUTO_RADIO); assert(button1);
	if (button1->GetCheck() & BST_CHECKED)
		pProfile->m_bMaxKeyFrameSpacing = TRUE;
	else 
		pProfile->m_bMaxKeyFrameSpacing = FALSE;

	edit = (CEdit*)GetDlgItem(IDC_KEYFRAME_EDIT); assert(edit);
	CString strNum;
	edit->GetWindowText(strNum);
	pProfile->m_nMaxKeyFrameSpacing = _tstoi(strNum)*1000;
	//�r�f�I�p�t�H�[�}���X
	button1 = (CButton*)this->GetDlgItem(IDC_COMPLEXITY_AUTO_RADIO); assert(button1);
	if (button1->GetCheck() & BST_CHECKED)
		pProfile->m_bVideoComplexity = TRUE;
	else 
		pProfile->m_bVideoComplexity = FALSE;
	CSliderCtrl* slider = (CSliderCtrl*)GetDlgItem(IDC_COMPLEXITY_SLIDER); assert(slider);
	pProfile->m_nVideoComplexity = slider->GetPos();
}

//////////////////////////////////////////////////
//�v���C���[�̃p�X��ύX����Q�ƃ{�^��
//////////////////////////////////////////////////
void CSettingDialog::OnBnClickedVideoPlayerButton()
{
	//�r�f�I�v���C���[�̃p�X
	CEdit* edit = (CEdit*)GetDlgItem(IDC_VIDEO_PLAYER_EDIT); assert(edit);
	CString strPlayerPath;
	edit->GetWindowText(strPlayerPath);

	CFileDialog fd(TRUE,
		_T(""),
		_T(""),
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST ,
		_T("Program File (*.exe)|*.exe|")
		_T("All files (*.*)|*.*|\0"),
		this);
	//�p�X�̕���
	TCHAR szDrive	[_MAX_DRIVE];
	TCHAR szDir		[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt		[_MAX_EXT];
	_tsplitpath_s(strPlayerPath,szDrive,_MAX_DRIVE,szDir,_MAX_DIR,szFileName,_MAX_FNAME,szExt,_MAX_EXT);
	CString strPlayerFolder;
	strPlayerFolder.Format(_T("%s%s"),szDrive,szDir);
	//�����f�B���N�g����ݒ�
	fd.m_ofn.lpstrInitialDir = strPlayerFolder;

	if(fd.DoModal() == IDOK) {
		edit->SetWindowText(fd.GetPathName());
	}
}
////////////////////////////////////////
//�v���r���[�����S�ɒ�~����
////////////////////////////////////////
void CSettingDialog::OnBnClickedForcePreviewOffCheck()
{
	BOOL bEnable = ((CButton*)GetDlgItem(IDC_FORCE_PREVIEW_OFF_CHECK))->GetCheck() & BST_CHECKED;
	bEnable = !bEnable;
	GetDlgItem(IDC_VMR_AUTO_RADIO)->EnableWindow(bEnable);
	GetDlgItem(IDC_VMR_0_RADIO)->EnableWindow(bEnable);
	GetDlgItem(IDC_VMR_7_OFF_RADIO)->EnableWindow(bEnable);
	GetDlgItem(IDC_VMR_7_ON_RADIO)->EnableWindow(bEnable);
	GetDlgItem(IDC_VMR_9_RADIO)->EnableWindow(bEnable);
}
////////////////////////////////////////
//�L�[�t���[���̊Ԋu
////////////////////////////////////////
void CSettingDialog::CheckKeyFrameButton(BOOL bEnable)
{
	GetDlgItem(IDC_KEYFRAME_EDIT)->EnableWindow(bEnable);
	GetDlgItem(IDC_KEYFRAME_SPIN)->EnableWindow(bEnable);
	return;
}
////////////////////////////////////////
//�r�f�I�̕��G��
////////////////////////////////////////
void CSettingDialog::CheckComplexityButton(BOOL bEnable)
{
	GetDlgItem(IDC_COMPLEXITY_SLIDER)->EnableWindow(bEnable);
	return;
}

////////////////////////////////////////
//�c�[���`�b�v�̕\��
////////////////////////////////////////
BOOL CSettingDialog::PreTranslateMessage(MSG* pMsg)
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
//�X�N���[���o�[�̍X�V
////////////////////////////////////////
void CSettingDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CSliderCtrl* slider = (CSliderCtrl*)this->GetDlgItem(IDC_COMPLEXITY_SLIDER); assert(slider);
	if (pScrollBar->GetSafeHwnd() == slider->GetSafeHwnd())
	{
		return;
	}
	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}

