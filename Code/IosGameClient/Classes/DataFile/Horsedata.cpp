#include "HorseData.h"
#include "MainMenu.h"
#include "MapAIObj.h"
#include "MapControl.h"

HorseCData::HorseCData()
{
	memset(m_nParam,0,HORSE_PARAM_MAX*4);
	memset(m_nPotential,0,HORSE_CULTIVATE_MAX*4);
	memset(m_szTempCul,0,4*HORSE_CULTIVATE_MAX);
	m_pBasic = NULL;
	m_pSkinObj= NULL;
	m_pSkinObjNext=NULL;
	m_byState = SLAVE_STATE_NORMAL;
	m_ifEquip = false;
	m_nSkin = 0;
}

HorseCData::~HorseCData()
{

}

void HorseCData::RecalParam()
{
	memset(m_nParam,0,4*HORSE_PARAM_MAX);
	m_pBasic = HorseBaseCfg::GetInstance().GetBasic(m_nType);
	if(m_pBasic != NULL)
	{
		m_nSkin = m_pBasic->nSkin;
		int nNextSkin = m_pBasic->szUpSkin[0];
		if(m_byLevel >= 70)
		{
			m_nSkin = m_pBasic->szUpSkin[2];
			nNextSkin = m_pBasic->szUpSkin[2];
		}
		else if(m_byLevel >= 50)
		{
			m_nSkin = m_pBasic->szUpSkin[1];
			nNextSkin = m_pBasic->szUpSkin[2];
		}
		else if(m_byLevel >= 30)
		{
			m_nSkin = m_pBasic->szUpSkin[0];
			nNextSkin = m_pBasic->szUpSkin[1];
		}
		m_pSkinObj = CfgObjSkin::GetInstance().GetObjSkin(m_nSkin);
		m_pSkinObjNext = CfgObjSkin::GetInstance().GetObjSkin(nNextSkin);
		for(int i = 0; i < HORSE_PARAM_MAX; i ++)
		{
			m_nParam[i] += m_pBasic->szBaseParam[i];
			m_nParam[i] += (m_byLevel-1)*m_pBasic->szBaseUp[i];
			m_nParam[i] += m_nPotential[i];
			if(m_nParam[i] < 0)
				m_nParam[i] = 0;
		}
	}
	else
	{
		m_pBasic = NULL;
		m_pSkinObj	= NULL;
		m_pSkinObjNext = NULL;
	}
}

void HorseCData::SetExp(BYTE byLevel,int nExp)
{
	m_nExp = nExp;
	m_byLevel = byLevel;
	m_byState = SLAVE_STATE_CHANGE;
}

int HorseCData::AddExp(int nExp,bool ifReal)
{
	HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(m_nType);
	if(pBasic == NULL)
		return m_byLevel;
	BYTE byNowLv = m_byLevel;
	int nNowExp = m_nExp;
	int nTotalExp = nExp;
	int nCount = 0;
	while(nTotalExp > 0)
	{
		if(nCount > 100)
			break;
		if(byNowLv >= pBasic->byLvLimit)
		{
			nTotalExp = 0;
			break;
		}
		int nLimit = HorseBaseCfg::GetInstance().GetLvExp(byNowLv);
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
	if(nCount > 0 && ifReal)
	{
		m_byLevel = byNowLv;
		m_nExp = nNowExp;
		m_byState = SLAVE_STATE_CHANGE;
	}
	return byNowLv;
}

HorseCCenter::HorseCCenter()
{
	m_pHorseList = xnList::Create();
	m_ifNumChange = false;
	m_pEquipHorse = NULL;
	m_ifRide = false;
	m_pFPacket = NULL;
}

HorseCCenter::~HorseCCenter()
{
	if(m_pHorseList)
	{
		while(m_pHorseList->Count)
			delete (HorseCData*)m_pHorseList->Delete(0);
		m_pHorseList->Free();
	}
}

void HorseCCenter::ClearList()
{
	m_ifRide = false;
	m_pEquipHorse = NULL;
	while(m_pHorseList->Count)
		delete (HorseCData*)m_pHorseList->Delete(0);
	m_ifNumChange = true;
}

bool HorseCCenter::AddHorse(HorseSendInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	HorseCData* pData = new HorseCData();
	pData->m_dwUin = pInfo->dwUin;
	pData->m_dwID = pInfo->dwID;
	pData->m_byLevel = pInfo->byLevel;
	pData->m_nExp = pInfo->dwExp;
	pData->m_bySlot = pInfo->bySlot;
	pData->m_nType = pInfo->nType;
	pData->m_nEatLimt = pInfo->nEatLimt;
	memcpy(pData->m_nPotential,pInfo->nPotential,4*HORSE_CULTIVATE_MAX);
	pData->m_pName = strdup(pInfo->szName);
	pData->m_byState = SLAVE_STATE_NEW;
	m_pHorseList->Add((void*)pData);
	return true;
}

bool HorseCCenter::UpdateHorse(HorseSendInfo* pInfo)
{
	if(pInfo == NULL)
		return false;
	for(int i = 0;i < m_pHorseList->Count; i ++)
	{
		HorseCData* pData = (HorseCData*)m_pHorseList->Items[i];
		if(pData->m_dwID == pInfo->dwID)
		{
			pData->m_byLevel = pInfo->byLevel;
			pData->m_nExp = pInfo->dwExp;
			pData->m_bySlot = pInfo->bySlot;
			pData->m_nType = pInfo->nType;
			pData->m_nEatLimt = pInfo->nEatLimt;
			memcpy(pData->m_nPotential,pInfo->nPotential,4*HORSE_CULTIVATE_MAX);
			pData->m_pName = strdup(pInfo->szName);
			pData->m_byState = SLAVE_STATE_CHANGE;
			return true;
		}
	}
	return AddHorse(pInfo);
}

void HorseCCenter::RemoveHorse(DWORD id)
{
	for(int i = 0; i < m_pHorseList->Count; i ++)
	{
		HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[i];
		if(pHorse->m_dwID == id)
		{
			pHorse->m_byState = SLAVE_STATE_RELEASE;
			return;
		}
	}
}

HorseCData* HorseCCenter::GetHorse(DWORD id)
{
	for(int i = 0; i < m_pHorseList->Count; i ++)
	{
		HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[i];
		if(pHorse->m_dwID == id)
		{
			return pHorse;
		}
	}
	return NULL;
}

void HorseCCenter::EquipHorse(DWORD id)
{
	HorseCData* pData = GetHorse(id);

	if(m_pEquipHorse && (m_pEquipHorse == pData || id == 0))	
	{
		m_pEquipHorse->m_ifEquip = false;
		m_pEquipHorse = NULL;
		return;
	}	
	if(pData == NULL)
		return;
	if(m_pEquipHorse)
		m_pEquipHorse->m_ifEquip = false;
	m_pEquipHorse = pData;
	m_pEquipHorse->m_ifEquip = true;
}

void HorseCCenter::Update()
{
	for(int i = m_pHorseList->Count-1; i >= 0; i --)
	{
		HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[i];
		if(pHorse->m_byState == SLAVE_STATE_NEW)
		{
			pHorse->m_byState = SLAVE_STATE_NORMAL;
			pHorse->RecalParam();
			m_ifNumChange = true;
		}
		else if(pHorse->m_byState == SLAVE_STATE_CHANGE)
		{
			pHorse->m_byState = SLAVE_STATE_NORMAL;
			pHorse->RecalParam();
		}
		else if(pHorse->m_byState == SLAVE_STATE_RELEASE)
		{
			m_pHorseList->Delete(i);
			delete pHorse;
			m_ifNumChange = true;
		}
	}
}

bool HorseCCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != JGSLAVE)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_ALLHORSE:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_ALLHORSE,pPacket);
			for(int i = 0;i < pRec->nCnt; i ++)
				AddHorse(&pRec->szInfo[i]);
		}
		return true;
	case S_SEND_HORSEINFO:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_HORSEINFO,pPacket);
			UpdateHorse(&pRec->Info);
		}
		return true;
	case S_SEND_HORSEEXP:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_HORSEEXP,pPacket);
			HorseCData* pData = GetHorse(pRec->dwID);
			if(pData)
				pData->SetExp(pRec->byLevel,pRec->dwExp);
		}
		return true;
	case S_SEND_REMOVEHORSE:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_REMOVEHORSE,pPacket);
			RemoveHorse(pRec->id);
		}
		return true;
	case S_SEND_EQUIP:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_EQUIP,pPacket);
			EquipHorse(pRec->dwID);
			m_ifRide = pRec->ifRide;
		}
		return true;
	case S_SEND_RIDE:
		{
			SET_DATA(pData,JGSLAVE,S_SEND_RIDE,pPacket);
			m_ifRide = pData->ifRide;
		}
		return true;
	case S_SEND_CULCHANGE:
		{
			SET_DATA(pRec,JGSLAVE,S_SEND_CULCHANGE,pPacket);
			HorseCData* pData = GetHorse(pRec->dwID);
			if(pData)
				memcpy(pData->m_szTempCul,pRec->szVal,4*HORSE_CULTIVATE_MAX);
		}
		return true;
	}
	return false;
}

void GoMallEvent3(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pActivityCtrl->Open(0);
}

