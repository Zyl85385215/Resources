#ifndef _CHARACTER_PACKET_H_
#define _CHARACTER_PACKET_H_

#include <windows.h>
#include "groupcmd.h"
#include "player_packet.h"
#include "AccountPointDefine.h"
#include "home_packet.h"

#pragma pack (push)
#pragma pack (1)

//////////////////////////////////////////////////////////////////////////

//	确保CHARACTER消息组中的消息是永远会被接收的


enum
{
	JOIN,                                                 //进入游戏
	EXIT_MAP,                                                 //退出游戏
	CHAR_CREATE,                                          //创建人物角色
	CHAR_CREATE_FAILED,                                   //创建人物角色失败
	GET_INFO,                                             //
	GET_INFO_FAILED,
	INFO,                                                 //人物角色信息

	PASS_CHECK,                                           //验证仓库密码
	PASS_CHECK_SUCCESS,                                   //仓库密码验证成功
	PASS_CHECK_FAILURE,                                   //仓库密码验证失败
	
	

	CHECKSPEED,                                           //检测玩家速度
	
	ATTRIB,                                               //玩家一级属性
	PARAM,                                                //最高生命/精神/当前生命/精神
	HP,                                                   //生命
	MP,                                                   //精神
	SP,                                                   //据说是气，目前没用到                                                
	EXP,                                                  //经验 
	LEVEL,                                                //等级 

	MAP_POSITION,                                         //地图中的位置

	FIGHT_PARAM,                                          //七项二级属性                                   
	
	CHAR_RESTART_GAME,                                    //玩家复活，重新游戏
	
	SHORTCUTS,                                            //快捷键
	MARRY_INFO,                                           //婚姻信息
	HOME_INFO,                                            //帮会信息
	HONOUR_LVL,											  //军阶

	CHECK_PING,                                           //检查ping值

	PKINFO,                                               //玩家PK值
	JOB,                                                  //玩家职业

	SKILL_TuiGong,                                        //？？？？
	RELEASE_ATTACK_TAG,                                   //目前没用到？？
	GET_POINT,
	ACCOUNT_POINT,
	ATKDISTANCE,

	SIMPLEINFO,                                           //玩家概要信息--simpleinfo
	SET_SHORTCUT,                                         //设置快捷键
	ONLINE_TIMEMSG,                                       //防沉迷的

	FRIENDS_ADD,                                          //添加好友
	FRIENDS_DEL,                                          //删除好友

	FRIENDS_SENDINFO,                                     //
	FRIENDS_LOGINOFF,
	FRIENDS_REFRESH,
	FRIENDS_SENDALL,

	CHAR_REVIVE_GAME,
	
	WAITING_QUEUE,                                       //等待队列
	MOVE_INMAP,
	
	ADD_FURY,											 //增加的怒气值
	CHANGE_LINE,										 //服务器换线申请
	ENTER_NEW_LINE,										 //连接新服务器
	CHANGE_LINEINFO,
	DELETE_CHAR,                                         //删除角色

	HONOUR_CHANGE,
	MPLAYER_CHANGE_NAME,								 //合区后引起角色重名,被修改的角色名前添加数字,如果出现数字,发送该消悉给玩家
	ITEM_TO_NAME,                                         //称号物品

	CLICK_SIGNAL_FLARE,    
	SIGNAL_FLARE,
	CALL_ALLY,
	COME_FOR_ALLY,
	REFUSE_ALLY,
	INTIMACY_INFO,										//亲密度	
	AUTOATTACK_SLOT_CHG,
	AUTOATTACK_DATE_CHG,
	AUTOATTACK_PERIOD_CHG,
	AUTOATTACK_START_PERIOD,
	AUTOATTACK_STOP_PERIOD,
	RETURN_CHARINFO,									//返回登陆
	AUTO_REPAIR,
	SEND_TITLES,
	SEND_ADD_TITLE,
	SEND_DELETE_TITLE,
	SET_ACTIVE_TITLE,
	CS_SEND_TITLES,										//中心服务器通知游戏服务器可以发称号了
	SHOW_MAP_WEATHER,
	PLAYER_TALENT,
	DO_SCRIPT,											//执行脚本文件

	SEND_QUESTINFO,			//发送任务列表
	SEND_GIVEUPQUEST,		//发送放弃任务
	SEND_ADDQUESTVARVAL,			//任务变量加值
	SEND_SETQUESTVARVAL,			//任务变量设置
	SEND_QUESTVAR,
	SEND_FLAG,						//任务标志
	SEND_CLEARQUEST,			//删除任务列表
	SEND_SHOWQUEST,
	SEND_INCOMPLETE,
	SEND_QUESTOPTION,
	SEND_CHOOSEQUESTITEM,	
	GET_CHOOSEQUESTITEM,	//选择任务物品
	GET_GIVEUPQUEST,		//接收放弃任务请求
	GET_SHOWQUEST,
	ACCEPT_QUEST,				//接受任务

	S_LOG_CC_TICK,
	S_LOG_CC_INFO,

	GETMALL_ITEM,
	//FIGHTFORRES,		//资源争夺战
	MAPVALUE,			//地图比分

	ADD_AWARD,
	GET_AWARD,
	//RELATIONSHIP,
	GET_TIME,
	REQUEST_TIME,

	TRIG_TIME,
	LUA_MSGC2S,

	FRIEND_LEVEL_UP,				// 发送好友升级信息，5分钟后自动消失。升级的玩家名字和uin，等级。
	CONGRATULATIONS_LEVEL_UP_C,		// 发送升级祝贺信息。升级玩家的uin。
	CONGRATULATIONS_LEVEL_UP_CS,	// 收到好友恭贺升级信息，5分钟后自动消失。升级的玩家名字和uin。
	THANK_CONGRATULATIONS_LEVEL_UP,
	// 中心服务器保存一个受祝贺的次数，客户端保存恭贺列表，列表成员是玩家名字和uin，等级，收到时间点。
	VIGOR,
	CLEARRS_CHGCTRY,
	REQUEST_ALLY,

	TIME_CHECKMOVE,

	// 
	SET_RECORD_BATTLELOG,
	SEND_RECORD_BATTLELOG,

	MARRY_NICK,
	SLAVE_PICKITEMS,
	SEND_MALLITEM_BUYCNT_INFOS,
	SEND_MALLITEM_BUYCNT_INFO,
	MPLAYER_INPUT_SERIAL_NUM,
	FRIEND_BEADD,					//被加好友

