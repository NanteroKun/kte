#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#include "InputPropertyDialog.h"
#include "OutputPropertyDialog.h"
#include "EncodePropertyDialog.h"
#include "MetadataPropertyDialog.h"

// CPropertyFormView �t�H�[�� �r���[

class CPropertyFormView : public CFormView
{
	DECLARE_DYNCREATE(CPropertyFormView)

protected:

public:
	CPropertyFormView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	virtual ~CPropertyFormView();
	enum { IDD = IDD_PROPERTYFORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	//�^�u�ƃ^�u�_�C�A���O
	CTabCtrl m_xcPropertyTab;			//�^�u�R���g���[��
	CInputPropertyDialog m_InputTab;	//���̓^�u
	COutputPropertyDialog m_OutputTab;	//�o�̓^�u
	CEncodePropertyDialog m_EncodeTab;	//���k�^�u
	CMetadataPropertyDialog m_MetadataTab;//���^�f�[�^�^�u

	CInputPropertyDialog* GetInputTab() {return &m_InputTab;}
	COutputPropertyDialog* GetOutputTab() {return &m_OutputTab;}
	CEncodePropertyDialog* GetEncodeTab() {return &m_EncodeTab;}
	CMetadataPropertyDialog* GetMetadataTab() {return &m_MetadataTab;}

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	LRESULT OnInitDialog(WPARAM wParam,LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTcnSelchangePropertyTab(NMHDR *pNMHDR, LRESULT *pResult);

	//�v���t�@�C���̕ύX�ʒm
	BOOL ChangeProfileNotify();
	//�v���t�@�C���̃Z�[�u
	BOOL SaveProfileSetting();
	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);
};


