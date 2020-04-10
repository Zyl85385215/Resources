#include "ClientFlag.h"
#include <stdlib.h>

ClientFlagCenter::ClientFlagCenter()
{
	memset(szbySLimit, 0,SHOPLIMIT_MAX);
}

ClientFlagCenter::~ClientFlagCenter()
{


}

int ClientFlagCenter::GetFlag( WORD wID )
{
	std::map<WORD,int>::iterator	itor	= mapFlags.find(wID);
	if(itor	!= mapFlags.end())
	{
		return itor->second;
	}
	return 0;
}

int ClientFlagCenter::GetCPlimitF( WORD wID )
{
	std::map<WORD,BYTE>::iterator	itor	= mapCopyLmt.find(wID);
	if(itor	!= mapCopyLmt.end())
	{
		return itor->second;
	}
	return 0;
}

int ClientFlagCenter::GetSLimitF( WORD wID )
{
	if(wID >= SHOPLIMIT_MAX)
		return 0;
	return szbySLimit[wID];
}

int ClientFlagCenter::GetQLineStep( WORD wLine )
{
	std::map<WORD,BYTE>::iterator	itor	= mapQLineStep.find(wLine);
	if(itor	!= mapQLineStep.end())
	{
		return itor->second;
	}
	return 0;
}

int ClientFlagCenter::GetQDayTime( WORD wQID )
{
	std::map<WORD,BYTE>::iterator	itor	= mapDayTime.find(wQID);
	if(itor	!= mapDayTime.end())
	{
		return itor->second;
	}
	return 0;
}



