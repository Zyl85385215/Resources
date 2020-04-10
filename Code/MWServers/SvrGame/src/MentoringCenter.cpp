#include "stdafx.h"
#include "MentoringCenter.h"
#include "UserList.h"
#include "UserData.h"
#include "MentoringCfg.h"
#include "MailCenter.h"
#include "TimeControl.h"
#include "FsGameCenter.h"
#include "ios_relation_packet.h"
MentoringCenter* g_pMentoringCenter = NULL;

static int FindMObjByUin(const void * Item1, const void * Item2)
{
	int id = *(int*)Item1;
	MRequestData * pData = *(MRequestData **)Item2;
	return (id - pData->pUser->dwUin);
}

MentoringCenter::MentoringCenter(DBControl*	pDB)
{
	m_dwMaxID = 10000;
	m_pDB = pDB;
	m_pTeacherList = xnList::Create();
	m_pRequestList = xnList::Create();
	m_pRubbishList = xnList::Create();
	m_nTListFlag = 1000;
}

MentoringCenter::~MentoringCenter()
{
	m_mapMentoring.clear();
	if(m_pTeacherList)
	{
		while(m_pTeacherList->Count)
			delete (MRequestData*)m_pTeacherList->Delete(0);
		m_pTeacherList->Free();
	}
	if(m_pRequestList)
	{
		while(m_pRequestList->Count)
			delete (MRequestData*)m_pRequestList->Delete(0);
		m_pRequestList->Free();
	}
	if(m_pRubbishList)
	{
		while(m_pRubbishList->Count)
			delete (MRequestData*)m_pRubbishList->Delete(0);
		m_pRubbishList->Free();
	}
}

int		MentoringCenter::_getMaxID()
{
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,
		"select Max(id) as maID from "RPG_MENTORING_TABLE))
	{
		return 10000;
	}
	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd != NULL)
	{
		if(pCmd->FetchRow())
		{
			m_dwMaxID = pCmd->GetInt("maID") + 1;
		}
	}
	if(m_dwMaxID < 10000)
		m_dwMaxID	= 10000;
	return m_dwMaxID;
}

void	MentoringCenter::DBInit()
{
	_getMaxID();
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MENTORING_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		MentoringData* pNew = new MentoringData();
		memset(pNew,0,sizeof(MentoringData));
		pNew->m_dwID = pCmd->GetInt("id");
		pNew->m_byState = pCmd->GetInt("state");
		int nTid = pCmd->GetInt("tuin");
		int nPid = pCmd->GetInt("puin");
		pNew->m_pTeacher = g_pUserList->GetUser(nTid);
		pNew->m_pPupil = g_pUserList->GetUser(nPid);
		if(pNew->m_pTeacher == NULL || pNew->m_pPupil == NULL)
		{
			delete pNew;
			continue;
		}
		pNew->m_bTeachFlag = pCmd->GetInt("tflag");
		pNew->m_bStudyFlag = pCmd->GetInt("pflag");
		m_mapMentoring.insert(std::make_pair(pNew->m_dwID,pNew));
	}

	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MREQUEST_TABLE))
		return;

	pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		MRequestData* pNew = new MRequestData();
		pNew->byType = pCmd->GetInt("type");
		pNew->dwEndTick = pCmd->GetInt("endtick");
		int nTempID = pCmd->GetInt("uin");
		pNew->pUser = g_pUserList->GetUser(nTempID);
		if(pNew->pUser == NULL)
		{
			delete pNew;
			continue;
		}
		pNew->nVal = pCmd->GetInt("value");
		if(pNew->byType == 1)
			m_pRequestList->Add((void*)pNew);
		else
			m_pTeacherList->Add((void*)pNew);
	}
	CheckTeacherList();
}

void	MentoringCenter::CheckTeacherList()
{
	xnList* pTemp = xnList::Create();
	for(int i = 0; i < m_pTeacherList->Count; i ++)
	{
		MRequestData* pData = (MRequestData*)m_pTeacherList->Items[i];
		GetRelationByuin(pData->pUser->dwUin,true,pTemp);
		while(pTemp->Count)
		{
			MentoringData* pCheck = (MentoringData*)pTemp->Delete(0);
			if(pCheck->m_byState == 1)
				pData->nVal ++;
		}
		pTemp->Clear();
	}
	pTemp->Free();
}

void	MentoringCenter::ResetDataFlag()
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.begin();
	while(itor != m_mapMentoring.end())
	{
		MentoringData* pData = itor->second;
		if(pData->m_byState == 1)
		{
			pData->m_bStudyFlag = 0;
			pData->m_bTeachFlag = 0;
			//_updateObj(pData);
			UserData* pUser = g_pGameCenter->GetPlayer(pData->m_pTeacher->dwUin);
			if(pUser)
				pUser->m_pUserMentoring->SendDataChanged(pData);
			pUser = g_pGameCenter->GetPlayer(pData->m_pPupil->dwUin);
			if(pUser)
				pUser->m_pUserMentoring->SendDataChanged(pData);
		}
		itor ++;
	}
	m_pDB->ExecuteSQL(FALSE,"update "RPG_MENTORING_TABLE" set tflag=0,pflag=0 where state=1");
}

void	MentoringCenter::Update()
{
	CMTime tmNow = CMTime::GetCurTime();
	for(int i = m_pTeacherList->Count-1; i >= 0; i --)
	{
		MRequestData* pData = (MRequestData*)m_pTeacherList->Items[i];
		if(pData->nVal == 0)
		{
			if(pData->dwEndTick == 0)
				pData->dwEndTick = tmNow.GetTickCount()+7*24*3600;
			else if(pData->dwEndTick > 0 && pData->dwEndTick <tmNow.GetTickCount())
			{
				m_nTListFlag ++;
				UserData* pUser = g_pGameCenter->GetPlayer(pData->pUser->dwUin);
				if(pUser && pUser->m_pUserMentoring)
				pUser->m_pUserMentoring->m_ifInstruct = false;
				m_pTeacherList->Delete(i);
				_deleteRequest(pData);
				delete pData;
			}
		}
		else
		{
			MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pData->pUser->nMentoringVal);
			if(pBase && pData->nVal >= pBase->nPuplecnt)
			{
				m_nTListFlag ++;
				UserData* pUser = g_pGameCenter->GetPlayer(pData->pUser->dwUin);
				if(pUser && pUser->m_pUserMentoring)
					pUser->m_pUserMentoring->m_ifInstruct = false;
				ClearTeachRequest(pData->pUser->dwUin);
				m_pTeacherList->Delete(i);
				_deleteRequest(pData);
				delete pData;
			}
			else if(pData->dwEndTick > 0)
				pData->dwEndTick = 0;
		}
	}
	for(int i = m_pRequestList->Count-1; i >= 0; i --)
	{
		MRequestData* pData = (MRequestData*)m_pRequestList->Items[i];
		if(!pData->ifDelete && pData->dwEndTick > 0 && pData->dwEndTick < tmNow.GetTickCount())
		{
			pData->ifDelete = true;
			_deleteRequest(pData);
			m_pRequestList->Delete(i);
			m_pRubbishList->Add(pData);
		}
	}
}

void	MentoringCenter::GetRelationByuin(int nUin,bool ifTeacher,OUT xnList* pList)
{
	if(pList == NULL)
		return;
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.begin();
	while(itor != m_mapMentoring.end())
	{
		MentoringData* pData = itor->second;
		if(ifTeacher && pData->m_pTeacher->dwUin == nUin)
			pList->Add(pData);
		else if(!ifTeacher && pData->m_pPupil->dwUin == nUin)
			pList->Add(pData);
		itor++;
	}
}

MentoringData*	MentoringCenter::GetRelation(int nTuin,int nPuin)
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.begin();
	while(itor != m_mapMentoring.end())
	{
		MentoringData* pData = itor->second;
		if(pData->m_byState == 1 && pData->m_pPupil->dwUin == nPuin && pData->m_pTeacher->dwUin == nTuin)
			return pData;
		itor++;
	}
	return NULL;
}

bool	MentoringCenter::DeleteObj(int nTuin,int nPuin)
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.begin();
	while(itor != m_mapMentoring.end())
	{
		MentoringData* pData = itor->second;
		if(pData->m_pPupil->dwUin == nPuin && pData->m_pTeacher->dwUin == nTuin)
		{
			_deleteObj(pData);
			pData->m_byState = 0;
			return true;
		}
		itor++;
	}
	return false;
}

bool	MentoringCenter::DeleteObj(MentoringData* pObj)
{
	return _deleteObj(pObj);
}

MRequestData*	MentoringCenter::GetTeacher(int nUin)
{
	for(int i = 0; i < m_pTeacherList->Count; i ++)
	{
		MRequestData* pRlt = (MRequestData*)m_pTeacherList->Items[i];
		if(pRlt->pUser->dwUin == nUin)
			return pRlt;
	}
	return NULL;
}

MRequestData*	MentoringCenter::GetRequest(int nPUin,int nTUin)
{
	for(int i = 0; i < m_pRequestList->Count; i ++)
	{
		MRequestData* pRlt = (MRequestData*)m_pRequestList->Items[i];
		if(!pRlt->ifDelete && pRlt->pUser->dwUin == nPUin && pRlt->nVal == nTUin)
			return pRlt;
	}
	return NULL;
}

bool	MentoringCenter::ClearRequest(int nPuin)
{
	for(int i = m_pRequestList->Count-1; i >= 0; i --)
	{
		MRequestData* pRlt = (MRequestData*)m_pRequestList->Items[i];
		if(!pRlt->ifDelete && pRlt->pUser->dwUin == nPuin)
		{
			pRlt->ifDelete = true;
			_deleteRequest(pRlt);
			m_pRequestList->Delete(i);
			m_pRubbishList->Add(pRlt);
		}
	}

	return true;
}

bool	MentoringCenter::ClearTeachRequest(int nTuin)
{
	for(int i = m_pRequestList->Count-1; i >= 0; i --)
	{
		MRequestData* pRlt = (MRequestData*)m_pRequestList->Items[i];
		if(!pRlt->ifDelete && pRlt->nVal == nTuin)
		{
			pRlt->ifDelete = true;
			_deleteRequest(pRlt);
			m_pRequestList->Delete(i);
			m_pRubbishList->Add(pRlt);
		}
	}

	return true;
}

bool	MentoringCenter::CheckGraduate(UserData* pUser)
{
	if(pUser == NULL || pUser->m_pUserMentoring == NULL)
		return false;
	if(pUser->m_xUWealth[UW_LV] < GRADUATEAGE)
		return false;
	bool ifGraduate = false;
	for(int i = pUser->m_pUserMentoring->m_pRelationList->Count-1; i >= 0; i --)
	{
		MentoringData* pData = (MentoringData*)pUser->m_pUserMentoring->m_pRelationList->Items[i];
		if(pData->m_pPupil == pUser->m_pMySimple && pData->m_byState == 1)
		{
			char szMsg[1024];
			sprintf(szMsg,"师德美名扬，桃李满天下！%s悉心教导，终将%s培养出师！",pData->m_pTeacher->szName,pData->m_pPupil->szName);
			ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);

			sprintf(szMsg,"师德美名扬，桃李满天下！在你的悉心教导下，%s已顺利成长出师！系统将解除你们的师徒关系！",pData->m_pPupil->szName);
			g_pGameCenter->m_pMailCenter->RequestSendMail(0,pData->m_pTeacher->dwUin,"%s",szMsg);

			pUser->m_pUserRecord->RunLuaRelationFunc(4,pData->m_pTeacher->dwUin);
			pData->m_byState = 255;
			_updateObj(pData);
			ifGraduate = true;
			pUser->m_pUserMentoring->DeleteData(i);
			MRequestData* pTeacher = GetTeacher(pData->m_pTeacher->dwUin);
			if(pTeacher)
				pTeacher->nVal--;
			break;
		}
	}
	if(ifGraduate)
	{
		int nExp = MentoringCfg::GetInstance().m_nGraduateexp;
		int nGold= MentoringCfg::GetInstance().m_nGraduategold;
		pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
		pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_MENTOR);
		pUser->m_pMySimple->nMentoringTick = 0;
		pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mentoringtick=%d where uin=%d",pUser->m_pMySimple->nMentoringTick,pUser->m_dwUin);
		char szMsg[128];
		sprintf(szMsg,"恭喜您毕业了，获得%d经验，%d金币",nExp,nGold);
		pUser->SendWarnMsg(szMsg);
		while(pUser->m_pUserMentoring->m_pRelationList->Count)
			pUser->m_pUserMentoring->DeleteData(0);
	}
	
	return true;
}

