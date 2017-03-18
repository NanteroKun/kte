#include "StdAfx.h"
#include "MyStatusBar.h"

#include "KTE.h"

static UINT indicators[] =
{
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
	ID_SEPARATOR,           // �X�e�[�^�X ���C�� �C���W�P�[�^
};

BEGIN_MESSAGE_MAP(CMyStatusBar, CStatusBar)
	ON_WM_TIMER()
END_MESSAGE_MAP()

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CMyStatusBar::CMyStatusBar(void)
{
	m_hQuery = NULL;
	m_hCounter = NULL;
}
//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CMyStatusBar::~CMyStatusBar(void)
{
}

//////////////////////////////////////////////////
//�E�B���h�E�̃X�^�C����ύX����
//////////////////////////////////////////////////
BOOL CMyStatusBar::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style &= ~WS_BORDER;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.dwExStyle |= WS_EX_WINDOWEDGE;
	return CStatusBar::PreCreateWindow(cs);
}
//////////////////////////////////////////////////
//�X�e�[�^�X�o�[���쐬�����Ƃ��ɌĂ΂��
//////////////////////////////////////////////////
BOOL CMyStatusBar::Create(CFrameWnd* pFrameWnd)
{
	if (!CStatusBar::Create(pFrameWnd))
		return FALSE;      // �쐬�ł��Ȃ��ꍇ

	//�C���W�P�[�^��ݒ�
	CStatusBar::SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));
	CStatusBar::SetPaneInfo(0,0,SBPS_STRETCH | SBPS_NOBORDERS,0);	//���E�B���h�E
	CStatusBar::SetPaneInfo(1,0,SBPS_NORMAL,25);					//��~�E�Đ�
	CStatusBar::SetPaneInfo(2,0,SBPS_NORMAL,40);					//�g��{��
	CStatusBar::SetPaneInfo(3,0,SBPS_NORMAL,120);					//�]����
	CStatusBar::SetPaneInfo(4,0,SBPS_NORMAL,70);					//CPU�g�p��

	SetPlayStatus(FALSE);
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	SetTimes(pSetting->m_dPreviewChildTimes);

	//CStatusBar::SetPaneText(2,_T("UPXXX.XXKbps"));
	//CStatusBar::SetPaneText(3,_T(""));

	//�����x�p�t�H�[�}���X�J�E���^�̏�����
	if (m_hQuery) PdhCloseQuery( m_hQuery );
	if ( PdhOpenQuery(NULL,0,&m_hQuery) == ERROR_SUCCESS )
	{
		PdhAddCounter( m_hQuery, _T("\\Processor(_Total)\\% Processor Time"), 0, &m_hCounter );
		PdhCollectQueryData( m_hQuery );

		//�^�C�}�[���N��
		CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
		if (pSetting->m_nMonitorUpdateTimer != 0)
			SetTimer(0,pSetting->m_nMonitorUpdateTimer, NULL);
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�X�e�[�^�X�o�[�ɕ��������������
//////////////////////////////////////////////////
BOOL CMyStatusBar::SetString(LPCTSTR lpszText)
{
	return CStatusBar::SetPaneText(0,lpszText);
}
//////////////////////////////////////////////////
//�X�e�[�^�X�o�[�̍Đ�/��~��Ԃ��X�V����
//////////////////////////////////////////////////
BOOL CMyStatusBar::SetPlayStatus(BOOL bPlay)
{
	if (bPlay)
		return CStatusBar::SetPaneText(1,_T("�Đ�"));
	return CStatusBar::SetPaneText(1,_T("��~"));
}
//////////////////////////////////////////////////
//�X�e�[�^�X�o�[�Ɋg��k���{������������
//////////////////////////////////////////////////
BOOL CMyStatusBar::SetTimes(double dTimes)
{
	if (this->GetSafeHwnd() == NULL)
		return FALSE;
	CString strNum;
	strNum.Format(_T("%.0f%%"),dTimes);
	return CStatusBar::SetPaneText(2,strNum);
}

//////////////////////////////////////////////////
//�X�e�[�^�X�o�[�ɓ]���ʂ���������
//////////////////////////////////////////////////
BOOL CMyStatusBar::SetTransform(LPCTSTR lpszText)
{
	return CStatusBar::SetPaneText(3,lpszText);
}


//////////////////////////////////////////////////
//WM_TIMER�ɂ��X�e�[�^�X�o�[�̍X�V
//////////////////////////////////////////////////
void CMyStatusBar::OnTimer(UINT_PTR nIDEvent)
{
	ASSERT(m_hQuery);
	ASSERT(m_hCounter);
	//CPU�g�p�����擾����
	PDH_FMT_COUNTERVALUE fntValue;
	fntValue.doubleValue = 0.0;
	PdhCollectQueryData( m_hQuery );
	PdhGetFormattedCounterValue( m_hCounter, PDH_FMT_DOUBLE, NULL, &fntValue );
	CString cstr;
	cstr.Format(_T("CPU:%02.0f%%"),fntValue.doubleValue);
	CStatusBar::SetPaneText(4,cstr);

	CStatusBar::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////
//�E�B���h�E�j���̃^�C�~���O�ŌĂ΂��
//////////////////////////////////////////////////
BOOL CMyStatusBar::DestroyWindow()
{
	//�N�G�����폜
	if (m_hQuery)
		PdhCloseQuery( m_hQuery );
	m_hQuery = NULL;
	m_hCounter = NULL;

	//�^�C�}�[���폜
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (pSetting->m_nMonitorUpdateTimer != 0)
		KillTimer(0);

	return CStatusBar::DestroyWindow();
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��(�v������)
//////////////////////////////////////////////////
BOOL CMyStatusBar::PreEncode(BOOL bStart)
{
	if (bStart == FALSE)
		SetString(_T("�G���R�[�h���~���Ă��܂�..."));
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CMyStatusBar::Encode(BOOL bStart)
{
	SetPlayStatus(bStart);
	if (bStart)
		SetString(_T("�G���R�[�h���J�n���܂���"));
	else
		SetString(_T("�G���R�[�h���I�����܂���"));
	SetTransform(_T(""));	//�]���ʂ̃N���A
	return TRUE;
}
