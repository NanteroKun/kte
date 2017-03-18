#pragma once

#include "DirectShow.h"
#include "DirectShowEncoder.h"
#include "DirectShowASFWriter.h"
#include "DirectShowAudioVolume.h"

//�v���O�C���̍\����
struct FilterPluginStruct
{
	GUID guidFilter;			//�t�B���^��GUID
	GUID guidCategory;			//�t�B���^�̃J�e�S��GUID (DMO�̂�)
	int nFilterID;				//�t�B���^�ɓo�^�����ۂɓ�����ID (-1�Ȃ疢�쐬)
	CString strFilterName;		//�t�B���^�ɓo�^�����ۂ̓o�^��
	BOOL bDMO;					//DMO�f�o�C�X���ǂ���	(DMO�̏ꍇ�̓J�e�S��GUID����`����Ă���)
	BOOL bEnable;				//���̃t�B���^���L�����ǂ���
};

//�v���V���N�\����
struct PullSinkStruct
{
	int nID;				//�C���f�b�N�XID
	int nRegistID;			//�o�^ID
	DWORD dwPort;			//�|�[�g�ԍ�
	DWORD dwMax;			//�ő�ڑ���
};
//�v�b�V���V���N�\����
struct PushSinkStruct
{
	int nID;				//�C���f�b�N�XID
	int nRegistID;			//�o�^ID
	CString strServerName;	//�T�[�o�[��
	BOOL bAutoDelete;		//�����폜
};
//�t�@�C���V���N�\����
struct FileSinkStruct
{
	int nID;				//�C���f�b�N�XID
	int nRegistID;			//�o�^ID
	CString strFileName;	//�t�@�C����
};

//WDM�t�B���^�̗񋓌^
enum ENUM_WDM_FILTER
{
	WDM_FILTER_NONE = 0,
	WDM_FILTER_CROSS_BAR1,
	WDM_FILTER_CROSS_BAR2,
	WDM_FILTER_TV_AUDIO,
	WDM_FILTER_TV_TUNER
};

//////////////////////////////////////////////////
//MFC������DirectShow�ɑ΂���A�N�Z�T�N���X
//////////////////////////////////////////////////
class CDirectShowAccess
{
public:
	CDirectShowAccess(void);			//�R���X�g���N�^
	~CDirectShowAccess(void);			//�f�X�g���N�^
	BOOL Init();						//DirectShow��������
	void Exit();						//DirectShow���I��

	BOOL ChangeProfileNotify();			//�v���t�@�C���̕ύX�ʒm
	BOOL SaveProfileSetting();			//�v���t�@�C������̃��[�h
	BOOL LoadProfileSetting();			//�v���t�@�C���ւ̃Z�[�u

	//�O���t�t�@�C���̕ۑ�
	HRESULT SaveGraphFile(WCHAR* wszPath)
		{return CDirectShowEtc::SaveGraphFile(m_pDirectShow->GetGraphBuilder(),wszPath);}

	//////////////////////////////////////////////////
	//�t�B���^�̎��s�ƒ�~
	BOOL RunFilter();					//�t�B���^�̊J�n
	BOOL StopFilter();					//�t�B���^�̒�~
	BOOL IsRunFilter()const{			//�t�B���^�̊J�n���
		return m_bRunFilter;}
	BOOL m_bRunFilter;					//�t�B���^�����s���Ă��邩�ǂ���

	//�v���r���[���\���ۂ�(�r�f�I�̂݃`�F�b�N)
	inline BOOL IsCanPreview() const {
		return (m_nVideoIndex != -1 && m_nVideoSettingIndex != -1);
	}
	inline BOOL IsCanAudioPreview() const {return (m_nAudioIndex != -1);}

	//////////////////////////////////////////////////
	//�r�f�I�L���v�`���f�o�C�X��I�����܂�
	BOOL SelectVideoIndex(int nIndex);
	//�r�f�I�L���v�`���f�o�C�X�̐ݒ��I�����܂�
	BOOL SelectVideoSettingIndex(int nSettingIndex,AM_MEDIA_TYPE *pamt);
	//���݂̃L���v�`���f�o�C�X�̃r�f�I�T�C�Y���擾
	BOOL GetCurrentVideoSize(int &nWidth,int &nHeight);

