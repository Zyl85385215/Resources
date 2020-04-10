#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "UserRecord.h"

#define UW_EXP_VALUE	1000000000

enum
{
	UW_RMB,
	UW_GOLD,
	UW_LV,
	UW_EXP,
	UW_POW,
	UW_SKILLPT,
	UW_VIPLV,
	UW_VIPEXP,
	UW_GSLAVELMT,
	UW_HONOUR,

	UW_STARLV,

	UW_BINDRMB,
	UW_LIFE_STARLV,
	UW_EXP_CNT,

	UW_MAX,
};

enum
{
	UW_GOLD_NULL,		//0
	UW_GOLD_TUMO,		//屠魔
	UW_GOLD_ADD,		//命令添加
	UW_GOLD_MARRY,		//结婚奖励
	UW_GOLD_MENTOR,		//师徒奖励
	UW_GOLD_NASALE,		//寄售行
	UW_GOLD_DREAM,		//翡翠梦境
	UW_GOLD_KILL_ENEMY,		//杀怪
	UW_GOLD_INVICODE,		//邀请码
	UW_GOLD_MCOPY,		//副本
	UW_GOLD_MAIL,		//邮件
	UW_GOLD_QUEST,		//任务奖励
	UW_GOLD_TEST,		//答题
	UW_GOLD_ZUAN,		//幸运大转盘
	UW_GOLD_AWARD,		//大转盘领奖
	UW_GOLD_LUCK_AWAED,		//矿石井
	UW_GOLD_VIP_GIFT,		//VIP礼包
	UW_GOLD_EXPCOPY,		//远征副本ExpeditionCopy
	UW_GOLD_ACHIEVENT,		//成就奖励
	UW_GOLD_ONLINEAWARD,		//在线奖励
	UW_GOLD_LVUPAWARD,		//升级奖励
	UW_GOLD_LOGINGIFT,		//累计登陆
	UW_GOLD_RECHARGE_AWARD,		//首冲奖励
	UW_GOLD_SIGUP_MOUTH,	//每月签到
	UW_GOLD_DEGREEAWARD,	//活跃度奖励

};

class DBControl;
class UserData;
class UserWealth
{
public:
	UserWealth(UserData* pUser);
	~UserWealth();

	int		GetWealth(BYTE bEnum)	{return	m_nWealths[bEnum];}

	bool	SubWealth(BYTE bEnum, int nVal,BYTE byOrigin = RMB_SAVE_NORMAL);		//不够返回false
	bool	AddWealth(BYTE bEnum, int nVal,BYTE byOrigin = RMB_SAVE_NORMAL,char* szDesc = NULL,BYTE byType = UW_GOLD_NULL);		//byOrigin，钻石或绑钻的消耗或增加，需写上原因
	bool	DBInitWealth(BYTE bEnum, int nVal);
	void	SendWealthSingle(BYTE bEnum , int nChangeVal,DWORD dwUin);

	void	SetExpWealthVal(DWORD);
	DWORD	GetExpWealthVal();
	bool	SubExpWealth(BYTE bEnum, DWORD nVal,BYTE byOrigin = RMB_SAVE_NORMAL);		//不够返回false

	void	SetTotalSend();

	int		operator[](BYTE bEnum)	{return m_nWealths[bEnum];}

	void	CheckSaveDB(DBControl* pDB,DWORD	dwUin,DWORD	dwAcc);
	void	SaveToDB(DBControl* pDB,DWORD	dwUin,DWORD	dwAcc);
	bool	SetWealth(BYTE bEnum, int nVal);
private:
	int		m_nWealths[UW_MAX];
	int		m_sznChgVal[UW_MAX];
	bool	m_bChged[UW_MAX];

	int		m_nGoldChangeVal;
	int		m_nExpChangeVal;
	UserData* m_pUser;
};