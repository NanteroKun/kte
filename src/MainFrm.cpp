
// MainFrm.cpp : CMainFrame �N���X�̎���
//

#include "stdafx.h"
#include "KTE.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define WM_STOP_ENCODE (WM_APP + 102)

//////////////////////////////////////////////////
//CObject�N���X����p�����Ă���ꍇ�͎��s���^���(RTTI)�Ƃ��āA�N���X�̌^���`���Ȃ���΂Ȃ�Ȃ�
//////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

//���b�Z�[�W�}�b�v�J�n
BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_DESTROY()

	ON_COMMAND(ID_SHOW_TOOLBAR, &CMainFrame::OnShowToolbar)
	ON_COMMAND(ID_SHOW_STATUSBAR, &CMainFrame::OnShowStatusbar)
	ON_COMMAND(ID_SHOW_PROPERTY, &CMainFrame::OnShowProperty)
	ON_COMMAND(ID_SHOW_MONITOR, &CMainFrame::OnShowMonitor)
	ON_COMMAND(ID_SHOW_PREVIEW, &CMainFrame::OnShowPreview)
	ON_COMMAND(ID_SHOW_VOLUME, &CMainFrame::OnShowVolume)

	ON_MESSAGE(WM_GRAPH_NOTIFY,&CMainFrame::OnGraphNotify)
	ON_MESSAGE(NOTIFY_TRAYICON,&CMainFrame::OnTrayIcon)
	ON_COMMAND(ID_SHOW_WINDOW, &CMainFrame::OnShowWindowFromTaskTray)

	ON_MESSAGE(WM_STOP_ENCODE,&CMainFrame::OnNotifyStopEncode)
	ON_COMMAND(ID_PLAY_ENCODE, &CMainFrame::OnStartEncode)
	ON_COMMAND(ID_STOP_ENCODE, &CMainFrame::OnStopEncode)

	ON_UPDATE_COMMAND_UI_RANGE(ID_PLAY_ENCODE, ID_STOP_ENCODE, &CMainFrame::OnUpdateEncode )

	ON_UPDATE_COMMAND_UI(ID_PROFILE_NEW, &CMainFrame::OnUpdateProfile)
	ON_UPDATE_COMMAND_UI(ID_PROFILE_OPEN, &CMainFrame::OnUpdateProfile)
