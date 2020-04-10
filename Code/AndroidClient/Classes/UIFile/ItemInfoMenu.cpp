#include "MainMenu.h"
#include "UserData.h"
#include "ItemInfoMenu.h"
#include "ios_item_packet.h"
#include "MallConfig.h"
#include "MapAIObj.h"
#include "GameActivityCfg.h"

char* szItemType[ITEMTYPE_MAX] = {"不可使用物品","可使用物品","装备","礼包","宠物技能","宝石","快捷物品","技能书","坐骑"};
char* szEquipSlot[EQUIP_MAX] = {"武器","衣服","裤子","头盔","鞋子","肩部","项链","戒指","披风","时装"};
char* szAttribute[EQUIP_ATTR_MAX] = {"生命","魔法","物攻","魔攻","物防","魔防","命中","闪避","暴击"};
extern void	SetEquipStoneToDCom(DComponent** pDomIcon,DComponent** pDomType,DComponent** pDomVal,ItemObj* pObj,DComponent** pDomBack,float fscale = 1.0f);
extern void	SetItemNameToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetItemNameToDCom(DComponent* pDom,ItemExBase* pObj);
extern void	SetItemToDCom(DComponent* pDom,int nID);
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
static void	InputResult(int nResult,void * param){
	MallItemCfg* pCfg = (MallItemCfg*)param;
	if (!pCfg)
	{
		return;
	}
	pChar->m_pItemCenter->BuyMallItem(pCfg->m_byType,pCfg->m_nItemID,nResult);
};
static void	InputResultShop(int nResult,void * param){
	ShopItemCfg* pCfg = (ShopItemCfg*)param;
	pChar->m_pItemCenter->BuyShopItem(pCfg->m_nTypeInfo,pCfg->m_nItemID,nResult);
};
static void UseItemExEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	EquipInfoMenu* pMenu = (EquipInfoMenu*)param;
	switch(pMenu->m_nOpenStyle){
	case FORM_BAG:
		{
			if (pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_EQUIP&&pMenu->m_pItemObj->pItemBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN))
			{
				pMainMenu->m_pEquipStarMenu->SetEquipItem(pMenu->m_pItemObj);
				pMainMenu->m_pBagMenu->Close();
				pMainMenu->m_pTabEquipCtrl->Open(0,0);
			}else{
				pMainMenu->PopTipMsg("该装备不能强化");
			}
			//
		}
		break;
	case FORM_BANK:
		{
			pChar->m_pItemCenter->SetItemBank(pMenu->m_pItemObj);
			//pChar->m_pItemCenter->SetItemBank(pMenu->m_pItemObj);
		}
		break;
	case FORM_MALL:
		{
			MallItemCfg* pItem = (MallItemCfg*)pMainMenu->m_pEquipInfoMenu->m_pParam;//MallConfig::GetInstance().GetCfg(pMenu->m_pItemBase->nTypeID);
			if (pItem)
			{
				if(pItem->m_nLimit == -1)
					break;
				int nMaxCount = 1000;//pChar->m_dwRmb*100/pItem->m_nPrice/pItem->m_nDiscount;
				//nMaxCount = nMaxCount>1000?1000:nMaxCount;
				if (pItem->m_nFlag != -1)
				{
					int nFlag = pChar->m_pClientFlag->GetSLimitF(pItem->m_nFlag);
					if (nFlag>=pItem->m_nLimit)
					{
						break;
					}
					nMaxCount = nMaxCount<(pItem->m_nLimit-nFlag)?nMaxCount:(pItem->m_nLimit-nFlag);
				}

				pMainMenu->m_pGetNumMenu->Open(InputResult,nMaxCount,0,(void*)pItem);

			}
		}
		break;
	case FORM_EQUIP_INTENSIFY:
		{
			if(!EquipStarMenu::sLvUpEquipObj) break;
			pChar->m_pItemCenter->EquipEat(EquipStarMenu::sLvUpEquipObj,pMenu->m_pItemObj);
		}
		break;
	case FORM_EQUIP_INTENSIFYEX:
		{
			pChar->m_pItemCenter->EquipTransfer(pMenu->m_pItemObj,EquipStarMenu::sLvUpEquipObj);
// 			pMainMenu->m_pEquipTransferMenu->m_pEquip = pMenu->m_pItemObj;
// 			pMainMenu->m_pEquipTransferMenu->m_bUpdate = true;
		}
		break;
	case FORM_SHOP:
		{
			if (pMenu->m_pItemObj)
			{
				if (pMenu->m_pItemObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_SELL)
				{
					pChar->m_pItemCenter->SellItem(pMenu->m_pItemObj);
				}else{
					pChar->m_pItemCenter->SellItem(pMenu->m_pItemObj);
				}
			}else{
				ShopItemCfg* pItem = ShopConfig::GetInstance().GetCfg((int)pMenu->m_pParam,pMenu->m_pItemBase->nTypeID);
				if (pItem)
				{
					int nMaxCount = 1000;//pChar->m_dwMoney/pItem->m_nPrice;
					//nMaxCount = nMaxCount>1000?1000:nMaxCount;
					//if (!nMaxCount)
					//{
					//	pMainMenu->PopTipMsg("金币不足");
					//	return;
					//}
					pMainMenu->m_pGetNumMenu->Open(InputResultShop,nMaxCount,0,(void*)pItem);

				}
			}
		}
		break;
	case FORM_SHOW:
		{
			pMainMenu->m_pChatMenu->InsertItem(pMenu->m_pItemObj);
			pMenu->Close();
			pMainMenu->m_pChatMenu->m_pItemMenu->Close();
		}
		break;
	case FORM_EX:
		{
			if (pMenu->fCallBackEvent)
			{
				pMenu->fCallBackEvent(pMenu->m_pItemObj,0);
			}
			pMenu->Close();
		}
		break;
	}

	pMenu->Close();

}
static void UseItem(InterfaceEvent * pEvent,void * param)
 {
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	EquipInfoMenu* pMenu = (EquipInfoMenu*)param;
	switch(pMenu->m_nOpenStyle){
	case FORM_BAG:
		{
			pChar->m_pItemCenter->UseItem(pMenu->m_pItemObj);
		}
		break;
	case FORM_BANK:
		{

			pChar->m_pItemCenter->SetItemBank(pMenu->m_pItemObj);
		}
		break;
	case FORM_EX:
		{
			if (pMenu->fCallBackEvent)
			{
				pMenu->fCallBackEvent(pMenu->m_pItemObj,1);
			}
			pMenu->Close();
		}
		break;
	case FORM_MALL:
		{
			if (pMenu->m_pItemBase->byType != ITEMTYPE_EQUIP&&pMenu->m_pItemBase->byType != ITEMTYPE_RIDE)
			{
				return;
			}
			switch(pMenu->m_pItemBase->byType)
			{
			case ITEMTYPE_EQUIP:
				{
					if (pMenu->m_pItemBase->bySexLimit&&(pMenu->m_pItemBase->nSkin > 1500)&&(pMenu->m_pItemBase->bySexLimit!=pChar->m_bySex+1))
					{
						pMainMenu->PopTipMsg("性别不符，无法试穿");
						return;
					}
					pMainMenu->m_pMallMenu->m_nCloth = pMenu->m_pItemBase->nSkin;
					pMainMenu->m_pMallMenu->m_nHorse = 0;
					pMainMenu->m_pMallMenu->m_bMallUpdate = true;
				}
				break;
			case ITEMTYPE_RIDE:
				{
					HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(pMenu->m_pItemBase->nSkin);
					if (pBasic)
					{
						pMainMenu->m_pMallMenu->m_nHorse = pBasic->nSkin;
						pMainMenu->m_pMallMenu->m_bMallUpdate = true;
					}
				}
				break;
			}
		}
		break;
	
	}
	pMenu->Close();
}
static void AskDestroyItem(void* param,int nVal)
{
	if(nVal == 1)
	{
		EquipInfoMenu* pMenu = (EquipInfoMenu*)param;
		pChar->m_pItemCenter->DestroyItem(pMenu->m_pItemObj);
		pMenu->Close();
	}else{
		
	}
}
static void AskRemoveStone(void* param,int nVal)
{
	if(nVal == 1)
	{
		if (pChar->m_dwMoney<(int)param)
		{
			pMainMenu->PopTipMsg("金币不足");
			return;
		}
		int nPos = 0;
		for (;nPos<nMaxStoneCount;nPos++)
		{
			if(EquipStarMenu::sLvUpEquipObj->GetStoneByPos(nPos) == pMainMenu->m_pItemInfoMenu->m_pItemBase) break;
		}
		pChar->m_pItemCenter->EquipRemoveStone(EquipStarMenu::sLvUpEquipObj,nPos);
	}else{

	}
}
static void DestroyItem(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	EquipInfoMenu* pMenu = (EquipInfoMenu*)param;
	if (pMenu->m_pItemObj->pItemBase->nStar>=2)
	{
		pMainMenu->PopAskMsgBox("此物品较为珍贵,是否确认销毁?",AskDestroyItem,param);
	}else if (pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_EQUIP&&pMenu->m_pItemObj->CheckAddPer())
	{
		pMainMenu->PopAskMsgBox("此物品较为珍贵,是否确认销毁?",AskDestroyItem,param);
	}else{
		pChar->m_pItemCenter->DestroyItem(pMenu->m_pItemObj);
		pMenu->Close();
	}

	
	
}

