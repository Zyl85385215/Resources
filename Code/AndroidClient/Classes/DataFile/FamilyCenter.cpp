#include "FamilyCenter.h"
#include "ios_home_packet.h"
#include "MainMenu.h"
FamilyCenter* pLeague = NULL;

int LeagueCompare(const void * Item1, const void * Item2){
	NHomeSInfo * pData1 = *(NHomeSInfo **)Item1;
	NHomeSInfo * pData2 = *(NHomeSInfo **)Item2;
	return pData2->bLv - pData1->bLv;
}
bool FamilyCenter::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != NHOME)
		return false;
	switch(pPacket->bCmd)
	{
	case S_HOMEINFO:
		{
			SET_DATA(pRecv, NHOME, S_HOMEINFO, pPacket);
			m_bLv = pRecv->bLv;
			m_dwExp = pRecv->dwExp;
			m_wCnt = pRecv->wCnt;
			m_bOccupy = pRecv->bOccupy;
			strncpy(m_szName,pRecv->szName,20);
			strncpy(m_szDesc,pRecv->szDesc,100);
			pChar->m_dwHomeID = pRecv->dwHomeID;
			pChar->m_dwHomeDuty = pRecv->byHomeDuty;
			m_xBuild	= pRecv->xBuild;
			while(m_pMemeber->Count) delete (NHomeMember*)m_pMemeber->Delete(0);
			for (int i=0;i<pRecv->wCnt;i++)
			{
				NHomeMember& dataInfo = pRecv->szMember[i];
				NHomeMember* pInfo = new NHomeMember(dataInfo);
				m_pMemeber->Add(pInfo);	

			}
			_familyUpdate();
		}
		return true;
	case S_HOME_DISSOLVED:
		{
			m_bLv = 0;
			m_dwExp = 0;
			m_wCnt = 0;
			m_bOccupy = 0;
			memset(m_szName,0,20);
			memset(m_szDesc,0,100);
			pChar->m_dwHomeID = 0;
			pChar->m_dwHomeDuty = 0;
			memset(&m_xBuild,0,sizeof(HomeBuildTag));
			while(m_pLeagueList->Count) delete m_pLeagueList->Delete(0);
			while(m_pMemeber->Count) delete m_pMemeber->Delete(0);
			while(m_pJoinList->Count) delete m_pJoinList->Delete(0);
			_familyUpdate();
			pMainMenu->m_pTabFamilyCtrl->Close();
		}
		return true;
	case S_HOMEDUTYCHG:
		{
			SET_DATA(pRecv, NHOME, S_HOMEDUTYCHG, pPacket);
			for (int i=0;i<m_pMemeber->Count;i++)
			{
				NHomeMember* pInfo = (NHomeMember*)m_pMemeber->Items[i];
				if (pInfo->dwUin == pRecv->dwUin)
				{
					pInfo->bDuty = pRecv->byDuty;
					break;
				}
			}
			_familyUpdate();
		}
		return true;
	case S_NEWREQUEST:
		{
			pMainMenu->m_pFastEquipMenu->NewHomeTips();
		}
		return true;
	case C_EXIT:
		{
			pChar->m_dwHomeID = 0;
			pChar->m_dwHomeDuty = 0;
			pMainMenu->m_pTabFamilyCtrl->Close();
			pMainMenu->PopTipMsg("你已经退出了军团");
		}
		return true;
	case S_HOMELIST:
		{
			SET_DATA(pRecv, NHOME, S_HOMELIST, pPacket);
			if(pRecv->ifClean)
			{
				while(m_pLeagueList->Count) delete m_pLeagueList->Delete(0);
			}
			for(int i=0;i<pRecv->wCnt;i++){
				NHomeSInfo& dataInfo = pRecv->szHome[i];
				if (dataInfo.bCountry != pChar->m_byCountry)
				{
					continue;
				}
				NHomeSInfo* pInfo = new NHomeSInfo();
				pInfo->bLv = dataInfo.bLv;
				strncpy(pInfo->szName , dataInfo.szName,20);
				pInfo->wHomeID = dataInfo.wHomeID;
				pInfo->wMemCnt = dataInfo.wMemCnt;
				pInfo->bCountry = dataInfo.bCountry;
				pInfo->bApply = dataInfo.bApply;
				m_pLeagueList->Add(pInfo);	
			}
			m_pLeagueList->Sort(LeagueCompare);
			_familyUpdate();
		}
		return true;
	case S_REQUEST_RESULT:
		{
			SET_DATA(pRecv, NHOME, S_REQUEST_RESULT, pPacket);
			for (int i=0;i<m_pLeagueList->Count;i++)
			{
				NHomeSInfo* pInfo = (NHomeSInfo*)m_pLeagueList->Items[i];
				if (pInfo->wHomeID == pRecv->wHomeID)
				{
					pInfo->bApply = true;
					pMainMenu->m_pFamilyListMenu->m_bUpdate = true;
				}
			}

		}
		return true;
	case S_REQUEST_CANCEL_RESULT:
		{
			SET_DATA(pRecv, NHOME, S_REQUEST_CANCEL_RESULT, pPacket);
			for (int i=0;i<m_pLeagueList->Count;i++)
			{
				NHomeSInfo* pInfo = (NHomeSInfo*)m_pLeagueList->Items[i];
				if (pInfo->wHomeID == pRecv->wHomeID)
				{
					pInfo->bApply = false;
					pMainMenu->m_pFamilyListMenu->m_bUpdate = true;
				}
			}

		}
		return true;
	case S_HOMEREQER:
		{
			while(m_pJoinList->Count) delete m_pJoinList->Delete(0);
			SET_DATA(pRecv, NHOME, S_HOMEREQER, pPacket);
			for(int i=0;i<pRecv->nCnt;i++){
				NHomeMember& dataInfo = pRecv->szReqers[i];
				NHomeMember* pInfo = new NHomeMember();
				pInfo->bLv = dataInfo.bLv;
				strncpy(pInfo->szName , dataInfo.szName,16);
				pInfo->dwUin = dataInfo.dwUin;
				pInfo->bJob = dataInfo.bJob;
				m_pJoinList->Add(pInfo);	
			}
			_familyUpdate();
		}
		return true;
	case C_SWITCH_BATTLE_POS:
		{
			SET_DATA(pRecv, NHOME, C_SWITCH_BATTLE_POS, pPacket);
			//if(pMainMenu->m_pLeagueChangePosMenu->m_pShipObj[pRecv->nPos1-1]) pMainMenu->m_pLeagueChangePosMenu->m_pShipObj[pRecv->nPos1-1]->nPos = pRecv->nPos2;
			//if(pMainMenu->m_pLeagueChangePosMenu->m_pShipObj[pRecv->nPos2-1]) pMainMenu->m_pLeagueChangePosMenu->m_pShipObj[pRecv->nPos2-1]->nPos = pRecv->nPos1;
			//pMainMenu->m_pLeagueChangePosMenu->m_bUpdate = true;
		}
		return true;
	case C_SET_BATTLE_POS:
		{
			SET_DATA(pRecv, NHOME, C_SET_BATTLE_POS, pPacket);
// 			for (int i=0;i<pMainMenu->m_pLeagueChangePosMenu->m_pList->Count;i++)
// 			{
// 				HomeBattleObj* pObj = (HomeBattleObj*)pMainMenu->m_pLeagueChangePosMenu->m_pList->Items[i];
// 				if (pObj->dwUin == pRecv->dwUin)
// 				{
// 					pObj->nPos = pRecv->nPos;
// 					pMainMenu->m_pLeagueChangePosMenu->m_bUpdate = true;
// 					return true;
// 				}
// 			}	
		}
		return true;
	case S_BATTLE_ACTOR:
		{
// 			while(pMainMenu->m_pLeagueChangePosMenu->m_pList->Count) delete pMainMenu->m_pLeagueChangePosMenu->m_pList->Items[0];
// // 			for (int i=0;i<18;i++)
// // 			{
// // 				pMainMenu->m_pLeagueChangePosMenu->m_pShipObj[i] = NULL;
// // 			}
// 			SET_DATA(pRecv, NHOME, S_BATTLE_ACTOR, pPacket);
// 			for(int i=0;i<pRecv->nCount;i++){
// 				HomeBattleObj& dataInfo = pRecv->BattleObj[i];
// 				HomeBattleObj* pInfo = new HomeBattleObj();
// 				(*pInfo) = dataInfo;
// 				pMainMenu->m_pLeagueChangePosMenu->m_pList->Add(pInfo);
// 			}
// 			//pMainMenu->m_pLeagueChangePosMenu->ResetObj();
// 			pMainMenu->m_pLeagueChangePosMenu->m_bUpdate = true;
		}
		return true;
	case S_HOMEBATTLEINFO:
		{
			SET_DATA(pRecv, NHOME, S_HOMEBATTLEINFO, pPacket);

		}
		return true;
	case C_CREATE:
		{
			pMainMenu->PopSystemMsg("创建军团成功");
			_familyUpdate();
 			pMainMenu->m_pTabSocialityCtrl->Close();
			pMainMenu->m_pTabFamilyCtrl->Open(0);
// 			pMainMenu->m_pLeagueInfoMenu->Open();
		}
		return true;
	case S_BUILDINFO:
		{
			SET_DATA(pRecv, NHOME, S_BUILDINFO, pPacket);
			m_xBuild = pRecv->xBuild;
			_familyUpdate();

		}
		return true;
	}
	return true;
}

FamilyCenter::FamilyCenter():m_bOccupy(0)
{
	m_pLeagueList = xnList::Create();
	m_pMemeber = xnList::Create();
	m_pJoinList = xnList::Create();
	memset(&m_xBuild,0,sizeof(HomeBuildTag));
}

FamilyCenter::~FamilyCenter()
{
	while(m_pLeagueList->Count) delete m_pLeagueList->Delete(0);
	while(m_pMemeber->Count) delete m_pMemeber->Delete(0);
	while(m_pJoinList->Count) delete m_pJoinList->Delete(0);
}

int FamilyCenter::GetContribute()
{
	for (int i=0;i<m_pMemeber->Count;i++)
	{
		NHomeMember* pInfo = (NHomeMember*)m_pMemeber->Items[i];
		if (pInfo->dwUin == pChar->m_dwUin)
		{
			//pMainMenu->m_pLeagueInfoMenu->m_bUpdate = true;
			//pMainMenu->m_pLeagueManageMenu->m_bUpdate = true;
			//pMainMenu->m_pLeagueMemberMenu->m_bUpdate = true;
			return pInfo->dwContribute;
		}
	}
	return 0;
}

int FamilyCenter::GetDuty()
{
	for (int i=0;i<m_pMemeber->Count;i++)
	{
		NHomeMember* pInfo = (NHomeMember*)m_pMemeber->Items[i];
		if (pInfo->dwUin == pChar->m_dwUin)
		{
			//pMainMenu->m_pLeagueInfoMenu->m_bUpdate = true;
			//pMainMenu->m_pLeagueManageMenu->m_bUpdate = true;
			//pMainMenu->m_pLeagueMemberMenu->m_bUpdate = true;
			return pInfo->bDuty;
		}
	}
	return 0;
}

