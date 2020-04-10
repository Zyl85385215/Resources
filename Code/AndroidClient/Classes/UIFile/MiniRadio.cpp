#include "MainMenu.h"
#include "MapControl.h"
#include "MiniRadio.h"
#include "UserData.h"
#include "MapMouse.h"
#include "MapStaticCfg.h"

void ClickMiniMap(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MiniMapMenu*	pMenu	= (MiniMapMenu*)param;
	pMenu->OpenBigMap();
}

void MoveFrmDown(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MiniMapMenu*	pMenu	= (MiniMapMenu*)param;
	pMenu->m_pFormBig->pCFrame->nChgY += pMenu->m_pFormBig->pCFrame->nDisStep;
}

void BigMapTouchEvent(InterfaceEvent * pEvent,void * param)
{
	MiniMapMenu*	pMenu	= (MiniMapMenu*)param;
	static int	nClickX	= 0;
	static int	nClickY	= 0;
	static int	nDownX	= 0;
	static int	nDownY	= 0;
	static DWORD	s_dwBMTouchDown	= xnGetTickCount();
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_DOWN:
		nClickX		= pEvent->x;
		nClickY		= pEvent->y;
		nDownX		= nClickX;
		nDownY		= nClickY;
		s_dwBMTouchDown	= xnGetTickCount();
		pMenu->m_dwTickBMapLock	= xnGetTickCount()+15000;
		break;
	case INTERFACE_EVENT_MOVE:
		pMenu->MoveMap(pEvent->pComponent->nX + pEvent->x - nClickX,pEvent->pComponent->nY + pEvent->y-nClickY);
		nClickX		= pEvent->x;
		nClickY		= pEvent->y;
		pMenu->m_dwTickBMapLock	= xnGetTickCount()+15000;
		break;
	case INTERFACE_EVENT_UP:
		if(xnGetTickCount() - s_dwBMTouchDown < 500 && (abs(nClickX - nDownX) + abs(nClickY-nDownY)) < 20)
		{
			if(pMenu->m_nCurBaseMIndex == g_pMapCenter->m_nBaseMapIndex)
				g_pMapCommand->CmdPoint((nClickX-pEvent->pComponent->nX+pMenu->m_nStartX)*8,(nClickY-pEvent->pComponent->nY+pMenu->m_nStartY)*8);
			else
				g_pMapCommand->CmdMapPoint(pMenu->m_nCurBaseMIndex,(nClickX-pEvent->pComponent->nX+pMenu->m_nStartX)*8,(nClickY-pEvent->pComponent->nY+pMenu->m_nStartY)*8);
		}
		pMenu->m_dwTickBMapLock	= xnGetTickCount()+3000;
		break;
	}
}

void MMapLeadEvent(int nPos,char* szName,void* param)
{
	MiniMapMenu* pMenu	= (MiniMapMenu*)param;

	_BaseGuidePt* pBPt	= (_BaseGuidePt*)pMenu->m_pFormBig->pCFrame->GetFPTag(nPos);
	//g_pMapCommand->CmdMapPoint(pBPt->nMapindex,pBPt->nPosX,pBPt->nPosY,pBPt);
	g_pMapCommand->CmdPathObj(pBPt);
	if(pMenu->m_nCurSelGuide != -1)
		pMenu->m_pFormBig->pCFrame->GetFPComp(pMenu->m_nCurSelGuide,"mb")->SetVisible(false);
	pMenu->m_nCurSelGuide	= nPos;
	pMenu->m_pFormBig->pCFrame->GetFPComp(pMenu->m_nCurSelGuide,"mb")->SetVisible(true);
}

void ClickMapEvent(InterfaceEvent * pEvent,void * param)
{
	char	szExTmp[64];
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_DOWN:
		{
			sprintf(szExTmp,"%sa",pEvent->pComponent->szName);
			pEvent->pComponent->pParent->GetComponent(szExTmp)->SetSprColor(128,128,128);
		}
		break;
	case INTERFACE_EVENT_UP:
		{
			sprintf(szExTmp,"%sa",pEvent->pComponent->szName);
			pEvent->pComponent->pParent->GetComponent(szExTmp)->SetSprColor(255,255,255);
		}
		break;
	}


	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if ((int)param == 101&&pChar->m_byCountry == 2||(int)param == 201&&pChar->m_byCountry == 1)
	{
		pMainMenu->PopTipMsg("敌方主城，无法自动寻路！");
		return;
	}
	pMainMenu->m_pMiniMap->ChgMap((int)param);
	pMainMenu->m_pMiniMap->m_pFormBig->Open();
	pMainMenu->m_pMiniMap->MoveMap(2000,2000);
	pMainMenu->m_pMiniMap->m_dwTickBMapLock	= xnGetTickCount()+200;
	pMainMenu->m_pMiniMap->m_bOpenInit			= true;

	if(nowScreenWidth < 960 || nowScreenHeight < 640)
		pMainInterface->m_pHideBackForm		= pMainMenu->m_pMiniMap->m_pFormBig;
}

void OpenWorldMapEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMiniMap->m_pFormBig->Close();
	pMainMenu->m_pMiniMap->m_pFormWorld->Open();
}

void CloseWorldEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMiniMap->m_pFormWorld->Close();
	pMainMenu->m_pMiniMap->OpenBigMap();
}

void BackCurMapEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMiniMap->ChgMap(g_pMapCenter->m_nBaseMapIndex);
}

MiniMapMenu::MiniMapMenu()
{
	m_pForm		= pMainInterface->GetDForm("map");

	m_pDCompRadar	= m_pForm->GetComponent("radar");
	m_pDCompRadar->SetEvent(ClickMiniMap, this);
	m_pDCompMine	= m_pForm->GetComponent("me");
	m_pDCompMName		= m_pForm->GetComponent("RgnName");
	m_pDCompPos		= m_pForm->GetComponent("zb");
	m_pDCompPos->m_nFontSize	= 18;
	//m_pDCompPos->SetSprColor(220,180,33);
	m_pListRadars	= xnList::Create();

	m_pFormBig	= pMainInterface->GetDForm("bigmap");
	m_pCompBigMap	= m_pFormBig->GetComponent("show");
	m_pDCompBMMine	= m_pFormBig->GetComponent("me");
	m_pFormBig->GetComponent("name")->m_nFontSize	= 26;
	m_pCompBigMap->byType	= COMPONENT_MAPGUIDE;
	m_pCompBigMap->SetEvent(BigMapTouchEvent, this);
	m_pCompBigMap->bAlignX	= 0;
	m_pCompBigMap->bAlignY	= 0;
	m_nSrcX		= m_pCompBigMap->nX;
	m_nSrcY		= m_pCompBigMap->nY;

	m_nMineDir	= 0;
	m_nCurSelGuide	= -1;
	m_nCurBaseMIndex	= -1;
	m_bGuideMark	= false;
	m_nMaxLineCnt	= 0;

	m_pFormBig->pCFrame		= new CCtrlFrame(588,154,350,350);

	DComponent*	pComp	= m_pFormBig->GetComponent ("mb");
	DComponent* pComItem[2];
	pComItem[0]			= m_pFormBig->GetComponent("fujin");
	pComItem[1]			= m_pFormBig->GetComponent("tb");
	m_pFormBig->pCFrame->InitFParts(pComp,pComItem,2);
	m_pFormBig->pCFrame->nSpace	= 10;
	m_pFormBig->pCFrame->nDisStep	= 50;
	m_pFormBig->pCFrame->SetFPCnt(8,false,false);
	m_pFormBig->pCFrame->SetEventEx(MMapLeadEvent,this);

	m_pFormBig->GetComponent("down1")->SetEvent(MoveFrmDown,this);

	m_dwTickBMapLock	= 0;

	m_pFormWorld		= pMainInterface->GetDForm("worldmap");

	char	szTmp[32];
	for (int i = 1; i < 50; i++)
	{
		sprintf(szTmp,"%d",i);
		DComponent*	pTmp	= m_pFormWorld->GetComponent(szTmp);
		if(pTmp)
			pTmp->SetEvent(ClickMapEvent, (void*)i);
	}

	for (int j = 0; j < 3; j++)
	{
		sprintf(szTmp,"d%d",j+1);
		m_pCTickComp[j] = m_pForm->GetComponent(szTmp);
	}

	m_pFormWorld->GetComponent("101")->SetEvent(ClickMapEvent, (void*)101);
	m_pFormWorld->GetComponent("201")->SetEvent(ClickMapEvent, (void*)201);
	m_nMaxWidth		= 0;
	m_nMaxHeight	= 0;

	m_pFormBig->GetComponent("shijie")->SetEvent(OpenWorldMapEvent, this);
	m_pFormBig->GetComponent("fanhui")->SetEvent(BackCurMapEvent, this);
	m_pFormWorld->GetComponent("close")->SetEvent(CloseWorldEvent, this);
	m_pFormWorld->GetComponent("name")->SetCaption("世界地图",1);
	m_pFormWorld->GetComponent("name")->m_nFontSize	= 26;
}

