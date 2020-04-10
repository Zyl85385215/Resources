#ifndef _CHUNK_PACKET_HDR
#define _CHUNK_PACKET_HDR
#pragma pack (push)
#pragma pack (1)

enum{
	CHUNK_STATUS_DESTROY,
	CHUNK_STATUS_DESTROY_ATTACK,
	CHUNK_STATUS_DAMAGED,
	CHUNK_STATUS_DAMAGED_ATTACK,
	CHUNK_STATUS_BUILD,
	CHUNK_STATUS_BUILD_ATTACK,
	CHUNK_STATUS_NORMAL,
	CHUNK_STATUS_NORMAL_ATTACK,
};
/*
enum
{
	CHUNK_TYPE_NORMAL,         //普通建筑
	CHUNK_TYPE_RECOVERY,       //防御性建筑
	CHUNK_TYPE_SIGN,            //标志性建筑
	CHUNK_TYPE_CENTER          //防御中心
};
*/
enum
{
	CHUNK_TYPE_NORMAL,			    //商业
	CHUNK_TYPE_RECOVERY,			//国防系统
	CHUNK_TYPE_SIGN,			    //所有权标志物
	CHUNK_TYPE_TEMPVOUCHER,		//临时标志物(攻防大营)
	CHUNK_TYPE_CENTER,			//科研中心
};

enum {
	SYNCCHUNK,
	CHUNK_STATUS,
	CHUNK_DISAP,
	CHUNK_HP,
	OPEN_STORAGE,
	CHUNK_ADDMONEY,
	CHUNK_ADDITEM,
	CHUNK_ITEM,
	CHUNK_MONEY,
	CHUNK_INFO,
	CHUNK_OWNER,
	CHUNK_PRICE,
	CHUNK_BUY,
	CHUNK_GAIN,
	CHUNK_GETGAIN
};

struct CHUNK_SYNCCHUNK_Data {
	WORD	index;
	WORD	type;
	WORD	x, y;
	BYTE    status;
	int     hp;
	int     maxHp;
	DWORD   confID;
	BYTE  isAttack;
};

struct CHUNK_CHUNK_STATUS_Data {
	WORD	index;
	BYTE    status;
};

struct CHUNK_CHUNK_HP_Data {
	WORD	index;
	int hp;
};

struct CHUNK_CHUNK_DISAP_Data {
	WORD	index;
};

struct CHUNK_CHUNK_ADDMONEY_Data {
	int count;
};

struct CHUNK_CHUNK_ADDITEM_Data {
	BYTE	index;
	int count;
};

struct CHUNK_CHUNK_ITEM_Data {
	WORD	itemID;
	int count;
};

struct CHUNK_CHUNK_MONEY_Data {
	int count;
};

struct CHUNK_CHUNK_INFO_Data {
	BYTE type;
	BYTE status;
	DWORD hp;
	DWORD maxhp;
	DWORD level;
	DWORD levelPoint;
	DWORD maxPoint;
	DWORD def;
	
};

struct CHUNK_CHUNK_OWNER_Data{
	DWORD uin;
	char name[16];
};

struct CHUNK_CHUNK_PRICE_Data{
	DWORD price;
};

struct CHUNK_CHUNK_GAIN_Data{
	DWORD money;
};

#pragma pack (pop)
#endif