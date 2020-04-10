#include "MainMenu.h"
#include "UserData.h"
#include "GreenModeMenu.h"
#include "MapCopyConfig.h"

void CBFrmGreenMEvent(int nPos,char* szName,void* param)
{
	if(szName[0] != 'x')
		return;
	int	nLv			= szName[strlen(szName)-1] - '0';
	int	nClickLv	= nPos*6+nLv;

	pMainMenu->m_pGreenModeMenu->OpenStart(nClickLv);
}
void StartMCopyEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pGreenModeMenu->StartMCopy((int)param);
}
void OverMCopyEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pGreenModeMenu->OverMCopy((int)param);
}

void ChickGreenAwardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pGreenAwardMenu->Open();
}

GreenModeMenu::GreenModeMenu()
{
	m_pForm		= pMainInterface->GetDForm("meng");

	m_pForm->pCFrame	= new CCtrlFrame(20,120,915,400);

	DComponent*	pCompBase	= m_pForm->GetComponent("kuang");
	DComponent*	ppComp[80];

	char	szTmp[32];
	int		nInxPos	= 0;
	for (int i = 0; i < 6; i++)
	{
		sprintf(szTmp,"taizi%d",i+1);
		ppComp[nInxPos++]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"gk%d",i+1);
		ppComp[nInxPos++]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"q%d",i+1);
		ppComp[nInxPos++]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"x%d",i+1);
		ppComp[nInxPos++]	= m_pForm->GetComponent(szTmp);
		
		for (int j = 0; j < 5; j++)
		{
			sprintf(szTmp,"t%d%d",i+1,j+1);
			DComponent* pCmp	= m_pForm->GetComponent(szTmp);
			if(pCmp)
				ppComp[nInxPos++]	= pCmp;
		}
	}

	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,nInxPos);
	m_pForm->pCFrame->nSpace	= 55;
	m_pForm->pCFrame->nDisStep	= 915;
	m_pForm->pCFrame->SetEventEx(CBFrmGreenMEvent,this);
	m_pForm->pCFrame->nZorder	= -1;
	m_pForm->pCFrame->SetFPCnt(0,true);

	m_nReachStep	= 0;
	m_pForm->GetComponent("left")->SetEvent(MoveFrmLeft,m_pForm->pCFrame);
	m_pForm->GetComponent("right")->SetEvent(MoveFrmRight,m_pForm->pCFrame);
	m_pForm->GetComponent("left")->m_nOrder	= 5;
	m_pForm->GetComponent("right")->m_nOrder	= 5;

	m_pForm->GetComponent("jl")->SetEvent(ChickGreenAwardEvent);

	m_pFormStart	= pMainInterface->GetDForm("meng2");
	pCompDelay		= NULL;
	m_dwChgTick     = 0;
	m_bChg          = false;
}

GreenModeMenu::~GreenModeMenu()
{

}

void GreenModeMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;

	int	nReachLv	= pChar->m_pClientFlag->GetFlag(23)+1;

	int	nGateCnt	= (nReachLv-1)/6+1;
	bool	bReset	= false;
	if(nGateCnt != m_pForm->pCFrame->GetFPCnt())
	{
		if (m_pForm->pCFrame->GetFPCnt())
		{
			m_dwChgTick = xnGetTickCount();
			m_bChg      = true;
		}
		m_pForm->pCFrame->SetFPCnt(nGateCnt,true);
		bReset		= true;
	}

	char	szTmp[32];
	if(bReset)
	{
		for (int i = 0; i < nGateCnt; i++)
		{
			for (int j = 0; j < 6; j++)
			{
				int	nLvPos	= 6*i+j+1;
				sprintf(szTmp,"gk%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szTmp)->SetCaptionEx("第%d关",1,nLvPos);
				if(nLvPos > nReachLv)
				{
					sprintf(szTmp,"taizi%d",j+1);
					m_pForm->pCFrame->GetFPComp(i,szTmp)->SetVisible(false);
					sprintf(szTmp,"gk%d",j+1);
					m_pForm->pCFrame->GetFPComp(i,szTmp)->SetVisible(false);
					sprintf(szTmp,"x%d",j+1);
					m_pForm->pCFrame->GetFPComp(i,szTmp)->SetVisible(false);
					sprintf(szTmp,"q%d",j+1);
					m_pForm->pCFrame->GetFPComp(i,szTmp)->SetVisible(false);
					for (int n = 0; n < 5; n++)
					{
						sprintf(szTmp,"t%d%d",j+1,n+1);
						DComponent* pCmp	= m_pForm->pCFrame->GetFPComp(i,szTmp);
						if(pCmp)
							pCmp->SetVisible(false);
					}
				}
				else
				{
					sprintf(szTmp,"q%d",j+1);
					m_pForm->pCFrame->GetFPComp(i,szTmp)->byRunAction	= 50;
				}
			}
		}
	}
	else
	{
		if(nReachLv > m_nReachStep)
		{//破关后的效果和控件动态出现
			if(m_nReachStep != 0)
			{
				int	nGPos	= (nReachLv-1)%6;
				int	nGate	= nGateCnt-1;
				int		nActInx	= 0;
				DComponent* pTmpComp	= NULL;
				for (int j = 0; j < 5; j++)
				{
					sprintf(szTmp,"t%d%d",nGPos+1,j+1);
					pTmpComp	= m_pForm->pCFrame->GetFPComp(nGate,szTmp);
					if(pTmpComp)
					{
						pTmpComp->SetVisible(true);
						pTmpComp->byRunAction	= 40+nActInx++;
					}
				}
				sprintf(szTmp,"taizi%d",nGPos+1);
				pTmpComp	= m_pForm->pCFrame->GetFPComp(nGate,szTmp);
				pTmpComp->SetVisible(true);
				pTmpComp->byRunAction	= 40+nActInx;
				sprintf(szTmp,"gk%d",nGPos+1);
				pTmpComp	= m_pForm->pCFrame->GetFPComp(nGate,szTmp);
				pTmpComp->SetVisible(true);
				pTmpComp->byRunAction	= 40+nActInx;
				sprintf(szTmp,"x%d",nGPos+1);
				pTmpComp	= m_pForm->pCFrame->GetFPComp(nGate,szTmp);
				pTmpComp->SetVisible(true);
				pTmpComp->byRunAction	= 40+nActInx;
				sprintf(szTmp,"q%d",nGPos+1);
				pTmpComp	= m_pForm->pCFrame->GetFPComp(nGate,szTmp);
				pTmpComp->SetVisible(true);
				pTmpComp->byRunAction	= 40+nActInx;
				pCompDelay	= pTmpComp;
			}
			m_nReachStep	= nReachLv;
		}
	}
	if (m_bChg&&(xnGetTickCount()-m_dwChgTick > 500))
	{
		m_pForm->pCFrame->nChgX = nGateCnt*m_pForm->pCFrame->nDisStep;
		m_bChg = false;
	}

	int nCurPage	= -m_pForm->pCFrame->nOffChgPos/910;
	if(nCurPage >=0 && nCurPage < m_pForm->pCFrame->GetFPCnt())
	{
		for (int i = 0; i < 6; i++)
		{
			sprintf(szTmp,"q%d",i+1);
			DComponent* pTmpComp	= m_pForm->pCFrame->GetFPComp(nCurPage,szTmp);
			if(pTmpComp->byRunAction == 0)
				pTmpComp->byRunAction	= 50;
		}
	}

	m_pForm->GetComponent("left")->SetVisible(nCurPage!=0);
	m_pForm->GetComponent("right")->SetVisible(nCurPage!=nGateCnt-1);
}

bool GreenModeMenu::CheckShine()
{
	if (pChar->m_byLvl < 36)
		return false;

	for (int i = 0; i < pChar->m_pClientFlag->GetFlag(23); i++)
	{
		int	nTime	= pChar->m_pClientFlag->GetCPlimitF(1001+i);
		if (nTime < 100)
			return true;
	}

	return false;
}

