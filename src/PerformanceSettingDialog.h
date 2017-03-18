#pragma once


// CPerformanceSettingDialog �_�C�A���O

class CPerformanceSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CPerformanceSettingDialog)

public:
	CPerformanceSettingDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPerformanceSettingDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PERFORMANCE_SETTING_DIALOG };

private:
	CToolTipCtrl m_ToolTip;					//�_�C�A���O�ɕ\������c�[���`�b�v

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCpuRadio1(){OnCPURadioButtonClick(1);}
	afx_msg void OnBnClickedCpuRadio2(){OnCPURadioButtonClick(2);}
	afx_msg void OnBnClickedCpuRadio3(){OnCPURadioButtonClick(3);}

	void OnCPURadioButtonClick(int nIndex);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedProcessDisableRadio(){ProcessEnableRadio(FALSE);}
	afx_msg void OnBnClickedProcessEnableRadio(){ProcessEnableRadio(TRUE);}
	void ProcessEnableRadio(BOOL bEnable);
};
