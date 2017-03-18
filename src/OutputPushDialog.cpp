// OutputPushDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "OutputPushDialog.h"
#include "OutputPropertyDialog.h"

// COutputPushDialog �_�C�A���O

IMPLEMENT_DYNAMIC(COutputPushDialog, CDialog)

COutputPushDialog::COutputPushDialog(CWnd* pParent /*=NULL*/)
	: COutputBaseDialog(COutputPushDialog::IDD, pParent)
{
	m_nRegisterID = -1;
	m_strRegisterServerName = _T("");
	m_bRegisterAutoDelete = FALSE;
}

COutputPushDialog::~COutputPushDialog()
{
}

void COutputPushDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COutputPushDialog, CDialog)
	ON_BN_CLICKED(IDC_OUTPUT_PUSH_ENABLE_CHECK, &COutputPushDialog::OnBnClickedOutputPushEnableCheck)
	ON_BN_CLICKED(IDC_OUTPUT_PUSH_AUTODELETE_CHECK, &COutputPushDialog::OnBnClickedOutputPushAutodeleteCheck)
	ON_BN_CLICKED(IDC_OUTPUT_APPLY_BUTTON, &COutputPushDialog::OnBnClickedOutputApplyButton)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OUTPUT_PLAYER_BUTTON, &COutputPushDialog::OnBnClickedOutputPlayerButton)
END_MESSAGE_MAP()

//////////////////////////////////////////////////
//�o�͖��̎擾
//////////////////////////////////////////////////
BOOL COutputPushDialog::GetOutputName(CString &strFileName)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PUSH_PATH_EDIT);
	if (edit == NULL)
		return FALSE;
	edit->GetWindowText(strFileName);
	return TRUE;
}
//////////////////////////////////////////////////
//�o�͖��̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPushDialog::SetOutputName(CString strFileName)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PUSH_PATH_EDIT);
	if (edit == NULL)
		return FALSE;
	edit->SetWindowText(strFileName);
	return TRUE;
}
//////////////////////////////////////////////////
//�^�u���̎擾
//////////////////////////////////////////////////
BOOL COutputPushDialog::GetTabName(CString &strTabName)
{
	CWnd* wnd = (CWnd*)this->GetDlgItem(IDC_OUTPUT_PUSH_GROUP);
	if (wnd == NULL)
		return FALSE;
	wnd->GetWindowText(strTabName);
	return TRUE;
}
//////////////////////////////////////////////////
//�^�u���̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPushDialog::SetTabName(CString strTabName)
{
	CWnd* wnd = (CWnd*)this->GetDlgItem(IDC_OUTPUT_PUSH_GROUP);
	if (wnd == NULL)
		return FALSE;
	wnd->SetWindowText(strTabName);
	return TRUE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�̎擾
//////////////////////////////////////////////////
BOOL COutputPushDialog::GetEnableCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PUSH_ENABLE_CHECK); 
	assert(button);
	if (button->GetCheck() & BST_CHECKED)
		return TRUE;
	return FALSE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPushDialog::SetEnableCheck(BOOL bCheck)
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PUSH_ENABLE_CHECK); 
	assert(button);
	if (bCheck)
		button->SetCheck(BST_CHECKED);
	else
		button->SetCheck(BST_UNCHECKED);
	return TRUE;
}
//////////////////////////////////////////////////
//�����폜�̎擾
//////////////////////////////////////////////////
BOOL COutputPushDialog::GetAutoDeleteCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PUSH_AUTODELETE_CHECK); 
	assert(button);
	if (button->GetCheck() & BST_CHECKED)
		return TRUE;
	return FALSE;
}
//////////////////////////////////////////////////
//�����폜�̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPushDialog::SetAutoDeleteCheck(BOOL bCheck)
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PUSH_AUTODELETE_CHECK); 
	assert(button);
	if (bCheck)
		button->SetCheck(BST_CHECKED);
	else
		button->SetCheck(BST_UNCHECKED);
	return TRUE;
}
//////////////////////////////////////////////////
//�K�p�{�^���̕\���Ɣ�\��
//////////////////////////////////////////////////
BOOL COutputPushDialog::ShowApplyButton(BOOL bShow)
{
	CWnd* button = (CWnd*)GetDlgItem(IDC_OUTPUT_APPLY_BUTTON);
	assert(button);
	button->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	CWnd* staticlabel = (CWnd*)GetDlgItem(IDC_OUTPUT_APPLY_STATIC);
	assert(staticlabel);
	staticlabel->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	button = (CWnd*)GetDlgItem(IDC_OUTPUT_PLAYER_BUTTON);
	assert(button);
	button->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	return TRUE;
}
//////////////////////////////////////////////////
//�L���{�^���̃`�F�b�N
//////////////////////////////////////////////////
void COutputPushDialog::OnBnClickedOutputPushEnableCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PUSH_ENABLE_CHECK); 
	assert(button);
	//�e�E�B���h�E�ɒʒm����
	CWnd* pWnd = this->GetParent();
	pWnd->SendMessage(NOTIFY_ENABLE_CHECK,(WPARAM)this,button->GetCheck());
}
//////////////////////////////////////////////////
//�����I�ɍ폜�̃`�F�b�N
//////////////////////////////////////////////////
void COutputPushDialog::OnBnClickedOutputPushAutodeleteCheck()
{
}
//////////////////////////////////////////////////
//�c�[���`�b�v�̕\��
//////////////////////////////////////////////////
BOOL COutputPushDialog::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE: 
			m_ToolTip.RelayEvent(pMsg);
			break;
	}
	return COutputBaseDialog::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////
//�_�C�A���O�̏�����
//////////////////////////////////////////////////
BOOL COutputPushDialog::OnInitDialog()
{
	COutputBaseDialog::OnInitDialog();

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PUSH_ENABLE_CHECK),IDC_OUTPUT_PUSH_ENABLE_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PUSH_PATH_EDIT),IDC_OUTPUT_PUSH_PATH_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PUSH_AUTODELETE_CHECK),IDC_OUTPUT_PUSH_AUTODELETE_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_APPLY_BUTTON),IDC_OUTPUT_APPLY_BUTTON);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PLAYER_BUTTON),IDC_OUTPUT_PLAYER_BUTTON);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	m_nRegisterID = -1;
	m_strRegisterServerName = _T("");
	m_bRegisterAutoDelete = FALSE;

	//�K�p�{�^��
	CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
	assert(pDSA);
	if (pDSA->IsEncode())
	{
		ShowApplyButton(TRUE);
		SetEnableCheck(FALSE);
		OnBnClickedOutputPushEnableCheck();
	} else {
		ShowApplyButton(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
//////////////////////////////////////////////////
//�_�C�A���O�̏I��
//////////////////////////////////////////////////
void COutputPushDialog::OnDestroy()
{
	//�G���R�[�h���ꍇ�͏o�͂��폜����
	if (m_nRegisterID != -1)
	{
		Encode(FALSE);
	}
	COutputBaseDialog::OnDestroy();
}

//////////////////////////////////////////////////
//�T�[�o�[���̕�����ϊ�
//����Ȃ�������⊮����
//http://ServerName:PortNumer/PublishingPoint
//////////////////////////////////////////////////
BOOL COutputPushDialog::ConvertServerName(CString strOldName,CString &strNewName)
{
	strOldName.Trim();	//�O�̂��߂̗]���̃g����
	int nIndex = strOldName.Find(_T("http://"));
	if (nIndex == 0) {
		strOldName = strOldName.Right( strOldName.GetLength() - 7);
	}
	nIndex = strOldName.Find(_T("ttp://"));
	if (nIndex == 0) {
		strOldName = strOldName.Right( strOldName.GetLength() - 6);
	}
	nIndex = strOldName.Find(_T("mms://"));
	if (nIndex == 0) {
		strOldName = strOldName.Right( strOldName.GetLength() - 6);
	}
	CString strServerName = _T("");
	CString strPortName = _T("8080");
	CString strPublishName = _T("");
	int nIndexColon = strOldName.Find(_T(':'));
	int nIndexSlash = strOldName.Find(_T('/'));
	if (nIndexColon == -1 && nIndexSlash == -1) {
		strServerName = strOldName;
	} else {
		if (nIndexColon == -1) {
			strServerName = strOldName.Left(nIndexSlash);
			strPublishName = strOldName.Mid(nIndexSlash + 1);
		} else {
			strServerName = strOldName.Left(nIndexColon);
			if (nIndexSlash == -1)
				strPortName = strOldName.Mid(nIndexColon + 1);
			else {
				strPortName = strOldName.Mid(nIndexColon + 1,nIndexSlash-nIndexColon - 1);
				strPublishName = strOldName.Mid(nIndexSlash + 1);
			}
		}
	}
	if (strPortName.IsEmpty() || strPortName.Compare(_T("")) == 0)
		strPortName = _T("8080");
	strNewName.Format(_T("http://%s:%s/%s"),strServerName,strPortName,strPublishName);
	return TRUE;
}

//////////////////////////////////////////////////
//�v���C���[�ŊJ���{�^��
//////////////////////////////////////////////////
void COutputPushDialog::OnBnClickedOutputPlayerButton()
{
	CString strServerName;
	this->GetOutputName(strServerName);
	if (!strServerName.IsEmpty())
	{
		CString strConvertServerName = _T("");
		ConvertServerName(m_strRegisterServerName,strConvertServerName);
		if (!strConvertServerName.IsEmpty())
		{
			CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
			if (!pSetting->m_strVideoPlayerPath.IsEmpty())
			{
				//�R�}���h���C���Ń��f�B�A�v���C���[���N������
				::ShellExecute(NULL,NULL,pSetting->m_strVideoPlayerPath,strConvertServerName,NULL,SW_SHOWNORMAL);
			}
		}
	}
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL COutputPushDialog::Encode(BOOL bStart)
{
	ShowApplyButton(bStart);

	if (bStart)
	{
		if (!GetEnableCheck())
			return TRUE;
		CString strServerName = _T("");
		this->GetOutputName(strServerName);
		if (strServerName.Compare(_T("")) == 0 || strServerName.IsEmpty()) {
			SetEnableCheck(FALSE);
			OnBnClickedOutputPushEnableCheck();
			return TRUE;
		}

		CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
		CString strConvertServerName = _T("");
		ConvertServerName(strServerName,strConvertServerName);
		BOOL bAlreadyInsert = FALSE;
		for (int i = 0;i<(int)pDSA->m_PushSinkList.size();i++)
		{
			if (pDSA->m_PushSinkList[i].strServerName.Compare(strConvertServerName) == 0)
			{
				AfxMessageBox(_T("�T�[�o�[�ڑ��悪�d�����Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				SetEnableCheck(FALSE);
				OnBnClickedOutputPushEnableCheck();
				return TRUE;	//�d�����x�ł̓G���[�ɂ͂�Ȃ�
			}
		}
		m_nRegisterID = -1;
		m_strRegisterServerName = strServerName;
		//ConvertServerName(m_strRegisterServerName,strConvertServerName);	//�T�[�o�[���̐���
		m_bRegisterAutoDelete = this->GetAutoDeleteCheck();
		return pDSA->AddPushSink(strConvertServerName,m_bRegisterAutoDelete,m_nRegisterID);
	} else {
		if (m_nRegisterID != -1) {
			CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
			assert(pDSA);
			pDSA->RemovePushSink(m_nRegisterID);
		}
		m_nRegisterID = -1;
		m_strRegisterServerName = _T("");
		m_bRegisterAutoDelete = FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�K�p�{�^��
//////////////////////////////////////////////////
void COutputPushDialog::OnBnClickedOutputApplyButton()
{
	//�L���{�^���̃`�F�b�N�̗L��
	BOOL bEnableCheck = this->GetEnableCheck();

	//�G���R�[�h�J�n�������̎�
	if (m_nRegisterID == -1) {
		if (bEnableCheck) {
			Encode(TRUE);
		} else {
			return;
		}
	}
	else{	//�G���R�[�h�J�n���L���̎�
		if (bEnableCheck) {
			//�T�[�o�[���̕ύX������ꍇ�ɂ́A�G���R�[�h�̍ċN��
			CString strServerName;
			this->GetOutputName(strServerName);
			if (strServerName.Compare(m_strRegisterServerName) != 0) {
				Encode(FALSE);
				Encode(TRUE);
			}
		} else {
			Encode(FALSE);	//�G���R�[�h�̒�~
			//�K�p�{�^�������͗L���ɂ��Ƃ�
			ShowApplyButton(TRUE);
		}
	}
}

/*
�K�p�{�^���̃V�i���I
�G���R�[�h�̊J�n(����->�L��)
�G���R�[�h�̖���(����->����) ���ω�����
�G���R�[�h�̕ύX(�L��->�L��)
	�T�[�o�[���݂̂̕ύX	�����Ē���
	�����폜�݂̂̕ύX		���ύX�Ȃ�
	�����̕ύX				�����Ē���
�G���R�[�h�̒�~(�L��->����)
*/
