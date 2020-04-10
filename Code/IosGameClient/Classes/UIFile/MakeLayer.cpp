#include "MakeLayer.h"
#include "EffectSprite.h"
#include "EffectObj.h"
static char szIconTxt[8][32] = {"ui/MB_SANXIAO1.png","ui/MB_SANXIAO2.png","ui/MB_SX1.png","ui/MB_SX2.png"
							,"ui/MB_SX3.png","ui/MB_SX4.png","ui/BHJZ3.png","ui/BHJZ4.png"};
bool MakeLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCRect rect = CCRect(0,0,nMaxWidth*nTileSize,nMaxHeight*nTileSize);
	if (!rect.containsPoint(convertToNodeSpace(pTouch->getLocation())))
	{
		return false;
	}
	if (m_bExchange)
	{
		return true;
	}
	if (CCClainSprite::nFallCount)
	{
		return true;
	}
	if (CCClainSprite::nCleanCount)
	{
		return true;
	}
	m_bClicked = true;
	m_ClickPoint = convertToNodeSpace(pTouch->getLocation());
	return true;
}



void MakeLayer::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if (m_bClicked)
	{
		CCPoint nowPoint = convertToNodeSpace(pTouch->getLocation());
		if (((int)nowPoint.x/nTileSize)>((int)m_ClickPoint.x/nTileSize)||((int)nowPoint.x/nTileSize)<((int)m_ClickPoint.x/nTileSize))
		{
			_exchange(m_ClickPoint.x/nTileSize,m_ClickPoint.y/nTileSize,nowPoint.x/nTileSize,m_ClickPoint.y/nTileSize);
			m_bClicked = false;
		}else if (((int)nowPoint.y/nTileSize)>((int)m_ClickPoint.y/nTileSize)||((int)nowPoint.y/nTileSize)<((int)m_ClickPoint.y/nTileSize))
		{
			_exchange(m_ClickPoint.x/nTileSize,m_ClickPoint.y/nTileSize,m_ClickPoint.x/nTileSize,nowPoint.y/nTileSize);
			m_bClicked = false;
		}
	}
}


void MakeLayer::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	m_bClicked = false;
}


 void MakeLayer::registerWithTouchDispatcher()
 {
 	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, -999, true);
 }

void MakeLayer::onEnter()
{
	CCLayer::onEnter();
inittile:
	_initTile();
	list<_tileGroup> resultList;
	for (int i=0;i<nMaxWidth-1;i++)
	{
		for (int j=0;j<nMaxHeight-1;j++)
		{
			_getTileGroup(i,j,resultList);
			if (resultList.size())
			{
				goto befound;	
			}
		}
	}
	goto inittile;
befound:
	;
}

void MakeLayer::_initTile()
{
	BYTE byType[nMaxWidth][nMaxHeight] = {0};
	for (int i=0;i<nMaxWidth;i++)
	{
		for (int j=0;j<nMaxHeight;j++)
		{
			byType[i][j] = rand()%6;
			if (j>=2&&byType[i][j] == byType[i][j-1]&&byType[i][j] == byType[i][j-2])
			{
				j--;
				continue;
			}else if (i>=2&&byType[i][j] == byType[i-1][j]&&byType[i][j] == byType[i-2][j])
			{
				j--;
				continue;
			}
		}
	}
	for (int i=0;i<nMaxWidth;i++)
	{
		for (int j=0;j<nMaxHeight;j++)
		{
			if(!m_pTile[i][j]){
				m_pTile[i][j] = new CCClainSprite;
				m_pTile[i][j]->m_byTile = byType[i][j];
				m_pTile[i][j]->initWithFile();
				addChild(m_pTile[i][j]);
			}else{
				m_pTile[i][j]->m_byTile = byType[i][j];
				m_pTile[i][j]->initWithFile();
			}
			//m_pTile[i][j]->m_byTile = m_byTile[i][j];

			//m_pTile[i][j]->setAnchorPoint(ccp(0,0));
			m_pTile[i][j]->stopAllActions();
			m_pTile[i][j]->setPosition(ccp(i*nTileSize,j*nTileSize));
		}
	}
	CCClainSprite::nFallCount = 0;
	CCClainSprite::nCleanCount = 0;
	nExchangeCount = 0;
}

void MakeLayer::_exchange( int nFormX,int nFormY,int nToX,int nToY )
{
	if (nFormX<0||nFormX>=nMaxWidth||nToX<0||nToX>=nMaxWidth)
	{
		return;
	}
	if (nFormY<0||nFormY>=nMaxWidth||nToY<0||nToY>=nMaxWidth)
	{
		return;
	}
	m_bExchange = true;
	if (_checkExchange(nFormX,nFormY,nToX,nToY))
	{
// 		BYTE byTemp = m_pTile[nFormX][nFormY]->m_byTile;
// 		m_byTile[nFormX][nFormY] = m_byTile[nToX][nToY];
// 		m_byTile[nToX][nToY] = byTemp;

		CCActionInterval * pJump1	= CCMoveTo::create(0.2f,m_pTile[nFormX][nFormY]->getPosition());
		CCActionInterval * pJump2	= CCMoveTo::create(0.2f,m_pTile[nToX][nToY]->getPosition());

		CCAction * pCallback1 = CCCallFunc::create(this, callfunc_selector(MakeLayer::_afterExchange)); 
		CCAction * pCallback2 = CCCallFunc::create(this, callfunc_selector(MakeLayer::_afterExchange)); 
		CCFiniteTimeAction* pRun1	= CCSequence::create (pJump1,pCallback1,NULL);
		CCFiniteTimeAction* pRun2	= CCSequence::create (pJump2,pCallback2,NULL);

		m_pTile[nFormX][nFormY]->runAction(pRun2);
		m_pTile[nToX][nToY]->runAction(pRun1);

		nExchangeCount+=2;

		CCClainSprite* pSprite = m_pTile[nFormX][nFormY];
		m_pTile[nFormX][nFormY] = m_pTile[nToX][nToY];
		m_pTile[nToX][nToY] = pSprite;
	}else{
		CCPoint midPoint = ccpMidpoint(m_pTile[nFormX][nFormY]->getPosition(),m_pTile[nToX][nToY]->getPosition());
		CCActionInterval * pJumpTo1	= CCMoveTo::create(0.1f,midPoint);
		CCActionInterval * pJumpBack1 = CCMoveTo::create(0.1f,m_pTile[nFormX][nFormY]->getPosition());
		CCActionInterval * pJumpTo2	= CCMoveTo::create(0.1f,midPoint);
		CCActionInterval * pJumpBack2 = CCMoveTo::create(0.1f,m_pTile[nToX][nToY]->getPosition());

		CCAction * pCallback = CCCallFunc::create(this, callfunc_selector(MakeLayer::_afterExchangeFailed)); 
		CCFiniteTimeAction* pRun1	= CCSequence::create (pJumpTo1,pJumpBack1,pCallback,NULL);
		CCFiniteTimeAction* pRun2	= CCSequence::create (pJumpTo2,pJumpBack2,NULL);

		m_pTile[nToX][nToY]->runAction(pRun2);
		m_pTile[nFormX][nFormY]->runAction(pRun1);
	}
	
}

MakeLayer::MakeLayer()
{
	memset(m_pTile,0,sizeof(CCClainSprite*)*nMaxHeight*nMaxWidth);
	setTouchEnabled (true);
	
	m_bExchange = false;
	scheduleUpdate();
	nTileSize = 90/POINT_PIX_CNT;
}

MakeLayer::~MakeLayer()
{
}

void MakeLayer::_afterExchange()
{
	m_bExchange = false;
	nExchangeCount--;
	if (!nExchangeCount)
	{
		CleanUp();
	}
}

bool FindTileByX(CCClainSprite* pSpr,int nX){
	return ((int)pSpr->getPositionX()/MakeLayer::nTileSize) == nX;
}

