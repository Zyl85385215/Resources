#include "stdafx.h"
#include "AccountCenter.h"
#include "ZNetServer.h"
#include "ServerCfg.h"
#include "DBTables.h"
#include "debug_def.h"
#include "TimeControl.h"
#include "md5Capi.h"

AccountCenter* g_pMainAccount	= NULL;

#define  WM_DEAL_ACCPACKET	WM_USER + 1501
#define  WM_RECHECK_APPPAY	WM_USER + 1502

BOOLEAN AccDealPacket(void * data, MSG * Msg)
{
	switch (Msg->message)
	{
	case	WM_DEAL_ACCPACKET:
		{
			g_pMainAccount->DealPacket();
		}
		break;
	case	WM_RECHECK_APPPAY:
		{
			g_pMainAccount->RecheckAppPay();
		}
		break;
	}
	return TRUE;
}

AccountCenter::AccountCenter()
{
	ServerCfg::GetInstance().LoadFile("ServerCfg.txt");
	m_pDBCreate		= NULL;
	srand(GetTickCount());
	InstallMyExceptHandle();

	m_nCreateCnt	= 0;
	m_nLoginCnt		= 0;
	m_pDBCreate	= new CreateDB();
	m_pLogDB	= new LogDB();
	m_bIsLockNewAcc	= false;
	if(ServerCfg::GetInstance().nServerID == 0)
		m_bIsMainAcc	= true;
	else
		m_bIsMainAcc	= false;

	m_pListPacket	= xnList::Create();
	InitializeCriticalSection(&m_xPacketLock);

	m_pLErrorIAP	= xnList::Create();
	InitializeCriticalSection(&m_xLockIAP);

	for (int i = 0; i < 20; i++)
	{
		ppThread[i]	= new xnThread(AccDealPacket, this, "AccDealPacket");
		ppThread[i]->SetPriority(THREAD_PRIORITY_NORMAL);
		ppThread[i]->Start();
	}
	m_pMarkDBCenter	= new MarkDBCenter(m_pDBCreate);
}

AccountCenter::~AccountCenter()
{
	if(m_pDBCreate)
		delete m_pDBCreate;
	if(m_pLogDB != NULL)
		delete m_pLogDB;
	m_pListPacket->Free();
	LeaveCriticalSection(&m_xPacketLock);
	m_pLErrorIAP->Free();
	LeaveCriticalSection(&m_xLockIAP);
	for (int i = 0; i < 20; i++)
	{
		ppThread[i]->Stop();
		delete ppThread[i];
	}
	delete m_pMarkDBCenter;
}

void AccountCenter::InitNet()
{
	g_pNetCenter	= new ZNetCenter();
	g_pNetCenter->StartNetCenter();

	g_pHttpCenter	= new ZHttpCenter();
	g_pHttpCenter->StartHttpCenter();
}

void AccountCenter::InitDB()
{
	if(!m_pDBCreate->OnConnectToDB(ServerCfg::GetInstance().szDBName,ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd))
		AfxMessageBox("连接数据库失败");

	m_pDBCreate->CreateCharacterDB();

	if(!m_pLogDB->OnConnectToDB(ServerCfg::GetInstance().szLogDB, ServerCfg::GetInstance().szDBIP, ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd))
	{
		return;
	}
	if(!m_pLogDB->CreateLogDB())
	{
		return;
	}
}
ZPacket* PacketDup(ZPacket * pPacket)
{
	int memSize = pPacket->wSize + sizeof(ZPacket)-1;
	void * rp = malloc(memSize);
	if (rp != NULL)
	{
		memcpy(rp, pPacket, memSize);
		return (ZPacket*)rp;
	}
	return NULL;
}
void AccountCenter::RecvPacket( ZPacket* pPacket )
{
	EnterCriticalSection(&m_xPacketLock);
	m_pListPacket->Add(PacketDup(pPacket));
	LeaveCriticalSection(&m_xPacketLock);
	//if(_processAccountPacket(pPacket))
	//	return;

}

void AccountCenter::DealPacket()
{
#ifndef _DEBUG
	__try
#endif
	{
		while(1)
		{
			ZPacket* pPacket	= NULL;
			EnterCriticalSection(&m_xPacketLock);
			if(m_pListPacket->Count)
				pPacket	= (ZPacket*)m_pListPacket->Delete(0);
			LeaveCriticalSection(&m_xPacketLock);
			if(pPacket)
			{
				_processAccountPacket(pPacket);
				free(pPacket);
			}
			else
				break;
		}
	}
#ifndef _DEBUG
	__except(DUMP_CALLSTACK())
	{
		DUMP_STRING("AccountCenter DealPacket Error!\n");
	}
#endif
}

