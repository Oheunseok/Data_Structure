#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<windows.h>
#define MAX 15

int parking[3][4][10] = { 0 };

typedef struct Car_info {
	char Car_Number[MAX];
	int hour;
	int min;
	int floor;
	int row;
	int col;
	int disable;
	int month;
	int date;
} Car_info;

typedef struct ListNode {
	Car_info data;
	struct ListNode *link;
} ListNode;

ListNode *CarList = NULL;
ListNode *TicketList = NULL;

void textcolor(int color_number)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color_number);
}

void insert_node(ListNode **phead, ListNode *p, ListNode *new_node)
{
	if (*phead == NULL) {
		new_node->link = NULL;
		*phead = new_node;
	}
	else if (p == NULL) {
		new_node->link = *phead;
		*phead = new_node;
	}
	else {
		new_node->link = p->link;
		p->link = new_node;
	}
}

void remove_node(ListNode **phead, ListNode *p, ListNode *removed)
{
	if (p == NULL)
		*phead = (*phead)->link;
	else
		p->link = removed->link;
	free(removed);
}

ListNode *search(ListNode *head, char Car_Number[])
{
	ListNode *p;
	p = head;
	while (p != NULL) {
		if (strcmp(p->data.Car_Number, Car_Number) == 0) return p;
		p = p->link;
	}
	return p;
}

ListNode *Psearch(ListNode *head, char Car_Number[])
{
	ListNode *p;
	p = head;
	while (p != NULL) {
		if (strcmp(p->data.Car_Number, Car_Number) == 0)
		{
			p = NULL;
			return p;
		}
		if (strcmp(p->link->data.Car_Number, Car_Number) == 0) return p;
		p = p->link;
	}
}

void print_parking(int lots[3][4][10])
{
	int a, b, c;
	int count1 = 0;
	int count2 = 0;
	int count3 = 0;
	for (a = 0; a<3; a++)
	{
		if (a == 0)
		{
			printf("1F");
		}
		else if (a == 1)
		{
			printf("2F");
		}
		else
		{
			printf("3F");
		}
		for (b = 0; b<4; b++)
		{
			printf("\n");
			for (c = 0; c<10; c++)
			{
				if ((lots[a][b][c] == 0 && c == 0) || (lots[a][b][c] == 0 && c == 1))
				{
					textcolor(13);
					printf("��");
				}
				else if ((lots[a][b][c] == 0 && b == 0 && c != 0 && c != 1))
				{
					textcolor(14);
					printf("��");
				}
				else if (lots[a][b][c] == 1)
				{
					textcolor(12);
					printf("��");
				}
				else
				{
					textcolor(15);
					printf("��");
				}
			}
			if (b == 3)
			{
				printf("\n\n");
			}
		}
	}
	for (a = 0; a<3; a++)
	{
		for (b = 0; b<4; b++)
		{
			for (c = 0; c<10; c++)
			{
				if (a == 0 && lots[a][b][c] == 0)
					count1++;
				else if (a == 1 && lots[a][b][c] == 0)
					count2++;
				else if (a == 2 && lots[a][b][c] == 0)
					count3++;
			}
		}
	}
	printf("�����ڸ���\n");
	printf("1F:%d / 2F:%d / 3F:%d\n\n", count1, count2, count3);
}

void allprint(ListNode *head)
{
	ListNode *p = head;

	if (p == NULL)
	{
		printf("������ ������ �����ϴ�.\n");
	}
	else
		while (p != NULL) {
			printf("-------------------\n");
			printf("���Խð�:  %02d:%02d\n", p->data.hour, p->data.min);
			printf("������ȣ: %s\n", p->data.Car_Number);
			printf("-------------------\n\n");
			p = p->link;
		}
}

void Ticketprint(ListNode *head)
{
	ListNode *p = head;

	if (p == NULL)
	{
		printf("����� ������ �����ϴ�.\n");
	}
	else
		while (p != NULL) {
			printf("-------------------\n");
			printf("������ȣ: %s\n", p->data.Car_Number);
			printf("��ϳ�¥:  %d�� %d��\n", p->data.month, p->data.date);
			printf("-------------------\n\n");
			p = p->link;
		}
}

