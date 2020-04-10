#include "MainMenu.h"
#include "ios_home_packet.h"
#include "FamilyMenu.h"
#include "FamilyCenter.h"
#include "ios_user_packet.h"
#include "UserConfig.h"
#include "MapStaticCfg.h"
#include "MapMouse.h"
char* szHomeDuty[] = {"成员","元老","副官","团长"};
static int nMemberLimit[] = {0,30,35,40,45,50,55,60,65,70,75};
void FamilyInfoMenu::Open( int nT )
{

	m_bUpdate = true;
	pChar->SendCmd(NHOME,C_GETHOMEINFO);
	m_pForm->Open (nT);
	
}
static void ChangeLeagueDescEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	char	*	szMsg	= pMainMenu->m_pFamilyNoticeMenu->m_pForm->GetComponent("txt")->GetEdit ();
	if(strlen (szMsg)>=100)
	{
		pMainMenu->PopTipMsg("军团公告不得超过100个字符");
		return;
	}
	if (pChar->m_dwHomeDuty == 0)
	{
		pMainMenu->PopTipMsg("没有相应的权限");
		return;
	}
	BuildPacketEx(pPacket,NHOME, C_CHANGE_LEAGUE_DESC, buf, 1024);
	SET_DATA(pData, NHOME, C_CHANGE_LEAGUE_DESC, pPacket);
	strncpy(pData->szDesc,szMsg,100);
	pChar->SendPacket(pPacket);
	pMainMenu->m_pFamilyNoticeMenu->m_pForm->Close();
}
static void OpenFamilyQuest(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pTabFamilyCtrl->Open(1,4);
}
static void OpenFamilyDesc(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pFamilyNoticeMenu->Open();
}
static void FamilyHome(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nTime = pChar->m_pClientFlag->GetCPlimitF(404);
	if (nTime)
	{
		pMainMenu->PopTipMsg("本日已进入过，无法再次挑战！");
		return;
	}
	if(pChar->m_pCQuestCenter->GetCurQuest(5001) ||
		pChar->m_pCQuestCenter->GetCurQuest(5002) ||
		pChar->m_pCQuestCenter->GetCurQuest(5003))
	{
		pMainMenu->PopTipMsg("你正在护送商队，无法进入副本！");
		return;
	}
	pChar->SendCmd(USER,C_JUMP_HOME_WARCOPY);
	pMainMenu->m_pFamilyInfoMenu->m_pForm->Close();
}

void	fun_backDissolveHome(void* param,int nVal)
{
	if(nVal == 1)
		pLeague->DissolveHome();
}

static void	ClickDissolveHomeButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->PopAskMsgBox("是否确认解散公会？",fun_backDissolveHome,NULL);
}

FamilyInfoMenu::FamilyInfoMenu()
{
	m_pForm = pMainInterface->GetDForm ("juntuan1");
	char sztxt[32];

	m_pForm->GetComponent("bj")->SetEvent(OpenFamilyDesc);
	m_pForm->GetComponent("add")->SetEvent(OpenFamilyQuest);
	m_pForm->GetComponent("fb")->SetEvent(FamilyHome);
	m_pForm->GetComponent("js")->SetEvent(ClickDissolveHomeButton);
	//m_pForm->pCFrame->SetFPCnt (10);
//	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);
// 	m_pForm->GetComponent("guanli")->SetEvent(OpenLeagueJoinEvent,this);
// 
// 	m_pForm->GetComponent("fanhui")->SetEvent(ReturnEvent);
// 	m_pForm->GetComponent("rutuansq")->SetEvent(JoinLeagueEvent);
// 	m_pForm->GetComponent("renming")->SetEvent(ChangeDutyEvent,(void*)2);
// 	m_pForm->GetComponent("yuanlao")->SetEvent(ChangeDutyEvent,(void*)1);
// 	m_pForm->GetComponent("bm")->SetEvent(ChangeDutyEvent,(void*)0);
// 	m_pForm->GetComponent("fire")->SetEvent(KickEvent);
// 	m_pForm->GetComponent("tc")->SetEvent(ExitEvent);
// 	m_pForm->GetComponent("zhuanrang")->SetEvent(ChangeLeagueLeaderEvent);
}

FamilyInfoMenu::~FamilyInfoMenu()
{

}

void FamilyInfoMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		m_pForm->GetComponent("nameC4")->SetCaption(pLeague->m_szName,0);
		m_pForm->GetComponent("lv")->SetCaptionEx("%d",0,pLeague->m_bLv);
		m_pForm->GetComponent("contri")->SetCaptionEx("%d/%d",0,pLeague->GetContribute(),pChar->m_dwHomeMaxCtb);
		m_pForm->GetComponent("post")->SetCaptionEx("%s",0,szHomeDuty[pLeague->GetDuty()]);
		m_pForm->GetComponent("res")->SetCaptionEx("%d",0,pLeague->m_dwExp);
		m_pForm->GetComponent("gonggao")->SetCaption(pLeague->m_szDesc,0);
		m_pForm->GetComponent("bj")->SetVisible(pChar->m_dwHomeDuty != 0);

		if (pLeague->m_pMemeber->Count > nMemberLimit[pLeague->m_bLv])
			m_pForm->GetComponent("cy")->SetCaptionEx("%d/%d",0,pLeague->m_pMemeber->Count,pLeague->m_pMemeber->Count);
		else
			m_pForm->GetComponent("cy")->SetCaptionEx("%d/%d",0,pLeague->m_pMemeber->Count,nMemberLimit[pLeague->m_bLv]);
	}
	if(pLeague->GetDuty() == 3)
		m_pForm->GetComponent("js")->SetVisible(true);
	else
		m_pForm->GetComponent("js")->SetVisible(false);
}

