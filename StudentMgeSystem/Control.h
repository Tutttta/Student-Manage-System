#pragma once
#include "Global.h"
#include "UI.h"
#include "IO.h"

class Control
{
public:
	Control() = default;

	void Run();
	// 用于初始化管理系统
	BOOLEAN InitSystem();
	// 初始化学生信息AVL树
	BOOLEAN InitStudentInfo(PPSTR ppstrStudentNameAry, size_t nSize);
	// 初始化课程信息AVL树
	BOOLEAN InitCourseInfo(PPSTR ppstrCourseNameAry, size_t nSize);
	BOOLEAN InitSystemData();


	BOOLEAN ChkInitFileExists(PSTR pszFilePathName, size_t nSize, PCSTR pcszFilePathName = STUDENTFILENAME);
	// 计算字符串的HASH值
	size_t NameHashCalc(PCSTR pcszName, size_t nSize);

	BOOLEAN SetSearchIDByStudentNameStructure(PSTR pStrName, size_t nID);
	BOOLEAN SetSearchIDByCourseNameStructure(PSTR pStrName, size_t nID);

	// 清除控制台屏幕后输出内容, 默认输出后跳行, 然后暂停运行
	void ClearScreenAndOutput(PCSTR pszText, BOOLEAN fNextLine = TRUE);
	void Insert();
	void Delete();
	void Query();
	void Statistic();

	// 插入子功能
	// 新增学生(ID, 名字)
	void AddStudent();
	// 新增选课(ID, 名字)
	void AddCourse();
	// 新增记录(学生ID, 课程ID, 分数)
	void AddRecord();


	// 删除子功能
	// 删除学生(ID, 名字)
	void DelStudent();
	// 删除课程(ID, 名字)
	void DelCourse();
	// 删除记录(学生ID, 课程ID)
	void DelRecord();
	

	// 查询子功能
	// 通过学生ID查询学生姓名
	void QueryStuNameByID();
	// 通过学生姓名查询学生ID
	void QueryIDByStuName();
	// 通过课程ID查询课程名
	void QueryCoNameByID();
	// 通过课程名查询课程ID
	void QueryIDByCoName();
	// 通过课程ID查询选修了该课程的学生以及成绩
	void QueryAllStuAndPointsByID();
	// 通过学生ID查询该学生选修的课程信息
	void QueryCoInfoByID();
private:
	UI m_UI;
	IO m_IO;
};

