#ifndef ITEM_BIND_PACKET_H
#define ITEM_BIND_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_BIND
{
	ITEM_BIND_OPEN,
	ITEM_BIND_START,   // NPC����װ��
	ITEM_BIND_TIMEUP,  // ������Ҫʱ�䵽
	ITEM_BIND_RESULT,  // �жϽ��
	ITEM_BIND_END,     // �رմ���
	ITEM_BIND_CANCEL,  // ȡ������
};

enum
{
	item_bind_rlt_success,    // �ɹ�
	item_bind_rlt_failed,     // ʧ��

	item_bind_rlt_checkpass,  // ���ͨ��
	item_bind_rlt_checkfailed // ��Ʒ����
};  

enum
{
	ITEM_BIND_STATUS_NONE,
	ITEM_BIND_STATUS_DOING,
	ITEM_BIND_STATUS_END
};

enum
{
	BIND_SAFE_SIX_HOUR,
	BIND_SAFE_ONE_DAY,
	BIND_SAFE_THREE_DAY,
	BIND_SAFE_SEVEN_DAY,
};

struct ITEMBIND_ITEM_BIND_START_Data 
{
	DWORD	dwIndex;				//װ��
	BYTE	byBindType;
};


struct ITEMBIND_ITEM_BIND_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif