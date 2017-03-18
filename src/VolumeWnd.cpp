// VolumeWnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "VolumeWnd.h"
#include "MainFrm.h"

// CVolumeWnd

IMPLEMENT_DYNAMIC(CVolumeWnd, CWnd)

BEGIN_MESSAGE_MAP(CVolumeWnd, CWnd)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_MESSAGE(WM_SENDDBDATA,&CVolumeWnd::OnSenddBData)
	ON_WM_SIZE()
	//ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipNotify)
	//ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipNotify)
	//ON_NOTIFY_EX(TTN_NEEDTEXT,0,CVolumeWnd::memberFxn)
	//ON_NOTIFY_EX(TTN_GETDISPINFO,0,CVolumeWnd::OnToolTip)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(ID_POPUP_ENABLEVOLUME, &CVolumeWnd::OnPopupEnableVolume)
END_MESSAGE_MAP()


//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CVolumeWnd::CVolumeWnd()
{
	m_phBackDC = NULL;
	m_phBackBitmap = NULL;

	m_ddB[0] = -90.0;
	m_ddB[1] = -90.0;
	m_rgbBlack       = RGB(0,0,0);
	m_rgbWhite       = RGB(255,255,255);

	m_rgbLightBar[0] = RGB(0,255,0);
	m_rgbLightBar[1] = RGB(255,181,24);
	m_rgbLightBar[2] = RGB(255,64,64);
	m_rgbLightBar[3] = RGB(255,64,64);

	m_rgbDarkBar[0] = RGB(0,89,0);
	m_rgbDarkBar[1] = RGB(89,63,8);
	m_rgbDarkBar[2] = RGB(89,22,22);
	m_rgbDarkBar[3] = RGB(89,22,22);

	for (int i=0;i<VOLUME_CHANNELS;i++) {
		for (int j=0;j<3;j++) {
			m_rcChannelColorBar[i][j].SetRect(0,0,0,0);
		}
		m_rcChannelBar[i].SetRect(0,0,0,0);
	}
	m_ddBLine[0] =-90.0;
	m_ddBLine[1] =-72.0;
	m_ddBLine[2] =-60.0;
	m_ddBLine[3] =-48.0;
	m_ddBLine[4] =-36.0;
	m_ddBLine[5] =-30.0;
	m_ddBLine[6] =-20.0;
	m_ddBLine[7] =-12.0;
	m_ddBLine[8] =-6.0;
	m_ddBLine[9] =-0.0;
	for (int i=0;i<VOLUME_LINE_COUNT;i++) {
		m_ndBLine[i] = 0;
	}
	/*
	for (int i=0;i<VOLUME_CHANNELS;i++) {
		for (int j=0;j<3;j++) {
			ZeroMemory(&m_DarkTriVertex[i][j][0],sizeof(TRIVERTEX));
			ZeroMemory(&m_DarkTriVertex[i][j][1],sizeof(TRIVERTEX));
			ZeroMemory(&m_LightTriVertex[i][j][0],sizeof(TRIVERTEX));
			ZeroMemory(&m_LightTriVertex[i][j][1],sizeof(TRIVERTEX));
			m_DarkTriVertex[i][j][0].Red   = GetRValue(m_rgbDarkBar[j]) << 8;
			m_DarkTriVertex[i][j][0].Green = GetGValue(m_rgbDarkBar[j]) << 8;
			m_DarkTriVertex[i][j][0].Blue  = GetBValue(m_rgbDarkBar[j]) << 8;
			m_DarkTriVertex[i][j][1].Red   = GetRValue(m_rgbDarkBar[j+1]) << 8;
			m_DarkTriVertex[i][j][1].Green = GetGValue(m_rgbDarkBar[j+1]) << 8;
			m_DarkTriVertex[i][j][1].Blue  = GetBValue(m_rgbDarkBar[j+1]) << 8;
			m_LightTriVertex[i][j][0].Red   = GetRValue(m_rgbLightBar[j]) << 8;
			m_LightTriVertex[i][j][0].Green = GetGValue(m_rgbLightBar[j]) << 8;
			m_LightTriVertex[i][j][0].Blue  = GetBValue(m_rgbLightBar[j]) << 8;
			//�ȉ��̏��͕`�掞�Ɍ���
			//m_LightTriVertex[i][j][1].Red   = GetRValue(m_rgbLightBar[j+1]) << 8;
			//m_LightTriVertex[i][j][1].Green = GetGValue(m_rgbLightBar[j+1]) << 8;
			//m_LightTriVertex[i][j][1].Blue  = GetBValue(m_rgbLightBar[j+1]) << 8;
		}
	}
	m_GradientRect.UpperLeft = 0;
	m_GradientRect.LowerRight = 1;
	*/
}
//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CVolumeWnd::~CVolumeWnd()
{
}



//////////////////////////////////////////////////
//�E�B���h�E�������̃I�v�V�����ύX
//////////////////////////////////////////////////
BOOL CVolumeWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	//�q�E�B���h�E���N���b�v����
	cs.style |= WS_CLIPCHILDREN;

	//cs.style |= CBRS_TOOLTIPS;

	cs.style &= ~CS_VREDRAW;
	cs.style &= ~CS_HREDRAW;

	//if (!CWnd::PreCreateWindow(cs))
	//	return FALSE;
	//cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, 
	//	::LoadCursor(NULL, IDC_ARROW),(HBRUSH)GetStockObject(NULL_BRUSH), NULL);
	return CWnd::PreCreateWindow(cs);
}

//////////////////////////////////////////////////
//�E�B���h�E���������ꂽ�Ƃ�(WM_CREATE)�ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
int CVolumeWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//�E�B���h�E�n���h����^����
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	pDSA->SetAudioVolumeHWnd(this->GetSafeHwnd());

	//�t�H���g�̍쐬
	m_VolumeTextFont.CreateFont(
		VOLUME_FONT_SIZE,	//�t�H���g�̍���(�傫��)
		0,					//�t�H���g�̕��B����0
		0,					//�p�x
		0,					//�������p�x
		FW_DONTCARE,		//�����̑���
		FALSE,				//�t�H���g���C�^���b�N�Ȃ�TRUE���w��
		FALSE,				//�����������Ȃ�TRUE
		FALSE,				//���������������Ȃ�TRUE
		SHIFTJIS_CHARSET,	//�t�H���g�̕����Z�b�g�B���̂܂܂�OK
		OUT_DEFAULT_PRECIS,	//�o�͐��x�̐ݒ�B���̂܂܂�OK
		CLIP_DEFAULT_PRECIS,//�N���b�s���O���x�B���̂܂܂�OK
		DRAFT_QUALITY,		//�t�H���g�̏o�͕i���B���̂܂܂�OK
		DEFAULT_PITCH,		//�t�H���g�̃s�b�`�ƃt�@�~�����w��B���̂܂܂�OK
		_T("Arial"));		// �t�H���g�̃^�C�v�t�F�C�X���̎w��B����͌����܂��

	//this->SetTimer(0,500,NULL);

	/*
	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.SetMaxTipWidth(300);
	//m_ToolTip.AddTool(this,_T("test"));
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);
	*/
	//EnableToolTips(TRUE);
	return 0;
}

//////////////////////////////////////////////////
//�E�B���h�E���j�������O�̃��b�Z�[�W
//////////////////////////////////////////////////
void CVolumeWnd::OnDestroy()
{
	CWnd::OnDestroy();

	//�t�H���g�̍폜
	m_VolumeTextFont.DeleteObject();

	//����ʂ̍폜
	DeleteBackSurface();
}
////////////////////////////////////////
//����ʂ��쐬����
////////////////////////////////////////
BOOL CVolumeWnd::CreateBackSurface(int nWidth,int nHeight)
{
	CDC* phBackDC = new CDC;
	CBitmap* phBackBitmap = new CBitmap;

	CDC* pdc = GetDC();

	phBackDC->CreateCompatibleDC(pdc);
	phBackBitmap->CreateCompatibleBitmap(pdc,nWidth,nHeight);
	phBackDC->SelectObject(phBackBitmap);

	if (m_phBackDC) {
		//TODO �����̃f�o�C�X������̂ŁABitblt�ŃR�s�[����
		DeleteBackSurface();
	}

	//�����o�ϐ���
	m_phBackDC     = phBackDC;
	m_phBackBitmap = phBackBitmap;

	ReleaseDC(pdc);
	return TRUE;
}
////////////////////////////////////////
//����ʂ��폜����
////////////////////////////////////////
BOOL CVolumeWnd::DeleteBackSurface()
{
	//����ʂ̍폜
	if (m_phBackDC) {
		m_phBackDC->DeleteDC();
		delete m_phBackDC;
		m_phBackDC = NULL;
	}
	if (m_phBackBitmap) {
		m_phBackBitmap->DeleteObject();
		delete m_phBackBitmap;
		m_phBackBitmap = NULL;
	}
	return TRUE;
}


