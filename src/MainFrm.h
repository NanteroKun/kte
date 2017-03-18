
// MainFrm.h : CMainFrame �N���X�̃C���^�[�t�F�C�X
//

#pragma once


//�e��o�[
#include "MyStatusBar.h"
#include "MyToolBar.h"
#include "MySplitterWnd.h"

//�q�E�B���h�E
#include "PropertyFormView.h"
#include "PreviewWnd.h"
#include "VolumeWnd.h"
#include "MonitorFormView.h"

#include "MyTaskTray.h"

//
#include "DirectShowAccess.h"


//////////////////////////////////////////////////
//���C���t���[���E�B���h�E
//	�e�E�B���h�E�̐����ƊǗ�
//	�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
class CMainFrame : public CFrameWnd
{
public:
	CMainFrame();			//�R���X�g���N�^
	virtual ~CMainFrame();	//�f�X�g���N�^

protected: 
	//CObject�N���X����p�����Ă���ꍇ�͎��s���^���(RTTI)�Ƃ��āA�N���X�̌^���`���Ȃ���΂Ȃ�Ȃ�
	DECLARE_DYNAMIC(CMainFrame)

public:
	//�E�B���h�E�������̍\����cs��ҏW�ł���
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// ����
public:
	//�e�E�B���h�E�N���X���擾����
	inline CMyToolBar* GetToolBar(){return &m_wndToolBar;}
	inline CMyStatusBar* GetStatusBar(){return &m_wndStatusBar;}
	inline CPreviewWnd* GetPreviewWnd(){return &m_wndPreviewWnd;}
	inline CVolumeWnd*  GetVolumeWnd() {return &m_wndVolumeWnd;}
	inline CPropertyFormView* GetPropertyWnd() {
		CPropertyFormView* wnd = (CPropertyFormView*)m_wndLeftRightSplitter.GetColumnWnd(0);
		assert(wnd);
		return wnd;
	}
	inline CMonitorFormView* GetMonitorWnd() {
		CMonitorFormView* wnd = (CMonitorFormView*)m_wndTopButtomSplitter.GetRowWnd(1);
		assert(wnd);
		return wnd;
	}

	//�G���R�[�h�J�n�ƒ�~�̃{�^����������Ă��邩�ǂ���
	BOOL IsStartStopThread()const {return m_bStartStopThread;}
	//��~�̂��߂̃A�C�h�����O���[�v
	BOOL StopThreadMessageLoop();

	//�^�C�g���o�[�̕ύX
	void ChangeTitleBar();
	//�v���t�@�C���̕ύX
	BOOL ChangeProfileNotify();
	//�v���t�@�C���̕ۑ�
	BOOL SaveProfileSetting();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:
	//////////////////////////////////////////////////
	//�����o�ϐ�
	CMyToolBar m_wndToolBar;				//�c�[���o�[
	CMyStatusBar m_wndStatusBar;			//�X�e�[�^�X�o�[
	CPreviewWnd m_wndPreviewWnd;			//�v���r���[�E�B���h�E(�E��)
	CVolumeWnd	m_wndVolumeWnd;				//�{�����[���E�B���h�E(�E��)

	//�e�핪���E�B���h�E�̏��
	CMySplitterWnd m_wndTopButtomSplitter;	//�㉺�����E�B���h�E
	CMySplitterWnd m_wndLeftRightSplitter;	//���E�����E�B���h�E
	CMySplitterWnd m_wndPreviewSplitter;	//�v���r���[�̕����E�B���h�E
	BOOL m_bSplitterInit;			//�����E�B���h�E�̏�����������Ă��邩�ǂ����H
	int  m_nSplitterColumnWidth[2];	//���E�y�C���̊e�T�C�Y(�ݒ�l)
	int  m_nSplitterRowHeight[2];	//�㉺�y�C���̊e�T�C�Y(�ݒ�l)
	int  m_nSplitterPreviewSize[2];	//�v���r���[�y�C���̊e�T�C�Y(�ݒ�l) �����̒l�͕��Ȃ̂������Ȃ̂��͕�����Ȃ�
	
	int m_nSplitterLineWidth;		//���E�y�C���̐��̕�   (�Œ�l)
	int m_nSplitterLineHegiht;		//�㉺�y�C���̐��̍��� (�Œ�l)

	//�X���b�h����
	static UINT ThreadProcCalc(LPVOID pParam);	//�X���b�h�֐�
	CWinThread* m_StartStopThread;				//�J�n�ƏI���X���b�h
	BOOL        m_bStartStopThread;				//�X���b�h�����s�����ǂ���

	CMyTaskTray m_TaskTray;						//�^�X�N�g���C
	//////////////////////////////////////////////////
	//�����o�ϐ�(�����܂�)

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:

	//WM_CREATE�̃C�x���g�n���h��
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//WM_SETFOCUS�̃C�x���g�n���h��
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	//�N���X�����b�Z�[�W�}�b�v��錾���邱�Ƃ��`����
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);

	//�����E�B���h�E�̐������擾(�T�u���[�`��)
	void CalcSplitterLine(int nClientWidth,int nClientHeight);

public:
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg LRESULT OnGraphNotify(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnTrayIcon(WPARAM wParam,LPARAM lParam);
	afx_msg void OnShowWindowFromTaskTray();

	afx_msg void OnUpdateEncode(CCmdUI *pCmdUI);
	afx_msg void OnUpdateProfile(CCmdUI *pCmdUI);

	void Showbar(const int nMenuID);
	afx_msg void OnShowToolbar(){Showbar(ID_SHOW_TOOLBAR);}
	afx_msg void OnShowStatusbar(){Showbar(ID_SHOW_STATUSBAR);}

	BOOL IsShowView(int nMenuID) {return (GetMenu()->GetMenuState(nMenuID,MF_BYCOMMAND) & MF_CHECKED);}

	void OnShowView(const int nMenuID,const BOOL bShow);
	afx_msg void OnShowProperty(){OnShowView(ID_SHOW_PROPERTY,!IsShowView(ID_SHOW_PROPERTY));}
	afx_msg void OnShowMonitor() {OnShowView(ID_SHOW_MONITOR, !IsShowView(ID_SHOW_MONITOR));}
	afx_msg void OnShowPreview() {OnShowView(ID_SHOW_PREVIEW, !IsShowView(ID_SHOW_PREVIEW));}
	afx_msg void OnShowVolume()  {OnShowView(ID_SHOW_VOLUME,  !IsShowView(ID_SHOW_VOLUME));}

	//�G���R�[�h�̊J�n�ƒ�~
	afx_msg void OnStartEncode();	//�G���R�[�h�̊J�n(�{�^��)
	afx_msg void OnStopEncode();	//�G���R�[�h�̒�~(�{�^��)
	afx_msg LRESULT OnNotifyStopEncode(WPARAM wParam,LPARAM lParam);
	BOOL StartEncode();			//�G���R�[�h�̊J�n(�֐�)
	BOOL PreStopEncode();		//�G���R�[�h��~�̃v������(WM���b�Z�[�W)
	BOOL PostStopEncode();		//�G���R�[�h��~�̃|�X�g����(WM���b�Z�[�W)
};

//////////////////////////////////////////////////
//[EOF]