	//////////////////////////////////////////////////
	//�I�[�f�B�I�L���v�`���f�o�C�X��I�����܂�
	BOOL SelectAudioIndex(int nIndex,BOOL bUseVideoPin);
	//�I�[�f�B�I�L���v�`���f�o�C�X�̐ݒ��I�����܂�
	BOOL SelectAudioSettingIndex(int nSettingIndex,AM_MEDIA_TYPE *pamt);
	//�I�[�f�B�I�{�����[���t�B���^�ɐڑ�����
	BOOL ConnectAudioVolume();

	int GetVideoIndex() const {return m_nVideoIndex;}
	int GetVideoSettingIndex() const {return m_nVideoSettingIndex;}

	int GetAudioIndex() const {return m_nAudioIndex;}
	int GetAudioSettingIndex() const {return m_nAudioSettingIndex;}
	int GetWDMFilterID(ENUM_WDM_FILTER e) const {
		switch (e) {
		case WDM_FILTER_NONE: return -1;
		case WDM_FILTER_CROSS_BAR1: return m_nCrossbar1ID;
		case WDM_FILTER_CROSS_BAR2: return m_nCrossbar2ID;
		case WDM_FILTER_TV_AUDIO:	 return m_nTVAudioID;
		case WDM_FILTER_TV_TUNER:   return m_nTVTunerID;
		default: return -1;
		}
	}
	BOOL IsVidoFindAudioPin() const {return m_bVideoFindAudioPin;}
	BOOL IsUseAudioVideoFilter() const {
		if (m_bUseAudioVideoFilter && m_nVideoFilterID == m_nAudioFilterID) return TRUE;
		return FALSE;}

	HRESULT RenderPreview();				//�v���r���[�̐ڑ��̎��s
	HRESULT RenderPreviewSubRoutine1(const int nLastFilterID,const wchar_t* lpszOutputPin);
	void    RenderPreviewSubRoutine2();

	BOOL CheckConnectFilter(BOOL bVideo);	//�t�B���^�̐ڑ��󋵂��m�F����T�u���[�`��


	//�R�[���o�b�N�E�B���h�E�n���h���̐ݒ�
	BOOL SetCallbackHWnd(HWND hWnd) {
		return (BOOL)m_pDirectShow->SetCallbackHWnd(hWnd);}
	//�R�[���o�b�N�̒ʒm
	LRESULT OnGraphNotify() {
		return m_pDirectShow->OnGraphNotify();}


	//////////////////////////////////////////////////
	//�r�f�I�v���r���[�̃A�N�Z�T
	BOOL SetPreviewWindow(HWND hWnd);		//�r�f�I�v���r���[�̃E�B���h�E�n���h����ݒ肷��
	BOOL RemovePreviewWindow();				//�r�f�I�v���r���[�̃E�B���h�E�n���h������������
	BOOL ResizePreviewWindow(RECT rcClient);//�r�f�I�v���r���[�̃E�B���h�E�T�C�Y��ύX����
	BOOL ShowPreviewWindow(BOOL bShow);		//�r�f�I�v���r���[�̕\���E��\����؂�ւ���
	
	//�v���p�e�B�E�B���h�E��\�����܂�
	BOOL ShowPropertySetting(BOOL bVideo,ENUM_WDM_FILTER eDevice,HWND hWnd);

	//////////////////////////////////////////////////
	//�����o�ϐ�
	//�r�f�I�L���v�`���f�o�C�X�Ɋւ���ݒ�
	vector<wstring> m_strVideoNameList;						//�S�Ẵr�f�I�L���v�`���f�o�C�X�̖��O���X�g
	vector<wstring> m_strVideoSettingList;					//���݂̃r�f�I�L���v�`���f�o�C�X�̐ݒ胊�X�g(�ݒ藪��)
	vector<AM_MEDIA_TYPE>			 m_VideoMediaTypeList;	//�r�f�I�̃��f�B�A�^�C�v���X�g (�΂ɂȂ��Ă���)
	vector<VIDEO_STREAM_CONFIG_CAPS> m_VideoCapsList;		//�r�f�I�̃f�o�C�X���\���X�g   (�΂ɂȂ��Ă���)
	AM_MEDIA_TYPE m_VideoFirstMediaType;					//���݂̃r�f�I�ڑ����f�B�A�^�C�v(�Ő�[)
	AM_MEDIA_TYPE m_VideoLastMediaType;						//���݂̃r�f�I�ڑ����f�B�A�^�C�v(�Ō��)

	//�I�[�f�B�I�L���v�`���f�o�C�X�Ɋւ���ݒ�
	vector<wstring> m_strAudioNameList;						//�S�ẴI�[�f�B�I�L���v�`���f�o�C�X�̖��O
	vector<wstring> m_strAudioSettingList;					//���݂̃I�[�f�B�I�L���v�`���f�o�C�X�̐ݒ胊�X�g(�ݒ藪��)
	vector<AM_MEDIA_TYPE> m_AudioMediaTypeList;				//�I�[�f�B�I�̃��f�B�A�^�C�v���X�g	(�΂ɂȂ��Ă���)
	vector<AUDIO_STREAM_CONFIG_CAPS> m_AudioCapsList;		//�I�[�f�B�I�̃f�o�C�X���\���X�g	(�΂ɂȂ��Ă���)
	AM_MEDIA_TYPE m_AudioFirstMediaType;					//���݂̃I�[�f�B�I�ڑ����f�B�A�^�C�v(�Ő�[)
	AM_MEDIA_TYPE m_AudioLastMediaType;						//���݂̃I�[�f�B�I�ڑ����f�B�A�^�C�v(�Ō��)
	ALLOCATOR_PROPERTIES m_AudioSetAllocator;				//�I�[�f�B�I�L���v�`���̃A���P�[�^�[�o�b�t�@(�v��)
	ALLOCATOR_PROPERTIES m_AudioGetAllocator;				//�I�[�f�B�I�L���v�`���̃A���P�[�^�[�o�b�t�@(���ۂ̐ڑ�)
	
	//////////////////////////////////////////////////
	//�v���O�C���Ɋւ���ݒ�
	vector<FilterPluginStruct> m_VideoPluginList;		//�r�f�I�v���O�C���̔z��
	vector<FilterPluginStruct> m_AudioPluginList;		//�I�[�f�B�I�v���O�C���̔z��

	//�t�B���^�z��̐ڑ�
	BOOL ChainPluginList(BOOL bVideo,int nInputFilterID,const wchar_t* lpszInputPinName,int nOutputFilterID,const wchar_t* lpszOutputPinName);
	//�v���O�C���̒ǉ��o�^
	BOOL AddPlugin(BOOL bVideo,const GUID guid,const GUID guidCategory,LPCWSTR lpszPluginName);
	//�v���O�C���̍폜
	BOOL RemovePlugin(BOOL bVideo,int nIndex);
	//�t�B���^�z��̓���ւ�(��ցE����)
	BOOL SwitchPlugin(BOOL bVideo,int nSrcIndex,int nDestIndex);
	//�v���O�C���̐ݒ�_�C�A���O��\��
	BOOL ShowPluginDialog(HWND hWnd,BOOL bVideo,int nIndex);

	//�v���O�C�������܂�
	//////////////////////////////////////////////////

