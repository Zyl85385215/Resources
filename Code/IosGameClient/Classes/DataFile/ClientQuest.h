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
	//����0��ʾ�ɽ�
	int			CheckQuestCanAccept(QuestObj* pQBase);

	void		Update();
	void		UpdateSec();

	void		ClearQuest();

	xnList*		m_pListQuest;		//�ѽ�����
	xnList*		m_pListCanAccpet;	//�ɽ�����

	bool		m_bUpdateQuestChg;
};