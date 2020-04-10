#pragma once

#include "ZNetPacket.h"
#include "MapBaseInfo.h"
#include "MapPlate.h"
#include "BornCenter.h"
#include "MapAICtrl.h"

class MapManager
{
public:
	MapManager(WORD wMI, MapCfgObj* pMCfg);
	~MapManager();

	void	Init();

	bool	Update();
	void	UpdateSec();
	void	RecvPacket(ZPacket* pPacket);

	void	MObjCallEnemy(MapAIObj* pMaster, WORD wBaseID, WORD wPow);

	void	AIObjEnterMap(MapAIObj* pObj,bool bWild = false);
	void	AIObjLeaveMap(MapAIObj* pObj);

	DWORD	LockEnemyID();

	MapAIObj*	GetMapObj(BYTE bType, DWORD dwID);

	//int			GetPlateRangeS(WORD wMPX,WORD wMPY,WORD wRange,OUT xnList* pList);
	//方形区域
	int			GetPlateRangeB(WORD wMPX,WORD wMPY,WORD wRangeX,WORD wRangY,OUT xnList* pList);
	//棱形区域
	int			GetPlateRangeA(WORD wMPX,WORD wMPY,WORD wRange,OUT xnList* pList);
	int			GetObjRange(WORD wX,WORD wY,WORD wRange,OUT xnList* pList);	//半径范围
	int			GetObjLine(WORD wX,WORD wY,WORD wTX,WORD wTY,int wRange,OUT xnList* pList);
	//全地图区域
	int			GetPlateAll(OUT xnList* pList);

	bool		CheckLineBlock(int nX, int nY, int nEX, int nEY, bool	bAtkTarget = false);
	bool		GetBlock(int nX, int nY);

	int			GetPlayerCnt();
	void		CalcObjForce(MapAIObj* pObj);

	void		SetWeatherTime(int nw,int nsec);

	DWORD		m_dwMapIndex;

	WORD		m_wMaxPX;
	WORD		m_wMaxPY;
	MapCfgObj*	m_pMCfgObj;			//障碍及地图大小

	MapPlate*	m_szMapPlates;		//地表板块列表
	BornCenter*	m_pBornCenter;
	MapAICenter*	m_pAICenter;
	MRActionCenter*	m_pRbActCenter;
	int			m_nActiveMUser;		//活动玩家数量
	int			m_nActiveCtrl;		//活动控制器数量

	DWORD		m_dwEnemyIndex;

	int			m_nActiveCnt;
	int			m_nUserCnt;
	WORD		m_wExPow;
	BYTE		m_byChgLv;
	BYTE		m_byForceType;

	DWORD		m_dwUpdateSec;
	BYTE		m_byWeather;
	bool		m_bLockAtk;

	//洒花
	DWORD		m_dwTempWeather;
	DWORD		m_dwWeatherSec;	
public:
	//内部函数区域
	void		ObjSyncPos(MapAIObj* pObj);
	void		ObjSyncInfo(MapAIObj* pObj);
	void		ObjKillReward(MapAIObj* pKill, MapAIObj* pDead);
	void		ObjBeHit(MapAIObj* pObj);
	void		LichKingKillBySystReward(MapAIObj* pDead);
};