void	MentoringCenter::RequestInstruct(UserData* pUser)
{
	if(pUser == NULL || pUser->m_xUWealth[UW_LV] < GRADUATEAGE)
		return;

	for(int i = 0; i < m_pTeacherList->Count; i ++)
	{
		MRequestData* pRlt = (MRequestData*)m_pTeacherList->Items[i];
		if(pRlt->pUser->dwUin == pUser->m_dwUin)
		{
			pUser->SendWarnMsg("已登记，请等待学生来求学。");
			return;
		}
	}
	
	xnList* pList = xnList::Create();
	GetRelationByuin(pUser->m_dwUin,true,pList);
	int nCount = 0;
	for(int i = 0; i < pList->Count; i ++)
	{
		MentoringData* pData = (MentoringData*)pList->Items[i];
		if(pData->m_byState == 1)
			nCount ++;
	}
	CMTime tmNow = CMTime::GetCurTime();
	MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pUser->m_pMySimple->nMentoringVal);
	if(pBase && pBase->nPuplecnt > nCount)
	{
		MRequestData* pCheck = GetTeacher(pUser->m_dwUin);
		if(pCheck == NULL)
		{
			pCheck = new MRequestData();
			pCheck->byType = 0;
			pCheck->dwEndTick = tmNow.GetTickCount()+7*24*3600;
			pCheck->pUser = pUser->m_pMySimple;
			pCheck->nVal = nCount;
			pCheck->ifDelete = false;
			_insertRequest(pCheck);
			m_pTeacherList->Add((void*)pCheck);
			pUser->m_pUserMentoring->m_ifInstruct = true;
			m_nTListFlag ++;
			pUser->SendWarnMsg("成功在学院内注册成为导师，可向前来学习的学生授业");

			char szTemp[256];
			sprintf(szTemp,"俯首甘为孺子牛！%s 的 %s 广纳学生、递薪传火，还没找到导师的玩家不要错过哦~", pUser->m_byCountry == 1?(char*)"联盟":(char*)"部落",pUser->m_szNick);
			ChatCenter::GetInstance().SendSystemMsg(szTemp, 0, 0, 1, true);
		}
	}else
		pUser->SendWarnMsg("当前可收学生数量已满，待提升师德声望后可增加学生数量");
	pList->Free();
}

void	MentoringCenter::RequestSendItem(UserData* pUser,int nID)
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.find(nID);
	if(itor == m_mapMentoring.end())
		return;
	MentoringData* pData = itor->second;
	MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pData->m_pTeacher->nMentoringVal);
	if(pBase == NULL)
		return;
	if(pData->m_pTeacher == pUser->m_pMySimple)
	{
		if(pData->m_bTeachFlag != 0)
			return;
		int nExp = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylyexpcoe*pBase->byLv;
		int nGold = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylygoldcoe*pBase->byLv;
		int nHonour = MentoringCfg::GetInstance().m_nDaylyhonour;
		pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
		pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_MENTOR);
		pUser->m_pMySimple->nMentoringVal += nHonour;
		m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mentoringval=%d where uin=%d",pUser->m_pMySimple->nMentoringVal,pUser->m_dwUin);
		pData->m_bTeachFlag = 1;
		_updateObj(pData);
		pUser->m_pUserMentoring->SendDataChanged(pData);
		UserData* pPU = g_pGameCenter->GetPlayer(pData->m_pPupil->dwUin);
		if(pPU)
			pPU->m_pUserMentoring->SendDataChanged(pData);
		char szMsg[128];
		sprintf(szMsg,"获得%d经验，%d金币，%d师德声望",nExp,nGold,nHonour);
		pUser->SendWarnMsg(szMsg);
		if (pUser->m_pUserMentoring)
			pUser->m_pUserMentoring->SendHonour();
		pUser->m_pUserRecord->RunLuaRelationFunc(5,pData->m_pPupil->dwUin);
	}
	else if(pData->m_pPupil == pUser->m_pMySimple)
	{
		if(pData->m_bStudyFlag != 0)
			return;
		int nExp = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylyexpcoe;
		int nGold = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylygoldcoe;
		pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
		pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_MENTOR);
		pData->m_bStudyFlag = 1;
		_updateObj(pData);
		pUser->m_pUserMentoring->SendDataChanged(pData);
		UserData* pPU = g_pGameCenter->GetPlayer(pData->m_pTeacher->dwUin);
		if(pPU)
			pPU->m_pUserMentoring->SendDataChanged(pData);
		char szMsg[128];
		sprintf(szMsg,"获得%d经验，%d金币",nExp,nGold);
		pUser->SendWarnMsg(szMsg);
		pUser->m_pUserRecord->RunLuaRelationFunc(2,pData->m_pTeacher->dwUin);
	}
}

void	MentoringCenter::RequestGetItem(UserData* pUser,int nID)
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.find(nID);
	if(itor == m_mapMentoring.end())
		return;
	MentoringData* pData = itor->second;
	MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pData->m_pTeacher->nMentoringVal);
	if(pBase == NULL)
		return;
	_mbInfo* pInfo = NULL;
	int nRlt = rand()%100;
	int nTempCount = 0;
	for(int i = 0; i < pBase->pList->Count; i ++)
	{
		_mbInfo* pI = (_mbInfo*)pBase->pList->Items[i];
		nTempCount += pI->nPer;
		if(nRlt < nTempCount)
		{
			pInfo = pI;
			break;
		}
	}
	if(pData->m_pTeacher == pUser->m_pMySimple)
	{
		if(pData->m_bStudyFlag != 1)
			return;
		int nExp = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylyexpcoe*pBase->byLv;
		int nGold = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylygoldcoe*pBase->byLv;
		int nPower = MentoringCfg::GetInstance().m_nDaylypower;
		pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
		pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_MENTOR);
		pUser->m_xUWealth.AddWealth(UW_POW,nPower);
		if(pInfo)
			pUser->AddItemEx(pInfo->nID,pInfo->nNum,RMB_SAVE_MENTOR,UW_GOLD_MENTOR,ITEM_LOG_MENTOR);
		pData->m_bStudyFlag = 2;
		_updateObj(pData);
		pUser->m_pUserMentoring->SendDataChanged(pData);
		UserData* pPU = g_pGameCenter->GetPlayer(pData->m_pPupil->dwUin);
		if(pPU)
			pPU->m_pUserMentoring->SendDataChanged(pData);
		char szMsg[128];
		sprintf(szMsg,"获得%d经验，%d金币，%d体力",nExp,nGold,nPower);
		pUser->SendWarnMsg(szMsg);
	}
	else if(pData->m_pPupil == pUser->m_pMySimple)
	{
		if(pData->m_bTeachFlag != 1)
			return;
		int nExp = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylyexpcoe;
		int nGold = pData->m_pPupil->bLv*MentoringCfg::GetInstance().m_nDaylygoldcoe;
		pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
		pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_MENTOR);
		if(pInfo)
			pUser->AddItemEx(pInfo->nID,pInfo->nNum,RMB_SAVE_MENTOR,UW_GOLD_MENTOR,ITEM_LOG_MENTOR);
		pData->m_bTeachFlag = 2;
		_updateObj(pData);
		pUser->m_pUserMentoring->SendDataChanged(pData);
		UserData* pPU = g_pGameCenter->GetPlayer(pData->m_pTeacher->dwUin);
		if(pPU)
			pPU->m_pUserMentoring->SendDataChanged(pData);
		char szMsg[128];
		sprintf(szMsg,"获得%d经验，%d金币",nExp,nGold);
		pUser->SendWarnMsg(szMsg);
	}
}

void	MentoringCenter::RequestTeacherList(UserData* pUser,int nFlag)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_TEACHERLIST_RLT,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_TEACHERLIST_RLT,pPacket);
	if(m_pTeacherList->Count == 0)
	{
		pSendData->nCnt = 0;
		pSendData->ifClear = true;
		pSendData->nFlag = m_nTListFlag;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
		return;
	}
	if(nFlag == m_nTListFlag)
	{
		pSendData->nCnt = 0;
		pSendData->ifClear = false;
		pSendData->nFlag = m_nTListFlag;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
		return;
	}
	int n = 0;
	pSendData->nCnt = 0;
	pSendData->ifClear = true;
	pSendData->nFlag = m_nTListFlag;
	for(int i = 0; i < m_pTeacherList->Count; i ++)
	{
		MRequestData* pTemp = (MRequestData*)m_pTeacherList->Items[i];
		if(pTemp->pUser->bCountry != pUser->m_byCountry)
			continue;
		pSendData->szInfo[n].dwPuin = pTemp->pUser->dwUin;
		pSendData->szInfo[n].bPJob = pTemp->pUser->bJob;
		pSendData->szInfo[n].bPLvl = pTemp->pUser->bLv;
		strcpy(pSendData->szInfo[n].szPName,pTemp->pUser->szName);
		n++;
		if(n >= 20)
		{
			pSendData->nCnt = n;
			pPacket->wSize += n*sizeof(MCRequest);
			TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
			pPacket->wSize	= sizeof(RPGRELATION_S_SEND_TEACHERLIST_RLT_Data);
			pSendData->ifClear = false;
			n = 0;
		}
	}
	if(n > 0)
	{
		pSendData->nCnt = n;
		pPacket->wSize += n*sizeof(MCRequest);
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
	}
}

void	MentoringCenter::RequestStudy(UserData* pUser,DWORD dwTUin)
{
	if(pUser == NULL|| pUser->m_xUWealth[UW_LV] < STUDENTAGE|| pUser->m_xUWealth[UW_LV] >= GRADUATEAGE-4)
		return;
	xnList* pList = xnList::Create();
	GetRelationByuin(pUser->m_dwUin,false,pList);
	if(pList->Count > 0)
	{
		for(int i = 0; i < pList->Count; i ++)
		{
			MentoringData* pData = (MentoringData*)pList->Items[i];
			if(pData->m_byState == 1)
				return;
		}
	}
	pList->Free();
	MRequestData* pCheck = GetTeacher(dwTUin);
	if(pCheck == NULL || pCheck->pUser->bCountry != pUser->m_byCountry)
		return;
	if(GetRequest(pUser->m_dwUin,dwTUin))
		return;
	CMTime tmNow = CMTime::GetCurTime();
	if(pUser->m_pMySimple->nMentoringTick > tmNow.GetTickCount())
	{
		pUser->SendWarnMsg("暂时无法寻找导师");
		return;
	}
	MRequestData* pNew = new MRequestData();
	pNew->byType = 1;
	pNew->nVal = dwTUin;
	pNew->dwEndTick = tmNow.GetTickCount()+24*3600;
	pNew->pUser = pUser->m_pMySimple;
	pNew->ifDelete = false;
	_insertRequest(pNew);
	m_pRequestList->Add((void*)pNew);
	UserData* pTU = g_pGameCenter->GetPlayer(dwTUin);
	if(pTU)
		pTU->m_pUserMentoring->AddRequest(pNew);
	pUser->m_pUserMentoring->AddMyRequest(pNew);
}

void	MentoringCenter::DealWithRequest(UserData* pUser,DWORD dwPUin,bool bRlt)
{
	MRequestData* pTeacher = GetTeacher(pUser->m_dwUin);
	if(pTeacher == NULL)
	{
		pUser->SendWarnMsg("请先在教导主任那登记教学。");
		return;
	}
	MRequestData* pPupil = GetRequest(dwPUin,pUser->m_dwUin);
	if(pPupil == NULL || pPupil->ifDelete)
		return;
	CMTime tmNow = CMTime::GetCurTime();
	if(bRlt && pUser->m_pMySimple->nMentoringTick > tmNow.GetCurTickCount())
		return;
	pPupil->ifDelete = true;
	_deleteRequest(pPupil);
	UserData* pNPU = g_pGameCenter->GetPlayer(dwPUin);
	
//拒绝
	if(!bRlt)
	{
		if(pNPU)
		{
			char szMsg[128];
			sprintf(szMsg,"[%s]拒绝了你的学习申请",pUser->m_szNick);
			pNPU->SendWarnMsg(szMsg);
		}
		return;
	}
//同意
	MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pUser->m_pMySimple->nMentoringVal);
	if(pBase == NULL || pBase->nPuplecnt <= pTeacher->nVal)
	{
		pUser->SendWarnMsg("当前可收学生数量已满，待提升师德声望后可增加学生数量");
		return;
	}
	if(pPupil->pUser->bCountry != pUser->m_byCountry)
		return;
	UserSimple* pTempS = g_pUserList->GetUser(dwPUin);
	if(pTempS->nMentoringTick > tmNow.GetTickCount())
	{
		pUser->SendWarnMsg("该学生刚与导师脱离关系，无法接受教诲");
		return;
	}
	MentoringData* pCheck = GetRelation(pUser->m_dwUin,dwPUin);
	if(pCheck && pCheck->m_byState == 1)
		return;
	MentoringData* pNew = new MentoringData();
	memset(pNew,0,sizeof(MentoringData));
	pNew->m_dwID = m_dwMaxID;
	m_dwMaxID ++;
	pNew->m_byState = 1;
	pNew->m_pTeacher = pUser->m_pMySimple;
	pNew->m_pPupil = pTempS;
	_insertObj(pNew);
	m_mapMentoring.insert(std::make_pair(pNew->m_dwID,pNew));
	pTeacher->nVal++;
	pUser->m_pUserRecord->RunLuaRelationFunc(1,dwPUin);
	ClearRequest(dwPUin);

	pUser->m_pUserMentoring->AddData(pNew);
	if(pNPU)
	{
		char szMsg[128];
		sprintf(szMsg,"你已被[%s]收为学生",pUser->m_szNick);
		pNPU->SendWarnMsg(szMsg);
	}
	xnList* pTempList = xnList::Create();
	GetRelationByuin(pUser->m_dwUin,true,pTempList);
	while(pTempList->Count > 0)
	{
		MentoringData* pP = (MentoringData*)pTempList->Delete(0);
		if(pP->m_byState != 1)
			continue;
		UserData* pPU = g_pGameCenter->GetPlayer(pP->m_pPupil->dwUin);
		if(pPU)
			pPU->m_pUserMentoring->AddData(pNew);
		if(pNPU && pNPU != pPU)
			pNPU->m_pUserMentoring->AddData(pP);
	}
	pTempList->Free();
}

void	MentoringCenter::DealWithRequestP(UserData* pPupil,DWORD dwTuin,bool bRlt)
{
	if(pPupil == NULL)
		return;
	
 	UserData* pNPU = g_pGameCenter->GetPlayer(dwTuin);
	if(pNPU == NULL)
	{
		pPupil->SendWarnMsg("未找到师傅");
		return;
	}
	MRequestData* pTeacher = GetTeacher(dwTuin);
	if(pTeacher == NULL)
	{
		pNPU->SendWarnMsg("请先在教导主任那登记教学。");
		return;
	}

	//拒绝
	if(!bRlt)
	{

		char szMsg[128];
		sprintf(szMsg,"[%s]拒绝了你的收徒申请",pPupil->m_szNick);
		pNPU->SendWarnMsg(szMsg);
		return;
	}
	//同意
	MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pNPU->m_pMySimple->nMentoringVal);
	if(pBase == NULL || pBase->nPuplecnt <= pTeacher->nVal)
	{
		pNPU->SendWarnMsg("当前可收学生数量已满，待提升师德声望后可增加学生数量");
		pPupil->SendWarnMsg("对方可收学生数量已满");
		return;
	}
	if(pNPU->m_byCountry != pPupil->m_byCountry)
		return;

	CMTime tmNow = CMTime::GetCurTime();
	if(pNPU->m_pMySimple->nMentoringTick > tmNow.GetTickCount())
	{
		pPupil->SendWarnMsg("队伍中有人刚刚脱离关系，无法接受教诲");
		pNPU->SendWarnMsg("队伍中有人刚刚脱离关系，无法接受教诲");
		return;
	}
	if (pPupil->m_pUserMentoring->m_pRelationList->Count > 0)
	{
		pNPU->SendWarnMsg("对方已有导师。");
		return;
	}
	MentoringData* pCheck = GetRelation(dwTuin,pPupil->m_dwUin);
	if(pCheck && pCheck->m_byState == 1)
		return;
	MentoringData* pNew = new MentoringData();
	memset(pNew,0,sizeof(MentoringData));
	pNew->m_dwID = m_dwMaxID;
	m_dwMaxID ++;
	pNew->m_byState = 1;
	pNew->m_pTeacher = pNPU->m_pMySimple;
	pNew->m_pPupil = pPupil->m_pMySimple;
	_insertObj(pNew);
	m_mapMentoring.insert(std::make_pair(pNew->m_dwID,pNew));
	pTeacher->nVal++;
	pNPU->m_pUserRecord->RunLuaRelationFunc(1,pPupil->m_dwUin);
	ClearRequest(pPupil->m_dwUin);

	pNPU->m_pUserMentoring->AddData(pNew);
	char szMsg[128];
	sprintf(szMsg,"成功收[%s]为学生",pPupil->m_szNick);
	pNPU->SendWarnMsg(szMsg);
	sprintf(szMsg,"您已被[%s]收为学生",pNPU->m_szNick);
	pPupil->SendWarnMsg(szMsg);
	xnList* pTempList = xnList::Create();
	GetRelationByuin(pNPU->m_dwUin,true,pTempList);
	while(pTempList->Count > 0)
	{
		MentoringData* pP = (MentoringData*)pTempList->Delete(0);
		if(pP->m_byState != 1)
			continue;
		UserData* pPU = g_pGameCenter->GetPlayer(pP->m_pPupil->dwUin);
		if(pPU)
			pPU->m_pUserMentoring->AddData(pNew);
		/*if(pNPU && pNPU != pPU)
			pNPU->m_pUserMentoring->AddData(pP);*/
	}
	pTempList->Free();
}

void	MentoringCenter::BreakRelationship(UserData* pUser,int nID)
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.find(nID);
	if(itor == m_mapMentoring.end())
		return;
	CMTime tmNow = CMTime::GetCurTime();
	MentoringData* pData = itor->second;
	if(pUser->m_pMySimple == pData->m_pTeacher)
	{
		if(pUser->m_xUWealth[UW_GOLD] < 100000)
			return;
		if(DeleteObj(pData))
		{
			char szMsg[128];
			pData->m_byState = 0;
			pUser->m_xUWealth.SubWealth(UW_GOLD,100000);			
			pUser->m_pMySimple->nMentoringTick = tmNow.GetTickCount()+24*3600;
			pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mentoringtick=%d where uin=%d",pUser->m_pMySimple->nMentoringTick,pUser->m_dwUin);
			sprintf(szMsg,"您开除了学生[%s],失去100000金币，24小时内不能收徒",pData->m_pPupil->szName);
			pUser->SendWarnMsg(szMsg);
			UserData* pPU = g_pGameCenter->GetPlayer(pData->m_pPupil->dwUin);
			if(pPU)
			{
				sprintf(szMsg,"你的老师[%s]已将你开除了",pData->m_pTeacher->szName);
				pPU->SendWarnMsg(szMsg);
			}
			MRequestData* pTeacher = GetTeacher(pUser->m_dwUin);
			if(pTeacher)
				pTeacher->nVal--;
		}
	}
	else if(pUser->m_pMySimple == pData->m_pPupil)
	{
		pUser->m_pUserRecord->RunLuaRelationFunc(3,pData->m_pTeacher->dwUin);
		char szMsg[128];
		DeleteObj(pData);
		pData->m_byState = 0;
		while(pUser->m_pUserMentoring->m_pRelationList->Count)
			pUser->m_pUserMentoring->DeleteData(0);
		pUser->m_pMySimple->nMentoringTick = tmNow.GetTickCount()+24*3600;
		pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mentoringtick=%d where uin=%d",pUser->m_pMySimple->nMentoringTick,pUser->m_dwUin);
		sprintf(szMsg,"你和老师[%s]脱离了关系,24小时内无法寻找导师",pData->m_pTeacher->szName);
		pUser->SendWarnMsg(szMsg);
		UserData* pTU = g_pGameCenter->GetPlayer(pData->m_pTeacher->dwUin);
		if(pTU)
		{
			sprintf(szMsg,"您的学生[%s]和您脱离了关系",pData->m_pPupil->szName);
			pTU->SendWarnMsg(szMsg);
		}
		MRequestData* pTeacher = GetTeacher(pData->m_pTeacher->dwUin);
		if(pTeacher)
			pTeacher->nVal--;
	}
}

