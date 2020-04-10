#ifndef ITEM_TRANSFER_PACKET_H
#define ITEM_TRANSFER_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_TRANSFER
{	
	ITEM_TRANSFER_OPEN,
	ITEM_TRANSFER_START,	// NPC升级装备	
	ITEM_TRANSFER_TIMEUP,	// 升级需要时间到
	ITEM_TRANSFER_RESULT,	// 判断结果
	ITEM_TRANSFER_END,		// 关闭窗口
	ITEM_TRANSFER_CANCEL,	// 取消升级
};

enum
{
	item_transfer_rlt_success,		// 成功
	item_transfer_rlt_failed,		// 失败

	item_transfer_rlt_checkpass,	// 检查通过
	item_transfer_rlt_checkfailed	// 物品不足
};  

enum
{
	ITEM_TRANSFER_STATUS_NONE,
	ITEM_TRANSFER_STATUS_DOING,
	ITEM_TRANSFER_STATUS_END
};


struct ITEMTRANSFER_ITEM_TRANSFER_START_Data 
{
	DWORD	dwFromIndex;				// 装备
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