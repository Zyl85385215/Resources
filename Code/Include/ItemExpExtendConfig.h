#ifndef _ITEM_EXP_EXTEND_CONFIG_H
#define _ITEM_EXP_EXTEND_CONFIG_H

#include <map>
using namespace std;

#include "item_packet.h"

#define SAFE_DELETE(p)	{ if(p) { delete (p); (p) = NULL; } }

struct ItemExpExtendInfo
{
	BYTE	bLevel;
	DWORD	dwExp;
	ExtendData	xExtendData;
};

typedef std::map<BYTE, ItemExpExtendInfo*>	LevelExtendInfoMap;			//	某等级下的扩展属性
typedef std::map<DWORD, LevelExtendInfoMap*>	ItemLevelMap;				//	物品的各个等级

class CItemExpExtendConfig
{
public:
	static CItemExpExtendConfig * Init()		{ if(!m_pItemExpExtendConfig) { m_pItemExpExtendConfig = new CItemExpExtendConfig(); } return m_pItemExpExtendConfig; }
	static void	DeInit()						{ SAFE_DELETE(m_pItemExpExtendConfig); }
	static CItemExpExtendConfig * GetInstance()	{ return m_pItemExpExtendConfig; }

	bool	Load(const char * szFileName);
	bool	GetExpExtendInfo(DWORD wItemId, BYTE bLevel, ItemExpExtendInfo& xExtendInfo);

private:
	CItemExpExtendConfig();
	~CItemExpExtendConfig();

	ItemLevelMap m_configMap;

	static CItemExpExtendConfig * m_pItemExpExtendConfig;
};

#endif