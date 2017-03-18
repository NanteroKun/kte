#include "StdAfx.h"
#include "DirectShow.h"

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CDirectShow::CDirectShow(void)
{
	m_bComInit = false;
	m_pGraphBuilder = NULL;
	m_pCaptureGraphBuilder2 = NULL;
	m_pMediaControl = NULL;
	m_pMediaEventEx = NULL;
	m_pReferenceClock = NULL;

	m_pVideoWindow = NULL;

	m_pVideoRenderer = NULL;
	m_pAudioRenderer = NULL;

	m_dwRegister = 0x0;
	m_bRegister = false;

	m_FilterData.clear();
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CDirectShow::~CDirectShow(void)
{
	Exit();
}

//////////////////////////////////////////////////
//DirectShow�̏�����
//////////////////////////////////////////////////
bool CDirectShow::Init(HINSTANCE hInst)
{
	HRESULT hr = NOERROR;
	if (!m_bComInit)
	{
		hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
		//hr = CoInitialize(NULL);
		if (!CHECK_RESULT("Com InitializeEx.",hr)) {return false;}
		m_bComInit = true;
	}

	//�f�o�b�O���C�u������������
#ifdef _DEBUG
	DbgInitialise(hInst);
	DbgSetModuleLevel(LOG_TRACE,5);
	DbgSetModuleLevel(LOG_ERROR,5);
#endif
	//NOTE1("%S", __FUNCTION__ );

	//�t�B���^�O���t�}�l�[�W��(m_pGraphBuilder)���쐬����
	hr = CoCreateInstance(CLSID_FilterGraph,
		NULL,
		CLSCTX_INPROC,	//CLSCTX_INPROCCLSCTX_INPROC_SERVER
		IID_IGraphBuilder, (void **)&m_pGraphBuilder);
	if (!CHECK_RESULT("IGraphBuilder Create.",hr)) {return false;}

	//CaptureGraphBuilder2�Ƃ����L���v�`���pGraphBuilder�𐶐�����
	hr = CoCreateInstance(CLSID_CaptureGraphBuilder2,
		NULL, CLSCTX_INPROC, 
		IID_ICaptureGraphBuilder2, 
		(void **)&m_pCaptureGraphBuilder2);
	if (!CHECK_RESULT("Create ICaptureGraphBuilder2.",hr)) {return false;}

	// FilterGraph���Z�b�g����
	hr = m_pCaptureGraphBuilder2->SetFiltergraph(m_pGraphBuilder);
	if (!CHECK_RESULT("ICaptureGraphBuilder2 SetFiltergraph.",hr)) {return false;}

	// MediaControl�C���^�[�t�F�[�X�擾
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaControl,
		(void **)&m_pMediaControl);
	if (!CHECK_RESULT("IGraphBuilder Create IMediaControl.",hr)) {return false;}

#ifdef _DEBUG
	CDirectShowEtc::AddToRot(m_pGraphBuilder,m_dwRegister);
	m_bRegister = true;
#endif 
	return true;
}
//////////////////////////////////////////////////
//�r�f�I�����_���̏�����
//////////////////////////////////////////////////
bool CDirectShow::InitVideoRenderer(int nVMR,BOOL CanUseOverlay)
{
	assert(m_pGraphBuilder);
	HRESULT hr;
	if (m_pVideoRenderer)
	{
		hr = m_pGraphBuilder->RemoveFilter(m_pVideoRenderer);
		SAFE_RELEASE(m_pVideoRenderer);
	}

	switch (nVMR)
	{
	case 0:		//�W���̃r�f�I�����_��
		hr = CoCreateInstance(CLSID_VideoRenderer,
			NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pVideoRenderer);
		if (!CHECK_RESULT("VideoRenderer(CLSID_VideoRenderer) Create.",hr)) {return false;}
		break;
	case 1:		//VMR7���g����Ƃ���7���g���A�����łȂ��Ȃ�W���̃r�f�I�����_��
		hr = CoCreateInstance(CLSID_VideoRendererDefault,
			NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pVideoRenderer);
		if (!CHECK_RESULT("VideoRenderer(CLSID_VideoRendererDefault) Create.",hr)) {return false;}
		break;
	case 7:		//VMR7�𒼐ڎw��
		hr = CoCreateInstance(CLSID_VideoMixingRenderer,
			NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pVideoRenderer);
		if (!CHECK_RESULT("VideoRenderer(CLSID_VideoMixingRenderer) Create.",hr)) {return false;}
		break;
	case 9:		//VMR9�𒼐ڎw��
		hr = CoCreateInstance(CLSID_VideoMixingRenderer9,
			NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pVideoRenderer);
		if (!CHECK_RESULT("VideoRenderer(CLSID_VideoMixingRenderer9) Create.",hr)) {return false;}
		break;
	/*
	case 10:	//Enhanced Video Renderer
		hr = CoCreateInstance(CLSID_EnhancedVideoRenderer,
			NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pVideoRenderer);
		if (!CHECK_RESULT("VideoRenderer(CLSID_EnhancedVideoRenderer) Create.",hr)) {return false;}
		break;
	*/
	}

	if (nVMR == 0) {
		CComPtr<IDirectDrawVideo> pDirectDrawVideo;
		hr = m_pVideoRenderer->QueryInterface(IID_IDirectDrawVideo,(void**)&pDirectDrawVideo);
		if (SUCCEEDED(hr)) {
			hr = pDirectDrawVideo->SetSwitches(AMDDS_DEFAULT);
		}
	}
	if (nVMR == 1 || nVMR == 7) {
		CComPtr<IVMRFilterConfig> pVMRFilterConfig;
		hr = m_pVideoRenderer->QueryInterface(IID_IVMRFilterConfig,(void**)&pVMRFilterConfig);
		if (SUCCEEDED(hr)) {
			hr = pVMRFilterConfig->SetNumberOfStreams(1);
			if (CanUseOverlay)	//�I�[�o�[���C������
				hr = pVMRFilterConfig->SetRenderingPrefs(RenderPrefs_ForceOverlays);
			else				//�I�t�X�N���[��������
				hr = pVMRFilterConfig->SetRenderingPrefs(RenderPrefs_ForceOffscreen);
		}
	}
	if (nVMR == 9) {
		CComPtr<IVMRFilterConfig9> pVMRFilterConfig9;
		hr = m_pVideoRenderer->QueryInterface(IID_IVMRFilterConfig9,(void**)&pVMRFilterConfig9);
		if (SUCCEEDED(hr)) {
			hr = pVMRFilterConfig9->SetNumberOfStreams(1);
		}
	}
	
	hr = m_pGraphBuilder->AddFilter(m_pVideoRenderer,L"Video Renderer");
	return true;
}
//////////////////////////////////////////////////
//�r�f�I�����_���̃E�B���h�E�n���h����ݒ�
//�E�B���h�E�`��I�u�W�F�N�g�������ƃG���[�ɂȂ�
//HWND �`���̃E�B���h�E�n���h��
//////////////////////////////////////////////////
bool CDirectShow::SetVideoRendererHWnd(HWND hWnd)
{
	assert(m_pGraphBuilder);
	assert(m_pVideoRenderer);
	HRESULT hr = NOERROR;

	SAFE_RELEASE(m_pVideoWindow)
	//�r�f�I�E�B���h�E�C���^�t�F�[�X���擾(HWND��ݒ�)
	hr = m_pGraphBuilder->QueryInterface(IID_IVideoWindow,(void **)&m_pVideoWindow);
	if (!CHECK_RESULT("IVideoWindow Create.",hr)) {return false;}
	//Window�n���h���֘A�t��
	//TODO �s�����ڑ�����Ă��Ȃ��ꍇ�͂��̑���͖����ɂȂ�
	hr = m_pVideoWindow->put_Owner((OAHWND)hWnd);
	if (!CHECK_RESULT("IVideoWindow put_Owner.",hr)) {return false;}
	//hr = m_pVideoWindow->put_WindowStyle(WS_CHILD);
	hr = m_pVideoWindow->put_WindowStyle(WS_CHILD|WS_CLIPSIBLINGS);
	if (!CHECK_RESULT("IVideoWindow put_WindowStyle.",hr)) {return false;}
	/*
	RECT rect;
	GetClientRect(hWnd,&rect);
	hr = m_pVideoWindow->SetWindowPosition(
		0, 0,
		rect.right - rect.left, rect.bottom - rect.top);
	if (!CHECK_RESULT("IVideoWindow SetWindowPosition.",hr)) {return false;}
	*/
	hr = m_pVideoWindow->SetWindowForeground(OATRUE);
	if (!CHECK_RESULT("IVideoWindow SetWindowForeground.",hr)) {return false;}
	hr = m_pVideoWindow->put_Visible(OATRUE);
	if (!CHECK_RESULT("IVideoWindow put_Visible.",hr)) {return false;}

	hr = m_pVideoWindow->put_MessageDrain((OAHWND)hWnd);
	if (!CHECK_RESULT("IVideoWindow put_MessageDrain.",hr)) {return false;}
	return true;
}
//////////////////////////////////////////////////
//�r�f�I�����_���̃E�B���h�E�n���h���̃T�C�Y��ύX����
//////////////////////////////////////////////////
bool CDirectShow::ResizeVideoRendererHWnd(RECT rcClient)
{
	assert(m_pGraphBuilder);
	assert(m_pVideoRenderer);
	assert(m_pVideoWindow);
	HRESULT hr = NOERROR;
	hr = m_pVideoWindow->SetWindowPosition(
		rcClient.left,
		rcClient.top,
		rcClient.right - rcClient.left,
		rcClient.bottom - rcClient.top);
	//�t�B���^�쐬�Ɏ��s���Ă���ꍇ�ɖ����ɂ�����
	//if (!CHECK_RESULT("IVideoWindow SetWindowPosition.",hr)) {return false;}
	return SUCCEEDED(hr);
}
//////////////////////////////////////////////////
//�r�f�I�����_���̕\���E��\����؂�ւ���(�����ł͕\�����[�h�ɂȂ��Ă���)
//////////////////////////////////////////////////
bool CDirectShow::ShowVideoRenderer(BOOL bShow)
{
	assert(m_pGraphBuilder);
	//assert(m_pVideoRenderer);
	assert(m_pVideoWindow);
	HRESULT hr = NOERROR;
	if (bShow)
		hr = m_pVideoWindow->put_Visible(OATRUE);
	else
		hr = m_pVideoWindow->put_Visible(OAFALSE);
	if (!CHECK_RESULT("IVideoWindow put_Visible.",hr)) {return false;}
	return true;
}
//////////////////////////////////////////////////
//�r�f�I�����_���̕\����Ԃ��擾����
//////////////////////////////////////////////////
bool CDirectShow::IsShowVideoRenderer()
{
	assert(m_pGraphBuilder);
	//assert(m_pVideoRenderer);
	if (!m_pVideoWindow)
		return true;
	HRESULT hr = NOERROR;
	LONG lVisible;
	hr = m_pVideoWindow->get_Visible(&lVisible);
	if (!CHECK_RESULT("IVideoWindow get_Visible.",hr)) {return false;}
	if (lVisible == OATRUE)
		return true;
	return false;
}
//////////////////////////////////////////////////
//�r�f�I�����_������E�B���h�E�n���h�����폜����
//////////////////////////////////////////////////
bool CDirectShow::ResetVideoRendererHWnd()
{
	assert(m_pGraphBuilder);
	//assert(m_pVideoRenderer);

	if (!m_pVideoWindow)
		return true;

	HRESULT hr = NOERROR;
	hr = m_pVideoWindow->put_Visible(OAFALSE);
	//TODO �����ŘA���G���[�o�Ă�
	//���s���Ă��C�ɂ��Ȃ��d�l�ɕύX
	//if (!CHECK_RESULT("IVideoWindow put_Visible.",hr)) {return false;}
	hr = m_pVideoWindow->put_Owner(NULL);
	//if (!CHECK_RESULT("IVideoWindow put_Owner.",hr)) {return false;}
	m_pVideoWindow->Release();
	m_pVideoWindow = NULL;
	return true;
}

