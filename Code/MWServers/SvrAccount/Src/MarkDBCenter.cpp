#include "stdafx.h"
#include "MarkDBCenter.h"
#include "TimeControl.h"
#include "AccountCenter.h"
#include "DBTables.h"

MarkDBCenter::MarkDBCenter(CreateDB* pDB)
{
	m_pDB		= pDB;
	m_nDayMark	= 0;
}

MarkDBCenter::~MarkDBCenter()
{

}

void MarkDBCenter::CheckDayReset()
{
	tm tmNow = CMTime::GetCurTime();

	//int nID	= tmNow.tm_year*1000+tmNow.tm_yday;
	int nID	= (CMTime::GetCurTickCount()+28800)/86400;
	if(nID != m_nDayMark)
	{//数据库重新插入
		//检测是否有该天的信息
		m_nDayMark	= nID;
		QueryRlt xRlt;
		if(m_pDB->GetDBServer()->Query(&xRlt,"SELECT * FROM "MARK_REGACCOUNT_TABLE" where daynumber = %d",m_nDayMark))
		{
			MySqlQuery* pCmd = xRlt.GetRlt();
			if(pCmd != NULL)
			{
				if(pCmd->GetRecordsCnt() >= 8)	
					return;	//今天的已在数据库里
			}
		}
		CreateRecord(TP_NULL);
		CreateRecord(TP_SKPAY);
		CreateRecord(TP_SKPAY1);
		CreateRecord(TP_SKPAY2);
		CreateRecord(TP_ADRUC);
		CreateRecord(TP_ADR91);
		CreateRecord(TP_ADROPPO);
		CreateRecord(TP_ADRBDDK);
		CreateRecord(TP_ADRMI);
		CreateRecord(TP_ADRLX);
		CreateRecord(TP_ADR360);
		CreateRecord(TP_ADRAZ);
		CreateRecord(TP_ADRDJOY);
		CreateRecord(TP_ADRWDJ);
		CreateRecord(TP_ADRHUAWEI);
		CreateRecord(TP_ADRMZ);
		CreateRecord(TP_ADRMZW);
		CreateRecord(TP_ADRPPS);

		CreateRecord(TP_ADRJF);
		CreateRecord(TP_ADRMMY);
		CreateRecord(TP_ADRYYH);
		CreateRecord(TP_ADRYL);

		CreateRecord(TP_LW);
		CreateRecord(TP_ADWLQ);
		CreateRecord(TP_ADYYSC);
		CreateRecord(TP_ADBLXQDL);

		CreateRecord(TP_91PAY);
		CreateRecord(TP_PPSPAY);
		CreateRecord(TP_TBTPAY);
		CreateRecord(TP_KYPAY);
		CreateRecord(TP_DJOYPAY);
		CreateRecord(TP_ITOOLPAY);
		CreateRecord(TP_ABPAY);
		CreateRecord(TP_ABPAYPG);
		CreateRecord(TP_IAP);
		//插入这天的信息
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_NULL);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+11000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_SKPAY);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+12000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_91PAY);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+13000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_PPSPAY);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+14000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_TBTPAY);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+15000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_KYPAY);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+21000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_ABPAY);
// 		m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,regcount,payusercount,paytotalcount,effcount,ativatecount,createcount,logincount,logintotal,rmbpay,tpmark) values (%d,%d,%d,%d,%d,%d, %d, %d,%d,%d,%d,%d)",
// 			m_nDayMark+22000000,m_nDayMark, 0,0,0,0,0,0,0,0, 0,TP_ABPAYPG);

	}
}

void MarkDBCenter::AddNewRegAcc( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set regcount=regcount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);
}

void MarkDBCenter::AddLoginAcc( BYTE byTP,bool bFirst )
{
	if(bFirst)
		m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set logincount=logincount+1,logintotal=logintotal+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);
	else
		m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set logintotal=logintotal+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);
}


void MarkDBCenter::AddRmbPay( BYTE byTP, int nVal )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set rmbpay=rmbpay+%d where daynumber=%d and tpmark=%d",nVal,m_nDayMark,byTP);
}

void MarkDBCenter::AddNewAtivAcc( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set ativatecount=ativatecount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);
}

void MarkDBCenter::AddNewCreateAcc( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set createcount=createcount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);
}

void MarkDBCenter::AddNewEffAcc( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set effcount=effcount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);
}

void MarkDBCenter::AddPayUserAcc( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set payusercount=payusercount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);

}

void MarkDBCenter::AddPayTotalAcc( BYTE byTP,int nCount )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set paytotalcount=paytotalcount+%d where daynumber=%d and tpmark=%d",nCount,m_nDayMark,byTP);

}

void MarkDBCenter::CreateRecord( int nTPID )
{
	m_pDB->OnExecuteSQL("Insert Into "MARK_REGACCOUNT_TABLE" (id,daynumber,tpmark) values (%d,%d,%d)",
		m_nDayMark+nTPID*1000000,m_nDayMark,nTPID);
}

void MarkDBCenter::AddDayPayUserAcc( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set daypayusercount=daypayusercount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);

}

void MarkDBCenter::AddDayPayUserTime( BYTE byTP )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set daypaytimecount=daypaytimecount+1 where daynumber=%d and tpmark=%d",m_nDayMark,byTP);

}

void MarkDBCenter::AddDayPayTotalAcc( BYTE byTP,int nCount )
{
	m_pDB->OnExecuteSQL("update "MARK_REGACCOUNT_TABLE" set daypaytotalcount=daypaytotalcount+%d where daynumber=%d and tpmark=%d",nCount,m_nDayMark,byTP);

}