	AUTOADV_REMAIN,
	OPEN_URL,
	INVITE_SETON_TRANSPORT,			// invite someone to take on your transport
	SET_AUTO_MAP,
	SEND_OFFLINE_TIME_2_USE,
	FIND_PLAYER,
	TOP_LEVEL_CONTROLLER_INFO,
	GAMEWORLD_REBATE,
	CHAR_REVIVE_RMB,
	JUMP_TO_FOUND_PLAYER,
	BOOST_CMD,
	ANSWER_BOOST_CMD,
	REQUEST_VIEW_EQUIP,	// 请求查看装备
	RESPOND_VIEW_EQUIP,
	BATTLE_STATUS,
	HONOUR_DAYWEEK,
	SEND_SKILL_NEXT_LEARN_TIME,
	MASTER_ADD,
	TUDI_ADD,
	MASTER_SURE,
	TUDI_SURE,
	HONORS_VAL,
	HOME_PRODUCTS_BANK_MONEY,
	HOME_PRODUCTS_LEVEL,
	HOME_PRODUCTS_STATE,
	HOME_PRODUCTS,

	SEND_ITEM_MARKET_BILL,
	MASTER_REFUSE,
	TUDI_REFUSE,
	LOCK_CLIENT_CTRL,

	EXPADD_FROM_FIRSTLV,
	PROTECT_VALUE,              //防护值重新计算
	ASK_JUMPTO_ACT,
	ANSWER_JUMPTO_ACT,
	TRANSPORT_LIST,
	TRANSPORT_UPDATE,
	TRANSPORT_DELETE,
	TRANSPORT_START,
	TRANSPORT_ADD_SUCCESS,
	TRANSPORT_UPDATE_SUCCESS,
	TRANSPORT_SLOT_ADD,
	SCORE_OPEN_BAG_CNT,//使用多少在线积分打开包裹
	CUR_ONLINE_SCORE,//当前积分值
	
	CLEAR_ADD_HOME_UP_TIME,      //时间到 清在线加士气的包
    SCORE_GOLD_OPEN_BANK_CNT, // 积分或者元宝打开仓库
	OPEN_NPC_WAR_MENU,
	ASK_JUMPTO_HOMEWAR,//发送给客户端，要求加入帮会挑战的提示
	ANSWER_JUMPTO_HOMEWAR,//客户端响应帮会挑战的提示

	S_PLAYERFLAGS,
	S_PLAYERFLAG_POS,
	SET_DELETE_CHAR_TIME,
	SEND_DELETE_CHAR_TIME,
	CANCEL_DELETE_CHAR,
	DISMISSHOME_OPEN_LOCK,
	DISMISSHOME_RESULT,
	ASK_QUIT_WAR,//发送给客户端，离开战场的提示
	ANSWER_QUIT_WAR,//客户端响应离开战场的提示
	ITEMOPERATION_OPEN_LOCK,
	ITEMOPERATION_RESULT,
	REQUEST_SLAVEINFO,
	SEND_VIEW_SLAVE,
	SEND_ARENA_SCORE,
	SEND_ARENA_BATTLEINFO,
	ASK_JUMPTO_HOMEQUEST,
	ANSWER_JUMPTO_HOMEQUEST,
	REQUEST_SDS_STATE,
	SLAVEOPERATION_OPEN_LOCK,
	SLAVEOPERATION_RESULT,

	S_AUTO2RECONNECT,
	SEND_ITEM_MARKET_BILL_TIP,

	C_REQUEST_HOMEEQUIP,
	
	FRIENDS_UPDATE,										  //更新好友亲密度相关信息
	FRIENDS_INGROUP,
	FRIENDS_SENDBLESS,

	CS_SERVER_START,

	ACCTACK_PROPERTY,                                // 攻击属性
	DEFENSE_PROPERTY,                                // 防御属性
	SERVET_CANNOT_JOIN,
	FRIENDS_SENDPRESENT,								//送礼方向服务器发送消息;服务器向收礼方发送消息;通过中心服务器转发;
	FRIENDS_RECVPRESENT,							//收礼方向服务器发送消息;服务器向送礼方发送消息:通过中心服务器转发;
	SEND_NEWBEGIFT,	   // 发送新手奖励信息
	GET_NEWBEGIFT,
	DEL_NEWBEGIFT,
	FLUSH_NEWBEGIFT,
	GET_LEADITEM,
	FORBID_VERSION,
	FORBID_UPDATE,
	ASK_JUMPTO_HOMEQUEST2,
	ANSWER_JUMPTO_HOMEQUEST2,
	HOME_DUTY_CHANGE,
	ASK_GOTO_PLACE,    //给客户端 发确定传 取消不传的窗口
	ANSWER_GOTO_PLACE,
	SEND_NA_BILL,
	SEND_NA_BILL_TIP,
	SEND_ADD_ACTER_POINT,
	OPEN_ACTER_MENU,
	ACTERINFO,
	SEND_ACTEX_POINT,
	SEND_ACTER_OK,
	SEND_ACTER_NO,
	SOAP_SETPLAYERFLAG,
	
	C_SWITCH_SPCSTATE,
	SEND_GIVEOVER_HOMEMONEY,
	SHOWPUSHMSG,
	TRADE_SURE,

	SEND_HSTATES_INFO,
	MPLAYER_CHANGE_HOME_NAME,
	HOME_SIGNAL_FLARE,
	HOME_CALL_ALLY,
	HOME_COME_FOR_ALLY,
	HOME_REFUSE_ALLY,
	SEND_PLAYER_COPY_CHALLENGE_COUNT,
	SEND_FISH_SCORE,
	SEND_SLAVE_COPY_CHALLENGE_COUNT,

	SEND_BOMB_BULLET,
	REQ_WARM_SKILLUP,
	REQ_WARM_LEVELUP,
	S_SEND_WM_INFO,
	S_SEND_WM_SIMPLE,

	SEND_NEWSP_COIN,
	
	OPEN_FORM_BY_NAME,
	OPEN_WORLD_MSG_FORM,

	S_SEND_STTIME,
	C_REQUEST_STTIME,

	SC_NEW_LUCKBOX,
	SEND_CLIENT_TISHI,
	CHAR_SEND_BOMB_INFO,
	SEND_GIVEOVER_HOMEGOLD,
	SEND_SWORDSPRITE_INFO,

	SENDTRADE_FLAG,
	SEND_VIP_INFO,
	REQUEST_ADD_VIP_LEVEL,
	REQUEST_ADD_VIP_TIME,
	REQUEST_VIP_FIX,
	REQUEST_VIP_BANK,
	JUMP_TO_JOB_TIP,
	S_SEND_LUCK_PRICE_ITEM,
	BUY_LUCK_ITEM,
	FORCE_JUMP_TO,
	SEND_CLIENT_FLAGS,
	USE_FURYSKILL_FAIL,
	CHAR_PACKET_MAX,
};

