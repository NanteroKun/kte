// MonitorPullDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MonitorPullDialog.h"
#include "MainFrm.h"

// CMonitorPullDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CMonitorPullDialog, CDialog)

CMonitorPullDialog::CMonitorPullDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorPullDialog::IDD, pParent)
{
	m_nAllConnectCount = 0;
	m_ConnectCountList.clear();
	m_ConnectList.clear();
	m_AddConnectList.clear();
}

CMonitorPullDialog::~CMonitorPullDialog()
{
}

void CMonitorPullDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorPullDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_MESSAGE(WM_NOTIFY_NETWORK_CLIENT_CONNECT,&CMonitorPullDialog::OnNetworkClientConnect)
	ON_MESSAGE(WM_NOTIFY_NETWORK_CLIENT_DISCONNECT,&CMonitorPullDialog::OnNetworkClientDisconnect)

END_MESSAGE_MAP()

////////////////////////////////////////
//�_�C�A���O������
////////////////////////////////////////
BOOL CMonitorPullDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	//�l�b�g���[�N�R�[���o�b�N�̃E�B���h�E�n���h���ɂɂ��̃E�B�h�E���֘A�t��������
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	pDSA->SetNetworkCallbackHWnd(this->GetSafeHwnd());

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	CListCtrl* list = GetListCtrl();
	LVCOLUMN lvc;
	TCHAR       caption[][32] = {
		_T("�|�[�g"),
		_T("IP�A�h���X"),
		_T("�z�X�g��"),
		_T("�|�[�g��"),
		_T("�ڑ��J�n����"),
		_T("�ڑ��o�ߎ���")};
	const int   clmNum = sizeof caption /sizeof caption[0];
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	for (int i = 0; i < clmNum; i++)
	{
		lvc.iSubItem = i;
		lvc.pszText = caption[i];
		lvc.cx = pSetting->m_nMonitorPullColumnWidthList[i];
		list->InsertColumn(i,&lvc);
	}
	//��P�ʂɑI��
	list->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);
	//�O���b�h����\��
	list->SetExtendedStyle((list->GetExtendedStyle()|LVS_EX_GRIDLINES));

	//�c�[���`�b�v����LVS_EX_INFOTIP

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

//////////////////////////////////////////////////
//�_�C�A���O���I������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorPullDialog::OnDestroy()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsEncode()) {
		Encode(FALSE);
	}

	//�ݒ�̕ۑ�(�J�����̑傫��)
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	pSetting->m_nMonitorPullColumnWidthList.clear();
	CListCtrl* list = GetListCtrl();
	CHeaderCtrl* header = list->GetHeaderCtrl();
	assert(header);
	for (int i=0;i < header->GetItemCount();i++)
	{
		int nWidth = list->GetColumnWidth(i);
		pSetting->m_nMonitorPullColumnWidthList.push_back(nWidth);
	}


	CDialog::OnDestroy();
}
//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX
//////////////////////////////////////////////////
void CMonitorPullDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_PULL_LIST);
	if (!list) return;

	CRect rcClient;
	this->GetClientRect(&rcClient);
	CRect rcListWindow;
	list->GetWindowRect(&rcListWindow);
	this->ScreenToClient(&rcListWindow);
	CRect rcNewList;
	rcNewList.left = rcListWindow.left;
	rcNewList.top = rcListWindow.top;
	rcNewList.right = rcClient.right - rcListWindow.left;//  - rcListWindow.left;
	rcNewList.bottom = rcClient.bottom - rcListWindow.left;

	list->MoveWindow(&rcNewList,TRUE);
}
//////////////////////////////////////////////////
//WM_PAINT�̕`��n���h��
//////////////////////////////////////////////////
void CMonitorPullDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	//�Ȃ����q�E�B���h�E�̃��X�g�R���g���[�������ĕ`�悳��Ȃ��̂ŁA�`��X�V���b�Z�[�W��^���Ă���
	CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_PULL_LIST);
	if (!list) return;
	list->InvalidateRect(NULL);
	list->UpdateWindow();
}


