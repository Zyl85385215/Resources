#include "stdafx.h"
#include "UserData.h"
#include "TitleCenter.h"

TitleCenter::TitleCenter(UserData* pUser)
{
	m_pParent = pUser;
	m_pObj = NULL;
	m_pNextObj = NULL;
}

TitleCenter::~TitleCenter()
{

}

void TitleCenter::AddHonour(int nVal)
{
	if(nVal == 0)
		return;
	int nOld = m_pParent->m_xUWealth[UW_HONOUR];
	int nNew = nOld + nVal;
	int nMax = TitleConfig::GetInstance().GetMaxHonour();
	if(nNew <= 0)
		nNew = 0;
	else if(nNew > nMax)
		nNew = nMax;
	int nDelta = nNew - nOld;
	if(nDelta > 0)
		m_pParent->m_xUWealth.AddWealth(UW_HONOUR,nDelta);
	else
		m_pParent->m_xUWealth.SubWealth(UW_HONOUR,-nDelta);
	ResetObj();
}

void TitleCenter::ResetObj()
{
	TitleObj* pOldObj = m_pObj;
	m_pObj = TitleConfig::GetInstance().GetObj(m_pParent->m_xUWealth[UW_HONOUR]);
	m_pNextObj = TitleConfig::GetInstance().GetNextObj(m_pParent->m_xUWealth[UW_HONOUR]);
	if(m_pObj != pOldObj)
	{
		m_pParent->RecalcParam();
		if(m_pParent->m_pMObjCenter && m_pParent->m_pMObjCenter->m_pPlayer)
		{
			char* szname = m_pObj->pName1;
			if(m_pParent->m_byCountry == 2)
				szname = m_pObj->pName2;
			m_pParent->m_pMObjCenter->m_pPlayer->ChangeTitle(szname);
		}
	}
}

void TitleCenter::KillEnemy(TitleObj* pObj)
{
	if(pObj)
	{
		int nHonour = pObj->nSupportVal;
		if(nHonour > 0 && m_pParent->m_pVIPCenter)
			nHonour = m_pParent->m_pVIPCenter->GetHonourAdd(nHonour);
		AddHonour(nHonour);
	}
}

void TitleCenter::BeKilled()
{
	if(m_pObj)
	{
		AddHonour(-m_pObj->nLostVal);
	}
}