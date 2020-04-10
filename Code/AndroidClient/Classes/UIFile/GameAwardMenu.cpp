#include "UserData.h"
#include "GameAwardMenu.h"
#include "GameAwardCfg.h"
#include "ItemConfig.h"
#include "MainMenu.h"
#include "ItemData.h"
#include "ios_user_packet.h"

void ShowSlaveInfoEvent(InterfaceEvent* pEvent, void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return ;

	pMainMenu->m_pSlaveInfoMenu->OpenByID((int)param);
}

void ClickGetOnlineButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return ;
	int nPos = (int)param;
	pChar->m_pOnlineAwardCenter->RequestForGift(nPos);
}

OnlineAwardMenu::OnlineAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("jiangli1");
	char szName[16];
	for(int i = 0; i < 5; i ++)
	{
		sprintf(szName,"get%d",i+1);
		m_pForm->GetComponent(szName)->SetEvent(ClickGetOnlineButton,(void*)i);
	}
}

OnlineAwardMenu::~OnlineAwardMenu()
{

}

void OnlineAwardMenu::Open(int nT)
{
	m_pForm->Open(nT);
	xnList* pList = GameAwardConfig::GetInstance().m_pOnlineList;
	for(int i = 0; i < 5; i ++)
	{
		char szName[16];
		char szIcon[32];
		sprintf(szName,"icon%d",i+1);
		DComponent* pComp = m_pForm->GetComponent(szName);
		if(i < pList->Count)
		{
			OnlineAward* pAward = (OnlineAward*)pList->Items[i];
			if(pAward->xAward.nID < 1000)
			{
				if(pAward->xAward.nID == 555)
					sprintf(szIcon,"MB_ZI_EXP1.png");
				else if(pAward->xAward.nID == 666)
					sprintf(szIcon,"MB_MONEY1.png");
				else if(pAward->xAward.nID == 888)
					sprintf(szIcon,"MB_ZUANSHI1.png");
				else if(pAward->xAward.nID == 333)
					sprintf(szIcon,"MB_BANGZUAN1.png");
				pComp->SetViewSpr(szIcon);
				pComp->SetCaptionEx("%d",2,pAward->xAward.nNum);
			}
			else
			{
				ShowItemInfoOnComp(pComp,pAward->xAward.nID,pAward->xAward.nNum);
			}

		}
		else
		{
			pComp->SetViewSpr("");
			pComp->SetCaption("",1);
		}
	}
}

void OnlineAwardMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		DComponent* pTimeComp = m_pForm->GetComponent("time");
		pTimeComp->SetCaptionEx("%02d:%02d",1,pChar->m_pOnlineAwardCenter->m_dwLeftsec/60,pChar->m_pOnlineAwardCenter->m_dwLeftsec%60);
		DComponent* pCompGet = NULL;
		DComponent* pCompRlt = NULL;
		char szName[32];
		for(int i = 0; i < 5; i ++)
		{
			sprintf(szName,"get%d",i+1);
			pCompGet = m_pForm->GetComponent(szName);
			sprintf(szName,"yi%d",i+1);
			pCompRlt = m_pForm->GetComponent(szName);
			if(pChar->m_pOnlineAwardCenter->m_byFlag > i)
			{
				pCompGet->SetVisible(false);
				pCompRlt->SetVisible(true);
			}
			else if(pChar->m_pOnlineAwardCenter->m_byFlag == i && pChar->m_pOnlineAwardCenter->m_dwLeftsec == 0)
			{
				pCompGet->SetVisible(true);
				pCompGet->bClickLocked = false;
				pCompGet->SetViewSpr("BT_LINGQU234.png");
				pCompRlt->SetVisible(false);
			}
			else
			{
				pCompGet->SetVisible(true);
				pCompGet->bClickLocked = true;
				pCompGet->SetViewSpr("BT_LINGQU234_H.png");
				pCompRlt->SetVisible(false);
			}
		}
	}
}

void ClickMounthSignButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pChar->m_pMounthAwardCenter->RequestSign(true);
}
void RechargeReturn7Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}
void ClickMounthReSignButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(pChar->m_dwRmb < 5)
	{
		pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn7Event,(void*)0,3);
		return;
	}
	pChar->m_pMounthAwardCenter->RequestSign(false);
}

void ClickMounthGetButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	pChar->m_pMounthAwardCenter->RequestGift(nPos);
}

MounthAwardMenu::MounthAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("jiangli2");
	m_pForm->GetComponent("qd")->SetEvent(ClickMounthSignButton,this);
	m_pForm->GetComponent("bu")->SetEvent(ClickMounthReSignButton,this);
	char szName[16];
	for(int i = 0; i < 5; i ++)
	{
		sprintf(szName,"get%d",i+1);
		m_pForm->GetComponent(szName)->SetEvent(ClickMounthGetButton,(void*)i);
	}
}

MounthAwardMenu::~MounthAwardMenu()
{

}

void MounthAwardMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		char szIcon[32];
		char szName[32];
		sprintf(szIcon,"%04d%02d.png",pChar->m_pMounthAwardCenter->m_nYear,pChar->m_pMounthAwardCenter->m_nMounth);
		m_pForm->GetComponent("ditu")->SetViewSpr(szIcon);
		m_pForm->GetComponent("cishu")->SetCaptionEx("%d",1,pChar->m_pMounthAwardCenter->m_nCount);
		for(int i = 0; i < 42; i ++)
		{
			sprintf(szName,"q%d",i+1);
			DComponent* pComp = m_pForm->GetComponent(szName);
			if(i < pChar->m_pMounthAwardCenter->m_nStart || i > (pChar->m_pMounthAwardCenter->m_nStart+30))
				pComp->SetViewSpr("");
			else
			{
				if(pChar->m_pMounthAwardCenter->m_szSignFlag[i-pChar->m_pMounthAwardCenter->m_nStart])
				{
					pComp->SetViewSpr("MB_QIANDAO.png");
					if((i-pChar->m_pMounthAwardCenter->m_nStart + 1) == pChar->m_pMounthAwardCenter->m_nDay)
						m_pForm->GetComponent("qd")->SetEnable(false);
				}
				else
				{
					pComp->SetViewSpr("");
					if((i-pChar->m_pMounthAwardCenter->m_nStart + 1) == pChar->m_pMounthAwardCenter->m_nDay)
						m_pForm->GetComponent("qd")->SetEnable(true);
				}
			}
		}
		if(pChar->m_pMounthAwardCenter->m_nLeftCount > 0)
			m_pForm->GetComponent("bu")->SetEnable(true);
		else
			m_pForm->GetComponent("bu")->SetEnable(false);

		for(int i = 0; i < 5; i ++)
		{
			MounthAward* pAward = NULL;
			if(i < GameAwardConfig::GetInstance().m_pMounthList->Count)
				pAward = (MounthAward*)GameAwardConfig::GetInstance().m_pMounthList->Items[i];
			sprintf(szName,"icon%d",i+1);
			DComponent* pCompIcon = m_pForm->GetComponent(szName);
			sprintf(szName,"get%d",i+1);
			DComponent* pCompGet = m_pForm->GetComponent(szName);
			sprintf(szName,"yi%d",i+1);
			DComponent* pCompRlt = m_pForm->GetComponent(szName);
			if(pAward)
			{
				if(pAward->xAward.nID < 1000)
				{
					if(pAward->xAward.nID == 555)
						sprintf(szIcon,"MB_ZI_EXP1.png");
					else if(pAward->xAward.nID == 666)
						sprintf(szIcon,"MB_MONEY1.png");
					else if(pAward->xAward.nID == 888)
						sprintf(szIcon,"MB_ZUANSHI1.png");
					else if(pAward->xAward.nID == 333)
						sprintf(szIcon,"MB_BANGZUAN1.png");
					pCompIcon->SetViewSpr(szIcon);
					pCompIcon->SetCaptionEx("%d",2,pAward->xAward.nNum);
				}
				else
				{
					ShowItemInfoOnComp(pCompIcon,pAward->xAward.nID,pAward->xAward.nNum);
				}
				if(pChar->m_pMounthAwardCenter->m_nCount < pAward->nCount)
				{
					pCompGet->SetVisible(true);
					pCompGet->bClickLocked = true;
					pCompGet->SetViewSpr("BT_LINGQU234_H.png");
					pCompRlt->SetVisible(false);
				}
				else if(pChar->m_pMounthAwardCenter->m_szGetFlag[i])
				{
					pCompGet->SetVisible(false);
					pCompRlt->SetVisible(true);
				}
				else
				{
					pCompGet->SetVisible(true);
					pCompGet->bClickLocked = false;
					pCompGet->SetViewSpr("BT_LINGQU234.png");
					pCompRlt->SetVisible(false);
				}
			}
			else
			{
				pCompIcon->SetViewSpr("");
				pCompGet->SetVisible(false);
				pCompRlt->SetVisible(false);
			}
		}
	}
}

