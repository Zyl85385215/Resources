#pragma once
#include "windows.h"
#include "xnlist.h"
#include "singleton.h"

// add by zp
class ExpandBagConfigElem
{
public:
	BYTE byBagPage; // 页数
	int iNeedScore;  // 需要的积分
	int iNeedMoney;  // 需要的钱
	WORD iUnbindCount; // 解绑包裹格子的数量
};



class ExpandBagConfig:public CSingleton<ExpandBagConfig>
{
public:
	ExpandBagConfig();
	~ExpandBagConfig();

	bool loadExpandBagConfig(char* szFile);
    const ExpandBagConfigElem* getExpandBagConfigByPage(BYTE byBagPage);
private:
	xnList* m_pkBagConfigList;
	
};

class SPItemCfgObj
{
public:
	DWORD	dwItemID;
	char	szDesc[255];
};

class SpecialBagConfig: public CSingleton<SpecialBagConfig>
{//宝物库配置
public:
	SpecialBagConfig();
	~SpecialBagConfig();

	void	LoadFile(char*	szFile);
	bool	CheckIsSPItem(DWORD	dwItemID);
	char*	GetSpItemDesc(DWORD	dwItemID);
	SPItemCfgObj*	GetSPIObj(int nIndex);
	
	xnList	*	m_pList;
};