#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"
#include <hash_map>
#include <map>
using namespace std;

enum
{
	LOCK_SVR_ACC,	//账号锁定,不能登录
	LOCK_SVR_CHAT,	//不能与其他玩家交互
	LOCK_SVR_PK,	//不能参与PK

	LOCK_SVR_MAX,
};

//网络连接的客户信息
class ZNetCData
{
public:
	DWORD	dwKey;			//唯一识别号
	DWORD	dwUin;			//UIN
	DWORD	dwAccount;		//ACC或临时通行号
	DWORD	dwRecv;
	DWORD	dwSend;
	DWORD	dwTick;
	BYTE	bMapSrvID;
	DWORD	dwHeartBeatTime;
	DWORD	dwTickHBReset;
	DWORD	szdwLockSvr[LOCK_SVR_MAX];
};

typedef std::hash_map<DWORD,JNetClient*>						HMapClient;
typedef std::hash_map<DWORD,JNetClient*>::iterator			HMapClientPtr;

class CListCtrl;
class ZNetClientCenter
{
public:
	ZNetClientCenter();
	~ZNetClientCenter();

	int				AddClient(JNetClient* pNClient);
	bool			RemoveClient(JNetClient* pNClient,bool bOutGame=true);
	void			RejoinClient();

	inline JNetClient*		GetByKey(DWORD dwKey);
	inline JNetClient*		GetByAccUin(DWORD dwAcc);

	void			RefreshListView(CListCtrl* pCtrlList);
private:
	DWORD							m_dwMaxKey;
	HMapClient						mapClients;
	CRITICAL_SECTION				m_xLock;
};

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
	inline int						ProcessData(char* pRecvBuffer,int nDataLen);

	inline bool						SendPacket(ZPacket * pPacket);

	
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
	void					CheckServerLink();

	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

	void						_dealClickPacket(JNetClient* pClient,ZPacket* pPacket);
	void						_dealSvrPacket(ZPacket* pPacket);
	void						RecvClientPacket(JNetClient* pClient,ZPacket* pPacket);
	void						RecvSvrPacket(ZPacket* pSvrPacket);

	void							RefreshUiView(CListCtrl* pCtrl)	{m_pNetClients->RefreshListView(pCtrl);}
	void					NoticeClient(JNetClient* pClient,WORD wDefSvr,BYTE bySvrType = 0);
	void					SendSeverState(JNetClient* pClient);
	void					SendAreaName(JNetClient* pClient);
	ZNetSvrLink*						m_pSvrGame;
	ZNetSvrLink*						m_pSvrAccount;
	ZNetSvrLink**						m_ppSvrMap;
	int								m_nLinkCnt;
	int								m_nLinkTotal;

	DWORD							m_dwRecvClient;
	DWORD							m_dwSendClient;
	int								m_nServerCnt;
	int								m_nMapSvrCnt;
	char*							m_szNotice;
	char*							m_szADNotice;
	xnList*							m_pHotSvrList;
	void                            RefHotSvrList(int nSvID,CListCtrl* pCtrlList,bool bClear);
	void                            GetSvrListByType(xnList* pOutList,bool bAD = false);
	int                             GetHotSvr();
	int                             GetADHotSvr();

	map<int,BYTE>                   m_mapSeverState;
	void                            RefSeverState(int nSvID,int nState,CListCtrl* pCtrlList,bool bClear = false);

	int                             m_nDefCountry;
private:
	inline	void					_sendPacketToClient(JNetClient* pClient, ZPacket* pPacket);
	//客户端连上来的玩家列表
	ZNetClientCenter*				m_pNetClients;
	//连接其他服务器的网络中心
	ZNetSLinkCenter*				m_pCCLinkSvr;
};

extern ZNetCenter*	g_pNetCenter;