#include "stdafx.h"
#include "ItemManage.h"
#include "DBControl.h"
#include "ItemCenter.h"
#include "UserData.h"
#include "ZLogFile.h"
#include "FsGameCenter.h"

extern ZLogFile* g_pLogFile;
extern int Random_Int(int min, int max);

bool ItemManage::DBInit( DBControl* pDB )
{
	m_pDB = pDB;
	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_ITEM_MANAGE_TABLE))
		return false;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return false;

	while(pCmd->FetchRow())
	{
		int nType = pCmd->GetDWORD("type");
		int nIndex = pCmd->GetDWORD("id");
		switch (nType)
		{
		case ITEM_MANAGE_TYPE_ITEMOBJ:
			{
				int nLen = 0;
				BYTE* pData = pCmd->GetBLOB("data",&nLen);
				ItemObj* pObj = ItemManage::CreateItemObjByString((char*)pData,nLen);
				if (pObj)
				{
					if(!AddItemObj(pObj,nIndex)){
						pObj->Release();
						delete pObj;
					}
				}
				if (m_nNextItemId<nIndex)
				{
					m_nNextItemId = nIndex;
				}
			}
			break;
		}

	}
	m_nNextItemId++;
	return true;
}

ItemObj* ItemManage::CreateItemObjByString( char* pData,int nSize )
{
	if ((*pData) != AMZ_DATA_ITEM)
	{
		return NULL;
	}
	pData++;nSize--;
	AmzDataBase* pBase = AmzDataManager::CreateAmzData(ITEM_AD_MAX,AMZ_DATA_ITEM,pData);
	pData += (ITEM_AD_MAX*sizeof(int));nSize -= (ITEM_AD_MAX*sizeof(int));
	if (!pBase)
	{
		return NULL;
	}
	ItemObj* pObj = new ItemObj(pBase,NULL);
	if (!pObj || pObj->m_pBase==NULL) {
		delete pBase;
		return NULL;
	}
	if (nSize<=0)
	{
		return pObj;
	}
	while(nSize>0){
		switch(*pData){
		case AMZ_DATA_EQUIPEX:
			{
				pData++;nSize--;
				AmzDataBase* pDataEx = AmzDataManager::CreateAmzData(EQUIPEX_AD_MAX,AMZ_DATA_EQUIPEX,pData);
				pObj->SetExData(ITEMEXDATA_EQUIPEX,pDataEx);
				pData += (EQUIPEX_AD_MAX*sizeof(int));nSize -= (EQUIPEX_AD_MAX*sizeof(int));
			}
			break;
		}
	}
	return pObj;
}

bool ItemManage::AddItemObj( ItemObj* pObj,int nIndex,bool bSaveDB )
{
	if (m_TotalMap.find(nIndex) != m_TotalMap.end())
	{
		g_pLogFile->OutPut("ItemMange AddItemObj Index %d exist",nIndex);
		return false;
	}
	int nID = pObj->GetDataVal(ITEM_AD_TYPE);
	pObj->SetDataVal(ITEM_AD_ID,nIndex);
	//pObj->SetDataVal(ITEM_AD_UIN,0);
	m_TotalMap[nIndex] = pObj;
	//m_TypeMap[nID].push_back(pObj);
	if (bSaveDB)
	{
		_dbInsertItem(nIndex,pObj);
	}
	return true;
}

int ItemManage::AddItemObj( ItemObj* pObj,bool bSaveDB )
{
	int nID = pObj->GetDataVal(ITEM_AD_TYPE);
	pObj->SetDataVal(ITEM_AD_ID,m_nNextItemId);
	//pObj->SetDataVal(ITEM_AD_UIN,0);
	m_TotalMap[m_nNextItemId] = pObj;
	//m_TypeMap[nID].push_back(pObj);
	if (bSaveDB)
	{
		_dbInsertItem(m_nNextItemId,pObj);
	}
	return m_nNextItemId++;
}

ItemObj* ItemManage::GetItemObj( int nIndex )
{
	map<int,ItemObj*>::iterator itor = m_TotalMap.find(nIndex);
	if (itor != m_TotalMap.end())
	{
		return itor->second;
	}else{
		g_pLogFile->OutPut("ItemMange Index %d can not be found",nIndex);
		return NULL;
	}
}

