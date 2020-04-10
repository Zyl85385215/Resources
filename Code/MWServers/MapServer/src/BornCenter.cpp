#include "stdafx.h"
#include "xnExtstr.h"
#include "BornCenter.h"
#include "MapAIObj.h"
#include "MapManager.h"
#include "EnemyConfig.h"
#include "SkillCenter.h"
extern int CalcDistance(int nX1,int nY1,int nX2,int nY2);
extern void GetBattleGPos( int& nX, int& nY,int nDestX, int nDestY, int nDir , int nPos);
extern int	CalcDirection(int nX1,int nY1,int nX2,int nY2);
//---------------------------------------------



BornPointInfo::BornPointInfo(WORD wMapW,WORD wMapH)
{
	pListEnemys		= xnList::Create();
	pListDeadTick	= xnList::Create();
	wMaxHeight		= wMapH;
	wMaxWidth		= wMapW;
	szRunPts		= NULL;
}

BornPointInfo::~BornPointInfo()
{
	if(szRunPts)
		delete[] szRunPts;

	while(pListEnemys->Count)
		delete (MapAIObj*)pListEnemys->Delete(0);
	pListEnemys->Free();
	pListDeadTick->Free();
}

void BornPointInfo::GetRandPoint( int& nRtlX, int& nRtlY,MapManager* pParent,BYTE& byStep )
{
	int nLoopCnt =0;
	int		nX		= wPosX;
	int		nY		= wPosY;
	while(1)
	{
		nLoopCnt++;
		if(nLoopCnt > 10)
		{
			nX		= wPosX;
			nY		= wPosY;
			break;
		}


		if(bPtRunCnt)
		{
			int		nStepPos	= -1;
			int		nMinDis	= 100000;
			for (int i = 0; i < bPtRunCnt; i++)
			{//
				int	nRealStep	= (i+byStep)%bPtRunCnt;
				if(pParent->CheckLineBlock(nRtlX,nRtlY,szRunPts[nRealStep].x,szRunPts[nRealStep].y) == false)
				{
					int	nTDis	= CalcDistance(nRtlX,nRtlY,szRunPts[nRealStep].x,szRunPts[nRealStep].y);
					if(nTDis < 100)
					{
						if(nRealStep != bPtRunCnt-1)
						{
							nStepPos	= nRealStep+1;
							break;
						}
						else
						{
							nStepPos	= 0;
							break;
						}
					}
					else if(nTDis < nMinDis)
					{
						nTDis		= nMinDis;
						nStepPos	= nRealStep;
					}
				}
			}
			if(nStepPos != -1)
			{
				byStep	= nStepPos;
				nX	= szRunPts[nStepPos].x;
				nY	= szRunPts[nStepPos].y;
				break;
			}
			else
			{
				nX	= wPosX;
				nY	= wPosY;
				byStep	= 0;
				break;
			}
		}
		else
		{
			int		nDisLen	= 1+rand()%wRange;
			int		nXLen	= rand()%nDisLen;
			int		nYLen	= nDisLen - nXLen;
			nX		= ((int)wPosX)	+ (rand()%2==0?-nXLen:nXLen);
			nY		= ((int)wPosY)	+ (rand()%2==0?-nYLen:nYLen);
		}

		if(nX	<= 0 || nY <= 0)
		{//重新随机
			continue;
		}
		else if(nX >= wMaxWidth || nY >= wMaxHeight)
			continue;
		else if(pParent->GetBlock(nX,nY))
		{//障碍,重新随机
			continue;
		}
		else if(nRtlX != 0 && nRtlY != 0)
		{//需要跟当前点直线无障碍
			if(pParent->CheckLineBlock(nRtlX,nRtlY,nX,nY))
				continue;
		}
		break;
	}
	
	//重新随机一定次数后不行就直接出生点刷
	if(nX	< 0 )
		nX	= 0;
	if(nY < 0)
		nY	= 0;
	if(nX >= wMaxWidth)
		nX	= wMaxWidth;
	if(nY >= wMaxHeight)
		nY	= wMaxHeight;


	nRtlX	= nX;
	nRtlY	= nY;
}

