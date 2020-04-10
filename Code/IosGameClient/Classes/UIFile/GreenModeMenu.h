#pragma once

#include "GlobalDefine.h"
#include "CtrlObj.h"

//FLAG 23ºÅ
class GreenModeMenu
{
public:
	GreenModeMenu();
	~GreenModeMenu();

	void		Update();
	void		OpenStart(int nGLv);
	void		StartMCopy(int nGLv);	//¿ªÊ¼
	void		OverMCopy(int nGLv);	//É¨µ´

	bool        CheckShine();

	DForm*		m_pForm;
	DForm*		m_pFormStart;
	int			m_nReachStep;
	DWORD       m_dwChgTick;
	bool        m_bChg;

	DComponent*	pCompDelay;
};

class GreenAwardMenu
{
public:
	GreenAwardMenu();
	~GreenAwardMenu();

	void		Open();
	void		Close()	{m_pForm->Close();};
	void		MoveXY(int	nMoveY);

	DForm*		m_pForm;
	DComponent*	m_pShowComp;
	int		m_nBaseX;
	int		m_nBaseY;

};