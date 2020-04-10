#include "stdafx.h"
#include "ItemCenter.h"
#include "UserData.h"
#include "UserQuest.h"
#include "ios_item_packet.h"
#include "LuaClass.h"
#include "MallConfig.h"
#include "UserFlag.h"
#include "FsGameCenter.h"
#include "ItemManage.h"
#include "MailCenter.h"
#include "ZLogFile.h"
#include "FortCenter.h"
#include "MakeSkillData.h"
#include "MarkGameDB.h"
#include "WarRank.h"
#include "MapConfig.h"
#include "ios_relation_packet.h"

extern CLuaStateManager * pLuaStateManager;
extern	FsGameCenter* g_pGameCenter;
extern	ZLogFile* g_pLogFile;
ItemObj::ItemObj(AmzDataBase* pData,ItemCenter* pCenter):m_pData(pData),m_pCenter(pCenter)
{
	for (int i=0;i<ITEMEXDATA_MAX;i++)
	{
		m_pDataEx[i] = NULL;
	}
	memset(m_nParam,0,sizeof(int)*EQUIP_ATTR_MAX);
	m_pBase = ItemConfig::GetInstance().GetItemBase(pData->GetVal(ITEM_AD_TYPE));

	CLuaClassObject<ItemObj>::Init(this);

}

ItemObj::~ItemObj()
{

}

bool ItemObj::SetExData( int nType,AmzDataBase* pData )
{
	switch(nType){
	case ITEMEXDATA_EQUIPEX:
		{
			if(m_pBase->byType != ITEMTYPE_EQUIP) return false;
			m_pDataEx[ITEMEXDATA_EQUIPEX] = pData;
			RecalcParam();
			return true;
		}
	}
	return false;
}

int ItemObj::GetDataVal( BYTE nType )
{
	return m_pData->GetVal(nType);
}

void ItemObj::SetDataVal( BYTE nType,int nData )
{
	m_pData->SetVal(nType,nData);
}

void ItemObj::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<ItemObj> * pscItemCenter = pLuaState->CreateScriptClass((ItemObj*)0);

	pscItemCenter->DefObjectFunction("GetItemID", &ItemObj::GetItemID)
		.DefObjectFunction("GetDataVal", &ItemObj::GetDataVal)
		.DefObjectFunction("GetDataExVal", &ItemObj::GetDataExVal)
		.DefObjectFunction("GetIndex", &ItemObj::GetIndex);
}

void ItemObj::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((ItemObj*)0);
}

void ItemObj::SetExDataVal( int nExType,BYTE nType,int nData )
{
	if (nExType>=ITEMEXDATA_MAX)
	{
		return;
	}
	m_pDataEx[nExType]->SetVal(nType,nData);
}

void ItemObj::RecalcParam()
{
	if (m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return;
	}
	memset(m_nParam,0,sizeof(int)*EQUIP_ATTR_MAX);
	if (!GetDataVal(ITEM_AD_NUM)&&m_pCenter)
	{
		if(GetDataVal(ITEM_AD_SLOT)>ITEM_SLOT_EQUIP_START&&GetDataVal(ITEM_AD_SLOT)<ITEM_SLOT_EQUIP_END) m_pCenter->m_pUser->RecalcParam();
		return;
	}
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		m_nParam[i] += m_pBase->nParam[i];
	}
	if (m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		int nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE1);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM1));
		}
		nType = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDTYPE2);
		if(nType){
			_recalcAttr(nType,m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ADDRANDOM2));
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
					m_nParam[i] *= (10000+pBase->nPer);
					m_nParam[i] /=10000;
				}
			}
		}
		for (int i=0;i<nMaxStoneCount;i++)
		{
			ItemExBase* pStone = _getEquipStoneByPos(i);
			if (pStone)
			{
				for (int j=0;j<EQUIP_ATTR_MAX;j++)
				{
					m_nParam[j] += pStone->nParam[j];
				}
			}
		}
		int nEatLv = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_EATLV)<<24>>24;
		if (nEatLv)
		{
			EquipEatBase* pBase = EquipEatConfig::GetInstance().GetEquipEat(nEatLv);
			int nType = EquipEatConfig::GetInstance().GetAddParam(m_pBase->bySlot);
			m_nParam[nType?EQUIP_ATTR_DEF_W:EQUIP_ATTR_ATK_W] += nType?pBase->nDefense:pBase->nDamage;
			m_nParam[nType?EQUIP_ATTR_DEF_N:EQUIP_ATTR_ATK_N] += nType?pBase->nDefense:pBase->nDamage;
		}
	}
	if(GetDataVal(ITEM_AD_SLOT)>ITEM_SLOT_EQUIP_START&&GetDataVal(ITEM_AD_SLOT)<ITEM_SLOT_EQUIP_END&&m_pCenter) 
		m_pCenter->m_pUser->RecalcParam();
}

void ItemObj::_recalcAttr( int nType,int nVal )
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
				m_nParam[i] += pBase->nBase[i];
				m_nParam[i] += nVal1;
				bFlag = false;
			}else if (pBase->nBase[i]&&!bFlag)
			{
				m_nParam[i] += pBase->nBase[i];
				m_nParam[i] += nVal2;
				break;
			}
		}
	}
}

int ItemObj::GetDataExVal( int nExType,BYTE nType )
{
	if (!m_pDataEx[nExType])
	{
		return 0;
	}
	return m_pDataEx[nExType]->GetVal(nType);
}

ItemExBase* ItemObj::_getEquipStoneByPos( int nPos )
{
	if (nPos>=nMaxStoneCount)
	{
		return NULL;
	}
	int nID = 0;
	if (nPos /2 == 0)
	{
		if(nPos%2) nID = GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE12)>>16;
		else nID = GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE12)<<16>>16;
	}else if (nPos/2 == 1)
	{
		if(nPos%2) nID = GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE34)>>16;
		else nID = GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE34)<<16>>16;
	}
	if (nID)
	{
		return ItemConfig::GetInstance().GetItemBase(nID);
	}
	return NULL;
}

void ItemObj::SendItemInfo(DWORD dwUin,int nKey)
{
	int nSize = 0;
	nSize += sizeof(BYTE);
	nSize += m_pData->GetByteSize();
	for (int i=0;i<ITEMEXDATA_MAX;i++)
	{
		if (m_pDataEx[i])
		{
			nSize += sizeof(BYTE);
			nSize += m_pDataEx[i]->GetByteSize();
		}
	}
	BuildPacketEx(pPacket, ITEM, S_SEND_ITEMINFO_SERVER, buf, 1024);
	SET_DATA(pSend, ITEM, S_SEND_ITEMINFO_SERVER, pPacket);
	pSend->nSize = nSize;
	pSend->nKey = nKey;
	pPacket->wSize += nSize;
	int nOff = 0;
	pSend->byData[nOff] = AMZ_DATA_ITEM;
	nOff += sizeof(BYTE);
	m_pData->GetByteData(pSend->byData+nOff);
	nOff += m_pData->GetByteSize();

	if (m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		pSend->byData[nOff] = AMZ_DATA_EQUIPEX;
		nOff += sizeof(BYTE);
		m_pDataEx[ITEMEXDATA_EQUIPEX]->GetByteData(pSend->byData+nOff);
		nOff += m_pData->GetByteSize();
	}
	TCP_SendPacketToUin(dwUin,pPacket);
}

int ItemObj::GetByteSize()
{
	int nSize = 0;
	nSize += sizeof(BYTE);
	nSize += m_pData->GetByteSize();
	for (int i=0;i<ITEMEXDATA_MAX;i++)
	{
		if (m_pDataEx[i])
		{
			nSize += sizeof(BYTE);
			nSize += m_pDataEx[i]->GetByteSize();
		}
	}
	return nSize;
}

void ItemObj::GetByteData( char* pData )
{
	int nOff = 0;
	pData[nOff] = AMZ_DATA_ITEM;
	nOff += sizeof(BYTE);
	m_pData->GetByteData(pData+nOff);
	nOff += m_pData->GetByteSize();

	if (m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		pData[nOff] = AMZ_DATA_EQUIPEX;
		nOff += sizeof(BYTE);
		m_pDataEx[ITEMEXDATA_EQUIPEX]->GetByteData(pData+nOff);
		nOff += m_pData->GetByteSize();
	}
}

int ItemObj::GetTransferCost()
{
	int nCost = 0;
	int nLv = m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_LV)<<24>>24;
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
		int nStoneLv = _getEquipStoneByPos(i)?_getEquipStoneByPos(i)->nTypeID:0;
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

bool ItemObj::IsEquiped()
{
	return GetDataVal(ITEM_AD_SLOT)>ITEM_SLOT_EQUIP_START&&GetDataVal(ITEM_AD_SLOT)<ITEM_SLOT_EQUIP_END;
}

void ItemObj::ChgDura( int nChg )
{
	int nDura = GetDataVal(ITEM_AD_NUM);
	int nPre = nDura;
	nDura+=nChg;
	if (nDura<0)
	{
		nDura = 0;
	}else if (nDura >m_pBase->nDura)
	{
		nDura = m_pBase->nDura;
	}
	SetDataVal(ITEM_AD_NUM,nDura);
	if (!nDura||!nPre)
	{
		RecalcParam();
	}
}

void ItemObj::Release()
{
	if (m_pData)
	{
		delete m_pData;
	}
	for (int i=0;i<ITEMEXDATA_MAX;i++)
	{
		if(m_pDataEx[i]) delete m_pDataEx[i];
	}
}

void ItemObj::SetDataValEx( BYTE nType,int nData,bool bSaveDB /*= true*/ )
{

	m_pData->SetVal(nType,nData,bSaveDB);
}

int ItemObj::GetItemID()
{
	if(m_pBase == NULL)
		return 0;
	return m_pBase->nTypeID;
}

int ItemObj::GetScore(int nJob)
{
	int nScore = 0;
	static int nAttrScore[EQUIP_ATTR_MAX] = {4,6,12,12,15,15,20,30,40};
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		nScore += m_nParam[i]*nAttrScore[i];
	}
	switch(nJob)
	{
	case 0:
		return 0;
	case 1:
		return nScore/30.5;
	case 2:
		return nScore/29.2;
	case 3:
		return nScore/34.4;
	case 4:
		return nScore/28.5;
	}
	return nScore;
}

int ItemObj::GetScore()
{
	int nScore = 0;
	static int nAttrScore[EQUIP_ATTR_MAX] = {4,6,12,12,15,15,20,30,40};
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		nScore += m_nParam[i]*nAttrScore[i];
	}
	int nJob = 0;
	for(int i = 1; i < 5; i ++)
	{
		if(m_pBase->nJob & (1<<i))
		{
			nJob = i;
			break;
		}
	}
	switch(nJob)
	{
	case 0:
		return 0;
	case 1:
		return nScore/30.5;
	case 2:
		return nScore/29.2;
	case 3:
		return nScore/34.4;
	case 4:
		return nScore/28.5;
	}
	return nScore;
}

ItemCenter::ItemCenter( UserData* pUser ):m_pUser(pUser)
{
	m_nBagSize = 40;
	m_nBankSize = 80;
	m_pItemInBag = new ItemObj*[m_nBagSize];
	memset(m_pItemInBag,0,sizeof(ItemObj*)*m_nBagSize);
	m_pItemInBank = new ItemObj*[m_nBankSize];
	memset(m_pItemInBank,0,sizeof(ItemObj*)*m_nBankSize);

	
	for (int i=0;i<EQUIP_MAX;i++)
	{
		m_pEquip[i] = NULL;
	}

	CLuaClassObject<ItemCenter>::Init(this);

	m_pSysMsgedList = xnList::Create();

	m_bEnemyDrop = false;
}

ItemCenter::~ItemCenter()
{
 	for (int i=0;i<m_nBagSize;i++)
 	{
 		if (m_pItemInBag[i])
 		{
 			delete m_pItemInBag[i];
 		}
 	}
 	delete [] m_pItemInBag;
 	for (int i=0;i<m_nBankSize;i++)
 	{
 		if (m_pItemInBank[i])
 		{
 			delete m_pItemInBank[i];
 		}
 	}
	delete [] m_pItemInBank;

	while(m_pSysMsgedList->Count)
		m_pSysMsgedList->Delete(0);
	m_pSysMsgedList->Free();

}

void ItemCenter::Init( )
{
	list<ItemObj*>	m_listLoad;
	for (int i=0;i<m_pUser->m_pAmzDManager->GetAmzTCnt(AMZ_DATA_ITEM);i++)
	{
		AmzDataBase* pBase = m_pUser->m_pAmzDManager->GetAmzDataByPos(AMZ_DATA_ITEM,i);
		ItemObj* pObj = new ItemObj(pBase,this);
		if(pObj->m_pBase == NULL)
		{
			delete pObj;
			continue;
		}
		
		m_mapItem[pBase->GetVal(ITEM_AD_ID)] = pObj;
		int nSite = pObj->GetDataVal(ITEM_AD_SLOT);
		if (nSite == ITEM_SLOT_BAG)
		{
			int nPos = pObj->GetDataVal(ITEM_AD_POS);
			if(nPos<m_nBagSize&&!m_pItemInBag[nPos]) m_pItemInBag[nPos] = pObj;
			else
			{
				OutMessage(msg_type_error,"%d的%d物品载入异常",pBase->GetVal(ITEM_AD_UIN),pBase->GetVal(ITEM_AD_ID));
				m_listLoad.push_back(pObj);
			}
		}else if (nSite>ITEM_SLOT_EQUIP_START&&nSite<ITEM_SLOT_EQUIP_END)
		{
			m_pEquip[EQUIP_SLOT_TO_INDEX(nSite)] = pObj;
		}else if (nSite == ITEM_SLOT_DEPOT)
		{
			if(pObj->GetDataVal(ITEM_AD_POS)<m_nBankSize) m_pItemInBank[pObj->GetDataVal(ITEM_AD_POS)] = pObj;
		}else if (nSite == ITEM_SLOT_SELL)
		{
			m_listSellItem.push_back(pObj);
			//if(pObj->GetDataVal(ITEM_AD_POS)<m_nBankSize) m_pItemInBank[pObj->GetDataVal(ITEM_AD_POS)] = pObj;
		}
	}
	for (list<ItemObj*>::iterator itor = m_listLoad.begin();itor!= m_listLoad.end();itor++)
	{
		int nIndex = _getBlankBagIndex();
		if (nIndex!=-1)
		{
			_setItemPos(*itor,ITEM_SLOT_BAG,nIndex);
		}else{
			//delete *itor;
		}
	}
	m_listLoad.clear();
	for (int i=m_pUser->m_pAmzDManager->GetAmzTCnt(AMZ_DATA_EQUIPEX)-1;i>=0;i--)
	{
		AmzDataBase* pBase = m_pUser->m_pAmzDManager->GetAmzDataByPos(AMZ_DATA_EQUIPEX,i);
		if(m_mapItem.find(pBase->GetVal(EQUIPEX_AD_ITEMID)) != m_mapItem.end()){
			m_mapItem[pBase->GetVal(EQUIPEX_AD_ITEMID)]->SetExData(ITEMEXDATA_EQUIPEX,pBase);
		}else{
			m_pUser->m_pAmzDManager->RemoveAmzData(AMZ_DATA_EQUIPEX,pBase->nModelID);
			OutMessage(msg_type_error,"EQUIP ERROR: UIN(%d)---%d(%d)",pBase->GetVal(EQUIPEX_AD_UIN),pBase->GetVal(EQUIPEX_AD_ITEMID),pBase->nModelID);
		}
	}
}

void	ItemCenter::CheckEquipRank()
{
	std::map<int,ItemObj*>::iterator itor = m_mapItem.begin();
	while(itor != m_mapItem.end())
	{
		ItemObj* pObj = itor->second;
		if(pObj->m_pBase->byType == ITEMTYPE_EQUIP && pObj->m_pBase->bySlot != EQUIP_SLOT_TO_INDEX(ITEM_SLOT_EQUIP_SKIN))
		{
			_MuliParamObj* pTemp = new _MuliParamObj();
			pTemp->nParam1 = m_pUser->m_dwUin;
			pTemp->nParam2 = pObj->GetIndex();
			g_pGameCenter->m_pRankCenter->CheckRank(RANK_EQUIP,(void*)pTemp,false);
		}
		itor++;
	}
}

ItemObj* ItemCenter::GetItemByIndex( int nID )
{
	if(m_mapItem.find(nID) != m_mapItem.end()){
		return m_mapItem[nID];
	}
	return NULL;
}

int ItemCenter::GetItemCountByID( int nType)
{
	return GetItemCountByIDEx(nType);
// 	int nCount = 0;
// 	for (map<int,ItemObj*>::iterator itor = m_mapItem.begin();itor!=m_mapItem.end();itor++)
// 	{
// 		if(itor->second->GetItemID() == nType ){
// 			nCount += itor->second->GetDataVal(ITEM_AD_NUM);
// 		}
// 	}
// 	return nCount;
}

void ItemCenter::SetBagSize( int nBagSize )
{
	if (m_nBagSize == nBagSize)
	{
		return;
	}
	ItemObj** pObj = new ItemObj*[nBagSize];
	memset(pObj,0,sizeof(ItemObj*)*nBagSize);
	memcpy(pObj,m_pItemInBag,sizeof(ItemObj*)*(nBagSize>m_nBagSize?m_nBagSize:nBagSize));
	m_nBagSize = nBagSize;
	delete [] m_pItemInBag;
	m_pItemInBag = pObj;
}

int ItemCenter::GetBlankBag()
{
	int nCount = 0;
	for (int i=0;i<m_nBagSize;i++)
	{
		if(!m_pItemInBag[i]) nCount++;
	}
	return nCount;
}

bool ItemCenter::GetItemByType( int nType,list<ItemObj*>& itemlist ,int nSite,bool bBind)
{
	for (map<int,ItemObj*>::iterator itor = m_mapItem.begin();itor!=m_mapItem.end();itor++)
	{
		if(itor->second->GetItemID() == nType&& ((1<<itor->second->GetDataVal(ITEM_AD_SLOT))&nSite) && (bBind == (bool)itor->second->GetDataVal(ITEM_AD_BIND))) itemlist.push_back(itor->second);
	}
	return true;
}

void ItemCenter::SetEnemyDrop(bool bEnemyDrop)
{
	m_bEnemyDrop = bEnemyDrop;
}

bool ItemCenter::AddItemByID( int nType,int nCount /*= 1*/ ,int byBind,int nLogType)
{
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nType);
	if (!pBase)
	{
		return false;
	}
	if (nLogType && pBase->GetItemFlag(ITEM_FLAG_LOG))
	{
		m_pUser->m_pUserRecord->AddItemLog(nType,nCount,nLogType);
	}
	if (nCount == 0)
	{
		return false;
	}
	if (nCount<0)
	{
		return RemoveItemByID(nType,-nCount);
	}
	int nAddCount = nCount;
	bool bCanAdd = true;
	if(pBase->nNum == 1||pBase->byType == ITEMTYPE_EQUIP) bCanAdd = false;
	if (bCanAdd)
	{
		list<ItemObj*> itemlist;
		GetItemByType(nType,itemlist,1<<ITEM_SLOT_BAG,byBind);
		for(list<ItemObj*>::iterator itor = itemlist.begin();itor != itemlist.end();itor++){
			int nNum = (*itor)->GetDataVal(ITEM_AD_NUM);
			if(nNum<pBase->nNum&&nAddCount>0){
				int nChg = (pBase->nNum-nNum>nAddCount)?nAddCount:(pBase->nNum-nNum);
				nAddCount -= nChg;
				(*itor)->SetDataVal(ITEM_AD_NUM,nNum+nChg);
			}
		}
		while (nAddCount>0)
		{
			if (pBase->nTypeID/1000 == 55&&_getBlankBagIndex() == -1)//任务物品 无法邮件  这边截断
			{
				m_pUser->SendWarnMsg("包裹已满");
				return false;
			}
			int nChg = (pBase->nNum>nAddCount)?nAddCount:(pBase->nNum);
			if(!_addNewItem(nType,nChg,byBind)){
				if (m_bEnemyDrop &&pBase->nStar < 2 )
				{
					return false;
				}else
					AddItemByMail(nType,nChg,byBind);
			}
			nAddCount -= nChg;

		}
	}else{
		while (nAddCount>0)
		{
			if (pBase->nTypeID/1000 == 55&&_getBlankBagIndex() == -1)//任务物品 无法邮件  这边截断
			{
				m_pUser->SendWarnMsg("包裹已满");
				return false;
			}
			int nChg = (pBase->nNum>nAddCount)?nAddCount:(pBase->nNum);
			if(!_addNewItem(nType,nChg,byBind)){
				if (m_bEnemyDrop &&pBase->nStar < 2 )
				{
					return false;
				}else
					AddItemByMail(nType,nChg,byBind);
			}
			nAddCount -= nChg;
		}
	}
	if(nCount - nAddCount > 0)
	{
		m_pUser->SendMsg("获得%s×%d",pBase->szName,nCount - nAddCount);
	}
	return true;
}

int ItemCenter::_getBlankBagIndex()
{
	for (int i=0;i<m_nBagSize;i++)
	{
		if(!m_pItemInBag[i]) return i;
	}
	return -1;
}

ItemObj* ItemCenter::_addNewItem( int nType,int nCount ,bool bBind)
{
	int nIndex = _getBlankBagIndex();
	if (nIndex == -1){
		return 0;}

	//m_pUser->m_pQuestCenter->AddItem(nType,nCount);

	AmzDataBase* pAmzBase = m_pUser->m_pAmzDManager->AddAmzData(ITEM_AD_MAX,AMZ_DATA_ITEM,m_pUser->LockADataID());
	if(!pAmzBase) return 0;
	LONGLONG temp = m_pUser->m_dwUin;
	pAmzBase->llKey	= ((temp<< 32)|pAmzBase->nModelID);

	pAmzBase->SetVal(ITEM_AD_UIN,m_pUser->m_dwUin);
	pAmzBase->SetVal(ITEM_AD_ID,pAmzBase->nModelID);
	pAmzBase->SetVal(ITEM_AD_TYPE,nType);
	pAmzBase->SetVal(ITEM_AD_NUM,nCount);
	pAmzBase->SetVal(ITEM_AD_SLOT,ITEM_SLOT_BAG);
	pAmzBase->SetVal(ITEM_AD_POS,nIndex);
	pAmzBase->SetVal(ITEM_AD_BIND,bBind);
	pAmzBase->bStateFlag = AMZ_STATE_CREATE;

	ItemObj* pObj = new ItemObj(pAmzBase,this);
	if(pObj->m_pBase == NULL)
		return NULL;
	if(pObj->m_pBase->byBind == ITEM_BINDTYPE_BIND_GET) pObj->SetDataVal(ITEM_AD_BIND,1);
	if (pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		pObj->SetDataVal(ITEM_AD_NUM,pObj->m_pBase->nDura);
		_addEquipEx(pObj);
		pObj->RecalcParam();
		m_pUser->m_pUserRecord->RunLuaEquipFunc(1,pObj,NULL);
		_MuliParamObj* pTemp = new _MuliParamObj();
		pTemp->nParam1 = m_pUser->m_dwUin;
		pTemp->nParam2 = pObj->GetIndex();
		g_pGameCenter->m_pRankCenter->CheckRank(RANK_EQUIP,(void*)pTemp,false);
	}
	if (pObj->m_pBase->dwTimeLimit)
	{
		pObj->SetDataVal(ITEM_AD_LIMITTIME,CMTime::GetCurTickCount()+pObj->m_pBase->dwTimeLimit*60);
	}
	m_mapItem[pAmzBase->GetVal(ITEM_AD_ID)] = pObj;
	m_pItemInBag[nIndex] = pObj;
	if(pObj->m_pBase->nTypeID == MARRY_ITEM_EFF)
		m_pUser->m_pUserMarry->ResetEff();
	return pObj;
}

bool ItemCenter::RemoveItemByIndex( int nID,int nCount /*= 1*/ ,int nLogType)
{
	ItemObj* pObj = GetItemByIndex(nID);
	if (!pObj||(pObj->GetDataVal(ITEM_AD_NUM)<nCount&&pObj->m_pBase->byType != ITEMTYPE_EQUIP))
	{
		return false;
	}

	int nIndex = pObj->m_pBase->nTypeID;
	int nNum = pObj->GetDataVal(ITEM_AD_NUM)-nCount;
	if (nNum && nCount!=-1 && pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		m_pUser->SendMsg("失去%s×%d",pObj->m_pBase->szName,nCount);
		if (pObj->m_pBase->GetItemFlag(ITEM_FLAG_LOG))
			m_pUser->m_pUserRecord->AddItemLog(pObj->m_pBase->nTypeID,-nCount,0);
		pObj->SetDataVal(ITEM_AD_NUM,nNum);
		g_pMarkGameDB->AddQICnt(pObj->GetItemID(),0,nCount);		
	}
	else if(pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		m_pUser->SendMsg("失去%s×%d",pObj->m_pBase->szName,1);
		if (pObj->m_pBase->GetItemFlag(ITEM_FLAG_LOG))
			m_pUser->m_pUserRecord->AddItemLog(pObj->m_pBase->nTypeID,-1,0);
		g_pMarkGameDB->AddQICnt(pObj->GetItemID(),0,1);
		_removeItem(pObj);
		
	}
	else{
		m_pUser->SendMsg("失去%s×%d",pObj->m_pBase->szName,pObj->GetDataVal(ITEM_AD_NUM));
		if (pObj->m_pBase->GetItemFlag(ITEM_FLAG_LOG))
			m_pUser->m_pUserRecord->AddItemLog(pObj->m_pBase->nTypeID,-pObj->GetDataVal(ITEM_AD_NUM),0);
		g_pMarkGameDB->AddQICnt(pObj->GetItemID(),0,pObj->GetDataVal(ITEM_AD_NUM));
		_removeItem(pObj);
		
	}
	if(nIndex == MARRY_ITEM_EFF)
		m_pUser->m_pUserMarry->ResetEff();

	
	return true;
}

bool ItemCenter::_removeItem( ItemObj* pObj )
{
	if(pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		g_pGameCenter->m_pRankCenter->DeleteRank(RANK_EQUIP,m_pUser->m_dwUin,pObj->GetIndex());
	}
	map<int,ItemObj*>::iterator itor = m_mapItem.find(pObj->GetDataVal(ITEM_AD_ID));
	if (itor !=m_mapItem.end())
	{
		if(pObj->GetItemID() == MARRY_ITEM_EFF)
			m_pUser->m_pUserMarry->ResetEff();
		m_mapItem.erase(itor);
		if(pObj->GetDataVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG&&m_pItemInBag[pObj->GetDataVal(ITEM_AD_POS)]) m_pItemInBag[pObj->GetDataVal(ITEM_AD_POS)] = NULL;
		else if(pObj->GetDataVal(ITEM_AD_SLOT) == ITEM_SLOT_DEPOT&&m_pItemInBank[pObj->GetDataVal(ITEM_AD_POS)]) m_pItemInBank[pObj->GetDataVal(ITEM_AD_POS)] = NULL;
		else if(pObj->GetDataVal(ITEM_AD_SLOT) == ITEM_SLOT_SELL) _removeSellItem(pObj,true);
		else if (pObj->IsEquiped()) m_pEquip[EQUIP_SLOT_TO_INDEX(pObj->GetDataVal(ITEM_AD_SLOT))] = NULL;
		else ;
		m_pUser->m_pAmzDManager->RemoveAmzData(AMZ_DATA_ITEM,pObj->GetDataVal(ITEM_AD_ID));
		if(pObj->m_pDataEx[ITEMEXDATA_EQUIPEX]) m_pUser->m_pAmzDManager->RemoveAmzData(AMZ_DATA_EQUIPEX,pObj->m_pDataEx[ITEMEXDATA_EQUIPEX]->GetVal(EQUIPEX_AD_ID));
		delete pObj;
		return true;
	}
	return false;
}

bool ItemCenter::RemoveItemByID( int nType,int nCount /*= 1*/ ,int nSite,int nLogType)
{

	return RemoveItemByIDEx(nType,nCount,nSite,nLogType);
// 	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nType);
// 	list<ItemObj*> itemlist;
// 	GetItemByType(nType,itemlist,nSite,true);
// 	GetItemByType(nType,itemlist,nSite,false);
// 	int nSubCount = nCount;
// 	for(list<ItemObj*>::iterator itor = itemlist.begin();itor != itemlist.end();itor++){
// 		ItemObj* pObj = (*itor);
// 		int nNum = (*itor)->GetDataVal(ITEM_AD_NUM);
// 		if(nSubCount>0){
// 			int nChg = (nNum>nSubCount)?nSubCount:(nNum);
// 			nSubCount -= nChg;
// 			if(nNum-nChg){
// 				(*itor)->SetDataVal(ITEM_AD_NUM,nNum-nChg);
// 			}else{
// 				//itor = itemlist.erase(itor);
// 				_removeItem(pObj);
// 			}
// 		}else{
// 			return true;
// 		}
// 	}
// 	if (!nSubCount)
// 	{
// 		m_pUser->SendMsg("失去%s×%d",pBase->szName,nCount);
// 		return true;
// 	}
// 	return false;
}

static int nCakeCost[] = {20000,40000,70000,110000,160000,220000,290000,370000,460000,560000};

bool ItemCenter::UseItem( ItemObj* pObj)
{
	if (!pObj)
	{
		return false;
	}
	//限制
	if (MapLimitConfig::GetInstance().CheckMapItemULimit(m_pUser->m_nRealMIndex,pObj->m_pBase->nTypeID) == true)
	{
		m_pUser->SendWarnMsg("当前地图无法使用该物品！");
		return false;
	}

	if (!(pObj->m_pBase->nJob&(1<<m_pUser->m_byJob)))
	{
		return false;
	}
	if (pObj->m_pBase->bySexLimit&&pObj->m_pBase->bySexLimit !=(m_pUser->m_bySex+1))
	{
		return false;
	}
	if (pObj->m_pBase->byLv>m_pUser->m_xUWealth.GetWealth(UW_LV))
	{
		return false;
	}
	if ((pObj->m_pBase->wCDPos&&pObj->m_pBase->dwCDTime)&&m_pUser->m_pUserCD->CheckIsLock(pObj->m_pBase->wCDPos))
	{
		return false;
	}
	if (pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		return EquipTakeOn(pObj);
	}

	if (pObj->m_pBase->nTypeID == 60022)
	{//甜蜜活动 蛋糕
		int nCakeFlag = m_pUser->m_pUserFlag->GetFlag(63);
		if (nCakeFlag > 19)
			return false;

		if (nCakeFlag > 9 && !m_pUser->m_xUWealth.SubWealth(UW_GOLD,nCakeCost[nCakeFlag-10]))
			return false;
	}

	if (pObj->m_pBase->byType == ITEMTYPE_SKILLBOOK)
	{//怒气技能书升级技能
		int nSkillID = pObj->m_pBase->nDura;
		int nLearnLv = pObj->m_pBase->byLv;
		if (nLearnLv > 1)
		{
			if (m_pUser->CheckSkillLvUp(nSkillID,nLearnLv))
				RemoveItemByIndex(pObj->GetIndex());

			return true;
		}
	}

	m_pUser->m_pUserCD->LockCD(pObj->m_pBase->wCDPos,pObj->m_pBase->dwCDTime/1000);

	if (pObj->m_pBase->nUseType)
	{
		_useItemEff(pObj);
	}
	if (pObj->m_pBase->nTypeID == 45011&&!m_pUser->m_pTurntableCenter->AddFreeTimes())
	{//幸运符
		return false;
	}else if (pObj->m_pBase->nTypeID == 45011)
	{
		RemoveItemByIndex(pObj->GetIndex());
		return true;
	}

	if (pObj->m_pBase->nTypeID == 45012&&!g_pGameCenter->m_pWarRankCenter->AddWarRankTimesByUse(m_pUser))
	{//竞技令
		return false;
	}else if (pObj->m_pBase->nTypeID == 45012)
	{
		RemoveItemByIndex(pObj->GetIndex());
		return true;
	}

	ItemExBase* pBaseTemp = pObj->m_pBase;
	if (pObj->m_pBase->GetItemFlag(ITEM_FLAG_COST))
	{
		RemoveItemByIndex(pObj->GetIndex());
	}
	if (pBaseTemp->nScrpitFlag & 1<<ITEM_SCRPITFLAG_ONUSE)
	{
		LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
		//pLuaState->DoFile(pObj->m_pBase->szLuaName);
		char sztxt[64];
		sprintf(sztxt,"%s%d",szLuaItemFlag_OnUse,pBaseTemp->nTypeID);
		LuaObject loFunction = pLuaState->GetGlobal(sztxt);
		if(!loFunction.IsFunction()){
			return true;
		}
		// 				LuaObject lTable = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetScriptClass((ItemCenter*)0)->GetMetaTable();
		// 
		int psss = NULL; 
		// 				LuaObject ltemp = lTable["AddItemByID"];
		// 				bool btemp = ltemp.IsFunction();
		// 				CLuaPlusSafeFunction_4<void,ItemCenter*,int ,int> func11;
		// 				LuaFunction<void> tlf(ltemp);
		// 				if(!func11(pLuaState, tlf, 0,this, 20000,1)){
		// 
		// 				}
		LuaFunction<int> lf(loFunction);

		//LuaObject a2aa = this->GetLuaObject().fun("GetBlankBag");
		CLuaPlusSafeFunction_3<int, ItemCenter*,ItemObj*> func;
		if(!func(pLuaState, lf, &psss, this,pObj))
		{
			g_pLogFile->OutPut("脚本函数%s执行错误",sztxt);
		}
	}
	
	return true;
}

bool ItemCenter::EquipTakeOn( ItemObj* pObj )
{
	if (!pObj)
	{
		return false;
	}
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	int nSlot = pObj->m_pBase->bySlot;
	if (nSlot>=EQUIP_MAX)
	{
		return false;
	}
	int nCurSlot	= pObj->GetDataVal(ITEM_AD_SLOT);
	if(nCurSlot == ITEM_SLOT_SELL || nCurSlot == ITEM_SLOT_DEPOT)
		return false;
	if (_checkEquip(pObj))
	{
		int nIndex = _getBlankBagIndex();
		if (nIndex == -1)
		{//没有空位
			m_pUser->SendWarnMsg("包裹没有足够空间，请解锁");
			return false;
		}else{
			_setItemPos(pObj,ITEM_SLOT_BAG,nIndex);
		}
	}else
	{
		ItemObj* pTemp = m_pEquip[nSlot];
		//int nPos = pObj->GetDataVal(ITEM_AD_POS);
		if (pTemp)
		{
			_exchangeItemPos(pTemp,pObj);
		}else{
			_setItemPos(pObj,EQUIP_INDEX_TO_SLOT(nSlot),0);

		}
		int nBind = pObj->GetDataVal(ITEM_AD_BIND);
		if(!nBind&&pObj->m_pBase->byBind == ITEM_BINDTYPE_BING_EQUIP) pObj->SetDataVal(ITEM_AD_BIND,1);
		//if(pTemp) _setItemPos(pTemp,ITEM_SLOT_BAG,nPos);
	}
	m_pUser->RecalcParam();
	m_pUser->m_pUserRecord->RunLuaEquipFunc(5,pObj,NULL);
	return true;
}

bool ItemCenter::_checkEquip( ItemObj* pObj )
{
	int nSite = pObj->GetDataVal(ITEM_AD_SLOT);
	return nSite<ITEM_SLOT_EQUIP_END&&nSite>ITEM_SLOT_EQUIP_START;
}

bool ItemCenter::_setItemPos(ItemObj* pObj,int nSite, int nPos,bool bSaveDB )
{
	if (nSite == ITEM_SLOT_BAG&&nPos>=m_nBagSize)
	{
		return false;
	}
	int nPreSite = pObj->GetDataVal(ITEM_AD_SLOT);
	int nPrePos = pObj->GetDataVal(ITEM_AD_POS);
	if (nPreSite>ITEM_SLOT_EQUIP_START&&nPreSite<ITEM_SLOT_EQUIP_END )
	{
		m_pEquip[EQUIP_SLOT_TO_INDEX(nPreSite)] = NULL;
	}
	if (nPreSite == ITEM_SLOT_BAG)
	{
		if(m_pItemInBag[nPrePos] == pObj)
			m_pItemInBag[nPrePos] = NULL;
	}
	if (nPreSite == ITEM_SLOT_DEPOT)
	{
		if(m_pItemInBank[nPrePos] == pObj)
			m_pItemInBank[nPrePos] = NULL;
	}
	pObj->SetDataValEx(ITEM_AD_SLOT,nSite,bSaveDB);
	pObj->SetDataValEx(ITEM_AD_POS,nPos,bSaveDB);
	if (nSite == ITEM_SLOT_BAG)
	{
		m_pItemInBag[nPos] = pObj;
	}else if (nSite>ITEM_SLOT_EQUIP_START&&nSite<ITEM_SLOT_EQUIP_END)
	{
		m_pEquip[EQUIP_SLOT_TO_INDEX(nSite)] = pObj;
	}else if (nSite == ITEM_SLOT_DEPOT)
	{
		m_pItemInBank[nPos] = pObj;
	}
	return true;
}

bool ItemCenter::ItemPacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != ITEM)
		return false;
	switch(pPacket->bCmd)
	{

	case C_USER_ITEM:
		{
			SET_DATA(pData, ITEM, C_USER_ITEM, pPacket);
			ItemObj* pObj = GetItemByIndex(pData->dwID);
			bool	bCost = false;

			ItemExBase* pTmpBase = NULL;
			if (pObj)
				pTmpBase = pObj->m_pBase;

			bool    bUseRlt = UseItem(pObj);

			if(bUseRlt && pTmpBase && pTmpBase->byType == ITEMTYPE_GIFT)
				m_pUser->PlayEffect(nEffectSuccess);
			
		}
		return true;
	case C_ITEM_FIXALL:
		{
			FixAllUsedEquip();
		}
		return true;
	case C_ITEM_QUICK_FIX:
		{
			FixAllUsedEquip(true);
		}
		return true;
	case C_ITEM_TEST:
		{
// 			AddItemByID(10126,5);
// 			AddItemByID(20010,100);
// 			AddItemByID(20020,100);
// 			AddItemByID(20030,100);
// 			AddItemByID(20040,100);
			AddItemByID(55555,1);

		}
		return true;
	case C_DESTROY_ITEM:
		{
			SET_DATA(pData, ITEM, C_USER_ITEM, pPacket);
			ItemObj* pObj = GetItemByIndex(pData->dwID);
			if (pObj&&!(pObj->m_pBase->nFlag&1<<ITEM_FLAG_DESTROY))
			{
				return true;
			}
			RemoveItemByIndex(pData->dwID, -1);
		}
		return true;
	case  C_MAKE_SKILL_ITEM:
		{
			SET_DATA(pData, ITEM, C_MAKE_SKILL_ITEM, pPacket);

			if(MakeSkillData::USE_MAKE_SKILL_SUCCESS == m_pUser->m_pMakeSkillData->UseMakeSkill(pData->nID)){
				TCP_SendCmdToUin(m_pUser->m_dwUin,ITEM,C_MAKE_SKILL_ITEM);
			}
			
		}
		return true;
	case  C_MAKE_SKILL_ITEM_RESULT:
		{
			SET_DATA(pData, ITEM, C_MAKE_SKILL_ITEM_RESULT, pPacket);
			
			m_pUser->m_pMakeSkillData->MakeSkillResult(pData->byResult);

		

		}
		return true;
	case  C_LEARN_MAKE_SKILL:
		{
			SET_DATA(pData, ITEM, C_LEARN_MAKE_SKILL, pPacket);
			int nRet = m_pUser->m_pMakeSkillData->LearnMakeSkill((MAKE_SKILL_TYPE)pData->byResult);
			if(nRet == MakeSkillData::LEARN_SKILL_SUCCESS)
				TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);

		}
		return true;
	case C_ITEM_BANK:
		{
			SET_DATA(pData, ITEM, C_ITEM_BANK, pPacket);
			ItemObj* pObj = GetItemByIndex(pData->dwID);
			if(pObj)
				SetItemBank(pObj);
		}
		return true;
	case C_ITEM_STARUP:
		{
			SET_DATA(pData, ITEM, C_ITEM_STARUP, pPacket);
			ItemObj* pObj = GetItemByIndex(pData->dwID);
			if(pObj)
				TryStarLvUp(pObj);
		}
		return true;
	case C_ITEM_BORE:
		{
			SET_DATA(pData, ITEM, C_ITEM_BORE, pPacket);
			ItemObj* pObj = GetItemByIndex(pData->dwID);
			if(pObj)
				TryAddStonePos(pObj);
		}
		return true;
	case C_ITEM_INLAY:
		{
			SET_DATA(pData, ITEM, C_ITEM_INLAY, pPacket);
			ItemObj* pEquip = GetItemByIndex(pData->dwEquip);
			ItemObj* pStone = GetItemByIndex(pData->dwStone);
			TryInlayStone(pEquip,pStone);
		}
		return true;
	case C_ITEM_REMOVE_STONE:
		{
			SET_DATA(pData, ITEM, C_ITEM_REMOVE_STONE, pPacket);
			ItemObj* pEquip = GetItemByIndex(pData->dwEquip);
			TryRemoveStone(pEquip,pData->nPos);
		}
		return true;
	case C_ITEM_COMPOSE_STONE:
		{
			SET_DATA(pData, ITEM, C_ITEM_COMPOSE_STONE, pPacket);
			TryComposeStone(pData->nID,pData->nPos);
		}
		return true;
	case C_ITEM_EAT_EQUIP:
		{
			SET_DATA(pData, ITEM, C_ITEM_EAT_EQUIP, pPacket);
			ItemObj* pEquip = GetItemByIndex(pData->nEquip);
			ItemObj* pEat = GetItemByIndex(pData->nEat);
			TryEatEquip(pEquip,pEat);
		}
		return true;
	case C_ITEM_TRANSFER_EQUIP:
		{
			SET_DATA(pData, ITEM, C_ITEM_TRANSFER_EQUIP, pPacket);
			ItemObj* pEquip = GetItemByIndex(pData->nEquip);
			ItemObj* pEat = GetItemByIndex(pData->nEat);
			TryTransferEquip(pEquip,pEat);
		}
		return true;
	case C_ITEM_MALL_BUY:
		{
			SET_DATA(pData, ITEM, C_ITEM_MALL_BUY, pPacket);
			TryBuyMallItem(pData->byType,pData->nItemID,pData->nCount);
		}
		return true;
	case C_ITEM_SHOP_BUY:
		{
			SET_DATA(pData, ITEM, C_ITEM_SHOP_BUY, pPacket);
			TryBuyShopItem(pData->nType,pData->nItemID,pData->nCount);
		}
		return true;
	case C_ITEM_SELL:
		{
			SET_DATA(pData, ITEM, C_ITEM_SELL, pPacket);
			ItemObj* pItem = GetItemByIndex(pData->nItemID);
			TrySellItem(pItem);
		}
		return true;
	case C_ITEM_FASTSELL:
		{
			SET_DATA(pData, ITEM, C_ITEM_FASTSELL, pPacket);
			TryFastSell(pData->nType);
		}
		return true;
	case C_GET_ITEMINFO:
		{
			//return true;
			SET_DATA(pData, ITEM, C_GET_ITEMINFO, pPacket);
			ItemObj* pObj = NULL;
			bool bRelease = false;
			if (!pData->dwUin) 
			{
				pObj = g_pGameCenter->m_pItemManage->GetItemObj(pData->nItemID);
			}else{
				UserData* pUser = g_pGameCenter->GetPlayer(pData->dwUin);
				if (pUser)
				{
					pObj = pUser->m_pItemCenter->GetItemByIndex(pData->nItemID);
				}else{
					pObj = ItemManage::GetItemByIndex(pData->dwUin,pData->nItemID);
					bRelease = true;
				}
			}
			
			if (pObj)
			{
				pObj->SendItemInfo(m_pUser->m_dwUin,pData->nKey);
				if (bRelease)
				{
					delete pObj;
				}
			}
			
		}
		return true;
	case C_SORT_BAG:
		{
			SortBag();
		}
		return true;
	case C_ADD_BAGSIZE_BYRMB:
		{
			AddBagSizeByRMB();
		}
		return true;
	}
	return false;
}

void ItemCenter::CreateScriptClass( CLuaState * pLuaState )
{
	CLuaClass<ItemCenter> * pscItemCenter = pLuaState->CreateScriptClass((ItemCenter*)0);

	pscItemCenter->DefObjectFunction("GetBlankBag", &ItemCenter::GetBlankBag)
 		.DefObjectFunction("AddItemByID", &ItemCenter::AddItemByID)
 		.DefObjectFunction("RemoveItemByIndex", &ItemCenter::RemoveItemByIndex)
 		.DefObjectFunction("GetItemByIndex", &ItemCenter::GetItemByIndex)
		.DefObjectFunction("CheckItemBlankBagByID", &ItemCenter::CheckItemBlankBagByID)
		.DefObjectFunction("GetUser", &ItemCenter::GetUser)
		.DefObjectFunction("GetEquipByIndex", &ItemCenter::GetEquipByIndex)
		.DefObjectFunction("_addEquipStar", &ItemCenter::_addEquipStar)
		.DefObjectFunction("_addEquipStartemp", &ItemCenter::_addEquipStartemp)
		.DefObjectFunction("AddBagSize", &ItemCenter::_addBagSize)
		.DefObjectFunction("GetBagSize", &ItemCenter::GetBagSize)
		.DefObjectFunction("GetItemCountByID", &ItemCenter::GetItemCountByID);
}

void ItemCenter::ReleaseScriptClass( CLuaState * pLuaState )
{
	pLuaState->ReleaseScriptClass((ItemCenter*)0);
}

void ItemCenter::_addEquipEx( ItemObj* pObj )
{

	AmzDataBase* pAmzBase = m_pUser->m_pAmzDManager->AddAmzData(EQUIPEX_AD_MAX,AMZ_DATA_EQUIPEX,m_pUser->LockADataID());
	if(!pAmzBase) return;
	LONGLONG temp = m_pUser->m_dwUin;
	pAmzBase->llKey	= ((temp<< 32)|pAmzBase->nModelID);

	pAmzBase->SetVal(EQUIPEX_AD_ITEMID,pObj->GetDataVal(ITEM_AD_ID));
	pAmzBase->SetVal(EQUIPEX_AD_UIN,pObj->GetDataVal(ITEM_AD_UIN));
	pAmzBase->SetVal(EQUIPEX_AD_ID,pAmzBase->nModelID);
	pObj->m_pDataEx[ITEMEXDATA_EQUIPEX] = pAmzBase;

	int nRandomMax = 2;
	int nRandomCount = Random_Int(1,nRandomMax>pObj->m_pBase->pAddAttr->Count?pObj->m_pBase->pAddAttr->Count:nRandomMax);
	int nTempCount = pObj->m_pBase->pAddAttr->Count;
	int nNeedCount = nRandomCount;
	int nAddCount = 0;
	for (int i=0;i<pObj->m_pBase->pAddAttr->Count;i++)
	{
		int nRandom = Random_Int(0,nTempCount);
		if(nNeedCount>=nRandom){
			int nType = (int)pObj->m_pBase->pAddAttr->Items[i];
			nNeedCount--;
			EquipAttrBase* pBase = EquipAttrConfig::GetInstance().GetEquipAttr(nType);
			if(pBase){
				int nVal = 0;int nCount = 0;
				for (int j=0;j<EQUIP_ATTR_MAX;j++)
				{
					if(pBase->nBase[j]){
						int nRand = Random_Int(0,pBase->nRandom[j]+1);
						nVal |= (nRand<<(nCount*16));
						nCount++;
						if(nCount == 2) break;
					}
				}
				_setEquipExAttr(pObj,nAddCount,nType,nVal);
				nAddCount++;
			}
		}
		nTempCount--;
		if(!nTempCount||!nNeedCount) break;
	}
	pAmzBase->bStateFlag = AMZ_STATE_CREATE;
	
}

void ItemCenter::_setEquipExAttr( ItemObj* pObj,int nSite,int nType,int nVal )
{
	if (nSite == 0)
	{
		pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDTYPE1,nType);
		pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDRANDOM1,nVal);
	}else if (nSite == 1)
	{
		pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDTYPE2,nType);
		pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDRANDOM2,nVal);
	}
}

bool ItemCenter::MoveItemToPos( ItemObj* pObj,int nPos )
{
	return false;
}

bool ItemCenter::_exchangeItemPos( ItemObj* pObj1,ItemObj* pObj2,bool bSaveDB )
{
	int nTempSite = pObj1->GetDataVal(ITEM_AD_SLOT);
	int nTempPos = pObj1->GetDataVal(ITEM_AD_POS);

	pObj1->SetDataValEx(ITEM_AD_SLOT,pObj2->GetDataVal(ITEM_AD_SLOT),bSaveDB);
	pObj1->SetDataValEx(ITEM_AD_POS,pObj2->GetDataVal(ITEM_AD_POS),bSaveDB);
	pObj2->SetDataValEx(ITEM_AD_SLOT,nTempSite,bSaveDB);
	pObj2->SetDataValEx(ITEM_AD_POS,nTempPos,bSaveDB);

	int nSite = pObj2->GetDataVal(ITEM_AD_SLOT);
	if (nSite == ITEM_SLOT_BAG)
	{
		m_pItemInBag[pObj2->GetDataVal(ITEM_AD_POS)] = pObj2;
	}else if (nSite>ITEM_SLOT_EQUIP_START&&nSite<ITEM_SLOT_EQUIP_END)
	{
		m_pEquip[EQUIP_SLOT_TO_INDEX(nSite)] = pObj2;
	}else if (nSite == ITEM_SLOT_DEPOT)
	{
		m_pItemInBank[pObj2->GetDataVal(ITEM_AD_POS)] = pObj2;
	}

	nSite = pObj1->GetDataVal(ITEM_AD_SLOT);
	if (nSite == ITEM_SLOT_BAG)
	{
		m_pItemInBag[pObj1->GetDataVal(ITEM_AD_POS)] = pObj1;
	}else if (nSite>ITEM_SLOT_EQUIP_START&&nSite<ITEM_SLOT_EQUIP_END)
	{
		m_pEquip[EQUIP_SLOT_TO_INDEX(nSite)] = pObj1;
	}else if (nSite == ITEM_SLOT_DEPOT)
	{
		m_pItemInBank[pObj1->GetDataVal(ITEM_AD_POS)] = pObj1;
	}
	return true;
}

int ItemCenter::_getBlankBankIndex()
{
	for (int i=0;i<m_nBankSize;i++)
	{
		if(!m_pItemInBank[i]) return i;
	}
	return -1;
}

ItemObj* ItemCenter::GetPileItemInBagByID(int nID,bool bBind)
{
	for (int i = 0; i < m_nBagSize; i++)
	{
		if (m_pItemInBag[i]&&m_pItemInBag[i]->m_pBase->nTypeID == nID 
			&& m_pItemInBag[i]->GetDataVal(ITEM_AD_NUM) < m_pItemInBag[i]->m_pBase->nNum
			&& m_pItemInBag[i]->GetDataVal(ITEM_AD_BIND) == bBind)
		{
			return m_pItemInBag[i];
		}
	}
	return NULL;
}

ItemObj* ItemCenter::GetPileItemInBankByID(int nID,bool bBind)
{
	for (int i = 0; i < m_nBankSize; i++)
	{
		if (m_pItemInBank[i]&&m_pItemInBank[i]->m_pBase->nTypeID == nID 
			&& m_pItemInBank[i]->GetDataVal(ITEM_AD_NUM) < m_pItemInBank[i]->m_pBase->nNum
			&& m_pItemInBank[i]->GetDataVal(ITEM_AD_BIND) == bBind)
		{
			return m_pItemInBank[i];
		}
	}
	return NULL;
}

bool ItemCenter::PileItemBySite(ItemObj* pItem,int nSite)
{
	if (!pItem)
	{
		return false;
	}

	if (nSite == ITEM_SLOT_BAG)
	{
		ItemObj* pTmpItem = GetPileItemInBagByID(pItem->m_pBase->nTypeID,pItem->GetDataVal(ITEM_AD_BIND));
		if (pTmpItem)
		{
			int nRlt = _pileItem(pTmpItem,pItem);
			if (nRlt == 0 || nRlt == 1)
				return true;
		}
	}else if (nSite == ITEM_SLOT_DEPOT)
	{
		ItemObj* pTmpItem = GetPileItemInBankByID(pItem->m_pBase->nTypeID,pItem->GetDataVal(ITEM_AD_BIND));
		if (pTmpItem)
		{
			int nRlt = _pileItem(pTmpItem,pItem);
			if (nRlt == 0 || nRlt == 1)
				return true;
		}
	}
	return false;
}

bool ItemCenter::SetItemBank( ItemObj* pItem )
{
	if (!pItem)
	{
		return false;
	}
	if (pItem->GetDataVal(ITEM_AD_SLOT) == ITEM_SLOT_BAG)
	{
		if (PileItemBySite(pItem,ITEM_SLOT_DEPOT))
		{
			return true;
		}

		int nIndex = _getBlankBankIndex();
		if (nIndex == -1)
		{
			m_pUser->SendWarnMsg("仓库已存满，无法存入物品");
			return false;
		}
		_setItemPos(pItem,ITEM_SLOT_DEPOT,nIndex);
	}else if (pItem->GetDataVal(ITEM_AD_SLOT) == ITEM_SLOT_DEPOT)
	{
		if (PileItemBySite(pItem,ITEM_SLOT_BAG))
		{
			return true;
		}

		int nIndex = _getBlankBagIndex();
		if (nIndex == -1)
		{
			return false;
		}
		_setItemPos(pItem,ITEM_SLOT_BAG,nIndex);
	}
	return false;
}
char* szStarCountry[] = {"无","联盟","部落"};
char* szRreSz[] = {"无","太幸运啦","人品爆发","霸气四溢","所向披靡","战神降临"};
void ItemCenter::CheckSLvSysMsg(int nStarLv,ItemObj* pObj)
{
	int nPreSz = 0;
	if (nStarLv == 10)
		nPreSz = 1;
	if (nStarLv == 15)
		nPreSz = 2;
	if (nStarLv == 18)
		nPreSz = 3;
	if (nStarLv == 19)
		nPreSz = 4;
	if (nStarLv == 20)
		nPreSz = 5;
	if (nPreSz)
	{
		DWORD nItemIndex = pObj->GetIndex()*10+nPreSz;
		if (m_pSysMsgedList->Count)
		{
			for (int i = 0; i  < m_pSysMsgedList->Count; i++)
			{
				if ((DWORD)m_pSysMsgedList->Items[i] == nItemIndex)
					return;
			}
		}
		char szMsg[128];
		sprintf(szMsg,"%s！[%s]的[%s]竟然将装备[%s]炼星到%d星，谁也无法阻挡他前进的脚步！",szRreSz[nPreSz],szStarCountry[m_pUser->m_byCountry],m_pUser->m_szNick,pObj->m_pBase->szName,nStarLv);
		ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

		m_pSysMsgedList->Add((void*)nItemIndex);
	}
}

int ItemCenter::GetStarState()
{
	int nAllStarCnt = 0;
	for (int i=0;i<EQUIP_MAX;i++)
	{
		if (!m_pEquip[i])
			continue;
		int nLv = m_pEquip[i]->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV)<<24>>24;
		if (!nLv)
			continue;
		nAllStarCnt += nLv;
	}
	if (nAllStarCnt > 179)
		return 7004;
	else if (nAllStarCnt > 149)
		return 7003;
	else if (nAllStarCnt > 119)
		return 7002;
	else if (nAllStarCnt > 89)
		return 7001;
	else
		return 0;
}

int ItemCenter::GetStoneState()
{
	int nLvSevenCnt = 0;
	int nLvEightCnt = 0;
	int nLvNineCnt = 0;
	int nLvTenCnt = 0;
	for (int j = 0; j < EQUIP_MAX; j++)
	{
		if (!m_pEquip[j])
			continue;
		for (int i=0;i<nMaxStoneCount;i++)
		{
			int nStoneLv = _getEquipStoneByPos(m_pEquip[j],i)?_getEquipStoneByPos(m_pEquip[j],i)->nTypeID:0;
			if (!nStoneLv)
			{
				continue;
			}
			if (nStoneLv%10 > 5)
				nLvSevenCnt++;
			if (nStoneLv%10 > 6)
				nLvEightCnt++;
			if (nStoneLv%10 > 7)
				nLvNineCnt++;
			if (nStoneLv%10 > 8)
				nLvTenCnt++;
		}
	}
	if (nLvTenCnt > 35)
		return 7015;
	else if (nLvNineCnt > 29)
		return 7014;
	else if (nLvNineCnt > 19)
		return 7013;
	else if (nLvEightCnt > 19)
		return 7012;
	else if (nLvSevenCnt > 19)
		return 7011;
	else if (nLvSevenCnt > 9)
		return 7010;
	else
		return 0;
}

void ItemCenter::_addEquipStar( ItemObj* pObj,bool bSuccess )
{
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return;
	}
	int nLv = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV);
	int nStar = nLv<<24>>24;
	int nExStar = nLv>>8;
	EquipStarBase* pBase = EquipStarConfig::GetInstance().GetEquipStar(nLv+1);
	int nRandom = Random_Int(0,100);
	if (bSuccess||nRandom<pBase->nSuccess)
	{
		//成功强化
		nStar++;

		CheckSLvSysMsg(nStar,pObj);

		if (pBase->nExCount!=-1)
		{
			nRandom = Random_Int(0,10000);
			if(nRandom<EquipStarConfig::GetInstance().aExSuccess[pBase->nExCount]) nExStar|=1<<pBase->nExCount;
		}
		pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV,(nExStar<<8|nStar));
		pObj->RecalcParam();

		m_pUser->PlayEffect(nEffectSuccess);
		m_pUser->m_pUserRecord->RunLuaEquipFunc(2,pObj,NULL);
		_MuliParamObj* pTemp = new _MuliParamObj();
		pTemp->nParam1 = m_pUser->m_dwUin;
		pTemp->nParam2 = pObj->GetIndex();
		g_pGameCenter->m_pRankCenter->CheckRank(RANK_EQUIP,(void*)pTemp,false);
		m_pUser->m_pUserRecord->AddEquipStarLogEx(pObj->GetItemID(),1,nStar);
	}else{
		if(nStar >1)nStar--;
		pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV,(nExStar<<8|nStar));
		pObj->RecalcParam();
		m_pUser->PlayEffect(nEffectFail);

		m_pUser->m_pUserRecord->AddEquipStarLogEx(pObj->GetItemID(),-1,nStar);
	}
	pObj->SetDataVal(ITEM_AD_BIND,1);
	m_pUser->m_pUserRecord->AddRecord(UA_OPREATE,UO_ITEM,pObj->GetItemID(),1);
}

bool ItemCenter::TryStarLvUp( ItemObj* pObj )
{
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	int nStar = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV)<<24>>24;
	EquipStarBase* pBase = EquipStarConfig::GetInstance().GetEquipStar(nStar+1);
	if (!pBase)
	{
		return false;
	}
	if (m_pUser->m_xUWealth.GetWealth(UW_GOLD)<pBase->nGold)
	{
		return false;
	}
	if (!RemoveItemByID(EquipStarConfig::GetInstance().nItemID,EquipStarConfig::GetInstance().nItemCount,1<<ITEM_SLOT_BAG))
	{
		return false;
	}
	if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,pBase->nGold))
	{
		return false;
	}
	
	_addEquipStar(pObj);
	m_pUser->m_pUserRecord->AddEquipStarLog(pObj->GetItemID());
	return true;
}

bool ItemCenter::TryAddStonePos( ItemObj* pObj )
{
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	if (!(pObj->m_pBase->GetItemFlag(ITEM_FLAG_BORE)))
	{
		return false;
	}
	int nPos = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
	int nNowPos = 0;
	for (;nNowPos<(sizeof(int)*8);nNowPos++)
	{
		if (!(nPos&(1<<nNowPos))) break;
	}
	if (nNowPos>=nMaxStoneCount)
	{
		return false;
	}
	if (m_pUser->m_xUWealth.GetWealth(UW_GOLD)<nItemBoreCostMoney[nNowPos])
	{
		return false;
	}
	if (!RemoveItemByID(nStoneCostItem,nItemBoreCostCount[nNowPos],1<<ITEM_SLOT_BAG))
	{
		return false;
	}
	if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nItemBoreCostMoney[nNowPos]))
	{
		return false;
	}
	nPos |= (1<<nNowPos);
	pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS,nPos);
	pObj->SetDataVal(ITEM_AD_BIND,1);
	m_pUser->PlayEffect(nEffectSuccess);
	m_pUser->m_pUserRecord->AddRecord(UA_OPREATE,UO_ITEM,pObj->GetItemID(),2);
	return true;
}

