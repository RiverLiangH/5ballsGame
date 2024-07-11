/********************************************
��Ҫ���ܣ����ʵ�ָ��ֹ��ܵ��߼�����

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
std::list<BaseBall*>Open, Closed;//����������������
/***************************************************************************
  �������ƣ�CreatBall()
  ��    �ܣ�����������
  ���������
  �� �� ֵ��
  ˵    ����class BaseBall����Ԫ������ִ�����ӵ��ƶ�����
***************************************************************************/
bool operated = true;//����ȫ�ֱ���operatedΪtrue
void CreatBall(int *x,int *y)
{
	//ÿ������3��������
	if (operated)
	{
		srand((unsigned)time(NULL));
		operated = false;
	}
	//��ֵ
	int total = 0;//����һ��������ʾ�����ɵ����ӵĸ���
	int c = 0, l = 0;
	while (1) {
		c = rand() % 9;
		l = rand() % 9;
		if (Ball[c][l].Exist != 1) {//���ѡ����λ����û������
			Ball[c][l].Exist = 1;//����������
			*x = c;//�������ָ��ָ���λ�ø�ֵ����¼�����������ӵ�λ����Ϣ
			*y = l;
			x++;//ָ��λ����ǰ�ƶ�һλ
			y++;
			total += 1;//�����ɵ����Ӹ���+1
		}
		if (total == 3)
			break;
	}
}

/***************************************************************************
�������ƣ�Player::Choose()
��    �ܣ����ѡ��Ҫ�ƶ�������
������������ṹ�������
�� �� ֵ��
˵    ��������m:���������Ϣ
              c:�����λ�õ���ת����Ball����������±�
			  l:�����λ�õ���ת����Ball����������±�
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
�������ƣ�Move_to()
��    �ܣ��������ƶ��������ѡ���λ��
���������
�� �� ֵ��
˵    ����start:�ƶ�����ʼBall����ָ��
          end:�ƶ�����ֹBall����
***********************************ָ��****************************************/
void Move_to(BaseBall *start,BaseBall *end)
{
	start->Exist = 0;
	end->Exist = 1;
	end->Scolor() = start->Scolor();
	srand((unsigned)time(NULL));
	start->Scolor() = rand() % 6;
}
//����Gֵ
int calcG(BaseBall* temp_start, BaseBall* point)
{
	int extraG = int((fabs(point->Col - temp_start->Col) + fabs(point->Lin - temp_start->Lin))*10);
	int parentG = point->Father == NULL ? 0 : point->Father->G; //����ǳ�ʼ�ڵ㣬���丸�ڵ��ǿ�
	return parentG + extraG;
}
//����Hֵ
int calcH(BaseBall* point, BaseBall* end)
{
	//�ü򵥵�ŷ����þ������H�����H�ļ����ǹؼ������кܶ��㷨��û�����о�^_^
	return int(sqrt((double)(end->Col - point->Col) * (double)(end->Col - point->Col) + (double)(end->Lin - point->Lin) * (double)(end->Lin - point->Lin)) * 10);
}
//����Fֵ
int calcF(BaseBall* point)
{
	return point->G + point->H;
}

/***************************************************************************
�������ƣ�BaseBall* GetLeastF()
��    �ܣ��ҳ�Fֵ��С�ķ��񣬷��ظ÷����ָ��
���������
�� �� ֵ��
˵    ����
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
�������ƣ�BaseBall::isInList()
��    �ܣ��жϸ÷����Ƿ��Ѿ����б���
���������
�� �� ֵ��
˵    ����
***************************************************************************/
BaseBall* isInList(const std::list<BaseBall*> list, const BaseBall* ball)
{
	//�ж�ĳ���ڵ��Ƿ����б��У����ﲻ�ܱȽ�ָ�룬��Ϊÿ�μ����б����¿��ٵĽڵ㣬ֻ�ܱȽ�����
	for (auto p : list)
		if (p->Col == ball->Col && p->Lin == ball->Lin)
			return p;
	return NULL;
}

/***************************************************************************
�������ƣ�BaseBall* GetLeastF()
��    �ܣ��жϸ÷����Ƿ��ǿɵ��﷽��
���������
�� �� ֵ��
˵    ����
***************************************************************************/
bool isCanreach(const BaseBall* ball, const BaseBall* target, bool isIgnoreCorner)
{
	if (target->Exist
		|| target->Col<0 || target->Col>LONG - 1
		|| target->Lin<0 || target->Lin>LONG - 1
		//|| target->Col == ball->Col && target->Lin == ball->Lin
		|| isInList(Closed, target)) //������뵱ǰ�ڵ��غϡ����ϰ�������ڹر��б��У�����false
		return false;
	else
	{
		return true;
	}
}
/***************************************************************************
�������ƣ�getSurroundBalls()
��    �ܣ��ҵ���ǰ��Χ4�����п���ͨ���ĸ���
���������
�� �� ֵ��
˵    ����������Χ�ɵ��﷽���ָ���б�
          ����˵����*ball�ǵ�ǰ���ҷ����ָ��
***************************************************************************/
std::vector<BaseBall*> getSurroundPoints(const BaseBall* ball, bool isIgnoreCorner)
{
	std::vector<BaseBall*> surroundPoints;
	//��
	if (ball->Col-1>=0&& ball->Col-1<LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col-1][ball->Lin], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col-1][ball->Lin]);
		}
	}
	//��
	if (ball->Col + 1 >= 0 && ball->Col + 1 < LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col+1][ball->Lin], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col+1][ball->Lin]);
		}
	}
	//��
	if (ball->Lin - 1 >= 0 && ball->Lin - 1 < LONG)
	{
		if (isCanreach(ball, &Ball[ball->Col][ball->Lin-1], isIgnoreCorner))
		{
			surroundPoints.push_back(&Ball[ball->Col][ball->Lin-1]);
		}
	}
	//��
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
�������ƣ�findPath()
��    �ܣ�Ѱ����̵��ƶ�·��
���������
�� �� ֵ��
˵    �������ò���˵����
          start:��ʼ������ָ��
		  end:��ֹ������ָ��    
***************************************************************************/

BaseBall* findPath(BaseBall* start, BaseBall* end, bool isIgnoreCorner)
{
	BaseBall *ball = start;
	
	Open.push_back(ball); //�������
	while (!Open.empty())
	{
		BaseBall* curBall = GetLeastF(); //�ҵ�Fֵ��С�ĵ�
		Open.remove(curBall); //�ӿ����б���ɾ��
		Closed.push_back(curBall); //�ŵ��ر��б�
		//1,�ҵ���ǰ��Χ4�����п���ͨ���ĸ���
		auto surroundPoints = getSurroundPoints(curBall, isIgnoreCorner);
		for (auto& target : surroundPoints)
		{
			//2,��ĳһ�����ӣ���������ڿ����б��У����뵽�����б����õ�ǰ��Ϊ�丸�ڵ㣬����F G H
			if (!isInList(Open, target))
			{
				target->Father = curBall;

				target->G = calcG(curBall, target);
				target->H = calcH(target, end);
				target->F = calcF(target);

				Open.push_back(target);
			}
			//3����ĳһ�����ӣ����ڿ����б��У�����Gֵ, �����ԭ���Ĵ�, ��ʲô������, �����������ĸ��ڵ�Ϊ��ǰ��,������G��F
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
				return resPoint; //�����б���Ľڵ�ָ�룬��Ҫ��ԭ�������endpointָ�룬��Ϊ���������
		}
	}
	return NULL;
}

/***************************************************************************
�������ƣ�GetPath()
��    �ܣ������ҵ���·��
���������
�� �� ֵ��
˵    �������ò���˵����
		  start:��ʼ������ָ��
		  end:��ֹ������ָ��
***************************************************************************/

std::list<BaseBall*> GetPath(BaseBall* start, BaseBall* end, bool isIgnoreCorner)
{
	BaseBall* result = findPath(start, end, isIgnoreCorner);
	std::list<BaseBall*> path;

	while (result)//����·�������û�ҵ�·�������ؿ�����
	{
		path.push_front(result);
		result = result->Father;
	}

	// �����ʱ�����б���ֹ�ظ�ִ��GetPath���½���쳣
	Open.clear();
	Closed.clear();
	return path;
}