static void CBFrameMember(int nPos,char* szName)
{
	if(strncmp (szName, "n1",2) == 0)
	{
		pMainMenu->m_pFamilyMemberMenu->Select(nPos);
	}
}

static void OpenJoinMenuEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pFamilyJoinMenu->Open(1);
}
void CBAskLeaveLeague(void * pParam,int nVal)
{
	if (nVal)
	{
		BuildPacketEx(pPacket,NHOME, C_EXIT, buf, 1024);
		SET_DATA(pData, NHOME, C_EXIT, pPacket);
		pData->dwUin = pChar->m_dwUin;
		pChar->SendPacket(pPacket);
		pMainMenu->m_pTabFamilyCtrl->Close();
	}
}
static void LeaveFamilyEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_dwHomeDuty == 3)
	{
		pMainMenu->PopTipMsg("军团长不能退出");
		return;
	}
	pMainMenu->PopAskMsgBox("是否确认退出军团?",CBAskLeaveLeague,NULL);
}
void CBAskChangeLeagueLeader(void * pParam,int nVal)
{
	if (nVal)
	{
		if (pMainMenu->m_pFamilyMemberMenu->m_nSelect<0)
		{
			pMainMenu->PopTipMsg("请先选择一位成员");
			return;
		}
		NHomeMember* pInfo = (NHomeMember* )pMainMenu->m_pFamilyMemberMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyMemberMenu->m_nSelect);
		if (!pInfo)
		{
			return;
		}
		BuildPacketEx(pPacket,NHOME, C_DUTY, buf, 1024);
		SET_DATA(pData, NHOME, C_DUTY, pPacket);
		pData->dwUin = pInfo->dwUin;
		pData->byDuty = 3;
		pChar->SendPacket(pPacket);
	}
}
static void ChangeLeaderEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFamilyMemberMenu->m_nSelect<0)
	{
		pMainMenu->PopTipMsg("请先选择一位成员");
		return;
	}
	NHomeMember* pInfo = (NHomeMember* )pMainMenu->m_pFamilyMemberMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyMemberMenu->m_nSelect);
	if (!pInfo)
	{
		return;
	}
	if (pInfo->dwUin == pChar->m_dwUin)
	{
		pMainMenu->PopTipMsg("不能选择自己");
		return;
	}
	pMainMenu->PopAskMsgBox("确定要转让团长吗?",CBAskChangeLeagueLeader,NULL);
}

static void ChangeDutyEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nDuty = (int)param;
	if (pMainMenu->m_pFamilyMemberMenu->m_nSelect<0)
	{
		pMainMenu->PopTipMsg("请先选择一位成员");
		return;
	}
	NHomeMember* pInfo = (NHomeMember* )pMainMenu->m_pFamilyMemberMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyMemberMenu->m_nSelect);
	if (!pInfo)
	{
		return;
	}
	if (pInfo->dwUin == pChar->m_dwUin)
	{
		pMainMenu->PopTipMsg("不能选择自己");
		return;
	}
	BuildPacketEx(pPacket,NHOME, C_DUTY, buf, 1024);
	SET_DATA(pData, NHOME, C_DUTY, pPacket);
	pData->dwUin = pInfo->dwUin;
	pData->byDuty = nDuty;
	pChar->SendPacket(pPacket);
}
void CBAskKickLeague(void * pParam,int nVal)
{
	if (nVal)
	{
		NHomeMember* pInfo = (NHomeMember*)pParam;
		BuildPacketEx(pPacket,NHOME, C_EXIT, buf, 1024);
		SET_DATA(pData, NHOME, C_EXIT, pPacket);
		pData->dwUin = pInfo->dwUin;
		pChar->SendPacket(pPacket);
	}
}
static void KickEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFamilyMemberMenu->m_nSelect<0)
	{
		pMainMenu->PopTipMsg("请先选择一位成员");
		return;
	}
	NHomeMember* pInfo = (NHomeMember* )pMainMenu->m_pFamilyMemberMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyMemberMenu->m_nSelect);
	if (!pInfo)
	{
		return;
	}
	if (pChar->m_dwHomeDuty<=pInfo->bDuty)
	{
		pMainMenu->PopTipMsg("权限不足");
		return;
	}
	char sztxt[64];
	sprintf(sztxt,"是否将%s移出军团?",pInfo->szName);
	pMainMenu->PopAskMsgBox(sztxt,CBAskKickLeague,pInfo);
}
FamilyMemberMenu::FamilyMemberMenu():m_nSelect(-1),m_bSelectPos(false)
{
	m_pForm = pMainInterface->GetDForm ("juntuan2");
	char sztxt[32];
	m_pForm->pCFrame	= new CCtrlFrame(40,190,876,340);

	DComponent*	pComp	= m_pForm->GetComponent ("n1");
	pComp->SetVisible (false);
	DComponent*	ppComps[7];
	ppComps[0]		= m_pForm->GetComponent ("xz");
	ppComps[1]		= m_pForm->GetComponent ("name");
	ppComps[2]		= m_pForm->GetComponent ("job");
	ppComps[3]		= m_pForm->GetComponent ("lvl");
	ppComps[4]		= m_pForm->GetComponent ("post");
	ppComps[5]		= m_pForm->GetComponent ("contri");
	ppComps[6]		= m_pForm->GetComponent ("x1");

	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep=50;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,7);
	m_pForm->pCFrame->SetEvent(CBFrameMember);

	m_pForm->GetComponent("list")->SetEvent(OpenJoinMenuEvent);
	m_pForm->GetComponent("tc")->SetEvent(LeaveFamilyEvent);
	m_pForm->GetComponent("zhuanrang")->SetEvent(ChangeLeaderEvent);
	m_pForm->GetComponent("renming")->SetEvent(ChangeDutyEvent,(void*)2);
	m_pForm->GetComponent("yuanlao")->SetEvent(ChangeDutyEvent,(void*)1);
	m_pForm->GetComponent("bm")->SetEvent(ChangeDutyEvent,(void*)0);
	m_pForm->GetComponent("fire")->SetEvent(KickEvent);
}

