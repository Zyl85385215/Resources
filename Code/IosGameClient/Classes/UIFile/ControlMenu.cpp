#include "MapControl.h"
#include "ControlMenu.h"
#include "UserConfig.h"
#include "UserData.h"
#include "MapMouse.h"
#include "SlaveFormCtrl.h"
#include "MainMenu.h"
#include "SkillConfig.h"
#include "GuideCenter.h"
#include "SoundCenter.h"
#include "EnemyConfig.h"
#include "VipConfig.h"

extern void	SetCharToDCom(DComponent* pDom,BYTE byJob,BYTE bySex,char* szAction = "stand",int nDirection = 5);
extern void	SetCharToDCom(DComponent* pDom,int	nSkin,char* szAction = "stand",int nDirection = 5);
extern void	SetWeaponToDCom(DComponent* pDom,int nID,char* szAction = "stand",int nDirection = 5);

void LeaveMCopyReturnEvent(void* param,int nVal)
{
	if(nVal == 1)
	{
		pChar->SendCmd(USER, C_LEAVEMCOPY);
	}
}

void CmdLeaveMCopy(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->PopAskMsgBox("是否立即离开该副本?",LeaveMCopyReturnEvent,(void*)0);
	
}

void CmdAtkEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;


	if(g_pMapCenter->m_pMObjSel)
	{
		MapAIObj* pPAObj = (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;
		if (g_pMapCenter->m_pMObjSel->byType == MAPOBJ_PLAYER && g_pMapCenter->m_pMObjSel->byIsEnemy == 0)
		{
			BuildPacketEx(pPacket,USER,C_GET_EQUIPINFO,buf,256);
			SET_DATA(pData, USER,C_GET_EQUIPINFO,pPacket);
			pData->dwUin = pPAObj->m_nID;
			pChar->SendPacket(pPacket);
		}else
		    g_pMapCommand->CmdNormalAtk(pPAObj);
	}
	else
	{
		g_pMapCenter->SearchNearObj(2,300000);
		pChar->m_bManualSel	= true;
	}
}

void CtrlSkillShort(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	SkillObj* pSkill	= (SkillObj*)pChar->m_pSkillCenter->m_ppActSkill[(int)param];
	if(pSkill && pSkill->pSBase)
	{
		if(pSkill->pSBase->bAoeType == 0)
		{
			g_pMapCommand->CmdSkillAtk(NULL,pSkill->pSBase->wSkillID);
		}
		else if(g_pMapCenter->m_pMObjSel == NULL)
			pMainMenu->PopTipMsg("请选择目标");
		else if (strncmp(g_pMapCenter->m_pMObjSel->szName, "硫磺草", 6)==0||strncmp(g_pMapCenter->m_pMObjSel->szName, "火药桶", 6)==0)
			return;       //===zjk
		else if (g_pMapCenter->m_pMObjSel->byType != MAPOBJ_NPC)
		{
			//pMainMenu->PopTipMsg(pSBase->szName);
			g_pMapCommand->CmdSkillAtk((MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj,pSkill->pSBase->wSkillID);
		}
	}
	else
	{
		if((int)param == 4)
		{
			pMainMenu->PopTipMsg("未装备怒气技能");
		}
		else
		{
			pMainMenu->PopTipMsg("未设置快捷技能");
		}
	}

}

void ClickRideControlButtol(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		if(pChar->m_pHorseCenter->m_ifRide)
			pChar->m_pHorseCenter->RequestRide(false);
		else
			pChar->m_pHorseCenter->RequestRide(true);
	}
}
void ClickFastItem(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	ItemObj* pItem = pChar->m_pItemCenter->GetItemByTypeID(pChar->m_nFastItem);

	if (!pItem)
		return;

	switch(pItem->pItemBase->nUseType)
	{
	case 1:
		if (g_pMapCenter->m_pMObjLockView->nHP == g_pMapCenter->m_pMObjLockView->nMaxHP)
		{
			pMainMenu->PopTipMsg("       您的血量已满！");
			return;
		}
		break;
	case 2:
		if (pChar->m_nMp == pChar->m_nMaxMp)
		{
			pMainMenu->PopTipMsg("       您的蓝量已满！");
			return;
		}
		break;
	}	
	pChar->SendCmd(USER,C_USE_FAST_ITEM);
}
void ClickOpenVipButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->m_pVipMenu->Open();
	}
}

void ClickOpenRechargeButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void	fun_cbRevive(void* param)
{
	BuildPacketEx(pPacket,USER,C_REQUEST_REVIVE,buf,256);
	SET_DATA(pSend,USER,C_REQUEST_REVIVE,pPacket);
	pSend->bItem	= true;
	pChar->SendPacket(pPacket);
}

	void CReviveItemEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	ControlMenu* pMenu	= (ControlMenu*)param;
	if (!pMenu)
		return;

	if(pChar->m_pItemCenter->GetItemCountByID(30007)==0)
	{
		pMainMenu->PopTipMsg("缺少重生十字架");
		return;
	}
	ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(30007);
	if (pBase)
	{
		pMainMenu->StartCast(pBase->nCast,"等待复活......",fun_cbRevive);
	}
}

void CReviveJumpEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	BuildPacketEx(pPacket,USER,C_REQUEST_REVIVE,buf,256);
	SET_DATA(pSend,USER,C_REQUEST_REVIVE,pPacket);
	pSend->bItem	= false;
	pChar->SendPacket(pPacket);
}

void COpenEquipQHEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	BuildPacketEx(pPacket,USER,C_REQUEST_REVIVE,buf,256);
	SET_DATA(pSend,USER,C_REQUEST_REVIVE,pPacket);
	pSend->bItem	= false;
	pChar->SendPacket(pPacket);

	pMainMenu->m_pBagMainCtrl->Open(0,0);
}

void COpenHorsePYEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	BuildPacketEx(pPacket,USER,C_REQUEST_REVIVE,buf,256);
	SET_DATA(pSend,USER,C_REQUEST_REVIVE,pPacket);
	pSend->bItem	= false;
	pChar->SendPacket(pPacket);

	pMainMenu->m_pHorseMainCtrl->Open(2,2);
}

void COpenPetPYEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	BuildPacketEx(pPacket,USER,C_REQUEST_REVIVE,buf,256);
	SET_DATA(pSend,USER,C_REQUEST_REVIVE,pPacket);
	pSend->bItem	= false;
	pChar->SendPacket(pPacket);

	pMainMenu->m_pPetMainCtrl->Open(2,2);
}
void NewMailEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pMailMenu->m_nSelectMenu = MAILMENU_GETMAIL;
	pMainMenu->m_pBagMainCtrl->Open(0,2);
}

void OpenFortEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pChar->m_bHarvest = false;
	pMainMenu->m_pManorMenu->Open();
}

static void OpenSkillMenuEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pCtrlMenu->m_pCompLvUp->SetVisible(false);
	pMainMenu->m_pSkillCtrl->Open(0,0);
}


