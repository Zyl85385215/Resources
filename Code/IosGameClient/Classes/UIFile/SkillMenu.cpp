#include "MainMenu.h"
#include "SkillMenu.h"
#include "SkillConfig.h"
#include "SkillData.h"
#include "ios_user_packet.h"

void SkillInfoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	SkillBase*	pSBase	= (SkillBase*)param;
	if(pSBase)
	{
		SkillObj* pSkill	= pChar->m_pSkillCenter->GetSkill(pSBase->wSkillID);
		BYTE	bCurLv	= 0;
		if(pSkill)
			bCurLv	= pSkill->bySLv;
		
		pMainMenu->m_pSkillMenu->ViewSkillInfo(pSBase,bCurLv);
	}
}

void SOpenSCutEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	SkillBase*	pSBase	= (SkillBase*)param;
	pMainMenu->m_pSkillMenu->OpenShortCut();
}

void SetSCutEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pSkillMenu->SetShortCut((int)param);
}

void SLvUpEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pSkillMenu->RequestLvUp();
}

void CZSkillPnt(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pMainMenu->m_pSkillMenu->RequestCZSkillPnt();
}

void ClickCancelShortCut(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		SkillMenu* pMenu = (SkillMenu*)param;
		pMenu->CloseShortCut();
	}
}
void CancelSettingSkill(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		SkillMenu* pMenu = (SkillMenu*)param;
		pMenu->SetShortCut(-1);
	}
}
SkillMenu::SkillMenu()
{
	char	szTmp[256];
	for (int i = 0; i < 4; i++)
	{
		sprintf(szTmp,"zhiye%d",i+1);
		m_pFormJob[i]		= pMainInterface->GetDForm(szTmp);


		m_pFormJob[i]->GetComponent("xue")->m_nFontSize	= 24;
		m_pFormJob[i]->GetComponent("yu")->m_nFontSize	= 24;

		m_pFormJob[i]->GetComponent("cz")->SetEvent(CZSkillPnt);
	}
	m_byCurJob	= 0;
	m_pFormSInfo		= pMainInterface->GetDForm("skill2");
	m_pFormSInfo->GetComponent("kj")->SetEvent(SOpenSCutEvent,this);

	m_pFormSInfo->GetComponent("xx")->SetEvent(SLvUpEvent,this);
	m_pFormSInfo->GetComponent("sj")->SetEvent(SLvUpEvent,this);
	m_pFormSInfo->GetComponent("qc")->SetEvent(CancelSettingSkill,this);

	m_pFormSCut			= pMainInterface->GetDForm("skill3");
	
	for (int i = 0; i < MAX_ACTIVE_SKILL; i++)
	{
		sprintf(szTmp,"%d",i+1);
		DComponent* pCompS = m_pFormSCut->GetComponent(szTmp);
		if(pCompS)
			pCompS->SetEvent(SetSCutEvent, (void*)i);
	}
	m_pFormSCut->GetComponent("min")->SetEvent(ClickCancelShortCut,this);
	m_pCurSelSkill	= NULL;
	m_dwLockSkillUp	= 0;

	m_pForm		= m_pFormJob[0];
}

SkillMenu::~SkillMenu()
{

}


void SkillMenu::Open( int nT )
{
	m_pForm		= m_pFormJob[pChar->m_byJob-1];
	OpenForm(pChar->m_byJob-1,nT);
}

void SkillMenu::OpenForm( BYTE bJob,int nT )
{
	if(bJob >= 4)
		return;
	m_byCurJob	= bJob;
	char	szTmp[256];
	m_pFormJob[m_byCurJob]->Open(nT);

	for (int i = 0; i < 12; i++)
	{
		sprintf(szTmp,"j%d",i+1);
		m_ppSkillIco[i]		= m_pFormJob[m_byCurJob]->GetComponent(szTmp);

		sprintf(szTmp,"n%d",i+1);
		m_ppSkillName[i]	= m_pFormJob[m_byCurJob]->GetComponent(szTmp);
		SkillBase*	pSBase	= SkillConfig::GetInstance().GetJobSkill(bJob+1,i);
		if(pSBase)
		{
			m_ppSkillName[i]->SetCaptionEx("0/%d",1,pSBase->pListLvInfo->Count);
			sprintf(szTmp,"skill/%d.png",pSBase->wIcon);
			m_ppSkillIco[i]->SetViewSpr(szTmp);
			m_ppSkillIco[i]->SetEvent(SkillInfoEvent,pSBase);
		}
	}
	for (int i = 0; i < 8; i++)
	{
		sprintf(szTmp,"jd%d",i+1);
		m_ppArrow[i]		= m_pFormJob[bJob]->GetComponent(szTmp);
	}
}

