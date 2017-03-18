// PluginDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "PluginDialog.h"
#include "MainFrm.h"

// CPluginDialog �_�C�A���O

IMPLEMENT_DYNAMIC(CPluginDialog, CDialog)

BEGIN_MESSAGE_MAP(CPluginDialog, CDialog)
	ON_BN_CLICKED(IDC_VIDEO_PLUGIN_INSERT_BUTTON, &CPluginDialog::OnBnClickedVideoPluginInsertButton)
	ON_BN_CLICKED(IDC_AUDIO_PLUGIN_INSERT_BUTTON, &CPluginDialog::OnBnClickedAudioPluginInsertButton)
	ON_BN_CLICKED(IDC_VIDEO_PLUGIN_DELETE_BUTTON, &CPluginDialog::OnBnClickedVideoPluginDeleteButton)
	ON_BN_CLICKED(IDC_AUDIO_PLUGIN_DELETE_BUTTON, &CPluginDialog::OnBnClickedAudioPluginDeleteButton)
	ON_BN_CLICKED(IDC_VIDEO_PLUGIN_PROPERTY_BUTTON, &CPluginDialog::OnBnClickedVideoPluginPropertyButton)
	ON_BN_CLICKED(IDC_AUDIO_PLUGIN_PROPERTY_BUTTON, &CPluginDialog::OnBnClickedAudioPluginPropertyButton)
	ON_BN_CLICKED(IDC_VIDEO_PLUGIN_UP_BUTTON, &CPluginDialog::OnBnClickedVideoPluginUpButton)
	ON_BN_CLICKED(IDC_VIDEO_PLUGIN_DOWN_BUTTON, &CPluginDialog::OnBnClickedVideoPluginDownButton)
	ON_BN_CLICKED(IDC_AUDIO_PLUGIN_UP_BUTTON, &CPluginDialog::OnBnClickedAudioPluginUpButton)
	ON_BN_CLICKED(IDC_AUDIO_PLUGIN_DOWN_BUTTON, &CPluginDialog::OnBnClickedAudioPluginDownButton)
	ON_BN_CLICKED(IDC_ENUM_PLUGIN_CHECK, &CPluginDialog::OnBnClickedEnumPluginCheck)
END_MESSAGE_MAP()

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CPluginDialog::CPluginDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CPluginDialog::IDD, pParent)
{
	m_AllDShowFilterList.clear();
	m_VideoDShowFilterList.clear();
	m_AudioDShowFilterList.clear();
	m_VideoDMOFliterList.clear();
	m_AudioDMOFliterList.clear();
}
//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CPluginDialog::~CPluginDialog()
{
}
//////////////////////////////////////////////////
//DDX/DDV
//////////////////////////////////////////////////
void CPluginDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
////////////////////////////////////////
//�_�C�A���O�̏�����
////////////////////////////////////////
BOOL CPluginDialog::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	//m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_MINI_TASKBAR_CHECK),IDC_MINI_TASKBAR_CHECK);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();

	//�}���\�ȃv���O�C���ꗗ�̗�
	EnumPluginList(pSetting->m_bEnumAllPlugin);
	CButton* check = (CButton*)GetDlgItem(IDC_ENUM_PLUGIN_CHECK);
	if (pSetting->m_bEnumAllPlugin)
		check->SetCheck(BST_CHECKED);

	//�v���O�C���\�̏�����
	LVCOLUMN lvc;
	TCHAR caption[][32] = {
		_T("���O"),
		_T("�L��")
	};
	const int clmNum = sizeof(caption) / sizeof(caption[0]);
	lvc.mask = LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	CListCtrl* list = (CListCtrl*)this->GetDlgItem(IDC_VIDEO_PLUGIN_LIST); assert(list);
	list->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);	//��P�ʂɑI��
	list->SetExtendedStyle((list->GetExtendedStyle()|LVS_EX_GRIDLINES));						//�O���b�h����\��
	for (int i = 0; i < clmNum; i++) {
		lvc.iSubItem = i;
		lvc.pszText = caption[i];
		if (i == 0) lvc.cx = 190;
		else        lvc.cx = 40;
		list->InsertColumn(i,&lvc);
	}
	list = (CListCtrl*)this->GetDlgItem(IDC_AUDIO_PLUGIN_LIST); assert(list);
	list->SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,LVS_EX_FULLROWSELECT,LVS_EX_FULLROWSELECT);	//��P�ʂɑI��
	list->SetExtendedStyle((list->GetExtendedStyle()|LVS_EX_GRIDLINES));						//�O���b�h����\��
	for (int i = 0; i < clmNum; i++) {
		lvc.iSubItem = i;
		lvc.pszText = caption[i];
		if (i == 0) lvc.cx = 190;
		else        lvc.cx = 40;
		list->InsertColumn(i,&lvc);
	}
	//�v���O�C���\�̕`��
	RedrawPlugin(TRUE);
	RedrawPlugin(FALSE);

	//�G���R�[�h���̏ꍇ�͊���̃{�^���������ɂȂ�
	if (pDSA->IsEncode())
	{
		CWnd* wnd;
		wnd = GetDlgItem(IDC_VIDEO_PLUGIN_INSERT_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_AUDIO_PLUGIN_INSERT_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_PLUGIN_DELETE_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_AUDIO_PLUGIN_DELETE_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_PLUGIN_UP_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_AUDIO_PLUGIN_UP_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_VIDEO_PLUGIN_DOWN_BUTTON);
		wnd->EnableWindow(FALSE);
		wnd = GetDlgItem(IDC_AUDIO_PLUGIN_DOWN_BUTTON);
		wnd->EnableWindow(FALSE);
	}

	m_bChange = FALSE;	//���̂Ƃ���ύX�_�Ȃ�
	return TRUE;
}

////////////////////////////////////////
//OK�{�^��
////////////////////////////////////////
void CPluginDialog::OnOK()
{
	CSetting* pSetting = ((CKTEApp*)AfxGetApp())->GetSetting();
	CButton* check = (CButton*)GetDlgItem(IDC_ENUM_PLUGIN_CHECK);
	if (check->GetCheck() & BST_CHECKED) {
		pSetting->m_bEnumAllPlugin = TRUE;
	} else {
		pSetting->m_bEnumAllPlugin = FALSE;
	}
	CDialog::OnOK();
}
////////////////////////////////////////
//�L�����Z���{�^��
//�Ƃ肠�����L�����Z���s�ɂ��Ă�����
////////////////////////////////////////
void CPluginDialog::OnCancel()
{
	CDialog::OnCancel();
}
////////////////////////////////////////
//�c�[���`�b�v�̕\��
////////////////////////////////////////
BOOL CPluginDialog::PreTranslateMessage(MSG* pMsg)
{
	switch (pMsg->message)
	{
		case WM_LBUTTONDOWN: 
		case WM_LBUTTONUP: 
		case WM_MOUSEMOVE: 
			m_ToolTip.RelayEvent(pMsg);
			break;
	}
	return CDialog::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////
//�v���O�C���̒ǉ��{�^��
//////////////////////////////////////////////////
BOOL CPluginDialog::InsertPluginButton(BOOL bVideo)
{
	CComboBox* combo;
	if (bVideo)
		combo = (CComboBox*)this->GetDlgItem(IDC_VIDEO_PLUGIN_LIST_COMBO);
	else
		combo = (CComboBox*)this->GetDlgItem(IDC_AUDIO_PLUGIN_LIST_COMBO);
	assert(combo);

	CString strVideo;
	if (bVideo) {
		strVideo = _T("�r�f�I");
	} else {
		strVideo = _T("�I�[�f�B�I");
	}

	int nComboIndex = combo->GetCurSel();
	if (nComboIndex == CB_ERR)
	{
		CString strMessage;
		strMessage.Format(_T("�}������%s�v���O�C�����I������Ă��܂���"),strVideo);
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	FilterInfoStruct* pFIS = NULL;
	pFIS = (FilterInfoStruct*)combo->GetItemDataPtr(nComboIndex);
	if (pFIS == NULL)
		return FALSE;

	//�v���O�C���̒ǉ������݂�
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	BOOL bRet;
	GUID guidCategory = GUID_NULL;
	if (bVideo) {
		for (int i=0;i<(int)m_VideoDMOFliterList.size();i++)
		{
			if (pFIS == &(m_VideoDMOFliterList[i])) {
				guidCategory = DMOCATEGORY_VIDEO_EFFECT;
				break;
			}
		}
	} else {
		for (int i=0;i<(int)m_AudioDMOFliterList.size();i++)
		{
			if (pFIS == &(m_AudioDMOFliterList[i])) {
				guidCategory = DMOCATEGORY_AUDIO_EFFECT;
				break;
			}
		}
	}
	//�t�B���^�̈ꎞ��~
	BOOL bRun = StopFilter();
	//�v���O�C���̒ǉ�
	bRet = pDSA->AddPlugin(bVideo,pFIS->guid,guidCategory,pFIS->strName.c_str());
	if (!bRet)
	{
		CString strMessage;
		strMessage.Format(_T("%s�v���O�C����%s��}������̂Ɏ��s���܂���"),strVideo,pFIS->strName.c_str());
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//�t�B���^�̍ĊJ (�����ȃt�B���^�̏ꍇ�A���̃^�C�~���O�Ŏ��s����)
	if (!StartFilter(bVideo,bRun,TRUE,FALSE))
	{
		CString strMessage;
		strMessage.Format(_T("%s�v���O�C���ɐڑ��s�\�ȃv���O�C��%s���}������܂���"),strVideo,pFIS->strName.c_str());
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//ListCtrl�̍Ō���Ƀv���O�C����ǉ�
	RedrawPlugin(bVideo);
	/*
	CListCtrl* pList = NULL;
	if (bVideo){
		pList = (CListCtrl*)GetDlgItem(IDC_VIDEO_PLUGIN_LIST); assert(pList);
	} else {
		pList = (CListCtrl*)GetDlgItem(IDC_AUDIO_PLUGIN_LIST); assert(pList);
	}
	if (!pList) return FALSE;
	pList->InsertItem(pList->GetItemCount(),pFIS->strName.c_str());
	*/

	///���튮��
	m_bChange = TRUE;	//�ύX����
	return TRUE;
}

//////////////////////////////////////////////////
//�v���O�C���̍폜�{�^��
//////////////////////////////////////////////////
BOOL CPluginDialog::DeletePluginButton(BOOL bVideo)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	//���ݑI�𒆂�Item���擾
	int nIndex = -1;
	nIndex = GetSelectedListCtrlIndex(bVideo);
	if (nIndex == -1)
		return FALSE;

	//�t�B���^�̈ꎞ��~
	BOOL bRun = StopFilter();
	//�v���O�C���̍폜
	BOOL bRet = pDSA->RemovePlugin(bVideo,nIndex);
	if (!bRet)
		return FALSE;
	//�t�B���^�̍ĊJ
	if (!StartFilter(bVideo,bRun,FALSE,FALSE))
		return FALSE;

	//���X�g����v���O�C�����폜
	RedrawPlugin(bVideo);
	/*
	CListCtrl* pList = NULL;
	if (bVideo){
		pList = (CListCtrl*)GetDlgItem(IDC_VIDEO_PLUGIN_LIST); assert(pList);
	} else {
		pList = (CListCtrl*)GetDlgItem(IDC_AUDIO_PLUGIN_LIST); assert(pList);
	}
	if (!pList)
		return FALSE;
	pList->DeleteItem(nIndex);
	*/

	m_bChange = TRUE;	//�ύX����
	return TRUE;
}
//////////////////////////////////////////////////
//��ցE���փ{�^��
//////////////////////////////////////////////////
BOOL CPluginDialog::UpDownButton(BOOL bVideo,BOOL bUp)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	CListCtrl* pList = NULL;
	if (bVideo){
		pList = (CListCtrl*)GetDlgItem(IDC_VIDEO_PLUGIN_LIST); assert(pList);
	} else {
		pList = (CListCtrl*)GetDlgItem(IDC_AUDIO_PLUGIN_LIST); assert(pList);
	}
	if (!pList)
		return FALSE;

	int nIndex = -1;
	int nDestIndex = -1;

	nIndex = GetSelectedListCtrlIndex(bVideo);
	if (nIndex == -1)
		return FALSE;
	if (nIndex == 0 && bUp == TRUE)								//�őO��ŏ�ւ��悤�Ƃ���
		return FALSE;
	if ((nIndex == pList->GetItemCount()-1) && bUp == FALSE)	//�Ō���ŉ��ւ��悤�Ƃ���
		return FALSE;

	if (bUp)
		nDestIndex = nIndex - 1;
	else
		nDestIndex = nIndex + 1;

	//�t�B���^�̈ꎞ��~
	BOOL bRun = StopFilter();

	//�v���O�C���̓���ւ�
	BOOL bRet = pDSA->SwitchPlugin(bVideo,nIndex,nDestIndex);
	if (!bRet)
		return FALSE;

	//�t�B���^�̍ĊJ
	if (!StartFilter(bVideo,bRun,FALSE,FALSE))
		return FALSE;

	//�����̓���ւ�
	pList->SetItemState(nIndex,0, LVIS_SELECTED);
	pList->SetItemState(nDestIndex,LVIS_SELECTED,LVIS_SELECTED);
	RedrawPlugin(bVideo);
	/*
	CString strSrcText;
	CString strDestText;
	strSrcText = pList->GetItemText(nIndex,0);
	strDestText = pList->GetItemText(nDestIndex,0);
	pList->SetItemText(nIndex,0,strDestText);
	pList->SetItemText(nDestIndex,0,strSrcText);
	*/

	//�ύX����
	m_bChange = TRUE;

	return TRUE;
}


//////////////////////////////////////////////////
//�v���p�e�B�̕\���{�^�� (�G���R�[�h���͂��̊֐��̂ݗL���B���͖���)
//////////////////////////////////////////////////
BOOL CPluginDialog::PropertyButton(BOOL bVideo)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	int nIndex = -1;	//�I�𒆂̃A�C�e���C���f�b�N�X
	nIndex = GetSelectedListCtrlIndex(bVideo);
	//�v���p�e�B�̕\��
	if (nIndex != -1)
		return pDSA->ShowPluginDialog(this->m_hWnd,bVideo,nIndex);
	return FALSE;
}

//////////////////////////////////////////////////
//�v���O�C���̑S�񋓃{�^��
//////////////////////////////////////////////////
void CPluginDialog::OnBnClickedEnumPluginCheck()
{
	CButton* check = (CButton*)GetDlgItem(IDC_ENUM_PLUGIN_CHECK);
	if (check->GetCheck() & BST_CHECKED) {
		EnumPluginList(TRUE);
	} else {
		EnumPluginList(FALSE);
	}
}

//////////////////////////////////////////////////
//�v���O�C���̗񋓂��s���T�u���[�`��
//////////////////////////////////////////////////
BOOL CPluginDialog::EnumPluginList(BOOL bAllPlugin)
{
	if (m_AllDShowFilterList.size() != 0)
		m_AllDShowFilterList.clear();
	if (m_VideoDShowFilterList.size() != 0)
		m_VideoDShowFilterList.clear();
	if (m_AudioDShowFilterList.size() != 0)
		m_AudioDShowFilterList.clear();
	if (m_VideoDMOFliterList.size() != 0)
		m_VideoDMOFliterList.clear();
	if (m_AudioDMOFliterList.size() != 0)
		m_AudioDMOFliterList.clear();
	//�r�f�I/�I�[�f�B�I�̃}�b�`�t�B���^�̗�
	CDirectShowEtc::EnumAllFilter(CLSID_LegacyAmFilterCategory,m_AllDShowFilterList);
	CDirectShowEtc::EnumMatchFilter(MEDIATYPE_Video,GUID_NULL,m_VideoDShowFilterList);
	CDirectShowEtc::EnumMatchFilter(MEDIATYPE_Audio,GUID_NULL,m_AudioDShowFilterList);
	CDirectShowEtc::EnumAllDMOFilter(DMOCATEGORY_VIDEO_EFFECT,m_VideoDMOFliterList);
	CDirectShowEtc::EnumAllDMOFilter(DMOCATEGORY_AUDIO_EFFECT,m_AudioDMOFliterList);

	//���X�g���O�̃t�B���^�̗�
	vector<FilterInfoStruct> VideoCompressorFilterList;	//���X�g���珜�������t�B���^
	vector<FilterInfoStruct> AudioCompressorFilterList;	//���X�g���珜�������t�B���^
	CDirectShowEtc::EnumAllFilter(CLSID_VideoCompressorCategory,VideoCompressorFilterList);
	CDirectShowEtc::EnumAllFilter(CLSID_AudioCompressorCategory,AudioCompressorFilterList);

	//�R���{�{�b�N�X���擾
	CComboBox* video = (CComboBox*)GetDlgItem(IDC_VIDEO_PLUGIN_LIST_COMBO); assert(video);
	CComboBox* audio = (CComboBox*)GetDlgItem(IDC_AUDIO_PLUGIN_LIST_COMBO); assert(audio);
	video->ResetContent();
	audio->ResetContent();
	if (!video->IsWindowEnabled()) video->EnableWindow(TRUE);
	if (!audio->IsWindowEnabled()) audio->EnableWindow(TRUE);

	int nInsertIndex;
	CString strInsertString;
	for (int i=0;i<(int)m_VideoDMOFliterList.size();i++)	//�r�f�I(DMO)
	{
		strInsertString.Format(_T("(DMO) %s"),m_VideoDMOFliterList[i].strName.c_str());
		nInsertIndex = video->AddString(strInsertString);
		video->SetItemDataPtr(nInsertIndex,&m_VideoDMOFliterList[i]);	//�|�C���^���d����
	}
	for (int i=0;i<(int)m_AudioDMOFliterList.size();i++)	//�I�[�f�B�I(DMO)
	{
		strInsertString.Format(_T("(DMO) %s"),m_AudioDMOFliterList[i].strName.c_str());
		nInsertIndex = audio->AddString(strInsertString);
		audio->SetItemDataPtr(nInsertIndex,&m_AudioDMOFliterList[i]);	//�|�C���^���d����
	}

	BOOL bFind;
	for (int i=0;i<(int)m_AllDShowFilterList.size();i++) {	//�r�f�I(DirectShow)
		bFind = FALSE;
		for (int j=0;j<(int)VideoCompressorFilterList.size();j++) {
			if (VideoCompressorFilterList[j].guid == m_AllDShowFilterList[i].guid) {
				bFind = TRUE;
				break;
			}
		}
		if (bFind)		//Compressor�͏��O
			continue;
		bFind = FALSE;
		for (int j=0;j<(int)m_VideoDShowFilterList.size();j++) {
			if (m_AllDShowFilterList[i].guid == m_VideoDShowFilterList[j].guid) {
				bFind = TRUE;
				break;
			}
		}
		if (bFind || bAllPlugin) {
			nInsertIndex = video->AddString(m_AllDShowFilterList[i].strName.c_str());
			video->SetItemDataPtr(nInsertIndex,&m_AllDShowFilterList[i]);	//�|�C���^���d����
		}
	}
	for (int i=0;i<(int)m_AllDShowFilterList.size();i++) {	//�I�[�f�B�I(DirectShow)
		bFind = FALSE;
		for (int j=0;j<(int)AudioCompressorFilterList.size();j++) {
			if (AudioCompressorFilterList[j].guid == m_AllDShowFilterList[i].guid) {
				bFind = TRUE;
				break;
			}
		}
		if (bFind)		//Compressor�͏��O
			continue;
		bFind = FALSE;
		for (int j=0;j<(int)m_AudioDShowFilterList.size();j++) {
			if (m_AllDShowFilterList[i].guid == m_AudioDShowFilterList[j].guid) {
				bFind = TRUE;
				break;
			}
		}
		if (bFind || bAllPlugin) {
			nInsertIndex = audio->AddString(m_AllDShowFilterList[i].strName.c_str());
			audio->SetItemDataPtr(nInsertIndex,&m_AllDShowFilterList[i]);	//�|�C���^���d����
		}
	}
	if (m_VideoDShowFilterList.size() == 0 && m_VideoDMOFliterList.size() == 0){
		video->AddString(_T("[�Ȃ�]"));
		video->SetCurSel(0);
		video->EnableWindow(FALSE);
	} else {
		video->SetCurSel(0);
	}
	if (m_AudioDShowFilterList.size() == 0 && m_AudioDMOFliterList.size() == 0){
		audio->AddString(_T("[�Ȃ�]"));
		audio->SetCurSel(0);
		audio->EnableWindow(FALSE);
	} else {
		audio->SetCurSel(0);
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�v���O�C���̃��X�g���X�V
//////////////////////////////////////////////////
BOOL CPluginDialog::RedrawPlugin(BOOL bVideo)
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	int nInsertIndex;
	CListCtrl* list = NULL;
	vector<FilterPluginStruct>* pPluginList = NULL;
	if (bVideo) {
		list = (CListCtrl*)this->GetDlgItem(IDC_VIDEO_PLUGIN_LIST); assert(list);
		pPluginList = &(pDSA->m_VideoPluginList);
	} else {
		list = (CListCtrl*)this->GetDlgItem(IDC_AUDIO_PLUGIN_LIST); assert(list);
		pPluginList = &(pDSA->m_AudioPluginList);
	}

	//���ݑI�𒆂̃A�C�e�����擾
	int nSelectedIndex = GetSelectedListCtrlIndex(bVideo);
	//���݂̕\���폜
	list->DeleteAllItems();
	for (int i=0;i<(int)pPluginList->size();i++)
	{
		nInsertIndex = list->InsertItem(list->GetItemCount(),
			pPluginList->at(i).strFilterName);
		list->SetItemText(nInsertIndex,1,pPluginList->at(i).bEnable?_T("�L��"):_T("����"));
	}

	//�A�C�e���̑I��
	if (list->GetItemCount() != 0)
	{
		if (nSelectedIndex != -1 && nSelectedIndex < list->GetItemCount())
			list->SetItemState(nSelectedIndex,LVIS_SELECTED,LVIS_SELECTED);
		else
			list->SetItemState(0,LVIS_SELECTED,LVIS_SELECTED);
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�I������Ă���ListCtrl�̃C���f�b�N�X�𓾂�T�u���[�`��
//////////////////////////////////////////////////
int CPluginDialog::GetSelectedListCtrlIndex(BOOL bVideo)
{
	CListCtrl* pList = NULL;
	if (bVideo){
		pList = (CListCtrl*)GetDlgItem(IDC_VIDEO_PLUGIN_LIST); assert(pList);
	} else {
		pList = (CListCtrl*)GetDlgItem(IDC_AUDIO_PLUGIN_LIST); assert(pList);
	}
	if (!pList) return -1;
	if (pList->GetSelectedCount() != 1)
		return -1;
	for (int i=0;i<(int)pList->GetItemCount();i++)
	{
		if (pList->GetItemState(i,LVIS_SELECTED) == LVIS_SELECTED)
			return i;
	}
	return -1;
}

//////////////////////////////////////////////////
//�t�B���^��~�̃T�u���[�`��
//////////////////////////////////////////////////
BOOL CPluginDialog::StopFilter()
{
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	BOOL bRun = pDSA->IsRunFilter();
	if (bRun)
		pDSA->StopFilter();
	return bRun;
}
//////////////////////////////////////////////////
//�t�B���^�ĊJ�̃T�u���[�`��
//BOOL bVideo �r�f�I�t�B���^���ǂ���
//BOOL bInsert �O�̏����̓t�B���^�}�������̂��ǂ���
//BOOL bRestart �ċN�����p
//////////////////////////////////////////////////
BOOL CPluginDialog::StartFilter(BOOL bVideo,BOOL bRun,BOOL bInsert,BOOL bRestart)
{
	BOOL bRet = FALSE;
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();

	//�t�B���^�̍č\�z����(�v���O�C���̍Ĕz�u�������)
	if (bVideo)
	{
		//�L���ȃ��f�B�A�^�C�v�����݂��Ă���ꍇ
		if (pDSA->m_VideoFirstMediaType.formattype != GUID_NULL) {
			bRet = pDSA->SelectVideoSettingIndex(pDSA->GetVideoSettingIndex(),&(pDSA->m_VideoFirstMediaType));
		} else if (pDSA->GetVideoSettingIndex() != -1){
			bRet = pDSA->SelectVideoSettingIndex(pDSA->GetVideoSettingIndex(),NULL);
		} else {
			return FALSE;
		}
	}
	else {
		if (pDSA->m_AudioFirstMediaType.formattype != GUID_NULL) {
			bRet = pDSA->SelectAudioSettingIndex(pDSA->GetAudioSettingIndex(),&(pDSA->m_AudioFirstMediaType));
		} else if (pDSA->GetAudioSettingIndex() != -1) {
			bRet = pDSA->SelectAudioSettingIndex(pDSA->GetAudioSettingIndex(),NULL);
		} else {
			return FALSE;
		}
	}
	if (bRestart == TRUE)	//�ċN��������߂�
		return TRUE;

	if (bRet == FALSE && bInsert == TRUE)
	{
		//�t�B���^�\�z���s�̏ꍇ�͍Ō�ɑ}�������v���O�C�����폜����
		if (bVideo)
			pDSA->RemovePlugin(bVideo,(int)(pDSA->m_VideoPluginList.size()-1));
		else
			pDSA->RemovePlugin(bVideo,(int)(pDSA->m_AudioPluginList.size()-1));
		//�ċN���� (bRestart == TRUE)
		StartFilter(bVideo,bRun,bInsert,TRUE);
	}

	//�t�B���^�̍ĊJ
	if (pDSA->IsCanPreview())
	{
		if (bVideo)
		{
			((CMainFrame*)AfxGetMainWnd())->GetPreviewWnd()->SetPreviewWnd();
			((CMainFrame*)AfxGetMainWnd())->GetPreviewWnd()->SetPreviewSize(TRUE);
		}
		BOOL bRunFilter = pDSA->RunFilter();
		return (bRet && bRunFilter);
	}
	return bRet;
}

