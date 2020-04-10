#pragma once


#include "Interface.h"

class MCAwardMenu
{
public:
	MCAwardMenu();
	~MCAwardMenu();

	void		Init(bool isGDream);
	void		Update();
	void		RequestOpenCard(int nPos);

	void		OpenForm(WORD	wMCopyID,BYTE byRankVal);
	void		SetCardItem(DWORD wItem);
	void		SetCardItems(DWORD wItems[],int nCnt);

	DForm*		m_pForm;

	DComponent*	m_ppCompCard[8];
	DComponent*	m_ppCompRtl[8];

	DComponent* m_ppCompIco[8];
	DComponent* m_ppCompName[8];

	int			m_nRunActionPos;
	DWORD		m_dwChgSpr;
	bool		m_bRecvItem;
	int			m_nOpenCardCnt;

	DComponent*	m_pCompTime;
	DWORD		m_dwOpenTick;
	DWORD		m_dwTickFormOpen;

	int         m_nOpenAllNeed;
	int         m_nOpenAllCnt;
	bool		m_bifOpen[8];
	int 		GetNextNeedRmb();
	void 		GetAllOpenParam(int* nRmb, int* nCnt);
	void        RequestOpenAllCard();

	xnList*     m_pList;
	bool		m_bIsGreenDream;
	bool		m_bDealyLeave;

	DWORD		m_dwCloseGMWTick;
};