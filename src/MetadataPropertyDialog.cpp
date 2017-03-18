// MetadataPropertyDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MetadataPropertyDialog.h"


// CMetadataPropertyDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CMetadataPropertyDialog, CSizeDialog)

BEGIN_MESSAGE_MAP(CMetadataPropertyDialog, CSizeDialog)
	ON_WM_DESTROY()
	//ON_WM_SIZE()
	//ON_WM_VSCROLL()
END_MESSAGE_MAP()


CMetadataPropertyDialog::CMetadataPropertyDialog(CWnd* pParent /*=NULL*/)
	: CSizeDialog(CMetadataPropertyDialog::IDD, pParent)
{

}

CMetadataPropertyDialog::~CMetadataPropertyDialog()
{
}

void CMetadataPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}




// CMetadataPropertyDialog ���b�Z�[�W �n���h��

//////////////////////////////////////////////////
//�_�C�A���O������
//////////////////////////////////////////////////
BOOL CMetadataPropertyDialog::OnInitDialog()
{
	CSizeDialog::OnInitDialog();

	//�v���t�@�C���̓ǂݍ��݂Ɣ��f
	LoadProfileSetting();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
//////////////////////////////////////////////////
//�_�C�A���O�I����
//////////////////////////////////////////////////
void CMetadataPropertyDialog::OnDestroy()
{
	//�v���t�@�C���ւ̕ۑ�
	SaveProfileSetting();
	CDialog::OnDestroy();
}

//////////////////////////////////////////////////
//�c�[���`�b�v�̕\��
//////////////////////////////////////////////////
BOOL CMetadataPropertyDialog::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL CMetadataPropertyDialog::ChangeProfileNotify()
{
	//�v���t�@�C���̃��[�h
	LoadProfileSetting();
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C������̃��[�h
//////////////////////////////////////////////////
BOOL CMetadataPropertyDialog::LoadProfileSetting()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	CEdit* edit = (CEdit*)GetDlgItem(IDC_METADATA_TITLE_EDIT); assert(edit);
	edit->SetWindowText(pProfile->m_strMetadataTitle);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_AUTHOR_EDIT); assert(edit);
	edit->SetWindowText(pProfile->m_strMetadataAuthor);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_COPYRIGHT_EDIT); assert(edit);
	edit->SetWindowText(pProfile->m_strMetadataCopyright);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_RATING_EDIT); assert(edit);
	edit->SetWindowText(pProfile->m_strMetadataRating);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_DESCRIPTION_EDIT); assert(edit);
	edit->SetWindowText(pProfile->m_strMetadataDescription);
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���ւ̃Z�[�u
//////////////////////////////////////////////////
BOOL CMetadataPropertyDialog::SaveProfileSetting()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	CEdit* edit = (CEdit*)GetDlgItem(IDC_METADATA_TITLE_EDIT); assert(edit);
	edit->GetWindowText(pProfile->m_strMetadataTitle);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_AUTHOR_EDIT); assert(edit);
	edit->GetWindowText(pProfile->m_strMetadataAuthor);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_COPYRIGHT_EDIT); assert(edit);
	edit->GetWindowText(pProfile->m_strMetadataCopyright);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_RATING_EDIT); assert(edit);
	edit->GetWindowText(pProfile->m_strMetadataRating);
	edit = (CEdit*)GetDlgItem(IDC_METADATA_DESCRIPTION_EDIT); assert(edit);
	edit->GetWindowText(pProfile->m_strMetadataDescription);
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL CMetadataPropertyDialog::Encode(BOOL bStart)
{
	if (bStart) {
		CString strMetadataTitle;
		CString strMetadataAuthor;
		CString strMetadataCopyright;
		CString strMetadataRating;
		CString strMetadataDescription;
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		CEdit* edit = (CEdit*)GetDlgItem(IDC_METADATA_TITLE_EDIT); assert(edit);
		edit->GetWindowText(strMetadataTitle);
		edit = (CEdit*)GetDlgItem(IDC_METADATA_AUTHOR_EDIT); assert(edit);
		edit->GetWindowText(strMetadataAuthor);
		edit = (CEdit*)GetDlgItem(IDC_METADATA_COPYRIGHT_EDIT); assert(edit);
		edit->GetWindowText(strMetadataCopyright);
		edit = (CEdit*)GetDlgItem(IDC_METADATA_RATING_EDIT); assert(edit);
		edit->GetWindowText(strMetadataRating);
		edit = (CEdit*)GetDlgItem(IDC_METADATA_DESCRIPTION_EDIT); assert(edit);
		edit->GetWindowText(strMetadataDescription);

		pDSA->SetMetadata(strMetadataTitle,strMetadataAuthor,
			strMetadataDescription,strMetadataRating,strMetadataCopyright);
	}
	//�R�����R���g���[���̖���
	((CWnd*)GetDlgItem(IDC_METADATA_TITLE_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_METADATA_AUTHOR_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_METADATA_COPYRIGHT_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_METADATA_RATING_EDIT))->EnableWindow(bStart ? FALSE : TRUE);
	((CWnd*)GetDlgItem(IDC_METADATA_DESCRIPTION_EDIT))->EnableWindow(bStart ? FALSE : TRUE);

	return TRUE;
}