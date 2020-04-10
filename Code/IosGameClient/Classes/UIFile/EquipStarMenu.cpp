#include "MainMenu.h"
#include "UserData.h"
#include "EquipStarMenu.h"
#include "ios_item_packet.h"
extern char* szAttribute[EQUIP_ATTR_MAX];

static void EquipStarLvUp(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	EquipStarMenu* pMenu = (EquipStarMenu*)param;
// 	if (!pMenu->m_pForm->GetComponent("icon2")->bEnable)
// 	{
// 		return;
// 	}
	pChar->m_pItemCenter->EquipStarUp(EquipStarMenu::sLvUpEquipObj);
}
ItemObj* EquipStarMenu::sLvUpEquipObj = NULL;

extern void	SetItemNameToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetBagItemToDComByID(DComponent* pDom,int nID,int nNeedCount = 0);
extern void	SetEquipStoneToDCom(DComponent** pDomIcon,DComponent** pDomType,DComponent** pDomVal,ItemObj* pObj,DComponent** pDomBack,float fscale = 1.0f);

static void UseLianXingItemEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = (ItemObj*)pEvent->pComponent->GetTag();
	if (pItem)
	{
		pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_NULL);
	}
}

static void UseStoneEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = pChar->m_pItemCenter->GetItemByTypeID(20000);	
	if (pItem)
	{
		pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_NULL);
	}
}

static void ChooseMoreEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pBagMenu->Open(2);
}

static void UseStoneEvent2(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = pChar->m_pItemCenter->GetItemByTypeID(20001);	
	if (pItem)
	{
		pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_NULL);
	}
}

static void UseStoneEvent3(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = pChar->m_pItemCenter->GetItemByTypeID(20005);	
	if (pItem)
	{
		pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_NULL);
	}
}

static void ClickOpenStarDeclareMenu(InterfaceEvent* pEvent,void* parma)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pEquipStarMenu->m_pDetailForm->Open();
}

EquipStarMenu::EquipStarMenu():m_bUpdate(true)
{
	m_pForm = pMainInterface->GetDForm ("lianxing");
	char sztxt[32];
	m_pForm->GetComponent("sx3")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
	m_pForm->GetComponent("sx4")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
// 	for (int i=0;i<10;i++)
// 	{
// 		m_pEquip[i] = m_pForm->GetComponent(szEquipComp[i]);
// 		m_pEquip[i]->SetEvent(UseItemEvent);
// 	}
// 	
 	m_pForm->GetComponent("zhengli")->SetEvent(EquipStarLvUp,this);
	
	m_pForm->GetComponent("icon1")->SetEvent(UseLianXingItemEvent, this);
	m_pForm->GetComponent("icon2")->SetEvent(UseStoneEvent, this);
	m_pForm->GetComponent("huan")->SetEvent(ChooseMoreEvent);

	m_pDetailForm = pMainInterface->GetDForm("lianxing2");
	m_pForm->GetComponent("sm")->SetEvent(ClickOpenStarDeclareMenu);
}

EquipStarMenu::~EquipStarMenu()
{

}


void EquipStarMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		
		SetItemToDCom(m_pForm->GetComponent("icon1"),sLvUpEquipObj);
		int nItemID = EquipStarConfig::GetInstance().nItemID;
		SetBagItemToDComByID(m_pForm->GetComponent("icon2"),nItemID,0);		
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nItemID);
		if (pBase)
		{		
			m_pForm->GetComponent("clname")->SetCaption(pBase->szName,0);
			m_pForm->GetComponent("num")->SetCaptionEx("%d个",0,EquipStarConfig::GetInstance().nItemCount);
		}
		if(sLvUpEquipObj){

			SetItemNameToDCom(m_pForm->GetComponent("name"),EquipStarMenu::sLvUpEquipObj);
			int nCount = 0;
			char sztxt1[32];
			int nBase[EQUIP_ATTR_MAX] = {0};
			sLvUpEquipObj->GetBaseData(nBase);
			int nLv = sLvUpEquipObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV);
			nLv = nLv<<24>>24;
			int nPer1 = sLvUpEquipObj->GetStarPer(nLv);
			int nPer2 = sLvUpEquipObj->GetStarPer(nLv+1);
			EquipStarBase* pCfg = EquipStarConfig::GetInstance().GetEquipStar(nLv+1);
			if (pCfg)
			{
				m_pForm->GetComponent("zhengli")->SetEnable(true);
				m_pForm->GetComponent("money")->SetCaptionEx("%d",0,pCfg->nGold);
				m_pForm->GetComponent("cgl")->SetCaptionEx("%d%%",0,pCfg->nSuccess);
			}else{
				m_pForm->GetComponent("zhengli")->SetEnable(false);
				m_pForm->GetComponent("money")->SetCaptionEx("",0);
				m_pForm->GetComponent("cgl")->SetCaptionEx("",0);

			}
			for (int i=0;i<EQUIP_ATTR_MAX;i++)
			{
				if(nBase[i]){
					m_pForm->GetComponent((char*)(nCount?"sxname2":"sxname1"))->SetCaption(szAttribute[i],0);
					m_pForm->GetComponent((char*)(nCount?"sxname4":"sxname3"))->SetCaption(szAttribute[i],0);

					m_pForm->GetComponent((char*)(nCount?"sx2":"sx1"))->SetCaptionEx("%d",0,nBase[i]*(nPer1+10000)/10000);
					m_pForm->GetComponent((char*)(nCount?"sx4":"sx3"))->SetCaptionEx("%d",0,nBase[i]*(nPer2+10000)/10000);
					nCount++;
				}
			}
			if (!pCfg)
			{
				m_pForm->GetComponent("sx4")->SetCaption("炼星已满",0);
				m_pForm->GetComponent("sx3")->SetCaption("炼星已满",0);

			}

		}else{
			m_pForm->GetComponent("money")->SetCaption("",0);
			m_pForm->GetComponent("name")->SetCaption("",0);
			m_pForm->GetComponent("sxname2")->SetCaption("",0);
			m_pForm->GetComponent("sxname1")->SetCaption("",0);
			m_pForm->GetComponent("sxname4")->SetCaption("",0);
			m_pForm->GetComponent("sxname3")->SetCaption("",0);
			m_pForm->GetComponent("sx2")->SetCaption("",0);
			m_pForm->GetComponent("sx1")->SetCaption("",0);
			m_pForm->GetComponent("sx4")->SetCaption("",0);
			m_pForm->GetComponent("sx3")->SetCaption("",0);
			m_pForm->GetComponent("cgl")->SetCaptionEx("",0);
		}
	}
}

void EquipStarMenu::SetEquipItem( ItemObj* pObj )
{
	sLvUpEquipObj = pObj;
	m_bUpdate = true;
}
static void EquipBoreUp(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	EquipStarMenu* pMenu = (EquipStarMenu*)param;
// 	if (!pMenu->m_pForm->GetComponent("icon5")->bEnable)
// 	{
// 		return;
// 	}
	pChar->m_pItemCenter->EquipBore(EquipStarMenu::sLvUpEquipObj);
}

static void EquipBoreInfo(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	if (!EquipStarMenu::sLvUpEquipObj)
	{
		return;
	}
	ItemExBase* pBase = EquipStarMenu::sLvUpEquipObj->GetStoneByPos(nPos);
	if (!pBase)
	{
		bool bIsBore = (EquipStarMenu::sLvUpEquipObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS)&1<<(nPos));	

		if (bIsBore)
		{			
			pMainMenu->PopTipMsg("选择需要镶嵌的宝石，点击镶嵌进行操作。");
		}else{		
			pMainMenu->PopTipMsg("请点击打孔按钮给装备打孔");
		}

		return;
	}
	pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_NULL);
}
EquipBoreMenu::EquipBoreMenu():m_bUpdate(true)
{
	m_pForm = pMainInterface->GetDForm ("dakong");

	m_pForm->GetComponent("dk")->SetEvent(EquipBoreUp,this);

	m_pForm->GetComponent("icon")->SetEvent(UseLianXingItemEvent, this);
	m_pForm->GetComponent("icon5")->SetEvent(UseStoneEvent2, this);

	char sztxt[32];
	for (int i=0;i<nMaxStoneCount;i++)
	{
		sprintf(sztxt,"icon%d",i+1);
		m_pForm->GetComponent(sztxt)->SetEvent(EquipBoreInfo,(void*)i);
	}

	m_pForm->GetComponent("huan")->SetEvent(ChooseMoreEvent);
}

EquipBoreMenu::~EquipBoreMenu()
{

}

void EquipBoreMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;

		

 		SetItemToDCom(m_pForm->GetComponent("icon"),EquipStarMenu::sLvUpEquipObj);
 		int nItemID = nStoneCostItem;
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nItemID);
		if (pBase)
		{
			m_pForm->GetComponent("clname")->SetCaption(pBase->szName,0);
		}
		DComponent* comIcon[nMaxStoneCount];
		DComponent* comType[nMaxStoneCount];
		DComponent* comVal[nMaxStoneCount];
		char sztxt[32];
		for (int i=0;i<nMaxStoneCount;i++)
		{
			sprintf(sztxt,"icon%d",i+1);
			comIcon[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"bs%d",i+1);
			comType[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"sz%d",i+1);
			comVal[i] = m_pForm->GetComponent(sztxt);
		}
		SetEquipStoneToDCom(comIcon,comType,comVal,EquipStarMenu::sLvUpEquipObj,NULL);
		SetItemNameToDCom(m_pForm->GetComponent("name"),EquipStarMenu::sLvUpEquipObj);
 		if(EquipStarMenu::sLvUpEquipObj){
 
 
 			int nPos = EquipStarMenu::sLvUpEquipObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
 			int nNowPos = 0;
 			for (;nNowPos<(sizeof(int)*8);nNowPos++)
 			{
 				if (!(nPos&(1<<nNowPos))) break;
 			}

 			
			m_pForm->GetComponent("num")->SetCaptionEx("%d个",0,nNowPos>=nMaxStoneCount?0:nItemBoreCostCount[nNowPos]);
 			m_pForm->GetComponent("money")->SetCaptionEx("%d",0,nNowPos>=nMaxStoneCount?0:nItemBoreCostMoney[nNowPos]);
			SetBagItemToDComByID(m_pForm->GetComponent("icon5"),nItemID,nNowPos>=nMaxStoneCount?-1:nItemBoreCostCount[nNowPos]);

 			




 		}else{
 			m_pForm->GetComponent("money")->SetCaption("",0);
 			m_pForm->GetComponent("name")->SetCaption("",0);


 			m_pForm->GetComponent("cgl")->SetCaptionEx("",0);
 		}				
	}
}

void EquipBoreMenu::SetEquipItem( ItemObj* pObj )
{
	EquipStarMenu::sLvUpEquipObj = pObj;
	m_bUpdate = true;
}

bool EquipBoreMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 25)
	{
		pMainMenu->PopTipMsg("等级达到25级开启打孔功能");
		return false;
	}
	return true;
}

static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pEquipInlayMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{
			pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_EQUIP_INTENSIFY);
			//pChar->m_pItemCenter->UseItem(pItem);
		}
	}
}
static void CBFrameFBStepEvent(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pEquipInlayMenu->m_nSelectPage)
	{
		pMainMenu->m_pEquipInlayMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pEquipInlayMenu->UpdatePage(nPos-1);
		}
		pMainMenu->m_pEquipInlayMenu->UpdatePage(nPos);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pEquipInlayMenu->UpdatePage(nPos+1);
		}
	}
}

static void EquipStoneInfo(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	if (!EquipStarMenu::sLvUpEquipObj)
	{
		return;
	}
	ItemExBase* pBase = EquipStarMenu::sLvUpEquipObj->GetStoneByPos(nPos);
	if (!pBase)
	{
		bool bIsBore = (EquipStarMenu::sLvUpEquipObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS)&1<<(nPos));

		if (bIsBore)  
		{			
			pMainMenu->PopTipMsg("选择需要镶嵌的宝石，点击镶嵌进行操作。");
		}else{		
			pMainMenu->PopTipMsg("请点击打孔按钮给装备打孔");
		}

		return;
	}
	pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_EQUIP_INTENSIFY);
}

static void ClickOpenInlayDeclareMenu(InterfaceEvent* pEvent,void* parma)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pEquipInlayMenu->m_pDetailForm->Open();
}

EquipInlayMenu::EquipInlayMenu():m_bUpdate(true),m_nSelectPage(0)
{
	m_pForm = pMainInterface->GetDForm ("xiangqian");

	char sztxt[32];
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(477,198,460,340);
	DComponent*	pComp	= m_pForm->GetComponent ("di2");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,20);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 460;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);

	for (int i=0;i<nMaxStoneCount;i++)
	{
		sprintf(sztxt,"icon%d",i+1);
		m_pForm->GetComponent(sztxt)->SetEvent(EquipStoneInfo,(void*)i);
	}

	m_pForm->GetComponent("icon")->SetEvent(UseLianXingItemEvent, this);
	m_pForm->GetComponent("huan")->SetEvent(ChooseMoreEvent);

	m_pDetailForm = pMainInterface->GetDForm("baoshi2");
	m_pForm->GetComponent("sm")->SetEvent(ClickOpenInlayDeclareMenu);
}

