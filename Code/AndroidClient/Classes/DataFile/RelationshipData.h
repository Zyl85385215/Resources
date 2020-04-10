#pragma once
#include "ios_relation_packet.h"
#include "xnlist.h"
#include "PacketCenter.h"

class MentoringClient	: public PacketListener
{
public:
	MentoringClient();
	~MentoringClient();
	bool PacketProcess(ZPacket* pPacket);
	void	AddData(MCData* pData);
	void	UpdateData(MCData* pData);
	void	DeleteData(int nID);
	void	AddRequest(MCRequest* pData);
	void	DeleteRequest(int nID);
	void	AddMyRequest(int nUin);
	void	DeleteMyRequest(int nUin);
	bool	ClearList();
	bool	CheckTeacherUin(int nUin);


	void	RequestTeacherList();
	void	RequestInstruct();
	void	RequestStudy(int nPos);
	void	RequestSendItem(int nPos);
	void	RequestGetItem(int nPos);
	void	RequestBreakRelation(int nPos);
	void	RequestDealWith(int nPos,bool ifOK);

	void	RequestDealWithST(DWORD dwStuUin, bool ifOK);
	void	RequestMsg(DWORD dwStuUin);
	void	ShowMentorAsk( char* szName, int nSFUin );
	bool	CheckGift();

	xnList*	m_pTeacherList;
	xnList*	m_pRelationList;
	xnList* m_pRequestList;
	xnList*	m_pMyRequestList;
	int		m_nTListFlag;
	bool	m_ifInstruct;
	bool	m_ifShow;

	bool	m_ifChg;
	bool	m_ifTChg;
	bool	m_ifRChg;
};

class MarryClient	: public PacketListener
{
public:
	MarryClient();
	~MarryClient();
	bool	PacketProcess(ZPacket* pPacket);
	void	AddData(ClientMarryData* pData);
	void	UpdateData(ClientMarryData* pData);
	void	DeleteData(int nID);
	bool	ClearList();
	ClientMarryData*	GetRelation();


	void	GetRequestList();
	void	RequestInstruct();
	void	CancelInstruct();
	void	RequestBreakRelation();
	void	RequestUpSkill(BYTE byPos);
	void	SendRing(BYTE byPos);
	void	ShowMarryTitle(bool ifShow);
	
	void	ShowMarryAsk(int nUin,char* szName);
	void	ShowGetmarriedAsk(char* szName);
	void	AnswerMarry(int nUin,bool ifAccept);

	void	SendActionEvent(BYTE byType);

	xnList*	m_pRelationList;
	xnList* m_pRequestList;
	int		m_nRListFlag;
	bool	m_ifInstruct;
	bool	m_ifShowTitle;

	bool	m_ifChg;
	bool	m_ifRChg;

	char		m_szMsgTitle[22];
	char		m_szMsgDetail[128];
	bool		m_bMarryRingRequest;
};