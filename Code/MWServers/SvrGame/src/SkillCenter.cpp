#include "stdafx.h"

#include "SkillCenter.h"
#include "UserData.h"

SkillObj::SkillObj( WORD wID,BYTE bLv )
{
	pSBase		= SkillConfig::GetInstance().GetSkillBase(wID);
	if(pSBase)
		SetLv(bLv);
	dwCDTick	= 0;
	byPos		= 0;
}

SkillObj::~SkillObj()
{

}

void SkillObj::SetLv( BYTE bLv )
{
	bySLv		= bLv;
	pLvBase		= pSBase->GetSELv(bLv);
}


SkillCenter::SkillCenter(UserData* pUD)
{
	m_pUser		= pUD;
	m_pListSkill	= xnList::Create();
}

SkillCenter::~SkillCenter()
{
	while(m_pListSkill->Count)
		delete (SkillObj*)m_pListSkill->Delete(0);
	m_pListSkill->Free();
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
	}
	else
	{
		pSObj->SetLv(bLv);
	}
	//其他初始化
	return pSObj;
}

void SkillCenter::RemoveSpSkill( WORD wSID )
{
	SkillObj*	pSkill	= GetSkill(wSID);
	if(pSkill)
	{
		pSkill->byPos	= 0;
		_syncMObj(pSkill);
	}

	for (int i = 0; i < m_pListSkill->Count; i++)
	{
		SkillObj* pSkill	= (SkillObj*)m_pListSkill->Items[i];

		if(pSkill->pSBase->bType == 4 && pSkill->pSBase->wSkillID == wSID)
		{
			_removeDB(pSkill);
			_sendDelSkill(pSkill->pSBase->wSkillID);
			m_pListSkill->Delete(i);
			delete pSkill;
			return;
		}
	}
}

void SkillCenter::ResetJobSkill(  )
{
	for (int i = 0; i < m_pListSkill->Count; i++)
	{
		SkillObj* pSkill	= (SkillObj*)m_pListSkill->Items[i];

		if(pSkill->pSBase->wSkillID < 5000)
		{
			_removeDB(pSkill);
			_sendDelSkill(pSkill->pSBase->wSkillID);
			m_pListSkill->Delete(i);
			delete pSkill;
			i--;
		}
	}
}

SkillObj* SkillCenter::GetSkillByPos( BYTE bPos)
{
	for (int i = 0; i < m_pListSkill->Count; i++)
	{
		SkillObj*	pSObj	= (SkillObj*)m_pListSkill->Items[i];
		if(pSObj->byPos	== bPos)
		{
			return	pSObj;
		}
	}
	return NULL;
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

bool SkillCenter::RequestSkillUp( WORD wSID,bool bJob )
{
	SkillBase* pSBase	= NULL;
	if(bJob)
	{
		for (int i = 0; i < 12; i++)
		{
			SkillBase* pSB	= SkillConfig::GetInstance().GetJobSkill(m_pUser->m_byJob,i);
			if(pSB->wSkillID == wSID)
			{
				pSBase	= pSB;
				break;
			}
		}
	}
	else
	{
		pSBase	= SkillConfig::GetInstance().GetSkillBase(wSID);
	}
	if(pSBase == NULL)
	{
		return false;
	}
	SkillObj*	pSkill	= GetSkill(wSID);
	BYTE	bLvNext	= 1;
	if(pSkill)
		bLvNext	= pSkill->bySLv +1;
	SEffLvInfo*	pLvNext	= pSBase->GetSELv(bLvNext,false);
	if(pLvNext == NULL)
	{
		return false;
	}
	if(pLvNext->byUpLvNeed > m_pUser->m_xUWealth[UW_LV])
	{
		return false;
	}
	if(pSBase->byCostSPoint > m_pUser->m_xUWealth[UW_SKILLPT])
	{
		return false;
	}
	if(pSBase->nGoldLvNeed > m_pUser->m_xUWealth[UW_GOLD])
		return false;

	if(pSBase->wPreSkillID)
	{
		SkillObj*	pSkillPre	= GetSkill(pSBase->wPreSkillID);
		if(pSkillPre == NULL || pSkillPre->bySLv < pSBase->bPreSkillLv)
		{
			return false;
		}
	}

	m_pUser->m_xUWealth.SubWealth(UW_GOLD,	pSBase->nGoldLvNeed);
	m_pUser->m_xUWealth.SubWealth(UW_SKILLPT,	pSBase->byCostSPoint);

	if(pSkill == NULL)
	{
		pSkill	= SetSkill(wSID,1);
		_insertDB(pSkill);
	}
	else
	{
		pSkill->SetLv(pSkill->bySLv+1);
		_saveDB(pSkill);
	}
	m_pUser->m_pUserRecord->RunLuaSkillFunc(1,pSkill->pSBase->wSkillID,pSkill->pSBase->bType,pSkill->bySLv);
	//同步数据
	_sendClient(pSkill);
	if(pSBase->bType != 4 || pSkill->byPos	== 5)
		_syncMObj(pSkill);
	return true;
}

void SkillCenter::DBInit()
{
	QueryRlt xRlt;
	if(!m_pUser->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_SKILL_TABLE" where uin=%d",m_pUser->m_dwUin))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		int	nSkill		= pCmd->GetInt("skill");
		int	nLv			= pCmd->GetInt("slv");
		SkillObj* pSkill	= SetSkill(nSkill,nLv);
		if(pSkill)
		{
			pSkill->dwCDTick	= pCmd->GetInt("cdsec");
			pSkill->byPos		= pCmd->GetInt("spos");
		}

	}
}

