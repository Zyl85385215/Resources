#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"

#define MAX_NETGATE		3
//网络连接的客户信息
class ZNetSLinkCenter	: public JClientCenter
{
public:
	virtual void					OnCreateConnect(CSocketTCP* pClient)	{}
	virtual void					OnDisconnect(CSocketTCP* pClient)	{}
	virtual int						ExtractPacket(CSocketTCP* pClient,char* pRecvBuffer,int nDataLen);	
};

//跟其他服务器连接
class ZNetSvrLink
{
	friend ZNetSLinkCenter;
public:
	ZNetSvrLink();
	virtual ~ZNetSvrLink();
	void							SetSocket(CSocketTCP * sock){ m_sSocketTcp = sock;}
	void							Disconnect();
	bool							isActive();
	int								ProcessData(char* pRecvBuffer,int nDataLen);

	bool							SendPacket(ZPacket * pPacket);

	
protected:
	//收到包
	CSocketTCP*						m_sSocketTcp;
};

class ZNetCenter	: public JNetServer
{
public:
	ZNetCenter();
	~ZNetCenter();

	void					StartNetCenter();
	void					StopNetCenter();
	void					CheckServerLink();

	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

	void							RecvNetGatePacket(JNetClient* pClient,ZPacket* pPacket);
	void							RecvSvrGamePacket(ZPacket* pSvrPacket);

	void							SendPacketToNetGate(ZPacket* pPacket,DWORD	dwKey);
	void							SendPacketToGameSrv(ZPacket* pPacket,DWORD	dwUin);

	xnList*							SwitchDealNetGateList();			//2张表翻转
	xnList*							SwitchDealGameSvrList();			//2张表翻转

	ZNetSvrLink*					m_pSvrGame;
private:
	//客户端连上来的玩家列表
	JNetClient*						m_szpNetGates[MAX_NETGATE];
	//连接其他服务器的网络中心
	ZNetSLinkCenter*				m_pCCLinkSvr;

	CRITICAL_SECTION				m_xLockGameSvr;
	CRITICAL_SECTION				m_xLockNetGate;
	xnList*							m_pListRecvGameSvr;		//收包填充表
	xnList*							m_pListRecvNetGate;		//收包填充表
	xnList*							m_pListDealGameSvr;		//处理表
	xnList*							m_pListDealNetGate;		//处理表



};

extern ZNetCenter*	g_pNetCenter;