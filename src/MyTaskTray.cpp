#include "StdAfx.h"
#include "MyTaskTray.h"
#include "KTE.h"

//////////////////////////////////////////////////
CMyTaskTray::CMyTaskTray(void)
	: m_pWnd(NULL),
	m_bid(FALSE)
{
    ZeroMemory ( &m_nid, sizeof( NOTIFYICONDATA ));
}

//////////////////////////////////////////////////
CMyTaskTray::~CMyTaskTray(void)
{
}

//////////////////////////////////////////////////
BOOL CMyTaskTray::Init(CWnd* pWnd)
{
	assert(pWnd);
	CSetting *pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	//�^�X�N�o�[�A�C�R�����쐬����(�\���͂܂����Ȃ�)
    ZeroMemory ( &m_nid, sizeof( NOTIFYICONDATA ));
    m_nid.cbSize            = sizeof( NOTIFYICONDATA );
	m_nid.hWnd              = pWnd->GetSafeHwnd();
    m_nid.uID               = TRAY_ID;
    m_nid.uFlags            = NIF_ICON | NIF_MESSAGE | NIF_TIP;
    m_nid.uCallbackMessage  = NOTIFY_TRAYICON;
    m_nid.hIcon             = (HICON)LoadImage( AfxGetInstanceHandle(), 
                        MAKEINTRESOURCE( IDI_BLUE_OFF_ICON ), IMAGE_ICON, 16, 16, 0);
	_tcscpy_s(m_nid.szTip,128,pSetting->m_strProgramName);
	m_bid = FALSE;
	m_pWnd = pWnd;
	return TRUE;
}
//////////////////////////////////////////////////
BOOL CMyTaskTray::Exit()
{
	if (m_bid)
	{
	    Shell_NotifyIcon( NIM_DELETE, &m_nid);
		m_bid = FALSE;
		m_pWnd = NULL;
	}
	return TRUE;
}

//////////////////////////////////////////////////
BOOL CMyTaskTray::NotifyIcon()
{
	assert(m_pWnd);
	//�^�X�N�o�[�A�C�R����o�^����
	Shell_NotifyIcon(NIM_ADD, &m_nid);
	m_bid = TRUE;
	m_pWnd->ShowWindow(SW_HIDE);
	return TRUE;
}

//////////////////////////////////////////////////
BOOL CMyTaskTray::SetIcon(int nIconID)
{
	assert(m_pWnd);
	HICON hIcon = AfxGetApp()->LoadIcon(nIconID);
	m_pWnd->SetIcon(hIcon,TRUE);
    m_nid.hIcon = (HICON)LoadImage( AfxGetInstanceHandle(),MAKEINTRESOURCE( IDI_RED_ON_ICON ), IMAGE_ICON, 16, 16, 0);
	return TRUE;
}

//////////////////////////////////////////////////
//�^�X�N�A�C�R���̃��b�Z�[�W�ʒm�C�x���g�n���h��
//////////////////////////////////////////////////
LRESULT CMyTaskTray::OnTrayIcon(WPARAM wParam,LPARAM lParam)
{
	if (wParam == TRAY_ID) {
		switch (lParam)
		{
		case WM_LBUTTONDBLCLK:	//���_�u���N���b�N
			{
				assert(m_pWnd);
				//�ŏ������畜�A����
				m_pWnd->ShowWindow(SW_RESTORE);
				m_pWnd->SetForegroundWindow();
				//�A�C�R�����폜����
				Shell_NotifyIcon(NIM_DELETE, &m_nid);
				m_bid = FALSE;
				break;
			}
		case WM_RBUTTONUP:	//�E�N���b�N
			{
				assert(m_pWnd);
				CMenu cMenu;
				cMenu.LoadMenu(IDR_TRAY_POPUP);
				CMenu* pPopup = cMenu.GetSubMenu(0);
				assert(pPopup);
				POINT pt;
				GetCursorPos(&pt);
				m_pWnd->SetForegroundWindow();
				pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,m_pWnd);
				cMenu.DestroyMenu();
				m_pWnd->PostMessage(WM_NULL,0,0);
			}
		}
	}
	return 0;
}