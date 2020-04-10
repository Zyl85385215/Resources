#ifndef _DB_DEFINE_H_
#define _DB_DEFINE_H_

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 数据库版本                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define DB_VERSION		2013081501
#define DB_LOG_VERSION	2012061301
//	注意：当数据库的表结构修改时，一定要注意修改这个版本号，否则不会更新数据库表结构
//	具体格式为：	时间(年月日+当天版本号）

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 游戏数据库                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DB_VERSION_TABLE			"DBVersionTable"								//数据库表版本号


#define OP_CARD_TABLE				"OpCardData"				//船的数据
#define OP_MAGIC_TABLE				"OpMagicData"				//船长数据
#define OP_ITEM_TABLE				"OpItemData"				//物品数据
#define OP_USER_TABLE				"OpUserData"				//账号的主数据
#define OP_EQUIP_TABLE				"OpEquipData"				//装备数据
#define	OP_FRIEND_TABLE				"OpFriendData"
#define	OP_MAIL_TABLE				"OpMailData"
#define MW_HOME_TABLE				"FamilyData"
#define OP_WORLD_FLAG_TABLE			"OpWorldFlagData"

#define PLAYER_TABLE_NAME			"CharData"										//人物角色数据
#define SKILL_TABLE_NAME			"CharSkill"                                     //人物技能
#define STATELIST_NAME				"StateList"                                     //状态
#define SLAVE_TABLE_NAME			"charSlave"                                     //玩家随从
#define QUEST_LIST					"Questlist"                                     //任务列表
#define QUEST_VAR					"QuestVar"                                      //任务变量
#define DROIT_TABLE_NAME			"DroitTable"									//权限
#define CHAR_TITLE					"CharTitle"										//头衔

#define ITEM_TABLE_NAME				"CharItem"                                      //人物物品
#define CHAR_BOMB_DATA				"CharBomb"
#define CHAR_PURPOSE				"PurposeSys"

#define HOME_TABLE                  "Home"                                          //帮会
#define HOME_ITEM_TABLE             "HomeItem"                                      //帮会仓库
#define HOME_MEMBER_TABLE           "HomeMember"                                    //帮会成员
#define HOME_SKILL_TABLE			"HomeSkill"										//帮会技能
#define HOME_RELATION_TABLE			"HomeRelation"									//帮会关系
#define HOME_LEAVE_MEMBER_TABLE     "HomeLeaveMember"                               //帮会成员离开帮会列表
#define HOME_DEATAL_RECORD          "HomeDeatalRecord"                              //帮会银两支出明细
#define HOME_HALL_TABLE             "HomeHall"                                      //帮会堂口

#define CHAR_RELATION_NAME			"CharRelationTable"                             //好友
#define ITEM_SELLBUY_TABLE			"ItemSellBuyTable"								//物品寄售
#define MAIL_STORAGE				"MailStorage"									//邮件保存
#define MAIL_ITEMS					"MailItems"										//邮件物品保存
#define AUCTION_LIST_TABLE			"AuctionList"									//拍卖
#define AUCTION_CURRENT_TABLE		"AuctionCurrent"								//当前拍卖
#define SCRIPT_VAL_TABLE_NAME		"ScriptVal"                                     //玩家标记位配置
#define ITEM_WING_DATA				"WingData"                                     //玩家标记位配置

#define CENTERSERVERCYCLETIME_TABLE	"CenterServerCycleTimeTable"					//循环时间事件
#define GLOBAL_VALUE_TABLE			"GlobalValueTable_Ex"							//全局变量表
#define MAPOWNER_TALBLE_NAME		"MapOwnerTable"                                 //地图归属
#define PRE_PAID_CARD_TABLE			"PrePaidCard"									//点卡
#define IP_AWARD_TABLE				"IPAward"										//IP奖励
#define COUNTRY_TABLE				"Country"	
#define NEWAUCTION_TABLE			"newauction"									//寄售
#define ACTIVITY_RANK				"activityrank"
#define MALLITEM_BUYCNT_TABLE		"MallItemBuyCnt"								//购买商城物品的次数统计（仅限于带有次数限制的商城物品）
#define COUNTRY_KING_TABLE			"CountryKing"									
#define RANK_EQUIP_TABLE			"rankequip"
#define OATH_TABLE					"Oath"											//结义
#define MAPCOPY_STAGE_TABLE			"MapCopyStageTable"								//副本表

#define RANK_SLAVE_TABLE			"rankslave"

#define MATERIAL_COLLECTED_STATUS_TABLE	"CollectStatus"								//采集物品记数
#define MAP_COPY_SPORT_LOG_TABLE	"mapCopySportLog"

#define ITEM_MARKET_TABLE			"ItemMarket"									//物品交易中心
#define BOOK_SKILL_COLLECT_TABLE	"bookSkill"										//秘籍收集
#define AWARD_TABLE					"AwardItem"										//人物奖励领取
#define PLAYER_COPY_TABLE			"PlayerCopy"									//怪物模仿人物数据表
#define SLAVE_COPY_TABLE			"SlaveCopy"										//怪物模仿宠物数据表
#define OFFLINE_STALLAGE_ITEM       "OfflineStallageItem"                           //离线摆摊物品
#define OFFLINE_STALLAGE            "OfflineStallage"                               //离线摆摊的摊位
#define OFFLINE_STALLAGE_BUYITEM    "OfflineStallageBuyItem"                        //收购物品
#define OFFLINE_STALLAGE_BOUGHTITEM "OfflineStallageBoughtItem"                     //收购物品结果
#define OFFLINE_STALLAGE_MESSAGE    "OfflineStallageMessage"                        //留言表
#define WAR_MACHINE_TABLE			"WarMachine"
#define MAIL_NOTICE_TABLE					"mailnotice"					//上线检测的

#define IOS_FARM_TABLE				"iosfarm"
#define IOS_HORSESOUL_TABLE			"iosHorseSoul"									//马魂 

#define NEW_BOOK_COLLECT_TABLE      "NewBookCollect"                                //残章
#define NEW_BOOK_PIECE_TABLE		"NewBookPiece"                          //残章片
#define PLAYER_ACHIEVE_TABLE		"PlayerAchieve"								//成就

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 日志数据库                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	OP_ITEM_LOG					"OpItemLog"						//物品操作日志
#define OP_ACTION_LOG				"OpActionLog"					//玩家的行为日志