void SkillMenu::Update()
{
	if(m_pFormJob[m_byCurJob]->bOpened == false)
		return;
	int	nChgScale	= xnGetTickCount()%2000;
	BYTE	bScale	= 0;
	if(nChgScale > 1000)
		bScale	= 90+(nChgScale-1000)/50;
	else
		bScale	= 110-nChgScale/50;
	for (int i = 0; i < 8; i++)
	{
		if(m_ppArrow[i])
			m_ppArrow[i]->byChgScaleY	= bScale;
	}
	m_pFormJob[m_byCurJob]->GetComponent("xue")->SetCaptionEx("%d",1,pChar->m_byLvl*3-pChar->m_wSkillPt-3);
	m_pFormJob[m_byCurJob]->GetComponent("yu")->SetCaptionEx("%d",1,pChar->m_wSkillPt);
	for (int i = 0; i < 12; i++)
	{
		SkillBase*	pSBase	= SkillConfig::GetInstance().GetJobSkill(m_byCurJob+1,i);
		if(pSBase)
		{
			BYTE	bCLv	= 0;
			bool	bCanLearn	= true;
			SkillObj*	pSkill	= pChar->m_pSkillCenter->GetSkill(pSBase->wSkillID);
			if(pSkill)
			{
				bCLv	= pSkill->bySLv;
			}
			m_ppSkillName[i]->SetCaptionEx("%d/%d",1,bCLv,pSBase->pListLvInfo->Count);

			
			if(pSBase->wPreSkillID)
			{
				SkillObj* pSkillNeed	= pChar->m_pSkillCenter->GetSkill(pSBase->wPreSkillID);
				if(!pSkillNeed || pSkillNeed->bySLv < pSBase->bPreSkillLv)
				{
					bCanLearn	= false;
				}
			}
		
			if(bCanLearn)
			{
				SEffLvInfo*	pSLv	= pSBase->GetSELv(1);
				if(pSLv)
				{
					if(pChar->m_byLvl < pSLv->byUpLvNeed)
					{
						bCanLearn = false;
					}
				}
			}
			if(bCanLearn)
				m_ppSkillIco[i]->SetSprColor(255,255,255);
			else
				m_ppSkillIco[i]->SetSprColor(150,150,150);
		}
	}

}

static char*	g_szParamTxt[]	= {"人物生命值", "最大生命值","人物魔法值", "最大魔法值", "人物物攻", "人物魔攻", "人物物防", "人物魔防", "命中","闪避","暴击", "移动速度","属性","属性","属性"};

bool skillReplaceTxtName( char* szTxt, char* szOut,SkillBase* pBase, SEffLvInfo* pLv )
{
	if(strlen (szTxt) > 500)
	{
		return false;
	}

	char* szFind	= strstr(szTxt,"$");
	if(szFind )
	{
		int nKey	= atoi (szFind+1);
		char	szReplace[128];

		switch(nKey)
		{
		case 1:
			{
				sprintf(szReplace," [");

				if(pLv->nP_X)
					sprintf(szReplace+strlen(szReplace),"%d",pLv->nP_X);

				if(pLv->nP_Y1)
				{
					if(pLv->nP_X)
						sprintf(szReplace+strlen(szReplace),"+");
					sprintf(szReplace+strlen(szReplace),"%d%%%s",pLv->nP_Y1,g_szParamTxt[pLv->nP_Y1Type]);
				}
				if(pLv->nP_Y2)
					sprintf(szReplace+strlen(szReplace),"+%d%%%s",pLv->nP_Y2,g_szParamTxt[pLv->nP_Y2Type]);
				sprintf(szReplace+strlen(szReplace),"] ");
			}
			break;
		case 2:
			{
				sprintf(szReplace,"%d",pLv->wRangeAdd+pBase->wAoeRange);
			}
			break;
		case 3:
			{
				sprintf(szReplace,"%d",pLv->bTargetAdd+pBase->bMaxTarget);
			}
			break;
		default:
			strcpy(szReplace," ");
			break;
		}
		strncpy (szOut,szTxt,(szFind-szTxt));

		sprintf(szOut+strlen (szOut),"%s",szReplace);


		char* szEnd	= strstr(szFind+1,"$");
		if(szEnd == 0)
			return false;

		char	szSecond[1024];
		memset(szSecond,0,1024);
		if(skillReplaceTxtName (szEnd+1,szSecond,pBase,pLv))
		{
			sprintf(szOut+strlen(szOut),"%s",szSecond);
		}
		else
			sprintf(szOut+strlen(szOut),"%s",szEnd+1);
		return true;
	}
	return false;
}

void SkillMenu::ViewPetSkillInfo(SkillBase* pBase,BYTE	bLv)
{
	m_pFormSInfo->Open(2);

	m_pFormSInfo->GetComponent("xx")->SetVisible(false);
	m_pFormSInfo->GetComponent("kj")->SetVisible(false);
	m_pFormSInfo->GetComponent("sj")->SetVisible(false);

	bool bVisInfo	= true;
	if(pBase->bType == 0)
		bVisInfo = false;

	m_pFormSInfo->GetComponent("djz")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("jlz")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("timez")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("xhz")->SetVisible(false);
	m_pFormSInfo->GetComponent("dj")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("jl")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("time")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("xh")->SetVisible(false);

	m_pFormSInfo->GetComponent("beidong")->SetVisible(!bVisInfo);

	char	szTmp[64];
	sprintf(szTmp,"skill/%d.png",pBase->wIcon);
	m_pFormSInfo->GetComponent("icon")->SetViewSpr(szTmp);
	m_pFormSInfo->GetComponent("name")->SetCaption(pBase->szName,1);
	m_pFormSInfo->GetComponent("name")->SetSprColor(0,255,0);
	m_pFormSInfo->GetComponent("dj")->SetCaptionEx("%d",1,bLv);


	SEffLvInfo* pLv	= pBase->GetSELv(bLv);
	SEffLvInfo* pLvNext	= pBase->GetSELv(bLv+1,false);

	if(pLv)
	{
		if(pLv->dwCDTick == 0)
			m_pFormSInfo->GetComponent("time")->SetCaption("无",1);
		else
			m_pFormSInfo->GetComponent("time")->SetCaptionEx("%d.%ds",1,pLv->dwCDTick/1000,pLv->dwCDTick%1000/100);
	}
	else if(pLvNext)
	{
		if(pLvNext->dwCDTick == 0)
			m_pFormSInfo->GetComponent("time")->SetCaption("无",1);
		else
			m_pFormSInfo->GetComponent("time")->SetCaptionEx("%d.%ds",1,pLvNext->dwCDTick/1000,pLvNext->dwCDTick%1000/100);
	}
	if(pBase->wAtkRange == 9999)
		m_pFormSInfo->GetComponent("jl")->SetCaption("自身",1);
	else
		m_pFormSInfo->GetComponent("jl")->SetCaptionEx("%d",1,pBase->wAtkRange);

	DComponent* pCompDesc	= m_pFormSInfo->GetComponent("ms");
	char	szDesc[1024]	= {0};
	int		nIndex = 0;
	pCompDesc->pSTextCenter->ClearSText();

	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"当前等级:",0,255,0);
	if(pLv)
	{
		pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,pLv->szDesc,255,255,255);
	}
	else
		pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"未学习",255,255,255);

	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true," ",255,255,255);

	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"下一等级:",255,0,0);

	STextObj* pSTObj	= pCompDesc->pSTextCenter->AddSText(nIndex++,0,false,szDesc,255,222,100);
	if(pSTObj)
	{
		pSTObj->nFontSize	= 16;
		pSTObj->nOffY		= 4;
	}
	memset(szDesc,0,1024);
	if(pLvNext)
	{
		pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,pLvNext->szDesc,150,150,150);
		
	}
	else
	{
		pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"已满级",0,255,0);
	}

	m_pFormSInfo->GetComponent("qc")->SetVisible(false);
}

