#include "UserData.h"
#include "MissonMenu.h"
#include "QuestConfig.h"
#include "MainMenu.h"
#include "MapStaticCfg.h"
#include "MapMouse.h"
#include "GuideCenter.h"

void CBFrameMissionEvent(int nPos,char* szName)
{
	pMainMenu->m_pMissionMenu->SetSelQuest(nPos);
}

void TrackAcceptEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	QuestObj* pQObj	= (QuestObj*)param;
	//寻路
	BuildPacketEx(pPacket,USER,C_QUEST_ACCEPT,buf,256);
	SET_DATA(pSend,USER,C_QUEST_ACCEPT,pPacket);
	pSend->wQid	= pQObj->wQID;
	pChar->SendPacket(pPacket);
}

void TrackCompleteEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	QuestObj* pQObj	= (QuestObj*)param;
	//寻路
	BuildPacketEx(pPacket,USER,C_QUEST_COMPLETE,buf,256);
	SET_DATA(pSend,USER,C_QUEST_COMPLETE,pPacket);
	pSend->wQid	= pQObj->wQID;
	pChar->SendPacket(pPacket);
}

void CBFrameTraceEvent(InterfaceEvent* pEvent, int nPos,char* szName,void* param)
{
	CCtrlFrame* pTrack	= (CCtrlFrame*)param;
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_DOWN:
		{
			DComponent*	pComp	= pTrack->GetFPComp(nPos,"t1");
			pComp->SetViewSpr("MB_XUANZHONG_RW.png");
		}
		break;
	case INTERFACE_EVENT_UP:
		{
			DComponent*	pComp	= pTrack->GetFPComp(nPos,"t1");
			pComp->SetViewSpr("");
		}
		break;
	case INTERFACE_EVENT_CLICK:
		{
			DComponent*	pComp	= pTrack->GetFPComp(nPos,"t1");
			if(strlen(pComp->szCaption))
			{//设置个空格
				QuestObj*	pQObj	= (QuestObj*)pTrack->GetFPTag(nPos);

				pMainMenu->m_pMissionMenu->m_pFormTrack->Open(2);
				pMainMenu->m_pMissionMenu->m_pDCompModel->SetVisible(false);
				pMainMenu->m_pMissionMenu->UpViewQuestComp(pMainMenu->m_pMissionMenu->m_pFormTrack,pQObj,NULL);

				ClientQuest* pCQ	= pChar->m_pCQuestCenter->GetCurQuest(pQObj->wQID);

				if(pCQ && pCQ->bOk)
				{
					pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetViewSpr("BT_WANCHENG_RW.png");
					pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetEvent(TrackCompleteEvent,pQObj);
				}
				else
				{
					pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetViewSpr("BT_JIESHOU_RW.png");
					pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetEvent(TrackAcceptEvent,pQObj);
				}
			}
			else
			{
				PathObj*	pPObj	= (PathObj*)pTrack->GetFPTag(nPos);
				if(pPObj )
				{
					if (pPObj->wPathQuestID/100 == 51) //运送资源
					{
						bool bState = false;
						for (int i = 0; i < 8; i++)
						{
							if (g_pMapCenter->m_pMObjLockView->pStateCenter->CheckState(6001+i))
							{
								bState = true;
								break;
							}
						}
						if (!bState)
						{
							BuildPacketEx(pPacket,USER,C_QUEST_GIVEUP,buf,256);
							SET_DATA(pSend,USER,C_QUEST_GIVEUP,pPacket);
							pSend->wQid	= pPObj->wPathQuestID;
							pChar->SendPacket(pPacket);

							pMainMenu->PopTipMsg("任务变身失效，任务失败");
							return;
						}
					}

					//if (pPObj->wPathQuestID == 5011)
					//{
					//	tm	ctime	= *(tm*)xnGetDayTime();
					//	if ((ctime.tm_wday != 6)&&(ctime.tm_hour<16 || ctime.tm_hour>17))
					//	{
					//		BuildPacketEx(pPacket,USER,C_QUEST_GIVEUP,buf,256);
					//		SET_DATA(pSend,USER,C_QUEST_GIVEUP,pPacket);
					//		pSend->wQid	= pPObj->wPathQuestID;
					//		pChar->SendPacket(pPacket);

					//		pMainMenu->PopTipMsg("不在活动时间内，任务失败");
					//		return;
					//	}
					//}

					g_pMapCommand->CmdPathObj(pPObj);
					//g_pMapCommand->CmdMapPoint(pPObj->nMapindex,pPObj->nPosX,pPObj->nPosY,pPObj);
				}
			}
		}
		break;
	}
}

void MissionTypeEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MissionMenu* pMenu	= (MissionMenu*)param;
	int	nType	= 0;
	sscanf(pEvent->pComponent->szName,"page%d",&nType);
	if (nType == 1)
	{
		nType--;
		pMenu->SetViewQuest(nType,pMenu->m_byCurAcpt);
	}else {
		if (pChar->m_byLvl < 20)
		{
			pMainMenu->PopTipMsg("等级达到20级开启。。。");
		}else{
			nType--;
			pMenu->SetViewQuest(nType,pMenu->m_byCurAcpt);
		}
	}
}

void MissionAcptEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MissionMenu* pMenu	= (MissionMenu*)param;
	int	nAcpt	= 0;
	sscanf(pEvent->pComponent->szName,"p%d",&nAcpt);
	nAcpt--;
	pMenu->SetViewQuest(pMenu->m_byCurType,nAcpt);
}

void MissionAcceptEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	MissionMenu* pMenu	= (MissionMenu*)param;
	if(pMenu->m_pCurSelQuest == NULL)
		return;
	
	BuildPacketEx(pPacket,USER,C_QUEST_ACCEPT,buf,256);
	SET_DATA(pSend,USER,C_QUEST_ACCEPT,pPacket);
	pSend->wQid	= pMenu->m_pCurSelQuest->wQID;
	pChar->SendPacket(pPacket);
}

void MissionCompleteEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MissionMenu* pMenu	= (MissionMenu*)param;
	BuildPacketEx(pPacket,USER,C_QUEST_COMPLETE,buf,256);
	SET_DATA(pSend,USER,C_QUEST_COMPLETE,pPacket);
	pSend->wQid	= pMenu->m_pCurSelQuest->wQID;
	pChar->SendPacket(pPacket);

}

void MissionGiveUpEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MissionMenu* pMenu	= (MissionMenu*)param;
	BuildPacketEx(pPacket,USER,C_QUEST_GIVEUP,buf,256);
	SET_DATA(pSend,USER,C_QUEST_GIVEUP,pPacket);
	pSend->wQid	= pMenu->m_pCurSelQuest->wQID;
	pChar->SendPacket(pPacket);
}

void MissionGotoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	MissionMenu* pMenu	= (MissionMenu*)param;
	//寻路
	if(pMenu->m_pCurSelQuest == NULL)
		return;
	ClientQuest* pCQ	= pChar->m_pCQuestCenter->GetCurQuest(pMenu->m_pCurSelQuest->wQID);

	int		nDestNpc	= 0;
	if(pCQ)
	{
		if(pCQ->bOk)
			nDestNpc	= pCQ->pQBase->wNpcEnd;
	}
	else
	{
		nDestNpc	= pMenu->m_pCurSelQuest->wNpcSrc;
	}
	if(nDestNpc)
	{
		PathObj*	pNpc	= NpcPathLeadCfg::GetInstance().GetNpcPath(nDestNpc);
		if(pNpc)
		{
			//g_pMapCommand->CmdMapPoint(pNpc->nMapindex,pNpc->nPosX,pNpc->nPosY,pNpc);
			g_pMapCommand->CmdPathObj(pNpc);
		}
		pMenu->m_pForm->Close();
	}

}

MissionMenu::MissionMenu()
{
	m_pForm		= pMainInterface->GetDForm("mission");

	char	szTmp[32];
	for (int i = 0; i < 4; i++)
	{
		sprintf(szTmp,"page%d",i+1);
		m_pCompType[i]		= m_pForm->GetComponent(szTmp);
		m_pCompType[i]->SetEvent(MissionTypeEvent,this);	
	}

	m_pCompAccepted[0]		= m_pForm->GetComponent("p1");
	m_pCompAccepted[1]		= m_pForm->GetComponent("p2");
	m_pCompAccepted[0]->SetEvent(MissionAcptEvent,this);
	m_pCompAccepted[1]->SetEvent(MissionAcptEvent,this);

	m_pForm->GetComponent("bt1")->SetEvent(MissionAcceptEvent,this);
	m_pForm->GetComponent("bt2")->SetEvent(MissionCompleteEvent,this);
	m_pForm->GetComponent("bt3")->SetEvent(MissionGiveUpEvent,this);
	m_pForm->GetComponent("bt4")->SetEvent(MissionGotoEvent,this);

	

	DForm*	pTmpForm	= pMainInterface->GetDForm("main7");
	pTmpForm->pCFrame	= new CCtrlFrame(-476,-300,200,220);
	m_pTrackQuest		= pTmpForm->pCFrame;

	DComponent*	pComp	= pTmpForm->GetComponent ("t1");
	DComponent* pTmpItem[3];
	pTmpItem[0]					= pTmpForm->GetComponent ("q1");
	pTmpItem[1]					= pTmpForm->GetComponent ("name1");
	pTmpItem[1]->m_nFontSize	= 20;
	pTmpItem[2]					= pTmpForm->GetComponent ("wc1");
	pTmpItem[2]->m_nFontSize	= 18;
	m_pTrackQuest->InitFParts(pComp,pTmpItem,3);
	m_pTrackQuest->SetEventAdv(CBFrameTraceEvent,m_pTrackQuest);
	m_pTrackQuest->nSpace	= 6;
	m_pTrackQuest->nDisStep	= 56;
	m_pTrackQuest->SetFPCnt(0,false);

	
	m_pForm->pCFrame	= new CCtrlFrame(15,200,510,380);

	DComponent* pComItem[2];
	pComItem[0]					= m_pForm->GetComponent ("zi2");
	pComItem[0]->m_nFontSize	= 26;
	pComItem[0]->SetSprColor(200,180,33);
	pComItem[1]					= m_pForm->GetComponent ("zi");
	pComItem[1]->m_nFontSize	= 26;
	pComp	= m_pForm->GetComponent ("mb");

	m_pForm->pCFrame->InitFParts(pComp,pComItem,2);
	m_pForm->pCFrame->SetEvent (CBFrameMissionEvent);
	m_pForm->pCFrame->nSpace	= 14;
	m_pForm->pCFrame->nDisStep	= 66;
	m_pForm->pCFrame->SetFPCnt(5,false);
	SetViewQuest(0,0);

	m_pFormTrack		= pMainInterface->GetDForm("renwu");
	//===zjk
	DComponent*	pCompCond2	= m_pFormTrack->GetComponent("zi2");
	pCompCond2->pSTextCenter->m_byFontSize	= 20;
	pCompCond2->pSTextCenter->ClearSText();

	m_pTFCompBtn		= m_pFormTrack->GetComponent("bt");
	m_pDCompModel		= m_pFormTrack->GetComponent("model");
	m_pFormTrack->bZorde	= FORM_ZORDER_QUEST;
}

MissionMenu::~MissionMenu()
{

}

void CB_TrackQuestEvent(void* pParam)
{
	if(pParam == NULL)
		return;

	_MuliParamObj* pMp	= (_MuliParamObj*)pParam;

	if(pMp->nParam2==1)
	{
		pMainMenu->m_pMissionMenu->m_pCompTrackQuest->pSTextCenter->ChgSText(pMp->nParam1,"ui/MB_XUANZHONG_RW.png",255,255,255);
	}
	if(pMp->nParam2==2)
	{
		int	nPID	= (int)pParam&0x3FFFFFFF;
		pMainMenu->m_pMissionMenu->m_pCompTrackQuest->pSTextCenter->ChgSText(pMp->nParam1,"ui/AAA.png",255,255,255);
	}
	else
	{
		pMainMenu->m_pMissionMenu->m_pCompTrackQuest->pSTextCenter->ChgSText(pMp->nParam1,"ui/AAA.png",255,255,255);
		QuestObj*	pQObj	= (QuestObj*)pMp->nParam1;

		pMainMenu->m_pMissionMenu->m_pFormTrack->Open(2);
		pMainMenu->m_pMissionMenu->m_pDCompModel->SetVisible(false);
		pMainMenu->m_pMissionMenu->UpViewQuestComp(pMainMenu->m_pMissionMenu->m_pFormTrack,pQObj,NULL);

		ClientQuest* pCQ	= pChar->m_pCQuestCenter->GetCurQuest(pQObj->wQID);

		if(pCQ && pCQ->bOk)
		{
			pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetViewSpr("BT_WANCHENG_RW.png");
			pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetEvent(TrackCompleteEvent,pQObj);
		}
		else
		{
			pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetViewSpr("BT_JIESHOU_RW.png");
			pMainMenu->m_pMissionMenu->m_pTFCompBtn->SetEvent(TrackAcceptEvent,pQObj);
		}
	}

}

void CB_TrackPathEvent(void* pParam)
{
	if(pParam == NULL)
		return;
	
	_MuliParamObj* pMp	= (_MuliParamObj*)pParam;

	if(pMp->nParam2==1)
	{
		pMainMenu->m_pMissionMenu->m_pCompTrackQuest->pSTextCenter->ChgSText(pMp->nParam1,"ui/MB_XUANZHONG_RW.png",255,255,255);
	}
	else if(pMp->nParam2==2)
	{
		pMainMenu->m_pMissionMenu->m_pCompTrackQuest->pSTextCenter->ChgSText(pMp->nParam1,"ui/AAA.png",255,255,255);
	}
	else
	{
		pMainMenu->m_pMissionMenu->m_pCompTrackQuest->pSTextCenter->ChgSText(pMp->nParam1,"ui/AAA.png",255,255,255);
		PathObj*	pPObj	= (PathObj*)pMp->nParam1;
		if(pPObj )
		{
			g_pMapCommand->CmdPathObj(pPObj);
			//g_pMapCommand->CmdMapPoint(pPObj->nMapindex,pPObj->nPosX,pPObj->nPosY,pPObj);
		}
	}
}

void MissionMenu::Update()
{
	if(m_pForm->bOpened)
	{
		if(pChar->m_pCQuestCenter->m_bUpdateQuestChg)
			SetViewQuest(m_byCurType,m_byCurAcpt);
	}

	_updateQuickTrack();
}

void MissionMenu::Open()
{
	m_pForm->Open(2);
	pChar->m_pCQuestCenter->ReCheckCanAccept();
	if(pChar->m_pCQuestCenter->m_pListQuest->Count)
		SetViewQuest(m_byCurType,true);
	else
		SetViewQuest(m_byCurType,false);
}

void MissionMenu::SetViewQuest( BYTE bType, bool bAccept )
{
	m_byCurType	= bType;
	m_byCurAcpt	= bAccept;

	int nDestType	= bType;
	if(bType == 2)
		nDestType	= 4;
	for (int i = 0; i < 4; i++)
	{
		if(i == bType)
			m_pCompType[i]->SetSprColor(255,255,255);
		else
			m_pCompType[i]->SetSprColor(127,127,127);
	}

	if(bAccept)
	{
		m_pCompAccepted[1]->SetSprColor(255,255,255);
		m_pCompAccepted[0]->SetSprColor(127,127,127);
	}
	else
	{
		m_pCompAccepted[1]->SetSprColor(127,127,127);
		m_pCompAccepted[0]->SetSprColor(255,255,255);
	}
	xnList*	pList	= xnList::Create();
	if(bAccept)
	{
		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
		{
			ClientQuest* pCQ	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
			if((nDestType == 0 || nDestType == 4)&&pCQ->pQBase->bType == nDestType)
			{
				pList->Add(pCQ->pQBase);
			}else if(nDestType != 0 && nDestType != 4 && pCQ->pQBase->bType != 0 && pCQ->pQBase->bType != 4 )
			{
				pList->Add(pCQ->pQBase);
			}
		}
	}
	else
	{
		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListCanAccpet->Count; i++)
		{
			QuestObj* pQBase	= (QuestObj*)pChar->m_pCQuestCenter->m_pListCanAccpet->Items[i];
			if(pQBase->bType == nDestType)
			{
				if(pQBase->wLine && pChar->m_pCQuestCenter->CheckQuestByLine(pQBase->wLine,pList) == false)
					pList->Add(pQBase);
			}
		}
	}

	m_pForm->pCFrame->SetFPCnt(pList->Count);
	for (int i = 0; i < pList->Count; i++)
	{
		QuestObj* pQObj	= (QuestObj*)pList->Items[i];
		m_pForm->pCFrame->SetFPCompCaption(i,"zi2",pQObj->szName,0);
		if (pQObj->bLvNeed)
		{
			char	szTmp[32];
			sprintf(szTmp,"[%d级]",pQObj->bLvNeed);
			m_pForm->pCFrame->SetFPCompCaption(i,"zi",szTmp,0);
		}else
			m_pForm->pCFrame->SetFPCompCaption(i,"zi","",0);
		m_pForm->pCFrame->SetFPTag(i,(void*)pQObj);
	}
	SetSelQuest(0);
	pList->Free();
}

