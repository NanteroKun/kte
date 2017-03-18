#include "StdAfx.h"

#include "DirectShowAccess.h"
#include "KTE.h"
#include "MainFrm.h"

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CDirectShowAccess::CDirectShowAccess(void)
{
	m_pDirectShow = NULL;
	m_bRunFilter = FALSE;

	m_nVideoIndex = -1;
	m_nVideoSettingIndex = -1;

	m_nVideoFilterID = -1;
	m_nVideoSmartTeeID = -1;
	m_nVideoColorSpaceID = -1;
	m_nVideoAVIDecompressorID = -1;
	m_nVideoOverlayMixerID = -1;

	m_nVideoFilterOutputPinIndex = -1;

	m_bVideoFindAudioPin = FALSE;
	m_bUseAudioVideoFilter = FALSE;

	m_nCrossbar1ID = -1;
	m_nCrossbar2ID = -1;
	m_nTVAudioID = -1;
	m_nTVTunerID = -1;

	m_hVideoWnd = NULL;
	m_bVideoShow = FALSE;
	m_bVideoForceOff = FALSE;
	m_nVideoVMR = 1;
	m_bVideoVMROverlay = TRUE;

	m_strVideoNameList.clear();
	m_strVideoSettingList.clear();
	m_VideoMediaTypeList.clear();
	m_VideoCapsList.clear();
	ZeroMemory(&m_VideoFirstMediaType,sizeof(AM_MEDIA_TYPE));
	ZeroMemory(&m_VideoLastMediaType,sizeof(AM_MEDIA_TYPE));

	m_nAudioIndex = -1;
	m_nAudioSettingIndex = -1;
	m_nAudioFilterID = -1;
	m_nAudioSmartTeeID = -1;
	//m_nAudioNullFilterID = -1;
	m_nAudioRenderID = -1;
	m_nAudioFilterOutputPinIndex = -1;

	m_nAudioNullFilterID = -1;
	m_pAudioVolumeFilter = NULL;
	m_bAudioVolumeEnable = FALSE;

	m_strAudioNameList.clear();
	m_strAudioSettingList.clear();
	m_AudioMediaTypeList.clear();
	m_AudioCapsList.clear();
	ZeroMemory(&m_AudioFirstMediaType,sizeof(AM_MEDIA_TYPE));
	ZeroMemory(&m_AudioLastMediaType,sizeof(AM_MEDIA_TYPE));
	ZeroMemory(&m_AudioSetAllocator,sizeof(ALLOCATOR_PROPERTIES));
	ZeroMemory(&m_AudioGetAllocator,sizeof(ALLOCATOR_PROPERTIES));

	m_VideoPluginList.clear();
	m_AudioPluginList.clear();

	//�G���R�[�_�[�֘A
	m_pDirectShowEncoder = NULL;
	m_VideoCodecList.clear();
	m_AudioCodecList.clear();
	m_AudioFormatList.clear();

	m_bStartEncode = FALSE;

	m_nVideoCodecIndex = -1;
	m_nAudioCodecIndex = -1;
	m_nAudioCodecFormatIndex = -1;

	//ASF���C�^�֘A
	m_pDirectShowASFWriter = NULL;
	m_strAsfTempName = _T("");

	m_PullSinkList.clear();
	m_PushSinkList.clear();
	m_FileSinkList.clear();

	m_strTitle.Empty();
	m_strAuthor.Empty();
	m_strDescription.Empty();
	m_strRating.Empty();
	m_strCopyright.Empty();

	m_hNetworkCallbackWnd = NULL;
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CDirectShowAccess::~CDirectShowAccess(void)
{
	//Exit();
}

//////////////////////////////////////////////////
//DirectShow�����������܂�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::Init()
{
	m_pDirectShow = new CDirectShow();
	bool bret = m_pDirectShow->Init(AfxGetInstanceHandle());
	if (!bret) return FALSE;

	//�r�f�I�L���v�`���f�o�C�X���̗�
	m_strVideoNameList.clear();
	if (!m_pDirectShow->EnumVideoCaptureDeviceName(m_strVideoNameList))
		return FALSE;
	//�I�[�f�B�I�L���v�`���f�o�C�X���̗�
	m_strAudioNameList.clear();
	if (!m_pDirectShow->EnumAudioCaptureDeviceName(m_strAudioNameList))
		return FALSE;

	//�G���R�[�_�[�A�N�Z�b�T�̏�����
	m_pDirectShowEncoder = new CDirectShowEncoder();
	HRESULT hr = m_pDirectShowEncoder->Create();
	if (FAILED(hr))
		return FALSE;

	//�r�f�I�G���R�[�_�̎擾
	m_VideoCodecList.clear();
	hr = m_pDirectShowEncoder->EnumCodec(WMMEDIATYPE_Video,m_VideoCodecList);
	if (FAILED(hr))
		return FALSE;
	m_nVideoCodecIndex = -1;

	//�I�[�f�B�I�G���R�[�_�̎擾
	m_AudioCodecList.clear();
	hr = m_pDirectShowEncoder->EnumCodec(WMMEDIATYPE_Audio,m_AudioCodecList);
	if (FAILED(hr))
		return FALSE;
	m_nAudioCodecIndex = -1;
	m_nAudioCodecFormatIndex = -1;

	//�e��ݒ��ǂݍ���
	CSetting* pSetting   = ((CKTEApp*)AfxGetApp())->GetSetting();
	m_bVideoForceOff     = pSetting->m_bPreviewForceOff;		//�����I�Ƀv���r���[��؂邩�ǂ����H
	m_nVideoVMR          = pSetting->m_nPreviewVMR;			//�r�f�I�~�L�V���O�����_���̐ݒ�
	m_bVideoVMROverlay   = pSetting->m_bPreviewVMROverlay;	//�I�[�o�[���C
	m_bAudioVolumeEnable = pSetting->m_bEnableAudioPreview;	//���ʃ��[�^�[�̗L��

	//�v���t�@�C���̃��[�h(����)
	LoadProfileSetting();
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���̕ύX�ʒm
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ChangeProfileNotify()
{
	assert(m_pDirectShow);
	if (IsRunFilter())
		StopFilter();
	//�v���O�C���̍폜
	for (int i=0;i<(int)m_VideoPluginList.size();i++) {
		if (m_VideoPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_VideoPluginList[i].nFilterID));
		}
		m_VideoPluginList[i].nFilterID = -1;
	}
	m_VideoPluginList.clear();
	for (int i=0;i<(int)m_AudioPluginList.size();i++) {
		if (m_AudioPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_AudioPluginList[i].nFilterID));
		}
		m_AudioPluginList[i].nFilterID = -1;
	}
	m_AudioPluginList.clear();

	//�v���t�@�C���̃��[�h
	LoadProfileSetting();
	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C������̃��[�h
//TODO ���[�h�̂������ρH
//////////////////////////////////////////////////
BOOL CDirectShowAccess::LoadProfileSetting()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	//�r�f�I�v���O�C��
	m_VideoPluginList.clear();
	for (int i=0;i<(int)pProfile->m_VideoPluginList.size();i++)
	{
		if (pProfile->m_VideoPluginList[i].guid == GUID_NULL)
			continue;
		FilterPluginStruct fps;
		fps.guidFilter = pProfile->m_VideoPluginList[i].guid;
		fps.guidCategory = pProfile->m_VideoPluginList[i].guidCategory;
		if (fps.guidCategory == GUID_NULL)
			fps.bDMO = FALSE;
		else
			fps.bDMO = TRUE;
		fps.nFilterID = -1;	//���̎��_�ł̓t�B���^�񐶐�
		fps.strFilterName = pProfile->m_VideoPluginList[i].strFilterName;
		fps.bEnable = pProfile->m_VideoPluginList[i].bEnable;
		//fps.nInputPinIndex = -1;
		//fps.nOutputPinIndex = -1;
		//fps.strGUIDName = pProfile->m_VideoPluginList[i].strFilterName;
		m_VideoPluginList.push_back(fps);
	}
	//�I�[�f�B�I�v���O�C��
	m_AudioPluginList.clear();
	for (int i=0;i<(int)pProfile->m_AudioPluginList.size();i++)
	{
		if (pProfile->m_AudioPluginList[i].guid == GUID_NULL)
			continue;
		FilterPluginStruct fps;
		fps.guidFilter = pProfile->m_AudioPluginList[i].guid;
		fps.guidCategory = pProfile->m_AudioPluginList[i].guidCategory;
		if (fps.guidCategory == GUID_NULL)
			fps.bDMO = FALSE;
		else
			fps.bDMO = TRUE;
		fps.nFilterID = -1;
		fps.strFilterName = pProfile->m_AudioPluginList[i].strFilterName;
		fps.bEnable = pProfile->m_AudioPluginList[i].bEnable;
		//fps.nInputPinIndex = -1;
		//fps.nOutputPinIndex = -1;
		//fps.strGUIDName = pProfile->m_AudioPluginList[i].strFilterName;
		m_AudioPluginList.push_back(fps);
	}

	return TRUE;
}
//////////////////////////////////////////////////
//�v���t�@�C���ւ̃Z�[�u
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SaveProfileSetting()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	//�r�f�I�v���O�C���̏��
	pProfile->m_VideoPluginList.clear();
	for (int i=0;i<(int)m_VideoPluginList.size();i++)
	{
		IniPluginStruct ips;
		ips.guid = m_VideoPluginList[i].guidFilter;
		ips.guidCategory = m_VideoPluginList[i].guidCategory;
		ips.strFilterName = m_VideoPluginList[i].strFilterName;
		ips.bEnable = m_VideoPluginList[i].bEnable;
		if (ips.guid != GUID_NULL && ips.strFilterName.Compare(_T("")) != 0)
			pProfile->m_VideoPluginList.push_back(ips);
	}
	pProfile->m_nVideoPluginCount = (int)pProfile->m_VideoPluginList.size();
	//�I�[�f�B�I�v���O�C���̏��
	pProfile->m_AudioPluginList.clear();
	for (int i=0;i<(int)m_AudioPluginList.size();i++)
	{
		IniPluginStruct ips;
		ips.guid = m_AudioPluginList[i].guidFilter;
		ips.guidCategory = m_AudioPluginList[i].guidCategory;
		ips.strFilterName = m_AudioPluginList[i].strFilterName;
		ips.bEnable = m_AudioPluginList[i].bEnable;
		if (ips.guid != GUID_NULL && ips.strFilterName.Compare(_T("")) != 0)
			pProfile->m_AudioPluginList.push_back(ips);
	}
	pProfile->m_nAudioPluginCount = (int)pProfile->m_AudioPluginList.size();
	return TRUE;
}

//////////////////////////////////////////////////
//DirectShow���I�����܂�
//////////////////////////////////////////////////
void CDirectShowAccess::Exit()
{
	if (m_bStartEncode) {
		StopEncode(TRUE);
	}
	m_bStartEncode = FALSE;

	m_PullSinkList.clear();
	m_PushSinkList.clear();
	m_FileSinkList.clear();
	if (m_pDirectShowASFWriter) {
		m_pDirectShowASFWriter->Delete(m_pDirectShow->GetGraphBuilder());
		delete m_pDirectShowASFWriter;
		m_pDirectShowASFWriter = NULL;
	}

	m_VideoCodecList.clear();
	m_AudioCodecList.clear();
	m_AudioFormatList.clear();
	m_nVideoCodecIndex = -1;
	m_nAudioCodecIndex = -1;
	m_nAudioCodecFormatIndex = -1;
	if (m_pDirectShowEncoder) {
		m_pDirectShowEncoder->Delete();
		delete m_pDirectShowEncoder;
		m_pDirectShowEncoder = NULL;
	}

	m_strVideoNameList.clear();
	m_strVideoSettingList.clear();
	//m_VideoSettingList.clear();
	if (m_VideoMediaTypeList.size() != 0) {
		CDirectShowEtc::ReleaseMediaTypeList(m_VideoMediaTypeList);
		m_VideoMediaTypeList.clear();
	}
	if (m_VideoCapsList.size() != 0)
		m_VideoCapsList.clear();
	CDirectShowEtc::ReleaseMediaType(m_VideoFirstMediaType);
	CDirectShowEtc::ReleaseMediaType(m_VideoLastMediaType);
	
	m_nVideoIndex = -1;
	m_nVideoSettingIndex = -1;
	m_nVideoFilterID = -1;
	m_nVideoSmartTeeID = -1;
	m_nVideoColorSpaceID = -1;
	m_nVideoAVIDecompressorID = -1;
	m_nVideoOverlayMixerID = -1;
	m_nVideoFilterOutputPinIndex = -1;

	m_nCrossbar1ID = -1;
	m_nCrossbar2ID = -1;
	m_nTVAudioID = -1;
	m_nTVTunerID = -1;

	m_strAudioNameList.clear();
	m_strAudioSettingList.clear();
	if (m_AudioMediaTypeList.size() != 0) {
		CDirectShowEtc::ReleaseMediaTypeList(m_AudioMediaTypeList);
		m_AudioMediaTypeList.clear();
	}
	if (m_AudioCapsList.size() != 0)
		m_AudioCapsList.clear();
	CDirectShowEtc::ReleaseMediaType(m_AudioFirstMediaType);
	CDirectShowEtc::ReleaseMediaType(m_AudioLastMediaType);
	//ZeroMemory(&m_AudioSetAllocator,sizeof(ALLOCATOR_PROPERTIES));
	//ZeroMemory(&m_AudioGetAllocator,sizeof(ALLOCATOR_PROPERTIES));

	m_nAudioIndex = -1;
	m_nAudioSettingIndex = -1;
	m_nAudioSmartTeeID = -1;
	m_nAudioFilterOutputPinIndex = -1;
	//m_nAudioRenderID = -1;
	m_bVideoFindAudioPin = FALSE;
	m_bUseAudioVideoFilter = FALSE;

	m_nAudioNullFilterID = -1;
	if (m_pAudioVolumeFilter) {
		m_pAudioVolumeFilter->Delete(m_pDirectShow->GetGraphBuilder());
		delete m_pAudioVolumeFilter;
		m_pAudioVolumeFilter = NULL;
	}
	m_bAudioVolumeEnable = FALSE;

	//�v���O�C���̍폜
	for (int i=0;i<(int)m_VideoPluginList.size();i++) {
		if (m_VideoPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_VideoPluginList[i].nFilterID));
		}
		m_VideoPluginList[i].nFilterID = -1;
	}
	m_VideoPluginList.clear();
	for (int i=0;i<(int)m_AudioPluginList.size();i++) {
		if (m_AudioPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_AudioPluginList[i].nFilterID));
		}
		m_AudioPluginList[i].nFilterID = -1;
	}
	m_AudioPluginList.clear();

	//�v���O�C���̍폜
	for (int i=0;i<(int)m_VideoPluginList.size();i++) {
		if (m_VideoPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_VideoPluginList[i].nFilterID));
		}
		m_VideoPluginList[i].nFilterID = -1;
	}
	m_VideoPluginList.clear();
	for (int i=0;i<(int)m_AudioPluginList.size();i++) {
		if (m_AudioPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_AudioPluginList[i].nFilterID));
		}
		m_AudioPluginList[i].nFilterID = -1;
	}
	m_AudioPluginList.clear();

	m_hVideoWnd = NULL;
	m_bVideoShow = FALSE;
	m_nVideoVMR = 1;
	m_bVideoVMROverlay = FALSE;

	if (m_pDirectShow) {
		//m_pDirectShow->RemoveFromRot();
		m_pDirectShow->Exit();
		delete m_pDirectShow;
		m_pDirectShow = NULL;
	}
	m_bRunFilter = FALSE;
}
//////////////////////////////////////////////////
//�t�B���^�O���t�̎��s
//////////////////////////////////////////////////
BOOL CDirectShowAccess::RunFilter()
{
	if (!IsCanPreview()) return TRUE;
	assert(m_pDirectShow);

	HRESULT hr;
	
	//TRACE0("StartPreview\n");
	SaveGraphFile(_T("RunFilter.grf"));

	/*
	//��N���b�N�̐ݒ���s��
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	BOOL bUse = pProfile->m_bGraphClockEnable;
	BOOL bDefaultSyncSource = FALSE;
	BOOL bMakeSystemClock = FALSE;
	IBaseFilter* pFilter = NULL;
	switch (pProfile->m_nGraphClockSetting)
	{
	case 0:	//�����I��
		bDefaultSyncSource = TRUE;
		break;
	case 1:	//�V�X�e���N���b�N
		bMakeSystemClock = TRUE;
		break;
	case 2:	//�I�[�f�B�I�����_��(DirectSound)
		{
			if (bUse == FALSE)
				break;
			if (m_nAudioRenderID == -1) {	//�I�[�f�B�I�����_���̐V�K�쐬
				hr = m_pDirectShow->AddFilter(CLSID_DSoundRender,_T("DirectSound Renderer"),&m_nAudioRenderID);
				if (FAILED(hr)) {
					AfxMessageBox(_T("DirectSound�I�[�f�B�I�����_���̐����Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
					return FALSE;
				}
			}
			pFilter = m_pDirectShow->GetFilterByID(m_nAudioRenderID);
			assert(pFilter);
		}
		break;
	}
	{
		//��N���b�N�̐ݒ�Ɋւ��āA�s�v�ȏꍇ�ɉ����āA�I�[�f�B�I�����_���̍폜���s��
		if (m_nAudioRenderID != -1 && pFilter == NULL) {
			hr = m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nAudioRenderID));
			m_nAudioRenderID = -1;
		}
	}
	//��N���b�N�̐ݒ�
	hr = m_pDirectShow->SetGraphClock(bUse,bDefaultSyncSource,bMakeSystemClock,pFilter);
	if (FAILED(hr)) {
		AfxMessageBox(_T("��N���b�N�̐ݒ�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//�����̗L���̐ݒ���s��
	hr = m_pDirectShow->SyncStreamOffset(pProfile->m_bGraphSyncEnable,NULL);
	if (FAILED(hr)) {
		AfxMessageBox(_T("�����̐ݒ�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	*/
	//AfxGetApp()->m_pMainWnd->SendMessage(WM_CLOSE);	//�����I��

	//�t�B���^�̎��s
	//hr = m_pDirectShow->Pause();
	hr = m_pDirectShow->Run();
	if (FAILED(hr)) {
		CString strErrorMessage;
		TCHAR szErr[MAX_ERROR_TEXT_LEN];
		AMGetErrorText(hr,szErr,MAX_ERROR_TEXT_LEN);
		strErrorMessage.Format(_T("%s\r\nHRESULT=0x%08X\r\n%s"),_T("StartPreview Run."),hr,szErr);
		::AfxMessageBox(strErrorMessage,MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	/*
	if (hr == 0x800705AA) {
		AfxMessageBox(_T("�L���v�`���f�o�C�X�̊m�ۂɎ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	} else if (hr == 0x8007048F) {
		AfxMessageBox(_T("�f�o�C�X���ڑ�����Ă��܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	} else if (hr == 0x8007001F) {
		AfxMessageBox(_T("�V�X�e���ɐڑ����ꂽ�f�o�C�X���@�\���Ă��܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	} else if (FAILED(hr)){	//��O�G���[
	}
	*/
	m_bRunFilter = TRUE;
	return TRUE;
}
//////////////////////////////////////////////////
//�t�B���^�O���t�̒�~
//////////////////////////////////////////////////
BOOL CDirectShowAccess::StopFilter()
{
	assert(m_pDirectShow);
	//HRESULT hr = m_pDirectShow->Pause();
	HRESULT hr = m_pDirectShow->Stop();
	m_bRunFilter = FALSE;
	return (BOOL)SUCCEEDED(hr);
}

//////////////////////////////////////////////////
//�I�[�f�B�I��I������
//int nIndex �I�[�f�B�I�L���v�`���f�o�C�X�z��̃C���f�b�N�X
//BOOL bUseVideoPin	�ڑ��Ƀr�f�I�s�����g���ꍇ(���L���[�h�ֈڍs)
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectAudioIndex(int nIndex,BOOL bUseVideoPin)
{
	assert(m_pDirectShow);
	assert(nIndex >= 0);
	assert(nIndex < (int)m_strAudioNameList.size());
	HRESULT hr;

	//SaveGraphFile(_T("SelectAudioIndex(start).grf"));

	//���݂̃I�[�f�B�I�L���v�`���f�o�C�X���폜����
	if (IsUseAudioVideoFilter()) {	//���݂͋��L���[�h�ł���
		//�ؒf�݂̂Ńt�B���^���폜���Ȃ�
		hr = m_pDirectShow->DisconnectFilterPin(
			m_pDirectShow->GetFilterByID(m_nAudioFilterID),
			m_nAudioFilterOutputPinIndex);
	} else {						//���݂͒ʏ탂�[�h�ł���
		//�t�B���^�̐ؒf�ƍ폜
		if (m_nAudioFilterID != -1) {
			if (m_nAudioFilterID != m_nVideoFilterID) {
				//hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nAudioFilterID),TRUE,TRUE);
				m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nAudioFilterID));
				m_nAudioFilterID = -1;
			}
		}
	}

	//�ݒ�̏�����
	m_nAudioIndex = -1;
	m_nAudioSettingIndex = -1;
	m_nAudioFilterID = -1;
	m_nAudioFilterOutputPinIndex = -1;

	//�I�[�f�B�I�L���v�`���f�o�C�X��V�K�쐬
	int nAudioFilterID = -1;
	if (bUseVideoPin)
	{
		//���L���[�h�Ɉڍs
		if (m_nVideoFilterID == -1) {
			AfxMessageBox(_T("�I�[�f�B�I�s����L����r�f�I�f�o�C�X�����݂��܂���ł���"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
		//�r�f�I�ƃI�[�f�B�I�Ńt�B���^ID�̋��L���s���ăt�B���^�̐V�K�쐬�͖���
		nAudioFilterID = m_nVideoFilterID;
	} else {		
		//�ʏ탂�[�h�Ɉڍs (�I�[�f�B�I�L���v�`���f�o�C�X��V�K�쐬)
		if (!m_pDirectShow->AddAudioCaptureDeviceByName(m_strAudioNameList[nIndex],&nAudioFilterID)) {
			AfxMessageBox(_T("�I�[�f�B�I�L���v�`���f�o�C�X�̒ǉ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}

	//�o�̓s���ԍ��������Ŏ擾���Ă���
	int nOutputPinIndex = -1;
	vector<PinInfoStruct> PinInfoList;
	hr = m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(nAudioFilterID),
		PinInfoList,NULL,NULL);
	for (int i=0;i<(int)PinInfoList.size();i++) {
		if (PinInfoList[i].PinDir != PINDIR_OUTPUT)
			continue;
		//if (PinInfoList[i].bConnected == TRUE)
		//	continue;
		vector<AM_MEDIA_TYPE> MediaTypeList;
		m_pDirectShow->EnumMediaType(
			m_pDirectShow->GetFilterByID(nAudioFilterID),
			i,MediaTypeList);
		if (MediaTypeList.size() == 0)
			continue;
		if (MediaTypeList[0].majortype == MEDIATYPE_Audio)
		{
			if (nOutputPinIndex == -1)
				nOutputPinIndex = i;
		}
		CDirectShowEtc::ReleaseMediaTypeList(MediaTypeList);
	}
	if (nOutputPinIndex == -1) //�K������I�[�f�B�I�o�̓s������������
		return FALSE;

	//TODO �����͒����K�v�����邩������Ȃ�
	/*
	int nID[4];
	hr = m_pDirectShow->AddWDMFilter(
		m_pDirectShow->GetFilterByID(nAudioFilterID),FALSE,
		&nID[0],&nID[1],&nID[2],&nID[3]);
		*/

	//�I�[�f�B�I�L���v�`���̃f�o�C�X���\���擾
	if (m_AudioMediaTypeList.size() != 0) {
		CDirectShowEtc::ReleaseMediaTypeList(m_AudioMediaTypeList);
		m_AudioMediaTypeList.clear();
	}
	m_AudioCapsList.clear();
	hr = m_pDirectShow->EnumAudioCaptureMediaType(
		m_pDirectShow->GetFilterByID(nAudioFilterID),
		nOutputPinIndex,m_AudioMediaTypeList,m_AudioCapsList);

	if (FAILED(hr) && bUseVideoPin)	//���L���[�h�̏ꍇ�A�L���v�`���f�o�C�X�łȂ��̂Ńf�o�C�X���\���擾�ł��Ȃ�
	{
		//���f�B�A�^�C�v�݂̂����擾
		m_pDirectShow->EnumMediaType(
			m_pDirectShow->GetFilterByID(nAudioFilterID),
			nOutputPinIndex,m_AudioMediaTypeList);
		//�_�~�[�̃I�[�f�B�I�������X�g�𐶐����Ă���
		CDirectShowEtc::MakeDummyAudioCapsList(m_AudioMediaTypeList,m_AudioCapsList);
	}

	//�ݒ荀�ڃ��X�g���擾���A�I�[�f�B�I��񕶎���𐶐�����
	if (m_strAudioSettingList.size() != 0)
		m_strAudioSettingList.clear();
	CDirectShowEtc::MakeAudioMediaString(m_AudioMediaTypeList,m_strAudioSettingList);

	m_nAudioIndex = nIndex;							//�I�������I�[�f�B�I�ԍ�
	m_nAudioSettingIndex = -1;						
	m_nAudioFilterOutputPinIndex = nOutputPinIndex;	//�I�������I�[�f�B�I�̏o�̓s���C���f�b�N�X
	m_nAudioFilterID = nAudioFilterID;				//�I�[�f�B�I�t�B���^��ID
	m_bUseAudioVideoFilter = bUseVideoPin;			//���L���[�h�̗L���E����
	//SaveGraphFile(_T("SelectAudioIndex(end).grf"));
	return TRUE;
}
//////////////////////////////////////////////////
//�I�[�f�B�I�L���v�`���ݒ��K�p����
//�G���R�[�h�J�n�̏ꍇ�̃t�B���^�ڑ����@
//TODO ����N������2��Ă΂�Ă���
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectAudioSettingIndex(int nSettingIndex,AM_MEDIA_TYPE *pamt)
{
	assert(m_pDirectShow);
	assert(nSettingIndex >= 0);
	assert(nSettingIndex < (int)m_strAudioSettingList.size());
	assert(m_nAudioFilterOutputPinIndex != -1);
	HRESULT hr = NOERROR;

	//�t�B���^�̐ؒf
	if (m_nAudioFilterID != -1) {
		//Audio�o�̓s���݂̂�ؒf����
		hr = m_pDirectShow->DisconnectFilterPin(
			m_pDirectShow->GetFilterByID(m_nAudioFilterID),m_nAudioFilterOutputPinIndex);
	}
	/*
	//�I�[�f�B�I�v���O�C����S�폜����
	for (int i=0;i<(int)m_AudioPluginList.size();i++)
	{
		if (m_AudioPluginList[i].nFilterID != -1)
		{
			m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_AudioPluginList[i].nFilterID));
			m_AudioPluginList[i].nFilterID = -1;
		}
	}*/
	/*
	//�I�[�f�B�I����t�B���^���폜����
	if (m_nAudioSmartTeeID != -1)
	{
		//hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),TRUE,TRUE);
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID));
		m_nAudioSmartTeeID = -1;
	}*/
	/*
	//�I�[�f�B�I�p��NULL�����_�����폜����
	if (m_nAudioNullFilterID != -1) {
		hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nAudioNullFilterID),TRUE,TRUE);
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nAudioNullFilterID));
		m_nAudioNullFilterID = -1;
	}*/

	//�ݒ�̓K�p
	if (pamt)		//���f�B�A�^�C�v�w��(pamt)
	{
		hr = m_pDirectShow->GetSetCaptureMediaType(
			m_pDirectShow->GetFilterByID(m_nAudioFilterID),
			m_nAudioFilterOutputPinIndex,TRUE,pamt);
	} else {		//���f�B�A�^�C�v�w��Ȃ�(�C���f�b�N�X�ɂ��ڑ�)
		hr = m_pDirectShow->GetSetCaptureMediaType(
			m_pDirectShow->GetFilterByID(m_nAudioFilterID),
			m_nAudioFilterOutputPinIndex,TRUE,&m_AudioMediaTypeList[nSettingIndex]);
	}

	//�I�[�f�B�I�A���P�[�^�[�̐ݒ�
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	if (pProfile->m_bAudioBufferEnable && pProfile->m_nAudioBufferTime != 0 && pProfile->m_nAudioBufferCount != 0)
	{
		m_AudioSetAllocator.cBuffers = pProfile->m_nAudioBufferCount;
		WAVEFORMATEX* WaveFormatEx;
		if (pamt) {
			WaveFormatEx = (WAVEFORMATEX*)pamt->pbFormat;
		} else {
			WaveFormatEx = (WAVEFORMATEX*)m_AudioMediaTypeList[nSettingIndex].pbFormat;
		}
		assert(WaveFormatEx);
		m_AudioSetAllocator.cbBuffer = WaveFormatEx->nAvgBytesPerSec *  pProfile->m_nAudioBufferTime / 1000;
		m_AudioSetAllocator.cbAlign  = -1;
		m_AudioSetAllocator.cbPrefix = -1;
	} else {
		m_AudioSetAllocator.cBuffers = -1;
		m_AudioSetAllocator.cbBuffer = -1;
		m_AudioSetAllocator.cbAlign  = -1;
		m_AudioSetAllocator.cbPrefix = -1;
	}
	hr = m_pDirectShow->SetGetAllocatorBuffer(
		m_pDirectShow->GetFilterByID(m_nAudioFilterID),
		m_nAudioFilterOutputPinIndex,TRUE,m_AudioSetAllocator);
	//if (FAILED(hr))
	//	return FALSE;

	//�I�[�f�B�I����t�B���^��V�K�쐬
	//int nAudioSmartTeeID = -1;
	if (m_nAudioSmartTeeID == -1)
	{
		hr = m_pDirectShow->AddFilter(CLSID_InfTee,_T("Audio Infinite Tee Filter"),&m_nAudioSmartTeeID);
		//hr = m_pDirectShow->AddFilter(CLSID_SmartTee,_T("Audio Smart Tee Filter"),&m_nAudioSmartTeeID);
		assert(m_nAudioSmartTeeID != -1);
		if (FAILED(hr)) {
			AfxMessageBox(_T("�I�[�f�B�I����t�B���^�̍쐬�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	} else {
		//�s���̐ؒf
		hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),TRUE,TRUE);
	}
	//hr = m_pDirectShow->AddFilter(CLSID_InfTee,_T("Audio Infinite Pin Tee Filter"),&nAudioSmartTeeID);

	//�s�������w�肵�Đڑ����s��
	vector<PinInfoStruct> PinInfoList;
	hr = m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(m_nAudioFilterID),
		PinInfoList,NULL,NULL);
	//�t�B���^�̐ڑ�(�I�[�f�B�I�L���v�`�����v���O�C�����X�g���I�[�f�B�I����t�B���^)
	if (!ChainPluginList(FALSE,m_nAudioFilterID,
		PinInfoList[m_nAudioFilterOutputPinIndex].strPinName.c_str(),
		m_nAudioSmartTeeID,NULL)) {
		return FALSE;
	}

	//�t�B���^�̍č\�z(�����܂�)
	if (!CheckConnectFilter(FALSE)) {
		AfxMessageBox(_T("�I�[�f�B�I�t�B���^�̐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//SaveGraphFile(_T("connect_audio_conplite.grf"));

	//���݂̐ڑ����f�B�A�^�C�v���擾����
	CDirectShowEtc::ReleaseMediaType(m_AudioFirstMediaType);
	CDirectShowEtc::ReleaseMediaType(m_AudioLastMediaType);
	ZeroMemory(&m_AudioFirstMediaType,sizeof(AM_MEDIA_TYPE));
	ZeroMemory(&m_AudioLastMediaType,sizeof(AM_MEDIA_TYPE));
	hr = m_pDirectShow->GetConnectionMediaType(
		m_pDirectShow->GetFilterByID(m_nAudioFilterID),
		m_nAudioFilterOutputPinIndex,m_AudioFirstMediaType);
	hr = m_pDirectShow->GetConnectionMediaType(
		m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),
		0,m_AudioLastMediaType);
	//CDirectShowEtc::DumpMediaType(&m_CurrentAudioMediaType,_T("CurrentAudioMediaType.dat"));

	//���݂̃A���P�[�^�[�T�C�Y���擾����
	hr = m_pDirectShow->SetGetAllocatorBuffer(
		m_pDirectShow->GetFilterByID(m_nAudioFilterID),
		m_nAudioFilterOutputPinIndex,FALSE,m_AudioGetAllocator);
	/*
	//�A���P�[�^�[�̔�r
	BOOL bSuccess = TRUE;
	if (SUCCEEDED(hr))
	{
		//�o�b�t�@
		if (pProfile->m_bAudioBufferEnable && pProfile->m_nAudioBufferTime != 0 && pProfile->m_nAudioBufferCount != 0)
		{
			//�v���T�C�Y�Ɠ����T�C�Y�������o�b�t�@���m�ۂł������ǂ���
			if (m_AudioSetAllocator.cbBuffer != m_AudioGetAllocator.cbBuffer || 
				(m_AudioSetAllocator.cBuffers != -1 && m_AudioSetAllocator.cBuffers != m_AudioGetAllocator.cBuffers)) {
					//�o�b�t�@���m�ۂł��Ă��Ȃ�
					if (m_AudioGetAllocator.cbBuffer == 0 || m_AudioGetAllocator.cBuffers == 0)
						bSuccess = FALSE;
			}
		}
	} else {
		bSuccess = FALSE;
	}
	if (!bSuccess)
		AfxMessageBox(_T("�I�[�f�B�I�o�b�t�@�̊m�ۂɎ��s���܂���\n�o�b�t�@�T�C�Y�̊m�F�����Ă�������"),MB_OK|MB_ICONINFORMATION);
	*/

	//�I�[�f�B�I���ʃt�B���^�ɐڑ�����
	ConnectAudioVolume();

	//m_nAudioSmartTeeID = nAudioSmartTeeID;		//�I�[�f�B�I����t�B���^��ID
	//m_nAudioNullFilterID = nAudioNullFilterID;	//NULL�t�B���^��ID
	m_nAudioSettingIndex = nSettingIndex;		//�I�������I�[�f�B�I�ݒ�ԍ�
	//SaveGraphFile(_T("SelectAudioSettingIndex().grf"));
	return TRUE;
}

//////////////////////////////////////////////////
//�I�[�f�B�I���ʃt�B���^�ɐڑ��E�ؒf����
//BOOL bConnect �ڑ����邩�ǂ��� (FALSE�Ȃ�ؒf)
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ConnectAudioVolume()
{
	HRESULT hr = NOERROR;

	//�I�[�f�B�I�p��NULL�����_����V�K�쐬
	if (m_nAudioNullFilterID == -1)
	{
		GUID guidNullRenderer = {0xC1F400A4,0x3F08,0x11D3,{0x9F,0x0B,0x00,0x60,0x08,0x03,0x9E,0x37}};//CLSID_NullRenderer
		hr = m_pDirectShow->AddFilter(guidNullRenderer,_T("Audio Volume Null Renderer"),&m_nAudioNullFilterID);
		if (FAILED(hr)) {
			AfxMessageBox(_T("�I�[�f�B�I�{�����[��NULL�����_���̒ǉ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	} else {
		hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nAudioNullFilterID),TRUE,TRUE);
	}
	if (!m_pAudioVolumeFilter) {
		m_pAudioVolumeFilter = new CDirectShowAudioVolume();
		hr = m_pAudioVolumeFilter->Create(m_pDirectShow->GetGraphBuilder());
		if (FAILED(hr)) {
			AfxMessageBox(_T("�I�[�f�B�I�{�����[���T���v���O���o�̒ǉ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	} else {
		hr = m_pDirectShow->DisconnectFilter(m_pAudioVolumeFilter->GetFilter(),TRUE,TRUE);
	}

	LPCWSTR lpszOutputPin  = NULL;
	vector<PinInfoStruct> PinInfo;
	m_pDirectShow->GetFilterInfo(m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),PinInfo,NULL,NULL);
	for (int i=0;i<(int)PinInfo.size();i++) {
		if (PinInfo[i].PinDir != PINDIR_OUTPUT)
			continue;
		if (PinInfo[i].bConnected == TRUE)
			continue;
		lpszOutputPin = PinInfo[i].strPinName.c_str();
		break;
	}
	hr =m_pDirectShow->ConnectFilterDirect(
		m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),
		m_pAudioVolumeFilter->GetFilter(),
		NULL,lpszOutputPin,NULL);

	if (FAILED(hr)) {
		AfxMessageBox(_T("�I�[�f�B�I����t�B���^�ƃI�[�f�B�I�{�����[���T���v���O���o�̐ڑ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	hr =m_pDirectShow->ConnectFilterDirect(
		m_pAudioVolumeFilter->GetFilter(),
		m_pDirectShow->GetFilterByID(m_nAudioNullFilterID),
		NULL,NULL,NULL);
	if (FAILED(hr)) {
		AfxMessageBox(_T("�I�[�f�B�I�{�����[���T���v���O���o�ƃI�[�f�B�I�{�����[��NULL�����_���̐ڑ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//�ڑ���Ƀ��f�B�A�^�C�v�̊m�F������
	hr = m_pAudioVolumeFilter->GetConnectionMediaType();
	
	//�L���E�����̐؂�ւ�
	m_pAudioVolumeFilter->EnableCapture(m_bAudioVolumeEnable);
	return TRUE;
}

//////////////////////////////////////////////////
//�r�f�I��I�����Đݒ荀�ڂ��擾����
//�r�f�I�̐؂�ւ�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectVideoIndex(int nIndex)
{
	HRESULT hr = NOERROR;
	assert(m_pDirectShow);
	assert(nIndex >= 0);
	assert(nIndex < (int)m_strVideoNameList.size());

	//���ݑI�𒆂̃r�f�I�̃r�f�I�L���v�`���f�o�C�X���폜����
	if (m_nVideoFilterID != -1) {
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nVideoFilterID));
		m_nVideoFilterID = -1;
		m_nVideoFilterOutputPinIndex = -1;
	}
	m_nVideoIndex = -1;
	m_nVideoSettingIndex = -1;
	m_bVideoFindAudioPin = FALSE;
	m_bUseAudioVideoFilter = FALSE;
	//�r�f�I�L���v�`���ɕt������WDM�t�B���^�[�̍폜
	if (m_nCrossbar1ID != -1)
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nCrossbar1ID));
	m_nCrossbar1ID = -1;
	if (m_nCrossbar2ID != -1)
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nCrossbar2ID));
	m_nCrossbar2ID = -1;
	if (m_nTVAudioID != -1)
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nTVAudioID));
	m_nTVAudioID = -1;
	if (m_nTVTunerID != -1)
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nTVTunerID));
	m_nTVTunerID = -1;

	//�r�f�I�L���v�`���f�o�C�X�ɒǉ�
	int nVideoFilterID = -1;
	if (!m_pDirectShow->AddVideoCaptureDeviceByName(m_strVideoNameList[nIndex],&nVideoFilterID))
		return FALSE;

	//WDM�h���C�o�t�B���^�̍쐬�ƒǉ�
	int nCrossbar1ID = -1;
	int nCrossbar2ID = -1;
	int nTVAudioID = -1;
	int nTVTunerID = -1;
	hr = m_pDirectShow->AddWDMFilter(
		m_pDirectShow->GetFilterByID(nVideoFilterID),TRUE,
		&nCrossbar1ID,&nCrossbar2ID,&nTVAudioID,&nTVTunerID);

	//�r�f�I�L���v�`���f�o�C�X�̏o�̓s�����擾����
	//�����ɃI�[�f�B�I�s�������邩�ۂ������ׂ�
	vector<PinInfoStruct> PinInfoList;
	int nOutputPinIndex = -1;
	BOOL bVideoFindAudioPin = FALSE;
	hr = m_pDirectShow->GetFilterInfo(m_pDirectShow->GetFilterByID(nVideoFilterID),PinInfoList,NULL,NULL);
	for (int i=0;i<(int)PinInfoList.size();i++) {
		if (PinInfoList[i].PinDir != PINDIR_OUTPUT)
			continue;
		//if (PinInfoList[i].bConnected == TRUE)
		//	continue;
		vector<AM_MEDIA_TYPE> MediaTypeList;
		m_pDirectShow->EnumMediaType(
			m_pDirectShow->GetFilterByID(nVideoFilterID),
			i,MediaTypeList);
		if (MediaTypeList.size() == 0)
			continue;
		if (MediaTypeList[0].majortype == MEDIATYPE_Video)
		{
			if (nOutputPinIndex == -1)
				nOutputPinIndex = i;
			CDirectShowEtc::ReleaseMediaTypeList(MediaTypeList);
			continue;
		} else if (MediaTypeList[0].majortype == MEDIATYPE_Audio) {
			bVideoFindAudioPin = TRUE;	//�r�f�I�f�o�C�X�ɃI�[�f�B�I�s�������݂���
		}
		CDirectShowEtc::ReleaseMediaTypeList(MediaTypeList);
	}
	if (nOutputPinIndex == -1)	//�K������r�f�I�o�̓s������������
		return FALSE;

	//�r�f�I�̐ݒ�̏������Ɛݒ�z��̎擾
	if (m_VideoMediaTypeList.size() != 0) {
		CDirectShowEtc::ReleaseMediaTypeList(m_VideoMediaTypeList);
		m_VideoMediaTypeList.clear();
	}
	if (m_VideoCapsList.size() != 0)
		m_VideoCapsList.clear();
	hr = m_pDirectShow->EnumVideoCaptureMediaType(m_pDirectShow->GetFilterByID(nVideoFilterID),
		nOutputPinIndex,m_VideoMediaTypeList,m_VideoCapsList);

	//�ݒ荀�ڃ��X�g���擾���A�r�f�I�ݒ��񕶎���𐶐�����
	if (m_strVideoSettingList.size() != 0)
		m_strVideoSettingList.clear();
	CDirectShowEtc::MakeVideoMediaString(m_VideoMediaTypeList,m_strVideoSettingList);

	//�ݒ�������o�ϐ��ɏ����o��
	m_nVideoIndex = nIndex;					//�I�������r�f�I�ԍ�
	m_nVideoSettingIndex = -1;
	m_nVideoFilterID = nVideoFilterID;		//�r�f�I�t�B���^��ID
	m_nVideoFilterOutputPinIndex = nOutputPinIndex;	//�I�������r�f�I�̏o�̓s���C���f�b�N�X
	m_bVideoFindAudioPin = bVideoFindAudioPin;	//�I�[�f�B�I�s���̗L��
	m_nCrossbar1ID = nCrossbar1ID;				//��1�N���X�o�[�̃t�B���^��ID
	m_nCrossbar2ID = nCrossbar2ID;				//��2�N���X�o�[�̃t�B���^��ID
	m_nTVAudioID   = nTVAudioID;				//TV�I�[�f�B�I�̃t�B���^��ID
	m_nTVTunerID   = nTVTunerID;				//TV�`���[�i�[�̃t�B���^��ID

	//SaveGraphFile(_T("SelectVideoIndex().grf"));
	return TRUE;
}
//////////////////////////////////////////////////
//�r�f�I�L���v�`���ݒ��K�p����
//�G���R�[�h�J�n�̏ꍇ�̃t�B���^�ڑ����@
//TODO �Ăяo��2��
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectVideoSettingIndex(int nSettingIndex,AM_MEDIA_TYPE *pamt)
{
	assert(m_pDirectShow);
	assert(nSettingIndex >= 0);
	assert(nSettingIndex < (int)m_strVideoSettingList.size());
	assert(m_nVideoFilterID != -1);
	assert(m_nVideoFilterOutputPinIndex != -1);

	HRESULT hr = NOERROR;

	//SaveGraphFile(_T("SelectVideoSettingIndex(0).grf"));

	//�t�B���^�̐ؒf
	//if (m_pDirectShow->GetVideoRenderer() != NULL)
	//	m_pDirectShow->DisconnectFilter(m_pDirectShow->GetVideoRenderer(),TRUE,TRUE);
	//if (m_nVideoConvertID != -1)
	//	m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nVideoConvertID),TRUE,TRUE);
	//if (m_nVideoSmartTeeID != -1)
	//	m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),TRUE,TRUE);
	if (m_nVideoFilterID != -1){	//���̐ؒf�͉����̂� 
		//���g�̃r�f�I�s���݂̂�ؒf����
		if (m_nVideoFilterOutputPinIndex != -1) {
			m_pDirectShow->DisconnectFilterPin(m_pDirectShow->GetFilterByID(m_nVideoFilterID),
				m_nVideoFilterOutputPinIndex);
		}
	}

	//�r�f�I����t�B���^�̍폜
	/*
	if (m_nVideoSmartTeeID != -1) {
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID));
		m_nVideoSmartTeeID = -1;
	}*/

	//�ݒ�̓K�p���@��AM_MEDIA_TYPE���g��
	if (pamt) {	//���f�B�A�^�C�v�w�肠��(pamt)
		hr = m_pDirectShow->GetSetCaptureMediaType(
			m_pDirectShow->GetFilterByID(m_nVideoFilterID),
			m_nVideoFilterOutputPinIndex,TRUE,pamt);
	} else {	//���f�B�A�^�C�v�w��Ȃ�(�C���f�b�N�X�ɂ��ڑ�)
		hr = m_pDirectShow->GetSetCaptureMediaType(
			m_pDirectShow->GetFilterByID(m_nVideoFilterID),
			m_nVideoFilterOutputPinIndex,TRUE,&m_VideoMediaTypeList[nSettingIndex]);
	}

	//�s�������擾
	vector<PinInfoStruct> PinInfoList;
	hr = m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(m_nVideoFilterID),
		PinInfoList,NULL,NULL);

	//�r�f�I����t�B���^�̍쐬 (�����͖���Đ�������悤�ɕύX����)
	if (m_nVideoSmartTeeID != -1) {
		hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),TRUE,TRUE);
		hr = m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID));
		m_nVideoSmartTeeID = -1;
	}
	hr = m_pDirectShow->AddFilter(CLSID_InfTee,_T("Video Infinite Tee Filter"),&m_nVideoSmartTeeID);
	//hr = m_pDirectShow->AddFilter(CLSID_SmartTee,_T("Video Smart Tee Filter"),&m_nVideoSmartTeeID);
	assert(m_nVideoSmartTeeID != -1);
	if (FAILED(hr))
	{
		AfxMessageBox(_T("�r�f�I����t�B���^�̍쐬�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//���̒i�K�ł̓t�B���^�͂�������
	//SaveGraphFile(_T("SelectVideoSettingIndex(5).grf"));

	//�t�B���^�̐ڑ�(�r�f�I�L���v�`�����v���O�C�����X�g���r�f�I����t�B���^)
	if (!ChainPluginList(TRUE,m_nVideoFilterID,
		PinInfoList[m_nVideoFilterOutputPinIndex].strPinName.c_str(),
						m_nVideoSmartTeeID,NULL))
	{
		AfxMessageBox(_T("�r�f�I�t�B���^�̐ڑ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//���̒i�K�ł͎��s���Ă���(�����ȃt�B���^�O���t)
	//SaveGraphFile(_T("SelectVideoSettingIndex(6).grf"));

	//���݂̃r�f�I���f�B�A�^�C�v���擾���� (�r�f�I�L���v�`���̐ڑ�)
	hr = CDirectShowEtc::ReleaseMediaType(m_VideoFirstMediaType);
	hr = CDirectShowEtc::ReleaseMediaType(m_VideoLastMediaType);
	ZeroMemory(&m_VideoFirstMediaType,sizeof(AM_MEDIA_TYPE));
	ZeroMemory(&m_VideoLastMediaType,sizeof(AM_MEDIA_TYPE));
	hr = m_pDirectShow->GetConnectionMediaType(
		m_pDirectShow->GetFilterByID(m_nVideoFilterID),
		m_nVideoFilterOutputPinIndex,m_VideoFirstMediaType);
	hr = m_pDirectShow->GetConnectionMediaType(
		m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),
		0,m_VideoLastMediaType);
	//�ڑ����f�B�A�^�C�v��Dump
	//CDirectShowEtc::DumpMediaType(&m_VideoFirstMediaType,_T("DumpVideoFirst.dat"));
	//CDirectShowEtc::DumpMediaType(&m_VideoLastMediaType,_T("DumpVideoLast.dat"));

	//�r�f�I�t�B���^�̐ڑ�
	hr = RenderPreview();
	if (FAILED(hr)) {
		AfxMessageBox(_T("�r�f�I�t�B���^�̐ڑ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}
	//�r�f�I�n���h���̐ݒ�
	if (m_hVideoWnd != NULL)
		SetPreviewWindow(m_hVideoWnd);

	//�t�B���^�̐ڑ�(�����܂�)
	if (!CheckConnectFilter(TRUE))
	{
		AfxMessageBox(_T("�r�f�I�t�B���^�̐ڑ��`�F�b�N�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//SaveGraphFile(_T("SelectVideoIndexSetting(end).grf"));

	m_nVideoSettingIndex = nSettingIndex;			//�I�������r�f�I�ݒ�ԍ�
	return TRUE;
}



//////////////////////////////////////////////////
//�v���r���[�̐ڑ��̎��s
//////////////////////////////////////////////////
HRESULT CDirectShowAccess::RenderPreview()
{
	if (m_bVideoForceOff == TRUE)	//������\�����[�h�̏ꍇ
		return S_OK;
	HRESULT hr  = NOERROR;

	//�r�f�I�����_�������݂��Ȃ��ꍇ�͍쐬
	if (m_pDirectShow->GetVideoRenderer() == NULL) {
		if (!m_pDirectShow->InitVideoRenderer(m_nVideoVMR,m_bVideoVMROverlay))
			return E_FAIL;
	} else {
		hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetVideoRenderer(),TRUE,TRUE);
	}

	int nLastFilterID = m_nVideoSmartTeeID;		//�Ō���̃t�B���^ID
	const wchar_t* lpszOutputPin;

	//�󂫂̗L������t�B���^�o�̓s��������
	vector<PinInfoStruct> PinInfo;
	m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),
		PinInfo,NULL,NULL);
	for (int i=0;i<(int)PinInfo.size();i++) {
		if (PinInfo[i].PinDir != PINDIR_OUTPUT)
			continue;
		if (PinInfo[i].bConnected == TRUE)
			continue;
		lpszOutputPin = PinInfo[i].strPinName.c_str();
		break;
	}

	//���ڐڑ������s
	hr = RenderPreviewSubRoutine1(nLastFilterID,lpszOutputPin);
	if (SUCCEEDED(hr)) {
		RenderPreviewSubRoutine2();
		return hr;
	}

	//�ڑ����̎擾
	BOOL bIsVideoInfo2;		//VideoInfoHeader2���ǂ���
	BOOL bRGB;				//RGB�n���ǂ��� (FALSE�Ȃ�YUV�n)
	if (m_VideoLastMediaType.formattype == FORMAT_VideoInfo) {
		bIsVideoInfo2 = FALSE;
		VIDEOINFOHEADER* pVideoInfoHeader = (VIDEOINFOHEADER *)m_VideoLastMediaType.pbFormat;
		if (pVideoInfoHeader->bmiHeader.biCompression == 0)
			bRGB = TRUE;
		else
			bRGB = FALSE;
	} else if (m_VideoLastMediaType.formattype == FORMAT_VideoInfo2){
		bIsVideoInfo2 = TRUE;
		VIDEOINFOHEADER2* pVideoInfoHeader2 = (VIDEOINFOHEADER2*)m_VideoLastMediaType.pbFormat;
		if (pVideoInfoHeader2->bmiHeader.biCompression == 0)
			bRGB = TRUE;
		else
			bRGB = FALSE;
	}

	//VideoInfoHeader2
	if (bIsVideoInfo2 == TRUE)
	{
		//�I�[�o�[���C�~�L�T������
		if (m_nVideoOverlayMixerID == -1) {
			hr = m_pDirectShow->AddFilter(CLSID_OverlayMixer,_T("Overlay Mixer"),&m_nVideoOverlayMixerID);
			assert(m_nVideoOverlayMixerID != -1);
		} else {
			hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nVideoOverlayMixerID),TRUE,TRUE);
		}
		hr = m_pDirectShow->ConnectFilterDirect(
			m_pDirectShow->GetFilterByID(nLastFilterID),
			m_pDirectShow->GetFilterByID(m_nVideoOverlayMixerID),
			NULL,lpszOutputPin,NULL);
		if (SUCCEEDED(hr)) {
			nLastFilterID = m_nVideoOverlayMixerID;
			lpszOutputPin = NULL;
			//���ڐڑ������s
			hr = RenderPreviewSubRoutine1(nLastFilterID,lpszOutputPin);
			if (SUCCEEDED(hr)) {
				RenderPreviewSubRoutine2();
				return S_OK;
			}
		}
	}

	//YUV�\�[�X�̏ꍇ
	if (bRGB == FALSE)
	{
		//AVI Decompressor������
		if (m_nVideoAVIDecompressorID == -1) {
			hr = m_pDirectShow->AddFilter(CLSID_AVIDec,_T("AVI Decompressor"),&m_nVideoAVIDecompressorID);
			assert(m_nVideoAVIDecompressorID != -1);
		} else {
			hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nVideoAVIDecompressorID),TRUE,TRUE);
		}
		hr = m_pDirectShow->ConnectFilterDirect(
			m_pDirectShow->GetFilterByID(nLastFilterID),
			m_pDirectShow->GetFilterByID(m_nVideoAVIDecompressorID),
			NULL,lpszOutputPin,NULL);
		if (SUCCEEDED(hr)) {
			nLastFilterID = m_nVideoAVIDecompressorID;
			lpszOutputPin = NULL;
			//���ڐڑ������s
			hr = RenderPreviewSubRoutine1(nLastFilterID,lpszOutputPin);
			if (SUCCEEDED(hr)) {
				RenderPreviewSubRoutine2();
				return S_OK;
			}
		}
	}

	//�F��ԕϊ����K�v�ȏꍇ�A�J���[�X�y�[�X�R���o�[�^�[������
	if (m_nVideoColorSpaceID == -1) {
		hr = m_pDirectShow->AddFilter(CLSID_Colour,_T("Color Space Converter"),&m_nVideoColorSpaceID);
		assert(m_nVideoColorSpaceID != -1);
	} else {
		hr = m_pDirectShow->DisconnectFilter(m_pDirectShow->GetFilterByID(m_nVideoColorSpaceID),TRUE,TRUE);
	}
	hr = m_pDirectShow->ConnectFilterDirect(
		m_pDirectShow->GetFilterByID(nLastFilterID),
		m_pDirectShow->GetFilterByID(m_nVideoColorSpaceID),
		NULL,lpszOutputPin,NULL);
	if (SUCCEEDED(hr)) {
		nLastFilterID = m_nVideoColorSpaceID;
		lpszOutputPin = NULL;
		//���ڐڑ������s
		hr = RenderPreviewSubRoutine1(nLastFilterID,lpszOutputPin);
		if (SUCCEEDED(hr)) {
			RenderPreviewSubRoutine2();
			return S_OK;
		}
	}
	RenderPreviewSubRoutine2();
	return E_FAIL;
}
//////////////////////////////////////////////////
//�v���r���[�̐ڑ��̎��s�Ŏg���T�u���[�`�� (�r�f�I�Ƃ̐ڑ������݂�)
HRESULT CDirectShowAccess::RenderPreviewSubRoutine1(const int nLastFilterID,const wchar_t* lpszOutputPin)
{
	return m_pDirectShow->ConnectFilterDirect(
		m_pDirectShow->GetFilterByID(nLastFilterID),
		m_pDirectShow->GetVideoRenderer(),
		NULL,lpszOutputPin,NULL);
}
//////////////////////////////////////////////////
//�v���r���[�̐ڑ��̎��s�Ŏg���T�u���[�`�� (���g�p�̃t�B���^�̍폜������)
void CDirectShowAccess::RenderPreviewSubRoutine2()
{
	int nInputPinCount = 0;
	int nOutputPinCount = 0;
	HRESULT hr = NOERROR;
	if (m_nVideoOverlayMixerID != -1) {
		CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nVideoOverlayMixerID),&nInputPinCount,&nOutputPinCount);
		if (nInputPinCount == 0 && nOutputPinCount == 0) {
			hr = m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nVideoOverlayMixerID));
			m_nVideoOverlayMixerID = -1;
		}
	}
	nInputPinCount = 0;
	nOutputPinCount = 0;
	if (m_nVideoAVIDecompressorID != -1) {
		CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nVideoAVIDecompressorID),&nInputPinCount,&nOutputPinCount);
		if (nInputPinCount == 0 && nOutputPinCount == 0) {
			hr = m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nVideoAVIDecompressorID));
			m_nVideoAVIDecompressorID = -1;
		}
	}
	nInputPinCount = 0;
	nOutputPinCount = 0;
	if (m_nVideoColorSpaceID != -1) {
		CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nVideoColorSpaceID),&nInputPinCount,&nOutputPinCount);
		if (nInputPinCount == 0 && nOutputPinCount == 0) {
			hr = m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(m_nVideoColorSpaceID));
			m_nVideoColorSpaceID = -1;
		}
	}
}