void SkillCenter::_insertDB( SkillObj* pSObj )
{
	m_pUser->m_pDB->ExecuteSQL(false,"INSERT INTO "RPG_SKILL_TABLE" (uin,skill,slv,spos,cdsec) \
																  VALUES(%d,%d,%d,%d,%d)", m_pUser->m_dwUin,pSObj->pSBase->wSkillID,pSObj->bySLv,pSObj->byPos,pSObj->dwCDTick);
}

void SkillCenter::_removeDB( SkillObj* pSObj )
{
	m_pUser->m_pDB->ExecuteSQL(false,"delete from "RPG_SKILL_TABLE" where uin=%d and skill=%d",m_pUser->m_dwUin,pSObj->pSBase->wSkillID);
}

void SkillCenter::_saveDB( SkillObj* pSObj )
{
	m_pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_SKILL_TABLE" set slv=%d,cdsec=%d,spos=%d where uin=%d and skill=%d",
		pSObj->bySLv,pSObj->dwCDTick,pSObj->byPos,m_pUser->m_dwUin,pSObj->pSBase->wSkillID);
}
void SkillCenter::_sendDelSkill( WORD wSID )
{
	BuildPacketEx(pPacket,USER,S_SKILL_SINGLE,buf,1024);
	SET_DATA(pSend, USER, S_SKILL_SINGLE,pPacket);
	pSend->wSkill	= wSID;	
	pSend->byLv		= 0;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void SkillCenter::_sendClient( SkillObj* pSObj )
{
	BuildPacketEx(pPacket,USER,S_SKILL_SINGLE,buf,1024);
	SET_DATA(pSend, USER, S_SKILL_SINGLE,pPacket);
	pSend->wSkill	= pSObj->pSBase->wSkillID;	
	pSend->byLv		= pSObj->bySLv;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void SkillCenter::_syncMObj( SkillObj* pSObj )
{
	m_pUser->m_pMObjCenter->m_pPlayer->SetSkill(pSObj->pSBase->wSkillID,pSObj->bySLv,pSObj->byPos,pSObj->dwCDTick);
}

void SkillCenter::SendSyncAllSkill()
{
	BuildPacketEx(pPacket,USER,S_SKILL_ALL,buf,1024);
	SET_DATA(pSend, USER, S_SKILL_ALL,pPacket);
	pSend->bCnt		= m_pListSkill->Count;
	pPacket->wSize	+= (pSend->bCnt-1)*sizeof(USER_S_SKILL_SINGLE_Data);
	for (int i = 0; i < m_pListSkill->Count; i++)
	{
		SkillObj*	pSkill	= (SkillObj*)m_pListSkill->Items[i];
		if(pSkill->pSBase->bType != 4 || pSkill->byPos == 5)
			m_pUser->m_pMObjCenter->m_pPlayer->SetSkill(pSkill->pSBase->wSkillID,pSkill->bySLv,pSkill->byPos,pSkill->dwCDTick);

		pSend->szSkill[i].wSkill	= pSkill->pSBase->wSkillID;
		pSend->szSkill[i].byLv		= pSkill->bySLv;
	}

	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void SkillCenter::RequestSetSCut( WORD wSID, BYTE bPos )
{
	if(bPos)
	{
		SkillObj*	pSTmp	= GetSkillByPos(bPos);
		if(pSTmp)
		{
			pSTmp->byPos	= 0;
			_syncMObj(pSTmp);
			_saveDB(pSTmp);
		}
	}

	SkillObj*	pSkill	= GetSkill(wSID);
	if(pSkill)
	{
		pSkill->byPos	= bPos;
		_syncMObj(pSkill);
		_saveDB(pSkill);
		if (bPos)
		{
			m_pUser->SendWarnMsg("设置成功");
		}else{
			m_pUser->SendWarnMsg("清除成功");
		}
	}
}