END_MESSAGE_MAP()
//���b�Z�[�W�}�b�v�I��

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CMainFrame::CMainFrame()
{
	m_nSplitterColumnWidth[0] = 0;
	m_nSplitterColumnWidth[1] = 0;
	m_nSplitterRowHeight[0] = 0;
	m_nSplitterRowHeight[1] = 0;
	m_nSplitterPreviewSize[0] = 0;
	m_nSplitterPreviewSize[1] = 0;
	//m_nSplitterPreviewPos = 0;

	m_bSplitterInit = FALSE;

	m_StartStopThread = NULL;
	m_bStartStopThread = FALSE;
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CMainFrame::~CMainFrame()
{
}

//////////////////////////////////////////////////
//�E�B���h�E�������̃X�^�C����ҏW����
//////////////////////////////////////////////////
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	//�����W���̃E�B���h�E�T�C�Y
	cs.cx = 750;
	cs.cy = 580;

	//WS_OVERLAPPED�̓I�[�o���b�v �E�B���h�E�́A�ʏ�L���v�V�����Ƌ��E�������܂��B
	cs.style = WS_OVERLAPPED | WS_CAPTION | 
		FWS_ADDTOTITLE | 
		WS_MINIMIZEBOX | WS_MAXIMIZEBOX
		| WS_SYSMENU | WS_THICKFRAME;

	cs.style &= ~WS_BORDER;
	cs.dwExStyle |= WS_EX_CLIENTEDGE;

	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

//////////////////////////////////////////////////
//WM_CREATE�̃C�x���g�n���h��
//////////////////////////////////////////////////
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�E�B���h�E�T�C�Y�̕ύX
	RECT rcWindow = pSetting->m_rcMainWindow;
	if ((rcWindow.right - rcWindow.left != 0) && (rcWindow.bottom - rcWindow.top != 0))
	{
		WINDOWPLACEMENT wnd;
		ZeroMemory(&wnd,sizeof(WINDOWPLACEMENT));
		wnd.length = sizeof(WINDOWPLACEMENT);
		GetWindowPlacement(&wnd);
		wnd.length = sizeof(WINDOWPLACEMENT);
		wnd.flags = 0;
		wnd.showCmd = SW_SHOW;
		wnd.rcNormalPosition = pSetting->m_rcMainWindow;
		SetWindowPlacement(&wnd);
	}

	//�X�e�[�^�X�o�[�̍쐬
	if (!m_wndStatusBar.Create(this)) {
		TRACE0("�X�e�[�^�X �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;
	}
	//�c�[���o�[�̍쐬
	if (!m_wndToolBar.Create(this)) {
		TRACE0("�c�[�� �o�[�̍쐬�Ɏ��s���܂����B\n");
		return -1;
	}

	//�^�C�g���o�[�̕ύX
	ChangeTitleBar();

	//�E�B���h�E�A�C�R���̕ύX
	SetIcon(AfxGetApp()->LoadIcon(IDI_BLUE_OFF_ICON),TRUE);

	//�^�X�N�o�[�A�C�R�����쐬����(�\���͂܂����Ȃ�)
	m_TaskTray.Init(this);

	//��ʕ\���ݒ�̕���
	if (!pSetting->m_bShowStatusBar)
		OnShowStatusbar();
	if (!pSetting->m_bShowToolBar)
		OnShowToolbar();
	if (!pSetting->m_bShowPropertyWindow)
		OnShowProperty();
	if (!pSetting->m_bShowMonitorWindow)
		OnShowMonitor();
	if (!pSetting->m_bShowPreviewWindow)
		OnShowPreview();
	if (!pSetting->m_bShowVolumeWindow)
		OnShowVolume();

	//�t�B���^�O���t�����GraphNotify���b�Z�[�W�ʒm�̐ݒ�
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	pDSA->SetCallbackHWnd(this->GetSafeHwnd());
	return 0;
}

//////////////////////////////////////////////////
//�E�B���h�E�I���O�̏���
//////////////////////////////////////////////////
void CMainFrame::OnClose()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
#ifndef _DEBUG
	if (pDSA->IsEncode())
	{
		if (AfxMessageBox(_T("�G���R�[�h���ł����I�����܂��B\n��낵���ł����H"),MB_YESNO) == IDNO)
		{
			return;
		}
	}
#endif

	//�G���R�[�h��~��/�G���R�[�h�Đ��� �ł������ꍇ�ɂ́A��~����܂őҋ@����
	if (IsStartStopThread() == TRUE) {
		StopThreadMessageLoop();
	}

	//�G���R�[�h���ł���΃G���R�[�h��~���ďI������
	if (pDSA->IsEncode() == TRUE)
	{
		this->PreStopEncode();
		pDSA->StopEncode(TRUE);
		this->PostStopEncode();
	}

	CFrameWnd::OnClose();
}
//////////////////////////////////////////////////
//�E�B���h�E�j�����ɌĂ΂��
//////////////////////////////////////////////////
void CMainFrame::OnDestroy()
{
	//TRACE0("CMainFrame::OnDestroy()\r\n");

	//�E�B���h�E�ʒu�̕ۑ��s��
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	WINDOWPLACEMENT wpl;
	GetWindowPlacement(&wpl);
	pSetting->m_rcMainWindow = wpl.rcNormalPosition;

	//���݂̕����E�B���h�E�̏�Ԃ��擾���Ă݂�
	int dummy;
	m_wndLeftRightSplitter.GetColumnInfo(0,pSetting->m_nMainSplitterWidth[0],dummy);
	m_wndLeftRightSplitter.GetColumnInfo(1,pSetting->m_nMainSplitterWidth[1],dummy);
	m_wndTopButtomSplitter.GetRowInfo(0,pSetting->m_nMainSplitterHeight[0],dummy);
	m_wndTopButtomSplitter.GetRowInfo(1,pSetting->m_nMainSplitterHeight[1],dummy);
	m_wndPreviewSplitter.GetRowInfo(0,pSetting->m_nMainSplitterPreview[0],dummy);
	m_wndPreviewSplitter.GetRowInfo(1,pSetting->m_nMainSplitterPreview[1],dummy);

	//��ʕ\���E��\���̎擾
	pSetting->m_bShowToolBar        = IsShowView(ID_SHOW_TOOLBAR);
	pSetting->m_bShowStatusBar      = IsShowView(ID_SHOW_STATUSBAR);
	pSetting->m_bShowPropertyWindow = IsShowView(ID_SHOW_PROPERTY);
	pSetting->m_bShowMonitorWindow  = IsShowView(ID_SHOW_MONITOR);
	pSetting->m_bShowPreviewWindow  = IsShowView(ID_SHOW_PREVIEW);
	pSetting->m_bShowVolumeWindow   = IsShowView(ID_SHOW_VOLUME);

	//�^�X�N�g���C�A�C�R���̍폜
	m_TaskTray.Exit();

	CFrameWnd::OnDestroy();
}

//////////////////////////////////////////////////
// CMainFrame �f�f
//////////////////////////////////////////////////
#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG
//////////////////////////////////////////////////



//////////////////////////////////////////////////
//�^�C�g���o�[�̕ύX
//////////////////////////////////////////////////
void  CMainFrame::ChangeTitleBar()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	//�E�B���h�E�^�C�g���� KoToEncoder - �V���[�g�v���t�@�C����.kte
	CString strTitle;
	if (pSetting->m_strProfileShortPath.IsEmpty() == FALSE) {
		strTitle.Format(_T("%s - %s"),pSetting->m_strProgramName,pSetting->m_strProfileShortPath);
	} else {
		if (pSetting->m_strProfileFullPath.IsEmpty() == FALSE) {
			strTitle.Format(_T("%s - %s"),pSetting->m_strProgramName,pSetting->m_strProfileFullPath);
		} else {
			strTitle.Format(_T("%s - ��̃v���t�@�C��"),pSetting->m_strProgramName);
		}
	}
	CWnd::SetWindowText((LPCTSTR)strTitle);
	return;
}
//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL CMainFrame::ChangeProfileNotify()
{
	//DSA�ɒʒB
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (!pDSA->ChangeProfileNotify())
		return FALSE;
	//�v���p�e�B�E�B�h�E�ɒʒm
	if (!GetPropertyWnd()->ChangeProfileNotify())
		return FALSE;
	//�v���r���[�E�B���h�E�ɒʒm
	if (!GetPreviewWnd()->ChangeProfileNotify())
		return FALSE;
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���̕ۑ�
//////////////////////////////////////////////////
BOOL CMainFrame::SaveProfileSetting()
{
	//DSA�ɒʒB
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (!pDSA->SaveProfileSetting())
		return FALSE;
	//�v���p�e�B�E�B���h�E�ɒʒB
	if (!GetPropertyWnd()->SaveProfileSetting())
		return FALSE;
	//�v���r���[�E�B���h�E���̂Ƀv���t�@�C���ۑ��̐ݒ肪�Ȃ�
	//if (!GetPreviewWnd()->SaveProfileSetting())
	//	return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////
//�O���t����̒ʒm���b�Z�[�W
//////////////////////////////////////////////////
LRESULT CMainFrame::OnGraphNotify(WPARAM wParam,LPARAM lParam)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	return pDSA->OnGraphNotify();
}

//////////////////////////////////////////////////
//�^�X�N�A�C�R���̃��b�Z�[�W�ʒm�C�x���g�n���h��
//////////////////////////////////////////////////
LRESULT CMainFrame::OnTrayIcon(WPARAM wParam,LPARAM lParam)
{
	return m_TaskTray.OnTrayIcon(wParam,lParam);
}
//////////////////////////////////////////////////
//�^�X�N�g���C�A�C�R���̉E�N���b�N���j���[(�E�B���h�E�̕\��)
//////////////////////////////////////////////////
void CMainFrame::OnShowWindowFromTaskTray()
{
	SendMessage(NOTIFY_TRAYICON,TRAY_ID,WM_LBUTTONDBLCLK);
}

//////////////////////////////////////////////////
//WM_SETFOCUS�̃C�x���g�n���h��
//////////////////////////////////////////////////
void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// �r���[ �E�B���h�E�Ƀt�H�[�J�X��^���܂�
	if (m_wndPreviewWnd.m_hWnd != NULL)
		m_wndPreviewWnd.SetFocus();
}

//////////////////////////////////////////////////
//�����r���[���쐬����
//////////////////////////////////////////////////
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	//�����E�B���h�E�̍쐬
	if(!m_wndTopButtomSplitter.CreateStatic(this,2,1,WS_CHILD | WS_VISIBLE)) {
		TRACE0("�����E�B���h�E�̍쐬�Ɏ��s���܂����B\n");
		return FALSE;
	}
	if(!m_wndLeftRightSplitter.CreateStatic(&m_wndTopButtomSplitter,1, 2, WS_CHILD | WS_VISIBLE,
		m_wndTopButtomSplitter.IdFromRowCol(0,0))) {
		TRACE0("�����E�B���h�E�̍쐬�Ɏ��s���܂����B\n");
		return FALSE;
	}
	if (!m_wndPreviewSplitter.CreateStatic(&m_wndLeftRightSplitter, 2, 1, WS_CHILD | WS_VISIBLE,
		m_wndLeftRightSplitter.IdFromRowCol(0,1))) {
		TRACE0("�����E�B���h�E�̍쐬�Ɏ��s���܂����B\n");
		return FALSE;
	}

	//�㍶���̃r���[(�v���p�e�B�y�[�W)
	if (!m_wndLeftRightSplitter.CreateView(
		0, 0,
		RUNTIME_CLASS(CPropertyFormView),
		CSize(0,0),
		pContext)) {
		return FALSE;
	}

	//��E���̃r���[(�{�����[���r���[�y�[�W)
	if (!m_wndVolumeWnd.CreateEx(NULL,NULL,_T(""),
		AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0),&m_wndPreviewSplitter,m_wndPreviewSplitter.IdFromRowCol(0, 0))) {
		return FALSE;
	}
	//��E���̃r���[(�v���r���[�y�[�W)
	if (!m_wndPreviewWnd.CreateEx(NULL,NULL,_T(""),
		AFX_WS_DEFAULT_VIEW,
		CRect(0,0,0,0),&m_wndPreviewSplitter,m_wndPreviewSplitter.IdFromRowCol(1, 0))) {
		return FALSE;
	}

	//�����̃r���[(���j�^�[�y�[�W)
	if(!m_wndTopButtomSplitter.CreateView(
		1, 0,
		RUNTIME_CLASS(CMonitorFormView),
		CSize(0,0),
		pContext)) {
		return FALSE;
	}

	return CFrameWnd::OnCreateClient(lpcs, pContext);
}