char* timeToString(struct tm *t, int*hour, int*min)
{
	static char s[20];
	*hour = t->tm_hour;
	*min = t->tm_min;
	return s;
}

int fee(ListNode *head, char Car_Number[])
{
	ListNode *q;
	int Nhour, Nmin, hour, min;
	int basic = 3000;
	struct tm *t;
	time_t timer;
	timer = time(NULL);
	t = localtime(&timer);
	timeToString(t, &Nhour, &Nmin);
	q = search(head, Car_Number);
	if (q != NULL)
	{
		hour = q->data.hour;
		min = q->data.min;

		if ((Nhour * 60 + Nmin) - (hour * 60 + min) <= 30)
		{
			if (q->data.disable == 1)
				return basic / 2;
			else
				return basic;
		}
		else if ((Nhour * 60 + Nmin) - (hour * 60 + min)>30)
		{
			if (q->data.disable == 1)
				return (basic + ((Nhour * 60 + Nmin) - (hour * 60 + min)) * 50);
			else
				return (basic + ((Nhour * 60 + Nmin) - (hour * 60 + min)) * 100);
		}
		else if ((Nhour * 60 + Nmin) - (hour * 60 + min)>180)
			if (q->data.disable == 1)
				return 10000;
			else
				return 15000;
		else if ((Nhour * 60 + Nmin) - (hour * 60 + min)>300)
			if (q->data.disable == 1)
				return 15000;
			else
				return 20000;
	}
}


Car_info Car_In()
{
	char Car_Number[15];
	int floor;
	int row;
	int col;
	int disable;
	int hour;
	int min;
	ListNode *q;
	Car_info x;

	struct tm *t;
	time_t timer;
	timer = time(NULL);
	t = localtime(&timer);
	timeToString(t, &hour, &min);

	printf("������ȣ�� �Է��Ͻÿ�:");
	scanf("%s", Car_Number);
	printf("����� �����Դϱ�? (Y:1/N:0) :");
	scanf("%d", &disable);
	print_parking(parking);
	printf("���������� �Է��Ͻÿ�(��/��ȣ) :");
	scanf("%d %d %d", &floor, &row, &col);

	while ((disable == 0 && col == 1) || (disable == 0 && col == 2)) {
		system("cls");
		printf("��������������� ������ �� �����ϴ�.\n\n");
		print_parking(parking);
		printf("���������� �Է��Ͻÿ�(��/��ȣ) :");
		scanf("%d %d %d", &floor, &row, &col);
	}

	strcpy(x.Car_Number, Car_Number);
	x.floor = floor;
	x.row = row;
	x.col = col;
	x.disable = disable;
	x.hour = hour;
	x.min = min;

	parking[x.floor - 1][x.row - 1][x.col - 1] = 1;

	system("cls");

	q = search(TicketList, Car_Number);
	if (q != NULL)
	{
		printf("-------------------\n");
		printf("���Խð�:  %02d:%02d\n", hour, min);
		printf("������ȣ: %s\n", Car_Number);
		printf("����� �����Դϴ�.\n");
		printf("-------------------\n");
	}
	else
	{
		printf("-------------------\n");
		printf("���Խð�:  %02d:%02d\n", hour, min);
		printf("������ȣ: %s\n", Car_Number);
		printf("-------------------\n");
	}
	return x;
}

