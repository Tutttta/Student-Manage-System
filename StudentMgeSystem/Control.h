#pragma once
#include "Global.h"
#include "UI.h"
#include "IO.h"

class Control
{
public:
	Control() = default;

	void Run();
	// ���ڳ�ʼ������ϵͳ
	BOOLEAN InitSystem();
	// ��ʼ��ѧ����ϢAVL��
	BOOLEAN InitStudentInfo(PPSTR ppstrStudentNameAry, size_t nSize);
	// ��ʼ���γ���ϢAVL��
	BOOLEAN InitCourseInfo(PPSTR ppstrCourseNameAry, size_t nSize);
	BOOLEAN InitSystemData();


	BOOLEAN ChkInitFileExists(PSTR pszFilePathName, size_t nSize, PCSTR pcszFilePathName = STUDENTFILENAME);
	// �����ַ�����HASHֵ
	size_t NameHashCalc(PCSTR pcszName, size_t nSize);

	BOOLEAN SetSearchIDByStudentNameStructure(PSTR pStrName, size_t nID);
	BOOLEAN SetSearchIDByCourseNameStructure(PSTR pStrName, size_t nID);

	// �������̨��Ļ���������, Ĭ�����������, Ȼ����ͣ����
	void ClearScreenAndOutput(PCSTR pszText, BOOLEAN fNextLine = TRUE);
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
private:
	UI m_UI;
	IO m_IO;
};