EquipInlayMenu::~EquipInlayMenu()
{

}

void EquipInlayMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;

		SetItemToDCom(m_pForm->GetComponent("icon"),EquipStarMenu::sLvUpEquipObj);
		
		DComponent* comIcon[nMaxStoneCount];
		DComponent* comType[nMaxStoneCount];
		DComponent* comVal[nMaxStoneCount];
		char sztxt[32];
		for (int i=0;i<nMaxStoneCount;i++)
		{
			sprintf(sztxt,"icon%d",i+1);
			comIcon[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"bs%d",i+1);
			comType[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"sz%d",i+1);
			comVal[i] = m_pForm->GetComponent(sztxt);
		}
		SetEquipStoneToDCom(comIcon,comType,comVal,EquipStarMenu::sLvUpEquipObj,NULL);
		SetItemNameToDCom(m_pForm->GetComponent("name"),EquipStarMenu::sLvUpEquipObj);

		if (m_nSelectPage == 0)
		{
			UpdatePage(1);
		}
		UpdatePage(m_nSelectPage);
	}
}

void EquipInlayMenu::SetEquipItem( ItemObj* pObj )
{
	EquipStarMenu::sLvUpEquipObj = pObj;
	m_bUpdate = true;
}

void EquipInlayMenu::UpdatePage( int nSelectPage )
{
	char szSpr[32];
	for (int i=nSelectPage*20;i<nSelectPage*20+20;i++)
	{
		sprintf(szSpr,"bag%d",i%20+1);
		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByType(pList,ITEMTYPE_STONE);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = i;
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
	pList->Free();
}

bool EquipInlayMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 25)
	{
		pMainMenu->PopTipMsg("等级达到25级开启镶嵌功能");
		return false;
	}
	return true;
}

static void CBFrameFBEvent2(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pEquipEatMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_EQUIP_INTENSIFY);
			//pChar->m_pItemCenter->UseItem(pItem);
		}
	}
}
static void CBFrameFBStepEvent2(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pEquipEatMenu->m_nSelectPage)
	{
		pMainMenu->m_pEquipEatMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pEquipEatMenu->UpdatePage(nPos-1);
		}
		pMainMenu->m_pEquipEatMenu->UpdatePage(nPos);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pEquipEatMenu->UpdatePage(nPos+1);
		}
	}
}

void ChickFastBtnEvent(InterfaceEvent* pEvent,void* parma)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	EquipEatMenu* pEstMenu = (EquipEatMenu*)parma;
	if (pEstMenu)
	{
		pEstMenu->m_pFastEatMenu->Open();
	}
}

EquipEatMenu::EquipEatMenu():m_nSelectPage(0),m_bUpdate(true),nPreExp(0)
{
	m_pForm = pMainInterface->GetDForm ("jinglian");

	char sztxt[32];
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(477,198,460,340);
	DComponent*	pComp	= m_pForm->GetComponent ("di2");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,20);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent2);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent2);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 460;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);

	m_pCProExp = new CCtrlProgress(m_pForm->GetComponent("jl"));

	m_pForm->GetComponent("icon")->SetEvent(UseLianXingItemEvent, this);	
	m_pForm->GetComponent("huan")->SetEvent(ChooseMoreEvent);

	m_pFastEatMenu = new FastEatMenu;
	m_pForm->GetComponent("yjjl")->SetEvent(ChickFastBtnEvent,this);

}

EquipEatMenu::~EquipEatMenu()
{
	delete m_pCProExp;
}

void EquipEatMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;

		SetItemToDCom(m_pForm->GetComponent("icon"),EquipStarMenu::sLvUpEquipObj);
		SetItemNameToDCom(m_pForm->GetComponent("name"),EquipStarMenu::sLvUpEquipObj);

		if(EquipStarMenu::sLvUpEquipObj){

			int nEatLv = EquipStarMenu::sLvUpEquipObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV);
			int nLv = nEatLv<<24>>24;
			int nVal = nEatLv>>8;
			EquipEatBase* pBase = EquipEatConfig::GetInstance().GetEquipEat(nLv);
			int nAddType = EquipEatConfig::GetInstance().GetAddParam(EquipStarMenu::sLvUpEquipObj->pItemBase->bySlot);

			m_pForm->GetComponent("jinglian")->SetCaptionEx("精炼等级:%d",0,nLv);

			m_pForm->GetComponent("sxname1")->SetCaption(nAddType?"防御":"攻击",0);
			m_pForm->GetComponent("sxname2")->SetCaption(nAddType?"防御":"攻击",0);
			int nAddVal = 0;
			if (pBase)
			{
				nAddVal = nAddType?pBase->nDefense:pBase->nDamage;
			}
			m_pForm->GetComponent("sx1")->SetCaptionEx("+%d",0,nAddVal);
			EquipEatBase* pBaseNext = EquipEatConfig::GetInstance().GetEquipEat(nLv+1);
			nAddVal = 0;
			if (pBaseNext)
			{
				nAddVal = nAddType?pBaseNext->nDefense:pBaseNext->nDamage;
			}

			EquipEatBase* pBaseNext2 = EquipEatConfig::GetInstance().GetEquipEat(nLv+2);

			m_pCProExp->SetProgress(nVal,pBaseNext?pBaseNext->exp:100,300);

			if (nAddVal)
			{
				m_pForm->GetComponent("sx2")->SetCaptionEx("+%d",0,nAddVal);
			}else{
				m_pForm->GetComponent("sx2")->SetCaptionEx("已精炼到最高等级",0);
			}

		}else{
			m_pForm->GetComponent("name")->SetCaption("",0);
			m_pForm->GetComponent("jinglian")->SetCaption("",0);
			m_pForm->GetComponent("sxname1")->SetCaption("",0);
			m_pForm->GetComponent("sxname2")->SetCaption("",0);
			m_pForm->GetComponent("sx1")->SetCaption("",0);
			m_pForm->GetComponent("sx2")->SetCaption("",0);
		}
		if (m_nSelectPage == 0)
		{
			UpdatePage(1);
		}
		UpdatePage(m_nSelectPage);
	}
	m_pCProExp->Update();
}
static bool GetItemEat(ItemObj* pObj,void* param){
	if (pObj->GetVal(ITEM_AD_SLOT) !=ITEM_SLOT_BAG )
	{
		return false;
	}
	if (pObj == EquipStarMenu::sLvUpEquipObj)
	{
		return false;
	}
	if (pObj->pItemBase->nStar >= 1&&pObj->pItemBase->byType == ITEMTYPE_EQUIP/*&&pObj->pItemBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN)*/)
	{
		return true;
	}
	return false;
}
void EquipEatMenu::UpdatePage( int nSelectPage )
{
	char szSpr[32];
	for (int i=nSelectPage*20;i<nSelectPage*20+20;i++)
	{
		sprintf(szSpr,"bag%d",i%20+1);
		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByFilter(pList,GetItemEat);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = i;
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
	pList->Free();
}

void EquipEatMenu::Open( int nT )
{
	m_bUpdate = true;
	int nEatLv = EquipStarMenu::sLvUpEquipObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV);
	int nLv = nEatLv<<24>>24;
	int nVal = nEatLv>>8;
	EquipEatBase* pBaseNext = EquipEatConfig::GetInstance().GetEquipEat(nLv+1);
	m_pForm->GetComponent("jl")->SetProgress(nVal,pBaseNext?pBaseNext->exp:100);
	m_pCProExp->SetProgress(nVal,pBaseNext?pBaseNext->exp:100,0);
	nPreExp = nVal;

	m_pForm->Open (nT);
}

bool EquipEatMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 29)
	{
		pMainMenu->PopTipMsg("等级达到29级开启精炼功能");
		return false;
	}
	return true;
}


