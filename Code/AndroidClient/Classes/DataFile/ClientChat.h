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

	//�������Ҫ�ã��µĲ���ȫ����ӣ�ǰ��Ĳ������Բ��ܸģ�Ҳ���ܻ�����
	BYTE	byCRType;
	BYTE	bySV;
	BYTE	byRV;
	ShowItemSimpleInfo	xInfo[5];
	char	szMsg[512];
};

struct SystemMsg
{
	BYTE byType;		//��������
	char szMsg[512];
	int nTimes;			//����
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

	xnList* m_pMsgList[MSGLIST_MAX];		//����Ƶ����Ϣ
	bool	m_ShieldSwitch[MSGLIST_MAX];		//��Ϣ���ο��أ�trueʱ��������Ϣ
	xnList* m_pShowList;				//������Ϣ�е���Ʒ��Ϣ
	xnList* m_pSimMsgList;				//��Ļ�·���С������Ϣ
	xnList*	m_pSystemMsg;				//������Ϣ
	xnList*	m_pWarSysMsg;				//ɱ�˹���
	bool m_bifLineChg;
	bool m_ifSimpleLineChg;
};