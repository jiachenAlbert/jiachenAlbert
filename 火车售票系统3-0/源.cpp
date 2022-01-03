/*
	��ʼ��2021��12��27��8��00��00��
	��ɣ�2021��12��30��11��47��30��
*/
#define _stprintf
#define _CRT_SECURE_NO_WARNINGS
#define MAX_USER 100
#define MAX_NODE 20

#include <stdio.h>
#include <string.h>
#include <graphics.h>

typedef struct login
{
	char name[16];
	char key[16];
}Login; //�˺�
typedef char VertexType;  //��������
typedef struct edge
{
	int money;
	int time;
}EdgeType;  //����Ȩֵ
typedef struct EdgeNode  //�ڵ�
{
	int adjvex; //�洢�����Ӧ�±�
	EdgeType weight; //���ڴ洢Ȩֵ
	struct EdgeNode* next;
}EdgeNode;
typedef struct VertexNode //�����
{
	VertexType data[16]; //������
	EdgeNode* firstedge; //�߱�ͷָ��
}VertexNode, AdjList[MAX_NODE];
typedef struct graph
{
	AdjList adjlist;
	int numVertexes;  //ͼ�е�ǰ������
	int numEdges;     //ͼ�е�ǰ����
}GraphAdjList;
typedef struct ticket
{
	int start; //��ʼ
	int end;  //����
	int num;  //����
	int site; //��λ
	char time[16]; //ʱ��
}Ticket;

void main_menu(GraphAdjList G); //��Ϊ���˵��������¼�����������˵��߼��߼�
void login_menu(GraphAdjList G); //��¼����
void logon_menu(GraphAdjList G); //ע�����
void CreatALGraph(GraphAdjList* G);  //��ȡ��������
void floyd_money(GraphAdjList G); //��������Ǯ
void floyd_time(GraphAdjList G); //��������ʱ��

int dis_money[MAX_NODE][MAX_NODE]; //�洢����Ǯ
int dis_time[MAX_NODE][MAX_NODE]; //�洢����ʱ��

