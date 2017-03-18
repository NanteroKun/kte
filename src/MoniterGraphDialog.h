#pragma once


// CMoniterGraphDialog �_�C�A���O

class CMoniterGraphDialog : public CDialog
{
	DECLARE_DYNAMIC(CMoniterGraphDialog)

	static const int TIMER_ID = 103;

public:
	CMoniterGraphDialog(CWnd* pParent = NULL);   // �W���R���X�g���N�^�[
	virtual ~CMoniterGraphDialog();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_MONITERGRAPHDIALOG };

	virtual void OnOK() {}
	virtual void OnCancel() {}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	BOOL Encode(BOOL bStart);
	afx_msg void OnTimer(UINT_PTR nIDEvent);


private:
	void CreateMediaTypeString(AM_MEDIA_TYPE* pAmt,CString &strMessage);
	void CreateFourCCString(WORD biBitCount,DWORD biCompression,DWORD data1,CString &strFourcc);
};
