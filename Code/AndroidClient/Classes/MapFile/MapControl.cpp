#include "MainMenu.h"
#include "MapControl.h"
#include "EffectObj.h"
#include "SkinConfig.h"
#include "MapObj.h"
#include "MapConfig.h"
#include "MObjActControl.h"
#include "UserData.h"
#include "MapMouse.h"
#include "ios_mapcmd_packet.h"
#include "EnemyConfig.h"
#include "MapStaticCfg.h"
#include "SoundCenter.h"
#include "MapLayer.h"
#include "SkillConfig.h"
#include "TextConfig.h"
#include "ParticleCenter.h"
#include "ReconnectGame.h"
#include "MapCopyConfig.h"

MapControl*		g_pMapCenter	= NULL;
//extern int	s_sznActionFrm[];

MapSprObj::MapSprObj()
{
	pSkin		= NULL;		//皮肤

	pSkinPA		= NULL;		//武器
	pSkinPB		= NULL;		//坐骑

	//pSprView	= NULL;
	pMObj		= NULL;
	pCountry	= NULL;
	pVipLevel	= NULL;

	byAction	= OBJ_ACTION_STAND;	//当前动作			实时更新
	byDirect	= 2;	//方向,				实时更新
	nPosX		= 2000;
	nPosY		= 1500;	//当前位置		实时更新
	byFrame		= 3;	//当前第几帧		实时更新
	nHp			= 80;
	nMaxHp		= 100;
	byUpdateFlag	= 1;
	byIsEnemy		= 0;

	//m_dwParticleID	= 0;
}

MapSprObj::~MapSprObj()
{
	if(g_pMapCenter->m_pMObjSel == this)
		g_pMapCenter->m_pMObjSel	= NULL;
}

void MapSprObj::InitSkin( int nSkin )
{
	if(pSkin && (pSkin->nId	== nSkin || nSkin == 0))
		return;
	pSkin		= CfgObjSkin::GetInstance().GetObjSkin(nSkin);
	if(pSkin	== NULL)
		pSkin		= CfgObjSkin::GetInstance().GetObjSkin(1001);
	
	dwFrmTick	= xnGetTickCount() + rand()%300;
	byFrame		= rand()%3;
}

void MapSprObj::SetSkinPA( int nSkinA )
{
	if(nSkinA == 0)
	{
		pSkinPA = NULL;
		return;
	}
	if(pSkinPA && pSkinPA->nId	== nSkinA)
		return;
	pSkinPA		= CfgObjSkin::GetInstance().GetObjSkin(nSkinA);
}

void MapSprObj::SetSkinPB( int nSkinB )
{
	if(nSkinB == 0)
	{
		pSkinPB = NULL;
		return;
	}
	if(pSkinPB && pSkinPB->nId	== nSkinB)
		return;
	pSkinPB		= CfgObjSkin::GetInstance().GetObjSkin(nSkinB);
}

int MapSprObj::GetTitleH()
{
	if(pSkinPB)
		return pSkin->nTitleH+pSkinPB->nTitleH;
	return pSkin->nTitleH;
}

//------------------------------------------------------------------------
MapControl::MapControl()
{
	m_nMapIndex			= 0;
	m_nBaseMapIndex		= 0;
	m_nX			= 2000;
	m_nY			= 1500;		//视角的坐标位置
	m_pMapTile		= NULL;
	m_pMapBarrier	= new MapBarrier();
	m_pMapAlpha		= new MapBarrier();
	m_pMapFindPath	= new MapFindPath();		//寻路模块
	m_pMapActCtrl	= new MObjActCtrl();

	m_pListTile		= xnList::Create();
	m_pListEff		= xnList::Create();		//地图光效, 玩家的技能光效(主要范围技能和飞行技能)
	
	m_pMEnemyList	= new MapAIObjManager();
	m_pMNpcList		= new MapAIObjManager();
	m_pMSlaveList	= new MapAIObjManager();
	m_pMPlayerList	= new MapPlayerManager();
	m_pListMObj		= xnList::Create();
	m_pLMapTmpEffs		= xnList::Create();
	m_pLRunEffOut	= xnList::Create();
	m_pLRunEffView	= xnList::Create();

	MapViewConfig::GetInstance().LoadFile("Map/mapconfig.ini");
	CfgObjSkin::GetInstance().InitFile("cfg/skincfg.ini");
	EffectCenter::GetInstance().InitFile("cfg/effectcfg.ini");

	m_pMObjLockView	= NULL;
	m_pMObjSel		= NULL;
	m_dwLockID		= 0;
	m_nWeatherID	= 0;
	m_nEffID		= 0;
	m_nWeatherSound	= 0;
	m_bySpEff		= 0;
	m_dwTickEnter	= xnGetTickCount();

	m_dwFrmBusyTime	= 0;
	m_dwBusyWordTick	= 0;
}

MapControl::~MapControl()
{
	delete m_pMapBarrier;
	delete m_pMapAlpha;
	m_pListTile->Free();

	while(m_pLMapTmpEffs->Count)
		delete (MapEffObj*)m_pLMapTmpEffs->Delete(0);
	m_pLMapTmpEffs->Free();

	m_pListEff->Free();
	m_pLRunEffView->Free();
	m_pLRunEffOut->Free();
}

bool MapControl::EnterMap( int nIndex,int nBase )
{
	m_pMapTile	= MapViewConfig::GetInstance().GetMapTileData(nBase);
	if(!m_pMapTile)
		return false;

	m_dwFrmBusyTime	= 0;
	m_dwBusyWordTick	= 0;
	g_pMapCommand->Reset(true);
	pChar->ChgMapEvent();
	if(ReconnectModel::GetInstance().GetModelWorked() == false)
		m_dwTickEnter	= xnGetTickCount();
	else
		ReconnectModel::GetInstance().SetModelWorked(false);
	pChar->m_bLockReconnet	= false;
	int		nOldMBaseIndex	= m_nBaseMapIndex;
	int		nOldMIndex		= m_nMapIndex;
	m_nMapIndex			= nIndex;
	m_nBaseMapIndex		= nBase;
	char	szTmp[256];
	sprintf(szTmp,"Map/%d/Barrier.bar",m_pMapTile->wRealIndex);
	m_pMapBarrier->InitMapBarrier (szTmp);
	sprintf(szTmp,"Map/%d/alphabar.bar",m_pMapTile->wRealIndex);
	m_pMapAlpha->InitMapBarrier (szTmp);

	m_pMapFindPath->InitBarrier(m_pMapBarrier);
	g_pSoundCenter->PlayerBackMp3(m_pMapTile->m_szBackMp3,true);
	MapEffectCfg::GetInstance().LoadFile(m_pMapTile->wRealIndex);

	if(pMapLayer)
		pMapLayer->InitMap();
	m_pLRunEffView->Clear();//pList,m_pLRunEffOut);
	m_pLRunEffOut->Clear();

	pMainMenu->m_pMiniMap->m_pDCompMName->SetCaption(m_pMapTile->m_szName,1);

	//每次进地图都清除重新加载文件
	m_pListMapEffStatic	= MapEffectCfg::GetInstance().m_pList;

	//pParticleCenter->m_nMapSizeH	= m_pMapTile->m_nTHeight;
	pParticleCenter->ResetParticle();
	g_pMapCenter->m_nWeatherID	= 0;
	if(g_pMapCenter->m_nWeatherSound)
	{
		g_pSoundCenter->StopEffect(g_pMapCenter->m_nWeatherSound);
		g_pMapCenter->m_nWeatherSound	= 0;
	}
	//pParticleCenter->RunParticle("other/snow.plist",nowScreenWidth/2,0,0,true);
	
	m_pListMapTP	= TransPointCfg::GetInstance().GetMapTP(m_pMapTile->wIndex);
	//传送点的光效加入永久光效中
	if(m_pListMapTP)
		for (int i = 0; i < m_pListMapTP->Count; i++)
		{
			_TransPoint*	pTP	= (_TransPoint*)m_pListMapTP->Items[i];
			_MapEffectObj*	pMEO	= new _MapEffectObj();
			pMEO->nMapX			= pTP->nPosX;
			pMEO->nMapY			= pTP->nPosY;
			pMEO->nEffectID		= pTP->wEffID;
			pMEO->dwTick		= 0;
			pMEO->pRunEff		= NULL;
			m_pListMapEffStatic->Add(pMEO);
		}

	//初始化NPC和传送点地图光效物件
	while(m_pLMapTmpEffs->Count)
		delete (_MapEffectObj*)m_pLMapTmpEffs->Delete(0);

	m_pMEnemyList->ClearMapObj();
	m_pMPlayerList->ClearMapObj();
	m_pMSlaveList->ClearMapObj();
	m_pMNpcList->ClearMapObj();

	xnList*	pListTmp	= xnList::Create();
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(m_nBaseMapIndex,pListTmp);
	for (int i = 0; i < pListTmp->Count; i++)
	{
		PathObj* pPath	= (PathObj*)pListTmp->Items[i];

		MapAIObj* pObj	= (MapAIObj*)m_pMNpcList->AddMapObj(pPath->nID,MAPOBJ_NPC);
		if(pObj)
		{
			pObj->nPosX			= pPath->nPosX;
			pObj->nPosY			= pPath->nPosY;
			pObj->wSpeed		= 0;
			pObj->m_szwSkin[0]	= pPath->nSkin;
			pObj->pPathObj		= pPath;
			//pObj->pBaseCfg		= pPath;
			pObj->bForce		= 0;
			pObj->m_byDir		= 4;
			pObj->nHP			= 99999;
			pObj->nMaxHP		= 99999;
			pObj->byLv			= pPath->nLv;
			strncpy(pObj->m_szName,pPath->szName,20);
			if (pPath->nEff)
				pObj->AddEff(pPath->nEff);
		}
	}
	pListTmp->Free();

	pMainMenu->m_pMiniMap->m_pFormBig->Close();
	pMainMenu->m_pMiniMap->ChgMap(nBase);
	pMainMenu->m_pMCAwardMenu->m_pForm->Close();
	pMainMenu->m_pAutoMenu->SwitchActive(false);

	
	bool	bGreenOpened	= false;
	if(nOldMIndex >= 1000)
	{
		MCopyBase* pMBase	= MCopyConfig::GetInstance().GetMCopyByMBase(nOldMBaseIndex);
		if(pMBase && pMBase->byType == 5)
		{
			pMainMenu->m_pGreenModeMenu->m_pForm->Open();
			bGreenOpened	= true;
		}
	}
	if(!bGreenOpened)
		pMainMenu->m_pGreenModeMenu->m_pForm->Close();
	
	if (nOldMBaseIndex == 27)
		pMainMenu->m_pWarRankMenu->Open();
	
	pMainMenu->CloseMessageBox();

	pMainMenu->m_pActivityCtrl->Close();
	pMainMenu->m_pActivityJoinMenu->m_pForm->Close();

	if(nIndex >= 1000)
	{
		pMainMenu->m_pMapCopyMenu->m_pForm->Close();
		pMainMenu->m_pCtrlMenu->m_pCompMCopy->SetVisible(true);
	}
	else
	{
		pMainMenu->m_pCtrlMenu->m_pCompMCopy->SetVisible(false);
	}
	pChar->m_bLockJump	= true;
	return true;
}

