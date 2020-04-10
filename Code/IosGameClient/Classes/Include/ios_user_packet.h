#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)
const int nMaxMailItemCount = 4;
const int nMakeSkillResetTimesFlag = 32;

enum 
{
	//S_JOINGAME,
	//C_REQUEST_JOIN,
	S_USER_INFO,
	C_REQUEST_USERINFO,			//�����ɫ��Ϣ
	S_NEED_CREATE,				//��Ҫ������ɫ

	CS_SEND_SIMPLEINFO,			//���ͽ�ɫ��Ϣ
	//C_CREATE_CHARACTER,			//������ɫ
	S_CREATE_CHARACTER_FAIL,			//������ɫʧ��
	S_OPEN_FORM,				//�򿪽���
	S_SEND_POP_MSG,
	S_SEND_MAIL_INFO,				//���ͺ�����Ϣ
	S_SEND_MAIL_DEL,
	C_DEL_MAIL,
	S_SEND_CHAT_MSG,				//����������Ϣ
	S_WARN_MSG,
	C_SEND_CHAT_MSG,				//����������Ϣ	
	S_SEND_USER_PARAM,

	S_WEALTH,

	N_NETGATEOK,

	C_JUMPMAP,

	S_FLAG_ALL,
	S_FLAG_SINGLE,

	S_FLAG_SLIMIT_ALL,
	S_FLAG_SLIMIT_SINGLE,

	S_FLAG_QUEST_ALL,
	S_FLAG_QUEST_SINGLE,

	S_WEALTH_SINGLE,

	S_QUEST_INFO,
	S_REMOVE_QUEST,

	C_QUEST_ACCEPT,
	C_QUEST_COMPLETE,
	C_QUEST_GIVEUP,

	CS_TEAM_STATE,			//���
	S_TEAM_INFO,			//���

	C_TEAM_INVITE,
	S_PLAYEFFECT,

	C_JOINMAPCOPY,
	C_LEAVEMCOPY,

	C_SKILLUP,

	S_SKILL_SINGLE,
	S_SKILL_ALL,
	C_SET_SSHORTCUT,
	C_REQUEST_VIPTEST,
	C_REQUEST_REVIVE,

	C_SKILLCZ,
	S_SEND_HOMESKILL,
	C_LEARN_HOMESKILL,
	S_SEND_HOME_QUEST,

	C_HOME_QUEST_ACCEPT,
	C_HOME_QUEST_COMPLETE,

	C_GET_FORTDATA,
	S_SEND_SERVERTIME,

	C_SEND_MAIL_INFO,

	S_SEND_MAPCOPY_OK,	//��ɸ���
	C_OPENCARD,
	S_OPENCARDRTL,

	C_RQ_TEAM_MCOPY,
	C_MCOPY_TEAM_TIMEUP,
	S_MCOPY_MASTERJOIN,
	S_RQ_TEAM_MCOPY_RLT,
	C_NEED_TD_REQUEST,

	C_GETMAIL_ITEM,
	C_READMAIL,
	C_DELETE_MAIL,
	CS_SEND_FAST_ITEM,
	C_USE_FAST_ITEM,
	S_TEAM_DROPRAND,
	C_GET_EQUIPINFO,
	S_SEND_EQUIPINFO,

	C_REQUEST_NPC_FORM,
	S_RESPON_NPC_FORM,

	C_REQUEST_LUCKBET,
	S_LUCKBET_RTL,
	C_RQ_LUCKBET_EX,
	S_LUCKBET_EX_RTL,
	C_GET_LUCK_AWARD,
	C_AUTO_FIX,
	C_RQ_STAR,
	C_RQ_LIFE_STAR,
	S_COOLDOWN_ALL,
	S_COOLDOWN_SINGLE,
	C_AUTO_BUYDRUG,
	C_GET_RANK_ENAMY,

	CHECK_GAMESVR_ALIVE,
	C_RQ_BACK_CITY,
	C_REQUEST_VIPGIFT,
	C_REQUEST_VIPPOWER,
	C_REMOVE_SPSKILL,

	S_FLAG_COPYLMT_ALL,
	S_FLAG_COPYLMT_SINGLE,

	C_CHAGE_PETSTATE,
	S_SEND_EXPMULTIPILE,
	S_SEND_KILLDROP,

