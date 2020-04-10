#include "MainMenu.h"
#include "UserData.h"
#include "ShopMenu.h"
#include "ios_item_packet.h"
#include "MallConfig.h"
#include "SoundCenter.h"
static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pShopMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{
			if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_SHOP);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_SHOP);
			}
		}
	}
}


extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetItemToDCom(DComponent* pDom,int nID);
static void CBFrameFBStepEvent(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pShopMenu->m_nSelectPage)
	{
		pMainMenu->m_pShopMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pShopMenu->UpdatePage(nPos-1,false);
		}
		//pMainMenu->m_pBagMenu->UpdatePage(nPos,true);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pShopMenu->UpdatePage(nPos+1,false);
		}
	}
	pMainMenu->m_pShopMenu->UpdatePage(nPos,true);
}
static void SwitchPage(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pShopMenu->m_pShopEx->Switch((bool)param);
}
static void OpenFastSell(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pFastSellMenu->Open();
}
void fun_cbFixEquip(void * pParam,int nVal)
{
	if(nVal == 1){
		if (pChar->m_dwMoney<(int)pParam)
		{
			pMainMenu->PopTipMsg("金币不足");
			return;
		}
		g_pSoundCenter->PlayerEffect("Sound/hammer");
		pChar->SendCmd(ITEM,C_ITEM_FIXALL);	
	}
}
int GetFixNeedGold()
{
	int nDuraTotal = 0;
	for (int i=0;i<EQUIP_MAX;i++)
	{
		ItemObj* pObj = pChar->m_pItemCenter->GetEquipBySlot(i);
		if (pObj)
		{
			int nNum = pObj->pItemBase->nDura - pObj->GetVal(ITEM_AD_NUM);
			nDuraTotal += nNum;
		}
	}
	return nDuraTotal*75;
}
static void OpenFixMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	char sztxt[128];
	int nDuraTotal = GetFixNeedGold();
	if (!nDuraTotal)
	{
		pMainMenu->PopTipMsg("你现在的装备无需修理");
		return;
	}
	sprintf(sztxt,"是否花费%d金币修理身上的装备？",nDuraTotal);
	pMainMenu->PopAskMsgBox(sztxt,fun_cbFixEquip,(void*)nDuraTotal);
}
ShopMenu::ShopMenu():m_bShopUpdate(true),m_nSelectPage(0)
{
	m_pForm = pMainInterface->GetDForm ("shop");
	char sztxt[32];
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(445,145,485,340);
	DComponent*	pComp	= m_pForm->GetComponent ("di3");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,20);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pForm->pCFrame->nSpace	= 55;
	m_pForm->pCFrame->nDisStep	= 510;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);


	m_pShopEx = new ShopMenuEx();
	m_pForm->GetComponent("page1")->SetEvent(SwitchPage,(void*)1);
	m_pForm->GetComponent("page2")->SetEvent(SwitchPage,(void*)0);

	m_pForm->GetComponent("kscs")->SetEvent(OpenFastSell);
	m_pForm->GetComponent("xiuli")->SetEvent(OpenFixMenu);
}

ShopMenu::~ShopMenu()
{
	delete m_pShopEx;
}

void ShopMenu::Open(int nIndex)
{
	m_pForm->Open();
	m_pShopEx->Open(nIndex);
}

void ShopMenu::Close()
{
	m_pShopEx->Close();
	m_pForm->Close();
}

void ShopMenu::Update()
{
	if (!m_pForm->bOpened&&m_pShopEx->m_pForm->bOpened)
	{
		m_pShopEx->Close();
	}
	if (!m_pForm->bOpened||!m_pShopEx->m_pForm->bOpened)
	{
		return;
	}
	m_pForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
	m_pForm->GetComponent("jinbiC2")->SetCaptionEx("%d",1,pChar->m_dwMoney);
	m_pForm->GetComponent("moneyC3")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);
	if (m_bShopUpdate)
	{
		m_bShopUpdate = false;
		
		char szSpr[32];
		if (m_nSelectPage == 0)
		{
			UpdatePage(1,false);
		}
		UpdatePage(m_nSelectPage,true);

		xnList* pList = xnList::Create();
		pChar->m_pItemCenter->GetItemInBagByPos(pList);
		for (int i= 0;i<pList->Count;i++)
		{
			ItemObj* pItem = (ItemObj*)pList->Items[i];
			int nPos = pItem->GetVal(ITEM_AD_POS);
			if (nPos<m_nSelectPage*20+20&&nPos>=m_nSelectPage*20)
			{
				sprintf(szSpr,"bag%d",nPos%20+1);
				SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),pItem);
			}	
		}
		pList->Free();

		m_pForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
		m_pForm->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);
		
	}
	m_pShopEx->Update();
}

void ShopMenu::UpdatePage( int nSelectPage,bool bSelect )
{
	char sztxt[32];
	char szSpr[32];
	for (int i=nSelectPage*20;i<nSelectPage*20+20;i++)
	{
		sprintf(szSpr,"bag%d",i%20+1);
		if (i>=pChar->m_pItemCenter->m_nBagSize)
		{
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
			m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr)->SetViewSpr("MB_SUO.png");
		}else{
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
		}
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pList);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = pItem->GetVal(ITEM_AD_POS);
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
	pList->Free();
	if (bSelect)
	{
		for (int i=0;i<nBagPageMax;i++)
		{
			sprintf(sztxt,"p%d",i+1);
			m_pForm->GetComponent(sztxt)->SetViewSpr( i== nSelectPage?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
		}
	}
}

static void CBFrameFBEventShopEx(int nPos,char* szName)
{
	if(strncmp (szName, "ck",2) == 0)
	{
		if (pMainMenu->m_pShopMenu->m_pShopEx->m_bShop)
		{
			ShopItemCfg* pItem = (ShopItemCfg*)pMainMenu->m_pShopMenu->m_pShopEx->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
			if (pItem)
			{
				ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pItem->m_nItemID);
				if (!pBase)
				{
					return;
				}
				if (pBase->byType != ITEMTYPE_EQUIP)
				{
					pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_SHOP,(void*)pItem->m_nTypeInfo);
					pMainMenu->m_pItemInfoMenu->m_pForm->GetComponent("money")->SetCaptionEx("%d",0,pItem->m_nPrice);
					pMainMenu->m_pItemInfoMenu->m_pForm->GetComponent("m")->SetVisible(true);
					pMainMenu->m_pItemInfoMenu->m_pForm->GetComponent("money")->SetSprColor(255,255,255);
				}else{
					pMainMenu->m_pEquipInfoMenu->Open(pBase,FORM_SHOP,(void*)pItem->m_nTypeInfo);
					pMainMenu->m_pEquipInfoMenu->m_pForm->GetComponent("moneyC15")->SetCaptionEx("%d",0,pItem->m_nPrice);
					pMainMenu->m_pEquipInfoMenu->m_pForm->GetComponent("m")->SetVisible(true);
					pMainMenu->m_pEquipInfoMenu->m_pForm->GetComponent("moneyC15")->SetSprColor(255,255,255);
				}
				
			}
		}else{
			ItemObj* pItem = (ItemObj*)pMainMenu->m_pShopMenu->m_pShopEx->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
			if(pItem&&pItem->pItemBase->byType == ITEMTYPE_EQUIP){
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_SHOP);
			}
			else if (pItem)
			{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_SHOP);
			}
		}
		
	}
}
static void CBFrameFBStepEventShopEx(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pShopMenu->m_pShopEx->m_nSelectPage)
	{
		pMainMenu->m_pShopMenu->m_pShopEx->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pShopMenu->m_pShopEx->UpdatePage(nPos-1,false);
		}
		pMainMenu->m_pShopMenu->m_pShopEx->UpdatePage(nPos,true);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pShopMenu->m_pShopEx->UpdatePage(nPos+1,false);
		}
	}
}

ShopMenuEx::ShopMenuEx():m_bShopExUpdate(true),m_nSelectPage(0),m_nShopType(-1),m_bShop(true)
{
	m_pForm = pMainInterface->GetDForm ("shop2");
	char sztxt[32];

	DComponent* pComItem[16];
	for (int i=0;i<16;i++)
	{
		sprintf(sztxt,"ck%d",i+1);
		// 		m_pBagItem[i] = m_pForm->GetComponent(sztxt);
		// 		m_pBagItem[i]->SetEvent(UseItemEvent);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(40,139,362,338);
	DComponent*	pComp	= m_pForm->GetComponent ("di4");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,16);
	m_pForm->pCFrame->SetEvent (CBFrameFBEventShopEx);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEventShopEx);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 367;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);

	
}