	//��N���b�N����̊�^�C���̎擾
	BOOL GetClockTime(REFERENCE_TIME &rtTime) {assert(m_pDirectShow); return SUCCEEDED(m_pDirectShow->GetGraphClockTime(rtTime));}
	//��N���b�N�̕���\���擾
	BOOL GetClockResolution(REFERENCE_TIME &rcResolution) {assert(m_pDirectShow); return SUCCEEDED(m_pDirectShow->GetGraphClockResolution(rcResolution));}
	//�r�f�I�L���v�`���t�B���^�̃t���[�����E�h���b�v���Ȃǂ��擾
	BOOL GetVideoDropFrame(long* plNotDropped,long* plDropped,long* plAverageSize)
	{
		assert(m_pDirectShow);
		if (m_nVideoFilterID == -1)
			return FALSE;
		if (m_nVideoFilterOutputPinIndex == -1)
			return FALSE;
		HRESULT hr;
		hr = m_pDirectShow->GetDropFrame(
			m_pDirectShow->GetFilterByID(m_nVideoFilterID),m_nVideoFilterOutputPinIndex,
			plNotDropped,plDropped,plAverageSize);
		return SUCCEEDED(hr);
	}
	//�f�o�C�X���\(�x�����ԁE�I�t�Z�b�g)�Ȃǂ��擾
	BOOL GetPushSourceInfo(BOOL bVideo,REFERENCE_TIME* prtLatency,ULONG* pFlags,
		REFERENCE_TIME* prtOffset,REFERENCE_TIME* prtMaxOffset)
	{
		assert(m_pDirectShow);
		int nID = bVideo? m_nVideoFilterID : m_nAudioFilterID;
		if (nID == -1) return FALSE;
		int nPinIndex = bVideo ? m_nVideoFilterOutputPinIndex : m_nAudioFilterOutputPinIndex;
		if (nPinIndex == -1) return FALSE;
		HRESULT hr = m_pDirectShow->GetPushSourceInfo(
			m_pDirectShow->GetFilterByID(nID),
			nPinIndex,prtLatency,pFlags,prtOffset,prtMaxOffset);
		return SUCCEEDED(hr);
	}
	//�X�g���[�������擾
	BOOL GetStreamInfo(BOOL bVideo,AM_STREAM_INFO* pAsi)
	{
		assert(m_pDirectShow);
		int nID = bVideo? m_nVideoFilterID : m_nAudioFilterID;
		if (nID == -1) return FALSE;
		int nPinIndex = bVideo ? m_nVideoFilterOutputPinIndex : m_nAudioFilterOutputPinIndex;
		if (nPinIndex == -1) return FALSE;
		HRESULT hr = m_pDirectShow->GetStreamInfo(
			m_pDirectShow->GetFilterByID(nID),nPinIndex,pAsi);
		return SUCCEEDED(hr);
	}

	//
	void SetAudioVolumeHWnd(HWND hWnd) {
		//assert(m_pAudioVolumeFilter);
		if (m_pAudioVolumeFilter)
			m_pAudioVolumeFilter->SetHWnd(hWnd);
	}
	int GetAudioVolumeChannels() {
		//assert(m_pAudioVolumeFilter);
		if (m_pAudioVolumeFilter)
			return m_pAudioVolumeFilter->GetChannels();
		return -1;
	}
	BOOL GetAudioVolumedB(double *pddB) {
		//assert(m_pAudioVolumeFilter);
		if (m_pAudioVolumeFilter)
			return m_pAudioVolumeFilter->GetdB(pddB);
		return FALSE;
	}
	BOOL IsAudioVolumeEnabled() const {
		if (m_pAudioVolumeFilter)
			return m_pAudioVolumeFilter->IsCaptureEnabled();
		return FALSE;
	}
	void AudioVolumeEnableCapture(BOOL bEnable) {
		if (m_pAudioVolumeFilter)
			m_pAudioVolumeFilter->EnableCapture(bEnable);
		m_bAudioVolumeEnable = bEnable;
	}

private:

	CDirectShow* m_pDirectShow;	//DirectShow�N���X

	//�r�f�I����
	int m_nVideoIndex;			//�r�f�I�I���̃C���f�b�N�X(�f�o�C�X���̃C���f�b�N�X)
	int m_nVideoSettingIndex;	//�r�f�I�I��ݒ�̃C���f�b�N�X(�ݒ藪�̂̃C���f�b�N�X)

	int m_nVideoFilterID;			//�r�f�I�t�B���^��ID
	int m_nVideoSmartTeeID;			//�r�f�I����t�B���^��ID
	int m_nVideoColorSpaceID;		//�r�f�I�ϊ��t�B���^(�J���[�X�y�[�X)��ID
	int m_nVideoAVIDecompressorID;	//�r�f�I�ϊ��t�B���^(AVI�f�R���v���b�T)��ID
	int m_nVideoOverlayMixerID;		//�I�[�o�[���C�~�L�T��ID
	int m_nCrossbar1ID;				//��1�N���X�o�[�̃t�B���^��ID
	int m_nCrossbar2ID;				//��2�N���X�o�[�̃t�B���^��ID
	int m_nTVAudioID;  				//TV�I�[�f�B�I�̃t�B���^��ID
	int m_nTVTunerID;  				//TV�`���[�i�[�̃t�B���^��ID

	int m_nVideoFilterOutputPinIndex;	//�r�f�I�t�B���^�̏o�̓s���̔ԍ�
	BOOL m_bVideoFindAudioPin;		//�r�f�I�f�o�C�X�ɃI�[�f�B�I�s�������邩�ǂ���
	BOOL m_bUseAudioVideoFilter;	//�I�[�f�B�I�f�o�C�X�ƃr�f�I�f�o�C�X�����p���Ă���

	//�I�[�f�B�I����
	int m_nAudioIndex;			//�I�[�f�B�I�I���̃C���f�b�N�X
	int m_nAudioSettingIndex;	//�I�[�f�B�I�I��ݒ�̃C���f�b�N�X(�ݒ藪�̂̃C���f�b�N�X)

	int m_nAudioFilterID;				//�I�[�f�B�I�t�B���^��ID
	int m_nAudioSmartTeeID;				//�I�[�f�B�I����t�B���^��ID
	int m_nAudioRenderID;				//�I�[�f�B�I�����_����ID (IReferenceClock�p)
	int m_nAudioNullFilterID;			//�I�[�f�B�I�t�B���^��Null�����_����ID
	CDirectShowAudioVolume* m_pAudioVolumeFilter;	//�I�[�f�B�I�{�����[���̃T���v���O���o�̃��b�p�[�̃|�C���^

	int m_nAudioFilterOutputPinIndex;	//�I�[�f�B�I�t�B���^�̏o�̓s���̔ԍ�
	BOOL m_bAudioVolumeEnable;			//�I�[�f�B�I�{�����[�����L�����ǂ���

	//�r�f�I�v���r���[�̍���
	HWND m_hVideoWnd;			//�r�f�I�v���r���[�̃E�B���h�E�n���h��
	BOOL m_bVideoShow;			//�r�f�I�v���r���[���\������Ă��邩�ǂ����H
	int m_nVideoVMR;			//�r�f�I�~�L�V���O�����_���̑I��(0,1,7,9,10�̑I��)
	BOOL m_bVideoVMROverlay;	//�v���r���[�̃r�f�I�~�L�V���O�����_���̃I�[�o�[���C�m�ۂ̗L��
	BOOL m_bVideoForceOff;		//�r�f�I�v���r���[�������I�ɐ؂�ݒ�(�ċN�����ŗL���ɂȂ�)



	//////////////////////////////////////////////////
	//�G���R�[�_�[�Ɋւ���ݒ�
	//////////////////////////////////////////////////
public:
	vector<CodecStruct> m_VideoCodecList;			//�r�f�I�R�[�f�b�N
	vector<CodecStruct> m_AudioCodecList;			//�I�[�f�B�I�R�[�f�b�N
	vector<AudioCodecFormatStruct> m_AudioFormatList;	//���݂̃I�[�f�B�I�R�[�f�b�N�ݒ胊�X�g

	BOOL SelectVideoCodecIndex(int nIndex);
	BOOL SetVideoCodecSetting(const DWORD dwBitrate,const DWORD msBufferWindow,const double dFPS,const RECT rcSource,const RECT rcTarget);
	BOOL SetVideoCodecSetting2(const DWORD dwQuality,const int nMaxKeyFrameSpacing);
	BOOL SetVideoComplexity(const WORD wComplexity);

	BOOL SelectAudioCodecIndex(int nIndex);
	BOOL SelectAudioCodecFormatIndex(int nIndex);

	BOOL StartEncode();						//�G���R�[�h�̊J�n
	BOOL StopEncode(BOOL bForceDisconnect);	//�G���R�[�h�̒�~

	//�G���R�[�h�\���ǂ���
	inline BOOL IsCanEncode() const {
		return IsCanPreview() && IsRunFilter() && IsCanAudioPreview() && !IsEncode() &&
			(m_nVideoCodecIndex != -1) && (m_nAudioCodecIndex != -1) && (m_nAudioCodecFormatIndex != -1);}
	//(m_PullSinkList.size() != 0 || m_PushSinkList.size() != 0 || m_FileSinkList.size() != 0)

	//�G���R�[�h���Ă��邩�ǂ������擾
	inline BOOL IsEncode() const { return m_bStartEncode;}

private:
	//ASF���C�^�̃p�����[�^��K�p����(StartEncode()�̃T�u���[�`��)
	HRESULT SetAsfWriterParam();

