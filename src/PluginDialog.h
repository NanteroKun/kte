#pragma once


// CPluginDialog �_�C�A���O

class CPluginDialog : public CDialog
{
	DECLARE_DYNAMIC(CPluginDialog)

public:
	CPluginDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CPluginDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_PLUGINDIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();

private:
	//////////////////////////////////////////////////
	//�����o�ϐ���������
	CToolTipCtrl m_ToolTip;					//�_�C�A���O�ɕ\������c�[���`�b�v

	//�}���\�ȃv���O�C���̃��X�g
	vector<FilterInfoStruct> m_AllDShowFilterList;			//DirectShow�t�B���^�J�e�S���̃v���O�C��
	vector<FilterInfoStruct> m_VideoDShowFilterList;		//DirectShow�r�f�I�v���O�C��
	vector<FilterInfoStruct> m_AudioDShowFilterList;		//DirectShow�I�[�f�B�I�v���O�C��
	vector<FilterInfoStruct> m_VideoDMOFliterList;			//DMO�r�f�I�v���O�C��
	vector<FilterInfoStruct> m_AudioDMOFliterList;			//DMO�I�[�f�B�I�v���O�C��

	BOOL m_bChange;	//�ύX�_�����������ǂ���

	//////////////////////////////////////////////////
	//�����o�ϐ������܂�

	//�����o�֐�
	BOOL InsertPluginButton(BOOL bVideo);	//�ǉ��{�^��
	BOOL DeletePluginButton(BOOL bVideo);	//�폜�{�^��
	BOOL PropertyButton(BOOL bVideo);		//�v���p�e�B�̕\���{�^��
	BOOL UpDownButton(BOOL bVideo,BOOL bUp);//��ցE���փ{�^��

	BOOL EnumPluginList(BOOL bAllPlugin);	
	BOOL RedrawPlugin(BOOL bVideo);
	int GetSelectedListCtrlIndex(BOOL bVideo);
	BOOL StopFilter();
	BOOL StartFilter(BOOL bVideo,BOOL bRun,BOOL bInsert,BOOL bRestart);

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedVideoPluginInsertButton(){InsertPluginButton(TRUE);}
	afx_msg void OnBnClickedAudioPluginInsertButton(){InsertPluginButton(FALSE);}
	afx_msg void OnBnClickedVideoPluginDeleteButton(){DeletePluginButton(TRUE);}
	afx_msg void OnBnClickedAudioPluginDeleteButton(){DeletePluginButton(FALSE);}
	afx_msg void OnBnClickedVideoPluginPropertyButton(){PropertyButton(TRUE);}
	afx_msg void OnBnClickedAudioPluginPropertyButton(){PropertyButton(FALSE);}
	afx_msg void OnBnClickedVideoPluginUpButton(){UpDownButton(TRUE,TRUE);}
	afx_msg void OnBnClickedAudioPluginUpButton(){UpDownButton(FALSE,TRUE);}
	afx_msg void OnBnClickedVideoPluginDownButton(){UpDownButton(TRUE,FALSE);}
	afx_msg void OnBnClickedAudioPluginDownButton(){UpDownButton(FALSE,FALSE);}
	afx_msg void OnBnClickedEnumPluginCheck();
};
