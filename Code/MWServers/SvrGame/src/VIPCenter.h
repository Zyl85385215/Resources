#pragma once
#include "VipConfig.h"


class UserData;
class VIPCenter
{
public:
	VIPCenter(UserData* pUser);
	~VIPCenter();
	void	Reflesh(BYTE byLv);
	void	AddVIPRMB(int nVal);
	void	SendWorldMessage(bool ifJoin);
	int		GetExpAdd(int nExp);			
	int		GetTaxSub(){};
	int		GetHonourAdd(int nHonour);
	int		GetExFields();
	int		GetExCards(){};
	int		GetChatVal();
	bool	CheckFixSwitch();
	bool	CheckBuySwitch();
	void	RequestForGift(int nLv);
	void	RequestGetPower();
	
	int		GetVipLv();

	void    SendChgAutoRight(int nVipLV);

	void    CheckAddVipState();
private:
	VipInfoObj* m_pObj;
	UserData* m_pParent;
};