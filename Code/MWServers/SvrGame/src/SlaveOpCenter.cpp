#include "stdafx.h"
#include "SlaveOpCenter.h"
#include "SlaveData.h"
#include "HorseData.h"
#include "UserWealth.h"
#include "UserData.h"
#include "UserFlag.h"
#include "ios_yjgslave_packet.h"

CreateSingleton(SlaveOperation);
CreateSingleton(HorseOperation);

void	SlaveOperation::SetActiveSlave(SlaveCenter* pSC,int nID)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_nLifePoint <= 0)
		return;
	if(pSC->m_pActiveSlave == pData)		
	{
		pSC->m_pActiveSlave->m_ifActive = false;
		pSC->m_pActiveSlave = NULL;
		pSC->LeaveMap();
		pSC->SendActive();
		return;
	}
	if(pData->m_nHP <= 0)
	{
		pSC->m_pParent->SendPopMsg("宠物已死亡，无法出战");
		return;
	}
	if(pSC->CheckActiveState())
	{
		pSC->m_pParent->SendPopMsg("当前地图不能召唤宠物");
		return;
	}
	if(pSC->m_pActiveSlave)
	{
		pSC->LeaveMap();
		pSC->m_pActiveSlave->m_ifActive = false;
	}
	pSC->m_pActiveSlave = pData;
	pSC->m_pActiveSlave->m_ifActive = true;
	pSC->SetMapObj();
	pSC->SendActive();
}

void	SlaveOperation::ChangeSlaveName(SlaveCenter* pSC,int nID,char* szName)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_nLifePoint <= 0)
		return;
	if(pData->m_nHP <= 0)
	{
		pSC->m_pParent->SendPopMsg("宠物已死亡，无法改名");
		return;
	}
	if(szName == NULL)
		return;
	int nLen = strlen(szName);
	if(nLen < 4 || nLen >= 14)
		return;
	if(pData->m_pName)
		delete pData->m_pName;
	pData->m_pName = strdup(szName);
	pData->m_ifChange = true;
}

void	SlaveOperation::ReleaseSlave(SlaveCenter* pSC,int nID)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_ifActive)
		return;
	//pData->AddExp(999999);
	pSC->RemoveSlave(nID);
}

int		SlaveOperation::CheckPetSkill(SlaveBaseData* pPet,int nID,int nLv)
{
	SkillBase* pBase = SkillConfig::GetInstance().GetSkillBase(nID);
	if(pBase == NULL)
		return -1;
	if(nLv == 1)
	{
		int nCount = 0;
		for(int i = 0; i < SLAVE_SKILL_MAX-1; i ++)
		{
			if(pPet->m_pNumData[SLAVE_NUM_SKILL]->GetVal(i) > 0)
				nCount ++;
			if(pPet->m_pNumData[SLAVE_NUM_SKILL]->GetVal(i) == nID)
				return -2;
		}
		int nRlt = rand()%5;
		if(nRlt >= nCount)
			nRlt = nCount;
		return nRlt;
	}
	else if(nLv > 1)
	{
		for(int i = 0; i < SLAVE_SKILL_MAX-1; i ++)
		{
			if(pPet->m_pNumData[SLAVE_NUM_SKILL]->GetVal(i) == nID)
			{
				if(nLv-pPet->m_pNumData[SLAVE_NUM_SKILLLV]->GetVal(i) == 1)
					return i;
				else if(nLv <= pPet->m_pNumData[SLAVE_NUM_SKILLLV]->GetVal(i))
					return -2;
			}
		}
		return -1;
	}	
	else
		return -1;
}

void	SlaveOperation::ReviveSlave(SlaveCenter* pSC,int nID,bool bItem)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_nLifePoint >= 250)
		return;
	if (bItem)
	{
		if(pSC->m_pParent->m_pItemCenter->GetItemCountByID(30017) <= 0)
			return;
		pSC->m_pParent->m_pItemCenter->RemoveItemByID(30017);
	}else
	{
		if (pSC->m_pParent->m_xUWealth.GetWealth(UW_GOLD) < 500000)
		{
			pSC->m_pParent->SendWarnMsg("金币不足！金币低于50w。");
			return;
		}

		int nMoney = (rand()%5+1)*100000;
		if (pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,nMoney) == false)
			return;

		if (pSC->m_pParent->m_pSlaveCenter->m_pActiveSlave == NULL)
		{
			pSC->m_pParent->m_pSlaveCenter->m_pActiveSlave = pData;
			pSC->m_pParent->m_pSlaveCenter->SetMapObj();
			pSC->m_pParent->m_pSlaveCenter->SendActive();
			//pSC->m_pParent->m_pSlaveCenter->m_dwReviveTick = GetTickCount()+11000;
		}
	}

	int nPoint = rand()%5+1;
	pData->m_nLifePoint += nPoint;
	if(pData->m_nLifePoint > 250)
		pData->m_nLifePoint = 250;
	if(pData->m_nHP <= 0)
		pData->SetHp(pData->m_nParam[SLAVE_PARAM_HPMAX]);
	pData->m_ifChange = true;
	pSC->m_pParent->PlayEffect(nEffectSuccess);
}

void	SlaveOperation::LearnSlaveSkill(SlaveCenter* pSC,int nID, int nItemID)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_ifActive)
		return;
	if(pData->m_nLifePoint <= 0)
		return;
	if(pData->m_nHP <= 0)
	{
		pSC->m_pParent->SendPopMsg("宠物已死亡，无法学习技能");
		return;
	}
	ItemObj* pItem = pSC->m_pParent->m_pItemCenter->GetItemByIndex(nItemID);
	if(pItem == NULL)
		return;
	if(pItem->m_pBase->byType != ITEMTYPE_PETSKILL)
		return;
	//获得技能ID
	int nSkillID = pItem->m_pBase->nDura;
	int nSkillLv = pItem->m_pBase->byLv;
	int nRlt = CheckPetSkill(pData,nSkillID,nSkillLv);
	//end
	if(nRlt == -1)
	{
		pSC->m_pParent->SendPopMsg("技能学习需要学会上一级技能。");
		return;
	}
	else if(nRlt == -2)
	{
		pSC->m_pParent->SendPopMsg("该技能已学习。");
		return;
	}
	if(nRlt >= 0)
	{
		bool ifSuccess = true;
		if(nSkillLv > 1)
		{
			if((rand()%100) < 40)
				ifSuccess = false;
		}
		if(ifSuccess)
		{
			pData->m_pNumData[SLAVE_NUM_SKILL]->SetVal(nRlt,nSkillID);
			pData->m_pNumData[SLAVE_NUM_SKILLLV]->SetVal(nRlt,nSkillLv);
			pData->m_ifChange = true;
			//pSC->m_pParent->m_pItemCenter->RemoveItemByIndex(nItemID);
			pSC->m_pParent->PlayEffect(nEffectSuccess);
			pSC->m_pParent->m_pUserRecord->RunLuaPetFunc(2,pData);
		}
		else
			pSC->m_pParent->PlayEffect(nEffectFail);
		pSC->m_pParent->m_pItemCenter->RemoveItemByIndex(nItemID);
	}
}

void	SlaveOperation::StrengthenSlave(SlaveCenter* pSC,int nID,BYTE byPos)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
	{
		return;
	}
	if(pData->m_ifActive)
	{
		return;
	}
	if(pData->m_nLifePoint <= 0)
		return;
	if(pData->m_nHP <= 0)
	{
		pSC->m_pParent->SendPopMsg("宠物已死亡，无法强化");
		return;
	}
	if(byPos >= SLAVE_STRENGTHEN_MAX)
	{
		return;
	}
	int nLv = pData->m_pNumData[SLAVE_NUM_STRENGTHEN]->GetVal(byPos);
	if(nLv >= 300)		//强化上限
	{
		return;
	}
	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(pData->m_nType);
	SlaveStreBasic* pStre = SlaveStrengthenCfg::GetInstance().GetBase(byPos,nLv + 1);
	if(pBasic->byExchangLv < pStre->nLvNeed)	//进化等级不足
	{
		return;
	}
	if(pSC->m_pParent->m_xUWealth[UW_GOLD] < pStre->nMoneyCost)		//银两不足
	{
		return;
	}
	//强化丹20004
	if(pSC->m_pParent->m_pItemCenter->GetItemCountByID(20004) < pStre->nItemCost)	//强化丹不足
	{
		return;		
	}	
	if(pStre->nItemCost > 0)
		pSC->m_pParent->m_pItemCenter->RemoveItemByID(20004,pStre->nItemCost);
	if(rand()%100 < pStre->nSucPer)		//成功
	{
		pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,pStre->nMoneyCost);
		pData->m_pNumData[SLAVE_NUM_STRENGTHEN]->SetVal(byPos,nLv + 1);
		pData->m_ifChange = true;		
		pSC->m_pParent->PlayEffect(nEffectSuccess);
		pSC->m_pParent->m_pUserRecord->RunLuaPetFunc(3,pData);
		pSC->m_pParent->m_pUserRecord->AddSlaveLvLog(nID,pBasic->nID,byPos,nLv+1);
	}
	else
	{
		pSC->m_pParent->PlayEffect(nEffectFail);
	}
}

void	SlaveOperation::ExchangeSlave(SlaveCenter* pSC,int nID)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_ifActive)
		return;
	if(pData->m_nLifePoint <= 0)
		return;
	if(pData->m_nHP <= 0)
	{
		pSC->m_pParent->SendPopMsg("宠物已死亡，无法进化");
		return;
	}
	SlaveBasic* pSelf = SlaveBaseCfg::GetInstance().GetBaseInfo(pData->m_nType);
	if(pSelf == NULL)
		return;
	if(pData->m_byLevel < pSelf->byLvLimit)
		return;
	if(pSelf->byExchangLv >= 3)
		return;
	int szMoney[3] = {1000000,2000000,4000000};
	if(pSC->m_pParent->m_xUWealth[UW_GOLD] < szMoney[pSelf->byExchangLv])
		return;
	int nCnt;
	int szID[6];
	if(!CheckEnoughPet(pSC,pData,szID,nCnt))
		return;
	pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,szMoney[pSelf->byExchangLv]);
	for(int i = 0; i < nCnt; i ++)
		pSC->RemoveSlave(szID[i]);
	pData->m_nType = pSelf->nExchangeID;
	pData->m_ifChange = true;
	pSC->m_pParent->PlayEffect(nEffectSuccess);
	pSC->m_pParent->m_pUserRecord->AddSlaveLog(pData->m_dwID,pData->m_nType,pData->m_byLevel,pData->m_byVariatLv,SLAVE_SAVE_PET_EXCHANGE);
}

bool	SlaveOperation::CheckEnoughPet(SlaveCenter* pSC,SlaveBaseData* pPet,int* szID,int& nCnt)
{
	SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(pPet->m_nType);
	xnList*pCheckList = xnList::Create();
	for(std::map<int,int>::iterator itor = pBasic->mapExchangeCost.begin();itor != pBasic->mapExchangeCost.end();itor++)
	{
		pCheckList->Add((void*)itor->second);
	}
	
	nCnt = 0;
	for(int i = 0; i < pSC->m_pSlaveList->Count; i ++)
	{
		SlaveBaseData* pSlave = (SlaveBaseData*)pSC->m_pSlaveList->Items[i];
		if(pSlave->m_dwID == pPet->m_dwID)
			continue;
		if(pSlave->m_ifActive)
			continue;
		for(int k = pCheckList->Count - 1; k >= 0; k --)
		{
			if(pSlave->m_nType == (int)pCheckList->Items[k])
			{				
				pCheckList->Delete(k);
				szID[nCnt] = pSlave->m_dwID;
				nCnt ++;
				break;
			}
		}
		if(pCheckList->Count == 0)
			break;
	}
	if(pCheckList->Count == 0)
		return true;
	return false;
}

