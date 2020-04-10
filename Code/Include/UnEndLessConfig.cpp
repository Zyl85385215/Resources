#include "UnEndLessConfig.h"
#include "xnExtstr.h"

CreateSingleton(EndLessConfig);
CreateSingleton(NewBookCollectConfig);
//extern int	Random_Int(int min, int max);
EndLessInfo::EndLessInfo()
{
	m_pList = xnList::Create();
}

EndLessInfo::~EndLessInfo()
{
	while (m_pList->Count)
	{
		GiveItemInfo * pInfo = (GiveItemInfo *)m_pList->Delete(0);
		if (pInfo)
			delete pInfo;
	}
	m_pList->Free();
}

EndLessConfig::EndLessConfig()
{
	m_pList = xnList::Create();
}

EndLessConfig::~EndLessConfig()
{
	while (m_pList->Count)
	{
		EndLessInfo *pInfo = (EndLessInfo *)m_pList->Delete(0);
		if (pInfo)
		{
			if (pInfo->szText)
				free(pInfo->szText);
			delete pInfo;
		}
	}
	m_pList->Free();
}

EndLessInfo* EndLessConfig::GetInfo(int nLvl, int nStep)
{
	EndLessInfo *pInfo = NULL;
	int nNum = m_pList->Count;
	for(int i=0; i<nNum; i++)
	{
		pInfo = (EndLessInfo *)m_pList->Items[i];
		if (pInfo)
		{
			if (pInfo->nLvlLow<=nLvl && pInfo->nLvlHigh>=nLvl && nStep==pInfo->nStep)
			{
				return pInfo;
			}
		}
	}
	return NULL;
}

DWORD EndLessConfig::GetGiveItem(int nLvl, int nStep, int nRnd, int nRnd2)
{
	EndLessInfo *pInfo = GetInfo(nLvl, nStep);
	if (pInfo)
	{
		if (nRnd2>0 && nRnd2<=pInfo->nPer)
		{
			int nNum = pInfo->m_pList->Count;
			for (int i=0; i<nNum; i++)
			{
				GiveItemInfo *pItem = (GiveItemInfo *)pInfo->m_pList->Items[i];
				if (pItem)
				{		
					if (nRnd <= pItem->nPer)
						return pItem->dwItemID;
				}
			}
		}
	}
	return NULL;
}


void EndLessConfig::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;
	int index = 0;
	char szTemp[64];
	while (pStr->SetSection(index++) != NULL)
	{
		EndLessInfo* pInfo = new EndLessInfo;
		pInfo->nLvlLow = atoi(pStr->ReadString("lowlvl", "0"));
		pInfo->nLvlHigh = atoi(pStr->ReadString("highlvl", "0"));
		pInfo->nExp = atoi(pStr->ReadString("exp", "0"));
		pInfo->nGold = atoi(pStr->ReadString("gold", "0"));
		pInfo->nStep = atoi(pStr->ReadString("step", "0"));
		pInfo->szText = strdup(pStr->ReadString("txt", ""));
		pInfo->nPer = atoi(pStr->ReadString("per", "0"));
		int nNum = atoi(pStr->ReadString("num", "0"));
		for (int i=0; i<nNum; i++)
		{
			GiveItemInfo * pItem = new GiveItemInfo;
			sprintf(szTemp, "itemid%d", i+1);
			pItem->dwItemID = atoi(pStr->ReadString(szTemp, "0"));
			sprintf(szTemp, "nper%d", i+1);
			pItem->nPer = atoi(pStr->ReadString(szTemp, "0"));
			pInfo->m_pList->Add(pItem);
		}
		m_pList->Add(pInfo);
	}
	pStr->Free();
}


NewBookCollectConfig::NewBookCollectConfig()
{
	m_pList = xnList::Create();
	m_pRList = xnList::Create();
	m_pJiangList = xnList::Create();
	m_pXiaList = xnList::Create();
	m_pDaoList = xnList::Create();
	m_pYaoList = xnList::Create();
	m_pCiList = xnList::Create();
	m_pGongList = xnList::Create();

	szsFlagName = NULL;
	szrFlagName = NULL;
}

