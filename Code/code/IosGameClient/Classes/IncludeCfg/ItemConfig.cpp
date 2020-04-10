#include "ItemConfig.h"
#include "xnExtstr.h"

#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }

#endif
CreateSingleton(ItemConfig);



ItemBase::ItemBase()
{
	szName			= NULL;
	szDesc			= NULL;
}

ItemBase::~ItemBase()
{
	SAFE_FREE(szName);
	SAFE_FREE(szDesc);
}

ItemConfig::ItemConfig()
{
	//pListMaterials	= xnList::Create ();
	pListItems		= xnList::Create ();
}

ItemConfig::~ItemConfig()
{
	while(pListItems->Count)
		delete (ItemBase*)pListItems->Delete (0);
	pListItems->Free ();
	//while(pListMaterials->Count)
	//	delete (ItemBase*)pListMaterials->Delete (0);
	//pListMaterials->Free ();
}

void ItemConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[32];
	while (pStr->SetSection(index++) != NULL)
	{
		ItemBase*	pIB	= new ItemBase();
		pIB->nTypeID		= atoi (pStr->sectionName);
		pIB->szName			= strdup(pStr->ReadString ("name","未知物品"));
		pIB->szDesc			= strdup(pStr->ReadString ("desc","神秘的"));
		pIB->nSpr			= atoi (pStr->ReadString ("spr","1"));
		pIB->nStar			= atoi (pStr->ReadString ("star","1"));
		pIB->byEffType		= atoi (pStr->ReadString ("efftype","1"));
		pIB->wEffVal		= atoi (pStr->ReadString ("effval","1"));
		pIB->bCanUsed		= atoi(pStr->ReadString ("canuse","0"));

		pListItems->Add (pIB);
	}
	pStr->Free ();
}

ItemBase* ItemConfig::GetItemBase( int nTypeID )
{
	for (int i = 0; i < pListItems->Count; i++)
	{
		ItemBase* pIB	= (ItemBase*)pListItems->Items[i];
		if(pIB->nTypeID == nTypeID)
			return pIB;
	}
	return NULL;
}
