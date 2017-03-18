#pragma once

//�g���C�A�C�R���Ǘ��pID���`�i1�ɂ͓��ɈӖ��͂Ȃ��j
#define TRAY_ID        1

//���O�̃��b�Z�[�W�ԍ����`�i100�ɂ͓��ɈӖ��͂Ȃ��j
#define NOTIFY_TRAYICON (WM_APP + 100)

//�^�X�N�g���C�̊Ǘ��N���X
class CMyTaskTray
{
public:
	CMyTaskTray(void);
	virtual ~CMyTaskTray(void);

	BOOL Init(CWnd* pWnd);
	BOOL Exit();
	BOOL NotifyIcon();
	BOOL SetIcon(int nIconID);

	LRESULT OnTrayIcon(WPARAM wParam,LPARAM lParam);

private:
	NOTIFYICONDATA  m_nid;		//�A�C�R���o�^�̍\����
	BOOL m_bid;					//�A�C�R���o�^������Ă��邩�ǂ���
	CWnd* m_pWnd;				//�o�^�����E�B���h�E�n���h��
};

