#include "stdafx.h"
#include "P2PAskCenter.h"
#include "ios_muli_packet.h"

CreateSingleton(P2pAskCenter);

P2pAskCenter::P2pAskCenter()
{
	InitializeCriticalSection(&m_xPAskLock);
	m_pList		= xnList::Create();
}

P2pAskCenter::~P2pAskCenter()
{
	while(m_pList->Count)
	{
		delete (P2pAskObj*)m_pList->Delete(0);
	}
	m_pList->Free();
	DeleteCriticalSection(&m_xPAskLock);
}

void P2pAskCenter::UpdateCheck()
{
	EnterCriticalSection(&m_xPAskLock);
	for (int i = m_pList->Count - 1; i >= 0; i--)
	{
		P2pAskObj*	pPA	= (P2pAskObj*)m_pList->Items[i];
		if(GetTickCount() - pPA->dwStartTime > 10000)
		{//过期的东西删除
			delete pPA;
			m_pList->Delete(i);
			continue;
		}
		if(pPA->bStep	== PAS_END)
		{// 已完成的删除
			delete pPA;
			m_pList->Delete(i);
			continue;
		}
	}
	LeaveCriticalSection(&m_xPAskLock);
}

bool P2pAskCenter::AddP2pAskObj( DWORD dwSend, DWORD dwRecv, BYTE bType, char* szName )
{
	EnterCriticalSection(&m_xPAskLock);
	for(int i = 0; i < m_pList->Count; i++)
	{
		P2pAskObj*	pPA		= (P2pAskObj*)m_pList->Items[i];
		if(pPA->dwSend	== dwSend && pPA->dwRecv	== dwRecv && pPA->bType	== bType)
		{
			LeaveCriticalSection(&m_xPAskLock);
			return false;
		}
	}
	P2pAskObj*	pPA		= new P2pAskObj();
	pPA->bStep			= PAS_ASK;
	pPA->bType			= bType;
	pPA->dwRecv			= dwRecv;
	pPA->dwSend			= dwSend;
	pPA->dwStartTime	= GetTickCount();
	pPA->nIndex			= m_nMaxIndex++;
	strcpy(pPA->szName,szName);
	m_pList->Add(pPA);
	LeaveCriticalSection(&m_xPAskLock);
	return true;
}

P2pAskObj* P2pAskCenter::GetMyPAObj( DWORD dwUin )
{
	P2pAskObj*	pRtl	= NULL;
	EnterCriticalSection(&m_xPAskLock);
	for(int i = 0; i < m_pList->Count; i++)
	{
		P2pAskObj*	pPA		= (P2pAskObj*)m_pList->Items[i];
		switch(pPA->bStep)
		{
		case PAS_ASK:
			{
				if(pPA->dwRecv	== dwUin)
				{
					pRtl	= pPA;
					pPA->dwStartTime	= GetTickCount();
				}
			}
			break;
		case PAS_ANSWER:
			{
				if(pPA->dwSend	== dwUin)
				{
					pRtl	= pPA;
					pPA->dwStartTime	= GetTickCount();
				}
			}
			break;
		}
	}
	LeaveCriticalSection(&m_xPAskLock);
	return pRtl;
}

P2pAskObj* P2pAskCenter::GetMyPAIndex( int nIndex )
{
	P2pAskObj*	pRtl	= NULL;
	EnterCriticalSection(&m_xPAskLock);
	for(int i = 0; i < m_pList->Count; i++)
	{
		P2pAskObj*	pPA		= (P2pAskObj*)m_pList->Items[i];

		if(pPA->nIndex	== nIndex)
		{
			pRtl	= pPA;
			pPA->dwStartTime	= GetTickCount();
		}
	}
	LeaveCriticalSection(&m_xPAskLock);
	return pRtl;
}
