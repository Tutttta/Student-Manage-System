#pragma once
#include "Global.h"
/*
	��ģ����Ҫ����UI��ʾ
*/
class UI
{
public:
	UI() {} 
	void PrintMenu(const eMenuType &eType);
private:
	// �����ܲ˵�
	void MainMenu();
	// ���빦���Ӳ˵�
	void InsertSubMenu();
	// ɾ�������Ӳ˵�
	void DeleteSubMenu();
	// ��ѯ�����Ӳ˵�
	void QuerySubMenu();
	// ͳ�ƹ����Ӳ˵�
	void StatisticSubMenu();
};