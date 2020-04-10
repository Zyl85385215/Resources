#include "GrowEquipConfig.h"
#include "xnExtstr.h"
#include <string>

CreateSingleton(GrowEquipList);

GrowEquipList::GrowEquipList()
{
	m_pList = xnList::Create();
}

GrowEquipList::~GrowEquipList()
{
	while (m_pList->Count)
	{
		GrowEquipData * pData = (GrowEquipData *)m_pList->Items[0];
		m_pList->Delete(0);
		SAFE_DELETE(pData);
	}
	m_pList->Free();
}

GrowEquipData * GrowEquipList::GetFixedGrowData( int nItemID, int nLevel)
{
	GrowEquipData * pFixedData = NULL;
	GrowEquipData * pPrevGrowData = GetPrevGrowData(nItemID,nLevel);
	GrowEquipData * pNextGrowData = GetNextGrowData(nItemID,nLevel);
	if (pPrevGrowData)
	{
		pFixedData = new GrowEquipData;
		if (!pNextGrowData)
		{
			for (int i=0;i<ATTRIB_MAX;i++)
			{
				pFixedData->wAttri[i] = pPrevGrowData->wAttri[i];
			}
		}
		else
		{
			if(pPrevGrowData->nLevel==pNextGrowData->nLevel)
			{
				for (int i=0;i<ATTRIB_MAX;i++)
				{
					pFixedData->wAttri[i] = pNextGrowData->wAttri[i];
				}
			}
			else
			{
				for (int i=0;i<ATTRIB_MAX;i++)
				{
					pFixedData->wAttri[i] = pPrevGrowData->wAttri[i] + (pNextGrowData->wAttri[i] - pPrevGrowData->wAttri[i])/(pNextGrowData->nLevel-pPrevGrowData->nLevel)*(nLevel-pPrevGrowData->nLevel);
				}
			}
		}
	}
	return pFixedData;
}

GrowEquipData * GrowEquipList::GetPrevGrowData( int nItemID ,int nLevel)
{
	for (int i=m_pList->Count-1;i>=0;i--)
	{
		GrowEquipData * pData = (GrowEquipData *)m_pList->Items[i];
		if (nItemID == pData->nItemID&&pData->nLevel <= nLevel)
		{
			return pData;
		}
	}
	return NULL;
}

GrowEquipData * GrowEquipList::GetNextGrowData( int nItemID ,int nLevel)
{
	for (int i=0;i<m_pList->Count;i++)
	{
		GrowEquipData * pData = (GrowEquipData *)m_pList->Items[i];
		if (nItemID == pData->nItemID&&pData->nLevel >= nLevel)
		{
			return pData;
		}
	}
	return NULL;
}

bool GrowEquipList::Load( const char * filename )
{
	xnExtstr * pIni = xnExtstr::Import((char*)filename);
	if(pIni == NULL) return false;

	int nSectionCount = pIni->sectionCount;
	for(int i=0; i<nSectionCount; i++)
	{
		pIni->SetSection(i);

		GrowEquipData * pData = new GrowEquipData;
		sscanf(pIni->sectionName,"%d|%d",&pData->nItemID,&pData->nLevel);
		char szText[32];
		for (int j=0;j<ATTRIB_MAX;j++)
		{
			sprintf(szText,"%d",j);
			pData->wAttri[j] = atoi(pIni->ReadString(szText,"0"));
		}
		m_pList->Add(pData);
	}
	pIni->Free();
}

GrowEquipData * GrowEquipList::GetGrowData( int nItemID )
{
	for (int i=0;i<m_pList->Count;i++)
	{
		GrowEquipData * pData = (GrowEquipData *)m_pList->Items[i];
		if (pData->nItemID == nItemID)
		{
			return pData;
		}
	}
	return NULL;
}

GrowEquipData::GrowEquipData()
{
	for (int i=0;i<ATTRIB_MAX;i++)
	{
		wAttri[i] = 0;
	}
	nItemID = 0;
	nLevel = 0;
}