#pragma once


//�v���O�C���f�[�^�ǂݏ����̍ۂ̍\����
struct IniPluginStruct {
	GUID guid;				//GUID
	GUID guidCategory;		//GUID�J�e�S��
	CString strFilterName;	//�t�B���^��
	BOOL bEnable;			//�L���E����
};
struct IniOutputBasicStruct {
	int nIndex;				//�C���f�b�N�X
	BOOL bTabEnable;		//�L�����������ǂ���
	CString strTabName;		//�^�u��
};
//�v���o�̓f�[�^�ǂݏ����̍ۂ̍\����
struct IniOutputPullStruct {
	IniOutputBasicStruct Data;
	int nPort;				//�|�[�g�ԍ�
	int nMax;				//�ő�ڑ���
};
//�v�b�V���o�̓f�[�^�ǂݏ����̍ۂ̍\����
struct IniOutputPushStruct {
	IniOutputBasicStruct Data;
	CString strServerName;	//�T�[�o�[��
	BOOL bAutoDelete;		//�����폜
};
//�t�@�C���o�̓f�[�^�ǂݏ����̍ۂ̍\����
struct IniOutputFileStruct {
	IniOutputBasicStruct Data;
	CString strFileName;	//�o�̓t�@�C����
	CString strFolderName;	//�o�̓t�H���_��
};

//////////////////////////////////////////////////
//�v���t�@�C���̓ǂݏ������s���N���X
//�v���t�@�C���͕����ݒ�ۑ��\�Ȑݒ�f�[�^�̂���
//�v���O�����N�����Ƀf�t�H���g�̃v���t�@�C�����ǂݍ��܂�A�I�����Ƀv���t�@�C���̕ۑ����s��
//�v���t�@�C���̓v���O�����̓r���ł��ǂݏ������s����
//////////////////////////////////////////////////
class CProfile
{
public:
	CProfile(void);				//�R���X�g���N�^
	virtual ~CProfile(void);	//�f�X�g���N�^
	void DefaultProfile();		//�����l�ɖ߂�
	BOOL LoadProfile(LPCTSTR lpszProfileName,BOOL bFirstFlag);	//�v���t�@�C���̓ǂݍ���
	BOOL SaveProfile(LPCTSTR lpszProfileName);					//�v���t�@�C���̏�������


	//////////////////////////////////////////////////
	//�l�X�ȃv���t�@�C�����ڂ͂����ɏ���
	//////////////////////////////////////////////////

	//���̓^�u�̐ݒ�
	CString m_strVideoName;			//�r�f�I�L���v�`����
	CString m_strVideoSettingName;	//�r�f�I�L���v�`���̐ݒ薼
	int     m_nVideoSettingIndex;	//�r�f�I�L���v�`���̐ݒ�C���f�b�N�X
	int     m_nVideoSettingWidth;	//�r�f�I�L���v�`���̕�
	int     m_nVideoSettingHeight;	//�r�f�I�L���v�`���̍���
	double  m_dVideoSettingFPS;		//�r�f�I�L���v�`���̃t���[�����[�g

	CString m_strAudioName;			//�I�[�f�B�I�L���v�`����
	CString m_strAudioSettingName;	//�I�[�f�B�I�L���v�`���̐ݒ薼
	int		m_nAudioSettingIndex;	//�I�[�f�B�I�L���v�`���̐ݒ�C���f�b�N�X
	BOOL    m_bAudioInputVideoPin;	//�I�[�f�B�I�L���v�`�����r�f�I�s������Ƃ邩�ۂ�
	DWORD   m_nSamplesPerSec;		//�I�[�f�B�I�L���v�`���̎��g��
	WORD    m_wBitsPerSample;		//�I�[�f�B�I�L���v�`���̕���\
	WORD    m_nChannels;			//�I�[�f�B�I�L���v�`���̃`�����l��
	BOOL m_bAudioBufferEnable;		//�I�[�f�B�I�o�b�t�@�ݒ�̉�
	int m_nAudioBufferTime;			//�I�[�f�B�I�o�b�t�@�̎���(�~���b)
	int m_nAudioBufferCount;		//�I�[�f�B�I�o�b�t�@�̌�

	BOOL m_bGraphClockEnable;		//��N���b�N�̐ݒ�̗L��
	int m_nGraphClockSetting;		//��N���b�N�̑I��(0�`2)
	BOOL m_bGraphSyncEnable;		//��N���b�N�̓����̗L��

	//�v���O�C���̐ݒ�
	int m_nVideoPluginCount;					//�r�f�I�v���O�C���̌�
	vector<IniPluginStruct> m_VideoPluginList;	//�r�f�I�v���O�C���̔z��
	int m_nAudioPluginCount;					//�I�[�f�B�I�v���O�C���̌�
	vector<IniPluginStruct> m_AudioPluginList;	//�I�[�f�B�I�v���O�C���̔z��

	//���k�^�u�̐ݒ�(�r�f�I�G���R�[�h)
	CString m_strVideoEncodeName;	//�r�f�I�G���R�[�_�̖��O
	int m_nVideoEnocdeBitrate;		//�r�f�I�r�b�g���[�g�l(�\����̐��� �P�ʂ�Kbps)
	double m_dVideoEncodeFramerate;	//�r�f�I�t���[�����[�g(�P�ʂ�fps)
	double m_dVideoEncodeMaxFramerateSetting;	//�r�f�I�G���R�[�h�̍ő�t���[�����[�g�ݒ�(�P�ʂ�fps)
	BOOL m_bVideoEncodeSize;		//�r�f�I�G���R�[�h�̃T�C�Y�ύX
	int m_nVideoEncodeWidth;		//�r�f�I�G���R�[�h�̕�
	int m_nVideoEncodeHeight;		//�r�f�I�G���R�[�h�̍���
	int m_nVideoEnocdeQuality;		//�r�f�I�G���R�[�h�̊��炩��
	int m_nBufferWindow;			//�o�b�t�@�����O����	(�P�ʂ�ms�B�\����ɂ͏o�Ă��Ȃ�)
	int m_nMaxKeyFrameSpacing;		//�L�[�t���[���̊Ԋu	(�P�ʂ�ms�B�\����ɂ͏o�Ă��Ȃ�)
	BOOL m_bMaxKeyFrameSpacing;		//�L�[�t���[���̊Ԋu�������Ō��肷�邩�ǂ����H
	int m_nVideoComplexity;			//�r�f�I�̕��G��
	BOOL m_bVideoComplexity;		//�r�f�I�̕��G���������Ō��߂邩�ǂ���

	//���k�^�u�̐ݒ�(�I�[�f�B�I�G���R�[�h)
	CString m_strAudioEncodeName;		//�I�[�f�B�I�G���R�[�_�[�̖��O
	//CString m_AudioEncodeDescription;	//�I�[�f�B�I�t�H�[�}�b�g�̐���
	int m_nAudioEncodeBitrate;			//�r�b�g���[�g		(�P�ʂ�bps)
	int m_nAudioEncodeChannels;			//�`�����l����      (�ʏ��2)
	int m_nAudioEncodeSamplesPerSec;	//�T���v�����O���[�g(�ʏ��44100)
	int m_nAudioEncodeBitsPerSample;	//�r�b�g��          (�ʏ��16)
	BOOL m_bAudioEncodeAVSynchronization;	//AV����		(�ʏ��TRUE)

	//�o�̓^�u�̐ݒ�
	BOOL m_bOutputPullCheck;			//�u���[�h�o���h�o�͂̃`�F�b�N
	BOOL m_bOutputPushCheck;			//�p�u���b�V���|�C���g�o�͂̃`�F�b�N
	BOOL m_bOutputFileCheck;			//�t�@�C���o�͂̃`�F�b�N
	int m_nOutputPullCount;				//�u���[�h�o���h�o�͂̐�
	int m_nOutputPushCount;				//�p�u���b�V���|�C���g�o�͂̐�
	int m_nOutputFileCount;				//�t�@�C���o�͂̐�
	vector<IniOutputPullStruct> m_OutputPullList;
	vector<IniOutputPushStruct> m_OutputPushList;
	vector<IniOutputFileStruct> m_OutputFileList;

	//�o�̓^�u�̏ڍאݒ�
	int  m_nOutputMaxPacketSize;		//�ő�p�P�b�g�T�C�Y(�ʏ�̓[��)
	int  m_nOutputMinPacketSize;		//�ŏ��p�P�b�g�T�C�Y(�ʏ�̓[��)
	BOOL m_bOutputInsertIndex;			//�C���f�b�N�X���[�h(�V�[�N�\�ȃr�f�I���o�͂���)
	BOOL m_bOutputInsertIndexEnd;		//�G���R�[�h�I�����ɃC���f�b�N�X��t�^����
	int  m_nOutputSyncToleranceTime;	//�l�b�g���[�N�ɏo�͂���ۂ̃o�b�t�@�T�C�Y(�ʏ��3000�~���b)
	BOOL m_bOutputForceDisconnect;		//�G���R�[�h�I�����Ƀl�b�g���[�N�ڑ��������ؒf����

	//�����^�u�̐ݒ�
	CString m_strMetadataTitle;			//�薼
	CString m_strMetadataAuthor;		//�쐬��
	CString m_strMetadataCopyright;		//���쌠
	CString m_strMetadataRating;		//�K��
	CString m_strMetadataDescription;	//����

	//CPU�ݒ�
	BOOL   m_bProcessPriority;			//�v���Z�X�D��x���L�����ǂ���
	int    m_nProcessPriority;			//�G���R�[�h�v���Z�X�̗D��x(0:�A�C�h���`5:���A���^�C��)
	BOOL   m_bProcessLimitEnable;		//�v���Z�X�̐������L�����ǂ���
	int    m_bProcessAffinityPriority;	//�v���Z�X�̊��S�������D�搧�����ǂ���(TRUE�Ȃ犮�S����,FALSE�Ȃ�D�搧��)
	DWORD  m_dwProcessAffinityMask;		//�v���Z�X�̃A�t�B�j�e�B�}�X�N(���S�����̏ꍇ)
	int    m_nProcessIdealProcessor;	//�v���Z�X�̗D��v���Z�b�T(�D�搧���̏ꍇ)

	//////////////////////////////////////////////////
	//�v���t�@�C���̕ϐ��ꗗ�����܂�
};
