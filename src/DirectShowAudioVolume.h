#pragma once

#include "DirectShow.h"



//�ʒm�p��WM���b�Z�[�W
#define WM_SENDDBDATA WM_APP + 108


//////////////////////////////////////////////////
//DirectShow WM ASF Writer�Ɋւ���N���X
//////////////////////////////////////////////////
class CDirectShowAudioVolume
{
public:
	//�R���X�g���N�^
	CDirectShowAudioVolume(void);
	//�f�X�g���N�^
	virtual ~CDirectShowAudioVolume(void);

	//�T���v���O���o���쐬����
	HRESULT Create(IGraphBuilder* pGraphBuilder);
	//�T���v���O���o���폜����
	void Delete(IGraphBuilder* pGraphBuilder);
	//�T���v���O���o�t�B���^�̎擾
	inline IBaseFilter* GetFilter(){assert(m_pFilter);return m_pFilter;}

	//�ʒm�p�̃E�B���h�E�n���h����ݒ�
	void SetHWnd(HWND hWnd){m_hWnd = hWnd;}

	//�L���v�`���L���E�����̐؂�ւ�
	void EnableCapture(BOOL bEnable) {m_bEnable = bEnable;}
	BOOL IsCaptureEnabled() const {return m_bEnable;}

	//�ڑ����f�B�A�^�C�v�̎擾 (�t�B���^�ڑ���ɕK���Ă�)
	HRESULT GetConnectionMediaType();

	//�`�����l�����̎擾
	int GetChannels() const {return m_nChannels;}
	//�f�V�x�����̎擾
	BOOL GetdB(double *pddB) {
		for (int i=0;i<m_nChannels;i++) {
			(*pddB) = m_pddB[i];
			pddB++;
		}
		return TRUE;
	}

private:
	HRESULT SetMediaType(AM_MEDIA_TYPE* pamt);
	HRESULT GetMediaType(AM_MEDIA_TYPE** ppamt);

	//�f�V�x���f�[�^�𑗐M
	HRESULT SenddBData(int nChannels,double* pddB);

	//���ɕK�v�Ȋ֐��͉�������H

private:

	//////////////////////////////////////////////////
	//���ʃ��[�^�[�̃T���v���O���o�̃R�[���o�b�N�N���X
	class CDirectShowAudioVolumeCB : public ISampleGrabberCB
	{
	public:
		//�R���X�g���N�^
		CDirectShowAudioVolumeCB(CDirectShowAudioVolume* pParentClass) {
			m_pParentClass = pParentClass;
		}
		//�Q�ƃJ�E���g�݂̂�����
		STDMETHODIMP_(ULONG) AddRef() { return 1; }
		STDMETHODIMP_(ULONG) Release() { return 2; }
		//�C���^�t�F�[�X�̒�
		STDMETHODIMP QueryInterface(REFIID riid, void **ppvObject)
		{
			if (ppvObject == NULL) return E_POINTER;
			if (riid == __uuidof(IUnknown)) {
				*ppvObject = static_cast<IUnknown*>(this);
				return S_OK;
			} else if (riid == __uuidof(ISampleGrabberCB)) {
				*ppvObject = static_cast<ISampleGrabberCB*>(this);
				return S_OK;
			}
			return E_NOTIMPL;
		}
		//�T���v���̒ʉ�(IMediaSample�^)
		STDMETHODIMP SampleCB(double Time, IMediaSample *pSample);/* {return E_NOTIMPL;}*/
		//�T���v���̒ʉ�(BYTE�^) �������͎g��Ȃ�
		STDMETHODIMP BufferCB(double Time, BYTE *pBuffer, long BufferLen) {return E_NOTIMPL;}
	private:
		CDirectShowAudioVolume* m_pParentClass;	//�e�|�C���^
	};

	//////////////////////////////////////////////////
	//�����o�ϐ�(��������)
	IBaseFilter* m_pFilter;								//�t�B���^
	ISampleGrabber* m_pSampleGrabber;					//�T���v���O���o�C���^�t�F�[�X
	CDirectShowAudioVolumeCB* m_pAudioVolumeCallback;	//�R�[���o�b�N
	AM_MEDIA_TYPE m_ConnectionMediaType;				//���݂̐ڑ����f�B�A�^�C�v
	HWND m_hWnd;										//�ʒm�p�̃E�B���h�E�n���h��
	double* m_pddB;										//���ʃf�[�^�̃|�C���^
	int     m_nChannels;								//���ʃf�[�^�̃`�����l����
	BOOL m_bEnable;										//�L���v�`�����L�����ǂ���
	//////////////////////////////////////////////////
	//�����o�ϐ�(�����܂�)
};
