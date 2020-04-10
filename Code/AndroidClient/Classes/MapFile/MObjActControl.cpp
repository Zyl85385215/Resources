#include "MapControl.h"
#include "MObjActControl.h"
#include "MapAIObj.h"
#include "SoundCenter.h"
#include "MainMenu.h"
#include "ParticleCenter.h"
#include "EnemyConfig.h"
#include "MapStaticCfg.h"

int		s_sznActionFrm[]	= {3,6,3,3,3,6,6,1,1,1};
int		s_sznActionTick[]	= {600,500,400,400,400,600,500,600,600,600};
//int		s_sznActionFrm[]	= {5,6,5,5,14,9,19,2,4,7,5,5};
//int		s_sznActionTick[]	= {600,500,400,400,400,600,500,600,600,600,600,600,600,600,600,600};

MObjActCtrl::MObjActCtrl()
{
	m_pListAllMObj	= xnList::Create();
	m_pListCache	= xnList::Create();
}

MObjActCtrl::~MObjActCtrl()
{
	while(m_pListAllMObj->Count)
		delete (MapSprObj*)m_pListAllMObj->Delete(0);
	m_pListAllMObj->Free();

	while(m_pListCache->Count)
		delete (MapSprObj*)m_pListCache->Delete(0);
	m_pListCache->Free();
}

bool MObjActCtrl::LinkCtrl( MapAIObj* pMObj )
{
	MapSprObj*	pMSpr	= new MapSprObj();
	pMSpr->pMObj		= pMObj;
	pMSpr->byAction		= pMObj->GetAction();
	pMSpr->byDirect		= pMObj->m_byDir;
	pMSpr->nMaxHp		= pMObj->nMaxHP;
	pMSpr->nHp			= pMObj->nHP;
	pMSpr->nPosX		= pMObj->nPosX;
	pMSpr->nPosY		= pMObj->nPosY;
	pMSpr->byType		= pMObj->m_byType;
	pMSpr->szName		= pMObj->m_szName;
	pMSpr->szTitle		= pMObj->m_szTitle;
	pMSpr->szMarryTitle	= pMObj->m_szMarryTitle;
	pMSpr->pCountry		= &pMObj->byCountry;
	pMSpr->pVipLevel	= &pMObj->byViplv;
	pMSpr->pListEffAdd	= pMObj->m_pLEffAdd;
	pMSpr->pListEffDel	= pMObj->m_pLEffDel;
	pMSpr->pListFightNum= pMObj->m_pLFightNum;
	pMSpr->pTalkInfo	= pMObj->pTalkObj;

	//pMSpr->m_dwParticleID	= 0;
	pMSpr->InitSkin(pMObj->m_szwSkin[MSKIN_BODY]);
	m_pListAllMObj->Add(pMSpr);
	//if(pMObj->nDestX == 0 && pMObj->nDestY == 0)
	//	pMSpr->byDirect		= 4;
	//else
	//	pMSpr->byDirect		= CalcDirection(pMObj->nPosX,pMObj->nPosY,pMObj->nDestX,pMObj->nDestY,2);
	return true;
}

bool MObjActCtrl::DisLinkCtrl( MapObj* pMObj )
{
	for (int i = 0; i < m_pListAllMObj->Count; i++)
	{
		MapSprObj* pMSpr	= (MapSprObj*)m_pListAllMObj->Items[i];
		if(pMSpr->pMObj == pMObj)
		{
			pMSpr->pMObj	= NULL;
			if(g_pMapCenter->m_pMObjSel == pMSpr)
				g_pMapCenter->m_pMObjSel	= NULL;
			pMSpr->dwFrmTick	= xnGetTickCount();
			//pParticleCenter->StopParticle(pMSpr->m_dwParticleID);

			m_pListAllMObj->Delete(i);
			m_pListCache->Add(pMSpr);
			return true;
		}
	}

	return false;
}