//////////////////////////////////////////////////
//WM_SIZE�̃C�x���g�n���h��
//////////////////////////////////////////////////
void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	//���̃E�B���h�E���o���ĂȂ��ꍇ�͂����ɖ߂�
	if (m_wndToolBar.GetSafeHwnd() == NULL || m_wndStatusBar.GetSafeHwnd() == NULL ||
		m_wndLeftRightSplitter.GetSafeHwnd() == NULL || m_wndTopButtomSplitter.GetSafeHwnd() == NULL || m_wndPreviewSplitter.GetSafeHwnd() == NULL)
	{
		CFrameWnd::OnSize(nType, cx, cy);
		return;
	}

	//�E�B���h�E�ŏ������̏���
	if (nType == SIZE_MINIMIZED)
	{
		CFrameWnd::OnSize(nType, cx, cy);
		CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
		if (pSetting->m_bMiniTaskBar)
		{
			//�^�X�N�o�[�A�C�R����o�^����
			m_TaskTray.NotifyIcon();
			return;
		}
	}

	//�E�B���h�E�̃N���C�A���g�̈�̎擾
	CRect rcClient;
	GetClientRect(&rcClient);
	//�c�[���o�[�ƃX�e�[�^�X�o�[�̍���������
	CRect rcToolBar(0,0,0,0);
	if (m_wndToolBar.IsVisible())
		m_wndToolBar.GetWindowRect(&rcToolBar);
	CRect rcStatusBar(0,0,0,0);
	if (m_wndStatusBar.IsVisible())
		m_wndStatusBar.GetWindowRect(&rcStatusBar);

	rcClient.bottom = rcClient.bottom - rcToolBar.Height() - rcStatusBar.Height();
	if (rcClient.bottom < 0)
		rcClient.bottom = 0;
	if (rcClient.right < 0)
		rcClient.right = 0;
	//���̎��_�ŃN���C�A���g�̈�̃T�C�Y��rcClient.bottom��rcClient.right�ɓ����Ă���
	if (rcClient.right == 0 || rcClient.bottom == 0) {
		CFrameWnd::OnSize(nType, cx, cy);
		return;
	}

	//�����E�B���h�E�̐��̕����擾
	if (m_bSplitterInit == FALSE)
		CalcSplitterLine(rcClient.right,rcClient.bottom);

	//����N���̏ꍇ�̃y�C���̈��ݒ�
	if (!m_bSplitterInit)
	{
		CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
		if (pSetting->m_nMainSplitterWidth[0] == 0 || pSetting->m_nMainSplitterWidth[1] == 0 ||
			pSetting->m_nMainSplitterHeight[0] == 0 || pSetting->m_nMainSplitterHeight[1] == 0 ||
			pSetting->m_nMainSplitterPreview[0] == 0 || pSetting->m_nMainSplitterPreview[1] == 0)
		{
			//�v���p�e�B�E�B���h�E�̍ő啝
			const int nColumnMaxWidth = 280;
			if (rcClient.right >= nColumnMaxWidth + m_nSplitterLineWidth) {	//���E�ɗ]���L��
				m_nSplitterColumnWidth[0] = nColumnMaxWidth;
				m_nSplitterColumnWidth[1] = rcClient.right - nColumnMaxWidth - m_nSplitterLineWidth;
			} else {	//���E�ɗ]������
				m_nSplitterColumnWidth[0] = rcClient.right - m_nSplitterLineWidth;
				m_nSplitterColumnWidth[1] = 0;
			}
			//���j�^�[�E�B���h�E�̏����Œ�l(ini����ǂޒl)
			const int nRowDefaultHeight = 170;
			if (rcClient.bottom >= nRowDefaultHeight + m_nSplitterLineHegiht) {	//�㉺�ɗ]���L��
				m_nSplitterRowHeight[0] = rcClient.bottom - nRowDefaultHeight - m_nSplitterLineHegiht;
				m_nSplitterRowHeight[1] = nRowDefaultHeight;
			} else {	//�㉺�ɗ]������
				m_nSplitterRowHeight[0] = 0;
				m_nSplitterRowHeight[1] = rcClient.bottom - m_nSplitterLineHegiht;
			}
			//�v���r���[�E�B���h�E�̏����Œ�l(ini����ǂޒl)
			const int nPreviewDefaultHeight = 40;
			if (m_nSplitterColumnWidth[1] >= nPreviewDefaultHeight + m_nSplitterLineHegiht) {	//�㉺�ɗ]���L��
				m_nSplitterPreviewSize[0] = nPreviewDefaultHeight;
				m_nSplitterPreviewSize[1] = m_nSplitterColumnWidth[1] - nPreviewDefaultHeight - m_nSplitterLineHegiht;
			} else {	//�㉺�ɗ]������
				m_nSplitterPreviewSize[0] = m_nSplitterColumnWidth[1] - m_nSplitterLineHegiht;
				m_nSplitterPreviewSize[1] = 0;
			}
		} else {
			//�L���Ȓl�������Ă���
			m_nSplitterColumnWidth[0] = pSetting->m_nMainSplitterWidth[0];
			m_nSplitterColumnWidth[1] = pSetting->m_nMainSplitterWidth[1];
			m_nSplitterRowHeight[0] = pSetting->m_nMainSplitterHeight[0];
			m_nSplitterRowHeight[1] = pSetting->m_nMainSplitterHeight[1];
			m_nSplitterPreviewSize[0] = pSetting->m_nMainSplitterPreview[0];
			m_nSplitterPreviewSize[1] = pSetting->m_nMainSplitterPreview[1];
		}
		m_bSplitterInit = TRUE;
	}
	else
	{
		//���݂̕����E�B���h�E�̏�Ԃ��擾���Ă݂�
		int dummy;
		int nSplitterWidth[2];
		int nSplitterHeight[2];
		int nPreviewPos[2];
		m_wndLeftRightSplitter.GetColumnInfo(0,nSplitterWidth[0],dummy);
		m_wndLeftRightSplitter.GetColumnInfo(1,nSplitterWidth[1],dummy);
		m_wndTopButtomSplitter.GetRowInfo(0,nSplitterHeight[0],dummy);
		m_wndTopButtomSplitter.GetRowInfo(1,nSplitterHeight[1],dummy);
		m_wndPreviewSplitter.GetRowInfo(0,nPreviewPos[0],dummy);
		m_wndPreviewSplitter.GetRowInfo(1,nPreviewPos[1],dummy);

		if (nSplitterWidth[0] + m_nSplitterLineWidth <= rcClient.right) {
			m_nSplitterColumnWidth[0] = nSplitterWidth[0];
			m_nSplitterColumnWidth[1] = rcClient.right - nSplitterWidth[0] - m_nSplitterLineWidth;
		} else {
			m_nSplitterColumnWidth[0] = rcClient.right - m_nSplitterLineWidth;
			m_nSplitterColumnWidth[1] = 0;
		}
		if (nSplitterHeight[1] + m_nSplitterLineHegiht <= rcClient.bottom) {
			m_nSplitterRowHeight[0] = rcClient.bottom - nSplitterHeight[1] - m_nSplitterLineHegiht;
			m_nSplitterRowHeight[1] = nSplitterHeight[1];
		} else {
			m_nSplitterRowHeight[0] = 0;
			m_nSplitterRowHeight[1] = rcClient.bottom - m_nSplitterLineHegiht;
		}
		if (nPreviewPos[0] + m_nSplitterLineHegiht <= m_nSplitterColumnWidth[1]) {
			m_nSplitterPreviewSize[0] = nPreviewPos[0];
			m_nSplitterPreviewSize[1] = m_nSplitterColumnWidth[1] - nPreviewPos[0] - m_nSplitterLineHegiht;
		} else {
			m_nSplitterPreviewSize[0] = m_nSplitterColumnWidth[1] - m_nSplitterLineHegiht;
			m_nSplitterPreviewSize[1] = 0;
		}
	}
	//�y�C����ݒ肷��
	assert(m_nSplitterColumnWidth[0] >= 0);
	assert(m_nSplitterColumnWidth[1] >= 0);
	assert(m_nSplitterRowHeight[0] >= 0);
	assert(m_nSplitterRowHeight[1] >= 0);
	assert(m_nSplitterPreviewSize[0] >= 0);
	assert(m_nSplitterPreviewSize[1] >= 0);

	m_wndLeftRightSplitter.SetColumnInfo(0,m_nSplitterColumnWidth[0],0);
	m_wndLeftRightSplitter.SetColumnInfo(1,m_nSplitterColumnWidth[1],0);
	m_wndTopButtomSplitter.SetRowInfo(0,m_nSplitterRowHeight[0],0);
	m_wndTopButtomSplitter.SetRowInfo(1,m_nSplitterRowHeight[1],0);
	m_wndPreviewSplitter.SetRowInfo(0,m_nSplitterPreviewSize[0],0);
	m_wndPreviewSplitter.SetRowInfo(1,m_nSplitterPreviewSize[1],0);

	//�����E�B���h�E�ɑ΂��Ē��߂�����ɐ������ĕ\������
	//�y�C�������̃E�B���h�E�͎����I�ɕ����E�B���h�E���̑傫����
	//�g�傳��ĕ\�������̂Ŗ��Ȃ�
	m_wndLeftRightSplitter.RecalcLayout();
	m_wndTopButtomSplitter.RecalcLayout();
	m_wndPreviewSplitter.RecalcLayout();

	CFrameWnd::OnSize(nType, cx, cy);

	return;
}
//////////////////////////////////////////////////
//�����E�B���h�E�̐��̕����擾����(�T�u���[�`��)
//m_nSplitterLineWidth ���̕�
//m_nSplitterLineHegiht ���̍���
//////////////////////////////////////////////////
void CMainFrame::CalcSplitterLine(int nClientWidth,int nClientHeight)
{
	m_wndPreviewSplitter.SetRowInfo(0,0,0);
	m_wndPreviewSplitter.SetRowInfo(1,0,0);
	m_wndLeftRightSplitter.SetColumnInfo(0,0,0);
	m_wndLeftRightSplitter.SetColumnInfo(1,0,0);
	m_wndTopButtomSplitter.SetRowInfo(0,0,0);
	m_wndTopButtomSplitter.SetRowInfo(1,0,0);

	m_wndPreviewSplitter.RecalcLayout();
	m_wndLeftRightSplitter.RecalcLayout();
	m_wndTopButtomSplitter.RecalcLayout();
	int dummy;
	int nSplitterWidth;
	int nSplitterHeight;
	m_wndLeftRightSplitter.GetColumnInfo(1,nSplitterWidth,dummy);
	m_nSplitterLineWidth = nClientWidth - nSplitterWidth;
	m_wndTopButtomSplitter.GetRowInfo(1,nSplitterHeight,dummy);
	m_nSplitterLineHegiht = nClientHeight - nSplitterHeight;
	m_wndTopButtomSplitter.GetRowInfo(0,nSplitterHeight,dummy);
	//CDirectShowEtc::OutputDebugWindow(_T("m_nSplitterLineWidth  = %d\n"),m_nSplitterLineWidth);
	//CDirectShowEtc::OutputDebugWindow(_T("m_nSplitterLineHegiht = %d\n"),m_nSplitterLineHegiht);
	assert(m_nSplitterLineWidth >= 0);
	assert(m_nSplitterLineHegiht >= 0);
	return;
}
//////////////////////////////////////////////////
//�X�e�[�^�X�o�[/�c�[���o�[�̕\���Ɣ�\���̐؂�ւ�
//////////////////////////////////////////////////
void CMainFrame::Showbar(const int nMenuID)
{
	UINT uState = this->GetMenu()->GetMenuState(nMenuID,MF_BYCOMMAND);
	if (uState & MF_CHECKED) {	//�\����Ԃ����\����Ԃ�
		if (nMenuID == ID_SHOW_TOOLBAR)
			m_wndToolBar.ShowWindow(SW_HIDE);
		else if (nMenuID == ID_SHOW_STATUSBAR)
			m_wndStatusBar.ShowWindow(SW_HIDE);
		this->GetMenu()->CheckMenuItem(nMenuID,MF_BYCOMMAND|MF_UNCHECKED);
	} else {					//��\����Ԃ���\����Ԃ�
		if (nMenuID == ID_SHOW_TOOLBAR)
			m_wndToolBar.ShowWindow(SW_SHOW);
		else if (nMenuID == ID_SHOW_STATUSBAR)
			m_wndStatusBar.ShowWindow(SW_SHOW);
		this->GetMenu()->CheckMenuItem(nMenuID,MF_BYCOMMAND|MF_CHECKED);
	}
	this->RecalcLayout();	//�E�B���h�E���C�A�E�g�̍Čv�Z���s��
}

