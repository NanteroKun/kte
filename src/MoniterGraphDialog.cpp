// MoniterGraphDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MoniterGraphDialog.h"
#include "afxdialogex.h"


// MoniterGraphDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CMoniterGraphDialog, CDialog)

BEGIN_MESSAGE_MAP(CMoniterGraphDialog, CDialog)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()


////////////////////////////////////////
//�R���X�g���N�^
////////////////////////////////////////
CMoniterGraphDialog::CMoniterGraphDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CMoniterGraphDialog::IDD, pParent)
{

}

////////////////////////////////////////
//�f�X�g���N�^
////////////////////////////////////////
CMoniterGraphDialog::~CMoniterGraphDialog()
{
}

void CMoniterGraphDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

// MoniterGraphDialog ���b�Z�[�W �n���h���[

////////////////////////////////////////
//�_�C�A���O������
////////////////////////////////////////
BOOL CMoniterGraphDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (pSetting->m_nMonitorUpdateTimer != 0)
		SetTimer(TIMER_ID,pSetting->m_nMonitorUpdateTimer,NULL);

	return TRUE;
}
//////////////////////////////////////////////////
//�_�C�A���O���I������Ƃ��ɌĂ΂��C�x���g�n���h��
//////////////////////////////////////////////////
void CMoniterGraphDialog::OnDestroy()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	if (pSetting->m_nMonitorUpdateTimer != 0)
		KillTimer(TIMER_ID);
	//�����ɃR�[�h
	CDialog::OnDestroy();
}
//////////////////////////////////////////////////
//�E�B���h�E�T�C�Y�ύX
//////////////////////////////////////////////////
void CMoniterGraphDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƏI��
//////////////////////////////////////////////////
BOOL CMoniterGraphDialog::Encode(BOOL bStart)
{
	return TRUE;
}

