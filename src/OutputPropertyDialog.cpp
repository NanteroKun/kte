// OutputPropertyDialog.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "KTE.h"
#include "OutputPropertyDialog.h"

// COutputPropertyDialog �_�C�A���O

IMPLEMENT_DYNAMIC(COutputPropertyDialog, CSizeDialog)

BEGIN_MESSAGE_MAP(COutputPropertyDialog, CSizeDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OUTPUT_INSERT_BUTTON, &COutputPropertyDialog::OnBnClickedOutputInsertButton)
	ON_BN_CLICKED(IDC_OUTPUT_DELETE_BUTTON, &COutputPropertyDialog::OnBnClickedOutputDeleteButton)
	ON_COMMAND(ID_OUTPUT_INSERT_PULL, &COutputPropertyDialog::OnInsertPull)
	ON_COMMAND(ID_OUTPUT_INSERT_PUSH, &COutputPropertyDialog::OnInsertPush)
	ON_COMMAND(ID_OUTPUT_INSERT_FILE, &COutputPropertyDialog::OnInsertFile)
	ON_NOTIFY(TVN_SELCHANGED, IDC_OUTPUT_TREE, &COutputPropertyDialog::OnTvnSelchangedOutputTree)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_OUTPUT_TREE, &COutputPropertyDialog::OnTvnEndlabeleditOutputTree)
	ON_MESSAGE(NOTIFY_ENABLE_CHECK,&COutputPropertyDialog::OnChildEnableCheck)
	ON_NOTIFY(TVN_GETINFOTIP, IDC_OUTPUT_TREE, &COutputPropertyDialog::OnTvnGetInfoTipOutputTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_OUTPUT_TREE, &COutputPropertyDialog::OnTvnItemexpandingOutputTree)
	//ON_NOTIFY(TVN_BEGINDRAG, IDC_OUTPUT_TREE, &COutputPropertyDialog::OnTvnBegindragOutputTree)
	//ON_WM_MOUSEMOVE()
	//ON_WM_LBUTTONUP()
	//ON_WM_SIZE()
	//ON_WM_VSCROLL()
END_MESSAGE_MAP()


//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
COutputPropertyDialog::COutputPropertyDialog(CWnd* pParent /*=NULL*/)
	: CSizeDialog(COutputPropertyDialog::IDD, pParent)
{
	m_pPullDialogList.clear();
	m_pPushDialogList.clear();
	m_pFileDialogList.clear();
	m_pNowSelectDialog = NULL;
	m_bFirstFlag = FALSE;
	//�h���b�O���h���b�v���̏�����
	//m_bDragging = FALSE;
	//m_hItemDrag = NULL;
	//m_hItemDrop = NULL;
	//m_pDragImage = NULL; 
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
COutputPropertyDialog::~COutputPropertyDialog()
{
}

void COutputPropertyDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

//////////////////////////////////////////////////
//�_�C�A���O������
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::OnInitDialog()
{
	CSizeDialog::OnInitDialog();

	m_bFirstFlag = TRUE;	//�_�C�A���O�������t���O

	m_ToolTip.Create(this);
	m_ToolTip.Activate(TRUE);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_INSERT_BUTTON),IDC_OUTPUT_INSERT_BUTTON);
	m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_DELETE_BUTTON),IDC_OUTPUT_DELETE_BUTTON);
	//m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_UP_BUTTON),IDC_OUTPUT_UP_BUTTON);
	//m_ToolTip.AddTool((CWnd *)GetDlgItem(IDC_OUTPUT_DOWN_BUTTON),IDC_OUTPUT_DOWN_BUTTON);
	m_ToolTip.SetMaxTipWidth(300);
	DWORD dwAutoPop = m_ToolTip.GetDelayTime(TTDT_AUTOPOP);
	m_ToolTip.SetDelayTime(TTDT_AUTOPOP,dwAutoPop*2);

	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE); assert(tree);

	//�c���[�̕\���X�^�C���̕ύX
	tree->ModifyStyle(0,TVS_INFOTIP);
	tree->ModifyStyle(TVS_CHECKBOXES, 0 );
	tree->ModifyStyle(0, TVS_CHECKBOXES );

	//�e�c���[�A�C�e���̒ǉ�
	m_hPullTree = tree->InsertItem(_T("�u���[�h�L���X�g"));
	m_hPushTree = tree->InsertItem(_T("�p�u���b�V���|�C���g"));
	m_hFileTree = tree->InsertItem(_T("�t�@�C���A�[�J�C�u"));

	//�����Ńc���[�𕜌����A�e��_�C�A���O�𐶐�����
	LoadProfileSetting();

	m_bFirstFlag = FALSE;	//�_�C�A���O�������t���O
	return TRUE;
}

