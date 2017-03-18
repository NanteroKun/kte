#pragma once

#include "SizeDialog.h"

// CInputPropertyDialog �_�C�A���O

class CInputPropertyDialog : public CSizeDialog
{
	DECLARE_DYNAMIC(CInputPropertyDialog)

public:
	CInputPropertyDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CInputPropertyDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_INPUTPROPERTYDIALOG };
	
	static const int SHARE_VIDEO_AUDIO = -2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	//�����o�ϐ���������
	BOOL m_bFirstFlag;			//����_�C�A���O�������t���O
	CToolTipCtrl m_ToolTip;		//�_�C�A���O�ɕ\������c�[���`�b�v
	//�����o�ϐ������܂�

	//�r�f�I�f�o�C�X�̏������Ɏ��s
	BOOL VideoDeviceFailed(int nLevel);
	//�r�f�I�f�o�C�X�̐��\��\��
	BOOL VideoDeviceCapacity(int nIndex);
	//���݂̃r�f�I���͒l(���E�����E�t���[�����[�g)�𓾂�
	void GetVideoCurrentInput(int &nWidth,int &nHeight,double &dFPS);
	//�r�f�I���͒l��ݒ�(�l��\�����邾���œK�p�͂��Ȃ�)
	BOOL SetVideoCurrentInput(int nWidth, int nHeight, double dFPS);

	//�I�[�f�B�I�f�o�C�X�̐��\��\��
	BOOL AudioDeviceCapacity(int nIndex);
	//���݂̃I�[�f�B�I���͒l�𓾂�
	BOOL GetAudioCurrentInput(DWORD &nSamplesPerSec,WORD &wBitsPerSample,WORD &nChannels);
	//�I�[�f�B�I���͒l��ݒ�(�l��\�����邾���œK�p�͂��Ȃ�)
	BOOL SetAudioCurrentInput(DWORD nSamplesPerSec,WORD wBitsPerSample,WORD nChannels);

	//�r�f�I�L���v�`���f�o�C�X�̃I�[�f�B�I�s���̗L���̏���
	void ProcVideoAudioInputPin();

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeVideoSettingCombo();
	afx_msg void OnCbnSelchangeVideoResolutionCombo();
	BOOL OnSelectVideoSettingIndex(AM_MEDIA_TYPE* pamt);


	afx_msg void OnCbnSelchangeAudioSettingCombo();
	afx_msg void OnBnClickedVideoSettingButton();
	afx_msg void OnBnClickedAudioSettingButton();
	afx_msg void OnDestroy();

	BOOL ChangeProfileNotify();	//�v���t�@�C���̕ύX�ʒm
	BOOL LoadProfileSetting();	//�v���t�@�C������̃��[�h
	BOOL SaveProfileSetting();	//�v���t�@�C���ւ̃Z�[�u

	BOOL Encode(BOOL bStart);	//�G���R�[�h�̊J�n�ƒ�~

	BOOL DeviceChange();	//�f�o�C�X�ʒm�̃R�[���o�b�N���b�Z�[�W

protected:
	virtual void OnOK(){}
	virtual void OnCancel(){}
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedVideoResolutionButton();

	//afx_msg void OnSize(UINT nType, int cx, int cy){CSizeDialog::OnSize(nType, cx, cy);}
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){CSizeDialog::OnVScroll(nSBCode, nPos, pScrollBar);}
	afx_msg void OnBnClickedAudioResolutionButton();
	afx_msg void OnCbnSelchangeAudioResolutionCombo();
	afx_msg void OnBnClickedInputConfigButton();

	afx_msg void OnWdmDevice(){OnWDMVideoSettingMenu(WDM_FILTER_NONE);}
	afx_msg void OnWdmCrossbar1(){OnWDMVideoSettingMenu(WDM_FILTER_CROSS_BAR1);}
	afx_msg void OnWdmCrossbar2(){OnWDMVideoSettingMenu(WDM_FILTER_CROSS_BAR2);}
	afx_msg void OnWdmTvaudio(){OnWDMVideoSettingMenu(WDM_FILTER_TV_AUDIO);}
	afx_msg void OnWdmTvtuner(){OnWDMVideoSettingMenu(WDM_FILTER_TV_TUNER);}

	void OnWDMVideoSettingMenu(ENUM_WDM_FILTER e);

};