void BornPointInfo::CheckBGroupCmd( MapAIObj* pObj,MapManager* pMap )
{
	if(pObj->bCmd != MCMD_NULL)
		return;
	if(pListEnemys->Count == 0)
		return;
	if(pObj == pListEnemys->Items[0])
	{//第一个怪物发布队列指令
		//检查编队状态, 在战斗的排除掉
		int		nDestX	= pObj->nX;
		int		nDestY	= pObj->nY;
		BYTE	bStep	= pObj->m_byAutoAtk;
		GetRandPoint(nDestX,nDestY,pMap,bStep);
		//if(bStep == pObj->m_byAutoAtk)
		//{//还未抵达当前目标点异常停下的
			//pObj->SetCmd(MCMD_RUN,(nDestY<<16)+nDestX);
		//}
		int	nRotate	= CalcDirection(pObj->nX,pObj->nY,nDestX,nDestY);
		bool	bGroupOk	= true;
		for (int i = 1; i < pListEnemys->Count; i++)
		{
			MapAIObj*	pAI		= (MapAIObj*)pListEnemys->Items[i];
			if(pAI->bCmd == MCMD_NULL || pAI->bCmd	== MCMD_RUN)
			{
				int	nAIX	= 0;
				int	nAIY	= 0;
				GetBattleGPos(nAIX,nAIY,pObj->nX,pObj->nY,nRotate,i);

				int	nDisLen	= CalcDistance(nAIX,nAIY, pAI->nX, pAI->nY);

				if(nDisLen > 1000)
				{//飞过去
					pAI->JumpInMap(nAIX,nAIY);
				}
				else if(nDisLen > 30)
				{//未就位
					bGroupOk	= false;
					pAI->SetCmd(MCMD_RUN,(nAIY<<16)+nAIX);
				}
			}
		}

		if(bGroupOk)
		{
			pObj->SetCmd(MCMD_RUN,(nDestY<<16)+nDestX);
			pObj->m_byAutoAtk	= bStep;

			for (int i = 1; i < pListEnemys->Count; i++)
			{
				MapAIObj*	pAI		= (MapAIObj*)pListEnemys->Items[i];
				if(pAI->bCmd == MCMD_NULL || pAI->bCmd	== MCMD_RUN)
				{
					int	nAIX	= 0;
					int	nAIY	= 0;
					GetBattleGPos(nAIX,nAIY,nDestX,nDestY,nRotate,i);
					pAI->SetCmd(MCMD_RUN,(nAIY<<16)+nAIX);
					pAI->m_byAutoAtk	= bStep;
				}
			}
		}
	}

}

//---------------------------------------------
BornCenter::BornCenter(WORD	wRMIndex,MapManager* pParent)
{
	srand(GetTickCount());
	m_pParent	= pParent;

	m_pListBornPts	= xnList::Create();
	m_pListRobot		= xnList::Create();
	m_pListGSPlayer	= xnList::Create();
	m_pListGSSlave	= xnList::Create();
	m_pListGSEnemy	= xnList::Create();
	m_pListCache	= xnList::Create();
	m_dwUpdateCnt	= 0;
	_initNpc();
	_initBorn(wRMIndex);
}

BornCenter::~BornCenter()
{
	while(m_pListBornPts->Count)
		delete (BornPointInfo*)m_pListBornPts->Delete(0);
	m_pListBornPts->Free();

	while(m_pListRobot->Count)
		delete (MapAIObj*)m_pListRobot->Delete(0);
	m_pListRobot->Free();
	while(m_pListGSPlayer->Count)
		delete (MapAIObj*)m_pListGSPlayer->Delete(0);
	m_pListGSPlayer->Free();
	while(m_pListGSSlave->Count)
		delete (MapAIObj*)m_pListGSSlave->Delete(0);
	m_pListGSSlave->Free();
	while(m_pListGSEnemy->Count)
		delete (MapAIObj*)m_pListGSEnemy->Delete(0);
	m_pListGSEnemy->Free();
	while(m_pListCache->Count)
		delete (MapAIObj*)m_pListCache->Delete(0);
	m_pListCache->Free();
}

void BornCenter::_initNpc()
{//初始化NPC并刷出NPC

}