void MObjActCtrl::Update()
{
	MapAIObj* pMapObj	= pChar->GetMyMapObj();
	if(!pMapObj)
		return;

	BYTE byMyAct	= pMapObj->GetAction();
	if ((byMyAct!=OBJ_ACTION_STAND&&byMyAct!=OBJ_ACTION_ATK2)&&pMapObj->pStateCenter->CheckState(8111)) // 动作取消钓鱼状态
	{
		BuildPacketEx(pPacket,USER,C_FISH_RLT,buf,256);
		SET_DATA(pData,USER,C_FISH_RLT,pPacket);
		pData->byFish = 5;
		pData->dwUin  = 0;
		pChar->SendPacket(pPacket);

		pMainMenu->m_pFishMenu->Close();
	}

	DWORD	dwTickNow	= xnGetTickCount();
	for (int i = 0; i < m_pListAllMObj->Count; i++)
	{
		MapSprObj*	pMSpr	= (MapSprObj*)m_pListAllMObj->Items[i];
		MapAIObj*	pMObj	= (MapAIObj*)pMSpr->pMObj;

		if(pMObj->pTalkObj != pMSpr->pTalkInfo)
			pMSpr->pTalkInfo	= pMObj->pTalkObj;

		BYTE byAct	= pMObj->GetAction();
		if(pMObj->m_dwIsRush)
		{
			switch(pMObj->m_byRushAct)
			{
			case 1:	//冲锋
				byAct	= OBJ_ACTION_RUN;
				if(xnGetTickCount()/50%2 == 0)
					pMSpr->byUpdateFlag	= 2;
				break;
			case 2:	//闪烁
				byAct	= OBJ_ACTION_STAND;
				break;
			case 3:	//被拉过去
				byAct	= OBJ_ACTION_HURT;
				break;
			case 4:	//击退
				byAct	= OBJ_ACTION_HURT;
				break;
			case 5:
				byAct	= OBJ_ACTION_STAND;
				break;
			}
		}

		if(pMSpr->byAction != byAct)
		{
			pMSpr->byFrame		= 0;
			pMSpr->byAction		= byAct;
			if(pMSpr->byUpdateFlag == 0)
				pMSpr->byUpdateFlag	= 1;
		}
		pMSpr->byDirect		= pMObj->m_byDir;
		pMSpr->InitSkin(pMObj->m_szwSkin[MSKIN_BODY]);
		pMSpr->SetSkinPA(pMObj->m_szwSkin[MSKIN_WEAPON]);
		pMSpr->SetSkinPB(pMObj->m_szwSkin[MSKIN_HORSE]);

		if(dwTickNow - pMSpr->dwFrmTick > s_sznActionTick[pMSpr->byAction]/s_sznActionFrm[pMSpr->byAction])
		{//间隔一段时间加帧
			if(pMSpr->dwFrmTick	!= 0)
				pMSpr->byFrame	++;
			if(pMSpr->byFrame >= s_sznActionFrm[pMSpr->byAction])
			{
				pMSpr->byFrame	= 0;
				if(pMObj->m_byState	== OBJ_STATE_ATK || pMObj->m_byState == OBJ_STATE_SKILL)
				{//一次攻击结束
					pMObj->m_byState	= OBJ_STATE_STAND;
					pMSpr->byAction		= pMObj->GetAction();
				}
					
			}

			if(pMSpr->pSkin && pMSpr->pSkin->nId < 2000)
			{
				if(pMSpr->pSkinPB)
					_checkPlaySound(MAPOBJ_PLAYER,((MapPlayer*)pMObj)->bySex,((MapPlayer*)pMObj)->byJob,byAct,pMSpr->byFrame,pMSpr->pSkinPB->szSoundEx);
				else
					_checkPlaySound(MAPOBJ_PLAYER,((MapPlayer*)pMObj)->bySex,((MapPlayer*)pMObj)->byJob,byAct,pMSpr->byFrame);
			}
			else
				_checkPlaySound(MAPOBJ_ENEMY,0,0,byAct,pMSpr->byFrame);

			pMSpr->dwFrmTick	= dwTickNow;
			if(pMSpr->byUpdateFlag == 0)
				pMSpr->byUpdateFlag	= 1;
		}
		pMSpr->nPosX		= pMObj->nPosX;
		pMSpr->nPosY		= pMObj->nPosY;
		pMSpr->nMaxHp		= pMObj->nMaxHP;
		pMSpr->nHp			= pMObj->nHP;
		if((pMObj->bForce && pMObj->bForce != pMapObj->bForce))
			pMSpr->byIsEnemy	= 1;
		else
			pMSpr->byIsEnemy	= 0;

		//if(xnGetTickCount() - pChar->m_dwTickJoin > 5000)
		//{
		//	if(pMObj == pChar->GetMyMapObj())
		//	{
		//		if(pMSpr->m_dwParticleID == 0)
		//		{
		//			pMSpr->m_dwParticleID	= g_pMapCenter->PlayerParticle(pMSpr->nPosX,pMSpr->nPosY-50,5);
		//		}
		//		else
		//		{
		//			g_pMapCenter->SetParticle(pMSpr->m_dwParticleID,pMSpr->nPosX,pMSpr->nPosY-50);
		//		}
		//	}
		//}
		
	}

	for (int i = 0; i < m_pListCache->Count; i++)
	{
		MapSprObj*	pMSpr	= (MapSprObj*)m_pListCache->Items[i];
		if(dwTickNow -pMSpr->dwFrmTick>3000)
		{
			delete pMSpr;
			m_pListCache->Delete(i);
			i--;
		}
	}
}