bool ItemCenter::TryInlayStone( ItemObj* pObj ,ItemObj* pStone)
{
	if (!pObj||!pStone)
	{
		return false;
	}
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP||pStone->m_pBase->byType != ITEMTYPE_STONE)
	{
		return false;
	}
	if (!(pObj->m_pBase->GetItemFlag(ITEM_FLAG_INLAY)))
	{
		return false;
	}
	int nPos = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
	int nNowPos = 0;
	int nEmpty = -1;
	for (;nNowPos<(sizeof(int)*8);nNowPos++)
	{
		if (!(nPos&(1<<nNowPos)))
		{
			if (nEmpty!=-1)
			{
				nNowPos = nEmpty;
				break;
			}
			else
				return false;
		}
		else{
			ItemExBase* pBase = _getEquipStoneByPos(pObj,nNowPos);
			if(!pBase)
			{
				if (nEmpty == -1)
					nEmpty = nNowPos;
				continue;
			}
			if (_checkStoneConflict(pBase,pStone->m_pBase))
			{
				m_pUser->SendWarnMsg("已有同类型宝石了");
				return false;
			}
		}
	}

	if (nNowPos>=nMaxStoneCount)
	{
		return false;
	}
	int nStoneID = pStone->GetItemID();

	if(GetItemByIndex(pStone->GetDataVal(ITEM_AD_ID))==NULL)
		return false;

	_inlayStone(pObj,nStoneID,nNowPos);
	m_pUser->PlayEffect(nEffectSuccess);
	m_pUser->m_pUserRecord->AddRecord(UA_OPREATE,UO_ITEM,pObj->GetItemID(),3);
	m_pUser->m_pUserRecord->RunLuaEquipFunc(3,pObj,pStone);
	_MuliParamObj* pTemp = new _MuliParamObj();
	pTemp->nParam1 = m_pUser->m_dwUin;
	pTemp->nParam2 = pObj->GetIndex();
	g_pGameCenter->m_pRankCenter->CheckRank(RANK_EQUIP,(void*)pTemp,false);
	RemoveItemByIndex(pStone->GetDataVal(ITEM_AD_ID));
	return true;
}

ItemExBase* ItemCenter::_getEquipStoneByPos( ItemObj* pObj,int nPos )
{
	return pObj->_getEquipStoneByPos(nPos);
}

bool ItemCenter::_checkStoneConflict( ItemExBase* pBase1,ItemExBase* pBase2 )
{
	if (!pBase1||!pBase2)
	{
		return false;
	}
	
	for (int i=0;i<EQUIP_ATTR_MAX;i++)
	{
		if (pBase1->nParam[i]&&pBase2->nParam[i])
		{
			return true;
		}
	}
	return false;
}

bool ItemCenter::_inlayStone( ItemObj* pObj,int nItemID,int nPos )
{
	int nType = 0;
	if (nPos/2 == 0)
	{
		nType = EQUIPEX_AD_STONE12;
	}else if (nPos/2 == 1)
	{
		nType = EQUIPEX_AD_STONE34;
	}else{
		return false;
	}
	int nID = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,nType);
	if (nPos%2 == 0)
	{
		nID = (nID>>16<<16)|nItemID;
	}else{
		nID = (nID<<16>>16)|(nItemID<<16);
	}
	pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,nType,nID);
	pObj->SetDataVal(ITEM_AD_BIND,1);
	pObj->RecalcParam();
	return true;
}

bool ItemCenter::TryRemoveStone( ItemObj* pObj,int nPos )
{
	if (!pObj)
	{
		return false;
	}
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	if (GetBlankBag()<1)
	{
		_sendBlankBagRlt();
		return false;
	}
	int nStonePos = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS);
	if (nStonePos&(1<<nPos))
	{
		ItemExBase* pBase = _getEquipStoneByPos(pObj,nPos);
		if(!pBase){
			return false;
		}
		int nCost = pBase->nTypeID%10*2500+5000;
		if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nCost))
		{
			return false;
		}
		if (_removeStone(pObj,nPos))
		{
			AddItemByID(pBase->nTypeID,1,true,ITEM_LOG_REMOVESTONE);
		}
	}
	return true;
}

bool ItemCenter::_removeStone( ItemObj* pObj,int nPos )
{
	int nType = 0;
	if (nPos/2 == 0)
	{
		nType = EQUIPEX_AD_STONE12;
	}else if (nPos/2 == 1)
	{
		nType = EQUIPEX_AD_STONE34;
	}else{
		return false;
	}
	int nID = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,nType);
	if (nPos%2 == 0)
	{
		nID = (nID>>16<<16);
	}else{
		nID = (nID<<16>>16);
	}
	pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,nType,nID);
	pObj->RecalcParam();
	return true;
}

bool ItemCenter::TryComposeStone( int nID,int nCount )
{
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nID);
	if (!pBase)
	{
		return false;
	}
	if (GetItemCountByID(nID)<nCount*nStoneComposeCount)
	{
		return false;
	}
	ItemExBase* pBaseNext = ItemConfig::GetInstance().GetItemBase(pBase->nDura);
	if (!pBaseNext)
	{
		return false;
	}
	if ((nCount/pBaseNext->nNum+1)>GetBlankBag())
	{
		_sendBlankBagRlt();
		return false;
	}
	if (nCount<=0)
	{
		return false;
	}
	if (!pBase->nDura||pBase->byType != ITEMTYPE_STONE)
	{
		return false;
	}
	if (m_pUser->m_xUWealth.GetWealth(UW_GOLD)<nCount*nStoneComposeMoney)
	{
		return false;
	}
	int nBindCnt = GetItemCountByIDEx(nID,1<<ITEM_SLOT_BAG,1);
	int nUnBindCnt = GetItemCountByIDEx(nID,1<<ITEM_SLOT_BAG,0);
	if (nBindCnt +nUnBindCnt < nCount*nStoneComposeCount)
	{
		return false;
	}
	if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nCount*nStoneComposeMoney))
	{
		return false;
	}
	int nGetUnBindCnt = nUnBindCnt/nStoneComposeCount;
	if (nGetUnBindCnt>=nCount)
	{
		if(RemoveItemByIDEx(nID,nCount*nStoneComposeCount,1<<ITEM_SLOT_BAG,0,0))
			AddItemByID(pBase->nDura,nCount,0,ITEM_LOG_COMPSTONE);
	}else{
		if(nGetUnBindCnt&&RemoveItemByIDEx(nID,nGetUnBindCnt*nStoneComposeCount,1<<ITEM_SLOT_BAG,0,0))
			AddItemByID(pBase->nDura,nGetUnBindCnt,0,ITEM_LOG_COMPSTONE);
		if(RemoveItemByID(nID,(nCount-nGetUnBindCnt)*nStoneComposeCount))
			AddItemByID(pBase->nDura,nCount-nGetUnBindCnt,1,ITEM_LOG_COMPSTONE);
	}
	int nStoneLv = nID%10;
	if (nStoneLv >= 5)
	{
		char sztxt[128];
		if (nStoneLv == 5)
		{
			sprintf(sztxt,"天生异象，秘宝现世！%s手中的%s光芒大作，最终平静了下来!",m_pUser->GetName(),pBaseNext->szName);
		}else if (nStoneLv == 6)
		{
			sprintf(sztxt,"电闪雷鸣，狂风肆虐！%s手中的%s光芒大作，最终平静了下来!",m_pUser->GetName(),pBaseNext->szName);
		}else if (nStoneLv == 7)
		{
			sprintf(sztxt,"万兽奔腾，虎啸龙吟！%s手中的%s光芒大作，最终平静了下来!",m_pUser->GetName(),pBaseNext->szName);
		}else if (nStoneLv == 8)
		{
			sprintf(sztxt,"祥云浮空，圣光耀世！%s手中的%s光芒大作，最终平静了下来!",m_pUser->GetName(),pBaseNext->szName);
		}else if (nStoneLv == 9)
		{
			sprintf(sztxt,"天地变色，日月无光！%s手中的%s光芒大作，照亮了整个艾泽拉斯大陆!",m_pUser->GetName(),pBaseNext->szName);
		}
		ChatCenter::GetInstance().SendSystemMsg(sztxt,0,0,1);
	}
	
	while(nCount--){
		m_pUser->m_pUserRecord->AddRecord(UA_OPREATE,UO_ITEM,pBase->nDura,5);
	}
	return true;
}

bool ItemCenter::TryEatEquip( ItemObj* pObj,ItemObj* pEat )
{
	if (!pObj||!pEat)
	{
		m_pUser->SendWarnMsg("物品失效");
		return false;
	}
	if (!pEat->m_pBase->nStar)
	{
		return false;
	}
	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP||pEat->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	if (pEat == pObj)
	{
		return false;
	}
	int nVal = pObj->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV);
	int nLv = nVal<<24>>24;
	int nExp = nVal>>8;
	EquipEatBase* pBase = EquipEatConfig::GetInstance().GetEquipEat(nLv+1);
	if (!pBase)
	{
		m_pUser->SendWarnMsg("配置失效");
		return false;
	}
	int nStar = pEat->m_pBase->nStar;
	if (!RemoveItemByIndex(pEat->GetDataVal(ITEM_AD_ID)))
	{
		return false;
	}
	nExp += EquipEatConfig::GetInstance().m_nEatEquipVal[nStar];
	while(nExp>=pBase->exp){
		nExp -= pBase->exp;
		nLv++;
		pBase = EquipEatConfig::GetInstance().GetEquipEat(nLv+1);
		m_pUser->PlayEffect(28);
		if(!pBase) break;
	}
	pObj->SetDataVal(ITEM_AD_BIND,1);
	pObj->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV,nExp<<8|nLv);
	pObj->RecalcParam();
	m_pUser->m_pUserRecord->AddRecord(UA_OPREATE,UO_ITEM,pObj->GetItemID(),4);
	m_pUser->m_pUserRecord->RunLuaEquipFunc(4,pObj,NULL);
	_MuliParamObj* pTemp = new _MuliParamObj();
	pTemp->nParam1 = m_pUser->m_dwUin;
	pTemp->nParam2 = pObj->GetIndex();
	g_pGameCenter->m_pRankCenter->CheckRank(RANK_EQUIP,(void*)pTemp,false);
	return true;
}

bool ItemCenter::TryTransferEquip( ItemObj* pEquip,ItemObj* pEat )
{
	if (!pEquip||!pEat)
	{
		return false;
	}
	if (pEat == pEquip)
	{
		return false;
	}
	if (pEquip->m_pBase->byType != ITEMTYPE_EQUIP||pEat->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	if (m_pUser->m_xUWealth.GetWealth(UW_GOLD)<nTransferMoney)
	{
		return false;
	}
	if (!RemoveItemByID(nTransferItemID,pEat->GetTransferCost()))
	{
		return false;
	}
	if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nTransferMoney))
	{
		return false;
	}
	pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_LV));
	//pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDTYPE1,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDTYPE1));
	//pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDRANDOM1,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDRANDOM1));
	//pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDTYPE2,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDTYPE2));
	//pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDRANDOM2,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_ADDRANDOM2));
	pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONEPOS));
	pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE12,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE12));
	pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE34,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_STONE34));
	pEquip->SetExDataVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV,pEat->GetDataExVal(ITEMEXDATA_EQUIPEX,EQUIPEX_AD_EATLV));
	pEquip->SetDataVal(ITEM_AD_BIND,1);
	if (!RemoveItemByIndex(pEat->GetDataVal(ITEM_AD_ID)))
	{
		return false;
	}
	pEquip->RecalcParam();
	UseItem(pEquip);
	m_pUser->PlayEffect(nEffectSuccess);
	TCP_SendCmdToUin(m_pUser->m_dwUin,ITEM,C_ITEM_TRANSFER_EQUIP);
	return true;
}

