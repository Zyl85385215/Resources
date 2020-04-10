#include "MainMenu.h"
#include "FriendMenu.h"
#include "UserData.h"
#include "ios_muli_packet.h"

extern char* szJobTxt[];
char* szCountryTxt[] = {"无","联盟","部落"};
static void CBFrameFBEvent(int nPos,char* szName)  
{
	if (strncmp (szName, "n1",2) == 0)
	{
		pMainMenu->m_pFriendMenu->Select(nPos);
	}
	else if(strncmp(szName,"zengsong1",9) == 0)
	{
		
	}else if(strncmp(szName,"get1",5) == 0)
	{
		FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendMenu->m_pForm->pCFrame->GetFPTag(nPos);
		if (pObj)
		{
			BuildPacketEx(pPacket,FRIEND,C_GET_GIFT,buf,256);
			SET_DATA(pData, FRIEND, C_GET_GIFT,pPacket);
			pData->dwUin	= pObj->dwUinB;
			pChar->SendPacket(pPacket);
		}
	}
}

static void CBFrmAddFrdEvent(int nPos,char* szName) // 好友邀请
{
	if (strncmp (szName, "add1",3) == 0)
	{
		BuildPacketEx(pPacket,FRIEND,C_P2PASK,buf,256);
		SET_DATA(pData, FRIEND,C_P2PASK,pPacket);
		
		//pData->dwObj	= (DWORD)pMainMenu->m_pFriendMenu->m_pFormAdd->pCFrame->GetFPTag(nPos);
		pData->bType	= PAT_FRIEND;
		pChar->SendPacket(pPacket);
	}
}

void SearthFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	FriendMenu*	pMenu	= (FriendMenu*)param;
	pChar->SendCmd(FRIEND,C_SEARTH_FRIEND);
	//pMenu->m_pFormAdd->pCFrame->SetFPCnt(0);
}

void OpenAddFormEvent(InterfaceEvent * pEvent,void * param) //邀请好友
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	pMainMenu->m_pAddFriendMenu->Open();
}

void fun_cbDeleteFriend(void * pParam,int nVal)
{
	if (nVal == 1)
	{
		pMainMenu->PopTipMsg("删除该玩家成功");
		FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFriendMenu->m_nSelect);
		if (pObj)
		{
			BuildPacketEx(pPacket,FRIEND,C_DEL_FRIEND,buf,256);
			SET_DATA(pData, FRIEND, C_DEL_FRIEND,pPacket);
			pData->dwUin	= pObj->dwUinB;
			pChar->SendPacket(pPacket);
		}
		pMainMenu->m_pFriendMenu->m_nSelect--; 
	}
}

void DeleteFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFriendMenu->m_nSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个好友");
		return;
	}else 
		pMainMenu->PopAskMsgBox("确定删除好友？", fun_cbDeleteFriend, param);//===zjk
}
static void Return1Event(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;

}
static void Return2Event(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	FriendMenu*	pMenu	= (FriendMenu*)param;

}
void GiveFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFriendMenu->m_nSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个好友");
		return;
	}
	FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFriendMenu->m_nSelect);
	if (pObj)
	{
		BuildPacketEx(pPacket,FRIEND,C_SEND_GIFT,buf,256);
		SET_DATA(pData, FRIEND, C_SEND_GIFT,pPacket);
		pData->dwUin	= pObj->dwUinB;
		pChar->SendPacket(pPacket);
	}
}
void GetFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFriendMenu->m_nSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个好友");
		return;
	}
	FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFriendMenu->m_nSelect);
	if (pObj)
	{
		BuildPacketEx(pPacket,FRIEND,C_GET_GIFT,buf,256);
		SET_DATA(pData, FRIEND, C_GET_GIFT,pPacket);
		pData->dwUin	= pObj->dwUinB;
		pChar->SendPacket(pPacket);
	}
}
void TeamFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFriendMenu->m_nSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个好友");
		return;
	}
	FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFriendMenu->m_nSelect);
	if (pObj)
	{		
		pChar->InvitePlayer(pObj->dwUinB); 
	}
}
void ChatFriendEvent(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	if (pMainMenu->m_pFriendMenu->m_nSelect == -1)
	{
		pMainMenu->PopTipMsg("请先选择一个好友");
		return;
	}
	FriendObj* pObj = (FriendObj*)pMainMenu->m_pFriendMenu->m_pForm->pCFrame->GetFPTag(pMainMenu->m_pFriendMenu->m_nSelect);
	if (pObj)
	{
		pMainMenu->m_pChatMenu->Open();
		pMainMenu->m_pChatMenu->SetTargent(pObj->szName);
		//pChar->InvitePlayer(pObj->dwUinB);
	}
}

FriendMenu::FriendMenu():m_bUpdate(true),m_nSelect(-1),m_bGift(false)
{
	m_pForm		= pMainInterface->GetDForm ("haoyou");
//	m_pFormAdd	= pMainInterface->GetDForm("jiahaoyou");

// 	m_pFormAdd->GetComponent("chazhao")->SetEvent(SearthFriendEvent, this);
// 	m_pFormAdd->pCFrame	= new CCtrlFrame(30,330,575,350);

// 	DComponent*	pComp	= m_pFormAdd->GetComponent ("di4");
// 	pComp->SetVisible (false);
// 	DComponent*	ppComps[7];
// 	ppComps[0]		= m_pFormAdd->GetComponent ("name1");
// 	ppComps[1]		= m_pFormAdd->GetComponent ("lvl1");
// 	ppComps[2]		= m_pFormAdd->GetComponent ("add1");
// 	ppComps[3]		= m_pFormAdd->GetComponent ("z");

// 	m_pFormAdd->pCFrame->nSpace=5;
// 	m_pFormAdd->pCFrame->InitFParts (pComp,ppComps,4);
// 	//m_pForm->pCFrame->SetFPCnt (10);
// 	m_pFormAdd->pCFrame->SetEvent (CBFrmAddFrdEvent);
// 	m_pFormAdd->GetComponent("close1")->SetEvent(Return2Event,this);


	m_pForm->pCFrame	= new CCtrlFrame(40,190,876,340);

	DComponent*	pComp	= m_pForm->GetComponent ("n1");
	pComp->SetVisible (false);
	DComponent*	ppComps[10];
	ppComps[0]		= m_pForm->GetComponent ("name");
	ppComps[1]		= m_pForm->GetComponent ("job");
	ppComps[2]		= m_pForm->GetComponent ("lvl");
	ppComps[3]		= m_pForm->GetComponent ("gj");
	ppComps[4]		= m_pForm->GetComponent ("org");
	ppComps[5]		= m_pForm->GetComponent ("xz");
	ppComps[6]		= m_pForm->GetComponent ("x1");
	ppComps[7]		= m_pForm->GetComponent ("yisong");
	ppComps[8]		= m_pForm->GetComponent ("get1");
	ppComps[9]		= m_pForm->GetComponent ("yi1");

	m_pForm->pCFrame->nSpace=11;
	m_pForm->pCFrame->nDisStep	= 50;
	m_pForm->pCFrame->InitFParts (pComp,ppComps,10);
	//m_pForm->pCFrame->SetFPCnt (10);
	m_pForm->pCFrame->SetEvent (CBFrameFBEvent);

	m_pForm->GetComponent("close")->SetEvent(Return1Event);
	m_pForm->GetComponent("tj")->SetEvent(OpenAddFormEvent,this);
	m_pForm->GetComponent("del")->SetEvent(DeleteFriendEvent,this);
	m_pForm->GetComponent("zs")->SetEvent(GiveFriendEvent,this);
	m_pForm->GetComponent("lq")->SetEvent(GetFriendEvent,this);
	m_pForm->GetComponent("zd")->SetEvent(TeamFriendEvent,this);
	m_pForm->GetComponent("sl")->SetEvent(ChatFriendEvent,this);
}

FriendMenu::~FriendMenu()
{

}

