#include "stdafx.h"
#include "MakeSkillData.h"
#include "ItemCenter.h"
#include "DBControl.h"
#include "UserData.h"
#include "debug_def.h"

int MakeSkillData::LearnMakeSkill( MAKE_SKILL_TYPE byType )
{
	if (m_pParent->GetWealth(UW_LV)<35)
	{
		return LEARN_SKILL_LV;
	}
	int nPos = -1;
	for (int i=0;i<nMakeSkillCount;i++)
	{
		if (nPos == -1&&m_skillInfo.bySkill[i] == MAKE_SKILL_TYPE_NULL)
		{
			nPos = i;
		}else if (m_skillInfo.bySkill[i] == byType)
		{
			return LEARN_SKILL_EXIST;
		}
	}
	if (nPos == -1)
	{
		return LEARN_SKILL_FULL;
	}
	int nCnt = m_pParent->GetFlag(nMakeSkillResetTimesFlag);
	if (nCnt == 0&&!m_pParent->m_xUWealth.SubWealth(UW_GOLD,10000))
	{
		return LEARN_SKILL_NOT_ENOUGH_MONEY;
	}else if (nCnt == 1&&!m_pParent->m_xUWealth.SubWealth(UW_GOLD,50000))
	{
		return LEARN_SKILL_NOT_ENOUGH_MONEY;
	}else if (nCnt>1&&!m_pParent->m_xUWealth.SubWealth(UW_RMB,50,RMB_SAVE_MAKE_SKILL))
	{
		return LEARN_SKILL_NOT_ENOUGH_MONEY;
	}

	_setMakeSkill(nPos,byType);
	m_pParent->AddFlag(nMakeSkillResetTimesFlag,1);
	return LEARN_SKILL_SUCCESS;
}

int MakeSkillData::GiveUpMakeSkill( MAKE_SKILL_TYPE byType )
{
	bool bFind = false;
	for (int i=0;i<nMakeSkillCount;i++)
	{
		if (!bFind&&m_skillInfo.bySkill[i] == byType)
		{
			bFind = true;
			_setMakeSkill(i,(i==nMakeSkillCount-1)?0:m_skillInfo.bySkill[i+1]);
		}
// 		else if (bFind)
// 		{
// 			_setMakeSkill(i,(i==nMakeSkillCount-1)?0:m_skillInfo.bySkill[i+1]);
// 		}
	}
	if (bFind)
	{
		return GIVE_UP_SKILL_SUCCESS;
	}
	return GIVE_UP_SKILL_UNEXIST;
}

void MakeSkillData::_setMakeSkill( int nPos,BYTE byType )
{
	ASSERT(nPos>=0&&nPos<nMakeSkillCount);
	ASSERT(byType<MAKE_SKILL_TYPE_MAX);
	m_skillInfo.bySkill[nPos] = byType;
	_updateDBSkill();
	SendMakeSkillInfo();
}

int MakeSkillData::UseMakeSkill( int nRule )
{
	MakeSkillTag* pTag = MakeSkillConfig::GetInstance().GetMakeItem(nRule);
	if (!pTag)
	{
		return USE_MAKE_SKILL_CFG_ERROR;
	}
	if (pTag->nLv>m_pParent->GetWealth(UW_LV))
	{
		return USE_MAKE_SKILL_LV_ERROR;
	}
	if (pTag->nVigor>m_skillInfo.nPower)
	{
		return USE_MAKE_SKILL_NOT_ENOUGH_POWER;
	}
	if (m_pParent->m_xUWealth.GetWealth(UW_GOLD) < pTag->nGold)
	{
		m_pParent->SendWarnMsg("金币不足，制作需要消耗金币%d",pTag->nGold);
		return USE_MAKE_SKILL_Gold_ERROR;
	}
	if (pTag->nGold > 0)
		m_pParent->m_xUWealth.SubWealth(UW_GOLD,pTag->nGold);
	AddPower(-pTag->nVigor);
	m_pTempTag = pTag;
	//m_pParent->m_pItemCenter->AddItemByID(pTag->tagGet.nItemID,pTag->tagGet.nItemCount);
	return USE_MAKE_SKILL_SUCCESS;
}

void MakeSkillData::_updateDBSkill()
{
	m_pParent->m_pDB->SetBLOB(FALSE,&m_skillInfo,sizeof(MakeSkill),"update "RPG_USER_TABLE" set makeskill = ? where uin = %d",m_pParent->m_dwUin);

	//m_pParent->m_pDB->SetBLOB();
}

void MakeSkillData::LoadMakeSkillInfo(BYTE* pbD, int nSize)
{
	//memset(&m_skillInfo,0,sizeof(MakeSkill));
	if (nSize)
	{
		memcpy(&m_skillInfo,pbD,nSize>sizeof(MakeSkill)?sizeof(MakeSkill):nSize);
	}
}

MakeSkillData::MakeSkillData( UserData* pData ):m_pParent(pData),m_pTempTag(NULL)
{
	memset(&m_skillInfo,0,sizeof(MakeSkill));
	m_skillInfo.nPower = 70;
}

void MakeSkillData::SendMakeSkillInfo()
{
	BuildPacketEx(pPacket, USER, S_MAKESKILL_INFO, buf, 1024);
	SET_DATA(pSend, USER, S_MAKESKILL_INFO, pPacket);
	pSend->skillInfo = m_skillInfo;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}
extern char* szStarCountry[];
void MakeSkillData::MakeSkillResult( BYTE byResult )
{
	if (!m_pTempTag)
	{
		return;
	}

	if (byResult == 1)
	{
		for (int i = 0;i<m_pTempTag->pListCost->Count;i++)
		{
			CommonItemTag* pItem = (CommonItemTag*)m_pTempTag->pListCost->Items[i];
			if(m_pParent->m_pItemCenter->GetItemCountByID(pItem->nItemID)<pItem->nItemCount){ 
				m_pParent->SendPopMsg("所需材料不足");
				return;}
		}
		if (m_pParent->m_pItemCenter->GetBlankBag()<1)
		{
			return;
		}
		for (int i = 0;i<m_pTempTag->pListCost->Count;i++)
		{
			CommonItemTag* pItem = (CommonItemTag*)m_pTempTag->pListCost->Items[i];
			if(!m_pParent->m_pItemCenter->RemoveItemByID(pItem->nItemID,pItem->nItemCount)) return;
		}
		m_pParent->m_pItemCenter->AddItemByID(m_pTempTag->tagGet.nItemID,m_pTempTag->tagGet.nItemCount,0,ITEM_LOG_MAKE);
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(m_pTempTag->tagGet.nItemID);
		if (pBase)
		{
			char szMsg[128];
			sprintf(szMsg,"[%s]的[%s]经过不懈的努力，终于成功制造出了[%s]！",szStarCountry[m_pParent->m_byCountry],m_pParent->m_szNick,pBase->szName);
			ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
		}
	}
	m_pTempTag = NULL;
}

bool MakeSkillData::CheckMakeSkill( MAKE_SKILL_TYPE byType )
{
	if (byType == MAKE_SKILL_TYPE_DIG)
	{
		return CheckMakeSkill(MAKE_SKILL_TYPE_FORGE)||CheckMakeSkill(MAKE_SKILL_TYPE_STONE)||CheckMakeSkill(MAKE_SKILL_TYPE_SEWING);
	}else if (byType == MAKE_SKILL_TYPE_COLLECT)
	{
		return CheckMakeSkill(MAKE_SKILL_TYPE_FORGE)||CheckMakeSkill(MAKE_SKILL_TYPE_STONE)||CheckMakeSkill(MAKE_SKILL_TYPE_SEWING)||CheckMakeSkill(MAKE_SKILL_TYPE_METALLURGY);
	}else if (byType == MAKE_SKILL_TYPE_FISH)
	{
		return CheckMakeSkill(MAKE_SKILL_TYPE_STONE)||CheckMakeSkill(MAKE_SKILL_TYPE_FOOD);
	}
	for (int i=0;i<nMakeSkillCount;i++)
	{
		if (m_skillInfo.bySkill[i] == byType)
		{
			return true;
		}
	}
	return false;
}

int MakeSkillData::GetPowerMax()
{
	int nlv = m_pParent->GetWealth(UW_LV);
	if (nlv<40)
	{
		return 70;
	}else if (nlv<50)
	{
		return 90;
	}else if (nlv<60)
	{
		return 110;
	}else if (nlv<70)
	{
		return 130;
	}else if (nlv<80)
	{
		return 150;
	}else if (nlv<90)
	{
		return 170;
	}else if (nlv<100)
	{
		return 190;
	}else
	{
		return 210;
	}
}

void MakeSkillData::AddPower( int nPower )
{
	m_skillInfo.nPower +=nPower;
	int powerMax = GetPowerMax();
	if (m_skillInfo.nPower<0)
	{
		m_skillInfo.nPower = 0;
	}else if (m_skillInfo.nPower>=powerMax)
	{
		m_skillInfo.nPower = powerMax;
	}
	_updateDBSkill();
	SendMakeSkillInfo();
}
