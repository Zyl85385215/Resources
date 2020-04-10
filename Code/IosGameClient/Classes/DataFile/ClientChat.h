#pragma once
#include "ios_rpgchat_packet.h"
#include "xnlist.h"
#include "PacketCenter.h"

#define MAX_MSG_INPAGE 35
#define MAX_ITEM_SAVE 30

enum
{
	MSGLIST_SYS,
	MSGLIST_WORLD,
	MSGLIST_COUNTRY,
	MSGLIST_MAP,
	MSGLIST_HOME,
	MSGLIST_TEAM,
	MSGLIST_P2P,
	MSGLIST_TOTAL,
	MSGLIST_MAX,
};

struct ChatInfo
{
	int		nCnt;
	char	szSName[20];
	int		nSUin;
	char	szRName[20];
	int		nRUin;

	//聊天界面要用，新的参数全往后加，前面的参数绝对不能改，也不能换次序
	BYTE	byCRType;
	BYTE	bySV;
	BYTE	byRV;
	ShowItemSimpleInfo	xInfo[5];
	char	szMsg[512];
};

struct SystemMsg
{
	BYTE byType;		//公告类型
	char szMsg[512];
	int nTimes;			//次数
};

struct ClientShowInfo
{
	DWORD dwShowid;
	ItemObj* pObj;
};

class ChatCenter : public PacketListener
{
public:
	ChatCenter();
	~ChatCenter();
	bool PacketProcess(ZPacket* pPacket);
	void AddMsg(RPGCHAT_S_SEND_MESSAGE_Data* pMsg);
	void AddSimplemsg(char * pMsg);
	void AddShowItem(ShowItemInfo* pInfo);
	void SendMsg(BYTE byType,char* pMsg,int* szId,int nCnt);
	ClientShowInfo* GetShowInfo(int nID);
	void RequestForItemInfo(int nID);
	void AddSystemMsg(char* szMsg,BYTE byType,int nCnt);

	void SaveSystemSet();
	void ClearList();

	xnList* m_pMsgList[MSGLIST_MAX];		//各个频道信息
	bool	m_ShieldSwitch[MSGLIST_MAX];		//消息屏蔽开关，true时不接收信息
	xnList* m_pShowList;				//聊天信息中的物品信息
	xnList* m_pSimMsgList;				//屏幕下方的小框中消息
	xnList*	m_pSystemMsg;				//公告信息
	xnList*	m_pWarSysMsg;				//杀人公告
	bool m_bifLineChg;
	bool m_ifSimpleLineChg;
};