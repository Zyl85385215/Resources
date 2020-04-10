#include "MainMenu.h"
#include "SkillSPMenu.h"
#include "SkillConfig.h"
#include "SkillData.h"
#include "ios_user_packet.h"
#include "UserData.h"

void SpSkillInfoEvent(InterfaceEvent * pEvent,void * param)
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

		pMainMenu->m_pSPSkillMenu->ViewSkillInfo(pSBase,bCurLv);
	}
}

void SPSActiveEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	SkillSPMenu* pMenu	= (SkillSPMenu*)param;
	if(pMenu->m_pCurSelSkill == NULL)
		return;

	BuildPacketEx(pPacket, USER, C_SET_SSHORTCUT,buf,256);
	SET_DATA(pSend, USER, C_SET_SSHORTCUT, pPacket);

	pSend->wSkill	= pMenu->m_pCurSelSkill->wSkillID;
	pSend->byPos	= 5;
	pChar->SendPacket(pPacket);
	pMenu->m_pFormSInfo->Close();
}

void fun_cbSPRemove(void * pParam,int nVal)
{
	if(nVal == 1)
	{
		BuildPacketEx(pPacket, USER, C_REMOVE_SPSKILL,buf,256);
		SET_DATA(pSend, USER, C_REMOVE_SPSKILL, pPacket);

		pSend->wSkill	= (int)pParam;
		pChar->SendPacket(pPacket);
	}

	pMainMenu->m_pSPSkillMenu->m_pFormSInfo->Close();
}

void SPSRemoveEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	SkillSPMenu* pMenu	= (SkillSPMenu*)param;
	if(pMenu->m_pCurSelSkill == NULL)
		return;

	pMainMenu->PopAskMsgBox("确定删除该怒气技能?",fun_cbSPRemove,(void*)pMenu->m_pCurSelSkill->wSkillID);
}

SkillSPMenu::SkillSPMenu()
{
	char	szTmp[256];

	m_pForm		= pMainInterface->GetDForm("nuqi1");

	m_pFormSInfo		= pMainInterface->GetDForm("nuqi2");
	m_pFormSInfo->GetComponent("zb")->SetEvent(SPSActiveEvent,this);
	m_pFormSInfo->GetComponent("qc")->SetEvent(SPSRemoveEvent,this);	//遗忘技能

	for (int i = 0; i < 12; i++)
	{
		sprintf(szTmp,"jn%d",i+1);
		m_ppSkillBack[i]		= m_pForm->GetComponent(szTmp);

		sprintf(szTmp,"n%d",i+1);
		m_ppSkillName[i]	= m_pForm->GetComponent(szTmp);
		sprintf(szTmp,"j%d",i+1);
		m_ppSkillIco[i]		= m_pForm->GetComponent(szTmp);
	}

	m_pCurSelSkill	= NULL;
}

SkillSPMenu::~SkillSPMenu()
{

}

void SkillSPMenu::Open(int nT)
{
	char	szTmp[256];

	for (int i = 0; i < 12; i++)
	{
		if(i >= pChar->m_pSkillCenter->m_pListSPSkill->Count)
		{
			m_ppSkillBack[i]->SetVisible(false);
			m_ppSkillIco[i]->SetVisible(false);
			m_ppSkillName[i]->SetVisible(false);
		}
		else
		{
			SkillObj* pSkill	= (SkillObj*)pChar->m_pSkillCenter->m_pListSPSkill->Items[i];
			m_ppSkillName[i]->SetCaption(pSkill->pSBase->szName,1);
			sprintf(szTmp,"skill/%d.png",pSkill->pSBase->wIcon);
			m_ppSkillIco[i]->SetViewSpr(szTmp);
			m_ppSkillIco[i]->SetEvent(SpSkillInfoEvent,pSkill->pSBase);
			m_ppSkillBack[i]->SetVisible(true);
			m_ppSkillIco[i]->SetVisible(true);
			m_ppSkillName[i]->SetVisible(true);
		}
	}
	m_pForm->Open(nT);
}

void SkillSPMenu::Update()
{
	if(m_pForm->bOpened == false)
		return;
	
	SkillObj*	pSkill	= pChar->m_pSkillCenter->m_ppActSkill[4];
	if(pSkill)
	{
		char	szTmp[256];
		sprintf(szTmp,"skill/%d.png",pSkill->pSBase->wIcon);
		m_pForm->GetComponent("jn")->SetViewSpr(szTmp);
		m_pForm->GetComponent("name")->SetCaption(pSkill->pSBase->szName,1);
		m_pForm->GetComponent("lv")->SetCaptionEx("%d",1,pSkill->bySLv);
		if(pSkill->pSBase->bCostNoPer)
			m_pForm->GetComponent("nuqi")->SetCaptionEx("%d",1,pSkill->pSBase->nSpCost);
		else
			m_pForm->GetComponent("nuqi")->SetCaptionEx("%d%%",1,pSkill->pSBase->nSpCost);
		m_pForm->GetComponent("miaosu")->SetCaption(pSkill->pLvBase->szDesc,0);
	}
	else
	{
		m_pForm->GetComponent("jn")->SetViewSpr("");
		m_pForm->GetComponent("name")->SetCaption("无",1);
		m_pForm->GetComponent("lv")->SetCaption("0",1);
		m_pForm->GetComponent("nuqi")->SetCaption("0",1);
		m_pForm->GetComponent("miaosu")->SetCaption("",0);
	}

}

void SkillSPMenu::ViewSkillInfo( SkillBase* pBase,BYTE	bLv )
{
	m_pCurSelSkill	= pBase;
	m_pFormSInfo->Open(2);

	m_pFormSInfo->GetComponent("xx")->SetVisible(!bLv);

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

	if (bLv<=0 || pBase->bType == 3)
	{
		m_pFormSInfo->GetComponent("kj")->SetVisible(false);

	}else
		m_pFormSInfo->GetComponent("kj")->SetVisible(bVisInfo);


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
	m_pFormSInfo->GetComponent("xh")->SetCaptionEx("%d",1,pBase->nSpCost);

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
	else
		pCompDesc->pSTextCenter->AddSText(nIndex++,0,true,"未学习",255,255,255);


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

}

bool SkillSPMenu::CheckCanOpen()
{
	if(pChar->m_byLvl < 18)
	{
		pMainMenu->PopTipMsg("等级达到18级完成追查任务开启怒气技能");
		return false;
	}
	return true;
}
