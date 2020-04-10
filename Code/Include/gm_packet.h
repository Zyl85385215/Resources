#ifndef _GAMEMASTER_PACKET_H_
#define _GAMEMASTER_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum  
{
	GM_JUMP,
	GM_SETUNHURT,
	//web interface
	GM_KICK_PLAYER,					//����
	GM_BLOCK_PLAYER,				//��̖
	GM_FORBID_PLAYER,				//����
	GM_POST_ANNOUNCE,				//�l������
	GM_SET_EXPTIME,					//�O�ý�򞱶��
	GM_SAVE_EXCEPTION,				//������Ү�����Ϣ
	GM_BLOCK_PLAYER_EX,				//��̖(���ɫ������->�Α������)
	GM_FORBID_PLAYER_EX,			//����(���ɫ������->�Α������)
	GM_SEND_MSG,					//˽��
	GM_RECOVER_PLAYER,				//�ָ���ɫ
	GM_SENDMAIL_PLAYER,				//�����ʼ�����ɫ
	GM_ADD_BINDGOLD,				//���Ӱ󶨽�Ԫ��
	GM_ADD_MONEY,					//��������
	GM_ADD_BINDMONEY,				//���Ӱ�����
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