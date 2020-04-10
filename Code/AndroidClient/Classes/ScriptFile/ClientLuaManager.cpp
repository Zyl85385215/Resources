#include "stdafx.h"
#include "ClientLuaManager.h"
#include "tolua++.h"

#include "CCLuaEngine.h" 
#include "MainMenu.h"
#include "Interface.h"
  
ClientLuaManager* ClientLuaManager::_shared = NULL; 

  
ClientLuaManager* ClientLuaManager::sharedHD(){ 
    if(!_shared){ 
        _shared = new ClientLuaManager(); 
    } 
    return _shared; 
} 
  
const char* ClientLuaManager::getLuaVarString(const char* luaFileName,const char* varName){ 
  
	lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_settop(ls, 0); 
    lua_getglobal(ls, varName); 
  
    int statesCode = lua_isstring(ls, 1); 
    if(statesCode!=1){ 
        CCLOG("Open Lua Error: %i", statesCode); 
        return NULL; 
    } 
  
    const char* str = lua_tostring(ls, 1); 
    lua_pop(ls, 1); 
  
    return str; 
} 
  
const char* ClientLuaManager::getLuaVarOneOfTable(const char* luaFileName,const char* varName,const char* keyName){ 
  
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_getglobal(ls, varName); 
  
    int statesCode = lua_istable(ls, -1); 
    if(statesCode!=1){ 
        CCLOG("Open Lua Error: %i", statesCode); 
        return NULL; 
    } 
  
    lua_pushstring(ls, keyName); 
    lua_gettable(ls, -2); 
    const char* valueString = lua_tostring(ls, -1); 
  
    lua_pop(ls, -1); 
  
    return valueString; 
} 
  
const char* ClientLuaManager::getLuaVarTable(const char* luaFileName,const char* varName){ 
    lua_State*  ls = CCLuaEngine::defaultEngine()->getLuaState(); 
  
    int isOpen = luaL_dofile(ls, getFileFullPath(luaFileName)); 
    if(isOpen!=0){ 
        CCLOG("Open Lua Error: %i", isOpen); 
        return NULL; 
    } 
  
    lua_getglobal(ls, varName); 
  
    int it = lua_gettop(ls); 
    lua_pushnil(ls); 
  
    string result=""; 
  
    while(lua_next(ls, it)) 
    { 
        string key = lua_tostring(ls, -2); 
        string value = lua_tostring(ls, -1); 
  
        result=result+key+":"+value+"\t"; 
  
        lua_pop(ls, 1); 
    } 
    lua_pop(ls, 1); 
  
    return result.c_str(); 
} 

const int ClientLuaManager::callLuaFunction(const char* luaFileName,const char* functionName){ 

	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	pEngine->executeScriptFile(luaFileName);

	lua_State* Ls = pEngine->getLuaState();
	//pEngine->executeGlobalFunction(functionName);

	lua_getglobal(Ls, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(Ls, -1))
	{
		CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(Ls, 1);
		return 0;
	}
	tolua_pushusertype(Ls,pMainMenu,"MainMenu");
	int error = lua_pcall(Ls, 1, 1, 0);             /* call function, stack: ret */
	// lua_gc(m_state, LUA_GCCOLLECT, 0);

	if (error)
	{
		CCLOG("[LUA ERROR] %s", lua_tostring(Ls, - 1));
		lua_pop(Ls, 1); // clean error message
		return 0;
	}

	// get return value
	if (!lua_isnumber(Ls, -1))
	{
		lua_pop(Ls, 1);
		return 0;
	}

	int ret = lua_tointeger(Ls, -1);
	lua_pop(Ls, 1);                                                /* stack: - */
	return ret;

} 

const int ClientLuaManager::Update(){ 

	char luaFileName[128];
	char functionName[128];
	sprintf(luaFileName,"Script/update.lua");
	sprintf(functionName,"MainUpdate");
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	//pEngine->executeScriptFile(luaFileName);

	lua_State* Ls = pEngine->getLuaState();

	lua_getglobal(Ls, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(Ls, -1))
	{
		CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(Ls, 1);
		return 0;
	}

	int error = lua_pcall(Ls, 0, 1, 0);             /* call function, stack: ret */
	// lua_gc(m_state, LUA_GCCOLLECT, 0);

	if (error)
	{
		CCLOG("[LUA ERROR] %s", lua_tostring(Ls, - 1));
		lua_pop(Ls, 1); // clean error message
		return 0;
	}

	// get return value
	if (!lua_isnumber(Ls, -1))
	{
		lua_pop(Ls, 1);
		return 0;
	}

	int ret = lua_tointeger(Ls, -1);
	lua_pop(Ls, 1);                                                /* stack: - */
	return ret;

} 

void	ClientLuaManager::DoFile(char* pName)
{
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	pEngine->executeScriptFile(getFileFullPath(pName));
}

