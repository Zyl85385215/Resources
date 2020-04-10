#include "stdafx.h"
#include "cocos2d.h"
#include "FaceConfig.h"
#include "xnExtstr.h"


CreateSingleton(FaceConfig);

FaceConfig::FaceConfig()
{
	m_pList = xnList::Create();
}

FaceConfig::~FaceConfig()
{
	if(m_pList)
	{
		while(m_pList->Count)
			delete (FaceObj*)m_pList->Delete(0);
		m_pList->Free();
	}
}

void FaceConfig::LoadFile(char * pName)
{
	xnExtstr* pStr = xnExtstr::Import(pName);
	if(pStr == NULL)
		return;
	int nIndex = 0;
	cocos2d::CCSpriteFrameCache* cache = cocos2d::CCSpriteFrameCache::sharedSpriteFrameCache();
	char szPlist[64];
	while(pStr->SetSection(nIndex++))
	{
		FaceObj* pObj = new FaceObj();
		pObj->nID = atoi(pStr->sectionName);
		pObj->nTotalFrm = atoi(pStr->ReadString("fram","4"));
		pObj->nSpeed = atoi(pStr->ReadString("speed","250"));
		m_pList->Add((void*)pObj);
		sprintf(szPlist,"icon/face/%d/1.plist",pObj->nID);
		cache->addSpriteFramesWithFile((const char*)szPlist);
	}
	pStr->Free();
}

FaceObj* FaceConfig::GetFace(int nId)
{
	for(int i = 0; i < m_pList->Count; i ++)
	{
		FaceObj* pObj = (FaceObj*)m_pList->Items[i];
		if(pObj->nID == nId)
			return pObj;
	}
	return NULL;
}