void BornCenter::UpdateSec()
{
	m_dwUpdateCnt++;
	_updateBorn();
	CheckMObjList();

	DWORD	dwTickNow	= GetTickCount();
	for (int i = m_pListCache->Count-1; i >= 0; i--)
	{
		MapAIObj* pObj	= (MapAIObj*)m_pListCache->Items[i];
		if(dwTickNow - pObj->dwTickCmd > 3000)
		{
			delete pObj;
			m_pListCache->Delete(i);
		}
	}
}

void BornCenter::_initBorn( WORD wRMIndex )
{
	char	szTmp[256];
	sprintf(szTmp,"Map//%d//enemyborn.txt",wRMIndex);
	xnExtstr* pStr	= xnExtstr::Import(szTmp);
	if(!pStr)
		return;

	int nIndex	= 0;
	while(pStr->SetSection(nIndex++))
	{
		BornPointInfo* pBPInfo	= new BornPointInfo(m_pParent->m_pMCfgObj->m_wMapSizeW,m_pParent->m_pMCfgObj->m_wMapSizeH);
		m_pListBornPts->Add(pBPInfo);
		pBPInfo->wEBaseID		= atoi(pStr->ReadString("baseid","0"));
		pBPInfo->wPosX			= atoi(pStr->ReadString("x","2000"));
		pBPInfo->wPosY			= atoi(pStr->ReadString("y","2000"));
		pBPInfo->wActiveNumMin	= atoi(pStr->ReadString("nummin","3"));
		pBPInfo->wActiveNumMax	= atoi(pStr->ReadString("nummax","5"));
		pBPInfo->wActiveNum		= pBPInfo->wActiveNumMin;
		pBPInfo->dwBornTickMin	= atoi(pStr->ReadString("borntickmin","5000"));
		pBPInfo->dwBornTickMax	= atoi(pStr->ReadString("borntickmax","2000"));
		pBPInfo->dwBornTick		= pBPInfo->dwBornTickMin;
		pBPInfo->bMaxPlayer		= atoi(pStr->ReadString("byactplayer","10"));
		pBPInfo->wRange			= atoi(pStr->ReadString("bornrange","1000"));
		pBPInfo->bForce			= atoi(pStr->ReadString("forceid","11"));
		pBPInfo->wExPow			= atoi(pStr->ReadString("expow","100"));
		if(pBPInfo->wExPow == 0)
			pBPInfo->wExPow	= 100;
		
		pBPInfo->byBattleGroup	= atoi(pStr->ReadString("battlegroup","0"));
		pBPInfo->wRangeRun		= atoi(pStr->ReadString("moverange","2000"));
		pBPInfo->wRangeAtk		= atoi(pStr->ReadString("maxrange","4000"));

		pBPInfo->bPtRunCnt		= atoi(pStr->ReadString("ptcnt","0"));
		pBPInfo->bLoopRun		= atoi(pStr->ReadString("isloop","0"));
		
		if(pBPInfo->bPtRunCnt)
		{
			pBPInfo->szRunPts		= new POINT[pBPInfo->bPtRunCnt];
			for (int i = 0; i < pBPInfo->bPtRunCnt; i++)
			{
				sprintf(szTmp,"ptx%d",i+1);
				pBPInfo->szRunPts[i].x	= atoi(pStr->ReadString(szTmp,"2000"));
				sprintf(szTmp,"pty%d",i+1);
				pBPInfo->szRunPts[i].y	= atoi(pStr->ReadString(szTmp,"2000"));
			}
		}
	}
	
}