void MapControl::Update()
{
	if(m_dwBusyWordTick && xnGetTickCount() - m_dwBusyWordTick > 120000)
	{//优化2分钟之后
		m_dwBusyWordTick	= 0;
		m_dwFrmBusyTime		= 0;
	}
	if(m_dwFrmBusyTime >= 20 && m_dwBusyWordTick == 0)
	{
		m_dwBusyWordTick = xnGetTickCount();
	}

	if(m_pMObjLockView == NULL)
	{
		if(m_dwLockID != 0)
		{
			m_pMObjLockView	= GetMapObj(m_byLockType,m_dwLockID);
		}
		if(!m_pMObjLockView)
			return;
	}
	
	m_pMEnemyList->Update();
	m_pMNpcList->Update();
	m_pMSlaveList->Update();
	m_pMPlayerList->Update();

	//获取界面绘制的地表信息
	m_pMapActCtrl->Update();
	if(m_pMObjLockView == NULL)
		return;

	m_nX	= m_pMObjLockView->nPosX-SCREEN_SIZE_W*POINT_PIX_CNT/2;
	m_nY	= m_pMObjLockView->nPosY+SCREEN_SIZE_H*POINT_PIX_CNT/2;

	if(m_nX < 0)
		m_nX	= 0;
	if(m_nY < SCREEN_SIZE_H*POINT_PIX_CNT)
		m_nY	= SCREEN_SIZE_H*POINT_PIX_CNT;
	if(m_nX > GetMapWidth()-SCREEN_SIZE_W*POINT_PIX_CNT)
		m_nX	= GetMapWidth() - SCREEN_SIZE_W*POINT_PIX_CNT;
	if(m_nY > GetMapHeight())
		m_nY	= GetMapHeight();

	//获取显示的内容
	m_pMapTile->GetInViewTile(m_nX-SCREEN_SIZE_W*POINT_PIX_CNT/4,m_nY-(SCREEN_SIZE_H*POINT_PIX_CNT*5/4),m_nX+(SCREEN_SIZE_W* POINT_PIX_CNT*5/4),m_nY+SCREEN_SIZE_H*POINT_PIX_CNT/4,m_pListTile);
	m_pMapActCtrl->GetInViewObj(m_nX-SCREEN_SIZE_W*POINT_PIX_CNT/4,m_nY-(SCREEN_SIZE_H*POINT_PIX_CNT*5/4),m_nX+(SCREEN_SIZE_W* POINT_PIX_CNT*5/4),m_nY+SCREEN_SIZE_H*POINT_PIX_CNT/4,m_pListMObj);


	xnList*	pList	= xnList::Create();
	_checkEffInView(m_nX-SCREEN_SIZE_W*POINT_PIX_CNT/4,m_nY-(SCREEN_SIZE_H*POINT_PIX_CNT*5/4),m_nX+(SCREEN_SIZE_W* POINT_PIX_CNT*5/4),m_nY+SCREEN_SIZE_H*POINT_PIX_CNT/3,pList);
	
	m_pLRunEffOut->Clear();
	m_pLRunEffView->CheckMyOnly(pList,m_pLRunEffOut);
	m_pLRunEffView->Free();
	m_pLRunEffView	= pList;
}

bool MapControl::LinkMapObj( MapAIObj* pObj )
{
	return m_pMapActCtrl->LinkCtrl(pObj);
}

bool MapControl::DeLinkMapObj( MapObj* pObj )
{
	return m_pMapActCtrl->DisLinkCtrl(pObj);
}

bool MapControl::CheckSelObj( int nX, int nY )
{
	MapSprObj*	pSelObj	= m_pMapActCtrl->CheckSelObj(nX,nY);
	if(pSelObj)
	{
		if(m_pMObjSel	== pSelObj)
		{//ATK
			g_pMapCommand->CmdNormalAtk((MapAIObj*)m_pMObjSel->pMObj);
		}
		else
		{
			m_pMObjSel	= pSelObj;
			pChar->m_bManualSel	= true;
		}
		return true;
	}
	return false;
}

void MapControl::AddMapEff( int nEffID, int nPosX, int nPosY, DWORD dwTick )
{
	if(nPosX == 0 || nPosY == 0)
		return;
	MapEffObj*	pMEff	= new MapEffObj();
	//pMEff->nDestX	= nDX;
	//pMEff->nDestY	= nDY;
	pMEff->nPosX	= nPosX;
	pMEff->nPosY	= nPosY;
	pMEff->nEffID	= nEffID;
	m_pLMapTmpEffs->Add(pMEff);
}

void MapControl::AddCharNAtkEff(MapAIObj* pObjA,MapAIObj* pObjB)
{
	if (!pObjA||!pObjB)
		return;

	if (pObjA->m_byType != MAPOBJ_PLAYER)
		return;

	int nCharJob = ((MapPlayer*)pObjA)->byJob;
	int nEff = 0;
	if (nCharJob == 2)
		nEff = 272;
	if (nCharJob == 3)
		nEff = 273;
	if (nCharJob == 4)
		nEff = 274;

	if (nEff)
		pObjB->AddEff(nEff,pObjA->nPosX-pObjB->nPosX,pObjA->nPosY-pObjB->nPosY);
}

