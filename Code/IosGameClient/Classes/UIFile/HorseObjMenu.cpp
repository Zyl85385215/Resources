#include "UserData.h"
#include "HorseObjMenu.h"
#include "MainMenu.h"


HorseMainMenu::HorseMainMenu(char* pName) : SlaveMenuObj(pName,1)
{
	if(m_pForm)
	{
		
	}
}

HorseMainMenu::~HorseMainMenu()
{

}

void HorseMainMenu::Open(int nT /* = 1 */)
{
	if(pChar->m_pClientFlag->GetFlag(3) < 2)
	{
		pMainMenu->PopTipMsg("等级达到18级，完成任务<风暴傀儡>后才能开启坐骑界面。");
		return;
	}
	m_pForm->Open(nT);
}

void ClickFeed1Button(InterfaceEvent*pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseFeedMenu* pMenu = (HorseFeedMenu*)param;
		pChar->m_pHorseCenter->RequestFeedHorse(pMenu->byCur,pMenu->m_ifSenior,1);
	}
}

void ClickFeed2Button(InterfaceEvent*pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseFeedMenu* pMenu = (HorseFeedMenu*)param;
		int nId = 20002;
		if(pMenu->m_ifSenior)
			nId = 20003;
		pChar->m_pHorseCenter->RequestFeedHorse(pMenu->byCur,pMenu->m_ifSenior,pChar->m_pItemCenter->GetItemCountByID(nId));
	}
}

void ClickFIconButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pHorseFeedMenu->m_ifSenior = (int)param;
}

HorseFeedMenu::HorseFeedMenu(char* pName) : SlaveMenuObj(pName,1)
{
	if(m_pForm)
	{
		m_ifSenior = false;
		m_pPro = m_pForm->GetComponent("dj");
		m_pChoose[0] = m_pForm->GetComponent("choose1");
		m_pChoose[1] = m_pForm->GetComponent("choose2");
		m_pForm->GetComponent("wy1")->SetEvent(ClickFeed1Button,this);
		m_pForm->GetComponent("wy2")->SetEvent(ClickFeed2Button,this);
		m_pForm->GetComponent("c1")->SetEvent(ClickFIconButton,(void*)0);
		m_pForm->GetComponent("c2")->SetEvent(ClickFIconButton,(void*)1);
		m_pForm->GetComponent("lv")->m_nFontSize = 40;
	}
}

HorseFeedMenu::~HorseFeedMenu()
{

}

void HorseFeedMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		HorseCData* pHorse = NULL;
		if(byCur > 0 && byCur <= pChar->m_pHorseCenter->m_pHorseList->Count)
			pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[byCur-1];
		if(pHorse)
		{
			int nMax = HorseBaseCfg::GetInstance().GetLvExp(pHorse->m_byLevel);
			m_pPro->SetProgress(pHorse->m_nExp,nMax);
			DComponent* pComp = m_pForm->GetComponent("model2");
			if(pComp)
			{
				char szList[64];
				char szFile[64];
				sprintf(szList,"skin/%d/1.plist",pHorse->m_pSkinObjNext->nRealID);
				sprintf(szFile,"skin/%d/hstand/3",pHorse->m_pSkinObjNext->nRealID);
				pComp->SetCartoonAction(szList,szFile,600,pHorse->m_pSkinObjNext?pHorse->m_pSkinObjNext->nScale:100);
			}
		}

		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(20002);
		if (pBase)
		{
			char szIcon[32];
			sprintf(szIcon,"item/%d.png",pBase->nSpr);
			m_pForm->GetComponent("c1")->SetViewSpr(szIcon);
			m_pForm->GetComponent("c1")->SetCaptionEx("%d",2,pChar->m_pItemCenter->GetItemCountByID(20002));
		}
		pBase = ItemConfig::GetInstance().GetItemBase(20003);
		if (pBase)
		{
			char szIcon[32];
			sprintf(szIcon,"item/%d.png",pBase->nSpr);
			m_pForm->GetComponent("c2")->SetViewSpr(szIcon);
			m_pForm->GetComponent("c2")->SetCaptionEx("%d",2,pChar->m_pItemCenter->GetItemCountByID(20003));
		} 		
		if(m_ifSenior)
		{
			m_pForm->GetComponent("c1")->SetAddSprByTag("",0,0,1);
			m_pForm->GetComponent("c2")->SetAddSprByTag("MB_XZ1.png",-15,-15,1);
		}
		else
		{
			m_pForm->GetComponent("c1")->SetAddSprByTag("MB_XZ1.png",-15,-15,1);
			m_pForm->GetComponent("c2")->SetAddSprByTag("",0,0,1);
		}
		if(m_pChoose[0] && m_pChoose[1])
		{
			if(m_ifSenior)
			{
				m_pChoose[1]->SetVisible(true);
				m_pChoose[0]->SetVisible(false);
			}
			else
			{
				m_pChoose[0]->SetVisible(true);
				m_pChoose[1]->SetVisible(false);
			}
		}
	}
}

void ClickCultivateButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseCultivateMenu* pMenu = (HorseCultivateMenu*)param;
		int x = pMenu->m_pRadio->GetSel();
		pChar->m_pHorseCenter->RequestCultivateHorse(pMenu->byCur,pMenu->m_pRadio->GetSel());
	}
}

void ClickAcceptButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseCultivateMenu* pMenu = (HorseCultivateMenu*)param;
		pChar->m_pHorseCenter->RequestCultivateRlt(pMenu->byCur,true);
	}
}

void ClickCancelButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseCultivateMenu* pMenu = (HorseCultivateMenu*)param;
		pChar->m_pHorseCenter->RequestCultivateRlt(pMenu->byCur,false);
	}
}

HorseCultivateMenu::HorseCultivateMenu(char* pName) : SlaveMenuObj(pName,1)
{
	if(m_pForm)
	{
		DComponent* pComp[4];
		pComp[0] = m_pForm->GetComponent("g1");
		pComp[1] = m_pForm->GetComponent("g2");
		pComp[2] = m_pForm->GetComponent("g3");
		pComp[3] = m_pForm->GetComponent("g4");
		m_pRadio = new CCtrlRadio(pComp,4);

		m_pComStart = m_pForm->GetComponent("py");
		m_pComAccept = m_pForm->GetComponent("wy1");
		m_pComGiveup = m_pForm->GetComponent("wy2");
		m_pForm->GetComponent("py")->SetEvent(ClickCultivateButton,this);
		m_pForm->GetComponent("wy1")->SetEvent(ClickAcceptButton,this);
		m_pForm->GetComponent("wy2")->SetEvent(ClickCancelButton,this);
	}
}

HorseCultivateMenu::~HorseCultivateMenu()
{

}

void HorseCultivateMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		HorseCData* pHorse = NULL;
		if(byCur > 0 && byCur <= pChar->m_pHorseCenter->m_pHorseList->Count)
			pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[byCur-1];
		DComponent* pCompAdd[6];
		pCompAdd[0] = m_pForm->GetComponent("hp2");
		pCompAdd[1] = m_pForm->GetComponent("mp2");
		pCompAdd[2] = m_pForm->GetComponent("wg2");
		pCompAdd[4] = m_pForm->GetComponent("mg2");
		pCompAdd[3] = m_pForm->GetComponent("wf2");
		pCompAdd[5] = m_pForm->GetComponent("mf2");
		if(pHorse)
		{
			for(int i = 0; i < 6; i ++)
			{
				if(pHorse->m_szTempCul[i] > 0)
				{
					pCompAdd[i]->SetCaptionEx("+%d",0,pHorse->m_szTempCul[i]);
					pCompAdd[i]->SetSprColor(0,220,0);
				}
				else if(pHorse->m_szTempCul[i] < 0)
				{
					pCompAdd[i]->SetCaptionEx("%d",0,pHorse->m_szTempCul[i]);
					pCompAdd[i]->SetSprColor(220,0,0);
				}
				else
				{
					pCompAdd[i]->SetCaption("",0);
				}
			}
			if(pHorse->m_szTempCul[HORSE_CULTIVATE_TOTAL] != 0)
			{
				m_pComStart->SetVisible(false);
				m_pComAccept->SetVisible(true);
				m_pComGiveup->SetVisible(true);
			}
			else
			{
				m_pComStart->SetVisible(true);
				m_pComAccept->SetVisible(false);
				m_pComGiveup->SetVisible(false);
			}
		}
		else
		{
			for(int i = 0; i < 6; i ++)
			{
				pCompAdd[i]->SetCaption("",0);
			}
			m_pComStart->SetVisible(false);
			m_pComAccept->SetVisible(false);
			m_pComGiveup->SetVisible(false);
		}
		m_pForm->GetComponent("pyd")->SetCaptionEx("%d",1,pChar->m_pItemCenter->GetItemCountByID(20006));
		int nUseNum = m_pRadio->GetSel();
		if (nUseNum == 1||nUseNum == 3)
			m_pForm->GetComponent("jb")->SetViewSpr("MB_ZUANSHI.png");
		else
			m_pForm->GetComponent("jb")->SetViewSpr("MB_MONEY.png");

		if (nUseNum == 0)
			m_pForm->GetComponent("money")->SetCaption("10000",1);
		else if (nUseNum == 1)
			m_pForm->GetComponent("money")->SetCaption("4",1);
		else if (nUseNum == 2)
			m_pForm->GetComponent("money")->SetCaption("100000",1);
		else if (nUseNum == 3)
			m_pForm->GetComponent("money")->SetCaption("40",1);
	}
}

bool HorseCultivateMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 35)
	{
		pMainMenu->PopTipMsg("等级达到35级开启培养功能");
		return false;
	}
	return true;
}

void ChooseHorse2EatFunc(xnList* pList,void* param)
{
	if(pList == NULL || param == NULL)
		return;
	HorseEatMenu * pMenu = (HorseEatMenu*)param;
	pMenu->m_nCnt = pList->Count;
	for(int i = 0; i < pList->Count && i < 4; i ++)
	{
		HorseCData* pHorse = (HorseCData*)pList->Items[i];
		pMenu->szID[i] = pHorse->m_dwID;
	}
}

void Click2HChooseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseEatMenu* pMenu = (HorseEatMenu*)param;
		int nID = 0;
		if(pMenu->byCur > 0 && pMenu->byCur <= pChar->m_pHorseCenter->m_pHorseList->Count)
		{
			HorseCData*pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[pMenu->byCur-1];
			nID = pHorse->m_dwID;
		}
		pMainMenu->m_pHorseChooseMenu->Open(nID,ChooseHorse2EatFunc,(void*)pMenu,4);
	}
}

void cb_CheckGoOn(void* pParam, int nVal)
{
	if (nVal)
	{
		HorseEatMenu* pMenu = (HorseEatMenu*)pParam;
		if (pMenu)
		{
			pChar->m_pHorseCenter->RequestEatHorse(pMenu->byCur,pMenu->szID,pMenu->m_nCnt);
			memset(pMenu->szID,0,4*4);
			pMenu->m_nCnt = 0;
		}
	}
}

void ClickHorseEatOkButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseEatMenu* pMenu = (HorseEatMenu*)param;

		if (pMenu->byCur == 0 || pMenu->byCur > pChar->m_pHorseCenter->m_pHorseList->Count)
		{
			pMainMenu->PopTipMsg("坐骑不存在");
			return;
		}

		HorseCData* pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[pMenu->byCur-1];
		if (pHorse&&pHorse->m_nEatLimt > 2999)
		{
			pMainMenu->PopAskMsgBox("吞噬可获得潜力值已满，再吞噬将只能获得经验值，是否继续吞噬？",cb_CheckGoOn,param);
		}else
		{
			pChar->m_pHorseCenter->RequestEatHorse(pMenu->byCur,pMenu->szID,pMenu->m_nCnt);
			memset(pMenu->szID,0,4*4);
			pMenu->m_nCnt = 0;
		}
	}
}

HorseEatMenu::HorseEatMenu(char* pName) : SlaveMenuObj(pName,1)
{
	if(m_pForm)
	{
		memset(szID,0,4*4);
		m_nCnt = 0;

		m_pForm->GetComponent("ts")->SetEvent(ClickHorseEatOkButton,this);
		char szname[32];
		for(int i = 0; i < 4; i ++)
		{
			sprintf(szname,"c%d",i+1);
			m_pForm->GetComponent(szname)->SetEvent(Click2HChooseButton,this);
		}
	}
}

HorseEatMenu::~HorseEatMenu()
{

}

void HorseEatMenu::Open(int nT /* = 1 */)
{
	memset(szID,0,4*4);
	m_nCnt = 0;
	m_pForm->Open(nT);
}

void HorseEatMenu::CFmEvent()
{
	memset(szID,0,4*4);
	m_nCnt = 0;
}

void HorseEatMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		HorseCData* pHorse = NULL;
		static int nHHCount = 0;
		if(byCur > 0 && byCur <= pChar->m_pHorseCenter->m_pHorseList->Count)
			pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[byCur-1];
		bool ifShowRlt = false;
		if(pHorse)
		{
			int nExp = 0;
			int nCul = 0;
			int nTempCul = pHorse->m_nEatLimt;
			for(int i = 0; i < 4; i++)
			{
				char szName[32];
				sprintf(szName,"c%d",i+1);
				DComponent* pCom = m_pForm->GetComponent(szName);
				HorseCData* pFood = NULL;
				if(i < m_nCnt)
					pFood = pChar->m_pHorseCenter->GetHorse(szID[i]);
				//===zjk
				m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,pHorse->m_byLevel);
				m_pForm->GetComponent("lv")->m_nFontSize = 24;
				m_pForm->GetComponent("lv")->SetSprColor(255, 255, 255);
				m_pForm->GetComponent("lv2")->m_nFontSize = 24;

				if(pFood)
				{
					ifShowRlt = true;
					g_pSlaveMenuCtrl->ShowIcon(pCom,pFood->m_nType,1);
					pCom->byRunAction = 0;
					int nTemp = HorseBaseCfg::GetInstance().GetFeedExp(pFood->m_byLevel,pFood->m_nExp);
					switch(pFood->m_pBasic->byQuality)
					{
					case 1:
						{
							nTemp = nTemp * 70 / 100;
						}
						break;
					case 2:
						{
							nTemp = nTemp * 90 / 100;
							if (pFood->m_pBasic->nID == 1002)
							{
								nCul += 50;
								nTempCul +=50;
							}
						}
						break;
					case 3:
						{
							if (nTempCul < 3000)
							{
								nCul += 100;
								nTempCul += 100;
							}
						}
						break;
					default:
						{
							nTemp = nTemp * 50 / 100;
						}
						break;

					}
					if(nTemp == 0)
						nTemp = 1;
					nExp += nTemp;
				}
				else
				{
					if(nHHCount%50 == 0)
					{
						pCom->SetViewSpr("BT_CW_JIA.png");
						pCom->byRunAction = 21;
					}
				}
			}
			if(ifShowRlt)
			{
				int nLv = pHorse->AddExp(nExp,false);
				m_pForm->GetComponent("lv2")->SetCaptionEx("%d",0,nLv);					
				m_pForm->GetComponent("ql2")->SetCaptionEx("%d",0,pHorse->m_nPotential[HORSE_CULTIVATE_TOTAL]+nCul);
			}
			else
			{
				m_pForm->GetComponent("lv2")->SetCaption("",0);				
				m_pForm->GetComponent("ql2")->SetCaption("",0);
			}
		}
		else
		{
			m_pForm->GetComponent("lv2")->SetCaption("",0);				
			m_pForm->GetComponent("ql2")->SetCaption("",0);
			for(int i = 0; i < 4; i ++)
			{
				char szName[32];
				sprintf(szName,"c%d",i+1);
				m_pForm->GetComponent((char*)szName)->SetViewSpr("");
			}
		}
		nHHCount ++;
	}
}

bool HorseEatMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 45)
	{
		pMainMenu->PopTipMsg("等级达到45级开启吞噬功能");
		return false;
	}
	return true;
}


void CFmHorseChooseEvent(int nPos,char* szname,void* param)
{
	if(param)
	{
		HorseChooseMenu* pMenu = (HorseChooseMenu*)param;
		if(nPos >= 0 && nPos < pMenu->m_pShowList->Count)
		{
			pMenu->AddRlt( pMenu->m_pShowList->Items[nPos]);
			pMenu->ReFresh();
		}
	}
}

void ClickHorseChooseOKButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		HorseChooseMenu* pMenu = (HorseChooseMenu*)param;
		if(pMenu->Func_Return && pMenu->m_pParam)
		{
			pMenu->Func_Return(pMenu->m_pRltList,pMenu->m_pParam);
			pMenu->Close();
		}
	}
}