void FriendMenu::Open(int nT)
{
	m_bUpdate = true;
	m_pForm->Open (nT);
}

void FriendMenu::Clear()
{
	pChar->m_pFriendList.clear();
	pChar->m_mapOnline.clear();
	m_nSelect = -1;
	m_bUpdate = true;
}

static void MsgbosReturnEvent(void* param,int nVal)
{
	if (nVal == 1)
	{
		pMainMenu->PopTipMsg("成功添加该玩家");
	}
	else
		pMainMenu->PopTipMsg("拒绝该玩家成功");
	FriendObj* pItem = (FriendObj*)param;
	BuildPacketEx(pPacket,FRIEND,C_P2PANSWER,buf,256);
	SET_DATA(pData, FRIEND, C_P2PANSWER,pPacket);
	pData->bOk	= nVal;
	pData->dwAsker	= pItem->dwUinB;
	//pData->nIndex	= pPA->nIndex;
	pData->bType	= PAT_FRIEND;
	pChar->SendPacket(pPacket);

}


void FriendMenu::Update()
{
	//if (!m_pForm->bOpened)
	//{
	//	return;
	//}
	FriendObj* pItemApply = NULL;
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
				if (bOnline)
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

					m_pForm->pCFrame->GetFPComp(i,"yisong")->SetVisible(_getRsValByType(pItem.dwUinB,pItem.nRsVal,FRIEND_RS_VAL_GIVE,true));
					m_pForm->pCFrame->GetFPComp(i,"yi1")->SetVisible(false);
					m_pForm->pCFrame->GetFPComp(i,"get1")->SetVisible(false);
					m_pForm->pCFrame->SetFPTag(i,&pItem);
					i++;
				}
			}

			if (pItem.byRsType == RS_TYPE_PRE_FRIENDEX)
			{
				pItemApply = &pItem;
			}
 		}
		for (std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.begin();itor!=pChar->m_pFriendList.end();itor++)
		{
			FriendObj& pItem	= itor->second;
			//size_t nCount = itor-pChar->m_pFriendList.begin();
			char szSpr[32] = {0};
			bool bOnline = pChar->CheckFriendOnline(pItem.dwUinB);
			if (pItem.byRsType == RS_TYPE_FRIEND)
			{
				if (!bOnline)
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

					m_pForm->pCFrame->GetFPComp(i,"yisong")->SetVisible(_getRsValByType(pItem.dwUinB,pItem.nRsVal,FRIEND_RS_VAL_GIVE,true));
					m_pForm->pCFrame->GetFPComp(i,"yi1")->SetVisible(false);
					m_pForm->pCFrame->GetFPComp(i,"get1")->SetVisible(false);
					m_pForm->pCFrame->SetFPTag(i,&pItem);
					i++;
				}
			}

			if (pItem.byRsType == RS_TYPE_PRE_FRIENDEX)
			{
				pItemApply = &pItem;
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
	if (pItemApply)
	{
		char sztxt[128];
		sprintf(sztxt,"%s申请成为你的好友,是否同意?",pItemApply->szName);
		pMainMenu->PopAskMsgBox(sztxt,MsgbosReturnEvent,pItemApply);
	}
	if(m_pForm->bOpened)
		m_pForm->pCFrame->CalcVisable();
}

 void CBAskBeFriend(void * pParam,int nVal)
 {
	 P2pAskObj* pPA	= (P2pAskObj*)pParam;
	 BuildPacketEx(pPacket,FRIEND,C_P2PANSWER,buf,256);
	 SET_DATA(pData, FRIEND, C_P2PANSWER,pPacket);
	 pData->bOk	= nVal;
	 pData->dwAsker	= pPA->dwSend;
	 pData->nIndex	= pPA->nIndex;
	 pData->bType	= pPA->bType;
	 pChar->SendPacket(pPacket);


	 delete pPA;
 }

bool FriendMenu::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != FRIEND)
		return false;

	switch(pPacket->bCmd)
	{
	case S_SEARTH_LIST:
		{
			SET_DATA(pData,FRIEND,S_SEARTH_LIST,pPacket);
// 			m_pFormAdd->pCFrame->SetFPCnt(pData->wCnt);
// 
// 			for (int i = 0; i < pData->wCnt; i++)
// 			{
// 				m_pFormAdd->pCFrame->SetFPCompCaption(i,"name1",pData->frdBase[i].szName);
// 				char	szTmp[32];
// 				sprintf(szTmp,"%d",pData->frdBase[i].byLv);
// 				m_pFormAdd->pCFrame->SetFPCompCaption(i,"lvl1",szTmp);
// 				m_pFormAdd->pCFrame->SetFPTag(i,(void*)pData->frdBase[i].dwUinB);
// 			}
			
			//pData->wCnt
		}
		return true;
	case S_P2PASK:
		{
			SET_DATA(pData,FRIEND,S_P2PASK,pPacket);

			P2pAskObj* pPA	= new P2pAskObj();
			pPA->dwSend	= pData->frd.dwUinB;
			pPA->nIndex	= pData->nIndex;
			pPA->bType	= pData->bType;
			pPA->nLvl   = pData->frd.byLv;
			strncpy(pPA->szName,pData->frd.szName,16);
			pPA->dwStartTime	= xnGetTickCount();
			pPA->dwRecv		= pChar->m_dwExp;

			char	szTmp[512];
			switch(pData->bType)
			{
			case PAT_FRIEND:
				sprintf(szTmp,"%d级的%s 邀请你加好友,确定?", pData->frd.byLv,pData->frd.szName);
				break;
			case PAT_PK:
				sprintf(szTmp,"%d级的%s 要求切磋,确定?", pData->frd.byLv,pData->frd.szName);
				break;
			}
			pMainMenu->PopAskMsgBox(szTmp,CBAskBeFriend,pPA);
		}
		return true;
	case S_FRIEND_LIST:
		{
			SET_DATA(pData,FRIEND,S_FRIEND_LIST,pPacket);
			for (int i = 0; i < pData->wCnt; i++)
			{
				FriendObj& pObj = pChar->m_pFriendList[pData->frdObj[i].dwUinB];
				memcpy(&pObj,&pData->frdObj[i],sizeof(FriendObj));
				pChar->m_mapOnline[pData->frdObj[i].dwUinB] = false;
			}
			m_bUpdate = true;
		}
		return true;
	case S_FRIEND_ONLINE_LIST:
		{
			SET_DATA(pData,FRIEND,S_FRIEND_ONLINE_LIST,pPacket);
			for (int i = 0; i < pData->nCnt; i++)
			{
				std::map<DWORD,bool>::iterator itor = pChar->m_mapOnline.find(pData->dwUin[i]);
				if (itor != pChar->m_mapOnline.end())
				{
					itor->second = true;
				}else
					pChar->m_mapOnline[pData->dwUin[i]] = true;
			}
			m_bUpdate = true;
		}
		return true;
	case S_FRIEND_DEL:
		{
			SET_DATA(pData,FRIEND,S_FRIEND_DEL,pPacket);
			std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.find(pData->dwUin);
			if (itor!=pChar->m_pFriendList.end())
			{
				pChar->m_pFriendList.erase(itor);
			}
			m_bUpdate = true;
		}
		return true;
	case S_FRIEND_VAL:
		{
			SET_DATA(pData,FRIEND,S_FRIEND_VAL,pPacket);
			std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.find(pData->frdObj.dwUinB);
			if (itor!=pChar->m_pFriendList.end())
			{
				itor->second = pData->frdObj;
			}
			bool bCanget = _getRsValByType(pData->frdObj.dwUinB,pData->frdObj.nRsVal,FRIEND_RS_VAL_GIVE,false);
			bool bBeGet = _getRsValByType(pData->frdObj.dwUinB,pData->frdObj.nRsVal,FRIEND_RS_VAL_GET,true);
			if (bCanget&&!bBeGet)
			{
				m_bGift = true;
				strncpy(m_szName,pData->frdObj.szName,16);
			}
			m_bUpdate = true;
		}
		return true;
	case S_FRIEND_ADD:
		{
			SET_DATA(pData,FRIEND,S_FRIEND_ADD,pPacket);

			FriendObj& pObj = pChar->m_pFriendList[pData->frdObj.dwUinB];
			memcpy(&pObj,&pData->frdObj,sizeof(FriendObj));
			m_bUpdate = true;

			if (pData->frdObj.byRsType == RS_TYPE_FRIEND)
			{
				char	szTmp[256];
				sprintf(szTmp,"新加好友 %s !",pObj.szName);
				pMainMenu->PopTipMsg(szTmp);
			}
			
		}
		return true;
	case S_SEND_SUITABLE:
		{
			SET_DATA(pRecv,FRIEND,S_SEND_SUITABLE,pPacket);
			while(pMainMenu->m_pAddFriendMenu->m_pSuitlist->Count)
				delete (SuitFriend*)pMainMenu->m_pAddFriendMenu->m_pSuitlist->Delete(0);
			for(int i = 0; i < 6; i ++)
			{
				if(pRecv->szInfo[i].dwUin > 0)
				{
					SuitFriend* pObj = new SuitFriend();
					memcpy(pObj,&pRecv->szInfo[i],sizeof(SuitFriend));
					pMainMenu->m_pAddFriendMenu->m_pSuitlist->Add(pObj);
				}
			}
		}
		return true;
	case S_SEND_REQUESTLIST:
		{
			SET_DATA(pRecv,FRIEND,S_SEND_REQUESTLIST,pPacket);
			if(pMainMenu->m_pFRequestMenu)
				pMainMenu->m_pFRequestMenu->AddRequest(&pRecv->xInfo);
		}
		return true;
	case S_FRIEND_ONLINE_CHG:
		{
			SET_DATA(pRecv,FRIEND,S_FRIEND_ONLINE_CHG,pPacket);
			if (pChar->m_mapOnline.size() == 0)
				return true;
			std::map<DWORD,bool>::iterator itor = pChar->m_mapOnline.find(pRecv->dwUin);
			if (itor != pChar->m_mapOnline.end())
			{
				itor->second = !itor->second;
			}else
				pChar->m_mapOnline[pRecv->dwUin] = true;

			m_bUpdate = true;
		}
		return true;
	case S_FRIEND_NAME_CHG:
		{
			SET_DATA(pRecv,FRIEND,S_FRIEND_NAME_CHG,pPacket);
			std::map<int,FriendObj>::iterator itor = pChar->m_pFriendList.find(pRecv->dwUin);
			if (itor != pChar->m_pFriendList.end())
			{
				strcpy(itor->second.szName, pRecv->szName);
			}
			m_bUpdate = true;
		}
		return true;
	}
	return false;
}

