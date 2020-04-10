#pragma once
#include "CtrlObj.h"

class FlappyBirdMenu
{
public:
	FlappyBirdMenu();
	~FlappyBirdMenu();
	void	Open();
	void	OpenForm();
	void	Close();
	void	Update();
	void	ReSetFly();
	void	InitStart();
	void	BeDead();

	void	StartGame();

	void  	ColumnRandChgY(DComponent* pComp1,DComponent* pComp2,int nTeamNum,bool bInit);

	DForm*	m_pForm;
	DForm*	m_pStartForm;
	DForm*		m_pGameOverForm;

	DComponent*	m_pCompBird;
	DComponent*	m_pCompColumn[6];
	DComponent*	m_pCompRoad[9];

	bool    m_bStart;
	bool    m_bDeath;
	bool    m_bInitColumnPos;

	int     m_nBOriginalX;
	int     m_nBOriginalY;
	int     m_nCOriginalW;
	int     m_nCOriginalY;
	int     m_nCOriginalX[6];
	int     m_nROriginalX[9];

	int     m_nChgY;
	int     m_nFlyTimes;
	DWORD   m_dwStarFlyTick;

	int     m_nClapRate;

	int     m_nNowGameScore;
	int     m_nHistoryScore;

	bool    m_bPass[3];
	int     m_nCMiddleWidth[3];

	bool    CheckCrash(DComponent* pBird,DComponent* pColumn,int nMiddleWidth);
	bool	m_bCanChick;
};