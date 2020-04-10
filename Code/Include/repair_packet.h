#ifndef _REPAIR_PACKET_H_
#define _REPAIR_PACKET_H_

#pragma pack (push)
#pragma pack (1)

enum _REPAIR{	
	REPAIR_OPEN,
	REPAIR_START,   // NPC修理	
	REPAIR_TIME,    // 修理时间
	REPAIR_RESULT,
	REPAIR_END,     // 取消修理
};

enum
{
	repair_type_normal,
	repair_type_good,
	repair_type_slave,
};

enum
{
	repair_rlt_success,
	repair_rlt_noitem,
	repair_rlt_nogold,
};

enum{
	REPAIR_STATUS_NONE,
	REPAIR_STATUS_DOING,
	REPAIR_STATUS_END
};


struct REPAIR_REPAIR_OPEN_Data {
	BYTE	bType;
};

struct REPAIR_REPAIR_START_Data {
	DWORD	itemIndex;
	BYTE    bType;
};

struct REPAIR_REPAIR_TIME_Data {	
	DWORD		itemIndex;	
	int 		nTime;		
};

struct REPAIR_REPAIR_RESULT_Data {		
	BYTE		bRlt;		
};

#pragma pack (pop)
#endif