#ifndef _SYSTEM_PACKET_H_
#define _SYSTEM_PACKET_H_

#include <Windows.h>

#pragma pack (push)
#pragma pack (1)

enum _SYSTEM {
    SERVER_LOGIN,
    SERVER_LOGOUT,

    SERVER_UPDATE_TICK,
    SERVER_ONLINE_COUNT,

    RECV_FROM_UIN,
    FORWARD_TO_UIN,

    RECV_FROM_TAG,
    FORWARD_TO_TAG,

    SET_UIN,
    SET_SILENT,

    KICK_UIN,
    KICK_TAG,

    CHECK_BILL,
    NEXT_CHECK_BILL_TIME,

    ONLINE_LIST,

	GET_GAME_SERVER_LIST,
	GET_GAME_SERVER_LIST_SCCUESS,

	GIVE_GIFT,
	REQUEST_GIFT,

	SERVER_ONLINE,
	
	PLAYER_ONLINE,
	PLAYER_OFFLINE,	

	CHECK_SERVER_ACTIVE,
	
	SET_GAME_MASTER,			// add by Truman by GM_VIEWER
	GM_BIND_UIN,				
	GM_UNBIND_UIN,
	GM_BIND_UIN_ERROR,
	
	COMPRESS_PACKET,
	DISCONNECT_ERROR_CODE,
	SET_SAMEIP_LIMIT,

	TPHTTP_ADDGOLD,

	POINTSERVER_LOGIN = 0xFE,
	POINTSERVER_LOGOUT = 0xFF,
};

enum {
	BIND_UIN_OK,
	BIND_UIN_NOT_FOUND,
	BIND_UIN_LOGOUT,
	BIND_UIN_IS_GAMEMASTER,
	BIND_UIN_BY_OTHER_GM,

};

struct SYSTEM_SERVER_LOGIN_Data {
    char    IP[16];
    WORD    port;
    BYTE    checkBill;
    char    serverName[1];
};
struct SYSTEM_SERVER_ONLINE_Data{
};

struct SYSTEM_SERVER_ONLINE_COUNT_Data {
    WORD    count;
};

struct SYSTEM_RECV_FROM_UIN_Data {
    unsigned int uin;
    BYTE    data[1];
};
#define SYSTEM_FORWARD_TO_UIN_Data  SYSTEM_RECV_FROM_UIN_Data

struct SYSTEM_RECV_FROM_TAG_Data {
    unsigned int tag;
    BYTE    data[1];
};
#define SYSTEM_FORWARD_TO_TAG_Data  SYSTEM_RECV_FROM_TAG_Data

struct SYSTEM_SET_UIN_Data {
    unsigned int tag;
    unsigned int uin;
	char id[1];
};

struct SYSTEM_SET_SILENT_Data {
    unsigned int uin;
    BYTE    isSilent;
};

struct SYSTEM_KICK_UIN_Data {
    unsigned int uin;
};
#define SYSTEM_KICK_TAG_Data    SYSTEM_KICK_UIN_Data

struct SYSTEM_CHECK_BILL_Data {
    unsigned int uin;
};

struct SYSTEM_NEXT_CHECK_BILL_TIME_Data {
    unsigned int uin;
    DWORD   time;
};

struct SYSTEM_ONLINE_LIST_Data {
    BYTE    count;
    unsigned int uin[1];
};

struct SYSTEM_GET_GAME_SERVER_LIST_Data {
};


struct SYSTEM_TPHTTP_ADDGOLD_Data
{
	int		dwMoney;
	char	szAcc[50];
	char	szOrder[50];
	int		nPlat;
};

struct GAME_SERVER {
	char	szIP[16];
	char	szName[32];
	int		nPort;
};

struct SYSTEM_GET_GAME_SERVER_LIST_SCCUESS_Data {
	BYTE		yCount;
	GAME_SERVER	tGameServer[1];
};

struct SYSTEM_GIVE_GIFT_Data {
	int	nUIN;
	int	nItemList[1];
};

struct SYSTEM_REQUEST_GIFT_Data {
	int	nUIN;
};

struct SYSTEM_PLAYER_ONLINE_Data {
	DWORD	uin;
	char	name[1];
};

struct SYSTEM_PLAYER_OFFLINE_Data {
	DWORD	uin;
};

struct SYSTEM_SET_GAME_MASTER_Data {
	DWORD	uin;
};

struct SYSTEM_GM_BIND_UIN_Data {
	DWORD	uin;
};

struct SYSTEM_GM_UNBIND_UIN_Data {
	DWORD	uin;
};

struct SYSTEM_GM_BIND_UIN_ERROR_Data {
	BYTE	errCode;
};

struct SYSTEM_COMPRESS_PACKET_Data {
	WORD size;
	BYTE data[1];
};

struct SYSTEM_DISCONNECT_ERROR_CODE_Data {
	BYTE code;
};
struct SYSTEM_POINTSERVER_LOGIN_Data
{

};

struct SYSTEM_POINTSERVER_LOGOUT_Data
{

};

struct SYSTEM_SET_SAMEIP_LIMIT_Data 
{
	int nLimit;
};

#pragma pack (pop)
#endif
