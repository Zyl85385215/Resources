#pragma once

class IosThirdPlat{
public:
    IosThirdPlat();
    ~IosThirdPlat();
   
	const char*		TP_GetMacAddress();	//获取MAC地址,7.0以上为固定空值
	int				TP_GetSystemVersion();	//IOS版本号, float*10000
	const char*		TP_GetIDFA();		//获取IDFA,6.0以下为空
	const char*			TP_GetDevID();
	void			TP_Update();
	int				TP_GetPlat();
	bool			TP_OPenCenter();
	int				TP_GetCenterType();
	//登录部分的信息
	bool			TP_Login();
	int		nLoginStep;		//0没开启,1平台准备登陆 2平台登录成功, 3正在进入, 
	int		nLoginMID;		
	char*	szToken;		//

	//支付部分的信息
	virtual	void	TP_RqPay(int nVal,int nSvrID, char* szAcc, char* szOrder);
	int		nPayStep;		//1支付完成  2IAP成功, 9失败
	const char*	szOrder_No;		//订单号,用于服务器超时验证查询等
	int		nMoney;

	void		TP_OtherEvent(int nType,char* szUrl); 
	int			nEventMark;

	int		nLockInUpdate;
};

extern char*	g_szAPNToken;
void	gfun_TPResetVersionFile();		//清理资源文件
char*	gfun_TPOpenVersionUrl(char*	szUrl,void* pData, int& nSize);
bool	gfun_TPDownZipFile(char*	szUrl,char*	szSaveName);
bool	gfun_TPUnZipFile(char*	szName);

//void	gfun_TPResetZipFileCnt();		//先设置为0,再开始解压,再获取解压文件数量确定进度
//int		gfun_TPGetZipFileCnt();