#define MAX_TRANSPORT_TEXT_LEN 33
#define MEMORY_POS_ITEM_ID 35083

enum JOB_TYPE 
{
	JOB_TYPE_NULL,
	JOB_TYPE_GENERAL,			//将
	JOB_TYPE_KNIGHT,			//侠
	JOB_TYPE_TAOIST,			//道
	JOB_TYPE_ALCHEMIST,			//药
	JOB_TYPE_MAX,
};

struct TransPortData 
{
	char szNote[MAX_TRANSPORT_TEXT_LEN];
	int nMapIndex;
	POINT ptPlayerPos;
};

struct stTitleTime
{
	WORD	wID;
	DWORD	dwTime;
};

struct PlayerSwordSpriteInfo 
{
	WORD wSwordSpriteLv[5];
};

struct CHARACTER_ACCTACK_PROPERTY_Data
{
	DWORD dwUin;          
	WORD  job;              // 职业参数
	int   wAtkW;           // 外功
	int   wAtkN;           // 内功
	int   wPpW;            // 致命
	int   wPpN;             // 重击
	int   wApN;            // 命中
	int   nCriticalAtk;     // 致命穿透 
};

struct CHARACTER_FORCE_JUMP_TO_Data
{
	int mapIndex;
	int xPos;
	int yPos;
};

struct CHARACTER_SEND_FISH_SCORE_Data 
{
	int nScore;
	int	nTmpS;
};

struct CHARACTER_SEND_NEWSP_COIN_Data 
{
	int nCoin;
};

struct CHARACTER_SEND_CLIENT_TISHI_Data 
{
	BYTE nClientTiShi;
	bool nShenGong;
	bool nHoseLeader;
};

struct CHARACTER_OPEN_WORLD_MSG_FORM_Data 
{
	int pos;
	DWORD val;
};
struct CHARACTER_OPEN_FORM_BY_NAME_Data 
{
	char FormName[64];
};
struct CHARACTER_DEFENSE_PROPERTY_Data
{
	DWORD dwUin;          
	WORD  job;              // 职业参数
	int   wDefenseW;        // 外防
	int   wDefenseN;       // 内防
	int   wShanBi;          // 闪避
	int   wZhaoJia;         // 招架率
	int   wPierceDefense;      // 致命穿透 
};

struct CHARACTER_TRANSPORT_ADD_SUCCESS_Data
{
	TransPortData pTransportData;
};

struct CHARACTER_TRANSPORT_UPDATE_SUCCESS_Data
{
	int nIndex;
	TransPortData pTransportData;
};

struct CHARACTER_TRANSPORT_UPDATE_Data
{
	BOOL bIsRename;
	char szText[MAX_TRANSPORT_TEXT_LEN];
	int nIndex;
};

struct CHARACTER_REQUEST_SLAVEINFO_Data{
	DWORD dwSID;
};

struct CHARACTER_REQUEST_SDS_STATE_Data{
	DWORD dwEID;
};

struct CHARACTER_SEND_CLIENT_FLAGS_Data{
	int nCnt;
	BYTE	bVal[1];
};

struct CHARACTER_TRANSPORT_DELETE_Data
{
	int nIndex;
};

struct CHARACTER_TRANSPORT_START_Data
{
	int nIndex;
	BOOL bItemType;
};

struct CHARACTER_TRANSPORT_SLOT_ADD_Data
{
	int nIndex;
};

struct CHARACTER_TRANSPORT_SLOT_ADD_SUCCESS_Data
{
	int nIndex;
};

struct CHARACTER_TRANSPORT_LIST_Data
{
	int nCount;
	TransPortData pTransportDatas[10];
};

struct CHARACTER_HOME_PRODUCTS_Data
{
	HomeProducts products[HOME_PRODUCTS_TYPE_MAX];
};

struct CHARACTER_HOME_PRODUCTS_BANK_MONEY_Data
{
	int nBankMoney[HOME_PRODUCTS_TYPE_MAX];
	int	nCostCount[HOME_PRODUCTS_TYPE_MAX];
};

struct CHARACTER_HOME_PRODUCTS_LEVEL_Data
{
	BYTE byLevel[HOME_PRODUCTS_TYPE_MAX];
	int nCostCount[HOME_PRODUCTS_TYPE_MAX];
};

struct CHARACTER_HOME_PRODUCTS_STATE_Data
{
	BOOL bIsGrowing[HOME_PRODUCTS_TYPE_MAX];
};

struct CHARACTER_GAMEWORLD_REBATE_Data
{
	int		nShopRebate;			// 商店折扣 0-100
	int		nRepairRebate;			// 修理折扣 0-100
	int		nMoHuaRebate;			// 魔化折扣 0-100
	int		nQiangHuaRebate;		// 强化折扣 0-100
};

struct CHARACTER_FIND_PLAYER_Data
{
	DWORD	dwFromUIN;
	DWORD	dwTargetUIN;
	int		nLineIndex;
	int		nMapIndex;
	int		nPosX;
	int		nPosY;
	bool	bCanJump;
	char	szName[32];
};

struct CHARACTER_TRIG_TIME_Data
{
	DWORD dwTime;
};

struct CHARACTER_HONORS_VAL_Data
{
	BYTE	bPos;
	int		nVal;
};

struct CHARACTER_HONOUR_CHANGE_Data
{
	DWORD   uin;
	DWORD	dwHonourVal;
	BYTE	bHonourLvl;
	DWORD   dwHonourDay;
	DWORD   dwHonourWeek;
};

struct CHARACTER_CHAR_CREATE_Data 
{
	DWORD   dwAccountUin;
	char	szName[16];
	BYTE	bySex;
	BYTE	byCountry;             //
	BYTE	byHead;
	BYTE	byDefaultSkinType;
};

struct CHARACTER_CHAR_CREATE_FAILED_Data 
{
	BYTE	failedCode;
};

enum
{
	CHARACTER_JOIN_TYPE_NORMAL,
	CHARACTER_JOIN_TYPE_CHANGE_LINE_FOR_NONE,
	CHARACTER_JOIN_TYPE_CHANGE_LINE_FOR_INSTANCE,
	CHARACTER_JOIN_TYPE_REJOIN,
	CHARACTER_JOIN_TYPE_MAX,
};

struct CHARACTER_JOIN_Data
{
	DWORD dwAccountUin;
	DWORD dwUin;
	BYTE  bJoinType;
};

