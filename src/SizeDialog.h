#pragma once


// CSizeDialog �_�C�A���O

class CSizeDialog : public CDialog
{
	DECLARE_DYNAMIC(CSizeDialog)

public:
	CSizeDialog(int nID,CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CSizeDialog();

// �_�C�A���O �f�[�^
	//enum { IDD = IDD_SIZEDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

	CSize m_ScrollSize;		//���݂̃_�C�A���O�̕��ƍ���
	CPoint m_ScrollPos;		//���݂�XY���W
	CRect m_ScorllRect;		//���݂̃_�C�A���O�̕�

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
