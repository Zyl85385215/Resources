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

	int nID;                            //活动ID
	int nMBaseIndex;                    //地图ID
	int nWaveCnt;                      //怪物波数
	int nWaiteTick;                //刷怪间隔
	int nScale;                    //刷怪范围
	int nTotalCnt;                 //总共刷怪数量

	int nNowWave;                    //当前波数
	DWORD dwNextTick;                //下一波怪刷新时间

	char* pBeginNotice;           //开始公告
	char* pOverNotice;            //结束公告

	xnList* pEnemyList;             //出生怪物
	xnList* pBornPosList;          //怪物出生位置

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