int main()
{
	GraphAdjList G;
	memset(G.adjlist, 0x00, sizeof G.adjlist); //��������ʼ��Ϊ��
	CreatALGraph(&G); //��ȡ������Ϣ������ͼ
	floyd_money(G);   //��������Ǯ
	floyd_time(G);    //��������ʱ��

	login_menu(G); //��¼����


	return 0;
}
void floyd_time(GraphAdjList G)
{
	int i = 0, j = 0, k = 0;
	for (i = 0; i < G.numVertexes; i++)
	{
		for (j = 0; j < G.numEdges; j++)
		{
			dis_time[i][j] = 0x3f3f3f3f;
		}
		dis_time[i][i] = 0;
	}
	for (i = 0; i < G.numVertexes; i++)
	{
		EdgeNode* s = G.adjlist[i].firstedge; //���ܳ���
		while (s)
		{
			dis_time[i][s->adjvex] = s->weight.time;
			s = s->next;
		}
	}
	for (k = 0; k < G.numVertexes; k++)
	{
		for (i = 0; i < G.numVertexes; i++)
		{
			for (j = 0; j < G.numVertexes; j++)
			{
				if (dis_time[i][k] + dis_time[k][j] < dis_time[i][j])
				{
					dis_time[i][j] = dis_time[i][k] + dis_time[k][j];
				}
			}
		}
	}
	//��ӡ���·��
	/*for (i = 0; i < G.numVertexes; i++)
	{
		for (j = 0; j < G.numVertexes; j++)
		{
			printf("%-5d", dis_time[i][j]);
		}
		printf("\n");
	}*/
}
void floyd_money(GraphAdjList G)
{
	int i = 0, j = 0, k = 0;
	for (i = 0; i < G.numVertexes; i++)
	{
		for (j = 0; j < G.numEdges; j++)
		{
			dis_money[i][j] = 0x3f3f3f3f;
		}
		dis_money[i][i] = 0;
	}
	for (i = 0; i < G.numVertexes; i++)
	{
		EdgeNode* s = G.adjlist[i].firstedge; //���ܳ���
		while (s)
		{
			dis_money[i][s->adjvex] = s->weight.money;
			s = s->next;
		}
	}
	for (k = 0; k < G.numVertexes; k++)
	{
		for (i = 0; i < G.numVertexes; i++)
		{
			for (j = 0; j < G.numVertexes; j++)
			{
				if (dis_money[i][k] + dis_money[k][j] < dis_money[i][j])
				{
					dis_money[i][j] = dis_money[i][k] + dis_money[k][j];
				}
			}
		}
	}
	//��ӡ���·��
	/*for (i = 0; i < G.numVertexes; i++)
	{
		for (j = 0; j < G.numVertexes; j++)
		{
			printf("%-5d", dis_money[i][j]);
		}
		printf("\n");
	}*/
}
void CreatALGraph(GraphAdjList* G)
{
	int i = 0, j = 0, k = 0;
	EdgeNode* e;
	FILE* fp_map;
	fp_map = fopen("map.txt", "rt");

	char data[16] = { 0 };
	int  numVertexes = 0, numEdges = 0;
	while (!feof(fp_map))  
	{
		fscanf(fp_map, "%s", data);
		strcpy(G->adjlist[i].data, data);
		G->adjlist[i].firstedge = NULL;
		i++;
		numVertexes++; //������
	}
	G->numVertexes = numVertexes;
	int money = 0, time = 0;
	FILE* fp_side;
	fp_side = fopen("side.txt", "rt");
	while (!feof(fp_side))
	{
		fscanf(fp_side, "%d %d %d %d", &i, &j, &money, &time);

		e = (EdgeNode*)malloc(sizeof(EdgeNode));
		e->adjvex = j;
		e->weight.money = money;
		e->weight.time = time;
		e->next = G->adjlist[i].firstedge;
		G->adjlist[i].firstedge = e;

		e = (EdgeNode*)malloc(sizeof(EdgeNode));
		e->adjvex = i;
		e->weight.money = money;
		e->weight.time = time;
		e->next = G->adjlist[j].firstedge;
		G->adjlist[j].firstedge = e;

		numEdges++; //����
	}
	G->numEdges = numEdges;
}
void logon_menu(GraphAdjList G) //ע�����
{
	initgraph(600, 800);
	// ���ñ���ɫΪ��ɫ
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	cleardevice();
	// ���û�ͼɫ
	setcolor(RGB(151, 152, 146));
	setfillcolor(RGB(228, 239, 248));
	// ������
	fillrectangle(150, 200, 450, 650);
	// ����ַ��� (VC6 / VC2008 / VC2010 / VC2012)
	setbkmode(TRANSPARENT);
	char str_load[] = "ע���˻�";
	settextstyle(75, 0, _T("����"));
	outtextxy(150, 75, str_load);
	//����Բ
	setcolor(RGB(151, 152, 146));
	setfillcolor(WHITE);
	fillellipse(250, 225, 350, 325);
	char str_user[] = "�û�";
	settextstyle(40, 0, _T("����"));
	outtextxy(265, 255, str_user);
	//�˺�
	fillrectangle(250, 400, 400, 425);
	char str_user_name[] = "�˺ţ�";
	settextstyle(25, 0, _T("����"));
	outtextxy(170, 400, str_user_name);
	//����
	fillrectangle(250, 450, 400, 475);
	char str_user_key[] = "���룺";
	settextstyle(25, 0, _T("����"));
	outtextxy(170, 450, str_user_key);
	//���ע�����
	char str_user_logon[] = "ע��";
	settextstyle(50, 0, _T("����"));
	outtextxy(250, 525, str_user_logon);

	//����жϼ������ж�
	char name[16] = { 0 };
	char key[16] = { 0 };
	int flag_name = 1, flag_key = 1;
	MOUSEMSG m;		// ���������Ϣ
	while (1)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();
		// ������������
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 250 && m.x <= 400 && m.y >= 400 && m.y <= 425 && flag_name)
			{
				setfillcolor(BLACK);
				fillcircle(425, 413, 10);
				flag_name = 0;
				//�����˺�
				scanf("%s", name);
				settextstyle(15, 0, _T("����"));
				outtextxy(260, 405, name);
			}
			else if (m.x >= 250 && m.x <= 400 && m.y >= 450 && m.y <= 475 && flag_key)
			{
				setfillcolor(BLACK);
				fillcircle(425, 463, 10);
				flag_key = 0;
				//��������
				scanf("%s", key);
				char c[16] = { 0 };
				int len = strlen(key);
				for (int i = 0; i < len; i++) c[i] = '*';
				settextstyle(15, 0, _T("����"));
				outtextxy(260, 455, c);

			}
			else if (m.x >= 250 && m.x <= 350 && m.y >= 525 && m.y <= 575)
			{
				FILE* fp_logon;
				fp_logon = fopen("logon.txt", "a");
				fprintf(fp_logon, "%s %s", name, key);
				fputc('\n', fp_logon);
				fclose(fp_logon);
				login_menu(G);
			}
		}
	}
	system("pause");
}
void login_menu(GraphAdjList G) //��¼����
{
	initgraph(600, 800);
	// ���ñ���ɫΪ��ɫ
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	cleardevice();
	// ���û�ͼɫ
	setcolor(RGB(151, 152, 146));
	setfillcolor(RGB(228, 239, 248));
	// ������
	fillrectangle(150, 200, 450, 650);
	// ����ַ��� (VC6 / VC2008 / VC2010 / VC2012)
	setbkmode(TRANSPARENT);
	char str_load[] = "�����˻�";
	settextstyle(75, 0, _T("����"));
	outtextxy(150, 75, str_load);
	//����Բ
	setcolor(RGB(151, 152, 146));
	setfillcolor(WHITE);
	fillellipse(250, 225, 350, 325);
	char str_user[] = "�û�";
	settextstyle(40, 0, _T("����"));
	outtextxy(265, 255, str_user);
	//�˺�
	fillrectangle(250, 400, 400, 425);
	char str_user_name[] = "�˺ţ�";
	settextstyle(25, 0, _T("����"));
	outtextxy(170, 400, str_user_name);
	//����
	fillrectangle(250, 450, 400, 475);
	char str_user_key[] = "���룺";
	settextstyle(25, 0, _T("����"));
	outtextxy(170, 450, str_user_key);
	//���ע��
	char str_user_logon[] = "û���˺ţ����ע��";
	settextstyle(25, 0, _T("����"));
	outtextxy(215, 600, str_user_logon);
	//�����¼���
	char str_user_login[] = "��¼";
	settextstyle(50, 0, _T("����"));
	outtextxy(250, 525, str_user_login);

	//����жϼ������ж�
	Login user[MAX_USER] = { 0 };
	char str_user_error[] = "�˺Ż��������";
	char name[16] = { 0 };
	char key[16] = { 0 };
	char name_fp[16] = { 0 };
	char key_fp[16] = { 0 };
	int flag_name = 1, flag_key = 1;
	MOUSEMSG m;		// ���������Ϣ
	while (1)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();
		// ������������
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 250 && m.x <= 400 && m.y >= 400 && m.y <= 425 && flag_name)
			{
				setfillcolor(BLACK);
				fillcircle(425, 413, 10);
				flag_name = 0;
				//�����˺�
				scanf("%s", name);
				settextstyle(15, 0, _T("����"));
				outtextxy(260, 405, name);

			}
			else if (m.x >= 250 && m.x <= 400 && m.y >= 450 && m.y <= 475 && flag_key)
			{
				setfillcolor(BLACK);
				fillcircle(425, 463, 10);
				flag_key = 0;
				//��������
				scanf("%s", key);
				char c[16] = { 0 };
				int len = strlen(key);
				for (int i = 0; i < len; i++) c[i] = '*';
				settextstyle(15, 0, _T("����"));
				outtextxy(260, 455, c);

			}
			else if (m.x >= 250 && m.x <= 350 && m.y >= 525 && m.y <= 575)
			{
				FILE* fp_login;
				fp_login = fopen("logon.txt", "rt");
				int i = 0;
				while (fscanf(fp_login, "%s %s", user[i].name, user[i].key) != EOF)
				{
					if (strcmp(name, user[i].name) == 0)
					{
						if (strcmp(key, user[i].key) == 0)
						{
							strcpy(name_fp, user[i].name);
							strcpy(key_fp, user[i].key);
						}
					}
					i++;
				}

				//��������˳�
				//system("pause");

				if (strcmp(name, name_fp) == 0)
				{
					if (strcmp(key, key_fp) == 0)
					{
						//exit(-1);  //ʹ��������
						//��ȡ���е�ͼ�Ľڵ���Ϣ
						closegraph();
						main_menu(G); //��Ϊ���˵��������¼�����������˵��߼�
					}
					else
					{
						settextstyle(50, 0, _T("����"));
						outtextxy(125, 700, str_user_error);
					}
				}
				else
				{
					settextstyle(50, 0, _T("����"));
					outtextxy(125, 700, str_user_error);
				}
			}
			else if (m.x >= 215 && m.x <= 450 && m.y >= 600 && m.y <= 650)
			{
				logon_menu(G); //ע��
				login_menu(G); //��¼
			}
		}
	}
}

void main_menu(GraphAdjList G)
{
	//������
	initgraph(600, 800, SHOWCONSOLE);
	system("cls");
	// ���ñ���ɫΪ��ɫ
	setbkcolor(WHITE);
	// �ñ���ɫ�����Ļ
	cleardevice();
	//���û�ͼɫ
	//setcolor(RGB(58, 88, 138));
	setfillcolor(RGB(174, 223, 255));
	// ������
	setcolor(BLACK);
	fillrectangle(0, 700, 600, 800);
	setfillcolor(RGB(251, 229, 236));
	fillrectangle(50, 100, 250, 200);  //���
	fillrectangle(350, 100, 550, 200); //�ұ�
	//����
//	setcolor(RGB(251, 229, 236));  //��ɫ
	setcolor(BLACK);
	line(200, 700, 200, 800);   //"��ѯ��Ʊ"��(0,700,200,800) 
	line(400, 700, 400, 800);   //"�ҵĶ���"��(200,700,400,800)
								//"��������"��(400,700,600,800)
	//��Բ
	setcolor(BLACK);
	setfillcolor(RGB(251, 229, 236));
	fillellipse(200, 200, 400, 300);   //����
	//ѡ����
	setbkmode(TRANSPARENT);
	settextstyle(49, 0, _T("����"));
	outtextxy(250, 225, "����");
	outtextxy(0 + 2, 725, "��ѯ��Ʊ");
	outtextxy(200 + 2, 725, "�ҵĶ���");
	outtextxy(400 + 2, 725, "��������");
	//��ͼ

	//ѡ��
	int flag_1 = 0;
	int flag_2 = 0;
	int city_f1 = 0;
	int city_f2 = 0;
	char temp[16] = { 0 };
	char city_1[16] = { 0 };
	char city_2[16] = { 0 };
	//Ticket p;
	Ticket re[MAX_NODE];
	memset(re, 0, sizeof re);
	
	MOUSEMSG m;		// ���������Ϣ
	while (1)
	{
		// ��ȡһ�������Ϣ
		m = GetMouseMsg();
		// ������������
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 50 && m.x <= 250 && m.y >= 100 && m.y <= 200)
			{
				flag_1 = 1;
				setfillcolor(WHITE);
				fillrectangle(50, 100, 250, 200);  //���
				scanf("%s", city_1);
				settextstyle(50, 0, _T("����"));
				outtextxy(60, 125, city_1);
			}
			else if (m.x >= 350 && m.x <= 550 && m.y >= 100 && m.y <= 200)
			{
				flag_2 = 1;
				setfillcolor(WHITE);
				fillrectangle(350, 100, 550, 200); //�ұ�
				scanf("%s", city_2);
				settextstyle(50, 0, _T("����"));
				outtextxy(360, 125, city_2);
			}
			else if (m.x >= 200 && m.x <= 400 && m.y >= 200 && m.y <= 300 && flag_1 && flag_2)
			{
				int i = 0, j = 0, num_t = 0;
				flag_1 = 0;
				flag_2 = 0;
				setfillcolor(WHITE);
				fillellipse(200, 200, 400, 300);   //����
				settextstyle(49, 0, _T("����"));
				setbkmode(TRANSPARENT);
				outtextxy(250, 225, "����");
				//����
				FILE* fp;
				fp = fopen("map.txt", "rt");
				for (i = 0; i < G.numVertexes; i++)
				{
					fscanf(fp, "%s", temp);
					if (strcmp(city_1, temp) == 0) city_f1 = i;
				}
				rewind(fp);
				for (i = 0; i < G.numVertexes; i++)
				{
					fscanf(fp, "%s", temp);
					if (strcmp(city_2, temp) == 0) city_f2 = i;
				}
				printf("\n��%s��%s�����ٻ���Ϊ%d\n", city_1, city_2, dis_money[city_f1][city_f2]);
				printf("��%s��%s�����ʱ��Ϊ%d\n", city_1, city_2, dis_time[city_f1][city_f2]);
				i = 0;
				//j = 0;
				j = 0 ;
				//�ļ��򲻿�����ȡ��������
				FILE* fp_t;
				fp_t = fopen("ticket.txt", "r");
				while (fscanf(fp_t, "%d %d %d %d %s", &re[j].start, &re[j].end, &re[j].num, &re[j].site, re[j].time) != EOF)   //����
				{
					//fscanf(fp_t, "%d %d %d %d %s", &p.start, &p.end, &p.num, &p.site, p.time);
					//re[j].start = p.start, re[j].end = p.end, re[j].num = p.num, re[j].site = p.site;
					//strcpy(re[j].time, p.time);

					//if (city_f1 == re[j].start && city_f2 == re[j].end && re[j].num != 0)
					//{
					//	char num[16] = { 0 };
					//	char site[16] = { 0 };
					//	//��Ҫһ������Ʊ�ĺ���������p
					//	setcolor(BLACK);
					//	setfillcolor(RGB(162, 220, 159));
					//	// ������
					//	fillrectangle(0, 0 + i * 100+300, 600, 100 + i * 100 + 300);
					//	//д��Ϣ
					//	setbkmode(TRANSPARENT);
					//	settextstyle(10, 0, _T("����"));
					//	outtextxy(50, 85 + i * 100 + 300, "ȥ��");
					//	settextstyle(25, 0, _T("����"));
					//	outtextxy(0, 75 + i * 100 + 300, city_1);
					//	outtextxy(75, 75 + i * 100 + 300, city_2);
					//	outtextxy(150 + 200, 75 + i * 100 + 300, "����:");
					//	sprintf(num, "%d", re[j].num);
					//	outtextxy(210 + 200, 75 + i * 100 + 300, num);
					//	outtextxy(150 + 200, 25 + i * 100 + 300, "��λ:");
					//	sprintf(site, "%d", re[j].site);
					//	outtextxy(210 + 200, 25 + i * 100 + 300, site);
					//	outtextxy(0, 25 + i * 100 + 300, "ʱ��:");
					//	outtextxy(60, 25 + i * 100+300, re[j].time);
					//	//��Ҫÿ�����ӵ��������
					//	i++;
					//}
					if (city_f1 == re[j].start && city_f2 == re[j].end && re[j].num != 1)
					{
						char num[16] = { 0 };
						char site[16] = { 0 };
						//��Ҫһ������Ʊ�ĺ���������p
						setcolor(BLACK);
						setfillcolor(RGB(162, 220, 159));
						// ������
						fillrectangle(0, 0 + i * 100 + 300, 600, 100 + i * 100 + 300);
						//д��Ϣ
						setbkmode(TRANSPARENT);
						settextstyle(10, 0, _T("����"));
						outtextxy(50, 85 + i * 100 + 300, "ȥ��");
						settextstyle(25, 0, _T("����"));
						outtextxy(0, 75 + i * 100 + 300, city_1);
						outtextxy(75, 75 + i * 100 + 300, city_2);
						outtextxy(150 + 200, 75 + i * 100 + 300, "����:");
						sprintf(num, "%d", re[j].num);
						outtextxy(210 + 200, 75 + i * 100 + 300, num);
						outtextxy(150 + 200, 25 + i * 100 + 300, "��λ:");
						sprintf(site, "%d", re[j].site);
						outtextxy(210 + 200, 25 + i * 100 + 300, site);
						outtextxy(0, 25 + i * 100 + 300, "ʱ��:");
						outtextxy(60, 25 + i * 100 + 300, re[j].time);
						//��Ҫÿ�����ӵ��������
						i++;
					}
					j++;
				}
				printf("����Ҫ����ĳ�Ʊ�ĳ���:\n");
				scanf("%d", &num_t);
				//bug
				i = 0;
				FILE* fp_ed;
				fp_ed = fopen("tked.txt", "a+");
				rewind(fp_t);
				while (fscanf(fp_t, "%d %d %d %d %s", &re[i].start, &re[i].end, &re[i].num, &re[i].site, re[i].time) != EOF)
				{
					if (num_t == re[i].num)
					{
						fprintf(fp_ed, "%d %d %d %d %s", re[i].start, re[i].end, re[i].num, re[i].site, re[i].time);
						fputc('\n', fp_ed); //���ܻ����
						re[i].num = 1;
					}
					i++;
				}
				j = i;
				i = 0;
				FILE* fp_t_w;
				fp_t_w = fopen("ticket.txt", "w");
				fputc('\n', fp_t_w);
				fclose(fp_t_w);
				fp_t_w = fopen("ticket.txt", "a+");
				while (i < j)
				{
					fprintf(fp_t_w, "%d %d %d %d %s", re[i].start, re[i].end, re[i].num, re[i].site, re[i].time);
					fputc('\n', fp_t_w); //���ܻ����
					i++;
				}
				printf("\n��Ʊ�ɹ�\n");
				fclose(fp);
				fclose(fp_t);
				fclose(fp_ed);
				fclose(fp_t_w);
			}
			else if (m.x >= 0 && m.x <= 200 && m.y >= 700 && m.y <= 800)  //��ѯ��Ʊ
			{
				setbkmode(TRANSPARENT);
				setfillcolor(WHITE);
				fillrectangle(0, 0, 600, 700);
				FILE* fp;
				fp = fopen("map.txt", "rt");
				FILE* fp_t;
				fp_t = fopen("ticket.txt", "r");
				int i = 0, k = 0, c = 0;
				while (fscanf(fp_t, "%d %d %d %d %s", &re[i].start, &re[i].end, &re[i].num, &re[i].site, re[i].time) != EOF)
				{
					int j;
					for (j = 0; j < G.numVertexes; j++)
					{
						if (j == re[i].start)
						{
							//fscanf(fp, "%s", temp);
							strcpy(city_1, G.adjlist[j].data);
							break;
						}
					}
					for (j = 0; j < G.numVertexes; j++)
					{
						if (j == re[i].end)
						{
							//fscanf(fp, "%s", temp);
							strcpy(city_2, G.adjlist[j].data);
							break;
						}
					}
					if (re[i].num == 1) k++;
					if (re[i].num != 1)
					{
						//rewind(fp);
						char num[16] = { 0 };
						char site[16] = { 0 };
						//��Ҫһ������Ʊ�ĺ���������p
						setcolor(BLACK);
						setfillcolor(RGB(162, 220, 159));
						//// ������
						//fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
						////д��Ϣ
						//setbkmode(TRANSPARENT);
						//settextstyle(10, 0, _T("����"));
						//outtextxy(50, 85 + i * 100, "ȥ��");
						//settextstyle(25, 0, _T("����"));
						//outtextxy(0, 75 + i * 100, city_1);
						//outtextxy(75, 75 + i * 100, city_2);
						//outtextxy(150 + 200, 75 + i * 100, "����:");
						//sprintf(num, "%d", re[i].num);
						//outtextxy(210 + 200, 75 + i * 100, num);
						//outtextxy(150 + 200, 25 + i * 100, "��λ:");
						//sprintf(site, "%d", re[i].site);
						//outtextxy(210 + 200, 25 + i * 100, site);
						//outtextxy(0, 25 + i * 100, "ʱ��:");
						//outtextxy(60, 25 + i * 100, re[i].time);
						////��Ҫÿ�����ӵ��������
						// ������
						fillrectangle(0, 0 + c * 100, 600, 100 + c * 100);
						//д��Ϣ
						setbkmode(TRANSPARENT);
						settextstyle(10, 0, _T("����"));
						outtextxy(50, 85 + c * 100, "ȥ��");
						settextstyle(25, 0, _T("����"));
						outtextxy(0, 75 + c * 100, city_1);
						outtextxy(75, 75 + c * 100, city_2);
						outtextxy(150 + 200, 75 + c * 100, "����:");
						sprintf(num, "%d", re[k].num);
						outtextxy(210 + 200, 75 + c * 100, num);
						outtextxy(150 + 200, 25 + c * 100, "��λ:");
						sprintf(site, "%d", re[k].site);
						outtextxy(210 + 200, 25 + c * 100, site);
						outtextxy(0, 25 + c * 100, "ʱ��:");
						outtextxy(60, 25 + c * 100, re[k].time);
						//��Ҫÿ�����ӵ��������
						k++;
						c++;
					}
				//	//rewind(fp);
				//	char num[16] = { 0 };
				//	char site[16] = { 0 };
				//	//��Ҫһ������Ʊ�ĺ���������p
				//	setcolor(BLACK);
				//	setfillcolor(RGB(162, 220, 159));
				//	// ������
				//	fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
				//	//д��Ϣ
				//	setbkmode(TRANSPARENT);
				//	settextstyle(10, 0, _T("����"));
				//	outtextxy(50, 85 + i * 100, "ȥ��");
				//	settextstyle(25, 0, _T("����"));
				//	outtextxy(0, 75 + i * 100, city_1);
				//	outtextxy(75, 75 + i * 100, city_2);
				//	outtextxy(150 + 200, 75 + i * 100, "����:");
				//	sprintf(num, "%d", re[i].num);
				//	outtextxy(210 + 200, 75 + i * 100, num);
				//	outtextxy(150 + 200, 25 + i * 100, "��λ:");
				//	sprintf(site, "%d", re[i].site);
				//	outtextxy(210 + 200, 25 + i * 100, site);
				//	outtextxy(0, 25 + i * 100, "ʱ��:");
				//	outtextxy(60, 25 + i * 100, re[i].time);
				//  ��Ҫÿ�����ӵ��������
					i++;
				}
				fclose(fp);
				fclose(fp_t);
			}
			else if (m.x >= 200 && m.x <= 400 && m.y >= 700 && m.y <= 800)  //�ҵĶ���
			{
				setbkmode(TRANSPARENT);
				setfillcolor(WHITE);
				fillrectangle(0, 0, 600, 700);
				FILE* fp_ed;
				/*fp_ed = fopen("tked.txt", "r");*/
				fp_ed = fopen("tked.txt", "r");
				int i = 0;
				int j = 0;
				while (fscanf(fp_ed, "%d %d %d %d %s", &re[i].start, &re[i].end, &re[i].num, &re[i].site, re[i].time) != EOF)
				{
					FILE* fp;
					fp = fopen("map.txt", "rt");
					for (j = 0; j < G.numVertexes; j++)
					{
						if (j == re[i].start)
						{
							//fscanf(fp, "%s", temp);
							strcpy(city_1, G.adjlist[j].data);
							break;
						}
					}
					for (j = 0; j < G.numVertexes; j++)
					{
						if (j == re[i].end)
						{
							//fscanf(fp, "%s", temp);
							strcpy(city_2, G.adjlist[j].data);
							break;
						}
					}
					if (re[i].num != 1)
					{
						char num[16] = { 0 };
						char site[16] = { 0 };
						//��Ҫһ������Ʊ�ĺ���������p
						setcolor(BLACK);
						setfillcolor(RGB(162, 220, 159));
						// ������
						fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
						//д��Ϣ
						setbkmode(TRANSPARENT);
						settextstyle(10, 0, _T("����"));
						outtextxy(50, 85 + i * 100, "ȥ��");
						settextstyle(25, 0, _T("����"));
						outtextxy(0, 75 + i * 100, city_1);
						outtextxy(75, 75 + i * 100, city_2);
						outtextxy(150 + 200, 75 + i * 100, "����:");
						sprintf(num, "%d", re[i].num);
						outtextxy(210 + 200, 75 + i * 100, num);
						outtextxy(150 + 200, 25 + i * 100, "��λ:");
						sprintf(site, "%d", re[i].site);
						outtextxy(210 + 200, 25 + i * 100, site);
						outtextxy(0, 25 + i * 100, "ʱ��:");
						outtextxy(60, 25 + i * 100, re[i].time);
						//��Ҫÿ�����ӵ��������
					}
					//char num[16] = { 0 };
					//char site[16] = { 0 };
					////��Ҫһ������Ʊ�ĺ���������p
					//setcolor(BLACK);
					//setfillcolor(RGB(162, 220, 159));
					//// ������
					//fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
					////д��Ϣ
					//setbkmode(TRANSPARENT);
					//settextstyle(10, 0, _T("����"));
					//outtextxy(50, 85 + i * 100, "ȥ��");
					//settextstyle(25, 0, _T("����"));
					//outtextxy(0, 75 + i * 100, city_1);
					//outtextxy(75, 75 + i * 100, city_2);
					//outtextxy(150 + 200, 75 + i * 100, "����:");
					//sprintf(num, "%d", re[i].num);
					//outtextxy(210 + 200, 75 + i * 100, num);
					//outtextxy(150 + 200, 25 + i * 100, "��λ:");
					//sprintf(site, "%d", re[i].site);
					//outtextxy(210 + 200, 25 + i * 100, site);
					//outtextxy(0, 25 + i * 100, "ʱ��:");
					//outtextxy(60, 25 + i * 100, re[i].time);
					////��Ҫÿ�����ӵ��������
					i++;
				}
				fclose(fp_ed);

				int key = 0;
				int key_num = 0;
				printf("�Ƿ���Ʊ:(1/0)\n");
				scanf("%d", &key);
				if (key)
				{
					printf("������Ʊ�ĳ���:\n");
					scanf("%d", &key_num);
					
					//�ļ���д����������Ʊ�����������
					i = 0;
					FILE* fp_ed;
					FILE* fp_t;
					fp_ed = fopen("tked.txt", "a+");
					fp_t = fopen("ticket.txt", "a+");
					while (fscanf(fp_ed, "%d %d %d %d %s", &re[i].start, &re[i].end, &re[i].num, &re[i].site, re[i].time) != EOF)
					{
						if (key_num == re[i].num)
						{
							fprintf(fp_t, "%d %d %d %d %s", re[i].start, re[i].end, re[i].num, re[i].site, re[i].time);
							fputc('\n', fp_t); //���ܻ����
						}
						i++;
					}
					fclose(fp_ed);
					fclose(fp_t);

					FILE* fp_t_w;
					fp_t_w = fopen("tked.txt", "w");
					fputc('\n', fp_t_w);
					fclose(fp_t_w);
					int k = i;
					if (k > 1)
					{
						fp_ed = fopen("tked.txt", "a+");
						for (int j = 1; j < k; j++)
						{
							fprintf(fp_ed, "%d %d %d %d %s", re[i].start, re[i].end, re[i].num, re[i].site, re[i].time);
							fputc('\n', fp_ed);
						}
					}
					printf("��Ʊ�ɹ�\n");
					fclose(fp_ed);
				}
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 700 && m.y <= 800)
			{
				////��Ҫһ������Ʊ�ĺ���������p
				//setcolor(BLACK);
				//setfillcolor(RGB(162,220,159));
				//// ������
				//fillrectangle(0, 0, 600, 100);
				////д��Ϣ
				//setbkmode(TRANSPARENT);
				//settextstyle(10, 0, _T("����"));
				//outtextxy(50, 85, "ȥ��");
				//settextstyle(25, 0, _T("����"));
				//outtextxy(0, 75, city_1);
				//outtextxy(75, 75, city_2);
				//outtextxy(150+200, 75, "����:");
				//outtextxy(150+200, 25, "��λ:");
				//outtextxy(0, 25, "ʱ��:");
				char name[16] = { 0 };
				char password[20] = { 0 };
				char tel[20] = { 0 };
				setbkmode(TRANSPARENT);
				setfillcolor(RGB(251, 229, 236));
				fillrectangle(0, 0, 600, 700);
				//����Բ
				setcolor(RGB(151, 152, 146));
				setfillcolor(WHITE);
				fillellipse(250, 225, 350, 325);
				char str_user[] = "�û�";
				settextstyle(40, 0, _T("����"));
				outtextxy(265, 255, str_user);
				FILE* fp_inf;
				fp_inf = fopen("inf.txt", "rt");
				while (fscanf(fp_inf, "%s %s %s", name, password, tel) != EOF)
				{
					outtextxy(25, 350, "����:");
					outtextxy(25, 400, "���֤��:");
					outtextxy(25, 450, "�绰����:");
					outtextxy(225, 350, name);
					outtextxy(225, 400, password);
					outtextxy(225, 450, tel);
				}
				fclose(fp_inf);
				int back = 0;
				printf("�Ƿ񷵻���������:(1/0)\n");
				scanf("%d", &back);
				if (back)
				{
					main_menu(G);
				}
			}
		}
	}
}