NewBookCollectConfig::~NewBookCollectConfig()
{
	while (m_pRList->Count)
	{
		BookRInfo *pInfo = (BookRInfo *)m_pRList->Delete(0);
		if (pInfo)
			delete pInfo;
	}
	m_pRList->Free();

	while (m_pList->Count)
	{
		BookCollectInfo *pBookInfp = (BookCollectInfo *)m_pList->Delete(0);
		if (pBookInfp)
			delete pBookInfp;
	}
	m_pList->Free();

	while (m_pJiangList->Count)
	{
		PieceID *pID = (PieceID *)m_pJiangList->Delete(0);
		if (pID)
			delete pID;
	}
	m_pJiangList->Free();

	while (m_pXiaList->Count)
	{
		PieceID *pID = (PieceID *)m_pXiaList->Delete(0);
		if (pID)
			delete pID;
	}
	m_pXiaList->Free();

	while (m_pDaoList->Count)
	{
		PieceID *pID = (PieceID *)m_pDaoList->Delete(0);
		if (pID)
			delete pID;
	}
	m_pDaoList->Free();

	while (m_pYaoList->Count)
	{
		PieceID *pID = (PieceID *)m_pYaoList->Delete(0);
		if (pID)
			delete pID;
	}
	m_pYaoList->Free();

	while (m_pCiList->Count)
	{
		PieceID *pID = (PieceID *)m_pCiList->Delete(0);
		if (pID)
			delete pID;
	}
	m_pCiList->Free();

	while (m_pGongList->Count)
	{
		PieceID *pID = (PieceID *)m_pGongList->Delete(0);
		if (pID)
			delete pID;
	}
	m_pGongList->Free();

	if (szsFlagName)
	{
		free(szsFlagName);
	}

	if (szrFlagName)
	{
		free(szrFlagName);
	}
}

DWORD  NewBookCollectConfig::GetMonDrop(int nLvl, int nRand)
{
	if (nLvl>=m_nOpenLevel && nRand>0 && nRand<=m_nMonPer)
		return m_dwBaodianID;
	return 0;
}

DWORD  NewBookCollectConfig::GetBossDrop(int nLvl, int nRand)
{
	if (nLvl>=m_nOpenLevel && nRand>0 && nRand<=m_nBossPer)
		return m_dwBaodianID;
	return 0;
}

int  NewBookCollectConfig::GetJobNum(BYTE bJob)
{
	if (bJob == 1)
	{
		return m_pJiangList->Count;
	}
	else if (bJob == 2)
	{
		return m_pXiaList->Count;
	}
	else if (bJob == 3)
	{
		return m_pDaoList->Count;
	}
	else if (bJob == 4)
	{
		return m_pYaoList->Count;
	}
	else if (bJob == 5)
	{
		return m_pCiList->Count;
	}
	else if (bJob == 6)
	{
		return m_pGongList->Count;
	}
	return 0;
}

bool NewBookCollectConfig::bCanGetRobot(int nDelt, int nPer)
{
	for (int i=0; i<m_pRList->Count; i++)
	{
		BookRInfo *pInfo = (BookRInfo *)m_pRList->Items[i];
		if (pInfo)
		{
			if (pInfo->nLvlDetal==nDelt)
			{
				if (nPer <= pInfo->nPer)
					return true;
				else
					return false;
			}
		}
	}
	return false;
}	

int  NewBookCollectConfig::GetTotalNum(int nPos)
{
	if (nPos>=1 && nPos<=m_pList->Count)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[nPos-1];
		if (pInfo)
		{
			return pInfo->m_pBookIDList->Count;
		}
	}
	return 0;
}

