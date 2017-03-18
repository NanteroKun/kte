// PreviewWnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "PreviewWnd.h"
#include "MainFrm.h"

IMPLEMENT_DYNAMIC(CPreviewWnd, CWnd)

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CPreviewWnd::CPreviewWnd()
	:	m_dTimes(100),
		m_nTimesID(ID_POPUP_100)
{
	m_bFirstFlag = FALSE;
	m_ClientSize.SetSize(0,0);
	m_ChildSize.SetSize(0,0);
	m_ChildPos.SetPoint(0,0);
	m_bChildShow = TRUE;
	m_bFixByClient = FALSE;
	m_bFixByWindow = FALSE;

	m_nScrollBarWidth = 0;
	m_nScrollBarHeight = 0;
	m_bShowHScroll = FALSE;
	m_bShowVScroll = FALSE;
	m_bRecalcWindowCall = FALSE;

	//m_hMouseHook = NULL;

	m_rgbBack = 0;
	m_rgbLine = 0;
}
//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CPreviewWnd::~CPreviewWnd()
{
}

//���b�Z�[�W�}�b�v�J�n
BEGIN_MESSAGE_MAP(CPreviewWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_MOUSEWHEEL()
	ON_WM_SETFOCUS()
	ON_WM_RBUTTONDOWN()
	ON_COMMAND_RANGE(ID_POPUP_400, ID_POPUP_25, &CPreviewWnd::OnViewPopup)
	ON_COMMAND(ID_POPUP_FIXWINDOWSIZE, &CPreviewWnd::OnPopupFixwindowsize)
	ON_COMMAND(ID_POPUP_FIXPREVIEWSIZE, &CPreviewWnd::OnPopupFixpreviewsize)
	ON_COMMAND(ID_POPUP_SHOWPREVIEW, &CPreviewWnd::OnPopupShowpreview)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()
//���b�Z�[�W�}�b�v�I��

//////////////////////////////////////////////////
//�E�B���h�E�������̃I�v�V�����ύX
//////////////////////////////////////////////////
BOOL CPreviewWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B
	cs.style &= ~WS_BORDER;
	//cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.dwExStyle |= WS_EX_WINDOWEDGE;

	//�q�E�B���h�E�̈�̕`������O
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (pSetting->m_bPreviewForceOff == FALSE)
		cs.style |= WS_CLIPCHILDREN;


	cs.style &= ~CS_VREDRAW;
	cs.style &= ~CS_HREDRAW;

	//�X�N���[���o�[��L����
	cs.style |= WS_VSCROLL;
	cs.style |= WS_HSCROLL;

	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

//	UINT nClassStyle = CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS;
	UINT nClassStyle = CS_DBLCLKS;
	//cs.lpszClass = AfxRegisterWndClass(nClassStyle, 
	//	::LoadCursor(NULL, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), NULL);
	cs.lpszClass = AfxRegisterWndClass(nClassStyle, 
		::LoadCursor(NULL, IDC_ARROW),(HBRUSH)GetStockObject(NULL_BRUSH), NULL);

	return TRUE;
}
//////////////////////////////////////////////////
//�E�B���h�E���������ꂽ�Ƃ�(WM_CREATE)�ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
int CPreviewWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_bFirstFlag = FALSE;

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	//�X�N���[���o�[�̍����ƕ����擾(�ݒ�Œ�l�Ƃ���)
	m_nScrollBarWidth = GetSystemMetrics(SM_CXVSCROLL);
	m_nScrollBarHeight = GetSystemMetrics(SM_CYHSCROLL);

	//�X�N���[���o�[�̏����ݒ�
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;	//SIF_ALL
	ScrollInfo.nMin = 0;				//�ŏ��̃X�N���[���ʒu
	ScrollInfo.nMax = 0;				//�ő�̃X�N���[���ʒu
	ScrollInfo.nPage = ScrollInfo.nMax/10;//�y�[�W�T�C�Y(PageUp/PageDown�̈ړ���)
	ScrollInfo.nPos = 0;				//���݂̈ʒu
	//ScrollInfo.nTrackPos = 50;		//�h���b�O���̂܂݂̈ʒu(Set�̏ꍇ�͕K�v�Ȃ�)
	this->SetScrollInfo(SB_HORZ,&ScrollInfo,FALSE);
	this->SetScrollInfo(SB_VERT,&ScrollInfo,FALSE);
	m_bShowHScroll = FALSE;
	m_bShowVScroll = FALSE;

	//�r���[�p�̎q�E�B���h�E�𐶐�����
	DWORD dwStyle = WS_CHILD | WS_VISIBLE; // | WS_BORDER;
	if (!m_wndChild.Create(NULL,_T(""),dwStyle,
		CRect(0,0,0,0),this,AFX_IDW_PANE_FIRST,NULL))
	{
		TRACE0("�r���[ �E�B���h�E���쐬�ł��܂���ł����B\n");
		return -1;
	}
	m_ChildPos.SetPoint(0,0);
	m_ClientSize.SetSize(0,0);

	/*
	//�}�E�X�C�x���g�̃t�b�N���s��
	m_hMouseHook = NULL;
	m_hMouseHook = SetWindowsHookEx(
		WH_MOUSE,
		(HOOKPROC)CPreviewWnd::MyMouseHookProc,
		AfxGetInstanceHandle(),
		AfxGetApp()->m_nThreadID);
	assert(m_hMouseHook);
	*/

	//�{���̃T�C�Y�̌���
	this->m_dTimes = pSetting->m_dPreviewChildTimes;
	this->m_bFixByClient = pSetting->m_bFixByClient;
	this->m_bFixByWindow = pSetting->m_bFixByWindow;
	if (!m_bFixByWindow) {	//�C�Ӕ{���łȂ��ꍇ
		switch ((int)m_dTimes) {
		case 400:
			m_nTimesID = ID_POPUP_400; break;
		case 300:
			m_nTimesID = ID_POPUP_300; break;
		case 200:
			m_nTimesID = ID_POPUP_200; break;
		case 150:
			m_nTimesID = ID_POPUP_150; break;
		case 100:
			m_nTimesID = ID_POPUP_100; break;
		case 75:
			m_nTimesID = ID_POPUP_75; break;
		case 66:
			m_nTimesID = ID_POPUP_66; break;
		case 50:
			m_nTimesID = ID_POPUP_50; break;
		case 33:
			m_nTimesID = ID_POPUP_33; break;
		case 25:
			m_nTimesID = ID_POPUP_25; break;
		default:
			m_nTimesID = 0;
		}
	}
	//���̃^�C�~���O���Ɩ����E�B���h�E���o���Ă��Ȃ�
	//SetStatusTimes(this->m_dTimes);

	//�F�̌���
	m_rgbBack = RGB(0,0,0);
	if (pSetting->m_bPreviewForceOff == TRUE)
		m_rgbLine = RGB(255,128,0);
	else
		m_rgbLine = RGB(128,128,255);

	//�v���r���[�J�n
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsCanPreview())
	{
		SetPreviewWnd();		//�v���r���[�E�B���h�E�̊��蓖�Ă��s��
		SetPreviewSize(FALSE);

		//�v���r���[�̊J�n(�t�B���^�O���t�̎��s)
		//CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd();
		//CPropertyFormView* propertywnd =  frame->GetPropertyWnd();
		//CInputPropertyDialog* pInputTab = propertywnd->GetInputTab();
		//assert(pInputTab);
		//assert(pInputTab->GetSafeHwnd());

		BOOL bRet = pDSA->RunFilter();
		//pInputTab->OnCbnSelchangeVideoResolutionCombo();	//�𑜓x�ύX���v���r���[�̊J�n�g���K�ƂȂ�
	}

	//�\���E��\����؂�ւ���
	this->m_bChildShow = pSetting->m_bPreviewShow;
	if (!this->m_bChildShow) {
		m_wndChild.ShowWindow(SW_HIDE);
		pDSA->ShowPreviewWindow(FALSE);		//��\�����[�h�ɂ���
	}

	m_bFirstFlag = TRUE;
	return 0;
}

/*
//////////////////////////////////////////////////
//�O���[�o���}�E�X�t�b�N�C�x���g
//////////////////////////////////////////////////
LRESULT CALLBACK CPreviewWnd::MyMouseHookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	HHOOK hHook = NULL;
	CKTEApp* app = (CKTEApp*)AfxGetApp();
	assert(app);
	CMainFrame* frame = (CMainFrame*)app->GetMainWnd();
	assert(frame);
	CPreviewWnd* preview = frame->GetPreviewWnd();
	assert(preview);
	hHook = preview->GetHook();

	if (nCode < 0)
		return CallNextHookEx(hHook, nCode, wParam, lParam);

   switch (wParam)
   {
	case WM_LBUTTONDOWN:
		{
			//TRACE0("WM_LBUTTONDOWN\n");
			MOUSEHOOKSTRUCTEX *mmsg;
			mmsg = (MOUSEHOOKSTRUCTEX *)lParam;

			CPoint pt;
			pt = mmsg->pt;
			::ClientToScreen(mmsg->hwnd,&pt);

			CRect rcClient;
			preview->GetChildWnd()->GetWindowRect(&rcClient);
			if (rcClient.PtInRect(pt))
			{
				TRACE0("ClientWM_LBUTTONDOWN\n");
				//TRACE3("nCode=%d,wParam=%d,lParam=%d\n",nCode,wParam,lParam);
				return 1;
			}
			if (mmsg->hwnd == preview->GetSafeHwnd() ||
				mmsg->hwnd == preview->GetChildWnd()->GetSafeHwnd())
			{
				TRACE3("nCode=%d,wParam=%d,lParam=%d\n",nCode,wParam,lParam);
				preview->SendMessage(WM_LBUTTONDOWN,0,MAKELPARAM(mmsg->pt.x,mmsg->pt.y));
				return 1;
			}
			CRect rcClient;
			preview->GetWindowRect(&rcClient);
			CPoint pt;
			pt.x = lParam & 0xFFFF;
			pt.y = (lParam >> 16) & 0xFFFF;
			if (rcClient.PtInRect(pt))
			{
				TRACE3("nCode=%d,wParam=%d,lParam=%d\n",nCode,wParam,lParam);
				preview->SendMessage(WM_LBUTTONDOWN,wParam,lParam);
				return 1;
			}
		}
		break;
	//case WM_MOUSEWHELL:
		//TRACE3("nCode=%d,wParam=%d,lParam=%d\n",nCode,wParam,lParam);
	//	break;
   }


	//if(nCode != HC_ACTION)
	return CallNextHookEx(hHook, nCode, wParam, lParam);
}
*/

