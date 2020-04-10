#include "stdafx.h"
#include "NumberData.h"

NumberData::NumberData(int nCnt)
{
	m_nMax = nCnt;
	m_pData = new BYTE[m_nMax*4];
	memset(m_pData,0,m_nMax*4);
}

NumberData::~NumberData()
{
	if(m_pData)
		delete m_pData;
}

void NumberData::SetVal(BYTE byEnum, int nVal)
{
	if(byEnum >= m_nMax)
		return;
	*(int*)(m_pData+4*byEnum) = nVal;
}

void NumberData::AddVal(BYTE byEnum, int nVal)
{
	if(byEnum >= m_nMax)
		return;
	*(int*)(m_pData+4*byEnum) += nVal;
}

int NumberData::GetVal(BYTE byEnum)
{
	if(byEnum >= m_nMax)
		return 0;
	return *(int*)(m_pData+4*byEnum);
}

bool NumberData::SetData(BYTE* p,int nSize)
{
	if(p == NULL || nSize < 4)
		return false;
	int nTemp = m_nMax*4;
	nTemp = nTemp < nSize ? nTemp : nSize;
	memcpy(m_pData,p,nTemp);
	return true;
}

bool NumberData::GetData(BYTE* p,int &nSize)
{
	nSize = m_nMax*4;
	memcpy(p,m_pData,nSize);
	return true;
}