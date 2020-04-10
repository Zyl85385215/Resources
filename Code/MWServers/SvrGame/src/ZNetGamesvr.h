#pragma once

#include "ZNet.h"

#include "xnlist.h"
#include "ZNetPacket.h"
#include <map>

enum
{
	LOCK_SVR_ACC,	//�˺�����,���ܵ�¼
	LOCK_SVR_CHAT,	//������������ҽ���
	LOCK_SVR_PK,	//���ܲ���PK

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
	void					CheckServerLink();		//�����˺ŷ�����,�ݲ���Ҫ

	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

	//�յ����صİ�����ͼ�������İ�, ��Ҫ�ľ�������ʶ���
	inline void							RecvClientPacket(JNetClient* pClient,ZPacket* pPacket);
	//�յ��˺ŷ������İ�
	inline void							RecvSvrPacket(ZPacket* pSvrPacket);

	//����������
	inline void							SendNGPacket(ZPacket* pNGPacket, DWORD dwKey);
	//���͸���ͼ������,�����ص�����ʶ���, Ŀ����MapSelCfg�ṩ
	bool								SendMSvrPacket(ZPacket* pMSvrPacket,DWORD mapIndex,JNetClient* pClient);

	xnList*								SwitchDealPList();			//2�ű�ת

	//ZNetSvrLink*						m_pSvrGame;
	//ZNetSvrLink*						m_pSvrAccount;
	int								m_nMSvrCnt;
private:
	JNetClient*						m_szpNetGates[MAX_NETGATE];
	//�����ļ��������¼��ͼ����ָ��
	//JNetClient**					m_szpMapServer;

	//������������������������
	//ZNetSLinkCenter*				m_pCCLinkSvr;

	CRITICAL_SECTION				m_xRecvPacketLock;
	xnList*							m_pListRecvPacket;		//�հ�����
	xnList*							m_pListDealPacket;		//�����
};

void	TCP_SendPacketToUin(DWORD dwUin, ZPacket* pPacket);
void	TCP_SendPacketToUinList(xnList* pList, ZPacket* pPacket);
void	TCP_SendPacketToList(xnList* pList, ZPacket* pPacket);
void	TCP_SendCmdToUin(DWORD dwUin, BYTE bCG, BYTE bCmd);
void	TCP_SendCmdToUinList(xnList* pList, BYTE bCG, BYTE bCmd);
void	TCP_SendCmdToKey(DWORD dwKey, BYTE bCG, BYTE bCmd);
void	TCP_SendPacketToKey(DWORD dwKey, ZPacket* pPacket);
extern ZNetCenter*	g_pNetCenter;