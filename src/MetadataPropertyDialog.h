#pragma once

#include "SizeDialog.h"

// CMetadataPropertyDialog �_�C�A���O

class CMetadataPropertyDialog : public CSizeDialog
{
	DECLARE_DYNAMIC(CMetadataPropertyDialog)

public:
	CMetadataPropertyDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CMetadataPropertyDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_METADATAPROPERTYDIALOG };

	virtual void OnOK(){}
	virtual void OnCancel(){}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();

	//�v���t�@�C���̕ύX�ʒm
	BOOL ChangeProfileNotify();
	//�v���t�@�C������̃��[�h
	BOOL LoadProfileSetting();
	//�v���t�@�C���ւ̃Z�[�u
	BOOL SaveProfileSetting();

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

	//afx_msg void OnSize(UINT nType, int cx, int cy){CSizeDialog::OnSize(nType, cx, cy);}
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){CSizeDialog::OnVScroll(nSBCode, nPos, pScrollBar);}
};
