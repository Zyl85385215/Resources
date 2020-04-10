#ifndef _ACPOINT_BILL_PACKET_H_
#define _ACPOINT_BILL_PACKET_H_

#include "cs_packet.h"
#include "AccountPointDefine.h"

#pragma pack (push)
#pragma pack (1)

//////////////////////////////////////////////////////////////////////////

#define BILL_NAME_LEN 20

enum
{
	//	����Ʊ�ݣ�1>�̳���Ʒ 2>������Ʒ 3>������Ʒ
	ACPOINT_MALL_BILL,
	ACPOINT_SELLBUY_BILL,
	ACPOINT_AUCTION_BILL,
	ACPOINT_REVIVE_COST,
	ACPOINT_ITEM_TRANSFER_BILL,
	ACPOINT_TRADE,
};

enum MALL_BILL_STATUS
{
	MALL_BILL_STATUS_NULL,
	MALL_BILL_STATUS_REQUEST,
	MALL_BILL_STATUS_AFFIRM,
	MALL_BILL_STATUS_CANCEL,
	MALL_BILL_STATUS_MAX,
};

struct ACPOINT_BILL_ACPOINT_MALL_BILL_Data
{
	DWORD dwUin;				//	uindex
	DWORD dwAccountUin;			//	account uindex
	char  szName[BILL_NAME_LEN];//	name

	DWORD dwTargetUin;			//	target uindex
	DWORD dwItemID;				//	item id
	DWORD dwItemCount;			//	item count
	DWORD dwGold[GOLD_TYPE_MAX];//	change gold
	DWORD dwSilver;				//	change silver
	BYTE  bStatus;				//	bill status
};

struct ACPOINT_BILL_ACPOINT_TRADE_Data
{
	DWORD dwIndex;
	DWORD dwUin;				//	uindex
	DWORD dwAccountUin;			//	account uindex
	DWORD dwObjUin;				//	uindex
	DWORD dwObjAccountUin;			//	account uindex

	DWORD dwGold;//	change gold
	DWORD dwObjGold;//	change gold

	BYTE  bStatus;				//	bill status
};


enum SELLBUY_BILL_STATUS
{
	SELLBUY_BILL_STATUS_NULL,
	SELLBUY_BILL_STATUS_PAY,
	SELLBUY_BILL_STATUS_PAY_FAILED,
	SELLBUY_BILL_STATUS_PAY_SUECCESSFUL,
	SELLBUY_BILL_STATUS_GET_PAY,
	SELLBUY_BILL_STATUS_GET_PAY_FAILED,
	SELLBUY_BILL_STATUS_GET_PAY_SUECCESSFUL,
	SELLBUY_BILL_STATUS_MAX,
};

struct ACPOINT_BILL_ACPOINT_SELLBUY_BILL_Data
{
	DWORD dwUin;				//	uindex
	DWORD dwAccountUin;			//	account uindex
	char  szName[BILL_NAME_LEN];//	name

	DWORD dwSellerUin;			//	seller uin
	DWORD dwSellerAccountUin;	//	seller account uindex
	DWORD dwItemID;				//	item id
	IX_ItemKey ixItemKey;		//	item key
	DWORD dwGold;				//	change gold
	DWORD dwSilver;				//	change silver

	BYTE  bStatus;				//	bill status
};

enum AUCTION_BILL_STATUS
{
	AUCTION_BILL_STATUS_NULL,
	AUCTION_BILL_STATUS_REQUEST,
	AUCTION_BILL_STATUS_AFFIRM,
	AUCTION_BILL_STATUS_CANCEL,
	AUCTION_BILL_STATUS_MAX,
};

struct ACPOINT_BILL_ACPOINT_AUCTION_BILL_Data
{
	DWORD dwUin;				//	uindex
	DWORD dwAccountUin;			//	account uindex
	char  szName[BILL_NAME_LEN];//	name

	BYTE  bType;				//  0>Ԫ������ 1>����Ԫ��
	DWORD itemindex;			//  ���������Ԫ��,��������Ч
	DWORD dwSellUin;			//	ͬ��
	DWORD price;				//	�����Ԫ������,��������Ч
	DWORD BuyItNow;				//	ͬ��
	DWORD dwGold;				//	change gold

	BYTE  bStatus;				//	bill status
	char  szData[1];
};

struct ACPOINT_BILL_ACPOINT_REVIVE_COST_Data
{
	DWORD dwUin;				//	uindex
	DWORD dwAccountUin;			//	account uindex
	DWORD dwGold;				//	change gold
	BYTE  bStatus;				//	bill status
	BYTE  bType;
	char  szData[1];
};

struct ACPOINT_BILL_ACPOINT_ITEM_TRANSFER_BILL_Data
{
	DWORD dwUin;
	DWORD dwAccountUin;
	DWORD dwGold;
	BYTE  bStatus;				//	bill status
	BYTE  bType;
	char  szData[1];
};

#pragma pack (pop)

#endif