#pragma once
#include "xnlist.h"
#include <map>
#include "LuaClassObject.h"
#include "LuaStateManagerPlus.h"

using namespace std;
class UserData;
class MapServerObj:public CLuaClassObject<MapServerObj>
{
public:
	MapServerObj(int nIndex,int nBase);
	~MapServerObj();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
	int		GetIndex();
	int		GetBaseID();
	int		GetUserCount();
	void	ChgMapForce(int nFType);
	void	ChgMapEnemyPow(int	nExPow, int nLv	= 0);
	UserData*	GetUserByPos(int nPos);

	LUAFUN void	GetAllUser(LuaStackObject luaTable);

	void	Update();

	int		m_nIndex;
	int		m_nBaseID;
	
	xnList*	m_pList;

};
class MapObjCenter:public CLuaClassObject<MapObjCenter>
{
public:
	MapObjCenter();
	static void			CreateScriptClass(CLuaState * pLuaState);
	static void			ReleaseScriptClass(CLuaState * pLuaState);
	MapServerObj*	GetMapObjByIndex(int nIndex);
	MapServerObj* CreateMapObj(int nIndex,int nBase);

	void	Init();

	bool	CheckJoin(UserData* pUser,int nIndex,int nBaseID);
	void	Join(UserData* pUser,int nIndex,int nBase);
	bool	CheckLeave(UserData* pUser,int nIndex);
	void	Leave(UserData* pUser,int nIndex);

	void	UpdateSec();
	void	RemoveMapObj(int nIndex);
private:
	bool	_runLuaFunctionForMap(MapServerObj* pObj,UserData* pUser,const char* const szFunctionName);
	bool	_runLuaFunctionForMap(int nBaseID,UserData* pUser,const char* const szFunctionName);
	map<int,MapServerObj*> m_MapObj;
};