#include "MainMenu.h"
#include "MapCopyMenu.h"
#include "UserData.h"
#include "MapCopyConfig.h"
#include "MapControl.h"
#include "MallConfig.h"
#include "ItemConfig.h"
#include "ios_item_packet.h"

void fun_cbMCopyTeam(void * pParam,int nVal)
{
	BuildPacketEx(pPacket, USER, C_RQ_TEAM_MCOPY,buf, 256);
	SET_DATA(pSend, USER, C_RQ_TEAM_MCOPY, pPacket);
	pSend->wMCID	= 0;
	pChar->SendPacket(pPacket);

	((MapCopyMenu*)pParam)->m_dwWaitTeam	= 0;
}

void MCopyFrameEvent(int nPos,char* szName,void* param)
{
	MapCopyMenu*	pMenu	= (MapCopyMenu*)param;

	if(strncmp(szName,"zi",2) == 0)
	{
		pMenu->SetCurSel(nPos);
	}
	else if(strncmp(szName,"dl",2) == 0)
	{
		pMainMenu->m_pMapCopyDropMenu->OpenDForm(nPos);
	}
}

void MCopyStartEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MapCopyMenu*	pMenu	= (MapCopyMenu*)param;

	if(pChar->m_pCQuestCenter->GetCurQuest(5001) ||
		pChar->m_pCQuestCenter->GetCurQuest(5002) ||
		pChar->m_pCQuestCenter->GetCurQuest(5003))
	{
		pMainMenu->PopTipMsg("你正在护送商队，无法进入副本！");
		return;
	}
	pMainMenu->ShowNetWaiting();
	pMenu->RequestEnter();
}

void MCopyTeamEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MapCopyMenu*	pMenu	= (MapCopyMenu*)param;
	if(pChar->m_pCQuestCenter->GetCurQuest(5001) ||
		pChar->m_pCQuestCenter->GetCurQuest(5002) ||
		pChar->m_pCQuestCenter->GetCurQuest(5003))
	{
		pMainMenu->PopTipMsg("你正在护送商队，无法进入副本！");
		return;
	}
	pMenu->RequestTeam();
}

void MCopyPageEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MapCopyMenu*	pMenu	= (MapCopyMenu*)param;

	int nPos	= 0;
	sscanf(pEvent->pComponent->szName,"page%d",&nPos);
	nPos--;
	pMenu->InitMenu(nPos);
}

void AddPowEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MapCopyMenu*	pMenu	= (MapCopyMenu*)param;
	pMainMenu->ShowNetWaiting();
	pMenu->RequestAddPow();
}

void BuyPowEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MapCopyMenu*	pMenu	= (MapCopyMenu*)param;
	pMenu->RequestBuyPow();
}

static void UseStrengthEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = pChar->m_pItemCenter->GetItemByTypeID(30008);	
	if (pItem)
	{
		pMainMenu->m_pItemInfoMenu->Open(pItem,FORM_NULL);
	}
}

