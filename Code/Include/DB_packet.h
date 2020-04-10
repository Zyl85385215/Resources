#ifndef _JCH_NEW_DB_PACKET_
#define _JCH_NEW_DB_PACKET_
#include "item_packet.h"
#include "char_packet.h"
#include "item_levelup_packet.h"
#include <sqltypes.h>
#include <windows.h>
#pragma pack (push)
#pragma pack (1)
#ifndef COLORATION
#define COLORATION
#endif

enum 
{
	DB_CONNECT,
	DB_CONNECT_OK,
	DB_DISCONNECT,
	DB_SELECT,
	DB_DELETE,
	DB_UPDATE,
	DB_INSERT,
	DB_RETURN,
	DB_SQLCMD,
	DB_RECORDSET,
	DB_CREATECHAR,           //
	DB_CHECK,
	DB_STARTLOAD,
	DB_SAVE,
	DB_SAVEOFF,
	DB_MEMDEL,
	DB_LOADGUILDS,
	DB_LOADCOUNTRY,
	DB_LOADMAPOWNER,
	DB_LOADALLPLAYERNAME,
	DB_LOADRANK,
	DB_LOADSCRIPTVAL,
};

enum
{
	RECORDSET_PLAYER,
	RECORDSET_PLAYERITEM,
	RECORDSET_PLAYERITEMEXTEND,
	RECORDSET_PLAYERSKILL,
	RECORDSET_PLAYERQUESTINFOS,
	RECORDSET_PLAYERQUESTVARS,
	RECORDSET_PLAYERRELATION,
	RECORDSET_PLAYERSTATE,
	RECORDSET_CHARINFO,
	RECORDSET_ARENARANK,
	RECORDSET_CONFLIST,
	RECORDSET_CONFMEM,
	RECORDSET_CONFITEM,
	RECORDSET_CONFREL,
	RECORDSET_SCRIPTVAL,
	RECORDSET_SLAVEBASE,
	RECORDSET_CHUNKDATA,
	RECORDSET_SHOPDATA,
	RECORDSET_SHOPITEM,
	RECORDSET_WANTEDLIST,
	RECORDSET_RANK,	
};

enum
{
	RANK_TYPE_HERO,
	RANK_TYPE_WEATH,
	RANK_TYPE_KILLER,
};

enum
{
	SELECT_PLAYEREXIT,
	SELECT_ALLPLAYERDATA,
	SELECT_CHARINFO,
	SELECT_SCRIPTVAL,
	SELECT_COUNTRY,
	SELECT_MAPOWNER,
	SELECT_RANK,
	SELECT_GUILDS,
	SELECT_ALLSIMPLEINFO,
};

enum
{
	INSERT_ACPOINTLOG,
	INSERT_PLAYERITEM,
	INSERT_PLAYERITEMEXTEND,
	INSERT_CONFINFO,
	INSERT_CONFITEM,
	INSERT_CONFMEM,
	INSERT_CONFREL,
	INSERT_SLAVEBASE,
	INSERT_CHUNKDATA,
	INSERT_ARENARANKITEM,
	INSERT_SCRIPTVAL,
	INSERT_WANTED,
	INSERT_SHOPDATA,
	INSERT_SHOPITEM,
	INSERT_SKILL,
	INSERT_STATE,
	INSERT_GUILDMEMBER,
	INSERT_GUILD,
	INSERT_MAIL,
	INSERT_QUESTVAR,
	INSERT_QUEST,
	INSERT_RUNLOG,
	INSERTDB_CREATECHAR,
};
enum 
{
	DELETE_PLAYERITEM,
	DELETE_PLAYERITEMEXTEND,
	DELETE_CONFITEM,
	DELETE_CONFMEM,
	DELETE_CONF,
	DELETE_CONFREL,
	DELETE_WANTED,
	DELETE_ARENARANK,
	DELETE_SHOPITEM,
	DELETE_SKILL,
	DELETE_ALLSKILL,
	DELETE_RELATION,
	DELETE_ALLSTATE,
	DELETE_MAIL,
	DELETE_REMOVEQUEST,
	DELETE_GUILEMEMBER,
	DELETE_GUILD,
};

enum
{
	UPDATE_ITEMDURA,
	UPDATE_ITEMDATA,
	UPDATE_PLAYERITEM,
	UPDATE_PLAYERITEMEXTEND,
	UPDATE_ITEMCOLOR,
	UPDATE_ITEMINDEX,
	UPDATE_ITEMSLOT,
	UPDATE_CHGITEMSLOT,
	
	UPDATE_RIDESP,
	UPDATE_RIDESLAVEEXP,
	UPDATE_SHOPITEM,
	UPDATE_CHUNKDATA,

	UPDATE_PLAYERCONFID,
	UPDATE_CONFCLOSE,
	UPDATE_CONFHEADER,
	UPDATE_CONFTENET,
	UPDATE_CONFMENOY,
	UPDATE_CONFMEMMONEY,
	UPDATE_CONFMEMKILLCNT,
	UPDATE_CONFMEMPOS,
	UPDATE_CONFMEMLEVEL,
	UPDATE_UNMARRY,
	UPDATE_SHOPMONEY,
	UPDATE_SHOPOWNER,
	UPDATE_SKILL,

	UPDATE_PLAYERHPMPPOS,
	
	UPDATE_PLAYERPOS,
	UPDATE_PLAYERATTRIB,
	UPDATE_PLAYERGOLD,
	UPDATE_SHORTCUT,
	UPDATE_PLAYEREXP,
	UPDATE_PKPRISON,
	UPDATE_PKCOUNT,
	UPDATE_PLAYERLVLUP,
	UPDATE_PLAYERHORSE,
	UPDATE_RESETATTRIB,
	UPDATE_PLAYERFLAG,
	UPDATE_PLAYERSTATE,

	UPDATE_STOREPWD,
	UPDATE_WANTEDKILLER,
	UPDATE_WANTEDGOLD,
	UPDATE_WANTEDTIME,
	UPDATE_PLAYERBALANCE,
	UPDATE_PLAYERJOB,
	UPDATE_PLAYERCOUNTRY,
	UPDATE_PLAYERNAME,
	UPDATE_HOMEPLACEINFO,
	UPDATE_VARVAL,
	UPDATE_ADDVARVAL,
	UPDATE_RESETONLINETIME,
	UPDATE_ONOFFLINETIME,
	UPDATE_COMPLETEQUEST,
	UPDATE_GUILDDUTYSETTING,
	UPDATE_GUILD,
	UPDATE_GUILDMEMBER,
};

enum
{
	CHECK_STOREPWD,
	CHECK_NAMEEXIST,
};

enum
{
	DBRETURN_UNMARRY,
	DBRETURN_CHECKSTOREPWD,
	DBRETURN_LOADOK,

};
///////////////////////////////////////////////////////////////////////////////

struct CREATECHAR:public CHARACTER_CHAR_CREATE_Data
{
	DWORD uin;
};

struct RUNLOGNAME
{ 
    int uin;
	int type;
	int online;
	char des[500];
};

struct MAPOWNERINFO           //地图所有信息----added by cf
{
	int MapIndex;
	BYTE MapOwner;
	BYTE MapForce;
};

struct PLAYERATTRIB           //人物的属性点---
{
	int Con;
	int Dex;
	int Ing;
	int Str;
	int Wis;
	int AttribPoint;
};

struct PLAYERRESETATTRIB : public PLAYERATTRIB   //人物属性点重新设置 -- change by cf
{
	WORD mp;
	WORD hp;
};

struct PLAYERLVLUP        //人物升级
{
	WORD mp;
	WORD hp;
	WORD attribpoint;
	DWORD exp;
	BYTE level;	
};

struct PLAYERINFO         //人物角色信息
{
	DWORD	uin;
	char	Name[20];
	short	sex;
	int		Position_Map;
	int		Position_X;
	int		Position_Y;
	int		zoneindex;
	BYTE	bWXjmsht;
	BYTE	job;
	BYTE	force;
	int		sx;
	int		sy;
	int		smap;
	int		con;
	int		dex;
	int		Ing;
	int		Str;
	int		Wis;
	int		AttribPoint;
	int		Exp;
	int		Level;
	int		SKIN;
	int		WeaponID;
	int		pkCount;
	int		pkPrison;;
	int		Lucky;
	int		HP ;
	int		MaxHp;
	int		MaxMp;
	int		MP ;
	int		Gold ;
	int		Balance ;
	byte	playerFlag[1024];
	short	SKIN2;
	int		FloorMap;
	ShortCut shortCut[40];	
	short	MarryFlag;
	int		CouplerUin;
	char	Nickname[48];
	int		ConfID;
	char	StorPass[20];
	char	szUsingFlag[3000];
	DWORD	QLEVEL[30];
	DWORD	OnLineTime;
	DWORD	OffLineTime;
};

struct PLAYERPOS       //人物位置
{
	int Position_Map;
	int Position_X;
	int Position_Y;
};

struct PLAYEREXIT:public PLAYERPOS
{
    int Hp;
	int Mp;
	int exp;
	int job;
	int level;
	int country;
};


struct PLAYERITEMINFO   //人物物品信息
{
	int		itemIndex; 
	short	Slot; 
	WORD	itemID;		//ItemBaseID
	WORD	Dura;		//持久
	WORD	DuraMax;
	BYTE	type;
	WORD	wData;
};

struct PLAYERITEMEXTENDINFO   //人物物品升级信息
{
	int			itemindex;
	short		slot;
	ItemExtend	xExtend;
};
//////////////////////////    ITEM       //////////////////////////
struct UPDATEITEMDURA    //
{
	DWORD itemindex;
	BYTE slot;
	WORD Dura;
	WORD MaxDura;
};

struct PLAYERITEMCOLOR   //
{
	BYTE slot;
	DWORD itemindex;
	WORD color;
};

struct PLAYERITEMDATA    //
{
    BYTE slot;
	DWORD itemindex;
	WORD data;
};

struct  PLAYERITEMINDEX      //人物物品index 改变
{
	DWORD oldIndex;
	DWORD newIndex;
	BYTE slot;
};

struct PLAYERITEMSLOT       //人物物品index 和 slot 改变
{
	DWORD oldIndex;
	DWORD newIndex;
	BYTE oldSlot;
	BYTE newSlot;
};

struct PLAYERITEMCHGSLOT    //人物物品slot 和 dura 改变
{
	WORD oldIndex;
	BYTE oldSlot;
	BYTE newSlot;
	WORD Dura;
};

struct PLAYERITEMKEY       //物品的index 和slot ---删除时使用
{
	int itemIndex;
	short Slot; 
};
///////////////////////////////////////   

struct UNMARRYINFO        //
{
	DWORD uin;
	DWORD couplerUin;
};

struct PLAYERSKILLINFO     //技能的信息------id
{
	WORD	SkillID; 
};

struct PLAYERSTATE         //人物状态的信息
{
	WORD	wID;
	BYTE	bRepeat;
	DWORD	dwInterval;
};

struct PLAYERQUESTITEM    //人物任务的信息
{
	int		nQID;
	bool	blFinished;
};

struct PLAYERQUESTVAR    //人物任务的变量
{
	int		nQID;
	char	varname[50];
	char	vardes[500];
	int		varval;
	int		varcompval;
	int		varfailval;
};

struct ACPOINTLOGINFO     
{
	DWORD ToUin;
	int Point;
	int LogIndex;
	TIMESTAMP_STRUCT Time;
};

struct SCRIPTVALINFO    //脚本相关的数据结构
{
	char  Name[64];
	short Type;
	int		Data;
};

struct WANTEDINFO   
{
	char Name[32];
	int  masterUin;
	int  wantedUin;
	int  gold;	
	int  timeCount;
	char killer[1024];
};

struct WANTEDGOLD
{
	int  gold;
	int  masterUin;
};

struct WANTEDTIME
{
	int  timeCount;
	int  masterUin;
};

struct ARENAINFO
{
	DWORD uin;
	char Name[32];
	int score;
	int timeCount;
};

struct RANKDATAINFO    //排行榜的结构
{
	DWORD uin;
	DWORD val1;
	DWORD val2;
	char Name[20];
};

struct CONFDATAINFO    //帮会信息的结构
{
	DWORD	dwConfID;
	int		HeaderUin;
	char	ConfTenet[128];
	char	ConfName[48];
};

struct CONFSETTINGINFO  //帮会设置的结构
{
    DWORD dwGuildID;
	int   indx;
	int   permitcode;
	char  des[50];
};

struct CONFMEMINFO     //帮会成员的结构
{
	DWORD	dwConfID;
	int		MemberUin;
	char	MemberName[20];
	char	LastLogin[20]; 
	int	MapIndex;
	int		MemberLevel;
	int		MemberPosition;
	char	Note[20];    //it's max 120 in db
	BYTE	Job;
};

struct GUILDMEMBERINFO
{
	BYTE DutyIndx;
	char Note[20];
	DWORD uin;
};

struct COUNTRYINFO
{
	BYTE btCountryID;
	DWORD dwOwner;
};

struct RELATIONINFO
{
	DWORD dwMeUin;
	DWORD dwPeerUin;
	BYTE  bRelation;
};

