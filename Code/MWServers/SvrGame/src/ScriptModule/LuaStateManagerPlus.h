#ifndef LUA_STATE_MANAGER_PLUS_H
#define LUA_STATE_MANAGER_PLUS_H

#include "luaplushelper.h"

#define LUA_DOFILE(luaState,filename) if(luaState->DoFile(filename)){ AfxMessageBox(lua_tostring(luaState->GetCState(),-1));}


class CTM;
class ItemCenter;
class ItemObj;
class UserData;
class MapObjCenter;
class MapServerObj;
class SlaveCenter;
class HorseCenter;
class SlaveBaseData;
class HorseBaseData;
class UserRecord;
DEFINE_SCRIPT_CLASS_EXTERN(MapObjCenter);
DEFINE_SCRIPT_CLASS_EXTERN(MapServerObj);
DEFINE_SCRIPT_CLASS_EXTERN(CTM);
DEFINE_SCRIPT_CLASS_EXTERN(ItemCenter);
DEFINE_SCRIPT_CLASS_EXTERN(ItemObj);
DEFINE_SCRIPT_CLASS_EXTERN(UserData);
DEFINE_SCRIPT_CLASS_EXTERN(SlaveBaseData);
DEFINE_SCRIPT_CLASS_EXTERN(SlaveCenter);
DEFINE_SCRIPT_CLASS_EXTERN(HorseBaseData);
DEFINE_SCRIPT_CLASS_EXTERN(HorseCenter);
DEFINE_SCRIPT_CLASS_EXTERN(UserRecord);
static void stackDump (lua_State *L) {
	int i; 
	int top = lua_gettop(L); 
	for (i = 1; i <= top; i++) {  /* repeat for each level */ 
		int t = lua_type(L, i); 
		switch (t) { 
		case LUA_TSTRING:  /* strings */ 
			printf("`%s'", lua_tostring(L, i)); 
			break; 
		case LUA_TBOOLEAN:  /* booleans */ 
			printf(lua_toboolean(L, i) ? "true" : "false"); 
			break; 
		case LUA_TNUMBER:  /* numbers */ 
			printf("%g", lua_tonumber(L, i)); 
			break; 
		default:  /* other values */ 
			printf("%s", lua_typename(L, t)); 
			break; 
		} 
		printf("  ");  /* put a separator */ 
	} 
	printf("\n");   /* end the listing */ 
}

class CLuaStateManagerPlus : public CLuaStateManager
{
public:
	CLuaState * CreateLuaState(void);
};

class CTM : public CLuaClassObject<CTM>
{
public:
	CTM(tm time)
	{
		CLuaClassObject<CTM>::Init(this);
		m_time = time;
	}

	int GetDay(void)	{ return m_time.tm_wday; }
	int GetHour(void)	{ return m_time.tm_hour; }
	int GetMin(void)	{ return m_time.tm_min; }
	int GetSec(void)	{ return m_time.tm_sec; }

	static void CreateScriptClass(CLuaState * pLuaState)
	{
		CLuaClass<CTM> * pscTm = pLuaState->CreateScriptClass((CTM*)0);

		pscTm->DefObjectFunction("GetDay", &CTM::GetDay)
			.DefObjectFunction("GetHour", &CTM::GetHour)
			.DefObjectFunction("GetMin", &CTM::GetMin)
			.DefObjectFunction("GetSec", &CTM::GetSec);
	}

	static void ReleaseScriptClass(CLuaState * pLuaState)
	{
		pLuaState->ReleaseScriptClass((CTM*)0);
	}

private:
	tm m_time;
};

#endif