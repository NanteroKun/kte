#pragma once

#include "SizeDialog.h"

// CEncodePropertyDialog �_�C�A���O

class CEncodePropertyDialog : public CSizeDialog
{
	DECLARE_DYNAMIC(CEncodePropertyDialog)

public:
	CEncodePropertyDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CEncodePropertyDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ENCODEPROPERTYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	BOOL m_bFirstFlag;		//����_�C�A���O�������t���O
	CToolTipCtrl m_ToolTip;	//�_�C�A���O�ɕ\������c�[���`�b�v

	//�I�[�f�B�I�G���R�[�_�[�̐ݒ�
	DWORD m_dwBitrate;				//�r�b�g���[�g
	DWORD m_nSamplesPerSec;			//���g��
	WORD m_wBitsPerSample;			//����\
	WORD m_nChannels;				//�`�����l��
	BOOL m_bAVSynchronization;		//A/V����
	
	void AudioEncoderSettingChange();

public:
	virtual BOOL OnInitDialog();

	afx_msg void OnCbnSelchangeVideoEncoderCombo();
	afx_msg void OnCbnSelchangeAudioEncoderCombo();
	afx_msg void OnDestroy();
	afx_msg void OnCbnSelchangeAudioEncoderBitrateCombo();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedSizeAutoRadio();
	afx_msg void OnEnChangeVideoQualityEdit();

	//�v���t�@�C���̕ύX�ʒm
	BOOL ChangeProfileNotify();
	//�v���t�@�C������̃��[�h
	BOOL LoadProfileSetting();
	//�v���t�@�C���ւ̃Z�[�u
	BOOL SaveProfileSetting();

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

protected:
	virtual void OnOK();
	virtual void OnCancel();
public:
	afx_msg void OnEnChangeVideoFramerateEdit();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

private:
public:
	//afx_msg void OnSize(UINT nType, int cx, int cy){CSizeDialog::OnSize(nType, cx, cy);}
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){CSizeDialog::OnVScroll(nSBCode, nPos, pScrollBar);}

	afx_msg void OnCbnSelchangeAudioEncoderSamplespersecCombo(){AudioEncoderSettingChange();}
	afx_msg void OnCbnSelchangeAudioEncoderBitspersampleCombo(){AudioEncoderSettingChange();}
	afx_msg void OnCbnSelchangeAudioEncoderChannelsCombo(){AudioEncoderSettingChange();}
	afx_msg void OnCbnSelchangeAudioEncoderAvsynchronization(){AudioEncoderSettingChange();}
};