//////////////////////////////////////////////////
//�R�[���o�b�N�p�̃E�B���h�E�n���h����ݒ�
//////////////////////////////////////////////////
bool CDirectShow::SetCallbackHWnd(HWND hWnd)
{
	assert(m_pGraphBuilder);
	HRESULT hr;
	if (m_pMediaEventEx) {
		m_pMediaEventEx->SetNotifyWindow(NULL, 0, 0);
		SAFE_RELEASE(m_pMediaEventEx)
	}
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaEventEx,
		(LPVOID *)&m_pMediaEventEx);
	if (!CHECK_RESULT("IGraphBuilder Create IMediaEventEx.",hr)) {return false;}
	hr = m_pMediaEventEx->CancelDefaultHandling(EC_REPAINT);
	hr = m_pMediaEventEx->SetNotifyWindow((OAHWND)hWnd,WM_GRAPH_NOTIFY, 0);
	if (!CHECK_RESULT("IMediaEventEx SetNotifyWindow.",hr)) {return false;}
	return true;
}
//////////////////////////////////////////////////
//�Đ��I���܂ő҂�
//////////////////////////////////////////////////
bool CDirectShow::WaitForCompletion(int nTimeOut)
{
	/*
	long eventCode;
 m_pMediaEventEx->WaitForCompletion(nTimeOut, &eventCode);
 switch (eventCode) {
 case 0:
	printf("timeout\n");
	break;
 case EC_COMPLETE:
	printf("complete\n");
	break;
 case EC_ERRORABORT:
	printf("errorabort\n");
	break;
 case EC_USERABORT:
	printf("userabort\n");
	break;
 }
 */
 return true;
}

//////////////////////////////////////////////////
//�R�[���o�b�N
//////////////////////////////////////////////////
LRESULT CDirectShow::OnGraphNotify()
{
	if (!m_pMediaEventEx)
		return NOERROR;
	long evCode;
	LONG_PTR param1,param2;
	// �C�x���g��S�Ď擾
	while (SUCCEEDED(m_pMediaEventEx->GetEvent(&evCode,(LONG_PTR*)&param1, (LONG_PTR*)&param2, 0)))
	{
		m_pMediaEventEx->FreeEventParams(evCode, param1, param2);
		switch (evCode) {
			case EC_COMPLETE:	//�Đ�������Ɋ����������Ƃ�m�点��B
				//MessageBox(NULL,_T("EC_COMPLETE"),NULL,MB_OK);
				break;
			case EC_USERABORT:	//���[�U�[���Đ��𒆒f�������Ƃ�m�点��B���[�U�[���r�f�I �E�B���h�E�����ƁA�r�f�I �����_�������̃C�x���g�𑗐M����B
				break;
			case EC_ERRORABORT:	//�G���[�̂��ߍĐ�����~�������Ƃ�m�点��B
				break;
			case EC_REPAINT:
//#ifdef _DEBUG
//				OutputDebugString(L"EC_REPAINT\n");
//#endif
				break;
		}
	}
	return NOERROR;
}

//////////////////////////////////////////////////
//�T�E���h�����_���̏�����
//////////////////////////////////////////////////
bool CDirectShow::InitAudioRenderer()
{
	assert(m_pGraphBuilder);
	SAFE_RELEASE(m_pAudioRenderer)
	HRESULT hr;
	hr = CoCreateInstance(CLSID_DSoundRender,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void **)&m_pAudioRenderer);
	if (!CHECK_RESULT("DSoundRenderer Create.",hr)) {return false;}
	hr = m_pGraphBuilder->AddFilter(m_pAudioRenderer,_T("DirectSound Renderer"));
	if (!CHECK_RESULT("IGraphBuilder AddFilter DirectSound Renderer.",hr)) {return false;}
	return true;
}

//////////////////////////////////////////////////
//�t�@�C������̃����_�����O���s��
//////////////////////////////////////////////////
bool CDirectShow::RenderFile(BSTR strFilename)
{
	assert(m_pGraphBuilder);
	assert(m_pMediaControl);
	HRESULT hr = m_pMediaControl->RenderFile(strFilename);
	if (!CHECK_RESULT("IMediaControl RenderFile.",hr)) {return false;}
	return true;
}

//////////////////////////////////////////////////
//DirectShow�̔j��
//////////////////////////////////////////////////
void CDirectShow::Exit()
{
	if (m_pMediaControl) {
		if (this->GetState() == State_Running) {
			this->Stop();
		}
	}

	//��N���b�N���폜
	SAFE_RELEASE(m_pReferenceClock);

	//�S�Ẵt�B���^�f�[�^���폜
	for (int i=0;i<(int)m_FilterData.size();i++)
	{
		if (m_pGraphBuilder && m_FilterData[i].pFilter)
			m_pGraphBuilder->RemoveFilter(m_FilterData[i].pFilter);
		SAFE_RELEASE(m_FilterData[i].pFilter)
		m_FilterData[i].strName.clear();
	}
	m_FilterData.clear();

	if (m_pVideoRenderer) {
		m_pGraphBuilder->RemoveFilter(m_pVideoRenderer);
		SAFE_RELEASE(m_pVideoRenderer)
	}
	if (m_pAudioRenderer) {
		m_pGraphBuilder->RemoveFilter(m_pAudioRenderer);
		SAFE_RELEASE(m_pAudioRenderer)
	}

	if (m_pVideoWindow){
		m_pVideoWindow->put_Visible(OAFALSE);
		m_pVideoWindow->put_Owner(NULL);
		SAFE_RELEASE(m_pVideoWindow)
	}
	if (m_pMediaEventEx){
		m_pMediaEventEx->SetNotifyWindow(NULL, 0, 0);
		SAFE_RELEASE(m_pMediaEventEx)
	}
	SAFE_RELEASE(m_pMediaControl);

	//������̃t�B���^���������������
	if (m_pGraphBuilder)
	{
		CComPtr<IEnumFilters> pEnum;
		m_pGraphBuilder->EnumFilters(&pEnum);
		pEnum->Reset();
		CComPtr<IBaseFilter> pFilter;
		ULONG nFetched1 = 0;
		while (pFilter.Release() , pEnum->Next(1,&(pFilter.p),&nFetched1) == S_OK) {
			if (!(pFilter.p)) continue;
			FILTER_INFO Info;
			pFilter->QueryFilterInfo(&Info);
			if (Info.pGraph) {
				SAFE_RELEASE(Info.pGraph);
			}
			m_pGraphBuilder->RemoveFilter(pFilter);
			pEnum->Reset();	//�񋓎q�����Z�b�g
			assert(_T("������̃t�B���^�����o����܂����̂ŁA������܂�") == 0);
		}		
	}
	
	if (m_bRegister)
	{
		CDirectShowEtc::RemoveFromRot(m_dwRegister);
		m_dwRegister = 0x0;
		m_bRegister = false;
	}

	SAFE_RELEASE(m_pCaptureGraphBuilder2)

	//�f�o�b�O�o�͂��~����
#ifdef _DEBUG
	DbgTerminate();
#endif

	SAFE_RELEASE(m_pGraphBuilder)

	if (m_bComInit)
	{
		CoUninitialize();
		m_bComInit = false;
	}
}

//////////////////////////////////////////////////
//�t�B���^�̏�Ԃ�ύX
//////////////////////////////////////////////////
HRESULT CDirectShow::SetState(OAFilterState ofs)
{
	assert(m_pGraphBuilder);
	assert(m_pMediaControl);
	HRESULT hr = NOERROR;
	switch (ofs)
	{
	case State_Running:	//�Đ�
		hr = m_pMediaControl->Run();
		if (hr == S_FALSE)	//�O���t�͎��s�̏��������Ă��邪�A���s��Ԃւ̈ڍs���������Ă��Ȃ��t�B���^������B
		{
			OAFilterState fs;
			hr = m_pMediaControl->GetState(500,&fs);
			switch (hr)
			{
			case S_OK:
				//OutputDebugString(_T("S_OK\r\n"));break;
			case VFW_S_STATE_INTERMEDIATE:
				//OutputDebugString(_T("VFW_S_STATE_INTERMEDIATE\r\n"));break;
			case VFW_S_CANT_CUE:
				//OutputDebugString(_T("VFW_S_CANT_CUE\r\n"));break;
			case E_FAIL:
				//OutputDebugString(_T("E_FAIL\r\n"));break;
			default:
				break;
			}
			if (hr == S_OK && fs == State_Running)
				return hr;
			if (!CHECK_RESULT("IMediaControl Run.",hr)) {return hr;}
		}
		break;
	case State_Paused:	//�ꎞ��~
		hr = m_pMediaControl->Pause();
		if (hr == S_FALSE)
		{
			OAFilterState fs;
			hr = m_pMediaControl->GetState(500,&fs);
			if (hr == S_OK && fs == State_Paused)
				return hr;
			if (!CHECK_RESULT("IMediaControl Pause.",hr)) {return hr;}
		}
		//if (!CHECK_RESULT("IMediaControl Pause.",hr)) {return false;}
		break;
	case State_Stopped:	//��~
		hr = m_pMediaControl->Stop();
		if (hr == S_FALSE)
		{
			OAFilterState fs;
			hr = m_pMediaControl->GetState(500,&fs);
			if (hr == S_OK && fs == State_Stopped)
				return hr;
			if (!CHECK_RESULT("IMediaControl Stop.",hr)) {return hr;}
		}
		//if (!CHECK_RESULT("IMediaControl Stop.",hr)) {return false;}
		break;
	}
	return hr;
}
//////////////////////////////////////////////////
//���݂̍Đ���Ԃ̎擾
//	State_Stopped
//	State_Paused
//	State_Running
//////////////////////////////////////////////////
OAFilterState CDirectShow::GetState()
{
	assert(m_pGraphBuilder);
	assert(m_pMediaControl);
	OAFilterState fs;
	m_pMediaControl->GetState(INFINITE,&fs);
	return fs;
}