//////////////////////////////////////////////////
//WM_TIMER�̃C�x���g�n���h��
//////////////////////////////////////////////////
void CMoniterGraphDialog::OnTimer(UINT_PTR nIDEvent)
{
	CString strMessage;
	CString strFourCC;
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();


	//�t�B���^�O���t�̏�
	CWnd* wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_STATUS_EDIT);assert(wnd);
	strMessage.Format(_T("�t�B���^��:%s\r\n�G���R�[�h��:%s\r\n�r�f�I�v���r���[:%s\r\n�I�[�f�B�I�v���r���[:%s\r\n�G���R�[�h:%s\r\n"),
		pDSA->IsRunFilter()?_T("�Đ�"):_T("��~"),
		pDSA->IsEncode()?_T("�Đ�"):_T("��~"),
		pDSA->IsCanPreview()?_T("��"):_T("��"),
		pDSA->IsCanAudioPreview()?_T("��"):_T("��"),
		pDSA->IsCanEncode()?_T("��"):_T("��"));
	wnd->SetWindowText(strMessage);

	//�r�f�I�t�H�[�}�b�g
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_VIDEO_INPUT_EDIT);assert(wnd);
	CreateMediaTypeString(&pDSA->m_VideoFirstMediaType,strMessage);
	wnd->SetWindowText(strMessage);
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_VIDEO_OUTPUT_EDIT);assert(wnd);
	CreateMediaTypeString(&pDSA->m_VideoLastMediaType,strMessage);
	wnd->SetWindowText(strMessage);

	//�I�[�f�B�I�t�H�[�}�b�g
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_AUDIO_INPUT_EDIT);assert(wnd);
	CreateMediaTypeString(&pDSA->m_AudioFirstMediaType,strMessage);
	wnd->SetWindowText(strMessage);
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_AUDIO_OUTPUT_EDIT);assert(wnd);
	CreateMediaTypeString(&pDSA->m_AudioLastMediaType,strMessage);
	wnd->SetWindowText(strMessage);

	//�I�[�f�B�I�o�b�t�@�T�C�Y
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_AUDIO_BUFFER_EDIT);assert(wnd);
	strMessage.Format(_T("Set.cBuffers:%d\r\nSet.cbBuffer:%d\r\nSet.cbAlign:%d\r\nSet.cbPrefix:%d\r\nGet.cBuffers:%d\r\nGet.cbBuffer:%d\r\nGet.cbAlign:%d\r\nGet.cbPrefix:%d\r\n"),
		pDSA->m_AudioSetAllocator.cBuffers,
		pDSA->m_AudioSetAllocator.cbBuffer,
		pDSA->m_AudioSetAllocator.cbAlign,
		pDSA->m_AudioSetAllocator.cbPrefix,
		pDSA->m_AudioGetAllocator.cBuffers,
		pDSA->m_AudioGetAllocator.cbBuffer,
		pDSA->m_AudioGetAllocator.cbAlign,
		pDSA->m_AudioGetAllocator.cbPrefix);
	wnd->SetWindowText(strMessage);

	//��N���b�N����̊�^�C���̎擾
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_REFTIME_EDIT);assert(wnd);
	REFERENCE_TIME rtTime = 0;
	REFERENCE_TIME rtResolution = 0;
	if (pDSA->GetClockTime(rtTime)) {
		if (pDSA->GetClockResolution(rtResolution)) {
			strMessage.Format(_T("Time%I64d(100ns)\r\n����\:%I64d(�}�C�N��s)"),rtTime,(rtResolution/(REFERENCE_TIME)100));
		}
	} else {
		strMessage.Format(_T("��^�C��:����"));
	}
	wnd->SetWindowText(strMessage);

	//�r�f�I�̑��t���[���E�h���b�v�t���[���̎擾
	//(�قƂ�ǂ̃r�f�I�L���v�`���f�o�C�X�͂��̃C���^�t�F�[�X���T�|�[�g���Ă��Ȃ�)
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_VIDEO_DROP_EDIT);assert(wnd);
	long lNotDropped = 0;
	long lDropped = 0;
	long lAverageSize = 0;
	if (pDSA->GetVideoDropFrame(&lNotDropped,&lDropped,&lAverageSize)) {
		strMessage.Format(_T("NotDropped:%d\r\nDropped:%d\r\nAverageSize:%d"),
			lNotDropped,lDropped,lAverageSize);
	} else {
		strMessage.Format(_T("Unknown"));
	}
	wnd->SetWindowText(strMessage);

	//�x���E�I�t�Z�b�g
	REFERENCE_TIME rtLatency,rtOffset,rtMaxOffset;		//�x�����ԁE�I�t�Z�b�g
	ULONG uFlags;                   //�t���O
	rtLatency = 0;
	rtOffset = 0;
	rtMaxOffset = 0;
	uFlags = 0;
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_VIDEO_PUSHINFO_EDIT);assert(wnd);
	if (pDSA->GetPushSourceInfo(TRUE,&rtLatency,&uFlags,&rtOffset,&rtMaxOffset)) {
		strMessage.Format(_T("Latency:%I64d\r\nOffset:%I64d\r\nMaxOffset:%I64d"),
			lNotDropped,lDropped,lAverageSize);
	} else {
		strMessage.Format(_T("Unknown"));
	}
	wnd->SetWindowText(strMessage);

	rtLatency = 0;
	rtOffset = 0;
	rtMaxOffset = 0;
	uFlags = 0;
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_AUDIO_PUSHINFO_EDIT);assert(wnd);
	if (pDSA->GetPushSourceInfo(FALSE,&rtLatency,&uFlags,&rtOffset,&rtMaxOffset)) {
		strMessage.Format(_T("Latency:%I64d\r\nOffset:%I64d\r\nMaxOffset:%I64d"),
			lNotDropped,lDropped,lAverageSize);
	} else {
		strMessage.Format(_T("Unknown"));
	}
	wnd->SetWindowText(strMessage);

	//�X�g���[�����
	AM_STREAM_INFO asi;
	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_VIDEO_STREAM_EDIT);assert(wnd);
	if (pDSA->GetStreamInfo(TRUE,&asi)) {
		strMessage.Format(_T("tStart:%I64d\r\ntStop:%I64d\r\nCokkie:%d-%d\r\nFlags:%d"),
			asi.tStart,asi.tStop,
			asi.dwStartCookie,asi.dwStopCookie,
			asi.dwFlags);
	} else {
		strMessage.Format(_T("Unknown"));
	}
	wnd->SetWindowText(strMessage);

	wnd = (CWnd*)GetDlgItem(IDC_MONITOR_GRAPH_AUDIO_STREAM_EDIT);assert(wnd);
	if (pDSA->GetStreamInfo(FALSE,&asi)) {
		strMessage.Format(_T("tStart:%I64d\r\ntStop:%I64d\r\nCokkie:%d-%d\r\nFlags:%d"),
			asi.tStart,asi.tStop,
			asi.dwStartCookie,asi.dwStopCookie,
			asi.dwFlags);
	} else {
		strMessage.Format(_T("Unknown"));
	}
	wnd->SetWindowText(strMessage);


	CDialog::OnTimer(nIDEvent);
}
//////////////////////////////////////////////////
//���f�B�A�^�C�v�����񕶎���𐶐�����
//////////////////////////////////////////////////
void CMoniterGraphDialog::CreateMediaTypeString(AM_MEDIA_TYPE* pAmt,CString &strMessage)
{
	strMessage = _T("");
	if (pAmt->formattype == FORMAT_VideoInfo) {
		CString strFourCC;
		VIDEOINFOHEADER* VideoInfoHeader = (VIDEOINFOHEADER*)pAmt->pbFormat;
		CreateFourCCString(VideoInfoHeader->bmiHeader.biBitCount,
			VideoInfoHeader->bmiHeader.biCompression,
			pAmt->subtype.Data1,strFourCC);
		strMessage.Format(_T("��:%d ����:%d\r\n�F���:%s\r\n�F��:%d\r\n���k:%s\r\nFPS:%f\r\n"),
			VideoInfoHeader->bmiHeader.biWidth,
			VideoInfoHeader->bmiHeader.biHeight,
			strFourCC,
			VideoInfoHeader->bmiHeader.biBitCount,
			VideoInfoHeader->bmiHeader.biCompression==BI_RGB?_T("�񈳏k"):_T("���k"),
			(double) ((double)10000000.0 / (double)VideoInfoHeader->AvgTimePerFrame));
	} else if (pAmt->formattype == FORMAT_VideoInfo2) {
		CString strFourCC;
		VIDEOINFOHEADER2* VideoInfoHeader2 = (VIDEOINFOHEADER2*)pAmt->pbFormat;
		CreateFourCCString(VideoInfoHeader2->bmiHeader.biBitCount,
			VideoInfoHeader2->bmiHeader.biCompression,
			pAmt->subtype.Data1,strFourCC);
		strMessage.Format(_T("��:%d ����:%d\r\n�F���:%s\r\n�F��:%d\r\n���k:%s\r\nFPS:%f\r\n"),
			VideoInfoHeader2->bmiHeader.biWidth,
			VideoInfoHeader2->bmiHeader.biHeight,
			strFourCC,
			VideoInfoHeader2->bmiHeader.biBitCount,
			VideoInfoHeader2->bmiHeader.biCompression==BI_RGB?_T("�񈳏k"):_T("���k"),
			(double) ((double)10000000.0 / (double)VideoInfoHeader2->AvgTimePerFrame));
	} else if (pAmt->formattype == FORMAT_WaveFormatEx){
		WAVEFORMATEX* WaveFormatEx = (WAVEFORMATEX*)pAmt->pbFormat;
		strMessage.Format(_T("���g��:%dHz\r\n����\%dbit\r\n�`�����l��%dch\r\n"),
			WaveFormatEx->nSamplesPerSec,
			WaveFormatEx->wBitsPerSample,
			WaveFormatEx->nChannels);
	} else {
		strMessage = _T("�s����\r\n�t�H�[�}�b�g");
	}
}

//////////////////////////////////////////////////
//FOURCC�����񐶐��̃T�u���[�`��
//////////////////////////////////////////////////
void CMoniterGraphDialog::CreateFourCCString(WORD biBitCount,DWORD biCompression,DWORD data1,CString &strFourcc)
{
	strFourcc = _T("");
	if (biCompression == 0)	//�񈳏k�t�H�[�}�b�g
	{
		strFourcc.Format(_T("RGB%d"),biBitCount);
	} else {				//���k�t�H�[�}�b�g(FOURCC)
		wchar_t wchData[5];
		wchData[0] = (wchar_t)(data1 & 0xFF); 
		wchData[1] = (wchar_t)((data1 >> 8) & 0xFF);
		wchData[2] = (wchar_t)((data1 >> 16) & 0xFF);
		wchData[3] = (wchar_t)((data1 >> 24) & 0xFF);
		wchData[4] = _T('\0');
		strFourcc = wchData;
	}
	return;
}