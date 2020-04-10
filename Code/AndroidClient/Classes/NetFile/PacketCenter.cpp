#include "PacketCenter.h"

PacketCenter	g_packetCenter;

PacketListener::PacketListener()
{
	g_packetCenter.AddListener(this);
}

PacketListener::~PacketListener()
{
	g_packetCenter.RemoveListener(this);
}

void PacketListener::SendPacket( ZPacket* pPacket )
{
	g_packetCenter.m_SocketManager.SendPacket (pPacket);
}

void PacketListener::SendCmd( BYTE bCmdGroup, BYTE bCmd )
{
	BYTE	PacketBuf[64];
	ZPacket * pSendPacket;
	BuildPacketSize(pSendPacket, bCmdGroup, bCmd, PacketBuf, sizeof(ZPacket));
	return SendPacket(pSendPacket);

}
PacketCenter::PacketCenter()
{
	m_pListListen	= xnList::Create ();
	m_bActive	= false;
	m_dwTickMapSrv	= 0;
	m_dwTickGameSrv	= 0;
}

PacketCenter::~PacketCenter()
{
	DisConnectServer();
	m_pListListen->Free ();
}

int PacketCenter::ConnectServer( char* szIP, int nPort )
{
	m_SocketManager.Connect (szIP,nPort);
	return 1;
}

bool PacketCenter::Update()
{
	if(m_bActive == true && m_SocketManager.nConnectState <= 0)
	{
		m_bActive	= false;
		return false;
	}
	else if(m_SocketManager.nConnectState > 0)
		m_bActive	= true;
	if(m_bActive == false)
		return true;

	m_bActive	= true;

	m_SocketManager.Filp ();

	ZPacket* pPacket	= NULL;
	while(pPacket	= m_SocketManager.GetPacket ())
	{
		PacketProcess (pPacket);
		//char szTmp[256];
		//sprintf(szTmp,"%d:%d\n",pPacket->bCmdGroup,pPacket->bCmd);
		//OutputDebugString(szTmp);
		free(pPacket);
	}
	return true;
}

bool PacketCenter::PacketProcess( ZPacket* pPacket )
{
	if(!m_bActive)
		return false;
	if(pPacket->bCmdGroup == MAPCMD)
	{//地图服务器
		m_dwTickMapSrv	= xnGetTickCount();
	}
	else
	{//游戏服务器
		m_dwTickGameSrv	= xnGetTickCount();
	}
	for (int i = 0; i < m_pListListen->Count; i++)
	{
		PacketListener* pPL	= (PacketListener*)m_pListListen->Items[i];

		if(pPL->PacketProcess (pPacket))
			return true;
	}
	return false;
}

void PacketCenter::AddListener( PacketListener* pPL )
{
	m_pListListen->Add (pPL);
}

void PacketCenter::RemoveListener( PacketListener* pPL )
{
	m_pListListen->Remove (pPL);
}

void PacketCenter::DisConnectServer()
{
	m_bActive	= false;
	m_SocketManager.DisConnect ();
}