#include "stdafx.h"
#include "xnExtstr.h"
#include "MapBaseInfo.h"

CreateSingleton(MapConfig);

MapBarrier::MapBarrier()
{
	m_pData	= NULL;
}

MapBarrier::~MapBarrier()
{
	free(m_pData);
}

void MapBarrier::InitMapBarrier( char* szFile )
{
	m_nWidth	= 0;
	m_nHeight	= 0;
	if(m_pData)
		free(m_pData);

	DWORD bufSize	= 0;
	FILE* pF	= fopen(szFile,"rb");
	if(pF == NULL)
		return;

	fseek(pF,0,SEEK_SET);
	fseek(pF,0,SEEK_END);
	bufSize	=	ftell(pF);
	fseek(pF,0,SEEK_SET);
	char*	szStrBuf	= (char*)malloc(bufSize);
	fread(szStrBuf,1,bufSize,pF);
	

	m_nWidth	= *(int*)szStrBuf;
	m_nHeight	= *(int*)(szStrBuf+4);
	m_pData	= (char*)malloc(bufSize-8);
	memset (m_pData,0,bufSize-8);
	memcpy(m_pData,szStrBuf+8,bufSize-8);
	m_nWidth	*= BLOCKING_SIZE*4;
	m_nHeight	*= BLOCKING_SIZE*4;

	free(szStrBuf);
}

bool MapBarrier::GetBlock( int x, int y )
{
	if (this == NULL)
		return true;
	if ((x < 0) || (y < 0) || (x >= m_nWidth) || (y >= m_nHeight))
		return true;
	x = (x)/ BLOCKING_SIZE; y = (y)/ BLOCKING_SIZE;
	BYTE k = m_pData[((m_nWidth/BLOCKING_SIZE+7)/8) * y + x / 8];
	BYTE u = (1 << 7) >> (x % 8);
	return ((k & u) != 0);			//该地图没有动态障碍

	//else
	//	//return ((k & u) || (*(byteLineEx[(y/*>>4*/)] + (x/*>>4*/))));		//该地图有动态障碍
	//	return ((k & u) || (*(byteLineEx[y] + x)));
}

bool MapBarrier::CheckLineBlock( int x1, int y1 ,int x2, int y2,int * pX,int * pY )
{
	if(pX)
		*pX	= x1;
	if(pY)
		*pY	= y1;
	if (this == NULL)
		return true;

	if(GetBlock(x1, y1) && !GetBlock(x2, y2))
		return false;

	if(pX == NULL)
		if (/*GetBlock(pos1->x, pos1->y) || */GetBlock(x2, y2))
			return true;

	int dx = x2 - x1;
	int dy = y2 - y1;
	int absdx = abs(dx);
	int absdy = abs(dy);
	if ((absdx == 0) && (absdy == 0))
		return false;

	if (absdx < absdy)
	{
		dx = dx * 256 / absdy;
		dy = dy / absdy;
		int x = x1 * 256 + dx;
		for (int i=y1+dy; i!= y2; i+=dy)
		{
			if (GetBlock(((x) / 256), i))
			{
				return true;
			}
			if(pX)
				*pX	= (x) / 256;
			if(pY)
				*pY	= i;
			x+=dx;
		}
	}
	else
	{
		dy = dy * 256 / absdx;
		dx = dx / absdx;
		int y = y1 * 256 + dy;
		for (int i=x1+dx; i!= x2; i+=dx)
		{
			if (GetBlock(i, ((y) / 256)))
			{
				return true;
			}
			if(pX)
				*pX	= i;
			if(pY)
				*pY	= (y) / 256;
			y += dy;
		}
	}
	return false;
}


//----------------------------------
MapConfig::MapConfig()
{
	m_pList		= xnList::Create();
}

MapConfig::~MapConfig()
{
	while(m_pList->Count)
		delete (MapCfgObj*)m_pList->Delete(0);
	m_pList->Free();
}

void MapConfig::LoadFile( char* szFile )
{
	xnExtstr * pStr = xnExtstr::Import(szFile);

	int nIndex=0;
	while(pStr->SetSection (nIndex++))
	{
		MapCfgObj*	pMObj	= new MapCfgObj();
		pMObj->m_wMapIndex		= atoi (pStr->sectionName);
		pMObj->m_wMapBase	= atoi(pStr->ReadString("realmap", "0"));
		if(pMObj->m_wMapBase	== 0)
			pMObj->m_wMapBase	= pMObj->m_wMapIndex;

		char	szTmp[256];
		sprintf(szTmp,"Map//%d//Barrier.bar",pMObj->m_wMapBase);
		pMObj->m_pMapBarr	= new MapBarrier();
		pMObj->m_pMapBarr->InitMapBarrier(szTmp);

		pMObj->m_wMapSizeW	= atoi(pStr->ReadString("mapsizew", "5000"));
		pMObj->m_wMapSizeH	= atoi(pStr->ReadString("mapsizeh", "3000"));
		pMObj->m_wReviveX	= atoi(pStr->ReadString("revivex", "0"));
		pMObj->m_wReviveY	= atoi(pStr->ReadString("revivey", "0"));
		pMObj->m_byFType	= atoi(pStr->ReadString("force", "0"));

		int		nCnt	= atoi(pStr->ReadString("wcnt","0"));
		for (int i = 0; i < nCnt; i++)
		{
			_MuliParamObj* pMP	= new _MuliParamObj();
			sprintf(szTmp,"wid%d",i+1);
			pMP->nParam1		= atoi(pStr->ReadString(szTmp,"0"));
			sprintf(szTmp,"wper%d",i+1);
			pMP->nParam2		= atoi(pStr->ReadString(szTmp,"0"));
			pMObj->pListWeather->Add(pMP);
		}

		m_pList->Add(pMObj);
	}
}

MapCfgObj* MapConfig::GetMapCfgObj( WORD wMIndex )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		MapCfgObj* pMObj	= (MapCfgObj*)m_pList->Items[i];
		if(pMObj->m_wMapIndex == wMIndex)
			return pMObj;
	}
	return NULL;
}

MapCfgObj::MapCfgObj()
{
	pListWeather	= xnList::Create();
}

MapCfgObj::~MapCfgObj()
{
	while(pListWeather->Count)
		delete (_MuliParamObj*)pListWeather->Delete(0);
	pListWeather->Free();
	delete m_pMapBarr;
}

BYTE MapCfgObj::RandGetWeather()
{
	if(pListWeather->Count == 0)
		return 0;

	int nRand	= rand()%100;
	for (int i = 0; i < pListWeather->Count; i++)
	{
		_MuliParamObj* pMP	= (_MuliParamObj*)pListWeather->Items[i];
		if(pMP->nParam2 > nRand)
		{
			return pMP->nParam1;
		}
		else
			nRand -= pMP->nParam2;
	}
	return 0;
}
