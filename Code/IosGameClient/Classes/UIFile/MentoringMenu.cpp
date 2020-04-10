#include "UserData.h"
#include "MentoringMenu.h"
#include "ios_relation_packet.h"
#include "MentoringCfg.h"
#include "MainMenu.h"

extern char* szJobTxt[];
extern char* szCountryTxt[];

void	MCF1ClickEvent(int nPos,char*pName,void* param)
{
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		pMenu->m_nPos1 = nPos;
	}
}

void	ClickInstructButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		pChar->m_pMentoringClient->RequestInstruct();
	}
}

void	InstructAskReturn(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->m_pMentoringClient->RequestInstruct();
	}
}

void	ClickInstructExButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->PopAskMsgBox("导师需要尽心尽责的教育学生。是否确定要在学院登记寻找学生。",InstructAskReturn,NULL);
	}
}

void	ClickStudyButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		if(pChar->m_byLvl > 45)
		{
			pMainMenu->PopTipMsg("等级大于45级不能拜师。");
		}
		else if (pChar->m_byLvl >= 10)
			pChar->m_pMentoringClient->RequestStudy(pMenu->m_nPos1);
		else
			pMainMenu->PopTipMsg("等级小于10级不能拜师。");
	}
}

void	ClickTalkButton1(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		if(pMenu->m_nPos1 >= 0 && pMenu->m_nPos1 < pChar->m_pMentoringClient->m_pTeacherList->Count)
		{
			MCRequest* pData = (MCRequest*)pChar->m_pMentoringClient->m_pTeacherList->Items[pMenu->m_nPos1];
			if(pData->dwPuin == pChar->m_dwUin)
				return;
			pMenu->Close();
			pMainMenu->m_pChatMenu->Open();
			pMainMenu->m_pChatMenu->SetTargent(pData->szPName);
		}
	}
}

void	MCF2ClickEvent(int nPos,char*pName,void* param)
{
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
// 		if(strcmp(pName,"song") == 0)
// 		{
// 			pChar->m_pMentoringClient->RequestSendItem(nPos);
// 		}
// 		else
			pMenu->m_nPos2 = nPos;
	}
}

void	ClickTalkButton2(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		if(pMenu->m_nPos2 >= 0 && pMenu->m_nPos2 < pChar->m_pMentoringClient->m_pRelationList->Count)
		{
			pMenu->Close();
			pMainMenu->m_pChatMenu->Open();
			MCData* pData = (MCData*)pChar->m_pMentoringClient->m_pRelationList->Items[pMenu->m_nPos2];
			if(pData->dwPupil == pChar->m_dwUin)
			{
				pMainMenu->m_pChatMenu->SetTargent(pData->szTName);
			}
			else if(pData->dwTeacher == pChar->m_dwUin)
			{
				pMainMenu->m_pChatMenu->SetTargent(pData->szPName);
			}
			else
			{
				pMainMenu->m_pChatMenu->SetTargent(pData->szPName);
			}
		}
	}
}

void	ClickGetItemButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		pChar->m_pMentoringClient->RequestGetItem(pMenu->m_nPos2);
	}
}

void	ClickSendItemButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		pChar->m_pMentoringClient->RequestSendItem(pMenu->m_nPos2);
	}
}

void	ClickBreakRelationButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		pChar->m_pMentoringClient->RequestBreakRelation(pMenu->m_nPos2);
	}
}

void	ClickTipInfoButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MentoringMenu* pMenu = (MentoringMenu*)param;
		pMenu->OpenTip();
	}
}

void	LoginStudyBtn(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if (pChar->m_byLvl >= 50)
	{
		pChar->m_pMentoringClient->RequestInstruct();
	}else
		pMainMenu->PopTipMsg("您等级还低，不能申请成为导师。");
}

void	FindTeacherBtn(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if (pChar->m_pMentoringClient->m_pRelationList->Count > 0)
	{
		if (pChar->m_byLvl >= 50)
		{
			pMainMenu->PopTipMsg("您已收满学生，进入人物学院界面，好好教导您的学生吧。");
		}else
			pMainMenu->PopTipMsg("您已找到导师，好好和导师学习吧。");
	}else
		pMainMenu->m_pTabSocialityCtrl->Open(1, 2);
}

void	GetStudentBtn(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if (pChar->m_byLvl < 50)
	{
		pMainMenu->PopTipMsg("等级达到50级，才可收学生");
		return;
	} 

	if (pMainMenu->m_pTeamMenu->GetTeamCount() != 0 && pMainMenu->m_pTeamMenu->GetTeamCount() <= 2)
	{
		/*	if (pChar->m_dwUin != pMainMenu->m_pTeamMenu->GetLeadID())
		{
		pMainMenu->PopTipMsg("只有队长才能收徒。");
		return;
		}*/
		if (pChar->m_byLvl >= 50)
		{
			_TeamNet* pStudent = pMainMenu->m_pTeamMenu->GetStudentInfo();
			if (pStudent)
			{
				pMainMenu->m_pMentoringMenu->m_pNpcForm->Close();
				pChar->m_pMentoringClient->RequestMsg(pStudent->dwUin);
			}else
				pMainMenu->PopTipMsg("所收玩家等级大于45级，不能成为徒弟");
		}else
			pMainMenu->PopTipMsg("等级不足50级，不能收徒。");
	}else
		pMainMenu->PopTipMsg("请组队带上你收徒的玩家。");
}

void	ClickMentorCopyButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if (param)
	{
		BuildPacketEx(pPacket,RPGACTIVITY,C_REQUEST_ENTERAC,buf,1024);
		SET_DATA(pData,RPGACTIVITY,C_REQUEST_ENTERAC,pPacket);
		pData->nID = 1029;
		pChar->SendPacket(pPacket);
	}
}

MentoringMenu::MentoringMenu()
{
	m_nPos1 = 0;
	m_nPos2 = 0;
	m_pForm3 = pMainInterface->GetDForm("shitu1");
	DComponent* pLCompBase = m_pForm3->GetComponent("n1");
	DComponent* ppLComp[7];
	ppLComp[0] = m_pForm3->GetComponent("xz");
	ppLComp[1] = m_pForm3->GetComponent("y1");
	ppLComp[2] = m_pForm3->GetComponent("x1");
	ppLComp[3] = m_pForm3->GetComponent("name");
	ppLComp[4] = m_pForm3->GetComponent("job");
	ppLComp[5] = m_pForm3->GetComponent("lv");
	ppLComp[6] = m_pForm3->GetComponent("org");
	m_pForm3->pCFrame = new CCtrlFrame(pLCompBase->nX,pLCompBase->nY,pLCompBase->nW,pLCompBase->nH*6);
	m_pForm3->pCFrame->InitFParts(pLCompBase,ppLComp,7);
	m_pForm3->pCFrame->nDisStep = pLCompBase->nH;
	m_pForm3->pCFrame->SetEventEx(MCF1ClickEvent,this);
	m_pForm3->pCFrame->SetFPCnt(0);
	m_pForm3->GetComponent("st")->SetEvent(ClickInstructButton,this);
	m_pForm3->GetComponent("qx")->SetEvent(ClickStudyButton,this);
	m_pForm3->GetComponent("sl")->SetEvent(ClickTalkButton1,this);
	m_pForm = m_pForm3;

	m_pForm2 = pMainInterface->GetDForm("shitu2");   
	DComponent* pRCompBase = m_pForm2->GetComponent("n1");
	DComponent* ppRComp[9];
	ppRComp[0] = m_pForm2->GetComponent("xz");
	ppRComp[1] = m_pForm2->GetComponent("gx");
	ppRComp[2] = m_pForm2->GetComponent("x1");
	ppRComp[3] = m_pForm2->GetComponent("name");
	ppRComp[4] = m_pForm2->GetComponent("job");
	ppRComp[5] = m_pForm2->GetComponent("lv");
	ppRComp[6] = m_pForm2->GetComponent("ch");
	ppRComp[7] = m_pForm2->GetComponent("y");
	ppRComp[8] = m_pForm2->GetComponent("y1");
	m_pForm2->pCFrame = new CCtrlFrame(pRCompBase->nX,pRCompBase->nY,pRCompBase->nW,pRCompBase->nH*6);
	m_pForm2->pCFrame->InitFParts(pRCompBase,ppRComp,9);
	m_pForm2->pCFrame->nDisStep = pRCompBase->nH;
	m_pForm2->pCFrame->SetEventEx(MCF2ClickEvent,this);
	m_pForm2->pCFrame->SetFPCnt(0);
	m_pForm2->GetComponent("siliao")->SetEvent(ClickTalkButton2,this);
	m_pForm2->GetComponent("lingqu")->SetEvent(ClickGetItemButton,this);
	m_pForm2->GetComponent("lingqu1")->SetEvent(ClickGetItemButton,this);
	m_pForm2->GetComponent("song")->SetEvent(ClickSendItemButton,this);
	m_pForm2->GetComponent("song1")->SetEvent(ClickSendItemButton,this);
	m_pForm2->GetComponent("tuili")->SetEvent(ClickBreakRelationButton,this);
	m_pForm2->GetComponent("shuoming")->SetEvent(ClickTipInfoButton,this);
	m_pForm2->GetComponent("st")->SetEvent(ClickInstructExButton,this);
	m_pForm2->GetComponent("fb")->SetEvent(ClickMentorCopyButton,this);

	m_pTipForm = pMainInterface->GetDForm("shitu4");

	m_pNpcForm = pMainInterface->GetDForm("shitu5");
	m_pNpcForm->GetComponent("a1")->SetEvent(LoginStudyBtn, this);
	m_pNpcForm->GetComponent("a2")->SetEvent(FindTeacherBtn, this);
	m_pNpcForm->GetComponent("a3")->SetEvent(GetStudentBtn, this);
}

