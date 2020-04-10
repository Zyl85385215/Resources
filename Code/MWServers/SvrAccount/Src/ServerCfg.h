#pragma once

#include "singleton.h"
#include "xnlist.h"

class _childAccSvrIP
{
public:
	char		szHttpIP[20];
	int			nHttpPort;
	int			nSvrID;
};

class ServerCfg	: public CSingleton<ServerCfg>
{
public:
	ServerCfg();
	~ServerCfg();
	void		LoadFile(char* szFile);
	_childAccSvrIP*	GetAccSvr(int nSvrID);


	char		szAccIP[20];
	int			nAccPort;
	char		szMainIP[20];
	int			nMainPort;
	char		szDBIP[20];
	char		szDBUser[20];
	char		szDBPswd[50];
	char		szDBName[20];
	char		szLogDB[20];
	char		szHttpIP[20];
	int			nHttpPort;
	int			nServerID;
	xnList*		pListChildAcc;
};