#pragma once
#include "Global.h"

class IO
{
public:
	// IO类型
	typedef enum _eIOType
	{
		READONLY = 1,
		CREATE
	} eIOType;
public:
	IO() = default;
	IO(const char *pcszFilePathName, eIOType eMode = READONLY);
	IO(const CMyString &strFileName, eIOType eMode = READONLY);
	// 设置IO模式
	inline void SetMode(eIOType eIOMode)
	{
		m_eMode = eIOMode;
	}
	// 获取文件大小
	ULONGLONG IOGetFileSize();
	// 从文件中按行读取内容
	char **GetLineFromFile(size_t &rnSizeOfStringAry);
	// 更新文件名
	void UpdateFileName(const char *pcszFilePathName)
	{
		if (!pcszFilePathName)
		{
			return;
		}
		m_strFileName = pcszFilePathName;
	}
	// 更新文件名
	void UpdateFileName(CMyString &strFileName)
	{
		UpdateFileName(strFileName.GetString());
	}
	// 判定文件是否存在
	BOOLEAN FileExists(const char *pcszFileName);
private:
	// 释放指针数组的空间
	void Release2DiAry(PPSTR ppStrAry, size_t nSize);
private:
	CMyString m_strFileName;
	eIOType m_eMode;
};

