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
	puts("ѧ������ϵͳ");
	puts("1. ����");
	puts("2. ɾ��");
	puts("3. ��ѯ");
	puts("4. ͳ��");
	puts("5. �˳�");
	printf("����: ");
}

void UI::InsertSubMenu()
{
	puts("���빦���Ӳ˵�: ");
	// ID������
	puts("1. ����ѧ��");
	puts("2. �����γ�");
	puts("3. ������¼");
	puts("4. ������һ��");
	printf("����: ");
}

void UI::DeleteSubMenu()
{
	puts("ɾ�������Ӳ˵�: ");
	// ID
	puts("1. ɾ��ѧ��");
	puts("2. ɾ���γ�");
	puts("3. ɾ��ѡ�μ�¼");
	puts("4. ������һ��");
	printf("����: ");
}

void UI::QuerySubMenu()
{
	puts("��ѯ�����Ӳ˵�: ");
	// ID
	puts("1. ͨ��ѧ��ID��ѯѧ������");
	puts("2. ͨ��ѧ��������ѯѧ��ID");
	puts("3. ͨ���γ�ID��ѯ�γ���");
	puts("4. ͨ���γ�����ѯ�γ�ID");
	puts("5. ͨ���γ�ID��ѯѡ���˸ÿγ̵�ѧ���Լ��ɼ�");
	puts("6. ͨ��ѧ��ID��ѯ��ѧ��ѡ�޵Ŀγ���Ϣ");
	puts("7. ������һ��");
	printf("����: ");
}

void UI::StatisticSubMenu()
{
	puts("ͳ�ƹ����Ӳ˵�: ");
	puts("1. �γ���");
	puts("2. ѧ����");
	puts("3. ������һ��");
	printf("����: ");
}

