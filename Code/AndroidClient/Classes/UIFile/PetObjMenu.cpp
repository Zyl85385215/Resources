#include "UserData.h"
#include "PetObjMenu.h"
#include "MainMenu.h"
#include "ItemConfig.h"
#include "SlaveFormCtrl.h"

extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);

void PetGoMallEvent2(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pActivityCtrl->Open(0);
}

void CFPetSkillClick(int nPos,char * pName,void* param)
{
	if(param == NULL)
		return;
	PetSkillMenu* pMenu = (PetSkillMenu*)param;
	if(strncmp(pName,"sk",2)==0)
	{
		int i = atoi(&pName[2]);
		if(nPos*10+i-1 >= pMenu->m_pBookList->Count)
		{
			pMainMenu->PopAskMsgBox("宠物技能书不足，可通过“商城”购买或者参加活动“钓鱼大赛”获得。",PetGoMallEvent2,(void*)0, 5);
			return;
		}
		pMenu->m_nChosen = nPos*10+i-1;
		ItemObj* pItem = (ItemObj*)pMenu->m_pBookList->Items[pMenu->m_nChosen];
		if (pItem)		
			pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_PETSKILL);
	}
}

void ClickPetLearnButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	PetSkillMenu* pMenu = (PetSkillMenu*)param;
	if(pMenu)
	{
		int nItem = 0;
		if(pMenu->m_nChosen < pMenu->m_pBookList->Count)
		{
			ItemObj* pItem = (ItemObj*)pMenu->m_pBookList->Items[pMenu->m_nChosen];
			nItem = pItem->GetVal(ITEM_AD_ID);
		}
		pChar->m_pSlaveCenter->RequestLearnSkill(pMenu->byCur,nItem);
	}
}

PetMainMenu::PetMainMenu(char* pName):SlaveMenuObj(pName,0)
{
	if(m_pForm)
	{
		
	}
	
}

PetMainMenu::~PetMainMenu()
{

}

void PetMainMenu::Open(int nT /* = 1 */)
{
	if(pChar->m_pClientFlag->GetFlag(3) < 1)
	{
		pMainMenu->PopTipMsg("等级达到12级，完成任务<宠物>后开启宠物界面。");
		return;
	}
	
	m_pForm->Open(nT);
}

PetSkillMenu::PetSkillMenu(char* pName):SlaveMenuObj(pName,0)
{
	if(m_pForm)
	{
		
		m_byBookPage = 0;
		m_nChosen = 0;
		m_pBookForm = pMainInterface->GetDForm("cwskil2");
		DComponent* pBase = m_pBookForm->GetComponent("qq");
		DComponent* ppCom[20];
		char szName[32];
		for(int i = 0; i < 10; i ++)
		{
			sprintf(szName,"sk%d",i+1);
			ppCom[i+10] = m_pBookForm->GetComponent(szName);
			sprintf(szName,"s%d",i+1);
			ppCom[i] = m_pBookForm->GetComponent(szName);
		}
		m_pBookForm->pCFrame = new CCtrlFrame(pBase->nX,pBase->nY,pBase->nW,pBase->nH);
		m_pBookForm->pCFrame->InitFParts(pBase,ppCom,20);

		m_pBookForm->pCFrame->nDisStep = pBase->nW;
		m_pBookForm->pCFrame->SetFPCnt(5,true);
		m_pBookForm->pCFrame->SetEventEx(CFPetSkillClick,this);
		
		m_pBookList = xnList::Create();
		m_pForm->GetComponent("xuexi")->SetEvent(ClickPetLearnButton,this);
	}
}

PetSkillMenu::~PetSkillMenu()
{
	if(m_pBookList)
	{
		while(m_pBookList->Count)
			m_pBookList->Delete(0);
		m_pBookList->Free();
	}
}

void PetSkillMenu::Open(int nT /* = 1 */)
{		
	m_pForm->Open(nT);
	m_pBookForm->Open();

	m_nChosen = 0;
}

void PetSkillMenu::Close()
{
	m_pBookForm->Close();
	m_pForm->Close();
}

void PetSkillMenu::Update()
{
	if(m_pBookForm && m_pBookForm->bOpened)
	{
		pChar->m_pItemCenter->GetItemsByKind(ITEMTYPE_PETSKILL,m_pBookList);
		for(int i = 0; i < 50; i ++)
		{
			int nPage = i / 10;
			char szName[32];
			sprintf(szName,"sk%d",i%10+1);
			DComponent* pComp = m_pBookForm->pCFrame->GetFPComp(nPage,szName);
			if(i < m_pBookList->Count)
			{
				ItemObj* pItem = (ItemObj*)m_pBookList->Items[i];
				//显示物品图标
				SetItemToDCom(pComp,pItem);
				//
			}
			else
			{
				pComp->SetEffect(0,1);
				pComp->SetViewSpr("");
				pComp->SetCaption("",1);
			}
			if(i == m_nChosen)
				pComp->SetAddSprByTag("MB_XZ1.png",-15,-15,1);
			else
				pComp->SetAddSprByTag("",-15,-15,1);
		}
		
	}
	/*
	if(m_pForm && m_pForm->bOpened)
		{
			SlaveCData* pSlave = NULL;
			if(byCur > 0 && byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
				pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[byCur-1];
			char szIcon[32];
			char szButton[32];
			for(int i = 0; i < 6; i ++)
			{
				sprintf(szButton,"cw%d",i+1);
				if(pSlave && pSlave->m_nSkill[i] > 0)
				{
					//显示技能图标
					//
				}
				else
				{
					m_pForm->GetComponent(szButton)->SetViewSpr("");
				}
	
			}
		}*/
	
}

void ClickPetStreButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param == NULL)
		return;
	PetStrengthenMenu*pMenu = (PetStrengthenMenu*)param;
	pChar->m_pSlaveCenter->RequestStrengthen(pMenu->byCur,pMenu->m_pRadio->GetSel());
}

PetStrengthenMenu::PetStrengthenMenu(char* pName):SlaveMenuObj(pName,0)
{
	if(m_pForm)
	{
		
		DComponent* ppCom[6];
		char szName[32];
		for(int i = 0; i < 6; i ++)
		{
			int n = i;
			if(i == 3)
				n = 4;
			else if( i == 4)
				n = 3;
			sprintf(szName,"g%d",n+1);
			ppCom[i] = m_pForm->GetComponent(szName);
		}
		m_pRadio = new CCtrlRadio(ppCom,6);
		m_pForm->GetComponent("qh")->SetEvent(ClickPetStreButton,this);
		m_pCompUp[0] = m_pForm->GetComponent("hp2");
		m_pCompUp[1] = m_pForm->GetComponent("mp2");
		m_pCompUp[2] = m_pForm->GetComponent("wg2");
		m_pCompUp[3] = m_pForm->GetComponent("wf2");
		m_pCompUp[4] = m_pForm->GetComponent("mg2");
		m_pCompUp[5] = m_pForm->GetComponent("mf2");
	}
}

PetStrengthenMenu::~PetStrengthenMenu()
{

}

void PetStrengthenMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		//m_pForm->GetComponent("icon")->SetCaptionEx("%d",2,pChar->m_pItemCenter->GetItemCountByID(20004));
		ShowItemInfoOnComp(m_pForm->GetComponent("icon"),20004,pChar->m_pItemCenter->GetItemCountByID(20004));
		SlaveCData* pSlave = NULL;
		if(byCur > 0 && byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
			pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[byCur-1];
		int nPos = m_pRadio->GetSel();
		if(pSlave)
		{
			for(int i = 0; i < SLAVE_STRENGTHEN_MAX; i ++)
			{
				SlaveStreBasic* pStb = SlaveStrengthenCfg::GetInstance().GetBase(i,pSlave->m_nStrengthen[i]);
				BYTE byColorType = 0;
				if(pStb)
				{
					if(pSlave->m_nStrengthen[i] > 50)
						byColorType = 1;
					else if(pSlave->m_nStrengthen[i] > 110)
						byColorType = 2;
					else if(pSlave->m_nStrengthen[i] > 180)
						byColorType = 3;
					m_pCompUp[i]->SetCaptionEx("+%d%%",0,pStb->nVal);	
				}
				else
				{
					m_pCompUp[i]->SetCaption("+0%",0);
				}
				m_pCompUp[i]->SetSprColor(nItemColor[byColorType][0],nItemColor[byColorType][1],nItemColor[byColorType][2]);
				
				if(i == nPos)
				{
					pStb = SlaveStrengthenCfg::GetInstance().GetBase(i,pSlave->m_nStrengthen[i]+1);
					if(pStb)
					{
						m_pForm->GetComponent("num")->SetCaptionEx("%d",0,pStb->nItemCost);
						m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,pStb->nLvNeed);
						m_pForm->GetComponent("lv")->m_nFontSize = 20;    //===zjk
						m_pForm->GetComponent("lv")->SetSprColor(255,255,255);
						m_pForm->GetComponent("cgl")->SetCaptionEx("%d%%",0,pStb->nSucPer);
						m_pForm->GetComponent("money")->SetCaptionEx("%d",0,pStb->nMoneyCost);
					}
					else
					{
						m_pForm->GetComponent("num")->SetCaptionEx("%d",0,0);
						m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,0);
						m_pForm->GetComponent("lv")->m_nFontSize = 20;
						m_pForm->GetComponent("lv")->SetSprColor(255,255,255);
						m_pForm->GetComponent("cgl")->SetCaptionEx("%d%%",0,0);
						m_pForm->GetComponent("money")->SetCaptionEx("%d",0,0);
					}
				}
				//显示属性加成
			}
			
		}
		else
		{
			m_pForm->GetComponent("num")->SetCaptionEx("%d",0,0);
			m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,0);
			m_pForm->GetComponent("lv")->m_nFontSize = 20;
			m_pForm->GetComponent("lv")->SetSprColor(255,255,255);
			m_pForm->GetComponent("cgl")->SetCaptionEx("%d%%",0,0);
			m_pForm->GetComponent("money")->SetCaptionEx("%d",0,0);
			for(int i = 0; i < SLAVE_STRENGTHEN_MAX; i ++)
			{
				m_pCompUp[i]->SetCaption("",0);
			}
		}
	}
}

bool PetStrengthenMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 20)
	{
		pMainMenu->PopTipMsg("等级达到20级开启强化功能");
		return false;
	}
	return true;
}

void ClickPExchangeButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param == NULL)
		return;
	PetExchangeMenu* pMenu = (PetExchangeMenu*)param;
	pChar->m_pSlaveCenter->RequestExchange(pMenu->byCur);
}

PetExchangeMenu::PetExchangeMenu(char* pName):SlaveMenuObj(pName,0)
{
	if(m_pForm)
	{
		
		m_pForm->GetComponent("jinhua")->SetEvent(ClickPExchangeButton,this);
	}

}

PetExchangeMenu::~PetExchangeMenu()
{

}

void PetExchangeMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		SlaveCData* pSlave = NULL;
		int szMoney[4] = {1000000,2000000,4000000,0};
		if(byCur > 0 && byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
			pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[byCur-1];
		if(pSlave)
		{
			SlaveBasic* pNext = SlaveBaseCfg::GetInstance().GetBaseInfo(pSlave->m_pBasic->nExchangeID);
			bool ifButtonShow = false;
			if(pNext)
			{
				char szList[64];
				char szFile[64];
				sprintf(szList,"skin/%d/1.plist",pNext->nSkin);
				ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pNext->nSkin);
				if(byDirection == 7)
				{
					sprintf(szFile,"skin/%d/stand/3",pNext->nSkin);
					m_pForm->GetComponent("model2")->SetCartoonAction(szList,szFile,600,(pSkin?pSkin->nScale:100),true);
				}
				else
				{
					sprintf(szFile,"skin/%d/stand/%d",pNext->nSkin,byDirection);
					m_pForm->GetComponent("model2")->SetCartoonAction(szList,szFile,600,(pSkin?pSkin->nScale:100));
				}
				
				if(pSlave->m_byLevel >= pSlave->m_pBasic->byLvLimit)
					ifButtonShow = true;
			}
			else
			{
				char szList[64];
				char szFile[64];
				ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pSlave->m_pBasic->nSkin);
				sprintf(szList,"skin/%d/1.plist",pSlave->m_pBasic->nSkin);
				if(byDirection == 7)
				{
					sprintf(szFile,"skin/%d/stand/3",pSlave->m_pBasic->nSkin);
					m_pForm->GetComponent("model2")->SetCartoonAction(szList,szFile,600,(pSkin?pSkin->nScale:100),true);
				}
				else
				{
					sprintf(szFile,"skin/%d/stand/%d",pSlave->m_pBasic->nSkin,byDirection);
					m_pForm->GetComponent("model2")->SetCartoonAction(szList,szFile,600,(pSkin?pSkin->nScale:100));
				}
			}
		//	m_pForm->GetComponent("jinhua")->SetEnable(ifButtonShow);
			m_pForm->GetComponent("money")->SetCaptionEx("%d",0,szMoney[pSlave->m_pBasic->byExchangLv]);
			int szRlt[8];
			pChar->m_pSlaveCenter->CheckExcEnough(pSlave,szRlt);
			for(int i = 0;i < 4; i ++)
			{
				char szName[32];
				sprintf(szName,"cs%d",i+1);
				DComponent* pComp = m_pForm->GetComponent(szName);
				if(i < pSlave->m_pBasic->mapExchangeCost.size() )
				{
					std::map<int,int>::iterator ite = pSlave->m_pBasic->mapExchangeCost.find(i);
					int nID = ite->second;
					g_pSlaveMenuCtrl->ShowIcon(pComp,nID,0);
					if(szRlt[i] > 0)
						pComp->SetSprColor(255,255,255);
					else
						pComp->SetSprColor(100,100,100);
				}
				else
				{
					pComp->SetViewSpr("");
				}
			}
		}
		else
		{
			m_pForm->GetComponent("model2")->SetCartoonAction("","",0);
		}
	}
}

bool PetExchangeMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 30)
	{
		pMainMenu->PopTipMsg("等级达到30级开启进化功能");
		return false;
	}
	return true;
}

void ChoosePet2VariatFunc(xnList* pList,void* param)
{
	if(pList == NULL || param == NULL)
		return;
	PetVariatMenu * pMenu = (PetVariatMenu*)param;
	pMenu->m_nCnt = pList->Count;
	for(int i = 0; i < pList->Count && i < 4; i ++)
	{
		SlaveCData* pSlave = (SlaveCData*)pList->Items[i];
		pMenu->szID[i] = pSlave->m_dwID;
	}
}

void Click2ChooseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		PetVariatMenu* pMenu = (PetVariatMenu*)param;
		int nID = 0;
		if(pMenu->byCur > 0 && pMenu->byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
		{
			SlaveCData*pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[pMenu->byCur-1];
			nID = pSlave->m_dwID;
		}
		pMainMenu->m_pPetChooseMenu->Open(nID,ChoosePet2VariatFunc,(void*)pMenu,4);
	}
}

void ClickPetVariatOkButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		PetVariatMenu* pMenu = (PetVariatMenu*)param;
		pChar->m_pSlaveCenter->RequestVariat(pMenu->byCur,pMenu->szID,pMenu->m_nCnt);
		memset(pMenu->szID,0,16);
		pMenu->m_nCnt = 0;
	}
}

PetVariatMenu::PetVariatMenu(char* pName):SlaveMenuObj(pName,0)
{
	if(m_pForm)
	{

		memset(szID,0,4*4);
		m_nCnt = 0;

		m_pForm->GetComponent("jinhua")->SetEvent(ClickPetVariatOkButton,this);
		char szname[32];
		for(int i = 0; i < 4; i ++)
		{
			sprintf(szname,"cs%d",i+1);
			m_pForm->GetComponent(szname)->SetEvent(Click2ChooseButton,this);
		}
	}

}

PetVariatMenu::~PetVariatMenu()
{

}

void PetVariatMenu::Open(int nT /* = 1 */)
{
	memset(szID,0,4*4);
	m_nCnt = 0;
	m_pForm->Open(nT);
}

void PetVariatMenu::CFmEvent()
{
	memset(szID,0,4*4);
	m_nCnt = 0;
}

void PetVariatMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		SlaveCData* pSlave = NULL;
		static int nPPCount = 0;
		if(byCur > 0 && byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
			pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[byCur-1];
		if(pSlave)
		{
			m_pForm->GetComponent("bylv")->SetCaptionEx("%d",1,pSlave->m_byVariatlv);
			int nSuc = 0;
			if(pSlave->m_pVariat)
				m_pForm->GetComponent("pre")->SetCaptionEx("%.2f%%",1,pSlave->m_pVariat->nVal/100.0);
			else
				m_pForm->GetComponent("pre")->SetCaption("0%",1);
			SlaveVariatBasic* pNext = SlaveVariatCfg::GetInstance().GetBase(pSlave->m_byVariatlv+1);
			for(int i = 0; i < 4; i++)
			{
				char szName[32];
				sprintf(szName,"cs%d",i+1);
				DComponent* pCom = m_pForm->GetComponent(szName);
				SlaveCData* pFood = NULL;
				if(i < m_nCnt)
					pFood = pChar->m_pSlaveCenter->GetSlave(szID[i]);
				if(pFood)
				{
					g_pSlaveMenuCtrl->ShowIcon(pCom,pFood->m_nType,0);
					if(pNext)
						nSuc += pNext->nSucPer[pFood->m_pBasic->byQuality];
				}
				else
				{
					if(nPPCount%50 == 0)
					{
						pCom->SetViewSpr("BT_CW_JIA.png");
						pCom->byRunAction = 21;
					}
				}
			}
			if(pNext)
				m_pForm->GetComponent("money")->SetCaptionEx("%d",1,pNext->nMoney);		
			else
				m_pForm->GetComponent("money")->SetCaption("0",1);
			m_pForm->GetComponent("cgl")->SetCaptionEx("%.2f%%",1,nSuc/100.0);
		//	if(pSlave->m_pBasic->byExchangLv > 0)
				m_pForm->GetComponent("jinhua")->SetEnable(true);
		//	else
		//		m_pForm->GetComponent("jinhua")->SetEnable(false);
		}
		else
		{
			m_pForm->GetComponent("bylv")->SetCaption("",1);
			m_pForm->GetComponent("pre")->SetCaption("",1);
			m_pForm->GetComponent("money")->SetCaption("",1);
			m_pForm->GetComponent("cgl")->SetCaption("",1);
			m_pForm->GetComponent("jinhua")->SetEnable(false);
			for(int i = 0; i < 4; i++)
			{
				char szName[32];
				sprintf(szName,"cs%d",i+1);
				DComponent* pCom = m_pForm->GetComponent(szName);
				pCom->SetViewSpr("");
			}
		}
		nPPCount ++;
	}
}

bool PetVariatMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 40)
	{
		pMainMenu->PopTipMsg("等级达到40级开启变异功能");
		return false;
	}
	return true;
}

void CFmPetChooseEvent(int nPos,char* szname,void* param)
{
	if(param)
	{
		PetChooseMenu* pMenu = (PetChooseMenu*)param;
		if(nPos >= 0 && nPos < pMenu->m_pShowList->Count)
		{
			pMenu->AddRlt( pMenu->m_pShowList->Items[nPos]);
			pMenu->ReFresh();
		}
	}
}

void ClickPetChooseOKButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		PetChooseMenu* pMenu = (PetChooseMenu*)param;
		if(pMenu->Func_Return && pMenu->m_pParam)
		{
			pMenu->Func_Return(pMenu->m_pRltList,pMenu->m_pParam);
			pMenu->Close();
		}
	}
}

PetChooseMenu::PetChooseMenu()
{
	m_pForm = pMainInterface->GetDForm("xuancw");
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
	m_pForm->GetComponent("lv")->m_nFontSize = 20;
	m_pForm->GetComponent("lv")->SetSprColor(250,255,255);
	ppComp[4] = m_pForm->GetComponent("g");
	m_pForm->pCFrame = new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,pCompBase->nH*3);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,5);
	m_pForm->pCFrame->nDisStep = pCompBase->nH;
	m_pForm->pCFrame->SetEventEx(CFmPetChooseEvent,this);

	m_pForm->GetComponent("ok")->SetEvent(ClickPetChooseOKButton,this);
}

PetChooseMenu::~PetChooseMenu()
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

void PetChooseMenu::AddRlt(void* pData)
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

void PetChooseMenu::Open(int nID,void (*func)(xnList* pList,void* param),void* param,int nLimit)
{
	while(m_pRltList->Count)
		m_pRltList->Delete(0);
	while(m_pShowList->Count)
		m_pShowList->Delete(0);
	nIgnoreID = nID;
	m_pParam = param;
	m_nChooseLimit = nLimit;
	Func_Return = func;
	for(int i = 0; i < pChar->m_pSlaveCenter->m_pSlaveList->Count; i ++)
	{
		SlaveCData* pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[i];
		if(pSlave->m_dwID == nIgnoreID)
			continue;
		if(pSlave->m_ifActive)
			continue;
		m_pShowList->Add((void*)pSlave);
	}
	m_pForm->Open();
	ReFresh();
}

void PetChooseMenu::Close()
{
	m_pForm->Close();
}

void PetChooseMenu::ReFresh()
{
	m_pForm->pCFrame->SetFPCnt(m_pShowList->Count);
	DComponent* pIcon = NULL;
	DComponent* pName = NULL;
	DComponent* pQuality = NULL;
	DComponent* pLV = NULL;
	DComponent* pCheck = NULL;
	for(int i = 0; i <m_pShowList->Count; i ++)
	{
		pIcon = m_pForm->pCFrame->GetFPComp(i,"icon");
		pName = m_pForm->pCFrame->GetFPComp(i,"name");
		pQuality = m_pForm->pCFrame->GetFPComp(i,"pz");
		pLV = m_pForm->pCFrame->GetFPComp(i,"lv");
		m_pForm->GetComponent("lv")->m_nFontSize = 18;
		m_pForm->GetComponent("lv")->SetSprColor(250,150,20);
		pCheck = m_pForm->pCFrame->GetFPComp(i,"g");
		pCheck->SetVisible(false);
		SlaveCData* pSlave = (SlaveCData*)m_pShowList->Items[i];
		char szIcon[32];
		sprintf(szIcon,"head/%d.png",pSlave->m_pSkinObj->nHead);
		pIcon->SetViewSpr(szIcon);
		pName->SetCaption(pSlave->m_pName,1);
		pName->SetSprColor(nItemColor[pSlave->m_pBasic->byQuality][0],nItemColor[pSlave->m_pBasic->byQuality][1],nItemColor[pSlave->m_pBasic->byQuality][2]);
		pQuality->SetCaptionEx("%s",1,s_qualityName[pSlave->m_pBasic->byQuality]);
		pLV->SetCaptionEx("%d",1,pSlave->m_byLevel);
		for(int k = 0; k < m_pRltList->Count; k ++)
		{
			if(pSlave == (SlaveCData*)m_pRltList->Items[k])
			{
				pCheck->SetVisible(true);
				break;
			}
				
		}
	}
}

void msgFunNormal(void* param,int nVal)
{
	if(nVal == 1)
	{
		int nGetType = (int)param;
		pChar->m_pSlaveCenter->RequestGetPet(nGetType);
	}
}

void NormalGetEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	int nGetType = (int)param;
	if(!pChar->m_pSlaveCenter->CheckCount())
		pMainMenu->PopAskMsgBox("您的宠物数量已达上限，是否继续？",msgFunNormal,param);
	else
		pChar->m_pSlaveCenter->RequestGetPet(nGetType);

}

void msgFunQuick(void* param,int nVal)
{
	if(nVal == 1)
	{
		int nGetType = (int)param;
		pChar->m_pSlaveCenter->RequestQuickGetPet(nGetType);
	}
}

void QuickGetEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	int nGetType = (int)param;
	if(pChar->m_pSlaveCenter->m_pSlaveList->Count + 10 > MIN_SLAVE_CNT + pChar->m_pSlaveCenter->m_byUnlockCnt)
		pMainMenu->PopAskMsgBox("您的宠物数量已达上限，是否继续？",msgFunQuick,param);
	else
		pChar->m_pSlaveCenter->RequestQuickGetPet(nGetType);
}

void CloseGetMenuEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	PetGetMenu* pMenu = (PetGetMenu*)param;
	if (pMenu)
	{
		pMenu->m_pGetForm->Close();
		
	}
}

PetGetMenu::PetGetMenu()
{
	m_pGetForm = pMainInterface->GetDForm("cwchoujiang");

	m_pGetForm->GetComponent("p1")->SetEvent(NormalGetEvent,(void*)1);
	m_pGetForm->GetComponent("p2")->SetEvent(NormalGetEvent,(void*)2);
	m_pGetForm->GetComponent("p3")->SetEvent(NormalGetEvent,(void*)3);
	m_pGetForm->GetComponent("t1")->SetEvent(QuickGetEvent,(void*)1);
	m_pGetForm->GetComponent("t2")->SetEvent(QuickGetEvent,(void*)2);
	m_pGetForm->GetComponent("t3")->SetEvent(QuickGetEvent,(void*)3);

	m_pGetForm->GetComponent("close")->SetEvent(CloseGetMenuEvent,this);
}

PetGetMenu::~PetGetMenu()
{
	if (m_pGetForm)
	{
		delete m_pGetForm;
	}
}

void PetGetMenu::Update()
{
	if (m_pGetForm)
	{
		int x = pChar->m_pClientFlag->GetFlag(12);
		if (!pChar->m_pClientFlag->GetFlag(12))
		{
			m_pGetForm->GetComponent("z0")->SetVisible(true);
			m_pGetForm->GetComponent("z1")->SetVisible(false);
			m_pGetForm->GetComponent("num")->SetCaption("1/1",1);
		}else
		{
			m_pGetForm->GetComponent("z0")->SetVisible(false);
			m_pGetForm->GetComponent("z1")->SetVisible(true);
			m_pGetForm->GetComponent("num")->SetCaptionEx("%d/10",1, 10 - pChar->m_nGetSlaveLmt > 0?10 - pChar->m_nGetSlaveLmt:0);
		}
		m_pGetForm->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);
		m_pGetForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
		m_pGetForm->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);
	}

}

void ClickToClose(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	ShowPetGetMenu* pMenu = (ShowPetGetMenu*)param;
	pMenu->ReFreshMenu();
	pMenu->m_pShowGetForm->Close();
}

ShowPetGetMenu::ShowPetGetMenu()
{
	m_pGetSlaveList = xnList::Create();

	m_pShowGetForm = pMainInterface->GetDForm ("cwhuode");

	char sztxt[32];
	for (int i=0;i<10;i++)
	{
		sprintf(sztxt,"sk%d",i+1);
		pComSlaveItem[i] = m_pShowGetForm->GetComponent(sztxt);
	}
	for (int i=0;i<10;i++)
	{
		sprintf(sztxt,"s%d",i+1);
		pComSlaveName[i] = m_pShowGetForm->GetComponent(sztxt);
	}
	for (int i=0;i<10;i++)
	{
		sprintf(sztxt,"gezi%d",i+1);
		pComSlaveBack[i] = m_pShowGetForm->GetComponent(sztxt);
	}

	m_pShowGetForm->GetComponent("ok")->SetEvent(ClickToClose,this);
}

ShowPetGetMenu::~ShowPetGetMenu()
{
	if (m_pShowGetForm)
	{
		delete m_pShowGetForm;
	}
	if (m_pGetSlaveList)
	{
		delete m_pGetSlaveList;
	}
}

