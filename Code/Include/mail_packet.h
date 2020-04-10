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
	C_SEND_NEW_MAIL,			// ����
	CS_SEND_NEW_MAIL_RLT,		// ������Ӧ

	C_READ_MAIL,				// ����
	GS_READ_MAIL_RLT,			// �����ظ�

	C_REMOVE_MAIL,				// ɾ��
	CS_REMOVE_RLT,				// ɾ����Ӧ

	CS_NOTIFY_HAS_MAIL,         // �����ʾ�����ʼ�

	CS_ADD_MAIL,				// �����������ˣ�����ʼ����ʼ���Ʒ��
	GS_ADD_MAIL,				// ����Ҫ��Ϣ��װ���ʰ������ʼ����ͻ���

	C_GET_MAILLIST,				// ��������ķ�������ȡ�ʼ��б�
	GS_GET_MAILLIST,			// ������uin�������ķ�������ȡ�ʼ��б�

	CS_SEND_SYSTEM_MAIL,		// �����ķ���������Ϸ����������ϵͳ�ʼ���,����GM����

	GS_OPEN_MAIL_BOX,			// ������,��ʱ�����ڽű�,�������NPC����������б�

	// *********************    ##��Ʒ���##    **************
	C_RETURN_MAIL,				// �ؼ�
	C_TAKE_MAIL_ITEM,			// ����Ʒ
	C_TAKE_MAIL_ITEMS,			// �ó�������Ʒ
	C_TAKE_MAIL_MONEY,			// ��Ǯ

	CS_RETURN_RLT,				// �ؼ����
	CS_TAKE_MAIL_ITEM_RLT,		// ����Ʒ,��Ӧ
	CS_TAKE_MAIL_ITEMS_RLT,		// �ó�������Ʒ,��Ӧ
	CS_TAKE_MAIL_MONEY_RLT,		// ��Ǯ.��Ӧ

	GS_SEND_MAIL_ITEM,
	GS_SEND_MAIL_ITEM_EXTEND,	
	GS_SEND_MAIL_ITEM_BINDINFO,
	GS_SEND_MAIL_ITEM_JUDGE,
	GS_SEND_MAIL_ITEM_GEMS,

	GS_SEND_MAX_ID,

	GET_WAGE_MAIL,				// ��ù�����

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
	DWORD	dwMailID;			// �ʼ���� 
	DWORD	dwSenderUin;		// ����uin,0 ��ʾϵͳ
	DWORD	dwRecverUin;		// �ռ�uin 
	DWORD	dwMoney;			// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
	DWORD	dwItemCount;		// ��Ʒ����
	DWORD	dwItemIndex[SEND_MAIL_ITEM_MAX];
	DWORD	timeSend;			// ����ʱ��
	DWORD	timeOverdue;		// ����ʱ��
	DWORD   dwPayItemIndex;		// �����ĸ���Ʒ
	bool	bVisited;			// �Ƿ������
	bool	bReturned;			// 0��ʾԭ��;1�ǻؼ�;ϵͳ�ʼ����ڻؼ�
	bool	bPayFor;			// ���Ƿ񸶷�ȡ��
	char	body[1];
//	char *	m_szRecverName;		// �ռ�������
// 	char *	szSenderName;		// ����������
// 	char *	szTitle;			// ����
// 	char *	szBody;				// ����
}
MAIL_CS_ADD_MAIL_Data;

struct MAIL_CS_SEND_SYSTEM_MAIL_Data
{
	BYTE	byPayType;
	DWORD dwRecvUin;
	DWORD dwMoney;
	DWORD dwItemID[SEND_MAIL_ITEM_MAX];
	char  szBody[1];
// 	char *	szTitle;			// ����
// 	char *	szBody;				// ����
};

typedef struct MailIdentifier
{
	DWORD	dwRecverUin;		// �ҵ��ռ���
	DWORD	dwSenderUin;
	DWORD	dwMailID;
	bool	bReturned;			// ��ȡ������,False ������ΪdwSenderUin,true ������ΪdwRecverUin,ȡ��Ʒ��
}
MAIL_C_READ_MAIL_Data,
MAIL_C_REMOVE_MAIL_Data,
MAIL_C_RETURN_MAIL_Data,
MAIL_C_TAKE_MAIL_ITEMS_Data,
MAIL_C_TAKE_MAIL_MONEY_Data;

