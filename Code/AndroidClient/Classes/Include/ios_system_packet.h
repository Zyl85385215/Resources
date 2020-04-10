#pragma once

#ifndef _ISSERVER
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum _SYSTEM
{
	NETGATE_LOGIN_ACC,	//�������������������İ�
	GAMESVR_LOGIN_ACC,

	MAPSERVER_LOGIN_GAMESRV,
};
enum _GMTOOL
{
	C_LUA,
};
struct GMTOOL_C_LUA_Data
{
	char	szAccount[32];
	char	szPassword[32];
	char	szLua[512];
};
struct SYSTEM_NETGATE_LOGIN_ACC_Data 
{
	BYTE	bySubKey;		//���ر��
};

struct SYSTEM_GAMESVR_LOGIN_ACC_Data 
{
	BYTE	bySubKey;		//���ر��
};

struct SYSTEM_MAPSERVER_LOGIN_GAMESRV_Data 
{
	char	szIp[20];
	int		nPort;
};

#pragma pack (pop)