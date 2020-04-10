#pragma once

#ifdef _ISSERVER
#include <Windows.h>
#else
#include "GlobalDefine.h"
#endif

#pragma pack (push)
#pragma pack (1)


enum NASALEENUM                     //���а�����
{
	NA_SELL_ITEM,		//������Ʒ
	NA_REQUST_ITEM,		//����Ʒ
	NA_BUY_ITEM,		//����Ʒ
	NA_ANSWER_ITEM,		//����Ʒ����

	NA_SEARCH_SELL,
	NA_SELL_LIST,

	NA_MY_LIST,

	NA_CALC_MYMONEY,

	NA_SELL_ITEM_EX,		//������Ʒ������
};

class NAItem_Data
{
public:
	DWORD	dwNaKey;
	BYTE	byType;		//0����  1�չ�

	WORD	wItemID;
	WORD	wCnt;
	int		nPrice;
	int		nRmSec;
	DWORD	dwExInfo;
};

struct NASALE_NA_SELL_ITEM_Data
{
	DWORD	dwIndex;
	int		nPrice;			//����
};

struct NASALE_NA_SELL_ITEM_EX_Data
{
	DWORD	dwIndex;
	int		nPrice;
	int     nCount;
};

struct NASALE_NA_REQUST_ITEM_Data
{
	WORD	wItemID;
	WORD	wCnt;
	int		nPrice;			//����
};

struct NASALE_NA_BUY_ITEM_Data
{
	DWORD	dwNAIndex;
	WORD	wCnt;
	
};

struct NASALE_NA_ANSWER_ITEM_Data
{
	DWORD	dwNAIndex;
	WORD	wCnt;
};

struct NASALE_NA_SELL_LIST_Data
{
	WORD	wCnt;
	int		wPageCnt;
	int		nEndPos;
	NAItem_Data	szData[1];
};

struct NASALE_NA_MY_LIST_Data
{
	WORD	wCnt;
	NAItem_Data	szData[1];
};

struct NASALE_NA_SEARCH_SELL_Data
{
	BYTE	bType;
	BYTE	bJob;
	BYTE	bLvStep;
	int		nStartPos;
};

#pragma pack (pop)