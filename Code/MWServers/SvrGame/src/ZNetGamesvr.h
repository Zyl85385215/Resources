#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"
#include <map>

enum
{
	LOCK_SVR_ACC,	//账号锁定,不能登录
	LOCK_SVR_CHAT,	//不能与其他玩家交互
	LOCK_SVR_PK,	//不能参与PK

	LOCK_SVR_MAX,
};

#define MAX_NETGATE		3


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
	//void							_recvPacket(SvrPacket * pPacket);
	CSocketTCP*						m_sSocketTcp;
};

class ZNetCenter	: public JNetServer
{
public:
	ZNetCenter();
	~ZNetCenter();

	void					StartNetCenter();
	void					StopNetCenter();
	void					CheckServerLink();		//连接账号服务器,暂不需要

	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

	//收到网关的包及地图服务器的包, 需要的具体的玩家识别号
	inline void							RecvClientPacket(JNetClient* pClient,ZPacket* pPacket);
	//收到账号服务器的包
	inline void							RecvSvrPacket(ZPacket* pSvrPacket);

	//发包给网关
	inline void							SendNGPacket(ZPacket* pNGPacket, DWORD dwKey);
	//发送给地图服务器,用网关的连接识别号, 目标由MapSelCfg提供
	bool								SendMSvrPacket(ZPacket* pMSvrPacket,DWORD mapIndex,JNetClient* pClient);

	xnList*								SwitchDealPList();			//2张表翻转

	//ZNetSvrLink*						m_pSvrGame;
	//ZNetSvrLink*						m_pSvrAccount;
	int								m_nMSvrCnt;
private:
	JNetClient*						m_szpNetGates[MAX_NETGATE];
	//配置文件对象里记录地图对象指针
	//JNetClient**					m_szpMapServer;

	//连接其他服务器的网络中心
	//ZNetSLinkCenter*				m_pCCLinkSvr;

	CRITICAL_SECTION				m_xRecvPacketLock;
	xnList*							m_pListRecvPacket;		//收包填充表
	xnList*							m_pListDealPacket;		//处理表
};

void	TCP_SendPacketToUin(DWORD dwUin, ZPacket* pPacket);
void	TCP_SendPacketToUinList(xnList* pList, ZPacket* pPacket);
void	TCP_SendPacketToList(xnList* pList, ZPacket* pPacket);
void	TCP_SendCmdToUin(DWORD dwUin, BYTE bCG, BYTE bCmd);
void	TCP_SendCmdToUinList(xnList* pList, BYTE bCG, BYTE bCmd);
void	TCP_SendCmdToKey(DWORD dwKey, BYTE bCG, BYTE bCmd);
void	TCP_SendPacketToKey(DWORD dwKey, ZPacket* pPacket);
extern ZNetCenter*	g_pNetCenter;