#define NUM_ONLINE_LOG				"OnLineLog"                                     //记录各线人数及全服总人数
#define ACTION_TABLE_NAME	    	"NewActionData"                                 //玩家记录
#define ITEMLOG_TABLE_NAME			"Newitemlog"                                    //物品记录 
#define EXP_EXCEPTION_TB			"NewExpException"								//经验异常检查
#define GOLD_EXCEPTION_TB			"NewGoldException"								//游戏币异常检查
#define FLOWER_EXCEPTION_TB         "NewFlowerException"
#define SLAVE_LOG_NAME				"NewSlaveLog"									//随从操作记录
#define GM_COMMAND_LOG				"GMCommandLog"									//GM操作日志
#define PRE_PAID_CARD_LOG_TABLE		"PrePaidCardLog"								//赠送充值卡操作日志
#define HOME_LOG					"homelog"										//帮会日志
#define HONOUR_LOG					"HonourLog"										//功勋日志
#define AUCTION_LOG					"AuctionLog"									//拍卖日志
#define LUAEVENT_LOG				"LUAEventLog"									//LUA事件日志
#define NEWAUCTION_LOG				"newauctionLog"
#define GETWEBRESULTLOG				"GetWebResultLog"
#define MIALLOG						"MailLog"
#define COUNTRYSYSTEM_LOG			"CountrySystemLog"								//国家系统的部分日志
#define WEALTHCHECK_LOG				"wealthchecklog"
#define FLOWERCHECK_LOG             "flowerchecklog"
#define SKILLEXP_EXCEPTION_TB		"SkillExpException"
#define ITEM_MARKET_BILL_LOG		"itemmarketbilllog"
#define NA_GOLD_TABLE_LOG           "NaGoldlog"                                     //拍卖元宝物品元宝获取和退还日志
#define HONOUR_CHANGE_LOG			"HonourChangeLog"								//名望变化日志
#define FISH_GOLD_LOG				"FishGoldLog"								//捕鱼积分

struct DB_Colume 
{
	char *	szName;
	char *	szType;
	char *	szDefault;
};

static DB_Colume OpUser_Colume[] =
{
	//{"Uin",				"int unsigned"},
	{"AccountUin",		"int unsigned"},
	//{"playerFlag",		"longBLOB"},
	{"LoginTime",		"datetime"},
	{"LogoutTime",		"datetime"},
	{"Nickname",		"varchar(20)"},
	{"nextitemid",		"int unsigned"},
	
	{"money",			"int unsigned"},
	{"power",			"int unsigned"},
	{"lvl",			"int unsigned"},
	{"exp",			"int unsigned"},
	{"isnew",		"int unsigned"},
	
	{"homeid",	"int unsigned"},
	{"homeduty",	"int unsigned"},
	{"flag",	"BLOB"},
};

static DB_Colume	Home_Colume[] =
{
	//{"homeid",		"int unsigned"},
	
	{"createuin",		"int unsigned"},
	{"fname",			"varchar(20)"},
	{"lvl",				"smallint"},
	
};

static DB_Colume	OpWorldFlag_Colume[] =
{
	
	//{"id",				"int unsigned"},
	{"flag",	"BLOB"},
};
static DB_Colume OpCard_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"type",			"int unsigned"},
	{"lv",				"smallint"},
	{"exp",				"int unsigned"},

	{"eqweapon",		"int unsigned"},
	{"eqcloth",		"int unsigned"},
	{"eqride",			"int unsigned"},
	{"eqmagic",		"int unsigned"},
	{"eqmagic1",		"int unsigned"},
	{"eqmagic2",		"int unsigned"},
	{"pos",		"int unsigned"},
	{"site",		"int unsigned"},
	{"potential",		"int unsigned"},
	{"pminatk",		"int unsigned"},
	{"pmaxatk",		"int unsigned"},
	{"pmindef",		"int unsigned"},
	{"pmaxdef",		"int unsigned"},
	{"pmaxhp",		"int unsigned"},
	{"pmaxmp",		"int unsigned"},
};

static DB_Colume OpMail_Colume[] =
{
	//{"mailid",        "int unsigned"},
	{"send",				"int unsigned"},		//编号,针对玩家
	{"recv",				"int unsigned"},		//编号,针对玩家
	{"state",				"smallint"},		//编号,针对玩家
	{"timetick",				"int unsigned"},		//编号,针对玩家

	{"item",				"int unsigned"},		//编号,针对玩家
	{"itemcnt",				"int unsigned"},		//编号,针对玩家

	{"money",				"int unsigned"},		//编号,针对玩家
	{"title",			"varchar(32)"},
	{"body",			"varchar(255)"},
};

static DB_Colume OpFriend_Colume[] =
{
	//{"uinA",				"int unsigned"},		//玩家
	{"uinB",				"int unsigned"},		//编号,针对玩家

	{"rstype",				"smallint"},		//模板ID
	{"rsval",				"int"},		//堆叠数量


};

static DB_Colume OpItem_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"baseid",			"int unsigned"},		//模板ID
	{"num",				"int unsigned"},		//堆叠数量

	//{"slot",			"smallint"},			//所处位置,  包裹,银行,邮件

	//{"star",		"BLOB"},
};

static DB_Colume OpEquip_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"baseid",			"int unsigned"},		//模板ID

	{"slot",			"smallint"},			//所处位置,  包裹,银行,装备中,邮件
	{"lv",			"int unsigned"},
	//{"star",		"BLOB"},
};


static DB_Colume OpMagic_Colume[] =
{
	//{"Uin",				"int unsigned"},		//玩家
	{"id",				"int unsigned"},		//编号,针对玩家
	{"objkey",				"bigint"},				//终身唯一

	{"baseid",			"int unsigned"},		//模板ID
	{"lv",				"smallint"},
	{"exp",				"int unsigned"},

	{"skill1",			"int unsigned"},			//
	{"skill2",			"int unsigned"},			//
	{"slot",		"smallint"},
};


static DB_Colume CharData_Colume[] =
{
	{"AccountUin",      "int unsigned"},
	{"Name",			"varchar(20)"},
	{"sex",				"smallint"},
	{"Position_Map",	"int"},
	{"Position_X",		"int"},
	{"Position_Y",		"int"},
	{"sx",				"int"},
	{"sy",				"int"},
	{"smap",			"int"},
	{"con",				"int"},
	{"dex",				"int"},
	{"Ing",				"int"},
	{"Str",				"int"},
	{"Wis",				"int"},
	{"AttribPoint",		"int"},
	{"Exp",				"int"},
	{"Level",			"int"},
	{"SKIN",			"int"},
	{"WeaponID",		"int"},
	{"pkCount",			"int"},
	{"pkPrison",		"int"},
	{"Lucky",			"int"},
	{"HP",				"int"},
	{"MaxHp",			"int"},
	{"MP",				"int"},
	{"MaxMP",			"int"},
	{"Gold",			"int"},
	{"Balance",			"int"},
	{"playerFlag",		"longBLOB"},
	{"SKIN2",			"smallint"},
	{"FloorMap",		"int"},
	{"shortCut",		"longBLOB"},
	{"MarryFlag",		"int"},
	{"CouplerUin",		"int"},
	{"MarryTime",		"datetime"},
	{"LogoutTime",		"datetime"},
	{"CreateTime",		"datetime"},
	{"Nickname",		"varchar(48)"},
	{"ConfID",			"int"},
	{"HomeID",	        "int"},
	{"country",			"int"},
	{"StorPass",		"varchar(20)"},
	{"job",				"smallint"},
	{"jobExp",			"int"},
	{"WXjmsht",			"smallint"},
	{"CoolDowns",		"varchar(1000)"},
	{"OnLineTime",		"int"},
	{"OffLineTime",		"int"},
	{"zoneindex",		"int"},
	{"QLEVEL",			"varchar(300)"},
	{"NextItemKey",		"int unsigned"},
	{"EnforceHp",		"int"},
	{"EnforceMp",		"int"},
	{"EnforceAtkw",		"int"},
	{"EnforceAtkn",		"int"},
	{"EnforceApn",		"int"},
	{"EnforceDefw",		"int"},
	{"EnforceDefn",		"int"},
	{"EnforcePpn",		"int"},
	{"EnforceDpw",		"int"},
	{"HonourVal",		"int"},
	{"HonourLvl",		"smallint"},
	{"isDeleted",		"smallint"},
	{"MasterPoint",		"int unsigned"},
	
	{"MasterUin",		"int unsigned"},
	{"isChuShi",		"smallint"},
	{"NextSlaveKey",	"int unsigned"},
	{"nextextendid",    "int unsigned"},
	{"ActiveTitle",		"int"},
	{"SkillPoint",		"int"},
	{"completeQuest",	"MediumBLOB"},
	{"serverindex",		"smallint", "-1"},
	{"FloorPos_X",		"int"},
	{"FloorPos_Y",		"int"},
	{"FloorZone",		"int"},
	{"vigor",			"int"},
	{"AntiGuajiState",	"int"},
	{"AntiCountTime",	"int"},
	{"BagAutoLock",		"int"},
	{"StorageAutoLock",	"int", "1"},
	{"SlaveAutoLock",	"int"},
	{"PointAutoLock",	"int"},
	{"ErrorPassTimes",  "int"},
	{"ForbidPassTime",	"int"},
	{"ShiTuLvlReward",	"int"},
	{"GraduationCnt",	"int"},
	
	{"TransferInfo",	"BLOB"},
	{"TodayOfflineTime2Store",	"int"},	// 今日还有多少离线时间可以存储，用于离线经验系统
	{"OfflineTime2Use",			"int"},	// 离线使用时间，用于离线经验系统
	{"OathID",					"int"},
	{"OathTitle",				"varchar(12)"},
	{"UseOathTitle",			"int"},
	{"HonourControlTime",		"datetime"},
	{"HonourControlValue",		"int"},
	{"HonourDay",		"int"},
	{"HonourWeek",		"int"},
	{"DefaultSkinType",	"int"},
	{"SkillExp",		"int"},
	{"playerskin",		"BLOB",	""},
	{"Flower",          "int"},
	{"MasterTeachCnt",	"smallint"},
	{"BaiShiCD",		"int"},
	{"honortitle",		"BLOB", ""},
	{"GoldBang",		"int"},
	{"TotalOnlineCount","int"},	//该角色总共上线游戏时间
	{"ItemMarketBill",	"int"},
	{"NFriends","BLOB",""},
	{"Invitedfriends","BLOB",""},
	{"bookskillbase","BLOB",""},	
	{"PlayerKillCount",	"BLOB", ""},
	{"IsEvilMode",		"smallint"},
	{"IsHeimuya",		"smallint"},
	{"IsPveMode",		"smallint"},
	{"transportlist",	"BLOB"}, //引路蜂记录列表
	{"total_integer", "int unsigned"},  // 总积分	
	{"total_time", "int unsigned"}, // 总的时间
	{"curweek_integer", "int unsigned"}, // 当前周的累计积分（为了算奖励积分）
	{"today_integer", "int unsigned"}, //  当天累计的积分（为了算限制后的积分）
	{"addhomemptime", "smallint"},     //  当天在线给帮会加的士气次数
	{"deleteTime","int unsigned"},	   // 人物删除时间
	{"ArenaScore", "int"},
	{"ArenaWinBattle", "int"},
	{"ArenaTotalBattle", "int"},
	{"blockflag", "smallint"},
	{"newbetime", "int"},
	{"MsgFlag",	"int"},
	{"speciallv","smallint", "0"},
	{"NaGold", "int"},
	{"NaBindGold", "int"},
	{"ActExPoint", "int", "0"},			//活动积分
	{"DinghunState", "int"},
	{"FishScore", "int"},
	{"homestates",	"BLOB", ""}, //引路蜂记录列表
	{"HomeHonourNum", "int", "0"},
	{"AllLuckInfo", "int", "0"},
	{"is_register", "TINYINT", "0"},
	{"master_desc", "varchar(50)", NULL},
	{"master_begin", "TINYINT", "0"},
	{"master_end", "TINYINT", "0"},
	{"TianxiaGroupId",	"int"},
	{"expball",	"smallint", "0"},
	{"playercopychallengecount","int","5"},
	{"playercopycooldownsec","int","0"},
	{"honourlimit","int","0"},
	{"slavecopychallengecount","int","10"},
	{"slavecopycooldownsec","int","0"},
	{"stfireskill","BLOB",""},
	{"WorldMsgflag","int","-1"},
	{"mailnotice","int","0"},
	{"MarryRingLevel","int","0"},
	{"MarryRingPoint","int","0"},
	{"dcfailval","int","0"},
	{"spdrvst","BLOB",""},
	{"PlayerSwordSprite",	"BLOB", ""}, //剑灵
	{"bsacttime","int","0"},
	{"EquipScore",		"int"},
	{"TudiPointUsed",	"int"},
	{"VipLevel",		"int"},
	{"VipAvailable",	"smallint"},
	{"VipRemainTime",	"int"},
	{"NextLearnSkillTime", "int"},
	{"achievements", "BLOB", ""},
	{"achievementsdone", "BLOB", ""},
	{"OnlinePrizeTime",		"int unsigned"},
	{"LoginDaysCount",	"int"},
	{"FlowerTotalPoint",      "int", "0"},
	{"FlowerDayPoint",        "int", "0"},
	{"FlowerWeekPoint",        "int", "0"},
	{"FlowerMonthPoint",        "int", "0"},
	{"CurTreasure",        "int", "0"},
	{"TreasureTimes",        "int", "0"},
	{"TreasureEnd",        "int", "0"},
	{"ResetFlagDay",		"int", "999"},
	{"Treasureptx",        "int", "0"},
	{"Treasurepty",        "int", "0"},
	{"playercopybuycount",	"int"},
	{"LastRefTime",	"int", "0"},
	{"RefRamainTime",	"int", "0"},
	{"FirstZhuoMa",	"int", "1"},
	{"HunterExp",	"int", "0"},
	{"HunterLV",	"int", "1"},
	
};

static DB_Colume SkillData_Colume[] = 
{
	{"SkillID",			"int"},
	{"SkillExp",		"int"},
	{"bHide",			"int"},
	{"SkillTalent",     "int"},
	{"bActive",			"int"},
};

