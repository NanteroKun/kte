#pragma once

//#include <afxsplitterwndex.h>
//#include "ExtSplitter.h"

//////////////////////////////////////////////////
//�����E�B���h�E
//////////////////////////////////////////////////
class CMySplitterWnd : public CSplitterWnd
{
	DECLARE_DYNAMIC(CMySplitterWnd)

public:
	//�R���X�g���N�^
	CMySplitterWnd();
	//�f�X�g���N�^
	virtual ~CMySplitterWnd();
protected:
	DECLARE_MESSAGE_MAP()
	//�E�B���h�E�f�U�C���ύX
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	//���N���b�N���ɌĂ΂��C�x���g�n���h��
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//���N���b�N�������ɌĂ΂��C�x���g�n���h��
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	//�}�E�X�ړ����ɌĂ΂��C�x���g�n���h��
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	//���\��
	void ShowColumn();
	//����\��
	void HideColumn(int colHide);
	//�񂪕\������Ă��邩�ǂ������ׂ�
	BOOL IsShowColumn() { return (BOOL) m_nHidedCol == -1;}
	//��\������擾
	int  IsHideColumnIndex() {return m_nHidedCol;}


	//��̃E�B���h�E�n���h�����擾����
	//���̊֐��͗񂪔�\���ł����Ă��J�E���g�����
	CWnd* GetColumnWnd(int col);

	//�s��\��
	void ShowRow();
	//�s���\��
	void HideRow(int rowHide);
	//�s���\������Ă��邩�ǂ������ׂ�
	BOOL IsShowRow() { return (BOOL) m_nHidedRow == -1;}
	//��\����ԍ����擾
	int  IsHideRowIndex() {return m_nHidedRow;}

	//�s�̃E�B���h�E�n���h�����擾����
	//���̊֐��͗񂪔�\���ł����Ă��J�E���g�����
	CWnd* GetRowWnd(int row);

	int m_nHidedCol;	//�B����Ă����
	int m_nHidedRow;	//�B����Ă���s
	int m_nHidedID;		//������Ă���s�E���ID
};


