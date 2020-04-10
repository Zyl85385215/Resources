#include "MapMouse.h"
#include "AutoMenu.h"
#include "UserData.h"
#include "MainMenu.h"
#include "ItemConfig.h"
#include "MapControl.h"
#include "MallConfig.h"
#include "VipConfig.h"
#include "EnemyConfig.h"

void AMSwitchAutoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu*	pMenu	= (AutoMenu*)param;
	pMenu->SwitchActive(!pMenu->m_bActive);
}

void AMMainAutoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu*	pMenu	= (AutoMenu*)param;
	if(pMenu->m_bActive)
		pMenu->SwitchActive(false);
	else
		pMenu->m_pFrom->Open(2);
}

void AMSettingEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu*	pMenu	= (AutoMenu*)param;
	pMenu->m_pFromEx->Open(2);
}

void CCloseSettingEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu*	pMenu	= (AutoMenu*)param;
	pMenu->m_pFromEx->Close();
}

void ChooseDrug2Func(xnList* pList,void* param)
{
	if(!pList->Count || param == NULL)
		return;
	AutoMenu * pMenu = (AutoMenu*)param;

	ItemObj* pItem = (ItemObj*)pList->Items[0];
	if (pItem&&(pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 1))
	{
		pMenu->m_nHpDrugID = pItem->pItemBase->nTypeID;
		SaveIntegerToXML("set_hpid",pMenu->m_nHpDrugID);
	}
	if (pItem&&(pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 2))
	{
		pMenu->m_nMpDrugID = pItem->pItemBase->nTypeID;
		SaveIntegerToXML("set_mpid",pMenu->m_nMpDrugID);
	}
	if (pItem&&(pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 5))
	{
		pMenu->m_nPetHpDrugID = pItem->pItemBase->nTypeID;
		SaveIntegerToXML("set_pethpid",pMenu->m_nPetHpDrugID);
	}
}

void GoBuyDrugEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pItemCenter->GoForBuy();
	}
}

void ChooseHpDrugEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu* pMenu = (AutoMenu*)param;

	xnList* pBagList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pBagList);

	int nHDCount = 0;
	for (int i= 0;i<pBagList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pBagList->Items[i];
		if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 1))
		{
			nHDCount++;
		}

	}
	if (nHDCount)
	{
		pMainMenu->m_pChooseAutoDrug->OpenDForm(0,ChooseDrug2Func,(void*)pMenu,nHDCount);
	}
	else
	{
		pMainMenu->PopAskMsgBox("包里没有血瓶，是否前往购买？",GoBuyDrugEvent,(void*)0);
		return;
	}
	pBagList->Free();
}

void ChooseMpDrugEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu* pMenu = (AutoMenu*)param;

	xnList* pBagList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pBagList);

	int nMDCount = 0;
	for (int i= 0;i<pBagList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pBagList->Items[i];
		if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 2))
		{
			nMDCount++;
		}

	}
	if (nMDCount)
	{
		pMainMenu->m_pChooseAutoDrug->OpenDForm(1,ChooseDrug2Func,(void*)pMenu, nMDCount);
	}
	else
	{
		pMainMenu->PopAskMsgBox("包里没有魔瓶，是否前往购买？",GoBuyDrugEvent,(void*)0);
		return;
	}
	pBagList->Free();
}

void GoBuyPetDrugEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pItemCenter->GoForBuy(3);
	}
}

void ChoosePHpDrugEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu* pMenu = (AutoMenu*)param;

	xnList* pBagList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pBagList);

	int nPHDCount = 0;
	for (int i= 0;i<pBagList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pBagList->Items[i];
		if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 5))
		{
			nPHDCount++;
		}

	}
	if (nPHDCount)
	{
		pMainMenu->m_pChooseAutoDrug->OpenDForm(2,ChooseDrug2Func,(void*)pMenu,nPHDCount);
	}
	else
	{
		pMainMenu->PopAskMsgBox("包里没有宠物药瓶，是否前往购买？",GoBuyPetDrugEvent,(void*)0);
		return;
	}
	pBagList->Free();
}

void SlaveAutoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu*	pMenu	= (AutoMenu*)param;
	pMenu->m_pFromPetEx->Open(2);
}

void ClickOKEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	AutoMenu*	pMenu	= (AutoMenu*)param;
	pMenu->m_pFromPetEx->Close();
}