	C_RQ_OPENALL,
	S_RQ_OPENALL_RLT,

	C_DELETE_MAIL_ALL,
	C_FIGHT_TARGET,
	S_FIGHT_TARGET,
	C_FIGHT_REQUEST,

	C_OVER_GREENMCOPY,
	C_JOINMAPCOPY_SP,
	C_JUMP_HOME_WARCOPY,

	C_FORT_HARVEST,
	C_FISH_RLT,
	S_MAKESKILL_INFO,
	C_GIVEUP_MAKESKILL,
	C_REQUEST_FISH,
	S_INVITE_CODE,
	C_GET_INVAWARS,
	C_ADD_WARRANKTIMES,
	C_GET_MULTISEC,
	C_BUY_MAKESKILL_POWER,
	C_OPENCARD_EX,
	C_SET_AUTOSKILL,
	C_ACTIVATE,
	S_SET_AUTOFIX,
	S_SET_AUTOBUY,
	S_SEND_LOGIN_DAYS,
	C_CHANGE_COUNTRY,

	C_BTN_DEFAULEVENT,

	C_CHG_USER_NAME,
	S_CHGNAME_RLT,
	C_GET_APPRS_AWARD,

	C_GET_FORTDATA_NEW,

	C_GET_NOTICE,
	S_SEND_NOTICE,

	S_SEND_CLOSE_WAITE,
	S_WORLD_INFO,

	S_SEND_HOME_MAXCTB,
	S_SEND_LUA_EVENT,
	C_FLAPPYBIRD_SCORE,
	C_FLAPPYBIRD_OPEN,
	S_FLAPPYBIRD_OPEN,
	SC_CHECKTIME,
};

enum{
	STATE_MAIL_SENDWAITING,		//�����ʼ�		������������ʱ�����
	STATE_MAIL_SEND,		//����			������Լ�����, �ʼ����Ĳ���
	STATE_MAIL_RETURNWAITING,	//�����ʼ�
	STATE_MAIL_RETURN,		//�˼�			������Լ�����, �ʼ����Ĳ���
	STATE_MAIL_OVER,		//�����ʼ�		ֻ�����ݿ���,�ڴ治����
	STATE_MAIL_READ,		//�Ѷ��ʼ�
	STATE_MAIL_GOT,		//��ȡ�ʼ�
};

enum TEAM_COMMIT{
	TEAM_COMMIT_INVITE,			//�����������
	TEAM_COMMIT_ACCEPT,			//ͬ��
	TEAM_COMMIT_REFUSE,			//�ܾ�
	TEAM_COMMIT_LEAVE,			//���
	TEAM_COMMIT_CHGLEADER,			//�ı�ӳ�
	TEAM_COMMIT_KICK,			//����
};

#define		TEAM_MAXUSER	5
enum TEAM_STATE{
	TEAM_CREATE,	//���ڴ���(��������ʱ������ʱ)
	TEAM_NORMAL,	//����
	TEAM_LEADER,	//�ӳ�
};
class _TeamNet{
public:
	_TeamNet(){dwUin=0;bOnline =false;};
	BYTE	nPos;
	DWORD	dwUin;
	bool	bOnline;
	BYTE	nState;
	int		nHp;
	int		nMaxHp;
	BYTE	byJob;
	int		nLv;
	char	szName[20];
	BYTE	bySex;
};

enum{
	USER_HOME_SKILL_HP,
	USER_HOME_SKILL_MP,
	USER_HOME_SKILL_HIT,
	USER_HOME_SKILL_MISS,
	USER_HOME_SKILL_CRIT,
	USER_HOME_SKILL_MAX,
};

class UserHomeSkill{
public:
	BYTE	bySkillLv[USER_HOME_SKILL_MAX];
};

#define ParamMax  9
struct UserParam{
	int		nParam[ParamMax];
	int		nSpeed;
	//���Ѫ��, �������, �﹥, ħ��, ���, ħ��, ����,����,����,�ƶ�
	int		nSkin;
	int		nWeapon;
	int		nHorse;
	int		nPvpJSDef;
};

struct CheckUserInfo
{
	DWORD dwUin;
	char szName[20];
	int	nLv;
	BYTE byJob;
	int nScore;
	int nSkin[3];
	char pData[10][128];
	BYTE szSize[10];
};