static void CBFrameFBEvent3(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pEquipTransferMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		if (pItem)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_EQUIP_INTENSIFYEX);
			pMainMenu->m_pEquipTransferMenu->m_pEquip = pItem;
			//pMainMenu->m_pEquipTransferMenu->m_pEquip = pItem;
			//pMainMenu->m_pEquipTransferMenu->m_bUpdate = true;
			//pChar->m_pItemCenter->UseItem(pItem);
		}
	}
}
static void CBFrameFBStepEvent3(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pEquipTransferMenu->m_nSelectPage)
	{
		pMainMenu->m_pEquipTransferMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pEquipTransferMenu->UpdatePage(nPos-1);
		}
		pMainMenu->m_pEquipTransferMenu->UpdatePage(nPos);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pEquipTransferMenu->UpdatePage(nPos+1);
		}
	}
}
EquipTransferMenu::EquipTransferMenu():m_nSelectPage(0),m_bUpdate(true),m_pEquip(NULL)
{
	m_pForm = pMainInterface->GetDForm ("zhuanyi");

	char sztxt[32];
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
		pComItem[i] = m_pForm->GetComponent(sztxt);
		pComItem[i]->SetVisible(false);
	}
	m_pForm->pCFrame	= new CCtrlFrame(477,198,460,340);
	DComponent*	pComp	= m_pForm->GetComponent ("di2");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,20);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent3);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEvent3);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= 460;

	m_pForm->pCFrame->SetFPCnt(nBagPageMax,true);

	m_pForm->GetComponent("icon")->SetEvent(UseLianXingItemEvent, this);
	m_pForm->GetComponent("icon2")->SetEvent(UseStoneEvent3, this);
	m_pForm->GetComponent("huan")->SetEvent(ChooseMoreEvent);
}

EquipTransferMenu::~EquipTransferMenu()
{

}

void EquipTransferMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;

		SetItemToDCom(m_pForm->GetComponent("icon"),EquipStarMenu::sLvUpEquipObj);
		SetItemNameToDCom(m_pForm->GetComponent("name"),EquipStarMenu::sLvUpEquipObj);


		//SetBagItemToDComByID(m_pForm->GetComponent("icon2"),nTransferItemID,nTransferItemCount);

		SetBagItemToDComByID(m_pForm->GetComponent("icon2"),nTransferItemID,nTransferItemCount);
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nTransferItemID);
		if (pBase)
			m_pForm->GetComponent("clname")->SetCaptionEx("%s*%d",0,pBase?pBase->szName:"",EquipStarMenu::sLvUpEquipObj->GetTransferCost());
		m_pForm->GetComponent("money")->SetCaptionEx("%d",0,nTransferMoney);

		if (m_nSelectPage == 0)
		{
			UpdatePage(1);
		}
		UpdatePage(m_nSelectPage);
	}
}
static bool GetItemTransfer(ItemObj* pObj,void* param){
	if (!EquipStarMenu::sLvUpEquipObj)
	{
		return false;
	}
	if (pObj == EquipStarMenu::sLvUpEquipObj)
	{
		return false;
	}
	if (pObj->pItemBase->bySlot == EquipStarMenu::sLvUpEquipObj->pItemBase->bySlot&&pObj->pItemBase->byType == ITEMTYPE_EQUIP&&pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG)
	{
		return true;
	}
	return false;
}
void EquipTransferMenu::UpdatePage( int nSelectPage )
{
	char szSpr[32];
	for (int i=nSelectPage*20;i<nSelectPage*20+20;i++)
	{
		sprintf(szSpr,"bag%d",i%20+1);
		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByFilter(pList,GetItemTransfer);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = i;
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
	pList->Free();
}

bool EquipTransferMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 30)
	{
		pMainMenu->PopTipMsg("等级达到30级开启转移功能");
		return false;
	}
	return true;
}
static void FastEat(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	FastEatMenu* pMenu = (FastEatMenu*)param;
	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByFilter(pList,GetItemEat);
	for (int i=0;i<pList->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)pList->Items[i];
		if (pMenu->m_pCheck[pObj->pItemBase->nStar-1]->GetCheck())
		{
			pChar->m_pItemCenter->EquipEat(EquipStarMenu::sLvUpEquipObj,pObj);
		}
	}
	pMenu->Close();
	pList->Free();
}
FastEatMenu::FastEatMenu()
{
	m_pForm = pMainInterface->GetDForm ("ksjl");
	char sztxt[32];
	for (int i=0;i<3;i++)
	{
		sprintf(sztxt,"g%d",i+1);
		m_pCheck[i] = new CCtrlCheck(m_pForm->GetComponent(sztxt),0);
	}
	m_pForm->GetComponent("ok")->SetEvent(FastEat,this);
}

FastEatMenu::~FastEatMenu()
{

}

void FastEatMenu::Open()
{
	m_pForm->Open();
}

void FastEatMenu::Close()
{
	m_pForm->Close();
}
