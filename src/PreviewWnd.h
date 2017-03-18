#pragma once

#include "MyChildWnd.h"

//////////////////////////////////////////////////
//�v���r���[�E�B���h�E
//////////////////////////////////////////////////
class CPreviewWnd : public CWnd
{
	DECLARE_DYNAMIC(CPreviewWnd)

public:
	//�R���X�g���N�^
	CPreviewWnd();
	//�f�X�g���N�^
	virtual ~CPreviewWnd();

protected:
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

private:

	//////////////////////////////////////////////////
	//�����o�ϐ�
	BOOL m_bFirstFlag;			//�������t���O
	CMyChildWnd m_wndChild;		//�v���r���[��ʖ{�̂̎q�E�B���h�E(���̃E�B���h�E�ɃL���v�`����ʂ��\�������)
	CSize m_ClientSize;			//�N���C�A���g�E�B���h�E�̃T�C�Y
	CSize m_ChildSize;			//�q�E�B���h�E�̃T�C�Y
	CPoint m_ChildPos;			//�q�E�B���h�E�̏ꏊ
	BOOL m_bChildShow;			//�q�E�B���h�E��\�����邩�ۂ�

	//�X�N���[���o�[�֘A
	BOOL m_bShowHScroll;		//�����X�N���[���o�[�̕\��
	BOOL m_bShowVScroll;		//�����X�N���[���o�[�̕\��
	BOOL m_bRecalcWindowCall;	//�X�N���[���o�[�̕ҏW���t���O
	int m_nScrollBarWidth;		//�����X�N���[���o�[�̕�(�Œ�l)
	int m_nScrollBarHeight;		//�����X�N���[���o�[�̍���(�Œ�l)

	COLORREF m_rgbBack;			//�w�i�F
	COLORREF m_rgbLine;			//�g���̐F

	//��ʂ̊g��k���{��
	double m_dTimes;			//�g��{��(�P�ʂ̓p�[�Z���g)
	int m_nTimesID;				//���݂̔{����MenuID
	BOOL m_bFixByClient;		//�v���C���[�̑傫�����v���r���[�ɂ��킹��
	BOOL m_bFixByWindow;		//�E�B���h�E�̃T�C�Y�ɍ��킹��

	//////////////////////////////////////////////////
	//�����o�ϐ�(�����܂�)

	/*
	HHOOK m_hMouseHook;			//�}�E�X�t�b�N�p�̃n���h��
	HHOOK GetHook() {return m_hMouseHook;};
	CWnd* GetChildWnd() {return &m_wndChild;}
	static LRESULT CALLBACK MyMouseHookProc(int nCode, WPARAM wp, LPARAM lp);
	*/

	//�v���C���[�̃T�C�Y���v���r���[�ɍ��킹��
	void FixWindowSize();
	//�v���r���[�̔{�������݂̃T�C�Y�ɍ��킹��
	void FixPreviewTimes(int nClientWidth ,int nClientHeight);

	//�X�e�[�^�X�o�[�Ɋg��{������������
	void SetStatusTimes(double dTimes);

public:

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
private:
	void MoveChildWindow();
public:
	//�v���r���[�E�B���h�E�����蓖�Ă�
	void SetPreviewWnd();
	//�v���r���[�T�C�Y��ݒ肷��
	void SetPreviewSize(BOOL bSizeUpdateFlag);

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);

	//�v���t�@�C���̕ύX�ʒm
	BOOL ChangeProfileNotify();

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	//�E�B���h�E�z�u�̌v�Z
	BOOL RecalcWindowArrangement(BOOL bOnSize);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnViewPopup(UINT nID);
	afx_msg void OnPopupFixwindowsize();
	afx_msg void OnPopupFixpreviewsize();
	afx_msg void OnPopupShowpreview();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};


