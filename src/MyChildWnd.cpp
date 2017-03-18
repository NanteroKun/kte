// MyChildWnd.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "MyChildWnd.h"


// CMyChildWnd

IMPLEMENT_DYNAMIC(CMyChildWnd, CWnd)

CMyChildWnd::CMyChildWnd()
{
}

CMyChildWnd::~CMyChildWnd()
{
}

BEGIN_MESSAGE_MAP(CMyChildWnd, CWnd)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

// CMyChildWnd ���b�Z�[�W �n���h��

void CMyChildWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	CWnd* parent = this->GetParent();
	assert(parent);
	parent->SendMessage(WM_LBUTTONDOWN,nFlags,MAKELPARAM(point.x,point.y));
	CWnd::OnLButtonDown(nFlags, point);
}
void CMyChildWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	CWnd* parent = this->GetParent();
	assert(parent);
	parent->SendMessage(WM_RBUTTONDOWN,nFlags,MAKELPARAM(point.x,point.y));
	CWnd::OnRButtonDown(nFlags, point);
}

BOOL CMyChildWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �����ɓ���ȃR�[�h��ǉ����邩�A�������͊�{�N���X���Ăяo���Ă��������B

	return CWnd::PreCreateWindow(cs);
}

void CMyChildWnd::OnSetFocus(CWnd* pOldWnd)
{
	CWnd::OnSetFocus(pOldWnd);
	//TRACE0("CMyChildWnd::OnSetFocus\n");
	// TODO: �����Ƀ��b�Z�[�W �n���h�� �R�[�h��ǉ����܂��B
}


BOOL CMyChildWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����邩�A����̏������Ăяo���܂��B
	//return CWnd::OnEraseBkgnd(pDC);
	return TRUE;
}


void CMyChildWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �����Ƀ��b�Z�[�W �n���h���[ �R�[�h��ǉ����܂��B
	// �`�惁�b�Z�[�W�� CWnd::OnPaint() ���Ăяo���Ȃ��ł��������B
}
