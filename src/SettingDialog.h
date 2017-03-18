#pragma once
#include "afxwin.h"


// CSettingDialog �_�C�A���O

class CSettingDialog : public CDialog
{
	DECLARE_DYNAMIC(CSettingDialog)

public:
	CSettingDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CSettingDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_SETTINGDIALOG };

private:
	CToolTipCtrl m_ToolTip;								//�_�C�A���O�ɕ\������c�[���`�b�v

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedApply();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedVideoPlayerButton();

	afx_msg void OnBnClickedKeyframeAutoRadio(){CheckKeyFrameButton(FALSE);}
	afx_msg void OnBnClickedKeyframeManualRadio(){CheckKeyFrameButton(TRUE);}
	void CheckKeyFrameButton(BOOL bEnable);

	afx_msg void OnBnClickedComplexityAutoRadio()   {CheckComplexityButton(FALSE);}
	afx_msg void OnBnClickedComplexityManualRadio() {CheckComplexityButton(TRUE);}
	void CheckComplexityButton(BOOL bEnable);

	afx_msg void OnNMCustomdrawCpuPrioritySlider(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedKeyframeManualRadio2();
	afx_msg void OnBnClickedComplexityAutoRadio2();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	afx_msg void OnBnClickedForcePreviewOffCheck();
};
