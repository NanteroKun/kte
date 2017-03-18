#pragma once

#include "DirectShow.h"

//�R�[�f�b�N�񋓂Ɋւ���\����
typedef enum {
  CODEC_FORMAT_CBR    = 0,
  CODEC_FORMAT_VBR    = 1
} CODEC_FORMAT_BITRATE;
typedef enum {
  CODEC_FORMAT_ONEPASS    = 0,
  CODEC_FORMAT_TWOPASS    = 1
} CODEC_FORMAT_NUMPASS;

struct CodecStruct{
	GUID guidType;				//�擾����majorGUID�� (WMMEDIATYPE_Audio,WMMEDIATYPE_Video)
	wstring strName;			//�R�[�f�b�N�̖��O
	DWORD dwComplexityMax;		//�R�[�f�b�N�̕��G���̍ő�l(0�`���̒l�܂�)
	DWORD dwComplexityOffline;	//���[�J���ɕۑ�����ۂ̒񏥍œK�l
	DWORD dwComplexityLive;		//�X�g���[�~���O�z�M����ۂ̒񏥍œK�l
	BOOL  bIsVBRSupported;		//VBR���T�|�[�g���Ă��邩�ǂ���
	DWORD bIsTwoPassSupported;	//2�p�X�G���R�[�h���T�|�[�g���Ă��邩�ǂ���
	DWORD dwFormatCounts[2][2];	//�ێ����Ă���t�H�[�}�b�g�� dwFormatCounts[CODEC_FORMAT_BITRATE][CODEC_FORMAT_NUMPASS]
};
struct AudioCodecFormatStruct {
	wstring strDescription;		//�t�H�[�}�b�g�̐�����
	DWORD dwBitrate;			//�r�b�g���[�g
	WORD  nChannels;			//�`���l����
	DWORD nSamplesPerSec;		//�T���v�����O���[�g(Hz) 8000,11025,22050,44100
	WORD  wBitsPerSample;		//�r�b�g��(8,12,16)
	BOOL bAVSynchronization;	//A/V�������ۂ�
};

//////////////////////////////////////////////////
//�G���R�[�_�[�Ɋւ���N���X
//////////////////////////////////////////////////
class CDirectShowEncoder
{
public:
	CDirectShowEncoder(void);
	virtual ~CDirectShowEncoder(void);
	
	//�v���t�@�C���̍쐬
	HRESULT Create();
	//�v���t�@�C���̍폜
	void Delete();

	//�v���t�@�C���̓K�p
	HRESULT ApplyProfiles();
	//�v���t�@�C���̎擾
	inline IWMProfile* GetProfiles() {assert(m_pWMProfile);return m_pWMProfile;}

	//�v���t�@�C���̃t�@�C���Z�[�u
	HRESULT SaveProfile(IWMProfile* pWMProfile,LPCWSTR pwszFileName);

	//�v���t�@�C���̃��[�h
	HRESULT LoadProfile(LPCWSTR pwszFileName);

	HRESULT ListAllProfiles(WMT_VERSION dwVersion);					//OS��`�̃v���t�@�C���̑S�񋓂��\��
	HRESULT ListProfiles(IWMProfile* pWMProfile);					//�v���t�@�C���̗񋓂��\��
	HRESULT ListStreamConfig(IWMStreamConfig* pWMStreamConfig);		//�X�g���[�~���O�ݒ�̕\��
	HRESULT ListAllCodec(const GUID guidType);						//�R�[�f�b�N�̑S�񋓂��\��

	//�R�[�f�b�N�̑S�񋓂�CodecStruct�\���̂Ɏ擾����
	HRESULT EnumCodec(const GUID guidType,vector<CodecStruct> &Codec);
	//�I�[�f�B�I�R�[�f�b�N�t�H�[�}�b�g��񋓂�AudioCodecFormatStruct�\���̂Ɏ擾����
	HRESULT EnumAudioCodecFormat(const DWORD dwCodecIndex,CODEC_FORMAT_BITRATE format_bitrate,CODEC_FORMAT_NUMPASS format_numpass,vector<AudioCodecFormatStruct> &AudioFormat);

	//�I�[�f�B�I�R�[�f�b�N�̐ݒ�
	HRESULT SetAudioCodec(const DWORD dwCodecIndex,const DWORD dwFormatIndex,const CODEC_FORMAT_BITRATE format_bitrate,const CODEC_FORMAT_NUMPASS format_numpass){
		return SetCodec(true,dwCodecIndex,dwFormatIndex,format_bitrate,format_numpass);}
	//�r�f�I�R�[�f�b�N�̐ݒ�
	HRESULT SetVideoCodec(DWORD dwCodecIndex,CODEC_FORMAT_BITRATE format_bitrate,CODEC_FORMAT_NUMPASS format_numpass){
		return SetCodec(false,dwCodecIndex,0,format_bitrate,format_numpass);}

	//�Ǝ��̃r�f�I�R�[�f�b�N��ݒ�
	HRESULT SetOriginalCodec(const bool bAudio,AM_MEDIA_TYPE* pAmt);

	//�r�f�I�R�[�f�b�N�̕ҏW���ڂ�ݒ�
	HRESULT SetVideoSetting(const DWORD dwBitrate,const DWORD msBufferWindow,const REFERENCE_TIME AvgTimePerFrame,const RECT rcSource,const RECT rcTarget);
	HRESULT SetVideoSetting2(const DWORD dwQuality,const REFERENCE_TIME MaxKeyFrameSpacing);
	HRESULT SetVideoComplexity(const WORD wComplexity);

	//�r�f�I�R�[�f�b�N���̃t�@�C���Z�[�u(�f�o�b�O�p)
	HRESULT SaveCurrentVideoCodec(wstring strFileName);

	//�ő�E�ŏ��p�P�b�g�T�C�Y�̐ݒ�
	HRESULT SetPacketSize(const DWORD dwMinPacketSize, const DWORD dwMaxPacketSize);
	//���v�r�b�g���[�g�̎擾
	HRESULT GetTotalBitrate(DWORD* pdwAudioBitrate,DWORD* pdwVideoBitrate,DWORD* pdwAddedBitrate,DWORD* pdwTotalBitrate);

private:
	//�R�[�f�b�N�̑I�������Đݒ���s���T�u���[�`��(Audio,Video)
	HRESULT SetCodec(const bool bAudio,const DWORD dwCodecIndex,const DWORD dwFormatIndex,CODEC_FORMAT_BITRATE format_bitrate,CODEC_FORMAT_NUMPASS format_numpass);

	IWMProfile* m_pWMProfile;				//�v���t�@�C�����
	IWMStreamConfig* m_pVideoStreamConfig;	//�r�f�I�X�g���[�~���O�̏��
	IWMStreamConfig* m_pAudioStreamConfig;	//�I�[�f�B�I�X�g���[�~���O�̏��

	BOOL MakeStringToGUID(TCHAR* szGUID,int nSize,GUID &guid);
};

/*
//��VBR�Ɋւ���ݒ�
http://msdn.microsoft.com/en-us/library/dd756995%28VS.85%29.aspx

//���ш敝��L
IWMBandwidthSharing* pWMBandwidthSharing = NULL;
hr = CreateNewBandwidthSharing();
�悭�킩��Ȃ����Ǒш敝���L�錾���邱�ƂŁA
�D��I�ɑш悪���蓖�Ă���̂�������Ȃ�

*/