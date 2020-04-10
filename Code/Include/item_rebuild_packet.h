#ifndef ITEM_REBUILD_PACKET_H
#define ITEM_REBUILD_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_REBUILD
{	
	ITEM_REBUILD_OPEN,
	ITEM_REBUILD_START,   // NPC����װ��	
	ITEM_REBUILD_TIMEUP,  // ������Ҫʱ�䵽
	ITEM_REBUILD_RESULT,  // �жϽ��
	ITEM_REBUILD_END,     // �رմ���
	ITEM_REBUILD_CANCEL,  // ȡ������
};

enum
{
	item_rebuild_rlt_success,    // �ɹ�
	item_rebuild_rlt_failed,     // ʧ��

	item_rebuild_rlt_checkpass,  // ���ͨ��
	item_rebuild_rlt_checkfailed // ��Ʒ����
};  

enum
{
	ITEM_REBUILD_STATUS_NONE,
	ITEM_REBUILD_STATUS_DOING,
	ITEM_REBUILD_STATUS_END
};


struct ITEMREBUILD_ITEM_REBUILD_START_Data 
{
	DWORD	dwIndex;				//װ��
};


struct ITEMREBUILD_ITEM_REBUILD_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif