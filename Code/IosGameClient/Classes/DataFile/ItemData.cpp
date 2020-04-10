#include "ItemData.h"
#include "UserData.h"
#include "MainMenu.h"
#include "ios_item_packet.h"
#include "MapControl.h"
#include "MapStaticCfg.h"
#include "MapMouse.h"

void ClickShowCompEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nType);
	if(pBase != NULL)
	{
		if(pBase->byType == ITEMTYPE_EQUIP)
			pMainMenu->m_pEquipInfoMenu->Open(pBase,FORM_NULL);
		else
			pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_NULL);
	}
}

void ShowItemInfoOnComp(DComponent* pComp,int nType,int nNum /* = 1 */)
{
	if(pComp == NULL)
		return;
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nType);
	if(pBase == NULL)
	{
		pComp->ClearEffect();
		pComp->SetViewSpr("");
		pComp->SetCaption("",1);
		return;
	}
	char szIcon[32];
	sprintf(szIcon,"item/%d.png",pBase->nSpr);
	pComp->SetViewSpr(szIcon);
	sprintf(szIcon,"MB_PZ%d.png",pBase->nStar+1);
	pComp->SetAddSprByTag(szIcon,-3,-3,1,100,0,-1);
	pComp->SetCaptionEx("%d",2,nNum);
	pComp->SetEffect(0,1);
	if (pBase->nStar == 2)
	{
		pComp->SetEffect(174,1);
	}else if (pBase->nStar == 3)
	{
		pComp->SetEffect(175,1);
	}
	pComp->SetEvent(ClickShowCompEvent,(void*)nType);
}

void	SetBagItem(DComponent**pCom,int nCount,xnList* pList,void (*EventFunc)(InterfaceEvent * pEvent,void * param),int nStartIndex = 0){
	int nComCount = nCount;
	int nDrawCount = (pList->Count-nStartIndex)>nComCount?nComCount:(pList->Count-nStartIndex);
	char sztxt[32];
	for (int i=nStartIndex;i<nCount;i++)
	{
		ItemObj* pObj = (ItemObj*)pList->Items[i];
		pCom[i-nStartIndex]->SetTag(pObj);
		sprintf(sztxt,"");
		pCom[i-nStartIndex]->SetViewSpr(sztxt);
		pCom[i-nStartIndex]->SetEvent(EventFunc,NULL);
	}
}
ItemObj* ItemObj::Create( AmzDataBase* pData )
{
	ItemExBase*		pIBCfg	= ItemConfig::GetInstance ().GetItemBase ((*pData)[ITEM_AD_TYPE]);
	if(pIBCfg == NULL)
	{
		pData->bStateFlag	= AMZ_STATE_RELEASE;
		return NULL;
	}
	ItemObj* pItem	= new ItemObj(pData);

	pItem->pItemBase		= pIBCfg;

	return pItem;
}

ItemObj::ItemObj( AmzDataBase* pData )
{
	pAmzData	= pData;
	pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
	for (int i=0;i<ITEMEXDATA_MAX;i++)
	{
		m_pDataEx[i] = NULL;
	}
	memset(m_nParam,0,sizeof(int)*EQUIP_ATTR_MAX);
}

ItemObj::~ItemObj()
{
	pChar->m_pDataCenter->RemoveAmzData (pAmzData->bModelType,pAmzData->nModelID,true);
	if(this == EquipStarMenu::sLvUpEquipObj)
		EquipStarMenu::sLvUpEquipObj	= NULL;
}

bool ItemObj::Update()
{
	if(pAmzData->bStateFlag == AMZ_STATE_DELETE)
	{
		return false;
	}

	if(pAmzData->bStateFlag	== AMZ_STATE_CHGED)
	{
		if((*pAmzData)[ITEM_AD_TYPE] != pItemBase->nTypeID)
			pItemBase	= ItemConfig::GetInstance ().GetItemBase  ((*pAmzData)[ITEM_AD_TYPE]);

		if(pItemBase == NULL)
			return false;

		//RecalcParam ();

		pAmzData->bStateFlag	= AMZ_STATE_NORMAL;
		pChar->m_pItemCenter->_updateMenu();

	}
	if (m_pDataEx[ITEMEXDATA_EQUIPEX]&&m_pDataEx[ITEMEXDATA_EQUIPEX]->bStateFlag	== AMZ_STATE_CHGED)
	{
		m_pDataEx[ITEMEXDATA_EQUIPEX]->bStateFlag = AMZ_STATE_NORMAL;
		RecalcParam();
		pChar->m_pItemCenter->_updateMenu();

	}
	return true;
}

int ItemObj::GetVal( BYTE bEnum )
{
	if(bEnum >= ITEM_AD_MAX)
		return 0;
	return (*pAmzData)[bEnum];
}

void ItemObj::RecalcParam()
{
	if (pItemBase->byType != ITEMTYPE_EQUIP)
	{
		return;
	}
	memset(m_nParam,0,sizeof(int)*EQUIP_ATTR_MAX);
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		m_nParam[i] += pItemBase->nParam[i];
	}
	if (m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		int nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE1);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM1),m_nParam);
		}
		nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE2);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM2),m_nParam);
		}
	}
	int nPer = GetStarPer(GetStarLv());
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		m_nParam[i] = m_nParam[i]*(10000+nPer)/10000;
	}


}

void ItemObj::_recalcAttr( int nType,int nVal ,int* nParam)
{
	EquipAttrBase* pBase = EquipAttrConfig::GetInstance().GetEquipAttr(nType);
	if (pBase)
	{
		int nVal1 = nVal<<16>>16;
		int nVal2 = nVal>>16;
		bool bFlag = true;
		for (int i=0;i<EQUIP_ATTR_MAX;i++)
		{
			if (pBase->nBase[i]&&bFlag)
			{
				nParam[i] += pBase->nBase[i];
				nParam[i] += nVal1;
				bFlag = false;
			}else if (pBase->nBase[i]&&!bFlag)
			{
				nParam[i] += pBase->nBase[i];
				nParam[i] += nVal2;
				break;
			}
		}
	}
}

bool ItemObj::SetExData( int nType,AmzDataBase* pData )
{
	switch(nType){
	case ITEMEXDATA_EQUIPEX:
		{
			if(pItemBase->byType != ITEMTYPE_EQUIP) return false;
			m_pDataEx[ITEMEXDATA_EQUIPEX] = pData;
			RecalcParam();
			return true;
		}
	}
	return false;
}

void ItemObj::GetBaseData( int* nBase )
{
	memset(nBase,0,sizeof(int)*EQUIP_ATTR_MAX);
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		nBase[i] += pItemBase->nParam[i];
	}
	if (m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		int nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE1);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM1),nBase);
		}
		nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE2);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM2),nBase);
		}
	}
}

int ItemObj::GetStarPer( int nLv )
{
	int nExpos = GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV)>>8;
	EquipStarBase* pBase = EquipStarConfig::GetInstance().GetEquipStar(nLv);
	int nPer = 0;
	if (pBase)
	{
		nPer = pBase->nPer;
		for (int i=0;i<nEquipStarExCount;i++)
		{
			if(nExpos&(1<<i)) nPer += EquipStarConfig::GetInstance().aExPer[i];
		}
	}
	return nPer;
}

int ItemObj::GetExData( int nType,BYTE bEnum )
{
	if (!m_pDataEx[nType])
	{
		return 0;
	}
	return m_pDataEx[nType]->GetVal(bEnum);
}

int ItemObj::GetStarLv()
{
	return GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV)<<24>>24;
}

int ItemObj::GetStarLvEx()
{
	int n = 0x01<<8;
	return GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV)&n;
}

ItemExBase* ItemObj::GetStoneByPos( int nPos )
{
	if (nPos>=nMaxStoneCount)
	{
		return NULL;
	}
	int nID = 0;
	if (nPos /2 == 0)
	{
		if(nPos%2) nID = GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE12)>>16;
		else nID = GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE12)<<16>>16;
	}else if (nPos/2 == 1)
	{
		if(nPos%2) nID = GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE34)>>16;
		else nID = GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE34)<<16>>16;
	}
	if (nID)
	{
		return ItemConfig::GetInstance().GetItemBase(nID);
	}
	return NULL;
}

int ItemObj::GetScore()
{
	int nScore = 0;
	static int nAttrScore[EQUIP_ATTR_MAX] = {4,6,12,12,15,15,20,30,40};
	int nParamTemp[EQUIP_ATTR_MAX] = {0};
	GetAllParam(nParamTemp);
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		//nScore += m_nParam[i]*nAttrScore[i];
		nScore += nParamTemp[i]*nAttrScore[i];
	}
	int nJob = 0;
	for(int i = 1; i < 5; i ++)
	{
		if(pItemBase->nJob & (1<<i))
		{
			nJob = i;
			break;
		}
	}
	switch(nJob)
	{
	case 1:
		return nScore/30.5;
	case 2:
		return nScore/29.2;
	case 3:
		return nScore/34.4;
	case 4:
		return nScore/28.5;
	}
	return nScore/30.5;
}

int ItemObj::GetTransferCost()
{
	int nCost = 0;
	int nLv = GetStarLv();
	if (nLv<=5)
	{
		//nCost += 1;
	}else if (nLv <= 8)
	{
		nCost += 1;
	}else if (nLv <= 11)
	{
		nCost +=2;
	}else if (nLv <= 12)
	{
		nCost +=4;
	}else if (nLv <= 13)
	{
		nCost +=6;
	}else if (nLv <= 14)
	{
		nCost +=8;
	}else if (nLv <= 15)
	{
		nCost +=10;
	}else if (nLv <= 16)
	{
		nCost +=15;
	}else{
		nCost += 20;
	}
	for (int i=0;i<nMaxStoneCount;i++)
	{
		int nStoneLv = GetStoneByPos(i)?GetStoneByPos(i)->nTypeID:0;
		if (!nStoneLv)
		{
			continue;
		}
		nStoneLv = nStoneLv%10;
		if (nStoneLv<=0)
		{
		}else if (nStoneLv<=1)
		{
			nCost+=1;
		}else if (nStoneLv<=2)
		{
			nCost+=2;
		}else if (nStoneLv<=5)
		{
			nCost+=3;
		}else if (nStoneLv<=8)
		{
			nCost+=4;
		}else
		{
			nCost+=5;
		}
	}
	int nEatLv = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_EATLV)<<24>>24;
	if (nEatLv<=0)
	{
		//nCost += 1;
	}else if (nEatLv <= 10)
	{
		nCost += 1;
	}else if (nLv <= 20)
	{
		nCost +=2;
	}else if (nLv <= 30)
	{
		nCost +=5;
	}else if (nLv <= 40)
	{
		nCost +=10;
	}else
	{
		nCost +=20;
	}
	return nCost;
}

bool ItemObj::CheckAddPer()
{
	int nLv = GetStarLv();
	if (nLv)
	{
		return true;
	}
	int nEatLv = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_EATLV)<<24>>24;
	if (nEatLv)
	{
		return true;
	}
	for (int i=0;i<nMaxStoneCount;i++)
	{
		if (GetStoneByPos(i))
		{
			return true;
		}	
	}
	return false;
}

bool ItemObj::IsEquip()
{
	return GetVal(ITEM_AD_SLOT)>ITEM_SLOT_EQUIP_START&&GetVal(ITEM_AD_SLOT)<ITEM_SLOT_EQUIP_END;
}

void ItemObj::GetAllParam( int* nBase )
{
	memset(nBase,0,sizeof(int)*EQUIP_ATTR_MAX);
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		nBase[i] += pItemBase->nParam[i];
	}
	if (m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		int nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE1);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM1),nBase);
		}
		nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE2);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM2),nBase);
		}
		int nLv = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_LV);
		if (nLv)
		{
			int nStar = nLv<<24>>24;
			int nExStar = nLv>>8;
			EquipStarBase* pBase = EquipStarConfig::GetInstance().GetEquipStar(nStar);
			if (pBase)
			{
				int nPer = pBase->nPer;
				for (int i=0;i<nEquipStarExCount;i++)
				{
					if(nExStar&(1<<i)) nPer += EquipStarConfig::GetInstance().aExPer[i];
				}
				for (int i=0;i<EQUIP_ATTR_MAX;i++)
				{
					nBase[i] *= (10000+pBase->nPer);
					nBase[i] /=10000;
				}
			}
		}
		for (int i=0;i<nMaxStoneCount;i++)
		{
			ItemExBase* pStone = GetStoneByPos(i);
			if (pStone)
			{
				for (int j=0;j<EQUIP_ATTR_MAX;j++)
				{
					nBase[j] += pStone->nParam[j];
				}
			}
		}
		int nEatLv = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_EATLV)<<24>>24;
		if (nEatLv)
		{
			EquipEatBase* pBase = EquipEatConfig::GetInstance().GetEquipEat(nEatLv);
			int nType = EquipEatConfig::GetInstance().GetAddParam(pItemBase->bySlot);
			nBase[nType?EQUIP_ATTR_DEF_W:EQUIP_ATTR_ATK_W] += nType?pBase->nDefense:pBase->nDamage;
			nBase[nType?EQUIP_ATTR_DEF_N:EQUIP_ATTR_ATK_N] += nType?pBase->nDefense:pBase->nDamage;
		}
	}
}

ItemCenter::ItemCenter( AmzDataManager* pDC )
{
	m_pListItems		= xnList::Create ();
	pDataCenter		= pDC;
	m_bChgNumber	= false;
	m_pListTempItems	= xnList::Create();
	m_pListCallback = xnList::Create();
	m_pMallList = xnList::Create();
	m_nBagSize = 40;
	m_nBagCurCount	 = 0;
	//m_bIsOkBuyBook   = false;
	//m_bBuyBook = false;
	//m_nLifeBookID	 = 0;
	m_ifNeedFix = false;
}

ItemCenter::~ItemCenter()
{
	while(m_pListItems->Count)
		delete (ItemObj*)m_pListItems->Delete (0);
	m_pListItems->Free ();
	while(m_pListTempItems->Count)
		delete (ItemObj*)m_pListTempItems->Delete (0);
	m_pListTempItems->Free ();

	while(m_pListCallback->Count)
		delete (_itemInfoCallback*)m_pListCallback->Delete (0);
	while(m_pMallList->Count)
		delete (MallItemCfg*)m_pMallList->Delete (0);
	m_pListCallback->Free ();
}

void ItemCenter::Update()
{
	//检测船只数据,刷新
	for (int i = m_pListItems->Count-1; i >= 0; i--)
	{
		ItemObj* pItem	= (ItemObj*)m_pListItems->Items[i];
		if(pItem->Update () == false)
		{
			if (pItem->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_DEPOT)
			{
				pMainMenu->m_pBankMenu->m_pBankEx->m_bBankUpdate = true;
			}else
			{
				pMainMenu->m_pBagMenu->m_bUpdate = true;
				pMainMenu->m_pBankMenu->m_bBagUpdate = true;
				pMainMenu->m_pChatMenu->m_pItemMenu->m_bUpdate = true;
			}
			_updateMenu();
			OnItemRemove(pItem);
			delete pItem;
			m_pListItems->Delete (i);
		}
	}

	//新增船只处理
	AmzDataBase* pAD	= pDataCenter->PopAmzData (AMZ_DATA_ITEM);
	while(pAD)
	{
		ItemObj* pItem	= ItemObj::Create(pAD);
		if(pItem)
		{
			//pMainToopTip->AddGetTipObj(pItem->pItemBase->szName,pItem->pItemBase->szDesc,pItem->pItemBase->nSpr,pItem->GetVal(ITEM_AD_NUM),3);
			m_pListItems->Add (pItem);
			_updateMenu();
		}
		pAD		= pDataCenter->PopAmzData (AMZ_DATA_ITEM);
	}
	m_ifNeedFix = false;
	for(int i = 0; i < EQUIP_MAX-1; i ++)
	{
		ItemObj* pItem = GetEquipBySlot(i);
		if(pItem && pItem->GetVal(ITEM_AD_NUM) == 0)
		{
			m_ifNeedFix = true;
			break;
		}
	}
	//GetBagCurCount();
}

bool ItemCenter::GetItemByID( int nID,std::list<ItemObj*>& itemlist )
{
	for (int i = 0; i < m_pListItems->Count; i++)
	{
		ItemObj* pItem	= (ItemObj*)m_pListItems->Items[i];
		if(pItem->GetVal(ITEM_AD_TYPE) == nID)
			//return pItem;
			itemlist.push_back(pItem);
	}
	return true;
}

bool ItemCenter::PacketProcess( ZPacket* pPacket )
{
	if (pPacket->bCmdGroup != ITEM)
	{
		return false;
	}
	switch(pPacket->bCmd){
	case C_ITEM_TRANSFER_EQUIP:
		{
			EquipStarMenu::sLvUpEquipObj = pMainMenu->m_pEquipTransferMenu->m_pEquip;
		}
		return true;
	case S_SEND_BAGSIZE:
		{
			SET_DATA(pData,ITEM,S_SEND_BAGSIZE,pPacket);
			m_nBagSize = pData->nBagsize;
			_updateMenu();
		}
		return true;
	case S_SEND_ITEMINFO_SERVER:
		{
			SET_DATA(pData,ITEM,S_SEND_ITEMINFO_SERVER,pPacket);
			ItemObj* pObj = CreateItemByData((char*)pData->byData,pData->nSize);
			if (!pObj)
			{
				return true;
			}
			AddTempItem(pObj);
			_itemInfoCallback* pCallback = _getItemInfoCallback(pData->nKey);
			if (pCallback)
			{
				pCallback->callBack(pObj);
				delete pCallback;
			}
			
		}
		return true;
	case S_SEND_BLANKBAGRLT:
		{
			pMainMenu->PopTipMsg("背包空间不足！");
		}
		return true;
	case C_LEARN_MAKE_SKILL:
		{
			SET_DATA(pData,ITEM,C_LEARN_MAKE_SKILL,pPacket);
			pMainMenu->m_pMakeSkillMenu->OpenMenu(pData->byResult);
		}
		return true;
	case S_SEND_MALL_ITEM:
		{
			SET_DATA(pData,ITEM,S_SEND_MALL_ITEM,pPacket);
			while(m_pMallList->Count) delete (MallItemCfg*)m_pMallList->Delete(0);
			for(int i=0;i<pData->nCnt;i++){
				MallItemCfg* pCfg = new MallItemCfg();
				memcpy(pCfg,&pData->xData[i],sizeof(MallItemCfg));
				m_pMallList->Add(pCfg);
			}
			pMainMenu->m_pMallMenu->m_bMallUpdate = true;
			pMainMenu->m_pMallMenu->m_pMallEx->m_bMallExUpdate = true;
		}
		return true;
	case C_MAKE_SKILL_ITEM:
		{
			pMainMenu->m_pMakeSkillMenu->StartMake(pMainMenu->m_pMakeSkillMenu->m_pNowTag);
		}
		return true;
	}
	return false;
}
void	fun_cbCastItemUse(void* param)
{
	pChar->m_pItemCenter->UseItem((ItemObj*)param,false);
}

void   cb_ClickCheckUseCake(void* pParma, int nVal)
{
	ItemObj* pObj = (ItemObj*)pParma;
	if (nVal)
	{
		BuildPacketEx(pPacket, ITEM, C_USER_ITEM, buffer,1024);
		SET_DATA(pdata, ITEM, C_USER_ITEM, pPacket);

		pdata->dwID	= pObj->GetVal(ITEM_AD_ID);
		pChar->SendPacket(pPacket);
	}
}

static int nCakeCost[] = {20000,40000,70000,110000,160000,220000,290000,370000,460000,560000};

bool ItemCenter::UseItem( ItemObj* pObj,bool bCast )
{
	if(pChar->m_szbyActLock[5] || pChar->m_szbyActLock[0])
	{
		pMainMenu->PopTipMsg("当前状态不能使用物品");
		return false;
	}

	if (!pObj)
	{
		return false;
	}
	if (pObj->pItemBase->byType == ITEMTYPE_ITEMCANNTUSE)
	{
		return false;
	}

	if (pObj->pItemBase->nTypeID == 45016)
	{//改名卡
		pMainMenu->m_pChangeNameF->Open();
		pMainMenu->m_pBagMenu->Close();
		return false;
	}

	if ((pObj->pItemBase->nUseType == 4)&&(pChar->m_dwPow >= 50))
	{
		pMainMenu->PopTipMsg("体力值已满，请稍后使用");
		return false;
	}
	if (!(pObj->pItemBase->nJob&(1<<pChar->m_byJob)))
	{
		pMainMenu->PopTipMsg("职业不符合");
		return false;
	}
	if (pObj->pItemBase->bySexLimit&&pObj->pItemBase->bySexLimit !=(pChar->m_bySex+1))
	{
		pMainMenu->PopTipMsg("性别不符合");
		return false;
	}
	if (pObj->pItemBase->byLv>pChar->m_byLvl)
	{
		pMainMenu->PopTipMsg("等级不符合");
		return false;
	}

	if(pObj->pItemBase->nTypeID == 60022)
	{//甜蜜活动蛋糕
		int nCakeFlag = pChar->m_pClientFlag->GetFlag(63);
		if (nCakeFlag > 19)
		{
			pMainMenu->PopTipMsg("每日只可开启20个");
			return false;
		}
		if (nCakeFlag > 9 && pChar->m_dwMoney > nCakeCost[nCakeFlag-10])
		{
			char szTmp[32];
			sprintf(szTmp,"是否花费%d开启蛋糕？",nCakeCost[nCakeFlag-10]);
			pMainMenu->PopAskMsgBox(szTmp,cb_ClickCheckUseCake,(void*)pObj);
			return true;
		}else if (nCakeFlag > 9)
		{
			pMainMenu->PopTipMsg("金币不足！");
			return false;
		}
	}

	if (bCast&&pObj->pItemBase->nCast)
	{
		if(pObj->pItemBase->nTypeID == 30009 )
			pMainMenu->StartCast(pObj->pItemBase->nCast,"正在回城中．．．",fun_cbCastItemUse,pObj);
		else if(pObj->pItemBase->nTypeID>30020&&pObj->pItemBase->nTypeID<30051)
			pMainMenu->StartCast(pObj->pItemBase->nCast,"正在传送中．．．",fun_cbCastItemUse,pObj);
		else
			pMainMenu->StartCast(pObj->pItemBase->nCast,"正在使用中．．．",fun_cbCastItemUse,pObj);
		return true;
	}

	if (!JudgeChangeCountryItem(pObj->pItemBase->nTypeID))
		return false;

	BuildPacketEx(pPacket, ITEM, C_USER_ITEM, buffer,1024);
	SET_DATA(pdata, ITEM, C_USER_ITEM, pPacket);


	pdata->dwID	= pObj->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);

	if (pObj->pItemBase->nTypeID == 30009&&pMainMenu->m_pBagMenu->m_pForm->bOpened)
		pMainMenu->m_pBagMenu->Close();

	return true;
}
bool ItemCenter::CheckEquip( ItemObj* pObj )
{
	int nSite = pObj->GetVal(ITEM_AD_SLOT);
	return nSite<ITEM_SLOT_EQUIP_END&&nSite>ITEM_SLOT_EQUIP_START;
}
static int ItemSortByPos(const void * Item1, const void * Item2)
{
	ItemObj * pData1 = *(ItemObj **)Item1;
	ItemObj * pData2 = *(ItemObj **)Item2;
	// 	if (pData1->GetVal(EQUIP_AD_SLOT)&&!pData2->GetVal(EQUIP_AD_SLOT))
	// 	{
	// 		return -100;
	// 	}else if (!pData1->GetVal(EQUIP_AD_SLOT)&&pData2->GetVal(EQUIP_AD_SLOT))
	// 	{
	// 		return 100;
	// 	}
	return pData2->GetVal(ITEM_AD_POS)-pData1->GetVal(ITEM_AD_POS);
	//return (pData1->dwUin - pData2->dwUin);
}
void ItemCenter::GetItemInBagByPos( xnList* pList )
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG)
		{
			pList->Add(pObj);
		}

	}
	pList->Sort(ItemSortByPos);
}
void ItemCenter::GetItemInBankByPos( xnList* pList )
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_DEPOT)
		{
			pList->Add(pObj);
		}

	}
	pList->Sort(ItemSortByPos);
}

void ItemCenter::GetItemsByKind(int nType,xnList* pList)
{
	if(pList == NULL)
		pList = xnList::Create();
	while(pList->Count)
		pList->Delete(0);
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG && pObj->pItemBase->byType == nType)
		{
			pList->Add(pObj);
		}
	}
}

	ItemObj* ItemCenter::GetEquipBySlot( int nSlot )
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (pObj->GetVal(ITEM_AD_SLOT) == EQUIP_INDEX_TO_SLOT(nSlot))
		{
			return pObj;
		}
	}
	return NULL;
}

bool ItemCenter::DestroyItem( ItemObj* pObj )
{
	if (!pObj)
	{
		return false;
	}
	if (!(pObj->pItemBase->nFlag&1<<ITEM_FLAG_DESTROY))
	{
		return false;
	}
	// 	int nSlot = pObj->pItemBase->bySlot;
	// 	if (nSlot>=EQUIP_MAX)
	// 	{
	// 		return false;
	// 	}
	BuildPacketEx(pPacket, ITEM, C_DESTROY_ITEM, buffer,1024);
	SET_DATA(pdata, ITEM, C_DESTROY_ITEM, pPacket);


	pdata->dwID	= pObj->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

bool ItemCenter::SetItemBank( ItemObj* pObj )
{
	if (!pObj)
	{
		return false;
	}
	if (!(pObj->pItemBase->nFlag&1<<ITEM_FLAG_BANK))
	{
		pMainMenu->PopTipMsg("该物品无法存入仓库");
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_BANK, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_BANK, pPacket);


	pdata->dwID	= pObj->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

int ItemCenter::GetItemCountByID( int nID,int nSlot )
{
	int nCount = 0;
	for (int i = 0; i < m_pListItems->Count; i++)
	{
		ItemObj* pItem	= (ItemObj*)m_pListItems->Items[i];
		int nSlot2 = pItem->GetVal(ITEM_AD_SLOT);
		if(pItem->GetVal(ITEM_AD_TYPE) == nID&& ((1<<nSlot2)&nSlot))
			nCount += pItem->GetVal(ITEM_AD_NUM);
	}
	return nCount;
}

ItemObj* ItemCenter::GetItemByIndex( int nID )
{
	for (int i = 0; i < m_pListItems->Count; i++)
	{
		ItemObj* pItem	= (ItemObj*)m_pListItems->Items[i];
		if(pItem->GetVal(ITEM_AD_ID) == nID)
			return pItem;
	}
	return NULL;
}

void GoMallAndFubenEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pMapCopyMenu->OpenForm();
}

void GoMallAndHuodongEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pMallMenu->Open();
	}else
		pMainMenu->m_pActivityCtrl->Open(0);
}

