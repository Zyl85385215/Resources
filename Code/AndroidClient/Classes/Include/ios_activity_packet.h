#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

struct UserDegreePart{
	BYTE byType;
	BYTE byCount;
	bool ifDone;
};

enum
{
	S_SEND_TESTINFO,
	C_ANSWER_QUESTION,
	C_REQUEST_START,
	S_SEND_START_RLT,
	C_REQUEST_GIVEUP,
	S_SEND_RANKINFO,
	C_REQUEST_FOR_GIFT,
	S_SEND_ACDEGREE,
	S_SEND_ACDLV,
	C_REQUEST_DEGREEAWARD,
	C_REQUEST_ACINFO,
	C_REQUEST_ENTERAC,
	S_SEND_ACINFO,
	C_REQUEST_TURNTABLE_START,
	C_REQUEST_TURNTABLE_GIFT,
	S_SEND_TURNTABLE_START,
	S_SEND_OPEN_TURNTABLE,
	S_SEND_FINDNPC,
	C_REQUEST_BOSSINFO,
	S_SEND_BOSSINFO,
	S_SEND_ACHIEVEMENT_ALL,
	S_SEND_ACHIEVEMENT_CHG,
	S_SEND_ACHIEVEMENT_SHOW,
	C_REQUEST_ACHIEVEMENT_GIFT,
	C_SEND_JUMPTONPC,
	C_REQUEST_TURNTABLE_FORTEN,
	S_SEND_TURNTABLE_FORTEN,
	S_JION_SEND_ACINFO,
	S_SEND_CURTIME_ACINFO,
	S_SEND_OPEN_HOMECOPY,
	S_SEND_OPEN_HOMEQUEST,

	S_CHECK_OPEN_MAPCOPY,
	C_OPEN_MAPCOPY_RLT,
};

enum TESTSTATE
{
	TEST_STATE_ON,
	TEST_STATE_OFF,
	TEST_STATE_REFLESH,
	TEST_STATE_WAIT,
	TEST_STATE_PAUSE,
	TEST_STATE_NULL,
};

struct ClientAchievement
{
	int nID;
	int nCnt;
	bool ifDone;
	int nFloor;
	int nGroup;
};

struct RPGACTIVITY_S_SEND_ACINFO_Data
{
	bool ifDifferent;
	int nShowFlag;
	int nCnt;
	WORD szID[1];
};

struct RPGACTIVITY_C_REQUEST_ACHIEVEMENT_GIFT_Data
{
	int nPos;
};

struct RPGACTIVITY_S_SEND_ACHIEVEMENT_ALL_Data
{
	int nCnt;
	ClientAchievement xInfo[1];
};

struct RPGACTIVITY_S_SEND_ACHIEVEMENT_CHG_Data
{
	ClientAchievement xInfo;
};

struct RPGACTIVITY_S_SEND_ACHIEVEMENT_SHOW_Data
{
	int nID;
};

struct RPGACTIVITY_S_SEND_FINDNPC_Data
{
	int nID;
};

struct RPGACTIVITY_C_REQUEST_TURNTABLE_GIFT_Data
{
	int nPos;
};

struct RPGACTIVITY_S_SEND_TURNTABLE_START_Data
{
	int nPos;
};

struct RPGACTIVITY_S_SEND_TURNTABLE_FORTEN_Data
{
	int  nCnt;
	BYTE byPos[1];
};

struct RPGACTIVITY_C_REQUEST_ACINFO_Data
{
	int nFlag;
};

struct RPGACTIVITY_C_REQUEST_ENTERAC_Data
{
	int nID;
};

struct RPGACTIVITY_S_SEND_TESTINFO_Data
{
	int nCount;
	int nStar;
	int nTypeID;
	BYTE byState;
	int nCorrect;
	int nTime;
	int nPlace;
	BYTE byCh[4];
};

struct RPGACTIVITY_S_SEND_RANKINFO_Data
{
	int nCnt;
	char szName[10][20];
	int szScore[10];
	int nMin;
};

struct RPGACTIVITY_C_ANSWER_QUESTION_Data
{
	BYTE byPos;
};

struct RPGACTIVITY_S_SEND_START_RLT_Data
{
	int isRlt;
};

struct RPGACTIVITY_S_SEND_ACDLV_Data 
{
	BYTE byDlv;
};

struct RPGACTIVITY_S_SEND_ACDEGREE_Data 
{
	int nCnt;
	BYTE byDLv;
	BYTE byPos;
	UserDegreePart szDegree[1];
};

struct RPGACTIVITY_C_REQUEST_DEGREEAWARD_Data 
{
	BYTE byPos;
};

struct RPGACTIVITY_S_SEND_BOSSINFO_Data 
{
	int nCnt;
	WORD wBossID[1];
};

struct RPGACTIVITY_C_SEND_JUMPTONPC_Data
{
	int nID;
};

struct RPGACTIVITY_S_JION_SEND_ACINFO_Data
{
	int nCnt;
	WORD szID[1];
};

struct RPGACTIVITY_S_SEND_CURTIME_ACINFO_Data
{
	WORD wID;
};

struct RPGACTIVITY_S_CHECK_OPEN_MAPCOPY_Data
{
	WORD  wMCID;
	DWORD dwCostGold;
};

struct RPGACTIVITY_C_OPEN_MAPCOPY_RLT_Data
{
	WORD wMCID;
};

#pragma pack (pop)