MiniMapMenu::~MiniMapMenu()
{
	m_pListRadars->Free();
}

void MiniMapMenu::Update()
{
	_updateRidar();
	_updatePathLine();
	if(m_bGuideMark)
	{//重新标注寻路的后缀
		if(m_pFormBig->bOpened)
		{
			for (int i = 0; i < m_pFormBig->pCFrame->GetFPCnt(); i++)
			{
				_BaseGuidePt*	pBPT	= (_BaseGuidePt*)m_pFormBig->pCFrame->GetFPTag(i);
			
				DComponent*	pDCompIco	= m_pFormBig->pCFrame->GetFPComp(i,"tb");
				
				switch(pBPT->byMark)
				{
				case 1:
					pDCompIco->SetViewSpr("TB_MAP1.png");
					break;
				case 2:
					pDCompIco->SetViewSpr("TB_MAP2.png");
					break;
				case 3:
					pDCompIco->SetViewSpr("TB_MAP3.png");
					break;
				case 4:
					pDCompIco->SetViewSpr("TB_MAP4.png");
					break;
				default:
					pDCompIco->SetViewSpr("");
					break;
				}
			}
			m_bGuideMark	= false;
		}
	}

	for (int i = 0; i < 3; i++)
	{
		m_pCTickComp[i]->SetCaption("",0);
	}	

	if (pChar->m_dwLogoutWaitTick)
	{
		int nTmpSec = (pChar->m_dwLogoutWaitTick - xnGetTickCount())/1000;
		char szTmp[32];
		for (int i = 0; i < 3; i++)
		{
			if (nTmpSec < 1)
			{
				pChar->m_dwLogoutWaitTick = 0;
				m_pCTickComp[i]->SetCaption("",0);
				continue;
			}

			int nNowSec = nTmpSec+2-i;
			if (nNowSec > 0)
			{
				sprintf(szTmp,"%d秒后服务器关闭维护。",nNowSec);
				m_pCTickComp[i]->SetCaption(szTmp,0);
				m_pCTickComp[i]->SetSprColor(255,0,0);
			}else
				m_pCTickComp[i]->SetCaption("",0);
		}	
	}
}

