#include "MainMenu.h"
#include "FortCenter.h"
#include "ItemData.h"
FortCenter::FortCenter( UserData* pData ):m_pUser(pData),m_dwSwitchFriendManor(0)
{
	m_pWarRankList = xnList::Create();
	m_pWarTenList = xnList::Create();
}
int nTempPos = 0;
bool FortCenter::PacketProcess( ZPacket* pPacket )
{
	if(pPacket->bCmdGroup != FRIEND)
		return false;

	switch(pPacket->bCmd)
	{
	case S_SEND_FORTDATA_NEW:
		{
			SET_DATA(pData,FRIEND,S_SEND_FORTDATA_NEW,pPacket);
			m_pManorData[pData->dwUin] = pData->xManorData;
			m_pWarRank[pData->dwUin] = pData->xWarRankData;
			_updateFortMenu();
		}
		return true;
	case C_SEND_WARRANK_STATE:
		{
			SET_DATA(pData,FRIEND,C_SEND_WARRANK_STATE,pPacket);
			if (pData->byState == WARRANK_STATE_START)
			{
				pMainMenu->m_pFortMenu->Close();
				pMainMenu->m_pWarRankMenu->Close();
			}else if (pData->byState == WARRANK_STATE_WIN)
			{
				nTempPos = pData->nVal+1;
				pMainMenu->PlayEffect(221);
			}else if (pData->byState == WARRANK_STATE_FAIL)
			{
				pMainMenu->PlayEffect(222);
			}

		}
		return true;
	case S_SEND_WARRANKDATA:
		{
			SET_DATA(pData,FRIEND,S_SEND_WARRANKDATA,pPacket);
			m_pWarRank[pData->dwUin] = pData->xWarRankData;
			_updateFortMenu();
		}
		return true;
	case S_SEND_MANOR_RECORD:
		{
			SET_DATA(pData,FRIEND,S_SEND_MANOR_RECORD,pPacket);
			m_pManorRocord.clear();
			for (int i = 0; i < pData->wCnt; i++)
			{
				m_pManorRocord[i+1] = pData->szRecord[i];
			}

			pMainMenu->m_pManorRecordMenu->InitMenu();
		}
		return true;
	case S_SEND_MANORDATA_NEW:
		{
			SET_DATA(pData,FRIEND,S_SEND_MANORDATA_NEW,pPacket);
			m_pManorData[pData->dwUin] = pData->xManorData;
			if (m_dwSwitchFriendManor == pData->dwUin)
			{
				m_dwSwitchFriendManor = 0;
				pMainMenu->m_pManorMenu->SetUin(pData->dwUin);
			}
			_updateFortMenu();
		}
		return true;
	case S_WARRANK_INFO:
		{
			SET_DATA(pData,FRIEND,S_WARRANK_INFO,pPacket);
			while(m_pWarRankList->Count) delete (WRankSimpleObj*)m_pWarRankList->Delete(0);
			for (int i=0;i<pData->bCnt;i++)
			{
				WRankSimpleObj* pObj = new WRankSimpleObj;
				memcpy(pObj,&pData->szWRObj[i],sizeof(WRankSimpleObj));
				m_pWarRankList->Add(pObj);
			}
			_updateFortMenu();
		}
		return true;
	case S_WARRANK_TEN_INFO:
		{
			SET_DATA(pData,FRIEND,S_WARRANK_TEN_INFO,pPacket);
			while(m_pWarTenList->Count) delete (WRankSimpleObj*)m_pWarTenList->Delete(0);
			for (int i=0;i<pData->bCnt;i++)
			{
				WRankSimpleObj* pObj = new WRankSimpleObj;
				memcpy(pObj,&pData->szWRObj[i],sizeof(WRankSimpleObj));
				m_pWarTenList->Add(pObj);
			}
			pMainMenu->m_pWarRankMenuEx->Reflesh();
		}
		return true;
	case S_SEND_FORTDATA_CHG:
		{
			SET_DATA(pData,FRIEND,S_SEND_FORTDATA_CHG,pPacket);
			std::map<DWORD,ManorDataEx>::iterator itor = m_pManorData.find(pData->dwUin);
			if (itor!= m_pManorData.end())
			{
				if (pData->byType == FORTDATA_CHG_MANOR_POS)
				{
					itor->second.nPos = pData->dwVal;
				}else if (pData->byType == FORTDATA_CHG_MANOR_EXP)
				{
					itor->second.nExp = pData->dwVal;
				}else if (pData->byType == FORTDATA_CHG_MANOR_LV)
				{
					itor->second.nLv = pData->dwVal;
				}else if (pData->byType == FORTDATA_CHG_MANOR_TIME)
				{
					itor->second.dwTime[pData->byPos] = pData->dwVal;
				}else if (pData->byType == FORTDATA_CHG_MANOR_ID)
				{
					itor->second.dwID[pData->byPos] = pData->dwVal;
				}else if (pData->byType == FORTDATA_CHG_MANOR_FAST)
				{
					itor->second.dwFast[pData->byPos] = pData->dwVal;
				}else if (pData->byType == FORTDATA_CHG_MANOR_FASTUIN)
				{
					for (int i = 0; i < 5; i++)
					{
						itor->second.dwFastUin[pData->byPos][i] = pData->dwVal;
					}
				}else if (pData->byType == FORTDATA_CHG_MANOR_EFFECT)
				{
					pMainMenu->m_pManorMenu->m_pComponentPic[pData->byPos]->SetEffect(pData->dwVal,1);
				}
			}
			std::map<DWORD,WarRankData>::iterator itor2 = m_pWarRank.find(pData->dwUin);
			if (itor2 != m_pWarRank.end())
			{
				if (pData->byType == FORTDATA_CHG_WARRANK_SCORE)
				{
					itor2->second.nRankScore = pData->dwVal;
				}
			}

			_updateFortMenu();
		}
		return true;
	}
	return false;
}

void FortCenter::_updateFortMenu()
{
	pMainMenu->m_pManorMenu->m_bUpdate = true;
	pMainMenu->m_pWarRankMenu->m_bUpdate = true;
}

ManorDataEx* FortCenter::GetManorData( DWORD dwUin )
{
	std::map<DWORD,ManorDataEx>::iterator itor = m_pManorData.find(dwUin);
	if (itor!= m_pManorData.end())
	{
		return &(itor->second);
	}
	return NULL;
}

void FortCenter::SeedManorPos( int nPos,ItemObj* pItem )
{
	if (!pItem)
	{
		return;
	}
	BuildPacketEx(pPacket, FRIEND, C_SEED_MANOR, buf, 1024);
	SET_DATA(pSend, FRIEND, C_SEED_MANOR, pPacket);
	pSend->byPos = nPos;
	pSend->dwIndex = pItem->GetVal(ITEM_AD_ID);
	pChar->SendPacket(pPacket);
}

void FortCenter::UnlockManorPos()
{
	pChar->SendCmd(FRIEND,C_UNLOCK_MANOR);
}

void FortCenter::FastManorPos( DWORD dwUin,int nPos )
{
	BuildPacketEx(pPacket, FRIEND, C_FAST_MANOR_NEW, buf, 1024);
	SET_DATA(pSend, FRIEND, C_FAST_MANOR_NEW, pPacket);
	pSend->byPos = nPos;
	pSend->dwUin = dwUin;
	pChar->SendPacket(pPacket);
}

void FortCenter::GetFriendManorData( DWORD dwUin )
{
	BuildPacketEx(pPacket, FRIEND, C_GET_MANORDATA_NEW, buf, 1024);
	SET_DATA(pSend, FRIEND, C_GET_MANORDATA_NEW, pPacket);
	pSend->dwUin = dwUin;
	pChar->SendPacket(pPacket);

	m_dwSwitchFriendManor = dwUin;
}

void FortCenter::GetManorReward( int nPos )
{
	BuildPacketEx(pPacket, FRIEND,C_GET_MANOR_REWARD , buf, 1024);
	SET_DATA(pSend, FRIEND, C_GET_MANOR_REWARD, pPacket);
	pSend->byPos = nPos;
	pChar->SendPacket(pPacket);

}

void FortCenter::GetManorReward( DWORD dwUin,int nPos )
{
	BuildPacketEx(pPacket, FRIEND,C_GET_MANOR_REWARDEX , buf, 1024);
	SET_DATA(pSend, FRIEND, C_GET_MANOR_REWARDEX, pPacket);
	pSend->dwUin = dwUin;
	pSend->byPos = nPos;
	pChar->SendPacket(pPacket);
}

void FortCenter::GetAllReward()
{
	pChar->SendCmd(FRIEND,C_GET_ALL_MANOR_REWARD);
}

WarRankData* FortCenter::GetWarRankData( DWORD dwUin )
{
	std::map<DWORD,WarRankData>::iterator itor = m_pWarRank.find(dwUin);
	if (itor!= m_pWarRank.end())
	{
		return &(itor->second);
	}
	return NULL;
}

FortCenter::~FortCenter()
{
	while(m_pWarRankList->Count) delete (WRankSimpleObj*) m_pWarRankList->Delete(0);
	while(m_pWarTenList->Count) delete (WRankSimpleObj*) m_pWarTenList->Delete(0);
}

void FortCenter::WarRankPos( int nPos )
{
	BuildPacketEx(pPacket, FRIEND,C_SEND_WARRANK_POS , buf, 1024);
	SET_DATA(pSend, FRIEND, C_SEND_WARRANK_POS, pPacket);
	pSend->nPos = nPos;
	pChar->SendPacket(pPacket);
}

int FortCenter::GetWarRankScore( DWORD dwUin )
{
	std::map<DWORD,WarRankData>::iterator itor = m_pWarRank.find(dwUin);
	if (itor!= m_pWarRank.end())
	{
		return itor->second.nRankScore;
	}
	return 0;
}

char* FortCenter::GetManorRocordName(DWORD dwUin)
{
	if (m_pManorRocord.size() == 0)
		return "";

	for (std::map<DWORD,NetManorRecord>::iterator itor = m_pManorRocord.begin(); itor != m_pManorRocord.end(); itor++)
	{
		if (itor->second.dwUin == dwUin)
			return itor->second.szName;
	}

	return "";
}

