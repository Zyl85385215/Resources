#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"
#include <map>
#include "HttpCenter.h"
#include "ZLogFile.h"

enum
{
	LOCK_SVR_ACC,	//账号锁定,不能登录
	LOCK_SVR_CHAT,	//不能与其他玩家交互
	LOCK_SVR_PK,	//不能参与PK

	LOCK_SVR_MAX,
};

#define MAX_NETGATE		3
#define MAX_GAMESVR		3

//游戏服务器和网关连接进来, 可能多个
class ZNetCenter	: public JNetServer
{
public:
	ZNetCenter();
	~ZNetCenter();

	void					StartNetCenter();
	
	void					StopNetCenter();

	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

	inline void							RecvClientPacket(JNetClient* pClient,ZPacket* pPacket);

	void							SendNGPacket(ZPacket* pPacket);

	JNetClient*						m_szpNetGates[MAX_NETGATE];
	JNetClient*						m_szpGameSvr[MAX_GAMESVR];

private:
	inline	void							_sendPacketToClient(JNetClient* pClient, ZPacket* pPacket);
	//客户端连上来的玩家列表
	//ZNetClientCenter*				m_pNetClients;

};

class ZHttpCenter	: public JNetServer
{
public:
	ZHttpCenter();
	~ZHttpCenter();

	void					StartHttpCenter();
	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

	HttpCenter*				m_pHttpCenter;
};

extern ZNetCenter*	g_pNetCenter;
extern ZHttpCenter*	g_pHttpCenter;
extern ZLogFile*	g_pLogFile;