void SkillMenu::ViewSkillInfo( SkillBase* pBase,BYTE	bLv)
{
	m_pCurSelSkill	= pBase;
	m_pFormSInfo->Open(2);

	m_pFormSInfo->GetComponent("xx")->SetVisible(!bLv);
	m_pFormSInfo->GetComponent("sj")->SetVisible(true);

	bool bVisInfo	= true;
	if(pBase->bType == 0)
		bVisInfo = false;
	
	m_pFormSInfo->GetComponent("djz")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("jlz")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("timez")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("xhz")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("dj")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("jl")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("time")->SetVisible(bVisInfo);
	m_pFormSInfo->GetComponent("xh")->SetVisible(bVisInfo);
	
	m_pFormSInfo->GetComponent("beidong")->SetVisible(!bVisInfo);

	char	szTmp[64];
	sprintf(szTmp,"skill/%d.png",pBase->wIcon);
	m_pFormSInfo->GetComponent("icon")->SetViewSpr(szTmp);
	m_pFormSInfo->GetComponent("name")->SetCaption(pBase->szName,1);

	m_pFormSInfo->GetComponent("name")->SetSprColor(0,255,0);

	m_pFormSInfo->GetComponent("dj")->SetCaptionEx("%d",1,bLv);
	
	//======zjk

	if (bLv<=0&&bVisInfo)
	{
		m_pFormSInfo->GetComponent("kj")->SetVisible(pBase->bType==0);

	}else if (pBase->bType == 3)
	{
		m_pFormSInfo->GetComponent("kj")->SetVisible(false);

	}else
		m_pFormSInfo->GetComponent("kj")->SetVisible(bVisInfo);


	SEffLvInfo* pLv	= pBase->GetSELv(bLv);
	SEffLvInfo* pLvNext	= pBase->GetSELv(bLv+1,false);

	m_pFormSInfo->GetComponent("xh")->SetCaptionEx("%d",1,pLv?pLv->nMpCost:pBase->nMpCost);
	if(pLv)
	{
		if(pLv->dwCDTick == 0)
			m_pFormSInfo->GetComponent("time")->SetCaption("无",1);
		else
			m_pFormSInfo->GetComponent("time")->SetCaptionEx("%d.%ds",1,pLv->dwCDTick/1000,pLv->dwCDTick%1000/100);
	}
	else if(pLvNext)
	{
		if(pLvNext->dwCDTick == 0)
			m_pFormSInfo->GetComponent("time")->SetCaption("无",1);
		else
			m_pFormSInfo->GetComponent("time")->SetCaptionEx("%d.%ds",1,pLvNext->dwCDTick/1000,pLvNext->dwCDTick%1000/100);
	}
	if(pBase->wAtkRange == 9999)
		m_pFormSInfo->GetComponent("jl")->SetCaption("自身",1);
	else
		m_pFormSInfo->GetComponent("jl")->SetCaptionEx("%d",1,pBase->wAtkRange);

	DComponent* pCompDesc	= m_pFormSInfo->GetComponent("ms");
	char	szDesc[1024]	= {0};
	int		nIndex = 0;
	pCompDesc->pSTextCenter->ClearSText();

	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"当前等级:",0,255,0);
	if(pLv)
	{
		//if(skillReplaceTxtName(pBase->szDesc,szDesc,pBase,pLv))
		//{
			pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,pLv->szDesc,255,255,255);
		//}
		//else
		//{
		//	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,pBase->szDesc,255,255,255);
		//}
	}
	else{
		if(m_pCurSelSkill->wPreSkillID)
		{		
			SkillBase* pSBasePre	= SkillConfig::GetInstance().GetSkillBase(m_pCurSelSkill->wPreSkillID);
			if(pSBasePre)
			{
				SEffLvInfo* pLv2	= pBase->GetSELv(1);
				char szDescTwo[64];
				if (pLv2->byUpLvNeed == 0)
				{
					sprintf(szDescTwo, "需要 前置技能 [%s] 达到lv.%d", pSBasePre->szName, m_pCurSelSkill->bPreSkillLv);
				}else
					sprintf(szDescTwo, "需要人物等级达到%d级才能学习;且前置技能 [%s] 达到lv.%d", pLv2->byUpLvNeed, pSBasePre->szName, m_pCurSelSkill->bPreSkillLv);
				pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,szDescTwo,255,255,255);
			}
		}else{
			SEffLvInfo* pLv3	= m_pCurSelSkill->GetSELv(1);
			char szDescOne[64];
			sprintf(szDescOne, "需要人物等级达到%d级才能学习。", pLv3->byUpLvNeed);
			pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,szDescOne,255,255,255);
		}
	}
		//pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"未学习",255,255,255);



	//if(pBase->bAoeType != 0)
	//	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"    需要目标",255,0,0);
	//else
	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true," ",255,255,255);

	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"下一等级:",255,0,0);

	sprintf(szDesc,"  (消耗 %d 技能点)",pBase->byCostSPoint);
	STextObj* pSTObj	= pCompDesc->pSTextCenter->AddSText(nIndex++,0,false,szDesc,255,222,100);
	if(pSTObj)
	{
		pSTObj->nFontSize	= 16;
		pSTObj->nOffY		= 4;
	}
	memset(szDesc,0,1024);
	if(pLvNext)
	{
		//if(skillReplaceTxtName(pBase->szDesc,szDesc,pBase,pLvNext))
		//{
			pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,pLvNext->szDesc,150,150,150);
		//}
		//else
		//{
		//	pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,pBase->szDesc,150,150,150);
		//}
	}
	else
	{
		pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"已满级",0,255,0);
	}

	m_pFormSInfo->GetComponent("qc")->SetVisible(pChar->m_pSkillCenter->CheckSkillSetting(pBase->wSkillID));
}


void SkillMenu::OpenShortCut()
{
	m_pFormSCut->Open(1);

// 	m_pFormSCut->GetComponent("1")->SetEffect(38,1);
// 	m_pFormSCut->GetComponent("2")->SetEffect(38,1);
// 	m_pFormSCut->GetComponent("3")->SetEffect(38,1);
// 	m_pFormSCut->GetComponent("4")->SetEffect(38,1);
	char szName[8];
	char szTmp[32];
	for(int i = 0; i < MAX_ACTIVE_SKILL; i ++)
	{
		sprintf(szName,"%d",i+1);
		DComponent* pComp = m_pFormSCut->GetComponent(szName);
		if(pComp)
		{
			pComp->SetEffect(38,1);
			SkillObj* pSkill	= pChar->m_pSkillCenter->m_ppActSkill[i];
			if(pSkill && pSkill->pSBase)
			{
				sprintf(szTmp,"skill/%d.png",pSkill->pSBase->wIcon);
				pComp->SetViewSpr(szTmp);
			}
			else
				pComp->SetViewSpr("MB_SKILL.png");
		}
	}
}

void SkillMenu::CloseShortCut()
{
	m_pFormSCut->Close();
}

void SkillMenu::SetShortCut( int nPos )
{
	if(m_pCurSelSkill)
	{//设置发送服务器设置技能
		BuildPacketEx(pPacket, USER, C_SET_SSHORTCUT,buf,256);
		SET_DATA(pSend, USER, C_SET_SSHORTCUT, pPacket);

		pSend->wSkill	= m_pCurSelSkill->wSkillID;
		pSend->byPos	= nPos+1;
		pChar->SendPacket(pPacket);
	}
	m_pFormSCut->Close();
	if (m_pFormSInfo->bOpened)
		m_pFormSInfo->Close();
}

