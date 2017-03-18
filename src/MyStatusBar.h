#pragma once
#include "afxext.h"

//////////////////////////////////////////////////
//����X�e�[�^�X�o�[
//////////////////////////////////////////////////
class CMyStatusBar : public CStatusBar
{
public:
	//�R���X�g���N�^
	CMyStatusBar(void);
	//�f�X�g���N�^
	virtual ~CMyStatusBar(void);

	//�X�e�[�^�X�o�[���쐬
	virtual BOOL Create(CFrameWnd* pFrameWnd);

	//�X�e�[�^�X�o�[�ɕ��������������
	BOOL SetString(LPCTSTR lpszText);
	//�X�e�[�^�X�o�[�ɓ]���ʂ���������
	BOOL SetTransform(LPCTSTR lpszText);

	//�X�e�[�^�X�o�[�̍Đ�/��~��Ԃ��X�V����
	BOOL SetPlayStatus(BOOL bPlay);

	//�X�e�[�^�X�o�[�Ɋg��k���{������������
	BOOL SetTimes(double dTimes);

	DECLARE_MESSAGE_MAP()

	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL DestroyWindow();

public:
	//�G���R�[�h�̊J�n�ƏI��(�v������)
	BOOL PreEncode(BOOL bStart);
	//�G���R�[�h�̊J�n�ƏI��
	BOOL Encode(BOOL bStart);

private:
	//�����o�ϐ�
	PDH_HQUERY m_hQuery;		//�����x�p�t�H�[�}���X�J�E���^�̃n���h��
	HCOUNTER   m_hCounter;		//�����x�p�t�H�[�}���X�J�E���^
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};