void MissionMenu::UpViewQuestComp( DForm* pForm, QuestObj* pQBase, ClientQuest* pClientQ )
{
	if(pQBase == NULL)
	{
		pForm->GetComponent("zi1")->SetVisible(false);
		pForm->GetComponent("gezi1")->SetVisible(false);
		pForm->GetComponent("gezi2")->SetVisible(false);
		pForm->GetComponent("icon1")->SetVisible(false);
		pForm->GetComponent("icon2")->SetVisible(false);
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
		pForm->GetComponent("gezi1")->SetVisible(true);
		pForm->GetComponent("gezi2")->SetVisible(true);
		pForm->GetComponent("icon1")->SetVisible(true);
		pForm->GetComponent("icon2")->SetVisible(true);
		pForm->GetComponent("zi2C8")->SetVisible(true);
		pForm->GetComponent("zi2")->SetVisible(true);
		pForm->GetComponent("di2")->SetVisible(true);
		pForm->GetComponent("rw")->SetVisible(true);		
		pForm->GetComponent("exp")->SetVisible(true);
		pForm->GetComponent("money")->SetVisible(true);
	}

	DComponent*	pCompCond	= pForm->GetComponent("zi1");
	pCompCond->pSTextCenter->m_byFontSize	= 20;
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
			sprintf(szTxt,"物品");

		pCompCond->pSTextCenter->AddSText(nIndex++,0,true,szTxt,10,220,10);
	}

	if(pQBase->nAwardExp == 0 && pQBase->nAwardGold == 0)
	{
		pForm->GetComponent("exp")->SetCaption("特殊",1);
		pForm->GetComponent("money")->SetCaption("特殊",1);
	}
	else
	{
		pForm->GetComponent("exp")->SetCaptionEx("%d",1,pQBase->nAwardExp);
		pForm->GetComponent("money")->SetCaptionEx("%d",1,pQBase->nAwardGold);
	}
	if(pQBase->bType == 4)
		pForm->GetComponent("jb")->SetViewSpr("MB_RONGYU1.png");
	else
		pForm->GetComponent("jb")->SetViewSpr("MB_MONEY.png");

	DComponent*	pCompItemB[2];
	pCompItemB[0]	= pForm->GetComponent("gezi1");
	pCompItemB[1]	= pForm->GetComponent("gezi2");
	DComponent*	pCompItem[2];
	pCompItem[0]	= pForm->GetComponent("icon1");
	pCompItem[1]	= pForm->GetComponent("icon2");

	int	nUserPos	= 0;

	for (int i = 0; i < pQBase->pLAwardItem->Count; i++)
	{
		if(nUserPos >= 2)
			break;
		_MuliParamObj* pMP	= (_MuliParamObj*)pQBase->pLAwardItem->Items[i];
		if(pMP->nParam1 >= 10000)
		{
			ItemExBase*	pIBase	= ItemConfig::GetInstance().GetItemBase(pMP->nParam1);
			if(pQBase->bSelOne)
			{
				if(pIBase == NULL || !(pIBase->nJob&(1<<pChar->m_byJob)))
					continue;	//多选一情况下只得到本职业物品
			}
// 			char sztxt[32];
// 			sprintf(sztxt,"item/%d.png",pIBase->nSpr);
// 			pCompItem[nUserPos]->SetViewSpr(sztxt);
			ShowItemInfoOnComp(pCompItem[nUserPos],pMP->nParam1,pMP->nParam2);
			nUserPos++;
		}
		else if(pMP->nParam1 < 2000)
		{//坐骑
			pCompItem[nUserPos]->eventFunc = NULL;
			pCompItem[nUserPos]->SetCaption("",1);
			g_pSlaveMenuCtrl->ShowIcon(pCompItem[nUserPos],pMP->nParam1,1);
			nUserPos++;
		}
		else
		{//宠物
			pCompItem[nUserPos]->eventFunc = NULL;
			pCompItem[nUserPos]->SetCaption("",1);
			g_pSlaveMenuCtrl->ShowIcon(pCompItem[nUserPos],pMP->nParam1,0);
			nUserPos++;
		}
	}

	for (int i = 0; i < 2; i ++)
	{
		pCompItemB[i]->SetVisible(i<nUserPos);
		pCompItem[i]->SetVisible(i<nUserPos);
	}

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

void MissionMenu::SetSelQuest( int nPos )
{
	m_pCurSelQuest	= (QuestObj*)m_pForm->pCFrame->GetFPTag(nPos);
	UpViewQuestComp(m_pForm,m_pCurSelQuest,NULL);

	m_pForm->GetComponent("bt1")->SetVisible(false);
	m_pForm->GetComponent("bt2")->SetVisible(false);
	m_pForm->GetComponent("bt3")->SetVisible(false);
	m_pForm->GetComponent("bt4")->SetVisible(false);
	if(m_pCurSelQuest)
	{
		ClientQuest* pCQ	= pChar->m_pCQuestCenter->GetCurQuest(m_pCurSelQuest->wQID);
		if(pCQ)
		{
			if(pCQ->bOk)
			{
				if(pCQ->pQBase->wNpcEnd)
					m_pForm->GetComponent("bt4")->SetVisible(true);
				else
					m_pForm->GetComponent("bt2")->SetVisible(true);

				m_pForm->GetComponent("bt4")->SetPosX(550);
				m_pForm->GetComponent("bt2")->SetPosX(550);
				m_pForm->GetComponent("bt3")->SetPosX(728);
				m_pForm->GetComponent("bt3")->SetVisible(true);
			}
			else
			{
				m_pForm->GetComponent("bt3")->SetPosX(642);
				m_pForm->GetComponent("bt3")->SetVisible(true);
			}
		}
		else
		{
			if(m_pCurSelQuest->wNpcSrc)
				m_pForm->GetComponent("bt4")->SetVisible(true);
			else
				m_pForm->GetComponent("bt1")->SetVisible(true);
			//m_pForm->GetComponent("bt4")->SetVisible(true);
		}
	}
}