static DB_Colume WarMachine_Colume[] = 
{
	//{"uin",			"int"},
	{"lv",				"int"},
	{"exp",				"int"},
	{"skilllearn",		"BLOB"},
	{"skilluse",		"BLOB"},
	{"sp",				"int"},
	{"dure",			"int"},
};

static DB_Colume IosHorseSoul_Colume[] =
{
	{"masteruin",	"int",	"0"},
	{"index",		"int",	"0"},
	{"type",		"int",	"0"},
	{"quality",		"int",	"0"},
	{"level",		"int",	"0"},
	{"exp",		"int",	"0"},
};

static DB_Colume PlayerAchieve_Colume[] =
{
	{"masteruin",	"int",	"0"},
	{"id",			"int",	"0"},
	{"done",	"smallint",	"0"},
	{"val1",		"int",	"0"},
	{"val2",		"int",	"0"},
	{"val3",		"int",	"0"},
};

static DB_Colume NewBookCollectTable_Colume[] = 
{
	//{"ID",              "int"},  //残章ID
	{"lvl",             "int"},  //残章等级
};

static DB_Colume NewBookPieceTable_Colume[] = 
{
	//{"ByIndex",             "int"},
	{"ID1",               "int"},
	{"ID2",               "int"},
	{"ID3",               "int"},
	{"ID4",               "int"},
	{"ID5",               "int"},
	{"ID6",               "int"},
};

static DB_Colume IosFarm_Colume[] = 
{
	//{"uin",			"int"},
	{"activetick",			"int"},
	{"farmval",				"int"},
	{"farmhp",				"int"},
	{"part1lv",				"smallint"},
	{"part1obj",			"int"},
	{"part1time",		"int"},
	{"part2lv",				"smallint"},
	{"part2obj",			"int"},
	{"part2time",		"int"},
	{"part3lv",				"smallint"},
	{"part3obj",			"int"},
	{"part3time",		"int"},
	{"part4lv",				"smallint"},
	{"part4obj",			"int"},
	{"part4time",		"int"},
	{"part5lv",				"smallint"},
	{"part5obj",			"int"},
	{"part5time",		"int"},
	{"part6lv",				"smallint"},
	{"part6obj",			"int"},
	{"part6time",		"int"},
	{"part7lv",				"smallint"},
	{"part7obj",			"int"},
	{"part7time",		"int"},
	{"part8lv",				"smallint"},
	{"part8obj",			"int"},
	{"part8time",		"int"},
	{"part9lv",				"smallint"},
	{"part9obj",			"int"},
	{"part9time",		"int"},
	{"part100lv",		"smallint"},		//养殖房
	{"part100time",		"int"},
	{"animal1",			"int"},
	{"animal1day",		"int"},
	{"animal2",			"int"},
	{"animal2day",		"int"},
	{"animal3",			"int"},
	{"animal3day",		"int"},
	{"animal4",			"int"},
	{"animal4day",		"int"},
	{"animal5",			"int"},
	{"animal5day",		"int"},
	{"animal6",			"int"},
	{"animal6day",		"int"},
	{"animal7",			"int"},
	{"animal7day",		"int"},
	{"animal8",			"int"},
	{"animal8day",		"int"},
	{"animal9",			"int"},
	{"animal9day",		"int"},
	{"animal10",		"int"},
	{"animal10day",		"int"},
	{"name",			"varchar(16)"},
	{"part1quality",		"smallint",	"0"},
	{"part2quality",		"smallint",	"0"},
	{"part3quality",		"smallint",	"0"},
	{"part4quality",		"smallint",	"0"},
	{"part5quality",		"smallint",	"0"},
	{"part6quality",		"smallint",	"0"},
	{"part7quality",		"smallint",	"0"},
	{"part8quality",		"smallint",	"0"},
	{"part9quality",		"smallint",	"0"},
	{"animal1quality",		"smallint",	"0"},
	{"animal2quality",		"smallint",	"0"},
	{"animal3quality",		"smallint",	"0"},
	{"animal4quality",		"smallint",	"0"},
	{"animal5quality",		"smallint",	"0"},
	{"animal6quality",		"smallint",	"0"},
	{"animal7quality",		"smallint",	"0"},
	{"animal8quality",		"smallint",	"0"},
	{"animal9quality",		"smallint",	"0"},
	{"animal10quality",		"smallint",	"0"},
	{"FirstOpen",		     "int",	"1"},
	{"DaoQieNum",		     "int",	"0"},
	{"DaoQieUin",		     "BLOB",""},
	{"DaoQieUin1",           "int",	"0"},
	{"DaoQieUin2",           "int",	"0"},
	{"DaoQieUin3",           "int",	"0"},
	{"DaoQieUin4",           "int",	"0"},
	{"DaoQieUin5",           "int",	"0"},
	{"DaoQieUin6",           "int",	"0"},
	{"DaoQieUin7",           "int",	"0"},
	{"DaoQieUin8",           "int",	"0"},
	{"DaoQieUin9",           "int",	"0"},
	{"DaoQieUin10",          "int",	"0"},
	{"DaoQieUin11",          "int",	"0"},
	{"DaoQieUin12",          "int",	"0"},
	{"DaoQieUin13",          "int",	"0"},
	{"DaoQieUin14",          "int",	"0"},
	{"DaoQieUin15",          "int",	"0"},
	{"DaoQieUin16",          "int",	"0"},
};


static DB_Colume PurposeSys_Colume[] = 
{
	//{"uin",			"int"},
	{"Quest1",				"int"},
	{"Quest2",				"int"},
	{"Quest3",				"int"},
	{"Quest4",				"int"},
	{"Quest5",				"int"},
	{"Flag",				"int"},
};

static DB_Colume NALog_Colume[] = 
{
	{"nakey",			"int"},
	{"itemID",		"int"},
	{"itemkey",			"bigint"},
	{"seller",			"int"},
	{"buyer",			"int"},
	{"price",			"int"},
	{"status",			"smallint"},
	{"opertion",		"smallint"},
	{"ex",			"varchar(255)"},
	{"time",		"datetime"},
};

static DB_Colume ItemData_Colume[] = 
{
	{"itemIndex",		"int"},
	{"Slot",			"smallint"},
	{"ItemID",			"int"},
	{"Type",			"smallint"},
	{"Dura",			"int"},
	{"DuraMax",			"smallint unsigned"},
	{"Data",			"smallint unsigned"},
	{"ItemKey",			"bigint"},
	{"DeadLine",		"int unsigned"},
	{"UnbindDeadLine",	"int unsigned"},
	{"Position",		"smallint unsigned"},
	{"MohuaParam",		"int"},
	{"level",			"smallint"},
	{"exp",				"int"},
	{"gems",			"TINYBLOB"},
	{"itembind",		"TINYBLOB"},
	{"itemextend",		"BLOB"},
	{"itemjudge",		"BLOB"},
	{"itemtrump",		"BLOB"},	//法宝2进制
	{"PlusLevel",		"smallint"},
	{"PlusScore",		"int"},
	{"plusparam",		"BLOB"},
	{"starluck",		"int"},
	{"growlevel",		"int"},
	{"growexp",			"int"},
	{"starprotect",		"BLOB"},
	{"itemcarve",		"BLOB"},
	{"holelevel",       "int", "0"},
	{"holeprotect",     "int", "0"},
	{"gemvalue",        "int", "0"},
	{"itemrepute",        "BLOB"},	//名望装备用
	{"HorseEq",				"BLOB"},//坐骑属性
	{"enchaseEx",			"TINYBLOB"},
};

