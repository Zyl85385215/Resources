#include "stdafx.h"
#include "MailCenter.h"
#include "TimeControl.h"
#include "UserData.h"
#include "ItemManage.h"
#include "ItemCenter.h"
#include "FsGameCenter.h"
#include "ZLogFile.h"
#include "WorldFlag.h"
#include "DBLogControl.h"
extern ZLogFile* g_pLogFile;
extern	FsGameCenter* g_pGameCenter;

MailCenter::MailCenter()
{
	InitializeCriticalSection(&m_xMailLock);
	m_pLMailWait	= xnList::Create();
	m_pDB			= NULL;
	m_dwMaxMailID	= 0;
}

MailCenter::~MailCenter()
{
	DeleteCriticalSection(&m_xMailLock);

	while(m_pLMailWait->Count)
	{
		delete (MailObject*)m_pLMailWait->Delete(0);
	}
	m_pLMailWait->Free();
}

MailObject* MailCenter::RequestSendMail( DWORD	dwSend,DWORD dwRecv, char* szT, char* szB, int nMoney, int* nIndex )
{
	MailObject*		pMail	= new MailObject();
	pMail->uinSender		= dwSend;
	pMail->uinRecver		= dwRecv;
	strcpy(pMail->szTitle,szT);
	strcpy(pMail->szBody,szB);
	pMail->byState			= STATE_MAIL_SENDWAITING;
	pMail->dwReturnTick		= CMTime::GetCurTickCount()+86400*7;
	//pMail->nItemID			= nItemID;
	//pMail->nCnt				= nCnt;
	pMail->nMoney			= nMoney;
	if (nIndex)
	{
		memcpy(pMail->nItemIndex,nIndex,sizeof(int)*nMaxMailItemCount);
	}else{
		memset(pMail->nItemIndex,0,sizeof(int)*nMaxMailItemCount);
	}
	pMail->SetItemID();

	EnterCriticalSection(&m_xMailLock);
	pMail->dwMailID			= m_dwMaxMailID++;
	m_pLMailWait->Add(pMail);
	LeaveCriticalSection(&m_xMailLock);
	pMail->DBInsertMail(m_pDB);

	AddMailLog(pMail);

	return pMail;
}

MailObject* MailCenter::RequestSendItemMail( DWORD dwSend, DWORD dwRecv, const char* szT,const char* szB, int nMoney /*= 0*/, int nItemId /*= 0*/,int nItemCnt /*= 0*/,bool bBind /*= false*/ )
{
	MailObject*		pMail	= new MailObject();
	pMail->uinSender		= dwSend;
	pMail->uinRecver		= dwRecv;
	strcpy(pMail->szTitle,szT);
	strcpy(pMail->szBody,szB);
	pMail->byState			= STATE_MAIL_SENDWAITING;
	pMail->dwReturnTick		= CMTime::GetCurTickCount()+86400*7;
	pMail->nMoney			= nMoney;
	if (nItemId)
	{
		ItemObj* pObj = ItemManage::CreateItemByID(nItemId,bBind);
		if (pObj)
		{
			if(pObj->m_pBase->byType != ITEMTYPE_EQUIP) pObj->SetDataVal(ITEM_AD_NUM,nItemCnt);
			int nIndex = g_pGameCenter->m_pItemManage->AddItemObj(pObj,true);
			pObj->SetDataVal(ITEM_AD_UIN,dwSend);
			pMail->AddItem(nIndex);
		}else{
			g_pLogFile->OutPut("创建item %d bind %d 失败",nItemId,bBind);
		}
	}
	EnterCriticalSection(&m_xMailLock);
	pMail->dwMailID			= m_dwMaxMailID++;
	m_pLMailWait->Add(pMail);
	LeaveCriticalSection(&m_xMailLock);
	pMail->DBInsertMail(m_pDB);

	AddMailLog(pMail);

	return pMail;
}

void MailCenter::DBInitMail(DBControl* pDB)
{
	m_pDB		= pDB;

	GetMaxMailID(pDB);
	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MAIL_TABLE" where state=%d or state=%d",STATE_MAIL_SENDWAITING,STATE_MAIL_RETURNWAITING))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		MailObject*		pMail	= new MailObject();
		pMail->dwMailID			= pCmd->GetDWORD("mailid");
		pMail->uinSender		= pCmd->GetDWORD("send");
		pMail->uinRecver		= pCmd->GetDWORD("recv");

		pMail->byState			= pCmd->GetInt("state");
		pMail->dwReturnTick		= pCmd->GetDWORD("timetick");
		//pMail->nItemID			= pCmd->GetInt("item");
		//pMail->nCnt				= pCmd->GetInt("itemcnt");
		pMail->nMoney			= pCmd->GetInt("money");
		strcpy(pMail->szTitle,pCmd->GetStr("title"));
		strcpy(pMail->szBody,pCmd->GetStr("body"));
		pMail->nItemIndex[0]		= pCmd->GetDWORD("index1");
		pMail->nItemIndex[1]		= pCmd->GetDWORD("index2");
		pMail->nItemIndex[2]		= pCmd->GetDWORD("index3");
		pMail->nItemIndex[3]		= pCmd->GetDWORD("index4");

		pMail->SetItemID();
		m_pLMailWait->Add(pMail);
	}

}

int MailCenter::GetMyMails( UserData* pUser, MailBox* pBox )
{
	EnterCriticalSection(&m_xMailLock);
	for(int i = m_pLMailWait->Count - 1; i >= 0; i--)
	{
		MailObject*	pMail	= (MailObject*)m_pLMailWait->Items[i];
		if(pMail->byState == STATE_MAIL_SENDWAITING && pMail->uinRecver == pUser->m_dwUin)
		{
			if (pBox->pListMails->Count>=30)
			{
				pUser->SendPopMsg("邮箱已满，无法接收新邮件，请清除过期邮件");
				continue;
				//pBox->DeleteMail(pMail->dwMailID);
			}
			pMail->byState	= STATE_MAIL_SEND;
			//记数据库
			pMail->DBUpdateMail(pUser->m_pDB);
			pBox->pListMails->Add(pMail);
			m_pLMailWait->Delete(i);
			
			continue;
		}
		if(pMail->byState == STATE_MAIL_RETURNWAITING && pMail->uinSender == pUser->m_dwUin)
		{
			pMail->byState	= STATE_MAIL_RETURN;
			//记数据库
			pMail->DBUpdateMail(pUser->m_pDB);

			pBox->pListMails->Add(pMail);
			m_pLMailWait->Delete(i);
			continue;
		}
		if (pMail->byState == STATE_MAIL_READ||pMail->byState == STATE_MAIL_GOT)
		{
			pBox->pListMails->Add(pMail);
			m_pLMailWait->Delete(i);
			continue;
		}
	}
	LeaveCriticalSection(&m_xMailLock);
	return pBox->pListMails->Count;
}

void MailCenter::UpdateMintue()
{
	EnterCriticalSection(&m_xMailLock);
	DWORD	dwCurTick	= CMTime::GetCurTickCount();
	for(int i = m_pLMailWait->Count - 1; i >= 0; i--)
	{
		MailObject*	pMail	= (MailObject*)m_pLMailWait->Items[i];
		if(pMail->byState == STATE_MAIL_SENDWAITING && pMail->dwReturnTick < dwCurTick)
		{//发送超期的邮件
			pMail->byState =	STATE_MAIL_RETURNWAITING;
			pMail->dwReturnTick	= dwCurTick	+ 86400*3;
			pMail->DBUpdateMail(m_pDB);
		}
		else if(pMail->byState == STATE_MAIL_RETURNWAITING && pMail->dwReturnTick < dwCurTick)
		{//退回超期的邮件
			pMail->byState =	STATE_MAIL_OVER;
			m_pLMailWait->Delete(i);
			pMail->DBDeleteMail(m_pDB);
			delete pMail;
		}
	}
	LeaveCriticalSection(&m_xMailLock);
}

