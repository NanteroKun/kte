#pragma once

//#pragma comment(lib,"libcpmt.lib")

#include "qedit.h"
#include "DirectShowEtc.h"

#define	WM_GRAPH_NOTIFY		(WM_APP + 505)

//�t�B���^�̃s�����̍\����
struct PinInfoStruct
{
	PIN_DIRECTION PinDir;	//�s���̕���(PINDIR_INPUT or PINDIR_OUTPUT)
	wstring strPinName;		//�s���̖��O
	BOOL bConnected;		//�ڑ�����Ă��邩�ǂ���
};

//////////////////////////////////////////////////
//DirectShow�̐���N���X
//////////////////////////////////////////////////
class CDirectShow
{
public:
	//�R���X�g���N�^
	CDirectShow(void);
	//�f�X�g���N�^
	virtual ~CDirectShow(void);

	//DirectShow�̏�����
	bool Init(HINSTANCE hInst);
	//DirectShow�̏I��
	void Exit();

	//�r�f�I�����_���̏�����
	bool InitVideoRenderer(int nVMR,BOOL CanUseOverlay);

	//�r�f�I�����_���ɃE�B���h�E�n���h�����֘A�t������
	bool SetVideoRendererHWnd(HWND hWnd);
	//�r�f�I�����_���̃E�B���h�E�T�C�Y��ύX����
	bool ResizeVideoRendererHWnd(RECT rcClient);
	//�r�f�I�����_������E�B���h�E�n���h�����폜����
	bool ResetVideoRendererHWnd();
	//�r�f�I�����_���̕\���E��\����؂�ւ���
	bool ShowVideoRenderer(BOOL bShow);
	bool IsShowVideoRenderer();

	//�R�[���o�b�N��̃E�B���h�E�n���h����ݒ肷��
	bool SetCallbackHWnd(HWND hWnd);
	//�R�[���o�b�N����Ăяo�����֐�
	LRESULT OnGraphNotify();

	//�r�f�I���Đ��I������܂ő҂�
	bool WaitForCompletion(int nTimeOut);

	//�I�[�f�B�I�����_���̏�����
	bool InitAudioRenderer();

	//�O���t�r���_�̎擾
	inline IGraphBuilder* GetGraphBuilder() {assert(m_pGraphBuilder);return m_pGraphBuilder;}
	//�r�f�I�����_���̎擾
	inline IBaseFilter* GetVideoRenderer() { /*assert(m_pVideoRenderer)*/;return m_pVideoRenderer; }
	//�I�[�f�B�I�����_���̎擾
	inline IBaseFilter* GetAudioRenderer() { assert(m_pAudioRenderer);return m_pAudioRenderer; }

	HRESULT SetState(OAFilterState ofs);				//�t�B���^�O���t�̏�Ԃ�ύX
	OAFilterState GetState();							//�t�B���^�O���t�̏�Ԃ��擾
	HRESULT Run(){return SetState(State_Running);}		//�t�B���^�O���t�̍Đ�
	HRESULT Stop(){return SetState(State_Stopped);}		//�t�B���^�O���t�̒�~
	HRESULT Pause(){return SetState(State_Paused);}		//�t�B���^�O���t�̈ꎞ��~
		
	//�t�@�C������C���e���W�F���g�ڑ��Ń����_�����O
	bool RenderFile(BSTR strFilename);

	//�t�B���^���m��ڑ�����(�_�C���N�g�ڑ�)
	HRESULT ConnectFilterDirect(IBaseFilter* pSrc,IBaseFilter* pDest,AM_MEDIA_TYPE* pAmt = NULL,LPCWSTR pSrcPinName = NULL,LPCWSTR pDestPinName = NULL);
	//�t�B���^������ڑ�����(�R���o�[�g�ڑ�)
	HRESULT ConnectFilterConvert(IBaseFilter* pSrc,IBaseFilter* pDest,AM_MEDIA_TYPE* pAmt = NULL,LPCWSTR pSrcPinName = NULL,int* pID = NULL);

	//�t�B���^�̑S�Ẵs����ؒf����
	HRESULT DisconnectFilter(IBaseFilter* pFilter,BOOL bDisconnectInputPins,BOOL bDisconnectOutputPins);
	//�t�B���^���1�̃s����ؒf����
	HRESULT DisconnectFilterPin(IBaseFilter* pFilter,int nPinIndex);

	//�t�B���^�̏����擾����(�s�����E�s���̖��O�E�s�����ڑ�����Ă��邩�ǂ���)
	HRESULT GetFilterInfo(IBaseFilter* pFilter,vector<PinInfoStruct>&PinInfoList,int* pInputPinCount,int* pOutputPinCount);
	//���́E�o�̓s���̔ԍ����擾����(���́E�o�̓s�����P�����Ȃ��ꍇ)
	HRESULT GetPinIndex(IBaseFilter* pFilter,int &nPinIndex,PIN_DIRECTION pin_dir);
	HRESULT GetOutputPinIndex(IBaseFilter* pFilter,int &nOutputPinIndex) { return GetPinIndex(pFilter,nOutputPinIndex,PINDIR_OUTPUT);}
	HRESULT GetInputPinIndex(IBaseFilter* pFilter,int &nInputPinIndex)   { return GetPinIndex(pFilter,nInputPinIndex,PINDIR_INPUT);}

