#include "stdafx.h"
#include "random.h"


CRandom::CRandom()
{
	m_wSeed = 0;
}

CRandom::~CRandom()
{
}

void CRandom::Random_Seed(int seed)
{
	m_wSeed = seed;	
}

int CRandom::Random_Int(int min, int max)
{
	if(min > max)
	{
		int nTemp = max;
		min = nTemp;
		max = min;
	}
	m_wSeed = 214013*m_wSeed + 2531011;
	int result = min + (m_wSeed ^ m_wSeed>>15)%(max-min+1);
	return result;
}

CRandom g_pRandom;

void Random_Seed(int seed)
{
	g_pRandom.Random_Seed(seed);
}

int Random_Int()
{
	return g_pRandom.Random_Int(0,32767);
}

int Random_Int(int min, int max)
{
	return g_pRandom.Random_Int(min,max);
}