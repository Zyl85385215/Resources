#ifndef _CONF_PACKET_H_
#define _CONF_PACKET_H_

#include "player_packet.h"
#include "item_packet.h"
#include "item_levelup_packet.h"

#pragma pack (push)
#pragma pack (1)
#define MAX_CUSTOM_CONF_ID				49999
#define MAX_CONFNAME_LEN				32
enum{
	PREDEFINE_CONF_ID_EAST=MAX_CUSTOM_CONF_ID+1,
	PREDEFINE_CONF_ID_SOUTH,
	PREDEFINE_CONF_ID_WEST,
	PREDEFINE_CONF_ID_NORTH,
	PREDEFINE_CONF_ID_CENTER,
	PREDEFINE_CONF_ID_SEA,
	PREDEFINE_CONF_ID_MAX,
};	
enum _CONF{
	CONF_START,
	CONF_OPEN,
	CONF_CREATE,
	CONF_INFO,
	CONF_BUSINESS,
	CONF_SUCCESS,
    CONF_CTENET,
    CONF_KICKOUT,
	CONF_QUESTINFO,
	CONF_SETTING,
	CONF_ADD,
	MEMBER_ON,
	MEMBER_DOWN,
	MEMBER_UPLEVER,
	MEMBER_DOWNLEVER,
	CONF_MONEY,
	CONF_ENEMY,
	CONF_ENEMYINFO,
	CONF_ANTI_GUAJI,//防掛機配置
	ANTI_OPEN,
	ANTI_LOCK,
	ANTI_ANSWER,
	ANTI_UNLOCK,
	CHECK_START,
	CHECK_SPR,
	CHECK_RET,
};

enum _CONFBANK{
	CONF_BANKOPEN,
	CONF_TAKEITEM,
	CONF_STOREITEM,
	CONF_TAKEMONEY,
	CONF_STOREMONEY,
	CONF_BANK_ITEMS,
	CONF_BANK,//暂时无用
	CONF_EXTEND_INFO,
	CONF_BANK_GOLD_CHANGED,
	CONF_BANK_REMOVE,
	CONF_BANK_ADD,
	CONF_TOKEN_INFO,
	CONF_RIDE_INFO,
};
enum
{
	CONF_RELATION_NULL,
	CONF_RELATION_ENEMY,
	CONF_RELATION_FRIEND,
};

enum{
	CONF_POSITION_FRESHMAN,			//新手			0
	CONF_POSITION_NULLI,			//未定义		1
	CONF_POSITION_NULLII,			//未定义		2	
	CONF_POSITION_SOLDIER,			//战士			3	
	CONF_POSITION_DUOZHU,			//舵主			4
	CONF_POSITION_TANGZHU,			//堂主			5
	CONF_POSITION_HUFA,				//护法			6
	CONF_POSITION_SUBHEADER,		//副帮主		7
	CONF_POSITION_HEADER,			//帮主			8
	CONF_POSITION_MAX,
		
};


struct CONF_CHECK_RET_Data 
{
	WORD	wCheckSpr[4];
	BYTE	bOpen;
	bool	bfirst;
	bool	bNext;
};

struct CONF_CHECK_SPR_Data
{
	char	szCheck[10];
	bool	bNext;
};

struct CONF_CONF_CREATE_Data {
	char name[1];
};

struct CONF_CONF_OPEN_Data {
	
};

struct CONF_CONF_INFO_Data {
	DWORD uin;
	DWORD confID;
	BYTE  playerLevel;
	BYTE  playerBusiness;
   	BOOLEAN	playerOnline;
	char positionname[12];
	char playername[1];
};

struct CONF_CONF_ANTI_GUAJI_Data 
{
	bool bOnOff;
	int		nPeriod;
	int		nTimeout;
};

struct CONF_ANTI_OPEN_Data
{
	DWORD dwOverTime;
	BYTE byPreAnswer;
};

struct CONF_ANTI_LOCK_Data
{
	
};

struct CONF_ANTI_ANSWER_Data
{
	BYTE byAnswer;
};

struct CONF_ANTI_UNLOCK_Data
{
	BOOLEAN flag;
	BYTE	byPreAnswer;
};

struct CONF_CONF_KICKOUT_Data{
	DWORD uin;    
};

struct CONF_CONF_CTENET_Data{
	char tenet[1];
};


struct CONF_CONF_BUSINESS_Data
{
	DWORD uin; 
	BYTE playerBusiness;
	char Business[1];
};


struct CONF_CONF_QUESTINFO_Data
{
	DWORD confID;
};

struct CONF_CONF_SETTING_Data
{
	DWORD confID;
	BYTE type;
	BYTE color;
	BOOLEAN query;
	BOOLEAN enemy;
	char name[1];
};

struct CONF_CONF_ADD_Data
{
	DWORD adduin;
};

struct CONF_MEMBER_ON_Data
{
	DWORD uin;
};


struct CONF_MEMBER_DOWN_Data
{
	DWORD uin;
};

struct CONF_MEMBER_UPLEVER_Data
{
	DWORD uin;
	BYTE  Level;
};

struct CONF_MEMBER_DOWNLEVER_Data
{
	DWORD uin;
	BYTE  Level;
};

struct CONF_CONF_MONEY_Data
{
	int money;
	int enemyscount;
	char name[1];
};

struct CONF_CONF_ENEMY_Data
{
	BYTE  isenemy;
	char enemyname[1];
};

struct CONF_CONF_ENEMYINFO_Data
{
	BYTE isenemy;
	char enemyname[1];
};

//////////////////////////////////////////////////////////////////////////
//CONFBANK
struct CONFBANK_CONF_TAKEITEM_Data {
	DWORD	itemIndex;
};


struct CONFBANK_CONF_STOREITEM_Data {
	DWORD	itemIndex;
};

struct CONFBANK_CONF_TAKEMONEY_Data {
	DWORD	money;
};

struct CONFBANK_CONF_STOREMONEY_Data {
	DWORD	money;
};

struct CONFBANK_CONF_BANK_ITEMS_Data {
	BYTE count;
	CLIENTITEM item[1];
};

struct CONFBANK_CONF_BANK_Data {
	BYTE count;
	CLIENTITEM item[1];
};

struct CONFBANK_CONF_EXTEND_INFO_Data {
	DWORD	itemIndex;
	BYTE	slot;
	ItemExtend extend;
};
struct CONFBANK_CONF_TOKEN_INFO_Data {
	DWORD	itemIndex;
	BYTE	slot;
	SlaveItemData slaveData;
};
struct CONFBANK_CONF_RIDE_INFO_Data {
	DWORD	itemIndex;
	BYTE	slot;
	RideItemData rideInfo;
};
struct CONFBANK_CONF_BANK_GOLD_CHANGED_Data {
	INT		Gold;//改变后的Gold
};

struct CONFBANK_CONF_BANK_REMOVE_Data {
	DWORD	itemIndex;
};

struct CONFBANK_CONF_BANK_ADD_Data {
	CLIENTITEM	item;
};

#pragma pack (pop)
#endif