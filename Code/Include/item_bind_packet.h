#ifndef ITEM_BIND_PACKET_H
#define ITEM_BIND_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_BIND
{
	ITEM_BIND_OPEN,
	ITEM_BIND_START,   // NPC升级装备
	ITEM_BIND_TIMEUP,  // 升级需要时间到
	ITEM_BIND_RESULT,  // 判断结果
	ITEM_BIND_END,     // 关闭窗口
	ITEM_BIND_CANCEL,  // 取消升级
};

enum
{
	item_bind_rlt_success,    // 成功
	item_bind_rlt_failed,     // 失败

	item_bind_rlt_checkpass,  // 检查通过
	item_bind_rlt_checkfailed // 物品不足
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
	DWORD	dwIndex;				//装备
	BYTE	byBindType;
};


struct ITEMBIND_ITEM_BIND_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif