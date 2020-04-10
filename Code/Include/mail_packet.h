#ifndef _PLAYERMAIL_H_
#define _PLAYERMAIL_H_
#include <windows.h>
#include "packetstruct.h"
#include "item_packet.h"
#include <map>
#include "misc.h"
using namespace std;
#define SEND_MAIL_COST		10
#define SEND_MAIL_ITEM_MAX	5

enum RLT_CODE 
{
	RLT_CODE_SUCCESS,
	RLT_CODE_NACTIVE_CENTER,
	RLT_CODE_NRECVER,
	RLT_CODE_NITEM,
	RLT_CODE_NREADY,
	RLT_CODE_NHAVEMAILPAY,
	RLT_CODE_TAKEMONEY_ERR,
	RLT_CODE_NMAIL,
	RLT_CODE_NMAILLIST,
	RLT_CODE_BAGFULL,
	RLT_CODE_MONEYFULL,
	RLT_CODE_MAILLISTFULL,
	RLT_CODE_MAILNOTTAKEMONEY,
};

enum
{
	C_SEND_NEW_MAIL,			// 发件
	CS_SEND_NEW_MAIL_RLT,		// 发件响应

	C_READ_MAIL,				// 读件
	GS_READ_MAIL_RLT,			// 读件回复

	C_REMOVE_MAIL,				// 删件
	CS_REMOVE_RLT,				// 删件响应

	CS_NOTIFY_HAS_MAIL,         // 命令，标示有新邮件

	CS_ADD_MAIL,				// 发给服务器端，添加邮件和邮件物品绑定
	GS_ADD_MAIL,				// 仅概要信息，装成邮包后发送邮件给客户端

	C_GET_MAILLIST,				// 命令，向中心服务器索取邮件列表
	GS_GET_MAILLIST,			// 附带上uin，向中心服务器索取邮件列表

	CS_SEND_SYSTEM_MAIL,		// 由中心服务器向游戏服务器发送系统邮件包,用于GM工具

	GS_OPEN_MAIL_BOX,			// 打开邮箱,到时候用于脚本,点击邮箱NPC打开这个邮箱列表

	// *********************    ##物品相关##    **************
	C_RETURN_MAIL,				// 回件
	C_TAKE_MAIL_ITEM,			// 拿物品
	C_TAKE_MAIL_ITEMS,			// 拿出所有物品
	C_TAKE_MAIL_MONEY,			// 拿钱

	CS_RETURN_RLT,				// 回件结果
	CS_TAKE_MAIL_ITEM_RLT,		// 拿物品,响应
	CS_TAKE_MAIL_ITEMS_RLT,		// 拿出所有物品,响应
	CS_TAKE_MAIL_MONEY_RLT,		// 拿钱.响应

	GS_SEND_MAIL_ITEM,
	GS_SEND_MAIL_ITEM_EXTEND,	
	GS_SEND_MAIL_ITEM_BINDINFO,
	GS_SEND_MAIL_ITEM_JUDGE,
	GS_SEND_MAIL_ITEM_GEMS,

	GS_SEND_MAX_ID,

	GET_WAGE_MAIL,				// 获得工资信

	GS_SEND_MAIL_ITEM_TRUMP,

	GS_SEND_MAIL_ITEM_PLUS,
	GS_SEND_MAIL_ITEM_CARVE,
	GS_SEND_MAIL_ITEM_WING,
	GS_SEND_MAIL_ITEM_HORSEEQ,
	GS_SEND_MAIL_ITEM_ENCHASE,
};

#define SAFE_FREE_EX(p) { if(p) { free(p); (p) = NULL; } }

#pragma pack (push)
#pragma pack (1)

struct MAIL_GS_GET_MAILLIST_Data
{
	DWORD	dwUin;
};

struct MAIL_GS_SEND_MAX_ID_Data
{
	DWORD	dwMailID;
	DWORD	dwItemIndex;
	DWORD	dwMailCnt;
	DWORD	dwItemCnt;
};

typedef struct MAIL_C_SEND_NEW_MAIL_Data
{
	DWORD	dwMailID;			// 邮件编号 
	DWORD	dwSenderUin;		// 发件uin,0 表示系统
	DWORD	dwRecverUin;		// 收件uin 
	DWORD	dwMoney;			// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
	DWORD	dwItemCount;		// 物品数量
	DWORD	dwItemIndex[SEND_MAIL_ITEM_MAX];
	DWORD	timeSend;			// 发送时间
	DWORD	timeOverdue;		// 发送时间
	DWORD   dwPayItemIndex;		// 消耗哪个物品
	bool	bVisited;			// 是否浏览过
	bool	bReturned;			// 0表示原件;1是回件;系统邮件属于回件
	bool	bPayFor;			// 看是否付费取信
	char	body[1];
//	char *	m_szRecverName;		// 收件人名字
// 	char *	szSenderName;		// 发件人名字
// 	char *	szTitle;			// 标题
// 	char *	szBody;				// 正文
}
MAIL_CS_ADD_MAIL_Data;

struct MAIL_CS_SEND_SYSTEM_MAIL_Data
{
	BYTE	byPayType;
	DWORD dwRecvUin;
	DWORD dwMoney;
	DWORD dwItemID[SEND_MAIL_ITEM_MAX];
	char  szBody[1];
// 	char *	szTitle;			// 标题
// 	char *	szBody;				// 正文
};

typedef struct MailIdentifier
{
	DWORD	dwRecverUin;		// 找到收件人
	DWORD	dwSenderUin;
	DWORD	dwMailID;
	bool	bReturned;			// 获取发起人,False 发起人为dwSenderUin,true 发起人为dwRecverUin,取物品用
}
MAIL_C_READ_MAIL_Data,
MAIL_C_REMOVE_MAIL_Data,
MAIL_C_RETURN_MAIL_Data,
MAIL_C_TAKE_MAIL_ITEMS_Data,
MAIL_C_TAKE_MAIL_MONEY_Data;

typedef struct MAIL_CS_REMOVE_RTL_Data
{
	MailIdentifier	miData;
	BYTE			byCode;				// 0表示成功
}
MAIL_CS_TAKE_MAIL_ITEMS_RLT_Data,
MAIL_CS_TAKE_MAIL_MONEY_RLT_Data,
MAIL_CS_SEND_NEW_MAIL_RLT_Data,
MAIL_CS_REMOVE_RLT_Data,			// 删件结果
MAIL_CS_RETURN_RLT_Data;

struct MAIL_C_TAKE_MAIL_ITEM_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;		// 
};

struct MAIL_CS_TAKE_MAIL_ITEM_RLT_Data
{
	MAIL_C_TAKE_MAIL_ITEM_Data pData;
	BYTE byReturnCode;
};

struct MAIL_GS_ADD_MAIL_Data
{
	MailIdentifier	miData;
	DWORD			timeSend;	
	DWORD			timeOverdue;
	bool			bVisited;			// 是否浏览过 
	char			body[1];
// 	char *			szSenderName;		// 发件人名字		
// 	char *			szTitle;			// 标题
};

struct MAIL_GS_READ_MAIL_RLT_Data
{
	MailIdentifier	miData;
	DWORD			dwMoney;			// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
	DWORD			dwItemCount;		// 物品数量
	DWORD			timeOverdue;
	bool			bPayFor;			// 看是否付费取信
	char			body[1];
// 	char *			szBody;				// 正文
};

struct MAIL_GS_SEND_MAIL_ITEM_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	CLIENTITEM		xItem;
};

struct MAIL_GS_SEND_MAIL_ITEM_EXTEND_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	ItemExtendInfo	xItemExtend;
};

struct MAIL_GS_SEND_MAIL_ITEM_BINDINFO_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	ItemBindInfo	xItemBindInfo;
};

struct MAIL_GS_SEND_MAIL_ITEM_JUDGE_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	ExtendData		xItemExtend;
};

struct MAIL_GS_SEND_MAIL_ITEM_GEMS_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	int				nSize;
	BYTE 			pbData[1];
};

struct MAIL_GS_SEND_MAIL_ITEM_TRUMP_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	TrumpSPData		pData;
};

struct MAIL_GS_SEND_MAIL_ITEM_PLUS_Data 
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	BYTE			byLevel;
	int				nScore;
	ItemPlusUpInfo	pPlusInfo;
};


struct MAIL_GET_WAGE_MAIL_Data
{
	DWORD	dwRecverUin;		// 找到收件人
	DWORD	dwVal;				// 工资值	
};

#pragma pack (pop)

enum
{// LOAD MAIL ITEM RESULT
	LMI_PRE,		// 准备
	LMI_OK,			// 填充
	LMI_NULL,		// 物品是空的
};

struct MailItemInfo
{
	DWORD	dwItemIndex;
	void*	pItem;
	BYTE	byItemLoadResult;
};

#define UNVISIT_TIME_PERSIST 604800	// 7 天   未读邮件持续时间
#define VISITED_TIME_PERSIST 86400	// 1 天   已读邮件持续时间

class CPlayerMailList;
class CPlayerMail
{
public:
 	MailIdentifier		m_miData;
private:
	DWORD				m_dwMoney;							// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
	DWORD				m_dwItemCount;						// 物品数量
	bool				m_bPayFor;							// 看是否付费取信
	char *				m_szSenderName;						// 发件人名字		
	char *				m_szRecverName;						// 收件人名字	

	char *				m_szTitle;							// 标题
	char *				m_szBody;							// 正文

	DWORD				m_timeSend;							// 发送时间
	DWORD				m_timeOverdue;							// 过期时间
	bool				m_bRead;							// 客户端 标示读过了,可以从缓存获取该信件信息
	bool				m_bVisited;							// 是否浏览过

	MailItemInfo		m_pItemInfo[SEND_MAIL_ITEM_MAX];	// 发件物品
public:

// 	bool	SetItemIntoMailFlag(DWORD dwItemIndex, BYTE byLoadMailResut);
// 	BYTE	GetItemIntoMailFlag(DWORD dwItemIndex) ;
// 	bool	IsItemLoadReady();
// 
// 	BYTE	GetItemByIndex(DWORD dwItemIndex) ;
// 	bool	AddMailItemIndex(int dwItemIndex );
// 	void	SetMailItemIndex(DWORD dwItemIndex,DWORD nPos);
// 	void	RemoveMailItemIndex(int nPos);
// 
// 	bool	AddMailItem( void* pItem,int dwItemIndex);
// 	bool	SetMailItem( void* pItem,int dwItemIndex);
// 	bool	RemoveMailItem(int dwItemIndex);
	DWORD	GetItemCount() const	{ return m_dwItemCount; }
	void*	GetItem(int nPos)		{ return m_pItemInfo[nPos].pItem; }
	DWORD	GetItemIndex(int nPos)	{ return m_pItemInfo[nPos].dwItemIndex; }
	void	SetItemCount(DWORD val)	{ m_dwItemCount = val;	}

	DWORD	GetTimeOverdue() const	{ return m_timeOverdue;	}
	DWORD	GetTimeSend() const		{ return m_timeSend;	}
	bool	GetPayFor() const		{ return m_bPayFor;		}
	DWORD	GetMoney() const		{ return m_dwMoney;		}
	char *	GetRecverName() const	{ return m_szRecverName;}
	char *	GetSenderName() const	{ return m_szSenderName;}
	bool	GetReadFlag()			{ return m_bRead; }

	void	SetTimeSend(DWORD dwNowTick) 			{ m_timeSend = dwNowTick;		}
	void	SetTimeOverdue(DWORD dwNowTick) 		{ m_timeOverdue = dwNowTick;		}
	void	SetTimeOverdue_UnVisit(DWORD dwNowTick) { m_timeOverdue = dwNowTick+UNVISIT_TIME_PERSIST;}
	void	SetTimeOverdue_Visited(DWORD dwNowTick) { m_timeOverdue = dwNowTick+VISITED_TIME_PERSIST;}
	void	SetReadFlag()							{ m_bRead	= true; }

	friend class CPlayerMailList;
	inline const char  * GetTitle()	{return	m_szTitle;		}
	inline const char  * GetBody()	{return	m_szBody;		}
	inline const char  * GetName()	{return	m_szSenderName;	}

	void	SetTitle(char * sztitle){ if(m_szTitle) { free(m_szTitle); } m_szTitle = strdup(sztitle);gfun_ReplaceStr(m_szTitle);}
	void	SetBody (char * szbody){ if(m_szBody) { free(m_szBody); } m_szBody = strdup(szbody);gfun_ReplaceStr(m_szBody);}
	void	SetSenderName (char * szSenderName)	{ if(m_szSenderName) { free(m_szSenderName); } m_szSenderName = strdup(szSenderName);}
	void	SetRecverName (char * szRecverName)	{ if(m_szRecverName) { free(m_szRecverName); } m_szRecverName = strdup(szRecverName);}
	void	SetBePayFor(bool bPayFor)			{ m_bPayFor	= bPayFor;}
	void	SetMoney(DWORD dwMoney)				{ m_dwMoney	= dwMoney; }
	void	SetVisit(bool bVisited = true)		{ m_bVisited = bVisited;  }
	bool	GetVisit()							{ return m_bVisited;  }

	// 客户端写件,发件函数
	CPlayerMail(DWORD dwSenderUin,char * szSenderName)// 发件人的构造函数,标明这封邮件是谁创造的
	{
		memset(this, 0, sizeof(CPlayerMail));
		ResetSenderMail();
		m_miData.dwSenderUin = 0;
		m_szSenderName		 = NULL;
		SetSender(dwSenderUin, szSenderName);
	}

	// 客户端收件
	CPlayerMail(MAIL_GS_ADD_MAIL_Data* pMailInfo)
	{
		m_miData.dwMailID		= pMailInfo->miData.dwMailID;				// 邮件编号 
		m_miData.dwSenderUin	= pMailInfo->miData.dwSenderUin;			// 发件uin
		m_miData.dwRecverUin	= pMailInfo->miData.dwRecverUin;			// 收件uin 
		m_bVisited				= pMailInfo->bVisited;						// 是否浏览过
		m_miData.bReturned		= pMailInfo->miData.bReturned;				// 0表示原件;1是回件;系统邮件属于回件
		m_timeSend				= pMailInfo->timeSend;
		m_szSenderName			= strdup(pMailInfo->body);					// 发件人名字
		int NameLen				= strlen(pMailInfo->body) + 1;
		m_szTitle				= strdup(pMailInfo->body + NameLen);		// 标题

		gfun_ReplaceStr(m_szTitle);
		m_szBody				= NULL;										// 正文
		m_szRecverName			= NULL;
		m_timeOverdue			= pMailInfo->timeOverdue;
		m_dwMoney				= 0;										// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
		m_bPayFor				= false;									// 看是否付费取信
		m_dwItemCount			= 0;										// 物品数量
		m_bRead					= false;
		memset(m_pItemInfo,0,sizeof(MailItemInfo)*SEND_MAIL_ITEM_MAX);
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			m_pItemInfo[i].byItemLoadResult = LMI_OK;	
		}
	}

	// 中心服务器,游戏服务器收件
	CPlayerMail(MAIL_C_SEND_NEW_MAIL_Data* pMailData)
	{
		m_miData.dwMailID		= pMailData->dwMailID;				// 邮件编号 
		m_miData.dwSenderUin	= pMailData->dwSenderUin;			// 发件uin
		m_miData.dwRecverUin	= pMailData->dwRecverUin;			// 收件uin 
		m_dwMoney				= pMailData->dwMoney;				// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
		m_dwItemCount			= pMailData->dwItemCount;			// 物品数量
		m_bVisited				= pMailData->bVisited;				// 是否浏览过
		m_miData.bReturned		= pMailData->bReturned;				// 0表示原件;1是回件;系统邮件属于回件
		m_bPayFor				= pMailData->bPayFor;				// 看是否付费取信

		m_szRecverName			= strdup(pMailData->body);

		int NameLen				= strlen(m_szRecverName) + 1;
		m_szSenderName			= strdup(pMailData->body + NameLen);			// 发件人名字

		NameLen					+= strlen(m_szSenderName) + 1;
		m_szTitle				= strdup(pMailData->body + NameLen);	// 标题

		NameLen					+= strlen(m_szTitle) + 1;
		m_szBody				= strdup(pMailData->body + NameLen);								// 正文

		gfun_ReplaceStr(m_szBody);
		gfun_ReplaceStr(m_szTitle);

		m_timeSend				= pMailData->timeSend;
		// 设置需要装填的位
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			m_pItemInfo[i].dwItemIndex		= pMailData->dwItemIndex[i];
			m_pItemInfo[i].pItem			= NULL;
			if(pMailData->dwItemIndex[i] != 0)
				m_pItemInfo[i].byItemLoadResult = LMI_PRE;
			else
				m_pItemInfo[i].byItemLoadResult = LMI_OK;	
		}
		m_timeOverdue			= pMailData->timeOverdue;
		m_bRead					= false;
	}

	CPlayerMail()
	{
		m_miData.dwMailID		= 0;					// 邮件编号 
		m_miData.dwSenderUin	= 0;					// 发件uin
		m_miData.dwRecverUin	= 0;					// 收件uin 
		m_dwMoney				= 0;					// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
		m_dwItemCount			= 0;					// 物品数量
		m_timeSend				= 0;					// 发送时间
		m_bVisited				= false;				// 是否浏览过
		m_bPayFor				= false;				// 看是否付费取信
		m_miData.bReturned		= false;				// 0表示原件;1是回件;系统邮件属于回件
		m_szSenderName			= strdup("");			// 发件人名字
		m_szTitle				= strdup("");			// 标题
		m_szBody				= strdup("");			// 正文
		m_szRecverName			= strdup("");
		m_timeOverdue				= 0;
		m_timeSend				= 0;
		m_bRead					= false;
		memset(m_pItemInfo,0,sizeof(MailItemInfo)*SEND_MAIL_ITEM_MAX);
	}

	~CPlayerMail()
	{
		if (m_szTitle) 
			free(m_szTitle);
		if(m_szBody)
			free(m_szBody);
		if (m_szSenderName)
			free(m_szSenderName);
		if (m_szRecverName)
			free(m_szRecverName);
	}

	void	SetSender(DWORD dwSenderUin,char * szSenderName)
	{
		m_miData.dwSenderUin	= dwSenderUin;			// 发件uin
		SetSenderName(szSenderName);					// 发件人名字
	}

	void	ResetSenderMail()
	{
		m_miData.dwMailID		= 0;					// 邮件编号 
		m_miData.dwRecverUin	= 0;					// 收件uin 
		m_dwMoney				= 0;					// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
		m_dwItemCount			= 0;					// 物品数量
		m_timeSend				= 0;					// 发送时间
		m_bVisited				= false;				// 是否浏览过
		m_miData.bReturned		= false;				// 0表示原件;1是回件;系统邮件属于回件
		m_bPayFor				= false;				// 看是否付费取信
		SAFE_FREE_EX(m_szTitle);
		SAFE_FREE_EX(m_szBody);
		SAFE_FREE_EX(m_szRecverName);
		m_szTitle				= strdup("");			// 标题
		m_szBody				= strdup("");			// 正文
		m_szRecverName			= strdup("");
		m_timeSend				= 0;
		m_timeOverdue			= 0;
		m_bRead					= 0;
		memset(m_pItemInfo,0,sizeof(MailItemInfo)*SEND_MAIL_ITEM_MAX);
	}
	void    GetSendLog(char **szText)
	{
		sprintf(*szText,"send:%s,money:%d,item:%d_%d_%d_%d_%d_%d,pay:%d,recv:%s\n",m_szSenderName,
			m_dwMoney,m_pItemInfo[0].dwItemIndex,m_pItemInfo[1].dwItemIndex,m_pItemInfo[2].dwItemIndex,
			m_pItemInfo[3].dwItemIndex,m_pItemInfo[4].dwItemIndex,m_pItemInfo[5].dwItemIndex,m_bPayFor,
			m_szRecverName);
	}

	Packet *BuildSendMailPacket(char* szInBuff) // >>发件<< 1,成功,2,收件人为空,3标题为空,4.正文为空
	{
		if( m_szSenderName == NULL	|| strlen(m_szSenderName) == 0)	return NULL;
		if( m_szTitle      == NULL	|| strlen(m_szTitle)      == 0)	return NULL;
		if( m_szBody       == NULL	|| strlen(m_szBody)       == 0)	return NULL;

		m_dwItemCount = 0;
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			if(m_pItemInfo[i].dwItemIndex != 0)
			{
				m_pItemInfo[i].dwItemIndex = m_pItemInfo[i].dwItemIndex;
				m_dwItemCount++;
			}
			else
				m_pItemInfo[i].dwItemIndex = 0;
		}

		Packet *pPacket;
		int		packetLen = sizeof(MAIL_C_SEND_NEW_MAIL_Data) + strlen(m_szRecverName) + strlen(m_szSenderName) + strlen(m_szTitle) + strlen(m_szBody) + 3;
		BuildPacketSize(pPacket,MAIL,C_SEND_NEW_MAIL, szInBuff, packetLen);
		SET_DATA(pData,MAIL,C_SEND_NEW_MAIL,pPacket);
		pData->dwSenderUin	= m_miData.dwSenderUin;
		pData->dwMoney		= m_dwMoney;			// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
		pData->dwItemCount	= m_dwItemCount;		// 物品数量

		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			pData->dwItemIndex[i] = m_pItemInfo[i].dwItemIndex;
		}

		gfun_ReplaceStr(m_szTitle);
		gfun_ReplaceStr(m_szBody);
		pData->bPayFor		= m_bPayFor;			// 看是否付费取信
		char* szTemp		= pData->body; 
		strcpy(szTemp, m_szRecverName);
		szTemp			   += strlen(m_szRecverName)+1;
		strcpy(szTemp, m_szSenderName);
		szTemp			   += strlen(m_szSenderName)+1;
		strcpy(szTemp, m_szTitle);
		szTemp			   += strlen(m_szTitle)+1;
		strcpy(szTemp, m_szBody);


		pData->timeSend		= m_timeSend;				// 发送时间(中心服务器确定)
		pData->bVisited		= m_bVisited;				// 是否浏览过
		pData->bReturned	= m_miData.bReturned;		// 0表示原件;1是回件;系统邮件属于回件
		pData->dwMailID		= m_miData.dwMailID;
		pData->dwRecverUin	= m_miData.dwRecverUin;		// 收件uin 
		pData->timeOverdue	= m_timeOverdue;
		return pPacket;
	}

	void	GetMailIdentifier(MailIdentifier* pMIData)
	{
		pMIData->dwSenderUin	= m_miData.dwSenderUin;
		pMIData->dwRecverUin	= m_miData.dwRecverUin;
		pMIData->dwMailID		= m_miData.dwMailID;
		pMIData->bReturned		= m_miData.bReturned;
	}

	Packet *BuildReadMailRltPacket(char* szInBuff) // >>发件<< 1,成功,2,收件人为空,3标题为空,4.正文为空
	{
		Packet *pPacket;
		int		packetLen = sizeof(MAIL_GS_READ_MAIL_RLT_Data) + strlen(m_szBody);
		BuildPacketSize(pPacket,MAIL,GS_READ_MAIL_RLT, szInBuff, packetLen);
		SET_DATA(pData,MAIL,GS_READ_MAIL_RLT,pPacket);

		pData->miData.dwSenderUin	= m_miData.dwSenderUin;
		pData->miData.dwRecverUin	= m_miData.dwRecverUin;
		pData->miData.dwMailID		= m_miData.dwMailID;
		pData->miData.bReturned		= m_miData.bReturned;

		pData->dwMoney		= m_dwMoney;			// 钱(付费取信的时候,表示付的钱,否则就为寄过来的钱)
		pData->dwItemCount	= m_dwItemCount;		// 物品数量
		pData->bPayFor		= m_bPayFor;			// 看是否付费取信
		pData->timeOverdue	= m_timeOverdue;
		strcpy(pData->body, m_szBody);
		return pPacket;
	}

	Packet *BuildAddMailPacket(char* szInBuff) // >>发件<< 1,成功,2,收件人为空,3标题为空,4.正文为空
	{
		Packet *pPacket;
		int		packetLen = sizeof(MAIL_GS_ADD_MAIL_Data) + strlen(m_szBody);
		BuildPacketSize(pPacket,MAIL,GS_ADD_MAIL, szInBuff, packetLen);
		SET_DATA(pData,MAIL,GS_ADD_MAIL,pPacket);

		pData->miData.dwSenderUin	= m_miData.dwSenderUin;
		pData->miData.dwRecverUin	= m_miData.dwRecverUin;
		pData->miData.dwMailID		= m_miData.dwMailID;
		pData->miData.bReturned		= m_miData.bReturned;

		pData->bVisited				= m_bVisited;
		pData->timeSend				= m_timeSend;
		pData->timeOverdue			= m_timeOverdue;
		strcpy(pData->body, m_szBody);
		return pPacket;
	}

	bool	ReturnMail()
	{
		if(m_miData.bReturned)	return false;
		m_bVisited				= false;
		m_miData.bReturned		= true;
		// 付费的改为非付费
		if(m_bPayFor)
		{
			m_dwMoney = 0;
			m_bPayFor = false;
		}
	}

	bool	IsItemLoadReady()
	{
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			if(m_pItemInfo[i].byItemLoadResult != LMI_OK)
				return false;
		}
		return true;
	}

	int		AddMailItemIndex(int dwItemIndex )
	{
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			if(m_pItemInfo[i].dwItemIndex == 0)
			{
				m_pItemInfo[i].pItem = NULL;
				m_pItemInfo[i].dwItemIndex = dwItemIndex;
				m_pItemInfo[i].byItemLoadResult = LMI_NULL;
				m_dwItemCount++;
				return i;
			}
			else if(dwItemIndex!=-1&&dwItemIndex == m_pItemInfo[i].dwItemIndex)
			{//复制出来的物品客户端加保护
				return -1;
			}
		}
		return -1;
	}

	bool	AddMailItem(void* pItem,int dwItemIndex )
	{
		int i = AddMailItemIndex(dwItemIndex);
		if(i != -1)
		{
			m_pItemInfo[i].pItem = pItem;
			m_pItemInfo[i].byItemLoadResult = LMI_OK;		
			return true;
		}
		return false;
	}

	BYTE	GetItemByIndex(DWORD dwItemIndex) 
	{
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			if(dwItemIndex == m_pItemInfo[i].dwItemIndex)
			{
				break;
			}
		}
		return i;
	}

	bool	SetMailItemByIndex(void* pItem,int dwItemIndex)
	{
		int nPos = GetItemByIndex(dwItemIndex);
		if(nPos != SEND_MAIL_ITEM_MAX)
		{
			if(pItem == NULL)
			{
				m_pItemInfo[nPos].byItemLoadResult = LMI_NULL;
			}
			else
			{
				m_pItemInfo[nPos].pItem = pItem;
				m_pItemInfo[nPos].byItemLoadResult = LMI_OK;
			}
			return true;
		}
		return false;
	}

	bool	SetMailItemByPos(void* pItem,int nPos)
	{
		if(nPos < SEND_MAIL_ITEM_MAX)
		{
			if(m_pItemInfo[nPos].dwItemIndex)
			{
				if(pItem == NULL)
				{
					m_pItemInfo[nPos].byItemLoadResult = LMI_NULL;
				}
				else
				{
					m_pItemInfo[nPos].pItem = pItem;
					m_pItemInfo[nPos].byItemLoadResult = LMI_OK;
				}
				return true;
			}
		}
		return false;
	}

	void	SetMailItemIndex(DWORD dwItemIndex,DWORD nPos)
	{
		if(m_pItemInfo[nPos].dwItemIndex == 0)
		{
			m_pItemInfo[nPos].dwItemIndex = dwItemIndex;
			m_pItemInfo[nPos].byItemLoadResult = LMI_PRE;
			m_dwItemCount++;
		}
		else
		{
			m_pItemInfo[nPos].dwItemIndex = dwItemIndex;
			m_pItemInfo[nPos].byItemLoadResult = LMI_OK;
		}
	}
	bool	RemoveMailItemByIndex(int dwItemIndex)
	{
		int nPos = GetItemByIndex(dwItemIndex);
		if(nPos != SEND_MAIL_ITEM_MAX)
		{
			RemoveMailItemByPos(nPos);
			return true;
		}
		return false;
	}

	void	RemoveMailItemByPos(int nPos)
	{
		if(m_pItemInfo[nPos].dwItemIndex)
		{
			m_pItemInfo[nPos].dwItemIndex	= 0;	
			m_dwItemCount --;
		}
		m_pItemInfo[nPos].pItem				= NULL;
		m_pItemInfo[nPos].byItemLoadResult	= LMI_OK;
	}

	bool	UpdateOverdue(DWORD dwNowTick)
	{
		if(m_timeOverdue <= dwNowTick)
		{
			m_timeOverdue += 18000;
			return true;
		}
		return false;
	}
};

