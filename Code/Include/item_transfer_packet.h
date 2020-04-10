#ifndef ITEM_TRANSFER_PACKET_H
#define ITEM_TRANSFER_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_TRANSFER
{	
	ITEM_TRANSFER_OPEN,
	ITEM_TRANSFER_START,	// NPC����װ��	
	ITEM_TRANSFER_TIMEUP,	// ������Ҫʱ�䵽
	ITEM_TRANSFER_RESULT,	// �жϽ��
	ITEM_TRANSFER_END,		// �رմ���
	ITEM_TRANSFER_CANCEL,	// ȡ������
};

enum
{
	item_transfer_rlt_success,		// �ɹ�
	item_transfer_rlt_failed,		// ʧ��

	item_transfer_rlt_checkpass,	// ���ͨ��
	item_transfer_rlt_checkfailed	// ��Ʒ����
};  

enum
{
	ITEM_TRANSFER_STATUS_NONE,
	ITEM_TRANSFER_STATUS_DOING,
	ITEM_TRANSFER_STATUS_END
};


struct ITEMTRANSFER_ITEM_TRANSFER_START_Data 
{
	DWORD	dwFromIndex;				// װ��
	DWORD	dwToIndex;
	bool	bNeedStone;
	bool	bHighStone;
};


struct ITEMTRANSFER_ITEM_TRANSFER_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif