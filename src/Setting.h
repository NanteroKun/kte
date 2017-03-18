#pragma once

#include "IniFile.h"

#define SETTING_INI_FILE _T("Setting.ini")

//////////////////////////////////////////////////
//�ݒ�f�[�^�̓ǂݏ������s���N���X
//�v���O�����N�����Ƀt�@�C���ǂݍ��݂��A�v���O�����I�����Ƀt�@�C���ۑ����s���̂�
//������̓v���t�@�C���ƈ����1�������݂��Ȃ�(Setting.ini)
//////////////////////////////////////////////////

class CSetting
{
public:
	CSetting(void);				//�R���X�g���N�^
	virtual ~CSetting(void);	//�f�X�g���N�^

	void DefaultSetting();						//�ݒ荀�ڂɏ����l�ɐݒ�
	BOOL LoadSetting();							//�ݒ�f�[�^�̓ǂݍ���
	BOOL SaveSetting();							//�ݒ�f�[�^�̏�������
	void DefaultProfilePath();					//�v���t�@�C���̃p�X���K��̃p�X�ɂ���
	BOOL SetProfilePath(LPCTSTR lpszFileName);	//�v���t�@�C���̃p�X��ݒ肷��

	//////////////////////////////////////////////////
	//�l�X�Ȑݒ荀�ڂ͂����ɏ���
	//////////////////////////////////////////////////

	//�S�̐ݒ�
	BOOL m_bEnableIniFile;			//INI�t�@�C���̑��݂̗L��
	BOOL m_bSaveIniFile;			//INI�t�@�C�����I�����ɃZ�[�u���邩�ۂ�
	CString m_strProgramName;		//�v���O������   ("KoToEncoder")
	int m_nVersion;					//�o�[�W������� (20100125)

	//�v���t�@�C���̏��
	CString m_strProfileShortPath;	//���[�h���ɓǂݍ��ރv���t�@�C����(�V���[�g�p�X)
	CString m_strProfileFullPath;	//���[�h���ɓǂݍ��ރv���t�@�C����(�t���p�X)

	//��ʐ݌v
	CRect m_rcMainWindow;			//���C���E�B�h�E�̍��W
	int m_nMainSplitterWidth[2];	//���C���̍��E�������
	int m_nMainSplitterHeight[2];	//���C���̏㉺�������
	int m_nMainSplitterPreview[2];	//���C���̃v���r���[�����̕������

	BOOL m_bShowToolBar;			//�c�[���o�[��\�����邩�ۂ�
	BOOL m_bShowStatusBar;			//�X�e�[�^�X�o�[��\�����邩�ۂ�
	BOOL m_bShowPropertyWindow;		//�v���p�e�B�E�B���h�E��\�����邩�ۂ�
	BOOL m_bShowMonitorWindow;		//���j�^�[�E�B���h�E��\�����邩�ۂ�
	BOOL m_bShowPreviewWindow;		//�v���r���[�E�B���h�E��\�����邩�ۂ�
	BOOL m_bShowVolumeWindow;		//�{�����[���E�B���h�E��\�����邩�ۂ�

	int m_nPropertyTabIndex;		//�v���p�e�B�^�u�̕\���C���f�b�N�X
	int m_nMonitorTabIndex;			//���j�^�[�^�u�̕\���C���f�b�N�X
	BOOL m_bShowDebugWindow;		//�f�o�b�O�E�B���h�E��\�����邩�ۂ�

	//���ݒ�
	BOOL m_bMiniTaskBar;			//�ŏ������Ƀ^�X�N�o�[�Ɋi�[���邩�ۂ�
	BOOL m_bCreateMutex;			//�����N�������ۂ��邩�ǂ���
	BOOL m_bShowTopMost;			//��ɍőO�ʂɕ\�����邩�ǂ���
	CString m_strVideoPlayerPath;	//�O���v���C���[�̃p�X
	CString m_strGetGlobalIPURL;	//�O���[�o��IP�A�h���X���擾����URL
	BOOL m_bGetGlobalIPCheck;		//�O���[�o��IP�A�h���X���擾����ۂɊm�F������
	CString m_strGlobalIPAddress;	//�O���[�o��IP�A�h���X
	CString m_strGlobalHostName;	//�O���[�o���z�X�g��
	BOOL m_bEnumAllPlugin;			//�v���O�C���̑S��

	//�v���r���[�E�B���h�E�̐ݒ�
	double m_dPreviewChildTimes;	//�q�v���r���[�E�B���h�E�̔{��
	BOOL m_bFixByWindow;			//�E�B���h�E�T�C�Y�Ɋg��k���{�������킹��
	BOOL m_bFixByClient;			//�g��k���{���ɃE�B���h�E�T�C�Y�����킹��
	BOOL m_bPreviewShow;			//�v���r���[��\�����邩�ǂ���
	BOOL m_bPreviewForceOff;		//�v���r���[�������I�ɐ؂�ݒ�
	int  m_nPreviewVMR;				//�v���r���[�̃r�f�I�~�L�V���O�����_���̑I��(0,1,7,9,10�̑I��)
	BOOL m_bPreviewVMROverlay;		//�v���r���[�̃r�f�I�~�L�V���O�����_���̃I�[�o�[���C�m�ۂ̗L��

	//�{�����[���E�B���h�E�̐ݒ�
	BOOL m_bEnableAudioPreview;		//���ʃ��[�^�[���L�����ǂ���

	//���j�^�[�E�B���h�E(���v�^�u)�̐ݒ�
	int m_nMonitorUpdateTimer;		//���v�^�C�}�[�̊Ԋu(�~���b)

	//���j�^�[�E�B���h�E(�u���[�h�L���X�g�^�u)�̐ݒ�
	vector<int> m_nMonitorPullColumnWidthList;

	//���j�^�[�E�B���h�E(�C�x���g���O)�̐ݒ�
	BOOL m_bEventLogAutoSave;			//���O�̎����ۑ�
	CString m_strEventLogAutoSavePath;	//���O�̕ۑ��p�X

	//////////////////////////////////////////////////
	//�ݒ�̕ϐ��ꗗ�����܂�
};