bool MakeLayer::_checkExchange( int nFormX,int nFormY,int nToX,int nToY )
{
	CCClainSprite* byTemp = m_pTile[nFormX][nFormY];
	m_pTile[nFormX][nFormY] = m_pTile[nToX][nToY];
	m_pTile[nToX][nToY] = byTemp;

	list<BYTE> listTile;

	if(nFormX>1) listTile.push_back(m_pTile[nFormX-2][nFormY]->m_byTile);
	if(nFormX>0) listTile.push_back(m_pTile[nFormX-1][nFormY]->m_byTile);
	listTile.push_back(m_pTile[nFormX][nFormY]->m_byTile);
	if(nFormX<nMaxWidth-1) listTile.push_back(m_pTile[nFormX+1][nFormY]->m_byTile);
	if(nFormX<nMaxWidth-2) listTile.push_back(m_pTile[nFormX+2][nFormY]->m_byTile);
	bool nRet = _checkClain(listTile);
	if (nRet)
	{
		goto returnval;
	}
	listTile.clear();

	if(nFormY>1) listTile.push_back(m_pTile[nFormX][nFormY-2]->m_byTile);
	if(nFormY>0) listTile.push_back(m_pTile[nFormX][nFormY-1]->m_byTile);
	listTile.push_back(m_pTile[nFormX][nFormY]->m_byTile);
	if(nFormY<nMaxHeight-1) listTile.push_back(m_pTile[nFormX][nFormY+1]->m_byTile);
	if(nFormY<nMaxHeight-2) listTile.push_back(m_pTile[nFormX][nFormY+2]->m_byTile);
	nRet = _checkClain(listTile);
	if (nRet)
	{
		goto returnval;
	}
	listTile.clear();


	if(nToX>1) listTile.push_back(m_pTile[nToX-2][nToY]->m_byTile);
	if(nToX>0) listTile.push_back(m_pTile[nToX-1][nToY]->m_byTile);
	listTile.push_back(m_pTile[nToX][nToY]->m_byTile);
	if(nToX<nMaxWidth-1) listTile.push_back(m_pTile[nToX+1][nToY]->m_byTile);
	if(nToX<nMaxWidth-2) listTile.push_back(m_pTile[nToX+2][nToY]->m_byTile);
	nRet = _checkClain(listTile);
	if (nRet)
	{
		goto returnval;
	}
	listTile.clear();


	if(nToY>1) listTile.push_back(m_pTile[nToX][nToY-2]->m_byTile);
	if(nToY>0) listTile.push_back(m_pTile[nToX][nToY-1]->m_byTile);
	listTile.push_back(m_pTile[nToX][nToY]->m_byTile);
	if(nToY<nMaxHeight-1) listTile.push_back(m_pTile[nToX][nToY+1]->m_byTile);
	if(nToY<nMaxHeight-2) listTile.push_back(m_pTile[nToX][nToY+2]->m_byTile);
	nRet = _checkClain(listTile);
returnval:
	byTemp = m_pTile[nFormX][nFormY];
	m_pTile[nFormX][nFormY] = m_pTile[nToX][nToY];
	m_pTile[nToX][nToY] = byTemp;
	return nRet;
}

bool MakeLayer::_checkClain( list<BYTE>& listTile )
{
	BYTE nCount = 1;
	BYTE temp = 255;
	for (list<BYTE>::iterator itor = listTile.begin();itor != listTile.end();itor++)
	{
		if (*itor != temp)
		{
			temp = *itor;
			nCount = 1;
		}else{
			nCount++;
			if (nCount>=3)
			{
				return true;
			}
		}
	}
	return false;
}

void MakeLayer::_afterExchangeFailed()
{
	m_bExchange = false;
}

void MakeLayer::_getClainTile( list<CCClainSprite*>& fromListTile,list<CCClainSprite*>& toStackTile )
{
	BYTE nCount = 0;
	BYTE temp = TILE_NULL;
	for (list<CCClainSprite*>::iterator itor = fromListTile.begin();itor != fromListTile.end();itor++)
	{
		if ((*itor)->m_byTile != temp)
		{
			temp = (*itor)->m_byTile;
			while(nCount>0&&nCount<3){
				nCount--;
				if(toStackTile.size()) toStackTile.pop_back();
			}
			nCount = 1;
			toStackTile.push_back(*itor);
		}else{
			nCount++;
			toStackTile.push_back(*itor);
		}
	}
	while(nCount>0&&nCount<3){
		nCount--;
		if(toStackTile.size()) toStackTile.pop_back();
	}
}

void MakeLayer::CleanUp()
{
	list<CCClainSprite*> listTile;
	list<CCClainSprite*> resultTile;
	for (int i=0;i<nMaxWidth;i++)
	{
		for (int j=0;j<nMaxHeight;j++)
		{
			listTile.push_back(m_pTile[i][j]);
		}
		_getClainTile(listTile,resultTile);
		listTile.clear();
	}

	for (int i=0;i<nMaxHeight;i++)
	{
		for (int j=0;j<nMaxWidth;j++)
		{
			listTile.push_back(m_pTile[j][i]);
		}
		_getClainTile(listTile,resultTile);
		listTile.clear();
	}
	size_t nSize = resultTile.size();

	while (resultTile.size())
	{
		CleanUp(resultTile.back());
		resultTile.pop_back();
	}

	if (!nSize)
	{
		_checkPos();
		list<_tileGroup> resultList;
		for (int i=0;i<nMaxWidth-1;i++)
		{
			for (int j=0;j<nMaxHeight-1;j++)
			{
				_getTileGroup(i,j,resultList);
				if (resultList.size())
				{
					goto befound;	
				}
			}
		}
befound:
		if (resultList.size())
		{
			//_tileGroup& tstep = resultList.front();
			//_exchange(tstep.x,tstep.y,tstep.x+((tstep.byDirect%2)?(tstep.byDirect/2):(-tstep.byDirect/2)),tstep.y+((tstep.byDirect==0)?1:(tstep.byDirect==1?(-1):0)));
		}else{
			m_fnCallBackEvent(1,0);
		}
	}
}

void MakeLayer::CleanUp( CCClainSprite* pTile )
{
	//pTile->stopAllActions();
	{
		CCFadeOut* pFade = CCFadeOut::create(0.2f);
		CCCallFuncND * pCallback = CCCallFuncND::create(this, callfuncND_selector(MakeLayer::_addResetSpr),pTile); 
		CCFiniteTimeAction* pRun	= CCSequence::create (pFade,pCallback,NULL);
		pTile->runAction(pRun);
		AddEff(pTile->getPositionX(),pTile->getPositionY());

		//pTile->RunFadeOutEffect();
	}


	int nX = (int)pTile->getPositionX()/nTileSize;
	int nY = (int)pTile->getPositionY()/nTileSize;

	//m_byTile[nX][nY] = TILE_NULL;
	m_pTile[nX][nY] = NULL;
	CCClainSprite::nCleanCount++;

}

void MakeLayer::_addResetSpr( CCNode* pNode,void* pParam )
{
	CCClainSprite* pSpr = (CCClainSprite*)pParam;
	m_ResetSpr.push_back(pSpr);
	CCClainSprite::nCleanCount--;
	m_fnCallBackEvent(0,((CCClainSprite*)pParam)->m_byTile);
}

void MakeLayer::update( float dt )
{
	_resetSpr();
	for (std::list<CCEffectSprite*>::iterator itor = m_pEff.begin();itor !=m_pEff.end();itor++)
	{
		CCEffectSprite* pSpr = (*itor);
		if (pSpr->CheckEnd())
		{
			removeChild(pSpr,false);
			itor = m_pEff.erase(itor);
			break;
			//pSpr->release();
		}
	}
}

void MakeLayer::_resetSpr()
{
	if (m_ResetSpr.size())
	{
		int nNullCount[nMaxWidth] = {0};
		for (int i=0;i<nMaxWidth;i++)
		{
			int nCountNull = 0;
			for (int j=0;j<nMaxHeight;j++)
			{
				if (m_pTile[i][j] == NULL)
				{
					nCountNull++;
				}else if(nCountNull){
					//m_byTile[i][j-nCountNull] = m_byTile[i][j];
					CCAssert(!m_pTile[i][j-nCountNull],"Tile Error");
					m_pTile[i][j-nCountNull] = m_pTile[i][j];
					m_pTile[i][j] = NULL;
					m_pTile[i][j-nCountNull]->SetFallDown(nCountNull);
				}
			}
			nNullCount[i] = nCountNull;
		}
		m_ResetSpr.unique();
		int nNullCountTemp[nMaxWidth];
		memcpy(nNullCountTemp,nNullCount,sizeof(int)*nMaxWidth);
		for (list<CCClainSprite*>::iterator itor = m_ResetSpr.begin();itor != m_ResetSpr.end();itor++)
		{
			int nX = (*itor)->getPositionX()/nTileSize;
			int nY = nMaxHeight-(nNullCount[nX]-nNullCountTemp[nX])-1;
			CCAssert(nNullCount[nX],"Null Count error");
			CCAssert(!m_pTile[nX][nY],"Tile Error");
			(*itor)->setPositionY(nTileSize*(nNullCountTemp[nX]+nMaxHeight-1));
			(*itor)->SetFallDown(nNullCount[nX]);
			(*itor)->setOpacity(255);


			m_pTile[nX][nY] = (*itor);
			m_pTile[nX][nY]->m_byTile = rand()%6;
			m_pTile[nX][nY]->initWithFile();
			//m_pTile[nX][nY]->m_byTile = m_byTile[nX][nY];

			nNullCountTemp[nX]--;
		}
		m_ResetSpr.clear();
	}
}

