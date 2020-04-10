#ifndef CLIENT_LUA_MANAGER_H
#define CLIENT_LUA_MANAGER_H

#include "cocos2d.h" 
using namespace cocos2d; 
using namespace std; 
  
extern "C" { 
#include "lua.h" 
#include "lualib.h" 
#include "lauxlib.h" 
}; 
  

class DForm;
class DComponent;
class ClientLuaManager{ 
public: 
    static ClientLuaManager* sharedHD(); 
  
        //------------  c++ -> lua ------------// 
  
    /* 
        getLuaVarString : 调用lua全局string 
  
        luaFileName  = lua文件名 
        varName = 所要取Lua中的变量名 www.it165.net
     */
    const char* getLuaVarString(const char* luaFileName,const char* varName); 
  
    /* 
     getLuaVarOneOfTable : 调用lua全局table中的一个元素 
  
     luaFileName  = lua文件名 
     varName = 所要取Lua中的table变量名 
     keyName = 所要取Lua中的table中某一个元素的Key 
     */
    const char* getLuaVarOneOfTable(const char* luaFileName,const char* varName,const char* keyName); 
  
    /* 
     getLuaVarTable : 调用lua全局table 
  
     luaFileName  = lua文件名 
     varName = 所要取的table变量名 
  
     （注：返回的是所有的数据，童鞋们可以自己使用Map等处理） 
     */
    const char* getLuaVarTable(const char* luaFileName,const char* varName); 
  
    /* 
     callLuaFunction : 调用lua函数 
  
     luaFileName  = lua文件名 
     functionName = 所要调用Lua中的的函数名 
     */
    const int callLuaFunction(const char* luaFileName,const char* functionName); 

	void	DoFile(char* pName);

	const int Update();
	const int FormOpen(DForm* pForm);
	const int FormClose(DForm* pForm);
	const int ButtonClick(DForm* pForm,DComponent* pComp);
	const int PacketRecv(int* szVal);
 
private: 
  
    static bool _isFirst; 
    static ClientLuaManager* _shared; 
    const char* getFileFullPath(const char* fileName); 
    ~ClientLuaManager(); 
}; 


#endif