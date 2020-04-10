#include "MainMenu.h"
#include "ios_user_packet.h"
#include "ManorMenu.h"
#include "FortCenter.h"
#include "TextConfig.h"
#include "MallConfig.h"
#include "MapControl.h"
extern void	SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj);
extern void	SetItemToDCom(DComponent* pDom,int nID);
extern void	SetCharToDCom(DComponent* pDom,int	nSkin,char* szAction = "stand",int nDirection = 5);
extern void	SetWeaponToDCom(DComponent* pDom,int nID,char* szAction = "stand",int nDirection = 5);

static void CBFrameFBEvent(int nPos,char* szName)
{
	if (strncmp (szName, "friend",6) == 0)
	{
		FriendObj* pItem = (FriendObj*)pMainMenu->m_pManorMenu->m_pForm->pCFrame->GetFPTag(nPos);
		if (pItem)
		{
			pChar->m_pFortCenter->GetFriendManorData(pItem->dwUinB);
		}
	}

}
static bool GetItemSeed(ItemObj* pObj,void* param){
	if (pObj->GetVal(ITEM_AD_SLOT) !=ITEM_SLOT_BAG )
	{
		return false;
	}
	if (pObj->pItemBase->byLv>pChar->m_byLvl)
	{
		return false;
	}
	int nId = pObj->GetVal(ITEM_AD_TYPE);

	if (nId>=25000&&nId<=25499)
	{
		return true;
	}
// 	if (pObj->pItemBase->nStar >= 1&&pObj->pItemBase->byType == ITEMTYPE_EQUIP)
// 	{
// 		return true;
// 	}
	return false;
}

void CBAskUnlock(void * pParam,int nVal)
{
	if (nVal)
	{
		pChar->m_pFortCenter->UnlockManorPos();
	}
}
void ManorDoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	if (pChar->m_dwUin == pMainMenu->m_pManorMenu->m_dwUin)
	{
		if (!strncmp(pEvent->pComponent->szSpr,szSeedSpr,strlen(szSeedSpr)))
		{
			pMainMenu->PopTipMsg("还未成熟，无法收获！");
		}
		if (!strcmp(pEvent->pComponent->szSpr,szUnLockSpr)&&pMainMenu->m_pManorMenu->CheckPos(nPos,MANOR_ACT_SEED))
		{
			pMainMenu->m_pManorMenu->m_nSelect = nPos;
			pMainMenu->m_pChatMenu->m_pItemMenu->SetType(FORM_MANOR);
			pMainMenu->m_pChatMenu->m_pItemMenu->SetFilter(GetItemSeed);
			pMainMenu->m_pChatMenu->m_pItemMenu->m_bReSort = true;
			pMainMenu->m_pChatMenu->m_pItemMenu->SetLockEquip(true);
			pMainMenu->m_pChatMenu->m_pItemMenu->Open();
			//pChar->m_pFortCenter->SeedManorPos(nPos)
		}else if (!strncmp(pEvent->pComponent->szSpr,szCompleteSpr,strlen(szCompleteSpr)))
		{
			pChar->m_pFortCenter->GetManorReward(nPos);
		}else if (!strcmp(pEvent->pComponent->szSpr,szLockSpr)&&nPos>=7)
		{
			char sztxt[64];
			int nCount = pMainMenu->m_pManorMenu->GetRmbUnLockCount(pChar->m_dwUin);
			if (nCount>=3)
			{
				return;
			}
			static int nCost[] = {20,100,500};
			if (pChar->m_dwRmb<nCost[nCount])
			{
				pMainMenu->PopTipMsg("需要%d钻石",nCost[nCount]);
				return;
			}
			sprintf(sztxt,"是否花费%d钻石开启下一块土地",nCost[nCount]);
			pMainMenu->PopAskMsgBox(sztxt,CBAskUnlock,(void*)nPos);
		}
	}else
	{
		if (!strncmp(pEvent->pComponent->szSpr,szSeedSpr,strlen(szSeedSpr))&&pMainMenu->m_pManorMenu->CheckPos(nPos,MANOR_ACT_FAST)){
			pChar->m_pFortCenter->FastManorPos(pMainMenu->m_pManorMenu->m_dwUin,nPos);
		}else if (!strncmp(pEvent->pComponent->szSpr,szCompleteSpr,strlen(szCompleteSpr))&&pMainMenu->m_pManorMenu->CheckPos(nPos,MANOR_ACT_STEAL))
		{
			pChar->m_pFortCenter->GetManorReward(pMainMenu->m_pManorMenu->m_dwUin,nPos);
		}
	}

}

void GoVipEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pVipMenu->Open();
	}
}

void ManorVipEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	if (pChar->m_dwUin == pMainMenu->m_pManorMenu->m_dwUin)
	{
		if (nPos==4&&pChar->m_byVipLvl<2)
		{
			pMainMenu->PopAskMsgBox("VIP等级不足，是否前往提升VIP等级？",GoVipEvent,(void*)0);
		}
		if (nPos==5&&pChar->m_byVipLvl<4)
		{
			pMainMenu->PopAskMsgBox("VIP等级不足，是否前往提升VIP等级？",GoVipEvent,(void*)0);
		}
		if (nPos==6&&pChar->m_byVipLvl<6)
		{
			pMainMenu->PopAskMsgBox("VIP等级不足，是否前往提升VIP等级？",GoVipEvent,(void*)0);
		}
	}
}

void BackToOwnEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pManorMenu->SetUin(pChar->m_dwUin);
}
int SeedCompare(const void * Item1, const void * Item2){
	ItemObj * pData1 = *(ItemObj **)Item1;
	ItemObj * pData2 = *(ItemObj **)Item2;
	return pData1->GetVal(ITEM_AD_TYPE)%10-pData2->GetVal(ITEM_AD_TYPE)%10;
}

void PetGoActionEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainInterface->GetDForm("zhuangyuan")->Close();
		pMainInterface->GetDForm("yaosai")->Close();
		pMainMenu->m_pActivityCtrl->Open(0);
	}
}
void AutoSeedEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ManorDataEx* pData = pChar->m_pFortCenter->GetManorData(pChar->m_dwUin);
	if (pData)
	{
		bool bTip = true;
		xnList* pList = xnList::Create();
		pChar->m_pItemCenter->GetItemInBagByFilter(pList,GetItemSeed);
		pList->Sort(SeedCompare);
		int nPos = 0;
		int nNum = 0;
		while (pList->Count)
		{
			ItemObj* pObj = (ItemObj*)pList->Delete(0);
			int nCount = pObj->GetVal(ITEM_AD_NUM);
			nNum = nCount;
			while(nCount){
				while(pData->dwID[nPos]&&nPos<10){
					nPos++;
				}
				if (nPos == 10)
				{
					break;
				}
				if (!strcmp(pMainMenu->m_pManorMenu->m_pComponentPic[nPos]->szSpr,szUnLockSpr))
				{
					pChar->m_pFortCenter->SeedManorPos(nPos,pObj);
					bTip = false;
					nCount--;
				}
				nPos++;
			}
		}
		if (bTip&&!nNum)
		{
			pMainMenu->PopAskMsgBox("包裹内缺少可种植种子，可通过参加活动“运送资源”获得。运送资源可通过打开日常活动界面点击立即前往接取任务。",PetGoActionEvent,(void*)0);
			return;
		}
		if (bTip&&nNum)
		{
			pMainMenu->PopTipMsg("已种植满，请收获后再种植");
			return;
		}
	}
}
void GetAllRewardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	//pMainMenu->m_pCtrlMenu->m_pHarvest->SetVisible(false);
	pChar->m_bHarvest = false;
	pChar->m_pFortCenter->GetAllReward();
}

void OneTouchWaterEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	bool bFlag = false;
	char szTemp[32];
	for (int i = 0; i < nMaxManorCount; i++)
	{
		sprintf(szTemp, "k%d", i + 1);
		DComponent* pComp = pMainMenu->m_pManorMenu->m_pForm->GetComponent(szTemp);
		if (pComp)
		{
			if (!strncmp(pComp->szSpr,szSeedSpr,strlen(szSeedSpr)) && pMainMenu->m_pManorMenu->CheckFastPos(i))
			{
				pMainMenu->m_pManorMenu->m_dwWaterTick = xnGetTickCount();
				pMainMenu->m_pManorMenu->m_pList->Add((void*)(pMainMenu->m_pManorMenu->m_dwWaterTick + 500));
				pMainMenu->m_pManorMenu->m_pNumList->Add((void*)i);
				bFlag = true;
			}
		}
	}
	if (bFlag)
	{
		pMainMenu->m_pManorMenu->m_bWater = true;
	}else
		pMainMenu->PopTipMsg("没有可浇水的植物。");
}

void OneTouchStealEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	bool bFlag = false;
	char szTemp[32];
	for (int i = 0; i < nMaxManorCount; i++)
	{
		sprintf(szTemp, "k%d", i + 1);
		DComponent* pComp = pMainMenu->m_pManorMenu->m_pForm->GetComponent(szTemp);
		if (pComp)
		{
			if (!strncmp(pComp->szSpr,szCompleteSpr,strlen(szCompleteSpr)) && pMainMenu->m_pManorMenu->CheckStealPos(i))
			{
				pMainMenu->m_pManorMenu->m_dwStealTick = xnGetTickCount();
				pMainMenu->m_pManorMenu->m_pStealList->Add((void*)(pMainMenu->m_pManorMenu->m_dwStealTick + 500));
				pMainMenu->m_pManorMenu->m_pStealNumList->Add((void*)i);
				bFlag = true;
			}
		}
	}
	if (bFlag)
	{
		pMainMenu->m_pManorMenu->m_bSteal = true;
	}else
		pMainMenu->PopTipMsg("没有可偷取的植物。");
}

void ChickManorAwardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pManorAwardMenu->Open();
}

void GetManorRecordEvent(InterfaceEvent* pEvent, void* param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pChar->SendCmd(FRIEND,C_GET_MANOR_RECORD);
}

ManorMenu::ManorMenu():m_nSelect(-1)
{
	m_pForm = pMainInterface->GetDForm("zhuangyuan");
	char sztxt[32];
	for (int i=0;i<nMaxManorCount;i++)
	{
		sprintf(sztxt,"yq%d",i+1);
		m_pComponentTxt[i] = m_pForm->GetComponent(sztxt);
		m_pComponentTxt[i]->SetEvent(ManorVipEvent,(void*)i);
		sprintf(sztxt,"k%d",i+1);
		m_pComponentPic[i] = m_pForm->GetComponent(sztxt);
		m_pComponentPic[i]->SetEvent(ManorDoEvent,(void*)i);
		sprintf(sztxt,"h%d",i+1);
		m_pComponentEth[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt, "j%d", i+1);
		m_pComponentFast[i] = m_pForm->GetComponent(sztxt);
		m_pComponentFast[i]->SetVisible(false);
	}

	m_pForm->pCFrame	= new CCtrlFrame(711,234,220,340);

	DComponent*	pComp	= m_pForm->GetComponent ("friend");
	pComp->SetVisible (false);
	DComponent*	ppComps[2];
	ppComps[0]		= m_pForm->GetComponent ("x");
	ppComps[1]		= m_pForm->GetComponent ("xz");


	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep	= 50;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,2);
	//m_pForm->pCFrame->SetFPCnt (10);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);

	m_pForm->GetComponent("back")->SetEvent(BackToOwnEvent);		//返回庄园
	m_pForm->GetComponent("auto")->SetEvent(AutoSeedEvent);		//自动种植
	m_pForm->GetComponent("harv")->SetEvent(GetAllRewardEvent);		//全部收获

	m_pForm->GetComponent("tou")->SetEvent(OneTouchStealEvent);
	m_pForm->GetComponent("water")->SetEvent(OneTouchWaterEvent);
	m_pForm->GetComponent("jl")->SetEvent(ChickManorAwardEvent);

	m_pForm->GetComponent("jilu")->SetEvent(GetManorRecordEvent);

	m_pList = xnList::Create();
	m_pNumList = xnList::Create();
	m_dwWaterTick = 0;
	m_bWater = false;
	m_pStealList = xnList::Create();
	m_pStealNumList = xnList::Create();
	m_dwStealTick = 0;
	m_bSteal = false;
}

void ManorMenu::Open()
{
	m_bUpdate = true;
	
	pChar->SendCmd(USER,C_GET_FORTDATA_NEW);
	SetUin(pChar->m_dwUin);
	m_pForm->Open();
}
int nLv = 0;
int nVipLv = 0;
static int fGainFast[] = {100,110,120,130,140,150,160,170,180,190,200};
void ManorMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	
	if (m_bUpdate)
	{
		m_bUpdate = false;

		nLv = pChar->m_byLvl;
		nVipLv = pChar->m_byVipLvl;;
		m_pForm->GetComponent("back")->SetVisible(m_dwUin != pChar->m_dwUin);
		m_pForm->GetComponent("auto")->SetVisible(m_dwUin == pChar->m_dwUin);
		m_pForm->GetComponent("harv")->SetVisible(m_dwUin == pChar->m_dwUin);
		m_pForm->GetComponent("water")->SetVisible(m_dwUin != pChar->m_dwUin);
		m_pForm->GetComponent("tou")->SetVisible(m_dwUin != pChar->m_dwUin);

		int nCount = 0;
		for (std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.begin();itor!=pChar->m_pFriendList.end();itor++)
		{
			FriendObj& pItem	= itor->second;
			if (pItem.byRsType == RS_TYPE_FRIEND&&pItem.byLv>=20)
			{
				nCount++;
			}
		}
		m_pForm->pCFrame->SetFPCnt (nCount);
		int i=0;
		for (std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.begin();itor!=pChar->m_pFriendList.end();itor++)
		{
			FriendObj& pItem	= itor->second;
			bool bOnline = pChar->CheckFriendOnline(pItem.dwUinB);
			if (pItem.byRsType == RS_TYPE_FRIEND&&pItem.byLv>=20)
			{
				m_pForm->pCFrame->GetFPCompBase(i)->SetCaptionEx("%s",1,pItem.szName);
				m_pForm->pCFrame->GetFPCompBase(i)->SetSprColor(bOnline?255:125,bOnline?255:125,bOnline?255:125);

				m_pForm->pCFrame->SetFPTag(i,&pItem);
				m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(pItem.dwUinB == m_dwUin);
				if (pItem.dwUinB == m_dwUin)
				{
					nLv = pItem.byLv;
					nVipLv = pItem.byVip;
				}i++;
			}
			
		}
	}

	ManorDataEx* pData = pChar->m_pFortCenter->GetManorData(m_dwUin);
	ManorDataEx* pMyData = pChar->m_pFortCenter->GetManorData(pChar->m_dwUin);
	if (!pData)
	{
		return;
	}
	if (!pMyData)
	{
		return;
	}

	if (m_bWater && xnGetTickCount() - (DWORD)m_pList->Items[0] >= 1000)
	{
		pChar->m_pFortCenter->FastManorPos(m_dwUin, (int)m_pNumList->Items[0]);
		m_pNumList->Delete(0);
		m_pList->Delete(0);
		if (m_pList->Count == 0)
		{
			m_bWater = false;
			m_dwWaterTick = 0;
		}
	}
	if (m_bSteal && xnGetTickCount() - (DWORD)m_pStealList->Items[0] >= 1000)
	{
		pChar->m_pFortCenter->GetManorReward(m_dwUin, (int)m_pStealNumList->Items[0]);
		m_pStealNumList->Delete(0);
		m_pStealList->Delete(0);
		if (m_pStealList->Count == 0)
		{
			m_bSteal = false;
			m_dwStealTick = 0;
		}
	}

	char sztxt[256];
	bool bIsMine = (m_dwUin == pChar->m_dwUin);
	for (int i=0;i<nMaxManorCount;i++)
	{
		memset(sztxt,0,256);
		DWORD dwTime = pChar->GetServerTime();
		m_pComponentEth[i]->SetVisible(false);
		if (pData->dwID[i])
		{
			m_pComponentTxt[i]->SetVisible(true);
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pData->dwID[i]);
			strcat(sztxt,pBase->szName);
			strcat(sztxt," ");
			static int nCostTime[] = {120*60,240*60,360*60,480*60,600*60,720*60};
			int nItemLv = pData->dwID[i]%10;
			DWORD dwCurTime = dwTime - pData->dwTime[i];
			if (dwCurTime >= nCostTime[nItemLv])
			{
				if(bIsMine) strcat(sztxt,"可收获");
				char szSpr[32];
				sprintf(szSpr,"%s%d.png",szCompleteSpr,pBase->nSkin);
				m_pComponentPic[i]->SetViewSpr(szSpr);
				m_pComponentFast[i]->SetVisible(false);
				if(CheckStealPos(i) == false && !bIsMine) 
				{
					m_pComponentFast[i]->SetVisible(true);
					m_pComponentFast[i]->SetViewSpr("BT_ZY_YT.png");
				}
					//m_pComponentFast[i]->SetAddSprByTag("BT_ZY_YT.png",0,0,100);
			}else{
				char sztime[32];
				sprintf(sztime,"%.2d:%.2d:%.2d",(nCostTime[nItemLv]-dwCurTime)/3600,(nCostTime[nItemLv]-dwCurTime)%3600/60,(nCostTime[nItemLv]-dwCurTime)%60);
				strcat(sztxt,sztime);
				sprintf(sztime,"%s%d.png",szSeedSpr,pBase->nSkin);
				m_pComponentPic[i]->SetViewSpr(sztime);

				if (m_dwUin == pChar->m_dwUin)
				{
					m_pComponentFast[i]->SetVisible(false);
				}else
				{
					if (CheckFastPos(i) == false)
					{
						m_pComponentFast[i]->SetVisible(true);
						m_pComponentFast[i]->SetViewSpr("MB_ZY_JIAOSHUI2.png");
					}else
						m_pComponentFast[i]->SetVisible(false);
				}
			}
			m_pComponentTxt[i]->SetCaptionEx("%s",1,sztxt);
		}else{
			m_pComponentFast[i]->SetVisible(false);
			m_pComponentTxt[i]->SetVisible(bIsMine);
			if (i == 0 && nLv<20)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"LV20开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 1 && nLv<20)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"LV20开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 2 && nLv<25)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"LV25开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 3 && nLv<30)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"LV30开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 4 && nVipLv<2)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"VIP2开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 5 && nVipLv<4)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"VIP4开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 6 && nVipLv<6)
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"VIP6开启":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr2);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 7 && !(pData->nPos&1))
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr);
				m_pComponentTxt[i]->SetVisible(false);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 8 && !(pData->nPos&2))
			{
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"":"");
				m_pComponentPic[i]->SetViewSpr(szLockSpr);
				m_pComponentTxt[i]->SetVisible(false);
				m_pComponentEth[i]->SetVisible(true);
			}else if (i == 9 && !(pData->nPos&4))
			{
				m_pComponentPic[i]->SetViewSpr(szLockSpr);
				m_pComponentTxt[i]->SetCaptionEx("%s",1,bIsMine?"":"");
				m_pComponentTxt[i]->SetVisible(false);
				m_pComponentEth[i]->SetVisible(true);
			}else{
				m_pComponentTxt[i]->SetVisible(false);
				m_pComponentPic[i]->SetViewSpr(szUnLockSpr);
				m_pComponentTxt[i]->SetCaptionEx("%s",1,"");
				m_pComponentEth[i]->SetVisible(false);
			}
		}
	}
	m_pForm->GetComponent("lv1")->SetCaptionEx("Lv.%d",1,pMyData->nLv);
	m_pForm->GetComponent("lv1")->m_nFontSize = 26;
	m_pForm->GetComponent("lv1")->SetSprColor(224,224,26);
	m_pForm->GetComponent("name1")->SetCaptionEx("%s的庄园",1,pChar->m_szNick);
	m_pForm->GetComponent("jc")->SetCaptionEx("收益加成%d%%",1,fGainFast[pMyData->nLv]);
	m_pForm->GetComponent("jc")->SetSprColor(224,17,17);
	if (!bIsMine)
	{
		std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.find(m_dwUin);
		if (itor != pChar->m_pFriendList.end())
		{
			FriendObj& pFriend	= itor->second;
			m_pForm->GetComponent("name2")->SetCaptionEx("%s的庄园",1,pFriend.szName);
		}else
		{
			m_pForm->GetComponent("name2")->SetCaptionEx("%s的庄园",1,pChar->m_pFortCenter->GetManorRocordName(m_dwUin));
		}
		
		m_pForm->GetComponent("lv2")->SetCaptionEx("Lv.%d",1,pData->nLv);
		m_pForm->GetComponent("lv2")->m_nFontSize = 26;
		m_pForm->GetComponent("lv2")->SetSprColor(224,224,26);
	}else
	{
		m_pForm->GetComponent("name2")->SetCaption("",0);
		m_pForm->GetComponent("lv2")->SetCaption("",0);
	}
	static int nNeedExp[] = {100,300,900,2700,8100,24300,72900,218700,656100,1968300,0};
	m_pForm->GetComponent("dj")->SetProgress(pMyData->nExp,nNeedExp[pMyData->nLv]);
}

void ManorMenu::SetUin( DWORD dwUin )
{
	m_bUpdate = true;
	m_dwUin = dwUin;
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++)
	{
		FriendObj* pObj = (FriendObj*)m_pForm->pCFrame->GetFPTag(i);
	}
}

bool ManorMenu::CheckPos( int nPos,BYTE byType )
{
	if (nPos<0||nPos>=nMaxManorCount)
	{
		return false;
	}
	ManorDataEx* pData = pChar->m_pFortCenter->GetManorData(m_dwUin);
	if (!pData)
	{
		return false;
	}

	switch(byType)
	{
	case MANOR_ACT_SEED:
		{
			if (m_dwUin != pChar->m_dwUin)
			{
				return false;
			}

			if (pData->dwID[nPos])
			{
				return false;
			}

			if (nPos == 0 && pChar->m_byLvl>=20)
			{
				return true;
			}else if (nPos == 1 && pChar->m_byLvl>=20)
			{
				return true;
			}else if (nPos == 2 && pChar->m_byLvl>=25)
			{
				return true;
			}else if (nPos == 3 && pChar->m_byLvl>=30)
			{
				return true;
			}else if (nPos == 4 && pChar->m_byVipLvl>=2)
			{
				return true;
			}else if (nPos == 5 && pChar->m_byVipLvl>=4)
			{
				return true;
			}else if (nPos == 6 && pChar->m_byVipLvl>=6)
			{
				return true;
			}else if (nPos == 7 && (pData->nPos&1))
			{
				return true;
			}else if (nPos == 8 && (pData->nPos&2))
			{
				return true;
			}else if (nPos == 9 && (pData->nPos&4))
			{
				return true;
			}else{
				return false;
			}
		}
		break;
	case MANOR_ACT_FAST:
		{
			if (CheckFastPos(nPos) == false)
				return false;
		}
		break;
	case MANOR_ACT_STEAL:
		{
			if (CheckStealPos(nPos) == false)
				return false;
		}
		break;
	}
	return true;
}

int ManorMenu::GetRmbUnLockCount( DWORD dwUin )
{
	ManorDataEx* pData = pChar->m_pFortCenter->GetManorData(m_dwUin);
	if (!pData)
	{
		return 0;
	}
	int nCount = 0;
	for (int i=0;i<3;i++)
	{
		if(pData->nPos&(1<<i)) nCount++;
	}
	return nCount;
}

ManorMenu::~ManorMenu()
{
	while(m_pList->Count)
		m_pList->Delete(0);
	m_pList->Free();
	while(m_pNumList->Count)
		m_pNumList->Delete(0);
	m_pNumList->Free();

	while(m_pStealList->Count)
		m_pStealList->Delete(0);
	m_pStealList->Free();
	while(m_pStealNumList->Count)
		m_pStealNumList->Delete(0);
	m_pStealNumList->Free();

}

bool ManorMenu::CheckFastPos( int nPos )
{
	if (nPos<0||nPos>=nMaxManorCount)
		return false;

	if (pChar->m_dwUin == m_dwUin)
		return false;
	ManorDataEx* pData = pChar->m_pFortCenter->GetManorData(m_dwUin);
	if (!pData)
		return false;
	if (!pData->dwID[nPos])
		return false;

	int nFastCnt = 0;
	for (int i = 0; i < 5; i++)
	{
		if (pData->dwFastUin[nPos][i])
		{
			if (pData->dwFastUin[nPos][i] == pChar->m_dwUin)
				return false;

			nFastCnt++;
		}
	}

	if(nFastCnt > 4)
		return false;

	return true;
}

bool ManorMenu::CheckStealPos( int nPos )
{
	if (nPos<0||nPos>=nMaxManorCount)
		return false;

	if (pChar->m_dwUin == m_dwUin)
		return false;
	ManorDataEx* pData = pChar->m_pFortCenter->GetManorData(m_dwUin);
	if (!pData)
		return false;
	if (!pData->dwID[nPos])
		return false;

	int nStealCnt = 0;
	for (int i = 0; i < 5; i++)
	{
		if (pData->nState[nPos][i])
		{
			if (pData->nState[nPos][i] == pChar->m_dwUin)
				return false;

			nStealCnt++;
		}

	}

	if (nStealCnt > 4)
		return false;

	return true;
}

void GoToFriendManorEvent(int nPos, char* szName, void* param)
{
	if (strncmp(szName,"go",2) == 0)
	{
		ManorRecordMenu* pMenu = (ManorRecordMenu*)param;
		if (pMenu)
		{
			DWORD dwUin = (DWORD)pMenu->m_pForm->pCFrame->GetFPTag(nPos);
			if (dwUin)
			{
				pChar->m_pFortCenter->GetFriendManorData(dwUin);
			}
		}

		pMenu->m_pForm->Close();
	}else
	{

	}
}

ManorRecordMenu::ManorRecordMenu()
{
	m_pForm = pMainInterface->GetDForm("zyjilu");
	
	DComponent* pBaseComp = m_pForm->GetComponent("n");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*4);
	DComponent* ppComp[2];
	ppComp[0] = m_pForm->GetComponent("name");
	ppComp[1] = m_pForm->GetComponent("go");

	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,2);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetEventEx(GoToFriendManorEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);
}

ManorRecordMenu::~ManorRecordMenu()
{

}

void ManorRecordMenu::InitMenu()
{
	int nFrameCnt = pChar->m_pFortCenter->m_pManorRocord.size();
	m_pForm->pCFrame->SetFPCnt(nFrameCnt);

	m_pForm->GetComponent("num")->SetCaptionEx("%d/200",1,nFrameCnt);
	if (nFrameCnt != 0)
	{
		char  szTxt[256] = {0};
		int nIndex = nFrameCnt-1;
		for (std::map<DWORD,NetManorRecord>::iterator itor = pChar->m_pFortCenter->m_pManorRocord.begin(); itor != pChar->m_pFortCenter->m_pManorRocord.end();itor++)
		{
			m_pForm->pCFrame->GetFPComp(nIndex,"go")->SetVisible(true);

			NetManorRecord* pItem = &itor->second;
			if (pItem)
			{
				m_pForm->pCFrame->SetFPTag(nIndex,(void*)pItem->dwUin);

				if (pItem->bVal < 10000)
				{
					sprintf(szTxt,"%s来庄园帮忙浇水。",pItem->szName);
				}else
				{
					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pItem->bVal);
					if (pBase)
					{
						sprintf(szTxt,"%s来庄园偷取了我的%s",pItem->szName,pBase->szName);
					}else
						sprintf(szTxt,"%s来庄园偷取了我的东西",pItem->szName);
				}
			}else
			{
				sprintf(szTxt,"这家伙居然没留下名字！");
				m_pForm->pCFrame->GetFPComp(nIndex,"go")->SetVisible(false);
			}

			m_pForm->pCFrame->GetFPComp(nIndex,"name")->SetCaption(szTxt,0);

			nIndex--;
		}
	}

	m_pForm->Open();
}

void OpenManorEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	FortMenu* pMenu = (FortMenu*)param;
	//pMenu->Close();
	if (pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("需要达到20级才可开启庄园");
		return;
	}
	pMainMenu->m_pManorMenu->Open();
}
void OpenWarRankEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_byLvl<30)
	{
		pMainMenu->PopTipMsg("需要等级30");
		return;
	}
	if(pChar->CheckInFight())
	{
		pMainMenu->PopTipMsg("战斗状态下不可打开该界面!");
		return;
	}
	if(g_pMapCenter->m_nBaseMapIndex == 66)
	{
		pMainMenu->PopTipMsg("该活动地图内，无法打开该界面!");
		return;
	}
	FortMenu* pMenu = (FortMenu*)param;
	//pMenu->Close();
	pMainMenu->m_pWarRankMenu->Open();
}

void OpenLoadingEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	//pMainMenu->PopTipMsg("功能暂未开放，敬请期待！");
	pMainMenu->m_pFlappyBirdMenu->OpenForm();

}

void OpenGreenModeEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_byLvl < 36)
	{
		pMainMenu->PopTipMsg("需达到36级才能开启！");
		return;
	}
	if(pChar->CheckInFight())
	{
		pMainMenu->PopTipMsg("战斗状态下不可打开该界面!");
		return;
	}
	if(g_pMapCenter->m_nBaseMapIndex == 66)
	{
		pMainMenu->PopTipMsg("该活动地图内，无法打开该界面!");
		return;
	}
	if(pChar->m_pCQuestCenter->GetCurQuest(5001) ||
		pChar->m_pCQuestCenter->GetCurQuest(5002) ||
		pChar->m_pCQuestCenter->GetCurQuest(5003))
	{
		pMainMenu->PopTipMsg("你正在护送商队，无法进入梦境！");
		return;
	}
	pMainMenu->m_pGreenModeMenu->m_pForm->Open(2);
	pMainMenu->m_pFortMenu->Close();
}

void OpenStarSysMenuEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pStarTabCtrl->Open(2,0);
	//pMainMenu->m_pStarSysMenu->Open();
}

FortMenu::FortMenu()
{
	m_pForm = pMainInterface->GetDForm("yaosai");
	m_pForm->GetComponent("zy")->SetEvent(OpenManorEvent,this);
	m_pForm->GetComponent("jjc")->SetEvent(OpenWarRankEvent,this);

	m_pForm->GetComponent("jjc2")->SetEvent(OpenGreenModeEvent,this);
	m_pForm->GetComponent("ps")->SetEvent(OpenStarSysMenuEvent,this);
	m_pForm->GetComponent("sd")->SetEvent(OpenLoadingEvent,this);
}

void FortMenu::Open()
{
	m_pForm->Open();
}

void FortMenu::Close()
{
	m_pForm->Close();
}
void StartWarRankEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_pClientFlag->GetFlag(nWarRankCntFlag)>=10)
	{
		pMainMenu->PopTipMsg("剩余次数不足");
		return;
	}
	WarRankData* pData = pChar->m_pFortCenter->GetWarRankData(pChar->m_dwUin);
	if (!pData)
	{
		return;
	}
	
	if (pMainMenu->m_pWarRankMenu->m_nSelect<pChar->m_pFortCenter->m_pWarRankList->Count)
	{
		WRankSimpleObj* pObj = (WRankSimpleObj*)pChar->m_pFortCenter->m_pWarRankList->Items[pMainMenu->m_pWarRankMenu->m_nSelect];
		strncpy(pMainMenu->m_pWarRankMenu->m_szName,pObj->szName,20);
		pChar->m_pFortCenter->WarRankPos(pObj->wPos);
	}else{
		pMainMenu->PopTipMsg("未知错误");
	}
	
}
void OpenRewardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pWarRankRewardMenu->Open();
}
static void SelectTargetEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	pMainMenu->m_pWarRankMenu->Select(nPos);

}
static void SelectTargetExEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	if (nPos<pChar->m_pFortCenter->m_pWarTenList->Count)
	{
		WRankSimpleObj* pObj = (WRankSimpleObj*)pChar->m_pFortCenter->m_pWarTenList->Items[nPos];
		ClientUserInfo* pGet = pChar->m_pRankCenter->GetUserInfo(pObj->dwUin);
		if(pGet)
		{
			pMainMenu->m_pSelObjInfoMenu->Open(pGet);
		}
	}

	//pMainMenu->m_pWarRankMenuEx->Select(nPos);

}
static void GoToRechargeEven(void * param,int nVal)
{
	if (nVal)
		pMainMenu->m_pRechargeMenu->OpenForm(2);
}
void AddChallengeTimes(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	if (pChar->m_pClientFlag->GetFlag(nWarRankCntFlag) < 1)
	{
		pMainMenu->PopTipMsg("次数为满，请先消耗次数后再购买");
		return;
	}
	if (pChar->m_dwRmb < 20)
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值",GoToRechargeEven,(void*)0);
		return;
	}
	if (pChar->m_pClientFlag->GetFlag(nBuyWarTimesFlag) > pChar->m_byVipLvl -1)
	{
		pMainMenu->PopAskMsgBox("购买次数为零，是否前往提升VIP等级，获得更多可购买次数？",GoToRechargeEven,(void*)0);
		return;
	}

	pChar->SendCmd(USER,C_ADD_WARRANKTIMES);
}

