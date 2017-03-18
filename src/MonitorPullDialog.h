#pragma once


// CMonitorPullDialog �_�C�A���O

class CMonitorPullDialog : public CDialog
{
	DECLARE_DYNAMIC(CMonitorPullDialog)

public:
	CMonitorPullDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CMonitorPullDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MONITORPULLDIALOG };

	virtual void OnOK() {}
	virtual void OnCancel() {}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()

public:

	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	LRESULT OnNetworkClientConnect(WPARAM wParam,LPARAM lParam);
	LRESULT OnNetworkClientDisconnect(WPARAM wParam,LPARAM lParam);


	BOOL Encode(BOOL bStart);

	afx_msg void OnPaint();

	int GetAllConnectCount()const {return m_nAllConnectCount;}

	int m_nAllConnectCount;			//���ڑ���
	vector<int> m_ConnectCountList;	//�e�|�[�g�̐ڑ���

	//���X�g�r���[�̏��
	struct PullConnectStruct
	{
		int nSrcPort;			//�����瑤�̃|�[�g�ԍ�
		CString strIPAddress;	//IP�A�h���X(������)
		int nIPAddress[4];		//IP�A�h���X(���l)
		DWORD dwIPAddress;		//IP�A�h���X
		CString strHostName;	//�z�X�g��
		int nDestPort;			//�����瑤�̃|�[�g�ԍ�
		CTime StartTime;		//�ڑ��J�n����
		CTimeSpan ConnectTime;	//�ڑ����̎���
		//BOOL bNewInsert;		//�V�����}�������f�[�^�̃t���O
	};
	vector<PullConnectStruct> m_ConnectList;	//���X�g�r���[�̏��
	//�ǉ�������
	struct AddConnectStruct
	{
		DWORD dwIPAddress;
		DWORD dwPort;
		int nIndex;
	};
	vector<AddConnectStruct> m_AddConnectList;	//�ǉ��������

	//���ڑ����\���̍X�V
	void UpdateConnectionEdit();

	//�^�C�}�[�̃C�x���g�n���h��
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	//���X�g�R���g���[���̎擾
	CListCtrl* GetListCtrl() {
		CListCtrl* list = (CListCtrl*)GetDlgItem(IDC_PULL_LIST);
		assert(list);
		return list;
	}
};
