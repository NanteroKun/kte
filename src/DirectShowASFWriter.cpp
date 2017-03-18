#include "StdAfx.h"
#include "DirectShowASFWriter.h"

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CDirectShowASFWriter::CDirectShowASFWriter(void)
{
	m_pAsfWriter = NULL;
	m_pWMWriterAdvanced3 = NULL;
	m_bIsWriterAdvanced3 = FALSE;
	m_pConfigAsfWriter2 = NULL;
	m_pWMHeaderInfo = NULL;

	m_NetworkSinkList.clear();
	m_PushSinkList.clear();
	m_FileSinkList.clear();

	m_strTitle.clear();
	m_strAuthor.clear();
	m_strDescription.clear();
	m_strRating.clear();
	m_strCopyright.clear();

	m_pNetworkCallback = NULL;
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CDirectShowASFWriter::~CDirectShowASFWriter(void)
{
	//Delete();
}

//////////////////////////////////////////////////
//ASF Writer���쐬����
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::Create(IGraphBuilder* pGraphBuilder,LPCTSTR lpszDefaultFileName)
{
	if (!pGraphBuilder)
		return E_FAIL;
	HRESULT hr = NOERROR;

	if (m_pAsfWriter)
		Delete(pGraphBuilder);

	//WM ASF Writer���쐬
	hr = CoCreateInstance(CLSID_WMAsfWriter,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_IBaseFilter,
		(void **)&m_pAsfWriter);
	if (!CHECK_RESULT("CoCreateInstance CLSID_WMAsfWriter.",hr)) {return hr;}

	hr = pGraphBuilder->AddFilter(m_pAsfWriter, L"WM ASF Writer");
	if (!CHECK_RESULT("IGraphBuilder AddFilter WM ASF Writer.",hr)) {return hr;}

	m_pConfigAsfWriter2 = NULL;
	hr = m_pAsfWriter->QueryInterface(IID_IConfigAsfWriter2, (void**)&m_pConfigAsfWriter2);
	if (!CHECK_RESULT("IWMAsfWriter QueryInterface IConfigAsfWriter2.",hr)) {return hr;}

	//ASFWriter��IFileSinkFilter���p�����Ă���̂ŁA��������̃t�@�C�����K�v
	IFileSinkFilter* pFileSinkFilter = NULL;
	hr = m_pAsfWriter->QueryInterface(IID_IFileSinkFilter,(void**)&pFileSinkFilter);
	if (!CHECK_RESULT("IAsfWriter QueryInterface IFileSinkFilter.",hr)) {return hr;}
	hr = pFileSinkFilter->SetFileName(lpszDefaultFileName,NULL);
	if (!CHECK_RESULT("IFileSinkFilter SetFileName.",hr)) {return hr;}
	pFileSinkFilter->Release();

	//���^�f�[�^�ҏW�p�̃C���^�t�F�[�X���擾
	m_pWMHeaderInfo = NULL;
	hr = m_pAsfWriter->QueryInterface(IID_IWMHeaderInfo,(void**)&m_pWMHeaderInfo);
	if (!CHECK_RESULT("QueryInterface IWMHeaderInfo.",hr)) {return hr;}

	//WM ASF Writer����IServiceProvider���擾
	IServiceProvider *pServiceProvider = NULL;	//�₢���킹�ׂ̈̃T�[�r�X�v���o�C�_
	hr = m_pAsfWriter->QueryInterface(IID_IServiceProvider, (void**)&pServiceProvider);
	if (!CHECK_RESULT("QueryInterface IServiceProvider.",hr)) {return hr;}
	//IServiceProvider����IWMWriterAdvanced2���擾
	m_bIsWriterAdvanced3 = TRUE;
	hr = pServiceProvider->QueryService(IID_IWMWriterAdvanced3,
		IID_IWMWriterAdvanced3, (void**)&m_pWMWriterAdvanced3);
	if (FAILED(hr)) {
		hr = pServiceProvider->QueryService(IID_IWMWriterAdvanced2,
			IID_IWMWriterAdvanced2, (void**)&m_pWMWriterAdvanced3);
		//��������������A���ׂĂ̐ݒ肪�擾�ł���
		//m_bIsWriterAdvanced3 = FALSE;
		if (!CHECK_RESULT("QueryService IWMWriterAdvanced2.",hr)) {return hr;}
	}
	pServiceProvider->Release();	//���̎��_�ŕs�v

	//���̓\�[�X�����C�u�\�[�X�ł��邱�Ƃ��
	hr = m_pWMWriterAdvanced3->SetLiveSource(TRUE);
	
	//���炩���ߑS�Ă�Sink���폜���Ă���
	hr = m_pWMWriterAdvanced3->RemoveSink(NULL);
	DWORD dwSinkCount = 0;
	while (1) {
		hr = m_pWMWriterAdvanced3->GetSinkCount(&dwSinkCount);
		if (!CHECK_RESULT("GetSinkCount.",hr)) {return hr;}
		if (dwSinkCount == 0)
			break;
		IWMWriterSink *pWMWriterSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(0,&pWMWriterSink);
		if (!CHECK_RESULT("GetSink.",hr)) {return hr;}
		hr = m_pWMWriterAdvanced3->RemoveSink(pWMWriterSink);
		if (!CHECK_RESULT("RemoveSink.",hr)) {return hr;}
		pWMWriterSink->Release();
	}

	//�l�b�g���[�N�p�̃R�[���o�b�N���쐬����
	if (m_pNetworkCallback) {
		m_pNetworkCallback->Release();
		delete 	m_pNetworkCallback;
		m_pNetworkCallback = NULL;
	}
	m_pNetworkCallback = new CDirectShowASFWriterNetworkCallback;

	return hr;
}
//////////////////////////////////////////////////
//ASF Writer���폜����
//////////////////////////////////////////////////
void CDirectShowASFWriter::Delete(IGraphBuilder* pGraphBuilder)
{
	assert(pGraphBuilder);
	HRESULT hr = NOERROR;

	//�S�Ă�Sink���폜����
	hr = DeleteAllSink();

	//�w�b�_�����폜
	if (m_pWMHeaderInfo) {
		SAFE_RELEASE(m_pWMHeaderInfo);
	}
	//IWMWriter���폜
	SAFE_RELEASE(m_pWMWriterAdvanced3);

	SAFE_RELEASE(m_pConfigAsfWriter2);

	//�t�B���^�O���t����폜
	if (m_pAsfWriter) {
		hr = pGraphBuilder->RemoveFilter(m_pAsfWriter);
		m_pAsfWriter->Release();
		//delete m_pAsfWriter;
		m_pAsfWriter = NULL;
	}
	return;
}
//////////////////////////////////////////////////
//�S�Ă�Sink���폜����
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::DeleteAllSink()
{
	HRESULT hr = NOERROR;

	//�l�b�g���[�N�V���N���폜����
	for(int i=0;i<(int)m_NetworkSinkList.size();i++)
	{
		if (m_NetworkSinkList[i]) {

			//�R�[���o�b�N�̔j��
			{
				CComPtr<IWMClientConnections> pWMClientConnections;
				CComPtr<IWMRegisterCallback> pWMRegisterCallback;
				hr = m_NetworkSinkList[i]->QueryInterface(IID_IWMClientConnections,(void**)&pWMClientConnections);
				hr = pWMClientConnections->QueryInterface(IID_IWMRegisterCallback,(void**)&pWMRegisterCallback);
				if (m_pNetworkCallback) {
					hr = pWMRegisterCallback->Unadvise(m_pNetworkCallback,NULL);
				}
			}
			hr = m_NetworkSinkList[i]->OnEndWriting();
			hr = m_NetworkSinkList[i]->Close();
			hr = m_NetworkSinkList[i]->Disconnect();
			hr = m_pWMWriterAdvanced3->RemoveSink(m_NetworkSinkList[i]);
			m_NetworkSinkList[i]->Release();
			m_NetworkSinkList[i] = NULL;
		}
	}
	m_NetworkSinkList.clear();

	if (m_pNetworkCallback) {
		m_pNetworkCallback->Release();
		delete 	m_pNetworkCallback;
		m_pNetworkCallback = NULL;
	}

	//�v�b�V���V���N���폜����
	for(int i=0;i<(int)m_PushSinkList.size();i++)
	{
		if (m_PushSinkList[i]) {
			hr = m_PushSinkList[i]->EndSession();
			hr = m_PushSinkList[i]->Disconnect();
			hr = m_pWMWriterAdvanced3->RemoveSink(m_PushSinkList[i]);
			m_PushSinkList[i]->Release();
			m_PushSinkList[i] = NULL;
		}
	}
	m_PushSinkList.clear();

	//�t�@�C���V���N���폜����
	for(int i=0;i<(int)m_FileSinkList.size();i++)
	{
		if (m_FileSinkList[i]) {
			hr = m_FileSinkList[i]->OnEndWriting();
			hr = m_pWMWriterAdvanced3->RemoveSink(m_FileSinkList[i]);
			m_FileSinkList[i]->Release();
			m_FileSinkList[i] = NULL;
		}
	}
	m_FileSinkList.clear();

	//�S�Ă�Sink���폜(�O�̂���)
	if (m_pWMWriterAdvanced3) {
		hr = m_pWMWriterAdvanced3->RemoveSink(NULL);
	}
	return hr;
}

//////////////////////////////////////////////////
//�v���t�@�C���̓K�p
//IWMProfile* pProfile �K�p����G���R�[�h�v���t�@�C��
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::ApplyProfiles(IWMProfile* pProfile)
{
	assert(m_pAsfWriter);
	assert(m_pConfigAsfWriter2);
	assert(pProfile);
	HRESULT hr = NOERROR;
	if (!pProfile) return E_FAIL;

	hr = m_pConfigAsfWriter2->ConfigureFilterUsingProfile(pProfile);
	if (!CHECK_RESULT("IConfigAsfWriter2 ConfigureFilterUsingProfile.",hr)) {return hr;}

	return hr;
}

//////////////////////////////////////////////////
//���݂̃t�B���^�ɒ�`����Ă���o�̓t�H�[�}�b�g���擾����
//AM_MEDIA_TYPE &amt	�擾���ꂽ���f�B�A�^�C�v
//BOOL bVideo			�r�f�I�s�����擾���邩�ǂ���
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::GetFormat(AM_MEDIA_TYPE &amt,BOOL bVideo)
{
	HRESULT hr = NOERROR;
	ULONG nFetched = 0;
	CComPtr<IEnumPins> pEnumPins;
	hr = m_pAsfWriter->EnumPins(&pEnumPins);
	if (!CHECK_RESULT("IBaseFilter EnumPins.",hr)) {return hr;}
	hr = pEnumPins->Reset();
	if (!CHECK_RESULT("IEnumPins Reset.",hr)) {return hr;}
	IPin* pPin = NULL;
	while (pEnumPins->Next(1,&pPin,&nFetched) == S_OK) {
		if (!pPin) continue;

		//�s���̖��O�̎擾
		PIN_INFO info;
		hr = pPin->QueryPinInfo(&info);
		if (!CHECK_RESULT("IPin QueryPinInfo.",hr)) {pPin->Release();return hr;}
		if (info.pFilter) {
			info.pFilter->Release();
			info.pFilter = NULL;
		}
		if (bVideo) {	//�r�f�I
			if (_tcscmp(info.achName,_T("Video Input 01")) != 0) {
				pPin->Release();
				continue;
			}
		} else {		//�I�[�f�B�I
			if (_tcscmp(info.achName,_T("Audio Input 01")) != 0) {
				pPin->Release();
				continue;
			}
		}
		//�s���̃t�H�[�}�b�g���擾
		CComPtr<IAMStreamConfig> pAMStreamConfig;
		hr = pPin->QueryInterface(IID_IAMStreamConfig,(void**)&pAMStreamConfig);
		if (!CHECK_RESULT("IPin QueryInterface IAMStreamConfig.",hr)) {pPin->Release();return hr;}
		AM_MEDIA_TYPE* pAmt = NULL;
		hr = pAMStreamConfig->GetFormat(&pAmt);
		if (SUCCEEDED(hr))
		{
			amt = *pAmt;
			pPin->Release();
			return S_OK;
		}
		pPin->Release();
		return hr;
	}
	return E_FAIL;
}

//////////////////////////////////////////////////
//�f�[�^�������ł��鎞�Ԃ�ݒ�(�f�t�H���g��3�b dwWindow=3000) �P�ʂ̓~���b
//���̒l���傫���ƁA�o�b�t�@�̈�Ƀf�[�^�����܂�₷�����O�ƂȂ�B�t�ɏ������ƃf�[�^��drop����\��������B
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::SetAsfSyncTolerance(const DWORD dwWindow)
{
	assert(m_pWMWriterAdvanced3);
	if (dwWindow == 0) return E_FAIL;
	HRESULT hr = NOERROR;
	hr = m_pWMWriterAdvanced3->SetSyncTolerance(dwWindow);
	if (!CHECK_RESULT("IWMWriterAdvanced3 SetSyncTolerance.",hr)) {return hr;}
	return hr;
}

//////////////////////////////////////////////////
//ASFWriter�ɑ΂���ݒ��K�p����@
//BOOL bIndexFile �t�@�C���ɃC���f�b�N�X�����Č�����L���ɂ��邩�ǂ����H
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::SetAsfIndexMode(const BOOL bIndexFile)
{
	HRESULT hr = NOERROR;
	hr = m_pConfigAsfWriter2->SetIndexMode(bIndexFile);
	if (!CHECK_RESULT("IWMWriterAdvanced2 SetIndexMode.",hr)) {return hr;}
	return hr;
}

//////////////////////////////////////////////////
//ASFWriter�ɑ΂���ݒ��K�p����A
//dwSettingParam	�Z�b�e�B���O�̃p�����[�^
//AM_CONFIGASFWRITER_PARAM_AUTOINDEX		�I�����ɃC���f�b�N�X�𐶐����邩�ۂ��H
//											�t���[���x�[�X�̃G���R�[�h�̏ꍇ��FALSE�ɂ���B
//AM_CONFIGASFWRITER_PARAM_MULTIPASS		2pass�ȏ�̏ꍇ��TRUE�ɂ���
//											�G���R�[�_�ɂ���Ă�2pass�T�|�[�g���Ă��Ȃ��̂ł�����m�F����K�v������
//AM_CONFIGASFWRITER_PARAM_DONTCOMPRESS		�񈳏k���[�h��L���ɂ��邩�ǂ����H
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::SetAsfParam(const DWORD dwSettingParam,const DWORD dwParam)
{
	assert(m_pAsfWriter);
	HRESULT hr = NOERROR;
	hr = m_pConfigAsfWriter2->SetParam(dwSettingParam,dwParam,0);
	if (!CHECK_RESULT("IWMWriterAdvanced2 SetParam.",hr)) {return hr;}
	return hr;
}

//////////////////////////////////////////////////
//�t���[�����[�g�̒��������邩���Ȃ���
//���̍�Ƃ�Sink���������ɐݒ���s������
//���̋@�\�͂ǂ��������Sink(File��Pull��Push��)�ɂ����@�\���Ă��炸�A�܂����ʂ��s��
//(35fps����̕ǂ��j��Ȃ�)
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::SetFixedFrameRate(BOOL bFixedFrameRate)
{
	assert(m_pAsfWriter);
	assert(m_pWMWriterAdvanced3);
	HRESULT hr = NOERROR;

	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i=0;i<(int)dwCount;i++)
	{
		BOOL bNowFixed;
		WMT_ATTR_DATATYPE attr;
		WORD cbLength;
		hr = m_pWMWriterAdvanced3->GetInputSetting(
			i,g_wszFixedFrameRate,&attr,NULL,&cbLength);
		hr = m_pWMWriterAdvanced3->GetInputSetting(
			i,g_wszFixedFrameRate,&attr,(BYTE*)&bNowFixed,&cbLength);

		hr = m_pWMWriterAdvanced3->SetInputSetting(
			i,g_wszFixedFrameRate,WMT_TYPE_BOOL,(BYTE*)&bFixedFrameRate,sizeof(BOOL));
		
	}
	return hr;
}