//////////////////////////////////////////////////
//�e��r���[�̕\���E��\���̐؂�ւ�
//////////////////////////////////////////////////
void CMainFrame::OnShowView(const int nMenuID,const BOOL bShow)
{
	if (bShow) {
		m_wndToolBar.PressButton(nMenuID,TRUE);
		GetMenu()->CheckMenuItem(nMenuID,MF_BYCOMMAND|MF_CHECKED);
		switch (nMenuID)
		{
		case ID_SHOW_MONITOR:
			m_wndTopButtomSplitter.ShowRow();
			break;
		case ID_SHOW_PROPERTY:
			m_wndLeftRightSplitter.ShowColumn();
			break;
		case ID_SHOW_PREVIEW:
			if (m_wndLeftRightSplitter.IsHideColumnIndex() == 1)
				m_wndLeftRightSplitter.ShowColumn();
			m_wndPreviewSplitter.ShowRow();
			if (IsShowView(ID_SHOW_VOLUME) == FALSE)
				m_wndPreviewSplitter.HideRow(0);
			break;
		case ID_SHOW_VOLUME:
			if (m_wndLeftRightSplitter.IsHideColumnIndex() == 1)
				m_wndLeftRightSplitter.ShowColumn();
			m_wndPreviewSplitter.ShowRow();
			if (IsShowView(ID_SHOW_PREVIEW) == FALSE)
				m_wndPreviewSplitter.HideRow(1);
			break;
		}
	} else {	//��\����
		m_wndToolBar.PressButton(nMenuID,FALSE);
		GetMenu()->CheckMenuItem(nMenuID,MF_BYCOMMAND|MF_UNCHECKED);
		switch (nMenuID)
		{
		case ID_SHOW_MONITOR:
			m_wndTopButtomSplitter.HideRow(1);
			break;
		case ID_SHOW_PROPERTY:
			if (m_wndLeftRightSplitter.IsShowColumn() == FALSE)
			{
				m_wndLeftRightSplitter.ShowColumn();	//�E�S�̂�\��
				OnShowView(ID_SHOW_VOLUME,TRUE);
				OnShowView(ID_SHOW_PREVIEW,TRUE);
				m_wndLeftRightSplitter.HideColumn(0);	//���̌�ɍ����B��
			} else {
				m_wndLeftRightSplitter.HideColumn(0);
			}
			break;
		case ID_SHOW_VOLUME:
			if (IsShowView(ID_SHOW_PREVIEW) == FALSE) {
				if (IsShowView(ID_SHOW_PROPERTY) == FALSE) {
					OnShowView(ID_SHOW_PROPERTY,TRUE);
				}
				m_wndLeftRightSplitter.HideColumn(1);	//�E�S�̂��B��
			} else {
				m_wndPreviewSplitter.HideRow(0);
			}
			break;
		case ID_SHOW_PREVIEW:
			if (IsShowView(ID_SHOW_VOLUME) == FALSE) {
				if (IsShowView(ID_SHOW_PROPERTY) == FALSE) {
					OnShowView(ID_SHOW_PROPERTY,TRUE);
				}
				m_wndLeftRightSplitter.HideColumn(1);	//�E�S�̂��B��
			} else {
				m_wndPreviewSplitter.HideRow(1);
			}
			break;
		}
	}
	RecalcLayout();	//�E�B���h�E���C�A�E�g�̍Čv�Z���s��
	return;
}