void	MentoringCenter::DeleteUser(DWORD dwUin)
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.begin();
	char szMsg[128];
	while(itor != m_mapMentoring.end())
	{
		MentoringData* pData = itor->second;
		if(pData->m_pTeacher->dwUin == dwUin && pData->m_byState != 0)
		{
			UserData* pPU = g_pGameCenter->GetPlayer(pData->m_pPupil->dwUin);
			if(pPU)
			{
				sprintf(szMsg,"你的老师[%s]已将你开除了",pData->m_pTeacher->szName);
				pPU->SendWarnMsg(szMsg);
			}
			pData->m_byState = 0;
			_deleteObj(pData);
		}
		else if(pData->m_pPupil->dwUin == dwUin && pData->m_byState != 0)
		{
			UserData* pTU = g_pGameCenter->GetPlayer(pData->m_pTeacher->dwUin);
			if(pTU)
			{
				sprintf(szMsg,"您的学生[%s]和您脱离了关系",pData->m_pPupil->szName);
				pTU->SendWarnMsg(szMsg);
			}
			pData->m_byState = 0;
			_deleteObj(pData);
		}
		itor++;
	}

	for(int i = m_pTeacherList->Count-1; i >= 0; i --)
	{
		MRequestData* pData = (MRequestData*)m_pTeacherList->Items[i];
		if(pData->pUser->dwUin == dwUin)
		{
			m_nTListFlag ++;
			ClearTeachRequest(pData->pUser->dwUin);
			m_pTeacherList->Delete(i);
			_deleteRequest(pData);
			delete pData;
			break;
		}
	}
	ClearRequest(dwUin);
}

void	MentoringCenter::RequestTotalInfo(UserData* pUser)
{
	if(pUser == NULL || pUser->m_pUserMentoring == NULL)
		return;
	CMTime tmNow = CMTime::GetCurTime();
	if(pUser->m_xUWealth[UW_LV] < GRADUATEAGE)
	{
		GetRelationByuin(pUser->m_dwUin,false,pUser->m_pUserMentoring->m_pRelationList);
		for(int i = pUser->m_pUserMentoring->m_pRelationList->Count -1; i >= 0; i --)
		{
			MentoringData* pData = (MentoringData*)pUser->m_pUserMentoring->m_pRelationList->Items[i];
			if(pData->m_byState != 1)
				pUser->m_pUserMentoring->m_pRelationList->Delete(i);
		}
		if(pUser->m_pUserMentoring->m_pRelationList->Count > 0)
		{
			MentoringData* pData = (MentoringData*)pUser->m_pUserMentoring->m_pRelationList->Delete(0);
			GetRelationByuin(pData->m_pTeacher->dwUin,true,pUser->m_pUserMentoring->m_pRelationList);
		}
		for(int i = 0; i < m_pRequestList->Count; i ++)
		{
			MRequestData* pTemp = (MRequestData*)m_pRequestList->Items[i];
			if(!pTemp->ifDelete)
			{
				if(pTemp->pUser == pUser->m_pMySimple)
					pUser->m_pUserMentoring->m_pMyRequestList->Add(pTemp);
			}
		}
	}
	else
	{
		GetRelationByuin(pUser->m_dwUin,true,pUser->m_pUserMentoring->m_pRelationList);
		for(int i = 0; i < m_pRequestList->Count; i ++)
		{
			MRequestData* pTemp = (MRequestData*)m_pRequestList->Items[i];
			if(!pTemp->ifDelete)
			{
				if(pTemp->nVal == pUser->m_dwUin)
					pUser->m_pUserMentoring->m_pRequestList->Add(pTemp);
			}
		}
		if(GetTeacher(pUser->m_dwUin))
			pUser->m_pUserMentoring->m_ifInstruct = true;
	}
	pUser->m_pUserMentoring->SendAll();
}

bool	MentoringCenter::_insertObj(MentoringData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_MENTORING_TABLE" (id,tuin,puin,tflag,pflag,state) values (%d,%d,%d,%d,%d,%d)",pObj->m_dwID,pObj->m_pTeacher->dwUin,pObj->m_pPupil->dwUin,pObj->m_bTeachFlag,pObj->m_bStudyFlag,pObj->m_byState);
	return true;
}

bool	MentoringCenter::_updateObj(MentoringData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_MENTORING_TABLE" set tflag=%d,pflag=%d,state=%d where id=%d",pObj->m_bTeachFlag,pObj->m_bStudyFlag,pObj->m_byState,pObj->m_dwID);
	return true;
}

bool	MentoringCenter::_deleteObj(MentoringData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_MENTORING_TABLE" where id = %d",pObj->m_dwID);
	return true;
}

bool	MentoringCenter::_insertRequest(MRequestData* pObj)
{
	if(pObj->byType == 0)
		m_pDB->ExecuteSQL(FALSE,"insert into "RPG_MREQUEST_TABLE" (uin,value,type,endtick) values (%d,%d,%d,%d)",pObj->pUser->dwUin,0,pObj->byType,pObj->dwEndTick);
	else
		m_pDB->ExecuteSQL(FALSE,"insert into "RPG_MREQUEST_TABLE" (uin,value,type,endtick) values (%d,%d,%d,%d)",pObj->pUser->dwUin,pObj->nVal,pObj->byType,pObj->dwEndTick);
	return true;
}

bool	MentoringCenter::_deleteRequest(MRequestData* pObj)
{
	if(pObj->byType == 0)
		m_pDB->ExecuteSQL(FALSE,"delete from "RPG_MREQUEST_TABLE" where uin = %d",pObj->pUser->dwUin);
	else
		m_pDB->ExecuteSQL(FALSE,"delete from "RPG_MREQUEST_TABLE" where uin = %d and value = %d",pObj->pUser->dwUin,pObj->nVal);
	return true;
}

void MentoringCenter::DealWithMsg( UserData* pUser, DWORD dwStuUin  )
{
	UserData* pNPU = g_pGameCenter->GetPlayer(dwStuUin);
	if (pNPU == NULL)
		return;

	MRequestData* pTeacher = GetTeacher(pUser->m_dwUin);
	if(pTeacher == NULL)
	{
		pUser->SendWarnMsg("请先在教导主任那登记教学。");
		return;
	}
	if (pNPU->m_pUserMentoring->m_pRelationList->Count > 0)
	{
		pUser->SendWarnMsg("对方已有导师");
		return;
	}
	MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pNPU->m_pMySimple->nMentoringVal);
	if(pBase == NULL || pBase->nPuplecnt <= pTeacher->nVal)
	{
		pUser->SendWarnMsg("当前可收学生数量已满，待提升师德声望后可增加学生数量");
		pNPU->SendWarnMsg("对方可收学生数量已满");
		return;
	}
	pUser->SendWarnMsg("成功发送收徒消息，等待学生回复。");
	BuildPacketEx(pPacket,RPGRELATION, S_SEND_MENTOR_MSG, buf, 1024);
	SET_DATA(pSendData,RPGRELATION, S_SEND_MENTOR_MSG, pPacket);
	strcpy(pSendData->szName, pUser->m_szNick);
	pSendData->dwSFUin = pUser->m_dwUin;
	TCP_SendPacketToUin(pNPU->m_dwUin, pPacket);
}

bool MentoringCenter::CheckTeacherUser( DWORD dwUin )
{
	std::map<DWORD,MentoringData*>::iterator itor = m_mapMentoring.begin();
	while(itor != m_mapMentoring.end())
	{
		MentoringData* pData = itor->second;
		if(pData && pData->m_pTeacher && pData->m_byState == 1 && pData->m_pTeacher->dwUin == dwUin)
			return true;
		itor++;
	}
	return false;
}

