#pragma once

#include "OutputBaseDialog.h"

// COutputFileDialog �_�C�A���O

class COutputFileDialog : public COutputBaseDialog
{
	DECLARE_DYNAMIC(COutputFileDialog)

public:
	COutputFileDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~COutputFileDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OUTPUTFILEDIALOG };

	//�o�͖��̎擾(�o�̓t�@�C��)
	BOOL GetOutputName(CString &strFileName);
	//�o�͖��̐ݒ�(�o�̓t�@�C��)
	BOOL SetOutputName(CString strFileName);
	//�^�u���̎擾
	BOOL GetTabName(CString &strTabName);
	//�^�u���̐ݒ�
	BOOL SetTabName(CString strTabName);
	//�L���`�F�b�N�̎擾
	BOOL GetEnableCheck();
	//�L���`�F�b�N�̐ݒ�
	BOOL SetEnableCheck(BOOL bCheck);
	//�K�p�{�^���̕\���Ɣ�\��
	BOOL ShowApplyButton(BOOL bShow);

	//�o�͐ݒ薼�̎擾�ƕύX
	BOOL GetOutputFileName(CString &strFileName);
	BOOL GetOutputFolderName(CString &strFolderName);
	BOOL SetOutputFileName(CString strFileName);
	BOOL SetOutputFolderName(CString strFolderName);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK(){}
	virtual void OnCancel(){}

public:
	afx_msg void OnBnClickedOutputFileButton();
	afx_msg void OnBnClickedOutputFileEnableCheck();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedOutputApplyButton();

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	int m_nRegisterID;				//�V���N�o�^ID(�t�@�C���V���N)
	CString m_strRegisterFileName;	//�V���N�o�^�����t�@�C����
	CTime m_RegisterTime;			//�V���N�o�^��������
	CToolTipCtrl m_ToolTip;			//�_�C�A���O�ɕ\������c�[���`�b�v

	static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData);

	//�o�̓t�@�C�����̐ݒ�
	BOOL MakeFilePath(CString strFolderPath,CString strFilePath,CTime &Time,CString &strMakePath);
	afx_msg void OnBnClickedOutputOpenButton();
};
