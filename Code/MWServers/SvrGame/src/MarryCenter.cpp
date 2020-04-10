#include "stdafx.h"
#include "MarryCenter.h"
#include "UserList.h"
#include "TimeControl.h"
#include "UserData.h"
#include "ios_relation_packet.h"
#include "MailCenter.h"
#include "FsGameCenter.h"
#include "MentoringCfg.h"
#include "GameAwardCfg.h"
#include "FamilyCenter.h"
#include "ChatCenter.h"
#include "CallMapEnemy.h"
#include "UserFlag.h"
#include <math.h>

MarryCenter* g_pMarryCenter = NULL;
extern char* szCountry[];

MarryData::MarryData()
{
	m_dwID = 0;
	m_dwWomanValue = 0;
	m_dwManValue = 0;
	m_dwWomanValue2 = 0;
	m_dwManValue2 = 0;
	m_pMan = NULL;
	m_pWoman = NULL;
	m_pManSkill = new NumberData(5);
	m_pWomanSkill = new NumberData(5);
	m_byState = 0;
	m_byAwardLv = 0;
	m_nMEnemyCnt = 0;
	m_nWEnemyCnt = 0;
	m_nMarryCnt	= 0;
	memset(&m_timeMarry,0,sizeof(CMTime));
}

MarryData::~MarryData()
{
	delete m_pManSkill;
	delete m_pWomanSkill;
}

MarryCenter::MarryCenter(DBControl*	pDB)
{
	m_dwMaxID = 10000;
	m_pDB = pDB;
	m_pRequestList = xnList::Create();
	m_nRListFlag = 1000;
	m_pCakeList = xnList::Create();
	m_dwCakeTick = 0;
	m_pStreet = NULL;
}

MarryCenter::~MarryCenter()
{
	m_mapMarry.clear();

	if(m_pRequestList)
	{
		while(m_pRequestList->Count)
			delete (MarryRequestData*)m_pRequestList->Delete(0);
		m_pRequestList->Free();
	}
	if(m_pCakeList)
	{
		while(m_pCakeList->Count)
			delete (MarryCakeObj*)m_pCakeList->Delete(0);
		m_pCakeList->Free();
	}
}

int		MarryCenter::_getMaxID()
{
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,
		"select Max(id) as maID from "RPG_MARRY_TABLE))
	{
		m_dwMaxID	= 10000;
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

void	MarryCenter::DBInit()
{
	_getMaxID();
	QueryRlt xRlt;
	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MARRY_TABLE))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		MarryData* pNew = new MarryData();
		pNew->m_dwID = pCmd->GetInt("id");
		pNew->m_byState = pCmd->GetInt("state");
		int nMid = pCmd->GetInt("muin");
		int nWid = pCmd->GetInt("wuin");
		pNew->m_pMan = g_pUserList->GetUser(nMid);
		pNew->m_pWoman = g_pUserList->GetUser(nWid);
		if(pNew->m_pMan == NULL || pNew->m_pWoman == NULL)
		{
			delete pNew;
			continue;
		}
		pNew->m_dwManValue = pCmd->GetInt("mvalue");
		pNew->m_dwWomanValue = pCmd->GetInt("wvalue");
		pNew->m_dwManValue2 = pCmd->GetInt("mpoint");
		pNew->m_dwWomanValue2 = pCmd->GetInt("wpoint");
		pNew->m_byAwardLv = pCmd->GetInt("awardlv");
		pNew->m_timeMarry = CMTime(pCmd->GetStr("marrytime"));
		int nSize = 0;
		BYTE*p = pCmd->GetBLOB("mskill",&nSize);
		pNew->m_pManSkill->SetData(p,nSize);
		p = pCmd->GetBLOB("wskill",&nSize);
		pNew->m_pWomanSkill->SetData(p,nSize);
		m_mapMarry.insert(std::make_pair(pNew->m_dwID,pNew));
	}

	if(!m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_MARRYREQUEST_TABLE))
		return;

	pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		MarryRequestData* pNew = new MarryRequestData();
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
		m_pRequestList->Add((void*)pNew);
	}
}

void	MarryCenter::Update()
{
	CMTime tmNow = CMTime::GetCurTime();
	for(int i = m_pRequestList->Count-1; i >= 0; i --)
	{
		MarryRequestData* pData = (MarryRequestData*)m_pRequestList->Items[i];
		if(!pData->ifDelete && pData->dwEndTick > 0 && pData->dwEndTick < tmNow.GetTickCount())
		{
			pData->ifDelete = true;
			_deleteRequest(pData);
			m_nRListFlag++;
			g_pGameCenter->m_pMailCenter->RequestSendMail(0,pData->pUser->dwUin,"征婚失败","漫长的等待，有缘人始终没有出现，不要气馁，再去登记一次吧！");
			UserData* pUser = g_pGameCenter->GetPlayer(pData->pUser->dwUin);
			if(pUser)
				pUser->m_pUserMarry->m_ifInstruct = false;
			m_pRequestList->Delete(i);
			delete pData;
		}
	}
	if(m_dwCakeTick < tmNow.GetCurTickCount())
	{
		for(int i = m_pCakeList->Count-1; i >= 0; i --)		//召蛋糕怪
		{
			MarryCakeObj* pObj = (MarryCakeObj*)m_pCakeList->Items[i];
			pObj->nCnt --;
			if(pObj->nCnt <= 0)
				m_pCakeList->Delete(i);
			int j = pObj->nNum;
			while(j > 0)
			{
				int nPosX = pObj->nPosx+rand()%500-250;
				if(nPosX < 0)
					nPosX = 0;
				int nPosY = pObj->nPosy+rand()%500-250;
				if(nPosY < 0)
					nPosY = 0;
				g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(1503,pObj->nMapIndex,nPosX,nPosY,11);
				j--;
			}
			m_dwCakeTick = tmNow.GetCurTickCount() + 30;
		}
	}
	if(m_pStreet)
	{
		UserData* pUser1 = g_pGameCenter->GetPlayer(m_pStreet->pData->m_pMan->dwUin);
		UserData* pUser2 = g_pGameCenter->GetPlayer(m_pStreet->pData->m_pWoman->dwUin);

		bool ifRemove = false;
		if(tmNow.GetCurTickCount() >= m_pStreet->dwEndTick)
		{
/*			if(pUser1 == NULL && pUser2 == NULL)*/
				ifRemove = true;
// 			else
// 			{
// 				UserData* pUser = NULL;
// 				if(pUser1)
// 					pUser = pUser1;
// 				else
// 					pUser = pUser2;
// 				if(pUser->m_nMIndex == m_pStreet->nMapIndex)
// 				{
// 					int nTX = pUser->GetPosX() - m_pStreet->ptEnd.x;
// 					int nTY = pUser->GetPosY() - m_pStreet->ptEnd.y;
// 					int nDelta = nTX*nTX + nTY*nTY;
// 					if(nDelta <= (200*200))
// 						ifRemove = true;
// 				}
// 			}
		}
		if(ifRemove)
		{
			if(pUser1)
				pUser1->m_pMObjCenter->SetMyCtrlLock(false,false,MAPOBJ_ENEMY,m_pStreet->nMasterID);
			if(pUser2)
				pUser2->m_pMObjCenter->SetMyCtrlLock(false,false,MAPOBJ_ENEMY,m_pStreet->nMasterID);
			g_pGameCenter->m_pCallEnemyCenter->RemoveEnemy(m_pStreet->nMasterID,m_pStreet->nMapIndex);
			for(int i = 0; i < m_pStreet->pFollowList->Count; i ++)
			{
				int nID = (int)m_pStreet->pFollowList->Items[i];
				g_pGameCenter->m_pCallEnemyCenter->RemoveEnemy(nID,m_pStreet->nMapIndex);
			}
			
			delete m_pStreet;
			m_pStreet = NULL;
		}
	}
}

