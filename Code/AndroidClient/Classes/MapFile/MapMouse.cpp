#include "MapMouse.h"
#include "MapControl.h"
#include "MainMenu.h"
#include "GuideLayer.h"
#include "MapStaticCfg.h"
#include "MapCopyConfig.h"

MapCommand*	g_pMapCommand	= NULL;
HandSignCmd*	g_pHandSign	= NULL;
MapEventCenter::MapEventCenter()
{
	m_dwTickMove	= 0;
}

MapEventCenter::~MapEventCenter()
{

}

void MapEventCenter::MapEventMove( int nX, int nY )
{
	if(CalcDistance2(nX,nY,m_nClickX,m_nClickY) > 14400 || xnGetTickCount() - m_dwTickMove > 500)
	{
		m_nClickX		= nX;
		m_nClickY		= nY;

		//g_pMapCenter->AddMapEff(3,nX,nY,0);
		m_dwTickMove	= xnGetTickCount();
		g_pMapCommand->CmdPoint(m_nClickX, m_nClickY);
	}
}

bool MapEventCenter::MapEventDown( int nX, int nY )
{
	m_nClickX		= nX;
	m_nClickY		= nY;
	
	if(g_pMapCenter->CheckSelObj(nX,nY) == false)
	{
		if(pMainMenu->m_pMainFormLA->bOpened)
		{
			pMainMenu->m_pMainFormLA->Close();
			pMainMenu->m_pMainFormTA->Close();
			if(pMainMenu->m_ifQMenuShow)
				pMainMenu->m_pMainFormLB->Open(3);
			else
				pMainMenu->m_pMainFormHide->Open();
			pMainMenu->m_pMainFormTB->Open(5);

			pCtrlLayer->setVisible(true);

		}
		//m_bClickHold	= true;
		m_dwTickMove	= xnGetTickCount();
		g_pMapCommand->CmdPoint(m_nClickX, m_nClickY);

		return true;
	}

	return false;
}

void MapEventCenter::MapEventUp( int nX, int nY )
{
	m_dwTickMove	= 0;
}

void MapEventCenter::Update()
{
	
}


//-----------------------------------------------------------
MapCommand::MapCommand()
{
	m_bReCmd	= false;
	m_pObjNAtk	= NULL;
	m_pLPtPath	= xnList::Create();
	m_pLMapTrans	= xnList::Create();
	pGuideCmd	= NULL;
	m_nLastMIndex	= -1;
	m_wTrySkill	= 0;
	m_bFromAuto	= false;
}

MapCommand::~MapCommand()
{
	while(m_pLPtPath->Count)
		delete (POINT*)m_pLPtPath->Delete(0);
	m_pLPtPath->Free();


	m_pLMapTrans->Free();
}

void MapCommand::Reset(bool bMapTrans)
{
	m_pObjNAtk	= NULL;
	m_nPathMIndex	= -1;
	m_bReCmd	= true;
	if(bMapTrans == false)
	{
		pGuideCmd		= NULL;
		m_nLastMIndex	= -1;
		m_pLMapTrans->Clear();
	}

	while(m_pLPtPath->Count)
		delete (POINT*)m_pLPtPath->Delete(0);
}

bool MapCommand::CmdMapPoint( int nMIndex, int nPosX, int nPosY, _BaseGuidePt* pGC )
{
	m_bFromAuto	= false;
	Reset();
	pChar->m_pHorseCenter->RequestRide(true,false);

	if(nMIndex >= 1000)
	{
		MCopyBase*	pMCBase	= MCopyConfig::GetInstance().GetMCopyByMBase(g_pMapCenter->m_nBaseMapIndex);
		if(pMCBase && pMCBase->wID == nMIndex%1000)
		{
			CmdPoint(nPosX, nPosY,false);
			m_nLastPX		= nPosX;
			m_nLastPY		= nPosY;
			pGuideCmd	= pGC;
			return true;
		}
	}

	if(g_pMapCenter->m_nBaseMapIndex == nMIndex)
	{
		CmdPoint(nPosX, nPosY,false);
		m_nLastPX		= nPosX;
		m_nLastPY		= nPosY;
	}
	else
	{
		if(TransPointCfg::GetInstance().FindMapPath(g_pMapCenter->m_nMapIndex,nMIndex,m_pLMapTrans) == false)
		{
			pMainMenu->PopTipMsg("无法到达目标地图");
		}
		
		m_nLastMIndex	= nMIndex;
		m_nLastPX		= nPosX;
		m_nLastPY		= nPosY;
	}

	pGuideCmd	= pGC;
	return true;
}