void MiniMapMenu::_updateRidar()
{
	if(g_pMapCenter->m_pMObjLockView == NULL)
		return;
	static	DWORD	s_dwUpTick	= 0;
	if(xnGetTickCount() - s_dwUpTick < 100)
		return;
	s_dwUpTick	= xnGetTickCount();

	int		nCenterX	= g_pMapCenter->m_pMObjLockView->nPosX;
	int		nCenterY	= g_pMapCenter->m_pMObjLockView->nPosY;

	m_pDCompPos->SetCaptionEx("(%d , %d)",1,nCenterX/64,nCenterY/64);

	xnList*	pList	= xnList::Create();
	for (int i = 0; i < g_pMapCenter->m_pMEnemyList->m_pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)g_pMapCenter->m_pMEnemyList->m_pList->Items[i];
		
		int nDis	= CalcDistance2(nCenterX,nCenterY,pObj->nPosX,pObj->nPosY);
		if(nDis	< 250000)
		{
			m_pDCompRadar->SetAddSprByTag("MB_MON.png",(pObj->nPosX-nCenterX)/10+m_pDCompRadar->nW/2,(nCenterY - pObj->nPosY)/10+m_pDCompRadar->nH/2,(int)pObj);
			pList->Add(pObj);
		}
	}
	for (int i = 0; i < g_pMapCenter->m_pMNpcList->m_pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)g_pMapCenter->m_pMNpcList->m_pList->Items[i];
		int nDis	= CalcDistance2(nCenterX,nCenterY,pObj->nPosX,pObj->nPosY);
		if(nDis	< 250000)
		{
			m_pDCompRadar->SetAddSprByTag("MB_NPC.png",(pObj->nPosX-nCenterX)/10+m_pDCompRadar->nW/2,(nCenterY - pObj->nPosY)/10+m_pDCompRadar->nH/2,(int)pObj);
			pList->Add(pObj);
		}
	}

	//for (int i = 0; i < g_pMapCenter->m_pMSlaveList->m_pList->Count; i++)
	//{
	//	MapAIObj* pObj	= (MapAIObj*)g_pMapCenter->m_pMSlaveList->m_pList->Items[i];
	//	int nDis	= CalcDistance2(nCenterX,nCenterY,pObj->nPosX,pObj->nPosY);
	//	if(nDis	< 250000)
	//	{
	//		m_pDCompRadar->SetAddSprByTag("MB_MON.png",(pObj->nPosX-nCenterX)/10+m_pDCompRadar->nW/2,(nCenterY - pObj->nPosY)/10+m_pDCompRadar->nH/2,(int)pObj);
	//		pList->Add(pObj);
	//	}
	//}

	for (int i = 0; i < g_pMapCenter->m_pMPlayerList->m_pList->Count; i++)
	{
		MapAIObj* pObj	= (MapAIObj*)g_pMapCenter->m_pMPlayerList->m_pList->Items[i];
		if(pObj == g_pMapCenter->m_pMObjLockView)
			continue;
		int nDis	= CalcDistance2(nCenterX,nCenterY,pObj->nPosX,pObj->nPosY);
		if(nDis	< 250000)
		{
			m_pDCompRadar->SetAddSprByTag("MB_WJ.png",(pObj->nPosX-nCenterX)/10+m_pDCompRadar->nW/2,(nCenterY - pObj->nPosY)/10+m_pDCompRadar->nH/2,(int)pObj);
			pList->Add(pObj);
		}
	}

	xnList*	pLOut	= xnList::Create();
	m_pListRadars->CheckMyOnly(pList,pLOut);
	while (pLOut->Count)
	{
		m_pDCompRadar->SetAddSprByTag("",0,0,(int)pLOut->Delete(0));
	}
	pLOut->Free();
	m_pListRadars->Free();
	m_pListRadars	= pList;
	//MB_NPC.png
	if(g_pMapCenter->m_pMObjLockView && g_pMapCenter->m_pMObjLockView->nPosX != g_pMapCenter->m_pMObjLockView->nDestX &&
		g_pMapCenter->m_pMObjLockView->nPosY != g_pMapCenter->m_pMObjLockView->nDestY)
	{
		m_nMineDir	= CalcDirection(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY
			,g_pMapCenter->m_pMObjLockView->nDestX,g_pMapCenter->m_pMObjLockView->nDestY,100);
		m_pDCompMine->SetSprRotation(m_nMineDir);

		int  nradio = 1;
	}

	if(m_pFormBig->bOpened)
	{
		m_pDCompBMMine->SetSprRotation(m_nMineDir);

		MapAIObj* pObj	= pChar->GetMyMapObj();
		if(pObj)
		{
			int nPosMX	= m_pCompBigMap->nX+pObj->nPosX/8;
			int nPosMY	= m_pCompBigMap->nY+pObj->nPosY/8;
			
			if(nPosMX < m_nSrcX)	nPosMX	= m_nSrcX;
			if(nPosMY < m_nSrcY)	nPosMY	= m_nSrcY;
			if(nPosMX > m_nSrcX+m_pCompBigMap->nW)	nPosMX = m_nSrcX+m_pCompBigMap->nW;
			if(nPosMY > m_nSrcY+m_pCompBigMap->nH)  nPosMY = m_nSrcY+m_pCompBigMap->nH;
			m_pDCompBMMine->SetPos(nPosMX-7,nPosMY-8);

			if(g_pMapCommand->IsMoving())
			{
				if(g_pMapCommand->m_nLastMIndex == -1 || g_pMapCommand->m_nLastMIndex == g_pMapCenter->m_nMapIndex)
				{
					int	nPosTX	= g_pMapCommand->m_nLastPX/8 -32;
					int nPosTY	= (g_pMapCenter->m_pMapTile->m_nTHeight-g_pMapCommand->m_nLastPY)/8 -8;

					char	szTmp[32];
					sprintf(szTmp,"MB_DITU%d.png",xnGetTickCount()/100%5 + 1);
					m_pCompBigMap->SetAddSprByTag(szTmp,nPosTX,nPosTY,999);
				}
			}
			else
				m_pCompBigMap->SetAddSprByTag("",0,0,999);
		}

		if(xnGetTickCount() > m_dwTickBMapLock)
		{//锁定当前角色
			MapAIObj* pObj	= pChar->GetMyMapObj();
			if(pObj && (pObj->m_byState == OBJ_STATE_RUN || m_bOpenInit)) 
			{
				m_bOpenInit	= false;
				int nPosMX	= m_nSrcX+m_pCompBigMap->nW/2-pObj->nPosX/8;
				int nPosMY	= m_nSrcY+m_pCompBigMap->nH/2-pObj->nPosY/8;
				MoveMap(nPosMX,nPosMY,300);
			}
		}
	}
}