void	MarryCenter::AddCakeEnemy(UserData* pUser,int nCnt,int nNum)
{
	if(pUser == NULL)
		return;
	MarryCakeObj* pObj = new MarryCakeObj();
	pObj->nCnt= nCnt;
	pObj->nNum = nNum;
	pObj->nMapIndex = pUser->m_nMIndex;
	pObj->nPosx = pUser->GetPosX();
	pObj->nPosy = pUser->GetPosY();
	m_pCakeList->Add(pObj);
}

void	MarryCenter::GetRelationByuin(int nUin,OUT xnList* pList)
{
	if(pList == NULL)
		return;
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.begin();
	while(itor != m_mapMarry.end())
	{
		MarryData* pData = itor->second;
		if(pData->m_pMan->dwUin == nUin || pData->m_pWoman->dwUin == nUin)
			pList->Add(pData);
		itor++;
	}
}

MarryData*	MarryCenter::GetRelation(int nTuin,int nPuin)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.begin();
	while(itor != m_mapMarry.end())
	{
		MarryData* pData = itor->second;
		if(pData->m_byState != 0 && ((pData->m_pMan->dwUin == nPuin && pData->m_pWoman->dwUin == nTuin) || (pData->m_pMan->dwUin == nTuin && pData->m_pWoman->dwUin == nPuin)))
			return pData;
		itor++;
	}
	return NULL;
}

MarryData*	MarryCenter::GetExistRelation(int nUin)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.begin();
	while(itor != m_mapMarry.end())
	{
		MarryData* pData = itor->second;
		if(pData->m_byState != 0 && (pData->m_pMan->dwUin == nUin || pData->m_pWoman->dwUin == nUin))
			return pData;
		itor++;
	}
	return NULL;
}

bool	MarryCenter::DeleteObj(int nTuin,int nPuin)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.begin();
	while(itor != m_mapMarry.end())
	{
		MarryData* pData = itor->second;
		if((pData->m_pMan->dwUin == nPuin && pData->m_pWoman->dwUin == nTuin) || (pData->m_pMan->dwUin == nTuin && pData->m_pWoman->dwUin == nPuin))
		{
			_deleteObj(pData);
			pData->m_byState = 0;
			return true;
		}
		itor++;
	}
	return false;
}

bool	MarryCenter::DeleteObj(MarryData* pObj)
{
	return _deleteObj(pObj);
}

MarryRequestData*	MarryCenter::GetRequest(int nUin)
{
	for(int i = 0; i < m_pRequestList->Count; i ++)
	{
		MarryRequestData* pRlt = (MarryRequestData*)m_pRequestList->Items[i];
		if(pRlt->pUser->dwUin == nUin)
			return pRlt;
	}
	return NULL;
}

bool	MarryCenter::ClearRequest(int nPuin)
{
	bool ifDelete = false;
	for(int i = m_pRequestList->Count -1; i >= 0; i --)
	{
		MarryRequestData* pRlt = (MarryRequestData*)m_pRequestList->Items[i];
		if(!pRlt->ifDelete && pRlt->pUser->dwUin == nPuin)
		{
			pRlt->ifDelete = true;
			_deleteRequest(pRlt);
			UserData* pUser = g_pGameCenter->GetPlayer(pRlt->pUser->dwUin);
			if(pUser)
				pUser->m_pUserMarry->m_ifInstruct = false;
			m_nRListFlag ++;
			ifDelete = true;
			m_pRequestList->Delete(i);
			delete pRlt;
		}
	}

	return ifDelete;
}

void	MarryCenter::DeleteUser(DWORD dwUin)
{
	ClearRequest(dwUin);
	MarryData* pData = GetExistRelation(dwUin);
	if(pData)
	{
		int nTUin = pData->m_pMan->dwUin;
		UserSimple* pUser = pData->m_pWoman;
		if(pData->m_pMan->dwUin == dwUin)
		{
			nTUin = pData->m_pWoman->dwUin;
			pUser = pData->m_pMan;
		}
		UserData* pTU = g_pGameCenter->GetPlayer(nTUin);
		if(pTU)
		{
			pTU->m_pUserMarry->ResetObj();
			pTU->m_pItemCenter->RemoveItemByID(MARRY_ITEM_EFF,1);
			pTU->RecalcParam();
		}
		char szMsg[128];
		sprintf(szMsg,"%s和你分道扬镳，强制和您解除了夫妻关系",pUser->szName);
		g_pGameCenter->m_pMailCenter->RequestSendMail(pUser->dwUin,nTUin,"离婚",szMsg);
		DeleteObj(pData);
	}
}

bool	MarryCenter::RequestInstruct(UserData* pUser)
{
	if(pUser == NULL)
		return false;
	CMTime tmNow = CMTime::GetCurTime();

	xnList* pList = xnList::Create();
	GetRelationByuin(pUser->m_dwUin,pList);
	int nCount = 0;
	for(int i = 0; i < pList->Count; i ++)
	{
		MarryData* pData = (MarryData*)pList->Items[i];
		if(pData->m_byState != 0)
			nCount ++;
	}
	if(nCount > 0)
		return false;
	MarryRequestData* pCheck = GetRequest(pUser->m_dwUin);
	if(pCheck == NULL)
	{
		if (pUser->m_pUserFlag->GetFlag(52))
		{
			pUser->SendWarnMsg("每天只能登记一次，请明天再来吧！");
			return false;
		}

		pUser->m_pUserFlag->SetFlag(52,1);
		pCheck = new MarryRequestData();
		pCheck->byType = 0;
		pCheck->dwEndTick = tmNow.GetTickCount()+7*24*3600;
		pCheck->pUser = pUser->m_pMySimple;
		pCheck->nVal = 0;
		pCheck->ifDelete = false;
		_insertRequest(pCheck);
		m_pRequestList->Add((void*)pCheck);
		m_nRListFlag ++;
		pUser->SendWarnMsg("登记成功,耐心等待有缘人吧");
		char szTemp[256];
		sprintf(szTemp,"有缘千里来相会！%s的%s 发布了征婚启示，单身的玩家不要错过美好姻缘哦~", pUser->m_byCountry == 1?(char*)"联盟":(char*)"部落",pUser->m_szNick);
		ChatCenter::GetInstance().SendSystemMsg(szTemp, 0, 0, 1, true);
		pUser->m_pUserMarry->m_ifInstruct = true;
		return true;
	}
	pList->Free();
	return false;
}

