#include "stdafx.h"
#include "MarkGameDB.h"
#include "TimeControl.h"
#include "DBTables.h"
#include "DBLogControl.h"
#include "FsGameCenter.h"

MarkGameDB* g_pMarkGameDB	= NULL;

MarkGameDB::MarkGameDB()
{
	m_nDayMark	= 0;
	m_dwTick5Minute	= GetTickCount();
	m_bCheckNowOnline	= false;

	m_nOnlineNew	= 0;
	m_nOnlineOld	= 0;
}

MarkGameDB::~MarkGameDB()
{

}

void MarkGameDB::CheckTimeEvent()
{
	tm tmNow = CMTime::GetCurTime();

	//每5分钟插入一项, 检测当前项是否存在
	if(m_bCheckNowOnline == false)
	{
		bool bFind = false;
		QueryRlt xRlt;
		if(g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM "MARK_ONLINECNT_TABLE" where timemark = 'now'"))
		{
			MySqlQuery* pCmd = xRlt.GetRlt();
			if(pCmd != NULL && pCmd->FetchRow())
			{
				bFind = true;
			}
		}
		if(!bFind)
			g_pDBase->ExecuteSQL(false,"Insert Into "MARK_ONLINECNT_TABLE" (timemark,newaccount,oldaccount) values ('now',0,0)");

		m_bCheckNowOnline	= true;
	}

	if(GetTickCount() - m_dwTick5Minute > 300000)
	{//5分钟记录一次
		tm time	= CMTime::GetCurTime();
		char	szTmp[32];
		sprintf(szTmp,"%d%02d%02d-%02d:%02d",time.tm_year,time.tm_mon+1,time.tm_mday,time.tm_hour,time.tm_min);

		g_pDBase->ExecuteSQL(false,"Insert Into "MARK_ONLINECNT_TABLE" (timemark,newaccount,oldaccount) values ('%s',%d,%d)",
			szTmp,m_nOnlineNew,g_pGameCenter->GetOnlineCnt());
		m_dwTick5Minute	= GetTickCount();
	}

	bool	bResetDay	= false;
	if(m_nDayMark == 0)
	{
		bool	bFind	= false;
		QueryRlt xRlt;
		MySqlQuery* pCmd = NULL;
		if(g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM "MARK_RMBCOST_TABLE" where objid = 0"))
		{
			pCmd = xRlt.GetRlt();
			if(pCmd != NULL && pCmd->FetchRow())
			{
				bFind	= true;
			}
		}
		if(!bFind)
		{
			g_pDBase->ExecuteSQL(false,"Insert Into "MARK_RMBCOST_TABLE" (objid,totalcnt,weekcnt,daycnt,checktime) values (%d,%d,%d, %d, now())",
				0,0, 0,0);
			g_pDBase->ExecuteSQL(false,"Insert Into "MARK_RMBCOST_TABLE" (objid,totalcnt,weekcnt,daycnt,checktime) values (%d,%d,%d, %d, now())",
				1,0, 0,0);
			g_pDBase->ExecuteSQL(false,"Insert Into "MARK_RMBCOST_TABLE" (objid,totalcnt,weekcnt,daycnt,checktime) values (%d,%d,%d, %d, now())",
				2,0, 0,0);
			m_nDayMark	= CMTime::GetCurTime().tm_yday;
		}
		else
		{
			CMTime time = CMTime(pCmd->GetStr("checktime"));
			m_nDayMark	= CMTime::GetCurTime().tm_yday;
			if(time[time_yeardays] != m_nDayMark)
			{
				bResetDay	= true;	
			}
		}
	}
	if(bResetDay)
	{
		if(CMTime::GetCurTime().tm_wday == 0)
		{
			g_pDBase->ExecuteSQL(false,"update "MARK_RMBCOST_TABLE" set daycnt=0,weekcnt=0,checktime=now()");
		}
		else
		{
			g_pDBase->ExecuteSQL(false,"update "MARK_RMBCOST_TABLE" set daycnt=0,checktime=now()");
		}
	}
}


void MarkGameDB::AddMarkCost( int nID,int nVal)
{
	std::map<int,BYTE>::iterator itor	= mapMarkDB.find(nID);
	if(itor == mapMarkDB.end() || itor->second == 0)
	{
		mapMarkDB[nID]	= 1;
		bool	bFind	= false;
		QueryRlt xRlt;
		MySqlQuery* pCmd = NULL;
		if(g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM "MARK_RMBCOST_TABLE" where objid = %d",nID))
		{
			pCmd = xRlt.GetRlt();
			if(pCmd != NULL && pCmd->FetchRow())
			{
				bFind	= true;
			}
		}
		if(!bFind)
		{
			g_pDBase->ExecuteSQL(false,"Insert Into "MARK_RMBCOST_TABLE" (objid,totalcnt,weekcnt,daycnt,checktime) values (%d,%d,%d, %d, now())",
				nID,nVal, nVal,nVal);
			return;
		}
	}

	g_pDBase->ExecuteSQL(false,"update "MARK_RMBCOST_TABLE" set daycnt=daycnt+%d,weekcnt=weekcnt+%d,totalcnt=totalcnt+%d where objid=%d",
		nVal,nVal,nVal,nID);
}

void MarkGameDB::AddQICnt( int nID,BYTE bType,int nCnt )
{
	std::map<int,BYTE>::iterator itor	= mapQIMDB.find(nID);
	if(itor == mapQIMDB.end() || itor->second == 0)
	{
		mapQIMDB[nID]	= 1;
		bool	bFind	= false;
		QueryRlt xRlt;
		MySqlQuery* pCmd = NULL;
		if(g_pLogDB->pDBServer->Query(&xRlt,"SELECT * FROM "MARK_QICNT_LOG" where objid = %d and objtype=%d",nID,bType))
		{
			pCmd = xRlt.GetRlt();
			if(pCmd != NULL && pCmd->FetchRow())
			{
				bFind	= true;
			}
		}
		if(!bFind)
		{
			g_pLogDB->ExecuteSQL(false,"insert into "MARK_QICNT_LOG" (objid,objtype,objcnt) values (%d,%d,%d)",
				nID,bType,nCnt);
			return;
		}
	}

	g_pLogDB->ExecuteSQL(false,"update "MARK_QICNT_LOG" set objcnt=objcnt+%d where objid=%d and objtype=%d",nCnt,nID,bType);
}

void MarkGameDB::ChgOnline( bool bNew, bool bAdd )
{
	if(bNew)
	{
		if(bAdd)
			m_nOnlineNew++;
		else
			m_nOnlineNew--;
		//g_pDBase->ExecuteSQL(false,"update "MARK_ONLINECNT_TABLE" set newaccount=%d where timemark='now'",m_nOnlineNew);
	}
	else
	{
		if(bAdd)
			m_nOnlineOld++;
		else
			m_nOnlineOld--;
		g_pDBase->ExecuteSQL(false,"update "MARK_ONLINECNT_TABLE" set oldaccount=%d where timemark='now'",g_pGameCenter->GetOnlineCnt());
	}
}
