#pragma once
#include "windows.h"
#include "xnlist.h"
#include "singleton.h"

// add by zp
class ExpandBagConfigElem
{
public:
	BYTE byBagPage; // ҳ��
	int iNeedScore;  // ��Ҫ�Ļ���
	int iNeedMoney;  // ��Ҫ��Ǯ
	WORD iUnbindCount; // ���������ӵ�����
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
{//���������
public:
	SpecialBagConfig();
	~SpecialBagConfig();

	void	LoadFile(char*	szFile);
	bool	CheckIsSPItem(DWORD	dwItemID);
	char*	GetSpItemDesc(DWORD	dwItemID);
	SPItemCfgObj*	GetSPIObj(int nIndex);
	
	xnList	*	m_pList;
};