//////////////////////////////////////////////////
//�t�B���^�̐ڑ��󋵂��m�F����T�u���[�`��
//BOOL bVideo �r�f�I�t�B���^���`�F�b�N���邩�A�I�[�f�B�I�t�B���^���`�F�b�N���邩
//////////////////////////////////////////////////
BOOL CDirectShowAccess::CheckConnectFilter(BOOL bVideo)
{
	assert(m_pDirectShow);
	int nInputCount = 0;
	int nOutputCount = 0;
	if (bVideo)
	{
		if (m_nVideoFilterID != -1) {
			CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nVideoFilterID),NULL,&nOutputCount);
			if (nOutputCount == 0) {
				AfxMessageBox(_T("�r�f�I�L���v�`���f�o�C�X�̏o�͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
		}
		if (m_nVideoSmartTeeID != -1) {
			CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),&nInputCount,&nOutputCount);
			if (nInputCount == 0) {
				AfxMessageBox(_T("�r�f�I����t�B���^�̓��͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
			if (m_bVideoForceOff == FALSE) {
				if (nOutputCount == 0) {
					AfxMessageBox(_T("�r�f�I����t�B���^�̏o�͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
					this->SaveGraphFile(_T("testes.grf"));
					return FALSE;}
			}
		}
		/*
		//TODO ���̃`�F�b�N������Ă���
		if (m_nVideoConvertID != -1) {
			CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nVideoConvertID),&nInputCount,&nOutputCount);
			if (nInputCount == 0) {
				AfxMessageBox(_T("�r�f�I�ϊ��t�B���^�̓��͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
			if (nOutputCount == 0) {
				AfxMessageBox(_T("�r�f�I�ϊ��t�B���^�̏o�͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
		}
		*/
		if (m_bVideoForceOff == FALSE) {
			CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetVideoRenderer(),&nInputCount,NULL);
			if (nOutputCount == 0) {
				AfxMessageBox(_T("�r�f�I�ϊ��t�B���^�̏o�͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
		}
	}
	else	//�I�[�f�B�I�L���v�`���̃`�F�b�N
	{
		if (m_nAudioFilterID != -1) {
			CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nAudioFilterID),NULL,&nOutputCount);
			if (nOutputCount == 0) {
				AfxMessageBox(_T("�I�[�f�B�I�L���v�`���f�o�C�X�̏o�͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
		}
		if (m_nAudioSmartTeeID != -1) {
			CDirectShowEtc::CheckConnectionFilter(m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),&nInputCount,NULL);
			if (nInputCount == 0) {
				AfxMessageBox(_T("�I�[�f�B�I����t�B���^�̓��͐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
				return FALSE;}
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////
//���݂̃r�f�I�T�C�Y���擾
//int &nWidth	�r�f�I�̕�
//int &nHeight	�r�f�I�̍���
//////////////////////////////////////////////////
BOOL CDirectShowAccess::GetCurrentVideoSize(int &nWidth,int &nHeight)
{
	nWidth = 0;
	nHeight = 0;
	AM_MEDIA_TYPE* pAmt = NULL;
	if (m_VideoLastMediaType.formattype == FORMAT_VideoInfo || 
		m_VideoLastMediaType.formattype == FORMAT_VideoInfo2)
	{
		pAmt = &(m_VideoLastMediaType);
	} else {
		pAmt = &(m_VideoFirstMediaType);
	}
	if (pAmt == NULL)
		return FALSE;
	if (pAmt->formattype == FORMAT_VideoInfo) {
		VIDEOINFOHEADER *pVideoInfoHeader = (VIDEOINFOHEADER *)pAmt->pbFormat;
		nWidth = pVideoInfoHeader->bmiHeader.biWidth;
		nHeight = pVideoInfoHeader->bmiHeader.biHeight;
	} else if (pAmt->formattype == FORMAT_VideoInfo2) {
		VIDEOINFOHEADER2 *pVideoInfoHeader2 = (VIDEOINFOHEADER2 *)pAmt->pbFormat;
		nWidth = pVideoInfoHeader2->bmiHeader.biWidth;
		nHeight = pVideoInfoHeader2->bmiHeader.biHeight;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�r�f�I�v���r���[�̃E�B���h�E�n���h����ݒ肷��
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SetPreviewWindow(HWND hWnd)
{
	bool bret = false;
	assert(m_pDirectShow);
	assert(hWnd);
	if (m_bVideoForceOff == TRUE)
		return TRUE;
	bret = m_pDirectShow->SetVideoRendererHWnd(hWnd);
	m_hVideoWnd = hWnd;
	return (BOOL) bret;
}
//////////////////////////////////////////////////
//�r�f�I�v���r���[�̃E�B���h�E�T�C�Y��ύX����
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ResizePreviewWindow(RECT rcClient)
{
	bool bret = false;
	assert(m_pDirectShow);
	if (m_bVideoForceOff == TRUE)
		return TRUE;
	if (!m_pDirectShow->GetVideoRenderer() || m_hVideoWnd == NULL)
		return FALSE;
	bret = m_pDirectShow->ResizeVideoRendererHWnd(rcClient);
	return (BOOL) bret;
}
//////////////////////////////////////////////////
//�r�f�I�v���r���[�̕\���E��\����؂�ւ���
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ShowPreviewWindow(BOOL bShow)
{
	bool bret = false;
	assert(m_pDirectShow);
	if (m_bVideoForceOff == TRUE)
		return TRUE;
	if (!m_pDirectShow->GetVideoRenderer() || m_hVideoWnd == NULL) {
		m_bVideoShow = FALSE;
		return FALSE;}
	bret = m_pDirectShow->ShowVideoRenderer(bShow);	//�r�f�I�����_���̕\���E��\��
	m_bVideoShow = bShow;
	return (BOOL) bret;
}
//////////////////////////////////////////////////
//�r�f�I�v���r���[�̃E�B���h�E�n���h������������
//////////////////////////////////////////////////
BOOL CDirectShowAccess::RemovePreviewWindow()
{
	assert(m_pDirectShow);
	if (m_bVideoForceOff == TRUE)
		return TRUE;
	bool bret = false;
	if (m_hVideoWnd != NULL) {
		bret = m_pDirectShow->ResetVideoRendererHWnd();
	} else {
		bret = TRUE;
	}
	m_hVideoWnd = NULL;
	m_bVideoShow = FALSE;
	return (BOOL) bret;
}
//////////////////////////////////////////////////
//�r�f�I/�I�[�f�B�I�L���v�`���f�o�C�X�̃v���p�e�B�y�[�W��\������
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ShowPropertySetting(BOOL bVideo,ENUM_WDM_FILTER eDevice,HWND hWnd)
{
	assert(m_pDirectShow);
	assert(hWnd);
	int nShowPropertyID = -1;
	if (eDevice != WDM_FILTER_NONE)
	{
		switch (eDevice)
		{
		case WDM_FILTER_CROSS_BAR1:
			nShowPropertyID = m_nCrossbar1ID; break;
		case WDM_FILTER_CROSS_BAR2:
			nShowPropertyID = m_nCrossbar2ID; break;
		case WDM_FILTER_TV_AUDIO:
			nShowPropertyID = m_nTVAudioID; break;
		case WDM_FILTER_TV_TUNER:
			nShowPropertyID = m_nTVTunerID; break;
		}
	} else {
		if (bVideo) {
			if (m_nVideoIndex != -1)
				nShowPropertyID = m_nVideoFilterID;
		} else {
			if (m_nAudioIndex != -1)
				nShowPropertyID = m_nAudioFilterID;
		}
	}

	if (nShowPropertyID != -1)
	{
		HRESULT hr = m_pDirectShow->ShowPropertyDialog(
			m_pDirectShow->GetFilterByID(nShowPropertyID),hWnd);
		return SUCCEEDED(hr);
	}
	return FALSE;
}

//////////////////////////////////////////////////
//�t�B���^�z��̐ڑ�
//BOOL bVideo �r�f�I�t�B���^���ǂ��� FALSE�Ȃ�audio�t�B���^
//int nInputFilterID		�ŏ����͂̃t�B���^ID
//LPCTSTR lpszInputPinName	�ŏ����̓s���̖��O
//int nOutputFilterID		�ŏI�o�͂̃t�B���^ID
//LPCTSTR lpszOutputPinName �ŏI�o�̓s���̖��O
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ChainPluginList(BOOL bVideo,int nInputFilterID,const wchar_t* lpszInputPinName,int nOutputFilterID,const wchar_t* lpszOutputPinName)
{
	assert(m_pDirectShow);
	assert(nInputFilterID != -1);
	assert(nOutputFilterID != -1);

	int nInputID = nInputFilterID;
	int nOutputID = -1;
	const wchar_t* lpszInputPin = lpszInputPinName;
	const wchar_t* lpszOutputPin = NULL;
	int nSize = 0;
	if (bVideo)
		nSize = (int)m_VideoPluginList.size();
	else
		nSize = (int)m_AudioPluginList.size();

	FilterPluginStruct *pDestFilter = NULL;	//�ڑ���̃t�B���^
	HRESULT hr = NOERROR;

	for (int i=0;i<=nSize;i++)
	{
		//���́E�o�͂̃t�B���^�[ID���擾
		if (nSize == i)	//�ŏI�ڑ�
		{
			pDestFilter = NULL;
			nOutputID = nOutputFilterID;
			lpszOutputPin = lpszOutputPinName;
		} else {		//�v���O�C���ڑ�
			if (bVideo)
				pDestFilter = &(m_VideoPluginList[i]);
			else
				pDestFilter = &(m_AudioPluginList[i]);
		}

		if (pDestFilter)	//�v���O�C���ڑ��̏ꍇ
		{
			if (pDestFilter->bEnable == FALSE) {			//�����ȃv���O�C���̏ꍇ
				if (pDestFilter->nFilterID != -1) {				//�t�B���^�����݂��Ă���̂Ȃ�폜���Ă���
					m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(pDestFilter->nFilterID));
					pDestFilter->nFilterID = -1;
				}
				continue;
			}
			/*
			if (pDestFilter->nFilterID == -1) {	//�t�B���^�����݂��Ȃ��̂ŁA�t�B���^�̐������s��
				HRESULT hr;
				if (pDestFilter->bDMO)
					hr = m_pDirectShow->AddDMOFilter(pDestFilter->guidFilter,pDestFilter->guidCategory,pDestFilter->strFilterName,&(pDestFilter->nFilterID));
				else
					hr = m_pDirectShow->AddFilter(pDestFilter->guidFilter,pDestFilter->strFilterName,&(pDestFilter->nFilterID));
				if (FAILED(hr))
				{
					CString strMessage;
					strMessage.Format(_T("%d�Ԗڂ�%s�v���O�C��\n%s�̐V�K�쐬�Ɏ��s���܂����B\n���̃v���O�C���͖����ɂȂ�܂�"),i+1,bVideo?_T("�r�f�I"):_T("�I�[�f�B�I"),pDestFilter->strFilterName);
					AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
					pDestFilter->nFilterID = -1;
					pDestFilter->bEnable = FALSE;
					continue;
				}
			} else {	//�t�B���^�͑��݂��Ă���̂Őؒf�����s��
				m_pDirectShow->DisconnectFilter(
					m_pDirectShow->GetFilterByID(pDestFilter->nFilterID),TRUE,TRUE);
			}*/
			if (pDestFilter->nFilterID != -1)
			{
				m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(pDestFilter->nFilterID));
				pDestFilter->nFilterID = -1;
			}
			HRESULT hr;
			if (pDestFilter->bDMO)
				hr = m_pDirectShow->AddDMOFilter(pDestFilter->guidFilter,pDestFilter->guidCategory,pDestFilter->strFilterName,&(pDestFilter->nFilterID));
			else
				hr = m_pDirectShow->AddFilter(pDestFilter->guidFilter,pDestFilter->strFilterName,&(pDestFilter->nFilterID));
			if (FAILED(hr))
			{
				CString strMessage;
				strMessage.Format(_T("%d�Ԗڂ�%s�v���O�C��\n%s�̐V�K�쐬�Ɏ��s���܂����B\n���̃v���O�C���͖����ɂȂ�܂�"),i+1,bVideo?_T("�r�f�I"):_T("�I�[�f�B�I"),pDestFilter->strFilterName);
				AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
				pDestFilter->nFilterID = -1;
				pDestFilter->bEnable = FALSE;
				continue;
			}
			nOutputID = pDestFilter->nFilterID;
			lpszOutputPin = NULL;
		}

		//�v���O�C���̘A�����s��
		assert(nInputID != -1);
		assert(nOutputID != -1);

		//�ڑ��ɂ͓��o�̓s������p���āA���ׂẴs���ɑ΂��čs��Őڑ������݂�
		vector<PinInfoStruct> InputPinInfo,OutputPinInfo;
		m_pDirectShow->GetFilterInfo(m_pDirectShow->GetFilterByID(nInputID),InputPinInfo,NULL,NULL);
		m_pDirectShow->GetFilterInfo(m_pDirectShow->GetFilterByID(nOutputID),OutputPinInfo,NULL,NULL);

		/*
		HRESULT CDirectShowEtc::ConnectFilterMatrix(
			IBaseFilter* pSrcFilter,
			IBaseFilter* pDestFiter,
			LPCWSTR lpszInputPin,
			LPCWSTR lpszOutputPin);
			*/

		if (lpszInputPin) {			//���̓s���w��
			if (lpszOutputPin) {
				//�o�̓s���w��
				hr = m_pDirectShow->ConnectFilterDirect(
					m_pDirectShow->GetFilterByID(nInputID),
					m_pDirectShow->GetFilterByID(nOutputID),
					NULL,lpszInputPin,lpszOutputPin);
			} else {
				//�o�̓s���s�w��
				for (int j=0;j<(int)OutputPinInfo.size();j++) {
					if (OutputPinInfo[j].PinDir != PINDIR_INPUT)
						continue;
					hr = m_pDirectShow->ConnectFilterDirect(
						m_pDirectShow->GetFilterByID(nInputID),
						m_pDirectShow->GetFilterByID(nOutputID),
						NULL,lpszInputPin,OutputPinInfo[j].strPinName.c_str());
					if (SUCCEEDED(hr))
						break;
				}
			}
		} else {					//���̓s���s�w��
			for (int i=0;i<(int)InputPinInfo.size();i++) {
				if (InputPinInfo[i].PinDir != PINDIR_OUTPUT)
					continue;

				if (lpszOutputPin) {	//�o�̓s���w��
					hr = m_pDirectShow->ConnectFilterDirect(
						m_pDirectShow->GetFilterByID(nInputID),
						m_pDirectShow->GetFilterByID(nOutputID),
						NULL,InputPinInfo[i].strPinName.c_str(),lpszOutputPin);
					if (SUCCEEDED(hr))
						break;
				} else {				//�o�̓s���s�w��
					for (int j=0;j<(int)OutputPinInfo.size();j++) {
						if (OutputPinInfo[j].PinDir != PINDIR_INPUT)
							continue;
						hr = m_pDirectShow->ConnectFilterDirect(
							m_pDirectShow->GetFilterByID(nInputID),
							m_pDirectShow->GetFilterByID(nOutputID),
							NULL,InputPinInfo[i].strPinName.c_str(),OutputPinInfo[j].strPinName.c_str());
						if (SUCCEEDED(hr))
							break;
					}
				}
				if (SUCCEEDED(hr))
					break;
			}
		}

		if (FAILED(hr))
		{
			SaveGraphFile(_T("PluginConnectionError.grf"));
			CString strMessage;
			strMessage.Format(_T("%d�Ԗڂ�%s�v���O�C��\n%s�̐ڑ��Ɏ��s���܂����B\n���̃v���O�C���͖����ɂȂ�܂�"),i+1,bVideo?_T("�r�f�I"):_T("�I�[�f�B�I"),
				pDestFilter?pDestFilter->strFilterName:_T("NULL"));
			AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);

			//�v���O�C���������ɂȂ�
			if (pDestFilter) {	//�ʏ�ڑ�(����->�v���O�C��)
				if (pDestFilter->nFilterID != -1) {
					m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(pDestFilter->nFilterID));
					pDestFilter->nFilterID = -1;
				}
				pDestFilter->bEnable = FALSE;
			} else {			//�ŏI�ڑ��̏ꍇ(�v���O�C��->�X�}�[�g�e�B�[)
				//�O�̃t�B���^���폜
				FilterPluginStruct* pSrcFilter = NULL;
				if (i >= 1) {
					if (bVideo)
						pSrcFilter = &(m_VideoPluginList[i-1]);
					else
						pSrcFilter = &(m_AudioPluginList[i-1]);
				}
				if (pSrcFilter == NULL) {
					return FALSE;
				}
				if (pSrcFilter->nFilterID != -1) {
					m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(pSrcFilter->nFilterID));
					pSrcFilter->nFilterID = -1;
				}
				pSrcFilter->bEnable = FALSE;
				//�����ŕK�v�ɂȂ�̂͂��������1�O��ID
				if (i >= 2) {
					if (bVideo)
						nInputID = m_VideoPluginList[i-2].nFilterID;
					else
						nInputID = m_AudioPluginList[i-2].nFilterID;
					lpszInputPin = NULL;
				} else {
					nInputID = nInputFilterID;
					lpszInputPin = lpszInputPinName;
				}
				i--;	//��������߂�
			}
			continue;
		}

		//���̃t�B���^�Ɉڍs���邽�߂ɏo�͂���͂ɂ���
		nInputID = nOutputID;
		lpszInputPin = NULL;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�v���O�C���̒ǉ��o�^(�Ō���Ƀv���O�C���t�B���^���ǉ������)
//BOOL bVideo				�r�f�I�t�B���^���ǂ���
//const GUID guid			�v���O�C����GUID��
//const GUID guidCategory	guid�̃J�e�S��(DMO�̏ꍇ�͒l������A����ȊO�̏ꍇ��GUID_NULL���w��)
//LPCWSTR lpszPluginName	�v���O�C���̖��O
//////////////////////////////////////////////////
BOOL CDirectShowAccess::AddPlugin(BOOL bVideo,const GUID guid,const GUID guidCategory,LPCWSTR lpszPluginName)
{
	assert(m_pDirectShow);
	if (guid == GUID_NULL)
		return FALSE;

	FilterPluginStruct fps;
	fps.guidFilter = guid;
	if (guidCategory == GUID_NULL)
		fps.bDMO = FALSE;
	else
		fps.bDMO = TRUE;
	fps.guidCategory = guidCategory;
	fps.nFilterID = -1;
	fps.strFilterName = lpszPluginName;
	fps.bEnable = TRUE;	//�ǉ������Ƃ��̓t�B���^�͗L���ɂ��Ă���

	//�����Ńt�B���^�̒ǉ������݂�
	HRESULT hr;
	if (fps.bDMO == TRUE)
	{
		hr = m_pDirectShow->AddDMOFilter(fps.guidFilter,fps.guidCategory,fps.strFilterName,&(fps.nFilterID));
	} else {
		hr = m_pDirectShow->AddFilter(fps.guidFilter,fps.strFilterName,&(fps.nFilterID));
	}
	if (FAILED(hr))
		return FALSE;	//�����ł��Ȃ��G���[

	//�t�B���^�̏����m�F
	vector<PinInfoStruct> pis;
	int nInputPinCount,nOutputPinCount;
	m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(fps.nFilterID),
		pis,&nInputPinCount,&nOutputPinCount);
	//���̓s���Əo�̓s���͍Œ�1���K�v�ł�
	if (nInputPinCount == 0 || nOutputPinCount == 0)
	{
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(fps.nFilterID));
		return FALSE;	//�t�B���^���̖̂��
	}
	if (bVideo)
		m_VideoPluginList.push_back(fps);
	else
		m_AudioPluginList.push_back(fps);
	return TRUE;
}
//////////////////////////////////////////////////
//�v���O�C���̍폜 (�폜�������vector�̍X�V���s����)
//BOOL bVideo	�r�f�I�v���O�C�����ǂ���
//int nIndex	�v���O�C���z��̃C���f�b�N�X
//////////////////////////////////////////////////
BOOL CDirectShowAccess::RemovePlugin(BOOL bVideo,int nIndex)
{
	assert(m_pDirectShow);
	assert(nIndex >= 0);
	if (bVideo) {
		if (nIndex >= (int)m_VideoPluginList.size())
			return FALSE;
	} else {
		if (nIndex >= (int)m_AudioPluginList.size())
			return FALSE;
	}
	FilterPluginStruct* pFilter = NULL;
	if (bVideo)
		pFilter = &(m_VideoPluginList[nIndex]);
	else
		pFilter = &(m_AudioPluginList[nIndex]);
	if (!pFilter)
		return FALSE;

	//�t�B���^�̍폜
	if (pFilter->nFilterID != -1) {
		m_pDirectShow->RemoveFilter(m_pDirectShow->GetFilterByID(pFilter->nFilterID));
		pFilter->nFilterID = -1;
	}

	//vector�̍X�V(�V�����z��Ɋm�ۂ����)
	vector<FilterPluginStruct> NewPluginList;
	if (bVideo) {
		for (int i=0;i<(int)m_VideoPluginList.size();i++)
		{
			if (i != nIndex)
				NewPluginList.push_back(m_VideoPluginList[i]);
		}
		m_VideoPluginList.clear();
		m_VideoPluginList = NewPluginList;
	} else {
		for (int i=0;i<(int)m_AudioPluginList.size();i++)
		{
			if (i != nIndex)
				NewPluginList.push_back(m_AudioPluginList[i]);
		}
		m_AudioPluginList.clear();
		m_AudioPluginList = NewPluginList;
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�t�B���^�z��̓���ւ�(��ցE����)
//BOOL bVideo		�r�f�I�t�B���^���ǂ���
//int nSrcIndex		����ւ���
//int nDestIndex	����ւ���
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SwitchPlugin(BOOL bVideo,int nSrcIndex,int nDestIndex)
{
	assert(m_pDirectShow);
	assert(nSrcIndex >= 0);
	assert(nDestIndex >= 0);
	if (bVideo) {
		if (nSrcIndex >= (int)m_VideoPluginList.size() || 
			nDestIndex >= (int)m_VideoPluginList.size())
			return FALSE;
	} else {
		if (nSrcIndex >= (int)m_AudioPluginList.size() ||
			nDestIndex >= (int)m_AudioPluginList.size())
			return FALSE;
	}

	FilterPluginStruct* pSrcFilter = NULL;
	FilterPluginStruct* pDestFilter = NULL;
	if (bVideo)
	{
		pSrcFilter = &(m_VideoPluginList[nSrcIndex]);
		pDestFilter = &(m_VideoPluginList[nDestIndex]);
	}
	else {
		pSrcFilter = &(m_AudioPluginList[nSrcIndex]);
		pDestFilter = &(m_AudioPluginList[nDestIndex]);
	}
	if (!pSrcFilter || !pDestFilter)
		return FALSE;

	FilterPluginStruct temp;
	temp = (*pSrcFilter);
	(*pSrcFilter) = (*pDestFilter);
	(*pDestFilter) = temp;

	return TRUE;
}

//////////////////////////////////////////////////
//�v���O�C���̐ݒ�_�C�A���O��\��
//HWND hWnd		�_�C�A���O��\������e�E�B�h�E�n���h��
//BOOL bVideo	�r�f�I�t�B���^���ǂ���
//int nIndex	�t�B���^�ԍ�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ShowPluginDialog(HWND hWnd,BOOL bVideo,int nIndex)
{
	assert(m_pDirectShow);
	assert(nIndex >= 0);

	FilterPluginStruct* pPlugin = NULL;
	if (bVideo) {
		if (nIndex < (int)m_VideoPluginList.size())
			pPlugin = &(m_VideoPluginList[nIndex]);
	}
	else {
		if (nIndex < (int)m_AudioPluginList.size())
			pPlugin = &(m_AudioPluginList[nIndex]);
	}
	if (!pPlugin)
		return FALSE;
	if (pPlugin->nFilterID == -1)
		return FALSE;
	HRESULT hr = m_pDirectShow->ShowPropertyDialog(m_pDirectShow->GetFilterByID(pPlugin->nFilterID),hWnd);
	return SUCCEEDED(hr);
}






//////////////////////////////////////////////////
//�R�[�f�b�N�֌W
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N�̑I��
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectVideoCodecIndex(int nIndex)
{
	assert(m_pDirectShowEncoder);
	assert(nIndex >= 0 && nIndex < (int)m_VideoCodecList.size());

	CODEC_FORMAT_BITRATE bitrate = CODEC_FORMAT_CBR;		//�r�b�g���[�g�̃^�C�v(CODEC_FORMAT_CBR,CODEC_FORMAT_VBR)
	CODEC_FORMAT_NUMPASS numpass = CODEC_FORMAT_ONEPASS;	//1�p�X2�p�X�G���R�[�h(CODEC_FORMAT_ONEPASS,CODEC_FORMAT_TWOPASS)
	HRESULT hr = m_pDirectShowEncoder->SetVideoCodec(nIndex,bitrate,numpass);
	if (FAILED(hr))
		return FALSE;
	m_nVideoCodecIndex = nIndex;
	return TRUE;
}
//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N�̐ݒ肻�̂P
//dwBitrate			�r�f�I�r�b�g���[�g(bit per second)
//msBufferWindow	�o�b�t�@�����O����(�P�ʂ�ms �W����8�b���炢)
//double dFPS		�t���[�����[�g(�P�ʂ̓t���[�����b)
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SetVideoCodecSetting(const DWORD dwBitrate,const DWORD msBufferWindow,const double dFPS,const RECT rcSource,const RECT rcTarget)
{
	assert(m_pDirectShowEncoder);
	assert(m_nVideoCodecIndex != -1);
	REFERENCE_TIME AvgTimePerFrame = 0;
	AvgTimePerFrame = (REFERENCE_TIME) ((double)10000000 / (double)dFPS);

	HRESULT hr = m_pDirectShowEncoder->SetVideoSetting(dwBitrate,msBufferWindow,AvgTimePerFrame,rcSource,rcTarget);
	if (FAILED(hr))
		return FALSE;
	return TRUE;
}
//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N�̐ݒ肻�̂Q
//dwQuality		���炩���`�N����(0�`100)
//nMaxKeyFrameSpacing �L�[�t���[���̊Ԋu(�P�ʂ�ms)
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SetVideoCodecSetting2(DWORD dwQuality,int nMaxKeyFrameSpacing)
{
	assert(m_pDirectShowEncoder);
	assert(m_nVideoCodecIndex != -1);
	assert(dwQuality >= 0 && dwQuality <= 100);
	assert(nMaxKeyFrameSpacing > 0 && nMaxKeyFrameSpacing <= 30000);	//0�`30�b�܂�
	REFERENCE_TIME MaxKeyFrameSpacing = nMaxKeyFrameSpacing * 10000;
	HRESULT hr = m_pDirectShowEncoder->SetVideoSetting2(dwQuality,MaxKeyFrameSpacing);
	if (FAILED(hr))
		return FALSE;
	return TRUE;
}
//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N�̕��G���̐ݒ�(�r�f�I�p�t�H�[�}���X)
//WORD wComplexity 0�`5�͈̔͂܂�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SetVideoComplexity(const WORD wComplexity)
{
	assert(m_pDirectShowEncoder);
	assert(m_nVideoCodecIndex != -1);
	assert(wComplexity >= 0 && wComplexity <= 5);
	HRESULT hr = m_pDirectShowEncoder->SetVideoComplexity(wComplexity);
	if (FAILED(hr))
		return FALSE;
	return TRUE;
}

//////////////////////////////////////////////////
//�I�[�f�B�I�R�[�f�b�N�̑I��
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectAudioCodecIndex(int nIndex)
{
	assert(m_pDirectShowEncoder);
	assert(nIndex >= 0 && nIndex < (int)m_AudioCodecList.size());
	m_AudioFormatList.clear();

	CODEC_FORMAT_BITRATE bitrate = CODEC_FORMAT_CBR;		//�r�b�g���[�g�̃^�C�v(CODEC_FORMAT_CBR,CODEC_FORMAT_VBR)
	CODEC_FORMAT_NUMPASS numpass = CODEC_FORMAT_ONEPASS;	//1�p�X2�p�X�G���R�[�h(CODEC_FORMAT_ONEPASS,CODEC_FORMAT_TWOPASS)
	HRESULT hr = m_pDirectShowEncoder->EnumAudioCodecFormat(nIndex,bitrate,numpass,m_AudioFormatList);
	if (FAILED(hr))
		return FALSE;

	//TIPS ���̒i�K�ł̓I�[�f�B�I�R�[�f�b�N��I�����Ȃ�
	m_nAudioCodecIndex = nIndex;
	m_nAudioCodecFormatIndex = -1;	//�����ȃt�H�[�}�b�g�ݒ�̑��
	return TRUE;
}
//////////////////////////////////////////////////
//�I�[�f�B�I�R�[�f�b�N�t�H�[�}�b�g�̑I��
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SelectAudioCodecFormatIndex(int nIndex)
{
	assert(m_pDirectShowEncoder);
	assert(m_nAudioCodecIndex != -1);

	if (nIndex < 0 || nIndex >= (int)m_AudioFormatList.size())
	{
		m_nAudioCodecFormatIndex = -1;
		return FALSE;
	}
	//assert(nIndex >= 0 && nIndex < (int)m_AudioFormatList.size());

	//�t�H�[�}�b�g�̓K�p
	CODEC_FORMAT_BITRATE bitrate = CODEC_FORMAT_CBR;		//�r�b�g���[�g�̃^�C�v(CODEC_FORMAT_CBR,CODEC_FORMAT_VBR)
	CODEC_FORMAT_NUMPASS numpass = CODEC_FORMAT_ONEPASS;	//1�p�X2�p�X�G���R�[�h(CODEC_FORMAT_ONEPASS,CODEC_FORMAT_TWOPASS)
	HRESULT hr = m_pDirectShowEncoder->SetAudioCodec(m_nAudioCodecIndex,nIndex,bitrate,numpass);
	if (FAILED(hr))
		return FALSE;
	m_nAudioCodecFormatIndex = nIndex;
	return TRUE;
}



//////////////////////////////////////////////////
//�G���R�[�h�̊J�n
//////////////////////////////////////////////////
BOOL CDirectShowAccess::StartEncode()
{
	if (m_bStartEncode)
		return FALSE;
	TRACE0("CDirectShowAccess::StartEncode()\n");

	assert(m_pDirectShow);
	assert(m_pDirectShowEncoder);
	assert(m_nVideoCodecIndex != -1);
	assert(m_nAudioCodecIndex != -1);
	assert(m_nAudioCodecFormatIndex != -1);

	HRESULT hr = NOERROR;

	//�v���r���[�s�̏�ԂȂ�G���R�[�h�s�ɂȂ�
	if (!IsCanPreview())
		return FALSE;

	//�ő�E�ŏ��p�P�b�g�T�C�Y�̐ݒ�
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	if (pProfile->m_nOutputMaxPacketSize != 0 || pProfile->m_nOutputMinPacketSize != 0) {
		hr = m_pDirectShowEncoder->SetPacketSize(pProfile->m_nOutputMinPacketSize,pProfile->m_nOutputMaxPacketSize);
		if (FAILED(hr))
		{
			AfxMessageBox(_T("�ő�E�ŏ��p�P�b�g�T�C�Y�̕ύX�Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}
	}

	//�v���t�@�C���̓K�p
	hr = m_pDirectShowEncoder->ApplyProfiles();
	if (FAILED(hr))	{
		AfxMessageBox(_T("�G���R�[�_�[�v���t�@�C���̓K�p�Ɏ��s���܂���\r\n���k�ݒ���������Ă�������"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

#ifdef DEBUG
	m_pDirectShowEncoder->SaveProfile(m_pDirectShowEncoder->GetProfiles(),_T("debug_profile.prx"));
#endif

	//�t�B���^�̒�~
	if (IsRunFilter())
		StopFilter();

	//�e���|�����o�̓p�X�̐ݒ�
	if (m_strAsfTempName.IsEmpty())
	{
		TCHAR strTempPath[MAX_PATH];
		GetTempPath(MAX_PATH,strTempPath);
		m_strAsfTempName = strTempPath;
		m_strAsfTempName += _T("kte_temp_asf.tmp");
	}

	//ASF���C�^�t�B���^�����݂��Ă���ꍇ�͍폜
	if (m_pDirectShowASFWriter) {
		m_pDirectShowASFWriter->Delete(m_pDirectShow->GetGraphBuilder());
		delete m_pDirectShowASFWriter;
		m_pDirectShowASFWriter = NULL;
	}
	//ASF���C�^�t�B���^�̍쐬
	m_pDirectShowASFWriter = new CDirectShowASFWriter;
	hr = m_pDirectShowASFWriter->Create(m_pDirectShow->GetGraphBuilder(),m_strAsfTempName);
	if (FAILED(hr)) {
		AfxMessageBox(_T("�l�b�g���[�N�o�͂̓K�p�Ɏ��s���܂���\r\n�o�͐ݒ���������Ă�������"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//ASF���C�^�Ƀv���t�@�C���̓K�p
	hr = m_pDirectShowASFWriter->ApplyProfiles(m_pDirectShowEncoder->GetProfiles());
	if (FAILED(hr)) {
		AfxMessageBox(_T("�l�b�g���[�N�o�͐ݒ�Ɉ��k���̓K�p�Ɏ��s���܂���\r\n�o�͐ݒ���������Ă�������"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//ASF���C�^�̎�X�̃p�����[�^�̐ݒ� (StartEncode()�̃T�u���[�`��)
	hr = SetAsfWriterParam();
	if (FAILED(hr)) {
		AfxMessageBox(_T("�l�b�g���[�N�o�͂̃p�����[�^�ݒ�̓K�p�Ɏ��s���܂���\r\n�o�́E�����ݒ���������Ă�������"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	LPCWSTR lpszOutputPin  = NULL;
	vector<PinInfoStruct> PinInfo;
	m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),
		PinInfo,NULL,NULL);
	for (int i=0;i<(int)PinInfo.size();i++) {
		if (PinInfo[i].PinDir != PINDIR_OUTPUT)
			continue;
		if (PinInfo[i].bConnected == TRUE)
			continue;
		lpszOutputPin = PinInfo[i].strPinName.c_str();
		break;
	}
	//�I�[�f�B�I�t�B���^�̐ڑ�
	hr = m_pDirectShow->ConnectFilterDirect(
		m_pDirectShow->GetFilterByID(m_nAudioSmartTeeID),
		m_pDirectShowASFWriter->GetFilter(),
		NULL/*&m_AudioLastMediaType*/,lpszOutputPin,L"Audio Input 01");
	if (FAILED(hr)){
		AfxMessageBox(_T("�l�b�g���[�N���C�^�ւ̃I�[�f�B�I�t�B���^�ڑ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//�r�f�I�t�B���^�̐ڑ�
	//�󂫂̗L������t�B���^�o�̓s��������
	lpszOutputPin  = NULL;
	PinInfo.clear();
	m_pDirectShow->GetFilterInfo(
		m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),
		PinInfo,NULL,NULL);
	for (int i=0;i<(int)PinInfo.size();i++) {
		if (PinInfo[i].PinDir != PINDIR_OUTPUT)
			continue;
		if (PinInfo[i].bConnected == TRUE)
			continue;
		lpszOutputPin = PinInfo[i].strPinName.c_str();
		break;
	}
	hr = m_pDirectShow->ConnectFilterDirect(
		m_pDirectShow->GetFilterByID(m_nVideoSmartTeeID),
		m_pDirectShowASFWriter->GetFilter(),
		NULL/*&m_VideoLastMediaType*/,lpszOutputPin,L"Video Input 01");
	if (FAILED(hr)){
		AfxMessageBox(_T("�l�b�g���[�N���C�^�ւ̃r�f�I�t�B���^�ڑ��Ɏ��s���܂���"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//���͐ڑ��̃`�F�b�N
	int nInputCount = -1;
	CDirectShowEtc::CheckConnectionFilter(m_pDirectShowASFWriter->GetFilter(),&nInputCount,NULL);
	if (nInputCount != 2) {
		AfxMessageBox(_T("�l�b�g���[�N���C�^�ւ̐ڑ��Ɏ��s���Ă��܂�"),MB_OK|MB_ICONINFORMATION);
		return FALSE;
	}

	//�O���t�t�@�C���̃Z�[�u
	SaveGraphFile(_T("encode_start.grf"));

	//�o�̓��X�g�̒ǉ�(�����Ŏ��s����\���͂���)
	if (!ApplyAllSink())
		return FALSE;
	
	//TODO �e�X�g�R�[�h
	//m_pDirectShowASFWriter->SetFixedFrameRate(FALSE);

	//�v���Z�X�v���C�I���e�B�̕ύX
	CKTEApp* pApp = ((CKTEApp*)AfxGetApp());
	pApp->SetPriorityClass(pProfile->m_bProcessPriority,pProfile->m_nProcessPriority);

	//�t�B���^�̍ĊJ
	if (!RunFilter())
	{
		AfxMessageBox(_T("�G���R�[�h�̊J�n�Ɏ��s���܂���"));
		return FALSE;
	}
	//�G���R�[�h���̃t���O������
	m_bStartEncode = TRUE;
	return TRUE;
}

//////////////////////////////////////////////////
//ASF���C�^�Ƀp�����[�^�̓K�p
//StartEncode()�̃T�u���[�`��
//////////////////////////////////////////////////
HRESULT CDirectShowAccess::SetAsfWriterParam()
{
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();

	HRESULT hr = NOERROR;
	//�o�b�t�@���Ԃ̐ݒ�
	if (pProfile->m_nOutputSyncToleranceTime != 0)
	{
		hr = m_pDirectShowASFWriter->SetAsfSyncTolerance(pProfile->m_nOutputSyncToleranceTime);
		if (FAILED(hr))return hr;
	}
	//�C���f�b�N�X���[�h�L��
	hr = m_pDirectShowASFWriter->SetAsfIndexMode(pProfile->m_bOutputInsertIndex);
	if (FAILED(hr))return hr;
	//�I�����ɃC���f�b�N�X�𐶐����邩�ۂ��H
	if (pProfile->m_bOutputInsertIndex)
	{
		hr = m_pDirectShowASFWriter->SetAsfParam(AM_CONFIGASFWRITER_PARAM_AUTOINDEX,pProfile->m_bOutputInsertIndexEnd);
		if (FAILED(hr))return hr;
	}
	//2pass�ȏ�̏ꍇ��TRUE�ɂ���
	hr = m_pDirectShowASFWriter->SetAsfParam(AM_CONFIGASFWRITER_PARAM_MULTIPASS,FALSE);
	if (FAILED(hr)) return hr;
	//�񈳏k���[�h��L���ɂ��邩�ǂ����H
	hr = m_pDirectShowASFWriter->SetAsfParam(AM_CONFIGASFWRITER_PARAM_DONTCOMPRESS,FALSE);
	if (FAILED(hr)) return hr;

	//���^�f�[�^�̐ݒ�
	m_pDirectShowASFWriter->SetTitle(m_strTitle);
	m_pDirectShowASFWriter->SetAuthor(m_strAuthor);
	m_pDirectShowASFWriter->SetDescription(m_strDescription);
	m_pDirectShowASFWriter->SetRating(m_strRating);
	m_pDirectShowASFWriter->SetCopyright(m_strCopyright);

	//�R�[���o�b�N�p�̃E�B���h�E�n���h���̐ݒ�
	assert(m_hNetworkCallbackWnd);
	if (m_hNetworkCallbackWnd)
		m_pDirectShowASFWriter->SetNetworkCallbackHWnd(m_hNetworkCallbackWnd);
	else
		return E_FAIL;
	return hr;
}
//////////////////////////////////////////////////
//�S�ẴV���N(�v���E�v�b�V���E�t�@�C��)��K�p����
//StartEncode()�̃T�u���[�`��
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ApplyAllSink()
{
	HRESULT hr = NOERROR;
	for (int i=0;i<(int)m_PullSinkList.size();i++) {
		if (m_PullSinkList[i].nID != -1 && m_PullSinkList[i].dwPort != 0) {
			hr = ApplyPullSink(i);
			if (FAILED(hr)) return FALSE;
		}
	}
	for (int i=0;i<(int)m_PushSinkList.size();i++) {
		if (m_PushSinkList[i].nID != -1 && !m_PushSinkList[i].strServerName.IsEmpty()) {
			hr = ApplyPushSink(i);
			if (FAILED(hr)) return FALSE;
		}
	}
	for (int i=0;i<(int)m_FileSinkList.size();i++) {
		if (m_FileSinkList[i].nID != -1 && !m_FileSinkList[i].strFileName.IsEmpty()) {
			hr = ApplyFileSink(i);
			if (FAILED(hr)) return FALSE;
		}
	}
	return TRUE;
}

//////////////////////////////////////////////////
//�G���R�[�h�̒�~
//////////////////////////////////////////////////
BOOL CDirectShowAccess::StopEncode(BOOL bForceDisconnect)
{
	//if (!m_bStartEncode)	//�G���R�[�h���J�n���Ă��Ȃ�
	//	return FALSE;
	TRACE0("CDirectShowAccess::StopEncode()\n");

	assert(m_pDirectShow);

	//�l�b�g���[�N�̋����ؒf�����s
	CProfile* pProfile = ((CKTEApp*)AfxGetApp())->GetProfile();
	if (bForceDisconnect == TRUE || pProfile->m_bOutputForceDisconnect)
	{
		if (m_pDirectShowASFWriter)
			m_pDirectShowASFWriter->DeleteAllSink();
	}

	//�t�B���^�̒�~
	if (IsRunFilter())
		StopFilter();

	//ASF���C�^�̍폜
	//m_pDirectShow->SaveGraphFile(_T("encode_end_start.grf"));
	if (m_pDirectShowASFWriter)
	{
		m_pDirectShowASFWriter->Delete(m_pDirectShow->GetGraphBuilder());
		delete m_pDirectShowASFWriter;
		m_pDirectShowASFWriter = NULL;
	}
	//m_pDirectShow->SaveGraphFile(_T("encode_end_end.grf"));

	//�e���|�����t�@�C���̍폜
	if (m_strAsfTempName.IsEmpty() != FALSE)
		DeleteFile(m_strAsfTempName);

	//�v���Z�X�v���C�I���e�B�����ɖ߂�
	CKTEApp* pApp = (CKTEApp*)AfxGetApp();
	pApp->SetPriorityClass(FALSE,0);
	
	//�v���r���[�̍ĊJ
	if (IsCanPreview())
		RunFilter();

	m_bStartEncode = FALSE;		//�G���R�[�h���̃t���O��������
	return TRUE;
}

//////////////////////////////////////////////////
//�v���o�͂�ǉ�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::AddPullSink(DWORD dwPort,DWORD dwMax,int &nID)
{
	assert(dwPort > 0 && dwPort <= 655535);
	assert(dwMax > 0 && dwMax <= 50);
	BOOL bAdd = FALSE;
	for (int i = 0;i<(int)m_PullSinkList.size();i++) {
		if (m_PullSinkList[i].nID == -1) {
			m_PullSinkList[i].nID = i;
			m_PullSinkList[i].nRegistID = -1;
			m_PullSinkList[i].dwPort = dwPort;
			m_PullSinkList[i].dwMax = dwMax;
			nID = m_PullSinkList[i].nID;
			bAdd = TRUE;
			break;
		}
	}
	if (bAdd == FALSE) {
		PullSinkStruct pss;
		pss.nID = (int)m_PullSinkList.size();
		pss.nRegistID = -1;
		pss.dwPort = dwPort;
		pss.dwMax = dwMax;
		nID = pss.nID;
		m_PullSinkList.push_back(pss);
	}
	//�G���R�[�h���̏ꍇ�͑����ǉ�����
	if (m_pDirectShowASFWriter && IsEncode()) {
		HRESULT hr;
		hr = ApplyPullSink(nID);
		return SUCCEEDED(hr);
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�v���o�͂��폜
//////////////////////////////////////////////////
BOOL CDirectShowAccess::RemovePullSink(int nID)
{
	assert(nID >= 0);
	//�G���R�[�h���̏ꍇ�̓V���N���폜����
	if (m_pDirectShowASFWriter && IsEncode()) {
		int nRegistID = -1;
		for (int i=0;i<(int)m_PullSinkList.size();i++)
		{
			if (m_PullSinkList[i].nID == nID) {
				nRegistID = m_PullSinkList[i].nRegistID;
				break;
			}
		}
		if (nRegistID != -1) {
			HRESULT hr = NOERROR;
			hr = m_pDirectShowASFWriter->RemoveNetworkSink(nRegistID);
			assert(SUCCEEDED(hr));
		}
	}
	//���X�g����폜
	for (int i=0;i<(int)m_PullSinkList.size();i++) {
		if (m_PullSinkList[i].nID == nID) {
			m_PullSinkList[i].nID = -1;
			m_PullSinkList[i].nRegistID = -1;
			m_PullSinkList[i].dwPort = 0;
			m_PullSinkList[i].dwMax = 0;
			return TRUE;
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////
//�v���o�͂̓K�p
//////////////////////////////////////////////////
HRESULT CDirectShowAccess::ApplyPullSink(int nIndex)
{
	assert(m_pDirectShowASFWriter);
	assert(nIndex >= 0 && nIndex <(int)m_PullSinkList.size());
	if (m_PullSinkList[nIndex].nID == -1)
		return E_FAIL;

	HRESULT hr = NOERROR;
	m_PullSinkList[nIndex].nRegistID = -1;
	//�l�b�g���[�N�V���N�̒ǉ�
	hr = m_pDirectShowASFWriter->AddNetworkSink(
		&(m_PullSinkList[nIndex].dwPort),
		m_PullSinkList[nIndex].dwMax,
		&(m_PullSinkList[nIndex].nRegistID));
	if (FAILED(hr))
	{
		CString strMessage;
		strMessage.Format(_T("�u���[�h�o���h�o�͂̓o�^�Ɏ��s���܂����B\n�|�[�g���d�����Ă���\��������܂��B\n�|�[�g�ԍ�:%d"),
			m_PullSinkList[nIndex].dwPort);
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		RemovePullSink(m_PullSinkList[nIndex].nID);
	}
	return hr;
}
//////////////////////////////////////////////////
//�v���o�͂�ύX(�ő�ڑ��l���̕ύX)
//////////////////////////////////////////////////
BOOL CDirectShowAccess::ChangePullSink(int nID,DWORD dwMax)
{
	assert(nID >= 0);
	if (m_pDirectShowASFWriter && IsEncode()) {
		int nRegistID = -1;
		for (int i=0;i<(int)m_PullSinkList.size();i++)
		{
			if (m_PullSinkList[i].nID == nID) {
				nRegistID = m_PullSinkList[i].nRegistID;
				break;
			}
		}
		if (nRegistID != -1) {
			HRESULT hr = NOERROR;
			hr = m_pDirectShowASFWriter->ChangeNetworkSink(nRegistID,dwMax);
			assert(SUCCEEDED(hr));
			return SUCCEEDED(hr);
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////
//�v���o�͂̏����擾
//////////////////////////////////////////////////
BOOL CDirectShowAccess::GetPullSink(int nIndex,DWORD dwIPAddress,DWORD dwPort,NetworkClientStruct &Client,DWORD &dwSrcPort)
{
	assert(nIndex >= 0);
	if (m_pDirectShowASFWriter && IsEncode()) {

		for (int i=0;i<(int)m_PullSinkList.size();i++)
		{
			if (m_PullSinkList[i].nRegistID == nIndex)
			{
				HRESULT hr = NOERROR;
				dwSrcPort = m_PullSinkList[i].dwPort;
				hr = m_pDirectShowASFWriter->GetNetworkSink(nIndex,dwIPAddress,dwPort,Client);
				return SUCCEEDED(hr);
			}
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////
//�v���o�͂������ȃf�[�^�ł���̂��`�F�b�N����
//����͖����ȃR�l�N�V�����������ꍇ�̃`�F�b�N�@�\�ł���
//////////////////////////////////////////////////
BOOL CDirectShowAccess::IsVaildPullSink(int nIndex)
{
	assert(nIndex >= 0);
	if (m_pDirectShowASFWriter && IsEncode()) {
		for (int i=0;i<(int)m_PullSinkList.size();i++)
		{
			if (m_PullSinkList[i].nRegistID == nIndex)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}


//////////////////////////////////////////////////
//�v�b�V���o�͂�ǉ�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::AddPushSink(CString strServerName,BOOL bAutoDelete,int &nID)
{
	assert(strServerName.Compare(_T("")) != 0 && strServerName.IsEmpty() != TRUE);
	BOOL bAdd = FALSE;
	for (int i = 0;i<(int)m_PushSinkList.size();i++) {
		if (m_PushSinkList[i].nID == -1) {
			m_PushSinkList[i].nID = i;
			m_PushSinkList[i].nRegistID = -1;
			m_PushSinkList[i].strServerName = strServerName;
			m_PushSinkList[i].bAutoDelete = bAutoDelete;
			nID = m_PushSinkList[i].nID;
			bAdd = TRUE;
			break;
		}
	}
	if (bAdd == FALSE) {
		PushSinkStruct pss;
		pss.nID = (int)m_PushSinkList.size();
		pss.nRegistID = -1;
		pss.strServerName = strServerName;
		pss.bAutoDelete = bAutoDelete;
		nID = pss.nID;
		m_PushSinkList.push_back(pss);
	}
	//�G���R�[�h���̏ꍇ�͑����ǉ�����
	if (m_pDirectShowASFWriter && IsEncode()) {
		HRESULT hr;
		hr = ApplyPushSink(nID);
		return SUCCEEDED(hr);
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�v�b�V���o�͂��폜
//////////////////////////////////////////////////
BOOL CDirectShowAccess::RemovePushSink(int nID)
{
	assert(nID >= 0);
	//�G���R�[�h���̏ꍇ�̓V���N���폜����
	if (m_pDirectShowASFWriter && IsEncode()) {
		int nRegistID = -1;
		for (int i=0;i<(int)m_PushSinkList.size();i++)
		{
			if (m_PushSinkList[i].nID == nID) {
				nRegistID = m_PushSinkList[i].nRegistID;
				break;
			}
		}
		if (nRegistID != -1) {
			HRESULT hr = NOERROR;
			hr = m_pDirectShowASFWriter->RemovePushSink(nRegistID);
			assert(SUCCEEDED(hr));
		}
	}
	//���X�g����폜
	for (int i=0;i<(int)m_PushSinkList.size();i++) {
		if (m_PushSinkList[i].nID == nID) {
			m_PushSinkList[i].nID = -1;
			m_PushSinkList[i].nRegistID = -1;
			m_PushSinkList[i].strServerName.Empty();
			m_PushSinkList[i].bAutoDelete = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////
//�v�b�V���o�͂̓K�p
//////////////////////////////////////////////////
HRESULT CDirectShowAccess::ApplyPushSink(int nIndex)
{
	assert(m_pDirectShowASFWriter);
	assert(nIndex >= 0 && nIndex <(int) m_PushSinkList.size());
	if (m_PushSinkList[nIndex].nID == -1)
		return E_FAIL;
	if (m_PushSinkList[nIndex].strServerName.IsEmpty())
		return E_FAIL;

	HRESULT hr = NOERROR;
	m_PushSinkList[nIndex].nRegistID = -1;
	hr = m_pDirectShowASFWriter->AddPushSink(
		m_PushSinkList[nIndex].strServerName,
		NULL,
		m_PushSinkList[nIndex].bAutoDelete,
		&(m_PushSinkList[nIndex].nRegistID));
	if (FAILED(hr))
	{
		CString strMessage;
		strMessage.Format(_T("�p�u���b�V���|�C���g�o�͂̓o�^�Ɏ��s���܂����B\n�����ȃT�[�o�[�����̓T�[�o�[�֐ڑ��ł��Ȃ���Ԃɂ���܂��B\n%s"),
			m_PushSinkList[nIndex].strServerName);
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		RemovePushSink(m_PushSinkList[nIndex].nID);
	}
	return hr;
}

//////////////////////////////////////////////////
//�t�@�C���ɏo�͂�ǉ�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::AddFileSink(CString strFileName,int &nID)
{
	assert(strFileName.Compare(_T("")) != 0 && strFileName.IsEmpty() != TRUE);
	BOOL bAdd = FALSE;
	for (int i = 0;i<(int)m_FileSinkList.size();i++) {
		if (m_FileSinkList[i].nID == -1) {
			m_FileSinkList[i].nID = i;
			m_FileSinkList[i].nRegistID = -1;
			m_FileSinkList[i].strFileName = strFileName;
			nID = m_FileSinkList[i].nID;
			bAdd = TRUE;
			break;
		}
	}
	if (bAdd == FALSE) {
		FileSinkStruct fss;
		fss.nID = (int)m_FileSinkList.size();
		fss.nRegistID = -1;
		fss.strFileName = strFileName;
		nID = fss.nID;
		m_FileSinkList.push_back(fss);
	}

	//�G���R�[�h���̏ꍇ�͑����ǉ�����
	if (m_pDirectShowASFWriter && IsEncode()) {
		HRESULT hr;
		hr = ApplyFileSink(nID);
		return SUCCEEDED(hr);
	}
	return TRUE;
}
//////////////////////////////////////////////////
//�t�@�C���ɏo�͂��폜
//////////////////////////////////////////////////
BOOL CDirectShowAccess::RemoveFileSink(int nID)
{
	assert(nID >= 0);
	//�G���R�[�h���̏ꍇ�̓V���N���폜����
	if (m_pDirectShowASFWriter && IsEncode()) {
		int nRegistID = -1;
		for (int i=0;i<(int)m_FileSinkList.size();i++)
		{
			if (m_FileSinkList[i].nID == nID) {
				nRegistID = m_FileSinkList[i].nRegistID;
				break;
			}
		}
		if (nRegistID != -1) {
			HRESULT hr = NOERROR;
			hr = m_pDirectShowASFWriter->RemoveFileSink(nRegistID);
			assert(SUCCEEDED(hr));
		}
	}
	//���X�g����폜(�����ȏ��ɂ��Ă���)
	for (int i=0;i<(int)m_FileSinkList.size();i++) {
		if (m_FileSinkList[i].nID == nID) {
			m_FileSinkList[i].nID = -1;
			m_FileSinkList[i].nRegistID = -1;
			m_FileSinkList[i].strFileName.Empty();
			return TRUE;
		}
	}
	return FALSE;
}
//////////////////////////////////////////////////
//�t�@�C���ɏo�͂̓K�p
//////////////////////////////////////////////////
HRESULT CDirectShowAccess::ApplyFileSink(int nIndex)
{
	assert(m_pDirectShowASFWriter);
	assert(nIndex >= 0 && nIndex <(int) m_FileSinkList.size());
	if (m_FileSinkList[nIndex].nID == -1)
		return E_FAIL;
	if (m_FileSinkList[nIndex].strFileName.IsEmpty())
		return E_FAIL;

	HRESULT hr = NOERROR;
	m_FileSinkList[nIndex].nRegistID = -1;
	hr = m_pDirectShowASFWriter->AddFileSink(m_FileSinkList[nIndex].strFileName,&(m_FileSinkList[nIndex].nRegistID));
	if (FAILED(hr))
	{
		CString strMessage;
		strMessage.Format(_T("�t�@�C���o�͂̓o�^�Ɏ��s���܂����B�p�X�̊m�F�����Ă��������B\n%s"),m_FileSinkList[nIndex].strFileName);
		AfxMessageBox(strMessage,MB_OK|MB_ICONINFORMATION);
		RemoveFileSink(m_FileSinkList[nIndex].nID);
	}
	return hr;
}

//////////////////////////////////////////////////
//���v�����擾
//////////////////////////////////////////////////
BOOL CDirectShowAccess::GetStatistics(QWORD &cnsCurrentTime,WM_WRITER_STATISTICS &Stats,WM_WRITER_STATISTICS_EX &StatsEx)
{
	assert(m_pDirectShowASFWriter);
	HRESULT hr = m_pDirectShowASFWriter->GetStatistics(cnsCurrentTime,Stats,StatsEx);
	return SUCCEEDED(hr);
}

//////////////////////////////////////////////////
//���^�f�[�^��ݒ�
//////////////////////////////////////////////////
BOOL CDirectShowAccess::SetMetadata(CString &strTitle,CString &strAuthor,CString &strDescription,CString &strRating,CString &strCopyright)
{
	//���̃^�C�~���O�ł͒l�̊i�[�݂̂����ł��Ȃ�(�ݒ��StartEncode()�ōs��)
	m_strTitle = strTitle;
	m_strAuthor = strAuthor;
	m_strDescription = strDescription;
	m_strRating = strRating;
	m_strCopyright = strCopyright;
	return TRUE;
}



