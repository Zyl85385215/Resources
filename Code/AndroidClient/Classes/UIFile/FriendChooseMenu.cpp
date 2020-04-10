#include "MainMenu.h"
#include "FriendChooseMenu.h"
#include "UserData.h"
#include "ios_muli_packet.h"

extern char* szJobTxt[];
extern char* szCountryTxt[];
static void CBFrameFBEvent(int nPos,char* szName){
	if (strncmp (szName, "n1",2) == 0)
	{
		pMainMenu->m_pFriendChooseMenu->Select(nPos);
	}
}
static void WriteMailEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFriendChooseMenu->m_nSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个好友");
		return;
	}
	FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendChooseMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFriendChooseMenu->m_nSelect);
	if (pObj)
	{
		if (!pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->bOpened)
		{
			pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->Open();
			pMainMenu->m_pMailMenu->ResetWriteMenu();
		}
		
		pMainMenu->m_pMailMenu->m_pFormMail[MAILMENU_WRITEMAIL]->GetComponent("name")->SetEdit(pObj->szName);
	}
	pMainMenu->m_pFriendChooseMenu->m_pForm->Close();
}
FriendChooseMenu::FriendChooseMenu():m_bUpdate(true),m_nSelect(-1)
{
	m_pForm		= pMainInterface->GetDForm ("youjianhy");
	m_pForm->bZorde = 150;

	m_pForm->pCFrame	= new CCtrlFrame(40,190,876,340);

	DComponent*	pComp	= m_pForm->GetComponent ("n1");
	pComp->SetVisible (false);
	DComponent*	ppComps[7];
	ppComps[0]		= m_pForm->GetComponent ("name");
	ppComps[1]		= m_pForm->GetComponent ("job");
	ppComps[2]		= m_pForm->GetComponent ("lvl");
	ppComps[3]		= m_pForm->GetComponent ("gj");
	ppComps[4]		= m_pForm->GetComponent ("org");
	ppComps[5]		= m_pForm->GetComponent ("xz");
	ppComps[6]		= m_pForm->GetComponent ("x1");

	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep	= 50;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,7);
	//m_pForm->pCFrame->SetFPCnt (10);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);

	m_pForm->GetComponent("xx")->SetEvent(WriteMailEvent);
}

FriendChooseMenu::~FriendChooseMenu()
{

}

void FriendChooseMenu::Open()
{
	m_bUpdate = true;
	m_pForm->Open ();
}



void FriendChooseMenu::Update()
{
	//if (!m_pForm->bOpened)
	//{
	//	return;
	//}
	if (m_bUpdate)
	{
		m_bUpdate = false;
		int nCount = 0;
		for (std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.begin();itor!=pChar->m_pFriendList.end();itor++)
		{
			FriendObj& pItem	= itor->second;
			if (pItem.byRsType == RS_TYPE_FRIEND)
			{
				nCount++;
			}
		}
		m_pForm->pCFrame->SetFPCnt (nCount);
		int i=0;
 		for (std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.begin();itor!=pChar->m_pFriendList.end();itor++)
 		{
 			FriendObj& pItem	= itor->second;
			//size_t nCount = itor-pChar->m_pFriendList.begin();
 			char szSpr[32] = {0};
			bool bOnline = pChar->CheckFriendOnline(pItem.dwUinB);
			if (pItem.byRsType == RS_TYPE_FRIEND)
			{
				m_pForm->pCFrame->GetFPComp(i,"name")->SetCaption(pItem.szName,1);
				m_pForm->pCFrame->GetFPComp(i,"name")->SetSprColor(bOnline?255:125,bOnline?255:125,bOnline?255:125);
				m_pForm->pCFrame->GetFPComp(i,"lvl")->SetCaptionEx("%d",1,pItem.byLv);
				m_pForm->pCFrame->GetFPComp(i,"lvl")->SetSprColor(bOnline?255:125,bOnline?255:125,bOnline?255:125);
				m_pForm->pCFrame->GetFPComp(i,"job")->SetCaptionEx("%s",1,szJobTxt[pItem.byJob]);
				m_pForm->pCFrame->GetFPComp(i,"job")->SetSprColor(bOnline?255:125,bOnline?255:125,bOnline?255:125);
				m_pForm->pCFrame->GetFPComp(i,"gj")->SetCaptionEx("%s",1,szCountryTxt[pItem.byCountry]);
				m_pForm->pCFrame->GetFPComp(i,"gj")->SetSprColor(bOnline?255:125,bOnline?255:125,bOnline?255:125);
				m_pForm->pCFrame->GetFPComp(i,"org")->SetCaptionEx("%s",1,pItem.szHomeName);
				m_pForm->pCFrame->GetFPComp(i,"org")->SetSprColor(bOnline?255:125,bOnline?255:125,bOnline?255:125);
				m_pForm->pCFrame->SetFPTag(i,&pItem);
				i++;
			}

 		}
		if (nCount&&m_nSelect == -1)
		{
			Select(0);
		}else if (nCount)
		{
			Select(m_nSelect);
		}
	}
}

void FriendChooseMenu::Select( int nSelect )
{
	m_nSelect = nSelect;
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++)
	{
		m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(m_nSelect == i);
	}
}