/********************************************
主要功能：
（1）存放main函数以及类的一系列构造函数与析构函数
（2）实现游戏的运行
（3）放置全局变量
*********************************************/
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include<math.h>
#include <windows.h>
#include"MoovingBalls.h"

//一系列的构造函数的定义
//class BaseBall default constructor
BaseBall::BaseBall(int m_col=0,int m_lin=0,int m_color=0,bool m_exi=0):Col(m_col),Lin(m_lin),Color(m_color),Exist(m_exi)
{
	Father = NULL;
	//起始默认无珠子
}
//class BaseBall copy constructor
BaseBall::BaseBall(const BaseBall& Ball)
{
	
	Exist = Ball.Exist;
	Col = Ball.Col;
	Lin = Ball.Lin;
	Color = Ball.Color;
	Father = Ball.Father;
	F = Ball.F;

}
//class BaseBall destructor
BaseBall::~BaseBall() {};

int &BaseBall::Scolor()
{
	//用于访问BaseBall的数据成员Color
	return Color;
}

//放置全局变量
BaseBall Ball[LONG][LONG];
int score = 0;



inline void Clear(const int c, const int l)
{
	//清除选择珠子时显示的绿色方框
	setlinecolor(RGB(225, 225, 225));
	rectangle(c * BLOCK_SIZE, l * BLOCK_SIZE, (c + 1) * BLOCK_SIZE, (l + 1) * BLOCK_SIZE);
}

/***************************************************************************
函数名称：ReadMouse()
功    能：用于读取玩家鼠标输入
输入参数：鼠标结构体的引用
返 回 值：
说    明：m:用于读取鼠标输入
          c:Ball对象数组的行下标
		  l:Ball对象数组的列下标
		  in_out:判断是起始位置，还是终止位置
		        （1：起始位置，2：终止位置）
***************************************************************************/
void ReadMouse(MOUSEMSG& m, int& c_1, int& l_1,int &c_2,int &l_2)
{
	int c = 10, l = 10;
	while (1) {
#pragma warning(disable : 4996)
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {//按下左键
			Choose(m, c_1, l_1);//标记选定的位置
			if (Ball[c_1][l_1].Exist) {
				//若选中的位置同时满足1）在规定的区域；2）珠子存在
				Clear(c,l);
				c = c_1;
				l = l_1;//记录选取珠子的信息
				continue;
			}
			if (c!=10) {
				c_2 = c_1;
				l_2 = l_1;//终止位置是本次读入的珠子位置
				c_1 = c;
				l_1 = l;//起始位置是上一次读入的珠子位置（由c、l记录）
				break;
			}
		}
	}
}
/***************************************************************************
  函数名称：JudEnd()
  功    能：判断游戏是否结束
  输入参数：
  返 回 值：
  说    明：游戏结束返回true，否则返回false
***************************************************************************/
inline bool JudEnd()
{
	bool end = false;
	int tol = 0;
	for (int i = 0; i < LONG;i++) {
		for (int j = 0; j < LONG;j++) {
			if (Ball[i][j].Exist) {
				tol++;
			}
		}
	}
	if (tol>=80) {
		end = true;
	}
	return end;
}

/***************************************************************************
  函数名称：clearA()
  功    能：将Ball对象数组与A*算法有关的数据成员重新初始化
  输入参数：
  返 回 值：
  说    明：游戏结束返回true，否则返回false
***************************************************************************/
inline void clearA()
{
	for(int i=0;i<LONG;i++)
		for (int j=0;j<LONG;j++)
		{
			Ball[i][j].Father = NULL;
			Ball[i][j].F = 0;
			Ball[i][j].G = 0;
			Ball[i][j].H = 0;
		}
}

int main()
{
	int c_1 = 0, l_1 = 0;//用于存取玩家选择的珠子的位置信息
	int c_2 = 0, l_2 = 0;//用于存取玩家移动位置信息
	int x[3] = { 0 };//用于记录新生成的三个珠子的位置坐标
	int y[3] = { 0 };
	MOUSEMSG m;		// 定义消息变量
	
	Init();//重新为对象数组的数据成员赋初值，并显示7个初始的珠子,仅执行一次
	initgraph(640, 480);// Create a graphics window with a size of 640x480 pixels
	//loadimage(0, _T("begin-1.jpg"));
	while (1) {//游戏循环开始
		Background();//初始化游戏背景
		//读入玩家鼠标输入
		ReadMouse(m, c_1, l_1,c_2,l_2);
		
		BaseBall* start = &Ball[c_1][l_1];
		BaseBall* end = &Ball[c_2][l_2];
		std::list<BaseBall*> path = GetPath(start, end, false);
	
		if (!path.empty()) {//如果玩家移动的路径复合规定(即返回的链表非空)
			ShowPath(path,start);
			Move_to(start, end);//则调用移动函数移动珠子
		}
		else {
			Showtip();
			Sleep(1500);
			clearA();
			continue;//如果路径不符合规定，跳过后面的语句，重新输入
		}
		
		Background();//初始化游戏背景
		Sleep(300);
		if (!Check(c_2, l_2)) {////检查是否有五子连珠的情况，当5颗珠子连在一起时不生成新的珠子
			CreatBall(x,y);//生成新珠子
			Background();//初始化游戏背景
			for (int i = 0; i < 3; i++) {
				Check(x[i], y[i], 0);//检查新生成的珠子是否被自动消去
			}
			Sleep(1000);
			Background();//初始化游戏背景
		}
		
		clearA();
		if (JudEnd()) {
			EndBack();
			break;
		}
	}
	_getch();
	closegraph();
	return 0;
}