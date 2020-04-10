#pragma once

#include <Windows.h>

class	UserData;
class	ShopCenter
{
public:
	ShopCenter();
	~ShopCenter();

	//谁购买什么类型的数据用 什么数据
	//nUseID 可以为各类wealth, 或者AmzData, 也可以是flag
	bool	MakeDeal(UserData* pUser, int nShipID, int	nUseID);	

private:

};

extern	ShopCenter*		g_pShopCenter;