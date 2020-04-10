#ifndef __NET_TCPIP_H__
#define __NET_TCPIP_H__

#define MAX_SEND_BUFFER_SIZE 		1024
#define	MAX_RECV_BUFFER_SIZE		MAX_SEND_BUFFER_SIZE
#define MAX_RECV_BUFFER_NUMBER		20

#include "xnlist.h"

struct xnNetSocket {
	int sockfd;
	BOOLEAN	connectFlag;
	char ip[16];
	int port;
	void * extData;
};

/*
struct xnNetClient {
	DWORD	packetMark;
	int		port;
	char	ip[16];
	BOOLEAN connectFlag;
	void * data;

	void (*Connect)(xnNetClient * p);
	void (*Disconnect)(xnNetClient * p);
	void (*Recv)(xnNetClient * p, void * buf, int recvSize);

	static xnNetClient * Create(void);
	void Free(void);
	void Open(char * ip, int port);
	void Close(void);
	void Send(void * buf, int sendSize);
	void * Recv(void);
};

struct xnNetServer {
	DWORD	packetMark;
	int		port;
	char	ip[16];
	BOOLEAN openFlag;
	void * data;

	xnList * clientList;

	void (*listen)(xnNetServer * p);
	void (*Open)(xnNetServer * p);
	void (*Close)(xnNetServer * p);

	void (*ClientConnect)(xnNetClient * p);
	void (*ClientDisconnect)(xnNetClient * p);
	void (*ClientRecv)(xnNetClient * p);

	static xnNetServer * Create(void);
	void Free(void);
	void Open(int port);
	void Close(void);
	void SendToAll(void * buf, int sendSize);
};
*/

BOOLEAN xnNetInit(void);
void xnNetShutDown(void);

xnNetSocket * xnNetServer(char * ip, int port, 
						  void (* recvFunc)(xnNetSocket * netData, void * recvData, int recvSize),
						  BOOLEAN (* ConnectFunc)(xnNetSocket * netData));
xnNetSocket * xnNetClient(char * ip, int port,
						  void (* recvFunc)(xnNetSocket *netData, void * recvData, int recvSize));
void xnNetClose(xnNetSocket * netData);

void xnNetSetPacketMark(int mark);
BOOLEAN xnNetSendData(xnNetSocket * netData, void * sendData, int sendSize);
int xnNetRecvData(xnNetSocket * netData, void * recvBuffer, int bufferSize);

// get http file function ============================================================
enum	{
	NET_FILE_ERROR	= -1,
	NET_FILE_WAIT	= 0,
	NET_FILE_OK		= 1
};

struct xnNetFile {
	int status;
};

xnNetFile * xnNetGetHTTP_File(char * httpName, char * rfName, char *wfName);
void xnNetCloseFile(xnNetFile * fileData);

// ping function ====================================================================
enum	{
	NET_PING_ERROR	= -1,
	NET_PING_WAIT	= 0,
	NET_PING_OK		= 1
};

struct xnNetPingData {
	int status;
	int	speed;
};

xnNetPingData * xnNetPingAlloc(char * ip);
void xnNetPingFree(xnNetPingData * pingData);
void xnNetPing(xnNetPingData * pingData);

#pragma comment(lib,"Ws2_32.lib")

#endif
