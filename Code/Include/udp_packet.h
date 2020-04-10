#ifndef _UDP_PACKET_H_
#define _UDP_PACKET_H_

enum {
	UDP_ASK_CONNECT,
	UDP_PEER_INFO,
	UDP_VOICE,
	SVRUI_CMD,
	TOSVRUI,
};

struct UDP_UDP_PEER_INFO_Data {
	DWORD uin;
	char IP[16];
	WORD port;
};

struct UDP_UDP_VOICE_Data {
	DWORD uin;
	DWORD timeTick;
	BYTE  data[1];
};

struct SVRUICMD_SVRUI_CMD_Data
{
	DWORD dwCmd;
};

struct UDP_TOSVRUI_Data
{
	DWORD dwType;
	DWORD dwData;
};
#endif