//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CMonitorPullDialog::Encode(BOOL bStart)
{
	const int TIMER_ID = 101;

	if (!bStart)
		KillTimer(TIMER_ID);

	//�S�ڑ�����j��
	m_nAllConnectCount = 0;		//�ڑ��l��
	m_ConnectCountList.clear();	//Pull���Ƃ̐ڑ��l��
	m_ConnectList.clear();		//�ڑ����
	m_AddConnectList.clear();	//�ǉ����ꂽ�ڑ����
	CListCtrl* list = GetListCtrl();
	list->DeleteAllItems();		//�S�ẴA�C�e�����폜����

	if (bStart)
		SetTimer(TIMER_ID,1000,NULL);
	return TRUE;
}
//////////////////////////////////////////////////
//WM_TIMER�̃C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorPullDialog::OnTimer(UINT_PTR nIDEvent)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CListCtrl* list = GetListCtrl();

	if (!pDSA->IsEncode())
		return;


	//�ǉ��̃A�C�e�����ɂ��Ē��ׂ�
	for (int i=0;i<(int)m_AddConnectList.size();i++)
	{
		NetworkClientStruct ncs;
		DWORD dwSrcPort;
		if (!pDSA->GetPullSink(
			m_AddConnectList[i].nIndex,
			m_AddConnectList[i].dwIPAddress,
			m_AddConnectList[i].dwPort,
			ncs,
			dwSrcPort))
			continue;

		//�\���̂Ɋi�[���Ă���
		PullConnectStruct pcs;
		pcs.dwIPAddress = ncs.dwNetworkAddress;
		pcs.nIPAddress[0]  = ncs.nNetworkAddress[0];
		pcs.nIPAddress[1]  = ncs.nNetworkAddress[1];
		pcs.nIPAddress[2]  = ncs.nNetworkAddress[2];
		pcs.nIPAddress[3]  = ncs.nNetworkAddress[3];
		pcs.strIPAddress = ncs.strNetworkAddress.c_str();
		pcs.strHostName = ncs.strDNSName.c_str();
		pcs.nSrcPort =  dwSrcPort;
		pcs.nDestPort = ncs.dwPort;
		pcs.StartTime = CTime::GetCurrentTime();
		//pcs.ConnectTime = CTime::GetCurrentTime() - pcs.StartTime;
		
		//m_ConnectList�̋󂫔z����`�F�b�N���đ}������
		BOOL bInsert = FALSE;
		int nInsertIndex = -1;	//�z��̑}���ꏊ
		for (int j=0;j<(int)m_ConnectList.size();j++)
		{
			if (m_ConnectList[j].dwIPAddress != 0)
				continue;
			if (m_ConnectList[j].nDestPort != 0)
				continue;
			if (m_ConnectList[j].nSrcPort != 0)
				continue;
			m_ConnectList[j] = pcs;
			nInsertIndex = j;
			bInsert = TRUE;
		}
		if (!bInsert) {
			m_ConnectList.push_back(pcs);	//�V�����z��ɒǉ�����
			nInsertIndex = (int)m_ConnectList.size() - 1;
		}

		//���X�g�r���[�ɕ\��
		CString strNum;
		strNum.Format(_T("%d"),pcs.nSrcPort);
		int nNewIndex = list->InsertItem(list->GetItemCount(),strNum);
		strNum.Format(_T("%d.%d.%d.%d"),
			pcs.nIPAddress[0],
			pcs.nIPAddress[1],
			pcs.nIPAddress[2],
			pcs.nIPAddress[3]);
		list->SetItemText(nNewIndex,1,strNum);
		list->SetItemText(nNewIndex,2,pcs.strHostName);
		strNum.Format(_T("%d"),pcs.nDestPort);
		list->SetItemText(nNewIndex,3,strNum);
		strNum.Format(_T("%s"),pcs.StartTime.Format(_T("%H:%M:%S")));
		list->SetItemText(nNewIndex,4,strNum);
		list->SetItemData(nNewIndex,nInsertIndex);

		//�ڑ��N���C�A���g�̑���
		m_nAllConnectCount++;
		if (m_AddConnectList[i].nIndex < (int)m_ConnectCountList.size()) {
			m_ConnectCountList[m_AddConnectList[i].nIndex]++;
		} else {
			m_ConnectCountList.resize(m_AddConnectList[i].nIndex+1,0);
			m_ConnectCountList[m_AddConnectList[i].nIndex]++;
		}
		//�ڑ��l���e�L�X�g���X�V����
		UpdateConnectionEdit();

		//�C�x���g���O�ɐڑ����𑗂�
		//CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd(); assert(frame);
		CString strAddLog;
		strAddLog.Format(_T("%d.%d.%d.%d(%s)���ڑ�����܂���"),
			pcs.nIPAddress[0],
			pcs.nIPAddress[1],
			pcs.nIPAddress[2],
			pcs.nIPAddress[3],
			pcs.strHostName);
		CMonitorFormView* pWndMoniter = (CMonitorFormView*)GetParent();
		CMonitorEventLogDialog* pWndLog = pWndMoniter->GetEventLogTab();
		pWndLog->AddEventLog(strAddLog);
		//�u���[�h�L���X�g �|�[�g: 8080 - 48 �N���C�A���g
		strAddLog.Format(_T("�u���[�h�L���X�g �|�[�g:%d - %d �N���C�A���g"),
			pcs.nSrcPort,
			m_ConnectCountList[m_AddConnectList[i].nIndex]);
		pWndLog->AddEventLog(strAddLog);
	}

	if (m_AddConnectList.size() != 0)
		m_AddConnectList.clear();	//�ǋL���͏����ς݂̂��߂ɑS�N���A����

	//�o�ߎ��Ԃ��X�V
	for (int i=0;i<(int)m_ConnectList.size();i++) {
		m_ConnectList[i].ConnectTime = CTime::GetCurrentTime() - m_ConnectList[i].StartTime;
	}

	//�o�ߎ��Ԃ̍X�V�����X�g�r���[�ɔ��f
	/*
	CString strNum;
	int nArrayIndex;
	LONGLONG Second,Hour,Minute;
	for (int i=0;i<list->GetItemCount();i++)
	{
		nArrayIndex = (int)list->GetItemData(i);
		Second = m_ConnectList[nArrayIndex].ConnectTime.GetTotalSeconds();
		Hour = Second / (60*60);
		Minute = (Second / 60) % 60;
		Second = Second % 60;
		strNum.Format(_T("%I64d:%02I64d:%02I64d"),Hour,Minute,Second);
		list->SetItemText(i,5,strNum);
	}
	*/
	CString strNum;
	int nArrayIndex;
	for (int i=0;i<list->GetItemCount();i++)
	{
		nArrayIndex = (int)list->GetItemData(i);
		assert(nArrayIndex >=0 && nArrayIndex<(int)m_ConnectList.size());
		LONGLONG GetHour = m_ConnectList[nArrayIndex].ConnectTime.GetTotalHours();
		LONG GetMinite = m_ConnectList[nArrayIndex].ConnectTime.GetMinutes();
		LONG GetSecond = m_ConnectList[nArrayIndex].ConnectTime.GetSeconds();
		strNum.Format(_T("%I64d:%02d:%02d"),GetHour,GetMinite,GetSecond);
		list->SetItemText(i,5,strNum);
	}

	CDialog::OnTimer(nIDEvent);
}
////////////////////////////////////////
//�l�b�g���[�N�V���N����ڑ��������Ƃ��̃C�x���g�n���h��
////////////////////////////////////////
LRESULT CMonitorPullDialog::OnNetworkClientConnect(WPARAM wParam,LPARAM lParam)
{
	//TRACE0("OnNetworkClientConnect\n");
	WM_CLIENT_PROPERTIES* pProp;
	pProp = (WM_CLIENT_PROPERTIES*)lParam;
	int nIndex;
	nIndex = (int)wParam;

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (!pDSA->IsVaildPullSink(nIndex))
		return 0;
	if (pProp->dwPort > USHRT_MAX)
		return 0;

	//�ǉ��A�C�e�����ɋL�ڂ���
	AddConnectStruct acs;
	acs.dwIPAddress = pProp->dwIPAddress;
	acs.dwPort = pProp->dwPort;
	acs.nIndex = nIndex;
	m_AddConnectList.push_back(acs);

	return 0;
}
////////////////////////////////////////
//�l�b�g���[�N�V���N����ؒf�������Ƃ��̃C�x���g�n���h��
////////////////////////////////////////
LRESULT CMonitorPullDialog::OnNetworkClientDisconnect(WPARAM wParam,LPARAM lParam)
{
	//TRACE0("OnNetworkClientDisconnect\n");
	WM_CLIENT_PROPERTIES* pProp;
	pProp = (WM_CLIENT_PROPERTIES*)lParam;
	int nIndex = (int)wParam;

	BOOL bDisconnect = FALSE;	//�{���ɐؒf���ꂽ���ǂ���
	int nSrcPort = -1;	//���M���|�[�g���L�����Ă���

	CListCtrl* list = GetListCtrl();
	for (int i=0;i<(int)m_ConnectList.size();i++)
	{
		if (m_ConnectList[i].dwIPAddress != pProp->dwIPAddress)
			continue;
		if (m_ConnectList[i].nDestPort != pProp->dwPort)
			continue;

		//�ڑ����𑗂�
		CString strAddLog;
		strAddLog.Format(_T("%d.%d.%d.%d(%s)���ؒf����܂���"),
			m_ConnectList[i].nIPAddress[0],
			m_ConnectList[i].nIPAddress[1],
			m_ConnectList[i].nIPAddress[2],
			m_ConnectList[i].nIPAddress[3],
			m_ConnectList[i].strHostName);
		CMonitorFormView* pWndMoniter = (CMonitorFormView*)GetParent();
		CMonitorEventLogDialog* pWndLog = pWndMoniter->GetEventLogTab();
		pWndLog->AddEventLog(strAddLog);


		//���X�g�r���[����A�C�e�����폜����
		//int nArrayIndex;
		CString strNum,strNum2;
		for (int j=0;j<list->GetItemCount();j++)	//�A�C�e�����̃��[�v
		{
			strNum = list->GetItemText(j,0);	//�|�[�g��
			if (_tstoi(strNum) != m_ConnectList[i].nSrcPort)
				continue;
			strNum = list->GetItemText(j,1);	//IP�A�h���X
			strNum2.Format(_T("%d.%d.%d.%d"),
				m_ConnectList[i].nIPAddress[0],
				m_ConnectList[i].nIPAddress[1],
				m_ConnectList[i].nIPAddress[2],
				m_ConnectList[i].nIPAddress[3]);
			if (strNum.Compare(strNum2) != 0)
				continue;
			strNum = list->GetItemText(j,2);	//�z�X�g��
			if (strNum.Compare(m_ConnectList[i].strHostName) != 0)
				continue;
			strNum = list->GetItemText(j,3);	//�|�[�g��
			if (_tstoi(strNum) != m_ConnectList[i].nDestPort)
				continue;
			list->DeleteItem(j);
			break;
			//nArrayIndex = (int)list->GetItemData(i);
			//if (nArrayIndex == i) {
			//	list->DeleteItem(j);
			//	break;
			//}
		}
		nSrcPort = m_ConnectList[i].nSrcPort;
		//���̏���
		m_ConnectList[i].nSrcPort = 0;
		m_ConnectList[i].strIPAddress = _T("");
		m_ConnectList[i].nIPAddress[0] = 0;
		m_ConnectList[i].nIPAddress[1] = 0;
		m_ConnectList[i].nIPAddress[2] = 0;
		m_ConnectList[i].nIPAddress[3] = 0;
		m_ConnectList[i].dwIPAddress = 0;
		m_ConnectList[i].strHostName = _T("");
		m_ConnectList[i].nDestPort = 0;
		m_ConnectList[i].nDestPort = 0;
		m_ConnectList[i].StartTime = 0;
		m_ConnectList[i].ConnectTime = 0;

		bDisconnect = TRUE;
		break;
	}

	if (bDisconnect)
	{
		//�ڑ��N���C�A���g�̌���
		m_nAllConnectCount--;
		if (nIndex < (int)m_ConnectCountList.size()) {
			m_ConnectCountList[nIndex]--;

			//�C�x���g���O�𑗂�
			CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd();
			assert(frame);
			CString strAddLog;
			//�u���[�h�L���X�g �|�[�g: 8080 - 48 �N���C�A���g
			strAddLog.Format(_T("�u���[�h�L���X�g �|�[�g:%d - %d �N���C�A���g"),
				nSrcPort,
				m_ConnectCountList[nIndex]);
			CMonitorFormView* pWndMoniter = (CMonitorFormView*)GetParent();
			CMonitorEventLogDialog* pWndLog = pWndMoniter->GetEventLogTab();
			pWndLog->AddEventLog(strAddLog);
		}
		//�ڑ��l���e�L�X�g���X�V����
		UpdateConnectionEdit();
	}
	return 0;
}

////////////////////////////////////////
//���ڑ����\���̍X�V
////////////////////////////////////////
void CMonitorPullDialog::UpdateConnectionEdit()
{
	CString strNum;
	CWnd* pWnd = this->GetDlgItem(IDC_PULL_MAX_COUNT_STATIC);
	assert(pWnd);
	strNum.Format(_T("���݂̑��ڑ��� %d"),m_nAllConnectCount);
	if (m_ConnectCountList.size() >= 2)
	{
		CString strNum2;
		strNum += _T(" (");
		for (int i = 0; i <(int)m_ConnectCountList.size();i++)
		{
			strNum2.Format(_T("%d"),m_ConnectCountList[i]);
			strNum += strNum2;
			if (i != (int)m_ConnectCountList.size() - 1)
			{
				strNum += _T("+");
			}
		}
		strNum += _T(")");
	}
	pWnd->SetWindowText(strNum);

	return;
}
