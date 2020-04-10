#ifndef _ITEM_AUCTION_PACKET_H_
#define _ITEM_AUCTION_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#include "item_packet.h"
#include "item_levelup_packet.h"

enum 
{
	OPEN_MENU,			//�ͻ��˴򿪽����ʱ��֪ͨ������
	CLOSE_MENU,			//
	
	REQUEST_CALCEL,		//
	REQUEST_BUY,		//

	SEND_INFO,			//���´򿪽�����˷�		
	SEND_RESULT,		//ֻ�����˷�
	EMPTY_INFO,			//û��������Ʒ��

	OPEN_AUCTION_MENU,	//���������ͻ���
	HAVE_AUCTION,

	S_SA_REFRESH,
};


enum eAuctionResultType
{
	AUCTION_ADD,
	AUCTION_CANCEL,
	AUCTION_BUY,
};

enum eAuctionResultCode
{
	AUCTION_RESULT_SUCCESS, ///>���׳ɹ�
	AUCTION_RESULT_NOT_ENOUGH_MONEY, ///>��Ҳ���
	AUCTION_RESULT_NOT_ENOUGH_YUANBAO, ///>Ԫ������
	AUCTION_RESULT_LIST_FULL,///>�����б���
	AUCTION_RESULT_ITEM_NOT_EXISTS, ///>������Ʒ������
	AUCTION_RESULT_PRICE_TOO_LOW,
	AUCTION_RESULT_LEVEL_TOO_LOW,					//��δ��20��,��������
	AUCTION_RESULT_ONE_PERSON_ONE_ITEM,				//ÿ�����ֻ��һ����Ʒ
	AUCTION_RESULT_NOT_ENOUGH_LEVEL_TO_BUY_GOLD_ITEM,//��δ��30��,���ܽ��׽�Ԫ����Ʒ
	AUCTION_RESULT_ITEM_CAN_NOT_AUCTION,
	AUCTION_RESULT_TOO_MUCH_GOLD,			//����Ԫ������
	AUCTION_RESULT_BUYITNOW_TOO_LOW,		//һ�ڼ����ù���
	AUCTION_RESULT_SAFE_LOCK,				//��ȫ������
};

struct AucInfo 
{
	DWORD sellerUin;
	char seller[20];
	BYTE type;  ///>��������0:Ԫ��1:���
	int	price;
	int iBuyItNow;	//һ�ڼ�
	DWORD AccountUin;
};

struct AucItem 
{
	AucInfo info;
	CLIENTITEM item;
	bool bIsHaveExtend;
	bool bIsHaveBindInfo;
	bool bIsHaveExpExtend;
	bool bIsHaveJudge;
	bool bIsHaveGem;
	bool bIsHaveTrump;
	bool bIsHavePlusInfo;
	bool bIsHaveCarve;
	ItemExtend itemextend;
	ItemBindInfo bindInfo;	
	ItemExpExtend xExpExtend;
	ExtendData itemjudge;	
	TrumpSPData trumpInfo;
	ItemPlusUpInfo plusInfo;
	ItemCarveInfo xCarveInfo;
	int nGemsSize;
	BYTE pGemsData[1];	
};

struct ITEM_AUCTION_OPEN_MENU_Data
{	
	DWORD version;
};

struct ITEM_AUCTION_REQUEST_CALCEL_Data
{	
	DWORD itemindex;
};

struct ITEM_AUCTION_REQUEST_BUY_Data
{
	DWORD uin;
	DWORD itemindex;
	int	price;
};

struct ITEM_AUCTION_SEND_INFO_Data
{
	int	nCount;				//��ǰ�б�����
	int nCurrentPrice;		//��ǰ�۸�
	int nRemainTime;		//ʣ��ʱ��
	int nBuyItNow;
	char buyer[20];
	DWORD version;
    AucItem item;		//��ǰ������Ʒ
};

struct ITEM_AUCTION_SEND_RESULT_Data
{
    BYTE ResultType;
	BYTE ResultCode;
	int	data;
};

struct ITEM_AUCTION_EMPTY_INFO_Data
{
	DWORD version;
};

struct ITEM_AUCTION_HAVE_AUCTION_Data
{
	bool isHave;
};

struct ITEM_AUCTION_S_SA_REFRESH_Data
{
	DWORD	nGold;
	DWORD	dwIndex;
	DWORD	param;
	DWORD	tmRemain;
	char	szName[16];
};



#pragma pack (pop)
#endif