AutoMenu::AutoMenu()
{
	m_pFrom		= pMainInterface->GetDForm("gauji");
	m_pFromEx	= pMainInterface->GetDForm("guaji2");
	m_pFromPetEx = pMainInterface->GetDForm("guaji3");
	m_pFromPetEx->bZorde = FORM_ZORDER_ITEMSHOW;

	m_pFrom->GetComponent("kq")->SetEvent(AMSwitchAutoEvent,this);
	m_pFrom->GetComponent("qx")->SetEvent(AMSwitchAutoEvent,this);
	m_pFrom->GetComponent("sz")->SetEvent(AMSettingEvent,this);
	m_pFrom->GetComponent("icon1")->SetEvent(ChooseHpDrugEvent,this);
	m_pFrom->GetComponent("icon2")->SetEvent(ChooseMpDrugEvent,this);
	m_pFrom->GetComponent("cw")->SetEvent(SlaveAutoEvent,this);
	m_pFromPetEx->GetComponent("icon")->SetEvent(ChoosePHpDrugEvent,this);
	m_pFromPetEx->GetComponent("ok")->SetEvent(ClickOKEvent,this);

	m_pFromEx->GetComponent("ok")->SetEvent(CCloseSettingEvent,this);

	pMainInterface->GetDForm("main5")->GetComponent("gj")->SetEvent(AMMainAutoEvent,this);

	m_bRecharge = false;
	m_bActive	= false;
	m_bifResetPetState = true;
	wSkillPos	= 0;
	m_dwAutoTeamTick = 0;
	s_dwBuyDrugTick = 0;
	m_dwFixDrugTick = 0;

	_loadCfg();

	m_pSlideHp			= new CCtrlSlide(m_pFrom->GetComponent("kuang1"),m_pFrom->GetComponent("k1"));
	m_pSlideMp		= new CCtrlSlide(m_pFrom->GetComponent("kuang2"),m_pFrom->GetComponent("k2"));
	m_pCheckAutoTeam = new CCtrlCheck(m_pFrom->GetComponent("g1"),0);
	m_pCheckAutoBuy	= new CCtrlCheck(m_pFrom->GetComponent("g2"),0);
	m_pCheckAutoFix	= new CCtrlCheck(m_pFrom->GetComponent("g3"),0);
	m_pCheckAutoRecover	= new CCtrlCheck(m_pFrom->GetComponent("g4"),0);
	m_pCheckAutoSkill = new CCtrlCheck(m_pFrom->GetComponent("g5"),0);
	m_pCheckLockPlayer = new CCtrlCheck(m_pFrom->GetComponent("g6"),0);

	m_pNoPickUpWhiteEquip = new CCtrlCheck(m_pFromEx->GetComponent("g1"),0);
	m_pNoPickUpBlueEquip	= new CCtrlCheck(m_pFromEx->GetComponent("g2"),0);
	m_pNoPickUpDrug	= new CCtrlCheck(m_pFromEx->GetComponent("g3"),0);
	m_pNoPickUpMaterial	= new CCtrlCheck(m_pFromEx->GetComponent("g4"),0);

	m_pSlidePetHp = new CCtrlSlide(m_pFromPetEx->GetComponent("kuang"),m_pFromPetEx->GetComponent("k1"));
	m_pCheckAutoAtk = new CCtrlCheck(m_pFromPetEx->GetComponent("g2"),0);
	m_pCheckAutoAtkb = new CCtrlCheck(m_pFromPetEx->GetComponent("g1"),0);
	m_pCheckAutoFlw	= new CCtrlCheck(m_pFromPetEx->GetComponent("g3"),0);

	m_pSlideHp->SetSlide(m_nValSHp);
	m_pSlideMp->SetSlide(m_nValSMp);
	m_pCheckAutoTeam->SetCheck(m_bAutoTeam);
	m_pCheckAutoBuy->SetCheck(m_bAutoBuy);
	m_pCheckAutoFix->SetCheck(m_bAutoFix);
	m_pCheckAutoRecover->SetCheck(m_bAutoRecover);
	m_pCheckAutoSkill->SetCheck(m_bAutoSkill);
	m_pCheckLockPlayer->SetCheck(m_bLockPlayer);

	m_pNoPickUpWhiteEquip->SetCheck(m_bNoWhite);
	m_pNoPickUpBlueEquip->SetCheck(m_bNoBlue);
	m_pNoPickUpDrug->SetCheck(m_bNoDrug);
	m_pNoPickUpMaterial->SetCheck(m_bNoMaterial);

	m_pSlidePetHp->SetSlide(m_nValPetHp);
	m_pCheckAutoAtk->SetCheck(m_bAutoAtk);
	m_pCheckAutoAtkb->SetCheck(m_bAutoAtkb);
	m_pCheckAutoFlw->SetCheck(m_bAutoFlw);
}

AutoMenu::~AutoMenu()
{
	delete m_pSlideHp;
	delete m_pSlideMp;
	delete m_pCheckAutoTeam;
	delete m_pCheckAutoBuy;
	delete m_pCheckAutoFix;
	delete m_pCheckAutoRecover;
	delete m_pCheckLockPlayer;
	delete m_pNoPickUpWhiteEquip;
	delete m_pNoPickUpBlueEquip;
	delete m_pNoPickUpDrug;
	delete m_pNoPickUpMaterial;
	delete m_pSlidePetHp;
	delete m_pCheckAutoAtk;
	delete m_pCheckAutoAtkb;
	delete m_pCheckAutoFlw;
}

void AutoMenu::SwitchActive( bool bActive,WORD wX,WORD wY)
{

	m_bActive		= bActive;
	m_pFrom->Close();

	if(m_bActive)
	{
		MapAIObj* pMapObj	= pChar->GetMyMapObj();
		if(pMapObj == NULL)
			return;

		nMIndex			= g_pMapCenter->m_nMapIndex;
		if (wX||wY)
		{
			wStartX		= wX;
			wStartY		= wY;
		}else
		{
			wStartX		= pMapObj->nPosX;
			wStartY		= pMapObj->nPosY;
		}

		m_dwAutoTeamTick = xnGetTickCount ();

		pMainInterface->GetDForm("main5")->GetComponent("gj")->SetViewSpr("BT_GJ_STOP.png");
	}
	else
	{
		pMainInterface->GetDForm("main5")->GetComponent("gj")->SetViewSpr("BT_GJ.png");
	}
}

void AutoMenu::AutoTeam()
{
	if(!m_bActive)
		return;

	if (m_bActive&&m_bAutoTeam)
	{
		m_dwAutoTeamTick = xnGetTickCount ();

		MapAIObj* pMapObj	= pChar->GetMyMapObj();
		if(pMapObj == NULL)
			return;

		for (int i = 0; i < g_pMapCenter->m_pMPlayerList->m_pList->Count; i++)
		{
			MapPlayer*	pObj	= (MapPlayer*)g_pMapCenter->m_pMPlayerList->m_pList->Items[i];
			if (pObj&&(pObj->m_nID != pMapObj->m_nID))
			{
				pChar->InvitePlayer(pObj->m_nID);
			}
		}
	}
}