ItemObj* ItemManage::RemoveItemObj( int nIndex ,bool bSaveDB)
{
	if (m_TotalMap.find(nIndex) != m_TotalMap.end())
	{
		ItemObj* pObj = m_TotalMap[nIndex];
		int nID = pObj->GetDataVal(ITEM_AD_TYPE);

		m_TotalMap.erase(nIndex);
		if(bSaveDB) _dbDeleteItem(nIndex);
// 		for (list<ItemObj*>::iterator itor=m_TypeMap[nID].begin();itor!=m_TypeMap[nID].end();itor++)
// 		{
// 			if (*itor == pObj)
// 			{
// 				m_TypeMap[nID].erase(itor);
// 				break;
// 			}
// 		}
		return pObj;
	}
	g_pLogFile->OutPut("ItemMange RemoveItemObj Index %d can not be found",nIndex);
	return NULL;
}

ItemManage::ItemManage():m_nNextItemId(0)
{

}

void ItemManage::_dbInsertItem( int nIndex,ItemObj* pItem )
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_ITEM_MANAGE_TABLE" (id, type, data) values (%d, %d, '')", 
		nIndex,ITEM_MANAGE_TYPE_ITEMOBJ);
	char sztxt[1024];
	pItem->GetByteData(sztxt);
	m_pDB->SetBLOB(FALSE,sztxt,pItem->GetByteSize(),"update "RPG_ITEM_MANAGE_TABLE" set data=? where id=%d",nIndex);
}

void ItemManage::_dbDeleteItem( int nIndex )
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_ITEM_MANAGE_TABLE" where id=%d",nIndex);
}

ItemObj* ItemManage::CreateItemByID( int nID,bool bBind )
{
	AmzDataBase* pBase = AmzDataManager::CreateAmzData(ITEM_AD_MAX,AMZ_DATA_ITEM);
	if (!pBase)
	{
		return NULL;
	}
	memset(pBase->pbData,0,sizeof(int)*ITEM_AD_MAX);
	pBase->SetVal(ITEM_AD_TYPE,nID);
	ItemObj* pObj = new ItemObj(pBase,NULL);
	if (!pObj || pObj->m_pBase == NULL) {
		delete pBase;
		return NULL;
	}
	if(pObj->m_pBase->byBind == ITEM_BINDTYPE_BIND_GET){
		pObj->SetDataVal(ITEM_AD_BIND,1);
	}else{
		pObj->SetDataVal(ITEM_AD_BIND,bBind);
	}
	if (pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		pObj->SetDataVal(ITEM_AD_NUM,pObj->m_pBase->nDura);
		_addEquipEx(pObj);
		pObj->RecalcParam();
	}
	if (pObj->m_pBase->dwTimeLimit)
	{
		pObj->SetDataVal(ITEM_AD_LIMITTIME,CMTime::GetCurTickCount()+7*24*60*60);
	}
	return pObj;
}


void ItemManage::_addEquipEx( ItemObj* pObj )
{

	AmzDataBase* pAmzBase = AmzDataManager::CreateAmzData(EQUIPEX_AD_MAX,AMZ_DATA_EQUIPEX);
	if(!pAmzBase) return;

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

}
void ItemManage::_setEquipExAttr( ItemObj* pObj,int nSite,int nType,int nVal )
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

bool ItemManage::SetItemOwner( UserData* pUser ,ItemObj* pObj,BYTE byLogType)
{
	if (!pUser||!pObj)
	{
		return false;
	}

	if (pObj->m_pBase->GetItemFlag(ITEM_FLAG_LOG))
	{
		int nItemNum = pObj->m_pBase->byType == ITEMTYPE_EQUIP?1:pObj->GetDataVal(ITEM_AD_NUM);
		pUser->m_pUserRecord->AddItemChangeLog(pObj->GetItemID(),nItemNum,pObj->GetDataVal(ITEM_AD_UIN),byLogType);
	}

	pUser->m_pAmzDManager->AddAmzData(pObj->m_pData);
	pObj->m_pData->nModelID = pUser->LockADataID();

	LONGLONG temp = pUser->m_dwUin;
	pObj->m_pData->llKey	= ((temp<< 32)|pObj->m_pData->nModelID);

	pObj->SetDataVal(ITEM_AD_UIN,pUser->m_dwUin);
	pObj->SetDataVal(ITEM_AD_ID,pObj->m_pData->nModelID);

	pObj->m_pData->bStateFlag = AMZ_STATE_CREATE;

	if (pObj->m_pDataEx[ITEMEXDATA_EQUIPEX])
	{
		pUser->m_pAmzDManager->AddAmzData(pObj->m_pDataEx[ITEMEXDATA_EQUIPEX]);


		AmzDataBase* pAmzBase = pObj->m_pDataEx[ITEMEXDATA_EQUIPEX];
		pAmzBase->nModelID = pUser->LockADataID();
		pAmzBase->llKey	= ((temp<< 32)|pAmzBase->nModelID);

		pAmzBase->SetVal(EQUIPEX_AD_ITEMID,pObj->GetDataVal(ITEM_AD_ID));
		pAmzBase->SetVal(EQUIPEX_AD_UIN,pObj->GetDataVal(ITEM_AD_UIN));
		pAmzBase->SetVal(EQUIPEX_AD_ID,pAmzBase->nModelID);
		pAmzBase->bStateFlag = AMZ_STATE_CREATE;
	}
	
	return true;
}
extern char* g_szTableName[];
extern char* g_szDbName1[];
extern char* g_szDbName2[];
 
ItemObj* ItemManage::GetItemByIndex( DWORD dwUin,int nIndex )
{
	QueryRlt xRlt;
	if(!g_pGameCenter->m_pItemManage->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM %s WHERE uin = %d and id=%d",g_szTableName[AMZ_DATA_ITEM] ,dwUin,nIndex)) {
		g_pLogFile->OutPut("ItemMange GetItemByIndex Uin %d  Index %d can not be found",dwUin,nIndex);
		return NULL;
	}
	AmzDataBase* pBase = AmzDataManager::CreateAmzData(ITEM_AD_MAX,AMZ_DATA_ITEM);
	if (!pBase)
	{
		return NULL;
	}
	memset(pBase->pbData,0,sizeof(int)*ITEM_AD_MAX);
	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{
			for (int i=0;i<ITEM_AD_MAX;i++)
			{
				pBase->SetVal(i,pCmd->GetInt(g_szDbName1[i]));
			}
		}else{
			g_pLogFile->OutPut("ItemMange GetItemByIndex Uin %d  Index %d can not be found",dwUin,nIndex);
			delete pBase;
			return NULL;
		}
	}
	ItemObj* pObj = new ItemObj(pBase,NULL);
	if (!pObj || pObj->m_pBase == NULL) {
		delete pBase;
		return NULL;
	}
	if (pObj->m_pBase->byType == ITEMTYPE_EQUIP)
	{
		if(g_pGameCenter->m_pItemManage->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM %s WHERE Uin = %d and itemid=%d",g_szTableName[AMZ_DATA_EQUIPEX] ,dwUin,nIndex)) {
			MySqlQuery* pCmd = xRlt.GetRlt();
			if (pCmd != NULL)
			{
				if(pCmd->FetchRow())
				{
					AmzDataBase* pBaseEx = AmzDataManager::CreateAmzData(EQUIPEX_AD_MAX,AMZ_DATA_EQUIPEX);
					for (int i=0;i<EQUIPEX_AD_MAX;i++)
					{
						pBaseEx->SetVal(i,pCmd->GetInt(g_szDbName2[i]));
					}
					pObj->SetExData(ITEMEXDATA_EQUIPEX,pBaseEx);
				}else{
					g_pLogFile->OutPut("ItemMange GetItemByIndex Equip Uin %d  Index %d can not be found",dwUin,nIndex);
				}
			}
		}else{
			g_pLogFile->OutPut("ItemMange GetItemByIndex Equip Uin %d  Index %d can not be found",dwUin,nIndex);
		}
	}
	return pObj;
}
