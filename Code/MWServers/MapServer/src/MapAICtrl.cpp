#include "stdafx.h"

#include "MapAICtrl.h"
#include "MapAIObj.h"
#include "MapManager.h"
#include "ios_mapcmd_packet.h"
#include "EnemyConfig.h"
#include "RobotConfig.h"
extern int CalcDistance(int nX1,int nY1,int nX2,int nY2);
extern int CalcDistance2(int nX1,int nY1,int nX2,int nY2);
MapAICenter::MapAICenter( MapManager* pParent )
{
	m_pMap	= pParent;
	m_pActCenter	= m_pMap->m_pRbActCenter;
}

MapAICenter::~MapAICenter()
{

}

bool MapAICenter::GetEnemyAICommand( MapAIObj* pObj )
{
	if(pObj->pBase == NULL || pObj->CheckCanRunCmd() == false)
		return false;

	//查看是否有可攻击目标
	if(pObj->pBase->bAtkType == 2 && pObj->bCmd != MCMD_ATTACK)
	{
		xnList* pList	= xnList::Create();
		//方圆一格以内为索敌范围

		int nFRange	= pObj->pBase->wFindRange/MAPPLATE_SIZE;

		m_pMap->GetPlateRangeB(pObj->wMPX,pObj->wMPY,nFRange,nFRange,pList);
		while(pList->Count)
		{
			//从列表中间开始查找
			MapPlate* pPlate	= (MapPlate*)pList->Delete(pList->Count/2);
			MapAIObj* pTarget	= pPlate->GetAtkTarget(pObj);
			if(pTarget)
			{
				pObj->SetCmd(MCMD_ATTACK,0,pTarget);
				pList->Free();
				return true;
			}
		}
		pList->Free();
	}

	//查看是否进行随机移动

	
	if(pObj->bCmd == MCMD_NULL)
	{
		if(pObj->pPtPath)	//是否设置过路径
		{
			if(pObj->m_byAutoAtk >= pObj->wPathPtCnt)
				pObj->m_byAutoAtk	= 0;

			int nDX	= pObj->pPtPath[pObj->m_byAutoAtk].x;
			int nDY	= pObj->pPtPath[pObj->m_byAutoAtk].y;
			
			pObj->SetCmd(MCMD_RUN,(nDY<<16)+nDX);
			pObj->m_byAutoAtk++;
		}
		else if(pObj->dwBattelGroup)	//是否设置过阵型队长
		{
			MapAIObj*	pMaster	= m_pMap->GetMapObj(MAPOBJ_ENEMY,pObj->dwBattelGroup);
			if(pMaster)
			{
				if(CalcDistance2(pMaster->nX,pMaster->nY,pObj->nX,pObj->nY)>40000)
				{
					pObj->JumpInMap(pMaster->nX,pMaster->nY);
				}
				int	nDestX	= pMaster->dwCmdVal&0xFFFF;
				int	nDestY	= pMaster->dwCmdVal>>16;
				if(CalcDistance2(nDestX,nDestY,pObj->nX,pObj->nY)>10000)
					pObj->SetCmd(pMaster->bCmd,pMaster->dwCmdVal);
			}
		}
		else if(pObj->pBornInfo != NULL)
		{
			if(pObj->pBornInfo->byBattleGroup)		//是否一出生就有阵型
			{
				pObj->pBornInfo->CheckBGroupCmd(pObj,m_pMap);
			}
			else if(GetTickCount() -	pObj->dwTickCmd > pObj->dwCmdVal || pObj->pBornInfo->bPtRunCnt)
			{
				int nDX	= pObj->nX;
				int nDY	= pObj->nY;
				pObj->pBornInfo->GetRandPoint(nDX,nDY,m_pMap,pObj->m_byAutoAtk);
				pObj->SetCmd(MCMD_RUN,(nDY<<16)+nDX);
			}

		}
		else if(pObj->dwDTargetID)
		{
			MapAIObj* pTagObj	= m_pMap->GetMapObj(pObj->byDTargetType,pObj->dwDTargetID);

			if(pTagObj)
			{
				if(abs(pObj->nX-pTagObj->nX) + abs(pObj->nY-pTagObj->nY) > 2000)
				{
					pObj->SetCmd(MCMD_RUN,(pTagObj->nY<<16)+pTagObj->nX);
				}
				else
					pObj->SetCmd(MCMD_ATTACK,0,pTagObj);
			}
		}
	}

	return true;
}

bool MapAICenter::GetRobotAICommand( MapAIObj* pObj )
{
	if(m_pActCenter == NULL)
		return false;

	if(pObj->pRunActPt == NULL)
		return false;

	if(pObj->dwTickRunAct && ((int)(GetTickCount() - pObj->dwTickRunAct)) < 0)
	{
		if(pObj->m_byAutoAtk)
			_tryAtk(pObj);
		return false;	//打怪的逻辑
	}
	
	if(abs(pObj->pRunActPt->nX - pObj->nX) > 50 || abs(pObj->pRunActPt->nY-pObj->nY)>50)
	{//回归寻路点
		int nDX	= pObj->pRunActPt->nX-30+rand()%60;
		int nDY	= pObj->pRunActPt->nY-30+rand()%60;
		pObj->SetCmd(MCMD_RUN,(nDY<<16)+nDX);
	}
	
	else if(pObj->pRunActPt->pLActPt->Count)
	{
		_MuliParamObj* pMP	= (_MuliParamObj*)pObj->pRunActPt->pLActPt->Items[rand()%pObj->pRunActPt->pLActPt->Count];
		switch(pMP->nParam1)
		{
		case 1:		//下个点
			{
				pObj->pRunActPt	= m_pActCenter->GetActPt(pMP->nParam2);
				if(pObj->pRunActPt)
				{
					int nDX	= pObj->pRunActPt->nX-30+rand()%60;
					int nDY	= pObj->pRunActPt->nY-30+rand()%60;
					pObj->SetCmd(MCMD_RUN,(nDY<<16)+nDX);
				}
				pObj->dwTickRunAct	= 0;
				pObj->m_byAutoAtk	= 0;
			}
			break;
		case 2:		//打怪
			pObj->dwTickRunAct	= GetTickCount()+pMP->nParam2 - 1500 + rand()%3000;
			pObj->m_byAutoAtk	= 1;
			break;
		case 3:		//不动
			pObj->dwTickRunAct	= GetTickCount()+pMP->nParam2 - 1500 + rand()%3000;
			break;
		}
	}



	return true;
}

void MapAICenter::_tryAtk( MapAIObj* pObj )
{
	if(pObj->bCmd != MCMD_ATTACK)
	{
		xnList* pList	= xnList::Create();
		//方圆一格以内为索敌范围

		int nFRange	= 2;

		m_pMap->GetPlateRangeB(pObj->wMPX,pObj->wMPY,nFRange,nFRange,pList);
		while(pList->Count)
		{
			//从列表中间开始查找
			MapPlate* pPlate	= (MapPlate*)pList->Delete(pList->Count/2);
			MapAIObj* pTarget	= pPlate->GetAtkTarget(pObj);
			if(pTarget)
			{
				pObj->SetCmd(MCMD_ATTACK,0,pTarget);
				pList->Free();
				return;
			}
		}
		pList->Free();
	}
}

