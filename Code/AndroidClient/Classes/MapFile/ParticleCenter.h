#pragma once

#include "GlobalDefine.h"
#include "xnlist.h"

class ParticleObj
{
public:
	ParticleObj();
	~ParticleObj();
	int		nID;
	DWORD	dwStartTick;
	DWORD	dwRunTick;
	int		nPosX,nPosY;
	BYTE	bActionType;
	DWORD	dwPosTick;
	char*	szFile;
	void*	pParam;
	bool	bDel;
	
};

class ParticleCenter
{
public:
	ParticleCenter();
	~ParticleCenter();

	int			RunParticle(char* szFile,int nPosX, int nPosY, DWORD dwTick,BYTE bType= 0);
	void		StopParticle(int nID);
	void		SetParticlePos(int nID,int nPosX, int nPosY);
	void		ResetParticle();

	void		Update();
	void		Clear();

	xnList*		m_pList;

	DWORD		m_dwMaxID;
	//int			m_nMapSizeH;
};

extern	ParticleCenter*	pParticleCenter;