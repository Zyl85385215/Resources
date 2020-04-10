#ifndef _DB_DEFINE_H_
#define _DB_DEFINE_H_

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 ���ݿ�汾                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////



#define DB_VERSION		2013081501
#define DB_LOG_VERSION	2012061301
//	ע�⣺�����ݿ�ı�ṹ�޸�ʱ��һ��Ҫע���޸�����汾�ţ����򲻻�������ݿ��ṹ
//	�����ʽΪ��	ʱ��(������+����汾�ţ�

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 ��Ϸ���ݿ�                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DB_VERSION_TABLE			"DBVersionTable"								//���ݿ��汾��


#define OP_CARD_TABLE				"OpCardData"				//��������
#define OP_MAGIC_TABLE				"OpMagicData"				//��������
#define OP_ITEM_TABLE				"OpItemData"				//��Ʒ����
#define OP_USER_TABLE				"OpUserData"				//�˺ŵ�������
#define OP_EQUIP_TABLE				"OpEquipData"				//װ������
#define	OP_FRIEND_TABLE				"OpFriendData"
#define	OP_MAIL_TABLE				"OpMailData"
#define MW_HOME_TABLE				"FamilyData"
#define OP_WORLD_FLAG_TABLE			"OpWorldFlagData"

#define PLAYER_TABLE_NAME			"CharData"										//�����ɫ����
#define SKILL_TABLE_NAME			"CharSkill"                                     //���＼��
#define STATELIST_NAME				"StateList"                                     //״̬
#define SLAVE_TABLE_NAME			"charSlave"                                     //������
#define QUEST_LIST					"Questlist"                                     //�����б�
#define QUEST_VAR					"QuestVar"                                      //�������
#define DROIT_TABLE_NAME			"DroitTable"									//Ȩ��
#define CHAR_TITLE					"CharTitle"										//ͷ��

#define ITEM_TABLE_NAME				"CharItem"                                      //������Ʒ
#define CHAR_BOMB_DATA				"CharBomb"
#define CHAR_PURPOSE				"PurposeSys"

#define HOME_TABLE                  "Home"                                          //���
#define HOME_ITEM_TABLE             "HomeItem"                                      //���ֿ�
#define HOME_MEMBER_TABLE           "HomeMember"                                    //����Ա
#define HOME_SKILL_TABLE			"HomeSkill"										//��Ἴ��
#define HOME_RELATION_TABLE			"HomeRelation"									//����ϵ
#define HOME_LEAVE_MEMBER_TABLE     "HomeLeaveMember"                               //����Ա�뿪����б�
#define HOME_DEATAL_RECORD          "HomeDeatalRecord"                              //�������֧����ϸ
#define HOME_HALL_TABLE             "HomeHall"                                      //����ÿ�

#define CHAR_RELATION_NAME			"CharRelationTable"                             //����
#define ITEM_SELLBUY_TABLE			"ItemSellBuyTable"								//��Ʒ����
#define MAIL_STORAGE				"MailStorage"									//�ʼ�����
#define MAIL_ITEMS					"MailItems"										//�ʼ���Ʒ����
#define AUCTION_LIST_TABLE			"AuctionList"									//����
#define AUCTION_CURRENT_TABLE		"AuctionCurrent"								//��ǰ����
#define SCRIPT_VAL_TABLE_NAME		"ScriptVal"                                     //��ұ��λ����
#define ITEM_WING_DATA				"WingData"                                     //��ұ��λ����

#define CENTERSERVERCYCLETIME_TABLE	"CenterServerCycleTimeTable"					//ѭ��ʱ���¼�
#define GLOBAL_VALUE_TABLE			"GlobalValueTable_Ex"							//ȫ�ֱ�����
#define MAPOWNER_TALBLE_NAME		"MapOwnerTable"                                 //��ͼ����
#define PRE_PAID_CARD_TABLE			"PrePaidCard"									//�㿨
#define IP_AWARD_TABLE				"IPAward"										//IP����
#define COUNTRY_TABLE				"Country"	
#define NEWAUCTION_TABLE			"newauction"									//����
#define ACTIVITY_RANK				"activityrank"
#define MALLITEM_BUYCNT_TABLE		"MallItemBuyCnt"								//�����̳���Ʒ�Ĵ���ͳ�ƣ������ڴ��д������Ƶ��̳���Ʒ��
#define COUNTRY_KING_TABLE			"CountryKing"									
#define RANK_EQUIP_TABLE			"rankequip"
#define OATH_TABLE					"Oath"											//����
#define MAPCOPY_STAGE_TABLE			"MapCopyStageTable"								//������

#define RANK_SLAVE_TABLE			"rankslave"

#define MATERIAL_COLLECTED_STATUS_TABLE	"CollectStatus"								//�ɼ���Ʒ����
#define MAP_COPY_SPORT_LOG_TABLE	"mapCopySportLog"

#define ITEM_MARKET_TABLE			"ItemMarket"									//��Ʒ��������
#define BOOK_SKILL_COLLECT_TABLE	"bookSkill"										//�ؼ��ռ�
#define AWARD_TABLE					"AwardItem"										//���ｱ����ȡ
#define PLAYER_COPY_TABLE			"PlayerCopy"									//����ģ���������ݱ�
#define SLAVE_COPY_TABLE			"SlaveCopy"										//����ģ�³������ݱ�
#define OFFLINE_STALLAGE_ITEM       "OfflineStallageItem"                           //���߰�̯��Ʒ
#define OFFLINE_STALLAGE            "OfflineStallage"                               //���߰�̯��̯λ
#define OFFLINE_STALLAGE_BUYITEM    "OfflineStallageBuyItem"                        //�չ���Ʒ
#define OFFLINE_STALLAGE_BOUGHTITEM "OfflineStallageBoughtItem"                     //�չ���Ʒ���
#define OFFLINE_STALLAGE_MESSAGE    "OfflineStallageMessage"                        //���Ա�
#define WAR_MACHINE_TABLE			"WarMachine"
#define MAIL_NOTICE_TABLE					"mailnotice"					//���߼���

#define IOS_FARM_TABLE				"iosfarm"
#define IOS_HORSESOUL_TABLE			"iosHorseSoul"									//��� 

#define NEW_BOOK_COLLECT_TABLE      "NewBookCollect"                                //����
#define NEW_BOOK_PIECE_TABLE		"NewBookPiece"                          //����Ƭ
#define PLAYER_ACHIEVE_TABLE		"PlayerAchieve"								//�ɾ�

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 ��־���ݿ�                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	OP_ITEM_LOG					"OpItemLog"						//��Ʒ������־
#define OP_ACTION_LOG				"OpActionLog"					//��ҵ���Ϊ��־


#define NUM_ONLINE_LOG				"OnLineLog"                                     //��¼����������ȫ��������
#define ACTION_TABLE_NAME	    	"NewActionData"                                 //��Ҽ�¼
#define ITEMLOG_TABLE_NAME			"Newitemlog"                                    //��Ʒ��¼ 
#define EXP_EXCEPTION_TB			"NewExpException"								//�����쳣���
#define GOLD_EXCEPTION_TB			"NewGoldException"								//��Ϸ���쳣���
#define FLOWER_EXCEPTION_TB         "NewFlowerException"
#define SLAVE_LOG_NAME				"NewSlaveLog"									//��Ӳ�����¼
#define GM_COMMAND_LOG				"GMCommandLog"									//GM������־
#define PRE_PAID_CARD_LOG_TABLE		"PrePaidCardLog"								//���ͳ�ֵ��������־
#define HOME_LOG					"homelog"										//�����־
#define HONOUR_LOG					"HonourLog"										//��ѫ��־
#define AUCTION_LOG					"AuctionLog"									//������־
#define LUAEVENT_LOG				"LUAEventLog"									//LUA�¼���־
#define NEWAUCTION_LOG				"newauctionLog"
#define GETWEBRESULTLOG				"GetWebResultLog"
#define MIALLOG						"MailLog"
#define COUNTRYSYSTEM_LOG			"CountrySystemLog"								//����ϵͳ�Ĳ�����־
#define WEALTHCHECK_LOG				"wealthchecklog"
#define FLOWERCHECK_LOG             "flowerchecklog"
#define SKILLEXP_EXCEPTION_TB		"SkillExpException"
#define ITEM_MARKET_BILL_LOG		"itemmarketbilllog"
#define NA_GOLD_TABLE_LOG           "NaGoldlog"                                     //����Ԫ����ƷԪ����ȡ���˻���־
#define HONOUR_CHANGE_LOG			"HonourChangeLog"								//�����仯��־
#define FISH_GOLD_LOG				"FishGoldLog"								//�������

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
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

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
	{"send",				"int unsigned"},		//���,������
	{"recv",				"int unsigned"},		//���,������
	{"state",				"smallint"},		//���,������
	{"timetick",				"int unsigned"},		//���,������

	{"item",				"int unsigned"},		//���,������
	{"itemcnt",				"int unsigned"},		//���,������

	{"money",				"int unsigned"},		//���,������
	{"title",			"varchar(32)"},
	{"body",			"varchar(255)"},
};

static DB_Colume OpFriend_Colume[] =
{
	//{"uinA",				"int unsigned"},		//���
	{"uinB",				"int unsigned"},		//���,������

	{"rstype",				"smallint"},		//ģ��ID
	{"rsval",				"int"},		//�ѵ�����


};

static DB_Colume OpItem_Colume[] =
{
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

	{"baseid",			"int unsigned"},		//ģ��ID
	{"num",				"int unsigned"},		//�ѵ�����

	//{"slot",			"smallint"},			//����λ��,  ����,����,�ʼ�

	//{"star",		"BLOB"},
};

static DB_Colume OpEquip_Colume[] =
{
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

	{"baseid",			"int unsigned"},		//ģ��ID

	{"slot",			"smallint"},			//����λ��,  ����,����,װ����,�ʼ�
	{"lv",			"int unsigned"},
	//{"star",		"BLOB"},
};


static DB_Colume OpMagic_Colume[] =
{
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

	{"baseid",			"int unsigned"},		//ģ��ID
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
	{"TodayOfflineTime2Store",	"int"},	// ���ջ��ж�������ʱ����Դ洢���������߾���ϵͳ
	{"OfflineTime2Use",			"int"},	// ����ʹ��ʱ�䣬�������߾���ϵͳ
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
	{"TotalOnlineCount","int"},	//�ý�ɫ�ܹ�������Ϸʱ��
	{"ItemMarketBill",	"int"},
	{"NFriends","BLOB",""},
	{"Invitedfriends","BLOB",""},
	{"bookskillbase","BLOB",""},	
	{"PlayerKillCount",	"BLOB", ""},
	{"IsEvilMode",		"smallint"},
	{"IsHeimuya",		"smallint"},
	{"IsPveMode",		"smallint"},
	{"transportlist",	"BLOB"}, //��·���¼�б�
	{"total_integer", "int unsigned"},  // �ܻ���	
	{"total_time", "int unsigned"}, // �ܵ�ʱ��
	{"curweek_integer", "int unsigned"}, // ��ǰ�ܵ��ۼƻ��֣�Ϊ���㽱�����֣�
	{"today_integer", "int unsigned"}, //  �����ۼƵĻ��֣�Ϊ�������ƺ�Ļ��֣�
	{"addhomemptime", "smallint"},     //  �������߸����ӵ�ʿ������
	{"deleteTime","int unsigned"},	   // ����ɾ��ʱ��
	{"ArenaScore", "int"},
	{"ArenaWinBattle", "int"},
	{"ArenaTotalBattle", "int"},
	{"blockflag", "smallint"},
	{"newbetime", "int"},
	{"MsgFlag",	"int"},
	{"speciallv","smallint", "0"},
	{"NaGold", "int"},
	{"NaBindGold", "int"},
	{"ActExPoint", "int", "0"},			//�����
	{"DinghunState", "int"},
	{"FishScore", "int"},
	{"homestates",	"BLOB", ""}, //��·���¼�б�
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
	{"PlayerSwordSprite",	"BLOB", ""}, //����
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
	//{"ID",              "int"},  //����ID
	{"lvl",             "int"},  //���µȼ�
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
	{"part100lv",		"smallint"},		//��ֳ��
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
	{"itemtrump",		"BLOB"},	//����2����
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
	{"itemrepute",        "BLOB"},	//����װ����
	{"HorseEq",				"BLOB"},//��������
	{"enchaseEx",			"TINYBLOB"},
};

