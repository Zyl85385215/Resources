#ifndef _GEM_UPGRADE_SPLITE_PACKET_H_
#define _GEM_UPGRADE_SPLITE_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define GEM_SUCCESS					0x00
#define	GEM_FAILED_TYPE				0x01
#define	GEM_FAILED_STONE			0x02
#define	GEM_FAILED_TIME				0x03
#define	GEM_FAILED_MONEY			0x04
#define	GEM_FAILED_OTHER			0x05

enum {	
	GEM_UPGRADE_OPEN,
	GEM_UPGRADE_CLOSE,
	GEM_UPGRADE_NOW,
	GEM_UPGRADE_OK,			
	GEM_UPGRADE_RESULT,	
};

enum {
	gemupgrade_rlt_success,
	gemupgrade_rlt_failed,
	gemupgrade_rlt_checknomoney,
	gemupgrade_rlt_checkfailed,
	gemupgrade_rlt_failedothers,
};

enum {
	gemsplite_rlt_success,
	gemsplite_rlt_failed,
	gemsplite_rlt_checknomoney,
	gemsplite_rlt_checkfailed,
	gemsplite_rlt_failedothers,
};

enum {
	equiphole_rlt_success,
	equiphole_rlt_failed,
	equiphole_rlt_checkmoney,
	equiphole_rlt_checkfailed,
	equiphole_rlt_noequip,
	equiphole_rlt_cannothole,
	equiphole_rlt_levellimit,
	equiphole_rlt_noholeitem,
	equiphole_rlt_failedothers,
};

enum {
	leveluphole_rlt_success,
	leveluphole_rlt_failed,
	leveluphole_rlt_checkmoney,
	leveluphole_rlt_checkfailed,
	leveluphole_rlt_noequip,
	leveluphole_rlt_cannothole,
	leveluphole_rlt_levellimit,
	leveluphole_rlt_noholeitem,
	leveluphole_rlt_failedothers,
	leveluphole_rlt_cannotlevel,
};

enum {
	addgem_rlt_success,
	addgem_rlt_failed,
	addgem_rlt_checkmoney,
	addgem_rlt_checkfailed,
	addgem_rlt_noequip,
	addgem_rlt_cannotadd,
	addgem_rlt_gemwrong,
	addgem_rlt_levellimit,
	addgem_rlt_noholeitem,
	addgem_rlt_failedothers,
	addgem_rlt_gemplacewrong,
};

enum {
	feedgem_rlt_success,
	feedgem_rlt_failed,
	feedgem_rlt_checkmoney,
	feedgem_rlt_checkfailed,
	feedgem_rlt_noequip,
	feedgem_rlt_cannotadd,
	feedgem_rlt_gemwrong,
	feedgem_rlt_levellimit,
	feedgem_rlt_noholeitem,
	feedgem_rlt_failedothers,
	feedgem_rlt_gemplacewrong,
	feedgem_rlt_valuemax,
};

enum {
	remgem_rlt_success,
	remgem_rlt_failed,
	remgem_rlt_checkmoney,
	remgem_rlt_checkfailed,
	remgem_rlt_noequip,
	remgem_rlt_cannotrem,
	remgem_rlt_notool,
	remgem_rlt_levellimit,
	remgem_rlt_failedothers,
};

enum {
	materialupgrade_rlt_success,
	materialupgrade_rlt_failed,
	materialupgrade_rlt_checkfailed,
	materialupgrade_rlt_checkmoney,
	materialupgrade_rlt_levellimit,
	materialupgrade_rlt_failedothers,
};

struct GEMUPGRADE_GEM_UPGRADE_OPEN_Data {
};

struct GEMUPGRADE_GEM_UPGRADE_CLOSE_Data {
};

struct GEMUPGRADE_GEM_UPGRADE_NOW_Data {
	DWORD itemIndex1;
	BYTE slot;
};

struct GEMUPGRADE_GEM_UPGRADE_OK_Data {
	DWORD	itemIndex;		
};

struct GEMUPGRADE_GEM_UPGRADE_RESULT_Data {	
	DWORD	itemIndex;	
	BYTE	success;
};

enum {
	GEM_SPLITE_OPEN,
	GEM_SPLITE_CLOSE,
	GEM_SPLITE_NOW,
	GEM_SPLITE_OK,
	GEM_SPLITE_RESULT,
};

struct GEMSPLITE_GEM_SPLITE_OPEN_Data {
};

struct GEMSPLITE_GEM_SPLITE_CLOSE_Data {
};

struct GEMSPLITE_GEM_SPLITE_NOW_Data {
	DWORD itemIndex;
};

struct GEMSPLITE_GEM_SPLITE_OK_Data {
	DWORD	itemIndex;		
};

