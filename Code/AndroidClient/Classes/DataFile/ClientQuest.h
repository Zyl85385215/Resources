#pragma once

#include "GlobalDefine.h"
#include "QuestConfig.h"
#include <map>

class ClientQuest
{
public:
	WORD		wQID;
	QuestObj*	pQBase;
	WORD*		pszVal;
	bool		bOk;
	bool		bChged;
};

class ClientQuestCenter
{
public:
	ClientQuestCenter();
	~ClientQuestCenter();


	void		ReCheckCanAccept();
	ClientQuest*	GetCurQuest(WORD wQID);
	ClientQuest*	GetCurQuestByLine(WORD wLine);
	bool		CheckQuestByLine(WORD wLine,xnList* pList);

	void		RecvQuestInfo(WORD	wQId, WORD* pszwVal);
	void		RecvDeleteQuest(WORD wQId,bool bComplete);
	//返回0表示可接
	int			CheckQuestCanAccept(QuestObj* pQBase);

	void		Update();
	void		UpdateSec();

	void		ClearQuest();

	xnList*		m_pListQuest;		//已接任务
	xnList*		m_pListCanAccpet;	//可接任务

	bool		m_bUpdateQuestChg;
};