void BornCenter::_updateBorn()
{//检测各刷新点是否需要刷怪
	DWORD	dwCurTick	= GetTickCount();
	for (int i = 0; i < m_pListBornPts->Count; i++)
	{
		BornPointInfo* pBPInfo	= (BornPointInfo*)m_pListBornPts->Items[i];

		if(m_dwUpdateCnt%5==0)
		{
			xnList*	pListObj	= xnList::Create();
			m_pParent->GetObjRange(pBPInfo->wPosX,pBPInfo->wPosY,pBPInfo->wRange+300,pListObj);
			int	nPlayerCnt	= 0;
			for (int j = 0; j < pListObj->Count; j++)
			{
				MapAIObj* pObj	= (MapAIObj*)pListObj->Items[j];
				if(pObj->byType == MAPOBJ_PLAYER || pObj->byType == MAPOBJ_ROBOT)
					nPlayerCnt++;
			}
			
			if(nPlayerCnt > pBPInfo->bMaxPlayer)
				nPlayerCnt	= pBPInfo->bMaxPlayer;
			pBPInfo->wActiveNum	= pBPInfo->wActiveNumMin + (pBPInfo->wActiveNumMax-pBPInfo->wActiveNumMin)*nPlayerCnt/pBPInfo->bMaxPlayer;

			pBPInfo->dwBornTick	= pBPInfo->dwBornTickMin + ((int)(pBPInfo->dwBornTickMax-pBPInfo->dwBornTickMin))/pBPInfo->bMaxPlayer*nPlayerCnt;
			pListObj->Free();
		}
		

		if(pBPInfo->pListEnemys->Count < pBPInfo->wActiveNum)
		{//数量
			DWORD	dwTickLimit	= 0;
			if(pBPInfo->pListDeadTick->Count)
			{
				if(pBPInfo->dwBornTick == 0)
					continue;	//设置为不刷新的不会因为怪物死亡而补刷
				dwTickLimit	= (DWORD)pBPInfo->pListDeadTick->Items[0];
			}
			if(dwCurTick - dwTickLimit > pBPInfo->dwBornTick)
			{//时间符合
				if(dwTickLimit)
					pBPInfo->pListDeadTick->Delete(0);
				EnemyBase* pBase	= EnemyBaseCfg::GetInstance().GetEnemyBase(pBPInfo->wEBaseID);
				if(pBase == NULL)
					continue;

				MapAIObj* pObj	= new MapAIObj();
				pObj->byType	= MAPOBJ_ENEMY;
				pObj->dwUin		= m_pParent->LockEnemyID();
				pObj->pBornInfo	= pBPInfo;
				if(m_pParent->m_byChgLv)
					pObj->bLv		= m_pParent->m_byChgLv;
				else
					pObj->bLv		= pBase->bLvl;
				pObj->nX		= pBPInfo->wPosX;
				pObj->nY		= pBPInfo->wPosY;
				pObj->m_byAutoAtk	= 0;
				pBPInfo->GetRandPoint(pObj->nX,pObj->nY,m_pParent,pObj->m_byAutoAtk);
				pObj->pBase		= pBase;
				pObj->wSkin[0]	= pBase->nID;

				int	nExPow	= pBPInfo->wExPow*m_pParent->m_wExPow/100;

				pObj->xParam.nHp	= pBase->nMaxHp*nExPow/100;
				pObj->xParam.nHpMax	= pBase->nMaxHp*nExPow/100;
				pObj->xParam.nMp	= 100;
				pObj->xParam.nMpMax	= 100;
				pObj->xParam.nAtkW	= pBase->nAtkW*nExPow/100;
				pObj->xParam.nAtkN	= pBase->nAtkN*nExPow/100;
				pObj->xParam.nDefW	= pBase->nDefW*nExPow/100;
				pObj->xParam.nDefN	= pBase->nDefN*nExPow/100;
				pObj->xParam.nAim	= pBase->nAim*nExPow/100;
				pObj->xParam.nCrit	= pBase->nCrit*nExPow/100;
				pObj->xParam.nDodge	= pBase->nDodge*nExPow/100;
				pObj->xParam.nMSpeed= pBase->nSpeed;
				pObj->xParam.nPvpDef	= 0;
				pBPInfo->pListEnemys->Add(pObj);
				pObj->bForce		= pBPInfo->bForce;

				for (int k = 0; k < pBase->pLSkill->Count; k++)
				{
					pObj->pSkillCenter->SetSkill((int)pBase->pLSkill->Items[k],1,0,0);
				}

				m_pParent->AIObjEnterMap(pObj,true);
			}
		}
		else
		{
			pBPInfo->pListDeadTick->Clear();
		}
	}
}

MapAIObj* BornCenter::GetMapObj( BYTE bType, DWORD dwID )
{
	MapAIObj* pObj	= NULL;
	switch(bType)
	{
	case MAPOBJ_ENEMY:
		pObj	= _findMObj(dwID,m_pListGSEnemy);
		if(pObj == NULL)
		{
			for (int i = 0; i < m_pListBornPts->Count; i++)
			{
				BornPointInfo* pBornPt	= (BornPointInfo*)m_pListBornPts->Items[i];
				pObj	= _findMObj(dwID,pBornPt->pListEnemys);
				if(pObj)
					break;
			}
		}
		break;
	//case MAPOBJ_NPC:
	//	pObj	= _findMObj(dwID,m_pListNpc);
	//	break;
	case MAPOBJ_SLAVE:
		pObj	= _findMObj(dwID,m_pListGSSlave);
		break;
	case MAPOBJ_PLAYER:
		pObj	= _findMObj(dwID,m_pListGSPlayer);
		break;
	case MAPOBJ_ROBOT:
		pObj	= _findMObj(dwID,m_pListRobot);
		break;
	}
	return pObj;
}

