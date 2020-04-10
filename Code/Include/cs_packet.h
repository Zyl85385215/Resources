#ifndef _CS_PACKET_H
#define _CS_PACKET_H

#include <time.h>
#include "home_packet.h"
#include "playercopy_packet.h"

#define MAX_NAMELEN 20

#pragma pack (push)
#pragma pack (1)

//enum
enum _CENTER
{
	GAMESERVER_LOGIN,               //game server login 
	GAMESERVER_LOGOUT,              //game server logout

	CRECV_FROM_PLAYER,              //packet recv from palyer should process player

    CSERVER_TO_GAMESERVER,                
	GAMESERVER_TO_CSERVER,

	CSERVER_TO_UIN,  
	CSERVER_TO_ALLUIN,       
	CSERVER_TO_AREAUIN,      
	CSERVER_TO_COUNTRYUIN,  

	SERVER_TRANSPORT,
	GAMESERVER_STOP,
	
	//update
	CSERVER_TIME_FIX,               //update game server's time by center server's
	GAMESERVER_PARAMETER,
	GAMESERVER_SETEXPTIME,
	GAMESERVER_GMCMD,               //out, forbit and so on
	GAMESERVER_ADDEXP,				//add exp
	CS_ITEMSELLBUY,
	RANK_MINVAL,
	RANK_UIN,
	GAMESERVER_START,				//game server start
	RANK_CHG,
	GAMESERVER_ANTIGUAJI,

	//	web
	WEBSERVER_TO_CSERVER,

	GAMESERVER_SET_SKILL_COMMON_CD,
	GAMESERVER_SET_MAXLEVEL,

	GAMESERVER_SETDROPTIME,

	GAMESERVER_SETSHOPREBATE,
	GAMESERVER_SETREPAIRREBATE,
	GAMESERVER_SETMOHUAREBATE,
	GAMESERVER_SETQIANGHUAREBATE,
	//center server request gameServer to send home join war dlg to client
	CSERVER_TO_HOMEMEM_JOINWAR,
	CSERVER_TO_HOMEMEM_JOINQUEST,

	GAMESERVER_SETCHECKWEB,
	GAMESERVER_OPEN_QASYS,
	GM_WORLD_MESSAGE,
	FORBID_CHANGE,
	CSERVER_TO_HOMEMEM_JOINQUEST2,
	CSERVER_NEEDREFLESH_ITEMBANK,
	GAMESERVER_SETHOMEHONOUR,
	CSERVER_COPY_END_BATTLE,
	GAMESERVER_SETVERSION,
	CSERVER_COPY_ADD_RANK,
	CSERVER_COPY_BATTLE_STATE,
	CSERVER_TO_ACTER_CHANGE,       //中心发游戏的包 用于更新前15名ACTER

	CSERVER_SLAVECOPY_END_BATTLE,
	CSERVER_SLAVECOPY_ADD_RANK,
	CSERVER_SLAVECOPY_BATTLE_STATE,

	GAMESERVER_ADD_LPITEM,
	SEND_JOIN_HOME_SUCCESS,
	SEND_CHECKBUY_LUCKPRICEITEM,
	SEND_BUY_LUCKPRICEITEM_RESULT,
	GAMESERVER_SETOPENHOLEMASTSUCCESS,

	CSOAP_DOWNJOY_PAY_NOTICE,
};

enum _PLAYER_PACKET
{
	//player
	CPLAYER_LOGIN,                  //player login
	CPLAYER_LOGOUT,	                //player logout
	CPLAYER_CHANGE_LINE,            //change server line
	CPLAYER_ENTER_LINE,             //player enter line
	CPLAYER_CREATE,                 //create
	CPLAYER_CREATE_RLT,

	CPLAYER_CHANGE_SEX,             //change sex
	CPLAYER_CHANGE_COUNTRY,         //change country
	CPLAYER_CHANGE_LEVEL,           //change level
	CPLAYER_CHANGE_NAME,            //change name
	CPLAYER_CHANGE_MAP,             //change map
	CPLAYER_CHANGE_JOB,             //change job
	CPLAYER_CHANGE_GOLD,            //change gold
	CPLAYER_CHANGE_PK,              //change pk value
	CPLAYER_CHANGE_TEACCNT,

	CPLAYER_OPERATOR_RESULT,        //operator result
	CPLAYER_ONLINE_LIST,            //uin list in this game server
	CPLAYER_CHECKGM_RESULT,         //check the player is gm

	CPLAYER_DISCONNECT_LINE,        //player diconnect

	CPLAYER_ENTER_INSTANCE,         //player enter instance   

	CREATE_GUILD_RESULT,
	REMOVE_GUILD_INFO,


	CPLAYER_GETSIMPLE,              //
	CPLAYER_SIMPLEINFO_RLT,    
	CPLAYER_REFRESH_SIMPLEINFO,
	CREATE_HOME_RESULT,
	PLAYER_HOME_INFO,				//成员所在帮会的ID、名称、等级、职位、以及本人贡献度等
	PLAYER_HOME_CHANGE,
	PLAYER_REMOVE_HOME,				//成员离开帮会
	PLAYER_MEMBER_CHANGE,

	CPLAYER_HONOUR_ADD,
	CPLAYER_DELETE_CHAR,

	CPLAYER_CHANGE_COUNTRY_STR,		//国家实力
	CPLAYER_REQUEST_GM_LIST,		//申请获得在线GM列表
	CPLAYER_SET_HONOUR_LVL,			//改变官阶

	CPLAYER_LEAGUE_LIST,
	CPLAYER_ADD_LEAGUE,
	CPLAYER_DEL_LEAGUE,
	IP_AWARD,						//IP相关奖励

	CPLAYER_CHANGE_LEVELUP,
	CPLAYER_REQUEST_YUANFEN_LIST,

	PLAYER_COUNTRY_INFO,			//国家信息
	DELETE_OATH_CS,
	CPLAYER_CHANGE_HEAD,

	CPLAYER_SKIN,
	CPLAYER_CHANGE_FLOWER,
	SEND_HOME_PRODUCTS,
	SEND_PRODUCTS_BANK_MONEY_CHANGE,
	SEND_PRODUCTS_LEVEL_CHANGE,
	SEND_PRODUCTS_STATE_CHANGE,

	CPLAYER_ADD_ITEM_MARKET_BILL,	//	充值幻剑币
	PLAYER_OPEN_REQUEST_HOME_CREATE,
	REQUEST_CREATE_HOME_RESULT,     //  响应帮会成功
	CPLAYER_CHANGE_TOTAL_INTEGER_SCORE,  //change 积分
	CPLAYER_ADD_ENEMY,
	CPLAYER_DEL_ENEMY,
	CPLAYER_ENEMY_LIST,
	CPLAYER_HEIMUYA_CHANGE,
	CPLAYER_DOWNLEVEL,
	CPLAYER_HOMEWAR_OPEN,          //帮会战开启 可以加提示信息等
	CPLAYER_HOME_CONTRIBUTE,
	CPLAYER_SET_DELETE_CHAR_TIME,
	CPLAYER_CANCEL_DELETE_CHAR,
	CPLAYER_SET_HONOUR_LVL_EX,
	CPLAYER_CHANGE_ARENA_SCORE,
	CPLAYER_FLESH_MARKET_BILL,
	CPLAYER_ADD_NA_BILL,
	CPLAYER_FLESH_NA_BILL,

	CPLAYER_CHANGE_ACTEX_POINT,		//
	CPLAYER_VIP_CHANGE,
	CPLAYER_ACTER_POINT,
	CPLAYER_ROSE_DOWN,
	CPLAYER_HOME_BANKMONEY,
	CPLAYER_HOME_GIVEOVER,
	CPLAYER_HOME_DAYS,
	CPLAYER_REDUCE_HOMEBILL, //自动扣会费
	CPLAYER_HOME_REBUILD,
	CPLAYER_HOME_REBUILD_RESULT,
	CPLAYER_HOME_REBUILD_HEIMUYA,
	CPLAYER_HOME_MEMBER_HEIMUYA,

	CPLAYER_HOME_STATES,
	CPLAYER_GET_PLAYER_HOME_INFO,
	CPLAYER_HOME_MEMBER_CONTRIBUTE,
	CPLAYER_COPY_BATTLE_START,
	CPLAYER_COPY_BATTLE_ERROR_RLT,

	CPLAYER_COPY_BATTLE_END,
	CPLAYER_OPEN_COPY_MENU,
	CPLAYER_UPDATE_PLAYER_COPY,
	CPLAYER_REQUEST_COPY_BATTLE_START,
	SEND_HOME_HALL_CHANGE,

	CPLAYER_SLAVECOPY_BATTLE_START,
	CPLAYER_SLAVECOPY_BATTLE_END,
	CPLAYER_OPEN_SLAVECOPY_MENU,
	CPLAYER_UPDATE_SLAVE_COPY,
	CPLAYER_REQUEST_SLAVECOPY_BATTLE_START,
	CPLAYER_HOME_BANKGOLD,
	CPLAYER_HOME_GIVEGOLDOVER,
	CPLAYER_CHANGE_MARRY_POINT,
	SEND_MARRY_RING_RANK_CHANGE,
	CPLAYER_CHANGE_FLOWER_TOTAL_POINT,
	CPLAYER_CHANGE_FLOWER_DAY_POINT,
	CPLAYER_SEND_FLOWER_POINT,
	CPLAYER_CHANGE_FLOWER_WEEK_POINT,
	CPLAYER_CHANGE_FLOWER_MONTH_POINT,
	CPLAYER_REQUEST_COPY_RANK_AWARD,
	CPLAYER_CHANGE_BOOK_PIECE,
	CPLAYER_GET_ROBOT_INFO,
	CPLAYER_REDUCE_ROBOT_BOOK_PIECE,
	CPLAYER_OPEN_FIRST_MENU,
};

enum _RESULT
{
	RESULT_NO_GAMESERVER,           //game server disconnect
	RESULT_FULL_PLAYERS,            //game server is full	
};

enum _GMCMD
{
	GMCMD_KICK_PLAYER,
	GMCMD_FORBID_PLAYER,
};

enum _SERVERLINE
{
	REQUEST_CHANGE_SERVER_LINE,     //let client request to change server line
	ENTER_NEW_SERVER_LINE,	        //enter new server line
	PULL_PLAYER,                    //pull player to a line
	DISCONNECT_LINE,                //notify game server to disconnect
	ACTIVE_LINE,					//active line
};

enum _CS_ITEMSELLBUY
{
	GS_REQUEST_SIMPLE_LIST,			//game server send a request when startup
	GS_DELETE_ITEM,			//game server to cs
	GS_UPDATE_ITEM,					//game server to cs
	GS_ADD_STATE_ITEM,					//game server to cs

	SIMPLE_SELL_INFO,				//cs send list to game server
	P_ADD_SELL_INFO,				//game server to cs, player add sell
	//	P_UPDATE_SELL_INFO,				//game server to cs, player change price
	P_DELETE_SELL_INFO,				//game server to cs, player cancel sell
	P_BUY_ITEM,						//game server to cs
	NOTIFY_ADD_SELL_INFO,			//cs to all game servers
	NOTIFY_UPDATE_SELL_INFO,		//cs to all game servers
	NOTIFY_DELETE_SELL_INFO,		//cs to all game servers
	//	ITEM_ADD_SUCCESS,				//cs to game server
	ITEM_BUY_SUCCESS,				//cs to game server
	ITEM_CANCEL_SUCCESS,
	ITEM_ADD_FAILED,
	ITEM_BUY_FAILED,				//cs to game server
	ITEM_CANCEL_FAILED,
	DELETE_CHAR_ITEM,
};

enum _SELL_STATE
{
	STATE_ONSELL,
	STATE_CRITICAL,
	STATE_SELLED = 10,
	STATE_SELLED_ITEM_CLEAR = 11,
	STATE_SELLED_MONEY_CLEAR = 20,
	STATE_SELLED_BOTH_CLEAR = 21,
};

struct IX_ItemKey 
{
	DWORD	uinSell;	
	DWORD	itemIndex;
	BYTE	itemSlot;
	friend bool operator < (IX_ItemKey const& k1, IX_ItemKey const& k2)
	{
		if (k1.uinSell != k2.uinSell)
		{
			return (k1.uinSell < k2.uinSell);
		}
		else if (k1.itemIndex != k2.itemIndex)
		{
			return (k1.itemIndex < k2.itemIndex);
		} 
		else
		{
			return (k1.itemSlot < k2.itemSlot);
		}
	};
};

struct SellBuyInfo 
{	
	DWORD	uinBuy;	
	int		price;
	BYTE	state;
	BYTE	type;	//金币或元宝，，元宝0，金币1
	time_t	beginTime;	
};

struct SellItem
{
	IX_ItemKey key;
	SellBuyInfo info;
	char	sellerName[20];
};

//packet struct
//gserver - cserver
struct CENTERSERVER_GAMESERVER_LOGIN_Data
{
	char    szIP[16];
	WORD    wServerIndex;
    char    szServerName[1];
};

struct CENTERSERVER_GAMESERVER_STOP_Data
{
    bool rlt;
};

struct CENTERSERVER_GAMESERVER_PARAMETER_Data
{
	int		nExpTimes;
	int		nMaxLevel;
	bool	bAddExp;
	int		nDropTimes;
	int		nShopRebate;		// 商店折扣 0-100
	int		nRepairRebate;			// 修理折扣 0-100
	int		nMoHuaRebate;			// 魔化折扣 0-100
	int		nQiangHuaRebate;		// 强化折扣 0-100
	BYTE	m_bFirstLevel;
	int		nCheckWeb;
	bool    bHomeHonour;
	bool    bOpenHoleMastSuccess;
	int		nVersion;
};

struct CENTERSERVER_GM_WORLD_MESSAGE_Data
{
	bool IsAdd;
	int index;
	WORD ItemId[3];
	int nGold;
	char szMsg[1];
};

struct CENTERSERVER_FORBID_CHANGE_Data
{
	bool IsAdd;
	char szMsg[1];
};

struct CENTERSERVER_GAMESERVER_SETEXPTIME_Data
{
	int nExpTime;
};

struct CENTERSERVER_GAMESERVER_SETCHECKWEB_Data
{
	int nVal;
};

struct CENTERSERVER_GAMESERVER_SETVERSION_Data
{
	int nVal;
};

struct CENTERSERVER_GAMESERVER_SETOPENHOLEMASTSUCCESS_Data
{
	bool  bOpen;
};
struct CENTERSERVER_GAMESERVER_OPEN_QASYS_Data
{
	int nSpaceTime;
	int nStartTime;
	BYTE	bType;
};

struct CENTERSERVER_GAMESERVER_ADD_LPITEM_Data
{
	DWORD	dwItem;
	int		nPrice;
	int		nPMin;
	int		nPMax;
	char	szName[1];
};

struct CENTERSERVER_GAMESERVER_SETDROPTIME_Data
{
	int nDropTime;
};

struct CENTERSERVER_GAMESERVER_SETSHOPREBATE_Data
{
	int nRebate;
};
struct CENTERSERVER_GAMESERVER_SETREPAIRREBATE_Data
{
	int nRebate;
};
struct CENTERSERVER_GAMESERVER_SETMOHUAREBATE_Data
{
	int nRebate;
};
struct CENTERSERVER_GAMESERVER_SETQIANGHUAREBATE_Data
{
	int nRebate;
};


struct CENTERSERVER_GAMESERVER_SET_MAXLEVEL_Data
{
	int nMaxLvl;
};