static DB_Colume CharBomb_Colume[] = 
{
	{"quality",			"smallint"},
	{"quaexp",			"int"},
	{"level",			"smallint"},
	{"exp",				"int"},		//升级时间
	{"maxhp",			"int"},		//精通等级
	{"maxmp",			"int"},		//暗器数目
	{"atkw",			"int"},		//暗器技能开关
	{"atkn",			"int"},
	{"defw",			"int"},
	{"defn",			"int"},
	{"apn",				"int"},
	{"dpw",				"int"},
	{"dunt",			"int"},
	{"mdl",				"int"},

	{"maxhplmt",		"int"},
	{"maxmplmt",		"int"},
	{"atkwlmt",			"int"},
	{"atknlmt",			"int"},
	{"defwlmt",			"int"},
	{"defnlmt",			"int"},
	{"apnlmt",			"int"},
	{"dpwlmt",			"int"},
	{"duntlmt",			"int"},
	{"mdllmt",			"int"},
	{"bombindex",			"int"},
};

static DB_Colume WingData_Colume[] = 
{
	//{"uin",				"int"},
	{"itemIndex",			"int"},
	{"steplv",        "smallint"},
	{"steppow",			"int"},
	{"quality",        "smallint"},
	{"addtype",			"int"},
	{"atkwc",			"int"},
	//{"atkwm",			"int"},
	{"atknc",			"int"},
	//{"atknm",			"int"},
	{"addpowc",			"int"},
	//{"addpowm",			"int"},
	{"decpowc",			"int"},
	//{"decpowm",			"int"},
	{"repairtime",		"smallint"},
	{"spvalqua",		"smallint","0"},
};

static DB_Colume CharSlave_Colume[] = 
{
	{"Materuin",		"int unsigned"},
	{"SlaveUIN",		"int unsigned"},
	{"SlaveIndex",		"int"},
	{"Quality",			"int"},
	{"BInBatalle",		"smallint"},
	{"level",			"smallint"},
	{"Exp",				"int"},
	{"ExpLevel",		"int"},
	{"Hp",				"int"},
	{"Mp",				"int"},
	{"powlv",			"smallint"},
	{"powExp",			"int"},
	{"phaseLimit",		"smallint"},

	{"str",				"int"},	
	{"wis",				"int"},	
	{"ing",				"int"},	
	{"dex",				"int"},	
	{"con",				"int"},	
	{"points",			"int"},	

	{"name",			"char(20)"},
	{"Life",			"int"},	
	{"SkillA",			"int"},
	{"SkillB",			"int"},
	{"SkillC",			"int"},
	{"SkillD",			"int"},
	{"SkillE",			"int"},
	{"SkillF",			"int"},

	{"Slot",			"smallint"},
	{"SlaveKey",		"bigint"},
	{"IsBB",			"int"},	
	{"phase",			"smallint"},

	{"SkillG",			"int"},
	{"SkillH",			"int"},
	{"SkillI",			"int"},
	{"skillslot",       "int"},


	// 原始属性
	{"base_str",		"int"},
	{"base_wis",		"int"},
	{"base_ing",		"int"},
	{"base_dex",		"int"},
	{"base_con",		"int"},
	// 绑定信息
	{"bindstatus",		"int"},
	{"needunbindtime",	"int"},
	{"unbindtickcount",	"int"},
	{"bindlevel",		"int"},
	// 冷却
	{"CoolDowns",		"varchar(1000)"},

	{"rideflag",		"smallint", "0"},
	{"otherlimit",		"smallint", "0"},

	{"nmaxhp",		"int","0"},
	{"nmaxmp",		"int","0"},
	{"ndunt",		"int","0"},
	{"natkw",		"int","0"},
	{"natkn",		"int","0"},
	{"napn",		"int","0"},
	{"ndefw",		"int","0"},
	{"ndefn",		"int","0"},
	{"nmdl",		"int","0"},
	{"ndpw",		"int","0"},

	{"skilltrans",		"int","2000"},
	{"wildsoul",	"int","0"},
	{"lockphase",	"int","0"},
	{"fusefeedval",	"int","0"},
	{"pexsoul",	"int","0"},
	{"pexval",	"int","0"},
	{"pexlv",	"smallint","0"},
	{"spvalqua",		"smallint","0"},
	{"limittime",	 "bigint"},
	{"phaseupcount",	"int","0"},

	{"hunger",	"int","0"},
	{"happy",	"int","0"},
	{"clean",	"int","0"},
	{"sickbit",	"int","0"},
};

static DB_Colume MapOwnerTable_Colume[] = 
{
	{"owner",			"int"},
	{"mapforce",		"int"}
};

static DB_Colume HomeItemTb_Colume[] = 
{
	{"HomeID",       "int"},//帮会ID
	{"Uin",          "int"},//捐献人UIN 
	{"ItemID",       "int"},
	{"Num",          "int"},
	//{"itemkey",		 "bigint"},//该物品的KEY
	{"itemdata",	 "BLOB"},   //该物品的具体数据
	{"ItemIndex",        "int"},//该物品在仓库中的INDEX
};

static DB_Colume HomeTb_Colume[] = 
{
	{"HomeName",		"varchar(48)"},		// 帮会名字
	{"HeaderUin",		"int"},				// 帮会首脑的Uin
	{"HomeTenet",		"varchar(128)"},	// 帮会宗旨
	{"Level",           "int"},             // 帮会等级
	{"Money",           "int"},             // 帮会资金
	{"MenagePoint",     "int"},             // 帮会管理费
	{"Power",           "int"},             // 帮会繁荣度
	{"ResourceLevel",   "int"},             // 帮会资源等级
	{"EnForcelevel",	"int"},             // 帮会强化等级
	{"RebuilderUin",    "int"},             // 自荐成员的Uin
	{"RebuildTime",     "int"},             // 自荐开启时间
	{"Honour",			"int"},             // 帮会功勋
	{"RenameTimes",		"int"},				// 重命名次数
	{"HomeICON",		"smallint", "-1"},	// 帮会图标
	{"HomeProductsInfo","BLOB"},			// 帮会产业
	{"HomeLock",        "int", "1"},
	{"CreateTime",      "int"},
	{"CreateTime1",      "datetime"},
	{"HomeProffer",      "smallint", "0"},
	{"IsHeimuya",        "smallint", "0"},
	{"IsDownLevel",      "smallint", "0"},
	{"ContributeTimes",  "int"},
	{"BankIndex",        "int"},
	{"HomeProffer2",      "smallint", "0"},
	{"HomeBankMoney",    "int"},
	{"HomeTurnoverHour", "int"},
	{"HomeProffer3",      "smallint", "0"},
	{"HomeDeleteLeagueHour", "int",   "0"},
	{"HomeRebuildFlag",   "int", "0"},
	{"HomeRebuildTime",   "int", "0"},
	{"HomeStateUnLock",   "int", "0"},
	{"HomeHonour",        "int", "0"},
	{"HomeBankGold",      "int", "0"},
	{"HomeBankNoGiveGold", "int", "0"},
	{"ChangeLeaderTime",   "int", "0"},
};

static DB_Colume HomeMemberTb_Colume[] = 
{
	{"MemberUin",		"int"},				// 帮会成员的Uin
	{"MemberPosition",	"int"},				// 成员在帮会中的职位		
	{"MemberPower",     "int"},             // 成员的贡献度
	{"TotalPower",      "int"},             // 成员的累计贡献度 
	{"TotalHomeVal",    "int"},             // 成员的人气值
	{"MemberPoint",     "int"},
	{"TotalPoint",      "int"},
	{"TipTimes",		"int"},				//用于客户端弹提示。目前只需要弹两个。不用了，可以用作他处
	{"nDays",           "int"},
	{"nContributeTime", "int"},
	{"nHallID",         "int"},             // 堂口ID
	{"nHallDutyIndex",  "int"},             // 堂口职位
	{"nTotalGold",      "int"},
};

static DB_Colume HomeLeaveMemberTb_Colume[] = 
{
	{"MemberUin",		"int"},				// 帮会成员的Uin
	{"HomePower",       "int"},             // 该家伙建过帮会 建立的时候帮会士气和帮会资金保留
	{"HomeFund",        "int"},
	{"MemberPoint",     "int"},             // 该家伙的个人贡献度和总贡献度保留
	{"TotalPoint",      "int"},           
};

static DB_Colume HomeDeatalRecordTb_Colume[] = 
{
	{"LeaderUin",                "int"},
	{"GiveUin",                  "int"},
	{"Money",					 "int"},
	{"HomeID",                   "int"},
	{"SystemHours",              "int"},
	{"TurnOverTime",             "datetime"},
	{"Type",                     "int"},
};

static DB_Colume HomeHallTb_Colume[] = 
{
	{"HallID",                 "int"},
	{"HomeID",                 "int"},
	{"HallName",               "varchar(48)"},
};

static DB_Colume RelationTable_Colume[] = 
{
	{"RelationUin",		"int"},
	{"RelationType",	"smallint unsigned"},
	{"FriendshipLevel",	"smallint unsigned",    "1"},
	{"Friendship",		"int",  "0"},
	{"ExpTimes",		"smallint unsigned",	"0"},
};

static DB_Colume State_Column[] = 
{ 
	{"StateId",			"int"}, 
	{"Level",			"smallint"}, 
	{"Repeats",			"int"}, 
	{"Intervals",		"int"},
	{"timetype",		"smallint","0"},
};

static DB_Colume Questlist[] = 
{
	{"uin",				"int"},
	{"state",			"int"},				//任务完成标志
	{"timeout",			"bigint"},
};

static DB_Colume RankEquipDB[] =
{
	//{"uin",					"int"},
	{"ranktype",			"smallint"},
	{"point",				"int"},
	{"itemkey",			"bigint"},
	{"itemdata",		"BLOB"}
};

static DB_Colume RankSlaveDB[] =
{
	//{"uin",					"int"},
	{"point",				"int"},
	{"sindex",			"int"},
	{"slavename",           "varchar(20)"},
};

static DB_Colume BookSkillDB[] =
{
	//{"uin",				"int"},
	{"skillid",				"int"},
	{"bookdata",		"BLOB"}
};

static  DB_Colume AwardItem[] = 
{
	 //{"uin",				"int"},
	 {"type",				"int"},
	 {"record",				"int"}
};

static DB_Colume NewAuctionDB[] =
{
	//{"nakey",				"int"},
	{"selluin",				"int"},
	{"buyuin",				"int"},
	{"curprice",			"int"},
	{"maxprice",			"int"},
	{"itemkey",			"bigint"},
	{"dptype",			"smallint"},
	{"lvl",				"smallint"},
	{"goldtype",		"smallint"},
	{"flag",		"smallint"},
	{"timeend",				"int"},
	{"search",		"varchar(32)"},
	{"itemdata",		"BLOB"}
};

static DB_Colume ActivityRankDB[] =
{
//	{"ActivityID",			"int"},
	{"uin",					"int unsigned"},
	{"position",			"int"},
	{"time",				"datetime"},
};

static DB_Colume QuestVarlist[] =
{
	{"uin",				"int"},
	{"varname",			"varchar(50)"},
	{"vardes",			"varchar(500)"},
	{"varval",			"int"},
	{"varcompval",		"int"},
	{"varfailval",		"int"}
};

static DB_Colume ScriptVal_Colume[] = 
{
	{"Type",			"smallint"},
	{"Data",			"int"}
};

static DB_Colume PlayerDroit_Colume[] =
{
	{"Type",			"smallint"},
	{"Time",			"int"},
};

static DB_Colume Centertable_Colume[] = 
{
	{"AreaMaxUin",      "int unsigned"},
};

static DB_Colume CenterServerCycleTimeTable_Colume[] = 
{
	{"time",			"varchar(50)"},
};

static DB_Colume GlobalValueTable_Colume[] =
{
	{"Value",			"varchar(64)"},
	{"time",			"datetime"},
	{"type",			"int"},
};

static DB_Colume MailNoticeTable_Colume[] =
{
	{"ID",				"int"},			//顺序号
	{"IDEx",			"int"},		//帮会ID
	{"info",			"varchar(255)"},
	{"item",			"int"},
	{"time",			"datetime"},
};

static DB_Colume ItemSellBuyTable_Colume[] = 
{	
	{"itemIndex",		"int"},
	{"Slot",			"smallint unsigned"},
	{"UinBuy",			"int"},
	{"price",			"int"},
	{"beginTime",		"int"},
	{"state",			"smallint unsigned"},
	{"type",			"smallint unsigned"},
};

static DB_Colume DBVersionTable_Colume[] =
{
	{"version",			"int"}
};

static DB_Colume MailStorage_Colume[] =
{
	{"senduin",			"int unsigned"},
	{"mailid",			"int unsigned"},
	{"recvuin",			"int unsigned"},	
	{"title",			"varchar(50)"},
	{"body",			"varchar(500)"},	
	{"money",			"int unsigned"},
	{"bpayfor",			"smallint"},
	{"returned",		"smallint"},
	{"visited",			"smallint"},
	{"sendname",		"varchar(50)"},
	{"sendtime",		"int unsigned"},
	{"overduetime",		"int unsigned"},
};

static DB_Colume MailItems_Colume[] =
{
	{"uin",				"int unsigned"},
	{"recvuin",			"int unsigned"},
	{"mailid",			"int unsigned"},
	{"itemindex",		"int unsigned"},
};

static DB_Colume HomeSkill_Colume[] =
{	
	{"Level",			"smallint"},		
	{"LevelUpTime",		"int unsigned"},
};

static DB_Colume Auction_Colume[] =
{		
	{"itemindex",		"int unsigned"},
	{"type",			"int"},
	{"price",			"int"},
	{"seller",			"binary(20)"},
	{"AccountUin",		"int unsigned"},
	{"ServerLine",		"int"},
	{"BuyItNow",		"int"},
};

static DB_Colume CurrentAuction_Colume[] =
{			
	{"buyeruin",		"int unsigned"},
	{"buyername",		"varchar(20)"},
	{"cprice",			"int"},	
	{"ServerLine",		"int"},
};

static DB_Colume CharTitle_Colume[] =
{			
	{"OverTime",		"int unsigned", "0"},
};

static DB_Colume Home_Relation_Colume[] =
{
	{"id",			"int unsigned"},
	{"targetid",	"int unsigned"},
	{"relation",	"smallint unsigned"},
	{"createtime",  "int"},//如果是盟友 则记录盟友建立的时间 如果是敌对 则记录敌对开始的时间 因为敌对开始的时候和消除的时间有用
};

static DB_Colume IPAward_Colume[] =
{
	//	{"IP",			"varchar(20)"},
	{"AwardType",	"int"},
	{"StartTime",	"varchar(20)"},
	{"EndTime",		"varchar(20)"},
};

static DB_Colume Country_Colume[] =
{
	//{"CountryID",	"int unsigned"},
	{"Honour",			"smallint", "-1"},	//	国家荣誉
	{"kinguin",			"int unsigned"},	//	国王编号
	{"protector1",		"int unsigned"},	//	左臣编号
	{"protector2",		"int unsigned"},	//	右臣编号
	{"Gen",				"int unsigned"},	//	将军编号
	{"countrypower",	"int"},				//	国家能力
	{"appointtimes",	"int unsigned"},	//	任命次数
	{"countrygold",		"int"},				//	国家资金
	{"decree",			"varchar(256)"},	//	国王圣旨
	{"age",				"int"},				//	国家世纪
	{"time",			"int"},				//	成立时间
	{"homeid",			"int"},				//	国王帮会
	{"imposetimes",		"int"},				//	征税次数
	{"decreetimes",		"int"},
};

static DB_Colume CountryKing_Colume[] = 
{
	//{"CountryID",	"int unsigned"},
	{"age",			"int"},
	{"uin",			"int unsigned"},
	{"time",		"datetime"},
};

static DB_Colume MallItemBuyCnt_Colume[] =
{
	/*{"uin",			"int unsigned"},*/
	{"itemid",		"int unsigned"},
	{"buycnt",		"smallint unsigned"},
};

static DB_Colume Oath_Colume[] =
{
	//{"OathID",			"int"},
	{"Number",			"int"},
	{"Name",			"varchar(12)"},
};

static DB_Colume OfflineStallageItem_Colume[] =
{
	{"itemindex",           "int"},         
	{"stallageid",			"int"},			//属于哪个摊位
	{"selluin",				"int"},
	{"buyuin",				"int"},
	{"curprice",			"int"},
	{"maxprice",			"int"},
	{"itemkey",				"bigint"},
	{"dptype",				"smallint"},
	{"goldtype",			"smallint"},
	{"flag",				"int"},
	//{"itemdata",			"BLOB"}        //物品信息 基本用不到 用人物身上的信息 现在这个暂时不用
};

static DB_Colume OfflineStallageBuyItem_Colume[] = 
{
	//{"uin",                "int"}, //收购人
	{"itemid",             "int"}, //收购物品ID
	{"price",              "int"}, //收购单价
	{"num",                "int"}, //当前还剩收购数量
	{"itemorder",              "int"}, //收购编号
	{"boughtnum",          "int"}, //已收购数量
};

static DB_Colume OfflineStallageBoughtItem_Colume[] = 
{
	//{"touin",                "int"},
	{"fromuin",              "int"},
	{"itemkey",                "bigint"},
};

static DB_Colume OfflineStallageMessage_Colume[] = 
{
	{"fromuin",              "int"},
	{"says",                 "varchar(100)"},
	{"itemkey",              "bigint"},
	{"time",			     "int"},
};

static DB_Colume OfflineStallage_Colume[] = 
{
	{"stallageid",         "int"},     //摊位ID
	{"selluin",            "int"},     //摊位所属于UIN
	{"serverline",		   "int"},     //摊位所在线
	{"opened",             "int"},     //是否开着的
	{"begintime",          "int"},     //此次摆摊开始时间
	{"lasttime",           "int"},     //此次摆摊持续时间
	{"mapid",              "int"},
	{"mapx",               "int"},
	{"mapy",               "int"},
	{"says",			   "varchar(48)"},
	{"name",               "varchar(20)"},
	{"soldmoney",          "int"},
};

static DB_Colume MapCopyStage_Colume[] =
{
/*	{"uin",				"int"},*/
	{"mapcopyid",		"int"},
	{"stageinfo",		"BLOB"},
	{"resetcd",			"int"},
};

static DB_Colume ItemMarket_Colume[] =
{
/*	{"itemid",			"int unsigned"},*/
	{"prevprice",		"float"},
	{"curprice",		"float"},
	{"record_time",     "DATETIME"},
	{"record_buy_count","int unsigned", "0"},
	{"record_sell_count", "int unsigned", "0" },
};

static DB_Colume PlayerCopy_Colume[] =
{
	{"param",			"BLOB"},
	{"skin",			"BLOB"},
	{"skill",			"BLOB"},
	{"recover",			"smallint unsigned"},
	{"Name",			"varchar(20)"},
	{"job",				"smallint"},
	{"level",			"int"},
	{"sex",				"int"},
	{"rankindex",		"int"},
	{"lastindex",		"int"},
	{"FisrtOpen",		"int", "1"},
};

static DB_Colume SlaveCopy_Colume[] =
{
	{"param",			"BLOB"},
	{"skill",			"BLOB"},
	{"MasterName",		"varchar(20)"},
	{"level",			"int"},
	{"rankindex",		"smallint"},
	{"monid",			"int"},
	{"name",			"varchar(20)"},
};
////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//日志部分
static DB_Colume ActionData_Colume[] = 
{
	{"operation",		"varchar(20)"},
	{"time",			"datetime"},
};

