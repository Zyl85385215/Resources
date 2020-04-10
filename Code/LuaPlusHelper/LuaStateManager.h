#ifndef _LUA_STATE_MANAGER_H
#define _LUA_STATE_MANAGER_H

#pragma once

#include <Windows.h>
#include <map>
#include <string>
#include "luaplus.h"
#include "LuaPlusSafeFunction.h"

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

template<typename Object>
class CLuaClass;

typedef std::map<std::string, void*> ScriptClassMap;

class CLuaState
{
public:
	CLuaState(void)			{ m_pLuaState = LuaState::Create(true); }
	virtual ~CLuaState(void){ LuaState::Destroy(m_pLuaState); }

	virtual void Init(void)		= 0;
	virtual void DeInit(void)	= 0;

	template<typename Object>
	CLuaClass<Object> * CreateScriptClass(Object * pObject)
	{
		CLuaClass<Object> * pScriptClass = new CLuaClass<Object>(m_pLuaState);

		std::string scriptclassname = typeid(Object).raw_name();
		m_ScriptClassMap[scriptclassname] = (void*)pScriptClass;
		return pScriptClass;
	}

	template<typename Object>
	CLuaClass<Object> * GetScriptClass(Object * pObject)
	{
		std::string scriptclassname = typeid(Object).raw_name();
		return (CLuaClass<Object>*)m_ScriptClassMap[scriptclassname];
	}

	template<typename Object>
	void ReleaseScriptClass(Object * pObject)
	{
		std::string scriptclassname = typeid(Object).raw_name();
		CLuaClass<Object> * pScriptClass = (CLuaClass<Object>*)m_ScriptClassMap[scriptclassname];
		SAFE_DELETE(pScriptClass);
	}

	LuaState * GetLuaState()	{ return m_pLuaState; }

protected:
	LuaState * m_pLuaState;

private:
	ScriptClassMap m_ScriptClassMap;
};

typedef std::map<DWORD, CLuaState*> LuaStateMap;

class CLuaStateManager
{
public:
	CLuaStateManager(void);
	virtual ~CLuaStateManager(void);

	virtual CLuaState * CreateLuaState(void) = 0;
	CLuaState * GetLuaState(DWORD dwThreadID);

	static CLuaStateManager * m_pLuaStateManager;

protected:
	LuaStateMap			m_mapLuaState;
	CRITICAL_SECTION	m_xCS;
};

void SetLuaStateManager(CLuaStateManager * pLuaStateManager);

#endif