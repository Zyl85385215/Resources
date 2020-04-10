#include "UserData.h"
#include "ClientChat.h"
#include "TextConfig.h"
#include "MainMenu.h"
#include "GlobalDefine.h"
#include "MapControl.h"


ChatCenter::ChatCenter()
{
	m_pShowList = xnList::Create();
	m_pSimMsgList = xnList::Create();
	char szName[32];
	for(int i = 0; i < MSGLIST_MAX; i++)
	{
		m_pMsgList[i] = xnList::Create();
		sprintf(szName,"chatsystemset%d",i+1);
		m_ShieldSwitch[i] = LoadBooleanFromXML(szName);
	}
	m_bifLineChg = false;
	m_pSystemMsg = xnList::Create();
	m_pWarSysMsg = xnList::Create();
	m_ifSimpleLineChg = false;
}

ChatCenter::~ChatCenter()
{
	if(m_pShowList)
	{
		while(m_pShowList->Count)
			delete (ClientShowInfo*)m_pShowList->Delete(0);
		m_pShowList->Free();
	}
	if(m_pSimMsgList)
	{
		while(m_pSimMsgList->Count)
			delete (ChatInfo*)m_pSimMsgList->Delete(0);
		m_pSimMsgList->Free();
	}
	for(int i = 0; i < MSGLIST_MAX; i ++)
	{
		if(m_pMsgList[i])
		{
			while(m_pMsgList[i]->Count)
				delete (ChatInfo*)m_pMsgList[i]->Delete(0);
			m_pMsgList[i]->Free();
		}
	}
	if(m_pSystemMsg)
	{
		while(m_pSystemMsg->Count)
			delete (SystemMsg*)m_pSystemMsg->Delete(0);
		m_pSystemMsg->Free();
	}
	if(m_pWarSysMsg)
	{
		while(m_pWarSysMsg->Count)
			delete (SystemMsg*)m_pWarSysMsg->Delete(0);
		m_pWarSysMsg->Free();
	}
}

void ChatCenter::ClearList()
{

	while(m_pShowList->Count)
		delete (ClientShowInfo*)m_pShowList->Delete(0);

	while(m_pSimMsgList->Count)
		delete (ChatInfo*)m_pSimMsgList->Delete(0);
	for(int i = 0; i < MSGLIST_MAX; i ++)
	{
		while(m_pMsgList[i]->Count)
			delete (ChatInfo*)m_pMsgList[i]->Delete(0);
	}

	while(m_pSystemMsg->Count)
		delete (SystemMsg*)m_pSystemMsg->Delete(0);
	while(m_pWarSysMsg->Count)
		delete (SystemMsg*)m_pWarSysMsg->Delete(0);
	m_bifLineChg = true;
	m_ifSimpleLineChg = true;
}

void ChatCenter::SaveSystemSet()
{
	char szName[32];
	for(int i = 0; i < MSGLIST_MAX; i++)
	{
		sprintf(szName,"chatsystemset%d",i+1);
		SaveBooleanToXML(szName,m_ShieldSwitch[i]);
	}
}

bool ChatCenter::PacketProcess(ZPacket* pPacket)
{
	if(pPacket->bCmdGroup != RPGCHAT)
		return false;
	switch(pPacket->bCmd)
	{
	case S_SEND_MESSAGE:
		{
			SET_DATA(pRecv,RPGCHAT,S_SEND_MESSAGE,pPacket);
			AddMsg(pRecv);
		}
		return true;
	case S_SEND_SIMPLEMSG:
		{
			SET_DATA(pRecv,RPGCHAT,S_SEND_SIMPLEMSG,pPacket);
			AddSimplemsg(pRecv->szMsg);
		}
		return true;
	case S_SEND_ITEMINFO:
		{
			SET_DATA(pRecv,RPGCHAT,S_SEND_ITEMINFO,pPacket);
			if(pRecv->ifSuccess)
			{
				AddShowItem(&pRecv->xInfo);
			}
			else
			{
				pMainMenu->PopTipMsg("物品信息已过期");
			}
		}
		return true;
	case S_SEND_PROCLAMATION:
		{
			SET_DATA(pRecv,RPGCHAT,S_SEND_PROCLAMATION,pPacket);
			AddSystemMsg(pRecv->szMsg,pRecv->byType,pRecv->nCnt);
		}
		return true;
	case S_P2PCHAT_RLT:
		{
			pMainMenu->PopTipMsg("该好友不在线！");
		}
		return true;
	}
	return true;
}

void ChatCenter::AddSystemMsg(char* szMsg,BYTE byType,int nCnt)
{
	SystemMsg* pSys = new SystemMsg();
	pSys->byType = byType;
	pSys->nTimes = nCnt;
	strncpy(pSys->szMsg,szMsg,512);
	if(byType == CHAT_ROOM_NULL)
	{
		m_pWarSysMsg->Add((void*)pSys);
		AddSimplemsg(szMsg);
	}
	else
		m_pSystemMsg->Add((void*)pSys);
}

void ChatCenter::AddSimplemsg(char * pMsg)
{
	ChatInfo* pNew = new ChatInfo();
	pNew->byCRType = CHAT_ROOM_NULL;
	pNew->nCnt = 0;
	memset(pNew->xInfo,0,5*sizeof(ShowItemSimpleInfo));
	RandNameConfig::GetInstance().CheckForbid(pMsg);			//屏蔽词检查，会替换成*号
	strncpy(pNew->szMsg,pMsg,512);
	memset(pNew->szSName,0,20);
	pNew->nSUin = 0;
	memset(pNew->szRName,0,20);
	pNew->nRUin = 0;
	m_pSimMsgList->Add((void*)pNew);
	if(m_pSimMsgList->Count >= 5)								//屏幕下方的显示框,显示内容不超过5行
		delete (ChatInfo*)m_pSimMsgList->Delete(0);
	m_ifSimpleLineChg = true;
	ChatInfo* pNew2 = new ChatInfo();
	memcpy(pNew2,pNew,sizeof(ChatInfo));
	m_pMsgList[MSGLIST_SYS]->Add((void*)pNew2);
	if(m_pMsgList[MSGLIST_SYS]->Count >= MAX_MSG_INPAGE)
		delete (ChatInfo*)m_pMsgList[MSGLIST_SYS]->Delete(0);
	m_bifLineChg = true;
}

void ChatCenter::AddMsg(RPGCHAT_S_SEND_MESSAGE_Data* pMsg)
{
	BYTE byPos = 0;
	switch(pMsg->byCRType)
	{
	case CHAT_ROOM_WORLD:
		byPos = MSGLIST_WORLD;
		break;
	case CHAT_ROOM_SYSTEM:
		byPos = MSGLIST_SYS;
		break;
	case CHAT_ROOM_COUNTRY:
		byPos = MSGLIST_COUNTRY;
		break;
	case CHAT_ROOM_MAP:
		{
			byPos = MSGLIST_MAP;
// 			MapAIObj* pObj = g_pMapCenter->GetMapObj(MAPOBJ_PLAYER,pMsg->nSUin);
// 			if(pObj)
// 			{
// 				pObj->SetTalkInfo(pMsg->szMsg);
// 			}
		}
		break;
	case CHAT_ROOM_HOME:
		byPos = MSGLIST_HOME;
		break;
	case CHAT_ROOM_TEAM:
		byPos = MSGLIST_TEAM;
		break;
	case CHAT_ROOM_P2P:
		byPos = MSGLIST_P2P;
		break;
	}
	if(m_ShieldSwitch[byPos] &&pMsg->byCRType != CHAT_ROOM_SYSTEM)			// 消息屏蔽
		return;
	ChatInfo* pNew = new ChatInfo();
	pNew->byCRType = pMsg->byCRType;
	pNew->nCnt = pMsg->nCnt;
	memcpy(pNew->xInfo,pMsg->xInfo,5*sizeof(ShowItemSimpleInfo));
	RandNameConfig::GetInstance().CheckForbid(pMsg->szMsg);			//屏蔽词检查，会替换成*号
	strncpy(pNew->szMsg,pMsg->szMsg,512);
	memcpy(pNew->szSName,pMsg->szSName,20);
	pNew->nSUin = pMsg->nSUin;
	pNew->bySV = pMsg->bySV;
	memcpy(pNew->szRName,pMsg->szRName,20);
	pNew->nRUin = pMsg->nRUin;
	pNew->byRV = pMsg->byRV;
	m_pMsgList[MSGLIST_TOTAL]->Add((void*)pNew);
	if(m_pMsgList[MSGLIST_TOTAL]->Count >= MAX_MSG_INPAGE)
		delete (ChatInfo*)m_pMsgList[MSGLIST_TOTAL]->Delete(0);
	if(byPos == MSGLIST_MAP)
	{
		ChatInfo* pTempNew = new ChatInfo();
		memcpy(pTempNew,pNew,sizeof(ChatInfo));
		MapAIObj* pObj = g_pMapCenter->GetMapObj(MAPOBJ_PLAYER,pMsg->nSUin);
		if(pObj)
		{
			pObj->SetTalkInfo(pTempNew);
		}
		delete pTempNew;
	}
	ChatInfo* pNew2 = new ChatInfo();
	memcpy(pNew2,pNew,sizeof(ChatInfo));
	m_pMsgList[byPos]->Add((void*)pNew2);
	if(m_pMsgList[byPos]->Count >= MAX_MSG_INPAGE)
		delete (ChatInfo*)m_pMsgList[byPos]->Delete(0);
	ChatInfo* pNew3 = new ChatInfo();
	memcpy(pNew3,pNew,sizeof(ChatInfo));
	m_pSimMsgList->Add((void*)pNew3);
	if(m_pSimMsgList->Count >= 5)								//屏幕下方的显示框,显示内容不超过5行
		delete (ChatInfo*)m_pSimMsgList->Delete(0);
	m_bifLineChg = true;
	m_ifSimpleLineChg = true;
}