bool AccountCenter::_processAccountPacket( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup	!= ACCOUNT)
		return false;

	switch(pPacket->bCmd)
	{
	case LOGIN:
		{
			SET_DATA(pData,ACCOUNT,LOGIN,pPacket);
			int	nAccUin	= 0;
			int nRtl	= PlayerLogin(pData->szLogin,pData->szPassword,pData->bSpeedReg,nAccUin,pData->bTPID,pData->szMacAddress,0,"",pData->szIp);
			
			BuildPacketEx(pSendPacket, ACCOUNT, LOGIN_RLT,buf,256);
			SET_DATA(pSend, ACCOUNT, LOGIN_RLT, pSendPacket);
			pSendPacket->dwKey	= pPacket->dwKey;
			pSend->nAccUin	= nAccUin;
			pSend->byErrorCode	= nRtl;
			pSend->byJoin		= pData->bJoin;
			strcpy(pSend->szRtlPswd,"");
			g_pNetCenter->SendNGPacket(pSendPacket);

			//更新登陆时间
			if(pData->bJoin && (nRtl == LOGIN_SUCCESSFUL || nRtl == LOGIN_SUCCESSFUL_BYAD))
			{
				//发送角色信息包
				BuildPacketEx(pSIPacket, ACCOUNT, SIMPLEINFOS,bufSI, 1024);
				SET_DATA(pSIData, ACCOUNT, SIMPLEINFOS, pSIPacket);
				pSIPacket->dwKey	= pPacket->dwKey;
				_FillSimpleUInfos(nAccUin,pSIData);
				pSIPacket->wSize	+= sizeof(USimpleInfo)*pSIData->bCnt;
				//if(pSIData->bCnt)
				//	pSIData->bCnt += 100;

				g_pNetCenter->SendNGPacket(pSIPacket);

				if(m_bIsMainAcc == false)
				{
					if(strlen(pData->szAPNToken))
						m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_ACCOUNT_TABLE" set logintime=now(),apntoken='%s' where accname='%s'",pData->szAPNToken,pData->szLogin);
					else
						m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_ACCOUNT_TABLE" set logintime=now() where accname='%s'",pData->szLogin);

					int nCoutryChoose = GetChooseCountry();
					if (nCoutryChoose)
					{
						BuildPacketEx(pSendPacket,ACCOUNT,COUNTRY_CHOOSE,buf,256);
						SET_DATA(pSend, ACCOUNT,COUNTRY_CHOOSE,pSendPacket);
						pSendPacket->dwKey = pPacket->dwKey;
						pSend->byCountry	= nCoutryChoose;
						g_pNetCenter->SendNGPacket(pSendPacket);
					}
				}

			}
		}
		break;
	case CHANGEPASSWORD:
		{
			SET_DATA(pData,ACCOUNT,CHANGEPASSWORD,pPacket);


			BuildPacketEx(pSendPacket, ACCOUNT, CHANGEPASSWORD_RTL,buf,256);
			SET_DATA(pSend, ACCOUNT, CHANGEPASSWORD_RTL, pSendPacket);
			pSendPacket->dwKey = pPacket->dwKey;
			pSend->byRtl	= ChgPassword(pData->szLoginName,pData->szOldPassword,pData->szNewPassword);
			g_pNetCenter->SendNGPacket(pSendPacket);
		}
		break;
	case LOGINMAIN:
		{
			SET_DATA(pData,ACCOUNT,LOGINMAIN,pPacket);
			int	nAccUin	= 0;

			//第三方广告验证
			int nRtl	= PlayerLogin(pData->szLogin,pData->szPassword,pData->bSpeedReg,nAccUin,0,pData->szMacAddress,pData->nIOSVersion,pData->szIDFA,pData->szIp);

			BuildPacketEx(pSendPacket, ACCOUNT, LOGIN_RLT,buf,256);
			SET_DATA(pSend, ACCOUNT, LOGIN_RLT, pSendPacket);
			pSendPacket->dwKey	= pPacket->dwKey;
			pSend->nAccUin	= nAccUin;
			pSend->byErrorCode	= nRtl;
			pSend->byJoin		= pData->bJoin;
			strcpy(pSend->szRtlPswd,pData->szPassword);
			g_pNetCenter->SendNGPacket(pSendPacket);

			//更新登陆时间
			if(pData->bJoin && (nRtl == LOGIN_SUCCESSFUL || nRtl == LOGIN_SUCCESSFUL_BYAD))
			{
				//发送角色信息包
				BuildPacketEx(pSIPacket, ACCOUNT, SIMPLEINFOS,bufSI, 1024);
				SET_DATA(pSIData, ACCOUNT, SIMPLEINFOS, pSIPacket);
				pSIPacket->dwKey	= pPacket->dwKey;
				_FillSimpleUInfos(nAccUin,pSIData);
				pSIPacket->wSize	+= sizeof(USimpleInfo)*pSIData->bCnt;

				g_pNetCenter->SendNGPacket(pSIPacket);

			}
		}
		break;
	case ACTIVE_CNT:
		{
			SET_DATA(pData,ACCOUNT,ACTIVE_CNT,pPacket);
			m_pMarkDBCenter->AddNewAtivAcc(pData->nPVal);
		}
		break;
					
	case TP_LOGIN:
		{
			SET_DATA(pData,ACCOUNT,TP_LOGIN,pPacket);
			char	szAcc[64]	= {0};		//32
			char	szPswd[32]	= {0};		//16
			char	szBackKey[50]	= {0};
			int		nRtl	= LOGIN_ERROR_OUT_SERVER;
			int		nAccUin	= 0;
			if(g_pHttpCenter->m_pHttpCenter->TPLogin(pData->szUin,pData->szToken,pData->byTPID,szAcc,szPswd,szBackKey))
			{
				nRtl	= PlayerLogin(szAcc,szPswd,true,nAccUin,pData->byTPID,pData->szMacAddress);
			}
			BuildPacketEx(pSendPacket, ACCOUNT, TP_LOGINRTL,buf,256);
			SET_DATA(pSend, ACCOUNT, TP_LOGINRTL, pSendPacket);
			pSendPacket->dwKey	= pPacket->dwKey;
			pSend->nAccUin	= nAccUin;
			pSend->byErrorCode	= nRtl;
			if(nRtl == LOGIN_SUCCESSFUL || nRtl == LOGIN_SUCCESSFUL_BYAD)
			{
				strcpy(pSend->szAcc,szAcc);
				strcpy(pSend->szPswd,szPswd);
			}
			else
				g_pLogFile->OutPut("TPLogin Error(%d): TP=%d, Acc=%s",nRtl,pData->byTPID,szAcc);
			g_pNetCenter->SendNGPacket(pSendPacket);
		}
		break;
	case TP_LOGIN_SPTOKEN:
		{
			SET_DATA(pData,ACCOUNT,TP_LOGIN_SPTOKEN,pPacket);
			char	szAcc[64]	= {0};		//32
			char	szPswd[32]	= {0};		//16
			char	szBackKey[80]	= {0};
			int		nRtl	= LOGIN_ERROR_OUT_SERVER;
			int		nAccUin	= 0;
			if(g_pHttpCenter->m_pHttpCenter->TPLogin(pData->szUin,pData->szToken,pData->byTPID,szAcc,szPswd,szBackKey))
			{
				nRtl	= PlayerLogin(szAcc,szPswd,true,nAccUin,pData->byTPID,pData->szMacAddress,pData->nIOSVersion,pData->szIDFA,pData->szIp);
			}
			BuildPacketEx(pSendPacket, ACCOUNT, TP_LOGINRTL,buf,256);
			SET_DATA(pSend, ACCOUNT, TP_LOGINRTL, pSendPacket);
			pSendPacket->dwKey	= pPacket->dwKey;
			pSend->nAccUin	= nAccUin;
			pSend->byErrorCode	= nRtl;
			if(nRtl == LOGIN_SUCCESSFUL || nRtl == LOGIN_SUCCESSFUL_BYAD)
			{
				strcpy(pSend->szAcc,szAcc);
				strcpy(pSend->szPswd,szPswd);
				strcpy(pSend->szBackKey,szBackKey);
			}
			else
				g_pLogFile->OutPut("TPLogin Error(%d): TP=%d, Acc=%s",nRtl,pData->byTPID,szAcc);
			g_pNetCenter->SendNGPacket(pSendPacket);
		}
		break;
	case TP_LOGINMAIN:
		{
			SET_DATA(pData,ACCOUNT,TP_LOGINMAIN,pPacket);
			char	szAcc[64]	= {0};		//32
			char	szPswd[32]	= {0};		//16
			char	szBackKey[80]	= {0};
			int		nRtl	= LOGIN_ERROR_OUT_SERVER;
			int		nAccUin	= 0;
			if(g_pHttpCenter->m_pHttpCenter->TPLogin(pData->szUin,pData->szToken,pData->byTPID,szAcc,szPswd,szBackKey))
			{
				nRtl	= PlayerLogin(szAcc,szPswd,true,nAccUin,pData->byTPID,pData->szMacAddress,pData->nIOSVersion,pData->szIDFA,pData->szIp);
			}

			BuildPacketEx(pSendPacket, ACCOUNT, TP_LOGINRTL,buf,256);
			SET_DATA(pSend, ACCOUNT, TP_LOGINRTL, pSendPacket);
			pSendPacket->dwKey	= pPacket->dwKey;
			pSend->nAccUin	= nAccUin;
			pSend->byErrorCode	= nRtl;
			if(nRtl == LOGIN_SUCCESSFUL || nRtl == LOGIN_SUCCESSFUL_BYAD)
			{
				strcpy(pSend->szAcc,szAcc);
				strcpy(pSend->szPswd,szPswd);
				strcpy(pSend->szBackKey,szBackKey);
			}
			else
				g_pLogFile->OutPut("TPLogin Error(%d): TP=%d, Acc=%s",nRtl,pData->byTPID,szAcc);
			g_pNetCenter->SendNGPacket(pSendPacket);
		}
		break;
	case GA_SEND:
		{
			SET_DATA(pData,ACCOUNT,GA_SEND,pPacket);


			QueryRlt xRlt;
			if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_INVITE_TABLE" where invitecode = '%s'", pData->szCode))
				return true;

			MySqlQuery* pCmd = xRlt.GetRlt();
			if(pCmd == NULL || !pCmd->FetchRow()) return true;

			int dwUin	= pCmd->GetInt("uinsvr");

			m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_INVITE_TABLE" set reachnum=reachnum+1 where invitecode='%s'",pData->szCode);


			BuildPacketEx(pSendPacket,ACCOUNT,GA_RECV,buf,256);
			SET_DATA(pSend, ACCOUNT,GA_RECV,pSendPacket);
			pSendPacket->dwKey	= 0;
			pSend->nUin	= dwUin;
			pSend->bAdd		= 1;
			g_pNetCenter->SendNGPacket(pSendPacket);

			//if(m_bIsMainAcc)
			//{//主账号服务器不收该包,错误
			//	g_pLogFile->OutPut("GA_SEND ERROR: %d---%s",pData->bEvent,pData->szCode);
			//	//if(pData->bEvent == 0)
			//	//{
			//	//	m_pDBCreate->GetDBServer()->ExecuteSQL("insert into "RPG_INVITE_TABLE"  (uinsvr,invitecode,reachnum,checknum) values (%d,'%s',%d,%d)",
			//	//		);
			//	//}
			//}
			//else
			//{
			//	char	szTmp[512]={0};
			//	sprintf(szTmp,"InviteCmd?svr=%d&invcode=%s&event=%d",ServerCfg::GetInstance().nServerID,pData->szCode,pData->bEvent);
			//	g_pHttpCenter->m_pHttpCenter->SendToMainAcc(szTmp);
			//}
		}
		break;
	case GA_GMDATA:
		{
			SET_DATA(pData,ACCOUNT,GA_GMDATA,pPacket);
			switch(pData->nType){
			case 0://创建角色
				{
					m_pMarkDBCenter->AddNewCreateAcc(pData->nPVal);
				}
				break;
			case 1://有效用户
				{
					m_pMarkDBCenter->AddNewEffAcc(pData->nPVal);
				}
				break;
			case 2://新增付费用户数
				{
					m_pMarkDBCenter->AddPayUserAcc(pData->nPVal);
				}
				break;
			case 3://新增付费额
				{
					m_pMarkDBCenter->AddPayTotalAcc(pData->nPVal,pData->nExVal);
				}
				break;
 			case 4://付费用户
 				{
 					m_pMarkDBCenter->AddDayPayUserAcc(pData->nPVal);
 				}
 				break;
			case 5://付费次数
				{
					m_pMarkDBCenter->AddDayPayUserTime(pData->nPVal);
				}
				break;
			case 6://付费额
				{
					m_pMarkDBCenter->AddDayPayTotalAcc(pData->nPVal,pData->nExVal);
				}
				break;
			}
			
		}
		break;
	case TP_IAPPAY:
		{
			SET_DATA(pData,ACCOUNT,TP_IAPPAY,pPacket);
			BYTE	bRtl	= 0;
			int		nLoopTime	= 0;
			do 
			{
				bRtl	= g_pHttpCenter->m_pHttpCenter->TPCheckIAPPay(pData->szAcc,pData->szToken);
				nLoopTime++;
			} while (bRtl==0 && nLoopTime>3);
			if(bRtl)
			{
				if(bRtl == 1)
					g_pLogFile->OutPut("HTTP IAPPay Success: %s(%d)",pData->szAcc,strlen(pData->szToken));
				else
					g_pLogFile->OutPut("HTTP IAPPay ERROR: %s(%s)",pData->szAcc,pData->szToken);
			}
			else
			{
				g_pLogFile->OutPut("HTTP IAPPay Failed: %s(%s)",pData->szAcc,pData->szToken);
				EnterCriticalSection(&m_xLockIAP);
				_errIAPToken*	pIAP	= new _errIAPToken();
				pIAP->nTime		= 1;
				pIAP->dwTickCheck	= GetTickCount()+15000;
				pIAP->szAcc		= strdup(pData->szAcc);
				pIAP->szToken	= strdup(pData->szToken);
				m_pLErrorIAP->Add(pIAP);
				LeaveCriticalSection(&m_xLockIAP);


			}
		}
		break;
	}
	return true;
}

