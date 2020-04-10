#pragma once

#include "luaplus.h"
using namespace LuaPlus;
template<typename RT>
class CLuaPlusSafeFunction_1
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf();
			else lf();
		}
		catch(...)
		{
			pLuaState->GetCState()->lua_pop(1);
			return false;
		}
		return true;
	}
};

template<>
class CLuaPlusSafeFunction_1<void>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf();
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1>
class CLuaPlusSafeFunction_2
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1);
			else lf(p1);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1>
class CLuaPlusSafeFunction_2<void, P1>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2>
class CLuaPlusSafeFunction_3
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2);
			else lf(p1, p2);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2>
class CLuaPlusSafeFunction_3<void, P1, P2>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3>
class CLuaPlusSafeFunction_4
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3);
			else lf(p1, p2, p3);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3>
class CLuaPlusSafeFunction_4<void, P1, P2, P3>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4>
class CLuaPlusSafeFunction_5
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3, P4 p4) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3, p4);
			else lf(p1, p2, p3, p4);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3, typename P4>
class CLuaPlusSafeFunction_5<void, P1, P2, P3, P4>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3, P4 p4) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3, p4);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5>
class CLuaPlusSafeFunction_6
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3, p4, p5);
			else lf(p1, p2, p3, p4, p5);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3, typename P4, typename P5>
class CLuaPlusSafeFunction_6<void, P1, P2, P3, P4, P5>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3, p4, p5);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class CLuaPlusSafeFunction_7
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3, p4, p5, p6);
			else lf(p1, p2, p3, p4, p5, p6);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6>
class CLuaPlusSafeFunction_7<void, P1, P2, P3, P4, P5, P6>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3, p4, p5, p6);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class CLuaPlusSafeFunction_8
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3, p4, p5, p6, p7);
			else lf(p1, p2, p3, p4, p5, p6, p7);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7>
class CLuaPlusSafeFunction_8<void, P1, P2, P3, P4, P5, P6, P7>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3, p4, p5, p6, p7);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class CLuaPlusSafeFunction_9
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3, p4, p5, p6, p7, p8);
			else lf(p1, p2, p3, p4, p5, p6, p7, p8);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8>
class CLuaPlusSafeFunction_9<void, P1, P2, P3, P4, P5, P6, P7, P8>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3, p4, p5, p6, p7, p8);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename RT, typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
class CLuaPlusSafeFunction_10
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<RT> & lf, RT * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) const 
	{
		if(!pLuaState) return false;
		try
		{
			if(lprt) *lprt = lf(p1, p2, p3, p4, p5, p6, p7, p8, p9);
			else lf(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};

template<typename P1, typename P2, typename P3, typename P4, typename P5, typename P6, typename P7, typename P8, typename P9>
class CLuaPlusSafeFunction_10<void, P1, P2, P3, P4, P5, P6, P7, P8, P9>
{
public:
	bool operator()(LuaState * pLuaState, LuaFunction<void> & lf, void * lprt, P1 p1, P2 p2, P3 p3, P4 p4, P5 p5, P6 p6, P7 p7, P8 p8, P9 p9) const 
	{
		if(!pLuaState) return false;
		try
		{
			lf(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}
		catch(...)
		{
			lua_pop(pLuaState->GetCState(), 1);
			return false;
		}
		return true;
	}
};