class CPlayerMailList
{
public:
	typedef DWORD											TYPE_UIN;
	typedef DWORD											TYPE_MAILID;
	typedef std::map< TYPE_MAILID,CPlayerMail* >			MAILLIST_MAILS;		// 如果不用指针,插入到map中的时候将会调用拷贝构造函数插入邮件列表中,方便就是他的内存分配都不用我管了
	// 假设有一个东西
	typedef std::map< TYPE_UIN,MAILLIST_MAILS* >			MAILLIST_PLAYERS;		// 发件人(非发送人)uin区分的邮件列表

	typedef std::map< TYPE_MAILID,CPlayerMail* >::iterator	MAILLIST_MAILS_ITOR;      
	typedef std::map< TYPE_UIN,MAILLIST_MAILS* >::iterator	MAILLIST_PLAYERS_ITOR; 

	MAILLIST_PLAYERS*				m_pMapMail;
	int								m_nUnvisitCnt;
	CPlayerMailList()	
	{
		m_pMapMail		= new MAILLIST_PLAYERS;
		m_nUnvisitCnt	= 0;
	}
	~CPlayerMailList()	
	{
		MAILLIST_PLAYERS_ITOR itor = m_pMapMail->begin();
		while(itor != m_pMapMail->end())
		{
			MAILLIST_MAILS*			pMailMap= itor->second;
			MAILLIST_MAILS_ITOR	itor0		= pMailMap->begin();
			while(itor0 != pMailMap->end())
			{
				delete itor0->second;
				pMailMap->erase(itor0++);
			}
			delete pMailMap;
			m_pMapMail->erase(itor++);
		}
		if(m_pMapMail)
			delete m_pMapMail;
	}