HorseChooseMenu::HorseChooseMenu()
{
	m_pForm = pMainInterface->GetDForm("xuanzq");
	m_pRltList = xnList::Create();
	m_pShowList = xnList::Create();
	nIgnoreID = 0;
	m_nChooseLimit = 0;
	m_pParam = NULL;
	Func_Return = NULL;
	DComponent* pCompBase = m_pForm->GetComponent("mb");
	DComponent* ppComp[5];
	ppComp[0] = m_pForm->GetComponent("icon");
	ppComp[1] = m_pForm->GetComponent("name");
	ppComp[2] = m_pForm->GetComponent("pz");
	ppComp[3] = m_pForm->GetComponent("lv");
	m_pForm->GetComponent("lv")->m_nFontSize = 24;
	m_pForm->GetComponent("lv")->SetSprColor(255,255,255);
	ppComp[4] = m_pForm->GetComponent("g");
	m_pForm->pCFrame = new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,pCompBase->nH*3);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,5);
	m_pForm->pCFrame->nDisStep = pCompBase->nH;
	m_pForm->pCFrame->SetEventEx(CFmHorseChooseEvent,this);

	m_pForm->GetComponent("ok")->SetEvent(ClickHorseChooseOKButton,this);
}

HorseChooseMenu::~HorseChooseMenu()
{
	if(m_pRltList)
	{
		while(m_pRltList->Count)
			m_pRltList->Delete(0);
		m_pRltList->Free();
	}
	if(m_pShowList)
	{
		while(m_pShowList->Count)
			m_pShowList->Delete(0);
		m_pShowList->Free();
	}
}

void HorseChooseMenu::AddRlt(void* pData)
{
	for(int i = m_pRltList->Count - 1; i >= 0; i --)
	{
		if(pData == m_pRltList->Items[i])
		{
			m_pRltList->Delete(i);
			return;
		}
	}
	if(m_pRltList->Count >= m_nChooseLimit)
		return;
	m_pRltList->Add(pData);
}

void HorseChooseMenu::Open(int nID,void (*func)(xnList* pList,void* param),void* param,int nLimit)
{
	while(m_pRltList->Count)
		m_pRltList->Delete(0);
	while(m_pShowList->Count)
		m_pShowList->Delete(0);
	nIgnoreID = nID;
	m_pParam = param;
	m_nChooseLimit = nLimit;
	Func_Return = func;
	for(int i = 0; i < pChar->m_pHorseCenter->m_pHorseList->Count; i ++)
	{
		HorseCData* pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[i];
		//if(pHorse->m_dwID == nIgnoreID || pHorse->m_byLevel <= 1)
		if(pHorse->m_dwID == nIgnoreID || pHorse->m_ifEquip)
			continue;
		m_pShowList->Add((void*)pHorse);
	}
	m_pForm->Open();
	ReFresh();
}

void HorseChooseMenu::Close()
{
	m_pForm->Close();
}

void HorseChooseMenu::ReFresh()
{
	m_pForm->pCFrame->SetFPCnt(m_pShowList->Count);
	DComponent* pIcon = NULL;
	DComponent* pName = NULL;
	DComponent* pQuality = NULL;
	DComponent* pLV = NULL;
	//pLV->m_nFontSize  = 48;//zjk

	DComponent* pCheck = NULL;
	for(int i = 0; i <m_pShowList->Count; i ++)
	{
		pIcon = m_pForm->pCFrame->GetFPComp(i,"icon");
		pName = m_pForm->pCFrame->GetFPComp(i,"name");
		pQuality = m_pForm->pCFrame->GetFPComp(i,"pz");
		pLV = m_pForm->pCFrame->GetFPComp(i,"lv");	
		pLV->m_nFontSize  = 24;
		pCheck = m_pForm->pCFrame->GetFPComp(i,"g");
		pCheck->SetVisible(false);
		HorseCData* pHorse = (HorseCData*)m_pShowList->Items[i];
		char szIcon[32];
		sprintf(szIcon,"head/%d.png",pHorse->m_pSkinObj->nHead);
		pIcon->SetViewSpr(szIcon);
		pName->SetCaption(pHorse->m_pName,1);
		pName->SetSprColor(nItemColor[pHorse->m_pBasic->byQuality][0],nItemColor[pHorse->m_pBasic->byQuality][1],nItemColor[pHorse->m_pBasic->byQuality][2]);
		pQuality->SetCaptionEx("%s",1,s_qualityName[pHorse->m_pBasic->byQuality]);
		pLV->SetCaptionEx("%d",1,pHorse->m_byLevel);
		for(int k = 0; k < m_pRltList->Count; k ++)
		{
			if(pHorse == (HorseCData*)m_pRltList->Items[k])
			{
				pCheck->SetVisible(true);
				break;
			}

		}
	}
}