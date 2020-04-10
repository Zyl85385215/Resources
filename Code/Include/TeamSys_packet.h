#pragma once

#pragma pack (push)
#pragma pack (1)

enum
{
	//客户端到中心
	TSYS_GETTEAMS,		//GetFullTST(int uin, OUT TSysTeam** ppTST, int nCnt, int nStartIndex = 0 )
	TSYS_GETPLAYERS,		//GetFullTSP(int uin, OUT TSysPlayer** ppTSP, int nCnt, int nStartIndex = 0 )
	TSYS_GETTEAMEX,
	TSYS_GETPLAYEREX,
	TSYS_REQUEST,		//GetTSTeam(int nIndex)
	TSYS_INVITE,		//GetTSPlayer(int nIndex)

	TSYS_ADDTST,		//AddTSTeam(int uin, BYTE byMax, BYTE byCur, WORD wInfo, BYTE* szLimit, int* szMember, char* szTxt)
	TSYS_ADDTSP,		//AddTSPlayer(int uin, WORD wInfo)

	TSYS_CHGTST,
	TSYS_CHGTSP,

	TSYS_CLEARTST,
	TSYS_CLEARTSP,

	//游戏服务器到中心
	//TSYS_CHGTSTMEM,
	//TSYS_ADDTEAM_LEAD,	//去掉求组信息
	TSYS_ADDTEAM,		//去掉求组信息,去掉招募信息
	TSYS_CHGLEAD,		//队长改变,修改UIN
	TSYS_LEAVETEAM,		//最后一人离开队伍
	TSYS_LEAVEGAME,		//去掉求组信息,去掉招募信息

	//中心到游戏服务器
	TSYS_RTL_REQUEST,	//InviteSucceed(int uin, int nObjUin);游戏服务器新加玩家向无队伍人员申请入队的功能
	TSYS_RTL_INVITE,	//RequestSucceed(int uin, int nObjUin);

	//中心到客户端
	TSYS_SEND_TST,
	TSYS_SEND_TSP,

	TSYS_SEND_TSTEX,
	TSYS_SEND_TSPEX,
};

class PacktTSysT
{
public:
	int			nIndex;		//识别号
	BYTE		byLimit;	//上限人数
	BYTE		byCurNum;	//当前人数
	WORD		wInfo;		//组队地图编号
	int			nUin;		//队长ID
	BYTE		byJobLvLimit[8];		//0
	BYTE		bLineIndex;
	char		szName[16];
	
};

class PacktTSysP
{
public:
	int			nIndex;
	WORD		wInfo;		//组队地图编号
	int			nUin;		//玩家ID
	BYTE		bLineIndex;
	char		szName[16];
};

struct	TEAM_SYSTEM_TSYS_ADDTST_Data
{
	BYTE	byMax;
	BYTE	byCur;
	WORD	wInfo;
	BYTE	byLimit[8];
	int		szMember[6];
	char	szTxt[1];
};

struct	TEAM_SYSTEM_TSYS_ADDTSP_Data
{
	WORD	wInfo;
	char	szTxt[1];
};

struct	TEAM_SYSTEM_TSYS_CHGTST_Data
{
	BYTE	byMax;
	WORD	wInfo;
	BYTE	byLimit[8];
	char	szTxt[1];
};
struct	TEAM_SYSTEM_TSYS_CLEARTST_Data
{
	int		nIndex;
};
struct	TEAM_SYSTEM_TSYS_CLEARTSP_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_CHGTSP_Data
{
	int		nIndex;
	WORD	wInfo;
	char	szTxt[1];
};
struct	TEAM_SYSTEM_TSYS_CHGTSTMEM_Data
{
	int		nMemUin;
	BYTE	byJoin;
};
struct	TEAM_SYSTEM_TSYS_ADDTEAM_Data
{
	int		nLeadID;
};
struct	TEAM_SYSTEM_TSYS_LEAVETEAM_Data
{
	int		nLeadID;
};

struct	TEAM_SYSTEM_TSYS_CHGLEAD_Data
{
	int		nNewUin;
};

struct	TEAM_SYSTEM_TSYS_GETTEAMS_Data
{
	int		nCnt;
	int		nStartIndex;
};

struct	TEAM_SYSTEM_TSYS_GETPLAYERS_Data
{
	int		nCnt;
	int		nStartIndex;
};

struct	TEAM_SYSTEM_TSYS_GETTEAMEX_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_GETPLAYEREX_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_REQUEST_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_INVITE_Data
{
	int		nIndex;
};

struct	TEAM_SYSTEM_TSYS_SEND_TST_Data
{
	WORD		wCnt;
	PacktTSysT	tst[1];
};

struct	TEAM_SYSTEM_TSYS_SEND_TSP_Data
{
	WORD		wCnt;
	PacktTSysP	tsp[1];
};

struct	TEAM_SYSTEM_TSYS_SEND_TSTEX_Data
{
	int			nIndex;
	char		szInfo[1];
};

struct	TEAM_SYSTEM_TSYS_SEND_TSPEX_Data
{
	int			nIndex;
	char		szInfo[1];
};

struct	TEAM_SYSTEM_TSYS_RTL_REQUEST_Data
{
	int		nUinFrom;	//请求者(单人)
	int		nUinTo;		//被询问者(任意,弹出询问)
};

struct	TEAM_SYSTEM_TSYS_RTL_INVITE_Data
{
	int		nUinFrom;	//邀请者(队员/长)
	int		nUinTo;		//被邀请者(单人,弹出提示)
};



#pragma pack (pop)