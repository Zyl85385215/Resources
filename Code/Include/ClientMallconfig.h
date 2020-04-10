#pragma once

#include <Windows.h>
#include "xnlist.h"
#include "xnExtstr.h"
#include "singleton.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = 0; } }


struct SellMallItem
{
	WORD	itemID;
	int		price;
	BYTE	bBindType;		//0,  1,  10
	BYTE	bType;
};

class MallSellConfig : public CSingleton<MallSellConfig>
{
public:
	MallSellConfig();
	~MallSellConfig();

	bool Load(const char * szFile);
	SellMallItem* GetItem(int nItemID,BYTE bType);

private:
	xnList* m_pList;
};