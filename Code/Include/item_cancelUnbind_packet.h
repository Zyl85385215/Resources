#ifndef ITEM_UNBINDCANCEL_PACKET_H
#define ITEM_UNBINDCANCEL_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_UNBINDCANCEL
{	
	ITEM_UNBINDCANCEL_OPEN,
	ITEM_UNBINDCANCEL_START,   // 
	ITEM_UNBINDCANCEL_TIMEUP,  // 升级需要时间到
	ITEM_UNBINDCANCEL_RESULT,  // 判断结果
	ITEM_UNBINDCANCEL_END,     // 关闭窗口
	ITEM_UNBINDCANCEL_CANCEL,  // 取消升级
};

enum
{
	item_unbindCancel_rlt_success,    // 成功
	item_unbindCancel_rlt_failed,     // 失败

	item_unbindCancel_rlt_checkpass,  // 检查通过
	item_unbindCancel_rlt_checkfailed // 物品不足
};  

enum
{
	ITEM_UNBINDCANCEL_STATUS_NONE,
	ITEM_UNBINDCANCEL_STATUS_DOING,
	ITEM_UNBINDCANCEL_STATUS_END
};


struct ITEMUNBINDCANCEL_ITEM_UNBINDCANCEL_START_Data 
{
	DWORD	dwIndex;				//装备
};


struct ITEMUNBINDCANCEL_ITEM_UNBINDCANCEL_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif