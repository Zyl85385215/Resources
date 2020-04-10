#include "UserData.h"
#include "ActivityMenu.h"
#include "GameActivity.h"
#include "MainMenu.h"
#include "ActivedegreeCfg.h"
#include "GameActivityCfg.h"
#include "MapControl.h"
#include "SelLineMenu.h"
#include "xnExtstr.h"

void ClickGetAwardEvent(InterfaceEvent* pEvent,void* param)  //活动度奖励 ：箱子点击
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pChar->m_pADegreeCenter->RequestForAward();
}

void ClickDegreeGoEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	ACDegreeMenu* pMenu = (ACDegreeMenu*)param;
	int nPos = pMenu->m_nPos;
	if(nPos < ActiveDegree::GetInstance().m_pDaylyList->Count)
	{
		UserDegreePart* pPart = (UserDegreePart*)pChar->m_pADegreeCenter->m_pList->Items[nPos];
		ADDaylyPart*	pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[nPos];
		if(pPart && pPartA)
		{
			ADPartChoose* pPartB = (ADPartChoose*)pPartA->pList->Items[pPart->byType];
			if(pPartB->pForm && strlen(pPartB->pForm) > 0 && pPartB->pComp)
			{
				DComponent* pCom = pMainInterface->GetComponent(pPartB->pForm,pPartB->pComp);
				if(pCom && pCom->eventFunc)
				{
					InterfaceEvent	IEvent;
					IEvent.x		= 0;
					IEvent.y		= 0;
					IEvent.event	= INTERFACE_EVENT_CLICK;
					IEvent.pComponent	= pCom;
					pCom->eventFunc (&IEvent,pCom->pCbParam);
				}
				pMainMenu->m_pACDegreeMenu->CloseTip();
			}
		}
	}
}

void ClickCloseTipButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ACDegreeMenu* pMenu = (ACDegreeMenu*)param;
		pMenu->CloseTip();
	}
}

void CheckAwardetl(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ACDegreeMenu* pMenu = (ACDegreeMenu*)param;
		int nPos	= 0;
		sscanf(pEvent->pComponent->szName,"xz%d",&nPos);
		pMenu->OpenAwardTip(nPos);
	}
}

void ACDFrmEvent(int nPos,char * szName)
{
	if(nPos < ActiveDegree::GetInstance().m_pDaylyList->Count)
	{
		pMainMenu->m_pACDegreeMenu->OpenTip(nPos);
	}
}

ACDegreeMenu::ACDegreeMenu()
{
	m_pForm = pMainInterface->GetDForm("huoyue");
	m_pTipForm = pMainInterface->GetDForm("huoyuetip");
	DComponent* pBaseComp = m_pForm->GetComponent("xz");
	DComponent* ppComp[4];
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*5);
	ppComp[0] = m_pForm->GetComponent("s");
	ppComp[1] = m_pForm->GetComponent("mb");
	ppComp[2] = m_pForm->GetComponent("name");
	ppComp[3] = m_pForm->GetComponent("hy");
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,4);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetEvent(ACDFrmEvent);
	m_pForm->pCFrame->SetFPCnt(0);

	m_pForm->GetComponent("lq")->SetEvent(ClickGetAwardEvent,this);
	
	m_pForm->GetComponent("dt")->byInputType	= 8;
	m_pForm->GetComponent("dt")->SetProgress(0,110," ");
	m_pForm->GetComponent("hyd")->SetCaption("0",1);
	m_nTipX = m_pTipForm->GetComponent("no")->nX;
	m_pTipForm->GetComponent("go")->SetEvent(ClickDegreeGoEvent,this);
	m_pTipForm->GetComponent("no")->SetEvent(ClickCloseTipButton,this);
	m_nPos = 0;

	char szTxt[32];
	for (int i = 0; i < 4; i++)
	{
		sprintf(szTxt,"xz%d",i+1);
		m_pForm->GetComponent(szTxt)->SetEvent(CheckAwardetl,this);
	}
}

ACDegreeMenu::~ACDegreeMenu()
{

}

void ACDegreeMenu::OpenTip(int nPos)
{
	UserDegreePart* pPart = (UserDegreePart*)pChar->m_pADegreeCenter->m_pList->Items[nPos];
	ADDaylyPart*	pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[nPos];
	if(pPart && pPartA)
	{
		ADPartChoose* pPartB = (ADPartChoose*)pPartA->pList->Items[pPart->byType];
		{
			if(pPartB->pForm && strlen(pPartB->pForm) > 0 && pPartB->pComp)
			{
				m_pTipForm->GetComponent("go")->SetVisible(true);
				m_pTipForm->GetComponent("no")->SetPosX(m_nTipX);
				m_pTipForm->GetComponent("no")->SetViewSpr("BT_ZB_XIULI2.png");
			}
			else
			{
				m_pTipForm->GetComponent("go")->SetVisible(false);
				m_pTipForm->GetComponent("no")->SetPosX(m_nTipX-100);
				m_pTipForm->GetComponent("no")->SetViewSpr("BT_HY_QUEDING.png");
			}
			if(pPartB->pDesc)
				m_pTipForm->GetComponent("txt")->SetCaptionEx("%s",0,pPartB->pDesc);
			else
				m_pTipForm->GetComponent("txt")->SetCaption("",0);
			m_pTipForm->Open(2);
			m_nPos = nPos;
		}
	}
}

void ACDegreeMenu::OpenAwardTip(int nAwardPos)
{
	m_pTipForm->GetComponent("go")->SetVisible(false);
	m_pTipForm->GetComponent("no")->SetPosX(m_nTipX-100);
	m_pTipForm->GetComponent("no")->SetViewSpr("BT_HY_QUEDING.png");

	char szDesc[1024] = {0};
	int  nListCnt = ActiveDegree::GetInstance().szAward[nAwardPos-1].pList->Count;
	for(int i = 0; i < nListCnt; i ++)
	{
		char szTempDesc[128];
		ADAwardPart* pAward = (ADAwardPart*)ActiveDegree::GetInstance().szAward[nAwardPos-1].pList->Items[i];
		if(pAward->nID > 0)
		{
			if (pAward->nID == 666)
			{
				int nRVal	= pAward->nCnt;
				if(pAward->nCnt < 1000)
					nRVal	= pAward->nCnt* pChar->m_byLvl*pChar->m_byLvl;
				sprintf(szTempDesc,"金币 x %d",nRVal);
				strcat(szDesc,szTempDesc);
			}else
			{
				ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(pAward->nID);
				if (!pBase)
					continue;
				sprintf(szTempDesc,"%s x %d",pBase->szName,pAward->nCnt);
				strcat(szDesc,szTempDesc);
			}
			if (i < nListCnt-1)
			{
				sprintf(szTempDesc,"，");
				strcat(szDesc,szTempDesc);
			}
		}
	}
	if (szDesc != NULL)
		m_pTipForm->GetComponent("txt")->SetCaptionEx("%s",0,szDesc);
	else
		m_pTipForm->GetComponent("txt")->SetCaption("",0);

	m_pTipForm->Open(2);
}

void ACDegreeMenu::CloseTip()
{
	m_pTipForm->Close();
}

void ACDegreeMenu::Update()
{
	if(pChar->m_pADegreeCenter->m_ifNeedReflesh)
	{
		pChar->m_pADegreeCenter->m_ifNeedReflesh = false;
		char szName[32];
		m_pForm->pCFrame->SetFPCnt(ActiveDegree::GetInstance().m_pDaylyList->Count);
		int nGetPoint = 0;
		for(int i = 0; i < ActiveDegree::GetInstance().m_pDaylyList->Count; i ++)
		{
			UserDegreePart* pTempPart = (UserDegreePart*)pChar->m_pADegreeCenter->m_pList->Items[i];
			ADDaylyPart* pPartA = (ADDaylyPart*)ActiveDegree::GetInstance().m_pDaylyList->Items[i];
			ADPartChoose* pPartB = (ADPartChoose*)pPartA->pList->Items[pTempPart->byType];
			int nCount = pTempPart->byCount > pPartB->nCount ? pPartB->nCount : pTempPart->byCount;
			if(pTempPart->ifDone)
				nGetPoint += pPartA->nPoint;

			DComponent* pTempCom = m_pForm->pCFrame->GetFPComp(i,"name");
			pTempCom->SetCaptionEx("%s(%d/%d)",1,pPartB->pName,nCount,pPartB->nCount);
			if(pTempPart->ifDone)
			{
				pTempCom->SetCaptionEx("%s(已完成)",1,pPartB->pName);
				pTempCom->SetSprColor(0,176,52);
			}
			else if(nCount > 0)
				pTempCom->SetSprColor(244,183,87);
			else
				pTempCom->SetSprColor(255,255,255);

			pTempCom = m_pForm->pCFrame->GetFPComp(i,"hy");
			pTempCom->SetCaptionEx("+%d",1,pPartA->nPoint);
			pTempCom->SetSprColor(0,88,250);

		}
		m_pForm->GetComponent("dt")->SetProgress(nGetPoint,110," ");
		m_pForm->GetComponent("hyd")->SetCaptionEx("%d",1,nGetPoint);
		int nLv = ActiveDegree::GetInstance().CheckLevel(nGetPoint);
		for(int i = 0; i < 4; i ++)
		{
			char szIcon[32];
			sprintf(szName,"xz%d",i+1);
			DComponent* pComp = m_pForm->GetComponent(szName);
			if(i >= pChar->m_pADegreeCenter->m_byDegreeLV && i < nLv)
			{
				sprintf(szIcon,"MB_XIANGZI%d.png",4-i);
				pComp->SetViewSpr(szIcon);
				pComp->SetSprColor(255,255,255);
			}
			else
			{
				if(i < pChar->m_pADegreeCenter->m_byDegreeLV)
				{
					sprintf(szIcon,"MB_XIANGZI%d.png",14-i);
					pComp->SetViewSpr(szIcon);
					pComp->SetSprColor(255,255,255);
				}
				else
				{
					sprintf(szIcon,"MB_XIANGZI%d.png",4-i);
					pComp->SetViewSpr(szIcon);
					pComp->SetSprColor(127,127,127);
				}
			}
		}
		if(nLv > pChar->m_pADegreeCenter->m_byDegreeLV)
		{
			m_pForm->GetComponent("lq")->bClickLocked = false;
			m_pForm->GetComponent("lq")->SetViewSpr("BT_LINGQU234.png");
		}
		else
		{
			m_pForm->GetComponent("lq")->bClickLocked = false;
			m_pForm->GetComponent("lq")->SetViewSpr("BT_LINGQU234_H.png");
		}
	}
}


void ActivityButtonEvent(int nPos,char* name)
{
	if(nPos < pChar->m_pActivityCenter->m_pList->Count)
	{
		if(strcmp(name,"go") == 0)
		{
			if(pChar->CheckInFight())
			{
				pMainMenu->PopTipMsg("战斗状态下无法前往活动!");
				return;
			}
			if(g_pMapCenter->m_nBaseMapIndex == 66)
			{
				pMainMenu->PopTipMsg("该活动地图内，无法打开该界面!");
				return;
			}
			pChar->m_pActivityCenter->RequestEnter(nPos);
		}
		else
		{
			pMainMenu->m_pActivityMenu->m_nPos = nPos;
		}
	}
}

void ClickFramDown(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ActivityMenu* pMenu = (ActivityMenu*)param;
		pMenu->m_pForm->pCFrame->nChgY += pMenu->m_pForm->pCFrame->nDisStep;
	}
}