bool MapControl::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != MAPCMD)
		return false;

	switch(pPacket->bCmd)
	{
	case MS_ENEMYSYNC:	//怪物信息包
		{
			SET_DATA(pData, MAPCMD, MS_ENEMYSYNC, pPacket);
			EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pData->wBaseID);
			if(pBase == NULL)
				return true;
			MapAIObj*	pObj	= (MapAIObj*)m_pMEnemyList->AddMapObj(pData->dwID,MAPOBJ_ENEMY);
			pObj->nPosX			= pData->wPosX;
			pObj->nPosY			= pData->wPosY;
			pObj->wSpeed		= pData->wSpeed;
			pObj->m_szwSkin[0]	= pBase->wSkin;
			pObj->m_szwSkin[1]	= pBase->wSkinEx;
			pObj->nHP			= pData->nHp;
			if(pBase->nMaxHp == 0)
				pObj->nMaxHP	= pObj->nHP;
			else
				pObj->nMaxHP	= pBase->nMaxHp;
			if(pObj->nHP > pObj->nMaxHP)
				pObj->nMaxHP	= pObj->nHP;
			pObj->pBaseCfg		= pBase;
			pObj->pPathObj		= MonsterPathLeadCfg::GetInstance().GetPathObj(pData->wBaseID);
			pObj->bForce		= pData->bForce;
			pObj->byCountry		= pData->bRandMove;
			//===zjk
			pObj->enemyID       = pBase->nID;
			pObj->dwMastUin		= pData->dwMastID;

			if (m_nBaseMapIndex == 9)// 军团副本怪等级可变
			{
				pObj->byLv = pChar->GetHomeCopyLv();
			}else
				pObj->byLv			= pBase->bLvl;
			//pObj->m_szwSkin[0]	= pData->wBaseID;
			if(strlen(pBase->szName) == 0)
				strncpy(pObj->m_szName,RandNameConfig::GetInstance().GetRndName(rand()%2),20);
			else
				strncpy(pObj->m_szName,pBase->szName,20);

			if (strlen(pData->szTitle) != 0)
				strncpy(pObj->m_szTitle,pData->szTitle,32);
			if( pBase->bType == 2)
				pObj->m_bySPEff		= 1;
			else
				pObj->m_bySPEff		= 0;

			pObj->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);

			AskBornEff(pObj);

 			pObj->pStateCenter->ClearState();
 			for (int i = 0; i < pData->byNewSCnt; i++)
 			{
 				pObj->pStateCenter->AddState(pData->wNewStateID[i]);
 			}
		}
		return true;
	case MS_ENEMYSYNCEX:	//怪物信息包
		{
			SET_DATA(pData, MAPCMD, MS_ENEMYSYNCEX, pPacket);
			EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pData->wBaseID);
			if(pBase == NULL)
				return true;
			MapAIObj*	pObj	= (MapAIObj*)m_pMEnemyList->AddMapObj(pData->dwID,MAPOBJ_ENEMY);
			pObj->nPosX			= pData->wPosX;
			pObj->nPosY			= pData->wPosY;
			pObj->wSpeed		= pData->wSpeed;
			pObj->m_szwSkin[0]	= pData->szSkin[0];
			pObj->m_szwSkin[1]	= pData->szSkin[1];
			pObj->m_szwSkin[2]	= pData->szSkin[2];
			pObj->nHP			= pData->nHp;
			pObj->nMaxHP		= pData->nMaxHp;
			pObj->pBaseCfg		= pBase;
			pObj->pPathObj		= MonsterPathLeadCfg::GetInstance().GetPathObj(pData->wBaseID);
			pObj->bForce		= pData->bForce;
			
			//===zjk
			pObj->enemyID       = pBase->nID;
			pObj->dwMastUin		= pData->dwMastID;
			pObj->byCountry		= 0;

			pObj->byLv			= pBase->bLvl;
			//pObj->m_szwSkin[0]	= pData->wBaseID;
			if(strlen(pBase->szName) == 0)
				strncpy(pObj->m_szName,RandNameConfig::GetInstance().GetRndName(rand()%2),20);
			else
				strncpy(pObj->m_szName,pBase->szName,20);
			if (pBase->nID >= 8002&&pBase->nID <= 8005)
			{
				strncpy(pObj->m_szName,pMainMenu->m_pWarRankMenu->m_szName,20);
				pObj->byLv			= pData->byLv;
			}

			pObj->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);

			AskBornEff(pObj);

			pObj->pStateCenter->ClearState();
			for (int i = 0; i < pData->byStateCnt; i++)
			{
				pObj->pStateCenter->AddState(pData->wStateID[i]);
			}
		}
		return true;
	case MS_SLAVESYNC:	//宠物包
		{
			SET_DATA(pData, MAPCMD, MS_SLAVESYNC, pPacket);
			//EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pData->wBaseID);
			//if(pBase)
			//	return true;
			MapAIObj*	pObj	= (MapAIObj*)m_pMSlaveList->AddMapObj(pData->dwID,MAPOBJ_SLAVE);
			pObj->nPosX			= pData->wPosX;
			pObj->nPosY			= pData->wPosY;
			pObj->wSpeed		= pData->wSpeed;
			pObj->nHP			= pData->nHp;
			pObj->nMaxHP		= pData->nHpMax;
			pObj->byLv			= pData->bLv;
			pObj->bForce		= pData->bForce;
			pObj->dwMastUin		= pData->dwMastID;
			pObj->nMarryEff		= pData->nMarryEff;
			//pObj->m_szwSkin[0]	= pBase->wSkin;
			//pObj->pBaseCfg		= pBase;
			pObj->m_szwSkin[0]	= pData->wSkin;
			strncpy(pObj->m_szName,pData->szName,20);
			if(pObj->dwMastUin == pChar->m_dwUin)
			{
				sprintf(pObj->m_szTitle,"%s的宠物",pChar->m_szNick);
			}
			else
			{
				MapPlayer* pPlayer	= (MapPlayer*)m_pMPlayerList->GetMapObj(pObj->dwMastUin);
				if(pPlayer)
					sprintf(pObj->m_szTitle,"%s的宠物",pPlayer->m_szName);
				else
					strcpy(pObj->m_szTitle,"玩家的宠物");
			}
			
			pObj->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);
			AskBornEff(pObj);

			if(pObj->nMarryEff > 0)
				pObj->AddEff(pObj->nMarryEff);

			
			pObj->pStateCenter->ClearState();
			for (int i = 0; i < pData->byStateCnt; i++)
			{
				pObj->pStateCenter->AddState(pData->wStateID[i]);
			}
		}
		return true;
	case MS_JUMPINMAP:
		{
			SET_DATA(pData, MAPCMD, MS_JUMPINMAP, pPacket);

			MapAIObj* pAIObj	= GetMapObj(pData->byType,pData->dwUin);
			//MapAIObj* pAIObj	= (MapAIObj*)m_pMPlayerList->AddMapObj(pData->dwUin,pData->byType);
			if(pAIObj)
			{
				pAIObj->nPosX		= pData->wPosX;
				pAIObj->nPosY		= pData->wPosY;
				AskBornEff(pAIObj);
			}
		}
		return true;
	case MS_RUSHINMAP:
		{
			SET_DATA(pData, MAPCMD, MS_RUSHINMAP, pPacket);
			MapAIObj* pAIObj	= GetMapObj(pData->byType,pData->dwUin);
			//MapAIObj* pAIObj	= (MapAIObj*)m_pMPlayerList->AddMapObj(pData->dwUin,pData->byType);
			if(pAIObj)
			{
				pAIObj->SetRush(pData->wPosX,pData->wPosY,pData->bAction);
			}
		}
		return true;
	case MS_USERSYNC:	//玩家信息包
		{
			SET_DATA(pData, MAPCMD, MS_USERSYNC, pPacket);
			MapPlayer* pPlayer	= (MapPlayer*)m_pMPlayerList->AddMapObj(pData->dwUin,MAPOBJ_PLAYER);
			pPlayer->nPosX		= pData->wPosX;
			pPlayer->nPosY		= pData->wPosY;
			memcpy(pPlayer->m_szwSkin,pData->wSkin,6);
			pPlayer->wSpeed		= pData->wSpeed;
			pPlayer->nHP		= pData->nHp;
			pPlayer->nMaxHP		= pData->nHpMax;
			pPlayer->byLv		= pData->bLv;
			pPlayer->byJob		= pData->bJob;
			pPlayer->bySex		= pData->bSex;
			pPlayer->bForce		= pData->bForce;
			pPlayer->byCountry	= pData->byCountry;
			pPlayer->byViplv	= pData->byViplv;
			pPlayer->nMarryEff	= pData->nMarryEff;


			if(strlen(pData->szName) == 0)
			{
				char*	szName	= RandNameConfig::GetInstance().GetRndName(pPlayer->bySex,pData->szTitle[0],pData->szTitle[1],pData->szTitle[2]);
				strncpy(pPlayer->m_szName,szName,20);
				strcpy(pPlayer->m_szTitle,"平民");
			}
			else
			{
				strncpy(pPlayer->m_szName,pData->szName,20);
				strncpy(pPlayer->m_szTitle,pData->szTitle,32);
				strncpy(pPlayer->m_szMarryTitle,pData->szMarryTitle,32);
			}

			pPlayer->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);
			

			AskBornEff(pPlayer);
			pPlayer->pStateCenter->ClearState();
			for (int i = 0; i < pData->byStateCnt; i++)
			{
				pPlayer->pStateCenter->AddState(pData->wStateID[i]);
			}
		}
		return true;
	case MS_PARAMCHG:
		{
			SET_DATA(pData, MAPCMD, MS_PARAMCHG, pPacket);
			MapAIObj* pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				if(pObj->m_byType == MAPOBJ_ENEMY)
				{
					if(pData->wSkin[0] == pObj->pBaseCfg->nID)
					{
						memset(pObj->m_szwSkin,0,6);

						pObj->m_szwSkin[0]	= pObj->pBaseCfg->wSkin;
						pObj->m_szwSkin[1]	= pObj->pBaseCfg->wSkinEx;
					}
					else
						memcpy(pObj->m_szwSkin,pData->wSkin,6);
				}
				else
					memcpy(pObj->m_szwSkin,pData->wSkin,6);
				
				pObj->wSpeed		= pData->wSpeed;
				pObj->bSpeedChged	= true;
				pObj->nHP		= pData->nHp;
				pObj->nMaxHP		= pData->nMaxHp;
				pObj->byLv			= pData->byLv;

			}
		}
		return true;
	case MS_TITLECHG:
		{
			SET_DATA(pData,MAPCMD,MS_TITLECHG,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				strncpy(pObj->m_szTitle,pData->szTitle,32);
			}
		}
		return true;
	case MS_MARRYTITLECHG:
		{
			SET_DATA(pData,MAPCMD,MS_MARRYTITLECHG,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				strncpy(pObj->m_szMarryTitle,pData->szTitle,32);
			}
		}
		return true;
	case MS_CHGBTGROUP:
		{
			SET_DATA(pData,MAPCMD,MS_CHGBTGROUP,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->byCountry	= pData->bRandW;
				pObj->dwMastUin	= pData->dwMast;
			}
		}
		return true;
	case MS_MARRYEFFCHG:
		{
			SET_DATA(pData,MAPCMD,MS_MARRYEFFCHG,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->nMarryEff = pData->nEff;
				if(pObj->nMarryEff > 0)
				{
					MapSprObj*	pSprT	= g_pMapCenter->m_pMapActCtrl->GetObjByID(MAPOBJ_PLAYER,pObj->nMarryEff);
					MapSprObj*	pSprF	= g_pMapCenter->m_pMapActCtrl->GetObjByID(MAPOBJ_PLAYER,pData->dwID);
					MapPlayer* pTPlayer	= (MapPlayer*)m_pMPlayerList->GetMapObj(pObj->nMarryEff);

					if(pSprF && pSprT && pTPlayer && pTPlayer->nMarryEff == pData->dwID)
						g_pMapCenter->AddMapEff(265,(int)pSprF,(int)pSprT,-1);
				}
			}
		}
		return true;
	case MS_VIPLVCHG:
		{
			SET_DATA(pData,MAPCMD,MS_VIPLVCHG,pPacket);
			MapAIObj* pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->byViplv = pData->byViplv;
			}
		}
		return true;
	case MS_SPVAL:
		{
			SET_DATA(pData,MAPCMD,MS_SPVAL,pPacket);

			pChar->m_nSPVal	= pData->wSpVal;
		}
		return true;
	case MS_STRIKESKILL:
		{
			SET_DATA(pData,MAPCMD,MS_STRIKESKILL,pPacket);
			pMainMenu->m_pCtrlMenu->StrikeSkill(pData->wSkill);
		}
		return true;
	case MS_SETLOCKEFF:
		{
			SET_DATA(pData,MAPCMD,MS_SETLOCKEFF,pPacket);
			if(pData->byType < 7)
			{
				pChar->m_szbyActLock[pData->byType]	= pData->byLocked;
				if (pData->byType == 6)
					pChar->m_bHorseState = pData->byLocked;
			}
		}
		return true;
	case MS_ACTSKILL:
		{
			SET_DATA(pData, MAPCMD, MS_ACTSKILL, pPacket);
			pChar->m_pSkillCenter->ClearActSkill();		
			for (int i = 0; i < pData->bSCnt; i++)
			{
				pChar->m_pSkillCenter->SetActSkill(pData->szSkill[i].wSkillID,pData->szSkill[i].bLv,pData->szSkill[i].bPos,pData->szSkill[i].nCDTick);
			}
			pMainMenu->m_pCtrlMenu->ResetSkill();
		}
		return true;
	case MS_ENEMYCMD:	//行为方针变化
		{
			SET_DATA(pData, MAPCMD, MS_ENEMYCMD, pPacket);
			MapAIObj* pObj	= GetMapObj(MAPOBJ_ENEMY,pData->dwID);
			if(pObj)
			{
				pObj->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);
			}
		}
		return true;
	case MS_SLAVECMD:	//
		{
			SET_DATA(pData, MAPCMD, MS_SLAVECMD, pPacket);
			MapAIObj* pObj	= GetMapObj(MAPOBJ_SLAVE,pData->dwID);
			if(pObj)
			{
				pObj->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);
			}
		}
		return true;
	case MS_USERCMD:	//
		{
			SET_DATA(pData, MAPCMD, MS_USERCMD, pPacket);
			MapAIObj* pObj	= GetMapObj(MAPOBJ_PLAYER,pData->dwID);
			if(pObj)
			{
				pObj->objAIModel.SetCmd(pData->bCmd,pData->dwCmdVal);
			}
		}
		return true;
	case MS_OBJOUT:	//
		{
			SET_DATA(pData, MAPCMD, MS_OBJOUT, pPacket);
			if(pData->dwMIndex == g_pMapCenter->m_nMapIndex)
				RemoveMapObj(pData->byType,pData->dwID);
		}
		return true;
	case MS_WEATHCHG:
		{
			SET_DATA(pData, MAPCMD, MS_WEATHCHG, pPacket);
			g_pMapCenter->SetWeather(pData->byWeath);
		}
		return true;
	case MS_CTRLINFO:	//操控对象的血量,蓝量, 技能信息,快捷物品信息,坐标(进入地图和切换控制的时候需要)
		{
			SET_DATA(pData, MAPCMD, MS_CTRLINFO, pPacket);

			g_pMapCenter->EnterMap(pData->dwMIndex,pData->wRMIndex);
			g_pMapCenter->SetWeather(pData->byWeath);
			m_dwLockID		= pData->dwID;
			m_byLockType	= pData->byType;
			pChar->m_dwCmdLock	= 0;
			g_pMapCenter->m_pMObjLockView	= NULL;//GetMapObj(pData->byType,pData->dwID);

			pChar->m_nMaxMp	= pData->nMpMax;
			pChar->m_nMp	= pData->nMp;
			pChar->m_nSPVal	= pData->wSP;
			pChar->m_wCtrlAtkDis	= pData->wAtkDis;
			pMainMenu->m_pCtrlMenu->m_pCompName->SetCaption(pData->szName,1);

			pChar->m_pSkillCenter->ClearActSkill();
			//memset(&pChar->m_szbyActLock,0,7);
			pChar->ChgMapEvent();
			for (int i = 0; i < pData->bSCnt; i++)
			{
				pChar->m_pSkillCenter->SetActSkill(pData->szSkill[i].wSkillID,pData->szSkill[i].bLv,pData->szSkill[i].bPos,pData->szSkill[i].nCDTick);
				//pChar->m_pListSkill->Add((void*)pData->wLinks[i]);
			}
			pMainMenu->m_pCtrlMenu->ResetSkill();
		}
		return true;
	case MS_ATKINFO:
		{//需要攻击动作,技能过程,受击掉血
			SET_DATA(pData, MAPCMD, MS_ATKINFO, pPacket);
			MapAIObj*	pObjAtk		= GetMapObj(pData->bTSrc,pData->dwIDSrc);
			MapAIObj*	pObjBeHit	= GetMapObj(pData->bTDest,pData->dwIDDest);

			if(pObjAtk == pChar->GetMyMapObj())
			{//攻击者为自己
				pChar->m_dwTickBattle	= xnGetTickCount();
				if(pObjBeHit && pObjBeHit->nHP < pData->nDamage && pObjBeHit->m_byType == MAPOBJ_ENEMY)
				{//杀死怪物
					if(pObjBeHit->pBaseCfg->bType != 3)
						g_pMapCenter->PlayFuryEff(pObjBeHit->nPosX,pObjBeHit->nPosY);
				}
			}
			if(pObjBeHit == pChar->GetMyMapObj())
				pChar->m_dwTickBattle	= xnGetTickCount();

			SkillBase*	pSBase	= NULL;
			if(pData->wSkill)
			{
				pSBase	= SkillConfig::GetInstance().GetSkillBase(pData->wSkill);
			}
			if(pData->bAoeHit == 0)
			{
				if(pObjAtk)
				{
					pObjAtk->objAIModel.SetCmd(MCMD_NULL,0);
					if(pObjAtk != pChar->GetMyMapObj())
					{
						if(pData->wSkill)
						{
							pObjAtk->SetSkillAtk(pObjBeHit);
						}
						else
						{
							pObjAtk->SetNAtk(pObjBeHit);
							AddCharNAtkEff(pObjAtk,pObjBeHit);
						}
					}
					else
					{
						if(pData->wSkill)
							pChar->m_pSkillCenter->ResetSkillCD(pData->wSkill);
					}
				}
			}
			else
			{
				if(pObjBeHit && (pData->nDamage || pData->bModel==3))
				{
					if(pSBase)
						pObjBeHit->pLHitRequest->Add((void*)(xnGetTickCount()+pSBase->dwEndDelay));
					else
						pObjBeHit->pLHitRequest->Add((void*)xnGetTickCount());
				}
			}

			if(pData->nDamage || pData->bModel==3)
			{
				if(pObjBeHit && CheckDispHitNum(pObjAtk,pObjBeHit))
				{
					pObjBeHit->AddDelayHit(pData->nDamage,pData->bModel);
				}
				else if(pObjBeHit)
				{
					pObjBeHit->AddDelayHit(pData->nDamage,99);
				}
			}


			
			if(pSBase)
			{//播放技能效果
				if(pObjAtk)
				{
					if(pSBase->byStartEffDir && pObjBeHit)
						pObjAtk->AddEff(pSBase->wStartEff,pObjAtk->nPosX-pObjBeHit->nPosX,pObjAtk->nPosY-pObjBeHit->nPosY,0,false);
					else
						pObjAtk->AddEff(pSBase->wStartEff);

					if(pSBase->wFlyEff && pObjBeHit)
					{
						EffectObj*	pEObj	= EffectCenter::GetInstance ().GetEffObj (pSBase->wFlyEff);
						if(pEObj && pEObj->m_nRunType == 7)
						{
							MapSprObj*	pSprF	= g_pMapCenter->m_pMapActCtrl->GetObjByID(pObjAtk->m_byType,pObjAtk->m_nID);
							MapSprObj*	pSprT	= g_pMapCenter->m_pMapActCtrl->GetObjByID(pObjBeHit->m_byType,pObjBeHit->m_nID);

							g_pMapCenter->AddMapEff(pSBase->wFlyEff,(int)pSprF,(int)pSprT,-1);
						}
						else
						{
							if(pData->bAoeHit == 0)
							{
								if(pSBase->dwEndDelay)
									pObjBeHit->AddEff(pSBase->wFlyEff,pObjAtk->nPosX-pObjBeHit->nPosX,pObjAtk->nPosY-pObjBeHit->nPosY);
								else
									pObjBeHit->AddEff(pSBase->wFlyEff);
							}
						}
					}
				}
				if(pObjBeHit)
				{//击中光效
					pObjBeHit->AddEff(pSBase->wEndEff,0,0,pSBase->dwEndDelay);
				}
			}
			
		}
		return true;
	case MS_HEALTHHPMP:
		{
			SET_DATA(pData,MAPCMD,MS_HEALTHHPMP,pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->byType,pData->dwUin);
			if(pObj)
				pObj->AddDelayHit(-pData->nAddHp,1);
		}
		return true;
	case MS_CESKILLCD:
		{
			SET_DATA(pData, MAPCMD, MS_CESKILLCD, pPacket);
			pChar->m_pSkillCenter->ResetSkillCD(pData->wSkill);
		}
		return true;
	case MS_HPINFO:
		{
			SET_DATA(pData, MAPCMD, MS_HPINFO, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->nHP	= pData->nHp;
				if(pObj->nHP <= 0)
				{
					pObj->ObjDead();
				}
			}
		}
		return true;
	case MS_HPMAXINFO:
		{
			SET_DATA(pData, MAPCMD, MS_HPMAXINFO, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->nHP	= pData->nHp;
				pObj->nMaxHP	= pData->nMaxHp;
				if(pObj->nHP <= 0)
				{
					pObj->ObjDead();
				}
			}
		}
		return true;
	case MS_USER_PARAMS:
		{
			SET_DATA(pData, MAPCMD, MS_USER_PARAMS, pPacket);
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_HP] = pData->nHpMax;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_MP] = pData->nMpMax;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_ATK_W] = pData->nAtkW;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_ATK_N] = pData->nAtkN;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_DEF_W] = pData->nDefW;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_DEF_N] = pData->nDefN;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_HIT] = pData->nAim;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_MISS] = pData->nDodge;
			pChar->m_xUserParam2.nParam[EQUIP_ATTR_CRIT] = pData->nCrit;
			pChar->m_xUserParam2.nSpeed = pData->nMSpeed;
			pMainMenu->m_pUserParamMenu->m_bUpdate = true;
		}
		return true;
	case MS_OBJCHGFORCE:
		{
			SET_DATA(pData, MAPCMD, MS_OBJCHGFORCE, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->bForce	= pData->bForce;
			}
		}
		return true;
	case MS_OBJCHGCTRL:
		{
			SET_DATA(pData, MAPCMD, MS_OBJCHGCTRL, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->dwMastUin	= pData->dwMast;
				pObj->bForce	= pData->bForce;
				if(pObj->m_byType == MAPOBJ_SLAVE)
				{
					if(pObj->dwMastUin == pChar->m_dwUin)
					{
						sprintf(pObj->m_szTitle,"%s的宠物",pChar->m_szNick);
					}
					else
					{
						MapPlayer* pPlayer	= (MapPlayer*)m_pMPlayerList->AddMapObj(pObj->dwMastUin,MAPOBJ_PLAYER);
						if(pPlayer)
							sprintf(pObj->m_szTitle,"%s的宠物",pPlayer->m_szName);
						else
							strcpy(pObj->m_szTitle,"玩家的宠物");
					}
				}
			}
		}
		return true;
	case MS_STATEINFO:
		{
			SET_DATA(pData, MAPCMD, MS_STATEINFO, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->pStateCenter->ClearState();
				for (int i = 0; i < pData->bCnt; i++)
				{
					pObj->pStateCenter->AddState(pData->szwStates[i]);
				}
			}
		}
		return true;
	case MS_STATEDEL:
		{
			SET_DATA(pData, MAPCMD, MS_STATEDEL, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->pStateCenter->RemoveState(pData->wState);
			}
		}
		return true;
	case MS_STATECHG:
		{
			SET_DATA(pData, MAPCMD, MS_STATECHG, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->bType,pData->dwID);
			if(pObj)
			{
				pObj->pStateCenter->AddState(pData->wState);
			}
		}
		return true;
	case MS_MPINFO:
		{
			SET_DATA(pData, MAPCMD, MS_MPINFO, pPacket);

			pChar->m_nMp	= pData->nMp;
			pChar->m_nMaxMp	= pData->nMaxMp;
		}
		return true;
	case MS_SPEEDCHG:
		{
			SET_DATA(pData, MAPCMD, MS_SPEEDCHG, pPacket);
			MapAIObj*	pObj	= GetMapObj(pData->byType,pData->dwID);
			if(pObj)
			{
				pObj->wSpeed	= pData->wSpeed;
				pObj->bSpeedChged	= true;
			}
		}
		return true;
		//MS_USKILL,			//对象放技能
		//MS_STATEINFO,		//状态信息
		//MS_STATECHG,		//状态改变和消失
		//MS_STATEDEL,
		//MS_HPLIST,			//血量改变信息合包发
	}
	return true;
}

