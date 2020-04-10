#ifndef _LUA_CLASS_H
#define _LUA_CLASS_H

#pragma once

#include "luaplus.h"
#include "luastatemanager.h"
#include <string>

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

template<typename Object>
class CLuaClass
{
public:
	CLuaClass(LuaState * pLuaState)
	{
		m_pLuaState = pLuaState;

		std::string strTable = "MetaClass_";
		strTable += typeid(Object).raw_name();

		m_loGlobalsTableObj = pLuaState->GetGlobals();
		m_loMetaTableObj = m_loGlobalsTableObj.CreateTable(strTable.c_str());
		m_loMetaTableObj.SetObject("__index", m_loMetaTableObj);

		strTable = "FuncClass_";
		strTable += typeid(Object).raw_name();

		m_loFuncTableObj = m_loGlobalsTableObj.CreateTable(strTable.c_str());
	}

	~CLuaClass(void)	
	{
	}

	template<typename Func>
		inline CLuaClass& DefObjectFunction(const char * name, Func func)
	{
		m_loMetaTableObj.RegisterObjectDirect(name, (Object*) 0, func);
		return *this;
	}

	inline CLuaClass& DefScriptFunction(int nID, int nFuncID, LuaObject & loFunction)
	{
		if(loFunction.IsFunction())
		{
			LuaObject loFuncTableObj = m_loFuncTableObj[nID];
			if(!loFuncTableObj.IsTable())
			{
				loFuncTableObj = m_loFuncTableObj.CreateTable(nID);
			}
			loFuncTableObj.SetObject(nFuncID, loFunction);
		}
		return *this;
	}

	inline CLuaClass& DefScriptFunction(int nID, int nFuncID, const char * szFuncName)
	{
		LuaObject loFunction = m_loGlobalsTableObj[szFuncName];
		return DefScriptFunction(nID, nFuncID, loFunction);
	}

	inline CLuaClass& DefScriptFunction(const char * szID, int nFuncID, LuaObject & loFunction)
	{
		if(loFunction.IsFunction())
		{
			LuaObject loFuncTableObj = m_loFuncTableObj[szID];
			if(!loFuncTableObj.IsTable())
			{
				loFuncTableObj = m_loFuncTableObj.CreateTable(szID);
			}
			loFuncTableObj.SetObject(nFuncID, loFunction);
		}
		return *this;
	}

	inline CLuaClass& DefScriptFunction(const char * szID, int nFuncID, const char * szFuncName)
	{
		LuaObject loFunction = m_loGlobalsTableObj[szFuncName];
		return DefScriptFunction(szID, nFuncID, loFunction);
	}

	LuaObject GetMetaTable(void) { return m_loMetaTableObj; }

	LuaObject GetLuaFunction(int nID, int nFuncID) 
	{
		LuaObject loFuncTableObj = m_loFuncTableObj[nID];
		if(loFuncTableObj.IsTable())
		{
			return loFuncTableObj[nFuncID];
		}
		return LuaObject(m_pLuaState);
	}

	LuaObject GetLuaFunction(const char * szID, int nFuncID) 
	{
		LuaObject loFuncTableObj = m_loFuncTableObj[szID];
		if(loFuncTableObj.IsTable())
		{
			return loFuncTableObj[nFuncID];
		}
		return LuaObject(m_pLuaState);
	}

private:
	LuaState * m_pLuaState;
	LuaObject m_loMetaTableObj;
	LuaObject m_loGlobalsTableObj;
	LuaObject m_loFuncTableObj;
};

#endif