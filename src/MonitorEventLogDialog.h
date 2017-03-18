#pragma once


// CMonitorEventLogDialog �_�C�A���O

class CMonitorEventLogDialog : public CDialog
{
	DECLARE_DYNAMIC(CMonitorEventLogDialog)

public:
	CMonitorEventLogDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CMonitorEventLogDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MONITOREVENTLOGDIALOG };

	virtual void OnOK() {}
	virtual void OnCancel() {}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	//�G���R�[�h�̊J�n�ƏI��
	BOOL Encode(BOOL bStart);

	//�C�x���g���O�̒ǉ�
	BOOL AddEventLog(const CString &strEventLog);

	//���O�C�x���g�̃o�b�t�@
	CString m_strLogEvent;

	CStdioFile m_fEventLogFile;		//�����ۑ��̃t�@�C���N���X
	BOOL m_bEventLogFile;			//�����ۑ��̃t�@�C�����J���Ă��邩�ǂ���

	BOOL OpenEventLogFile(CString &strFileName);
	void CloseEventLogFile();

	afx_msg void OnBnClickedEventLogClearButton();
	afx_msg void OnBnClickedEventLogSaveButton();
	afx_msg void OnBnClickedAutoSaveCheck();
	afx_msg void OnBnClickedAutoSaveButton();
};
