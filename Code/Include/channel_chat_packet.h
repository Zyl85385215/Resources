#ifndef _CHAT_PACKET_H
#define _CHAT_PACKET_H


#pragma pack (push)
#pragma pack (1)

enum	_CHATSEVER
{
	TOOLS_LOGIN,
	TOOLS_LOGOUT,
	CS_TO_TOOL,
	TOOL_TO_CS,
	CS_TO_UIN,
};

enum
{
	MSG_IN,
	MSG_OUT,
};

enum
{
	CHATSEVER,
	CHATTOOL,

};

enum
{
	ENTER_ROOM,
	CREATE_CHANNEL,
	RELEASE_CHANNEL,
	ENTER_CHANNEL,
	EXIT_CHANNEL,
	SENDMSG_TO_CHANNEL,
	SENDMSG_TO_CHATOR,
	EXIT_ROOM,
};

enum	_MSG_TYPE
{
	INCHANNEL,
	NOTCHANNEL,
};

struct CHATSEVER_TOOLS_LOGIN_Data
{
	WORD    wServerIndex;
};

struct CHATSEVER_TOOL_TO_CS_Data
{
	 BYTE     data[1];
};
struct CHATTOOL_CS_TO_TOOL_Data
{
	char	szIP[20];
	BYTE	data[1];
};

struct CHATTOOL_CS_TO_UIN_Data
{
	DWORD	uin;
	BYTE     data[1];
};

struct MSG_IN_ENTER_ROOM_Data
{
	WORD    wServerIndex;
	char	szChatorId[1];
};

struct MSG_IN_EXIT_ROOM_Data
{
	char	szChatorId[1];
};

struct MSG_IN_CREATE_CHANNEL_Data
{
	char	szChatorId[1];
};

struct MSG_OUT_CREATE_CHANNEL_Data
{
	int		nChannel;
	bool	bresult;
	char	szChatorId[1];
};

struct MSG_IN_RELEASE_CHANNEL_Data
{
	int		nChannel;
	char	szChatorId[1];
};

struct MSG_OUT_RELEASE_CHANNEL_Data
{
	int		nChannel;
	bool	bresult;
	char	szChatorId[1];
};

struct MSG_IN_ENTER_CHANNEL_Data
{
	int		nChannel;
	char	szChatorId[1];
};

struct MSG_OUT_ENTER_CHANNEL_Data
{
	int		nChannel;
	bool	bresult;
	char	szChatorId[1];
};

struct MSG_IN_EXIT_CHANNEL_Data
{
	int		nChannel;
	char	szChatorId[1];
};

struct MSG_OUT_EXIT_CHANNEL_Data
{
	int		nChannel;
	bool	bresult;
	char	szChatorId[1];
};

struct MSG_IN_SENDMSG_TO_CHANNEL_Data
{
	int		nChannel;
	DWORD	nChatorId;
	char	szMSG[1];
};

struct MSG_OUT_SENDMSG_TO_CHANNEL_Data
{
	int		nChannel;
	DWORD	nChatorId;
	char	szMSG[1];
};

struct MSG_IN_SENDMSG_TO_CHATOR_Data
{
	DWORD	nChatorId;
	DWORD	nSenderId;
	char	szMSG[1];
};

struct MSG_OUT_SENDMSG_TO_CHATOR_Data
{
	DWORD	nChatorId;
	DWORD	nSenderId;
	char	szMSG[1];
};

#pragma pack (pop)
#endif  