bool ItemCenter::EquipStarUp( ItemObj* pObj )
{
	if (!pObj||pObj->pItemBase->byType!=ITEMTYPE_EQUIP)
	{
		pMainMenu->PopTipMsg("该物品无法炼星");
		return false;
	}
	if (GetItemCountByID(EquipStarConfig::GetInstance().nItemID)<EquipStarConfig::GetInstance().nItemCount)
	{
		pMainMenu->PopAskMsgBox("炼星石不足，可通过“商城”购买或者参加活动“副本悬赏”获得。",GoMallAndHuodongEvent,(void*)0, 5);		
		return false;
	}
	if (pObj->GetStarLv() >= EquipStarConfig::GetInstance().GetMaxLv())
	{
		pMainMenu->PopTipMsg("该物品炼星已到最大值");
		return false;
	}
	if (!pObj->pItemBase->GetItemFlag(ITEM_FLAG_LVUP))
	{
		pMainMenu->PopTipMsg("该物品无法炼星");
		return false;
	}
	EquipStarBase* pBase = EquipStarConfig::GetInstance().GetEquipStar(pObj->GetStarLv()+1);

	if (pBase&&pChar->m_dwMoney<pBase->nGold)
	{
		pMainMenu->PopTipMsg("所需金币不足");
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_STARUP, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_STARUP, pPacket);


	pdata->dwID	= pObj->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

void ItemCenter::_updateMenu()
{
	pMainMenu->m_pBankMenu->m_pBankEx->m_bBankUpdate = true;
	pMainMenu->m_pBankMenu->m_bBagUpdate = true;
	pMainMenu->m_pBagMenu->m_bUpdate = true;
	pMainMenu->m_pChatMenu->m_pItemMenu->m_bUpdate = true;
	pMainMenu->m_pEquipStarMenu->m_bUpdate = true;
	pMainMenu->m_pEquipBoreMenu->m_bUpdate = true;
	pMainMenu->m_pEquipInlayMenu->m_bUpdate = true;
	pMainMenu->m_pEquipEatMenu->m_bUpdate = true;
	pMainMenu->m_pEquipTransferMenu->m_bUpdate = true;
	pMainMenu->m_pStoneComposeMenu->m_bUpdate = true;
	pMainMenu->m_pShopMenu->m_bShopUpdate = true;
	pMainMenu->m_pShopMenu->m_pShopEx->m_bShopExUpdate = true;
	pMainMenu->m_pUserParamMenu->m_bUpdate = true;
	pMainMenu->m_pMallMenu->m_bMallUpdate = true;
	pMainMenu->m_pMallMenu->m_pMallEx->m_bMallExUpdate = true;
	pMainMenu->m_pMailMenu->m_bWriteUpdate = true;
}

bool ItemCenter::EquipBore( ItemObj* pObj )
{
	if (!pObj||pObj->pItemBase->byType!=ITEMTYPE_EQUIP)
	{
		pMainMenu->PopTipMsg("该物品无法打孔");
		return false;
	}
	int nPos = pObj->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
	int nNowPos = 0;
	for (;nNowPos<(sizeof(int)*8);nNowPos++)
	{
		if (!(nPos&(1<<nNowPos))) break;
	}
	if (nNowPos>=nMaxStoneCount)
	{
		pMainMenu->PopTipMsg("该物品已到最大孔数");
		return false;
	}
	if (!pObj->pItemBase->GetItemFlag(ITEM_FLAG_BORE))
	{
		pMainMenu->PopTipMsg("该物品无法打孔");
		return false;
	}
	if (pChar->m_dwMoney<=nItemBoreCostMoney[nNowPos])
	{
		pMainMenu->PopTipMsg("该物品所需金币不足");
		return false;
	}
	if (GetItemCountByID(nStoneCostItem)<nItemBoreCostCount[nNowPos])
	{
		pMainMenu->PopAskMsgBox("打孔石不足，可通过“商城”购买或者“副本”通关获得。",GoMallAndFubenEvent,(void*)0, 6);			
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_BORE, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_BORE, pPacket);


	pdata->dwID	= pObj->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

void ItemCenter::GetItemInBagByType( xnList* pList ,int nType)
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (pObj->GetVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG&&pObj->pItemBase->byType == nType)
		{
			pList->Add(pObj);
		}

	}
	pList->Sort(ItemSortByPos);
}

