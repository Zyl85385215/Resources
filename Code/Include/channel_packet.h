#ifndef _CHANNEL_PACKET_
#define _CHANNEL_PACKET_

#pragma pack (push)
#pragma pack (1)

enum 
{
	SET_FILTER,											//--------------
	MESSAGE_IN,                                         //消息--客户端-->服务器
	MESSAGE_OUT,                                        //消息--服务器-->客户端
	ENABLE_BLOCK,										//---------------
	DISABLE_BLOCK,										//---------------
	QUESTION_MSG,
	ANSWER_MSG,
	SERVER_NEWS,										//
	MESSAGE_SYSTEM,										//
	MESSAGECODE_SYSTEM,										//	
	SERVER_OFFICIAL_NEWS,
	BULLETIN_MESSAGE,
	SERVER_COUNTRY_NEWS,
	POP_SYSTEM_MSG,
	POP_SYSTEM_CODE,


	ROSE_REQUEST,
	XUNYUAN_NOTIFY,
};

enum 
{
	CHANNEL_COUNTRY,                                   //国家频道
	CHANNEL_GANG,                                      //帮会频道
	CHANNEL_GROUP,                                     //组队频道
	CHANNEL_PRIVE,                                     //私聊频道
	CHANNEL_SCREEN,                                    //说话频道
	CHANNEL_BATTLE,                                    //战场频道
	CHANNEL_WORLD,                                     //世界频道 
	CHANNEL_CLIENT_SYSTEMMSG,
	CHANNEL_CLIENT_SYSTEMMSG_FROM_CONFIG,
	CHANNEL_SPECIAL,
	CHANNEL_ARENA,
	CHANNEL_SPECIAL_PLAYER,
	CHANNEL_SYSTEM = 128,
};

#define CHANNEL_FILTER_COUNTRY	(1<<CHANNEL_COUNTRY)
#define CHANNEL_FILTER_SCREEN	(1<<CHANNEL_SCREEN)
#define CHANNEL_FILTER_GROUP	(1<<CHANNEL_GROUP)
#define CHANNEL_FILTER_GANG		(1<<CHANNEL_GANG)
#define CHANNEL_FILTER_PRIVE	(1<<CHANNEL_PRIVE)
#define CHANNEL_FILTER_BATTLE   (1<<CHANNEL_BATTLE)
#define CHANNEL_FILTER_WORLD	(1<<CHANNEL_WORLD)


enum 
{
	QID_TRADE,
	QID_GROUP,
	QID_CONF,
	QID_DARE,
	QID_HOME,
	QID_GROUP_ASKJOIN,		//申请入队
	QID_BIG_GROUP,
	QID_SLAVE_PK,
	QID_BIG_GROUP_ASKJOIN,
	QID_SLAVE_PK_MONEY,
	QID_MAX,
};

enum 
{
	QID_ANSWER_YES,
	QID_ANSWER_NO,
	QID_ANSWER_TIMEOUT,
	QID_ANSWER_ERR,
	QID_ANSWER_ERR2,
	QID_ANSWER_MAX,
};

enum MESSAGE_SYSTEM_TYPE
{
	SYSTEM_MSG,					// 有利的系统提示
	COMMON_MSG,					// 角色的得失提示与任务提示等
	BULLITIN_MSG,				// 闪动提示
	BAD_SYSTEM_MSG,				// 不利的系统提示
	OPERATION_SUCCESSFUL_MSG,	// 主动操作成功提示
	OPERATION_FAILED_MSG,		// 主动操作失败提示
	BULLITIN_PLAYER_MSG,
	ITEM_MSG,
	CORNER_MSG,					//右下角提示信息
	RUNNING_MSG,

	BULLITIN_MSG_GOLD	= 21,
	BULLITIN_MSG_BLUE,
	BULLITIN_MSG_YELLOW,
	BULLITIN_MSG_PINK,
	BULLITIN_MSG_PURPLE,
	BULLITIN_MSG_PURPLE_NEW,

	MESSAGE_CHAR_TIP,
};

struct CHANNEL_SET_FILTER_Data 
{
	BYTE	FILTER;
};

struct CHANNEL_MESSAGE_IN_Data               
{
	BYTE	channel;
	BYTE    bSpecil;
	char	msg[1];
};

struct CHANNEL_MESSAGE_OUT_Data             
{
	BYTE	channel;
	BYTE	bCountry;
	DWORD   sendUin;
	DWORD   recvUin;
	char	send[16];
	char	recv[16];
	BYTE    bSpecil;
	char	msg[1];
};

struct CHANNEL_ENABLE_BLOCK_Data 
{
	DWORD uin;
};

struct CHANNEL_DISABLE_BLOCK_Data 
{
	DWORD uin;
};

struct CHANNEL_QUESTION_MSG_Data 
{
	BYTE	QID;
	DWORD	sendUin;
	DWORD	recvUin;
	char	msg[1];
};

struct CHANNEL_ANSWER_MSG_Data 
{
	BYTE	QID;
	BYTE	answer;
	DWORD	sendUin;
	DWORD	recvUin;
};

struct CHANNEL_BULLETIN_MESSAGE_Data 
{
	WORD	wItemID;
	DWORD	dwItemIndex;
	char	msg[1];
};

struct CHANNEL_SERVER_NEWS_Data 
{
	BOOL	m_GM;
	BYTE	m_Times;
	BYTE	m_Country;
	char	msg[1];
};

struct CHANNEL_SERVER_OFFICIAL_NEWS_Data 
{
	char	msg[1];
};

//system message,type=0 player action result message,type=1 system message(news,notify etc).
struct CHANNEL_MESSAGE_SYSTEM_Data
{
	BYTE	type;
	BYTE	Index;
	//time_t	localTime;
	char	msg[1];
};

struct CHANNEL_POP_SYSTEM_MSG_Data 
{
	BYTE	type;
	BYTE	Index;
	//time_t	localTime;
	char	msg[1];
};

struct CHANNEL_MESSAGECODE_SYSTEM_Data
{
	BYTE	type;
	time_t	localTime;
	BYTE	byMsgIndex;
};

struct CHANNEL_POP_SYSTEM_CODE_Data 
{
	BYTE	type;
	time_t	localTime;
	BYTE	byMsgIndex;
};

struct CHANNEL_ROSE_REQUEST_Data 
{
	BYTE	type;
};

struct CHANNEL_XUNYUAN_NOTIFY_Data 
{
	char szText[512];
};

struct CHANNEL_SERVER_COUNTRY_NEWS_Data
{
	BYTE	bCountry;
	char	msg[1];
};
#pragma pack (pop)

#endif