/////////////////////////////////////////////////
//�t�B���^�ڑ���ؒf����
//IBaseFilter* pFilter �ؒf����t�B���^
//BOOL bDisconnectInputPins		���̓s����ؒf���邩�ǂ���
//BOOL bDisconnectOutputPins	�o�̓s����ؒf���邩�ǂ���
/////////////////////////////////////////////////
HRESULT CDirectShow::DisconnectFilter(IBaseFilter* pFilter,BOOL bDisconnectInputPins,BOOL bDisconnectOutputPins)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	HRESULT hr = NOERROR;
	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	//�s���̗�
	CComPtr<IPin> pPin;
	while (pPin.Release(), pEnumPins->Next(1, &pPin, &nFetched) == S_OK)
	{
		if (!pPin) continue;

		PIN_DIRECTION PinDir;
		hr = pPin->QueryDirection(&PinDir);
		if (PinDir == PINDIR_INPUT && !bDisconnectInputPins) {
			continue;
		} else if (PinDir == PINDIR_OUTPUT && !bDisconnectOutputPins) {
			continue;
		}
		CComPtr<IPin> pConnectPin;
		hr = pPin->ConnectedTo(&pConnectPin);
		if (SUCCEEDED(hr) && (pConnectPin.p))
		{
			hr = m_pGraphBuilder->Disconnect(pConnectPin.p);
			hr = m_pGraphBuilder->Disconnect(pPin.p);
		}
	}
	return hr;
}
/////////////////////////////////////////////////
//�t�B���^���1�̃s����ؒf����
//���̏ꍇ�A�ڑ��̉ۂ̂�����炸�ؒf�����̂ł����S_OK��Ԃ�
//IBaseFilter* pFilter �ؒf����t�B���^

/////////////////////////////////////////////////
HRESULT CDirectShow::DisconnectFilterPin(IBaseFilter* pFilter,int nPinIndex)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	HRESULT hr = NOERROR;
	
	CComPtr<IPin> pPin;
	pPin.Attach(CDirectShowEtc::GetPinByIndex(pFilter,nPinIndex));

	CComPtr<IPin> pConnectPin;
	hr = pPin->ConnectedTo(&(pConnectPin.p));
	if (SUCCEEDED(hr) && pConnectPin.p)
	{
		hr = m_pGraphBuilder->Disconnect(pConnectPin.p);	//�ڑ���̐ؒf
		hr = m_pGraphBuilder->Disconnect(pPin.p);			//�ڑ����̐ؒf
	}
	pPin.Release();
	pConnectPin.Release();
	return hr;
}
/////////////////////////////////////////////////
//�t�B���^�̏����擾����
//IBaseFilter* pFilter �����擾����t�B���^
//vector<PinInfoStruct>&PinInfoList �s����񂪓�����PinInfoStruct�\���̂̔z��
//pInputPinCount ���̓s���̌�(NULL����)
//pOutputPinCount �o�̓s���̌�(NULL����)
/////////////////////////////////////////////////
HRESULT CDirectShow::GetFilterInfo(IBaseFilter* pFilter,vector<PinInfoStruct>&PinInfoList,int* pInputPinCount,int* pOutputPinCount)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	HRESULT hr = NOERROR;
	if (pInputPinCount)
		(*pInputPinCount) = 0;
	if (pOutputPinCount)
		(*pOutputPinCount) = 0;
	PinInfoList.clear();
	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	IPin* pPin = NULL;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) {
		if (!pPin) continue;

		PinInfoStruct pis;

		//�s���̕������擾
		PIN_DIRECTION direction;
		hr = pPin->QueryDirection(&direction);
		if (!CHECK_RESULT("IPin QueryDirection.",hr)) {pPin->Release(); pPin = NULL; return hr;}
		pis.PinDir = direction;
		if (direction ==  PINDIR_INPUT){
			if (pInputPinCount)
				(*pInputPinCount)++;
		} else {
			if (pOutputPinCount)
				(*pOutputPinCount)++;
		}

		//�s���̖��O�̎擾
		PIN_INFO info;
		hr = pPin->QueryPinInfo(&info);
		if (!CHECK_RESULT("IPin QueryPinInfo.",hr)) {pPin->Release(); pPin = NULL; return hr;}
		//pis.strPinName = reinterpret_cast<LPWSTR>(info.achName);
		pis.strPinName = info.achName;
		if (info.pFilter) {
			info.pFilter->Release();
			info.pFilter = NULL;
		}
		//�s�����ڑ�����Ă��邩�ǂ����`�F�b�N
		IPin* pConnectToPin = NULL;
		hr = pPin->ConnectedTo(&pConnectToPin);
		if (hr == VFW_E_NOT_CONNECTED) {	//�ڑ�����Ă��Ȃ�
			pis.bConnected = FALSE;
		} else if (hr == S_OK) {	//�ڑ�����Ă���
			pis.bConnected = TRUE;
			pConnectToPin->Release();
			pConnectToPin = NULL;
		} else {	//���̑��̃G���[
			if (!CHECK_RESULT("IPin ConnectedTo.",hr)) {pPin->Release(); pPin = NULL; return hr;}
		}
		//�s������ǋL
		PinInfoList.push_back(pis);
		pPin->Release();
		pPin = NULL;
	}
	return S_OK;
}
/////////////////////////////////////////////////
//���́E�o�̓s���̃s���ԍ����擾����
//(���́E�o�̓s�����P�����Ȃ��ꍇ)
/////////////////////////////////////////////////
HRESULT CDirectShow::GetPinIndex(IBaseFilter* pFilter,int &nPinIndex,PIN_DIRECTION pin_dir)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	nPinIndex = -1;
	HRESULT hr = NOERROR;
	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	IPin* pPin = NULL;
	int nIndex = 0;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) {
		if (!pPin) continue;
		//�s���̕������擾
		PIN_DIRECTION direction;
		hr = pPin->QueryDirection(&direction);
		if (!CHECK_RESULT("IPin QueryDirection.",hr)) {pPin->Release(); pPin = NULL; return hr;}
		if (direction == pin_dir)
		{
			nPinIndex = nIndex;
			pPin->Release();
			break;
		}
		nIndex++;
		pPin->Release();
		pPin = NULL;
	}
	if (nPinIndex == -1)
		return E_FAIL;
	return hr;
}

/////////////////////////////////////////////////
//�s���̃��f�B�A�^�C�v�̗񋓂��s��
//�ȑO��SetCaptureMediaType()���������Ă����ꍇ�A1������MediaTypeList���Ԃ�
//MediaTypeList�͎擾������ReleaseMediaTypeList()���Ă�������
//IBaseFilter* pFilter	�����擾����t�B���^
//int nPinIndex			�s���̔ԍ�
//vector<AM_MEDIA_TYPE> &MediaTypeList �񋓂��ꂽAM_MEDIA_TYPE�\���̂̔z��(�߂�l)
/////////////////////////////////////////////////
HRESULT CDirectShow::EnumMediaType(IBaseFilter* pFilter,int nPinIndex,vector<AM_MEDIA_TYPE> &MediaTypeList)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;
	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	IPin* pPin = NULL;
	int nIndex = 0;
	BOOL bFind = FALSE;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) {
		if (!pPin) continue;
		
		if (nIndex == nPinIndex)
		{
			CComPtr<IEnumMediaTypes> pEnumMediaType;
			hr = pPin->EnumMediaTypes(&pEnumMediaType);
			if (!CHECK_RESULT("IPin EnumMediaTypes.",hr)){return hr;}
			hr = pEnumMediaType->Reset();
			if (!CHECK_RESULT("IPin Reset.",hr)){return hr;}
			AM_MEDIA_TYPE *amt = NULL;
			while (pEnumMediaType->Next(1,&amt,NULL) == S_OK) {
				MediaTypeList.push_back(*amt);
			}
			bFind = TRUE;
		}
		pPin->Release();
		pPin = NULL;
		nIndex++;
		if (bFind)
			break;
	}
	return hr;
}

/////////////////////////////////////////////////
//���ݐڑ�����Ă���s���̃��f�B�A�^�C�v���擾
//�s�����ڑ�����Ă��Ȃ��ꍇ�AVFW_E_NOT_CONNECTED���Ԃ�
//AM_MEDIA_TYPE�͎擾������ReleaseMediaType()���Ă�������
/////////////////////////////////////////////////
HRESULT CDirectShow::GetConnectionMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE &amt)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;

	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	IPin* pPin = NULL;
	int nIndex = 0;
	BOOL bFind = FALSE;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) {
		if (!pPin) continue;
		
		if (nIndex == nPinIndex)
		{
			hr = pPin->ConnectionMediaType(&amt);
			bFind = TRUE;
		}
		pPin->Release();
		pPin = NULL;
		nIndex++;
		if (bFind)
			break;
	}
	return hr;
}

