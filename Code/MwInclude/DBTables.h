#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 数据库版本                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define DB_VERSION		2014091806
#define DB_LOG_VERSION	2014072301


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 游戏数据库                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DB_DATA_VERSION				"DBDataVersion"								//数据库表版本号

#define RPG_ACCOUNT_TABLE			"RpgAccountData"

#define RPG_INVITE_TABLE			"RpgInviteData"
#define RPG_ITEM_TABLE				"RpgItemData"				//物品数据
#define RPG_EQUIPEX_TABLE			"RpgEquipExData"				//装备额外数据
#define RPG_USER_TABLE				"RpgUserData"				//角色主数据
#define RPG_SLAVE_TABLE				"RpgSlaveData"		//已弃用
//#define RPG_EQUIP_TABLE				"RpgEquipData"				//装备数据
#define	RPG_FRIEND_TABLE			"RpgFriendData"
#define	RPG_MAIL_TABLE				"RpgMailData"
#define RPG_HOME_TABLE				"RpgmilyData"
#define RPG_NEWSLAVE_TABLE			"RpgNewSlaveData"
#define ACTIVATE_TABLE				"Activate"
#define RPG_SKILL_TABLE				"SkillData"
#define RPG_STATE_TABLE				"StateData"

#define RPG_QUEST_TABLE				"RpgQuestData"			//进行中的任务列表
#define RPG_WORLDTEST_TABLE			"RpgTestData"			//世界答题

#define RPG_FORT_TABLE			"RpgFortData"			//要塞
#define RPG_ITEM_MANAGE_TABLE		"RpgItemManageData"			//物品(用于邮件,寄售行)
#define RPG_SYS_MAIL_TABLE		"RpgSysMailData"			//系统邮件

#define RPG_RANK_WAR_TABLE			"RpgWarRank"			//竞技场

#define RPG_RANKDATA_TABLE		"RpgRankData"			//排行榜
#define RPG_USERINFO_TABLE		"RpgUserInfoData"		//排行榜人物信息
#define RPG_UWATCH_TABLE		"RpgUWatchData"		//排行榜装备信息

#define RPG_NASALE_TABLE		"RpgNASale"			//寄售行
#define RPG_WORLDFLAG_TABLE		"RpgWorldFlag"			//世界变量

#define RPG_MENTORING_TABLE		"RpgMentoringData"		//师徒
#define RPG_MREQUEST_TABLE		"RpgMRequestData"		//师徒申请

#define RPG_ACHIEVEMENT_TABLE	"RpgAchievementData"	//成就

#define RPG_MARRY_TABLE			"RpgMarryData"		//结婚
#define RPG_MARRYREQUEST_TABLE		"RpgMarryRequestData"		//征婚申请

#define RPG_IOSAD_TABLE			"iosADSupport"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 服务器监控数据                                            ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	MARK_REGACCOUNT_TABLE		"MkRegAccount"		//每日注册的账号量表
//#define	MARK_LOGINACCOUNT_TABLE		"logLoginAccount"		//每日登录账号量表,今日没登录过才可计入
#define MARK_ONLINECNT_TABLE		"MkOnlineCnt"		//在线用数,每5分钟记录一份,分新老用户, 某项为当前用户数
#define MARK_RMBCOST_TABLE			"MkRmbCost"			//充值,消耗,金币,绑钻 统计,总, 周, 日//商城物品的销售量统计,总, 周, 日
#define MARK_PAY_TABLE				"MkPay"	
#define MARK_LOGINOUT_TABLE			"MkLoginOut"	//登陆、退出	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 日志数据库                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	RPG_ITEM_LOG				"RpgItemLog"						
#define RPG_ACTION_LOG				"RpgActionLog"					//行为日志
#define RPG_GOLD_LOG				"RpgGoldLog"
#define RPG_RMB_LOG					"RpgRmbLog"
#define RPG_BINDRMB_LOG				"RpgBindRmbLog"
#define RPG_EXP_LOG					"RpgExpLog"
#define RPG_SLAVE_LOG				"RpgSlaveLog"
#define RPG_GMCODE_LOG				"RpgGMCodeLog"
#define	RPG_ITEMCHANGE_LOG			"RpgItemChangeLog"						
#define	RPG_PAYERROR_LOG			"RpgPayErrorLog"						
#define	RPG_LUA_LOG					"RpgLuaLog"						

#define MARK_QICNT_LOG				"MkQICntLog"
#define RPG_LVUP_LOG				"RpgLvupLog"
#define RPG_RANK_LOG				"RpgRankLog"
#define RPG_USERINFO_LOG				"RpgUserInfoLog"
#define RPG_MAIL_LOG				"RpgMailLog"
#define RPG_GOLD_FROM_LOG			"RpgGoldFromLog"
#define RPG_EQUIP_STAR_LOG			"RpgEquipStarLog"
#define RPG_EQUIP_STAREX_LOG			"RpgEquipStarExLog"
#define RPG_SLAVE_LV_LOG			"RpgSlaveLvLog"
#define RPG_FORT_LOG				"RpgFortLog"
#define RPG_WAR_RANK_LOG			"RpgWarRankLog"

struct DB_Colume 
{
	char *	szName;
	char *	szType;
	char *	szDefault;
};

//账号服务器维护,每日
static DB_Colume	MkRegAcc_Colume[] =
{
	//{"id",			"int unsigned"},	//自增加的编号
	{"daynumber",		"int unsigned"},	//日期编号
	{"regcount",		"int unsigned"},	//注册人数  登录人数-注册人数 就是老用户数
	{"ativatecount",		"int unsigned"},	//激活人数
	{"createcount",		"int unsigned"},	//创建角色人数
	{"effcount",		"int unsigned"},	//有效用户人数
	{"payusercount",		"int unsigned"},	//新增付费用户人数
	{"paytotalcount",		"int unsigned"},	//付费
	{"regcount",		"int unsigned"},	//注册人数  登录人数-注册人数 就是老用户数
	{"logincount",		"int unsigned"},	//登录人数,排除重复账号
	{"logintotal",		"int unsigned"},	//登录次数			
	{"rmbpay",			"int unsigned"},	//充值总数
	{"tpmark",			"smallint"},		//平台编号
	{"daypayusercount",		"int unsigned"},	//付费用户人数
	{"daypaytimecount",		"int unsigned"},	//付费次数
	{"daypaytotalcount",		"int unsigned"},	//付费数
};
//游戏服务器维护
static DB_Colume	MkOnline_Colume[] =
{
	//{"id",			"int unsigned"},	//自增加的编号
	{"timemark",		"varchar(20)"},		//时间编号, "now"为实时更新, 其他5分钟记录一个 20140302-15:02
	{"newaccount",		"int unsigned"},	//新注册在线
	{"oldaccount",		"int unsigned"},	//老用户在线
};

static DB_Colume	MkRmbCost_Colume[] =
{
	//{"objid",			"int unsigned"},		//0钻石,  1帮钻, 2金币 , 其他物品ID销售量, RMB服务器类的销售量
	{"totalcnt",			"int unsigned"},	//金币以100为单位
	{"weekcnt",			"int unsigned"},
	{"daycnt",			"int unsigned"},
	{"checktime",		"datetime"},		//检测隔天的时间
};

static DB_Colume	IosADSup_Colume[] =
{
	//{"objid",			"int unsigned"},	
	{"macad",			"varchar(32)"},	//金币以100为单位
	{"idfa",			"varchar(50)"},
	{"exmsg",			"varchar(250)"},
	{"state",			"int"},
	{"tpname",			"varchar(16)"},	//dianru
	{"linktime",		"datetime"},
	{"createtime",		"datetime"},
};

static DB_Colume	Account_Colume[] =
{
	//{"AccountUin",		"int unsigned"},

	{"accname",			"varchar(32)"},
	{"accpswd",			"varchar(16)"},
	{"createtime",		"datetime"},
	{"logintime",		"datetime"},

	{"rmbmoney",			"int"},
	{"gmlv",			"int","0"},
	{"beinvitecode",	"varchar(12)"},		//模板ID
	{"rmbtotal",			"int","0"},
	{"tpid",			"smallint","0"},
	{"rechargetemp",	"int",	"0"},
	{"macaddress",		"varchar(32)"},
	{"temprmbpay",			"int unsigned"},	//临时的累积充值
	{"accstate",			"int unsigned"},	//封号1

	{"apntoken",		"varchar(80)"},
	{"sysver",			"int"},
	{"idfa",		"varchar(50)"},
	{"fromad",			"smallint","0"},
	{"loginip",		"varchar(20)"},
	{"mainaccpswd",			"varchar(16)"},		//修改密码功能,主服务器用这个
};

