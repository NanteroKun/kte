#include "StdAfx.h"
#include "DirectShowEncoder.h"

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CDirectShowEncoder::CDirectShowEncoder(void)
{
	m_pWMProfile = NULL;
	m_pVideoStreamConfig = NULL;
	m_pAudioStreamConfig = NULL;
}
//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CDirectShowEncoder::~CDirectShowEncoder(void)
{
	Delete();
}
//////////////////////////////////////////////////
//�G���R�[�_�v���t�@�C�����쐬����
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::Create()
{
	HRESULT hr = NOERROR;
	if (m_pWMProfile) {
		SAFE_RELEASE(m_pWMProfile);
	}
	//��̃v���t�@�C���̍쐬
	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	if (!CHECK_RESULT("WMCreateProfileManager.",hr)){return hr;}
	hr = pWMProfileManager->CreateEmptyProfile(WMT_VER_9_0,&m_pWMProfile);
	if (!CHECK_RESULT("IWMProfileManager CreateEmptyProfile.",hr)){return hr;}
	hr = m_pWMProfile->SetName(L"Live Encoder");
	if (!CHECK_RESULT("IWMProfile SetName.",hr)){return hr;}
	hr = m_pWMProfile->SetDescription(L"Live Encoder Settings");
	if (!CHECK_RESULT("IWMProfile SetDescription.",hr)){return hr;}
	return hr;
}
//////////////////////////////////////////////////
//�v���t�@�C���̃��[�h
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::LoadProfile(LPCWSTR pwszFileName)
{
	HRESULT hr = NOERROR;
	if (m_pWMProfile) {
		SAFE_RELEASE(m_pWMProfile);
	}

	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	//WCHAR szProfile[4096];	//MAX_PROFILE_SIZE_W

	FILE* fp = NULL;
	_tfopen_s(&fp,pwszFileName,_T("r,ccs=UTF-16LE"));

	fpos_t fsize = 0;
	fseek(fp,0,SEEK_END);
	fgetpos(fp,&fsize);
	fseek(fp,0,SEEK_SET);

	WCHAR* szProfile = new WCHAR[((int)fsize/2)+1];
	ZeroMemory(szProfile,sizeof(WCHAR)*(((int)fsize/2)+1));

	fread(szProfile,((int)fsize/2)+1,sizeof(WCHAR),fp);

	hr = pWMProfileManager->LoadProfileByData(szProfile, &m_pWMProfile);
	if (!CHECK_RESULT("IWMProfileManager LoadProfileByData.",hr)) {return hr;}

	delete [] szProfile;
	fclose(fp);
	return hr;
}

//////////////////////////////////////////////////
//�G���R�[�_�v���t�@�C�����폜����
//////////////////////////////////////////////////
void CDirectShowEncoder::Delete()
{
	SAFE_RELEASE(m_pVideoStreamConfig);
	SAFE_RELEASE(m_pAudioStreamConfig);
	SAFE_RELEASE(m_pWMProfile);
}

//////////////////////////////////////////////////
//�G���R�[�_�v���t�@�C���̓K�p���s��
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::ApplyProfiles()
{
	assert(m_pWMProfile);
	assert(m_pAudioStreamConfig);
	//assert(m_pVideoStreamConfig);
	HRESULT hr = NOERROR;
	DWORD cStreams = 0;
	hr = m_pWMProfile->GetStreamCount(&cStreams);
	if (!CHECK_RESULT("IWMProfile GetStreamCount.",hr)){return hr;}
	if (cStreams != 0)
	{
		//�����̃v���t�@�C�������݂����̂ŁA�č쐬����
		hr = Create();
		if (FAILED(hr))
			return hr;
	}
	//�v���t�@�C���̓K�p(Audio��Video�̒ǉ�)
	if (m_pAudioStreamConfig) {
		hr = m_pWMProfile->AddStream(m_pAudioStreamConfig);	//�I�[�f�B�I�v���t�@�C��
		if (!CHECK_RESULT("IWMProfile AddStream IAudioStreamConfig.",hr)){return hr;}
	}
	if (m_pVideoStreamConfig) {
		hr = m_pWMProfile->AddStream(m_pVideoStreamConfig);	//�r�f�I�v���t�@�C��
		if (!CHECK_RESULT("IWMProfile AddStream IVideoStreamConfig.",hr)){return hr;}
	}
	return hr;
}
//////////////////////////////////////////////////
//�G���R�[�h�v���t�@�C���̃t�@�C���Z�[�u����T�u���[�`��
//pWMProfile	�ۑ�����v���t�@�C��
//pwszFileName	�o�̓t�@�C����
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SaveProfile(IWMProfile* pWMProfile,LPCWSTR pwszFileName)
{
	assert(pWMProfile);
	assert(pwszFileName);
	HRESULT hr = NOERROR;
	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	DWORD dwLength = 0;
	hr = pWMProfileManager->SaveProfile(pWMProfile,NULL,&dwLength);
	WCHAR* pwszProfile = new WCHAR[dwLength];
	hr = pWMProfileManager->SaveProfile(pWMProfile,pwszProfile,&dwLength);

	FILE *fp = NULL;
	_tfopen_s(&fp,pwszFileName,_T("w,ccs=UTF-16LE"));
	_ftprintf_s(fp,_T("%s"),pwszProfile);
	fclose(fp);
	delete [] pwszProfile;
	return hr;
}