/////////////////////////////////////////////////
//�t�B���^���m��ڑ�����(�R���o�[�g�ڑ�)
//IBaseFilter* pSrc		�ڑ����̃t�B���^
//IBaseFilter* pDest	�ڑ���̃t�B���^
//AM_MEDIA_TYPE* pAmt �ڑ�����D�惁�f�B�A�^�C�v(NULL����,NULL�̏ꍇ�͗D�惁�f�B�A�^�C�v���w�肵�Ȃ�)
//LPCWSTR pSrcPinName	�ڑ����̃t�B���^�̏o�̓s����(NULL����)
//int* pID				�C���e���W�F���g�ڑ��ɂ����āA�V���ɒǉ����ꂽ�t�B���^�����݂���ꍇ�A�t�B���^ID��Ԃ�
/////////////////////////////////////////////////
HRESULT CDirectShow::ConnectFilterConvert(IBaseFilter* pSrc,IBaseFilter* pDest,AM_MEDIA_TYPE* pAmt,LPCWSTR pSrcPinName,int* pID)
{
	if (pID)
		*pID = -1;
	assert(m_pGraphBuilder);
	assert(m_pCaptureGraphBuilder2);
	assert(pSrc);
	HRESULT hr = NOERROR;

	if (pSrcPinName) {
		CComPtr<IPin> pOutPin;
		hr = GetPin(pSrc,PINDIR_OUTPUT,pOutPin,pAmt,pSrcPinName);
		if (!CHECK_RESULT("IBaseFilter GetPin PINDIR_OUTPUT.",hr)) {return hr;}
		assert(pOutPin);
		hr = m_pCaptureGraphBuilder2->RenderStream(NULL,((pAmt)?&(pAmt->majortype):NULL),pOutPin,NULL,pDest);
	} else {
		hr = m_pCaptureGraphBuilder2->RenderStream(NULL,((pAmt)?&(pAmt->majortype):NULL),pSrc,NULL,pDest);
	}

	if (FAILED(hr)) {	//�C���e���W�F���g�ڑ��Ɏ��s���Ă���
		return hr;}
	if (!CHECK_RESULT("ICaptureGraphBuilder2 RenderStream.",hr)) {return hr;}

	//�C���e���W�F���g�ڑ��Œǉ����ꂽ�t�B���^�O���t����������
	CComPtr<IPin> pOutPin;
	bool bOut;
	hr = GetPin(pSrc,PINDIR_OUTPUT,pOutPin,pAmt,pSrcPinName);
	CComPtr<IEnumFilters> pEnum;
	hr = m_pGraphBuilder->EnumFilters(&pEnum);
	hr = pEnum->Reset();
	IBaseFilter *pFilter = NULL;
	ULONG nFetched1 = 0;
	while (pEnum->Next(1,&pFilter,&nFetched1) == S_OK) {
		if (!pFilter) continue;
		bOut = false;
		//�s�����擾
		CComPtr<IEnumPins> pEnumPins;
		hr = pFilter->EnumPins(&pEnumPins);
		hr = pEnumPins->Reset();
		IPin* pPin = NULL;
		ULONG nFetched2 = 0;
		while (pEnumPins->Next(1,&pPin,&nFetched2) == S_OK)
		{
			if (!pPin) continue;
			IPin* pConnectedPin = NULL;
			hr = pPin->ConnectedTo(&pConnectedPin);	//�ڑ�����擾
			if (pConnectedPin == pOutPin)
				bOut = true;
			if (pConnectedPin) {
				pConnectedPin->Release();
				pConnectedPin = NULL;
			}
			pPin->Release();
			pPin = NULL;
		}
		if (bOut && pFilter != pSrc && pFilter != pDest)
		{
			//FILTER_INFO Info;
			//hr = pFilter->QueryFilterInfo(&Info);
			//Info.pGraph->Release();
			//�t�B���^���t�B���^���X�g�ɒǉ�
			this->AddFilterList(pFilter,pID);
			return hr;	//�Ǘ��t�B���^�ɒu�����߂�Release()���Ȃ�
		}

		pFilter->Release();
		pFilter = NULL;
	}

	return hr;
}
/////////////////////////////////////////////////
//�t�B���^���m��ڑ�����(�_�C���N�g�ڑ�)
//IBaseFilter* pSrc		�ڑ����̃t�B���^
//IBaseFilter* pDest	�ڑ���̃t�B���^
//AM_MEDIA_TYPE* pAmt �ڑ�����D�惁�f�B�A�^�C�v(NULL����,NULL�̏ꍇ�͗D�惁�f�B�A�^�C�v���w�肵�Ȃ�)
//LPCWSTR pSrcPinName	�ڑ����̃t�B���^�̏o�̓s����(NULL����)
//LPCWSTR pDestPinName	�ڑ���̃t�B���^�̓��̓s����(NULL����)
/////////////////////////////////////////////////
HRESULT CDirectShow::ConnectFilterDirect(IBaseFilter* pSrc,IBaseFilter* pDest,AM_MEDIA_TYPE* pAmt,LPCWSTR pSrcPinName,LPCWSTR pDestPinName)
{
	assert(m_pGraphBuilder);
	assert(pSrc);
	assert(pDest);
	HRESULT hr = NOERROR;

	/*
	//TODO ���������Ɖ��̂�VMR�Ōq����Ȃ����ۂ���Ȃ̂ŗv����
	IPin *pInputPin,*pOutputPin;
	pInputPin = CDirectShowEtc::GetPinByName(pSrc,PINDIR_OUTPUT,pSrcPinName,pAmt);
	assert(pInputPin);
	pOutputPin = CDirectShowEtc::GetPinByName(pDest,PINDIR_INPUT,pDestPinName,NULL);
	assert(pOutputPin);
	hr = m_pGraphBuilder->ConnectDirect(pOutputPin,pInputPin,pAmt);
	pInputPin->Release();
	pOutputPin->Release();
	*/

	CComPtr<IPin> pOutPin,pInPin;
	hr = GetPin(pSrc,PINDIR_OUTPUT,pOutPin,pAmt,pSrcPinName);
	if (!CHECK_RESULT("IBaseFilter GetPin PINDIR_OUTPUT.",hr)) {return hr;}
	assert(pOutPin.p);
	hr = GetPin(pDest,PINDIR_INPUT,pInPin ,NULL,pDestPinName);
	if (!CHECK_RESULT("IBaseFilter GetPin PINDIR_INPUT.",hr)) {return hr;}
	assert(pInPin.p);
	hr = m_pGraphBuilder->ConnectDirect(pOutPin,pInPin,pAmt);
	pOutPin.Release();
	pInPin.Release();

	return hr;	//�ڑ��ł������ǂ�����hr�Ŕ��f����
}
/////////////////////////////////////////////////
//�t�B���^����s�����擾����T�u���W���[��
//IBaseFilter* pFilter			�擾�ΏۂƂȂ�t�B���^
//PIN_DIRECTION pin_direction	�s���̕���(PINDIR_OUTPUT��PINDIR_INPUT)
//CComPtr<IPin> &pin			�擾���ꂽ�s�����Ԃ�(�߂�l)
//AM_MEDIA_TYPE* pAmt			���f�B�A�^�C�v�̏���
//LPCWSTR pPinName				���������s���̖��O
/////////////////////////////////////////////////
HRESULT CDirectShow::GetPin(IBaseFilter* pFilter,PIN_DIRECTION pin_direction,CComPtr<IPin> &pin,AM_MEDIA_TYPE* pAmt,LPCWSTR pPinName)
{
	assert(pFilter);
	HRESULT hr = NOERROR;
	CComPtr<IEnumPins> pEnumPins;
	ULONG nFetched1 = 0;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}

	//�s���̗�
	while (pin.Release(),pEnumPins->Next(1, &pin.p, &nFetched1) == S_OK)
	{
		if (!pin) continue;

		//���o��Pin�̂ǂ��炩�擾
		PIN_DIRECTION this_direction;
		hr = pin->QueryDirection(&this_direction);
		if (!CHECK_RESULT("IPin QueryDirection.",hr)) {return hr;}
		if (this_direction != pin_direction)	//���o��pin���Ⴄ
			continue;

		//�s�����w�肪����ꍇ�̓s�����̃`�F�b�N���s��
		if (pPinName != NULL) {
			PIN_INFO PinInfo;
			hr = pin->QueryPinInfo(&PinInfo);
			if (PinInfo.pFilter)
				PinInfo.pFilter->Release();
			if (_tcsnccmp(pPinName, PinInfo.achName, _tcslen(pPinName)) != 0)
				continue;
		}
		
		//���f�B�A�^�C�v���w�肳��Ă��Ȃ��ꍇ�͂��̃s���Ō��肷��
		if (!pAmt) {
			return S_OK;
		}
		//���f�B�A�^�C�v�̎󂯓�����m�F����
		//http://msdn.microsoft.com/ja-jp/library/cc369447.aspx
		hr = pin->QueryAccept(pAmt);
		if (hr == S_OK) {
			return S_OK;
		} else {
			continue;
		}
	}
	//���s�̏ꍇ
	pin.Release();
	return E_FAIL;
}


/////////////////////////////////////////////////
//�L���v�`���f�o�C�X���̗�
//vector<wstring> strDeviceStrings	�f�o�C�X������̗�
//bVideo							�r�f�I or �I�[�f�B�I
//bName								���O(�t�����h���l�[��) or ID(�f�o�C�X�p�X)
//////////////////////////////////////////////////
bool CDirectShow::EnumCaptureDevice(vector<wstring>& strDeviceStrings,bool bVideo,bool bName)
{
	HRESULT hr;
	strDeviceStrings.clear();

	CComPtr<ICreateDevEnum> pCreateDevEnum;
	CComPtr<IEnumMoniker> pEnumMoniker;
	IMoniker *pMoniker = NULL;
	ULONG nFetched = 0;

	//�f�o�C�X��񋓂��邽�߂�CreateDevEnum���쐬����
	hr = CoCreateInstance(CLSID_SystemDeviceEnum,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,
		(PVOID *)&pCreateDevEnum);
	if (!CHECK_RESULT("ICreateDevEnum Create.",hr)) {return false;}

	//VideoInputDevice��񋓂��邽�߂�EnumMoniker�𐶐�
	if (bVideo) {
		hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
			&pEnumMoniker, 0);
	} else {
		hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,
			&pEnumMoniker, 0);
	}
	if (!CHECK_RESULT("ICreateDevEnum CreateClassEnumerator.",hr)) {return false;}
	if (pEnumMoniker == NULL)
	{
		return true;			//�񋓂ł���L���v�`���f�o�C�X���Ȃ�
	}

	// EnumMoniker��Reset����BReset����ƍŏ����琔���Ȃ���
	hr = pEnumMoniker->Reset();
	if (!CHECK_RESULT("IEnumMoniker Reset.",hr)) {return false;}
	while (pEnumMoniker->Next(1, &pMoniker, &nFetched) == S_OK) {
		if (!pMoniker) continue;
		CComPtr<IPropertyBag> pPropertyBag;
		// IPropertyBag��bind����
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
			(void **)&pPropertyBag);
		//if (!CHECK_RESULT("IMoniker BindToStorage.",hr)) {return false;}
		if (FAILED(hr)) {
			pMoniker->Release(); pMoniker = NULL;
			continue;
		}
		VARIANT var;
		VariantInit(&var);
		var.vt = VT_BSTR;
		if (bName) {
			// FriendlyName���擾
			hr = pPropertyBag->Read(L"FriendlyName", &var, 0);
			//if (!CHECK_RESULT("IPropertyBag Read FriendlyName.",hr)) {return false;}
			//this->OutputDebugWindow(_T("Device FriendlyName:%s\n"),var.bstrVal);
		} else {
			// DevicePath���擾
			hr = pPropertyBag->Read(L"DevicePath", &var, 0);
			//if (!CHECK_RESULT("IPropertyBag Read DevicePath.",hr)) {return false;}
			//this->OutputDebugWindow(_T("Device FriendlyName:%s\n"),var.bstrVal);
		}
		if (FAILED(hr)) {
			VariantClear(&var);
			pMoniker->Release();
			pMoniker = NULL;
			continue;
		}
		//�������ǋL
		strDeviceStrings.push_back(var.bstrVal);

		VariantClear(&var);
		pMoniker->Release();
		pMoniker = NULL;
	}
	if (pMoniker)
		pMoniker->Release();
	return true;
}