	CPlayerMail* AddMail( MAIL_GS_ADD_MAIL_Data* pMailInfo )		
	{
		CPlayerMail* pMail = new CPlayerMail(pMailInfo);
		return AddMail(pMail);
	}
	CPlayerMail* AddMail( MAIL_C_SEND_NEW_MAIL_Data* pMailData )		
	{
		CPlayerMail* pMail = new CPlayerMail(pMailData);
		return AddMail(pMail);
	}
	CPlayerMail* AddMail( CPlayerMail* pMail )		
	{// 插入邮件发起人列表中
		DWORD uin = pMail->m_miData.dwSenderUin;
		if(pMail->m_miData.bReturned)
			uin = pMail->m_miData.dwRecverUin;

		MAILLIST_PLAYERS_ITOR	itor = m_pMapMail->find(uin);
		MAILLIST_MAILS*			pSubMailMap = NULL;
		if(itor == m_pMapMail->end())
		{
			pSubMailMap = new MAILLIST_MAILS;
			m_pMapMail->insert(make_pair(uin,pSubMailMap));
		}
		else
		{
			pSubMailMap = itor->second;
		}

		// 把在内存中的先删除,放置内存泄露
		MAILLIST_MAILS_ITOR	itor0 = pSubMailMap->find(pMail->m_miData.dwMailID);
		if(itor0 != pSubMailMap->end())
		{
			delete itor0->second;
			pSubMailMap->erase(itor0);
		}

		pSubMailMap->insert(make_pair(pMail->m_miData.dwMailID,pMail));
		if(pMail->GetVisit() == false)
			m_nUnvisitCnt ++;
		return pMail;
	}

