#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)

enum 
{
	S_FRIEND_LIST,
	S_FRIEND_ADD,
	S_FRIEND_VAL,
	S_FRIEND_DEL,
	S_SEARTH_LIST,


	C_SEARTH_FRIEND,
	C_DEL_FRIEND,
	C_SEND_GIFT,
	C_GET_GIFT,

	C_P2PASK,
	S_P2PASK,
	C_P2PANSWER,

	S_WARRANK_INFO,
	C_FIGHT_RPOS,
	C_RQWRANK_INFO,

	S_SEND_FORTDATA,
	S_SEND_FORTDATA_CHG,
	S_SEND_MANORDATA,

	C_SEED_MANOR,
	C_UNLOCK_MANOR,
	C_FAST_MANOR,
	C_GET_MANORDATA,
	C_GET_MANOR_REWARD,
	C_GET_ALL_MANOR_REWARD,

	S_SEND_WARRANKDATA,
	C_SEND_WARRANK_POS,
	C_SEND_WARRANK_STATE,
	C_GET_MANOR_REWARDEX,
	C_REQUEST_SUITABLE,
	S_SEND_SUITABLE,
	C_ASK_ADDFRIEND,
	C_DEALWHIT_SUITABLE,
	S_SEND_REQUESTLIST,
	S_WARRANK_TEN_INFO,
	S_FRIEND_ONLINE_CHG,
	S_FRIEND_ONLINE_LIST,

	S_SEND_MANOR_RECORD,
	C_GET_MANOR_RECORD,

	S_SEND_FORTDATA_NEW,
	S_SEND_MANORDATA_NEW,

	C_FAST_MANOR_NEW,
	C_GET_MANORDATA_NEW,
};
enum{
	WARRANK_STATE_START,
	WARRANK_STATE_WIN,
	WARRANK_STATE_FAIL,
};
enum	P2PASK_TYPE
{
	PAT_FRIEND,		//邀请加为好友
	PAT_HELP,
	PAT_PK,
};

enum	P2PASK_STEP
{
	PAS_ASK,		//发起
	PAS_RECV,		//对方收到
	PAS_ANSWER,		//对方回答
	PAS_END,			//发起方确认并删除
};

enum
{
	RS_TYPE_FRIEND,
	RS_TYPE_ENEMY,
	RS_TYPE_PRE_FRIEND,			//
	RS_TYPE_PRE_FRIENDEX,		//被申请
};

enum
{
	FRIEND_RS_VAL_GIVE,
	FRIEND_RS_VAL_GET,

};
class P2PUserBase
{
public:
	DWORD	dwUinB;
	char	szName[16];
	BYTE	byLv;
	BYTE	byVip;
	
};
class P2pAskObj
{
public:
	int		nIndex;
	DWORD	dwSend;
	DWORD	dwRecv;
	DWORD	dwStartTime;
	BYTE	bStep;
	BYTE	bType;
	int		nLvl;
	char	szName[16];
};

class WRankSimpleObj
{
public:
	DWORD	dwUin;
	WORD	wPos;
	char	szName[16];
	BYTE	byLv;
	BYTE	byJob;
	BYTE	bySex;
	DWORD	dwScore;
	WORD	wSkin[2];
};

class FriendObj	: public P2PUserBase
{
public:

	BYTE	byRsType;
	int		nRsVal;
	char	szHomeName[20];
	BYTE	byJob;
	BYTE	byCountry;
};


const int nMaxManorCount = 10;
const int nMaxManorFast = 10;
class ManorData{
public:
	int		nPos;
	int		nExp;
	BYTE	nLv;
	DWORD	dwTime[nMaxManorCount];
	DWORD	dwID[nMaxManorCount];
	DWORD	dwFast[nMaxManorCount];

	DWORD	dwFastUin[nMaxManorFast];				//给好友加速
	int		nState[nMaxManorCount];
};

class ManorDataEx{
public:
	int		nPos;
	int		nExp;
	BYTE	nLv;
	DWORD	dwTime[nMaxManorCount];
	DWORD	dwID[nMaxManorCount];
	DWORD	dwFast[nMaxManorCount];

	DWORD	dwFastUin[nMaxManorFast][5];				//给好友加速
	DWORD		nState[nMaxManorCount][5];
};

struct MapManorRecord
{
	DWORD	dwUin;
	WORD	wVal;
};

class WarRankData{
public:
	int	nRankPos;
	int	nRankScore;
	int	nTimes;
};

struct SuitFriend{
	DWORD dwUin;
	char szName[20];
	BYTE byCountry;
	BYTE byLv;
	BYTE byJob;
	char szHomeName[20];
};

