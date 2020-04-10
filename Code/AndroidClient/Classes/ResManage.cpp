#include "ResManage.h"
#include "cocos2d.h"

using namespace cocos2d;

CreateSingleton(ResManager);

_plistResObj::_plistResObj()
{
	szPList	= NULL;
	nUseCnt	= 0;
	dwTick	= 0;
}

_plistResObj::~_plistResObj()
{
	if(szPList)
		free(szPList);
}

ResManager::ResManager()
{
	for (int i = 0; i < 5; i++)
	{
		m_pListCache[i]	= xnList::Create();
	}
}

ResManager::~ResManager()
{
	for (int i = 0; i < 5; i++)
	{
		while(m_pListCache[i]->Count)
			delete (_plistResObj*)m_pListCache[i]->Delete(0);

		m_pListCache[i]->Free();
	}
}

void ResManager::AddPList( char* szPL,int nMark,BYTE bType )
{
	if(bType >= 5)
		return;
	_plistResObj* pPL	= NULL;
	for (int i = 0; i < m_pListCache[bType]->Count; i++)
	{
		pPL	= (_plistResObj*)m_pListCache[bType]->Items[i];
		if(pPL->nMark == nMark)
		{
			if(strcmp(pPL->szPList,szPL) == 0)
			{
				pPL->nUseCnt++;
				return;		//已存在,累加
			}
		}
	}
	pPL	= new _plistResObj();
	pPL->szPList	= strdup(szPL);
	pPL->nUseCnt	= 1;
	pPL->nMark		= nMark;
	m_pListCache[bType]->Add(pPL);
}

void ResManager::SubPList( char* szPL,int nMark,BYTE bType )
{
	if(bType >= 5)
		return;
	_plistResObj* pPL	= NULL;
	for (int i = 0; i < m_pListCache[bType]->Count; i++)
	{
		pPL	= (_plistResObj*)m_pListCache[bType]->Items[i];
		if(nMark == 0 || pPL->nMark == nMark)
		{
			if(strcmp(pPL->szPList,szPL) == 0)
			{
				if(pPL->nUseCnt)
				{
					pPL->nUseCnt--;

					if(pPL->nUseCnt == 0)
						pPL->dwTick	= xnGetTickCount();
				}
				return;		//已存在,累加
			}
		}
	}
}

void ResManager::ClearPList()
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	for (int i = 0; i < 5; i++)
	{
		while(m_pListCache[i]->Count)
		{
			_plistResObj* pPL	= (_plistResObj*)m_pListCache[i]->Delete(0);
			cache->removeSpriteFramesFromFile(pPL->szPList);
			delete pPL;
		}
	}
}

void ResManager::Update()
{
	DWORD	dwCur	= xnGetTickCount();
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	for (int n = 0; n < 5; n++)
	{
		for (int i = m_pListCache[n]->Count-1; i >= 0; i--)
		{
			_plistResObj* pPL	= pPL	= (_plistResObj*)m_pListCache[n]->Items[i];
	
			if(pPL->nUseCnt==0 && pPL->dwTick && dwCur - pPL->dwTick > 30000)
			{//超过半分钟不用的对象需要删除
				m_pListCache[n]->Delete(i);
				cache->removeSpriteFramesFromFile(pPL->szPList);
				delete pPL;
			}
		}
	}
	
}
