#include "UI.h"
#include <cstdio>

void UI::PrintMenu(const eMenuType &eType)
{
	switch (eType)
	{
		case MAIN:
		{
			MainMenu();
			break;
		}
		case INSERT:
		{
			InsertSubMenu();
			break;
		}
		case DELETE:
		{
			DeleteSubMenu();
			break;
		}
		case QUERY:
		{
			QuerySubMenu();
			break;
		}
	}

	return;
}

void UI::MainMenu()
{
	puts("学生管理系统");
	puts("1. 插入");
	puts("2. 删除");
	puts("3. 查询");
	puts("4. 统计");
	puts("5. 退出");
	printf("命令: ");
}

void UI::InsertSubMenu()
{
	puts("插入功能子菜单: ");
	// ID和名字
	puts("1. 新增学生");
	puts("2. 新增课程");
	puts("3. 新增记录");
	puts("4. 返回上一级");
	printf("命令: ");
}

void UI::DeleteSubMenu()
{
	puts("删除功能子菜单: ");
	// ID
	puts("1. 删除学生");
	puts("2. 删除课程");
	puts("3. 删除选课记录");
	puts("4. 返回上一级");
	printf("命令: ");
}

void UI::QuerySubMenu()
{
	puts("查询功能子菜单: ");
	// ID
	puts("1. 通过学生ID查询学生姓名");
	puts("2. 通过学生姓名查询学生ID");
	puts("3. 通过课程ID查询课程名");
	puts("4. 通过课程名查询课程ID");
	puts("5. 通过课程ID查询选修了该课程的学生以及成绩");
	puts("6. 通过学生ID查询该学生选修的课程信息");
	puts("7. 返回上一级");
	printf("命令: ");
}

void UI::StatisticSubMenu()
{
	puts("统计功能子菜单: ");
	puts("1. 课程名");
	puts("2. 学生名");
	puts("3. 返回上一级");
	printf("命令: ");
}