bool MapCommand::CmdPoint( int nPosX, int nPosY ,bool bReset,bool bFromAuto)
{
	if(g_pMapCenter->m_pMObjLockView == NULL)
		return false;
	if(nPosX < 0 || nPosY < 0 || nPosX > g_pMapCenter->GetMapWidth() || nPosY > g_pMapCenter->GetMapHeight())
	{//地图外面
		return false;
	}
	if(g_pMapCenter->m_pMapBarrier->GetBlock(nPosX,nPosY))
	{
		int	nNewX	= 0,nNewY	= 0;
		g_pMapCenter->m_pMapBarrier->CheckLineBlock(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY,nPosX,nPosY,&nNewX,&nNewY);
		if((nNewX != nPosX || nNewY != nPosY) && (nNewX != g_pMapCenter->m_pMObjLockView->nPosX || nNewY != g_pMapCenter->m_pMObjLockView->nPosY))
		{
			nPosX	= nNewX;
			nPosY	= nNewY;
		}
		else
		{
			//if(bFromAuto == false)
				//pMainMenu->PopTipMsg("目标点存在障碍");
			return false;
		}
	}
	if(bReset)
	{
		Reset();
		m_nLastPX	= nPosX;
		m_nLastPY	= nPosY;
		m_bFromAuto	= bFromAuto;
	}
	else
	{
		m_bReCmd	= true;
		while(m_pLPtPath->Count)
			delete (POINT*)m_pLPtPath->Delete(0);
	}

	if(!bFromAuto && pMainMenu->m_pAutoMenu->m_bActive)
	{
		pMainMenu->m_pAutoMenu->wStartX	= nPosX;
		pMainMenu->m_pAutoMenu->wStartY	= nPosY;
	}

	m_nPathMIndex	= g_pMapCenter->m_nMapIndex;
	return g_pMapCenter->m_pMapFindPath->RequestFindPath(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY,nPosX,nPosY,m_pLPtPath);
}

bool MapCommand::CmdNormalAtk( MapAIObj* pObj,bool bClearSkill )
{
	if(pObj==NULL || pObj->nHP <= 0)
		return false;

	if(g_pMapCenter->m_pMObjLockView == NULL)
		return false;

	if((g_pMapCenter->m_nBaseMapIndex != 50)&&((pObj->m_byType == MAPOBJ_PLAYER && pObj->byLv < 40) ||
		(pObj->m_byType == MAPOBJ_PLAYER && g_pMapCenter->m_pMObjLockView->byLv < 40)))
	{
		if(pObj->bForce != 0 && pObj->bForce != g_pMapCenter->m_pMObjLockView->bForce)
			pMainMenu->PopTipMsg("40级以下不能PK");
		return false;
	}


	Reset();
	if(g_pMapCenter->CheckLineBlock(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY,pObj->nPosX,pObj->nPosY,true))
	{
		CmdPoint(pObj->nPosX,pObj->nPosY,false);
		//m_pObjNAtk	= pObj;
		return false;
	}
	if(bClearSkill)
		m_wTrySkill	= 0;
	m_pObjNAtk	= pObj;
	return true;
}

bool MapCommand::CmdSkillAtk( MapAIObj* pObj,WORD wSID )
{
	if(pObj && pObj->nHP <= 0)
		pObj	= NULL;

	if(g_pMapCenter->m_pMObjLockView == NULL)
		return false;

	if(pObj)
	{
		if((g_pMapCenter->m_nBaseMapIndex != 50)&&((pObj->m_byType == MAPOBJ_PLAYER && pObj->byLv < 40) ||
			(pObj->m_byType == MAPOBJ_PLAYER && g_pMapCenter->m_pMObjLockView->byLv < 40)))
		{
			if(pObj->bForce != 0 && pObj->bForce != g_pMapCenter->m_pMObjLockView->bForce)
				pMainMenu->PopTipMsg("40级以下不能PK");
			return false;
		}
	}
	if(pObj && pObj->bForce == g_pMapCenter->m_pMObjLockView->bForce)
		return false;


	Reset();
	if(pObj && g_pMapCenter->CheckLineBlock(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY,pObj->nPosX,pObj->nPosY,true))
	{
		CmdPoint(pObj->nPosX,pObj->nPosY,false);
		return false;
	}
	pChar->m_dwLockSkill	= xnGetTickCount();
	pChar->m_wLockSkillID	= wSID;
	m_wTrySkill	= wSID;
	m_pObjNAtk	= pObj;
	return true;
}

bool MapCommand::PopNextPoint(int nMap, POINT& pt)
{
	if(m_nPathMIndex != nMap)
	{
		for (int i = 0; i < m_pLMapTrans->Count; i++)
		{
			_TransPoint* pTP	= (_TransPoint*)m_pLMapTrans->Items[i];
			if(pTP->nMapindex == nMap)
			{
				CmdPoint(pTP->nPosX,pTP->nPosY,false);
				m_pLMapTrans->Delete(i);
				break;
			}
		}
		if(m_nLastMIndex == nMap)
		{
			m_pLMapTrans->Clear();
			CmdPoint(m_nLastPX,m_nLastPY,false);
		}
	}

	if((m_nPathMIndex != -1 && nMap != m_nPathMIndex) || m_pLPtPath->Count == 0)
	{
		if(m_bReCmd)
		{
			m_bReCmd = false;
			return true;
		}
		return false;
	}

	if(m_bReCmd)
	{//重下指令上层改变目标
		pt	= *(POINT*)m_pLPtPath->Items[0];
		m_bReCmd	= false;
		return true;
	}
	else
	{
		POINT*	ptCur	= (POINT*)m_pLPtPath->Items[0];
		if(abs(pt.x - ptCur->x) < 5 && abs(pt.y-ptCur->y) < 5)
		{//走完当前路径节点转下个节点
			delete ptCur;
			m_pLPtPath->Delete(0);
			if(m_pLPtPath->Count)
			{
				pt	= *(POINT*)m_pLPtPath->Items[0];
				return true;
			}
		}
	}
	
	return false;
}

MapAIObj* MapCommand::PopAtkCmd(int& nSkill)
{
	nSkill	= m_wTrySkill;
	if(m_pObjNAtk && m_pObjNAtk->nHP <= 0)
	{
		m_pObjNAtk	= NULL;
	}
	return m_pObjNAtk;
}

bool MapCommand::IsMoving()
{
	MapAIObj* pAIObj	= pChar->GetMyMapObj();
	if(pAIObj && pAIObj->m_byState == OBJ_STATE_RUN)
		return true;
	return m_pLPtPath->Count;
}

bool MapCommand::IsInRest()
{
	MapAIObj* pAIObj	= pChar->GetMyMapObj();
	if(pAIObj && pAIObj->m_byState != OBJ_STATE_STAND)
		return false;
	if(m_pLPtPath->Count)
		return false;

	if(pMainMenu->IsCasting())
		return false;

	if(m_pObjNAtk)
		return false;

	//任务对话中
	if(pMainMenu->m_pMissionMenu->m_pFormTrack->bOpened)
		return false;

	return true;
}

bool MapCommand::CmdPathObj( _BaseGuidePt* pGC )
{
	pMainMenu->m_pAutoMenu->SwitchActive(false);
	if(pGC->nMapindex >= 1000 && g_pMapCenter->m_nBaseMapIndex != pGC->nMapindex%1000)
	{
		MCopyBase*	pMCBase	= MCopyConfig::GetInstance().GetMCopyByMBase(pGC->nMapindex-1000);
		if(pMCBase == NULL)
			pMCBase	= MCopyConfig::GetInstance().GetMCopy(pGC->nMapindex-1000);
		if(pMCBase && pMCBase->byType >= 3)
		{//寻路
			return g_pMapCommand->CmdMapPoint(pGC->nMapindex,pGC->nPosX,pGC->nPosY,pGC);
		}
		else
		{
			pMainMenu->m_pMapCopyMenu->OpenForm(pMCBase);
			return true;
		}
	}
	else if(pGC->nMapindex < 1000 && MapViewConfig::GetInstance().GetMapTileData(pGC->nMapindex)	== NULL)
	{
		if(pChar->m_byCountry == 1)
		{
			return g_pMapCommand->CmdMapPoint(pGC->nMapindex+100,pGC->nPosX,pGC->nPosY,pGC);
		}
		else
			return g_pMapCommand->CmdMapPoint(pGC->nMapindex+200,pGC->nPosX,pGC->nPosY,pGC);
	}
	else
		return g_pMapCommand->CmdMapPoint(pGC->nMapindex%1000,pGC->nPosX,pGC->nPosY,pGC);

	return false;
}

HandSignCmd::HandSignCmd()
{
	pListPt		= xnList::Create();
	dwTickCmd	= 0;
}

HandSignCmd::~HandSignCmd()
{
	while(pListPt->Count)
		delete (_MuliParamObj*)pListPt->Delete(0);
	pListPt->Free();
}

BYTE HandSignCmd::AddPtCheck( int nX, int nY )
{
	if(xnGetTickCount() - dwTickCmd > 800)
		ClearPt();
	_MuliParamObj*	pMp	= new _MuliParamObj;
	pMp->nParam1	= nX;
	pMp->nParam2	= nY;
	pListPt->Add(pMp);

	dwTickCmd	= xnGetTickCount();
	return _AnalyPtList();
}

void HandSignCmd::ClearPt()
{
	while(pListPt->Count)
		delete (_MuliParamObj*)pListPt->Delete(0);
}

BYTE HandSignCmd::_AnalyPtList()
{
	if(pListPt->Count < 2)
		return 0;
	_MuliParamObj* pMPTmp	= (_MuliParamObj*)pListPt->Items[0];
	int	nStartX	= pMPTmp->nParam1;
	int	nStartY	= pMPTmp->nParam2;
	pMPTmp	= (_MuliParamObj*)pListPt->Items[pListPt->Count-1];
	int	nEndX	= pMPTmp->nParam1;
	int	nEndY	= pMPTmp->nParam2;

	int	nDifX	= nEndX-nStartX;
	int	nDifY	= nEndY-nStartY;

	if(abs(nDifX)+abs(nDifY) > 30)
	{
		if(abs(nDifY) < abs(nDifX)/3)
		{
			if(nDifX > 0)
			{
				if(_checkPts(1))
					return 1;
			}
			else
				if(_checkPts(2))
					return 2;
		}
		else if(abs(nDifY)/3 > abs(nDifX))
		{
			if(nDifY > 0)
			{
				if(_checkPts(4))
					return 4;
			}
			else
				if(_checkPts(3))
					return 3;
		}
		else
		{
			if(nDifX < 0)
			{
				if(nDifY > 0)
				{
					if(_checkPts(5))
						return 5;
				}
				else
					if(_checkPts(7))
						return 7;
			}
			else
			{
				if(nDifY > 0)
				{
					if(_checkPts(6))
						return 6;
				}
				else
					if(_checkPts(8))
						return 8;
			}
		}
	}
	return 0;
}

bool HandSignCmd::_checkPts( BYTE bType )
{
	int		nOkPtCnt	= 0;
	int		nTotalCnt	= pListPt->Count;
	for (int i = 1; i < nTotalCnt; i++)
	{
		_MuliParamObj*	pMp1	= (_MuliParamObj*)pListPt->Items[i-1];
		_MuliParamObj*	pMp2	= (_MuliParamObj*)pListPt->Items[i];
		if(bType == _checkPt(pMp1->nParam1,pMp1->nParam2,pMp2->nParam1,pMp2->nParam2))
		{
			nOkPtCnt++;
		}
	}

	ClearPt();
	if(nOkPtCnt > nTotalCnt/3)
	{
		return true;
	}
	else
		return false;
}

BYTE HandSignCmd::_checkPt( int nSX, int nSY, int nEX, int nEY )
{
	int	nDifX	= nEX-nSX;
	int	nDifY	= nEY-nSY;

	if(abs(nDifX)+abs(nDifY) > 3)
	{
		if(abs(nDifY) < abs(nDifX)/3)
		{
			if(nDifX > 0)
			{
				return 1;
			}
			else
				return 2;
		}
		else if(abs(nDifY)/3 > abs(nDifX))
		{
			if(nDifY > 0)
			{
				return 4;
			}
			else
				return 3;
		}
		else
		{
			if(nDifX < 0)
			{
				if(nDifY > 0)
				{
					return 5;
				}
				else
					return 7;
			}
			else
			{
				if(nDifY > 0)
				{
					return 6;
				}
				else
					return 8;
			}
		}
	}
}