//////////////////////////////////////////////////
//�_�C�A���O�I����
//////////////////////////////////////////////////
void COutputPropertyDialog::OnDestroy()
{
	//�v���t�@�C���ւ̃Z�[�u
	SaveProfileSetting();

	/*
	//�h���b�O�C���[�W�̍폜
	CImageList *pImageList;
	pImageList = tree->GetImageList(TVSIL_NORMAL);
	if (pImageList) {
		pImageList->DeleteImageList();
		delete pImageList; 
	}
	*/
	//���ׂẴ_�C�A���O�����
	DeleteAllDialog();

	CDialog::OnDestroy();
}
//////////////////////////////////////////////////
//�_�C�A���O�z������
//////////////////////////////////////////////////
void COutputPropertyDialog::DeleteAllDialog()
{
	for (int i=0;i<(int)m_pPullDialogList.size();i++) {
		if (m_pPullDialogList[i])
		{
			m_pPullDialogList[i]->EndDialog(0);
			m_pPullDialogList[i]->DestroyWindow();
			delete m_pPullDialogList[i];
			m_pPullDialogList[i] = NULL;
		}
	}
	m_pPullDialogList.clear();
	for (int i=0;i<(int)m_pPushDialogList.size();i++) {
		if (m_pPushDialogList[i])
		{
			m_pPushDialogList[i]->EndDialog(0);
			m_pPushDialogList[i]->DestroyWindow();
			delete m_pPushDialogList[i];
			m_pPushDialogList[i] = NULL;
		}
	}
	m_pPushDialogList.clear();
	for (int i=0;i<(int)m_pFileDialogList.size();i++) {
		if (m_pFileDialogList[i])
		{
			m_pFileDialogList[i]->EndDialog(0);
			m_pFileDialogList[i]->DestroyWindow();
			delete m_pFileDialogList[i];
			m_pFileDialogList[i] = NULL;
		}
	}
	m_pFileDialogList.clear();
	return;
}

