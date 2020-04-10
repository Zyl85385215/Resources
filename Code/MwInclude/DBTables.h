#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 ���ݿ�汾                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#define DB_VERSION		2014091806
#define DB_LOG_VERSION	2014072301


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 ��Ϸ���ݿ�                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define DB_DATA_VERSION				"DBDataVersion"								//���ݿ��汾��

#define RPG_ACCOUNT_TABLE			"RpgAccountData"

#define RPG_INVITE_TABLE			"RpgInviteData"
#define RPG_ITEM_TABLE				"RpgItemData"				//��Ʒ����
#define RPG_EQUIPEX_TABLE			"RpgEquipExData"				//װ����������
#define RPG_USER_TABLE				"RpgUserData"				//��ɫ������
#define RPG_SLAVE_TABLE				"RpgSlaveData"		//������
//#define RPG_EQUIP_TABLE				"RpgEquipData"				//װ������
#define	RPG_FRIEND_TABLE			"RpgFriendData"
#define	RPG_MAIL_TABLE				"RpgMailData"
#define RPG_HOME_TABLE				"RpgmilyData"
#define RPG_NEWSLAVE_TABLE			"RpgNewSlaveData"
#define ACTIVATE_TABLE				"Activate"
#define RPG_SKILL_TABLE				"SkillData"
#define RPG_STATE_TABLE				"StateData"

#define RPG_QUEST_TABLE				"RpgQuestData"			//�����е������б�
#define RPG_WORLDTEST_TABLE			"RpgTestData"			//�������

#define RPG_FORT_TABLE			"RpgFortData"			//Ҫ��
#define RPG_ITEM_MANAGE_TABLE		"RpgItemManageData"			//��Ʒ(�����ʼ�,������)
#define RPG_SYS_MAIL_TABLE		"RpgSysMailData"			//ϵͳ�ʼ�

#define RPG_RANK_WAR_TABLE			"RpgWarRank"			//������

#define RPG_RANKDATA_TABLE		"RpgRankData"			//���а�
#define RPG_USERINFO_TABLE		"RpgUserInfoData"		//���а�������Ϣ
#define RPG_UWATCH_TABLE		"RpgUWatchData"		//���а�װ����Ϣ

#define RPG_NASALE_TABLE		"RpgNASale"			//������
#define RPG_WORLDFLAG_TABLE		"RpgWorldFlag"			//�������

#define RPG_MENTORING_TABLE		"RpgMentoringData"		//ʦͽ
#define RPG_MREQUEST_TABLE		"RpgMRequestData"		//ʦͽ����

#define RPG_ACHIEVEMENT_TABLE	"RpgAchievementData"	//�ɾ�

#define RPG_MARRY_TABLE			"RpgMarryData"		//���
#define RPG_MARRYREQUEST_TABLE		"RpgMarryRequestData"		//��������

#define RPG_IOSAD_TABLE			"iosADSupport"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 �������������                                            ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	MARK_REGACCOUNT_TABLE		"MkRegAccount"		//ÿ��ע����˺�����
//#define	MARK_LOGINACCOUNT_TABLE		"logLoginAccount"		//ÿ�յ�¼�˺�����,����û��¼���ſɼ���
#define MARK_ONLINECNT_TABLE		"MkOnlineCnt"		//��������,ÿ5���Ӽ�¼һ��,�������û�, ĳ��Ϊ��ǰ�û���
#define MARK_RMBCOST_TABLE			"MkRmbCost"			//��ֵ,����,���,���� ͳ��,��, ��, ��//�̳���Ʒ��������ͳ��,��, ��, ��
#define MARK_PAY_TABLE				"MkPay"	
#define MARK_LOGINOUT_TABLE			"MkLoginOut"	//��½���˳�	

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///                                                 ��־���ݿ�                                                ///
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#define	RPG_ITEM_LOG				"RpgItemLog"						
#define RPG_ACTION_LOG				"RpgActionLog"					//��Ϊ��־
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

//�˺ŷ�����ά��,ÿ��
static DB_Colume	MkRegAcc_Colume[] =
{
	//{"id",			"int unsigned"},	//�����ӵı��
	{"daynumber",		"int unsigned"},	//���ڱ��
	{"regcount",		"int unsigned"},	//ע������  ��¼����-ע������ �������û���
	{"ativatecount",		"int unsigned"},	//��������
	{"createcount",		"int unsigned"},	//������ɫ����
	{"effcount",		"int unsigned"},	//��Ч�û�����
	{"payusercount",		"int unsigned"},	//���������û�����
	{"paytotalcount",		"int unsigned"},	//����
	{"regcount",		"int unsigned"},	//ע������  ��¼����-ע������ �������û���
	{"logincount",		"int unsigned"},	//��¼����,�ų��ظ��˺�
	{"logintotal",		"int unsigned"},	//��¼����			
	{"rmbpay",			"int unsigned"},	//��ֵ����
	{"tpmark",			"smallint"},		//ƽ̨���
	{"daypayusercount",		"int unsigned"},	//�����û�����
	{"daypaytimecount",		"int unsigned"},	//���Ѵ���
	{"daypaytotalcount",		"int unsigned"},	//������
};
//��Ϸ������ά��
static DB_Colume	MkOnline_Colume[] =
{
	//{"id",			"int unsigned"},	//�����ӵı��
	{"timemark",		"varchar(20)"},		//ʱ����, "now"Ϊʵʱ����, ����5���Ӽ�¼һ�� 20140302-15:02
	{"newaccount",		"int unsigned"},	//��ע������
	{"oldaccount",		"int unsigned"},	//���û�����
};

static DB_Colume	MkRmbCost_Colume[] =
{
	//{"objid",			"int unsigned"},		//0��ʯ,  1����, 2��� , ������ƷID������, RMB���������������
	{"totalcnt",			"int unsigned"},	//�����100Ϊ��λ
	{"weekcnt",			"int unsigned"},
	{"daycnt",			"int unsigned"},
	{"checktime",		"datetime"},		//�������ʱ��
};

static DB_Colume	IosADSup_Colume[] =
{
	//{"objid",			"int unsigned"},	
	{"macad",			"varchar(32)"},	//�����100Ϊ��λ
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
	{"beinvitecode",	"varchar(12)"},		//ģ��ID
	{"rmbtotal",			"int","0"},
	{"tpid",			"smallint","0"},
	{"rechargetemp",	"int",	"0"},
	{"macaddress",		"varchar(32)"},
	{"temprmbpay",			"int unsigned"},	//��ʱ���ۻ���ֵ
	{"accstate",			"int unsigned"},	//���1

	{"apntoken",		"varchar(80)"},
	{"sysver",			"int"},
	{"idfa",		"varchar(50)"},
	{"fromad",			"smallint","0"},
	{"loginip",		"varchar(20)"},
	{"mainaccpswd",			"varchar(16)"},		//�޸����빦��,�������������
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
	{"rmindex",			"smallint"},	//�������ͼ
	{"rmposx",			"smallint"},	//�������ͼ
	{"rmposy",			"smallint"},	//�������ͼ
	{"paramhp",			"smallint"},	//��ǰѪ������	,��6λ��Ҫ����65535/2

	{"cfgflag",			"longBLOB"},	//�ɾʹ��
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
	{"everforbid",	"int unsigned"},	//���ý��� 1
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
	{"uinfrom",				"int unsigned"},		//���
	{"type",				"int unsigned"},		//����  0 ����, 1�չ�
	{"startsec",			"int unsigned"},		
	{"price",				"int unsigned"},		
	
	{"itemid",				"int unsigned"},
	{"itemcnt",				"int unsigned"},
	{"iteminfo",			"int unsigned"},

};

static DB_Colume RpgMail_Colume[] =
{
	//{"mailid",        "int unsigned"},
	{"send",				"int unsigned"},		//���,������
	{"recv",				"int unsigned"},		//���,������
	{"state",				"smallint"},		//���,������
	{"timetick",				"int unsigned"},		//���,������

	//{"item",				"int unsigned"},		//���,������
	//{"itemcnt",				"int unsigned"},		//���,������

	{"money",				"int"},		//���,������
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


	{"money",				"int"},		//���,������
	{"title",			"varchar(32)"},
	{"body",			"varchar(255)"},
	{"itemid",				"int unsigned"},
	{"itemcount",				"int unsigned"},
	{"bind",				"smallint"}

};

static DB_Colume RpgFriend_Colume[] =
{
	//{"uinA",				"int unsigned"},		//���
	{"uinB",				"int unsigned"},		//���,������

	{"rstype",				"smallint"},		//ģ��ID
	{"rsvalA",				"int"},		//
	{"rsvalB",				"int"},		//

};

static DB_Colume RpgInvite_Colume[] =
{
	//{"uinsvr",				"int unsigned"},		//���/���������
	{"invitecode",				"varchar(12)"},		//ģ��ID
	{"reachnum",				"smallint"},	//�ﵽ�����ĸ���
	{"checknum",				"smallint"},	//���չ��ĸ���
};

static DB_Colume RpgWorldTest_Colume[] =
{
	//{"uinA",				"int unsigned"},		//���

	{"star",				"smallint"},		//����
	{"count",				"int"},				//�������
	{"typeid",				"int"},				//���
	{"choose1",				"smallint"},		//ѡ��
	{"choose2",				"smallint"},
	{"choose3",				"smallint"},
	{"choose4",				"smallint"},
	{"correct",				"int"},				//��ȷ����
	{"name",				"varchar(20)"},		//�������
	{"awardflag",			"smallint"},		//�����Ƿ���ȡ

};

static DB_Colume RpgQuest_Colume[] =
{
	//{"uin",				"int unsigned"},		//���
	{"qid",					"int unsigned"},		//����ID

	{"progress",			"BLOB"},				//���������Ϣ

};

static DB_Colume RpgSkill_Colume[] =
{
	//{"uin",				"int unsigned"},		//���
	{"skill",					"int unsigned"},		//����ID

	{"slv",					"smallint"},		//���ܵȼ�
	{"spos",					"smallint"},		//���ܵȼ�
	{"cdsec",					"int unsigned"},		//���ܵȼ�
};

static DB_Colume RpgState_Colume[] =
{
	//{"uin",				"int unsigned"},		//���
	{"state",					"int unsigned"},		//����ID

	{"slv",					"smallint"},		//���ܵȼ�
	{"endsec",					"int unsigned"},		//���ܵȼ�
};

static DB_Colume RpgSlave_Colume[] =
{
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

	{"baseid",			"int unsigned"},		//ģ��ID

};

static DB_Colume RpgWorldFlag_Colume[] =
{
	//{"Uin",				"int unsigned"},
	{"flagdata",			"varchar(60)"},	

};

static DB_Colume RpgNewSlave_Colume[] =
{
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

	{"baseid",			"int unsigned"},		//ģ��ID
	{"kind",			"smallint"},

	{"name",			"varchar(20)"},
	{"slot",			"smallint"},			//����λ��,  ����,����,�ʼ�
	{"level",			"smallint"},
	{"exp",				"int unsigned"},
	{"hp",				"int"},
	{"mp",				"int"},
	{"data0",			"longBLOB"},
	{"data1",			"longBLOB"},
	{"variatlv",		"int"	,"0"},
	{"data2",			"longBLOB"},
	{"lifepoint",		"int",	"250"},			//��������
	{"eatgainlimit",	"int",	"0"},
};
static DB_Colume RpgItemManage_Colume[] =
{
	//{"id",				"int unsigned"},		//����
	{"type",					"smallint"},		//0 ��Ʒ
	{"data",					"BLOB"},
};
static DB_Colume RpgItem_Colume[] =
{
	//{"Uin",				"int unsigned"},		//���
	{"id",				"int unsigned"},		//���,������
	{"objkey",				"bigint"},				//����Ψһ

	{"baseid",			"int unsigned"},		//ģ��ID
	{"num",				"int unsigned"},		//�ѵ�����

	{"slot",			"smallint"},			//����λ��,  ����,����,�ʼ�
	{"pos",				"int"},					//��Ʒ����
	{"limittime",				"int unsigned"},					//ʱ��
	{"bind",				"int"},					//��
	//{"data",		"BLOB"},
};

//װ����������
static DB_Colume RpgEquipEx_Colume[] =
{
	//{"id",				"int unsigned"},		//����װ��
	{"Uin",				"int unsigned"},
	{"itemid",				"int"},
	{"lvl",				"int"},
	{"addtype1",				"int"},
	{"addrandom1",				"int"},
	{"addtype2",				"int"},
	{"addrandom2",				"int"},
	{"objkey",				"bigint"},				//����Ψһ
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
	{"mvalue",				"int unsigned"},	//�з��Ѻö�
	{"wvalue",				"int unsigned"},
	{"mpoint",				"int unsigned"},	//�з����ܶ�
	{"wpoint",				"int unsigned"},
	{"mskill",				"BLOB"},		//�з�����
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
// 	//{"Uin",				"int unsigned"},		//���
// 	{"id",				"int unsigned"},		//���,������
// 	{"objkey",				"bigint"},				//����Ψһ
// 
// 	{"baseid",			"int unsigned"},		//ģ��ID
// 
// 	{"slot",			"smallint"},			//����λ��,  ����,����,װ����,�ʼ�
// 	{"lv",			"int unsigned"},
// 	//{"star",		"BLOB"},
// };

static DB_Colume DBVersionTable_Colume[] =
{
	{"version",			"int"},
};



////////////////////////////////////////��־/////////////////////////////////////////////////////////

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
	{"ctime",		"datetime"},			//ʱ��
	{"leftrmb",		"int unsigned"},		//���
	{"addval",			"int"},				//ʹ�û�������
	{"type",			"smallint"},		//ʹ��ԭ��
	{"logdesc",			"varchar(64)"},
};