static bool IsValidDigit(const char* pstring)
{
	assert(pstring);

	//for login name and password
	int	i(strlen(pstring));
	if(i < 1 || i > 50)
		return  FALSE;
	while(i--)
	{
		char	a	= pstring[i];

		if(!((a >= '0' && a <= '9') || (a >= 'a' && a <= 'z') || (a>='A' &&a <='Z') || a == '-' || a == '_' || a=='@' || a=='.'))
			return false;
	}

	return true;
}

static bool IsValidPassword(const char* pstring)
{
	assert(pstring);

	//for login name and password
	int	i(strlen(pstring));
	if(i < 1 || i > 50)
		return  FALSE;

	while(i--)
	{
		char	a	= pstring[i];

		if(!((a >= '0' && a <= '9') || (a >= 'A' && a <= 'Z') || (a >= 'a' && a <= 'z') || a == '-' || a == '_'))
			return false;
	}

	return true;
}


int AccountCenter::ChgPassword( char* szAcc, char* szPswdOld, char* szPswdNew )
{
	if(strlen(szAcc) < 6)
		return 3;

	if(strlen(szPswdNew) < 6)
		return 3;

	//第三方平台的账号用 $ 符号开头,需要过滤这个判断
	if(!IsValidDigit(szAcc))
		return 3;

	if(!IsValidPassword(szPswdNew))
		return 3;

	DBIO* pDB	= m_pDBCreate->RandGetDB();
	//	读取数据
	bool	bExist	= true;
	bool	bIsIoaAD	= false;
	QueryRlt xRlt;
	MySqlQuery* pCmd = NULL;
	if(!pDB->Query(&xRlt,
		"SELECT accname,accpswd,accstate,AccountUin,mainaccpswd FROM "RPG_ACCOUNT_TABLE" where accname = '%s'", szAcc))
		return 1;
	pCmd = xRlt.GetRlt();               
	//	帐号不存在
	if(pCmd == NULL || !pCmd->FetchRow())
		return 1;

	char*	szDBAcc		= pCmd->GetStr("accname");
	char*	szDBPswd	= pCmd->GetStr("accpswd");
	char*	szDBMainPswd	= pCmd->GetStr("mainaccpswd");

	bIsIoaAD	= pCmd->GetInt("fromad");

	if(strlen(szDBMainPswd) >= 6)
	{//主服务器有重新设置过密码的用新密码
		if(strcmp(szDBMainPswd,szPswdOld) != 0)
			return 2;
	}
	else if(strcmp(szDBPswd,szPswdOld) != 0)
		return 2;

	pDB->ExecuteSQL("update "RPG_ACCOUNT_TABLE" set mainaccpswd='%s' where accname='%s'",szPswdNew,szAcc);

	return 0;
}

