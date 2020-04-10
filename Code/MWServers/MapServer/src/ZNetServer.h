#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"

#define MAX_NETGATE		3
//�������ӵĿͻ���Ϣ
class ZNetSLinkCenter	: public JClientCenter
{
public:
	virtual void					OnCreateConnect(CSocketTCP* pClient)	{}
	virtual void					OnDisconnect(CSocketTCP* pClient)	{}
	virtual int						ExtractPacket(CSocketTCP* pClient,char* pRecvBuffer,int nDataLen);	
};

//����������������
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
	//�յ���
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

	xnList*							SwitchDealNetGateList();			//2�ű�ת
	xnList*							SwitchDealGameSvrList();			//2�ű�ת

	ZNetSvrLink*					m_pSvrGame;
private:
	//�ͻ���������������б�
	JNetClient*						m_szpNetGates[MAX_NETGATE];
	//������������������������
	ZNetSLinkCenter*				m_pCCLinkSvr;

	CRITICAL_SECTION				m_xLockGameSvr;
	CRITICAL_SECTION				m_xLockNetGate;
	xnList*							m_pListRecvGameSvr;		//�հ�����
	xnList*							m_pListRecvNetGate;		//�հ�����
	xnList*							m_pListDealGameSvr;		//�����
	xnList*							m_pListDealNetGate;		//�����



};

extern ZNetCenter*	g_pNetCenter;