//////////////////////////////////////////////////
//�E�B���h�E���j�������O�̃��b�Z�[�W
//////////////////////////////////////////////////
void CPreviewWnd::OnDestroy()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	pSetting->m_dPreviewChildTimes = this->m_dTimes;
	pSetting->m_bFixByClient = this->m_bFixByClient;
	pSetting->m_bFixByWindow = this->m_bFixByWindow;
	pSetting->m_bPreviewShow = this->m_bChildShow;

	/*
	//�}�E�X�t�b�N���I������
	if (m_hMouseHook)
		UnhookWindowsHookEx(m_hMouseHook);
	*/

	//TRACE0("CPreviewWnd::OnDestroy()");
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	/*
	//�G���R�[�h���Ȃ�G���R�[�h���~����		�����̏����͐e�E�B���h�E�̎��_�ŏ����ς�
	if (pDSA->IsEncode()) {
		CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd();
		frame->OnStopEncode();
	}
	*/
	//�E�B���h�E�n���h������菜��
	if (pDSA->IsRunFilter()) {
		pDSA->StopFilter();
	}
	//�E�B���h�E�n���h�����폜����
	pDSA->RemovePreviewWindow();

	//TODO ���̃^�C�~���O�Ń����[�X����
	//pDSA->Exit();

	CWnd::OnDestroy();
}

//////////////////////////////////////////////////
//WM_PAINT�������Ƃ��̃C�x���g�n���h��
//////////////////////////////////////////////////
void CPreviewWnd::OnPaint() 
{
	CPaintDC dc(this);

	//�S�̂����œh��Ԃ�
	CRect rcWindow;
	this->GetWindowRect(&rcWindow);
	dc.FillSolidRect(0,0,
		rcWindow.right-rcWindow.left,
		rcWindow.bottom-rcWindow.top,
		m_rgbBack);

	//�g���̕\��
	CRect rcChild;
	m_wndChild.GetWindowRect(&rcChild);
	rcChild.MoveToXY(rcChild.left - rcWindow.left,rcChild.top - rcWindow.top);
	rcChild.left--;
	rcChild.top--;
	rcChild.right++;
	rcChild.bottom++;
	CBrush line;
    CBrush *pOldBrush;
	line.CreateSolidBrush(m_rgbLine);
	pOldBrush = dc.SelectObject(&line);
	dc.FrameRect(&rcChild,&line);	//�l�p��`��

	//��\�����̊D�F�h��Ԃ�
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (m_bChildShow == FALSE || pSetting->m_bPreviewForceOff == TRUE) {
		rcChild.left++;
		rcChild.top++;
		rcChild.right--;
		rcChild.bottom--;
		CBrush FillAreaBrush;
		FillAreaBrush.CreateSolidBrush(RGB(128,128,128));
		CBrush* Old = dc.SelectObject(&FillAreaBrush);
		dc.FillRect(&rcChild,&FillAreaBrush);
		dc.SelectObject(Old);
		FillAreaBrush.DeleteObject();
	}
	dc.SelectObject(pOldBrush);
	line.DeleteObject();
}

//////////////////////////////////////////////////
//�v���r���[�̃E�B���h�E�����蓖�Ă�
//�r�f�I�T�C�Y���ύX���ꂽ�Ƃ���InputPropertyDialog����Ă΂��
//////////////////////////////////////////////////
void CPreviewWnd::SetPreviewWnd()
{
	//�v���r���[�p�̃E�B���h�E�n���h�������蓖�Ă�
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (m_wndChild.GetSafeHwnd() != NULL)
	{
		if (pSetting->m_bPreviewForceOff == FALSE)
		{
			pDSA->SetPreviewWindow(m_wndChild.GetSafeHwnd());
		}
	}
	return;
}
//////////////////////////////////////////////////
//�v���r���[�̃T�C�Y��ύX
//�r�f�I�T�C�Y���ύX���ꂽ�Ƃ���InputPropertyDialog����Ă΂��
//���Ɗg��k���{�����Ă΂ꂽ�Ƃ��ɂ��Ă΂��
//////////////////////////////////////////////////
void CPreviewWnd::SetPreviewSize(BOOL bSizeUpdateFlag)
{
	//�����q�E�B���h�E���쐬����Ă��Ȃ��ꍇ��FALSE��Ԃ�
	if (m_wndChild.GetSafeHwnd() == NULL)
		return;

	//TRACE0("CPreviewWnd::SetPreviewSize()\n");
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	int nSrcWidth = 0;
	int nSrcHeight = 0;
	pDSA->GetCurrentVideoSize(nSrcWidth,nSrcHeight);

	assert(m_wndChild.GetSafeHwnd() != NULL);
	CSize size;
	size.cx = (int)(((double)nSrcWidth * this->m_dTimes) / 100);
	size.cy = (int)(((double)nSrcHeight * this->m_dTimes) / 100);
	//m_wndChild.MoveWindow(0,0,size.cx,size.cy,FALSE);

	m_ChildSize = size;

	//�E�B���h�E�T�C�Y�ύX
	RecalcWindowArrangement(FALSE);

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (pSetting->m_bPreviewForceOff == FALSE)
	{
		CRect rcClient;
		rcClient.SetRect(0,0,size.cx,size.cy );
		BOOL bRet = pDSA->ResizePreviewWindow(rcClient);
		//TODO ���������H
		//assert(bRet);
	}

	if (bSizeUpdateFlag && this->m_bFixByClient)
		FixWindowSize();

	SetStatusTimes(this->m_dTimes);
	
	//�E�B���h�E�̍X�V
	InvalidateRect(NULL,FALSE);
	return;
}