static DB_Colume itemlog_Colume[] = 
{
	{"ItemID",			"int"},				//物品模板ID
	{"ItemKey",			"bigint"},	    	//由uin<<32|NextItemKey产生,产生后永不修改,可以作为物品在全世界的唯一id
	{"Operation",		"varchar(20)"},		//操作
	{"time",			"datetime"},		//时间
	{"Count",			"int"},				//数量
	{"TradeID",			"char(32)"},		//交易ID
	{"Dura",			"int"},		//耐久
	{"DuraMax",			"int"},		//最大耐久
};

static DB_Colume NoOnLineLog_Colume[] =
{
	{"onlinenum",		"varchar(200)"},
	{"totalNum",		"int"},
};

static DB_Colume WealthCheckLog_Colume[] =
{
	//{"uin",			"int"},
	{"Time",			"datetime"},
	{"GoldAdd",			"int"},
	{"GoldDec",			"int"},
};

static DB_Colume FlowerCheckLog_Colume[] = 
{
	{"Time",			"datetime"},
	{"FlowerAdd",		"int"},
	{"FlowerDec",		"int"},
};

static DB_Colume ExpException_Colume[] = 
{
	{"Type",			"varchar(16)"},
	{"ObjUin",			"int unsigned"},
	{"Level",			"int"},
	{"Exp",				"int"},
	{"Time",			"datetime"},
};

static DB_Colume GoldException_Colume[] = 
{
	{"Type",			"varchar(16)"},
	{"ObjUin",			"int unsigned"},
	{"Level",			"int"},
	{"Gold",			"int"},
	{"BindGold",        "int"},
	{"Time",			"datetime"},
};

static DB_Colume FlowerException_Colume[] = 
{
	{"Type",            "varchar(16)"},
	{"ObjUin",          "int unsigned"},
	{"Level",			"int"},
	{"Flower",          "int"},
	{"Time",            "datetime"},
};

static DB_Colume SlaveLog_Colume[] = 
{	
	{"uin",					"int unsigned"},
	{"slaveuin",			"int unsigned"},
	{"slot",				"int"},
	{"Type",				"int"},
	{"slaveindex",			"int"},
	{"level",				"int"},
	{"quality",				"int"},
	{"attribute",			"int"},
	{"skill",				"int"},	
	{"newuin",				"int unsigned"},
	{"Time",				"datetime"},
	{"SlaveKey",			"bigint unsigned"},
};

static DB_Colume GMCommandLog_Colume[] =
{
	//	{"uin",				"int unsigned"},
	{"GMCommand",		"varchar(128)"},
	{"Time",			"datetime"},
};

static DB_Colume Pre_Paid_Card_Colume[] =
{
	//  {"ID",			"int unsigned"}
	//	{"Card",		"varchar(16)"},
	{"Pwd",			"varchar(8)"},
	{"Rmb",			"float"},
	{"Used",		"smallint unsigned"},
};

static DB_Colume Pre_Paid_Card_Log_Colume[] =
{
	//  {"ID",			"int unsigned"}
	{"uin",			"int unsigned"},
	{"tip",			"varchar(256)"},
	{"ret",			"int unsigned"},
	{"time",		"datetime"},
};

static DB_Colume Home_Log[] = 
{
	{"homeid",		"int unsigned"},
	{"operator",	"varchar(20)"},
	{"exid",		"int"},
	{"exsz",		"varchar(50)"},
	{"time",		"datetime"},
	{"addpoint",    "int"},
	{"nowpoint",    "int"},
	{"totalpoint",  "int"},
};

static DB_Colume Honour_log_Colume[] =
{
	{"uin",			"int unsigned"},
	{"type",		"int unsigned"},
	{"objuin",		"int unsigned"},
	{"honour",		"int"},
	{"time",		"datetime"},
};

static DB_Colume Auction_log_Colume[] =
{
	{"uin",					"int unsigned"},
	{"AuctionType",			"int unsigned"},
	{"ActionType",			"int unsigned"},
	{"ItemKey",				"bigint"},
	{"Money",				"int"},
	{"Gold",				"int"},
	{"time",				"datetime"},
	{"ItemID",				"int"},
};

static DB_Colume LUAEvent_log_Colume[] =
{
//  {"ID",			"int unsigned"}
	{"Event",		"varchar(128)"},
	{"time",		"datetime"},
};

static DB_Colume GetWebResultLog_Colume[] =
{
//	{"uin",			"int unsigned"},
	{"URL",			"varchar(128)"},
	{"Rtn",			"varchar(128)"},
	{"time",		"datetime"},
};

static DB_Colume MailtLog_Colume[] =
{
//	{"uin",		"int unsigned"},
	{"RecvUin",		"int unsigned"},
	{"MailID",		"int unsigned"},
	{"Action",		"varchar(128)"},
	{"time",		"datetime"},
};

static DB_Colume CountrySystemLog_Colume[] =
{
//	{"id",			"int"},
	{"country",		"int"},
	{"operation",	"smallint"},
	{"param",		"int"},
	{"time",		"datetime"},
};

static DB_Colume SkillExpLog_Colume[] = 
{
	{"uin",			"int unsigned"},
	{"skillexp",	"int"},
	{"type",		"int"},
	{"time",		"datetime"},
};

static DB_Colume MapCopySportLog_Colume[] =
{
//	{"mapcopyid",		"int"},
	{"time",			"int"},
	{"completetime",	"int"},
	{"uin1",			"int unsigned"},
	{"uin2",			"int unsigned"},
	{"uin3",			"int unsigned"},
	{"uin4",			"int unsigned"},
	{"uin5",			"int unsigned"},
	{"uin6",			"int unsigned"},
};

static DB_Colume	  CollectStatus_Colume[]=
{
//	{"uin",					"int unsigned"},
	{"itemid",				"int unsigned"},
	{"collectedcount",	"int"},
};

static DB_Colume ItemMarketBillLog_Colume[]=
{
	{"uin",				"int unsigned"},
	{"billchange",		"int"},
	{"operator",		"varchar(64)"},
	{"billremain",		"int unsigned"},
	{"Action",			"varchar(128)"},
	{"time",			"datetime"},
};

static DB_Colume NaGoldLog_Colume[] = 
{
	{"uin",				"int unsigned"},
	{"billgold",		"int"},
	{"billbindgold",    "int"},
	{"operator",		"varchar(64)"},
	{"goldremain",		"int unsigned"},
	{"bindgoldremain",  "int unsigned"},
	{"Action",			"varchar(128)"},
	{"time",			"datetime"},
};

static DB_Colume HonourChangeLog_Colume[]=
{
	{"uin",             "int unsigned"},
	{"honourchange",	"int"},
	{"operator",		"varchar(64)"},
	{"honourremain",    "int unsigned"},
	{"byuin",			"int unsigned"},
	{"time",			"datetime"},
};


static DB_Colume FishGoldLog_Colume[]=
{
	{"uin",             "int unsigned"},
	{"operator",		"smallint"},
	{"curVal",			"int"},
	{"chgVal",			"int"},
	{"time",			"datetime"},
};

#endif