void	SlaveOperation::VariatSlave(SlaveCenter* pSC,int nID,int* szID,int nCnt)
{
	SlaveBaseData* pData = pSC->GetSlave(nID);
	if(pData == NULL)
		return;
	if(pData->m_ifActive)
		return;
	if(pData->m_nLifePoint <= 0)
		return;
	if(pData->m_nHP <= 0)
	{
		pSC->m_pParent->SendPopMsg("宠物已死亡，无法变异");
		return;
	}
	if(pData->m_byVariatLv >= SlaveVariatCfg::GetInstance().GetMaxLv())
		return;
	if(nCnt == 0)
		return;
	SlaveBasic* pSelf = SlaveBaseCfg::GetInstance().GetBaseInfo(pData->m_nType);
	if(pSelf == NULL)
		return;
	SlaveVariatBasic* pVa = SlaveVariatCfg::GetInstance().GetBase(pData->m_byVariatLv+1);
	if(pSC->m_pParent->m_xUWealth[UW_GOLD] < pVa->nMoney)
		return;
	int nSuc = 0;
	for(int i = 0; i < nCnt; i ++)
	{
		SlaveBaseData* pTempS = pSC->GetSlave(szID[i]);
		if(pTempS)
		{
			if(pTempS->m_ifActive || pTempS->m_nLifePoint <= 0 || pTempS->m_nHP <= 0)
			{
				if (pTempS->m_nLifePoint <= 0)
					pSC->m_pParent->SendWarnMsg("寿命为零的宠物，无法作为变异材料！");
				return;
			}
			SlaveBasic* pTempB = SlaveBaseCfg::GetInstance().GetBaseInfo(pTempS->m_nType);
			if(pTempB)
			{
				nSuc += pVa->nSucPer[pTempB->byQuality]; 
			}
		}
	}
	if(rand()%10000 < nSuc)
	{
		pData->m_byVariatLv ++;
		pData->m_ifChange = true;
		pSC->m_pParent->PlayEffect(nEffectSuccess);
		pSC->m_pParent->m_pUserRecord->AddSlaveLog(pData->m_dwID,pData->m_nType,pData->m_byLevel,pData->m_byVariatLv,SLAVE_SAVE_PET_VARIATY);
		pSC->m_pParent->m_pUserRecord->RunLuaPetFunc(4,pData);
	}
	else
	{
		pSC->m_pParent->PlayEffect(nEffectFail);
		pSC->m_pParent->m_pUserRecord->AddSlaveLog(pData->m_dwID,pData->m_nType,pData->m_byLevel,pData->m_byVariatLv,SLAVE_SAVE_PET_VFAIL);
	}
	pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,pVa->nMoney);
	for(int i = 0; i < nCnt; i ++)
		pSC->RemoveSlave(szID[i]);
	
}

void	SlaveOperation::GetSlave(SlaveCenter* pSC,int nGetType)
{
	if (nGetType == 1)
	{
		if (pSC->m_pParent->m_xUWealth[UW_GSLAVELMT] >= 10)
			return;
		else
		{
			if (!pSC->m_pParent->m_pUserFlag->GetFlag(12))
			{
				pSC->m_pParent->m_pUserFlag->SetFlag(12,1);
			}else
			{
				if(pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,10000)==false)
					return;

				pSC->m_pParent->m_xUWealth.AddWealth(UW_GSLAVELMT,1);
				
			}
		}
	}else if (nGetType == 2)
	{
		if(pSC->m_pParent->m_xUWealth.SubWealth(UW_RMB,50,RMB_SAVE_SLAVE_GET) == false)
			return;
	}else if (nGetType == 3)
	{
		if(pSC->m_pParent->m_xUWealth.SubWealth(UW_RMB,100,RMB_SAVE_SLAVE_GET) == false)
			return;
	}

	int nGetID = SlaveBaseCfg::GetInstance().RandSlaveID(nGetType,pSC->m_pParent);
	if (nGetID)
	{
		pSC->AddNewSlave(nGetID);
	}
	pSC->SendGet(nGetID);
}

void	SlaveOperation::QuickGetSlave(SlaveCenter* pSC,int nGetType)
{
	if(nGetType < 1 || nGetType > 3)
		return;
	if (nGetType == 1)
	{
		if (pSC->m_pParent->m_xUWealth[UW_GSLAVELMT] > 0)
			return;
		else if (pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,100000))
		{
			pSC->m_pParent->m_xUWealth.AddWealth(UW_GSLAVELMT,10);
		}else
			return;
	}else if (nGetType == 2&&!pSC->m_pParent->m_xUWealth.SubWealth(UW_RMB,400,RMB_SAVE_SLAVE_GET))
		return;
	else if (nGetType == 3&&!pSC->m_pParent->m_xUWealth.SubWealth(UW_RMB,800,RMB_SAVE_SLAVE_GET))
		return;

	int nID[10] = {0};
	int nCnt = 0;
	while(nCnt < 10)
	{
		int nGetID = SlaveBaseCfg::GetInstance().RandSlaveID(nGetType,pSC->m_pParent);
		if (nGetID)
		{
			nID[nCnt] = nGetID;
			pSC->AddNewSlave(nGetID);
		}
		nCnt++;
	}
	//for (int i = 0; i < 10; i++)
	//{
	//	int nGetID = SlaveBaseCfg::GetInstance().RandSlaveID(nGetType,pSC->m_pParent);
	//	if (nGetID)
	//	{
	//		nID[i] = nGetID;
	//		pSC->AddNewSlave(nGetID);
	//	}
	//}
	pSC->SendQuickGet(nID);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void	HorseOperation::EquipHorse(HorseCenter* pSC,int nID)
{
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	if(pSC->m_pEquipHorse == pData)		
	{
		pSC->m_pEquipHorse->m_ifEquip = false;
		pSC->m_pEquipHorse = NULL;
		pSC->m_ifEquipChange = true;
		return;
	}
	if(pSC->m_pEquipHorse)
		pSC->m_pEquipHorse->m_ifEquip = false;
	pSC->m_pEquipHorse = pData;
	pSC->m_pEquipHorse->m_ifEquip = true;
	pSC->m_ifEquipChange = true;
}

void	HorseOperation::FeedHorse(HorseCenter* pSC,int nID,bool isSenior,int nCnt)
{
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(pData->m_nType);
	if(pBasic == NULL)
		return;
	if(pData->m_byLevel >= pBasic->byLvLimit)
		return;
	if(nCnt == 0)
		return;
	int nItem = 20002;
	int nPoint = 5;
	if(isSenior)
	{
		nItem = 20003;
		nPoint = 20;
	}
	if(pSC->m_pParent->m_pItemCenter->GetItemCountByID(nItem) < nCnt)
		return;

	
	pSC->m_pParent->m_pItemCenter->RemoveItemByID(nItem,nCnt);
	int nLv = pData->m_byLevel;
	pData->AddExp(nPoint*nCnt);
	if(pData->m_byLevel != nLv)
	{
		pSC->m_pParent->PlayEffect(nEffectSuccess);
	}
}

void	HorseOperation::EatHorse(HorseCenter* pSC,int nID,int* szID,int nCnt)
{
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(pData->m_nType);
	if(pBasic == NULL)
		return;
	if(pData->m_byLevel >= pBasic->byLvLimit)
		return;
	if(nCnt == 0)
		return;
	int nExp = 0;
	int nCulAdd = 0;
	for(int i = 0; i < nCnt;i ++)
	{
		HorseBaseData* pFood = pSC->GetHorse(szID[i]);
		if(pFood)
		{
			HorseBasic* pFBasic = HorseBaseCfg::GetInstance().GetBasic(pFood->m_nType);
			if(pFBasic)
			{
				int nTemp = HorseBaseCfg::GetInstance().GetFeedExp(pFood->m_byLevel,pFood->m_nExp);
				switch(pFBasic->byQuality)
				{
				case 1:
					{
						nTemp = nTemp * 70 / 100;
					}
					break;
				case 2:
					{
						nTemp = nTemp * 90 / 100;
						if (pFBasic->nID == 1002)
						{
							nCulAdd += 50;
							pData->m_nEatCulLimt+=50;
						}
					}
					break;
				case 3:
					{
						if (pData->m_nEatCulLimt < 3000)
						{
							nCulAdd += 100;
							pData->m_nEatCulLimt+=100;
						}
					}
					break;
				default:
					{
						nTemp = nTemp * 50/100;
					}
					
					break;
				}
				if(nTemp == 0)
					nTemp = 1;
				nExp += nTemp;
			}
		
		}
	}
	if(nExp == 0)
		return;
	pData->AddExp(nExp);
	if(nCulAdd > 0)
		pData->m_pNumData[HORSE_NUM_CULTIVATE]->AddVal(HORSE_CULTIVATE_TOTAL,nCulAdd);
	pData->m_ifChange = true;
	for(int i = 0; i < nCnt; i ++)
		pSC->RemoveHorse(szID[i]);
	pSC->m_pParent->PlayEffect(nEffectSuccess);
	pSC->m_pParent->m_pUserRecord->RunLuaHorseFunc(3,pData,nCnt);
}

