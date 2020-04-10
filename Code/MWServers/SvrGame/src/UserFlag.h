#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>
#include <map>

class UFlagObj
{
public:
	int		nId;
	BYTE	bType;				//占位大小255,65535,42Ytime
	BYTE	bResetType;			//0,不重置,  1每天, 2 每周	
	bool	bSyncClient;		//通知客户端, 客户端用map保存
	bool	bSaved;				//是否更改就存数据库
	int		nOffPos;
};

class UFlagConfig	: public CSingleton<UFlagConfig>
{
public:
	UFlagConfig();
	~UFlagConfig();

	void		LoadFile(char* szFile);
	UFlagObj*	GetFlagByID(int nID);
	xnList*		m_pList;
	int			m_nCurOffPos;
};

enum	SHOPLIMITFLAG
{
	SHOPLIMIT_NEWUSERGIFT,		//新手礼包




	SHOPLIMIT_MAX = 32,
};

class UserData;
class UserFlag
{
public:
	UserFlag(UserData* pU);
	~UserFlag();

	void	DBInitFlag(BYTE* pbD, int nSize);		
	void	DBInitSLimit(BYTE* pbD, int nSize);
	void	DBInitQuest(BYTE* pbD, int nSize);
	void	DBInitMark(BYTE* pbD, int nSize);

	int		SetFlag(WORD wID,int nVal);
	int		GetFlag(WORD wID);
	int		AddFlag(WORD wID,int nVal);

	int		SetMarkFlag(WORD wID,int nVal);
	int		GetMarkFlag(WORD wID);
	int		AddMarkFlag(WORD wID);

	int		SetSLimit(BYTE	bSL,int nVal);
	int		GetSLimit(BYTE	bSL);

	int		SetQuestStep(WORD wLine, BYTE bStep);
	int		SetQuestTime(WORD wQId, BYTE bTime);
	int		GetQuestStep(WORD wLine);
	int		GetQuestTime(WORD wQId);

	void	SyncClient();
	void	SyncSLimitClient();
	void	SyncQuestClient();
	void	SyncCopyLmtClient();

	void	CheckResetDaily(bool bWeek = false, bool bSend = true);
private:
	void	_saveDB();
	void	_sendClient(WORD wID, int nVal);
	void	_saveSLDB();
	void	_sendSLClient(BYTE bSL, BYTE byVal);
	void	_saveQuestDB();
	void	_sendQuestDB(BYTE bIsTime,WORD	wLine, BYTE bStep);

	void	_saveMarkDB();
	void	_sendMarkClient(WORD wID, BYTE byVal);

	BYTE*	pbyData;
	BYTE	pBySLimit[SHOPLIMIT_MAX];
	std::map<WORD, BYTE>	mapLineStep;
	std::map<WORD, BYTE>	mapDayTime;
	std::map<WORD, BYTE>	mapMark;

	UserData*	pUser;
};

class UserCoolDown
{
public:
	UserCoolDown(UserData* pU);
	~UserCoolDown();

	void	DBInitCD(BYTE* pbD, int nSize);
	void	LockCD(WORD wPos, DWORD	dwTickSec);		//秒为单位
	DWORD	CheckIsLock(WORD wPos);					//返回剩余秒数

	void	ReSaveDB();
	void	SyncClient();
	void	SyncClient(WORD wPos);

private:
	std::map<WORD,DWORD>	m_mapCD;		//30秒下线保存, 10分钟的实时存
	UserData*	pUser;
};