int  NewBookCollectConfig::GetBookCollectID(BYTE bJob, int nRand)
{
	if (bJob == 1)
	{
		if (nRand>0 && nRand<=m_pJiangList->Count)
		{
			PieceID *pID = (PieceID *)m_pJiangList->Items[nRand-1];
			if (pID)
				return pID->nPieceID;
		}
		return 0;
	}
	else if (bJob == 2)
	{
		if (nRand>0 && nRand<=m_pXiaList->Count)
		{
			PieceID *pID = (PieceID *)m_pXiaList->Items[nRand-1];
			if (pID)
				return pID->nPieceID;
		}
		return 0;
	}
	else if (bJob == 3)
	{
		if (nRand>0 && nRand<=m_pDaoList->Count)
		{
			PieceID *pID = (PieceID *)m_pDaoList->Items[nRand-1];
			if (pID)
				return pID->nPieceID;
		}
		return 0;
	}
	else if (bJob == 4)
	{
		if (nRand>0 && nRand<=m_pYaoList->Count)
		{
			PieceID *pID = (PieceID *)m_pYaoList->Items[nRand-1];
			if (pID)
				return pID->nPieceID;
		}
		return 0;
	}
	else if (bJob == 5)
	{
		if (nRand>0 && nRand<=m_pCiList->Count)
		{
			PieceID *pID = (PieceID *)m_pCiList->Items[nRand-1];
			if (pID)
				return pID->nPieceID;
		}
		return 0;
	}
	else if (bJob == 6)
	{
		if (nRand>0 && nRand<=m_pGongList->Count)
		{
			PieceID *pID = (PieceID *)m_pGongList->Items[nRand-1];
			if (pID)
				return pID->nPieceID;
		}
		return 0;
	}
	return 0;
}

void NewBookCollectConfig::LoadFile( char* szFile )
{
	xnExtstr* pStr = xnExtstr::Import(szFile);
	if(pStr == NULL)
		return;
	int index = 0;
	char szTemp[64];

	if (pStr->SetSection("main") != NULL)
	{
		m_nOpenLevel = atoi(pStr->ReadString("lvl", "0"));
		m_nMonPer = atoi(pStr->ReadString("monper", "0"));
		m_nBossPer = atoi(pStr->ReadString("bossper", "0"));
		m_nRChance = atoi(pStr->ReadString("rchance", "0"));
		m_dwBaodianID = atoi(pStr->ReadString("宝典ID","0"));
		
		szrFlagName = strdup(pStr->ReadString("rplayerflag",""));
		szsFlagName = strdup(pStr->ReadString("sPlayerflag",""));
		int nNum = atoi(pStr->ReadString("rnum", "0"));
		int i=0;
		for (i=0; i<nNum; i++)
		{
			BookRInfo * pInfo = new BookRInfo;
			sprintf(szTemp, "rlvl%d", i+1);
			pInfo->nLvlDetal = atoi(pStr->ReadString(szTemp, "0"));
			sprintf(szTemp, "rgetper%d", i+1);
			pInfo->nPer = atoi(pStr->ReadString(szTemp, "0"));
			m_pRList->Add(pInfo);
		}

		nNum = atoi(pStr->ReadString("jiangnum", "0"));
		for (i=0; i<nNum; i++)
		{
			PieceID *pID = new PieceID;
			sprintf(szTemp, "jiangid%d", i+1);
			pID->nPieceID = atoi(pStr->ReadString(szTemp, "0"));
			m_pJiangList->Add(pID);
		}

		nNum = atoi(pStr->ReadString("xianum", "0"));
		for (i=0; i<nNum; i++)
		{
			PieceID *pID = new PieceID;
			sprintf(szTemp, "xiaid%d", i+1);
			pID->nPieceID = atoi(pStr->ReadString(szTemp, "0"));
			m_pXiaList->Add(pID);
		}

		nNum = atoi(pStr->ReadString("daonum", "0"));
		for (i=0; i<nNum; i++)
		{
			PieceID *pID = new PieceID;
			sprintf(szTemp, "daoid%d", i+1);
			pID->nPieceID = atoi(pStr->ReadString(szTemp, "0"));
			m_pDaoList->Add(pID);
		}

		nNum = atoi(pStr->ReadString("yaonum", "0"));
		for (i=0; i<nNum; i++)
		{
			PieceID *pID = new PieceID;
			sprintf(szTemp, "yaoid%d", i+1);
			pID->nPieceID = atoi(pStr->ReadString(szTemp, "0"));
			m_pYaoList->Add(pID);
		}

		nNum = atoi(pStr->ReadString("cinum", "0"));
		for (i=0; i<nNum; i++)
		{
			PieceID *pID = new PieceID;
			sprintf(szTemp, "ciid%d", i+1);
			pID->nPieceID = atoi(pStr->ReadString(szTemp, "0"));
			m_pCiList->Add(pID);
		}

		nNum = atoi(pStr->ReadString("gongnum", "0"));
		for (i=0; i<nNum; i++)
		{
			PieceID *pID = new PieceID;
			sprintf(szTemp, "gongid%d", i+1);
			pID->nPieceID = atoi(pStr->ReadString(szTemp, "0"));
			m_pGongList->Add(pID);
		}

		m_nSChance = atoi(pStr->ReadString("schance", "0"));
		m_nSPer = atoi(pStr->ReadString("sgetper", "0"));
		int nIndex = 1;
		while (pStr->SetSection(nIndex++) != NULL)
		{
			BookCollectInfo *pBookInfo = new BookCollectInfo;
			pBookInfo->m_nBaseSkillID = atoi(pStr->ReadString("skill", "0"));
			nNum = atoi(pStr->ReadString("num", "0"));
			for (int j=0; j<nNum; j++)
			{
				BookId *pID = new BookId;
				sprintf(szTemp, "id%d", j+1);
				pID->nSkillID = atoi(pStr->ReadString(szTemp, "0"));
				pBookInfo->m_pBookIDList->Add(pID);
			}

			nNum = atoi(pStr->ReadString("skillnum", "0"));
			for (int p=0; p<nNum; p++)
			{
				CollectInfo *pCollectInfo = new CollectInfo;
				sprintf(szTemp, "skill%d", p+1);
				pCollectInfo->nSkillID = atoi(pStr->ReadString(szTemp, "0"));
				sprintf(szTemp, "per%d", p+1);
				pCollectInfo->nPer = atoi(pStr->ReadString(szTemp, "0"));
				sprintf(szTemp, "statea%d", p+1);
				pCollectInfo->dwStateA = atoi(pStr->ReadString(szTemp, "0"));
				sprintf(szTemp, "stateb%d", p+1);
				pCollectInfo->dwStateB = atoi(pStr->ReadString(szTemp, "0"));
				sprintf(szTemp, "val%d", p+1);
				pCollectInfo->dwVal = atoi(pStr->ReadString(szTemp, "0"));
				pBookInfo->m_pbookSkillList->Add(pCollectInfo);
			}
			m_pList->Add(pBookInfo);
		}
	}
	pStr->Free();
}


