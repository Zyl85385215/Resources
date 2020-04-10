#include "SlaveData.h"
#include "UserData.h"
#include "TextConfig.h"
#include "MainMenu.h"

void GoMallEvent5(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();

	}else{
		if (pChar->m_dwMoney < 500000)
		{
			pMainMenu->PopTipMsg("金币不足！金币低于50w。");
			return;
		}
		if (pChar->m_pSlaveCenter->m_pActiveSlave && pChar->m_pSlaveCenter && pChar && pChar->m_pSlaveCenter->m_pActiveSlave->m_nHp <= 0)
		{
			pMainMenu->PopTipMsg("宠物正在复活请稍等...");
			return;
		}

		BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_REVIVEPET_BYGOLD,buf,1024);
		SET_DATA(pData,JGSLAVE,C_REQUEST_REVIVEPET_BYGOLD,pPacket);
		pData->dwID = (int)param;
		pChar->SendPacket(pPacket);
	}
}

SlaveCData::SlaveCData()
{
	memset(m_nParam,0,SLAVE_PARAM_MAX*4);
	memset(m_nSkill,0,SLAVE_SKILL_MAX*4);
	memset(m_nStrengthen,0,SLAVE_STRENGTHEN_MAX*4);
	memset(m_nSkillLv,0,SLAVE_SKILL_MAX*4);
	m_pBasic = NULL;
	m_pSkinObj = NULL;
	m_pVariat = NULL;
	m_byState = SLAVE_STATE_NORMAL;
	m_ifActive = false;
	m_dwMapID = 0;
	m_nHp = 0;
	m_nLifePoint = 0;
}

SlaveCData::~SlaveCData()
{

}

void SlaveCData::RecalParam()
{
	memset(m_nParam,0,4*SLAVE_PARAM_MAX);
	m_pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(m_nType);
	if(m_pBasic != NULL)
	{
		m_pSkinObj = CfgObjSkin::GetInstance().GetObjSkin(m_pBasic->nSkin);
		if(m_byVariatlv > 0)
			m_pVariat = SlaveVariatCfg::GetInstance().GetBase(m_byVariatlv);
		else
			m_pVariat = NULL;
		for(int i = 0; i < SLAVE_PARAM_MAX; i ++)
		{
			m_nParam[i] += m_pBasic->szBaseParam[i];
			m_nParam[i] += (m_byLevel-1)*m_pBasic->szBaseUp[i];
			SlaveStreBasic* pStrBasic = SlaveStrengthenCfg::GetInstance().GetBase(i,m_nStrengthen[i]);
			if(pStrBasic)
				m_nParam[i] = m_nParam[i]*(100+pStrBasic->nVal)/100;
		}

		for (int j = 0; j < SLAVE_SKILL_MAX; j++)
		{
			SkillBase* pSkillBase = SkillConfig::GetInstance().GetSkillBase(m_nSkill[j]);
			if(pSkillBase && pSkillBase->bType == 0)
			{
				for (int s = 0; s < pSkillBase->pListStateAdd->Count; s++)
				{
					_MuliParamObj*	pMP	= (_MuliParamObj*)pSkillBase->pListStateAdd->Items[s];
					if (pMP)
					{
						StateBase*	pBase	= StateConfig::GetInstance().GetStateBase(pMP->nParam2 + (pSkillBase->GetSELv(m_nSkillLv[j]))->bAddStateID);
						if(pBase)
						{
							for (int e = 0; e < pBase->pListStateEff->Count; e++)
							{
								StateEffVal* pEff = (StateEffVal*)pBase->pListStateEff->Items[e];
								if (pEff)
								{
									if (pEff->bType == 1)
										m_nParam[SLAVE_PARAM_HPMAX] += pEff->nPer*m_nParam[SLAVE_PARAM_HPMAX]/10000;
									if (pEff->bType == 3)
										m_nParam[SLAVE_PARAM_MPMAX] += pEff->nPer*m_nParam[SLAVE_PARAM_MPMAX]/10000;
									if (pEff->bType == 4)
										m_nParam[SLAVE_PARAM_AD] += pEff->nPer*m_nParam[SLAVE_PARAM_AD]/10000;
									if (pEff->bType == 6)
										m_nParam[SLAVE_PARAM_DD] += pEff->nPer*m_nParam[SLAVE_PARAM_DD]/10000;
									if (pEff->bType == 5)
										m_nParam[SLAVE_PARAM_AP] += pEff->nPer*m_nParam[SLAVE_PARAM_AP]/10000;
									if (pEff->bType == 7)
										m_nParam[SLAVE_PARAM_DP] += pEff->nPer*m_nParam[SLAVE_PARAM_DP]/10000;
								}
							}
						}
					}
				}
			}
		}
	}
}

