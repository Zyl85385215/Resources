#include "stdafx.h"
#include "ChatCenter.h"
#include "ios_user_packet.h"
#include "ZNetGamesvr.h"
#include "UserData.h"
#include "FsGameCenter.h"
#include "ZLogFile.h"

CreateSingleton(ChatCenter);


ChatRoom::ChatRoom(BYTE bT, int nKey)
{
	pList		= xnList::Create();
	byCRType	= bT;
	nKeyVal		= nKey;
}

ChatRoom::~ChatRoom()
{
	pList->Free();
}

bool ChatRoom::CheckMember(DWORD dwuin)
{
	for (int i=0;i<pList->Count;i++)
	{
		DWORD dwUin = (DWORD)pList->Items[i];
		if(dwUin == dwuin)
			return true;
	}
	return false;
}

void ChatRoom::AddMsg(char * szMsg,ShowItemSimpleInfo* szInfo,int nCnt,UserData* pUser1,UserData* pUser2)
{
	ZPacket* pSend = NULL;
	char buffer[1024] = {0};
	int	nSize = strlen(szMsg);

	BuildPacketSize(pSend,RPGCHAT,S_SEND_MESSAGE,buffer,nSize+sizeof(RPGCHAT_S_SEND_MESSAGE_Data));
	SET_DATA(p, RPGCHAT, S_SEND_MESSAGE, pSend);
	strcpy(p->szMsg,szMsg);
	if(pUser1)
	{
		memcpy(p->szSName,pUser1->m_szNick,20);
		p->nSUin = pUser1->m_dwUin;
		p->bySV = pUser1->m_pVIPCenter->GetChatVal();
	}
	else
	{
		memset(p->szSName,0,20);
		p->nSUin = 0;
		p->bySV = 0;
	}
	if(pUser2)
	{
		memcpy(p->szRName,pUser2->m_szNick,20);
		p->nRUin = pUser2->m_dwUin;
		p->byRV = pUser2->m_pVIPCenter->GetChatVal();
	}
	else
	{
		memset(p->szRName,0,20);
		p->nRUin = 0;
		p->byRV = 0;
	}
	p->byCRType = byCRType;
	p->nKeyVal = nKeyVal;
	p->nCnt = nCnt;
	memcpy(p->xInfo,szInfo,sizeof(ShowItemSimpleInfo)*5);
	if(byCRType == CHAT_ROOM_P2P)
	{
		if(pUser1 == NULL || pUser2 == NULL)
		{
			if (pUser1)
			{
				TCP_SendCmdToUin(pUser1->m_dwUin,RPGCHAT,S_P2PCHAT_RLT);
			}
			return;
		}
		if(pUser1)
			TCP_SendPacketToUin(pUser1->m_dwUin,pSend);
		if(pUser2)
			TCP_SendPacketToUin(pUser2->m_dwUin,pSend);
	}
	else
	{
		for (int i=0;i<pList->Count;i++)
		{
			DWORD dwUin = (DWORD)pList->Items[i];

			TCP_SendPacketToUin(dwUin,pSend);
		}
	}
	
	
}

void ChatRoom::AddP2PMsg( DWORD dwObj, char* szTitle, char* szMsg )
{
	
}

ChatCenter::ChatCenter()
{
	m_pList		= xnList::Create();
	m_pShowList = xnList::Create();

	m_dwNextID	= 1000;
}

ChatCenter::~ChatCenter()
{
	while(m_pList->Count)
		delete (ChatRoom*)m_pList->Delete(0);
	m_pList->Free();
	while(m_pShowList->Count)
		delete (ShowItemInfo*)m_pShowList->Delete(0);
	m_pShowList->Free();
}

ChatRoom* ChatCenter::RegChatObj(BYTE byCRT, int nKeyVal,UserData* pUser )
{
	ChatRoom* pRoom = _getChatRoom(byCRT,nKeyVal);
	if (!pRoom)
	{
		pRoom = new ChatRoom(byCRT,nKeyVal);
		m_pList->Add(pRoom);
	}
	if(pRoom)
	{
		for (int i=0;i<pRoom->pList->Count;i++)
		{
			DWORD uin = (DWORD)pRoom->pList->Items[i];
			if (uin == pUser->m_dwUin)
				return pRoom;
		}
		pRoom->pList->Add((void*)pUser->m_dwUin);
		pUser->m_pChatRoom[byCRT] = pRoom;
	}
	return pRoom;
}

void ChatCenter::UnRegChatObj( ChatRoom* pRoom,UserData* pUser)
{
	if (pRoom)
	{
		for (int i=0;i<pRoom->pList->Count;i++)
		{
			DWORD uin = (DWORD)pRoom->pList->Items[i];
			if (uin == pUser->m_dwUin)
			{
				pRoom->pList->Delete(i);
				pUser->m_pChatRoom[pRoom->byCRType] = NULL;
				break;
			}
		}
	}
}

ChatRoom* ChatCenter::_getChatRoom( BYTE byCRT, int nKeyVal )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		ChatRoom* pCRoom	= (ChatRoom*)m_pList->Items[i];
		if(pCRoom->byCRType	== byCRT && pCRoom->nKeyVal	== nKeyVal)
			return pCRoom;
	}
	return NULL;
}

char*	ForbidFace(char* szMsg,OUT char* p)
{
	std::string strCheck(szMsg);
	int nPos = strCheck.find("/#",0);
	while(nPos != std::string::npos)
	{
		std::string strTemp = strCheck.substr(nPos+2,4);
		if(strTemp.compare("!WP$") == 0)
		{
			nPos = strCheck.find("/#",nPos+6);
		}
		else
		{
			strCheck.erase(nPos,6);
			nPos = strCheck.find("/#",nPos);
		}
	}
	strcpy(p,strCheck.c_str());
	strCheck.clear();
	return p;
}

char*	CheckFace(char* szMsg,OUT char* p)
{
	std::string strCheck(szMsg);
	int nPos = strCheck.find("/#",0);
	while(nPos != std::string::npos)
	{
		if(nPos+6 > strCheck.size())
			break;

		strCheck.insert(nPos+6," ");
		nPos = strCheck.find("/#",nPos+7);
	}
	strcpy(p,strCheck.c_str());
	strCheck.clear();
	return p;
}

bool ChatCenter::AddMsg(ChatRoom* pRoom,UserData* pSendUser,DWORD dwRecvUin,char* szLMsg,int* szID,int nCnt)
{
	if(pRoom == NULL)
		return false;
	if(strlen(szLMsg) == 0)
		return false;
	DWORD dwNow = GetTickCount();
	UserData* pRecvData = NULL;
	if(szLMsg[0] == '/' && szLMsg[1] == '/')
	{
		char* p1 = strstr(szLMsg,"//:");		
		if(p1)
		{
			char* p2 = szLMsg+2;
			szLMsg = p1+3;
			*p1 = '\0';
			pRecvData = g_pGameCenter->GetPlayerByname(p2);
			if(pRecvData == NULL)
				pRecvData = g_pGameCenter->GetPlayer(atoi(p2));
		}
	}
	char szMsg[512];
	if(CHAT_ROOM_MAP == pRoom->byCRType)
		ForbidFace(szLMsg,szMsg);
	else
		CheckFace(szLMsg,szMsg);

	if(strlen(szMsg) > 512)
		szMsg[511] = '\0';
	if(pSendUser != NULL)
	{
		pSendUser->m_pUserRecord->AddRecord(UA_SEND,UO_MESSAGE,pRoom->byCRType,1);
	}
	ShowItemSimpleInfo szSimpInfo[5];
	for(int i = 0; i < nCnt && i < 5; i ++)
	{
		ItemObj* pItem = pSendUser->m_pItemCenter->GetItemByIndex(szID[i]);
		if(pItem == NULL)
			pItem = pSendUser->m_pItemCenter->GetEquipByIndex(szID[i]);
		if(pItem)
		{
			ShowItemInfo* pInfo = new ShowItemInfo();
			pItem->GetByteData(pInfo->szData);
			pInfo->nSize = pItem->GetByteSize();
			pInfo->dwStartTick = dwNow;
			pInfo->showID = m_dwNextID++;
			m_pShowList->Add((void*)pInfo);
			szSimpInfo[i].nItemType = pItem->m_pBase->nTypeID;
			szSimpInfo[i].showID = pInfo->showID;
		}
	}
	pRoom->AddMsg(szMsg,szSimpInfo,nCnt,pSendUser,pRecvData);
	if(pRoom->byCRType == CHAT_ROOM_SYSTEM || pRoom->byCRType == CHAT_ROOM_WORLD || pRoom->byCRType == CHAT_ROOM_COUNTRY || pRoom->byCRType == CHAT_ROOM_HOME)	//聊天日志
	{
		int nUin = 0;
		if(pSendUser)
			nUin = pSendUser->m_dwUin;
		std::string strTemp("<聊天发言>");
		char szTemp[1024];
		sprintf(szTemp,"<频道:%d><发言人:%d><内容:%s>",pRoom->byCRType,nUin,szMsg);
		strTemp.append(szTemp);
		for(int k = 0; k < nCnt; k ++)
		{
			sprintf(szTemp,"<item:%d>",szSimpInfo[k].nItemType);
			strTemp.append(szTemp);
		}
		g_pLogFile->OutPut(strTemp.c_str());
	}
	return true;
}

bool ChatCenter::AddMsg(BYTE byCRT,int nKeyVal,UserData* pSendUser,DWORD dwRecvUin,char* szMsg,int* szID,int nCnt)
{
	ChatRoom* pRoom = _getChatRoom(byCRT,nKeyVal);
	if(pRoom == NULL)
		return false;
	return AddMsg(pRoom,pSendUser,dwRecvUin,szMsg,szID,nCnt);
}

void ChatCenter::Update()
{
	int nNowTick = GetTickCount();
	for(int i = m_pShowList->Count - 1; i >= 0; i --)
	{
		ShowItemInfo* pItem = (ShowItemInfo*)m_pShowList->Items[i];
		if((nNowTick-pItem->dwStartTick) > 3*60*1000)		//物品信息保存30分钟
			delete m_pShowList->Delete(i);
	}
}

void ChatCenter::SendRoomMsg(char* szMsg,ChatRoom* pRoom)
{
	if(pRoom == NULL)
		return;
	AddMsg(pRoom,NULL,0,szMsg);
}

void ChatCenter::SendSystemMsg(char* szMsg,BYTE byType,int nid,int nCnt,bool ifPop)
{
	ChatRoom* pRoom = NULL;
	if(byType == CHAT_ROOM_NULL)
	{
		pRoom = _getChatRoom(CHAT_ROOM_SYSTEM,0);
	}
	else
	{
		pRoom = _getChatRoom(byType,nid);
		if(pRoom == NULL)
			return;
		AddMsg(pRoom,NULL,0,szMsg);
	}
	if(ifPop)
	{
		BuildPacketEx(pPacket,RPGCHAT,S_SEND_PROCLAMATION,buf,1024);
		SET_DATA(pData,RPGCHAT,S_SEND_PROCLAMATION,pPacket);
		pData->byType = byType;
		pData->nCnt = nCnt;
		strcpy(pData->szMsg,szMsg);
		pPacket->wSize += strlen(szMsg);
		for(int i = 0; i < pRoom->pList->Count; i ++)
		{
			TCP_SendPacketToUin((DWORD)pRoom->pList->Items[i],pPacket);
		}
	}
}

void ChatCenter::GetItemInfo(UserData* pUser,int nID)
{
	for(int i = 0; i < m_pShowList->Count; i ++)
	{
		ShowItemInfo* pInfo = (ShowItemInfo*)m_pShowList->Items[i];
		if(nID == pInfo->showID)
		{
			BuildPacketEx(pPacket,RPGCHAT,S_SEND_ITEMINFO,buf,1024);
			SET_DATA(pData,RPGCHAT,S_SEND_ITEMINFO,pPacket);
			pData->ifSuccess = true;
			memcpy(&pData->xInfo,pInfo,sizeof(ShowItemInfo));
			TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
			return;
		}
	}
	BuildPacketEx(pPacket,RPGCHAT,S_SEND_ITEMINFO,buf,1024);
	SET_DATA(pData,RPGCHAT,S_SEND_ITEMINFO,pPacket);
	pData->ifSuccess = false;
	TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
}