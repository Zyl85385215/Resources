#include "stdafx.h"
#include "DBControl.h"
#include "DBIO.h"

#include "FsGameCenter.h"
#include "UserData.h"
#include "ios_ac_packet.h"
#include <mbstring.h>
#include "UserList.h"
#include "ios_user_packet.h"
#include "debug_def.h"
#include "MarkGameDB.h"
#include "MailCenter.h"

DBControl*	g_pDBase	= NULL;

void	DBThreadFun(void * data, MSG * Msg)
{
	DBControl * pThisDBase = (DBControl *)data;
	switch (Msg->message)
	{
	case	WM_JOIN_GAME:
		{
			pThisDBase->UserJoinGame((ZPacket*)Msg->lParam,true);
			free((ZPacket*)Msg->lParam);
		}
		break;
	case	WM_CREATE_USER:
		{
			int	nError	= pThisDBase->CreateUser((ZPacket*)Msg->lParam,true);
			if(nError)
			{//创建名字失败
				BuildPacketEx(pSendPacket,USER,S_CREATE_CHARACTER_FAIL,buf,256);
				SET_DATA(pSend, USER,S_CREATE_CHARACTER_FAIL,pSendPacket);
				pSend->bErrorCode	= nError;
				TCP_SendPacketToKey(((ZPacket*)Msg->lParam)->dwKey,pSendPacket);
			}
			free((ZPacket*)Msg->lParam);
		}
		break;
	case	WM_EXCUTE_SQL:
		{
			SQLCmd* pCmd = (SQLCmd*)Msg->lParam;
			if(pCmd != NULL)
			{
				pThisDBase->ExecuteSQL(true,pCmd->szSqlCmd);

				if(pCmd)
					delete pCmd;
			}
		}
		break;
	case    WM_SET_BOLB:
		{
			DB_BLOB* pBlob = (DB_BLOB*)Msg->lParam;
			if(pBlob != NULL)
			{
				pThisDBase->pDBServer->SetBLOB(pBlob->data,pBlob->size,pBlob->sql->szSqlCmd);
				free(pBlob->data);
				if(pBlob->sql)
					delete pBlob->sql;
				free(pBlob);
			}
		}
		break;
	case	WM_EXCUTE_SQL_PARAMS:
		{

		}
		break;
	}
}

BOOLEAN DBaseThreadProc(void * data, MSG * Msg)
{
#ifndef _DEBUG
	__try
#endif
	{
		DBThreadFun(data,Msg);
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		char	szDmpMsg[256];
		sprintf(szDmpMsg,"DBaseThreadProc Error: %d\n",Msg->message);
		DUMP_STRING(szDmpMsg);
	}
#endif
	return TRUE;
}


bool DBControl::ConnectToDB( const char * ip, const char * userName, const char * password, const char * dBaseName )
{
	return pDBServer->ConnectDB(ip, userName, password, dBaseName, false);
}

void DBControl::Start(void)
{
	if (this != NULL)
	{
		printf("PlayerDB%d Start.\n",pDBServer->m_dwID);
		m_pThread->Start();
	}
}

void DBControl::Stop(void)
{
	if (this != NULL)
	{
		Sleep(1000);
		m_pThread->Stop(60*30*1000);                          //
	}
}

void DBControl::Reborn( char * threadName )
{
	if (!m_pThread->IsActive())
	{
		m_pThread->Stop(0);
		delete m_pThread;

		m_pThread = new xnThread(DBaseThreadProc, this, threadName);
		m_pThread->SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);

		//OutMessage(msg_type_error,"PlayerDB%d ReStart.\n", pDBServer->m_dwID);
		m_pThread->Start();
	}
}

BOOLEAN DBControl::ExecuteSQL( BOOLEAN bImmediately, char* szFormat, ... )
{
	if (!this) return false;

	char szSQL[4096] = {0};
	vsprintf(szSQL, szFormat, (char*) (&szFormat + 1));	
//#ifdef RELEASE_TEST
//	bImmediately = true;
//#endif
	if(!bImmediately && m_pThread->IsRunning())
	{
		SQLCmd* pCmd = new SQLCmd(szSQL);
		m_pThread->SendMessage(WM_EXCUTE_SQL, 0, (LPARAM)pCmd, true);
	}
	else
	{
		//if(!m_pThread->IsRunning()) OutMessage(msg_type_error,"DB Thread Is Not Running!\n");

		DWORD dwTick = GetTickCount();
		
		BOOLEAN rlt = pDBServer->ExecuteSQL(szSQL);
		dwTick = GetTickCount() - dwTick;
		if(dwTick > 1000 || !rlt)
			OutMessage(msg_type_error,"ExecuteSQL ERROR: %s(%dms)",szSQL,dwTick);
		else
			OutMessage(msg_type_sql,szSQL);
		return rlt;
	}
	return true;
}

