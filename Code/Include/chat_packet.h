#ifndef _CHAT_PACKET_H_
#define _CHAT_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define	ADD_FRIEND_NOTIFY_IN_BLK				0x01
#define	ADD_FRIEND_NOTIFY_ALREADY_FRIEND		0x02
#define	ADD_FRIEND_NOTIFY_SUCCESS				0x03
#define ADD_FRIEND_NOTIFY_FRIEND_SWITCH_OFF		0X04

#define DELETE_FRIEND_NOTIFY_NO_SUCH_FRIEND		0x10
#define DELETE_FRIEND_NOTIFY_SUCCESS			0x11

#define ADD_BLACKLIST_NOTIFY_SUCCESS			0x20
#define ADD_BLACKLIST_NOTIFY_FAIL				0x21

#define DELETE_BLACKLIST_NOTIFY_SUCCESS			0x30
#define DELETE_BLACKLIST_NOTIFY_FAIL			0x31

enum _CHAT {

	MSGSEND,
	MSGSEND_FAILED,

	ADD_FRIEND,
	ADD_FRIEND_NOTIFY,

	DELETE_FRIEND,
	DELETE_FRIEND_NOTIFY,

	ADD_BLACKLIST,
	ADD_BLACKLIST_NOTIFY,

	DELETE_BLACKLIST,
	DELETE_BLACKLIST_NOTIFY,

	//add on 2004-11-16
	FRIEND_LIST,
	FRIEND_ONLINE_LIST,
	FRIEND_ONLINE,
	FRIEND_OFFLINE,
	GM_ONLINE,
	GM_OFFLINE,

	BLACK_LIST,
	BLACK_ONLINE_LIST,
	BLACK_ONLINE,
	BLACK_OFFLINE,
	
	FRIEND_SWITCH,
	
	SYSTEM_NEWS,
	GMSILENT1,
	SILENTTIME1

};

struct PLAYINFO
{
	DWORD	uin;
	char	playName[20];
};

struct CHAT_MSGSEND_Data {
	DWORD	m_ID;
	DWORD	m_ToID;
	char	m_content[1];
};

struct CHAT_GMSILENT1_Data
{
	int uin;
	int time1;
	int time2;
};

struct CHAT_SILENTTIME1_Data
{
	int uin;
	int time1;
	int time2;
};

struct CHAT_MSGSEND_FAILED_Data {
	DWORD	m_ID;
	DWORD   m_ToID;
	char	m_info[1];	
};

struct CHAT_ADD_FRIEND_Data {
	DWORD	m_ID;
	DWORD	fr_ID;	
};

struct CHAT_ADD_FRIEND_NOTIFY_Data {
	DWORD		m_ID;
	PLAYINFO	playinfo;
	char		m_info[1];
};

struct CHAT_DELETE_FRIEND_Data {
	DWORD	m_ID;
	DWORD	fr_ID;
};

struct CHAT_DELETE_FRIEND_NOTIFY_Data {
	DWORD		m_ID;
	DWORD		fr_ID;
	char		m_info[1];
};

struct CHAT_ADD_BLACKLIST_Data {
	DWORD	m_ID;
	DWORD	blk_ID;
};

struct CHAT_ADD_BLACKLIST_NOTIFY_Data {
	DWORD	m_ID;
	DWORD	blk_ID;
	char	m_info[1];
};

struct CHAT_DELETE_BLACKLIST_Data {
	DWORD	m_ID;
	DWORD	blk_ID;
};

struct CHAT_DELETE_BLACKLIST_NOTIFY_Data {
	DWORD	m_ID;
	DWORD	blk_ID;
	char	m_info[1];
};

//add on 2004-11-16

struct CHAT_FRIEND_LIST_Data {
	DWORD		m_ID;
    BYTE		count;
	PLAYINFO	playinfo[1];
};

struct CHAT_BLACK_LIST_Data {
	DWORD		m_ID;
    BYTE		count;
	PLAYINFO	playinfo[1];
};

struct CHAT_FRIEND_ONLINE_Data {
	DWORD		m_ID;
	PLAYINFO	playinfo;
};

struct CHAT_GM_ONLINE_Data {
	DWORD		gm_ID;
	PLAYINFO	playinfo;
};

struct CHAT_GM_OFFLINE_Data {

};

struct CHAT_FRIEND_OFFLINE_Data {
	DWORD		m_ID;
	DWORD		uin;
};

struct CHAT_BLACK_ONLINE_Data {
	DWORD		m_ID;
	PLAYINFO	playinfo;
};

struct CHAT_BLACK_OFFLINE_Data {
	DWORD		m_ID;
	DWORD		uin;
};

struct CHAT_FRIEND_SWITCH_Data{
	BOOL	flag;		//0:okey; 1:blocked
};

struct CHAT_SYSTEM_NEWS_Data {
	char msg[1];
};

#pragma pack (pop)

#endif

