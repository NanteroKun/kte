
// KTE.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "KTE.h"
#include "MainFrm.h"
#include "DirectShowAccess.h"
#include "AboutDialog.h"
#include "PluginDialog.h"
#include "SettingDialog.h"
#include "PerformanceSettingDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////
CKTEApp theApp;		// �B��� CKTEApp �I�u�W�F�N�g�ł��B
//////////////////////////////////////////////////

//���b�Z�[�W�}�b�v
BEGIN_MESSAGE_MAP(CKTEApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CKTEApp::OnAboutDialog)
	ON_COMMAND(ID_SETTING_DIALOG, &CKTEApp::OnSettingDialog)
	ON_COMMAND(ID_SETTING_DEFAULT, &CKTEApp::OnSettingDefault)
	ON_COMMAND(ID_PERFORMANCE_SETTING_DIALOG, &CKTEApp::OnPerformanceSettingDialog)
	ON_COMMAND(ID_PROFILE_NEW, &CKTEApp::OnProfileNew)
	ON_COMMAND(ID_PROFILE_OPEN, &CKTEApp::OnProfileOpen)
	ON_COMMAND(ID_PROFILE_SAVE, &CKTEApp::OnProfileSave)
	ON_COMMAND(ID_PROFILE_SAVEAS, &CKTEApp::OnProfileSaveas)
	ON_COMMAND(ID_PLUGIN_DIALOG, &CKTEApp::OnPluginDialog)
