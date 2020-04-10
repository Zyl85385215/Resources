#ifndef _NAUCTION_DATA_H_
#define	_NAUCTION_DATA_H_

#include <windows.h>
#include <time.h>
#pragma pack (push)
#pragma pack (1)

#define NEW_AUCTION_VERSION 0

enum
{
	NA_ITEM_OVERTIME,		//过期
	NA_ITEM_SELLOUT,		//卖出

	NA_ITEM_NORMAL,			//拍卖中

	NA_ITEM_ERROR = 99,
};

enum
{
	NA_OP_ADD,
	NA_OP_ADD_ERROR,	//物品超出退物品
	NA_OP_BUY,
	NA_OP_BUY_OVER,	//竞价被超出
	NA_OP_SELLOUT,
	NA_OP_OVERTIME,
	NA_OP_PICKOUT,
};

#define PNA_PACKET_DATA	BYTE*
#define PNA_OBJ_DATA	BYTE*
#define PNA_ITEM_DATA	BYTE*

#define NA_MAX_ITEMCNT	10

class NA_Object
{//发送给客户端物品的简单信息,让客户端排序
public:

	NA_Object::NA_Object()
	{
		memset(this,0,sizeof(NA_Object));
		tmEnd = time(NULL) + 86400;
		byFlag = NA_ITEM_NORMAL;
	}

	NA_Object::~NA_Object()
	{
		if(pData)
			free(pData);
	}
	char*	GetItemName(char* szName)
	{
		strcpy(szName,szSearch);
		for (int i = 0; i < strlen(szName); i++)
		{
			if(szName[i] == '#')
			{
				szName[i] = 0;
				break;
			}
		}
		return szName;
	}
	DWORD  dwSeller;	//寄售者
	DWORD  dwBuyer;		//当前的最高价格者
	int	   nPriceCur;	//当前价
	int	   nPriceMax;	//一口价
	DWORD  dwKey;		//识别编号
	LONGLONG	   llKey;		//物品INDEX
	BYTE   byType;		//物品类型
	BYTE   byLvl;		//物品等级
	BYTE   byGoldType;
	char   szSearch[32];	//检索关键字,默认为物品名字
	time_t tmEnd;		//time(NULL);开始寄售的时间
	BYTE   byFlag;		//物品状态  0-->已卖出, 1-->寄售中
	PNA_OBJ_DATA  pData;	//PNA_OBJ_DATA
};

class NA_SearchKey
{//检索条件类
public:
	NA_SearchKey()
	{
		memset(this,0,sizeof(NA_SearchKey));
	}
	DWORD dwSeller;
	DWORD dwBuyer;
	BYTE byType;
	BYTE byLvlMin;
	BYTE byLvlMax;
	char szKey[32];
	time_t tmStart;

};

struct NADATA_ItemBase
{
	WORD					m_wItemID;
	BYTE					m_bType;
	//BYTE					m_bSlot;								//	位置,11，255，装备部位0--10
	WORD					m_wDura;								//	持久
	WORD					m_wDuraMax;
	WORD					m_wData;
	DWORD					m_dwItemIndex;							//	唯一标识玩家一个物品
	DWORD					m_wDeadLine;
	DWORD					m_dwCreateTime;							//	创建时的时间
	LONGLONG				m_lItemKey;								//	预留,尽量别用

	BYTE					m_bDemonPowLvl;							//	魔化等级
	BYTE					m_bPlusLevel;
	int						m_nPlusScore;
};

#pragma pack (pop)
#endif