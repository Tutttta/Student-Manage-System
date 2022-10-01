#include "Control.h"

void Control::Run()
{

}

BOOLEAN Control::InitSystem()
{
	char szStudentNameFilePath[LINEMAX] = { 0 };
	char szCourseNameFilePath[LINEMAX] = { 0 };
	PPSTR ppStudentsNameAry = nullptr;
	PPSTR ppCoursesNameAry = nullptr;
	size_t nLineOfStudentNames = 0;
	size_t nLineOfCourseNames = 0;
	BOOLEAN bInitSuccess = FALSE;

	do
	{
		// �ж���ȡ�ļ��Ƿ����, ������ֱ�ӷ���
		if (!ChkInitFileExists(szStudentNameFilePath, LINEMAX, STUDENTFILENAME) ||
			!ChkInitFileExists(szCourseNameFilePath, LINEMAX, COURSEFILENAME))
		{
			break;
		}
		// ��ȡ����ѧ������
		m_IO.UpdateFileName(szStudentNameFilePath);
		ppStudentsNameAry = m_IO.GetLineFromFile(nLineOfStudentNames);
		for (size_t nIdx = 0; nIdx < nLineOfStudentNames; ++nIdx)
		{
			cout << ppStudentsNameAry[nIdx] << endl;
		}
		bInitSuccess = TRUE;
		//// ��ȡ���пγ���
		//m_IO.UpdateFileName(szStudentNameFilePath);
	} while (FALSE);
	
	return(bInitSuccess);
}


BOOLEAN Control::ChkInitFileExists(PSTR pszFilePathName, size_t nSize, PCSTR pcszFileName/* = STUDENTFILENAME*/)
{
	char szFilePathName[LINEMAX] = "..\\data\\";
	int iLen = 0;

	if (!pcszFileName)
	{
		return(FALSE);
	}
	iLen = strlen(pcszFileName);
	strncat_s(szFilePathName, LINEMAX, pcszFileName, iLen);

	// �ж�students.txt�Ƿ����
	if (!m_IO.FileExists(szFilePathName))
	{
		// ���students.txt������, �����ڵ�ǰ·����Ѱ��
		strncpy_s(szFilePathName, LINEMAX, ".\\", sizeof(".\\"));
		strncat_s(szFilePathName, LINEMAX, pcszFileName, iLen);
		if (!m_IO.FileExists(szFilePathName))
		{
			// ���ɲ�����
			printf("�޷��ҵ�%s, �뽫������뱾����ͬһĿ¼�²��ٴγ���", pcszFileName);
			return(FALSE);
		}
	}
	if (pszFilePathName)
	{
		strncpy_s(pszFilePathName, nSize, szFilePathName, strlen(szFilePathName));
	}

	return(TRUE);
}
