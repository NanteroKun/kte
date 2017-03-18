#pragma once

#include "MonitorStatisticsDialog.h"
#include "MonitorPullDialog.h"
#include "MonitorEventLogDialog.h"
#include "MoniterGraphDialog.h"

// CMonitorFormView �t�H�[�� �r���[

class CMonitorFormView : public CFormView
{
	DECLARE_DYNCREATE(CMonitorFormView)

protected:
	CMonitorFormView();           // ���I�����Ŏg�p����� protected �R���X�g���N�^
	virtual ~CMonitorFormView();

public:
	enum { IDD = IDD_MONITORFORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	LRESULT OnInitDialog(WPARAM wParam,LPARAM lParam);

public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();

	BOOL Encode(BOOL bStart);
	afx_msg void OnTcnSelchangeMonitorTab(NMHDR *pNMHDR, LRESULT *pResult);


	CMonitorStatisticsDialog* GetStatisticsTab(){return &m_StatisticsTab;}
	CMonitorPullDialog* GetPullTab(){return &m_PullTab;}
	CMonitorEventLogDialog* GetEventLogTab(){return &m_EventLogTab;}

protected:
	virtual void OnDraw(CDC* /*pDC*/);

	//�����o�ϐ�
	CMonitorStatisticsDialog m_StatisticsTab;	//���v�^�u
	CMonitorPullDialog m_PullTab;				//�u���[�h�o���h�^�u
	CMonitorEventLogDialog m_EventLogTab;		//�C�x���g���O�^�u
	CMoniterGraphDialog m_GraphTab;				//�O���t�^�u
	//�����܂�
};