//////////////////////////////////////////////////
//�t�@�C���ɏo�͂̒ǉ�
//const WCHAR* pwszFileName �o�̓t�@�C����(�g���q��.asf���]�܂���)
//int* pnIndex �C���f�b�N�X�l
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::AddFileSink(const WCHAR* pwszFileName,int* pnIndex)
{
	assert(m_pWMWriterAdvanced3);
	HRESULT hr = NOERROR;
	if (!pwszFileName) return E_FAIL;
	IWMWriterFileSink* pWMWriterFileSink = NULL;
	hr = WMCreateWriterFileSink(&pWMWriterFileSink);
	if (!CHECK_RESULT("WMCreateWriterFileSink.",hr)) {return hr;}

	hr = pWMWriterFileSink->Open(pwszFileName);
	if (FAILED(hr))		//�����Ńt�@�C�����J���Ȃ��ꍇ������(�G���[��~���Ȃ�)
	{
		pWMWriterFileSink->Release();
		return hr;
	}
	/*
	if (!CHECK_RESULT("IWMWriterFileSink Open.",hr)) {
		pWMWriterFileSink->Release();
		return hr;}
		*/
	hr = m_pWMWriterAdvanced3->AddSink(pWMWriterFileSink);
	if (!CHECK_RESULT("IWMWriterAdvanced3 AddSink.",hr)) {
		pWMWriterFileSink->Release();
		return hr;}

	m_FileSinkList.push_back(pWMWriterFileSink);
	if (pnIndex)
		*pnIndex = (int)m_FileSinkList.size() -1;

	//�����ő�������ݒ肷��
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i=0;i<(int)dwCount;i++) {
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (pSink == pWMWriterFileSink) {
			hr = SetAttribute(i);
			break;
		}
	}
	return hr;
}
//////////////////////////////////////////////////
//�t�@�C���ɏo�͂̍폜
//int nIndex m_FileSinkList�̔z��C���f�b�N�X (0����)
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::RemoveFileSink(const int nIndex)
{
	assert(m_pWMWriterAdvanced3);
	if (nIndex >= (int)m_FileSinkList.size())
		return E_FAIL;

	HRESULT hr = NOERROR;
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i = 0;i<(int)dwCount;i++)
	{
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (!pSink) continue;

		if (pSink == m_FileSinkList[nIndex]) {
			hr = m_pWMWriterAdvanced3->RemoveSink(pSink);
			CHECK_RESULT("IWMWriterAdvanced3 RemoveSink.",hr);

			m_FileSinkList[nIndex]->Release();
			m_FileSinkList[nIndex] = NULL;

			return hr;
		}
	}
	return hr;
}

//////////////////////////////////////////////////
//�u���[�h�L���X�g�o�͂̒ǉ�
//const WCHAR* pwszFileName �o�̓t�@�C����(�g���q��.asf���]�܂���)
//int* pnIndex �C���f�b�N�X�l
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::AddNetworkSink(DWORD* pdwPortNum,const DWORD dwMaxClients,int* pnIndex)
{
	assert(m_pWMWriterAdvanced3);
	HRESULT hr = NOERROR;
	if (!pdwPortNum) return E_FAIL;
	if (dwMaxClients == 0 || dwMaxClients > 50) return E_FAIL;

	IWMWriterNetworkSink* pWMWriterNetworkSink = NULL;
	hr = WMCreateWriterNetworkSink(&pWMWriterNetworkSink);
	if (!CHECK_RESULT("WMCreateWriterNetworkSink.",hr)) {return hr;}

	hr = pWMWriterNetworkSink->SetNetworkProtocol(WMT_PROTOCOL_HTTP);
	if (!CHECK_RESULT("IWMWriterNetworkSink SetNetworkProtocol.",hr)) {
		pWMWriterNetworkSink->Release();
		return hr;}
	hr = pWMWriterNetworkSink->SetMaximumClients(dwMaxClients);
	if (!CHECK_RESULT("IWMWriterNetworkSink SetMaximumClients.",hr)) {
		pWMWriterNetworkSink->Release();
		return hr;}
	//�|�[�g���J��
	//�����|�[�g�ԍ����ƊJ���Ƃ��ɃG���[�������Ŕ�������
	hr = pWMWriterNetworkSink->Open(pdwPortNum);
	if (FAILED(hr)) {
		pWMWriterNetworkSink->Release();
		return hr;
	}
	hr = m_pWMWriterAdvanced3->AddSink(pWMWriterNetworkSink);
	if (!CHECK_RESULT("IWMWriterAdvanced3 AddSink.",hr)) {
		pWMWriterNetworkSink->Release();
		return hr;}
	m_NetworkSinkList.push_back(pWMWriterNetworkSink);
	if (pnIndex)
		*pnIndex = (int)m_NetworkSinkList.size() -1;

	//�����ő�������ݒ肷��
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i=0;i<(int)dwCount;i++) {
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (pSink == pWMWriterNetworkSink) {
			hr = SetAttribute(i);
			break;
		}
	}
	//�R�[���o�b�N�̐ݒ�
	CComPtr<IWMClientConnections> pWMClientConnections;
	CComPtr<IWMRegisterCallback> pWMRegisterCallback;
	hr = pWMWriterNetworkSink->QueryInterface(IID_IWMClientConnections,(void**)&pWMClientConnections);
	hr = pWMClientConnections->QueryInterface(IID_IWMRegisterCallback,(void**)&pWMRegisterCallback);
	if (m_pNetworkCallback) {
		pWMRegisterCallback->Advise(m_pNetworkCallback,(void*)*pnIndex);	//�R�[���o�b�N���w��
	}
	return hr;
}