void HorseCCenter::RequestEquipHorse(int byPos)
{
	if(byPos == 0 || byPos > m_pHorseList->Count)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[byPos-1];
	if(pHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_EQUIP,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_EQUIP,pPacket);
	pData->dwID = pHorse->m_dwID;
	SendPacket(pPacket);
}

void MsgboxFeedhorse(void* param,int nVal)
{
	HorseCCenter* pTemp = (HorseCCenter*)param;
	if(pTemp && pTemp->m_pFPacket)
	{
		if(nVal == 1)
			pTemp->SendPacket(pTemp->m_pFPacket);
		delete pTemp->m_pFPacket;
		pTemp->m_pFPacket = NULL;
	}
}

void HorseCCenter::RequestFeedHorse(int byPos,bool ifSenior,int nCnt)
{
	if(byPos == 0 || byPos > m_pHorseList->Count)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[byPos-1];
	if(pHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}

	if(pHorse->m_byLevel >= pHorse->m_pBasic->byLvLimit)
	{
		pMainMenu->PopTipMsg("该坐骑已达到最高级");
		return;
	}
	if(pHorse->m_byLevel >= pChar->m_byLvl + 5)
	{
		pMainMenu->PopTipMsg("坐骑等级不能超过人物等级5级");
		return;
	}
	int nItem = 20002;
	int nPoint = 5;
	if(ifSenior)
	{
		nItem = 20003;
		nPoint = 20;
	}
	if(pChar->m_pItemCenter->GetItemCountByID(nItem) < nCnt || nCnt == 0)
	{
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nItem);
		if (pBase->nTypeID == 20002)
		{
			char szTemp[128];
			sprintf(szTemp,"%s不足，可通过“商城”坐骑界面购买或者参加活动“护送商队”获得。",pBase->szName);
			pMainMenu->PopAskMsgBox(szTemp, GoMallEvent3,(void*)0, 5);	
		}else	if (pBase->nTypeID == 20003)
		{
			char szTemp[128];
			sprintf(szTemp,"%s不足，可通过“商城”坐骑界面购买或者参加活动“幸运大转盘”获得。",pBase->szName);
			pMainMenu->PopAskMsgBox(szTemp, GoMallEvent3,(void*)0, 5);	
		}else
			pMainMenu->PopTipMsg("%s不足",pBase->szName);
		return;
	}
	BYTE* buf = new BYTE[1024];
	BuildPacketSize(m_pFPacket,JGSLAVE,C_REQUEST_FEED,buf,sizeof(JGSLAVE_C_REQUEST_FEED_Data));
	SET_DATA(pData,JGSLAVE,C_REQUEST_FEED,m_pFPacket);
	pData->dwID = pHorse->m_dwID;
	pData->ifSenior = ifSenior;
	pData->nCnt = nCnt;

	int nNextLv = pHorse->AddExp(nPoint*nCnt,false);
	if(nNextLv >= pChar->m_byLvl + 5)
	{
		pMainMenu->PopAskMsgBox("坐骑等级不能超过人物等级5级，超过的经验将不会保留，是否继续？",MsgboxFeedhorse,this);
		return;
	}
	else
	{
		SendPacket(m_pFPacket);
		if(m_pFPacket)
		{
			delete m_pFPacket;
			m_pFPacket = NULL;
		}
	}
}

void RechargeReturn5Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void HorseCCenter::RequestCultivateHorse(int byPos,BYTE  byType)
{
	if (byType < 0||byType>3)
		return;
	if(byPos == 0 || byPos > m_pHorseList->Count)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[byPos-1];
	if(pHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	if((byType < 2&&pHorse->m_nPotential[HORSE_CULTIVATE_TOTAL] == 0)||(byType > 1&&pHorse->m_nPotential[HORSE_CULTIVATE_TOTAL] < 10))
	{
		pMainMenu->PopTipMsg("潜力值不足");
		return;
	}
	if(pHorse->m_szTempCul[HORSE_CULTIVATE_TOTAL] != 0)
	{
		pMainMenu->PopTipMsg("请选择是否接受此次培养");
		return;
	}
	if(byType == 0 && pChar->m_dwMoney < 10000||byType == 2 && pChar->m_dwMoney < 100000)
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	else if(byType == 1 && pChar->m_dwRmb < 4||byType == 3 && pChar->m_dwRmb < 40)
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn5Event,(void*)0,3);
		return;
	}

	if((byType < 2&&pChar->m_pItemCenter->GetItemCountByID(20006) < 4)||(byType >1&&pChar->m_pItemCenter->GetItemCountByID(20006) < 40))
	{
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(20006);
		char szTemp[128];
		sprintf(szTemp,"%s不足，可通过“商城”坐骑界面购买或者参加活动“护送商队”获得。",pBase->szName);
		pMainMenu->PopAskMsgBox(szTemp, GoMallEvent3,(void*)0, 5);			
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_CULTIVATE,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_CULTIVATE,pPacket);
	pData->dwID = pHorse->m_dwID;
	pData->byType = byType;
	SendPacket(pPacket);
}

