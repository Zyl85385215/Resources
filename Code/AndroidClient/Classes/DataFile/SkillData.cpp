#include "MainMenu.h"
#include "SkillData.h"
#include "UserData.h"
#include "MapAIObj.h"

SkillObj::SkillObj( WORD wID,BYTE bLv )
{
	pSBase		= SkillConfig::GetInstance().GetSkillBase(wID);
	if(pSBase)
		SetLv(bLv);
	dwCDTick	= xnGetTickCount();
}

SkillObj::~SkillObj()
{

}

//void SkillObj::ResetCD()
//{
//	dwCDTick	= xnGetTickCount() + pLvBase->dwCDTick;
//}

bool SkillObj::CheckTargetRange( int nDis )
{
	if(pSBase->bAoeType == 0 || pSBase->bAoeType == 3)
		return true;
	if(nDis < pSBase->wAtkRange)
		return true;

	return false;
}

void SkillObj::SetLv( BYTE bLv )
{
	bySLv		= bLv;
	pLvBase		= pSBase->GetSELv(bLv);
}


SkillCenter::SkillCenter()
{
	m_pListSkill	= xnList::Create();
	m_pListSPSkill	= xnList::Create();
	memset(m_ppActSkill,0,MAX_ACTIVE_SKILL*4);
}

SkillCenter::~SkillCenter()
{
	while(m_pListSkill->Count)
		delete (SkillObj*)m_pListSkill->Delete(0);
	m_pListSkill->Free();
	ClearActSkill();
	m_pListSPSkill->Free();
}

SkillObj* SkillCenter::CheckUserSkill( WORD wSID )
{
	SkillObj*	pSObj	= GetActiveSkill(wSID);	//只有这张表的里有CD时间记录
	if(pSObj == NULL)
		pSObj	= GetSkill(wSID);
	if(pSObj)
	{
		if(pSObj->dwCDTick && ((int)(pSObj->dwCDTick-xnGetTickCount()))>0)
			return NULL;
		
		int	nCostVal	= 0;

		if(pSObj->pSBase->nHpCost)
		{
			if(!pSObj->pSBase->bCostNoPer)
			{
				nCostVal	= pChar->GetMyMapObj()->nMaxHP*pSObj->pSBase->nHpCost/100;
			}
			else
				nCostVal	= pSObj->pSBase->nHpCost;

			if(pChar->GetMyMapObj()->nHP < nCostVal)
			{
				if(pMainMenu->m_pAutoMenu->m_bActive == false)
					pMainMenu->PopTipMsg("生命量不足");
				return NULL;
			}
		}

		int	nMpCost	= pSObj->pLvBase->nMpCost;
		if(nMpCost == 0)
			nMpCost	= pSObj->pSBase->nMpCost;
		if(nMpCost)
		{
			if(!pSObj->pSBase->bCostNoPer)
			{
				nCostVal	= pChar->m_nMaxMp*nMpCost/100;
			}
			else
				nCostVal	= nMpCost;

			if(pChar->m_nMp < nCostVal)
			{
				if(pMainMenu->m_pAutoMenu->m_bActive == false)
					pMainMenu->PopTipMsg("魔法值不足");
				return NULL;
			}

		}
		if(pSObj->pSBase->nSpCost)
		{
			if(!pSObj->pSBase->bCostNoPer)
			{
				nCostVal	= 50*pSObj->pSBase->nSpCost/100;
			}
			else
				nCostVal	= pSObj->pSBase->nSpCost;

			if(pChar->m_nSPVal < nCostVal)
			{
				if(pMainMenu->m_pAutoMenu->m_bActive == false)
					pMainMenu->PopTipMsg("怒气值不足");
				return NULL;
			}
		}
		return pSObj;
	}

	return NULL;
}

void SkillCenter::SetActSkill( WORD wSID, BYTE bLv,BYTE bPos,int nCD )
{
	if(bPos >= MAX_ACTIVE_SKILL)
		return;

	if(m_ppActSkill[bPos]	!= NULL)
	{
		if(m_ppActSkill[bPos]->pSBase->wSkillID != wSID)
		{
			delete m_ppActSkill[bPos];
			m_ppActSkill[bPos]	= NULL;
		}
	}
	for (int i= 0; i < MAX_ACTIVE_SKILL; i++)
	{
		if(m_ppActSkill[i] && m_ppActSkill[i]->pSBase->wSkillID == wSID && i != bPos)
		{
			delete m_ppActSkill[i];
			m_ppActSkill[i]	= NULL;
		}
	}
	if(m_ppActSkill[bPos]	== NULL)
	{
		m_ppActSkill[bPos]	= new SkillObj(wSID,bLv);
		if(m_ppActSkill[bPos]->pSBase == NULL)
		{
			delete m_ppActSkill[bPos];
			m_ppActSkill[bPos]	= NULL;
			return;
		}
	}
	else
	{
		m_ppActSkill[bPos]->SetLv(bLv);
	}

	//其他初始化
	if(m_ppActSkill[bPos])
		m_ppActSkill[bPos]->dwCDTick	= xnGetTickCount() + nCD;
}

void SkillCenter::RemoveSkill( WORD wSID )
{
	for (int i = 0; i < m_pListSkill->Count; i++)
	{
		SkillObj*	pSObj	= (SkillObj*)m_pListSkill->Items[i];
		if(pSObj->pSBase->wSkillID	== wSID)
		{
			if(pSObj->pSBase->bType == 4)
				m_pListSPSkill->Remove(pSObj);
			m_pListSkill->Delete(i);
			delete	pSObj;
			return;
		}
	}
}


SkillObj* SkillCenter::GetSkill( WORD wSID )
{
	for (int i = 0; i < m_pListSkill->Count; i++)
	{
		SkillObj*	pSObj	= (SkillObj*)m_pListSkill->Items[i];
		if(pSObj->pSBase->wSkillID	== wSID)
		{
			return	pSObj;
		}
	}
	return NULL;
}

SkillObj* SkillCenter::GetActiveSkill( WORD wSID )
{
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		SkillObj*	pSObj	= m_ppActSkill[i];
		if(pSObj && pSObj->pSBase->wSkillID	== wSID)
		{
			return	pSObj;
		}
	}
	return NULL;
}

void SkillCenter::ResetSkillCD( WORD wSID )
{
	SkillObj*	pSkill	= GetActiveSkill(wSID);
	if(pSkill && pSkill->pLvBase)
	{
		pSkill->dwCDTick	= xnGetTickCount()+pSkill->pLvBase->dwCDTick;
	}
	pChar->m_dwLockSkill	= 0;
	pChar->m_wLockSkillID	= 0;
}

void SkillCenter::ClearActSkill()
{
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		if(m_ppActSkill[i])
			delete m_ppActSkill[i];
		m_ppActSkill[i]	= NULL;
	}
}

void SkillCenter::ClearSkill()
{
	ClearActSkill();
	while(m_pListSkill->Count)
		delete (SkillObj*)m_pListSkill->Delete(0);
	m_pListSPSkill->Clear();
}

SkillObj* SkillCenter::SetSkill( WORD wSID, BYTE bLv )
{
	SkillObj*	pSObj	= GetSkill(wSID);
	if(pSObj	== NULL)
	{
		pSObj	= new SkillObj(wSID,bLv);
		if(pSObj->pSBase == NULL)
		{
			delete pSObj;
			return NULL;
		}
		m_pListSkill->Add(pSObj);

		if(pSObj->pSBase->bType == 4)
			m_pListSPSkill->Add(pSObj);
	}
	else
		pSObj->SetLv(bLv);
	return pSObj;
}

int SkillCenter::CheckSkillSetting( WORD wSID )
{
	for (int i=0;i<MAX_ACTIVE_SKILL;i++)
	{
		if (m_ppActSkill[i]&&m_ppActSkill[i]->pSBase->wSkillID == wSID)
		{
			return i+1;
		}
	}
	return 0;
}