void	MarryCenter::GetRequestList(UserData* pUser,int nFlag)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_REQUESTLIST,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MARRY_REQUESTLIST,pPacket);
	if(m_pRequestList->Count == 0)
	{
		pSendData->nCnt = 0;
		pSendData->ifClear = true;
		pSendData->nFlag = m_nRListFlag;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
		return;
	}
	if(nFlag == m_nRListFlag)
	{
		pSendData->nCnt = 0;
		pSendData->ifClear = false;
		pSendData->nFlag = m_nRListFlag;
		TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
		return;
	}
	int n = 0;
	pSendData->nCnt = 0;
	pSendData->ifClear = true;
	pSendData->nFlag = m_nRListFlag;
	for(int i = 0; i < m_pRequestList->Count; i ++)
	{
		MRequestData* pTemp = (MRequestData*)m_pRequestList->Items[i];
		if(pTemp->pUser->bCountry != pUser->m_byCountry)
			continue;
		if(pTemp->ifDelete)
			continue;
		pSendData->szInfo[n].dwPuin = pTemp->pUser->dwUin;
		pSendData->szInfo[n].bPJob = pTemp->pUser->bJob;
		pSendData->szInfo[n].bPLvl = pTemp->pUser->bLv;
		pSendData->szInfo[n].bSex = pTemp->pUser->bSex;
		strcpy(pSendData->szInfo[n].szPName,pTemp->pUser->szName);
		n++;
		if(n >= 20)
		{
			pSendData->nCnt = n;
			pPacket->wSize += n*sizeof(MCRequest);
			TCP_SendPacketToUin(pUser->m_dwUin,pPacket);
			pPacket->wSize	= sizeof(RPGRELATION_S_SEND_MARRY_REQUESTLIST_Data);
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

void	MarryCenter::BreakRelationship(UserData* pUser,int nID)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	CMTime tmNow = CMTime::GetCurTime();
	MarryData* pData = itor->second;
	if(pUser->m_pMySimple == pData->m_pMan || pUser->m_pMySimple == pData->m_pWoman)
	{
		if(pUser->m_xUWealth[UW_GOLD] < 1000000)
		{
			pUser->SendWarnMsg("金币不足");
			return;
		}
		if(DeleteObj(pData))
		{
			UserSimple* pTarget = NULL;
			if(pUser->m_pMySimple == pData->m_pMan)
				pTarget = pData->m_pWoman;
			else
				pTarget = pData->m_pMan;
			char szMsg[128];
			pData->m_byState = 0;
			pUser->m_xUWealth.SubWealth(UW_GOLD,1000000);
			sprintf(szMsg,"您已成功与%s解除关系",pTarget->szName);
			pUser->SendWarnMsg(szMsg);
			pUser->m_pUserMarry->ResetObj();
			pUser->m_pItemCenter->RemoveItemByID(MARRY_ITEM_EFF,1);
			UserData* pTU = g_pGameCenter->GetPlayer(pTarget->dwUin);
			if(pTU)
			{
				pTU->m_pUserMarry->ResetObj();
				pTU->m_pItemCenter->RemoveItemByID(MARRY_ITEM_EFF,1);
				pTU->RecalcParam();
			}
			sprintf(szMsg,"%s和你分道扬镳，强制和您解除了夫妻关系",pUser->m_szNick);
			g_pGameCenter->m_pMailCenter->RequestSendMail(pUser->m_dwUin,pTarget->dwUin,"离婚",szMsg);
			pUser->RecalcParam();
			pUser->m_pUserRecord->RunLuaMarryFunc(3,pTarget->dwUin,0,0);
		}
	}
}

bool	MarryCenter::GetEngaged(DWORD dwUin1,DWORD dwUin2)
{
	MarryData* pData = GetRelation(dwUin1,dwUin2);
	if(pData)
		return false;
	if(GetExistRelation(dwUin1) || GetExistRelation(dwUin2))
		return false;
	UserSimple* pUser1 = g_pUserList->GetUser(dwUin1);
	UserSimple* pUser2 = g_pUserList->GetUser(dwUin2);
	if(pUser1 == NULL || pUser2 == NULL || pUser1->bSex == pUser2->bSex || pUser1->bCountry != pUser2->bCountry)
		return false;
	pData = new MarryData();
	pData->m_dwID = m_dwMaxID;
	m_dwMaxID ++;
	if(pUser1->bSex == 1)
	{
		pData->m_pMan = pUser1;
		pData->m_pWoman = pUser2;
	}
	else
	{
		pData->m_pMan = pUser2;
		pData->m_pWoman = pUser1;
	}
	pData->m_byState = 1;
	_insertObj(pData);
	m_mapMarry.insert(std::make_pair(pData->m_dwID,pData));
	UserData* pUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
	if(pUser)
	{
		pUser->m_pUserMarry->AddData(pData);
		pUser->SendWarnMsg("订婚成功!");
		pUser->m_pUserMarry->ResetObj();
	}
	pUser = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
	if(pUser)
	{
		pUser->m_pUserMarry->AddData(pData);
		pUser->SendWarnMsg("订婚成功!");
		pUser->m_pUserMarry->ResetObj();
	}
	ClearRequest(dwUin1);
	ClearRequest(dwUin2);
	return true;
}

void	MarryCenter::AddMarryValue(UserData* pU,int nID,int nVal)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	MarryData* pData = itor->second;
	pData->m_dwManValue += nVal;
	pData->m_dwWomanValue += nVal;
	pData->m_dwManValue2 += nVal;
	pData->m_dwWomanValue2 += nVal;
	_updateObj(pData);
	int nTU = 0;
	if(pU->m_pMySimple == pData->m_pMan)
	{
		nTU = pData->m_pWoman->dwUin;
		pU->m_pUserRecord->RunLuaMarryFunc(4,nTU,pData->m_dwManValue,pData->m_dwWomanValue);
	}
	else
	{
		nTU = pData->m_pMan->dwUin;
		pU->m_pUserRecord->RunLuaMarryFunc(4,nTU,pData->m_dwWomanValue,pData->m_dwManValue);
	}
	pU->m_pUserMarry->SendDataChanged(pData);
	UserData* pUser = g_pGameCenter->GetPlayer(nTU);
	if(pUser)
		pUser->m_pUserMarry->SendDataChanged(pData);
}

void	MarryCenter::AddMarryValue(UserData* pU,int nID,int nVal,bool ifMan)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	MarryData* pData = itor->second;
	if(ifMan)
	{
		pData->m_dwManValue += nVal;
		pData->m_dwManValue2 += nVal;
	}
	else
	{
		pData->m_dwWomanValue += nVal;
		pData->m_dwWomanValue2 += nVal;
	}
	_updateObj(pData);
	int nTU = 0;
	if(pU->m_pMySimple == pData->m_pMan)
	{
		nTU = pData->m_pWoman->dwUin;
		pU->m_pUserRecord->RunLuaMarryFunc(4,nTU,pData->m_dwManValue,pData->m_dwWomanValue);
	}
	else
	{
		nTU = pData->m_pMan->dwUin;
		pU->m_pUserRecord->RunLuaMarryFunc(4,nTU,pData->m_dwWomanValue,pData->m_dwManValue);
	}
	pU->m_pUserMarry->SendDataChanged(pData);
	UserData* pUser = g_pGameCenter->GetPlayer(nTU);
	if(pUser)
		pUser->m_pUserMarry->SendDataChanged(pData);
}

bool	MarryCenter::CheckMarryConditions(UserData* pUser,int nID)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
	{
		pUser->SendWarnMsg("您尚未订婚");
		return false;
	}
	MarryData* pData = itor->second;
	if(pData->m_byState == 0 || pData->m_byState >= 2)
	{
		pUser->SendWarnMsg("您已结过婚");
		return false;
	}
	if(pData->m_pMan != pUser->m_pMySimple)
	{
		pUser->SendWarnMsg("婚礼还是让男士申请吧");
		return false;
	}
	if(pData->m_pMan->bLv < 30 || pData->m_pWoman->bLv < 30)
	{
		pUser->SendWarnMsg("夫妻双方需等级达到30级才能举行婚礼");
		return false;
	}
	if(pUser->m_xUWealth[UW_GOLD] < 520520)
	{
		pUser->SendWarnMsg("先准备好520520的礼金，再来申请婚礼吧");
		return false;
	}

	if(pData->m_dwManValue < 1000 || pData->m_dwWomanValue < 1000)
	{
		pUser->SendWarnMsg("夫妻双方友好度达到1000才能举行婚礼");
		return false;
	}
	return true;
}

bool	MarryCenter::RequestMarry(UserData* pUser,int nID)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
	{
		pUser->SendWarnMsg("您尚未订婚");
		return false;
	}
	MarryData* pData = itor->second;
	if(pData->m_byState == 0 || pData->m_byState >= 2)
	{
		pUser->SendWarnMsg("您已结过婚");
		return false;
	}
	if(pData->m_pMan != pUser->m_pMySimple)
	{
		pUser->SendWarnMsg("婚礼还是让男士申请吧");
		return false;
	}
	if(pData->m_pMan->bLv < 30 || pData->m_pWoman->bLv < 30)
	{
		pUser->SendWarnMsg("夫妻双方需等级达到30级才能举行婚礼");
		return false;
	}
	if(pUser->m_xUWealth[UW_GOLD] < 520520)
	{
		pUser->SendWarnMsg("先准备好520520的礼金，再来申请婚礼吧");
		return false;
	}

	if(pData->m_dwManValue < 1000 || pData->m_dwWomanValue < 1000)
	{
		pUser->SendWarnMsg("夫妻双方友好度达到1000才能举行婚礼");
		return false;
	}
	pUser->m_xUWealth.SubWealth(UW_GOLD,520520);
	pData->m_byState = 2;
	_updateObj(pData);
	pData->m_timeMarry = CMTime::GetCurTime();
	m_pDB->ExecuteSQL(FALSE,"update "RPG_MARRY_TABLE" set marrytime=now() where id=%d",pData->m_dwID);
	char szMsg[128];
	sprintf(szMsg,"以%s法律所赋予的合法权利，我现在宣布，你们正式结为合法夫妻。",szCountry[pUser->m_byCountry]);
	UserData* pSUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
	if(pSUser)
	{
		pSUser->m_pUserMarry->SendDataChanged(pData);
		pSUser->m_pUserMarry->ResetObj();
		pSUser->PopMarryMessage("结婚",szMsg);
		pSUser->AddItemEx(MARRY_ITEM_CAKE,10,1,0,ITEM_LOG_MERRY);
		pSUser->AddItemEx(MARRY_ITEM_EFF,1,1,0,ITEM_LOG_MERRY);
	}
	pSUser = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
	if(pSUser)
	{
		pSUser->m_pUserMarry->SendDataChanged(pData);
		pSUser->m_pUserMarry->ResetObj();
		pSUser->PopMarryMessage("结婚",szMsg);
		pSUser->AddItemEx(MARRY_ITEM_CAKE,10,1,0,ITEM_LOG_MERRY);
		pSUser->AddItemEx(MARRY_ITEM_EFF,1,1,0,ITEM_LOG_MERRY);
	}
	sprintf(szMsg,"%s和%s成功交换钻戒，正式结为合法夫妻。",pData->m_pMan->szName,pData->m_pWoman->szName);
	ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
	pUser->m_pMObjCenter->ChgWeather(11,25);
	pUser->m_pUserRecord->RunLuaMarryFunc(2,pData->m_pWoman->dwUin,0,0);
	AddCakeEnemy(pUser, 3, 10);
	return true;
}

// int		MarryCenter::CheckMarryTime(int nID)
// {
// 	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
// 	if(itor == m_mapMarry.end())
// 		return 0;
// 	MarryData* pData = itor->second;
// 	if(pData->m_byState < 2)
// 		return 0;
// 	if(pData->m_timeMarry.GetTickCount() == 0)
// 		return 0;
// 	else
// 	{
// 		CMTime tmNow = CMTime::GetCurTime();
// 		int nCnt = tmNow.GetSystime().wYear - pData->m_timeMarry.GetSystime().wYear;
// 		if(tmNow.GetSystime().wMonth == pData->m_timeMarry.GetSystime().wMonth && tmNow.GetSystime().wDay == pData->m_timeMarry.GetSystime().wDay)
// 			nCnt ++;
// 		else
// 			nCnt = 0;
// 		return nCnt;
// 	}
// }

void	MarryCenter::RequestAward(UserData* pUser,int nID,BYTE byType)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	MarryData* pData = itor->second;
	if(byType == 0 ||byType >= MarrySkillCfg::GetInstance().m_pAwardList->Count+1)
		return;
	int nFlag = pUser->m_pUserFlag->GetFlag(29);
	int nCheck = 0x01;
	nCheck = nCheck<<(byType-1);
	if((nFlag & nCheck) == nCheck)
	{
		pUser->SendWarnMsg("您已经领取过该奖励了");
		return;
	}
	int nTemp = 0;
	if(pUser->m_pMySimple == pData->m_pMan)
		nTemp = pData->m_dwManValue2;
	else
		nTemp = pData->m_dwWomanValue2;
	MarryAward* pAward = (MarryAward*)MarrySkillCfg::GetInstance().m_pAwardList->Items[byType-1];
	if(nTemp < pAward->nNeed)
	{
		pUser->SendWarnMsg("亲密度不足");
		return;
	}
	nFlag = nFlag | nCheck;
	pUser->m_pUserFlag->SetFlag(29,nFlag);
	int nLv = pUser->m_xUWealth[UW_LV];
	pUser->m_xUWealth.AddWealth(UW_EXP,nLv*nLv*pAward->nExp);
	pUser->m_xUWealth.AddWealth(UW_GOLD,nLv*pAward->nGold,1,NULL,UW_GOLD_MARRY);
	if(pUser->m_pMySimple == pData->m_pMan)
	{
		AddMarryValue(pUser,nID,pAward->nPoint,true);
		pUser->m_pUserMarry->SendDataChanged(pData);
		UserData* pT = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
		if(pT)
			pT->m_pUserMarry->SendDataChanged(pData);
	}
	else
	{
		AddMarryValue(pUser,nID,pAward->nPoint,false);
		pUser->m_pUserMarry->SendDataChanged(pData);
		UserData* pT = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
		if(pT)
			pT->m_pUserMarry->SendDataChanged(pData);
	}
}

// bool	MarryCenter::RequestYearlyAward(int nID)
// {
// 	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
// 	if(itor == m_mapMarry.end())
// 		return false;
// 	MarryData* pData = itor->second;
// 	int nCnt = CheckMarryTime(nID);
// 	if(nCnt == 0)
// 		return false;
// 	if(nCnt <= pData->m_byAwardLv)
// 		return false;
// 	int nExp = 200;
// 	int nMoney=1500;
// 	int nPoint=350;
// 	if(nCnt == 2)
// 	{
// 		nExp = 300;
// 		nMoney=2500;
// 		nPoint=450;
// 	}
// 	else if(nCnt == 3)
// 	{
// 		nExp = 500;
// 		nMoney=3500;
// 		nPoint=550;
// 	}
// 	else if(nCnt == 4)
// 	{
// 		nExp = 800;
// 		nMoney=4500;
// 		nPoint=750;
// 	}
// 	else if(nCnt >= 5)
// 	{
// 		nExp = 1500;
// 		nMoney=6000;
// 		nPoint=990;
// 	}
// 	pData->m_byAwardLv = nCnt;
// 	AddMarryValue(nID,nPoint);
// 	UserData* pUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
// 	if(pUser)
// 	{
// 		int nLv = pUser->m_xUWealth[UW_LV];
// 		pUser->m_xUWealth.AddWealth(UW_EXP,nLv*nLv*nExp);
// 		pUser->m_xUWealth.AddWealth(UW_GOLD,nLv*nMoney);
// 		pUser->m_pUserMarry->SendDataChanged(pData);
// 	}
// 	pUser = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
// 	if(pUser)
// 	{
// 		int nLv = pUser->m_xUWealth[UW_LV];
// 		pUser->m_xUWealth.AddWealth(UW_EXP,nLv*nLv*nExp);
// 		pUser->m_xUWealth.AddWealth(UW_GOLD,nLv*nMoney);
// 		pUser->m_pUserMarry->SendDataChanged(pData);
// 	}
// }

void	MarryCenter::CallStreetEnemy(MarryData* pData,UserData* pUser1,UserData* pUser2,int nCnt)
{
	CMTime tmNow = CMTime::GetCurTime();
	m_pStreet = new MarryStreetObj();
	m_pStreet->pFollowList = xnList::Create();
	m_pStreet->pData = pData;
	m_pStreet->nMapIndex = pUser1->m_nMIndex;
	m_pStreet->ptEnd.x = 4666;
	m_pStreet->ptEnd.y = 1115;
	m_pStreet->nMasterID = g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(1506,pUser1->m_nMIndex,4138,686,0);
	POINT szPoint[5];
	szPoint[0].x = 4666;szPoint[0].y = 1115;
	szPoint[1].x = 2000;szPoint[1].y = 2731;
	szPoint[2].x = 3283;szPoint[2].y = 3556;
	szPoint[3].x = 6619;szPoint[3].y = 1820;
	szPoint[4].x = 5121;szPoint[4].y = 986;
	pUser1->m_pMObjCenter->SetEnemyRunPath(m_pStreet->nMasterID,5,szPoint);
	int nCar = g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(1504,pUser1->m_nMIndex,4138,686,0);
	m_pStreet->pFollowList->Add((void*)nCar);
	pUser1->m_pMObjCenter->SetEnemyRunGroup(nCar,0,m_pStreet->nMasterID);
	for(int i = 0; i < nCnt; i ++)
	{
		int nID = g_pGameCenter->m_pCallEnemyCenter->CallEnemyByCfg(1505,pUser1->m_nMIndex,4138,686,0);
		m_pStreet->pFollowList->Add((void*)nID);
		pUser1->m_pMObjCenter->SetEnemyRunGroup(nID,6+i,m_pStreet->nMasterID);
	}
	int nDelta = 0;
	POINT tmpPt;
	tmpPt.x = 4138;tmpPt.y = 686;
	for(int i = 0;i < 5; i ++)
	{
		int nTX = szPoint[i].x-tmpPt.x;
		int nTY = szPoint[i].y-tmpPt.y;
		nDelta += sqrt((double)(nTX*nTX + nTY*nTY)/75/75);
		tmpPt.x = szPoint[i].x;
		tmpPt.y = szPoint[i].y;
	}
	int nTX = szPoint[0].x-tmpPt.x;
	int nTY = szPoint[0].y-tmpPt.y;
	nDelta += sqrt((double)(nTX*nTX + nTY*nTY)/75/75);
	m_pStreet->dwEndTick = tmNow.GetCurTickCount() + nDelta;
	pUser1->m_pMObjCenter->ChgWeather(11,nDelta);
	pUser1->m_pMObjCenter->SetMyCtrlLock(true,true,MAPOBJ_ENEMY,m_pStreet->nMasterID);
	pUser2->m_pMObjCenter->SetMyCtrlLock(true,true,MAPOBJ_ENEMY,m_pStreet->nMasterID);
}