//////////////////////////////////////////////////
//�Đ��ƒ�~�ׂ̈̃��[�J�X���b�h
//�ʃX���b�h�Ƃ��Ď��s�����B��̊֐�
//////////////////////////////////////////////////
UINT CMainFrame::ThreadProcCalc(LPVOID pParam)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CMainFrame* frame = (CMainFrame*)pParam;
	frame->m_bStartStopThread = TRUE;	//�X���b�h�J�n

	BOOL bStart;
	if (pDSA->IsEncode())
		bStart = FALSE;
	else
		bStart = TRUE;

	//TRACE0("Call CMainFrame::ThreadProcCalc\r\n");
	if (bStart)	//�G���R�[�h��~����J�n��
	{
		if (!pDSA->StartEncode()) {
			AfxMessageBox(_T("�G���R�[�h�̊J�n�Ɏ��s���܂����B"),MB_OK|MB_ICONINFORMATION);
			pDSA->StopEncode(TRUE);
			::SendMessage(frame->GetSafeHwnd(),WM_STOP_ENCODE,0,0);
		}
	} else		//�G���R�[�h�J�n�����~��
	{
		if (!pDSA->StopEncode(FALSE)) {
			AfxMessageBox(_T("�G���R�[�h�̒�~�Ɏ��s���܂����B"),MB_OK|MB_ICONINFORMATION);
		}
		::SendMessage(frame->GetSafeHwnd(),WM_STOP_ENCODE,0,0);
	}

	frame->m_bStartStopThread = FALSE;	//�X���b�h��~
	AfxEndThread(0,TRUE);	//�X���b�h�C���X�^���X���폜����
	return 0;
}
//////////////////////////////////////////////////
//��~�̂��߂̃A�C�h�����O���[�v
//////////////////////////////////////////////////
BOOL CMainFrame::StopThreadMessageLoop()
{
	if (IsStartStopThread() == FALSE)
		return FALSE;
	//m_StartStopThread->PostThreadMessage(WM_QUIT,0,0);
	while ( IsStartStopThread() == TRUE )
	{ 
		MSG msg;
		while ( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) ) 
		{ 
			if (IsStartStopThread() == FALSE)
				break;
		} 
		LONG lIdle = 0;
		while ( AfxGetApp()->OnIdle(lIdle++ ) )
			;  
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n(�{�^��)
//////////////////////////////////////////////////
void CMainFrame::OnStartEncode()
{
	//�G���R�[�h�̊J�n
	BOOL bRet = StartEncode();
	if (bRet == FALSE)	//���s���Ă����ꍇ�͒�~
	{
		PreStopEncode();
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		pDSA->StopEncode(TRUE);
		PostStopEncode();
	}
}
//////////////////////////////////////////////////
//�G���R�[�h�̊J�n(�֐�)
//�E�B���h�E���b�Z�[�W���Ăяo�����
//////////////////////////////////////////////////
BOOL CMainFrame::StartEncode()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsEncode() || !pDSA->IsCanPreview())
		return FALSE;	//�G���R�[�h�������ł��Ă��Ȃ�

	//�e��E�B���h�E�ɃG���R�[�h�̊J�n��ʒm����
	//���̍�Ƃ́A��ɃR�����R���g���[����Disable���s��
	if (!m_wndToolBar.Encode(TRUE))
		return FALSE;
	if (!m_wndStatusBar.Encode(TRUE))
		return FALSE;

	//�v���p�e�B�E�B���h�E�ɃG���R�[�h�J�n��ʒm����
	CPropertyFormView* pPropertyWnd = this->GetPropertyWnd();
	if (!pPropertyWnd->Encode(TRUE))
		return FALSE;
	//�v���r���[�E�B���h�E�ɃG���R�[�h�J�n��ʒm����
	CPreviewWnd* pPreviewWnd = this->GetPreviewWnd();
	if (!pPreviewWnd->Encode(TRUE))
		return FALSE;
	//���j�^�[�E�B���h�E�ɃG���R�[�h�J�n��ʒm����
	CMonitorFormView* pMonitorWnd = this->GetMonitorWnd();
	if (!pMonitorWnd->Encode(TRUE))
		return FALSE;

	//�A�C�R���̕ύX
	m_TaskTray.SetIcon(IDI_RED_ON_ICON);

	//�X���b�h�𐶐�����
	if (m_StartStopThread)	//���łɃX���b�h�����݂���
	{
		//�X���b�h��~�܂őҋ@
		StopThreadMessageLoop();
	}
	m_StartStopThread = ::AfxBeginThread(CMainFrame::ThreadProcCalc,(LPVOID)this);
	if (m_StartStopThread == NULL)	//�X���b�h�������s�̎�
		return FALSE;

	return TRUE;
}
//////////////////////////////////////////////////
//�G���R�[�h�̏I��(�{�^��)
//////////////////////////////////////////////////
void CMainFrame::OnStopEncode()
{
	if (!PreStopEncode()) {
		AfxMessageBox(_T("�G���R�[�h��~�̊J�n�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return;
	}
	//�X���b�h�𐶐�����
	if (m_StartStopThread) {
		//�X���b�h��~�܂őҋ@
		StopThreadMessageLoop();
	}
	m_StartStopThread = ::AfxBeginThread(CMainFrame::ThreadProcCalc,(LPVOID)this);
	if (m_StartStopThread == NULL)
		return;
	return;
}
//////////////////////////////////////////////////
//�E�B���h�E���b�Z�[�W�����ŗ����G���R�[�h�̒�~
//////////////////////////////////////////////////
LRESULT CMainFrame::OnNotifyStopEncode(WPARAM wParam,LPARAM lParam)
{
	if (!PostStopEncode())
	{
		AfxMessageBox(_T("�G���R�[�h��~�̏I���Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return 0;
	}
	return 0;
}
//////////////////////////////////////////////////
//�G���R�[�h��~�̃v������
//���̃��b�Z�[�W�̓E�B���h�E���b�Z�[�W���ŏ�������
//////////////////////////////////////////////////
BOOL CMainFrame::PreStopEncode()
{
	//�e��E�B���h�E�ɃG���R�[�h�̃v���I����ʒm����
	if (!m_wndStatusBar.PreEncode(FALSE))
		return FALSE;
	//�^�C�}�[���g�p���Ă���E�B���h�E�ɂ��ẮA��~���߂������ōs��
	//���j�^�[�E�B���h�E�ɃG���R�[�h�I����ʒm����
	CMonitorFormView* pMonitorWnd = this->GetMonitorWnd();
	if (!pMonitorWnd->Encode(FALSE))
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h��~�̃|�X�g����
//���̃��b�Z�[�W�̓E�B���h�E���b�Z�[�W���ŏ�������
//////////////////////////////////////////////////
BOOL CMainFrame::PostStopEncode()
{
	//�e��E�B���h�E�ɃG���R�[�h�̏I����ʒm����
	if (!m_wndToolBar.Encode(FALSE))
		return FALSE;
	if (!m_wndStatusBar.Encode(FALSE))
		return FALSE;

	//�v���p�e�B�E�B���h�E�ɃG���R�[�h�I����ʒm����
	CPropertyFormView* pPropertyWnd = this->GetPropertyWnd();
	if (!pPropertyWnd->Encode(FALSE))
		return FALSE;
	//�v���r���[�E�B���h�E�ɃG���R�[�h�I����ʒm����
	CPreviewWnd* pPreviewWnd = this->GetPreviewWnd();
	if (!pPreviewWnd->Encode(FALSE))
		return FALSE;
	/*
	//���j�^�[�E�B���h�E�ɃG���R�[�h�I����ʒm����
	CMonitorFormView* pMonitorWnd = this->GetMonitorWnd();
	if (!pMonitorWnd->Encode(FALSE))
		return FALSE;
	*/
	//�A�C�R���̕ύX
	m_TaskTray.SetIcon(IDI_BLUE_OFF_ICON);

	return TRUE;
}

//////////////////////////////////////////////////
//ON_UPDATE_COMMAND_UI�́A���j���[��c�[���o�[�ɑ΂��ėL���E�����܂��́A�`�F�b�N�}�[�N�������肷��̂ɗ��p����
//���̊֐��́A���j���[��{�^���������ꂽ�Ƃ��ɔ�������̂ł͂Ȃ��A(ON_COMMAND�ł͂Ȃ��A)
//���j���[��c�[���o�[��ŃJ�[�\������������Ƃ��A���j���[�̍X�V���K�v�Ȏ��ɌĂ΂��C�x���g�n���h���ł���B
//�܂�A���̊֐����`���Ȃ�����A�L���E�����̃`�F�b�N���삪�o���Ȃ�
//////////////////////////////////////////////////
void CMainFrame::OnUpdateEncode(CCmdUI *pCmdUI)
{
	//���̓c�[���o�[�Ƀ}�E�X������������ł��������Ă��܂��Ă���Ƃ���
	if (pCmdUI->m_nID == ID_PLAY_ENCODE)
	{
		//TRACE0("ID_PLAY_ENCODE()\n");
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		BOOL bEnable = TRUE;
		if (!pDSA->IsCanEncode())
			bEnable = FALSE;
		if (IsStartStopThread())
			bEnable = FALSE;
		pCmdUI->Enable(bEnable);
	} else if (pCmdUI->m_nID == ID_STOP_ENCODE)
	{
		//TRACE0("ID_STOP_ENCODE()\n");
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		BOOL bEnable = TRUE;
		if (!pDSA->IsEncode())
			bEnable = FALSE;
		if (IsStartStopThread())
			bEnable = FALSE;
		pCmdUI->Enable(bEnable);
	}
}
//////////////////////////////////////////////////
//���j���[����(�V�K�쐬�ƊJ��)�̉�
//////////////////////////////////////////////////
void CMainFrame::OnUpdateProfile(CCmdUI *pCmdUI)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	BOOL bEnable = TRUE;
	if (pDSA->IsEncode())
		bEnable = FALSE;
	if (IsStartStopThread())
		bEnable = FALSE;
	pCmdUI->Enable(bEnable);
}
