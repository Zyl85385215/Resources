#include "MainMenu.h"
#include "UserData.h"
#include "TeamMenu.h"
#include "ios_user_packet.h"


static void OpenQuestMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pTeamMenu->Close();
	pMainMenu->m_pMainFormLB->Open(3);
}

static void HideTeamMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pTeamMenu->Close();
	pMainMenu->m_pMainFormHide->Open();
}

static void ClickSelectTeamMember(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	int nPos = (int)param;
	if (nPos == pMainMenu->m_pTeamMenu->m_nTeamMemberPos)
	{
		pMainMenu->m_pTeamMenu->m_pCompD4[nPos]->SetVisible(false);
		pMainMenu->m_pTeamMenu->m_nTeamMemberPos = -1;
	}else
	{
		pMainMenu->m_pTeamMenu->m_pCompD4[nPos]->SetVisible(true);
		pMainMenu->m_pTeamMenu->m_pCompD4[pMainMenu->m_pTeamMenu->m_nTeamMemberPos]->SetVisible(false);
		pMainMenu->m_pTeamMenu->m_nTeamMemberPos = nPos;
	}
}

static void ClickMoreOperation(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	int nTMID = pMainMenu->m_pTeamMenu->GetSelectMemberID();
	if (!nTMID)
		return;

	switch((int)param)
	{
	case 1:
		{
			//pMainMenu->PopTipMsg("提交队长");
			pChar->SendTeamState(TEAM_COMMIT_CHGLEADER,(BYTE)pMainMenu->m_pTeamMenu->GetSelectMemberPos());
		}
		return;
	case 2:
		{
			//pMainMenu->PopTipMsg("踢出队伍");
			pChar->SendTeamState(TEAM_COMMIT_KICK,(BYTE)pMainMenu->m_pTeamMenu->GetSelectMemberPos());
		}
		return;
	case 3:
		{
			BuildPacketEx(pPacket,USER,C_GET_EQUIPINFO,buf,256);
			SET_DATA(pData, USER,C_GET_EQUIPINFO,pPacket);
			pData->dwUin = nTMID;
			pChar->SendPacket(pPacket);
		}
		return;
	}
	return;
}

static void ClickLeaveTeam(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	pChar->SendTeamState(TEAM_COMMIT_LEAVE,(char*)NULL);
}

TeamMenu::TeamMenu():m_bUpdate(true),m_dwLeaderUin(0)
{
	m_pForm = pMainInterface->GetDForm ("zudui");
	char sztxt[32];
// 	for (int i=0;i<10;i++)
// 	{
// 		sprintf(sztxt,"%d",i);
// 		m_pForm->GetComponent(sztxt)->SetEvent(NumInput,(void*)i);
// 	}
// 	m_pForm->GetComponent("no")->SetEvent(DeleteNumInput);
 	m_pForm->GetComponent("page1")->SetEvent(OpenQuestMenu);
	m_pForm->GetComponent("page3")->SetEvent(HideTeamMenu);

	m_pForm->GetComponent("tj")->SetEvent(ClickMoreOperation,(void*)1);
	m_pForm->GetComponent("ti")->SetEvent(ClickMoreOperation,(void*)2);
	m_pForm->GetComponent("cha")->SetEvent(ClickMoreOperation,(void*)3);
	m_pForm->GetComponent("tui")->SetEvent(ClickLeaveTeam);

	memset(m_xUser,0,TEAM_MAXUSER*sizeof(_TeamNet));
	for (int i=0;i<TEAM_MAXUSER-1;i++)
	{
		sprintf(sztxt,"name%d",i+1);
		m_pCompName[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"hp%d",i+1);
		m_pCompHp[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"tx%d",i+1);
		m_pCompJob[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"lv%d",i+1);
		m_pCompLv[i] = m_pForm->GetComponent(sztxt);
		m_pForm->GetComponent(sztxt)->m_nFontSize = 12;
		sprintf(sztxt,"x%d",i+1);
		m_pCompD1[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"t%d",i+1);
		m_pCompD2[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"lvd%d",i+1);
		m_pCompD3[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"xuan%d",i+1);
		m_pCompD4[i] = m_pForm->GetComponent(sztxt);
		sprintf(sztxt,"xuan1%d",i+1);
		m_pCompSelect[i] = m_pForm->GetComponent(sztxt);
		m_pCompSelect[i]->SetEvent(ClickSelectTeamMember,(void*)i);
		sprintf(sztxt,"q%d",i+1);
		m_pCompLeader[i] = m_pForm->GetComponent(sztxt);
	}
	m_nTeamMemberPos = -1;
}

TeamMenu::~TeamMenu()
{
}


void TeamMenu::Close()
{
	m_pForm->Close();
}

void TeamMenu::Update()
{
	if (!m_pForm->bOpened)
	{
		return;
	}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		m_pForm->GetComponent("tj")->SetVisible(GetMyTeamState()==TEAM_LEADER);
		m_pForm->GetComponent("ti")->SetVisible(GetMyTeamState()==TEAM_LEADER);
		for (int i=0;i<TEAM_MAXUSER-1;i++)
		{
			m_pCompName[i]->SetVisible(false);
			m_pCompHp[i]->SetVisible(false);
			m_pCompJob[i]->SetVisible(false);
			m_pCompLv[i]->SetVisible(false);
			m_pCompD1[i]->SetVisible(false);
			m_pCompD2[i]->SetVisible(false);
			m_pCompD3[i]->SetVisible(false);
			m_pCompD4[i]->SetVisible(false);
			m_pCompSelect[i]->SetVisible(false);
			m_pCompLeader[i]->SetVisible(false);
		}

		if (m_nTeamMemberPos >= 0)
			m_pCompD4[m_nTeamMemberPos]->SetVisible(true);

		int nCount = 0;
		char szTxt[32];
		for (int i=0;i<TEAM_MAXUSER;i++)
		{
			if (m_xUser[i].nState == TEAM_LEADER)
			{
				if (m_dwLeaderUin == 0)
				{
					m_dwLeaderUin = m_xUser[i].dwUin;
				}
			}
			if (!m_xUser[i].dwUin||m_xUser[i].dwUin == pChar->m_dwUin||m_xUser[i].nState == TEAM_CREATE)
			{
				continue;
			}
			m_pCompName[nCount]->SetCaption(m_xUser[i].szName,1);
			m_pCompHp[nCount]->SetProgress(m_xUser[i].nHp,m_xUser[i].nMaxHp);
			m_pCompLv[nCount]->SetCaptionEx("%d",1,m_xUser[i].nLv);
			m_pCompName[nCount]->SetVisible(true);
			m_pCompHp[nCount]->SetVisible(true);

			sprintf(szTxt,"MB_ZHIYE%d.png",m_xUser[i].byJob);
			m_pCompJob[nCount]->SetViewSpr(szTxt);
			m_pCompJob[nCount]->SetVisible(true);

			m_pCompLv[nCount]->SetVisible(true);
			m_pCompD1[nCount]->SetVisible(true);
			m_pCompD2[nCount]->SetVisible(true);
			m_pCompD3[nCount]->SetVisible(true);
			m_pCompSelect[nCount]->SetVisible(true);
			if (m_xUser[i].nState == TEAM_LEADER)
			{
				m_pCompLeader[nCount]->SetVisible(true);
			}
			nCount++;

		}
	}
}

void TeamMenu::Open(int nEff)
{
	m_bUpdate = true;
	m_pForm->Open(nEff);
}

int TeamMenu::GetTeamCount()
{
	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if(m_xUser[i].dwUin) nCount++;
	}
	return nCount;
}

int TeamMenu::GetTeamedCount()
{
	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if(m_xUser[i].dwUin&&m_xUser[i].nState!=TEAM_CREATE) nCount++;
	}
	return nCount;
}

_TeamNet* TeamMenu::GetStudentInfo()
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if(m_xUser[i].dwUin && m_xUser[i].nLv < 45)
			return &m_xUser[i];
	}

	return NULL;
}

int	TeamMenu::GetMyTeamState()
{
	bool bIsLeader = false;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if(m_xUser[i].dwUin&&m_xUser[i].dwUin != pChar->m_dwUin&&m_xUser[i].nState == TEAM_LEADER) 
			bIsLeader = true;
	}
	if (bIsLeader)
		return 0;
	else
		return TEAM_LEADER;
}

DWORD TeamMenu::GetLeadID()
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (!m_xUser[i].dwUin)
			continue;
		
		if(m_xUser[i].nState == TEAM_LEADER)
			return m_xUser[i].dwUin;
	}
	return 0;
}

DWORD TeamMenu::GetSelectMemberID()
{
	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (!m_xUser[i].dwUin||m_xUser[i].dwUin == pChar->m_dwUin)
		{
			continue;
		}
		if (nCount == m_nTeamMemberPos)
		{
			return m_xUser[i].dwUin;
		}
		nCount++;
	}
	return 0;
}

int TeamMenu::GetSelectMemberPos()
{
	int nCount = 0;
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if (!m_xUser[i].dwUin||m_xUser[i].dwUin == pChar->m_dwUin)
		{
			continue;
		}
		if (nCount == m_nTeamMemberPos)
		{
			return i;
		}
		nCount++;
	}
	return -1;
}

bool TeamMenu::IsTeamMember( DWORD dwID )
{
	for (int i=0;i<TEAM_MAXUSER;i++)
	{
		if(m_xUser[i].dwUin && m_xUser[i].dwUin==dwID)
			return true;
	}
	return false;
}

