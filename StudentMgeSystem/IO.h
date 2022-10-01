#pragma once
#include "Global.h"

class IO
{
public:
	// IO����
	typedef enum _eIOType
	{
		READONLY = 1,
		CREATE
	} eIOType;
public:
	IO() = default;
	IO(const char *pcszFilePathName, eIOType eMode = READONLY);
	IO(const CMyString &strFileName, eIOType eMode = READONLY);
	// ����IOģʽ
	inline void SetMode(eIOType eIOMode)
	{
		m_eMode = eIOMode;
	}
	// ��ȡ�ļ���С
	ULONGLONG IOGetFileSize();
	// ���ļ��а��ж�ȡ����
	char **GetLineFromFile(size_t &rnSizeOfStringAry);
	// �����ļ���
	void UpdateFileName(const char *pcszFilePathName)
	{
		if (!pcszFilePathName)
		{
			return;
		}
		m_strFileName = pcszFilePathName;
	}
	// �����ļ���
	void UpdateFileName(CMyString &strFileName)
	{
		UpdateFileName(strFileName.GetString());
	}
	// �ж��ļ��Ƿ����
	BOOLEAN FileExists(const char *pcszFileName);
private:
	// �ͷ�ָ������Ŀռ�
	void Release2DiAry(PPSTR ppStrAry, size_t nSize);
private:
	CMyString m_strFileName;
	eIOType m_eMode;
};