////////////////////////////////////////
//�w�i�F�̕`���}��
////////////////////////////////////////
BOOL CVolumeWnd::OnEraseBkgnd(CDC* pDC)
{
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}

//////////////////////////////////////////////////
//WM_PAINT�������Ƃ��̃C�x���g�n���h��
//////////////////////////////////////////////////
void CVolumeWnd::OnPaint()
{
	CPaintDC dc(this);

	//�N���C�A���g�̃T�C�Y���擾
	CRect rcClient;
	GetClientRect(&rcClient);

	//����ʂɕ`����s��
	OnPaint2(*m_phBackDC);

	//�����(m_phBackDC)����\���(dc)�ɉ摜��]������
	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),m_phBackDC,0,0,SRCCOPY);
}

//////////////////////////////////////////////////
//WM_PAINT�������Ƃ��̃C�x���g�n���h��
//////////////////////////////////////////////////
void CVolumeWnd::OnPaint2(CDC &dc)
{
	CString strText;

	//�S�̂����œh��Ԃ�
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	dc.FillSolidRect(0,0,rcWindow.Width(),rcWindow.Height(),m_rgbBlack);

	//���������̕`��
	CPen cLine(PS_SOLID,1,m_rgbWhite);
	int nPrevBkMode = dc.SetBkMode(TRANSPARENT);
	CPen* pOldObject = dc.SelectObject(&cLine);
	for (int i=0;i<VOLUME_LINE_COUNT;i++) {
		dc.MoveTo(m_ndBLine[i],m_rcChannelBar[0].top    - 1);
		dc.LineTo(m_ndBLine[i],m_rcChannelBar[0].bottom + 1);
		dc.MoveTo(m_ndBLine[i],m_rcChannelBar[1].top    - 1);
		dc.LineTo(m_ndBLine[i],m_rcChannelBar[1].bottom + 1);
	}
	//�ڐ��蕶���̕`��
	CFont* pOldFont = dc.SelectObject(&m_VolumeTextFont);
	dc.SetTextColor(m_rgbWhite);
	dc.SetBkMode(TRANSPARENT);
	int nTextTop = (m_rcChannelBar[0].bottom + m_rcChannelBar[1].top) / 2 - (VOLUME_FONT_SIZE/2);
	for (int i=0;i<VOLUME_LINE_COUNT;i++) {
		strText.Format(_T("%.f"),m_ddBLine[i]);
		//�C�ӕ�����̕��������擾������@��������Ȃ������̂ŁA�����t�H���g�Ɖ��肵��length�Ŋ����Ă���
		dc.TextOut(m_ndBLine[i] - (strText.GetLength()/2 * VOLUME_FONT_SIZE /2),nTextTop,strText);

	}
	//L������R�����̕`��
	int nTextLeft = (m_rcChannelBar[0].left - (VOLUME_FONT_SIZE)) / 2;
	strText.Format(_T("L"));
	dc.TextOut(nTextLeft,(m_rcChannelBar[0].top + m_rcChannelBar[0].bottom - VOLUME_FONT_SIZE) / 2,strText);
	strText.Format(_T("R"));
	dc.TextOut(nTextLeft,(m_rcChannelBar[1].top + m_rcChannelBar[1].bottom - VOLUME_FONT_SIZE) / 2,strText);

	//�o�[(�ÐF)�̕`��
	for (int i=0;i<VOLUME_CHANNELS;i++) {
		for (int j=0;j<3;j++) {
			//dc.GradientFill(m_DarkTriVertex[i][j],2,&m_GradientRect,1,GRADIENT_FILL_RECT_H);
			dc.FillSolidRect(m_rcChannelColorBar[i][j],m_rgbDarkBar[j]);
		}
	}

	//�f�V�x���f�[�^�̎擾
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	m_ddB[0] = -90.0;
	m_ddB[1] = -90.0;
	if (pDSA->IsAudioVolumeEnabled())
	{
		int nChannels = pDSA->GetAudioVolumeChannels();
		if (nChannels >= 1)
		{
			double* pddB = new double[nChannels];
			if (pDSA->GetAudioVolumedB(pddB))
			{
				if (nChannels == 1) {
					m_ddB[0] = pddB[0];
					m_ddB[1] = pddB[0];
				} else {
					m_ddB[0] = pddB[0];
					m_ddB[1] = pddB[1];
				}
			}
			delete [] pddB;
		}

	}

	//�o�[(���F)�̕`��
	int ndB[VOLUME_CHANNELS];
	for (int i=0;i<VOLUME_CHANNELS;i++) {
		ndB[i] = GetdBPos(m_rcChannelBar[i],m_ddB[i]);
		for (int j=0;j<3;j++) 
		{
			if (ndB[i] < m_rcChannelColorBar[i][j].left)
				break;	//���̃O���f�[�V�����܂ŕ`�悵�Ȃ��ėǂ�

			/*
			//�`��̈�̌���
			if (ndB[i] < m_rcChannelColorBar[i][j].right) {	//�����`��
				m_LightTriVertex[i][j][1].x     = ndB[i];
				double dPercent = 0.0;
				if (m_LightTriVertex[i][j][1].x - m_LightTriVertex[i][j][0].x != 0)
					dPercent = ((double)ndB[i]) / ((double)m_LightTriVertex[i][j][1].x - (double)m_LightTriVertex[i][j][0].x);
				else
					break;
//					dPercent = 0.0;
				m_LightTriVertex[i][j][1].Red   = ((COLOR16)(dPercent * (GetRValue(m_rgbLightBar[j+1]) - GetRValue(m_rgbLightBar[j])))) << 8;
				m_LightTriVertex[i][j][1].Green = ((COLOR16)(dPercent * (GetGValue(m_rgbLightBar[j+1]) - GetGValue(m_rgbLightBar[j])))) << 8;
				m_LightTriVertex[i][j][1].Blue  = ((COLOR16)(dPercent * (GetBValue(m_rgbLightBar[j+1]) - GetBValue(m_rgbLightBar[j])))) << 8;
			} else {										//�S�`��
				m_LightTriVertex[i][j][1].x     = m_rcChannelColorBar[i][j].right;
				m_LightTriVertex[i][j][1].Red   = ((COLOR16)GetRValue(m_rgbLightBar[j+1])) << 8;
				m_LightTriVertex[i][j][1].Green = ((COLOR16)GetGValue(m_rgbLightBar[j+1])) << 8;
				m_LightTriVertex[i][j][1].Blue  = ((COLOR16)GetBValue(m_rgbLightBar[j+1])) << 8;
			}
			*/

			if (ndB[i] < m_rcChannelColorBar[i][j].right) {	//�����`��
				CRect rcBar;
				rcBar = m_rcChannelColorBar[i][j];
				rcBar.right = ndB[i];
				dc.FillSolidRect(rcBar,m_rgbLightBar[j]);
			} else {										//�S�`��
				dc.FillSolidRect(m_rcChannelColorBar[i][j],m_rgbLightBar[j]);
			}
			//dc.GradientFill(m_LightTriVertex[i][j],2,&m_GradientRect,1,GRADIENT_FILL_RECT_H);
		}
	}

	//�t�H���g�𕜌�
	dc.SelectObject(&pOldFont);
	//�u���V�𕜌�
	dc.SelectObject(&pOldObject);
	//BkMode�𕜌�
	dc.SetBkMode(nPrevBkMode);
}

