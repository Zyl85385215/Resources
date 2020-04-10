#include "stdafx.h"
#ifdef _ISSERVER
#include "stdio.h"
#define SAFE_FREE(p) { if(p) { free(p); p=NULL; } }
#endif

#include "EquipConfig.h"
#include "xnExtstr.h"

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
		pEB->szName		= strdup(pStr->ReadString ("name","Î´Öª×°±¸"));
		pEB->szDesc		= strdup(pStr->ReadString ("desc",""));
		pEB->nSpr			= atoi (pStr->ReadString ("spr","1"));
		pEB->nStar			= atoi (pStr->ReadString ("star","1"));
		pEB->bSlot			= atoi (pStr->ReadString ("type","0"));
		pEB->nLevel			= atoi (pStr->ReadString ("level","1"));
		pEB->nNext			= atoi(pStr->ReadString ("next","0"));
		pEB->nSellGold			= atoi(pStr->ReadString ("gold","0"));

		if(!mapEquip[pEB->bSlot][pEB->nLevel][pEB->nStar]) mapEquip[pEB->bSlot][pEB->nLevel][pEB->nStar] = xnList::Create();
		mapEquip[pEB->bSlot][pEB->nLevel][pEB->nStar]->Add(pEB);
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