typedef struct MAIL_CS_REMOVE_RTL_Data
{
	MailIdentifier	miData;
	BYTE			byCode;				// 0��ʾ�ɹ�
}
MAIL_CS_TAKE_MAIL_ITEMS_RLT_Data,
MAIL_CS_TAKE_MAIL_MONEY_RLT_Data,
MAIL_CS_SEND_NEW_MAIL_RLT_Data,
MAIL_CS_REMOVE_RLT_Data,			// ɾ�����
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
	bool			bVisited;			// �Ƿ������ 
	char			body[1];
// 	char *			szSenderName;		// ����������		
// 	char *			szTitle;			// ����
};

struct MAIL_GS_READ_MAIL_RLT_Data
{
	MailIdentifier	miData;
	DWORD			dwMoney;			// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
	DWORD			dwItemCount;		// ��Ʒ����
	DWORD			timeOverdue;
	bool			bPayFor;			// ���Ƿ񸶷�ȡ��
	char			body[1];
// 	char *			szBody;				// ����
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
	DWORD	dwRecverUin;		// �ҵ��ռ���
	DWORD	dwVal;				// ����ֵ	
};

#pragma pack (pop)

enum
{// LOAD MAIL ITEM RESULT
	LMI_PRE,		// ׼��
	LMI_OK,			// ���
	LMI_NULL,		// ��Ʒ�ǿյ�
};

struct MailItemInfo
{
	DWORD	dwItemIndex;
	void*	pItem;
	BYTE	byItemLoadResult;
};

#define UNVISIT_TIME_PERSIST 604800	// 7 ��   δ���ʼ�����ʱ��
#define VISITED_TIME_PERSIST 86400	// 1 ��   �Ѷ��ʼ�����ʱ��

class CPlayerMailList;
class CPlayerMail
{
public:
 	MailIdentifier		m_miData;
private:
	DWORD				m_dwMoney;							// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
	DWORD				m_dwItemCount;						// ��Ʒ����
	bool				m_bPayFor;							// ���Ƿ񸶷�ȡ��
	char *				m_szSenderName;						// ����������		
	char *				m_szRecverName;						// �ռ�������	

	char *				m_szTitle;							// ����
	char *				m_szBody;							// ����

	DWORD				m_timeSend;							// ����ʱ��
	DWORD				m_timeOverdue;							// ����ʱ��
	bool				m_bRead;							// �ͻ��� ��ʾ������,���Դӻ����ȡ���ż���Ϣ
	bool				m_bVisited;							// �Ƿ������

	MailItemInfo		m_pItemInfo[SEND_MAIL_ITEM_MAX];	// ������Ʒ
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

	// �ͻ���д��,��������
	CPlayerMail(DWORD dwSenderUin,char * szSenderName)// �����˵Ĺ��캯��,��������ʼ���˭�����
	{
		memset(this, 0, sizeof(CPlayerMail));
		ResetSenderMail();
		m_miData.dwSenderUin = 0;
		m_szSenderName		 = NULL;
		SetSender(dwSenderUin, szSenderName);
	}