void MObjActCtrl::GetInViewObj( int nPosX, int nPosY, int nEX, int nEY ,xnList* pLOut )
{
	pLOut->Clear();

	for (int i = 0; i < m_pListAllMObj->Count; i++)
	{
		MapSprObj* pMSpr	= (MapSprObj*)m_pListAllMObj->Items[i];

		if(((MapAIObj*)pMSpr->pMObj)->objAIModel.byAIType >= MCMD_LOCKVIEW_ENEMY)
			continue;

		if(pMSpr->nPosX>= nPosX && pMSpr->nPosX <= nEX 
			&& pMSpr->nPosY>= nPosY && pMSpr->nPosY <= nEY)
		{
			if(pMSpr->byType == MAPOBJ_PLAYER || pMSpr->byType == MAPOBJ_SLAVE)
			{
				if(pMainMenu->m_pSettingMenu->m_bHideUser)
				{
					if(pMSpr->pMObj != g_pMapCenter->m_pMObjLockView)
					{
						if((pMSpr->pMObj && ((MapAIObj*)pMSpr->pMObj)->dwMastUin != g_pMapCenter->m_pMObjLockView->m_nID))
							continue;
					}
				}
				if(g_pMapCenter->m_dwFrmBusyTime >= 20)
				{
					if(pLOut->Count > 20 && ((MapAIObj*)pMSpr->pMObj)->byLv < 40)
						continue;
				}
			}

			if(pMSpr->pMObj && ((MapAIObj*)pMSpr->pMObj)->pStateCenter->m_bUnVisable && pMSpr->pMObj != pChar->GetMyMapObj() && pMainMenu->m_pTeamMenu->IsTeamMember(((MapAIObj*)pMSpr->pMObj)->m_nID)==false)
				continue;
			pLOut->Add(pMSpr);
		}
	}
}

