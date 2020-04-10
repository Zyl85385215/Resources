
#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#include "ItemConfig.h"
#include "xnExtstr.h"
#ifdef _ISSERVER
#include "LuaPlus.h"
#endif

#include <string>
#include <list>

CreateSingleton(EquipAttrConfig);
CreateSingleton(ItemConfig);
CreateSingleton(EquipStarConfig);
CreateSingleton(EquipEatConfig);
CreateSingleton(ItemReplaceConfig);
CreateSingleton(EquipResolveConfig);
CreateSingleton(EquipGroupConfig);
CreateSingleton(MakeSkillConfig);
std::list<std::string> SpliceString(std::string szStr,std::string szSplice){
	std::list<std::string> list;

	int nPos = szStr.find(szSplice);
	while (nPos != std::string::npos)
	{
		list.push_back(szStr.substr(0,nPos));
		szStr = szStr.substr(nPos+1,szStr.length());
		nPos = szStr.find(szSplice);
	}
	list.push_back(szStr);


	//char*	szNew	= strdup(szStr);
	//char*	szRtl	= szNew;
	//char*	szFind	= szRtl;
	//while(szFind	= strstr(szRtl,szSplice))
	//{
	//	szFind[0]	= 0;
	//	list.push_back(szRtl);
	//	szRtl+=strlen(szRtl)+1;
	//}
	//if(strlen(szRtl))
	//	list.push_back(szRtl);
	//free(szNew);
	return list;
}

ItemExBase::ItemExBase()
{
	szName			= NULL;
	szDesc			= NULL;
	pAddAttr		= xnList::Create();
}

ItemExBase::~ItemExBase()
{
	SAFE_FREE(szName);
	SAFE_FREE(szDesc);
	pAddAttr->Free();
}

ItemConfig::ItemConfig()
{
	//pListMaterials	= xnList::Create ();
	pListItems		= xnList::Create ();
}

ItemConfig::~ItemConfig()
{
	while(pListItems->Count)
		delete (ItemExBase*)pListItems->Delete (0);
	pListItems->Free ();
	//while(pListMaterials->Count)
	//	delete (ItemBase*)pListMaterials->Delete (0);
	//pListMaterials->Free ();
}
int ChangeToJob(char* szJob,int nSize){
	int nVal = 0;
	for (int i=0;i<nSize;i++)
	{
		if(szJob[i] != '0') nVal |= 1<<i;
	}
	return nVal;
}
void ItemConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[128];
#ifdef _ISSERVER
	LuaPlus::LuaState* L = LuaPlus::LuaState::Create();
	int	iError = L->DoFile("data\\lua_script\\itemscript.lua");
#endif
	while (pStr->SetSection(index++) != NULL)
	{
		ItemExBase*	pIB	= new ItemExBase();
		pIB->nTypeID		= atoi (pStr->sectionName);
		pIB->szName			= strdup(pStr->ReadString ("name","未知物品"));
		pIB->szDesc			= strdup(pStr->ReadString ("desc",""));
		sprintf(szTmp,"data\\lua_script\\itemscript\\%s",pStr->ReadString ("script",""));
		pIB->szLuaName		= strdup(szTmp);
		pIB->nSpr			= atoi (pStr->ReadString ("spr","1"));
		pIB->nStar			= atoi (pStr->ReadString ("quatity","0"));
		pIB->byType			= atoi (pStr->ReadString ("type","1"));
		pIB->byLv			= atoi (pStr->ReadString ("lv","1"));
		pIB->nNum			= atoi (pStr->ReadString ("num","1"));
		pIB->bySlot			= atoi (pStr->ReadString ("slot","1"));
		pIB->nDura			= atoi (pStr->ReadString ("dura","1"));
		pIB->nPrice			= atoi (pStr->ReadString ("price","0"));
		pIB->byBind			= atoi (pStr->ReadString ("bindtype","1"));
		pIB->nSkin			= atoi (pStr->ReadString ("skin","0"));
		pIB->nUseType			= atoi (pStr->ReadString ("usetype","0"));
		pIB->nUseVal			= atoi (pStr->ReadString ("useval","0"));
		pIB->nJob			= ChangeToJob(pStr->ReadString ("job","11111"),5);
		pIB->nFlag			= ChangeToJob(pStr->ReadString ("Flag","111111111111"),12);
		pIB->wCDPos			= atoi (pStr->ReadString ("cdpos","0"));
		pIB->dwCDTime			= atoi (pStr->ReadString ("itemcd","0"));
		pIB->nCast				= atoi (pStr->ReadString ("cast","0"));
		pIB->bySexLimit				= atoi (pStr->ReadString ("sexlimit","0"));
		pIB->dwTimeLimit		= atoi (pStr->ReadString ("timelimit","0"));
		pIB->nParam[EQUIP_ATTR_HP]			= atoi (pStr->ReadString ("life","0"));
		pIB->nParam[EQUIP_ATTR_MP]			= atoi (pStr->ReadString ("magic","0"));
		pIB->nParam[EQUIP_ATTR_ATK_W]			= atoi (pStr->ReadString ("ad","0"));
		pIB->nParam[EQUIP_ATTR_ATK_N]			= atoi (pStr->ReadString ("ap","0"));
		pIB->nParam[EQUIP_ATTR_DEF_W]			= atoi (pStr->ReadString ("adDef","0"));
		pIB->nParam[EQUIP_ATTR_DEF_N]			= atoi (pStr->ReadString ("apDef","0"));
		pIB->nParam[EQUIP_ATTR_HIT]			= atoi (pStr->ReadString ("hit","0"));
		pIB->nParam[EQUIP_ATTR_CRIT]			= atoi (pStr->ReadString ("crit","0"));
		pIB->nParam[EQUIP_ATTR_MISS]			= atoi (pStr->ReadString ("dodge","0"));
		pIB->nGroup					= atoi (pStr->ReadString ("group","0"));
		std::list<std::string> templist = SpliceString(pStr->ReadString ("attach","0"),",");
		for (std::list<std::string>::iterator itor = templist.begin();itor!=templist.end();itor++)
		{
			pIB->pAddAttr->Add((void*)(atoi(itor->c_str())));
		}
		pIB->nScrpitFlag	= 0;

#ifdef _ISSERVER

		char sztxt[64];
		sprintf(sztxt,"%s%d",szLuaItemFlag_OnUse,pIB->nTypeID);
		LuaPlus::LuaObject a1 = L->GetGlobal(sztxt);
		int iError = a1.IsFunction();
		if (iError){
			pIB->nScrpitFlag |= (1<<ITEM_SCRPITFLAG_ONUSE);
		}

		sprintf(sztxt,"%s%d",szLuaItemFlag_SP,pIB->nTypeID);
		a1 = L->GetGlobal(sztxt);
		iError = a1.IsFunction();
		if (iError){
			pIB->nScrpitFlag |= (1<<ITEM_SCRPITFLAG_SP);
		}
		
#endif
		pListItems->Add (pIB);
	}
#ifdef _ISSERVER
	LuaPlus::LuaState::Destroy(L);
#endif
	pStr->Free ();
}

ItemExBase* ItemConfig::GetItemBase( WORD nTypeID )
{
	for (int i = 0; i < pListItems->Count; i++)
	{
		ItemExBase* pIB	= (ItemExBase*)pListItems->Items[i];
		if(pIB->nTypeID == nTypeID)
			return pIB;
	}
	return NULL;
}

EquipAttrConfig::EquipAttrConfig()
{
	pListItems = xnList::Create();
}

EquipAttrConfig::~EquipAttrConfig()
{
	while(pListItems->Count) delete (EquipAttrBase*)pListItems->Delete(0);
	pListItems->Free();
}

void EquipAttrConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	while (pStr->SetSection(index++) != NULL)
	{
		EquipAttrBase* pBase = new EquipAttrBase;
		pBase->nID = atoi (pStr->sectionName);
		pBase->nType = atoi (pStr->ReadString ("type","0"));
		std::list<std::string> templist = SpliceString(pStr->ReadString ("baseatr","0"),"|");
		int i = 0;
		for(std::list<std::string>::iterator itor = templist.begin();itor!=templist.end()&&i<EQUIP_ATTR_MAX;itor++,i++){
			pBase->nBase[i] = atoi (itor->c_str());
		}
		templist = SpliceString(pStr->ReadString ("randomatr","0"),"|");
		i = 0;
		for(std::list<std::string>::iterator itor = templist.begin();itor!=templist.end()&&i<EQUIP_ATTR_MAX;itor++,i++){
			pBase->nRandom[i] = atoi (itor->c_str());
		}
		pListItems->Add(pBase);
	}
	pStr->Free();
}