void LvupAwardFrmEvent(int nPos,char* pName)
{
	if(strcmp(pName,"get") == 0)
	{
		pChar->m_pLvupAwardCenter->RequestGift(nPos);
	}
	else if(strncmp(pName,"icon",4) == 0)
	{
		int n = (atoi(&pName[4]));
		xnList* pList = GameAwardConfig::GetInstance().m_pLvupList;
		LvupAward* pAward = (LvupAward*)pList->Items[nPos];
		if(n <= pAward->pList->Count)
		{
			_simpleAward* pA = (_simpleAward*)pAward->pList->Items[n-1];
			if(pA->nID >= 1000)
			{
				ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
				if(pBase)
				{
					if(pBase->byType == ITEMTYPE_EQUIP)
						pMainMenu->m_pEquipInfoMenu->Open(pBase,FORM_NULL);
					else
						pMainMenu->m_pItemInfoMenu->Open(pBase,FORM_NULL);
				}
			}
		}
	}
}

LvupAwardMenu::LvupAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("jiangli3");
	DComponent* pBaseComp = m_pForm->GetComponent("z");
	DComponent* ppComp[13];
	ppComp[0] = m_pForm->GetComponent("xuan");
	ppComp[1] = m_pForm->GetComponent("gezi1");
	ppComp[2] = m_pForm->GetComponent("gezi2");
	ppComp[3] = m_pForm->GetComponent("gezi3");
	ppComp[4] = m_pForm->GetComponent("gezi4");
	ppComp[5] = m_pForm->GetComponent("gezi5");
	ppComp[6] = m_pForm->GetComponent("icon1");
	ppComp[7] = m_pForm->GetComponent("icon2");
	ppComp[8] = m_pForm->GetComponent("icon3");
	ppComp[9] = m_pForm->GetComponent("icon4");
	ppComp[10] = m_pForm->GetComponent("icon5");
	ppComp[11] = m_pForm->GetComponent("yi");
	ppComp[12] = m_pForm->GetComponent("get");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*4);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,13);
	m_pForm->pCFrame->SetEvent(LvupAwardFrmEvent);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetFPCnt(0);
}

LvupAwardMenu::~LvupAwardMenu()
{

}

