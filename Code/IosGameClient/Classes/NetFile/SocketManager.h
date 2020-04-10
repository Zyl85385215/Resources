#pragma once

#include "xnlist.h"
#include <pthread.h>
#include "cocos2d.h"
#include "ODSocket.h"
#include "GlobalDefine.h"
#include "ZNetPacket.h"
#include "random.h"


#pragma comment(lib,"pthreadVCE2.lib")

#define PACKET_MARK 0x1234
#define NAKED_PACKET_MARK 0x7777


class ODSocket;

class PacketPool
{
public:
	PacketPool();
	~PacketPool();
	xnList * m_pFrontPacketList;
	xnList * m_pBackPacketList;
	void Flip();
	void AddPacket(ZPacket * pPacket);
	ZPacket * GetNextPacket();
	pthread_mutex_t mainPacketLock;
};

class SocketManager;
class SendPacketPool
{
public:
	SendPacketPool(SocketManager* pParent);
	~SendPacketPool();
	void AddSendPacket(ZPacket * pPacket);
	void UpdateSendPacket();
	xnList * m_pSendPacketList;
	pthread_cond_t	sendPacketSign;
	pthread_mutex_t sendPacketLock;
	SocketManager* m_pParent;
};

class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	void SendPacket(ZPacket * pPacket);
	void	Filp();
	ZPacket* GetPacket();
	int Connect(char * szIp, int nPort);
	void	DisConnect();

	void PacketProcess();
	void _handleSendPacket(ZPacket * pPacket);
	ODSocket mainSocket;
	
	//BYTE bTempBuff[2048];
	BYTE		pbRemainbuf[409600];
	int			nRemainSize;
	int			nConnectState;

	PacketPool		m_PacketPool;
	SendPacketPool*	m_pSendPacketPool;
	CRandom			m_xRandom;

private:
	bool	m_bActive;
	char	m_szIp[32];
	int		m_nPort;
	bool	m_bReConnect;
};
