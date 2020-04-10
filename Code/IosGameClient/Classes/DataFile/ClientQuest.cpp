#include "UserData.h"
#include "ClientQuest.h"
#include "MainMenu.h"

#include "GuideCenter.h"

ClientQuestCenter::ClientQuestCenter()
{
	m_pListCanAccpet	= xnList::Create();
	m_pListQuest		= xnList::Create();
	m_bUpdateQuestChg			= true;
}

ClientQuestCenter::~ClientQuestCenter()
{
	m_pListCanAccpet->Free();
	while(m_pListQuest->Count)
	{
		ClientQuest* pCQ	= (ClientQuest*)m_pListQuest->Delete(0);
		if(pCQ->pszVal)
			free(pCQ->pszVal);
		delete pCQ;

	}
	m_pListQuest->Free();
}

void ClientQuestCenter::ReCheckCanAccept()
{
	xnList*	pList	= xnList::Create();
	for (int i = 0; i < QuestCfg::GetInstance().m_pListQLine->Count; i++)
	{
		QuestLine*	pQLine	= (QuestLine*)QuestCfg::GetInstance().m_pListQLine->Items[i];
		for (int j = 0; j < pQLine->m_pListQStep->Count; j++)
		{
			QuestObj*	pQObj	= (QuestObj*)pQLine->m_pListQStep->Items[j];
			if(CheckQuestCanAccept(pQObj) == 0)
			{
				pList->Add(pQObj);
			}
		}
	}

	bool	bIsSame	= true;
	if(pList->Count != m_pListCanAccpet->Count)
		bIsSame	= false;
	if(bIsSame)
	{
		for (int i = 0; i < m_pListCanAccpet->Count; i++)
		{
			if(m_pListCanAccpet->Items[i] != pList->Items[i])
			{
				bIsSame =false;
				break;
			}
		}
	}
	if(bIsSame)
		pList->Free();
	else
	{
		m_pListCanAccpet->Free();
		m_pListCanAccpet	= pList;
		m_bUpdateQuestChg	= true;
	}
}

int ClientQuestCenter::CheckQuestCanAccept( QuestObj* pQBase )
{
	if(GetCurQuest(pQBase->wQID))	//已接
		return 1;

	if(pQBase->wLine)
	{
		if(GetCurQuestByLine(pQBase->wLine))
			return 9;
	}

	if(pChar->m_byLvl	< pQBase->bLvNeed && pQBase->bType!=0)
		return 2;

	if(pChar->m_byCountry >= 3 || pQBase->bCtyNeed[pChar->m_byCountry] == 0 )
		return 3;
	if(pChar->m_byJob >= 5 || pQBase->bJobNeed[pChar->m_byJob] == 0 )
		return 4;

	if(pQBase->wLine)
	{//该线任务已过
		if(pChar->m_pClientFlag->GetQLineStep(pQBase->wLine) != pQBase->bStep)
			return 7;
	}

	if(pQBase->bDayTime)
	{
		if(pChar->m_pClientFlag->GetQDayTime(pQBase->wQID) >= pQBase->bDayTime)
			return 5;
	}

	if(pQBase->wItemNeed)
	{//消耗物品接任务
		int nCnt = pChar->m_pItemCenter->GetItemCountByID(pQBase->wItemNeed);
		if(nCnt == 0)
			return 6;
		
		if(pQBase->bItemCost>nCnt)
			return 6;
	}

	//查看任务前置
	if(pQBase->pLPreQ->Count)
	{
		bool bPreQOk	= false;
		for (int i = 0; i < pQBase->pLPreQ->Count; i++)
		{
			_MuliParamObj* pMP	= (_MuliParamObj*)pQBase->pLPreQ->Items[i];
			if(pChar->m_pClientFlag->GetQLineStep(pMP->nParam1) <= pMP->nParam2)
			{//前置任务未完成
				if(pQBase->bAllDone)
					return 8;
				else
				{
					bPreQOk	= true;
				}
			}
		}
		if(!bPreQOk)
		{//前置任务未完成
			return 8;
		}
	}

	return 0;
}

ClientQuest* ClientQuestCenter::GetCurQuest( WORD wQID )
{
	for (int i = 0; i < m_pListQuest->Count; i++)
	{
		ClientQuest* pCQuest	= (ClientQuest*)m_pListQuest->Items[i];
		if(pCQuest->wQID	== wQID)
			return pCQuest;
	}
	return NULL;
}

ClientQuest* ClientQuestCenter::GetCurQuestByLine( WORD wLine )
{
	for (int i = 0; i < m_pListQuest->Count; i++)
	{
		ClientQuest* pCQuest	= (ClientQuest*)m_pListQuest->Items[i];
		if(pCQuest->pQBase->wLine	== wLine)
			return pCQuest;
	}
	return NULL;
}

bool ClientQuestCenter::CheckQuestByLine( WORD wLine,xnList* pList )
{
	for (int i = 0; i < pList->Count; i++)
	{
		QuestObj* pCQuest	= (QuestObj*)pList->Items[i];
		if(pCQuest->wLine	== wLine)
			return true;
	}
	return false;
}

void ClientQuestCenter::Update()
{
	for (int i = 0; i < m_pListQuest->Count; i++)
	{
		ClientQuest*	pCQuest	= (ClientQuest*)m_pListQuest->Items[i];		
		int	nCheckPos	= 0;
		bool	bIsOK	= true;
		for (int i = 0; i < pCQuest->pQBase->pLQCondtion->Count; i++)
		{
			_QConditon*	pQC	= (_QConditon*)pCQuest->pQBase->pLQCondtion->Items[i];
			switch(pQC->bDType)
			{//1杀怪 2物品 8玩家行为 9引导
			case 1:
			case 8:
			case 9:
				if(pCQuest->pQBase->byValCnt < nCheckPos)
					bIsOK	= false;

				if(pCQuest->pszVal[nCheckPos]	< pQC->wDNum)
					bIsOK	= false;


				nCheckPos++;
				break;
			case 2:
			case 3:
				if(pChar->m_pItemCenter->GetItemCountByID(pQC->wDID) < pQC->wDNum)
					bIsOK	= false;
				break;
			}
			if(bIsOK == false)
				break;
		}
		if(pCQuest->bOk	!= bIsOK)
		{
			pCQuest->bOk	= bIsOK;
			m_bUpdateQuestChg	= true;
		}
	}
}

void ClientQuestCenter::RecvQuestInfo( WORD wQId, WORD* pszwVal )
{
	
	ClientQuest* pCQuest	= GetCurQuest(wQId);
	if(pCQuest == NULL)
	{
		pMainMenu->m_pMissionMenu->m_pFormTrack->Close();
		pCQuest		= new ClientQuest();
		pCQuest->wQID	= wQId;
		pCQuest->pQBase	= QuestCfg::GetInstance().GetQuestByID(wQId);
		if(pCQuest->pQBase == NULL)
		{
			delete pCQuest;
			return;
		}
		if(pCQuest->pQBase->byValCnt)
		{
			pCQuest->pszVal	= (WORD*)malloc(pCQuest->pQBase->byValCnt*2);
			memset(pCQuest->pszVal,0,pCQuest->pQBase->byValCnt*2);
		}
		else
		{
			pCQuest->pszVal	= NULL;
		}
		
		m_pListQuest->Add(pCQuest);

		pCQuest->bOk	= false;
		pCQuest->bChged	= false;

		if(pChar->m_dwTickJoin && xnGetTickCount() - pChar->m_dwTickJoin > 8000 )
			pMainMenu->PopTipMsg("接受任务[%s]",pCQuest->pQBase->szName);
		ReCheckCanAccept();	//更新可接任务
		m_bUpdateQuestChg	= true;
	}

	if(pCQuest->pszVal)
	{
		memcpy(pCQuest->pszVal,pszwVal,pCQuest->pQBase->byValCnt*2);
		pCQuest->bChged	= true;
	}
}

void ClientQuestCenter::RecvDeleteQuest( WORD wQId,bool bComplete )
{
	pMainMenu->m_pMissionMenu->m_pFormTrack->Close();
	QuestObj*	pQBase	= NULL;
	for (int i = 0; i < m_pListQuest->Count; i++)
	{
		ClientQuest*	pCQuest	= (ClientQuest*)m_pListQuest->Items[i];	
		if(pCQuest->pQBase->wQID == wQId)
		{
			pQBase	= pCQuest->pQBase;
			if(pCQuest->pszVal)
				free(pCQuest->pszVal);
			delete pCQuest;
			m_pListQuest->Delete(i);
			break;
		}
	}
	if(pQBase)
	{
		if(bComplete)
		{
			if(pQBase->bType == 6)
				pMainMenu->PopTipMsg("完成任务[%s],获得友好度+10",pQBase->szName);
			else
				pMainMenu->PopTipMsg("完成任务[%s]",pQBase->szName);
		}
		else
			pMainMenu->PopTipMsg("放弃任务[%s]",pQBase->szName);

		ReCheckCanAccept();	//更新可接任务
		m_bUpdateQuestChg	= true;
		if(bComplete && pQBase->wNpcEnd)
			pMainMenu->m_pMissionMenu->CheckNpcQ(pQBase->wNpcEnd);
	}

}

void ClientQuestCenter::UpdateSec()
{

}

void ClientQuestCenter::ClearQuest()
{
	while(m_pListQuest->Count)
	{
		ClientQuest*	pCQuest	= (ClientQuest*)m_pListQuest->Delete(0);	
		if(pCQuest->pszVal)
			free(pCQuest->pszVal);
		delete pCQuest;
	}
}
