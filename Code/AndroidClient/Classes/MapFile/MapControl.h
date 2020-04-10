#pragma once

#include "MapOthers.h"
#include "GlobalDefine.h"
#include "SkinConfig.h"
#include "MapConfig.h"
#include "MapObjManagers.h"



/*---------------------------------------------------------------------
用于控制界面显示内容
数据跟界面表现的交互通道
--------------------------------------------------------------------*/

enum
{
	OBJ_ACTION_STAND,
	OBJ_ACTION_RUN,
	OBJ_ACTION_ATK1,
	OBJ_ACTION_ATK2,
	OBJ_ACTION_HURT,
	OBJ_ACTION_HSTAND,
	OBJ_ACTION_HRUN,

	OBJ_ACTION_DEAD,
	OBJ_ACTION_MAX,
};

//角色移动由外部数据层控制
class MapTileData;
class MObjActCtrl;
class ToopTip9Spr;
class MapSprObj
{
public:
	MapSprObj();
	~MapSprObj();

	void	InitSkin(int nSkin);
	void	SetSkinPA(int nSkinA);
	void	SetSkinPB(int nSkinB);

	int		GetTitleH();
	void*	pMObj;

	ObjSkinInfo*	pSkin;		//皮肤

	ObjSkinInfo*	pSkinPA;		//武器
	ObjSkinInfo*	pSkinPB;		//坐骑

	BYTE		byType;
	BYTE		byAction;	//当前动作			实时更新
	BYTE		byDirect;	//方向,				实时更新
	int			nPosX,nPosY;	//当前位置		实时更新
	BYTE		byFrame;	//当前第几帧		实时更新

	int			nHp, nMaxHp;
	char*		szName;			//直接指向MapObj的名字地址
	char*		szTitle;
	char*		szMarryTitle;
	BYTE*		pCountry;
	BYTE*		pVipLevel;
	xnList*		pListEffAdd;	//直接指向MapObj的列表地址
	xnList*		pListEffDel;
	xnList*		pListFightNum;

	//BYTE		byDelState;		//1外部删除, 2内部已同步,可释放指针	
	ToopTip9Spr*	pTalkInfo;

	DWORD		dwFrmTick;
	BYTE		byUpdateFlag;
	BYTE		byIsEnemy;

	//DWORD		m_dwParticleID;
	//void*		pSprView;
};

class MapEffObj
{
public:
	int			nEffID;
	int			nPosX,nPosY;
};

//收集并保存地图层绘制所需要的元素, 再地图层UPDATE里更新
class	MapControl;
extern	MapControl*		g_pMapCenter;
class MapControl	: public PacketListener
{
public:
	MapControl();
	~MapControl();
	static	MapControl*	GetDefault(){return g_pMapCenter;}


	virtual bool PacketProcess(ZPacket* pPacket);
	//进入一副地图,需要初始化很多东西
	bool	EnterMap(int nIndex,int nBase);
	void	SetWeather(BYTE bVal);
	int		m_nWeatherID;
	int		m_nEffID;
	int		m_nWeatherSound;
	void	Update();

	int		m_nMapIndex;		//地图ID
	int		m_nBaseMapIndex;	//副本时采用的模板地图ID
	int		m_nX, m_nY;		//视角的左下角坐标位置

	DWORD	m_dwBusyWordTick;	//开始优化的时间
	DWORD	m_dwFrmBusyTime;	//卡的帧数

	int		GetMapWidth()	{return m_pMapTile?m_pMapTile->m_nTWidth:0;}
	int		GetMapHeight()	{return m_pMapTile?m_pMapTile->m_nTHeight:0;}
	bool	LinkMapObj(MapAIObj* pObj);
	bool	DeLinkMapObj(MapObj* pObj);

	void	AddMapEff(int nEffID, int nPosX, int nPosY, DWORD	dwTick);
	MapAIObj*	GetMapObj(BYTE byType, DWORD dwID);
	bool		RemoveMapObj(BYTE byType, DWORD dwID);

	void	AskBornEff(MapAIObj* pObj);

	//地图点击检测选中目标
	void	SearchNearObj(BYTE bEnemy,DWORD	dwMaxLen = 100000, DWORD dwQusEnemyID = 0);
	bool	CheckSelObj(int nX, int nY);

	bool	CheckDispHitNum(MapAIObj* pA, MapAIObj* pB);

	void	PlayFuryEff(int nX, int nY);
	DWORD	PlayerParticle(int nX, int nY,DWORD	dwRunTick, BYTE bType);
	void	SetParticle(int nID,int nX, int nY );

	bool	CheckLineBlock(int nX, int nY, int nEX, int nEY, bool bAtkTarget=false);

	void	AddCharNAtkEff(MapAIObj* pObjA,MapAIObj* pObjB);
	
	MapTileData*	m_pMapTile;			//地表对象
	MapBarrier*		m_pMapBarrier;		//障碍模块
	MapBarrier*		m_pMapAlpha;
	MapFindPath*	m_pMapFindPath;		//寻路模块

	MObjActCtrl*	m_pMapActCtrl;
	
	xnList*		m_pListTile;
	xnList*		m_pListMObj;
	xnList*		m_pListEff;		//地图正在播放的光效, 玩家的技能光效(主要范围技能和飞行技能)

	xnList*		m_pLRunEffView;	//已播放的地图光效
	xnList*		m_pLRunEffOut;

	xnList*		m_pListMapTP;
	xnList*		m_pListMapEffStatic;
	xnList*		m_pLMapTmpEffs;		//地图等待播放的光效

	MapAIObjManager*	m_pMEnemyList;
	MapAIObjManager*	m_pMSlaveList;
	MapAIObjManager*	m_pMNpcList;
	MapPlayerManager*	m_pMPlayerList;
	
	MapAIObj*			m_pMObjLockView;	//控制目标
	DWORD				m_dwLockID;
	BYTE				m_byLockType;
	BYTE				m_bySpEff;

	MapSprObj*			m_pMObjSel;			//选中目标

	DWORD				m_dwTickEnter;
private:
	void				_checkEffInView(int nPosX, int nPosY,int nEX,int nEY,xnList* pList);
};
