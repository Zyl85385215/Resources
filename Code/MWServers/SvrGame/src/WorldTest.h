#pragma once
#include "singleton.h"
#include "TestLibraryCfg.h"
#include "TestAwardCfg.h"
#include "DBControl.h"
#include "ios_activity_packet.h"

struct WTestInfo
{
	int nUin;
	int nStar;
	int nPlace;
	TestObj* pObj;
	BYTE szCh[4];		//ֵ1~4
	int nCount;
	int nCorrect;
	int nEndSec;
	char szName[20];
	BYTE byGiftFlag;
};

struct WWaitObj
{
	DWORD dwTick;
	WTestInfo* pInfo;
};

class UserData;
class WorldTestCenter
{
public:
	WorldTestCenter(DBControl*	pDB);
	~WorldTestCenter();
	void		SetState(BYTE byState);
	WTestInfo*	AddNewObj(UserData* pUser);
	void		RandTest(WTestInfo* pObj);
	WTestInfo*	GetObjByUin(DWORD dwUin);
	bool		AnswerQuestion(UserData* pUser,BYTE byCh);	
	void		SendQuestion(UserData* pUser);
	void		SendQuestion(WTestInfo* pObj);
	void		Update25();
	void		Update();
	bool		DBInit();
	void		CheckQuestionTime();
	void		RequestStart(UserData* pUser);
	void		GiveUpTest(UserData* pUser);
	void		SupportAwards(WTestInfo* pObj,UserData* pUser);
	void		SendRankInfo(int nUin);
	void		RequestForGift(UserData* pUser);
	void		AddWaitSend(WTestInfo* pObj);
	BYTE		m_byState;
	bool		m_ifSend;
	bool		m_ifCheckPlace;

private:

	bool		_InsertInfo(WTestInfo* pInfo);
	bool		_UpdateInfo(WTestInfo* pInfo);
	bool		_RemoveInfo(WTestInfo* pInfo);
	bool		_ClearSql();
	DBControl*	m_pDB;

	xnList* m_pList;
	xnList*	m_pWaitList;
};

class TurntableCenter
{
public:
	TurntableCenter(UserData* pUser);
	~TurntableCenter();
	void RefreshFlag();
	void ReSetFlag();
	void Update();
	void Start();
	void StartForTen();
	void RequestForGift(int nPos);

	bool AddFreeTimes();

	DWORD m_dwEndTime;
	int m_nPos[10];
	int m_nCount;
	int m_nFreeTimes;
	int m_nGiftFlag;
	UserData* m_pUser;
};