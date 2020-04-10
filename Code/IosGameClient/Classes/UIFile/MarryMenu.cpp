#include "UserData.h"
#include "MarryMenu.h"
#include "ios_relation_packet.h"
#include "MentoringCfg.h"
#include "MainMenu.h"
#include "GameAwardCfg.h"
#include "MapStaticCfg.h"
#include "MapMouse.h"

extern char* szJobTxt[];
extern char* szCountryTxt[];

void	MarryCFClickEvent(int nPos,char*pName,void* param)
{
	if(param)
	{
		MarryListMenu* pMenu = (MarryListMenu*)param;
		pMenu->nPos = nPos;
	}
}

void	MakesureMarryInstruct(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pMarryCenter->RequestInstruct();
	}
}

void	ClickMarryInstructButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->PopAskMsgBox("征婚后，您可以等待有缘人出现，是否确定征婚？",MakesureMarryInstruct,NULL);
	}
}

void	MakesureCancelInstruct(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pMarryCenter->CancelInstruct();
	}
}

void	ClickCancelInstructButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->PopAskMsgBox("是否取消征婚？",MakesureCancelInstruct,NULL);
	}
}

void	ClickMarryTalkButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MarryListMenu* pMenu = (MarryListMenu*)param;
		if(pMenu->nPos >= 0 && pMenu->nPos < pChar->m_pMarryCenter->m_pRequestList->Count)
		{
			MCRequest* pData = (MCRequest*)pChar->m_pMarryCenter->m_pRequestList->Items[pMenu->nPos];
			if(pData->dwPuin == pChar->m_dwUin)
				return;
			pMenu->Close();
			pMainMenu->m_pChatMenu->Open();
			pMainMenu->m_pChatMenu->SetTargent(pData->szPName);
// 			BuildPacketEx(pPacket,RPGRELATION,MARRY_SELT_TEST,buf,1024);
// 			SET_DATA(pSend,RPGRELATION,MARRY_SELT_TEST,pPacket);
// 			strcpy(pSend->szname,pData->szPName);
// 			pChar->SendPacket(pPacket);
		}
	}
}

MarryListMenu::MarryListMenu()
{
	m_pForm = pMainInterface->GetDForm("jiehun1");
	DComponent* pBaseComp = m_pForm->GetComponent("n1");
	DComponent* ppComp[7];
	ppComp[0] = m_pForm->GetComponent("xz");
	ppComp[1] = m_pForm->GetComponent("x1");
	ppComp[2] = m_pForm->GetComponent("name");
	ppComp[3] = m_pForm->GetComponent("job");
	ppComp[4] = m_pForm->GetComponent("lv");
	ppComp[5] = m_pForm->GetComponent("org");
	ppComp[6] = m_pForm->GetComponent("gender");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*6);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,7);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetEventEx(MarryCFClickEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);
	m_pForm->GetComponent("st")->SetEvent(ClickMarryInstructButton,this);
	m_pForm->GetComponent("qx")->SetEvent(ClickCancelInstructButton,this);
	m_pForm->GetComponent("sl")->SetEvent(ClickMarryTalkButton,this);
}

MarryListMenu::~MarryListMenu()
{

}

void	MarryListMenu::Open(int nT)
{
	if(pChar->m_pMarryCenter->m_pRelationList->Count > 0)
	{
		pMainMenu->m_pMarryCtrl->Open(nT,0);
	}
	else
	{
		pChar->m_pMarryCenter->GetRequestList();
		m_pForm->Open(nT);
	}
}

void	MarryListMenu::Close()
{
	m_pForm->Close();
}

void	MarryListMenu::Update()
{
	if(m_pForm->bOpened)
	{
		DComponent* pComp = NULL;
		if(pChar->m_pMarryCenter->m_ifRChg)
		{
			pChar->m_pMarryCenter->m_ifRChg = false;
			int nCnt = pChar->m_pMarryCenter->m_pRequestList->Count;
			m_pForm->pCFrame->SetFPCnt(nCnt);
			for(int i = 0; i < nCnt; i ++)
			{
				MCRequest* pData = (MCRequest*)pChar->m_pMarryCenter->m_pRequestList->Items[i];
				pComp = m_pForm->pCFrame->GetFPComp(i,"name");
				pComp->SetCaptionEx("%s",1,pData->szPName);
				pComp = m_pForm->pCFrame->GetFPComp(i,"job");
				pComp->SetCaptionEx("%s",1,szJobTxt[pData->bPJob]);
				pComp = m_pForm->pCFrame->GetFPComp(i,"lv");
				pComp->SetCaptionEx("%d",1,pData->bPLvl);
				pComp = m_pForm->pCFrame->GetFPComp(i,"org");
				pComp->SetCaptionEx("%s",1,szCountryTxt[pChar->m_byCountry]);
				pComp = m_pForm->pCFrame->GetFPComp(i,"gender");
				pComp->SetCaptionEx("%s",1,pData->bSex==1?"男":"女");
			}
		}
		if(!pChar->m_pMarryCenter->m_ifInstruct)
		{
			m_pForm->GetComponent("st")->SetVisible(true);
			m_pForm->GetComponent("qx")->SetVisible(false);
		}
		else
		{
			m_pForm->GetComponent("st")->SetVisible(false);
			m_pForm->GetComponent("qx")->SetVisible(true);
		}
		int nPCount = m_pForm->pCFrame->GetFPCnt();
		for(int i = 0; i < nPCount; i ++)
		{
			if(i >= pChar->m_pMarryCenter->m_pRequestList->Count)
				break;
			MCRequest* pData = (MCRequest*)pChar->m_pMentoringClient->m_pTeacherList->Items[i];
			pComp = m_pForm->pCFrame->GetFPComp(i,"xz");
			if(i == nPos)
				pComp->SetVisible(true);
			else
				pComp->SetVisible(false);
		}
	}
}

void	ClickMInfoValueButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return ;
	if(param)
	{
		MarryInfoMenu* pMenu = (MarryInfoMenu*)param;
		pMainMenu->m_pSendRingMenu->Open();
	}
}

void	ClickShowMTitleButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MarryInfoMenu* pMenu = (MarryInfoMenu*)param;
		if(strcmp(pEvent->pComponent->szName,"xs") == 0)
			pChar->m_pMarryCenter->ShowMarryTitle(true);
		else
			pChar->m_pMarryCenter->ShowMarryTitle(false);
	}
}

void	ClickBreakMRelationButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
 		MarryInfoMenu* pMenu = (MarryInfoMenu*)param;
 		pChar->m_pMarryCenter->RequestBreakRelation();
//		pChar->SendCmd(RPGRELATION,MARRY_SELT_TEST);
	}
}

void	ClickMTeamButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		ClientMarryData* pMarry = pChar->m_pMarryCenter->GetRelation();
		if(pMarry)
		{
			int nID = pMarry->dwWoman;
			if(pChar->m_dwUin == pMarry->dwWoman)
				nID = pMarry->dwMan;
			pChar->InvitePlayer(nID);
		}
	}
}
void	ClickMarryCopyButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ENTERAC,buf,1024);
		SET_DATA(pData,RPGACTIVITY,C_REQUEST_ENTERAC,pPacket);
		pData->nID = 1028;
		pChar->SendPacket(pPacket);
	}
}

MarryInfoMenu::MarryInfoMenu()
{
	m_pForm = pMainInterface->GetDForm("jiehun2");
	m_pForm->GetComponent("add")->SetEvent(ClickMInfoValueButton,this);
	m_pForm->GetComponent("xs")->SetEvent(ClickShowMTitleButton,this);
	m_pForm->GetComponent("yc")->SetEvent(ClickShowMTitleButton,this);
	m_pForm->GetComponent("lh")->SetEvent(ClickBreakMRelationButton,this);
	m_pForm->GetComponent("zd")->SetEvent(ClickMTeamButton,this);
	m_pForm->GetComponent("fb")->SetEvent(ClickMarryCopyButton,this);
}

MarryInfoMenu::~MarryInfoMenu()
{

}

char*	szMTitle[] = {"","未婚夫","丈夫","模范丈夫"};
char*	szWTitle[] = {"","未婚妻","妻子","模范妻子"};

void	MarryInfoMenu::Update()
{
	if(m_pForm->bOpened)
	{
		ClientMarryData* pData = pChar->m_pMarryCenter->GetRelation();
		if(pData)
		{
			DComponent* pComp = NULL;
			if(pData->dwMan == pChar->m_dwUin)
			{
				pComp = m_pForm->GetComponent("name");
				pComp->SetCaptionEx("%s",1,pData->szWName);
				pComp = m_pForm->GetComponent("zy");
				pComp->SetCaptionEx("%s",1,szJobTxt[pData->bWJob]);
				pComp = m_pForm->GetComponent("dj");
				pComp->SetCaptionEx("%d",1,pData->bWLvl);
				pComp = m_pForm->GetComponent("jt");
				pComp->SetCaptionEx("%s",1,pData->szWHomeName);
				pComp = m_pForm->GetComponent("yh");
				pComp->SetCaptionEx("%d",1,pData->nMValue);
				pComp = m_pForm->GetComponent("ch");
				pComp->SetCaptionEx("%s",1,szWTitle[pData->byState]);
				pComp = m_pForm->GetComponent("qm");
				pComp->SetCaptionEx("%d",1,pData->nMpoint);
			}
			else
			{
				pComp = m_pForm->GetComponent("name");
				pComp->SetCaptionEx("%s",1,pData->szMName);
				pComp = m_pForm->GetComponent("zy");
				pComp->SetCaptionEx("%s",1,szJobTxt[pData->bMJob]);
				pComp = m_pForm->GetComponent("dj");
				pComp->SetCaptionEx("%d",1,pData->bMLvl);
				pComp = m_pForm->GetComponent("jt");
				pComp->SetCaptionEx("%s",1,pData->szMHomeName);
				pComp = m_pForm->GetComponent("yh");
				pComp->SetCaptionEx("%d",1,pData->nWValue);
				pComp = m_pForm->GetComponent("ch");
				pComp->SetCaptionEx("%s",1,szMTitle[pData->byState]);
				pComp = m_pForm->GetComponent("qm");
				pComp->SetCaptionEx("%d",1,pData->nWpoint);
			}
		}
		else
		{
			m_pForm->GetComponent("name")->SetCaption("无",1);
			m_pForm->GetComponent("zy")->SetCaption("无",1);
			m_pForm->GetComponent("dj")->SetCaption("无",1);
			m_pForm->GetComponent("jt")->SetCaption("无",1);
			m_pForm->GetComponent("yh")->SetCaption("无",1);
			m_pForm->GetComponent("ch")->SetCaption("无",1);
			m_pForm->GetComponent("qm")->SetCaption("无",1);
		}
		m_pForm->GetComponent("xs")->SetVisible(!pChar->m_pMarryCenter->m_ifShowTitle);
		m_pForm->GetComponent("yc")->SetVisible(pChar->m_pMarryCenter->m_ifShowTitle);
	}
}

void	MarrySkillClickEvent(int nPos,char*pName,void* param)
{
	if(param)
	{
		if(strcmp(pName,"learn") == 0)
		{
			pChar->m_pMarryCenter->RequestUpSkill(nPos);
		}
		else
		{
			MarrySkillMenu* pMenu = (MarrySkillMenu*)param;
			pMenu->m_nPos = nPos;
		}
		
	}
}

char*	szSkillInfo[] = {"增加生命上限","增加物理攻击","增加物理防御","增加法术攻击","增加法术防御"};

MarrySkillMenu::MarrySkillMenu()
{
	m_pForm = pMainInterface->GetDForm("jiehun3");
	DComponent* pBaseComp = m_pForm->GetComponent("zi");
	DComponent* ppComp[9];
	ppComp[0] = m_pForm->GetComponent("gezi");
	ppComp[1] = m_pForm->GetComponent("icon");
	ppComp[2] = m_pForm->GetComponent("ms");
	ppComp[3] = m_pForm->GetComponent("name");
	ppComp[4] = m_pForm->GetComponent("num");
	ppComp[5] = m_pForm->GetComponent("learn");
	ppComp[6] = m_pForm->GetComponent("yh");
	ppComp[7] = m_pForm->GetComponent("x1");
	ppComp[8] = m_pForm->GetComponent("mb");
	m_pForm->pCFrame = new CCtrlFrame(pBaseComp->nX,pBaseComp->nY,pBaseComp->nW,pBaseComp->nH*4);
	m_pForm->pCFrame->InitFParts(pBaseComp,ppComp,9);
	m_pForm->pCFrame->nDisStep = pBaseComp->nH;
	m_pForm->pCFrame->SetEventEx(MarrySkillClickEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);
}

MarrySkillMenu::~MarrySkillMenu()
{

}