static DB_Colume CharBomb_Colume[] = 
{
	{"quality",			"smallint"},
	{"quaexp",			"int"},
	{"level",			"smallint"},
	{"exp",				"int"},		//����ʱ��
	{"maxhp",			"int"},		//��ͨ�ȼ�
	{"maxmp",			"int"},		//������Ŀ
	{"atkw",			"int"},		//�������ܿ���
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


	// ԭʼ����
	{"base_str",		"int"},
	{"base_wis",		"int"},
	{"base_ing",		"int"},
	{"base_dex",		"int"},
	{"base_con",		"int"},
	// ����Ϣ
	{"bindstatus",		"int"},
	{"needunbindtime",	"int"},
	{"unbindtickcount",	"int"},
	{"bindlevel",		"int"},
	// ��ȴ
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
	{"HomeID",       "int"},//���ID
	{"Uin",          "int"},//������UIN 
	{"ItemID",       "int"},
	{"Num",          "int"},
	//{"itemkey",		 "bigint"},//����Ʒ��KEY
	{"itemdata",	 "BLOB"},   //����Ʒ�ľ�������
	{"ItemIndex",        "int"},//����Ʒ�ڲֿ��е�INDEX
};

static DB_Colume HomeTb_Colume[] = 
{
	{"HomeName",		"varchar(48)"},		// �������
	{"HeaderUin",		"int"},				// ������Ե�Uin
	{"HomeTenet",		"varchar(128)"},	// �����ּ
	{"Level",           "int"},             // ���ȼ�
	{"Money",           "int"},             // ����ʽ�
	{"MenagePoint",     "int"},             // �������
	{"Power",           "int"},             // ��ᷱ�ٶ�
	{"ResourceLevel",   "int"},             // �����Դ�ȼ�
	{"EnForcelevel",	"int"},             // ���ǿ���ȼ�
	{"RebuilderUin",    "int"},             // �Լ���Ա��Uin
	{"RebuildTime",     "int"},             // �Լ�����ʱ��
	{"Honour",			"int"},             // ��Ṧѫ
	{"RenameTimes",		"int"},				// ����������
	{"HomeICON",		"smallint", "-1"},	// ���ͼ��
	{"HomeProductsInfo","BLOB"},			// ����ҵ
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
	{"MemberUin",		"int"},				// ����Ա��Uin
	{"MemberPosition",	"int"},				// ��Ա�ڰ���е�ְλ		
	{"MemberPower",     "int"},             // ��Ա�Ĺ��׶�
	{"TotalPower",      "int"},             // ��Ա���ۼƹ��׶� 
	{"TotalHomeVal",    "int"},             // ��Ա������ֵ
	{"MemberPoint",     "int"},
	{"TotalPoint",      "int"},
	{"TipTimes",		"int"},				//���ڿͻ��˵���ʾ��Ŀǰֻ��Ҫ�������������ˣ�������������
	{"nDays",           "int"},
	{"nContributeTime", "int"},
	{"nHallID",         "int"},             // �ÿ�ID
	{"nHallDutyIndex",  "int"},             // �ÿ�ְλ
	{"nTotalGold",      "int"},
};