void LvupAwardMenu::Open(int nT)
{
	m_pForm->Open(nT);
	xnList* pList = GameAwardConfig::GetInstance().m_pLvupList;
	char szName[32];
	char szIcon[32];
	m_pForm->pCFrame->SetFPCnt(pList->Count);
	for(int i = 0; i < pList->Count; i ++)
	{
		LvupAward* pAward = (LvupAward*)pList->Items[i];
		DComponent* pComp = m_pForm->pCFrame->GetFPComp(i,"xuan");
		sprintf(szIcon,"DB_ZAIXIAN%d.png",i+1);
		pComp->SetViewSpr(szIcon);
		for(int j = 0; j < 5; j ++)
		{
			if(j < pAward->pList->Count)
			{
				sprintf(szName,"gezi%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(true);
				sprintf(szName,"icon%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(true);
				DComponent* pIcon = m_pForm->pCFrame->GetFPComp(i,szName);
				_simpleAward* pA = (_simpleAward*)pAward->pList->Items[j];
				bool bIsItem = false;
				if(pA->nID == 555)
					sprintf(szIcon,"MB_ZI_EXP1.png");
				else if(pA->nID == 666)
					sprintf(szIcon,"MB_MONEY1.png");
				else if(pA->nID == 888)
					sprintf(szIcon,"MB_ZUANSHI1.png");
				else if(pA->nID == 333)
					sprintf(szIcon,"MB_BANGZUAN1.png");
				else
				{
					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
					sprintf(szIcon,"item/%d.png",pBase->nSpr);
					bIsItem = true;
				}
				if (bIsItem)
				{
					pIcon->SetViewSpr(szIcon);
					pIcon->SetCaptionEx("%d",2,pA->nNum);

					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
					char szText[32];
					sprintf(szText,"MB_PZ%d.png",pBase->nStar+1);
					pIcon->SetAddSprByTag(szText,-3,-3,1,100,0,-1);
					pIcon->ClearEffect();
					if (pBase->nStar == 2)
					{
						pIcon->SetEffect(174,1);
					}else if (pBase->nStar == 3)
					{
						pIcon->SetEffect(175,1);
					}
				}else
				{
					pIcon->ClearEffect();
					pIcon->SetViewSpr(szIcon);
					pIcon->SetCaptionEx("%d",2,pA->nNum);
				}
			}
			else
			{
				sprintf(szName,"gezi%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(false);
				sprintf(szName,"icon%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(false);
			}
		}
	}
}

void LvupAwardMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		DComponent* pGetComp = NULL;
		DComponent* pRltComp = NULL;
		for(int i = 0; i < GameAwardConfig::GetInstance().m_pLvupList->Count; i ++)
		{
			LvupAward* pAward = (LvupAward*)GameAwardConfig::GetInstance().m_pLvupList->Items[i];
			pGetComp = m_pForm->pCFrame->GetFPComp(i,"get");
			pRltComp = m_pForm->pCFrame->GetFPComp(i,"yi");
			if(pAward->nLv <= pChar->m_byLvl)
			{
				if(pChar->m_pLvupAwardCenter->m_szGetFlag[i])
				{
					pGetComp->SetVisible(false);
					pRltComp->SetVisible(true);
				}
				else
				{
					pGetComp->SetVisible(true);
					pGetComp->bClickLocked = false;
					pGetComp->SetViewSpr("BT_LINGQU234.png");
					pRltComp->SetVisible(false);
				}
			}
			else
			{
				pGetComp->SetVisible(true);
				pGetComp->bClickLocked = true;
				pGetComp->SetViewSpr("BT_LINGQU234_H.png");
				pRltComp->SetVisible(false);
			}
		}
	}
}

void LoginAwardFrmEvent(int nPos,char* pName)
{
	if(strcmp(pName,"get") == 0)
	{
		pChar->m_pLoginAwardCenter->RequestGift(nPos);
	}
}

void ClickAwardFramDown(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		LoginAwardMenu* pMenu = (LoginAwardMenu*)param;
		pMenu->m_pForm->pCFrame->nChgY += pMenu->m_pForm->pCFrame->nDisStep;
	}
}


LoginAwardMenu::LoginAwardMenu()
{
	m_pForm = pMainInterface->GetDForm("jiangli7");
	DComponent* pBaseComp = m_pForm->GetComponent("xuan");
	DComponent* ppComp[5];
	ppComp[0] = m_pForm->GetComponent("tian");
	m_pForm->GetComponent("tian")->m_nFontSize = 25;
	ppComp[1] = m_pForm->GetComponent("icon1");
	ppComp[2] = m_pForm->GetComponent("icon2");
	ppComp[3] = m_pForm->GetComponent("yi");
	ppComp[4] = m_pForm->GetComponent("get");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,(pBaseComp->nH+10)*3);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,5);
	m_pForm->pCFrame->SetEvent(LoginAwardFrmEvent);
	m_pForm->pCFrame->nSpace   = 10;
	m_pForm->pCFrame->nDisStep = pBaseComp->nH + 10;
	m_pForm->pCFrame->SetFPCnt(0);

	m_pForm->GetComponent("down")->SetEvent(ClickAwardFramDown,this);
}

LoginAwardMenu::~LoginAwardMenu()
{

}

void LoginAwardMenu::Open(int nT)
{
	m_pForm->Open(nT);

	m_pForm->GetComponent("z1")->SetVisible(false);
	m_pForm->GetComponent("z2")->SetVisible(false);

	xnList* pList = NULL;
	if (pChar->m_nLoginDays > 1000)
	{
		pList = GameAwardConfig::GetInstance().m_pLoginNewList;
		m_pForm->GetComponent("z2")->SetVisible(true);
	}
	else
	{
		pList = GameAwardConfig::GetInstance().m_pLoginList;
		m_pForm->GetComponent("z1")->SetVisible(true);
	}

	char szName[32];
	char szIcon[32];
	m_pForm->pCFrame->SetFPCnt(pList->Count);
	for(int i = 0; i < pList->Count; i ++)
	{
		LoginAward* pAward = (LoginAward*)pList->Items[i];
		DComponent* pComp = m_pForm->pCFrame->GetFPComp(i,"tian");
		pComp->SetCaptionEx("%d",1,pAward->nDays);
		for(int j = 0; j < 2; j ++)
		{
			if(j < pAward->pList->Count)
			{
				sprintf(szName,"icon%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(true);
				DComponent* pIcon = m_pForm->pCFrame->GetFPComp(i,szName);
				_simpleAward* pA = (_simpleAward*)pAward->pList->Items[j];
				if(pA->nID < 1000)
				{
					pIcon->eventFunc = NULL;
					if(pA->nID == 555)
					{
						pIcon->SetViewSpr("MB_ZI_EXP1.png");
					}
					else if(pA->nID == 666)
					{
						pIcon->SetEffect(236,2);
						pIcon->SetViewSpr("MB_MONEY1.png");
					}
					else if(pA->nID == 888)
					{
						pIcon->SetViewSpr("MB_ZUANSHI1.png");
					}
					else if(pA->nID == 333)
					{
						pIcon->SetEffect(223,2);
						pIcon->SetViewSpr("MB_BANGZUAN1.png");
					}
					else if(pA->nID == 999)
					{
						pIcon->SetEffect(238,2);
						g_pSlaveMenuCtrl->ShowIcon(pIcon,pA->nNum,0);
						SlaveBasic* pSBase = SlaveBaseCfg::GetInstance().GetBaseInfo(pA->nNum);
						if (pSBase)
						{
							char szTmp[32];
							sprintf(szTmp,"MB_PZ%d.png",pSBase->byQuality+1);
							pIcon->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
							pIcon->SetEvent(ShowSlaveInfoEvent,(void*)pSBase->nID);
						}
					}

					if (pA->nID == 999)
						pIcon->SetCaption("",0);
					else
						pIcon->SetCaptionEx("%d",2,pA->nNum);
				}
				else
				{
					ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
					if (pBase->nStar == 3)
						pIcon->SetEffect(238,2);
					if (pBase->nStar == 2)
						pIcon->SetEffect(223,2);
					ShowItemInfoOnComp(pIcon,pA->nID,pA->nNum);
				}
			}
			else
			{
				sprintf(szName,"icon%d",j+1);
				m_pForm->pCFrame->GetFPComp(i,szName)->SetVisible(false);
			}
		}
	}

	if (pChar->m_pLoginAwardCenter->m_nOpenStap > 3 && pChar->m_pLoginAwardCenter->m_nOpenStap <= m_pForm->pCFrame->GetFPCnt())
		m_pForm->pCFrame->nChgY = (pChar->m_pLoginAwardCenter->m_nOpenStap - 2)* m_pForm->pCFrame->nDisStep;
}

void LoginAwardMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		xnList* pList = NULL;
		int nTmpLD = 0;
		if (pChar->m_nLoginDays > 1000)
		{
			nTmpLD = pChar->m_nLoginDays - 1000;
			pList = GameAwardConfig::GetInstance().m_pLoginNewList;
		}
		else
		{
			nTmpLD = pChar->m_nLoginDays;
			pList = GameAwardConfig::GetInstance().m_pLoginList;
		}

		DComponent* pGetComp = NULL;
		DComponent* pRltComp = NULL;
		for(int i = 0; i < pList->Count; i ++)
		{
			LoginAward* pAward = (LoginAward*)pList->Items[i];
			pGetComp = m_pForm->pCFrame->GetFPComp(i,"get");
			pRltComp = m_pForm->pCFrame->GetFPComp(i,"yi");
			if(pAward->nDays <= nTmpLD)
			{
				if(pChar->m_pLoginAwardCenter->m_szGetFlag[i])
				{
					pGetComp->SetVisible(false);
					pRltComp->SetVisible(true);
				}
				else
				{
					pGetComp->SetVisible(true);
					pGetComp->bClickLocked = false;
					pGetComp->SetViewSpr("BT_LINGQU234.png");
					pRltComp->SetVisible(false);
				}
			}
			else
			{
				pGetComp->SetVisible(true);
				pGetComp->bClickLocked = true;
				pGetComp->SetViewSpr("BT_LINGQU234_H.png");
				pRltComp->SetVisible(false);
			}
		}
	}
}

void Click2RechargeButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pRechargeMenu->OpenForm(2);
}

void ClickRechargeGiftButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pChar->m_pRechargeAwardCenter->RequestGift(pMainMenu->m_pRechargeAwardMenu->m_nSelPos);
}

void ClickCloseGWMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pRechargeAwardMenu->m_pForm->Close();
}

void ClickChgAwardEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	int nPos = (int)param;
	if (nPos > -1 && nPos < 3)
	{
		pMainMenu->m_pRechargeAwardMenu->RefreshAward(nPos);
	}
}

RechargeAwardMenu::RechargeAwardMenu():m_nSelPos(0)
{
	m_pForm = pMainInterface->GetDForm("jiangli5");
	m_pForm->GetComponent("cz")->SetEvent(Click2RechargeButton,this);
	m_pForm->GetComponent("get")->SetEvent(ClickRechargeGiftButton,this);
	m_pForm->GetComponent("close")->SetEvent(ClickCloseGWMenu,this);

	m_pForm->GetComponent("page6")->SetEvent(ClickChgAwardEvent,(void*)0);
	m_pForm->GetComponent("page7")->SetEvent(ClickChgAwardEvent,(void*)1);
	m_pForm->GetComponent("page8")->SetEvent(ClickChgAwardEvent,(void*)2);
}

RechargeAwardMenu::~RechargeAwardMenu()
{

}

void RechargeAwardMenu::Open(int nT)
{
	m_pForm->Open(nT);
	RefreshAward(0);
}

void RechargeAwardMenu::RefreshAward(int nPos)
{
	if (nPos<0 || nPos >2)
		return;

	m_pForm->GetComponent("page6")->SetSprColor(nPos == 0?255:125,nPos == 0?255:125,nPos == 0?255:125);
	m_pForm->GetComponent("page7")->SetSprColor(nPos == 1?255:125,nPos == 1?255:125,nPos == 1?255:125);
	m_pForm->GetComponent("page8")->SetSprColor(nPos == 2?255:125,nPos == 2?255:125,nPos == 2?255:125);

	m_nSelPos = nPos;
	RechargeAward* pAward = (RechargeAward*)GameAwardConfig::GetInstance().m_pRechargeList->Items[nPos];
	if (pAward)
	{
		xnList* pList = pAward->pList;
		char szName[32];
		char szIcon[32];
		for(int i = 0; i < 5; i ++)
		{
			sprintf(szName,"icon%d",i+1);
			DComponent* pComp = m_pForm->GetComponent(szName);
			if(i < pList->Count)
			{
				if (i<2)
					pComp->SetEffect(238,2);
				else if (i<6)
					pComp->SetEffect(223,2);
				else
					pComp->SetEffect(236,2);

				_simpleAward* pAward = (_simpleAward*)pList->Items[i];
				if(pAward->nID < 1000)
				{
					if(pAward->nID == 555)
						sprintf(szIcon,"MB_ZI_EXP1.png");
					else if(pAward->nID == 666)
						sprintf(szIcon,"MB_MONEY1.png");
					else if(pAward->nID == 888)
						sprintf(szIcon,"MB_ZUANSHI1.png");
					else if(pAward->nID == 333)
						sprintf(szIcon,"MB_BANGZUAN1.png");
					pComp->SetViewSpr(szIcon);
					pComp->SetCaptionEx("%d",2,pAward->nNum);
				}
				else
				{
					ShowItemInfoOnComp(pComp,pAward->nID,pAward->nNum);
				}
			}
			else
			{
				pComp->SetViewSpr("");
				pComp->SetCaption("",1);
			}
		}
	}
}

void RechargeAwardMenu::Update()
{
	if (m_pForm->bOpened == false)
		return;

	DComponent* pCompRecharge = m_pForm->GetComponent("cz");
	DComponent* pCompGet = m_pForm->GetComponent("get");
	DComponent* pCompRlt = m_pForm->GetComponent("yi");
	if(pChar->m_pRechargeAwardCenter->m_ifRecharge)
	{
		pCompRecharge->SetVisible(false);
		if(pChar->m_pRechargeAwardCenter->m_ifGet[m_nSelPos])
		{
			pCompGet->SetVisible(false);
			pCompRlt->SetVisible(true);
		}
		else
		{
			if (pChar->m_pRechargeAwardCenter->m_CanGet[m_nSelPos])
			{
				pCompGet->SetVisible(true);
				pCompGet->bClickLocked = false;
				pCompGet->SetViewSpr("BT_LINGQU234.png");
				pCompRlt->SetVisible(false);
			}else
			{
				pCompGet->SetVisible(true);
				pCompGet->bClickLocked = true;
				pCompGet->SetViewSpr("BT_LINGQU234_H.png");
				pCompRlt->SetVisible(false);
			}
		}
	}
	else
	{
		pCompRecharge->SetVisible(true);
		pCompGet->SetVisible(false);
		pCompRlt->SetVisible(false);
	}
}

void IAGetAwardEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pChar->SendCmd(USER,C_GET_INVAWARS);
	pEvent->pComponent->bClickLocked = true;
	pEvent->pComponent->SetViewSpr("BT_LINGQU234_H.png");
}

InviteAwardMenu::InviteAwardMenu()
{
	m_pForm	= pMainInterface->GetDForm("jiangli6");
	m_pForm->GetComponent("fx")->SetVisible(false);
}

InviteAwardMenu::~InviteAwardMenu()
{

}