static DB_Colume	RpgWarRank_Colume[] =
{
	//{"pos",		"int unsigned"},
	{"uin",				"int unsigned"},
	{"lv",				"int unsigned"},
	{"name",	"varchar(16)"},
	{"job",				"int unsigned"},
	{"sex",				"int unsigned"},
	{"score",				"int unsigned"},
	{"wincnt",				"int unsigned"},
	{"param",	"BLOB"},
};
static DB_Colume	ActiveTable_Colume[] =
{
	//{"id",		"int unsigned"},
	{"type",				"int unsigned"},
	{"state",				"int unsigned"},
};
static DB_Colume RpgUser_Colume[] =
{
	//{"Uin",				"int unsigned"},
	{"AccountUin",		"int unsigned"},

	{"lvl",			"int unsigned"},
	{"exp",			"int unsigned"},
	{"expcnt",		"smallint"},
	{"job",			"int unsigned"},
	{"country",			"int unsigned"},
	{"sex",			"int unsigned"},

	{"gold",			"int unsigned"},
	{"power",			"int unsigned"},
	{"skillpt",			"int unsigned"},

	//{"playerFlag",		"longBLOB"},
	{"LoginTime",		"datetime"},
	{"LogoutTime",		"datetime"},
	{"Nickname",		"varchar(20)"},
	{"nextitemid",		"int unsigned"},

	{"homeid",	"int unsigned"},
	{"homeduty",	"int unsigned"},
	{"flag",	"longBLOB"},
	{"viplvl",			"int unsigned"},
	{"slaveunlock",		"smallint"},
	{"equiphorse",		"int unsigned", "0"},

	{"mapindex",		"smallint"},
	{"mapx",			"smallint"},
	{"mapy",			"smallint"},
	{"rmindex",			"smallint"},	//重生点地图
	{"rmposx",			"smallint"},	//重生点地图
	{"rmposy",			"smallint"},	//重生点地图
	{"paramhp",			"smallint"},	//当前血量参数	,才6位不要超过65535/2

	{"cfgflag",			"longBLOB"},	//成就达成
	{"slimitflag",		"BLOB"},
	{"questflag",		"longBLOB"},
	{"cdflag",			"longBLOB"},

	{"hp",				"int unsigned"},
	{"mp",				"int unsigned"},
	{"bagsize",				"smallint","40"},
	{"vipexp",			"int unsigned"},
	{"getslavelimit",	"int unsigned","0"},
	{"honour",			"int unsigned","0"},
	{"homecontribution",	"int unsigned","0"},
	{"homeskill",	"BLOB"},
	{"sysmailid",			"int unsigned",		"0"},
	{"activedegree",	"longBLOB"},
	{"degreelevel",		"smallint"},
	{"fastitem",			"int unsigned",		"0"},
	{"daylyonline",			"int unsigned",		"0"},
	{"starlv",			"int unsigned",		"0"},
	{"activepet",		"int unsigned", "0"},
	{"expmultiple",	"BLOB"},
	{"rmbbind",			"int","0"},
	{"mentoringval",	"int","0"},
	{"mentoringtick",	"int","0"},
	{"makeskill",	"BLOB"},
	{"showmarrytitle",	"smallint",	"0"},
	{"onlineawardsec",	"int unsigned",	"0"},
	{"ResetTime",		"datetime"},
	{"maxrankpos",		"int","99999"},
	{"logindays",		"int unsigned", "0"},
	{"mulipay",		"smallint",		"0"},
	{"homemaxcontribution",	"int unsigned","0"},
	{"everforbid",	"int unsigned"},	//永久禁言 1
	{"loginawarded",		"smallint","0"},
	{"lifestarlv",			"int unsigned",		"0"},
};

static DB_Colume	Home_Colume[] =
{
	//{"homeid",		"int unsigned"},

	{"createuin",		"int unsigned"},
	{"fname",			"varchar(20)"},
	{"lvl",				"smallint"},
	{"exp",			"int unsigned"},
	{"build",			"BLOB"},
	{"fdesc",			"varchar(100)"},
	{"country",			"smallint"},
	{"createname",			"varchar(20)"},

};

static DB_Colume RpgNASale_Colume[] =
{
	//{"naindex",        "int unsigned"},
	{"uinfrom",				"int unsigned"},		//玩家
	{"type",				"int unsigned"},		//类型  0 寄售, 1收购
	{"startsec",			"int unsigned"},		
	{"price",				"int unsigned"},		
	
	{"itemid",				"int unsigned"},
	{"itemcnt",				"int unsigned"},
	{"iteminfo",			"int unsigned"},

};

static DB_Colume RpgMail_Colume[] =
{
	//{"mailid",        "int unsigned"},
	{"send",				"int unsigned"},		//编号,针对玩家
	{"recv",				"int unsigned"},		//编号,针对玩家
	{"state",				"smallint"},		//编号,针对玩家
	{"timetick",				"int unsigned"},		//编号,针对玩家

	//{"item",				"int unsigned"},		//编号,针对玩家
	//{"itemcnt",				"int unsigned"},		//编号,针对玩家

	{"money",				"int"},		//编号,针对玩家
	{"title",			"varchar(32)"},
	{"body",			"varchar(255)"},
	{"index1",				"int unsigned"},
	{"index2",				"int unsigned"},
	{"index3",				"int unsigned"},
	{"index4",				"int unsigned"},
};

static DB_Colume RpgSysMail_Colume[] =
{
	//{"mailid",        "int unsigned"},
	{"timetick",				"int unsigned"},		//


	{"money",				"int"},		//编号,针对玩家
	{"title",			"varchar(32)"},
	{"body",			"varchar(255)"},
	{"itemid",				"int unsigned"},
	{"itemcount",				"int unsigned"},
	{"bind",				"smallint"}

};

static DB_Colume RpgFriend_Colume[] =
{
	//{"uinA",				"int unsigned"},		//玩家
	{"uinB",				"int unsigned"},		//编号,针对玩家

	{"rstype",				"smallint"},		//模板ID
	{"rsvalA",				"int"},		//
	{"rsvalB",				"int"},		//

};

static DB_Colume RpgInvite_Colume[] =
{
	//{"uinsvr",				"int unsigned"},		//玩家/服务器编号
	{"invitecode",				"varchar(12)"},		//模板ID
	{"reachnum",				"smallint"},	//达到条件的个数
	{"checknum",				"smallint"},	//验收过的个数
};

static DB_Colume RpgWorldTest_Colume[] =
{
	//{"uinA",				"int unsigned"},		//玩家

	{"star",				"smallint"},		//星数
	{"count",				"int"},				//答题次数
	{"typeid",				"int"},				//题号
	{"choose1",				"smallint"},		//选项
	{"choose2",				"smallint"},
	{"choose3",				"smallint"},
	{"choose4",				"smallint"},
	{"correct",				"int"},				//正确次数
	{"name",				"varchar(20)"},		//玩家名字
	{"awardflag",			"smallint"},		//奖励是否领取

};

static DB_Colume RpgQuest_Colume[] =
{
	//{"uin",				"int unsigned"},		//玩家
	{"qid",					"int unsigned"},		//任务ID

	{"progress",			"BLOB"},				//任务进度信息

};

static DB_Colume RpgSkill_Colume[] =
{
	//{"uin",				"int unsigned"},		//玩家
	{"skill",					"int unsigned"},		//技能ID

	{"slv",					"smallint"},		//技能等级
	{"spos",					"smallint"},		//技能等级
	{"cdsec",					"int unsigned"},		//技能等级
};

static DB_Colume RpgState_Colume[] =
{
	//{"uin",				"int unsigned"},		//玩家
	{"state",					"int unsigned"},		//技能ID

	{"slv",					"smallint"},		//技能等级
	{"endsec",					"int unsigned"},		//技能等级
};

static DB_Colume RpgSlave_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"baseid",			"int unsigned"},		//模板ID

};

static DB_Colume RpgWorldFlag_Colume[] =
{
	//{"Uin",				"int unsigned"},
	{"flagdata",			"varchar(60)"},	

};

static DB_Colume RpgNewSlave_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"baseid",			"int unsigned"},		//模板ID
	{"kind",			"smallint"},

	{"name",			"varchar(20)"},
	{"slot",			"smallint"},			//所处位置,  包裹,银行,邮件
	{"level",			"smallint"},
	{"exp",				"int unsigned"},
	{"hp",				"int"},
	{"mp",				"int"},
	{"data0",			"longBLOB"},
	{"data1",			"longBLOB"},
	{"variatlv",		"int"	,"0"},
	{"data2",			"longBLOB"},
	{"lifepoint",		"int",	"250"},			//宠物寿命
	{"eatgainlimit",	"int",	"0"},
};
static DB_Colume RpgItemManage_Colume[] =
{
	//{"id",				"int unsigned"},		//索引
	{"type",					"smallint"},		//0 物品
	{"data",					"BLOB"},
};
static DB_Colume RpgItem_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"baseid",			"int unsigned"},		//模板ID
	{"num",				"int unsigned"},		//堆叠数量

	{"slot",			"smallint"},			//所处位置,  包裹,银行,邮件
	{"pos",				"int"},					//物品格子
	{"limittime",				"int unsigned"},					//时间
	{"bind",				"int"},					//绑定
	//{"data",		"BLOB"},
};

//装备额外属性
static DB_Colume RpgEquipEx_Colume[] =
{
	//{"id",				"int unsigned"},		//所属装备
	{"Uin",				"int unsigned"},
	{"itemid",				"int"},
	{"lvl",				"int"},
	{"addtype1",				"int"},
	{"addrandom1",				"int"},
	{"addtype2",				"int"},
	{"addrandom2",				"int"},
	{"objkey",				"bigint"},				//终身唯一
	{"stonepos",				"int"},
	{"stone12",				"int"},
	{"stone34",				"int"},
	{"eatlv",				"int"},

};


static DB_Colume RpgFort_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ManorData",				"BLOB"},
	{"WarRankData",				"BLOB"},
	{"ManorRecord",				"BLOB"},
};


static DB_Colume RpgRankData_Colume[] =
{
	//	{"id1",				"int unsigned"},
	{"id2",				"int unsigned"},
	{"type",			"int"},
	{"pos",				"int"},
	{"check1",			"int unsigned"},
	{"check2",			"int unsigned"},
	{"val1",			"int unsigned"},
	{"val2",			"int unsigned"},
	{"val3",			"int unsigned"},
	{"val4",			"int unsigned"},
	{"desc1",			"varchar(20)"},
	{"desc2",			"varchar(20)"},
	{"desc3",			"varchar(20)"},
	{"desc4",			"varchar(20)"},
	{"score",			"int","0"},
};

static DB_Colume RpgRankUser_Colume[] =
{
	//	{"uin",				"int unsigned"},
	{"level",				"int"},
	{"name",			"varchar(20)"},
	{"skin1",				"int"},
	{"skin2",				"int"},
	{"equip1",				"BLOB"},
	{"equip2",				"BLOB"},
	{"equip3",				"BLOB"},
	{"equip4",				"BLOB"},
	{"equip5",				"BLOB"},
	{"equip6",				"BLOB"},
	{"equip7",				"BLOB"},
	{"equip8",				"BLOB"},
	{"equip9",				"BLOB"},
	{"equip10",				"BLOB"},
	{"equip11",				"BLOB"},
	{"equip12",				"BLOB"},
	{"skin3",				"int"},
	{"job",					"int"},
	{"score",				"int"},
};

static DB_Colume RpgRankUWatch_Colume[] =
{
	//	{"uin",				"int unsigned"},
	{"id",				"int"},
	{"type",			"smallint"},
	{"data",				"BLOB"},
};

static DB_Colume MentoringTable_Colume[] =
{
	//	{"id",				"int unsigned"},
	{"tuin",				"int unsigned"},
	{"puin",				"int unsigned"},
	{"tflag",				"smallint"},
	{"pflag",				"smallint"},
	{"state",				"smallint"},
};

static DB_Colume MentoringRequest_Colume[] =
{
	//	{"uin",				"int unsigned"},
	{"value",				"int unsigned"},
	{"type",				"smallint"},
	{"endtick",				"int unsigned"},
};

static DB_Colume Achievement_Colume[] =
{
	//	{"uin",				"int unsigned"},
	{"id",				"int"},
	{"cnt",				"int"},
	{"ifdone",				"smallint"},
	{"ifshow",				"smallint"},
};

static DB_Colume MarryTable_Colume[] =
{
	//	{"id",				"int unsigned"},
	{"muin",				"int unsigned"},
	{"wuin",				"int unsigned"},
	{"mvalue",				"int unsigned"},	//男方友好度
	{"wvalue",				"int unsigned"},
	{"mpoint",				"int unsigned"},	//男方亲密度
	{"wpoint",				"int unsigned"},
	{"mskill",				"BLOB"},		//男方技能
	{"wskill",				"BLOB"},
	{"state",				"smallint"},
	{"marrytime",			"datetime"},
	{"awardlv",				"smallint"},
};

static DB_Colume MarryRequest_Colume[] =
{
	//	{"uin",				"int unsigned"},
	{"value",				"int unsigned"},
	{"type",				"smallint"},
	{"endtick",				"int unsigned"},
};

// static DB_Colume RpgEquip_Colume[] =
// {
// 	//{"Uin",				"int unsigned"},		//玩家
// 	{"id",				"int unsigned"},		//编号,针对玩家
// 	{"objkey",				"bigint"},				//终身唯一
// 
// 	{"baseid",			"int unsigned"},		//模板ID
// 
// 	{"slot",			"smallint"},			//所处位置,  包裹,银行,装备中,邮件
// 	{"lv",			"int unsigned"},
// 	//{"star",		"BLOB"},
// };

static DB_Colume DBVersionTable_Colume[] =
{
	{"version",			"int"},
};



////////////////////////////////////////日志/////////////////////////////////////////////////////////

static DB_Colume RpgGoldLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},
	{"money",		"int unsigned"},
};

static DB_Colume RpgExpLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},
	{"exp",		"int unsigned"},
	{"lvl",		"smallint"},
};

static DB_Colume RpgRMBLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"accuin",		"int unsigned"},
	{"ctime",		"datetime"},			//时间
	{"leftrmb",		"int unsigned"},		//余额
	{"addval",			"int"},				//使用或加入多少
	{"type",			"smallint"},		//使用原因
	{"logdesc",			"varchar(64)"},
};

static DB_Colume RpgBindRMBLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},			//时间
	{"leftrmb",		"int unsigned"},		//余额
	{"addval",			"int"},				//使用或加入多少
	{"type",			"smallint"},		//使用原因
	{"logdesc",			"varchar(30)"},
};

static DB_Colume RpgSlaveLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},			//时间
	{"slaveid",		"int unsigned"},		//宠物（坐骑）ID
	{"baseid",		"int unsigned"},		//模板ID
	{"lvl",			"smallint"},		//操作
	{"variatlv",			"smallint"},		//操作
	{"action",			"smallint"},		//操作
	{"logdesc",			"varchar(30)"},
};

static DB_Colume RpgGMCodeLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},			//时间
	{"name",		"varchar(20)"},
	{"code",			"varchar(100)"},
};

static DB_Colume RpgItemLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int"},			//时间
	{"itemcnt",		"int"},		//余额
	{"itemtype",			"int"},				//使用或加入多少
	{"ctime",		"datetime"},			//时间
};

static DB_Colume RpgItemChangeLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int"},			//
	{"itemcnt",		"int"},		//
	{"touin",		"int"},			//
	{"type",		"smallint"},
	{"ctime",		"datetime"},			//时间
};
static DB_Colume RpgPayErrorLog_Colume[] =
{
	//	{"tpid",				"int unsigned"},
	{"ctime",		"datetime"},			//时间
	{"logdesc",			"varchar(32)"},		//
};
static DB_Colume RpgLuaLog_Colume[] =
{
	//	{"id",				"int unsigned"},
	{"logdesc",			"varchar(1024)"},		//
	{"bsuccess",		"int unsigned"},
	{"ctime",		"datetime"},			//时间
};
static DB_Colume MkQICntLog_Colume[] =
{
	//{"objid",			"int"},			//时间
	{"objtype",		"int"},		//接受次数
	{"objcnt",		"int"},		//接受次数
};

static DB_Colume RpgLvupLog_Colume[] =
{
	//{"uin",			"int"},
	{"lvl",		"int"},		//等级
	{"ctime",		"datetime"},	//时间
};

//排行榜日志，结构与排行榜一样
static DB_Colume RpgRankLog_Colume[] =
{
	//	{"id1",				"int unsigned"},
	{"id2",				"int unsigned"},
	{"type",			"int"},
	{"pos",				"int"},
	{"check1",			"int unsigned"},
	{"check2",			"int unsigned"},
	{"val1",			"int unsigned"},
	{"val2",			"int unsigned"},
	{"val3",			"int unsigned"},
	{"val4",			"int unsigned"},
	{"desc1",			"varchar(20)"},
	{"desc2",			"varchar(20)"},
	{"desc3",			"varchar(20)"},
	{"desc4",			"varchar(20)"},
	{"score",			"int","0"},
};

static DB_Colume RpgUserInfoLog_Colume[] =
{
	//{"Uin",				"int unsigned"},
	{"AccountUin",		"int unsigned"},
	{"lvl",			"int unsigned"},
	{"exp",			"int unsigned"},
	{"job",			"int unsigned"},
	{"country",			"int unsigned"},
	{"sex",			"int unsigned"},
	{"gold",			"int unsigned"},
	{"Nickname",		"varchar(20)"},
	{"homeid",	"int unsigned"},
	{"homeduty",	"int unsigned"},
	{"viplvl",			"int unsigned"},
	{"honour",			"int unsigned","0"},
	{"starlv",			"int unsigned",		"0"},
	{"onlineawardsec",	"int unsigned",	"0"},
};

static DB_Colume RpgMailLog_Colume[] =
{
	//{"send",		"int unsigned"},		//
	{"recv",		"int unsigned"},	
	{"title",		"varchar(32)"},
	{"body",			"varchar(255)"},
	{"money",		"int unsigned"},	
	{"item1",		"int unsigned"},	
	{"cnt1",		"int unsigned"},	
	{"item2",		"int unsigned"},	
	{"cnt2",		"int unsigned"},	
	{"ctime",		"datetime"},	//时间

};

static DB_Colume RpgGoldFromLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"addval",		"int unsigned"},	
	{"money",		"int unsigned"},
	{"type",		"smallint"},
	{"ctime",		"datetime"},	//时间

};

static DB_Colume RpgEquipStarLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int unsigned"},
	{"starcnt",		"int unsigned"},	
	{"ctime",		"datetime"},	//时间
};

static DB_Colume RpgEquipStarExLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int unsigned"},
	{"changecnt",	"int"},	
	{"starlv",		"int unsigned"},	
	{"ctime",		"datetime"},	//时间
};

static DB_Colume	MkPay_Colume[] =
{
	//{"accuin",			"int unsigned"},
	{"totalpay",			"int unsigned"},
	{"paytime",				"datetime"},
};
static DB_Colume	MkLoginOut_Colume[] =
{
	//{"uin",			"int unsigned"},
	{"lv",				"int unsigned"},
	{"type",			"smallint"},		//0进入 1 离开游戏
	{"logtime",			"datetime"},
};

static DB_Colume RpgSlaveLvLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"slaveid",		"int unsigned"},
	{"baseid",		"int unsigned"},	
	{"npos",		"smallint"},	
	{"poslvl",		"int unsigned"},	
	{"ctime",		"datetime"},	//时间
};

static DB_Colume RpgFortLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"chgscore",	"int"},
	{"warscore",	"int"},
	{"curpos",		"int unsigned"},	
	{"manorlv",		"smallint"},	
	{"manorexp",	"int unsigned"},	
	{"forttype",	"smallint"},		//1竞技场 2庄园
	{"ctime",		"datetime"},	//时间
};

static DB_Colume RpgWarRankLog_Colume[] =
{
	{"uin",				"int unsigned"},
	{"lv",				"int unsigned"},
	{"name",	"varchar(16)"},
	{"job",				"int unsigned"},
	{"sex",				"int unsigned"},
	{"score",				"int unsigned"},
	{"wincnt",				"int unsigned"},
	{"param",	"BLOB"},
};