static DB_Colume HomeLeaveMemberTb_Colume[] = 
{
	{"MemberUin",		"int"},				// ����Ա��Uin
	{"HomePower",       "int"},             // �üһｨ����� ������ʱ����ʿ���Ͱ���ʽ���
	{"HomeFund",        "int"},
	{"MemberPoint",     "int"},             // �üһ�ĸ��˹��׶Ⱥ��ܹ��׶ȱ���
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
	{"state",			"int"},				//������ɱ�־
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
	{"ID",				"int"},			//˳���
	{"IDEx",			"int"},		//���ID
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
	{"createtime",  "int"},//��������� ���¼���ѽ�����ʱ�� ����ǵж� ���¼�жԿ�ʼ��ʱ�� ��Ϊ�жԿ�ʼ��ʱ���������ʱ������
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
	{"Honour",			"smallint", "-1"},	//	��������
	{"kinguin",			"int unsigned"},	//	�������
	{"protector1",		"int unsigned"},	//	�󳼱��
	{"protector2",		"int unsigned"},	//	�ҳ����
	{"Gen",				"int unsigned"},	//	�������
	{"countrypower",	"int"},				//	��������
	{"appointtimes",	"int unsigned"},	//	��������
	{"countrygold",		"int"},				//	�����ʽ�
	{"decree",			"varchar(256)"},	//	����ʥּ
	{"age",				"int"},				//	��������
	{"time",			"int"},				//	����ʱ��
	{"homeid",			"int"},				//	�������
	{"imposetimes",		"int"},				//	��˰����
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
	{"stallageid",			"int"},			//�����ĸ�̯λ
	{"selluin",				"int"},
	{"buyuin",				"int"},
	{"curprice",			"int"},
	{"maxprice",			"int"},
	{"itemkey",				"bigint"},
	{"dptype",				"smallint"},
	{"goldtype",			"smallint"},
	{"flag",				"int"},
	//{"itemdata",			"BLOB"}        //��Ʒ��Ϣ �����ò��� ���������ϵ���Ϣ ���������ʱ����
};

static DB_Colume OfflineStallageBuyItem_Colume[] = 
{
	//{"uin",                "int"}, //�չ���
	{"itemid",             "int"}, //�չ���ƷID
	{"price",              "int"}, //�չ�����
	{"num",                "int"}, //��ǰ��ʣ�չ�����
	{"itemorder",              "int"}, //�չ����
	{"boughtnum",          "int"}, //���չ�����
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
	{"stallageid",         "int"},     //̯λID
	{"selluin",            "int"},     //̯λ������UIN
	{"serverline",		   "int"},     //̯λ������
	{"opened",             "int"},     //�Ƿ��ŵ�
	{"begintime",          "int"},     //�˴ΰ�̯��ʼʱ��
	{"lasttime",           "int"},     //�˴ΰ�̯����ʱ��
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
//��־����
static DB_Colume ActionData_Colume[] = 
{
	{"operation",		"varchar(20)"},
	{"time",			"datetime"},
};

static DB_Colume itemlog_Colume[] = 
{
	{"ItemID",			"int"},				//��Ʒģ��ID
	{"ItemKey",			"bigint"},	    	//��uin<<32|NextItemKey����,�����������޸�,������Ϊ��Ʒ��ȫ�����Ψһid
	{"Operation",		"varchar(20)"},		//����
	{"time",			"datetime"},		//ʱ��
	{"Count",			"int"},				//����
	{"TradeID",			"char(32)"},		//����ID
	{"Dura",			"int"},		//�;�
	{"DuraMax",			"int"},		//����;�
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