//////////////////////////////////////////////////
//�u���[�h�L���X�g�o�͂̍폜
//int nIndex m_NetworkSinkList�̔z��C���f�b�N�X (0����)
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::RemoveNetworkSink(const int nIndex)
{
	assert(m_pWMWriterAdvanced3);
	if (nIndex >= (int)m_NetworkSinkList.size())
		return E_FAIL;

	HRESULT hr = NOERROR;
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i = 0;i<(int)dwCount;i++)
	{
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (pSink == m_NetworkSinkList[nIndex]) {
			hr = m_pWMWriterAdvanced3->RemoveSink(pSink);

			//m_NetworkSinkList[nIndex]->Disconnect();
			m_NetworkSinkList[nIndex]->Close();
			m_NetworkSinkList[nIndex]->Release();
			m_NetworkSinkList[nIndex] = NULL;

			return hr;
		}
	}
	return hr;
}
//////////////////////////////////////////////////
//�u���[�h�L���X�g�o�͂̐ݒ�ύX(�l���̕ύX)
//int nIndex m_NetworkSinkList�̔z��C���f�b�N�X (0����)
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::ChangeNetworkSink(const int nIndex,const DWORD dwMaxClients)
{
	assert(m_pWMWriterAdvanced3);
	if (nIndex >= (int)m_NetworkSinkList.size())
		return E_FAIL;
	HRESULT hr = NOERROR;
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i = 0;i<(int)dwCount;i++)
	{
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (pSink == m_NetworkSinkList[nIndex]) {
			DWORD dwNowMaxClients = 0;
			m_NetworkSinkList[nIndex]->GetMaximumClients(&dwNowMaxClients);
			if (dwNowMaxClients != dwMaxClients)
				hr = m_NetworkSinkList[nIndex]->SetMaximumClients(dwMaxClients);
			return hr;
		}
	}
	return hr;
}