//////////////////////////////////////////////////
//OS�Œ�`�ς݃v���t�@�C���̑S��
//dwVersion = WMT_VER_7_0 , WMT_VER_8_0 , WMT_VER_9_0
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::ListAllProfiles(WMT_VERSION dwVersion)
{
	HRESULT hr = NOERROR;

	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	if (!CHECK_RESULT("WMCreateProfileManager.",hr)) {return hr;}

	CComPtr<IWMProfileManager2> pWMProfileManager2;
	hr = pWMProfileManager->QueryInterface(IID_IWMProfileManager2,(void**)&pWMProfileManager2);
	if (!CHECK_RESULT("IWMProfileManager QueryInterface IWMProfileManager2.",hr)) {return hr;}

	//�v���t�@�C���o�[�W�����̐ݒ�
	hr = pWMProfileManager2->SetSystemProfileVersion(dwVersion);
	if (!CHECK_RESULT("IWMProfileManager2 SetSystemProfileVersion.",hr)) {return hr;}

	//�v���t�@�C���̐����擾
	DWORD dwPcProfiles;
	hr = pWMProfileManager->GetSystemProfileCount(&dwPcProfiles);
	//�v���t�@�C���̐��������[�v
	for (int i=0;i<(int)dwPcProfiles;i++)
	{
		//�v���t�@�C���̎擾
		CComPtr<IWMProfile> pWMProfile;
		hr = pWMProfileManager->LoadSystemProfile(i,&pWMProfile);
		if (!CHECK_RESULT("IWMProfileManager LoadSystemProfile.",hr)) {return false;}

		//�v���t�@�C���̕\��
		ListProfiles(pWMProfile);
	}
	return hr;
}
//////////////////////////////////////////////////
//�v���t�@�C���̗�
//pWMProfile = �񋓂���IWMProfile
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::ListProfiles(IWMProfile* pWMProfile)
{
	assert(pWMProfile);
	HRESULT hr = NOERROR;

	//���O�̎擾
	DWORD cchName = 0;
	hr = pWMProfile->GetName(NULL,&cchName);
	WCHAR* pwszName = new WCHAR[cchName];
	hr = pWMProfile->GetName(pwszName,&cchName);
	//�����̎擾
	DWORD cchDescription = 0;
	hr = pWMProfile->GetDescription(NULL,&cchDescription);
	WCHAR* pwszDescription = new WCHAR[cchDescription];
	hr = pWMProfile->GetDescription(pwszDescription,&cchDescription);
	//NOTE1("Name=%s",pwszName);
	//NOTE1("Description=%s",pwszDescription);

	//�v���t�@�C���̃Z�[�u
	/*
	WCHAR wszFileName[MAX_PATH];
	_stprintf_s(wszFileName,MAX_PATH,_T("%s.prx"),pwszName);
	hr = SaveProfile(pWMProfile,wszFileName);
	*/

	//�o�[�W�������̎擾
	WMT_VERSION dwVersion;
	hr = pWMProfile->GetVersion(&dwVersion);

	DWORD cStreams = 0;	//�X�g���[����
	hr = pWMProfile->GetStreamCount(&cStreams);
	for (int i=1;i<=(int)cStreams;i++)
	{
		IWMStreamConfig* pWMStreamConfig = NULL;
		hr = pWMProfile->GetStreamByNumber((WORD)i,&pWMStreamConfig);

		//�X�g���[�~���O�ݒ�̕\��
		ListStreamConfig(pWMStreamConfig);

		pWMStreamConfig->Release();
	}
	NOTE("\n");
	delete [] pwszName;
	delete [] pwszDescription;
	return hr;
}
//////////////////////////////////////////////////
//�X�g���[�~���O�ݒ�̕\��
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::ListStreamConfig(IWMStreamConfig* pWMStreamConfig)
{
	HRESULT hr = NOERROR;
	assert(pWMStreamConfig);

	//���O�̎擾
	WORD cchStreamName = 0;
	hr = pWMStreamConfig->GetStreamName(NULL,&cchStreamName);
	WCHAR* pwszStreamName = new WCHAR[cchStreamName];
	hr = pWMStreamConfig->GetStreamName(pwszStreamName,&cchStreamName);

	//�ڑ����̎擾
	WORD cchInputName = 0;
	hr = pWMStreamConfig->GetConnectionName(NULL,&cchInputName);
	WCHAR* pwszInputName = new WCHAR[cchInputName];
	hr = pWMStreamConfig->GetConnectionName(pwszInputName,&cchInputName);

	DWORD dwBitrate = 0;
	hr = pWMStreamConfig->GetBitrate(&dwBitrate);			//�r�b�g���[�g
	DWORD msBufferWindow = 0;
	hr = pWMStreamConfig->GetBufferWindow(&msBufferWindow);	//�o�b�t�@�E�B���h�E
	WORD wStreamNum = 0;
	hr = pWMStreamConfig->GetStreamNumber(&wStreamNum);		//�X�g���[���ԍ�
	GUID guidStreamType;
	hr = pWMStreamConfig->GetStreamType(&guidStreamType);	//GUID��

	NOTE1("StreamName     = %s",pwszStreamName);
	NOTE1("ConnectionName = %s",pwszInputName);
	NOTE1("Bitrate        = %d",dwBitrate);
	NOTE1("BufferWindow   = %d",msBufferWindow);
	NOTE1("StreamNum      = %d",wStreamNum);

	RPC_WSTR StringUuid = 0;
	UuidToString(&guidStreamType,&StringUuid);
	NOTE1("StreamType     = %s",StringUuid);
	RpcStringFree(&StringUuid);

	//���f�B�A�^�C�v�̎擾
	IWMMediaProps* pWMMediaProps = NULL;
	hr = pWMStreamConfig->QueryInterface(IID_IWMMediaProps,(void**)&pWMMediaProps);
	GUID guidType;
	hr = pWMMediaProps->GetType(&guidType);
	DWORD cbType = 0;
	hr = pWMMediaProps->GetMediaType(NULL,&cbType);
	WM_MEDIA_TYPE* pType = (WM_MEDIA_TYPE*) new BYTE[cbType];
	hr = pWMMediaProps->GetMediaType(pType,&cbType);

	//DisplayType(L"",(AM_MEDIA_TYPE*)pType);
	GUID majortype				= pType->majortype;
	GUID subtype				= pType->subtype;
	BOOL bFixedSizeSamples		= pType->bFixedSizeSamples;
	BOOL bTemporalCompression	= pType->bTemporalCompression;
	ULONG lSampleSize			= pType->lSampleSize;
	GUID formattype				= pType->formattype;
	IUnknown* pUnk				= pType->pUnk;
	ULONG cbFormat				= pType->cbFormat;


	if (pType->formattype == WMFORMAT_VideoInfo) {
		WMVIDEOINFOHEADER *pWMVideoInfoHeader = (WMVIDEOINFOHEADER *)pType->pbFormat;
		RECT rcSource = pWMVideoInfoHeader->rcSource;
		RECT rcTarget = pWMVideoInfoHeader->rcTarget;
		DWORD dwBitRate = pWMVideoInfoHeader->dwBitRate;
		DWORD dwBitErrorRate = pWMVideoInfoHeader->dwBitErrorRate;
		LONGLONG AvgTimePerFrame = pWMVideoInfoHeader->AvgTimePerFrame;

		DWORD biSize = pWMVideoInfoHeader->bmiHeader.biSize;
		LONG biWidth = pWMVideoInfoHeader->bmiHeader.biWidth;
		LONG biHeight = pWMVideoInfoHeader->bmiHeader.biHeight;
		WORD biPlanes = pWMVideoInfoHeader->bmiHeader.biPlanes;
		WORD biBitCount =pWMVideoInfoHeader->bmiHeader.biBitCount;
		DWORD biCompression = pWMVideoInfoHeader->bmiHeader.biCompression;
		DWORD biSizeImage = pWMVideoInfoHeader->bmiHeader.biSizeImage;
		LONG biXPelsPerMeter = pWMVideoInfoHeader->bmiHeader.biXPelsPerMeter;
		LONG biYPelsPerMeter = pWMVideoInfoHeader->bmiHeader.biYPelsPerMeter;
		DWORD biClrUsed = pWMVideoInfoHeader->bmiHeader.biClrUsed;
		DWORD biClrImportant = pWMVideoInfoHeader->bmiHeader.biClrImportant;
	}
	else if (pType->formattype == WMFORMAT_WaveFormatEx) {
		WAVEFORMATEX *pWaveFormatEx = (WAVEFORMATEX*)pType->pbFormat;
		WORD wFormatTag = pWaveFormatEx->wFormatTag;
		WORD nChannels = pWaveFormatEx->nChannels;
		DWORD nSamplesPerSec = pWaveFormatEx->nAvgBytesPerSec;
		DWORD nAvgBytesPerSec = pWaveFormatEx->nAvgBytesPerSec;
		WORD nBlockAlign = pWaveFormatEx->nBlockAlign;
		WORD wBitsPerSample = pWaveFormatEx->wBitsPerSample;
		WORD cbSize = pWaveFormatEx->cbSize;	//TODO ���̃T�C�Y���s��
	}

	delete [] pType;
	pWMMediaProps->Release();

	delete [] pwszStreamName;
	delete [] pwszInputName;

	return hr;
}


