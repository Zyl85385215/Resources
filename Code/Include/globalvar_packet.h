#ifndef _GLOBAL_VAR_PACKET_H_
#define _GLOBAL_VAR_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define GLOBALVAR_NAME_LEN	64
#define GLOBALVAR_VALUE_LEN	64

struct GlobalVarInt
{
	char	szName[GLOBALVAR_NAME_LEN];
	int		nVar;
};

struct GlobalVarString
{
	char	szName[GLOBALVAR_NAME_LEN];
	char	szVar[GLOBALVAR_VALUE_LEN];
};

enum
{
	GLOBAL_VALUE_TYPE_INT,
	GLOBAL_VALUE_TYPE_STRING,
	GLOBAL_VALUE_TYPE_MAX,
};

//	packet head : GLOBALVAR
enum 
{
	GLOBALVAR_INT_SEND_LIST,			//	发送列表
	GLOBALVAR_INT_ADD_VALUE,			//	变量加法
	GLOBALVAR_INT_SET_VALUE,			//	设置变量
	GLOBALVAR_STRING_SEND_LIST,			//	字符串变量列表
	GLOBALVAR_STRING_SET_VALUE,			//	设置字符串变量
	GLOBALVAR_INT_SEND_TO_CLIENT,		//  发送到一个客户端
	GLOBALVAR_MAX,
};

struct GLOBALVAR_GLOBALVAR_INT_SEND_LIST_Data
{
	int nCnt;
	GlobalVarInt var[1];
};

struct GLOBALVAR_GLOBALVAR_INT_ADD_VALUE_Data
{
	char	szName[GLOBALVAR_NAME_LEN];
	int		nVar;
};

struct GLOBALVAR_GLOBALVAR_INT_SET_VALUE_Data
{
	char	szName[GLOBALVAR_NAME_LEN];
	int		nVar;
};

struct GLOBALVAR_GLOBALVAR_STRING_SEND_LIST_Data
{
	int	nCnt;
	GlobalVarString var[1];
};

struct GLOBALVAR_GLOBALVAR_STRING_SET_VALUE_Data
{
	char	szName[GLOBALVAR_NAME_LEN];
	char	szVar[GLOBALVAR_VALUE_LEN];
};

struct GLOBALVAR_GLOBALVAR_INT_SEND_TO_ALLCLIENT_Data
{
	char	szName[GLOBALVAR_NAME_LEN];
	int		nVar;
};

struct GLOBALVAR_GLOBALVAR_INT_SEND_TO_CLIENT_Data
{
	char	szName[GLOBALVAR_NAME_LEN];
	int		nVar;
};

#pragma pack (pop)

#endif