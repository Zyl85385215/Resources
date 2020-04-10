#pragma once

enum
{
	ITEMTYPE_NULL,
	ITEMTYPE_GOODS,			 //物品
	ITEMTYPE_PERSONEXP,		 //人物经验
	ITEMTYPE_SKILLEXP,		 //技能经验
	ITEMTYPE_GOLDS,          //金钱
	ITEMTYPE_PRESTIGE,       //声望
	ITEMTYPE_EXPLOIT,		 //功勋
	ITEMTYPE_BINDITEM,
};

class TREASUREBOX_ItemNode
{
public:

	WORD itemType;
    int number;
	int rand;

	TREASUREBOX_ItemNode(WORD itemType=0, int number=0, int rand=100)
	{
		this->itemType = itemType;
		this->number = number;
		this->rand = rand;
	}
};

enum
{
	TREASUREBOX_STATUS_NONE,
	TREASUREBOX_STATUS_DOING,
	TREASUREBOX_STATUS_GIVED,
	TREASUREBOX_STATUS_END,
};

enum 
{
	TREASUREBOX_OPENNULL,
	TREASUREBOX_OPENTRSBOX,
	TREASUREBOX_OPENPRIZEBOX,
};

enum 
{
	OPENTYPE_NONE,
	OPENTYPE_OUT,
	OPENTYPE_IN,
	OPENTYPE_PRIZE,
	OPENTYPE_PRESTIGE,
};

enum _TREASUREBOX
{
	TREASUREBOX_OPEN,
	TREASUREBOX_STOP,
	TREASUREBOX_RES,
	TREASUREBOX_END,
	PRIZEBOX_OPEN,
	PRIZEBOX_STOP,
	PRIZEBOX_RES,
	PRIZEBOX_END,
	PRIZEBOX_LOCK,
	PRIZEBOX_LOCK_RESULT,
};


struct TREASUREBOX_TREASUREBOX_OPEN_Data
{

	BYTE  openType;			//界面的打开方式(不同物品可能要读入的物品不同)
	TREASUREBOX_ItemNode outItem[20];
	TREASUREBOX_ItemNode inItem[12];
};

struct TREASUREBOX_PRIZEBOX_OPEN_Data
{
	BYTE  openType;			//界面的打开方式(不同物品可能要读入的物品不同)
	TREASUREBOX_ItemNode randItem[25];
};

struct TREASUREBOX_TREASUREBOX_STOP_Data
{
	int randRes;
};

struct TREASUREBOX_PRIZEBOX_STOP_Data
{
	int randRes;
};

struct TREASUREBOX_PRIZEBOX_LOCK_Data
{
	DWORD dwLockBit;
};

struct TREASUREBOX_PRIZEBOX_LOCK_RESULT_Data
{
	bool bSuccessful;
};