//////////////////////////////////////////////////
//�u���[�h�L���X�g�o�͂ł̐ڑ����ꂽ�����擾
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::GetNetworkSink(const int nIndex,DWORD dwIPAddress,DWORD dwPort,NetworkClientStruct &Client)
{
	assert(m_pWMWriterAdvanced3);
	if (nIndex < 0  || nIndex >= (int)m_NetworkSinkList.size())
		return E_FAIL;

	HRESULT hr = NOERROR;
	BOOL bGetData = FALSE;
	IWMClientConnections2 *pIWMClientConnections2 = NULL;
	hr = m_NetworkSinkList[nIndex]->QueryInterface(IID_IWMClientConnections2,(void**)&pIWMClientConnections2);

	//�ڑ��N���C�A���g�����擾
	DWORD dwClientCount = 0;
	hr = pIWMClientConnections2->GetClientCount(&dwClientCount);
	WM_CLIENT_PROPERTIES prop;
	for (int i=0;i<(int)dwClientCount;i++)
	{
		prop.dwIPAddress = 0;
		prop.dwPort = 0;
		hr = pIWMClientConnections2->GetClientProperties(i,&prop);
		if (prop.dwIPAddress != dwIPAddress)
			continue;
		if (prop.dwPort != dwPort)
			continue;

		DWORD cchNetworkAddress = 0;
		DWORD cchPort = 0;
		DWORD cchDNSName = 0;
		hr = pIWMClientConnections2->GetClientInfo(
			i,
			NULL,&cchNetworkAddress,
			NULL,&cchPort,
			NULL,&cchDNSName);
		WCHAR* lpszNetworkAddress = new WCHAR[cchNetworkAddress];
		WCHAR* lpszPort = new WCHAR[cchPort];
		WCHAR* lpszDNSName = new WCHAR[cchDNSName];

		hr = pIWMClientConnections2->GetClientInfo(i,
			lpszNetworkAddress,&cchNetworkAddress,
			lpszPort,&cchPort,
			lpszDNSName,&cchDNSName);

		Client.strNetworkAddress = lpszNetworkAddress;
		Client.strDNSName = lpszDNSName;
		Client.dwPort = prop.dwPort;
		Client.dwNetworkAddress = prop.dwIPAddress;
		Client.nNetworkAddress[0] = (0xFF000000 & prop.dwIPAddress) >> 24;
		Client.nNetworkAddress[1] = (0xFF0000 & prop.dwIPAddress) >> 16;
		Client.nNetworkAddress[2] = (0xFF00 & prop.dwIPAddress) >> 8;
		Client.nNetworkAddress[3] = (0xFF & prop.dwIPAddress);
		delete [] lpszNetworkAddress;
		delete [] lpszPort;
		delete [] lpszDNSName;
		bGetData = TRUE;
		break;
	}
	SAFE_RELEASE(pIWMClientConnections2);

	if (bGetData)
		return S_OK;
	return E_FAIL;
}

//////////////////////////////////////////////////
//�v�b�V���o��
//LPCWSTR pwszURL URL������ ��	"http://myserver.net:8080/PublishingPoint"
//LPCWSTR pwszTemplateURL �ǉ��̕�����
//const BOOL fAutoDestroy Push�悩�珜���ꂽ��A������sink��ؒf���邩�ǂ����H
//int* pnIndex �C���f�b�N�X�l
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::AddPushSink(LPCWSTR pwszURL,LPCWSTR pwszTemplateURL,const BOOL fAutoDestroy,int* pnIndex)
{
	assert(m_pWMWriterAdvanced3);
	HRESULT hr = NOERROR;
	if (!pwszURL) return E_FAIL;

	IWMWriterPushSink* pWMWriterPushSink = NULL;
	hr = WMCreateWriterPushSink(&pWMWriterPushSink);
	if (!CHECK_RESULT("WMCreateWriterPushSink.",hr)) {return hr;}
	//�T�[�o�[�ɐڑ�����
	//�����Ŗ����ȃT�[�o�[�������͂��Ă���ꍇ�A�G���[���N����
	hr = pWMWriterPushSink->Connect(pwszURL,pwszTemplateURL,fAutoDestroy);
	if (FAILED(hr)) {
		pWMWriterPushSink->Release();
		return hr;
	}
	hr = m_pWMWriterAdvanced3->AddSink(pWMWriterPushSink);
	if (!CHECK_RESULT("IWMWriterAdvanced3 AddSink.",hr)) {
		pWMWriterPushSink->Release();
		return hr;}
	m_PushSinkList.push_back(pWMWriterPushSink);
	if (pnIndex)
		*pnIndex = (int)m_PushSinkList.size() -1;

	//�����ő�������ݒ肷��
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i=0;i<(int)dwCount;i++) {
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (pSink == pWMWriterPushSink) {
			hr = SetAttribute(i);
			break;
		}
	}
	return hr;
}
//////////////////////////////////////////////////
//�v�b�V���o�͂̍폜
//int nIndex m_PushSinkList�̔z��C���f�b�N�X (0����)
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::RemovePushSink(const int nIndex)
{
	assert(m_pWMWriterAdvanced3);
	if (nIndex >= (int)m_PushSinkList.size())
		return E_FAIL;

	HRESULT hr = NOERROR;
	DWORD dwCount = 0;
	hr = m_pWMWriterAdvanced3->GetSinkCount(&dwCount);
	for (int i = 0;i<(int)dwCount;i++)
	{
		IWMWriterSink* pSink = NULL;
		hr = m_pWMWriterAdvanced3->GetSink(i,&pSink);
		if (pSink == m_PushSinkList[nIndex]) {

			hr = m_pWMWriterAdvanced3->RemoveSink(pSink);

			hr = m_PushSinkList[nIndex]->EndSession();
			hr = m_PushSinkList[nIndex]->Disconnect();
			m_PushSinkList[nIndex]->Release();
			m_PushSinkList[nIndex] = NULL;
			return hr;
		}
	}
	return hr;
}

//////////////////////////////////////////////////
//���v�����擾
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::GetStatistics(QWORD &cnsCurrentTime,WM_WRITER_STATISTICS &Stats,WM_WRITER_STATISTICS_EX &StatsEx)
{
	assert(m_pWMWriterAdvanced3);

	HRESULT hr = NOERROR;
	hr = m_pWMWriterAdvanced3->GetWriterTime(&cnsCurrentTime);
	if (!CHECK_RESULT("IWMWriterAdvanced3 GetWriterTime.",hr)) {return hr;}
	//�X�g���[�~���O�ԍ��͊����0���w��
	WORD wStreamNum = 0;
	ZeroMemory(&Stats,sizeof(WM_WRITER_STATISTICS));
	hr = m_pWMWriterAdvanced3->GetStatistics(wStreamNum,&Stats);
	if (!CHECK_RESULT("IWMWriterAdvanced3 GetStatistics.",hr)) {return hr;}
	ZeroMemory(&StatsEx,sizeof(WM_WRITER_STATISTICS_EX));
	if (m_bIsWriterAdvanced3) {
		hr = m_pWMWriterAdvanced3->GetStatisticsEx(wStreamNum,&StatsEx);
		if (FAILED(hr))
			m_bIsWriterAdvanced3 = FALSE;
		//if (!CHECK_RESULT("IWMWriterAdvanced3 GetStatisticsEx.",hr)) {return hr;}
	}
	return hr;
}

