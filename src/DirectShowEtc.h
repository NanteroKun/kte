#pragma once

#define CHECK_RESULT(str,hr) CDirectShowEtc::CheckResult(_T(str),hr,__FILE__,__LINE__)

//�t�B���^�̏��\����
struct FilterInfoStruct
{
	GUID guid;			//�t�B���^��GUID
	wstring strName;	//�t�B���^�̖��O
};

class CDirectShowEtc
{
public:
	//////////////////////////////////////////////////
	//���f�B�A�^�C�v�֘A�̃T�u���[�`��
	//���f�B�A�^�C�v�̃t�H�[�}�b�g�u���b�N���J������
	static HRESULT ReleaseMediaType(AM_MEDIA_TYPE &amt);
	static HRESULT ReleaseMediaTypeList(vector<AM_MEDIA_TYPE> &MediaTypeList);

	//////////////////////////////////////////////////
	//�f�o�b�O�֘A�̃T�u���[�`��
	//GRF�t�@�C���̃Z�[�u
	static HRESULT SaveGraphFile(IGraphBuilder* pGraphBuilder,WCHAR *wszPath);
	//ROT�e�[�u���ɒǋL
	static HRESULT AddToRot(IGraphBuilder* pGraphBuilder,DWORD &dwRegister);
	static void RemoveFromRot(DWORD &dwRegister);
	//DirectShow�̖߂�l���`�F�b�N����T�u���W���[��
	static bool CheckResult(TCHAR* string,HRESULT hr,const char* pFile,int nLine);
	//�f�o�b�O�E�B���h�E�ɏo��
	static void OutputDebugWindow(LPCTSTR pString, ...);

	//////////////////////////////////////////////////
	//�s���֘A�̃T�u���[�`��
	static IPin* GetPinByIndex(IBaseFilter* pFilter,int nPinIndex);
	static IPin* GetPinByName(IBaseFilter* pFilter,PIN_DIRECTION PinDir,LPCWSTR pPinName,const AM_MEDIA_TYPE* pAmt);
	//�t�B���^�̐ڑ��`�F�b�N
	static BOOL CheckConnectionFilter(IBaseFilter* pFilter,int* pInputCount,int* pOutputCount);

	//////////////////////////////////////////////////
	//GUID�̕�����ϊ��̃T�u���[�`��
	static BOOL GUIDToString(TCHAR* szGUID,int nSize,const GUID &guid);
	static BOOL StringToGUID(LPTSTR szGUID,GUID &guid);
	static unsigned long MakeGUIDSubRoutine(LPTSTR szGUID,int nStart,int nLength);
	//���[�W���[�^�C�v�̕�����ϊ�
	static BOOL MakeStringToMajorTypeGUID(TCHAR* szGUID,int nSize,GUID &guid);
	static BOOL MakeStringToSubTypeGUID(TCHAR* szGUID,int nSize,GUID &guid);
	//�r�f�I�T�u�^�C�v�̕�����ϊ�
	static BOOL MakeStringToVideoSubTypeGUID(TCHAR* szGUID,int nSize,GUID &guid);
	//�I�[�f�B�I�T�u�^�C�v�̕�����ϊ�
	static BOOL MakeStringToAudioSubTypeGUID(TCHAR* szGUID,int nSize,GUID &guid);
	//�X�g���[���T�u�^�C�v�̕�����ϊ�
	static BOOL MakeStringToStreamSubTypeGUID(TCHAR* szGUID,int nSize,GUID &guid);

	//�t�H�[�}�b�g�^�C�v�̕�����ϊ�
	static BOOL MakeStringToFormatTypeGUID(TCHAR* szGUID,int nSize,GUID &guid);

	//�_���v�o��
	static BOOL DumpMediaType(AM_MEDIA_TYPE* pamt,LPCTSTR lpszFileName);
	static BOOL DumpMediaType(AM_MEDIA_TYPE* pamt,FILE* fp);
	static BOOL DumpVideoStreamConfigCaps(VIDEO_STREAM_CONFIG_CAPS* pvscc,LPCTSTR lpszFileName);
	static BOOL DumpVideoStreamConfigCaps(VIDEO_STREAM_CONFIG_CAPS* pvscc,FILE* fp);
	static BOOL DumpAudioStreamConfigCaps(AUDIO_STREAM_CONFIG_CAPS* pascc,LPCTSTR lpszFileName);
	static BOOL DumpAudioStreamConfigCaps(AUDIO_STREAM_CONFIG_CAPS* pascc,FILE* fp);

	//���ׂẴt�B���^�̗�
	static HRESULT EnumAllFilter(const GUID &guidCategory,vector<FilterInfoStruct> &FilterList);
	static HRESULT EnumAllDMOFilter(const GUID &guidCategory,vector<FilterInfoStruct> &FilterList);
	static HRESULT EnumMatchFilter(const GUID &guidMediaType,const GUID &guidMediaSubType,vector<FilterInfoStruct> &FilterList);

	//AM_MEDIA_TYPE����L���v�`���ݒ�̕�����𐶐�����T�u���[�`��
	static BOOL MakeVideoMediaString(const vector<AM_MEDIA_TYPE> &MediaTypeList,vector<wstring> &strVideoSettingList);
	static BOOL MakeAudioMediaString(const vector<AM_MEDIA_TYPE> &MediaTypeList,vector<wstring> &strAudioSettingList);
	//AM_MEDIA_TYPE����_�~�[��AUDIO_STREAM_CONFIG_CAPS���X�g�����T�u���[�`��
	static BOOL MakeDummyAudioCapsList(const vector<AM_MEDIA_TYPE> &MediaTypeList,vector<AUDIO_STREAM_CONFIG_CAPS> &AudioStreamConfigCapsList);

	//�C���X�g�[������Ă���t�B���^�̌���(�t�����h�����ł̌���)
	static HRESULT FindInstallFilter(wstring strFilterName,BOOL &bFind);
};