struct CHUNKINFO
{
	char name[50];
	int money;
	int def;
	int hp;
	int maxHp;
	int level;
	short status;
	int levelPoint;
	int owerUin;
	int goods[6];
};

struct SHOPINFO
{
	int	shopindex;
	char npcname[32];
	int mapindex;
	int money;
	int money1;
	int owner;
	char ownerName[20];
};

struct SHOPMONEY
{
	int money;
	int money1;
};

struct SHOPOWNER
{
	int owner;
	char ownerName[20];
};

struct SHOPITEMINFO
{
	int ItemID;
	int Num;
};

struct SLAVEBASEINFO
{
	int index;
	char name[64];
	int type;
	int subtype;
	int size;
	int speed;
	int atkArea;
	int atkType;
	int level;
};

struct UPDATERIDESP
{
	DWORD itemindex;
	BYTE slot;
	WORD ap;
	WORD dp;
};

struct UPDATERIDESLAVEEXP
{
	DWORD itemindex;
	BYTE slot;
	WORD maxSp;
	WORD maxMp;
};

struct UPDATECONFMEMMONEY
{
	DWORD uin;
	int	 money;
};

struct UPDATECONFMEMLEVEL
{
	DWORD uin;
	int	 level;
};

struct UPDATECONFMEMKILLCNT
{
	DWORD uin;
	int	 killCnt;
};

struct UPDATECONFMEMPOS
{
	DWORD uin;
	BYTE pos;
	char posname[12];
};

struct CHECKSTOREPWD
{
	DWORD uin;
	BYTE rlt;
};

struct RECORDSET
{
	BYTE	bRecordType;
	WORD	wRecordCnt;
	DWORD	dwKey;
	BYTE	data[1];
};

struct VARVAL
{
	int		QID;
	char	varname[50];
	short	val;
};

struct MAILSAVE
{
	DWORD 	fromuin;
	DWORD 	touin;
	BYTE 	type;
	DWORD 	itemid;
	DWORD 	itemcount;
	DWORD 	money;
	BYTE	willpayfor;
	DWORD 	sendtime;
	BYTE 	visited;
	char	fromplayer[64];
	char	title[64];
	char	body[64];
};

struct ONLINEINFO
{
	DWORD dwOn;
	DWORD dwOff;
};


struct DUTY
{
	char DutyName[64];
	WORD Permitcode;
};

struct DUTYSETTINGINFO
{
	int nCnt;
	DUTY  duty[1];
};

//////////////////////////////////////////////////////////////////////////
//DB packetS
struct NEWDB_CMD
{
	DWORD	dwCmdIndex;
};

struct NEWDB_DB_CONNECT_Data:public NEWDB_CMD 
{	
	char	szIP[20];
	char	szUser[20];
	char	szPWD[20];
	char	szDBName[20];
};

struct NEWDB_DB_SQLCMD_Data:public NEWDB_CMD 
{
	char	szSQLCmd[1];
};

struct NEWDB_DB_RECORDSET_Data
{
	DWORD	dwRealSize;
	DWORD	dwCompressSize;
	BYTE	data[1];
};

struct NEWDB_DB_UPDATE_Data:public NEWDB_CMD
{
	BYTE	bInfoType;
	DWORD	dwKey;
	BYTE	pData[1];
};

struct NEWDB_DB_INSERT_Data:public NEWDB_CMD
{
	BYTE	bInfoType;
	DWORD	dwKey;
	BYTE	pData[1];
};

struct NEWDB_DB_SELECT_Data:public NEWDB_CMD
{
	BYTE	bInfoType;
	DWORD	dwKey;
};

struct NEWDB_DB_DELETE_Data:public NEWDB_CMD
{
	BYTE	bInfoType;
	DWORD	dwKey;
	BYTE	pData[1];
};

struct NEWDB_DB_CHECK_Data:public NEWDB_CMD
{
	BYTE	bInfoType;
	DWORD	dwKey;
	BYTE	pData[1];
};

struct NEWDB_DB_RETURN_Data
{
	BYTE	bInfoType;
	BYTE	pData[1];
};

struct NEWDB_DB_CREATECHAR_Data 
{
	DWORD uin;
	char name[16];
	char storPass[16];
	BYTE sex;
	int	 smap;
	BYTE WXjmsht;
	BYTE country;
	BYTE job;
	DWORD sx;
	DWORD sy;	
};

#pragma pack (pop)
#endif