/***************************************************************************
�������ƣ�Check()
��    �ܣ�����Ƿ�������5�ŵ�����
���������
�� �� ֵ�����������鷵��true����֮����false
˵    ����c:����ƶ�������ֹλ�õ���
          l:����ƶ�׷����ֹλ�õ���
		  ��˵�����ٶ�ԭ������Ϸ����������������һ������ӣ���Ψһ�䶯��
		  ֻ������ƶ����ǿţ�������Ϸ��ַ����ı�󣬳��������������һ��
		  �����ֻ����������ƶ������Ӹ�����
		  jud:�ж��Ƿ����Զ����ɵ����ӣ�Ĭ�ϲ���Ϊ1������Ϊһ�����������
		  ������������Ҳ�����Ĭ�ϲ�����ͷ�ļ�������
***************************************************************************/
bool Check(const int c,const int l,bool jud)
{
	const int color = Ball[c][l].Scolor();//��¼����ƶ������ӵ���ɫ
	bool uch = 0;//��ʼĬ���������������һ������
	int cx = c, ly = l;//��Ϊ��ʱ������¼���ӵ�����������Ϣ
	int total_o[4] = { 0 };//ÿ���������������Ӹ���
	int total_n[4] = { 0 };//ÿ�����򸺷������Ӹ���
	int dir[4] = { 4 };//��¼����5���������ϵķ���
	//1����ֱ����0
	for (cx = c, ly = l; Ball[cx][ly].Exist&&Ball[cx][ly].Scolor() ==color; cx++) {
		total_o[0] += 1;
	}//����forѭ��������ִ��˳����������ʼʱ�ȸ�total+1
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; cx--) {
		total_n[0] += 1;
	}
	//2��ˮƽ����1
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; ly++) {
		total_o[1] += 1;
	}
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; ly--) {
		total_n[1] += 1;
	}
	//3��2,4����б�ԽǷ���2
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color;cx++, ly++) {
		total_o[2] += 1;
	}
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color;cx--, ly--) {
		total_n[2] += 1;
	}
	//4��1,3����б�ԽǷ���3
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; cx++, ly--) {
		total_o[3] += 1;
	}
	for (cx = c, ly = l; Ball[cx][ly].Exist && Ball[cx][ly].Scolor() == color; cx--, ly++) {
		total_n[3] += 1;
	}
	int k = 0;
	int j = 0;
	int total[4] = { 0 };
	//�������������Ӹ����Լ���¼�����ѭ��
	for (; k < 4; k++) {
		total[k] = total_o[k] + total_n[k];//����ÿ�������ϵ����Ӹ���+1
		if (total[k] >= 6) {//�������������һ��5�ţ����ǵ�forѭ����ִ��˳��
			dir[k] = k;//��¼����������ķ���
		}
	}
	for (k = 0; k < 4; k++) {
		//���ж��Ƿ�������������������������ٽ�������Ĳ���
		if (total[k] >= 6) {
			uch = 1;//����5��������������һ������
			if(jud)
				score += 2;//�ȼ����ƶ������ӱ���ķ���
			//���������ѭ��
			for (; j < 4; j++) {//����4������
				if (dir[j] != 4) {
					if (jud) {
						score = score + (total[j] - 2) * 2;//������ҵ÷�
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
�������ƣ�ClearBall()
��    �ܣ���ȥ5����������һ�����
���������
�� �� ֵ��
˵    ������Check()��������ʹ��
          ����˵����������������Check()��������
		  uch:�Ƿ���5����������һ������
		  dir:����5�����ӵķ���
		     ��0����ֱ����
			   1��ˮƽ����
			   2��24���޵�б�ԽǷ���
			   3��13���޵�б�ԽǷ���
		  op:����5�����������������ϵ����Ӹ���+1
		  na:����5���������Ӹ������ϵ����Ӹ���+1
		  c:���ѡ���ƶ�����ֹλ��������
		  l:���ѡ���ƶ�����ֹλ��������
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