void SkillMenu::RequestLvUp()
{
	if(m_dwLockSkillUp && xnGetTickCount() - m_dwLockSkillUp < 800)
		return;
	if(m_pCurSelSkill == NULL)
		return;

	SkillObj*	pSkill	= pChar->m_pSkillCenter->GetSkill(m_pCurSelSkill->wSkillID);
	BYTE	bRqLv	= 1;
	if(pSkill)
		bRqLv	= pSkill->bySLv	+ 1;
	SEffLvInfo*	pSLv	= m_pCurSelSkill->GetSELv(bRqLv,false);
	if(pSLv == NULL)
	{
		pMainMenu->PopTipMsg("该技能已满级");
		return;
	}
	if(pSLv->byUpLvNeed > pChar->m_byLvl)
	{
		pMainMenu->PopTipMsg("需要人物等级lv.%d",pSLv->byUpLvNeed);
		return;
	}
	if(m_pCurSelSkill->byCostSPoint > pChar->m_wSkillPt)
	{
		pMainMenu->PopTipMsg("技能点不足，人物等级每提升1级可获得3点技能点！");
		return;
	}
	if(m_pCurSelSkill->nGoldLvNeed > pChar->m_dwMoney)
	{
		pMainMenu->PopTipMsg("金币不足");
		return;
	}
	if(m_pCurSelSkill->wPreSkillID)
	{
		SkillObj*	pSkillPre	= pChar->m_pSkillCenter->GetSkill(m_pCurSelSkill->wPreSkillID);
		if(pSkillPre == NULL || pSkillPre->bySLv < m_pCurSelSkill->bPreSkillLv)
		{
			SkillBase* pSBasePre	= SkillConfig::GetInstance().GetSkillBase(m_pCurSelSkill->wPreSkillID);
			if(pSBasePre)
				pMainMenu->PopTipMsg("需要前置技能 [%s] 达到lv.%d",pSBasePre->szName,m_pCurSelSkill->bPreSkillLv);
			return;
		}
	}

	//学习技能
	BuildPacketEx(pPacket, USER, C_SKILLUP,buf,256);
	SET_DATA(pSend, USER, C_SKILLUP, pPacket);
	
	pSend->wSkill	= m_pCurSelSkill->wSkillID;
	pChar->SendPacket(pPacket);

	m_dwLockSkillUp	= xnGetTickCount();
	//m_pFormSInfo->Close();
}
void fun_cbResetSkill(void * pParam,int nVal)
{
	if(nVal == 1)
		pChar->SendCmd(USER,C_SKILLCZ);	
}

void RechargeReturn4Event(void* param,int nVal)
{
	if(nVal == 1)
	{
		pMainMenu->m_pRechargeMenu->OpenForm(2);
	}
}

void SkillMenu::RequestCZSkillPnt()
{
	//重置技能
	if(pChar->m_wSkillPt == pChar->m_byLvl*3-3)
	{
		pMainMenu->PopTipMsg("未分配技能点");
		return;
	}
	if(pChar->m_byLvl >= 35)
	{
		if(pChar->m_dwRmb < 30)
		{
			pMainMenu->PopAskMsgBox("钻石不足，是否前往充值?",RechargeReturn4Event,(void*)0,3);
			return;
		}
		else
		{
			pMainMenu->PopAskMsgBox("消耗30钻石, 确定重置?",fun_cbResetSkill,this);
		}
	}
	else
		pMainMenu->PopAskMsgBox("35级以下免费, 确定重置?",fun_cbResetSkill,this);
		
}

extern void TMCSelPage(InterfaceEvent * pEvent,void * param);
extern void TMCCloseEvent(InterfaceEvent * pEvent,void * param);
void SkillMenu::InitTabEvent(int nCnt)
{
	char	szTmp[256];
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < nCnt; j++)
		{
			sprintf(szTmp,"page%d",j+1);
			m_pFormJob[i]->GetComponent(szTmp)->eventFunc	= m_pForm->GetComponent (szTmp)->eventFunc;
			m_pFormJob[i]->GetComponent(szTmp)->pCbParam	= m_pForm->GetComponent (szTmp)->pCbParam;
		}
		m_pFormJob[i]->GetComponent("close")->eventFunc	= m_pForm->GetComponent ("close")->eventFunc;
		m_pFormJob[i]->GetComponent("close")->pCbParam	= m_pForm->GetComponent ("close")->pCbParam;
	}
	
}

void SkillMenu::CancelSettingShortCut()
{

}