MapCopyMenu::MapCopyMenu()
{
	m_pForm		= pMainInterface->GetDForm("fuben");

	m_pForm->GetComponent("ks")->SetEvent(MCopyStartEvent, this);
	m_pForm->GetComponent("zd")->SetEvent(MCopyTeamEvent, this);
	m_pForm->GetComponent("sy")->SetEvent(AddPowEvent, this);
	m_pForm->GetComponent("gm")->SetEvent(BuyPowEvent, this);


	m_ppCompPage[0]	= m_pForm->GetComponent("page1");
	m_ppCompPage[0]->SetEvent(MCopyPageEvent, this);
	m_ppCompPage[1]	= m_pForm->GetComponent("page2");
	m_ppCompPage[1]->SetEvent(MCopyPageEvent, this);
	m_ppCompPage[1]->SetSprColor(127,127,127);
	m_ppCompPage[2]	= m_pForm->GetComponent("page3");
	m_ppCompPage[2]->SetEvent(MCopyPageEvent, this);
	m_ppCompPage[2]->SetSprColor(127,127,127);

	m_ppCompBack[0] = m_pForm->GetComponent("di1");
	m_ppCompBack[1] = m_pForm->GetComponent("di2");
	m_ppCompBack[2] = m_pForm->GetComponent("di3");

	m_pForm->pCFrame	= new CCtrlFrame(20,240,926,310);

	DComponent*	pCompBase	= m_pForm->GetComponent("mb");
	DComponent*	ppCompPt[10];
	ppCompPt[0]		= m_pForm->GetComponent("gezi1");
	ppCompPt[1]		= m_pForm->GetComponent("icon1");
	ppCompPt[2]		= m_pForm->GetComponent("zhi");
	ppCompPt[3]		= m_pForm->GetComponent("name");
	ppCompPt[3]->SetSprColor(0,255,0);
	ppCompPt[4]		= m_pForm->GetComponent("lv");
	ppCompPt[5]		= m_pForm->GetComponent("tl1");
	ppCompPt[6]		= m_pForm->GetComponent("time");
	ppCompPt[7]		= m_pForm->GetComponent("zi");
	ppCompPt[8]		= m_pForm->GetComponent("dl");
	ppCompPt[9]		= m_pForm->GetComponent("x1");
	m_pForm->pCFrame->InitFParts(pCompBase,ppCompPt,10);
	m_pForm->pCFrame->nSpace	= 20;
	m_pForm->pCFrame->nDisStep	= 108;
	m_pForm->pCFrame->SetEventEx(MCopyFrameEvent,this);

	m_nCurSel		= -1;
	m_bCurType		= 255;
	m_dwWaitTeam	= 0;
	m_pForm->bZorde	= 1;

	m_pForm->GetComponent("icon")->SetEvent(UseStrengthEvent, this);
}

MapCopyMenu::~MapCopyMenu()
{

}

void MapCopyMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;

	m_pForm->GetComponent("tl")->SetProgress(pChar->m_dwPow,50);

	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(30008);
	if (pBase)
	{
		int nCount = pChar->m_pItemCenter->GetItemCountByID(30008);
		char sztxt[32];
		sprintf(sztxt,"item/%d.png",pBase->nSpr);
		m_pForm->GetComponent("icon")->SetViewSpr(sztxt);
		m_pForm->GetComponent("icon")->SetCaptionEx("%d",2,nCount);
	}

	if(m_dwWaitTeam)
	{
		if(xnGetTickCount() - m_dwWaitTeam > 15000)
		{
			pMainMenu->CloseMessageBox();
			pChar->SendCmd(USER,C_MCOPY_TEAM_TIMEUP);
			m_dwWaitTeam	= 0;
		}
		else
		{
			char	szTmp[256];
			sprintf(szTmp,"正在寻找队伍...%d",(15000+m_dwWaitTeam-xnGetTickCount())/1000);
			pMainMenu->PopAskMsgBox(szTmp,fun_cbMCopyTeam,this,2);
		}
	}
}
const int	nColor[3][3] = {
	{0,255,0},
	{21,137,255},
	{212,9,255},
};
void MapCopyMenu::InitMenu(BYTE byType,int nSelID)
{
	if(byType >= 3)
		return;
	if(m_bCurType < 3)
		m_ppCompPage[m_bCurType]->SetSprColor(127,127,127);
	m_bCurType	= byType;
	m_ppCompPage[m_bCurType]->SetSprColor(255,255,255);
	m_nCurSel	= -1;

	for (int k = 0; k < 3; k++)
	{
		m_ppCompBack[k]->SetVisible(false);
	}
	m_ppCompBack[byType]->SetVisible(true);
	CCtrlFrame*	pFrame	= m_pForm->pCFrame;
	pFrame->SetFPCnt(MCopyConfig::GetInstance().m_nTypeCnt[byType]);
	int		nIndex	= 0;
	for (int i = 0; i < MCopyConfig::GetInstance().m_pList->Count; i++)
	{
		MCopyBase*	pMBase	= (MCopyBase*)MCopyConfig::GetInstance().m_pList->Items[i];

		if(pMBase->byType == byType)
		{
			pFrame->SetFPTag(nIndex,pMBase);
			pFrame->GetFPComp(nIndex,"mb")->SetVisible(false);
			pFrame->GetFPComp(nIndex,"name")->SetCaption(pMBase->szName,0);
			pFrame->GetFPComp(nIndex,"name")->SetSprColor(nColor[byType][0],nColor[byType][1],nColor[byType][2]);
			pFrame->GetFPComp(nIndex,"lv")->SetCaptionEx("%d",1,pMBase->byLvNeed);
			pFrame->GetFPComp(nIndex,"tl1")->SetCaptionEx("%d",1,pMBase->byLifeCost);

			char	szTmp[32];
			sprintf(szTmp,"map/%d.png",pMBase->wIcon);
			pFrame->GetFPComp(nIndex,"icon1")->SetViewSpr(szTmp);

			if(pMBase->byLimitTime)
			{
				int nTime = pChar->m_pClientFlag->GetCPlimitF(pMBase->wID);
				int nLmtTime = pMBase->byLimitTime - nTime;
				pFrame->GetFPComp(nIndex,"time")->SetCaptionEx("%d",1,nLmtTime>0?nLmtTime:0);
			}
			else
				pFrame->GetFPComp(nIndex,"time")->SetCaption("无限",1);

			if(pMBase->wID == nSelID)
			{
				SetCurSel(nIndex);
			}

			nIndex++;
		}
	}
}

