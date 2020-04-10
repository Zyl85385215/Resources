#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "LuaStateManager.h"
#include "LuaClassObject.h"

enum	USER_RECORD_ACTION	//操作动作
{
	UA_NULL,
	UA_ENTER,		//进入
	UA_LEAVE,		//离开
	UA_OPREATE,		//操作
	UA_ATTACK,		//攻击
	UA_BEATTACK,	//被攻击
	UA_HEAL,		//治疗
	UA_BEHEAL,		//被治疗
	UA_KILL,		//杀死
	UA_BEKILLED,	//被杀死
	UA_RELIVE,		//复活
	UA_BERELIVE,	//被复活
	UA_GAIN,		//获得
	UA_LOST,		//失去
	UA_USE,			//使用
	UA_COMPLETE,	//完成
	UA_SEND,		//发送
	UA_MAX,
};

enum	USER_RECORD_OBJECT	//操作对像
{
	UO_NULL,
	UO_MAP,			//地图
	UO_MYSELF,		//自己
	UO_PET,			//宠物
	UO_HORSE,		//坐骑
	UO_PLAYER,		//玩家
	UO_ITEM,		//物品
	UO_MAPCOPY,		//副本
	UO_QUEST,		//任务
	UO_MESSAGE,		//消息
	UO_MAIL,		//邮件
	UO_CROPS,		//农作物
	UO_ENEMY,		//怪物
	UO_ACTIVITY,	//活动
	UO_TIME,		//时间
	UO_MAX,
};

class URInfoObj
{
public:
	BYTE	bAction;		//动作
	BYTE	bObject;		//对象
	DWORD	dwTick;			//操作时间
	int		nID;			//ID
	int		nVal;			//额外值（不同对象不同）
};

enum{
	RMB_SAVE_NULL,
	RMB_SAVE_NORMAL,		//默认，也就是未知
	RMB_SAVE_RECHARGE,		//充值
	RMB_SAVE_SHOPBUY,		//商城购买
	RMB_SAVE_DEBUGCODE,		//DEBUG命令
	RMB_SAVE_FORT_UNLOCKMANOR,	//解锁庄园
	RMB_SAVE_SIGNUP_MOUNTH,		//每月签到
	RMB_SAVE_EXPANDBAG,		//扩展包裹
	RMB_SAVE_ONLINEAWARD,		//在线奖励
	RMB_SAVE_LVUPAWARD,		//升级奖励
	RMB_SAVE_RECHARGEAWARD,		//首充奖励
	RMB_SAVE_DEGREEAWARD,		//活跃度奖励
	RMB_SAVE_RESETSKILL,		//技能点重置
	RMB_SAVE_MCCARD,		//副本翻牌
	RMB_SAVE_GAMBLE,		//赌搏机
	RMB_SAVE_TURNTABLE,		//大转盘
	RMB_SAVE_SLAVE_GET,		//抽宠物
	RMB_SAVE_HORSE_CULTIVATE,		//坐骑培养
	RMB_SAVE_GREENMODE,		//翡翠梦境

	RMB_SAVE_TPCOMMPAY,		//充值
	RMB_SAVE_ACHIEVEMENT,	//成就奖励
	RMB_SAVE_MARRY,	//结婚
	RMB_SAVE_INVITE,
	RMB_SAVE_SCRPIT,
	RMB_SAVE_WAR_RANK,	//竞技场
	RMB_SAVE_FAMILY,	//军团
	RMB_SAVE_EQUIP,	//修装备
	RMB_SAVE_MAKE_SKILL,	//生活技能
	RMB_SAVE_MAKE_POWER,	//生活精力
	RMB_SAVE_LOGINGIFT,	//累计登陆
	RMB_SAVE_MENTOR,	//师徒
};

static char* szRmbSaveName[] = {"无（下线）",
	"未知","充值","商城购买","debug命令","解锁庄园",
	"每月签到(补签或奖励)","扩展包裹","在线奖励","升级奖励","首充奖励",
	"活跃度奖励","技能点重置","副本翻牌","挖矿机","大转盘",
	"抽宠物","坐骑培养","翡翠梦境","充值","成就奖励",
	"结婚相关","邀请码奖励","活动脚本加","竞技场","军团","修理装备",
	"生活技能","生活精力","累计登陆","师徒"};


enum{
	SLAVE_SAVE_NORMAL,
	SLAVE_SAVE_GETPET,
	SLAVE_SAVE_REMOVEPET,
	SLAVE_SAVE_GETHORSE,
	SLAVE_SAVE_REMOVEHORSE,
	SLAVE_SAVE_PET_LVUP,
	SLAVE_SAVE_HORSE_LVUP,
	SLAVE_SAVE_PET_EXCHANGE,
	SLAVE_SAVE_PET_VARIATY,
	SLAVE_SAVE_PET_VFAIL,
};

static char* szSlaveSaveName[] = {"未知","获得宠物","移除宠物","获得坐骑","移除坐骑","宠物升级","坐骑升级","宠物进化","宠物变异","宠物变异失败"};

//输出日志以及部分特殊事件
class UserData;
class ItemObj;
class SlaveBaseData;
class HorseBaseData;
class UserRecord:public CLuaClassObject<UserRecord> 
{
public:
	UserRecord(UserData* pUser);
	~UserRecord();

	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);

	void		AddRecord(BYTE	bUA,BYTE byUO, int nID, int nVal);
	void		Update();
	//查询一定时间内的某个操作
	bool		CheckRecord(BYTE bUR, int nTick);

	bool		_checkDegree(URInfoObj* pObj);
	bool		_checkAchievement(URInfoObj* pObj);

	xnList*		m_pList;	//保留一定长度/时间
	UserData*	m_pParent;

	///////////////////////////LUA函数/////////////////////////////////
	bool		RunLuaQuestFunc(BYTE byAction,int nQID,BYTE byType);
	bool		RunLuaChatFunc(BYTE byChannel);
	bool		RunLuaDegreeFunc(int nPoint);
	bool		RunLuaActivityFunc(int nID,int nVal1,int nVal2);
	bool		RunLuaKillEnemyFunc(int nID,bool ifWBoss);
	bool		RunLuaRelationFunc(int nAction,int nTarid);
	bool		RunLuaFamilyFunc(int nAction,int nVal);
	bool		RunLuaStarFunc(int nLv);
	bool		RunLuaEquipFunc(int nAction,ItemObj* pItem,ItemObj* pItem2);
	bool		RunLuaPetFunc(int nAction,SlaveBaseData* pPet);
	bool		RunLuaHorseFunc(int nAction,HorseBaseData* pPet,int nNum);
	bool		RunLuaMapCopyFunc(int nAction,int nID,int nLevel,int nScore);
	bool		RunLuaPVPFunc(int nCount);
	bool		RunLuaWealthFunc(int nType,int nGet,int nTotal);
	bool		RunLuaSkillFunc(int nAction,int nID,int nType,int nLv);
	bool		RunLuaFarmFunc(int nAction,int nVal);
	bool		RunLuaDeadFunc();
	bool		RunLuaMarryFunc(BYTE byAction,int nTarget,int nPoint1,int nPoint2);

	//////////////////////////日志数据/////////////////////////////////
	void		SaveAllLog();

	bool		AddGoldLog(int nVal);
	bool		AddGoldFromLog(int nAddVal, int nVal, BYTE byType);
	bool		AddExpLog(int nVal,BYTE byLvl);
	bool		AddRmbLog(int nLeftVal,int nVal,BYTE byType,char* szDesc=NULL);
	bool		AddBindRmbLog(int nLeftVal,int nVal,BYTE byType);
	bool		AddItemLog(int nItemID,int nCount,int nType);
	bool		AddSlaveLog(int nID,int nBaseid,BYTE bylvl,BYTE byVariatlv,BYTE byAction);
	bool		AddGMCodeLog(const char* szMsg);
	bool		AddItemChangeLog(int nItemID,int nCount,int nFrom,BYTE byType);
	bool		AddLvupLog(BYTE byLv);
	bool		AddEquipStarLog( int nItemID );
	bool		AddEquipStarLogEx( int nItemID,int nCnt,int nStarLv );
	bool		AddSlaveLvLog( int nSlaveID,int nBaseID,int nPos,int nPosLv  );
	bool		AddFortLog( int nChgScore,int nWarScore,int nCurPos,int nManorLv ,int nManorExp, BYTE byType );
};

class WorldLog
{
public:
	WorldLog();
	void		UpdateSec();
	void		SaveAll();
	bool		SaveRankLog();
	bool		SaveUserInfoLog();
	bool		SaveWarRankLog();

	bool		ifSave;
	DWORD		dwSaveTick;
};