int AccountCenter::PlayerLogin( char* szAcc, char* szPswd, BYTE bReg,int & accUin,BYTE byTP,char* szMac,int nSysVer,char*	szIDFA,char* szIP )
{
	if(strlen(szAcc) < 6)
		return LOGIN_ERROR_ACOUNT_ERROR;

	if(strlen(szPswd) < 6)
		return LOGIN_ERROR_PSWD_ERROR;

	//第三方平台的账号用 $ 符号开头,需要过滤这个判断
	if(!IsValidDigit(szAcc))
		return LOGIN_ERROR_ACOUNT_ERROR;

	if(!IsValidPassword(szPswd))
		return LOGIN_ERROR_PSWD_ERROR;
	if(strlen(szIP) > 18)
		szIP	= "";

	//if(strlen(szInvite) && m_bIsMainAcc)
	//{
	//	QueryRlt xRlt1;
	//	if(!m_pDBCreate->GetDBServer()->Query(&xRlt1,
	//		"SELECT * FROM "RPG_INVITE_TABLE" where invitecode = '%s'", szInvite))
	//	{
	//		return LOGIN_ERROR_INVITE_ERROR;
	//	}
	//	MySqlQuery* pCmd1 = xRlt1.GetRlt();               

	//	//	帐号不存在
	//	if(pCmd1 == NULL || !pCmd1->FetchRow())
	//	{
	//		return LOGIN_ERROR_INVITE_ERROR;
	//	}
	//}
	DBIO* pDB	= m_pDBCreate->RandGetDB();
	//	读取数据
	bool	bExist	= true;
	bool	bIsIoaAD	= false;
	QueryRlt xRlt;
	MySqlQuery* pCmd = NULL;
	if(!pDB->Query(&xRlt,
		"SELECT accname,accpswd,accstate,AccountUin,mainaccpswd FROM "RPG_ACCOUNT_TABLE" where accname = '%s'", szAcc))
	{
		bExist	= false;
	}
	else
	{
		pCmd = xRlt.GetRlt();               

		//	帐号不存在
		if(pCmd == NULL || !pCmd->FetchRow())
		{
			bExist	= false;
		}else{
			int nState = pCmd->GetInt("accstate");
			if (nState == 1)
			{
				return LOGIN_ERROR_UNENABLE;
			}
		}
	}
	if(bExist==false)
	{
		if(bReg == false)
		{
			return LOGIN_ERROR_ACOUNT_ERROR;
		}
		else
		{//插入账号
			if(m_bIsLockNewAcc && m_bIsMainAcc == false)
			{
				return LOGIN_ERROR_SERVER_USER_FULL;
			}

			if(m_bIsMainAcc)
			{
				bIsIoaAD	= CheckIosAD(szMac,szIDFA,pDB);
			}

			if(!pDB->ExecuteSQL("INSERT INTO "RPG_ACCOUNT_TABLE" (accname,accpswd,createtime,logintime,rmbmoney,gmlv,beinvitecode,tpid,macaddress,sysver,idfa,fromad,loginip,mainaccpswd) \
																					  VALUES('%s','%s',now(),now(),0,0,'',%d,'%s',%d,'%s',%d,'%s','')", szAcc,szPswd,byTP,szMac,nSysVer,szIDFA,bIsIoaAD,szIP))
																					  return LOGIN_ERROR_ACOUNT_ERROR;

			if(!pDB->Query(&xRlt,
				"SELECT accname,accpswd,accstate,AccountUin,mainaccpswd FROM "RPG_ACCOUNT_TABLE" where accname = '%s'", szAcc))
				return LOGIN_ERROR_ACOUNT_ERROR;
			pCmd = xRlt.GetRlt();
			if(pCmd == NULL || !pCmd->FetchRow())
				return LOGIN_ERROR_ACOUNT_ERROR;

			m_pMarkDBCenter->AddNewRegAcc(byTP);
			m_nCreateCnt++;
		}
	}
	else
	{
		if(bReg == 2)
			return LOGIN_ERROR_ACOUNT_ISBEREG;
	
		pDB->ExecuteSQL("update "RPG_ACCOUNT_TABLE" set loginip='%s' where accname = '%s'",szIP,szAcc);
	}
	
	
	char*	szDBAcc		= pCmd->GetStr("accname");
	char*	szDBPswd	= pCmd->GetStr("accpswd");
	char*	szDBMainPswd	= pCmd->GetStr("mainaccpswd");
	
	bIsIoaAD	= pCmd->GetInt("fromad");

	if(strlen(szDBMainPswd) >= 6 && m_bIsMainAcc)
	{//主服务器有重新设置过密码的用新密码
		if(strcmp(szDBMainPswd,szPswd) != 0)
			return LOGIN_ERROR_PSWD_ERROR;

		strcpy(szPswd,szDBPswd);	//进入下级服务器统一用原密码
	}
	else if(strcmp(szDBPswd,szPswd) != 0)
		return LOGIN_ERROR_PSWD_ERROR;

	//char*	szTmp	= pCmd->GetStr("beinvitecode");
	//if(strlen(szTmp)<3 && strlen(szInvite))
	//{//补填的邀请码
	//	m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_ACCOUNT_TABLE" set beinvitecode='%s' where accname = '%s'",szInvite,szDBAcc);
	//}
	
	m_nLoginCnt++;

	accUin	= pCmd->GetInt("AccountUin");
	CMTime time = CMTime(pCmd->GetStr("logintime"));
	tm tmNow = CMTime::GetCurTime();
	if(bExist == false || tmNow.tm_yday != time[time_yeardays])
	{
		m_pMarkDBCenter->AddLoginAcc(byTP,true);
	}
	else
		m_pMarkDBCenter->AddLoginAcc(byTP,false);

	if(bIsIoaAD && m_bIsMainAcc)
		return LOGIN_SUCCESSFUL_BYAD;
	else
		return LOGIN_SUCCESSFUL;
}

void AccountCenter::_FillSimpleUInfos( int nAccUin, ACCOUNT_SIMPLEINFOS_Data* pSIData )
{
	pSIData->bCnt	= 0;
	QueryRlt xRlt;
	if(!m_pDBCreate->GetDBServer()->Query(&xRlt,
		"SELECT * FROM "RPG_USER_TABLE" where AccountUin = %d", nAccUin))
	{
		return;
	}

	MySqlQuery* pCmd = xRlt.GetRlt();          
	if(pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		pSIData->szUSInfos[pSIData->bCnt].dwUin		= pCmd->GetInt("Uin");
		pSIData->szUSInfos[pSIData->bCnt].byLv		= pCmd->GetInt("lvl");
		pSIData->szUSInfos[pSIData->bCnt].byJob		= pCmd->GetInt("job");
		pSIData->szUSInfos[pSIData->bCnt].byCountry		= pCmd->GetInt("country");
		pSIData->szUSInfos[pSIData->bCnt].bySex		= pCmd->GetInt("sex");
		strcpy(pSIData->szUSInfos[pSIData->bCnt].szNick,pCmd->GetStr("Nickname"));
		pSIData->bCnt++;
	}
}

bool AccountCenter::HttpPayRequest( char* szAcc, char* szOrder,int nMoney,int nTPID )
{
	//查询该order订单是否已处理过
	if(m_pLogDB->CheckOrderNo(szOrder))
	{//已处理过的订单
		return false;
	}
	QueryRlt xRlt;
	if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_ACCOUNT_TABLE" where accname = '%s'", szAcc))
		return false;
	
	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd == NULL || !pCmd->FetchRow()) return false;

	m_pMarkDBCenter->AddRmbPay(nTPID,nMoney);
	
	int accUin	= pCmd->GetInt("AccountUin");
	
	//发包给游戏服务器加钱
	BuildPacketEx(pPacket, ACCOUNT,TPCOMM_PAY,buf,1024);
	SET_DATA(pSend, ACCOUNT,TPCOMM_PAY,pPacket);
	pPacket->dwKey	= 0;
	pSend->nAccUin	= accUin;
	pSend->nRealVal	= GetRmbByPayMoney(nMoney);
	pSend->nRmbVal	= nMoney;
	pSend->byType	= nTPID;
	strcpy(pSend->szOrder,szOrder);
	pPacket->wSize+=strlen(szOrder);
	g_pNetCenter->SendNGPacket(pPacket);

	m_pLogDB->SaveTPPayLog(accUin,szAcc,nTPID,szOrder,pSend->nRealVal,nMoney);
	return true;
}

int AccountCenter::GetRmbByPayMoney( int nPay )
{
	switch(nPay)
	{
	case 6:
		return 60;
	case 30:
		return 300;
	case 78:
		return 780;
	case 198:
		return 2178;
	case 328:
		return 3772;
	case 648:
		return 7776;
	default:
		return nPay*10;
	}
	return nPay*10;
}

void AccountCenter::RecvInviteCmd( int nFrmSvr,char* szInvCode,BYTE bEvent )
{
	if(m_bIsMainAcc)
	{
		if(bEvent == 0)
		{
			QueryRlt xRlt;
			if(m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_INVITE_TABLE" where invitecode = '%s'", szInvCode))
			{//邀请码是否重复,理论上不可能
				MySqlQuery* pCmd = xRlt.GetRlt();
				if(pCmd && pCmd->FetchRow()) 
				{
					g_pLogFile->OutPut("Invitecode EXIST: %d---%s",nFrmSvr,szInvCode);
					return;
				}
			}

			m_pDBCreate->GetDBServer()->ExecuteSQL("insert into "RPG_INVITE_TABLE" (uinsvr,invitecode,reachnum,checknum) values (%d,'%s',%d,%d)",
				nFrmSvr,szInvCode,0,0);
		}
		else if(bEvent == 1)
		{
			QueryRlt xRlt;
			if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_INVITE_TABLE" where invitecode = '%s'", szInvCode))
				return;

			MySqlQuery* pCmd = xRlt.GetRlt();
			if(pCmd == NULL || !pCmd->FetchRow()) return;

			int nSvr	= pCmd->GetInt("uinsvr");
			char	szTmp[512]={0};
			sprintf(szTmp,"InviteCmd?svr=%d&invcode=%s&event=%d",nFrmSvr,szInvCode,2);
			g_pHttpCenter->m_pHttpCenter->SendToChildAcc(szTmp,nSvr);
			m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_INVITE_TABLE" set reachnum=reachnum+1 where invitecode='%s'",szInvCode);
		}
	}
	else
	{//收到其他服玩家邀请码生效的请求
		if(bEvent == 2)
		{
			QueryRlt xRlt;
			if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_INVITE_TABLE" where invitecode = '%s'", szInvCode))
				return;

			MySqlQuery* pCmd = xRlt.GetRlt();
			if(pCmd == NULL || !pCmd->FetchRow()) return;

			int dwUin	= pCmd->GetInt("uinsvr");

			m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_INVITE_TABLE" set reachnum=reachnum+1 where invitecode='%s'",szInvCode);


			BuildPacketEx(pSendPacket,ACCOUNT,GA_RECV,buf,256);
			SET_DATA(pSend, ACCOUNT,GA_RECV,pSendPacket);
			pSendPacket->dwKey	= 0;
			pSend->nUin	= dwUin;
			pSend->bAdd		= 1;
			g_pNetCenter->SendNGPacket(pSendPacket);
		}
	}
}

void AccountCenter::Update()
{
	m_pMarkDBCenter->CheckDayReset();
	//DealPacket();
	for (int i = 0; i < 20; i++)
	{
		ppThread[i]->SendMessage(WM_DEAL_ACCPACKET,0,0,true);
	}
	ppThread[0]->SendMessage(WM_RECHECK_APPPAY,0,0,true);
}