	//�s���̗D�惁�f�B�A�^�C�v��񋓂���
	HRESULT EnumMediaType(IBaseFilter* pFilter,int nPinIndex,vector<AM_MEDIA_TYPE> &MediaTypeList);
	//���ɐڑ�����Ă���s���̃��f�B�A�^�C�v���擾����
	HRESULT GetConnectionMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE &amt);

	//�s�����o�̓��f�B�A�^�C�v�ύX�\�Ȃ�ݒ���s��
	//HRESULT SetMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE* pamt);

	//�v���p�e�B�y�[�W�̕\��
	HRESULT ShowPropertyDialog(IBaseFilter *pFilter,HWND hWnd);

	//�L���v�`���f�o�C�X�̒ǉ�
	bool EnumCaptureDevice(vector<wstring>& strDeviceStrings,bool bVideo,bool bName);
	bool AddCaptureDevice(wstring strDeviceName,bool bVideo,bool bName,int* pID);

	//�L���v�`���f�o�C�X�̗�(�r�f�I/�I�[�f�B�I , ���O/ID)
	bool EnumVideoCaptureDeviceName(vector<wstring>& strDeviceStrings) {return EnumCaptureDevice(strDeviceStrings,true,true);}
	bool EnumVideoCaptureDeviceID(vector<wstring>& strDeviceStrings)   {return EnumCaptureDevice(strDeviceStrings,true,false);}
	bool EnumAudioCaptureDeviceName(vector<wstring>& strDeviceStrings) {return EnumCaptureDevice(strDeviceStrings,false,true);}
	bool EnumAudioCaptureDeviceID(vector<wstring>& strDeviceStrings)   {return EnumCaptureDevice(strDeviceStrings,false,false);}
	//�L���v�`���f�o�C�X�̒ǉ�(�r�f�I/�I�[�f�B�I , ���O/ID)
	bool AddVideoCaptureDeviceByName(wstring strDeviceName,int* pID)   {return AddCaptureDevice(strDeviceName,true,true,pID);}
	bool AddVideoCaptureDeviceByID(wstring strDeviceName,int* pID)     {return AddCaptureDevice(strDeviceName,true,false,pID);}
	bool AddAudioCaptureDeviceByName(wstring strDeviceName,int* pID)   {return AddCaptureDevice(strDeviceName,false,true,pID);}
	bool AddAudioCaptureDeviceByID(wstring strDeviceName,int* pID)     {return AddCaptureDevice(strDeviceName,false,false,pID);}

	//�L���v�`���f�o�C�X(�r�f�I)�̃��f�B�A�^�C�v��񋓂���
	HRESULT EnumVideoCaptureMediaType(IBaseFilter* pFilter, int nPinIndex, vector<AM_MEDIA_TYPE> &MediaTypeList,vector<VIDEO_STREAM_CONFIG_CAPS> &VideoCapsList);
	//�L���v�`���f�o�C�X(�I�[�f�B�I)�̃��f�B�A�^�C�v��񋓂���
	HRESULT EnumAudioCaptureMediaType(IBaseFilter* pFilter, int nPinIndex, vector<AM_MEDIA_TYPE> &MediaTypeList,vector<AUDIO_STREAM_CONFIG_CAPS> &AudioCapsList);

	//�L���v�`���f�o�C�X�Ƀ��f�B�A�^�C�v�̐ݒ���s��
	HRESULT SetMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE* pAmt);
	//�L���v�`���f�o�C�X�̃��f�B�A�^�C�v���擾����
	HRESULT GetMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE &Amt);

	HRESULT GetSetCaptureMediaType(IBaseFilter* pFilter,int nPinIndex,BOOL bSet,AM_MEDIA_TYPE* pAmt);

	//�r�f�I���k�Ɋւ�����𓾂�
	HRESULT GetVideoCompressInfo(IBaseFilter* pFilter,int nPinIndex);

