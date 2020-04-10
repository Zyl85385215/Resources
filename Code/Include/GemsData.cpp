#include "gemsdata.h"

SetGemsDataFunc m_pSetGemsDataFuncArray[GEMS_DATA_VERSION_MAX] = { CGemsData::SetGemsDataV0 };
GetGemsDataFunc m_pGetGemsDataFuncArray[GEMS_DATA_VERSION_MAX] = { CGemsData::GetGemsDataV0 };

bool CGemsData::SetGemsDataV0(BYTE * pbData, int nSize)
{
	int nTempPos = 0;
	int nTempSize = 0;

	//	int version
	nTempSize = sizeof(int);
	pbData += nTempSize;
	nTempPos += nTempSize;

	//	int slot count
	nTempSize = sizeof(int);
	m_nSlotCount = *(int*)pbData;

	pbData += nTempSize;
	nTempPos += nTempSize;

	//	GemsData
	nTempSize = sizeof(GemData);
	int nGemsCount = (nSize - nTempPos) / nTempSize;
	if(nGemsCount == 0) return false;

	for(int i=0; i<nGemsCount; i++)
	{
		GemData * pGemData = (GemData*)pbData;
		AddGem(pGemData->bPos, pGemData->nGemID);

		pbData += nTempSize;
		nTempPos += nTempSize;
	}

	if(nTempSize != nSize) return false;
	return true;
}

bool CGemsData::GetGemsDataV0( BYTE * pbData, int & nSize )
{
	int nTempSize = 0;
	nSize = 0;

	//	int version
	*(int*)pbData = GEMS_DATA_VERSION_0;

	nTempSize = sizeof(int);
	pbData += nTempSize;
	nSize += nTempSize;

	//	int slot count
	*(int*)pbData = m_nSlotCount;

	nTempSize = sizeof(int);
	pbData += nTempSize;
	nSize += nTempSize;

	//	GemsData
	nTempSize = sizeof(GemData);

	GemData * pGemDataArray = (GemData*)pbData;
	for(int i=0, j=0; i<MAX_GEM_POSITION || j<m_nGemsCount; i++)
	{
		if(m_GemsArray[i] != 0)
		{
			pGemDataArray[j].bPos = i;
			pGemDataArray[j].nGemID = m_GemsArray[i];

			j++;
			nSize += nTempSize;
		}	
	}
	return true;
}

CGemsData::CGemsData( void )
{
	memset(m_GemsArray, 0, MAX_GEM_POSITION * sizeof(int));
	m_nGemsCount		= 0;
	m_nSlotCount		= 0;
}

CGemsData::CGemsData(BYTE * pbData, int nSize)
{
	memset(m_GemsArray, 0, MAX_GEM_POSITION * sizeof(int));
	m_nGemsCount		= 0;
	m_nSlotCount		= 0;

	(this->*m_pSetGemsDataFuncArray[*((int*)pbData)])(pbData, nSize);
}

CGemsData::CGemsData( CGemsData * pGemsData )
{
	if(pGemsData)
	{
		memcpy(m_GemsArray, pGemsData->m_GemsArray, MAX_GEM_POSITION * sizeof(int));
		m_nGemsCount = pGemsData->m_nGemsCount;
		m_nSlotCount = pGemsData->m_nSlotCount;
	}
	else
	{
		memset(m_GemsArray, 0, MAX_GEM_POSITION * sizeof(int));
		m_nGemsCount		= 0;
		m_nSlotCount		= 0;
	}
}

CGemsData::~CGemsData(void)
{
}

bool CGemsData::GetGemsData( BYTE * pbData, int & nSize )
{
	return (this->*m_pGetGemsDataFuncArray[GEMS_DATA_VERSION_NEWEST])(pbData, nSize);
}

bool CGemsData::AddGem(BYTE bPos, int nItemID)
{
	if(bPos >= m_nSlotCount) return false;
	if(m_GemsArray[bPos] == 0) 
	{
		m_nGemsCount++;
	}
	m_GemsArray[bPos] = nItemID;
	return true; 
}

bool CGemsData::DelGem(BYTE bPos)
{
	if(bPos >= m_nSlotCount) return false;
	if(m_GemsArray[bPos] != 0) 
	{
		m_nGemsCount--;
		while (bPos < m_nGemsCount)
		{
			m_GemsArray[bPos] = m_GemsArray[bPos+1];
			bPos++;
		}
		m_GemsArray[m_nGemsCount] = 0;
	}
	return true; 
}

int	CGemsData::GetGem(BYTE bPos)	
{ 
	if(bPos >= m_nSlotCount) return 0; 
	return m_GemsArray[bPos]; 
}

bool CGemsData::AddSlot()
{
	if(m_nSlotCount >= MAX_GEM_POSITION) return false;
	m_nSlotCount++;
	return true;
}