//////////////////////////////////////////////////
//�L���v�`���f�o�C�X�̒ǉ�
//bVideo �r�f�I or �I�[�f�B�I
//bName	���O(�t�����h���l�[��) or ID(�f�o�C�X�p�X)
//////////////////////////////////////////////////
bool CDirectShow::AddCaptureDevice(wstring strDeviceName,bool bVideo,bool bName,int* pID)
{
	assert(m_pGraphBuilder);

	HRESULT hr = NOERROR;
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	CComPtr<IEnumMoniker> pEnumMoniker;
	IMoniker *pMoniker = NULL;
	ULONG nFetched = 0;
	//�f�o�C�X��񋓂��邽�߂�CreateDevEnum���쐬����
	hr = CoCreateInstance(CLSID_SystemDeviceEnum,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,
		(PVOID *)&pCreateDevEnum);
	if (!CHECK_RESULT("ICreateDevEnum Create.",hr)) {return false;}

	//VideoInputDevice��񋓂��邽�߂�EnumMoniker�𐶐�
	if (bVideo) {
		hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
			&pEnumMoniker, 0);
	} else {
		hr = pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory,
			&pEnumMoniker, 0);
	}
	if (!CHECK_RESULT("ICreateDevEnum CreateClassEnumerator.",hr)) {return false;}
	if (pEnumMoniker == NULL) {
		return false;			//�񋓂ł���L���v�`���f�o�C�X���Ȃ�
	}
	// EnumMoniker��Reset����
	hr = pEnumMoniker->Reset();
	if (!CHECK_RESULT("IEnumMoniker Reset.",hr)) {return false;}
	bool bFindFlag = false;
	while (pEnumMoniker->Next(1, &pMoniker, &nFetched) == S_OK)
	{
		if (!pMoniker) continue;

		CComPtr<IPropertyBag> pPropertyBag;
		// IPropertyBag��bind����
		hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
			(void **)&pPropertyBag);
		//if (!CHECK_RESULT("IMoniker BindToStorage.",hr)) {return false;}
		if (FAILED(hr)) {
			pMoniker->Release(); pMoniker = NULL;
			continue;
		}
		VARIANT var;
		VariantInit(&var);
		var.vt = VT_BSTR;
		if (bName) {	// FriendlyName���擾
			hr = pPropertyBag->Read(L"FriendlyName", &var, 0);
			//if (!CHECK_RESULT("IPropertyBag Read FriendlyName.",hr)) {return false;}
		} else {		// Device Path(ID)���擾
			hr = pPropertyBag->Read(L"DevicePath", &var, 0);
			//if (!CHECK_RESULT("IPropertyBag Read DevicePath.",hr)) {return false;}
		}
		if (FAILED(hr)) {
			VariantClear(&var);
			pMoniker->Release(); pMoniker = NULL;
			continue;
		}

		if ( _tcscmp(var.bstrVal,strDeviceName.c_str()) == 0)
			bFindFlag = true;
		else
			bFindFlag = false;
		VariantClear(&var);

		if (bFindFlag == true)
		{
			IBaseFilter *pCap = NULL;
			VARIANT name;
			VariantInit(&name);
			name.vt = VT_BSTR;
			hr = pPropertyBag->Read(L"FriendlyName", &name, 0);

			//�L���v�`���f�o�C�X�̃o�C���h
			hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)&pCap);
			if (!CHECK_RESULT("IMoniker BindToObject.",hr)) {return false;}

			//�L���v�`���t�B���^�̒ǉ�
			hr = m_pGraphBuilder->AddFilter(pCap, name.bstrVal);
			if (!CHECK_RESULT("IGraphBuilder AddFilter.",hr)) {return false;}

			//�󂢂Ă���vector��T��
			bool bInsert = false;
			for (int i=0;i<(int)m_FilterData.size();i++) {
				if (!m_FilterData[i].pFilter) {
					//m_FilterData[i].guid = guid;
					m_FilterData[i].pFilter = pCap;
					m_FilterData[i].strName = name.bstrVal;
					*pID = i;
					bInsert = true;
					break;
				}
			}
			//�󂫂��Ȃ��̂Ȃ疖���ɒǉ�
			if (!bInsert) {
				FilterData Data;
				//Data.guid = guid;
				Data.pFilter = pCap;
				Data.strName = name.bstrVal;
				*pID = (int)m_FilterData.size();
				m_FilterData.push_back(Data);
			}
			VariantClear(&name);
		}

		pMoniker->Release();
		if (bFindFlag == TRUE)
			break;
	}
	return bFindFlag;
}
//////////////////////////////////////////////////
//�L���v�`���f�o�C�X�̃��f�B�A�^�C�v���擾��ݒ���s��
//IBaseFilter* pFilter �񋓂���t�B���^
//int nPinIndex �s���̃C���f�b�N�X
//BOOL bSet		�ݒ肷�邩�ۂ�
//AM_MEDIA_TYPE* pAmt �ݒ�/�擾���郁�f�B�A�^�C�v (NULL���w�肷��ƋK��̃t�H�[�}�b�g�ŏ����������)
//////////////////////////////////////////////////
HRESULT CDirectShow::GetSetCaptureMediaType(IBaseFilter* pFilter,int nPinIndex,BOOL bSet,AM_MEDIA_TYPE* pAmt)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	//assert(pAmt);
	HRESULT hr = NOERROR;

	CComPtr<IPin> pPin;
	pPin.Attach(CDirectShowEtc::GetPinByIndex(pFilter,nPinIndex));
	if (!pPin.p)
		return E_FAIL;

	CComPtr<IAMStreamConfig> pAMStreamConfig;
	hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pAMStreamConfig);
	if (FAILED(hr))
		return hr;
	if (bSet)
		hr = pAMStreamConfig->SetFormat(pAmt);
	else
		hr = pAMStreamConfig->GetFormat(&pAmt);
	if (FAILED(hr))
		return hr;
	return hr;
}


//////////////////////////////////////////////////
//�r�f�I���k�Ɋւ�����𓾂�(�e�X�g�R�[�h)
//////////////////////////////////////////////////
HRESULT CDirectShow::GetVideoCompressInfo(IBaseFilter* pFilter,int nPinIndex)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;
	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = pFilter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	IPin* pPin = NULL;
	int nIndex = 0;
	BOOL bFind = FALSE;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) {
		if (!pPin) continue;
		
		if (nIndex == nPinIndex)
		{
			CComPtr<IAMVideoCompression> pAMVideoCompression;
			hr = pPin->QueryInterface(IID_IAMVideoCompression,(void**)&pAMVideoCompression);
			/*
			CComPtr<IEnumMediaTypes> pEnumMediaType;
			hr = pPin->EnumMediaTypes(&pEnumMediaType);
			if (!CHECK_RESULT("IPin EnumMediaTypes.",hr)){return hr;}
			hr = pEnumMediaType->Reset();
			if (!CHECK_RESULT("IPin Reset.",hr)){return hr;}
			AM_MEDIA_TYPE *amt = NULL;
			while (pEnumMediaType->Next(1,&amt,NULL) == S_OK) {
				MediaTypeList.push_back(*amt);
			}
			*/
			bFind = TRUE;
		}
		pPin->Release();
		pPin = NULL;
		nIndex++;
		if (bFind)
			break;
	}
	return hr;
}

//////////////////////////////////////////////////
//�Ǘ������t�B���^�Ƀt�B���^�̒ǉ�
//////////////////////////////////////////////////
HRESULT CDirectShow::AddFilterList(IBaseFilter* pFilter,int* pID)
{
	HRESULT hr = NOERROR;
	GUID guid;
	hr = pFilter->GetClassID(&guid);
	FILTER_INFO Info;
	hr = pFilter->QueryFilterInfo(&Info);
	if (Info.pGraph != NULL)
	{
		Info.pGraph->Release();
	}

	//�󂢂Ă���vector��T��
	for (int i=0;i<(int)m_FilterData.size();i++) {
		if (!m_FilterData[i].pFilter) {
			m_FilterData[i].guid = guid;
			m_FilterData[i].pFilter = pFilter;
			m_FilterData[i].strName = Info.achName;
			if (pID)
				*pID = i;
			return S_OK;
		}
	}
	//�󂫂��Ȃ��̂Ȃ疖���ɒǉ�
	FilterData Data;
	Data.guid = guid;
	Data.pFilter = pFilter;
	Data.strName = Info.achName;
	if (pID)
		*pID = (int)m_FilterData.size();
	m_FilterData.push_back(Data);
	return S_OK;
}

//////////////////////////////////////////////////
//�t�B���^�̒ǉ�
//////////////////////////////////////////////////
HRESULT CDirectShow::AddFilter(GUID guid,LPCWSTR strName,int* pID)
{
	assert(m_pGraphBuilder);
	HRESULT hr = NOERROR;
	IBaseFilter* pFilter = NULL;

	hr = CoCreateInstance(guid,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void **)&pFilter);
	if (SUCCEEDED(hr))
	{
		if (!CHECK_RESULT("AddCreateFilter.",hr)) {return hr;}
		hr = m_pGraphBuilder->AddFilter(pFilter, strName);
		if (!CHECK_RESULT("IGraph Builder AddFilter.",hr)) {return hr;}
		hr = AddFilterList(pFilter,pID);
		return hr;
	}
	
	//�ꕔ�̃t�B���^��CoCreateInstance()�ł͐����ł��Ȃ��̂ŁA
	//System Device Enumerator�܂��̓t�B���^�}�b�p�[���g���Đ������Ȃ��Ă͂Ȃ�Ȃ�
	//http://msdn.microsoft.com/ja-jp/library/cc371168.aspx
	CComPtr<ICreateDevEnum> pCreateDevEnum;
	hr = CoCreateInstance(CLSID_SystemDeviceEnum,NULL,CLSCTX_INPROC_SERVER,
		IID_ICreateDevEnum,(void**)&pCreateDevEnum);
	if (FAILED(hr))
		return hr;
	CComPtr<IEnumMoniker> pEnumMoniker;
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_LegacyAmFilterCategory,&pEnumMoniker,0);
	if (hr != S_OK)
	{
		CHECK_RESULT("ICreateDevEnum CreateClassEnumerator.",hr);
		return hr;
	}
	CComPtr<IMoniker> pMoniker;
	ULONG cFetched;
	while (pEnumMoniker->Next(1,&(pMoniker.p),&cFetched) == S_OK)
	{
		CComPtr<IPropertyBag> pPropertyBag;
		GUID comp_guid = GUID_NULL;
		hr = pMoniker->BindToStorage(0,0,IID_IPropertyBag,(void**)&pPropertyBag);
		if (SUCCEEDED(hr))
		{
			VARIANT varCLSID;
			VariantInit(&varCLSID);
			hr = pPropertyBag->Read(L"CLSID",&varCLSID,0);
			if (SUCCEEDED(hr))
			{
				CDirectShowEtc::StringToGUID(varCLSID.bstrVal,comp_guid);
			}
			VariantClear(&varCLSID);
		}
		if (comp_guid == guid)	//GUID����v
		{
			//�t�B���^�̒ǉ�
			hr = pMoniker->BindToObject(NULL,NULL,IID_IBaseFilter,(void**)&pFilter);
			if (!CHECK_RESULT("IMoniker BindToObject IID_IBaseFilter.",hr)) {return hr;}
			hr = m_pGraphBuilder->AddFilter(pFilter, strName);
			if (!CHECK_RESULT("IGraph Builder AddFilter.",hr)) {return hr;}
			hr = AddFilterList(pFilter,pID);
			return hr;
		}

		pMoniker.p->Release();
		pMoniker.p = NULL;
	}
	return hr;
}
//////////////////////////////////////////////////
//DMO�t�B���^�̒ǉ�
//GUID guidCategory DMO�t�B���^�̃J�e�S��
//	guidCategory�ꗗ
//		DMO�̃J�e�S����					DirectShow�̌݊��J�e�S����
//		DMOCATEGORY_AUDIO_ENCODER		CLSID_AudioCompressorCategory
//		DMOCATEGORY_AUDIO_DECODER		CLSID_LegacyAmFilterCategory
//		DMOCATEGORY_AUDIO_EFFECT
//		DMOCATEGORY_VIDEO_ENCODER		CLSID_VideoCompressorCategory
//		DMOCATEGORY_VIDEO_DECODER		CLSID_LegacyAmFilterCategory
//		DMOCATEGORY_VIDEO_EFFECT
//		DMOCATEGORY_AUDIO_CAPTURE_EFFECT
//////////////////////////////////////////////////
HRESULT CDirectShow::AddDMOFilter(GUID guid,GUID guidCategory,LPCWSTR strName,int* pID)
{
	assert(m_pGraphBuilder);
	HRESULT hr = NOERROR;
	IBaseFilter* pFilter = NULL;

	hr = CoCreateInstance(CLSID_DMOWrapperFilter,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void **)&pFilter);
	if (!CHECK_RESULT("AddCreateFilter.",hr)) {return hr;}

	IDMOWrapperFilter* pDMOWrapperFilter;
	hr = pFilter->QueryInterface(
		IID_IDMOWrapperFilter,
		(void **)&pDMOWrapperFilter);
	if (!CHECK_RESULT("IBaseFilter QueryInterface IDMOWrapperFilter.",hr)) {return hr;}
	hr = pDMOWrapperFilter->Init(guid, guidCategory); 
	if (!CHECK_RESULT("IDMOWrapperFilter Init.",hr)) {return hr;}
	SAFE_RELEASE(pDMOWrapperFilter);

	hr = m_pGraphBuilder->AddFilter(pFilter, strName);
	if (!CHECK_RESULT("IGraph Builder AddFilter.",hr)) {return hr;}

	hr = AddFilterList(pFilter,pID);
	return hr;
}