void MapCopyMenu::SetCurSel( int nSel )
{
	if(m_nCurSel != -1)
	{
		m_pForm->pCFrame->GetFPComp(m_nCurSel,"mb")->SetVisible(false);
	}
	m_nCurSel	= nSel;
	m_pForm->pCFrame->GetFPComp(m_nCurSel,"mb")->SetVisible(true);
}

void MapCopyMenu::RequestTeam()
{
	WORD wID	= CheckCurMCopy();

	if(wID)
	{
		//m_dwWaitTeam	= xnGetTickCount();
		//pMainMenu->PopAskMsgBox("正在寻找队伍...",fun_cbMCopyTeam,this,2);
		BuildPacketEx(pPacket, USER, C_RQ_TEAM_MCOPY,buf, 256);
		SET_DATA(pSend, USER, C_RQ_TEAM_MCOPY, pPacket);
		pSend->wMCID	= wID;
		pChar->SendPacket(pPacket);
	}
}

void MapCopyMenu::RequestEnter()
{
	DWORD	dwLead	= pMainMenu->m_pTeamMenu->GetLeadID();
	if(dwLead && dwLead	!= pChar->m_dwUin)
	{
		pMainMenu->PopTipMsg("组队情况下队长才能选择副本");
		return;
	}

	WORD wID	= CheckCurMCopy();
	if(wID)
	{
		BuildPacketEx(pPacket,USER,C_JOINMAPCOPY,buf,256);
		SET_DATA(pData,USER,C_JOINMAPCOPY,pPacket);
		pData->wMCID	= wID;
		pChar->SendPacket(pPacket);
	}
}

void MapCopyMenu::OpenForm(MCopyBase* pBase)
{
	if(pChar->CheckInFight())
	{
		pMainMenu->PopTipMsg("战斗状态下不可打开该界面!");
		return;
	}
	if(g_pMapCenter->m_nBaseMapIndex == 66)
	{
		pMainMenu->PopTipMsg("该活动地图内，无法打开该界面!");
		return;
	}
	if(pBase)
	{
		InitMenu(pBase->byType,pBase->wID);
		m_pForm->pCFrame->nChgY = m_nCurSel<1? 0:m_nCurSel*108;
	}
	else if(m_bCurType >= 3)
		InitMenu(0);
	else if(m_bCurType >= 0)
		InitMenu(m_bCurType);
	m_pForm->Open(2);
}

