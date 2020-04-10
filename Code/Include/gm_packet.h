#ifndef _GAMEMASTER_PACKET_H_
#define _GAMEMASTER_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum  
{
	GM_JUMP,
	GM_SETUNHURT,
	//web interface
	GM_KICK_PLAYER,					//踢人
	GM_BLOCK_PLAYER,				//封號
	GM_FORBID_PLAYER,				//禁言
	GM_POST_ANNOUNCE,				//發布公告
	GM_SET_EXPTIME,					//設置經驗倍率
	GM_SAVE_EXCEPTION,				//保存玩家異常信息
	GM_BLOCK_PLAYER_EX,				//封號(多角色，中心->游戲服務器)
	GM_FORBID_PLAYER_EX,			//禁言(多角色，中心->游戲服務器)
	GM_SEND_MSG,					//私聊
	GM_RECOVER_PLAYER,				//恢复角色
	GM_SENDMAIL_PLAYER,				//发送邮件给角色
	GM_ADD_BINDGOLD,				//增加绑定金元宝
	GM_ADD_MONEY,					//增加银两
	GM_ADD_BINDMONEY,				//增加绑定银两
	GM_ADD_MAILNOTICE,
};

struct GMCMD_GM_JUMP_Data
{
	int	bMapIndex;
	int		nX;
	int		nY;
};

struct GMCMD_GM_SETUNHURT_Data
{
	bool	blUnHurt;
};

struct GMCMD_GM_KICK_PLAYER_Data
{
	DWORD   uin;
};

struct GMCMD_GM_BLOCK_PLAYER_Data
{
	DWORD	uin;
	int		DisMinute;
};

struct GMCMD_GM_FORBID_PLAYER_Data
{
	DWORD	uin;
	int		DisMinute;
	bool	VIP;
};

struct GMCMD_GM_POST_ANNOUNCE_Data
{
	int		nServerIndex;
	int     nCnt;
	char	szText[1];
};

struct GMCMD_GM_SET_EXPTIME_Data
{
	int		nExpTime;
};

struct GMCMD_GM_SAVE_EXCEPTION_Data
{
	bool	blSave;
};

struct GMCMD_GM_BLOCK_PLAYER_EX_Data
{
	DWORD	uin;
	DWORD	dwAccountUin;
	int		DisMinute;
};

struct GMCMD_GM_FORBID_PLAYER_EX_Data
{
	DWORD	uin;
	DWORD	dwAccountUin;
	int		DisMinute;
	bool	VIP;
};

struct GMCMD_GM_SEND_MSG_Data
{
	int     nUin;
	char	szText[1];
};

struct GMCMD_GM_RECOVER_PLAYER_Data
{
	DWORD	dwUin;
};

struct GMCMD_GM_SENDMAIL_PLAYER_Data
{
	DWORD dwRecvUin;
	DWORD dwMoney;
	DWORD dwItem1;
	DWORD dwItem2;
	DWORD dwItem3;
	DWORD dwItem4;
	DWORD dwItem5;
	DWORD dwItem6;
	char szBody[1];
};

struct GMCMD_GM_ADD_BINDGOLD_Data
{
	DWORD   uin;
	int     nCnt;
};

struct GMCMD_GM_ADD_MONEY_Data
{
	DWORD   uin;
	int     nCnt;
};

struct GMCMD_GM_ADD_MAILNOTICE_Data
{
	DWORD	uin;
	DWORD	dwItem;
	char	szBody[1];
};

struct GMCMD_GM_ADD_BINDMONEY_Data
{
	DWORD   uin;
	int     nCnt;
};
#pragma pack (pop)

#endif