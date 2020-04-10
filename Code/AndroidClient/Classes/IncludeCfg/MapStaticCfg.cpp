#include "stdafx.h"
#ifndef _ISSERVER
#include "UserData.h"
#endif
#include "xnExtstr.h"
#include "MapStaticCfg.h"

#include "MapConfig.h"


CreateSingleton(NpcPathLeadCfg);
CreateSingleton(MonsterPathLeadCfg);
CreateSingleton(TransPointCfg);
CreateSingleton(MapEffectCfg);

NpcPathLeadCfg::NpcPathLeadCfg()
{
	m_pList		= xnList::Create ();
}

NpcPathLeadCfg::~NpcPathLeadCfg()
{
	while(m_pList->Count)
	{
		delete	(PathObj*)m_pList->Delete (0);
	}
	m_pList->Free ();
}

void NpcPathLeadCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[64];
	while (pStr->SetSection(index) != NULL)
	{
		PathObj* pPathObj		= new PathObj;
		pPathObj->nID			= atoi(pStr->sectionName);
		pPathObj->nSkin			= atoi(pStr->ReadString("skin","0"));
		pPathObj->nMapindex		= atoi(pStr->ReadString("map","0"));
		pPathObj->nPosX			= atoi(pStr->ReadString("posx","0"));
		pPathObj->nPosY			= atoi(pStr->ReadString("posy","0"));
		pPathObj->nLv			= atoi(pStr->ReadString("lv","99"));
		pPathObj->nEff          = atoi(pStr->ReadString("eff","0"));

		pPathObj->bMapTalk		= atoi(pStr->ReadString("rtalkfree","0"));//是否在屏幕上说话
		pPathObj->nShopIndex	= atoi(pStr->ReadString("shop","-1"));
		int nTCnt	= atoi(pStr->ReadString("randtalkcnt","0"));
		for (int t = 0; t < nTCnt; t++)
		{
			sprintf(szTmp,"randtalk%d",t+1);
			pPathObj->pListTalk->Add(strdup(pStr->ReadString(szTmp,"NULL")));
		}

		//pPathObj->nEff			= atoi(pStr->ReadString("eff","0"));
		strncpy(pPathObj->szName,pStr->ReadString("name","NULL"),32);
		pPathObj->byType	= 1;
		m_pList->Add(pPathObj);
		index++;
	}
	pStr->Free();
}

PathObj* NpcPathLeadCfg::GetNpcPath( char* szName, int nMapIndex )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];
		if(strcmp (szName,pPO->szName) == 0 && (nMapIndex==-1 || pPO->nMapindex	== nMapIndex))
			return pPO;
	}
	return NULL;
}

PathObj* NpcPathLeadCfg::GetNpcPath( int nNpcID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];
		if(nNpcID	== pPO->nID)
			return pPO;
	}
	return NULL;
}

bool NpcPathLeadCfg::GetNpcListByMIndex( int nMapIndex, xnList* pOutList )
{
	//pOutList->Clear ();
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];

		if(MapViewConfig::GetInstance().GetMapTileData(pPO->nMapindex) == NULL)
		{
			if(pPO->nMapindex+100 == nMapIndex || pPO->nMapindex+200==nMapIndex)
			{
				pOutList->Add(pPO);
			}
		}
		else if(pPO->nMapindex	== nMapIndex)
		{
			pOutList->Add (pPO);
		}
	}
	return true;
}

void NpcPathLeadCfg::ResetMark()
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];
		if(pPO->byMark)
			pPO->byMark	= 0;
	}
}

MonsterPathLeadCfg::MonsterPathLeadCfg()
{
	m_pList		= xnList::Create ();
}

MonsterPathLeadCfg::~MonsterPathLeadCfg()
{
	while(m_pList->Count)
	{
		delete	(PathObj*)m_pList->Delete (0);
	}
	m_pList->Free ();
}

void MonsterPathLeadCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index) != NULL)
	{
		PathObj* pPathObj		= new PathObj;
		pPathObj->nID			= atoi(pStr->sectionName);
		pPathObj->nSkin			= atoi(pStr->ReadString("skin","0"));
		pPathObj->nMapindex		= atoi(pStr->ReadString("map","0"));
		pPathObj->nPosX			= atoi(pStr->ReadString("posx","0"));
		pPathObj->nPosY			= atoi(pStr->ReadString("posy","0"));
		pPathObj->nLv			= atoi(pStr->ReadString("lv","0"));

		pPathObj->bMapTalk		= atoi(pStr->ReadString("rtalkfree","0"));//是否在屏幕上说话
		int nTCnt	= atoi(pStr->ReadString("randtalkcnt","0"));
		for (int t = 0; t < nTCnt; t++)
		{
			sprintf(szTmp,"randtalk%d",t+1);
			pPathObj->pListTalk->Add(strdup(pStr->ReadString(szTmp,"NULL")));
		}

		//pPathObj->nEff			= atoi(pStr->ReadString("eff","0"));
		strncpy(pPathObj->szName,pStr->ReadString("name","NULL"),32);
		pPathObj->byType	= 2;
		m_pList->Add(pPathObj);
		index++;
	}
	pStr->Free();
}

