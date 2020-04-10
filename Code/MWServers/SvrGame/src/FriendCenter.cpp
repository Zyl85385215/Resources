#include "stdafx.h"
#include "FriendCenter.h"
#include "UserData.h"
#include "ios_muli_packet.h"
#include "FsGameCenter.h"
#include "UserList.h"
#include "P2PAskCenter.h"
#include "FamilyCenter.h"
#include "FortCenter.h"
#include "FsGameCenter.h"
#include "WarRank.h"
#include "UserFlag.h"
extern FsGameCenter* g_pGameCenter;

FriendList::FriendList( UserData* pU )
{
	m_pParent		= pU;
	m_pList		= xnList::Create();
}

FriendList::~FriendList()
{
	while(m_pList->Count)
	{
		delete (FriendObj*)m_pList->Delete(0);
	}
	m_pList->Free();
}

void FriendList::DBInit()
{
	QueryRlt xRlt;
	if(!m_pParent->m_pDB->pDBServer->Query(&xRlt,"SELECT * FROM "RPG_FRIEND_TABLE" where uinA=%d or uinB=%d",m_pParent->m_dwUin, m_pParent->m_dwUin))
		return;

	MySqlQuery* pCmd = xRlt.GetRlt();
	if (pCmd == NULL)
		return;

	while(pCmd->FetchRow())
	{
		FriendObj*		pFObj	= new FriendObj();
		pFObj->dwUinB		= pCmd->GetInt("uinA");

		m_mapState[pFObj->dwUinB] = CheckFriendOnline(pFObj->dwUinB);
		
		if(pFObj->dwUinB == m_pParent->m_dwUin)
		{
			pFObj->dwUinB	= pCmd->GetInt("uinB");
			pFObj->byRsType			= pCmd->GetInt("rstype");
			pFObj->nRsVal			= pCmd->GetInt("rsvalA");
		}
		else{
			pFObj->byRsType			= pCmd->GetInt("rstype");
			if (pFObj->byRsType == RS_TYPE_PRE_FRIEND)
			{
				pFObj->byRsType = RS_TYPE_PRE_FRIENDEX;
			}
			pFObj->nRsVal			= pCmd->GetInt("rsvalA");
		}


		UserSimple* pSimp	= g_pUserList->GetUser(pFObj->dwUinB);
		if(pSimp)
		{
			pFObj->byLv		= pSimp->bLv;
			pFObj->byJob	= pSimp->bJob;
			pFObj->byCountry = pSimp->bCountry;
			pFObj->byVip	= pSimp->bVipLv;
			strcpy(pFObj->szHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pSimp->wHomeID));
			strcpy(pFObj->szName,pSimp->szName);
			m_pList->Add(pFObj);
		}
		else
			delete pFObj;
		
	}
}

void FriendList::_insertDB( FriendObj* pFObj )
{
	m_pParent->m_pDB->ExecuteSQL(FALSE,"insert into "RPG_FRIEND_TABLE" (uinA, uinB, rstype, rsvalA, rsvalB) values (%d, %d, %d,%d,%d)", 
		m_pParent->m_dwUin,pFObj->dwUinB,pFObj->byRsType,0,0);
}

void FriendList::_deleteDB( FriendObj* pFObj )
{
	m_pParent->m_pDB->ExecuteSQL(FALSE,"delete from "RPG_FRIEND_TABLE" where (uinA=%d and uinB=%d) or (uinA=%d and uinB=%d)",
		m_pParent->m_dwUin,pFObj->dwUinB,pFObj->dwUinB,m_pParent->m_dwUin);
}

void FriendList::_updateDB( FriendObj* pFObj )
{
	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_FRIEND_TABLE" set rstype=%d,rsvalA=%d where uinA=%d and uinB=%d",
		pFObj->byRsType,pFObj->nRsVal,m_pParent->m_dwUin,pFObj->dwUinB);

	m_pParent->m_pDB->ExecuteSQL(FALSE,"update "RPG_FRIEND_TABLE" set rstype=%d,rsvalA=%d where uinA=%d and uinB=%d",
		pFObj->byRsType,pFObj->nRsVal,pFObj->dwUinB,m_pParent->m_dwUin);
}

