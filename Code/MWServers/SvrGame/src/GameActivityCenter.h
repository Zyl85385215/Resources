#pragma once


#include "xnlist.h"
#include <Windows.h>
#include "GameActivityCfg.h"
#include <map>


class UserData;
class BossInfo;
class BossState;

class GameActivityCenter
{
public:
	GameActivityCenter();
	~GameActivityCenter();
	void UpdateSec();
	void CheckDaylyList();
	void RequestForInfo(UserData* pUser,int nFlag);
	void RequestEnter(UserData* pUser,int nID);
	void EnterActivity(UserData* pUser,ActivityObj* pObj);
	void FreeActivityMap(int nID,int nMIndex);

	xnList* m_pTodayList;
	xnList* m_pOtherList;
	int m_nShowFlag;
	std::map<int,xnList*> mapActivityMCopy;

	//转盘
	void RequestTurntableStart(UserData* pUser);
	void RequestTurntableStartForTen(UserData* pUser);
	void RequestTurntableGift(UserData* pUser,int nPos);

	//活跃度
	void ResetActiveDegree(UserData* pUser);
	void ResetActiveDegree(UserData* pUser, BYTE byPos);
	void SaveActiveDegree(UserData* pUser);
	void LoadActiveDegree(UserData* pUser,BYTE* byData,int nSize);
	void SendActiveDegree(UserData* pUser,int nPos);
	void AddDegreeVal(UserData* pUser,BYTE byPos,int nVal);
	int	GetDegreeRlt(UserData* pUser);
	bool RequestDegreeAward(UserData* pUser);

	//世界boss
	void RequestBossState(UserData* pUser);

	//登入活动推送
	void JoinSendActive(UserData* pUser);

	//活动到点提醒
	void SendWarnTipActive(ActivityObj* pObj);
	void FilterSendListByAct(xnList* pList,ActivityObj* pObj);
	xnList* m_pRubbishList;

	//GM设置活动偏移时间
	int     m_nOffsetTime;

	BYTE		m_bActivityTime[24];
	void		SetActivyTime(ActivityObj* pActivityObj,int nHour);

	//是否在活动时间内
	bool  CheckBeingActive(int nActID);
	bool  CheckActiveEnd(int nActID);

	bool  m_bKillActBoss;

	tm	  m_tmTmp;
};