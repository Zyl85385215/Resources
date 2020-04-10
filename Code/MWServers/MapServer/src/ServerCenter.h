#pragma once

#include "xnThread.h"
#include "ZLogFile.h"

class MapServerCenter
{
public:
	MapServerCenter();
	~MapServerCenter();

	void		MTFireHeartBeat();
	void		MTFireDealPacket();

	void		MTInitCenter();
	void		HeartBeat();
	void		DealPacket();
	void		Update();
private:
	void		_dealClientPackets();

	xnThread*	m_pMainThread;
};

extern	MapServerCenter*	g_pMSvrCenter;
extern void OutMessage(int channel, char* lpszFormat, ...);
extern	ZLogFile* g_pLogFile;