END_MESSAGE_MAP()

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CKTEApp::CKTEApp()
	:	m_pSetting(NULL),
		m_pProfile(NULL),
		m_pDSA(NULL),
		m_hMutexApp(NULL),
		m_bEnablePriority(FALSE)
{
	// TODO: ���̃A�v���P�[�V���� ID ���������ӂ� ID ������Œu�����܂��B���������
	// ������̌`���� CompanyName.ProductName.SubProduct.VersionInformation �ł�
	SetAppID(_T("KTE.AppID.NoVersion"));

	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CKTEApp::~CKTEApp()
{
}

//////////////////////////////////////////////////
//�A�v���P�[�V�����G���g���|�C���g
//////////////////////////////////////////////////
BOOL CKTEApp::InitInstance()
{
	//n�Ԗڂ̃��������[�N�����o
	//_CrtSetBreakAlloc(n);

	setlocale(LC_CTYPE, "jpn");

	// �A�v���P�[�V���� �}�j�t�F�X�g�� visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll Version 6 �ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �� InitCommonControlsEx() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// �A�v���P�[�V�����Ŏg�p���邷�ׂẴR���� �R���g���[�� �N���X���܂߂�ɂ́A
	// �����ݒ肵�܂��B
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit �R���g���[�����g�p����ɂ� AfxInitRichEdit2() ���K�v�ł�	
	// AfxInitRichEdit2();

	// �W��������
	// �����̋@�\���g�킸�ɍŏI�I�Ȏ��s�\�t�@�C����
	// �T�C�Y���k���������ꍇ�́A�ȉ�����s�v�ȏ�����
	// ���[�`�����폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ��Ж��܂��͑g�D���Ȃǂ̓K�؂ȕ������
	// ���̕������ύX���Ă��������B
	//SetRegistryKey(_T("�A�v���P�[�V���� �E�B�U�[�h�Ő������ꂽ���[�J�� �A�v���P�[�V����"));

	//�������`�F�b�N���āA�ċN�����̃v���Z�X���I������܂ő҂��Ă���
	if (!ArgCheck1())
		return FALSE;

	//OS�̃o�[�W�����`�F�b�N
	if (!OSVersionCheck())
		return FALSE;

	//�ݒ�̏������Ɠǂݍ���
	m_pSetting = new CSetting();
	assert(m_pSetting);
	if (!m_pSetting->LoadSetting())
		return FALSE;
	//�������`�F�b�N���āA(*.kte)�������Ɋ܂܂�Ă��邩�̊m�F
	ArgCheck2();

	//�v���t�@�C���̏������Ɠǂݍ���
	m_pProfile = new CProfile();
	assert(m_pProfile);
	if (!m_pProfile->LoadProfile(m_pSetting->m_strProfileFullPath,TRUE))
	{
		if (m_pSetting->m_strProfileShortPath.Compare(_T("Default.kte")) != 0)
		{
			//�v���t�@�C�������݂��Ȃ�
			CString strMessage;
			strMessage.Format(_T("�v���t�@�C�������݂��܂���ł����B\n%s\n��̃v���t�@�C����V�K�쐬���܂��B"),m_pSetting->m_strProfileFullPath);
			AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		}
		m_pProfile->DefaultProfile();
	}

	//�����N���}���p�̃~���[�e�b�N�X���쐬
	if (!SetMutex(m_pSetting->m_bCreateMutex))
		return FALSE;

	//CPU�ݒ��K�p����
	if (!SetAffinityMask(TRUE))
		return FALSE;

	//DirectShow������
	m_pDSA = new CDirectShowAccess;
	assert(m_pDSA);
	if (!m_pDSA->Init())
		return FALSE;
	
	// ���C�� �E�B���h�E���쐬����Ƃ��A���̃R�[�h�͐V�����t���[�� �E�B���h�E �I�u�W�F�N�g���쐬���A
	// ������A�v���P�[�V�����̃��C�� �E�B���h�E�ɃZ�b�g���܂�
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;

	// �t���[�������\�[�X���烍�[�h���č쐬���܂�
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	//��Ɏ�O�ɕ\���̓K�p
	if (!SetTopMost(m_pSetting->m_bShowTopMost))
		return FALSE;
	
	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	pFrame->ShowWindow(SW_SHOW);
	pFrame->UpdateWindow();
	// �ڔ��������݂���ꍇ�ɂ̂� DragAcceptFiles ���Ăяo���Ă��������B
	//  SDI �A�v���P�[�V�����ł́AProcessShellCommand �̒���ɂ��̌Ăяo�����������Ȃ���΂Ȃ�܂���B
	return TRUE;
}

//////////////////////////////////////////////////
//�A�v���P�[�V�����I���|�C���g
//////////////////////////////////////////////////
int CKTEApp::ExitInstance()
{
	//DirectShow�I��
	if (m_pDSA)
	{
		if (m_pDSA->IsEncode() == TRUE)
			m_pDSA->StopEncode(TRUE);
		if (m_pDSA->IsRunFilter())
			m_pDSA->StopFilter();
		m_pDSA->SaveProfileSetting();		//DirectShowAccess�̕ێ����Ă���f�[�^���v���t�@�C���̃Z�[�u
		m_pDSA->Exit();
		delete m_pDSA;
		m_pDSA = NULL;
	}

	//�����N���~���[�e�b�N�X�J��
	SetMutex(FALSE);

	//�v���t�@�C���̕ۑ��ƏI��
	if (m_pProfile)
	{
		if (m_pSetting->m_strProfileFullPath.IsEmpty() == FALSE)
			m_pProfile->SaveProfile(m_pSetting->m_strProfileFullPath);
		delete m_pProfile;
		m_pProfile = NULL;
	}

	//�ݒ�̕ۑ��ƏI��
	if (m_pSetting)
	{
		if (m_pSetting->m_bSaveIniFile)	//�Z�[�u���s��
			m_pSetting->SaveSetting();
		else {							//��Z�[�u���(�ݒ�̏�����)
			//�ݒ���v���t�@�C�����폜����
			CFileFind find;
			if (find.FindFile(SETTING_INI_FILE))
				DeleteFile(SETTING_INI_FILE);
			m_pSetting->DefaultProfilePath();
			if (find.FindFile(m_pSetting->m_strProfileFullPath))
				DeleteFile(m_pSetting->m_strProfileFullPath);
		}
		delete m_pSetting;
		m_pSetting = NULL;
	}

	//COM�̉���Ɏ��s���Ă���ꍇ�A�����Ōł܂�
	AfxOleTerm(FALSE);

	return CWinApp::ExitInstance();
}

//////////////////////////////////////////////////
//�o�[�W��������\������_�C�A���O��\��
//////////////////////////////////////////////////
void CKTEApp::OnAboutDialog()
{
	CAboutDialog dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////
//�v���O�C���ݒ��\������_�C�A���O��\��
//////////////////////////////////////////////////
void CKTEApp::OnPluginDialog()
{
	CPluginDialog dlg;
	dlg.DoModal();
}
//////////////////////////////////////////////////
//���ݒ��\������_�C�A���O��\��
//////////////////////////////////////////////////
void CKTEApp::OnSettingDialog()
{
	CSettingDialog dlg;
	INT_PTR nRet = dlg.DoModal();
	if (nRet != IDOK) {
		return;
	}
	//�����N���֎~�p�̃~���[�e�b�N�X�̓K�p
	SetMutex(m_pSetting->m_bCreateMutex);
	//��Ɏ�O�ɕ\���̓K�p
	SetTopMost(m_pSetting->m_bShowTopMost);
}

//////////////////////////////////////////////////
//�p�t�H�[�}���X�ݒ��\������_�C�A���O��\��
//////////////////////////////////////////////////
void CKTEApp::OnPerformanceSettingDialog()
{
	//�_�C�A���O��\��
	CPerformanceSettingDialog dlg;
	INT_PTR nRet = dlg.DoModal();
	if (nRet != IDOK)
		return;

	//�A�t�B�j�e�B�}�X�N�̓K�p
	SetAffinityMask(FALSE);

	//�G���R�[�h���ł������ꍇ�A�v���Z�X�v���C�I���e�B��ύX
	CDirectShowAccess* pDSA = GetDSA();
	if (pDSA->IsEncode() == TRUE)
		SetPriorityClass(m_pProfile->m_bProcessPriority,m_pProfile->m_nProcessPriority);
	return;
}

//////////////////////////////////////////////////
//���ݒ���f�t�H���g�ɖ߂�
//////////////////////////////////////////////////
void CKTEApp::OnSettingDefault()
{
	int ret_code;
	ret_code = AfxMessageBox(
		_T("�ݒ��S�ď�����Ԃɖ߂��܂����A��낵���ł����H\n(�v���O�������ċN�����܂�)"),
		MB_YESNO | MB_DEFBUTTON2 | MB_ICONINFORMATION);
	if (ret_code == IDYES)
	{
		//���g�̎��s�t�@�C�����v���Z�X�ŗ����グ��
		TCHAR strPath[MAX_PATH];
		::GetModuleFileName(AfxGetInstanceHandle(),strPath,sizeof(strPath)/sizeof(TCHAR));
		TCHAR strCurDir[MAX_PATH];
		::GetCurrentDirectory(sizeof(strCurDir)/sizeof(TCHAR),strCurDir);
		DWORD dwProcessID = ::GetCurrentProcessId();
		CString strCmdLine;
		strCmdLine.Format(_T("/waitterminate %u"),dwProcessID);
		//�v���O���������s(�ċN�������A�����`�F�b�N(/waitterminate)�Ō��݂̃v���Z�X���I������܂ő҂d�l�ɂȂ��Ă���)
		::ShellExecute(NULL,NULL,strPath,strCmdLine,strCurDir,SW_SHOWNORMAL);

		//�v���O�����I��
		m_pSetting->m_bSaveIniFile = FALSE;
		m_pMainWnd->SendMessage(WM_CLOSE);
	}
}

//////////////////////////////////////////////////
//OS�o�[�W�����`�F�b�N���s��
//////////////////////////////////////////////////
BOOL CKTEApp::OSVersionCheck()
{
	if (IsWindowsXPOrGreater() == false) {
		AfxMessageBox(_T("OS�̓���v���𖞂����Ă��Ȃ��̂ŏI�����܂�\n�{�v���O������Windows XP�ȏ�̃o�[�W�����œ��삵�܂�\n"));
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�����̃`�F�b�N(�ċN���̍ۂ̈����̃`�F�b�N)
//////////////////////////////////////////////////
BOOL CKTEApp::ArgCheck1()
{
	//�ċN���̍ۂ̃R�}���h
	BOOL bWaitTerminate = FALSE;
	DWORD dwWaitProcessID = 0;
	for (int i = 1;i<__argc;i++)
	{
		if (!(__wargv[i][0] == _T('/') || __wargv[i][0] == _T('-'))) {
			continue;
		}

		if (_tcsicmp(&__wargv[i][1],_T("waitterminate")) == 0)
		{
			if (++i >= __argc)
				continue;
			dwWaitProcessID = (DWORD)_tstoi(__wargv[i]);
			if (dwWaitProcessID != 0) {
				bWaitTerminate = TRUE;
			}
		}
	}
	if (bWaitTerminate)
	{
		HANDLE hWaitProcess = ::OpenProcess(SYNCHRONIZE, FALSE, dwWaitProcessID);
		if (hWaitProcess != NULL) {
			if (::WaitForSingleObject(hWaitProcess, 10 * 1000) == WAIT_TIMEOUT)
			{
				return FALSE;
			}
		}
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�����̃`�F�b�N ����2 �֘A�t�����ꂽ�v���t�@�C���̓ǂݍ���
//////////////////////////////////////////////////
BOOL CKTEApp::ArgCheck2()
{
	BOOL bFilePath = FALSE;
	if (__argc == 1)	//�ʏ�N��
		return FALSE;
	CFileFind find;
	if (find.FindFile(__wargv[1]) == TRUE)
	{
		m_pSetting->SetProfilePath(__wargv[1]);
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�����N���̐ݒ���s��
//////////////////////////////////////////////////
BOOL CKTEApp::SetMutex(BOOL bEnable)
{
#ifndef _DEBUG
	if (bEnable) {
		if (m_hMutexApp)
			return TRUE;	//����Mutex�����݂��Ă���
		m_hMutexApp = CreateMutex(NULL,TRUE,m_pSetting->m_strProgramName);
		if(GetLastError() == ERROR_ALREADY_EXISTS)
		{
			CString str;
			str.LoadString(IDS_ALREADY_EXISTS);
			AfxMessageBox(str,MB_ICONSTOP|MB_OK);
			return FALSE;
		}
	} else {
		if (m_hMutexApp)
		{
			ReleaseMutex(m_hMutexApp);
			CloseHandle(m_hMutexApp);
			m_hMutexApp = NULL;
		}
	}
#endif
	return TRUE;
}
//////////////////////////////////////////////////
//��Ɏ�O�ɕ\��
//////////////////////////////////////////////////
BOOL CKTEApp::SetTopMost(BOOL bEnable)
{
	CWnd* wnd = AfxGetApp()->GetMainWnd();
	if (!wnd)
		return FALSE;
	if (bEnable)
		wnd->SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	else
		wnd->SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	return TRUE;
}
//////////////////////////////////////////////////
//�v���Z�X�D��x�̐ݒ�
//���ꂪpublic�ɂȂ��Ă���̂́ApDSA����̃A�N�Z�X�����邩��
//////////////////////////////////////////////////
BOOL CKTEApp::SetPriorityClass(BOOL bEnable,DWORD dwPriorityClass)
{
	if (bEnable)
	{
		DWORD dwSetPriority = NORMAL_PRIORITY_CLASS;
		switch (dwPriorityClass)
		{
		case 0: dwSetPriority = IDLE_PRIORITY_CLASS; break;
		case 1: dwSetPriority = BELOW_NORMAL_PRIORITY_CLASS; break;
		case 2: dwSetPriority = NORMAL_PRIORITY_CLASS; break;
		case 3: dwSetPriority = ABOVE_NORMAL_PRIORITY_CLASS; break;
		case 4: dwSetPriority = HIGH_PRIORITY_CLASS; break;
		case 5: dwSetPriority = HIGH_PRIORITY_CLASS; break;
		}
		if (dwSetPriority != ::GetPriorityClass(GetCurrentProcess())) {
			::SetPriorityClass(GetCurrentProcess(),dwSetPriority);
			m_bEnablePriority = TRUE;
		}
	} else
	{
		if (m_bEnablePriority) {
			::SetPriorityClass(GetCurrentProcess(),NORMAL_PRIORITY_CLASS);
		}
		m_bEnablePriority = FALSE;
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�v���Z�X�̃A�t�B�j�e�B�}�X�N��ݒ肷��
//BOOL bInit = ����N�����̏ꍇ��TRUE,����ȊO��FALSE
//////////////////////////////////////////////////
BOOL CKTEApp::SetAffinityMask(BOOL bInit)
{
	if (bInit == TRUE && m_pProfile->m_bProcessLimitEnable == FALSE)
		return TRUE;

	//�e��V�X�e�����̎擾
	SYSTEM_INFO sys_info;
	ZeroMemory(&sys_info,sizeof(SYSTEM_INFO));
	GetSystemInfo(&sys_info);
	DWORD_PTR dwProcessAffinityMask;
	DWORD_PTR dwSystemAffinityMask;
	::GetProcessAffinityMask(GetCurrentProcess(),&dwProcessAffinityMask,&dwSystemAffinityMask);

	//�v���Z�X�̐����ƗD��v���Z�b�T�̎w��
	if (m_pProfile->m_bProcessAffinityPriority == TRUE) {	//�v���Z�X�̊��S�������[�h
		if (bInit == FALSE) {		//�v���Z�X�̗D��x�͌��ɖ߂����
			SetThreadIdealProcessor(GetCurrentThread(),(DWORD)MAXIMUM_PROCESSORS);}
		if (dwSystemAffinityMask >= m_pProfile->m_dwProcessAffinityMask &&
			m_pProfile->m_dwProcessAffinityMask != 0 && m_pProfile->m_dwProcessAffinityMask != 0xFFFFFFFF) {
				SetProcessAffinityMask(GetCurrentProcess(),m_pProfile->m_dwProcessAffinityMask);}
	} else {	//�D��v���Z�X�̐ݒ胂�[�h
		if (bInit == FALSE) {		//�A�t�B�j�e�B�}�X�N�͌��ɖ߂����
			SetProcessAffinityMask(GetCurrentProcess(),dwSystemAffinityMask);}
		if (sys_info.dwNumberOfProcessors < (DWORD)m_pProfile->m_nProcessIdealProcessor) {
			SetThreadIdealProcessor(GetCurrentThread(),(DWORD)m_pProfile->m_nProcessIdealProcessor);
		}
	}
	return TRUE;
}
/////////////////////////////////////////////////
//�v���t�@�C���̐V�K�쐬
//////////////////////////////////////////////////
void CKTEApp::OnProfileNew()
{
	int nRet = AfxMessageBox(_T("��̃v���t�@�C����V�K�쐬���܂��B\n��낵���ł����H"),MB_YESNO);
	if (nRet == IDNO)
		return;
	//�v���t�@�C���̏�����
	m_pProfile->DefaultProfile();
	//�p�X��������
	m_pSetting->DefaultProfilePath();
	//���̌�ɂ��ׂẴ_�C�A���O�ɒʒm���s��
	CMainFrame* pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->ChangeProfileNotify();
	//�^�C�g���o�[���ύX
	pFrame->ChangeTitleBar();

	//CPU�D��x�ɂ��Ă��ύX
	SetAffinityMask(FALSE);
}

//////////////////////////////////////////////////
//�v���t�@�C�����J��
//////////////////////////////////////////////////
void CKTEApp::OnProfileOpen()
{
	//�t�@�C�����J���_�C�A���O
	CFileDialog fd(TRUE,
		_T(""), _T(""),
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST  ,
		_T("KoToEncoder Profile File (*.kte)|*.kte|")
		_T("All files (*.*)|*.*|\0"),
		::AfxGetMainWnd());

	//�����f�B���N�g���̐ݒ�
	CString strDefaultFolder;
	if (m_pSetting->m_strProfileFullPath.IsEmpty() == FALSE)
	{
		//�p�X�̕���
		TCHAR szDrive	[_MAX_DRIVE];
		TCHAR szDir		[_MAX_DIR];
		TCHAR szFileName[_MAX_FNAME];
		TCHAR szExt		[_MAX_EXT];
		_tsplitpath_s(m_pSetting->m_strProfileFullPath,szDrive,_MAX_DRIVE,szDir,_MAX_DIR,szFileName,_MAX_FNAME,szExt,_MAX_EXT);
		strDefaultFolder.Format(_T("%s%s"),szDrive,szDir);
		fd.m_ofn.lpstrInitialDir = strDefaultFolder;
	}
	//�t�@�C�����J���_�C�A���O���N��
	if(fd.DoModal() != IDOK)
		return;
	if (fd.GetPathName().IsEmpty() == TRUE)
		return;

	//�v���t�@�C���̓ǂݍ���
	if (!m_pProfile->LoadProfile(fd.GetPathName(),FALSE)) {
		CString strMessage;
		strMessage.Format(_T("�v���t�@�C���̓ǂݍ��݂Ɏ��s���܂���\n%s\n��̃v���t�@�C����ǂݍ��݂܂�"),fd.GetPathName());
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		m_pProfile->DefaultProfile();		//��̃v���t�@�C��
	}
	//�v���t�@�C�����̐ݒ�
	m_pSetting->SetProfilePath(fd.GetPathName());

	//���̌�ɂ��ׂẴ_�C�A���O�ɒʒm���s��
	CMainFrame* pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->ChangeTitleBar();				//�^�C�g���o�[�ύX
	pFrame->ChangeProfileNotify();			//�v���t�@�C���ύX�̒ʒm

	//CPU�D��x�ɂ��Ă��ύX
	SetAffinityMask(FALSE);
}

//////////////////////////////////////////////////
//�v���t�@�C�����㏑���ۑ�
//////////////////////////////////////////////////
void CKTEApp::OnProfileSave()
{
	if (m_pSetting->m_strProfileFullPath.IsEmpty() == TRUE) {
		OnProfileSaveas();	//���O�����ĕۑ�
	} else {
		//�e�E�B���h�E���̏����v���t�@�C���ɔ��f
		CMainFrame* pFrame = (CMainFrame*)::AfxGetMainWnd();
		pFrame->SaveProfileSetting();
		//�v���t�@�C���̏㏑���ۑ�
		m_pProfile->SaveProfile(m_pSetting->m_strProfileFullPath);
	}
	return;
}
//////////////////////////////////////////////////
//�v���t�@�C���𖼑O�����ĕۑ�
//////////////////////////////////////////////////
void CKTEApp::OnProfileSaveas()
{
	CFileDialog fd(FALSE,
		_T(""), _T(""),
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT ,
		_T("KoToEncoder Profile File (*.kte)|*.kte|")
		_T("All files (*.*)|*.*|\0"),
		::AfxGetMainWnd());

	//�����f�B���N�g���̐ݒ�
	CString strDefaultFolder;
	if (m_pSetting->m_strProfileFullPath.IsEmpty() == FALSE)
	{
		//�p�X�̕���
		TCHAR szDrive	[_MAX_DRIVE];
		TCHAR szDir		[_MAX_DIR];
		TCHAR szFileName[_MAX_FNAME];
		TCHAR szExt		[_MAX_EXT];
		_tsplitpath_s(m_pSetting->m_strProfileFullPath,szDrive,_MAX_DRIVE,szDir,_MAX_DIR,szFileName,_MAX_FNAME,szExt,_MAX_EXT);
		strDefaultFolder.Format(_T("%s%s"),szDrive,szDir);
		fd.m_ofn.lpstrInitialDir = strDefaultFolder;
	}

	//�t�@�C���_�C�A���O���N��
	if(fd.DoModal() != IDOK)
		return;
	if (fd.GetPathName().IsEmpty() == TRUE)
		return;

	//�e�E�B���h�E�����v���t�@�C���ɔ��f
	CMainFrame* pFrame = (CMainFrame*)::AfxGetMainWnd();
	pFrame->SaveProfileSetting();
	//�v���t�@�C���̕ۑ�
	m_pProfile->SaveProfile(fd.GetPathName());
	//�f�t�H���g�v���t�@�C�����ύX
	m_pSetting->SetProfilePath(fd.GetPathName());
	//�^�C�g���o�[�ύX
	pFrame->ChangeTitleBar();
}

//////////////////////////////////////////////////
//[EOF]