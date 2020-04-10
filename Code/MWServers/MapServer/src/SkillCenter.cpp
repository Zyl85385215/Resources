#include "stdafx.h"

#include "SkillCenter.h"
#include "MapAIObj.h"
#include "StateCenter.h"
#include "ControlObject.h"
#include "ios_mapobj_packet.h"
#include "ZNetServer.h"

SkillObj::SkillObj( WORD wID,BYTE bLv )
{
	pSBase		= SkillConfig::GetInstance().GetSkillBase(wID);
	if(pSBase)
		SetLv(bLv);
	dwCDTick	= GetTickCount();
}

SkillObj::~SkillObj()
{

}

void SkillObj::ResetCD(MapAIObj* pParent)
{
	dwCDTick	= GetTickCount() + pLvBase->dwCDTick;

	if(pLvBase->dwCDTick > 2000 && pParent && pParent->pCtrl && pParent->pCtrl->m_pMainAIObj == pParent)
	{
		BuildPacketEx(pPacket, MAPOBJ, MS_SETSKILLCD, buf, 256);
		SET_DATA(pData, MAPOBJ, MS_SETSKILLCD,pPacket);
		pData->wSkillID	= pSBase->wSkillID;
		pData->dwTickCD	= pLvBase->dwCDTick;
		g_pNetCenter->SendPacketToGameSrv(pPacket,pParent->dwUin);
	}





}


void SkillObj::SetLv( BYTE bLv )
{
	bySLv		= bLv;
	pLvBase		= pSBase->GetSELv(bLv);
}

bool SkillObj::CheckTargetRange( int nDis )
{
	if(pSBase->bAoeType == 0 || pSBase->bAoeType == 3)
		return true;
	if(nDis < pSBase->wAtkRange+50)
		return true;

	return false;
}


SkillCenter::SkillCenter(MapAIObj* pObj)
{
	m_pListSkill	= xnList::Create();
	memset(m_ppActSkill,0,MAX_ACTIVE_SKILL*4);
	m_pParent		= pObj;
	m_wStrikeSkill	= 0;
}

SkillCenter::~SkillCenter()
{
	while(m_pListSkill->Count)
		delete (SkillObj*)m_pListSkill->Delete(0);
	m_pListSkill->Free();
}

SkillObj* SkillCenter::AutoUserSkill()
{
	if(m_wStrikeSkill)
	{
		SkillObj* pSkill = CheckUserSkill(m_wStrikeSkill);
		if(pSkill)
			return pSkill;
	}
	if(m_pListSkill->Count)
	{
		int	nStartPos	= GetTickCount()/10;
		for (int i = nStartPos; i < nStartPos+m_pListSkill->Count; i++)
		{
			SkillObj*	pSObj	= (SkillObj*)m_pListSkill->Items[i%m_pListSkill->Count];
			if(pSObj->dwCDTick && ((int)(GetTickCount() - pSObj->dwCDTick))<0)
				continue;
			if(pSObj->pSBase->bType == 3 || pSObj->pSBase->bType == 0)
				continue;

			return pSObj;
		}
	}
	return NULL;
}

SkillObj* SkillCenter::CheckUserSkill( WORD wSID )
{
	SkillObj*	pSObj	= GetSkill(wSID);
	if(pSObj)
	{
		if(pSObj->dwCDTick && ((int)(GetTickCount() - pSObj->dwCDTick))<0)
			return NULL;

		if(pSObj->pSBase->bType == 3)
		{
			//触发类技能只有被触发过才能释放
			if(pSObj->pSBase->wSkillID != m_wStrikeSkill)
				return NULL;
		}


		if(m_pParent->byType == MAPOBJ_PLAYER)
		{//人物角色需要耗蓝
			int	nCostVal	= 0;
			
			if(pSObj->pSBase->nHpCost)
			{
				if(!pSObj->pSBase->bCostNoPer)
				{
					nCostVal	= m_pParent->xParam.nHpMax*pSObj->pSBase->nHpCost/100;
				}
				else
					nCostVal	= pSObj->pSBase->nHpCost;

				if(m_pParent->xParam.nHp <= nCostVal)
					return NULL;

				m_pParent->ChgHp(-nCostVal);
			}
			int	nMpCost	= pSObj->pLvBase->nMpCost;
			if(nMpCost == 0)
				nMpCost	= pSObj->pSBase->nMpCost;
			if(nMpCost)
			{
				if(!pSObj->pSBase->bCostNoPer)
				{
					nCostVal	= m_pParent->xParam.nMpMax*nMpCost/100;
				}
				else
					nCostVal	= nMpCost;

				if(m_pParent->xParam.nMp < nCostVal)
					return NULL;

				m_pParent->ChgMp(-nCostVal);
			}
			if(pSObj->pSBase->nSpCost && m_pParent->pCtrl)
			{
				if(!pSObj->pSBase->bCostNoPer)
				{
					nCostVal	= 50*pSObj->pSBase->nSpCost/100;
				}
				else
					nCostVal	= pSObj->pSBase->nSpCost;

				if(m_pParent->pCtrl->m_nSpVal < nCostVal)
					return NULL;

				m_pParent->pCtrl->ChgSP(-nCostVal);
			}
			
		}

		return pSObj;
	}

	return NULL;
}

void SkillCenter::SetSkill( WORD wSID, BYTE bLv,BYTE	bPos, DWORD	dwCD )
{
	SkillObj*	pSObj	= GetSkill(wSID);
	if(pSObj	== NULL)
	{
		pSObj	= new SkillObj(wSID,bLv);
		if(pSObj->pSBase == NULL)
		{
			delete pSObj;
			return;
		}
		m_pListSkill->Add(pSObj);
	}
	else
	{
		pSObj->SetLv(bLv);
	}

	pSObj->byPos	= bPos;
	pSObj->dwCDTick	= GetTickCount()+dwCD;
	
	if(bPos != 0 && bPos <= MAX_ACTIVE_SKILL)
		m_ppActSkill[bPos-1]	= pSObj;

	if(pSObj->pSBase->bType == 0)
	{//被动技能直接加状态效果
		for(int i = 0; i < pSObj->pSBase->pListStateAdd->Count; i++)
		{
			_MuliParamObj*	pMP	= (_MuliParamObj*)pSObj->pSBase->pListStateAdd->Items[i];
			m_pParent->pStateCenter->AddState(pMP->nParam2+pSObj->pLvBase->bAddStateID);
		}
	}

}

void SkillCenter::ResetJobSkill()
{
	while(m_pListSkill->Count)
	{
		SkillObj* pSObj	= (SkillObj*)m_pListSkill->Delete(0);
		if(pSObj->pSBase->bType == 0)
		{//被动技能直接加状态效果
			for(int i = 0; i < pSObj->pSBase->pListStateAdd->Count; i++)
			{
				_MuliParamObj*	pMP	= (_MuliParamObj*)pSObj->pSBase->pListStateAdd->Items[i];
				m_pParent->pStateCenter->RemoveState(pMP->nParam2+pSObj->pLvBase->bAddStateID);
			}
		}
		delete pSObj;
	}

	memset(m_ppActSkill,0,MAX_ACTIVE_SKILL*4);
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

void SkillCenter::StrikeSkill( WORD wSkill )
{
	m_wStrikeSkill	= wSkill;

	if(m_pParent->pCtrl)
	{
		m_pParent->pCtrl->StrikeSkill(wSkill);
	}
}

