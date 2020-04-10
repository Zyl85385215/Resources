#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum
{
	C_CHAT_MSG,
	S_SEND_MESSAGE,
	C_REQUEST_ITEMINFO,
	S_SEND_ITEMINFO,
	S_SEND_PROCLAMATION,	//����
	S_SEND_SIMPLEMSG,
	S_P2PCHAT_RLT,
};

enum
{
	CHAT_ROOM_SYSTEM,		//ϵͳ��Ϣ
	CHAT_ROOM_WORLD,		//����Ƶ��
	CHAT_ROOM_COUNTRY,		//����
	CHAT_ROOM_P2P,			//˽��
	CHAT_ROOM_MAP,			//��ͼ
	CHAT_ROOM_TEAM,			//����
	CHAT_ROOM_HOME,			//����
	CHAT_ROOM_NULL,			//��ɫ��ϵͳ��ʾ
	CHAT_ROOM_MAX,
};

struct ShowItemInfo
{
	DWORD showID;
	DWORD dwStartTick;
	int nSize;
	char szData[256];
};

struct ShowItemSimpleInfo
{
	DWORD showID;
	int nItemType;
};

struct RPGCHAT_C_CHAT_MSG_Data
{
	BYTE	byCRType;
	int		nCnt;
	int		nRecvUin;
	int		szID[5];
	char	szMsg[1];
};

struct RPGCHAT_S_SEND_SIMPLEMSG_Data
{
	char	szMsg[1];
};

struct RPGCHAT_S_SEND_PROCLAMATION_Data
{
	BYTE	byType;
	int		nCnt;
	char	szMsg[1];
};

struct RPGCHAT_S_SEND_MESSAGE_Data
{
	BYTE	byCRType;
	BYTE	nKeyVal;
	char	szSName[20];
	int		nSUin;
	BYTE	bySV;
	char	szRName[20];
	int		nRUin;
	BYTE	byRV;
	int		nCnt;
	ShowItemSimpleInfo	xInfo[5];
	char	szMsg[1];
};

struct RPGCHAT_C_REQUEST_ITEMINFO_Data
{
	int nID;
};

struct RPGCHAT_S_SEND_ITEMINFO_Data
{
	ShowItemInfo xInfo;
	bool ifSuccess;
};

#pragma pack (pop)