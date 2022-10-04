#pragma once
#include "Global.h"
/*
	该模块主要用于UI显示
*/
class UI
{
public:
	UI() {} 
	// 开放的接口, 其通过一个枚举值确定打印的菜单
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