	CDirectShowEncoder* m_pDirectShowEncoder;		//�G���R�[�_�[�N���X

	int m_bStartEncode;				//�G���R�[�h�����Ă��邩�ǂ����̐ݒ�

	int m_nVideoCodecIndex;			//���݂̃r�f�I�R�[�f�b�N�̃C���f�b�N�X
	int m_nAudioCodecIndex;			//���݂̃I�[�f�B�I�R�[�f�b�N�̃C���f�b�N�X
	int m_nAudioCodecFormatIndex;	//���݂̃I�[�f�B�I�R�[�f�b�N�̃t�H�[�}�b�g�C���f�b�N�X



	//////////////////////////////////////////////////
	//ASF���C�^�Ɋւ���ݒ�
	//////////////////////////////////////////////////
private:
	CDirectShowASFWriter* m_pDirectShowASFWriter;		//ASF���C�^�N���X
	CString m_strAsfTempName;							//ASF���C�^�̃e���|�����t�@�C��(c:\temp\�ɏo����t�@�C����)
public:
	//�v���o�͂�ǉ�
	BOOL AddPullSink(DWORD dwPort,DWORD dwMax,int &nID);
	//�v���o�͂��폜
	BOOL RemovePullSink(int nID);
	//�v���o�͂̓K�p
	HRESULT ApplyPullSink(int nIndex);
	//�v���o�͂�ύX(�ő�ڑ��l���̕ύX)
	BOOL ChangePullSink(int nID,DWORD dwMax);

	//�v���z�M�̏����擾
	BOOL GetPullSink(int nIndex,DWORD dwIPAddress,DWORD dwPort,NetworkClientStruct &Client,DWORD &dwSrcPort);
	//�v���o�͂������ȃf�[�^�ł���̂��`�F�b�N����
	BOOL IsVaildPullSink(int nIndex);

	//�v�b�V���o�͂�ǉ�
	BOOL AddPushSink(CString strServerName,BOOL bAutoDelete,int &nID);
	//�v�b�V���o�͂��폜
	BOOL RemovePushSink(int nID);
	//�v�b�V���o�͂̓K�p
	HRESULT ApplyPushSink(int nIndex);

	//�t�@�C���ɏo�͂�ǉ�
	BOOL AddFileSink(CString strFileName,int &nID);
	//�t�@�C���ɏo�͂��폜
	BOOL RemoveFileSink(int nID);
	//�t�@�C���ɏo�͂̓K�p
	HRESULT ApplyFileSink(int nIndex);

	vector<PullSinkStruct> m_PullSinkList;		//�v���V���N
	vector<PushSinkStruct> m_PushSinkList;		//�v�b�V���V���N
	vector<FileSinkStruct> m_FileSinkList;		//�t�@�C���V���N

	//���v�����擾
	BOOL GetStatistics(QWORD &cnsCurrentTime,WM_WRITER_STATISTICS &Stats,WM_WRITER_STATISTICS_EX &StatsEx);

	//�l�b�g���[�N�R�[���o�b�N�p�̃E�B���h�E�n���h�����֘A�t��
	void SetNetworkCallbackHWnd(HWND hWnd){
		m_hNetworkCallbackWnd = hWnd;}
	HWND GetNetworkCallbackHWnd()const{
		return m_hNetworkCallbackWnd;}

	//���^�f�[�^��ݒ�
	BOOL SetMetadata(CString &strTitle,CString &strAuthor,CString &strDescription,
		CString &strRating,CString &strCopyright);

private:
	//�S�ẴV���N(�v���E�v�b�V���E�t�@�C��)��K�p����
	BOOL ApplyAllSink();

	//�����^�u
	CString m_strTitle;			//�薼
	CString m_strAuthor;		//�쐬��
	CString m_strDescription;	//����
	CString m_strRating;		//�K��
	CString m_strCopyright;		//���쌠

	//�v���o�͂̐ڑ��E�ؒf�̃R�[���o�b�N���󂯎��E�B���h�E�n���h��
	HWND m_hNetworkCallbackWnd;
};

//////////////////////////////////////////////////
//[EOF]