FriendObj* FriendList::AddFriend( DWORD dwUinOt,char*	szName,BYTE bLv,BYTE byJob,BYTE byCountry,char* szHomeName, BYTE byRsT , bool bSaveDb)
{
	FriendObj*	pFObj	= _getFriend(dwUinOt);
	if(pFObj)
	{
		m_pParent->SendWarnMsg("该好友已存在!");
		return NULL;
	}

	pFObj	= new FriendObj();
	pFObj->dwUinB		= dwUinOt;
	pFObj->byRsType		= byRsT;
	pFObj->nRsVal		= 0;
	pFObj->byLv			= bLv;
	pFObj->byJob		= byJob;
	pFObj->byCountry	= byCountry;
	strcpy(pFObj->szHomeName,szHomeName);
	strcpy(pFObj->szName,szName);
	m_pList->Add(pFObj);
	if(bSaveDb)
		_insertDB(pFObj);

	BuildPacketEx(pPacket,FRIEND, S_FRIEND_ADD,buf, 256);
	SET_DATA(pSend,FRIEND, S_FRIEND_ADD,pPacket);
	memcpy(&pSend->frdObj,pFObj,sizeof(FriendObj));
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	return pFObj;
}

void	FriendList::AddFriendRequest(UserData* pUser)
{
	if(pUser == NULL)
		return;
	if(pUser->m_pFriendList->m_pList->Count >= 20+pUser->m_xUWealth[UW_LV]/5)
	{
		pUser->SendWarnMsg("已达到好友人数上限!");
		return;
	}
	if(m_pList->Count >= 20+m_pParent->m_xUWealth[UW_LV]/5)
	{
		pUser->SendWarnMsg("对方已达到好友人数上限!");
		return;
	}
	if(GetFriend(pUser->m_dwUin))
		return;
	BuildPacketEx(pPacket,FRIEND,S_SEND_REQUESTLIST,buf,1024);
	SET_DATA(pSend,FRIEND,S_SEND_REQUESTLIST,pPacket);
	strcpy(pSend->xInfo.szName, pUser->m_szNick);
	pSend->xInfo.dwUin = pUser->m_dwUin;
	pSend->xInfo.byCountry = pUser->m_byCountry;
	pSend->xInfo.byJob = pUser->m_byJob;
	pSend->xInfo.byLv = pUser->m_xUWealth[UW_LV];
	strcpy(pSend->xInfo.szHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->m_dwHomeID));
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	pUser->SendWarnMsg("已向对方发送好友请求");
}

void	FriendList::DealwithRequest(int nID,bool ifOK)
{
	if(GetFriend(nID))
		return;
	UserData* pTargent = g_pGameCenter->GetPlayer(nID);
	if(pTargent)
	{
		if(pTargent->m_pFriendList->m_pList->Count >= 20+pTargent->m_xUWealth[UW_LV]/5)
		{
			m_pParent->SendWarnMsg("对方已达到好友人数上限!");
			return;
		}
		if(ifOK)
		{
			AddFriend(pTargent->m_dwUin,pTargent->m_szNick,pTargent->m_xUWealth.GetWealth(UW_LV),pTargent->m_byJob,pTargent->m_byCountry,g_pGameCenter->m_pHomeCenter->GetHomeName(pTargent->m_dwHomeID),RS_TYPE_FRIEND,true);
			pTargent->m_pFriendList->AddFriend(m_pParent->m_dwUin,m_pParent->m_szNick,m_pParent->m_xUWealth.GetWealth(UW_LV),m_pParent->m_byJob,m_pParent->m_byCountry,g_pGameCenter->m_pHomeCenter->GetHomeName(m_pParent->m_dwHomeID),RS_TYPE_FRIEND,false);

		}
		else
		{
			pTargent->SendWarnMsg("%s拒绝了你的好友申请",m_pParent->m_szNick);
		}
	}
	else
	{
		m_pParent->SendWarnMsg("对方已下线");
	}
}

