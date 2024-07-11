/********************************************
主要功能：存放与游戏界面相关的函数
（1）生成起始时刻所有“隐形”珠子
（2）展示游戏页面
（3）显示操作后“新的”游戏界面
*********************************************/
#include<iostream>
#include <graphics.h>
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include<math.h>
#include <stdlib.h>
#include <time.h>
#include"MoovingBalls.h"
using namespace std;
/***************************************************************************
  函数名称：Init()
  功    能：生成起始时刻棋盘上的所有“隐形”的珠子以及7个初始的珠子
  输入参数：
  返 回 值：
  说    明：该函数在整个程序运行中仅执行一次
***************************************************************************/
bool operate = true;//定义一个全局变量
void Init()
{
	if (operate)
	{
		srand((unsigned)time(NULL));
		operate = false;
	}
	//重新为对象数组的数据成员赋初值
	for (int i = 0; i < LONG; i++) {
		for (int j = 0; j < LONG; j++) {
			Ball[i][j].Col = i;
			Ball[i][j].Lin = j;
			Ball[i][j].Color = rand() % 6;
			//坐标为（i,j）颜色随机
		}
	}
	int total = 0;//定义一个变量表示新生成的珠子的个数
	int c = 0, l = 0;
	while (1) {
		if (total == 7)
			break;
		c = rand() % 9;
		l = rand() % 9;
		if (Ball[c][l].Exist != 1) {//如果选定的位置上没有珠子
			Ball[c][l].Exist = 1;//生成新珠子
			total += 1;//新生成的珠子个数+1
		}
	}
}

/***************************************************************************
  函数名称：BaseBall::Show()
  功    能：在exe文件上显示珠子
  输入参数：
  返 回 值：
  说    明：class BaseBall的成员函数,当珠子状态为“存在”时，在相应位置显示珠子
***************************************************************************/
void BaseBall::Show()
{
	if (Exist != 0) {//Exist!=0说明珠子已生成
		switch (Color) {
			case 0:
				setfillcolor(RGB(255, 255, 0));
				break;
			case 1:
				setfillcolor(RGB(255, 128, 0));
				break;
			case 2:
				setfillcolor(RGB(0, 255, 255));
				break;
			case 3:
				setfillcolor(RGB(218, 112, 214));
				break;
			case 4:
				setfillcolor(RGB(0, 0, 255));
				break;
			case 5:
				setfillcolor(RGB(255, 0, 0));
				break;
			default:
				break;
		}
		fillcircle(Col * BLOCK_SIZE + 25, Lin * BLOCK_SIZE + 25, 25);
	}
}

/***************************************************************************
  函数名称：Background()
  功    能：初始化游戏背景
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Background()
{
	cleardevice(); // 清屏
	settextcolor(WHITE);
	TCHAR str[] = _T("Your Score:");
	TCHAR s[] = _T("00000");
#pragma warning(disable : 4996)
	_stprintf(s, _T("%5d"), score);

	outtextxy(500, 20, str);
	outtextxy(500, 50, s);

	setlinecolor(WHITE);
	for (int i=0; i <= LONG; i++) {
		line(0,i*BLOCK_SIZE,LONG*BLOCK_SIZE,i*BLOCK_SIZE);
	}//绘制游戏界面的横线
	for (int j = 0; j <= LONG; j++) {
		line(j * BLOCK_SIZE,0,j*BLOCK_SIZE,LONG*BLOCK_SIZE);
	}//绘制游戏界面的竖线

	//_getch();
	Show();
}

/***************************************************************************
  函数名称：Show()
  功    能：遍历每个格子上的珠子，并显示“存在”的珠子
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Show()
{
	BeginBatchDraw();//开始批量绘制
	for (int i = 0; i < LONG; i++) {
		for (int j = 0; j < LONG; j++)
		{
			Ball[i][j].Show();
		}
	}
	EndBatchDraw();//输出到屏幕
}

/***************************************************************************
  函数名称：EndBack()
  功    能：显示游戏结束
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void EndBack()
{
	settextcolor(WHITE);
	TCHAR str[] = _T("Game is Over!!!");
	outtextxy(500, 80, str);
	TCHAR s[] = _T("按任意键结束运行");
	outtextxy(500, 110, s);
}

/***************************************************************************
  函数名称：ShowPath()
  功    能：显示珠子的移动路径
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void ShowPath(std::list<BaseBall*> path,BaseBall *start)
{
	// 打印结果
	
	for (auto& p : path) {
		switch (start->Color) {
			case 0:
				setfillcolor(RGB(255, 255, 0));
				break;
			case 1:
				setfillcolor(RGB(255, 128, 0));
				break;
			case 2:
				setfillcolor(RGB(0, 255, 255));
				break;
			case 3:
				setfillcolor(RGB(218, 112, 214));
				break;
			case 4:
				setfillcolor(RGB(0, 0, 255));
				break;
			case 5:
				setfillcolor(RGB(255, 0, 0));
				break;
			default:
				break;
		}
		fillcircle(p->Col * BLOCK_SIZE + 25, p->Lin * BLOCK_SIZE + 25, 25);
		Sleep(100);
		setlinecolor(RGB(0, 0, 0));
		setfillcolor(RGB(0, 0, 0));
		fillcircle(p->Col * BLOCK_SIZE + 25, p->Lin * BLOCK_SIZE + 25, 25);
		setlinecolor(WHITE);
		rectangle(p->Col * BLOCK_SIZE, p->Lin * BLOCK_SIZE, p->Col * BLOCK_SIZE+50, p->Lin * BLOCK_SIZE+50);
	}
}

/***************************************************************************
  函数名称：Showtip()
  功    能：当玩家移动路径不符合游戏规则时给出相应的提示
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void Showtip()
{
	settextcolor(WHITE);
	TCHAR str[] = _T("The path is unavailable!");
	outtextxy(470, 80, str);
}