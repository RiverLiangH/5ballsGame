/********************************************
��Ҫ���ܣ�
��1�����main�����Լ����һϵ�й��캯������������
��2��ʵ����Ϸ������
��3������ȫ�ֱ���
*********************************************/
#include<iostream>
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include<math.h>
#include <windows.h>
#include"MoovingBalls.h"

//һϵ�еĹ��캯���Ķ���
//class BaseBall default constructor
BaseBall::BaseBall(int m_col=0,int m_lin=0,int m_color=0,bool m_exi=0):Col(m_col),Lin(m_lin),Color(m_color),Exist(m_exi)
{
	Father = NULL;
	//��ʼĬ��������
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
	//���ڷ���BaseBall�����ݳ�ԱColor
	return Color;
}

//����ȫ�ֱ���
BaseBall Ball[LONG][LONG];
int score = 0;



inline void Clear(const int c, const int l)
{
	//���ѡ������ʱ��ʾ����ɫ����
	setlinecolor(RGB(225, 225, 225));
	rectangle(c * BLOCK_SIZE, l * BLOCK_SIZE, (c + 1) * BLOCK_SIZE, (l + 1) * BLOCK_SIZE);
}

/***************************************************************************
�������ƣ�ReadMouse()
��    �ܣ����ڶ�ȡ����������
������������ṹ�������
�� �� ֵ��
˵    ����m:���ڶ�ȡ�������
          c:Ball������������±�
		  l:Ball������������±�
		  in_out:�ж�����ʼλ�ã�������ֹλ��
		        ��1����ʼλ�ã�2����ֹλ�ã�
***************************************************************************/
void ReadMouse(MOUSEMSG& m, int& c_1, int& l_1,int &c_2,int &l_2)
{
	int c = 10, l = 10;
	while (1) {
#pragma warning(disable : 4996)
		m = GetMouseMsg();
		if (m.uMsg == WM_LBUTTONDOWN) {//�������
			Choose(m, c_1, l_1);//���ѡ����λ��
			if (Ball[c_1][l_1].Exist) {
				//��ѡ�е�λ��ͬʱ����1���ڹ涨������2�����Ӵ���
				Clear(c,l);
				c = c_1;
				l = l_1;//��¼ѡȡ���ӵ���Ϣ
				continue;
			}
			if (c!=10) {
				c_2 = c_1;
				l_2 = l_1;//��ֹλ���Ǳ��ζ��������λ��
				c_1 = c;
				l_1 = l;//��ʼλ������һ�ζ��������λ�ã���c��l��¼��
				break;
			}
		}
	}
}
/***************************************************************************
  �������ƣ�JudEnd()
  ��    �ܣ��ж���Ϸ�Ƿ����
  ���������
  �� �� ֵ��
  ˵    ������Ϸ��������true�����򷵻�false
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
  �������ƣ�clearA()
  ��    �ܣ���Ball����������A*�㷨�йص����ݳ�Ա���³�ʼ��
  ���������
  �� �� ֵ��
  ˵    ������Ϸ��������true�����򷵻�false
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
	int c_1 = 0, l_1 = 0;//���ڴ�ȡ���ѡ������ӵ�λ����Ϣ
	int c_2 = 0, l_2 = 0;//���ڴ�ȡ����ƶ�λ����Ϣ
	int x[3] = { 0 };//���ڼ�¼�����ɵ��������ӵ�λ������
	int y[3] = { 0 };
	MOUSEMSG m;		// ������Ϣ����
	
	Init();//����Ϊ������������ݳ�Ա����ֵ������ʾ7����ʼ������,��ִ��һ��
	initgraph(640, 480);// Create a graphics window with a size of 640x480 pixels
	//loadimage(0, _T("begin-1.jpg"));
	while (1) {//��Ϸѭ����ʼ
		Background();//��ʼ����Ϸ����
		//��������������
		ReadMouse(m, c_1, l_1,c_2,l_2);
		
		BaseBall* start = &Ball[c_1][l_1];
		BaseBall* end = &Ball[c_2][l_2];
		std::list<BaseBall*> path = GetPath(start, end, false);
	
		if (!path.empty()) {//�������ƶ���·�����Ϲ涨(�����ص�����ǿ�)
			ShowPath(path,start);
			Move_to(start, end);//������ƶ������ƶ�����
		}
		else {
			Showtip();
			Sleep(1500);
			clearA();
			continue;//���·�������Ϲ涨�������������䣬��������
		}
		
		Background();//��ʼ����Ϸ����
		Sleep(300);
		if (!Check(c_2, l_2)) {////����Ƿ�������������������5����������һ��ʱ�������µ�����
			CreatBall(x,y);//����������
			Background();//��ʼ����Ϸ����
			for (int i = 0; i < 3; i++) {
				Check(x[i], y[i], 0);//��������ɵ������Ƿ��Զ���ȥ
			}
			Sleep(1000);
			Background();//��ʼ����Ϸ����
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