void StrikeSkillEvent(InterfaceEvent* pEvent,void* param)
{
	ControlMenu* pMenu	= (ControlMenu*)param;

	static int nDownX,nDownY;
	static int	nCutDir	= 0;
	switch(pEvent->event)
	{
	case INTERFACE_EVENT_DOWN:
		nDownX	= pEvent->x;
		nDownY	= pEvent->pComponent->nH - pEvent->y;
		nCutDir	= 0;
		pMenu->m_pCompStrikeBtn->SetAddSprByTag("",nDownX,nDownY,1,10,0);
		break;
	case INTERFACE_EVENT_MOVE:
		{
			int nEY	= pEvent->pComponent->nH - pEvent->y;
			//int nDir	= CalcDirection(pEvent->x,nDownY,nDownX,nEY,100);
			int	nDis	= CalcDistance(nDownX,nDownY,pEvent->x,nEY);

			if(nDis > 100)
			{
				if(nCutDir == 0)
					nCutDir	= CalcDirection(pEvent->x,nDownY,nDownX,nEY,100);
				int	nScale	= nDis/4 + 10;
				pMenu->m_pCompStrikeBtn->SetAddSprByTag("DAOGUANG.png",nDownX,nDownY,1,nScale,nCutDir+90);
			}
		}
		break;
	case INTERFACE_EVENT_UP:
		if(pMenu->CheckHitStrike(nDownX,nDownY,pEvent->x,pEvent->pComponent->nH - pEvent->y))
		{
			//pMenu->m_pCompStrikeBtn->SetEffect(88,1);
			pMenu->m_dwTickStrike	= xnGetTickCount();
			g_pSoundCenter->PlayerEffect("Sound/cut");
			pMenu->m_byStrikeStep	= 2;
		}
		else
		{
			pMenu->m_byStrikeStep	= 0;
			pMenu->m_pFormStrike->Close();
			pMenu->m_pCompStrikeBtn->SetAddSprByTag("",nDownX,nDownY,1,10,0);
		}
		
		//g_pMapCommand->CmdSkillAtk(g_pMapCommand->m_pObjNAtk,pMenu->m_wStrikeSkill);
		//pMenu->m_pFormStrike->Close();
		break;
	}
}

void OpenSelObjHandleMenu(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if (!g_pMapCenter->m_pMObjSel)
		return;
	if ((g_pMapCenter->m_pMObjSel->byType == MAPOBJ_PLAYER)&&pMainMenu->m_pSelObjHandleMenu->m_pForm->bOpened)
		pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
	else if (g_pMapCenter->m_pMObjSel->byType == MAPOBJ_PLAYER)
	    pMainMenu->m_pSelObjHandleMenu->Open();
}

void ClickShowMultipileButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	pChar->m_bClickExp = true;
	pChar->SendCmd(USER,C_GET_MULTISEC);
}

ControlMenu::ControlMenu()
{
	m_pFormTeam		= pMainInterface->GetDForm("team");
	m_pFormSkill	= pMainInterface->GetDForm("main4");
	m_pFormSelObj	= pMainInterface->GetDForm("mubiaotx");
	m_pFormStrike	= pMainInterface->GetDForm("qieshuiguo");

	m_pFormTeam->GetComponent("exp")->SetEvent(ClickShowMultipileButton,this);
	m_pFormTeam->GetComponent("time")->m_nFontSize = 14;
	m_pCompStrikeBtn	= m_pFormStrike->GetComponent("qy");
	m_pCompStrikeBtn->SetEvent(StrikeSkillEvent, this);
	m_pCompStrikeIco	= m_pFormStrike->GetComponent("tubiao");

	m_pCompHp		= m_pFormTeam->GetComponent("hp1");
	m_pCompMp		= m_pFormTeam->GetComponent("mp1");
	m_pCompName		= m_pFormTeam->GetComponent("name1");
	m_pCompIco		= m_pFormTeam->GetComponent("tx1");
	m_pCompLv		= m_pFormTeam->GetComponent("f1");

	m_pCompAtk		= m_pFormSkill->GetComponent("min");
	m_pCompAtk->SetEvent(CmdAtkEvent, this);
	m_pCompAtk->bSound	= false;
	m_pCompMCopy	= pMainInterface->GetDForm("map")->GetComponent("likai");
	m_pCompMCopy->SetEvent(CmdLeaveMCopy,this);
	m_pCompMail	= pMainInterface->GetDForm("map")->GetComponent("mail");
	m_pCompMail->SetEvent(NewMailEvent,this);
	m_pCompMail->SetVisible(false);

	m_pHarvest	= pMainInterface->GetDForm("map")->GetComponent("cs");
	m_pHarvest->SetEvent(OpenFortEvent,this);
	m_pHarvest->SetVisible(false);

	m_pCompLvUp	= pMainInterface->GetDForm("map")->GetComponent("up");
	m_pCompLvUp->SetEvent(OpenSkillMenuEvent,this);
	m_pCompLvUp->SetVisible(false);

	m_pCompSelName	= m_pFormSelObj->GetComponent("name1");
	m_pCompSelIco	= m_pFormSelObj->GetComponent("tx");
	m_pCompSelIco->SetEvent(OpenSelObjHandleMenu,this);
	m_pCompSelHp	= m_pFormSelObj->GetComponent("hp1");
	m_pCompSelLv	= m_pFormSelObj->GetComponent("f1");

	m_pPetIcon		= m_pFormTeam->GetComponent("ico");
	m_pPetIcon->m_nFontSize = 30;
	m_pPetIconback	= m_pFormTeam->GetComponent("cw");
	m_pPetLvl		= m_pFormTeam->GetComponent("f2");
	m_pPetLvlback	= m_pFormTeam->GetComponent("lv");
	m_pPetHp		= m_pFormTeam->GetComponent("cwhp");
	m_pPetHpBack	= m_pFormTeam->GetComponent("cwdi");
	m_pPetName		= m_pFormTeam->GetComponent("cwname");
	m_pPetName->m_nFontSize = 16;

	m_pCompExp		= new CCtrlProgress(m_pFormSkill->GetComponent("exp"));
	m_pCompExp->m_bHideTxt	= true;
	m_pCompName->SetSprColor(255,255,255);
	m_pCompLv->SetSprColor(255,255,255);

	char	szTmp[32];
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		sprintf(szTmp,"%d",i+1);
		m_pCompSkill[i]		= m_pFormSkill->GetComponent(szTmp);
		m_pCompSkill[i]->SetEvent(CtrlSkillShort,(void*)i);
		m_pCompSkill[i]->bSound	= false;
	}

	for (int i = 0; i < 7; i++)
	{
		sprintf(szTmp,"z%d",i+1);
		m_pCompState[i]		= m_pFormTeam->GetComponent(szTmp);
	}
	for (int i=0;i<7;i++)
	{
		sprintf(szTmp,"cd%d",i+1);
		m_pCompCD[i] = m_pFormSkill->GetComponent(szTmp);
	}
	m_pFormSkill->GetComponent("shang")->SetEvent(ClickRideControlButtol,this);
	m_pFormSkill->GetComponent("xia")->SetEvent(ClickRideControlButtol,this);

	m_pFormSkill->GetComponent("10")->SetEvent(ClickFastItem,this);
	m_pFormSkill->GetComponent("yao")->SetEvent(ClickFastItem,this);

	m_pFormTeam->GetComponent("vip")->SetEvent(ClickOpenVipButton,this);
	m_pFormTeam->GetComponent("cz")->SetEvent(ClickOpenRechargeButton,this);

	m_pCompFuryBar	= m_pFormSkill->GetComponent("dt");
	m_pCompFuryBar->byInputType	= 8;

	m_pCompSelName->m_nFontSize	= 18;
	m_pCompSelLv->m_nFontSize	= 18;
	m_pCompLv->m_nFontSize	= 18;
	m_pFormRevive		= pMainInterface->GetDForm("siwang");
	m_pFormRevive->bZorde	= FORM_ZORDER_REVIVE;
	m_pFormRevive->GetComponent("yd")->SetEvent(CReviveItemEvent, this);
	m_pFormRevive->GetComponent("hc")->SetEvent(CReviveJumpEvent, this);
	m_nCurValFury	= 0;
	m_pFormRevive->GetComponent("zb")->SetEvent(COpenEquipQHEvent, this);
	m_pFormRevive->GetComponent("zq")->SetEvent(COpenHorsePYEvent, this);
	m_pFormRevive->GetComponent("cw")->SetEvent(COpenPetPYEvent, this);
	m_dwTickStrike	= 0;
	m_dwTickMCopyLeave	= 0;
}