//////////////////////////////////////////////////
//WM_LBUTTONDOWN�������Ƃ��̃C�x���g�n���h��
//////////////////////////////////////////////////
void CPreviewWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	//TRACE0("CPreviewWnd::OnLButtonDown\n");
	SetFocus();
	CWnd::OnLButtonDown(nFlags, point);
}
//////////////////////////////////////////////////
//�}�E�X�z�C�[���̃C�x���g�n���h��
//////////////////////////////////////////////////
BOOL CPreviewWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (m_bFixByWindow == TRUE)	//�u�v���r���[�̔{�����v���C���[�̑傫���ɍ��킹��v�Ƃ��ɂ̓z�C�[���ł��Ȃ�
		return TRUE;

	UINT nID = 0;
	switch (m_nTimesID)
	{
	case ID_POPUP_400:
		nID = zDelta > 0 ? 0 : ID_POPUP_300;            break;
	case ID_POPUP_300:
		nID = zDelta > 0 ? ID_POPUP_400 : ID_POPUP_200; break;
	case ID_POPUP_200:
		nID = zDelta > 0 ? ID_POPUP_300 : ID_POPUP_150; break;
	case ID_POPUP_150:
		nID = zDelta > 0 ? ID_POPUP_200 : ID_POPUP_100; break;
	case ID_POPUP_100:
		nID = zDelta > 0 ? ID_POPUP_150 : ID_POPUP_75;  break;
	case ID_POPUP_75:
		nID = zDelta > 0 ? ID_POPUP_100 : ID_POPUP_66;  break;
	case ID_POPUP_66:
		nID = zDelta > 0 ? ID_POPUP_75 : ID_POPUP_50;   break;
	case ID_POPUP_50:
		nID = zDelta > 0 ? ID_POPUP_66 : ID_POPUP_33;   break;
	case ID_POPUP_33:
		nID = zDelta > 0 ? ID_POPUP_50 : ID_POPUP_25;  break;
	case ID_POPUP_25:
		nID = zDelta > 0 ? ID_POPUP_33 : 0; break;
	default:
		{
			//�C�ӃT�C�Y����T�C�Y�w��Ɉڍs�����ꍇ
			double dTimes = this->m_dTimes;
			if (dTimes >= 400) {
				nID = zDelta > 0 ? 0 : ID_POPUP_400;            break;
			} else if (dTimes > 300) {
				nID = zDelta > 0 ? ID_POPUP_400 : ID_POPUP_300;            break;
			} else if (dTimes > 200) {
				nID = zDelta > 0 ? ID_POPUP_300 : ID_POPUP_200;            break;
			} else if (dTimes > 150) {
				nID = zDelta > 0 ? ID_POPUP_200 : ID_POPUP_150;            break;
			} else if (dTimes > 100) {
				nID = zDelta > 0 ? ID_POPUP_150 : ID_POPUP_100;            break;
			} else if (dTimes > 75) {
				nID = zDelta > 0 ? ID_POPUP_100 : ID_POPUP_75;            break;
			} else if (dTimes > 66) {
				nID = zDelta > 0 ? ID_POPUP_75 : ID_POPUP_66;            break;
			} else if (dTimes > 50) {
				nID = zDelta > 0 ? ID_POPUP_66 : ID_POPUP_50;            break;
			} else if (dTimes > 33) {
				nID = zDelta > 0 ? ID_POPUP_50 : ID_POPUP_33;            break;
			} else if (dTimes > 25) {
				nID = zDelta > 0 ? ID_POPUP_33 : ID_POPUP_25;            break;
			} else {
				nID = zDelta > 0 ? ID_POPUP_25 : 0; break;
			}
		}
	}
	if (nID != 0)
		OnViewPopup(nID);
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//////////////////////////////////////////////////
//�����X�N���[���o�[
//////////////////////////////////////////////////
void CPreviewWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO info;
	this->GetScrollInfo(SB_HORZ,&info);
	int nMax = info.nMax - info.nPage + 1;
	int pos = info.nPos;
	switch (nSBCode)
	{
	case SB_LEFT:		//���[�փX�N���[��
		pos = info.nMin; break;
	case SB_RIGHT:		//�E�[�փX�N���[��
		pos = info.nMax; break;
	case SB_LINELEFT:	//���փX�N���[��
		pos -= 1; break;
	case SB_LINERIGHT:	//�E�փX�N���[��
		pos += 1; break;
	case SB_PAGELEFT:	//�P�y�[�W���փX�N���[��
		pos -= info.nPage; break;
	case SB_PAGERIGHT:	//�P�y�[�W�E�փX�N���[��
		pos += info.nPage; break;
	case SB_THUMBPOSITION:	//��Έʒu�փX�N���[��(nPos�L��)
	case SB_THUMBTRACK:		//�X�N���[�� �{�b�N�X���w��ʒu�փh���b�O(nPos�L��)
		pos = nPos; break;
	case SB_ENDSCROLL:	//�X�N���[���I��
		break;
	}
	if (pos < 0) pos = 0;
	if (pos > nMax) pos = nMax;
	this->SetScrollPos(SB_HORZ,pos,TRUE);
	MoveChildWindow();
	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}