void MiniMapMenu::ChgMap(int	nBase)
{
	if(m_nCurBaseMIndex == nBase)
		return;

	MapTileData*	pTile	= MapViewConfig::GetInstance().GetMapTileData(nBase);
	if(!pTile)
	{
		nBase	+= pChar->m_byCountry*100;
		pTile	= MapViewConfig::GetInstance().GetMapTileData(nBase);
	}

	m_nMaxWidth		= 0;
	m_nMaxHeight	= 0;
	m_nCurBaseMIndex	= nBase;

	m_pFormWorld->Close();
	m_pDCompBMMine->SetVisible(m_nCurBaseMIndex == g_pMapCenter->m_nBaseMapIndex);

	if(!pTile)
		return;
	
	m_nMaxWidth		= pTile->m_nTWidth;
	m_nMaxHeight	= pTile->m_nTHeight;

	char	szTmp[256];
	if(pTile->byRealMiniMap)
		sprintf(szTmp,"Map/%d/minmap.jpg",nBase);
	else
		sprintf(szTmp,"Map/%d/minmap.jpg",pTile->wRealIndex);
	m_pCompBigMap->SetViewSpr(szTmp);

	m_pFormBig->GetComponent("name")->SetCaption(pTile->m_szName,1);

	xnList*	pListTmp	= xnList::Create();
	NpcPathLeadCfg::GetInstance().GetNpcListByMIndex(nBase,pListTmp);
	int nNpcCnt	= pListTmp->Count;
	MonsterPathLeadCfg::GetInstance().GetMonListByMIndex(nBase,pListTmp);
	int nMonCnt	= pListTmp->Count;
	xnList*	pLTP	= TransPointCfg::GetInstance().GetMapTP(nBase);
	if(pLTP)
	{
		for (int i = 0; i < pLTP->Count; i++)
		{
			pListTmp->Add(pLTP->Items[i]);
		}
	}

	m_pFormBig->pCFrame->SetFPCnt(pListTmp->Count);
	for (int i = 0; i < pListTmp->Count; i++)
	{
		_BaseGuidePt*	pBPT	= (_BaseGuidePt*)pListTmp->Items[i];
		m_pFormBig->pCFrame->SetFPTag(i,pBPT);
		DComponent*	pDCompTxt	= m_pFormBig->pCFrame->GetFPComp(i,"fujin");
		pDCompTxt->SetCaption(pBPT->szName,0);
		if(i < nNpcCnt)
			pDCompTxt->SetSprColor(0,255,0);
		else if(i < nMonCnt)
			pDCompTxt->SetSprColor(255,0,0);
		else
			pDCompTxt->SetSprColor(255,255,255);
		m_pFormBig->pCFrame->GetFPComp(i,"mb")->SetVisible(false);

	}
	m_nCurSelGuide	= -1;
	pListTmp->Free();
}

void MiniMapMenu::OpenBigMap()
{
	if(m_nCurBaseMIndex	!= g_pMapCenter->m_nBaseMapIndex)
		ChgMap(g_pMapCenter->m_nBaseMapIndex);
	m_pFormBig->Open(1);
	m_bGuideMark	= true;
	MoveMap(m_nSrcX,m_nSrcY);
	m_dwTickBMapLock	= xnGetTickCount()+200;
	m_bOpenInit			= true;

	if(nowScreenWidth < 960 || nowScreenHeight < 640)
		pMainInterface->m_pHideBackForm		= m_pFormBig;
}