ControlMenu::~ControlMenu()
{	
}

void ControlMenu::Update()
{
	if(g_pMapCenter->m_pMObjLockView == NULL)
		return;

	_updateFuryBar();
	_updateStrike();
	_updateState();
	m_dwUpdateTick	= xnGetTickCount();

	char szIcon[16];
	sprintf(szIcon,"BT_VIP%d.png",pChar->m_byVipLvl);
	m_pFormTeam->GetComponent("vip")->SetViewSpr(szIcon);
	if(g_pMapCenter->m_pMObjLockView->IsDead())
	{

		memset(&pChar->m_szbyActLock,0,7);
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase(30007);
		int nCount = pChar->m_pItemCenter->GetItemCountByID(30007);
		if (pBase)
		{
			char szIcon[32];
			sprintf(szIcon,"item/%d.png",pBase->nSpr);
			m_pFormRevive->GetComponent("icon")->SetViewSpr(szIcon);
			m_pFormRevive->GetComponent("icon")->SetCaptionEx("%d",2,nCount);
		}

		if((g_pMapCenter->m_nBaseMapIndex>=50&&g_pMapCenter->m_nBaseMapIndex<=56) || g_pMapCenter->m_nBaseMapIndex==27)
		{
			if(m_dwTickMCopyLeave == 0)
			{
				m_dwTickMCopyLeave	= xnGetTickCount();
				if(g_pMapCenter->m_nBaseMapIndex != 27)
					pMainMenu->PlayEffect(222);
			}
			//else if(xnGetTickCount() - m_dwTickMCopyLeave > 3000)
			//{
			//	if(g_pMapCenter->m_nMapIndex >= 1000)
			//	{
			//		pChar->SendCmd(USER,C_LEAVEMCOPY);
			//		m_dwTickMCopyLeave	= 0;
			//	}
			//}
		}
		else if(m_pFormRevive->bOpened == false)
		{
			char sztxt[32];
			ItemExBase*	pBase	= ItemConfig::GetInstance().GetItemBase(30007);
			if(pBase)
			{
				sprintf(sztxt,"item/%d.png",pBase->nSpr);
				m_pFormRevive->GetComponent("icon")->SetViewSpr(sztxt);
				m_pFormRevive->GetComponent("num")->SetCaptionEx("%d",1,pChar->m_pItemCenter->GetItemCountByID(30007));
			}
			m_dwReviveAuto	= xnGetTickCount();
			m_pFormRevive->Open(2);
		}
		
		if (g_pMapCenter->m_nBaseMapIndex != 27 && (g_pMapCenter->m_nBaseMapIndex< 50 || g_pMapCenter->m_nBaseMapIndex > 56))
		{
			if(xnGetTickCount()-m_dwReviveAuto<60000)
			{
				char	szTmpex[32];
				int		nRemainS	= m_dwReviveAuto+60000-xnGetTickCount();
				sprintf(szTmpex,"%d",nRemainS/1000);
				m_pFormRevive->GetComponent("dj")->SetProgress(nRemainS,60000,szTmpex);
			}
			else
			{
				BuildPacketEx(pPacket,USER,C_REQUEST_REVIVE,buf,256);
				SET_DATA(pSend,USER,C_REQUEST_REVIVE,pPacket);
				pSend->bItem	= false;
				pChar->SendPacket(pPacket);
			}
		}
		
	}
	else if(m_pFormRevive->bOpened)
	{
		m_pFormRevive->Close();
		m_dwTickMCopyLeave	= 0;
	}
	else
		m_dwTickMCopyLeave	= 0;

	UpdateSkillCD();
	m_pCompHp->SetProgress(g_pMapCenter->m_pMObjLockView->nHP,g_pMapCenter->m_pMObjLockView->nMaxHP);
	m_pCompMp->SetProgress(pChar->m_nMp,pChar->m_nMaxMp/*pChar->m_xUserParam.nParam[EQUIP_ATTR_MP]*/);

	if(pChar->m_dwTickJoin && xnGetTickCount() - pChar->m_dwTickJoin > 8000 )
	{
		if (pChar->m_dwExp > 10000)
			m_pCompExp->SetProgress(pChar->m_dwExp/10000,UserConfig::GetInstance().GetExp(pChar->m_byLvl)/10000);
		else
			m_pCompExp->SetProgress(pChar->m_dwExp,UserConfig::GetInstance().GetExp(pChar->m_byLvl));
	}
	else
	{
		if (pChar->m_dwExp > 10000)
			m_pCompExp->SetProgress(pChar->m_dwExp/10000,UserConfig::GetInstance().GetExp(pChar->m_byLvl)/10000,0);
		else
			m_pCompExp->SetProgress(pChar->m_dwExp,UserConfig::GetInstance().GetExp(pChar->m_byLvl),0);
	}

	if(pChar->m_pHorseCenter->m_pEquipHorse == NULL)
	{
		m_pFormSkill->GetComponent("shang")->SetVisible(false);
		m_pFormSkill->GetComponent("xia")->SetVisible(false);
	}
	else
	{
		if(pChar->m_pHorseCenter->m_ifRide)
		{
			m_pFormSkill->GetComponent("shang")->SetVisible(false);
			m_pFormSkill->GetComponent("xia")->SetVisible(true);
		}
		else
		{
			m_pFormSkill->GetComponent("shang")->SetVisible(true);
			m_pFormSkill->GetComponent("xia")->SetVisible(false);
		}
	}
	m_pFormTeam->GetComponent("time")->SetCaptionEx("%d%%",1,pChar->m_nUserMultipile);

	m_pCompExp->Update();

	m_pCompLv->SetCaptionEx("%d",1,g_pMapCenter->m_pMObjLockView->byLv);
	if(g_pMapCenter->m_pMObjLockView->m_byType == MAPOBJ_PLAYER && g_pMapCenter->m_pMObjLockView->m_nID == pChar->m_dwUin && pChar->m_pSlaveCenter->m_pActiveSlave)
	{
		m_pPetIcon->SetVisible(true);
		m_pPetIconback->SetVisible(true);
		m_pPetLvl->SetVisible(true);
		m_pPetLvlback->SetVisible(true);
		m_pPetHp->SetVisible(true);
		m_pPetHpBack->SetVisible(true);
		m_pPetName->SetVisible(true);
		m_pPetLvl->SetCaptionEx("%d",1,pChar->m_pSlaveCenter->m_pActiveSlave->m_byLevel);
		if(pChar->m_pSlaveCenter->m_pActiveSlave->m_nHp == 0)
		{
			if(pChar->m_pSlaveCenter->m_dwReviveTick > 0 && pChar->m_pSlaveCenter->m_dwReviveTick > xnGetTickCount())
			{
				m_pPetIcon->SetViewSpr("");
				m_pPetIcon->SetCaptionEx("%d",1,(pChar->m_pSlaveCenter->m_dwReviveTick - xnGetTickCount())/1000);
				m_pPetIcon->SetSprColor(50,50,220);
			}
		}
		else
		{
			m_pPetIcon->SetCaption("",1);
			m_pPetIcon->SetSprColor(255,255,255);
			g_pSlaveMenuCtrl->ShowIcon(m_pPetIcon,pChar->m_pSlaveCenter->m_pActiveSlave->m_nType,0);
		}
		m_pPetName->SetCaptionEx("%s",1,pChar->m_pSlaveCenter->m_pActiveSlave->m_pName);
		MapAIObj* pSObj = g_pMapCenter->GetMapObj(MAPOBJ_SLAVE,pChar->m_pSlaveCenter->m_pActiveSlave->m_dwMapID);
		if(pSObj)
		{
			m_pPetHp->SetProgress(pSObj->nHP,pSObj->nMaxHP);
		}
// 		else
// 			m_pPetHp->SetProgress(0,100);
	}
	else
	{
		m_pPetIcon->SetVisible(false);
		m_pPetIconback->SetVisible(false);
		m_pPetLvl->SetVisible(false);
		m_pPetLvlback->SetVisible(false);
		m_pPetHp->SetVisible(false);
		m_pPetHpBack->SetVisible(false);
		m_pPetName->SetVisible(false);
	}
	if(pMainMenu->m_pMainFormTB->bOpened && g_pMapCenter->m_pMObjSel)
	{
		if(m_pFormSelObj->bOpened == false)
			m_pFormSelObj->Open(5);
		if(m_pFormSelObj->nPosY < 0)
			m_pFormSelObj->SetPos(m_pFormSelObj->nPosX,m_pFormSelObj->nPosY+300,200);
	}
	else
	{
		if(m_pFormSelObj->nPosY > 0)
			m_pFormSelObj->SetPos(m_pFormSelObj->nPosX,m_pFormSelObj->nPosY-300,200);
		pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
	}
	if(g_pMapCenter->m_pMObjSel)
	{
		_updateSelObj();
	}
	else
	{
		m_pCompAtk->SetViewSpr("BT_QH4.png");
	}
	UpdateFastItem();   //===zjk
}