void MakeLayer::_getTileGroup( int x,int y,list<_tileGroup>& resultList )
{
	if (x>=nMaxWidth-1||y>=nMaxHeight-1)
	{
		return;
	}
	for (int i=0;i<4;i++)
	{
		BYTE byType1 = m_pTile[x+i%2][y+i/2]->m_byTile;
		//	0		1		2		3
		//	11		12		12		21
		//	23		13		31		13
		for (int j=i+1;j<4;j++)
		{
			BYTE byType2 = m_pTile[x+j%2][y+j/2]->m_byTile;
			if (byType1 == byType2)
			{
				if ((x+i%2 )== (x+j%2))
				{
					//1
					int nymin = (y+j/2)>(y+i/2)?(y+i/2):(y+j/2);
					int nymax = (y+j/2)<(y+i/2)?(y+i/2):(y+j/2);
					if(_getTileType(x+i%2-1,nymax+1) == byType1) resultList.push_back(_tileGroup(x+i%2-1,nymax+1,DIRECT_EAST));
					if(_getTileType(x+i%2,nymax+2) == byType1) resultList.push_back(_tileGroup(x+i%2,nymax+2,DIRECT_SOUTH));
					if(_getTileType(x+i%2+1,nymax+1) == byType1) resultList.push_back(_tileGroup(x+i%2+1,nymax+1,DIRECT_WEST));
					if(_getTileType(x+i%2-1,nymin-1) == byType1) resultList.push_back(_tileGroup(x+i%2-1,nymin-1,DIRECT_EAST));
					if(_getTileType(x+i%2,nymin-2) == byType1) resultList.push_back(_tileGroup(x+i%2,nymin-2,DIRECT_NORTH));
					if(_getTileType(x+i%2+1,nymin-1) == byType1) resultList.push_back(_tileGroup(x+i%2+1,nymin-1,DIRECT_WEST));
				}else	if ((y+i/2) == (y+j/2))
				{
					//0
					int nxmin = (x+i%2)>(x+j%2)?(x+j%2):(x+i%2);
					int nxmax = (x+i%2)<(x+j%2)?(x+j%2):(x+i%2);
					if(_getTileType(nxmin-1,y+j/2-1) == byType1) resultList.push_back(_tileGroup(nxmin-1,y+j/2-1,DIRECT_NORTH));
					if(_getTileType(nxmin-2,y+j/2) == byType1) resultList.push_back(_tileGroup(nxmin-2,y+j/2,DIRECT_EAST));
					if(_getTileType(nxmin-1,y+j/2+1) == byType1) resultList.push_back(_tileGroup(nxmin-1,y+j/2+1,DIRECT_SOUTH));
					if(_getTileType(nxmax+1,y+j/2-1) == byType1) resultList.push_back(_tileGroup(nxmax+1,y+j/2-1,DIRECT_NORTH));
					if(_getTileType(nxmax+2,y+j/2) == byType1) resultList.push_back(_tileGroup(nxmax+2,y+j/2,DIRECT_WEST));
					if(_getTileType(nxmax+1,y+j/2+1) == byType1) resultList.push_back(_tileGroup(nxmax+1,y+j/2+1,DIRECT_SOUTH));
				}else	if((j-i) == 1){
					//2
					if(_getTileType(x+i%2-2,y+i/2) == byType1) resultList.push_back(_tileGroup(x+j%2,y+j/2,DIRECT_SOUTH));
					if(_getTileType(x+i%2-1,y+i/2-1) == byType1) resultList.push_back(_tileGroup(x+i%2,y+i/2,DIRECT_WEST));
					if(_getTileType(x+i%2+1,y+i/2+1) == byType1) resultList.push_back(_tileGroup(x+i%2,y+i/2,DIRECT_NORTH));
					if(_getTileType(x+i%2,y+i/2+2) == byType1) resultList.push_back(_tileGroup(x+j%2,y+j/2,DIRECT_EAST));
				}else if ((j-i)==3)
				{
					if(_getTileType(x+i%2-1,y+i/2+1) == byType1) resultList.push_back(_tileGroup(x+i%2,y+i/2,DIRECT_NORTH));
					if(_getTileType(x+i%2,y+i/2+2) == byType1) resultList.push_back(_tileGroup(x+j%2,y+j/2,DIRECT_WEST));
					if(_getTileType(x+i%2+1,y+i/2-1) == byType1) resultList.push_back(_tileGroup(x+i%2,y+i/2,DIRECT_EAST));
					if(_getTileType(x+i%2+2,y+i/2) == byType1) resultList.push_back(_tileGroup(x+j%2,y+j/2,DIRECT_SOUTH));
				}
			}
		}
	}
}