MentoringMenu::~MentoringMenu()
{

}

void	MentoringMenu::OpenTip()
{
	m_pTipForm->Open(2);
}

void	MentoringMenu::CloseTip()
{
	m_pTipForm->Close();
}

void	MentoringMenu::Open(int nT)
{
	if(pChar->m_pMentoringClient->m_pRelationList->Count > 0)
		m_pForm = m_pForm2;
	else
	{
		pChar->m_pMentoringClient->RequestTeacherList();
		m_pForm = m_pForm3;
	}
	m_pForm->Open(nT);
}

void	MentoringMenu::Close()
{
	if(m_pForm3 && m_pForm3->bOpened)
		m_pForm3->Close();
	if(m_pForm2 && m_pForm2->bOpened)
		m_pForm2->Close();
}

void MentoringMenu::InitTabEvent(int nCnt)
{
	char	szTmp[256];
	for (int j = 0; j < nCnt; j++)
	{
		sprintf(szTmp,"page%d",j+1);
		m_pForm2->GetComponent(szTmp)->eventFunc	= m_pForm->GetComponent (szTmp)->eventFunc;
		m_pForm2->GetComponent(szTmp)->pCbParam	= m_pForm->GetComponent (szTmp)->pCbParam;
	}
	m_pForm2->GetComponent("close")->eventFunc	= m_pForm->GetComponent ("close")->eventFunc;
	m_pForm2->GetComponent("close")->pCbParam	= m_pForm->GetComponent ("close")->pCbParam;

}

void	MentoringMenu::Update()
{
	if(m_pForm3->bOpened)
	{
		DComponent* pComp = NULL;
		if(pChar->m_pMentoringClient->m_ifTChg)
		{
			pChar->m_pMentoringClient->m_ifTChg = false;
			int nCnt = pChar->m_pMentoringClient->m_pTeacherList->Count;
			m_pForm3->pCFrame->SetFPCnt(nCnt);
			for(int i = 0; i < nCnt; i ++)
			{
				MCRequest* pData = (MCRequest*)pChar->m_pMentoringClient->m_pTeacherList->Items[i];
				pComp = m_pForm3->pCFrame->GetFPComp(i,"name");
				pComp->SetCaptionEx("%s",1,pData->szPName);
				pComp = m_pForm3->pCFrame->GetFPComp(i,"job");
				pComp->SetCaptionEx("%s",1,szJobTxt[pData->bPJob]);
				pComp = m_pForm3->pCFrame->GetFPComp(i,"lv");
				pComp->SetCaptionEx("%d",1,pData->bPLvl);
				pComp = m_pForm3->pCFrame->GetFPComp(i,"org");
				pComp->SetCaptionEx("%s",1,szCountryTxt[pChar->m_byCountry]);
				pComp = m_pForm3->pCFrame->GetFPComp(i,"y1");
				if(pChar->m_pMentoringClient->CheckTeacherUin(pData->dwPuin))
					pComp->SetVisible(true);
				else
					pComp->SetVisible(false);
			}
		}
		if(pChar->m_byLvl >= 50 && !pChar->m_pMentoringClient->m_ifInstruct)
			m_pForm3->GetComponent("st")->SetVisible(true);
		else
			m_pForm3->GetComponent("st")->SetVisible(false);
		m_pForm3->GetComponent("qx")->SetVisible(false);
		int nPCount = m_pForm3->pCFrame->GetFPCnt();
		for(int i = 0; i < nPCount; i ++)
		{
			if(i >= pChar->m_pMentoringClient->m_pTeacherList->Count)
				break;
			MCRequest* pData = (MCRequest*)pChar->m_pMentoringClient->m_pTeacherList->Items[i];
			pComp = m_pForm3->pCFrame->GetFPComp(i,"xz");
			if(i == m_nPos1)
				pComp->SetVisible(true);
			else
				pComp->SetVisible(false);
			if(pChar->m_byLvl < 50 && !pChar->m_pMentoringClient->CheckTeacherUin(pData->dwPuin))
				m_pForm3->GetComponent("qx")->SetVisible(true);
		}
	}
	if(m_pForm2->bOpened)
	{
		DComponent* pComp = NULL;
		if(pChar->m_pMentoringClient->m_ifChg)
		{
			bool ifShow = true;
			pChar->m_pMentoringClient->m_ifChg = false;
			int nCnt = pChar->m_pMentoringClient->m_pRelationList->Count;
			m_pForm2->pCFrame->SetFPCnt(nCnt);
			for(int i = 0; i < nCnt; i ++)
			{
				int nHVal = 0;
				MCData* pData = (MCData*)pChar->m_pMentoringClient->m_pRelationList->Items[i];
				if(pData->dwPupil == pChar->m_dwUin)
				{
					pComp = m_pForm2->pCFrame->GetFPComp(i,"name");
					pComp->SetCaptionEx("%s",1,pData->szTName);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"job");
					pComp->SetCaptionEx("%s",1,szJobTxt[pData->bTJob]);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"lv");
					pComp->SetCaptionEx("%d",1,pData->bTLvl);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"gx");
					pComp->SetCaption("老师",1);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"ch");
					MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pData->nHonour);
					if(pBase)
						pComp->SetCaptionEx("%s",1,pBase->szName);
					else
						pComp->SetCaption("见习助教",1);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"y1");
					pComp->SetVisible(false);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"y");
					if(pData->bStudyFlag == 0)
						pComp->SetVisible(false);
					else
						pComp->SetVisible(true);

					ifShow = false;
				}
				else if(pData->dwTeacher == pChar->m_dwUin)
				{
					pComp = m_pForm2->pCFrame->GetFPComp(i,"name");
					pComp->SetCaptionEx("%s",1,pData->szPName);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"job");
					pComp->SetCaptionEx("%s",1,szJobTxt[pData->bPJob]);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"lv");
					pComp->SetCaptionEx("%d",1,pData->bPLvl);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"gx");
					pComp->SetCaption("学生",1);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"ch");
					pComp->SetCaption("无",1);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"y");
					pComp->SetVisible(false);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"y1");
					if(pData->bTeachFlag == 0)
						pComp->SetVisible(false);
					else
						pComp->SetVisible(true);
					nHVal = pData->nHonour;
				}
				else
				{
					pComp = m_pForm2->pCFrame->GetFPComp(i,"name");
					pComp->SetCaptionEx("%s",1,pData->szPName);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"job");
					pComp->SetCaptionEx("%s",1,szJobTxt[pData->bPJob]);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"lv");
					pComp->SetCaptionEx("%d",1,pData->bPLvl);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"gx");
					pComp->SetCaption("同学",1);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"ch");
					pComp->SetCaption("无",1);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"y1");
					pComp->SetVisible(false);
					pComp = m_pForm2->pCFrame->GetFPComp(i,"y");
					pComp->SetVisible(false);
				}
				m_pForm2->GetComponent("sw")->SetCaptionEx("%d",1,nHVal);
			}
			m_pForm2->GetComponent("z1")->SetVisible(ifShow);
			m_pForm2->GetComponent("sw")->SetVisible(ifShow);
			m_pForm2->GetComponent("z")->SetVisible(ifShow);
			m_pForm2->GetComponent("num")->SetVisible(ifShow);
		}
		if(pChar->m_byLvl >= 50 && !pChar->m_pMentoringClient->m_ifInstruct)
			m_pForm2->GetComponent("st")->SetVisible(true);
		else
			m_pForm2->GetComponent("st")->SetVisible(false);
		int nRCount = m_pForm2->pCFrame->GetFPCnt();
		m_pForm2->GetComponent("lingqu")->SetVisible(false);
		if(nRCount == 0)
			m_pForm2->GetComponent("num")->SetCaptionEx("0/0",1);
		else
		{
			for(int i = 0; i < nRCount; i ++)
			{
				if(i >= pChar->m_pMentoringClient->m_pRelationList->Count)
					break;
				MCData* pData = (MCData*)pChar->m_pMentoringClient->m_pRelationList->Items[i];
				pComp = m_pForm->pCFrame->GetFPComp(i,"xz");
				if(i == m_nPos2)
				{
					pComp->SetVisible(true);
					if(pData->dwPupil == pChar->m_dwUin && pData->bTeachFlag == 1)
					{
						m_pForm2->GetComponent("lingqu")->SetVisible(false);
						m_pForm2->GetComponent("lingqu1")->SetVisible(true);
					}
					else if(pData->dwTeacher == pChar->m_dwUin && pData->bStudyFlag == 1)
					{
						m_pForm2->GetComponent("lingqu")->SetVisible(true);
						m_pForm2->GetComponent("lingqu1")->SetVisible(false);
					}
					else
					{
						m_pForm2->GetComponent("lingqu")->SetVisible(false);
						m_pForm2->GetComponent("lingqu1")->SetVisible(false);
					}

					if(pData->dwPupil == pChar->m_dwUin && pData->bStudyFlag == 0)
					{
						m_pForm2->GetComponent("song")->SetVisible(true);
						m_pForm2->GetComponent("song1")->SetVisible(false);
					}
					else if(pData->dwTeacher == pChar->m_dwUin && pData->bTeachFlag == 0)
					{
						m_pForm2->GetComponent("song")->SetVisible(false);
						m_pForm2->GetComponent("song1")->SetVisible(true);
					}
					else
					{
						m_pForm2->GetComponent("song")->SetVisible(false);
						m_pForm2->GetComponent("song1")->SetVisible(false);
					}
				}
				else
					pComp->SetVisible(false);
			}
			MCData* pData = (MCData*)pChar->m_pMentoringClient->m_pRelationList->Items[0];
			MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pData->nHonour);
			if(pBase)
			{
				int nDelta = pBase->nPuplecnt-nRCount;
				if(nDelta < 0)
					nDelta = 0;
				m_pForm2->GetComponent("num")->SetCaptionEx("%d/%d",1,nDelta,pBase->nPuplecnt);
			}
			else
				m_pForm2->GetComponent("num")->SetCaptionEx("0/0",1);
		}
	}
}

