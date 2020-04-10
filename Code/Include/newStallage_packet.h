#ifndef _NEW_STALLAGE_PACKET_
#define _NEW_STALLAGE_PACKET_
#include "groupcmd.h"
#include <windows.h>
#include "item_packet.h"
#include "item_levelup_packet.h"
#include "slave_packet.h"
#pragma pack (push)
#pragma pack (1)


enum 
{
	OPEN_NEWSTALLAGE,                                 //�򿪰�̯����--��ʹ����Ʒ�������������˷����ͻ�����Ӧ��
	ADD_NEWSTALLAGEITEM,                              //��̯����������һ����Ʒ�����������˷����ͻ�����Ӧ��
	REMOVE_NEWSTALLAGEITEM,                           //��̯������ɾ��һ��ȥƷ

	//SET_NEWSTALLAGEITEM,                              //����̯����������һ����Ʒ�����ͻ��˷�������������Ӧ��
	//CANCEL_NEWSTALLAGEITEM,                           //ȡ��һ����Ʒ

	//SET_NEWSTALLAGEOK,                                //��̯��ʼ
	//SET_NEWSTALLAGEPRE,                               //��̯׼��
	CANCEL_NEWSTALLAGE,                               //��̯����

	BUY_NEWSTALLAGEITEM,                              //��ҹ����̯��Ʒ
	SEND_NEWSTALLAGEITEMS,                            //�����а�̯��Ʒ���͸��鿴���
	//OPEN_NEWSELLBUY,                                  //�鿴�������̯λ
	//SELL_NEWSTALLAGEITEM,                             //��Ʒ����ҹ���

	ENTER_NEWSTALLAGE,
	EXIT_NEWSTALLAGE,

	SEND_NEWEXTEND,
	//CLOSE_NEWSELLBUY,

	APPLY_NEWSTALLAGE,
	APPLY_NEWSTALLAGE_RLT,

	SEND_NEWJUDGE,
	SEND_NEWGEMS_INFO,
	STALLAGE_NEWTRUMP_INFO,
	REMOVE_NEWSTALLAGESLAVE,
	ADD_NEWSTALLAGESLAVE,
	CANCEL_NEWSTALLAGESLAVE,
	BUY_NEWSTALLAGESLAVE,
	SEND_NEWSTALLAGESLAVES, 
	SELL_NEWSTALLAGESLAVE,
	SEND_NEWCARVE,
	STALLAGE_SEND_NEWWING,
	OPEN_NEWSTALLAGE_SELL,
	ASK_NEWSTALLAGE_OPEN,
	SYNCNEWSTALLAGE,
	UPDATE_NEWSTALLAGE_ITEM,
	ADD_NEWSTALLAGEBUYITEM,
	REMOVE_NEWSTALLAGEBUYITEM,
	GET_NEWSTALLAGEBUYITEM,
	SELL_NEWSTALLAGEBUYITEM,     //����Ʒ
	UPDATE_NEWSTALLAGEBUYITEM,
	SEND_ADD_NEWSTALLAGEMESSAGE,
	REQUEST_ADD_MESSAGE,
};
enum
{
	NEWSTALLAGE_RLT_PLACE,
	NEWSTALLAGE_RLT_STATE,
	NEWSTALLAGE_RLT_OK,
	NEWSTALLAGE_RLT_MAX,
};

struct NEWSTALLAGEMENU_OPEN_NEWSTALLAGE_Data
{

};

struct NEWSTALLAGEMENU_OPEN_NEWSTALLAGE_SELL_Data
{
	bool bOpenFlag;
};

struct NEWSTALLAGEMENU_ASK_NEWSTALLAGE_OPEN_Data
{

};

struct NEWSTALLAGE_SYNCNEWSTALLAGE_Data
{
	DWORD     index;
	int       nType;   //�������� ��Ҫ���õ���Ʒ���;���
	int       nPosx;
	int       nPosy;
	char      szSays[48];
	char      szName[16];
};

struct NEWSTALLAGEMENU_UPDATE_NEWSTALLAGE_ITEM_Data
{
	DWORD      itemIndex;
	DWORD      dwUin;
	int		   nNum;
};
/*
struct  NEWSTALLAGEMENU_SET_NEWSTALLAGEITEM_Data 
{
	DWORD	itemIndex;
	int		cnt;
	int		price;	
};

struct  NEWSTALLAGEMENU_CANCEL_NEWSTALLAGEITEM_Data 
{
	DWORD	itemIndex;
};*/

struct  NEWSTALLAGEMENU_ADD_NEWSTALLAGEITEM_Data 
{
	DWORD	itemIndex;
	WORD	itemID;
	int		price;	
	int     num;
};

struct NEWSTALLAGEMENU_ADD_NEWSTALLAGEBUYITEM_Data
{
	DWORD itemID;
	int   price;
	int   num;
	int   norder;
};

struct NEWSTALLAGEMENU_UPDATE_NEWSTALLAGEBUYITEM_Data
{
	DWORD itemID;
	int   price;
	int   num;
	int   norder;
};

struct NEWSTALLAGEMENU_SEND_ADD_NEWSTALLAGEMESSAGE_Data
{
	DWORD dwTime;
	char szText[100];
};
//struct NEWSTALLAGEMENU_SET_NEWSTALLAGEOK_Data
//{
//	char	szInfo[35];
//};

struct  NEWSTALLAGEMENU_REMOVE_NEWSTALLAGEITEM_Data 
{
	DWORD	itemIndex;
	int     nNum;
	LONGLONG llkey;
	BOOLEAN self;
};

struct NEWSTALLAGEMENU_REMOVE_NEWSTALLAGEBUYITEM_Data
{
	int		nOrder;
	DWORD	dwItemID;
	BOOLEAN self;
};



struct NEWSTALLAGEMENU_BUY_NEWSTALLAGEITEM_Data
{
	DWORD	itemIndex;
	int		cnt;
	DWORD	uin;
};

struct NEWSTALLAGEMENU_GET_NEWSTALLAGEBUYITEM_Data
{
	LONGLONG llItemKey;
	DWORD    dwItemID;
	DWORD    dwFromUin;
};

struct NEWSTALLAGEMENU_SELL_NEWSTALLAGEBUYITEM_Data
{
	DWORD    dwItemID;
	DWORD    dwIndex;
	int      nNum;
	int      nOrder;
	DWORD    dwStallageUin;
};

struct NEWSTALLAGEMENU_REQUEST_ADD_MESSAGE_Data     //�ı乫��
{
	DWORD dwUin;
	char  szNote[1];
};

struct NEWSTALLAGEMENU_SEND_NEWSTALLAGEITEMS_Data 
{
	BYTE count;
	CLIENTITEMEX item[1];
};

//struct NEWSTALLAGEMENU_SELL_NEWSTALLAGEITEM_Data
//{
//	DWORD	itemIndex;
//};

struct NEWSTALLAGEMENU_ENTER_NEWSTALLAGE_Data
{
	DWORD	uin;
};

struct NEWSTALLAGEMENU_EXIT_NEWSTALLAGE_Data
{
	DWORD	uin;
};



struct NEWSTALLAGEMENU_SEND_NEWEXTEND_Data 
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemExtend	extend;
};

struct NEWSTALLAGEMENU_APPLY_NEWSTALLAGE_RLT_Data
{
    BYTE  Rlt;
	BYTE  bSkin;
};
struct NEWSTALLAGEMENU_SEND_NEWJUDGE_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ExtendData	xExtend;
};

struct NEWSTALLAGEMENU_SEND_NEWGEMS_INFO_Data
{
	DWORD		itemIndex;
	BYTE		slot;

	int			nSize;
	BYTE		pbData[1];
};
struct NEWSTALLAGEMENU_STALLAGE_NEWTRUMP_INFO_Data
{
	DWORD		itemIndex;
	BYTE		slot;

	TrumpSPData		tSPData;
};
struct  NEWSTALLAGEMENU_REMOVE_NEWSTALLAGESLAVE_Data 
{
	DWORD	itemIndex;
	BOOLEAN self;
};

struct  NEWSTALLAGEMENU_ADD_NEWSTALLAGESLAVE_Data 
{
	DWORD	itemIndex;
	WORD	itemID;
	int		price;	
};

struct  NEWSTALLAGEMENU_CANCEL_NEWSTALLAGESLAVE_Data 
{
	DWORD	itemIndex;
};

struct NEWSTALLAGEMENU_BUY_NEWSTALLAGESLAVE_Data
{
	DWORD	itemIndex;
	int		cnt;
	DWORD	uin;
};

/*struct SlaveAddData;
{
	BYTE bSlot;
	SlaveDataEx data;
	int	skin;
	bool bCapture;
	int price;
};*/

struct NEWSTALLAGEMENU_SEND_NEWSTALLAGESLAVES_Data
{
	BYTE count;
	//SlaveAddData data[1];
};

struct NEWSTALLAGEMENU_SELL_NEWSTALLAGESLAVE_Data
{
	DWORD	itemIndex;
};

struct NEWSTALLAGEMENU_STALLAGE_SEND_NEWWING_Data
{
	DWORD	dwIndex;
	DWORD	dwTypeBit;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//Ʒ��
	int		nRepairTime;	//�޸�����
	DWORD	dwQuaVal[4];	//4������

};

struct NEWSTALLAGEMENU_SEND_NEWCARVE_Data
{
	DWORD		itemIndex;
	BYTE		slot;
	ItemCarveInfo xInfo;
};

#pragma pack (pop)
#endif//_NEW_STALLAGE_PACKET_