EquipAttrBase* EquipAttrConfig::GetEquipAttr( int nTypeID )
{
	for (int i=0;i<pListItems->Count;i++)
	{
		EquipAttrBase* pBase = (EquipAttrBase*)pListItems->Items[i];
		if (pBase->nID == nTypeID)
		{
			return pBase;
		}
	}
	return NULL;
}

EquipAttrBase::EquipAttrBase()
{
	nType = 0;
	nID = 0;
	memset(nBase,0,EQUIP_ATTR_MAX*sizeof(int));
	memset(nRandom,0,EQUIP_ATTR_MAX*sizeof(int));
}

EquipStarConfig::EquipStarConfig()
{
	memset(aExPer,0,nEquipStarExCount*sizeof(int));
	memset(aExSuccess,0,nEquipStarExCount*sizeof(int));
	pListItems = xnList::Create();
}

EquipStarConfig::~EquipStarConfig()
{
	while(pListItems->Count) delete (EquipStarBase*)pListItems->Delete(0);
}

void EquipStarConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	if(pStr->SetSection(0))
	{
		nItemCount = atoi (pStr->ReadString ("itemCnt","0"));
		nItemID = atoi (pStr->ReadString ("itemID","0"));
		
	}
	int index = 1;
	int nCount = 0;
	while (pStr->SetSection(index++) != NULL)
	{
		EquipStarBase* pBase = new EquipStarBase;
		pBase->nPer = atoi (pStr->ReadString ("per","0"));
		pBase->nSuccess = atoi (pStr->ReadString ("success","0"));
		pBase->nGold = atoi (pStr->ReadString ("gold","0"));
		pBase->nExCount = -1;
		int nExPer = atoi (pStr->ReadString ("exper","0"));
		if (nExPer&&nCount<nEquipStarExCount)
		{
			aExPer[nCount] = nExPer;
			aExSuccess[nCount] = atoi (pStr->ReadString ("exsuccess","0"));
			pBase->nExCount = nCount;
			nCount++;
		}
		pListItems->Add(pBase);
	}
	pStr->Free();
}

EquipStarBase* EquipStarConfig::GetEquipStar( int nTypeID )
{
	if (nTypeID > pListItems->Count||nTypeID<=0)
	{
		return NULL;
	}
	return (EquipStarBase*)pListItems->Items[nTypeID-1];
}

EquipEatConfig::EquipEatConfig()
{
	pListItems = xnList::Create();
}

EquipEatConfig::~EquipEatConfig()
{
	while(pListItems->Count) delete (EquipEatBase*)pListItems->Delete(0);
}

void EquipEatConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	char sztxt[32];
	if(pStr->SetSection(0))
	{
		for (int i=0;i<4;i++)
		{
			sprintf(sztxt,"quality%d",i);
			m_nEatEquipVal[i] = atoi (pStr->ReadString (sztxt,"0"));
		}
		m_nSlot = ChangeToJob(pStr->ReadString ("sort","011110001"),9);
	}
	int index = 1;
	while (pStr->SetSection(index++) != NULL)
	{
		EquipEatBase* pBase = new EquipEatBase;
		pBase->exp = atoi (pStr->ReadString ("exp","0"));
		pBase->nDamage = atoi (pStr->ReadString ("damage","0"));
		pBase->nDefense = atoi (pStr->ReadString ("defense","0"));
		pListItems->Add(pBase);
	}
	pStr->Free();
}

EquipEatBase* EquipEatConfig::GetEquipEat( int nTypeID )
{
	if (nTypeID > pListItems->Count||nTypeID<=0)
	{
		return NULL;
	}
	return (EquipEatBase*)pListItems->Items[nTypeID-1];
}

int EquipEatConfig::GetAddParam( int nSlot )
{
	return (m_nSlot&(1<<nSlot))?1:0;
}


ItemReplaceTag::ItemReplaceTag()
{
	pListCost = xnList::Create();
	pListGet = xnList::Create();
}

ItemReplaceTag::~ItemReplaceTag()
{
	while(pListCost->Count) delete (CommonItemTag*)pListCost->Delete(0);
	while(pListGet->Count) delete (CommonItemTag*)pListGet->Delete(0);
}

ItemReplaceConfig::ItemReplaceConfig()
{
	pList = xnList::Create();
}

ItemReplaceConfig::~ItemReplaceConfig()
{
	while(pList->Count) delete (ItemReplaceTag*)pList->Delete(0);
}

void ItemReplaceConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		ItemReplaceTag*	pEB	= new ItemReplaceTag();
		int nCnt = atoi (pStr->ReadString ("costcnt","0"));
		pEB->nType = atoi (pStr->ReadString ("type","0"));
		for (int i=0;i<nCnt;i++)
		{
			CommonItemTag* pCost = new CommonItemTag;
			sprintf(szTmp,"costitem%d",i+1);
			pCost->nItemID = atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"costitemcnt%d",i+1);
			pCost->nItemCount = atoi (pStr->ReadString (szTmp,"0"));
			pEB->pListCost->Add(pCost);
		}
		nCnt = atoi (pStr->ReadString ("getcnt","0"));
		for (int i=0;i<nCnt;i++)
		{
			CommonItemTag* pCost = new CommonItemTag;
			sprintf(szTmp,"getitem%d",i+1);
			pCost->nItemID = atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"getitemcnt%d",i+1);
			pCost->nItemCount = atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"getper%d",i+1);
			pCost->nPer = atoi (pStr->ReadString (szTmp,"0"));
			pEB->pListGet->Add(pCost);
		}
		pList->Add (pEB);
	}
	pStr->Free ();
}

ItemReplaceTag* ItemReplaceConfig::GetItemReplaceTag( int nIndex )
{
	if (nIndex>=pList->Count)
	{
		return NULL;
	}
	return (ItemReplaceTag*)pList->Items[nIndex];
}

void EquipResolveConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	char	szTmp[32];
	if(pStr->SetSection(0))
	{
		std::list<std::string> templist = SpliceString(pStr->ReadString ("lv","0"),"|");
		int nCount = 0;
		for (std::list<std::string>::iterator itor = templist.begin();itor!=templist.end();itor++)
		{

			nLv[nCount] = atoi(itor->c_str());
			nCount++;
		}
		for (int i=0;i<4;i++)
		{
			sprintf(szTmp,"cloth%d",i);
			templist = SpliceString(pStr->ReadString (szTmp,"0"),"|");
			nCount = 0;
			for (std::list<std::string>::iterator itor = templist.begin();itor!=templist.end();itor++)
			{
				nCloth[i][nCount] = atoi(itor->c_str());
				nCount++;
			}
			sprintf(szTmp,"iron%d",i);
			templist = SpliceString(pStr->ReadString (szTmp,"0"),"|");
			nCount = 0;
			for (std::list<std::string>::iterator itor = templist.begin();itor!=templist.end();itor++)
			{
				nIron[i][nCount] = atoi(itor->c_str());
				nCount++;
			}
		}
		
	}
	pStr->Free ();
}

int EquipResolveConfig::GetLvType( int nInLv )
{
	for (int i=0;i<nMaxResolveTypeCount;i++)
	{
		if(nLv[i]>nInLv) return i-1;
	}
	return 9;
}

EquipResolveConfig::EquipResolveConfig()
{

}

EquipResolveConfig::~EquipResolveConfig()
{

}

void MakeSkillConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		MakeSkillTag*	pEB	= new MakeSkillTag();
		int nCnt = atoi (pStr->ReadString ("needcnt","0"));
		pEB->nType = atoi (pStr->ReadString ("type","0"));
		pEB->nID = atoi(pStr->sectionName);
		for (int i=0;i<nCnt;i++)
		{
			CommonItemTag* pCost = new CommonItemTag;
			sprintf(szTmp,"needitem%d",i+1);
			pCost->nItemID = atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"needcount%d",i+1);
			pCost->nItemCount = atoi (pStr->ReadString (szTmp,"0"));
			pEB->pListCost->Add(pCost);
		}
		pEB->tagGet.nItemID = atoi (pStr->ReadString ("getitem","0"));
		pEB->tagGet.nItemCount = atoi (pStr->ReadString ("getitemcount","0"));
		pEB->nLv = atoi (pStr->ReadString ("lvl","0"));
		pEB->nMake1 = atoi (pStr->ReadString ("make1","0"));
		pEB->nMake2 = atoi (pStr->ReadString ("make2","0"));
		pEB->nJob = ChangeToJob(pStr->ReadString ("job1","00000"),5);
		pEB->nVigor = atoi (pStr->ReadString ("needvigor","0"));
		pEB->nGold = atoi (pStr->ReadString ("needgold","0"));
		pEB->szFilter = pStr->ReadString ("sort","缺省");
		m_mapInfo[pEB->nType][pEB->szFilter][pEB->nID] = pEB;
		m_mapIDInfo[pEB->nID] = pEB;
		m_pList[pEB->nType]->Add(pEB);
	}
	pStr->Free ();
}