void AccountCenter::Update25()
{
	if(m_pDBCreate->m_pDB->IsConnected() == false)
		m_pDBCreate->OnConnectToDB(ServerCfg::GetInstance().szDBName,ServerCfg::GetInstance().szDBIP,ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd);

	if(m_pLogDB->m_pDB->IsConnected() == false)
		m_pLogDB->OnConnectToDB(ServerCfg::GetInstance().szLogDB, ServerCfg::GetInstance().szDBIP, ServerCfg::GetInstance().szDBUser,ServerCfg::GetInstance().szDBPswd);


	//static bool bbbbb	= true;
	//if(bbbbb)
	//	g_pMainAccount->TTTTTTTTTTT();

	//bbbbb	= false;
}

BYTE AccountCenter::GetChooseCountry()
{
	return 0;
	QueryRlt xRlt;
	if(!m_pDBCreate->GetDBServer()->Query(&xRlt,
		"select country, count(country) as cnt FROM "RPG_USER_TABLE" group by country"))
	{
		return 0;
	}
	MySqlQuery* pCmd = xRlt.GetRlt();
	std::map<int,int> mapCountry;
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{
			mapCountry[pCmd->GetInt("country")] = pCmd->GetInt("cnt");
		}
	}
	if (mapCountry[1] < mapCountry[2])
	{
		return 1;
	}else{
		return 2;
	}
}

bool AccountCenter::_checkADMacIDFA( char* szMac, char* szIDFA )
{
	QueryRlt xRlt;
	if(strlen(szMac) < 10 || strcmp(szMac,"02:00:00:00:00:00")==0 ||strcmp(szMac,"00:00:00:00:00:00")==0)
	{
		if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" where idfa='%s'",szIDFA))
			return false;
	}
	else if(strlen(szIDFA) < 10)
	{
		if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" where macad='%s'",szMac))
			return false;
	}
	else
	{
		if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" where macad='%s' or idfa='%s'",szMac,szIDFA))
			return false;
	}
	
	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd == NULL || !pCmd->FetchRow()) return false;
	return true;
}

void AccountCenter::RecvIosAD( char* szMac, char* szIDFA, char* szTP, char* szEx )
{
	if(_checkADMacIDFA(szMac,szIDFA))
		return;

	m_pDBCreate->GetDBServer()->ExecuteSQL("insert into "RPG_IOSAD_TABLE" (macad,idfa,exmsg,state,tpname,linktime) values ('%s','%s','%s',%d,'%s',now())",
		szMac,szIDFA,szEx,0,szTP);
}

extern int requestTPlat(bool bPost,char* hostname,int nPort, char* api, char* parameters, OUT char* szOut,BYTE bTP);
bool AccountCenter::CheckIosAD( char* szMac, char* szIDFA,DBIO* pDB )
{
	QueryRlt xRlt;
	if(strlen(szMac) < 10 || strcmp(szMac,"02:00:00:00:00:00")==0 ||strcmp(szMac,"00:00:00:00:00:00")==0)
	{
		if(strlen(szIDFA) < 10)
			return false;
		if(!pDB->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" where idfa='%s'",szIDFA))
			return false;
	}
	else if(strlen(szIDFA) < 10)
	{
		if(!pDB->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" where macad='%s'",szMac))
			return false;
	}
	else
	{
		if(!pDB->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" where (macad='%s' or idfa='%s')",szMac,szIDFA))
			return false;
	}
	

	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd == NULL) return false;
	char	szTPName[20];
	char	szExt[255];
	char	szDBMac[32];
	char	szDBIDFA[50];
	int		nState	= 0;
	if(pCmd->FetchRow())
	{
		nState	= pCmd->GetInt("state");
		if(nState)
			return true;
		strcpy(szTPName,pCmd->GetStr("tpname"));
		strcpy(szExt,pCmd->GetStr("exmsg"));
		strcpy(szDBMac,pCmd->GetStr("macad"));
		strcpy(szDBIDFA,pCmd->GetStr("idfa"));

		CMTime time = CMTime(pCmd->GetStr("linktime"));
		DWORD	dwTNow	= CMTime::GetCurTickCount();
		if(dwTNow - time.GetTickCount() < 120)
		{
			pDB->ExecuteSQL("update "RPG_IOSAD_TABLE" set state=2 where idfa='%s' and macad='%s'",szDBIDFA,szDBMac);
			return true;
		}

		if(strlen(szDBMac) < 10 && strlen(szDBIDFA) < 30)
			return false;
	}
	else
		return false;

	char	szUrl[1024]={0};
	char	szRecv[1024]={0};
	if(strstr(szTPName,"dianru"))
	{
		if(strlen(szDBMac) > 10)
			sprintf(szUrl,"/adwall/ios/returndata.ashx?drkey=%s%s&isvalidactive=true",szExt,szDBMac);
		else
			sprintf(szUrl,"/adwall/ios/returndata.ashx?drkey=%s%s&isvalidactive=true",szExt,szDBIDFA);
		requestTPlat(false,"api.wall.dianru.com",80,szUrl,"",szRecv,0);
	}
	else if(strstr(szTPName,"youmi"))
	{
		char*	szTmpA	= strstr(szExt,"://");
		if(szTmpA == NULL || strlen(szTmpA) < 5)
			return false;
		char*	szTmpB	= strstr(szTmpA+3,"/");
		if(szTmpB == NULL)
			return false;

		char	szTmpC[255];
		memset(szTmpC,0,255);
		memcpy(szTmpC,szTmpA+3,szTmpB-szTmpA-3);

		requestTPlat(false,szTmpC,80,szTmpB,"",szRecv,0);
	}
	else if(strstr(szTPName,"adsage"))
	{
		char*	szTmpA	= strstr(szExt,"://");
		if(szTmpA == NULL || strlen(szTmpA) < 5)
			return false;
		char*	szTmpB	= strstr(szTmpA+3,"/");
		if(szTmpB == NULL)
			return false;

		char	szTmpC[255];
		memset(szTmpC,0,255);
		memcpy(szTmpC,szTmpA+3,szTmpB-szTmpA-3);

		requestTPlat(false,szTmpC,80,szTmpB,"",szRecv,0);
	}
	else if(strstr(szTPName,"duomeng"))
	{
		Cmd5Capi	md5Capi;
		char	szSign[1024]	= {0};
		//等待密钥
		sprintf(szSign,"843125355,%s,,%s,,e818fbf8d0e7f80e4cde25716199028d",szDBMac,szDBIDFA);
		char* szMd5	= md5Capi.Digest(szSign);
		
		sprintf(szUrl,"/track/ow/api/postback?appId=843125355&udid=%s&ifa=%s&acttime=%d&returnFormat=1&sign=%s",szDBMac,szDBIDFA,CMTime::GetCurTickCount(),szMd5);
		requestTPlat(false,"e.domob.cn",80,szUrl,"",szRecv,0);
	}
	else if(strstr(szTPName,"wanpu"))
	{
		sprintf(szUrl,"/ios/receiver/activate?app=843125355&udid=%s&idfa=%s",szDBMac,szDBIDFA);
		requestTPlat(false,"ios.wapx.cn",80,szUrl,"",szRecv,0);
	}
	else if(strstr(szTPName,"limei"))
	{
		char*	szTmpA	= strstr(szExt,"://");
		if(szTmpA == NULL || strlen(szTmpA) < 5)
			return false;
		char*	szTmpB	= strstr(szTmpA+3,"/");
		if(szTmpB == NULL)
			return false;

		char	szTmpC[255];
		memset(szTmpC,0,255);
		memcpy(szTmpC,szTmpA+3,szTmpB-szTmpA-3);

		requestTPlat(false,szTmpC,80,szTmpB,"",szRecv,0);
	}

	if(strlen(szDBMac) < 10 && strlen(szDBIDFA) < 30)
		return false;
	pDB->ExecuteSQL("update "RPG_IOSAD_TABLE" set state=1 where idfa='%s' and macad='%s'",szDBIDFA,szDBMac);

	return true;
}

void AccountCenter::TTTTTTTTTTT()
{
	QueryRlt xRlt;
	if(!m_pDBCreate->GetDBServer()->Query(&xRlt,"SELECT * FROM "RPG_IOSAD_TABLE" "))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if(pCmd == NULL) return;

	int	nIndex	= 0;
	char	szDBMac[32];
	char	szDBIDFA[50];
	while(pCmd->FetchRow())
	{
		nIndex	++;

		strcpy(szDBMac,pCmd->GetStr("macad"));
		strcpy(szDBIDFA,pCmd->GetStr("idfa"));
		CMTime time = CMTime(pCmd->GetStr("linktime"));
		DWORD	dwTickLink	= time.GetTickCount();
		QueryRlt xRlt1;
		if(!m_pDBCreate->GetDBServer()->Query(&xRlt1,"SELECT createtime FROM "RPG_ACCOUNT_TABLE" where idfa='%s' or macaddress='%s'",szDBIDFA,szDBMac))
			return;
		MySqlQuery* pCmd1 = xRlt1.GetRlt();
		if(pCmd1 == NULL) return;
		if(pCmd1->FetchRow())
		{
			CMTime time1 = CMTime(pCmd1->GetStr("createtime"));
			DWORD	dwTickReg	= time1.GetTickCount();

			if(dwTickReg - dwTickLink < 1200)
			{
				int nVal	= dwTickReg-dwTickLink;
				if(nVal < 2)
					nVal = 2;
				m_pDBCreate->GetDBServer()->ExecuteSQL("update "RPG_IOSAD_TABLE" set state=%d,createtime='%s' where idfa='%s' and macad='%s'",nVal,pCmd1->GetStr("createtime"),szDBIDFA,szDBMac);
			}
		}
	}
}

void AccountCenter::RecheckAppPay()
{
	EnterCriticalSection(&m_xLockIAP);
	for (int n = m_pLErrorIAP->Count-1; n >= 0; n--)
	{
		_errIAPToken*	pIAP	= (_errIAPToken*)m_pLErrorIAP->Items[n];

		if(GetTickCount() < pIAP->dwTickCheck)
			continue;

		pIAP->nTime++;
		pIAP->dwTickCheck	= GetTickCount() + pIAP->nTime*15000;
		
		BYTE bRCheck	= 0;
		bRCheck	= g_pHttpCenter->m_pHttpCenter->TPCheckIAPPay(pIAP->szAcc,pIAP->szToken);
		if(bRCheck)
		{
			g_pLogFile->OutPut("HTTP IAPPay CHECKPAY Success: %s(%d)",pIAP->szAcc,bRCheck);
		}
		if(pIAP->nTime > 6 || bRCheck)
		{
			if(bRCheck==0){
				g_pLogFile->OutPut("HTTP IAPPay CHECKPAY Failed: %s(%s)",pIAP->szAcc,pIAP->szToken);
				BuildPacketEx(pSendPacket,ACCOUNT,IAP_CHECK_FAIL,buf,256);
				SET_DATA(pSend, ACCOUNT,IAP_CHECK_FAIL,pSendPacket);
				pSendPacket->dwKey	= 0;
				strncpy(pSend->szAccount ,pIAP->szAcc,20);
				g_pNetCenter->SendNGPacket(pSendPacket);
			}
			m_pLErrorIAP->Delete(n);
			free(pIAP->szAcc);
			free(pIAP->szToken);
		}
	}
	LeaveCriticalSection(&m_xLockIAP);
}

