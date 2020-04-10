#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>

class EnemyPos
{
public:
	int nX;
	int nY;
};

class GMActiveBase
{
public:
	GMActiveBase();
	~GMActiveBase();

	int nID;                            //�ID
	int nMBaseIndex;                    //��ͼID
	int nWaveCnt;                      //���ﲨ��
	int nWaiteTick;                //ˢ�ּ��
	int nScale;                    //ˢ�ַ�Χ
	int nTotalCnt;                 //�ܹ�ˢ������

	int nNowWave;                    //��ǰ����
	DWORD dwNextTick;                //��һ����ˢ��ʱ��

	char* pBeginNotice;           //��ʼ����
	char* pOverNotice;            //��������

	xnList* pEnemyList;             //��������
	xnList* pBornPosList;          //�������λ��

	int   RandBornEnemy();
};


class GMActiveConfig : public CSingleton<GMActiveConfig>
{
public:
	GMActiveConfig();
	~GMActiveConfig();

	void			LoadFile(char*	szFile);
	GMActiveBase*   GetActiveBase(int nID);
private:
	xnList*		m_pList;
};

class GMActiveCenter
{
public:
	GMActiveCenter();
	~GMActiveCenter();

	void    AddActive(int nID,bool bOver = false);
	void    Update();
private:
	xnList* m_pList;
};