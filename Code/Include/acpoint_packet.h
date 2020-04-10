#ifndef _ACPOINT_PACKET_H_
#define _ACPOINT_PACKET_H_

#include "AccountPointDefine.h"

#pragma pack (push)
#pragma pack (1)

//add 2005/10/17

enum
{
	ACPOINT_GET_POINT,          //�������Ϸ�в�ѯ����
	ACPOINT_PLAYER_POINT,       //����ʺ��еĽ�Ԫ������Ԫ��
	ACPOINT_TRADE_POINT,        //�������Ϸ�н��׵���
	ACPOINT_TRADE_RELT,         //���׽��
	ACPOINT_SYSTRADE_POINT,     //��ϵͳ���׵���
	ACPOINT_SYSTRADE_RELT,      //���׽��
	ACPOINT_ACCOUNTERROR,		
	ACPOINT_QUERY_CLUB,         //��ѯ�Ƿ��Ա
	ACPOINT_CLUB,               //�Ƿ�Ϊ��Ա

	ACPOINT_GETLEVEL,           //��ȡ��ҵȼ�
	ACPOINT_LEVEL,              //������ҵȼ�
	ACPOINT_LOGIN,              //��ҵ�½
	ACPOINT_LOGIN_RESULT,       //��ҵ�½���
	CHECKGM,
	CHECKGM_RESULT,

	ACPOINT_SUB_COIN,			//�۵�
	ACPOINT_SUB_RELT,           //�۵���

	GM_BLOCKPLAYER,				//GM������ʺ�
	GM_FORBIDPLAYER,

	CHECKFORBID,
	CHECKFORBID_RESULT,
	CHECKADULT,
	ACPOINT_ADD_POINT,         //��֮ǰ��ӣ�����
	CHECK_ITEM,
	ACPOINT_ADD_ITEM,
	ACPOINT_DEL_ITEM,
	GET_REBATE_INFO,

	ACCOUNT_EVENT_LEVELUP,		//�����¼�

	MARKET_SUB_COIN,            // �������ļ��ٻ���
	ITEM_MARKET_SUB_COIN_RLT,  //  �������ļ��ٻ��ҽ�� 
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
{//	�����ע�������󣬲����˺ŷ����������ַ���ƥ��
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

//	�޸��˸ð��Ľṹ����ƥ���µĵ�ȯ���ѷ�ʽ
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