bool DBControl::SetBLOB(BOOLEAN waitReply, void * data, int size, char* szFormat, ...)    
{
	if (!this) return false;

	char szSQL[4096] = {0};
	vsprintf(szSQL, szFormat, (char*) (&szFormat + 1));
//#ifdef RELEASE_TEST
//	waitReply = true;
//#endif
	if(waitReply)
	{
		return pDBServer->SetBLOB((BYTE*)data, size, szSQL);
	}
	else
	{
		DB_BLOB* pBlob = (DB_BLOB*)malloc(sizeof(DB_BLOB));
		pBlob->data = (BYTE*)malloc(size+1);
		memcpy(pBlob->data,(BYTE*)data,size);
		pBlob->size = size;
		pBlob->sql = new SQLCmd(szSQL);
		OutMessage(msg_type_sql,szSQL);
		m_pThread->SendMessage(WM_SET_BOLB, 0, (LPARAM)pBlob);
	}
	return true;
}
//	private

BOOLEAN DBControl::_GetBLOB(void * data, int bufSize, char* szFormat, ...)
{
	if (this != NULL)
	{
		char szSQL[4096] = {0};
		vsprintf(szSQL, szFormat, (char*) (&szFormat + 1));
		return pDBServer->GetBLOB((BYTE*)data, bufSize, szSQL);
	}
	return FALSE;
}

BOOLEAN DBControl::_GetBLOB(void* data, int bufSize, int &dataSize, char* szFormat, ...)
{
	if (this != NULL)
	{
		char szSQL[4096] = {0};
		vsprintf(szSQL, szFormat, (char*) (&szFormat + 1));
		return pDBServer->GetBLOB((BYTE*)data, bufSize, dataSize, szSQL);
	}
	return FALSE;
}

DBControl::DBControl( char * threadName, int nGameLogID /*= -1*/ )
{
	m_pThread = new xnThread(DBaseThreadProc, this, threadName);
	m_pThread->SetPriority(THREAD_PRIORITY_ABOVE_NORMAL);
	pDBServer	= new DBIO("maindb");
}

DBControl::~DBControl( void )
{
	Stop();
	Sleep(1000);
	if(m_pThread)
		delete m_pThread;
	if(pDBServer)
		delete pDBServer;
}

bool CheckNameValid(char* szName)
{
	char* rstr = szName;
	int strLen;
	while ((strLen = strlen(rstr)) > 0)
	{
		int d = _mbsnextc((unsigned char *)rstr);
		if ((d >> 8) == 0)
		{
			if(d<48 || (d>57&&d<65) || (d>90&&d<97) || d>122)
				return false;
			rstr++;
		}
		else
		{
			if (d>>8 >= 0xaa && d>>8 <= 0xaf || d>>8 >= 0xfa && d>>8 <= 0xff)
				return false;
			rstr += 2;
		}
	}
	return true;
}

