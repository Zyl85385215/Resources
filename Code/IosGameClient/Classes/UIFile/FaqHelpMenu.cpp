#include "MainMenu.h"
#include "UserData.h"
#include "FaqHelpMenu.h"
#include "ios_item_packet.h"

#include "xnExtstr.h"
#include "FaqHelpCfg.h"

static void CBFrameFBEvent(int nPos,char* szName)
{
	if (nPos < 0)
		return;

	pMainMenu->m_pFaqHelpMenu->m_nCurPos = nPos;
	pMainMenu->m_pFaqHelpMenu->m_bUpdate = true;
	//pMainMenu->m_pFaqHelpMenuEx->m_bUpdate = true;
	pMainMenu->m_pFaqHelpMenuEx->m_byHelpType = nPos;

}

static void CBFrameFBEventBank(int nPos,char* szName)
{

}
static void CBFrameFBStepEventBank(int nPos)
{

}

void ChickCloseEvent(InterfaceEvent * pEvent,void * param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	FaqHelpMenu* pFaqMenu = (FaqHelpMenu*)param;
	if (pFaqMenu)
		pFaqMenu->Close();
}

FaqHelpMenu::FaqHelpMenu():m_bUpdate(false)
{
	m_pForm = pMainInterface->GetDForm ("bangzhu");

	DComponent*	pBaseComp	= m_pForm->GetComponent ("s2");
	DComponent* pComp[2];
	pComp[0] = m_pForm->GetComponent("s1");
	pComp[1] = m_pForm->GetComponent("xuan");
	m_pForm->pCFrame	= new CCtrlFrame(pBaseComp->nX, pBaseComp->nY, pBaseComp->nW, pBaseComp->nH*6 + pBaseComp->nH/2);
	m_pForm->pCFrame->InitFParts(pBaseComp, pComp, 2);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= pBaseComp->nH;
	m_pForm->pCFrame->SetFPCnt(0, false);

	m_pForm->GetComponent("close")->SetEvent(ChickCloseEvent, this);
}


FaqHelpMenu::~FaqHelpMenu()
{

}

void FaqHelpMenu::Open()
{
	m_pForm->Open();
	m_bUpdate = true;
}

void FaqHelpMenu::Close()
{
	m_pForm->Close();
	pMainMenu->m_pFaqHelpMenuEx->Close();
	m_nCurPos = 0;
}

void FaqHelpMenu::Update()
{
	if (!m_pForm->bOpened)
		return;
	if (m_bUpdate)
	{
		m_bUpdate = false;

		int nCount = FaqHelpCfg::GetInstance().m_pList->Count;
		m_pForm->pCFrame->SetFPCnt(nCount);

		for (int i = 0; i < nCount; i++)
		{
			FaqHelpObj*	pFaqObj = (FaqHelpObj*)FaqHelpCfg::GetInstance().m_pList->Items[i];
			if (pFaqObj)
			{
				m_pForm->pCFrame->GetFPComp(i, "s1")->SetCaption(pFaqObj->szName, 1);
				if (m_nCurPos == i)
					m_pForm->pCFrame->GetFPComp(i, "xuan")->SetVisible(true);
				else
					m_pForm->pCFrame->GetFPComp(i, "xuan")->SetVisible(false);
				//m_pForm->pCFrame->GetFPComp(i, "s1")->SetSprColor(255, 215, 100);
			}
		}
		pMainMenu->m_pFaqHelpMenuEx->Open();
	}
}

FaqHelpMenuEx::FaqHelpMenuEx():m_bUpdate(false),m_byHelpType(0)
{
	m_pForm = pMainInterface->GetDForm ("bangzhu2");
	m_pForm->bZorde = FORM_ZORDER_KUANG1;

	DComponent*	pBaseComp	= m_pForm->GetComponent ("qq");
	m_pForm->pCFrame	= new CCtrlFrame(pBaseComp->nX, pBaseComp->nY, pBaseComp->nW, pBaseComp->nH*2+50);
	DComponent* pComp[5];
	pComp[0] = m_pForm->GetComponent("kuang"); 
	pComp[1] = m_pForm->GetComponent("name"); 
	pComp[2] = m_pForm->GetComponent("des"); 
	pComp[3] = m_pForm->GetComponent("tu"); 
	pComp[4] = m_pForm->GetComponent("num"); 

	m_pForm->pCFrame->InitFParts(pBaseComp, pComp, 5);
	m_pForm->pCFrame->SetEvent (CBFrameFBEventBank);
	m_pForm->pCFrame->SetStepEvent(CBFrameFBStepEventBank);
	m_pForm->pCFrame->nSpace	= 5;
	m_pForm->pCFrame->nDisStep	= pBaseComp->nH;

	m_pForm->pCFrame->SetFPCnt(0, false);
}

FaqHelpMenuEx::~FaqHelpMenuEx()
{

}

void FaqHelpMenuEx::Open()
{
	m_pForm->Open();
	m_bUpdate = true;
}

void FaqHelpMenuEx::Close()
{
	m_pForm->Close();
	m_byHelpType = 0;
}

void FaqHelpMenuEx::Update()
{
	if (!m_pForm->bOpened)
		return;
	if (m_bUpdate)
	{
		m_bUpdate = false;

		FaqHelpObj*	pFaqObj = (FaqHelpObj*)FaqHelpCfg::GetInstance().m_pList->Items[m_byHelpType];
		if (pFaqObj)
		{
			m_pForm->pCFrame->SetFPCnt(pFaqObj->nFaqCount);
			for (int i = 0; i < pFaqObj->nFaqCount; i++)
			{
				FaqHelpMsg* pFaqMsg = (FaqHelpMsg*)pFaqObj->pFaqHelpMsgList->Items[i];
				if (pFaqMsg)
				{
					m_pForm->pCFrame->GetFPComp(i, "tu")->SetSprColor(255, 215, 0);
					m_pForm->pCFrame->GetFPComp(i, "num")->SetCaptionEx("%d", 1, i+1);
					m_pForm->pCFrame->GetFPComp(i, "name")->SetCaption(pFaqMsg->szFaqName, 0);
					m_pForm->pCFrame->GetFPComp(i, "name")->SetSprColor(255, 215, 0);
					m_pForm->pCFrame->GetFPComp(i, "des")->SetCaption(pFaqMsg->szFaqDesc, 0);
					m_pForm->pCFrame->GetFPComp(i, "des")->SetSprColor(255, 255, 255);
				}
			}
		}
	}
}