void FamilyCenter::_familyUpdate()
{
	pMainMenu->m_pFamilyBuildMenu->m_bUpdate = true;
	pMainMenu->m_pFamilyInfoMenu->m_bUpdate = true;
	pMainMenu->m_pFamilyListMenu->m_bUpdate = true;
	pMainMenu->m_pFamilyMemberMenu->m_bUpdate = true;
	pMainMenu->m_pFamilyQuestMenu->m_bUpdate = true;
	pMainMenu->m_pFamilySkillMenu->m_bUpdate = true;
	pMainMenu->m_pFamilyJoinMenu->m_bUpdate = true;
	pMainMenu->m_pUserParamMenu->m_bUpdate = true;
}

NHomeSInfo* FamilyCenter::GetHomeInfoByID(DWORD wHomeID)
{
	if (!wHomeID)
	{
		return NULL;
	}
	for (int i = 0; i < m_pLeagueList->Count; i++)
	{
		NHomeSInfo*  HomeInfo = (NHomeSInfo*)m_pLeagueList->Items[i];
		if (wHomeID == HomeInfo->wHomeID)
		{
			return HomeInfo;
		}
	}
	return NULL;
}

int FamilyCenter::GetOnlineCount()
{
	int nCount = 0;
	for (int i=0;i<m_pMemeber->Count;i++)
	{
		NHomeMember* pInfo = (NHomeMember*)m_pMemeber->Items[i];
		if (pInfo->bOnline)
		{
			//pMainMenu->m_pLeagueInfoMenu->m_bUpdate = true;
			//pMainMenu->m_pLeagueManageMenu->m_bUpdate = true;
			//pMainMenu->m_pLeagueMemberMenu->m_bUpdate = true;
			nCount++;
		}
	}
	return nCount;
}

void	FamilyCenter::DissolveHome()
{
	if(GetDuty() != 3)	//团长
	{
		pMainMenu->PopTipMsg("只有军团长可以执行该操作");
		return;
	}
	SendCmd(NHOME,C_DISSOLVE_HOME);
}