const int nMakeSkillCount = 1;

struct MakeSkill{
	BYTE bySkill[nMakeSkillCount];
	int		nPower;
};

struct NetUFlag
{
	WORD	wID;
	int		nVal;
};

struct USER_S_FLAG_ALL_Data 
{
	WORD	wCnt;
	NetUFlag	szFlag[1];
};

struct USER_S_SEND_EXPMULTIPILE_Data 
{
	int nUserMultipile;
	int nPetMultipile;
	int nUserMultipileSec;
	int nPetMultipileSec;
};

struct USER_S_SEND_LUA_EVENT_Data 
{
	int nCnt;
	int szValue[5];
};

struct USER_S_COOLDOWN_ALL_Data 
{
	WORD	wCnt;
	NetUFlag	szFlag[1];
};

struct USER_C_REQUEST_VIPGIFT_Data 
{
	int nLv;
};

struct USER_S_CREATE_CHARACTER_FAIL_Data 
{
	BYTE	bErrorCode;
};

struct USER_S_QUEST_INFO_Data 
{
	WORD	wQid;
	WORD	wVal[1];
};
struct USER_S_REMOVE_QUEST_Data 
{
	WORD	wQid;
	BYTE	bComplete;
};

struct USER_C_JOINMAPCOPY_Data 
{
	WORD	wMCID;
};

struct USER_C_JOINMAPCOPY_SP_Data 
{
	WORD	wMCID;
};
struct USER_S_INVITE_CODE_Data 
{
	char	szInvite[10];
	int		nReachNum;
	int		nCheckNum;
};


struct USER_S_MAKESKILL_INFO_Data 
{
	MakeSkill	skillInfo;
};
struct USER_C_GIVEUP_MAKESKILL_Data 
{
	int		nType;
};
struct USER_C_ACTIVATE_Data 
{
	char sztxt[32];
};
struct USER_C_BTN_DEFAULEVENT_Data 
{
	int nEvent;
};

struct USER_C_RQ_TEAM_MCOPY_Data 
{
	WORD	wMCID;
};

struct USER_S_MCOPY_MASTERJOIN_Data 
{
	WORD	wMCID;
};
struct USER_C_GETMAIL_ITEM_Data 
{
	DWORD	dwMailID;
};
struct USER_C_READMAIL_Data 
{
	DWORD	dwMailID;
};
struct USER_C_DELETE_MAIL_Data 
{
	DWORD	dwMailID;
};
struct USER_CS_SEND_FAST_ITEM_Data 
{
	DWORD	dwItemID;
};
typedef struct USER_C_QUEST_ACCEPT_Data 
{
	WORD	wQid;
}USER_C_QUEST_COMPLETE_Data,USER_C_QUEST_GIVEUP_Data;

typedef struct USER_C_HOME_QUEST_ACCEPT_Data 
{
	WORD	wQid;
}USER_C_HOME_QUEST_COMPLETE_Data;

struct USER_S_SEND_SERVERTIME_Data 
{
	DWORD	dwServerTime;
};

struct USER_S_SEND_MAPCOPY_OK_Data 
{
	WORD	wMCopyID;
	BYTE	byRankVal;
};

struct USER_S_OPENCARDRTL_Data 
{
	DWORD	wItem;
};

struct NetQuestFlag
{
	WORD	wID;
	BYTE	bVal;
};

struct USER_S_FLAG_QUEST_ALL_Data 
{
	WORD	wCntLine;
	WORD	wCntTime;
	NetQuestFlag	szFlag[1];
};

struct USER_S_FLAG_QUEST_SINGLE_Data 
{
	BYTE		bIsTime;
	NetQuestFlag	szFlag;
};

struct USER_S_FLAG_SLIMIT_ALL_Data
{
	WORD	wCnt;
	BYTE	szbyData[1];
};

typedef struct USER_S_FLAG_SINGLE_Data 
{
	NetUFlag	szFlag;
}USER_S_FLAG_SLIMIT_SINGLE_Data,USER_S_COOLDOWN_SINGLE_Data;