//////////////////////////////////////////////////
//�f�V�x���f�[�^��������Ƃ��̃��b�Z�[�W�n���h��
//////////////////////////////////////////////////
LRESULT CVolumeWnd::OnSenddBData(WPARAM wParam,LPARAM lParam)
{
	InvalidateRect(NULL,FALSE);	//�`�惁�b�Z�[�W�𔭍s
	return 0;
}

//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX�ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CVolumeWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	//�T�C�Y�ύX��Ɋe��o�[�̈�̃T�C�Y�̏�����
	CRect rcWindow;
	GetWindowRect(&rcWindow);
	m_rcChannelBar[0] = GetBar(rcWindow,TRUE);
	m_rcChannelBar[1] = GetBar(rcWindow,FALSE);
	int n20dB = GetdBPos(m_rcChannelBar[0],-20.0);
	int n6dB  = GetdBPos(m_rcChannelBar[0],-6.0);
	for (int i=0;i<2;i++) {
		m_rcChannelColorBar[i][0].SetRect(
			m_rcChannelBar[i].left,m_rcChannelBar[i].top,
			n20dB,m_rcChannelBar[i].bottom);
		m_rcChannelColorBar[i][1].SetRect(
			n20dB,m_rcChannelBar[i].top,
			n6dB,m_rcChannelBar[i].bottom);
		m_rcChannelColorBar[i][2].SetRect(
			n6dB,m_rcChannelBar[i].top,
			m_rcChannelBar[i].right,m_rcChannelBar[i].bottom);
	}

	/*
	//�O���f�[�V���������X�V
	for (int i=0;i<VOLUME_CHANNELS;i++) {
		for (int j=0;j<3;j++) {
			m_DarkTriVertex[i][j][0].x = m_rcChannelColorBar[i][j].left;
			m_DarkTriVertex[i][j][0].y = m_rcChannelColorBar[i][j].top;
			m_DarkTriVertex[i][j][1].x = m_rcChannelColorBar[i][j].right;
			m_DarkTriVertex[i][j][1].y = m_rcChannelColorBar[i][j].bottom;

			m_LightTriVertex[i][j][0].x = m_rcChannelColorBar[i][j].left;
			m_LightTriVertex[i][j][0].y = m_rcChannelColorBar[i][j].top;
			//m_LightTriVertex[i][j][1].x = m_rcChannelColorBar[i][j].right;	//���̏���Draw���Ɍ���
			m_LightTriVertex[i][j][1].y = m_rcChannelColorBar[i][j].bottom;
		}
	}
	*/

	//���������̍��W���擾
	for (int i=0;i<VOLUME_LINE_COUNT;i++) {
		m_ndBLine[i] = GetdBPos(m_rcChannelBar[0],m_ddBLine[i]);
	}

	//�\��ʂ̓����傫���̃T�[�t�F�X���쐬
	CreateBackSurface(rcWindow.Width(),rcWindow.Height());

}