struct CHARACTER_DELETE_CHAR_Data
{
	DWORD dwAccountUin;
	DWORD dwUin;
};

struct CHARACTER_SET_DELETE_CHAR_TIME_Data
{
	DWORD dwAccountUin;
	DWORD dwUin;
};

struct CHARACTER_CANCEL_DELETE_CHAR_Data 
{
	DWORD dwAccountUin;
	DWORD dwUin;
};

struct CHARACTER_SEND_DELETE_CHAR_TIME_Data 
{
	DWORD dwAccountUin;
	DWORD dwUin;
	DWORD dwDeleteTime;
};

struct CHARACTER_SKILL_GROUP_TIMEOUT_Data 
{
	BYTE	type;                     // 1:timeout;  2:remove state halo
};

#define TRANSFER_PLAYER_TIME (24*60*60)

//	角色转移信息
struct PlayerTransferInfo
{
	DWORD dwBeginTransferTime;	//	开始转移的时间
	DWORD dwTargetAccountUin;	//	转移目标的账号UIN
};

struct SimpleInfo
{
	DWORD	dwUin;
	char	szName[16];
	BYTE	bySex;
	//BYTE    bySkinHead;
	BYTE	byLevel;
	BYTE	byCountry;
	BYTE	byJob;
	DWORD	dwExp;
	DWORD	dwLvlupexp;
	DWORD	dwHp;
	DWORD	dwMaxhp;
	DWORD	dwMp;
	DWORD	dwMaxmp;
	DWORD	dwDeleteTime;
	PlayerTransferInfo xTransferInfo;
	BYTE	byDefaultSkinType;
	//DWORD   dwOnlineScore;
	BYTE	bySkin[SKIN_MAX];
	BYTE    bHomeDuty;
};

struct CHARACTER_SIMPLEINFO_Data
{
	DWORD       dwAccountUin;
	int         nCount;
	SimpleInfo  pInfos[1];
};

struct CHARACTER_CHANGE_LINEINFO_Data
{
    DWORD   dwUin;
	char    szName[16];
	int     nStars;
	int     nFury;
};

struct CHARACTER_INFO_Data 
{
	DWORD	uin;
	char	name[16];
	DWORD	maxHp;
	DWORD	hp;
	BYTE	level;
	BYTE	country;
	BYTE	sex;
	BYTE	skin[SKIN_MAX];
	WORD	WeaponID;
	DWORD   lvlUp_exp;
	DWORD	exp;
	WORD	job;
	DWORD	skillexp;
	BYTE	bExpBall;
	//DWORD   dwOnlineScore;//在线积分
};

struct CHARACTER_PKINFO_Data 
{
	WORD	pkcount;    
};

struct CHARACTER_VIGOR_Data 
{
	int		nVal;    
};

struct CHARACTER_CLEARRS_CHGCTRY_Data
{

};

struct CHARACTER_TIME_CHECKMOVE_Data
{
	DWORD dwTime;
};

struct CHARACTER_REQUEST_ALLY_Data
{
	DWORD dwUin;
	char	szName[16];
};

struct CHARACTER_ATTRIB_Data 
{
	WORD	strength;			// 力
	WORD	constitution;		// 体
	WORD	intelligence;		// 智
	WORD	dexterity;			// 敏
	WORD	wisdom;				// 心
	WORD	attribPoint;
};

struct CHARACTER_PARAM_Data
{
	DWORD	uin;
	DWORD	HP;
	DWORD   maxHP;			// 精
	DWORD	MP;
	DWORD   maxMP;			// 神
};

struct CHARACTER_MP_Data 
{
	DWORD	uin;
	DWORD	MP;
};

struct CHARACTER_ATKDISTANCE_Data 
{
	WORD	Distance;
};

struct CHARACTER_JOB_Data 
{
	WORD	job;
};

struct CHARACTER_HP_Data 
{
	DWORD	uin;
	DWORD	HP;
};

struct CHARACTER_SP_Data 
{
	DWORD	uin;
	WORD	SP;
};

struct CHARACTER_EXP_Data 
{
	DWORD	exp;
};

struct CHARACTER_LEVEL_Data 
{
	DWORD	uin;
	BYTE	level;
	DWORD	exp;
	DWORD   lvlUp_exp;
	int		learnSkillId;
	BYTE	bExpBall;
};

struct CHARACTER_MAP_POSITION_Data 
{
	int		mapIndex;
	WORD	x, y;
	BYTE	zoneIndex;
	BYTE	owner;
	BYTE	force;
	BYTE	zoneType;
	bool	instance;
};

struct FightParam 
{
	int		atk;				// 攻击力
	int		def;				// 防御力
	int		ap;					// 命中率
	int		pp;					// 防御率
	int		dp;					// 躲避率

	//int		nAtkYY;				//	陰陽攻擊
	//int		nDefYY;				//	陰陽防御
	//// 五行属性
	//int		nJin;
	//int		nMu;	
	//int		nShui;
	//int		nHuo;
	//int		nTu;

	int		nCriticalAtk;
	int		nCriticalDef;
	int		nParry;
	int     nPvPAdd;
	int     nPvpReduce;
};

struct CHARACTER_FIGHT_PARAM_Data 
{
	FightParam	fparamW;
	FightParam	fparamN;
};

struct ShortCut 
{
	DWORD	id;
	BYTE	type;
};

struct CHARACTER_SHORTCUTS_Data 
{
	ShortCut xItems[40];
};

struct CHARACTER_SET_SHORTCUT_Data
{
	BYTE	bIndex;
	BYTE	bType;
	DWORD	wID;
};

struct CHARACTER_DEC_ONLINE_SCORE_Data
{
	DWORD dwDecOnlineScore;
};
struct CHARACTER_MARRY_INFO_Data 
{
	DWORD	uin;
	BYTE	marryFlag;
	DWORD	couplerUin;
	DWORD dwIntimacyVal;
	char	nickname[48];
};

struct CHARACTER_INTIMACY_INFO_Data 
{
	DWORD dwIntimacyVal;
};
struct CHARACTER_HOME_INFO_Data
{
	DWORD  dwUin;
	DWORD  dwHomeID;
	BYTE   byHomePosition;
	char   szHomeName[20];
	DWORD  dwHallID;
	BYTE   byHallDuty;
	char   szHallName[16];
	_CreateHome chLock;
};

struct CHARACTER_HONOUR_LVL_Data
{
	DWORD  dwUin;
	BYTE   bHonourLvl;
};

struct CHARACTER_ITEM_TO_NAME_Data
{
	DWORD  dwUin;
	BYTE   bItemToName;
};