void FriendMenu::Select( int nSelect )
{
	if (nSelect==0)
	{
		m_nSelect = -1;
	}
	m_nSelect = nSelect;
	for (int i=0;i<m_pForm->pCFrame->GetFPCnt();i++)
	{
		m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(m_nSelect == i);

		FriendObj* pObj = (FriendObj*)m_pForm->pCFrame->GetFPTag(i);
		if (pObj)
		{
			bool bCanget = _getRsValByType(pObj->dwUinB,pObj->nRsVal,FRIEND_RS_VAL_GIVE,false);
			bool bBeGet = _getRsValByType(pObj->dwUinB,pObj->nRsVal,FRIEND_RS_VAL_GET,true);
			m_pForm->pCFrame->GetFPComp(i,"get1")->SetVisible(bCanget&&!bBeGet);
			m_pForm->pCFrame->GetFPComp(i,"yi1")->SetVisible(bBeGet);
		}
	}
	if (nSelect != -1)
	{
		FriendObj* pObj = (FriendObj*)m_pForm->pCFrame->GetFPTag(nSelect);
		if (pObj)
		{
			m_pForm->GetComponent("zs")->SetEnable(!_getRsValByType(pObj->dwUinB,pObj->nRsVal,FRIEND_RS_VAL_GIVE,true));
			bool bCanget = _getRsValByType(pObj->dwUinB,pObj->nRsVal,FRIEND_RS_VAL_GIVE,false);
			bool bBeGet = _getRsValByType(pObj->dwUinB,pObj->nRsVal,FRIEND_RS_VAL_GET,true);
			m_pForm->GetComponent("lq")->SetEnable(bCanget&&!bBeGet);
			int nVal = pObj->nRsVal;
		}
	}
}