//////////////////////////////////////////////////
//�R�[�f�b�N�̑S��
//guidType = WMMEDIATYPE_Audio or WMMEDIATYPE_Video
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::ListAllCodec(const GUID guidType)
{
	HRESULT hr = NOERROR;

	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);

	CComPtr<IWMCodecInfo3> pWMCodecInfo3;
	hr =pWMProfileManager->QueryInterface(IID_IWMCodecInfo3,(void**)&pWMCodecInfo3);

	DWORD cCodecs = 0;
	hr = pWMCodecInfo3->GetCodecInfoCount(guidType, &cCodecs);

	for(DWORD dwCodecIndex = 0; dwCodecIndex < cCodecs; dwCodecIndex++)
	{
		//�R�[�f�b�N���̎擾
		DWORD cchName = 0;
		hr = pWMCodecInfo3->GetCodecName(guidType,dwCodecIndex,NULL,&cchName);
		WCHAR* pwszCodecName = new WCHAR[cchName];
		hr = pWMCodecInfo3->GetCodecName(guidType,dwCodecIndex,pwszCodecName,&cchName);
		NOTE2("CodecName[%d]           = %s",dwCodecIndex,pwszCodecName);

		//�R�[�f�b�N�Ɋւ���v���p�e�B�擾
		WMT_ATTR_DATATYPE data_type;
		DWORD dwSize = 0;
		DWORD dwComplexityMax = 0;
		hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszComplexityMax,&data_type,(BYTE*)&dwComplexityMax,&dwSize);
		DWORD dwComplexityOffline = 0;
		hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszComplexityOffline,&data_type,(BYTE*)&dwComplexityOffline,&dwSize);
		DWORD dwComplexityLive = 0;
		hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszComplexityLive,&data_type,(BYTE*)&dwComplexityLive,&dwSize);
		BOOL bIsVBRSupported = FALSE;	//VBR���L���ɂł��邩�ǂ���
		hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszIsVBRSupported,&data_type,(BYTE*)&bIsVBRSupported,&dwSize);

		//�񋓂Ɋւ���ݒ���擾
		BOOL bVBREnabled = FALSE;	//VBR��L���ɂ��ė񋓂��s��
		hr = pWMCodecInfo3->GetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszVBREnabled,&data_type,(BYTE*)&bVBREnabled,&dwSize);
		DWORD dwNumPasses = 0;		//�����p�X(1or2)��L���ɂ��ė񋓂��s��
		hr = pWMCodecInfo3->GetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,&data_type,(BYTE*)&dwNumPasses,&dwSize);

		//�t�H�[�}�b�g���̎擾
		DWORD cFormat = 0;
		hr = pWMCodecInfo3->GetCodecFormatCount(guidType,dwCodecIndex,&cFormat);
		NOTE2("CodecFormatCount[%d]    = %d",dwCodecIndex,cFormat);

		for (DWORD cFormatIndex = 0; cFormatIndex<cFormat;cFormatIndex++)
		{
			//�t�H�[�}�b�g�Ɋւ��Ă̐������̎擾
			DWORD cchDesc = 0;
			pWMCodecInfo3->GetCodecFormatDesc(guidType,dwCodecIndex,cFormatIndex,NULL,NULL,&cchDesc);
			WCHAR* pwszDesc = new WCHAR[cchDesc];
			pWMCodecInfo3->GetCodecFormatDesc(guidType,dwCodecIndex,cFormatIndex,NULL,pwszDesc,&cchDesc);
			NOTE3("CodecDescription[%d][%d] = %s",dwCodecIndex,cFormatIndex,pwszDesc);
			delete [] pwszDesc;

			//�t�H�[�}�b�g�Ɋւ���v���p�e�B�擾
			DWORD dwSpeechCaps = WMT_MS_CLASS_MUSIC;
			hr = pWMCodecInfo3->GetCodecFormatProp(guidType,dwCodecIndex,cFormatIndex,g_wszSpeechCaps,&data_type,(BYTE*)dwSpeechCaps,&dwSize);

			//�X�g���[�~���O�ݒ�̎擾
			IWMStreamConfig* pWMStreamConfig = NULL;
			hr =pWMCodecInfo3->GetCodecFormat(guidType,dwCodecIndex,cFormatIndex,&pWMStreamConfig);
	
			//�X�g���[�~���O�ݒ�̕\��
			ListStreamConfig(pWMStreamConfig);
			pWMStreamConfig->Release();
		}

		delete [] pwszCodecName;
	}
	return hr;
}

//////////////////////////////////////////////////
//�R�[�f�b�N�̑S�񋓂��\���̂Ɏ擾����
//guidType = WMMEDIATYPE_Audio or WMMEDIATYPE_Video
//Codec �󂫂�CodecStruct�\����
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::EnumCodec(const GUID guidType,vector<CodecStruct> &Codec)
{
	Codec.clear();
	HRESULT hr = NOERROR;
	//hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	if (!CHECK_RESULT("WMCreateProfileManager.",hr)){return hr;}

	CComPtr<IWMCodecInfo3> pWMCodecInfo3;
	hr =pWMProfileManager->QueryInterface(IID_IWMCodecInfo3,(void**)&pWMCodecInfo3);
	if (!CHECK_RESULT("IWMProfileManager QueryInterface IWMCodecInfo3.",hr)){return hr;}
	DWORD cCodecs = 0;
	hr = pWMCodecInfo3->GetCodecInfoCount(guidType, &cCodecs);
	if (!CHECK_RESULT("IWMCodecInfo3 GetCodecInfoCount.",hr)){return hr;}
	for (DWORD dwCodecIndex = 0; dwCodecIndex < cCodecs; dwCodecIndex++)
	{
		//�R�[�f�b�N���̎擾
		DWORD cchName = 0;
		hr = pWMCodecInfo3->GetCodecName(guidType,dwCodecIndex,NULL,&cchName);
		if (!CHECK_RESULT("IWMCodecInfo3 GetCodecName.",hr)){return hr;}

		WCHAR* pwszCodecName = new WCHAR[cchName];
		hr = pWMCodecInfo3->GetCodecName(guidType,dwCodecIndex,pwszCodecName,&cchName);
		if (!CHECK_RESULT("IWMCodecInfo3 GetCodecName.",hr)){return hr;}

		//�R�[�f�b�N�Ɋւ���v���p�e�B�擾
		WMT_ATTR_DATATYPE data_type;
		DWORD dwSize = sizeof(DWORD);
		DWORD dwComplexityMax = 0;
		if (guidType != WMMEDIATYPE_Audio)
		{
			hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszComplexityMax,&data_type,(BYTE*)&dwComplexityMax,&dwSize);
			if (FAILED(hr))
			{
				dwComplexityMax = 0;
			}
			//if (!CHECK_RESULT("IWMCodecInfo3 GetCodecProp g_wszComplexityMax.",hr)){return hr;}
		}
		DWORD dwComplexityOffline = 0;
		if (guidType != WMMEDIATYPE_Audio)
		{
			hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszComplexityOffline,&data_type,(BYTE*)&dwComplexityOffline,&dwSize);
			if (FAILED(hr))
			{
				dwComplexityOffline = 0;
			}
			//if (!CHECK_RESULT("IWMCodecInfo3 GetCodecProp g_wszComplexityOffline.",hr)){return hr;}
		}
		DWORD dwComplexityLive = 0;
		if (guidType != WMMEDIATYPE_Audio)
		{
			hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszComplexityLive,&data_type,(BYTE*)&dwComplexityLive,&dwSize);
			if (FAILED(hr))
			{
				dwComplexityLive = 0;
			}
			//if (!CHECK_RESULT("IWMCodecInfo3 GetCodecProp g_wszComplexityLive.",hr)){return hr;}
		}
		BOOL bIsVBRSupported = FALSE;
		dwSize = sizeof(BOOL);
		if (guidType != WMMEDIATYPE_Audio)
		{
			hr = pWMCodecInfo3->GetCodecProp(guidType,dwCodecIndex,g_wszIsVBRSupported,&data_type,(BYTE*)&bIsVBRSupported,&dwSize);
			if (FAILED(hr))
			{
				bIsVBRSupported = FALSE;
			}
			//if (!CHECK_RESULT("IWMCodecInfo3 GetCodecProp g_wszIsVBRSupported.",hr)){return hr;}
		}

		BOOL bIsTwoPassSupported = FALSE;
		DWORD dwNumPasses = 0;
		data_type = WMT_TYPE_DWORD;
		dwSize = sizeof(DWORD);
		//hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(DWORD));
		hr = pWMCodecInfo3->GetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,&data_type,(BYTE*)&dwNumPasses,&dwSize);
		//if (!CHECK_RESULT("IWMCodecInfo3 SetCodecEnumerationSetting g_wszNumPasses.",hr)){return hr;}
		if (hr == S_OK && dwNumPasses >= 2) {
			bIsTwoPassSupported = TRUE;
		}
		else
			bIsTwoPassSupported = FALSE;

		//�t�H�[�}�b�g���̎擾
		DWORD dwFormatCounts[2][2] = {{0,0},{0,0}};

		if (guidType == WMMEDIATYPE_Audio)
		{
			//CBR,1�p�X�̃t�H�[�}�b�g�����擾
			BOOL bVBREnabled = FALSE;
			dwNumPasses = 1;
			hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszVBREnabled,WMT_TYPE_BOOL,(BYTE*)&bVBREnabled,sizeof(bVBREnabled));
			hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(dwNumPasses));
			hr = pWMCodecInfo3->GetCodecFormatCount(guidType,dwCodecIndex,&dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_ONEPASS]);

			//VBR,1�p�X�̃t�H�[�}�b�g�����擾
			if (bIsVBRSupported) {
				bVBREnabled = TRUE;
				dwNumPasses = 1;
				hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszVBREnabled,WMT_TYPE_BOOL,(BYTE*)&bVBREnabled,sizeof(bVBREnabled));
				hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(dwNumPasses));
				hr = pWMCodecInfo3->GetCodecFormatCount(guidType,dwCodecIndex,&dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_ONEPASS]);
			}
			//CBR,2�p�X�̃t�H�[�}�b�g�����擾
			if (bIsTwoPassSupported) {
				bVBREnabled = FALSE;
				dwNumPasses = 2;
				hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszVBREnabled,WMT_TYPE_BOOL,(BYTE*)&bVBREnabled,sizeof(BOOL));
				hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(DWORD));
				hr = pWMCodecInfo3->GetCodecFormatCount(guidType,dwCodecIndex,&dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_TWOPASS]);
			}
			//VBR,2�p�X�̃t�H�[�}�b�g���̎擾
			if (bIsVBRSupported && bIsTwoPassSupported) {
				bVBREnabled = TRUE;
				dwNumPasses = 2;
				hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszVBREnabled,WMT_TYPE_BOOL,(BYTE*)&bVBREnabled,sizeof(BOOL));
				hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(DWORD));
				hr = pWMCodecInfo3->GetCodecFormatCount(guidType,dwCodecIndex,&dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_TWOPASS]);
			}
		} else {	//�r�f�I�̏ꍇ
			dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_ONEPASS] = 1;
			if (bIsVBRSupported)
				dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_ONEPASS] = 1;
			if (bIsTwoPassSupported)
				dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_TWOPASS] = 1;
			if (bIsVBRSupported && bIsTwoPassSupported)
				dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_TWOPASS] = 1;
		}
		//�\���̂Ɋi�[
		CodecStruct cs;
		cs.guidType            = guidType;
		cs.strName             = pwszCodecName;
		cs.dwComplexityMax     = dwComplexityMax;
		cs.dwComplexityOffline = dwComplexityOffline;
		cs.dwComplexityLive    = dwComplexityLive;
		cs.bIsVBRSupported     = bIsVBRSupported;
		cs.bIsTwoPassSupported = bIsTwoPassSupported;
		cs.dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_ONEPASS] = dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_ONEPASS];
		cs.dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_TWOPASS] = dwFormatCounts[CODEC_FORMAT_CBR][CODEC_FORMAT_TWOPASS];
		cs.dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_ONEPASS] = dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_ONEPASS];
		cs.dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_TWOPASS] = dwFormatCounts[CODEC_FORMAT_VBR][CODEC_FORMAT_TWOPASS];
		Codec.push_back(cs);
		delete [] pwszCodecName;
		pwszCodecName = NULL;
	}
	pWMCodecInfo3.Release();
	pWMProfileManager.Release();
	return hr;
}

//////////////////////////////////////////////////
//�I�[�f�B�I�R�[�f�b�N�t�H�[�}�b�g�̎擾
//dwCodecIndex   �I�[�f�B�I�R�[�f�b�N�̃C���f�b�N�X�ԍ�
//format_bitrate �r�b�g���[�g�̃^�C�v(CODEC_FORMAT_CBR,CODEC_FORMAT_VBR)
//format_numpass 1�p�X2�p�X�G���R�[�h(CODEC_FORMAT_ONEPASS,CODEC_FORMAT_TWOPASS)
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::EnumAudioCodecFormat(const DWORD dwCodecIndex,CODEC_FORMAT_BITRATE format_bitrate,CODEC_FORMAT_NUMPASS format_numpass,vector<AudioCodecFormatStruct> &AudioFormat)
{
	AudioFormat.clear();
	HRESULT hr = NOERROR;

	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	CComPtr<IWMCodecInfo3> pWMCodecInfo3;
	hr =pWMProfileManager->QueryInterface(IID_IWMCodecInfo3,(void**)&pWMCodecInfo3);
	//�R�[�f�b�N���̎擾
	DWORD cCodecs = 0;
	hr = pWMCodecInfo3->GetCodecInfoCount(WMMEDIATYPE_Audio, &cCodecs);
	if (dwCodecIndex >= cCodecs) return E_FAIL;

	//�t�H�[�}�b�g�̐ݒ�
	BOOL bVBREnabled = FALSE;
	if (format_bitrate == CODEC_FORMAT_VBR)
		bVBREnabled = TRUE;
	DWORD dwNumPasses  = 1;
	if (format_numpass == CODEC_FORMAT_TWOPASS)
		dwNumPasses = 1;
	hr = pWMCodecInfo3->SetCodecEnumerationSetting(WMMEDIATYPE_Audio,dwCodecIndex,g_wszVBREnabled,WMT_TYPE_BOOL,(BYTE*)&bVBREnabled,sizeof(BOOL));
	hr = pWMCodecInfo3->SetCodecEnumerationSetting(WMMEDIATYPE_Audio,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(DWORD));
	//�ݒ肵���t�H�[�}�b�g���̎擾
	DWORD dwFormatCount = 0;
	hr = pWMCodecInfo3->GetCodecFormatCount(WMMEDIATYPE_Audio,dwCodecIndex,&dwFormatCount);
	if (dwFormatCount <= 0) return S_OK;
	for (DWORD dwFormatIndex = 0; dwFormatIndex<dwFormatCount;dwFormatIndex++)
	{
		//�t�H�[�}�b�g�Ɋւ��Ă̐������̎擾
		DWORD cchDesc = 0;
		hr = pWMCodecInfo3->GetCodecFormatDesc(WMMEDIATYPE_Audio,dwCodecIndex,dwFormatIndex,NULL,NULL,&cchDesc);
		WCHAR* pwszDesc = new WCHAR[cchDesc];
		hr = pWMCodecInfo3->GetCodecFormatDesc(WMMEDIATYPE_Audio,dwCodecIndex,dwFormatIndex,NULL,pwszDesc,&cchDesc);

		//�X�g���[�~���O�ݒ�̎擾
		IWMStreamConfig* pWMStreamConfig = NULL;
		hr = pWMCodecInfo3->GetCodecFormat(WMMEDIATYPE_Audio,dwCodecIndex,dwFormatIndex,&pWMStreamConfig);
		DWORD dwBitrate = 0;									//�r�b�g���[�g
		hr = pWMStreamConfig->GetBitrate(&dwBitrate);
		//���f�B�A�v���p�e�B�C���^�t�F�[�X�̎擾
		IWMMediaProps* pWMMediaProps = NULL;
		hr = pWMStreamConfig->QueryInterface(IID_IWMMediaProps, (void**)&pWMMediaProps);
		//���f�B�A�^�C�v�̎擾
		DWORD cbMediaType = 0;
		WM_MEDIA_TYPE* pMediaType  = NULL;
		hr = pWMMediaProps->GetMediaType(NULL,&cbMediaType);
		pMediaType = (WM_MEDIA_TYPE*) new BYTE[cbMediaType];
		hr = pWMMediaProps->GetMediaType(pMediaType,&cbMediaType);

		assert(pMediaType->formattype == FORMAT_WaveFormatEx);

		//�E�F�[�u�w�b�_�̎擾�Ɛݒ�
		WAVEFORMATEX *pWaveFormatEx = NULL;
		pWaveFormatEx = (WAVEFORMATEX *)pMediaType->pbFormat;
		WORD nChannels = pWaveFormatEx->nChannels;				//�`���l����
		DWORD nSamplesPerSec = pWaveFormatEx->nSamplesPerSec;	//�T���v�����O���[�g(Hz) 8000,11025,22050,44100
		WORD wBitsPerSample = pWaveFormatEx->wBitsPerSample;	//�r�b�g��(8,12,16)


		//(A/V)���[�h���ǂ����̃`�F�b�N
		BOOL bAVSynchronization = FALSE;
		if((pWaveFormatEx->nAvgBytesPerSec / pWaveFormatEx->nBlockAlign) >= 
			   ((pWaveFormatEx->nAvgBytesPerSec >= 4000) ? 5.0 : 3.0))
		{
			bAVSynchronization = TRUE;
		}


		//�\���̂Ɋi�[
		AudioCodecFormatStruct acfs;
		acfs.strDescription = pwszDesc;
		acfs.dwBitrate = dwBitrate;
		acfs.nChannels = nChannels;
		acfs.nSamplesPerSec = nSamplesPerSec;
		acfs.wBitsPerSample = wBitsPerSample;
		acfs.bAVSynchronization = bAVSynchronization;
		AudioFormat.push_back(acfs);

		delete [] pMediaType;
		pWMMediaProps->Release();
		pWMStreamConfig->Release();
		delete [] pwszDesc;
	}

	return hr;
}
//////////////////////////////////////////////////
//�Ǝ��R�[�f�b�N��K�p����
//
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetOriginalCodec(const bool bAudio,AM_MEDIA_TYPE* pAmt)
{
	assert(pAmt);
	HRESULT hr = NOERROR;

	if (bAudio)
	{
		SAFE_RELEASE(m_pAudioStreamConfig);
		hr = m_pWMProfile->CreateNewStream(WMMEDIATYPE_Audio,&m_pAudioStreamConfig);
		hr = m_pAudioStreamConfig->SetStreamName(L"Audio Stream");
		hr = m_pAudioStreamConfig->SetConnectionName(L"Audio");
		hr = m_pAudioStreamConfig->SetStreamNumber(1);
		hr = m_pAudioStreamConfig->SetBufferWindow(0xFFFFFFFF);	//-1
	} else {
		SAFE_RELEASE(m_pVideoStreamConfig);
		hr = m_pWMProfile->CreateNewStream(WMMEDIATYPE_Video,&m_pVideoStreamConfig);
		hr = m_pVideoStreamConfig->SetStreamName(L"Video Stream");
		hr = m_pVideoStreamConfig->SetConnectionName(L"Video");
		hr = m_pVideoStreamConfig->SetStreamNumber(2);
		hr = m_pVideoStreamConfig->SetBufferWindow(5000);	//�f�t�H���g�̃o�b�t�@����
	}

	WM_MEDIA_TYPE wmt;
	wmt.majortype            = pAmt->majortype;
	wmt.subtype              = pAmt->subtype;
	wmt.formattype           = pAmt->formattype;
	wmt.bFixedSizeSamples    = pAmt->bFixedSizeSamples;
	wmt.bTemporalCompression = pAmt->bTemporalCompression;
	wmt.lSampleSize          = pAmt->lSampleSize;
	wmt.cbFormat             = pAmt->cbFormat;
	wmt.pbFormat             = pAmt->pbFormat;
	wmt.pUnk                 = pAmt->pUnk;

	if (bAudio)
	{
		//�I�[�f�B�I�r�b�g���[�g�l�̐ݒ�
		WAVEFORMATEX* pWaveFormatEx = (WAVEFORMATEX*)pAmt->pbFormat;
		DWORD dwBitrate = 0;
		dwBitrate = pWaveFormatEx->nAvgBytesPerSec * pWaveFormatEx->wBitsPerSample / pWaveFormatEx->nChannels;
		hr = m_pAudioStreamConfig->SetBitrate(dwBitrate);
	} else {
		if (pAmt->formattype == FORMAT_VideoInfo)
		{
			VIDEOINFOHEADER* pVideoInfoHeader = (VIDEOINFOHEADER*)pAmt->pbFormat;
			pVideoInfoHeader->bmiHeader.biSizeImage = 0;
			m_pVideoStreamConfig->SetBitrate(pVideoInfoHeader->dwBitRate);
		} else if (pAmt->formattype == FORMAT_VideoInfo2)
		{
			VIDEOINFOHEADER2* pVideoInfoHeader2 = (VIDEOINFOHEADER2*)pAmt->pbFormat;
			pVideoInfoHeader2->bmiHeader.biSizeImage = 0;
			m_pVideoStreamConfig->SetBitrate(pVideoInfoHeader2->dwBitRate);
		}
		//m_pVideoStreamConfig->SetBitrate(0);
	}

	CComPtr<IWMMediaProps> pWMMediaProps;
	if (bAudio)
		hr = m_pAudioStreamConfig->QueryInterface(IID_IWMMediaProps,(void**)&pWMMediaProps);
	else
		hr = m_pVideoStreamConfig->QueryInterface(IID_IWMMediaProps,(void**)&pWMMediaProps);

	hr = pWMMediaProps->SetMediaType(&wmt);
	CHECK_RESULT("IWMMediaProps SetMediaType.",hr);

	return hr;
}

//////////////////////////////////////////////////
BOOL CDirectShowEncoder::MakeStringToGUID(TCHAR* szGUID,int nSize,GUID &guid)
{
	//{A98C8400-4181-11D1-A520-00A0D10129C0}
	ZeroMemory(szGUID,sizeof(TCHAR)*nSize);
	wsprintf(szGUID,_T("{%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}"),
		guid.Data1,
		guid.Data2,
		guid.Data3,
		guid.Data4[0],
		guid.Data4[1],
		guid.Data4[2],
		guid.Data4[3],
		guid.Data4[4],
		guid.Data4[5],
		guid.Data4[6],
		guid.Data4[7]);
	return TRUE;
}

//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N���̃t�@�C���Z�[�u(�f�o�b�O�p)
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SaveCurrentVideoCodec(wstring strFileName)
{
	assert(m_pVideoStreamConfig);
	WCHAR* szBuffer = NULL;

	//�t�@�C�����J��
	FILE* fp;
	_tfopen_s(&fp,strFileName.c_str(),_T("w, ccs=UTF-8"));

	DWORD dwBitrate;
	m_pVideoStreamConfig->GetBitrate(&dwBitrate);
	_ftprintf_s(fp,_T("Bitrate,%d\n"),dwBitrate);
	DWORD dwBufferWindow;
	m_pVideoStreamConfig->GetBufferWindow(&dwBufferWindow);
	_ftprintf_s(fp,_T("BufferWindow,%d\n"),dwBufferWindow);
	WORD wStreamNum;
	m_pVideoStreamConfig->GetStreamNumber(&wStreamNum);
	_ftprintf_s(fp,_T("StreamNumer,%d\n"),wStreamNum);
	//�ڑ���
	WORD cchInputName;
	m_pVideoStreamConfig->GetConnectionName(NULL,&cchInputName);
	szBuffer = new WCHAR[cchInputName];
	m_pVideoStreamConfig->GetConnectionName(szBuffer,&cchInputName);
	_ftprintf_s(fp,_T("ConnectionName,%s\n"),szBuffer);
	delete [] szBuffer;
	//�X�g���[����
	WORD cchStreamName;
	m_pVideoStreamConfig->GetStreamName(NULL,&cchStreamName);
	szBuffer = new WCHAR[cchStreamName];
	m_pVideoStreamConfig->GetStreamName(szBuffer,&cchStreamName);
	_ftprintf_s(fp,_T("StreamName,%s\n"),szBuffer);
	delete [] szBuffer;

	//
	CComPtr<IWMMediaProps> pWMMediaProps;
	m_pVideoStreamConfig->QueryInterface(IID_IWMMediaProps,(void**)&pWMMediaProps);
	DWORD cbType;
	pWMMediaProps->GetMediaType(NULL,&cbType);
	WM_MEDIA_TYPE* pwmt = (WM_MEDIA_TYPE*)new BYTE[cbType];
	pWMMediaProps->GetMediaType(pwmt,&cbType);

	TCHAR* szGUID = new TCHAR[256];
	MakeStringToGUID(szGUID,256,pwmt->majortype);
	_ftprintf_s(fp,_T("majortype,%s\n"),szGUID);
	MakeStringToGUID(szGUID,256,pwmt->subtype);
	_ftprintf_s(fp,_T("subtype,%s\n"),szGUID);
	_ftprintf_s(fp,_T("bFixedSizeSamples,%s\n"),pwmt->bFixedSizeSamples?_T("TRUE"):_T("FALSE"));
	_ftprintf_s(fp,_T("bTemporalCompression,%s\n"),pwmt->bTemporalCompression?_T("TRUE"):_T("FALSE"));
	_ftprintf_s(fp,_T("lSampleSize,%d\n"),pwmt->lSampleSize);
	MakeStringToGUID(szGUID,256,pwmt->formattype);
	_ftprintf_s(fp,_T("formattype,%s\n"),szGUID);
	_ftprintf_s(fp,_T("pUnk,%d\n"),(int)pwmt->pUnk);
	_ftprintf_s(fp,_T("cbFormat,%d\n"),pwmt->cbFormat);

	if (pwmt->formattype != WMFORMAT_VideoInfo) {
		assert(TRUE == FALSE);
	}
	WMVIDEOINFOHEADER* pwvih = (WMVIDEOINFOHEADER*)pwmt->pbFormat;
	_ftprintf_s(fp,_T("rcSource,{%d-%d-%d-%d}\n"),pwvih->rcSource.left,pwvih->rcSource.top,pwvih->rcSource.right,pwvih->rcSource.bottom);
	_ftprintf_s(fp,_T("rcTarget,{%d-%d-%d-%d}\n"),pwvih->rcTarget.left,pwvih->rcTarget.top,pwvih->rcTarget.right,pwvih->rcTarget.bottom);
	_ftprintf_s(fp,_T("dwBitRate,%d\n"),pwvih->dwBitRate);
	_ftprintf_s(fp,_T("dwBitErrorRate,%d\n"),pwvih->dwBitErrorRate);
	_ftprintf_s(fp,_T("AvgTimePerFrame,%I64d\n"),pwvih->AvgTimePerFrame);
	_ftprintf_s(fp,_T("bmiHeader.biSize,%d\n"),pwvih->bmiHeader.biSize);
	_ftprintf_s(fp,_T("bmiHeader.biWidth,%d\n"),pwvih->bmiHeader.biWidth);
	_ftprintf_s(fp,_T("bmiHeader.biHeight,%d\n"),pwvih->bmiHeader.biHeight);
	_ftprintf_s(fp,_T("bmiHeader.biPlanes,%d\n"),pwvih->bmiHeader.biPlanes);
	_ftprintf_s(fp,_T("bmiHeader.biBitCount,%d\n"),pwvih->bmiHeader.biBitCount);
	_ftprintf_s(fp,_T("bmiHeader.biCompression,%d\n"),pwvih->bmiHeader.biCompression);
	_ftprintf_s(fp,_T("bmiHeader.biSizeImage,%d\n"),pwvih->bmiHeader.biSizeImage);
	_ftprintf_s(fp,_T("bmiHeader.biXPelsPerMeter,%d\n"),pwvih->bmiHeader.biXPelsPerMeter);
	_ftprintf_s(fp,_T("bmiHeader.biYPelsPerMeter,%d\n"),pwvih->bmiHeader.biYPelsPerMeter);
	_ftprintf_s(fp,_T("bmiHeader.biClrUsed,%d\n"),pwvih->bmiHeader.biClrUsed);
	_ftprintf_s(fp,_T("bmiHeader.biClrImportant,%d\n"),pwvih->bmiHeader.biClrImportant);

	delete [] (BYTE*)pwmt;
	delete [] szGUID;
	fclose(fp);
	return S_OK;
}

//////////////////////////////////////////////////
//�I�[�f�B�I�ƃr�f�I�̃R�[�f�b�N�̃x�[�X��ݒ肷��T�u���[�`��
//bAudio         true=�I�[�f�B�I,false=�r�f�I
//dwCodecIndex   �R�[�f�b�N�̃C���f�b�N�X�ԍ�
//dwFormatIndex�@�R�[�f�b�N�̃t�H�[�}�b�g�ԍ�
//format_bitrate �r�b�g���[�g�̃^�C�v(CODEC_FORMAT_CBR,CODEC_FORMAT_VBR)
//format_numpass 1�p�X2�p�X�G���R�[�h(CODEC_FORMAT_ONEPASS,CODEC_FORMAT_TWOPASS)
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetCodec(const bool bAudio,const DWORD dwCodecIndex,const DWORD dwFormatIndex,CODEC_FORMAT_BITRATE format_bitrate,CODEC_FORMAT_NUMPASS format_numpass)
{
	assert(dwCodecIndex >= 0);
	assert(dwFormatIndex >= 0);

	HRESULT hr = NOERROR;
	GUID guidType;
	if (bAudio)	guidType = WMMEDIATYPE_Audio;
	else		guidType = WMMEDIATYPE_Video;

	//�v���t�@�C���}�l�[�W���[����R�[�f�b�N���C���^�t�F�[�X���擾
	CComPtr<IWMProfileManager> pWMProfileManager;
	hr = WMCreateProfileManager(&pWMProfileManager);
	CComPtr<IWMCodecInfo3> pWMCodecInfo3;
	hr =pWMProfileManager->QueryInterface(IID_IWMCodecInfo3,(void**)&pWMCodecInfo3);

	//�R�[�f�b�N���̎擾
	DWORD dwCodecs = 0;
	hr = pWMCodecInfo3->GetCodecInfoCount(guidType, &dwCodecs);
	if (dwCodecIndex >= dwCodecs) return E_FAIL;

	//�t�H�[�}�b�g�̐ݒ�
	BOOL bVBREnabled = FALSE;
	if (format_bitrate == CODEC_FORMAT_VBR)
		bVBREnabled = TRUE;
	DWORD dwNumPasses  = 1;
	if (format_numpass == CODEC_FORMAT_TWOPASS)
		dwNumPasses = 1;
	hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszVBREnabled,WMT_TYPE_BOOL,(BYTE*)&bVBREnabled,sizeof(BOOL));
	hr = pWMCodecInfo3->SetCodecEnumerationSetting(guidType,dwCodecIndex,g_wszNumPasses,WMT_TYPE_DWORD,(BYTE*)&dwNumPasses,sizeof(DWORD));

	//�ݒ肵���t�H�[�}�b�g���̎擾
	DWORD dwFormatCount = 0;
	hr = pWMCodecInfo3->GetCodecFormatCount(guidType,dwCodecIndex,&dwFormatCount);
	if (dwFormatIndex >= dwFormatCount) return E_FAIL;

	//�X�g���[���ݒ�̎擾���A�I�[�f�B�I/�r�f�I�G���R�[�_�Ƃ��Đݒ�
	if (bAudio) {
		SAFE_RELEASE(m_pAudioStreamConfig);
		hr = pWMCodecInfo3->GetCodecFormat(guidType,dwCodecIndex,dwFormatIndex,&m_pAudioStreamConfig);
		hr = m_pAudioStreamConfig->SetStreamName(L"Audio Stream");
		hr = m_pAudioStreamConfig->SetConnectionName(L"Audio");
		hr = m_pAudioStreamConfig->SetStreamNumber(1);
	} else {
		SAFE_RELEASE(m_pVideoStreamConfig);
		hr = pWMCodecInfo3->GetCodecFormat(guidType,dwCodecIndex,dwFormatIndex,&m_pVideoStreamConfig);
		hr = m_pVideoStreamConfig->SetStreamName(L"Video Stream");
		hr = m_pVideoStreamConfig->SetConnectionName(L"Video");
		hr = m_pVideoStreamConfig->SetStreamNumber(2);
	}
	return hr;
}