//////////////////////////////////////////////////
//�����X�N���[���o�[
//////////////////////////////////////////////////
void CPreviewWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	SCROLLINFO info;
	this->GetScrollInfo(SB_VERT,&info);
	int nMax = info.nMax - info.nPage + 1;
	int pos = info.nPos;
	switch (nSBCode)
	{
	case SB_TOP:		//��ԏ�܂ŃX�N���[��
		pos = info.nMin; break;
	case SB_BOTTOM:		//��ԉ��܂ŃX�N���[��
		pos = info.nMax; break;
	case SB_LINEDOWN:	//��s���փX�N���[��
		pos += 1; break;
	case SB_LINEUP:		//��s��փX�N���[��
		pos -= 1; break;
	case SB_PAGEDOWN:	//�P�y�[�W���փX�N���[��
		pos += info.nPage; break;
	case SB_PAGEUP:		//�P�y�[�W��փX�N���[��
		pos -= info.nPage; break;
	case SB_THUMBPOSITION:	//��Έʒu�փX�N���[��(nPos�L��)
	case SB_THUMBTRACK:		//�X�N���[�� �{�b�N�X���w��ʒu�փh���b�O(nPos�L��)
		pos = nPos; break;
	case SB_ENDSCROLL:	//�X�N���[���I��
		break;
	}
	if (pos < 0) pos = 0;
	if (pos > nMax) pos = nMax;
	this->SetScrollPos(SB_VERT,pos,TRUE);
	MoveChildWindow();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}

//////////////////////////////////////////////////
//�X�N���[�������ۂ̃��W���[��
//////////////////////////////////////////////////
void CPreviewWnd::MoveChildWindow()
{
	CPoint pt(0,0);
	if (this->m_bShowHScroll)
		pt.x = -this->GetScrollPos(SB_HORZ);
	else
		pt.x = m_ChildPos.x;
	if (this->m_bShowVScroll)
		pt.y = -this->GetScrollPos(SB_VERT);
	else
		pt.y = m_ChildPos.y;
	m_wndChild.MoveWindow(m_ChildPos.x,m_ChildPos.y,m_ChildSize.cx,m_ChildSize.cy,TRUE);
	m_ChildPos = pt;
	return;
}

//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX���ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CPreviewWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	RecalcWindowArrangement(TRUE);
}
//////////////////////////////////////////////////
//�E�B���h�E�z�u�̌v�Z
//BOOL bOnSize �T�C�Y�ύX����Ă΂ꂽ���ǂ���
//////////////////////////////////////////////////
BOOL CPreviewWnd::RecalcWindowArrangement(BOOL bOnSize)
{
	//�E�B���h�E�z�u�̌v�Z���J�n
	if (m_bRecalcWindowCall)
		return TRUE;
	m_bRecalcWindowCall	= TRUE;

	//�N���C�A���g�̃T�C�Y�̓X�N���[���o�[���܂߂��傫�����w��
	CRect rcClient;
	GetClientRect(&rcClient);
	if (m_bShowHScroll)
		rcClient.bottom += m_nScrollBarHeight;
	if (m_bShowVScroll)
		rcClient.right += m_nScrollBarWidth;

	//�v���r���[�E�B���h�E�̔{����ύX����
	FixPreviewTimes(rcClient.right,rcClient.bottom);

	BOOL bUseHScroll;		//�����X�N���[���o�[�͕K�v���ǂ���
	BOOL bUseVScroll;		//�����X�N���[���o�[�͕K�v���ǂ���
	CSize diff;
	diff.cx = rcClient.right - m_ChildSize.cx;
	diff.cy = rcClient.bottom - m_ChildSize.cy;
	if (diff.cx < 0)	//�K�������X�N���[���o�[���K�v
	{
		bUseHScroll = TRUE;
		if (diff.cy < 0)	//�K�������X�N���[���o�[���K�v
		{
			bUseVScroll = TRUE;
		} else if (diff.cy < m_nScrollBarWidth)	//�ꍇ�ɂ���Ă͐����X�N���[���o�[���K�v
		{
			bUseVScroll = TRUE;
		} else				//�����X�N���[���o�[�͕s�v
		{
			bUseVScroll = FALSE;
		}
	} else if (diff.cx < m_nScrollBarWidth)		//�ꍇ�ɂ���Ă͐����X�N���[���o�[���K�v
	{
		if (diff.cy < 0)	//�K�������X�N���[���o�[���K�v
		{
			bUseHScroll = TRUE;
			bUseVScroll = TRUE;
		} else if (diff.cy < m_nScrollBarWidth)	//�ꍇ�ɂ���Ă͐����X�N���[���o�[���K�v
		{
			bUseHScroll = FALSE;
			bUseVScroll = FALSE;
		} else				//�����X�N���[���o�[�͕s�v
		{
			bUseHScroll = FALSE;
			bUseVScroll = FALSE;
		}
	}
	else	//�����X�N���[���o�[�͕s�v
	{
		bUseHScroll = FALSE;
		if (diff.cy < 0)	//�K�������X�N���[���o�[���K�v
		{
			bUseVScroll = TRUE;
		} else if (diff.cy < m_nScrollBarWidth)	//�ꍇ�ɂ���Ă͐����X�N���[���o�[���K�v
		{
			bUseVScroll = FALSE;
		}
		else				//�����X�N���[���o�[�͕s�v
		{
			bUseVScroll = FALSE;
		}
	}
	//�X�N���[���o�[���K�v�ȏꍇ�̓N���C�A���g�T�C�Y���ύX�ɂȂ�
	if (bUseHScroll)
		rcClient.bottom -= m_nScrollBarHeight;
	if (bUseVScroll)
		rcClient.right -= m_nScrollBarWidth;
	//���̃^�C�~���O�ŃX�N���[���o�[�̗L�����܂߂��N���C�A���g�̈�̃T�C�Y�����肳���

	//�q�E�B���h�E�̍��W(�N���C�A���g���W)
	CPoint pos(0,0);
	//�X�N���[���o�[�\���̂̏�����
	SCROLLINFO ScrollInfo;
	ScrollInfo.cbSize = sizeof(SCROLLINFO);
	ScrollInfo.fMask = SIF_RANGE | SIF_PAGE | SIF_POS | SIF_TRACKPOS;	//SIF_ALL
	ScrollInfo.nMin = 0;				//�ŏ��̃X�N���[���ʒu
	if (bUseHScroll)
	{
		ScrollInfo.nMax = m_ChildSize.cx;	//�ő�̃X�N���[���ʒu
		ScrollInfo.nPage = rcClient.right;	//�y�[�W�T�C�Y(PageUp/PageDown�̈ړ���)
		if (m_bShowHScroll)	//�ȑO���X�N���[���o�[���������ꍇ
		{
			ScrollInfo.nPos = - m_ChildPos.x;
			if (ScrollInfo.nPos + (int)ScrollInfo.nPage > ScrollInfo.nMax)
				ScrollInfo.nPos = ScrollInfo.nMax - ScrollInfo.nPage;
		} else {
			ScrollInfo.nPos = 0;//(m_ChildSize.cx - rcClient.right) / 2;				//���݂̈ʒu
		}
		this->SetScrollInfo(SB_HORZ,&ScrollInfo,FALSE);
		m_bShowHScroll = TRUE;
		pos.x = - ScrollInfo.nPos;
	} else {
		ScrollInfo.nMax = 0;				//�ő�̃X�N���[���ʒu
		ScrollInfo.nPage = 0;				//�y�[�W�T�C�Y(PageUp/PageDown�̈ړ���)
		ScrollInfo.nPos = 0;				//���݂̈ʒu
		this->SetScrollInfo(SB_HORZ,&ScrollInfo,FALSE);
		m_bShowHScroll = FALSE;
		pos.x = (rcClient.right - m_ChildSize.cx) / 2;	//�����Ɉړ�
	}

	if (bUseVScroll)
	{
		ScrollInfo.nMax = m_ChildSize.cy;	//�ő�̃X�N���[���ʒu
		ScrollInfo.nPage = rcClient.bottom;	//�y�[�W�T�C�Y(PageUp/PageDown�̈ړ���)
		if (m_bShowVScroll)
		{
			ScrollInfo.nPos = - m_ChildPos.y;
			if (ScrollInfo.nPos + (int)ScrollInfo.nPage > ScrollInfo.nMax)
				ScrollInfo.nPos = ScrollInfo.nMax - ScrollInfo.nPage;
		} else {
			ScrollInfo.nPos = 0;//(m_ChildSize.cy - rcClient.bottom) / 2;				//���݂̈ʒu
		}
		this->SetScrollInfo(SB_VERT,&ScrollInfo,FALSE);
		m_bShowVScroll = TRUE;
		pos.y = - ScrollInfo.nPos;
	} else {
		ScrollInfo.nMax = 0;				//�ő�̃X�N���[���ʒu
		ScrollInfo.nPage = 0;				//�y�[�W�T�C�Y(PageUp/PageDown�̈ړ���)
		ScrollInfo.nPos = 0;				//���݂̈ʒu
		this->SetScrollInfo(SB_VERT,&ScrollInfo,FALSE);
		m_bShowVScroll = FALSE;
		pos.y = (rcClient.bottom - m_ChildSize.cy) / 2;	//�����Ɉړ�
	}

	m_wndChild.MoveWindow(pos.x,pos.y,m_ChildSize.cx,m_ChildSize.cy,FALSE);
	m_ChildPos = pos;	//���݂̎q�v���r���[�E�B���h�E�̈ʒu���L��
	m_ClientSize.SetSize(rcClient.right,rcClient.bottom);

	//��ʑS�̂̍X�V�ʒm���s��
	//TODO �������R�����g�A�E�g����
	//InvalidateRect(NULL, FALSE);

	m_bRecalcWindowCall	= FALSE;
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL CPreviewWnd::ChangeProfileNotify()
{
	m_bFirstFlag = FALSE;

	//�v���r���[�J�n
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsCanPreview())
	{
		SetPreviewWnd();		//�v���r���[�E�B���h�E�̊��蓖�Ă��s��
		SetPreviewSize(FALSE);

		//�v���r���[�̊J�n(�t�B���^�O���t�̎��s)
		//CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd();
		//CPropertyFormView* propertywnd =  frame->GetPropertyWnd();
		//CInputPropertyDialog* pInputTab = propertywnd->GetInputTab();
		//assert(pInputTab);
		//assert(pInputTab->GetSafeHwnd());

		BOOL bRet = pDSA->RunFilter();
		//pInputTab->OnCbnSelchangeVideoResolutionCombo();	//�𑜓x�ύX���v���r���[�̊J�n�g���K�ƂȂ�
	}
	m_bFirstFlag = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL CPreviewWnd::Encode(BOOL bStart)
{
	return TRUE;
}
//////////////////////////////////////////////////
//�t�H�[�J�X�̐ݒ�
//////////////////////////////////////////////////
void CPreviewWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
}
//////////////////////////////////////////////////
//�E�N���b�N�ł̊g��k�����j���[��\��
//////////////////////////////////////////////////
void CPreviewWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CMenu cMenu;
	cMenu.LoadMenu(IDR_VIEW_POPUP);
	CMenu* pPopup = cMenu.GetSubMenu(0);
	assert(pPopup);
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow();

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (pSetting->m_bPreviewForceOff == TRUE)
		pPopup->EnableMenuItem(ID_POPUP_SHOWPREVIEW,MF_GRAYED | MF_BYCOMMAND);

	if (m_bChildShow)
		pPopup->CheckMenuItem(ID_POPUP_SHOWPREVIEW,MF_CHECKED|MF_BYCOMMAND);
	if (m_bFixByClient)
		pPopup->CheckMenuItem(ID_POPUP_FIXWINDOWSIZE,MF_CHECKED|MF_BYCOMMAND);

	if (m_bFixByWindow)
	{
		pPopup->CheckMenuItem(ID_POPUP_FIXPREVIEWSIZE,MF_CHECKED|MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_400,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_300,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_200,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_150,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_100,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_75,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_66,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_50,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_33,MF_GRAYED | MF_BYCOMMAND);
		pPopup->EnableMenuItem(ID_POPUP_25,MF_GRAYED | MF_BYCOMMAND);
	} else {
		if (m_nTimesID != 0)
			pPopup->CheckMenuItem(m_nTimesID,MF_CHECKED|MF_BYCOMMAND);
	}

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	cMenu.DestroyMenu();

	CWnd::OnRButtonDown(nFlags, point);
}
//////////////////////////////////////////////////
//�v���r���[�̕\���̔�\����؂�ւ���(�|�b�v�A�b�v���j���[)
//////////////////////////////////////////////////
void CPreviewWnd::OnPopupShowpreview()
{
	if (m_wndChild.GetSafeHwnd() == NULL)
		return;
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (m_bChildShow) {
		m_wndChild.ShowWindow(SW_HIDE);
		m_bChildShow = FALSE;
		pDSA->ShowPreviewWindow(FALSE);
	} else {
		m_wndChild.ShowWindow(SW_SHOW);
		m_bChildShow = TRUE;
		pDSA->ShowPreviewWindow(TRUE);
	}
}
//////////////////////////////////////////////////
//�g��k���̉E�N���b�N���j���[��I������
//////////////////////////////////////////////////
void CPreviewWnd::OnViewPopup(UINT nID)
{
	switch (nID)
	{
	case ID_POPUP_400:
		m_dTimes = 400;break;
	case ID_POPUP_300:
		m_dTimes = 300;break;
	case ID_POPUP_200:
		m_dTimes = 200;break;
	case ID_POPUP_150:
		m_dTimes = 150;break;
	case ID_POPUP_100:
		m_dTimes = 100;break;
	case ID_POPUP_75:
		m_dTimes = 75;break;
	case ID_POPUP_66:
		m_dTimes = 66;break;
	case ID_POPUP_50:
		m_dTimes = 50;break;
	case ID_POPUP_33:
		m_dTimes = 33;break;
	case ID_POPUP_25:
		m_dTimes = 25;break;
	};
	m_nTimesID = nID;
	SetPreviewSize(TRUE);
	return;
}
//////////////////////////////////////////////////
//���݂̃v���r���[�E�B���h�E���Y��Ɏ��܂�悤��
//�e�E�B���h�E�T�C�Y��ύX����
//////////////////////////////////////////////////
void CPreviewWnd::OnPopupFixwindowsize()
{
	if (m_bFixByClient)
		m_bFixByClient = FALSE;
	else
	{
		m_bFixByClient = TRUE;
		m_bFixByWindow = FALSE;	//��������
		SetPreviewSize(TRUE);
	}
}
//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�Ɏ��܂�悤�Ƀv���r���[�̑傫����ύX����
//////////////////////////////////////////////////
void CPreviewWnd::OnPopupFixpreviewsize()
{
	if (m_bFixByWindow)
		m_bFixByWindow = FALSE;
	else
	{
		m_bFixByWindow = TRUE;
		m_bFixByClient = FALSE;	//��������
		m_nTimesID = 0;
		RecalcWindowArrangement(TRUE);
		//FixPreviewTimes(m_ClientSize.cx,m_ClientSize.cy);
	}
}