BookCollectInfo::BookCollectInfo()
{
	m_pBookIDList = xnList::Create();
	m_pbookSkillList = xnList::Create();
}

BookCollectInfo::~BookCollectInfo()
{
	while (m_pBookIDList->Count)
	{
		BookId * pItem = (BookId *)m_pBookIDList->Delete(0);
		if (pItem)
			delete pItem;
	}
	m_pBookIDList->Free();

	while (m_pbookSkillList->Count)
	{
		CollectInfo *pInfo = (CollectInfo *)m_pbookSkillList->Delete(0);
		if (pInfo)
			delete pInfo;
	}
	m_pbookSkillList->Free();
}

BookCollectInfo *NewBookCollectConfig::GetBookCollectInfo(int nSkillID)
{
	for (int i=0; i<m_pList->Count; i++)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[i];
		if (pInfo)
		{
			if (pInfo->m_nBaseSkillID == nSkillID)
			{
				return pInfo;
			}
		}
	}
	return NULL;
}

int NewBookCollectConfig::GetSkillIDByOrder(int nPos)
{
	if (nPos>=1 && nPos<=m_pList->Count)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[nPos-1];
		if (pInfo)
			return pInfo->m_nBaseSkillID;
	}
	return 0;
}

int NewBookCollectConfig::GetAddBookLevelPer(int nSkillID, int nLevel)
{
	BookCollectInfo *pInfo = GetBookCollectInfo(nSkillID);
	if (pInfo)
	{
		if (nLevel>=1 && nLevel<=pInfo->m_pbookSkillList->Count)
		{
			CollectInfo *pCollect = (CollectInfo *)pInfo->m_pbookSkillList->Items[nLevel-1];
			if (pCollect)
			{
				return pCollect->nPer;
			}
		}
	}
	return 0;
}

