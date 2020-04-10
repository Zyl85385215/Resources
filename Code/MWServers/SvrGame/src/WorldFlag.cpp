#include "stdafx.h"
#include "WorldFlag.h"
#include "DBControl.h"

void WorldFlag::InitDB( DBControl* pDb )
{
	m_pDB	= pDb;
	QueryRlt xRlt;
	if(!pDb->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_WORLDFLAG_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		int nId = pCmd->GetInt("id");
		std::string szData	= pCmd->GetStr("flagdata");
		m_WorldFlag[nId] = szData;	
	}
}

int WorldFlag::GetFlagInt( int nType,int nDefault /*= 0*/ )
{
	std::map<int,std::string>::iterator itor = m_WorldFlag.find(nType);
	if (itor != m_WorldFlag.end())
	{
		return atoi(itor->second.c_str());
	}
	return nDefault;
}

const char* WorldFlag::GetFlagString( int nType,const char* szDefault /*= ""*/ )
{
	std::map<int,std::string>::iterator itor = m_WorldFlag.find(nType);
	if (itor != m_WorldFlag.end())
	{
		return itor->second.c_str();
	}
	return szDefault;
}

void WorldFlag::SetFlagInt( int nType,int nValue )
{
	std::map<int,std::string>::iterator itor = m_WorldFlag.find(nType);
	char sztxt[16];
	sprintf(sztxt,"%d",nValue);
	if (itor != m_WorldFlag.end())
	{
		itor->second = sztxt;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_WORLDFLAG_TABLE" set flagdata=%s where id=%d",sztxt,nType);
	}else{
		m_WorldFlag[nType] = sztxt;
		m_pDB->ExecuteSQL(FALSE,"insert into "RPG_WORLDFLAG_TABLE" (id, flagdata) values (%d, '%s')",nType,sztxt);
	}
}

void WorldFlag::SetFlagString( int nType,const char* szValue )
{
	std::map<int,std::string>::iterator itor = m_WorldFlag.find(nType);
	if (itor != m_WorldFlag.end())
	{
		itor->second = szValue;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_WORLDFLAG_TABLE" set flagdata=%s where id=%d",szValue,nType);
	}else{
		m_WorldFlag[nType] = szValue;
		m_pDB->ExecuteSQL(FALSE,"insert into "RPG_WORLDFLAG_TABLE" (id, flagdata) values (%d, '%s')",nType,szValue);
	}
}