void SlaveCData::SetExp(BYTE byLevel,int nExp)
{
	m_nExp = nExp;
	m_byLevel = byLevel;
	m_byState = SLAVE_STATE_CHANGE;
}

void SlaveCData::AddExp(int nExp)
{
	BYTE byNowLv = m_byLevel;
	int nNowExp = m_nExp;
	int nTotalExp = nExp;
	int nCount = 0;
	while(nTotalExp > 0)
	{
		if(nCount > 100)
			break;
		if(byNowLv >= 100)
		{
			nTotalExp = 0;
			break;
		}
		int nLimit = SlaveBaseCfg::GetInstance().GetLvExp(byNowLv);
		if(nNowExp + nTotalExp >= nLimit)
		{
			nTotalExp = nNowExp + nTotalExp - nLimit;
			nNowExp = 0;
			byNowLv ++;
		}
		else
		{
			nNowExp += nTotalExp;
			nTotalExp = 0;
		}
		nCount ++;
	}
	if(nCount > 0)
	{
		m_byLevel = byNowLv;
		m_nExp = nNowExp;
		m_byState = SLAVE_STATE_CHANGE;
	}
}


static int PetSortBylv(const void * Item1, const void * Item2)
{
	SlaveCData * pData1 = *(SlaveCData **)Item1;
	SlaveCData * pData2 = *(SlaveCData **)Item2;
	
	return (pData2->m_pBasic->byExchangLv - pData1->m_pBasic->byExchangLv)*10000000 + (pData2->m_byLevel - pData1->m_byLevel)*100000 + (pData2->m_pBasic->byQuality - pData1->m_pBasic->byQuality)*10000 + (pData2->m_nType - pData1->m_nType);
}

SlaveCCenter::SlaveCCenter()
{
	m_pSlaveList = xnList::Create();
	m_ifNumChange = false;
	m_pActiveSlave = NULL;
	m_dwReviveTick = 0;
	m_byUnlockCnt = 0;
}

SlaveCCenter::~SlaveCCenter()
{
	if(m_pSlaveList)
	{
		while(m_pSlaveList->Count)
			delete (SlaveCData*)m_pSlaveList->Delete(0);
		m_pSlaveList->Free();
	}
}

void SlaveCCenter::ClearList()
{
	m_pActiveSlave = NULL;
	while(m_pSlaveList->Count)
		delete (SlaveCData*)m_pSlaveList->Delete(0);
	m_ifNumChange = true;
}

void GoMallAndHuodongEvent2(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pActivityCtrl->Open(2,0);
}

bool SlaveCCenter::UpdateSlave(SlaveSendInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	for(int i = 0;i < m_pSlaveList->Count; i ++)
	{
		SlaveCData* pData = (SlaveCData*)m_pSlaveList->Items[i];
		if(pData->m_dwID == pInfo->dwID)
		{
			if(pData->m_byState == SLAVE_STATE_RELEASE)
				return false;
			pData->m_byLevel = pInfo->byLevel;
			pData->m_nExp = pInfo->dwExp;
			pData->m_bySlot = pInfo->bySlot;			
			pData->m_byVariatlv = pInfo->byVariatlv;
			pData->m_nHp = pInfo->nHp;
			int nDelta = pInfo->nLifePoint - pData->m_nLifePoint;
			if(nDelta > 0)
			{
				pMainMenu->PopTipMsg("您的宠物获得%d点寿命",nDelta);
			}
			else if(nDelta < 0)
	//		if(pInfo->nHp == 0 && m_dwReviveTick == 0)
			{
				m_dwReviveTick = xnGetTickCount()+10000;
				if(pInfo->nLifePoint > 0)
					pMainMenu->PopTipMsg("您的宠物已死亡，失去%d点寿命,将在10秒后复活",-nDelta);
				else
					pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pData->m_dwID, 7);	
			}
			pData->m_nLifePoint = pInfo->nLifePoint;
			memcpy(pData->m_nStrengthen,pInfo->nStrengthen,4*SLAVE_STRENGTHEN_MAX);
			for (int s = 0; s < SLAVE_SKILL_MAX; s++)
			{
				pData->m_nSkill[s]	= pInfo->wSkill[s];
				pData->m_nSkillLv[s]	= pInfo->bySkillLv[s];
			}
			pData->m_pName = strdup(pInfo->szName);
			pData->m_byState = SLAVE_STATE_CHANGE;
			if(pData->m_nType != pInfo->nType)
			{
				pData->m_nType = pInfo->nType;
				pData->m_byState = SLAVE_STATE_EXCHANGE;
			}
			return true;
		}
	}
	SlaveCData* pData = new SlaveCData();
	pData->m_dwUin = pInfo->dwUin;
	pData->m_dwID = pInfo->dwID;
	pData->m_byLevel = pInfo->byLevel;
	pData->m_nExp = pInfo->dwExp;
	pData->m_bySlot = pInfo->bySlot;
	pData->m_nType = pInfo->nType;
	pData->m_byVariatlv = pInfo->byVariatlv;
	pData->m_nHp = pInfo->nHp;
	pData->m_nLifePoint = pInfo->nLifePoint;
	memcpy(pData->m_nStrengthen,pInfo->nStrengthen,4*SLAVE_STRENGTHEN_MAX);
	for (int s = 0; s < SLAVE_SKILL_MAX; s++)
	{
		pData->m_nSkill[s]	= pInfo->wSkill[s];
		pData->m_nSkillLv[s]	= pInfo->bySkillLv[s];
	}
	pData->m_pName = strdup(pInfo->szName);
	pData->m_byState = SLAVE_STATE_NEW;
	m_pSlaveList->Add((void*)pData);
	return true;
}

bool SlaveCCenter::AddSlave(SlaveSendInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	SlaveCData* pData = new SlaveCData();
	pData->m_dwUin = pInfo->dwUin;
	pData->m_dwID = pInfo->dwID;
	pData->m_byLevel = pInfo->byLevel;
	pData->m_nExp = pInfo->dwExp;
	pData->m_bySlot = pInfo->bySlot;
	pData->m_nType = pInfo->nType;
	pData->m_byVariatlv = pInfo->byVariatlv;
	pData->m_nHp = pInfo->nHp;
	pData->m_nLifePoint = pInfo->nLifePoint;
	memcpy(pData->m_nStrengthen,pInfo->nStrengthen,4*SLAVE_STRENGTHEN_MAX);
	for (int s = 0; s < SLAVE_SKILL_MAX; s++)
	{
		pData->m_nSkill[s]	= pInfo->wSkill[s];
		pData->m_nSkillLv[s]	= pInfo->bySkillLv[s];
	}
	pData->m_pName = strdup(pInfo->szName);
	pData->m_byState = SLAVE_STATE_NEW;
	m_pSlaveList->Add((void*)pData);
	return true;
}

void SlaveCCenter::RemoveSlave(DWORD id)
{
	for(int i = 0;i < m_pSlaveList->Count; i ++)
	{
		SlaveCData* pData = (SlaveCData*)m_pSlaveList->Items[i];
		if(pData->m_dwID == id)
		{
			pData->m_byState = SLAVE_STATE_RELEASE;
			return;
		}
	}
}

