#pragma once
#include<list>
#include<vector>

#define BLOCK_SIZE 50//每个小格子的长宽大小
#define LONG 9//高度和宽度均为9个格子


//声明一个类作为基类
//BaseBall表示棋盘上的棋子
class BaseBall
{
public:
	BaseBall(int m_col, int m_lin, int m_color,bool m_exi);
	BaseBall(const BaseBall& Ball);
	~BaseBall();
	//友元函数
	friend void Init();

	//与A*算法有关的友元函数
	friend std::list<BaseBall*> GetPath(BaseBall* start, BaseBall* end, bool isIgnoreCorner);
	friend BaseBall* findPath(BaseBall* start, BaseBall* end, bool isIgnoreCorner);
	friend int calcF(BaseBall* point);
	friend int calcG(BaseBall* temp_start, BaseBall* point);
	friend  int calcH(BaseBall* point, BaseBall* end);
	friend BaseBall* GetLeastF();
	friend BaseBall* isInList(const std::list<BaseBall*> list, const BaseBall* point);
	friend std::vector<BaseBall*> getSurroundPoints(const BaseBall* ball, bool isIgnoreCorner);
	friend bool isCanreach(const BaseBall* ball, const BaseBall* target, bool isIgnoreCorner);
	friend void ShowPath(std::list<BaseBall*> path, BaseBall* start);
	friend void clearA();


	//成员函数
	void Show();
	int &Scolor();

	//共有数据成员
	bool Exist;//该位置上是否有珠子
	BaseBall* Father=NULL;//用于寻路A*算法中标记父节点，初始值为this指针
private:
	//一般参数
	int Col;//珠子位置的行
	int Lin;//珠子位置的列
	int Color;//珠子的颜色
	//用于A*寻路算法的参数
	int G = 0;
	int H = 0;
	int F = 0;//用于记录A*算法中F的值
};//定义一个全局对象数组记录所有游戏数据


//声明全局函数和全局变量
#ifndef _MOOVINGBALLS_H_
#define _MOOVINGBALLS_H_

extern BaseBall Ball[LONG][LONG];//二维对象数组，记录所有的游戏数据
extern int score;//全局变量，记录玩家的得分


#endif

//其他功能函数
void Init();//生成起始时刻棋盘上的所有“隐形”的珠子
void Background();//绘制游戏背景的函数
void CreatBall(int* x, int* y);//生成新珠子
void Show();//遍历所有当前棋盘上的珠子，并显示
void Move_to(BaseBall* start, BaseBall* end);//移动珠子
void Choose(const MOUSEMSG& m,int &c,int &l);//选择要移动的珠子的函数
bool Check(const int c, const int l,bool jud=1);//检查是否有连成5颗的珠子并消去
void ClearBall(int dir, int op, int na, const int c, const int l);//消去5颗以上连在一起的球
void EndBack();//显示游戏结束
void ShowPath(std::list<BaseBall*> path, BaseBall* start);//显示珠子移动的路径
void Showtip();//当移动路径不符合游戏规则时给出提示