bool ItemCenter::EquipInlay( ItemObj* pEquip,ItemObj* pStone )
{
	if (!pEquip||!pStone||pEquip->pItemBase->byType!=ITEMTYPE_EQUIP||pStone->pItemBase->byType!=ITEMTYPE_STONE)
	{
		return false;
	}
	int nPos = pEquip->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
	int nNowPos = 0;
	for (;nNowPos<(sizeof(int)*8);nNowPos++)
	{
		if ( (nPos&(1<<nNowPos)) && !pEquip->GetStoneByPos(nNowPos)) break;
	}
	if (nNowPos>=nMaxStoneCount)
	{
		pMainMenu->PopTipMsg("该物品已经无法镶嵌");
		return false;
	}
	if (!pEquip->pItemBase->GetItemFlag(ITEM_FLAG_INLAY))
	{
		pMainMenu->PopTipMsg("该物品无法镶嵌");
		return false;
	}

	BuildPacketEx(pPacket, ITEM, C_ITEM_INLAY, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_INLAY, pPacket);


	pdata->dwStone	= pStone->GetVal(ITEM_AD_ID);
	pdata->dwEquip	= pEquip->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

bool ItemCenter::EquipRemoveStone( ItemObj* pEquip,int nPos )
{
	if (!pEquip||pEquip->pItemBase->byType!=ITEMTYPE_EQUIP)
	{
		return false;
	}

	if (!pEquip->GetStoneByPos(nPos)){
		return false;
	}
	if (nPos>=nMaxStoneCount)
	{
		return false;
	}

	BuildPacketEx(pPacket, ITEM, C_ITEM_REMOVE_STONE, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_REMOVE_STONE, pPacket);

	pdata->dwEquip	= pEquip->GetVal(ITEM_AD_ID);
	pdata->nPos = nPos;
	SendPacket(pPacket);
	return true;
}

bool ItemCenter::ComposeStone( int nID,int nCount )
{
	if (pChar->m_dwMoney<nCount*nStoneComposeMoney)
	{
		pMainMenu->PopTipMsg("所需金币不足");
		return false;
	}
	if (GetItemCountByID(nID)<nCount*nStoneComposeCount || nCount == 0)
	{
		pMainMenu->PopAskMsgBox("宝石数量不足,可通过“商城”购买或参与“日常”活动获得",GoMallAndHuodongEvent,(void*)0, 5);	
		return false;
	}
	pMainMenu->PlayEffect(28);
	pMainMenu->PopTipMsg("        合成宝石成功！");

	BuildPacketEx(pPacket, ITEM, C_ITEM_COMPOSE_STONE, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_COMPOSE_STONE, pPacket);

	pdata->nID	= nID;
	pdata->nPos = nCount;
	SendPacket(pPacket);
	return true;
}

bool ItemCenter::EquipEat( ItemObj* pEquip,ItemObj* pEat )
{
	if (!pEquip||!pEat)
	{
		pMainMenu->PopTipMsg("物品失效");
		return false;
	}
	if (!pEat->pItemBase->nStar)
	{
		pMainMenu->PopTipMsg("该物品无法精炼");
		return false;
	}
	if (pEquip->pItemBase->byType != ITEMTYPE_EQUIP||pEat->pItemBase->byType != ITEMTYPE_EQUIP)
	{
		pMainMenu->PopTipMsg("该物品无法精炼");
		return false;
	}
	int nVal = pEquip->GetExData(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV);
	int nLv = nVal<<24>>24;
	int nExp = nVal>>8;
	EquipEatBase* pBase = EquipEatConfig::GetInstance().GetEquipEat(nLv+1);
	if (!pBase)
	{
		pMainMenu->PopTipMsg("该物品已经精炼到最大值");
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_EAT_EQUIP, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_EAT_EQUIP, pPacket);

	pdata->nEquip	= pEquip->GetVal(ITEM_AD_ID);
	pdata->nEat = pEat->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

static void cb_ClickGoToMall(void* pParam, int nVal)
{
	if (nVal)
	{
		pMainMenu->m_pMallMenu->Open();
	}
}

bool ItemCenter::EquipTransfer( ItemObj* pEquip,ItemObj* pEat )
{
	if (!pEquip||!pEat)
	{
		return false;
	}
	if (pEquip->pItemBase->byType != ITEMTYPE_EQUIP||pEat->pItemBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	if (pChar->m_dwMoney<nTransferMoney)
	{
		pMainMenu->PopTipMsg("所需金币不足");
		return false;
	}
	if (GetItemCountByID(nTransferItemID)<pEat->GetTransferCost())
	{
		pMainMenu->PopAskMsgBox("物品不足，是否前往商城购买？",cb_ClickGoToMall,(void*)0);
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_TRANSFER_EQUIP, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_TRANSFER_EQUIP, pPacket);

	pdata->nEquip	= pEquip->GetVal(ITEM_AD_ID);
	pdata->nEat = pEat->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

void ItemCenter::GetItemInBagByFilter( xnList* pList,bool (*nFun)(ItemObj* pObj,void* param),void* param )
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (nFun(pObj,param))
		{
			pList->Add(pObj);
		}

	}
	pList->Sort(ItemSortByPos);
}

void RechargeReturnEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

bool ItemCenter::BuyMallItem( BYTE byType,int nItemID,int nCount )
{
	MallItemCfg* pItem = MallConfig::GetInstance().GetCfg(byType,nItemID);
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nItemID);
	if (!pItem||!pBase)
	{
		pMainMenu->PopTipMsg("未知错误");
		return false;
	}
	if ((pItem->m_nPrice*nCount*pItem->m_nDiscount/100 > pChar->m_dwRmb)&&byType != 6)
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturnEvent,(void*)0,3);
		return false;
	}else if (pItem->m_nPrice*nCount*pItem->m_nDiscount/100 > pChar->m_dwBindRmb&&byType == 6)
	{
		pMainMenu->PopTipMsg("绑钻不足");
		return false;
	}else if ((pChar->m_pItemCenter->m_nBagSize - (pChar->m_pItemCenter->GetBagCurCount())) < (pBase->byType == ITEMTYPE_EQUIP?nCount:((nCount-1)/pBase->nNum+1)))
	{
		pMainMenu->PopTipMsg("空间不足，购买失败！"); 
		return false;
	}
	pMainMenu->PopTipMsg("       购买成功！");
	BuildPacketEx(pPacket, ITEM, C_ITEM_MALL_BUY, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_MALL_BUY, pPacket);
	pdata->byType = byType;
	pdata->nItemID	= nItemID;
	pdata->nCount = nCount;
	SendPacket(pPacket);
	return true;
}

bool ItemCenter::SellItem( ItemObj* pObj )
{
	if (!pObj)
	{
		return false;
	}
	if (!pObj->pItemBase->GetItemFlag(ITEM_FLAG_SELL))
	{
		pMainMenu->PopTipMsg("该物品无法出售");
		return false;
	}
	if (!pObj->pItemBase->GetItemFlag(ITEM_FLAG_DESTROY))
	{
		pMainMenu->PopTipMsg("该物品无法销毁");
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_SELL, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_SELL, pPacket);
	pdata->nItemID	= pObj->GetVal(ITEM_AD_ID);
	SendPacket(pPacket);
	return true;
}

bool ItemCenter::BuyShopItem( int nType,int nItemID,int nCount )
{
	ShopItemCfg* pItem = ShopConfig::GetInstance().GetCfg(nType,nItemID);
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nItemID);
	if (!pBase)
	{
		return false;
	}
	if (pItem&&(pItem->m_nPrice*nCount > pChar->m_dwMoney)&&pItem->m_byType == SHOP_TYPE_MONEY)
	{
		pMainMenu->PopTipMsg("金币不足，购买失败！");
		return false;
	}else if (pItem&&(pItem->m_nPrice*nCount > pChar->m_pFortCenter->GetWarRankScore(pChar->m_dwUin))&&pItem->m_byType == SHOP_TYPE_WARRANK)
	{
		pMainMenu->PopTipMsg("积分不足，购买失败！");
		return false;
	}else if ((pChar->m_pItemCenter->m_nBagSize - (pChar->m_pItemCenter->GetBagCurCount())) < (pBase->byType == ITEMTYPE_EQUIP?nCount:((nCount-1)/pBase->nNum+1)))
	{
		pMainMenu->PopTipMsg("空间不足，购买失败！");
		return false;
	}
	pMainMenu->PopTipMsg("       购买成功！");
	BuildPacketEx(pPacket, ITEM, C_ITEM_SHOP_BUY, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_SHOP_BUY, pPacket);
	pdata->nType	= nType;
	pdata->nItemID	= nItemID;
	pdata->nCount = nCount;
	SendPacket(pPacket);	
	return true;
}

void ItemCenter::GetItemInBagBySlot( xnList* pList,int nType )
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		if (pObj->GetVal(ITEM_AD_SLOT) == nType)
		{
			pList->Add(pObj);
		}

	}
	pList->Sort(ItemSortByPos);
}

bool ItemCenter::FastSell( int nType )
{
	if (!nType)
	{
		return false;
	}
	BuildPacketEx(pPacket, ITEM, C_ITEM_FASTSELL, buffer,1024);
	SET_DATA(pdata, ITEM, C_ITEM_FASTSELL, pPacket);
	pdata->nType	= nType;
	SendPacket(pPacket);
	return true;
}

ItemObj* ItemCenter::GetTempItem( int dwUin,int nItemID )
{
	for (int i=0;i<m_pListTempItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListTempItems->Items[i];
		if (pObj->GetVal(ITEM_AD_UIN) == dwUin&& pObj->GetVal(ITEM_AD_ID) == nItemID)
		{
			return pObj;
		}
	}
	return NULL;
}

void ItemCenter::AddTempItem( ItemObj* pItem )
{
	m_pListTempItems->Add(pItem);
}

ItemObj* ItemCenter::CreateItemByData( char* pData,int nSize )
{
	if ((*pData) != AMZ_DATA_ITEM)
	{
		return NULL;
	}
	pData++;nSize--;
	AmzDataBase* pBase = pDataCenter->CreateAmzData(ITEM_AD_MAX,AMZ_DATA_ITEM,pData);
	pData += (ITEM_AD_MAX*sizeof(int));nSize -= (ITEM_AD_MAX*sizeof(int));
	if (!pBase)
	{
		return NULL;
	}
	RemoveTempItem(pBase->GetVal(ITEM_AD_UIN),pBase->GetVal(ITEM_AD_ID));
	ItemObj* pObj = ItemObj::Create(pBase);
	if (!pObj) return NULL;
	if (nSize<=0)
	{
		return pObj;
	}
	while(nSize>0){
		switch(*pData){
		case AMZ_DATA_EQUIPEX:
			{
				pData++;nSize--;
				AmzDataBase* pDataEx = pDataCenter->CreateAmzData(EQUIPEX_AD_MAX,AMZ_DATA_EQUIPEX,pData);
				pObj->SetExData(ITEMEXDATA_EQUIPEX,pDataEx);
				pData += (EQUIPEX_AD_MAX*sizeof(int));nSize -= (EQUIPEX_AD_MAX*sizeof(int));
			}
			break;
		}
	}
	return pObj;
}

_itemInfoCallback* ItemCenter::_getItemInfoCallback( int nKey )
{
	for (int i=0;i<m_pListCallback->Count;i++)
	{
		_itemInfoCallback* pCallback = (_itemInfoCallback*)m_pListCallback->Items[i];
		if (pCallback->nKey == nKey)
		{
			m_pListCallback->Delete(i);
			return pCallback;
		}
	}
	return NULL;
}

void ItemCenter::GetItemInfoFormServer( int dwUin,int nItemID,void (*ItemInfoCallback)(ItemObj* pObj) )
{
	BuildPacketEx(pPacket, ITEM, C_GET_ITEMINFO, buffer,1024);
	SET_DATA(pdata, ITEM, C_GET_ITEMINFO, pPacket);
	pdata->dwUin	= dwUin;
	pdata->nItemID	= nItemID;
	static int nKey = 0;
	pdata->nKey		= nKey++;
	SendPacket(pPacket);
	_itemInfoCallback* pTag = new _itemInfoCallback;
	pTag->callBack = ItemInfoCallback;
	pTag->nKey = pdata->nKey;
	m_pListCallback->Add(pTag);
}

void ItemCenter::RemoveTempItem( int dwUin,int nItemID )
{
	for (int i=0;i<m_pListTempItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListTempItems->Items[i];
		if (pObj->GetVal(ITEM_AD_UIN) == dwUin&& pObj->GetVal(ITEM_AD_ID) == nItemID)
		{
			delete (ItemObj*)m_pListTempItems->Delete(i);
			return;
		}
	}
}

int ItemCenter::GetWeaponSkin()
{
	ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(0);
	if (pItem&&pItem->pItemBase->nSkin>1000)
	{
		return pItem->pItemBase->nSkin+(pChar->m_bySex?0:1);
	}
	return 2999+pChar->m_byJob*2+(pChar->m_bySex?0:1);
}

int ItemCenter::GetClothSkin()
{
	ItemObj* pItem = pChar->m_pItemCenter->GetEquipBySlot(EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN));
	if (pItem&&pItem->pItemBase->nSkin>1000)
	{
		return pItem->pItemBase->nSkin;
	}
	pItem = pChar->m_pItemCenter->GetEquipBySlot(EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_CLOTH));
	if (pItem&&pItem->pItemBase->nSkin>1000)
	{
		return pItem->pItemBase->nSkin+(pChar->m_bySex?0:1);
	}
	return 999+pChar->m_byJob*2+(pChar->m_bySex?0:1);
}

void ItemCenter::SortBag()
{
	static int nTime = 0;
	int nTick = xnGetTickCount();
	if (nTime&&nTick-nTime<60000)
	{
		pMainMenu->PopTipMsg("每60秒才能整理一次背包");
		return;
	}
	nTime = nTick;
	pChar->SendCmd(ITEM,C_SORT_BAG);
}

void ItemCenter::GoForFix()
{
	xnList* pList = xnList::Create();
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(g_pMapCenter->m_nMapIndex,pList);
	int nSafeMap = 101;
	if(pChar->m_byCountry == 2)
		nSafeMap = 201;
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(nSafeMap,pList);
	for(int i = 0; i < pList->Count; i ++)
	{
		PathObj* pObj = (PathObj*)pList->Items[i];
		if(pObj->nShopIndex >= 0 && pObj->nShopIndex < 1000)
		{
			g_pMapCommand->CmdPathObj(pObj);
			pList->Free();
			return;
		}
	}
	pList->Free();
}

void ItemCenter::GoForBuy(int nType)
{
	xnList* pList = xnList::Create();
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(g_pMapCenter->m_nMapIndex,pList);
	int nSafeMap = 101;
	if(pChar->m_byCountry == 2)
		nSafeMap = 201;
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(nSafeMap,pList);
	for(int i = 0; i < pList->Count; i ++)
	{
		PathObj* pObj = (PathObj*)pList->Items[i];
		if(pObj->nShopIndex == nType)
		{
			g_pMapCommand->CmdPathObj(pObj);
			pList->Free();
			return;
		}
	}
	pList->Free();
}

void ItemCenter::GetMallItemByType( xnList* pList,BYTE byType )
{
	for (int i=0;i<m_pMallList->Count;i++)
	{
		MallItemCfg* pItem = (MallItemCfg*)m_pMallList->Items[i];
		if (pItem->m_byType == byType)
		{
			pList->Add(pItem);
		}
	}
}

ItemObj* ItemCenter::GetItemByTypeID(int nTypeID,int nSlot)
{
	for (int i=0;i<m_pListItems->Count;i++)
	{
		ItemObj* pObj = (ItemObj*)m_pListItems->Items[i];
		int nSlot2 = pObj->GetVal(ITEM_AD_SLOT);
		if (pObj->pItemBase&&pObj->pItemBase->nTypeID == nTypeID&& ((1<<nSlot2)&nSlot))
		{
			return pObj;
		}
	}
	return NULL;
}

int ItemCenter::GetBagCurCount()
{
	xnList* pList = xnList::Create();
	GetItemInBagByPos(pList);
	m_nBagCurCount = pList->Count;
	return m_nBagCurCount;
}

bool ItemCenter::CheckSkin( int nSkin )
{
	return nSkin>=1500&&nSkin<2000;

}

void ItemCenter::OnItemRemove( ItemObj* pObj )
{
	if (pMainMenu->m_pItemInfoMenu->m_pForm->bOpened&&pMainMenu->m_pItemInfoMenu->m_pItemObj == pObj)
	{
		pMainMenu->m_pItemInfoMenu->Close();
	}
	if (pMainMenu->m_pEquipInfoMenu->m_pForm->bOpened&&pMainMenu->m_pEquipInfoMenu->m_pItemObj == pObj)
	{
		pMainMenu->m_pEquipInfoMenu->Close();
	}
}

void ItemCenter::UseMakeSkill( MakeSkillTag* pTag )
{
	if (!pTag)
	{
		return;
	}
	for (int i=0;i < pTag->pListCost->Count;i++)
	{
		CommonItemTag* pItem = (CommonItemTag*)pTag->pListCost->Items[i];
		if (GetItemCountByID(pItem->nItemID)<pItem->nItemCount)
		{
			pMainMenu->PopTipMsg("所需材料不足");
			return;
		}
	}
	if (GetBagCurCount()>=m_nBagSize)
	{
		pMainMenu->PopTipMsg("包裹空间不足");
		return;
	}
	BuildPacketEx(pPacket, ITEM, C_MAKE_SKILL_ITEM, buffer,1024);
	SET_DATA(pdata, ITEM, C_MAKE_SKILL_ITEM, pPacket);
	pdata->nID	= pTag->nID;
	SendPacket(pPacket);
}

void ItemCenter::MakeSkillResult( BYTE byResult )
{
	if (byResult == 1)
	{
		pMainMenu->PlayEffect(264);
	}else{
		pMainMenu->PopTipMsg("没有可消除的物体,制作失败");
	}
	BuildPacketEx(pPacket, ITEM, C_MAKE_SKILL_ITEM_RESULT, buffer,1024);
	SET_DATA(pdata, ITEM, C_MAKE_SKILL_ITEM_RESULT, pPacket);
	pdata->byResult	= byResult;
	SendPacket(pPacket);
	pMainMenu->m_pMakeSkillMenu->m_pFormMake->Close();
}


int ItemCenter::GetEquipCountInGroup( int nGroup )
{
	int nCount = 0;
	for (int i=0;i<EQUIP_MAX;i++)
	{
		ItemObj* pObj = GetEquipBySlot(i);
		if (pObj&&pObj->pItemBase->nGroup == nGroup)
		{
			nCount++;	
		}
	}
	return nCount;
}

void ItemCenter::LearnMakeSkill( BYTE byMakeSkill )
{
	if (pChar->m_makeSkill.bySkill[0] != MAKE_SKILL_TYPE_NULL)
	{
		pMainMenu->PopTipMsg("已经学习了一项生活技能");
		return;
	}
	if (byMakeSkill<MAKE_SKILL_TYPE_FORGE&&byMakeSkill>=MAKE_SKILL_TYPE_MAX)
	{
		pMainMenu->PopTipMsg("未知生活技能");
		return;
	}
	if (pChar->m_byLvl<35)
	{
		pMainMenu->PopTipMsg("需要人物等级达到35");
		return;
	}
	int nCnt = pChar->m_pClientFlag->GetFlag(nMakeSkillResetTimesFlag);
	if (nCnt == 0&&pChar->m_dwMoney<10000)
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}else if (nCnt == 1&&pChar->m_dwMoney<50000)
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}else if (nCnt>=2&&pChar->m_dwRmb<50)
	{
		pMainMenu->PopTipMsg("钻石不足");
		return;
	}
	pMainMenu->PopTipMsg("学习成功！");
	pMainMenu->PlayEffect(28);
	BuildPacketEx(pPacket, ITEM, C_LEARN_MAKE_SKILL, buffer,1024);
	SET_DATA(pdata, ITEM, C_LEARN_MAKE_SKILL, pPacket);
	pdata->byResult	= byMakeSkill;
	SendPacket(pPacket);
}

bool ItemCenter::JudgeChangeCountryItem( int nItemID )
{
	if (nItemID == 45015)	//	转国符
	{
		if (pChar->m_pMarryCenter->m_pRelationList->Count)
		{
			pMainMenu->PopTipMsg("您必须先离婚后才能转国。");
			return false;
		}
		if (pChar->m_pMentoringClient->m_pRelationList->Count)
		{
			pMainMenu->PopTipMsg("您必须先脱离师徒关系才能转国。");
			return false;
		}
		if (pChar->m_dwHomeID != 0)
		{
			pMainMenu->PopTipMsg("您必须先退出军团才能转国");
			return false;
		}
		if (pMainMenu->m_pTeamMenu->GetTeamCount())
		{
			pMainMenu->PopTipMsg("您必须先退出队伍才能转国");
			return false;
		}
	}
	return true;
}
