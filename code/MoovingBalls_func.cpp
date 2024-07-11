/********************************************
主要功能：存放实现各种功能的逻辑函数

*********************************************/
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include<math.h>
#include <stdlib.h>
#include <time.h>
#include<list>
#include<vector>
#include"MoovingBalls.h"
using namespace std;
std::list<BaseBall*>Open, Closed;//创建两个向量容器
/***************************************************************************
  函数名称：CreatBall()
  功    能：生成新珠子
  输入参数：
  返 回 值：
  说    明：class BaseBall的友元函数，执行珠子的移动操作
***************************************************************************/
bool operated = true;//定义全局变量operated为true
void CreatBall(int *x,int *y)
{
	//每次生成3个新珠子
	if (operated)
	{
		srand((unsigned)time(NULL));
		operated = false;
	}
	//赋值
	int total = 0;//定义一个变量表示新生成的珠子的个数
	int c = 0, l = 0;
	while (1) {
		c = rand() % 9;
		l = rand() % 9;
		if (Ball[c][l].Exist != 1) {//如果选定的位置上没有珠子
			Ball[c][l].Exist = 1;//生成新珠子
			*x = c;//给传入的指针指向的位置赋值，记录下新生成珠子的位置信息
			*y = l;
			x++;//指针位置向前移动一位
			y++;
			total += 1;//新生成的珠子个数+1
		}
		if (total == 3)
			break;
	}
}

/***************************************************************************
函数名称：Player::Choose()
功    能：玩家选择要移动的珠子
输入参数：鼠标结构体的引用
返 回 值：
说    明：参数m:传递鼠标消息
              c:将鼠标位置的行转化成Ball对象数组的下标
			  l:将鼠标位置的列转化成Ball对象数组的下标
***************************************************************************/
void Choose(const MOUSEMSG& m, int& c, int& l)
{
	c = (m.x - m.x % BLOCK_SIZE) / BLOCK_SIZE;
	l = (m.y - m.y % BLOCK_SIZE) / BLOCK_SIZE;
	if (Ball[c][l].Exist) {
		setlinecolor(RGB(0, 255, 0));
		rectangle(c * BLOCK_SIZE, l * BLOCK_SIZE, (c + 1) * BLOCK_SIZE, (l + 1) * BLOCK_SIZE);
	}
}

/***************************************************************************
函数名称：Move_to()
功    能：将珠子移动到玩家新选择的位置
输入参数：
返 回 值：
说    明：start:移动的起始Ball对象指针
          end:移动的终止Ball对象
***********************************指针****************************************/
void Move_to(BaseBall *start,BaseBall *end)
{
	start->Exist = 0;
	end->Exist = 1;
	end->Scolor() = start->Scolor();
	srand((unsigned)time(NULL));
	start->Scolor() = rand() % 6;
}
//计算G值
int calcG(BaseBall* temp_start, BaseBall* point)
{
	int extraG = int((fabs(point->Col - temp_start->Col) + fabs(point->Lin - temp_start->Lin))*10);
	int parentG = point->Father == NULL ? 0 : point->Father->G; //如果是初始节点，则其父节点是空
	return parentG + extraG;
}
//计算H值
int calcH(BaseBall* point, BaseBall* end)
{
	//用简单的欧几里得距离计算H，这个H的计算是关键，还有很多算法，没深入研究^_^
	return int(sqrt((double)(end->Col - point->Col) * (double)(end->Col - point->Col) + (double)(end->Lin - point->Lin) * (double)(end->Lin - point->Lin)) * 10);
}
//计算F值
int calcF(BaseBall* point)
{
	return point->G + point->H;
}

/***************************************************************************
函数名称：BaseBall* GetLeastF()
功    能：找出F值最小的方格，返回该方格的指针
输入参数：
返 回 值：
说    明：
***************************************************************************/
BaseBall* GetLeastF()
{
	if (!Open.empty())
	{
		auto resBall = Open.front();
		for (auto& Ball : Open)
			if (Ball->F < resBall->F)
				resBall = Ball;
		return resBall;
	}
	return NULL;
}

/***************************************************************************
函数名称：BaseBall::isInList()
功    能：判断该方格是否已经在列表中
输入参数：
返 回 值：
说    明：
***************************************************************************/
BaseBall* isInList(const std::list<BaseBall*> list, const BaseBall* ball)
{
	//判断某个节点是否在列表中，这里不能比较指针，因为每次加入列表是新开辟的节点，只能比较坐标
	for (auto p : list)
		if (p->Col == ball->Col && p->Lin == ball->Lin)
			return p;
	return NULL;
}

/***************************************************************************
函数名称：BaseBall* GetLeastF()
功    能：判断该方格是否是可到达方格
输入参数：
返 回 值：
说    明：
***************************************************************************/
bool isCanreach(const BaseBall* ball, const BaseBall* target, bool isIgnoreCorner)
{
	if (target->Exist
		|| target->Col<0 || target->Col>LONG - 1
		|| target->Lin<0 || target->Lin>LONG - 1
		//|| target->Col == ball->Col && target->Lin == ball->Lin
		|| isInList(Closed, target)) //如果点与当前节点重合、是障碍物、或者在关闭列表中，返回false
		return false;
	else
	{
		return true;
	}
}
/***************************************************************************
函数名称：getSurroundBalls()
功    能：找到当前周围4个格中可以通过的格子
输入参数：
返 回 值：
说    明：返回周围可到达方格的指针列表
          参数说明：*ball是当前查找方格的指针
***************************************************************************/
std::vector<BaseBall*> getSurroundPoints(const BaseBall* ball, bool isIgnoreCorner)
{
	std::vector<BaseBall*> surroundPoints;
	//左
	if (ball->Col-1>=0&& ball->Col-1<LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col-1][ball->Lin], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col-1][ball->Lin]);
		}
	}
	//右
	if (ball->Col + 1 >= 0 && ball->Col + 1 < LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col+1][ball->Lin], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col+1][ball->Lin]);
		}
	}
	//上
	if (ball->Lin - 1 >= 0 && ball->Lin - 1 < LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col][ball->Lin-1], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col][ball->Lin-1]);
		}
	}
	//下
	if (ball->Lin + 1 >= 0 && ball->Lin + 1 < LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col][ball->Lin+1], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col][ball->Lin+1]);
		}
	}
	return surroundPoints;
}
/***************************************************************************
函数名称：findPath()
功    能：寻找最短的移动路径
输入参数：
返 回 值：
说    明：调用参数说明：
          start:起始点对象的指针
		  end:终止点对象的指针    
***************************************************************************/

BaseBall* findPath(BaseBall* start, BaseBall* end, bool isIgnoreCorner)
{
	BaseBall *ball = start;
	
	Open.push_back(ball); //置入起点
	while (!Open.empty())
	{
		BaseBall* curBall = GetLeastF(); //找到F值最小的点
		Open.remove(curBall); //从开启列表中删除
		Closed.push_back(curBall); //放到关闭列表
		//1,找到当前周围4个格中可以通过的格子
		auto surroundPoints = getSurroundPoints(curBall, isIgnoreCorner);
		for (auto& target : surroundPoints)
		{
			//2,对某一个格子，如果它不在开启列表中，加入到开启列表，设置当前格为其父节点，计算F G H
			if (!isInList(Open, target))
			{
				target->Father = curBall;

				target->G = calcG(curBall, target);
				target->H = calcH(target, end);
				target->F = calcF(target);

				Open.push_back(target);
			}
			//3，对某一个格子，它在开启列表中，计算G值, 如果比原来的大, 就什么都不做, 否则设置它的父节点为当前点,并更新G和F
			else
			{
				int tempG = calcG(curBall, target);
				if (tempG < target->G)
				{
					target->Father = curBall;
					target->G = tempG;
					target->F = calcF(target);
				}
			}
			BaseBall* resPoint = isInList(Open, end);
			if (resPoint)
				return resPoint; //返回列表里的节点指针，不要用原来传入的endpoint指针，因为发生了深拷贝
		}
	}
	return NULL;
}

/***************************************************************************
函数名称：GetPath()
功    能：返回找到的路径
输入参数：
返 回 值：
说    明：调用参数说明：
		  start:起始点对象的指针
		  end:终止点对象的指针
***************************************************************************/

std::list<BaseBall*> GetPath(BaseBall* start, BaseBall* end, bool isIgnoreCorner)
{
	BaseBall* result = findPath(start, end, isIgnoreCorner);
	std::list<BaseBall*> path;

	while (result)//返回路径，如果没找到路径，返回空链表
	{
		path.push_front(result);
		result = result->Father;
	}

	// 清空临时开闭列表，防止重复执行GetPath导致结果异常
	Open.clear();
	Closed.clear();
	return path;
}

/***************************************************************************
函数名称：Check()
功    能：检查是否有连成5颗的珠子
输入参数：
返 回 值：有五子连珠返回true，反之返回false
说    明：c:玩家移动珠子终止位置的行
          l:玩家移动追踪终止位置的列
		  （说明：假定原来的游戏格局无五颗以上连在一起的珠子，则唯一变动的
		  只有玩家移动的那颗，所以游戏格局发生改变后，出现五颗珠子连在一起
		  情况的只可能是玩家移动的珠子附近）
		  jud:判断是否是自动连成的珠子，默认参数为1（即认为一般情况消掉的
		  珠子是由于玩家操作）默认参数在头文件中声明
***************************************************************************/
bool Check(const int c,const int l,bool jud)
{
	const int color = Ball[c][l].Scolor();//记录玩家移动的珠子的颜色
	bool uch = 0;//起始默认无五颗珠子连在一起的情况
	int cx = c, ly = l;//作为临时变量记录珠子的行列坐标信息
	int total_o[4] = { 0 };//每个方向正方向珠子个数
	int total_n[4] = { 0 };//每个方向负方向珠子个数
	int dir[4] = { 4 };//记录连成5个珠子以上的方向
	//1、竖直方向0
	for (cx = c, ly = l; Ball[cx][ly].Exist&&Ball[cx][ly].Scolor() ==color; cx++) {
		total_o[0] += 1;
	}//由于for循环的语句的执行顺序，总是在起始时先给total+1
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; cx--) {
		total_n[0] += 1;
	}
	//2、水平方向1
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; ly++) {
		total_o[1] += 1;
	}
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; ly--) {
		total_n[1] += 1;
	}
	//3、2,4象限斜对角方向2
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color;cx++, ly++) {
		total_o[2] += 1;
	}
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color;cx--, ly--) {
		total_n[2] += 1;
	}
	//4、1,3象限斜对角方向3
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; cx++, ly--) {
		total_o[3] += 1;
	}
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; cx--, ly++) {
		total_n[3] += 1;
	}
	int k = 0;
	int j = 0;
	int total[4] = { 0 };
	//计算各方向的珠子个数以及记录方向的循环
	for (; k < 4; k++) {
		total[k] = total_o[k] + total_n[k];//计算每个方向上的珠子个数+1
		if (total[k] >= 6) {//加上自身的珠子一共5颗，考虑到for循环的执行顺序
			dir[k] = k;//记录下五子连珠的方向
		}
	}
	for (k = 0; k < 4; k++) {
		//先判断是否有五子连珠的情况，如果有则再进行下面的操作
		if (total[k] >= 6) {
			uch = 1;//出现5颗以上珠子连在一起的情况
			if(jud)
				score += 2;//先加上移动的珠子本身的分数
			//计算分数的循环
			for (; j < 4; j++) {//遍历4个方向
				if (dir[j] != 4) {
					if (jud) {
						score = score + (total[j] - 2) * 2;//计算玩家得分
					}
					ClearBall(dir[j], total_o[j], total_n[j], c, l);
				}
			}
			break;
		}
	}
	return uch;
}

/***************************************************************************
函数名称：ClearBall()
功    能：消去5颗以上连在一起的球
输入参数：
返 回 值：
说    明：与Check()函数配套使用
          参数说明：三个参数均由Check()函数传递
		  uch:是否有5颗珠子连在一起的情况
		  dir:连成5颗珠子的方向
		     （0：竖直方向
			   1：水平方向
			   2：24象限的斜对角方向
			   3：13象限的斜对角方向）
		  op:连成5颗以上珠子正方向上的珠子个数+1
		  na:连成5颗以上珠子负方向上的珠子个数+1
		  c:玩家选择移动的终止位置行坐标
		  l:玩家选择移动的终止位置列坐标
***************************************************************************/
void ClearBall(int dir,int op,int na,const int c,const int l)
{
	srand((unsigned)time(NULL));
	switch (dir) {
		case 0:
			for (int i = 0;i<op;i++) {
				Ball[c + i][l].Exist = 0;
				Ball[c + i][l].Scolor() = rand() % 6;
			}
			for (int i = 0; i < na; i++) {
				Ball[c - i][l].Exist = 0;
				Ball[c - i][l].Scolor() = rand() % 6;
			}
			break;
		case 1:
			for (int i = 0; i < op; i++) {
				Ball[c][l + i].Exist = 0;
				Ball[c][l + i].Scolor() = rand() % 6;
			}
			for (int i = 0; i < na; i++) {
				Ball[c][l - i].Exist = 0;
				Ball[c][l - i].Scolor() = rand() % 6;
			}
			break;
		case 2:
			for (int i = 0; i < op; i++) {
				Ball[c + i][l + i].Exist = 0;
				Ball[c + i][l + i].Scolor() = rand() % 6;
			}
			for (int i = 0; i < na; i++) {
				Ball[c - i][l - i].Exist = 0;
				Ball[c - i][l - i].Scolor() = rand() % 6;
			}
			break;
		case 3:
			for (int i = 0; i < op; i++) {
				Ball[c + i][l - i].Exist = 0;
				Ball[c + i][l - i].Scolor() = rand() % 6;
			}
			for (int i = 0; i < na; i++) {
				Ball[c - i][l + i].Exist = 0;
				Ball[c - i][l + i].Scolor() = rand() % 6;
			}
			break;
		default:
			break;
	}
}