//////////////////////////////////////////////////
//�t�B���^�̍폜
//////////////////////////////////////////////////
bool CDirectShow::RemoveFilter(IBaseFilter* pFilter)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	for (int i=0;i<(int)m_FilterData.size();i++) {
		if (m_FilterData[i].pFilter == pFilter) {
			HRESULT hr = m_pGraphBuilder->RemoveFilter(pFilter);
			if (!CHECK_RESULT("IGraphBuilder RemoveFilter.",hr)) {return false;}
			//m_FilterData[i].guid = {};
			SAFE_RELEASE(m_FilterData[i].pFilter);
			m_FilterData[i].pFilter = NULL;
			m_FilterData[i].strName.clear();
			return true;
		}
	}
	return false;
}
//////////////////////////////////////////////////
//�t�B���^��ID���猟��
//////////////////////////////////////////////////
IBaseFilter* CDirectShow::GetFilterByID(const int nID)
{
	assert(m_pGraphBuilder);
	assert(nID >= 0 && nID < (int)m_FilterData.size());
	assert(m_FilterData[nID].pFilter);
	return m_FilterData[nID].pFilter;
}
//////////////////////////////////////////////////
//�t�B���^�𖼑O���猟��
//////////////////////////////////////////////////
IBaseFilter* CDirectShow::GetFilterByName(LPCWSTR strName)
{
	assert(m_pGraphBuilder);
	for (int i=0;i<(int)m_FilterData.size();i++) {
		if (m_FilterData[i].strName.compare(strName) == 0)
			return m_FilterData[i].pFilter;
	}
	return NULL;
}
//////////////////////////////////////////////////
//�L���v�`���f�o�C�X(�r�f�I)�̃��f�B�A�^�C�v��񋓂���
//IBaseFilter* pFilter �񋓂���t�B���^
//int nPinIndex �s���̃C���f�b�N�X
//vector<AM_MEDIA_TYPE> &MediaTypeList ���f�B�A�^�C�v�̔z�񃊃X�g
//vector<VIDEO_STREAM_CONFIG_CAPS> &VideoCapsList �r�f�I�L���v�`���^�C�v�̔z�񃊃X�g
//////////////////////////////////////////////////
HRESULT CDirectShow::EnumVideoCaptureMediaType(IBaseFilter* pFilter, int nPinIndex, vector<AM_MEDIA_TYPE> &MediaTypeList,vector<VIDEO_STREAM_CONFIG_CAPS> &VideoCapsList)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	CDirectShowEtc::ReleaseMediaTypeList(MediaTypeList);
	MediaTypeList.clear();
	HRESULT hr = NOERROR;

	//�s�����
	CComPtr<IEnumPins> pEnumPins;
	IPin* pPin = NULL;
	ULONG nFetched = 0;
	hr = pFilter->EnumPins(&pEnumPins);
	hr = pEnumPins->Reset();
	int nIndex = 0;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) 
	{
		if (!pPin) continue;
		if (nIndex != nPinIndex) {
			nIndex++;
			pPin->Release();
			continue;
		}

		PIN_DIRECTION this_dir;
		hr = pPin->QueryDirection(&this_dir);
		if (this_dir != PINDIR_OUTPUT) {
			pPin->Release();
			return E_FAIL;	//�o�̓s���łȂ�
		}

		CComPtr<IAMStreamConfig> pAMStreamConfig;
		hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pAMStreamConfig);
		int iCount = -1;	//�񋓂����t�H�[�}�b�g��
		int iSize = -1;		//�\���\���̂̃T�C�Y(�o�C�g�P��)
		hr = pAMStreamConfig->GetNumberOfCapabilities(&iCount,&iSize);
		if (iSize != sizeof(VIDEO_STREAM_CONFIG_CAPS)) {
			continue;
		}
		for (int i=0;i<iCount;i++)
		{
			AM_MEDIA_TYPE* pAmt = NULL;
			VIDEO_STREAM_CONFIG_CAPS VideoStreamConfigCaps;
			hr = pAMStreamConfig->GetStreamCaps(i,&pAmt,(BYTE*)&VideoStreamConfigCaps);
			if (!pAmt) continue;
			MediaTypeList.push_back(*pAmt);
			VideoCapsList.push_back(VideoStreamConfigCaps);
		}
		pPin->Release();
		pPin = NULL;
		return hr;
	}
	return hr;
}
//////////////////////////////////////////////////
//�L���v�`���f�o�C�X(�I�[�f�B�I)�̃��f�B�A�^�C�v��񋓂���
//IBaseFilter* pFilter �񋓂���t�B���^
//int nPinIndex �s���̃C���f�b�N�X
//vector<AM_MEDIA_TYPE> &MediaTypeList ���f�B�A�^�C�v�̔z�񃊃X�g
//vector<VIDEO_STREAM_CONFIG_CAPS> &VideoCapsList �r�f�I�L���v�`���^�C�v�̔z�񃊃X�g
//////////////////////////////////////////////////
HRESULT CDirectShow::EnumAudioCaptureMediaType(IBaseFilter* pFilter, int nPinIndex, vector<AM_MEDIA_TYPE> &MediaTypeList,vector<AUDIO_STREAM_CONFIG_CAPS> &AudioCapsList)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	CDirectShowEtc::ReleaseMediaTypeList(MediaTypeList);
	MediaTypeList.clear();
	HRESULT hr = NOERROR;

	//�s�����
	CComPtr<IEnumPins> pEnumPins;
	IPin* pPin = NULL;
	ULONG nFetched = 0;
	hr = pFilter->EnumPins(&pEnumPins);
	hr = pEnumPins->Reset();
	int nIndex = 0;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) 
	{
		if (!pPin) continue;
		if (nIndex != nPinIndex) {
			nIndex++;
			pPin->Release();
			continue;
		}

		PIN_DIRECTION this_dir;
		hr = pPin->QueryDirection(&this_dir);
		if (this_dir != PINDIR_OUTPUT) {
			pPin->Release();
			return E_FAIL;	//�o�̓s���łȂ�
		}

		CComPtr<IAMStreamConfig> pAMStreamConfig;
		hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pAMStreamConfig);
		if (FAILED(hr)) {
			pPin->Release();
			return E_FAIL;
		}
		int iCount = -1;	//�񋓂����t�H�[�}�b�g��
		int iSize = -1;		//�\���\���̂̃T�C�Y(�o�C�g�P��)
		hr = pAMStreamConfig->GetNumberOfCapabilities(&iCount,&iSize);
		if (iSize != sizeof(AUDIO_STREAM_CONFIG_CAPS)) {
			continue;
		}
		for (int i=0;i<iCount;i++)
		{
			AM_MEDIA_TYPE* pAmt = NULL;
			AUDIO_STREAM_CONFIG_CAPS AudioStreamConfigCaps;
			hr = pAMStreamConfig->GetStreamCaps(i,&pAmt,(BYTE*)&AudioStreamConfigCaps);
			if (!pAmt) continue;
			MediaTypeList.push_back(*pAmt);
			AudioCapsList.push_back(AudioStreamConfigCaps);
		}
		pPin->Release();
		pPin = NULL;
		return hr;
	}
	return hr;
}
//////////////////////////////////////////////////
//�L���v�`���f�o�C�X�̌��݂̃��f�B�A�^�C�v���擾����
//////////////////////////////////////////////////
HRESULT CDirectShow::GetMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE &Amt)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;

	//�s�����
	CComPtr<IEnumPins> pEnumPins;
	IPin* pPin = NULL;
	ULONG nFetched = 0;
	hr = pFilter->EnumPins(&pEnumPins);
	hr = pEnumPins->Reset();
	int nIndex = 0;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) 
	{
		if (!pPin) continue;
		if (nIndex != nPinIndex) {
			nIndex++;
			pPin->Release();
			continue;
		}
		PIN_DIRECTION this_dir;
		hr = pPin->QueryDirection(&this_dir);
		if (this_dir != PINDIR_OUTPUT) {
			pPin->Release();
			return E_FAIL;	//�o�̓s���łȂ�����
		}

		CComPtr<IAMStreamConfig> pAMStreamConfig;
		hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pAMStreamConfig);
		AM_MEDIA_TYPE* pAmt;
		hr = pAMStreamConfig->GetFormat(&pAmt);
		Amt = *pAmt;
		pPin->Release();
		pPin = NULL;
		CHECK_RESULT("IAMStreamConfig SetFormat.",hr);
		return hr;
	}
	return E_FAIL;
}

//////////////////////////////////////////////////
//�L���v�`���f�o�C�X�Ƀ��f�B�A�^�C�v�̐ݒ���s��
//IBaseFilter* pFilter �񋓂���t�B���^
//int nPinIndex �s���̃C���f�b�N�X
//AM_MEDIA_TYPE* pAmt �ݒ肷�郁�f�B�A�^�C�v (NULL���w�肷��ƋK��̃t�H�[�}�b�g�ŏ����������)
//////////////////////////////////////////////////
HRESULT CDirectShow::SetMediaType(IBaseFilter* pFilter,int nPinIndex,AM_MEDIA_TYPE* pAmt)
{
	assert(m_pGraphBuilder);
	assert(pFilter);
	assert(nPinIndex >= 0);
	assert(pAmt);
	HRESULT hr = NOERROR;

	//�s�����
	CComPtr<IEnumPins> pEnumPins;
	IPin* pPin = NULL;
	ULONG nFetched = 0;
	hr = pFilter->EnumPins(&pEnumPins);
	hr = pEnumPins->Reset();
	int nIndex = 0;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) 
	{
		if (!pPin) continue;
		if (nIndex != nPinIndex) {
			nIndex++;
			pPin->Release();
			continue;
		}
		PIN_DIRECTION this_dir;
		hr = pPin->QueryDirection(&this_dir);
		if (this_dir != PINDIR_OUTPUT) {
			pPin->Release();
			return E_FAIL;	//�o�̓s���łȂ�����
		}

		CComPtr<IAMStreamConfig> pAMStreamConfig;
		hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pAMStreamConfig);
		if (!CHECK_RESULT("IPin QueryInterface IAMStreamConfig.",hr)){pPin->Release(); return hr;}
		hr = pAMStreamConfig->SetFormat(pAmt);
		pPin->Release();
		pPin = NULL;
		CHECK_RESULT("IAMStreamConfig SetFormat.",hr);
		return hr;
	}
	return E_FAIL;
}

