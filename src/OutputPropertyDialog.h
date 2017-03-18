#pragma once

#include "SizeDialog.h"
#include "OutputBaseDialog.h"
#include "OutputPullDialog.h"
#include "OutputPushDialog.h"
#include "OutputFileDialog.h"

//���O�̃��b�Z�[�W�ԍ����`�i100�ɂ͓��ɈӖ��͂Ȃ��j
#define NOTIFY_ENABLE_CHECK (WM_APP + 99)

// COutputPropertyDialog �_�C�A���O

class COutputPropertyDialog : public CSizeDialog
{
	DECLARE_DYNAMIC(COutputPropertyDialog)

public:
	COutputPropertyDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~COutputPropertyDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_OUTPUTPROPERTYDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK() {}
	virtual void OnCancel() {}

public:
	//�v���t�@�C���̕ύX�ʒm
	BOOL ChangeProfileNotify();
	//�v���t�@�C������̃��[�h
	BOOL LoadProfileSetting();
	//�v���t�@�C���ւ̃Z�[�u
	BOOL SaveProfileSetting();

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

	afx_msg void OnBnClickedOutputInsertButton();
	afx_msg void OnBnClickedOutputDeleteButton();

	enum OUTPUT_TYPE{
		OUTPUT_TYPE_PULL = 0,
		OUTPUT_TYPE_PUSH,
		OUTPUT_TYPE_FILE
	};

	afx_msg void OnInsertPull(){ this->OnInsertOutput(OUTPUT_TYPE_PULL,NULL,FALSE);}
	afx_msg void OnInsertPush(){ this->OnInsertOutput(OUTPUT_TYPE_PUSH,NULL,FALSE);}
	afx_msg void OnInsertFile(){ this->OnInsertOutput(OUTPUT_TYPE_FILE,NULL,FALSE);}
	HTREEITEM OnInsertOutput(OUTPUT_TYPE nOutputType,LPCTSTR lpszItemName,BOOL bCheck);

	virtual BOOL OnInitDialog();
private:
	//////////////////////////////////////////////////
	//�����o�ϐ�
	//�c���[�r���[�̃��[�g�A�C�e���̃n���h��
	HTREEITEM m_hPullTree;	//�u���[�h�o���h
	HTREEITEM m_hPushTree;	//�p�u���b�V���|�C���g
	HTREEITEM m_hFileTree;	//�t�@�C���A�[�J�C�u

	//����ʃ_�C�A���O�z��
	vector<COutputPullDialog*> m_pPullDialogList;
	vector<COutputPushDialog*> m_pPushDialogList;
	vector<COutputFileDialog*> m_pFileDialogList;

	COutputBaseDialog* m_pNowSelectDialog;		//���ݕ\�����̉���ʃ_�C�A���O�̃|�C���^
	BOOL m_bFirstFlag;							//����_�C�A���O�������t���O
	CToolTipCtrl m_ToolTip;						//�_�C�A���O�ɕ\������c�[���`�b�v
	//�����o�ϐ������܂�
	//////////////////////////////////////////////////

public:
	afx_msg void OnTvnSelchangedOutputTree(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();
	afx_msg void OnTvnEndlabeleditOutputTree(NMHDR *pNMHDR, LRESULT *pResult);

	BOOL OnTreeCheckEvent(HTREEITEM hTreeItem,BOOL bCheck,BOOL bNotifyCall);
protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg LRESULT OnChildEnableCheck(WPARAM wParam,LPARAM lParam);

	//�_�C�A���O�z������
	void DeleteAllDialog();

public:
	/*
	afx_msg void OnTvnBegindragOutputTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	*/
	afx_msg void OnTvnGetInfoTipOutputTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingOutputTree(NMHDR *pNMHDR, LRESULT *pResult);

	//afx_msg void OnSize(UINT nType, int cx, int cy){CSizeDialog::OnSize(nType, cx, cy);}
	//afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar){CSizeDialog::OnVScroll(nSBCode, nPos, pScrollBar);}
};