struct CHARACTER_CHECK_PING_Data 
{
	DWORD	tickCount;
};

struct CHARACTER_PASS_CHECK_Data 
{
	DWORD	dwUin;
	char	pass[16];
};

struct CHARACTER_SKILL_TuiGong_Data 
{                  
	BYTE	proc;
};

struct CHARACTER_ACCOUNT_POINT_Data
{
	int		nGold[GOLD_TYPE_MAX];
	int		nSilver;
};

struct CHARACTER_ONLINE_TIMEMSG_Data 
{
	BYTE bStateType;
	BYTE bVal;
};

//struct CHARACTER_RELATIONSHIP_Data
//{
//	DWORD uin;
//	BYTE  type;
//	BYTE  RelationshipType;
//};

enum RELATION_TYPE
{
	RELATION_FRIEND,	//朋友
	RELATION_TEMP,
	RELATION_ENMITY,    //仇人
	RELATION_BLACKLIST,	//黑名单
	RELATION_ALLY
};

enum REFRESH_TYPE      //刷新
{
	REFRESH_JOB,       //职业
	REFRESH_LEVEL,     //等级
	REFRESH_MAPINDEX,  //地图
	REFRESH_NAME,
};
#define  CHAR_NAME_MAXLEN 20         // 最大名字长度
#define	 SLAVE_PICKUP_MAXNUM 5		//宠物拾取物品颜色数量

struct CHARACTER_FRIENDS_ADD_Data 
{
	DWORD  dwFriendUin;
	char   szFriendName[CHAR_NAME_MAXLEN];
    BYTE   byRelationType;
};

struct CHARACTER_SEND_PLAYER_COPY_CHALLENGE_COUNT_Data 
{
	BYTE bCount;
	BYTE bBuyCount;
};
struct CHARACTER_SEND_BOMB_BULLET_Data{
	int	num;
};

struct CHARACTER_REQ_WARM_SKILLUP_Data{
	WORD	wSkill;
};

struct CHARACTER_S_SEND_WM_INFO_Data{
	BYTE	bLv;
	DWORD	dwExp;
	int		nSPPoint;
	DWORD	dwCurHp;
	DWORD	dwDure;
	BYTE	bSCnt;
	BYTE	bUCnt;
	WORD	szSkill[1];
};

struct CHARACTER_SEND_SKILL_NEXT_LEARN_TIME_Data 
{
	DWORD dwTime;
};

struct CHARACTER_S_SEND_WM_SIMPLE_Data{
	DWORD	dwExp;
	DWORD	dwCurHp;
	int		nSPPoint;
	DWORD	dwDure;
};

struct CHARACTER_S_SEND_STTIME_Data{
	WORD	wBaseID;
	int		nTmSpan;
};
struct CHARACTER_C_REQUEST_STTIME_Data{
	WORD	wBaseID;
	BYTE	bIsLearn;
};

struct CHARACTER_SC_NEW_LUCKBOX_Data{
	BYTE	bStep;		//次数包0,客户端发开始1,服务器回结果2,客户端回得物品3
	BYTE	bTime;		//剩余次数
	DWORD	dwItem;		//随机到得物品
};

struct CHARACTER_SEND_SLAVE_COPY_CHALLENGE_COUNT_Data 
{
	BYTE bCount;
};

struct CHARACTER_FRIENDS_UPDATE_Data 
{
	DWORD  dwFriendUin;
	int	   nFriendship;
	short   wFriendshipLv;
	short   wExpTimes;
};

struct CHARACTER_FRIENDS_INGROUP_Data {
	DWORD relationUin;
};

struct CHARACTER_FRIENDS_SENDBLESS_Data 
{
	DWORD  dwUin;
	int   nExp;
};

struct CHARACTER_FRIENDS_SENDPRESENT_Data 
{
	DWORD	dwSendUin;
	DWORD	dwRecvUin;
	WORD	wPresentID;
	int		nCount;
	char	szSendName[CHAR_NAME_MAXLEN];
};

struct CHARACTER_FRIENDS_RECVPRESENT_Data
{
	DWORD	dwSendUin;
	DWORD	dwRecvUin;
	short   wPresentID;
	int		nCount;
	char	szRecvName[CHAR_NAME_MAXLEN];
	bool	bIsRecv;
};

struct CHARACTER_SEND_NEWBEGIFT_Data{
	BYTE	bNum;
	int		itemid;
	int     itime;	//剩余时间
};

struct CHARACTER_GET_NEWBEGIFT_Data{
	BYTE    bNum;
	int     itemid;
};

struct CHARACTER_DEL_NEWBEGIFT_Data{
	BYTE    bNum;
	int     itemid;
};

struct CHARACTER_FLUSH_NEWBEGIFT_Data{
	BYTE    bNum;
	int     itemid;
	int     itime;	//剩余时间
};

struct CHARACTER_FRIENDS_DEL_Data 
{
	DWORD  dwFriendUin;
	char   szFriendName[CHAR_NAME_MAXLEN];
	BYTE   byRelationType;
};

struct CHARACTER_FORBID_VERSION_Data
{
	DWORD	dwVersion;
};

struct CHARACTER_FORBID_UPDATE_Data
{
	char szText[1];
};

struct FriendsInfo
{
	DWORD   dwUin;
	char	szname[CHAR_NAME_MAXLEN];
	BYTE	isOnline;
	BYTE    byJob;	
	int    nMapIndex;
	BYTE    byLevel;
	BYTE    byRelationType;
	BYTE	bLineIndex;
};

struct CHARACTER_FRIENDS_REFRESH_Data
{
    DWORD   dwUin;
	BYTE    byType;
	WORD    wValue;
	char	szName[1];
};

struct CHARACTER_FRIENDS_SENDINFO_Data 
{
	FriendsInfo info;
};

struct CHARACTER_FRIENDS_SENDALL_Data
{
	BYTE	count;
	FriendsInfo info[1];
};

struct CHARACTER_FRIENDS_GETINFO_Data 
{
	DWORD	uin;
};

struct CHARACTER_FRIENDS_LOGINOFF_Data
{
	DWORD	fUin;
	BYTE	on;
	BYTE	type;
	BYTE	line;
};

struct CHARACTER_WAITING_QUEUE_Data
{
	DWORD	wQueueLenth;          //队列长度
	bool	bGetInfo;             //是否取得了全部玩家消息
};

struct CHARACTER_MOVE_INMAP_Data
{
	int		x;       
	int		y;         
};

struct CHARACTER_REFRESH_FRIEND_Data
{
	WORD	uin;
};

struct CHARACTER_ADD_FURY_Data
{
	int nStar;
	int nFury;
};

struct CHARACTER_CHANGE_LINE_Data
{
	WORD wServerID;
};

struct CHARACTER_ENTER_NEW_LINE_Data
{
	DWORD uin;
};

enum MCHANGE_NAME_STATUS
{
	MCHANGE_NAME_WAIT_INPUT,
	MCHANGE_NAME_INPUTED,
	MCHANGE_NAME_PASS,
};

struct CHARACTER_MPLAYER_CHANGE_NAME_Data
{
	DWORD dwUin;
	char  szName[256];
	BYTE  bChangeNameStatus;
};

struct CHARACTER_MPLAYER_CHANGE_HOME_NAME_Data
{
	DWORD dwUin;
	char szName[256];
	BYTE bChangeNameStatus;
};

struct CHARACTER_CLICK_SIGNAL_FLARE_Data
{
	BYTE bLineIndex;
};

struct CHARACTER_SIGNAL_FLARE_Data
{
	DWORD uin;
};

struct CHARACTER_HOME_SIGNAL_FLARE_Data
{
	DWORD uin;
};


struct CHARACTER_CALL_ALLY_Data
{
	DWORD uin;
	char  szName[CHAR_NAME_MAXLEN];	
	int  bMapIndex;
};

struct CHARACTER_HOME_CALL_ALLY_Data
{
	DWORD uin;
	char  szName[CHAR_NAME_MAXLEN];	
	int  bMapIndex;
};

struct CHARACTER_COME_FOR_ALLY_Data
{
	DWORD uin;
	BOOL bCome;
};

struct CHARACTER_HOME_COME_FOR_ALLY_Data
{
	DWORD uin;
	BOOL bCome;
};

struct CHARACTER_REFUSE_ALLY_Data
{	
	char  szName[CHAR_NAME_MAXLEN];
	BOOL  bCome;
};

struct CHARACTER_HOME_REFUSE_ALLY_Data
{	
	char  szName[CHAR_NAME_MAXLEN];
	BOOL  bCome;
};

struct CHARACTER_AUTOATTACK_SLOT_CHG_Data
{
	DWORD dwRemainCnt;
};

struct CHARACTER_AUTOATTACK_DATE_CHG_Data
{
	DWORD dwEndTime;
};

struct CHARACTER_AUTOATTACK_PERIOD_CHG_Data
{
	DWORD dwRemainTime;
};

struct CHARACTER_AUTOADV_REMAIN_Data
{
	DWORD dwRemainTime;
	DWORD dwGiveTime;
};

struct CHARACTER_OPEN_URL_Data
{
	char szUrl[256];
};

struct CHARACTER_SET_AUTO_MAP_Data
{
	BYTE byFLead;
	int	nMapIndex;
};



struct CHARACTER_AUTO_REPAIR_Data
{
	DWORD dwItemIndex;
	DWORD dwBoxIndex;
	BYTE dwItemFixType;		//0:特殊修理	1:普通修理
};

struct CHARACTER_SEND_TITLES_Data
{
	BYTE  bCount;
	WORD  wActiveTitleID;
	stTitleTime	stTitle[1];
};

struct CHARACTER_SEND_ADD_TITLE_Data
{
	WORD  wTitleID;
	DWORD	dwTime;
};

struct CHARACTER_SEND_DELETE_TITLE_Data
{
	WORD  wTitleID;
};

struct CHARACTER_SET_ACTIVE_TITLE_Data
{
	WORD  wTitleID;
};

struct CHARACTER_CS_SEND_TITLES_Data
{
};

struct CHARACTER_SHOW_MAP_WEATHER_Data
{
	BYTE	bWeatherType;
};

struct CHARACTER_PLAYER_TALENT_Data
{
	WORD	point;
};

struct CHARACTER_DO_SCRIPT_Data
{
	int		nID;
};

//向客户端发送任务信息
struct CHARACTER_SEND_QUESTINFO_Data    //used
{
	DWORD	uin;
	WORD QID;
	BYTE bNew;
	BYTE complete;
};
//向客户端发送放弃任务信息
struct CHARACTER_SEND_GIVEUPQUEST_Data  //used
{
	DWORD	uin;
	WORD QID;	
	WORD flag;//0 完成任务,1放弃任务
};
//从客户端接收放弃任务请求
typedef struct CHARACTER_GET_GIVEUPQUEST_Data  //used
{
	DWORD	uin;
	WORD QID;
	int Level;
}CHARACTER_SEND_SHOWQUEST_Data,
CHARACTER_GET_SHOWQUEST_Data,
CHARACTER_SEND_CHOOSEQUESTITEM_Data,
CHARACTER_SEND_INCOMPLETE_Data,
CHARACTER_ACCEPT_QUEST_Data;

struct CHARACTER_SEND_QUESTOPTION_Data    //used
{
	DWORD	uin;
	char NpcName[32];
	char Option[1];
};
//任务变量加值描述
struct CHARACTER_SEND_ADDQUESTVARVAL_Data   //used
{
	DWORD uin;
	WORD QID;
	char varName[8];
	short varVal;
};
//清除任务列表
typedef struct CHARACTER_SEND_CLEARQUEST_Data   //used
{
	DWORD uin;
} CHARACTER_SEND_CMD_REPLY_Data,
CHARACTER_CLIENT_STOP_MOVE_Data;
//发送任务变量
struct CHARACTER_SEND_QUESTVAR_Data        //used
{
	DWORD uin;
	WORD QID;
	char varName[8];		//变量名
	char varDes[50];		//变量描述
	short varVal;				//已完成值
	short varCompleteVal;	//完成值,当varVal==varCompleteVal时表示任务完成
	short varFailVal;			//失败值,当varVal==VarFailVal时表示任务失败
};
//设置任务变量值
struct CHARACTER_SEND_SETQUESTVARVAL_Data   //used
{
	DWORD uin;
	WORD QID;
	char varName[8];
	short varVal;
};

struct CHARACTER_GET_CHOOSEQUESTITEM_Data     //used
{
	WORD qid;
	char selectindex;
};

struct CHARACTER_SEND_FLAG_Data                 //used
{
	DWORD uin;
	int Size;
	WORD Data[1];
};

struct CHARACTER_S_LOG_CC_TICK_Data                 //used
{
    DWORD tick;
};
struct CHARACTER_CHAR_SEND_BOMB_INFO_Data 
{