	bool RemoveMail(CPlayerMail* pMail)
	{// 确认信息:1.谁发给谁的 2.是否回件  false返回不存在邮件
		return RemoveMail(pMail->m_miData.dwSenderUin, pMail->m_miData.dwRecverUin, pMail->m_miData.dwMailID, pMail->m_miData.bReturned);
	}

	bool RemoveMail(MailIdentifier* pMIData)
	{// 确认信息:1.谁发给谁的 2.是否回件  false返回不存在邮件
		return RemoveMail(pMIData->dwSenderUin, pMIData->dwRecverUin, pMIData->dwMailID, pMIData->bReturned);
	}

	bool RemoveMail(DWORD dwSenderUin, DWORD dwRecverUin, DWORD dwMailID, bool bReturned)
	{// 确认信息:1.谁发给谁的 2.是否回件  false返回不存在邮件
		DWORD uin = dwSenderUin;
		if(bReturned)
			uin = dwRecverUin;
		MAILLIST_PLAYERS_ITOR itor = m_pMapMail->find(uin);
		MAILLIST_MAILS* pSubMailMap = NULL;
		if(itor == m_pMapMail->end())		return false;

		pSubMailMap = itor->second;
		MAILLIST_MAILS_ITOR itor0 = pSubMailMap->find(dwMailID);
		if(itor0 == pSubMailMap->end())	return false;

		CPlayerMail* pInfo = itor0->second;
		if(pInfo->GetVisit() == false)	
			m_nUnvisitCnt --;

		delete pInfo;
		pSubMailMap->erase(itor0);
		return true;
	}