void	MarryCenter::StreetMarry(UserData* pUser,int nID,BYTE byType)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	MarryData* pData = itor->second;
	UserData* pTUser = NULL;
	if(pUser->m_pMySimple != pData->m_pMan && pUser->m_pMySimple != pData->m_pWoman)
		return;
	if(pUser->m_pMySimple == pData->m_pMan)
	{
		pTUser = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
	}
	else
		pTUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
	if(pData->m_byState < 2)
	{
		pUser->SendWarnMsg("结婚后才能进行巡游");
		return;
	}
	if(m_pStreet)
	{
		pUser->SendWarnMsg("现在正有一对新人巡游，请稍候尝试");
		return;
	}
	switch(byType)
	{
	case 0:
		{
			if(pUser->m_xUWealth[UW_GOLD] < 100000)
			{
				pUser->SendWarnMsg("需100000金币");
				return;
			}
			if (pUser->m_pUserFlag->GetFlag(73) > 9 || pTUser->m_pUserFlag->GetFlag(73) > 9)
			{
				pUser->SendWarnMsg("该巡游每日只能进行10次！请明天再来吧！");
				return;
			}
			
			pUser->m_pUserFlag->AddFlag(73,1);
			pTUser->m_pUserFlag->AddFlag(73,1);

			pUser->m_xUWealth.SubWealth(UW_GOLD,100000);
			AddMarryValue(pUser,nID,10);
			CallStreetEnemy(pData,pUser,pTUser,0);
		}
		break;
	case 1:
		{
			if(pUser->m_xUWealth[UW_RMB] < 299)
			{
				pUser->SendWarnMsg("需299钻石");
				return;
			}
			pUser->m_xUWealth.SubWealth(UW_RMB,299,RMB_SAVE_MARRY);
			AddMarryValue(pUser,nID,250);
			CallStreetEnemy(pData,pUser,pTUser,2);
		}
		break;
	case 2:
		{
			if(pUser->m_xUWealth[UW_RMB] < 999)
			{
				pUser->SendWarnMsg("需999钻石");
				return;
			}
			pUser->m_xUWealth.SubWealth(UW_RMB,999,RMB_SAVE_MARRY);
			pData->m_byState = 3;
			AddMarryValue(pUser,nID,1000);
			pUser->m_pUserMarry->ResetObj();
			if(pTUser)
				pTUser->m_pUserMarry->ResetObj();
			CallStreetEnemy(pData,pUser,pTUser,6);
		}
		break;
	}
}

void	MarryCenter::SendRing(UserData* pUser,int nID,BYTE byType)
{
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	MarryData* pData = itor->second;
	if(pUser->m_pMySimple != pData->m_pMan && pUser->m_pMySimple != pData->m_pWoman)
		return;
	if(pData->m_byState == 0)
		return;
	UserData* pTarget = NULL;
	UserSimple* pTS = NULL;
	if(pUser->m_pMySimple == pData->m_pMan)
	{
		pTarget = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
		pTS = pData->m_pWoman;
	}
	else
	{
		pTarget = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
		pTS = pData->m_pMan;
	}
	char* szRingName[] = {"水晶","翡翠","钻石"};
	switch(byType)
	{
	case 0:
		{
			if(pUser->m_xUWealth[UW_GOLD] < 50000)
				return;
			if(pUser->GetFlag(30) >= 5)
				return;
			pUser->m_xUWealth.SubWealth(UW_GOLD,50000);
			AddMarryValue(pUser,nID,5);
			pUser->AddFlag(30,1);
		}
		break;
	case 1:
		{
			if(pUser->m_xUWealth[UW_RMB] < 52)
				return;
			pUser->m_xUWealth.SubWealth(UW_RMB,52,RMB_SAVE_MARRY);
			AddMarryValue(pUser,nID,50);
			char szMsg[128];
			sprintf(szMsg,"%s送了%s一枚%s戒指，祝他们的爱情甜甜蜜蜜。",pUser->m_szNick,pTS->szName,szRingName[byType]);
			ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
		}
		break;
	case 2:
		{
			if(pUser->m_xUWealth[UW_RMB] < 520)
				return;
			pUser->m_xUWealth.SubWealth(UW_RMB,520,RMB_SAVE_MARRY);
			AddMarryValue(pUser,nID,500);
			char szMsg[128];
			sprintf(szMsg,"丘比特之箭射出，%s送了%s一枚%s戒指，表达了对爱情的忠贞，祝他们爱情长长久久。",pUser->m_szNick,pTS->szName,szRingName[byType]);
			ChatCenter::GetInstance().SendSystemMsg(szMsg,0,0,1);
			pUser->m_pMObjCenter->ChgWeather(11,25);
		}
		break;
	}
	pUser->PlayEffect(nEffectSuccess);
	if(pTarget)
	{
		char szMsg[128];
		sprintf(szMsg,"%s向您送上一枚%s戒指，表达了对您的忠贞不渝!",pUser->m_szNick,szRingName[byType]);
		pTarget->PopMarryMessage("祝福",szMsg);
	}
	
}

void	MarryCenter::RequestUpSkill(UserData* pUser,int nID,BYTE byPos)
{
	if(byPos >= MarrySkillCfg::GetInstance().m_pList->Count)
		return;
	std::map<DWORD,MarryData*>::iterator itor = m_mapMarry.find(nID);
	if(itor == m_mapMarry.end())
		return;
	MarryData* pData = itor->second;
	MarrySKObj* pSkill = (MarrySKObj*)MarrySkillCfg::GetInstance().m_pList->Items[byPos];
	if(pData->m_pMan == pUser->m_pMySimple)
	{
		int nLv = pData->m_pManSkill->GetVal(byPos);
		if(nLv >= pSkill->pLvList->Count)
			return;
		_simpleAward* pS = (_simpleAward*)pSkill->pLvList->Items[nLv];
		if(pData->m_dwManValue < pS->nID)
		{
			pUser->SendWarnMsg("友好度不足");
			return;
		}
		pData->m_dwManValue -=pS->nID;
		nLv ++;
		pData->m_pManSkill->SetVal(byPos,nLv);
		_updateObj(pData);
		_updateSkill(pData,true);
		pUser->PlayEffect(nEffectSuccess);
		pUser->m_pUserMarry->SendDataChanged(pData);
		UserData* pSUser = g_pGameCenter->GetPlayer(pData->m_pWoman->dwUin);
		if(pSUser)
			pSUser->m_pUserMarry->SendDataChanged(pData);
	}
	else if(pData->m_pWoman == pUser->m_pMySimple)
	{
		int nLv = pData->m_pWomanSkill->GetVal(byPos);
		if(nLv >= pSkill->pLvList->Count)
			return;
		_simpleAward* pS = (_simpleAward*)pSkill->pLvList->Items[nLv];
		if(pData->m_dwWomanValue < pS->nID)
			return;
		pData->m_dwWomanValue -=pS->nID;
		nLv ++;
		pData->m_pWomanSkill->SetVal(byPos,nLv);
		_updateObj(pData);
		_updateSkill(pData,false);
		pUser->PlayEffect(nEffectSuccess);
		pUser->m_pUserMarry->SendDataChanged(pData);
		UserData* pSUser = g_pGameCenter->GetPlayer(pData->m_pMan->dwUin);
		if(pSUser)
			pSUser->m_pUserMarry->SendDataChanged(pData);
	}
	pUser->RecalcParam();
}

void	MarryCenter::RequestTotalInfo(UserData* pUser)
{
	if(pUser == NULL || pUser->m_pUserMarry == NULL)
		return;
	CMTime tmNow = CMTime::GetCurTime();
	GetRelationByuin(pUser->m_dwUin,pUser->m_pUserMarry->m_pRelationList);
	if(GetRequest(pUser->m_dwUin))
		pUser->m_pUserMarry->m_ifInstruct = true;

	pUser->m_pUserMarry->SendAll();
	if(m_pStreet && m_pStreet->pData == pUser->m_pUserMarry->GetRelation())
	{
		pUser->m_pMObjCenter->SetMyCtrlLock(true,true,MAPOBJ_ENEMY,m_pStreet->nMasterID);
	}
}

bool	MarryCenter::_insertObj(MarryData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_MARRY_TABLE" (id,muin,wuin,mvalue,wvalue,mpoint,wpoint,state,marrytime,awardlv,mskill,wskill) values (%d,%d,%d,%d,%d,%d,%d,%d,0,%d,'','')",
		pObj->m_dwID,pObj->m_pMan->dwUin,pObj->m_pWoman->dwUin,pObj->m_dwManValue,pObj->m_dwWomanValue,pObj->m_dwManValue2,pObj->m_dwWomanValue2,pObj->m_byState,pObj->m_byAwardLv);
	return true;
}

bool	MarryCenter::_updateObj(MarryData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"update "RPG_MARRY_TABLE" set mvalue=%d,wvalue=%d,mpoint=%d,wpoint=%d,awardlv=%d,state=%d where id=%d",pObj->m_dwManValue,pObj->m_dwWomanValue,pObj->m_dwManValue2,pObj->m_dwWomanValue2,pObj->m_byAwardLv,pObj->m_byState,pObj->m_dwID);
	return true;
}

bool	MarryCenter::_updateSkill(MarryData* pObj,bool ifMan)
{
	int nSize = 0;
	BYTE byData[1024] = {0};
	if(ifMan)
	{
		pObj->m_pManSkill->GetData(byData,nSize);
		m_pDB->SetBLOB(FALSE,byData,nSize,"update "RPG_MARRY_TABLE" set mskill=? where id =%d",pObj->m_dwID);
		return true;
	}
	else
	{
		pObj->m_pWomanSkill->GetData(byData,nSize);
		m_pDB->SetBLOB(FALSE,byData,nSize,"update "RPG_MARRY_TABLE" set wskill=? where id =%d",pObj->m_dwID);
		return true;
	}
	return false;
}

bool	MarryCenter::_deleteObj(MarryData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_MARRY_TABLE" where id = %d",pObj->m_dwID);
	return true;
}

bool	MarryCenter::_insertRequest(MarryRequestData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"insert into "RPG_MARRYREQUEST_TABLE" (uin,value,type,endtick) values (%d,%d,%d,%d)",pObj->pUser->dwUin,pObj->nVal,pObj->byType,pObj->dwEndTick);
	return true;
}

bool	MarryCenter::_deleteRequest(MarryRequestData* pObj)
{
	m_pDB->ExecuteSQL(FALSE,"delete from "RPG_MARRYREQUEST_TABLE" where uin = %d and value = %d",pObj->pUser->dwUin,pObj->nVal);
	return true;
}


UserMarry::UserMarry(UserData* pUser)
{
	m_pUser = pUser;
	m_pRelationList = xnList::Create();
	m_ifInstruct = false;
	m_SendInstruct = false;
	m_ifShow = false;
	m_ifSendShow = false;
}

UserMarry::~UserMarry()
{
	if(m_pRelationList)
	{
		while(m_pRelationList->Count)
			m_pRelationList->Delete(0);
		m_pRelationList->Free();
	}
}

void UserMarry::AddData(MarryData* pData)
{
	m_pRelationList->Add(pData);
	SendDataAdd(pData);
}

void UserMarry::DeleteData(int i)
{
	if(i >= 0 && i < m_pRelationList->Count)
	{
		MarryData* pData = (MarryData*)m_pRelationList->Items[i];
		m_pRelationList->Delete(i);
		SendDataDelete(pData);
	}
}

bool	UserMarry::MarryExpAdd()
{
	MarryData* pMarry = GetRelation();
	if (!pMarry)
		return false;

	UserData* pMan = g_pGameCenter->GetPlayer(pMarry->m_pMan->dwUin);
	UserData* pWoman = g_pGameCenter->GetPlayer(pMarry->m_pWoman->dwUin);
	if (!pMan || !pWoman)
		return false;

	if (pMan->m_nMIndex == pWoman->m_nMIndex)
		return true;

	return false;
}

MarryData*	UserMarry::GetRelation()
{
	for(int i = 0; i < m_pRelationList->Count; i ++)
	{
		MarryData* pData = (MarryData*)m_pRelationList->Items[i];
		if(pData->m_byState != 0)
			return pData;
	}
	return NULL;
}

void	UserMarry::Update()
{
	bool ifChg = false;
	for(int i = m_pRelationList->Count-1; i >= 0; i --)
	{
		MarryData* pData = (MarryData*)m_pRelationList->Items[i];
		if(pData->m_byState == 0)
		{
			m_pRelationList->Delete(i);
			SendDataDelete(pData);
			ifChg = true;
		}
	}
	if(ifChg)
		m_pUser->RecalcParam();

	if(m_SendInstruct != m_ifInstruct)
		SendInstruct();
	if(m_ifSendShow != m_ifShow)
		SendShow();
}

void UserMarry::SendAll()
{
	SendInstruct();
	if(m_pRelationList->Count > 0)
	{
		BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_ALL,buf,1024);
		SET_DATA(pSendData,RPGRELATION,S_SEND_MARRY_ALL,pPacket);
		int n = 0;
		pSendData->nCnt = 0;
		for(int i = 0; i < m_pRelationList->Count; i ++)
		{
			MarryData* pTemp = (MarryData*)m_pRelationList->Items[i];
			pSendData->szInfo[n].dwID = pTemp->m_dwID;
			pSendData->szInfo[n].byState = pTemp->m_byState;
			pSendData->szInfo[n].dwMan = pTemp->m_pMan->dwUin;
			pSendData->szInfo[n].bMJob = pTemp->m_pMan->bJob;
			pSendData->szInfo[n].bMLvl = pTemp->m_pMan->bLv;
			pSendData->szInfo[n].nMValue = pTemp->m_dwManValue;
			pSendData->szInfo[n].nMpoint = pTemp->m_dwManValue2;
			for(int i = 0; i < 5; i ++)
			{
				pSendData->szInfo[n].szMSkill[i] = pTemp->m_pManSkill->GetVal(i);
			}
			strcpy(pSendData->szInfo[n].szMName,pTemp->m_pMan->szName);
			strcpy(pSendData->szInfo[n].szMHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pTemp->m_pMan->wHomeID));
			pSendData->szInfo[n].dwWoman = pTemp->m_pWoman->dwUin;
			pSendData->szInfo[n].bWJob = pTemp->m_pWoman->bJob;
			pSendData->szInfo[n].bWLvl = pTemp->m_pWoman->bLv;
			pSendData->szInfo[n].nWValue = pTemp->m_dwWomanValue;
			pSendData->szInfo[n].nWpoint = pTemp->m_dwWomanValue2;
			for(int i = 0; i < 5; i ++)
			{
				pSendData->szInfo[n].szWSkill[i] = pTemp->m_pWomanSkill->GetVal(i);
			}
			strcpy(pSendData->szInfo[n].szWName,pTemp->m_pWoman->szName);
			strcpy(pSendData->szInfo[n].szWHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pTemp->m_pWoman->wHomeID));
			n++;
			if(n >= 10)
			{
				pSendData->nCnt = n;
				pPacket->wSize += n*sizeof(ClientMarryData);
				TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
				pPacket->wSize	= sizeof(RPGRELATION_S_SEND_MARRY_ALL_Data);
				n = 0;
			}
		}
		if(n > 0)
		{
			pSendData->nCnt = n;
			pPacket->wSize += n*sizeof(ClientMarryData);
			TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
		}
	}
}