int MailCenter::GetMaxMailID( DBControl* pDB )
{
	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,
		"select Max(mailid) as maxmailid from "RPG_MAIL_TABLE))
	{
		return 0;
	}
	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{
			m_dwMaxMailID = pCmd->GetInt("maxmailid") + 1;
		}
	}
	return m_dwMaxMailID;
}

void MailCenter::AddMailLog(MailObject*  pMail)
{
	g_pLogDB->ExecuteSQL(FALSE,"insert into "RPG_MAIL_LOG" (send,recv,title,body,money,item1,cnt1,item2,cnt2,ctime) values (%d,%d,'%s','%s',%d,%d,%d,%d,%d,now())",
		pMail->uinSender,pMail->uinRecver,pMail->szTitle,pMail->szBody,pMail->nMoney,pMail->nItemID[0],pMail->nCnt[0],pMail->nItemID[1],pMail->nCnt[1]);
}

int MailCenter::GetUserMailCntBy( DWORD dwUin )
{
	int nCount = 0;
	QueryRlt xRlt;
	if(!g_pDBase->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MAIL_TABLE" where (send=%d and state=%d) or (recv=%d and state=%d) or (recv=%d and state=%d) or (recv=%d and state=%d)",
		dwUin,STATE_MAIL_RETURN,dwUin,STATE_MAIL_SEND,dwUin,STATE_MAIL_READ,dwUin,STATE_MAIL_GOT))
		return nCount;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return nCount;

	while(pCmd->FetchRow())
	{
		nCount++;
	}
	return nCount;
}

MailBox::MailBox(DWORD dwUin)
{
	pListMails	= xnList::Create();
	m_dwUin		= dwUin;
	m_nMailCount = 0;
}

MailBox::~MailBox()
{
	while(pListMails->Count)
	{
		delete (MailObject*)pListMails->Delete(0);
	}
	pListMails->Free();
}

void MailBox::DBInitMBox(DBControl* pDB)
{
	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MAIL_TABLE" where (send=%d and state=%d) or (recv=%d and state=%d) or (recv=%d and state=%d) or (recv=%d and state=%d)",
		m_dwUin,STATE_MAIL_RETURN,m_dwUin,STATE_MAIL_SEND,m_dwUin,STATE_MAIL_READ,m_dwUin,STATE_MAIL_GOT))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		MailObject*		pMail	= new MailObject();
		pMail->dwMailID			= pCmd->GetDWORD("mailid");
		pMail->uinSender		= pCmd->GetDWORD("send");
		pMail->uinRecver		= pCmd->GetDWORD("recv");

		pMail->byState			= pCmd->GetInt("state");
		pMail->dwReturnTick		= pCmd->GetDWORD("timetick");
		pMail->nItemIndex[0]		= pCmd->GetDWORD("index1");
		pMail->nItemIndex[1]		= pCmd->GetDWORD("index2");
		pMail->nItemIndex[2]		= pCmd->GetDWORD("index3");
		pMail->nItemIndex[3]		= pCmd->GetDWORD("index4");
		//pMail->nItemID			= pCmd->GetInt("item");
		//pMail->nCnt				= pCmd->GetInt("itemcnt");
		pMail->nMoney			= pCmd->GetInt("money");
		strcpy(pMail->szTitle,pCmd->GetStr("title"));
		strcpy(pMail->szBody,pCmd->GetStr("body"));
		pMail->SetItemID();
		pListMails->Add(pMail);
	}
}

bool MailBox::DeleteMail( DWORD dwMailID )
{
	for (int i = 0; i < pListMails->Count; i++)
	{
		MailObject* pMail	= (MailObject*)pListMails->Items[i];
		if(pMail->dwMailID	== dwMailID)
		{
			pMail->DBDeleteMail(g_pDBase);
			pListMails->Delete(i);
			for (int j=0;j<nMaxMailItemCount;j++)
			{
				if(pMail->nItemIndex[j]) g_pGameCenter->m_pItemManage->RemoveItemObj(pMail->nItemIndex[j],true);
			}
			delete pMail;
			m_nMailCount--;
			return true;
		}
	}
	return false;
}