//////////////////////////////////////////////////
//�v���p�e�B�y�[�W�̕\��
//////////////////////////////////////////////////
HRESULT CDirectShow::ShowPropertyDialog(IBaseFilter *pFilter,HWND hWnd)
{
	assert(m_pGraphBuilder);
	assert(pFilter);

	HRESULT hr;
	ISpecifyPropertyPages *pProp = NULL;
	hr = pFilter->QueryInterface(IID_ISpecifyPropertyPages, (void **)&pProp);
	//�t�B���^���v���p�e�B�y�[�W�������Ă��Ȃ��ꍇ������̂Œ���
	if (SUCCEEDED(hr))
	{
		// �t�B���^�̖��O�� IUnknown �|�C���^���擾����B
		FILTER_INFO FilterInfo;
		hr = pFilter->QueryFilterInfo(&FilterInfo); 
		IUnknown *pFilterUnk;
		pFilter->QueryInterface(IID_IUnknown, (void **)&pFilterUnk);
		// �y�[�W��\������B
		CAUUID caGUID;
		pProp->GetPages(&caGUID);
		pProp->Release();
		OleCreatePropertyFrame(
			hWnd,                   // �e�E�B���h�E�B
			0, 0,                   // �\��ς݁B
			FilterInfo.achName,     // �_�C�A���O �{�b�N�X�̃L���v�V�����B
			1,                      // �I�u�W�F�N�g�� (�t�B���^�̂�)�B
			&pFilterUnk,            // �I�u�W�F�N�g �|�C���^�̔z��B
			caGUID.cElems,          // �v���p�e�B �y�[�W���B
			caGUID.pElems,          // �v���p�e�B �y�[�W CLSID �̔z��B
			0,                      // ���P�[�����ʎq�B
			0, NULL                 // �\��ς݁B
		);
		// �N���[�� �A�b�v�B
		pFilterUnk->Release();
		FilterInfo.pGraph->Release(); 
		CoTaskMemFree(caGUID.pElems);
	}

	return hr;
}

//////////////////////////////////////////////////
//��N���b�N�̐ݒ�
//BOOL bUse	��N���b�N���g�p���邩�ۂ�
//BOOL bDefaultSyncSource	bUse==TRUE�̂Ƃ��A�f�t�H���g�̊�N���b�N���g�����ǂ����H
//BOOL bMakeSystemClock	bUse==TRUE && bDefaultSyncSource==FALSE �̂Ƃ��A�V�X�e����N���b�N�𐶐����Ďg�����ǂ���
//IBaseFilter* pFilter	bUse==TRUE && bDefaultSyncSource==FALSE �̂Ƃ��A�t�B���^�����N���b�N���擾���Ďg�����ǂ����H
//////////////////////////////////////////////////
HRESULT CDirectShow::SetGraphClock(BOOL bUse,BOOL bDefaultSyncSource,BOOL bMakeSystemClock,IBaseFilter* pFilter)
{
	assert(m_pGraphBuilder);
	HRESULT hr = NO_ERROR;
	CComPtr<IMediaFilter> pMediaFilter;
	hr = m_pGraphBuilder->QueryInterface(IID_IMediaFilter,(void**)&pMediaFilter);
	if (!CHECK_RESULT("IGraphBuilder QueryInterface IMediaFilter.",hr)){return hr;}

	if (bUse)		//��N���b�N���g�p����(�t�B���^���m����̊�N���b�N�œ��ꂳ���)
	{
		if (bDefaultSyncSource) {			//�f�t�H���g�̊�N���b�N���g�p����(�����w�肵�Ȃ��ꍇ�̋K��̓���)
			hr = m_pGraphBuilder->SetDefaultSyncSource();
			if (m_pReferenceClock) {
				SAFE_RELEASE(m_pReferenceClock); }
			//�f�t�H���g�Ɍ��肳�ꂽ��N���b�N���擾����
			hr = pMediaFilter->GetSyncSource(&m_pReferenceClock);
		} else {
			SAFE_RELEASE(m_pReferenceClock);
			if (bMakeSystemClock) {			//�V�X�e���N���b�N�𐶐�����(����\1ms���x�̒ᐸ�x�^�C�}�[���g��)
				hr = CoCreateInstance(CLSID_SystemClock,
					NULL,CLSCTX_INPROC_SERVER,
					IID_IReferenceClock,(void**)&m_pReferenceClock); 
				if (FAILED(hr))
					return hr;
			} else {						//�t�B���^�����N���b�N���w�肷��
				if (!pFilter)
					return E_FAIL;
				hr = pFilter->QueryInterface(IID_IReferenceClock,(void**)&m_pReferenceClock);
				if (FAILED(hr))
					return hr;
			}
			//�w��̊�N���b�N��^����
			hr = pMediaFilter->SetSyncSource(m_pReferenceClock);
		}
	} else {
		//��N���b�N���g�p���Ȃ�(�e�t�B���^�̃N���b�N�҂��������Ȃ�̂ōő��̏����ɂȂ�)
		hr = pMediaFilter->SetSyncSource(NULL);
		if (m_pReferenceClock) {
			SAFE_RELEASE(m_pReferenceClock);}
	}
	return hr;
}
//////////////////////////////////////////////////
//��N���b�N���猻�݂̊�^�C�����擾����
//REFERENCE_TIME &rtTime	����\(100�i�m�b�P��)
//////////////////////////////////////////////////
HRESULT CDirectShow::GetGraphClockTime(REFERENCE_TIME &rtTime)
{
	if (!m_pReferenceClock)
		return E_FAIL;
	return m_pReferenceClock->GetTime(&rtTime);
}
//////////////////////////////////////////////////
//��N���b�N�̕���\���擾����
//REFERENCE_TIME &rtResolution	����\(100�i�m�b�P��)
//////////////////////////////////////////////////
HRESULT CDirectShow::GetGraphClockResolution(REFERENCE_TIME &rtResolution)
{
	if (!m_pReferenceClock)
		return E_FAIL;
	HRESULT hr = NOERROR;
	REFERENCE_TIME rtBeginTime;
	REFERENCE_TIME rtEndTime;
	hr = m_pReferenceClock->GetTime(&rtBeginTime);
	if (FAILED(hr))
		return E_FAIL;
	do {
		hr = m_pReferenceClock->GetTime(&rtEndTime);
	} while (SUCCEEDED(hr) && rtBeginTime == rtEndTime);
	rtResolution = rtEndTime - rtBeginTime;
	return hr;
}

//////////////////////////////////////////////////
//�t�B���^�O���t�ɓ����̉ۂ�ݒ肷��B�܂��A�K�v������΍ő�x�����Ԃ�ݒ肷��
//BOOL bUseStreamOffset	������L���ɂ��邩�ǂ���
//REFERENCE_TIME* prtMaxGraphLatency �x�����Ԃ�ݒ肷��(NULL�̏ꍇ�͐ݒ�͖���)
//////////////////////////////////////////////////
HRESULT CDirectShow::SyncStreamOffset(BOOL bUseStreamOffset,REFERENCE_TIME* prtMaxGraphLatency)
{
	assert(m_pGraphBuilder);
	if (!m_pGraphBuilder)
		return E_FAIL;
	HRESULT hr = NOERROR;
	CComPtr<IAMGraphStreams> pAMGraphStreams;
	hr = m_pGraphBuilder->QueryInterface(IID_IAMGraphStreams,(void**)&pAMGraphStreams);
	if (!CHECK_RESULT("IGraphBuilder QueryInterface IAMGraphStreams.",hr)){return hr;}
	hr = pAMGraphStreams->SyncUsingStreamOffset(bUseStreamOffset);
	if (!CHECK_RESULT("IAMGraphStreams SyncUsingStreamOffset.",hr)){return hr;}
	if (bUseStreamOffset && prtMaxGraphLatency)
	{
		hr = pAMGraphStreams->SetMaxGraphLatency(*prtMaxGraphLatency);
		if (!CHECK_RESULT("IAMGraphStreams SetMaxGraphLatency.",hr)){return hr;}
	}
	return hr;
}

//////////////////////////////////////////////////
//�A���P�[�^�[�o�b�t�@�̐ݒ�
//BOOL bSet �l��ݒ肷��ꍇ��TRUE, �l���擾����ꍇ��FALSE
//�I�[�f�B�I�s���̏ꍇ�A�f�t�H���g��500�~���`1000�~���b�̒x��
//�ʏ��80�~���b�̃o�b�t�@�����S�ł͂��邪�A30�`40�~���b�̃o�b�t�@�ł��\���ȏꍇ������
//�o�b�t�@������������Ɖ����͗򉻂���(�v�`�v�`)
//////////////////////////////////////////////////
HRESULT CDirectShow::SetGetAllocatorBuffer(IBaseFilter* pFilter,int nPinIndex,BOOL bSet,ALLOCATOR_PROPERTIES &AllocProp)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;

	CComPtr<IPin> pPin;
	pPin.Attach(CDirectShowEtc::GetPinByIndex(pFilter,nPinIndex));
	if (!pPin.p) return E_FAIL;

	CComPtr<IAMBufferNegotiation> pAMBufferNegotation;
	hr = pPin->QueryInterface(IID_IAMBufferNegotiation,(void**)&pAMBufferNegotation);
	if (FAILED(hr)) {
		return E_FAIL;
	}
	if (bSet) {
		//�A���P�[�^�̐ݒ�̓s���ڑ��O�ł����ł��Ȃ�
		hr = pAMBufferNegotation->SuggestAllocatorProperties(&AllocProp);
	} else {
		//�A���P�[�^�̃`�F�b�N�̓s���ڑ���ł����ł��Ȃ�
		AllocProp.cBuffers  = -1;
		AllocProp.cbBuffer = -1;
		AllocProp.cbAlign = -1;
		AllocProp.cbPrefix = -1;
		hr = pAMBufferNegotation->GetAllocatorProperties(&AllocProp);
	}
	return hr;
}

//////////////////////////////////////////////////
//�t�B���^�J�n�ȍ~�̃t���[�����E�h���b�v���Ȃǂ��擾
//long* plNotDropped	�h���b�v����Ȃ��������t���[����
//long* plDropped		�h���b�v���ꂽ���t���[����
//long* plAverageSize	�t���[���̕��σT�C�Y(�o�C�g�P��)
//////////////////////////////////////////////////
HRESULT CDirectShow::GetDropFrame(IBaseFilter* pFilter,int nPinIndex,long* plNotDropped,long* plDropped,long* plAverageSize)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;

	CComPtr<IPin> pPin;
	pPin.Attach(CDirectShowEtc::GetPinByIndex(pFilter,nPinIndex));
	if (!pPin.p) return E_FAIL;

	CComPtr<IAMDroppedFrames> pAMDroppedFrames;
	hr = pPin->QueryInterface(IID_IAMDroppedFrames,(void**)&pAMDroppedFrames);
	if (FAILED(hr))
		return E_FAIL;	//�C���^�t�F�[�X���擾�ł��Ȃ��ꍇ���قƂ�ǂł���
	//if (!CHECK_RESULT("IPin QueryInterface IAMDroppedFrames.",hr)){return hr;}

	//�X�g���[�~���O�̊J�n�ȍ~�A�t�B���^���z�M�������t���[�������擾����
	if (plNotDropped) {
		hr = pAMDroppedFrames->GetNumNotDropped(plNotDropped);
		if (FAILED(hr))
			(*plNotDropped) = 0;
		//if (!CHECK_RESULT("IAMDroppedFrames GetNumNotDropped.",hr)){return hr;}
	}
	//�X�g���[�~���O�̊J�n�ȍ~�A�t�B���^���h���b�v���������t���[�������擾����
	if (plDropped) {
		hr = pAMDroppedFrames->GetNumDropped(plDropped);
		if (FAILED(hr))
			(*plDropped) = 0;
		//if (!CHECK_RESULT("IAMDroppedFrames GetNumDropped.",hr)){return hr;}
	}
	//�t�B���^���L���v�`�������t���[���̕��σT�C�Y���擾����B 
	if (plAverageSize) {
		hr = pAMDroppedFrames->GetAverageFrameSize(plAverageSize);
		if (FAILED(hr))
			(*plAverageSize) = 0;
		//if (!CHECK_RESULT("IAMDroppedFrames GetAverageFrameSize.",hr)){return hr;}
	}
	return hr;
}