MapAIObj* MapControl::GetMapObj( BYTE byType, DWORD dwID )
{
	switch(byType)
	{
	case MAPOBJ_PLAYER:
	case MAPOBJ_ROBOT:
		return (MapAIObj*)m_pMPlayerList->GetMapObj(dwID);
	case MAPOBJ_ENEMY:
		return (MapAIObj*)m_pMEnemyList->GetMapObj(dwID);
	case MAPOBJ_SLAVE:
		return (MapAIObj*)m_pMSlaveList->GetMapObj(dwID);
	case MAPOBJ_NPC:
		return (MapAIObj*)m_pMNpcList->GetMapObj(dwID);
	}
	return NULL;
}

bool MapControl::RemoveMapObj( BYTE byType, DWORD dwID )
{
	switch(byType)
	{
	case MAPOBJ_PLAYER:
	case MAPOBJ_ROBOT:
		return m_pMPlayerList->DelMapObj(dwID);
	case MAPOBJ_ENEMY:
		return m_pMEnemyList->DelMapObj(dwID);
	case MAPOBJ_SLAVE:
		return m_pMSlaveList->DelMapObj(dwID);
	}
	return NULL;
}

bool MapControl::CheckDispHitNum( MapAIObj* pA, MapAIObj* pB )
{
	if(!m_pMObjLockView)
		return false;

	if(pA == m_pMObjLockView || pB == m_pMObjLockView)
		return true;
	
	if((pA && pA->dwMastUin == m_pMObjLockView->m_nID) || (pB && pB->dwMastUin == m_pMObjLockView->m_nID))
		return true;
	return false;
}