	BYTE	byQuality;		//1阶1段-9阶9段	%9 段 /9 阶
	int		nQualityExp;	//暗器品质经验
	BYTE	byLevel;		//0-100
	BYTE	byMasteryLv;	//暗器精通等级
	//DWORD	dwBullet;		//子弹数目  0-1000
	DWORD	dwTime;			//剩余时间
	DWORD	index;
	DWORD	SkillUse;
};
struct CHARACTER_S_LOG_CC_INFO_Data                 //used
{
	bool bIsBigMode;
	bool bIsFunScreen;
	bool bIsHighFPS;
	BYTE byCharLevel;
};

struct CHARACTER_GETMALL_ITEM_Data
{
	DWORD	dwItemID;
	int		nCount;
	DWORD	dwUin;
};

//struct CHARACTER_FIGHTFORRES_Data
//{
//	int		mapIndex;
//	int		GlobalVar[4];
//};

struct CHARACTER_MAPVALUE_Data
{
	int		mapIndex;
	char	szForceName[32];
	char	szValue[1];
};

struct CHARACTER_LUA_MSGC2S_Data
{
	char	szFirst[32];
	char	szSecond[1];
};

struct CHARACTER_ADD_AWARD_Data
{
	DWORD	dwAwardID;
	DWORD	dwItemID;
	int		nCnt;
	bool	bNewAward;
	DWORD	dwLastTime;
	char	szTitle[1];
};

struct CHARACTER_GET_TIME_Data
{
	DWORD dwTmCur;
};

struct CHARACTER_GET_AWARD_Data
{
	DWORD	dwAwardID;
	bool	isGiveUp;
};


struct CHARACTER_FRIEND_LEVEL_UP_Data
{
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
	int		nLevel;
};

struct CHARACTER_REQUEST_ADD_VIP_LEVEL_Data
{
	bool bAdd;
};

struct CHARACTER_CONGRATULATIONS_LEVEL_UP_C_Data
{
	DWORD	dwUinCongratulateTo;
};

struct CHARACTER_CONGRATULATIONS_LEVEL_UP_CS_Data
{
	DWORD	dwUin;
	char	szName[MAX_NAMELEN];
};

struct CHARACTER_THANK_CONGRATULATIONS_LEVEL_UP_Data
{
	DWORD	dwUinThankTo;
};

struct CHARACTER_SET_RECORD_BATTLELOG_Data
{
	bool	bToRecord;
};

struct CHARACTER_SEND_RECORD_BATTLELOG_Data
{
	DWORD	dwUin;
	bool	bToRecord;
};

struct CHARACTER_MARRY_NICK_Data 
{
	DWORD	uin;
	char	nickname[48];
};

struct CHARACTER_SLAVE_PICKITEMS_Data
{
	BYTE	byCnt;
	int		sznIndex[1];
};

struct MallItemBuyCntInfo
{
	DWORD	dwItemId;
	WORD	wBuyCnt;
};

struct CHARACTER_SEND_MALLITEM_BUYCNT_INFOS_Data
{
	BYTE	byInfoCnt;
	MallItemBuyCntInfo info[1];
};

struct CHARACTER_SEND_MALLITEM_BUYCNT_INFO_Data
{
	MallItemBuyCntInfo info;
};

struct CHARACTER_MPLAYER_INPUT_SERIAL_NUM_Data
{
	DWORD dwUin;
	char  szURL[256];
	char  szSerialNum[256];
	BYTE  bChangeNameStatus;
};

struct CHARACTER_FRIEND_BEADD_Data
{
	DWORD	dwUin;
	char	szMsg[MAX_NAMELEN];
};

struct CHARACTER_INVITE_SETON_TRANSPORT_Data
{
	DWORD	dwHostUin;
	DWORD	dwPassengerUin;
};

struct CHARACTER_SEND_OFFLINE_TIME_2_USE_Data
{
	DWORD	dwOfflineTime;
	DWORD	dwTodayOfflineTime2Store;
};

struct CHARACTER_TOP_LEVEL_CONTROLLER_INFO_Data
{
	DWORD dwLevel;
	DWORD dwTickCnt;
};

struct CHARACTER_BOOST_CMD_Data
{
	int		nSrcMapIndex;
	POINT	ptPos;
	int		nEnemyPlayerCountry;
	int		nPosIndex;
	char	szDeadPlayerName[MAX_NAMELEN];
	char	szEnemyPlayerName[MAX_NAMELEN];
};

struct CHARACTER_ANSWER_BOOST_CMD_Data
{
	int		nPosIndex;
};

struct CHARACTER_REQUEST_VIEW_EQUIP_Data{
	DWORD dwRequestUin;
	DWORD dwRespondUin;
};

struct CHARACTER_RESPOND_VIEW_EQUIP_Data{
	DWORD dwRequestUin;
	DWORD dwRespondUin;
};

struct CHARACTER_BATTLE_STATUS_Data
{
	bool blInBattle;
};

struct CHARACTER_SKILL_EXP_Data
{
	DWORD exp;
};

struct CHARACTER_HONOUR_DAYWEEK_Data
{
	DWORD uin;
	BYTE  bDayWeekFlag;
	DWORD dwVal;
};

struct CHARACTER_MASTER_ADD_Data
{
	BYTE byLevel;
	DWORD dwMasterUin;
	char  szName[20];
};

struct CHARACTER_TUDI_ADD_Data
{
	int   iTimeLimit;
	DWORD dwTudiUin;
	char  szName[20];
};

struct CHARACTER_MASTER_REFUSE_Data
{
	DWORD dwMasterUin;
	char  szName[20];
};

struct CHARACTER_TUDI_REFUSE_Data
{
	DWORD dwTudiUin;
	char  szName[20];
};

struct CHARACTER_MASTER_SURE_Data
{
	DWORD dwMasterUin;
};

struct CHARACTER_TUDI_SURE_Data
{
	DWORD dwTudiUin;
};

struct CHARACTER_SEND_ITEM_MARKET_BILL_Data
{
	DWORD dwItemMarketBill;
};

struct CHARACTER_SEND_NA_BILL_Data
{
	DWORD  dwNaGold;
	DWORD  dwNaBindGold;
};

struct CHARACTER_SEND_NA_BILL_TIP_Data
{
	int nBill;
	int nBindBill;
};

struct CHARACTER_SEND_ADD_ACTER_POINT_Data
{
	DWORD dwItemID;	
	BYTE  bNum;
	char  szName[40];
};

struct CHARACTER_SEND_GIVEOVER_HOMEMONEY_Data
{
	DWORD dwHomeBankMoney;
	char szName[40];
};