const int ClientLuaManager::PacketRecv(int* szVal){ 

	char luaFileName[128];
	char functionName[128];
	sprintf(luaFileName,"Script/packet.lua");
	sprintf(functionName,"OnRecvPacket");
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	//pEngine->executeScriptFile(luaFileName);

	lua_State* Ls = pEngine->getLuaState();

	lua_getglobal(Ls, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(Ls, -1))
	{
		CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(Ls, 1);
		return 0;
	}
	for(int i = 0; i < 5; i ++)
		tolua_pushnumber(Ls,szVal[i]);
	int error = lua_pcall(Ls, 5, 1, 0);             /* call function, stack: ret */
	// lua_gc(m_state, LUA_GCCOLLECT, 0);

	if (error)
	{
		CCLOG("[LUA ERROR] %s", lua_tostring(Ls, - 1));
		lua_pop(Ls, 1); // clean error message
		return 0;
	}

	// get return value
	if (!lua_isnumber(Ls, -1))
	{
		lua_pop(Ls, 1);
		return 0;
	}

	int ret = lua_tointeger(Ls, -1);
	lua_pop(Ls, 1);                                                /* stack: - */
	return ret;

} 

const int ClientLuaManager::FormOpen(DForm* pForm){ 

	char luaFileName[128];
	char functionName[128];
	sprintf(luaFileName,"Script/event.lua");
	sprintf(functionName,"OnFormOpen");
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	//pEngine->executeScriptFile(luaFileName);

	lua_State* Ls = pEngine->getLuaState();

	lua_getglobal(Ls, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(Ls, -1))
	{
		CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(Ls, 1);
		return 0;
	}

	tolua_pushusertype(Ls,pForm,"DForm");
	int error = lua_pcall(Ls, 1, 1, 0);             /* call function, stack: ret */
	// lua_gc(m_state, LUA_GCCOLLECT, 0);

	if (error)
	{
		CCLOG("[LUA ERROR] %s", lua_tostring(Ls, - 1));
		lua_pop(Ls, 1); // clean error message
		return 0;
	}

	// get return value
	if (!lua_isnumber(Ls, -1))
	{
		lua_pop(Ls, 1);
		return 0;
	}

	int ret = lua_tointeger(Ls, -1);
	lua_pop(Ls, 1);                                                /* stack: - */
	return ret;

} 

const int ClientLuaManager::FormClose(DForm* pForm){ 

	char luaFileName[128];
	char functionName[128];
	sprintf(luaFileName,"Script/event.lua");
	sprintf(functionName,"OnFormClose");
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	//pEngine->executeScriptFile(luaFileName);

	lua_State* Ls = pEngine->getLuaState();

	lua_getglobal(Ls, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(Ls, -1))
	{
		CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(Ls, 1);
		return 0;
	}

	tolua_pushusertype(Ls,pForm,"DForm");
	int error = lua_pcall(Ls, 1, 1, 0);             /* call function, stack: ret */
	// lua_gc(m_state, LUA_GCCOLLECT, 0);

	if (error)
	{
		CCLOG("[LUA ERROR] %s", lua_tostring(Ls, - 1));
		lua_pop(Ls, 1); // clean error message
		return 0;
	}

	// get return value
	if (!lua_isnumber(Ls, -1))
	{
		lua_pop(Ls, 1);
		return 0;
	}

	int ret = lua_tointeger(Ls, -1);
	lua_pop(Ls, 1);                                                /* stack: - */
	return ret;

} 

const int ClientLuaManager::ButtonClick(DForm* pForm,DComponent* pComp){ 

	char luaFileName[128];
	char functionName[128];
	sprintf(luaFileName,"Script/event.lua");
	sprintf(functionName,"OnClickButton");
	CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();  
	//pEngine->executeScriptFile(luaFileName);

	lua_State* Ls = pEngine->getLuaState();

	lua_getglobal(Ls, functionName);       /* query function by name, stack: function */
	if (!lua_isfunction(Ls, -1))
	{
		CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName);
		lua_pop(Ls, 1);
		return 0;
	}

	tolua_pushusertype(Ls,pForm,"DForm");
	tolua_pushusertype(Ls,pComp,"DComponent");
	int error = lua_pcall(Ls, 2, 1, 0);             /* call function, stack: ret */
	// lua_gc(m_state, LUA_GCCOLLECT, 0);

	if (error)
	{
		CCLOG("[LUA ERROR] %s", lua_tostring(Ls, - 1));
		lua_pop(Ls, 1); // clean error message
		return 0;
	}

	// get return value
	if (!lua_isnumber(Ls, -1))
	{
		lua_pop(Ls, 1);
		return 0;
	}

	int ret = lua_tointeger(Ls, -1);
	lua_pop(Ls, 1);                                                /* stack: - */
	return ret;

} 
  
const char* ClientLuaManager::getFileFullPath(const char* fileName){ 
	return CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(fileName);
} 
  
ClientLuaManager::~ClientLuaManager(){ 
  
    CC_SAFE_DELETE(_shared); 
    _shared=NULL; 
}