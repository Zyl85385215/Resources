#ifndef PAGETRANS_PACKET_H
#define PAGETRANS_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _PAGETRANS
{	
	PAGETRANS_OPEN,
	PAGETRANS_START,   // NPC升級裝備	
	PAGETRANS_TIMEUP,  // 升級需要時間到
	PAGETRANS_RESULT,  // 判斷結果
	PAGETRANS_END,     // 關閉窗口
	PAGETRANS_CANCEL,  // 取消升級
};

enum
{
	pagetrans_rlt_success,    // 成功
	pagetrans_rlt_failed,     // 失敗

	pagetrans_rlt_checkpass,  // 檢查通過
	pagetrans_rlt_checkfailed // 物品不足
};  

enum
{
	PAGETRANS_STATUS_NONE,
	PAGETRANS_STATUS_DOING,
	PAGETRANS_STATUS_END
};


struct PAGETRANS_PAGETRANS_START_Data 
{
	DWORD	dwIndex[3];				//秘笈
};


struct PAGETRANS_PAGETRANS_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif