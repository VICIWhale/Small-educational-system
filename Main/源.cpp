#define _CRT_SECURE_NO_WARNINGS
#include<graphics.h>
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<windows.h>
#include<string.h>
#include<string>
#include<stdlib.h>
#include<wchar.h>

using namespace std;

#define MAX_LEN 10 // 程序中所有字符串的最大长度
#define MAX_COURSE 5 // 最大考试项数目

typedef struct Student //  定义学生类型
{
	wchar_t num[MAX_LEN]; // 学号
	wchar_t name[MAX_LEN]; // 姓名
	wchar_t Math[MAX_LEN];  //  单科成绩
	wchar_t English[MAX_LEN];
	wchar_t Chinese[MAX_LEN];
	wchar_t sum_score[MAX_LEN]; // 总分
	wchar_t average_score[MAX_LEN]; // 平均成绩
	struct Student* next;
}Stud, * Stud_list;

int stud_nums;  // 学生总数
Stud_list Linklist; // 将链表定义为全局变量

// 图片
IMAGE start;
IMAGE start_button;
IMAGE start_menu;
IMAGE start_menu_button1;
IMAGE start_menu_button2;
IMAGE start_menu_button3;
IMAGE start_menu_button4;
IMAGE information1;
IMAGE information12;
IMAGE information13;
IMAGE information14;
IMAGE information15;
IMAGE information21;
IMAGE information22;
IMAGE exit1;
IMAGE exit2;

// 函数
//int Print_Manu(void); //  打印主界面  // 返回值为main函数中switch的匹配值
//void Insert_Stud(int n, int m, Stud_list& L); // 创建学生信息  
//void Delete_Stud_Name(wchar_t query_name[], Stud_list L); // 删除学生信息 按照学生姓名删除
//void Delete_Stud_Num(wchar_t query_num[], Stud_list L); // 删除学生信息 按照学生学号删除
//void Modify_Stud_Num(void); // 修改学生信息
//void Inquiry_Name(wchar_t query_name[], int m, Stud_list L); // 查询学生信息1 // 考虑将1 2 两种形式合并（后期）
//void Inquiry_Num(wchar_t query_num[], int m, Stud_list L); // 查询学生信息2
//void print_alls(Stud_list L, int m);

void Welcome(void);
void Select_MainMenu(void);
void insert_score(Stud_list& Linklist);
void initlinklist(Stud_list& Linklist);
void Modify_Information(Stud_list& Linklist);
void Add_Information(Stud_list& Linklist);
void Delete_informatiom(Stud_list& Linklist);
void change_information(Stud_list& Linklist);
void sort_information(Stud_list& Linklist,int n);
Stud_list inquery_information(Stud_list linklist);
void inquery_information_work(Stud_list Linklist);
void output_information(Stud_list Linklist);
void WchartoChar(char* charStr, const wchar_t* wcharStr);
void say_goodbye(void);

void WchartoChar(char* charStr, const wchar_t* wcharStr)
{
	int len = WideCharToMultiByte(CP_ACP, 0, wcharStr, wcslen(wcharStr), NULL, 0, NULL, NULL);
	WideCharToMultiByte(CP_ACP, 0, wcharStr, wcslen(wcharStr), charStr, len, NULL, NULL);
	charStr[len] = '\0';
}

void initlinklist(Stud_list& Linklist) // 初始化链表
{
	stud_nums = 0;
	Linklist = (Stud_list)malloc(sizeof(Student)); // 初始化学生链表
	Linklist->next = NULL;
}

void Welcome()
{
	loadimage(&start, L"start.jpg");
	putimage(0, 0, &start);
	MOUSEMSG msg; // MOUSEMSG是一个结构体，具体查看定义
	FlushMouseMsgBuffer(); // 清空鼠标消息缓存，防止混乱。
	while (true)
	{
		while (MouseHit()) // 监听
		{
			msg = GetMouseMsg(); // 获取鼠标信息
			if (WM_LBUTTONDOWN == msg.uMsg) // 当前鼠标信息为“左键被按下”
			{
				if (msg.x > 432 && msg.x < 848 && msg.y > 361 && msg.y < 457) //  在按钮内
				{
					loadimage(&start_button, L"start_button.jpg");
					putimage(0, 0, &start_button);
					Sleep(100);
					Select_MainMenu();
				}
			}

		}
	}
}

void Select_MainMenu()
{
	loadimage(&start_menu, L"menu0.jpg");//导入初始界面
	putimage(0, 0, &start_menu);//显示初始界面
	MOUSEMSG msg;//定义变量，保存鼠标消息
	FlushMouseMsgBuffer();
	FlushMouseMsgBuffer();
	FlushMouseMsgBuffer();
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{ 
				if (msg.x > 73 && msg.x < 295 && msg.y > 350 && msg.y < 435)    // 信息输入
				{
					loadimage(&start_menu_button1, L"menu1.jpg");
					putimage(0, 0, &start_menu_button1);
					insert_score(Linklist);
					Select_MainMenu();
				}
				if (msg.x > 368 && msg.x < 593 && msg.y > 350 && msg.y < 435)   // 信息维护 //BUG 删除
				{
					loadimage(&start_menu_button2, L"menu2.jpg");
					putimage(0, 0, &start_menu_button2);
					Modify_Information(Linklist);
				}
				if (msg.x > 662 && msg.x < 884 && msg.y > 350 && msg.y < 435)   // 成绩查询 //无BUG
				{
					loadimage(&start_menu_button3, L"menu3.jpg");
					putimage(0, 0, &start_menu_button3);
					Stud_list p = inquery_information(Linklist);
					if (p != NULL) inquery_information_work(p);
					Select_MainMenu();
				}
				if (msg.x > 958 && msg.x < 1180 && msg.y > 350 && msg.y < 435)  // 输出成绩 // 无BUG
				{
					loadimage(&start_menu_button4, L"menu4.jpg");
					putimage(0, 0, &start_menu_button4);
					output_information(Linklist);
				}
			}
		}
	}
}

void insert_score(Stud_list& Linklist) // 录入学生信息
{

	wchar_t alls_student[MAX_LEN];
	InputBox(alls_student, MAX_LEN, L"请输入学生人数：");
	int cnt = _wtoi(alls_student);
	stud_nums = cnt;
	Stud_list r = Linklist; // 定义辅助指针
	for (int i = 1; i <= cnt; i++) // 尾插法
	{
		Stud_list p = (Stud_list)malloc(sizeof(Student));

		InputBox(p->num, MAX_LEN, L"请输入学生学号：");
		InputBox(p->name, MAX_LEN, L"请输入学生姓名：");
		InputBox(p->Chinese, MAX_LEN, L"请输入语文成绩：");
		InputBox(p->Math, MAX_LEN, L"请输入数学成绩：");
		InputBox(p->English, MAX_LEN, L"请输入英语成绩：");

		 //字符转数字
		int Chinese = _wtoi(p->Chinese);
		int Math = _wtoi(p->Math);
		int English = _wtoi(p->English);

		//p->sum_score= Chinese + Math + English;
		//p->average_score = p->sum_score / 3;
		//p->next = NULL;
		int alls = Chinese + Math + English;
		int ave = alls / 3;

		_itow(alls,p->sum_score, 10);
		_itow(ave, p->average_score, 10);

		p->next = NULL;

		r->next = p;
		r = r->next;
	}
	MessageBox(NULL, _T("成绩输入成功！"), _T("成绩输入"), MB_SETFOREGROUND);
}

void Modify_Information(Stud_list& Linklist)
{
	// 图片展示
	loadimage(&information1, L"information1.jpg");
	putimage(0, 0, &information1);
		
	// 在视图中显示成绩总表
	int position_x = 230;//“序号”横坐标
	int position_y = 150;//“序号”纵坐标
	setbkmode(TRANSPARENT);//设置字体背景为透明
	settextcolor(COLORREF(RGB(0, 0, 0)));//设置字体颜色为黑色
	settextstyle(20, 0, _T("楷体"));//设置字体大小20，格式楷体
	outtextxy(position_x, position_y, _T("学号      姓名      语文成绩      数学成绩      英语成绩      总分      平均分"));//显示表格第一行表头
	 //打印成绩表
	Stud_list p = Linklist->next;
	while (p)
	{	
		position_y += 20; // 换行
		outtextxy(230, position_y, p->num);//显示学号
		outtextxy(330, position_y, p->name);//显示姓名
		outtextxy(450, position_y, p->Chinese);//语文成绩
		outtextxy(590, position_y, p->Math);//数学成绩
		outtextxy(730, position_y, p->English);//英语成绩
		outtextxy(850, position_y, p->sum_score);//总分
		outtextxy(960, position_y, p->average_score);//平均分
		p = p->next;
	}

	MOUSEMSG msg;//定义变量，保存鼠标消息
	FlushMouseMsgBuffer();
	while (true)
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 160 && msg.x < 300 && msg.y > 629 && msg.y < 695)    // 添加信息
				{
					loadimage(&information12, L"information12.jpg");
					putimage(0, 0, &information12);
					Add_Information(Linklist); // 注意完善后，需要重新显示出学生成绩
					Select_MainMenu();
				}
				if (msg.x > 390 && msg.x < 530 && msg.y > 629 && msg.y < 695)   // 删除信息
				{
					loadimage(&information13, L"information13.jpg");
					putimage(0, 0, &information13);
					Delete_informatiom(Linklist);
					Select_MainMenu();
				}
				if (msg.x > 625 && msg.x < 760 && msg.y > 629 && msg.y < 695)   // 修改信息
				{
					loadimage(&information14, L"information14.jpg");
					putimage(0, 0, &information14);
					change_information(Linklist);
					Select_MainMenu();
				}
				if (msg.x > 860 && msg.x < 1000 && msg.y > 629 && msg.y < 695)   // 排序
				{
					loadimage(&information15, L"information15.jpg");
					putimage(0, 0, &information15);
					sort_information(Linklist,stud_nums);
					Select_MainMenu();
				}
				if (msg.x > 1144 && msg.x < 1260 && msg.y > 596 && msg.y < 646)  // 返回
				{
					Select_MainMenu();
				}
			}
		}
	}
}

void Add_Information(Stud_list& Linklist) // 在末尾处添加学生
{
	stud_nums++;
	wchar_t add_student[MAX_LEN];
	InputBox(add_student, MAX_LEN, L"请输入要添加的学生人数（注：所有学生总人数不超过10人）：");
	int cnt = _wtoi(add_student);
	Stud_list r = Linklist; // 定义辅助指针
	while (r->next) // 找到末尾指针
	{
		r = r->next;
	}

	for (int i = 1; i <= cnt; i ++)
	{
		Stud_list p = (Stud_list)malloc(sizeof(Student)); // 分配空间

		InputBox(p->num, MAX_LEN, L"请输入学生学号：");
		InputBox(p->name, MAX_LEN, L"请输入学生姓名：");
		InputBox(p->Chinese, MAX_LEN, L"请输入语文成绩：");
		InputBox(p->Math, MAX_LEN, L"请输入数学成绩：");
		InputBox(p->English, MAX_LEN, L"请输入英语成绩：");

		//字符转数字
		int Chinese = _wtoi(p->Chinese);
		int Math = _wtoi(p->Math);
		int English = _wtoi(p->English);

		int alls = Chinese + Math + English;
		int ave = alls / 3;

		_itow(alls, p->sum_score, 10);
		_itow(ave, p->average_score, 10);

		p->next = NULL;

		r->next = p;
		r = r->next;
	}
}

void Delete_informatiom(Stud_list& Linklist)  //  待修改，至少保留一个学生在表中
{
	bool flag = false; // 用于判断是否删除成功
	Stud_list p = Linklist->next; // 指向首元结点的辅助指针
	if (p == NULL)
	{
		MessageBox(NULL, _T("成绩表无学生信息。"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
		Modify_Information(Linklist);
	}
	if (p->next == NULL)
	{
		MessageBox(NULL, _T("成绩表中至少保留一位学生信息。"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
		Modify_Information(Linklist);
	}
	// 判断是通过姓名还是通过学号删除 // 输入错误要重新询问
	wchar_t ways[MAX_LEN];
	wchar_t way1[MAX_LEN];
	wcscpy_s(way1, L"学号");
	wchar_t way2[MAX_LEN];
	wcscpy_s(way2, L"姓名");

	while (true)
	{
		InputBox(ways, MAX_LEN, L"通过何种方式查询？（学号/姓名）");

		if (wcscmp(ways, way1) == 0) // 学号查询删除
		{
			wchar_t nums[MAX_LEN];
			InputBox(nums, MAX_LEN, L"请输入要删除的学生学号");
			if (wcscmp(nums, p->num) == 0) // 特判首元结点
			{
				Stud_list r = Linklist;
				r->next = p->next;
				delete(p);
				stud_nums--;
				flag = true;
				MessageBox(NULL, _T("删除成功！"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
				break;
			}
			while (p->next) // 非头结点 站在前一个结点删除后一个结点
			{
				if (wcscmp(nums, p->next->num) == 0) // 匹配成功
				{
					Stud_list r = p->next;
					p->next = r->next;
					delete(r);
					stud_nums--;
					flag = true;
					MessageBox(NULL, _T("删除成功！"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
					break;
				}
				else p = p->next;
			}
			if(!flag) // 删除失败
				MessageBox(NULL, _T("删除失败！未查询到相关信息。"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
			break;
		}
		else if (wcscmp(ways, way2) == 0) // 姓名查询删除
		{
			wchar_t names[MAX_LEN];
			InputBox(names, MAX_LEN, L"请输入要删除的学生姓名");
			if (wcscmp(names, p->name) == 0) // 特判首元结点
			{
				Stud_list r = Linklist;
				r->next = p->next;
				delete(p);
				stud_nums--;
				flag = true;
				MessageBox(NULL, _T("删除成功！"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
				break;
			}
			while (p->next) // 非头结点 站在前一个结点删除后一个结点
			{
				if (wcscmp(names, p->next->name) == 0) // 匹配成功
				{
					Stud_list r = p->next;
					p->next = r->next;
					delete(r);
					flag = true;
					stud_nums--;
					MessageBox(NULL, _T("删除成功！"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
					break;
				}
				else p = p->next;
			}
			if (!flag) // 删除失败
				MessageBox(NULL, _T("删除失败！未查询到相关信息。"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
			break;
		}
		else
		{
			MessageBox(NULL, _T("输入错误！请重新输入"), _T("信息删除"), MB_ICONSTOP | MB_SETFOREGROUND);
			Modify_Information(Linklist);
		}
	}
} 

void change_information(Stud_list& Linklist)
{
	bool flag = false; // 用于判断是否删除成功
	Stud_list p = Linklist->next; // 指向首元结点的辅助指针
	// 判断是通过姓名还是通过学号 // 输入错误要重新询问
	wchar_t ways[MAX_LEN];
	wchar_t way1[MAX_LEN];
	wcscpy_s(way1, L"学号");
	wchar_t way2[MAX_LEN];
	wcscpy_s(way2, L"姓名");

	while (true)
	{
		InputBox(ways, MAX_LEN, L"通过何种方式查询？（学号/姓名）");

		if (wcscmp(ways, way1) == 0) // 学号查询修改信息
		{
			wchar_t nums[MAX_LEN];
			InputBox(nums, MAX_LEN, L"请输入要修改信息的学生学号：");

			while (p) // 开始查找
			{
				if (wcscmp(nums, p->num) == 0) // 匹配成功
				{
					//  修改内容
					InputBox(p->Chinese, MAX_LEN, L"请输入修改后的语文成绩：");
					InputBox(p->Math, MAX_LEN, L"请输入修改后的数学成绩：");
					InputBox(p->English, MAX_LEN, L"请输入修改后的英语成绩：");

					//字符转数字
					int Chinese = _wtoi(p->Chinese);
					int Math = _wtoi(p->Math);
					int English = _wtoi(p->English);

					int alls = Chinese + Math + English;
					int ave = alls / 3;

					_itow(alls, p->sum_score, 10);
					_itow(ave, p->average_score, 10);
					flag = true;
					MessageBox(NULL, _T("修改成功！"), _T("信息修改"), MB_ICONSTOP | MB_SETFOREGROUND);
					break;
				}
				p = p->next;
			}
			if (!flag) // 删除失败
				MessageBox(NULL, _T("修改失败！未查询到相关信息。"), _T("信息修改"), MB_ICONSTOP | MB_SETFOREGROUND);
			break;
		}
		else if (wcscmp(ways, way2) == 0) // 姓名查询删除
		{
			wchar_t names[MAX_LEN];
			InputBox(names, MAX_LEN, L"请输入要修改信息的学生姓名：");
		
			while (p)
			{
				if (wcscmp(names, p->num) == 0) // 匹配成功
				{
					// 修改信息

					//  修改内容
					InputBox(p->Chinese, MAX_LEN, L"请输入语文成绩：");
					InputBox(p->Math, MAX_LEN, L"请输入数学成绩：");
					InputBox(p->English, MAX_LEN, L"请输入英语成绩：");

					//字符转数字
					int Chinese = _wtoi(p->Chinese);
					int Math = _wtoi(p->Math);
					int English = _wtoi(p->English);

					int alls = Chinese + Math + English;
					int ave = alls / 3;

					_itow(alls, p->sum_score, 10);
					_itow(ave, p->average_score, 10);
					flag = true;
					MessageBox(NULL, _T("修改成功！"), _T("信息修改"), MB_ICONSTOP | MB_SETFOREGROUND);
					break;
				}
				p = p->next;
			}
			if (!flag) // 删除失败
				MessageBox(NULL, _T("修改失败！未查询到相关信息"), _T("信息修改"), MB_ICONSTOP | MB_SETFOREGROUND);
			break;
		}
		else
		{
			MessageBox(NULL, _T("输入错误！请重新输入。"), _T("信息修改"), MB_ICONSTOP | MB_SETFOREGROUND);
			Modify_Information(Linklist);
		}
	}
}

void sort_information(Stud_list& Linklist,int stud_nums) // 链表和总人数
{
	// 按照总分排序，英语排序，语文排序，数学排序
	wchar_t ways[MAX_LEN];
	wchar_t way1[MAX_LEN];
	wcscpy_s(way1, L"总分");
	wchar_t way2[MAX_LEN];
	wcscpy_s(way2, L"语文");
	wchar_t way3[MAX_LEN];
	wcscpy_s(way3, L"数学");
	wchar_t way4[MAX_LEN];
	wcscpy_s(way4, L"英语");

	while (true)
	{
		InputBox(ways, MAX_LEN, L"通过何种方式排序？（总分/语文/数学/英语）");

		if (wcscmp(ways, way1) == 0) // 总分 // 可以代表平均分了~
		{
			for (int i = 1; i <= stud_nums; i++) // 加一个全局变量，记录学生总数
			{
				Stud_list p = Linklist;

				while (p->next->next)
				{
					int next_num = _wtoi(p->next->sum_score);
					int next_next_num = _wtoi(p->next->next->sum_score);
					if (next_num < next_next_num) // 小的往后放
					{
						Stud_list r1 = p->next;
						Stud_list r2 = p->next->next;
						r1->next = r2->next;
						r2->next = r1;
						p->next = r2;
					}

					p = p->next;
				}
			}

			break;
		}
		else if (wcscmp(ways, way2) == 0) // 语文
		{
			for (int i = 1; i <= stud_nums; i++) // 加一个全局变量，记录学生总数
			{
				Stud_list p = Linklist;

				while (p->next->next)
				{
					int next_num = _wtoi(p->next->Chinese);
					int next_next_num = _wtoi(p->next->next->Chinese);
					if (next_num < next_next_num) // 小的往后放
					{
						Stud_list r1 = p->next;
						Stud_list r2 = p->next->next;
						r1->next = r2->next;
						r2->next = r1;
						p->next = r2;
					}

					p = p->next;
				}
			}

			break;
		}
		else if (wcscmp(ways, way3) == 0) // 数学
		{
			for (int i = 1; i <= stud_nums; i++) // 加一个全局变量，记录学生总数
			{
				Stud_list p = Linklist;

				while (p->next->next)
				{
					int next_num = _wtoi(p->next->Math);
					int next_next_num = _wtoi(p->next->next->Math);
					if (next_num < next_next_num) // 小的往后放
					{
						Stud_list r1 = p->next;
						Stud_list r2 = p->next->next;
						r1->next = r2->next;
						r2->next = r1;
						p->next = r2;
					}

					p = p->next;
				}
			}

			break;
		}
		else if (wcscmp(ways, way4) == 0) // 英语
		{
			for (int i = 1; i <= stud_nums; i++) // 加一个全局变量，记录学生总数
			{
				Stud_list p = Linklist;

				while (p->next->next)
				{
					int next_num = _wtoi(p->next->English);
					int next_next_num = _wtoi(p->next->next->English);
					if (next_num < next_next_num) // 小的往后放
					{
						Stud_list r1 = p->next;
						Stud_list r2 = p->next->next;
						r1->next = r2->next;
						r2->next = r1;
						p->next = r2;
					}
					p = p->next;
				}
			}

			break;
		}
		else // 输入错误，重新输入
		{
			MessageBox(NULL, _T("输入错误！请重新输入。"), _T("成绩排序"), MB_ICONSTOP | MB_SETFOREGROUND);
			Modify_Information(Linklist);
		}

	}

}

// 查询失败返回NULL 查询成功返回相关指针
Stud_list inquery_information(Stud_list linklist) // 信息查询
{
	bool flag = false; // 用于判断是否查询成功
	Stud_list p = Linklist->next; // 指向首元结点的辅助指针
	// 判断是通过姓名还是通过学号查询 // 输入错误要重新询问
	wchar_t ways[MAX_LEN];
	wchar_t way1[MAX_LEN];
	wcscpy_s(way1, L"学号");
	wchar_t way2[MAX_LEN];
	wcscpy_s(way2, L"姓名");

	while (true)
	{
		InputBox(ways, MAX_LEN, L"通过何种方式查询？（学号/姓名）");

		if (wcscmp(ways, way1) == 0) // 通过学号查询
		{
			wchar_t nums[MAX_LEN];
			InputBox(nums, MAX_LEN, L"请输入要查询的学生学号：");
			while(p)
			{
				if (wcscmp(nums, p->num) == 0) // 查询成功
				{
					return p;
				}
				p = p->next;
			}

			MessageBox(NULL, _T("查询失败！未查询到相关信息。"), _T("信息查询"), MB_ICONSTOP | MB_SETFOREGROUND);

			return NULL;
		}

		else if (wcscmp(ways, way2) == 0) // 通过姓名查询
		{
			wchar_t names[MAX_LEN];
			InputBox(names, MAX_LEN, L"请输入要查询的学生姓名：");
			while (p)
			{
				if (wcscmp(names, p->name) == 0) // 查询成功
				{
					return p;
				}
				p = p->next;
			}

			MessageBox(NULL, _T("查询失败！未查询到相关信息。"), _T("信息查询"), MB_ICONSTOP | MB_SETFOREGROUND);

			return NULL;
		}

		else
		{
			MessageBox(NULL, _T("输入错误！请重新输入"), _T("信息查询"), MB_ICONSTOP | MB_SETFOREGROUND);
			Select_MainMenu();
		}
	}
} 

// 结合上一条使用 有返回功能
void inquery_information_work(Stud_list Linklist)
{
	loadimage(&information22, L"information22.jpg");
	putimage(0, 0, &information22);

	// 在视图中显示成绩总表
	int position_x = 230;//“序号”横坐标
	int position_y = 410;//“序号”纵坐标
	setbkmode(TRANSPARENT);//设置字体背景为透明
	settextcolor(COLORREF(RGB(0, 0, 0)));//设置字体颜色为黑色
	settextstyle(20, 0, _T("楷体"));//设置字体大小20，格式楷体
	outtextxy(position_x, position_y, _T("学号      姓名      语文成绩      数学成绩      英语成绩      总分      平均分"));//显示表格第一行表头
	 //打印成绩表
	Stud_list p = Linklist;
		position_y += 20; // 换行
		outtextxy(230, position_y, p->num);//显示学号
		outtextxy(330, position_y, p->name);//显示姓名
		outtextxy(450, position_y, p->Chinese);//语文成绩
		outtextxy(590, position_y, p->Math);//数学成绩
		outtextxy(730, position_y, p->English);//英语成绩
		outtextxy(850, position_y, p->sum_score);//总分
		outtextxy(960, position_y, p->average_score);//平均分

	MOUSEMSG msg;//定义变量，保存鼠标消息
	FlushMouseMsgBuffer();

	while (true) // 返回主菜单
	{
		while (MouseHit())
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				if (msg.x > 1129 && msg.x < 1260 && msg.y > 594 && msg.y < 656)
					Select_MainMenu();
			}
		}
	}
}

void say_goodbye()
{
	loadimage(&exit1, L"exit1.jpg");
	putimage(0, 0, &exit1);

	MOUSEMSG msg; 
	FlushMouseMsgBuffer();

	while (true)
	{
		while (MouseHit()) // 监听
		{
			msg = GetMouseMsg();
			if (WM_LBUTTONDOWN == msg.uMsg)
			{
				msg = GetMouseMsg();
				{
					if (msg.x > 435 && msg.x < 845 && msg.y > 365 && msg.y < 450)
					{
						loadimage(&exit2, L"exit2.jpg");
						putimage(0, 0, &exit2);
						Sleep(200);
						exit(0); // 退出程序
					}
				}
			}
		}
	}
}

void output_information(Stud_list Linklist) // 输出链表到本地文件并实现推出功能
{
	FILE* fp = NULL;
	Stud_list p = Linklist->next; // 辅助指针指向开头
	fp = fopen("information_report.txt", "w");

	fprintf(fp, "学号\t姓名\t语文\t数学\t英语\t总分\t平均分\n");
	while (p)
	{
		// 愚蠢的不停转换
		char num_char[1024];
		char name_char[1024];
		char Math_char[1024];
		char English_char[1024];
		char Chinese_char[1024];
		char sum_score_char[1024];
		char average_score_char[1024];
		WchartoChar(name_char, p->name);
		WchartoChar(num_char, p->num);
		WchartoChar(Chinese_char, p->Chinese);
		WchartoChar(Math_char, p->Math);
		WchartoChar(English_char, p->English);
		WchartoChar(average_score_char, p->average_score);
		WchartoChar(sum_score_char, p->sum_score);

		fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t%s\n", num_char, name_char, Chinese_char, Math_char, English_char, sum_score_char, average_score_char);

		p = p->next;
	}
	fclose(fp);

	say_goodbye();
	
}

int main()
{
	initgraph(1280, 720);
	initlinklist(Linklist);
	Welcome();
	//测试
	//wchar_t s[10];
	//Inputbox(s, 10, l"请输入");
	//Modify_Information(Linklist);

	_getch();
	
	return 0;
}
