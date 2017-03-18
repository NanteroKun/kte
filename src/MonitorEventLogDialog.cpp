// MonitorEventLogDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MonitorEventLogDialog.h"


// CMonitorEventLogDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CMonitorEventLogDialog, CDialog)

CMonitorEventLogDialog::CMonitorEventLogDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorEventLogDialog::IDD, pParent)
{
	m_bEventLogFile = FALSE;
	m_strLogEvent = _T("");
}

CMonitorEventLogDialog::~CMonitorEventLogDialog()
{
}

void CMonitorEventLogDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorEventLogDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_EVENT_LOG_CLEAR_BUTTON, &CMonitorEventLogDialog::OnBnClickedEventLogClearButton)
	ON_BN_CLICKED(IDC_EVENT_LOG_SAVE_BUTTON, &CMonitorEventLogDialog::OnBnClickedEventLogSaveButton)
	ON_BN_CLICKED(IDC_AUTO_SAVE_CHECK, &CMonitorEventLogDialog::OnBnClickedAutoSaveCheck)
	ON_BN_CLICKED(IDC_AUTO_SAVE_BUTTON, &CMonitorEventLogDialog::OnBnClickedAutoSaveButton)
END_MESSAGE_MAP()


////////////////////////////////////////
//�_�C�A���O������
////////////////////////////////////////
BOOL CMonitorEventLogDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	m_strLogEvent = _T("");

	CButton* button = (CButton*)this->GetDlgItem(IDC_AUTO_SAVE_CHECK);
	assert(button);
	if (pSetting->m_bEventLogAutoSave)
		button->SetCheck(BST_CHECKED);
	else
		button->SetCheck(BST_UNCHECKED);
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_AUTO_SAVE_EDIT);
	assert(edit);
	edit->SetWindowText(pSetting->m_strEventLogAutoSavePath);

	if (pSetting->m_bEventLogAutoSave && !pSetting->m_strEventLogAutoSavePath.IsEmpty()) {
		if (!OpenEventLogFile(pSetting->m_strEventLogAutoSavePath))
		{
			button->SetCheck(BST_UNCHECKED);
		}
	}

	CString strAddLog;
	strAddLog.Format(_T("%s(%d)���N�����܂���"),pSetting->m_strProgramName,pSetting->m_nVersion);
	AddEventLog(strAddLog);

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

//////////////////////////////////////////////////
//�_�C�A���O���I������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorEventLogDialog::OnDestroy()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CString strAddLog;
	strAddLog.Format(_T("%s(%d)���I�����܂���\n"),pSetting->m_strProgramName,pSetting->m_nVersion);
	AddEventLog(strAddLog);

	CButton* button = (CButton*)this->GetDlgItem(IDC_AUTO_SAVE_CHECK);
	assert(button);
	if (button->GetCheck() & BST_CHECKED) {
		pSetting->m_bEventLogAutoSave = TRUE;
	} else {
		pSetting->m_bEventLogAutoSave = FALSE;
	}
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_AUTO_SAVE_EDIT);
	assert(edit);
	edit->GetWindowText(pSetting->m_strEventLogAutoSavePath);

	CloseEventLogFile();

	CDialog::OnDestroy();
}

//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX
//////////////////////////////////////////////////
void CMonitorEventLogDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CEdit* edit = (CEdit*)GetDlgItem(IDC_EVENT_LOG_EDIT);
	if (!edit) return;

	CRect rcClient;
	this->GetClientRect(&rcClient);
	CRect rcEditWindow;
	edit->GetWindowRect(&rcEditWindow);
	this->ScreenToClient(&rcEditWindow);
	CRect rcNewList;
	rcNewList.left = rcEditWindow.left;
	rcNewList.top = rcEditWindow.top;
	rcNewList.right = rcClient.right - rcEditWindow.left;//  - rcListWindow.left;
	rcNewList.bottom = rcClient.bottom - rcEditWindow.left;

	edit->MoveWindow(&rcNewList,TRUE);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}
