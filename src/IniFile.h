#pragma once

//////////////////////////////////////////////////
//INI�t�@�C����ǂݏ�������N���X
//////////////////////////////////////////////////
class CIniFile
{
public:
	//�R���X�g���N�^
	CIniFile(void);
	//�f�X�g���N�^
	virtual ~CIniFile(void);

	//INI�t�@�C�����J��
	BOOL Open(LPCTSTR strProfileName,CWinApp* pWinApp = NULL);
	//INI�t�@�C�������
	void Close();
	//�t�@�C���̑��݂̊m�F
	BOOL IsFind() const {return m_bFind;}

	//INI�t�@�C���̓ǂݍ��݂Ə�������
	BOOL WriteProfileInt(LPCTSTR lpszSection,LPCTSTR lpszEntry,int nValue);
	BOOL WriteProfileBOOL(LPCTSTR lpszSection,LPCTSTR lpszEntry,BOOL bValue);
	BOOL WriteProfilebool(LPCTSTR lpszSection,LPCTSTR lpszEntry,bool bValue);
	BOOL WriteProfileFloat(LPCTSTR lpszSection,LPCTSTR lpszEntry,float fValue);
	BOOL WriteProfileDouble(LPCTSTR lpszSection,LPCTSTR lpszEntry,float dValue);
	BOOL WriteProfileString(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPCTSTR lpszValue);
	BOOL WriteProfileGUID(LPCTSTR lpszSection,LPCTSTR lpszEntry,GUID guidValue);

	int GetProfileInt(LPCTSTR lpszSection,LPCTSTR lpszEntry,int nDefault);
	BOOL GetProfileBOOL(LPCTSTR lpszSection,LPCTSTR lpszEntry,BOOL bDefault);
	bool GetProfilebool(LPCTSTR lpszSection,LPCTSTR lpszEntry,bool bDefault);
	CString GetProfileString(LPCTSTR lpszSection,LPCTSTR lpszEntry,LPCTSTR lpszDefault = NULL);
	double GetProfileDouble(LPCTSTR lpszSection,LPCTSTR lpszEntry,double dDefault);
	float GetProfileFloat(LPCTSTR lpszSection,LPCTSTR lpszEntry,float fDefault);
	BOOL GetProfileGUID(LPCTSTR lpszSection,LPCTSTR lpszEntry,GUID guidDefault,GUID &guidValue);

private:
	CWinApp* m_pWinApp;	//WinApp�N���X�̃|�C���^
	BOOL m_bFind;		//�t�@�C���̑��݂̗L��
	CString m_strFullPath;		//INI�̐�΃p�X
	CString m_strRelativePath;	//INI�̑��΃p�X

	//���݂̃p�X��ݒ�
	BOOL SetCurrentProfilePath(LPCTSTR lpszFileName);
};
