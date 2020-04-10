#pragma once

#include "battle_packet.h"

#pragma pack(push)
#pragma pack(1)

enum
{
	SCHOOL_WAR_OPEN_MENU,					//打开界面
	SCHOOL_WAR_CLOSE_MENU,					//关闭界面
	SCHOOL_WAR_REQUEST_LIST_VIEW_BY_ID,		//根据战场id请求链表
	SCHOOL_WAR_RESPONCE_LIST_VIEW_BY_ID,	//战场id请求回复
};

struct SCHOOL_WAR_SCHOOL_WAR_OPEN_MENU_Data
{
	int schoolId;
};

struct SCHOOL_WAR_SCHOOL_WAR_REQUEST_LIST_VIEW_BY_ID_Data
{
	int	warBaseId;
};

struct SCHOOL_WAR_SCHOOL_WAR_RESPONCE_LIST_VIEW_BY_ID_Data
{
	bool begin;
	int	warBaseId;
	int	count;
	globalWarId	warId[1];
};

#pragma pack(pop)