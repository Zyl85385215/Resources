#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif
#include "HorseConfig.h"
#include "SlaveConfig.h"

#define	MAX_SLAVE_CNT	2000
#define MIN_SLAVE_CNT	500

#define	MAX_HORSE_CNT	2000

#pragma pack (push)
#pragma pack (1)

enum
{
	S_SEND_ALL,					//宠物
	S_SEND_INFO,
	S_SEND_EXP,
	S_SEND_REMOVE,
	C_REQUEST_STRENGTHEN,
	C_REQUEST_EXCHANGE,
	S_SEND_ACTIVE,
	C_REQUEST_CHANGENAME,
	C_REQUEST_RELEASE,
	C_REQUEST_LEARN_SKILL,
	C_REQUEST_VARIATION,
	C_REQUEST_ACTIVE,
	C_REQUEST_GET,
	C_REQUEST_QUICKGET,
	S_REQUEST_GET,
	S_REQUEST_QUICKGET,
	C_REQUEST_REVIVEPET,
	C_REQUEST_REVIVEPET_BYGOLD,


	S_SEND_ALLHORSE = 130,		//坐骑
	S_SEND_HORSEINFO,
	S_SEND_HORSEEXP,
	S_SEND_REMOVEHORSE,
	S_SEND_EQUIP,
	S_SEND_CULCHANGE,
	C_REQUEST_FEED,
	C_REQUEST_CULTIVATE,
	C_REQUEST_CULTIVATE_RLT,
	C_REQUEST_EAT,
	C_REQUEST_RELEASE_HORSE,
	C_REQUEST_RIDE,
	C_REQUEST_EQUIP,
	S_SEND_RIDE,

	C_TEST = 255,
};

enum 
{
	SLAVE_STATE_NORMAL,
	SLAVE_STATE_NEW,
	SLAVE_STATE_CHANGE,
	SLAVE_STATE_RELEASE,
	SLAVE_STATE_EXCHANGE,		//进化时模板要变
};

struct SlaveSendInfo
{
	DWORD	dwUin;
	DWORD	dwID;
	char	szName[20];
	int		nType;
	BYTE	bySlot;
	BYTE	byLevel;
	BYTE	byVariatlv;
	DWORD	dwExp;
	int		nHp;
	int		nLifePoint;
	int		nStrengthen[SLAVE_STRENGTHEN_MAX];
	WORD	wSkill[SLAVE_SKILL_MAX];
	BYTE	bySkillLv[SLAVE_SKILL_MAX];
};

struct HorseSendInfo
{
	DWORD	dwUin;
	DWORD	dwID;
	char	szName[20];
	int		nType;
	BYTE	bySlot;
	BYTE	byLevel;
	DWORD	dwExp;
	int		nEatLimt;
	int		nPotential[HORSE_CULTIVATE_MAX];
};

struct JGSLAVE_S_SEND_ALL_Data
{
	int nCnt;
	SlaveSendInfo szInfo[1];
};

struct JGSLAVE_S_SEND_INFO_Data
{
	SlaveSendInfo Info;
};

struct JGSLAVE_S_SEND_CULCHANGE_Data
{
	DWORD dwID;
	int szVal[HORSE_CULTIVATE_MAX];
};

struct JGSLAVE_C_REQUEST_REVIVEPET_Data
{
	DWORD dwID;
};

struct JGSLAVE_C_REQUEST_CHANGENAME_Data
{
	DWORD dwID;
	char szName[20];
};

struct JGSLAVE_C_REQUEST_RIDE_Data
{
	bool ifOn;
};

struct JGSLAVE_C_REQUEST_RELEASE_Data
{
	DWORD dwID;
};

struct JGSLAVE_C_REQUEST_STRENGTHEN_Data
{
	DWORD dwID;
	BYTE	bySel;
};

struct JGSLAVE_C_REQUEST_EXCHANGE_Data
{
	DWORD dwID;
};

struct JGSLAVE_C_REQUEST_VARIATION_Data
{
	DWORD dwID;
	int	szID[4];
	int nCnt;
};

struct JGSLAVE_C_REQUEST_LEARN_SKILL_Data
{
	DWORD dwPetID;
	int	nItemID;
};

struct JGSLAVE_C_REQUEST_ACTIVE_Data
{
	DWORD dwID;
};


struct JGSLAVE_S_SEND_ACTIVE_Data
{
	DWORD dwID;
	DWORD dwMapID;
};

struct JGSLAVE_S_SEND_EXP_Data
{
	DWORD dwID;
	DWORD dwExp;
	BYTE byLevel;
};

struct JGSLAVE_S_SEND_REMOVE_Data
{
	DWORD id;
};

struct JGSLAVE_S_SEND_ALLHORSE_Data
{
	int nCnt;
	HorseSendInfo szInfo[1];
};

struct JGSLAVE_S_SEND_HORSEINFO_Data
{
	HorseSendInfo Info;
};

struct JGSLAVE_S_SEND_HORSEEXP_Data
{
	DWORD dwID;
	DWORD dwExp;
	BYTE byLevel;
};

struct JGSLAVE_S_SEND_REMOVEHORSE_Data
{
	DWORD id;
};

struct JGSLAVE_S_SEND_EQUIP_Data
{
	DWORD dwID;
	bool ifRide;
};

struct JGSLAVE_S_SEND_RIDE_Data
{
	bool ifRide;
};

struct JGSLAVE_C_REQUEST_FEED_Data
{
	DWORD dwID;
	bool ifSenior;
	int nCnt;
};

struct JGSLAVE_C_REQUEST_CULTIVATE_Data
{
	DWORD dwID;
	BYTE  byType;
};

struct JGSLAVE_C_REQUEST_CULTIVATE_RLT_Data
{
	DWORD dwID;
	bool ifSure;
};

struct JGSLAVE_C_REQUEST_EAT_Data
{
	DWORD dwID;
	int	 szID[4];
	int nCnt;
};

struct JGSLAVE_C_REQUEST_RELEASE_HORSE_Data
{
	DWORD dwID;
};

struct JGSLAVE_C_REQUEST_EQUIP_Data
{
	DWORD dwID;
};

struct JGSLAVE_C_REQUEST_GET_Data
{
	int nGetType;
};

struct JGSLAVE_C_REQUEST_QUICKGET_Data
{
	int nGetType;
};

struct JGSLAVE_S_REQUEST_GET_Data
{
	int nSID;
};

struct JGSLAVE_S_REQUEST_QUICKGET_Data
{
	int	nSID[10];
};

struct JGSLAVE_C_REQUEST_REVIVEPET_BYGOLD_Data
{
	DWORD dwID;
};

#pragma pack (pop)