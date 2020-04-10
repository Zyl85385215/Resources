#ifndef ITEM_UNBINDCANCEL_PACKET_H
#define ITEM_UNBINDCANCEL_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_UNBINDCANCEL
{	
	ITEM_UNBINDCANCEL_OPEN,
	ITEM_UNBINDCANCEL_START,   // 
	ITEM_UNBINDCANCEL_TIMEUP,  // ������Ҫʱ�䵽
	ITEM_UNBINDCANCEL_RESULT,  // �жϽ��
	ITEM_UNBINDCANCEL_END,     // �رմ���
	ITEM_UNBINDCANCEL_CANCEL,  // ȡ������
};

enum
{
	item_unbindCancel_rlt_success,    // �ɹ�
	item_unbindCancel_rlt_failed,     // ʧ��

	item_unbindCancel_rlt_checkpass,  // ���ͨ��
	item_unbindCancel_rlt_checkfailed // ��Ʒ����
};  

enum
{
	ITEM_UNBINDCANCEL_STATUS_NONE,
	ITEM_UNBINDCANCEL_STATUS_DOING,
	ITEM_UNBINDCANCEL_STATUS_END
};


struct ITEMUNBINDCANCEL_ITEM_UNBINDCANCEL_START_Data 
{
	DWORD	dwIndex;				//װ��
};


struct ITEMUNBINDCANCEL_ITEM_UNBINDCANCEL_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif