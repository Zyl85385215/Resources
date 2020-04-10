#pragma once
#include "CtrlObj.h"
#include "xnlist.h"


class MarryListMenu	:	public TabMenuObj
{
public:
	MarryListMenu();
	~MarryListMenu();
	void	Open(int nT);
	void	Close();
	void	Update();

	int		nPos;
};

class MarryInfoMenu	:	public TabMenuObj
{
public:
	MarryInfoMenu();
	~MarryInfoMenu();
	
	void	Update();
};

class MarrySkillMenu	:	public TabMenuObj
{
public:
	MarrySkillMenu();
	~MarrySkillMenu();

	void	Open(int nT);
	void	Update();
	int		m_nPos;
};

class MarryQuestMenu	:	public TabMenuObj
{
public:
	MarryQuestMenu();
	~MarryQuestMenu();

	void	Update();
	bool	m_bUpdate;

	void	Open(int nT);

	void	UpViewQuestComp(DForm* pForm, QuestObj* pQBase, ClientQuest* pClientQ);
};

class MarryAskMenu
{
public:
	MarryAskMenu();
	~MarryAskMenu();
	void	PopMsgbox(char* szTitle, char* szMsg,void (*func)(void * pParam,int nVal) = NULL,void* param = NULL,int nType = 0);
	void	ClickAnswer(bool bOk);

	DForm	*	m_pForm;

	void (*Func_CBMsgBox)(void * pParam,int nVal);
	void	*	pParamMsgbox;
private:
	DComponent*	m_pCompTitle;
	DComponent*	m_pCompMsg;
	DComponent*	m_pCompOk;
	DComponent*	m_pCompCancel;
	DComponent*	m_pCompAccept;
};

class SendRingMenu
{
public:
	SendRingMenu();
	~SendRingMenu();
	void	Open();

	void	Update();

	DForm*	m_pForm;
	int		m_nPos;
	CCtrlRadio* m_pRadio;
};

class MarryNpcMenu
{
public:
	MarryNpcMenu();

	void	Open();
	void	OpenChoose();
	void	OpenAward();
	void	Update();
	void	Close();

	DForm*	m_pForm;
	DForm*	m_pChooseForm;
	DForm*	m_pAwardForm;
	CCtrlRadio* m_pRadio;
};