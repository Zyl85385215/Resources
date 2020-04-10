#ifndef _NEWAUCTION_PACKET_H_
#define _NEWAUCTION_PACKET_H_

#include "NAuction_Data.h"
#pragma pack (push)
#pragma pack (1)

enum
{
//----------------���ķ�������----------
	NA_ADD_ITEM,			//������Ʒ
	NA_TBUG_ITEM,			//���Գ���
	NA_REQUEST_LIST,		//�����б�
	NA_REQUEST_ITEMS,		//���ݱ��������Ʒ��ϸ��Ϣ
	NA_REQUEST_OWNER,
	NA_PICKOUT_ITEM,

//----------------���ķ�������----------
	NA_SEND_LIST,			//������Ʒ����Ϣ
	NA_SEND_ITEMS,			//������ϸ��Ϣ
	NA_UPDATA_ITEM,
	NA_DEL_ITEM,

//----------------����Ϸ�������Ľ���------------
	NA_GS_TBUG_ITEM,
	NA_GS_GIVE_ITEM,

	NA_SEND_MSG2ALL,

	NA_ADD_RANK_EQUIP,
	NA_RANK_EQUIP_INFO,
	NA_REQUEST_REQUIPINFO,
	NA_OPEN_GOLDAUCTION,
	NA_CLOSE_GOLDAUCTION,
	NA_REQUEST_GOLDLIST,
	NA_GS_TBUG_GOLD_ITEM,
	NA_TBUG_GOLDITEM,
	NA_GET_GOLDITEM_NUM,
	NA_FLESH_ITEM_NUM,
	//NA_SEND_HOME_BANK_ITEMS,
	//NA_SEND_HOME_SIMPLE_INFO,
};

//ֱ�������а��DATA������

class NA_ItemInfo
{
public:
	char	szSeller[16];	//������
	char	szBuyer[16];		//��ǰ����߼۸���
	int	   nPriceCur;	//��ǰ��
	int	   nPriceMax;	//һ�ڼ�
	DWORD  dwKey;		//ʶ����
	DWORD  dwTmRemain;		//ʣ������,����Ϊ0��ʾ������
	BYTE   byLvl;		//��Ʒ�ȼ�
	BYTE   byGoldType;
	WORD   wItemID;	//
	BYTE   byFlag;
	WORD   wData;   //����
};

struct	NAITEM_NA_GS_TBUG_ITEM_Data
{
	DWORD	dwUin;
	int		nMoney;
	DWORD	dwKey;
	DWORD   dwItemNum;
};

struct  NAITEM_NA_GS_TBUG_GOLD_ITEM_Data
{
	DWORD	dwMark;
	DWORD   dwUin;
	int     nGold;
	DWORD   dwKey;
	DWORD   dwItemNum;
};

struct	NAITEM_NA_GS_GIVE_ITEM_Data
{
	DWORD	dwToUin;
	DWORD	dwFromUin;
	LONGLONG	llKey;
	BYTE	data[1];
};

struct	NAITEM_NA_SEND_MSG2ALL_Data
{
	char	data[1];
};

struct	NAITEM_NA_PICKOUT_ITEM_Data
{
	//DWORD	dwUin;
	DWORD	dwKey;
};

struct  NAITEM_NA_ADD_ITEM_Data
{
	//DWORD  dwSeller;	//������
	int	   nPriceCur;	//��ǰ��
	int	   nPriceMax;	//һ�ڼ�
	LONGLONG	   llKey;		//��ƷINDEX
	BYTE   byType;		//��Ʒ����
	BYTE   byLvl;		//��Ʒ�ȼ�
	BYTE   byGoldType;	//0���,1Ԫ��
	char   szSearch[32];	//�����ؼ���,Ĭ��Ϊ��Ʒ����
	BYTE   pData[1];
};

struct  NAITEM_NA_ADD_RANK_EQUIP_Data
{
	//DWORD	dwUin;	//������
	DWORD	dwPoint;
	LONGLONG llItemKey;
	BYTE	bRankType;
	BYTE	pData[1];
};


struct  NAITEM_NA_RANK_EQUIP_INFO_Data
{
	DWORD	dwIndex;
	//BYTE	byType;
	//BYTE	byCountry;
	BYTE	pInfo[1];
};

struct	NAITEM_NA_REQUEST_REQUIPINFO_Data
{
	//DWORD	dwUin;
	BYTE	byCnt;
	DWORD	dwIndex[1];
};

struct  NAITEM_NA_OPEN_GOLDAUCTION_Data
{

};

struct  NAITEM_NA_CLOSE_GOLDAUCTION_Data
{

};

struct  NAITEM_NA_TBUG_ITEM_Data
{
	//DWORD dwUin;
	DWORD dwKey;
	int	  nMoney;
	DWORD dwItemNum;
};

struct  NAITEM_NA_TBUG_GOLDITEM_Data
{
	DWORD dwKey;
	int  nGold;
	DWORD dwNum;
};

struct  NAITEM_NA_GET_GOLDITEM_NUM_Data
{
	DWORD dwUin;
};

struct NAITEM_NA_FLESH_ITEM_NUM_Data
{
	int num;
};

struct  NAITEM_NA_REQUEST_LIST_Data
{
	//DWORD dwUin;
	NA_SearchKey cNASKey;
};

struct NAITEM_NA_REQUEST_OWNER_Data
{
	//DWORD dwUin;
	BYTE bGoldType;
};

struct NAITEM_NA_REQUEST_GOLDLIST_Data
{
	BYTE flag;
};
struct	NAITEM_NA_REQUEST_ITEMS_Data
{
	//DWORD	dwUin;
	BYTE    bGoldType;
	WORD	nCnt;
	DWORD	szKey[1];
};

struct	NAITEM_NA_SEND_LIST_Data
{
	BYTE		byPart;	//0���ֵ�ʱ��ͻ������,֮��Ĳ���,�Ӻ���
	WORD		nCnt;
	NA_ItemInfo	szNaItems[1];
};

struct	NAITEM_NA_UPDATA_ITEM_Data
{
	NA_ItemInfo	szNaItem;
};

struct	NAITEM_NA_SEND_ITEMS_Data
{
	BYTE	itemList[1];
};

struct NAITEM_NA_DEL_ITEM_Data
{
	DWORD dwKey;
};

#pragma pack (pop)

#endif