WarRankMenuEx::WarRankMenuEx()
{
	m_pForm = pMainInterface->GetDForm("jjc2");
	char sztxt[32];
	for (int i=0;i<10;i++)
	{
		sprintf(sztxt,"zhanli%d",i);
		m_pComponentScore[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"model1%d",i);
		m_pComponentTarget[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"name%d",i);
		m_pComponentName[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"zy%d",i);
		m_pComponentJob[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"pm%d",i);
		m_pComponentRank[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"lv%d",i);
		m_pComponentLv[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"huode%d",i);
		m_pComponentAwardScore[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"model1%d",i);
		m_pComponentModel1[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"model2%d",i);
		m_pComponentModel2[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"x%d",i);
		m_pComponentSelect[i] = m_pForm->GetComponent(sztxt);
		m_pComponentSelect[i]->SetEvent(SelectTargetExEvent,(void*)i);
	}
}

void	WarRankMenuEx::Open()
{
	m_pForm->Open();
}

void	WarRankMenuEx::Close()
{
	if(m_pForm->bOpened)
		m_pForm->Close();
}

void	WarRankMenuEx::Reflesh()
{
	if(m_pForm)
	{
		char sztxt[32];
		for (int i=0;i<10;i++)
		{
			if (i<pChar->m_pFortCenter->m_pWarTenList->Count)
			{
				WRankSimpleObj* pObj = (WRankSimpleObj*)pChar->m_pFortCenter->m_pWarTenList->Items[i];
				m_pComponentLv[i]->SetVisible(true);
				m_pComponentLv[i]->SetCaptionEx("%d",0,pObj->byLv);
				m_pComponentJob[i]->SetVisible(true);
				sprintf(sztxt,"MB_ZHIYE%d.png",pObj->byJob);
				m_pComponentJob[i]->SetViewSpr(sztxt);
				m_pComponentName[i]->SetVisible(true);
				m_pComponentName[i]->SetCaptionEx("%s",0,strcmp(pObj->szName,"robot")?pObj->szName:RandNameConfig::GetInstance().GetRndName(pObj->bySex));
				m_pComponentRank[i]->SetVisible(true);
				m_pComponentRank[i]->SetCaptionEx("%d",0,pObj->wPos+1);
				m_pComponentScore[i]->SetVisible(true);
				m_pComponentScore[i]->SetCaptionEx("%d",0,pObj->dwScore);
				m_pComponentAwardScore[i]->SetVisible(true);
				m_pComponentAwardScore[i]->SetCaptionEx("%d",0,pMainMenu->m_pWarRankMenu->GetAwardScore(pObj->wPos+1));
				m_pComponentTarget[i]->SetVisible(true);

				bool bSwitch = false;
				if (pObj->byJob==3&&pObj->bySex == 1)
				{
					bSwitch = true;
				}
				m_pComponentModel1[i]->SetVisible(true);
				m_pComponentModel2[i]->SetVisible(true);
				SetCharToDCom(bSwitch?m_pComponentModel1[i]:m_pComponentModel2[i],pObj->wSkin[0]);
				if (pChar->m_pItemCenter->CheckSkin(pObj->wSkin[0]))
				{
					(bSwitch?m_pComponentModel2[i]:m_pComponentModel1[i])->SetVisible(false);
				}else{
					(bSwitch?m_pComponentModel2[i]:m_pComponentModel1[i])->SetVisible(true);
					SetWeaponToDCom(bSwitch?m_pComponentModel2[i]:m_pComponentModel1[i],pObj->wSkin[1]);

				}

				//m_pComponentTarget[i]->SetCaptionEx("%d",0,pObj->szName);
			}else{
				m_pComponentLv[i]->SetVisible(false);
				m_pComponentJob[i]->SetVisible(false);
				m_pComponentName[i]->SetVisible(false);
				m_pComponentRank[i]->SetVisible(false);
				m_pComponentScore[i]->SetVisible(false);
				m_pComponentTarget[i]->SetVisible(false);
				m_pComponentModel1[i]->SetVisible(false);
				m_pComponentModel2[i]->SetVisible(false);
				m_pComponentAwardScore[i]->SetVisible(false);
			}
		}
	}
}

void	ClickOpenTenRankButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->m_pWarRankMenuEx->Open();
	}
}
void	OpenWarrankPos(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pWarRankMenu->m_pRankPos->Open();
}
WarRankMenu::WarRankMenu()
{
	m_pForm = pMainInterface->GetDForm("jjc");
	char sztxt[32];
	for (int i=0;i<nMaxRankWarCount;i++)
	{
		sprintf(sztxt,"zhanli%d",i+1);
		m_pComponentScore[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"model1%d",i+1);
		m_pComponentTarget[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"name%d",i+1);
		m_pComponentName[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"zy%d",i+1);
		m_pComponentJob[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"pm%d",i+1);
		m_pComponentRank[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"lv%d",i+1);
		m_pComponentLv[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"huode%d",i+1);
		m_pComponentAwardScore[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"model1%d",i+1);
		m_pComponentModel1[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"model2%d",i+1);
		m_pComponentModel2[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"x%d",i+1);
		m_pComponentSelect[i] = m_pForm->GetComponent(sztxt);
		m_pComponentSelect[i]->SetEvent(SelectTargetEvent,(void*)i);
	}
	m_pForm->GetComponent("kaishi")->SetEvent(StartWarRankEvent,this);
	m_pForm->GetComponent("duihuan")->SetEvent(OpenRewardEvent,this);
	m_pForm->GetComponent("goumai")->SetEvent(AddChallengeTimes,this);
	m_pForm->GetComponent("ph")->SetEvent(ClickOpenTenRankButton,this);
	m_pForm->GetComponent("jl")->SetEvent(OpenWarrankPos,this);

	m_pRankPos = new WarRankPosMenu;
}

void WarRankMenu::Open()
{
	m_bUpdate = true;
	Select(0);
	pChar->SendCmd(USER,C_GET_FORTDATA_NEW);
	pChar->SendCmd(USER,C_GET_RANK_ENAMY);
	m_pForm->Open();
}

void WarRankMenu::Close()
{
	m_pForm->Close();
}

int WarRankMenu::GetAwardScore(int nPos)
{
	if (nPos == 0)
		return 0;

	int nWPos = nPos>500?500:nPos;
	int nScore	= (600-nWPos) * (650-nWPos)/5000;

	return nScore;
}

static int WarRankSortByPos(const void * Item1, const void * Item2)
{
	WRankSimpleObj * pData1 = *(WRankSimpleObj **)Item1;
	WRankSimpleObj * pData2 = *(WRankSimpleObj **)Item2;
	return pData2->wPos - pData1->wPos;
}

extern char* szJobTxt[];
void WarRankMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}

	if (m_bUpdate)
	{
		m_bUpdate = false;

		if (pChar->m_pFortCenter->m_pWarRankList->Count)
			pChar->m_pFortCenter->m_pWarRankList->Sort(WarRankSortByPos);

		WarRankData* pData = pChar->m_pFortCenter->GetWarRankData(pChar->m_dwUin);
		if (!pData)
		{
			return;
		}

		char sztxt[32];
		sprintf(sztxt,"MB_ZHIYE%d.png",pChar->m_byJob);
		m_pForm->GetComponent("zy")->SetViewSpr(sztxt);
		m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,pChar->m_byLvl);
		m_pForm->GetComponent("jf")->SetCaptionEx("%d",0,pData->nRankScore);
		m_pForm->GetComponent("zhanli")->SetCaptionEx("%d",0,pChar->GetScore());
		
		bool bSwitch = true;
		if (pChar->m_byJob==2)
		{
			bSwitch = false;
		}

		SetCharToDCom(bSwitch?m_pForm->GetComponent("model"):m_pForm->GetComponent("model3"),pChar->m_pItemCenter->GetClothSkin());
		if (pChar->m_pItemCenter->CheckSkin(pChar->m_pItemCenter->GetClothSkin()))
		{
			(bSwitch?m_pForm->GetComponent("model3"):m_pForm->GetComponent("model"))->SetVisible(false);
		}else{
			(bSwitch?m_pForm->GetComponent("model3"):m_pForm->GetComponent("model"))->SetVisible(true);
			SetWeaponToDCom(bSwitch?m_pForm->GetComponent("model3"):m_pForm->GetComponent("model"),pChar->m_pItemCenter->GetWeaponSkin());
		}

		if (pData->nRankPos == -1)
		{
			m_pForm->GetComponent("pm")->SetCaption("未进入排名",0);
		}else{
			m_pForm->GetComponent("pm")->SetCaptionEx("%d",0,pData->nRankPos+1);
		}
		m_pForm->GetComponent("cishu")->SetCaptionEx("%d",0,10-pChar->m_pClientFlag->GetFlag(nWarRankCntFlag));
		m_pForm->GetComponent("huode")->SetCaptionEx("%d",0, GetAwardScore(pData->nRankPos>-1?pData->nRankPos+1:0));
		m_pForm->GetComponent("gcishu")->SetCaptionEx("%d",0,pChar->m_byVipLvl - pChar->m_pClientFlag->GetFlag(nBuyWarTimesFlag));

		for (int i=0;i<nMaxRankWarCount;i++)
		{
			if (i<pChar->m_pFortCenter->m_pWarRankList->Count)
			{
				WRankSimpleObj* pObj = (WRankSimpleObj*)pChar->m_pFortCenter->m_pWarRankList->Items[i];
				m_pComponentLv[i]->SetVisible(true);
				m_pComponentLv[i]->SetCaptionEx("%d",0,pObj->byLv);
				m_pComponentJob[i]->SetVisible(true);
				sprintf(sztxt,"MB_ZHIYE%d.png",pObj->byJob);
				m_pComponentJob[i]->SetViewSpr(sztxt);
				m_pComponentName[i]->SetVisible(true);
				m_pComponentName[i]->SetCaptionEx("%s",0,strcmp(pObj->szName,"robot")?pObj->szName:RandNameConfig::GetInstance().GetRndName(pObj->bySex));
				m_pComponentRank[i]->SetVisible(true);
				m_pComponentRank[i]->SetCaptionEx("%d",0,pObj->wPos+1);
				m_pComponentScore[i]->SetVisible(true);
				m_pComponentScore[i]->SetCaptionEx("%d",0,pObj->dwScore);
				m_pComponentAwardScore[i]->SetVisible(true);
				m_pComponentAwardScore[i]->SetCaptionEx("%d",0,GetAwardScore(pObj->wPos+1));
				m_pComponentTarget[i]->SetVisible(true);

				bSwitch = false;
				if (pObj->byJob==3&&pObj->bySex == 1)
				{
					bSwitch = true;
				}
				m_pComponentModel1[i]->SetVisible(true);
				m_pComponentModel2[i]->SetVisible(true);
				SetCharToDCom(bSwitch?m_pComponentModel1[i]:m_pComponentModel2[i],pObj->wSkin[0]);
				if (pChar->m_pItemCenter->CheckSkin(pObj->wSkin[0]))
				{
					(bSwitch?m_pComponentModel2[i]:m_pComponentModel1[i])->SetVisible(false);
				}else{
					(bSwitch?m_pComponentModel2[i]:m_pComponentModel1[i])->SetVisible(true);
					SetWeaponToDCom(bSwitch?m_pComponentModel2[i]:m_pComponentModel1[i],pObj->wSkin[1]);

				}

				//m_pComponentTarget[i]->SetCaptionEx("%d",0,pObj->szName);
			}else{
				m_pComponentLv[i]->SetVisible(false);
				m_pComponentJob[i]->SetVisible(false);
				m_pComponentName[i]->SetVisible(false);
				m_pComponentRank[i]->SetVisible(false);
				m_pComponentScore[i]->SetVisible(false);
				m_pComponentTarget[i]->SetVisible(false);
				m_pComponentModel1[i]->SetVisible(false);
				m_pComponentModel2[i]->SetVisible(false);
				m_pComponentAwardScore[i]->SetVisible(false);
			}
		}
	}
}

