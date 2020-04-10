#include "stdafx.h"
#include "LuaStateManager.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

CLuaStateManager * g_pLuaStateManager = 0;

CLuaStateManager::CLuaStateManager(void)
{
	InitializeCriticalSection(&m_xCS);
}

CLuaStateManager::~CLuaStateManager(void)
{
	LuaStateMap::iterator iterator = m_mapLuaState.begin();
	while(iterator != m_mapLuaState.end())
	{
		CLuaState * pLuaState = iterator->second;
		pLuaState->DeInit();
		SAFE_DELETE(pLuaState);
		iterator++;
	}

	DeleteCriticalSection(&m_xCS);
}

CLuaState * CLuaStateManager::GetLuaState(DWORD dwThreadID)
{
	CLuaState * pLuaState = 0;

	EnterCriticalSection(&m_xCS);
	pLuaState = m_mapLuaState[dwThreadID];
	LeaveCriticalSection(&m_xCS);
	
	if(!pLuaState)
	{
		pLuaState = CreateLuaState();
		EnterCriticalSection(&m_xCS);
		m_mapLuaState[dwThreadID] = pLuaState;
		LeaveCriticalSection(&m_xCS);
		pLuaState->Init();
	}
	return pLuaState;
}

void SetLuaStateManager(CLuaStateManager * pLuaStateManager)
{
	g_pLuaStateManager = pLuaStateManager;
}