#pragma once

//////////////////////////////////////////////////
//�_�C�A���O�N���X
//////////////////////////////////////////////////
class CAboutDialog : public CDialog
{
	DECLARE_DYNAMIC(CAboutDialog)

public:
	//�R���X�g���N�^
	CAboutDialog(CWnd* pParent = NULL);
	//�f�X�g���N�^
	virtual ~CAboutDialog();

	//�_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
};