PathObj* MonsterPathLeadCfg::GetPathObj( int	nMonID )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];
		if(nMonID == pPO->nID)
			return pPO;
	}
	return NULL;
}

bool MonsterPathLeadCfg::GetMonListByMIndex( int nMapIndex, xnList* pOutList )
{
	//pOutList->Clear ();
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];
		if(MapViewConfig::GetInstance().GetMapTileData(pPO->nMapindex) == NULL)
		{
			if(pPO->nMapindex+100 == nMapIndex || pPO->nMapindex+200==nMapIndex)
			{
				pOutList->Add(pPO);
			}
		}
		else if(pPO->nMapindex	== nMapIndex)
		{
			pOutList->Add (pPO);
		}
	}
	return true;
}

void MonsterPathLeadCfg::ResetMark()
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		PathObj* pPO	= (PathObj*)m_pList->Items[i];
		if(pPO->byMark)
		{
			pPO->byMark	= 0;
		}
	}
}

TransPointCfg::TransPointCfg()
{
	m_pTmpList		= xnList::Create ();
}

TransPointCfg::~TransPointCfg()
{
	std::map<WORD,xnList*>::iterator	itor	= mapTPMaps.begin();
	while(itor	!= mapTPMaps.end())
	{
		while(itor->second->Count)
			delete (_TransPoint*)itor->second->Delete(0);
		itor->second->Free();
		itor++;
	}

	while(m_pTmpList->Count)
		delete (_MuliParamObj*)m_pTmpList->Delete(0);
	m_pTmpList->Free ();
}

void TransPointCfg::LoadFile( char* szFile )
{
	xnExtstr*	pStr	= xnExtstr::Import (szFile);
	if(pStr == NULL)
		return;

	int nIndex	= 0;
	while(pStr->SetSection (nIndex++))
	{
		_TransPoint*	pTp	= new _TransPoint();
		pTp->nID		= atoi(pStr->sectionName);
		pTp->nMapindex	= atoi(pStr->ReadString("srcmap","0"));

		xnList* pListMapTP	= GetMapTP(pTp->nMapindex);
		if(pListMapTP == NULL)
		{
			pListMapTP	= xnList::Create();
			mapTPMaps[pTp->nMapindex]	= pListMapTP;
		}

		strncpy(pTp->szName,pStr->ReadString("name","TransGate"),32);
		pTp->nToMapIndex	= atoi(pStr->ReadString("destmap","0"));
		pTp->byToMapCopy	= atoi(pStr->ReadString("ismapcopy","0"));
		pTp->nPosX			= atoi(pStr->ReadString("srcx","1000"));
		pTp->nPosY			= atoi(pStr->ReadString("srcy","1000"));//
		pTp->nSrcRange		= atoi(pStr->ReadString("srcrange","100"));//
		pTp->nDestX			= atoi(pStr->ReadString("destx","1000"));
		pTp->nDestY			= atoi(pStr->ReadString("desty","1000"));//
		pTp->nDestRange		= atoi(pStr->ReadString("destrange","100"));//
		pTp->wEffID			= atoi(pStr->ReadString("effid","0"));//
		pTp->byLvLimit			= atoi(pStr->ReadString("lvlimit","0"));//
		pTp->byType			= 3;
		pTp->byMark			= 4;
		pListMapTP->Add(pTp);
	}

	pStr->Free ();
}

bool TransPointCfg::FindMapPath( int nMIndexS, int nMIndexT, xnList* pList )
{
	pList->Clear();
	while(m_pTmpList->Count)
		delete (_MuliParamObj*)m_pTmpList->Delete(0);
	m_nLoopCnt	= 0;
	return _findMapPath (nMIndexS,nMIndexT,pList,0);
}

xnList* TransPointCfg::GetMapTP( int nMIndex )
{
	std::map<WORD,xnList*>::iterator	itor	= mapTPMaps.find(nMIndex);
	if(itor	!= mapTPMaps.end())
	{
		return itor->second;
	}
	return NULL;
}

