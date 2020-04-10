#include "MainMenu.h"
#include "UserData.h"
#include "BagMenu.h"
#include "ios_item_packet.h"

#include "GuideCenter.h"
extern void	SetBagItem(DComponent**pCom,int nCount,xnList* pList,void (*EventFunc)(InterfaceEvent * pEvent,void * param),int nStartIndex = 0);
extern char* szAttribute[EQUIP_ATTR_MAX];

static void AddBagSizeByRmb(void* param,int nVal)
{
	if(nVal == 1)
	{
		int nCost = 0;
		if (pChar->m_pItemCenter->m_nBagSize<59)
		{
			nCost = pChar->m_pItemCenter->m_nBagSize-39;
		}else{
			nCost = 20;
		}
		if (pChar->m_dwRmb < nCost)
		{
			pMainMenu->PopTipMsg("钻石不足");
			return;
		}
		pChar->SendCmd(ITEM,C_ADD_BAGSIZE_BYRMB);
	}
}
static void OpenNewBagEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	//解锁包裹
	//pMainMenu->PopTipMsg("扩展包裹栏需要消耗包裹扩充券。");

	char sztxt[64];
	if (pChar->m_pItemCenter->m_nBagSize>=100)
	{
		pMainMenu->PopTipMsg("包裹已经扩充到最大");
		return;
	}
	int nCost = 0;

	if (pChar->m_pItemCenter->m_nBagSize<59)
	{
		nCost = pChar->m_pItemCenter->m_nBagSize-39;
	}else{
		nCost = 20;
	}
	sprintf(sztxt,"是否花费%d钻石来开启一个包裹格子?",nCost);
	pMainMenu->PopAskMsgBox(sztxt,AddBagSizeByRmb,NULL);
}
void	callBacktest(ItemObj* pObj){
	int aa=123;
}

static void UseItemEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = (ItemObj*)pEvent->pComponent->GetTag();
	if (pItem)
	{
		if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			if (pItem->pItemBase->bySlot == 9)
			{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BAG);
			}else
				pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BAG);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BAG);
		}
		//pChar->m_pItemCenter->UseItem(pItem);
	}
}

static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "bag",3) == 0)
	{
		ItemObj* pItem = (ItemObj*)pMainMenu->m_pBagMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->GetTag();
		
		if (pItem)
		{
// 			pChar->m_pItemCenter->GetItemInfoFormServer(pItem->GetVal(ITEM_AD_UIN),pItem->GetVal(ITEM_AD_ID),callBacktest);
// 			return;
			if (pItem->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				if (pItem->pItemBase->bySlot == 9)
				{
					pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BAG);
				}else
					pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_BAG);
			}else{
				pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_BAG);
			}
		}else{
			if(!strncmp(pMainMenu->m_pBagMenu->m_pForm->pCFrame->GetFPComp(nPos,szName)->szSpr,"MB_SUO.png",strlen("MB_SUO.png"))){
				char sztxt[64];
				if (pChar->m_pItemCenter->m_nBagSize>=100)
				{
					pMainMenu->PopTipMsg("包裹已经扩充到最大");
					return;
				}
				int nCost = 0;

				if (pChar->m_pItemCenter->m_nBagSize<59)
				{
					nCost = pChar->m_pItemCenter->m_nBagSize-39;
				}else{
					nCost = 20;
				}
				sprintf(sztxt,"是否花费%d钻石来开启一个包裹格子?",nCost);
				pMainMenu->PopAskMsgBox(sztxt,AddBagSizeByRmb,NULL);
			}

			
		}
		
	}
}
static void CBFrameFBStepEvent(int nPos)
{
	if (nPos<0)
	{
		return;
	}
	if (nPos!=pMainMenu->m_pBagMenu->m_nSelectPage)
	{
		pMainMenu->m_pBagMenu->m_nSelectPage = nPos;
		if (nPos>=1)
		{
			pMainMenu->m_pBagMenu->UpdatePage(nPos-1,false);
		}
		//pMainMenu->m_pBagMenu->UpdatePage(nPos,true);
		if (nPos<nBagPageMax-1)
		{
			pMainMenu->m_pBagMenu->UpdatePage(nPos+1,false);
		}
	}
	pMainMenu->m_pBagMenu->UpdatePage(nPos,true);
}
char* szEquipComp[] = {"zb4","zb2","zb3","zb1","zb10","zb7","zb6","zb9","zb8","zb5",};
void	SetCharToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5){
	if (!pDom)
	{
		return;
	}
	char szList[64];
	char szFile[64];
	sprintf(szList,"skin/%d/1.plist",999+byJob*2+(bySex?0:1));
	sprintf(szFile,"skin/%d/%s/%d",999+byJob*2+(bySex?0:1),szAction,nDirection == 7?3:nDirection);
	ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(999+byJob*2+(bySex?0:1));

	pDom->SetCartoonAction(szList,szFile,600,pSkin?pSkin->nScale:100,nDirection == 7);
}
void	SetCharToDCom(DComponent* pDom,int	nSkin,char* szAction = "stand",int nDirection = 5){
	if (!pDom)
	{
		return;
	}
	char szList[64];
	char szFile[64];
	sprintf(szList,"skin/%d/1.plist",nSkin);
	sprintf(szFile,"skin/%d/%s/%d",nSkin,szAction,nDirection == 7?3:nDirection);
	ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(nSkin);
	pDom->SetCartoonAction(szList,szFile,600,pSkin?pSkin->nScale:100,nDirection == 7);
}
void	SetWeaponToDCom(DComponent* pDom,int nID,char* szAction = "stand",int nDirection = 5){
	if (!pDom)
	{
		return;
	}
	char szList[64];
	char szFile[64];
	sprintf(szList,"skin/%d/1.plist",nID);
	sprintf(szFile,"skin/%d/%s/%d",nID,szAction,nDirection == 7?3:nDirection);
	ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(nID);
	pDom->SetCartoonAction(szList,szFile,600,pSkin?pSkin->nScale:100,nDirection == 7);
}
void	SetWeaponToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5){
	if (!pDom)
	{
		return;
	}
	char szList[64];
	char szFile[64];
	sprintf(szList,"skin/%d/1.plist",2999+byJob*2+(bySex?0:1));
	sprintf(szFile,"skin/%d/%s/%d",2999+byJob*2+(bySex?0:1),szAction,nDirection == 7?3:nDirection);
	ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(2999+byJob*2+(bySex?0:1));
	pDom->SetCartoonAction(szList,szFile,600,pSkin?pSkin->nScale:100,nDirection == 7);
}

void SetStarLvEff(DComponent* pDom,ItemObj* pObj)
{
	if (pObj)
	{
		pDom->SetEffect(0,2);
        if(pObj->GetStarLv()>18)
		{
			pDom->SetEffect(238,2);
		}else if(pObj->GetStarLv()>17)
		{
			pDom->SetEffect(237,2);
		}else if(pObj->GetStarLv()>14)
		{
			pDom->SetEffect(236,2);
		}else if (pObj->GetStarLv()>9)
		{
			pDom->SetEffect(223,2);
		}
	}
}

