#ifndef _JCH_NETWORK_UTILITY_H
#define _JCH_NETWORK_UTILITY_H
#include <winsock2.h>
#include <mswsock.h>
#include <windows.h>
#include <process.h>
#include "random.h"


enum
{
	NETWORK_CLIENT_EVENT = WM_USER+100,
	NETWORK_UDP_EVENT,
	NETWORK_SERVER_EVENT
};
#ifndef MAX_RECV_BUF    
#define MAX_RECV_BUF				m_dwRecvMaxSize//30760     //网关服务器缓存不能过大,需改小     
#endif  

#ifndef MAX_SEND_BUF    
#define MAX_SEND_BUF				m_dwSendMaxSize//92160
#endif

//#ifndef MAX_COMPRESS_BUF    
//#define MAX_COMPRESS_BUF			30750
//#endif

#ifndef MAX_REUSE_CLIENT
#define MAX_REUSE_CLIENT			3000
#endif

//#ifndef MAX_REUSE_RECVBUF
//#define MAX_REUSE_RECVBUF			1000
//#endif

#ifndef max_waitsocket
#define max_waitsocket			100
#endif
class SOCKETDATA
{
protected:
	char *			m_pIp;
	sockaddr_in		m_xAddress;
	BYTE*			m_pbReceiveBuffer;
	DWORD			m_dwRecvMaxSize;
	int				m_nReceivePosition;
public:
	SOCKET			m_skSocket;
	int				m_nPort;
	bool			m_blActive;
	void*			m_pData;
	SOCKETDATA(bool bBigBuf);
	~SOCKETDATA();
};
class JnetSocket;
class CSocketTCP
{
	friend			JnetSocket;
	JnetSocket*		m_pSocket;
private:
	CSocketTCP();
	~CSocketTCP();
	void			_SetSocket(JnetSocket* pSocket);
public:
	void			Diconnect();
	bool			SendData(BYTE* pData,DWORD dwLen);
	DWORD			GetID();
	bool			IsActive();
};


class CSocketUDP:public SOCKETDATA  
{
public:
	CSocketUDP();
	virtual ~CSocketUDP();
	bool			Start(int port);
	void			Stop();
	void			Diconnect();
	void			RecvData();
	bool			SendData(char* ip,int port,void* dataBuf, int dataSize);
	virtual void	ProcessData(sockaddr_in* pPeer,int size);
	virtual void	OnStop();
	virtual void	OnStart();
};

class CCore;
class JClientCenter
{
	CCore*			m_pCore;
public:
	JClientCenter();
	virtual ~JClientCenter();
	CSocketTCP*						InitConnect(const char * szIP, int nPort, const char * szLocalIP = "",int nBindPort=0);
	bool							InitCenter(DWORD m_dwMaxConnectedCnt);
	void							UnInitServer();
	bool							Start();
	void							Stop();
	DWORD							GetActiveClientCnt();
	DWORD							GetWorkThreadCnt();
	virtual void					OnCreateConnect(CSocketTCP* pClient);
	virtual void					OnDisconnect(CSocketTCP* pClient);
	virtual int						ExtractPacket(CSocketTCP* pClient,char* pRecvBuffer,int nDataLen);	
};

//----------------------------------------------------------------------------------
//server
class JNetClient;
static GUID g_GUIDAcceptEx             = WSAID_ACCEPTEX;
static GUID g_GUIDGetAcceptExSockaddrs = WSAID_GETACCEPTEXSOCKADDRS;


#define SendGroupCnt	10
class JNetServer;
class JNetServerCore;
class JNetMain;
class JEmptyClientList;
class JActiveClientList;

class SSocket;
class JNetClient
{
	friend	SSocket;	
	friend	JNetServerCore;
private:
	SSocket*						m_pSocket;
	void*							m_pData;
	JNetClient();
	~JNetClient();
	void							_SetSocket(SSocket* pSocket);
public:
	void							SendData(BYTE* pData,DWORD dwLen);
	void							SetData(void* pData);
	void*							ExtractData();	
	void*							GetData();
	void							ShutDown();
	DWORD							GetID();
	SOCKADDR_IN*					GetAddress();
	bool							IsActive();

	DWORD							m_dwLastSendTm;
	DWORD							m_dwLasrRecvTm;
	DWORD							m_dwLastSendRemain;
	CRandom							m_xRandom;
};

class JNetServer
{
	friend	JNetMain;
	friend	JNetClient;
	JNetServerCore*					m_pCore;
public:
	JNetServer();
	virtual	~JNetServer();
	bool							InitServer(int nPort, const char * szAddress, DWORD dwMaxConnectedCnt,bool bBigBuf);
	//const char * szAddress = "", DWORD dwMaxConnectedCnt = 0xFFFFFFFF
	void							UnInitServer();
	bool							Start();
	void							Stop();
	DWORD							GetActiveClientCnt();
	DWORD							GetWorkThreadCnt();
	virtual void					OnCreateConnect(JNetClient* pClient);
	virtual void					OnDisconnect(JNetClient* pClient);
	virtual int						ExtractPacket(JNetClient* pClient,char* pRecvBuffer,int nDataLen);

};
typedef void (*DumpMsgFunc)(const char* szMsg);
typedef int (*CompressDataFunc)(void* dstin, void* srcin, int datalen, JNetClient* pclient);

class JNetMain
{
	friend	CSocketTCP;
	friend	CSocketUDP;
private:
	static SOCKETDATA*		_GetSocketData(SOCKET socket);	
	static void				_AddSocketData(SOCKETDATA* pSocketData);
	static void				_RemoveSocketData(SOCKET socket);	
public:
	static LRESULT CALLBACK JNET_MessageCallback(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	static bool				NetInit(DumpMsgFunc func, CompressDataFunc cfunc);
	static bool				NetShutDown();
	static void				DumpMsg(const char* szFormat,...);
	static int				CompressData(void* dstin, void* srcin, int datalen, JNetClient* pclient);
};
#endif