bool ItemCenter::TryBuyMallItem( BYTE byType,int nItemID,int nCount )
{
	MallItemCfg* pCfg = MallConfig::GetInstance().GetCfg(byType,nItemID);
	if (!pCfg)
	{
		return false;
	}
	if (nCount<=0)
	{
		return false;
	}else if (nCount>1000)
	{
		return false;
	}
	if (!CheckItemBlankBagByID(nItemID,nCount))
	{
		_sendBlankBagRlt();
		return false;
	}
	int nTotalCost = nCount*pCfg->m_nPrice*pCfg->m_nDiscount/100;
	if (byType!=6&&m_pUser->m_xUWealth.GetWealth(UW_RMB)<nTotalCost)
	{
		return false;
	}else if (byType == 6&&m_pUser->m_xUWealth.GetWealth(UW_BINDRMB)<nTotalCost)
	{
		return false;
	}
	if(pCfg->m_nLimit == -1)
	{
		return false;
	}
	if (pCfg->m_nFlag != -1)
	{
		int nTime = m_pUser->m_pUserFlag->GetSLimit(pCfg->m_nFlag);
		if (nTime >= pCfg->m_nLimit||nTime+nCount>pCfg->m_nLimit)
		{
			return false;
		}
	}

	if (byType!=6&&!m_pUser->m_xUWealth.SubWealth(UW_RMB,nTotalCost,RMB_SAVE_SHOPBUY))
	{
		return false;
	}
	if (byType == 6 &&!m_pUser->m_xUWealth.SubWealth(UW_BINDRMB,nTotalCost,RMB_SAVE_SHOPBUY))
	{
		return false;
	}
	//flag
	if (pCfg->m_nFlag != -1)
	{
		int nTime = m_pUser->m_pUserFlag->GetSLimit(pCfg->m_nFlag);
		m_pUser->m_pUserFlag->SetSLimit(pCfg->m_nFlag,nTime+nCount);
	}

	if(!AddItemByID(nItemID,nCount,byType == 6,ITEM_LOG_MALL)){
		return false;
	}
	g_pMarkGameDB->AddMarkCost(nItemID,nCount);
	return true;
}

bool ItemCenter::CheckItemBlankBagByID( int nItemID,int nCount )
{
	ItemExBase* pItemBase = ItemConfig::GetInstance().GetItemBase(nItemID);
	if (!pItemBase)
	{
		return false;
	}
	if(nCount<1){
		return false;
	}
	if (pItemBase->byType == ITEMTYPE_EQUIP)
	{
		return GetBlankBag()>=nCount;
	}else{
		return GetBlankBag()>=(((nCount-1)/pItemBase->nNum)+1);
	}
}

bool ItemCenter::TryBuyShopItem( int nType,int nItemID,int nCount )
{
	ShopItemCfg* pCfg = ShopConfig::GetInstance().GetCfg(nType,nItemID);
	if (!pCfg)
	{
		return false;
	}
	if (nCount<=0)
	{
		return false;
	}else if (nCount>1000)
	{
		return false;
	}
	if (!CheckItemBlankBagByID(nItemID,nCount))
	{
		_sendBlankBagRlt();
		return false;
	}
	int nTotalCost = nCount*pCfg->m_nPrice;
	switch(pCfg->m_byType){
	case SHOP_TYPE_MONEY:
		{
			if (m_pUser->m_xUWealth.GetWealth(UW_GOLD)<nTotalCost)
			{
				return false;
			}
			if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nTotalCost))
			{
				return false;
			}
		}
		break;
	case SHOP_TYPE_WARRANK:
		{
			FortData* pData = m_pUser->GetFortData();
			if (!pData||pData->m_pWarRankData->nRankScore<nTotalCost)
			{
				return false;
			}
			pData->m_bShopWarRank = true;
			pData->AddWarScore(-nTotalCost);
		}
		break;
	default:
		return false;
	}
	return AddItemByID(nItemID,nCount,pCfg->m_byType == SHOP_TYPE_WARRANK,ITEM_LOG_SHOP);
	return true;
}

bool ItemCenter::TrySellItem( ItemObj* pObj )
{
	if (!pObj)
	{
		return false;
	}
	if (!pObj->m_pBase->GetItemFlag(ITEM_FLAG_SELL))
	{
		return false;
	}
	if (!pObj->m_pBase->GetItemFlag(ITEM_FLAG_DESTROY))
	{
		return false;
	}
	int nCount = pObj->m_pBase->byType == ITEMTYPE_EQUIP?1:pObj->GetDataVal(ITEM_AD_NUM);
	int nGetMoney = nCount*pObj->m_pBase->nPrice;
	//_removeItem(pObj);
	if (pObj->GetDataVal(ITEM_AD_SLOT) == ITEM_SLOT_SELL)
	{
		if (!GetBlankBag())
		{
			_sendBlankBagRlt();
			return false;
		}
		if (!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nGetMoney))
		{
			m_pUser->SendPopMsg("金币不足");
			return false;
		}
		_removeSellItem( pObj );

	}else if (_addSellItem(pObj))
	{
		m_pUser->m_xUWealth.AddWealth(UW_GOLD,nGetMoney);
	}
	return true;
}

bool ItemCenter::_addSellItem( ItemObj* pObj )
{
	if (m_listSellItem.size()>=16)
	{
		ItemObj* pObj = m_listSellItem.front();
		m_listSellItem.erase(m_listSellItem.begin());
		_removeItem(pObj);
	}
	int nIndex = m_listSellItem.size()?(m_listSellItem.back()->GetDataVal(ITEM_AD_POS)+1):0;
	_setItemPos(pObj,ITEM_SLOT_SELL,nIndex);
	m_listSellItem.push_back(pObj);
	return true;
}

bool ItemCenter::_removeSellItem( ItemObj* pObj ,bool bDelete)
{
	for (list<ItemObj*>::iterator itor = m_listSellItem.begin();itor!=m_listSellItem.end();itor++)
	{
		if (pObj == (*itor))
		{
			int nIndex = _getBlankBagIndex();
			if (nIndex == -1)
			{
				return false;
			}
			m_listSellItem.erase(itor);
			if(!bDelete) _setItemPos(pObj,ITEM_SLOT_BAG,nIndex);
			return true;
		}
	}
	return false;
}

bool ItemCenter::TryFastSell( int nType )
{
	for (int i=0;i<m_nBagSize;i++)
	{
		if(m_pItemInBag[i]&&m_pItemInBag[i]->m_pBase->byType == ITEMTYPE_EQUIP&&((1<<m_pItemInBag[i]->m_pBase->nStar)&nType)){
			TrySellItem(m_pItemInBag[i]);
		}
	}
	return true;
}

bool ItemCenter::CheckReplaceItem( int nIndex )
{
	ItemReplaceTag* pTag = ItemReplaceConfig::GetInstance().GetItemReplaceTag(nIndex);
	if (!pTag)
	{
		return false;
	}
	for (int i=0;i<pTag->pListCost->Count;i++)
	{
		CommonItemTag* pItem = (CommonItemTag*)pTag->pListCost->Items[i];
		if (!GetItemCountByID(pItem->nItemID)>=pItem->nItemCount)
		{
			return false;
		}
	}
	if ((pTag->nType == 0?pTag->pListGet->Count:1)>GetBlankBag())
	{
		_sendBlankBagRlt();
		return false;
	}
	return true;
}

bool ItemCenter::ReplaceItem( int nIndex )
{
	ItemReplaceTag* pTag = ItemReplaceConfig::GetInstance().GetItemReplaceTag(nIndex);
	if (!pTag)
	{
		return false;
	}
	for (int i=0;i<pTag->pListCost->Count;i++)
	{
		CommonItemTag* pItem = (CommonItemTag*)pTag->pListCost->Items[i];
		RemoveItemByID(pItem->nItemID,pItem->nItemCount);
	}
	if (pTag->nType == 0)
	{
		for (int i=0;i<pTag->pListGet->Count;i++)
		{
			CommonItemTag* pItem = (CommonItemTag*)pTag->pListGet->Items[i];
			AddItemByID(pItem->nItemID,pItem->nItemCount);
		}
	}else{
		int nRand = Random_Int(0,10000);
		for (int i=0;i<pTag->pListGet->Count;i++)
		{
			CommonItemTag* pItem = (CommonItemTag*)pTag->pListGet->Items[i];
			if (nRand>pItem->nPer)
			{
				nRand -= pItem->nPer;
				continue;
			}
			AddItemByID(pItem->nItemID,pItem->nItemCount);
			break;
		}
	}

	return true;
}

bool ItemCenter::_useItemEff( ItemObj* pObj )
{
	switch (pObj->m_pBase->nUseType)
	{
	case ITEM_USE_ADDHP:
		{
			m_pUser->ChgHp(pObj->m_pBase->nUseVal);
		}
		break;
	case ITEM_USE_ADDMP:
		{
			m_pUser->ChgMp(pObj->m_pBase->nUseVal);
		}
		break;
	case ITEM_USE_ADDSTATE:
		{
			m_pUser->AddState(pObj->m_pBase->nUseVal,false);
		}
		break;
	case ITEM_USE_ADDPOW:
		{
			m_pUser->m_xUWealth.AddWealth(UW_POW,10);
			//RemoveItemByIndex(pObj->GetIndex());
		}
		break;
	case ITEM_USE_ADDPHP:
		{
			if(m_pUser->m_pMObjCenter->m_pSlave)
			{
				m_pUser->m_pMObjCenter->HealthHpMp(m_pUser->m_pMObjCenter->m_pSlave,pObj->m_pBase->nUseVal,0);
			}
		}
		break;
	}
	return true;
}

void ItemCenter::SortBag()
{
	PileBag();
	PileBagByID();
	
	int nIndex = _getBlankBagIndex();
	if (nIndex == -1)
	{
		return;
	}
	for (int i=0;i<m_nBagSize;i++)
	{
		if (m_pItemInBag[i])
		{
			if(m_pItemInBag[i]->GetDataVal(ITEM_AD_POS)>nIndex){
				_setItemPos(m_pItemInBag[i],ITEM_SLOT_BAG,nIndex,true);
				nIndex = _getBlankBagIndex();
			}
		}
	}
}

bool ItemCenter::TryResolveEquip( ItemObj* pEquip )
{
	if (!pEquip)
	{
		return false;
	}
	if (pEquip->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return false;
	}
	int nType = EquipResolveConfig::GetInstance().GetLvType(pEquip->m_pBase->byLv);
	if (!nType)
	{
		return false;
	}
	if (!RemoveItemByIndex(pEquip->GetDataVal(ITEM_AD_ID)))
	{
		return false;
	}
	int nAdd =1;
	if(pEquip->m_pBase->nStar == 0&&Random_Int(0,100)<=5)
	{
		nAdd = 2;
	}else if (pEquip->m_pBase->nStar == 1&&Random_Int(0,100)<=10)
	{
		nAdd = 2;
	}else if (pEquip->m_pBase->nStar == 2&&Random_Int(0,100)<=20)
	{
		nAdd = 2;
	}else if (pEquip->m_pBase->nStar == 3&&Random_Int(0,100)<=30)
	{
		nAdd = 2;
	}else if (pEquip->m_pBase->nStar == 3&&Random_Int(0,100)<=10)
	{
		nAdd = 3;
	}
	int nRType = Random_Int(0,9);
	if (nRType<=4)
	{
		AddItemByID(EquipResolveConfig::GetInstance().nClothID,nAdd*EquipResolveConfig::GetInstance().nCloth[pEquip->m_pBase->nStar][nType]);
	}else{
		AddItemByID(EquipResolveConfig::GetInstance().nIronID,nAdd*EquipResolveConfig::GetInstance().nCloth[pEquip->m_pBase->nStar][nType]);
	}
}

bool ItemCenter::_addBagSize( int nSize )
{
	if (m_nBagSize>=100)
	{
		return false;
	}
	int nNewSize = nSize+m_nBagSize;
	if (nNewSize>100)
	{
		nNewSize = 100;
	}
	SetBagSize(nNewSize);
	m_pUser->m_pUserRecord->RunLuaWealthFunc(5,nSize,m_nBagSize);
	m_pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set bagsize=%d where uin=%d",nNewSize,m_pUser->m_dwUin);
	_sendBagSize();
	return true;
}