void	SetItemToDCom(DComponent* pDom,ItemObj* pObj){
	if (pObj)
	{
		char sztxt[32];
		sprintf(sztxt,"item/%d.png",pObj->pItemBase->nSpr);
		pDom->SetViewSpr(sztxt);
		sprintf(sztxt,"MB_PZ%d.png",pObj->pItemBase->nStar+1);
		pDom->SetAddSprByTag(sztxt,-3,-3,1,100,0,-1);
		SetStarLvEff(pDom,pObj);
		pDom->SetEffect(0,1);
		if (pObj->pItemBase->nStar == 2)
		{
			pDom->SetEffect(174,1);
		}else if (pObj->pItemBase->nStar == 3)
		{
			pDom->SetEffect(175,1);
		}
		pDom->SetTag(pObj);
		if (pObj->pItemBase->nNum == 1||pObj->GetVal(ITEM_AD_NUM) == 1)
		{
			pDom->SetCaption("",2);
		}else{
			pDom->m_nFontSize = 18;
			pDom->SetCaptionEx("%d",2,pObj->GetVal(ITEM_AD_NUM));
		}		
	}else{
		pDom->ClearEffect();
		pDom->SetViewSpr("");
		pDom->SetTag(NULL);
		pDom->SetCaption("",2);
	}
}
void	SetItemNameToDCom(DComponent* pDom,ItemObj* pObj){
	if (!pDom->pSTextCenter)
	{
		return;
	}
	pDom->pSTextCenter->ClearSText();
	if (pObj)
	{
		char sztxt[32];
		pDom->pSTextCenter->AddSText(0,0,false,pObj->pItemBase->szName,nItemColor[pObj->pItemBase->nStar][0],nItemColor[pObj->pItemBase->nStar][1],nItemColor[pObj->pItemBase->nStar][2]);
		if (pObj->GetStarLvEx())
		{
			sprintf(sztxt,"ui/STAR20.png");
			STextObj* pCheck = pDom->pSTextCenter->AddSText(1,1,false,sztxt,255,215,0);
		}
		else if (pObj->GetStarLv())
		{
			sprintf(sztxt," %d★",pObj->GetStarLv());
			pDom->pSTextCenter->AddSText(1,0,false,sztxt,255,215,0);
		}
	}
}
void	SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj,int nAlignment){
	if (pDom->pSTextCenter)
	{
		pDom->pSTextCenter->ClearSText();
		if (pObj)
		{
			char sztxt[32];
			pDom->pSTextCenter->AddSText(0,0,false,pObj->szName,nItemColor[pObj->nStar][0],nItemColor[pObj->nStar][1],nItemColor[pObj->nStar][2]);
		}
		return;
	}else{
		if (pObj)
		{
			pDom->SetCaptionEx(pObj->szName,nAlignment);
			pDom->SetSprColor(nItemColor[pObj->nStar][0],nItemColor[pObj->nStar][1],nItemColor[pObj->nStar][2]);
		}else{
			pDom->SetCaptionEx("",nAlignment);
		}

	}
}
void	SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj){
	SetItemNameToDCom(pDom,pObj,0);
// 	if (pDom->pSTextCenter)
// 	{
// 		pDom->pSTextCenter->ClearSText();
// 		if (pObj)
// 		{
// 			char sztxt[32];
// 			pDom->pSTextCenter->AddSText(0,0,false,pObj->szName,nItemColor[pObj->nStar][0],nItemColor[pObj->nStar][1],nItemColor[pObj->nStar][2]);
// 		}
// 		return;
// 	}else{
// 		if (pObj)
// 		{
// 			pDom->SetCaptionEx(pObj->szName,0);
// 			pDom->SetSprColor(nItemColor[pObj->nStar][0],nItemColor[pObj->nStar][1],nItemColor[pObj->nStar][2]);
// 		}else{
// 			pDom->SetCaptionEx("",0);
// 		}
// 		
// 	}
	
}
void	SetItemToDCom(DComponent* pDom,int nID){
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nID);
	if (pBase)
	{
		char sztxt[32];
		sprintf(sztxt,"item/%d.png",pBase->nSpr);
		pDom->SetViewSpr(sztxt);
		pDom->SetCaption("",0);
		pDom->SetTag(pBase);
		sprintf(sztxt,"MB_PZ%d.png",pBase->nStar+1);
		pDom->SetAddSprByTag(sztxt,-3,-3,1,100,0,-1);
		pDom->ClearEffect();
		if (pBase->nStar == 2)
		{
			pDom->SetEffect(174,1);
		}else if (pBase->nStar == 3)
		{
			pDom->SetEffect(175,1);
		}
	}else{
		pDom->ClearEffect();
		pDom->SetCaption("",0);
		pDom->SetViewSpr("");
		pDom->SetTag(NULL);
	}
}
void	SetBagItemToDComByID(DComponent* pDom,int nID,int nNeedCount = 0){
	int nCount = pChar->m_pItemCenter->GetItemCountByID(nID);
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nID);
	char sztxt[32] = {0};
	if (pBase)
	{
		sprintf(sztxt,"item/%d.png",pBase->nSpr);
	}
	pDom->SetViewSpr(sztxt);
	pDom->SetCaptionEx("%d",2,nCount);
	pDom->SetEnable((nCount>=nNeedCount&&nNeedCount!=-1)?true:false);
}


