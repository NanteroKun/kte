// OutputPullDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "OutputPullDialog.h"
#include "OutputPropertyDialog.h"



// COutputPullDialog �_�C�A���O

IMPLEMENT_DYNAMIC(COutputPullDialog, CDialog)

COutputPullDialog::COutputPullDialog(CWnd* pParent /*=NULL*/)
	: COutputBaseDialog(COutputPullDialog::IDD, pParent)
{
	m_bFirstFlag = TRUE;
	m_nRegisterID = -1;
	m_nRegisterPort = 0;
	m_nRegisterMax = 0;
}

COutputPullDialog::~COutputPullDialog()
{
}

void COutputPullDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(COutputPullDialog, CDialog)
	ON_BN_CLICKED(IDC_OUTPUT_PULL_ENABLE_CHECK, &COutputPullDialog::OnBnClickedOutputPullEnableCheck)
	ON_EN_CHANGE(IDC_OUTPUT_PULL_MAX_EDIT, &COutputPullDialog::OnEnChangeOutputPullMaxEdit)
	ON_BN_CLICKED(IDC_OUTPUT_APPLY_BUTTON, &COutputPullDialog::OnBnClickedOutputApplyButton)
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_OUTPUT_PULL_PORT_EDIT, &COutputPullDialog::OnEnChangeOutputPullPortEdit)
	ON_BN_CLICKED(IDC_OUTPUT_PLAYER_BUTTON, &COutputPullDialog::OnBnClickedOutputPlayerButton)
	ON_BN_CLICKED(IDC_OUTPUT_IPADDR_BUTTON, &COutputPullDialog::OnBnClickedOutputIpaddrButton)
	ON_COMMAND(ID_IP_POPUP, &COutputPullDialog::OnIpPopup)
	ON_COMMAND(ID_HOST_POPUP, &COutputPullDialog::OnHostPopup)
END_MESSAGE_MAP()

//////////////////////////////////////////////////
//�_�C�A���O�̏�����
//////////////////////////////////////////////////
BOOL COutputPullDialog::OnInitDialog()
{
	COutputBaseDialog::OnInitDialog();

	m_bFirstFlag = TRUE;

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PULL_ENABLE_CHECK),IDC_OUTPUT_PULL_ENABLE_CHECK);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PULL_PORT_EDIT),IDC_OUTPUT_PULL_PORT_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PULL_MAX_EDIT),IDC_OUTPUT_PULL_MAX_EDIT);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_APPLY_BUTTON),IDC_OUTPUT_APPLY_BUTTON);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_PLAYER_BUTTON),IDC_OUTPUT_PLAYER_BUTTON);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_OUTPUT_PULL_PORT_SPIN);
	assert(spin);
	spin->SetRange32(1,65535);
	spin->SetPos32(8080);

	//�ڑ��ő�l��
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_EDIT);
	assert(edit);
	CString strNum;
	strNum.Format(_T("%d"),10);
	edit->SetWindowText(strNum);
	spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_SPIN);
	assert(spin);
	spin->SetRange32(1,50);
	spin->SetPos32(10);

	//�K�p�{�^��
	CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
	assert(pDSA);
	if (pDSA->IsEncode())
	{
		ShowApplyButton(TRUE);
		SetEnableCheck(FALSE);
		OnBnClickedOutputPullEnableCheck();
	} else {
		ShowApplyButton(FALSE);
	}

	m_nRegisterID = -1;
	m_nRegisterPort = 0;
	m_nRegisterMax = 0;

	m_bFirstFlag = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
//////////////////////////////////////////////////
//�_�C�A���O�̏I��
//////////////////////////////////////////////////
void COutputPullDialog::OnDestroy()
{
	//�G���R�[�h���ꍇ�͏o�͂��폜����
	if (m_nRegisterID != -1)
	{
		Encode(FALSE);
	}
	COutputBaseDialog::OnDestroy();
}
//////////////////////////////////////////////////
//�o�͖��̎擾
//////////////////////////////////////////////////
BOOL COutputPullDialog::GetOutputName(CString &strFileName)
{
	return TRUE;
}
//////////////////////////////////////////////////
//�o�͖��̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPullDialog::SetOutputName(CString strFileName)
{
	return TRUE;
}
//////////////////////////////////////////////////
//�^�u���̎擾
//////////////////////////////////////////////////
BOOL COutputPullDialog::GetTabName(CString &strTabName)
{
	CWnd* wnd = (CWnd*)this->GetDlgItem(IDC_OUTPUT_PULL_GROUP);
	if (wnd == NULL)
		return FALSE;
	wnd->GetWindowText(strTabName);
	return TRUE;
}
//////////////////////////////////////////////////
//�^�u���̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPullDialog::SetTabName(CString strTabName)
{
	CWnd* wnd = (CWnd*)this->GetDlgItem(IDC_OUTPUT_PULL_GROUP);
	if (wnd == NULL)
		return FALSE;
	wnd->SetWindowText(strTabName);
	return TRUE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�̎擾
//////////////////////////////////////////////////
BOOL COutputPullDialog::GetEnableCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PULL_ENABLE_CHECK); 
	assert(button);
	if (button->GetCheck() & BST_CHECKED)
		return TRUE;
	return FALSE;
}
//////////////////////////////////////////////////
//�L���`�F�b�N�̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPullDialog::SetEnableCheck(BOOL bCheck)
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PULL_ENABLE_CHECK); 
	assert(button);
	if (bCheck)
		button->SetCheck(BST_CHECKED);
	else
		button->SetCheck(BST_UNCHECKED);
	return TRUE;
}
//////////////////////////////////////////////////
//�|�[�g�ԍ��̎擾
//////////////////////////////////////////////////
BOOL COutputPullDialog::GetPort(int &nPort)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_PORT_EDIT);
	if (!edit) return FALSE;
	CString strNum;
	edit->GetWindowText(strNum);
	nPort = _tstoi(strNum);
	return TRUE;
}
//////////////////////////////////////////////////
//�|�[�g�ԍ��̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPullDialog::SetPort(int nPort)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_PORT_EDIT);
	if (!edit) return FALSE;
	CString strNum;
	strNum.Format(_T("%d"),nPort);
	edit->SetWindowText(strNum);
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_OUTPUT_PULL_PORT_SPIN);
	if (!spin) return FALSE;
	spin->SetPos32(nPort);
	return TRUE;
}
//////////////////////////////////////////////////
//�ő�ڑ��l���̎擾
//////////////////////////////////////////////////
BOOL COutputPullDialog::GetMax(int &nMax)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_EDIT);
	if (!edit) return FALSE;
	CString strNum;
	edit->GetWindowText(strNum);
	nMax = _tstoi(strNum);
	return TRUE;
}
//////////////////////////////////////////////////
//�ő�ڑ��l���̐ݒ�
//////////////////////////////////////////////////
BOOL COutputPullDialog::SetMax(int nMax)
{
	CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_EDIT);
	if (!edit) return FALSE;
	CString strNum;
	strNum.Format(_T("%d"),nMax);
	edit->SetWindowText(strNum);
	CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_SPIN);
	if (!spin) return FALSE;
	spin->SetPos32(nMax);
	return TRUE;
}
//////////////////////////////////////////////////
//�K�p�{�^���̕\���Ɣ�\��
//////////////////////////////////////////////////
BOOL COutputPullDialog::ShowApplyButton(BOOL bShow)
{
	CWnd* button = (CWnd*)GetDlgItem(IDC_OUTPUT_APPLY_BUTTON); assert(button);
	button->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	CWnd* staticlabel = (CWnd*)GetDlgItem(IDC_OUTPUT_APPLY_STATIC); assert(staticlabel);
	staticlabel->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	button = (CWnd*)GetDlgItem(IDC_OUTPUT_IPADDR_BUTTON); assert(button);
	button->ShowWindow(bShow ? SW_SHOW:SW_HIDE);
	button = (CWnd*)GetDlgItem(IDC_OUTPUT_PLAYER_BUTTON); assert(button);
	button->ShowWindow(bShow ? SW_SHOW:SW_HIDE);

	return TRUE;
}
//////////////////////////////////////////////////
//�L���{�^���̃`�F�b�N
//////////////////////////////////////////////////
void COutputPullDialog::OnBnClickedOutputPullEnableCheck()
{
	CButton* button = (CButton*)this->GetDlgItem(IDC_OUTPUT_PULL_ENABLE_CHECK); 
	assert(button);
	//�e�E�B���h�E�ɒʒm����
	CWnd* pWnd = this->GetParent();
	pWnd->SendMessage(NOTIFY_ENABLE_CHECK,(WPARAM)this,button->GetCheck());
}
//////////////////////////////////////////////////
//�|�[�g�ԍ��̕ύX
//////////////////////////////////////////////////
void COutputPullDialog::OnEnChangeOutputPullPortEdit()
{
	if (!m_bFirstFlag)
	{
		CString strNum;
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_PORT_EDIT);
		assert(edit);
		CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_OUTPUT_PULL_PORT_SPIN);
		assert(spin);
		edit->GetWindowText(strNum);
		int nPortNum = _tstoi(strNum);
		if (nPortNum <= 0 || nPortNum > USHRT_MAX)
		{
			AfxMessageBox(_T("�|�[�g�ԍ���1�ȏ�65535�ȉ��ɂ��Ă�������"),MB_OK|MB_ICONINFORMATION);
			if (nPortNum <= 0)
				spin->SetPos32(1);
			else if (nPortNum > USHRT_MAX)
				spin->SetPos32(USHRT_MAX);
		}
	}
}
//////////////////////////////////////////////////
//�ڑ��l���̕ύX
//////////////////////////////////////////////////
void COutputPullDialog::OnEnChangeOutputPullMaxEdit()
{
	if (!m_bFirstFlag)
	{
		CString strNum;
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_EDIT);
		assert(edit);
		CSpinButtonCtrl* spin = (CSpinButtonCtrl*)this->GetDlgItem(IDC_OUTPUT_PULL_MAX_SPIN);
		assert(spin);

		edit->GetWindowText(strNum);
		int nMaxNum = _tstoi(strNum);
		if (nMaxNum <= 0 || nMaxNum > 50)
		{
			AfxMessageBox(_T("�ڑ��l����1�ȏ�50�ȉ��ɂ��Ă�������"),MB_OK|MB_ICONINFORMATION);
			if (nMaxNum <= 0)
				spin->SetPos32(1);
			else if (nMaxNum > 50)
				spin->SetPos32(50);
		}
	}
}
//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL COutputPullDialog::Encode(BOOL bStart)
{
	ShowApplyButton(bStart);

	if (bStart)
	{
		if (GetEnableCheck() == FALSE)
			return TRUE;
		CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
		int nPort = -1;
		int nMax = -1;
		this->GetPort(nPort);
		this->GetMax(nMax);
		if (nPort == -1 || nMax == -1) {
			SetEnableCheck(FALSE);
			OnBnClickedOutputPullEnableCheck();
			return TRUE;
		}

		for (int i= 0;i<(int)pDSA->m_PullSinkList.size();i++)
		{
			if(pDSA->m_PullSinkList[i].dwPort == nPort)
			{
				AfxMessageBox(_T("�|�[�g�ԍ����d�����Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				SetEnableCheck(FALSE);
				OnBnClickedOutputPullEnableCheck();
				return TRUE;	//�d�����x�ł̓G���[�ɂ͂�Ȃ�
			}
		}
		m_nRegisterID = -1;
		m_nRegisterPort = nPort;
		m_nRegisterMax = nMax;
		return pDSA->AddPullSink(m_nRegisterPort,m_nRegisterMax,m_nRegisterID);
	} else {
		if (m_nRegisterID != -1) {
			CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
			pDSA->RemovePullSink(m_nRegisterID);
		}
		m_nRegisterID = -1;
		m_nRegisterPort = 0;
		m_nRegisterMax = 0;
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�c�[���`�b�v�̕\��
//////////////////////////////////////////////////
BOOL COutputPullDialog::PreTranslateMessage(MSG* pMsg)
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
//IP�A�h���X���N���b�v�{�[�h�ɃR�s�[
//////////////////////////////////////////////////
void COutputPullDialog::OnBnClickedOutputIpaddrButton()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	int nPort = 8080;
	if (!GetPort(nPort)) return;

	POINT pt;
	GetCursorPos(&pt);

	if (pSetting->m_bGetGlobalIPCheck)
	{
		int nRet = AfxMessageBox(_T("�O���[�o��IP�A�h���X���擾���邽�߂Ƀl�b�g���[�N�ɐڑ����܂��B\n��낵���ł����H"),MB_YESNO|MB_ICONINFORMATION);
		if (nRet == IDNO) return;
	}
	CString strIPAddress = _T("");
	CString strHostName = _T("");
	BOOL bRet = GetGlobalIPAddress(pSetting->m_strGetGlobalIPURL,strIPAddress,strHostName);
	if (!bRet) return;
	if (strIPAddress.IsEmpty() && strHostName.IsEmpty())
		return;
	if (strIPAddress.IsEmpty() == FALSE)
		pSetting->m_strGlobalIPAddress = strIPAddress;
	if (strHostName.IsEmpty() == FALSE)
		pSetting->m_strGlobalHostName = strHostName;

	CString strIPURLFormat = _T("");
	CString strIPHostFormat = _T("");
	if (strIPAddress.IsEmpty() == FALSE)
		strIPURLFormat.Format(_T("http://%s:%d/"),strIPAddress,nPort);
	if (strHostName.IsEmpty() == FALSE)
		strIPHostFormat.Format(_T("http://%s:%d/"),strHostName,nPort);

	CMenu menu;
	menu.CreatePopupMenu();
	if (strIPURLFormat.IsEmpty() == FALSE)
		menu.AppendMenu(MF_ENABLED|MF_STRING,ID_IP_POPUP,strIPURLFormat);
	if (strIPHostFormat.IsEmpty() == FALSE)
		menu.AppendMenu(MF_ENABLED|MF_STRING,ID_HOST_POPUP,strIPHostFormat);
	menu.TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	menu.DestroyMenu();
	return;
}
//////////////////////////////////////////////////
//IP�A�h���X���N���b�v�{�[�h�ɃR�s�[
//////////////////////////////////////////////////
void COutputPullDialog::OnIPHostPopup(BOOL bIP)
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CString strURLFormat = _T("");
	int nPort = 8080;
	if (!GetPort(nPort)) return;
	if (bIP) {
		if (pSetting->m_strGlobalIPAddress.IsEmpty() == TRUE)
			return;
		strURLFormat.Format(_T("http://%s:%d/"),pSetting->m_strGlobalIPAddress,nPort);
	}
	else {
		if (pSetting->m_strGlobalHostName.IsEmpty() == TRUE)
			return;
		strURLFormat.Format(_T("http://%s:%d/"),pSetting->m_strGlobalHostName,nPort);
	}
	if (!SetClipText(strURLFormat))
	{
		AfxMessageBox(_T("�N���b�v�{�[�h�̃R�s�[�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
	}
}
//////////////////////////////////////////////////
//�O���[�o��IP�A�h���X���擾
//////////////////////////////////////////////////
BOOL COutputPullDialog::GetGlobalIPAddress(CString strURL,CString &strIPAddress,CString &strHostName)
{
	strIPAddress = _T("");
	strHostName = _T("");
	if (strURL.IsEmpty()) return FALSE;

	CInternetSession session(_T("My Session"),0,INTERNET_OPEN_TYPE_PRECONFIG,NULL, NULL,INTERNET_FLAG_DONT_CACHE);

	DWORD dwServiceType = 0;
	CString strServer = _T("");
	CString strObject = _T("");
	INTERNET_PORT nPort = 80;
	if (!AfxParseURL(strURL,dwServiceType,strServer,strObject,nPort))
		return FALSE;
	DWORD dwReqFlag;
	if (dwServiceType == AFX_INET_SERVICE_HTTPS) {
		dwReqFlag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_SECURE;
	} else {
		dwReqFlag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE;
	}
	CHttpConnection *pHttp = NULL;
	CString strUser = _T("");
	CString strPass = _T("");
	pHttp = session.GetHttpConnection(strServer, nPort, strUser, strPass);
	CHttpFile *pHttpFile = NULL;
	pHttpFile = pHttp->OpenRequest(CHttpConnection::HTTP_VERB_GET,strObject, NULL, 1, NULL, NULL, dwReqFlag);
	if (!pHttpFile) {
		pHttp->Close();
		delete pHttp;
		pHttp = NULL;
		session.Close();
		return FALSE;
	}

	if (dwServiceType == AFX_INET_SERVICE_HTTPS) {
		DWORD dwFlags = 0;
		DWORD dwbuffLen = sizeof(dwFlags);
		InternetQueryOption(*pHttpFile,INTERNET_OPTION_SECURITY_FLAGS,(LPVOID)&dwFlags,&dwbuffLen);
		dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
		InternetSetOption(*pHttpFile, INTERNET_OPTION_SECURITY_FLAGS, &dwFlags,  sizeof (dwFlags));
	}
	if (!pHttpFile->SendRequest()){
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
		pHttp->Close();
		delete pHttp;
		pHttp = NULL;
		session.Close();
		return FALSE;
	}
	DWORD dwRc = 0;
	pHttpFile->QueryInfoStatusCode(dwRc);
	if (dwRc != HTTP_STATUS_OK) {
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
		pHttp->Close();
		delete pHttp;
		pHttp = NULL;
		session.Close();
		return FALSE;
	}

	if (pHttpFile) {
		DWORD dwSize = (DWORD)pHttpFile->GetLength();
		BYTE* pByte = new BYTE[dwSize + 1];
		ZeroMemory(pByte,(int)((dwSize+1)*sizeof(BYTE)));

		pHttpFile->Read(pByte,dwSize);
		pByte[dwSize] = '\0';
		int nLen = MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)pByte,-1,NULL,0);
		wchar_t* szBuffer = new wchar_t[nLen];
		ZeroMemory(szBuffer,nLen*sizeof(wchar_t));
		MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,(LPCSTR)pByte,-1,szBuffer,nLen);

		CString strBuff = szBuffer;
		int nIndexReturn = strBuff.Find(_T('\n'));
		int nIndexEndReturn = strBuff.ReverseFind(_T('\n'));
		if (nIndexReturn != nIndexEndReturn)
			strBuff = strBuff.Right(strBuff.GetLength() -1);

		strIPAddress = strBuff.Left(nIndexEndReturn - 2);
		strHostName = strBuff.Mid(nIndexEndReturn,strBuff.GetLength() - nIndexEndReturn);

		strBuff.Empty();
		delete [] szBuffer;
		szBuffer = NULL;
		delete [] pByte;
		pByte = NULL;
	}

	if (pHttpFile) {
		pHttpFile->Close();
		delete pHttpFile;
		pHttpFile = NULL;
	}
	if (pHttp) {
		pHttp->Close();
		delete pHttp;
		pHttp = NULL;
	}
	session.Close();
	return TRUE;
}

//////////////////////////////////////////////////
//�N���b�v�{�[�h�ɕ�����̐ݒ������
//////////////////////////////////////////////////
BOOL COutputPullDialog::SetClipText(LPCTSTR text)
{
    // �N���b�v�{�[�h�Ƀe�L�X�g����������
    if(!OpenClipboard()) {
        return FALSE;
    }
    if(!EmptyClipboard()) {
        return FALSE;
    };

	int length = lstrlenW(text) * sizeof(TCHAR) + sizeof(TCHAR);
    HGLOBAL    hGlobal = GlobalAlloc(GHND | GMEM_DDESHARE, length);
    TCHAR* clipInText = (TCHAR*)GlobalLock(hGlobal);
    //lstrcpyW(clipInText, lpwCmdLineList[1]);
    memcpy(clipInText, text, length);
    GlobalUnlock(hGlobal);
    if(SetClipboardData(CF_UNICODETEXT, clipInText) == NULL) {
        GlobalFree(hGlobal);
        CloseClipboard();
        return FALSE;
    }
    CloseClipboard();
	return TRUE;
}

//////////////////////////////////////////////////
//�v���C���[�ŊJ���{�^��
//////////////////////////////////////////////////
void COutputPullDialog::OnBnClickedOutputPlayerButton()
{
	int nPort;
	this->GetPort(nPort);

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (!pSetting->m_strVideoPlayerPath.IsEmpty())
	{
		//�R�}���h���C���Ń��f�B�A�v���C���[���N������
		CString strCmdLine;
		strCmdLine.Format(_T("http://127.0.0.1:%d/"),nPort);
		::ShellExecute(NULL,NULL,pSetting->m_strVideoPlayerPath,strCmdLine,NULL,SW_SHOWNORMAL);
	}
}

//////////////////////////////////////////////////
//�K�p�{�^��
//////////////////////////////////////////////////
void COutputPullDialog::OnBnClickedOutputApplyButton()
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
			int nPort = 0;
			GetPort(nPort);
			int nMax = 0;
			GetMax(nMax);
			if (nPort != m_nRegisterPort) {	//�|�[�g�ԍ��̕ύX��������
				Encode(FALSE);
				Encode(TRUE);
			} else if (nMax != m_nRegisterMax) {	//�l���݂̂̕ύX
				CDirectShowAccess* pDSA= ((CKTEApp*)AfxGetApp())->GetDSA();
				assert(pDSA);
				BOOL bRet = FALSE;
				bRet = pDSA->ChangePullSink(m_nRegisterID,nMax);	//�l���݂̂̕ύX�����݂�
				m_nRegisterMax = nMax;
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
	�|�[�g�ԍ��݂̂̕ύX	�����Ē���
	�|�[�g�ԍ��Ɛl���̕ύX	�����Ē���
	�l���݂̂̕ύX			�����Ē������l���ύX�����݂�
�G���R�[�h�̒�~(�L��->����)
*/