void GreenModeMenu::OpenStart( int nGLv )
{
	if (pChar->m_byLvl < 36)
		return;
	int	nTime	= pChar->m_pClientFlag->GetCPlimitF(1000+nGLv);
	if(nTime >= 100)
	{
		pMainMenu->PopTipMsg("该关卡已挑战成功");
		return;
	}
	if(pMainMenu->m_pTeamMenu->GetTeamedCount() != 0)
	{
		pMainMenu->PopTipMsg("组队状态下不可挑战");
		return;
	}

	m_pFormStart->GetComponent("jr")->SetEvent(StartMCopyEvent,(void*)nGLv);
	m_pFormStart->GetComponent("sdC0")->SetEvent(OverMCopyEvent,(void*)nGLv);
	
	MCopyBase* pMCBase	= MCopyConfig::GetInstance().GetMCopy(1000+nGLv);
	if(!pMCBase)
		return;
	m_pFormStart->GetComponent("icon3")->SetVisible(nGLv%6==0);
	m_pFormStart->GetComponent("name")->SetCaption(pMCBase->szName,1);
	m_pFormStart->GetComponent("money")->SetCaptionEx("%d",1,pMCBase->nGold);
	m_pFormStart->GetComponent("exp")->SetCaptionEx("%d",1,pMCBase->nExp);

	m_pFormStart->GetComponent("sdC0")->SetVisible(m_nReachStep > nGLv);


	if(nTime == 0)
	{
		m_pFormStart->GetComponent("ci")->SetCaption("免费",1);
	}
	else if(nTime == 1 && pChar->m_byVipLvl >=3)
	{
		m_pFormStart->GetComponent("ci")->SetCaption("VIP免费",1);
	}
	else
		m_pFormStart->GetComponent("ci")->SetCaption("20",1);

	m_pFormStart->Open(2);
}
void fun_CBStartGreen(void * pParam,int nVal)
{
	if(nVal == 1)
	{
		BuildPacketEx(pPacket,USER,C_JOINMAPCOPY_SP,buf,256);
		SET_DATA(pData,USER,C_JOINMAPCOPY_SP,pPacket);
		pData->wMCID	= (int)pParam;
		pChar->SendPacket(pPacket);
	}
}
void GreenModeMenu::StartMCopy( int nGLv )
{
	m_pFormStart->Close();
	if(pChar->m_pClientFlag->GetCPlimitF(1000+nGLv) >= 100)
		return;
	if (pChar->m_byLvl < 36)
		return;

	if(strcmp(m_pFormStart->GetComponent("ci")->szCaption,"20") == 0)
	{//要钱
		pMainMenu->PopAskMsgBox("是否花费20钻石挑战?",fun_CBStartGreen,(void*)(1000+nGLv));
	}
	else
	{
		BuildPacketEx(pPacket,USER,C_JOINMAPCOPY_SP,buf,256);
		SET_DATA(pData,USER,C_JOINMAPCOPY_SP,pPacket);
		pData->wMCID	= 1000+nGLv;
		pChar->SendPacket(pPacket);
	}

}

void GreenModeMenu::OverMCopy( int nGLv )
{
	m_pFormStart->Close();
	if(pChar->m_pClientFlag->GetCPlimitF(1000+nGLv) >= 100)
		return;
	if(m_nReachStep > nGLv)
	{
		BuildPacketEx(pPacket,USER,C_OVER_GREENMCOPY,buf,256);
		SET_DATA(pData,USER,C_OVER_GREENMCOPY,pPacket);
		pData->wMCopyID	= 1000+nGLv;
		pChar->SendPacket(pPacket);
	}
}

void	ClickGreenShowEvent(InterfaceEvent* pEvent,void* param)
{
	if(param)
	{
		static int	nClickY	= 0;
		GreenAwardMenu* pMenu = (GreenAwardMenu*)param;
		switch(pEvent->event)
		{
		case INTERFACE_EVENT_DOWN:
			{
				nClickY = pEvent->y;
			}
			break;
		case INTERFACE_EVENT_MOVE:
			{
				pMenu->MoveXY(pEvent->pComponent->nY + pEvent->y - nClickY);
				nClickY		= pEvent->y;
			}
			break;
		case INTERFACE_EVENT_UP:
			{
				pMenu->MoveXY(pEvent->pComponent->nY + pEvent->y - nClickY);
				nClickY		= 0;
			}
			break;
		}
	}
}

GreenAwardMenu::GreenAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("meng5");
	m_pShowComp = m_pForm->GetComponent("z");
	m_pShowComp->byType = COMPONENT_MAPGUIDE;
	m_pShowComp->SetViewSpr("ui/MB_MENG1.png");
	m_pShowComp->SetEvent(ClickGreenShowEvent,this);
	m_pShowComp->bAlignX	= 1;
	m_pShowComp->bAlignY	= 0;
	m_nBaseX = m_pShowComp->nX;
	m_nBaseY = m_pShowComp->nY;

}

GreenAwardMenu::~GreenAwardMenu()
{

}

void GreenAwardMenu::Open()
{
	m_pForm->Open();
	m_pShowComp->SetPos(m_nBaseX, m_nBaseY);
}

void GreenAwardMenu::MoveXY(int	nMoveY)
{
	if(nMoveY > m_nBaseY)
		nMoveY = m_nBaseY;
	if(nMoveY < (m_nBaseY - m_pShowComp->nSprPixHeight + m_pShowComp->nH))
		nMoveY = m_nBaseY - m_pShowComp->nSprPixHeight + m_pShowComp->nH;
	
	m_pShowComp->SetPos(m_nBaseX, nMoveY);

}