//////////////////////////////////////////////////
//���^�f�[�^�̐ݒ�
//////////////////////////////////////////////////
HRESULT CDirectShowASFWriter::SetAttribute(int nIndex)
{
	assert(m_pWMHeaderInfo);

	HRESULT hr = NOERROR;
	if (m_strTitle.length() != 0){
		hr = m_pWMHeaderInfo->SetAttribute(nIndex,
			g_wszWMTitle,
			WMT_TYPE_STRING,
			(const BYTE*)m_strTitle.c_str(),
			(WORD)m_strTitle.length()*2);
	}
	if (m_strAuthor.length() != 0){
		hr = m_pWMHeaderInfo->SetAttribute(nIndex,
			g_wszWMAuthor,
			WMT_TYPE_STRING,
			(const BYTE*)m_strAuthor.c_str(),
			(WORD)m_strAuthor.length()*2);
	}
	if (m_strDescription.length() != 0) {
		hr = m_pWMHeaderInfo->SetAttribute(nIndex,
			g_wszWMDescription,
			WMT_TYPE_STRING,
			(const BYTE*)m_strDescription.c_str(),
			(WORD)m_strDescription.length()*2);
	}
	if (m_strRating.length() != 0) {
		hr = m_pWMHeaderInfo->SetAttribute(nIndex,
			g_wszWMRating,
			WMT_TYPE_STRING,
			(const BYTE*)m_strRating.c_str(),
			(WORD)m_strRating.length()*2);
	}
	if (m_strCopyright.length() != 0) {
		hr = m_pWMHeaderInfo->SetAttribute(nIndex,
			g_wszWMCopyright,
			WMT_TYPE_STRING,
			(const BYTE*)m_strCopyright.c_str(),
			(WORD)m_strCopyright.length()*2);
	}
	return hr;
}




//////////////////////////////////////////////////
//�l�b�g���[�N�V���N�̃R�[���o�b�N�p�̃C���^�t�F�[�X
//////////////////////////////////////////////////

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CDirectShowASFWriterNetworkCallback::CDirectShowASFWriterNetworkCallback()
{
	m_hWnd = NULL;
}
//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CDirectShowASFWriterNetworkCallback::~CDirectShowASFWriterNetworkCallback()
{
}
//////////////////////////////////////////////////
//�R�[���o�b�N
//////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE CDirectShowASFWriterNetworkCallback::OnStatus(WMT_STATUS Status,HRESULT hr,WMT_ATTR_DATATYPE dwType,BYTE __RPC_FAR *pValue,void __RPC_FAR *pvContext)
{
	if (Status == WMT_CLIENT_CONNECT || Status == WMT_CLIENT_DISCONNECT)
	{
		WM_CLIENT_PROPERTIES* pProp;
		pProp = (WM_CLIENT_PROPERTIES*)pValue;
		if (Status == WMT_CLIENT_CONNECT)
			SendMessage(m_hWnd,WM_NOTIFY_NETWORK_CLIENT_CONNECT,(WPARAM)pvContext,(LPARAM)pProp);
		else  if (Status == WMT_CLIENT_DISCONNECT)
			SendMessage(m_hWnd,WM_NOTIFY_NETWORK_CLIENT_DISCONNECT,(WPARAM)pvContext,(LPARAM)pProp);
	}
	return S_OK;
}
//////////////////////////////////////////////////
//�C���^�t�F�[�X�擾
//////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE CDirectShowASFWriterNetworkCallback::QueryInterface(REFIID riid,void __RPC_FAR *__RPC_FAR *ppvObject)
{
	if (riid == IID_IWMStatusCallback)
	{
		*ppvObject = (IWMStatusCallback*)this;
	} else {
		*ppvObject = NULL;
		return E_NOINTERFACE;
	}
	return S_OK;
}