EquipInfoMenu::EquipInfoMenu():m_bUpdate(true),m_nOpenStyle(0),fCallBackEvent(NULL)
{
	m_pForm = pMainInterface->GetDForm ("zb1");
	m_pForm->bZorde = FORM_ZORDER_CHAT_TIP;
	char sztxt[32];
	m_pForm->GetComponent("cz1")->SetEvent(UseItemExEvent,this);
	m_nPX1 = m_pForm->GetComponent("cz1")->nX;
	m_pForm->GetComponent("cz2")->SetEvent(UseItem,this);
	m_nPX2 = m_pForm->GetComponent("cz2")->nX;
	m_pForm->GetComponent("c1")->SetEvent(DestroyItem,this);
	for (int i=0;i<nMaxStoneCount;i++)
	{
		sprintf(sztxt,"lx%d",i+1);
		m_pForm->GetComponent(sztxt)->m_nFontSize = 18;
		sprintf(sztxt,"sz%d",i+1);
		m_pForm->GetComponent(sztxt)->m_nFontSize = 18;
	}

	m_pFormContrast = pMainInterface->GetDForm ("zbduibi");
	m_pFormContrast->bZorde = FORM_ZORDER_CHAT_TIP;
	m_pFormContrast->GetComponent("b1")->m_nOrder = 999;
}

EquipInfoMenu::~EquipInfoMenu()
{

}

void EquipInfoMenu::Open(ItemObj* pObj,int nOpenStyle)
{
	m_pItemObj = pObj;
	m_pItemBase = pObj->pItemBase;
	m_nOpenStyle = nOpenStyle;
	m_bUpdate = true;
	m_pForm->Open();
	m_pForm->GetComponent("cz1")->SetVisible(true);
	m_pForm->GetComponent("cz2")->SetVisible(true);
	m_pForm->GetComponent("cz2")->SetPosX(m_nPX2);
	m_pForm->GetComponent("c2")->SetVisible(false);//===zjk  分解按钮暂时隐去
	switch(m_nOpenStyle){
	case FORM_BAG:
		{
			m_pForm->GetComponent("cz1")->SetPosX(86);
			if (pObj->pItemBase->byType == ITEMTYPE_EQUIP)
			{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_QH_CZ.png");
			}
			if(m_pItemObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG){
				m_pForm->GetComponent("cz2")->SetViewSpr("BT_ZB_CZ.png");
			}else{
				m_pForm->GetComponent("cz2")->SetViewSpr("BT_XX_CZ.png");
			}
		}
		break;
	case FORM_BANK:
		{
		//	m_pForm->GetComponent("cz1")->SetPosX(86);
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			if(m_pItemObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG){
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_YH_CUNRU.png");
			}else{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_YH_QUCHU.png");
			}
		}
		break;
	case FORM_EQUIP_INTENSIFY:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_YYJL.png");
		}
		break;
	case FORM_EQUIP_INTENSIFYEX:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_JSZY.png");
		}
		break;
	case FORM_SHOP:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			//m_pForm->GetComponent("cz1")->SetViewSpr("BT_KSCS.png");
			if (pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_SELL)
			{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_HUIGOU.png");
			}else{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_KSCS.png");

			}
		}
		break;
	case FORM_NULL:
		{
			m_pForm->GetComponent("cz1")->SetVisible(false);
			m_pForm->GetComponent("cz2")->SetVisible(false);
		}
		break;
	case FORM_SHOW:
		{
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_ZHANSHI.png");
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("c1")->SetVisible(false);
			//m_pForm->GetComponent("c2")->SetVisible(false);
		}
		break;
	}

	m_pFormContrast->GetComponent("b1")->pSTextCenter->ClearSText();
	ItemObj* pNow = NULL;
	if (pObj&&pObj->pItemBase)
		pNow = pChar->m_pItemCenter->GetEquipBySlot(pObj->pItemBase->bySlot);
	int nLine = 0;
	if (pNow&&pNow != pObj)
	{
		int nParambase1[EQUIP_ATTR_MAX] = {0};
		pObj->GetAllParam(nParambase1);
		int nParambase2[EQUIP_ATTR_MAX] = {0};
		pNow->GetAllParam(nParambase2);
		m_pFormContrast->GetComponent("b1")->pSTextCenter->AddSText(nLine,0,true,"与当前装备对比",255,255,255);
		nLine++;
		char sztxt[64] = {0};
		for (int i=0;i<EQUIP_ATTR_MAX;i++)
		{
			int nVal = nParambase1[i]-nParambase2[i];
			if (nVal)
			{
				sprintf(sztxt,"%s %s%d",szAttribute[i],nVal>0?"+":"",nVal);
				m_pFormContrast->GetComponent("b1")->pSTextCenter->AddSText(nLine,0,true,sztxt,nVal>0?nItemLimitColor[1][0]:nItemLimitColor[0][0],nVal>0?nItemLimitColor[1][1]:nItemLimitColor[0][1],nVal>0?nItemLimitColor[1][2]:nItemLimitColor[0][2]);
				nLine++;
			}
		}
		int nVal = pObj->GetScore()-pNow->GetScore();
		sprintf(sztxt,"装备评分 %d",nVal);
		m_pFormContrast->GetComponent("b1")->pSTextCenter->AddSText(nLine,0,true,sztxt,nVal>0?nItemLimitColor[1][0]:nItemLimitColor[0][0],nVal>0?nItemLimitColor[1][1]:nItemLimitColor[0][1],nVal>0?nItemLimitColor[1][2]:nItemLimitColor[0][2]);
		nLine++;
		m_pFormContrast->Open();
	}
	if (pObj->pItemBase->nGroup)
	{
		_setGroupInfo(m_pFormContrast->GetComponent("b1"),pObj->pItemBase->nGroup);
		m_pFormContrast->Open();

	}

}

void EquipInfoMenu::Open( ItemExBase* pObj,int nOpenStyle /*= FORM_EX*/,void* param )
{
	m_pItemObj = NULL;
	m_pItemBase = pObj;
	m_nOpenStyle = nOpenStyle;
	m_bUpdate = true;
	m_pParam = param;
	m_pForm->Open();
	m_pForm->GetComponent("cz1")->SetVisible(true);
	m_pForm->GetComponent("cz2")->SetVisible(true);
	m_pForm->GetComponent("cz2")->SetPosX(m_nPX2);
	m_pForm->GetComponent("c2")->SetVisible(false);    //===zjk 分解按钮暂时隐去
	switch(m_nOpenStyle){
	case FORM_SHOP:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_GM_CZ.png");
		}
		break;
	case FORM_NULL:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetVisible(false);
		//	m_pForm->GetComponent("c2")->SetVisible(false);
			m_pForm->GetComponent("c1")->SetVisible(false);
		}
		break;
	case FORM_MALL:
		{
			if (pObj->byType == ITEMTYPE_EQUIP)
			{
				m_pForm->GetComponent("cz1")->SetPosX(86);
				m_pForm->GetComponent("cz2")->SetVisible(true);
				m_pForm->GetComponent("cz2")->SetViewSpr("BT_SHICHUANG.png");
			}
			if (pObj->byType == ITEMTYPE_RIDE)
			{
				m_pForm->GetComponent("cz1")->SetPosX(86);
				m_pForm->GetComponent("cz2")->SetVisible(true);
				m_pForm->GetComponent("cz2")->SetViewSpr("BT_SHIQI.png");
			}
			MallItemCfg* pItem = (MallItemCfg*)param;
			if(pItem && pItem->m_nLimit == -1)
			{
				m_pForm->GetComponent("cz1")->SetVisible(false);
				m_pForm->GetComponent("cz2")->SetPosX(158);
			}
			else
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_GM_CZ.png");
		}
		break;
	}
	if (pObj->nGroup)
	{
		m_pFormContrast->GetComponent("b1")->pSTextCenter->ClearSText();
		_setGroupInfo(m_pFormContrast->GetComponent("b1"),pObj->nGroup);
		m_pFormContrast->Open();

	}
}

void EquipInfoMenu::Close()
{
	m_pForm->Close();
	m_pFormContrast->Close();
}
void ChangeJobToTxt(char* sztxt,int nJob){
	sztxt[0] = 0;
	if (nJob == ((1<<5)-1))
	{
		strcpy(sztxt,"无");
		return;
	}
	if(nJob&1<<0) strcat(sztxt,"无  "); 
	if(nJob&1<<1) strcat(sztxt,"战士  "); 
	if(nJob&1<<2) strcat(sztxt,"法师  "); 
	if(nJob&1<<3) strcat(sztxt,"枪手  "); 
	if(nJob&1<<4) strcat(sztxt,"牧师"); 
}

