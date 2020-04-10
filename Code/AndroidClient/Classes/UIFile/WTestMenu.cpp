#include "UserData.h"
#include "WTestMenu.h"
#include "GameActivity.h"
#include "MainMenu.h"

void MsgboxGiveupTest(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pWTestCenter->RequestGiveup();
		WTestMenu* pMenu = (WTestMenu*)param;
		pMenu->Close();
	}
}

void ClickTCloseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->PopAskMsgBox("是否放弃今天的答题？",MsgboxGiveupTest,param);
	}
}

void ClickRadioButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nRlt = (int)param;
	if(pMainMenu->m_pWTestMenu->m_pRadio->GetSel() >= 0)
		return;
	pMainMenu->m_pWTestMenu->m_pRadio->SetSel(nRlt);
	pChar->m_pWTestCenter->AnswerQuestion(nRlt);
}

WTestMenu::WTestMenu()
{
	m_pForm = pMainInterface->GetDForm("dati");
	DComponent* pComp[4];
	char szName[8];
	for(int i = 0; i < 4; i ++)
	{
		sprintf(szName,"k%d",i+1);
		pComp[i] = m_pForm->GetComponent(szName);
	}
	m_pForm->GetComponent("close")->SetEvent(ClickTCloseButton,this);
	m_pRadio = new CCtrlRadio(pComp,4);
	m_pRadio->SetSel(-1);
	for(int i = 0; i < 4; i ++)
		pComp[i]->SetEvent(ClickRadioButton,(void*)i);
}

WTestMenu::~WTestMenu()
{

}

void WTestMenu::Open()
{
	m_pForm->Open();
}

void WTestMenu::Close()
{
	m_pForm->Close();
}

void WTestMenu::Answer()
{
	pChar->m_pWTestCenter->AnswerQuestion(m_pRadio->GetSel());
}

void WTestMenu::Update()
{
	if(pChar->m_pWTestCenter->m_ifRefresh)
	{
		pChar->m_pWTestCenter->m_ifRefresh = false;
		m_pRadio->SetSel(-1);
	}
	if(m_pForm && m_pForm->bOpened)
	{
		DComponent* pComp = NULL;
		pComp = m_pForm->GetComponent("tishu");
		if(pComp)
		{
			pComp->SetCaptionEx("%d/%d",1,pChar->m_pWTestCenter->m_nCount+1,TestAwardCfg::GetInstance().m_nACount);
		}
		pComp = m_pForm->GetComponent("paiming");
		if(pComp)
		{
			pComp->SetCaptionEx("%d",1,pChar->m_pWTestCenter->m_nPlace);
		}
		pComp = m_pForm->GetComponent("jifen");
		if(pComp)
		{
			pComp->SetCaptionEx("%d",1,pChar->m_pWTestCenter->m_nStar);
		}
		pComp = m_pForm->GetComponent("dadui");
		if(pComp)
		{
			pComp->SetCaptionEx("%d",1,pChar->m_pWTestCenter->m_nCorrect);
		}
		pComp = m_pForm->GetComponent("time");
		if(pComp)
		{
			if(pChar->m_pWTestCenter->m_nEndSec > 0 && pChar->m_pWTestCenter->m_nEndSec > time(0))
				pComp->SetCaptionEx("%02d",1,pChar->m_pWTestCenter->m_nEndSec - time(0));
			else
				pComp->SetCaption("",1);
		}
		if(pChar->m_pWTestCenter->m_pObj)
		{
			pComp = m_pForm->GetComponent("timu");
			pComp->SetCaptionEx("%s",1,pChar->m_pWTestCenter->m_pObj->pQuestion);
			for(int i = 0; i < 3; i ++)
			{
				char szicon[4];
				sprintf(szicon,"x%d",i+1);
				pComp = m_pForm->GetComponent(szicon);
				if(i < pChar->m_pWTestCenter->m_pObj->nStar)
					pComp->SetVisible(true);
				else
					pComp->SetVisible(false);
			}
			for(int i = 0; i < 4; i ++)
			{
				char szC[] = {'A','B','C','D'};
				char szname[8];
				sprintf(szname,"d%d",i+1);
				pComp = m_pForm->GetComponent(szname);
				if(pChar->m_pWTestCenter->m_byCh[i] == 1)
					pComp->SetCaptionEx("%c.%s",0,szC[i],pChar->m_pWTestCenter->m_pObj->pAnswer);
				else if(pChar->m_pWTestCenter->m_byCh[i] > 1)
					pComp->SetCaptionEx("%c.%s",0,szC[i],pChar->m_pWTestCenter->m_pObj->pChoosen[pChar->m_pWTestCenter->m_byCh[i]-2]);
				else
					pComp->SetCaption("",1);
			}
		}
		else
		{
			pComp = m_pForm->GetComponent("timu");
			pComp->SetCaption("",1);
			for(int i = 0; i < 3; i ++)
			{
				char szicon[4];
				sprintf(szicon,"x%d",i+1);
				pComp = m_pForm->GetComponent(szicon);
				pComp->SetVisible(false);
			}
			for(int i = 0; i < 4; i ++)
			{
				char szname[8];
				sprintf(szname,"d%d",i+1);
				pComp = m_pForm->GetComponent(szname);
				pComp->SetCaption("",1);
			}
		}
	}
}

void ClickTAcceptButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pChar->m_pWTestCenter->RequestForGift();
}

WTestRankMenu::WTestRankMenu()
{
	m_pForm = pMainInterface->GetDForm("datibang");
	m_pForm->GetComponent("jiangli")->SetEvent(ClickTAcceptButton,this);
	DComponent* pBaseCom = m_pForm->GetComponent("xz");
	DComponent* ppComp[3];
	ppComp[0] = m_pForm->GetComponent("pm");
	ppComp[1] = m_pForm->GetComponent("name");
	ppComp[2] = m_pForm->GetComponent("jf");
	m_pForm->pCFrame = new CCtrlFrame(pBaseCom->nX,pBaseCom->nY,pBaseCom->nW,pBaseCom->nH*6);
	m_pForm->pCFrame->InitFParts(pBaseCom,ppComp,3);
	m_pForm->pCFrame->nDisStep = pBaseCom->nH;
	m_pForm->pCFrame->SetFPCnt(0);
}

WTestRankMenu::~WTestRankMenu()
{

}

void WTestRankMenu::Open()
{
	m_pForm->Open(2);
}

void WTestRankMenu::Close()
{
	m_pForm->Close();
}

void WTestRankMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		DComponent* pComp = NULL;
		pComp = m_pForm->GetComponent("paiming");
		if(pComp)
		{
			if(pChar->m_pWTestCenter->m_nPlace == 0)
				pComp->SetCaption("未参加",1);
			else
				pComp->SetCaptionEx("%d",1,pChar->m_pWTestCenter->m_nPlace);
		}
		pComp = m_pForm->GetComponent("jiangli");
		if(pComp)
		{
			if(pChar->m_pWTestCenter->m_nPlace == 0)
				pComp->SetVisible(false);
			else
				pComp->SetVisible(true);
		}
		pComp = m_pForm->GetComponent("jifen");
		if(pComp)
		{
			pComp->SetCaptionEx("%d",1,pChar->m_pWTestCenter->m_nStar);
		}
		pComp = m_pForm->GetComponent("dadui");
		if(pComp)
		{
			pComp->SetCaptionEx("%d",1,pChar->m_pWTestCenter->m_nCorrect);
		}
		m_pForm->pCFrame->SetFPCnt(pChar->m_pWTestCenter->m_nRankCnt);
		for(int i = 0; i < pChar->m_pWTestCenter->m_nRankCnt; i ++)
		{
			pComp = m_pForm->pCFrame->GetFPComp(i,"pm");
			char szIcon[32];
			sprintf(szIcon,"MB_DT%d.png",i+1);
			pComp->SetViewSpr(szIcon);
			pComp = m_pForm->pCFrame->GetFPComp(i,"name");
			pComp->SetCaptionEx("%s",0,pChar->m_pWTestCenter->m_szName[i]);
			pComp = m_pForm->pCFrame->GetFPComp(i,"jf");
			pComp->SetCaptionEx("%d",0,pChar->m_pWTestCenter->m_szScore[i]);
		}
		if(pChar->m_pWTestCenter->m_nTime > 0 && pChar->m_pWTestCenter->m_nTime > xnGetSecCount())
		{
			int nLeft = (pChar->m_pWTestCenter->m_nTime - xnGetSecCount())/60;
			if(nLeft > 59)
				m_pForm->GetComponent("time")->SetCaptionEx("%d小时",1,nLeft/60);
			else
				m_pForm->GetComponent("time")->SetCaptionEx("%d分钟",1,nLeft+1);
		}
		else
		{
			m_pForm->GetComponent("time")->SetCaption("0分钟",1);
		}
	}
}