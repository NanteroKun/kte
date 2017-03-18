#pragma once

#include "DirectShow.h"

/*
�菇
	ASFWriter�̍쐬
	�R�[�f�b�N�̗�(Video��Audio)
	�R�[�f�b�N�̑I��(Video��Audio)
	�r�f�I�R�[�f�b�N�̐ݒ�̕ҏW(Video�̂�)
	ASFWriter�t�B���^�փt�B���^�̐ڑ�
	�v���t�@�C���̓K�p	ApplyProfiles();
	Sink�̒ǉ��ƍ폜
	�t�B���^�O���t�̎��s
*/

#define WM_NOTIFY_NETWORK_CLIENT_CONNECT    (WM_APP + 500)
#define WM_NOTIFY_NETWORK_CLIENT_DISCONNECT (WM_APP + 501)

//////////////////////////////////////////////////
//�l�b�g���[�N�\����
//////////////////////////////////////////////////
struct NetworkClientStruct
{
	wstring strNetworkAddress;	//IP�A�h���X(������)
	int   nNetworkAddress[4];	//IP�A�h���X(���l)
	DWORD dwNetworkAddress;		//IP�A�h���X
	DWORD dwPort;				//�N���C�A���g���̃|�[�g�ԍ�
	wstring strDNSName;			//DNS��(������)
};

//////////////////////////////////////////////////
//�R�[���o�b�N�p�̃C���^�t�F�[�X
//////////////////////////////////////////////////
class CDirectShowASFWriterNetworkCallback : public IWMStatusCallback
{
public:
	//�R���X�g���N�^
	CDirectShowASFWriterNetworkCallback();
	//�f�X�g���N�^
	~CDirectShowASFWriterNetworkCallback();
	//�R�[���o�b�N
    virtual HRESULT STDMETHODCALLTYPE OnStatus(WMT_STATUS Status,HRESULT hr,WMT_ATTR_DATATYPE dwType,BYTE __RPC_FAR *pValue,void __RPC_FAR *pvContext);
	//�Q�Ɖ�
	ULONG STDMETHODCALLTYPE AddRef(void){return 1;}
	ULONG STDMETHODCALLTYPE Release(void){return 1;}
	//�C���^�t�F�[�X�擾
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid,void __RPC_FAR *__RPC_FAR *ppvObject);
	void SetHWnd(HWND hWnd){m_hWnd = hWnd;}
	HWND GetHWnd()const{return m_hWnd;}
private:
	HWND m_hWnd;
};


//////////////////////////////////////////////////
//DirectShow WM ASF Writer�Ɋւ���N���X
//////////////////////////////////////////////////
class CDirectShowASFWriter
{
public:
	//�R���X�g���N�^
	CDirectShowASFWriter(void);
	//�f�X�g���N�^
	virtual ~CDirectShowASFWriter(void);

	//ASF Writer���쐬����
	HRESULT Create(IGraphBuilder* pGraphBuilder,LPCTSTR lpszDefaultFileName);
	//ASF Writer���폜����
	void Delete(IGraphBuilder* pGraphBuilder);
	//�S�Ă�Sink���폜����
	HRESULT DeleteAllSink();

	//WM ASF Writer�t�B���^�̎擾
	inline IBaseFilter* GetFilter(){assert(m_pAsfWriter);return m_pAsfWriter;}

	//�v���t�@�C���̓K�p
	HRESULT ApplyProfiles(IWMProfile* pProfile);

	//���݂̃r�f�I/�I�[�f�B�I�s���̎󂯓���t�H�[�}�b�g���擾����
	HRESULT GetFormat(AM_MEDIA_TYPE &amt,BOOL bVideo = TRUE);

	//ASF Writer�Ɋւ��鏔�ݒ��K�p����
	HRESULT SetAsfSyncTolerance(const DWORD dwWindow);
	HRESULT SetAsfIndexMode(const BOOL bIndexFile);
	HRESULT SetAsfParam(const DWORD dwSettingParam,const DWORD dwParam);
	HRESULT SetFixedFrameRate(BOOL bFixedFrameRate);


	//�t�@�C���ɏo��
	HRESULT AddFileSink(const WCHAR* pwszFileName,int* pnIndex);
	HRESULT RemoveFileSink(const int nIndex);

	//�u���[�h�L���X�g�o��
	HRESULT AddNetworkSink(DWORD* pdwPortNum,const DWORD dwMaxClients,int* pnIndex);
	HRESULT RemoveNetworkSink(const int nIndex);
	HRESULT ChangeNetworkSink(const int nIndex,const DWORD dwMaxClients);

	//�u���[�h�L���X�g�o�͂ł̐ڑ����ꂽ�����擾
	HRESULT GetNetworkSink(const int nIndex,DWORD dwIPAddress,DWORD dwPort,NetworkClientStruct &Client);

	//�v�b�V���o��
	HRESULT AddPushSink(LPCWSTR pwszURL,LPCWSTR pwszTemplateURL,const BOOL fAutoDestroy,int* pnIndex);
	HRESULT RemovePushSink(const int nIndex);

	//���v�����擾
	HRESULT GetStatistics(QWORD &cnsCurrentTime,WM_WRITER_STATISTICS &Stats,WM_WRITER_STATISTICS_EX &StatsEx);

	//���^�f�[�^��ݒ肷��
	void SetTitle(LPCTSTR strTitle) { m_strTitle = strTitle;}
	void SetAuthor(LPCTSTR strAuthor) { m_strAuthor = strAuthor;}
	void SetDescription(LPCTSTR strDescription) { m_strDescription = strDescription;}
	void SetRating(LPCTSTR strRating) { m_strRating = strRating;}
	void SetCopyright(LPCTSTR strCopyright) { m_strCopyright = strCopyright;}

	//�R�[���o�b�N�p�̃E�B���h�E�n���h�����֘A�t��
	void SetNetworkCallbackHWnd(HWND hWnd){
		assert(m_pNetworkCallback);
		m_pNetworkCallback->SetHWnd(hWnd);}
	HWND GetNetworkCallbackHWnd()const{
		assert(m_pNetworkCallback);
		return m_pNetworkCallback->GetHWnd();}

private:
	IBaseFilter* m_pAsfWriter;					//WM ASF Writer �t�B���^
	IWMWriterAdvanced3 *m_pWMWriterAdvanced3;	//IWMWriterSink�ւ̖₢���킹�C���^�t�F�[�X
	IConfigAsfWriter2* m_pConfigAsfWriter2;		//ASF���C�^�̐ݒ�


	BOOL m_bIsWriterAdvanced3;

	vector<IWMWriterNetworkSink*> m_NetworkSinkList;
	vector<IWMWriterPushSink*>    m_PushSinkList;
	vector<IWMWriterFileSink*>    m_FileSinkList;

	//���^�f�[�^
	IWMHeaderInfo* m_pWMHeaderInfo;
	wstring m_strTitle;			//�^�C�g��
	wstring m_strAuthor;		//�����
	wstring m_strDescription;	//����
	wstring m_strRating;		//�K��
	wstring m_strCopyright;		//����

	//���^�f�[�^�̐ݒ�
	HRESULT SetAttribute(int nIndex);

	//�l�b�g���[�N�V���N�̃R�[���o�b�N�p�̃N���X
	CDirectShowASFWriterNetworkCallback* m_pNetworkCallback;
};

