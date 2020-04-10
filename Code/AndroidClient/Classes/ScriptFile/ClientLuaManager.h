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
        getLuaVarString : ����luaȫ��string 
  
        luaFileName  = lua�ļ��� 
        varName = ��ҪȡLua�еı����� www.it165.net
     */
    const char* getLuaVarString(const char* luaFileName,const char* varName); 
  
    /* 
     getLuaVarOneOfTable : ����luaȫ��table�е�һ��Ԫ�� 
  
     luaFileName  = lua�ļ��� 
     varName = ��ҪȡLua�е�table������ 
     keyName = ��ҪȡLua�е�table��ĳһ��Ԫ�ص�Key 
     */
    const char* getLuaVarOneOfTable(const char* luaFileName,const char* varName,const char* keyName); 
  
    /* 
     getLuaVarTable : ����luaȫ��table 
  
     luaFileName  = lua�ļ��� 
     varName = ��Ҫȡ��table������ 
  
     ��ע�����ص������е����ݣ�ͯЬ�ǿ����Լ�ʹ��Map�ȴ��� 
     */
    const char* getLuaVarTable(const char* luaFileName,const char* varName); 
  
    /* 
     callLuaFunction : ����lua���� 
  
     luaFileName  = lua�ļ��� 
     functionName = ��Ҫ����Lua�еĵĺ����� 
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