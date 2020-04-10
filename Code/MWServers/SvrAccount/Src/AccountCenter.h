#pragma once


#include "DBCreate.h"
#include "ZNetPacket.h"
#include "ios_ac_packet.h"
#include "MarkDBCenter.h"

enum TP_NUMBER
{
	TP_NULL		= 0,
	TP_IAP	= 10,
	
	TP_91PAY	= 12,
	TP_PPSPAY	= 13,
	TP_TBTPAY	= 14,
	TP_KYPAY	= 15,
	TP_DJOYPAY	= 16,		//����
	TP_ITOOLPAY	= 17,
	TP_ADRUC	= 18,
	TP_ABPAY	= 21,		///10%2 ȷ���Ƿ����˺�ϵͳ �ڴ�
	TP_ABPAYPG	= 22,		

	TP_SKPAY	= 25,		//
	TP_SKPAY1	= 26,		//
	TP_SKPAY2	= 27,		//
	TP_SKPAY3	= 28,
	TP_LW		= 29,		//����

	TP_ADR91	= 31,
	TP_ADROPPO	= 32,
	TP_ADRBDDK	= 33,
	TP_ADRMI	= 34,
	TP_ADRLX	= 35,
	TP_ADR360	= 36,
	TP_ADRAZ	= 37,

	TP_ADRDJOY	= 38,
	TP_ADRWDJ	= 39,

	TP_ADRHUAWEI	= 51,
	TP_ADRMZ		= 52,
	TP_ADRPPS		= 53,
	TP_ADRMZW		= 54,

	TP_ADRJF		= 55,
	TP_ADRMMY		= 56,
	TP_ADRYYH		= 57,
	TP_ADRYL   = 58,

	TP_ADWLQ	= 60,	//����Ȥ
	TP_ADYYSC	= 61,	//�����г�
	TP_ADBLXQDL	= 62,	//������� ������̨
};

class _errIAPToken
{
public:
	char*	szAcc;
	char*	szToken;
	int		nTime;
	DWORD	dwTickCheck;
};

class AccountCenter
{
public:
	AccountCenter();
	~AccountCenter();

	void	InitDB();
	void	InitNet();

	void	Update();
	void	Update25();
	void	DealPacket();
	void	RecheckAppPay();

	void	TTTTTTTTTTT();

	void	RecvPacket(ZPacket* pPacket);
	bool	HttpPayRequest(char* szAcc, char* szOrder,int nMoney,int nTPID);
	int		GetRmbByPayMoney(int nPay);
	void	RecvInviteCmd(int nFrmSvr,char*	szInvCode,BYTE bEvent);


	void	RecvIosAD(char* szMac, char* szIDFA, char* szTP, char* szEx);
	bool	CheckIosAD(char* szMac, char* szIDFA,DBIO* pDB);

	xnThread*	ppThread[20];
	int		m_nCreateCnt;
	int		m_nLoginCnt;
	bool	m_bIsMainAcc;
	bool	m_bIsLockNewAcc;

	CRITICAL_SECTION m_xPacketLock;
	xnList*		m_pListPacket;

	CRITICAL_SECTION m_xLockIAP;
	xnList*		m_pLErrorIAP;

	MarkDBCenter*	m_pMarkDBCenter;

	BYTE	GetChooseCountry();		//����������Ӫ
private:

	bool	_processAccountPacket(ZPacket* pPacket);


	//��ҵ�½, bReg��ʾ�˺Ų����ڵĻ�ֱ��ע��
	int		PlayerLogin(char* szAcc, char* szPswd, BYTE bReg,int & accUin,BYTE byTP,char* szMac,int nSysVer=0,char*	szIDFA="",char* szIP = "");
	int		ChgPassword(char* szAcc, char* szPswdOld, char* szPswdNew);
	void	_FillSimpleUInfos(int nAccUin, ACCOUNT_SIMPLEINFOS_Data* pSIData);

	bool	_checkADMacIDFA(char* szMac, char* szIDFA);
	CreateDB*	m_pDBCreate;
	LogDB*		m_pLogDB;
};

extern AccountCenter* g_pMainAccount;