void	UserMarry::SendInstruct()
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_INSTRUCT,buf,1024);
	SET_DATA(pData,RPGRELATION,S_SEND_MARRY_INSTRUCT,pPacket);
	pData->ifInstruct = m_ifInstruct;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	m_SendInstruct = m_ifInstruct;
}

void	UserMarry::SendShow()
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_SHOW_TITLE,buf,1024);
	SET_DATA(pData,RPGRELATION,S_SEND_MARRY_SHOW_TITLE,pPacket);
	pData->ifShow = m_ifShow;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
	m_ifSendShow = m_ifShow;
}

void	UserMarry::SendDataAdd(MarryData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_DATA_ADD,buf,1024);
	SET_DATA(pSend,RPGRELATION,S_SEND_MARRY_DATA_ADD,pPacket);
	pSend->xInfo.dwID	= pData->m_dwID;
	pSend->xInfo.byState= pData->m_byState;
	pSend->xInfo.dwMan	= pData->m_pMan->dwUin;
	pSend->xInfo.bMJob	= pData->m_pMan->bJob;
	pSend->xInfo.bMLvl	= pData->m_pMan->bLv;
	pSend->xInfo.nMValue= pData->m_dwManValue;
	pSend->xInfo.nMpoint= pData->m_dwManValue2;
	strcpy(pSend->xInfo.szMName,pData->m_pMan->szName);
	strcpy(pSend->xInfo.szMHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pData->m_pMan->wHomeID));
	for(int i = 0; i < 5; i ++)
	{
		pSend->xInfo.szMSkill[i] = pData->m_pManSkill->GetVal(i);
	}
	pSend->xInfo.dwWoman= pData->m_pWoman->dwUin;
	pSend->xInfo.bWJob	= pData->m_pWoman->bJob;
	pSend->xInfo.bWLvl	= pData->m_pWoman->bLv;
	pSend->xInfo.nWValue= pData->m_dwWomanValue;
	pSend->xInfo.nWpoint= pData->m_dwWomanValue2;
	strcpy(pSend->xInfo.szWName,pData->m_pWoman->szName);
	strcpy(pSend->xInfo.szWHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pData->m_pWoman->wHomeID));
	for(int i = 0; i < 5; i ++)
	{
		pSend->xInfo.szWSkill[i] = pData->m_pWomanSkill->GetVal(i);
	}
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void	UserMarry::SendDataChanged(MarryData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_DATA_CHG,buf,1024);
	SET_DATA(pSend,RPGRELATION,S_SEND_MARRY_DATA_CHG,pPacket);
	pSend->xInfo.dwID	= pData->m_dwID;
	pSend->xInfo.byState= pData->m_byState;
	pSend->xInfo.dwMan	= pData->m_pMan->dwUin;
	pSend->xInfo.bMJob	= pData->m_pMan->bJob;
	pSend->xInfo.bMLvl	= pData->m_pMan->bLv;
	pSend->xInfo.nMValue= pData->m_dwManValue;
	pSend->xInfo.nMpoint= pData->m_dwManValue2;
	strcpy(pSend->xInfo.szMName,pData->m_pMan->szName);
	strcpy(pSend->xInfo.szMHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pData->m_pMan->wHomeID));
	for(int i = 0; i < 5; i ++)
	{
		pSend->xInfo.szMSkill[i] = pData->m_pManSkill->GetVal(i);
	}
	pSend->xInfo.dwWoman= pData->m_pWoman->dwUin;
	pSend->xInfo.bWJob	= pData->m_pWoman->bJob;
	pSend->xInfo.bWLvl	= pData->m_pWoman->bLv;
	pSend->xInfo.nWValue= pData->m_dwWomanValue;
	pSend->xInfo.nWpoint= pData->m_dwWomanValue2;
	strcpy(pSend->xInfo.szWName,pData->m_pWoman->szName);
	strcpy(pSend->xInfo.szWHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pData->m_pWoman->wHomeID));
	for(int i = 0; i < 5; i ++)
	{
		pSend->xInfo.szWSkill[i] = pData->m_pWomanSkill->GetVal(i);
	}
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void	UserMarry::SendDataDelete(MarryData* pData)
{
	BuildPacketEx(pPacket,RPGRELATION,S_SEND_MARRY_DATA_DEL,buf,1024);
	SET_DATA(pSendData,RPGRELATION,S_SEND_MARRY_DATA_DEL,pPacket);
	pSendData->nID = pData->m_dwID;
	TCP_SendPacketToUin(m_pUser->m_dwUin,pPacket);
}

void	UserMarry::ResetObj()
{
	MarryData* pData = GetRelation();
	if(pData && m_ifShow)
	{
		char szTitle[32] = {0};
		if(m_pUser->m_bySex == 1)
		{
			char* szTempTitle[] = {"","未婚夫","丈夫","模范丈夫"};
			sprintf(szTitle,"%s的%s",pData->m_pWoman->szName,szTempTitle[pData->m_byState]);
		}
		else
		{
			char* szTempTitle[] = {"","未婚妻","妻子","模范妻子"};
			sprintf(szTitle,"%s的%s",pData->m_pMan->szName,szTempTitle[pData->m_byState]);
		}
		m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryTitle(szTitle);
	}
	else
	{
		m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryTitle("");
	}
}

void	UserMarry::ResetEff()
{
	MarryData* pData = GetRelation();
	if(pData && pData->m_byState >= 2)
	{
		if(m_pUser->m_bySex == 1)
		{
			if(m_pUser->m_pItemCenter->GetItemCountByID(MARRY_ITEM_EFF))
				m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryEff(pData->m_pWoman->dwUin);
			else
				m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryEff(0);
		}
		else
		{
			if(m_pUser->m_pItemCenter->GetItemCountByID(MARRY_ITEM_EFF))
				m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryEff(pData->m_pMan->dwUin);
			else
				m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryEff(0);
		}
	}
	else
	{
		m_pUser->m_pMObjCenter->m_pPlayer->ChangeMarryEff(0);
	}
}