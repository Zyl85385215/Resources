#pragma once
#include "CtrlObj.h"

#define SHOW_OBJ_CNT 4


class SlaveMenuObj	: public TabMenuObj
{
public:
	SlaveMenuObj(char* pName,BYTE byType);
	int	byCur;
	BYTE byDirection;
	virtual		void	CFmEvent()	{return;}
};

class SlaveMenuCtrl
{
public:
	SlaveMenuCtrl();
	~SlaveMenuCtrl();
	void	AddObjS(SlaveMenuObj* pMenu);
	void	AddObjH(SlaveMenuObj* pMenu);
	void	UpdateSlaveMenu();
	void	UpdateHorseMenu();
	void	Update();
	void	ShowIcons(BYTE byType,DComponent** ppCom,int nCnt,int nPage = 1);			//byType 0³èÎï 1×øÆï
	void	ShowIcon(DComponent* pCom,int nType,BYTE byT);
private:
	xnList*	m_pSFormList;
	xnList* m_pHFormList;
	int		m_nSCur;
	int		m_nHCur;
	BYTE	m_bySDire;
	BYTE	m_byHDire;
	int     m_nHorsePY[4];
};

extern SlaveMenuCtrl* g_pSlaveMenuCtrl;