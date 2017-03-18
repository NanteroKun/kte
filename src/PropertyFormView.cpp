// PropertyFormView.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "PropertyFormView.h"


// CPropertyFormView

IMPLEMENT_DYNCREATE(CPropertyFormView, CFormView)

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CPropertyFormView::CPropertyFormView()
	: CFormView(CPropertyFormView::IDD)
{
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CPropertyFormView::~CPropertyFormView()
{
}

//////////////////////////////////////////////////
//DDX/DDV�̏�����
//////////////////////////////////////////////////
void CPropertyFormView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CPropertyFormView, CFormView)
	ON_MESSAGE( WM_INITDIALOG, &CPropertyFormView::OnInitDialog)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_PROPERTY_TAB, &CPropertyFormView::OnTcnSelchangePropertyTab)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPropertyFormView �f�f

#ifdef _DEBUG
void CPropertyFormView::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CPropertyFormView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG

//////////////////////////////////////////////////
//�E�B���h�E�X�^�C���ύX
//////////////////////////////////////////////////
BOOL CPropertyFormView::PreCreateWindow(CREATESTRUCT& cs)
{
	BOOL bResult = CFormView::PreCreateWindow(cs);

	//cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	//cs.dwExStyle |= WS_EX_WINDOWEDGE;
	//cs.style &= ~WS_BORDER;

	return bResult;
}

//////////////////////////////////////////////////
//WM_INITDIALOG�̃C�x���g�n���h��
//���̒i�K��HWND�ƃR���g���[���͏���������Ă���(�g�p�\)
//����DDX/DDV�͎g�p�s�\�Ȃ̂�GetDlgItem����擾���邱�ƁB
//////////////////////////////////////////////////
LRESULT CPropertyFormView::OnInitDialog(WPARAM wParam,LPARAM lParam)
{
	//�e��^�u(�q�_�C�A���O)���쐬����
	m_InputTab.Create(CInputPropertyDialog::IDD,this);
	m_OutputTab.Create(COutputPropertyDialog::IDD,this);
	m_EncodeTab.Create(CEncodePropertyDialog::IDD,this);
	m_MetadataTab.Create(CMetadataPropertyDialog::IDD,this);

	//�q�_�C�A���O���ړ�����
	CRect rcDialog;
	m_InputTab.GetClientRect(&rcDialog);
	rcDialog.OffsetRect(0,20);
	m_InputTab.MoveWindow(&rcDialog,FALSE);
	m_OutputTab.MoveWindow(&rcDialog,FALSE);
	m_EncodeTab.MoveWindow(&rcDialog,FALSE);
	m_MetadataTab.MoveWindow(&rcDialog,FALSE);

	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_PROPERTY_TAB);
	tab->ModifyStyle(0,TCS_FIXEDWIDTH,0);

	tab->InsertItem(0,_T("����"));
	tab->InsertItem(1,_T("�o��"));
	tab->InsertItem(2,_T("���k"));
	tab->InsertItem(3,_T("����"));

	CSize size;
	CRect rcRect;
	tab->GetItemRect(0,&rcRect);
	size.SetSize(65,rcRect.bottom - rcRect.top);
	CSize old = tab->SetItemSize(size);
	size.SetSize(65,old.cy);
	tab->SetItemSize(size);

	//�^�u��\��
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	tab->SetCurSel(pSetting->m_nPropertyTabIndex);
	OnTcnSelchangePropertyTab(NULL,NULL);
	return 0;
}
//////////////////////////////////////////////////
//�_�C�A���O���I������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CPropertyFormView::OnDestroy()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_PROPERTY_TAB);
	if (tab)
		pSetting->m_nPropertyTabIndex = tab->GetCurSel();

	//�e��^�u�_�C�A���O�����
	if (m_EncodeTab.GetSafeHwnd())
	{
		m_EncodeTab.EndDialog(0);
		m_EncodeTab.DestroyWindow();
	}
	if (m_OutputTab.GetSafeHwnd())
	{
		m_OutputTab.EndDialog(0);
		m_OutputTab.DestroyWindow();
	}
	if (m_InputTab.GetSafeHwnd())
	{
		m_InputTab.EndDialog(0);
		m_InputTab.DestroyWindow();
	}
	if (m_MetadataTab.GetSafeHwnd())
	{
		m_MetadataTab.EndDialog(0);
		m_MetadataTab.DestroyWindow();
	}

	CFormView::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX
//////////////////////////////////////////////////
void CPropertyFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	//�^�u�R���g���[�����ő剻����
	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_PROPERTY_TAB);
	if (!tab) return;
	CRect rcClient;
	this->GetClientRect(&rcClient);
	tab->MoveWindow(rcClient,TRUE);

	//�����̃_�C�A���O�^�u���ő剻���ۂ��ړ�����
	if (!m_InputTab.GetSafeHwnd() || !m_OutputTab.GetSafeHwnd() || !m_EncodeTab.GetSafeHwnd())
		return;
	rcClient.OffsetRect(0,22);
	rcClient.right -= 5;
	rcClient.bottom -= 25;
	m_InputTab.MoveWindow(&rcClient);
	m_OutputTab.MoveWindow(&rcClient);
	m_EncodeTab.MoveWindow(&rcClient);
	m_MetadataTab.MoveWindow(&rcClient);
}

//////////////////////////////////////////////////
//�^�u��ύX�������ɌĂяo�����C�x���g�n���h��
//////////////////////////////////////////////////
void CPropertyFormView::OnTcnSelchangePropertyTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CTabCtrl* tab = (CTabCtrl*)GetDlgItem(IDC_PROPERTY_TAB);
	if (!tab) return;

	int nSelectSel = tab->GetCurSel();
	m_InputTab.ShowWindow(nSelectSel == 0 ? SW_SHOW : SW_HIDE);
	m_OutputTab.ShowWindow(nSelectSel == 1 ? SW_SHOW : SW_HIDE);
	m_EncodeTab.ShowWindow(nSelectSel == 2 ? SW_SHOW : SW_HIDE);
	m_MetadataTab.ShowWindow(nSelectSel == 3 ? SW_SHOW : SW_HIDE);

	if (pResult) *pResult = 0;
}

//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL CPropertyFormView::ChangeProfileNotify()
{
	m_InputTab.ChangeProfileNotify();
	m_OutputTab.ChangeProfileNotify();
	m_EncodeTab.ChangeProfileNotify();
	m_MetadataTab.ChangeProfileNotify();
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���̃Z�[�u
//////////////////////////////////////////////////
BOOL CPropertyFormView::SaveProfileSetting()
{
	m_InputTab.SaveProfileSetting();
	m_OutputTab.SaveProfileSetting();
	m_EncodeTab.SaveProfileSetting();
	m_MetadataTab.SaveProfileSetting();
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CPropertyFormView::Encode(BOOL bStart)
{
	if (!m_InputTab.Encode(bStart))
		return FALSE;
	if (!m_OutputTab.Encode(bStart))
		return FALSE;
	if (!m_EncodeTab.Encode(bStart))
		return FALSE;
	if (!m_MetadataTab.Encode(bStart))
		return FALSE;
	return TRUE;
}