bool MissionMenu::CheckNpcQ( int nNpcID )
{
	for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
	{
		ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
		if(pCQuest->bOk)
		{
			if(pCQuest->pQBase->wNpcEnd == nNpcID)
			{
				m_pFormTrack->Open(2);
				UpViewQuestComp(m_pFormTrack,pCQuest->pQBase,pCQuest);
				m_pTFCompBtn->SetViewSpr("BT_WANCHENG_RW.png");
				m_pTFCompBtn->SetEvent(TrackCompleteEvent,pCQuest->pQBase);


				PathObj* pNpc	= NpcPathLeadCfg::GetInstance().GetNpcPath(nNpcID);
				if(pNpc)
				{
					char szList[64];
					char szFile[64];
					sprintf(szList,"skin/%d/1.plist",pNpc->nSkin);
					sprintf(szFile,"skin/%d/%s/5",pNpc->nSkin,"stand");
					m_pDCompModel->SetCartoonAction(szList,szFile,600,100);
					m_pDCompModel->SetVisible(true);
				}

				return true;
			}
		}
	}
	xnList*	pRtlList	= xnList::Create();
	for (int i = 0; i < pChar->m_pCQuestCenter->m_pListCanAccpet->Count; i++)
	{
		QuestObj* pQObj	= (QuestObj*)pChar->m_pCQuestCenter->m_pListCanAccpet->Items[i];
		
		if(pQObj->wNpcSrc == nNpcID)
		{
			pRtlList->Add(pQObj);
		}
	}
	if(pRtlList->Count)
	{//所有可接任务中随机一个
		QuestObj* pQObj	= (QuestObj*)pRtlList->Items[rand()%pRtlList->Count];
		m_pFormTrack->Open(2);
		PathObj* pNpc	= NpcPathLeadCfg::GetInstance().GetNpcPath(nNpcID);
		if(pNpc)
		{
			char szList[64];
			char szFile[64];
			sprintf(szList,"skin/%d/1.plist",pNpc->nSkin);
			sprintf(szFile,"skin/%d/%s/5",pNpc->nSkin,"stand");
			m_pDCompModel->SetCartoonAction(szList,szFile,600,100);
			m_pDCompModel->SetVisible(true);
		}
		UpViewQuestComp(m_pFormTrack,pQObj,NULL);
		m_pTFCompBtn->SetViewSpr("BT_JIESHOU_RW.png");
		m_pTFCompBtn->SetEvent(TrackAcceptEvent,pQObj);
		pRtlList->Free();
		return true;
	}
	pRtlList->Free();
	return false;
}

void MissionMenu::_updateQuickTrack()
{
	//更新快速追踪界面
	if(pChar->m_pCQuestCenter->m_bUpdateQuestChg)
	{
		pMainMenu->m_pMiniMap->m_bGuideMark	= true;
		NpcPathLeadCfg::GetInstance().ResetMark();
		MonsterPathLeadCfg::GetInstance().ResetMark();

		xnList*	pListTmpA	= xnList::Create();		//主线已接
		xnList*	pListTmpB	= xnList::Create();		//主线可接
		xnList*	pListTmpC	= xnList::Create();		//支线已接
		xnList*	pListTmpD	= xnList::Create();		//支线可接

		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
			pCQuest->bChged	= false;
			if(pCQuest->pQBase->bType == 0)
				pListTmpA->Add(pCQuest);
			else
				pListTmpC->Add(pCQuest);
		}
		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListCanAccpet->Count; i++)
		{
			QuestObj* pQBase	= (QuestObj*)pChar->m_pCQuestCenter->m_pListCanAccpet->Items[i];
			if(pQBase->wLine && pChar->m_pCQuestCenter->CheckQuestByLine(pQBase->wLine,pListTmpC))
				continue;
			
			if(pQBase->bType != 0 && pQBase->bType != 1 && pQBase->bType != 4)
				continue;
			
			if(pQBase->bType == 0)
				pListTmpB->Insert(0,pQBase);
			else
				pListTmpD->Add(pQBase);
		}

		m_pTrackQuest->SetFPCnt(pListTmpA->Count+pListTmpB->Count+pListTmpC->Count+pListTmpD->Count);
		
		int nPos	= 0;
		for (int i = 0; i < pListTmpA->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pListTmpA->Items[i];

			_trackQuest(nPos++,pCQuest);
		}
		for (int i = 0; i < pListTmpB->Count; i++)
		{
			QuestObj* pQBase	= (QuestObj*)pListTmpB->Items[i];

			_trackQBase(nPos++,pQBase);
		}
		for (int i = 0; i < pListTmpC->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pListTmpC->Items[i];

			_trackQuest(nPos++,pCQuest);
		}
		for (int i = 0; i < pListTmpD->Count; i++)
		{
			QuestObj* pQBase	= (QuestObj*)pListTmpD->Items[i];

			_trackQBase(nPos++,pQBase);
		}
		pChar->m_pCQuestCenter->m_bUpdateQuestChg	= false;
		pListTmpA->Free();
		pListTmpB->Free();
		pListTmpC->Free();
		pListTmpD->Free();
	}
	else
	{
		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
			if(pCQuest->bOk || pCQuest->bChged == false)
				continue;

			char	szTmp[256];
			for (int j = 0; j < pCQuest->pQBase->pLQCondtion->Count; j++)
			{
				_QConditon* pQC	= (_QConditon*)pCQuest->pQBase->pLQCondtion->Items[j];

				if(pQC->bDType	== 1)
				{
					if(pCQuest->pszVal[pQC->bValPos] > pQC->wDNum)
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
					else
					{
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pCQuest->pszVal[pQC->bValPos],pQC->wDNum);
						break;
					}
				}
				else if(pQC->bDType == 2||pQC->bDType == 3)
				{
					int nItemCnt	= pChar->m_pItemCenter->GetItemCountByID(pQC->wDID);
					if(nItemCnt > pQC->wDNum)
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
					else
					{
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,nItemCnt,pQC->wDNum);
						break;
					}
				}
				else
					sprintf(szTmp,"(未完成)");
			}
			for (int j = 0; j < m_pTrackQuest->GetFPCnt(); j++)
			{
				if(strcmp(m_pTrackQuest->GetFPComp(j,"name1")->szCaption,pCQuest->pQBase->szName)==0)
				{
					m_pTrackQuest->GetFPComp(j,"wc1")->SetCaption(szTmp,0);
				}
			}
			pCQuest->bChged	= false;
		}
	}
}

