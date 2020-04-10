#pragma once

#include "xnThread.h"
#include <map>


class HttpCenter
{
public:
	HttpCenter();
	~HttpCenter();

	void	SendToMainAcc(char*	szMsg);
	void	SendToChildAcc(char*	szMsg,int nSvr);
	int		RecvHttpMsg(DWORD dwMark, char*	szBuf,int nLen);

	bool	TPLogin(char* szUin,char*	szToken, BYTE nTPlat, OUT char*	szAcc,OUT char* szPswd,OUT char* szBKey);

	BYTE	TPCheckIAPPay(char* szAcc,char*	szToken);

	xnThread*	m_pHttpThread[4];

	std::map<DWORD,char*>	mapHttpCache;


private:

	bool	SKPayDeal(char*	szParam);
	bool	KYPayDeal(char*	szParam);
	bool	PPSPayDeal(char*	szParam);
	bool	IToolPayDeal(char*	szParam);
	bool	TP91PayDeal(char*	szParam);

	bool	TBTPayDeal(char*	szParam);
	bool	UCPayDeal(char* szParam);
	bool	ABPayDeal(char*	szParam);
	bool	DJoyPayDeal(char* szParam);
	bool	_checkABOk(char *sKey,char *pTransData,char *pSign);

	bool	TP91AdrDeal(char*	szParam);
	bool	TPOppoAdrDeal(char*	szParam);
	bool	TPBDDKAdrDeal(char*	szParam);
	bool	TPAdr360Deal(char*	szParam);
	bool	TPAdrLXDeal(char*	szParam);
	bool	TPAdrAZDeal(char*	szParam);
	bool	TPAdrMiDeal(char*	szParam);
	bool	TPAdrDJoyDeal(char* szParam);
	bool	TPAdrWDJDeal(char* szParam);
	bool	TPAdrHWDeal(char* szParam);
	bool	TPAdrMZDeal(char* szParam);
	bool	TPAdrMZWDeal(char* szParam);
	bool	TPAdrPPSDeal(char* szParam);

	bool	TPAdrJFDeal(char* szParam);
	bool	TPAdrMMYDeal(char* szParam);
	bool	TPAdrYYHDeal(char* szParam);
	bool    TPAdrYLDeal(char* szParam);

	//----------------------------------
	bool	DianRuCheck(char* szParam);
	bool	YouMiCheck(char* szParam);
	bool	AdsageCheck(char* szParam);
	bool	DuoMengCheck(char* szParam);
	bool	WanPuCheck(char* szParam);
	bool	LiMeiCheck(char* szParam);
	bool	InviteDeal(char*	szParam);
};