void ControlMenu::ResetSkill()
{
	char	szTmp[32];
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		SkillObj* pSkill	= pChar->m_pSkillCenter->m_ppActSkill[i];
		if(pSkill && pSkill->pSBase)
		{
			if(i == 6 || i == 5)
			{
				m_pCompSkill[i]->SetVisible(true);
				m_pCompCD[i]->SetVisible(true);
			}
			sprintf(szTmp,"skill/%d.png",pSkill->pSBase->wIcon);
			m_pCompSkill[i]->SetViewSpr(szTmp);
		}
		else
		{
			if(i == 6 || i == 5)
			{
				m_pCompSkill[i]->SetVisible(false);
				m_pCompCD[i]->SetVisible(false);
			}
			m_pCompSkill[i]->SetViewSpr("MB_SKILL.png");
		}
	}

}

void ControlMenu::UpdateFastItem()
{
	if (pChar->m_pItemCenter->GetItemCountByID(pChar->m_nFastItem) == 0)
	{
		m_pFormSkill->GetComponent("10")->SetVisible(false);
		m_pFormSkill->GetComponent("yao")->SetVisible(false);
		m_pFormSkill->GetComponent("num")->SetCaptionEx(" ", 0);
	}else{
		m_pFormSkill->GetComponent("10")->SetVisible("MB_SKILL2.png");	
		m_pFormSkill->GetComponent("yao")->SetVisible(true);

		ItemExBase* pBase = (ItemExBase*)m_pFormSkill->GetComponent("yao")->GetTag();	
		char sztxt[32];
		sprintf(sztxt,"item/%d.png",pBase->nSpr);
		m_pFormSkill->GetComponent("yao")->SetViewSpr(sztxt);
		if (pBase&&pBase->wCDPos&&pBase->dwCDTime)
		{
			if (pChar->m_pClientFlag->mapCD[pBase->wCDPos])
			{
				int nTime = pChar->m_pClientFlag->mapCD[pBase->wCDPos] - xnGetTickCount();
				if (nTime<0)
				{
					nTime = 0;
				}
				m_pFormSkill->GetComponent("yao")->SetProgress(nTime,pBase->dwCDTime);
			}
		}else{
			m_pFormSkill->GetComponent("yao")->SetProgress(0,100);
		}
		m_pFormSkill->GetComponent("num")->SetCaptionEx("%d", 0, pChar->m_pItemCenter->GetItemCountByID(pChar->m_nFastItem));
		m_pFormSkill->GetComponent("num")->m_nFontSize = 20;
		m_pFormSkill->GetComponent("num")->SetSprColor(255, 255, 255);
	}
}

void ControlMenu::UpdateSkillCD()
{
	DWORD	dwTickCur	= xnGetTickCount();
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		SkillObj* pSkill	= pChar->m_pSkillCenter->m_ppActSkill[i];
		if(pSkill && pSkill->pLvBase)
		{
			if(pSkill->dwCDTick < dwTickCur)
			{
				if(pChar->m_dwLockSkill && dwTickCur-pChar->m_dwLockSkill < 1000)
				{
					if(pChar->m_wLockSkillID == pSkill->pSBase->wSkillID)
						m_pCompSkill[i]->SetProgress(100,100);
				}
				else
					m_pCompSkill[i]->SetProgress(0,100);
				if (i!=4)
				{
					m_pCompCD[i]->SetCaption("",1);
				}
			}
			else
			{
				char sztxt[32];
				sprintf(sztxt,"%d",(pSkill->dwCDTick-dwTickCur)/1000+1);
				m_pCompSkill[i]->SetProgress(pSkill->dwCDTick-dwTickCur,pSkill->pLvBase->dwCDTick);
				if (i!=4)
				{
					m_pCompCD[i]->SetCaption(sztxt,1);
				}
			}
		}
		else
		{
			m_pCompSkill[i]->SetProgress(0,100);
		}
	}
}

