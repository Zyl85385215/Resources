#include "stdafx.h"
#include "ZNetServer.h"
#include "MapServerCfg.h"
#include "ZNetPacket.h"
#include "ios_system_packet.h"

ZNetCenter*	g_pNetCenter	= NULL;

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

ZNetSvrLink::ZNetSvrLink()
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
				g_pNetCenter->RecvSvrGamePacket(pPacket);
				
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
	if(pClient == g_pNetCenter->m_pSvrGame->m_sSocketTcp)
		return g_pNetCenter->m_pSvrGame->ProcessData(pRecvBuffer,nDataLen);

	return nDataLen;
}

ZNetCenter::ZNetCenter()
{
	m_pCCLinkSvr	= new ZNetSLinkCenter();
	memset(m_szpNetGates,0,MAX_NETGATE*4);
	m_pSvrGame		= new ZNetSvrLink();


	m_pListRecvGameSvr	= xnList::Create();
	m_pListRecvNetGate	= xnList::Create();
	m_pListDealGameSvr	= xnList::Create();
	m_pListDealNetGate	= xnList::Create();
	InitializeCriticalSection(&m_xLockGameSvr);
	InitializeCriticalSection(&m_xLockNetGate);
}

ZNetCenter::~ZNetCenter()
{
	delete m_pCCLinkSvr;

	delete	m_pSvrGame;

	m_pListRecvGameSvr->Free();
	m_pListRecvNetGate->Free();
	m_pListDealGameSvr->Free();
	m_pListDealNetGate->Free();

	DeleteCriticalSection(&m_xLockGameSvr);
	DeleteCriticalSection(&m_xLockNetGate);
}

void ZNetCenter::StartNetCenter()
{
	InitServer(MapServerCfg::GetInstance().nPort,MapServerCfg::GetInstance().szIP,10,true);
	m_pCCLinkSvr->InitCenter(10);
	m_pCCLinkSvr->Start();
	if(Start() == false)
		AfxMessageBox("服务器启动失败");
}

void ZNetCenter::StopNetCenter()
{
	Stop();
	m_pCCLinkSvr->Stop();
}

void ZNetCenter::OnCreateConnect( JNetClient* pClient )
{//网关服务器连接成功
	//m_pNetGate	= pClient;
	//BuildPacketEx(pPacket,ACCOUNT,INIT_CLIENT,buf, 256);
	//SET_DATA(pSend, ACCOUNT, INIT_CLIENT, pPacket);
	//pSend->dwKey	= dwKey;
	//pClient->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
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
				RecvNetGatePacket(pClient,pPacket);

				k += packetSize;
				dataSize -= packetSize;
			}
			else
				break;
		}
	}
	return (nDataLen - dataSize);
}

void ZNetCenter::RecvSvrGamePacket( ZPacket* pSvrPacket )
{//收到游戏服务器的包,处理  key是mapindex
	EnterCriticalSection(&m_xLockGameSvr);
	m_pListRecvGameSvr->Add(PacketDup(pSvrPacket));
	LeaveCriticalSection(&m_xLockGameSvr);
	//if(pSvrPacket->bCmdGroup == ACCOUNT)
	//{
	//	switch(pSvrPacket->bCmd)
	//	{
	//	case LOGIN_RLT:
	//		{
	//			SET_DATA(pAccData, ACCOUNT, LOGIN_RLT, pSvrPacket);
	//			if(pAccData->byErrorCode == LOGIN_SUCCESSFUL)
	//			{
	//				//检查多重登陆
	//				JNetClient* pOldNC	= m_pNetClients->GetByAccUin(pAccData->nAccUin);
	//				if(pOldNC && pOldNC != pClient)
	//				{//连接中断, ?通知?
	//					m_pNetClients->RemoveClient(pOldNC);
	//				}

	//				//赋值连接对象的账号
	//				ZNetCData* pND	= (ZNetCData*)pClient->GetData();
	//				pND->dwAccount	= pAccData->nAccUin;
	//			}
	//		}
	//		break;
	//	}
	//}
	//_sendPacketToNetGate(m_pNetGate,pSvrPacket);
}

void ZNetCenter::RecvNetGatePacket( JNetClient* pClient,ZPacket* pPacket )
{//key是KEY
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
			}

		}
		break;
	default:
		EnterCriticalSection(&m_xLockNetGate);
		m_pListRecvNetGate->Add(PacketDup(pPacket));
		LeaveCriticalSection(&m_xLockNetGate);
		break;
	}
}

void ZNetCenter::SendPacketToGameSrv(ZPacket* pPacket,DWORD dwUin)
{
	pPacket->dwKey	= dwUin;
	m_pSvrGame->SendPacket(pPacket);
}

void ZNetCenter::SendPacketToNetGate( ZPacket* pPacket,DWORD	dwKey )
{
	int bySubKey	= dwKey>>28;
	if(bySubKey >= MAX_NETGATE || m_szpNetGates[bySubKey] == NULL || m_szpNetGates[bySubKey]->IsActive() == false)
		return;

	pPacket->dwKey	= dwKey;
	m_szpNetGates[bySubKey]->SendData((BYTE*)pPacket,pPacket->wSize + sizeof(ZPacket)-1);
}

void ZNetCenter::CheckServerLink()
{
	if(m_pSvrGame->isActive() == false)
	{
		CSocketTCP* pSockGame	= m_pCCLinkSvr->InitConnect(MapServerCfg::GetInstance().szGameIP,MapServerCfg::GetInstance().nGamePort);


		if(pSockGame)
		{
			m_pSvrGame->SetSocket(pSockGame);
			//认证连接信息
			BuildPacketEx(pPacket,SYSTEM,MAPSERVER_LOGIN_GAMESRV,buf, 256);
			SET_DATA(pSend,SYSTEM,MAPSERVER_LOGIN_GAMESRV,pPacket );
			strcpy(pSend->szIp,MapServerCfg::GetInstance().szIP);
			pSend->nPort		= MapServerCfg::GetInstance().nPort;
			m_pSvrGame->SendPacket(pPacket);
		}
	}
}

xnList* ZNetCenter::SwitchDealNetGateList()
{
	EnterCriticalSection(&m_xLockNetGate);
	xnList*	pList	= m_pListRecvNetGate;
	m_pListRecvNetGate	= m_pListDealNetGate;
	m_pListDealNetGate	= pList;
	LeaveCriticalSection(&m_xLockNetGate);

	return m_pListDealNetGate;
}

xnList* ZNetCenter::SwitchDealGameSvrList()
{
	EnterCriticalSection(&m_xLockGameSvr);
	xnList*	pList	= m_pListRecvGameSvr;
	m_pListRecvGameSvr	= m_pListDealGameSvr;
	m_pListDealGameSvr	= pList;
	LeaveCriticalSection(&m_xLockGameSvr);

	return m_pListDealGameSvr;
}