void MapCopyMenu::RequestAddPow()
{
	if (pChar->m_dwPow >= 50)
	{
		pMainMenu->PopTipMsg("体力值已满，请稍后使用");
		return;
	}
	if(pChar->m_pItemCenter->GetItemCountByID(30008)==0)
	{
		pMainMenu->PopTipMsg("体力药不足");
		return;
	}
	ItemObj* pObj = pChar->m_pItemCenter->GetItemByTypeID(30008);
	if (pObj&&pObj->pItemBase)
	{
		pChar->m_pItemCenter->UseItem(pObj);
	}

	Update();
}

static void ShowInfoEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nType = (int)param;
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(nType);
	if(pBase != NULL)
	{
		if(pBase->byType == ITEMTYPE_EQUIP)
			pMainMenu->m_pEquipInfoMenu->Open(pBase,FORM_NULL);
		else
			pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_NULL);
	}
}

static void	InputResultPowBuy(int nResult,void * param)
{
	pChar->m_pItemCenter->BuyMallItem(5,(int)param,nResult);
};

void MapCopyMenu::RequestBuyPow()
{
	MallItemCfg* pItem = MallConfig::GetInstance().GetCfg(30008);
	if (pItem)
	{
		int nMaxCount = 1000;
		pMainMenu->m_pGetNumMenu->Open(InputResultPowBuy,nMaxCount,0,(void*)pItem->m_nItemID);

	}
}

WORD MapCopyMenu::CheckCurMCopy()
{
	MCopyBase*	pMBase	= (MCopyBase*)m_pForm->pCFrame->GetFPTag(m_nCurSel);
	if(pMBase == NULL)
	{
		pMainMenu->PopTipMsg("请选择副本");
		return 0;
	}
	if(pChar->m_byLvl < pMBase->byLvNeed)
	{
		pMainMenu->PopTipMsg("等级不足");
		return 0;
	}
	if(pChar->m_dwPow < pMBase->byLifeCost)
	{
		pMainMenu->PopTipMsg("体力不足！您可通过商城“其他”购买获得");
		return 0;
	}
	if(pMBase->CheckTime() == false)
	{
		pMainMenu->PopTipMsg("副本未开放");
		return 0;
	}
	if(g_pMapCenter->m_nMapIndex	>= 1000)
	{
		pMainMenu->PopTipMsg("请先离开副本");
		return 0;
	}

	return pMBase->wID;
}

void ClickDownEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MapCopyDropMenu*	pMenu	= (MapCopyDropMenu*)param;
	if (pMenu)
	{
		pMenu->m_pDForm->pCFrame->nChgY += pMenu->m_pDForm->pCFrame->nDisStep;
	}
}

MapCopyDropMenu::MapCopyDropMenu()
{
	m_pDForm		= pMainInterface->GetDForm("fubendiaoluo");

	DComponent*	pCompBase	= m_pDForm->GetComponent("n1");
	m_pDForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+20)*3);
	DComponent*	ppCompPt[7];
	ppCompPt[0]		= m_pDForm->GetComponent("gezi1");
	ppCompPt[1]		= m_pDForm->GetComponent("icon1");
	ppCompPt[2]		= m_pDForm->GetComponent("name1");
	ppCompPt[3]		= m_pDForm->GetComponent("gezi2");
	ppCompPt[4]		= m_pDForm->GetComponent("icon2");
	ppCompPt[5]		= m_pDForm->GetComponent("name2");
	ppCompPt[6]		= m_pDForm->GetComponent("x1");
	m_pDForm->pCFrame->InitFParts(pCompBase,ppCompPt,7);
	m_pDForm->pCFrame->nSpace	= 20;
	m_pDForm->pCFrame->nDisStep	= pCompBase->nH+20;

	m_pDForm->GetComponent("down")->SetEvent(ClickDownEvent,this);
}

MapCopyDropMenu::~MapCopyDropMenu()
{

}

