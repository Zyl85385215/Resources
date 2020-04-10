#pragma once

#include "ios_groupCmd.h"
#include "SocketManager.h"

class PacketListener
{
public:
	PacketListener();
	virtual ~PacketListener();
	virtual	bool PacketProcess(ZPacket* pPacket)	{return false;}
	void	SendPacket(ZPacket* pPacket);
	void	SendCmd(BYTE bCmdGroup, BYTE bCmd);
};


class PacketCenter
{
public:
	PacketCenter();
	~PacketCenter();

	bool		Update();
	bool		PacketProcess(ZPacket* pPacket);
	void		AddListener(PacketListener* pPL);
	void		RemoveListener(PacketListener* pPL);
	int			ConnectServer(char* szIP, int nPort);
	void		DisConnectServer();

	bool		m_bActive;
	xnList*		m_pListListen;

	DWORD		m_dwTickGameSrv;
	DWORD		m_dwTickMapSrv;

	SocketManager m_SocketManager;
};

extern	PacketCenter	g_packetCenter;