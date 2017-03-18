// AboutDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "AboutDialog.h"


// CAboutDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CAboutDialog, CDialog)

//�R���X�g���N�^
CAboutDialog::CAboutDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAboutDialog::IDD, pParent)
{
}
//�f�X�g���N�^
CAboutDialog::~CAboutDialog()
{
}
//DDX/DDV
void CAboutDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//���b�Z�[�W�}�b�v�J�n
BEGIN_MESSAGE_MAP(CAboutDialog, CDialog)
	ON_BN_CLICKED(IDOK, &CAboutDialog::OnBnClickedOk)
END_MESSAGE_MAP()
//���b�Z�[�W�}�b�v�I��


void CAboutDialog::OnBnClickedOk()
{
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	OnOK();
}

BOOL CAboutDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CStatic* text = (CStatic*)this->GetDlgItem(IDC_VERSION_STATIC);
	assert(text);

	CString strVersion(_T(""));
	//text->GetWindowText(strVersion);
	strVersion.Format(_T("%s(KTE)  Version %d"),pSetting->m_strProgramName,pSetting->m_nVersion);
	text->SetWindowText(strVersion);

	HICON hIcon = ::LoadIcon (AfxGetResourceHandle(), MAKEINTRESOURCE(IDI_BLUE_ON_ICON));
	((CStatic*)GetDlgItem(IDC_PICTURE_STATIC))->SetIcon(hIcon);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
