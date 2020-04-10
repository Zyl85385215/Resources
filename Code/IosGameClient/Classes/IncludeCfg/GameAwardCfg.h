#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct _simpleAward
{
	int nID;
	int nNum;
};

struct OnlineAward
{
	int nTime;
	_simpleAward xAward;
};

struct MounthAward
{
	int nCount;
	_simpleAward xAward;
};

struct LvupAward
{
	int nLv;
	xnList* pList;
};

struct RechargeAward
{
	int nDays;
	xnList* pList;
};

struct LoginAward
{
	int nDays;
	xnList* pList;
};

class GameAwardConfig	: public CSingleton<GameAwardConfig>
{
public:
	GameAwardConfig();
	~GameAwardConfig();
	void	LoadFile(char* pFile);
	OnlineAward* GetOnlineAward(BYTE byFlag);

	xnList* m_pOnlineList;		//在线奖励
	xnList* m_pMounthList;		//每月签到奖励
	xnList* m_pLvupList;		//升级奖励
	xnList* m_pLoginList;		//连续登入奖励
	xnList* m_pRechargeList;	//首充奖励
	xnList* m_pInviteList;	    //好友邀请奖励
	xnList* m_pLoginNewList;	//新累计奖励
};