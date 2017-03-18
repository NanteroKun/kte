#pragma once


// CInputPropertyConfigDialog �_�C�A���O

class CInputPropertyConfigDialog : public CDialog
{
	DECLARE_DYNAMIC(CInputPropertyConfigDialog)

public:
	CInputPropertyConfigDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CInputPropertyConfigDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_INPUTPROPERTYCONFIGDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();

	//�ύX�����������ǂ����H
	BOOL m_bChange;
	BOOL m_bAudioBufferChange;

	CToolTipCtrl m_ToolTip;					//�_�C�A���O�ɕ\������c�[���`�b�v

public:
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedClockDisableRadio(){OnBnClickedClockEnableRadio(FALSE);}
	afx_msg void OnBnClickedClockEnableRadio(){OnBnClickedClockEnableRadio(TRUE);}
	void OnBnClickedClockEnableRadio(BOOL bEnable);

	afx_msg void OnBnClickedAudioBufferAutoRadio(){OnBnClickedAudioBufferRadio(FALSE);}
	afx_msg void OnBnClickedAudioBufferManualRadio(){OnBnClickedAudioBufferRadio(TRUE);}
	void OnBnClickedAudioBufferRadio(BOOL bEnable);

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedInputSettingResetButton();
};
