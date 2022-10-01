#pragma once
#include "Global.h"
/*
	该模块主要用于UI显示
*/
class UI
{
public:
	UI() {} 
	void PrintMenu(const eMenuType &eType);
private:
	// 主功能菜单
	void MainMenu();
	// 插入功能子菜单
	void InsertSubMenu();
	// 删除功能子菜单
	void DeleteSubMenu();
	// 查询功能子菜单
	void QuerySubMenu();
	// 统计功能子菜单
	void StatisticSubMenu();
};