bool MentoringMenu::CheckCanOpen()
{
// 	if(pChar->m_byLvl < 10)
// 	{
// 		pMainMenu->PopTipMsg("进入学院学习需要等级达到10级哦，稍微长大后再来吧");
// 		return false;
// 	}
	return true;
}


void	MCFRQClickEvent(int nPos,char*pName,void* param)
{
	if(param)
	{
		MRequestMenu* pMenu = (MRequestMenu*)param;
		pMenu->m_nPos = nPos;
	}
}

void	ClickMRAcceptButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MRequestMenu* pMenu = (MRequestMenu*)param;
		pChar->m_pMentoringClient->RequestDealWith(pMenu->m_nPos,true);
	}
}

void	ClickMRRefuseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		MRequestMenu* pMenu = (MRequestMenu*)param;
		pChar->m_pMentoringClient->RequestDealWith(pMenu->m_nPos,false);
	}
}

MRequestMenu::MRequestMenu()
{
	m_nPos = 0;
	m_pForm = pMainInterface->GetDForm("shitu3");
	DComponent* pCompBase = m_pForm->GetComponent("n1");
	DComponent* ppComp[6];
	ppComp[0] = m_pForm->GetComponent("xz");
	ppComp[1] = m_pForm->GetComponent("x1");
	ppComp[2] = m_pForm->GetComponent("name");
	ppComp[3] = m_pForm->GetComponent("lv");
	ppComp[4] = m_pForm->GetComponent("job");
	ppComp[5] = m_pForm->GetComponent("jun");
	m_pForm->pCFrame = new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,pCompBase->nH*6);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,6);
	m_pForm->pCFrame->nDisStep = pCompBase->nH;
	m_pForm->pCFrame->SetEventEx(MCFRQClickEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);
	m_pForm->GetComponent("ok")->SetEvent(ClickMRAcceptButton,this);
	m_pForm->GetComponent("no")->SetEvent(ClickMRRefuseButton,this);
}

MRequestMenu::~MRequestMenu()
{

}

void MRequestMenu::Open()
{
	m_pForm->Open(2);
}

void MRequestMenu::Close()
{
	m_pForm->Close();
}

void MRequestMenu::Update()
{
	if(m_pForm->bOpened)
	{
		DComponent* pComp = NULL;
		if(pChar->m_pMentoringClient->m_ifRChg)
		{
			pChar->m_pMentoringClient->m_ifRChg = false;
			int nCnt = pChar->m_pMentoringClient->m_pRequestList->Count;
			m_pForm->pCFrame->SetFPCnt(nCnt);
			for(int i = 0; i < nCnt; i ++)
			{
				MCRequest* pData = (MCRequest*)pChar->m_pMentoringClient->m_pRequestList->Items[i];
				pComp = m_pForm->pCFrame->GetFPComp(i,"name");
				pComp->SetCaptionEx("%s",1,pData->szPName);
				pComp = m_pForm->pCFrame->GetFPComp(i,"lv");
				pComp->SetCaptionEx("%d",1,pData->bPLvl);
				pComp = m_pForm->pCFrame->GetFPComp(i,"job");
				pComp->SetCaptionEx("%s",1,szJobTxt[pData->bPJob]);
				pComp = m_pForm->pCFrame->GetFPComp(i,"jun");
				pComp->SetCaption("平民",1);
			}
		}
		int nRCount = m_pForm->pCFrame->GetFPCnt();
		for(int i = 0; i < nRCount; i ++)
		{
			pComp = m_pForm->pCFrame->GetFPComp(i,"xz");
			if(i == m_nPos)
				pComp->SetVisible(true);
			else
				pComp->SetVisible(false);
		}
	}
}