#include "StdAfx.h"
#include "Setting.h"

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CSetting::CSetting(void)
{
	DefaultSetting();
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CSetting::~CSetting(void)
{
}

//////////////////////////////////////////////////
//�ݒ荀�ڂɏ����l�ɐݒ�
//////////////////////////////////////////////////
void CSetting::DefaultSetting()
{
	//�S��
	m_bEnableIniFile = TRUE;
	m_bSaveIniFile = TRUE;
	m_strProgramName = _T("KoToEncoder");
	//m_strProgramName = _T("KoToEncoder[TEST]");
	m_nVersion = 20170319;

	//�����v���t�@�C���̃p�X��ݒ�
	DefaultProfilePath();

	//��ʐ݌v
	m_rcMainWindow.SetRectEmpty();
	m_nMainSplitterWidth[0] = 0;
	m_nMainSplitterWidth[1] = 0;
	m_nMainSplitterHeight[0] = 0;
	m_nMainSplitterHeight[1] = 0;
	m_nMainSplitterPreview[0] = 0;
	m_nMainSplitterPreview[1] = 0;
	m_bShowToolBar = TRUE;
	m_bShowStatusBar = TRUE;
	m_bShowPropertyWindow = TRUE;
	m_bShowMonitorWindow = TRUE;
	m_bShowPreviewWindow = TRUE;
	m_bShowVolumeWindow = TRUE;
	m_nPropertyTabIndex = 0;
	m_nMonitorTabIndex = 0;
#ifdef _DEBUG
	m_bShowDebugWindow = TRUE;
#else
	m_bShowDebugWindow = FALSE;
#endif
	//���ݒ�
	m_bMiniTaskBar = FALSE;
	m_bCreateMutex = TRUE;
	m_bShowTopMost = FALSE;
	m_strVideoPlayerPath = _T("");
	m_strGetGlobalIPURL = _T("");
	m_bGetGlobalIPCheck = TRUE;
	m_strGlobalIPAddress = _T("");
	m_strGlobalHostName = _T("");
	m_bEnumAllPlugin = FALSE;

	//�v���r���[�E�B���h�E�̐ݒ�
	m_dPreviewChildTimes = 100;
	m_bFixByWindow = FALSE;
	m_bFixByClient = FALSE;
	m_bPreviewShow = TRUE;
	m_bPreviewForceOff = FALSE;
	m_nPreviewVMR = 0;
	m_bPreviewVMROverlay = FALSE;

	m_bEnableAudioPreview = TRUE;

	//���j�^�[�E�B���h�E(���v�^�u)�̐ݒ�
	m_nMonitorUpdateTimer = 1000;
	//���j�^�[�E�B���h�E(�u���[�h�L���X�g�^�u)�̐ݒ�
	m_nMonitorPullColumnWidthList.clear();
	//�C�x���g���O�̐ݒ�
	m_bEventLogAutoSave = FALSE;
	m_strEventLogAutoSavePath = _T("");
}

//////////////////////////////////////////////////
//�ݒ�f�[�^�̓ǂݍ���
//////////////////////////////////////////////////
BOOL CSetting::LoadSetting()
{
	CIniFile Ini;
	if (!Ini.Open(SETTING_INI_FILE))
	{
		AfxMessageBox(_T("Ini�t�@�C�����J���̂Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		Ini.Close();
		return FALSE;
	}

	//INI�t�@�C���̑��݂̊m�F
	m_bEnableIniFile = Ini.IsFind();

	//�v���t�@�C��
	m_strProfileShortPath = Ini.GetProfileString(_T("Profile"),_T("CurrentShortPath"),_T("Default.kte"));
	m_strProfileFullPath = Ini.GetProfileString(_T("Profile"),_T("CurrentLongPath"),_T(""));
	if (m_strProfileFullPath.IsEmpty() == TRUE || m_strProfileShortPath.IsEmpty() == TRUE) {
		DefaultProfilePath();	//�����v���t�@�C���ɖ߂�
	}

	//��ʐ݌v
	m_rcMainWindow.left = Ini.GetProfileInt(_T("MainWindow"),_T("Left"),0);
	m_rcMainWindow.top  = Ini.GetProfileInt(_T("MainWindow"),_T("Top"),0);
	m_rcMainWindow.right = Ini.GetProfileInt(_T("MainWindow"),_T("Right"),0);
	m_rcMainWindow.bottom = Ini.GetProfileInt(_T("MainWindow"),_T("Bottom"),0);

	m_nMainSplitterWidth[0] = Ini.GetProfileInt(_T("MainWindow"),_T("SplitterWidth0"),0);
	m_nMainSplitterWidth[1] = Ini.GetProfileInt(_T("MainWindow"),_T("SplitterWidth1"),0);
	m_nMainSplitterHeight[0] = Ini.GetProfileInt(_T("MainWindow"),_T("SplitterHeight0"),0);
	m_nMainSplitterHeight[1] = Ini.GetProfileInt(_T("MainWindow"),_T("SplitterHeight1"),0);
	m_nMainSplitterPreview[0] = Ini.GetProfileInt(_T("MainWindow"),_T("SplitterPreview0"),0);
	m_nMainSplitterPreview[1] = Ini.GetProfileInt(_T("MainWindow"),_T("SplitterPreview1"),0);

	m_bShowToolBar = Ini.GetProfileBOOL(_T("Show"),_T("ToolBar"),TRUE);
	m_bShowStatusBar = Ini.GetProfileBOOL(_T("Show"),_T("StatusBar"),TRUE);
	m_bShowPropertyWindow = Ini.GetProfileBOOL(_T("Show"),_T("PropertyWindow"),TRUE);
	m_bShowMonitorWindow = Ini.GetProfileBOOL(_T("Show"),_T("MonitorWindow"),TRUE);
	m_bShowPreviewWindow = Ini.GetProfileBOOL(_T("Show"),_T("PreviewWindow"),TRUE);
	m_bShowVolumeWindow = Ini.GetProfileBOOL(_T("Show"),_T("VolumeWindow"),TRUE);

	m_nPropertyTabIndex = Ini.GetProfileInt(_T("Show"),_T("PropertyTabIndex"),0);
	m_nMonitorTabIndex = Ini.GetProfileInt(_T("Show"),_T("MonitorTabIndex"),0);
#ifdef _DEBUG
	m_bShowDebugWindow = Ini.GetProfileBOOL(_T("Show"),_T("DebugWindow"),TRUE);
#else
	m_bShowDebugWindow = Ini.GetProfileBOOL(_T("Show"),_T("DebugWindow"),FALSE);
#endif

	//���ݒ�
	m_bMiniTaskBar = Ini.GetProfileBOOL(_T("Environment"),_T("MiniTaskBar"),FALSE);
	m_bCreateMutex = Ini.GetProfileBOOL(_T("Environment"),_T("CreateMutex"),TRUE);
	m_bShowTopMost = Ini.GetProfileBOOL(_T("Environment"),_T("ShowTopMost"),FALSE);
	m_strVideoPlayerPath = Ini.GetProfileString(_T("Environment"),_T("VideoPlayerPath"),_T(""));
	if (!m_bEnableIniFile)	//�������f�B�A�v���C���[�̃p�X��ݒ肷��
	{
		TCHAR SpecialPath[MAX_PATH];
		SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_PROGRAM_FILES,FALSE);
		m_strVideoPlayerPath.Format(_T("%s\\Windows Media Player\\wmplayer.exe"),SpecialPath);
		CFileFind file;
		if (!file.FindFile(m_strVideoPlayerPath))
		{
			SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_PROGRAM_FILESX86,FALSE);
			m_strVideoPlayerPath.Format(_T("%s\\Windows Media Player\\wmplayer.exe"),SpecialPath);
			if (!file.FindFile(m_strVideoPlayerPath))
				m_strVideoPlayerPath = _T("");
		}
	}
	m_strGetGlobalIPURL = Ini.GetProfileString(_T("Environment"),_T("GlobalIPURL"),_T("http://kte.cavelis.net/cgi-bin/global.php"));
	m_bGetGlobalIPCheck = Ini.GetProfileBOOL(_T("Environment"),_T("GlobalIPCheck"),TRUE);
	m_strGlobalIPAddress = _T("");
	m_strGlobalHostName = _T("");
	m_bEnumAllPlugin = Ini.GetProfileBOOL(_T("Environment"),_T("EnumAllPlugin"),FALSE);

	//�v���r���[�E�B���h�E�̐ݒ�
	m_dPreviewChildTimes = Ini.GetProfileDouble(_T("Preview"),_T("ChildTimes"),100.0);
	m_bFixByWindow = Ini.GetProfileBOOL(_T("Preview"),_T("FixByWindow"),FALSE);
	m_bFixByClient = Ini.GetProfileBOOL(_T("Preview"),_T("FixByClient"),FALSE);
	if (m_bFixByWindow && m_bFixByClient) {
		m_dPreviewChildTimes = 100.0;
		m_bFixByWindow = FALSE;
		m_bFixByClient = FALSE;
	}
	m_bPreviewShow = Ini.GetProfileBOOL(_T("Preview"),_T("ChildShow"),TRUE);
	m_bPreviewForceOff = Ini.GetProfileBOOL(_T("Preview"),_T("ForceOff"),FALSE);
	m_nPreviewVMR = Ini.GetProfileInt(_T("Preview"),_T("VMR"),0);
	m_bPreviewVMROverlay = Ini.GetProfileBOOL(_T("Preview"),_T("Overlay"),FALSE);

	m_bEnableAudioPreview = Ini.GetProfileBOOL(_T("AudioPreview"),_T("Enable"),TRUE);

	//���j�^�[�E�B���h�E(���v�^�u)�̐ݒ�
	m_nMonitorUpdateTimer = Ini.GetProfileInt(_T("MonitorWindow"),_T("UpdateTimer"),1000);
	//���j�^�[�E�B���h�E(�u���[�h�L���X�g�^�u)�̐ݒ�
	CString strEntry;
	const int nMonitorPullColumnCount = 6;
	for (int i=0;i<nMonitorPullColumnCount;i++)
	{
		strEntry.Format(_T("PullColumnWidth%d"),i+1);
		int nWidth = Ini.GetProfileInt(_T("MonitorWindow"),strEntry,100);
		m_nMonitorPullColumnWidthList.push_back(nWidth);
	}
	//�C�x���g���O�̐ݒ�
	m_bEventLogAutoSave = Ini.GetProfileBOOL(_T("MonitorWindow"),_T("EventAutoSave"),FALSE);
	m_strEventLogAutoSavePath = Ini.GetProfileString(_T("MonitorWindow"),_T("EventAutoSavePath"),_T(""));

	//INI�ҏW�I��
	Ini.Close();
	return TRUE;
}

//////////////////////////////////////////////////
//�ݒ�f�[�^�̏�������
//////////////////////////////////////////////////
BOOL CSetting::SaveSetting()
{
	CIniFile Ini;
	if (!Ini.Open(SETTING_INI_FILE))
	{
		AfxMessageBox(_T("Ini�t�@�C�����J���̂Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		Ini.Close();
		return FALSE;
	}

	//�v���t�@�C��
	Ini.WriteProfileString(_T("Profile"),_T("CurrentShortPath"),m_strProfileShortPath);
	Ini.WriteProfileString(_T("Profile"),_T("CurrentLongPath"),m_strProfileFullPath);

	//��ʐ݌v
	Ini.WriteProfileInt(_T("MainWindow"),_T("Left"),m_rcMainWindow.left);
	Ini.WriteProfileInt(_T("MainWindow"),_T("Top"),m_rcMainWindow.top);
	Ini.WriteProfileInt(_T("MainWindow"),_T("Right"),m_rcMainWindow.right);
	Ini.WriteProfileInt(_T("MainWindow"),_T("Bottom"),m_rcMainWindow.bottom);

	Ini.WriteProfileInt(_T("MainWindow"),_T("SplitterWidth0"),m_nMainSplitterWidth[0]);
	Ini.WriteProfileInt(_T("MainWindow"),_T("SplitterWidth1"),m_nMainSplitterWidth[1]);
	Ini.WriteProfileInt(_T("MainWindow"),_T("SplitterHeight0"),m_nMainSplitterHeight[0]);
	Ini.WriteProfileInt(_T("MainWindow"),_T("SplitterHeight1"),m_nMainSplitterHeight[1]);
	Ini.WriteProfileInt(_T("MainWindow"),_T("SplitterPreview0"),m_nMainSplitterPreview[0]);
	Ini.WriteProfileInt(_T("MainWindow"),_T("SplitterPreview1"),m_nMainSplitterPreview[1]);

	Ini.WriteProfileBOOL(_T("Show"),_T("ToolBar"),m_bShowToolBar);
	Ini.WriteProfileBOOL(_T("Show"),_T("StatusBar"),m_bShowStatusBar);
	Ini.WriteProfileBOOL(_T("Show"),_T("PropertyWindow"),m_bShowPropertyWindow);
	Ini.WriteProfileBOOL(_T("Show"),_T("MonitorWindow"),m_bShowMonitorWindow);
	Ini.WriteProfileBOOL(_T("Show"),_T("PreviewWindow"),m_bShowPreviewWindow);
	Ini.WriteProfileBOOL(_T("Show"),_T("VolumeWindow"),m_bShowVolumeWindow);
	Ini.WriteProfileInt(_T("Show"),_T("PropertyTabIndex"),m_nPropertyTabIndex);
	Ini.WriteProfileInt(_T("Show"),_T("MonitorTabIndex"),m_nMonitorTabIndex);
	Ini.WriteProfileBOOL(_T("Show"),_T("DebugWindow"),m_bShowDebugWindow);

	//���ݒ�
	Ini.WriteProfileBOOL(_T("Environment"),_T("MiniTaskBar"),m_bMiniTaskBar);
	Ini.WriteProfileBOOL(_T("Environment"),_T("CreateMutex"),m_bCreateMutex);
	Ini.WriteProfileBOOL(_T("Environment"),_T("ShowTopMost"),m_bShowTopMost);
	Ini.WriteProfileString(_T("Environment"),_T("VideoPlayerPath"),m_strVideoPlayerPath);
	Ini.WriteProfileString(_T("Environment"),_T("GlobalIPURL"),m_strGetGlobalIPURL);
	Ini.WriteProfileBOOL(_T("Environment"),_T("GlobalIPCheck"),m_bGetGlobalIPCheck);
	Ini.WriteProfileBOOL(_T("Environment"),_T("EnumAllPlugin"),m_bEnumAllPlugin);

	//�v���r���[�E�B���h�E�̐ݒ�
	Ini.WriteProfileDouble(_T("Preview"),_T("ChildTimes"),(float)m_dPreviewChildTimes);
	Ini.WriteProfileBOOL(_T("Preview"),_T("FixByWindow"),m_bFixByWindow);
	Ini.WriteProfileBOOL(_T("Preview"),_T("FixByClient"),m_bFixByClient);
	Ini.WriteProfileBOOL(_T("Preview"),_T("ChildShow"),m_bPreviewShow);
	Ini.WriteProfileBOOL(_T("Preview"),_T("ForceOff"),m_bPreviewForceOff);
	Ini.WriteProfileInt(_T("Preview"),_T("VMR"),m_nPreviewVMR);
	Ini.WriteProfileBOOL(_T("Preview"),_T("Overlay"),m_bPreviewVMROverlay);

	Ini.WriteProfileBOOL(_T("AudioPreview"),_T("Enable"),m_bEnableAudioPreview);

	//���j�^�[�E�B���h�E(���v�^�u)�̐ݒ�
	Ini.WriteProfileInt(_T("MonitorWindow"),_T("UpdateTimer"),m_nMonitorUpdateTimer);
	//���j�^�[�E�B���h�E(�u���[�h�L���X�g�^�u)�̐ݒ�
	CString strEntry;
	for (int i=0;i<(int)m_nMonitorPullColumnWidthList.size();i++)
	{
		strEntry.Format(_T("PullColumnWidth%d"),i+1);
		Ini.WriteProfileInt(_T("MonitorWindow"),strEntry,m_nMonitorPullColumnWidthList[i]);
	}
	//�C�x���g���O�̐ݒ�
	Ini.WriteProfileBOOL(_T("MonitorWindow"),_T("EventAutoSave"),m_bEventLogAutoSave);
	Ini.WriteProfileString(_T("MonitorWindow"),_T("EventAutoSavePath"),m_strEventLogAutoSavePath);

	//INI�ҏW�I��
	Ini.Close();
	return TRUE;
}

//////////////////////////////////////////////////
//�v���t�@�C���̃p�X���K��̃p�X("Default.kte")�ɂ���
//////////////////////////////////////////////////
void CSetting::DefaultProfilePath()
{
	m_strProfileShortPath = _T("Default.kte");
	TCHAR szNewPath	[_MAX_PATH];
	TCHAR szPath	[_MAX_PATH];
	TCHAR szDrive	[_MAX_DRIVE];
	TCHAR szDir		[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt		[_MAX_EXT];
	TCHAR szShortPath [_MAX_PATH];

	::GetModuleFileName(NULL,szPath,_MAX_PATH);
	_tsplitpath_s(szPath,szDrive,_MAX_DRIVE,szDir,_MAX_DIR,szFileName,_MAX_FNAME,szExt,_MAX_EXT);
	_tcscpy_s(szShortPath, m_strProfileShortPath);
	_stprintf_s(szNewPath,_MAX_PATH,_T("%s%s%s"),szDrive,szDir, szShortPath);
	m_strProfileFullPath = szNewPath;
	return;
}

//////////////////////////////////////////////////
//�v���t�@�C���̃p�X��ύX����
//LPCTSTR lpszFileName �t���p�X
//////////////////////////////////////////////////
BOOL CSetting::SetProfilePath(LPCTSTR lpszFileName)
{
	if (lpszFileName == NULL)
	{
		m_strProfileShortPath = _T("");
		m_strProfileFullPath = _T("");
		return FALSE;
	}
	TCHAR szDrive	[_MAX_DRIVE];
	TCHAR szDir		[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt		[_MAX_EXT];
	_tsplitpath_s(lpszFileName,szDrive,_MAX_DRIVE,szDir,_MAX_DIR,szFileName,_MAX_FNAME,szExt,_MAX_EXT);
	m_strProfileShortPath.Format(_T("%s%s"),szFileName,szExt);
	m_strProfileFullPath = lpszFileName;
	return TRUE;
}