void	MarrySkillMenu::Open(int nT)
{
	xnList* pList = MarrySkillCfg::GetInstance().m_pList;
	m_pForm->pCFrame->SetFPCnt(pList->Count);
	for(int i = 0; i < pList->Count; i ++)
	{
		MarrySKObj* pObj = (MarrySKObj*)pList->Items[i];
		m_pForm->pCFrame->GetFPComp(i,"name")->SetCaptionEx("%s",1,pObj->pName);
		m_pForm->pCFrame->GetFPComp(i,"icon")->SetViewSpr(pObj->pIcon);
	}
	m_pForm->Open(nT);
}

void	MarrySkillMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		ClientMarryData* pData = pChar->m_pMarryCenter->GetRelation();
		xnList* pList = MarrySkillCfg::GetInstance().m_pList;
		if(pData)
		{
			for(int i = 0; i < pList->Count; i ++)
			{
				MarrySKObj* pObj = (MarrySKObj*)pList->Items[i];
				int nLv = 0;
				if(pData->dwMan == pChar->m_dwUin)
					nLv = pData->szMSkill[i];
				else
					nLv = pData->szWSkill[i];
				_simpleAward* pS = NULL;
				if(nLv >= pObj->pLvList->Count)
					pS = (_simpleAward*)pObj->pLvList->Items[pObj->pLvList->Count-1];
				else
					pS = (_simpleAward*)pObj->pLvList->Items[nLv];
				m_pForm->pCFrame->GetFPComp(i,"ms")->SetCaptionEx("%s%d点",0,szSkillInfo[pObj->byType],pS->nNum);
				m_pForm->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",0,pS->nID);
				m_pForm->pCFrame->GetFPComp(i,"learn")->SetVisible(nLv >= pObj->pLvList->Count ? false : true);
				m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(m_nPos == i ? true : false);
			}
			if(pData->dwMan == pChar->m_dwUin)
				m_pForm->GetComponent("num1")->SetCaptionEx("%d",1,pData->nMValue);
			else
				m_pForm->GetComponent("num1")->SetCaptionEx("%d",1,pData->nWValue);
		}
		else
		{
			for(int i = 0; i < pList->Count; i ++)
			{
				MarrySKObj* pObj = (MarrySKObj*)pList->Items[i];
				_simpleAward* pS = (_simpleAward*)pObj->pLvList->Items[0];
				m_pForm->pCFrame->GetFPComp(i,"ms")->SetCaptionEx("%s%d点",0,szSkillInfo[pObj->byType],pS->nNum);
				m_pForm->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",0,pS->nID);
				m_pForm->pCFrame->GetFPComp(i,"learn")->SetVisible(false);
				m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
			}
			m_pForm->GetComponent("num1")->SetCaption("0",1);
		}
	}
}

static void MarryMissionAcceptEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(28);
	if(!nQuestID)
		return;

	pChar->SendCmd(RPGRELATION,C_MARRY_QUEST_ACCEPT);
}

static void MarryMissionCompleteEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(28);
	if(!nQuestID)
		return;

	pChar->SendCmd(RPGRELATION,C_MARRY_QUEST_COMPLETE);

}

static void MarryMissionGiveUpEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(28);
	if(!nQuestID)
		return;
	pChar->SendCmd(RPGRELATION,C_MARRY_QUEST_GIVEUP);
}

static void MarryMissionGotoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(28);
	if(!nQuestID)
		return;
	ClientQuest* pCQ	= pChar->m_pCQuestCenter->GetCurQuest(nQuestID);
	QuestObj* pObj = QuestCfg::GetInstance().GetQuestByID(nQuestID);
	if (!pObj)
	{
		return;
	}
	int		nDestNpc	= 0;
	if(pCQ)
	{
		if(pCQ->bOk)
			nDestNpc	= pCQ->pQBase->wNpcEnd;
	}
	else
	{
		nDestNpc	= pObj->wNpcSrc;
	}
	if(nDestNpc)
	{
		PathObj*	pNpc	= NpcPathLeadCfg::GetInstance().GetNpcPath(nDestNpc);
		if(pNpc)
		{
			g_pMapCommand->CmdPathObj(pNpc);
		}
		pMainMenu->m_pFamilyQuestMenu->m_pForm->Close();
	}

}
MarryQuestMenu::MarryQuestMenu()
{
	m_pForm = pMainInterface->GetDForm ("jiehun4");
	m_pForm->GetComponent("bt1")->SetEvent(MarryMissionAcceptEvent,this);
	m_pForm->GetComponent("bt2")->SetEvent(MarryMissionCompleteEvent,this);
	m_pForm->GetComponent("bt3")->SetEvent(MarryMissionGiveUpEvent,this);
	m_pForm->GetComponent("bt4")->SetEvent(MarryMissionGotoEvent,this);


}

