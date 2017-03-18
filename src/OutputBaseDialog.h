#pragma once


// COutputBaseDialog �_�C�A���O

class COutputBaseDialog : public CDialog
{
	//DECLARE_DYNAMIC(COutputBaseDialog)

public:
	//COutputBaseDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^

	explicit COutputBaseDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL)
		: CDialog(nIDTemplate,pParentWnd)
	{
	}
	virtual ~COutputBaseDialog(){
	}

	//�t�@�C�����̎擾
	virtual BOOL GetOutputName(CString &strFileName) = 0;
	//�t�@�C�����̐ݒ�
	virtual BOOL SetOutputName(CString strFileName) = 0;
	//�^�u���̎擾
	virtual BOOL GetTabName(CString &strTabName) = 0;
	//�^�u���̐ݒ�
	virtual BOOL SetTabName(CString strTabName) = 0;
	//�L���`�F�b�N�̎擾
	virtual BOOL GetEnableCheck() = 0;
	//�L���`�F�b�N�̐ݒ�
	virtual BOOL SetEnableCheck(BOOL bCheck) = 0;

// �_�C�A���O �f�[�^
	//enum { IDD = IDD_OUTPUTBASEDIALOG };

	//�G���R�[�h�̊J�n�ƒ�~
	virtual BOOL Encode(BOOL bStart) = 0;

protected:
	//virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//DECLARE_MESSAGE_MAP()
};