void	FriendList::GetSuitableList(int nLv)
{
	xnList* pTempList = xnList::Create();
	g_pGameCenter->GetPlayerByLv(m_pParent->m_dwUin,nLv-10,nLv+10,pTempList);
// 	if(pTempList->Count < 6)
// 	{
// 		int nCount = 6-pTempList->Count;
// 		g_pGameCenter->m_pRankCenter->GetOnlineUser(nLv+10,pTempList,nCount);
// 	}
	g_pGameCenter->m_pRankCenter->GetOnlineUser(nLv+10,pTempList,20);
	for(int i = pTempList->Count-1; i >= 0; i --)
	{
		UserData* pUser = (UserData*)pTempList->Items[i];
		if(GetFriend(pUser->m_dwUin))
			pTempList->Delete(i);
	}
	if(pTempList->Count > 0)
	{
		BuildPacketEx(pPacket,FRIEND,S_SEND_SUITABLE,buf,1024);
		SET_DATA(pSend,FRIEND,S_SEND_SUITABLE,pPacket);
		memset(pSend->szInfo,0,sizeof(SuitFriend)*6);
		for(int i = 0; i < pTempList->Count && i < 6; i ++)
		{
			UserData* pUser = (UserData*)pTempList->Items[i];
			pSend->szInfo[i].dwUin = pUser->m_dwUin;
			strcpy(pSend->szInfo[i].szName,pUser->m_szNick);
			pSend->szInfo[i].byJob = pUser->m_byJob;
			pSend->szInfo[i].byLv = pUser->m_xUWealth[UW_LV];
			pSend->szInfo[i].byCountry = pUser->m_byCountry;
			strcpy(pSend->szInfo[i].szHomeName,g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->m_dwHomeID));
		}
		TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
	}
	pTempList->Free();
}

void FriendList::DelFriend( DWORD dwUinOt , bool bSaveDb)
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		FriendObj* pFObj	= (FriendObj*)m_pList->Items[i];
		if(pFObj->dwUinB	== dwUinOt)
		{
			if(bSaveDb)
				_deleteDB(pFObj);

			BuildPacketEx(pPacket,FRIEND, S_FRIEND_DEL,buf, 256);
			SET_DATA(pSend,FRIEND, S_FRIEND_DEL,pPacket);
			pSend->dwUin = dwUinOt;
			TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
			delete pFObj;
			m_pList->Delete(i);
			return;
		}
	}
}

FriendObj*	FriendList::GetFriend(DWORD dwUin)
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		FriendObj* pFObj	= (FriendObj*)m_pList->Items[i];
		if(pFObj->dwUinB	== dwUin)
		{
			return pFObj;
		}
	}
	return NULL;
}

void FriendList::SetFriendVal( DWORD dwUinOt,  int nVal , bool bSaveDb)
{
	FriendObj*	pFObj	= _getFriend(dwUinOt);
	if(pFObj == NULL)
		return;

	pFObj->nRsVal	= nVal;
	if(bSaveDb)
		_updateDB(pFObj);
	SendFriend(pFObj);
}

FriendObj* FriendList::_getFriend( DWORD dwUinOt )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		FriendObj* pFObj	= (FriendObj*)m_pList->Items[i];
		if(pFObj->dwUinB	== dwUinOt)
			return pFObj;
	}
	return NULL;
}