void WarRankMenu::Select( int nPos )
{
	m_bUpdate = true;
	m_nSelect = nPos;
	for (int i=0;i<nMaxRankWarCount;i++)
	{
		m_pComponentSelect[i]->SetViewSpr(nPos == i?"BT_JJC_XUAN.png":"");
	}
}

static void	InputResultShop(int nResult,void * param){
	ShopItemCfg* pCfg = (ShopItemCfg*)param;
	pChar->m_pItemCenter->BuyShopItem(pCfg->m_nTypeInfo,pCfg->m_nItemID,nResult);
};
static void CBFrameFBEventReward(int nPos,char* szName)
{
	if(strncmp (szName, "dh",2) == 0)
	{
		ShopItemCfg* pTag = (ShopItemCfg*)pMainMenu->m_pWarRankRewardMenu->m_pForm->pCFrame->GetFPComp(nPos,"ico1")->GetTag();
		if (pTag)
		{
			int nMaxCount = 1000;//pChar->m_dwMoney/pItem->m_nPrice;
			//nMaxCount = nMaxCount>1000?1000:nMaxCount;
			//if (!nMaxCount)
			//{
			//	pMainMenu->PopTipMsg("金币不足");
			//	return;
			//}
			pMainMenu->m_pGetNumMenu->Open(InputResultShop,nMaxCount,0,(void*)pTag);
		}
	}
}
WarRankRewardMenu::WarRankRewardMenu()
{
	m_pForm = pMainInterface->GetDForm("jjcduihuan");
	char sztxt[32];
	DComponent* pComItem[6];
	pComItem[1] = m_pForm->GetComponent("ico1");
	pComItem[0] = m_pForm->GetComponent("gezi1");
	pComItem[2] = m_pForm->GetComponent("name");
	pComItem[3] = m_pForm->GetComponent("z");
	pComItem[4] = m_pForm->GetComponent("jf");
	pComItem[5] = m_pForm->GetComponent("dh");
	m_pForm->pCFrame	= new CCtrlFrame(40,135,360,355);
	DComponent*	pComp	= m_pForm->GetComponent ("d");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,6);
	m_pForm->pCFrame->SetEvent (CBFrameFBEventReward);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 90;

//	m_pForm->GetComponent("kaishi")->SetEvent(StartWarRankEvent,this);
}

void WarRankRewardMenu::Open()
{
	pChar->SendCmd(USER,C_GET_FORTDATA_NEW);
	m_bUpdate = true;
	m_pForm->Open();
}

void WarRankRewardMenu::Close()
{
	m_pForm->Close();
}

void WarRankRewardMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}

	if (m_bUpdate)
	{
		m_bUpdate = false;
		ShopInfoCfg* pCfg = ShopConfig::GetInstance().GetCfg(nWarRankShop);
		if (!pCfg)
		{
			return;
		}
		m_pForm->pCFrame->SetFPCnt(pCfg->pItemList->Count);
		for (int i= 0;i<pCfg->pItemList->Count;i++)
		{
			ShopItemCfg* pItem = (ShopItemCfg*)pCfg->pItemList->Items[i];
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(i,"ico1"),pItem->m_nItemID);
			m_pForm->pCFrame->GetFPComp(i,"ico1")->SetTag(pItem);
			SetItemNameToDCom(m_pForm->pCFrame->GetFPComp(i,"name"),ItemConfig::GetInstance().GetItemBase(pItem->m_nItemID));
			m_pForm->pCFrame->GetFPComp(i,"jf")->SetCaptionEx("%d",0,pItem->m_nPrice);
		}
		WarRankData* pData = pChar->m_pFortCenter->GetWarRankData(pChar->m_dwUin);
		if (pData)
		{
			m_pForm->GetComponent("total")->SetCaptionEx("%d",0,pData->nRankScore);
		}
	}
}

WarRankPosMenu::WarRankPosMenu()
{
	m_pForm = pMainInterface->GetDForm("jjc3");
	char sztxt[32];
	DComponent* pComItem[1];
	pComItem[0] = m_pForm->GetComponent("xuan");
	m_pForm->pCFrame	= new CCtrlFrame(37,113,565,355);
	DComponent*	pComp	= m_pForm->GetComponent ("z");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,1);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 90;

	m_pForm->pCFrame->SetFPCnt(9);

	for(int i=0;i<9;i++){
		sprintf(sztxt,"MB_JJC%d.png",i+1);
		m_pForm->pCFrame->GetFPComp(i,"xuan")->SetViewSpr(sztxt);
	}
}

void WarRankPosMenu::Open()
{
	m_pForm->Open();
}

void	ClickManorShowEvent(InterfaceEvent* pEvent,void* param)
{
	if(param)
	{
		static int nClickY = 0;
		ManorAwardMenu* pMenu = (ManorAwardMenu*)param;
		switch(pEvent->event)
		{
		case INTERFACE_EVENT_DOWN:
			{
				nClickY = pEvent->y;
			}
			break;
		case INTERFACE_EVENT_MOVE:
			{
				pMenu->MoveXY(pEvent->pComponent->nY + pEvent->y - nClickY);
				nClickY		= pEvent->y;
			}
			break;
		case INTERFACE_EVENT_UP:
			{
				pMenu->MoveXY(pEvent->pComponent->nY + pEvent->y - nClickY);
				nClickY		= 0;
			}
			break;
		}
	}
}

ManorAwardMenu::ManorAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("zhuangyuan2");
	m_pShowComp = m_pForm->GetComponent("z");
	m_pShowComp->byType = COMPONENT_MAPGUIDE;
	m_pShowComp->SetViewSpr("ui/MB_YS1.png");
	m_pShowComp->SetEvent(ClickManorShowEvent,this);
	m_pShowComp->bAlignX	= 1;
	m_pShowComp->bAlignY	= 0;
	m_nBaseX = m_pShowComp->nX;
	m_nBaseY = m_pShowComp->nY;
}

ManorAwardMenu::~ManorAwardMenu()
{

}

void ManorAwardMenu::Open()
{
	m_pForm->Open();
	m_pShowComp->SetPos(m_nBaseX, m_nBaseY);
}

void ManorAwardMenu::Close()
{
	m_pForm->Close();
}

void ManorAwardMenu::MoveXY( int nMoveY )
{
	if(nMoveY > m_nBaseY)
		nMoveY = m_nBaseY;
	if(nMoveY < (m_nBaseY - m_pShowComp->nSprPixHeight + m_pShowComp->nH))
		nMoveY = m_nBaseY - m_pShowComp->nSprPixHeight + m_pShowComp->nH;

	m_pShowComp->SetPos(m_nBaseX, nMoveY);
}