void ControlMenu::_updateFuryBar()
{
	if(pChar->m_pSkillCenter->m_ppActSkill[4] == NULL)
	{
		m_pCompSkill[4]->SetVisible(false);
		m_pCompFuryBar->SetVisible(false);
		m_pFormSkill->GetComponent("nq")->SetVisible(false);
		m_pFormSkill->GetComponent("d")->SetVisible(false);
	}
	else
	{
		m_pCompSkill[4]->SetVisible(true);
		m_pCompFuryBar->SetVisible(true);
		m_pFormSkill->GetComponent("nq")->SetVisible(true);
		m_pFormSkill->GetComponent("d")->SetVisible(true);
	}

	int nToVal	= pChar->m_nSPVal*100;
	if(nToVal >= m_nCurValFury)
		m_nCurValFury	+= xnGetTickCount() - m_dwUpdateTick;
	else
		m_nCurValFury	-= xnGetTickCount() - m_dwUpdateTick;
	if(m_nCurValFury > nToVal)
		m_nCurValFury	= nToVal;
	if(m_nCurValFury < 0)
		m_nCurValFury	= 0;

	m_pCompFuryBar->SetProgress(m_nCurValFury,5000," ");

	if(pChar->m_nSPVal >= 50)
	{
		m_pCompSkill[4]->SetEffect(79,1);
		if (pGuideCenter->bAtk)
		{
			pGuideCenter->nCurID = 2222;
		}
	}
	else
		m_pCompSkill[4]->SetEffect(0,1);
}

void ControlMenu::_updateSelObj()
{
	MapAIObj*	pAObj	= (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;
	if(g_pMapCenter->m_pMObjSel->byType == MAPOBJ_NPC)
		m_pCompAtk->SetViewSpr("BT_QH2.png");
	else if(g_pMapCenter->m_pMObjSel->byIsEnemy)
	{
		if(pAObj->m_byType == MAPOBJ_ENEMY && pAObj->pBaseCfg->bType == 3)
		{
			if(pAObj->pBaseCfg->nID == 1061 || pAObj->pBaseCfg->nID == 1119)
				m_pCompAtk->SetViewSpr("BT_QH6.png");
			else
				m_pCompAtk->SetViewSpr("BT_QH5.png");
		}
		else
			m_pCompAtk->SetViewSpr("BT_QH1.png");		
	}
	else
		m_pCompAtk->SetViewSpr("BT_QH3.png");

	m_pCompSelName->SetCaption(pAObj->m_szName,1);
	char szIco[32];
	if(pAObj->m_byType == MAPOBJ_PLAYER)
	{
		sprintf(szIco,"MB_S_MAN%d.png",((MapPlayer*)pAObj)->byJob*2+(((MapPlayer*)pAObj)->bySex?0:1)-1);
		m_pCompSelIco->SetViewSprByScale(szIco,0.65f,0.65f);
	}
	else
	{
		if(pAObj->bForce && g_pMapCenter->m_pMObjLockView->bForce != pAObj->bForce)
			strcpy(szIco,"MB_GTX.png");
		else
			strcpy(szIco,"MB_NTX.png");
		m_pCompSelIco->SetViewSpr(szIco);
	}

	if(pAObj->bForce && g_pMapCenter->m_pMObjLockView->bForce != pAObj->bForce)
	{
		m_pCompSelName->SetSprColor(200,0,0);
		m_pCompSelLv->SetSprColor(200,0,0);		
	}
	else
	{
		m_pCompSelName->SetSprColor(0,255,0);
		m_pCompSelLv->SetSprColor(0,255,0);
	}


	if(pAObj->m_byType == MAPOBJ_NPC)
	{
		m_pCompSelHp->SetProgress(pAObj->nHP,pAObj->nMaxHP," ");
		m_pCompSelLv->SetCaption("N",1);
	}
	else if (pAObj->m_byType == MAPOBJ_ENEMY &&
		(g_pMapCenter->m_nBaseMapIndex == 69 
		|| g_pMapCenter->m_nBaseMapIndex == 70 
		|| g_pMapCenter->m_nBaseMapIndex == 62 
		|| (g_pMapCenter->m_nBaseMapIndex > 50 && g_pMapCenter->m_nBaseMapIndex < 57)))
	{
		if(pAObj->nMaxHP < 100)
			pAObj->nMaxHP	 = 100;
		m_pCompSelHp->SetProgress(pAObj->nHP,pAObj->nMaxHP);
		m_pCompSelLv->SetCaption("M",1);
	}
	else
	{
		if(pAObj->nMaxHP < 100)
			pAObj->nMaxHP	 = 100;
		m_pCompSelHp->SetProgress(pAObj->nHP,pAObj->nMaxHP);
		m_pCompSelLv->SetCaptionEx("%d",1,pAObj->byLv);
	}
}

void ControlMenu::StrikeSkill( WORD wSkill )
{
	if(pMainMenu->m_pSettingMenu->m_bNoSkill)
		return;
	SkillBase* pBase	= SkillConfig::GetInstance().GetSkillBase(wSkill);
	if(!pBase)
		return;

	m_pFormStrike->Open();
	m_dwTickStrike	= xnGetTickCount();
	m_wStrikeSkill	= wSkill;

	char	szTmp[64];
	sprintf(szTmp,"skill/%d.png",pBase->wIcon);
	m_pCompStrikeIco->SetViewSpr(szTmp);
	m_pCompStrikeBtn->SetAddSprByTag("",0,0,1,10,0);
	m_byStrikeStep	= 1;

	pGuideCenter->SetStrikeGuide();
	m_pFormStrike->GetComponent("tubiao2")->SetViewSpr("");
}

void ControlMenu::_updateStrike()
{
	if(m_pFormStrike->bOpened == false)
		return;
	static float	fScale	= 0.5;
	switch(m_byStrikeStep)
	{
	case 3:

		if(xnGetTickCount() - m_dwTickStrike > 500)
		{
			m_byStrikeStep	= 0;
			m_pFormStrike->Close();
			pGuideCenter->bStrikeEff = false;
		}
		else if(xnGetTickCount() - m_dwTickStrike > 200)
		{
			
			m_pFormStrike->GetComponent("tubiao2")->SetViewSpr("");
			SkillBase*	pBase	= SkillConfig::GetInstance().GetSkillBase(m_wStrikeSkill);
			if(pBase)
			{
				char	szTmp[64];
				sprintf(szTmp,"fuckskill/%d.png",pBase->wIcon);
				float nScale	= fScale*10/15.0;
				m_pCompStrikeIco->SetViewSprByScale(szTmp,nScale,nScale);
			}
		}
		return;
	case 2:
		{
			m_pCompStrikeBtn->SetAddSprByTag("",0,0,1,10,0);
			m_byStrikeStep	= 3;
			float nScale	= fScale*10/15.0;
			m_pFormStrike->GetComponent("tubiao2")->SetViewSprByScale("SUILIE.png",nScale,nScale);
			SkillBase*	pSBase	= SkillConfig::GetInstance().GetSkillBase(m_wStrikeSkill);
			if (pSBase)
			{
				if(g_pMapCenter->m_pMObjSel && pSBase->bAoeType != 0)
					g_pMapCommand->CmdSkillAtk((MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj,m_wStrikeSkill);
				else
					g_pMapCommand->CmdSkillAtk(NULL,m_wStrikeSkill);
			}
		}

		return;
	default:
		{						
			if(xnGetTickCount() - m_dwTickStrike > ((pGuideCenter->bStrikeEff)?7000:2000))
			{					
				m_pFormStrike->Close();	
				pGuideCenter->bStrikeEff = false;
				return;
			}			
			fScale	= (xnGetTickCount() - m_dwTickStrike)/600.0;
			if(fScale > 1.5)
				fScale	= 1.5;
			m_pCompStrikeIco->SetScale(0.5+fScale,0.5+fScale);
		}
		break;
	}

	if(pMainMenu->m_pAutoMenu->m_bActive && m_byStrikeStep != 2 && pMainMenu->m_pAutoMenu->m_bAutoSkill && xnGetTickCount() - m_dwTickStrike > 1800)
	{
		g_pSoundCenter->PlayerEffect("Sound/cut");
		m_pCompStrikeBtn->SetAddSprByTag("DAOGUANG.png",m_pCompStrikeBtn->nW/2-140,m_pCompStrikeBtn->nH/2-140,1,100,315);
		m_dwTickStrike	= xnGetTickCount();
		m_byStrikeStep	= 2;
	}
}

bool ControlMenu::CheckHitStrike( int nX, int nY, int nTX, int nTY )
{
	int		nDis	= CalcDistance(nX,nY,nTX,nTY);
	if(nDis < 100)
		return false;

	int nMinX	= m_pCompStrikeBtn->nW/2 - 34;
	int	nMinY	= m_pCompStrikeBtn->nH/2 - 34;
	int nMaxX	= m_pCompStrikeBtn->nW/2 + 34;
	int	nMaxY	= m_pCompStrikeBtn->nH/2 + 34;
	if( (nX < nMinX && nTX < nMinX) || (nX > nMaxX && nTX > nMaxX) || 
		(nY < nMinY && nTY < nMinY) || (nY > nMaxY && nTY > nMaxY))
	{
		return false;
	}

	return true;
}

void ControlMenu::_updateState()
{
	StateCenter* pSC	= pChar->GetMyMapObj()->pStateCenter;

	char	szTmp[64];
	for (int i = 0; i < 7; i++)
	{
		if(i < pSC->m_pListState->Count)
		{
			StateBase* pState	= (StateBase*)pSC->m_pListState->Items[i];

			sprintf(szTmp,"state/%d.png",pState->wIcon);
			m_pCompState[i]->SetViewSpr(szTmp);
		}
		else
			m_pCompState[i]->SetViewSpr("");
	}
}

void SetBackLogin(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pChar->ReturnLogin();
}

void UserCenterEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	g_pTPObj->TP_OPenCenter();
}


void ChickHelpBtnEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pFaqHelpMenu->Open();
}

void ChickGLBtnEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	//g_pTPObj->TP_OtherEvent(3,"");	//攻略
}

SettingMenu::SettingMenu()
{
	_loadCfg();

	m_pForm		= pMainInterface->GetDForm("music");
	m_pSlideMp3			= new CCtrlSlide(m_pForm->GetComponent("kuang1"),m_pForm->GetComponent("k1"));
	m_pSlideSEff		= new CCtrlSlide(m_pForm->GetComponent("kuang2"),m_pForm->GetComponent("k2"));

	m_pCheckHideUser	= new CCtrlCheck(m_pForm->GetComponent("g1"),0);
	m_pCheckHideTitle	= new CCtrlCheck(m_pForm->GetComponent("g2"),0);
	m_pCheckNoTeam	= new CCtrlCheck(m_pForm->GetComponent("g3"),0);
	m_pCheckNoSkill	= new CCtrlCheck(m_pForm->GetComponent("g4"),0);

	m_pForm->GetComponent("fh")->SetEvent(SetBackLogin,this);
	m_pForm->GetComponent("yh")->SetEvent(UserCenterEvent,this);

	/*if(g_pTPObj->TP_GetPlat() != 13
							 && g_pTPObj->TP_GetPlat() != 12
							 && g_pTPObj->TP_GetPlat() != 14)*/
	switch(g_pTPObj->TP_GetCenterType())
	{
	case 0:
		m_pForm->GetComponent("yh")->SetVisible(false);
		break;
	case 1:
		m_pForm->GetComponent("yh")->SetVisible(true);
		break;
	case 2:
		m_pForm->GetComponent("yh")->SetViewSpr("BT_YONGHU1.png");
		break;
	case -1:
		{
			if(g_pTPObj->TP_GetPlat()==10 || g_pTPObj->TP_GetPlat()==34 || g_pTPObj->TP_GetPlat()==35 || g_pTPObj->TP_GetPlat()==33 || g_pTPObj->TP_GetPlat()==39|| g_pTPObj->TP_GetPlat()==51|| g_pTPObj->TP_GetPlat()==54)
				m_pForm->GetComponent("yh")->SetVisible(false);
			else if(g_pTPObj->TP_GetPlat()==36 || g_pTPObj->TP_GetPlat()==52 || g_pTPObj->TP_GetPlat()==53)
				m_pForm->GetComponent("yh")->SetViewSpr("BT_YONGHU1.png");
			else
				m_pForm->GetComponent("yh")->SetVisible(g_pTPObj->TP_GetPlat()/10%2);
		}
		break;
	}



	m_pSlideSEff->SetSlide(m_nValSEff);
	m_pSlideMp3->SetSlide(m_nValSMp3);
	m_pCheckHideUser->SetCheck(m_bHideUser);
	m_pCheckNoTeam->SetCheck(m_bNoTeam);
	m_pCheckNoSkill->SetCheck(m_bNoSkill);
	m_pCheckHideTitle->SetCheck(m_bHideTitle);

	m_pForm->GetComponent("bz")->SetEvent(ChickHelpBtnEvent);
	m_pForm->GetComponent("gl")->SetEvent(ChickGLBtnEvent);
}

SettingMenu::~SettingMenu()
{
	delete m_pSlideSEff;
	delete m_pSlideMp3;
	delete m_pCheckHideUser;
	delete m_pCheckHideTitle;
	delete m_pCheckNoTeam;
	delete m_pCheckNoSkill;
}

void SettingMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;

	bool	bChged	= false;
	if(m_nValSMp3	!= m_pSlideMp3->GetSlide())
	{
		m_nValSMp3	= m_pSlideMp3->GetSlide();
		g_pSoundCenter->SetMusicVal(m_nValSMp3);
		SaveIntegerToXML("set_music",m_nValSMp3);
		bChged	= true;
	}
	if(m_nValSEff	!= m_pSlideSEff->GetSlide())
	{
		m_nValSEff	= m_pSlideSEff->GetSlide();
		g_pSoundCenter->SetSoundVal(m_nValSEff);
		SaveIntegerToXML("set_sound",m_nValSEff);
		bChged	= true;
	}

	if(m_bHideUser	!= m_pCheckHideUser->GetCheck())
	{
		m_bHideUser	= m_pCheckHideUser->GetCheck();
		SaveBooleanToXML("set_hideuser",m_bHideUser);
		bChged	= true;
	}
	if(m_bHideTitle	!= m_pCheckHideTitle->GetCheck())
	{
		m_bHideTitle	= m_pCheckHideTitle->GetCheck();
		SaveBooleanToXML("set_hidetitle",m_bHideTitle);
		bChged	= true;
	}
	if(m_bNoTeam	!= m_pCheckNoTeam->GetCheck())
	{
		m_bNoTeam	= m_pCheckNoTeam->GetCheck();
		SaveBooleanToXML("set_hideteam",m_bNoTeam);
		bChged	= true;
	}
	if(m_bNoSkill	!= m_pCheckNoSkill->GetCheck())
	{
		m_bNoSkill	= m_pCheckNoSkill->GetCheck();
		SaveBooleanToXML("set_hideskill",m_bNoSkill);
		bChged	= true;
	}

	if(bChged)
		SaveToXMLOK();
}

