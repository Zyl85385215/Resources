#ifndef _LUA_CLASS_OBJECT_H
#define _LUA_CLASS_OBJECT_H

#define LUAFUN	

#pragma once

#include <Windows.h>
#include <assert.h>
#include <string>
#include "luastatemanager.h"

extern CLuaStateManager * g_pLuaStateManager;

template<typename Object>
class CLuaClass;

template<typename Object>
class CLuaClassObject
{
public:
	//	construction
	CLuaClassObject(void)
	{
		m_pParent	= 0;
		m_pLuaState	= 0;
		m_dwThreadID= 0;
		m_nID		= 0;
		m_pObject	= 0;
	}
	~CLuaClassObject() 
	{
		;
	}

	void Init(Object * pObject, int nID = 0)
	{
		m_nID		= nID;
		m_pObject	= pObject;
		_SetThread(GetCurrentThreadId());
	}

	void Init(Object * pObject, const char * szID)
	{
		m_strID		= szID;
		m_pObject	= pObject;
		_SetThread(GetCurrentThreadId());
	}

	//	run script function
	template<typename RT>
		inline bool RunFunction(int nFuncID, RT * pRt = 0)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_2<RT, Object*> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject);
	}

	template<typename RT, typename P1>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_3<RT, Object*, P1> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1);
	}

	template<typename RT, typename P1, typename P2>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_4<RT, Object*, P1, P2> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2);
	}

	template<typename RT, typename P1, typename P2, typename P3>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2, P3 p3)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_5<RT, Object*, P1, P2, P3> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2, p3);
	}

	template<typename RT, typename P1, typename P2, typename P3, typename P4>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2, P3 p3, P4 p4)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_6<RT, Object*, P1, P2, P3, P4> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2, p3, p4);
	}

	template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_7<RT, Object*, P1, P2, P3, P4, P5> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2, p3, p4, p5);
	}

	template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_8<RT, Object*, P1, P2, P3, P4, P5, P6> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2, p3, p4, p5, p6);
	}

	template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_9<RT, Object*, P1, P2, P3, P4, P5, P6, P7> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2, p3, p4, p5, p6, p7);
	}

	template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
		inline bool RunFunction(int nFuncID, RT * pRt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8)
	{
		_CheckThread();
		LuaFunction<RT> lf(_GetLuaFunction(nFuncID));
		static CLuaPlusSafeFunction_10<RT, Object*, P1, P2, P3, P4, P5, P6, P7, P8> func;
		return func(m_pLuaState->GetLuaState(), lf, pRt, m_pObject, p1, p2, p3, p4, p5, p6, p7, p8);
	}

	LuaObject GetLuaObject()
	{
		_CheckThread();
		LuaState * pLuaState = m_pLuaState->GetLuaState();
		LuaObject loThis = pLuaState->BoxPointer(m_pObject);
		pLuaState->Pop();
		loThis.SetMetaTable(m_pParent->GetMetaTable());
		return loThis; 
	}

	bool IsFunctionExisted(int nFuncID)
	{
		_CheckThread();
		LuaObject loFunction = _GetLuaFunction(nFuncID);
		return loFunction.IsFunction();
	}

private:
	//	make sure this object is in a single thread
	inline void _CheckThread(void)
	{
		DWORD dwThreadID = GetCurrentThreadId();
		if(m_dwThreadID != dwThreadID) 
		{
			_SetThread(dwThreadID);
		}
	}

	//	set a lua_State
	inline void _SetThread(DWORD dwThreadID)
	{
		m_pLuaState = g_pLuaStateManager->GetLuaState(dwThreadID);
		m_pParent = m_pLuaState->GetScriptClass((Object*)0);

		m_dwThreadID= dwThreadID;
	}

	//	get function luaobject
	inline LuaObject _GetLuaFunction(int nFuncID)
	{
		LuaObject loFunction = m_strID.empty() ? m_pParent->GetLuaFunction(m_nID, nFuncID) : m_pParent->GetLuaFunction(m_strID.c_str(), nFuncID);
		return loFunction;
	}

	CLuaClass<Object> * m_pParent;
	Object *	m_pObject;
	DWORD		m_dwThreadID;
	CLuaState * m_pLuaState;
	int			m_nID;
	std::string	m_strID;
};

#endif