void MissionMenu::_trackQuest( int nPos, ClientQuest* pCQuest )
{
	char	szTmp[256];
	if(pCQuest->pQBase->bType == 0)
		sprintf(szTmp,"MB_RW1.png");
	else if(pCQuest->pQBase->bType == 4)
		sprintf(szTmp,"MB_RW5.png");
	else
		sprintf(szTmp,"MB_RW2.png");

	m_pTrackQuest->GetFPComp(nPos,"q1")->SetViewSpr(szTmp);

	DComponent*	pComp	= m_pTrackQuest->GetFPComp(nPos,"name1");
	pComp->SetCaption(pCQuest->pQBase->szName,0);

	if(pCQuest->bOk)
	{
		pComp->SetSprColor(113,164,70);
		pComp	= m_pTrackQuest->GetFPComp(nPos,"wc1");
		pComp->SetSprColor(113,164,70);
		pComp->SetCaption("(完成)",0);

		if(pCQuest->pQBase->wNpcEnd)
		{
			m_pTrackQuest->GetFPComp(nPos,"t1")->SetCaption("",1);	//任务类追踪
			PathObj* pObj	= NpcPathLeadCfg::GetInstance().GetNpcPath(pCQuest->pQBase->wNpcEnd);
			pObj->wPathQuestID = pCQuest->wQID;
			m_pTrackQuest->SetFPTag(nPos,(void*)pObj);
			if(pObj)
				pObj->byMark		= 2;
		}
		else
		{
			m_pTrackQuest->GetFPComp(nPos,"t1")->SetCaption(" ",1);
			m_pTrackQuest->SetFPTag(nPos,(void*)pCQuest->pQBase);
		}
	}
	else
	{
		pComp->SetSprColor(213,213,18);
		pComp	= m_pTrackQuest->GetFPComp(nPos,"wc1");
		pComp->SetSprColor(255,255,255);

		for (int j = 0; j < pCQuest->pQBase->pLQCondtion->Count; j++)
		{
			_QConditon* pQC	= (_QConditon*)pCQuest->pQBase->pLQCondtion->Items[j];
			bool	bBreak	= false;
			PathObj* pObj	= NULL;
			if(pQC->bDType	== 1)
			{
				pObj	= MonsterPathLeadCfg::GetInstance().GetPathObj(pQC->wDID);
				if(pCQuest->pszVal[pQC->bValPos] > pQC->wDNum)
					sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
				else
				{
					sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pCQuest->pszVal[pQC->bValPos],pQC->wDNum);
					bBreak	= true;
				}
			}
			else if(pQC->bDType == 2||pQC->bDType == 3)
			{
				int nItemCnt	= pChar->m_pItemCenter->GetItemCountByID(pQC->wDID);

				for (int j = 0; j < pCQuest->pQBase->pLMonDrop->Count; j++)
				{
					_QMonDrop* pQMD	= (_QMonDrop*)pCQuest->pQBase->pLMonDrop->Items[j];
					if(pQMD->wItemID == pQC->wDID)
					{
						pObj	= MonsterPathLeadCfg::GetInstance().GetPathObj(pQMD->wMonID);
						break;
					}
				}

				if(nItemCnt > pQC->wDNum)
					sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
				else
				{
					sprintf(szTmp,"%s:%d/%d",pQC->szTitle,nItemCnt,pQC->wDNum);
					bBreak	= true;
				}
			}
			else
				sprintf(szTmp,"(未完成)");

			if(pObj)
				pObj->byMark	= 3;

			pComp->SetCaption(szTmp,0);

			m_pTrackQuest->GetFPComp(nPos,"t1")->SetCaption("",1);
			m_pTrackQuest->SetFPTag(nPos,(void*)pObj);
			if(bBreak)	//有一个追踪条件就break
				break;
		}
	}
}

void MissionMenu::_trackQBase( int nPos, QuestObj* pQBase )
{
	char	szTmp[256];
	if(pQBase->bType == 0)
		sprintf(szTmp,"MB_RW1.png");
	else if(pQBase->bType == 4)
		sprintf(szTmp,"MB_RW5.png");
	else
		sprintf(szTmp,"MB_RW2.png");

	m_pTrackQuest->GetFPComp(nPos,"q1")->SetViewSpr(szTmp);

	DComponent*	pComp	= m_pTrackQuest->GetFPComp(nPos,"name1");
	pComp->SetCaption(pQBase->szName,0);

	pComp->SetSprColor(11,161,216);

	pComp	= m_pTrackQuest->GetFPComp(nPos,"wc1");
	if(pQBase->bLvNeed > pChar->m_byLvl)
	{
		pComp->SetSprColor(111,111,122);
		pComp->SetCaptionEx("(等级%d可接)",0,pQBase->bLvNeed);
	}
	else
	{
		pComp->SetSprColor(11,161,216);
		pComp->SetCaption("(可接)",0);
	}

	PathObj* pObj	= NULL;
	if(pQBase->wNpcSrc)
		pObj	= NpcPathLeadCfg::GetInstance().GetNpcPath(pQBase->wNpcSrc);
	if(pObj)
	{
		m_pTrackQuest->GetFPComp(nPos,"t1")->SetCaption("",1);
		m_pTrackQuest->SetFPTag(nPos,(void*)pObj);
		pObj->byMark		= 1;
	}
	else
	{
		m_pTrackQuest->GetFPComp(nPos,"t1")->SetCaption(" ",1);
		m_pTrackQuest->SetFPTag(nPos,(void*)pQBase);
	}
}