struct USER_S_USER_INFO_Data 
{
	DWORD	dwUin;
	// 	WORD	wHomeID;
	// 	BYTE	byHomeDuty;
	// 	BYTE	byVipLvl;
	BYTE	byJob;
	BYTE	bySex;
	BYTE	byCountry;
	char	szNick[20];
};
struct USER_S_SEND_USER_PARAM_Data 
{
	UserParam m_Param;
};
struct DropTag{
	int nPos;
	int nWave;
	int nItem;
	BYTE nType;
};
struct MazeData{
	BYTE nx,ny;
	BYTE byType;	//0δ���� 1δ̽�� 2�հ� 3δ̽������ 4����
};

struct SimpleUserInfo
{
	DWORD	dwUin;
	char	szNickName[20];
	BYTE	byLevel;	
	DWORD	dwExp;
};

struct USER_CS_SEND_SIMPLEINFO_Data
{
	DWORD       dwAccountUin;
	int         nCount;
	SimpleUserInfo  pInfos[1];
};
struct USER_N_NETGATEOK_Data
{

};

struct USER_CS_GETSERVERTIME_Data
{
	int         nTime;
};

struct USER_C_JUMPMAP_Data
{
	int			nTransPtID;
};

struct USER_S_WARN_MSG_Data
{
	char	szMsg[1];
};

struct USER_C_CREATE_CHARACTER_Data
{
	char	szName[20];
};

//struct USER_S_WEALTH_Data
//{
//	DWORD	dwRmb;
//	DWORD	dwGold;
//	DWORD	dwExp;
//	BYTE	bLvl;
//	WORD	wPow;	
//	WORD	wSkillPt;
//};

struct USER_S_WEALTH_SINGLE_Data
{
	BYTE	byType;
	DWORD	dwVal;
};

struct USER_S_OPEN_FORM_Data 
{
	int		nType;
	char	szFormName[20];
	char	szCompName[20];
};
struct USER_C_REQUEST_JOIN_Data 
{
	DWORD	dwUin;
};

struct USER_C_OVER_GREENMCOPY_Data 
{
	WORD	wMCopyID;
};


struct USER_C_SKILLUP_Data 
{
	WORD	wSkill;
};

struct USER_C_REMOVE_SPSKILL_Data 
{
	WORD	wSkill;
};

struct USER_C_SET_SSHORTCUT_Data 
{
	WORD	wSkill;
	BYTE	byPos;
};

struct USER_C_REQUEST_REVIVE_Data 
{
	BYTE	bItem;
};

struct USER_S_SEND_HOMESKILL_Data 
{
	UserHomeSkill	xSkill;
};
struct USER_C_LEARN_HOMESKILL_Data 
{
	BYTE byType;
};
struct USER_S_SEND_HOME_QUEST_Data 
{
	BYTE byTimes;
	WORD wQuestID;
};

struct USER_S_SEND_FRIEND_INFO_Data 
{
	int		nUin;
	int		nUin2;
	int		nType;
};

struct USER_S_SEND_MAIL_DEL_Data
{
	DWORD	dwMailID;
};

struct USER_C_DEL_MAIL_Data
{
	DWORD	dwMailID;
};

struct USER_S_SEND_MAIL_INFO_Data 
{
	DWORD	dwMailID;
	DWORD	uinSender;
	DWORD	uinRecver;
	char	szTitle[32];
	BYTE	byState;		
	int		nMoney;			//������Ǯ��ʾ����ȡ��, ���ķ����ʼ���ʽ���ظ�������
	int		nItemID[nMaxMailItemCount];
	int		nCnt[nMaxMailItemCount];
	int		nItemIndex[nMaxMailItemCount];
	DWORD	dwReturnTick;	//��Ч��, ���ڵ��ʼ����Զ��˻�
	char	szBody[1];

};
struct USER_C_SEND_MAIL_INFO_Data 
{
	char	szTitle[32];
	char	szBody[255];
	int		nMoney;			//������Ǯ��ʾ����ȡ��, ���ķ����ʼ���ʽ���ظ�������
	int		nCnt[nMaxMailItemCount];
	int		nItemIndex[nMaxMailItemCount];
};
struct USER_S_SEND_CHAT_MSG_Data 
{
	BYTE	byCRType;
	int		nKeyVal;
	char	szMsg[1];
};
struct USER_C_SEND_CHAT_MSG_Data 
{
	BYTE	byCRType;
	char	szMsg[1];
};

struct USER_S_SEND_POP_MSG_Data 
{
	char szMsg[1];
};