void ChatCenter::AddShowItem(ShowItemInfo* pInfo)
{
	ClientShowInfo* pNew = new ClientShowInfo();
	pNew->dwShowid = pInfo->showID;
	pNew->pObj = pChar->m_pItemCenter->CreateItemByData(pInfo->szData,pInfo->nSize);
	m_pShowList->Add((void*)pNew);
	if(pNew->pObj)
	{
		if (pNew->pObj->pItemBase->byType == ITEMTYPE_EQUIP)
		{
			pMainMenu->m_pEquipInfoMenu->Open(pNew->pObj,FORM_NULL);
		}else{
			pMainMenu->m_pItemInfoMenu->Open(pNew->pObj,FORM_NULL);
		}
	}
	if(m_pShowList->Count >= MAX_ITEM_SAVE)
		delete (ClientShowInfo*)m_pShowList->Delete(0);
}

ClientShowInfo* ChatCenter::GetShowInfo(int nID)
{
	for(int i = 0; i < m_pShowList->Count; i ++)
	{
		ClientShowInfo* pInfo = (ClientShowInfo*)m_pShowList->Items[i];
		if(pInfo->dwShowid == nID)
			return pInfo;
	}
	return NULL;
}

void ChatCenter::RequestForItemInfo(int nID)
{
	BuildPacketEx(pPacket,RPGCHAT,C_REQUEST_ITEMINFO,buf,1024);
	SET_DATA(pData,RPGCHAT,C_REQUEST_ITEMINFO,pPacket);
	pData->nID = nID;
	SendPacket(pPacket);
}

void ChatCenter::SendMsg(BYTE byType,char* pMsg,int* szId,int nCnt)
{
	if(pMsg == NULL || strlen(pMsg) == 0)
		return;
	BuildPacketEx(pPacket,RPGCHAT,C_CHAT_MSG,buf,1024);
	SET_DATA(pSend,RPGCHAT,C_CHAT_MSG,pPacket);
	pSend->byCRType = byType;
	strncpy(pSend->szMsg,pMsg,strlen(pMsg));
	pSend->nCnt = nCnt > 5? 5: nCnt;
	for(int i = 0; i < pSend->nCnt;i ++)
	{
		pSend->szID[i] = szId[i];
	}
	pPacket->wSize += strlen(pMsg);
	SendPacket(pPacket);
}