void MapControl::AskBornEff( MapAIObj* pObj )
{
	if(xnGetTickCount() - m_dwTickEnter < 1000)
		return;
	
	if(pObj->nPosX > m_nX && pObj->nPosY > m_nY-SCREEN_SIZE_H*POINT_PIX_CNT
		&& pObj->nPosX < m_nX+SCREEN_SIZE_W*POINT_PIX_CNT && pObj->nPosY < m_nY)
	{
		//在屏幕范围内刷出来才算出生
		pObj->AddEff(25);
	}
}

void MapControl::SearchNearObj( BYTE bEnemy,DWORD	dwMaxLen, DWORD dwQusEnemyID )
{
	if(!m_pMObjLockView)
		return;

	m_pMObjSel	= m_pMapActCtrl->SearchObj(m_pMObjLockView->nPosX,m_pMObjLockView->nPosY,bEnemy,dwMaxLen,dwQusEnemyID);
}

void MapControl::_checkEffInView( int nPosX, int nPosY,int nEX,int nEY,xnList* pList )
{
	for (int i = 0; i < m_pListMapEffStatic->Count; i++)
	{
		_MapEffectObj* pMEO	= (_MapEffectObj*)m_pListMapEffStatic->Items[i];
		if(pMEO->nMapX >= nPosX && pMEO->nMapX <= nEX && pMEO->nMapY >= nPosY && pMEO->nMapY <= nEY)
		{
			pList->Add(pMEO);
		}
	}
}

void MapControl::SetWeather( BYTE bVal )
{
	if(m_nWeatherID)
	{
		if(m_nWeatherID == 11)
		{
			pParticleCenter->StopParticle(m_nEffID);
			pParticleCenter->StopParticle(m_nEffID+1);
		}
		else
			pParticleCenter->StopParticle(m_nEffID);
	}
	if(m_nWeatherSound)
		g_pSoundCenter->StopEffect(m_nWeatherSound);

	m_nWeatherSound	= 0;
	m_nWeatherID	= bVal;
	m_nEffID = 0;
	switch(bVal)
	{
	case 0:		//无
		m_nWeatherID	= 0;
		break;
	case 3:		//气泡
		{
			char	szTmp[64];
			sprintf(szTmp,"other/weather%d.plist",bVal);
			m_nEffID	= pParticleCenter->RunParticle(szTmp,SCREEN_SIZE_W*POINT_PIX_CNT/2,SCREEN_SIZE_H,0);
		}
		break;
	case 11:	//洒花
		{
			char	szTmp[64];
			sprintf(szTmp,"other/weather11.plist",bVal);
			m_nEffID	= pParticleCenter->RunParticle(szTmp,SCREEN_SIZE_W*POINT_PIX_CNT/2,0,0);
			sprintf(szTmp,"other/weather12.plist",bVal);
			pParticleCenter->RunParticle(szTmp,SCREEN_SIZE_W*POINT_PIX_CNT/2,0,0);
		}
		break;
	default:	//下雨
		{
			char	szTmp[64];
			sprintf(szTmp,"other/weather%d.plist",bVal);
			m_nEffID	= pParticleCenter->RunParticle(szTmp,SCREEN_SIZE_W*POINT_PIX_CNT/2,0,0);
			if(bVal == 1)
				m_nWeatherSound	= g_pSoundCenter->PlayerEffect("Sound/rain",true);
		}
		break;
	}
}

