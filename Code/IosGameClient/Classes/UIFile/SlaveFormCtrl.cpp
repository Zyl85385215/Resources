#include "UserData.h"
#include "SlaveFormCtrl.h"
#include "SlaveConfig.h"
#include "HorseConfig.h"
#include "MainMenu.h"



SlaveMenuCtrl* g_pSlaveMenuCtrl = NULL;

void CFMIconEvent(int i , char* pName,void* param)
{
	if(param == NULL)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(strcmp(pName,"slave1") == 0 || strcmp(pName,"zuoqi1") == 0)
	{
		pMenu->byCur = i+1;
		pMenu->CFmEvent();
	}
}

void ClickPActiveButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		pChar->m_pSlaveCenter->RequestActiveSlave(pMenu->byCur);
		pMainMenu->m_pAutoMenu->m_bifResetPetState = true;
	}
}

void MsgbosReturnEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		int nRlt = (int)param;
		pChar->m_pSlaveCenter->RequestRelease(nRlt);
	}
}

void ClickPReleaseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		pMainMenu->PopAskMsgBox("是否确定放生当前宠物?",MsgbosReturnEvent,(void*)pMenu->byCur);
	}
}

void ClickChgPNameButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		pMainMenu->m_pChangePNameMenu->Open(pMenu->byCur);
	}
}

void ClickPGetButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	DForm* pForm = pMainInterface->GetDForm((char*)param);
	if (pForm)
	{
		pForm->Open();
	}
}

void ClickHEquipButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		pChar->m_pHorseCenter->RequestEquipHorse(pMenu->byCur);
	}
}

void HMsgbosReturnEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		int nRlt = (int)param;
		pChar->m_pHorseCenter->RequestReleaseHorse(nRlt);
	}
}

void ClickHReleaseButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		pMainMenu->PopAskMsgBox("是否确定放生当前坐骑?",HMsgbosReturnEvent,(void*)pMenu->byCur);
	}
}

void ClickTurnDirectionLeft(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
		pMenu->byDirection += 2;
	if(pMenu->byDirection > 7)
		pMenu->byDirection = 1;
}

void ClickTurnDirectionRight(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
		pMenu->byDirection -= 2;
	if(pMenu->byDirection > 200)
		pMenu->byDirection = 7;
}

void ClickSSkillIncoEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		if(pMenu->byCur > 0 && pMenu->byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
		{
			SlaveCData*	pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[pMenu->byCur-1];
			int nSPos = atoi(&pEvent->pComponent->szName[2]);
			SkillBase* pSkill = SkillConfig::GetInstance().GetSkillBase(pSlave->m_nSkill[nSPos-1]);
			if(pSkill)
			{
				pMainMenu->m_pSkillMenu->ViewPetSkillInfo(pSkill,pSlave->m_nSkillLv[nSPos-1]);
			}
			else
			{
				pMainMenu->PopTipMsg("尚未学习技能，可通过宠物技能界面学习");
			}
		}
	}
}

void CastReviveEvent(void * param)
{
	int nRlt = (int)param;
	pChar->m_pSlaveCenter->RequestRivive(nRlt);
}

void ClickSReviveButton(InterfaceEvent* pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if(pMenu)
	{
		//pMainMenu->StartCast(5000,"正在复活宠物",CastReviveEvent,(void*)pMenu->byCur);
		pChar->m_pSlaveCenter->RequestRivive(pMenu->byCur);
	}
}

void ChickPetSkillHelpEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SlaveMenuObj* pMenu = (SlaveMenuObj*)param;
	if (pMenu)
	{
		DForm* pForm = pMainInterface->GetDForm("cwskill3");
		if (pForm)
			pForm->Open();
	}
}

SlaveMenuObj::SlaveMenuObj(char* pName,BYTE byType)
{
	m_pForm = pMainInterface->GetDForm(pName);
	byCur = 0;
	byDirection = 5;
	if(byType == 0)
		g_pSlaveMenuCtrl->AddObjS(this);
	else
		g_pSlaveMenuCtrl->AddObjH(this);
}

SlaveMenuCtrl::SlaveMenuCtrl()
{
	m_pHFormList = xnList::Create();
	m_pSFormList = xnList::Create();
	m_nSCur = 0;
	m_nHCur = 0;
	m_byHDire = 5;
	m_bySDire = 5;
	for (int i = 0; i < 4; i++)
	{
		m_nHorsePY[i] = 0;
	}
}

SlaveMenuCtrl::~SlaveMenuCtrl()
{
	if(m_pSFormList)
	{
		while(m_pSFormList->Count)
		delete	(SlaveMenuObj*)m_pSFormList->Delete(0);
		m_pSFormList->Free();
	}
	if(m_pHFormList)
	{
		while(m_pHFormList->Count)
			delete	(SlaveMenuObj*)m_pHFormList->Delete(0);
		m_pHFormList->Free();
	}
}

void SlaveMenuCtrl::AddObjS(SlaveMenuObj* pMenu)
{
	if(pMenu == NULL || pMenu->m_pForm == NULL)
		return;
	DComponent* pComp = pMenu->m_pForm->GetComponent("gezi");
	if(pComp == NULL)
		return;
	DComponent* ppCom[1];
	ppCom[0] = pMenu->m_pForm->GetComponent("slave1");
	pMenu->m_pForm->pCFrame = new CCtrlFrame(pComp->nX,pComp->nY,pComp->nW*SHOW_OBJ_CNT,pComp->nH);
	pMenu->m_pForm->pCFrame->InitFParts(pComp,ppCom,1);
	pMenu->m_pForm->pCFrame->nDisStep = pComp->nW;
	pMenu->m_pForm->GetComponent("left")->SetEvent(MoveFrmLeft,pMenu->m_pForm->pCFrame);
	pMenu->m_pForm->GetComponent("right")->SetEvent(MoveFrmRight,pMenu->m_pForm->pCFrame);
	pMenu->m_pForm->GetComponent("name")->m_nFontSize = 24;
	pMenu->m_pForm->pCFrame->SetEventEx(CFMIconEvent,(void*)pMenu);

	DComponent* pButton = NULL;
	pButton = pMenu->m_pForm->GetComponent("chuzhan");
	if(pButton)
		pButton->SetEvent(ClickPActiveButton,pMenu);
	pButton = pMenu->m_pForm->GetComponent("fangsheng");
	if(pButton)
		pButton->SetEvent(ClickPReleaseButton,pMenu);
	pButton = pMenu->m_pForm->GetComponent("zhaohuan");
	if(pButton)
		pButton->SetEvent(ClickPGetButton,(void*)"cwchoujiang");
	pButton = pMenu->m_pForm->GetComponent("gaiming");
	if(pButton)
		pButton->SetEvent(ClickChgPNameButton,pMenu);
	pButton = pMenu->m_pForm->GetComponent("left1");
	if(pButton)
		pButton->SetEvent(ClickTurnDirectionLeft,pMenu);
	pButton = pMenu->m_pForm->GetComponent("right1");
	if(pButton)
		pButton->SetEvent(ClickTurnDirectionRight,pMenu);
	pButton = pMenu->m_pForm->GetComponent("zjsm");
	if(pButton)
		pButton->SetEvent(ClickSReviveButton,pMenu);
	char szName[32];
	for(int i = 0; i < SLAVE_SKILL_MAX; i ++)
	{
		sprintf(szName,"cw%d",i+1);
		pButton = pMenu->m_pForm->GetComponent(szName);
		if(pButton)
			pButton->SetEvent(ClickSSkillIncoEvent,pMenu);
	}
	m_pSFormList->Add((void*)pMenu);

	pButton = pMenu->m_pForm->GetComponent("sm1");
	if (pButton)
		pButton->SetEvent(ChickPetSkillHelpEvent,pMenu);
}

void SlaveMenuCtrl::AddObjH(SlaveMenuObj* pMenu)
{
	if(pMenu == NULL || pMenu->m_pForm == NULL)
		return;
	DComponent* pComp = pMenu->m_pForm->GetComponent("gezi");
	if(pComp == NULL)
		return;
	DComponent* ppCom[1];
	ppCom[0] = pMenu->m_pForm->GetComponent("zuoqi1");
	pMenu->m_pForm->pCFrame = new CCtrlFrame(pComp->nX,pComp->nY,pComp->nW*SHOW_OBJ_CNT,pComp->nH);
	pMenu->m_pForm->pCFrame->InitFParts(pComp,ppCom,1);
	pMenu->m_pForm->pCFrame->nDisStep = pComp->nW;
	pMenu->m_pForm->GetComponent("left")->SetEvent(MoveFrmLeft,pMenu->m_pForm->pCFrame);
	pMenu->m_pForm->GetComponent("right")->SetEvent(MoveFrmRight,pMenu->m_pForm->pCFrame);
	pMenu->m_pForm->GetComponent("name")->m_nFontSize = 24;	
	pMenu->m_pForm->pCFrame->SetEventEx(CFMIconEvent,(void*)pMenu);
	DComponent* pButton = NULL;
	pButton = pMenu->m_pForm->GetComponent("chuzhan");
	if(pButton)
		pButton->SetEvent(ClickHEquipButton,pMenu);
	pButton = pMenu->m_pForm->GetComponent("fangsheng");
	if(pButton)
		pButton->SetEvent(ClickHReleaseButton,pMenu);
	pButton = pMenu->m_pForm->GetComponent("left1");
	if(pButton)
		pButton->SetEvent(ClickTurnDirectionLeft,pMenu);
	pButton = pMenu->m_pForm->GetComponent("right1");
	if(pButton)
		pButton->SetEvent(ClickTurnDirectionRight,pMenu);
	m_pHFormList->Add((void*)pMenu);
}


void PetSkillMsg(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->PopTipMsg("尚未学习技能，可通过宠物技能界面学习");
}

