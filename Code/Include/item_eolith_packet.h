#ifndef ITEM_EOLITH_PACKET_H
#define ITEM_EOLITH_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_EOLITH
{	
	ITEM_EOLITH_OPEN,
	ITEM_EOLITH_START,	// 
	ITEM_EOLITH_RESULT,	// �Д�Y��
	ITEM_EOLITH_END,	// �P�]����
};

enum
{
	item_eolith_rlt_success,	// �ɹ�
	item_eolith_rlt_failed,		// ʧ��
};  

enum
{
	ITEM_EOLITH_STATUS_NONE,
	ITEM_EOLITH_STATUS_END
};

struct ITEMEOLITH_ITEM_EOLITH_START_Data 
{
	BYTE	bCount;				//	����
};

struct ITEMEOLITH_ITEM_EOLITH_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif