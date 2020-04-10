#pragma once
#include "CtrlObj.h"
#include "xnlist.h"


class MentoringMenu	:	public TabMenuObj
{
public:
	MentoringMenu();
	~MentoringMenu();

	virtual bool CheckCanOpen();
	void	Open(int nT);
	void	Close();
	void	Update();
	void	InitTabEvent(int nCnt);

	void	OpenTip();
	void	CloseTip();

	DForm*	m_pForm2;
	DForm*	m_pForm3;
	int		m_nPos1;
	int		m_nPos2;
	DForm*	m_pTipForm;
	DForm*	m_pNpcForm;
};

class MRequestMenu
{
public:
	MRequestMenu();
	~MRequestMenu();
	void	Open();
	void	Close();
	void	Update();

	DForm*	m_pForm;
	int		m_nPos;
};