ActivityMenu::ActivityMenu()
{
	m_nPos = 0;
	m_pForm = pMainInterface->GetDForm("huodong");
	DComponent* pBaseCom = m_pForm->GetComponent("zi");
	DComponent* ppComp[9];
	m_pForm->pCFrame = new CCtrlFrame(pBaseCom->nX,pBaseCom->nY,pBaseCom->nW,pBaseCom->nH*3);
	ppComp[0] = m_pForm->GetComponent("name");
	ppComp[1] = m_pForm->GetComponent("lv");
	ppComp[2] = m_pForm->GetComponent("time");
	ppComp[3] = m_pForm->GetComponent("go");
	ppComp[4] = m_pForm->GetComponent("mb");
	ppComp[5] = m_pForm->GetComponent("x1");
	ppComp[6] = m_pForm->GetComponent("gezi");
	ppComp[7] = m_pForm->GetComponent("icon");
	ppComp[8] = m_pForm->GetComponent("leixing");
	m_pForm->pCFrame->InitFParts(pBaseCom,ppComp,9);
	m_pForm->pCFrame->nDisStep = pBaseCom->nH;
	m_pForm->pCFrame->SetEvent(ActivityButtonEvent);
	m_pForm->pCFrame->SetFPCnt(0);

	m_pForm->GetComponent("down")->SetEvent(ClickFramDown,this);
}

ActivityMenu::~ActivityMenu()
{

}

void ActivityMenu::Open(int nT)
{
	m_pForm->Open(nT);
	pChar->m_pActivityCenter->RequestForInfo();
}

void ActivityMenu::SetFramStepByActID(int nID)
{
	xnList* pList = pChar->m_pActivityCenter->m_pList;
	for (int i=0; i<pList->Count; i++)
	{
		ActivityObj* pObj = (ActivityObj*)pList->Items[i];	
		if (pObj && pObj->nID == nID)     
		{		
			if (i > 2)
			{
				pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nSetY = pMainMenu->m_pActivityMenu->m_pForm->pCFrame->nDisStep*(i-1);
			}
			m_nPos = i;
		}
	}
}

void ActivityMenu::Update()
{
	if(pChar->m_pActivityCenter->m_ifChg)
	{
		pChar->m_pActivityCenter->m_ifChg = false;
		xnList* pList = pChar->m_pActivityCenter->m_pList;
		m_pForm->pCFrame->SetFPCnt(pList->Count);
		for(int i = 0; i < pList->Count; i ++)
		{
			ActivityObj* pObj = (ActivityObj*)pList->Items[i];
			DComponent* pComp = m_pForm->pCFrame->GetFPComp(i,"name");
			if (pObj->nFlag)
			{
				if (pObj->byActionEvent == 6)
					pComp->SetCaptionEx("%s(%d/%d)",1,pObj->pName,pChar->m_pClientFlag->GetCPlimitF(pObj->nFlag),pObj->nMaxnum);
				else
					pComp->SetCaptionEx("%s(%d/%d)",1,pObj->pName,pChar->m_pClientFlag->GetFlag(pObj->nFlag),pObj->nMaxnum);
			}else
				pComp->SetCaptionEx("%s",1,pObj->pName);
			pComp = m_pForm->pCFrame->GetFPComp(i,"icon");
			pComp->SetViewSpr(pObj->pIcon);
			pComp = m_pForm->pCFrame->GetFPComp(i,"leixing");
			if (pObj->nAwardType == 1)
				pComp->SetViewSpr("MB_HD_EXP.png");
			else if (pObj->nAwardType == 2)
				pComp->SetViewSpr("MB_HD_MONEY.png");
			else if (pObj->nAwardType == 3)
				pComp->SetViewSpr("MB_HD_DAOJU.png");
			else
				pComp->SetViewSpr("");
			pComp = m_pForm->pCFrame->GetFPComp(i,"lv");
			if(pObj->nLvMin == 0 && pObj->nLvMax == 0)
				pComp->SetCaption("无",1);
			else if(pObj->nLvMin > 0 && pObj->nLvMax == 0)
				pComp->SetCaptionEx("%d级以上",1,pObj->nLvMin);
			else if(pObj->nLvMin == 0 && pObj->nLvMax > 0)
				pComp->SetCaptionEx("%d级以下",1,pObj->nLvMax);
			else
				pComp->SetCaptionEx("%d级~%d级",1,pObj->nLvMin,pObj->nLvMax);
			pComp = m_pForm->pCFrame->GetFPComp(i,"time");
			char* szWeekName[] = {"周一","周二","周三","周四","周五","周六","周日"};
			if(pObj->pWeekList->Count == 7)
				pComp->SetCaptionEx("每天%02d:%02d到%02d:%02d",1,pObj->dwStartTime>>16,pObj->dwStartTime&0xFFFF,pObj->dwEndTime>>16,pObj->dwEndTime&0xFFFF);
			else
			{
				std::string strMsg("每");
				for(int j = 0; j < pObj->pWeekList->Count; j ++)
				{
					int n = (int)pObj->pWeekList->Items[j];
					strMsg.append(szWeekName[n-1]);
					if(j != pObj->pWeekList->Count-1)
						strMsg.append("、");
				}
				char szTemp[32];
				sprintf(szTemp,"%02d:%02d到%02d:%02d",pObj->dwStartTime>>16,pObj->dwStartTime&0xFFFF,pObj->dwEndTime>>16,pObj->dwEndTime&0xFFFF);
				strMsg.append(szTemp);
				pComp->SetCaptionEx("%s",1,strMsg.c_str());
			}
			pComp = m_pForm->pCFrame->GetFPComp(i,"go");
			if(pObj->byActionEvent == 0)
				pComp->SetVisible(false);
			else
				pComp->SetVisible(true);
		}
	}
	if(m_pForm && m_pForm->bOpened)
	{
		for(int i = 0; i < pChar->m_pActivityCenter->m_pList->Count; i ++)
		{
			DComponent* pCompChoose = m_pForm->pCFrame->GetFPComp(i,"mb");
			if(pCompChoose == NULL)
				continue;
			if(i == m_nPos)
				pCompChoose->SetVisible(true);
			else
				pCompChoose->SetVisible(false);

			if(i < pChar->m_pActivityCenter->m_pList->Count)
			{
				ActivityObj* pObj = (ActivityObj*)pChar->m_pActivityCenter->m_pList->Items[i];
				DComponent* pComp = m_pForm->pCFrame->GetFPComp(i,"name");
				if (pObj->nFlag)
				{
					if (pObj->byActionEvent == 6)
						pComp->SetCaptionEx("%s(%d/%d)",1,pObj->pName,pChar->m_pClientFlag->GetCPlimitF(pObj->nFlag),pObj->nMaxnum);
					else
						pComp->SetCaptionEx("%s(%d/%d)",1,pObj->pName,pChar->m_pClientFlag->GetFlag(pObj->nFlag),pObj->nMaxnum);
				}else
					pComp->SetCaptionEx("%s",1,pObj->pName);
			}
			
		}
		DComponent* pCompDesc = m_pForm->GetComponent("ms");
		if(pCompDesc == NULL)
			return;
		if(m_nPos < pChar->m_pActivityCenter->m_pList->Count)
		{
			ActivityObj* pObj = (ActivityObj*)pChar->m_pActivityCenter->m_pList->Items[m_nPos];
			pCompDesc->SetCaptionEx("%s",0,pObj->pDesc);
		}
		else
		{
			pCompDesc->SetCaption("",1);
		}
	}
}