void EquipInfoMenu::Update()
{
	if (!m_pForm->bOpened&&m_pFormContrast->bOpened)
	{
		m_pFormContrast->Close();
	}
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
// 		if (!m_pItemObj)
// 		{
// 			return;
// 		}
		if (m_pItemObj)
		{
			SetItemToDCom(m_pForm->GetComponent("ico"),m_pItemObj);
			SetItemNameToDCom(m_pForm->GetComponent("name"),m_pItemObj);
		}else{
			SetItemToDCom(m_pForm->GetComponent("ico"),m_pItemBase->nTypeID);
			SetItemNameToDCom(m_pForm->GetComponent("name"),m_pItemBase);
		}
		

		m_pForm->GetComponent("buwei")->SetCaption(szEquipSlot[m_pItemBase->bySlot],0);
		if (m_pItemBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN))
		{m_pForm->GetComponent("najiu")->SetCaptionEx("耐久:%d/%d",0,m_pItemObj?m_pItemObj->GetVal(ITEM_AD_NUM):m_pItemBase->nDura,m_pItemBase->nDura);}else{
			m_pForm->GetComponent("najiu")->SetCaptionEx("",0);
		}
		m_pForm->GetComponent("lv")->SetCaptionEx("使用等级:%d",0,m_pItemBase->byLv);
		if (m_pItemBase->byLv > pChar->m_byLvl)
		{
			m_pForm->GetComponent("lv")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
		}
		else{
			m_pForm->GetComponent("lv")->SetSprColor(255,255,255);
		}
		char sztxt[32];
		ChangeJobToTxt(sztxt,m_pItemBase->nJob);
		m_pForm->GetComponent("zhiye")->SetCaptionEx("职业限制:%s",0,sztxt);
		if (!(m_pItemBase->nJob &(1<<pChar->m_byJob)))
		{
			m_pForm->GetComponent("zhiye")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
		}
		else{
			m_pForm->GetComponent("zhiye")->SetSprColor(255,255,255);
		}
		if (m_nOpenStyle != FORM_SHOP||m_pItemObj)
		{
			if (m_pItemBase->nFlag&1<<ITEM_FLAG_SELL)
			{
				m_pForm->GetComponent("moneyC15")->SetCaptionEx("    %d",0,m_pItemBase->nPrice);
				m_pForm->GetComponent("m")->SetVisible(true);
				m_pForm->GetComponent("moneyC15")->SetSprColor(255,255,255);
			}else{
				m_pForm->GetComponent("moneyC15")->SetCaptionEx("%s",0,"不可售店");
				m_pForm->GetComponent("m")->SetVisible(false);
				m_pForm->GetComponent("moneyC15")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
			}
		}
		sztxt[0] = 0;
		if (m_pItemBase->nFlag&1<<ITEM_FLAG_BANK)
		{
			strcat(sztxt,"可存储 ");
			m_pForm->GetComponent("xianzhi1")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
		}else{
			strcat(sztxt,"不可存储 ");
			m_pForm->GetComponent("xianzhi1")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
		}
		m_pForm->GetComponent("xianzhi1")->SetCaptionEx("%s",0,sztxt);
		sztxt[0] = 0;
		if (m_pItemBase->byBind == ITEM_BINDTYPE_BIND_GET||(m_pItemObj?m_pItemObj->GetVal(ITEM_AD_BIND):false))
		{
			strcat(sztxt,"不可交易 ");
			m_pForm->GetComponent("xianzhi2")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
		}else if(m_pItemBase->byBind == ITEM_BINDTYPE_BING_EQUIP){
			strcat(sztxt,"装备绑定 ");
			m_pForm->GetComponent("xianzhi2")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
		}else{
			strcat(sztxt,"可交易 ");
			m_pForm->GetComponent("xianzhi2")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
		}
		m_pForm->GetComponent("xianzhi2")->SetCaptionEx("%s",0,sztxt);

		bool bEquip = m_pItemObj?pChar->m_pItemCenter->CheckEquip(m_pItemObj):true;
		if(m_nOpenStyle == FORM_NULL || m_nOpenStyle == FORM_SHOW|| m_nOpenStyle == FORM_SHOP||m_nOpenStyle == FORM_EX)
		{
			m_pForm->GetComponent("c1")->SetVisible(false);
			//m_pForm->GetComponent("c2")->SetVisible(false);
		}
		else if(bEquip)
		{
			m_pForm->GetComponent("c1")->SetVisible(!bEquip);
			//m_pForm->GetComponent("c2")->SetVisible((m_pItemObj->pItemBase->nFlag&1<<ITEM_FLAG_RESOLVE)&&!bEquip);
		}else{
			m_pForm->GetComponent("c1")->SetVisible(m_pItemObj->pItemBase->nFlag&1<<ITEM_FLAG_DESTROY);
		}
		

		char sztxt2[256] = {0};
		char sztxt3[256] = {0};
		m_pForm->GetComponent("shuxing1")->pSTextCenter->ClearSText();
		m_pForm->GetComponent("shuxing2")->pSTextCenter->ClearSText();
		m_pForm->GetComponent("jl1")->pSTextCenter->ClearSText();
		if (m_pItemObj)
		{
			bool bFlag = true;
			int nParambase[EQUIP_ATTR_MAX] = {0};
			m_pItemObj->GetBaseData(nParambase);
			for (int i=0;i<EQUIP_ATTR_MAX;i++)
			{
				if (nParambase[i])
				{
					if (bFlag)
					{
						sprintf(sztxt,"%s:%d",szAttribute[i],nParambase[i]);
						m_pForm->GetComponent("shuxing1")->pSTextCenter->AddSText(0,0,false,(char*)sztxt,255,255,255);
						if (m_pItemObj->GetStarLv())
						{
							sprintf(sztxt,"+%d",nParambase[i]*m_pItemObj->GetStarPer(m_pItemObj->GetStarLv())/10000);
							m_pForm->GetComponent("shuxing1")->pSTextCenter->AddSText(1,0,false,(char*)sztxt,nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
						}
						bFlag = false;
					}else{
						sprintf(sztxt,"%s:%d",szAttribute[i],nParambase[i]);
						m_pForm->GetComponent("shuxing2")->pSTextCenter->AddSText(0,0,false,(char*)sztxt,255,255,255);
						if (m_pItemObj->GetStarLv())
						{
							sprintf(sztxt,"+%d",nParambase[i]*m_pItemObj->GetStarPer(m_pItemObj->GetStarLv())/10000);
							m_pForm->GetComponent("shuxing2")->pSTextCenter->AddSText(1,0,false,(char*)sztxt,nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
						}
						break;
					}

				}
			}
			int nEatLv = m_pItemObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV);
			int nLv = nEatLv<<24>>24;
			if (nLv)
			{
				EquipEatBase* pBase = EquipEatConfig::GetInstance().GetEquipEat(nLv);
				if (pBase)
				{
					int nAddType = EquipEatConfig::GetInstance().GetAddParam(m_pItemObj->pItemBase->bySlot);
					int nAddVal = nAddType?pBase->nDefense:pBase->nDamage;
					char sztxt[64];
					sprintf(sztxt,"精炼效果:额外%s+%d",nAddType?"防御":"攻击",nAddVal);
					STextObj* pText = m_pForm->GetComponent("jl1")->pSTextCenter->AddSText(0,0,true,sztxt,255,255,255);
					if(pText) pText->nOffY = -3;
				}
				
			}
			if (m_pItemObj->GetScore())
				m_pForm->GetComponent("pingfen")->SetCaptionEx("装备评分:%d",0,m_pItemObj->GetScore());
			else if(m_pItemObj->pItemBase->dwTimeLimit)
			{
				DWORD nTime = m_pItemObj->GetVal(ITEM_AD_LIMITTIME)- xnGetSecCount();
				m_pForm->GetComponent("pingfen")->SetCaptionEx("剩余时间:%d天%d小时%d分",0,nTime/(24*3600),nTime%(24*3600)/3600,nTime%3600/60);
				m_pForm->GetComponent("pingfen")->SetSprColor(255,0,0);
			}else
				m_pForm->GetComponent("pingfen")->SetCaption("",0);
		}else{
			if (m_pItemBase->pAddAttr->Count)
			{
				EquipAttrBase* pBase = EquipAttrConfig::GetInstance().GetEquipAttr((int)m_pItemBase->pAddAttr->Items[0]);
				if (pBase)
				{
					bool bflag = true;
					char sztxt[32];
					for (int i=0;i<EQUIP_ATTR_MAX;i++){
						if (pBase->nBase[i]&&bflag)
						{
							sprintf(sztxt,"%s:%d~%d",szAttribute[i],pBase->nBase[i],pBase->nBase[i]+pBase->nRandom[i]);
							m_pForm->GetComponent("shuxing1")->pSTextCenter->AddSText(0,0,true,(char*)sztxt,255,255,255)->nOffY -=3;
							bflag = false;
						}else if (pBase->nBase[i])
						{
							sprintf(sztxt,"%s:%d~%d",szAttribute[i],pBase->nBase[i],pBase->nBase[i]+pBase->nRandom[i]);
							m_pForm->GetComponent("shuxing2")->pSTextCenter->AddSText(0,0,true,(char*)sztxt,255,255,255)->nOffY -=3;
							break;
						}
					}
				}
			}			
		}
		

		DComponent* comIcon[nMaxStoneCount];
		DComponent* comType[nMaxStoneCount];
		DComponent* comVal[nMaxStoneCount];
		DComponent* comBack[nMaxStoneCount];
		for (int i=0;i<nMaxStoneCount;i++)
		{
			sprintf(sztxt,"icon%d",i+1);
			comIcon[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"lx%d",i+1);
			comType[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"sz%d",i+1);
			comVal[i] = m_pForm->GetComponent(sztxt);
			sprintf(sztxt,"g%d",i+1);
			comBack[i] = m_pForm->GetComponent(sztxt);
		}
		SetEquipStoneToDCom(comIcon,comType,comVal,m_pItemObj,comBack,0.78571);
		
		//m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,m_pItemObj->pItemBase->byLv);
	}
}

void EquipInfoMenu::SetButtonSpr( const char* szSpr1,const char* szSpr2 /*= NULL*/ )
{
	m_pForm->GetComponent("cz1")->SetVisible(szSpr1);
	m_pForm->GetComponent("cz2")->SetVisible(szSpr2);
	if (!szSpr2)
	{
		m_pForm->GetComponent("cz1")->SetPosX(158);
	}else
	{
		m_pForm->GetComponent("cz1")->SetPosX(86);
	}
	if (szSpr1)
	{
		m_pForm->GetComponent("cz1")->SetViewSpr(szSpr1);
	}
	if(szSpr2)
	{
		m_pForm->GetComponent("cz2")->SetViewSpr(szSpr2);
	}
}