//////////////////////////////////////////////////
//�c�[���`�b�v�̕\��
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::PreTranslateMessage(MSG* pMsg)
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
//���b�Z�[�W�ʒm�̃C�x���g�n���h��
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	LPNMHDR lpnmh = (LPNMHDR) lParam;
	if ((lpnmh->code  == NM_CLICK) && (lpnmh->idFrom == IDC_OUTPUT_TREE))
	{
		TVHITTESTINFO ht = {0};
		DWORD dwpos = GetMessagePos();
		ht.pt.x = GET_X_LPARAM(dwpos);
		ht.pt.y = GET_Y_LPARAM(dwpos);
		::MapWindowPoints(HWND_DESKTOP, lpnmh->hwndFrom, &ht.pt, 1);
		TreeView_HitTest(lpnmh->hwndFrom, &ht);
		if(TVHT_ONITEMSTATEICON & ht.flags)
		{
			HTREEITEM hItem = ht.hItem;
			CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
			assert(tree);
			BOOL bCheck = tree->GetCheck(hItem);
			OnTreeCheckEvent(hItem,bCheck? FALSE:TRUE,TRUE);	//�`�F�b�N�{�b�N�X�Ƀ`�F�b�N�����������Ƃ�ʒm����
		}
	}
	return CDialog::OnNotify(wParam, lParam, pResult);
}
//////////////////////////////////////////////////
//�q�E�B���h�E������̃��b�Z�[�W�ʒm�C�x���g�n���h��
//�c���[�r���[�̃`�F�b�N�}�[�N�ʒm�ׂ̈Ɏg����
//////////////////////////////////////////////////
LRESULT COutputPropertyDialog::OnChildEnableCheck(WPARAM wParam,LPARAM lParam)
{
	//TRACE0("OnChildEnableCheck()\n");
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
	assert(tree);

	COutputBaseDialog* pDialog = (COutputBaseDialog*)wParam;	//�����Ƃ��ēn���ꂽ�|�C���^
	assert(pDialog);

	//�_�C�A���O�Ɗ֘A�t�����ꂽ�c���[�A�C�e���̌���
	HTREEITEM hDialogItem = NULL;
	HTREEITEM hChildItem;

	hChildItem = tree->GetChildItem(m_hPullTree);
	while (hChildItem != NULL) {
		if (pDialog == m_pPullDialogList[tree->GetItemData(hChildItem)]) {
			hDialogItem = hChildItem;
			break;
		}
		hChildItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
	}
	hChildItem = tree->GetChildItem(m_hPushTree);
	while (hChildItem != NULL) {
		if (pDialog == m_pPushDialogList[tree->GetItemData(hChildItem)]) {
			hDialogItem = hChildItem;
			break;
		}
		hChildItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
	}
	hChildItem = tree->GetChildItem(m_hFileTree);
	while (hChildItem != NULL) {
		if (pDialog == m_pFileDialogList[tree->GetItemData(hChildItem)]) {
			hDialogItem = hChildItem;
			break;
		}
		hChildItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
	}
	//�`�F�b�N������
	if (hDialogItem)
	{
		BOOL bCheck = (BOOL)lParam;
		tree->SetCheck(hDialogItem,bCheck);
		OnTreeCheckEvent(hDialogItem,bCheck,TRUE);
	}
	return 0;
}
//////////////////////////////////////////////////
//�o�̓\�[�X�̒ǉ��{�^��
//�E�N���b�N���j���[�Œǉ����j���[��\������
//////////////////////////////////////////////////
void COutputPropertyDialog::OnBnClickedOutputInsertButton()
{
	CMenu cMenu;
	cMenu.LoadMenu(IDR_OUTPUT_INSERT_POPUP);
	CMenu* pPopup = cMenu.GetSubMenu(0);
	assert(pPopup);
	POINT pt;
	GetCursorPos(&pt);
	pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,pt.x,pt.y,this);
	cMenu.DestroyMenu();
}
//////////////////////////////////////////////////
//�o�̓\�[�X�̒ǉ�
//OUTPUT_TYPE 	OUTPUT_TYPE_PULL		�u���[�h�L���X�g�̒ǉ�
//				OUTPUT_TYPE_PUSH		�p�u���b�V���|�C���g�̒ǉ�
//				OUTPUT_TYPE_FILE		�t�@�C���A�[�J�C�u�̒ǉ�
//////////////////////////////////////////////////
HTREEITEM COutputPropertyDialog::OnInsertOutput(OUTPUT_TYPE nOutputType,LPCTSTR lpszItemName,BOOL bCheck)
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
	assert(tree);
	HTREEITEM hNewItem = NULL;
	HTREEITEM hParentItem = NULL;
	switch (nOutputType) {
	case OUTPUT_TYPE_PULL:
		hParentItem = m_hPullTree; break;
	case OUTPUT_TYPE_PUSH:
		hParentItem = m_hPushTree; break;
	case OUTPUT_TYPE_FILE:
		hParentItem = m_hFileTree; break;
	}
	//�c���[�^�u�̃f�t�H���g����ύX����
	CString strTabName(_T(""));
	if (lpszItemName == NULL) {
		switch (nOutputType) {
		case OUTPUT_TYPE_PULL:
			lpszItemName = _T("�v��"); break;
		case OUTPUT_TYPE_PUSH:
			lpszItemName = _T("�v�b�V��"); break;
		case OUTPUT_TYPE_FILE:
			lpszItemName = _T("�t�@�C��"); break;
		}
		strTabName = lpszItemName;	//�Ƃ肠�����������Ƃ��ē���Ă���
		CString strSuggestTabName(_T(""));
		for (int i=1;i<INT_MAX;i++) {
			strSuggestTabName.Format(_T("%s%d"),lpszItemName,i);
			BOOL bConflict = FALSE;
			HTREEITEM hChildItem, hNextItem;
			hChildItem = tree->GetChildItem(hParentItem);
			while (hChildItem != NULL) {
				hNextItem = tree->GetNextItem(hChildItem, TVGN_NEXT);
				if (strSuggestTabName.Compare(tree->GetItemText(hChildItem)) == 0)
					bConflict = TRUE;
				hChildItem = hNextItem;
			}
			if (bConflict == FALSE) {
				strTabName = strSuggestTabName;
				break;
			}
		}
	} else {
		strTabName = lpszItemName;
	}

	//�c���[�A�C�e���̒ǉ�
	hNewItem= tree->InsertItem(strTabName,hParentItem,TVI_LAST);

	//�V�����_�C�A���O���쐬����
	CRect rcDialog(0,0,0,0);
	CSize OffsetSize(0,130);
	switch (nOutputType)
	{
	case OUTPUT_TYPE_PULL:
		{
			COutputPullDialog* pPullDialog = new COutputPullDialog;
			pPullDialog->Create(COutputPullDialog::IDD,this);
			pPullDialog->GetClientRect(&rcDialog);
			rcDialog.OffsetRect(OffsetSize);
			pPullDialog->MoveWindow(&rcDialog,FALSE);
			pPullDialog->SetTabName(strTabName);
			//����Pull�E�B���h�E�̒l���`�F�b�N���ă|�[�g�ԍ�������
			int nSuggestPort = 8080;
			for (;nSuggestPort <= 65535;nSuggestPort++)
			{
				BOOL bSuccess = TRUE;
				for (int i=0;i<(int)m_pPullDialogList.size();i++)
				{
					if (m_pPullDialogList[i] != NULL)
					{
						int nPort;
						m_pPullDialogList[i]->GetPort(nPort);
						if (nSuggestPort == nPort)
							bSuccess = FALSE;
					}
				}
				if (bSuccess == TRUE)
					break;
			}
			pPullDialog->SetPort(nSuggestPort);
			int nInsertIndex = -1;
			BOOL bInsert = FALSE;
			for (int i=0;i<(int)m_pPullDialogList.size();i++) {
				if (m_pPullDialogList[i] == NULL) {
					m_pPullDialogList[i] = pPullDialog;
					nInsertIndex = i;
					bInsert = TRUE;
					break;
				}
			}
			if (bInsert == FALSE) {
				m_pPullDialogList.push_back(pPullDialog);
				nInsertIndex = (int)(m_pPullDialogList.size() - 1);
			}
			tree->SetItemData(hNewItem,nInsertIndex);
		}
		break;
	case OUTPUT_TYPE_PUSH:
		{
			COutputPushDialog* pPushDialog = new COutputPushDialog;
			pPushDialog->Create(COutputPushDialog::IDD,this);
			pPushDialog->GetClientRect(&rcDialog);
			rcDialog.OffsetRect(OffsetSize);
			pPushDialog->MoveWindow(&rcDialog,FALSE);
			pPushDialog->SetTabName(strTabName);
			int nInsertIndex = -1;
			BOOL bInsert = FALSE;
			for (int i=0;i<(int)m_pPushDialogList.size();i++) {
				if (m_pPushDialogList[i] == NULL) {
					m_pPushDialogList[i] = pPushDialog;
					nInsertIndex = i;
					bInsert = TRUE;
					break;
				}
			}
			if (bInsert == FALSE) {
				m_pPushDialogList.push_back(pPushDialog);
				nInsertIndex = (int)(m_pPushDialogList.size() - 1);
			}
			tree->SetItemData(hNewItem,nInsertIndex);
		}
		break;
	case OUTPUT_TYPE_FILE:
		{
			COutputFileDialog* pFileDialog = new COutputFileDialog;
			pFileDialog->Create(COutputFileDialog::IDD,this);
			pFileDialog->GetClientRect(&rcDialog);
			rcDialog.OffsetRect(OffsetSize);
			pFileDialog->MoveWindow(&rcDialog,FALSE);
			pFileDialog->SetTabName(strTabName);

			//�f�t�H���g�̃t�H���_���ƃt�@�C�����̌���
			TCHAR SpecialPath[MAX_PATH];
			if (SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_MYVIDEO,FALSE) != TRUE)
				SHGetSpecialFolderPath(NULL,SpecialPath,CSIDL_DESKTOP,FALSE);
			//pFileDialog->SetOutputFileName(_T("Live %Y�N%m��%d��%H��%M��%S�b.asf"));//�t�@�C�����͏d��������邽�߂ɋ󔒂ɂ��Ă���
			pFileDialog->SetOutputFolderName(SpecialPath);

			int nInsertIndex = -1;
			BOOL bInsert = FALSE;
			for (int i=0;i<(int)m_pFileDialogList.size();i++) {
				if (m_pFileDialogList[i] == NULL) {
					m_pFileDialogList[i] = pFileDialog;
					nInsertIndex = i;
					bInsert = TRUE;
					break;
				}
			}
			if (bInsert == FALSE) {
				m_pFileDialogList.push_back(pFileDialog);
				nInsertIndex = (int)(m_pFileDialogList.size() - 1);
			}
			tree->SetItemData(hNewItem,nInsertIndex);
		}
		break;
	}

	//�L���`�F�b�N�̗L���̌���
	CDirectShowAccess* pDSA = ((CKTEApp*)AfxGetApp())->GetDSA();
	if (pDSA->IsEncode()) {
		tree->SetCheck(hNewItem,FALSE);		//�G���R�[�h���͏����L���{�^���Ƀ`�F�b�N�����Ȃ�
	} else if (m_bFirstFlag) {
		tree->SetCheck(hNewItem,bCheck);	//����N���ł͈����ɏ]��
	} else {
		tree->SetCheck(hNewItem,tree->GetCheck(hParentItem));		//�e���`�F�b�N��ԂȂ�q���`�F�b�N��Ԃɂ���
	}

	//�c���[��W�J���đI����Ԃɂ���
	if (!m_bFirstFlag) {
		tree->Expand(hParentItem,TVE_EXPAND);
		tree->SelectItem(hNewItem);
	}

	//�_�C�A���O�Ɛe�c���[�Ƀ`�F�b�N�̒ʒm���s��
	OnTreeCheckEvent(hNewItem,tree->GetCheck(hNewItem),FALSE);

	return hNewItem;
}