struct USER_C_USEITEM_Data 
{
	int nItemID;
};
struct USER_CS_TEAM_STATE_Data 
{
	BYTE byType;
	BYTE byPos;
	char szMsg[1];
};
struct USER_S_TEAM_INFO_Data 
{
	_TeamNet xUser;
};
struct USER_C_TEAM_INVITE_Data 
{
	DWORD dwUin;
};
struct USER_S_PLAYEFFECT_Data 
{
	int nEffect;
};

struct USER_S_SKILL_SINGLE_Data 
{
	WORD	wSkill;
	BYTE	byLv;
};

struct USER_S_SKILL_ALL_Data 
{
	BYTE	bCnt;
	USER_S_SKILL_SINGLE_Data	szSkill[1];
};

struct USER_S_TEAM_DROPRAND_Data
{
	DWORD nItemID;
};

struct USER_C_GET_EQUIPINFO_Data
{
	DWORD dwUin;
};

struct USER_S_SEND_EQUIPINFO_Data
{
	CheckUserInfo pUserInfo;
};

struct USER_C_REQUEST_NPC_FORM_Data
{
	int nNpcCmd;
};

struct USER_C_REQUEST_LUCKBET_Data
{
	BYTE	byCoinType;
	WORD	wVal[8];
};

struct USER_S_LUCKBET_RTL_Data
{
	BYTE	byEndPos;
	int		nGetVal;
};

struct USER_C_RQ_LUCKBET_EX_Data
{
	BYTE	bySide;
	int		nExVal;
};

struct USER_C_RQ_STAR_Data
{
	BYTE bToLv;
};
struct USER_C_RQ_LIFE_STAR_Data
{
	BYTE bToLv;
};

struct USER_S_LUCKBET_EX_RTL_Data
{
	BYTE	byRtlVal;
	int		nGetVal;
};


struct USER_S_RESPON_NPC_FORM_Data
{
	int nNpcCmd;
};

struct USER_C_NEED_TD_REQUEST_Data
{
	BYTE	byRtlNeed;
	DWORD   dwUin;
	DWORD   dwItemID;
};

struct USER_C_AUTO_BUYDRUG_Data
{
	int		nType;
	DWORD	nItemID;
	DWORD	nCount;
};

struct USER_S_FLAG_COPYLMT_ALL_Data
{
	WORD	wCnt;
	NetQuestFlag	szFlag[1];
};

struct USER_S_FLAG_COPYLMT_SINGLE_Data
{
	DWORD	nCopyID;
	BYTE	bVal;
};

struct USER_C_CHAGE_PETSTATE_Data
{
	BYTE	byPetState;
};

struct USER_S_SEND_KILLDROP_Data
{
	WORD	nWID;
};

struct USER_C_RQ_OPENALL_Data
{
	WORD	nWRmb;
	WORD    nWCnt;
};

struct USER_S_RQ_OPENALL_RLT_Data 
{
	WORD	wCnt;
	DWORD	wItem[1];
};
struct USER_C_FIGHT_TARGET_Data 
{
	DWORD	dwUin;
};
struct USER_C_FIGHT_REQUEST_Data 
{
	BYTE	byResult;
};

struct USER_S_FIGHT_TARGET_Data 
{
	char	szName[20];
};
struct USER_C_DELETE_MAIL_ALL_Data 
{
	WORD	wCnt;
	DWORD	dwMailID[1];
};

struct USER_C_FISH_RLT_Data 
{
	BYTE  byFish;
	DWORD dwUin;
};

struct USER_S_SEND_LOGIN_DAYS_Data 
{
	int   nLoginD;
};

struct USER_C_CHG_USER_NAME_Data
{
	char	szName[20];
};

struct USER_S_SEND_NOTICE_Data 
{
	char	szMsg[1];
};

struct USER_S_WORLD_INFO_Data 
{
	WORD	wFstHome;
};

struct USER_S_SEND_CLOSE_WAITE_Data 
{
	DWORD   dwWaitSec;
};

struct USER_S_SEND_HOME_MAXCTB_Data 
{
	DWORD   dwHomeMaxCtb;
};
struct USER_C_FLAPPYBIRD_SCORE_Data
{
	WORD	wScore;
};

#pragma pack (pop)