void AutoMenu::CheckPetState()
{
	if (m_bAutoAtk)
	{
		BuildPacketEx(pPacket,USER,C_CHAGE_PETSTATE,buf,256);
		SET_DATA(pSend,USER,C_CHAGE_PETSTATE,pPacket);
		pSend->byPetState	= 2;
		pChar->SendPacket(pPacket);
	}
	if (m_bAutoAtkb)
	{
		BuildPacketEx(pPacket,USER,C_CHAGE_PETSTATE,buf,256);
		SET_DATA(pSend,USER,C_CHAGE_PETSTATE,pPacket);
		pSend->byPetState	= 1;
		pChar->SendPacket(pPacket);
	}
	if (m_bAutoFlw)
	{
		BuildPacketEx(pPacket,USER,C_CHAGE_PETSTATE,buf,256);
		SET_DATA(pSend,USER,C_CHAGE_PETSTATE,pPacket);
		pSend->byPetState	= 3;
		pChar->SendPacket(pPacket);
	}
	m_bifResetPetState = false;
}

void GoVipMenuEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pVipMenu->Open();
	}
}

void AutoMenu::AutoSwitchDrug(int nType,int nID)
{
	ItemExBase* pDrugBase = ItemConfig::GetInstance().GetItemBase(nID);
	int nChgID = 0;
	if (pDrugBase&&pDrugBase->byLv > pChar->m_byLvl)
	{
		nChgID = nID -1;
	}

	xnList* pBagList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pBagList);

	switch(nType)
	{
	case 1:
		{
			if (nChgID)
			{
				ItemExBase* pDrugBase = ItemConfig::GetInstance().GetItemBase(nChgID);
				if ((pDrugBase->byType == 6)&&(pDrugBase->nUseType == 1))
				{
					m_nHpDrugID = nChgID;
					SaveIntegerToXML("set_hpid",m_nHpDrugID);
					break;
				}
			}

			if (pChar->m_pItemCenter->GetItemCountByID(m_nHpDrugID) == 0)
			{
				for (int i= 0;i<pBagList->Count;i++)
				{
					ItemObj* pItem = (ItemObj*)pBagList->Items[i];
					if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 1)&&(pItem->pItemBase->byLv < pChar->m_byLvl+1))
					{
						m_nHpDrugID = pItem->pItemBase->nTypeID;
						SaveIntegerToXML("set_hpid",m_nHpDrugID);
						break;
					}
				}
			}
		}
		break;
	case 2:
		{
			if (nChgID)
			{
				ItemExBase* pDrugBase = ItemConfig::GetInstance().GetItemBase(nChgID);
				if ((pDrugBase->byType == 6)&&(pDrugBase->nUseType == 2))
				{
					m_nMpDrugID = nChgID;
					SaveIntegerToXML("set_mpid",m_nMpDrugID);
					break;
				}
			}

			if (pChar->m_pItemCenter->GetItemCountByID(m_nMpDrugID) == 0)
			{
				for (int i= 0;i<pBagList->Count;i++)
				{
					ItemObj* pItem = (ItemObj*)pBagList->Items[i];
					if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 2)&&(pItem->pItemBase->byLv < pChar->m_byLvl+1))
					{
						m_nMpDrugID = pItem->pItemBase->nTypeID;
						SaveIntegerToXML("set_mpid",m_nMpDrugID);
						break;
					}
				}
			}
		}
		break;
	case 3:
		{
			if (nChgID)
			{
				ItemExBase* pDrugBase = ItemConfig::GetInstance().GetItemBase(nChgID);
				if ((pDrugBase->byType == 6)&&(pDrugBase->nUseType == 5))
				{
					m_nPetHpDrugID = nChgID;
					SaveIntegerToXML("set_pethpid",m_nPetHpDrugID);
					break;
				}
			}

			MapAIObj* pSObj = g_pMapCenter->GetMapObj(MAPOBJ_SLAVE,pChar->m_pSlaveCenter->m_pActiveSlave->m_dwMapID);
			if (pSObj == NULL)
				break;

			if (pChar->m_pItemCenter->GetItemCountByID(m_nPetHpDrugID) == 0)
			{
				for (int i= 0;i<pBagList->Count;i++)
				{
					ItemObj* pItem = (ItemObj*)pBagList->Items[i];
					if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 5)&&(pItem->pItemBase->byLv < pSObj->byLv+1))
					{
						m_nPetHpDrugID = pItem->pItemBase->nTypeID;
						SaveIntegerToXML("set_pethpid",m_nPetHpDrugID);
						break;
					}
				}
			}
		}
		break;
	}

	pBagList->Free();
}