private:
	//�s�����擾����T�u���W���[��
	//HRESULT GetPin(IBaseFilter *pFilter, PIN_DIRECTION dir, CComPtr<IPin> &pin, GUID majorType,LPCWSTR pPinName);

	//�s�����擾����T�u���W���[��
	HRESULT GetPin(IBaseFilter* pFilter,PIN_DIRECTION pin_direction,CComPtr<IPin> &pin,AM_MEDIA_TYPE* pAmt,LPCWSTR pPinName);

	//�����o�ϐ�
	bool m_bComInit;									//COM������������Ă��邩�ǂ���

	IGraphBuilder*         m_pGraphBuilder;				//�t�B���^�O���t�}�l�[�W���[
	ICaptureGraphBuilder2* m_pCaptureGraphBuilder2;		//�L���v�`���p�̃O���t�r���_
	IMediaControl*         m_pMediaControl;				//�X�g���[�~���O�𐧌䂷�郁�\�b�h
	IMediaEventEx*         m_pMediaEventEx;				//�C�x���g�ʒm�p�̃C���^�t�F�[�X
	IVideoWindow*          m_pVideoWindow;				//�E�B���h�E�n���h���C���^�t�F�[�X
	IReferenceClock*       m_pReferenceClock;			//��N���b�N

	IBaseFilter*           m_pVideoRenderer;			//�W���̃r�f�I�����_���t�B���^�[
	IBaseFilter*           m_pAudioRenderer;			//�W���̃I�[�f�B�I�����_���t�B���^�[(DirectSound)


	//////////////////////////////////////////////////
	//�t�B���^�̌^�̍\����
	struct FilterData {
		IBaseFilter* pFilter;	//�t�B���^�̃|�C���^
		GUID         guid;		//�t�B���^��GUID
		wstring      strName;	//�t�B���^��
	};
	vector<FilterData> m_FilterData;					//�t�B���^���X�g

public:
	//�t�B���^���Ǘ��t�B���^�ɒǉ�
	HRESULT AddFilterList(IBaseFilter* pFilter,int* pID);
	//�t�B���^�̒ǉ�
	HRESULT AddFilter(GUID guid,LPCWSTR strName,int* pID);

	//DMO�t�B���^�̒ǉ�
	HRESULT AddDMOFilter(GUID guid,GUID guidCategory,LPCWSTR strName,int* pID);

	//�t�B���^�̏���
	bool RemoveFilter(IBaseFilter* pFilter);
	//
	IBaseFilter* GetFilterByID(const int nID);
	IBaseFilter* GetFilterByName(LPCWSTR strName);
	//////////////////////////////////////////////////

private:
	DWORD m_dwRegister;			//ROT�̃��W�X�^ID
	bool m_bRegister;			//ROT�Ƀ��W�X�g���Ă��邩�ǂ���

public:

	//�O���t�N���b�N(��N���b�N)�̐ݒ�
	HRESULT SetGraphClock(BOOL bUse,BOOL bDefaultSyncSource,BOOL bMakeSystemClock,IBaseFilter* pFilter);
	//��N���b�N���猻�݂̊�^�C�����擾����
	HRESULT GetGraphClockTime(REFERENCE_TIME &rtTime);
	//��N���b�N�̕���\���擾����
	HRESULT GetGraphClockResolution(REFERENCE_TIME &rtResolution);
	//�t�B���^�O���t�ɓ����̉ۂ�ݒ肷��B�܂��A�K�v������΍ő�x�����Ԃ�ݒ肷��
	HRESULT SyncStreamOffset(BOOL bUseStreamOffset,REFERENCE_TIME* prtMaxGraphLatency);
	//�A���P�[�^�[�o�b�t�@�̐ݒ�
	HRESULT SetGetAllocatorBuffer(IBaseFilter* pFilter,int nPinIndex,BOOL bSet,ALLOCATOR_PROPERTIES &AllocProp);

	//�t�B���^�J�n�ȍ~�̃t���[�����E�h���b�v���Ȃǂ��擾
	HRESULT GetDropFrame(IBaseFilter* pFilter,int nPinIndex,long* plNotDropped,long* plDropped,long* plAverageSize);
	//�v�b�V���\�[�X�t�B���^�̐��\�Ə��𓾂�
	HRESULT GetPushSourceInfo(IBaseFilter* pFilter,int nPinIndex,REFERENCE_TIME *prtLatency,ULONG* pFlags,REFERENCE_TIME *prtOffset,REFERENCE_TIME *prtMaxOffset);
	//���݂̃X�g���[�~���O�����擾
	HRESULT GetStreamInfo(IBaseFilter* pFilter,int nPinIndex,AM_STREAM_INFO* pAsi);
	//WDM�h���C�o�\�[�X�t�B���^���玩���ŕK�{�t�B���^�̒ǉ����s��
	HRESULT AddWDMFilter(IBaseFilter* pFilter,BOOL bVideo,int* pCrossbar1ID,int* pCrossbar2ID,int* pTVAudioID,int* pTVTunerID);
};

	//TODO �I�[�f�B�I�L���v�`���f�o�C�X�̃{�����[���ύX�Ɋւ��錟�؃R�[�h
	//HRESULT SetAudioCaptureConfig(IBaseFilter* pFilter);
	//http://msdn.microsoft.com/ja-jp/library/cc352249.aspx


//////////////////////////////////////////////////
//[EOF]
