#pragma once
#include "afxext.h"

//����c�[���o�[�N���X
class CMyToolBar : public CToolBar
{
protected:
	//DECLARE_DYNAMIC(CToolBar)
	//�N���X�����b�Z�[�W�}�b�v��錾���邱�Ƃ��`����
	//DECLARE_MESSAGE_MAP()

public:
	//�R���X�g���N�^
	CMyToolBar(void);
	//�f�X�g���N�^
	~CMyToolBar(void);

	//�c�[���o�[�̍쐬
	BOOL Create(CFrameWnd* pFrameWnd);
	//�{�^������������Ԃɂ���
	BOOL PressButton(int nID,BOOL bPress);
	//�G���R�[�h�̊J�n�ƏI��
	BOOL Encode(BOOL bStart);

	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};