/*
void MissionMenu::_updateQuickTrack()
{
	//更新快速追踪界面
	if(pChar->m_pCQuestCenter->m_bUpdateQuestChg)
	{
		pMainMenu->m_pMiniMap->m_bGuideMark	= true;
		NpcPathLeadCfg::GetInstance().ResetMark();
		MonsterPathLeadCfg::GetInstance().ResetMark();

		int		nIndex	= 0;
		m_pCompTrackQuest->pSTextCenter->ClearSText();

		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
			pCQuest->bChged	= false;
			if(pCQuest->bOk)
			{
				char	szTmp[256];
				if(pCQuest->pQBase->bType == 0)
					sprintf(szTmp,"ui/MB_RW1.png");
				else
					sprintf(szTmp,"ui/MB_RW2.png");

				STextObj*	pSTO1	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,1,true,szTmp,255,255,255);
				sprintf(szTmp,"%-12s",pCQuest->pQBase->szName);
				STextObj*	pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,false,szTmp,113,164,70);
				pSTO->nFontSize		= 18;

				//sprintf(szTmp,"%22s","(完成)");
				sprintf(szTmp,"%-12s","(完成)");
				if(pCQuest->pQBase->wNpcEnd)
				{
					PathObj* pObj	= NpcPathLeadCfg::GetInstance().GetNpcPath(pCQuest->pQBase->wNpcEnd);
					//sprintf(szTmp,"    %s",pObj->szName);
					pSTO->funCBLink		= CB_TrackPathEvent;
					pSTO->pParam		= pObj;
					pSTO->bTouchEvent	= true;
					pSTO1->funCBLink		= CB_TrackPathEvent;
					pSTO1->pParam		= pObj;
					pSTO1->bTouchEvent	= true;
					pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,true,szTmp,113,164,70,pObj,CB_TrackPathEvent);
					pSTO->bTouchEvent	= true;
					pSTO->nOffY	= -24/POINT_PIX_CNT;
					pSTO->nOffX	= 50/POINT_PIX_CNT;
					if(pObj)
						pObj->byMark		= 2;
				}
				else
				{
					pSTO->funCBLink		= CB_TrackQuestEvent;
					pSTO->pParam		= pCQuest->pQBase;
					pSTO->bTouchEvent	= true;
					pSTO1->funCBLink	= CB_TrackQuestEvent;
					pSTO1->pParam		= pCQuest->pQBase;
					pSTO1->bTouchEvent	= true;
					pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,true,szTmp,113,164,70,pCQuest->pQBase,CB_TrackQuestEvent);
					pSTO->bTouchEvent	= true;
					pSTO->nOffY	= -24/POINT_PIX_CNT;
					pSTO->nOffX	= 50/POINT_PIX_CNT;
				}
				//m_pCompTrackQuest->pSTextCenter->ChgSText(pCQuest->pQBase,)
				STextObj* pSObj	= m_pCompTrackQuest->pSTextCenter->AddSText((int)pSTO->pParam,1,true,"ui/AAA.png",255,255,255);
				pSObj->nOffY	= -64/POINT_PIX_CNT;
			}
		}

		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
			if(pCQuest->bOk == false)
			{
				char	szTmp[256];
				if(pCQuest->pQBase->bType == 0)
					sprintf(szTmp,"ui/MB_RW1.png");
				else
					sprintf(szTmp,"ui/MB_RW2.png");

				STextObj*	pSTO1	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,1,true,szTmp,255,255,255);
				sprintf(szTmp,"%-12s",pCQuest->pQBase->szName);
				STextObj*	pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,false,szTmp,213,213,18);
				pSTO->nFontSize		= 18;
				for (int j = 0; j < pCQuest->pQBase->pLQCondtion->Count; j++)
				{
					_QConditon* pQC	= (_QConditon*)pCQuest->pQBase->pLQCondtion->Items[j];

					PathObj* pObj	= NULL;
					if(pQC->bDType	== 1)
					{
						pObj	= MonsterPathLeadCfg::GetInstance().GetPathObj(pQC->wDID);
						if(pCQuest->pszVal[pQC->bValPos] > pQC->wDNum)
							sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
						else
							sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pCQuest->pszVal[pQC->bValPos],pQC->wDNum);
						if(pSTO->funCBLink	== NULL)
						{
							pSTO->funCBLink		= CB_TrackPathEvent;
							pSTO->pParam		= pObj;
							pSTO->bTouchEvent	= true;
							pSTO1->funCBLink		= CB_TrackPathEvent;
							pSTO1->pParam		= pObj;
							pSTO1->bTouchEvent	= true;
						}	
					}
					else if(pQC->bDType == 2)
					{
						int nItemCnt	= pChar->m_pItemCenter->GetItemCountByID(pQC->wDID);

						for (int j = 0; j < pCQuest->pQBase->pLMonDrop->Count; j++)
						{
							_QMonDrop* pQMD	= (_QMonDrop*)pCQuest->pQBase->pLMonDrop->Items[j];
							if(pQMD->wItemID == pQC->wDID)
							{
								pObj	= MonsterPathLeadCfg::GetInstance().GetPathObj(pQMD->wMonID);
								break;
							}
						}

						if(nItemCnt > pQC->wDNum)
							sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
						else
							sprintf(szTmp,"%s:%d/%d",pQC->szTitle,nItemCnt,pQC->wDNum);
					}
					else
						sprintf(szTmp,"(未完成)");

					if(pObj)
						pObj->byMark	= 3;

					pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(i*1000+j,0,true,szTmp,255,255,255,pObj,CB_TrackPathEvent);
					pSTO->bTouchEvent	= true;
					pSTO->nOffY	= -24/POINT_PIX_CNT;
					pSTO->nOffX	= 50/POINT_PIX_CNT;
				}
				STextObj* pSObj	= m_pCompTrackQuest->pSTextCenter->AddSText((int)pSTO->pParam,1,true,"ui/AAA.png",255,255,255);
				pSObj->nOffY	= -64/POINT_PIX_CNT;
			}
		}
		xnList*	pListTmp	= xnList::Create();
		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListCanAccpet->Count; i++)
		{
			QuestObj* pQBase	= (QuestObj*)pChar->m_pCQuestCenter->m_pListCanAccpet->Items[i];
			if(pQBase->wLine && pChar->m_pCQuestCenter->CheckQuestByLine(pQBase->wLine,pListTmp))
			{
				continue;
			}
			pListTmp->Add(pQBase);
			if(pQBase->bType != 0 && pQBase->bType != 1)
			{
				continue;
			}

			char	szTmp[256];
			//if(pQBase->bType == 0)
			//	sprintf(szTmp,"[主]%s",pQBase->szName);
			//else
			//	sprintf(szTmp,"[支]%s",pQBase->szName);
			if(pQBase->bType == 0)
				sprintf(szTmp,"ui/MB_RW1.png");
			else
				sprintf(szTmp,"ui/MB_RW2.png");
			//sprintf(szTmp,"[可接]%s",pQBase->szName);
			STextObj*	pSTO1	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,1,true,szTmp,255,255,255);
			sprintf(szTmp,"%-12s",pQBase->szName);
			STextObj*	pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,false,szTmp,11,161,216);
			pSTO->nFontSize		= 18;
			
			BYTE	byR	= 11;
			BYTE	byG	= 161;
			BYTE	byB	= 216;
			if(pQBase->bLvNeed > pChar->m_byLvl)
			{
				sprintf(szTmp,"(等级%d%-10s",pQBase->bLvNeed,"可接)");
				byR	= 111;
				byG	= 111;
				byB	= 122;

			}
			else
				sprintf(szTmp,"%-12s","(可接)");


			if(pQBase->wNpcSrc)
			{
				PathObj* pObj	= NpcPathLeadCfg::GetInstance().GetNpcPath(pQBase->wNpcSrc);
				if(pObj)
				{
					//sprintf(szTmp,"    %s",pObj->szName);
					pSTO->funCBLink		= CB_TrackPathEvent;
					pSTO->pParam		= pObj;
					pSTO->bTouchEvent	= true;
					pSTO1->funCBLink		= CB_TrackPathEvent;
					pSTO1->pParam		= pObj;
					pSTO1->bTouchEvent	= true;
					pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,true,szTmp,byR,byG,byB,pObj,CB_TrackPathEvent);
					pSTO->bTouchEvent	= true;
					pSTO->nOffY		= -24/POINT_PIX_CNT;
					pSTO->nOffX		= 50/POINT_PIX_CNT;
					pObj->byMark		= 1;
				}
				else
				{
					//sprintf(szTmp,"    无");
					pSTO->funCBLink		= CB_TrackQuestEvent;
					pSTO->pParam		= pQBase;
					pSTO->bTouchEvent	= true;
					pSTO1->funCBLink		= CB_TrackQuestEvent;
					pSTO1->pParam		= pQBase;
					pSTO1->bTouchEvent	= true;
					pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,true,szTmp,byR,byG,byB,pQBase,CB_TrackQuestEvent);
					pSTO->bTouchEvent	= true;
					pSTO->nOffY		= -24/POINT_PIX_CNT;
					pSTO->nOffX		= 50/POINT_PIX_CNT;
				}
			}
			else
			{
				//sprintf(szTmp,"    无");
				pSTO->funCBLink		= CB_TrackQuestEvent;
				pSTO->pParam		= pQBase;
				pSTO->bTouchEvent	= true;
				pSTO1->funCBLink	= CB_TrackQuestEvent;
				pSTO1->pParam		= pQBase;
				pSTO1->bTouchEvent	= true;
				pSTO	= m_pCompTrackQuest->pSTextCenter->AddSText(nIndex++,0,true,szTmp,byR,byG,byB,pQBase,CB_TrackQuestEvent);
				pSTO->bTouchEvent	= true;
				pSTO->nOffY		= -24/POINT_PIX_CNT;
				pSTO->nOffX		= 50/POINT_PIX_CNT;
			}

			STextObj* pSObj	= m_pCompTrackQuest->pSTextCenter->AddSText((int)pSTO->pParam,1,true,"ui/AAA.png",255,255,255);
			pSObj->nOffY	= -64/POINT_PIX_CNT;
		}
		pListTmp->Free();
		pChar->m_pCQuestCenter->m_bUpdateQuestChg	= false;
	}
	else
	{
		for (int i = 0; i < pChar->m_pCQuestCenter->m_pListQuest->Count; i++)
		{
			ClientQuest* pCQuest	= (ClientQuest*)pChar->m_pCQuestCenter->m_pListQuest->Items[i];
			if(pCQuest->bOk || pCQuest->bChged == false)
				continue;
			for (int j = 0; j < pCQuest->pQBase->pLQCondtion->Count; j++)
			{
				_QConditon* pQC	= (_QConditon*)pCQuest->pQBase->pLQCondtion->Items[j];

				char	szTmp[256];
				if(pQC->bDType	== 1)
				{
					if(pCQuest->pszVal[pQC->bValPos] > pQC->wDNum)
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
					else
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pCQuest->pszVal[pQC->bValPos],pQC->wDNum);
				}
				else if(pQC->bDType == 2)
				{
					int nItemCnt	= pChar->m_pItemCenter->GetItemCountByID(pQC->wDID);
					if(nItemCnt > pQC->wDNum)
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,pQC->wDNum,pQC->wDNum);
					else
						sprintf(szTmp,"%s:%d/%d",pQC->szTitle,nItemCnt,pQC->wDNum);
				}
				else
					sprintf(szTmp,"(未完成)");

				m_pCompTrackQuest->pSTextCenter->ChgSText(i*1000+j,szTmp,255,255,255);
			}
			pCQuest->bChged	= false;
		}
	}
}
*/
