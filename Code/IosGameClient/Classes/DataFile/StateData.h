#pragma once

#include "StateConfig.h"

class StateObj
{
public:
	StateBase*	pSBase;
	BYTE		bLv;
};

class MapAIObj;
class StateCenter
{
public:
	StateCenter(MapAIObj * pObj);
	~StateCenter();

	void	AddState(WORD	wState);
	void	RemoveState(WORD wState);
	void	ClearState();

	bool	CheckState(WORD wState);
	void	Update();

	xnList*		m_pListState;
	MapAIObj *	m_pObj;
	bool		m_bUnVisable;	//ÒþÉí
};