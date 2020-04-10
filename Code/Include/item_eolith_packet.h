#ifndef ITEM_EOLITH_PACKET_H
#define ITEM_EOLITH_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _ITEM_EOLITH
{	
	ITEM_EOLITH_OPEN,
	ITEM_EOLITH_START,	// 
	ITEM_EOLITH_RESULT,	// 判斷結果
	ITEM_EOLITH_END,	// 關閉窗口
};

enum
{
	item_eolith_rlt_success,	// 成功
	item_eolith_rlt_failed,		// 失敗
};  

enum
{
	ITEM_EOLITH_STATUS_NONE,
	ITEM_EOLITH_STATUS_END
};

struct ITEMEOLITH_ITEM_EOLITH_START_Data 
{
	BYTE	bCount;				//	数量
};

struct ITEMEOLITH_ITEM_EOLITH_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif