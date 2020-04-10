#ifndef _STORAGE_PACKET_H_
#define _STORAGE_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum {
		OPEN_STORE,
			
		TAKE_ITEM,
		STORE_ITEM,
		
		TAKE_MONEY,
		STORE_MONEY,
		SET_PASS,//��������
		CHECK_PASS,//��������
		PASS_SUCCEED,
		PASS_UNSUCCEED,
		SET_SUCCEED,//���óɹ�
		SET_UNSUCCEED,//����ʧ��
		
		SEND_ITEM_DATA,
};

struct STORAGE_TAKE_ITEM_Data {
	DWORD	itemIndex;//bank��index
};

struct STORAGE_STORE_ITEM_Data {
	DWORD	itemIndex;//bag��index
};

struct STORAGE_TAKE_MONEY_Data {
	DWORD	money;
};

struct STORAGE_STORE_MONEY_Data {
	DWORD	money;
};

struct STORAGE_SET_PASS_Data {
	char oldPass[17];
	char newPass[1];
};

struct STORAGE_CHECK_PASS_Data {
	char storPass[1];
};

struct STORAGE_SEND_ITEM_DATA_Data
{
	DWORD itemIndex;
	DWORD wData; 
};
#pragma pack (pop)
#endif