//////////////////////////////////////////////////
//�폜�{�^��
//////////////////////////////////////////////////
void COutputPropertyDialog::OnBnClickedOutputDeleteButton()
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
	assert(tree);
	HTREEITEM hItem = tree->GetSelectedItem();
	if (hItem != NULL && hItem != m_hPullTree && hItem != m_hPushTree && hItem != m_hFileTree)
	{
		HTREEITEM hParentItem = tree->GetParentItem(hItem);
		assert(hParentItem);
		int nIndex = (int)tree->GetItemData(hItem);
		if (hParentItem == m_hPullTree)
		{
			m_pPullDialogList[nIndex]->ShowWindow(SW_HIDE);
			m_pPullDialogList[nIndex]->EndDialog(0);
			m_pPullDialogList[nIndex]->DestroyWindow();
			delete m_pPullDialogList[nIndex];
			m_pPullDialogList[nIndex] = NULL;
		}
		else if (hParentItem == m_hPushTree)
		{
			m_pPushDialogList[nIndex]->ShowWindow(SW_HIDE);
			m_pPushDialogList[nIndex]->EndDialog(0);
			m_pPushDialogList[nIndex]->DestroyWindow();
			delete m_pPushDialogList[nIndex];
			m_pPushDialogList[nIndex] = NULL;
		}
		else if (hParentItem == m_hFileTree)
		{
			m_pFileDialogList[nIndex]->ShowWindow(SW_HIDE);
			m_pFileDialogList[nIndex]->EndDialog(0);
			m_pFileDialogList[nIndex]->DestroyWindow();
			delete m_pFileDialogList[nIndex];
			m_pFileDialogList[nIndex] = NULL;
		}
		else {
			AfxMessageBox(_T("�c���[�r���[�̃A�C�e�����폜�ł��܂���ł���"),MB_OK|MB_ICONINFORMATION);
			return;
		}
		//�t�H�[�J�X�����̃A�C�e���Ɉړ�
		HTREEITEM hNextItem = tree->GetNextItem(hItem,TVGN_NEXT);
		if (hNextItem == NULL)
		{
			hNextItem = tree->GetNextItem(hItem,TVGN_PREVIOUS);
			if (hNextItem == NULL)
				hNextItem = tree->GetParentItem(hItem);
		}
		assert(hNextItem);
		m_pNowSelectDialog = NULL;
		tree->DeleteItem(hItem);	//�A�C�e�����폜
		tree->SelectItem(hNextItem);
	}
}

//////////////////////////////////////////////////
//�c���[�R���g���[�����獀�ڑI����ύX�����Ƃ��ɑ����Ă��郁�b�Z�[�W�n���h��
//////////////////////////////////////////////////
void COutputPropertyDialog::OnTvnSelchangedOutputTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	switch(pNMTreeView->hdr.code)
	{
		case TVN_SELCHANGED:
		{
			HTREEITEM hItem = pNMTreeView->itemNew.hItem;
			if (hItem != 0)
			{
				if (m_pNowSelectDialog != NULL)
					m_pNowSelectDialog->ShowWindow(SW_HIDE);
				m_pNowSelectDialog = NULL;
				CButton* button = (CButton*)GetDlgItem(IDC_OUTPUT_DELETE_BUTTON);
				assert(button);
				//�e�c���[���j���[��I��
				if (hItem == m_hPullTree || hItem == m_hPushTree || hItem == m_hFileTree)
				{
					button->EnableWindow(FALSE);					//�폜�{�^���𖳌��ɂ���
					break;
				} else {	//�q�c���[���j���[��I��
					button->EnableWindow(TRUE);						//�폜�{�^����L���ɂ���
					CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
					assert(tree);
					int nIndex = (int)pNMTreeView->itemNew.lParam;
					HTREEITEM hParent = tree->GetParentItem(hItem);
					if (hParent == m_hPullTree)
					{
						m_pPullDialogList[nIndex]->ShowWindow(SW_SHOW);
						m_pNowSelectDialog = m_pPullDialogList[nIndex];
					}
					else if (hParent == m_hPushTree)
					{
						m_pPushDialogList[nIndex]->ShowWindow(SW_SHOW);
						m_pNowSelectDialog = m_pPushDialogList[nIndex];
					}
					else if (hParent == m_hFileTree)
					{
						m_pFileDialogList[nIndex]->ShowWindow(SW_SHOW);
						m_pNowSelectDialog = m_pFileDialogList[nIndex];
					}
				}
			}
			break;
		}
	}
	*pResult = 0;
}
//////////////////////////////////////////////////
//�e�L�X�g�ҏW��ɒʒm�����C�x���g�n���h��
//////////////////////////////////////////////////
void COutputPropertyDialog::OnTvnEndlabeleditOutputTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	HTREEITEM hTreeItem = pTVDispInfo->item.hItem;
	if (hTreeItem != NULL && hTreeItem != m_hPullTree && hTreeItem != m_hPushTree && hTreeItem != m_hFileTree)
	{
		CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE); assert(tree);
		tree->SetItem(&pTVDispInfo->item);
		if (pTVDispInfo->item.pszText != NULL)
		{
			//�ݒ��K�p����
			int nIndex = (int)tree->GetItemData(hTreeItem);
			HTREEITEM hParentItem = tree->GetParentItem(hTreeItem);
			if (hParentItem == m_hPullTree) {
				m_pPullDialogList[nIndex]->SetTabName(pTVDispInfo->item.pszText);
			} else if (hParentItem == m_hPushTree) {
				m_pPushDialogList[nIndex]->SetTabName(pTVDispInfo->item.pszText);
			} else if (hParentItem == m_hFileTree) {
				m_pFileDialogList[nIndex]->SetTabName(pTVDispInfo->item.pszText);
			} else {
				AfxMessageBox(_T("�e�L�X�g�ҏW����A�C�e����������܂���ł���"),MB_OK|MB_ICONINFORMATION);
			}
		}
	}
	*pResult = 0;
}
//////////////////////////////////////////////////
//�c���[�r���[�̃`�F�b�N�{�b�N�X�Ƀ`�F�b�N�����������̃C�x���g�n���h��
//bNotifyCall�̓}�E�X�N���b�N�ɂ��`�F�b�N�ɔ��������ꍇ
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::OnTreeCheckEvent(HTREEITEM hTreeItem,BOOL bCheck,BOOL bNotifyCall)
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
	assert(tree);

	//�e�c���[���N���b�N
	if (hTreeItem == m_hPullTree || hTreeItem == m_hPushTree || hTreeItem == m_hFileTree){
		tree->SelectItem(hTreeItem);

		HTREEITEM hChildItem = tree->GetChildItem(hTreeItem);
		HTREEITEM hNextItem;
		while (hChildItem != NULL)
		{
			hNextItem = tree->GetNextItem(hChildItem, TVGN_NEXT);
			tree->SetCheck(hChildItem,bCheck);

			//�q�̃`�F�b�N�{�b�N�X�ɒʒm(�ċA)
			OnTreeCheckEvent(hChildItem,bCheck,FALSE);
			hChildItem = hNextItem;
		}
		return TRUE;
	}
	else {	//�q�c���[���N���b�N
		//TODO �����Ɏq�̃`�F�b�N�L���Ɩ����Ɋւ���R�[�h���L�q����

		int nIndex = (int)tree->GetItemData(hTreeItem);
		HTREEITEM hParentItem2 = tree->GetParentItem(hTreeItem);
		if (hParentItem2 == m_hPullTree) {
			if (m_pPullDialogList[nIndex]->GetEnableCheck() != bCheck)
				m_pPullDialogList[nIndex]->SetEnableCheck(bCheck);
		} else if (hParentItem2 == m_hPushTree) {
			if (m_pPushDialogList[nIndex]->GetEnableCheck() != bCheck)
				m_pPushDialogList[nIndex]->SetEnableCheck(bCheck);
		} else if (hParentItem2 == m_hFileTree) {
			if (m_pFileDialogList[nIndex]->GetEnableCheck() != bCheck)
				m_pFileDialogList[nIndex]->SetEnableCheck(bCheck);
		} else {
		}

		if (bNotifyCall == TRUE)
			tree->SelectItem(hTreeItem);

		//�S�Ă̎q�c���[�̃`�F�b�N���O��Ă�����t���Ă��肵���ꍇ�e�̃`�F�b�N��ω�������
		if (bNotifyCall == TRUE)
		{
			HTREEITEM hParentItem = NULL;
			hParentItem = tree->GetParentItem(hTreeItem);
			assert(hParentItem);
			BOOL bParentCheck = tree->GetCheck(hParentItem);	//�e�̃`�F�b�N�}�[�N�̏��

			BOOL bParentCheckChangeFlag = TRUE;	//�e�̃`�F�b�N��ύX���邩�̃t���O
			HTREEITEM hChildItem = tree->GetChildItem(hParentItem);
			HTREEITEM hNextItem;
			while (hChildItem != NULL)
			{
				hNextItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
				BOOL bChildCheck;
				if (hChildItem != hTreeItem)
					bChildCheck = tree->GetCheck(hChildItem);
				else
					bChildCheck = bCheck;
				if (bParentCheck == bChildCheck)
					bParentCheckChangeFlag = FALSE;
				hChildItem = hNextItem;
			}
			if (bParentCheckChangeFlag == TRUE)
				tree->SetCheck(hParentItem,bParentCheck?FALSE:TRUE);
		}
		return TRUE;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�c���[�r���[�̓c�[���`�b�v���𓾂邽�߂ɐe�E�B���h�E�� 
//TVN_GETINFOTIP �ʒm���b�Z�[�W�𑗂�܂��B
//////////////////////////////////////////////////
void COutputPropertyDialog::OnTvnGetInfoTipOutputTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMTVGETINFOTIP>(pNMHDR);
	const int nBufferSize = 400;
	CString strToolTip;
	TCHAR* pStr = strToolTip.GetBuffer(nBufferSize);
	if (this->m_hFileTree == pGetInfoTip->hItem || this->m_hPullTree == pGetInfoTip->hItem || this->m_hPushTree == pGetInfoTip->hItem)
	{
		if (this->m_hFileTree == pGetInfoTip->hItem)
			::LoadString(::AfxGetInstanceHandle(),IDS_STRING_OUTPUT_FILE,pStr,nBufferSize+1);
		else if (this->m_hPullTree == pGetInfoTip->hItem)
			::LoadString(::AfxGetInstanceHandle(),IDS_STRING_OUTPUT_PULL,pStr,nBufferSize+1);
		else 
			::LoadString(::AfxGetInstanceHandle(),IDS_STRING_OUTPUT_PUSH,pStr,nBufferSize+1);
		 _tcscpy_s(pGetInfoTip->pszText,nBufferSize+1,strToolTip);
		pGetInfoTip->cchTextMax = strToolTip.GetLength();
	} else {	//�q�A�C�e���̏ꍇ
		CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
		assert(tree);
		HTREEITEM hParentItem = tree->GetParentItem(pGetInfoTip->hItem);
		if (hParentItem == this->m_hFileTree) {
			//COutputFileDialog* pFileDialog = (COutputFileDialog*)tree->GetItemData(pGetInfoTip->hItem);
			COutputFileDialog* pFileDialog = m_pFileDialogList[tree->GetItemData(pGetInfoTip->hItem)];
			assert(pFileDialog);
			CString strTabName = tree->GetItemText(pGetInfoTip->hItem);
			CString strFileName;
			CString strFolderName;
			pFileDialog->GetOutputFileName(strFileName);
			pFileDialog->GetOutputFolderName(strFolderName);
			strToolTip.Format(_T("%s\r\n�t�@�C�� %s\r\n�t�H���_ %s"),strTabName,strFileName,strFolderName);
		} else if (hParentItem == this->m_hPullTree) {
			//COutputPullDialog* pPullDialog = (COutputPullDialog*)tree->GetItemData(pGetInfoTip->hItem);
			COutputPullDialog* pPullDialog = m_pPullDialogList[tree->GetItemData(pGetInfoTip->hItem)];
			assert(pPullDialog);
			CString strTabName = tree->GetItemText(pGetInfoTip->hItem);
			int nMax,nPort;
			pPullDialog->GetMax(nMax);
			pPullDialog->GetPort(nPort);
			strToolTip.Format(_T("%s\r\n�|�[�g�ԍ� %d\r\n�ő�ڑ��l�� %d"),strTabName,nPort,nMax);
		} else {
			//COutputPushDialog* pPushDialog = (COutputPushDialog*)tree->GetItemData(pGetInfoTip->hItem);
			COutputPushDialog* pPushDialog = m_pPushDialogList[tree->GetItemData(pGetInfoTip->hItem)];
			assert(pPushDialog);
			CString strTabName = tree->GetItemText(pGetInfoTip->hItem);
			CString strFileName;
			pPushDialog->GetOutputName(strFileName);
			BOOL bAutoDelete = pPushDialog->GetAutoDeleteCheck();
			strToolTip.Format(_T("%s\r\n�T�[�o�[�� %s\r\n�����폜 %s"),strTabName,strFileName,bAutoDelete?_T("�L��"):_T("����"));
		}
		 _tcscpy_s(pGetInfoTip->pszText,nBufferSize+1,strToolTip);
		pGetInfoTip->cchTextMax = strToolTip.GetLength();
	}
	// TODO: �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	if (pResult)
		*pResult = 0;
}

//////////////////////////////////////////////////
//�c���[�r���[���J�����ςȂ��ɂ���
//////////////////////////////////////////////////
void COutputPropertyDialog::OnTvnItemexpandingOutputTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	if (pNMTreeView->action == TVE_COLLAPSE)
		*pResult = 1;
	else
		*pResult = 0;
}
//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::ChangeProfileNotify()
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE); assert(tree);

	m_bFirstFlag = TRUE;		//�_�C�A���O�������t���O
	m_pNowSelectDialog = NULL;	//���ݑI�𒆂̃_�C�A���O��������

	//�_�C�A���O���X�g�����
	DeleteAllDialog();

	//�c���[�r���[���S�폜
	HTREEITEM hChildItem,hNextItem;
	hChildItem = tree->GetChildItem(m_hPullTree);
	while (hChildItem != NULL)
	{
		hNextItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
		tree->DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}
	hChildItem = tree->GetChildItem(m_hPushTree);
	while (hChildItem != NULL)
	{
		hNextItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
		tree->DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}
	hChildItem = tree->GetChildItem(m_hFileTree);
	while (hChildItem != NULL)
	{
		hNextItem = tree->GetNextItem(hChildItem,TVGN_NEXT);
		tree->DeleteItem(hChildItem);
		hChildItem = hNextItem;
	}

	//�v���t�@�C������̃��[�h
	LoadProfileSetting();

	m_bFirstFlag = FALSE;	//�_�C�A���O�������t���O

	//�E�B���h�E�̕`��ʒm
	CWnd* parent = this->GetParent();
	parent->InvalidateRect(NULL);
	parent->UpdateWindow();
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C������̃��[�h
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::LoadProfileSetting()
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE); assert(tree);

	//�����Ńc���[�𕜌����A�e��_�C�A���O�𐶐�����
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	tree->SetCheck(m_hPullTree,pProfile->m_bOutputPullCheck);
	tree->SetCheck(m_hPushTree,pProfile->m_bOutputPushCheck);
	tree->SetCheck(m_hFileTree,pProfile->m_bOutputFileCheck);
	HTREEITEM hItem;
	for (int i=0;i<pProfile->m_nOutputPullCount;i++)
	{
		hItem = OnInsertOutput(OUTPUT_TYPE_PULL,
			pProfile->m_OutputPullList[i].Data.strTabName,
			pProfile->m_OutputPullList[i].Data.bTabEnable);
		assert(hItem);
		COutputPullDialog* pDialog = m_pPullDialogList[tree->GetItemData(hItem)];
		pDialog->SetPort(pProfile->m_OutputPullList[i].nPort);
		pDialog->SetMax(pProfile->m_OutputPullList[i].nMax);
	}
	for (int i=0;i<pProfile->m_nOutputPushCount;i++)
	{
		hItem = OnInsertOutput(OUTPUT_TYPE_PUSH,
			pProfile->m_OutputPushList[i].Data.strTabName,
			pProfile->m_OutputPushList[i].Data.bTabEnable);
		assert(hItem);
		COutputPushDialog* pDialog = m_pPushDialogList[tree->GetItemData(hItem)];
		pDialog->SetOutputName(pProfile->m_OutputPushList[i].strServerName);
		pDialog->SetAutoDeleteCheck(pProfile->m_OutputPushList[i].bAutoDelete);
	}
	for (int i=0;i<pProfile->m_nOutputFileCount;i++)
	{
		hItem = OnInsertOutput(OUTPUT_TYPE_FILE,
			pProfile->m_OutputFileList[i].Data.strTabName,
			pProfile->m_OutputFileList[i].Data.bTabEnable);
		assert(hItem);
		COutputFileDialog* pDialog = m_pFileDialogList[tree->GetItemData(hItem)];
		pDialog->SetOutputFileName(pProfile->m_OutputFileList[i].strFileName);
		pDialog->SetOutputFolderName(pProfile->m_OutputFileList[i].strFolderName);
	}
	tree->Expand(m_hPullTree,TVE_EXPAND);		//�c���[��W�J����
	tree->Expand(m_hPushTree,TVE_EXPAND);		//�c���[��W�J����
	tree->Expand(m_hFileTree,TVE_EXPAND);		//�c���[��W�J����

	//�����c���[�r���[�̃t�H�[�J�X��ݒ肷��
	HTREEITEM hSelectItem = 0;
	if (m_pPullDialogList.size() != 0) {
		hSelectItem = tree->GetChildItem(m_hPullTree);
	} else if (m_pPushDialogList.size() != 0) {
		hSelectItem = tree->GetChildItem(m_hPushTree);
	} else if (m_pFileDialogList.size() != 0) {
		hSelectItem = tree->GetChildItem(m_hFileTree);
	}
	if (hSelectItem != 0)
		tree->SelectItem(hSelectItem);
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���ւ̃Z�[�u
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::SaveProfileSetting()
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE); assert(tree);

	//�e��^�u�̊���ۑ�
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	pProfile->m_bOutputPullCheck = tree->GetCheck(m_hPullTree);
	pProfile->m_bOutputPushCheck = tree->GetCheck(m_hPushTree);
	pProfile->m_bOutputFileCheck = tree->GetCheck(m_hFileTree);

	pProfile->m_OutputPullList.clear();
	pProfile->m_OutputPushList.clear();
	pProfile->m_OutputFileList.clear();
	int i = 0;
	HTREEITEM hChildItem, hNextItem;
	hChildItem = tree->GetChildItem(m_hPullTree);
	while (hChildItem != NULL) {
		hNextItem = tree->GetNextItem(hChildItem, TVGN_NEXT);
		COutputPullDialog* pDialog = m_pPullDialogList[tree->GetItemData(hChildItem)];
		assert(pDialog);
		IniOutputPullStruct iops;
		iops.Data.nIndex = i;
		iops.Data.bTabEnable = tree->GetCheck(hChildItem);
		iops.Data.strTabName = tree->GetItemText(hChildItem);
		iops.nPort = 8080;
		pDialog->GetPort(iops.nPort);
		iops.nMax = 10;
		pDialog->GetMax(iops.nMax);
		pProfile->m_OutputPullList.push_back(iops);
		hChildItem = hNextItem;
		i++;
	}
	pProfile->m_nOutputPullCount = (int)pProfile->m_OutputPullList.size();

	i = 0;
	hChildItem = tree->GetChildItem(m_hPushTree);
	while (hChildItem != NULL) {
		hNextItem = tree->GetNextItem(hChildItem, TVGN_NEXT);
		COutputPushDialog* pDialog = m_pPushDialogList[tree->GetItemData(hChildItem)];
		assert(pDialog);
		IniOutputPushStruct iops;
		iops.Data.nIndex = i;
		iops.Data.bTabEnable = tree->GetCheck(hChildItem);
		iops.Data.strTabName = tree->GetItemText(hChildItem);
		iops.bAutoDelete = pDialog->GetAutoDeleteCheck();
		iops.strServerName = _T("");
		pDialog->GetOutputName(iops.strServerName);
		pProfile->m_OutputPushList.push_back(iops);
		hChildItem = hNextItem;
		i++;
	}
	pProfile->m_nOutputPushCount = (int)pProfile->m_OutputPushList.size();

	i = 0;
	hChildItem = tree->GetChildItem(m_hFileTree);
	while (hChildItem != NULL) {
		hNextItem = tree->GetNextItem(hChildItem, TVGN_NEXT);
		COutputFileDialog* pDialog = m_pFileDialogList[tree->GetItemData(hChildItem)];
		assert(pDialog);
		IniOutputFileStruct iofs;
		iofs.Data.nIndex = i;
		iofs.Data.bTabEnable = tree->GetCheck(hChildItem);
		iofs.Data.strTabName = tree->GetItemText(hChildItem);
		iofs.strFileName = _T("");
		pDialog->GetOutputFileName(iofs.strFileName);
		pDialog->GetOutputFolderName(iofs.strFolderName);
		pProfile->m_OutputFileList.push_back(iofs);
		hChildItem = hNextItem;
		i++;
	}
	pProfile->m_nOutputFileCount = (int)pProfile->m_OutputFileList.size();
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̊J�n�ƒ�~
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::Encode(BOOL bStart)
{
	//�ێ����Ă���q�c���[�̃_�C�A���O�ɑ΂��ăG���R�[�h�̒ʒm���s��(�L���E�����ɂ�����炸�ʒm���s��)
	BOOL bRet = FALSE;

	for (int i=0;i<(int)m_pPullDialogList.size();i++) {
		if (m_pPullDialogList[i])
		{
			bRet = m_pPullDialogList[i]->Encode(bStart);
			if (!bRet) {
				AfxMessageBox(_T("�u���[�h�o���h�o�͂̐ݒ蒆�ɃG���[���������܂���"));
				return FALSE;
			}
		}
	}
	for (int i=0;i<(int)m_pPushDialogList.size();i++) {
		if (m_pPushDialogList[i])
		{
			bRet = m_pPushDialogList[i]->Encode(bStart);
			if (!bRet) {
				AfxMessageBox(_T("�p�u���b�V���|�C���g�o�͂̐ݒ蒆�ɃG���[���������܂���"));
				return FALSE;
			}
		}
	}
	for (int i=0;i<(int)m_pFileDialogList.size();i++) {
		if (m_pFileDialogList[i])
		{
			bRet = m_pFileDialogList[i]->Encode(bStart);
			if (!bRet) {
				AfxMessageBox(_T("�t�@�C���o�͂̐ݒ蒆�ɃG���[���������܂���"));
				return FALSE;
			}
		}
	}
	return TRUE;
}

/*
//////////////////////////////////////////////////
//�h���b�O�̊J�n
//////////////////////////////////////////////////
void COutputPropertyDialog::OnTvnBegindragOutputTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	//�h���b�O�J�n���ڂ̎擾 
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
	assert(tree);
	HTREEITEM hDragItem = tree->HitTest( pNMTreeView->ptDrag );
	if(hDragItem != NULL && hDragItem != m_hPullTree && hDragItem != m_hPushTree && hDragItem != m_hFileTree)
	{
		m_bDragging = TRUE;
		m_hItemDrag = hDragItem;
		m_hItemDrop = NULL;
		CPoint ptAction;
		GetCursorPos(&ptAction);
		tree->ScreenToClient(&ptAction);
		
		m_pDragImage = tree->CreateDragImage( hDragItem );
		assert(m_pDragImage);
		m_pDragImage->DragShowNolock( TRUE );
		m_pDragImage->SetDragCursorImage( 0, CPoint(0, 0) );
		m_pDragImage->BeginDrag( 0, CPoint(0,0) ); 
		m_pDragImage->DragMove( ptAction ); 
		m_pDragImage->DragEnter(tree, ptAction );
		tree->SetCapture();
		//this->SetCapture();
	}
	*pResult = 0;
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void COutputPropertyDialog::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_bDragging)
	{
		if(m_pDragImage) {
			CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
			assert(tree);
			m_pDragImage->DragMove(point);
			HTREEITEM hItem;
			hItem = tree->HitTest(point);
			if(hItem)
			{
				m_pDragImage->DragLeave(tree);
				// �h���b�v����ۑ�
				tree->SelectDropTarget(hItem);
				m_hItemDrop = hItem;
				m_pDragImage->DragEnter(tree,point);
			}
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
void COutputPropertyDialog::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_bDragging){ 
		CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
		assert(tree);
		if(m_pDragImage){ 
			m_pDragImage->DragLeave(tree);
			m_pDragImage->EndDrag();
			delete m_pDragImage;
			m_pDragImage = NULL;

			if(m_hItemDrag != m_hItemDrop
				&& !IsChildNodeOf(m_hItemDrop,m_hItemDrag ) 
				&& tree->GetParentItem(m_hItemDrag) != m_hItemDrop ){
					// this->m_hItemDrag ���h���b�O���A�C�e��
					// this->m_hItemDrop ���h���b�v��A�C�e�� 
			}
		}
		::ReleaseCapture();
		m_bDragging = FALSE;
		m_hItemDrag = NULL;
		m_hItemDrop = NULL;
		tree->SelectDropTarget(NULL);
	}

	CDialog::OnLButtonUp(nFlags, point);
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
BOOL COutputPropertyDialog::IsChildNodeOf( HTREEITEM hItemChild, HTREEITEM hItemSuspectedParent )
{
	CTreeCtrl* tree = (CTreeCtrl*)this->GetDlgItem(IDC_OUTPUT_TREE);
	assert(tree);
	do {
		if( hItemChild == hItemSuspectedParent ){ 
			break; 
		}
	} while( (hItemChild = tree->GetParentItem( hItemChild ) ) != NULL ); 
	return ( hItemChild != NULL ); 
}
*/