/*
//�_�~�[�����܁[
void CVolumeWnd::OnTimer(UINT_PTR nIDEvent)
{
	m_ddB[0] += 1.0;
	m_ddB[1] += 1.0;
	if (m_ddB[0] >= 0) {
		m_ddB[0] = -30.0;
		m_ddB[1] = -30.0;
	}
	InvalidateRect(NULL);	//�`�惁�b�Z�[�W�𔭍s
	CWnd::OnTimer(nIDEvent);
}
*/

/*
////////////////////////////////////////
//�c�[���`�b�v�̕\��
////////////////////////////////////////
BOOL CVolumeWnd::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE: 
			m_ToolTip.RelayEvent(pMsg);
			break;
	}
	return CWnd::PreTranslateMessage(pMsg);
}
BOOL CVolumeWnd::OnToolTip( UINT ID_notused, NMHDR *pNMHDR, LRESULT *Result_notused )
{
	NMTTDISPINFO* pTTT = (NMTTDISPINFO*)pNMHDR;
	pTTT->lpszText = _T("test");
	pTTT->hinst = AfxGetResourceHandle();
	return FALSE;
}
BOOL CVolumeWnd::OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
    UINT nID =pNMHDR->idFrom;
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        // idFrom is actually the HWND of the tool
        nID = ::GetDlgCtrlID((HWND)nID);
        if(nID)
        {
            pTTT->lpszText = _T("test");
            pTTT->hinst = AfxGetResourceHandle();
            //pTTT->lpszText = MAKEINTRESOURCE(nID);
            //pTTT->hinst = AfxGetResourceHandle();
            return(TRUE);
        }
    }
    return(FALSE);
}
*/

//////////////////////////////////////////////////
//�E�N���b�N�ł̃|�b�v�A�b�v�E�B���h�E��\��
//////////////////////////////////////////////////
void CVolumeWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	//�|�b�v�A�b�v�E�B���h�E��\��
	CMenu cMenu;
	cMenu.LoadMenu(IDR_VOLUME_POPUP);
	CMenu* pPopup = cMenu.GetSubMenu(0); assert(pPopup);
	POINT pt;
	GetCursorPos(&pt);
	SetForegroundWindow();

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsAudioVolumeEnabled())
		pPopup->CheckMenuItem(ID_POPUP_ENABLEVOLUME,MF_CHECKED|MF_BYCOMMAND);

	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	cMenu.DestroyMenu();

	CWnd::OnRButtonDown(nFlags, point);
}
//////////////////////////////////////////////////
//�{�����[���̗L���E����
//////////////////////////////////////////////////
void CVolumeWnd::OnPopupEnableVolume()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	BOOL bEnable = !pDSA->IsAudioVolumeEnabled();

	//
	pDSA->AudioVolumeEnableCapture(bEnable);

	//�ݒ�t�@�C���ɂ������o��
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	pSetting->m_bEnableAudioPreview = bEnable;

	InvalidateRect(NULL,FALSE);	//�`�惁�b�Z�[�W�𔭍s
	return;
}