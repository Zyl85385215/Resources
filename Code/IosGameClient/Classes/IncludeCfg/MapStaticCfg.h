#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <map>
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

const char* const szLuaFunctionBeforeJumpMap = "BeforeJumpMap";
const char* const szLuaFunctionOnJumpMap = "PlayerJoin";
const char* const szLuaFunctionBeforeLeaveMap = "BeforeLeaveMap";
const char* const szLuaFunctionOnLeaveMap = "PlayerExit";
const char* const szLuaFunctionInitMap = "MapInit";
const char* const szLuaFunctionDeleteMap = "MapDeInit";
const char* const szLuaFunctionUpdateMap = "MapUpdate";

class _BaseGuidePt
{
public:
	_BaseGuidePt()	{byMark	= 0;}
	virtual ~_BaseGuidePt()	{}
	char	szName[32];
	int		nID;
	int		nMapindex;
	int		nPosX;
	int		nPosY;
	BYTE	byMark;
	BYTE	byType;
};

class PathObj	: public _BaseGuidePt
{
public:
	PathObj();
	virtual ~PathObj();
	
	int		nSkin;
	int		nLv;
	int		nEff;
	int		nShopIndex;

	WORD	wPathQuestID;  //寻路任务ID
	
	bool	bMapTalk;
	xnList*	pListTalk;
};

class NpcPathLeadCfg	: public CSingleton<NpcPathLeadCfg>
{
public:
	NpcPathLeadCfg();
	~NpcPathLeadCfg();

	void			LoadFile(char* szFile);
	PathObj*		GetNpcPath(char* szName, int nMapIndex);
	PathObj*		GetNpcPath(int	nNpcID);
	bool			GetNpcListByMIndex(int nMapIndex, xnList* pOutList);

	void			ResetMark();
private:
	xnList*		m_pList;
};

class MonsterPathLeadCfg	: public CSingleton<MonsterPathLeadCfg>
{
public:
	MonsterPathLeadCfg();
	~MonsterPathLeadCfg();

	void			LoadFile(char* szFile);
	PathObj*		GetPathObj(int	nMonID);
	bool			GetMonListByMIndex(int nMapIndex, xnList* pOutList);
	void			ResetMark();
private:
	xnList*		m_pList;
};

class _TransPoint	: public _BaseGuidePt
{
public:
	//int		nMapindex;	//srcmap
	int		nToMapIndex;	//destmap
	//int		nPosX;		//srcx
	//int		nPosY;		//srcy
	int		nSrcRange;	//srcrange
	int		nDestX;		//destx
	int		nDestY;		//desty
	int		nDestRange;	//destrange
	WORD	wEffID;		//effid
	BYTE	byLvLimit;
	BYTE	byToMapCopy;
};

class TransPointCfg	: public CSingleton<TransPointCfg>
{
public:
	TransPointCfg();
	~TransPointCfg();

	void			LoadFile(char* szFile);
	bool			FindMapPath(int nMIndexS, int nMIndexT, xnList* pList);
	xnList	*		GetMapTP(int nMIndex);
	_TransPoint*	GetTransByID(WORD wMIndex,int nTPID);
	
private:
	bool			_findMapPath(int nMIndexS, int nMIndexT, xnList* pList,int nStep);
	int				m_nLoopCnt;
	xnList*			m_pTmpList;

	std::map<WORD, xnList*>	mapTPMaps;
	
};

class _MapEffectObj
{
public:
	int		nMapX;
	int		nMapY;
	int		nEffectID;
	DWORD	dwTick;		//非0的需要不断触发
	DWORD	dwRunTick;	//运行时TICK
	DWORD	dwEndTick;	//过时消除,用于地图上需要多次发动光效的技能
	void*	pRunEff;
};

class MapEffectCfg	: public CSingleton<MapEffectCfg>
{
public:
	MapEffectCfg();
	~MapEffectCfg();

	void		LoadFile(int nMapIndex);

	xnList*		m_pList;		//可被其他地方引用
};