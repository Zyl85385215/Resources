#include "ItemColorConfig.h"
#include "xnExtstr.h"

CreateSingleton(CItemColorConfig);

CItemColorConfig::CItemColorConfig( void )
{

}

CItemColorConfig::~CItemColorConfig( void )
{
	std::vector<ItemColorInfo*>::iterator iterator = m_vecItemColorInfo.begin();
	while(iterator != m_vecItemColorInfo.end())
	{
		ItemColorInfo * p = *iterator;
		SAFE_DELETE(p);
		++iterator;
	}
}

bool CItemColorConfig::load( const char * szFile )
{
	xnExtstr * pExtstr = xnExtstr::Import((char*)szFile);
	if(!pExtstr) return false;

	char szSection[64] = {0};
	int i=0;
	sprintf(szSection, "%d", i++);
	while(pExtstr->SetSection(szSection))
	{
		ItemColorInfo * p = new ItemColorInfo;
		memset(p, 0, sizeof(ItemColorInfo));

		p->m_nQuality = atoi(pExtstr->ReadString("quality", "0"));
#ifdef _CLIENT
		char * szString = pExtstr->ReadString("color");
		if(szString) sscanf(szString, "%d,%d,%d", &p->m_byR, &p->m_byG, &p->m_byB);
		szString = pExtstr->ReadString("name");
		if(szString) strncpy(p->m_szName, szString, ITEM_COLOR_NAME_LEN-1);
		szString = pExtstr->ReadString("spr");
		if(szString) strncpy(p->m_szSpr, szString, ITEM_COLOR_SPR_PATH_LEN-1);
		szString = pExtstr->ReadString("act");
		if(szString) strncpy(p->m_szAct, szString, ITEM_COLOR_ACT_PATH_LEN-1);
#endif
		m_vecItemColorInfo.push_back(p);

		sprintf(szSection, "%d", i++);
		m_nCount+=1;
	}

	pExtstr->Free();
	return true;
}