#pragma once

#include <windows.h>
#include "itemmarket_define.h"

#pragma pack(push)
#pragma pack(1)

enum EITEMMARKET_PACKET_CMD
{
	ITEMMARKET_PACKET_CMD_CLIENT_ENTER,					//	���뽻������
	ITEMMARKET_PACKET_CMD_CLIENT_LEAVE,					//	�뿪��������

	ITEMMARKET_PACKET_CMD_CLIENT_ITEM_INFO_LIST,		//	�ͻ��˽��ս������ĵ�������Ʒ��Ϣ
	ITEMMARKET_PACKET_CMD_CLIENT_ITEM_INFO,				//	�ͻ��˽��ս������ĵ�ĳ��Ʒ��Ϣ

	ITEMMARKET_PACKET_CMD_CLIENT_REQUEST_SELL,			//	�ͻ������������Ʒ
	ITEMMARKET_PACKET_CMD_CLIENT_REQUEST_BUY,			//	�ͻ������빺����Ʒ
	ITEMMARKET_PACKET_CMD_CLIENT_RESPONCE_SELL,			//	�ظ��ͻ�����Ʒ��������
	ITEMMARKET_PACKET_CMD_CLIENT_RESPONCE_BUY,			//	�ظ��ͻ�����Ʒ��������

	ITEMMARKET_PACKET_CMD_CENTERSERVER_REQUEST_SELL,	//	���ķ���������ȷ�ϳ���
	ITEMMARKET_PACKET_CMD_CENTERSERVER_RESPONCE_SELL,	//	���ķ������յ�ȷ�ϳ���
	ITEMMARKET_PACKET_CMD_CENTERSERVER_ACQUIRE_ITEM,	//	���ķ����������ɫ�����Ʒ

	ITEMMARKET_PACKET_CMD_CENTERSERVER_REQUEST_BUY, // ���ķ�����Ҫ������ add by zp
	ITEMMARKET_PACKET_CMD_RESPONSE_CENTERSERVER_REQUEST_BUY, // ��Ӧ���ķ������������Ϣ,�۵���Ԫ���Ƿ�ɹ�

	ITEMMARKET_PACKET_CMD_GM_MODIFY_BUYPRICE,             // GM�޸Ĺ���۸�
	ITEMMARKET_PACKET_CMD_RESPONCE_GM_MODIFY,                // ��ӦGM�޸ļ۸�

	ITEMMARKET_PACKET_CMD_GM_MODIFY_BUYPRICEUPER,          // GM�޸����Ǽ۸�
	ITEMMARKET_PACKET_CMD_GM_MODIFY_BUYLEVEL,              // GM�޸Ĺ���ȼ�

	ITEMMARKET_PACKET_CMD_GM_MODIFY_DAY_LIMITCOUNT,        // GM�޸�ÿ�������
};

struct marketItemPacketInfo
{
	DWORD itemID;
	DWORD sellPrice;
	DWORD buyPrice;
	float priceFluctuate;

	DWORD dwSystemCountPerDay;   // ϵͳÿ�칩Ӧ��
	int dwSellCountPerDay;     // ���ÿ��������
	DWORD dwBuyCountPerDay;      // ���ÿ��������
	DWORD dwDayCount;            // ÿ������������

};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_GM_MODIFY_BUYPRICEUPER_Data
{
	int iItemId;
	int iModifyPriceUper;
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_GM_MODIFY_DAY_LIMITCOUNT_Data
{
	int iItemId;
	int iModifyDayLimitCount;
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_GM_MODIFY_BUYLEVEL_Data
{
	int iItemId;
	int iLevel;
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_GM_MODIFY_BUYPRICE_Data
{
	int iItemId;
	int iModifyPrice;
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_RESPONCE_GM_MODIFY_Data
{
	BOOL bRlt;
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CENTERSERVER_REQUEST_BUY_Data
{
	DWORD dwItemId;
	DWORD dwPrice;
	BYTE byMark;
	WORD	wKey;
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_RESPONSE_CENTERSERVER_REQUEST_BUY_Data
{
	DWORD dwItemId;
	BYTE  bySubCoinRlt;  // 1 ��ʾ�ɹ���0 ��ʾʧ��
	BYTE byMark;         // 
};


struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CLIENT_ITEM_INFO_LIST_Data
{
	BYTE count;
	marketItemPacketInfo itemInfo[1];
};

struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CLIENT_ITEM_INFO_Data
{
	marketItemPacketInfo itemInfo;
};

typedef struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CLIENT_REQUEST_SELL_Data
{
	DWORD itemID;
	DWORD price; 
}ITEMMARKET_ITEMMARKET_PACKET_CMD_CLIENT_REQUEST_BUY_Data;

typedef struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CLIENT_RESPONCE_SELL_Data
{
	int result;
}ITEMMARKET_ITEMMARKET_PACKET_CMD_CLIENT_RESPONCE_BUY_Data;

typedef struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CENTERSERVER_REQUEST_SELL_Data
{
	DWORD itemID;
	DWORD price;
}ITEMMARKET_ITEMMARKET_PACKET_CMD_CENTERSERVER_RESPONCE_SELL_Data;

typedef struct ITEMMARKET_ITEMMARKET_PACKET_CMD_CENTERSERVER_ACQUIRE_ITEM_Data
{
	DWORD itemID;
	WORD	wKey;
};

#pragma pack(pop)