void MailBox::SetMailRead( DWORD	dwMailID,DBControl* pDB )
{
	for (int i = 0; i < pListMails->Count; i++)
	{
		MailObject* pMail	= (MailObject*)pListMails->Items[i];
		if(pMail->dwMailID	== dwMailID)
		{
			pMail->byState = STATE_MAIL_READ;
			pMail->DBUpdateMail(pDB);
		}
	}
}

MailObject* MailBox::GetMail( DWORD dwMailID )
{
	for (int i=0;i<pListMails->Count;i++)
	{
		MailObject* pObj = (MailObject*)pListMails->Items[i];
		if (pObj->dwMailID == dwMailID)
		{
			return pObj;
		}
	}
	return NULL;
}

void MailObject::DBInsertMail( DBControl* pDB )
{
	pDB->ExecuteSQL(FALSE,"insert into "RPG_MAIL_TABLE" (mailid, send, recv, state, money,timetick,title,body,index1,index2,index3,index4) values (%d, %d, %d, %d,%d,%d,'%s','%s',%d,%d,%d,%d)", 
		dwMailID,uinSender,uinRecver,byState,nMoney,dwReturnTick,szTitle,szBody,nItemIndex[0],nItemIndex[1],nItemIndex[2],nItemIndex[3]);
}

void MailObject::DBUpdateMail( DBControl* pDB )
{
	pDB->ExecuteSQL(FALSE,"update "RPG_MAIL_TABLE" set state=%d,timetick=%d,index1=%d,index2=%d,index3=%d,index4=%d,money=%d where mailid = %d",byState,dwReturnTick,nItemIndex[0],nItemIndex[1],nItemIndex[2],nItemIndex[3],nMoney,dwMailID);
}

void MailObject::DBDeleteMail( DBControl* pDB )
{
	pDB->ExecuteSQL(FALSE,"delete from "RPG_MAIL_TABLE" where mailid=%d",dwMailID);
}

void MailObject::SetItemID()
{
	for (int i=0;i<nMaxMailItemCount;i++)
	{
		if (!nItemIndex[i])
		{
			continue;
		}
		ItemObj* pItem = g_pGameCenter->m_pItemManage->GetItemObj(nItemIndex[i]);
		if (!pItem)
		{
			nItemID[i] = nCnt[i] = 0;
			return;
		}
		nItemID[i] = pItem->GetDataVal(ITEM_AD_TYPE);
		if (pItem->m_pBase->byType == ITEMTYPE_EQUIP)
		{
			nCnt[i] = 1;
		}else{
			nCnt[i] = pItem->GetDataVal(ITEM_AD_NUM);
		}
	}
}

bool MailObject::AddItem( int nIndex )
{
	for (int i=0;i<nMaxMailItemCount;i++)
	{
		if (nItemIndex[i])
		{
			continue;
		}
		nItemIndex[i] = nIndex;
		SetItemID();
		return true;

	}
	return false;
}

int MailObject::GetMailItemCount()
{
	int nCount = 0;
	for (int i = 0;i<nMaxMailItemCount;i++)
	{
		if (nItemIndex[i])
		{
			nCount++;
		}
	}
	return nCount;
}

SysMailCenter::SysMailCenter():m_nNextID(0)
{
	m_pLMailList = xnList::Create();
}

void SysMailCenter::DBInit( DBControl* pDB )
{
	m_pDB		= pDB;

	QueryRlt xRlt;
	if(!pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_SYS_MAIL_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		SysMailObj*		pMail	= new SysMailObj();
		pMail->dwID			= pCmd->GetDWORD("id");
		pMail->dwReturnTick		= pCmd->GetDWORD("timetick");
		pMail->nCnt		= pCmd->GetDWORD("itemcount");

		pMail->nItemID			= pCmd->GetInt("itemid");
		pMail->nMoney		= pCmd->GetDWORD("money");
		pMail->bBind		= pCmd->GetInt("bind");

		strcpy(pMail->szTitle,pCmd->GetStr("title"));
		strcpy(pMail->szBody,pCmd->GetStr("body"));

		m_pLMailList->Add(pMail);

		//if(m_nNextID < pMail->dwID) m_nNextID = pMail->dwID;
	}
	//m_nNextID = g_pGameCenter->m_pWorldFlag->GetFlagInt(nNextSysMailWFlag,1);
	m_nNextID	= GetMaxSysID();
	//m_nNextID++;
}

int SysMailCenter::AddSysMail(const char* szT,const char* szB,DWORD dwTime,int nMoney /*= 0*/, int nItemId /*= 0*/,int nItemCnt /*= 0*/,BYTE byBind /*= 0*/ )
{
	SysMailObj*		pMail	= new SysMailObj();
	pMail->dwID			= m_nNextID;
	pMail->dwReturnTick		= dwTime+CMTime::GetCurTickCount();
	pMail->nCnt		= nItemCnt;

	pMail->nItemID			= nItemId;
	pMail->nMoney		= nMoney;
	pMail->bBind		= byBind;

	strcpy(pMail->szTitle,szT);
	strcpy(pMail->szBody,szB);
	
	_insertDB(pMail);
	m_pLMailList->Add(pMail);
	m_nNextID++;
	g_pGameCenter->m_pWorldFlag->SetFlagInt(nNextSysMailWFlag,m_nNextID);
	return m_nNextID;
}

SysMailCenter::~SysMailCenter()
{
	while(m_pLMailList->Count) delete (SysMailObj*)m_pLMailList->Delete(0);
}

void SysMailCenter::Update()
{
	DWORD dwTime = CMTime::GetCurTickCount();
	for (int i=m_pLMailList->Count-1;i>=0;i--)
	{
		SysMailObj* pObj = (SysMailObj*)m_pLMailList->Items[i];
		if (dwTime>=pObj->dwReturnTick)
		{
			m_pLMailList->Delete(i);
			_deleteDB(pObj->dwID);
			delete pObj;
		}
	}
}

SysMailObj* SysMailCenter::GetSysMailFromID( DWORD dwID )
{
	for (int i=0;i<m_pLMailList->Count;i++)
	{
		SysMailObj* pObj = (SysMailObj*)m_pLMailList->Items[i];
		if (pObj->dwID>dwID)
		{
			return pObj;
		}
	}
	return NULL;
}

void SysMailCenter::RemoveSysMail( DWORD dwID )
{
	for (int i=0;i<m_pLMailList->Count;i++)
	{
		SysMailObj* pObj = (SysMailObj*)m_pLMailList->Items[i];
		if (pObj->dwID == dwID)
		{
			m_pLMailList->Delete(i);
			_deleteDB(dwID);
			delete pObj;
		}
	}
}

void SysMailCenter::_deleteDB( DWORD dwID )
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_SYS_MAIL_TABLE" where id=%d",dwID);
}

void SysMailCenter::_insertDB( SysMailObj* pObj )
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_SYS_MAIL_TABLE" (id, timetick, money,title,body,itemid,itemcount,bind) values (%d, %d, %d,'%s','%s',%d,%d,%d)", 
		pObj->dwID,pObj->dwReturnTick,pObj->nMoney,pObj->szTitle,pObj->szBody,pObj->nItemID,pObj->nCnt,pObj->bBind);
}

void SysMailCenter::GetSystemMail( UserData* pUser )
{
	if (!pUser)
	{
		return;
	}
	SysMailObj* pObj = NULL;
	while(pObj = GetSysMailFromID(pUser->m_nSysMailID)){
		g_pGameCenter->m_pMailCenter->RequestSendItemMail(0,pUser->m_dwUin,pObj->szTitle,pObj->szBody,pObj->nMoney,pObj->nItemID,pObj->nCnt,pObj->bBind);
		pUser->SetSysMailID(pObj->dwID);
	}
}

int SysMailCenter::GetMaxSysID()
{
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,
		"select Max(sysmailid) as maxsysid from "RPG_USER_TABLE))
	{
		return 1;
	}
	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{
			m_nNextID = pCmd->GetInt("maxsysid") + 1;
		}
	}
	return m_nNextID;
}
