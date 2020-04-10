#ifndef ITEM_REBUILD_PACKET_H
#define ITEM_REBUILD_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_REBUILD
{	
	ITEM_REBUILD_OPEN,
	ITEM_REBUILD_START,   // NPC升级装备	
	ITEM_REBUILD_TIMEUP,  // 升级需要时间到
	ITEM_REBUILD_RESULT,  // 判断结果
	ITEM_REBUILD_END,     // 关闭窗口
	ITEM_REBUILD_CANCEL,  // 取消升级
};

enum
{
	item_rebuild_rlt_success,    // 成功
	item_rebuild_rlt_failed,     // 失败

	item_rebuild_rlt_checkpass,  // 检查通过
	item_rebuild_rlt_checkfailed // 物品不足
};  

enum
{
	ITEM_REBUILD_STATUS_NONE,
	ITEM_REBUILD_STATUS_DOING,
	ITEM_REBUILD_STATUS_END
};


struct ITEMREBUILD_ITEM_REBUILD_START_Data 
{
	DWORD	dwIndex;				//装备
};


struct ITEMREBUILD_ITEM_REBUILD_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif