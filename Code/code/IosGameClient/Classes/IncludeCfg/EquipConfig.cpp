#include "EquipConfig.h"
#include "xnExtstr.h"

#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }

#endif
CreateSingleton(EquipConfig);


EquipBase::EquipBase()
{
	szName			= NULL;
	szDesc			= NULL;
}

EquipBase::~EquipBase()
{
	SAFE_FREE(szName);
	SAFE_FREE(szDesc);
}

EquipConfig::EquipConfig()
{
	pListEquip		= xnList::Create ();
}

EquipConfig::~EquipConfig()
{
	while(pListEquip->Count)
		delete (EquipBase*)pListEquip->Delete (0);
	pListEquip->Free ();
}

void EquipConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		EquipBase*	pEB	= new EquipBase();
		pEB->nTypeID		= atoi (pStr->sectionName);
		pEB->szName		= strdup(pStr->ReadString ("name","未知装备"));
		pEB->szDesc		= strdup(pStr->ReadString ("desc","神秘的"));
		pEB->nSpr			= atoi (pStr->ReadString ("spr","1"));
		pEB->nStar			= atoi (pStr->ReadString ("star","1"));
		pEB->bSlot			= atoi (pStr->ReadString ("slot","0"));

		pEB->nParams[WARS_PARAM_MAXHP]		= atoi (pStr->ReadString ("hp","0"));
		pEB->nParams[WARS_PARAM_ATK]		= atoi (pStr->ReadString ("atk","0"));
		pEB->nParams[WARS_PARAM_DEF]		= atoi (pStr->ReadString ("def","0"));
		pEB->nParams[WARS_PARAM_MISS]		= atoi (pStr->ReadString ("miss","0"));
		pEB->nParams[WARS_PARAM_CRITIC]		= atoi (pStr->ReadString ("critic","0"));

		pListEquip->Add (pEB);
	}
	pStr->Free ();
}

EquipBase* EquipConfig::GetEquipBase( int nType )
{
	for (int i = 0; i < pListEquip->Count; i++)
	{
		EquipBase* pEB	= (EquipBase*)pListEquip->Items[i];
		if(pEB->nTypeID == nType)
			return pEB;
	}
	return NULL;
}