#pragma once
#include<list>
#include<vector>

#define BLOCK_SIZE 50//ÿ��С���ӵĳ����С
#define LONG 9//�߶ȺͿ�Ⱦ�Ϊ9������


//����һ������Ϊ����
//BaseBall��ʾ�����ϵ�����
class BaseBall
{
public:
	BaseBall(int m_col, int m_lin, int m_color,bool m_exi);
	BaseBall(const BaseBall& Ball);
	~BaseBall();
	//��Ԫ����
	friend void Init();

	//��A*�㷨�йص���Ԫ����
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


	//��Ա����
	void Show();
	int &Scolor();

	//�������ݳ�Ա
	bool Exist;//��λ�����Ƿ�������
	BaseBall* Father=NULL;//����Ѱ·A*�㷨�б�Ǹ��ڵ㣬��ʼֵΪthisָ��
private:
	//һ�����
	int Col;//����λ�õ���
	int Lin;//����λ�õ���
	int Color;//���ӵ���ɫ
	//����A*Ѱ·�㷨�Ĳ���
	int G = 0;
	int H = 0;
	int F = 0;//���ڼ�¼A*�㷨��F��ֵ
};//����һ��ȫ�ֶ��������¼������Ϸ����


//����ȫ�ֺ�����ȫ�ֱ���
#ifndef _MOOVINGBALLS_H_
#define _MOOVINGBALLS_H_

extern BaseBall Ball[LONG][LONG];//��ά�������飬��¼���е���Ϸ����
extern int score;//ȫ�ֱ�������¼��ҵĵ÷�


#endif

//�������ܺ���
void Init();//������ʼʱ�������ϵ����С����Ρ�������
void Background();//������Ϸ�����ĺ���
void CreatBall(int* x, int* y);//����������
void Show();//�������е�ǰ�����ϵ����ӣ�����ʾ
void Move_to(BaseBall* start, BaseBall* end);//�ƶ�����
void Choose(const MOUSEMSG& m,int &c,int &l);//ѡ��Ҫ�ƶ������ӵĺ���
bool Check(const int c, const int l,bool jud=1);//����Ƿ�������5�ŵ����Ӳ���ȥ
void ClearBall(int dir, int op, int na, const int c, const int l);//��ȥ5����������һ�����
void EndBack();//��ʾ��Ϸ����
void ShowPath(std::list<BaseBall*> path, BaseBall* start);//��ʾ�����ƶ���·��
void Showtip();//���ƶ�·����������Ϸ����ʱ������ʾ