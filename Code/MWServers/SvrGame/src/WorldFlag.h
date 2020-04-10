#pragma once
#include <map>
#include <string>
class DBControl;
class WorldFlag{
public:
	std::map<int,std::string> m_WorldFlag;
	void	InitDB(DBControl* pDb);

	int		GetFlagInt(int nType,int nDefault = 0);
	const char*	GetFlagString(int nType,const char* szDefault = "");
	void	SetFlagInt(int nType,int nValue);
	void	SetFlagString(int nType,const char* szValue);
private:
	DBControl*	m_pDB;

};