#pragma once
#include "xnlist.h"
#include <Windows.h>
#include <map>
#include "ItemConfig.h"

#define MAX_WARRANK_CNT		500

class DBControl;
class UserData;
class WarRankParam
{
public:
	WarRankParam();
	int			nSkin[2];
	int			nSpeed;
	int			nParams[EQUIP_ATTR_MAX];
	WORD		wSkillID[12];
	BYTE		bySkillLv[12];
};


class WarRankUser
{
public:
	int			dwUin;
	char		szName[16];
	BYTE		byLv;
	WORD		wRankPos;
	DWORD		wScore;//战斗力
	BYTE		byJob;
	BYTE		bySex;
	int			nWinCnt;		//连胜场
	WarRankParam		xParam;

	void		AddWinCnt();
	void		ClearWinCnt(char* szWinName);
	void		_SaveDBInfo(bool bNew);
	void		_addUserScore(int nScore);

	UserData*	pFight;
};

class WarRankCenter
{
public:
	WarRankCenter();
	~WarRankCenter();

	void	DBInit(DBControl* pDB);
	void	CfgInit(char*	szFile);	//第一次启动的时候用配置文件或其他方式填充数值

	void	RequestFight(UserData* pUser, WORD wPos);
	void	RequestWRObjs(UserData* pUser);
	void	RequestForAward(UserData* pUser,BYTE byPos);
	void	RequestBuyItem(UserData* pUser,BYTE byType);

	WarRankUser*	GetWRUserByPos(WORD		wPos);
	WarRankUser*	GetWRUserByUin(DWORD	dwUin);

	void	UpdateRank(UserData* pUser);

	void	Update10Mintue();

	std::map<WORD,WarRankUser*>		m_mapWarRank;

	void	OnBattleResult(UserData* pUser,bool bWin);

	void	LockFight(WarRankUser* pData,UserData* pUser);
	void	UnlockFight(WarRankUser* pData);

	void	SendWarRankState(DWORD dwUin,BYTE byState,int nVal);

	void	AddNewRankUser(UserData* pUser);

	void	AddWarRankTimes(UserData* pUser);
	bool	AddWarRankTimesByUse(UserData* pUser);

	void	GMChgWarRankPos(UserData* pUser,int nPos);
private:
	void	_changeEnamyParam(WORD wID,WarRankUser* pData,UserData* pUser);
	void	_changeEnamySkill(WORD wID,WarRankUser* pData,UserData* pUser);
};