CollectInfo  *NewBookCollectConfig::GetAddStateOnSkill(DWORD dwSkillID, int nRand, int nLvl)
{
	for (int i=0; i<m_pList->Count; i++)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[i];
		if (pInfo)
		{
			if (pInfo->m_nBaseSkillID == dwSkillID)
			{
				if (nLvl>=1 && nLvl<=pInfo->m_pbookSkillList->Count)
				{
					CollectInfo *pCollect = (CollectInfo *)pInfo->m_pbookSkillList->Items[nLvl-1];
					if (pCollect)
					{
						if (nRand <= pCollect->dwVal)
						{	//可以放
							return pCollect;
						}
					}
				}
				
			}
		}
	}
	return NULL;
}

int  NewBookCollectConfig::GetDrawSkillID(BYTE bJob, int nPos)
{
	int j=0;
	int i=0;
	PieceID *pID = NULL;
	if (bJob == 1)
	{
		for (i=0; i<m_pJiangList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pJiangList->Count)
			{
				pID = (PieceID *)(m_pJiangList->Items[nPos-1]);
				if (pID)
				{
					if (pID->nPieceID>=1 && pID->nPieceID<=m_pList->Count)
					{
						CollectInfo *pInfo = (CollectInfo *)m_pList->Items[pID->nPieceID-1];
						if (pInfo)
						{
							return pInfo->nSkillID;
						}
					}
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 2)
	{
		for (i=0; i<m_pXiaList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pXiaList->Count)
			{
				pID = (PieceID *)(m_pXiaList->Items[nPos-1]);
				if (pID)
				{
					if (pID->nPieceID>=1 && pID->nPieceID<=m_pList->Count)
					{
						CollectInfo *pInfo = (CollectInfo *)m_pList->Items[pID->nPieceID-1];
						if (pInfo)
						{
							return pInfo->nSkillID;
						}
					}
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 3)
	{
		for (i=0; i<m_pDaoList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pDaoList->Count)
			{
				pID = (PieceID *)(m_pDaoList->Items[nPos-1]);
				if (pID)
				{
					if (pID->nPieceID>=1 && pID->nPieceID<=m_pList->Count)
					{
						CollectInfo *pInfo = (CollectInfo *)m_pList->Items[pID->nPieceID-1];
						if (pInfo)
						{
							return pInfo->nSkillID;
						}
					}
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 4)
	{
		for (i=0; i<m_pYaoList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pYaoList->Count)
			{
				pID = (PieceID *)(m_pYaoList->Items[nPos-1]);
				if (pID)
				{
					if (pID->nPieceID>=1 && pID->nPieceID<=m_pList->Count)
					{
						CollectInfo *pInfo = (CollectInfo *)m_pList->Items[pID->nPieceID-1];
						if (pInfo)
						{
							return pInfo->nSkillID;
						}
					}
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 5)
	{
		for (i=0; i<m_pCiList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pCiList->Count)
			{
				pID = (PieceID *)(m_pCiList->Items[nPos-1]);
				if (pID)
				{
					if (pID->nPieceID>=1 && pID->nPieceID<=m_pList->Count)
					{
						CollectInfo *pInfo = (CollectInfo *)m_pList->Items[pID->nPieceID-1];
						if (pInfo)
						{
							return pInfo->nSkillID;
						}
					}
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 6)
	{
		for (i=0; i<m_pGongList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pGongList->Count)
			{
				pID = (PieceID *)(m_pGongList->Items[nPos-1]);
				if (pID)
				{
					if (pID->nPieceID>=1 && pID->nPieceID<=m_pList->Count)
					{
						CollectInfo *pInfo = (CollectInfo *)m_pList->Items[pID->nPieceID-1];
						if (pInfo)
						{
							return pInfo->nSkillID;
						}
					}
				}
				return 0;
			}
		}
		return 0;
	}
}

int NewBookCollectConfig::GetOrderBySkillID(int nSkillID)
{
	for (int i=0; i<m_pList->Count; i++)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[i];
		if (pInfo && pInfo->m_nBaseSkillID==nSkillID)
		{
			return i+1;
		}
	}
	return 0;
}

int  NewBookCollectConfig::GetDrawSkillOrder(BYTE bJob, int nPos)
{
	int i=0;
	PieceID *pID = NULL;
	if (bJob == 1)
	{
		for (i=0; i<m_pJiangList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pJiangList->Count)
			{
				pID = (PieceID *)(m_pJiangList->Items[nPos-1]);
				if (pID)
				{
					return pID->nPieceID;
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 2)
	{
		for (i=0; i<m_pXiaList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pXiaList->Count)
			{
				pID = (PieceID *)(m_pXiaList->Items[nPos-1]);
				if (pID)
				{
					return pID->nPieceID;
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 3)
	{
		for (i=0; i<m_pDaoList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pDaoList->Count)
			{
				pID = (PieceID *)(m_pDaoList->Items[nPos-1]);
				if (pID)
				{
					return pID->nPieceID;
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 4)
	{
		for (i=0; i<m_pYaoList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pYaoList->Count)
			{
				pID = (PieceID *)(m_pYaoList->Items[nPos-1]);
				if (pID)
				{
					return pID->nPieceID;
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 5)
	{
		for (i=0; i<m_pCiList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pCiList->Count)
			{
				pID = (PieceID *)(m_pCiList->Items[nPos-1]);
				if (pID)
				{
					return pID->nPieceID;
				}
				return 0;
			}
		}
		return 0;
	}
	else if (bJob == 6)
	{
		for (i=0; i<m_pGongList->Count; i++)
		{
			if (nPos>=1 && nPos<=m_pGongList->Count)
			{
				pID = (PieceID *)(m_pGongList->Items[nPos-1]);
				if (pID)
				{
					return pID->nPieceID;
				}
				return 0;
			}
		}
		return 0;
	}
	return 0;
}

bool NewBookCollectConfig::GetThisJobCanGet(BYTE bJob, int j)
{
	int i=0;
	PieceID *pID = NULL;
	if (bJob == 1)
	{
		for (i=0; i<m_pJiangList->Count; i++)
		{
			pID = (PieceID *)(m_pJiangList->Items[i]);
			if (pID && pID->nPieceID==j)
				return true;
		}
		return false;
	}
	else if (bJob == 2)
	{
		for (i=0; i<m_pXiaList->Count; i++)
		{
			pID = (PieceID *)(m_pXiaList->Items[i]);
			if (pID && pID->nPieceID==j)
				return true;
		}
		return false;
	}
	else if (bJob == 3)
	{
		for (i=0; i<m_pDaoList->Count; i++)
		{
			pID = (PieceID *)(m_pDaoList->Items[i]);
			if (pID && pID->nPieceID==j)
				return true;
		}
		return false;
	}
	else if (bJob == 4)
	{
		for (i=0; i<m_pYaoList->Count; i++)
		{
			pID = (PieceID *)(m_pYaoList->Items[i]);
			if (pID && pID->nPieceID==j)
				return true;
		}
		return false;
	}
	else if (bJob == 5)
	{
		for (i=0; i<m_pCiList->Count; i++)
		{
			pID = (PieceID *)(m_pCiList->Items[i]);
			if (pID && pID->nPieceID==j)
				return true;
		}
		return false;
	}
	else if (bJob == 6)
	{
		for (i=0; i<m_pGongList->Count; i++)
		{
			pID = (PieceID *)(m_pGongList->Items[i]);
			if (pID && pID->nPieceID==j)
				return true;
		}
		return false;
	}
	return false;
}

bool  NewBookCollectConfig::AnalySkill(DWORD dwSkillID, int nLevel, BYTE bJob)
{
	for (int i=0; i<m_pList->Count; i++)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[i];
		if (pInfo && pInfo->m_nBaseSkillID==dwSkillID)
		{	
			if(nLevel>=1 && nLevel<=pInfo->m_pbookSkillList->Count)
			{	//判断是不是这个职业可用
				if (GetThisJobCanGet(bJob, i+1))
				{	//看这个 等级是否符合 要求

					return true;
				}
			}
			return false;
		}
	}
	return false;
}

bool  NewBookCollectConfig::AnalyPiece(DWORD dwSkillID, DWORD dwOrder, BYTE bJob)
{
	for (int i=0; i<m_pList->Count; i++)
	{
		BookCollectInfo *pInfo = (BookCollectInfo *)m_pList->Items[i];
		if (pInfo && pInfo->m_nBaseSkillID==dwSkillID)
		{	//判断是不是这个职业可用
			if (dwOrder>= 1 && dwOrder<=pInfo->m_pBookIDList->Count)
			{
				return true;
			}
			return false;
		}
	}
	return false;
}