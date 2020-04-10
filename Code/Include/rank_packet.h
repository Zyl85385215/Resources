#ifndef _RANK_PACKET_H_
#define _RANK_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define _RANK_MAX_ 20

enum 
{
	RANK_LIST,
	RANK_DATA,
	RANK_VIEW,
	RANK_VIEW_EX,
	VIEW_PLAYER,
	VIEW_REQUIP,
	RANK_PERSON,

	ADD_SLAVE_INFO,
};

enum RANKTYPE                     //排行榜种类
{
	RANK_HERO,//等级
	RANK_WEATH,//财富
	RANK_KILLER,//恶人
	RANK_MAJOR,  //新手
	RANK_ZHANSHI,//猛士
	RANK_XIAKE,//剑客
	RANK_DAOSHI,//道士
	RANK_YAOSHI,//药
	RANK_MAJOR_SHANG,//新手上旬（隐藏）
	RANK_MAJOR_ZHONG,//新手中旬（隐藏）
	RANK_MAJOR_XIA,//新手下旬（隐藏）
	RANK_HOME,//帮会
	RANK_COUNTRY,//国家（隐藏）
	RANK_HONOUR,//官阶
	RANK_XUANXIU_MAN,//（隐藏）
	RANK_XUANXIU_WOM,//（隐藏）
	RANK_EQUIP,//装备
	RANK_TRUMP,//（隐藏）
	RANK_FLOWER,//（隐藏）
	RANK_INTERER_SCORE,//积分
	RANK_ARENA_SCORE,
	RANK_ACTEX_POINT,
	RANK_SLAVE,
	RANK_MARRY_POINT,
	RANK_HOME_CONTRIBUTION,	//帮会捐献
	RANK_TEACH_MONEY,	//名师提成
	RANK_TEACH_STUDENTS,	//名师高徒
	RANK_EQUIP_SCORE,	//装备评分
	RANK_FLOWER_TOTAL_POINT,
	RANK_FLOWER_DAY_POINT,
	RANK_FLOWER_WEEK_POINT,
	RANK_FLOWER_MONTH_POINT,
	RANK_TOTAL,//注意以上的枚举值一一对应于界面上的控件，用INTERFACE工具打开Interface.IFP后order的界面上，控件都在，但是有些是隐藏了
};


enum COUNTRYRANKTYPE                     //国家性的排行榜
{
	COUNTRY_ALL,
	COUNTRY_QIGUO,
	COUNTRY_CHUGUO,
	COUNTRY_ZHAOGUO,
	COUNTRY_WUGUO,
	COUNTRY_QINGGUO,
	COUNTRY_TOTAL,
};

struct RANK_RANK_LIST_Data 
{
	BYTE       nCount;
	BYTE       byType;
	BYTE	   byCountry;
	//char       szTitle[1];
};

struct RANK_RANK_DATA_Data 
{
	DWORD      dwUin;
	DWORD      dwVal1;
	DWORD      dwVal2;
	BYTE       byPosition;
	BOOLEAN    bOnline;
	char       szName[1];
};

struct  RANK_ADD_SLAVE_INFO_Data
{
	DWORD	dwPoint;
	//DWORD	dwSlaveUin;
	DWORD	dwUin;
	char	szName[1];
};


struct RANK_RANK_PERSON_Data
{
	DWORD dwOrder;
};

struct RANK_VIEW_REQUIP_Data 
{
	DWORD      dwIndex;
	DWORD      dwPoint;
	DWORD      dwItemID;
	BYTE       byPosition;
	BYTE		byType;
	BYTE		byCountry;
	char       szName[1];
};

struct RANK_RANK_VIEW_Data 
{
	DWORD      dwUin;
	WORD       wRankIndex;
	BYTE       byCountry;
	BYTE       byOnlySelf; // 只取个人排行信息 add by zp
};

struct RANK_RANK_VIEW_EX_Data 
{
	DWORD      dwUin;
	WORD       wRankIndex;
	BYTE	   byCountry;
	BYTE       byStart;
	BYTE	   byCnt;
};

struct RANK_VIEW_PLAYER_Data 
{
	DWORD      dwUin;
	char	   szDest[1];
};

#pragma pack (pop)
#endif