//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CMonitorEventLogDialog::Encode(BOOL bStart)
{
	if (bStart) {
		AddEventLog(_T("�G���R�[�h���J�n���܂���"));
	} else {
		AddEventLog(_T("�G���R�[�h���I�����܂���"));
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�C�x���g���O�̒ǉ�
//////////////////////////////////////////////////
BOOL CMonitorEventLogDialog::AddEventLog(const CString &strEventLog)
{
	CTime time;
	time = CTime::GetCurrentTime();
	CString strAddString;
	strAddString.Format(_T("%s : %s"),time.Format(_T("%Y/%m/%d %H:%M:%S")),strEventLog);

	m_strLogEvent += strAddString;
	m_strLogEvent += _T("\r\n");
	CEdit* edit = (CEdit*)GetDlgItem(IDC_EVENT_LOG_EDIT);
	if (!edit) return FALSE;
	edit->SetWindowText(m_strLogEvent);

	//���O�̒ǋL��������
	if (m_bEventLogFile) {
		m_fEventLogFile.WriteString(strAddString);
		m_fEventLogFile.WriteString(_T("\n"));
		m_fEventLogFile.Flush();
	}

	return TRUE;
}
//////////////////////////////////////////////////
//�C�x���g���O�̃N���A
//////////////////////////////////////////////////
void CMonitorEventLogDialog::OnBnClickedEventLogClearButton()
{
	m_strLogEvent = _T("");
	CEdit* edit = (CEdit*)GetDlgItem(IDC_EVENT_LOG_EDIT);
	if (!edit) return;
	edit->SetWindowText(m_strLogEvent);
}
//////////////////////////////////////////////////
//�t�@�C���ɕۑ�
//////////////////////////////////////////////////
void CMonitorEventLogDialog::OnBnClickedEventLogSaveButton()
{
   CFileDialog fd(FALSE,
		_T(""),
		_T(""),
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT ,
		_T("Text File (*.txt)|*.txt|")
		_T("Data File (*.dat)|*.dat|")
		_T("All files (*.*)|*.*|\0"),
		this);
	if(fd.DoModal() != IDOK)
		return;

	CStdioFile stdFile;
	if (stdFile.Open(fd.GetPathName(),CFile::modeWrite | CFile::modeCreate | CFile::typeBinary/*| CFile::typeText*/) == 0)
		return;
	stdFile.SeekToBegin();
	BYTE byBOM[2];
	byBOM[0] = 0xFF;
	byBOM[1] = 0xFE;
	stdFile.Write(byBOM,2);
	stdFile.WriteString(m_strLogEvent);
	stdFile.Close();
}
//////////////////////////////////////////////////
//�����ۑ��Ƀ`�F�b�N
//////////////////////////////////////////////////
void CMonitorEventLogDialog::OnBnClickedAutoSaveCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_AUTO_SAVE_CHECK); assert(button);
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_AUTO_SAVE_EDIT); assert(edit);
	CString strAutoSavePath;
	edit->GetWindowText(strAutoSavePath);

	if (button->GetCheck() & BST_CHECKED) {
		//����->�L����
		if (strAutoSavePath.IsEmpty()) {
			button->SetCheck(BST_UNCHECKED);
			return;
		}
		if (!OpenEventLogFile(strAutoSavePath))
			button->SetCheck(BST_UNCHECKED);
	} else {
		//�L��->������
		CloseEventLogFile();
	}
}
//////////////////////////////////////////////////
//�Q�ƃ{�^��
//////////////////////////////////////////////////
void CMonitorEventLogDialog::OnBnClickedAutoSaveButton()
{
	//�����ۑ��̃p�X
	CEdit* edit = (CEdit*)GetDlgItem(IDC_AUTO_SAVE_EDIT);
	assert(edit);
	CString strPath;
	edit->GetWindowText(strPath);

	CFileDialog fd(FALSE,
		_T(""),
		_T(""),
		OFN_NOCHANGEDIR | OFN_HIDEREADONLY | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT ,
		_T("Text File (*.txt)|*.txt|")
		_T("Data File (*.dat)|*.dat|")
		_T("All files (*.*)|*.*|\0"),
		this);
	//�p�X�̕���
	TCHAR szDrive	[_MAX_DRIVE];
	TCHAR szDir		[_MAX_DIR];
	TCHAR szFileName[_MAX_FNAME];
	TCHAR szExt		[_MAX_EXT];
	_tsplitpath_s(strPath,szDrive,_MAX_DRIVE,szDir,_MAX_DIR,szFileName,_MAX_FNAME,szExt,_MAX_EXT);
	CString strFolder;
	strFolder.Format(_T("%s%s"),szDrive,szDir);
	//�����f�B���N�g����ݒ�
	fd.m_ofn.lpstrInitialDir = strFolder;

	if(fd.DoModal() != IDOK)
		return;

	edit->SetWindowText(fd.GetPathName());

	//�`�F�b�N���L���Ńp�X�̕ύX�������ꍇ
	CButton* button = (CButton*)this->GetDlgItem(IDC_AUTO_SAVE_CHECK); assert(button);
	if (button->GetCheck() & BST_CHECKED) {
		CloseEventLogFile();
		if (!OpenEventLogFile(fd.GetPathName()))
		{
			button->SetCheck(BST_UNCHECKED);
		}
	}
}

//////////////////////////////////////////////////
BOOL CMonitorEventLogDialog::OpenEventLogFile(CString &strFileName)
{
	if (strFileName.IsEmpty())
		return FALSE;
	if (m_bEventLogFile)	//���Ƀt�@�C�����J���Ă���ꍇ
		CloseEventLogFile();

	CFileFind find;
	if (find.FindFile(strFileName)) {
		if (m_fEventLogFile.Open(strFileName,CFile::modeWrite | CFile::modeNoTruncate | CFile::typeText | CFile::shareDenyWrite) == 0)
			return FALSE;
		m_fEventLogFile.SeekToEnd();
	} else {
		if (m_fEventLogFile.Open(strFileName,CFile::modeWrite | CFile::modeCreate | CFile::typeText | CFile::shareDenyWrite) == 0)
			return FALSE;
	}
	m_bEventLogFile = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////
void CMonitorEventLogDialog::CloseEventLogFile()
{
	if (!m_bEventLogFile)
		return;
	m_fEventLogFile.Close();
	m_bEventLogFile = FALSE;
}