void HorseCCenter::RequestCultivateRlt(int byPos,bool ifSure)
{
	if(byPos == 0 || byPos > m_pHorseList->Count)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[byPos-1];
	if(pHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	if(pHorse->m_szTempCul[HORSE_CULTIVATE_TOTAL] == 0)
	{
		pMainMenu->PopTipMsg("请先对该坐骑进行培养");
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_CULTIVATE_RLT,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_CULTIVATE_RLT,pPacket);
	pData->dwID = pHorse->m_dwID;
	pData->ifSure = ifSure;
	SendPacket(pPacket);
}

void HorseCCenter::RequestEatHorse(int byPos,int* szID,int nCnt)
{
	if(byPos == 0 || byPos > m_pHorseList->Count)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[byPos-1];
	if(pHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	if(nCnt == 0)
	{
		pMainMenu->PopTipMsg("请先选择食用的坐骑");
		return;
	}
	BYTE* buf = new BYTE[1024];
	BuildPacketSize(m_pFPacket,JGSLAVE,C_REQUEST_EAT,buf,sizeof(JGSLAVE_C_REQUEST_EAT_Data));
	SET_DATA(pData,JGSLAVE,C_REQUEST_EAT,m_pFPacket);
	pData->dwID = pHorse->m_dwID;
	pData->nCnt = nCnt > 4 ? 4 : nCnt;
	memcpy(pData->szID,szID,4*4);

	int nTotalExp = 0;
	for(int i = 0; i < pData->nCnt; i ++)
	{
		HorseCData* pH = pChar->m_pHorseCenter->GetHorse(pData->szID[i]);
		if(pH)
			nTotalExp += HorseBaseCfg::GetInstance().GetFeedExp(pH->m_byLevel,pH->m_nExp);
	}
	int nNextLv = pHorse->AddExp(nTotalExp,false);
	if(nNextLv >= pChar->m_byLvl + 5)
	{
		pMainMenu->PopAskMsgBox("坐骑等级不能超过人物等级5级，超过的经验将不会保留，是否继续？",MsgboxFeedhorse,this);
		return;
	}
	else
	{
		SendPacket(m_pFPacket);
		if(m_pFPacket)
		{
			delete m_pFPacket;
			m_pFPacket = NULL;
		}
	}
}

void HorseCCenter::RequestReleaseHorse(int byPos)
{

	if(byPos == 0 || byPos > m_pHorseList->Count)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	HorseCData* pHorse = (HorseCData*)m_pHorseList->Items[byPos-1];
	if(pHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		pMainMenu->PopTipMsg("坐骑不存在");
		return;
	}
	if(pHorse->m_ifEquip)
	{
		pMainMenu->PopTipMsg("装备中的坐骑，不能操作");
		return;
	}
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_RELEASE_HORSE,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_RELEASE_HORSE,pPacket);
	pData->dwID = pHorse->m_dwID;
	SendPacket(pPacket);
}

void HorseCCenter::RequestRide(bool ifOn, bool bWarn)
{
	if(m_pEquipHorse == NULL || m_pEquipHorse->m_byState == SLAVE_STATE_RELEASE)
	{
		if(ifOn && bWarn)
			pMainMenu->PopTipMsg("未装备坐骑");
		return;
	}
	if(ifOn && pChar->m_szbyActLock[6])
	{
		pMainMenu->PopTipMsg("当前状态不能上坐骑");
		return;
	}
	if(ifOn && pChar->GetMyMapObj() && pChar->GetMyMapObj()->m_szwSkin[MSKIN_BODY] > 2000)
	{
		if(bWarn)
			pMainMenu->PopTipMsg("变身状态不能上坐骑");
		return;
	}
	if(m_ifRide == ifOn)
		return;
	BuildPacketEx(pPacket,JGSLAVE,C_REQUEST_RIDE,buf,1024);
	SET_DATA(pData,JGSLAVE,C_REQUEST_RIDE,pPacket);
	pData->ifOn = ifOn;
	SendPacket(pPacket);
	m_ifRide = ifOn;
	MapAIObj* pSelf = pChar->GetMyMapObj();
	if(pSelf)
	{
		if(m_ifRide)
			pSelf->m_szwSkin[MSKIN_HORSE]	= m_pEquipHorse->m_pSkinObj->nId;
		else
			pSelf->m_szwSkin[MSKIN_HORSE]	= 0;
	}
}