int main()
{
	int menu;
	int month, date;
	char Car_Number[MAX];

	ListNode *new_node, *p, *q;
	Car_info a;

	while (1) {
		system("cls");
		printf("*******************************************************************************\n");
		printf("*                                                                             *\n");
		printf("*                                   <�޴�>                                    *\n");
		printf("*                                                                             *\n");
		printf("*                                                                             *\n");
		printf("*                            1. ��  ��  ��  ��                                *\n");
		printf("*                            2. ��  ��  ��  ��                                *\n");
		printf("*                            3. ��  ��  ��  ȸ                                *\n");
		printf("*                            4. ��  ü  ��  ��                                *\n");
		printf("*                            5. �� �� �� �� ��                                *\n");
		printf("*                            6. �� �� �� �� ��                                *\n");
		printf("*                            7. �� �� �� �� ��                                *\n");
		printf("*                            8. ��          ��                                *\n");
		printf("*                                                                             *\n");
		printf("*                                                                             *\n");
		printf("*******************************************************************************\n");

		printf("��ȣ�� �Է��Ͻÿ�:");
		scanf("%d", &menu);

		switch (menu)
		{
		case 1:
			system("cls");
			a = Car_In();
			print_parking(parking);
			new_node = (ListNode*)malloc(sizeof(ListNode));
			new_node->data = a;
			p = NULL;
			insert_node(&CarList, p, new_node);

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
			{
				goto finish;
			}

		case 2:
			system("cls");
			printf("������ȣ�� �Է��Ͻÿ�:");
			scanf("%s", Car_Number);

			q = search(CarList, Car_Number);
			p = search(TicketList, Car_Number);
			if (q != NULL)
			{
				if (p != NULL)
				{
					printf("-------------------\n");
					printf("������ȣ: %s\n", q->data.Car_Number);
					printf("���Խð�:  %02d:%02d\n", q->data.hour, q->data.min);
					printf("�������: 0��\n");
					printf("����� �����Դϴ�.\n");
					printf("%d�� %d�ϱ����Դϴ�.\n", ((p->data.month) + 1) % 12, p->data.date);
					printf("-------------------\n");
				}
				else
				{
					printf("-------------------\n");
					printf("������ȣ: %s\n", q->data.Car_Number);
					printf("���Խð�:  %02d:%02d\n", q->data.hour, q->data.min);
					printf("�������: %d��\n", fee(CarList, Car_Number));
					printf("-------------------\n");
				}

				parking[q->data.floor - 1][q->data.row - 1][q->data.col - 1] = 0;
				p = Psearch(CarList, Car_Number);
				remove_node(&CarList, p, q);
			}
			else
				printf("������ �����ϴ�.\n");

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
				goto finish;

		case 3:
			system("cls");
			printf("������ȣ�� �Է��Ͻÿ�:");
			scanf("%s", Car_Number);
			q = search(CarList, Car_Number);
			if (q != NULL)
			{
				printf("-------------------\n");
				printf("������ȣ: %s\n", q->data.Car_Number);
				printf("���Խð�:  %02d:%02d\n", q->data.hour, q->data.min);
				printf("������: %d��\n", fee(CarList, Car_Number));
				printf("-------------------\n");
			}
			else
				printf("������ �����ϴ�.\n");

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
				goto finish;

		case 4:
			system("cls");
			allprint(CarList);

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
				goto finish;

		case 5:
			system("cls");
			print_parking(parking);

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
				goto finish;

		case 6:
			system("cls");
			new_node = (ListNode*)malloc(sizeof(ListNode));
			printf("������ȣ�� �Է��Ͻÿ�:");
			scanf("%s", Car_Number);
			printf("��¥�� �Է��Ͻÿ�:");
			scanf("%d %d", &month, &date);
			strcpy(new_node->data.Car_Number, Car_Number);
			new_node->data.month = month;
			new_node->data.date = date;
			p = NULL;
			insert_node(&TicketList, p, new_node);

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
				goto finish;

		case 7:
			system("cls");
			Ticketprint(TicketList);

			printf("������ ������ȣ�� �Է��Ͻÿ�:");
			scanf("%s", Car_Number);

			q = search(TicketList, Car_Number);
			if (q != NULL)
			{
				p = Psearch(TicketList, Car_Number);
				remove_node(&TicketList, p, q);
				printf("���� ������ �����Ͽ����ϴ�.\n");
			}
			else
				printf("������ �����ϴ�.\n");

			printf("�޴��� ���ư��ðڽ��ϱ�? (Y:1/N:0):");
			scanf("%d", &menu);
			if (menu == 1)
			{
				break;
			}
			else
				goto finish;

		case 8:
			goto finish;
		}
	}
finish:
	system("cls");
	printf("*******************************************************************************\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                      ��      ��      ��      ��      ��                     *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*                                                                             *\n");
	printf("*******************************************************************************\n");
}