void ClickCloseMenu(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	TeamDropMenu*	pMenu	= (TeamDropMenu*)param;

	BuildPacketEx(pPacket, USER, C_NEED_TD_REQUEST,buf, 256);
	SET_DATA(pSend, USER, C_NEED_TD_REQUEST, pPacket);
	pSend->byRtlNeed = 2;
	pSend->dwUin = pChar->m_dwUin;
	pSend->dwItemID = pMenu->m_dwItemID;
	pChar->SendPacket(pPacket);

	pMenu->m_pDropList->Delete(0);
	pMenu->Close();
}

void ClickRandItem(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

	TeamDropMenu*	pMenu	= (TeamDropMenu*)param;

	BuildPacketEx(pPacket, USER, C_NEED_TD_REQUEST,buf, 256);
	SET_DATA(pSend, USER, C_NEED_TD_REQUEST, pPacket);
	pSend->byRtlNeed = 1;
	pSend->dwUin = pChar->m_dwUin;
	pSend->dwItemID = pMenu->m_dwItemID;
	pChar->SendPacket(pPacket);

	pMenu->m_pDropList->Delete(0);
	pMenu->Close();
}

TeamDropMenu::TeamDropMenu()
{
	m_pForm = pMainInterface->GetDForm ("zuduidiaoluo");

	m_pForm->GetComponent("close")->SetEvent(ClickCloseMenu, this);
	m_pForm->GetComponent("need")->SetEvent(ClickRandItem, this);

	m_dwWaitTDNeed = 0;
	m_dwItemID = 0;
	m_pDropList = xnList::Create();
}

TeamDropMenu::~TeamDropMenu()
{
	while(m_pDropList->Count)
		m_pDropList->Delete(0);
	m_pDropList->Free();
}

void TeamDropMenu::Close()
{
	m_dwWaitTDNeed = 0;
	m_pForm->Close();
}

void TeamDropMenu::Updata()
{
	if(!m_pDropList->Count)
		return;
	if (!m_dwWaitTDNeed)
	{
		m_pForm->Open(4);
		m_dwWaitTDNeed	= xnGetTickCount();
		m_dwItemID = (DWORD)m_pDropList->Items[0];
		ItemExBase* pItem = ItemConfig::GetInstance().GetItemBase(m_dwItemID);
		if (pItem)
		{
			char	szTmp[32];
			sprintf(szTmp,"item/%d.png",pItem->nSpr);
			m_pForm->GetComponent("icon")->SetViewSpr(szTmp);
			sprintf(szTmp,"MB_PZ%d.png",pItem->nStar+1);
			m_pForm->GetComponent("icon")->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
			m_pForm->GetComponent("name")->SetCaption(pItem->szName,1);
			m_pForm->GetComponent("name")->SetSprColor(nItemColor[pItem->nStar][0],nItemColor[pItem->nStar][1],nItemColor[pItem->nStar][2]);
		}
	}

	if(xnGetTickCount() - m_dwWaitTDNeed > 20000)
	{
		BuildPacketEx(pPacket, USER, C_NEED_TD_REQUEST,buf, 256);
		SET_DATA(pSend, USER, C_NEED_TD_REQUEST, pPacket);
		pSend->byRtlNeed = 2;
		pSend->dwUin = pChar->m_dwUin;
		pSend->dwItemID = m_dwItemID;
		pChar->SendPacket(pPacket);

		m_pDropList->Delete(0);
		Close();
	}
	else
	{
		m_pForm->GetComponent("time")->SetCaptionEx("%d",1,(20000+m_dwWaitTDNeed-xnGetTickCount())/1000);
	}

}

DropShowMenu::DropShowMenu():m_dwWaitTick(0)
{
	m_pForm = pMainInterface->GetDForm ("wupingdiaoluo");
	m_pList = xnList::Create ();
}

DropShowMenu::~DropShowMenu()
{
	while(m_pList->Count)
		delete m_pList->Delete(0);
	m_pList->Free();
}

void DropShowMenu::Updata()
{
	if(!m_pList->Count)
		return;
	if (!m_dwWaitTick)
	{
		m_pForm->Open(2);
		m_dwWaitTick = xnGetTickCount();
		ItemExBase* pBase = ItemConfig::GetInstance().GetItemBase((DWORD)m_pList->Items[0]);
		if (pBase)
		{
			char	szTmp[32];
			sprintf(szTmp,"item/%d.png",pBase->nSpr);
			m_pForm->GetComponent("icon")->SetViewSpr(szTmp);
			sprintf(szTmp,"MB_PZ%d.png",pBase->nStar+1);
			m_pForm->GetComponent("icon")->SetAddSprByTag(szTmp,-3,-3,1,100,0,-1);
			m_pForm->GetComponent("name")->SetCaption(pBase->szName,1);
			m_pForm->GetComponent("name")->SetSprColor(nItemColor[pBase->nStar][0],nItemColor[pBase->nStar][1],nItemColor[pBase->nStar][2]);
		}
	}
	if(xnGetTickCount() - m_dwWaitTick > 2000)
	{
		m_dwWaitTick = 0;
		m_pList->Delete(0);
		m_pForm->Close();
	}
}