BYTE DBControl::CreateUser( ZPacket* p, BOOLEAN immediately /*= FALSE*/ )
{
	//	插入登录队列并返回当前队列长度
	if (!immediately)
	{
		m_pThread->SendMessage(WM_CREATE_USER, 0, (long)p,true);
		return 0;
	}
	//	正式处理登陆
	else
	{	
		SET_DATA(pJoin,ACCOUNT,C_CREATE_CHARACTER,p);

		//	判断是否双重登录
		if(strlen(pJoin->szName)< 4 || strlen(pJoin->szName) > 14 || !CheckNameValid(pJoin->szName))
		{//失败
			return 2;
		}
		if(atoi(pJoin->szName))
		{//失败
			return 2;
		}
		if(pJoin->bSex > 1)
			return 3;
		if(pJoin->bJob > 4 || pJoin->bJob == 0)
			return 3;
		if(pJoin->bCountry > 2 || pJoin->bCountry == 0)
			return 3;
		DBIO* pDBSvr	= this->pDBServer;
		QueryRlt xRlt;

		if(strlen(pJoin->szInvite))
		{
			if(!pDBSvr->Query(&xRlt,	"SELECT * FROM "RPG_INVITE_TABLE" where invitecode = '%s'", pJoin->szInvite))
			{
				return 4;
			}
			MySqlQuery* pCmd1 = xRlt.GetRlt();               

			//	帐号不存在
			if(pCmd1 == NULL || !pCmd1->FetchRow())
			{
				return 4;
			}
		}
		
		if(pDBSvr->Query(&xRlt,"SELECT * FROM "RPG_USER_TABLE" WHERE Nickname = '%s'", 	pJoin->szName)) 
		{
			MySqlQuery* pCmd = xRlt.GetRlt();
			if (pCmd != NULL)
			{
				if(pCmd->FetchRow())	//已存在就返回失败
					return 5;
			}
		}
		if (g_pGameCenter->m_nLockCountry == pJoin->bCountry)
		{
			return 6;
		}

		
		int nCountry	= pJoin->bCountry*100+3;

		if(!pDBSvr->ExecuteSQL("insert into "RPG_USER_TABLE" (Uin,AccountUin,job,country,sex,LoginTime,LogoutTime,Nickname,nextitemid,power,skillpt,lvl,flag,cfgflag,slimitflag,questflag,cdflag,getslavelimit,\
															  mapindex,mapx,mapy,rmindex,rmposx,rmposy,homeskill,activedegree,starlv,expmultiple,makeskill,ResetTime,sysmailid) \
								 VALUES(%d,%d,%d,%d,%d,now(),now(),'%s',1,50,0,1,'','','','','',0,%d,2900,2300,%d,2900,2300,'','',0,'','',now(),%d)", 

								 g_pUserList->m_dwMaxUin++,pJoin->dwAcc,pJoin->bJob,pJoin->bCountry,pJoin->bSex,pJoin->szName,nCountry,nCountry,g_pGameCenter->m_pSysMailCenter->m_nNextID-1))
		{
			//g_pUserList->m_dwMaxUin++;
			return 1;
		}
		if(pDBSvr->Query(&xRlt,"SELECT * FROM "RPG_ACCOUNT_TABLE" WHERE AccountUin=%d", 	pJoin->dwAcc)) 
		{
			MySqlQuery* pCmd = xRlt.GetRlt();
			if(pCmd && pCmd->FetchRow())	//已存在就返回失败
			{
				g_pGameCenter->SendPValInfo(pCmd->GetInt("tpid"),0);
			}
		}

		BuildPacketEx(pSIPacket, ACCOUNT, SIMPLEINFOS,bufSI, 1024);
		SET_DATA(pSIData, ACCOUNT, SIMPLEINFOS, pSIPacket);
		pSIData->bCnt	= 0;
		if(pDBSvr->Query(&xRlt,
			"SELECT * FROM "RPG_USER_TABLE" where AccountUin = %d", pJoin->dwAcc))
		{
			MySqlQuery* pCmd = xRlt.GetRlt();          
			while(pCmd && pCmd->FetchRow())
			{
				pSIData->szUSInfos[pSIData->bCnt].dwUin		= pCmd->GetInt("Uin");
				pSIData->szUSInfos[pSIData->bCnt].byLv		= pCmd->GetInt("lvl");
				pSIData->szUSInfos[pSIData->bCnt].byJob		= pCmd->GetInt("job");
				pSIData->szUSInfos[pSIData->bCnt].byCountry		= pCmd->GetInt("country");
				pSIData->szUSInfos[pSIData->bCnt].bySex		= pCmd->GetInt("sex");
				strcpy(pSIData->szUSInfos[pSIData->bCnt].szNick,pCmd->GetStr("Nickname"));
				pSIData->bCnt++;
			}
			pSIPacket->wSize	+= sizeof(USimpleInfo)*pSIData->bCnt;
			//if(pSIData->bCnt)
			//	pSIData->bCnt += 100;
			TCP_SendPacketToKey(p->dwKey,pSIPacket);
		}
// 		UserData * pUser = new UserData(g_pUserList->m_dwMaxUin,this);
// 		g_pUserList->m_dwMaxUin++;
// 		pUser->m_dwAccount	= pJoin->dwAcc;
// 		strcpy(pUser->m_szNick,pJoin->szName);
// 		pUser->m_dwKey	= p->dwKey;		//网络识别号赋值
// 		pUser->m_byCountry		= pJoin->bCountry;
// 		pUser->m_byJob		= pJoin->bJob;
// 		pUser->m_bySex		= pJoin->bSex;
// 		pUser->m_nSysMailID = g_pGameCenter->m_pSysMailCenter->m_nNextID-1;
// 		pUser->m_dwResetTime = CMTime::GetCurTickCount();
// 
// 		
		if(strlen(pJoin->szInvite))
			pDBSvr->ExecuteSQL("update "RPG_ACCOUNT_TABLE" set beinvitecode='%s' WHERE AccountUin=%d", pJoin->szInvite,	pJoin->dwAcc);
		
// 		pUser->CreateInit();
// 		g_pGameCenter->SendPValInfo(pUser->m_nTPID,0);
// 		UserSimple* pUsp = new UserSimple;
// 		pUsp->bCountry = pUser->m_byCountry;
// 		pUsp->bJob = pUser->m_byJob;
// 		pUsp->bLv = 1;
// 		pUsp->bSex = pUser->m_bySex;
// 		pUsp->dwUin = pUser->m_dwUin;
// 		pUsp->szName = strdup(pUser->m_szNick);
// 		pUsp->bOnline = true;
// 		g_pUserList->AddUser(pUsp);
// 
// 		pUser->m_pMySimple = pUsp;
// 
// 		g_pMarkGameDB->ChgOnline(true,true);
// 		g_pGameCenter->DBT_PlayerJoinWait(pUser);
	}
	return 0;
}

bool DBControl::UserJoinGame( ZPacket* p, BOOLEAN immediately /*= FALSE*/ )
{
	//	插入登录队列并返回当前队列长度
	if (!immediately)
	{
		m_pThread->SendMessage(WM_JOIN_GAME, 0, (long)p,true);
	}
	//	正式处理登陆
	else
	{	
		SET_DATA(pJoin,ACCOUNT,C_REQUEST_JOIN,p);

		//	判断是否双重登录
		UserData * pData = g_pGameCenter->GetPlayer(pJoin->dwUin);
		int		nJoinWait	= 2;
		if(pData)
		{//请求踢出玩家, 延时2秒进入游戏
			OutMessage(msg_type_error,"USER ReJoin %d",pData->m_dwUin);
			BuildPacketEx(pSPacket, ACCOUNT,LOGOUT, pBuf,256);
			SET_DATA(pLogout, ACCOUNT,LOGOUT, pSPacket);
			pLogout->dwUin	= pData->m_dwUin;
			g_pNetCenter->RecvClientPacket(NULL,pSPacket);

			nJoinWait	= 15;

			pData	= NULL;
		}

		if (pData == NULL)
		{
			pData	= UserData::CreateOpUserByDB(pJoin->dwUin,this);
			if(pData)
			{
				pData->m_wJoinWaitTick	= nJoinWait;
				pData->m_dwKey	= p->dwKey;		//网络识别号赋值
				if(pData->m_dwAccount != pJoin->dwAcc)
				{
					OutMessage(msg_type_error,"USER Join Account != %d",pData->m_dwUin);
					delete pData;
					return false;
				}
				pData->m_pDB	= g_pGameCenter->m_ppPlayerDB[pJoin->dwUin%8];
				pData->m_pAmzDManager->m_pDB	= g_pGameCenter->m_ppPlayerDB[pJoin->dwUin%8];
				g_pGameCenter->DBT_PlayerJoinWait(pData);
			}
		}
		else
		{//已在线就先踢下线才能上
			OutMessage(msg_type_error,"USER ReJoin %d",pData->m_dwUin);
			//if(pData->m_dwAccount != pJoin->dwAcc)
			//{
			//	OutMessage(msg_type_error,"USER Join Account != %d",pData->m_dwUin);
			//	return false;
			//}
			//if(p->dwKey != pData->m_dwKey)
			//{
			//BuildPacketEx(pSendPacket,ACCOUNT,OUT_BY_RELOGIN,bufs,256);
			//g_pNetCenter->SendNGPacket(pSendPacket,pData->m_dwKey);
			//}

			//pData->m_dwKey	= p->dwKey;		//网络识别号赋值
			//g_pGameCenter->DBT_PlayerJoinWait(pData);
		}
	}
	return true;
}