void SettingMenu::_loadCfg()
{
	m_nValSMp3		= LoadIntegerFromXML("set_music",50);
	m_nValSEff		= LoadIntegerFromXML("set_sound",50);

	g_pSoundCenter->SetMusicVal(m_nValSMp3);
	g_pSoundCenter->SetSoundVal(m_nValSEff);

	m_bHideUser		= LoadBooleanFromXML("set_hideuser",false);
	m_bHideTitle	= LoadBooleanFromXML("set_hidetitle",false);
	m_bNoTeam		= LoadBooleanFromXML("set_hideteam",false);
	m_bNoSkill		= LoadBooleanFromXML("set_hideskill",false);
}

//void LoginMenu::SaveConfig()
//{
//	SaveStringToXML("default_account",g_szLoginAcc);
//	SaveStringToXML("default_password",g_szLoginPswd);
//	SaveToXMLOK();
//}

static void CBFrameEvent(int nPos,char* szName,void* param)
{
	SelObjHandleMenu*	pMenu	= (SelObjHandleMenu*)param;

	MapAIObj*	pAObj	= (MapAIObj*)g_pMapCenter->m_pMObjSel->pMObj;

	if (!pAObj)
		return;

	if (pAObj->IsDead())
	{
		pMainMenu->PopTipMsg("       该玩家已阵亡！");
		return;
	}
	if (strncmp (szName, "k1",2) == 0)
	{
		switch(nPos)
		{
		case 0:
			{
				pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
				pMainMenu->m_pChatMenu->Open();
				pMainMenu->m_pChatMenu->SetTargent(pAObj->m_szName);
			}
			return;
		case 1:
			{
				BuildPacketEx(pPacket,USER,C_GET_EQUIPINFO,buf,256);
				SET_DATA(pData, USER,C_GET_EQUIPINFO,pPacket);
				pData->dwUin = pAObj->m_nID;
				pChar->SendPacket(pPacket);

				pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
			}
			return;
		case 2:
			{
				pChar->InvitePlayer(pAObj->m_nID);
				pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
			}
			return;
		case 3:
			{
				BuildPacketEx(pPacket,FRIEND,C_P2PASK,buf,256);
				SET_DATA(pData, FRIEND,C_P2PASK,pPacket);

				memcpy(pData->szName,pAObj->m_szName,20);
				pData->bType	= PAT_FRIEND;
				pChar->SendPacket(pPacket);

				pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
			}
			return;
		case 4:
			{
				pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
				if (pChar->m_byLvl < 20)
				{
					pMainMenu->PopTipMsg("等级达到20级开启邮件功能");
					return;
				}
				pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->Open();
				pMainMenu->m_pMailMenu->ResetWriteMenu();
				pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("name")->SetEdit(pAObj->m_szName);
			}
			return;
		case 5:
			{
				pMainMenu->m_pSelObjHandleMenu->m_pForm->Close();
				BuildPacketEx(pPacket,USER,C_FIGHT_TARGET,buf,256);
				SET_DATA(pData, USER,C_FIGHT_TARGET,pPacket);
				pData->dwUin = pAObj->m_nID;
				pChar->SendPacket(pPacket);
			}
			return;
		}
	}
}

SelObjHandleMenu::SelObjHandleMenu()
{
	m_pForm = pMainInterface->GetDForm("mubiao");

	DComponent*	pCompBase	= m_pForm->GetComponent("d1");
	m_pForm->pCFrame	= new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,(pCompBase->nH+15)*6);
	DComponent*	ppCompPt[2];
	ppCompPt[0]		= m_pForm->GetComponent("k1");
	ppCompPt[1]		= m_pForm->GetComponent("x1");
	m_pForm->pCFrame->InitFParts(pCompBase,ppCompPt,2);
	m_pForm->pCFrame->nSpace	= 15;
	m_pForm->pCFrame->nDisStep	= pCompBase->nH+15;
	m_pForm->pCFrame->SetEventEx(CBFrameEvent,this);

	m_pForm->pCFrame->SetFPCnt(6,false);
}

SelObjHandleMenu::~SelObjHandleMenu()
{

}

static char* szButtonName[] = {"私聊","查看装备","组队邀请","添加好友","发送邮件","切磋"};

void  SelObjHandleMenu::Open()
{
	for (int i=0; i < 6; i++)
	{
		m_pForm->pCFrame->GetFPComp(i,"k1")->SetCaptionEx("%s",1,szButtonName[i]);
	}
	m_pForm->Open();
}

static void ClickMoreOperate(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	ClientUserInfo* pAObj = pMainMenu->m_pSelObjInfoMenu->m_pClientInfo;

	if (!pAObj)
		return;

	int nButton = (int)param;
	switch(nButton)
	{
	case 0:
		{
			pMainMenu->m_pChatMenu->Open();
			pMainMenu->m_pChatMenu->SetTargent(pAObj->szName);
		}
		return;
	case 1:
		{
			pChar->InvitePlayer(pAObj->dwUin);
		}
		return;
	case 2:
		{
			BuildPacketEx(pPacket,FRIEND,C_P2PASK,buf,256);
			SET_DATA(pData, FRIEND,C_P2PASK,pPacket);

			memcpy(pData->szName,pAObj->szName,20);
			pData->bType	= PAT_FRIEND;
			pChar->SendPacket(pPacket);
		}
		return;
	case 3:
		{
			if (pChar->m_byLvl < 20)
			{
				pMainMenu->PopTipMsg("等级达到20级开启邮件功能");
				return;
			}
			pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->Open();
			pMainMenu->m_pMailMenu->ResetWriteMenu();
			pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("name")->SetEdit(pAObj->szName);
		}
		return;
	case 4:
		{
			pMainMenu->PopTipMsg("添加屏蔽");
		}
		return;
	}
		
}

static void EquipInfoCallback(ItemObj* pObj)
{
	if (pObj)
	{
		if (pObj->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pObj,FORM_NULL);
			pMainMenu->m_pEquipInfoMenu->SetButtonSpr(NULL,NULL);
			pMainMenu->m_pEquipInfoMenu->SetCallBackEvent(NULL);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pObj,FORM_NULL);
			pMainMenu->m_pItemInfoMenu->SetButtonSpr(NULL,NULL);
			pMainMenu->m_pItemInfoMenu->SetCallBackEvent(NULL);
		}
	}
}

static void CheckEquipInfoEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	ClientUserInfo* pCUinfo = pMainMenu->m_pSelObjInfoMenu->m_pClientInfo;
    if(pCUinfo)
	{
		ItemObj* pObj = pCUinfo->szItem[(int)param];
		if(pObj)
			pMainMenu->m_pEquipInfoMenu->Open(pObj,FORM_NULL);
	}
	
}

static void ClickCloseEvent(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	SelObjInfoMenu*	pMenu	= (SelObjInfoMenu*)param;
	if (pMenu)
	{
		pMenu->m_pForm->Close();
		pMenu->m_pClientInfo = NULL;
	}
	

}