ShopMenuEx::~ShopMenuEx()
{

}

void ShopMenuEx::Open(int nIndex)
{
	m_pForm->Open();
	m_nShopType = nIndex;
	m_bShopExUpdate = true;
}

void ShopMenuEx::Close()
{
	m_pForm->Close();
}

void ShopMenuEx::Update()
{
	if (m_bShopExUpdate)
	{
		m_bShopExUpdate = false;
		m_pForm->pCFrame->SetFPCnt(m_bShop?nBagPageMax:1,true);

		if (m_nSelectPage == 0)
		{
			UpdatePage(1,false);
		}
		UpdatePage(m_nSelectPage,true);

	}
}

void ShopMenuEx::UpdatePage( int nSelectPage,bool bSelect )
{
	if (m_nShopType == -1)
	{
		return;
	}
	ShopInfoCfg* pCfg = ShopConfig::GetInstance().GetCfg(m_nShopType);
	if (!pCfg)
	{
		return;
	}
	char sztxt[32];
	char szSpr[32];
	nSelectPage = m_bShop?nSelectPage:0;
	for (int i=nSelectPage*16;i<nSelectPage*16+16;i++)
	{
		sprintf(szSpr,"ck%d",i%16+1);
		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
	}

	if (m_bShop)
	{
		for (int i= 0;i<pCfg->pItemList->Count;i++)
		{
			ShopItemCfg* pItem = (ShopItemCfg*)pCfg->pItemList->Items[i];
			if (i<nSelectPage*16+16&&i>=nSelectPage*16)
			{
				sprintf(szSpr,"ck%d",i%16+1);
				SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem->m_nItemID);
				m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr)->SetTag(pItem);
			}	
		}
	}else{
		xnList* pList = xnList::Create();
		pChar->m_pItemCenter->GetItemInBagBySlot(pList,ITEM_SLOT_SELL);
		for (int i= 0;i<pList->Count;i++)
		{
			ItemObj* pItem = (ItemObj*)pList->Items[i];
			if (i<m_nSelectPage*16+16&&i>=m_nSelectPage*16)
			{
				sprintf(szSpr,"ck%d",i%16+1);
				SetItemToDCom(m_pForm->pCFrame->GetFPComp(m_nSelectPage,szSpr),pItem);
			}	
		}
		pList->Free();

	}
	
	if (bSelect)
	{
		for (int i=0;i<nBagPageMax;i++)
		{
			sprintf(sztxt,"cp%d",i+1);
			pMainMenu->m_pShopMenu->m_pForm->GetComponent(sztxt)->SetViewSpr( i== nSelectPage?(char*)"MB_DIAN_B.png":(char*)"MB_DIAN_S.png");
		}
	}
}

void ShopMenuEx::Switch( bool bShop )
{
	m_bShop = bShop;
	m_nSelectPage = 0;
	m_bShopExUpdate = true;
}
static void FastSell(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	FastSellMenu* pMenu = (FastSellMenu*)param;
	int nType = 0;
	for (int i=0;i<4;i++)
	{
		if (pMenu->m_pCheck[i]->GetCheck())
		{
			nType |= (1<<i);
		}
	}
	if (pChar->m_pItemCenter->FastSell(nType))
	{
		pMenu->Close();
	}
	
}
FastSellMenu::FastSellMenu()
{
	m_pForm = pMainInterface->GetDForm ("KSCS");
	char sztxt[32];
	for (int i=0;i<4;i++)
	{
		sprintf(sztxt,"g%d",i+1);
		m_pCheck[i] = new CCtrlCheck(m_pForm->GetComponent(sztxt),0);
	}
	m_pForm->GetComponent("ok")->SetEvent(FastSell,this);
}

FastSellMenu::~FastSellMenu()
{

}

void FastSellMenu::Open()
{
	m_pForm->Open();
}

void FastSellMenu::Close()
{
	m_pForm->Close();
}

void FastSellMenu::Update()
{
	if (m_bUpdate)
	{
		m_bUpdate = false;
	}
}
