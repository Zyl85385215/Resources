#pragma once

#include <Windows.h>

class	UserData;
class	ShopCenter
{
public:
	ShopCenter();
	~ShopCenter();

	//˭����ʲô���͵������� ʲô����
	//nUseID ����Ϊ����wealth, ����AmzData, Ҳ������flag
	bool	MakeDeal(UserData* pUser, int nShipID, int	nUseID);	

private:

};

extern	ShopCenter*		g_pShopCenter;