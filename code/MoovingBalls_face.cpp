/********************************************
��Ҫ���ܣ��������Ϸ������صĺ���
��1��������ʼʱ�����С����Ρ�����
��2��չʾ��Ϸҳ��
��3����ʾ�������µġ���Ϸ����
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
  �������ƣ�Init()
  ��    �ܣ�������ʼʱ�������ϵ����С����Ρ��������Լ�7����ʼ������
  ���������
  �� �� ֵ��
  ˵    �����ú������������������н�ִ��һ��
***************************************************************************/
bool operate = true;//����һ��ȫ�ֱ���
void Init()
{
	if (operate)
	{
		srand((unsigned)time(NULL));
		operate = false;
	}
	//����Ϊ������������ݳ�Ա����ֵ
	for (int i = 0; i < LONG; i++) {
		for (int j = 0; j < LONG; j++) {
			Ball[i][j].Col = i;
			Ball[i][j].Lin = j;
			Ball[i][j].Color = rand() % 6;
			//����Ϊ��i,j����ɫ���
		}
	}
	int total = 0;//����һ��������ʾ�����ɵ����ӵĸ���
	int c = 0, l = 0;
	while (1) {
		if (total == 7)
			break;
		c = rand() % 9;
		l = rand() % 9;
		if (Ball[c][l].Exist != 1) {//���ѡ����λ����û������
			Ball[c][l].Exist = 1;//����������
			total += 1;//�����ɵ����Ӹ���+1
		}
	}
}

/***************************************************************************
  �������ƣ�BaseBall::Show()
  ��    �ܣ���exe�ļ�����ʾ����
  ���������
  �� �� ֵ��
  ˵    ����class BaseBall�ĳ�Ա����,������״̬Ϊ�����ڡ�ʱ������Ӧλ����ʾ����
***************************************************************************/
void BaseBall::Show()
{
	if (Exist != 0) {//Exist!=0˵������������
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
  �������ƣ�Background()
  ��    �ܣ���ʼ����Ϸ����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void Background()
{
	cleardevice(); // ����
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
	}//������Ϸ����ĺ���
	for (int j = 0; j <= LONG; j++) {
		line(j * BLOCK_SIZE,0,j*BLOCK_SIZE,LONG*BLOCK_SIZE);
	}//������Ϸ���������

	//_getch();
	Show();
}

/***************************************************************************
  �������ƣ�Show()
  ��    �ܣ�����ÿ�������ϵ����ӣ�����ʾ�����ڡ�������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void Show()
{
	BeginBatchDraw();//��ʼ��������
	for (int i = 0; i < LONG; i++) {
		for (int j = 0; j < LONG; j++)
		{
			Ball[i][j].Show();
		}
	}
	EndBatchDraw();//�������Ļ
}

/***************************************************************************
  �������ƣ�EndBack()
  ��    �ܣ���ʾ��Ϸ����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void EndBack()
{
	settextcolor(WHITE);
	TCHAR str[] = _T("Game is Over!!!");
	outtextxy(500, 80, str);
	TCHAR s[] = _T("���������������");
	outtextxy(500, 110, s);
}

/***************************************************************************
  �������ƣ�ShowPath()
  ��    �ܣ���ʾ���ӵ��ƶ�·��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void ShowPath(std::list<BaseBall*> path,BaseBall *start)
{
	// ��ӡ���
	
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
  �������ƣ�Showtip()
  ��    �ܣ�������ƶ�·����������Ϸ����ʱ������Ӧ����ʾ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void Showtip()
{
	settextcolor(WHITE);
	TCHAR str[] = _T("The path is unavailable!");
	outtextxy(470, 80, str);
}