void RechargeReturn6Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void	msgFunStartTable(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pTurntableCenter->RequestStart();
	}
}

void ClickTurntableBeginButton(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		TurntableMenu* pMenu = (TurntableMenu*)param;
		if(pChar->m_pTurntableCenter->m_nFreeTimes >= 3 && pChar->m_dwRmb < 20)
		{
			pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn6Event,(void*)0,3);
			return;
		}
		if(!pChar->m_pSlaveCenter->CheckCount())
		{
			pMainMenu->PopAskMsgBox("您的宠物数量已达上限，是否继续？",msgFunStartTable,NULL);
			return;
		}
		pChar->m_pTurntableCenter->RequestStart();
	}
}

void	msgFunTenStartTable(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pTurntableCenter->RequestGetTen();
	}
}

void ClickTurntableBeginTenButton(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		TurntableMenu* pMenu = (TurntableMenu*)param;
		if(pChar->m_dwRmb < 20*(10 - (3 - pChar->m_pTurntableCenter->m_nFreeTimes))*80/100)
		{
			pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn6Event,(void*)0,3);
			return;
		}
		if(!pChar->m_pSlaveCenter->CheckCount())
		{
			pMainMenu->PopAskMsgBox("您的宠物数量已达上限，是否继续？",msgFunTenStartTable,NULL);
			return;
		}
		pChar->m_pTurntableCenter->RequestGetTen();
	}
}

void ClickTurntableSeeButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		TurntableMenu* pMenu = (TurntableMenu*)param;
		pMenu->m_pForm2->Open();
	}
}

void ClickTurntableGiftButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int n = (int)param;
	pChar->m_pTurntableCenter->RequestForGift(n);
}

void ClickCloseTurntableGet(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	TurntableMenu* pMenu = (TurntableMenu*)param;
	pMenu->CloseGet();
}

void ClickOKToCloseMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	TurntableMenu* pMenu = (TurntableMenu*)param;
	if (pMenu)
	{
		pMenu->CloseGetTen();
	}
}

TurntableMenu::TurntableMenu()
{
	m_pForm = pMainInterface->GetDForm("zhuanzhuanle");
	m_pForm2 = pMainInterface->GetDForm("leijicishu");
	m_pForm3 = pMainInterface->GetDForm("zzlhuode");
	m_pForm4 = pMainInterface->GetDForm("zzlhuode2");
	m_pPointComp = m_pForm->GetComponent("zhizhen");
	m_pForm->GetComponent("kaishi")->SetEvent(ClickTurntableBeginButton,this);
	m_pForm->GetComponent("kaishi10")->SetEvent(ClickTurntableBeginTenButton,this);
	m_pForm->GetComponent("leiji")->SetEvent(ClickTurntableSeeButton,this);
	for(int i = 0; i < 5; i ++)
	{
		char szName[32];
		sprintf(szName,"get%d",i+1);
		m_pForm2->GetComponent(szName)->SetEvent(ClickTurntableGiftButton,(void*)i);
	}
	m_pForm3->GetComponent("ok")->SetEvent(ClickCloseTurntableGet,this);

	char sztxt[32];
	for (int i=0;i<10;i++)
	{
		sprintf(sztxt,"sk%d",i+1);
		pComGiftItem[i] = m_pForm4->GetComponent(sztxt);
		sprintf(sztxt,"s%d",i+1);
		pComGiftName[i] = m_pForm4->GetComponent(sztxt);
	}
	m_bShowTen = false;
	m_pForm4->GetComponent("ok")->SetEvent(ClickOKToCloseMenu,this);
}

TurntableMenu::~TurntableMenu()
{

}

void ShowSlaveInfoExEvent(InterfaceEvent* pEvent, void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return ;

	pMainMenu->m_pSlaveInfoMenu->OpenByID((int)param);
}

void TurntableMenu::OpenGetTen(BYTE byPos[],int nCnt)
{
	ItemExBase*pBase = NULL;
	for (int i = 0; i < nCnt; i++)
	{
		if(byPos[i] > 0)
		{
			pComGiftItem[i]->SetEffect(0,2);
			TurntableAward award = TurntableCfg::GetInstance().m_szAward[byPos[i]-1];
			char szMsg[128] = {0};
			if(award.nID < 1000)
			{
				pComGiftItem[i]->eventFunc = NULL;
				if(award.nID == 555)
				{
					pComGiftItem[i]->SetViewSpr("MB_ZI_EXP1.png");
					sprintf(szMsg,"经验x%d",award.nNum);
				}
				else if(award.nID == 666)
				{
					pComGiftItem[i]->SetViewSpr("MB_MONEY1.png");
					sprintf(szMsg,"金币x%d",award.nNum);
				}
				else if(award.nID == 888)
				{
					pComGiftItem[i]->SetViewSpr("MB_ZUANSHI1.png");
					sprintf(szMsg,"钻石x%d",award.nNum);
				}
				else if(award.nID == 333)
				{
					pComGiftItem[i]->SetViewSpr("MB_BANGZUAN1.png");
					sprintf(szMsg,"绑钻x%d",award.nNum);
				}
				else if(award.nID == 999)
				{
					pComGiftItem[i]->SetEffect(238,2);
					g_pSlaveMenuCtrl->ShowIcon(pComGiftItem[i],award.nNum,0);
					SlaveBasic* pSBase = SlaveBaseCfg::GetInstance().GetBaseInfo(award.nNum);
					if (pSBase)
						sprintf(szMsg,"宠物%s",pSBase->pName);
					else
						sprintf(szMsg,"");
				}
			}
			else
			{
				ShowItemInfoOnComp(pComGiftItem[i],award.nID,award.nNum);
				pBase = ItemConfig::GetInstance().GetItemBase(award.nID);
				if(pBase)
				{
					sprintf(szMsg,"%sx%d",pBase->szName,award.nNum);
					if (pBase->nStar == 3)
						pComGiftItem[i]->SetEffect(238,2);
					if (pBase->nStar == 2)
						pComGiftItem[i]->SetEffect(223,2);
				}
			}
			pComGiftName[i]->SetCaption(szMsg,1);
		}
	}

	m_pForm4->Open(2);
	pMainMenu->PlayEffect(275);
}

void TurntableMenu::CloseGetTen()
{
	ReFreshGiftTen();
	m_pForm4->Close();
}

void TurntableMenu::ReFreshGiftTen()
{
	for (int i = 0; i < 10; i++)
	{
		pComGiftItem[i]->SetViewSpr("");
		pComGiftName[i]->SetCaption("", 0);
	}
}

void TurntableMenu::OpenGet()
{
	m_pForm3->Open(2);
}

void TurntableMenu::CloseGet()
{
	m_pForm3->Close();
}

void TurntableMenu::Open(int nT)
{
	m_pForm->Open(nT);
	char szIcon[32];
	char szName[32];
	for(int i = 0; i < 8; i ++)
	{
		TurntableAward award = TurntableCfg::GetInstance().m_szAward[i];
		sprintf(szName,"ico%d",i+1);
		DComponent* pComp = m_pForm->GetComponent(szName);
		if(award.nID < 1000)
		{
			if(award.nID == 555)
			{
				pComp->SetViewSpr("MB_ZI_EXP1.png");
			}
			else if(award.nID == 666)
			{
				pComp->SetEffect(236,2);
				pComp->SetViewSpr("MB_MONEY1.png");
			}
			else if(award.nID == 888)
			{
				pComp->SetViewSpr("MB_ZUANSHI1.png");
			}
			else if(award.nID == 333)
			{
				pComp->SetViewSpr("MB_BANGZUAN1.png");
			}
			else if(award.nID == 999)
			{
				pComp->SetEffect(238,2);
				g_pSlaveMenuCtrl->ShowIcon(pComp,award.nNum,0);
				SlaveBasic* pSBase = SlaveBaseCfg::GetInstance().GetBaseInfo(award.nNum);
				if (pSBase)
				{
					char szTmp[32];
					sprintf(szTmp,"MB_PZ%d.png",pSBase->byQuality+1);
					pComp->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
					pComp->SetEvent(ShowSlaveInfoExEvent,(void*)pSBase->nID);
				}
			}

			if (award.nID == 999)
				pComp->SetCaption("",0);
			else
				pComp->SetCaptionEx("%d",2,award.nNum);
		}
		else
		{
			ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(award.nID);
			if (pBase->nStar == 3)
				pComp->SetEffect(238,2);
			if (pBase->nStar == 2)
				pComp->SetEffect(223,2);
			ShowItemInfoOnComp(pComp,award.nID,award.nNum);
		}
	}
}

void TurntableMenu::ActionStart(BYTE byPos)
{
	if(byPos == 0 || byPos > 8)
		return;
	m_pPointComp->byRunAction = 20;
	m_pPointComp->m_pActionParam = (void*)(byPos-1);
}

void TurntableMenu::Update()
{
	DWORD dwNow = xnGetTickCount();
	bool ifShow = false;
	if(pChar->m_pTurntableCenter->m_dwEndTick > dwNow)
	{
		m_pForm->GetComponent("kaishi")->SetEnable(false);
		m_pForm->GetComponent("kaishi10")->SetEnable(false);
	}
	else if(pChar->m_pTurntableCenter->m_dwEndTick > 0 && pChar->m_pTurntableCenter->m_dwEndTick <= dwNow)
	{
		m_pForm->GetComponent("kaishi")->SetEnable(true);
		m_pForm->GetComponent("kaishi10")->SetEnable(true);
		pChar->m_pTurntableCenter->m_dwEndTick = 0;
		ifShow = true;
	}

	if(m_pForm && m_pForm->bOpened)
	{
		pChar->m_pTurntableCenter->RefreshFlag();
		m_pForm->GetComponent("cishu")->SetCaptionEx("%d",1,3-pChar->m_pTurntableCenter->m_nFreeTimes);
		m_pForm->GetComponent("price")->SetCaption("20",1);
		m_pForm->GetComponent("zuan")->SetCaptionEx("%d",1,pChar->m_dwRmb);
		m_pForm->GetComponent("money")->SetCaptionEx("%d",1,pChar->m_dwMoney);

		if(ifShow)
		{
			if (m_bShowTen)
			{
				m_bShowTen = false;
				OpenGetTen(pChar->m_pTurntableCenter->m_byTenPos);
			}
			else
			{
				ItemExBase*pBase = NULL;
				if(pChar->m_pTurntableCenter->m_byPos > 0)
				{
					TurntableAward award = TurntableCfg::GetInstance().m_szAward[pChar->m_pTurntableCenter->m_byPos-1];
					DComponent* pCompI = m_pForm3->GetComponent("icon");
					DComponent* pCompN = m_pForm3->GetComponent("txtC5");
					char szMsg[128] = {0};
					if(award.nID < 1000)
					{
						if(award.nID == 555)
						{
							pCompI->SetViewSpr("MB_ZI_EXP1.png");
							sprintf(szMsg,"获得%d经验",award.nNum);
						}
						else if(award.nID == 666)
						{
							pCompI->SetViewSpr("MB_MONEY1.png");
							sprintf(szMsg,"获得%d金币",award.nNum);
						}
						else if(award.nID == 888)
						{
							pCompI->SetViewSpr("MB_ZUANSHI1.png");
							sprintf(szMsg,"获得%d钻石",award.nNum);
						}
						else if(award.nID == 333)
						{
							pCompI->SetViewSpr("MB_BANGZUAN1.png");
							sprintf(szMsg,"获得%d绑钻",award.nNum);
						}
						else if(award.nID == 999)
						{
							pCompI->SetEffect(238,2);
							g_pSlaveMenuCtrl->ShowIcon(pCompI,award.nNum,0);
							SlaveBasic* pBase = SlaveBaseCfg::GetInstance().GetBaseInfo(award.nNum);
							sprintf(szMsg,"获得宠物%s",pBase->pName);
						}
						if (award.nID == 999)
							pCompI->SetCaption("",0);
						else
							pCompI->SetCaptionEx("%d",2,award.nNum);
						pCompI->eventFunc = NULL;
					}
					else
					{
						ShowItemInfoOnComp(pCompI,award.nID,award.nNum);
						pBase = ItemConfig::GetInstance().GetItemBase(award.nID);
						if(pBase)
							sprintf(szMsg,"获得%s×%d",pBase->szName,award.nNum);
					}
					pCompN->SetCaption(szMsg,1);
					OpenGet();
			    }
			}

		}

		m_pForm->GetComponent("leijicishu")->SetCaptionEx("%d",1,pChar->m_pTurntableCenter->m_nCount);
		m_pForm2->GetComponent("num")->SetCaptionEx("%d",1,pChar->m_pTurntableCenter->m_nCount);
		char szIcon[32];
		char szName[32];
		/*
		for(int i = 0; i < 8; i ++)
				{
					TurntableAward award = TurntableCfg::GetInstance().m_szAward[i];
					sprintf(szName,"ico%d",i+1);
					DComponent* pComp = m_pForm->GetComponent(szName);
					if(award.nID == 555)
					{
						pComp->SetViewSpr("MB_ZI_EXP.png");
					}
					else if(award.nID == 666)
					{
						pComp->SetViewSpr("MB_MONEY.png");
					}
					else if(award.nID == 888)
					{
						pComp->SetViewSpr("MB_ZUANSHI.png");
					}
					else
					{
						ItemExBase*pBase = ItemConfig::GetInstance().GetItemBase(award.nID);
						sprintf(szIcon,"item/%d.png",pBase->nSpr);
						pComp->SetViewSpr(szIcon);
					}
					pComp->SetCaptionEx("%d",2,award.nNum);
				}*/
		
		int szCheck[] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
		m_pForm->GetComponent("leiji")->SetViewSpr("MB_WUJIANGLI.png");
		m_pForm->GetComponent("leiji")->SetCaption("",1);
		bool ifSet = false;
		for(int i = 0; i < 5; i ++)
		{
			TurntableAward* pAward = (TurntableAward*)TurntableCfg::GetInstance().m_pList->Items[i];
			sprintf(szName,"get%d",i+1);
			DComponent* pCompGet = m_pForm2->GetComponent(szName);
			sprintf(szName,"yi%d",i+1);
			DComponent* pCompDone= m_pForm2->GetComponent(szName);
			if(pAward->nVal > pChar->m_pTurntableCenter->m_nCount)
			{
				pCompDone->SetVisible(false);
				pCompGet->SetVisible(true);
				pCompGet->bClickLocked = true;
				pCompGet->SetViewSpr("BT_LINGQU234_H.png");
			}
			else if((pChar->m_pTurntableCenter->m_nGiftFlag&szCheck[i]) == szCheck[i])
			{
				pCompDone->SetVisible(true);
				pCompGet->SetVisible(false);
			}
			else
			{
				pCompDone->SetVisible(false);
				pCompGet->SetVisible(true);
				pCompGet->bClickLocked = false;
				pCompGet->SetViewSpr("BT_LINGQU234.png");
				if(!ifSet)
				{
					ItemExBase*pBase = ItemConfig::GetInstance().GetItemBase(pAward->nID);
					sprintf(szIcon,"item/%d.png",pBase->nSpr);
					m_pForm->GetComponent("leiji")->SetViewSpr(szIcon);
					m_pForm->GetComponent("leiji")->SetCaptionEx("%d",2,pAward->nNum);
					ifSet = true;
				}
			}
		}
	}
}

void ACBossCheckEvent(int nPos,char* name)
{
	if(nPos < ACBossCfg::GetInstance().m_pList->Count)
	{
		if(strcmp(name,"go") == 0)
		{
			pChar->m_pActivityCenter->RequestFind(nPos);
		}
		else
		{
			pMainMenu->m_pACBossMenu->m_nPos = nPos;
		}
	}
}

void ClickpFramDown(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ACBossMenu* pMenu = (ACBossMenu*)param;
		pMenu->m_pForm->pCFrame->nChgY += pMenu->m_pForm->pCFrame->nDisStep;
	}
}

ACBossMenu::ACBossMenu():m_nPos(0)
{
	m_pForm = pMainInterface->GetDForm("boss");
	DComponent* pBaseCom = m_pForm->GetComponent("zi");
	m_pForm->pCFrame = new CCtrlFrame(pBaseCom->nX,pBaseCom->nY,pBaseCom->nW,pBaseCom->nH*3);
	DComponent* ppComp[8];
	ppComp[0] = m_pForm->GetComponent("name");
	ppComp[1] = m_pForm->GetComponent("lv");
	ppComp[2] = m_pForm->GetComponent("time");
	ppComp[3] = m_pForm->GetComponent("go");
	ppComp[4] = m_pForm->GetComponent("mb");
	ppComp[5] = m_pForm->GetComponent("x1");
	ppComp[6] = m_pForm->GetComponent("gezi");
	ppComp[7] = m_pForm->GetComponent("icon");
	m_pForm->pCFrame->InitFParts(pBaseCom,ppComp,8);
	m_pForm->pCFrame->nDisStep = pBaseCom->nH;
	m_pForm->pCFrame->SetEvent(ACBossCheckEvent);

	m_pStateList = xnList::Create();

	m_pForm->GetComponent("down")->SetEvent(ClickpFramDown,this);
}