static DB_Colume RpgBindRMBLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},			//ʱ��
	{"leftrmb",		"int unsigned"},		//���
	{"addval",			"int"},				//ʹ�û�������
	{"type",			"smallint"},		//ʹ��ԭ��
	{"logdesc",			"varchar(30)"},
};

static DB_Colume RpgSlaveLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},			//ʱ��
	{"slaveid",		"int unsigned"},		//������ID
	{"baseid",		"int unsigned"},		//ģ��ID
	{"lvl",			"smallint"},		//����
	{"variatlv",			"smallint"},		//����
	{"action",			"smallint"},		//����
	{"logdesc",			"varchar(30)"},
};

static DB_Colume RpgGMCodeLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"ctime",		"datetime"},			//ʱ��
	{"name",		"varchar(20)"},
	{"code",			"varchar(100)"},
};

static DB_Colume RpgItemLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int"},			//ʱ��
	{"itemcnt",		"int"},		//���
	{"itemtype",			"int"},				//ʹ�û�������
	{"ctime",		"datetime"},			//ʱ��
};

static DB_Colume RpgItemChangeLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int"},			//
	{"itemcnt",		"int"},		//
	{"touin",		"int"},			//
	{"type",		"smallint"},
	{"ctime",		"datetime"},			//ʱ��
};
static DB_Colume RpgPayErrorLog_Colume[] =
{
	//	{"tpid",				"int unsigned"},
	{"ctime",		"datetime"},			//ʱ��
	{"logdesc",			"varchar(32)"},		//
};
static DB_Colume RpgLuaLog_Colume[] =
{
	//	{"id",				"int unsigned"},
	{"logdesc",			"varchar(1024)"},		//
	{"bsuccess",		"int unsigned"},
	{"ctime",		"datetime"},			//ʱ��
};
static DB_Colume MkQICntLog_Colume[] =
{
	//{"objid",			"int"},			//ʱ��
	{"objtype",		"int"},		//���ܴ���
	{"objcnt",		"int"},		//���ܴ���
};

static DB_Colume RpgLvupLog_Colume[] =
{
	//{"uin",			"int"},
	{"lvl",		"int"},		//�ȼ�
	{"ctime",		"datetime"},	//ʱ��
};

//���а���־���ṹ�����а�һ��
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
	{"ctime",		"datetime"},	//ʱ��

};

static DB_Colume RpgGoldFromLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"addval",		"int unsigned"},	
	{"money",		"int unsigned"},
	{"type",		"smallint"},
	{"ctime",		"datetime"},	//ʱ��

};

static DB_Colume RpgEquipStarLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int unsigned"},
	{"starcnt",		"int unsigned"},	
	{"ctime",		"datetime"},	//ʱ��
};

static DB_Colume RpgEquipStarExLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"itemid",		"int unsigned"},
	{"changecnt",	"int"},	
	{"starlv",		"int unsigned"},	
	{"ctime",		"datetime"},	//ʱ��
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
	{"type",			"smallint"},		//0���� 1 �뿪��Ϸ
	{"logtime",			"datetime"},
};

static DB_Colume RpgSlaveLvLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"slaveid",		"int unsigned"},
	{"baseid",		"int unsigned"},	
	{"npos",		"smallint"},	
	{"poslvl",		"int unsigned"},	
	{"ctime",		"datetime"},	//ʱ��
};

static DB_Colume RpgFortLog_Colume[] =
{
	//	{"Uin",				"int unsigned"},
	{"chgscore",	"int"},
	{"warscore",	"int"},
	{"curpos",		"int unsigned"},	
	{"manorlv",		"smallint"},	
	{"manorexp",	"int unsigned"},	
	{"forttype",	"smallint"},		//1������ 2ׯ԰
	{"ctime",		"datetime"},	//ʱ��
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

