#pragma once

#include "item_packet.h"
#include "char_packet.h"
#include "slave_packet.h"
#include "skill_packet.h"

#pragma pack (push)
#pragma pack (1)

#define MAX_NAMELEN 20

#define PLAYERINFO_VIEWER_ITEM 44902

enum EPLAYERINFO_VIEWER_CMD
{
	PLAYERINFO_VIEWER_CHARACTERDATA,
	PLAYERINFO_VIEWER_ITEMS,
	PLAYERINFO_VIEWER_SLAVES,
	PLAYERINFO_VIEWER_SKILLS,

	PLAYERINFO_VIEWER_INPUT,
	PLAYERINFO_VIEWER_REQUEST,
	PLAYERINFO_VIEWER_REQUEST_RESULT,
};

struct PLAYERINFO_VIEWER_PLAYERINFO_VIEWER_CHARACTERDATA_Data
{
	char szName[MAX_NAMELEN];		//	角色名
	DWORD dwUin;					//	id
	BYTE bJob;						//	职业
	BYTE bCountry;					//	国家
	BYTE bLevel;					//	等级
	int nBagGold;					//	包裹银两
	BYTE bSex;						//  性别
	int	nHonor;						//  名望
	int nPK;						//	PK值
	int nMaxBagCount;				//  包裹容量
	int	nSlaveSlotCount;			//  宠物包裹容量

	//	一级属性
	WORD wStr;
	WORD wCon;
	WORD wInt;
	WORD wDex;
	WORD wWis;
	WORD wAttribPoint;

	//	二级属性
	FightParam	fparamW;			
	FightParam	fparamN;

	DWORD dwMaxHp;
	DWORD dwMaxMp;
	DWORD dwExp;
	DWORD dwNextExp;
	DWORD dwOnlineScore;
	WORD  wActiveTitle;
	char  szHomeName[MAX_HOMENAME];
	BYTE  bmarryFlag;
	char  marryName[48];
	DWORD dwIntimacyVal;
	WORD  wPkCount;
	BYTE  bDrivePow[4];
	int   nBindmoney;			//绑定银两
	int	  nUnbindGold;			//未绑定元宝
	int	  nBindGold;			//绑定元宝
	int	  nFishMoney;
};

struct PlayerInfoItem
{
	CLIENTITEM xItem;
	int nExtendInfoSize;
	int nExpExtendInfoSize;
	int nBindInfoSize;
	int nGemsDataSize;
	int nJudgeDataSize;
	int nTrumpSPDataSize;
	int nCarveDataSize;
	int nPlusUpInfoSize;
	int nWingInfoSize;
	int nGemValue;
	BYTE bItemData[1];
};

struct PLAYERINFO_VIEWER_PLAYERINFO_VIEWER_ITEMS_Data
{
	int nItemCount;
	BYTE bItemsData[1];
};

struct PLAYERINFO_VIEWER_PLAYERINFO_VIEWER_SLAVES_Data
{
	int nSlaveCount;
	SlaveDataEx xSlavesData[1];
};

struct PLAYERINFO_VIEWER_PLAYERINFO_VIEWER_SKILLS_Data
{
	int nSkillCount;
	SKILLITEM xSkillItem[1];
};

struct PLAYERINFO_VIEWER_PLAYERINFO_VIEWER_REQUEST_Data
{
	DWORD dwUin;
};

enum EPLAYERINFO_VIEWER_REQUEST_RESULT_CODE
{
	PLAYERINFO_VIEWER_REQUEST_RESULT_CODE_SUCCESSFUL = 0,
	PLAYERINFO_VIEWER_REQUEST_RESULT_CODE_PLAYER_NOT_EXISTS = -1,
	PLAYERINFO_VIEWER_REQUEST_RESULT_CODE_USE_ITEM_FAILED = -2,
};

struct PLAYERINFO_VIEWER_PLAYERINFO_VIEWER_REQUEST_RESULT_Data
{
	EPLAYERINFO_VIEWER_REQUEST_RESULT_CODE eCode;
};

#pragma pack (pop)