ACBossMenu::~ACBossMenu()
{
	while(m_pStateList->Count)
		delete m_pStateList->Delete(0);
	m_pStateList->Free();
}

void ACBossMenu::Open(int nT)
{
	m_pForm->Open(nT);
	while(m_pStateList->Count)
		m_pStateList->Delete(0);
	pChar->m_pActivityCenter->RequestBossInfo();

	xnList* pList = ACBossCfg::GetInstance().m_pList;
	if (!pList)
		return;
	m_pForm->pCFrame->SetFPCnt(pList->Count);
	for (int i = 0; i < pList->Count; i++)
	{
		BossInfoObj* pInfo = (BossInfoObj*)pList->Items[i];
		m_pForm->pCFrame->GetFPComp(i,"name")->SetCaptionEx("%s",0,pInfo->pName);
		m_pForm->pCFrame->GetFPComp(i,"lv")->SetCaptionEx("%d",1,pInfo->nLv);
		char szIcon[32];
		sprintf(szIcon,"activity/%d.png",pInfo->pIcon);
		m_pForm->pCFrame->GetFPComp(i,"icon")->SetViewSpr(szIcon);
		m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
	}
}

void ACBossMenu::Update()
{
	if (!m_pForm->bOpened)
		return;
	xnList* pList = ACBossCfg::GetInstance().m_pList;
	if (!pList)
		return;
	for (int i = 0; i < pList->Count; i++)
	{
		m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
	}
	if (m_pStateList->Count)
	{
		for (int j = 0; j < pList->Count; j++)
		{
			BossInfoObj* pTempInfo = (BossInfoObj*)pList->Items[j];
			for (int m = 0; m < m_pStateList->Count; m++)
			{
				if (pTempInfo->nID == (DWORD)m_pStateList->Items[m])
				{
					m_pForm->pCFrame->GetFPComp(j,"time")->SetCaption("活跃",1);
					break;
				}
				else
					m_pForm->pCFrame->GetFPComp(j,"time")->SetCaption("死亡",1);
			}
		}
	}else
	{
		for (int k = 0; k < pList->Count; k++)
		{
			m_pForm->pCFrame->GetFPComp(k,"time")->SetCaption("死亡",1);
		}
	}
	if (m_nPos>=0&&m_nPos<pList->Count)
	{
		m_pForm->pCFrame->GetFPComp(m_nPos,"mb")->SetVisible(true);
		BossInfoObj* pInfo = (BossInfoObj*)pList->Items[m_nPos];
		m_pForm->GetComponent("ms")->SetCaptionEx("%s",0,pInfo->pDesc);
	}else
	{
		m_pForm->GetComponent("ms")->SetCaption("",1);
	}
}

void GetAppraiseAward(InterfaceEvent* pEvent, void* param)
{
	if (pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	g_pTPObj->TP_OtherEvent(1,ServerLineCfg::GetInstance().m_szAppUrl);
	pChar->SendCmd(USER,C_GET_APPRS_AWARD);
}

AppraiseMenu::AppraiseMenu()
{
	m_pForm = pMainInterface->GetDForm("pinglun");
	if (m_pForm)
	{
		m_pForm->GetComponent("go")->SetEvent(GetAppraiseAward);
	}
}

AppraiseMenu::~AppraiseMenu()
{

}

void AppraiseMenu::Update()
{

}

void AppraiseMenu::Open(int nT)
{
	if (m_pForm)
	{
		DComponent* pComp = m_pForm->GetComponent("icon1");
		if (pComp)
		{
			pComp->SetViewSpr("MB_BANGZUAN1.png");
			pComp->SetCaptionEx("%d",2,300);
		}
		pComp = m_pForm->GetComponent("icon2");
		if (pComp)
		{
			pComp->SetEffect(238,2);
			ShowItemInfoOnComp(pComp,45011,5);
		}

		m_pForm->Open(nT);
	}
}

NoticeMenu::NoticeMenu()
{
	m_pForm = pMainInterface->GetDForm("hdgx");
}

NoticeMenu::~NoticeMenu()
{

}

void NoticeMenu::Open(int nT)
{
	if (m_pForm)
	{
		pChar->SendCmd(USER,C_GET_NOTICE);
		m_pForm->Open();
	}
}

void NoticeMenu::InitNotice(char* szNotice)
{
	if (m_pForm)
	{
		DComponent* pComp = m_pForm->GetComponent("txt");
		pComp->pSTextCenter->ClearSText();
		if(strlen(szNotice) == 0)
		{
			strNotice.assign("");
			pComp->pSTextCenter->AddSText(1,0,true,(char*)"暂无更新",255,255,255);
		}
		else
		{
			strNotice.assign(szNotice);
			std::vector<std::string> vecTemp;
			StringSplit(strNotice,"|",vecTemp);
			for(int i = 0; i < vecTemp.size(); i ++)
			{
				pComp->pSTextCenter->AddSText(i,0,true,(char*)vecTemp[i].c_str(),255,255,255);
			}
		}
	}
}

void ActivityJoinButtonEvent(int nPos,char* name)
{
	if(nPos < pChar->m_pActivityCenter->m_pJoinList->Count)
	{
		if(strcmp(name,"go") == 0)
		{
			pChar->m_pActivityCenter->RequestEnterEx(nPos);
		}
		else
		{
			pMainMenu->m_pActivityJoinMenu->m_nPos = nPos;
		}
	}
}

void ClickJoinFramDown(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ActivityJoinMenu* pMenu = (ActivityJoinMenu*)param;
		pMenu->m_pForm->pCFrame->nChgY += pMenu->m_pForm->pCFrame->nDisStep;
	}
}

