#pragma once


// CVolumeWnd
#define VOLUME_CHANNELS         2	/*�{�����[���`�����l����*/
#define VOLUME_LEFT_MARGIN     15	/*���}�[�W��(�Œ�l)*/
#define VOLUME_RIGHT_MARGIN    10	/*�E�}�[�W��(�Œ�l)*/
#define VOLUME_MIN_DB       -90.0	/*�{�����[���̍ŏ��l*/
#define VOLUME_FONT_SIZE       10	/*�{�����[���̃t�H���g�T�C�Y*/
#define VOLUME_LINE_COUNT      10	/*���̐�*/

class CVolumeWnd : public CWnd
{
	DECLARE_DYNAMIC(CVolumeWnd)

public:
	//�R���X�g���N�^
	CVolumeWnd();
	//�f�X�g���N�^
	virtual ~CVolumeWnd();


protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

public:

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	//�f�V�x���f�[�^��������Ƃ��̃��b�Z�[�W�n���h��
	LRESULT OnSenddBData(WPARAM wParam,LPARAM lParam);

	BOOL CreateBackSurface(int nWidth,int nHeight);
	BOOL DeleteBackSurface();


private:
	//////////////////////////////////////////////////
	//�����o�ϐ�(��������)
	//CToolTipCtrl m_ToolTip;		//�_�C�A���O�ɕ\������c�[���`�b�v

	//�����
	CDC*     m_phBackDC;			//�f�o�C�X�R���e�L�X�g
	CBitmap* m_phBackBitmap;		//�r�b�g�}�b�v

	double m_ddB[VOLUME_CHANNELS];	//�f�V�x���f�[�^(L,R)

	//�e��o�[�̈�̃T�C�Y
	CRect m_rcChannelBar[VOLUME_CHANNELS];	//�`�����l���̃o�[�̗̈�(�S��)
	CRect m_rcChannelColorBar[VOLUME_CHANNELS][3];	//�`�����l���̃o�[�̗̈�(�F����)

	//TRIVERTEX m_DarkTriVertex[VOLUME_CHANNELS][3][2];	//���ʃ��[�^�[�̃o�[�̃O���f�[�V�������(�ÐF)
	//TRIVERTEX m_LightTriVertex[VOLUME_CHANNELS][3][2];//���ʃ��[�^�[�̃o�[�̃O���f�[�V�������(���F)
	//GRADIENT_RECT m_GradientRect;		//���ʃ��[�^�[�̃O���f�[�V����RECT

	//�ڐ������
	double m_ddBLine[VOLUME_LINE_COUNT];	//�ڐ�����̃f�[�^(�f�V�x��)
	int    m_ndBLine[VOLUME_LINE_COUNT];	//�ڐ���K��X���W(�s�N�Z��)

	//�Œ�F
	COLORREF m_rgbBlack;		//�w�i�F
	COLORREF m_rgbWhite;		//�����F
	COLORREF m_rgbLightBar[4];	//�o�[�̐F(���F)
	COLORREF m_rgbDarkBar[4];	//�o�[�̐F(�ÐF)
	CFont m_VolumeTextFont;		//���ʃ��[�^�[�̃t�H���g
	//////////////////////////////////////////////////
	//�����o�ϐ�(�����܂�)

private:
	//�`��ׂ̈֗̕��T�u���[�`��

	//�`�����l���o�[�̑傫��(CRect)�𓾂�
	inline CRect GetBar(CRect &rcWindow,BOOL bLChannel) {
		CRect rcBar(0,0,0,0);
		if (rcWindow.Width() <= VOLUME_LEFT_MARGIN + VOLUME_RIGHT_MARGIN) {
			return rcBar;
		}
		rcBar.left = VOLUME_LEFT_MARGIN;
		rcBar.right = rcWindow.Width() - VOLUME_RIGHT_MARGIN;
		if (bLChannel) {
			rcBar.top = (int)(rcWindow.Height() * 0.125);
		} else {
			rcBar.top = (int)(rcWindow.Height() * 0.625);
		}
		rcBar.bottom = rcBar.top + (int)(rcWindow.Height() * 0.25);
		return rcBar;
	}
	//�o�[�̑傫������w��̃f�V�x����X���W�𓾂�
	inline int GetdBPos(CRect &rcBar,double dB) {
		if (dB <= VOLUME_MIN_DB)
			return rcBar.left;
		else if (dB >= 0.0)
			return rcBar.right - 1;
		double dPer = rcBar.Width() / (VOLUME_MIN_DB);	//����\
		return (int) (rcBar.right - (dB * dPer));
	}

	//�`��2
	void CVolumeWnd::OnPaint2(CDC &dc);

public:
	//afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//afx_msg BOOL OnToolTip(UINT ID_notused,NMHDR *pNMHDR,LRESULT *Result_notused);
	//afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR,LRESULT *pResult);
	//afx_msg BOOL memberFxn( UINT id, NMHDR * pTTTStruct, LRESULT * pResult );
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPopupEnableVolume();
};