	// �ͻ����ռ�
	CPlayerMail(MAIL_GS_ADD_MAIL_Data* pMailInfo)
	{
		m_miData.dwMailID		= pMailInfo->miData.dwMailID;				// �ʼ���� 
		m_miData.dwSenderUin	= pMailInfo->miData.dwSenderUin;			// ����uin
		m_miData.dwRecverUin	= pMailInfo->miData.dwRecverUin;			// �ռ�uin 
		m_bVisited				= pMailInfo->bVisited;						// �Ƿ������
		m_miData.bReturned		= pMailInfo->miData.bReturned;				// 0��ʾԭ��;1�ǻؼ�;ϵͳ�ʼ����ڻؼ�
		m_timeSend				= pMailInfo->timeSend;
		m_szSenderName			= strdup(pMailInfo->body);					// ����������
		int NameLen				= strlen(pMailInfo->body) + 1;
		m_szTitle				= strdup(pMailInfo->body + NameLen);		// ����

		gfun_ReplaceStr(m_szTitle);
		m_szBody				= NULL;										// ����
		m_szRecverName			= NULL;
		m_timeOverdue			= pMailInfo->timeOverdue;
		m_dwMoney				= 0;										// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
		m_bPayFor				= false;									// ���Ƿ񸶷�ȡ��
		m_dwItemCount			= 0;										// ��Ʒ����
		m_bRead					= false;
		memset(m_pItemInfo,0,sizeof(MailItemInfo)*SEND_MAIL_ITEM_MAX);
		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			m_pItemInfo[i].byItemLoadResult = LMI_OK;	
		}
	}

	// ���ķ�����,��Ϸ�������ռ�
	CPlayerMail(MAIL_C_SEND_NEW_MAIL_Data* pMailData)
	{
		m_miData.dwMailID		= pMailData->dwMailID;				// �ʼ���� 
		m_miData.dwSenderUin	= pMailData->dwSenderUin;			// ����uin
		m_miData.dwRecverUin	= pMailData->dwRecverUin;			// �ռ�uin 
		m_dwMoney				= pMailData->dwMoney;				// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
		m_dwItemCount			= pMailData->dwItemCount;			// ��Ʒ����
		m_bVisited				= pMailData->bVisited;				// �Ƿ������
		m_miData.bReturned		= pMailData->bReturned;				// 0��ʾԭ��;1�ǻؼ�;ϵͳ�ʼ����ڻؼ�
		m_bPayFor				= pMailData->bPayFor;				// ���Ƿ񸶷�ȡ��

		m_szRecverName			= strdup(pMailData->body);

		int NameLen				= strlen(m_szRecverName) + 1;
		m_szSenderName			= strdup(pMailData->body + NameLen);			// ����������

		NameLen					+= strlen(m_szSenderName) + 1;
		m_szTitle				= strdup(pMailData->body + NameLen);	// ����

		NameLen					+= strlen(m_szTitle) + 1;
		m_szBody				= strdup(pMailData->body + NameLen);								// ����

		gfun_ReplaceStr(m_szBody);
		gfun_ReplaceStr(m_szTitle);

		m_timeSend				= pMailData->timeSend;
		// ������Ҫװ���λ
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
		m_miData.dwMailID		= 0;					// �ʼ���� 
		m_miData.dwSenderUin	= 0;					// ����uin
		m_miData.dwRecverUin	= 0;					// �ռ�uin 
		m_dwMoney				= 0;					// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
		m_dwItemCount			= 0;					// ��Ʒ����
		m_timeSend				= 0;					// ����ʱ��
		m_bVisited				= false;				// �Ƿ������
		m_bPayFor				= false;				// ���Ƿ񸶷�ȡ��
		m_miData.bReturned		= false;				// 0��ʾԭ��;1�ǻؼ�;ϵͳ�ʼ����ڻؼ�
		m_szSenderName			= strdup("");			// ����������
		m_szTitle				= strdup("");			// ����
		m_szBody				= strdup("");			// ����
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
		m_miData.dwSenderUin	= dwSenderUin;			// ����uin
		SetSenderName(szSenderName);					// ����������
	}

	void	ResetSenderMail()
	{
		m_miData.dwMailID		= 0;					// �ʼ���� 
		m_miData.dwRecverUin	= 0;					// �ռ�uin 
		m_dwMoney				= 0;					// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
		m_dwItemCount			= 0;					// ��Ʒ����
		m_timeSend				= 0;					// ����ʱ��
		m_bVisited				= false;				// �Ƿ������
		m_miData.bReturned		= false;				// 0��ʾԭ��;1�ǻؼ�;ϵͳ�ʼ����ڻؼ�
		m_bPayFor				= false;				// ���Ƿ񸶷�ȡ��
		SAFE_FREE_EX(m_szTitle);
		SAFE_FREE_EX(m_szBody);
		SAFE_FREE_EX(m_szRecverName);
		m_szTitle				= strdup("");			// ����
		m_szBody				= strdup("");			// ����
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

	Packet *BuildSendMailPacket(char* szInBuff) // >>����<< 1,�ɹ�,2,�ռ���Ϊ��,3����Ϊ��,4.����Ϊ��
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
		pData->dwMoney		= m_dwMoney;			// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
		pData->dwItemCount	= m_dwItemCount;		// ��Ʒ����

		for(int i=0; i<SEND_MAIL_ITEM_MAX; i++)
		{
			pData->dwItemIndex[i] = m_pItemInfo[i].dwItemIndex;
		}

		gfun_ReplaceStr(m_szTitle);
		gfun_ReplaceStr(m_szBody);
		pData->bPayFor		= m_bPayFor;			// ���Ƿ񸶷�ȡ��
		char* szTemp		= pData->body; 
		strcpy(szTemp, m_szRecverName);
		szTemp			   += strlen(m_szRecverName)+1;
		strcpy(szTemp, m_szSenderName);
		szTemp			   += strlen(m_szSenderName)+1;
		strcpy(szTemp, m_szTitle);
		szTemp			   += strlen(m_szTitle)+1;
		strcpy(szTemp, m_szBody);


		pData->timeSend		= m_timeSend;				// ����ʱ��(���ķ�����ȷ��)
		pData->bVisited		= m_bVisited;				// �Ƿ������
		pData->bReturned	= m_miData.bReturned;		// 0��ʾԭ��;1�ǻؼ�;ϵͳ�ʼ����ڻؼ�
		pData->dwMailID		= m_miData.dwMailID;
		pData->dwRecverUin	= m_miData.dwRecverUin;		// �ռ�uin 
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

	Packet *BuildReadMailRltPacket(char* szInBuff) // >>����<< 1,�ɹ�,2,�ռ���Ϊ��,3����Ϊ��,4.����Ϊ��
	{
		Packet *pPacket;
		int		packetLen = sizeof(MAIL_GS_READ_MAIL_RLT_Data) + strlen(m_szBody);
		BuildPacketSize(pPacket,MAIL,GS_READ_MAIL_RLT, szInBuff, packetLen);
		SET_DATA(pData,MAIL,GS_READ_MAIL_RLT,pPacket);

		pData->miData.dwSenderUin	= m_miData.dwSenderUin;
		pData->miData.dwRecverUin	= m_miData.dwRecverUin;
		pData->miData.dwMailID		= m_miData.dwMailID;
		pData->miData.bReturned		= m_miData.bReturned;

		pData->dwMoney		= m_dwMoney;			// Ǯ(����ȡ�ŵ�ʱ��,��ʾ����Ǯ,�����Ϊ�Ĺ�����Ǯ)
		pData->dwItemCount	= m_dwItemCount;		// ��Ʒ����
		pData->bPayFor		= m_bPayFor;			// ���Ƿ񸶷�ȡ��
		pData->timeOverdue	= m_timeOverdue;
		strcpy(pData->body, m_szBody);
		return pPacket;
	}

	Packet *BuildAddMailPacket(char* szInBuff) // >>����<< 1,�ɹ�,2,�ռ���Ϊ��,3����Ϊ��,4.����Ϊ��
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
		// ���ѵĸ�Ϊ�Ǹ���
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
			{//���Ƴ�������Ʒ�ͻ��˼ӱ���
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
	typedef std::map< TYPE_MAILID,CPlayerMail* >			MAILLIST_MAILS;		// �������ָ��,���뵽map�е�ʱ�򽫻���ÿ������캯�������ʼ��б���,������������ڴ���䶼�����ҹ���
	// ������һ������
	typedef std::map< TYPE_UIN,MAILLIST_MAILS* >			MAILLIST_PLAYERS;		// ������(�Ƿ�����)uin���ֵ��ʼ��б�

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
	{// �����ʼ��������б���
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

		// �����ڴ��е���ɾ��,�����ڴ�й¶
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
	{// ȷ����Ϣ:1.˭����˭�� 2.�Ƿ�ؼ�  false���ز������ʼ�
		return RemoveMail(pMail->m_miData.dwSenderUin, pMail->m_miData.dwRecverUin, pMail->m_miData.dwMailID, pMail->m_miData.bReturned);
	}

	bool RemoveMail(MailIdentifier* pMIData)
	{// ȷ����Ϣ:1.˭����˭�� 2.�Ƿ�ؼ�  false���ز������ʼ�
		return RemoveMail(pMIData->dwSenderUin, pMIData->dwRecverUin, pMIData->dwMailID, pMIData->bReturned);
	}

	bool RemoveMail(DWORD dwSenderUin, DWORD dwRecverUin, DWORD dwMailID, bool bReturned)
	{// ȷ����Ϣ:1.˭����˭�� 2.�Ƿ�ؼ�  false���ز������ʼ�
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
	{// ȷ����Ϣ:1.˭����˭�� 2.�Ƿ�ؼ�  false���ز������ʼ�
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

	// ����
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
	BYTE	bQuality;		//Ʒ��
	int		nRepairTime;	//�޸�����
	DWORD	dwQuaVal[4];	//4������
};

#endif