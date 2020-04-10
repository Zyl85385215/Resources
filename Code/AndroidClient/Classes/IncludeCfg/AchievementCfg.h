#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct ATileObj{
	char*	pName;
	int		nID;
	int		nGroup;
	int		nCount;
};

struct AchievemetnObj{
	char*	pName;
	int		nID;
	int		nFloor;
	char*	pDesc;
	char*	pIcon;
	int		nPoint;
	int		nCnt;
};

struct _simpleAward;

struct AchieveAward 
{
	char*	pName;
	char*	pDesc;
	BYTE	byType;		//0成就点数，1floor,2group
	int		nValue;
	xnList*	pList;
};

class AchievementCfg	: public CSingleton<AchievementCfg>
{
public:
	AchievementCfg();
	~AchievementCfg();
	void	LoadFile(char* pFile);
	void	LoadAwardFile(char* pFile);
	void	GetObjList(int nFloor,OUT xnList* pList);
	AchievemetnObj*	GetObj(int nID);
	ATileObj*	GetFloor(int nID);
	ATileObj*	GetGroup(int nID);

	xnList*		m_pGroupList;
	xnList*		m_pFloorList;
	xnList*		m_pObjList;

	xnList*		m_pAwardList;
private:
	void	AddFloorCnt(int nID,int nVal);
	void	AddGroupCnt(int nID,int nVal);
};