void	HorseOperation::CultivateHorse(HorseCenter* pSC,int nID,BYTE  byType)
{
	if (byType<0||byType>1)
		return;
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	int nCostMoney = 10000;
	int nCostRmb = 4;
	int nVal = pData->m_pNumData[HORSE_NUM_CULTIVATE]->GetVal(HORSE_CULTIVATE_TOTAL);
	if(nVal == 0)
		return;
	if(pData->m_szTempCul[HORSE_CULTIVATE_TOTAL] != 0)
		return;
	if(pSC->m_pParent->m_pItemCenter->GetItemCountByID(20006) < 4)
		return;
	if(byType == 0 && pSC->m_pParent->m_xUWealth[UW_GOLD] < nCostMoney)
		return;
	else if(byType == 1 && pSC->m_pParent->m_xUWealth[UW_RMB] < nCostRmb)
		return;

	if(byType == 0)
		pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,nCostMoney);
	else if(byType == 1)
		pSC->m_pParent->m_xUWealth.SubWealth(UW_RMB,nCostRmb,RMB_SAVE_HORSE_CULTIVATE);

	pSC->m_pParent->m_pItemCenter->RemoveItemByID(20006,4);

	pData->RecalParam();

	int nTotalVal = 0;
	int nMax = 0;
	int nMin = 0;
	if (byType == 0)
	{
		nTotalVal = rand()%2+1;		//消耗潜力1~2
		nMax = 4;
		nMin = -3;
	}
	else if(byType == 1)
	{
		nTotalVal = rand()%3+3;		//消耗潜力3~5
		nMax = 8;
		nMin = -2;
	}

	if(nTotalVal > nVal)
		nTotalVal = nVal;
	int szRlt[4] = {0};
	szRlt[0] = -rand()%(-nMin)-1;
	szRlt[1] = -rand()%(-nMin)-1;
	int nT2= nTotalVal-szRlt[0]-szRlt[1];
	if(nT2 == 1)
	{
		szRlt[2] = rand()%nT2;
	}
	else if(nT2 <= nMax)
	{
		szRlt[2] = rand()%(nT2-1)+1;
	}
	else
	{
		szRlt[2] = rand()%(2*nMax-nT2+1)+(nT2-nMax);
	}
	szRlt[3] = nT2 - szRlt[2];
	//打乱排序
	for(int i = 0; i < 4; i ++)
	{
		int j = rand()%4;
		if(i != j)
		{
			int nTem = szRlt[i];
			szRlt[i] = szRlt[j];
			szRlt[j] = nTem;
		}
	}
	//随机赋值
	int nNeed = 4;
	for(int i = HORSE_CULTIVATE_HP; i < HORSE_CULTIVATE_TOTAL; i++)
	{
		if(nNeed == 0)
			break;
		if(rand()%(HORSE_CULTIVATE_TOTAL-i) < nNeed)
		{
			pData->m_szTempCul[i] = szRlt[nNeed-1];
			nNeed --;
		}
	}

	//换值，保证结果均不小于0
	for(int i = HORSE_CULTIVATE_HP; i < HORSE_CULTIVATE_TOTAL; i++)
	{
		if(pData->m_nParam[i] + pData->m_szTempCul[i] < 0)
		{
			for(int j = HORSE_CULTIVATE_HP; j < HORSE_CULTIVATE_TOTAL; j++)
			{
				if(j == i)
					continue;
				if((pData->m_nParam[i] + pData->m_szTempCul[j] >= 0) &&(pData->m_nParam[j] + pData->m_szTempCul[i] >= 0))
				{
					int nTemp = pData->m_szTempCul[i];
					pData->m_szTempCul[i] = pData->m_szTempCul[j];
					pData->m_szTempCul[j] = nTemp;
				}
			}
		}
	}

	pData->m_szTempCul[HORSE_CULTIVATE_TOTAL] = -nTotalVal;
	pData->m_ifSendCul = true;
}

