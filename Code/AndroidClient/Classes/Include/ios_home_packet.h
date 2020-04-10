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
	C_CREATE,	//需要先判断条件的
	C_REQUEST,	//申请加入帮会
	C_ACCEPT,	//同意加入
	C_DUTY,
	C_EXIT,		//离开或踢人
	C_RQLIST,	//查看申请列表
	C_RQHOMES,	//查看帮会列表
	C_GETHOMEINFO,
	C_LEAGUE_BATTLE,	//开始军团战
	C_LEAGUE_BATTLE_JOIN,	//报名军团战
	C_GET_HOMEBATTLEINFO,	//查询战报
	C_GET_BATTLE_ACTOR,		//查询参战者
	C_SWITCH_BATTLE_POS,
	C_SET_BATTLE_POS,

	S_HOMEINFO,	//发送玩家他自己的帮会信息
	S_HOMELIST,	//随机发10个未满帮会的信息给玩家
	S_HOMEREQER,
	S_HOMEDUTYCHG,	//成员职位变更
	S_HOMEMEMBERADD,	//新增成员
	S_HOMEMEMBERDEL,	//成员离开
	S_HOMESIMPLE,
	S_HOMEBATTLEINFO,
	S_BATTLE_ACTOR,		//发送参战者
	C_CHANGE_LEAGUE_DESC,		//修改军团信息
	C_CONTRIBUTION,			//军团贡献
	C_ADD_BUILD_LV,
	S_BUILDINFO,
	S_REQUEST_RESULT,
	C_CANCEL_REQUEST,	//取消申请加入帮会
	S_REQUEST_CANCEL_RESULT,
	S_NEWREQUEST,
	S_HOME_DISSOLVED,	//解散公会
	C_DISSOLVE_HOME,
};

const int nMaxBuildLv = 10;
enum{
	HOME_BUILD_RESEARCH,
	HOME_BUILD_MAX,
};
class HomeBuildTag
{
public:
	BYTE nBuildLv[HOME_BUILD_MAX];
};

struct NHomeMember
{
	BYTE	bLv;
	char	szName[16];
	DWORD	dwUin;
	BYTE	bDuty;
	DWORD	dwContribute;
	BYTE	bJob;
	bool	bOnline;
};

struct NHomeSInfo
{
	BYTE	bLv;			//等级
	char	szName[20];		//名字
	WORD	wHomeID;
	WORD	wMemCnt;		//已有人数
	char	szCreateName[20];		//名字
	BYTE	bCountry;		//名字
	bool	bApply;			//是否申请
};
class HomeBattleObj{
public:
	char szName[20];
	int	nLeader;
	DWORD dwUin;
	int  nMaxHp;
	int	 nNowHp;
	int  nAtkMin;
	int  nAtkMax;
	int	 nDefMin;
	int  nDefMax;
	int  nPos;		//0,不在队列
};
struct BattleStep{
	int	nAtkPos;
	int nDefPos;
	int nDamage;
	bool bAtk;
};
struct NHOME_S_HOMEMEMBERADD_Data 
{
	NHomeMember	member;
};

struct NHOME_S_HOMEREQER_Data 
{
	int		nCnt;
	NHomeMember	szReqers[1];
};

struct NHOME_S_HOMEMEMBERDEL_Data 
{
	DWORD	dwUin;
};

struct NHOME_S_HOMEDUTYCHG_Data 
{
	DWORD	dwUin;
	BYTE	byDuty;
};
struct NHOME_C_CHANGE_LEAGUE_DESC_Data 
{
	char szDesc[100];
};
struct NHOME_C_ADD_BUILD_LV_Data 
{
	BYTE byType;
};
struct NHOME_S_HOMEINFO_Data 
{
	char	szName[20];
	BYTE	bLv;
	DWORD	dwExp;
	WORD	wCnt;
	char	szDesc[100];
	BYTE	bOccupy;
	DWORD	dwHomeID;
	BYTE	byHomeDuty;
	HomeBuildTag	xBuild;
	NHomeMember	szMember[1];
};
struct NHOME_S_HOMEBATTLEINFO_Data 
{
	int nPos;
	BYTE byResult;
	HomeBattleObj	AtkObj[6];
	HomeBattleObj	DefObj[6];
	WORD	wCnt;
	BattleStep	Step[1];
};
struct NHOME_S_BUILDINFO_Data 
{
	HomeBuildTag	xBuild;
};
struct NHOME_S_HOMESIMPLE_Data 
{
	BYTE	bLv;
	DWORD	dwExp;
};
struct NHOME_C_GET_HOMEBATTLEINFO_Data 
{
	int	nPos;
};
struct NHOME_C_SWITCH_BATTLE_POS_Data 
{
	int	nPos1;
	int	nPos2;
};
struct NHOME_C_SET_BATTLE_POS_Data 
{
	DWORD dwUin;
	int	nPos;
};
struct NHOME_S_BATTLE_ACTOR_Data 
{
	int nCount;
	HomeBattleObj	BattleObj[1];
};
struct NHOME_S_HOMELIST_Data 
{
	bool	ifClean;
	WORD	wCnt;
	NHomeSInfo	szHome[1];
};

struct NHOME_C_CREATE_Data 
{
	char	szName[20];
	bool    bUseRmb;
};

struct NHOME_C_REQUEST_Data 
{
	WORD	wHomeID;
};
struct NHOME_C_CANCEL_REQUEST_Data 
{
	WORD	wHomeID;
};
struct NHOME_S_REQUEST_RESULT_Data 
{
	WORD	wHomeID;
};
struct NHOME_S_REQUEST_CANCEL_RESULT_Data 
{
	WORD	wHomeID;
};
struct NHOME_C_ACCEPT_Data 
{
	WORD	wResult;
	DWORD	dwUin;
};

struct NHOME_C_DUTY_Data 
{
	WORD	wHomeID;
	BYTE	byDuty;
	DWORD	dwUin;
};

struct NHOME_C_EXIT_Data 
{
	WORD	wHomeID;
	DWORD	dwUin;
};
struct NHOME_C_LEAGUE_BATTLE_Data 
{
	int	nPos;
};
#pragma pack (pop)