//////////////////////////////////////////////////
//�v�b�V���\�[�X�t�B���^�̐��\�Ə��𓾂�
//REFERENCE_TIME *prtLatency	�O���t�̒x����������^�C������
//ULONG* pFlags AM_PUSHSOURCE_FLAGS�񋓌^�Œ�`�����g�ݍ��킹
//REFERENCE_TIME *prtOffset		���݂̃X�g���[�� �I�t�Z�b�g��������^�C��
//REFERENCE_TIME *prtMaxOffset	�t�B���^���T�|�[�g�ł���ő�I�t�Z�b�g��������^�C��
//////////////////////////////////////////////////
HRESULT CDirectShow::GetPushSourceInfo(IBaseFilter* pFilter,int nPinIndex,REFERENCE_TIME *prtLatency,ULONG* pFlags,REFERENCE_TIME *prtOffset,REFERENCE_TIME *prtMaxOffset)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;

	CComPtr<IPin> pPin;
	pPin.Attach(CDirectShowEtc::GetPinByIndex(pFilter,nPinIndex));
	if (!pPin.p) return E_FAIL;

	CComPtr<IAMPushSource> pAMPushSource;
	hr = pPin->QueryInterface(IID_IAMPushSource,(void**)&pAMPushSource);
	if (FAILED(hr))
		return E_FAIL;	//�C���^�t�F�[�X���擾�ł��Ȃ��ꍇ������ (��:AmaRec Audio Capture)
	//if (!CHECK_RESULT("IPin QueryInterface IAMPushSource.",hr)){return hr;}

	//���̃t�B���^�Ɋ֘A�t�����Ă���A�\�������x�����Ԃ��擾����B 
	if (prtLatency) {
		hr = pAMPushSource->GetLatency(prtLatency);
		if (FAILED(hr))
			(*prtLatency) = 0;
		//if (!CHECK_RESULT("IAMPushSource GetLatency.",hr)){return hr;}
	}
	//�t�B���^�̓����\���t���O�̑g�ݍ��킹���擾����B
	if (pFlags) {
		hr = pAMPushSource->GetPushSourceFlags(pFlags);
		if (FAILED(hr))
			(*pFlags) = 0;
		//if (!CHECK_RESULT("IAMPushSource GetPushSourceFlags.",hr)){return hr;}
	}
	//�^�C�� �X�^���v�𐶐�����Ƃ��Ƀt�B���^���g���I�t�Z�b�g���擾����B
	if (prtOffset) {
		hr = pAMPushSource->GetStreamOffset(prtOffset);
		if (FAILED(hr))
			(*prtOffset) = 0;
		//if (!CHECK_RESULT("IAMPushSource GetStreamOffset.",hr)){return hr;}
	}
	//�t�B���^���T�|�[�g�ł���ő�X�g���[�� �I�t�Z�b�g���擾����B
	if (prtMaxOffset) {
		hr = pAMPushSource->GetMaxStreamOffset(prtMaxOffset);
		if (FAILED(hr))
			(*prtMaxOffset) = 0;
		//if (!CHECK_RESULT("IAMPushSource GetMaxStreamOffset.",hr)){return hr;}
	}
	return hr;
}
//////////////////////////////////////////////////
//���݂̃X�g���[�~���O�����擾
//AM_STREAM_INFO* pAsi	�X�g���[�~���O������
//////////////////////////////////////////////////
HRESULT CDirectShow::GetStreamInfo(IBaseFilter* pFilter,int nPinIndex,AM_STREAM_INFO* pAsi)
{
	assert(pFilter);
	assert(nPinIndex >= 0);
	HRESULT hr = NOERROR;

	CComPtr<IPin> pPin;
	pPin.Attach(CDirectShowEtc::GetPinByIndex(pFilter,nPinIndex));
	if (!pPin.p) return E_FAIL;

	CComPtr<IAMStreamControl> pAMStreamControl;
	hr = pPin->QueryInterface(IID_IAMStreamControl,(void**)&pAMStreamControl);
	if (FAILED(hr))
		return E_FAIL;	//�C���^�t�F�[�X���擾�ł��Ȃ��ꍇ������

	if (pAsi) {
		hr = pAMStreamControl->GetInfo(pAsi);
		return hr;
	}
	return E_FAIL;
}
//////////////////////////////////////////////////
//�L���v�`���t�B���^�̐ڑ�
//IBaseFilter* pFilter	�ǉ����ƂȂ�t�B���^
//BOOL bVideo			�r�f�I�t�B���^���ǂ���
//int* pCrossbar1ID		��1�N���X�o�[�̃t�B���^ID
//int* pCrossbar2ID		��2�N���X�o�[�̃t�B���^ID
//int* pTVAudioID		TV�I�[�f�B�I�̃t�B���^ID		
//int* pTVTunerID		TV�`���[�i�[�̃t�B���^ID
//////////////////////////////////////////////////
HRESULT CDirectShow::AddWDMFilter(IBaseFilter* pFilter,BOOL bVideo,int* pCrossbar1ID,int* pCrossbar2ID,int* pTVAudioID,int* pTVTunerID)
{
	assert(pFilter);
	assert(m_pGraphBuilder);
	assert(m_pCaptureGraphBuilder2);
	assert(pCrossbar1ID);
	assert(pCrossbar2ID);
	assert(pTVAudioID);
	assert(pTVTunerID);
	if (!pFilter) return E_FAIL;
	if (pCrossbar1ID) *pCrossbar1ID = -1;
	if (pCrossbar2ID) *pCrossbar2ID = -1;
	if (pTVAudioID) *pTVAudioID = -1;
	if (pTVTunerID) *pTVTunerID = -1;

	HRESULT hr = NOERROR;

	//�ꎞ�ڑ������p��NULL�����_���̍쐬
	CComPtr<IBaseFilter> pNullRenderer;
	int nNullRendererID = -1;
	GUID guidNullRenderer = {0xC1F400A4,0x3F08,0x11D3,{0x9F,0x0B,0x00,0x60,0x08,0x03,0x9E,0x37}};//CLSID_NullRenderer
	hr = CoCreateInstance(guidNullRenderer,
		NULL,CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,(void **)&pNullRenderer);
	if (SUCCEEDED(hr)) {
		hr = m_pGraphBuilder->AddFilter(pNullRenderer,_T("Null Renderer"));
		if (!CHECK_RESULT("IGraph Builder AddFilter.",hr)) {return hr;}
	}

	//�t�B���^�̐ڑ� (�K�v�ȃt�B���^�������ő}�������)
	hr = m_pCaptureGraphBuilder2->RenderStream(
		bVideo?(&PIN_CATEGORY_PREVIEW):(&PIN_CATEGORY_PREVIEW),/*&PIN_CATEGORY_CAPTURE,*/
		bVideo?(&MEDIATYPE_Video):(&MEDIATYPE_Audio),
		pFilter,
		NULL,
		pNullRenderer);

	//�ꎞ�����p��NULL�����_���̍폜
	m_pGraphBuilder->RemoveFilter(pNullRenderer);

	//�L���v�`���s�������Ȃ��ꍇ�͌���Ɏ�����SmartTee���}������Ă���̂ŁA�폜����
	if (hr == VFW_S_NOPREVIEWPIN)
	{
		CComPtr<IBaseFilter> pInsertFilter;
		hr = m_pCaptureGraphBuilder2->FindInterface(
			&LOOK_DOWNSTREAM_ONLY,	//&LOOK_UPSTREAM_ONLY
			&MEDIATYPE_Video,		//�o�̓s���̃��W���[�^�C�v
			pFilter,
			IID_IBaseFilter,
			(void**)&pInsertFilter);
		if (SUCCEEDED(hr)) {
			hr = m_pGraphBuilder->RemoveFilter(pInsertFilter);
		}
	} else if (FAILED(hr)) {
		return hr;		//RenderStream�Ɏ��s
	}

	//�O���Ɏ����ǉ����ꂽ�t�B���^�Q���Ǘ��t�B���^���ɒu��

	//��1�N���X�o�[
	CComPtr<IAMCrossbar> pAMCrossbar;
	hr = m_pCaptureGraphBuilder2->FindInterface(
		&LOOK_UPSTREAM_ONLY,NULL,pFilter,
		IID_IAMCrossbar,(void**)&pAMCrossbar);
	if (SUCCEEDED(hr)) {
		IBaseFilter* pCrossbarFilter = NULL;
		hr = pAMCrossbar->QueryInterface(IID_IBaseFilter,(void**)&pCrossbarFilter);
		if (SUCCEEDED(hr) && pCrossbarFilter) {
			if (pCrossbar1ID)
				hr = this->AddFilterList(pCrossbarFilter,pCrossbar1ID);

			//��2�N���X�o�[���m�F
			CComPtr<IAMCrossbar> pAMCrossbar2;
			hr = m_pCaptureGraphBuilder2->FindInterface(
				&LOOK_UPSTREAM_ONLY,NULL,pCrossbarFilter,
				IID_IAMCrossbar,(void**)&pAMCrossbar2);
			if (SUCCEEDED(hr))
			{
				IBaseFilter* pCrossbarFilter2 = NULL;
				hr = pAMCrossbar->QueryInterface(IID_IBaseFilter,(void**)&pCrossbarFilter2);
				if (SUCCEEDED(hr) && pCrossbarFilter2) {
					if (pCrossbar2ID)
						hr = this->AddFilterList(pCrossbarFilter2,pCrossbar2ID);
				}
			}

		}
	}

	//TV�I�[�f�B�I
	CComPtr<IAMTVAudio> pAMTVAudio;
	hr = m_pCaptureGraphBuilder2->FindInterface(
		&LOOK_UPSTREAM_ONLY,NULL,pFilter,
		IID_IAMTVAudio,(void**)&pAMTVAudio);
	if (SUCCEEDED(hr)) {
		IBaseFilter* pTVAudioFilter = NULL;
		hr = pAMTVAudio->QueryInterface(IID_IBaseFilter,(void**)&pTVAudioFilter);
		if (SUCCEEDED(hr) && pTVAudioFilter) {
			if (pTVAudioID)
				hr = this->AddFilterList(pTVAudioFilter,pTVAudioID);
		}
	}

	//TV�`���[�i�[
	CComPtr<IAMTVTuner> pAMTVTuner;
	hr = m_pCaptureGraphBuilder2->FindInterface(
		&LOOK_UPSTREAM_ONLY,NULL,pFilter,
		IID_IAMTVTuner,(void**)&pAMTVTuner);
	if (SUCCEEDED(hr)) {
		IBaseFilter* pTVTunerFilter = NULL;
		hr = pAMTVTuner->QueryInterface(IID_IBaseFilter,(void**)&pTVTunerFilter);
		if (SUCCEEDED(hr) && pTVTunerFilter) {
			if (pTVTunerID)
				hr = this->AddFilterList(pTVTunerFilter,pTVTunerID);
		}
	}

	return S_OK;
}

