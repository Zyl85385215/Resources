#pragma once

#include "xnlist.h"

class UserData;
class QuestObj;
class UQuestObj
{
public:
	WORD	wQId;
	WORD*	pszwVal;
	QuestObj*	pQBase;
	DWORD	dwSPID;
	bool	bSaveDB;
};

class UserQuestCenter
{
public:
	UserQuestCenter(UserData* pData);
	~UserQuestCenter();
	
	void	DBInit();
	void	SyncClient();
	void	UpdateMinuteOrLeave();	//杀怪信息只在离线或每分钟保存

	UQuestObj*	GetQuest(WORD wQID);
	UQuestObj*	GetQuestByLine(WORD wLine);
	void	KillEnemy(WORD wEnemyID);
	//void	AddItem(int nID, int nCount);
	bool	RequestAcceptQuest(WORD wQID,bool bForce = false);
	bool	RequestCompleteQuest(WORD wQID);
	bool	CheckCompleteQuest(WORD wQID);
	bool	RequestGiveUpQuest(WORD wQID);

	void	ResetMapObj();
	void	CheckCtrlDead(DWORD	dwSID);

	xnList*		m_pLQuest;
private:
	void	_insertDB(UQuestObj* pQObj);
	void	_saveDB(UQuestObj* pQObj);
	void	_removeDB(WORD wQId);
	void	_sendClient(UQuestObj* pQObj);
	void	_removeClient(WORD wQID,bool bComplete);

	UserData*	m_pUser;

};
