#ifndef _MAKE_PACKET_H_
#define _MAKE_PACKET_H_

#pragma pack (push)
#pragma pack (1)

#define MAX_KIND			5

enum _MAKE 
{	

	MAKE_OPEN,
	MAKE_RESULT,
	MAKE_START,
	MAKE_TIME,          //合成时间
	MAKE_END,//取消合成
	MAKE_OPEN2,
	MAKE_OPEN3,
	MAKE_OPEN4,
};

enum 
{
	GONGQIBIN,
	QIBIN,
	GONGBIN,
	BUBIN
};

enum
{
	MAKE_STATUS_NONE,
	MAKE_STATUS_DOING,
	MAKE_STATUS_END
};

enum
{
	MAKE_SUCCESS,
	MAKE_ERROR,
	MAKE_CANNOTMAKE,
	MAKE_NOITEM,
	MAKE_NOATT,
};

struct MAKE_ITEM 
{
	DWORD	dwItemIndex;
	BYTE	btNum;
};

struct MAKE_MAKE_OPEN_Data 
{
	WORD		wIndex;
	WORD		wSkillID;
};

struct MAKE_MAKE_TIME_Data 
{
	DWORD		itemIndex;
	WORD        wTime;
};

struct MAKE_MAKE_START_Data
{
	WORD		wIndex;			//选项卡编号
	WORD		wItemID;
	int		ncount;
};

struct MAKE_MAKE_RESULT_Data 
{	
	WORD		wItemID;	
	BYTE		bCnt;
	WORD		wID[1];
};

#pragma pack (pop)
#endif


