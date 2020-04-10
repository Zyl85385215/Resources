#include "FriendsConfig.h"
#include "xnExtstr.h"
#include "stdio.h"
#include "misc.h"
#include "debug_def.h"

CreateSingleton(FriendsConfig);

FriendsConfig::FriendsConfig( )
{
	m_pListFriends		= xnList::Create();
}

FriendsConfig::~FriendsConfig()
{
	while(m_pListFriends->Count)
		delete (FriendCInfo*)m_pListFriends->Delete(0);

	m_pListFriends->Free();
}

void FriendsConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if(!pStr)	return;
	int iIndex = 0;
	char szTmp[32];

	while(pStr->SetSection(iIndex++))
	{
		FriendCInfo*	pInfo	= new FriendCInfo();
		m_pListFriends->Add(pInfo);
		pInfo->wID	= atoi(pStr->sectionName);
		pInfo->wStateID	= atoi(pStr->ReadString("StateID","0"));
		pInfo->wSkillID[0]	= atoi(pStr->ReadString("Skill1","0"));
		pInfo->wSkillID[1]	= atoi(pStr->ReadString("Skill1","0"));
		pInfo->wSkillID[2]	= atoi(pStr->ReadString("Skill1","0"));
		pInfo->wSkillID[3]	= atoi(pStr->ReadString("Skill1","0"));
		pInfo->wSkillID[4]	= atoi(pStr->ReadString("Skill1","0"));
#ifndef _ISSERVER
		strcpy(pInfo->szAge,pStr->ReadString("Age",""),5);
		strcpy(pInfo->szJob,pStr->ReadString("Job",""),16);
		strcpy(pInfo->szName,pStr->ReadString("Name",""),16);
		strcpy(pInfo->szDesc,pStr->ReadString("Test",""),255);
		for (int i = 0; i < 4; i++)
		{
			sprintf(szTmp,"StateText%d",i+1);
			strcpy(pInfo->szStatetxt[i],pStr->ReadString(szTmp,""),255);
		}
#endif
		int nCnt	= atoi(pStr->ReadString("CostCnt", "0"));
		for (int i = 0; i < nCnt; i++)
		{
			FIValType* pFIV = new FIValType();
			pInfo->pListCost->Add(pFIV);
			sprintf(szTmp,"CostType%d",i+1);
			pFIV->bType	= atoi(pStr->ReadString(szTmp, "0"));
			sprintf(szTmp,"Cost%d",i+1);
			pFIV->nVal1	= atoi(pStr->ReadString(szTmp, "0"));
		}
		char * szSplit[10];
		int nSpltCnt = SplitString(pStr->ReadString("NeedLvl","0"), '|', szSplit, 2);
		pInfo->bLvMin	= (nSpltCnt == 0) ? 0 : atoi(szSplit[0]);
		if(nSpltCnt == 2)
			pInfo->bLvMax	= atoi(szSplit[1]);
		else
			pInfo->bLvMax	= 255;

		for (int i=0; i<nSpltCnt;i++)
			free(szSplit[i]);

		nSpltCnt = SplitString(pStr->ReadString("NeedMW","0"), '|', szSplit, 2);
		pInfo->bHLvMin	= (nSpltCnt == 0) ? 0 : atoi(szSplit[0]);
		if(nSpltCnt == 2)
			pInfo->bHLvMax	= atoi(szSplit[1]);
		else
			pInfo->bHLvMax	= 255;

		for (int i=0; i<nSpltCnt;i++)
			free(szSplit[i]);

		nSpltCnt = SplitString(pStr->ReadString("NeedPK","0"), '|', szSplit, 2);
		pInfo->wPkMin	= (nSpltCnt == 0) ? 0 : atoi(szSplit[0]);
		if(nSpltCnt == 2)
			pInfo->wPKMax	= atoi(szSplit[1]);
		else
			pInfo->wPKMax	= 255;

		for (int i=0; i<nSpltCnt;i++)
			free(szSplit[i]);

		nSpltCnt = SplitString(pStr->ReadString("NeedTitle","0"), '|', szSplit, 5);
		for (int i = 0; i < nSpltCnt; i++)
		{
			int nTID	= atoi(szSplit[i]);
			if(nTID)
				pInfo->pListTitles->Add((void*)nTID);
			free(szSplit[i]);
		}
		nSpltCnt = SplitString(pStr->ReadString("NeedFriend","0"), '|', szSplit, 5);
		for (int i = 0; i < nSpltCnt; i++)
		{
			int nFID	= atoi(szSplit[i]);
			if(nFID)
				pInfo->pListFriends->Add((void*)nFID);
			free(szSplit[i]);
		}
		nSpltCnt = SplitString(pStr->ReadString("NeedMPSW","0"), '|', szSplit, 9);
		for (int i = 2; i < nSpltCnt; i+=3)
		{
			FIValType* pFIV = new FIValType();
			pInfo->pListSchool->Add(pFIV);
			pFIV->bType	= atoi(szSplit[i-2]);
			pFIV->nVal1	= atoi(szSplit[i-1]);
			pFIV->nVal2	= atoi(szSplit[i]);
			free(szSplit[i]);
		}
	}

	pStr->Free();
}

FriendCInfo* FriendsConfig::GetFInfoByID( WORD wID )
{
	for (int i = 0; i < m_pListFriends->Count; i++)
	{
		FriendCInfo* pFInfo	= (FriendCInfo*)m_pListFriends->Items[i];
		if(pFInfo->wID == wID)
			return pFInfo;
	}
	return NULL;
}

FriendCInfo::FriendCInfo()
{
	pListTitles		= xnList::Create();
	pListSchool		= xnList::Create();
	pListFriends	= xnList::Create();
	pListCost		= xnList::Create();
}

FriendCInfo::~FriendCInfo()
{
	while(pListCost->Count)
		delete (FIValType*)pListCost->Delete(0);
	pListCost->Free();
	while(pListSchool->Count)
		delete (FIValType*)pListSchool->Delete(0);
	pListSchool->Free();
	pListTitles->Free();
	pListFriends->Free();
}