bool TransPointCfg::_findMapPath( int nMIndexS, int nMIndexT, xnList* pList,int nStep )
{
#ifndef _ISSERVER
	if(MapViewConfig::GetInstance().GetMapTileData(nMIndexS) == NULL)
	{

		if(pChar->m_byCountry == 1)
			nMIndexS	+= 100;
		else
			nMIndexS	+= 200;
	}
#endif
	for (int i = 0; i < m_pTmpList->Count; i++)
	{
		_MuliParamObj*	pMP	= (_MuliParamObj*)m_pTmpList->Items[i];
		if(pMP->nParam1 == nMIndexS && pMP->nParam2	<= nStep)
			return false;
	}
	_MuliParamObj*	pMP	= new _MuliParamObj();
	pMP->nParam1	= nMIndexS;
	pMP->nParam2	= nStep;
	m_pTmpList->Add (pMP);

	m_nLoopCnt++;
	if(m_nLoopCnt > 1000)
	{
		return false;
	}
	xnList* pListMapTPt	= GetMapTP (nMIndexS);
	if(!pListMapTPt)
		return false;

	//找直接的
	for (int i = 0; i < pListMapTPt->Count; i++)
	{
		_TransPoint* pTP	= (_TransPoint*)pListMapTPt->Items[i];
		if(pTP->byToMapCopy || nMIndexT >= 1000)
		{
			if(nMIndexT >= 1000 && pTP->byToMapCopy)
			{
				if(nMIndexT%1000 == pTP->nToMapIndex)
				{
					pList->Add (pTP);
					return true;
				}
			}
		}
		else if(MapViewConfig::GetInstance().GetMapTileData(pTP->nToMapIndex) == NULL)
		{
			if(pTP->nToMapIndex +100 == nMIndexT
				|| pTP->nToMapIndex +200 == nMIndexT)
			{
				pList->Add (pTP);
				return true;
			}
		}
		else if(pTP->nToMapIndex == nMIndexT)
		{
			pList->Add (pTP);
			return true;
		}
	}
	//递归查找
	xnList*	pLA	= xnList::Create();

	for (int i = 0; i < pListMapTPt->Count; i++)
	{
		_TransPoint* pTP	= (_TransPoint*)pListMapTPt->Items[i];
		xnList*	pLB	= xnList::Create();
		pLB->Add(pTP);
		if(_findMapPath (pTP->nToMapIndex,nMIndexT,pLB,nStep+1))
		{
			pLA->Add(pLB);
		}
		else
			pLB->Free();
	}

	int	nMinPt	= 10000;
	xnList*	pLMin	= NULL;
	for (int i = 0; i < pLA->Count; i++)
	{
		xnList* pTmp	= (xnList*)pLA->Items[i];
		if(pTmp->Count < nMinPt)
		{
			pLMin	= pTmp;
			nMinPt	= pTmp->Count;
		}
	}

	if(pLMin)
	{
		while(pLMin->Count)
			pList->Add(pLMin->Delete(0));

		while(pLA->Count)
		{
			xnList* pTmp	= (xnList*)pLA->Delete(0);
			pTmp->Free();
		}
		pLA->Free();
		return true;
	}
	pLA->Free();
	return false;
}

_TransPoint* TransPointCfg::GetTransByID(WORD wMIndex, int nTPID )
{
	xnList* pList	= GetMapTP(wMIndex);
	if(pList)
	{
		for (int i = 0; i < pList->Count; i++)
		{
			_TransPoint* pTP	= (_TransPoint*)pList->Items[i];
			if(pTP->nID == nTPID)
				return pTP;
		}
	}
	return NULL;
}

MapEffectCfg::MapEffectCfg()
{
	m_pList		= xnList::Create ();
}

MapEffectCfg::~MapEffectCfg()
{
	while(m_pList->Count)
		delete (_MapEffectObj*)m_pList->Delete (0);
	m_pList->Free ();
}

void MapEffectCfg::LoadFile( int nMapIndex )
{
	while(m_pList->Count)
		delete (_MapEffectObj*)m_pList->Delete (0);

	char	szTmp[256];
	sprintf(szTmp,"Map/%d/mapeffcfg.txt",nMapIndex);
	xnExtstr*	pStr	= xnExtstr::Import (szTmp);
	if(pStr == NULL)
		return;

	int nIndex	= 0;
	while(pStr->SetSection (nIndex++))
	{
		_MapEffectObj*	pEO	= new _MapEffectObj();
		pEO->nEffectID	= atoi (pStr->ReadString ("effid","0"));
		pEO->nMapX	= atoi (pStr->ReadString ("x","1000"));
		pEO->nMapY	= atoi (pStr->ReadString ("y","1000"));
		pEO->dwTick	= atoi (pStr->ReadString ("tick","0"));
		pEO->pRunEff	= NULL;
		m_pList->Add (pEO);
	}
	pStr->Free ();
}

PathObj::PathObj()
{
	pListTalk	= xnList::Create();
	bMapTalk	= false;
	wPathQuestID = 0;
}

PathObj::~PathObj()
{
	while(pListTalk->Count)
		free(pListTalk->Delete(0));
	pListTalk->Free();
}
