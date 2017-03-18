#include "StdAfx.h"
#include "MyToolBar.h"

#include "Resource.h"


//IMPLEMENT_DYNAMIC(CMyToolBar, CToolBar)

//BEGIN_MESSAGE_MAP(CMyToolBar, CToolBar)
//END_MESSAGE_MAP()


//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CMyToolBar::CMyToolBar(void)
{
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CMyToolBar::~CMyToolBar(void)
{
}
//////////////////////////////////////////////////
//
//////////////////////////////////////////////////
BOOL CMyToolBar::PreCreateWindow(CREATESTRUCT& cs)
{
	return CToolBar::PreCreateWindow(cs);
}
//////////////////////////////////////////////////
//�c�[���o�[�̍쐬
//////////////////////////////////////////////////
BOOL CMyToolBar::Create(CFrameWnd* pFrameWnd)
{

	DWORD dwCrtlStlye = TBSTYLE_FLAT | TBSTYLE_LIST | TBSTYLE_TOOLTIPS ;
	DWORD dwStyle = WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC;

	if (!CreateEx(pFrameWnd, dwCrtlStlye , dwStyle ))
		return FALSE;      // �쐬�ł��܂���ł����B
	if (!LoadToolBar(IDR_MAINFRAME))
		return FALSE;

	//�c�[���o�[�̃C���[�W���X�g��������
    CImageList* pImgList = this->GetToolBarCtrl().GetImageList();
    pImgList->DeleteImageList();
	//�C���[�W���X�g��8Bit(256�F)�ō쐬
    int nBtnCnt = 5; //�{�^���̐�
	//pImgList->Create(28,26, ILC_COLOR24 | ILC_MASK, nBtnCnt, nBtnCnt);
	pImgList->Create(16,15, ILC_COLOR24 | ILC_MASK, nBtnCnt, nBtnCnt);
    //�r�b�g�}�b�v��ǂݍ���
    CBitmap bmp;
    //bmp.LoadBitmap(IDB_TOOLBAR256);
    bmp.LoadBitmap(IDB_TOOLBAR_SMALL);
    //�C���[�W���X�g�ɒǉ�����B
    //���ߐF���Q�Ԗڂ̈����Őݒ肵�Ă���B
    pImgList->Add(&bmp, RGB(255, 0, 255));
    //�A�C�R����ToolBar�Ɋ��蓖�Ă�
    this->GetToolBarCtrl().SetImageList(pImgList);
    bmp.DeleteObject();

    int buttonNum = 5;		//�{�^����
    //int buttonWidth = 28;	//�{�^���̕�
    //int buttonHeight = 26;	//�{�^���̍���
    int buttonWidth = 16;	//�{�^���̕�
    int buttonHeight = 15;	//�{�^���̍���

    //�c�[���o�[�̃T�C�Y�ݒ�
    CRect temp;
    this->GetItemRect(0,&temp);
    this->SetSizes(CSize(temp.Width(),
		temp.Height()),CSize(buttonWidth, buttonHeight));

    //�{�^�����̐ݒ�
    this->SetButtons( NULL, 7 );
    this->SetButtonInfo(0, ID_SHOW_PROPERTY, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 0 );
    this->SetButtonInfo(1, ID_SHOW_MONITOR, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 1 );
    this->SetButtonInfo(2, ID_SHOW_PREVIEW, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 2 );
    this->SetButtonInfo(3, ID_SHOW_VOLUME, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 3 );
	this->SetButtonInfo(4, ID_SEPARATOR, TBBS_SEPARATOR, 12 );
    this->SetButtonInfo(5, ID_PLAY_ENCODE, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 4 );
    this->SetButtonInfo(6, ID_STOP_ENCODE, TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE , 5 );

	this->SetButtonText(0,_T("�v���p�e�B"));
	this->SetButtonText(1,_T("���j�^�["));
	this->SetButtonText(2,_T("�v���r���["));
	this->SetButtonText(3,_T("�{�����[��"));
	this->SetButtonText(5,_T("�G���R�[�h�̊J�n"));
	this->SetButtonText(6,_T("��~"));

	//�c�[���`�b�v��z�u����
	CToolTipCtrl* tips = this->GetToolBarCtrl().GetToolTips();
	//tips->AddTool(this->GetToolBarCtrl().GetDlgItem(ID_PLAY_ENCODE),ID_PLAY_ENCODE);
	//tips->AddTool(this->GetToolBarCtrl().GetDlgItem(ID_STOP_ENCODE),ID_STOP_ENCODE);

	//�{�^���̖�����(�����ON_UPDATE_COMMAND_UI�����Ă���Ă�͂�)
	//this->GetToolBarCtrl().EnableButton(ID_STOP_ENCODE,FALSE);

	//�T�C�Y�𒲐�����
	this->GetToolBarCtrl().AutoSize();

	//�{�^����������Ԃɂ���
	PressButton(ID_SHOW_PROPERTY,TRUE);
	PressButton(ID_SHOW_MONITOR,TRUE);
	PressButton(ID_SHOW_PREVIEW,TRUE);
	PressButton(ID_SHOW_VOLUME,TRUE);

	return TRUE;
}

//�{�^������������Ԃɂ���
BOOL CMyToolBar::PressButton(int nID,BOOL bPress)
{
    CToolBarCtrl &rcToorBarCtrl = this->GetToolBarCtrl();
	rcToorBarCtrl.PressButton(nID,bPress);
	return TRUE;
}


//�G���R�[�h�̊J�n�ƏI��
BOOL CMyToolBar::Encode(BOOL bStart)
{
	/*
    CToolBarCtrl &rcToorBarCtrl = this->GetToolBarCtrl();
	rcToorBarCtrl.EnableButton(ID_PLAY_ENCODE,bStart? FALSE : TRUE);
	rcToorBarCtrl.EnableButton(ID_STOP_ENCODE,bStart? TRUE : FALSE);
	*/
	return TRUE;
}
BOOL CMyToolBar::PreTranslateMessage(MSG* pMsg)
{
	/*
	//�}�E�X�̈ړ������o
	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE: 
			{
				//TRACE0("CMyToolBar::PreTranslateMessage()\n");
				CToolTipCtrl* tips = this->GetToolBarCtrl().GetToolTips();
				tips->RelayEvent(pMsg);
				//m_ToolTip.RelayEvent(pMsg);
			}
			break;
	}
	*/
	return CToolBar::PreTranslateMessage(pMsg);
}

BOOL CMyToolBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	switch (((LPNMHDR)lParam)->code)
	{
		case TTN_GETDISPINFO:
			{
				LPTOOLTIPTEXT lpttt;
				lpttt = (LPTOOLTIPTEXT) lParam;
				lpttt->hinst = ::AfxGetInstanceHandle();
				switch (lpttt->hdr.idFrom)
				{
				case ID_SHOW_PROPERTY:
				case ID_SHOW_MONITOR:
				case ID_SHOW_PREVIEW:
				case ID_SHOW_VOLUME:
				case ID_PLAY_ENCODE:
				case ID_STOP_ENCODE:
					lpttt->lpszText = MAKEINTRESOURCE(lpttt->hdr.idFrom);
				}
			}
		break;
	}
	return CToolBar::OnNotify(wParam, lParam, pResult);
}