struct CHARACTER_SEND_GIVEOVER_HOMEGOLD_Data
{
	DWORD dwHomeBankGold;
	char szName[40];
};
struct CHARACTER_SEND_SWORDSPRITE_INFO_Data  
{
	PlayerSwordSpriteInfo minfo;
};
struct CHARACTER_SHOWPUSHMSG_Data
{
	BYTE bFlag;
	BYTE pText[40];
};

struct CHARACTER_TRADE_SURE_Data
{
	
};

struct CHARACTER_SENDTRADE_FLAG_Data
{
	DWORD dwUin;
};

struct CHARACTER_SEND_VIP_INFO_Data 
{
	BYTE bLevel;
	bool bAvailable;
	DWORD dwTime;
};

struct CHARACTER_OPEN_ACTER_MENU_Data
{
	DWORD dwItemID;
};

struct CHARACTER_ACTERINFO_Data
{
	char szName[40];
	int  nNum;
};

struct CHARACTER_LOCK_CLIENT_CTRL_Data
{
	BYTE	bLock;
};

struct CHARACTER_EXPADD_FROM_FIRSTLV_Data
{
	BYTE	bExpFLv;
};

struct CHARACTER_PROTECT_VALUE_Data
{
	int    nValue;
	int    nMaxValue;
};

struct CHARACTER_ASK_JUMPTO_ACT_Data
{
	DWORD	dwTmRm;
	BYTE	bType;
	char	szMsg[1];
};

struct CHARACTER_ANSWER_JUMPTO_ACT_Data
{
	DWORD	dwSourceMapIndex;
};

struct CHARACTER_S_PLAYERFLAGS_Data
{
	int		nSvrVersion;
	int		nCnt;
	int		nCnt2;
	BYTE	szbyVal[1];
};

struct CHARACTER_S_PLAYERFLAG_POS_Data
{
	WORD	wPos;
	BYTE	byVal;
};

struct CHARACTER_SEND_VIEW_SLAVE_Data{
	BYTE	pData[1];
};

struct CHARACTER_SCORE_GOLD_OPEN_BANK_CNT_Data
{
	bool blGoldFg;
};

struct CHARACTER_OPEN_NPC_WAR_MENU_Data
{
	BYTE bNpcFlag;
};

struct CHARACTER_CUR_ONLINE_SCORE_Data
{
	DWORD uin;
	DWORD dwCurOnlineScore;
};

struct CHARACTER_CLEAR_ADD_HOME_UP_TIME_Data
{
	DWORD uin;
	DWORD val;
};

struct CHARACTER_ASK_JUMPTO_HOMEWAR_Data
{
	WORD	wHomeID;
	DWORD	dwTmRm;
	BYTE	bType;
	char    szTitile[32];
	char	szContent[1];
};

struct CHARACTER_ANSWER_JUMPTO_HOMEWAR_Data
{
	WORD	wHomeID;
};

struct CHARACTER_ASK_QUIT_WAR_Data
{
	DWORD   dwTimeRemain;
	char    szTitile[32];
	char	szContent[1];
};

struct CHARACTER_ANSWER_QUIT_WAR_Data{
	
};

struct CHARACTER_ASK_GOTO_PLACE_Data
{
	int		nMap;
	int		nX;
	int		nY;
	DWORD   dwTimeRemain;
	char    szTitile[32];
	char	szContent[1];
};

struct CHARACTER_ANSWER_GOTO_PLACE_Data{
	int		nMap;
	int		nX;
	int		nY;
};

struct CHARACTER_DISMISSHOME_OPEN_LOCK_Data
{
	char szPass[20];
};

struct CHARACTER_DISMISSHOME_RESULT_Data
{
	bool bReslut;
};

struct CHARACTER_ITEMOPERATION_OPEN_LOCK_Data
{
	BYTE bItemOperationType;
	char szPass[20];
};

struct CHARACTER_SLAVEOPERATION_OPEN_LOCK_Data
{
	char szPass[20];
};

struct CHARACTER_SLAVEOPERATION_RESULT_Data
{
	bool bReslut;
	BYTE bType;
};

struct CHARACTER_ITEMOPERATION_RESULT_Data
{
	bool bReslut;
	BYTE bType;
};

struct CHARACTER_SEND_ARENA_SCORE_Data 
{
	int nArenaScore;
};

struct CHARACTER_SEND_HSTATES_INFO_Data 
{
	BYTE	byHState[8];
};

struct CHARACTER_SEND_ACTEX_POINT_Data 
{
	DWORD dwActExPoint;
	BYTE	bActOpened;
};

struct CHARACTER_SEND_ACTER_OK_Data
{
	char szName[40];
	int  nNum;
	bool bSex;
};

struct CHARACTER_SEND_ACTER_NO_Data
{

};

struct CHARACTER_SOAP_SETPLAYERFLAG_Data
{
	char	szFlag[20];
	int		nVal;
};

struct CHARACTER_C_SWITCH_SPCSTATE_Data
{
	WORD	wState;
	BYTE	bSwitch;
};

struct CHARACTER_SEND_ARENA_BATTLEINFO_Data 
{
	int nArenaWinBattle;
	int nArenaTotalBattle;
};

struct CHARACTER_ASK_JUMPTO_HOMEQUEST_Data
{
	WORD	wHomeID;
	DWORD	dwTmRm;
	BYTE	bType;
	//char    szTitile[32];
	//char	szContent[1];
};

struct CHARACTER_ASK_JUMPTO_HOMEQUEST2_Data
{
	WORD	wHomeID;
	DWORD	dwTmRm;
	BYTE	bType;
	char    szTitile[32];
	char	szContent[1];
};

struct CHARACTER_ANSWER_JUMPTO_HOMEQUEST_Data
{
	WORD   wHomeID;
};

struct CHARACTER_ANSWER_JUMPTO_HOMEQUEST2_Data
{
	WORD   wHomeID;
};
struct CHARACTER_SEND_ITEM_MARKET_BILL_TIP_Data
{
	int	  nBill;
};

struct CHARACTER_C_REQUEST_HOMEEQUIP_Data
{
	BYTE	  bVal;
};

struct CHARACTER_CS_SERVER_START_Data
{
	int		nVal;
};

struct CHARACTER_HOME_DUTY_CHANGE_Data
{
	DWORD   dwUin;
	BYTE   bDuty;
};
struct CHARACTER_S_SEND_LUCK_PRICE_ITEM_Data 
{
	DWORD	dwItemID;
	int		nPrice;
	int		nPriceMin;
	int		nPriceMax;
	DWORD	dwTime;
	BYTE	bState;
};
struct CHARACTER_BUY_LUCK_ITEM_Data
{
	DWORD dwUin;
};
#pragma pack (pop)
#endif