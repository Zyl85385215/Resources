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
	char szName[MAX_NAMELEN];		//	��ɫ��
	DWORD dwUin;					//	id
	BYTE bJob;						//	ְҵ
	BYTE bCountry;					//	����
	BYTE bLevel;					//	�ȼ�
	int nBagGold;					//	��������
	BYTE bSex;						//  �Ա�
	int	nHonor;						//  ����
	int nPK;						//	PKֵ
	int nMaxBagCount;				//  ��������
	int	nSlaveSlotCount;			//  �����������

	//	һ������
	WORD wStr;
	WORD wCon;
	WORD wInt;
	WORD wDex;
	WORD wWis;
	WORD wAttribPoint;

	//	��������
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
	int   nBindmoney;			//������
	int	  nUnbindGold;			//δ��Ԫ��
	int	  nBindGold;			//��Ԫ��
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