char* szSelObjEquipComp[] = {"zb4","zb2","zb3","zb1","zb10","zb7","zb6","zb9","zb8","zb5",};
SelObjInfoMenu::SelObjInfoMenu()
{
	m_pForm = pMainInterface->GetDForm("chakan");

	m_pForm->GetComponent("sl")->SetEvent(ClickMoreOperate,(void*)0);
	m_pForm->GetComponent("zd")->SetEvent(ClickMoreOperate,(void*)1);
	m_pForm->GetComponent("hy")->SetEvent(ClickMoreOperate,(void*)2);
	m_pForm->GetComponent("xx")->SetEvent(ClickMoreOperate,(void*)3);
	m_pForm->GetComponent("pb")->SetEvent(ClickMoreOperate,(void*)4);

	for (int i=0;i<10;i++)
	{
		m_pEquip[i] = m_pForm->GetComponent(szSelObjEquipComp[i]);
		m_pEquip[i]->SetEvent(CheckEquipInfoEvent,(void*)i);
	}
	m_pClientInfo = NULL;

	m_pForm->GetComponent("close")->SetEvent(ClickCloseEvent,this);
}

SelObjInfoMenu::~SelObjInfoMenu()
{
	if(m_pClientInfo)
		delete m_pClientInfo;
}
void SelObjInfoMenu::ClearWeapon()
{
	m_pForm->GetComponent("model3")->SetCartoonAction("","",0);
	m_pForm->GetComponent("model2")->SetCartoonAction("","",0);
}

extern void	SetItemToDCom(DComponent* pDom,ItemObj* pObj);
void SelObjInfoMenu::Open(ClientUserInfo* pInfo)
{
	ClearWeapon();

	m_pClientInfo = pInfo;
	for (int j=0;j<10;j++)
	{
		if (m_pClientInfo->szItem[j])
		{
			SetItemToDCom(m_pEquip[j],m_pClientInfo->szItem[j]);
		}else
		{
			m_pEquip[j]->ClearEffect();
			m_pEquip[j]->SetViewSpr("");
			m_pEquip[j]->SetAddSprByTag("",0,0,1);
		}
	}
	if(m_pClientInfo->nScore > 0)
		m_pForm->GetComponent("zd1")->SetCaptionEx("%d",1,m_pClientInfo->nScore);
	else
		m_pForm->GetComponent("zd1")->SetCaption("",0);
	int nWeaponSkin = m_pClientInfo->nSkin[1];

	bool bSwitch = true;
	if (m_pClientInfo->byJob ==2)
	{
		bSwitch = false;
	}
	if (m_pClientInfo->nSkin[0] > 1500)
	{
		m_pForm->GetComponent("model3")->SetVisible(false);
		m_pForm->GetComponent("model2")->SetVisible(false);
	}
	else
	{
		if(bSwitch)
		{
			m_pForm->GetComponent("model3")->SetVisible(true);
			m_pForm->GetComponent("model2")->SetVisible(false);
		}
		else
		{
			m_pForm->GetComponent("model3")->SetVisible(false);
			m_pForm->GetComponent("model2")->SetVisible(true);
		}
		if(nWeaponSkin > 0)
			SetWeaponToDCom(m_pForm->GetComponent((char*)(bSwitch?"model3":"model2")),nWeaponSkin);
	}
	
	SetCharToDCom(m_pForm->GetComponent((char*)"model"), m_pClientInfo->nSkin[0]);

	m_pForm->Open();
}

static void ClickFish(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	if (pMainMenu->m_pFishMenu->m_bFishBite)
	{
		BuildPacketEx(pPacket,USER,C_FISH_RLT,buf,256);
		SET_DATA(pData,USER,C_FISH_RLT,pPacket);

		pData->byFish = rand()%100>40?1:2;
		if (pData->byFish == 1)
			pMainMenu->PopTipMsg("好大一条鱼！");
		if (pData->byFish == 2)
			pMainMenu->PopTipMsg("好大一根鱼骨头～！");

		pChar->SendPacket(pPacket);
	}
	else
		pMainMenu->PopTipMsg("大鱼溜走啦～");

	pMainMenu->m_pFishMenu->Close();
}

FishMenu::FishMenu():m_dwWaiteTick(0),m_nRandSec(0),m_dwFishBiteTick(0),m_bFishBite(false)
{
	m_pForm = pMainInterface->GetDForm("diaoyu");

	m_pCompFish = m_pForm->GetComponent("diao");
	m_pCompFish->SetVisible(false);
	m_pCompFish->SetEvent(ClickFish,this);

	m_pCompCircle = m_pForm->GetComponent("di");
	m_pCompCircle->SetVisible(false);

	m_pCompTxt = m_pForm->GetComponent("desC0");
	m_pCompTxt->m_nFontSize = 30;
	m_pCompTxt->SetSprColor(255,204,51);
	DComponent* pBack = m_pForm->GetComponent("di0");
	pBack->SetViewSprByScale("GRAY.png",140.0,142.0);
}

FishMenu::~FishMenu()
{

}

void FishMenu::Open()
{
	m_pForm->Open();

	m_pCompFish->SetVisible(true);
	m_pCompFish->SetViewSpr("BT_DIAOYU.png");

	m_pCompCircle->SetVisible(true);
	m_pCompCircle->byRunAction = 60;
}
void FishMenu::Close()
{
	m_pCompFish->SetVisible(false);
	m_pCompCircle->SetVisible(false);

	m_bFishBite = false;
	m_nRandSec = 0;
	m_dwWaiteTick = 0;
	m_dwFishBiteTick = 0;

	m_pForm->Close();
}
void FishMenu::Update()
{
	if (!m_pForm->bOpened)
		return;

	if(g_pMapCenter->m_pMObjLockView->IsDead())
	{
		BuildPacketEx(pPacket,USER,C_FISH_RLT,buf,256);
		SET_DATA(pData,USER,C_FISH_RLT,pPacket);
		pData->byFish = 5;
		pData->dwUin = 0;
		pChar->SendPacket(pPacket);

		pMainMenu->m_pFishMenu->Close();
	}

	if (!m_dwWaiteTick)
	{
		m_nRandSec   = rand()%3+4;
		m_dwWaiteTick = xnGetTickCount() + m_nRandSec*1000;
	}

	if (!m_bFishBite&&xnGetTickCount() > m_dwWaiteTick)
	{
		m_nRandSec   = rand()%3+1;
		m_pCompCircle->byRunAction = 60 + m_nRandSec;
		m_dwFishBiteTick = xnGetTickCount() + m_nRandSec*1000;
		m_bFishBite = true;

		m_pCompFish->SetViewSpr("BT_DIAOYU2.png");
	}
	if (m_bFishBite&&xnGetTickCount() > m_dwFishBiteTick)
	{
		m_nRandSec   = rand()%3+4;
		m_dwWaiteTick = xnGetTickCount() + m_nRandSec*1000;
		m_pCompCircle->byRunAction = 60;
		m_bFishBite  = false;

		m_pCompFish->SetViewSpr("BT_DIAOYU.png");
	}

	if (!m_bFishBite)
	{
		int nDotNum	= xnGetTickCount()/500%3;
		if(nDotNum == 1)
			m_pCompTxt->SetCaption("钓鱼中，请耐心等待鱼儿上钩 ．．",0);
		else if(nDotNum == 2)
			m_pCompTxt->SetCaption("钓鱼中，请耐心等待鱼儿上钩 ．．．",0);
		else
			m_pCompTxt->SetCaption("钓鱼中，请耐心等待鱼儿上钩 ．",0);
	}else
		m_pCompTxt->SetCaption("",0);
}