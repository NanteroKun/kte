// MonitorFormView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MonitorFormView.h"


// CMonitorFormView

IMPLEMENT_DYNCREATE(CMonitorFormView, CFormView)

CMonitorFormView::CMonitorFormView()
	: CFormView(CMonitorFormView::IDD)
{

}

CMonitorFormView::~CMonitorFormView()
{
}

void CMonitorFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMonitorFormView, CFormView)
	ON_MESSAGE( WM_INITDIALOG, &CMonitorFormView::OnInitDialog)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_MONITOR_TAB, &CMonitorFormView::OnTcnSelchangeMonitorTab)
END_MESSAGE_MAP()


// CMonitorFormView �f�f

#ifdef _DEBUG
void CMonitorFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMonitorFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMonitorFormView ���b�Z�[�W �n���h��

//////////////////////////////////////////////////
//�E�B���h�E�X�^�C���ύX
//////////////////////////////////////////////////
BOOL CMonitorFormView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
    // TODO:
    //CSize sizeTotal;
    //sizeTotal.cx = sizeTotal.cy = 100;
    //SetScrollSizes(MM_TEXT, sizeTotal);

	return CFormView::PreCreateWindow(cs);
}

//////////////////////////////////////////////////
//WM_INITDIALOG�̃C�x���g�n���h��
//���̒i�K��HWND�ƃR���g���[���͏���������Ă���(�g�p�\)
//����DDX/DDV�͎g�p�s�\�Ȃ̂�GetDlgItem����擾���邱�ƁB
//////////////////////////////////////////////////
LRESULT CMonitorFormView::OnInitDialog(WPARAM wParam,LPARAM lParam)
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	//�e��^�u(�q�_�C�A���O)�𐶐�����
	m_StatisticsTab.Create(CMonitorStatisticsDialog::IDD,this);
	m_PullTab.Create(CMonitorPullDialog::IDD,this);
	m_EventLogTab.Create(CMonitorEventLogDialog::IDD,this);
	if (pSetting->m_bShowDebugWindow) {
		m_GraphTab.Create(CMoniterGraphDialog::IDD,this);}

	//�q�_�C�A���O���ړ�����
	CRect rcDialog;
	m_StatisticsTab.GetClientRect(&rcDialog);
	rcDialog.OffsetRect(0,22);
	m_StatisticsTab.MoveWindow(&rcDialog,FALSE);
	m_PullTab.MoveWindow(&rcDialog,FALSE);
	m_EventLogTab.MoveWindow(&rcDialog,FALSE);
	if (pSetting->m_bShowDebugWindow) {
		m_GraphTab.MoveWindow(&rcDialog,FALSE); }

	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_MONITOR_TAB);
	assert(tab);
	tab->ModifyStyle(0,TCS_FIXEDWIDTH,0);

	tab->InsertItem(0,_T("���v"));
	tab->InsertItem(1,_T("�u���[�h�L���X�g"));
	tab->InsertItem(2,_T("�C�x���g���O"));
	if (pSetting->m_bShowDebugWindow) {
		tab->InsertItem(3,_T("�t�B���^�[�O���t")); }

	CSize size;
	CRect rcRect;
	tab->GetItemRect(0,&rcRect);
	size.SetSize(100,rcRect.bottom - rcRect.top);
	CSize old = tab->SetItemSize(size);
	size.SetSize(100,old.cy);
	tab->SetItemSize(size);

	//�^�u��\��
	tab->SetCurSel(pSetting->m_nMonitorTabIndex);
	OnTcnSelchangeMonitorTab(NULL,NULL);
	return 0;
}
//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX
//////////////////////////////////////////////////
void CMonitorFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	//�^�u�R���g���[�����ő剻����
	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_MONITOR_TAB);
	if (!tab) return;
	CRect rcClient;
	this->GetClientRect(&rcClient);
	tab->MoveWindow(rcClient,TRUE);

	//�����̃_�C�A���O�^�u���ő剻���ۂ��ړ�����
	if (!m_StatisticsTab.GetSafeHwnd() || !m_PullTab.GetSafeHwnd())
		return;
	rcClient.OffsetRect(0,22);
	rcClient.right -= 5;
	rcClient.bottom -= 25;
	m_StatisticsTab.MoveWindow(&rcClient,TRUE);
	m_PullTab.MoveWindow(&rcClient,TRUE);
	m_EventLogTab.MoveWindow(&rcClient,TRUE);
	if (pSetting->m_bShowDebugWindow) {
		m_GraphTab.MoveWindow(&rcClient,TRUE); }

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}
//////////////////////////////////////////////////
//�_�C�A���O���I������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorFormView::OnDestroy()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_MONITOR_TAB);
	if (tab)
		pSetting->m_nMonitorTabIndex = tab->GetCurSel();

	//�e��^�u�_�C�A���O�����
	if (m_StatisticsTab.GetSafeHwnd())
	{
		m_StatisticsTab.EndDialog(0);
		m_StatisticsTab.DestroyWindow();
	}
	if (m_PullTab.GetSafeHwnd())
	{
		m_PullTab.EndDialog(0);
		m_PullTab.DestroyWindow();
	}
	if (m_EventLogTab.GetSafeHwnd())
	{
		m_EventLogTab.EndDialog(0);
		m_EventLogTab.DestroyWindow();
	}
	if (m_GraphTab.GetSafeHwnd())
	{
		m_GraphTab.EndDialog(0);
		m_GraphTab.DestroyWindow();
	}

	CFormView::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CMonitorFormView::Encode(BOOL bStart)
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	if (!m_StatisticsTab.Encode(bStart))
		return FALSE;
	if (!m_PullTab.Encode(bStart))
		return FALSE;
	if (!m_EventLogTab.Encode(bStart))
		return FALSE;
	if (pSetting->m_bShowDebugWindow) {
		if (!m_GraphTab.Encode(bStart))
			return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�^�u�̕ύX
//////////////////////////////////////////////////
void CMonitorFormView::OnTcnSelchangeMonitorTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_MONITOR_TAB);
	if (!tab) return;

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	int nSelectSel = tab->GetCurSel();
	m_StatisticsTab.ShowWindow(nSelectSel == 0 ? SW_SHOW : SW_HIDE);
	m_PullTab.ShowWindow(nSelectSel == 1 ? SW_SHOW : SW_HIDE);
	m_EventLogTab.ShowWindow(nSelectSel == 2  ? SW_SHOW : SW_HIDE);
	if (pSetting->m_bShowDebugWindow) {
		m_GraphTab.ShowWindow(nSelectSel == 3  ? SW_SHOW : SW_HIDE);
	}
	if (pResult) *pResult = 0;
}

void CMonitorFormView::OnDraw(CDC* /*pDC*/)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
}