void AutoMenu::Update()
{
	if(g_pMapCenter->m_pMObjLockView == NULL)
		return;

	m_pFrom->GetComponent("kq")->SetVisible(!m_bActive);
	m_pFrom->GetComponent("qx")->SetVisible(m_bActive);

	m_pCheckAutoFix->SetEnable(true);
	m_pCheckAutoBuy->SetEnable(true);
	m_pCheckAutoSkill->SetEnable(true);

	VipInfoObj* pVipObj = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);
	if(m_bAutoBuy != m_pCheckAutoBuy->GetCheck()&&(pVipObj == NULL || !pVipObj->m_buyswitch)&&!m_bRecharge)
	{
		m_pCheckAutoBuy->SetCheck(false);
		m_pCheckAutoBuy->SetEnable(false);
		pMainMenu->PopAskMsgBox("Vip等级不足，是否升级Vip等级?",GoVipMenuEvent,(void*)0);
	}
	if(m_bAutoFix != m_pCheckAutoFix->GetCheck()&&(pVipObj == NULL || !pVipObj->m_fixswitch)&&!m_bRecharge)
	{
		m_pCheckAutoFix->SetCheck(false);
		m_pCheckAutoFix->SetEnable(false);
		pMainMenu->PopAskMsgBox("Vip等级不足，是否升级Vip等级?",GoVipMenuEvent,(void*)0);
	}

	if(m_bAutoSkill != m_pCheckAutoSkill->GetCheck()&&(pVipObj == NULL || !pVipObj->m_skillswitch)&&!m_bRecharge)
	{
		m_pCheckAutoSkill->SetCheck(false);
		m_pCheckAutoSkill->SetEnable(false);
		pMainMenu->PopAskMsgBox("Vip等级不足，是否升级Vip等级?",GoVipMenuEvent,(void*)0);
	}

	if (m_nHpDrugID)
	{
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(m_nHpDrugID);

		if (pBase)
		{
			int nCount = pChar->m_pItemCenter->GetItemCountByID(m_nHpDrugID);

			char	szTmp[32];
			sprintf(szTmp,"item/%d.png",pBase->nSpr);
			m_pFrom->GetComponent("icon1")->SetViewSpr(szTmp);
			m_pFrom->GetComponent("icon1")->SetCaptionEx("%d",2,nCount);

		}
	}
	if (m_nMpDrugID)
	{
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(m_nMpDrugID);

		if (pBase)
		{
			int nCount = pChar->m_pItemCenter->GetItemCountByID(m_nMpDrugID);

			char	szTmp[32];
			sprintf(szTmp,"item/%d.png",pBase->nSpr);
			m_pFrom->GetComponent("icon2")->SetViewSpr(szTmp);
			m_pFrom->GetComponent("icon2")->SetCaptionEx("%d",2,nCount);
		}
	}
	CheckSaveCfg();
	
	if (m_pFromPetEx->bOpened)
	{
		
		m_pFromPetEx->GetComponent("hp")->SetCaptionEx("%d",1,m_nValPetHp);

		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(m_nPetHpDrugID);

		int nCount = pChar->m_pItemCenter->GetItemCountByID(m_nPetHpDrugID);
		char	szTmp[32];
		sprintf(szTmp,"item/%d.png",pBase->nSpr);
		m_pFromPetEx->GetComponent("icon")->SetViewSpr(szTmp);
		m_pFromPetEx->GetComponent("icon")->SetCaptionEx("%d",2,nCount);
	}

	m_pFrom->GetComponent("hp")->SetCaptionEx("%d",1,m_nValSHp);
	m_pFrom->GetComponent("mp")->SetCaptionEx("%d",1,m_nValSMp);

	if (m_nHpDrugID&&(g_pMapCenter->m_pMObjLockView->nHP>0)&&(g_pMapCenter->m_pMObjLockView->nHP < m_nValSHp*g_pMapCenter->m_pMObjLockView->nMaxHP/100))
	{
		AutoSwitchDrug(1,m_nHpDrugID);

		ItemObj* pObj = pChar->m_pItemCenter->GetItemByTypeID(m_nHpDrugID);
		if (pObj&&pObj->pItemBase)
		{
			pChar->m_pItemCenter->UseItem(pObj);
		}
	}
	if (m_nMpDrugID&&(g_pMapCenter->m_pMObjLockView->nHP>0)&&(pChar->m_nMp < m_nValSMp*pChar->m_nMaxMp/100))
	{
		AutoSwitchDrug(2,m_nMpDrugID);

		ItemObj* pObj = pChar->m_pItemCenter->GetItemByTypeID(m_nMpDrugID);
		if (pObj&&pObj->pItemBase)
		{
			pChar->m_pItemCenter->UseItem(pObj);
		}
	}
	if (pChar&&(g_pMapCenter->m_pMObjLockView->nHP>0)&&pChar->m_pSlaveCenter&&pChar->m_pSlaveCenter->m_pActiveSlave)
	{
		MapAIObj* pSObj = g_pMapCenter->GetMapObj(MAPOBJ_SLAVE,pChar->m_pSlaveCenter->m_pActiveSlave->m_dwMapID);
		if(pSObj)
		{
			if (m_nPetHpDrugID&&(pSObj->nHP < m_nValPetHp*pSObj->nMaxHP/100))
			{
				AutoSwitchDrug(3,m_nPetHpDrugID);

				ItemObj* pObj = pChar->m_pItemCenter->GetItemByTypeID(m_nPetHpDrugID);
				if (pObj&&pObj->pItemBase)
				{
					pChar->m_pItemCenter->UseItem(pObj);
				}
			}
		}
	}
	if (pChar->m_pSlaveCenter->m_pActiveSlave&&m_bifResetPetState)
		CheckPetState();
	if(!m_bActive)
		return;
	static	DWORD	s_dwActiveTick	= xnGetTickCount();
	if(xnGetTickCount() - s_dwActiveTick > 200)
	{
		s_dwActiveTick	= xnGetTickCount();
		RunAuto();
	}

	if(xnGetTickCount() - m_dwAutoTeamTick > 600000)
	{
		AutoTeam();
	}

	if (m_bAutoBuy&&m_nHpDrugID&&!pChar->m_pItemCenter->GetItemCountByID(m_nHpDrugID)&&(xnGetTickCount() - s_dwBuyDrugTick > 5000))
	{
		s_dwBuyDrugTick	= xnGetTickCount();
		ShopItemCfg* pItem = ShopConfig::GetInstance().GetCfg(0,m_nHpDrugID);
		if (pItem)
		{
			int nMaxCount = pChar->m_dwMoney/pItem->m_nPrice;

			BuildPacketEx(pPacket, USER, C_AUTO_BUYDRUG, buffer,1024);
			SET_DATA(pdata, USER, C_AUTO_BUYDRUG, pPacket);
			pdata->nType	= pItem->m_byType;
			pdata->nItemID	= m_nHpDrugID;
			pdata->nCount = nMaxCount>10?10:nMaxCount;
			pChar->SendPacket(pPacket);
		}
	}
	if (m_bAutoBuy&&m_nMpDrugID&&!pChar->m_pItemCenter->GetItemCountByID(m_nMpDrugID)&&(xnGetTickCount() - s_dwBuyDrugTick > 5000))
	{
		s_dwBuyDrugTick	= xnGetTickCount();
		ShopItemCfg* pItem = ShopConfig::GetInstance().GetCfg(0,m_nMpDrugID);
		if (pItem)
		{
			int nMaxCount = pChar->m_dwMoney/pItem->m_nPrice;

			BuildPacketEx(pPacket, USER, C_AUTO_BUYDRUG, buffer,1024);
			SET_DATA(pdata, USER, C_AUTO_BUYDRUG, pPacket);
			pdata->nType	= pItem->m_byType;
			pdata->nItemID	= m_nMpDrugID;
			pdata->nCount = nMaxCount>10?10:nMaxCount;
			pChar->SendPacket(pPacket);
		}
	}
	if (m_bAutoBuy&&m_nPetHpDrugID&&!pChar->m_pItemCenter->GetItemCountByID(m_nPetHpDrugID)&&(xnGetTickCount() - s_dwBuyDrugTick > 5000))
	{
		s_dwBuyDrugTick	= xnGetTickCount();
		ShopItemCfg* pItem = ShopConfig::GetInstance().GetCfg(3,m_nPetHpDrugID);
		if (pItem)
		{
			int nMaxCount = pChar->m_dwMoney/pItem->m_nPrice;

			BuildPacketEx(pPacket, USER, C_AUTO_BUYDRUG, buffer,1024);
			SET_DATA(pdata, USER, C_AUTO_BUYDRUG, pPacket);
			pdata->nType	= pItem->m_byType;
			pdata->nItemID	= m_nPetHpDrugID;
			pdata->nCount = nMaxCount>10?10:nMaxCount;
			pChar->SendPacket(pPacket);
		}
	}
	if(m_bAutoFix && pChar->CheckFixEquip()&&(xnGetTickCount() - m_dwFixDrugTick > 5000))
	{
		m_dwFixDrugTick	= xnGetTickCount();
		pChar->SendCmd(USER,C_AUTO_FIX);
	}

}

void AutoMenu::RunAuto()
{
	if(!m_bActive)
		return;

	//回血
	MapAIObj* pMapObj	= pChar->GetMyMapObj();
	if(pMapObj == NULL)
		return;

	_checkHealth();
	_checkSlave();

	if(pMainMenu->IsCasting())
		return;

	//任务对话中
	if(pMainMenu->m_pMissionMenu->m_pFormTrack->bOpened)
		return;

	if(g_pMapCommand->m_bFromAuto==false && pMapObj->m_byState == OBJ_STATE_RUN)
		return;		//玩家操作中
	//战斗
	if(_checkSelObj())
	{
		//if(g_pMapCommand->m_wTrySkill == 0 || pChar->m_pSkillCenter->CheckUserSkill(g_pMapCommand->m_wTrySkill)==false)
		if(pMapObj->IsAtkCD() == false && pMainMenu->m_pCtrlMenu->m_pFormStrike->bOpened==false)
		{//不处于攻击CD状态 并且 切技能界面没打开
			if(pChar->m_dwLockSkill==0 || xnGetTickCount()-pChar->m_dwLockSkill >= 1000)
			{
				MapAIObj* pTarget	= ((MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj);
				if(pTarget->m_byType == MAPOBJ_ENEMY && pTarget->pBaseCfg && pTarget->pBaseCfg->bType == 3)
				{
					//采集怪不能用技能
				}
				else
				{
					for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
					{
						SkillObj* pSkill	= pChar->m_pSkillCenter->m_ppActSkill[(wSkillPos+i)%MAX_ACTIVE_SKILL];
						if(pSkill)
						{
							if(pChar->m_pSkillCenter->CheckUserSkill(pSkill->pSBase->wSkillID))
							{
								wSkillPos++;
								g_pMapCommand->CmdSkillAtk((MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj,pSkill->pSBase->wSkillID);
								return;
							}
						}
					}
				}
				g_pMapCommand->CmdNormalAtk((MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj,true);
			}
		}

		return;
	}

	if(g_pMapCommand->IsInRest() == false)
		return;

	_randWalk();
}

bool AutoMenu::_checkSelObj()
{
	MapAIObj* pMapObj	= pChar->GetMyMapObj();
	if(g_pMapCenter->m_pMObjSel)
	{
		MapAIObj* pObj	= (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;

		if(pObj->bForce && pObj->bForce != pMapObj->bForce && pObj->IsDead() == false 
			&& g_pMapCenter->m_pMapBarrier->CheckLineBlock(pMapObj->nPosX,pMapObj->nPosY,pObj->nPosX,pObj->nPosY) == false)
			return true;
	}

	g_pMapCenter->SearchNearObj(1,1000000);
	if(g_pMapCenter->m_pMObjSel)
		return true;

	return false;
}


bool AutoMenu::_checkSlave()
{

	return true;
}

bool AutoMenu::_checkHealth()
{


	return true;
}

void AutoMenu::_randWalk()
{
	MapAIObj* pMapObj	= pChar->GetMyMapObj();
	if(pMapObj->m_nMIndex != nMIndex)
		return;

	int	nDestPosX	= wStartX-800 + rand()%1600;
	int	nDestPosY	= wStartY-800 + rand()%1600;
	g_pMapCommand->CmdPoint(nDestPosX,nDestPosY,true,true);
}

void AutoMenu::_loadCfg()
{
	m_nHpDrugID		= LoadIntegerFromXML("set_hpid",30001);
	m_nMpDrugID		= LoadIntegerFromXML("set_mpid",30004);
	m_nPetHpDrugID		= LoadIntegerFromXML("set_pethpid",30012);

	m_nValSHp		= LoadIntegerFromXML("set_hp",60);
	m_nValSMp		= LoadIntegerFromXML("set_mp",60);

	m_bAutoTeam		= LoadBooleanFromXML("set_autoteam",true);
	m_bAutoRecover	= LoadBooleanFromXML("set_autorecover",false);
	

	m_bNoWhite		= LoadBooleanFromXML("set_nowhite",false);
	m_bNoBlue	    = LoadBooleanFromXML("set_noblue",false);
	m_bNoDrug		= LoadBooleanFromXML("set_nodrug",false);
	m_bNoMaterial	= LoadBooleanFromXML("set_nomaterial",false);

	m_nValPetHp		= LoadIntegerFromXML("set_pethp",60);
	m_bAutoAtk		= LoadBooleanFromXML("set_autoatk",false);
	m_bAutoAtkb	    = LoadBooleanFromXML("set_autoatkb",true);
	m_bAutoFlw		= LoadBooleanFromXML("set_autoflw",false);
	
	m_bAutoBuy	    = false;
	m_bAutoFix		= false;
	m_bAutoSkill	= false;
}

void AutoMenu::LoadPersonalCfg()
{
	char szName[32];
	sprintf(szName,"set_autobuy_%d",pChar->m_dwUin);
	m_bAutoBuy	    = LoadBooleanFromXML(szName,false);
	sprintf(szName,"set_autofix_%d",pChar->m_dwUin);
	m_bAutoFix		= LoadBooleanFromXML(szName,false);
	sprintf(szName,"set_autoskill_%d",pChar->m_dwUin);
	m_bAutoSkill	= LoadBooleanFromXML(szName,false);
	sprintf(szName,"set_lockplayer_%d",pChar->m_dwUin);
	m_bLockPlayer	= LoadBooleanFromXML(szName,false);
	m_pCheckAutoBuy->SetCheck(m_bAutoBuy);
	m_pCheckAutoFix->SetCheck(m_bAutoFix);
	m_pCheckAutoSkill->SetCheck(m_bAutoSkill);
	m_pCheckLockPlayer->SetCheck(m_bLockPlayer);
}

void AutoMenu::CheckSaveCfg()
{
	bool bChged	= false;

	if(m_nValSHp	!= m_pSlideHp->GetSlide())
	{
		m_nValSHp	= m_pSlideHp->GetSlide();
		SaveIntegerToXML("set_hp",m_nValSHp);
		bChged	= true;
	}
	if(m_nValSMp	!= m_pSlideMp->GetSlide())
	{
		m_nValSMp	= m_pSlideMp->GetSlide();
		SaveIntegerToXML("set_mp",m_nValSMp);
		bChged	= true;
	}
	if(m_bAutoAtk	!= m_pCheckAutoAtk->GetCheck())
	{
		if (!m_bAutoAtk)
		{
			m_pCheckAutoAtkb->SetCheck(false);
			m_pCheckAutoFlw->SetCheck(false);

		}
		m_bAutoAtk	= m_pCheckAutoAtk->GetCheck();
		SaveBooleanToXML("set_autoatk",m_bAutoAtk);
		bChged	= true;
		m_bifResetPetState = true;
	}
	if(m_bAutoAtkb	!= m_pCheckAutoAtkb->GetCheck())
	{
		if (!m_bAutoAtkb)
		{
			m_pCheckAutoAtk->SetCheck(false);
			m_pCheckAutoFlw->SetCheck(false);
		}
		m_bAutoAtkb	= m_pCheckAutoAtkb->GetCheck();
		SaveBooleanToXML("set_autoatkb",m_bAutoAtkb);
		bChged	= true;
		m_bifResetPetState = true;
	}
	if(m_bAutoFlw	!= m_pCheckAutoFlw->GetCheck())
	{
		if (!m_bAutoFlw)
		{
			m_pCheckAutoAtkb->SetCheck(false);
			m_pCheckAutoAtk->SetCheck(false);
		}
		m_bAutoFlw	= m_pCheckAutoFlw->GetCheck();
		SaveBooleanToXML("set_autoflw",m_bAutoFlw);
		bChged	= true;
		m_bifResetPetState = true;
	}
	if(m_nValPetHp	!= m_pSlidePetHp->GetSlide())
	{
		m_nValPetHp	= m_pSlidePetHp->GetSlide();
		SaveIntegerToXML("set_pethp",m_nValPetHp);
		bChged	= true;
	}
	if(m_bAutoTeam	!= m_pCheckAutoTeam->GetCheck())
	{
		m_bAutoTeam	= m_pCheckAutoTeam->GetCheck();
		SaveBooleanToXML("set_autoteam",m_bAutoTeam);
		bChged	= true;

		if (m_bAutoTeam)
		{
			m_dwAutoTeamTick = xnGetTickCount ();
		}
	}
	char szTempN[32];
	if(m_bAutoBuy	!= m_pCheckAutoBuy->GetCheck())
	{
		m_bAutoBuy	= m_pCheckAutoBuy->GetCheck();
		sprintf(szTempN,"set_autobuy_%d",pChar->m_dwUin);
		SaveBooleanToXML(szTempN,m_bAutoBuy);
		bChged	= true;
	}
	if(m_bAutoFix	!= m_pCheckAutoFix->GetCheck())
	{
		m_bAutoFix	= m_pCheckAutoFix->GetCheck();
		sprintf(szTempN,"set_autofix_%d",pChar->m_dwUin);
		SaveBooleanToXML(szTempN,m_bAutoFix);
		bChged	= true;
	}
	if(m_bAutoSkill	!= m_pCheckAutoSkill->GetCheck())
	{
		m_bAutoSkill	= m_pCheckAutoSkill->GetCheck();
		sprintf(szTempN,"set_autoskill_%d",pChar->m_dwUin);
		SaveBooleanToXML(szTempN,m_bAutoSkill);
		bChged	= true;
	}
	if(m_bLockPlayer	!= m_pCheckLockPlayer->GetCheck())
	{
		m_bLockPlayer	= m_pCheckLockPlayer->GetCheck();
		sprintf(szTempN,"set_lockplayer_%d",pChar->m_dwUin);
		SaveBooleanToXML(szTempN,m_bLockPlayer);
		bChged	= true;
	}

	if(m_bAutoRecover	!= m_pCheckAutoRecover->GetCheck())
	{
		m_bAutoRecover	= m_pCheckAutoRecover->GetCheck();
		SaveBooleanToXML("set_autorecover",m_bAutoRecover);
		bChged	= true;
	}

	if(m_bNoWhite	!= m_pNoPickUpWhiteEquip->GetCheck())
	{
		m_bNoWhite	= m_pNoPickUpWhiteEquip->GetCheck();
		SaveBooleanToXML("set_nowhite",m_bNoWhite);
		bChged	= true;
	}
	if(m_bNoBlue	!= m_pNoPickUpBlueEquip->GetCheck())
	{
		m_bNoBlue	= m_pNoPickUpBlueEquip->GetCheck();
		SaveBooleanToXML("set_noblue",m_bNoBlue);
		bChged	= true;
	}
	if(m_bNoDrug	!= m_pNoPickUpDrug->GetCheck())
	{
		m_bNoDrug	= m_pNoPickUpDrug->GetCheck();
		SaveBooleanToXML("set_nodrug",m_bNoDrug);
		bChged	= true;
	}
	if(m_bNoMaterial	!= m_pNoPickUpMaterial->GetCheck())
	{
		m_bNoMaterial	= m_pNoPickUpMaterial->GetCheck();
		SaveBooleanToXML("set_nomaterial",m_bNoMaterial);
		bChged	= true;
	}

	if(bChged)
		SaveToXMLOK();
}

void ClickChooseItem(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	if(param)
	{
		ChooseAutoDrug* pMenu = (ChooseAutoDrug*)param;
		if(pMenu->Func_Return && pMenu->m_pParam)
		{
			pMenu->Func_Return(pMenu->m_pRltItems,pMenu->m_pParam);
			pMenu->Close();
		}
	}
}

void CFmDrugChooseEvent(int nPos,char* szname,void* param)
{
	if(param)
	{
		ChooseAutoDrug* pMenu = (ChooseAutoDrug*)param;
		if(nPos >= 0 && nPos < pMenu->m_pListItems->Count)
		{
			ItemObj* pItem = (ItemObj*)pMenu->m_pListItems->Items[nPos];
			if (pItem->pItemBase&&pItem->pItemBase->byLv <= pChar->m_byLvl)
			{
				pMenu->AddRlt(pItem);
				pMenu->ReFresh();
			}else
				pMainMenu->PopTipMsg("等级不足，无法使用！");
		}
	}
}

ChooseAutoDrug::ChooseAutoDrug()
{
	m_pCDForm =  pMainInterface->GetDForm("xuanyw");
	m_pCDForm->bZorde = FORM_ZORDER_ITEMSHOW;

	m_pRltItems = xnList::Create();
	m_pListItems = xnList::Create();
	m_pParam = NULL;
	Func_Return = NULL;

	DComponent*	pCompBase	= m_pCDForm->GetComponent("mb");
	m_pCDForm->pCFrame	= new CCtrlFrame(pCompBase->nX-10,pCompBase->nY,pCompBase->nW+20,(pCompBase->nH+5)*3);
	DComponent*	ppCompPt[5];
	ppCompPt[0]		= m_pCDForm->GetComponent("icon");
	ppCompPt[1]		= m_pCDForm->GetComponent("name");
	ppCompPt[2]		= m_pCDForm->GetComponent("lv");
	ppCompPt[3]		= m_pCDForm->GetComponent("pz");
	ppCompPt[4]		= m_pCDForm->GetComponent("g");
	m_pCDForm->pCFrame->InitFParts(pCompBase,ppCompPt,5);
	m_pCDForm->pCFrame->nSpace	= 5;
	m_pCDForm->pCFrame->nDisStep	= pCompBase->nH+5;
	m_pCDForm->pCFrame->SetEventEx(CFmDrugChooseEvent,this);

	m_pCDForm->GetComponent("ok")->SetEvent(ClickChooseItem,this);

}

ChooseAutoDrug::~ChooseAutoDrug()
{
	if(m_pRltItems)
	{
		while(m_pRltItems->Count)
			m_pRltItems->Delete(0);
		m_pRltItems->Free();
	}
	if(m_pListItems)
	{
		while(m_pListItems->Count)
			m_pListItems->Delete(0);
		m_pListItems->Free();
	}
}

void ChooseAutoDrug::AddRlt(void* pData)
{

	if(m_pRltItems->Count&&(pData == m_pRltItems->Items[0]))
	{
		m_pRltItems->Delete(0);
		return;
	}
	if(m_pRltItems->Count&&(pData != m_pRltItems->Items[0]))
	{
		m_pRltItems->Delete(0);
		m_pRltItems->Add(pData);
		return;
	}
	m_pRltItems->Add(pData);
}

void ChooseAutoDrug::OpenDForm(int nType, void (*func)(xnList* pList,void* param),void* param, int nCount)
{
	while(m_pRltItems->Count)
		m_pRltItems->Delete(0);
	while(m_pListItems->Count)
		m_pListItems->Delete(0);

	m_pParam = param;
	Func_Return = func;

	CCtrlFrame*	pFrame	= m_pCDForm->pCFrame;
	pFrame->SetFPCnt(nCount,false);
	int		nIndex	= 0;

	xnList* pBagList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pBagList);

	switch(nType)
	{
	case 0:
		for (int i= 0;i<pBagList->Count;i++)
		{
			ItemObj* pItem = (ItemObj*)pBagList->Items[i];
			if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 1))
			{
				pFrame->SetFPTag(nIndex,pItem->pItemBase);
				pFrame->GetFPComp(nIndex,"g")->SetVisible(false);
				pFrame->GetFPComp(nIndex,"name")->SetCaption(pItem->pItemBase->szName,1);
				pFrame->GetFPComp(nIndex,"lv")->SetCaptionEx("%d", 1, pItem->pItemBase->byLv);
				pFrame->GetFPComp(nIndex,"pz")->SetCaptionEx("%d", 1, pItem->pItemBase->nUseVal);

				char	szTmp[32];
				sprintf(szTmp,"item/%d.png",pItem->pItemBase->nSpr);
				pFrame->GetFPComp(nIndex,"icon")->SetViewSpr(szTmp);

				nIndex++;

				m_pListItems->Add(pItem);
			}

		}
		break;
	case 1:
		for (int i= 0;i<pBagList->Count;i++)
		{
			ItemObj* pItem = (ItemObj*)pBagList->Items[i];
			if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 2))
			{
				pFrame->SetFPTag(nIndex,pItem->pItemBase);
				pFrame->GetFPComp(nIndex,"g")->SetVisible(false);
				pFrame->GetFPComp(nIndex,"name")->SetCaption(pItem->pItemBase->szName,1);
				pFrame->GetFPComp(nIndex,"lv")->SetCaptionEx("%d", 1, pItem->pItemBase->byLv);
				pFrame->GetFPComp(nIndex,"pz")->SetCaptionEx("%d", 1, pItem->pItemBase->nUseVal);

				char	szTmp[32];
				sprintf(szTmp,"item/%d.png",pItem->pItemBase->nSpr);
				pFrame->GetFPComp(nIndex,"icon")->SetViewSpr(szTmp);

				nIndex++;

				m_pListItems->Add(pItem);
			}

		}
		break;
	case 2:
		for (int i= 0;i<pBagList->Count;i++)
		{
			ItemObj* pItem = (ItemObj*)pBagList->Items[i];
			if ((pItem->pItemBase->byType == 6)&&(pItem->pItemBase->nUseType == 5))
			{
				pFrame->SetFPTag(nIndex,pItem->pItemBase);
				pFrame->GetFPComp(nIndex,"g")->SetVisible(false);
				pFrame->GetFPComp(nIndex,"name")->SetCaption(pItem->pItemBase->szName,1);
				pFrame->GetFPComp(nIndex,"lv")->SetCaptionEx("%d", 1, pItem->pItemBase->byLv);
				pFrame->GetFPComp(nIndex,"pz")->SetCaptionEx("%d", 1, pItem->pItemBase->nUseVal);

				char	szTmp[32];
				sprintf(szTmp,"item/%d.png",pItem->pItemBase->nSpr);
				pFrame->GetFPComp(nIndex,"icon")->SetViewSpr(szTmp);

				nIndex++;

				m_pListItems->Add(pItem);
			}

		}
		break;
	}
	m_pCDForm->Open(2);
	ReFresh();
}

void ChooseAutoDrug::ReFresh()
{
	CCtrlFrame*	pFrame	= m_pCDForm->pCFrame;
	pFrame->SetFPCnt(m_pListItems->Count,false);
	for(int i = 0; i <m_pListItems->Count; i ++)
	{
		ItemObj* pItem = (ItemObj*)m_pListItems->Items[i];
		pFrame->GetFPComp(i,"g")->SetVisible(false);
		pFrame->GetFPComp(i,"name")->SetCaption(pItem->pItemBase->szName,1);
		pFrame->GetFPComp(i,"lv")->SetCaptionEx("%d", 1, pItem->pItemBase->byLv);
		pFrame->GetFPComp(i,"pz")->SetCaptionEx("%d", 1, pItem->pItemBase->nUseVal);

		char	szTmp[32];
		sprintf(szTmp,"item/%d.png",pItem->pItemBase->nSpr);
		pFrame->GetFPComp(i,"icon")->SetViewSpr(szTmp);
		if(m_pRltItems->Count&&(pItem == (ItemObj*)m_pRltItems->Items[0]))
		{
			pFrame->GetFPComp(i,"g")->SetVisible(true);
		}
	}
}

void ChooseAutoDrug::Close()
{
	m_pCDForm->Close();
}