#pragma once

#ifndef __AFXWIN_H__
	#error "PCH �ɑ΂��Ă��̃t�@�C�����C���N���[�h����O�� 'stdafx.h' ���C���N���[�h���Ă�������"
#endif

#include "resource.h"       // ���C�� �V���{��
#include "DirectShowAccess.h"
#include "Setting.h"
#include "Profile.h"

//////////////////////////////////////////////////
//�A�v���P�[�V�����N���X
//////////////////////////////////////////////////
class CKTEApp : public CWinApp
{
public:
	//�R���X�g���N�^
	CKTEApp();
	//�f�X�g���N�^
	virtual ~CKTEApp();

	//�A�v���P�[�V�����G���g���|�C���g
	virtual BOOL InitInstance();
	//�A�v���P�[�V�����I���|�C���g
	virtual int ExitInstance();

	//�ݒ�t�@�C���̎擾
	inline CSetting* GetSetting() const {assert(m_pSetting); return m_pSetting;}
	//�v���t�@�C���̎擾
	inline CProfile* GetProfile() const {assert(m_pProfile); return m_pProfile;}
	//DirectShow�A�N�Z�X�N���X�̏���
	inline CDirectShowAccess* GetDSA() const {assert(m_pDSA);return m_pDSA;}

	//���j���[����̌Ăяo��
	afx_msg void OnSettingDialog();				//���ݒ�_�C�A���O��\��
	afx_msg void OnPluginDialog();				//�v���O�C���_�C�A���O��\��
	afx_msg void OnPerformanceSettingDialog();	//�v���Z�b�T�ݒ�_�C�A���O��\��
	afx_msg void OnSettingDefault();			//�ݒ��������Ԃɖ߂�
	afx_msg void OnAboutDialog();				//�o�[�W�������_�C�A���O��\��
	afx_msg void OnProfileNew();				//�v���t�@�C����V�K�쐬
	afx_msg void OnProfileOpen();				//�v���t�@�C�����J��
	afx_msg void OnProfileSave();				//�v���t�@�C����ۑ�
	afx_msg void OnProfileSaveas();				//�v���t�@�C����ʖ��ۑ�

	//�N���X�����b�Z�[�W�}�b�v��錾���邱�Ƃ��`����
	DECLARE_MESSAGE_MAP()

private:
	//////////////////////////////////////////////////
	//�����o�ϐ�(��������)
	CSetting* m_pSetting;			//�ݒ�(INI)�ւ̃A�N�Z�X
	CProfile* m_pProfile;			//�v���t�@�C���ւ̃A�N�Z�X
	CDirectShowAccess* m_pDSA;		//DirectShow�ւ̃A�N�Z�X�N���X

	HANDLE m_hMutexApp;				//�����N���}���p�̃~���[�e�b�N�X
	BOOL   m_bEnablePriority;		//CPU�D��x�̕ύX�������ǂ���
	//////////////////////////////////////////////////
	//�����o�ϐ�(�����܂�)

	//�T�u���[�`��
	BOOL OSVersionCheck();				//OS�̃o�[�W�����`�F�b�N���s��
	BOOL ArgCheck1();					//�����̃`�F�b�N���s��
	BOOL ArgCheck2();					//�����̃`�F�b�N���s��
	BOOL SetMutex(BOOL bEnable);		//�����N���̐ݒ���s��
	BOOL SetTopMost(BOOL bEnable);		//��Ɏ�O�ɕ\������
	BOOL SetAffinityMask(BOOL bInit);	//�v���Z�X�̃A�t�B�j�e�B�}�X�N��ݒ肷�� (CProfile�ۑ�)
public:
	BOOL SetPriorityClass(BOOL bEnable,DWORD dwPriorityClass);	//�v���Z�X�̗D��x��ݒ肷�� (CProfile�ۑ�)

};

//////////////////////////////////////////////////
//theApp�͗B��̃O���[�o���ϐ�(�O���[�o���N���X)
//////////////////////////////////////////////////
extern CKTEApp theApp;

//////////////////////////////////////////////////
//[EOF]