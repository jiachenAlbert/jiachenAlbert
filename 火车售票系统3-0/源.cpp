/*
	开始：2021年12月27日8点00分00秒
	完成：2021年12月30日11点47分30秒
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
}Login; //账号
typedef char VertexType;  //顶点类型
typedef struct edge
{
	int money;
	int time;
}EdgeType;  //边上权值
typedef struct EdgeNode  //节点
{
	int adjvex; //存储顶点对应下标
	EdgeType weight; //用于存储权值
	struct EdgeNode* next;
}EdgeNode;
typedef struct VertexNode //顶点表
{
	VertexType data[16]; //顶点域
	EdgeNode* firstedge; //边表头指针
}VertexNode, AdjList[MAX_NODE];
typedef struct graph
{
	AdjList adjlist;
	int numVertexes;  //图中当前顶点数
	int numEdges;     //图中当前边数
}GraphAdjList;
typedef struct ticket
{
	int start; //起始
	int end;  //结束
	int num;  //车次
	int site; //座位
	char time[16]; //时间
}Ticket;

void main_menu(GraphAdjList G); //作为主菜单，负责登录界面与三个菜单逻辑逻辑
void login_menu(GraphAdjList G); //登录界面
void logon_menu(GraphAdjList G); //注册界面
void CreatALGraph(GraphAdjList* G);  //读取城市数据
void floyd_money(GraphAdjList G); //计算最少钱
void floyd_time(GraphAdjList G); //计算最少时间

int dis_money[MAX_NODE][MAX_NODE]; //存储最少钱
int dis_time[MAX_NODE][MAX_NODE]; //存储最少时间

int main()
{
	GraphAdjList G;
	memset(G.adjlist, 0x00, sizeof G.adjlist); //城市名初始化为空
	CreatALGraph(&G); //读取城市信息，创建图
	floyd_money(G);   //计算最少钱
	floyd_time(G);    //计算最少时间

	login_menu(G); //登录界面


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
		EdgeNode* s = G.adjlist[i].firstedge; //可能出错
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
	//打印最短路径
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
		EdgeNode* s = G.adjlist[i].firstedge; //可能出错
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
	//打印最短路径
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
		numVertexes++; //顶点数
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

		numEdges++; //边数
	}
	G->numEdges = numEdges;
}
void logon_menu(GraphAdjList G) //注册界面
{
	initgraph(600, 800);
	// 设置背景色为白色
	setbkcolor(WHITE);
	// 用背景色清空屏幕
	cleardevice();
	// 设置绘图色
	setcolor(RGB(151, 152, 146));
	setfillcolor(RGB(228, 239, 248));
	// 画矩形
	fillrectangle(150, 200, 450, 650);
	// 输出字符串 (VC6 / VC2008 / VC2010 / VC2012)
	setbkmode(TRANSPARENT);
	char str_load[] = "注册账户";
	settextstyle(75, 0, _T("宋体"));
	outtextxy(150, 75, str_load);
	//画椭圆
	setcolor(RGB(151, 152, 146));
	setfillcolor(WHITE);
	fillellipse(250, 225, 350, 325);
	char str_user[] = "用户";
	settextstyle(40, 0, _T("宋体"));
	outtextxy(265, 255, str_user);
	//账号
	fillrectangle(250, 400, 400, 425);
	char str_user_name[] = "账号：";
	settextstyle(25, 0, _T("宋体"));
	outtextxy(170, 400, str_user_name);
	//密码
	fillrectangle(250, 450, 400, 475);
	char str_user_key[] = "密码：";
	settextstyle(25, 0, _T("宋体"));
	outtextxy(170, 450, str_user_key);
	//点击注册完成
	char str_user_logon[] = "注册";
	settextstyle(50, 0, _T("宋体"));
	outtextxy(250, 525, str_user_logon);

	//鼠标判断及输入判断
	char name[16] = { 0 };
	char key[16] = { 0 };
	int flag_name = 1, flag_key = 1;
	MOUSEMSG m;		// 定义鼠标消息
	while (1)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		// 如果是左键按下
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 250 && m.x <= 400 && m.y >= 400 && m.y <= 425 && flag_name)
			{
				setfillcolor(BLACK);
				fillcircle(425, 413, 10);
				flag_name = 0;
				//输入账号
				scanf("%s", name);
				settextstyle(15, 0, _T("宋体"));
				outtextxy(260, 405, name);
			}
			else if (m.x >= 250 && m.x <= 400 && m.y >= 450 && m.y <= 475 && flag_key)
			{
				setfillcolor(BLACK);
				fillcircle(425, 463, 10);
				flag_key = 0;
				//输入密码
				scanf("%s", key);
				char c[16] = { 0 };
				int len = strlen(key);
				for (int i = 0; i < len; i++) c[i] = '*';
				settextstyle(15, 0, _T("宋体"));
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
void login_menu(GraphAdjList G) //登录界面
{
	initgraph(600, 800);
	// 设置背景色为白色
	setbkcolor(WHITE);
	// 用背景色清空屏幕
	cleardevice();
	// 设置绘图色
	setcolor(RGB(151, 152, 146));
	setfillcolor(RGB(228, 239, 248));
	// 画矩形
	fillrectangle(150, 200, 450, 650);
	// 输出字符串 (VC6 / VC2008 / VC2010 / VC2012)
	setbkmode(TRANSPARENT);
	char str_load[] = "登入账户";
	settextstyle(75, 0, _T("宋体"));
	outtextxy(150, 75, str_load);
	//画椭圆
	setcolor(RGB(151, 152, 146));
	setfillcolor(WHITE);
	fillellipse(250, 225, 350, 325);
	char str_user[] = "用户";
	settextstyle(40, 0, _T("宋体"));
	outtextxy(265, 255, str_user);
	//账号
	fillrectangle(250, 400, 400, 425);
	char str_user_name[] = "账号：";
	settextstyle(25, 0, _T("宋体"));
	outtextxy(170, 400, str_user_name);
	//密码
	fillrectangle(250, 450, 400, 475);
	char str_user_key[] = "密码：";
	settextstyle(25, 0, _T("宋体"));
	outtextxy(170, 450, str_user_key);
	//点击注册
	char str_user_logon[] = "没有账号？点击注册";
	settextstyle(25, 0, _T("宋体"));
	outtextxy(215, 600, str_user_logon);
	//点击登录完成
	char str_user_login[] = "登录";
	settextstyle(50, 0, _T("宋体"));
	outtextxy(250, 525, str_user_login);

	//鼠标判断及输入判断
	Login user[MAX_USER] = { 0 };
	char str_user_error[] = "账号或密码错误！";
	char name[16] = { 0 };
	char key[16] = { 0 };
	char name_fp[16] = { 0 };
	char key_fp[16] = { 0 };
	int flag_name = 1, flag_key = 1;
	MOUSEMSG m;		// 定义鼠标消息
	while (1)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		// 如果是左键按下
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 250 && m.x <= 400 && m.y >= 400 && m.y <= 425 && flag_name)
			{
				setfillcolor(BLACK);
				fillcircle(425, 413, 10);
				flag_name = 0;
				//输入账号
				scanf("%s", name);
				settextstyle(15, 0, _T("宋体"));
				outtextxy(260, 405, name);

			}
			else if (m.x >= 250 && m.x <= 400 && m.y >= 450 && m.y <= 475 && flag_key)
			{
				setfillcolor(BLACK);
				fillcircle(425, 463, 10);
				flag_key = 0;
				//输入密码
				scanf("%s", key);
				char c[16] = { 0 };
				int len = strlen(key);
				for (int i = 0; i < len; i++) c[i] = '*';
				settextstyle(15, 0, _T("宋体"));
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

				//按任意键退出
				//system("pause");

				if (strcmp(name, name_fp) == 0)
				{
					if (strcmp(key, key_fp) == 0)
					{
						//exit(-1);  //使用来测试
						//读取所有的图的节点信息
						closegraph();
						main_menu(G); //作为主菜单，负责登录界面与三个菜单逻辑
					}
					else
					{
						settextstyle(50, 0, _T("宋体"));
						outtextxy(125, 700, str_user_error);
					}
				}
				else
				{
					settextstyle(50, 0, _T("宋体"));
					outtextxy(125, 700, str_user_error);
				}
			}
			else if (m.x >= 215 && m.x <= 450 && m.y >= 600 && m.y <= 650)
			{
				logon_menu(G); //注册
				login_menu(G); //登录
			}
		}
	}
}

void main_menu(GraphAdjList G)
{
	//主界面
	initgraph(600, 800, SHOWCONSOLE);
	system("cls");
	// 设置背景色为白色
	setbkcolor(WHITE);
	// 用背景色清空屏幕
	cleardevice();
	//设置绘图色
	//setcolor(RGB(58, 88, 138));
	setfillcolor(RGB(174, 223, 255));
	// 画矩形
	setcolor(BLACK);
	fillrectangle(0, 700, 600, 800);
	setfillcolor(RGB(251, 229, 236));
	fillrectangle(50, 100, 250, 200);  //左边
	fillrectangle(350, 100, 550, 200); //右边
	//画线
//	setcolor(RGB(251, 229, 236));  //粉色
	setcolor(BLACK);
	line(200, 700, 200, 800);   //"查询余票"：(0,700,200,800) 
	line(400, 700, 400, 800);   //"我的订单"：(200,700,400,800)
								//"个人中心"：(400,700,600,800)
	//椭圆
	setcolor(BLACK);
	setfillcolor(RGB(251, 229, 236));
	fillellipse(200, 200, 400, 300);   //搜索
	//选项栏
	setbkmode(TRANSPARENT);
	settextstyle(49, 0, _T("宋体"));
	outtextxy(250, 225, "搜索");
	outtextxy(0 + 2, 725, "查询余票");
	outtextxy(200 + 2, 725, "我的订单");
	outtextxy(400 + 2, 725, "个人中心");
	//插图

	//选择
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
	
	MOUSEMSG m;		// 定义鼠标消息
	while (1)
	{
		// 获取一条鼠标消息
		m = GetMouseMsg();
		// 如果是左键按下
		if (m.uMsg == WM_LBUTTONDOWN)
		{
			if (m.x >= 50 && m.x <= 250 && m.y >= 100 && m.y <= 200)
			{
				flag_1 = 1;
				setfillcolor(WHITE);
				fillrectangle(50, 100, 250, 200);  //左边
				scanf("%s", city_1);
				settextstyle(50, 0, _T("宋体"));
				outtextxy(60, 125, city_1);
			}
			else if (m.x >= 350 && m.x <= 550 && m.y >= 100 && m.y <= 200)
			{
				flag_2 = 1;
				setfillcolor(WHITE);
				fillrectangle(350, 100, 550, 200); //右边
				scanf("%s", city_2);
				settextstyle(50, 0, _T("宋体"));
				outtextxy(360, 125, city_2);
			}
			else if (m.x >= 200 && m.x <= 400 && m.y >= 200 && m.y <= 300 && flag_1 && flag_2)
			{
				int i = 0, j = 0, num_t = 0;
				flag_1 = 0;
				flag_2 = 0;
				setfillcolor(WHITE);
				fillellipse(200, 200, 400, 300);   //搜索
				settextstyle(49, 0, _T("宋体"));
				setbkmode(TRANSPARENT);
				outtextxy(250, 225, "搜索");
				//跳入
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
				printf("\n从%s到%s的最少花费为%d\n", city_1, city_2, dis_money[city_f1][city_f2]);
				printf("从%s到%s的最短时间为%d\n", city_1, city_2, dis_time[city_f1][city_f2]);
				i = 0;
				//j = 0;
				j = 0 ;
				//文件打不开，读取不到内容
				FILE* fp_t;
				fp_t = fopen("ticket.txt", "r");
				while (fscanf(fp_t, "%d %d %d %d %s", &re[j].start, &re[j].end, &re[j].num, &re[j].site, re[j].time) != EOF)   //错误
				{
					//fscanf(fp_t, "%d %d %d %d %s", &p.start, &p.end, &p.num, &p.site, p.time);
					//re[j].start = p.start, re[j].end = p.end, re[j].num = p.num, re[j].site = p.site;
					//strcpy(re[j].time, p.time);

					//if (city_f1 == re[j].start && city_f2 == re[j].end && re[j].num != 0)
					//{
					//	char num[16] = { 0 };
					//	char site[16] = { 0 };
					//	//需要一个画车票的函数，传入p
					//	setcolor(BLACK);
					//	setfillcolor(RGB(162, 220, 159));
					//	// 画矩形
					//	fillrectangle(0, 0 + i * 100+300, 600, 100 + i * 100 + 300);
					//	//写信息
					//	setbkmode(TRANSPARENT);
					//	settextstyle(10, 0, _T("宋体"));
					//	outtextxy(50, 85 + i * 100 + 300, "去往");
					//	settextstyle(25, 0, _T("宋体"));
					//	outtextxy(0, 75 + i * 100 + 300, city_1);
					//	outtextxy(75, 75 + i * 100 + 300, city_2);
					//	outtextxy(150 + 200, 75 + i * 100 + 300, "车次:");
					//	sprintf(num, "%d", re[j].num);
					//	outtextxy(210 + 200, 75 + i * 100 + 300, num);
					//	outtextxy(150 + 200, 25 + i * 100 + 300, "座位:");
					//	sprintf(site, "%d", re[j].site);
					//	outtextxy(210 + 200, 25 + i * 100 + 300, site);
					//	outtextxy(0, 25 + i * 100 + 300, "时间:");
					//	outtextxy(60, 25 + i * 100+300, re[j].time);
					//	//需要每次增加的坐标变量
					//	i++;
					//}
					if (city_f1 == re[j].start && city_f2 == re[j].end && re[j].num != 1)
					{
						char num[16] = { 0 };
						char site[16] = { 0 };
						//需要一个画车票的函数，传入p
						setcolor(BLACK);
						setfillcolor(RGB(162, 220, 159));
						// 画矩形
						fillrectangle(0, 0 + i * 100 + 300, 600, 100 + i * 100 + 300);
						//写信息
						setbkmode(TRANSPARENT);
						settextstyle(10, 0, _T("宋体"));
						outtextxy(50, 85 + i * 100 + 300, "去往");
						settextstyle(25, 0, _T("宋体"));
						outtextxy(0, 75 + i * 100 + 300, city_1);
						outtextxy(75, 75 + i * 100 + 300, city_2);
						outtextxy(150 + 200, 75 + i * 100 + 300, "车次:");
						sprintf(num, "%d", re[j].num);
						outtextxy(210 + 200, 75 + i * 100 + 300, num);
						outtextxy(150 + 200, 25 + i * 100 + 300, "座位:");
						sprintf(site, "%d", re[j].site);
						outtextxy(210 + 200, 25 + i * 100 + 300, site);
						outtextxy(0, 25 + i * 100 + 300, "时间:");
						outtextxy(60, 25 + i * 100 + 300, re[j].time);
						//需要每次增加的坐标变量
						i++;
					}
					j++;
				}
				printf("输入要购买的车票的车次:\n");
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
						fputc('\n', fp_ed); //可能会出错
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
					fputc('\n', fp_t_w); //可能会出错
					i++;
				}
				printf("\n购票成功\n");
				fclose(fp);
				fclose(fp_t);
				fclose(fp_ed);
				fclose(fp_t_w);
			}
			else if (m.x >= 0 && m.x <= 200 && m.y >= 700 && m.y <= 800)  //查询余票
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
						//需要一个画车票的函数，传入p
						setcolor(BLACK);
						setfillcolor(RGB(162, 220, 159));
						//// 画矩形
						//fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
						////写信息
						//setbkmode(TRANSPARENT);
						//settextstyle(10, 0, _T("宋体"));
						//outtextxy(50, 85 + i * 100, "去往");
						//settextstyle(25, 0, _T("宋体"));
						//outtextxy(0, 75 + i * 100, city_1);
						//outtextxy(75, 75 + i * 100, city_2);
						//outtextxy(150 + 200, 75 + i * 100, "车次:");
						//sprintf(num, "%d", re[i].num);
						//outtextxy(210 + 200, 75 + i * 100, num);
						//outtextxy(150 + 200, 25 + i * 100, "座位:");
						//sprintf(site, "%d", re[i].site);
						//outtextxy(210 + 200, 25 + i * 100, site);
						//outtextxy(0, 25 + i * 100, "时间:");
						//outtextxy(60, 25 + i * 100, re[i].time);
						////需要每次增加的坐标变量
						// 画矩形
						fillrectangle(0, 0 + c * 100, 600, 100 + c * 100);
						//写信息
						setbkmode(TRANSPARENT);
						settextstyle(10, 0, _T("宋体"));
						outtextxy(50, 85 + c * 100, "去往");
						settextstyle(25, 0, _T("宋体"));
						outtextxy(0, 75 + c * 100, city_1);
						outtextxy(75, 75 + c * 100, city_2);
						outtextxy(150 + 200, 75 + c * 100, "车次:");
						sprintf(num, "%d", re[k].num);
						outtextxy(210 + 200, 75 + c * 100, num);
						outtextxy(150 + 200, 25 + c * 100, "座位:");
						sprintf(site, "%d", re[k].site);
						outtextxy(210 + 200, 25 + c * 100, site);
						outtextxy(0, 25 + c * 100, "时间:");
						outtextxy(60, 25 + c * 100, re[k].time);
						//需要每次增加的坐标变量
						k++;
						c++;
					}
				//	//rewind(fp);
				//	char num[16] = { 0 };
				//	char site[16] = { 0 };
				//	//需要一个画车票的函数，传入p
				//	setcolor(BLACK);
				//	setfillcolor(RGB(162, 220, 159));
				//	// 画矩形
				//	fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
				//	//写信息
				//	setbkmode(TRANSPARENT);
				//	settextstyle(10, 0, _T("宋体"));
				//	outtextxy(50, 85 + i * 100, "去往");
				//	settextstyle(25, 0, _T("宋体"));
				//	outtextxy(0, 75 + i * 100, city_1);
				//	outtextxy(75, 75 + i * 100, city_2);
				//	outtextxy(150 + 200, 75 + i * 100, "车次:");
				//	sprintf(num, "%d", re[i].num);
				//	outtextxy(210 + 200, 75 + i * 100, num);
				//	outtextxy(150 + 200, 25 + i * 100, "座位:");
				//	sprintf(site, "%d", re[i].site);
				//	outtextxy(210 + 200, 25 + i * 100, site);
				//	outtextxy(0, 25 + i * 100, "时间:");
				//	outtextxy(60, 25 + i * 100, re[i].time);
				//  需要每次增加的坐标变量
					i++;
				}
				fclose(fp);
				fclose(fp_t);
			}
			else if (m.x >= 200 && m.x <= 400 && m.y >= 700 && m.y <= 800)  //我的订单
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
						//需要一个画车票的函数，传入p
						setcolor(BLACK);
						setfillcolor(RGB(162, 220, 159));
						// 画矩形
						fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
						//写信息
						setbkmode(TRANSPARENT);
						settextstyle(10, 0, _T("宋体"));
						outtextxy(50, 85 + i * 100, "去往");
						settextstyle(25, 0, _T("宋体"));
						outtextxy(0, 75 + i * 100, city_1);
						outtextxy(75, 75 + i * 100, city_2);
						outtextxy(150 + 200, 75 + i * 100, "车次:");
						sprintf(num, "%d", re[i].num);
						outtextxy(210 + 200, 75 + i * 100, num);
						outtextxy(150 + 200, 25 + i * 100, "座位:");
						sprintf(site, "%d", re[i].site);
						outtextxy(210 + 200, 25 + i * 100, site);
						outtextxy(0, 25 + i * 100, "时间:");
						outtextxy(60, 25 + i * 100, re[i].time);
						//需要每次增加的坐标变量
					}
					//char num[16] = { 0 };
					//char site[16] = { 0 };
					////需要一个画车票的函数，传入p
					//setcolor(BLACK);
					//setfillcolor(RGB(162, 220, 159));
					//// 画矩形
					//fillrectangle(0, 0 + i * 100, 600, 100 + i * 100);
					////写信息
					//setbkmode(TRANSPARENT);
					//settextstyle(10, 0, _T("宋体"));
					//outtextxy(50, 85 + i * 100, "去往");
					//settextstyle(25, 0, _T("宋体"));
					//outtextxy(0, 75 + i * 100, city_1);
					//outtextxy(75, 75 + i * 100, city_2);
					//outtextxy(150 + 200, 75 + i * 100, "车次:");
					//sprintf(num, "%d", re[i].num);
					//outtextxy(210 + 200, 75 + i * 100, num);
					//outtextxy(150 + 200, 25 + i * 100, "座位:");
					//sprintf(site, "%d", re[i].site);
					//outtextxy(210 + 200, 25 + i * 100, site);
					//outtextxy(0, 25 + i * 100, "时间:");
					//outtextxy(60, 25 + i * 100, re[i].time);
					////需要每次增加的坐标变量
					i++;
				}
				fclose(fp_ed);

				int key = 0;
				int key_num = 0;
				printf("是否退票:(1/0)\n");
				scanf("%d", &key);
				if (key)
				{
					printf("输入退票的车次:\n");
					scanf("%d", &key_num);
					
					//文件读写操作进行退票，对照上面的
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
							fputc('\n', fp_t); //可能会出错
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
					printf("退票成功\n");
					fclose(fp_ed);
				}
			}
			else if (m.x >= 400 && m.x <= 600 && m.y >= 700 && m.y <= 800)
			{
				////需要一个画车票的函数，传入p
				//setcolor(BLACK);
				//setfillcolor(RGB(162,220,159));
				//// 画矩形
				//fillrectangle(0, 0, 600, 100);
				////写信息
				//setbkmode(TRANSPARENT);
				//settextstyle(10, 0, _T("宋体"));
				//outtextxy(50, 85, "去往");
				//settextstyle(25, 0, _T("宋体"));
				//outtextxy(0, 75, city_1);
				//outtextxy(75, 75, city_2);
				//outtextxy(150+200, 75, "车次:");
				//outtextxy(150+200, 25, "座位:");
				//outtextxy(0, 25, "时间:");
				char name[16] = { 0 };
				char password[20] = { 0 };
				char tel[20] = { 0 };
				setbkmode(TRANSPARENT);
				setfillcolor(RGB(251, 229, 236));
				fillrectangle(0, 0, 600, 700);
				//画椭圆
				setcolor(RGB(151, 152, 146));
				setfillcolor(WHITE);
				fillellipse(250, 225, 350, 325);
				char str_user[] = "用户";
				settextstyle(40, 0, _T("宋体"));
				outtextxy(265, 255, str_user);
				FILE* fp_inf;
				fp_inf = fopen("inf.txt", "rt");
				while (fscanf(fp_inf, "%s %s %s", name, password, tel) != EOF)
				{
					outtextxy(25, 350, "姓名:");
					outtextxy(25, 400, "身份证号:");
					outtextxy(25, 450, "电话号码:");
					outtextxy(225, 350, name);
					outtextxy(225, 400, password);
					outtextxy(225, 450, tel);
				}
				fclose(fp_inf);
				int back = 0;
				printf("是否返回搜索界面:(1/0)\n");
				scanf("%d", &back);
				if (back)
				{
					main_menu(G);
				}
			}
		}
	}
}