void MapControl::PlayFuryEff( int nX, int nY )
{
	if(pChar->m_pSkillCenter->m_ppActSkill[4])
		pParticleCenter->RunParticle("other/fire.plist",nX-m_nX,nY+SCREEN_SIZE_H*POINT_PIX_CNT-m_nY,2000,1);
}

DWORD MapControl::PlayerParticle( int nX, int nY,DWORD	dwRunTick, BYTE bType )
{
	return pParticleCenter->RunParticle("other/fire.plist",nX,nY,dwRunTick,bType);
}

void MapControl::SetParticle( int nID,int nX, int nY )
{
	pParticleCenter->SetParticlePos(nID,nX-m_nX,nY+SCREEN_SIZE_H*POINT_PIX_CNT-m_nY);
}

bool MapControl::CheckLineBlock( int nX, int nY, int nEX, int nEY, bool bAtkTarget/*=false*/ )
{
	if(bAtkTarget)
	{
		if(CalcDistance(nX,nY,nEX,nEY) < 200)
		{
			//if(m_pMapBarrier->GetBlock(nX,nY))
			//	return false;
			if(m_pMapBarrier->GetBlock(nEX,nEY))
				return false;
		}
	}

	return m_pMapBarrier->CheckLineBlock(nX,nY,nEX,nEY);
}
