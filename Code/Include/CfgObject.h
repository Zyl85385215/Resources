#pragma once

//#include "singleton.h"
#include <map>

typedef std::map<char*, char*> map_CfgObjs;
class CCfgObject// : public CSingleton<CCfgObject>
{
public:
	CCfgObject(char* szFile);
	~CCfgObject();

	int		GetInt(char* szSection,char* szKey,int nDef = 0);	//aaa_bb
	char*	GetString(char* szSection,char* szKey);
private:
	map_CfgObjs		m_mapCfg;
};