UserMentoring::UserMentoring(UserData* pUser)
{
	m_pUser = pUser;
	m_pRelationList = xnList::Create();
	m_pRequestList = xnList::Create();
	m_pMyRequestList = xnList::Create();
	m_ifInstruct = false;
	m_SendInstruct = false;

	m_ifShow = true;
	m_SendShow = true;
}

UserMentoring::~UserMentoring()
{
	if(m_pRequestList)
	{
		while(m_pRequestList->Count)
			m_pRequestList->Delete(0);
		m_pRequestList->Free();
	}
	if(m_pRelationList)
	{
		while(m_pRelationList->Count)
			m_pRelationList->Delete(0);
		m_pRelationList->Free();
	}
	if(m_pMyRequestList)
	{
		while(m_pMyRequestList->Count)
			m_pMyRequestList->Delete(0);
		m_pMyRequestList->Free();
	}
}

void UserMentoring::AddData(MentoringData* pData)
{
	m_pRelationList->Add(pData);
	SendDataAdd(pData);
}

void UserMentoring::DeleteData(int i)
{
	if(i >= 0 && i < m_pRelationList->Count)
	{
		MentoringData* pData = (MentoringData*)m_pRelationList->Items[i];
		m_pRelationList->Delete(i);
		SendDataDelete(pData);
	}
}

void UserMentoring::AddRequest(MRequestData* pData)
{
	m_pRequestList->Add(pData);
	SendRequestAdd(pData);
}

void UserMentoring::AddMyRequest(MRequestData* pData)
{
	m_pMyRequestList->Add(pData);
	SendMyRequestAdd(pData);
}

void UserMentoring::Update()
{
	for(int i = m_pRelationList->Count-1; i >= 0; i --)
	{
		MentoringData* pData = (MentoringData*)m_pRelationList->Items[i];
		if(pData->m_byState == 0)
		{
			m_pRelationList->Delete(i);
			SendDataDelete(pData);
		}
		else if(pData->m_byState == 255)
		{
			if(pData->m_pTeacher == m_pUser->m_pMySimple)
			{
				MentoringBase* pBase = MentoringCfg::GetInstance().GetCurBase(pData->m_pTeacher->nMentoringVal);
				if(pBase)
				{
					int nExp = MentoringCfg::GetInstance().m_nGraduateexp*pBase->byLv;
					int nGold = MentoringCfg::GetInstance().m_nGraduategold*pBase->byLv;
					int nHonour = MentoringCfg::GetInstance().m_nGraduatehonour;
					m_pUser->m_xUWealth.AddWealth(UW_EXP,nExp);
					m_pUser->m_xUWealth.AddWealth(UW_GOLD,nGold,1,NULL,UW_GOLD_MENTOR);
					m_pUser->m_pMySimple->nMentoringVal += nHonour;
					m_pUser->m_pDB->ExecuteSQL(FALSE,"update "RPG_USER_TABLE" set mentoringval=%d where uin=%d",m_pUser->m_pMySimple->nMentoringVal,m_pUser->m_dwUin);
					char szMsg[128];
					sprintf(szMsg,"您的学生[%s]毕业了，获得%d经验，%d金币，%d师德声望",pData->m_pPupil->szName,nExp,nGold,nHonour);
					m_pUser->SendWarnMsg(szMsg);
					if (m_pUser->m_pUserMentoring)
						m_pUser->m_pUserMentoring->SendHonour();
				}

				pData->m_byState = 0;
				g_pMentoringCenter->DeleteObj(pData);
			}
			m_pRelationList->Delete(i);
			SendDataDelete(pData);
		}
	}

	CMTime tmNow = CMTime::GetCurTime();
	if(m_pUser->m_pMySimple->nMentoringTick > tmNow.GetTickCount())
		m_ifShow = false;
	else
		m_ifShow = true;

	for(int i = m_pRequestList->Count-1; i >= 0; i --)
	{
		MRequestData* pData = (MRequestData*)m_pRequestList->Items[i];
		if(pData->ifDelete)
		{
			m_pRequestList->Delete(i);
			SendRequestDelete(pData);
		}
	}
	for(int i = m_pMyRequestList->Count-1; i >= 0; i --)
	{
		MRequestData* pData = (MRequestData*)m_pMyRequestList->Items[i];
		if(pData->ifDelete)
		{
			m_pMyRequestList->Delete(i);
			SendMyRequestDelete(pData);
		}
	}
	if(m_SendInstruct != m_ifInstruct)
		SendInstruct();

	if(m_ifShow != m_SendShow)
		SendShow();
}

void UserMentoring::SendAll()
{
	SendInstruct();
	SendShow();
	if(m_pRelationList->Count > 0)
	{
		BuildPacketEx(pPacket,RPGRELATION,S_SEND_MENTORING_ALL,buf,1024);
		SET_DATA(pSendData,RPGRELATION,S_SEND_MENTORING_ALL,pPacket);
		int n = 0;
		pSendData->nCnt = 0;
		for(int i = 0; i < m_pRelationList->Count; i ++)
		{
			MentoringData* pTemp = (MentoringData*)m_pRelationList->Items[i];
			pSendData->szInfo[n].dwID = pTemp->m_dwID;
			pSendData->szInfo[n].nHonour = pTemp->m_pTeacher->nMentoringVal;
			pSendData->szInfo[n].dwTeacher = pTemp->m_pTeacher->dwUin;
			pSendData->szInfo[n].bTeachFlag = pTemp->m_bTeachFlag;
			pSendData->szInfo[n].bTJob = pTemp->m_pTeacher->bJob;
			pSendData->szInfo[n].bTLvl = pTemp->m_pTeacher->bLv;
			strcpy(pSendData->szInfo[n].szTName,pTemp->m_pTeacher->szName);
			pSendData->szInfo[n].dwPupil = pTemp->m_pPupil->dwUin;
			pSendData->szInfo[n].bStudyFlag = pTemp->m_bStudyFlag;
			pSendData->szInfo[n].bPJob = pTemp->m_pPupil->bJob;
			pSendData->szInfo[n].bPLvl = pTemp->m_pPupil->bLv;
			strcpy(pSendData->szInfo[n].szPName,pTemp->m_pPupil->szName);
			n++;
			if(n >= 10)
			{
				pSendData->nCnt = n;
				pPacket->wSize += n*sizeof(MCData);
				TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
				pPacket->wSize	= sizeof(RPGRELATION_S_SEND_MENTORING_ALL_Data);
				n = 0;
			}
		}
		if(n > 0)
		{
			pSendData->nCnt = n;
			pPacket->wSize += n*sizeof(MCData);
			TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
		}
	}
	if(m_pRequestList->Count)
	{
		BuildPacketEx(pPacket,RPGRELATION,S_SEND_MREQUEST_ALL,buf,1024);
		SET_DATA(pSendData,RPGRELATION,S_SEND_MREQUEST_ALL,pPacket);
		int n = 0;
		pSendData->nCnt = 0;
		for(int i = 0; i < m_pRequestList->Count; i ++)
		{
			MRequestData* pTemp = (MRequestData*)m_pRequestList->Items[i];
			pSendData->szInfo[n].dwPuin = pTemp->pUser->dwUin;
			pSendData->szInfo[n].bPJob = pTemp->pUser->bJob;
			pSendData->szInfo[n].bPLvl = pTemp->pUser->bLv;
			strcpy(pSendData->szInfo[n].szPName,pTemp->pUser->szName);
			n++;
			if(n >= 20)
			{
				pSendData->nCnt = n;
				pPacket->wSize += n*sizeof(MCRequest);
				TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
				pPacket->wSize	= sizeof(RPGRELATION_S_SEND_MREQUEST_ALL_Data);
				n = 0;
			}
		}
		if(n > 0)
		{
			pSendData->nCnt = n;
			pPacket->wSize += n*sizeof(MCRequest);
			TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
		}
	}
	if(m_pMyRequestList->Count)
	{
		BuildPacketEx(pPacket,RPGRELATION,S_SEND_MYMREQUEST_ALL,buf,1024);
		SET_DATA(pSendData,RPGRELATION,S_SEND_MYMREQUEST_ALL,pPacket);
		int n = 0;
		pSendData->nCnt = 0;
		for(int i = 0; i < m_pMyRequestList->Count; i ++)
		{
			MRequestData* pTemp = (MRequestData*)m_pMyRequestList->Items[i];
			pSendData->nUin[n] = pTemp->nVal;
			n++;
			if(n >= 20)
			{
				pSendData->nCnt = n;
				pPacket->wSize += n*4;
				TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
				pPacket->wSize	= sizeof(RPGRELATION_S_SEND_MYMREQUEST_ALL_Data);
				n = 0;
			}
		}
		if(n > 0)
		{
			pSendData->nCnt = n;
			pPacket->wSize += n*4;
			TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
		}
	}
}

void UserMentoring::SendDataAdd(MentoringData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MENTORING_ADD,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MENTORING_ADD,pPacket);
	pSendData->xInfo.dwID = pData->m_dwID;
	pSendData->xInfo.nHonour = pData->m_pTeacher->nMentoringVal;
	pSendData->xInfo.dwTeacher = pData->m_pTeacher->dwUin;
	pSendData->xInfo.bTeachFlag = pData->m_bTeachFlag;
	pSendData->xInfo.bTJob = pData->m_pTeacher->bJob;
	pSendData->xInfo.bTLvl = pData->m_pTeacher->bLv;
	strcpy(pSendData->xInfo.szTName,pData->m_pTeacher->szName);
	pSendData->xInfo.dwPupil = pData->m_pPupil->dwUin;
	pSendData->xInfo.bStudyFlag = pData->m_bStudyFlag;
	pSendData->xInfo.bPJob = pData->m_pPupil->bJob;
	pSendData->xInfo.bPLvl = pData->m_pPupil->bLv;
	strcpy(pSendData->xInfo.szPName,pData->m_pPupil->szName);
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendDataChanged(MentoringData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MENTORING_CHG,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MENTORING_CHG,pPacket);
	pSendData->xInfo.dwID = pData->m_dwID;
	pSendData->xInfo.nHonour = pData->m_pTeacher->nMentoringVal;
	pSendData->xInfo.dwTeacher = pData->m_pTeacher->dwUin;
	pSendData->xInfo.bTeachFlag = pData->m_bTeachFlag;
	pSendData->xInfo.bTJob = pData->m_pTeacher->bJob;
	pSendData->xInfo.bTLvl = pData->m_pTeacher->bLv;
	strcpy(pSendData->xInfo.szTName,pData->m_pTeacher->szName);
	pSendData->xInfo.dwPupil = pData->m_pPupil->dwUin;
	pSendData->xInfo.bStudyFlag = pData->m_bStudyFlag;
	pSendData->xInfo.bPJob = pData->m_pPupil->bJob;
	pSendData->xInfo.bPLvl = pData->m_pPupil->bLv;
	strcpy(pSendData->xInfo.szPName,pData->m_pPupil->szName);
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendDataDelete(MentoringData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MENTORING_DEL,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MENTORING_DEL,pPacket);
	pSendData->nID = pData->m_dwID;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendRequestAdd(MRequestData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MREQUEST_ADD,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MREQUEST_ADD,pPacket);
	pSendData->xInfo.dwPuin = pData->pUser->dwUin;
	pSendData->xInfo.bPJob = pData->pUser->bJob;
	pSendData->xInfo.bPLvl = pData->pUser->bLv;
	strcpy(pSendData->xInfo.szPName,pData->pUser->szName);
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendRequestDelete(MRequestData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MREQUEST_DEL,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MREQUEST_DEL,pPacket);
	pSendData->nID = pData->pUser->dwUin;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendMyRequestAdd(MRequestData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MYMREQUEST_ADD,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MYMREQUEST_ADD,pPacket);
	pSendData->nUin = pData->nVal;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendMyRequestDelete(MRequestData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MYMREQUEST_DEL,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MYMREQUEST_DEL,pPacket);
	pSendData->nID = pData->nVal;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void UserMentoring::SendInstruct()
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_INSTRUCT_CHG,buf,1024);
	SET_DATA(pData,RPGRELATION,S_SEND_INSTRUCT_CHG,pPacket);
	pData->ifInstruct = m_ifInstruct;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	m_SendInstruct = m_ifInstruct;
}

void UserMentoring::SendShow()
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_SHOWFLAG_CHG,buf,1024);
	SET_DATA(pData,RPGRELATION,S_SEND_SHOWFLAG_CHG,pPacket);
	pData->ifShow = m_ifShow;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	m_SendShow = m_ifShow;
}

void UserMentoring::SendHonour()
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_HONOUR_ADD,buf,1024);
	SET_DATA(pData,RPGRELATION,S_SEND_HONOUR_ADD,pPacket);
	pData->nHonour = m_pUser->m_pMySimple->nMentoringVal;
	pData->dwUin	= m_pUser->m_dwUin;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		MentoringData* pTemp = (MentoringData*)m_pRelationList->Items[i];
		if (pTemp && pTemp->m_pPupil)
		{
			BuildPacketEx(pPacket,RPGRELATION,S_SEND_HONOUR_ADD,buf,1024);
			SET_DATA(pData,RPGRELATION,S_SEND_HONOUR_ADD,pPacket);
			pData->nHonour = m_pUser->m_pMySimple->nMentoringVal;
			pData->dwUin	= m_pUser->m_dwUin;
			TCP_SendPacketToUin(pTemp->m_pPupil->dwUin,pPacket);
		}
	}
}
