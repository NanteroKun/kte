// OutputFileDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "OutputFileDialog.h"
#include "OutputPropertyDialog.h"

// COutputFileDialog �_�C�A���O

IMPLEMENT_DYNAMIC(COutputFileDialog, CDialog)

BEGIN_MESSAGE_MAP(COutputFileDialog, CDialog)
	ON_BN_CLICKED(IDC_OUTPUT_FILE_BUTTON, &COutputFileDialog::OnBnClickedOutputFileButton)
	ON_BN_CLICKED(IDC_OUTPUT_FILE_ENABLE_CHECK, &COutputFileDialog::OnBnClickedOutputFileEnableCheck)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OUTPUT_APPLY_BUTTON, &COutputFileDialog::OnBnClickedOutputApplyButton)
	ON_BN_CLICKED(IDC_OUTPUT_OPEN_BUTTON, &COutputFileDialog::OnBnClickedOutputOpenButton)
END_MESSAGE_MAP()

COutputFileDialog::COutputFileDialog(CWnd* pParent /*=NULL*/)
	: COutputBaseDialog(COutputFileDialog::IDD, pParent)
{
	m_nRegisterID = -1;
	m_strRegisterFileName = _T("");
	m_RegisterTime = 0;
}

COutputFileDialog::~COutputFileDialog()
{
}

void COutputFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// COutputFileDialog ���b�Z�[�W �n���h��

//////////////////////////////////////////////////
//�o�͖��̎擾
//////////////////////////////////////////////////
BOOL COutputFileDialog::GetOutputName(CString &strFileName)
{
	TRACE0("COutputFileDialog::GetOutputName");
	/*
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FILE_PATH_EDIT);
	if (edit == NULL)
		return FALSE;
	edit->GetWindowText(strFileName);
	*/
	return TRUE;
}
//////////////////////////////////////////////////
//�o�͖��̐ݒ�
//////////////////////////////////////////////////
BOOL COutputFileDialog::SetOutputName(CString strFileName)
{
	TRACE0("COutputFileDialog::SetOutputName");
	/*
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FILE_PATH_EDIT);
	if (edit == NULL)
		return FALSE;
	edit->SetWindowText(strFileName);
	*/
	return TRUE;
}
//////////////////////////////////////////////////
//�o�͐ݒ薼�̎擾�ƕύX
//////////////////////////////////////////////////
BOOL COutputFileDialog::GetOutputFileName(CString &strFileName)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FILE_PATH_EDIT);
	if (edit == NULL) return FALSE;
	edit->GetWindowText(strFileName);
	return TRUE;
}
BOOL COutputFileDialog::GetOutputFolderName(CString &strFolderName)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FOLDER_PATH_EDIT);
	if (edit == NULL) return FALSE;
	edit->GetWindowText(strFolderName);
	return TRUE;
}
BOOL COutputFileDialog::SetOutputFileName(CString strFileName)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FILE_PATH_EDIT);
	if (edit == NULL) return FALSE;
	edit->SetWindowText(strFileName);
	return TRUE;
}
BOOL COutputFileDialog::SetOutputFolderName(CString strFolderName)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FOLDER_PATH_EDIT);
	if (edit == NULL) return FALSE;
	edit->SetWindowText(strFolderName);
	return TRUE;
}

//////////////////////////////////////////////////
//�^�u���̎擾
//////////////////////////////////////////////////
BOOL COutputFileDialog::GetTabName(CString &strTabName)
{
	CWnd* wnd = (CWnd*)this->GetDlgItem(IDC_OUTPUT_FILE_GROUP);
	if (wnd == NULL)
		return FALSE;
	wnd->GetWindowText(strTabName);
	return TRUE;
}
//////////////////////////////////////////////////
//�^�u���̐ݒ�
//////////////////////////////////////////////////
BOOL COutputFileDialog::SetTabName(CString strTabName)
{
	CWnd* wnd = (CWnd*)this->GetDlgItem(IDC_OUTPUT_FILE_GROUP);
	if (wnd == NULL)
		return FALSE;
	wnd->SetWindowText(strTabName);
	return TRUE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�̎擾
//////////////////////////////////////////////////
BOOL COutputFileDialog::GetEnableCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_FILE_ENABLE_CHECK); 
	assert(button);
	if (button->GetCheck() & BST_CHECKED)
		return TRUE;
	return FALSE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�̐ݒ�
//////////////////////////////////////////////////
BOOL COutputFileDialog::SetEnableCheck(BOOL bCheck)
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_FILE_ENABLE_CHECK); 
	assert(button);
	if (bCheck)
		button->SetCheck(BST_CHECKED);
	else
		button->SetCheck(BST_UNCHECKED);
	return TRUE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�{�b�N�X�̃N���b�N
//////////////////////////////////////////////////
void COutputFileDialog::OnBnClickedOutputFileEnableCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_FILE_ENABLE_CHECK); 
	assert(button);
	//�e�E�B���h�E�ɒʒm����
	CWnd* pWnd = this->GetParent();
	pWnd->SendMessage(NOTIFY_ENABLE_CHECK,(WPARAM)this,button->GetCheck());
}
//////////////////////////////////////////////////
//�K�p�{�^���̕\���Ɣ�\��
//////////////////////////////////////////////////
BOOL COutputFileDialog::ShowApplyButton(BOOL bShow)
{
	CWnd* button = (CWnd*)GetDlgItem(IDC_OUTPUT_APPLY_BUTTON);
	CWnd* staticlabel = (CWnd*)GetDlgItem(IDC_OUTPUT_APPLY_STATIC);
	assert(button);
	assert(staticlabel);
	button->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	staticlabel->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	return TRUE;
}
//////////////////////////////////////////////////
//�Q�ƃ{�^��
//////////////////////////////////////////////////
void COutputFileDialog::OnBnClickedOutputFileButton()
{
	LPMALLOC pMalloc;
	if( ::SHGetMalloc( &pMalloc ) != NOERROR )
		return;

	//���[�g�f�B���N�g�����擾
	LPITEMIDLIST pRoot = NULL;
	if (::SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &pRoot) != TRUE)
		::SHGetSpecialFolderLocation(NULL,CSIDL_DESKTOP,&pRoot);

	//�����f�B���N�g�����擾
	TCHAR SpecialPath[MAX_PATH];
	if (SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_MYVIDEO,FALSE) != TRUE) {
		SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_DESKTOP,FALSE);
	}

	//�t�H���_���J���_�C�A���O��\��
	BROWSEINFO bInfo;
	::ZeroMemory(&bInfo,sizeof(BROWSEINFO));
	TCHAR path[MAX_PATH];
	LPITEMIDLIST pIDList = NULL;
	//�t�H���_�I���_�C�A���O�@����
	bInfo.hwndOwner             = this->GetSafeHwnd();		//�E�C���h�E�̃n���h��
	bInfo.pidlRoot              = pRoot;						//�f�t�H���g�p�X�iNULL�F�f�X�N�g�b�v)
	bInfo.pszDisplayName        = path;				//�I�����ꂽ�p�X�󂯎��p�o�b�t�@
	bInfo.lpszTitle             = _T("�o�̓t�H���_�̑I��");		//�_�C�A���O�ɕ\������^�C�g�� 
	bInfo.ulFlags               = BIF_RETURNONLYFSDIRS;
	bInfo.lpfn                  = BrowseCallbackProc;
	bInfo.lParam                = (LPARAM)SpecialPath;

	//�t�H���_�I���_�C�A���O�\��
	pIDList = ::SHBrowseForFolder(&bInfo);
	if (pIDList != NULL)
	{
		if(::SHGetPathFromIDList(pIDList, path)) {						//�p�X���펞
			CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FOLDER_PATH_EDIT);
			assert(edit);
			edit->SetWindowText(path);
		}
	}
	if( pRoot != NULL )
		pMalloc->Free( pRoot );
	if( pIDList != NULL )
		pMalloc->Free( pIDList );
	pMalloc->Release();
	return;
}
//////////////////////////////////////////////////
//�J���{�^��
//////////////////////////////////////////////////
void COutputFileDialog::OnBnClickedOutputOpenButton()
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_FOLDER_PATH_EDIT);
	CString strPath;
	edit->GetWindowText(strPath);
	if (strPath.IsEmpty())
		return;
	ShellExecute(HWND_DESKTOP,_T("open"),_T("explorer.exe"),strPath,NULL,SW_SHOW);
}

////////////////////////////////////////////////////
//�t�H���_���J���_�C�A���O�̃R�[���o�b�N
////////////////////////////////////////////////////
int CALLBACK COutputFileDialog::BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
  if (uMsg == BFFM_INITIALIZED)
    ::SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
  return 0;
}

//////////////////////////////////////////////////
//�o�̓t�@�C�����̍쐬
//strFolderPath �t�H���_��
//strFilePath �t�@�C����
//Time �쐬���鎞��
//strMakePath �o�̓t�@�C����
//////////////////////////////////////////////////
BOOL COutputFileDialog::MakeFilePath(CString strFolderPath,CString strFilePath,CTime &Time,CString &strMakePath)
{
	//�t�H���_������Ȃ�f�X�N�g�b�v���w�肷��
	if (strFolderPath.IsEmpty()) {
		//�����f�B���N�g�����擾
		TCHAR SpecialPath[MAX_PATH];
		SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_DESKTOP,FALSE);
		strFolderPath = SpecialPath;
	}

	//�t�H���_�p�X�̐ݒ�
	strMakePath = strFolderPath;
	if ((strFolderPath.Right(1)).Compare(_T("\\")) != 0)
		strMakePath += _T("\\");

	//FilePath������̒u������
	LPCTSTR pPtr = strFilePath;
	CString strAppend;
	CString strFormat;
	while (*pPtr != _T('\0'))
	{
		if (*pPtr == _T('%'))	//�ϊ�����
		{
			strFormat = _T("%");
			pPtr = _tcsinc(pPtr);
			if (*pPtr == _T('\0'))
				break;
			if (*pPtr == _T('#')) {
				strFormat.Append(pPtr,1);
				pPtr = _tcsinc(pPtr);
			}
			strFormat.Append(pPtr,1);
			switch (*pPtr)
			{
			case _T('Y'):	//10�i���ŕ\��4���̐���
			case _T('y'):	//10�i���ŕ\������̉�2��(00�`99)
			case _T('B'):	//���̐�����
			case _T('b'):	//���̏ȗ��`
			case _T('m'):	//10�i���ŕ\����(01�`12)
			case _T('A'):	//�j���̐�����
			case _T('a'):	//�j���̏ȗ��`
			case _T('d'):	//10�i���ŕ\�����̓��t(01�`31)
			case _T('H'):	//24���ԕ\�L�̎���(00�`23)
			case _T('I'):	//12���ԕ\�L�̎���(00�`12)
			case _T('j'):	//10�i���ŕ\���N������̓��t(000�`366)
			case _T('M'):	//10�i���ŕ\����(00�`59)
			case _T('S'):	//10�i���ŕ\���b(00�`59)
			case _T('c'):	//���P�[���ɑΉ�������t�Ǝ����̕\��
			case _T('p'):	//���݂̃��P�[��̌ߑO/�ߌ�
			case _T('U'):	//10�i���ŕ\���T�̒ʂ��ԍ��B���j�����T�̍ŏ��̓��Ƃ���(00�`53)
			case _T('w'):	//10�i���ŕ\���j��(0�`6�A���j����0)
			case _T('W'):	//10�i���ŕ\���T�̒ʂ��ԍ��B���j�����T�̍ŏ��̓��Ƃ���(00�`53)
			case _T('x'):	//���݂̃��P�[���̓��t�\��
			case _T('X'):	//���݂̃��P�[��̎����\��
			case _T('z'):	//���W�X�g���̐ݒ�ɉ����āA�^�C���]�[���̖��O�܂��͏ȗ��`���w�肵�܂��B�^�C���]�[�����s���ȏꍇ�͎w�肵�܂���
			case _T('Z'):	//���W�X�g���̐ݒ�ɉ����āA�^�C���]�[���̖��O�܂��͏ȗ��`���w�肵�܂��B�^�C���]�[�����s���ȏꍇ�͎w�肵�܂���
				strAppend = Time.Format(strFormat);
				break;
			case _T('%'):
				strAppend = _T('%');
			default:
				strAppend = _T('%');
				strAppend.Append(pPtr,1);
			}
			pPtr = _tcsinc(pPtr);
			strMakePath.Append(strAppend);
		} else {	//���̑��̕���
			strMakePath.Append(pPtr,1);
			pPtr = _tcsinc(pPtr);
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL COutputFileDialog::Encode(BOOL bStart)
{
	ShowApplyButton(bStart);

	if (bStart)
	{
		if (!this->GetEnableCheck())
			return TRUE;
		CString strFileName;
		this->GetOutputFileName(strFileName);
		CString strFolderName;
		this->GetOutputFolderName(strFolderName);
		if (strFileName.IsEmpty() || strFileName.Compare(_T("")) == 0 || strFolderName.IsEmpty() || strFolderName.Compare(_T("")) == 0) {
			SetEnableCheck(FALSE);
			OnBnClickedOutputFileEnableCheck();
			return TRUE;
		}

		CString strMakePath;
		CTime time = CTime::GetCurrentTime();
		this->MakeFilePath(strFolderName,strFileName,time,strMakePath);
		if (strMakePath.IsEmpty() || strMakePath.Compare(_T("")) == 0) {
			SetEnableCheck(FALSE);
			OnBnClickedOutputFileEnableCheck();
			return TRUE;
		}

		//�����Ńt�@�C����(strMakePath)�̏d�����`�F�b�N����
		CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
		for (int i=0;i<(int)pDSA->m_FileSinkList.size();i++)
		{
			if (pDSA->m_FileSinkList[i].strFileName.Compare(strMakePath) == 0)
			{
				AfxMessageBox(_T("�t�@�C�������d�����Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				SetEnableCheck(FALSE);
				OnBnClickedOutputFileEnableCheck();
				return TRUE;	//�d�����x�ł̓G���[�ɂ͂�Ȃ�
			}
		}
		m_nRegisterID = -1;
		m_strRegisterFileName = strMakePath;
		m_RegisterTime = time;
		return pDSA->AddFileSink(m_strRegisterFileName,m_nRegisterID);
	} else {
		if (m_nRegisterID != -1) {
			CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
			assert(pDSA);
			pDSA->RemoveFileSink(m_nRegisterID);
		}
		m_nRegisterID = -1;
		m_strRegisterFileName = _T("");
		m_RegisterTime = 0;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�c�[���`�b�v�̕\��
//////////////////////////////////////////////////
BOOL COutputFileDialog::PreTranslateMessage(MSG* pMsg)
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
//�_�C�A���O����������
//////////////////////////////////////////////////
BOOL COutputFileDialog::OnInitDialog()
{
	COutputBaseDialog::OnInitDialog();

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_FILE_ENABLE_CHECK),IDC_OUTPUT_FILE_ENABLE_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_FILE_PATH_EDIT),IDC_OUTPUT_FILE_PATH_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_FOLDER_PATH_EDIT),IDC_OUTPUT_FOLDER_PATH_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_FILE_BUTTON),IDC_OUTPUT_FILE_BUTTON);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_APPLY_BUTTON),IDC_OUTPUT_APPLY_BUTTON);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	m_nRegisterID = -1;
	m_strRegisterFileName = _T("");
	m_RegisterTime = 0;


	//�G���R�[�h���������ꍇ
	CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsEncode())
	{
		ShowApplyButton(TRUE);
		SetEnableCheck(FALSE);
		OnBnClickedOutputFileEnableCheck();
	} else {
		ShowApplyButton(FALSE);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

//////////////////////////////////////////////////
//�_�C�A���O�I������
//////////////////////////////////////////////////
void COutputFileDialog::OnDestroy()
{
	//�G���R�[�h���ꍇ�͏o�͂��폜����
	if (m_nRegisterID != -1)
	{
		Encode(FALSE);
	}

	COutputBaseDialog::OnDestroy();
}
//////////////////////////////////////////////////
//�G���R�[�h���̐ݒ�ύX�̓K�p�{�^��
//////////////////////////////////////////////////
void COutputFileDialog::OnBnClickedOutputApplyButton()
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
			//�t�@�C�����̕ύX������ꍇ�ɂ́A�G���R�[�h�̍ċN��
			CString strFileName;
			CString strFolderName;
			CString strMakePath;
			GetDlgItem(IDC_OUTPUT_FILE_PATH_EDIT)->GetWindowText(strFileName);
			GetDlgItem(IDC_OUTPUT_FOLDER_PATH_EDIT)->GetWindowText(strFolderName);
			this->MakeFilePath(strFolderName,strFileName,this->m_RegisterTime,strMakePath);

			if (strMakePath.Compare(m_strRegisterFileName) != 0) {
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
	�t�@�C���������܂�
	�t�@�C�������Ⴄ
�G���R�[�h�̒�~(�L��->����)
*/