void InviteAwardMenu::InitMenu()
{
	char	szTmp[256];
	DComponent* pComp	= NULL;
	xnList* pList = GameAwardConfig::GetInstance().m_pInviteList;
	for (int i = 0; i < 4 && i < pList->Count; i++)
	{
		sprintf(szTmp,"icon%d",i+1);
		pComp	= m_pForm->GetComponent(szTmp);
		_simpleAward* pA = (_simpleAward*)pList->Items[i];
		if(pA->nID < 1000)
		{
			pComp->eventFunc = NULL;
			if(pA->nID == 555)
			{
				pComp->SetViewSpr("MB_ZI_EXP1.png");
			}
			else if(pA->nID == 666)
			{
				pComp->SetEffect(236,2);
				pComp->SetViewSpr("MB_MONEY1.png");
			}
			else if(pA->nID == 888)
			{
				pComp->SetViewSpr("MB_ZUANSHI1.png");
			}
			else if(pA->nID == 333)
			{
				pComp->SetEffect(223,2);
				pComp->SetViewSpr("MB_BANGZUAN1.png");
			}
			else if(pA->nID == 999)
			{
				pComp->SetEffect(238,2);
				g_pSlaveMenuCtrl->ShowIcon(pComp,pA->nNum,0);
				SlaveBasic* pSBase = SlaveBaseCfg::GetInstance().GetBaseInfo(pA->nNum);
				if (pSBase)
				{
					char szTmp[32];
					sprintf(szTmp,"MB_PZ%d.png",pSBase->byQuality+1);
					pComp->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
					pComp->SetEvent(ShowSlaveInfoEvent,(void*)pSBase->nID);
				}
			}

			if (pA->nID == 999)
				pComp->SetCaption("",0);
			else
				pComp->SetCaptionEx("%d",2,pA->nNum);
		}
		else
		{
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pA->nID);
			if (pBase->nStar == 3)
				pComp->SetEffect(238,2);
			if (pBase->nStar == 2)
				pComp->SetEffect(223,2);
			ShowItemInfoOnComp(pComp,pA->nID,pA->nNum);
		}

		sprintf(szTmp,"get%d",i+1);
		pComp	= m_pForm->GetComponent(szTmp);
		pComp->SetEvent(IAGetAwardEvent,this);
		pComp->bClickLocked = true;
		pComp->SetViewSpr("BT_LINGQU234_H.png");
		sprintf(szTmp,"yi%d",i+1);
		pComp	= m_pForm->GetComponent(szTmp);
		pComp->SetVisible(false);
	}
}

bool InviteAwardMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 30)
	{
		pMainMenu->PopTipMsg("需要达到30级!");
		return false;
	}
	return true;
}

void InviteAwardMenu::SetInvInfo(char*	szCode, int nReach, int nCheck)
{
	InitMenu();

	m_pForm->GetComponent("num")->SetCaptionEx("%d",1,nReach);
	m_pForm->GetComponent("ma")->SetCaption(szCode,1);

	static int nCheckVal[4]	= {1,3,5,10};
	char		szTmp[32];
	for (int i = 0; i < 4; i++)
	{
		if(nCheck>=nCheckVal[i])
		{
			sprintf(szTmp,"get%d",i+1);
			m_pForm->GetComponent(szTmp)->SetVisible(false);
			sprintf(szTmp,"yi%d",i+1);
			m_pForm->GetComponent(szTmp)->SetVisible(true);
		}
		else if(nReach>=nCheckVal[i])
		{
			sprintf(szTmp,"get%d",i+1);
			m_pForm->GetComponent(szTmp)->SetVisible(true);
			m_pForm->GetComponent(szTmp)->bClickLocked = false;
			m_pForm->GetComponent(szTmp)->SetViewSpr("BT_LINGQU234.png");
			sprintf(szTmp,"yi%d",i+1);
			m_pForm->GetComponent(szTmp)->SetVisible(false);
		}
		else
		{
			sprintf(szTmp,"get%d",i+1);
			m_pForm->GetComponent(szTmp)->SetVisible(true);
			m_pForm->GetComponent(szTmp)->bClickLocked = true;
			m_pForm->GetComponent(szTmp)->SetViewSpr("BT_LINGQU234_H.png");
			sprintf(szTmp,"yi%d",i+1);
			m_pForm->GetComponent(szTmp)->SetVisible(false);
		}
	}
}