struct GEMSPLITE_GEM_SPLITE_RESULT_Data {	
	DWORD	itemIndex;	
	BYTE	success;		
};

enum {
	EQUIP_HOLE_OPEN,
	EQUIP_HOLE_CLOSE,
	EQUIP_HOLE_NOW,
	EQUIP_HOLE_OK,
	EQUIP_HOLE_RESULT,
	EQUIP_HOLE_FEED_GEM,
};

struct EQUIPHOLE_EQUIP_HOLE_OPEN_Data {
	BYTE flag;
};
struct EQUIPHOLE_EQUIP_HOLE_CLOSE_Data {
};
struct EQUIPHOLE_EQUIP_HOLE_NOW_Data {
	DWORD itemIndex;
	DWORD itemHoleIndex;
	BYTE	slot;
	bool  bBind;
};

struct EQUIPHOLE_EQUIP_HOLE_FEED_GEM_Data {
	DWORD itemIndex;
	DWORD itemGemIndex;
};
struct EQUIPHOLE_EQUIP_HOLE_OK_Data {
	DWORD	itemIndex;		
};
struct EQUIPHOLE_EQUIP_HOLE_RESULT_Data {	
	DWORD	itemIndex;	
	BYTE	success;		
};

enum {
	EQUIP_ADDGEM_OPEN,
	EQUIP_ADDGEM_CLOSE,
	EQUIP_ADDGEM_NOW,
	EQUIP_ADDGEM_OK,
	EQUIP_ADDGEM_RESULT,
	EQUIP_FEEDGEM_RESULT,
};
struct EQUIPADDGEM_EQUIP_ADDGEM_OPEN_Data {
};
struct EQUIPADDGEM_EQUIP_ADDGEM_CLOSE_Data {
};
struct EQUIPADDGEM_EQUIP_ADDGEM_NOW_Data {
	DWORD itemIndex;
	DWORD gemIndex;
	BYTE	Slot;
};
struct EQUIPADDGEM_EQUIP_ADDGEM_OK_Data {
	DWORD	itemIndex;		
};
struct EQUIPADDGEM_EQUIP_ADDGEM_RESULT_Data {	
	DWORD	itemIndex;	
	BYTE	success;		
};
struct EQUIPADDGEM_EQUIP_FEEDGEM_RESULT_Data {
	DWORD   itemIndex;
	int     nValue;
	BYTE    success;
};

enum {
	EQUIP_REMGEM_OPEN,
	EQUIP_REMGEM_CLOSE,
	EQUIP_REMGEM_NOW,
	EQUIP_REMGEM_OK,
	EQUIP_REMGEM_RESULT,
};
struct EQUIPREMOVEGEM_EQUIP_REMGEM_OPEN_Data {
};
struct EQUIPREMOVEGEM_EQUIP_REMGEM_CLOSE_Data {
};
struct EQUIPREMOVEGEM_EQUIP_REMGEM_NOW_Data {
	DWORD itemIndex;
	DWORD toolIndex;
	BYTE	slot;
};
struct EQUIPREMOVEGEM_EQUIP_REMGEM_OK_Data {
	DWORD	itemIndex;		
};
struct EQUIPREMOVEGEM_EQUIP_REMGEM_RESULT_Data {	
	DWORD	itemIndex;	
	BYTE	success;		
};

enum {
	MATERIAL_UPGRADE_OPEN,
	MATERIAL_UPGRADE_CLOSE,
	MATERIAL_UPGRADE_NOW,
	MATERIAL_UPGRADE_OK,
	MATERIAL_UPGRADE_RESULT,
	MAKE_MATERIAL_START,
};
struct MATERIALUPGRADE_MATERIAL_UPGRADE_OPEN_Data {
};
struct MATERIALUPGRADE_MATERIAL_UPGRADE_CLOSE_Data {
};
struct MATERIALUPGRADE_MATERIAL_UPGRADE_NOW_Data {
	BYTE bNum;
	DWORD itemIndex[4];
	BYTE  bCnt[4];
};
struct MATERIALUPGRADE_MATERIAL_UPGRADE_OK_Data {
	DWORD	itemIndex;		
};
// struct MATERIALUPGRADE_MATERIAL_UPGRADE_RESULT_Data {	
// 	DWORD	itemIndex;	
// 	BYTE	success;		
// };

struct MATERIALUPGRADE_MAKE_MATERIAL_START_Data
{
	WORD		wIndex;			//Ñ¡Ïî¿¨±àºÅ
	WORD		wItemID;
	BYTE		bUseBind;
};

struct MATERIALUPGRADE_MATERIAL_UPGRADE_RESULT_Data 
{	
	WORD		wItemID;	
	BYTE		bCnt;
	WORD		wID[1];
};
#pragma pack (pop)
#endif//_GEM_UPGRADE_SPLITE_PACKET_H_