FamilyMemberMenu::~FamilyMemberMenu()
{

}
extern char* szJobTxt[];

static int MemberSortByOnline(const void * Item1, const void * Item2)
{
	NHomeMember * pData1 = *(NHomeMember **)Item1;
	NHomeMember * pData2 = *(NHomeMember **)Item2;
    return (pData2->bOnline?100:0)-(pData1->bOnline?100:0);
}

void FamilyMemberMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}

	if (m_bSelectPos)
	{
		m_bSelectPos = false;
		for (int i=0;i<pLeague->m_pMemeber->Count;i++)
		{
			m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(i == m_nSelect?true:false);
		}
	}

	if (m_bUpdate)
	{
		m_bUpdate = false;
		m_pForm->GetComponent("tc")->SetVisible(pChar->m_dwHomeDuty != 3);
		m_pForm->GetComponent("renming")->SetVisible(pChar->m_dwHomeDuty >=2);
		m_pForm->GetComponent("yuanlao")->SetVisible(pChar->m_dwHomeDuty == 3);
		m_pForm->GetComponent("bm")->SetVisible(pChar->m_dwHomeDuty >=2);
		m_pForm->GetComponent("fire")->SetVisible(pChar->m_dwHomeDuty >=2);
		m_pForm->pCFrame->SetFPCnt (pLeague->m_pMemeber->Count);

		pLeague->m_pMemeber->Sort(MemberSortByOnline);

		for (int i=0;i<pLeague->m_pMemeber->Count;i++)
		{
			NHomeMember* pItem	= (NHomeMember*)pLeague->m_pMemeber->Items[i];

			m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(i == m_nSelect?true:false);
			m_pForm->pCFrame->GetFPComp(i,"name")->SetCaption(pItem->szName,0);
			m_pForm->pCFrame->GetFPComp(i,"name")->SetSprColor(pItem->bOnline?255:125,pItem->bOnline?255:125,pItem->bOnline?255:125);
			m_pForm->pCFrame->GetFPComp(i,"lvl")->SetCaptionEx("%d",1,pItem->bLv);
			m_pForm->pCFrame->GetFPComp(i,"lvl")->SetSprColor(pItem->bOnline?255:125,pItem->bOnline?255:125,pItem->bOnline?255:125);
			m_pForm->pCFrame->GetFPComp(i,"post")->SetCaptionEx("%s",1,szHomeDuty[pItem->bDuty]);
			m_pForm->pCFrame->GetFPComp(i,"post")->SetSprColor(pItem->bOnline?255:125,pItem->bOnline?255:125,pItem->bOnline?255:125);
			m_pForm->pCFrame->GetFPComp(i,"contri")->SetCaptionEx("%d",1,pItem->dwContribute);
			m_pForm->pCFrame->GetFPComp(i,"contri")->SetSprColor(pItem->bOnline?255:125,pItem->bOnline?255:125,pItem->bOnline?255:125);
			m_pForm->pCFrame->GetFPComp(i,"job")->SetCaptionEx("%s",1,szJobTxt[pItem->bJob]);
			m_pForm->pCFrame->GetFPComp(i,"job")->SetSprColor(pItem->bOnline?255:125,pItem->bOnline?255:125,pItem->bOnline?255:125);
			m_pForm->pCFrame->SetFPTag(i,pItem);
		}

		m_pForm->GetComponent("num")->SetCaptionEx("%d",0,pLeague->GetOnlineCount());
	}
}

void FamilyMemberMenu::Open( int nT )
{
	m_bUpdate = true;
	//pChar->SendCmd(NHOME,C_GETHOMEINFO);
	m_pForm->Open (nT);
}