SlaveCData* SlaveCCenter::GetSlave(DWORD id)
{
	if(id == 0)
		return NULL;
	for(int i = 0;i < m_pSlaveList->Count; i ++)
	{
		SlaveCData* pData = (SlaveCData*)m_pSlaveList->Items[i];
		if(pData->m_dwID == id)
		{
			return pData;
		}
	}
	return NULL;
}

int SlaveCCenter::GetSlaveCnt(int nType)
{
	int nCount = 0;
	for(int i = 0;i < m_pSlaveList->Count; i ++)
	{
		SlaveCData* pData = (SlaveCData*)m_pSlaveList->Items[i];
		if(pData->m_nType == nType)
		{
			nCount ++;
		}
	}
	return nCount;
}

bool SlaveCCenter::CheckExcEnough(SlaveCData* pSlave,int* szRlt)
{
	memset(szRlt,0,4*8);
	if(pSlave == NULL)
		return false; 
	
	int i = 0;
	int n = 0;
	int nCnt = 0;
	for(std::map<int,int>::iterator itor = pSlave->m_pBasic->mapExchangeCost.begin();itor != pSlave->m_pBasic->mapExchangeCost.end();itor++)
	{
		if(n != itor->second)
		{
			i = 0;
			n = itor->second;
		}
		for(; i < m_pSlaveList->Count; i ++)
		{
			SlaveCData* pData = (SlaveCData*)m_pSlaveList->Items[i];
			if(pSlave == pData)
				continue;
			if(pData->m_nType == n)
			{
				szRlt[itor->first] = pData->m_dwID;
				nCnt ++;
				i++;
				break;
			}
		}
	}
	if(nCnt == pSlave->m_pBasic->mapExchangeCost.size())
		return true;
	return false;
}

bool SlaveCCenter::CheckCount()
{
	if(m_pSlaveList->Count >= MIN_SLAVE_CNT + m_byUnlockCnt)
		return false;
	return true;
}

void SlaveCCenter::ActiveSlave(DWORD id,DWORD mapid)
{
	if(m_pActiveSlave && m_pActiveSlave->m_dwID == id)
	{
		m_pActiveSlave->m_ifActive = false;
		m_pActiveSlave = NULL;
		return;
	}
	if(m_pActiveSlave)
	{
		m_pActiveSlave->m_ifActive = false;
		m_pActiveSlave = NULL;
	}
	SlaveCData* pData = GetSlave(id);
	if(pData)
	{
		pData->m_ifActive = true;
		m_pActiveSlave = pData;
		pData->m_dwMapID = mapid;
	}
}

void SlaveCCenter::Update()
{
	bool ifReflesh = false;
	if(m_dwReviveTick > 0 && m_dwReviveTick <= xnGetTickCount())
		m_dwReviveTick = 0;
	for(int i = m_pSlaveList->Count-1; i >= 0;i --)
	{
		SlaveCData* pData = (SlaveCData*)m_pSlaveList->Items[i];
		switch(pData->m_byState)
		{
		case SLAVE_STATE_NEW:
		case SLAVE_STATE_EXCHANGE:
			{
				pData->m_byState = SLAVE_STATE_NORMAL;
				pData->RecalParam();
				m_ifNumChange = true;
				ifReflesh = true;
			}
			break;
		case SLAVE_STATE_CHANGE:
			{
				pData->m_byState = SLAVE_STATE_NORMAL;
				pData->RecalParam();
			}
			break;
		case SLAVE_STATE_RELEASE:
			{
				m_pSlaveList->Delete(i);
				delete pData;
				m_ifNumChange = true;
				ifReflesh = true;
			}
			break;
		}
	}
	if(ifReflesh)
	{
		m_pSlaveList->Sort(PetSortBylv);
	}
}

bool SlaveCCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != JGSLAVE)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_ALL:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_ALL,pPacket);
			for(int i = 0;i < pRec->nCnt; i ++)
				AddSlave(&pRec->szInfo[i]);
		}
		return true;
	case S_SEND_INFO:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_INFO,pPacket);
			UpdateSlave(&pRec->Info);
		}
		return true;
	case S_SEND_EXP:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_EXP,pPacket);
			SlaveCData* pData = GetSlave(pRec->dwID);
			if(pData)
				pData->SetExp(pRec->byLevel,pRec->dwExp);
		}
		return true;
	case S_SEND_REMOVE:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_REMOVE,pPacket);
			RemoveSlave(pRec->id);
		}
		return true;
	case S_SEND_ACTIVE:
		{
			SET_DATA(pData,JGSLAVE,S_SEND_ACTIVE,pPacket);
			ActiveSlave(pData->dwID,pData->dwMapID);
		}
		return true;
	case S_REQUEST_GET:
		{
			SET_DATA(pData,JGSLAVE,S_REQUEST_GET,pPacket);
			pMainMenu->m_pShowPetGetMenu->ReFreshMenu();
			if (pData->nSID == 0)
			{
				pMainMenu->PopTipMsg("获得宠物失败～");
			}else{
				SlaveBasic* GetslaveInfo = SlaveBaseCfg::GetInstance().GetBaseInfo(pData->nSID);
				if (GetslaveInfo)
				{
					pMainMenu->m_pShowPetGetMenu->m_pGetSlaveList->Add(GetslaveInfo);
				}
				//pMainMenu->PopTipMsg("获得宠物：%s", GetslaveInfo->pName);
				pMainMenu->m_pShowPetGetMenu->Open();
			}
		}
		return true;
	case S_REQUEST_QUICKGET:
		{
			SET_DATA(pData,JGSLAVE,S_REQUEST_QUICKGET,pPacket);
			pMainMenu->m_pShowPetGetMenu->ReFreshMenu();
			for (int i = 0; i < 10; i++)
			{
				SlaveBasic* GetslaveInfo = SlaveBaseCfg::GetInstance().GetBaseInfo(pData->nSID[i]);
				if (GetslaveInfo)
				{
					pMainMenu->m_pShowPetGetMenu->m_pGetSlaveList->Add(GetslaveInfo);
				}
			}
			pMainMenu->m_pShowPetGetMenu->Open();
		}
		return true;
	}
	return false;
}

void SlaveCCenter::RequestActiveSlave(int byPos)
{

	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(m_pActiveSlave && m_pActiveSlave->m_nHp == 0)
	{
		pMainMenu->PopTipMsg("出战宠物正在复活中，请耐心等待");
		return;
	}
	if(pSlave->m_nLifePoint <= 0)
	{
		pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pSlave->m_dwID, 7);
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_ACTIVE,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_ACTIVE,pPacket);
	pData->dwID = pSlave->m_dwID;
	SendPacket(pPacket);
}

void SlaveCCenter::RequestRivive(int byPos)
{
	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_nLifePoint >= 250)
	{
		pMainMenu->PopTipMsg("宠物寿命已满，无法继续增加");
		return;
	}
	if(pChar->m_pItemCenter->GetItemCountByID(30017) <= 0)
	{
		ItemExBase* pIBase = ItemConfig::GetInstance().GetItemBase(30017);
		
		char szTemps[128];
		sprintf(szTemps,"%s不足，可通过“商城”购买获得，或者直接消耗一定金币增加宠物寿命。",pIBase->szName);
		pMainMenu->PopAskMsgBox(szTemps, GoMallEvent5,(void*)pSlave->m_dwID, 7);	
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_REVIVEPET,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_REVIVEPET,pPacket);
	pData->dwID = pSlave->m_dwID;
	SendPacket(pPacket);
}

void SlaveCCenter::RequestChangeName(int byPos,char* pName)
{
	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_nLifePoint <= 0)
	{
		pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pSlave->m_dwID, 7);	
		return;
	}
	if(pSlave->m_ifActive)
	{
		pMainMenu->PopTipMsg("宠物出战状态不允许改名");
		return;
	}
	if(pName == NULL)
	{
		pMainMenu->PopTipMsg("请输入名字");
		return;
	}
	int nLen = strlen(pName);
	if(nLen < 4 || nLen >10)
	{
		pMainMenu->PopTipMsg("名字长度请保持2-5个字");
		return;
	}
	if(RandNameConfig::GetInstance().CheckForbid(pName))
	{
		pMainMenu->PopTipMsg("名字内容不合规定");
		return;
	}

	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_CHANGENAME,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_CHANGENAME,pPacket);
	pData->dwID = pSlave->m_dwID;
	strncpy(pData->szName,pName,20);
	SendPacket(pPacket);
}

void SlaveCCenter::RequestRelease(int byPos)
{

	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_ifActive)
	{
		pMainMenu->PopTipMsg("宠物正在参战中，不能操作");
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_RELEASE,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_RELEASE,pPacket);
	pData->dwID = pSlave->m_dwID;
	SendPacket(pPacket);
}

void PetGoMallEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pActivityCtrl->Open(0);
}

void		SlaveCCenter::RequestLearnSkill(int byPos,int nItemID)
{
	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_nLifePoint <= 0)
	{
		pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pSlave->m_dwID, 7);
		return;
	}
	if(pSlave->m_ifActive)
	{
		pMainMenu->PopTipMsg("宠物正在参战中，不能操作");
		return;
	}
	ItemObj* pItem = pChar->m_pItemCenter->GetItemByIndex(nItemID);
	if(pItem == NULL || pItem->pItemBase->byType != ITEMTYPE_PETSKILL)
	{	
		pMainMenu->PopAskMsgBox("宠物技能书不足，可通过“商城”购买或者参加活动“钓鱼大赛”获得。",PetGoMallEvent,(void*)0, 5);
		return;
	}

	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_LEARN_SKILL,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_LEARN_SKILL,pPacket);
	pData->dwPetID = pSlave->m_dwID;
	pData->nItemID = nItemID;
	SendPacket(pPacket);
}

void GoMallEvent2(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pActivityCtrl->Open(0);
}

void		SlaveCCenter::RequestStrengthen(int byPos,BYTE bySel)
{
	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_nLifePoint <= 0)
	{
		pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pSlave->m_dwID, 7);
		return;
	}
	if(pSlave->m_ifActive)
	{
		pMainMenu->PopTipMsg("宠物正在参战中，不能操作");
		return;
	}
	int nLv = pSlave->m_nStrengthen[bySel];
	if(nLv >= 300)		//强化上限
	{
		pMainMenu->PopTipMsg("该强化属性已是最高级");
		return;
	}
	SlaveStreBasic* pStre = SlaveStrengthenCfg::GetInstance().GetBase(bySel,nLv + 1);
	if(pSlave->m_pBasic->byExchangLv < pStre->nLvNeed)	//等级不足
	{
		pMainMenu->PopTipMsg("需宠物进化等级达到%d级",pStre->nLvNeed);
		return;
	}
	if(pChar->m_dwMoney < pStre->nMoneyCost)		//银两不足
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	if(pChar->m_pItemCenter->GetItemCountByID(20004) < pStre->nItemCost)	//强化丹不足
	{
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(20004);		
		char szTemp[128];
		sprintf(szTemp,"%s不足，可通过“商城”购买或者参加活动“钓鱼大赛”获得。",pBase->szName);
		pMainMenu->PopAskMsgBox(szTemp, GoMallEvent2,(void*)0, 5);	
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_STRENGTHEN,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_STRENGTHEN,pPacket);
	pData->dwID = pSlave->m_dwID;
	pData->bySel = bySel;
	SendPacket(pPacket);
}