void EquipInfoMenu::SetCallBackEvent( void (*fnCallBack)(void* pParam,BYTE byType) )
{
	fCallBackEvent = fnCallBack;
}

void EquipInfoMenu::_setExtParam( DComponent* pCom1,DComponent* pCom2,ItemExBase* m_pItemBase )
{
	if (!pCom1||!pCom1->pSTextCenter)
	{
		return;
	}
	if (!pCom2||!pCom2->pSTextCenter)
	{
		return;
	}
	
	
}

void EquipInfoMenu::_setGroupInfo( DComponent* pCom1,int nGroup )
{
	if (!pCom1->pSTextCenter)
	{
		return;
	}
	EquipGroupInfo* pInfo = EquipGroupConfig::GetInstance().GetItem(nGroup);
	if (!pInfo)
	{
		return;
	}
	pCom1->pSTextCenter->AddSText(0,0,true,"套装属性",204,129,11);
	char sztxt[32];
	int nGroupCount = pChar->m_pItemCenter->GetEquipCountInGroup(nGroup);
	sprintf(sztxt,"%s  (%d/%d)",pInfo->szName,nGroupCount>pInfo->nItemID.size()?pInfo->nItemID.size():nGroupCount,pInfo->nItemID.size());
	pCom1->pSTextCenter->AddSText(0,0,true,sztxt,0,255,0);
	for (std::list<int>::iterator itor = pInfo->nItemID.begin();itor!=pInfo->nItemID.end();itor++)
	{
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(*itor);
		if (pItem)
		{
			ItemObj* pObj = pChar->m_pItemCenter->GetEquipBySlot(pItem->bySlot);
			bool bEquip = (pObj&&pObj->pItemBase==pItem )?true:false;
			pCom1->pSTextCenter->AddSText(0,0,true,pItem->szName,bEquip?0:127,bEquip?255:127,bEquip?0:127);
		}
	}
	for (std::list<EquipGroupVal>::iterator itor = pInfo->groupVal.begin();itor!=pInfo->groupVal.end();itor++)
	{
		sprintf(sztxt,"%d件:%s",itor->byCount,itor->szDesc);
		pCom1->pSTextCenter->AddSText(0,0,true,sztxt,nGroupCount>=itor->byCount?180:127,nGroupCount>=itor->byCount?0:127,nGroupCount>=itor->byCount?255:127);
	}
	
}

static void AskDestroyItem2(void* param,int nVal)
{
	if(nVal == 1)
	{
		ItemInfoMenu* pMenu = (ItemInfoMenu*)param;
		pChar->m_pItemCenter->DestroyItem(pMenu->m_pItemObj);
		pMenu->Close();
	}else{

	}
}
static void DestroyItem2(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemInfoMenu* pMenu = (ItemInfoMenu*)param;
	if (pMenu->m_pItemObj->pItemBase->nStar>=2)
	{
		pMainMenu->PopAskMsgBox("此物品较为珍贵,是否确认销毁?",AskDestroyItem2,param);
	}else{
		pChar->m_pItemCenter->DestroyItem(pMenu->m_pItemObj);
		pMenu->Close();
	}
}
static void FastItem2(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemInfoMenu* pMenu = (ItemInfoMenu*)param;
	if (!pMenu->m_pItemObj)
	{
		return;
	}
	if (pMenu->m_pItemObj->pItemBase->byLv > pChar->m_byLvl)
	{
		pMainMenu->PopTipMsg("等级不足，无法设置快捷！");
		return;
	}
	pMainMenu->PopTipMsg("设置快捷成功");
	BuildPacketEx(pPacket, USER, CS_SEND_FAST_ITEM, buffer,1024);
	SET_DATA(pdata, USER, CS_SEND_FAST_ITEM, pPacket);


	pdata->dwItemID= pMenu->m_pItemObj->GetVal(ITEM_AD_TYPE);	
	pChar->SendPacket(pPacket);
	pMenu->Close();

}

void fun_ChangeBackLogin(void * pParam,int nVal)
{
	if(nVal == 1)
	{
		ItemObj* pObj = (ItemObj*)pParam;
		if (pObj)
			pChar->m_pItemCenter->UseItem(pObj);
	}
}


static void UseItem2(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemInfoMenu* pMenu = (ItemInfoMenu*)param;
	switch(pMenu->m_nOpenStyle){
	case FORM_BAG:
		{
			if (pMenu->m_pItemBase->byType == ITEMTYPE_STONE)
			{
				if (pMenu->m_pItemBase->nDura)
				{
					pMainMenu->m_pStoneComposeMenu->Open(pMenu->m_pItemBase->nTypeID);
				}else{
					pMainMenu->PopTipMsg("该宝石无法合成");
				}
			}else if (pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_ITEMCANUSE||pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_FAST||pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_SKILLBOOK
				||pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_RIDE||pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_GIFT)
			{
				if (pMenu->m_pItemObj->pItemBase->nTypeID == 45015)
				{
					pMainMenu->PopAskMsgBox("您确定转换国家，返回登录界面吗？",fun_ChangeBackLogin,pMenu->m_pItemObj,1);
				}else
					pChar->m_pItemCenter->UseItem(pMenu->m_pItemObj);
			}else if (pMenu->m_pItemBase->bySlot == 9)
			{
				pChar->m_pItemCenter->UseItem(pMenu->m_pItemObj);
			}
		}
		break;
	case FORM_BANK:
		{

			pChar->m_pItemCenter->SetItemBank(pMenu->m_pItemObj);
		}
		break;
	case FORM_EQUIP_INTENSIFY:
		{
			if (!EquipStarMenu::sLvUpEquipObj)
			{
				break;
			}
			if (pMenu->m_pItemBase->byType == ITEMTYPE_STONE)
			{
				if (pMenu->m_pItemObj)
				{
					pChar->m_pItemCenter->EquipInlay(EquipStarMenu::sLvUpEquipObj,pMenu->m_pItemObj);
				}else{
					char sztxt[128];
					int nLv = pMenu->m_pItemBase->nTypeID%10;
					int nMoney = nLv*2500+5000;
					sprintf(sztxt,"是否消耗%d金币来摘除宝石?",nMoney);
					pMainMenu->PopAskMsgBox(sztxt,AskRemoveStone,(void*)nMoney);
				}
			}
		}
		break;
	case FORM_MALL:
		{
			MallItemCfg* pItem = (MallItemCfg*)pMainMenu->m_pItemInfoMenu->m_pParam;//MallConfig::GetInstance().GetCfg(pMenu->m_pItemBase->nTypeID);
			if (pItem)
			{
				if(pItem->m_nLimit == -1)
					break;
				int nMaxCount = 1000;//pChar->m_dwRmb*100/pItem->m_nPrice/pItem->m_nDiscount;
				//nMaxCount = nMaxCount>1000?1000:nMaxCount;
				if (pItem->m_nFlag != -1)
				{
					int nFlag = pChar->m_pClientFlag->GetSLimitF(pItem->m_nFlag);
					if (nFlag>=pItem->m_nLimit)
					{
						break;
					}
					nMaxCount = nMaxCount<(pItem->m_nLimit-nFlag)?nMaxCount:(pItem->m_nLimit-nFlag);
				}

				pMainMenu->m_pGetNumMenu->Open(InputResult,nMaxCount,0,(void*)pItem);
				
			}
			
			//m_pForm->GetComponent("cz1")->SetViewSpr("BT_GM_CZ.png");
		}
		break;
	case FORM_SHOP:
		{
			if (pMenu->m_pItemObj)
			{
				if (pMenu->m_pItemObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_SELL)
				{
					pChar->m_pItemCenter->SellItem(pMenu->m_pItemObj);
				}else{
					pChar->m_pItemCenter->SellItem(pMenu->m_pItemObj);
				}
			}else{
				ShopItemCfg* pItem = ShopConfig::GetInstance().GetCfg((int)pMenu->m_pParam,pMenu->m_pItemBase->nTypeID);
				if (pItem)
				{
					int nMaxCount = 1000;//pChar->m_dwMoney/pItem->m_nPrice;
					//nMaxCount = nMaxCount>1000?1000:nMaxCount;
					//if (!nMaxCount)
					//{
					//	pMainMenu->PopTipMsg("金币不足");
					//	return;
					//}
					pMainMenu->m_pGetNumMenu->Open(InputResultShop,nMaxCount,0,(void*)pItem);

				}
			}
		}
		break;
	case FORM_SHOW:
		{
			pMainMenu->m_pChatMenu->InsertItem(pMenu->m_pItemObj);
			pMenu->Close();
			pMainMenu->m_pChatMenu->m_pItemMenu->Close();
		}
		break;
	case FORM_MANOR:
		{
			pChar->m_pFortCenter->SeedManorPos(pMainMenu->m_pManorMenu->m_nSelect,pMenu->m_pItemObj);
			pMenu->Close();
			pMainMenu->m_pChatMenu->m_pItemMenu->Close();
		}
		break;
	case FORM_EX:
		{
			if (pMenu->fCallBackEvent)
			{
				pMenu->fCallBackEvent(pMenu->m_pItemObj?(void*)pMenu->m_pItemObj:(void*)pMenu->m_pItemBase,0);
			}
			pMenu->Close();
		}
		break;
	case FORM_PETSKILL:
		{
			if(pMenu->m_pItemObj)
			{
				pChar->m_pSlaveCenter->RequestLearnSkill(pMainMenu->m_pPetSkillMenu->byCur,pMenu->m_pItemObj->GetVal(ITEM_AD_ID));
			}
			pMenu->Close();
		}
		break;
	}

	if (pMenu->m_nOpenStyle == FORM_BAG 
		&& pMenu->m_pItemObj 
		&& !pMenu->m_pItemObj->pItemBase->nCast 
		&& pMenu->m_pItemObj->pItemBase->byType == ITEMTYPE_GIFT 
		&& pMenu->m_pItemObj->GetVal(ITEM_AD_NUM) > 1)
	{
		pMenu->m_bUpdate = true;
	}
	else
		pMenu->Close();
}
static void UseItem2Ex(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemInfoMenu* pMenu = (ItemInfoMenu*)param;
	switch(pMenu->m_nOpenStyle){
	case FORM_MALL:
		{
			if (pMenu->m_pItemBase->byType != ITEMTYPE_EQUIP&&pMenu->m_pItemBase->byType != ITEMTYPE_RIDE)
			{
				return;
			}
			switch(pMenu->m_pItemBase->byType)
			{
			case ITEMTYPE_EQUIP:
				{
					if (pMenu->m_pItemBase->bySexLimit&&(pMenu->m_pItemBase->nSkin > 1500)&&(pMenu->m_pItemBase->bySexLimit!=pChar->m_bySex+1))
					{
						pMainMenu->PopTipMsg("性别不符，无法试穿");
						return;
					}
					pMainMenu->m_pMallMenu->m_nCloth = pMenu->m_pItemBase->nSkin;
					pMainMenu->m_pMallMenu->m_nHorse = 0;
					pMainMenu->m_pMallMenu->m_bMallUpdate = true;
				}
				break;
			case ITEMTYPE_RIDE:
				{
					HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(pMenu->m_pItemBase->nSkin);
					if (pBasic)
					{
						pMainMenu->m_pMallMenu->m_nHorse = pBasic->nSkin;
						pMainMenu->m_pMallMenu->m_bMallUpdate = true;
					}
				}
				break;
			}
		}
		break;
	}
	pMenu->Close();
}
ItemInfoMenu::ItemInfoMenu():m_bUpdate(true),m_nOpenStyle(0),m_pParam(NULL),fCallBackEvent(NULL)
{
	m_pForm = pMainInterface->GetDForm ("zb2");
	m_pForm->bZorde = FORM_ZORDER_CHAT_TIP;
	char sztxt[32];
	m_pForm->GetComponent("cz1")->SetEvent(UseItem2,this);
	m_nPX1 = m_pForm->GetComponent("cz1")->nX;
	m_pForm->GetComponent("cz2")->SetEvent(UseItem2Ex,this);
	m_nPX2 = m_pForm->GetComponent("cz2")->nX;
	m_pForm->GetComponent("c1")->SetEvent(DestroyItem2,this);
	m_pForm->GetComponent("c2")->SetEvent(FastItem2,this);
}

ItemInfoMenu::~ItemInfoMenu()
{

}

void ItemInfoMenu::Open( ItemObj* pObj,int nOpenStyle)
{
	m_pItemObj = pObj;
	m_nOpenStyle = nOpenStyle;
	m_bUpdate = true;
	m_pForm->Open();
	m_pForm->GetComponent("cz1")->SetPosX(158);
	m_pForm->GetComponent("cz2")->SetVisible(false);
	m_pForm->GetComponent("cz2")->SetPosX(m_nPX2);
	m_pForm->GetComponent("c2")->SetVisible(false);
	m_pForm->GetComponent("cz1")->SetVisible(true);
	m_pItemBase = pObj->pItemBase;
	switch(m_nOpenStyle){
	case FORM_BAG:
		{
			if (m_pItemObj->pItemBase->byType == ITEMTYPE_STONE)
			{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_HC_CZ.png");
			}else if(m_pItemObj->pItemBase->byType == ITEMTYPE_ITEMCANUSE||m_pItemObj->pItemBase->byType == ITEMTYPE_FAST
				||m_pItemObj->pItemBase->byType == ITEMTYPE_RIDE||m_pItemObj->pItemBase->byType == ITEMTYPE_GIFT){
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_SY_CZ.png");
			}else if(m_pItemObj->pItemBase->byType == ITEMTYPE_SKILLBOOK)
			{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_CW_XUEXI.png");
			}
			else if (m_pItemObj->pItemBase->bySlot == 9) //===zjk
			{
				if(m_pItemObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG) 
				{
					m_pForm->GetComponent("cz1")->SetViewSpr("BT_ZB_CZ.png");
				}else{
					m_pForm->GetComponent("cz1")->SetViewSpr("BT_XX_CZ.png");
				}
			}else
			{
				m_pForm->GetComponent("cz1")->SetVisible(false);
			}
		}
		break;
	case FORM_BANK:
		{
			if(m_pItemObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG){
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_YH_CUNRU.png");
			}else{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_YH_QUCHU.png");
			}
		}
		break;
	case FORM_EQUIP_INTENSIFY:
		{
			if(m_pItemObj->pItemBase->byType == ITEMTYPE_STONE){
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_XQ_CZ.png");
			}else{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_XX_CZ.png");
			}
		}
		break;
	case FORM_SHOP:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			if (pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_SELL)
			{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_HUIGOU.png");
			}else{
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_KSCS.png");

			}
		}
		break;
	case FORM_NULL:
		{
			m_pForm->GetComponent("cz1")->SetVisible(false);
			m_pForm->GetComponent("cz2")->SetVisible(false);
		}
		break;
	case FORM_PETSKILL:
		{
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_CW_XUEXI.png");
			m_pForm->GetComponent("cz2")->SetVisible(false);
		}
		break;
	case FORM_SHOW:
		{
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_ZHANSHI.png");
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("c1")->SetVisible(false);
		}
		break;
	case FORM_MANOR:
		{
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_ZHONGZHI.png");
			m_pForm->GetComponent("cz2")->SetVisible(false);
		}
		break;
	}
}

void ItemInfoMenu::Open( ItemExBase* pObj,int nOpenStyle,void* param )
{
	m_pItemObj = NULL;
	m_pParam = param;
	m_pItemBase = pObj;
	m_nOpenStyle = nOpenStyle;
	m_bUpdate = true;
	m_pForm->Open();
	m_pForm->GetComponent("cz1")->SetPosX(158);
	m_pForm->GetComponent("cz2")->SetVisible(false);
	m_pForm->GetComponent("cz2")->SetPosX(m_nPX2);
	m_pForm->GetComponent("cz1")->SetVisible(true);
	m_pForm->GetComponent("c2")->SetVisible(false);
	switch(m_nOpenStyle){
	case FORM_BAG:
		{
			m_pForm->GetComponent("cz1")->SetVisible(false);
			
		}
		break;
	case FORM_BANK:
		{
			m_pForm->GetComponent("cz1")->SetVisible(false);
		}
		break;
	case FORM_EQUIP_INTENSIFY:
		{
			if(m_pItemBase->byType == ITEMTYPE_STONE){
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_ZHAICHU.png");
			}else{
				m_pForm->GetComponent("cz1")->SetVisible(false);
			}
		}
		break;
	case FORM_MALL:
		{
			if (pObj->byType == ITEMTYPE_EQUIP)
			{
				m_pForm->GetComponent("cz1")->SetPosX(86);
				m_pForm->GetComponent("cz2")->SetVisible(true);
				m_pForm->GetComponent("cz2")->SetViewSpr("BT_SHICHUANG.png");
			}
			if (pObj->byType == ITEMTYPE_RIDE)
			{
				m_pForm->GetComponent("cz1")->SetPosX(86);
				m_pForm->GetComponent("cz2")->SetVisible(true);
				m_pForm->GetComponent("cz2")->SetViewSpr("BT_SHIQI.png");
			}
			MallItemCfg* pItem = (MallItemCfg*)param;
			if(pItem && pItem->m_nLimit == -1)
			{
				m_pForm->GetComponent("cz1")->SetVisible(false);
				m_pForm->GetComponent("cz2")->SetPosX(158);
			}
			else
				m_pForm->GetComponent("cz1")->SetViewSpr("BT_GM_CZ.png");
		}
		break;
	case FORM_SHOP:
		{
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_GM_CZ.png");
		}
		break;
	case FORM_NULL:
		{
			m_pForm->GetComponent("cz1")->SetVisible(false);
			m_pForm->GetComponent("cz2")->SetVisible(false);
		}
		break;
	case FORM_SHOW:
		{
			m_pForm->GetComponent("cz1")->SetPosX(158);
			m_pForm->GetComponent("cz1")->SetViewSpr("BT_ZHANSHI.png");
			m_pForm->GetComponent("cz2")->SetVisible(false);
			m_pForm->GetComponent("c1")->SetVisible(false);
		}
		break;
	}
	
}

void ItemInfoMenu::Close()
{
	m_pForm->Close();
}

void ItemInfoMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		if (!m_pItemBase)
		{
			return;
		}
		if(m_pItemObj){
			SetItemToDCom(m_pForm->GetComponent("ico"),m_pItemObj);
		}else{
			SetItemToDCom(m_pForm->GetComponent("ico"),m_pItemBase->nTypeID);
		}

		m_pForm->GetComponent("pingfen")->SetCaption("",0);

		m_pForm->GetComponent("zb1C0name")->SetCaption(m_pItemBase->szName,0);
		m_pForm->GetComponent("zb1C0name")->SetSprColor(nItemColor[m_pItemBase->nStar][0],nItemColor[m_pItemBase->nStar][1],nItemColor[m_pItemBase->nStar][2]);

		m_pForm->GetComponent("buwei")->SetCaption(szItemType[m_pItemBase->byType],0);

		m_pForm->GetComponent("lv")->SetCaptionEx("使用等级:%d",0,m_pItemBase->byLv);
		char sztxt[32];
		ChangeJobToTxt(sztxt,m_pItemBase->nJob);
		m_pForm->GetComponent("zhiye")->SetCaptionEx("职业限制:%s",0,sztxt);
		bool bLimit = m_pItemBase->nJob&(1<<pChar->m_byJob);
		m_pForm->GetComponent("zhiye")->SetSprColor(bLimit?255:nItemLimitColor[0][0],bLimit?255:nItemLimitColor[0][1],bLimit?255:nItemLimitColor[0][2]);
		if (m_nOpenStyle != FORM_SHOP||m_pItemObj)
		{
			if (m_pItemBase->nFlag&1<<ITEM_FLAG_SELL )
			{
				m_pForm->GetComponent("money")->SetCaptionEx("    %d",0,m_pItemBase->nPrice);
				m_pForm->GetComponent("m")->SetVisible(true);
				m_pForm->GetComponent("money")->SetSprColor(255,255,255);
			}else{
				m_pForm->GetComponent("money")->SetCaptionEx("%s",0,"不可售店");
				m_pForm->GetComponent("m")->SetVisible(false);
				m_pForm->GetComponent("money")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
			}
		}
		
		sztxt[0] = 0;
		if (m_pItemBase->nFlag&1<<ITEM_FLAG_BANK)
		{
			strcat(sztxt,"可存储 ");
			m_pForm->GetComponent("xianzhi1")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
		}else{
			strcat(sztxt,"不可存储 ");
			m_pForm->GetComponent("xianzhi1")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
		}
		m_pForm->GetComponent("xianzhi1")->SetCaptionEx("%s",0,sztxt);
		sztxt[0] = 0;
		if (m_pItemBase->byBind == ITEM_BINDTYPE_BIND_GET||(m_pItemObj&&m_pItemObj->GetVal(ITEM_AD_BIND)))
		{
			strcat(sztxt,"不可交易 ");
			m_pForm->GetComponent("xianzhi2")->SetSprColor(nItemLimitColor[0][0],nItemLimitColor[0][1],nItemLimitColor[0][2]);
		}else if(m_pItemBase->byBind == ITEM_BINDTYPE_BING_EQUIP){
			strcat(sztxt,"装备绑定 ");
			m_pForm->GetComponent("xianzhi2")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
		}else{
			strcat(sztxt,"可交易 ");
			m_pForm->GetComponent("xianzhi2")->SetSprColor(nItemLimitColor[1][0],nItemLimitColor[1][1],nItemLimitColor[1][2]);
		}
		m_pForm->GetComponent("xianzhi2")->SetCaptionEx("%s",0,sztxt);

		if(m_nOpenStyle == FORM_NULL || m_nOpenStyle == FORM_PETSKILL|| m_nOpenStyle == FORM_SHOW|| m_nOpenStyle == FORM_SHOP||m_nOpenStyle == FORM_EX)
			m_pForm->GetComponent("c1")->SetVisible(false);
		else
			m_pForm->GetComponent("c1")->SetVisible(m_pItemObj&&m_pItemBase->GetItemFlag(ITEM_FLAG_DESTROY));

// 		char sztxt2[256] = {0};
// 		strcat(sztxt2,m_pItemBase->szDesc);
// 		strcat(sztxt2,"\r\n");
		m_pForm->GetComponent("c2")->SetVisible(m_pItemObj&&m_pItemObj->pItemBase->byType == ITEMTYPE_FAST);
		m_pForm->GetComponent("shuxing")->pSTextCenter->ClearSText();
		m_pForm->GetComponent("shuxing")->pSTextCenter->AddSText(0,0,true,m_pItemBase->szDesc,255,255,255);
		for (int i=0;i<EQUIP_ATTR_MAX;i++)
		{
			if (m_pItemBase->nParam[i])
			{
				sprintf(sztxt,"%s+%d",szAttribute[i],m_pItemBase->nParam[i]);
				//strcat(sztxt2,sztxt);
				m_pForm->GetComponent("shuxing")->pSTextCenter->AddSText(1,0,true,(char*)sztxt,217,104,13);
				//break;
			}
		}
		//m_pForm->GetComponent("shuxing")->SetCaption(sztxt2,0);
		if (m_pItemObj)
		{
			if(m_pItemObj->pItemBase->dwTimeLimit)
			{
				DWORD nTime = m_pItemObj->GetVal(ITEM_AD_LIMITTIME)- xnGetSecCount();
				m_pForm->GetComponent("pingfen")->SetCaptionEx("剩余时间:%d天%d小时%d分",0,nTime/(24*3600),nTime%(24*3600)/3600,nTime%3600/60);
				m_pForm->GetComponent("pingfen")->SetSprColor(255,0,0);
			}else
				m_pForm->GetComponent("pingfen")->SetCaption("",0);
		}
		

	}
}

void ItemInfoMenu::SetCallBackEvent( void (*fnCallBack)(void* pParam,BYTE byType) )
{
	fCallBackEvent = fnCallBack;
}

void ItemInfoMenu::SetButtonSpr( const char* szSpr1,const char* szSpr2 /*= NULL*/ )
{
	m_pForm->GetComponent("cz1")->SetVisible(szSpr1);
	m_pForm->GetComponent("cz2")->SetVisible(szSpr2);
	if (!szSpr2)
	{
		m_pForm->GetComponent("cz1")->SetPosX(158);
	}else
	{
		m_pForm->GetComponent("cz1")->SetPosX(86);
	}
	if (szSpr1)
	{
		m_pForm->GetComponent("cz1")->SetViewSpr(szSpr1);
	}
	if(szSpr2)
	{
		m_pForm->GetComponent("cz2")->SetViewSpr(szSpr2);
	}
}


//===zjk
extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
extern void	SetItemToDCom(DComponent* pDom,int nID);
static void OnceUseItem(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	
	ItemObj* pItem	= pChar->m_pItemCenter->GetItemByIndex((int)param);
	if(!pItem)
	{
		pMainMenu->PopTipMsg("物品不存在！装备失败");
		pMainMenu->m_pFastEquipMenu->m_pListTips->Remove(param);
		pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
		pMainMenu->m_pFastEquipMenu->m_pTipForm->Close();
		return;
	}
	///////年末===zjk
	ItemObj* pNewItems = pChar->m_pItemCenter->GetEquipBySlot(pItem->pItemBase->bySlot);
	if (pNewItems)
	{		
		if ((pItem->GetScore() == pNewItems->GetScore()) && (pItem->pItemBase->nTypeID == pNewItems->pItemBase->nTypeID))
		{
			pMainMenu->PopTipMsg("您已装备该装备");
			pMainMenu->m_pFastEquipMenu->m_pListTips->Remove(param);
			pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
			pMainMenu->m_pFastEquipMenu->m_pTipForm->Close();
			return;
		}else
			pChar->m_pItemCenter->UseItem(pItem);
	}else
		pChar->m_pItemCenter->UseItem(pItem);

	pMainMenu->m_pFastEquipMenu->m_pListTips->Remove(param);

	if (pMainMenu->m_pFastEquipMenu->m_pListTips->Count > 0)
	{
		int nIndex	= (int)pMainMenu->m_pFastEquipMenu->m_pListTips->Items[pMainMenu->m_pFastEquipMenu->m_pListTips->Count - 1];
		ItemObj* pNextItem	= pChar->m_pItemCenter->GetItemByIndex(nIndex);

		if (!pNextItem)
		{
			pMainMenu->m_pFastEquipMenu->m_pListTips->Remove((void*)nIndex);
			pMainMenu->m_pFastEquipMenu->m_pTipForm->Close();
			pMainMenu->m_pFastEquipMenu->CheckListFormClose();
		}
		else
			pMainMenu->m_pFastEquipMenu->OpenTipEquip(pNextItem);
	}else
		pMainMenu->m_pFastEquipMenu->m_pTipForm->Close();

	pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
	pChar->GetMyMapObj()->AddEff(205);
}

static void OpenUseItem(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	
	ItemObj* pItem	= pChar->m_pItemCenter->GetItemByIndex((int)param);
	if(!pItem)
	{
		pMainMenu->PopTipMsg("物品不存在！请打开包裹");
		pMainMenu->m_pFastEquipMenu->m_pListTips->Remove(param);
		pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
		pMainMenu->m_pFastEquipMenu->CheckListFormClose();
		return;
	}		
	ItemObj* pNewItems = pChar->m_pItemCenter->GetEquipBySlot(pItem->pItemBase->bySlot);
	if (pNewItems)
	{		
		if ((pItem->GetScore() == pNewItems->GetScore()) && (pItem->pItemBase->nTypeID == pNewItems->pItemBase->nTypeID))
		{
			pMainMenu->PopTipMsg("您已装备该装备");
			pMainMenu->m_pFastEquipMenu->m_pListTips->Remove(param);
			pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
			pMainMenu->m_pFastEquipMenu->CheckListFormClose();
			return;
		}
	}
	pMainMenu->m_pFastEquipMenu->OpenTipEquip(pItem);
	pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
}