void MiniMapMenu::MoveMap( int nDifX, int nDifY, bool dwTick )
{
	if(m_nMaxWidth == 0 || m_nMaxHeight	== 0)
		return;
	if(m_nMaxWidth/8 < m_pCompBigMap->nW)
	{
		nDifX	= m_nSrcX+(m_pCompBigMap->nW-m_nMaxWidth/8)/2;
		m_nStartX	= nDifX;
	}
	else
	{
		m_nStartX	= m_nSrcX;
		if(nDifX > m_nSrcX)
			nDifX	= m_nSrcX;
		if(nDifX < m_nSrcX-m_nMaxWidth/8 + m_pCompBigMap->nW)
			nDifX	= m_nSrcX-m_nMaxWidth/8+ m_pCompBigMap->nW;
	}


	if(m_nMaxHeight/8 < m_pCompBigMap->nH)
	{
		nDifY	= m_nSrcY+(m_pCompBigMap->nH-m_nMaxHeight/8)/2;
		m_nStartY	= nDifY;
	}
	else
	{
		m_nStartY	= m_nSrcY;
		if(nDifY > m_nSrcY)
			nDifY	= m_nSrcY;

		if(nDifY < m_nSrcY-m_nMaxHeight/8+ m_pCompBigMap->nH)
			nDifY	= m_nSrcY-m_nMaxHeight/8+ m_pCompBigMap->nH;
	}

	if(m_pCompBigMap->SetPos(nDifX,nDifY))
	{
		MapAIObj* pObj	= pChar->GetMyMapObj();
		if(pObj)
		{
			int nPosMX	= m_pCompBigMap->nX+pObj->nPosX/8;
			int nPosMY	= m_pCompBigMap->nY+pObj->nPosY/8;

			if(nPosMX < m_nSrcX)	nPosMX	= m_nSrcX;
			if(nPosMY < m_nSrcY)	nPosMY	= m_nSrcY;
			if(nPosMX > m_nSrcX+m_pCompBigMap->nW)	nPosMX = m_nSrcX+m_pCompBigMap->nW;
			if(nPosMY > m_nSrcY+m_pCompBigMap->nH)  nPosMY = m_nSrcY+m_pCompBigMap->nH;
			m_pDCompBMMine->SetPos(nPosMX-7,nPosMY-8,dwTick);
		}
	}

}

void MiniMapMenu::_updatePathLine()
{
	if(m_pFormBig->bOpened == false)
		return;
	if(m_nCurBaseMIndex != g_pMapCenter->m_nBaseMapIndex)
		return;
	MapAIObj* pObj	= pChar->GetMyMapObj();
	if(pObj == NULL || pObj->m_byState != OBJ_STATE_RUN)
	{
		for (int i = 0; i < m_nMaxLineCnt; i++)
		{
			m_pCompBigMap->SetAddSprByTag("",0,0,i);
		}
		return;
	}

	int	nPosMX	= pObj->nPosX/8;
	int nPosMY	= (g_pMapCenter->m_pMapTile->m_nTHeight-pObj->nPosY)/8;


	m_nMaxLineCnt	= max(m_nMaxLineCnt,g_pMapCommand->m_pLPtPath->Count);
	for (int i = 0; i < g_pMapCommand->m_pLPtPath->Count; i++)
	{
		POINT* pPt	= (POINT*)g_pMapCommand->m_pLPtPath->Items[i];
		int		nLineX	= pPt->x/8;
		int		nLineY	= (g_pMapCenter->m_pMapTile->m_nTHeight-pPt->y)/8;

		int	nDis	= CalcDistance(nPosMX,nPosMY,nLineX,nLineY);
		int	nDir	= CalcDirection(-nPosMX,nPosMY,-nLineX,nLineY,100);
		m_pCompBigMap->SetAddSprByTag("MB_LINE.png",nPosMX,nPosMY,i,nDis*100/3,(nDir+90)%360);

		nPosMX	= nLineX;
		nPosMY	= nLineY;
	}
	for (int i = g_pMapCommand->m_pLPtPath->Count; i < m_nMaxLineCnt; i++)
	{
		m_pCompBigMap->SetAddSprByTag("",0,0,i);
	}
}
