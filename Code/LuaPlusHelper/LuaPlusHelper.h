#ifndef _LUA_CLASS_H_
#define _LUA_CLASS_H_

#pragma once

#include "luaplus.h"
#include "luastatemanager.h"
#include "luaclass.h"
#include "luamodule.h"
#include "luaclassobject.h"

#if defined(DEBUG) || defined(_DEBUG)
#pragma comment(lib, "LuaPlusLibD_1084.lib")
#else
#pragma comment(lib, "LuaPlusLib_1084.lib")
#endif
using namespace LuaPlus;

#define SAFE_DELETE(p) { if(p) { delete (p); (p) = NULL; } }

#define DEFINE_SCRIPT_CLASS(classname)\
namespace LPCD\
{\
	using namespace LuaPlus;\
	void Push(lua_State* L, classname * value) { if(!value) { LuaNil nil; Push(L, nil); } else Push(L, value->CLuaClassObject<classname>::GetLuaObject()); }\
	void Push(lua_State* L, classname & value) { Push(L, value.CLuaClassObject<classname>::GetLuaObject());  }\
	inline bool	Match(TypeWrapper<classname*>, lua_State* L, int idx)\
	{  (void)L, (void)idx;  return true;  }\
	classname * Get(TypeWrapper<classname*>, lua_State* L, int idx)\
	{\
		return *static_cast<classname**>((LuaObject(LuaState::CastState(L), idx)).GetUserData());\
	}\
}\

#define DEFINE_SCRIPT_CLASS_EXTERN(classname)\
namespace LPCD\
{\
	using namespace LuaPlus;\
	extern void Push(lua_State* L, classname * value);\
	extern void Push(lua_State* L, classname & value);\
	extern inline bool	Match(TypeWrapper<classname*>, lua_State* L, int idx);\
	extern classname * Get(TypeWrapper<classname*>, lua_State* L, int idx);\
}\

#endif