BYTE MakeLayer::_getTileType( int x,int y )
{
	if (x<0||x>=nMaxWidth||y<0||y>=nMaxHeight)
	{
		return TILE_NULL;
	}
	return m_pTile[x][y]->m_byTile;
}

void MakeLayer::_checkPos()
{
	static int nCheckCount = 0;
	++nCheckCount;
	for (int i=0;i<nMaxWidth;i++)
	{
		for (int j=0;j<nMaxHeight;j++)
		{
			CCPoint nPoint= m_pTile[i][j]->getPosition();
			CCAssert(nPoint.x/nTileSize==i&&nPoint.y/nTileSize==j,"position error");
		}
	}
}

void MakeLayer::RandomMake()
{
	_initTile();
}

void MakeLayer::SetType( BYTE byType,void (*fnCallBackEvent)(int nEvent,int nVal) )
{
	char sztxt[32];
	sprintf(sztxt,"ui/MB_SANXIAO%d.png",byType+1);
	strcpy(szIconTxt[0] ,sztxt);
	sprintf(sztxt,"ui/MB_SANXIAO%d.png",byType+6);
	strcpy(szIconTxt[1] ,sztxt);

	m_fnCallBackEvent = fnCallBackEvent;
}

void MakeLayer::AddEff( int nX,int nY )
{
	CCEffectSprite* m_pEffSpr = CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(263));
	addChild(m_pEffSpr,1);
	//m_pEffSpr->setAnchorPoint(ccp(0,0));
	m_pEffSpr->setPosition(ccp(nX+nTileSize/2,nY));
	m_pEff.push_back(m_pEffSpr);
}

int MakeLayer::nTileSize = 90;

int MakeLayer::nExchangeCount = 0;

void CCClainSprite::SetFallDown( int nDistance )
{
	//m_pEffSpr->setVisible(false);
	CCMoveBy* pAction = CCMoveBy::create(0.2f,ccp(0,-nDistance*MakeLayer::nTileSize));
	CCAction * pCallback = CCCallFunc::create(this, callfunc_selector(CCClainSprite::_onFallCallBack)); 
	CCFiniteTimeAction* pRun	= CCSequence::create (pAction,pCallback,NULL);
	runAction(pRun);
	nFallCount++;
}

bool CCClainSprite::initWithFile(  )
{
	bool nRet = CCSprite::initWithFile(szIconTxt[m_byTile]);
	setAnchorPoint(ccp(0,0));
// 	if (!m_pEffSpr->getParent())
// 	{
// 		addChild(m_pEffSpr);
// 		m_pEffSpr->setVisible(false);
// 	}
	return nRet;
}

void CCClainSprite::_onFallCallBack()
{
	nFallCount--;
	if (nFallCount == 0)
	{
		MakeLayer* pLayer = (MakeLayer*)getParent();
		pLayer->CleanUp();
	}
}

CCClainSprite::CCClainSprite()
{
	//m_pEffSpr = CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(263));
}

void CCClainSprite::RunFadeOutEffect()
{
	//m_pEffSpr->setVisible(true);
	//m_pEffSpr->InitSprAction(-1);
	//m_pEffSpr->runAction(m_pEffSpr->m_pAnimate);
}

int CCClainSprite::nFallCount = 0;
int CCClainSprite::nCleanCount = 0;
