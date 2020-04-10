#include "MainMenu.h"
#include "MCAwardMenu.h"
#include "stdio.h"
#include "MapCopyConfig.h"
#include "ItemConfig.h"
#include "UserData.h"
#include "VipConfig.h"

void MCAOpenCardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MCAwardMenu*	pMenu	= (MCAwardMenu*)param;
	if (strncmp(pMenu->m_pForm->szName,"meng1",5) == 0)
	{
		if (pMenu->m_dwCloseGMWTick)
		{
			pMainMenu->PopTipMsg("你已经选择过了翻牌奖励！");
			return;
		}else
			pMenu->m_dwCloseGMWTick = xnGetTickCount();
	}

	int nPos	= 0;
	sscanf(pEvent->pComponent->szName,"g%d",&nPos);
	pMenu->RequestOpenCard(nPos-1);
}
void MCAOpenAllCardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MCAwardMenu*	pMenu	= (MCAwardMenu*)param;
	pMainMenu->ShowNetWaiting();
	pMenu->RequestOpenAllCard();
}
void fun_cbMCALeave(void * pParam,int nVal)
{
	if(nVal == 1)
	{
		MCAwardMenu* pMenu	= (MCAwardMenu*)pParam;
		pMenu->m_pForm->Close();
		pChar->SendCmd(USER, C_LEAVEMCOPY);
	}
}

void MCALeaveMCopy(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->PopAskMsgBox("是否确定离开副本?",fun_cbMCALeave,param);
}

MCAwardMenu::MCAwardMenu()
{
	m_pForm		= pMainInterface->GetDForm("fuben2");

	char	szTmp[64];
	for (int i = 0; i < 8; i++)
	{
		sprintf(szTmp,"g%d",i+1);
		m_ppCompCard[i]		= m_pForm->GetComponent(szTmp);
		m_ppCompCard[i]->SetEvent(MCAOpenCardEvent,this);
		sprintf(szTmp,"a%d",i+1);
		m_ppCompRtl[i]		= m_pForm->GetComponent(szTmp);

		sprintf(szTmp,"d%d",i+1);
		m_ppCompIco[i]		= m_pForm->GetComponent(szTmp);
		//m_ppCompIco[i]->SetVisible(false);
		sprintf(szTmp,"n%d",i+1);
		m_ppCompName[i]		= m_pForm->GetComponent(szTmp);
		//m_ppCompName[i]->SetVisible(false);
	}
	m_nRunActionPos	= -1;
	m_dwOpenTick	= 0;
	m_dwTickFormOpen	= 0;
	m_bRecvItem		= false;
	m_pList = xnList::Create();

	m_pCompTime	= m_pForm->GetComponent("time");
	m_pCompTime->m_nFontSize	= 22;
	m_pCompTime->SetSprColor(0,222,0);

	m_pForm->GetComponent("ok")->SetEvent(MCALeaveMCopy,this);
	m_pForm->GetComponent("close")->SetEvent(MCALeaveMCopy,this);
	m_pForm->GetComponent("quan")->SetEvent(MCAOpenAllCardEvent,this);
	m_bIsGreenDream	= false;
	m_bDealyLeave	= false;

	m_dwCloseGMWTick = 0;
}

void MCAwardMenu::Init( bool isGDream )
{
	if(isGDream)
		m_pForm		= pMainInterface->GetDForm("meng1");
	else
		m_pForm		= pMainInterface->GetDForm("fuben2");
	m_bIsGreenDream	= isGDream;
	char	szTmp[64];
	for (int i = 0; i < 8; i++)
	{
		sprintf(szTmp,"g%d",i+1);
		m_ppCompCard[i]		= m_pForm->GetComponent(szTmp);
		m_ppCompCard[i]->SetEvent(MCAOpenCardEvent,this);
		sprintf(szTmp,"a%d",i+1);
		m_ppCompRtl[i]		= m_pForm->GetComponent(szTmp);

		sprintf(szTmp,"d%d",i+1);
		m_ppCompIco[i]		= m_pForm->GetComponent(szTmp);
		//m_ppCompIco[i]->SetVisible(false);
		sprintf(szTmp,"n%d",i+1);
		m_ppCompName[i]		= m_pForm->GetComponent(szTmp);
		//m_ppCompName[i]->SetVisible(false);
	}
	m_pCompTime	= m_pForm->GetComponent("time");
	m_pCompTime->m_nFontSize	= 22;
	m_pCompTime->SetSprColor(0,222,0);
	m_pForm->GetComponent("ok")->SetEvent(MCALeaveMCopy,this);
	m_pForm->GetComponent("close")->SetEvent(MCALeaveMCopy,this);
	m_nRunActionPos	= -1;
	m_dwOpenTick	= 0;
	m_dwTickFormOpen	= 0;
}

MCAwardMenu::~MCAwardMenu()
{
	while(m_pList->Count)
		m_pList->Delete(0);
	m_pList->Free();
}

void MCAwardMenu::Update()
{
	if(m_pForm->bOpened == false)
	{
		if(m_dwTickFormOpen && xnGetTickCount() - m_dwTickFormOpen > 3000)
		{
			if(m_bDealyLeave)
				pChar->SendCmd(USER,C_LEAVEMCOPY);
			else
			{
				m_pForm->Open();
			}
			m_bDealyLeave		= false;
			m_dwTickFormOpen	= 0;
		}
		return;
	}
	m_dwTickFormOpen	= 0;
	if(m_nRunActionPos != -1)
	{
		if(m_nRunActionPos == 99)
		{
			int nTempCnt = m_nOpenAllCnt;
			for (int i = 0; i < 8; i++)
			{
				if(!m_bifOpen[i]&&nTempCnt)
				{
					bool	bStep	= false;
					if(xnGetTickCount() - m_dwChgSpr >= 500 && m_ppCompCard[i]->bVisable)
					{
						m_ppCompCard[i]->SetVisible(false);
						//m_ppCompRtl[i]->byRunAction	= 89;
						//bStep	= true;
						
					}
					else if(xnGetTickCount() - m_dwChgSpr >= 800 /*&& m_ppCompIco[i]->bVisable == false*/)
					{
						//m_ppCompIco[i]->SetVisible(true);
						//m_ppCompName[i]->SetVisible(true);
						bStep	= true;
					}

					nTempCnt--;
					if(bStep)
					{
						m_bifOpen[i] = true;
						m_nOpenAllCnt--;
					}
					//if(bStep)
					//	m_bStepCard	++;
					//
					//if(m_bStepCard >= 3)
					//{
					//	memset(m_bifOpen,1,8);
					//	m_nRunActionPos	= -1;
					//}
				}
			}
			if (m_nOpenAllCnt == 0)
			{
				m_nRunActionPos = -1;
			}
		}
		else
		{
			if(m_ppCompCard[m_nRunActionPos]->bVisable && xnGetTickCount() - m_dwChgSpr >= 500)
			{
				m_ppCompCard[m_nRunActionPos]->SetVisible(false);
				m_ppCompIco[m_nRunActionPos]->SetVisible(true);
				m_ppCompName[m_nRunActionPos]->SetVisible(true);
				//m_ppCompRtl[m_nRunActionPos]->byRunAction	= 89;
			}
			else if(xnGetTickCount() - m_dwChgSpr >= 800 /*&& m_ppCompIco[m_nRunActionPos]->bVisable == false*/)
			{
				if( m_bRecvItem)
					m_nRunActionPos	= -1;
			}
		}

	}

	if (m_dwCloseGMWTick&&(xnGetTickCount() > m_dwCloseGMWTick + 1500))
	{
		m_dwCloseGMWTick = 0;
		m_pForm->Close();
		pChar->SendCmd(USER, C_LEAVEMCOPY);
	}

	m_pForm->GetComponent("total")->SetCaptionEx("%d",1,pChar->m_dwRmb);

	DWORD	dwRmTick	= xnGetTickCount() - m_dwOpenTick;
	int		nRmSec	= 120 - dwRmTick/1000;
	if(nRmSec >=0)
		m_pCompTime->SetCaptionEx("%d",1,nRmSec);
	else
	{
		m_pForm->Close();
		pChar->SendCmd(USER, C_LEAVEMCOPY);
	}
}

void fun_cbOpenCardEvent(void * pParam,int nVal)
{
	MCAwardMenu* pMenu	= (MCAwardMenu*)pMainMenu->m_pMCAwardMenu;
	if(nVal == 1)
	{
		pChar->SendCmd(USER,C_OPENCARD);
		pMenu->m_nRunActionPos	= (int)pParam;
		pMenu->m_ppCompCard[pMenu->m_nRunActionPos]->byRunAction	= 88;
		pMenu->m_ppCompRtl[pMenu->m_nRunActionPos]->byRunAction	= 89;
		pMenu->m_ppCompIco[pMenu->m_nRunActionPos]->byRunAction	= 89;
		pMenu->m_ppCompName[pMenu->m_nRunActionPos]->byRunAction	= 89;
		pMenu->m_bRecvItem		= false;
		pMenu->m_dwChgSpr	= xnGetTickCount();
		pMenu->m_bifOpen[(int)pParam] = true;
	}
	else
		pMenu->m_nRunActionPos	= -1;
}

void RechargeReturn3Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void MCAwardMenu::RequestOpenCard( int nPos )
{
	if(m_nRunActionPos != -1)
		return;
	if(nPos >= 8)
		return;

	if(m_bIsGreenDream && m_nOpenCardCnt)
		return;

	int	nCostRmb	= 0;
	VipInfoObj* pVip = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);
	if(pVip)
	{
		nCostRmb	= (m_nOpenCardCnt-pVip->m_nCardtimes)*5;
	}
	else
		nCostRmb	= m_nOpenCardCnt*5;

	if(nCostRmb > 0)
	{
		if(nCostRmb >= pChar->m_dwRmb)
		{
			pMainMenu->PopAskMsgBox("钻石不足，是否前往充值？",RechargeReturn3Event,(void*)0,3);
			return;
		}
		char	szAsk[64];
		sprintf(szAsk,"是否花费 %d 钻石继续翻牌？",nCostRmb);
		pMainMenu->PopAskMsgBox(szAsk,fun_cbOpenCardEvent,(void*)nPos);
	}
	else
	{
		m_bifOpen[nPos] = true;
		pChar->SendCmd(USER,C_OPENCARD);
		m_ppCompCard[nPos]->byRunAction	= 88;
		m_ppCompRtl[nPos]->byRunAction	= 89;
		m_ppCompIco[nPos]->byRunAction	= 89;
		m_ppCompName[nPos]->byRunAction	= 89;
		m_nRunActionPos	= nPos;
		m_bRecvItem		= false;
		m_dwChgSpr	= xnGetTickCount();
	}

}
static double nRank[] = {0.75,1,1.5,2};
void MCAwardMenu::OpenForm( WORD wMCopyID,BYTE byRankVal)
{
	MCopyBase* pBase	= MCopyConfig::GetInstance().GetMCopy(wMCopyID);
	if(!pBase)
		return;
	if (wMCopyID == 404 || wMCopyID == 410 ||  wMCopyID == 412)
	{
		m_pForm->Close();
		pMainMenu->PlayEffect(207);
		m_dwTickFormOpen	= xnGetTickCount();
		m_bDealyLeave	= true;
		return;
	}else if (pBase->byType == 3)
	{
		m_pForm->Close();
		m_dwTickFormOpen	= xnGetTickCount();
		m_bDealyLeave	= true;
		return;
	}

	if(pBase->byType == 5)
		Init(true);
	else
		Init(false);

	if(pBase->pLItemGet->Count == 0)
	{
		m_pForm->Close();
		pMainMenu->PlayEffect(207);
		m_dwTickFormOpen	= xnGetTickCount();
		m_bDealyLeave	= true;
		return;
	}

	m_pForm->GetComponent("need")->SetCaption("0",1);
	int nGold = pBase->nGold*nRank[byRankVal];
	int nExp = pBase->nExp*nRank[byRankVal];
	m_pForm->GetComponent("money")->SetCaptionEx("%d",1,nGold);
	m_pForm->GetComponent("exp")->SetCaptionEx("%d",1,nExp);
	char	szIcon[32];
	sprintf(szIcon,"MB_FB_RLT%d.png",byRankVal);
	m_pForm->GetComponent("assess")->SetViewSpr(szIcon);
	m_dwChgSpr = 0;
	for (int i = 0; i < 8; i++)
	{
		m_bifOpen[i] = false;
		m_ppCompCard[i]->SetVisible(true);
		//m_ppCompIco[i]->SetVisible(false);
		m_ppCompIco[i]->SetViewSpr("");
		//m_ppCompName[i]->SetVisible(false);
		m_ppCompName[i]->SetCaption("",1);
	}
	m_nOpenAllNeed = 0;
	m_nOpenAllCnt = 0;
	m_nOpenCardCnt	= 0;
	m_nRunActionPos	= -1;
	m_dwOpenTick	= xnGetTickCount();
	while(m_pList->Count)
		m_pList->Delete(0);

	//播放光效, 延迟1秒
	pMainMenu->PlayEffect(207);
	m_dwTickFormOpen	= xnGetTickCount();
}


void MCAwardMenu::SetCardItems( DWORD wItems[] ,int nCnt)
{
	if(m_nRunActionPos == 99)
	{
		m_nOpenCardCnt	+= nCnt;

		for (int i = 0; i < 8; i++)
		{
			if(m_bifOpen[i]	== false && nCnt)
			{
				ItemExBase*	pIB	= ItemConfig::GetInstance().GetItemBase(wItems[nCnt-1]);
				if(pIB)
				{
					char	szTmp[32];
					sprintf(szTmp,"item/%d.png",pIB->nSpr);
					m_ppCompIco[i]->SetViewSpr(szTmp);
					m_ppCompName[i]->SetCaption(pIB->szName,1);
					m_ppCompName[i]->SetSprColor(nItemColor[pIB->nStar][0],nItemColor[pIB->nStar][1],nItemColor[pIB->nStar][2]);
				}
				nCnt--;
			}
		}
	}

	if(m_nOpenAllNeed > 0)
		m_pForm->GetComponent("need")->SetCaptionEx("%d",1,m_nOpenAllNeed);
	else
		m_pForm->GetComponent("need")->SetCaption("0",1);
}

void MCAwardMenu::SetCardItem( DWORD wItem )
{
	if(m_nRunActionPos == 99)
		return;
	
	if(m_nRunActionPos != -1)
	{
		
		{
			m_nOpenCardCnt	++;
			ItemExBase*	pIB	= ItemConfig::GetInstance().GetItemBase(wItem);

			if(pIB)
			{
				char	szTmp[32];
				sprintf(szTmp,"item/%d.png",pIB->nSpr);
				m_ppCompIco[m_nRunActionPos]->SetViewSpr(szTmp);
				m_ppCompName[m_nRunActionPos]->SetCaption(pIB->szName,1);
				m_ppCompName[m_nRunActionPos]->SetSprColor(nItemColor[pIB->nStar][0],nItemColor[pIB->nStar][1],nItemColor[pIB->nStar][2]);
			}
		}

		m_bRecvItem	= true;
	}
	int	nCostRmb	= 0;
	VipInfoObj* pVip = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);
	if(pVip)
	{
		nCostRmb	= (m_nOpenCardCnt-pVip->m_nCardtimes)*5;
	}
	else
		nCostRmb	= (m_nOpenCardCnt)*5;
	if(nCostRmb > 0)
		m_pForm->GetComponent("need")->SetCaptionEx("%d",1,nCostRmb);
	else
		m_pForm->GetComponent("need")->SetCaption("0",1);
}

int MCAwardMenu::GetNextNeedRmb()
{
	int	nCostRmb	= 0;
	VipInfoObj* pVip = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);
	if(pVip)
	{
		nCostRmb	= (m_nOpenCardCnt-pVip->m_nCardtimes)*5;
	}
	else
		nCostRmb	= (m_nOpenCardCnt)*5;
	if (nCostRmb > 0)
		return nCostRmb;
	else
		return 0;
}

void MCAwardMenu::GetAllOpenParam(int* nRmb, int* nCnt)
{
	int	nCostRmb	= 0;
	int	nReturnCnt = 0;
	int nTempCardCnt = m_nOpenCardCnt;
	int TempCostRmb = 0;
	int TempCnt = 0;
	for (int i = 0; i < 8; i++)
	{
		if (!m_bifOpen[i])
		{
			VipInfoObj* pVip = VipConfig::GetInstance().GetObjByLv(pChar->m_byVipLvl);
			if(pVip)
			{
				TempCostRmb += nTempCardCnt-pVip->m_nCardtimes<0?0:(nTempCardCnt-pVip->m_nCardtimes)*5;
				nTempCardCnt++;
				TempCnt++;
			}
			else
			{
				TempCostRmb += (nTempCardCnt)*5;
				nTempCardCnt++;
				TempCnt++;
			}
			if (TempCostRmb>pChar->m_dwRmb)
			{
				*nRmb = nCostRmb;
				*nCnt = nReturnCnt;
				return;
			}
			else
			{
				nCostRmb = TempCostRmb;
				nReturnCnt = TempCnt;
			}
		}
	}
	*nRmb = nCostRmb;
	*nCnt = nReturnCnt;
	return;
}

void MCAwardMenu::RequestOpenAllCard()
{
	if(m_bIsGreenDream)
		return;
	if (GetNextNeedRmb()>pChar->m_dwRmb)
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值？",RechargeReturn3Event,(void*)0,3);
		return;
	}
	int nNeedRmb = 0;
	int nOpenCnt = 0;
	GetAllOpenParam(&nNeedRmb,&nOpenCnt);
	if (!nOpenCnt)
	{
		pMainMenu->PopTipMsg("无可翻卡牌！");
		return;
	}
	BuildPacketEx(pPacket, USER, C_RQ_OPENALL,buf,256);
	SET_DATA(pSend, USER,C_RQ_OPENALL,pPacket);
	pSend->nWRmb = nNeedRmb;
	pSend->nWCnt = nOpenCnt;
	pChar->SendPacket(pPacket);

	m_nOpenAllNeed = nNeedRmb;
	m_nOpenAllCnt = nOpenCnt;
	for (int i = 0; i < 8; i++)
	{
		if(m_bifOpen[i] == false&&nOpenCnt)
		{
			m_ppCompCard[i]->byRunAction	= 88;
			m_ppCompRtl[i]->byRunAction	= 89;
			m_ppCompIco[i]->byRunAction	= 89;
			m_ppCompName[i]->byRunAction	= 89;
			nOpenCnt--;
		}
	}
	m_nRunActionPos	= 99;
	m_bRecvItem		= false;
	m_dwChgSpr	= xnGetTickCount();
}