enum{
	FORTDATA_CHG_MANOR_POS,
	FORTDATA_CHG_MANOR_TIME,
	FORTDATA_CHG_MANOR_ID,
	FORTDATA_CHG_MANOR_FAST,
	FORTDATA_CHG_MANOR_FASTUIN,
	FORTDATA_CHG_MANOR_EXP,
	FORTDATA_CHG_MANOR_LV,
	FORTDATA_CHG_WARRANK_SCORE,
	FORTDATA_CHG_MANOR_EFFECT,
};
struct	FRIEND_S_SEND_FORTDATA_CHG_Data
{
	BYTE	byType;
	DWORD	dwUin;
	BYTE	byPos;
	DWORD	dwVal;
};

struct	FRIEND_S_SEND_SUITABLE_Data
{
	SuitFriend szInfo[6];
};

struct	FRIEND_C_ASK_ADDFRIEND_Data
{
	int nId;
};

struct	FRIEND_C_DEALWHIT_SUITABLE_Data
{
	int nId;
	bool ifOk;
};

struct	FRIEND_S_SEND_REQUESTLIST_Data
{
	SuitFriend xInfo;
};

struct	FRIEND_C_GET_MANORDATA_Data
{
	DWORD	dwUin;
};

struct	FRIEND_C_GET_MANORDATA_NEW_Data
{
	DWORD	dwUin;
};

struct	FRIEND_C_GET_MANOR_REWARD_Data
{
	BYTE	byPos;
};
struct	FRIEND_C_GET_MANOR_REWARDEX_Data
{
	DWORD	dwUin;
	BYTE	byPos;
};
struct	FRIEND_S_SEND_FORTDATA_Data
{
	DWORD	dwUin;
	ManorData	xManorData;
	WarRankData	xWarRankData;
};
struct	FRIEND_S_SEND_FORTDATA_NEW_Data
{
	DWORD	dwUin;
	ManorDataEx	xManorData;
	WarRankData	xWarRankData;
};
struct	FRIEND_S_SEND_MANORDATA_Data
{
	DWORD	dwUin;
	ManorData	xManorData;
};
struct	FRIEND_S_SEND_MANORDATA_NEW_Data
{
	DWORD	dwUin;
	ManorDataEx	xManorData;
};
struct	FRIEND_S_SEND_WARRANKDATA_Data
{
	DWORD	dwUin;
	WarRankData	xWarRankData;
};
struct	FRIEND_C_SEND_WARRANK_POS_Data
{
	int		nPos;
};
struct	FRIEND_C_SEND_WARRANK_STATE_Data
{
	BYTE	byState;
	int		nVal;
};
struct	FRIEND_C_SEED_MANOR_Data
{
	BYTE	byPos;
	DWORD	dwIndex;
};
struct	FRIEND_C_FAST_MANOR_Data
{
	BYTE	byPos;
	DWORD	dwUin;
};
struct	FRIEND_C_FAST_MANOR_NEW_Data
{
	BYTE	byPos;
	DWORD	dwUin;
};
struct	FRIEND_S_WARRANK_INFO_Data
{
	BYTE	bCnt;
	WRankSimpleObj	szWRObj[1];
};

struct	FRIEND_S_WARRANK_TEN_INFO_Data
{
	BYTE	bCnt;
	WRankSimpleObj	szWRObj[1];
};

struct	FRIEND_C_FIGHT_RPOS_Data
{
	WORD	wPos;
};

struct	FRIEND_S_SEARTH_LIST_Data
{
	WORD		wCnt;
	P2PUserBase	frdBase[1];
};

struct	FRIEND_S_FRIEND_LIST_Data
{
	WORD		wCnt;
	FriendObj	frdObj[1];
};

struct	FRIEND_S_P2PASK_Data
{
	BYTE		bType;
	int			nIndex;
	P2PUserBase	frd;
};

struct	FRIEND_S_FRIEND_ADD_Data
{
	FriendObj	frdObj;
};
struct	FRIEND_S_FRIEND_VAL_Data
{
	FriendObj	frdObj;
};
struct	FRIEND_S_FRIEND_DEL_Data
{
	DWORD	dwUin;
};
struct	FRIEND_C_SEND_GIFT_Data
{
	DWORD	dwUin;
};
struct	FRIEND_C_GET_GIFT_Data
{
	DWORD	dwUin;
};
struct	FRIEND_C_DEL_FRIEND_Data
{
	DWORD	dwUin;
};
struct	FRIEND_C_P2PASK_Data
{
	BYTE	bType;
	char	szName[20];
};

struct	FRIEND_C_P2PANSWER_Data
{
	int		nIndex;
	DWORD	dwAsker;
	BYTE	bType;
	BYTE	bOk;
};

struct	FRIEND_S_FRIEND_ONLINE_CHG_Data
{
	DWORD	dwUin;
};

struct	FRIEND_S_FRIEND_ONLINE_LIST_Data
{
	int nCnt;
	DWORD dwUin[1];
};

struct NetManorRecord
{
	DWORD	dwUin;
	WORD	bVal;
	char    szName[20];
};

struct FRIEND_S_SEND_MANOR_RECORD_Data
{
	WORD	wCnt;
	NetManorRecord	szRecord[1];
};


#pragma pack (pop)