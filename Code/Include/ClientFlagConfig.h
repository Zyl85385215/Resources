#pragma once

#include "singleton.h"
#include "windows.h"
#include "xnlist.h"

class CCFlagFormCmp
{
public:
	char	szForm[20];
	char	szComp[20];
};

class CCFlagObj
{
public:
	CCFlagObj();
	~CCFlagObj();
	char	szFlag[20];
	xnList*	pListFC;
};

class ClientCompFlagCfg : public CSingleton<ClientCompFlagCfg>
{
public:
	ClientCompFlagCfg();
	~ClientCompFlagCfg();

	void	LoadFile(char* szFile);
	int		GetFlagPos(char* szFlag);

	xnList*	m_pListFlag;
};

class ClientFlagConfig : public CSingleton<ClientFlagConfig>
{
public:
	ClientFlagConfig();
	~ClientFlagConfig();

	void	LoadFile(char* szFile);
	int		GetCnt()	{return m_pList->Count;}
	int		GetFlagPos(char* szFlag);
	char*	GetPosFlag(int nPos);
private:
	xnList*		m_pList;
};

class LItemObjInfo
{
public:
	DWORD	dwItemID;
	int		nVal;
};

class ItemLuckBox
{
public:
	ItemLuckBox();
	~ItemLuckBox();

	DWORD	dwItemID;
	BYTE	byNeedPos;
	xnList*	pListLuckI;
	xnList*	pListNormalI;
	xnList*	pListPointI;
};


class LuckBoxConfig : public CSingleton<LuckBoxConfig>
{
public:
	LuckBoxConfig();
	~LuckBoxConfig();

	void	LoadFile(char* szFile);
	ItemLuckBox* GetLuckItemInfo(DWORD dwID);
private:
	xnList*		m_pList;
};