MarryQuestMenu::~MarryQuestMenu()
{

}

void MarryQuestMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
//	if (m_bUpdate)
	{
		m_bUpdate = false;
		int nQuestID = pChar->m_pClientFlag->GetFlag(28);
		int nQuestTimes = pChar->m_pClientFlag->GetFlag(27);
		QuestObj* pObj = QuestCfg::GetInstance().GetQuestByID(nQuestID);
		
		if (pObj)
		{
			UpViewQuestComp(m_pForm,pObj,NULL);

			m_pForm->GetComponent("exp")->SetCaptionEx("%d",1,pObj->nAwardExp);
			m_pForm->GetComponent("money")->SetCaption("20",1);
			m_pForm->GetComponent("zi2")->SetCaptionEx("%s",0,pObj->szName);

			m_pForm->GetComponent("bt1")->SetVisible(false);
			m_pForm->GetComponent("bt2")->SetVisible(false);
			m_pForm->GetComponent("bt3")->SetVisible(false);
			m_pForm->GetComponent("bt4")->SetVisible(false);
			ClientQuest* pCQ	= pChar->m_pCQuestCenter->GetCurQuest(nQuestID);
			if(pCQ)
			{
				if(pCQ->bOk)
				{
					if(pCQ->pQBase->wNpcEnd)
						m_pForm->GetComponent("bt4")->SetVisible(true);
					else
						m_pForm->GetComponent("bt2")->SetVisible(true);
				}
				else
					m_pForm->GetComponent("bt3")->SetVisible(true);
			}
			else
			{
				if(pObj->wNpcSrc)
					m_pForm->GetComponent("bt4")->SetVisible(true);
				else
					m_pForm->GetComponent("bt1")->SetVisible(true);
			}

			if (pObj->bLvNeed)
			{
				char	szTmp[32];
				sprintf(szTmp,"[%d级]",pObj->bLvNeed);
				m_pForm->GetComponent("zi")->SetCaption(szTmp,0);
			}else
				m_pForm->GetComponent("zi")->SetCaption("",0);
		}

	}
}

void MarryQuestMenu::Open( int nT )
{
	m_bUpdate = true;
	m_pForm->Open (nT);
}

