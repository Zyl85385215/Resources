#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#define MARRY_ITEM_EFF 60013
#define MARRY_ITEM_CAKE 60014

#pragma pack (push)
#pragma pack (1)

struct MCData
{
	DWORD	dwID;			// ¶ÕΩ–≈œ¢ID
	BYTE	bTeachFlag;
	BYTE	bStudyFlag;
	DWORD	dwTeacher;	
	DWORD	dwPupil;
	char	szTName[20];
	char	szPName[20];
	BYTE	bTJob;
	BYTE	bPJob;
	BYTE	bTLvl;
	BYTE	bPLvl;
	int		nHonour;
};

struct ClientMarryData
{
	DWORD	dwID;			//∑Ú∆ﬁID
	BYTE	byState;
	int		nMValue;
	int		nMpoint;
	int		nWValue;
	int		nWpoint;
	DWORD	dwMan;	
	DWORD	dwWoman;
	char	szMName[20];
	char	szWName[20];
	char	szMHomeName[20];
	char	szWHomeName[20];
	BYTE	bMJob;
	BYTE	bWJob;
	BYTE	bMLvl;
	BYTE	bWLvl;
	int		szMSkill[5];
	int		szWSkill[5];
};

struct MCRequest
{
	DWORD	dwPuin;
	BYTE	bPJob;
	BYTE	bPLvl;
	BYTE	bSex;
	char	szPName[20];
};

enum
{
	S_SEND_INSTRUCT_CHG,
	S_SEND_MENTORING_ALL,
	S_SEND_MENTORING_ADD,
	S_SEND_MENTORING_CHG,
	S_SEND_MENTORING_DEL,
	S_SEND_MREQUEST_ALL,
	S_SEND_MREQUEST_ADD,
	S_SEND_MREQUEST_DEL,
	S_SEND_MYMREQUEST_ALL,
	S_SEND_MYMREQUEST_ADD,
	S_SEND_MYMREQUEST_DEL,
	C_REQUEST_TEACHERLIST,
	S_SEND_TEACHERLIST_RLT,
	C_REQUEST_INSTRUCT,
	C_REQUEST_STUDY,
	C_REQUEST_SENDITEM,
	C_REQUEST_GETITEM,
	C_REQUEST_BREAKRELATION,
	C_REQUEST_DEALWITH,
	S_SEND_SHOWFLAG_CHG,
	
	S_SEND_MARRY_REQUESTLIST,
	S_SEND_MARRY_ALL,
	S_SEND_MARRY_INSTRUCT,
	S_SEND_MARRY_DATA_ADD,
	S_SEND_MARRY_DATA_CHG,
	S_SEND_MARRY_DATA_DEL,
	C_REQUEST_MARRY_LIST,
	C_REQUEST_MARRY_INSTRUCT,
	C_REQUEST_MARRY_BREAKE,
	C_REQUEST_MARRY_SKILL,
	C_MARRY_QUEST_ACCEPT,
	C_MARRY_QUEST_COMPLETE,
	C_MARRY_QUEST_GIVEUP,
	C_MARRY_SENDRING,
	S_MARRY_SENDASK,
	C_MARRY_SENDANSWER,
	S_MARRY_SENDASKRLT,
	S_SEND_GETMARRIED,
	C_ANSWER_GETMARRIED,
	S_SEND_MARRYFAILED,
	S_SEND_POP_MARRYMESSAGE,
	S_SEND_MARRY_SHOW_TITLE,
	C_REQUEST_MARRY_SHOW_TITLE,
	C_MARRY_ACTION_EVENT,

	S_SEND_MENTOR_MSG,
	C_SEND_MENTOR_MSG,
	C_REQUEST_DEALWITH_ST,
	C_REQUEST_CANCEL_MINSTRUCT,

	MARRY_SELT_TEST=255,
};

struct RPGRELATION_C_MARRY_ACTION_EVENT_Data
{
	BYTE byAction;
};

struct RPGRELATION_C_REQUEST_MARRY_SHOW_TITLE_Data
{
	bool ifShow;
};

struct RPGRELATION_S_SEND_MARRY_SHOW_TITLE_Data
{
	bool ifShow;
};

struct RPGRELATION_MARRY_SELT_TEST_Data
{
	char szname[22];
};

struct RPGRELATION_S_SEND_POP_MARRYMESSAGE_Data
{
	char szTitle[22];
	char szMsg[128];
};

struct RPGRELATION_C_ANSWER_GETMARRIED_Data
{
	bool bRlt;
};

struct RPGRELATION_S_SEND_GETMARRIED_Data
{
	char szName[22];
};

struct RPGRELATION_S_MARRY_SENDASKRLT_Data
{
	char szName[22];
};

struct RPGRELATION_S_MARRY_SENDASK_Data
{
	int nUin;
	char szName[22];
};

struct RPGRELATION_C_MARRY_SENDANSWER_Data
{
	bool bRlt;
	int nUin;
};

struct RPGRELATION_C_MARRY_SENDRING_Data
{
	BYTE byPos;
};

struct RPGRELATION_C_REQUEST_MARRY_SKILL_Data
{
	BYTE byPos;
};

struct RPGRELATION_C_REQUEST_MARRY_LIST_Data
{
	int nFlag;
};

struct RPGRELATION_S_SEND_MARRY_DATA_DEL_Data
{
	int nID;
};

struct RPGRELATION_S_SEND_MARRY_DATA_ADD_Data
{
	ClientMarryData xInfo;
};

struct RPGRELATION_S_SEND_MARRY_DATA_CHG_Data
{
	ClientMarryData xInfo;
};

struct RPGRELATION_C_REQUEST_MARRY_REQUESTLIST_Data
{
	int nFlag;
};

struct RPGRELATION_S_SEND_MARRY_INSTRUCT_Data
{
	bool	ifInstruct;
};

struct RPGRELATION_S_SEND_MARRY_REQUESTLIST_Data
{
	bool ifClear;
	int nFlag;
	int nCnt;
	MCRequest szInfo[1];
};

struct RPGRELATION_S_SEND_MARRY_ALL_Data
{
	int nCnt;
	ClientMarryData szInfo[1];
};

struct RPGRELATION_C_REQUEST_DEALWITH_Data
{
	int nUin;
	bool ifOk;
};

struct RPGRELATION_S_SEND_SHOWFLAG_CHG_Data
{
	bool ifShow;
};

struct RPGRELATION_C_REQUEST_BREAKRELATION_Data
{
	int nID;
};

struct RPGRELATION_C_REQUEST_GETITEM_Data
{
	int nID;
};

struct RPGRELATION_C_REQUEST_SENDITEM_Data
{
	int nID;
};

struct RPGRELATION_C_REQUEST_STUDY_Data
{
	int nUin;
};

struct RPGRELATION_C_REQUEST_TEACHERLIST_Data
{
	int nFlag;
};

struct RPGRELATION_S_SEND_TEACHERLIST_RLT_Data
{
	bool ifClear;
	int nFlag;
	int nCnt;
	MCRequest szInfo[1];
};

struct RPGRELATION_S_SEND_INSTRUCT_CHG_Data
{
	bool ifInstruct;
};

struct RPGRELATION_S_SEND_MENTORING_ALL_Data
{
	int nCnt;
	MCData szInfo[1];
};

struct RPGRELATION_S_SEND_MENTOR_MSG_Data		
{
	char	szName[20]; 
	DWORD   dwSFUin;
};
struct RPGRELATION_C_SEND_MENTOR_MSG_Data		
{
	DWORD   dwStuUin;
};

struct RPGRELATION_C_REQUEST_DEALWITH_ST_Data		
{
	DWORD nUin;
	bool ifOk;
};

struct RPGRELATION_S_SEND_MENTORING_ADD_Data
{
	MCData xInfo;
};

struct RPGRELATION_S_SEND_MENTORING_CHG_Data
{
	MCData xInfo;
};

struct RPGRELATION_S_SEND_MENTORING_DEL_Data
{
	int nID;
};

struct RPGRELATION_S_SEND_MREQUEST_ALL_Data
{
	int nCnt;
	MCRequest szInfo[1];
};

struct RPGRELATION_S_SEND_MREQUEST_ADD_Data
{
	MCRequest xInfo;
};

struct RPGRELATION_S_SEND_MREQUEST_DEL_Data
{
	int nID;
};

struct RPGRELATION_S_SEND_MYMREQUEST_ALL_Data
{
	int nCnt;
	int nUin[1];
};

struct RPGRELATION_S_SEND_MYMREQUEST_ADD_Data
{
	int nUin;
};

struct RPGRELATION_S_SEND_MYMREQUEST_DEL_Data
{
	int nID;
};

#pragma pack (pop)