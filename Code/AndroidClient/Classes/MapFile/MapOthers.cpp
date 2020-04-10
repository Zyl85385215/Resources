#include "MapOthers.h"
#include "MapStaticCfg.h"

#define BLOCKING_SIZE 16

using namespace cocos2d;

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
	SAFE_FREE(m_pData);

	DWORD bufSize	= NULL;
	char * szStrBuf = (char *)CCFileUtils::sharedFileUtils()->getFileData(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(szFile),"rb", &bufSize);
	if (szStrBuf == NULL)
		return;

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
	if(m_pData == NULL)
		return false;
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

	if(GetBlock(x1, y1) && !GetBlock(x2, y2) && CalcDistance (x1,y1,x2,y2)< 150)
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
			if(i%10 == 0)
			{
				if(pX)
					*pX	= (x) / 256;
				if(pY)
					*pY	= i;
			}

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
			if(i %10==0)
			{
				if(pX)
					*pX	= i;
				if(pY)
					*pY	= (y) / 256;
			}

			y += dy;
		}
	}
	return false;
}

MapFindPath::MapFindPath()
{
	m_pPathRtl	= NULL;
	m_pMapTransPt	= xnList::Create ();
	m_bLeadCmd	= 0;
	m_dwLeadParam	= 0;
	m_dwLeadParam2	= 0;
	m_nPosMTPIndex	= -1;
	m_nPosMTPX		= 0;
	m_nPosMTPY		= 0;
	m_pBarrier		= NULL;
}

MapFindPath::~MapFindPath()
{
	m_pMapTransPt->Free ();
}

//左上角坐标系
bool MapFindPath::RequestFindPath( int nPosFX,int nPosFY, int nPosTX, int nPosTY,xnList* pLRtlPts )
{
	if(!m_pBarrier || m_pBarrier->GetBlock (nPosTX,nPosTY))
		return false;
	m_pPathRtl	= pLRtlPts;

	if(m_pBarrier->CheckLineBlock(nPosFX,nPosFY,nPosTX,nPosTY) == false)
	{
		POINT* pt	= new POINT;
		pt->x	= nPosTX;
		pt->y	= nPosTY;
		m_pPathRtl->Add (pt);
		return true;
	}

	POINT ptFMin,ptTMin,ptRctLT,ptRctRB;
	ptFMin.x	= nPosFX/BLOCKING_SIZE;
	ptFMin.y	= nPosFY/BLOCKING_SIZE;
	ptTMin.x	= nPosTX/BLOCKING_SIZE;
	ptTMin.y	= nPosTY/BLOCKING_SIZE;
	//ptRctLT.x	= min(ptFMin.x,ptTMin.x);
	//ptRctLT.y	= min(ptFMin.y,ptTMin.y);
	//ptRctRB.x	= max(ptFMin.x,ptTMin.x);
	//ptRctRB.y	= max(ptFMin.y,ptTMin.y);
	ptRctLT.x	= 0;
	ptRctLT.y	= 0;
	ptRctRB.x	= (m_pBarrier->m_nWidth+15)/16;
	ptRctRB.y	= (m_pBarrier->m_nHeight+15)/16;
	
	if(_findPath(ptFMin,ptTMin,ptRctLT,ptRctRB) == false)
		return false;

	return true;
}

//已除BLOCKING_SIZE
bool MapFindPath::_findPath( POINT ptFMin, POINT ptTMin,POINT ptRctLT,POINT ptRctRB )
{
	while(m_pPathRtl->Count)
	{
		delete (POINT*)m_pPathRtl->Delete (0);
	}

	int	nWidth	= ptRctRB.x - ptRctLT.x;
	int nHeight	= ptRctRB.y - ptRctLT.y;
	int	nSX		= ptRctLT.x;
	int nSY		= ptRctLT.y;

	int*	sznMark	= new int[nWidth*nHeight];
	_initBlock(sznMark,nSX,nSY,nWidth,nHeight);

	int	nOutF=-1,nOutT=-1;
	//双A*算法
	if(_doubleAx(sznMark,ptFMin.x - nSX,ptFMin.y - nSY, ptTMin.x - nSX,ptTMin.y - nSY,nWidth,nHeight,nOutF,nOutT) == false)
	{
		delete[] sznMark;
		return false;
	}

	xnList*	pListTmp	= xnList::Create ();
	int	nMark	=0;
	if(nOutF != -1)
	{
		pListTmp->Insert (0,(void*)nOutF);
		nMark	= sznMark[nOutF];
		while(--nMark > 0)
		{
			if(nOutF%nWidth	!= nWidth-1 && sznMark[nOutF-1] == nMark)
				nOutF	=	nOutF-1;
			else if(nOutF%nWidth	!= 0 && sznMark[nOutF+1] == nMark)
				nOutF	=	nOutF+1;
			else if(nOutF/nWidth	!= nHeight-1 && sznMark[nOutF+nWidth] == nMark)
				nOutF	=	nOutF+nWidth;
			else if(nOutF/nWidth	!= 0 && sznMark[nOutF-nWidth] == nMark)
				nOutF	=	nOutF-nWidth;

			else if(nOutF/nWidth	!= 0 && nOutF%nWidth	!= nWidth-1 && sznMark[nOutF-nWidth-1] == nMark)
				nOutF	=	nOutF-nWidth-1;
			else if(nOutF/nWidth	!= 0 && nOutF%nWidth	!= 0 && sznMark[nOutF-nWidth+1] == nMark)
				nOutF	=	nOutF-nWidth+1;
			else if(nOutF/nWidth	!= nHeight-1 && nOutF%nWidth	!= nWidth-1 && sznMark[nOutF+nWidth-1] == nMark)
				nOutF	=	nOutF+nWidth-1;
			else if(nOutF/nWidth	!= nHeight-1 && nOutF%nWidth	!= 0 && sznMark[nOutF+nWidth+1] == nMark)
				nOutF	=	nOutF+nWidth+1;

			else
			{
				delete[] sznMark;
				return false;
			}

			pListTmp->Insert (0,(void*)nOutF);
		}
	}

	if(nOutT != -1)
	{
		pListTmp->Add ((void*)nOutT);
		nMark	= sznMark[nOutT];
		while(--nMark > 10000)
		{
			if(nOutT%nWidth	!= nWidth-1 && sznMark[nOutT-1] == nMark)
				nOutT	=	nOutT-1;
			else if(nOutT%nWidth	!= 0 && sznMark[nOutT+1] == nMark)
				nOutT	=	nOutT+1;
			else if(nOutT/nWidth	!= nHeight-1 && sznMark[nOutT+nWidth] == nMark)
				nOutT	=	nOutT+nWidth;
			else if(nOutT/nWidth	!= 0 && sznMark[nOutT-nWidth] == nMark)
				nOutT	=	nOutT-nWidth;
			else if(nOutT/nWidth	!= 0 && nOutT%nWidth	!= nWidth-1 && sznMark[nOutT-nWidth-1] == nMark)
				nOutT	=	nOutT-nWidth-1;
			else if(nOutT/nWidth	!= 0 && nOutT%nWidth	!= 0 && sznMark[nOutT-nWidth+1] == nMark)
				nOutT	=	nOutT-nWidth+1;
			else if(nOutT/nWidth	!= nHeight-1 && nOutT%nWidth	!= nWidth-1 && sznMark[nOutT+nWidth-1] == nMark)
				nOutT	=	nOutT+nWidth-1;
			else if(nOutT/nWidth	!= nHeight-1 && nOutT%nWidth	!= 0 && sznMark[nOutT+nWidth+1] == nMark)
				nOutT	=	nOutT+nWidth+1;


			else
			{
				delete[] sznMark;
				return false;
			}

			pListTmp->Add ((void*)nOutT);
		}
	}


	int nCnt	= pListTmp->Count;
	POINT*	ppPtRtl	= new POINT[nCnt];
	int nIndex	= 0;
	while(pListTmp->Count)
	{
		int nNum	= (int)pListTmp->Delete (0);
		int	nNumY	= nNum / nWidth;
		int nNumX	= nNum % nWidth;
		ppPtRtl[nIndex].x	= (nNumX+nSX)*BLOCKING_SIZE + BLOCKING_SIZE/2;
		ppPtRtl[nIndex].y	= (nNumY+nSY)*BLOCKING_SIZE + BLOCKING_SIZE/2;
		nIndex++;
	}

	POINT* pCheckPT	= NULL;
	for (int i = 0; i < nCnt; i++)
	{
		if(i == nCnt-1)
		{
			POINT* pt	= new POINT;
			pt->x	= ppPtRtl[i].x;
			pt->y	= ppPtRtl[i].y;

			m_pPathRtl->Add (pt);
		}
		else if(pCheckPT	== NULL)
			pCheckPT	= &ppPtRtl[i];
		else
		{
			if(m_pBarrier->CheckLineBlock(pCheckPT->x,pCheckPT->y,ppPtRtl[i].x,ppPtRtl[i].y))
			{
				POINT* pt	= new POINT;
				pt->x	= ppPtRtl[i].x;
				pt->y	= ppPtRtl[i].y;
				m_pPathRtl->Add (pt);
				pCheckPT	= &ppPtRtl[i];
			}
		}
	}
	//delete (POINT*)m_pPathRtl->Delete (0);

	pListTmp->Free ();
	delete[] sznMark;
	delete[] ppPtRtl;
	return true;
}

#define DFUN_SETAXVALF(mark,pos,val)	\
{		\
	if(mark == 0)				\
	{						\
		mark	= val;		\
		pListAxFEx->Add ((void*)(pos));	\
	}						\
	else if(mark > 10000)		\
	{						\
		nOutTx	= pos;		\
		bOK	= true;		\
	}				\
}

#define DFUN_SETAXVALT(mark,pos,val)	\
{		\
	if(mark == 0)				\
	{						\
		mark	= val;		\
		pListAxTEx->Add ((void*)(pos));	\
	}						\
	else if(mark < 10000 && mark > 0)		\
	{						\
		nOutFx	= pos;		\
		bOK	= true;		\
	}				\
}


bool MapFindPath::_doubleAx(int* sznMark, int nX1, int nY1, int nX2, int nY2, int nW, int nH, int& nOutFx, int& nOutTx )
{
	xnList*		pListAxF	= xnList::Create ();
	xnList*		pListAxT	= xnList::Create ();
	xnList*		pListAxFEx	= xnList::Create ();
	xnList*		pListAxTEx	= xnList::Create ();

	pListAxF->Add ((void*)(nY1*nW+nX1));
	pListAxT->Add ((void*)(nY2*nW+nX2));

	sznMark[nY1*nW+nX1]	= 1;
	sznMark[nY2*nW+nX2]	= 10001;
	int		nValAxF	= 1;
	int		nValAxT	= 10001;
	bool	bSpecialDir	= false;

	while(pListAxF->Count && pListAxT->Count )
	{
		bSpecialDir	= !bSpecialDir;
		bool	bOK	= false;
		nValAxF++;
		nValAxT++;
		while(pListAxF->Count)
		{
			int nT	= (int)pListAxF->Delete (0);
			if(nT%nW	< nW-1)
				DFUN_SETAXVALF(sznMark[nT+1],nT+1,nValAxF);
			if(nT%nW	> 0)
				DFUN_SETAXVALF(sznMark[nT-1],nT-1,nValAxF);
			if(nT/nW	< nH-1)
				DFUN_SETAXVALF(sznMark[nT+nW],nT+nW,nValAxF);
			if(nT/nW	> 0)
				DFUN_SETAXVALF(sznMark[nT-nW],nT-nW,nValAxF);

			if(bSpecialDir)
			{//斜向找,权值1.5
				if(nT/nW > 0)
				{
					if(nT%nW < nW-1)
						DFUN_SETAXVALF(sznMark[nT-nW+1],nT-nW+1,nValAxF);
					if(nT%nW	> 0)
						DFUN_SETAXVALF(sznMark[nT-nW-1],nT-nW-1,nValAxF);
				}
				if(nT/nW	< nH-1)
				{
					if(nT%nW < nW-1)
						DFUN_SETAXVALF(sznMark[nT+nW+1],nT+nW+1,nValAxF);
					if(nT%nW	> 0)
						DFUN_SETAXVALF(sznMark[nT+nW-1],nT+nW-1,nValAxF);
				}
			}

			nOutFx	= nT;

			if(bOK)
			{
				pListAxF->Free ();
				pListAxT->Free ();
				pListAxFEx->Free ();
				pListAxTEx->Free ();
				return true;
			}
		}


		xnList* pListTmp	= pListAxFEx;
		pListAxFEx	= pListAxF;
		pListAxF	= pListTmp;

		while(pListAxT->Count)
		{
			int nT	= (int)pListAxT->Delete (0);

			if(nT%nW	< nW-1)
				DFUN_SETAXVALT(sznMark[nT+1],nT+1,nValAxT);
			if(nT%nW	> 0)
				DFUN_SETAXVALT(sznMark[nT-1],nT-1,nValAxT);
			if(nT/nW	< nH-1)
				DFUN_SETAXVALT(sznMark[nT+nW],nT+nW,nValAxT);
			if(nT/nW	> 0)
				DFUN_SETAXVALT(sznMark[nT-nW],nT-nW,nValAxT);
			if(bSpecialDir)
			{//斜向找,权值1.5
				if(nT/nW > 0)
				{
					if(nT%nW < nW-1)
						DFUN_SETAXVALT(sznMark[nT-nW+1],nT-nW+1,nValAxT);
					if(nT%nW	> 0)
						DFUN_SETAXVALT(sznMark[nT-nW-1],nT-nW-1,nValAxT);
				}
				if(nT/nW	< nH-1)
				{
					if(nT%nW < nW-1)
						DFUN_SETAXVALT(sznMark[nT+nW+1],nT+nW+1,nValAxT);
					if(nT%nW	> 0)
						DFUN_SETAXVALT(sznMark[nT+nW-1],nT+nW-1,nValAxT);
				}
			}
			nOutTx	= nT;

			if(bOK)
			{
				pListAxF->Free ();
				pListAxT->Free ();
				pListAxFEx->Free ();
				pListAxTEx->Free ();
				return true;
			}
		}

		pListTmp	= pListAxTEx;
		pListAxTEx	= pListAxT;
		pListAxT	= pListTmp;

	}

	pListAxF->Free ();
	pListAxT->Free ();
	pListAxFEx->Free ();
	pListAxTEx->Free ();
	return false;
}

