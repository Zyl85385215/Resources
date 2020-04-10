#ifndef _ACPOINT_PACKET_H_
#define _ACPOINT_PACKET_H_

#include "AccountPointDefine.h"

#pragma pack (push)
#pragma pack (1)

//add 2005/10/17

enum
{
	ACPOINT_GET_POINT,          //玩家在游戏中查询点数
	ACPOINT_PLAYER_POINT,       //玩家帐号中的金元宝、银元宝
	ACPOINT_TRADE_POINT,        //玩家在游戏中交易点数
	ACPOINT_TRADE_RELT,         //交易结果
	ACPOINT_SYSTRADE_POINT,     //与系统交易点数
	ACPOINT_SYSTRADE_RELT,      //交易结果
	ACPOINT_ACCOUNTERROR,		
	ACPOINT_QUERY_CLUB,         //查询是否会员
	ACPOINT_CLUB,               //是否为会员

	ACPOINT_GETLEVEL,           //获取玩家等级
	ACPOINT_LEVEL,              //返回玩家等级
	ACPOINT_LOGIN,              //玩家登陆
	ACPOINT_LOGIN_RESULT,       //玩家登陆结果
	CHECKGM,
	CHECKGM_RESULT,

	ACPOINT_SUB_COIN,			//扣点
	ACPOINT_SUB_RELT,           //扣点结果

	GM_BLOCKPLAYER,				//GM封玩家帐号
	GM_FORBIDPLAYER,

	CHECKFORBID,
	CHECKFORBID_RESULT,
	CHECKADULT,
	ACPOINT_ADD_POINT,         //这之前别加！！！
	CHECK_ITEM,
	ACPOINT_ADD_ITEM,
	ACPOINT_DEL_ITEM,
	GET_REBATE_INFO,

	ACCOUNT_EVENT_LEVELUP,		//升级事件

	MARKET_SUB_COIN,            // 交易中心减少货币
	ITEM_MARKET_SUB_COIN_RLT,  //  交易中心减少货币结果 
	CHECK_NEW_ITEMBANK,
};

enum
{
	TRADE_POINT_SUCCESS,
	TRADE_POINT_ERROR,
	TRADE_POINT_LACK,
};
enum
{
	ACPOINT_LOG_UIN_SYSTEM,
	ACPOINT_LOG_UIN_GROUND,		
};

enum ESUB_COIN_ACTION_TYPE
{//	添加请注意加在最后，并且账号服务器进行字符串匹配
	SUB_COIN_ACTION_TYPE_BUY_ITEM,
	SUB_COIN_ACTION_TYPE_DO_QUEST,
	SUB_COIN_ACTION_TYPE_REVIVE,
	SUB_COIN_ACTION_TYPE_MASTERSYS_EXCHAGE_POINT,
	SUB_COIN_ACTION_TYPE_AUCTION,
	SUB_COIN_ACTION_TYPE_ITEM_TRANSFER,
	SUB_COIN_ACTION_TYPE_TRADE,
	SUB_COIN_ACTION_TYPE_SKILL,
	SUB_COIN_ACTION_TYPE_TRANSPORT_ADD,
	SUB_COIN_ACTION_TYPE_SELL_ITEM,
	SUB_COIN_ACTION_TYPE_ITEMMARKET_USER_BUY,

	PAY_FROM_TP_SINCE		= 100,
	PAY_FROM_TP_DOWNJOY,
	PAY_FROM_TP_UC,
	PAY_FROM_TP_PPS,
	PAY_FROM_TP_TB,
	PAY_FROM_TP_KY,
	PAY_FROM_TP_91,
	SUB_COIN_ACTION_TYPE_MAX,
};

struct GAMESVRQUERY_ACPOINT_GET_POINT_Data
{
//	int uin;
	char bActive;
};

struct GAMESVRQUERY_ITEM_MARKET_SUB_COIN_RLT_Data
{
	DWORD dwItemID;
	BYTE  byRlt;

	int   nGold[GOLD_TYPE_MAX];
	int   nSilver;
	BYTE  byMark;
};

struct GAMESVRQUERY_ACPOINT_PLAYER_POINT_Data 
{
	int   nGold[GOLD_TYPE_MAX];
	int   nSilver;
	int	  nSPGold;
};

struct GAMESVRQUERY_ACPOINT_TRADE_POINT_Data 
{
	int point;
	int uin;
	int index;
};

struct GAMESVRQUERY_ACPOINT_TRADE_RELT_Data 
{
	int currPoint;
	int addPoint;
};

struct GAMESVRQUERY_ACPOINT_SYSTRADE_POINT_Data 
{
	int point;
	int index;
};

struct GAMESVRQUERY_ACPOINT_SYSTRADE_RELT_Data 
{
	int currPoint;
	int addPoint;
};

struct GAMESVRQUERY_ACPOINT_CLUB_Data 
{
	DWORD member;
};


struct GAMESVRQUERY_ACPOINT_LEVEL_Data 
{
	int uin;
	int level;
};

struct GAMESVRQUERY_ACPOINT_LOGIN_Data 
{
	int uin;
	int level;
};

struct GAMESVRQUERY_ACPOINT_LOGIN_RESULT_Data 
{
	int uin;
	int rlt;
};


struct GAMESVRQUERY_CHECKGM_RESULT_Data
{
	DWORD uin;
	BYTE  btGM;
};

struct GAMESVRQUERY_CHECKADULT_Data
{
	DWORD uin;
	bool  blAdult;
};

//	修改了该包的结构用来匹配新的点券消费方式
struct GAMESVRQUERY_ACPOINT_SUB_COIN_Data
{
	BYTE	bAction;
	int		nGold[GOLD_TYPE_MAX];
	int		nSilver;
	int		nSPGold;
	char	szData[1];
};

struct GAMESVRQUERY_MARKET_SUB_COIN_Data
{
	BYTE	bAction;
	int		nGold[GOLD_TYPE_MAX];
	int		nSilver;
	DWORD   dwItemId;
	BYTE    byMark;
	char	szData[1];
};

struct GAMESVRQUERY_ACPOINT_SUB_RELT_Data 
{
	int nGold[GOLD_TYPE_MAX];
	int nSilver;
	int rlt;
	int nVal;
};

struct GAMESVRQUERY_GM_BLOCKPLAYER_Data
{
	DWORD uin;
	int	  nDisSecond;
};

struct GAMESVRQUERY_GM_FORBIDPLAYER_Data
{
	DWORD uin;
	int	  nDisSecond;
};

struct GAMESVRQUERY_CHECKFORBID_RESULT_Data
{
	DWORD uin;
	int	  nDisSecond;
};

struct GAMESVRQUERY_ACPOINT_ADD_POINT_Data
{
	DWORD uin;
	int	  nGold;
	int   nSilver;
};

struct GAMESVRQUERY_ACPOINT_ADD_ITEM_Data
{
	DWORD	dwOrderNum;
	DWORD	dwItemID;
	BYTE    byItemData;
};

struct GAMESVRQUERY_ACPOINT_DEL_ITEM_Data
{
	DWORD	dwOrderNum;
};

struct GAMESVRQUERY_GET_REBATE_INFO_Data
{
	BYTE btRebate;
	time_t nRebateTime;
};

struct GAMESVRQUERY_ACCOUNT_EVENT_LEVELUP_Data
{
	DWORD	dwLevel;
};

#pragma pack (pop)

#endif