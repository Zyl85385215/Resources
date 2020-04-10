#include "stdafx.h"
#include "ZNetGamesvr.h"
#include "ServerCfg.h"
#include "ZNetPacket.h"
#include "ios_system_packet.h"
#include "ios_ac_packet.h"
#include "ios_user_packet.h"
#include "FsGameCenter.h"
#include "LuaStateManager.h"
#include "DBLogControl.h"
extern CLuaStateManager * pLuaStateManager;

ZNetCenter*	g_pNetCenter	= NULL;

ZNetSvrLink::ZNetSvrLink( )
{
	m_sSocketTcp	= NULL;
}

ZNetSvrLink::~ZNetSvrLink()
{
	Disconnect();
}

void ZNetSvrLink::Disconnect()
{
	if(m_sSocketTcp)
	{
		m_sSocketTcp->Diconnect();
		m_sSocketTcp	= NULL;
	}
}

bool ZNetSvrLink::isActive()
{
	return (m_sSocketTcp && m_sSocketTcp->IsActive());
}

int ZNetSvrLink::ProcessData( char* pRecvBuffer,int nDataLen )
{
	int dataSize = nDataLen;
	char * k = pRecvBuffer;
	while (dataSize >= sizeof(ZPacket)-1)
	{
		ZPacket * pPacket = (ZPacket *)k;
		if (pPacket->wMark != ZPACKET_MARK)
		{
			//Error Data
			if (strnicmp((char*)k, "FUCK", 4) == 0)
			{
				k += 4;
				dataSize -= 4;
			}
			else
			{
				k++;
				dataSize--;
			}
		}
		else
		{
			int packetSize = sizeof(ZPacket) -1 + pPacket->wSize;
			if (dataSize >= packetSize)
			{
				g_pNetCenter->RecvSvrPacket(pPacket);
				
				k += packetSize;
				dataSize -= packetSize;
			}
			else
				break;
		}
	}
	return (nDataLen - dataSize);
}

bool ZNetSvrLink::SendPacket( ZPacket * pPacket )
{
	if(isActive())
		return m_sSocketTcp->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
	return false;
}


int ZNetSLinkCenter::ExtractPacket( CSocketTCP* pClient,char* pRecvBuffer,int nDataLen )
{
	//if(pClient	== g_pNetCenter->m_pSvrAccount->m_sSocketTcp)
	//	return g_pNetCenter->m_pSvrAccount->ProcessData(pRecvBuffer,nDataLen);
	//if(pClient == g_pNetCenter->m_pSvrGame->m_sSocketTcp)
	//	return g_pNetCenter->m_pSvrGame->ProcessData(pRecvBuffer,nDataLen);

	return nDataLen;
}


ZNetCenter::ZNetCenter()
{
	//m_pCCLinkSvr	= new ZNetSLinkCenter();

	//m_pSvrAccount	= new ZNetSvrLink();
	//m_pSvrGame		= new ZNetSvrLink();
	m_pListRecvPacket	= xnList::Create();
	m_pListDealPacket	= xnList::Create();
	memset(m_szpNetGates,0,4*MAX_NETGATE);

	InitializeCriticalSection(&m_xRecvPacketLock);
}

ZNetCenter::~ZNetCenter()
{
	//delete m_pCCLinkSvr;

	//delete	m_pSvrAccount;
	//delete	m_pSvrGame;
	while (m_pListRecvPacket->Count)
	{
		free(m_pListRecvPacket->Delete(0));
	}
	m_pListRecvPacket->Free();
	while (m_pListDealPacket->Count)
	{
		free(m_pListDealPacket->Delete(0));
	}
	m_pListDealPacket->Free();
	DeleteCriticalSection(&m_xRecvPacketLock);
}

void ZNetCenter::StartNetCenter()
{
	m_nMSvrCnt		= MapSelCfg::GetInstance().GetMapSvrCnt();

	InitServer(ServerCfg::GetInstance().nPort,ServerCfg::GetInstance().szIP,20,true);
	if(Start() == false)
		AfxMessageBox("服务器启动失败");

	//CheckServerLink();
}