bool FriendMenu::_getRsValByType( DWORD dwUin,int nVal,BYTE byType ,bool Own)
{
	if (pChar->m_dwUin>dwUin&&Own)
	{
		nVal = nVal>>16;
	}else if (pChar->m_dwUin<dwUin&&!Own)
	{
		nVal = nVal>>16;
	}
	return nVal&1<<byType;
}

static void AddFriendEvent(InterfaceEvent * pEvent,void * param){
	if(pEvent->event	!= INTERFACE_EVENT_CLICK)
		return;
	char* szName = pMainMenu->m_pAddFriendMenu->m_pForm->GetComponent("name")->GetEdit();
	if (strlen(szName)<=0&&strlen(szName)>=19)
	{
		pMainMenu->PopTipMsg("输入的字符太长或者太短");
		return;
	}
	BuildPacketEx(pPacket,FRIEND,C_P2PASK,buf,256);
	SET_DATA(pData, FRIEND,C_P2PASK,pPacket);

	memcpy(pData->szName,szName,20);
	pData->bType	= PAT_FRIEND;
	pChar->SendPacket(pPacket);
	pMainMenu->m_pAddFriendMenu->m_pForm->Close();
}

void	ClickAddSuitSingle(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	AddFriendMenu* pMenu = (AddFriendMenu*)param;
	if(pMenu)
	{
		if(pMenu->m_nCur >= pMenu->m_pSuitlist->Count)
			return;
		SuitFriend* pObj = (SuitFriend*)pMenu->m_pSuitlist->Items[pMenu->m_nCur];
		BuildPacketEx(pPacket,FRIEND,C_ASK_ADDFRIEND,buf,1024);
		SET_DATA(pSend,FRIEND,C_ASK_ADDFRIEND,pPacket);
		pSend->nId = pObj->dwUin;
		pChar->SendPacket(pPacket);
		delete (SuitFriend*)pMenu->m_pSuitlist->Delete(pMenu->m_nCur);
	}
}

void	ClickAddSuitTotal(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	AddFriendMenu* pMenu = (AddFriendMenu*)param;
	if(pMenu)
	{
		while(pMenu->m_pSuitlist->Count)
		{
			SuitFriend* pObj = (SuitFriend*)pMenu->m_pSuitlist->Delete(0);
			BuildPacketEx(pPacket,FRIEND,C_ASK_ADDFRIEND,buf,1024);
			SET_DATA(pSend,FRIEND,C_ASK_ADDFRIEND,pPacket);
			pSend->nId = pObj->dwUin;
			pChar->SendPacket(pPacket);
			delete pObj;
		}
	}
}

void SuitFriendChooseEvent(int nPos,char* pName,void* param)
{
	if(param)
	{
		AddFriendMenu* pMenu = (AddFriendMenu*)param;
		pMenu->m_nCur = nPos;
	}
}

AddFriendMenu::AddFriendMenu()
{
	m_pForm = pMainInterface->GetDForm ("haoyou2");
	m_pForm->GetComponent("name")->SetCaption("输入好友ID或姓名", 0);
	m_pForm->GetComponent("tj")->SetEvent(AddFriendEvent);
	m_pForm->GetComponent("jia")->SetEvent(ClickAddSuitSingle,this);
	m_pForm->GetComponent("quan")->SetEvent(ClickAddSuitTotal,this);
	DComponent* pCompBase = m_pForm->GetComponent("n1");
	DComponent* ppComp[6];
	ppComp[0] = m_pForm->GetComponent("name1");
	ppComp[1] = m_pForm->GetComponent("job1");
	ppComp[2] = m_pForm->GetComponent("lvl1");
	ppComp[3] = m_pForm->GetComponent("gj1");
	ppComp[4] = m_pForm->GetComponent("org");
	ppComp[5] = m_pForm->GetComponent("xz");
	m_pForm->pCFrame = new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,pCompBase->nH*7);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,6);
	m_pForm->pCFrame->nDisStep = pCompBase->nH;
	m_pForm->pCFrame->SetEventEx(SuitFriendChooseEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);

	m_pSuitlist = xnList::Create();
}

AddFriendMenu::~AddFriendMenu()
{
	if(m_pSuitlist)
	{
		while(m_pSuitlist->Count)
			delete (SuitFriend*)m_pSuitlist->Delete(0);
		m_pSuitlist->Free();
	}
}

void AddFriendMenu::Open()
{
	m_pForm->GetComponent("name")->SetEdit("");
	pChar->SendCmd(FRIEND,C_REQUEST_SUITABLE);
	m_nCur = 0;
	m_pForm->Open();
}

void	AddFriendMenu::ClearList()
{
	m_nCur = 0;
	while(m_pSuitlist->Count)
		delete (SuitFriend*)m_pSuitlist->Delete(0);
}

void	AddFriendMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		int nCnt = m_pForm->pCFrame->GetFPCnt();
		if(nCnt != m_pSuitlist->Count)
			m_pForm->pCFrame->SetFPCnt(m_pSuitlist->Count);

		for(int i = 0; i < m_pSuitlist->Count; i ++)
		{
			SuitFriend* pObj = (SuitFriend*)m_pSuitlist->Items[i];
			m_pForm->pCFrame->GetFPComp(i,"name1")->SetCaptionEx("%s",1,pObj->szName);
			m_pForm->pCFrame->GetFPComp(i,"job1")->SetCaptionEx("%s",1,szJobTxt[pObj->byJob]);
			m_pForm->pCFrame->GetFPComp(i,"lvl1")->SetCaptionEx("%d",1,pObj->byLv);
			m_pForm->pCFrame->GetFPComp(i,"gj1")->SetCaptionEx("%s",1,szCountryTxt[pObj->byCountry]);
			m_pForm->pCFrame->GetFPComp(i,"org")->SetCaptionEx("%s",1,pObj->szHomeName);
			if(i == m_nCur)
				m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(true);
			else
				m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(false);
		}
		if(m_pSuitlist->Count > 0)
			m_pForm->GetComponent("quan")->SetVisible(true);
		else
			m_pForm->GetComponent("quan")->SetVisible(false);

		if(m_nCur < m_pSuitlist->Count)
			m_pForm->GetComponent("jia")->SetVisible(true);
		else
			m_pForm->GetComponent("jia")->SetVisible(false);
	}
}

void	SuitFRquestChooseEvent(int nPos,char* pName,void* param)
{
	if(param)
	{
		FRequestMenu* pMenu = (FRequestMenu*)param;
		pMenu->m_nCur = nPos;
	}
}

void	ClickAgreeSuitButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		FRequestMenu* pMenu = (FRequestMenu*)param;
		if(pMenu->m_nCur >= pMenu->m_pList->Count)
			return;
		SuitFriend* pObj = (SuitFriend*)pMenu->m_pList->Delete(pMenu->m_nCur);
		BuildPacketEx(pPacket,FRIEND,C_DEALWHIT_SUITABLE,buf,1024);
		SET_DATA(pSend,FRIEND,C_DEALWHIT_SUITABLE,pPacket);
		pSend->nId = pObj->dwUin;
		pSend->ifOk = true;
		pChar->SendPacket(pPacket);
		delete pObj;
	}
}

void	ClickRefuseSuitButton(InterfaceEvent* pEvent,void* param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;
	if(param)
	{
		FRequestMenu* pMenu = (FRequestMenu*)param;
		if(pMenu->m_nCur >= pMenu->m_pList->Count)
			return;
		SuitFriend* pObj = (SuitFriend*)pMenu->m_pList->Delete(pMenu->m_nCur);
		BuildPacketEx(pPacket,FRIEND,C_DEALWHIT_SUITABLE,buf,1024);
		SET_DATA(pSend,FRIEND,C_DEALWHIT_SUITABLE,pPacket);
		pSend->nId = pObj->dwUin;
		pSend->ifOk = false;
		pChar->SendPacket(pPacket);
		delete pObj;
	}
}

FRequestMenu::FRequestMenu()
{
	m_pForm = pMainInterface->GetDForm("haoyousq");
	m_nCur = 0;
	m_pList = xnList::Create();
	DComponent* pCompBase = m_pForm->GetComponent("n1");
	DComponent* ppComp[7];
	ppComp[0] = m_pForm->GetComponent("name");
	ppComp[1] = m_pForm->GetComponent("job");
	ppComp[2] = m_pForm->GetComponent("lvl");
	ppComp[3] = m_pForm->GetComponent("gj");
	ppComp[4] = m_pForm->GetComponent("org");
	ppComp[5] = m_pForm->GetComponent("xz");
	ppComp[6] = m_pForm->GetComponent("x1");
	m_pForm->pCFrame = new CCtrlFrame(pCompBase->nX,pCompBase->nY,pCompBase->nW,pCompBase->nH*7);
	m_pForm->pCFrame->InitFParts(pCompBase,ppComp,7);
	m_pForm->pCFrame->nDisStep = pCompBase->nH;
	m_pForm->pCFrame->SetEventEx(SuitFRquestChooseEvent,this);
	m_pForm->pCFrame->SetFPCnt(0);

	m_pForm->GetComponent("ok")->SetEvent(ClickAgreeSuitButton,this);
	m_pForm->GetComponent("no")->SetEvent(ClickRefuseSuitButton,this);
}

FRequestMenu::~FRequestMenu()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (SuitFriend*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void	FRequestMenu::Open()
{
	m_pForm->Open();
}

void	FRequestMenu::Update()
{
	if(m_pForm && m_pForm->bOpened)
	{
		int nCnt = m_pForm->pCFrame->GetFPCnt();
		if(nCnt != m_pList->Count)
			m_pForm->pCFrame->SetFPCnt(m_pList->Count);

		for(int i = 0; i < m_pList->Count; i ++)
		{
			SuitFriend* pObj = (SuitFriend*)m_pList->Items[i];
			m_pForm->pCFrame->GetFPComp(i,"name")->SetCaptionEx("%s",1,pObj->szName);
			m_pForm->pCFrame->GetFPComp(i,"job")->SetCaptionEx("%s",1,szJobTxt[pObj->byJob]);
			m_pForm->pCFrame->GetFPComp(i,"lvl")->SetCaptionEx("%d",1,pObj->byLv);
			m_pForm->pCFrame->GetFPComp(i,"gj")->SetCaptionEx("%s",1,szCountryTxt[pObj->byCountry]);
			m_pForm->pCFrame->GetFPComp(i,"org")->SetCaptionEx("%s",1,pObj->szHomeName);
			if(i == m_nCur)
				m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(true);
			else
				m_pForm->pCFrame->GetFPComp(i,"xz")->SetVisible(false);
		}
	}
}

void	FRequestMenu::AddRequest(SuitFriend*pObj)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		SuitFriend* pCheck = (SuitFriend*)m_pList->Items[i];
		if(pObj->dwUin == pCheck->dwUin)
			return;
	}
	SuitFriend* pNew = new SuitFriend();
	memcpy(pNew,pObj,sizeof(SuitFriend));
	m_pList->Add(pNew);
	if(m_pList->Count > 100)
		delete (SuitFriend*)m_pList->Delete(0);
}

void	FRequestMenu::ClearList()
{
	m_nCur = 0;
	while(m_pList->Count)
		delete (SuitFriend*)m_pList->Delete(0);
}