struct CENTERSERVER_GAMESERVER_GMCMD_Data
{
	DWORD  uin;
	BYTE   CmdType;
};

struct CENTERSERVER_GAMESERVER_ADDEXP_Data
{
	bool	bAddExp;
};

struct CENTERSERVER_GAMESERVER_SETHOMEHONOUR_Data
{
	bool    bSetHomeHonour;
};

struct CENTERSERVER_CSERVER_COPY_END_BATTLE_Data 
{
	DWORD dwWinUin;
	DWORD dwLoseUin;
	SimplePlayerCopyData pWinData;
};

struct CENTERSERVER_CSERVER_SLAVECOPY_END_BATTLE_Data 
{
	DWORD dwWinUin;
	DWORD dwLoseUin;
	SimpleSlaveCopyData pWinData;
};

struct CENTERSERVER_CSERVER_COPY_ADD_RANK_Data 
{
	SimplePlayerCopyData pNewData;
};

struct CENTERSERVER_CSERVER_SLAVECOPY_ADD_RANK_Data 
{
	SimpleSlaveCopyData pNewData;
};

struct CENTERSERVER_CSERVER_COPY_BATTLE_STATE_Data 
{
	DWORD dwUin1;
	DWORD dwUin2;
	bool bInBattle1;
	bool bInBattle2;
};

struct CENTERSERVER_CSERVER_SLAVECOPY_BATTLE_STATE_Data 
{
	DWORD dwUin1;
	DWORD dwUin2;
	bool bInBattle1;
	bool bInBattle2;
};

struct ActerData
{
	char  szName[20];
	DWORD dwValue;
	BYTE  bPosition; //排名
};

struct CENTERSERVER_CSERVER_TO_ACTER_CHANGE_Data
{
	//BYTE bActerNum;
	//ActerData pData[1];
	char  szName[20];
	DWORD dwValue;
};

struct CENTERSERVER_CRECV_FROM_PLAYER_Data
{
    BYTE     data[1];
};

struct CENTERSERVER_CSERVER_TIME_FIX_Data
{
	time_t   tmTime;
};

//gserver - gserver
struct CENTERSERVER_CSERVER_TO_UIN_Data
{
	DWORD    uin;
	BYTE     data[1];
};

struct CENTERSERVER_CSERVER_TO_AREAUIN_Data
{
	DWORD    uin;
	BYTE     data[1];
};

struct CENTERSERVER_CSERVER_TO_ALLUIN_Data
{
	BYTE     data[1];
};

struct CENTERSERVER_CSERVER_TO_COUNTRYUIN_Data
{
	DWORD    uin;
	BYTE     country;
	BYTE     data[1];
};

struct CENTERSERVER_CSERVER_TO_GAMESERVER_Data
{
	BYTE     data[1];
};

struct CENTERSERVER_GAMESERVER_TO_CSERVER_Data
{
	DWORD    uin;
	BYTE     data[1];
};

//client - cserver
//player

struct CRECV_FROM_PLAYER_CPLAYER_GETSIMPLE_Data
{
	DWORD   uin;
	BYTE    bLineIndex;
	char	szIP[16];
};

struct CRECV_FROM_PLAYER_CPLAYER_SIMPLEINFO_RLT_Data
{
	DWORD  uin;
	bool   filed;
};

struct CRECV_FROM_PLAYER_CPLAYER_CREATE_RLT_Data
{
	DWORD uin;
	DWORD dwUin;
	bool  filed;
};

struct CRECV_FROM_PLAYER_CPLAYER_DELETE_CHAR_Data
{
	DWORD dwAccountUin;
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_SET_DELETE_CHAR_TIME_Data
{
	DWORD dwAccountUin;
	DWORD dwUin;
	BYTE  btLineIndex;
};

struct CRECV_FROM_PLAYER_CPLAYER_CANCEL_DELETE_CHAR_Data 
{
	DWORD dwAccountUin;
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_COUNTRY_STR_Data
{
	DWORD	dwUin;
	BYTE	bCountryType;
	int		nCountryStr;
};

struct CRECV_FROM_PLAYER_CPLAYER_REFRESH_SIMPLEINFO_Data
{
	DWORD uin;
	BYTE  level;
	DWORD exp; 
	DWORD maxhp;
	DWORD hp;
    DWORD mp;
	DWORD maxmp;
	BYTE  job;
	int   nStars;
	int   nFury;
	DWORD dwEquipScore;
};

struct CRECV_FROM_PLAYER_CPLAYER_DISCONNECT_LINE_Data
{
	DWORD uin;
	BYTE  LineIndex;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHECKGM_RESULT_Data
{
    DWORD    uin;
    bool     isGM;
};

struct CRECV_FROM_PLAYER_CPLAYER_LOGIN_Data
{
	DWORD   uin;
	WORD    wServerIndex;
};

struct CRECV_FROM_PLAYER_CPLAYER_LOGOUT_Data
{
	DWORD   uin;
	WORD    wServerIndex;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_LINE_Data
{
	DWORD   uin;
	DWORD	dwAccountUin;
	WORD    wServerIndex;
	char	szAccountId[1];
};

struct CRECV_FROM_PLAYER_CPLAYER_CREATE_Data            
{
	BYTE    bLineIndex;
	DWORD   uin;
	BYTE    bSex;
	BYTE    bCountry;
	BYTE    bHead;
	DWORD   maxhp;
	DWORD   maxmp;
	char    szName[16];
	BYTE	byDefaultSkinType;
	//BYTE	bySkinDress;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_SEX_Data
{
	DWORD   uin;
	BYTE    bSex;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_COUNTRY_Data
{
	DWORD   uin;
	BYTE    bCountry;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_LEVEL_Data
{
	DWORD   uin;
	BYTE    bLevel;
	BYTE		bCanInRank;
	int     nExp;
	BYTE	bExpBall;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_LEVELUP_Data
{
	DWORD   uin;
};



struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_NAME_Data
{
	DWORD   uin;
	char    szName[16];
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_MAP_Data
{
	DWORD   uin;
	int    nMapIndex;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_JOB_Data
{
	DWORD   uin;
	BYTE    bJob;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_GOLD_Data
{
	DWORD   uin;
	int     nGold;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_TOTAL_INTEGER_SCORE_Data
{
	DWORD uin;
	DWORD  dwOnlineTime;
	DWORD  dwScore;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_TEACCNT_Data
{
	DWORD uin;
	int teacCnt;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_ACTEX_POINT_Data
{
	DWORD uin;
	int		nChgVal;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_MARRY_POINT_Data 
{
	DWORD uin;
	int nMarryLevel;
	int nMarryPoint;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_PK_Data
{
	DWORD   uin;
	WORD    wPk;
};

struct CRECV_FROM_PLAYER_SEND_MARRY_RING_RANK_CHANGE_Data 
{
	DWORD	dwUin;
	int nDeleteTitle;
	int nAddTitle;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_ARENA_SCORE_Data 
{
	DWORD uin;
	int nScore;
};

struct CRECV_FROM_PLAYER_CPLAYER_FLESH_MARKET_BILL_Data
{
	DWORD uin;
};

struct CRECV_FROM_PLAYER_CPLAYER_FLESH_NA_BILL_Data
{
	DWORD uin;
};

struct CRECV_FROM_PLAYER_CPLAYER_ROSE_DOWN_Data
{
	DWORD uin;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_BANKMONEY_Data
{
	DWORD   dwUin;
	DWORD   dwHomeBankMoney;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_STATES_Data
{
	DWORD	dwUin;
	DWORD   dwBitStateUL;
};

struct CRECV_FROM_PLAYER_CPLAYER_VIP_CHANGE_Data
{
	DWORD uin;
	bool  bFlag;
};

struct CRECV_FROM_PLAYER_CPLAYER_GET_PLAYER_HOME_INFO_Data
{
	DWORD sendUin;
	DWORD getUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_OPERATOR_RESULT_Data
{
	DWORD    uin;
	BYTE     bRlt;
};

struct CRECV_FROM_PLAYER_CPLAYER_ONLINE_LIST_Data
{
	WORD    wServerIndex;
	BYTE    count;
    DWORD   uin[1];
};

struct CRECV_FROM_PLAYER_CPLAYER_ENTER_INSTANCE_Data
{
    DWORD   uin;
	WORD    line;                 //line id
	WORD    id;                   //instance id
};

struct CRECV_FROM_PLAYER_CREATE_GUILD_RESULT_Data
{
    BYTE  rlt;
	DWORD byuin;
};

struct CRECV_FROM_PLAYER_CREATE_HOME_RESULT_Data
{
	BYTE  rlt;
	DWORD byuin;
	DWORD dwHomeID;
};

struct CRECV_FROM_PLAYER_REQUEST_CREATE_HOME_RESULT_Data
{
	BYTE rlt;
	DWORD byuin;
	DWORD dwHomeID;
};

struct CRECV_FROM_PLAYER_SEND_HOME_PRODUCTS_Data 
{
	DWORD dwUin;
	HomeProducts pProducts[HOME_PRODUCTS_TYPE_MAX];
};

struct CRECV_FROM_PLAYER_SEND_PRODUCTS_BANK_MONEY_CHANGE_Data
{
	DWORD dwUin;
	int	nBankMoney[HOME_PRODUCTS_TYPE_MAX];
	int	nCostCount[HOME_PRODUCTS_TYPE_MAX];
};

struct CRECV_FROM_PLAYER_SEND_PRODUCTS_LEVEL_CHANGE_Data
{
	DWORD dwUin;
	BYTE byLevel[HOME_PRODUCTS_TYPE_MAX];
	int nCostCount[HOME_PRODUCTS_TYPE_MAX];
};

struct CRECV_FROM_PLAYER_SEND_PRODUCTS_STATE_CHANGE_Data
{
	DWORD dwUin;
	BYTE byStates[HOME_PRODUCTS_TYPE_MAX];				//0未开启 1冻结 2激活
};

struct CRECV_FROM_PLAYER_PLAYER_HOME_INFO_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	char  szHomeName[MAX_HOMENAME];
	BYTE  byHomeLevel;
	BYTE  byHomeResourceLevel;
	BYTE  byHomeEnForceLevel;
	BYTE  byDutyIndex;
	DWORD dwPower;
	DWORD dwHomeMoney;
	DWORD dwHomePower;
	DWORD dwHomeMenage;
	DWORD dwHomeHonour;
	DWORD dwHomeLeaderUin;
	int	  nHomeICON;
	char  szHomeLeaderName[MAX_NAMELEN];
	_CreateHome chLock;
	DWORD dwPoint;
	bool  bHomeProffer;
	DWORD dwContributeTime;
	bool bHomeProffer2;
	DWORD dwHomeBankMoney;
	int  nDays;
	DWORD	dwBitHStateUnLock;
	DWORD dwHallID;
	BYTE  bHallDutyIndex;
	char  szHallName[16];
	DWORD dwHomeBankGold;
	DWORD dwHomeBankNoGiveGold;
};

struct CRECV_FROM_PLAYER_PLAYER_HOME_CHANGE_Data
{
	DWORD dwUin;
	BYTE  byHomeLevel;
	BYTE  byHomeResourceLevel;
	BYTE  byHomeEnForceLevel;
	DWORD dwHomeMoney;
	DWORD dwHomePower;
	DWORD dwHomeMenage;
	DWORD dwHomeHonour;
	DWORD dwHomeHonour2;
};

struct CRECV_FROM_PLAYER_PLAYER_MEMBER_CHANGE_Data
{
	DWORD dwUin;
	BYTE  byDutyIndex;
	DWORD dwPower;
	DWORD dwPoint;
};

struct CRECV_FROM_PLAYER_CPLAYER_HONOUR_ADD_Data
{
	DWORD dwUin;
	int	  nDeltaHonour;
	BYTE  bType;
	DWORD dwByUin;
};

struct CRECV_FROM_PLAYER_PLAYER_REMOVE_HOME_Data
{
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_REMOVE_GUILD_INFO_Data
{
	DWORD uin;
	WORD  ID;
};

struct CRECV_FROM_PLAYER_CPLAYER_REQUEST_GM_LIST_Data
{
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_SET_HONOUR_LVL_Data
{
	DWORD dwUin;
	int	nHonourLvl;
};

struct CRECV_FROM_PLAYER_CPLAYER_SET_HONOUR_LVL_EX_Data
{
	DWORD dwUin;
	int nHonourLvl;
};
struct CRECV_FROM_PLAYER_CPLAYER_LEAGUE_LIST_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
	DWORD	dwLeagueCnt;
	DWORD	dwLeagueID[1];
};

struct EnemyHomeInfo{
	DWORD dwEnmeyHomeID;
	DWORD dwTime;
};
struct CRECV_FROM_PLAYER_CPLAYER_ENEMY_LIST_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
	//DWORD   dwTime;
	DWORD	dwLeagueCnt;
	EnemyHomeInfo	pInfo[1];
};

struct CRECV_FROM_PLAYER_CPLAYER_HEIMUYA_CHANGE_Data
{
	DWORD   uin;
	BYTE    bIsHeimuya;
};

struct CRECV_FROM_PLAYER_CPLAYER_DOWNLEVEL_Data
{
	DWORD  dwUin;
	BYTE   bErrorCode;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOMEWAR_OPEN_Data
{
	DWORD   dwUin;
	char	szName[48];
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_CONTRIBUTE_Data
{
	DWORD   dwUin;
	DWORD   dwContributeTime;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_MEMBER_CONTRIBUTE_Data
{
	DWORD   dwUin;
	DWORD   dwContributeTime;
};

struct CRECV_FROM_PLAYER_CPLAYER_UPDATE_PLAYER_COPY_Data
{
	PlayerCopyData  pCopyData;
};

struct CRECV_FROM_PLAYER_CPLAYER_UPDATE_SLAVE_COPY_Data 
{
	SlaveCopyData  pCopyData;
};

struct CRECV_FROM_PLAYER_CPLAYER_REQUEST_COPY_BATTLE_START_Data 
{
	DWORD dwPlayerUin;
	DWORD dwCopyUin;
	bool  bRobot;
};

struct CRECV_FROM_PLAYER_CPLAYER_REQUEST_SLAVECOPY_BATTLE_START_Data 
{
	DWORD dwPlayerUin;
	DWORD dwCopyUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_BANKGOLD_Data
{
	DWORD dwUin;
	DWORD dwHomeBankGold;
	DWORD dwHomeBankNoGiveGold;
};

struct CRECV_FROM_PLAYER_CPLAYER_COPY_BATTLE_END_Data
{
	DWORD dwWinUin;
	DWORD dwLoseUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_SLAVECOPY_BATTLE_END_Data 
{
	DWORD dwWinUin;
	DWORD dwLoseUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_OPEN_COPY_MENU_Data 
{
	DWORD dwUin;
};
struct CRECV_FROM_PLAYER_CPLAYER_OPEN_FIRST_MENU_Data 
{
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_OPEN_SLAVECOPY_MENU_Data 
{
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_COPY_BATTLE_START_Data
{
	DWORD dwPlayerUin;
	PlayerCopyData pCopyData;
};

struct CRECV_FROM_PLAYER_CPLAYER_SLAVECOPY_BATTLE_START_Data 
{
	DWORD dwPlayerUin;
	SlaveCopyData pCopyData;
};

struct CRECV_FROM_PLAYER_CPLAYER_COPY_BATTLE_ERROR_RLT_Data
{
	DWORD dwPlayerUin;
	BYTE bRlt;
};


struct CRECV_FROM_PLAYER_SEND_HOME_HALL_CHANGE_Data
{
	DWORD   dwUin;
	DWORD   dwHallID;
	BYTE    bHallDutyIndex;
	char    szHallName[16];
};

typedef struct CRECV_FROM_PLAYER_CPLAYER_ADD_LEAGUE_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
	DWORD	dwLeagueID;
}CRECV_FROM_PLAYER_CPLAYER_DEL_LEAGUE_Data;

typedef struct CRECV_FROM_PLAYER_IP_AWARD_Data
{
	DWORD	dwUin;
	BYTE	byIPAward;
}CRECV_FROM_PLAYER_IP_AWARD_Data;

struct CRECV_FROM_PLAYER_CPLAYER_REQUEST_YUANFEN_LIST_Data
{
	DWORD	dwUin;
	int		nLevelPix;
	BYTE	bLevelCnt;
	DWORD	dwInfoCnt;
	bool	bDiffSex;
	bool	bSameCountry;
};

struct CRECV_FROM_PLAYER_PLAYER_COUNTRY_INFO_Data
{
	DWORD	dwUin;
	int		nHonourIconIndex;
};

//serverline
struct SERVERLINE_REQUEST_CHANGE_SERVER_LINE_Data
{
	WORD  wServerID;
};

struct SERVERLINE_ENTER_NEW_SERVER_LINE_Data
{
	DWORD uin;
	BYTE  OldLine;
	bool bGoToInstance;
};

struct SERVERLINE_PULL_PLAYER_Data
{
	int   nLine;
};

struct CS_ITEMSELLBUY_GS_REQUEST_SIMPLE_LIST_Data
{	
	WORD  wServerID;
};

struct CS_ITEMSELLBUY_GS_DELETE_ITEM_Data
{	
	IX_ItemKey key;
};

struct CS_ITEMSELLBUY_GS_UPDATE_ITEM_Data
{	
	SellItem item;
};

struct CS_ITEMSELLBUY_GS_ADD_STATE_ITEM_Data
{	
	IX_ItemKey key;
	BYTE val;
};

struct CS_ITEMSELLBUY_SIMPLE_SELL_INFO_Data
{	
	BYTE	count;
	SellItem item[1];
};

struct CS_ITEMSELLBUY_P_ADD_SELL_INFO_Data
{
	SellItem item;
	WORD wServerID;
};

struct CS_ITEMSELLBUY_P_UPDATE_SELL_INFO_Data
{
	SellItem item;
};

struct CS_ITEMSELLBUY_P_DELETE_SELL_INFO_Data
{
	IX_ItemKey key;
	WORD wServerID;
};

struct CS_ITEMSELLBUY_P_BUY_ITEM_Data
{	
	IX_ItemKey key;
	WORD wServerID;
	DWORD dwUin;
};

struct CS_ITEMSELLBUY_NOTIFY_ADD_SELL_INFO_Data
{
	SellItem item;	
};

struct CS_ITEMSELLBUY_NOTIFY_UPDATE_SELL_INFO_Data
{
	SellItem item;
};

struct CS_ITEMSELLBUY_NOTIFY_DELETE_SELL_INFO_Data
{
	IX_ItemKey key;
};

struct CS_ITEMSELLBUY_ITEM_BUY_SUCCESS_Data
{
	IX_ItemKey key;
	DWORD dwUin;
};

struct CS_ITEMSELLBUY_ITEM_CANCEL_SUCCESS_Data
{
	IX_ItemKey key;	
};

struct CS_ITEMSELLBUY_ITEM_ADD_FAILED_Data
{	
	IX_ItemKey key;
};

struct CS_ITEMSELLBUY_ITEM_BUY_FAILED_Data
{	
	DWORD dwUin;
};

struct CS_ITEMSELLBUY_ITEM_CANCEL_FAILED_Data
{	
	DWORD dwUin;
};

struct CS_ITEMSELLBUY_DELETE_CHAR_ITEM_Data
{	
	DWORD dwUin;
};

struct CENTERSERVER_RANK_MINVAL_Data 
{
	BYTE bCountry;
	BYTE bRankType;
	DWORD dwVal1;
	DWORD dwVal2;		//暫時沒用
	//DWORD dwUin;//最小值uin
	//DWORD dwNewUin;//新入榜uin也可能原来就在榜中
};

struct CENTERSERVER_RANK_CHG_Data 
{
	BYTE bCountry;
	BYTE bRankType;
	DWORD dwMinUin;//最小值uin
	DWORD dwNewUin;//新入榜uin
};

struct CENTERSERVER_CSERVER_TO_HOMEMEM_JOINWAR_Data
{
	DWORD dwUin;
};

struct CENTERSERVER_CSERVER_TO_HOMEMEM_JOINQUEST_Data
{
	DWORD dwUin;
	BYTE bType;
};

struct CENTERSERVER_CSERVER_TO_HOMEMEM_JOINQUEST2_Data
{
	DWORD dwUin;
};

struct CENTERSERVER_CSERVER_NEEDREFLESH_ITEMBANK_Data
{
	DWORD dwAccId;
};

struct CENTERSERVER_CSOAP_DOWNJOY_PAY_NOTICE_Data
{
	int		dwMoney;
	char	szAcc[50];
	char	szOrder[50];
	int		nPlat;
	//char	szExt[32];
};

struct CENTERSERVER_RANK_UIN_Data 
{
	BYTE bCnt;
	BYTE bCountry;
	BYTE bRankType;
	DWORD dwUin[1];
};

struct CENTERSERVER_GAMESERVER_ANTIGUAJI_Data
{
	BOOLEAN antiSwitch;
	BYTE antiPeriod;
	BYTE antiOverTime;
};

struct CENTERSERVER_WEBSERVER_TO_CSERVER_Data
{
	BYTE	data[1];
};

struct CENTERSERVER_SEND_JOIN_HOME_SUCCESS_Data
{
	DWORD bReslut;
};
struct CENTERSERVER_SEND_CHECKBUY_LUCKPRICEITEM_Data
{
	DWORD dwUin;
	DWORD dwPrice;
	DWORD dwItemID;
	char  szItemName[64];
};

struct CENTERSERVER_SEND_BUY_LUCKPRICEITEM_RESULT_Data
{
	DWORD dwuin;
	DWORD result;
};

struct CENTERSERVER_GAMESERVER_SET_SKILL_COMMON_CD_Data
{
	DWORD	dwSkillCommonCD;
};

struct CRECV_FROM_PLAYER_DELETE_OATH_CS_Data
{
	DWORD	dwMemberUin;
	DWORD	dwOathID;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_HEAD_Data
{
	DWORD   uin;
	BYTE    bHead;
};

struct CRECV_FROM_PLAYER_CPLAYER_SKIN_Data
{
	DWORD   uin;
	BYTE    bSkin[9];
};

struct CRECV_FROM_PLAYER_CPLAYER_REQUEST_COPY_RANK_AWARD_Data 
{
	DWORD	uin;
	int		nIndex;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_BOOK_PIECE_Data
{
	DWORD uin;
	int   nSkillID;
	int   nPos;
	int   nNum;
};

struct CRECV_FROM_PLAYER_CPLAYER_GET_ROBOT_INFO_Data
{
	int nSkillID;
	int nPos;
	DWORD uin;
};

struct CRECV_FROM_PLAYER_CPLAYER_REDUCE_ROBOT_BOOK_PIECE_Data
{
	DWORD dwUin;
	int   nSkillID;
	int   nPos;
	int   nNum;
};
struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_FLOWER_Data
{
	DWORD   uin;
	int     nFlower;
};

struct CRECV_FROM_PLAYER_CPLAYER_ADD_ITEM_MARKET_BILL_Data
{
	DWORD	dwUin;
	int		nItemMarketBill;
	char	szData[1];
};

struct CRECV_FROM_PLAYER_CPLAYER_ADD_NA_BILL_Data
{
	DWORD  dwUin;
	int    nNaBill;
	int    nNaBindBill;
	char   szData[1];
};

struct CRECV_FROM_PLAYER_PLAYER_OPEN_REQUEST_HOME_CREATE_Data
{
	DWORD   dwUin;
};


struct CRECV_FROM_PLAYER_CPLAYER_ADD_ENEMY_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
	DWORD	dwEnemyID;
	DWORD   dwTime;
};

struct CRECV_FROM_PLAYER_CPLAYER_DEL_ENEMY_Data
{
	DWORD	dwUin;
	DWORD	dwHomeID;
	DWORD	dwEnemyID;
};

struct CRECV_FROM_PLAYER_CPLAYER_ACTER_POINT_Data
{
	DWORD dwSelfUin;
	DWORD dwItemID;	
	BYTE  bNum;
	char  szName[40];
	bool  bSex;
	int   nNum;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_GIVEOVER_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	DWORD dwHomeBankMoney;
	char szName[40];
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_GIVEGOLDOVER_Data
{
	DWORD dwUin;
	DWORD dwHomeID;
	DWORD dwHomeBankGold;
	char szName[40];
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_DAYS_Data
{
	DWORD dwUin;
	int   nDays;
};

struct CRECV_FROM_PLAYER_CPLAYER_REDUCE_HOMEBILL_Data
{
	DWORD dwUin;
	int   nDays;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_REBUILD_Data
{
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_REBUILD_HEIMUYA_Data
{
	DWORD dwUin;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_MEMBER_HEIMUYA_Data
{
	DWORD dwUin;
	bool  bHeimuya;
};

struct CRECV_FROM_PLAYER_CPLAYER_HOME_REBUILD_RESULT_Data
{
	DWORD dwUin;
	bool  bFlag;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_FLOWER_MONTH_POINT_Data
{
	DWORD uin;
	int   nChgVal;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_FLOWER_WEEK_POINT_Data
{
	DWORD uin;
	int   nChgVal;
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_FLOWER_DAY_POINT_Data
{
	DWORD uin;
	int   nChgVal;
};

struct CRECV_FROM_PLAYER_CPLAYER_SEND_FLOWER_POINT_Data
{
	DWORD dwSelfUin;
	int   nMoney;
	int   nBangMoney;
	int   nBindGold;
	int   nGold;
	DWORD dwItemID;
	DWORD dwTicketTime;
	char  szName[20];
};

struct CRECV_FROM_PLAYER_CPLAYER_CHANGE_FLOWER_TOTAL_POINT_Data
{
	DWORD uin;
	int   nChgVal;
};

#pragma pack (pop)
#endif  