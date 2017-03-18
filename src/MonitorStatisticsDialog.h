#pragma once


// CMonitorStatisticsDialog �_�C�A���O

class CMonitorStatisticsDialog : public CDialog
{
	DECLARE_DYNAMIC(CMonitorStatisticsDialog)

public:
	CMonitorStatisticsDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CMonitorStatisticsDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MONITORSTATISTICSDIALOG };

	virtual void OnOK() {}
	virtual void OnCancel() {}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//�G���R�[�h�̊J�n�ƒ�~
	BOOL Encode(BOOL bStart);
	//���v���̕\��
	void DrawStatics();

private:
	CTime m_tStartTime;			//�J�n����
	CTime m_tEndTime;			//�I������
	CTimeSpan m_tElapseTime;	//�o�ߎ���

	//�o�C�g�𕶎���ɕϊ�����T�u���[�`��
	CString ByteToString(QWORD qwByte);
public:
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};