//////////////////////////////////////////////////
//MainFrame���v���r���[�T�C�Y�ɍ��킹��
//////////////////////////////////////////////////
void CPreviewWnd::FixWindowSize()
{
	CWnd* pParent = ::AfxGetMainWnd();
	if (!pParent)
		return;
	if (!pParent->GetSafeHwnd())
		return;
	if (!this->m_bFirstFlag)	//����N�����ɂ͂��̐ݒ�͓K�p���Ȃ�
		return;
	//assert(pParent);
	CRect rcParentWindowRect;
	pParent->GetWindowRect(&rcParentWindowRect);

	CSize diff(0,0);	//����������N���C�A���g�̈�̃T�C�Y
	if (m_ChildPos.x < 0)
		diff.cx = m_ChildSize.cx - m_ClientSize.cx;
	else
		diff.cx  = m_ClientSize.cx - m_ChildSize.cx;
	if (this->m_bShowVScroll)
		diff.cx += this->m_nScrollBarWidth;

	if (m_ChildPos.y < 0)
		diff.cy = m_ChildSize.cy - m_ClientSize.cy;
	else
		diff.cy  = m_ClientSize.cy - m_ChildSize.cy;
	if (this->m_bShowHScroll)
		diff.cy += this->m_nScrollBarHeight;

	rcParentWindowRect.right = rcParentWindowRect.right - diff.cx;
	rcParentWindowRect.bottom = rcParentWindowRect.bottom - diff.cy;
	pParent->MoveWindow(&rcParentWindowRect,TRUE);
	//pParent->InvalidateRect(NULL,TRUE);
	//pParent->UpdateWindow();
}

