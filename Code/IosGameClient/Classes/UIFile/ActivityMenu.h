#pragma once
#include "CtrlObj.h"


class TurntableMenu
{
public:
	TurntableMenu();
	~TurntableMenu();
	void	Update();
	void	ActionStart(BYTE byPos);
	void	Open(int nT);
	void	OpenGet();
	void	CloseGet();

	DForm* m_pForm;
	DForm* m_pForm2;
	DForm* m_pForm3;
	DComponent* m_pPointComp;
	BYTE	m_byPos;

	DForm* m_pForm4;
	DComponent* pComGiftItem[10];
	DComponent* pComGiftName[10];
	bool    m_bShowTen;
	void	OpenGetTen(BYTE byPos[],int nCnt=10);
	void	CloseGetTen();
	void    ReFreshGiftTen();
};

class ACDegreeMenu	: public TabMenuObj
{
public:
	ACDegreeMenu();
	~ACDegreeMenu();
	void	Update();
	void	OpenTip(int nPos);
	void	OpenAwardTip(int nAwardPos);
	void	CloseTip();

	DForm* m_pTipForm;
	int		m_nTipX;
	int		m_nPos;
};


class ActivityMenu	: public TabMenuObj
{
public:
	ActivityMenu();
	~ActivityMenu();
	void	Update();
	void	Open(int nT);

	int		m_nPos;
	void    SetFramStepByActID(int nID);
};

class ACBossMenu : public TabMenuObj
{
public:
	ACBossMenu();
	~ACBossMenu();
	void	Update();
	void	Open(int nT);

	int		m_nPos;
	xnList* m_pStateList;
};

class AppraiseMenu : public TabMenuObj
{
public:
	AppraiseMenu();
	~AppraiseMenu();
	void	Update();
	void	Open(int nT);
};

class NoticeMenu : public TabMenuObj
{
public:
	NoticeMenu();
	~NoticeMenu();

	void	Open(int nT);
	void    InitNotice(char* szNotice);

	std::string	strNotice;
};

class ActivityJoinMenu
{
public:
	ActivityJoinMenu();
	~ActivityJoinMenu();
	void	OpenFrom();
	void	Update();

	int		m_nPos;
	DForm*	m_pForm;
};