static void CloseUseItem(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	
	ItemObj* pItem	= pChar->m_pItemCenter->GetItemByIndex((int)param);

	///////年末===zjk
	for (int i = 0; i < 4; i++)
	{		
		if(i < pMainMenu->m_pFastEquipMenu->m_pListTips->Count)
		{
			int nIndex	= (int)pMainMenu->m_pFastEquipMenu->m_pListTips->Items[i];
			ItemObj* pListItem	= pChar->m_pItemCenter->GetItemByIndex(nIndex);
			if(!pListItem)
				continue;
			if((pItem->pItemBase->nTypeID == pListItem->pItemBase->nTypeID)&& (pItem->GetScore()==pListItem->GetScore()))
			{
				pMainMenu->m_pFastEquipMenu->m_pListTips->Delete(i);
			}
		}
	}

	if (pMainMenu->m_pFastEquipMenu->m_pListTips->Count > 0)
	{
		int nIndex	= (int)pMainMenu->m_pFastEquipMenu->m_pListTips->Items[pMainMenu->m_pFastEquipMenu->m_pListTips->Count - 1];
		ItemObj* pNextItem	= pChar->m_pItemCenter->GetItemByIndex(nIndex);

		if (!pNextItem)
		{
			pMainMenu->m_pFastEquipMenu->m_pListTips->Remove((void*)nIndex);
			pMainMenu->m_pFastEquipMenu->m_pTipForm->Close();
			pMainMenu->m_pFastEquipMenu->CheckListFormClose();
		}
		else
			pMainMenu->m_pFastEquipMenu->OpenTipEquip(pNextItem);
	}else
		pMainMenu->m_pFastEquipMenu->m_pTipForm->Close();
	pMainMenu->m_pFastEquipMenu->ResetTipsEquip();
}
static void OpenHomeRequest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	
	pMainMenu->m_pFamilyJoinMenu->Open(1);
	pMainMenu->m_pFastEquipMenu->m_pHomeTip->SetVisible(false);
}
static void UserFastEquip(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = (ItemObj*)pEvent->pComponent->GetTag();
	if (pItem)
	{
		pMainMenu->m_pEquipInfoMenu->Open(pItem,FORM_NULL);
	}
}

static void AccpetFightRequest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	

	pChar->DealFightRequest(true);
}

static void	OpenMentoringRequest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	

	pMainMenu->m_pMRequestMenu->Open();
	pMainMenu->m_pFastEquipMenu->m_pMentoringTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pMentoringTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();
}

static void	OpenTeamRequest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	

	pChar->DealTeamRequest(true);
}

static void	OpenMarryRingRequest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	

	pMainMenu->m_pMarryAskMenu->PopMsgbox(pChar->m_pMarryCenter->m_szMsgTitle,pChar->m_pMarryCenter->m_szMsgDetail,NULL,NULL,1);

	pChar->m_pMarryCenter->m_bMarryRingRequest = false;
	pMainMenu->m_pFastEquipMenu->m_pMarryRingTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pMarryRingTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();
}

void cb_GoGetGiftPow(void* pParma, int nVal)
{
	if (nVal)
	{
		pMainMenu->m_pTabSocialityCtrl->Open(1,0);
	}
}

static void	OpenGetFriendGift(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;	

	pMainMenu->m_pFriendMenu->m_bGift = false;
	pMainMenu->m_pFastEquipMenu->m_pFriendGiftTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pFriendGiftTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();

	char szTmp[128];
	sprintf(szTmp,"%s赠送你1点体力，是否前去领取？",pMainMenu->m_pFriendMenu->m_szName);
	pMainMenu->PopAskMsgBox(szTmp,cb_GoGetGiftPow,(void*)0);

}

static void OpenFriendRequest(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pFRequestMenu->Open();
	pMainMenu->m_pFastEquipMenu->m_pFriendRequestTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pFriendRequestTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();
}

static void OpenMentoringAward(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;


	pMainMenu->m_pFastEquipMenu->m_pMentoringAwardTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pMentoringAwardTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();

	pMainMenu->m_pTabSocialityCtrl->Open(2,2);
}

static void cb_ClickDealActEvent(void* pParma, int nVal)
{
	if (nVal)
	{
		ActivityObj* pObj = (ActivityObj*)pParma;
		if (pObj)
		{
			BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ENTERAC,buf,1024);
			SET_DATA(pData,RPGACTIVITY,C_REQUEST_ENTERAC,pPacket);
			pData->nID = pObj->nID;
			pChar->SendPacket(pPacket);
		}
		/*pMainMenu->m_pActivityCtrl->Open(0);
		pMainMenu->m_pActivityMenu->SetFramStepByActID((int)pChar->m_pActivityCenter->m_pWarnActiveList->Items[0]);*/
		//pChar->m_pActivityCenter->m_pWarnActiveList->Delete(0);
		//pChar->m_pActivityCenter->m_bShowActive = true;
	}

	pChar->m_pActivityCenter->m_bShowActive = true;
	pChar->m_pActivityCenter->m_pWarnActiveList->Delete(0);
}

static void OpenNowActiveMenuEvent(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;


	pMainMenu->m_pFastEquipMenu->m_pCurTimeActiveTip->SetVisible(false);
	pMainMenu->m_pFastEquipMenu->m_pCurTimeActiveTip->byRunAction = 16;
	pMainMenu->m_pFastEquipMenu->CheckListFormClose();

	int nActID = (int)pChar->m_pActivityCenter->m_pWarnActiveList->Items[0];
	if (nActID)
	{
		ActivityObj* pObj = GameActivityCfg::GetInstance().GetObj(nActID);
		if (pObj)
		{
			char szTmp[128];
			sprintf(szTmp,"%s活动已经开始啦，快去活动吧！",pObj->pName);
			pMainMenu->PopAskMsgBox(szTmp,cb_ClickDealActEvent,(void*)pObj);
		}
	}
}

static void UserFishItemEvent(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	
	if (pChar->m_pHorseCenter->m_ifRide)
		pChar->m_pHorseCenter->RequestRide(false);

	ItemObj* pItem = pChar->m_pItemCenter->GetItemByTypeID(22000);
	if (pItem)
	{
		pChar->m_pItemCenter->UseItem(pItem);
		pChar->SendCmd(USER, C_REQUEST_FISH);
		pChar->m_bIsFishSkin = true;
	}else
	{
		pMainMenu->PopTipMsg("包裹内缺少钓鱼物品");
		pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->SetVisible(false);
		pMainMenu->m_pFastEquipMenu->m_pCatchFishTip->byRunAction = 16;
		pMainMenu->m_pFastEquipMenu->CheckListFormClose();
	}
}

FastEquipMenu::FastEquipMenu()
{
	m_pTipForm = pMainInterface->GetDForm ("tip");	
	m_pTipForm->bZorde = FORM_ZORDER_NORMAL;
	m_pTipListForm = pMainInterface->GetDForm ("tips");
	m_pTipListForm->bZorde	= FORM_ZORDER_NORMAL;
	m_pListTips		= xnList::Create();

	m_pHomeTip = m_pTipListForm->GetComponent("tip5");
	m_pHomeTip->SetEvent(OpenHomeRequest);
	m_pHomeTip->SetVisible(false);
	m_pHomeTip->byRunAction = 16;

	m_pFightTip = m_pTipListForm->GetComponent("tip7");
	m_pFightTip->SetEvent(AccpetFightRequest);
	m_pFightTip->SetVisible(false);
	m_pFightTip->byRunAction = 16;

	m_pMentoringTip = m_pTipListForm->GetComponent("tip6");
	m_pMentoringTip->SetEvent(OpenMentoringRequest);
	m_pMentoringTip->SetVisible(false);
	m_pMentoringTip->byRunAction = 16;

	m_pTeamInviteTip = m_pTipListForm->GetComponent("dui");
	m_pTeamInviteTip->SetEvent(OpenTeamRequest);
	m_pTeamInviteTip->SetVisible(false);
	m_pTeamInviteTip->byRunAction = 16;

	m_pMarryRingTip = m_pTipListForm->GetComponent("fq");
	m_pMarryRingTip->SetEvent(OpenMarryRingRequest);
	m_pMarryRingTip->SetVisible(false);
	m_pMarryRingTip->byRunAction = 16;

	m_pFriendGiftTip = m_pTipListForm->GetComponent("add");
	m_pFriendGiftTip->SetEvent(OpenGetFriendGift);
	m_pFriendGiftTip->SetVisible(false);
	m_pFriendGiftTip->byRunAction = 16;

	m_pFriendRequestTip = m_pTipListForm->GetComponent("haoyou");
	m_pFriendRequestTip->SetEvent(OpenFriendRequest);
	m_pFriendRequestTip->SetVisible(false);
	m_pFriendRequestTip->byRunAction = 16;

	m_pMentoringAwardTip = m_pTipListForm->GetComponent("shitu");
	m_pMentoringAwardTip->SetEvent(OpenMentoringAward);
	m_pMentoringAwardTip->SetVisible(false);
	m_pMentoringAwardTip->byRunAction = 16;

	m_pCurTimeActiveTip = m_pTipListForm->GetComponent("hd");
	m_pCurTimeActiveTip->SetEvent(OpenNowActiveMenuEvent);
	m_pCurTimeActiveTip->SetVisible(false);
	m_pCurTimeActiveTip->byRunAction = 16;

	pMainInterface->GetDForm ("tip")->GetComponent("icon")->SetEvent(UserFastEquip);

	m_pCatchFishTip = m_pTipListForm->GetComponent("tip8");
	m_pCatchFishTip->SetEvent(UserFishItemEvent);
	m_pCatchFishTip->SetVisible(false);
	m_pCatchFishTip->byRunAction = 16;
}


FastEquipMenu::~FastEquipMenu()
{
	m_pListTips->Free();
}


void FastEquipMenu::OpenTipEquip( ItemObj* pObj )
{	
	m_dwViewIndex	= pObj->GetVal(ITEM_AD_ID);

	DComponent* pName = m_pTipForm->GetComponent("name");

	char szTemp[32];
	sprintf(szTemp,"item/%d.png",pObj->pItemBase->nSpr);	
	pName->SetCaptionEx("%s", 1, pObj->pItemBase->szName);
	pName->SetSprColor(nItemColor[pObj->pItemBase->nStar][0],nItemColor[pObj->pItemBase->nStar][1],nItemColor[pObj->pItemBase->nStar][2]);

	SetItemToDCom(m_pTipForm->GetComponent("icon"),pObj);
	m_pTipForm->Open();

	m_pTipForm->GetComponent("zb")->SetEvent(OnceUseItem,(void*)m_dwViewIndex);
	m_pTipForm->GetComponent("close")->SetEvent(CloseUseItem,(void*)m_dwViewIndex);

}

void FastEquipMenu::ResetTipsEquip()
{
	for (int i = 0; i < 4; i++)
	{
		char	szTmp[32];
		sprintf(szTmp,"tip%d",i+1);
		DComponent* pComp	= 	m_pTipListForm->GetComponent(szTmp);	
		if(i < m_pListTips->Count)
		{
			int nIndex	= (int)m_pListTips->Items[i];
			ItemObj* pItem	= pChar->m_pItemCenter->GetItemByIndex(nIndex);
			if(!pItem)
				continue;							
			SetItemToDCom(pComp,pItem);
			pComp->SetEvent(OpenUseItem,(void*)nIndex);	
		}else
			SetItemToDCom(pComp,NULL);
	}
	if(m_pListTips->Count||m_pHomeTip->bVisable)
		m_pTipListForm->Open();
	else
		CheckListFormClose();
}

void FastEquipMenu::GetEquip( ItemObj* pObj )
{
	if (pObj->pItemBase->nTypeID ==10000||pObj->pItemBase->nTypeID ==11000||pObj->pItemBase->nTypeID ==12000||pObj->pItemBase->nTypeID ==13000)
		return;

	if (pObj->GetVal(ITEM_AD_SLOT) != 0)
		return;

	if(m_pListTips->Count >= 4)
	{
		m_pListTips->Delete(0);
	}
	
	for (int i = 0; i < m_pListTips->Count; i++)
	{
		int nIndex	= (int)m_pListTips->Items[i];
		ItemObj* pOldItem	= pChar->m_pItemCenter->GetItemByIndex(nIndex);

		if (pOldItem && pObj->pItemBase->bySlot == pOldItem->pItemBase->bySlot)
		{
			if ((pObj->GetScore() <= pOldItem->GetScore()))
				return;
			else
			{
				m_pListTips->Delete(i);
				ResetTipsEquip();
			}
		}
	}
	m_pListTips->Add((void*)pObj->GetVal(ITEM_AD_ID));		

	if((pObj->pItemBase->nJob & (1<<pChar->m_byJob)))
	{		
		if (pObj->pItemBase->bySexLimit == 0)
		{
			ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(pObj->pItemBase->bySlot);
			if (pItem)
			{		
				if (pObj->pItemBase->byLv <= pChar->m_byLvl&&pObj->GetScore() > pItem->GetScore())
				{		
					if (m_pListTips->Count != 1)				
						ResetTipsEquip();				
					OpenTipEquip(pObj);	
				}else
					m_pListTips->Delete(m_pListTips->Count-1);
			}else{	
				if (pObj->pItemBase->byLv <= pChar->m_byLvl)
				{
					if (m_pListTips->Count != 1)			
						ResetTipsEquip();			
					OpenTipEquip(pObj);	
				}else
					m_pListTips->Delete(m_pListTips->Count-1);
			}
		}
		if(pObj->pItemBase->bySexLimit == (pChar->m_bySex + 1))
		{
			ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(pObj->pItemBase->bySlot);
			if (pItem)
			{		
				if (pObj->pItemBase->byLv <= pChar->m_byLvl&&pObj->GetScore() > pItem->GetScore())
				{		
					if (m_pListTips->Count != 1)				
						ResetTipsEquip();				
					OpenTipEquip(pObj);	
				}else
					m_pListTips->Delete(m_pListTips->Count-1);
			}else{	
				if (pObj->pItemBase->byLv <= pChar->m_byLvl)
				{
					if (m_pListTips->Count != 1)			
						ResetTipsEquip();			
					OpenTipEquip(pObj);	
				}else
					m_pListTips->Delete(m_pListTips->Count-1);
			}
		}		
	}else
		m_pListTips->Delete(m_pListTips->Count-1);
}

void FastEquipMenu::NewHomeTips()
{
	m_pHomeTip->SetVisible(true);
	m_pTipListForm->Open();
}

void	FastEquipMenu::Update()
{
	CheckMentoringTip();
	CheckFriendTip();
}

void	FastEquipMenu::CheckMentoringTip()
{
	bool ifOK = false;
	if(pChar->m_pMentoringClient->m_ifShow && pChar->m_pMentoringClient->m_pRequestList->Count > 0)
		ifOK = true;
	if(ifOK)
	{
		if(!m_pTipListForm->bOpened)
			m_pTipListForm->Open();
		m_pMentoringTip->SetVisible(true);
	}
	else
		m_pMentoringTip->SetVisible(false);
}

void	FastEquipMenu::CheckFriendTip()
{
	bool ifOK = false;
	if(pMainMenu->m_pFRequestMenu->m_pList->Count > 0)
		ifOK = true;
	if(ifOK)
	{
		if(!m_pTipListForm->bOpened)
			m_pTipListForm->Open();
		m_pFriendRequestTip->SetVisible(true);
	}
	else
		m_pFriendRequestTip->SetVisible(false);
}

void	FastEquipMenu::CheckListFormClose()
{
	if (m_pHomeTip->bVisable)
		return;
	if (m_pFightTip->bVisable)
		return;
	if (m_pMentoringTip->bVisable)
		return;
	if (m_pTeamInviteTip->bVisable)
		return;
	if (m_pMarryRingTip->bVisable)
		return;
	if (m_pFriendGiftTip->bVisable)
		return;
	if (m_pFriendRequestTip->bVisable)
		return;
	if (m_pMentoringAwardTip->bVisable)
		return;
	if (m_pCurTimeActiveTip->bVisable)
		return;
	if (m_pCatchFishTip->bVisable)
		return;

	m_pTipListForm->Close();
}

void FastEquipMenu::UpdateActivityIcon()
{
	if (pChar->m_pActivityCenter->m_pWarnActiveList->Count&&pChar->m_pActivityCenter->m_bShowActive)
	{
		int nCurActID = (int)pChar->m_pActivityCenter->m_pWarnActiveList->Items[0];
		if (nCurActID)
		{
			ActivityObj* pActObj = GameActivityCfg::GetInstance().GetObj(nCurActID);
			if (pActObj && (pChar->m_byLvl >= pActObj->nLvMin || pActObj->nLvMin == 0)
				&& (pChar->m_byLvl <= pActObj->nLvMax || pActObj->nLvMax == 0))
			{
				pChar->m_pActivityCenter->m_bShowActive = false;
				m_pTipListForm->Open();
				m_pCurTimeActiveTip->SetVisible(true);
			}
		}
	}
}

SlaveInfoMenu::SlaveInfoMenu()
{
	m_pForm = pMainInterface->GetDForm("zb3");
}

SlaveInfoMenu::~SlaveInfoMenu()
{

}

void SlaveInfoMenu::OpenByID(int nSID)
{
	SlaveBasic* pBase = SlaveBaseCfg::GetInstance().GetBaseInfo(nSID);
	if (!pBase)
		return;

	char szIcon[32];
	sprintf(szIcon,"");

	ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pBase->nSkin);
	if(pSkin)
		sprintf(szIcon,"head/%d.png",pSkin->nHead);

	m_pForm->GetComponent("ico")->SetViewSpr(szIcon);
	char szTmp[32];
	sprintf(szTmp,"MB_PZ%d.png",pBase->byQuality+1);
	m_pForm->GetComponent("ico")->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);

	m_pForm->GetComponent("name")->SetCaptionEx("%s",0,pBase->pName);
	m_pForm->GetComponent("name")->SetSprColor(nItemColor[pBase->byQuality][0],nItemColor[pBase->byQuality][1],nItemColor[pBase->byQuality][2]);
	m_pForm->GetComponent("pz")->SetCaptionEx("%s",1,s_qualityName[pBase->byQuality]);
	m_pForm->GetComponent("lv")->SetCaptionEx("%d",1,1);
	m_pForm->GetComponent("sm")->SetCaptionEx("%d",1,250);
	m_pForm->GetComponent("hp")->SetCaptionEx("%d",1,pBase->szBaseParam[EQUIP_ATTR_HP]);
	m_pForm->GetComponent("mp")->SetCaptionEx("%d",1,pBase->szBaseParam[EQUIP_ATTR_MP]);
	m_pForm->GetComponent("wg")->SetCaptionEx("%d",1,pBase->szBaseParam[EQUIP_ATTR_ATK_W]);
	m_pForm->GetComponent("mg")->SetCaptionEx("%d",1,pBase->szBaseParam[EQUIP_ATTR_ATK_N]);
	m_pForm->GetComponent("wf")->SetCaptionEx("%d",1,pBase->szBaseParam[EQUIP_ATTR_DEF_W]);
	m_pForm->GetComponent("mf")->SetCaptionEx("%d",1,pBase->szBaseParam[EQUIP_ATTR_DEF_N]);

	m_pForm->Open();
}