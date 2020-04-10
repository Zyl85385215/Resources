#include "stdafx.h"

#include "QuestConfig.h"
#include "xnExtstr.h"
CreateSingleton(QuestCfg);


QuestObj::QuestObj()
{
	szName		= NULL;
	szDescSrc	= NULL;
	szDescEnd	= NULL;
	pLPreQ		= xnList::Create();
	pLTalkSrc	= xnList::Create();
	pLTalkEnd	= xnList::Create();

	pLQCondtion	= xnList::Create();
	pLMonDrop	= xnList::Create();

	pLAwardItem	= xnList::Create();
}

QuestObj::~QuestObj()
{
	if(szName)
		free(szName);
	if(szDescSrc)
		free(szDescSrc);
	if(szDescEnd)
		free(szDescEnd);

	while(pLTalkSrc->Count)
		free(pLTalkSrc->Delete(0));
	pLTalkSrc->Free();
	while(pLTalkEnd->Count)
		free(pLTalkEnd->Delete(0));
	pLTalkEnd->Free();

	while(pLQCondtion->Count)
	{
		_QConditon* pQCond	= (_QConditon*)pLQCondtion->Delete(0);
		if(pQCond->szTitle)
			free(pQCond->szTitle);
		delete pQCond;
	}
	pLQCondtion->Free();

	while(pLMonDrop->Count)
		delete (_QMonDrop*)pLMonDrop->Delete(0);
	pLMonDrop->Free();
	while(pLPreQ->Count)
		delete (_MuliParamObj*)pLPreQ->Delete(0);
	pLPreQ->Free();

	while(pLAwardItem->Count)
		delete (_MuliParamObj*)pLAwardItem->Delete(0);
	pLAwardItem->Free();
}

QuestLine::QuestLine()
{
	m_pListQStep	= xnList::Create();
}

QuestLine::~QuestLine()
{
	while(m_pListQStep->Count)
		delete (QuestObj*)m_pListQStep->Delete(0);
	m_pListQStep->Free();
}

QuestCfg::QuestCfg()
{
	m_pListQLine	= xnList::Create();	
}

QuestCfg::~QuestCfg()
{
	while(m_pListQLine->Count)
		delete (QuestLine*)m_pListQLine->Delete(0);
	m_pListQLine->Free();
}

void QuestCfg::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	char	szTmp[32];
	int		nIndex	= 0;
	int		nCnt	= 0;
	while(pStr->SetSection(nIndex++))
	{
		QuestObj*	pQObj	= new QuestObj();
		pQObj->wQID			= atoi(pStr->sectionName);
		pQObj->wLine		= atoi(pStr->ReadString("line","0"));
		pQObj->bStep		= atoi(pStr->ReadString("step","0"));
		pQObj->bType		= atoi(pStr->ReadString("type","0"));
		_addNewQuest(pQObj,pQObj->wLine);

		pQObj->bAllDone		= atoi(pStr->ReadString("sptype","0"));

		nCnt	= atoi(pStr->ReadString("spcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MuliParamObj* pMP	= new _MuliParamObj();
			pQObj->pLPreQ->Add(pMP);
			sprintf(szTmp,"line%d",i+1);
			pMP->nParam1	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"step%d",i+1);
			pMP->nParam2	= atoi(pStr->ReadString(szTmp,"0"));
		}
			
		pQObj->bLvNeed		= atoi(pStr->ReadString("lvneed","0"));
		char*	szStr		= pStr->ReadString("country","111");
		pQObj->bCtyNeed[0]	= szStr[0]-'0';
		pQObj->bCtyNeed[1]	= szStr[1]-'0';
		pQObj->bCtyNeed[2]	= szStr[2]-'0';
		szStr		= pStr->ReadString("job","11111");
		pQObj->bJobNeed[0]	= szStr[0]-'0';
		pQObj->bJobNeed[1]	= szStr[1]-'0';
		pQObj->bJobNeed[2]	= szStr[2]-'0';
		pQObj->bJobNeed[3]	= szStr[3]-'0';
		pQObj->bJobNeed[4]	= szStr[4]-'0';
		
		pQObj->wNpcSrc		= atoi(pStr->ReadString("npcsrc","0"));
		pQObj->wNpcEnd		= atoi(pStr->ReadString("npcend","0"));
		pQObj->wItemNeed	= atoi(pStr->ReadString("itemsrc","0"));
		pQObj->bItemCost	= atoi(pStr->ReadString("itemtype","0"));
		pQObj->wEnemyID		= atoi(pStr->ReadString("enemyid","0"));
		pQObj->bCallType	= atoi(pStr->ReadString("calltype","0"));
		pQObj->wStateID		= atoi(pStr->ReadString("stateid","0"));

		pQObj->bDayTime		= atoi(pStr->ReadString("daytime","0"));
		pQObj->szName		= strdup(pStr->ReadString("name","unkown"));

		nCnt	= atoi(pStr->ReadString("dlgsrccnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"dlgsrc%d",i+1);
			pQObj->pLTalkSrc->Add(strdup(pStr->ReadString(szTmp,"txt")));
		}
		nCnt	= atoi(pStr->ReadString("dlgendcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			sprintf(szTmp,"dlgend%d",i+1);
			pQObj->pLTalkEnd->Add(strdup(pStr->ReadString(szTmp,"txt")));
		}
		pQObj->szDescSrc	= strdup(pStr->ReadString("textsrc","unkown"));
		pQObj->szDescEnd	= strdup(pStr->ReadString("textdest","unkown"));

		nCnt	= atoi(pStr->ReadString("destcnt","0"));
		int nValPos	= 0;
		for (int i = 0; i < nCnt; i++)
		{
			_QConditon*	pQC	= new _QConditon();
			sprintf(szTmp,"desttype%d",i+1);
			pQC->bDType		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"destid%d",i+1);
			pQC->wDID		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"destnum%d",i+1);
			pQC->wDNum		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"desttxt%d",i+1);
			pQC->szTitle	= strdup(pStr->ReadString(szTmp,"QCondtion"));
			if(pQC->bDType != 2)
				pQC->bValPos	= nValPos++;
			else
				pQC->bValPos	= 0;

			pQObj->pLQCondtion->Add(pQC);
		}
		pQObj->byValCnt	= nValPos;

		nCnt	= atoi(pStr->ReadString("chgmoncnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_QMonDrop*	pQM	= new _QMonDrop();
			sprintf(szTmp,"chgmonster%d",i+1);
			pQM->wMonID		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"chgmonitem%d",i+1);
			pQM->wItemID	= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"chgmonper%d",i+1);
			pQM->wPerChance	= atoi(pStr->ReadString(szTmp,"10000"));
			pQObj->pLMonDrop->Add(pQM);
		}
		
		pQObj->nAwardExp	= atoi(pStr->ReadString("awardexp","1"));
		pQObj->nAwardGold	= atoi(pStr->ReadString("awardgold","1"));
		pQObj->bSelOne		= atoi(pStr->ReadString("awarditemtype","0"));

		nCnt	= atoi(pStr->ReadString("awarditemcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MuliParamObj*	pMP	= new _MuliParamObj();
			sprintf(szTmp,"awarditem%d",i+1);
			pMP->nParam1		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"awardnum%d",i+1);
			pMP->nParam2	= atoi(pStr->ReadString(szTmp,"0"));
			if(pMP->nParam1 == 0 && pMP->nParam2 == 0)
			{
				delete pMP;
				continue;
			}
			pQObj->pLAwardItem->Add(pMP);
		}
	}
	pStr->Free();
}

void QuestCfg::_addNewQuest( QuestObj* pObj,WORD wLine )
{
	QuestLine* pQLine	= _getQLine(wLine,true);
	if(pQLine)
		pQLine->m_pListQStep->Add(pObj);
}

QuestLine* QuestCfg::_getQLine( WORD wLine,bool bCreate /*= false*/ )
{
	 for (int i = 0; i < m_pListQLine->Count; i++)
	 {
		 QuestLine* pQLine	= (QuestLine*)m_pListQLine->Items[i];
		 if(pQLine->wLine == wLine)
			 return pQLine;
	 }
	 if(bCreate)
	 {
		 QuestLine*	pQLine	= new QuestLine();
		 pQLine->wLine		= wLine;
		 m_pListQLine->Add(pQLine);
		 return pQLine;
	 }
	 return NULL;
}

QuestObj* QuestCfg::GetQuestByID( WORD wQId )
{
	for (int i = 0; i < m_pListQLine->Count; i++)
	{
		QuestLine* pQLine	= (QuestLine*)m_pListQLine->Items[i];
		for (int j = 0; j < pQLine->m_pListQStep->Count; j++)
		{
			QuestObj* pQObj	= (QuestObj*)pQLine->m_pListQStep->Items[j];
			if(pQObj->wQID == wQId)
				return pQObj;
		}
	}
	return NULL;
}

bool QuestCfg::GetFamilyQuest( int nLv,xnList* pList )
{
	for (int i = 0; i < m_pListQLine->Count; i++)
	{
		QuestLine* pQLine	= (QuestLine*)m_pListQLine->Items[i];
		for (int j = 0; j < pQLine->m_pListQStep->Count; j++)
		{
			QuestObj* pQObj	= (QuestObj*)pQLine->m_pListQStep->Items[j];
			if(pQObj->bLvNeed <= nLv&&pQObj->bType == 3)
				pList->Add(pQObj);
		}
	}
	return true;
}

bool QuestCfg::GetMarryQuest( int nLv,xnList* pList )
{
	for (int i = 0; i < m_pListQLine->Count; i++)
	{
		QuestLine* pQLine	= (QuestLine*)m_pListQLine->Items[i];
		for (int j = 0; j < pQLine->m_pListQStep->Count; j++)
		{
			QuestObj* pQObj	= (QuestObj*)pQLine->m_pListQStep->Items[j];
			if(pQObj->bLvNeed <= nLv&&pQObj->bType == 6)
				pList->Add(pQObj);
		}
	}
	return true;
}