void MapFindPath::_initBlock( int * szMark,int nSX,int nSY, int nW,int nH )
{
	char* szBlock	= m_pBarrier->m_pData;
	int nLimitW	= m_pBarrier->m_nWidth/BLOCKING_SIZE;
	int nLimitH	= m_pBarrier->m_nHeight/BLOCKING_SIZE;
	for (int y = nSY; y < nSY+nH; y ++)
	{
		for (int x	= nSX; x < nSX+nW; x++)
		{
			if ((x < 0) || (y < 0) || (x >= nLimitW) || (y >= nLimitH))
				szMark[(y-nSY)*nW+x-nSX]	= -1;
			else
			{
				BYTE k = szBlock[((nLimitW+7)/8) * y + x / 8];
				BYTE u = (1 << 7) >> (x % 8);
				if(k&u)
					szMark[(y-nSY)*nW+x-nSX]	= -1;
				else
					szMark[(y-nSY)*nW+x-nSX]	= 0;
			}
		}
	}
}

_TransPoint* MapFindPath::popTransPoint(int nMapIndex)
{
	if(m_pMapTransPt->Count)
	{
		_TransPoint* pTP	= (_TransPoint*)m_pMapTransPt->Items[m_pMapTransPt->Count-1];
		if(pTP->nMapindex == nMapIndex)
		{
			m_pMapTransPt->Delete (m_pMapTransPt->Count-1);
			return pTP;
		}
	}

	return NULL;
}

void MapFindPath::InitBarrier( MapBarrier* pBarrier )
{
	m_pBarrier		= pBarrier;
}