bool FriendList::RecvPacket( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup	!= FRIEND)
		return false;

	switch(pPacket->bCmd)
	{
	case C_P2PASK:
		{
			SET_DATA(pRecv, FRIEND,C_P2PASK, pPacket);
			DWORD dwUin = atoi(pRecv->szName);
			UserSimple*	pUser	= dwUin?g_pUserList->GetUser(dwUin):g_pUserList->GetUser(pRecv->szName);
			switch(pRecv->bType)
			{//根据类型判断条件
			case PAT_FRIEND:
				{
					if(m_pList->Count >= 20+m_pParent->m_xUWealth[UW_LV]/5)
					{
						m_pParent->SendWarnMsg("已达到好友人数上限!");
						return true;		//超出数量
					}
					if(pUser && _getFriend(pUser->dwUin))	
					{
						m_pParent->SendWarnMsg("已存在!");
						return true;		//已有好友
					}
					if (pUser)
					{
						UserData* pUserData = g_pGameCenter->GetPlayer(pUser->dwUin);
						if (pUserData && pUserData->m_pFriendList->m_pList->Count >= 20+pUserData->m_xUWealth[UW_LV]/5)
						{
							m_pParent->SendWarnMsg("对方已达到好友人数上限!");
							return true;		//超出数量
						}
					}
					if (pUser)
					{
						m_pParent->SendWarnMsg("已发送邀请！");
					}else
						m_pParent->SendWarnMsg("已发送邀请！");
				}
				break;
			}
			
			if (pUser)
			{
				AddFriend(pUser->dwUin,pUser->szName,pUser->bLv,pUser->bJob,pUser->bCountry,g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->wHomeID),RS_TYPE_PRE_FRIEND,true);
			}else{
				return true;
			}
			UserData*	pUserdata	= dwUin?g_pGameCenter->GetPlayer(dwUin):g_pGameCenter->GetPlayerByname(pRecv->szName);
			if(pUserdata) pUserdata->m_pFriendList->AddFriend(m_pParent->m_dwUin,m_pParent->m_szNick,m_pParent->m_xUWealth.GetWealth(UW_LV),m_pParent->m_byJob,m_pParent->m_byCountry,g_pGameCenter->m_pHomeCenter->GetHomeName(m_pParent->m_dwHomeID),RS_TYPE_PRE_FRIENDEX,false);
			//P2pAskCenter::GetInstance().AddP2pAskObj(m_pParent->m_dwUin,pRecv->dwObj,pRecv->bType,m_pParent->m_szNick);

			//BuildPacketEx(pSendPacket, FRIEND,S_ASK_FRIEND,buf, 256);
			//SET_DATA(pSend, FRIEND,S_ASK_FRIEND,pSendPacket);
			//pSend->frd.dwUinB	= m_pParent->m_dwUin;
			//strcpy(pSend->frd.szName,m_pParent->m_szNick);
			//pSend->frd.byLv		= m_pParent->m_xUWealth[UW_LV];
			//TCP_SendPacketToUin(pRecv->dwUin,pSendPacket);
		}
		return true;
	case C_REQUEST_SUITABLE:
		{
			GetSuitableList(m_pParent->m_xUWealth[UW_LV]);
		}
		return true;
	case C_ASK_ADDFRIEND:
		{
			SET_DATA(pRecv,FRIEND,C_ASK_ADDFRIEND,pPacket);
			UserData* pUser = g_pGameCenter->GetPlayer(pRecv->nId);
			if(pUser)
				pUser->m_pFriendList->AddFriendRequest(m_pParent);
		}
		return true;
	case C_DEALWHIT_SUITABLE:
		{
			SET_DATA(pRecv,FRIEND,C_DEALWHIT_SUITABLE,pPacket);
			DealwithRequest(pRecv->nId,pRecv->ifOk);
		}
		return true;
	case C_P2PANSWER:
		{
			SET_DATA(pRecv, FRIEND,C_P2PANSWER, pPacket);
			UserSimple* pUser = g_pUserList->GetUser(pRecv->dwAsker);
			if (pRecv->bType == PAT_FRIEND)
			{
				UserData* pUserData = g_pGameCenter->GetPlayer(pRecv->dwAsker);
				if(pRecv->bOk == 1)
				{//同意
					//多线程下有问题
					if(pUserData&&pUserData->m_pFriendList->m_pList->Count >= 20+pUserData->m_xUWealth[UW_LV]/5)
					{
						DelFriend(pRecv->dwAsker,true);
						pUserData->m_pFriendList->DelFriend(m_pParent->m_dwUin,false);
						m_pParent->SendWarnMsg("对方已达到好友人数上限!");
						return true;
					}
					if(m_pList->Count >= 20+m_pParent->m_xUWealth[UW_LV]/5)
					{
						DelFriend(pRecv->dwAsker,true);
						if(pUserData) pUserData->m_pFriendList->DelFriend(m_pParent->m_dwUin,false);
						m_pParent->SendWarnMsg("已达到好友人数上限!");
						return true;
					}
					ChgFriendRsT(pRecv->dwAsker,RS_TYPE_FRIEND,true);
					if (pUserData)
					{
						pUserData->m_pFriendList->ChgFriendRsT(m_pParent->m_dwUin,RS_TYPE_FRIEND,false);
						pUserData->SendWarnMsg("成功添加好友");
					}
					//char* szHomeName = g_pGameCenter->m_pHomeCenter->GetHomeName(m_pParent->m_dwHomeID);
					//szHomeName = g_pGameCenter->m_pHomeCenter->GetHomeName(pUser->m_dwHomeID);
					//AddFriend(pPA->dwSend,pUser->m_szNick,pUser->m_xUWealth[UW_LV],pUser->m_byJob,pUser->m_byCountry,szHomeName,1,false);
				}
				else
				{
					DelFriend(pRecv->dwAsker,true);
					if(pUserData){
						pUserData->m_pFriendList->DelFriend(m_pParent->m_dwUin,false);
						pUserData->SendWarnMsg("对方拒绝了你的请求");
					}

				}
			}
			//P2pAskObj* pPA	= P2pAskCenter::GetInstance().GetMyPAIndex(pRecv->nIndex);
			//if(pPA)
// 			{
// 				UserData*	pUser	= g_pGameCenter->GetPlayer(pPA->dwSend);
// 				if(pUser)
// 				{
// 					switch(pPA->bType)
// 					{
// 					case PAT_FRIEND:
// 						{
// 							
// 							
// 						}
// 						break;
// 					}
// 				}
// 				pPA->bStep	= PAS_END;//PAS_ANSWER;
// 			}
		}
		return true;
	case C_SEARTH_FRIEND:
		{
			xnList*	pList	= xnList::Create();
			g_pGameCenter->RandGetPlayerByLv(m_pParent->m_dwUin,m_pParent->m_xUWealth[UW_LV]-50,m_pParent->m_xUWealth[UW_LV]+50,pList,10);
			if(pList->Count == 0)
				return true;
			BuildPacketEx(pSendPacket,FRIEND,S_SEARTH_LIST,buf, 2048);
			SET_DATA(pSend, FRIEND, S_SEARTH_LIST, pSendPacket);
			pSend->wCnt		= pList->Count;
			pSendPacket->wSize	+= pSend->wCnt*sizeof(P2PUserBase);
			for (int i = 0; i < pList->Count; i++)
			{
				UserData* pUser	= (UserData*)pList->Items[i];
				pSend->frdBase[i].dwUinB	= pUser->m_dwUin;
				strcpy(pSend->frdBase[i].szName,pUser->m_szNick);
				pSend->frdBase[i].byLv		= pUser->m_xUWealth[UW_LV];
			}
			TCP_SendPacketToUin(m_pParent->m_dwUin,pSendPacket);

			pList->Free();
		}
		return true;
	case C_FIGHT_RPOS:
		{
			SET_DATA(pData,FRIEND,C_FIGHT_RPOS,pPacket);
			//g_pGameCenter->m_pWarRankCenter->RequestFight(m_pParent,pData->wPos);
		}
		return true;
	case C_RQWRANK_INFO:
		{
			//g_pGameCenter->m_pWarRankCenter->RequestWRObjs(m_pParent);
		}
		return true;
	case C_DEL_FRIEND:
		{
			SET_DATA(pData,FRIEND,C_DEL_FRIEND,pPacket);
			FriendObj* pObj = _getFriend(pData->dwUin);
			if (pObj&&pObj->byRsType == RS_TYPE_FRIEND)
			{
				DelFriend(pData->dwUin,true);
				UserData* pUserData = g_pGameCenter->GetPlayer(pData->dwUin);
				if(pUserData){
					pUserData->m_pFriendList->DelFriend(m_pParent->m_dwUin,false);
				}
			}
			
		}
		return true;
	case C_SEND_GIFT:
		{
			SET_DATA(pData,FRIEND,C_SEND_GIFT,pPacket);
			FriendObj* pObj = _getFriend(pData->dwUin);
			if (pObj&&pObj->byRsType == RS_TYPE_FRIEND)
			{
				int nCount = m_pParent->m_pUserFlag->GetFlag(59);
				if (nCount>=20)
				{
					m_pParent->SendWarnMsg("每天最多赠送20次");
					return true;
				}
				bool bGive = GetRsValByType(pData->dwUin,FRIEND_RS_VAL_GIVE,true);
				if (bGive)
				{
					m_pParent->SendWarnMsg("已经赠送过该好友");
					return true;
				}
				UserData* pUserData = g_pGameCenter->GetPlayer(pData->dwUin);
				SetRsValByType(pData->dwUin,FRIEND_RS_VAL_GIVE,true,true);
				m_pParent->SendWarnMsg("成功赠送对方1点体力");

				m_pParent->m_pUserFlag->AddFlag(59,1);

				if(pUserData){
					pUserData->m_pFriendList->SetRsValByType(m_pParent->m_dwUin,FRIEND_RS_VAL_GIVE,true,false,false);
				}
			}

		}
		return true;
	case C_GET_GIFT:
		{
			SET_DATA(pData,FRIEND,C_GET_GIFT,pPacket);
			FriendObj* pObj = _getFriend(pData->dwUin);
			if (pObj&&pObj->byRsType == RS_TYPE_FRIEND)
			{
				int nCount = m_pParent->m_pUserFlag->GetFlag(60);
				if (nCount>=20)
				{
					m_pParent->SendWarnMsg("每天最多领取20次");
					return true;
				}
				bool bGive = GetRsValByType(pData->dwUin,FRIEND_RS_VAL_GIVE,false);
				if (!bGive)
				{
					//m_pParent->SendPopMsg("已经赠送过该好友");
					return true;
				}
				bool bGet = GetRsValByType(pData->dwUin,FRIEND_RS_VAL_GET,true);
				if (bGet)
				{
					m_pParent->SendWarnMsg("已经领取过该好友赠送的体力");
					return true;
				}
				if (m_pParent->GetWealth(UW_POW) >= 50)
				{
					m_pParent->SendWarnMsg("体力已满，无法领取");
					return true;
				}
				//UserData* pUserData = g_pGameCenter->GetPlayer(pData->dwUin);
				SetRsValByType(pData->dwUin,FRIEND_RS_VAL_GET,true,true,true);
				m_pParent->AddWealth(UW_POW,1);
				m_pParent->SendWarnMsg("成功领取1点体力");

				m_pParent->m_pUserFlag->AddFlag(60,1);

				if (!GetRsValByType(pData->dwUin,FRIEND_RS_VAL_GIVE,true))
					m_pParent->SendPopMsg("领取成功，快去回赠好友吧！");
			}

		}
		return true;
	case C_SEED_MANOR:
		{
			SET_DATA(pData,FRIEND,C_SEED_MANOR,pPacket);
			ItemObj* pObj = m_pParent->m_pItemCenter->GetItemByIndex(pData->dwIndex);
			FortData* pFort = m_pParent->GetFortData();
			if (pFort&&pObj)
			{
				pFort->SeedManorPos(pData->byPos,pObj);
			}

		}
		return true;
	case C_GET_MANOR_REWARD:
		{
			SET_DATA(pData,FRIEND,C_GET_MANOR_REWARD,pPacket);
			FortData* pFort = m_pParent->GetFortData();
			if (pFort)
			{
				pFort->GetManorReward(pData->byPos,m_pParent);
			}

		}
		return true;
	case C_GET_MANOR_REWARDEX:
		{
			SET_DATA(pData,FRIEND,C_GET_MANOR_REWARDEX,pPacket);
			FortData* pFort = g_pGameCenter->m_pFortCenter->GetFortData(pData->dwUin,false);
			if (pFort)
			{
				pFort->GetManorReward(pData->byPos,m_pParent);
			}

		}
		return true;
	case C_GET_ALL_MANOR_REWARD:
		{
			FortData* pFort = m_pParent->GetFortData();
			if (pFort)
			{
				bool bTip = true;
				bool bTip2 = true;
				for (int i=0;i<nMaxManorCount;i++)
				{
					if(pFort->GetManorReward(i,m_pParent)) bTip = false;
					if(pFort->m_pManorData->dwID[i]) bTip2 =false;
				}
				if(bTip&&!bTip2) m_pParent->SendWarnMsg("您的植物尚未成熟。");
				if(bTip&&bTip2) m_pParent->SendWarnMsg("请先种植");
			}

		}
		return true;
	case C_UNLOCK_MANOR:
		{
			FortData* pFort = m_pParent->GetFortData();
			if (pFort)
			{
				pFort->UnlockManorPos();
			}

		}
		return true;
	case C_GET_MANORDATA:
		{
			SET_DATA(pData,FRIEND,C_GET_MANORDATA,pPacket);
			FortData* pFort = g_pGameCenter->m_pFortCenter->GetFortData(pData->dwUin);
			if (pFort)
			{
				pFort->SendManorData(m_pParent->m_dwUin,true);
			}

		}
		return true;
	case C_GET_MANORDATA_NEW:
		{
			SET_DATA(pData,FRIEND,C_GET_MANORDATA_NEW,pPacket);
			FortData* pFort = g_pGameCenter->m_pFortCenter->GetFortData(pData->dwUin);
			if (pFort)
			{
				pFort->SendManorData(m_pParent->m_dwUin,false);
			}

		}
		return true;
	case C_GET_MANOR_RECORD:
		{
			FortData* pFort = m_pParent->GetFortData();
			if (pFort)
			{
				pFort->SendManorRecord(m_pParent->m_dwUin);
			}

		}
		return true;
	case C_SEND_WARRANK_POS:
		{
			SET_DATA(pData,FRIEND,C_SEND_WARRANK_POS,pPacket);
			g_pGameCenter->m_pWarRankCenter->RequestFight(m_pParent,pData->nPos);
// 			FortData* pFort = g_pGameCenter->m_pFortCenter->GetFortData(pData->dwUin);
// 			if (pFort)
// 			{
// 				pFort->SendManorData(m_pParent->m_dwUin);
// 			}

		}
		return true;
	case C_FAST_MANOR:
		{
			SET_DATA(pData,FRIEND,C_FAST_MANOR,pPacket);
			FortData* pFort = m_pParent->GetFortData();
			FortData* pFastFort = g_pGameCenter->m_pFortCenter->GetFortData(pData->dwUin);
			if (pFort&&pFastFort)
			{
				int nExp = 0;
				if (!pFastFort->CheckFast(m_pParent->m_dwUin,pData->byPos,true))
				{
					m_pParent->SendWarnMsg("该植物已被浇过水了");
					return true;
				}
				if(pFastFort->FastManorPos(pData->byPos,m_pParent->m_dwUin,&nExp)){
					pFort->AddManorExp(nExp);
				}else{
					m_pParent->SendWarnMsg("该植物无法浇水");

				}
			}

		}
		return true;
	case C_FAST_MANOR_NEW:
		{
			SET_DATA(pData,FRIEND,C_FAST_MANOR_NEW,pPacket);
			FortData* pFort = m_pParent->GetFortData();
			FortData* pFastFort = g_pGameCenter->m_pFortCenter->GetFortData(pData->dwUin);
			if (pFort&&pFastFort)
			{
				int nExp = 0;
				if (!pFastFort->CheckFast(m_pParent->m_dwUin,pData->byPos))
				{
					m_pParent->SendWarnMsg("你今天已经给这位好友浇过水了");
					return true;
				}else if (pFastFort->GetFastCount(pData->byPos) > 4)
				{
					m_pParent->SendWarnMsg("该植物已浇水5次，无法再浇水");
					return true;
				}
				if(pFastFort->FastManorPosEx(pData->byPos,m_pParent->m_dwUin,&nExp)){
					pFort->AddManorExp(nExp);
				}else{
					m_pParent->SendWarnMsg("该植物无法浇水");

				}
			}

		}
		return true;
	}
	return false;
}

void FriendList::SendFriendList()
{
	int nCutPos	= 0;
	BuildPacketEx(pPacket,FRIEND, S_FRIEND_LIST,buf, 4096);
	SET_DATA(pSend,FRIEND, S_FRIEND_LIST,pPacket);
	
	pSend->wCnt		= m_pList->Count>80?80:m_pList->Count;
	pPacket->wSize	+=	pSend->wCnt*sizeof(FriendObj);
	for (int i = 0; i < m_pList->Count&&i<80; i++)
	{
		memcpy(&pSend->frdObj[i],m_pList->Items[i],sizeof(FriendObj));
	}
	

	//memcpy(&pSend->frdObj,pFObj,sizeof(FriendObj));
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void FriendList::ChgFriendRsT( DWORD dwUinOt, BYTE byRsT, bool bSaveDb )
{
	FriendObj*	pFObj	= _getFriend(dwUinOt);
	if(pFObj == NULL)
		return;

	pFObj->byRsType	= byRsT;
	if(bSaveDb)
		_updateDB(pFObj);
	SendFriend(pFObj);
}

bool FriendList::GetRsValByType( DWORD dwUinOt,BYTE byType,bool isOwn )
{
	FriendObj*	pFObj	= _getFriend(dwUinOt);
	if(pFObj == NULL)
		return false;
	int nVal = pFObj->nRsVal;

	if ((m_pParent->m_dwUin>dwUinOt) && isOwn)
	{
		nVal = nVal>>16;
	}else if ((m_pParent->m_dwUin<dwUinOt) && !isOwn)
	{
		nVal = nVal>>16;
	}
	return nVal&1<<byType;
}

void FriendList::SetRsValByType( DWORD dwUinOt,BYTE byType,bool bVal,bool bSaveDb, bool isOwn)
{
	FriendObj*	pFObj	= _getFriend(dwUinOt);
	if(pFObj == NULL)
		return;
	int nVal = pFObj->nRsVal;
	int nOff = byType;
	if (m_pParent->m_dwUin>dwUinOt&&isOwn)
	{
		nOff += 16;
	}else if (m_pParent->m_dwUin<dwUinOt&&!isOwn)
	{
		nOff += 16;
	}
	if(bVal) nVal |= (1<<nOff);
	else nVal&= (~(1<<nOff));

	SetFriendVal(dwUinOt,nVal,bSaveDb);
}

void FriendList::ResetRsVal()
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		FriendObj* pFObj	= (FriendObj*)m_pList->Items[i];
		int nOff = FRIEND_RS_VAL_GIVE;
		if (m_pParent->m_dwUin>pFObj->dwUinB)
		{
			nOff += 16;
		}
		int nVal = pFObj->nRsVal;
		nVal&= (~(1<<nOff));
		nOff = FRIEND_RS_VAL_GET;
		if (m_pParent->m_dwUin>pFObj->dwUinB)
		{
			nOff += 16;
		}
		nVal&= (~(1<<nOff));
		SetFriendVal(pFObj->dwUinB,nVal,true);
	}
}

void FriendList::SendFriend( FriendObj* pObj )
{
	BuildPacketEx(pPacket,FRIEND, S_FRIEND_VAL,buf, 4096);
	SET_DATA(pSend,FRIEND, S_FRIEND_VAL,pPacket);
	memcpy(&pSend->frdObj,pObj,sizeof(FriendObj));

	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

int FriendList::GetRsValByTypeCount(BYTE byType,bool isOwn /*= true*/ )
{
	int nCount = 0;
	for (int i=0;i<m_pList->Count;i++)
	{
		FriendObj* pFObj	= (FriendObj*)m_pList->Items[i];
		
		bool bTmpVal = GetRsValByType(pFObj->dwUinB,byType,true);
		if(bTmpVal) nCount++;

	}
	return nCount;	
}

bool FriendList::CheckFriendOnline(DWORD dwUin)
{
	UserData* pUser = g_pGameCenter->GetPlayer(dwUin);
	if (pUser)
		return true;
	return false;
}

void FriendList::UpdateCheckFrdOnline()
{
	if (m_pList->Count == 0)
		return;

	for (int i = 0; i < m_pList->Count; i++)
	{
		FriendObj* pFObj	= (FriendObj*)m_pList->Items[i];
		if (pFObj)
		{
			bool bOnline = CheckFriendOnline(pFObj->dwUinB);
			map<DWORD,bool>::iterator itor = m_mapState.find(pFObj->dwUinB);
			if (itor != m_mapState.end())
			{
				if (itor->second != bOnline)
				{
					itor->second = bOnline;
					SendFrdOnlineState(pFObj->dwUinB);
				}
			}else
			{
				m_mapState[pFObj->dwUinB] = bOnline;
				SendFrdOnlineState(pFObj->dwUinB);
			}
		}
	}
}

void FriendList::SendFrdOnlineList()
{
	BuildPacketEx(pPacket,FRIEND, S_FRIEND_ONLINE_LIST,buf,1024);
	SET_DATA(pSend,FRIEND, S_FRIEND_ONLINE_LIST,pPacket);
	int nCnt = 0;
	for (map<DWORD,bool>::iterator itor = m_mapState.begin(); itor != m_mapState.end(); itor++)
	{
		if (itor->second == true)
		{
			pSend->dwUin[nCnt] = itor->first;
			nCnt++;
		}
	}
	pSend->nCnt	= nCnt;
	pPacket->wSize	+=	nCnt*sizeof(DWORD);
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void FriendList::SendFrdOnlineState(DWORD dwUin)
{
	BuildPacketEx(pPacket,FRIEND, S_FRIEND_ONLINE_CHG,buf,1024);
	SET_DATA(pSend,FRIEND, S_FRIEND_ONLINE_CHG,pPacket);
	pSend->dwUin = dwUin;
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}

void FriendList::SendFriendChangeMsg( DWORD dwUin, char* szName )
{
	BuildPacketEx(pPacket,FRIEND, S_FRIEND_NAME_CHG,buf,1024);
	SET_DATA(pSend,FRIEND, S_FRIEND_NAME_CHG,pPacket);
	pSend->dwUin = dwUin;
	strcpy(pSend->szName,szName);
	TCP_SendPacketToUin(m_pParent->m_dwUin,pPacket);
}