MakeSkillConfig::MakeSkillConfig()
{
	for (int i=0;i<5;i++)
	{
		m_pList[i] = xnList::Create();
	}
}

MakeSkillConfig::~MakeSkillConfig()
{
	for (int i=0;i<5;i++)
	{
		m_pList[i]->Free();
	}
	m_mapInfo.clear();
	for (map<int,MakeSkillTag*>::iterator itor = m_mapIDInfo.begin();itor != m_mapIDInfo.end();itor++)
	{
		
		delete itor->second;
		//itor = m_mapInfo.erase(itor);
	} 
	m_mapIDInfo.clear();
}

void MakeSkillConfig::GetMakeList( int nType,int nJob,int nLv,xnList* pList )
{
	if (nType>=5)
	{
		return;
	}
	for (int i=0;i<m_pList[nType]->Count;i++)
	{
		MakeSkillTag* pTag = (MakeSkillTag*)m_pList[nType]->Items[i];
		if (/*pTag->nLv<=nLv&&*/(pTag->nJob&(1<<nJob)))
		{
			pList->Add(pTag);
		}
	}
}

void MakeSkillConfig::GetMakeList( int nType,int nJob,tempstring& szFilter,xnList* pList )
{
	if (nType>=5)
	{
		return;
	}
	if (m_mapInfo[nType].find(szFilter)==m_mapInfo[nType].end()&&m_mapInfo[nType].size())
	{
		szFilter = m_mapInfo[nType].begin()->first;
	}else if (m_mapInfo[nType].find(szFilter)==m_mapInfo[nType].end())
	{
		return;
	}
	for (map<int,MakeSkillTag*>::iterator itor = m_mapInfo[nType][szFilter].begin();itor!=m_mapInfo[nType][szFilter].end();itor++)
	{
		MakeSkillTag* pTag = itor->second;
		if (/*pTag->nLv<=nLv&&*/(pTag->nJob&(1<<nJob)))
		{
			pList->Add(pTag);
		}
	}
}

MakeSkillTag* MakeSkillConfig::GetMakeItem( int nID )
{
	map<int,MakeSkillTag*>::iterator itor = m_mapIDInfo.find(nID);
	if (itor != m_mapIDInfo.end())
	{
		return itor->second;
	}
	return NULL;
}

MakeSkillTag::MakeSkillTag()
{
	pListCost = xnList::Create();
}

MakeSkillTag::~MakeSkillTag()
{
	while(pListCost->Count) delete (CommonItemTag*)pListCost->Delete(0);
}

void EquipGroupConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		EquipGroupInfo*	pEB	= new EquipGroupInfo();
		int nCnt = atoi (pStr->ReadString ("count","0"));
		strcpy(pEB->szName,pStr->ReadString ("name","?"));
		pEB->nID = atoi(pStr->sectionName);
		EquipGroupVal groupVal;
		for (int i=0;i<nCnt;i++)
		{
			sprintf(szTmp,"needcnt%d",i+1);
			groupVal.byCount = atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"state%d",i+1);
			groupVal.nState = atoi (pStr->ReadString (szTmp,"0"));
			sprintf(szTmp,"txt%d",i+1);
			strcpy(groupVal.szDesc,pStr->ReadString (szTmp,"?"));
			pEB->groupVal.push_back(groupVal);
		}
		std::list<string> temp = SpliceString(pStr->ReadString ("id",""),",");
		for (std::list<string>::iterator itor = temp.begin();itor!=temp.end();itor++)
		{
			pEB->nItemID.push_back(atoi(itor->c_str()));
		}
		m_mapInfo[pEB->nID] = pEB;
	}
	pStr->Free ();
}

EquipGroupConfig::~EquipGroupConfig()
{
	for (map<int,EquipGroupInfo*>::iterator itor = m_mapInfo.begin();itor != m_mapInfo.end();)
	{
		delete itor->second;
		m_mapInfo.erase(itor++);
	}
}

EquipGroupInfo* EquipGroupConfig::GetItem( int nID )
{
	map<int,EquipGroupInfo*>::iterator itor = m_mapInfo.find(nID);
	if (itor != m_mapInfo.end())
	{
		return itor->second;
	}
	return NULL;
}
