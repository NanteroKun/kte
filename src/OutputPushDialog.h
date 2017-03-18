#pragma once

#include "OutputBaseDialog.h"

// COutputPushDialog �_�C�A���O

class COutputPushDialog : public COutputBaseDialog
{
	DECLARE_DYNAMIC(COutputPushDialog)

public:
	COutputPushDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~COutputPushDialog();
// �_�C�A���O �f�[�^
	enum { IDD = IDD_OUTPUTPUSHDIALOG };

	//�o�͖��̎擾(�T�[�o�[���ƌ��J�|�C���g)
	BOOL GetOutputName(CString &strFileName);
	//�o�͖��̐ݒ�(�T�[�o�[���ƌ��J�|�C���g)
	BOOL SetOutputName(CString strFileName);
	//�^�u���̎擾
	BOOL GetTabName(CString &strTabName);
	//�^�u���̐ݒ�
	BOOL SetTabName(CString strTabName);
	//�L���`�F�b�N�̎擾
	BOOL GetEnableCheck();
	//�L���`�F�b�N�̐ݒ�
	BOOL SetEnableCheck(BOOL bCheck);

	//�����폜�̎擾
	BOOL GetAutoDeleteCheck();
	//�����폜�̐ݒ�
	BOOL SetAutoDeleteCheck(BOOL bCheck);

	//�K�p�{�^���̕\���E��\��
	BOOL ShowApplyButton(BOOL bShow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK(){}
	virtual void OnCancel(){}

public:
	afx_msg void OnBnClickedOutputPushEnableCheck();
	afx_msg void OnBnClickedOutputPushAutodeleteCheck();

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

	//�T�[�o�[���̕�����ϊ�
	BOOL ConvertServerName(CString strOldName,CString &strNewName);

	CToolTipCtrl m_ToolTip;	//�_�C�A���O�ɕ\������c�[���`�b�v
	int m_nRegisterID;				//�V���N�o�^ID(�l�b�g���[�N�V���N)
	CString m_strRegisterServerName;//�V���N�o�^�����T�[�o�[��
	int m_bRegisterAutoDelete;		//�V���N�o�^���������폜�ݒ�

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOutputApplyButton();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOutputPlayerButton();
};