//////////////////////////////////////////////////
//�v���r���[�̊g��{�������݂̃v���C���[�T�C�Y�Ƃ҂�����ɂȂ�悤�ɒ�������
//////////////////////////////////////////////////
void CPreviewWnd::FixPreviewTimes(int nClientWidth ,int nClientHeight)
{
	if (m_bFixByWindow)
	{
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		int nWidth = 0;
		int nHeight = 0;
		pDSA->GetCurrentVideoSize(nWidth,nHeight);
		if (nWidth == 0 || nHeight == 0) {
			//AfxMessageBox(_T("�r�f�I�̉𑜓x��0�ł�"),MB_OK|MB_ICONINFORMATION);
			return;
		}
		//���̉𑜓x
		double dWidthTimes = ((double)nClientWidth / (double)nWidth);
		double dHeightTimes = ((double)nClientHeight / (double)nHeight);
		double dTimes = dWidthTimes > dHeightTimes ? dHeightTimes : dWidthTimes;
		if (dTimes == 0.0) {
			//AfxMessageBox(_T("�r�f�I�̔{����0�ł�"),MB_OK|MB_ICONINFORMATION);
			return;
		}

		//���̔{���l��ύX����
		CSize size;
		size.cx = (int) ((double)nWidth * dTimes);
		size.cy = (int) ((double)nHeight * dTimes);
		if (size.cx == 0 || size.cy == 0) {
			//AfxMessageBox(_T("�v���r���[�̃T�C�Y��0�ł�"),MB_OK|MB_ICONINFORMATION);
			return;
		}

		this->m_dTimes = dTimes;
		m_ChildSize.cx = size.cx;
		m_ChildSize.cy = size.cy;
		m_wndChild.MoveWindow(0,0,m_ChildSize.cx,m_ChildSize.cy,FALSE);
		CRect rcWindow;
		rcWindow.SetRect(0,0,m_ChildSize.cx,m_ChildSize.cy);
		pDSA->ResizePreviewWindow(rcWindow);

		this->m_dTimes = this->m_dTimes * 100;
		SetStatusTimes(this->m_dTimes);
		InvalidateRect(NULL,FALSE);
	}
}

//////////////////////////////////////////////////
//�X�e�[�^�X�o�[�Ɋg��{������������
//////////////////////////////////////////////////
void CPreviewWnd::SetStatusTimes(double dTimes)
{
	CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd();
	if (!frame) return;
	CMyStatusBar* status = frame->GetStatusBar();
	if (!status) return;
	status->SetTimes(dTimes);
	return;
}


BOOL CPreviewWnd::OnEraseBkgnd(CDC* pDC)
{
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}