void MarryQuestMenu::UpViewQuestComp( DForm* pForm, QuestObj* pQBase, ClientQuest* pClientQ )
{
	if(pQBase == NULL)
	{
		pForm->GetComponent("zi1")->SetVisible(false);
		// 		pForm->GetComponent("gezi1")->SetVisible(false);
		// 		pForm->GetComponent("gezi2")->SetVisible(false);
		// 		pForm->GetComponent("icon1")->SetVisible(false);
		// 		pForm->GetComponent("icon2")->SetVisible(false);
		pForm->GetComponent("zi2C8")->SetVisible(false);
		pForm->GetComponent("zi2")->SetVisible(false);
		pForm->GetComponent("di2")->SetVisible(false);
		pForm->GetComponent("rw")->SetVisible(false);
		pForm->GetComponent("exp")->SetVisible(false);
		pForm->GetComponent("money")->SetVisible(false);
		return;
	}
	else
	{
		pForm->GetComponent("zi1")->SetVisible(true);
		// 		pForm->GetComponent("gezi1")->SetVisible(true);
		// 		pForm->GetComponent("gezi2")->SetVisible(true);
		// 		pForm->GetComponent("icon1")->SetVisible(true);
		// 		pForm->GetComponent("icon2")->SetVisible(true);
		pForm->GetComponent("zi2C8")->SetVisible(true);
		pForm->GetComponent("zi2")->SetVisible(true);
		pForm->GetComponent("di2")->SetVisible(true);
		pForm->GetComponent("rw")->SetVisible(true);		
		pForm->GetComponent("exp")->SetVisible(true);
		pForm->GetComponent("money")->SetVisible(true);
	}

	DComponent*	pCompCond	= pForm->GetComponent("zi1");
	pCompCond->pSTextCenter->m_byFontSize	= 18;
	pCompCond->pSTextCenter->ClearSText();
	char	szChg[1024];
	memset(szChg,0,1024);
	if(gfun_ReplaceTxtName(pQBase->szDescEnd,szChg))
	{
		pCompCond->pSTextCenter->AddSText(1,0,true,szChg,255,255,255);
	}
	else
		pCompCond->pSTextCenter->AddSText(1,0,true,pQBase->szDescEnd,10,220,10);

	int nIndex	= 2;
	for (int i = 0; i < pQBase->pLQCondtion->Count; i++)
	{
		_QConditon* pQC	= (_QConditon*)pQBase->pLQCondtion->Items[i];
		char	szTxt[256];
		if(pQC->bDType	!= 2&&pQC->bDType != 3)
		{
			if(pClientQ == NULL)
				sprintf(szTxt,"%s:  %d",pQC->szTitle,pQC->wDNum);
			else if(pClientQ->pszVal[pQC->bValPos] < pQC->wDNum)
				sprintf(szTxt,"%s:  %d",pQC->szTitle/*,pClientQ->pszVal[pQC->bValPos]*/,pQC->wDNum);
			else
				sprintf(szTxt,"%s:  %d",pQC->szTitle/*,pQC->wDNum*/,pQC->wDNum);
		}
		else
			sprintf(szTxt,"物品配置");

		pCompCond->pSTextCenter->AddSText(nIndex++,0,true,szTxt,10,220,10);
	}

	//pForm->GetComponent("exp")->SetCaptionEx("%d",1,pQBase->nAwardExp);
	//pForm->GetComponent("money")->SetCaptionEx("%d",1,pQBase->nAwardGold);



	DComponent*	pDesc	= pForm->GetComponent("zi2C8");
	if(pDesc && pDesc->pSTextCenter)
	{
		pDesc->pSTextCenter->ClearSText();

		char	szChg[1024];
		memset(szChg,0,1024);
		if(gfun_ReplaceTxtName(pQBase->szDescSrc,szChg))
		{
			pDesc->pSTextCenter->AddSText(1,0,false,szChg,255,255,255);
		}
		else
			pDesc->pSTextCenter->AddSText(1,0,false,pQBase->szDescSrc,255,255,255);

	}

	DComponent*	pTalk	= pForm->GetComponent("zi2");
	xnList*	pLTmpTalk	= pQBase->pLTalkSrc;
	if(pClientQ && pClientQ->bOk)
		pLTmpTalk	= pQBase->pLTalkEnd;

	if(pTalk && pTalk->pSTextCenter && pLTmpTalk->Count)
	{
		pTalk->pSTextCenter->ClearSText();
		char	szChg[1024];
		memset(szChg,0,1024);
		if(gfun_ReplaceTxtName((char*)pLTmpTalk->Items[0],szChg))
		{
			pTalk->pSTextCenter->AddSText(1,0,false,szChg,255,255,255);
		}
		else
			pTalk->pSTextCenter->AddSText(1,0,false,(char*)pLTmpTalk->Items[0],255,255,255);
	}
}

void	ClickMAskOkButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MarryAskMenu* pMenu = (MarryAskMenu*)param;
		pMenu->ClickAnswer(true);
	}
}

void	ClickMAskCancelButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MarryAskMenu* pMenu = (MarryAskMenu*)param;
		pMenu->ClickAnswer(false);
	}
}

MarryAskMenu::MarryAskMenu()
{
	m_pForm = pMainInterface->GetDForm("junhunmes");
	Func_CBMsgBox = NULL;
	pParamMsgbox = NULL;
	m_pCompTitle	= m_pForm->GetComponent ("title");
	m_pCompTitle->m_nFontSize	= 22;
	m_pCompTitle->SetSprColor(200,180,33);
	m_pCompMsg		= m_pForm->GetComponent ("txt");
	m_pCompMsg->m_nFontSize	= 22;
	m_pCompMsg->SetSprColor(210,200,200);

	m_pCompOk		= m_pForm->GetComponent ("ok");
	m_pCompOk->SetEvent(ClickMAskOkButton,this);
	m_pCompCancel	= m_pForm->GetComponent ("no");
	m_pCompCancel->SetEvent(ClickMAskCancelButton,this);
	m_pCompAccept	= m_pForm->GetComponent ("yes");
	m_pCompAccept->SetEvent(ClickMAskOkButton,this);
}

MarryAskMenu::~MarryAskMenu()
{

}

void	MarryAskMenu::PopMsgbox(char* szTitle, char* szMsg,void (*func)(void * pParam,int nVal),void* param,int nType)
{
	m_pCompTitle->SetCaptionEx("%s",1,szTitle);
	m_pCompMsg->SetCaptionEx("%s",1,szMsg);
	Func_CBMsgBox = func;
	pParamMsgbox = param;
	if(nType == 0)
	{
		m_pCompOk->SetVisible(false);
		m_pCompAccept->SetVisible(true);
		m_pCompCancel->SetVisible(true);
	}
	else
	{
		m_pCompOk->SetVisible(true);
		m_pCompAccept->SetVisible(false);
		m_pCompCancel->SetVisible(false);
	}
	m_pForm->Open(2);
}

void	MarryAskMenu::ClickAnswer(bool bOk)
{
	if(Func_CBMsgBox)
	{
		Func_CBMsgBox(pParamMsgbox,bOk?1:0);
	}
	m_pForm->Close();
}

void	SRFrameStepEvent(int nPos)
{
	if(nPos >= 0)
	{
		pMainMenu->m_pSendRingMenu->m_nPos = nPos+1;
	}
}

void	ClickSendRingButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		SendRingMenu* pMenu = (SendRingMenu*)param;
		int nPos = pMenu->m_pRadio->GetSel();
		pChar->m_pMarryCenter->SendRing(nPos);
	}
}

SendRingMenu::SendRingMenu()
{
	m_pForm = pMainInterface->GetDForm("jiehun5");
	char szname[8];
	DComponent* ppComp[3];
	for(int i = 0; i < 3; i ++)
	{
		sprintf(szname,"g%d",i+1);
		ppComp[i] = m_pForm->GetComponent(szname);
	}
	m_pRadio = new CCtrlRadio(ppComp,3);

	m_pForm->GetComponent("song")->SetEvent(ClickSendRingButton,this);
}

SendRingMenu::~SendRingMenu()
{

}

void	SendRingMenu::Open()
{
	m_pForm->Open(2);
}

void	SendRingMenu::Update()
{
	int nFlag = pChar->m_pClientFlag->GetFlag(30);
	int nRlt = 5-nFlag;
	if(nRlt < 0)
		nRlt = 0;
	m_pForm->GetComponent("ci")->SetCaptionEx("%d",1,nRlt);
}

void ClickMNChooseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int n = (int)param;
	if(n == 3)
	{
		ClientMarryData* pData = pChar->m_pMarryCenter->GetRelation();
		if(pData == NULL || pData->byState < 2)
		{
			pMainMenu->PopTipMsg("结婚后才能使用该功能");
			return;
		}
		pMainMenu->m_pMarryNpcMenu->OpenChoose();
	}
	else if(n == 4)
	{
		ClientMarryData* pData = pChar->m_pMarryCenter->GetRelation();
		if(pData == NULL)
		{
			pMainMenu->PopTipMsg("订婚后才能使用该功能");
			return;
		}
		pMainMenu->m_pMarryNpcMenu->OpenAward();
	}
	else
	{
		if(n == 0)
		{
			if (pMainMenu->m_pTeamMenu->GetTeamedCount() != 0)
			{
				ClientMarryData* pData = pChar->m_pMarryCenter->GetRelation();
			/*	if (pData)
				{
					pMainMenu->PopTipMsg("抱歉，我们不支持婚外恋。");
					return;
				}
				if (pMainMenu->m_pTeamMenu->m_xUser[0].bySex == pMainMenu->m_pTeamMenu->m_xUser[1].bySex)
				{
					pMainMenu->PopTipMsg("抱歉，我们不支持同性恋。");
					return;
					}*/
				if (!pData && pMainMenu->m_pTeamMenu->m_xUser[0].bySex != pMainMenu->m_pTeamMenu->m_xUser[1].bySex)
					pMainMenu->m_pMarryAskMenu->PopMsgbox("求婚","求婚成功，等待对方回复。",NULL,NULL,1);
			}
			pMainMenu->m_pMarryNpcMenu->Close();
		}
		pChar->m_pMarryCenter->SendActionEvent(n);
	}
}

void ClickSendRunTypeButton(InterfaceEvent * pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	MarryNpcMenu* pMenu = (MarryNpcMenu*)param;
	if(pMenu)
	{
		pMenu->m_pForm->Close();
		pMenu->m_pChooseForm->Close();
		int nType = pMenu->m_pRadio->GetSel();
		pChar->m_pMarryCenter->SendActionEvent(nType+10);
	}
}

void	ClickGetMAwardButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	pChar->m_pMarryCenter->SendActionEvent(nPos+20);
}

MarryNpcMenu::MarryNpcMenu()
{
	m_pForm = pMainInterface->GetDForm("jiehun6");
	char szname[8];
	for(int i = 0; i < 5; i ++)
	{
		sprintf(szname,"a%d",i+1);
		DComponent* pComp = m_pForm->GetComponent(szname);
		if(pComp)
			pComp->SetEvent(ClickMNChooseButton,(void*)i);
	}

	m_pChooseForm = pMainInterface->GetDForm("jiehun7");
	DComponent* ppComp[3];
	for(int i = 0; i < 3; i ++)
	{
		sprintf(szname,"g%d",i+1);
		ppComp[i] = m_pChooseForm->GetComponent(szname);
	}
	m_pRadio = new CCtrlRadio(ppComp,3);
	m_pChooseForm->GetComponent("yes")->SetEvent(ClickSendRunTypeButton,this);
	
	m_pAwardForm = pMainInterface->GetDForm("jiehun8");
	for(int i = 0; i < 5; i ++)
	{
		sprintf(szname,"get%d",i+1);
		DComponent* pComp = m_pAwardForm->GetComponent(szname);
		if(pComp)
			pComp->SetEvent(ClickGetMAwardButton,(void*)i);
	}
}

void	MarryNpcMenu::Open()
{
	m_pForm->Open(2);
}

void	MarryNpcMenu::Close()
{
	if(m_pForm->bOpened)
		m_pForm->Close();
	if(m_pChooseForm->bOpened)
		m_pChooseForm->Close();
	if(m_pAwardForm->bOpened)
		m_pAwardForm->Close();
}

void	MarryNpcMenu::OpenChoose()
{
	m_pChooseForm->Open(2);
}

void	MarryNpcMenu::OpenAward()
{
	m_pAwardForm->Open(2);
}

void	MarryNpcMenu::Update()
{
	if(m_pAwardForm && m_pAwardForm->bOpened)
	{
		ClientMarryData* pData = pChar->m_pMarryCenter->GetRelation();
		char szName[32];
		DComponent * pComp = NULL;
		if(pData)
		{
			int nPoint = 0;
			if(pData->dwMan == pChar->m_dwUin)
				nPoint = pData->nMpoint;
			else
				nPoint = pData->nWpoint;
			m_pAwardForm->GetComponent("num")->SetCaptionEx("%d",1,nPoint);
			int nFlag = pChar->m_pClientFlag->GetFlag(29);
			int nCheck = 0x01;
			for(int i = 0; i < 5; i ++)
			{
				if(i > 0)
					nCheck = nCheck << 1;
				if((nFlag & nCheck) == nCheck)
				{
					sprintf(szName,"get%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(false);
					sprintf(szName,"yi%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(true);
				}
				else
				{
					sprintf(szName,"yi%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(false);
					sprintf(szName,"get%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(true);
					MarryAward* pAward = (MarryAward*)MarrySkillCfg::GetInstance().m_pAwardList->Items[i];
					if(nPoint < pAward->nNeed)
					{
						m_pAwardForm->GetComponent(szName)->bClickLocked = true;
						m_pAwardForm->GetComponent(szName)->SetViewSpr("BT_LINGQU234_H.png");
					}
					else
					{
						m_pAwardForm->GetComponent(szName)->bClickLocked = false;
						m_pAwardForm->GetComponent(szName)->SetViewSpr("BT_LINGQU234.png");
					}
				}
				
			}
		}
		else
		{
			m_pAwardForm->GetComponent("num")->SetCaption("0",1);
			int nFlag = pChar->m_pClientFlag->GetFlag(29);
			int nCheck = 0x01;
			for(int i = 0; i < 5; i ++)
			{
				if(nFlag & nCheck == nCheck)
				{
					sprintf(szName,"get%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(false);
					sprintf(szName,"yi%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(true);
				}
				else
				{
					sprintf(szName,"yi%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(false);
					sprintf(szName,"get%d",i+1);
					m_pAwardForm->GetComponent(szName)->SetVisible(true);
					m_pAwardForm->GetComponent(szName)->bClickLocked = true;
					m_pAwardForm->GetComponent(szName)->SetViewSpr("BT_LINGQU234_H.png");
				}
			}
		}
		
	}
}