void FamilyMemberMenu::Select( int nPos )
{
	m_nSelect = nPos;
	//m_bUpdate = true;
	m_bSelectPos = true;
}

static void CBFrameFBEvent(int nPos,char* szName)
{
	if(strncmp (szName, "learn",3) == 0)
	{
		if (pChar->m_xHomeSkill.bySkillLv[nPos]>=10)
		{
			pMainMenu->PopTipMsg("该技能已到最高级");
			return;
		}
		if (pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]<=pChar->m_xHomeSkill.bySkillLv[nPos])
		{
			pMainMenu->PopTipMsg("需要研究院等级达到%d级",pChar->m_xHomeSkill.bySkillLv[nPos]+1);
			return;
		}
		BuildPacketEx(pPacket,USER, C_LEARN_HOMESKILL, buf, 1024);
		SET_DATA(p, USER, C_LEARN_HOMESKILL, pPacket);
		p->byType = nPos;
		pChar->SendPacket(pPacket);
	}
}
FamilySkillMenu::FamilySkillMenu()
{
	m_pForm = pMainInterface->GetDForm ("juntuan3");

	DComponent*	pComp	= m_pForm->GetComponent ("zi");
	m_pForm->pCFrame	= new CCtrlFrame(pComp->nX,pComp->nY,pComp->nW,(pComp->nH+5)*4);
	pComp->SetVisible (false);
	DComponent*	ppComps[9];
	ppComps[1]		= m_pForm->GetComponent ("icon");
	ppComps[0]		= m_pForm->GetComponent ("gezi");
	ppComps[2]		= m_pForm->GetComponent ("ms");
	ppComps[3]		= m_pForm->GetComponent ("name");
	ppComps[4]		= m_pForm->GetComponent ("learn");
	ppComps[5]		= m_pForm->GetComponent ("gx");
	ppComps[6]		= m_pForm->GetComponent ("num");
	ppComps[7]		= m_pForm->GetComponent ("mb");
	ppComps[8]		= m_pForm->GetComponent ("x1");

	m_pForm->pCFrame->nSpace = 5;
	m_pForm->pCFrame->nDisStep = pComp->nH+5;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,9);
	m_pForm->pCFrame->SetFPCnt(USER_HOME_SKILL_MAX);
	m_pForm->pCFrame->SetEvent(CBFrameFBEvent);
}

FamilySkillMenu::~FamilySkillMenu()
{

}

void FamilySkillMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		for (int i=0;i<USER_HOME_SKILL_MAX;i++)
		{
			static char* szTitle[USER_HOME_SKILL_MAX] = {"生命强化","魔法强化","命中强化","闪避强化","暴击强化"};
			m_pForm->pCFrame->GetFPComp(i,"name")->SetCaptionEx("%s%d级",0,szTitle[i],pChar->m_xHomeSkill.bySkillLv[i]);
			static char* szDesc[USER_HOME_SKILL_MAX] = {"生命提高","魔法提高","命中提高","闪避提高","暴击提高"};
			m_pForm->pCFrame->GetFPComp(i,"ms")->SetCaptionEx("%s%d%%",0,szTitle[i],pChar->m_xHomeSkill.bySkillLv[i]);
			static int nContr[11] = {100,200,400,800,1600,3200,6400,12800,25600,51200,0};
			m_pForm->pCFrame->GetFPComp(i,"num")->SetCaptionEx("%d",0,nContr[pChar->m_xHomeSkill.bySkillLv[i]]);
			//m_pForm->pCFrame->GetFPComp(i,"learn")->SetEnable(pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]>pChar->m_xHomeSkill.bySkillLv[i]);
			m_pForm->pCFrame->GetFPComp(i,"mb")->SetVisible(false);
			static char* szSkill[USER_HOME_SKILL_MAX] = {"BHSKILL1.png","BHSKILL2.png","BHSKILL3.png","BHSKILL4.png","BHSKILL5.png"};

			m_pForm->pCFrame->GetFPComp(i,"icon")->SetViewSpr(szSkill[i]);

		}
		m_pForm->GetComponent("num1")->SetCaptionEx("%d",0,pLeague->GetContribute());
	}
}

void FamilySkillMenu::Open( int nT )
{
	m_bUpdate = true;
	m_pForm->Open (nT);
}

static void BuildEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
// 	if (pLeague->m_dwExp<=)
// 	{
// 	}
	pChar->SendCmd(NHOME,C_ADD_BUILD_LV);
}
static void CBFrameFBBuildEvent(int nPos,char* szName)
{
	if(strncmp (szName, "up",2) == 0)
	{
		static int nCost1[] = {50,100,200,400,800,1600,3200,6400,12800,25600};
		static int nCost2[] = {0,250,500,1000,2000,4000,8000,16000,32000,64000};
		int nCost = 0;
		if (nPos == 1)
		{
			nCost = nCost1[pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]];
			if (pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]>=pLeague->m_bLv)
			{
				pMainMenu->PopTipMsg("军团等级不足，无法升级。 ");
				return;
			}
			if (pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]>=10)
			{
				pMainMenu->PopTipMsg("该建筑已满级");
				return;
			}
		}else if (nPos == 0)
		{
			nCost = nCost2[pLeague->m_bLv];
			if (pLeague->m_bLv>=10)
			{
				pMainMenu->PopTipMsg("该建筑已满级");
				return;
			}
		}else{
			return;
		}
		if (pLeague->m_dwExp< nCost)
		{
			pMainMenu->PopTipMsg("所需军团资源不足");
			return;
		}
		BuildPacketEx(pPacket,NHOME, C_ADD_BUILD_LV, buf, 1024);
		SET_DATA(p, NHOME, C_ADD_BUILD_LV, pPacket);
		p->byType = nPos;
		pChar->SendPacket(pPacket);
	}
}
FamilyBuildMenu::FamilyBuildMenu()
{
	m_pForm = pMainInterface->GetDForm ("juntuan4");

	m_pForm->pCFrame	= new CCtrlFrame(40,230,876,340);

	DComponent*	pComp	= m_pForm->GetComponent ("zi");
	pComp->SetVisible (false);
	DComponent*	ppComps[9];
	ppComps[1]		= m_pForm->GetComponent ("icon");
	ppComps[0]		= m_pForm->GetComponent ("gezi");
	ppComps[2]		= m_pForm->GetComponent ("ms");
	ppComps[3]		= m_pForm->GetComponent ("name");
	ppComps[4]		= m_pForm->GetComponent ("tj");
	ppComps[5]		= m_pForm->GetComponent ("up");
	ppComps[6]		= m_pForm->GetComponent ("num");
	ppComps[7]		= m_pForm->GetComponent ("mb");
	ppComps[8]		= m_pForm->GetComponent ("x1");

	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep=105;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,9);
	m_pForm->pCFrame->SetFPCnt(2);
	m_pForm->pCFrame->SetEvent(CBFrameFBBuildEvent);
	m_pForm->GetComponent("up")->SetEvent(BuildEvent);
}

FamilyBuildMenu::~FamilyBuildMenu()
{

}

void FamilyBuildMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;

		m_pForm->GetComponent("res")->SetCaptionEx("%d",0,pLeague->m_dwExp);


		static int nCost1[] = {50,100,200,400,800,1600,3200,6400,12800,25600};
		if (pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH] < 10)
		{
			m_pForm->pCFrame->GetFPComp(1,"ms")->SetCaptionEx("消耗军团资源%d",0,nCost1[pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]]);
			m_pForm->pCFrame->GetFPComp(1,"num")->SetCaptionEx("军团等级%d",0,pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]+1);
		}
		else
		{
			m_pForm->pCFrame->GetFPComp(1,"ms")->SetCaption("已满级",0);
			m_pForm->pCFrame->GetFPComp(1,"num")->SetCaption("无",0);
		}
		m_pForm->pCFrame->GetFPComp(1,"name")->SetCaptionEx("研究院%d级",0,pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]);
		//m_pForm->pCFrame->GetFPComp(1,"up")->SetEnable(pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]<pLeague->m_bLv);
		m_pForm->pCFrame->GetFPComp(1,"mb")->SetVisible(false);
		m_pForm->pCFrame->GetFPComp(1,"icon")->SetViewSpr("BHJZ2.png");

		static int nCost2[] = {0,250,500,1000,2000,4000,8000,16000,32000,64000};
		if (pLeague->m_bLv < 10)
			m_pForm->pCFrame->GetFPComp(0,"ms")->SetCaptionEx("消耗军团资源%d",0,nCost2[pLeague->m_bLv]);
		else
			m_pForm->pCFrame->GetFPComp(0,"ms")->SetCaption("已满级",0);
		m_pForm->pCFrame->GetFPComp(0,"name")->SetCaptionEx("军团等级%d级",0,pLeague->m_bLv);
		m_pForm->pCFrame->GetFPComp(0,"num")->SetCaptionEx("无",0);
		m_pForm->pCFrame->GetFPComp(0,"mb")->SetVisible(false);
		m_pForm->pCFrame->GetFPComp(0,"icon")->SetViewSpr("BHJZ1.png");
		//m_pForm->pCFrame->GetFPComp(0,"up")->SetEnable(pLeague->m_xBuild.nBuildLv[HOME_BUILD_RESEARCH]<pLeague->m_bLv);
	}
}

void FamilyBuildMenu::Open( int nT )
{
	m_bUpdate = true;
	m_pForm->Open (nT);
}


static void FamilyMissionAcceptEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_pClientFlag->GetFlag(nFamilyQuestTimesFlag)>=10)
	{
		pMainMenu->PopTipMsg("每天最多领取10次军团任务");
		return;
	}
	int nQuestID = pChar->m_pClientFlag->GetFlag(nFamilyQuestIDFlag);
	if(!nQuestID)
		return;
	
	BuildPacketEx(pPacket,USER,C_HOME_QUEST_ACCEPT,buf,256);
	SET_DATA(pSend,USER,C_HOME_QUEST_ACCEPT,pPacket);
	pSend->wQid	= nQuestID;
	pChar->SendPacket(pPacket);
}

static void FamilyMissionCompleteEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(nFamilyQuestIDFlag);
	if(!nQuestID)
		return;

	BuildPacketEx(pPacket,USER,C_HOME_QUEST_COMPLETE,buf,256);
	SET_DATA(pSend,USER,C_HOME_QUEST_COMPLETE,pPacket);
	pSend->wQid	= nQuestID;
	pChar->SendPacket(pPacket);

}

static void FamilyMissionGiveUpEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(nFamilyQuestIDFlag);
	if(!nQuestID)
		return;
	BuildPacketEx(pPacket,USER,C_QUEST_GIVEUP,buf,256);
	SET_DATA(pSend,USER,C_QUEST_GIVEUP,pPacket);
	pSend->wQid	= nQuestID;
	pChar->SendPacket(pPacket);
}

static void FamilyMissionGotoEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nQuestID = pChar->m_pClientFlag->GetFlag(nFamilyQuestIDFlag);
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
			//g_pMapCommand->CmdMapPoint(pNpc->nMapindex,pNpc->nPosX,pNpc->nPosY,pNpc);
		}
		pMainMenu->m_pFamilyQuestMenu->m_pForm->Close();
	}

}
FamilyQuestMenu::FamilyQuestMenu()
{
	m_pForm = pMainInterface->GetDForm ("juntuan5");
	m_pForm->GetComponent("bt1")->SetEvent(FamilyMissionAcceptEvent,this);
	m_pForm->GetComponent("bt2")->SetEvent(FamilyMissionCompleteEvent,this);
	m_pForm->GetComponent("bt3")->SetEvent(FamilyMissionGiveUpEvent,this);
	m_pForm->GetComponent("bt4")->SetEvent(FamilyMissionGotoEvent,this);


}

FamilyQuestMenu::~FamilyQuestMenu()
{

}

void FamilyQuestMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		int nQuestID = pChar->m_pClientFlag->GetFlag(nFamilyQuestIDFlag);
		int nQuestTimes = pChar->m_pClientFlag->GetFlag(nFamilyQuestTimesFlag);
		QuestObj* pObj = QuestCfg::GetInstance().GetQuestByID(nQuestID);
		static int nGetContr[] = {3,6,9,12,15,18,21,24,27,30};
		if (pObj)
		{
			UpViewQuestComp(m_pForm,pObj,NULL);
			m_pForm->GetComponent("money")->SetCaptionEx("%d",1,nGetContr[nQuestTimes]);
			m_pForm->GetComponent("exp")->SetCaptionEx("%d",1,(nQuestTimes+1)*pChar->m_byLvl*pChar->m_byLvl);
			m_pForm->GetComponent("zi2")->SetCaptionEx("%s(%d/10)",0,pObj->szName,nQuestTimes);

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
				//m_pForm->GetComponent("bt4")->SetVisible(true);
			}
		}
		
	}
}

void FamilyQuestMenu::Open( int nT )
{
	m_bUpdate = true;
	m_pForm->Open (nT);
}

void FamilyQuestMenu::UpViewQuestComp( DForm* pForm, QuestObj* pQBase, ClientQuest* pClientQ )
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

static void OpenLeagueCreateEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pFamilyCreateMenu->Open();
}
static void JoinLeague(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pChar->m_byLvl>=20)
	{
		NHomeSInfo* pInfo = (NHomeSInfo* )pMainMenu->m_pFamilyListMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyListMenu->m_nSelect);
		if (pInfo)
		{
			if(pMainMenu->m_pFamilyListMenu->m_nRqCnt >= 5)
			{
				pMainMenu->PopTipMsg("最多申请5个军团");
				return;
			}

			BuildPacketEx(pPacket,NHOME, C_REQUEST, buf, 1024);
			SET_DATA(pData, NHOME, C_REQUEST, pPacket);
			pData->wHomeID = pInfo->wHomeID;
			pChar->SendPacket(pPacket);
		}
	}else
		pMainMenu->PopTipMsg("达到20级才能申请军团");
}
static void CancelJoinLeague(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NHomeSInfo* pInfo = (NHomeSInfo* )pMainMenu->m_pFamilyListMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyListMenu->m_nSelect);
	if (pInfo&&pInfo->bApply)
	{

		BuildPacketEx(pPacket,NHOME, C_CANCEL_REQUEST, buf, 1024);
		SET_DATA(pData, NHOME, C_CANCEL_REQUEST, pPacket);
		pData->wHomeID = pInfo->wHomeID;
		pChar->SendPacket(pPacket);


	}
}
static void CBFrameList(int nPos,char* szName)
{
	if(strncmp (szName, "n1",2) == 0)
	{
		pMainMenu->m_pFamilyListMenu->Select(nPos);
	}
}
FamilyListMenu::FamilyListMenu():m_nSelect(-1)
{
	m_pForm = pMainInterface->GetDForm ("juntuan6");

	m_pForm->pCFrame	= new CCtrlFrame(40,190,876,340);

	DComponent*	pComp	= m_pForm->GetComponent ("n1");
	pComp->SetVisible (false);
	DComponent*	ppComps[8];
	ppComps[0]		= m_pForm->GetComponent ("xz");
	ppComps[1]		= m_pForm->GetComponent ("name");
	ppComps[2]		= m_pForm->GetComponent ("org");
	ppComps[3]		= m_pForm->GetComponent ("lv");
	ppComps[4]		= m_pForm->GetComponent ("leader");
	ppComps[5]		= m_pForm->GetComponent ("num");
	ppComps[6]		= m_pForm->GetComponent ("x1");
	ppComps[7]		= m_pForm->GetComponent ("y1");

	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep=50;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,8);

	m_pForm->pCFrame->SetEvent(CBFrameList);
	m_pForm->GetComponent("jl")->SetEvent(OpenLeagueCreateEvent);
	m_pForm->GetComponent("join")->SetEvent(JoinLeague);
	m_pForm->GetComponent("qx")->SetEvent(CancelJoinLeague);

	m_nRqCnt	= 0;
}

FamilyListMenu::~FamilyListMenu()
{

}

void FamilyListMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		int nLeagueCnt = 0;
		for (int i=0;i<pLeague->m_pLeagueList->Count;i++)
		{
			NHomeSInfo* pItem	= (NHomeSInfo*)pLeague->m_pLeagueList->Items[i];
			if (pItem->bCountry != pChar->m_byCountry)
			{
				continue;
			}
			nLeagueCnt++;
		}
		m_pForm->pCFrame->SetFPCnt (nLeagueCnt);
		m_nRqCnt	= 0;
		nLeagueCnt = 0;
		for (int i=0;i<pLeague->m_pLeagueList->Count;i++)
		{
			NHomeSInfo* pItem	= (NHomeSInfo*)pLeague->m_pLeagueList->Items[i];
			if (pItem->bCountry != pChar->m_byCountry)
			{
				continue;
			}
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"xz")->SetVisible(nLeagueCnt==m_nSelect);
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"name")->SetCaption(pItem->szName,0);
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"lv")->SetCaptionEx("%d",0,pItem->bLv);
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"num")->SetCaptionEx("%d/%d",0,pItem->wMemCnt,pItem->wMemCnt>nMemberLimit[pItem->bLv]?pItem->wMemCnt:nMemberLimit[pItem->bLv]);
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"org")->SetCaptionEx("%s",0,pItem->bCountry == 1?"联盟":"部落");
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"leader")->SetCaptionEx("%s",0,pItem->szCreateName);
			m_pForm->pCFrame->GetFPComp(nLeagueCnt,"y1")->SetVisible(pItem->bApply);
			if(pItem->bApply)
				m_nRqCnt++;//限制军团次数
			//m_pForm->pCFrame->GetFPComp(i,"leader")->SetCaptionEx("%d",0,pItem->wMemCnt);
			m_pForm->pCFrame->SetFPTag(nLeagueCnt,pItem);
			nLeagueCnt++;
		}
		if (m_nSelect>=0&&m_nSelect <pLeague->m_pLeagueList->Count)
		{
			NHomeSInfo* pItem	= (NHomeSInfo*)pLeague->m_pLeagueList->Items[m_nSelect];
			m_pForm->GetComponent("qx")->SetVisible(pItem->bApply);
		}
	}
}

void FamilyListMenu::Open( int nT )
{
	if (pChar->m_dwHomeID)
	{
		pMainMenu->m_pTabFamilyCtrl->Open(0);
		return;
	}
	m_bUpdate = true;
	pChar->SendCmd(NHOME,C_RQHOMES);
	m_pForm->Open (nT);
}

void FamilyListMenu::Select( int nPos )
{
	m_nSelect = nPos;
	m_bUpdate = true;
	if (m_nSelect>=0&&m_nSelect <pLeague->m_pLeagueList->Count)
	{
		NHomeSInfo* pItem	= (NHomeSInfo*)pLeague->m_pLeagueList->Items[m_nSelect];
		m_pForm->GetComponent("qx")->SetVisible(pItem->bApply);
	}
}

static void FamilyCreateEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	char	*	szMsg	= pMainMenu->m_pFamilyCreateMenu->m_pForm->GetComponent("name")->GetEdit ();
	if(strlen (szMsg) == 0||strlen (szMsg)>=14)
	{
		pMainMenu->PopTipMsg("军团名长度不正确");
		return;
	}
// 	if (pChar->m_byVipLvl<3)
// 	{
// 		pMainMenu->PopTipMsg("创建军团需要VIP等级3");
// 		return;
// 	}
// 	if (pChar->m_dwMoney<50000)
// 	{
// 		pMainMenu->PopSystemMsgEx("金钱不足","金钱不足，可前往征战关卡获得金钱！",2);
// 		return;
// 	}
// 	if (pChar->m_byLvl<20)
// 	{
// 		pMainMenu->PopTipMsg("创建军团需要等级20");
// 		return;
// 	}

	BuildPacketEx(pPacket,NHOME, C_CREATE, buf, 1024);
	SET_DATA(p, NHOME, C_CREATE, pPacket);
	strncpy(p->szName,szMsg,20);
	p->bUseRmb = pMainMenu->m_pFamilyCreateMenu->m_bUseRmb;
	pChar->SendPacket(pPacket);
	pMainMenu->m_pFamilyCreateMenu->m_pForm->Close();

}

static void ChooseWealthType(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	FamilyCreateMenu*  pMenu = (FamilyCreateMenu*)param;
	if (pMenu)
	{
		pMenu->m_bUseRmb = !pMenu->m_bUseRmb;
		pMenu->Refresh();
	}
}

FamilyCreateMenu::FamilyCreateMenu()
{
	m_pForm = pMainInterface->GetDForm ("juntuan7");
	m_pForm->GetComponent("ok")->SetEvent(FamilyCreateEvent);
	m_pForm->GetComponent("gg1")->SetEvent(ChooseWealthType,this);
	m_pForm->GetComponent("gg2")->SetEvent(ChooseWealthType,this);

	m_bUseRmb = false;
}

void FamilyCreateMenu::Open()
{
	m_pForm->GetComponent("name")->SetEdit("");
	m_pForm->Open();
	Refresh();
}

void FamilyCreateMenu::Refresh()
{
	m_pForm->GetComponent("g1")->SetVisible(!m_bUseRmb);
	m_pForm->GetComponent("g2")->SetVisible(m_bUseRmb);
}

static void AccpetJoin(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NHomeMember* pInfo = (NHomeMember* )pMainMenu->m_pFamilyJoinMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyJoinMenu->m_nSelect);
	if (!pInfo)
	{
		return;
	}
	BuildPacketEx(pPacket,NHOME, C_ACCEPT, buf, 1024);
	SET_DATA(pData, NHOME, C_ACCEPT, pPacket);
	pData->wResult = 1;
	pData->dwUin = pInfo->dwUin;
	pChar->SendPacket(pPacket);
}

static void RefuseJoin(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	NHomeMember* pInfo = (NHomeMember* )pMainMenu->m_pFamilyJoinMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFamilyJoinMenu->m_nSelect);
	if (!pInfo)
	{
		return;
	}
	BuildPacketEx(pPacket,NHOME, C_ACCEPT, buf, 1024);
	SET_DATA(pData, NHOME, C_ACCEPT, pPacket);
	pData->wResult = 0;
	pData->dwUin = pInfo->dwUin;
	pChar->SendPacket(pPacket);
}
static void CBFrameJoin(int nPos,char* szName)
{
	if(strncmp (szName, "n1",2) == 0)
	{
		pMainMenu->m_pFamilyJoinMenu->Select(nPos);
	}
}
FamilyJoinMenu::FamilyJoinMenu():m_nSelect(-1)
{
	m_pForm = pMainInterface->GetDForm ("juntuan8");

	m_pForm->pCFrame	= new CCtrlFrame(40,190,876,340);

	DComponent*	pComp	= m_pForm->GetComponent ("n1");
	pComp->SetVisible (false);
	DComponent*	ppComps[6];
	ppComps[0]		= m_pForm->GetComponent ("xz");
	ppComps[1]		= m_pForm->GetComponent ("name");
	ppComps[2]		= m_pForm->GetComponent ("job");
	ppComps[3]		= m_pForm->GetComponent ("lv");
	ppComps[4]		= m_pForm->GetComponent ("jun");
	ppComps[5]		= m_pForm->GetComponent ("x1");

	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep=50;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,6);
 	m_pForm->pCFrame->SetEvent(CBFrameJoin);
// 	m_pForm->GetComponent("jl")->SetEvent(OpenLeagueCreateEvent);
	m_pForm->GetComponent("ok")->SetEvent(AccpetJoin);
 	m_pForm->GetComponent("no")->SetEvent(RefuseJoin);
}

void FamilyJoinMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		m_pForm->pCFrame->SetFPCnt (pLeague->m_pJoinList->Count);
		for (int i=0;i<pLeague->m_pJoinList->Count;i++)
		{
			NHomeMember* pItem	= (NHomeMember*)pLeague->m_pJoinList->Items[i];

			m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(i==m_nSelect);
			m_pForm->pCFrame->GetFPComp(i,"name")->SetCaption(pItem->szName,0);
			m_pForm->pCFrame->GetFPComp(i,"lv")->SetCaptionEx("%d",0,pItem->bLv);
			m_pForm->pCFrame->GetFPComp(i,"job")->SetCaptionEx("%s",0,szJobTxt[pItem->bJob]);
			//m_pForm->pCFrame->GetFPComp(i,"leader")->SetCaptionEx("%s",0,pItem->szCreateName);
			m_pForm->pCFrame->SetFPTag(i,pItem);
		}
	}
}

void FamilyJoinMenu::Open( int nT )
{
	m_bUpdate = true;
	pChar->SendCmd(NHOME,C_RQLIST);
	m_pForm->Open (nT);
}

void FamilyJoinMenu::Select( int nPos )
{
	m_nSelect = nPos;
	m_bUpdate = true;
}

FamilyNoticeMenu::FamilyNoticeMenu()
{
	m_pForm = pMainInterface->GetDForm ("juntuan9");
	m_pForm->GetComponent("ok")->SetEvent(ChangeLeagueDescEvent);
}

void FamilyNoticeMenu::Open()
{
	m_pForm->Open();
	m_pForm->GetComponent("title")->SetCaption("输入军团公告",0);
	m_pForm->GetComponent("txt")->SetEdit("");
}
