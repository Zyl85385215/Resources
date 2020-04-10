#include "ParticleCenter.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"

ParticleCenter*	pParticleCenter	= NULL;
ParticleObj::ParticleObj()
{
	szFile	= NULL;
	dwStartTick	= 0;
	pParam		= NULL;
	bActionType	= 0;
}

ParticleObj::~ParticleObj()
{
	if(szFile)
	{
		free(szFile);
		szFile = NULL;
	}
	if(pParam)
	{
		//free(pParam);
		pParam = NULL;
	}
}


ParticleCenter::ParticleCenter()
{
	m_pList		= xnList::Create();

	m_dwMaxID	= 1;
}

ParticleCenter::~ParticleCenter()
{
	Clear();
	m_pList->Free();
}

void ParticleCenter::Update()
{
	if(this == NULL)
		return;
}

int ParticleCenter::RunParticle( char* szFile,int nPosX, int nPosY, DWORD dwTick,BYTE bType )
{
	if(this == NULL)
		return 0;
	ParticleObj*	pParticle	= new ParticleObj();
	pParticle->nID		= m_dwMaxID;
	pParticle->dwStartTick	= 0;
	pParticle->pParam		= NULL;
	pParticle->nPosX	= nPosX;
	pParticle->nPosY	= nPosY;
	pParticle->dwRunTick	= dwTick;
	pParticle->bDel		= false;
	pParticle->szFile	= strdup(szFile);
	pParticle->bActionType	= bType;
	m_pList->Add(pParticle);

	m_dwMaxID++;
	return pParticle->nID;
}

void ParticleCenter::StopParticle( int nID )
{
	if(this == NULL)
		return;
	for (int i = 0; i < m_pList->Count; i++)
	{
		ParticleObj*	pParticle	= (ParticleObj*)m_pList->Items[i];
		if(pParticle->nID == nID)
		{
			pParticle->bDel	= true;
			return;
		}
	}
}

void ParticleCenter::Clear()
{
	if(this == NULL)
		return;
	while(m_pList->Count)
		delete (ParticleObj*)m_pList->Delete(0);
}

void ParticleCenter::ResetParticle()
{
	if(this == NULL)
		return;
	for (int i = 0; i < m_pList->Count; i++)
	{
		ParticleObj*	pParticle	= (ParticleObj*)m_pList->Items[i];
		pParticle->bDel	= true;
	}
}

void ParticleCenter::SetParticlePos( int nID,int nPosX, int nPosY )
{
	if(this == NULL)
		return;
	for (int i = 0; i < m_pList->Count; i++)
	{
		ParticleObj*	pParticle	= (ParticleObj*)m_pList->Items[i];
		if(pParticle->nID == nID)
		{
			pParticle->nPosX	= nPosX;
			pParticle->nPosY	= nPosY;
			pParticle->bActionType	= 5;
			break;
		}
	}
}