void	SetEquipStoneToDCom(DComponent** pDomIcon,DComponent** pDomType,DComponent** pDomVal,ItemObj* pObj,DComponent** pDomBack,float fscale = 1.0f){
	if (pObj)
	{
		int nPos = pObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
		for (int i=0;i<nMaxStoneCount;i++)
		{
			bool bIsBore = (nPos&1<<i);
			if(pDomBack&&pDomBack[i]) pDomBack[i]->SetVisible(bIsBore);
			ItemExBase* pStoneBase = pObj->GetStoneByPos(i);
		
			char sztxt[32] = {0};
			if (pStoneBase)
			{
				sprintf(sztxt,"item/%d.png",pStoneBase->nSpr);
			}
			

			if(pDomBack){
				pDomIcon[i]->SetViewSprByScale(bIsBore?sztxt:(char*)"",fscale,fscale);				
			}else{
				pDomIcon[i]->SetViewSprByScale(bIsBore?sztxt:(char*)"MB_SUO.png",fscale,fscale);					
			}
			for (int j=0;j<EQUIP_ATTR_MAX;j++)
			{
				if (pStoneBase&&pStoneBase->nParam[j])
				{
					pDomType[i]->SetCaptionEx("%s",1,szAttribute[j]);
					pDomVal[i]->SetCaptionEx("+%d",1,pStoneBase->nParam[j]);
					pDomVal[i]->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
					break;
				}else if (!pStoneBase){
					if(pDomBack){
						pDomType[i]->SetCaptionEx("%s",1,bIsBore?"未镶嵌":"");
					}else{
						pDomType[i]->SetCaptionEx("%s",1,bIsBore?"未镶嵌":"未开孔");
					}
					pDomVal[i]->SetCaptionEx("",0);
					break;
				}
			}
		}
	}else{
		for (int i=0;i<nMaxStoneCount;i++)
		{
			if(pDomBack&&pDomBack[i]) pDomBack[i]->SetVisible(false);
			pDomIcon[i]->SetViewSpr("");
			pDomType[i]->SetCaptionEx("",0);
			pDomVal[i]->SetCaptionEx("",0);
		}
	}
	
}
static void ItemTest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pChar->m_pItemCenter->SortBag();
}
BagMenu::BagMenu():m_bUpdate(true),m_nSelectPage(0)
{
	m_pForm = pMainInterface->GetDForm ("bag");
	char sztxt[32];
	DComponent* pComItem[20];
	for (int i=0;i<20;i++)
	{
		sprintf(sztxt,"bag%d",i+1);
// 		m_pBagItem[i] = m_pForm->GetComponent(sztxt);
// 		m_pBagItem[i]->SetEvent(UseItemEvent);
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
	for (int i=0;i<10;i++)
	{
		m_pEquip[i] = m_pForm->GetComponent(szEquipComp[i]);
		m_pEquip[i]->SetEvent(UseItemEvent);
	}
	
	m_pForm->GetComponent("zhengli")->SetEvent(ItemTest);
}

BagMenu::~BagMenu()
{

}

void BagMenu::Open(int nT)
{
	m_bUpdate = true;
	m_pForm->Open(nT);
}

void BagMenu::Close()
{
	m_pForm->Close();
}

void BagMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	m_pForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
	m_pForm->GetComponent("jinbi")->SetCaptionEx("%d",1,pChar->m_dwMoney);
	m_pForm->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwBindRmb);
	if (m_bUpdate)
	{
		m_bUpdate = false;

		bool bSwitch = true;
		if (pChar->m_byJob==2)
		{
			bSwitch = false;
		}
		if (pChar->m_xUserParam.nSkin > 1500)
		{
			m_pForm->GetComponent((char*)(bSwitch?"model":"model2"))->SetVisible(false);
		}else{
			m_pForm->GetComponent((char*)(bSwitch?"model":"model2"))->SetVisible(true);
			SetWeaponToDCom(m_pForm->GetComponent((char*)(bSwitch?"model":"model2")),pChar->m_pItemCenter->GetWeaponSkin());
		}
		SetCharToDCom(m_pForm->GetComponent(bSwitch?(char*)"model2":(char*)"model"),pChar->m_xUserParam.nSkin);

		m_pForm->GetComponent("zd")->SetCaptionEx("%d",1,pChar->GetScore());
// 		char szList[64];
// 		char szFile[64];
// 		sprintf(szList,"skin/%d/1.plist",1000+pChar->m_byJob*2+(pChar->m_bySex?0:1));
// 		sprintf(szFile,"skin/%d/stand/5",1000+pChar->m_byJob*2+(pChar->m_bySex?0:1));
// 		m_pForm->GetComponent("model")->SetCartoonAction(szList,szFile,600,100);

		char szSpr[32];
// 		if (m_nSelectPage == 0)
// 		{
// 			UpdatePage(1,false);
// 		}
// 		UpdatePage(m_nSelectPage,true);

		int i = 0;
		for(;i < 100; i ++)
		{
			sprintf(szSpr,"bag%d",i%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(i/20,szSpr),NULL);
			if (i>=pChar->m_pItemCenter->m_nBagSize)
			{
				m_pForm->pCFrame->GetFPComp(i/20,szSpr)->SetViewSpr("MB_SUO.png");
				//m_pForm->pCFrame->GetFPComp(i/20,szSpr)->SetEvent(OpenNewBagEvent);
			}else{
				m_pForm->pCFrame->GetFPComp(i/20,szSpr)->SetProgress(0,100,NULL);
				//m_pForm->pCFrame->GetFPComp(i/20,szSpr)->SetEvent(NULL);
			}
		}

		xnList* pList = xnList::Create();
		pChar->m_pItemCenter->GetItemInBagByPos(pList);
		for (i = 0;i<pList->Count;i++)
		{
			ItemObj* pItem = (ItemObj*)pList->Items[i];
			int nPos = pItem->GetVal(ITEM_AD_POS);		

		//	pGuideCenter->SetBagMenuGuide(pItem,nPos);	//===zjk
			
			//if (nPos<m_nSelectPage*20+20&&nPos>=m_nSelectPage*20)
			{
				sprintf(szSpr,"bag%d",nPos%20+1);
				SetItemToDCom(m_pForm->pCFrame->GetFPComp(nPos/20,szSpr),pItem);
			}	
		}


		pList->Free();
		i=0;
		for (;i<10;i++)
		{
			ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(i);
			SetItemToDCom(m_pEquip[i],pItem);
			
		}
	}
	char szSpr[32];
	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pList);
	for (int i= 0;i<pList->Count;i++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[i];
		int nPos = pItem->GetVal(ITEM_AD_POS);
		if (pItem&&pItem->pItemBase->wCDPos&&pItem->pItemBase->dwCDTime)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			if (pChar->m_pClientFlag->mapCD[pItem->pItemBase->wCDPos])
			{
				int nTime = pChar->m_pClientFlag->mapCD[pItem->pItemBase->wCDPos] - xnGetTickCount();
				if (nTime<0)
				{
					nTime = 0;
				}
				m_pForm->pCFrame->GetFPComp(nPos/20,szSpr)->SetProgress(nTime,pItem->pItemBase->dwCDTime);
				m_pForm->pCFrame->GetFPComp(nPos/20,szSpr)->SetCaptionEx("%d",2,pItem->GetVal(ITEM_AD_NUM));
			}else{
			//	m_pForm->pCFrame->GetFPComp(nPos/20,szSpr)->SetProgress(0,100,NULL);
			}
		}

	}
	pList->Free();
