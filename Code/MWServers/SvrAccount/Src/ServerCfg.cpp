#include "stdafx.h"
#include "ServerCfg.h"
#include "xnExtstr.h"

#include "md5Capi.h"

CreateSingleton(ServerCfg);

ServerCfg::ServerCfg()
{
	pListChildAcc	= xnList::Create();
}

ServerCfg::~ServerCfg()
{
	while(pListChildAcc->Count)
	{
		delete (_childAccSvrIP*)pListChildAcc->Delete(0);
	}
	pListChildAcc->Free();
}

void ServerCfg::LoadFile( char* szFile )
{
	xnExtstr* pStr	= xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;

	while(pListChildAcc->Count)
		delete (_childAccSvrIP*)pListChildAcc->Delete(0);
	
	if(pStr->SetSection(0))
	{
		strcpy(szAccIP,pStr->ReadString("ip","127.0.0.1"));
		nAccPort	= atoi(pStr->ReadString("port","9801"));

		strcpy(szHttpIP,pStr->ReadString("httpip","127.0.0.1"));
		nHttpPort	= atoi(pStr->ReadString("httpport","8901"));
		nServerID	= atoi(pStr->ReadString("serverid","101"));
		strcpy(szMainIP,pStr->ReadString("mainaccip","127.0.0.1"));
		nMainPort	= atoi(pStr->ReadString("mainaccport","9801"));


		strcpy(szDBIP,pStr->ReadString("dbip","127.0.0.1"));
		strcpy(szDBUser,pStr->ReadString("dbuser","root"));
		strcpy(szDBPswd,pStr->ReadString("dbpswd","123456"));
		bool bMd5Lock	= atoi(pStr->ReadString("md5lock","0"));
		if(bMd5Lock)
		{
			Cmd5Capi	md5Capi;
			strcat(szDBPswd,"Mwzyl");
			char* szMd5	= md5Capi.Digest(szDBPswd);
			strcpy(szDBPswd,szMd5);
		}

		strcpy(szDBName,pStr->ReadString("dbtable","mw_rpg_table"));
		strcpy(szLogDB,pStr->ReadString("logdbtable","mw_rpg_log_table"));

		int	nChildAccCnt	= atoi(pStr->ReadString("acccnt","0"));
		char	szTmp[32];
		for (int i = 0; i < nChildAccCnt; i++)
		{
			_childAccSvrIP* pCAcc	= new _childAccSvrIP();
			sprintf(szTmp,"caccip%d",i+1);
			strcpy(pCAcc->szHttpIP,pStr->ReadString(szTmp,""));
			sprintf(szTmp,"caccport%d",i+1);
			pCAcc->nHttpPort	= atoi(pStr->ReadString(szTmp,"8901"));
			sprintf(szTmp,"caccsvrid%d",i+1);
			pCAcc->nSvrID	= atoi(pStr->ReadString(szTmp,"101"));
			pListChildAcc->Add(pCAcc);
		}
		

	}
}

_childAccSvrIP* ServerCfg::GetAccSvr( int nSvrID )
{
	for (int i = 0 ; i< pListChildAcc->Count; i++)
	{
		_childAccSvrIP* pRtl	= (_childAccSvrIP*)pListChildAcc->Items[i];
		if(pRtl->nSvrID == nSvrID)
			return pRtl;
	}
	return NULL;
}


