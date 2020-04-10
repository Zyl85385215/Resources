#pragma once

#include <Windows.h>
#include "xnList.h"
/*------------------------------------------------------------------------------------
只登记怪物的生死, 数量及刷新时间的维持
对象指针产生和销毁的地方
逻辑部分由外部实现, 指定AI策略
------------------------------------------------------------------------------------*/
class MapManager;
class MapAIObj;
//怪物刷新点信息
class BornPointInfo
{
public:
	BornPointInfo(WORD wMapW,WORD wMapH);
	~BornPointInfo();

	void	GetRandPoint(int& nX, int& nY,MapManager* pParent,BYTE& byStep);
	void	CheckBGroupCmd(MapAIObj* pObj,MapManager* pMap);
	//void	GetBattleGPos(int& nX, int& nY,int nDestX, int nDestY, int nDir, int nPos);
	//基础刷新模块
	WORD	wPosX;		//刷新点
	WORD	wPosY;
	WORD	wActiveNumMin;		//最大人口
	WORD	wActiveNumMax;		//最大人口
	DWORD	dwBornTickMin;		//刷新时间间隔
	DWORD	dwBornTickMax;
	WORD	wEBaseID;		//怪物的攻击特性,索敌距离由基础模板指定
	WORD	wRange;			//刷新区域
	WORD	wExPow;			//百分比强度系数
	BYTE	bMaxPlayer;
	BYTE	bForce;			//势力

	//行为控制模块
	WORD	wRangeRun;		//随机移动距离
	WORD	wRangeAtk;		//最大追击距离
	
	BYTE	bPtRunCnt;		//为0表示固定位置随机走
	bool	bLoopRun;		//循环移动
	POINT*	szRunPts;

	//运行时变量
	xnList*	pListEnemys;	//当前怪数量
	WORD	wActiveNum;		//最大人口
	DWORD	dwBornTick;		//刷新时间间隔
	xnList*	pListDeadTick;	//怪物死亡时间

	WORD	wMaxWidth;
	WORD	wMaxHeight;

	BYTE	byBattleGroup;	//阵型, 0 无  1 单列, 2 两排
};

//刷新控制模块,收集刷新点现有数量
class BornCenter
{
public:
	BornCenter(WORD	wRMIndex,MapManager* pParent);
	~BornCenter();

	void	CheckGSSync();
	void	UpdateSec();

	//返回false表示这个对象已存在
	bool	GSBornMapObj(MapAIObj* pObj);

	MapAIObj*	GetMapObj(BYTE bType, DWORD dwID);
	void	CheckMObjList();

	int		GetPlayerCnt()	{return m_pListGSPlayer->Count;}
private:
	void	_initNpc();		//初始化NPC,加载地图的时候生成
	void	_initBorn(WORD wRMIndex);
	void	_updateBorn();		//检查怪物出现

	MapAIObj*	_findMObj(DWORD dwID, xnList* pList);
	void		_checkMObj(xnList* pList);

	xnList*		m_pListBornPts;		//BornPointInfo
	xnList*		m_pListRobot;			//NPC列表
	xnList*		m_pListGSPlayer;	//游戏服务器增加的对象, 如玩家,宠物,活动怪
	xnList*		m_pListGSSlave;
	xnList*		m_pListGSEnemy;
	xnList*		m_pListCache;
	MapManager*	m_pParent;
	DWORD		m_dwUpdateCnt;
};