void ZNetCenter::StopNetCenter()
{
	Stop();
	//m_pCCLinkSvr->Stop();
}

void ZNetCenter::OnCreateConnect( JNetClient* pClient )
{

}

void ZNetCenter::OnDisconnect( JNetClient* pClient )
{

}

int ZNetCenter::ExtractPacket( JNetClient* pClient,char* pRecvBuffer,int nDataLen )
{
	int dataSize = nDataLen;
	char * k = pRecvBuffer;
	while (dataSize >= sizeof(ZPacket)-1)
	{
		ZPacket * pPacket = (ZPacket *)k;
		if (pPacket->wMark != ZPACKET_MARK)
		{
			//Error Data
			if (strnicmp((char*)k, "FUCK", 4) == 0)
			{
				k += 4;
				dataSize -= 4;
			}
			else
			{
				k++;
				dataSize--;
			}
		}
		else
		{
			int packetSize = sizeof(ZPacket) -1 + pPacket->wSize;
			if (dataSize >= packetSize)
			{
				RecvClientPacket(pClient,pPacket);

				k += packetSize;
				dataSize -= packetSize;
			}
			else
				break;
		}
	}
	return (nDataLen - dataSize);
}

void ZNetCenter::RecvSvrPacket( ZPacket* pSvrPacket )
{//收到其他服务器的包
	
}

ZPacket* PacketDup(ZPacket * pPacket)
{
	int memSize = pPacket->wSize + sizeof(ZPacket)-1;
	void * rp = malloc(memSize);
	if (rp != NULL)
	{
		memcpy(rp, pPacket, memSize);
		return (ZPacket*)rp;
	}
	return NULL;
}

void TCP_SendPacketToUin( DWORD dwUin, ZPacket* pPacket )
{
	UserData* pUser	= g_pGameCenter->GetPlayer(dwUin);
	if(pUser)
		g_pNetCenter->SendNGPacket(pPacket,pUser->m_dwKey);
}

void TCP_SendPacketToKey( DWORD dwKey, ZPacket* pPacket )
{
	g_pNetCenter->SendNGPacket(pPacket,dwKey);
}

void TCP_SendPacketToUinList( xnList* pList, ZPacket* pPacket )
{
	for (int i = 0; i < pList->Count; i++)
	{
		DWORD	dwUin	= (DWORD)pList->Items[i];
		UserData* pUser	= g_pGameCenter->GetPlayer(dwUin);
		if(pUser)
			g_pNetCenter->SendNGPacket(pPacket,pUser->m_dwKey);
	}
}

void TCP_SendPacketToList( xnList* pList, ZPacket* pPacket )
{

	
}

void	TCP_SendCmdToUinList(xnList* pList, BYTE bCG, BYTE bCmd)
{
	for (int i = 0; i < pList->Count; i++)
	{
		DWORD	dwUin	= (DWORD)pList->Items[i];
		UserData* pUser	= g_pGameCenter->GetPlayer(dwUin);
		if(pUser)
		{
			char	szBuf[64];
			ZPacket*	pPacket;
			BuildPacketSize(pPacket,bCG,bCmd,szBuf,0);
			g_pNetCenter->SendNGPacket(pPacket,pUser->m_dwKey);
		}
	}
}

void TCP_SendCmdToUin( DWORD dwUin, BYTE bCG, BYTE bCmd )
{
	UserData* pUser	= g_pGameCenter->GetPlayer(dwUin);
	if(pUser)
	{
		char	szBuf[64];
		ZPacket*	pPacket;
		BuildPacketSize(pPacket,bCG,bCmd,szBuf,0);
		g_pNetCenter->SendNGPacket(pPacket,pUser->m_dwKey);
	}
}

void TCP_SendCmdToKey( DWORD dwKey, BYTE bCG, BYTE bCmd )
{
	char	szBuf[64];
	ZPacket*	pPacket;
	BuildPacketSize(pPacket,bCG,bCmd,szBuf,0);
	g_pNetCenter->SendNGPacket(pPacket,dwKey);
}

void ZNetCenter::RecvClientPacket( JNetClient* pClient,ZPacket* pPacket )
{
	switch (pPacket->bCmdGroup)
	{
	case SYSTEM:
		{
			switch(pPacket->bCmd)
			{
			case NETGATE_LOGIN_ACC:
				{
					SET_DATA(pRecv,SYSTEM,NETGATE_LOGIN_ACC,pPacket);
					if(pRecv->bySubKey < MAX_NETGATE)
						m_szpNetGates[pRecv->bySubKey]	= pClient;
				}
				break;
			case MAPSERVER_LOGIN_GAMESRV:
				{
					SET_DATA(pRecv,SYSTEM,MAPSERVER_LOGIN_GAMESRV,pPacket);

					if(strcmp(pRecv->szIp,MapSelCfg::GetInstance().m_pMapSrvDefaul->szIP) == 0 && pRecv->nPort == MapSelCfg::GetInstance().m_pMapSrvDefaul->nPort)
					{
						MapSelCfg::GetInstance().m_pMapSrvDefaul->pMapSvr	= pClient;
					}
					else
					{
						MapSrvObj* pMSvr	= NULL;
						for (int m = 1; m < m_nMSvrCnt; m++)
						{
							pMSvr	= (MapSrvObj*)MapSelCfg::GetInstance().m_pList->Items[m-1];
							if(strcmp(pRecv->szIp,pMSvr->szIP) == 0 && pRecv->nPort == pMSvr->nPort)
								pMSvr->pMapSvr	= pClient;
						}
					}

				}
				break;
			}
		}
		break;
	default:
		{
			EnterCriticalSection(&m_xRecvPacketLock);
			m_pListRecvPacket->Add(PacketDup(pPacket));
			LeaveCriticalSection(&m_xRecvPacketLock);
		}
		break;
	}
}

void ZNetCenter::CheckServerLink()
{

}

void ZNetCenter::SendNGPacket( ZPacket* pNGPacket, DWORD dwKey )
{
	if(dwKey == 0)
	{
		for (int i = 0; i < MAX_NETGATE; i++)
		{
			if(m_szpNetGates[i] && m_szpNetGates[i]->IsActive())
			{
				m_szpNetGates[i]->SendData((BYTE*)pNGPacket,pNGPacket->wSize + sizeof(ZPacket)-1);
				return;
			}
		}
	}
	int bySubKey	= (dwKey>>28);
	if(bySubKey >= MAX_NETGATE || m_szpNetGates[bySubKey] == NULL || m_szpNetGates[bySubKey]->IsActive() == false)
		return;

	pNGPacket->dwKey	= dwKey;
	m_szpNetGates[bySubKey]->SendData((BYTE*)pNGPacket,pNGPacket->wSize + sizeof(ZPacket)-1);
}

bool ZNetCenter::SendMSvrPacket( ZPacket* pMSvrPacket,DWORD mapIndex,JNetClient* pClient )
{
	if(pClient == NULL)
	{
		OutMessage(msg_type_error,"MapServer Link ERROR");
		return false;
	}
	pMSvrPacket->dwKey	= mapIndex;
	pClient->SendData((BYTE*)pMSvrPacket,pMSvrPacket->wSize + sizeof(ZPacket)-1);
	return true;
}

xnList* ZNetCenter::SwitchDealPList()
{
	EnterCriticalSection(&m_xRecvPacketLock);
	xnList*	pList	= m_pListRecvPacket;
	m_pListRecvPacket	= m_pListDealPacket;
	m_pListDealPacket	= pList;
	LeaveCriticalSection(&m_xRecvPacketLock);

	return m_pListDealPacket;
}

