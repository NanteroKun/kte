#pragma once

#include "OutputBaseDialog.h"

// COutputPullDialog �_�C�A���O

class COutputPullDialog : public COutputBaseDialog
{
	DECLARE_DYNAMIC(COutputPullDialog)

public:
	COutputPullDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~COutputPullDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OUTPUTPULLDIALOG };

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

	//�|�[�g�ԍ��̎擾
	BOOL GetPort(int &nPort);
	//�|�[�g�ԍ��̐ݒ�
	BOOL SetPort(int nPort);
	//�ő�ڑ��l���̎擾
	BOOL GetMax(int &nMax);
	//�ő�ڑ��l���̐ݒ�
	BOOL SetMax(int nMax);

	//�K�p�{�^���̕\���E��\��
	BOOL ShowApplyButton(BOOL bShow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK(){}
	virtual void OnCancel(){}

public:
	afx_msg void OnBnClickedOutputPullEnableCheck();
	virtual BOOL OnInitDialog();
	afx_msg void OnEnChangeOutputPullMaxEdit();
	//afx_msg void OnBnClickedOutputPullApplyButton();

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

private:
	BOOL m_bFirstFlag;	//�������t���O
	CToolTipCtrl m_ToolTip;	//�_�C�A���O�ɕ\������c�[���`�b�v

	int m_nRegisterID;				//�V���N�o�^ID(�l�b�g���[�N�V���N)
	int m_nRegisterPort;			//�V���N�o�^�����|�[�g�ԍ�
	int m_nRegisterMax;			//�V���N�o�^�����ő�ڑ��l��

	//�O���[�o��IP�A�h���X���擾
	BOOL GetGlobalIPAddress(CString strURL,CString &strIPAddress,CString &strHostName);
	//�N���b�v�{�[�h�ɕ������ݒ肷��
	BOOL SetClipText(LPCTSTR text);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedOutputApplyButton();
	afx_msg void OnDestroy();
	afx_msg void OnEnChangeOutputPullPortEdit();
	afx_msg void OnBnClickedOutputPlayerButton();
	afx_msg void OnBnClickedOutputIpaddrButton();

	afx_msg void OnIpPopup(){OnIPHostPopup(TRUE);}
	afx_msg void OnHostPopup(){OnIPHostPopup(FALSE);}

	void OnIPHostPopup(BOOL bIP);
};