// 	for (int i=0;i<nBagPageMax;i++)
// 	{
// 		for(int j= 0;j<20;j++){
// 			
// 			ItemObj* pItem = (ItemObj*)m_pForm->pCFrame->GetFPComp(i,szSpr)->GetTag();
// 			if (pItem&&pItem->pItemBase->wCDPos&&pItem->pItemBase->dwCDTime)
// 			{
// 				
// 			}else{
// 			}
// 		}
// 	}
}


void BagMenu::UpdatePage( int nSelectPage,bool bSelect )
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
			//m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr)->SetEvent(OpenNewBagEvent);
		}else{
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
		}
	}

	xnList* pList = xnList::Create();
	pChar->m_pItemCenter->GetItemInBagByPos(pList);
	int k = 0;
	for (;k<pList->Count;k++)
	{
		ItemObj* pItem = (ItemObj*)pList->Items[k];
		int nPos = pItem->GetVal(ITEM_AD_POS);
		if (nPos<nSelectPage*20+20&&nPos>=nSelectPage*20)
		{
			sprintf(szSpr,"bag%d",nPos%20+1);
			SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),pItem);
		}	
	}
// 	for(;k< 20; k ++)
// 	{
// 		sprintf(szSpr,"bag%d",k+1);
// 		SetItemToDCom(m_pForm->pCFrame->GetFPComp(nSelectPage,szSpr),NULL);
// 	}
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