void		SlaveCCenter::RequestExchange(int byPos)
{
	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_nLifePoint <= 0)
	{
		pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pSlave->m_dwID, 7);
		return;
	}
	if(pSlave->m_ifActive)
	{
		pMainMenu->PopTipMsg("宠物正在参战中，不能操作");
		return;
	}
	if(pSlave->m_byLevel < pSlave->m_pBasic->byLvLimit)
	{
		pMainMenu->PopTipMsg("宠物等级需达到%d级",pSlave->m_pBasic->byLvLimit);
		return;
	}
	if(pSlave->m_pBasic->byExchangLv >= 3)
	{
		pMainMenu->PopTipMsg("该宠物已进化到最高级");
		return;
	}
	int szMoney[3] = {1000000,2000000,4000000};
	if(pChar->m_dwMoney < szMoney[pSlave->m_pBasic->byExchangLv])
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	int szRlt[8];
	if(!CheckExcEnough(pSlave,szRlt))
	{
		pMainMenu->PopTipMsg("材料不足");
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_EXCHANGE,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_EXCHANGE,pPacket);
	pData->dwID = pSlave->m_dwID;
	SendPacket(pPacket);
}

void		SlaveCCenter::RequestVariat(int byPos,int* szID,int nCnt)
{
	if(byPos == 0 || byPos > m_pSlaveList->Count)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	SlaveCData* pSlave = (SlaveCData*)m_pSlaveList->Items[byPos-1];
	if(pSlave->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("宠物不存在");
		return;
	}
	if(pSlave->m_nLifePoint <= 0)
	{
		pMainMenu->PopAskMsgBox("宠物寿命已耗尽，可通过“商城”购买生命之源或者消耗一定金币复活宠物。",GoMallEvent5,(void*)pSlave->m_dwID, 7);
		return;
	}
	if(pSlave->m_ifActive)
	{
		pMainMenu->PopTipMsg("宠物正在参战中，不能操作");
		return;
	}
	if(pSlave->m_byVariatlv >= SlaveVariatCfg::GetInstance().GetMaxLv())
	{
		pMainMenu->PopTipMsg("该宠物已达到最大变异等级");
		return;
	}
	if(nCnt == 0)
	{
		pMainMenu->PopTipMsg("请加入消耗的宠物");
		return;
	}
// 	if(pSlave->m_pBasic->byExchangLv == 0)
// 	{
// 		pMainMenu->PopTipMsg("未进化宠物不能进行变异");
// 		return;
// 	}
	SlaveVariatBasic* pNext = SlaveVariatCfg::GetInstance().GetBase(pSlave->m_byVariatlv+1);
	if(pNext == NULL)
	{
		pMainMenu->PopTipMsg("该宠物不能进行变异");
		return;
	}
	if(  pChar->m_dwMoney < pNext->nMoney)
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_VARIATION,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_VARIATION,pPacket);
	pData->dwID = pSlave->m_dwID;
	pData->nCnt = nCnt > 4 ? 4 : nCnt;
	for(int i = 0 ; i < pData->nCnt; i ++)
	{
		pData->szID[i] = szID[i];
	}
	SendPacket(pPacket);
}

void RechargeReturn1Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void SlaveCCenter::RequestGetPet(int nGetType)
{
	if (nGetType == 1&&(10 - pChar->m_nGetSlaveLmt <= 0))
	{
		pMainMenu->PopTipMsg("本日召唤次数已用完");
		return;
	}
	if (nGetType == 1&&(pChar->m_dwMoney < 10000)&&pChar->m_pClientFlag->GetFlag(12))
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	if ((nGetType == 2&&pChar->m_dwRmb < 50)||(nGetType == 3&&pChar->m_dwRmb < 100))
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn1Event,(void*)0,3);
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_GET,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_GET,pPacket);
	pData->nGetType = nGetType;
	SendPacket(pPacket);
}

void SlaveCCenter::RequestQuickGetPet(int nGetType)
{
	if (nGetType == 1&&(10 - pChar->m_nGetSlaveLmt < 10))
	{
		pMainMenu->PopTipMsg("召唤次数不足");
		return;
	}
	if (nGetType == 1&&(pChar->m_dwMoney < 100000))
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	if ((nGetType == 2&&(pChar->m_dwRmb < 400))||(nGetType == 3&&(pChar->m_dwRmb < 800)))
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn1Event,(void*)0,3);
		return;
	}
	pMainMenu->PlayEffect(275);
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_QUICKGET,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_QUICKGET,pPacket);
	pData->nGetType = nGetType;
	SendPacket(pPacket);
}