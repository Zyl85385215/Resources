#ifndef __GAME_CORE_DLL_H__
#define __GAME_CORE_DLL_H__
#include "packetstruct.h"
#ifndef BOOLEAN
#define BOOLEAN unsigned char
#endif

#ifndef WORD
#define WORD unsigned short
#endif

#ifndef	DWORD
#define DWORD unsigned long
#endif

#pragma pack (push)
#pragma pack (1)

struct GameShellPublic {
	void (*TcpSendData)(unsigned int uin, void * sendData, int sendSize);
	void (*MessageOut)(char * message, int channel);
	void (*OnJoinGame)(unsigned int uin, char * name, char * nickname);
    void (*OnExitGame)(unsigned int uin);
	BOOLEAN (*DBaseCmd)(unsigned int tag, unsigned int cmd, char * sqlStr);
	BOOLEAN (*DBaseSetBlob)(unsigned int tag, char *sqlStr, void * blobData, unsigned int dataSize);
#ifdef USE_NEW_DB
	BOOLEAN (*SendDBPacket)(Packet* pPacket);
#endif
};

struct GameCorePublic {
	BOOLEAN	(*Init)(GameShellPublic * p);
	void	(*ShutDown)(void);
	BOOLEAN	(*RecvData)(unsigned int uin, void * recvData, int recvSize);
	BOOLEAN (*GetCharacterName)(const unsigned int uin, char * name, char * nickname);
	BOOLEAN	(*AddSpecialItem)(const unsigned int uin, unsigned int * itemList);
    void    (*HeartBeat)(unsigned int frameTime);
	char **  dllVersion;
	int	 * onlineCount;
	DWORD * versionCode;
};

#pragma pack (pop)


#define	GameCordDllExportName	"dllFunc"

#undef DWORD
#undef WORD
#undef BOOLEAN

#endif
