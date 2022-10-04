#pragma once
#include "Global.h"
#include "UI.h"
#include "IO.h"

class Control
{
public:
	Control() = default;
	// ���������к���, Ҳ��Ψһ��¶�Ľӿ�
	void Run();
private:
	// ���ڳ�ʼ������ϵͳ
	BOOLEAN InitSystem();
	// ���������û�����
	BOOLEAN GenerateUserData();
	// ��ʼ��ѧ����ϢAVL��
	BOOLEAN InitStudentInfo(PPSTR ppstrStudentNameAry, size_t nSize);
	// ��ʼ���γ���ϢAVL��
	BOOLEAN InitCourseInfo(PPSTR ppstrCourseNameAry, size_t nSize);
	// ��ʼ�������ļ�·�����ж����������ļ��Ƿ����
	BOOLEAN ChkInitFileExists(PSTR pszFilePathName, size_t nSize, PCSTR pcszFilePathName = STUDENTFILENAME);
	// �������stSearchIDByStudentName�ṹ��, ���������g_stSrhStudentNameTree��
	BOOLEAN SetSearchIDByStudentNameStructure(PSTR pStrName, size_t nID);
	// �������stSearchIDByCourseName�ṹ��, ���������g_stSrhCourseNameTree��
	BOOLEAN SetSearchIDByCourseNameStructure(PSTR pStrName, size_t nID);

	// �������̨��Ļ���������, Ĭ�����������, Ȼ����ͣ����
	void ClearScreenAndOutput(PCSTR pszText, BOOLEAN fNextLine = TRUE);

	// ����, ɾ��, ��ѯ��ͳ�ƹ��ܵ�������
	void Insert();
	void Delete();
	void Query();
	void Statistic();

	// �����ӹ���
	// ����ѧ��(ID, ����)
	void AddStudent();
	// ����ѡ��(ID, ����)
	void AddCourse();
	// ������¼(ѧ��ID, �γ�ID, ����)
	void AddRecord();

	// ɾ���ӹ���
	// ɾ��ѧ��(ID, ����)
	void DelStudent();
	// ɾ���γ�(ID, ����)
	void DelCourse();
	// ɾ����¼(ѧ��ID, �γ�ID)
	void DelRecord();

	// ��ѯ�ӹ���
	// ͨ��ѧ��ID��ѯѧ������
	void QueryStuNameByID();
	// ͨ��ѧ��������ѯѧ��ID
	void QueryIDByStuName();
	// ͨ���γ�ID��ѯ�γ���
	void QueryCoNameByID();
	// ͨ���γ�����ѯ�γ�ID
	void QueryIDByCoName();
	// ͨ���γ�ID��ѯѡ���˸ÿγ̵�ѧ���Լ��ɼ�
	void QueryAllStuAndPointsByID();
	// ͨ��ѧ��ID��ѯ��ѧ��ѡ�޵Ŀγ���Ϣ
	void QueryCoInfoByID();

	// ͳ���ӹ���
	// ��ӡ���пγ���Ϣ
	void PrintAllCourseInfo();
	// ��ӡ����ѧ����Ϣ
	void PrintAllStudentInfo();
private:
	// UI��������ʹ�õ������
	UI m_UI;
	// IO����ʹ�õ������
	IO m_IO;
};