void ItemCenter::_sendBagSize()
{
	BuildPacketEx(pPacket, ITEM, S_SEND_BAGSIZE, buf, 1024);
	SET_DATA(pSend, ITEM, S_SEND_BAGSIZE, pPacket);
	pSend->nBagsize = m_nBagSize;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void ItemCenter::_sendBlankBagRlt()
{
	TCP_SendCmdToUin(m_pUser->m_dwUin,ITEM,S_SEND_BLANKBAGRLT);
}

void ItemCenter::SendToClient()
{
	_sendBagSize();
}

void ItemCenter::FixEquip( ItemObj* pObj,bool bRmb /*= false*/ )
{
	if (!pObj)
	{
		return;
	}

	if (pObj->m_pBase->byType != ITEMTYPE_EQUIP)
	{
		return;
	}
	int nPreDura = pObj->GetDataVal(ITEM_AD_NUM);
	int nChg = pObj->m_pBase->nDura-nPreDura;
	if (nChg<=0)
	{
		return;
	}
	if (!bRmb&&!m_pUser->m_xUWealth.SubWealth(UW_GOLD,nChg*75))
	{
		m_pUser->SendWarnMsg("金币不足！");
		return;
	}
	pObj->SetDataVal(ITEM_AD_NUM,pObj->m_pBase->nDura);
	m_pUser->SendWarnMsg("修理成功！");
	if (!nPreDura)
	{
		pObj->RecalcParam();
	}
}

void ItemCenter::FixAllUsedEquip(bool bRmb/* = false*/)
{
	if (bRmb&&!m_pUser->m_xUWealth.SubWealth(UW_RMB,10,RMB_SAVE_EQUIP))
		return;

	for (int i=0;i<EQUIP_MAX;i++)
	{
		if (m_pEquip[i])
		{
			FixEquip(m_pEquip[i],bRmb);
		}
	}
}

void ItemCenter::OnItemScrpit( ItemObj* pObj,BYTE byType )
{
	if (pObj->m_pBase->nScrpitFlag & 1<<byType)
	{
		return;
	}
	LuaState * pLuaState = pLuaStateManager->GetLuaState(GetCurrentThreadId())->GetLuaState();
	pLuaState->DoFile(pObj->m_pBase->szLuaName);
	LuaObject loFunction = pLuaState->GetGlobal(szLuaItemFlag_OnUse);
	if(!loFunction.IsFunction()){
		return;
	}
	int psss = NULL; 
	LuaFunction<int> lf(loFunction);

	CLuaPlusSafeFunction_3<int, ItemCenter*,ItemObj*> func;
	if(!func(pLuaState, lf, &psss, this,pObj))
	{
		//OutDumpMessage("DoLuaFunction Exception! Function Name: %s\n", luaFuncName.c_str());
	}
}

void ItemCenter::SendMallItem()
{
	BuildPacketEx(pPacket, ITEM, S_SEND_MALL_ITEM, buf, 2048);
	SET_DATA(pSend, ITEM, S_SEND_MALL_ITEM, pPacket);
	int nItemCount = 0;
	for (int i=0; i < MallConfig::GetInstance().m_pItemList->Count; i++)
	{
		MallItemCfg* pCfg = (MallItemCfg*)MallConfig::GetInstance().m_pItemList->Items[i];
		if (pCfg)
		{
			if (pCfg->m_nFlag > 0 && pCfg->m_nLimit > 0
				&& (pCfg->m_byType != 1 || pCfg->m_byType != 2)
				&& (m_pUser->m_pUserFlag->GetSLimit(pCfg->m_nFlag) >= pCfg->m_nLimit))
			{
				continue;
			}

			pSend->xData[nItemCount] = *pCfg;
			nItemCount++;

			if(nItemCount > 90)
				break;
		}
	}

	pSend->nCnt = nItemCount;
	pPacket->wSize += sizeof(MallItemCfg)*pSend->nCnt;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void ItemCenter::PileBag()
{
	std::map<int,ItemObj*> tempMap;
	for (int i=0;i<m_nBagSize;i++)
	{
		if (m_pItemInBag[i]&&!(m_pItemInBag[i]->m_pBase->byType == ITEMTYPE_EQUIP||m_pItemInBag[i]->m_pBase->nNum == 1))
		{
			int nKey = m_pItemInBag[i]->GetItemID()*10+m_pItemInBag[i]->GetDataVal(ITEM_AD_BIND);
			std::map<int,ItemObj*>::iterator itor = tempMap.find(nKey);
			if (itor != tempMap.end())
			{
				int nResult = _pileItem(itor->second,m_pItemInBag[i]);
				if (nResult == 1)
				{
					tempMap.erase(itor);
				}else if (nResult == 2)
				{
					tempMap[nKey] = m_pItemInBag[i];
				}
			}else{
				tempMap[nKey] = m_pItemInBag[i];
			}
		}
	}
}

int ItemCenter::_pileItem( ItemObj* pObjTo,ItemObj* pObjFrom )
{
	if(pObjTo == pObjFrom)
		return 0;
	int nMax = pObjTo->m_pBase->nNum;
	int nTo = pObjTo->GetDataVal(ITEM_AD_NUM);
	int nFrom = pObjFrom->GetDataVal(ITEM_AD_NUM);
	if (nTo+nFrom<nMax)
	{
		pObjTo->SetDataVal(ITEM_AD_NUM,nTo+nFrom);
		_removeItem(pObjFrom);
		return 0;
	}else if(nTo+nFrom==nMax){
		pObjTo->SetDataVal(ITEM_AD_NUM,nTo+nFrom);
		_removeItem(pObjFrom);
		return 1;
	}else{
		pObjTo->SetDataVal(ITEM_AD_NUM,nMax);
		pObjFrom->SetDataVal(ITEM_AD_NUM,nFrom+nTo-nMax);
		return 2;
	}
	return -1;
}

ItemObj* ItemCenter::CutItemByIndex( ItemObj* pObj,int nCount )
{
	if (!pObj || nCount <= 0)
	{
		return NULL;
	}
	if (pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		char sztxt[2048];
		pObj->GetByteData(sztxt);
		ItemObj* pObjCut = ItemManage::CreateItemObjByString(sztxt,pObj->GetByteSize());
		_removeItem(pObj);
		return pObjCut;
	}
	int nNum = pObj->GetDataVal(ITEM_AD_NUM);
	if (nNum<nCount)
	{
		return NULL;
	}
	ItemObj* pObjCut = ItemManage::CreateItemByID(pObj->GetItemID(),pObj->GetDataVal(ITEM_AD_BIND));
	if (!pObjCut)
	{
		return NULL;
	}
	pObjCut->SetDataVal(ITEM_AD_NUM,nCount);
	if (nNum == nCount)
	{
		_removeItem(pObj);
	}else{
		pObj->SetDataVal(ITEM_AD_NUM,nNum-nCount);
	}
	return pObjCut;
}

bool ItemCenter::AddItem( ItemObj* pObj )
{
	if (!pObj)
	{
		return false;
	}

	//if (PileItemBySite(pObj,ITEM_SLOT_BAG))
	//{
	//	return true;
	//}

	int nIndex = _getBlankBagIndex();
	if (nIndex == -1)
	{
		return false;
	}
	pObj->SetDataVal(ITEM_AD_SLOT,ITEM_SLOT_BAG);
	pObj->SetDataVal(ITEM_AD_POS,nIndex);

	m_pItemInBag[nIndex] = pObj;
	m_mapItem[pObj->GetDataVal(ITEM_AD_ID)] = pObj;
	if(pObj->m_pBase->byType == ITEMTYPE_EQUIP)
		m_pUser->m_pUserRecord->RunLuaEquipFunc(1,pObj,NULL);
	return true;
}

bool ItemCenter::AddItemByMail( int nItemID,int nItemCnt,bool bBind )
{
	return g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,m_pUser->m_dwUin,"系统","有冒险者发现您遗失的物品,现已邮寄给您。",0,nItemID,nItemCnt,bBind);
}

void ItemCenter::UpdateSec()
{
	DWORD dwNowTime = CMTime::GetCurTickCount();
	for (map<int,ItemObj*>::iterator itor = m_mapItem.begin();itor != m_mapItem.end();itor++)
	{
		if (itor->second->m_pBase->dwTimeLimit)
		{
			if (dwNowTime > itor->second->GetDataVal(ITEM_AD_LIMITTIME))
			{
				_removeItem(itor->second);
				break;
			}
		}
	}
}

bool ItemCenter::AddBagSizeByRMB()
{
	if (m_nBagSize>=100||m_nBagSize<40)
	{
		return false;
	}
	int nCost = 0;
	if (m_nBagSize<59)
	{
		nCost = m_nBagSize-39;
	}else{
		nCost = 20;
	}
	if (!m_pUser->m_xUWealth.SubWealth(UW_RMB,nCost,RMB_SAVE_EXPANDBAG))
	{
		return false;
	}
	return _addBagSize(1);
}

void ItemCenter::PileBagByID()
{
	std::map<int,ItemObj*> tempMap;
	for (int i=0;i<m_nBagSize;i++)
	{
		if (m_pItemInBag[i])
		{
			for (int j=i+1;j<m_nBagSize;j++)
			{
				if (m_pItemInBag[j]&&(m_pItemInBag[i]->GetItemID()*100+m_pItemInBag[i]->m_pBase->byType<m_pItemInBag[j]->GetItemID()*100+m_pItemInBag[j]->m_pBase->byType))
				{
					_exchangeItemPos(m_pItemInBag[i],m_pItemInBag[j]);
				}
			}
		}
	}
}

int ItemCenter::GetItemCountByIDEx( int nType,int nSite /*= (1<<ITEM_SLOT_BAG|1<<ITEM_SLOT_DEPOT)*/,int nBind /*= -1*/ )
{
	int nCount = 0;
	for (map<int,ItemObj*>::iterator itor = m_mapItem.begin();itor!=m_mapItem.end();itor++)
	{
		if(itor->second->GetItemID() == nType&& ((1<<itor->second->GetDataVal(ITEM_AD_SLOT))&nSite) ){
			if (nBind == -1||nBind == itor->second->GetDataVal(ITEM_AD_BIND))
			{
				nCount += itor->second->GetDataVal(ITEM_AD_NUM);
			}
		}
	}
	return nCount;
}

bool ItemCenter::RemoveItemByIDEx( int nType,int nCount /*= 1*/,int nSite /*= (1<<ITEM_SLOT_BAG)*/,int nLogType /*= ITEM_LOG_NULL*/,int nBind /*= false */ )
{
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nType);
	list<ItemObj*> itemlist;
	if (nBind == -1)
	{
		GetItemByType(nType,itemlist,nSite,true);
		GetItemByType(nType,itemlist,nSite,false);
	}else{
		GetItemByType(nType,itemlist,nSite,nBind);
	}
	int nSubCount = nCount;
	for(list<ItemObj*>::iterator itor = itemlist.begin();itor != itemlist.end();itor++){
		ItemObj* pObj = (*itor);
		int nNum = (*itor)->GetDataVal(ITEM_AD_NUM);
		if(nSubCount>0){
			int nChg = (nNum>nSubCount)?nSubCount:(nNum);
			nSubCount -= nChg;
			if(nNum-nChg){
				(*itor)->SetDataVal(ITEM_AD_NUM,nNum-nChg);
			}else{
				//itor = itemlist.erase(itor);
				_removeItem(pObj);
			}
		}else{
			return true;
		}
	}
	if (!nSubCount)
	{
		m_pUser->SendMsg("失去%s×%d",pBase->szName,nCount);
		if (pBase->GetItemFlag(ITEM_FLAG_LOG))
			m_pUser->m_pUserRecord->AddItemLog(pBase->nTypeID,-nCount,0);
		return true;
	}
	return false;
}

int ItemCenter::_addEquipStartemp(int nLv)
{
	EquipStarBase* pBase = EquipStarConfig::GetInstance().GetEquipStar(nLv+1);
	int nRandom = Random_Int(0,100);
	if (nRandom<pBase->nSuccess)
	{
		return nLv+1;
	}else{
		return (nLv-1>0)?(nLv-1):0;
	}
}
