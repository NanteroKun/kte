
// stdafx.h : �W���̃V�X�e�� �C���N���[�h �t�@�C���̃C���N���[�h �t�@�C���A�܂���
// �Q�Ɖ񐔂������A�����܂�ύX����Ȃ��A�v���W�F�N�g��p�̃C���N���[�h �t�@�C��
// ���L�q���܂��B

#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Windows �w�b�_�[����g�p����Ă��Ȃ����������O���܂��B
#endif

//OS�̃o�[�W�����ɂ���ăw�b�_�̐ݒ��ς����
#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �ꕔ�� CString �R���X�g���N�^�[�͖����I�ł��B

// ��ʓI�Ŗ������Ă����S�� MFC �̌x�����b�Z�[�W�̈ꕔ�̔�\�����������܂��B
#define _AFX_ALL_WARNINGS

#include <locale.h>


#include <afxwin.h>         // MFC �̃R�A����ѕW���R���|�[�l���g
#include <afxext.h>         // MFC �̊g������

#include <afxinet.h>

#include <afxmt.h>			// MFC (�N���e�B�J���Z�N�V����)

#include <afxdisp.h>        // MFC �I�[�g���[�V���� �N���X



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �R���� �R���g���[�� �T�|�[�g
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �R���� �R���g���[�� �T�|�[�g
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>            // MFC �̃\�P�b�g�g���@�\


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#define _WIN32_DCOM 
#pragma warning(disable : 4995)

//#include <stdio.h>
//#include <tchar.h>

//#include <windows.h>		//�E�B���h�E�Y�̕W���w�b�_
//#include <atlbase.h>		//ATL�̃w�b�_

//GetVersionEx�̑��
#include <VersionHelpers.h>

//�W��
#include <assert.h>
#include <tchar.h>

//STL
#include <vector>
#include <list>
#include <sstream>
//#include <algorithm>
//#include <functional>
using namespace std;

//DirectShow�w�b�_
#include <dshow.h>
#include <Dvdmedia.h>

//Direct3D�w�b�_
#include <D3d9.h>
#include <Vmr9.h>

// for DMO, WMV, WMA
#include <dmodshow.h>
#include <dmoreg.h>
#include <wmcodecdsp.h>
#pragma comment(lib, "Dmoguids.lib")
#pragma comment(lib, "wmcodecdspuuid.lib")


#pragma comment(lib,"Msdmo.lib")


//Windows Media Format SDK 11
#include <wmsdk.h>		//Windows Media Format �{��
#include <dshowasf.h>	//WMF��DirectShow�̘A�g
#include <wmsysprf.h>	//�G���R�[�h�ݒ�Ɋւ���f�t�H���g��GUID

//���C�u�����t�@�C��
#pragma comment(lib,"strmiids.lib")	// �N���X���ʎq (CLSID) �ƃC���^�[�t�F�C�X���ʎq (IID) �G�N�X�|�[�g����B���ׂĂ� DirecShow �A�v���P�[�V�����ɂ͂��̃��C�u�������K�v�B
#pragma comment(lib,"quartz.lib")	// AMGetErrorText �֐����G�N�X�|�[�g����B���̊֐����Ăяo���Ȃ��ꍇ�A���̃��C�u�����͕K�v�Ȃ��B
#pragma comment(lib,"Wmvcore.lib")

#pragma comment(lib,"Wmvcore.lib")

#pragma comment(lib, "winmm.lib")		//�E�B���h�E�Y�}���`���f�B�A���C�u����
//#pragma comment(lib, "comctl32.lib")
//#pragma comment(lib, "avrt.lib")


//DirectShow���N���X���C�u����
#include <wxdebug.h>	//DirectShow�f�o�b�O���C�u����
//#include <streams.h>
#ifdef _DEBUG
#pragma comment(lib, "comsuppwd.lib")
#pragma comment(lib, "strmbasd.lib")
//#pragma comment(lib, "msvcrtd.lib")
#else
#pragma comment(lib, "comsuppw.lib")
#pragma comment(lib, "strmbase.lib")
//#pragma comment(lib, "msvcrt.lib")
#endif

//GUID�̃w���p�N���X
#include <rpc.h>
#pragma comment(lib ,"rpcrt4.lib")


//�p�t�H�[�}���X�f�[�^�w���p�[(PDH)
#include <pdh.h>
#pragma comment (lib, "pdh.lib")

//USB�̐ڑ������o
#include <Dbt.h>

#ifdef _DEBUG
#ifdef DEBUG
#define DEBUG
#endif
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(x) \
   if(x != NULL)        \
   {                    \
      x->Release();     \
      x = NULL;         \
   }
#endif

#ifndef SAFE_ARRAY_DELETE
#define SAFE_ARRAY_DELETE(x) \
   if(x != NULL)             \
   {                         \
      delete[] x;            \
      x = NULL;              \
   }
#endif


//#include <qedit.h>	//DirectShow SampleGrabber�̃w�b�_ (�vDirectX SDK)


#include <Objbase.h>