void	HorseOperation::CultivateHorseEx(HorseCenter* pSC,int nID,BYTE  byType)
{
	if (byType<2||byType>3)
		return;
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	int nCostMoney = 100000;
	int nCostRmb = 40;
	int nVal = pData->m_pNumData[HORSE_NUM_CULTIVATE]->GetVal(HORSE_CULTIVATE_TOTAL);
	if(nVal/10 == 0)
		return;
	if(pData->m_szTempCul[HORSE_CULTIVATE_TOTAL] != 0)
		return;
	if(pSC->m_pParent->m_pItemCenter->GetItemCountByID(20006) < 40)
		return;

    if(byType == 2 && pSC->m_pParent->m_xUWealth[UW_GOLD] < nCostMoney)
		return;
	else if(byType == 3 && pSC->m_pParent->m_xUWealth[UW_RMB] < nCostRmb)
		return;

	if(byType == 2)
		pSC->m_pParent->m_xUWealth.SubWealth(UW_GOLD,nCostMoney);
	else if(byType == 3)
		pSC->m_pParent->m_xUWealth.SubWealth(UW_RMB,nCostRmb,RMB_SAVE_HORSE_CULTIVATE);

	pSC->m_pParent->m_pItemCenter->RemoveItemByID(20006,40);

	pData->RecalParam();

	int nTotalVal = 0;
	int nMax = 0;
	int nMin = 0;
	if (byType == 2)
	{
		nTotalVal = rand()%2+1;		//消耗潜力10~20
		nMax = 4;
		nMin = -3;
	}
	else if(byType == 3)
	{
		nTotalVal = rand()%3+3;		//消耗潜力30~50
		nMax = 8;
		nMin = -2;
	}

	if(nTotalVal > nVal/10)
		nTotalVal = nVal/10;
	int szRlt[4] = {0};
	szRlt[0] = -rand()%(-nMin)-1;
	szRlt[1] = -rand()%(-nMin)-1;
	int nT2= nTotalVal-szRlt[0]-szRlt[1];
	if(nT2 == 1)
	{
		szRlt[2] = rand()%nT2;
	}
	else if(nT2 <= nMax)
	{
		szRlt[2] = rand()%(nT2-1)+1;
	}
	else
	{
		szRlt[2] = rand()%(2*nMax-nT2+1)+(nT2-nMax);
	}
	szRlt[3] = nT2 - szRlt[2];
	//打乱排序
	for(int i = 0; i < 4; i ++)
	{
		int j = rand()%4;
		if(i != j)
		{
			int nTem = szRlt[i];
			szRlt[i] = szRlt[j];
			szRlt[j] = nTem;
		}
	}
	//随机赋值
	int nNeed = 4;
	for(int i = HORSE_CULTIVATE_HP; i < HORSE_CULTIVATE_TOTAL; i++)
	{
		if(nNeed == 0)
			break;
		if(rand()%(HORSE_CULTIVATE_TOTAL-i) < nNeed)
		{
			pData->m_szTempCul[i] = szRlt[nNeed-1]*10;
			nNeed --;
		}
	}

	//换值，保证结果均不小于0
	for(int i = HORSE_CULTIVATE_HP; i < HORSE_CULTIVATE_TOTAL; i++)
	{
		if(pData->m_nParam[i] + pData->m_szTempCul[i] < 0)
		{
			for(int j = HORSE_CULTIVATE_HP; j < HORSE_CULTIVATE_TOTAL; j++)
			{
				if(j == i)
					continue;
				if((pData->m_nParam[i] + pData->m_szTempCul[j] >= 0) &&(pData->m_nParam[j] + pData->m_szTempCul[i] >= 0))
				{
					int nTemp = pData->m_szTempCul[i];
					pData->m_szTempCul[i] = pData->m_szTempCul[j];
					pData->m_szTempCul[j] = nTemp;
				}
			}
		}
	}

	pData->m_szTempCul[HORSE_CULTIVATE_TOTAL] = -nTotalVal*10;
	pData->m_ifSendCul = true;
}

void	HorseOperation::CultivateHorseRlt(HorseCenter* pSC,int nID,bool ifSure)
{
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	if(pData->m_szTempCul[HORSE_CULTIVATE_TOTAL] == 0)
		return;
	if(!ifSure)
	{
		memset(pData->m_szTempCul,0,4*HORSE_CULTIVATE_MAX);
		pData->m_ifSendCul = true;
		return;
	}
	for(int i = HORSE_CULTIVATE_HP; i < HORSE_CULTIVATE_MAX; i ++)
	{
		pData->m_pNumData[HORSE_NUM_CULTIVATE]->AddVal(i,pData->m_szTempCul[i]);
	}
	memset(pData->m_szTempCul,0,4*HORSE_CULTIVATE_MAX);
	pData->m_ifSendCul = true;
	pData->m_ifChange = true;
	pSC->m_pParent->PlayEffect(nEffectSuccess);
}

void	HorseOperation::ReleaseHorse(HorseCenter* pSC,int nID)
{
	HorseBaseData* pData = pSC->GetHorse(nID);
	if(pData == NULL)
		return;
	if(pData->m_ifEquip)
		return;
	pSC->RemoveHorse(nID);
}

void	HorseOperation::RideHorse(HorseCenter* pSC,bool ifOn)
{
	if(pSC == NULL || pSC->m_pEquipHorse == NULL)
		return;
	if(pSC->m_ifRide != ifOn)
	{
		pSC->m_ifRide =	ifOn;
		pSC->m_ifRideChange = true;
	}
}