ActivityJoinMenu::ActivityJoinMenu():m_nPos(0)
{
	m_pForm = pMainInterface->GetDForm("huodong2");

	DComponent* pBaseCom = m_pForm->GetComponent("zi");
	DComponent* ppComp[9];
	m_pForm->pCFrame = new CCtrlFrame(pBaseCom->nX,pBaseCom->nY,pBaseCom->nW,pBaseCom->nH*3);
	ppComp[0] = m_pForm->GetComponent("name");
	ppComp[1] = m_pForm->GetComponent("lv");
	ppComp[2] = m_pForm->GetComponent("time");
	ppComp[3] = m_pForm->GetComponent("go");
	ppComp[4] = m_pForm->GetComponent("mb");
	ppComp[5] = m_pForm->GetComponent("x1");
	ppComp[6] = m_pForm->GetComponent("gezi");
	ppComp[7] = m_pForm->GetComponent("icon");
	ppComp[8] = m_pForm->GetComponent("leixing");
	m_pForm->pCFrame->InitFParts(pBaseCom,ppComp,9);
	m_pForm->pCFrame->nDisStep = pBaseCom->nH;
	m_pForm->pCFrame->SetEvent(ActivityJoinButtonEvent);
	m_pForm->pCFrame->SetFPCnt(0);

	m_pForm->GetComponent("down")->SetEvent(ClickJoinFramDown,this);
}

ActivityJoinMenu::~ActivityJoinMenu()
{

}

void ActivityJoinMenu::OpenFrom()
{
	xnList* pList = pChar->m_pActivityCenter->m_pJoinList;
	m_pForm->pCFrame->SetFPCnt(pList->Count);
	for(int i = 0; i < pList->Count; i ++)
	{
		ActivityObj* pObj = (ActivityObj*)pList->Items[i];
		DComponent* pComp = m_pForm->pCFrame->GetFPComp(i,"name");
		if (pObj->nFlag)
		{
			pComp->SetCaptionEx("%s(%d/%d)",1,pObj->pName,pChar->m_pClientFlag->GetFlag(pObj->nFlag),pObj->nMaxnum);
		}else
			pComp->SetCaptionEx("%s",1,pObj->pName);
		//pComp->SetCaptionEx("%s",1,pObj->pName);
		pComp = m_pForm->pCFrame->GetFPComp(i,"icon");
		pComp->SetViewSpr(pObj->pIcon);
		pComp = m_pForm->pCFrame->GetFPComp(i,"leixing");
		if (pObj->nAwardType == 1)
			pComp->SetViewSpr("MB_HD_EXP.png");
		else if (pObj->nAwardType == 2)
			pComp->SetViewSpr("MB_HD_MONEY.png");
		else if (pObj->nAwardType == 3)
			pComp->SetViewSpr("MB_HD_DAOJU.png");
		else
			pComp->SetViewSpr("");
		pComp = m_pForm->pCFrame->GetFPComp(i,"lv");
		if(pObj->nLvMin == 0 && pObj->nLvMax == 0)
			pComp->SetCaption("无",1);
		else if(pObj->nLvMin > 0 && pObj->nLvMax == 0)
			pComp->SetCaptionEx("%d级以上",1,pObj->nLvMin);
		else if(pObj->nLvMin == 0 && pObj->nLvMax > 0)
			pComp->SetCaptionEx("%d级以下",1,pObj->nLvMax);
		else
			pComp->SetCaptionEx("%d级~%d级",1,pObj->nLvMin,pObj->nLvMax);
		pComp = m_pForm->pCFrame->GetFPComp(i,"time");
		char* szWeekName[] = {"周一","周二","周三","周四","周五","周六","周日"};
		if(pObj->pWeekList->Count == 7)
			pComp->SetCaptionEx("每天%02d:%02d到%02d:%02d",1,pObj->dwStartTime>>16,pObj->dwStartTime&0xFFFF,pObj->dwEndTime>>16,pObj->dwEndTime&0xFFFF);
		else
		{
			std::string strMsg("每");
			for(int j = 0; j < pObj->pWeekList->Count; j ++)
			{
				int n = (int)pObj->pWeekList->Items[j];
				strMsg.append(szWeekName[n-1]);
				if(j != pObj->pWeekList->Count-1)
					strMsg.append("、");
			}
			char szTemp[32];
			sprintf(szTemp,"%02d:%02d到%02d:%02d",pObj->dwStartTime>>16,pObj->dwStartTime&0xFFFF,pObj->dwEndTime>>16,pObj->dwEndTime&0xFFFF);
			strMsg.append(szTemp);
			pComp->SetCaptionEx("%s",1,strMsg.c_str());
		}
		pComp = m_pForm->pCFrame->GetFPComp(i,"go");
		if(pObj->byActionEvent == 0)
			pComp->SetVisible(false);
		else
			pComp->SetVisible(true);
	}

	m_pForm->Open();
}

void ActivityJoinMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;

	for(int i = 0; i < pChar->m_pActivityCenter->m_pJoinList->Count; i ++)
	{
		DComponent* pCompChoose = m_pForm->pCFrame->GetFPComp(i,"mb");
		if(pCompChoose == NULL)
			continue;
		if(i == m_nPos)
			pCompChoose->SetVisible(true);
		else
			pCompChoose->SetVisible(false);
	}
	DComponent* pCompDesc = m_pForm->GetComponent("ms");
	if(pCompDesc == NULL)
		return;
	if(m_nPos < pChar->m_pActivityCenter->m_pJoinList->Count)
	{
		ActivityObj* pObj = (ActivityObj*)pChar->m_pActivityCenter->m_pJoinList->Items[m_nPos];
		pCompDesc->SetCaptionEx("%s",0,pObj->pDesc);
	}
	else
	{
		pCompDesc->SetCaption("",1);
	}
}