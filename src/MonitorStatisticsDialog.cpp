// MonitorStatisticsDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MonitorStatisticsDialog.h"
#include "MainFrm.h"

// CMonitorStatisticsDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CMonitorStatisticsDialog, CDialog)

CMonitorStatisticsDialog::CMonitorStatisticsDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorStatisticsDialog::IDD, pParent)
{

}

CMonitorStatisticsDialog::~CMonitorStatisticsDialog()
{
}

void CMonitorStatisticsDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMonitorStatisticsDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CMonitorStatisticsDialog ���b�Z�[�W �n���h��

////////////////////////////////////////
//�_�C�A���O������
////////////////////////////////////////
BOOL CMonitorStatisticsDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �����ɏ�������ǉ����Ă�������

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}

//////////////////////////////////////////////////
//�_�C�A���O���I������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorStatisticsDialog::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}

//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX
//////////////////////////////////////////////////
void CMonitorStatisticsDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}
//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CMonitorStatisticsDialog::Encode(BOOL bStart)
{
	const int TIMER_ID = 102;
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	if (bStart)
	{
		//�J�n����
		m_tStartTime = CTime::GetCurrentTime();
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_START_TIME_EDIT);
		assert(edit);
		edit->SetWindowText(m_tStartTime.Format(_T("%Y/%m/%d/%H:%M:%S")));
		//�o�ߎ���
		edit = (CEdit*)this->GetDlgItem(IDC_ELAPSE_TIME_EDIT);
		assert(edit);
		edit->SetWindowText(_T(""));
		//�I������
		edit = (CEdit*)this->GetDlgItem(IDC_END_TIME_EDIT);
		assert(edit);
		edit->SetWindowText(_T(""));
		//���v�^�C�}�[�̋N��
		if (pSetting->m_nMonitorUpdateTimer != 0)
			SetTimer(TIMER_ID,pSetting->m_nMonitorUpdateTimer,NULL);
	} else {
		//���v�^�C�}�[�̏I��
		if (pSetting->m_nMonitorUpdateTimer != 0)
			KillTimer(TIMER_ID);
		//�I�����̍X�V
		DrawStatics();

		//�I������
		m_tEndTime = CTime::GetCurrentTime();
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_END_TIME_EDIT);
		assert(edit);
		edit->SetWindowText(m_tEndTime.Format(_T("%Y/%m/%d/%H:%M:%S")));
		//�o�ߎ���
		m_tElapseTime = m_tEndTime - m_tStartTime;
		edit = (CEdit*)this->GetDlgItem(IDC_ELAPSE_TIME_EDIT);
		assert(edit);
		CString strText;
		LONGLONG GetHour = m_tElapseTime.GetTotalHours();
		LONG GetMinite = m_tElapseTime.GetMinutes();
		LONG GetSecond = m_tElapseTime.GetSeconds();
		strText.Format(_T("%I64d:%02d:%02d"),GetHour,GetMinite,GetSecond);
		edit->SetWindowText(strText);
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�E�B���h�E�̕\���E��\���ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorStatisticsDialog::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	if (bShow) {
		CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
		if (pDSA->IsEncode()) {
			DrawStatics();
		}
	}
}

//////////////////////////////////////////////////
//WM_TIMER�̃C�x���g�n���h��
//////////////////////////////////////////////////
void CMonitorStatisticsDialog::OnTimer(UINT_PTR nIDEvent)
{
	//���v���̕\��
	if (IsWindowVisible())
		DrawStatics();

	CDialog::OnTimer(nIDEvent);
}

