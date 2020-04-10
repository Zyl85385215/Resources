#pragma once

class IosThirdPlat{
public:
    IosThirdPlat();
    ~IosThirdPlat();
   
	const char*		TP_GetMacAddress();
	const char*			TP_GetDevID();
	void			TP_Update();
	int				TP_GetPlat();
	bool			TP_OPenCenter();
	//登录部分的信息
	bool			TP_Login();
	int		nLoginStep;		//0没开启,1平台准备登陆 2平台登录成功, 3正在进入, 
	int		nLoginMID;		
	char*	szToken;		//

	//支付部分的信息
	virtual	void	TP_RqPay(int nVal,int nSvrID, char* szAcc, char* szOrder);
	int		nPayStep;		//1支付完成
	char*	szOrder_No;		//订单号,用于服务器超时验证查询等
	int		nMoney;
};