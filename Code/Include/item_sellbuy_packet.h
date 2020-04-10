#ifndef _ITEM_SELLBUY_PACKET_H_
#define _ITEM_SELLBUY_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#include "item_packet.h"
#include "cs_packet.h"
#include "item_levelup_packet.h"

#define SELL_RATE 50	//���۷���
#define SELL_SPACE 20	//���ۿռ�
#define BUY_SPACE 20	//�������ռ�
//enum
enum _ITEM_SELLBUY
{
	//	REQUEST_SELL_LIST,
	PLAYER_OPEN_MENU,
	PLAYER_CLOSE_MENU,
	SHOW_SELL_LIST,
	EMPTY_LIST,
	ADD_SELL_ITEM,
	CANCEL_SELL_ITEM,
	TRY_BUY_ITEM,
	GET_BUYED_ITEM,
	GET_SELLED_MONEY,
	//	NOTIFY_CLIENT_REFRESH,
	ITEM_ADD,
	ITEM_CANCEL,
	ITEM_SELLED,
	ITEM_REMOVE_SELLED_TO_S,
	ITEM_REMOVE_SELLED_TO_B,
	ADD_SELL_ITEM_FAIL,
	ADD_SELL_RESULT,
};

enum _FAILED_INFO
{
	ITEM_NOT_IN_LIST,
	ITEM_NOT_IN_DB,
	NOT_ENOUGH_MONEY,		
	NOT_ENOUGH_SELL_RATE,	//���۷��ò���
	NOT_ENOUGH_SELL_SPACE,	//���ۿռ䲻��
};

enum _SELL_ITEM_TYPE
{
	SELL_ITEM_ALL,
	SELL_ITEM_WEAPON,
	SELL_ITEM_HEAD,
	SELL_ITEM_BODY,
	SELL_ITEM_HAND,
	SELL_ITEM_TROUSERS,
	SELL_ITEM_SHOES,
	SELL_ITEM_NECK,
	SELL_ITEM_RING,
	SELL_ITEM_BANGLE,
	SELL_ITEM_WAISTBAND,
	SELL_ITEM_STONE,
	SELL_ITEM_OTHER,
	SELL_ITEM_FASHION,
	SELL_ITEM_MAX,
};



struct FatItemInfo 
{
	WORD				itemID;		        //ItemBaseID
	WORD				Dura;		            //�־�
	WORD				DuraMax;
	BYTE				type;				//�������ͣ���һ�Ԫ����Ԫ��0�����1
	WORD				wData;
	BYTE				bExtend;			//�Ƿ���ItemExtend	
	BYTE				bJudge;
	DWORD				m_wDeadLine; 
	
	DWORD				uinBuy;
	int					price;
	BYTE				state;
	time_t				beginTime;
	char				sellerName[20];
};

struct FatItem
{
	IX_ItemKey key;
	FatItemInfo info;
};

struct ITEMSELLBUY_PLAYER_OPEN_MENU_Data
{
	DWORD dwUin;
	short version;
};

struct ITEMSELLBUY_SHOW_SELL_LIST_Data
{
	BYTE	bEnd;
	BYTE	count;
	BYTE	bFirst;//�ǲ��ǵ�һ����
	short	version;
	BYTE	judgecount;
	FatItem data[1];
	ItemExtend extend[1];
};

struct ITEMSELLBUY_ITEM_ADD_Data
{
	FatItem item;
	ItemExtend extend;
};

struct ITEMSELLBUY_ITEM_CANCEL_Data
{
	IX_ItemKey key;
	BYTE sellType;
};

struct ITEMSELLBUY_ITEM_SELLED_Data
{
	IX_ItemKey key;
	BYTE sellType;
	DWORD uinBuy;
};

struct ITEMSELLBUY_ITEM_REMOVE_SELLED_TO_S_Data
{
	IX_ItemKey key;	
};

struct ITEMSELLBUY_ITEM_REMOVE_SELLED_TO_B_Data
{
	IX_ItemKey key;			
};

struct ITEMSELLBUY_ADD_SELL_ITEM_Data
{
	DWORD dwUin;
	DWORD itemIndex;
	int price;
	BYTE type;
};

struct ITEMSELLBUY_CANCEL_SELL_ITEM_Data
{	
	DWORD dwUin;
	DWORD itemIndex;
};

struct ITEMSELLBUY_TRY_BUY_ITEM_Data
{	
	IX_ItemKey key;
};


struct ITEMSELLBUY_GET_BUYED_ITEM_Data
{	
	IX_ItemKey key;
};

struct ITEMSELLBUY_GET_SELLED_MONEY_Data
{	
	IX_ItemKey key;
};

struct ITEMSELLBUY_REMOVE_SELLED_ITEM_Data
{	
	IX_ItemKey key;
};

struct ITEMSELLBUY_ITEM_BUY_FAIL_Data
{	
	IX_ItemKey key;
	BYTE	   bFailInfo;	
};

struct ITEMSELLBUY_ADD_SELL_RESULT_Data
{	
	DWORD itemIndex;
	int cause;//0 succeed else failed
};

#pragma pack (pop)

#endif