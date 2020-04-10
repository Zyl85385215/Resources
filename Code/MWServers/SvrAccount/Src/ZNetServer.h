#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"
#include <map>
#include "HttpCenter.h"
#include "ZLogFile.h"

enum
{
	LOCK_SVR_ACC,	//�˺�����,���ܵ�¼
	LOCK_SVR_CHAT,	//������������ҽ���
	LOCK_SVR_PK,	//���ܲ���PK

	LOCK_SVR_MAX,
};

#define MAX_NETGATE		3
#define MAX_GAMESVR		3

//��Ϸ���������������ӽ���, ���ܶ��
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
	//�ͻ���������������б�
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