MapSprObj* MObjActCtrl::CheckSelObj( int nX, int nY )
{
	int nZorde	= -999;
	MapSprObj*	pRtl	= NULL;
	for (int n = 0; n < m_pListAllMObj->Count; n++)
	{
		MapSprObj* pMSpr	= (MapSprObj*)m_pListAllMObj->Items[n];
		if(pMSpr->nPosY <= nZorde)
			continue;
		if(pMSpr->nHp == 0 && pMSpr->byType != MAPOBJ_PLAYER)
			continue;	//死亡怪不可选中
		if(pMSpr->byType == MAPOBJ_PLAYER || pMSpr->byType == MAPOBJ_SLAVE)
		{
			if(pMainMenu->m_pSettingMenu->m_bHideUser)
				continue;
		}

		int nDifX	= nX - pMSpr->nPosX;
		int nDifY	= pMSpr->nPosY - nY;
		for (int i = 0; i < pMSpr->pSkin->bRectCnt; i++)
		{

			if(nDifX	>= pMSpr->pSkin->pnVal[4*i] && nDifX <= pMSpr->pSkin->pnVal[4*i+2]
					&& nDifY	>= pMSpr->pSkin->pnVal[4*i+1] && nDifY <= pMSpr->pSkin->pnVal[4*i+3])
			{
				if (pMSpr->pMObj != g_pMapCenter->m_pMObjLockView)
				{
					nZorde	= pMSpr->nPosY;
					pRtl	= pMSpr;
				}
			}
		}
	}
	return pRtl;
}

void MObjActCtrl::_checkPlaySound( BYTE bType, BYTE bSex,BYTE bJob, BYTE bAct, BYTE bFrm,char*	szSkinSound )
{
	if(bType == MAPOBJ_PLAYER)
	{
		if(bAct == OBJ_ACTION_HURT && bFrm == 1)
		{
			if(rand()%10 == 0)
			{
				if(bSex == 1)
					g_pSoundCenter->PlayerEffect("Sound/male/behurt");
				else
					g_pSoundCenter->PlayerEffect("Sound/female/behurt");
			}
		}
		//if(bAct == OBJ_ACTION_RUN && (bFrm == 1))
		//{
		//	if(bSex == 1)
		//		g_pSoundCenter->PlayerEffect("Sound/male/run");
		//	else
		//		g_pSoundCenter->PlayerEffect("Sound/female/run");
		//}

		if(bAct == OBJ_ACTION_HRUN && (bFrm == 1))
		{
			if(szSkinSound == NULL || strlen(szSkinSound) == 0)
			{
				g_pSoundCenter->PlayerEffect("Sound/horse_run");
			}
			else
			{
				char	szTmp[64];
				sprintf(szTmp,"Sound/%s",szSkinSound);
				g_pSoundCenter->PlayerEffect(szTmp);
			}
		}

		if((/*bAct == OBJ_ACTION_ATK1 || */bAct == OBJ_ACTION_ATK2) && bFrm == 1)
		{
			if(bSex == 1)
				g_pSoundCenter->PlayerEffect("Sound/male/atk");
			else
				g_pSoundCenter->PlayerEffect("Sound/female/atk");
		}
		if((bAct == OBJ_ACTION_ATK1/* || bAct == OBJ_ACTION_ATK2*/) && bFrm == 1)
		{
			if(bJob == 1)
				g_pSoundCenter->PlayerEffect("Sound/skill_zs");
			else if(bJob == 2)
				g_pSoundCenter->PlayerEffect("Sound/skill_fs");
			else if(bJob == 3)
				g_pSoundCenter->PlayerEffect("Sound/skill_hq");
			else
				g_pSoundCenter->PlayerEffect("Sound/skill_ms");
		}
	}
	else
	{
		//if(bAct == OBJ_ACTION_HURT && bFrm == 1)
			//g_pSoundCenter->PlayerEffect("Sound/1/behurt");
		//if((bAct == OBJ_ACTION_ATK1 || bAct == OBJ_ACTION_ATK2) && bFrm == 1)
			//g_pSoundCenter->PlayerEffect("Sound/1/atk");
		//if(bAct == OBJ_ACTION_RUN && (bFrm == 1))
		//	g_pSoundCenter->PlayerEffect("Sound/1/run");
	}
}

MapSprObj* MObjActCtrl::SearchObj(int nX, int nY, BYTE bEnemy,DWORD	nMinDistance, DWORD dwQusEnemyID)
{
	MapSprObj*	pRtl	= NULL;
	for (int n = 0; n < m_pListAllMObj->Count; n++)
	{
		MapSprObj* pMSpr	= (MapSprObj*)m_pListAllMObj->Items[n];
		//if(pMSpr->nPosX == nX && pMSpr->nPosY == nY)
		//	continue;		//对象应该是自己
		if(pMSpr->pMObj == pChar->GetMyMapObj())
			continue;

		if (dwQusEnemyID&&dwQusEnemyID != ((MapAIObj*)pMSpr->pMObj)->enemyID)
			continue;

		if (pMainMenu->m_pAutoMenu->m_bActive == true&&(((MapAIObj*)pMSpr->pMObj)->enemyID == 1500||((MapAIObj*)pMSpr->pMObj)->enemyID == 1501||((MapAIObj*)pMSpr->pMObj)->enemyID == 1502))
			continue;

		if(bEnemy != 2 && pMSpr->byIsEnemy != bEnemy)
			continue;

		bool	bLockPlayer = false;;
		if(pMSpr->byType == MAPOBJ_PLAYER || pMSpr->byType == MAPOBJ_SLAVE)
		{//开了选择敌对玩家的情况下可以选择敌对玩家
			if(pMainMenu->m_pAutoMenu->m_bLockPlayer)
			{
				if(!pMSpr->byIsEnemy)
					continue;
				else
					bLockPlayer	= true;
			}
			else
				continue;
		}

		if(pMSpr->byType == MAPOBJ_ENEMY && pMSpr->byIsEnemy == 0)
			continue;

		if(pMSpr->nHp == 0)
			continue;

		if(g_pMapCenter->m_pMapBarrier->CheckLineBlock(nX,nY,pMSpr->nPosX,pMSpr->nPosY))
			continue;
		if(bLockPlayer==false && pRtl && pRtl->byType == MAPOBJ_PLAYER)
		{//对象不是敌对玩家的并且当前选择的是敌对玩家
			continue;
		}

		int nDis	= CalcDistance2(nX,nY, pMSpr->nPosX,pMSpr->nPosY);
		if(pRtl == NULL)
		{
			pRtl	= pMSpr;
			nMinDistance	= nDis;
		}
		else if(bLockPlayer && (pRtl->byType != MAPOBJ_PLAYER))
		{//该对象时玩家并且之前对象不是
			pRtl	= pMSpr;
			nMinDistance	= nDis;
		}
		else if(nDis < nMinDistance)
		{
			pRtl	= pMSpr;
			nMinDistance	= nDis;
		}
		if(((MapAIObj*)pMSpr->pMObj)->pPathObj && ((MapAIObj*)pMSpr->pMObj)->pPathObj->byMark == 3 && nDis < nMinDistance*1000)
		{
			pRtl	= pMSpr;
			nMinDistance	= nDis/1000;
		}
		if(((MapAIObj*)pMSpr->pMObj)->pPreAtk == pChar->GetMyMapObj() && nDis < nMinDistance*9)
		{//之前攻击过我的优先
			pRtl	= pMSpr;
			nMinDistance	= nDis/9;
		}
	}
	return pRtl;
}

void MObjActCtrl::ResetObj()
{
	//for (int n = 0; n < m_pListAllMObj->Count; n++)
	//{
	//	MapSprObj* pMSpr	= (MapSprObj*)m_pListAllMObj->Items[n];

	//	pMSpr->pSprView		= NULL;
	//}
}

MapSprObj* MObjActCtrl::GetObjByID( BYTE byType, int nID )
{
	for (int n = 0; n < m_pListAllMObj->Count; n++)
	{
		MapSprObj* pMSpr	= (MapSprObj*)m_pListAllMObj->Items[n];

		if(pMSpr->byType == byType)
		{
			if(((MapObj*)pMSpr->pMObj)->m_nID == nID)
				return pMSpr;
		}
	}
	return NULL;
}
