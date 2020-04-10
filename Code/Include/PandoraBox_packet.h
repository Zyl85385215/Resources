#ifndef PANDORABOX_PACKET_H
#define PANDORABOX_PACKET_H

#pragma pack (push)
#pragma pack (1)

enum _PANDORABOX
{	
	PANDORABOX_OPEN,
	PANDORABOX_START,   // NPC升級裝備	
	PANDORABOX_TIMEUP,  // 升級需要時間到
	PANDORABOX_RESULT,  // 判斷結果
	PANDORABOX_END,     // 關閉窗口
	PANDORABOX_CANCEL,  // 取消升級
};

enum
{
	pandorabox_rlt_success,    // 成功
	pandorabox_rlt_failed,     // 失敗

	pandorabox_rlt_checkpass,  // 檢查通過
	pandorabox_rlt_checkfailed // 物品不足
};  

enum
{
	PANDORABOX_STATUS_NONE,
	PANDORABOX_STATUS_DOING,
	PANDORABOX_STATUS_END
};


struct PANDORABOX_PANDORABOX_START_Data 
{
	DWORD	dwIndex;				//裝備
};


struct PANDORABOX_PANDORABOX_RESULT_Data 
{		
	BYTE	bRlt;
};

#pragma pack (pop)
#endif