void SlaveMenuCtrl::UpdateSlaveMenu()
{
	if(pChar->m_pSlaveCenter->m_ifNumChange)
	{
		pChar->m_pSlaveCenter->m_ifNumChange = false;
		for(int i = m_pSFormList->Count - 1; i >= 0; i --)
		{
			SlaveMenuObj* pObj = (SlaveMenuObj*)m_pSFormList->Items[i];
			int nCount = pChar->m_pSlaveCenter->m_pSlaveList->Count;
			pObj->m_pForm->pCFrame->SetFPCnt(nCount,true);
			for(int j = 0; j < nCount; j ++)
			{
				//显示头像
				SlaveCData* pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[j];
				if(pSlave&&pSlave->m_pSkinObj)
				{
					char szIcon[32];
					sprintf(szIcon,"head/%d.png",pSlave->m_pSkinObj->nHead);
					pObj->m_pForm->pCFrame->GetFPComp(j,"slave1")->SetViewSpr(szIcon);
				}
			}
		}
	}

	for(int i = m_pSFormList->Count - 1; i >= 0; i --)
	{
		SlaveMenuObj* pObj = (SlaveMenuObj*)m_pSFormList->Items[i];
		if(pObj->m_pForm->bOpened)
		{
			if(pChar->m_pSlaveCenter->m_pSlaveList->Count == 0)
				pObj->byCur = 0;
			else if(pObj->byCur == 0 || pObj->byCur > pChar->m_pSlaveCenter->m_pSlaveList->Count)
				pObj->byCur = 1;
			if(m_nSCur != pObj->byCur)
				m_nSCur = pObj->byCur;
			if(m_bySDire != pObj->byDirection)
				m_bySDire = pObj->byDirection;
			int nFrmCount = pObj->m_pForm->pCFrame->GetFPCnt();
			for(int k = 0; k < nFrmCount && k < pChar->m_pSlaveCenter->m_pSlaveList->Count; k ++)
			{
				if( k == pObj->byCur-1)
					pObj->m_pForm->pCFrame->GetFPComp(k,"slave1")->SetAddSprByTag("MB_XZ1.png",-15,-15,1);
				else
					pObj->m_pForm->pCFrame->GetFPComp(k,"slave1")->SetAddSprByTag("",0,0,1);
				SlaveCData* pTemp = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[k];
				if(pTemp->m_nHp <= 0)
					pObj->m_pForm->pCFrame->GetFPComp(k,"slave1")->SetAddSprByTag("MB_CW_SIWANG.png",43,0,2);
				else if(pTemp->m_ifActive)
					pObj->m_pForm->pCFrame->GetFPComp(k,"slave1")->SetAddSprByTag("MB_CW_CHUZHAN.png",43,0,2);
				else
					pObj->m_pForm->pCFrame->GetFPComp(k,"slave1")->SetAddSprByTag("",0,0,2);
			}
		}
		else
		{
			if(pObj->byCur != m_nSCur)
				pObj->byCur = m_nSCur;
			if(pObj->byDirection != m_bySDire)
				pObj->byDirection = m_bySDire;
		}
		

		if(pObj->m_pForm && pObj->m_pForm->bOpened)
		{
			
			SlaveCData* pSlave = NULL;
			if(pObj->byCur > 0 && pObj->byCur <= pChar->m_pSlaveCenter->m_pSlaveList->Count)
				pSlave = (SlaveCData*)pChar->m_pSlaveCenter->m_pSlaveList->Items[pObj->byCur-1];
			DComponent* pCompLv = pObj->m_pForm->GetComponent("lv");			
			DComponent* pComppz = pObj->m_pForm->GetComponent("pz");
			DComponent* pComphp = pObj->m_pForm->GetComponent("hp");
			DComponent* pCompmp = pObj->m_pForm->GetComponent("mp");
			DComponent* pCompwg = pObj->m_pForm->GetComponent("wg");
			DComponent* pCompwf = pObj->m_pForm->GetComponent("wf");
			DComponent* pCompmg = pObj->m_pForm->GetComponent("mg");
			DComponent* pCompmf = pObj->m_pForm->GetComponent("mf");
			DComponent* pCompna = pObj->m_pForm->GetComponent("name");
			DComponent*	pCompCartoon = pObj->m_pForm->GetComponent("model");
			DComponent* pCompAc = pObj->m_pForm->GetComponent("chuzhan");
			DComponent* pCompRv = pObj->m_pForm->GetComponent("fh");
			DComponent* pCompExp = pObj->m_pForm->GetComponent("exp");
			DComponent* pCompLife = pObj->m_pForm->GetComponent("sm");
			DComponent* pCompSkill[SLAVE_SKILL_MAX];
			char szName[32];
			for(int k = 0; k < SLAVE_SKILL_MAX; k ++)
			{
				sprintf(szName,"cw%d",k+1);
				pCompSkill[k] = pObj->m_pForm->GetComponent(szName);				
			}
			if(pSlave && pSlave->m_pBasic)
			{//属性显示
				if(pCompLv){
					pCompLv->SetCaptionEx("%d",0,pSlave->m_byLevel);
					pCompLv->m_nFontSize = 22;//===zjk
					pCompLv->SetSprColor(255,255,255);
				}
				if(pComppz)
					pComppz->SetCaptionEx("%s",0,s_qualityName[pSlave->m_pBasic->byQuality]);
				if(pComphp)
					pComphp->SetCaptionEx("%d",0,pSlave->m_nParam[SLAVE_PARAM_HPMAX]);
				if(pCompmp)
					pCompmp->SetCaptionEx("%d",0,pSlave->m_nParam[SLAVE_PARAM_MPMAX]);
				if(pCompwg)
					pCompwg->SetCaptionEx("%d",0,pSlave->m_nParam[SLAVE_PARAM_AD]);
				if(pCompwf)
					pCompwf->SetCaptionEx("%d",0,pSlave->m_nParam[SLAVE_PARAM_DD]);
				if(pCompmg)
					pCompmg->SetCaptionEx("%d",0,pSlave->m_nParam[SLAVE_PARAM_AP]);
				if(pCompmf)
					pCompmf->SetCaptionEx("%d",0,pSlave->m_nParam[SLAVE_PARAM_DP]);
				if(pCompna)
				{
					pCompna->SetCaption(pSlave->m_pName,1);
					pCompna->SetSprColor(nItemColor[pSlave->m_pBasic->byQuality][0],nItemColor[pSlave->m_pBasic->byQuality][1],nItemColor[pSlave->m_pBasic->byQuality][2]);
				}
				if(pCompLife)
					pCompLife->SetCaptionEx("%d/250",0,pSlave->m_nLifePoint);
				if(pCompRv)
				{
					if(pSlave->m_nHp <= 0)
						pCompRv->SetVisible(true);
					else
						pCompRv->SetVisible(false);
				}
				if(pCompExp)
				{
					int nMax = SlaveBaseCfg::GetInstance().GetLvExp(pSlave->m_byLevel);
					pCompExp->SetProgress(pSlave->m_nExp,nMax);
				}
				if(pCompAc)
				{
					if(pSlave->m_ifActive)
						pCompAc->SetViewSpr("BT_XIUXI.png");
					else
						pCompAc->SetViewSpr("BT_CHUZHAN.png");
// 					if(pSlave->m_nHp > 0)
// 						pCompAc->SetVisible(true);
// 					else
// 						pCompAc->SetVisible(false);
				}
				if(pCompCartoon)
				{
					char szList[64];
					char szFile[64];
					sprintf(szList,"skin/%d/1.plist",pSlave->m_pBasic->nSkin);
					if(pObj->byDirection == 7)
					{
						sprintf(szFile,"skin/%d/stand/3",pSlave->m_pBasic->nSkin);
						pCompCartoon->SetCartoonAction(szList,szFile,600,pSlave->m_pSkinObj?pSlave->m_pSkinObj->nScale:100,true);
					}
					else
					{
						sprintf(szFile,"skin/%d/stand/%d",pSlave->m_pBasic->nSkin,pObj->byDirection);
						pCompCartoon->SetCartoonAction(szList,szFile,600,pSlave->m_pSkinObj?pSlave->m_pSkinObj->nScale:100);
					}
					
				}
				for(int k = 0; k < SLAVE_SKILL_MAX; k ++)
				{
					if(pCompSkill[k])
					{
						if(pSlave->m_nSkill[k] > 0)
						{
							SkillBase*	pSBase	= SkillConfig::GetInstance().GetSkillBase(pSlave->m_nSkill[k]);
							if(pSBase)
							{
								sprintf(szName,"skill/%d.png",pSBase->wIcon);
								pCompSkill[k]->SetViewSpr(szName);
								pCompSkill[k]->SetCaptionEx("Lv%d",2,pSlave->m_nSkillLv[k]);
							}
						}
						else
						{
							pCompSkill[k]->SetViewSpr("");
							pCompSkill[k]->SetCaption("",2);
	//						pCompSkill[k]->SetEvent(PetSkillMsg); //===zjk
						}
					}
				}
			}
			else
			{
				if(pCompLv){
					pCompLv->SetCaption("",0);					
				}
				if(pComppz)
					pComppz->SetCaption("",0);
				if(pComphp)
					pComphp->SetCaption("",0);
				if(pCompmp)
					pCompmp->SetCaption("",0);
				if(pCompwg)
					pCompwg->SetCaption("",0);
				if(pCompwf)
					pCompwf->SetCaption("",0);
				if(pCompmg)
					pCompmg->SetCaption("",0);
				if(pCompmf)
					pCompmf->SetCaption("",0);
				if(pCompna)
					pCompna->SetCaption("",1);
				if(pCompExp)
					pCompExp->SetProgress(0,100," ");
				if(pCompLife)
					pCompLife->SetCaption("0/250",0);
				if(pCompCartoon)
					pCompCartoon->SetCartoonAction("","",0);
				if(pCompRv)
					pCompRv->SetVisible(false);
				if(pCompAc)
				{
					pCompAc->SetVisible(true);
					pCompAc->SetViewSpr("BT_CHUZHAN.png");
				}
				for(int k = 0; k < SLAVE_SKILL_MAX; k ++)
				{
					if(pCompSkill[k])
					{
						pCompSkill[k]->SetViewSpr("");
						pCompSkill[k]->SetCaption("",2);
					}
				}
			}
		}
	}
}

void SlaveMenuCtrl::UpdateHorseMenu()
{
	if(pChar->m_pHorseCenter->m_ifNumChange)
	{
		pChar->m_pHorseCenter->m_ifNumChange = false;
		for(int i = m_pHFormList->Count - 1; i >= 0; i --)
		{
			SlaveMenuObj* pObj = (SlaveMenuObj*)m_pHFormList->Items[i];
			int nCount = pChar->m_pHorseCenter->m_pHorseList->Count;
			pObj->m_pForm->pCFrame->SetFPCnt(nCount,true);
			for(int j = 0; j < nCount; j ++)
			{
				//显示头像
				HorseCData* pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[j];
				if(pHorse)
				{
				//	if (pHorse->m_pSkinObj)
					{
						char szIcon[32];
						sprintf(szIcon,"head/%d.png",pHorse->m_pSkinObj->nHead);
						pObj->m_pForm->pCFrame->GetFPComp(j,"zuoqi1")->SetViewSpr(szIcon);
					}
				}
			}
		}
	}
	
	for(int i = m_pHFormList->Count - 1; i >= 0; i --)
	{
		SlaveMenuObj* pObj = (SlaveMenuObj*)m_pHFormList->Items[i];
		if(pObj->m_pForm->bOpened)
		{
			if(pChar->m_pHorseCenter->m_pHorseList->Count == 0)
				pObj->byCur = 0;
			else if(pObj->byCur == 0 || pObj->byCur > pChar->m_pHorseCenter->m_pHorseList->Count)
				pObj->byCur = 1;
			if(m_nHCur != pObj->byCur)
				m_nHCur = pObj->byCur;
			if(m_byHDire != pObj->byDirection)
				m_byHDire = pObj->byDirection;
			int nFrmCount = pObj->m_pForm->pCFrame->GetFPCnt();
			for(int k = 0; k < nFrmCount && k < pChar->m_pHorseCenter->m_pHorseList->Count; k ++)
			{
				if( k == pObj->byCur-1)
					pObj->m_pForm->pCFrame->GetFPComp(k,"zuoqi1")->SetAddSprByTag("MB_XZ1.png",-15,-15,1);
				else
					pObj->m_pForm->pCFrame->GetFPComp(k,"zuoqi1")->SetAddSprByTag("",0,0,1);
			}
		}
		else
		{
			if(pObj->byCur != m_nHCur)
				pObj->byCur = m_nHCur;
			if(pObj->byDirection != m_byHDire)
				pObj->byDirection = m_byHDire;
		}
		if(pObj->m_pForm && pObj->m_pForm->bOpened)
		{
			
			HorseCData* pHorse = NULL;
			if(pObj->byCur > 0 && pObj->byCur <= pChar->m_pHorseCenter->m_pHorseList->Count)
				pHorse = (HorseCData*)pChar->m_pHorseCenter->m_pHorseList->Items[pObj->byCur-1];
			DComponent* pCompLv = pObj->m_pForm->GetComponent("lv");			
			DComponent* pComppz = pObj->m_pForm->GetComponent("pz");
			DComponent* pCompsp = pObj->m_pForm->GetComponent("speed");
			DComponent* pCompql = pObj->m_pForm->GetComponent("ql");
			DComponent* pComphp = pObj->m_pForm->GetComponent("hp");
			DComponent* pCompmp = pObj->m_pForm->GetComponent("mp");
			DComponent* pCompwg = pObj->m_pForm->GetComponent("wg");
			DComponent* pCompwf = pObj->m_pForm->GetComponent("wf");
			DComponent* pCompmg = pObj->m_pForm->GetComponent("mg");
			DComponent* pCompmf = pObj->m_pForm->GetComponent("mf");
			DComponent* pCompna = pObj->m_pForm->GetComponent("name");
			DComponent*	pCompCartoon = pObj->m_pForm->GetComponent("model");
			DComponent* pCompAc = pObj->m_pForm->GetComponent("chuzhan");
			if(pHorse)
			{//属性显示
				if(pCompLv){
					pCompLv->SetCaptionEx("%d",0,pHorse->m_byLevel);
					pCompLv->m_nFontSize = 32;//===zjk
					pCompLv->SetSprColor(255,255,255);
				}
				if(pComppz)
					pComppz->SetCaptionEx("%s",0,s_qualityName[pHorse->m_pBasic->byQuality]);
				if(pCompsp)
					pCompsp->SetCaptionEx("%d",0,pHorse->m_pBasic->nSpeed);
				if(pCompql)
					pCompql->SetCaptionEx("%d",0,pHorse->m_nPotential[HORSE_CULTIVATE_TOTAL]);
				if(pComphp)
					pComphp->SetCaptionEx("%d",0,pHorse->m_nParam[HORSE_PARAM_HPMAX]);
				if(pCompmp)
					pCompmp->SetCaptionEx("%d",0,pHorse->m_nParam[HORSE_PARAM_MPMAX]);
				if(pCompwg)
					pCompwg->SetCaptionEx("%d",0,pHorse->m_nParam[HORSE_PARAM_AD]);
				if(pCompwf)
					pCompwf->SetCaptionEx("%d",0,pHorse->m_nParam[HORSE_PARAM_DD]);
				if(pCompmg)
					pCompmg->SetCaptionEx("%d",0,pHorse->m_nParam[HORSE_PARAM_AP]);
				if(pCompmf)
					pCompmf->SetCaptionEx("%d",0,pHorse->m_nParam[HORSE_PARAM_DP]);
				if(pCompna)
				{
					pCompna->SetCaption(pHorse->m_pBasic->pName,1);
					pCompna->SetSprColor(nItemColor[pHorse->m_pBasic->byQuality][0],nItemColor[pHorse->m_pBasic->byQuality][1],nItemColor[pHorse->m_pBasic->byQuality][2]);
				}
				if(pCompAc)
				{
					if(pHorse->m_ifEquip)
						pCompAc->SetViewSpr("BT_XIUXI.png");
					else
						pCompAc->SetViewSpr("BT_QICHENG.png");
				}
				if(pCompCartoon)
				{
					char szList[64];
					char szFile[64];
					sprintf(szList,"skin/%d/1.plist",pHorse->m_nSkin);
					sprintf(szFile,"skin/%d/hstand/3",pHorse->m_nSkin);
					pCompCartoon->SetCartoonAction(szList,szFile,600,pHorse->m_pSkinObj?pHorse->m_pSkinObj->nScale:100);
					if (m_nHorsePY[i]!=pHorse->m_pSkinObj->pnVal[3])
					{
						pCompCartoon->SetPos(pCompCartoon->nX,pCompCartoon->nY-pHorse->m_pSkinObj->pnVal[3]+m_nHorsePY[i]);
						m_nHorsePY[i] = pHorse->m_pSkinObj->pnVal[3];
					}
				}
			}
			else
			{
				if(pCompLv){
					pCompLv->SetCaption("",0);					
				}
				if(pComppz)
					pComppz->SetCaption("",0);
				if(pCompsp)
					pCompsp->SetCaption("",0);
				if(pCompql)
					pCompql->SetCaption("",0);
				if(pComphp)
					pComphp->SetCaption("",0);
				if(pCompmp)
					pCompmp->SetCaption("",0);
				if(pCompwg)
					pCompwg->SetCaption("",0);
				if(pCompwf)
					pCompwf->SetCaption("",0);
				if(pCompmg)
					pCompmg->SetCaption("",0);
				if(pCompmf)
					pCompmf->SetCaption("",0);
				if(pCompna)
					pCompna->SetCaption("",0);
				if(pCompCartoon)
					pCompCartoon->SetCartoonAction("","",0);
				if(pCompAc)
					pCompAc->SetViewSpr("BT_QICHENG.png");
			}
		}
	}
}

void SlaveMenuCtrl::Update()
{
	UpdateSlaveMenu();
	UpdateHorseMenu();
}

void SlaveMenuCtrl::ShowIcons(BYTE byType,DComponent** ppCom,int nCnt,int nPage /* = 1 */)
{
	xnList* pList = NULL;
	if(byType == 0)
		pList = pChar->m_pSlaveCenter->m_pSlaveList;
	else if(byType == 1)
		pList = pChar->m_pHorseCenter->m_pHorseList;
	if(pList == NULL)
		return;
	char szIcon[32];
	for(int i = 0; i < nCnt; i ++)
	{
		if(nCnt*(nPage-1)+i < pList->Count)
		{
			sprintf(szIcon,"");
			if(byType == 0)
			{
				SlaveCData* pSlave = (SlaveCData*)pList->Items[i];
				ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pSlave->m_pBasic->nSkin);
				if(pSkin)
					sprintf(szIcon,"head/%d.png",pSkin->nHead);	
			}
			else
			{
				HorseCData* pHorse = (HorseCData*)pList->Items[i];
				ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pHorse->m_nSkin);
				if(pSkin)
					sprintf(szIcon,"head/%d.png",pSkin->nHead);	
			}
			ppCom[i]->SetViewSpr(szIcon);
		}
		else
		{
			ppCom[i]->SetViewSpr("");
		}
	}
}

void SlaveMenuCtrl::ShowIcon(DComponent* pCom,int nType,BYTE byT)
{
	if(pCom == NULL)
		return;
	char szIcon[32];
	sprintf(szIcon,"");
	if(byT == 0)
	{
		SlaveBasic* pBasic = SlaveBaseCfg::GetInstance().GetBaseInfo(nType);
		if(pBasic)
		{
			ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pBasic->nSkin);
			if(pSkin)
				sprintf(szIcon,"head/%d.png",pSkin->nHead);
		}

	}
	else if(byT == 1)
	{
		HorseBasic* pBasic = HorseBaseCfg::GetInstance().GetBasic(nType);
		if(pBasic)
		{
			ObjSkinInfo* pSkin = CfgObjSkin::GetInstance().GetObjSkin(pBasic->nSkin);
			if(pSkin)
				sprintf(szIcon,"head/%d.png",pSkin->nHead);
		}

	}
	pCom->SetViewSpr(szIcon);
//	pCom->SetViewSpr("BT_GOU.png");
}