//////////////////////////////////////////////////
//���v���̕\��
//////////////////////////////////////////////////
void CMonitorStatisticsDialog::DrawStatics()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	if (!pDSA->IsEncode())
		return;

	QWORD cnsCurrentTime;				//�o�ߎ���
	WM_WRITER_STATISTICS Stats;			//���v���
	WM_WRITER_STATISTICS_EX StatsEx;	//���v���EX
	if (pDSA->GetStatistics(cnsCurrentTime,Stats,StatsEx))
	{
		CString strText;
		//�G���R�[�h�o�ߎ��Ԃ��v�Z
		CEdit* edit = (CEdit*)this->GetDlgItem(IDC_ELAPSE_TIME_EDIT); assert(edit);
		m_tElapseTime = CTime::GetCurrentTime() - m_tStartTime;
		LONGLONG GetHour = m_tElapseTime.GetTotalHours();
		LONG GetMinite = m_tElapseTime.GetMinutes();
		LONG GetSecond = m_tElapseTime.GetSeconds();
		strText.Format(_T("%I64d:%02d:%02d"),GetHour,GetMinite,GetSecond);
		edit->SetWindowText(strText);

		//�T���v����
		edit = (CEdit*)this->GetDlgItem(IDC_SAMPLE__EDIT); assert(edit);
		strText.Format(_T("%I64d %s"),Stats.qwSampleCount,ByteToString(Stats.qwByteCount));
		edit->SetWindowText(strText);
		//���������T���v����
		edit = (CEdit*)this->GetDlgItem(IDC_SAMPLE_LOST_EDIT); assert(edit);
		strText.Format(_T("%I64d %s"),Stats.qwDroppedSampleCount,ByteToString(Stats.qwDroppedByteCount));
		edit->SetWindowText(strText);
		//���������T���v���̏ڍ�
		edit = (CEdit*)this->GetDlgItem(IDC_SAMPLE_QUEUE_EDIT); assert(edit);
		strText.Format(_T("%d (%d)"),StatsEx.dwTotalSampleDropsInQueue,StatsEx.dwCurrentSampleDropRateInQueue);
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_SAMPLE_CODEC_EDIT); assert(edit);
		strText.Format(_T("%d (%d)"),StatsEx.dwTotalSampleDropsInCodec,StatsEx.dwCurrentSampleDropRateInCodec);
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_SAMPLE_MULTIPLEXER_EDIT); assert(edit);
		strText.Format(_T("%d (%d)"),StatsEx.dwTotalSampleDropsInMultiplexer,StatsEx.dwCurrentSampleDropRateInMultiplexer);
		edit->SetWindowText(strText);
		//�r�b�g���[�g�̏ڍ�
		edit = (CEdit*)this->GetDlgItem(IDC_BITRATE_NOW_EDIT); assert(edit);
		strText.Format(_T("%.2f (Kbps)"),(((double)Stats.dwCurrentBitrate)/1000));
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_BITRATE_AVERAGE_EDIT); assert(edit);
		strText.Format(_T("%.2f (Kbps)"),(((double)Stats.dwAverageBitrate)/1000));
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_BITRATE_EXPECTED_EDIT); assert(edit);
		strText.Format(_T("%.2f (Kbps)"),(((double)Stats.dwExpectedBitrate)/1000));
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_BITRATE_OVERHEAD_EDIT); assert(edit);
		strText.Format(_T("%.2f (Kbps)"),(((double)StatsEx.dwBitratePlusOverhead)/1000));
		edit->SetWindowText(strText);
		//�t���[�����[�g�̏ڍ�
		edit = (CEdit*)this->GetDlgItem(IDC_FRAMERATE_NOW_EDIT); assert(edit);
		strText.Format(_T("%.2f (fps)"),(((double)Stats.dwCurrentSampleRate)/1000));
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_FRAMERATE_AVERAGE_EDIT); assert(edit);
		strText.Format(_T("%.2f (fps)"),(((double)Stats.dwAverageSampleRate)/1000));
		edit->SetWindowText(strText);
		edit = (CEdit*)this->GetDlgItem(IDC_FRAMERATE_EXPECTED_EDIT); assert(edit);
		strText.Format(_T("%.2f (fps)"),(((double)Stats.dwExpectedSampleRate)/1000));
		edit->SetWindowText(strText);

		//�X�e�[�^�X�o�[�̍X�V
		CMainFrame* frame = (CMainFrame*)::AfxGetMainWnd(); assert(frame);
		CMyStatusBar* status = frame->GetStatusBar();
		//�]���ʂ̌v�Z
		LONGLONG llTransformRate;		//�ŏI�I�Ɍv�Z�����]����
		DWORD dwTransformBitrate;
		if (StatsEx.dwBitratePlusOverhead != 0) {
			dwTransformBitrate = StatsEx.dwBitratePlusOverhead;
		} else {
			dwTransformBitrate = Stats.dwCurrentBitrate;
		}
		//�ڑ��l�����擾
		CMonitorFormView* monitor = frame->GetMonitorWnd();
		assert(monitor);
		CMonitorPullDialog* pull = monitor->GetPullTab();
		assert(pull);
		int nConnectCount = pull->GetAllConnectCount();
		llTransformRate = dwTransformBitrate * nConnectCount;	//�]���ʂƐl�����|����

		//Push�z�M�ʕ����v�Z
		int nPushSinkCount = 0;
		for (int i=0;i<(int)pDSA->m_PushSinkList.size();i++)
		{
			if (pDSA->m_PushSinkList[i].nID != -1)
				continue;
			if (pDSA->m_PushSinkList[i].nRegistID != -1)
				continue;
			if (pDSA->m_PushSinkList[i].strServerName.IsEmpty())
				continue;
			nPushSinkCount++;
			nConnectCount++;
		}
		llTransformRate += (dwTransformBitrate * nPushSinkCount);

		if (llTransformRate >= (1000*1000)) {
			double dMbps = ((double)llTransformRate) / ((double)(1000*1000));
			strText.Format(_T("UP:%.2fMbps (%d)"),dMbps,nConnectCount);
		} else {
			double dKbps = ((double)llTransformRate) / ((double)(1000));
			strText.Format(_T("UP:%.2fKbps (%d)"),dKbps,nConnectCount);
		}
		status->SetTransform(strText);	//�X�e�[�^�X�o�[�ɒʒm
	}
}

//////////////////////////////////////////////////
//�o�C�g�T�C�Y�𕶎���ɕϊ�����T�u���[�`��
//////////////////////////////////////////////////
CString CMonitorStatisticsDialog::ByteToString(QWORD qwByte)
{
	CString strText(_T(""));
	QWORD qwKillo;
	QWORD qwMega;
	QWORD qwGiga;
	qwKillo = qwByte / 1000;
	qwMega = qwKillo / 1000;
	qwGiga = qwMega / 1000;
	if (qwGiga != 0) {
		qwMega = qwMega % 1000;
		strText.Format(_T("(%I64d.%02I64dGB)"),qwGiga,qwMega/10);
	} else if (qwMega != 0) {
		qwKillo = qwKillo % 1000;
		strText.Format(_T("(%I64d.%02I64dMB)"),qwMega,qwKillo/10);
	} else if (qwKillo != 0) {
		qwByte = qwByte % 1000;
		strText.Format(_T("(%I64d.%02I64dKB)"),qwKillo,qwByte/10);
	} else {
		//qwByte = qwByte % 1000;
		strText.Format(_T("(%I64dB)"),qwByte);
	}
	return strText;
}