//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N�̕ҏW���ڂ�ҏW
//const DWORD dwBitrate	�r�b�g���[�g
//const DWORD msBufferWindow �o�b�t�@����(�P�ʂ�ms) -1�Ŏ����ݒ�(�����ݒ�ł�3sec=3000ms)
//const REFERENCE_TIME AvgTimePerFrame 1�t���[���̕��Ϗ�������(��fps)
//const RECT rcSource ���\�[�X��RECT�\����
//const RECT rcTarget �g��k�����RECT�\����
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetVideoSetting(const DWORD dwBitrate,const DWORD msBufferWindow,const REFERENCE_TIME AvgTimePerFrame,const RECT rcSource,const RECT rcTarget)
{
	assert(m_pVideoStreamConfig);
	HRESULT hr = NOERROR;
	//�r�b�g���[�g�ƃo�b�t�@�T�C�Y�̐ݒ�
	hr = m_pVideoStreamConfig->SetBitrate(dwBitrate);
	if (!CHECK_RESULT("IVideoStreamConfig SetBitrate.",hr)){return hr;}
	hr = m_pVideoStreamConfig->SetBufferWindow(msBufferWindow);
	if (!CHECK_RESULT("IVideoStreamConfig SetBufferWindow.",hr)){return hr;}

	//���f�B�A�v���p�e�B�C���^�t�F�[�X�̎擾
	CComPtr<IWMMediaProps> pWMMediaProps;
	hr = m_pVideoStreamConfig->QueryInterface(IID_IWMMediaProps, (void**)&pWMMediaProps);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IWMMediaProps.",hr)){return hr;}

	//���f�B�A�^�C�v�̎擾
	DWORD cbMediaType = 0;
	WM_MEDIA_TYPE* pMediaType  = NULL;
	hr = pWMMediaProps->GetMediaType(NULL,&cbMediaType);
	if (!CHECK_RESULT("IWMMediaProps GetMediaType.",hr)){return hr;}

	pMediaType = (WM_MEDIA_TYPE*) new BYTE[cbMediaType];
	hr = pWMMediaProps->GetMediaType(pMediaType,&cbMediaType);
	if (!CHECK_RESULT("IWMMediaProps GetMediaType.",hr)){
		//CDirectShowEtc::ReleaseMediaType(*(AM_MEDIA_TYPE*)pMediaType);
		delete [] pMediaType;
		return hr;}

	//TODO WMV��VIF2�ŏo�͂����炵��
	if (pMediaType->formattype == FORMAT_VideoInfo) {
		//�r�f�I�w�b�_�̎擾�Ɛݒ�
		VIDEOINFOHEADER	*pVideoInfoHeader = NULL;
		pVideoInfoHeader = (VIDEOINFOHEADER	*)pMediaType->pbFormat;
		pVideoInfoHeader->rcSource = rcSource;
		pVideoInfoHeader->rcTarget = rcTarget;
		//�r�b�g���[�g�̐ݒ�
		pVideoInfoHeader->dwBitRate = dwBitrate;
		//FPS�̐ݒ�
		pVideoInfoHeader->AvgTimePerFrame = AvgTimePerFrame;
		//��ʃT�C�Y�̐ݒ�
		pVideoInfoHeader->bmiHeader.biWidth = rcTarget.right - rcTarget.left;
		pVideoInfoHeader->bmiHeader.biHeight = rcTarget.bottom - rcTarget.top;
	} else if (pMediaType->formattype == FORMAT_VideoInfo2) {
		//�r�f�I�w�b�_�̎擾�Ɛݒ�
		VIDEOINFOHEADER2 *pVideoInfoHeader2 = NULL;
		pVideoInfoHeader2 = (VIDEOINFOHEADER2*)pMediaType->pbFormat;
		pVideoInfoHeader2->rcSource = rcSource;
		pVideoInfoHeader2->rcTarget = rcTarget;
		//�r�b�g���[�g�̐ݒ�
		pVideoInfoHeader2->dwBitRate = dwBitrate;
		//FPS�̐ݒ�
		pVideoInfoHeader2->AvgTimePerFrame = AvgTimePerFrame;
		//��ʃT�C�Y�̐ݒ�
		pVideoInfoHeader2->bmiHeader.biWidth = rcTarget.right - rcTarget.left;
		pVideoInfoHeader2->bmiHeader.biHeight = rcTarget.bottom - rcTarget.top;
	} else {
		//CDirectShowEtc::ReleaseMediaType(*(AM_MEDIA_TYPE*)pMediaType);
		delete [] pMediaType;
		return E_FAIL;
	}

	hr = pWMMediaProps->SetMediaType(pMediaType);
	if (!CHECK_RESULT("IWMMediaProps SetMediaType.",hr)){
		//CDirectShowEtc::ReleaseMediaType(*(AM_MEDIA_TYPE*)pMediaType);
		delete [] pMediaType;
		return hr;}

	//CDirectShowEtc::ReleaseMediaType(*(AM_MEDIA_TYPE*)pMediaType);
	delete [] pMediaType;
	return hr;
}
//////////////////////////////////////////////////
//�r�f�I�R�[�f�b�N�̕ҏW���ڂ�ҏW
//const DWORD dwQuality�@�N�I���e�B
//const REFERENCE_TIME MaxKeyFrameSpacing	�L�[�t���[���̊Ԋu
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetVideoSetting2(const DWORD dwQuality,const REFERENCE_TIME MaxKeyFrameSpacing)
{
	assert(m_pVideoStreamConfig);
	HRESULT hr = NOERROR;

	CComPtr<IWMVideoMediaProps> pWMVideoMediaProps;
	hr = m_pVideoStreamConfig->QueryInterface(IID_IWMVideoMediaProps,(void**)&pWMVideoMediaProps);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IWMVideoMediaProps.",hr)){return hr;}

	//�N�I���e�B�̐ݒ�(0�`100�܂ŁB0�ɋ߂��قǃt���[���D��,100�ɋ߂��قǉ掿�D��)
	//�l�͈̔͂ɂ����͈̂ȉ��̒ʂ�
	// 0�`19	�ł����炩
	//20�`39	��芊�炩
	//40�`59	���U
	//60�`79	���N��
	//80�`100	�ł��N��
	hr = pWMVideoMediaProps->SetQuality(dwQuality);
	if (!CHECK_RESULT("IWMVideoMediaProps SetQuality.",hr)){return hr;}

	//�L�[�t���[���̊Ԋu��100�i�m�b�Ŏw��
	//�l�͈̔͂�1sec�`30sec�܂łɗ��߂Ă���
	//�}�C�N���\�t�g�̌��̒l http://msdn.microsoft.com/en-us/library/dd757001%28VS.85%29.aspx
	// 22Kbps�`300Kbps	8sec	(80000000)
	//300Kbps�`600Kbps	6sec	(60000000)
	//600Kbps�`  2Mbps	4sec	(40000000)
	//  2Mbps�`			3sec	(30000000)
	hr = pWMVideoMediaProps->SetMaxKeyFrameSpacing(MaxKeyFrameSpacing);
	if (!CHECK_RESULT("IWMVideoMediaProps SetMaxKeyFrameSpacing.",hr)){return hr;}

	return hr;
}
//////////////////////////////////////////////////
//�r�f�I�G���R�[�_�̃p�t�H�[�}���X�̐ݒ�
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetVideoComplexity(WORD wComplexity)
{
	assert(m_pVideoStreamConfig);
	HRESULT hr = NOERROR;

	CComPtr<IWMPropertyVault> pWMPropertyVault;
	hr = m_pVideoStreamConfig->QueryInterface(IID_IWMPropertyVault,(void**)&pWMPropertyVault);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IWMPropertyVault.",hr)){return hr;}

	WMT_ATTR_DATATYPE pType = WMT_TYPE_WORD;
	DWORD dwSize = sizeof(wComplexity);
	hr = pWMPropertyVault->SetProperty(g_wszComplexity,pType,(BYTE*)&wComplexity,dwSize);
	if (!CHECK_RESULT("IWMPropertyVault SetProperty g_wszComplexity.",hr)){return hr;}
	return hr;
}
/*
//////////////////////////////////////////////////
//�R�[�f�b�N�̃v���p�e�B��ݒ�
//TODO IWMPropertyVault
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetCodecProperty()
{
	assert(m_pVideoStreamConfig);
	HRESULT hr = NOERROR;

	CComPtr<IWMCodecInfo3> pWMCodecInfo3;
	hr = m_pVideoStreamConfig->QueryInterface(IID_IWMCodecInfo3,(void**)&pWMCodecInfo3);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IWMCodecInfo3.",hr)){return hr;}

	CComPtr<IWMPropertyVault> pWMPropertyVault;
	hr = m_pVideoStreamConfig->QueryInterface(IID_IWMPropertyVault,(void**)&pWMPropertyVault);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IWMPropertyVault.",hr)){return hr;}

	//g_wszWMVCComplexityMode
	//g_wszComplexity
	DWORD dwCount;
	hr = pWMPropertyVault->GetPropertyCount(&dwCount);
	for (int i=0;i<(int)dwCount;i++)
	{
		DWORD dwNameLen;
		DWORD dwSize;
		WCHAR* szName;
		WMT_ATTR_DATATYPE Type;
		pWMPropertyVault->GetPropertyByIndex(i,NULL,&dwNameLen,&Type,NULL,&dwSize);
		szName = new WCHAR[dwNameLen];
		pWMPropertyVault->GetPropertyByIndex(i,szName,&dwNameLen,&Type,NULL,&dwSize);
		delete [] szName;
	}
	//pWMPropertyVault->SetProperty(g_wszWMVCComplexityMode,VT_I4,

	WORD wComp = 10;
	hr = pWMPropertyVault->SetProperty(g_wszComplexity,WMT_TYPE_WORD,(BYTE*)&wComp,sizeof(wComp));

	WORD wCompEx = 10;
	hr = pWMPropertyVault->SetProperty(g_wszWMVCComplexityEx,WMT_TYPE_WORD,(BYTE*)&wCompEx,sizeof(wCompEx));

	WORD wThread = 2;
	hr = pWMPropertyVault->SetProperty(g_wszWMVCNumThreads,WMT_TYPE_WORD,(BYTE*)&wThread,sizeof(wThread));

	BSTR strProp = NULL;
	WMT_ATTR_DATATYPE Type;
	DWORD dwSize = sizeof(strProp);
	hr = pWMPropertyVault->GetPropertyByName(g_wszWMVCDecoderComplexityProfile,&Type,(BYTE*)strProp,&dwSize);

	CComPtr<IPropertyBag> pPropertyBag;
	hr = pWMPropertyVault->QueryInterface(IID_IPropertyBag,(void**)&pPropertyBag);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IMediaObject.",hr)){return hr;}

	CComPtr<IMediaObject> pMediaObject;
	hr = m_pVideoStreamConfig->QueryInterface(IID_IMediaObject,(void**)&pMediaObject);
	if (!CHECK_RESULT("IVideoStreamConfig QueryInterface IMediaObject.",hr)){return hr;}


	WMT_ATTR_DATATYPE Type;
	DWORD dwSize = sizeof(double);
	double d;
	hr = pWMPropertyVault->GetPropertyByName(g_wszWMVCAvgFrameRate,&Type,(BYTE*)&d,&dwSize);

	return hr;
}
*/
//////////////////////////////////////////////////
//�ő�E�ŏ��p�P�b�g�T�C�Y�̐ݒ�
//�p�P�b�g�T�C�Y�̐ݒ�l�ɂ��Ă͈ȉ���URL���Q�Ƃ��邱��
//http://msdn.microsoft.com/ja-jp/library/cc294579.aspx
//http://74.125.155.132/search?q=cache:OS3XqK5a_lQJ:207.46.192.254/windows/windowsmedia/ja/9series/encoder/readme.aspx+maxpacket+minpacket&cd=3&hl=ja&ct=clnk&gl=jp&lr=lang_ja&client=firefox-a
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::SetPacketSize(const DWORD dwMinPacketSize, const DWORD dwMaxPacketSize)
{
	assert(m_pWMProfile);
	HRESULT hr = NOERROR;
	/*
	if (dwMinPacketSize != 0 && dwMinPacketSize < 100)
		return E_FAIL;
		*/
	if (dwMaxPacketSize > 100 || dwMaxPacketSize < 64000)
		return E_FAIL;
	CComPtr<IWMPacketSize2> pWMPacketSize2;
	hr = m_pWMProfile->QueryInterface(IID_IWMPacketSize2,(void**)&pWMPacketSize2);
	DWORD dwMin = 0;
	DWORD dwMax = 0;
	hr = pWMPacketSize2->GetMinPacketSize(&dwMin);
	hr = pWMPacketSize2->GetMaxPacketSize(&dwMax);
	if (dwMinPacketSize != dwMin) {
		hr = pWMPacketSize2->SetMinPacketSize(dwMinPacketSize);
		if (!CHECK_RESULT("IWMPacketSize2 SetMinPacketSize.",hr)){return hr;}
	}
	if (dwMaxPacketSize != dwMax) {
		hr = pWMPacketSize2->SetMaxPacketSize(dwMaxPacketSize);
		if (!CHECK_RESULT("IWMPacketSize2 SetMaxPacketSize.",hr)){return hr;}
	}
	return hr;
}

//////////////////////////////////////////////////
//���v�r�b�g���[�g�̎擾
//DWORD* pdwAudioBitrate �I�[�f�B�I�r�b�g���[�g�̒l(bps)
//DWORD* pdwVideoBitrate �r�f�I�r�b�g���[�g�̒l(bps)
//DWORD* pdwAddedBitrate �ǉ��r�b�g���[�g�̒l(bps)
//DWORD* pdwTotalBitrate ���v�r�b�g���[�g�̒l(bps)
//////////////////////////////////////////////////
HRESULT CDirectShowEncoder::GetTotalBitrate(DWORD* pdwAudioBitrate,DWORD* pdwVideoBitrate,DWORD* pdwAddedBitrate,DWORD* pdwTotalBitrate)
{
	HRESULT hr = NOERROR;
	DWORD dwAudioBitrate = 0;	//�I�[�f�B�I�r�b�g���[�g(bps)
	DWORD dwVideoBitrate = 0;	//�r�f�I�r�b�g���[�g(bps)
	DWORD dwAddedBitrate = 0;	//�t�����ꂽ�I�[�o�[�w�b�h�r�b�g���[�g(bps)
	DWORD dwTotalBitrate = 0;	//���v�r�b�g���[�g(bps)
	//�I�[�f�B�I�r�b�g���[�g�̎擾
	if (m_pAudioStreamConfig) {
		hr = m_pAudioStreamConfig->GetBitrate(&dwAudioBitrate);
	}
	//�r�f�I�r�b�g���[�g�̎擾
	if (m_pVideoStreamConfig) {
		hr = m_pVideoStreamConfig->GetBitrate(&dwVideoBitrate);
	}
	dwTotalBitrate = dwAudioBitrate + dwVideoBitrate;
	//�I�[�o�[�w�b�h�r�b�g���[�g�̌v�Z http://msdn.microsoft.com/en-us/library/dd743727%28VS.85%29.aspx
	if (dwTotalBitrate <= 16000) {
		dwAddedBitrate = 3000;}
	else if (dwTotalBitrate > 16000 && dwTotalBitrate <= 30000) {
		dwAddedBitrate = 4000;}
	else if (dwTotalBitrate > 30000 && dwTotalBitrate <= 45000) {
		dwAddedBitrate = 5000;}
	else if (dwTotalBitrate > 45000 && dwTotalBitrate <= 70000) {
		dwAddedBitrate = 6000;}
	else if (dwTotalBitrate > 70000 && dwTotalBitrate <= 225000) {
		dwAddedBitrate = 7000;}
	else if (dwTotalBitrate > 225000) {
		dwAddedBitrate = 9000;}
	//���v�r�b�g���[�g�ɃI�[�o�[�w�b�h�r�b�g���[�g��t��
	dwTotalBitrate += dwAddedBitrate;

	if (pdwAudioBitrate)
		*pdwAudioBitrate = dwAudioBitrate;
	if (pdwVideoBitrate)
		*pdwVideoBitrate = dwVideoBitrate;
	if (pdwAddedBitrate)
		*pdwAddedBitrate = dwAddedBitrate;
	if (pdwTotalBitrate)
		*pdwTotalBitrate = dwTotalBitrate;

	return hr;
}
