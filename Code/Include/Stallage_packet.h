#ifndef _STALLAGE_PACKET_
#define _STALLAGE_PACKET_
#include "groupcmd.h"
#include <windows.h>
#include "item_packet.h"
#include "item_levelup_packet.h"
#include "slave_packet.h"
#pragma pack (push)
#pragma pack (1)


enum 
{
	OPEN_STALLAGE,                                 //打开摆摊界面--由使用物品触发（服务器端发，客户端响应）
	ADD_STALLAGEITEM,                              //摆摊界面上增加一件物品，（服务器端发，客户端响应）
	REMOVE_STALLAGEITEM,                           //摆摊界面上删除一件去品

	SET_STALLAGEITEM,                              //往摆摊界面上增加一件物品，（客户端发，服务器端响应）
	CANCEL_STALLAGEITEM,                           //取消一个物品

	SET_STALLAGEOK,                                //摆摊开始
	SET_STALLAGEPRE,                               //摆摊准备
	CANCEL_STALLAGE,                               //摆摊结束

	BUY_STALLAGEITEM,                              //玩家购买摆摊物品
	SEND_STALLAGEITEMS,                            //将所有摆摊物品发送给查看玩家
	OPEN_SELLBUY,                                  //查看其他玩家摊位
	SELL_STALLAGEITEM,                             //物品被玩家购买

	ENTER_STALLAGE,
	EXIT_STALLAGE,

	SEND_EXTEND,
	CLOSE_SELLBUY,

	APPLY_STALLAGE,
	APPLY_STALLAGE_RLT,

	SEND_JUDGE,
	SEND_GEMS_INFO,
	STALLAGE_TRUMP_INFO,
	REMOVE_STALLAGESLAVE,
	ADD_STALLAGESLAVE,
	CANCEL_STALLAGESLAVE,
	BUY_STALLAGESLAVE,
	SEND_STALLAGESLAVES, 
	SELL_STALLAGESLAVE,
	SEND_CARVE,
	STALLAGE_SEND_WING,
	SET_NEWSTALLAGEITEM,
	CANCEL_NEWSTALLAGEITEM,
	SET_NEWSTALLAGEPRE,
	SET_NEWSTALLAGEOK,
	OPEN_NEWSELLBUY,
	CLOSE_NEWSELLBUY,
	SEND_NEWSTALLAGEITEMSEX,
	SELL_NEWSTALLAGEITEM,
	STALLAGE_SEND_NEWWINGEX,
	SEND_NEWEXTENDEX,
	SEND_NEWCARVEEX,
	SEND_NEWJUDGEEX,
	SEND_NEWGEMS_INFOEX,
	STALLAGE_NEWTRUMP_INFOEX,
	STALLAGE_NEWUPDATEITEM,
	SEND_HORSEEQ,
	SET_NEWSTALLAGESHOUTAN,
	SEND_SEENEWSTALLAGE,
	//BUY_NEWSTALLAGEITEM,
	SELL_UPDATESTALLAGEITEM,                             //物品被玩家购买
	UPDATE_STALLAGEITEM,
	SET_NEWSTALLAGEBUYITEM,
	CANCEL_NEWSTALLAGEBUYITEM,
	SEND_NEWSTALLAGEBUYITEMSEX,
	STALLAGE_NEWUPDATEBUYITEM,
	SEND_NEWSTALLAGEITEMSEX2,
	SEND_NEWEXTENDEX2,
	SEND_NEWJUDGEEX2,
	SEND_NEWGEMS_INFOEX2,
	STALLAGE_NEWTRUMP_INFOEX2,
	REMOVE_NEWSTALLAGE_BOUGHTITEM,
	GETNEWSTALLAGEINFO,
	STALLAGE_SEND_WINGEX,
	SEND_CARVEEX,
	SEND_HORSEEQEX,
	SEND_NEWSTALLAGE_MESSAGE,
	SEND_REPUTE,
	SEND_ENCHASE,
};
enum
{
	STALLAGE_RLT_PLACE,
	STALLAGE_RLT_STATE,
	STALLAGE_RLT_OK,
	STALLAGE_RLT_MAX,
};

struct  STALLAGEMENU_SET_STALLAGEITEM_Data 
{
	DWORD	itemIndex;
	int		cnt;
	int		price;	
};

struct STALLAGEMENU_SET_NEWSTALLAGEITEM_Data
{
	DWORD  itemIndex;
	int    cnt;
	int    price;
};

struct STALLAGEMENU_SET_NEWSTALLAGEBUYITEM_Data
{
	DWORD dwItemID;
	int   cnt;
	int   price;
};
struct  STALLAGEMENU_CANCEL_STALLAGEITEM_Data 
{
	DWORD	itemIndex;
};

struct STALLAGEMENU_CANCEL_NEWSTALLAGEITEM_Data
{
	DWORD  itemIndex;
};

struct STALLAGEMENU_CANCEL_NEWSTALLAGEBUYITEM_Data
{
	DWORD dwItemID;
	int   nOrder;
};

struct STALLAGEMENU_SET_NEWSTALLAGEPRE_Data
{

};

struct STALLAGEMENU_SET_NEWSTALLAGESHOUTAN_Data
{

};


struct STALLAGEMENU_SEND_SEENEWSTALLAGE_Data
{
	DWORD dwObjUin;
};

struct STALLAGEMENU_SET_NEWSTALLAGEOK_Data
{
	char	szInfo[35];
};


struct  STALLAGEMENU_ADD_STALLAGEITEM_Data 
{
	DWORD	itemIndex;
	WORD	itemID;
	int		price;	
};

struct STALLAGEMENU_SET_STALLAGEOK_Data
{
	char	szInfo[35];
};

struct  STALLAGEMENU_REMOVE_STALLAGEITEM_Data 
{
	DWORD	itemIndex;
	BOOLEAN self;
};

struct STALLAGEMENU_UPDATE_STALLAGEITEM_Data
{
	DWORD   itemIndex;
	DWORD   dwData;
	BOOLEAN self;
};


struct STALLAGE_BUY_STALLAGEITEM_Data
{
	DWORD	itemIndex;
	int		cnt;
	DWORD	uin;
};

//struct STALLAGE_BUY_NEWSTALLAGEITEM_Data
//{
//	DWORD   itemIndex;
//	int     cnt;
//	DWORD   uin;
//};

struct STALLAGE_SEND_STALLAGEITEMS_Data 
{
	BYTE count;
	CLIENTITEMEX item[1];
};

struct STALLAGE_SEND_NEWSTALLAGEITEMSEX_Data
{
	BYTE count;
	CLIENTITEMEX item[1];
};

struct STALLAGE_SEND_NEWSTALLAGEITEMSEX2_Data
{
	BYTE count;
	CLIENTITEMEX2 item[1];
};
/*
struct STALLAGE_STALLAGE_NEWTRUMP_INFOEX_Data
{
	BYTE count;
	CLIENTITEMEX item[1];
};

struct STALLAGE_STALLAGE_NEWTRUMP_INFOEX2_Data
{
	BYTE count;
	CLIENTITEMEX item[1];
};*/
struct NewStallageBuyItem
{
	DWORD   dwItemID;
	int     nPrice;
	int     nNum;
	int     nOrder;
	int     nBoughtNum;
};
struct STALLAGE_SEND_NEWSTALLAGEBUYITEMSEX_Data
{
	BYTE count;
	NewStallageBuyItem item[1];
};

struct STALLAGE_SELL_STALLAGEITEM_Data
{
	DWORD	itemIndex;
};

struct STALLAGE_SELL_UPDATESTALLAGEITEM_Data
{
	DWORD  itemIndex;
	DWORD  dwData;
};

struct STALLAGE_SELL_NEWSTALLAGEITEM_Data
{
	DWORD   itemIndex;
};

struct STALLAGE_STALLAGE_SEND_NEWWINGEX_Data
{
	DWORD	dwIndex;
	DWORD	dwTypeBit;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//品质
	int		nRepairTime;	//修复次数
	DWORD	dwQuaVal[4];	//4项属性

};

struct STALLAGE_SEND_NEWEXTENDEX_Data 
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemExtend	extend;
};

struct STALLAGE_SEND_NEWEXTENDEX2_Data 
{
	LONGLONG	llItemKey;
	BYTE		slot;
	ItemExtend	extend;
};


struct STALLAGE_SEND_NEWCARVEEX_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemCarveInfo xInfo;
};
struct STALLAGE_SEND_NEWJUDGEEX_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ExtendData	xExtend;
};

struct STALLAGE_SEND_NEWJUDGEEX2_Data
{
	LONGLONG	llItemKey;
	BYTE		slot;
	ExtendData	xExtend;
};

struct STALLAGE_SEND_NEWGEMS_INFOEX_Data
{
	DWORD		itemIndex;
	BYTE		slot;

	int			nSize;
	BYTE		pbData[1];
};

struct STALLAGE_SEND_NEWGEMS_INFOEX2_Data
{
	LONGLONG	llItemKey;
	BYTE		slot;

	int			nSize;
	BYTE		pbData[1];
};
struct STALLAGE_STALLAGE_NEWTRUMP_INFOEX_Data
{
	DWORD		itemIndex;
	BYTE		slot;

	TrumpSPData		tSPData;
};

struct STALLAGE_STALLAGE_NEWTRUMP_INFOEX2_Data
{
	LONGLONG	llItemKey;
	BYTE		slot;

	TrumpSPData		tSPData;
};

struct STALLAGE_REMOVE_NEWSTALLAGE_BOUGHTITEM_Data
{
	DWORD     dwItemID;
	LONGLONG  llkey;
	DWORD     dwUin;
};

struct STALLAGEMENU_GETNEWSTALLAGEINFO_Data
{
	DWORD     dwObjUin;
};

struct STALLAGEMENU_SEND_NEWSTALLAGE_MESSAGE_Data
{
};

struct STALLAGE_STALLAGE_NEWUPDATEITEM_Data
{
	DWORD      itemIndex;
	DWORD      dwUin;
	int		   nNum;
	BYTE       bType;
};

struct STALLAGE_STALLAGE_NEWUPDATEBUYITEM_Data
{
	DWORD     dwItemID;
	int       nOrder;
	int       nNum;
	DWORD     dwUin;
};

struct STALLAGE_ENTER_STALLAGE_Data
{
	DWORD	uin;
};

struct STALLAGE_EXIT_STALLAGE_Data
{
	DWORD	uin;
};

struct STALLAGE_OPEN_SELLBUY_Data
{
	DWORD	uin;
};

struct STALLAGE_OPEN_NEWSELLBUY_Data
{
	DWORD	uin;
};

struct STALLAGE_SEND_EXTEND_Data 
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemExtend	extend;
};

struct STALLAGEMENU_APPLY_STALLAGE_RLT_Data
{
    BYTE  Rlt;
	BYTE  bSkin;
};
struct STALLAGE_SEND_JUDGE_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ExtendData	xExtend;
};

struct STALLAGE_SEND_GEMS_INFO_Data
{
	DWORD		itemIndex;
	BYTE		slot;

	int			nSize;
	BYTE		pbData[1];
};
struct STALLAGE_STALLAGE_TRUMP_INFO_Data
{
	DWORD		itemIndex;
	BYTE		slot;

	TrumpSPData		tSPData;
};
struct  STALLAGEMENU_REMOVE_STALLAGESLAVE_Data 
{
	DWORD	itemIndex;
	BOOLEAN self;
};

struct  STALLAGEMENU_ADD_STALLAGESLAVE_Data 
{
	DWORD	itemIndex;
	WORD	itemID;
	int		price;	
};

struct  STALLAGEMENU_CANCEL_STALLAGESLAVE_Data 
{
	DWORD	itemIndex;
};

struct STALLAGE_BUY_STALLAGESLAVE_Data
{
	DWORD	itemIndex;
	int		cnt;
	DWORD	uin;
};

struct SlaveAddData
{
	BYTE bSlot;
	SlaveDataEx data;
	int	skin;
	bool bCapture;
	int price;
};

struct STALLAGE_SEND_STALLAGESLAVES_Data
{
	BYTE count;
	SlaveAddData data[1];
};

struct STALLAGE_SELL_STALLAGESLAVE_Data
{
	DWORD	itemIndex;
};

struct STALLAGE_STALLAGE_SEND_WING_Data
{
	DWORD	dwIndex;
	DWORD	dwTypeBit;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//品质
	int		nRepairTime;	//修复次数
	DWORD	dwQuaVal[4];	//4项属性

};

struct STALLAGE_STALLAGE_SEND_WINGEX_Data
{
	DWORD	dwIndex;
	DWORD	dwTypeBit;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//品质
	int		nRepairTime;	//修复次数
	DWORD	dwQuaVal[4];	//4项属性

};

struct STALLAGE_SEND_CARVE_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemCarveInfo xInfo;
};

struct STALLAGE_SEND_REPUTE_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemReputeInfo xInfo;
};

struct STALLAGE_SEND_CARVEEX_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemCarveInfo xInfo;
};
struct STALLAGE_SEND_HORSEEQ_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	HorseEqInfo xInfo;
};

struct STALLAGE_SEND_ENCHASE_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	EnchaseExInfo xInfo;
};
struct STALLAGE_SEND_HORSEEQEX_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	HorseEqInfo xInfo;
};

#pragma pack (pop)
#endif