void MapCopyDropMenu::OpenDForm(int nPos)
{

	BYTE byType =  pMainMenu->m_pMapCopyMenu->m_bCurType;

	for (int i = 0; i < MCopyConfig::GetInstance().m_pList->Count; i++)
	{
		MCopyBase*	pMBase	= (MCopyBase*)MCopyConfig::GetInstance().m_pList->Items[i];

		if(pMBase->byType == byType)
		{
			int nCnt = MCopyConfig::GetInstance().m_nTypeCnt[byType];

	        MCopyBase*	pMBase	= (MCopyBase*)MCopyConfig::GetInstance().m_pList->Items[nCnt*byType + nPos];
	        if (pMBase)
	        {
	        	int nCount = 0;
		        for (int i = 0; i < pMBase->pLItemGet->Count; i++)
		         {
		        	_MCopyDrop*	pMCD	= (_MCopyDrop*)pMBase->pLItemGet->Items[i];
			        if (pMCD->wDis == 1)
				    nCount++;
		         }
		        int nNum = 0;
		        if (nCount%2)
			        nNum = nCount/2 + 1;
		        else
			        nNum = nCount/2;
		        CCtrlFrame*	pFrame	= m_pDForm->pCFrame;
		        pFrame->SetFPCnt(nNum,false);
		        int		nIndex	= 0;
		        int     nifaddframe = 0;
		        for (int i = 0; i < pMBase->pLItemGet->Count; i++)
		        {
		        	_MCopyDrop*	pMCD	= (_MCopyDrop*)pMBase->pLItemGet->Items[i];
		        	if (pMCD->wDis == 1)
		        	{
		        		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pMCD->wItemID);
		        		if (pBase)
		        		{
			        		if (!(nifaddframe%2))
				        	{
				        		pFrame->SetFPTag(nIndex,pBase);
					        	pFrame->GetFPComp(nIndex,"gezi1")->SetVisible(true);
					        	pFrame->GetFPComp(nIndex,"name1")->SetCaption(pBase->szName,1);
					        	pFrame->GetFPComp(nIndex,"name1")->SetSprColor(nItemColor[pBase->nStar][0],nItemColor[pBase->nStar][1],nItemColor[pBase->nStar][2]);

				        		char	szTmp[32];
					        	sprintf(szTmp,"item/%d.png",pBase->nSpr);
					        	pFrame->GetFPComp(nIndex,"icon1")->SetViewSpr(szTmp);
					        	pFrame->GetFPComp(nIndex,"icon1")->SetEvent(ShowInfoEvent,(void*)pBase->nTypeID);
								sprintf(szTmp,"MB_PZ%d.png",pBase->nStar+1);
								pFrame->GetFPComp(nIndex,"icon1")->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);

					        	pFrame->GetFPComp(nIndex,"gezi2")->SetVisible(false);
					        	nifaddframe++;
				        	}else
				        	{
				        		pFrame->SetFPTag(nIndex,pBase);
				        		pFrame->GetFPComp(nIndex,"gezi2")->SetVisible(true);
				        		pFrame->GetFPComp(nIndex,"name2")->SetCaption(pBase->szName,1);
					        	pFrame->GetFPComp(nIndex,"name2")->SetSprColor(nItemColor[pBase->nStar][0],nItemColor[pBase->nStar][1],nItemColor[pBase->nStar][2]);

					        	char	szTmp[32];
				        		sprintf(szTmp,"item/%d.png",pBase->nSpr);
					        	pFrame->GetFPComp(nIndex,"icon2")->SetViewSpr(szTmp);
					        	pFrame->GetFPComp(nIndex,"icon2")->SetEvent(ShowInfoEvent,(void*)pBase->nTypeID);
								sprintf(szTmp,"MB_PZ%d.png",pBase->nStar+1);
								pFrame->GetFPComp(nIndex,"icon2")->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);

					        	pFrame->GetFPComp(nIndex,"x1")->SetVisible(true);

					       	nifaddframe++;
					       	nIndex++;
					        }
				        }
			        }
		        }
		        pMainMenu->m_pMapCopyDropMenu->m_pDForm->Open();
	        }
		}
	}
}