void ShowPetGetMenu::Open()
{
	if (!m_pGetSlaveList)
	{
		return;
	}

	if (m_pGetSlaveList->Count&&m_pGetSlaveList->Count == 1)
	{
		SlaveBasic* GetslaveInfo = (SlaveBasic*)m_pGetSlaveList->Items[0];

		m_pShowGetForm->GetComponent("gezi11")->SetVisible(true);
		g_pSlaveMenuCtrl->ShowIcon(m_pShowGetForm->GetComponent("sk11"),GetslaveInfo->nID,0);
		char	szTmp[32];
		sprintf(szTmp,"MB_PZ%d.png",GetslaveInfo->byQuality+1);
		m_pShowGetForm->GetComponent("sk11")->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
		m_pShowGetForm->GetComponent("s11")->SetCaption(GetslaveInfo->pName, 1);
		m_pShowGetForm->GetComponent("s11")->SetSprColor(nItemColor[GetslaveInfo->byQuality][0],nItemColor[GetslaveInfo->byQuality][1],nItemColor[GetslaveInfo->byQuality][2]);

	}else if(m_pGetSlaveList->Count)
	{
		for (int i = 0; i< m_pGetSlaveList->Count; i++)
		{
			pComSlaveBack[i]->SetVisible(true);

			SlaveBasic* GetslaveInfo = (SlaveBasic*)m_pGetSlaveList->Items[i];
			if (GetslaveInfo)
			{
				g_pSlaveMenuCtrl->ShowIcon(pComSlaveItem[i],GetslaveInfo->nID,0);
				char	szTmp[32];
				sprintf(szTmp,"MB_PZ%d.png",GetslaveInfo->byQuality+1);
				pComSlaveItem[i]->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
				pComSlaveName[i]->SetCaption(GetslaveInfo->pName,1);
				pComSlaveName[i]->SetSprColor(nItemColor[GetslaveInfo->byQuality][0],nItemColor[GetslaveInfo->byQuality][1],nItemColor[GetslaveInfo->byQuality][2]);

			}else{
				pComSlaveItem[i]->SetViewSpr("");
				pComSlaveName[i]->SetCaption("", 0);
			}
		}
	}
	m_pShowGetForm->Open();

	while(m_pGetSlaveList->Count)
		m_pGetSlaveList->Delete(0);
}


void ShowPetGetMenu::ReFreshMenu()
{
	for (int i = 0; i < 10; i++)
	{
		pComSlaveItem[i]->SetViewSpr("");
		pComSlaveName[i]->SetCaption("", 0);
		pComSlaveBack[i]->SetVisible(false);
	}
	m_pShowGetForm->GetComponent("gezi11")->SetVisible(false);
	m_pShowGetForm->GetComponent("sk11")->SetViewSpr("");
	m_pShowGetForm->GetComponent("s11")->SetCaption("", 0);
}

void ChgPNameReturnEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		char* szName = (char*)param;
		pChar->m_pSlaveCenter->RequestChangeName(pMainMenu->m_pChangePNameMenu->m_nCur,szName);
	}
	pMainMenu->m_pChangePNameMenu->Close();
}

void ClickToChgPName(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	ChangePNameMenu* pMenu = (ChangePNameMenu*)param;
	if (pMenu)
	{
		pMenu->m_szName = pMenu->m_pForm->GetComponent("name")->GetEdit();
		pMainMenu->PopAskMsgBox("是否更改宠物名字?",ChgPNameReturnEvent,(void*)pMenu->m_szName);
	}
}

ChangePNameMenu::ChangePNameMenu():m_nCur(0),m_szName(NULL)
{
	m_pForm = pMainInterface->GetDForm("cwgaiming");

	m_pForm->GetComponent("ok")->SetEvent(ClickToChgPName,this);
}

ChangePNameMenu::~ChangePNameMenu()
{

}

void ChangePNameMenu::Open(int nCur)
{
	m_nCur = nCur;
	m_pForm->Open();
}

void ChangePNameMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		char* pName = m_pForm->GetComponent("name")->GetEdit();
		if(pName && strlen(pName) > 10)
		{
			*(pName+10) = '\0';
			m_pForm->GetComponent("name")->SetEdit(pName);
		}
	}
}

void ChangePNameMenu::Close()
{
	m_pForm->GetComponent("name")->SetEdit("");
	m_pForm->Close();
}