MapAIObj* BornCenter::_findMObj( DWORD dwID, xnList* pList )
{
	for (int i = 0; i < pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)pList->Items[i];
		if(pObj->dwUin		== dwID)
			return pObj;
	}
	return NULL;
}

bool BornCenter::GSBornMapObj( MapAIObj* pObj )
{
	xnList* pList	= NULL;
	switch (pObj->byType)
	{
	case MAPOBJ_PLAYER:
		pList	= m_pListGSPlayer;
		break;
	case MAPOBJ_SLAVE:
		pList	= m_pListGSSlave;
		break;
	case MAPOBJ_ENEMY:
		pList	= m_pListGSEnemy;
		break;
	case MAPOBJ_ROBOT:
		pList	= m_pListRobot;
		break;
	}

	if(pList->IndexOf(pObj) == -1)
		pList->Add(pObj);
	else
	{
		pObj->bDisappear	= false;
		return false;
	}
	return true;
}

void BornCenter::CheckMObjList()
{
	_checkMObj(m_pListGSEnemy);
	
	for (int i = 0; i < m_pListBornPts->Count; i++)
	{
		BornPointInfo* pBornPt	= (BornPointInfo*)m_pListBornPts->Items[i];
		_checkMObj(pBornPt->pListEnemys);
	}
	
	_checkMObj(m_pListRobot);
	_checkMObj(m_pListGSSlave);
	_checkMObj(m_pListGSPlayer);
}

void BornCenter::_checkMObj( xnList* pList )
{
	DWORD	dwCutTick	= GetTickCount();
	for (int i = pList->Count-1; i >= 0; i--)
	{
		MapAIObj* pObj	= (MapAIObj*)pList->Items[i];

		if(pObj->bDisappear==false && pObj->byType != MAPOBJ_PLAYER && pObj->byType != MAPOBJ_ROBOT && pObj->xParam.nHp <= 0)
		{
			if(dwCutTick - pObj->dwTickCmd > 2000)
			{
				m_pParent->AIObjLeaveMap(pObj);
				if(pObj->pBornInfo)	//通知出生点死亡时间
					pObj->pBornInfo->pListDeadTick->Add((void*)dwCutTick);
			}
		}

		if(pObj->bDisappear && pObj->wLockCnt == 0)
		{
			if(pObj->byType == MAPOBJ_PLAYER)
			{
				pObj->dwTickCmd	= GetTickCount();
				m_pListCache->Add(pObj);
			}
			else
			{
				delete pObj;
			}
			pList->Delete(i);
		}
	}
}

void BornCenter::CheckGSSync()
{
	//玩家信息需要同步,如传送点,血量死亡等
	for (int i = 0; i < m_pListGSPlayer->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)m_pListGSPlayer->Items[i];
		if(pObj->bGSSyncPos)
		{
			pObj->bGSSyncPos	= false;
			m_pParent->ObjSyncPos(pObj);
		}
		if(pObj->bGSSyncInfo)
		{
			pObj->bGSSyncInfo	= false;
			m_pParent->ObjSyncInfo(pObj);
		}
	}
	for (int i = 0; i < m_pListGSSlave->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)m_pListGSSlave->Items[i];
		if(pObj->bGSSyncInfo)
		{
			pObj->bGSSyncInfo	= false;
			m_pParent->ObjSyncInfo(pObj);
		}
	}

	//部分怪物信息游戏服务器可能需要关注的
	for (int i = 0; i < m_pListGSEnemy->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)m_pListGSEnemy->Items[i];
		if(pObj->bGSSyncPos)
		{
			pObj->bGSSyncPos	= false;
			if(pObj->pCtrl)
				m_pParent->ObjSyncPos(pObj);
		}
	}
}