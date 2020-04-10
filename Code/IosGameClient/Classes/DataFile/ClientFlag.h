#pragma once

#include "GlobalDefine.h"
#include <map>

enum	SHOPLIMITFLAG
{
	SHOPLIMIT_NEWUSERGIFT,		//ÐÂÊÖÀñ°ü




	SHOPLIMIT_MAX = 32,
};

class ClientFlagCenter
{
public:
	ClientFlagCenter();
	~ClientFlagCenter();

	int		GetFlag(WORD wID);
	int		GetSLimitF(WORD wID);
	int		GetCPlimitF(WORD wID);

	int		GetQLineStep(WORD wLine);
	int		GetQDayTime(WORD wQID);

	std::map<WORD, int>	mapFlags;
	std::map<WORD, int>	mapCD;
	BYTE				szbySLimit[SHOPLIMIT_MAX];
	std::map<WORD, BYTE>	mapQLineStep;
	std::map<WORD, BYTE>	mapDayTime;
	std::map<WORD, BYTE>	mapCopyLmt;
};