	CPlayerMail* ExtractMail(DWORD dwSenderUin, DWORD dwRecverUin, DWORD dwMailID, bool bReturned)
	{// 确认信息:1.谁发给谁的 2.是否回件  false返回不存在邮件
		DWORD uin = dwSenderUin;
		if(bReturned)
			uin = dwRecverUin;
		MAILLIST_PLAYERS_ITOR itor = m_pMapMail->find(uin);
		MAILLIST_MAILS* pSubMailMap = NULL;
		if(itor == m_pMapMail->end())		return NULL;

		pSubMailMap = itor->second;
		MAILLIST_MAILS_ITOR itor0 = pSubMailMap->find(dwMailID);
		if(itor0 == pSubMailMap->end())	return NULL;

		CPlayerMail* pInfo = itor0->second;
		if(pInfo->GetVisit() == false)	
			m_nUnvisitCnt --;

		pSubMailMap->erase(itor0);
		return pInfo;
	}

	CPlayerMail* GetMail(MailIdentifier* pMIData)
	{
		DWORD uin = pMIData->dwSenderUin;

		MAILLIST_PLAYERS_ITOR itor = m_pMapMail->find(uin);
		if(itor == m_pMapMail->end())		return NULL;

		MAILLIST_MAILS* pSubMailMap = NULL;
		pSubMailMap = itor->second;
		MAILLIST_MAILS_ITOR itor0 = pSubMailMap->find(pMIData->dwMailID);
		if(itor0 == pSubMailMap->end())	
			return NULL;

		CPlayerMail* pInfo = itor0->second;
		return pInfo;
	}

	MAILLIST_MAILS* GetPlayerMails(DWORD dwUin)
	{
		MAILLIST_PLAYERS_ITOR	itor		= m_pMapMail->find(dwUin);
		if(itor == m_pMapMail->end())		return NULL;
		return itor->second;
	}

	CPlayerMail* SetVisit(MailIdentifier* pMIData)
	{
		CPlayerMail* pInfo = GetMail(pMIData);
		if(pInfo && pInfo->GetVisit() == false)
		{
			pInfo->SetVisit();
			m_nUnvisitCnt --;
		}
		return pInfo;
	}

	int			GetUnvisitCnt()
	{
		return m_nUnvisitCnt;
	}

	// 遍历
	CPlayerMail* GetFirstMail()
	{
		m_itorMails	= m_pMapMail->begin();
		while(m_itorMails != m_pMapMail->end())
		{
			m_itorMail = m_itorMails->second->begin();
			if(m_itorMail != m_itorMails->second->end())
			{
				CPlayerMail* pPlayerMail = (m_itorMail ++)->second;
				return pPlayerMail;
			}
			m_itorMails++;
		}
		return NULL;
	}
	CPlayerMail* GetNextMail()
	{
		while(m_itorMails != m_pMapMail->end())
		{
			if(m_itorMail != m_itorMails->second->end())
			{
				CPlayerMail* pPlayerMail = (m_itorMail ++)->second;
				return pPlayerMail;
			}
			m_itorMails++;
			if(m_itorMails != m_pMapMail->end())
				m_itorMail =  m_itorMails->second->begin();
		}
		return NULL;
	}

	MAILLIST_PLAYERS_ITOR	m_itorMails;
	MAILLIST_MAILS_ITOR		m_itorMail;

};

struct MAIL_GS_SEND_MAIL_ITEM_CARVE_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	ItemCarveInfo	xItemInfo;
};

struct MAIL_GS_SEND_MAIL_ITEM_HORSEEQ_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	HorseEqInfo		xItemInfo;
};

struct MAIL_GS_SEND_MAIL_ITEM_ENCHASE_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	EnchaseExInfo		xItemInfo;
};



struct MAIL_GS_SEND_MAIL_ITEM_WING_Data
{
	MailIdentifier	miData;
	DWORD			dwItemIndex;
	DWORD	dwTypeBit;
	BYTE	bSlot;
	BYTE	bStepLv;
	int		nStepPow;
	BYTE	bQuality;		//品质
	int		nRepairTime;	//修复次数
	DWORD	dwQuaVal[4];	//4项属性
};

#endif