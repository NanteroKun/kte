#include "StdAfx.h"
#include "DirectShowAudioVolume.h"

//////////////////////////////////////////////////
//�R���X�g���N�^
//////////////////////////////////////////////////
CDirectShowAudioVolume::CDirectShowAudioVolume(void)
	:   m_pFilter(NULL),
		m_pSampleGrabber(NULL),
		m_pAudioVolumeCallback(NULL),
		m_hWnd(NULL),
		m_pddB(NULL),
		m_nChannels(0),
		m_bEnable(FALSE)
{
	ZeroMemory(&m_ConnectionMediaType,sizeof(AM_MEDIA_TYPE));
}

//////////////////////////////////////////////////
//�f�X�g���N�^
//////////////////////////////////////////////////
CDirectShowAudioVolume::~CDirectShowAudioVolume(void)
{
	assert(!m_pFilter);
	assert(!m_pSampleGrabber);
	assert(!m_pAudioVolumeCallback);
	CDirectShowEtc::ReleaseMediaType(m_ConnectionMediaType);
}

//////////////////////////////////////////////////
//�T���v���O���o���쐬����
//////////////////////////////////////////////////
HRESULT CDirectShowAudioVolume::Create(IGraphBuilder* pGraphBuilder)
{
	assert(pGraphBuilder);
	if (!pGraphBuilder) return E_POINTER;
	if (m_pFilter)
		Delete(pGraphBuilder);

	//�t�B���^�̐���
	HRESULT hr = NOERROR;
	hr = CoCreateInstance(CLSID_SampleGrabber,
		NULL,CLSCTX_INPROC_SERVER,IID_IBaseFilter,(void **)&m_pFilter);
	if (FAILED(hr)) return hr;
	hr = pGraphBuilder->AddFilter(m_pFilter,_T("Audio Volume SampleGrabber"));
	if (FAILED(hr)) return hr;

	//Filter����ISampleGrabber�C���^�[�t�F�[�X���擾
	if (m_pSampleGrabber) {
		SAFE_RELEASE(m_pSampleGrabber);}

	hr = m_pFilter->QueryInterface(IID_ISampleGrabber,(void **)&m_pSampleGrabber);
	if (FAILED(hr)) return hr;

	//SampleGrabber�̐ڑ����f�B�A�^�C�v��ݒ�
	AM_MEDIA_TYPE amt;
	ZeroMemory(&amt, sizeof(amt));
	amt.majortype = MEDIATYPE_Audio;
	amt.subtype = MEDIASUBTYPE_PCM;
	amt.formattype = FORMAT_WaveFormatEx;
	hr = m_pSampleGrabber->SetMediaType(&amt);

	//�t�B���^����ʂ�T���v�����o�b�t�@�ɃR�s�[���邩�ǂ������w�肷��
	hr = m_pSampleGrabber->SetBufferSamples(FALSE);
	//��L�֐����T���v����ON/OFF��؂�ւ���

	//�����V���b�g���[�h��OFF�ɂ���
	hr = m_pSampleGrabber->SetOneShot(FALSE);

	//�R�[���o�b�N�C���^�t�F�[�X�̐ݒ�
	m_pAudioVolumeCallback = new CDirectShowAudioVolume::CDirectShowAudioVolumeCB(this);
	hr = m_pSampleGrabber->SetCallback(m_pAudioVolumeCallback,0);
	//0�Ȃ�SampleCB�^�C�v��1�Ȃ�BufferCB�^�C�v

	//�ڑ����f�B�A�^�C�v�̏���
	CDirectShowEtc::ReleaseMediaType(m_ConnectionMediaType);

	//���ʂ̏�����
	m_nChannels = 0;
	if (m_pddB) {
		delete [] m_pddB;
		m_pddB = NULL;
	}

	m_hWnd = NULL;
	return hr;
}

//////////////////////////////////////////////////
//�T���v���O���o���폜����
//////////////////////////////////////////////////
void CDirectShowAudioVolume::Delete(IGraphBuilder* pGraphBuilder)
{
	HRESULT hr = NOERROR;
	assert(pGraphBuilder);
	if (!pGraphBuilder)
		return;

	//�C���^�t�F�[�X�̉��
	if (m_pSampleGrabber) {
		SAFE_RELEASE(m_pSampleGrabber);
	}
	//�t�B���^�O���t����폜
	if (m_pFilter) {
		hr = pGraphBuilder->RemoveFilter(m_pFilter);
		SAFE_RELEASE(m_pFilter);
	}
	//�R�[���o�b�N�̍폜�͍Ō��
	if (m_pAudioVolumeCallback){
		delete m_pAudioVolumeCallback;
		m_pAudioVolumeCallback = NULL;
	}

	//���ʏ��̍폜
	m_nChannels = 0;
	if (m_pddB) {
		delete [] m_pddB;
		m_pddB = NULL;
	}

	m_bEnable = FALSE;

	return;
}
//////////////////////////////////////////////////
//�ڑ����f�B�A�^�C�v�̎擾(�ڑ���ɌĂяo��)
//////////////////////////////////////////////////
HRESULT CDirectShowAudioVolume::GetConnectionMediaType()
{
	//�ڑ����f�B�A�^�C�v�̏���
	CDirectShowEtc::ReleaseMediaType(m_ConnectionMediaType);
	//�ڑ����f�B�A�^�C�v�̎擾
	HRESULT hr = NOERROR;
	hr = m_pSampleGrabber->GetConnectedMediaType(&m_ConnectionMediaType);
	return hr;
}
//////////////////////////////////////////////////
//�ڑ����f�B�A�^�C�v�̕ύX�ʒm (private)
//////////////////////////////////////////////////
HRESULT CDirectShowAudioVolume::SetMediaType(AM_MEDIA_TYPE* pamt)
{
	CDirectShowEtc::ReleaseMediaType(m_ConnectionMediaType);
	m_ConnectionMediaType = (*pamt);
	return S_OK;
}
//////////////////////////////////////////////////
//�ڑ����f�B�A�^�C�v�̎擾 (private)
//////////////////////////////////////////////////
HRESULT CDirectShowAudioVolume::GetMediaType(AM_MEDIA_TYPE** ppamt)
{
	(*ppamt) = &m_ConnectionMediaType;
	return S_OK;
}

//////////////////////////////////////////////////
//�f�V�x���f�[�^��o�^���A�e�E�B���h�E�ɒʒm����(WM_SENDDBDATA�𑗂邾��)
//////////////////////////////////////////////////
HRESULT CDirectShowAudioVolume::SenddBData(int nChannels,double* pddB)
{
	if (m_nChannels != nChannels) {
		assert(nChannels != 0);
		if (m_pddB) {
			delete [] m_pddB;
		}
		m_pddB = new double[nChannels];
		m_nChannels = nChannels;
	}
	for (int i=0;i<m_nChannels;i++) {
		m_pddB[i] = pddB[i];
	}

	if (!m_hWnd)
		return E_FAIL;
	PostMessage(m_hWnd,WM_SENDDBDATA,0,0);
	return S_OK;
}

//////////////////////////////////////////////////
//�T���v���O���o��ʉ߂����ۂɌĂ΂��R�[���o�b�N
//////////////////////////////////////////////////
STDMETHODIMP CDirectShowAudioVolume::CDirectShowAudioVolumeCB::SampleCB(double Time, IMediaSample *pSample)
{
	HRESULT hr = NOERROR;

	//���f�B�A�^�C�v�̕ύX�ʒm�̗L���̊m�F
	AM_MEDIA_TYPE* pamt = NULL;
	hr = pSample->GetMediaType(&pamt);
	if (hr == S_OK) {
		//���f�B�A�^�C�v���ύX����Ă���̂ŕύX�ʒm
		m_pParentClass->SetMediaType(pamt);
	}

	//�L���v�`���������������ꍇ�̏���
	if (m_pParentClass->IsCaptureEnabled() == FALSE)
		return S_OK;

	//���f�B�A�^�C�v���擾
	hr = m_pParentClass->GetMediaType(&pamt);
	if (pamt == NULL)
		return S_OK;

	//���f�B�A�^�C�v����e��K�v�ȏ������o��
	WAVEFORMATEX* pWaveFormatEx = NULL;
	if (pamt->formattype == FORMAT_WaveFormatEx) {
		pWaveFormatEx = (WAVEFORMATEX*)pamt->pbFormat;
	}
	if (!pWaveFormatEx)
		return S_OK;

	//�T���v���̎擾
	long lSampleSize = pSample->GetSize();	//�T���v���̃T�C�Y
	BYTE* pData = NULL;						//�T���v���̃f�[�^
	hr = pSample->GetPointer(&pData);
	if (FAILED(hr) || pData == NULL)
		return S_OK;

	long* plMin = new long[pWaveFormatEx->nChannels];		//�ő剹��
	long* plMax = new long[pWaveFormatEx->nChannels];		//�ŏ�����
	double* pddB = new double[pWaveFormatEx->nChannels];	//�f�V�x���ϊ��l
	for (int i=0;i<pWaveFormatEx->nChannels;i++) {
		plMin[i] = LONG_MAX;
		plMax[i] = LONG_MIN;
		pddB[i] = -90,0;
	}
	BYTE* pNowPtr = pData;
	BYTE* pEndPtr = pData + lSampleSize;
	int nData;
	while (pNowPtr < pEndPtr)
	{
		for (int i=0;i<pWaveFormatEx->nChannels;i++){
			//bit���Ƃɖ����̒l���قȂ�̂Œ���
			switch (pWaveFormatEx->wBitsPerSample)
			{
			case 8:
				//8bit�Ȃ�Ε�������(0�`255,������ 128)
				nData = ((long) *((BYTE*)pNowPtr)) - 128;
				pNowPtr++;
				break;
			case 16:
				//16bit�Ȃ�Ε����t��(-32768 �` +32767, ������ 0)
				nData = (long) *((signed short*)pNowPtr);
				pNowPtr++;
				pNowPtr++;
				break;
			case 24:
				//24bit�̏ꍇ�̓��g���G�C�f�B�A���Ŋi�[����Ă���
				//������0�Ȃ̂��ǂ��Ȃ̂�������Ȃ����A������8388608�Ɖ��肵�Ă݂�
				nData = (long) (*((BYTE*)pNowPtr));
				pNowPtr++;
				nData |= (long) ((*((BYTE*)pNowPtr)) << 8);
				pNowPtr++;
				nData |= (long) ((*((BYTE*)pNowPtr)) << 16);
				pNowPtr++;
				nData -= 8388608;
			default:
				nData = 0;
				break;
			}
			plMin[i] = min(plMin[i],nData);
			plMax[i] = max(plMax[i],nData);
		}
	}

	//���ʂ��f�V�x���ɕϊ�
	for (int i=0;i<pWaveFormatEx->nChannels;i++)
	{
		nData = max(labs(plMin[i]),labs(plMax[i]));
		switch (pWaveFormatEx->wBitsPerSample)
		{
		case 8:
			pddB[i] = 20 * log10((double)nData / (double)(SCHAR_MAX+1));
			break;
		case 16:
			pddB[i] = 20 * log10((double)nData / (double)(SHRT_MAX+1));
			break;
		case 24:
			pddB[i] = 20 * log10((double)nData / (double)(8388608));	//2^24/2 = 8388608
			break;
		}
	}

	//�e�N���X�Ƀf�V�x���̃f�[�^�𑗂�
	m_pParentClass->SenddBData(pWaveFormatEx->nChannels,pddB);

	delete [] pddB;
	delete [] plMin;
	delete [] plMax;

	//�|�C���^�̍폜�͕s�v
	//delete [] pData;

	return S_OK;
}

