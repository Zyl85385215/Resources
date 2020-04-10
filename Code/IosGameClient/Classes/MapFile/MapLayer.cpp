#include "MapLayer.h"
#include "MapControl.h"
#include "MapTile.h"
#include "MapSprite.h"
#include "UserData.h"
#include "MainMenu.h"
#include "MapMouse.h"
#include "EffectSprite.h"
#include "GuideLayer.h"
#include "MapStaticCfg.h"
#include "MObjActControl.h"
//#include "ParticleLayer.h"
#include "ResManage.h"

#ifdef WIN32
#include "debug_def.h"
#endif

MapLayer*	pMapLayer	= NULL;
using namespace	cocos2d;

bool MapLayer::init()
{
	CCLayer::init();

	scheduleUpdate();

	setTouchEnabled(true);

	return true;
}


MapLayer::MapLayer()
{
	m_pLTileOld		= xnList::Create();
	m_pLMObjOld		= xnList::Create();
	m_pLSprEff		= xnList::Create();
	m_pListGroundEff	= xnList::Create();
	m_pMapEvent		= new MapEventCenter();
	m_bClickHold	= false;
	m_nRunMIndex	= 0;
}


MapLayer::~MapLayer()
{
	m_pLTileOld->Free();
	m_pLMObjOld->Free();
	m_pLSprEff->Free();

	while(m_pListGroundEff->Count)
	{
		CCSprite* pSpr	= (CCSprite*)m_pListGroundEff->Delete(0);
		pSpr->release();
	}
	m_pListGroundEff->Free();
	delete m_pMapEvent;

}

void MapLayer::registerWithTouchDispatcher()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, 10, true);
}

bool MapLayer::ccTouchBegan( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(isVisible() == false)
		return false;
	if(pMainInterface->m_pLockForm)
		return false;
	if(xnGetTickCount() - g_pMapCenter->m_dwTickEnter < 1500)
		return false;	//跳地图延迟时间
	//g_ptLastClick	= pTouch->getLocation ();
	//m_ptTouched = pTouch->getLocation ();
	m_ptClick		= pTouch->getLocation();
	CCPoint pt	= ccpSub (m_ptClick,getPosition ());
	if(nowRateW&& nowRateH)
	{
		pt.x	/=	nowRateW;
		pt.y	/=	nowRateH;
	}
	int nX, nY;
	PtCocos2Map(pt,nX,nY);
	
	m_bClickHold	= m_pMapEvent->MapEventDown(nX,nY);
	return true;
}

void MapLayer::ccTouchEnded( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	CCPoint pt	= ccpSub (pTouch->getLocation (),getPosition ());
	if(nowRateW&& nowRateH)
	{
		pt.x	/=	nowRateW;
		pt.y	/=	nowRateH;
	}
	int nX, nY;
	PtCocos2Map(pt,nX,nY);
	
	m_pMapEvent->MapEventUp(nX,nY);

	m_bClickHold	= false;
}

void MapLayer::ccTouchMoved( cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent )
{
	if(m_bClickHold)
	{
		m_ptClick		= pTouch->getLocation();
		CCPoint pt	= ccpSub (m_ptClick,getPosition ());
		if(nowRateW&& nowRateH)
		{
			pt.x	/=	nowRateW;
			pt.y	/=	nowRateH;
		}
		int nX, nY;
		PtCocos2Map(pt,nX,nY);
		m_pMapEvent->MapEventUp(nX,nY);
	}
}

void MapLayer::TryUpdate()
{
	if(g_pMapCenter->m_nBaseMapIndex == 0)
		return;

	//m_pLayerTile->setVisible(!pMainInterface->m_pHideBackForm);
	//m_pLayerMObj->setVisible(!pMainInterface->m_pHideBackForm);

	if(m_nRunMIndex != g_pMapCenter->m_nMapIndex)
	{
		InitMap();
		m_nRunMIndex	= g_pMapCenter->m_nMapIndex;
	}
	//pChar->Update();
	//pMainMenu->Update();
	g_pMapCenter->Update ();

	CCPoint ptCur	= PtMap2Cocos (g_pMapCenter->m_nX,g_pMapCenter->m_nY);
	if(nowRateW&& nowRateH)
		setPosition (-ptCur.x*nowRateW,-ptCur.y*nowRateH);
	else
		setPosition (-ptCur.x,-ptCur.y);

	_updateControl();

	_updateTile();
	_updateMObj();
	_updateEffs();
	_updateOther();
}

void MapLayer::update( float dt )
{
	CCLayer::update (dt);

#ifdef WIN32

	TryUpdate();

#else
	try
	{
		TryUpdate();
	}
	catch (...)
	{

	}
#endif
}

cocos2d::CCPoint MapLayer::PtMap2Cocos( int nX, int nY )
{
	return ccp(nX/POINT_PIX_CNT, (g_pMapCenter->GetMapHeight() - nY)/POINT_PIX_CNT);
}

bool MapLayer::PtCocos2Map( cocos2d::CCPoint ptIn, int& nX, int& nY )
{
	nX	= ptIn.x*POINT_PIX_CNT;
	nY	= g_pMapCenter->GetMapHeight()-ptIn.y*POINT_PIX_CNT;
	return true;
}

void MapLayer::onEnter()
{
	CCLayer::onEnter ();

	m_pLayerTile	= CCLayer::create();
	m_pLayerTile->setPosition(0,0);
	m_pLayerTile->setAnchorPoint(ccp(0,0));
	addChild(m_pLayerTile,1,1);
	m_pLayerMObj	= CCLayer::create();
	addChild(m_pLayerMObj,2,2);
	//m_pLayerParticle	= ParticleLayer::create();
	//addChild(m_pLayerParticle,10,10);

	setAnchorPoint(ccp(0,0));
	//g_pMapCenter->EnterMap(2,2000,1800);


	//m_pSprSel	= CCSprite::create("ui/MB_XZ_P.png");
	//m_pSprSel->retain();

	//CCActionInterval* pScaleS	= CCScaleTo::create(0.5,1.1f);
	//CCActionInterval* pScaleE	= CCScaleTo::create(0.5,0.9f);
	//CCActionInterval*	pSeq	= (CCActionInterval*)CCSequence::create(pScaleS,pScaleE,NULL);
	//m_pSprSel->runAction(CCRepeatForever::create(pSeq));
	m_pEffMove	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(3));
	m_pEffMove->retain();
	m_pEffSel	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(1));
	m_pEffSel->retain();


	addChild(m_pEffMove,5,5);

	if(nowRateW && nowRateH){
		this->setScaleX(nowRateW);
		this->setScaleY(nowRateH);
	}

}

void MapLayer::onExit()
{
	CCLayer::onExit ();

	//m_pSprSel->release();
	m_pEffMove->release();
	m_pEffSel->release();
	//释放SKIN
	//for (int i = 0; i < CfgObjSkin::GetInstance ().m_pList->Count; i++)
	//{
	//	ObjSkinInfo*	pSkin	= (ObjSkinInfo*)CfgObjSkin::GetInstance ().m_pList->Items[i];
	//	if(pSkin->bUsed)
	//	{
	//		for (int i = 0; i < pSkin->bPartCnt; i++)
	//		{
	//			char szList[256];
	//			sprintf(szList,"skin/%d/%d.plist",pSkin->nId,i+1);
	//			CCSpriteFrameCache::sharedSpriteFrameCache ()->removeSpriteFramesFromFile(szList);
	//		}
	//		pSkin->bUsed	= false;
	//	}
	//}

	////释放Eff
	//for (int i = 0; i < EffectCenter::GetInstance ().m_pList->Count; i++)
	//{
	//	EffectObj*	pEff	= (EffectObj*)EffectCenter::GetInstance ().m_pList->Items[i];
	//	if(pEff->m_bUsed)
	//	{
	//		char	szFrame[64];
	//		sprintf(szFrame,"effect/%d.plist",pEff->m_nRealSprID);
	//		CCSpriteFrameCache::sharedSpriteFrameCache ()->removeSpriteFramesFromFile(szFrame);
	//		pEff->m_bUsed	= false;
	//	}
	//}
	//for (int i = 0; i < m_pListOldTile->Count; i++)
	//{
	//	MapTileObj* pTile	= (MapTileObj*)m_pListOldTile->Items[i];
	//	pTile->pSpr	= NULL;
	//}

	//removeAllChildrenWithCleanup (true);
	//stopAllActions ();
}

void MapLayer::_updateTile()
{
	xnList*		pListNew	= g_pMapCenter->m_pListTile;

	if(pListNew->Count)
	{
		xnList* pListTmp	= xnList::Create ();
		if(m_pLTileOld->CheckMyOnly (pListNew,pListTmp))
		{
			for (int i = 0; i < pListTmp->Count; i++)
			{
				_mapTile* pTile	= (_mapTile*)pListTmp->Items[i];


				CMTileSprite* pMSpr	= (CMTileSprite*)m_pLayerTile->getChildByTag((int)pTile);
				if(pMSpr)
				{
					m_pLayerTile->removeChildByTag ((int)pTile,true);
					m_pLTileOld->Remove(pTile);
				}
				else if(pMSpr == NULL)
				{
					m_pLTileOld->Remove(pTile);
				}
				else
				{//还在多线程加载就不移除了
					
				}
			}
		}

		if(pListNew->CheckMyOnly (m_pLTileOld,pListTmp))
		{
			for (int i = 0; i < pListTmp->Count; i++)
			{
				_mapTile* pTile	= (_mapTile*)pListTmp->Items[i];
				if(m_pLayerTile->getChildByTag ((int)pTile) == NULL)
				{
					CMTileSprite* pSprite = CMTileSprite::create(pTile->szSpr);
					pSprite->setAnchorPoint (ccp(0,1));
					pSprite->setPosition(PtMap2Cocos(pTile->nPosX, pTile->nPosY));
					m_pLayerTile->addChild(pSprite,1,(int)pTile);
				}
				m_pLTileOld->Add (pTile);
			}
		}
		pListTmp->Free ();
	}
}

void MapLayer::_updateMObj()
{
	xnList*		pListNew	= g_pMapCenter->m_pListMObj;

	if(pListNew->Count)
	{
		xnList* pListTmp	= xnList::Create ();
		if(m_pLMObjOld->CheckMyOnly (pListNew,pListTmp))
		{
			for (int i = 0; i < pListTmp->Count; i++)
			{
				MapSprObj* pMObj	= (MapSprObj*)pListTmp->Items[i];

				CCMObjSprite* pMSpr	= (CCMObjSprite*)m_pLayerMObj->getChildByTag((int)pMObj);
				if(pMSpr)
				{
					pMSpr->ClearShadow();
					m_pLayerMObj->removeChildByTag ((int)pMObj,true);
				}
			}
		}

		g_pMapCenter->m_pListMObj	= m_pLMObjOld;
		m_pLMObjOld	= pListNew;

		for (int i = 0; i < m_pLMObjOld->Count; i++)
		{
			MapSprObj* pMObj	= (MapSprObj*)m_pLMObjOld->Items[i];
			CCMObjSprite* pSprite = (CCMObjSprite*)m_pLayerMObj->getChildByTag ((int)pMObj);//pMObj->pSprView;
			if(pMObj->pMObj == NULL)
			{//上层已删除
				if(pSprite)
				{
					pSprite->ClearShadow();
					m_pLayerMObj->removeChildByTag ((int)pMObj,true);
				}
				m_pLMObjOld->Delete(i);
				i--;
				continue;
			}

			if(pSprite == NULL)
			{
				pSprite = CCMObjSprite::create(pMObj);
				m_pLayerMObj->addChild(pSprite,1,(int)pMObj);

				if(((MapAIObj*)pMObj->pMObj)->m_bySPEff == 1)
				{
					CCSprite*	pSpr	= CCSprite::create("ui/BOSS.png");
					pSpr->retain();
					m_pLayerTile->addChild(pSpr,5,(int)pMObj);
					m_pListGroundEff->Add(pSpr);
				}
				if(((MapAIObj*)pMObj->pMObj)->nMarryEff > 0)		//人物结婚光效
				{

					MapSprObj*	pSprT	= g_pMapCenter->m_pMapActCtrl->GetObjByID(MAPOBJ_PLAYER,((MapAIObj*)pMObj->pMObj)->nMarryEff);

					if(pSprT)
					{
						MapAIObj* pTPlayer	= (MapAIObj*)pSprT->pMObj;

						if(pTPlayer && pTPlayer->nMarryEff == ((MapAIObj*)pMObj->pMObj)->m_nID)
						{
							g_pMapCenter->AddMapEff(265,(int)pMObj,(int)pSprT,-1);
						}
					}
					
				}
			}
			pSprite->CheckUpdate();
			pSprite->setPosition(PtMap2Cocos(pMObj->nPosX, pMObj->nPosY));

			if(pSprite->getZOrder () != pMObj->nPosY)
			{
				m_pLayerMObj->reorderChild(pSprite,pMObj->nPosY);
			}

			if(g_pMapCenter->m_pMapAlpha->GetBlock(pMObj->nPosX,pMObj->nPosY)|| (pMObj->pMObj && ((MapAIObj*)pMObj->pMObj)->pStateCenter->m_bUnVisable))
			{
				pSprite->SetAlphaVal(120);
			}
			else
				pSprite->SetAlphaVal(255);

			if(g_pMapCenter->m_pMObjSel == pMObj && m_pEffSel->getParent()!= pSprite)
			{//
				if(m_pEffSel->getParent())
					m_pEffSel->getParent()->removeChild(m_pEffSel,false);
				pSprite->addChild(m_pEffSel,0);

				if(pMObj->byIsEnemy)
					m_pEffSel->setColor(ccc3(255,0,0));
				else
					m_pEffSel->setColor(ccc3(0,255,0));
			}
			else if(g_pMapCenter->m_pMObjSel == NULL)
			{
				if(m_pEffSel->getParent())
					m_pEffSel->getParent()->removeChild(m_pEffSel,false);
			}
		}

		for (int i = m_pListGroundEff->Count-1; i >= 0; i--)
		{
			CCSprite*	pSpr	= (CCSprite*)m_pListGroundEff->Items[i];
			
			CCSprite*	pMObj	= (CCSprite*)m_pLayerMObj->getChildByTag(pSpr->getTag());
			if(pMObj == NULL)
			{
				m_pLayerTile->removeChild(pSpr,true);
				pSpr->release();
				m_pListGroundEff->Delete(i);
				continue;
			}
			else
			{
				pSpr->setPosition(pMObj->getPosition());
			}

		}
		

		pListTmp->Free ();
	}
}

void MapLayer::_updateOther()
{
	m_pEffMove->CheckEnd();
	m_pEffSel->CheckEnd();
	if(g_pMapCenter->m_pMObjLockView == NULL || g_pMapCenter->m_pMObjLockView->m_byState!=OBJ_STATE_RUN)
	{
		if(xnGetTickCount() - m_dwMEffTick > 500)
		{
			m_pEffMove->setVisible(false);
		}
		else
		{
			CCPoint	pt	= m_pEffMove->getPosition();
			int nX,nY;
			PtCocos2Map(pt,nX,nY);
			if(g_pMapCenter->m_pMapBarrier->GetBlock(nX,nY))
				m_pEffMove->setColor(ccc3(200,33,0));
		}
	}
	

//	//==================以下是障碍显示部分===================
//#ifdef WIN32
//	static CCSprite**	szSpr	= NULL;
//	if(szSpr == NULL)
//	{
//		szSpr	= new CCSprite*[10000];
//		CCSpriteBatchNode* pSprBatch	= CCSpriteBatchNode::create("ui/MB_ZUANSHI.png");
//		pSprBatch->setPosition(CCPointZero);
//		addChild(pSprBatch,1);
//		for (int i = 0; i < 10000; i++)
//		{
//			szSpr[i]	= CCSprite::createWithTexture(pSprBatch->getTexture());
//			szSpr[i]->setBatchNode(pSprBatch);
//			pSprBatch->addChild(szSpr[i],99);
//		}
//	}
//	
//	int nIndex	= 0;
//	CCPoint	ptCur	= getPosition();
//	for (int i = -((int)(ptCur.y/64))*64; i < g_pMapCenter->m_pMapTile->m_nTHeight; i+=64)
//	{
//		for (int j = -((int)(ptCur.x/64))*64; j < g_pMapCenter->m_pMapTile->m_nTWidth; j+=64)
//		{
//			int nX,nY;
//			PtCocos2Map(ccp(j,i),nX,nY);
//			if(g_pMapCenter->m_pMapBarrier->GetBlock(nX,nY))
//			{
//				szSpr[nIndex]->setPosition(ccp(j,i));
//				szSpr[nIndex]->setVisible(true);
//				nIndex++;
//			}
//		}
//	}
//	for (int i = nIndex; i < 10000; i++)
//	{
//		szSpr[i]->setVisible(false);
//	}
//#endif
//	//=========================================================
}

void	randShackNode(CCNode* pNode)
{
	CCPoint pt	= ccp((rand()%6-2)/POINT_PIX_CNT,(rand()%9-4)/POINT_PIX_CNT);
	CCActionInterval* pMove1	= CCMoveTo::create (0.08,pt);
	pt	= ccp((rand()%6-2)/POINT_PIX_CNT,(rand()%9-4)/POINT_PIX_CNT);
	CCActionInterval* pMove2	= CCMoveTo::create (0.08,pt);
	pt	= ccp((rand()%6-2)/POINT_PIX_CNT,(rand()%9-4)/POINT_PIX_CNT);
	CCActionInterval* pMove3	= CCMoveTo::create (0.08,pt);
	pt	= ccp((rand()%6-2)/POINT_PIX_CNT,(rand()%9-4)/POINT_PIX_CNT);
	CCActionInterval* pMove4	= CCMoveTo::create (0.08,pt);
	pt	= ccp(0,0);
	CCActionInterval* pMove	= CCMoveTo::create (0.05,pt);

	CCFiniteTimeAction* seq1 = CCSequence::create (CCEaseSineOut::create (pMove1), CCEaseSineOut::create (pMove2), CCEaseSineOut::create (pMove3), CCEaseSineOut::create (pMove4),NULL);
	CCFiniteTimeAction* seq = CCSequence::create (seq1, seq1, seq1, seq1, pMove,NULL);
	pNode->stopAllActions();
	pNode->runAction(seq);
}

void MapLayer::_updateEffs()
{
	switch(g_pMapCenter->m_bySpEff)
	{
	case 1:
		{
			randShackNode(m_pLayerTile);
			randShackNode(m_pLayerMObj);
		}
		break;
	}

	g_pMapCenter->m_bySpEff	= 0;

	//只有临时光效, 以后需要删除功能
	while(g_pMapCenter->m_pLMapTmpEffs->Count)
	{
		MapEffObj*	pMEff	= (MapEffObj*)g_pMapCenter->m_pLMapTmpEffs->Delete(0);

		bool bIsExist	= false;
		for (int i = 0; i < m_pLSprEff->Count; i++)
		{
			CCEffectSprite*	pSprEff	= (CCEffectSprite*)m_pLSprEff->Items[i];
			if(pSprEff->m_pSpVal)
			{
				MapEffObj*	pMCheck	= (MapEffObj*)pSprEff->m_pSpVal;
				if(pMCheck->nEffID == pMEff->nEffID && pMCheck->nPosX == pMEff->nPosX && pMCheck->nPosY == pMEff->nPosY)
				{
					bIsExist	= true;
					break;
				}
			}
		}
		if(bIsExist)
			continue;

		EffectObj*	pEffObj	= EffectCenter::GetInstance().GetEffObj(pMEff->nEffID);
		CCEffectSprite*	pSprEff	= CCEffectSprite::create(pEffObj);

		if(pEffObj->m_nRunType == 7)
		{
			MapSprObj*	pMObjFrom	= (MapSprObj*)pMEff->nPosX;
			MapSprObj*	pMObjTo		= (MapSprObj*)pMEff->nPosY;
			
			CCSprite*	pMObjF	= (CCSprite*)m_pLayerMObj->getChildByTag((int)pMObjFrom);
			CCSprite*	pMObjT	= (CCSprite*)m_pLayerMObj->getChildByTag((int)pMObjTo);
			if(pMObjF == NULL || pMObjT == NULL)
			{
				delete pMEff;
				continue;
			}

			pSprEff->setPosition(pMObjF->getPosition());
			pSprEff->setAnchorPoint(ccp(0.5,1));
			m_pLayerMObj->addChild(pSprEff,pMObjFrom->nPosY);

			pMEff->nEffID		= xnGetTickCount();
			pSprEff->m_pSpVal	= pMEff;
		}
		else
		{
			pSprEff->setPosition(PtMap2Cocos(pMEff->nPosX,pMEff->nPosY));
			m_pLayerMObj->addChild(pSprEff,pMEff->nPosY);
			delete pMEff;
		}

		m_pLSprEff->Add(pSprEff);

	}

	while(g_pMapCenter->m_pLRunEffOut->Count)
	{
		_MapEffectObj* pMEO	= (_MapEffectObj*)g_pMapCenter->m_pLRunEffOut->Delete(0);
		if(pMEO->pRunEff)
		{
			m_pLSprEff->Remove(pMEO->pRunEff);
			m_pLayerMObj->removeChild((CCEffectSprite*)pMEO->pRunEff,false);
			pMEO->pRunEff	= NULL;
		}
	}

	DWORD	dwCurTick	= xnGetTickCount();
	for (int i = 0; i < g_pMapCenter->m_pLRunEffView->Count; i++)
	{
		_MapEffectObj* pMEO	= (_MapEffectObj*)g_pMapCenter->m_pLRunEffView->Items[i];
		if(pMEO->pRunEff == NULL || (pMEO->dwTick && dwCurTick - pMEO->dwRunTick > pMEO->dwTick))
		{
			CCEffectSprite*	pSprEff	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(pMEO->nEffectID));
			if(pSprEff)
			{
				pSprEff->setPosition(PtMap2Cocos(pMEO->nMapX,pMEO->nMapY));
				m_pLayerMObj->addChild(pSprEff,pMEO->nMapY);
				m_pLSprEff->Add(pSprEff);
				pMEO->pRunEff	= pSprEff;
				pMEO->dwRunTick	= dwCurTick;
			}
		}
	}

	for (int i = 0; i < m_pLSprEff->Count; i++)
	{
		CCEffectSprite*	pSprEff	= (CCEffectSprite*)m_pLSprEff->Items[i];
		if(pSprEff->CheckEnd())
		{
			if(pSprEff->m_pSpVal)
				delete (MapSprObj*)pSprEff->m_pSpVal;

			m_pLayerMObj->removeChild(pSprEff,false);
			m_pLSprEff->Delete(i--);
		}
		else
		{
			if(pSprEff->m_pSpVal)
			{
				MapEffObj*	pME	= (MapEffObj*)pSprEff->m_pSpVal;

				int nCLen	= pSprEff->getContentSize().height;
				CCSprite*	pMObjF	= (CCSprite*)m_pLayerMObj->getChildByTag(pME->nPosX);
				CCSprite*	pMObjT	= (CCSprite*)m_pLayerMObj->getChildByTag(pME->nPosY);
				if(pMObjF == NULL || pMObjT == NULL)
				{
					if(pSprEff->m_nLoopTime == 0)
					{
						delete (MapSprObj*)pSprEff->m_pSpVal;

						m_pLayerMObj->removeChild(pSprEff,false);
						m_pLSprEff->Delete(i--);
					}

					continue;
				}
				
				DWORD	dwPassTick	= dwCurTick	- pME->nEffID;
				int		nDir	= CalcDirection(pMObjT->getPositionX(),pMObjF->getPositionY(),pMObjF->getPositionX(),pMObjT->getPositionY(),100);
				int		nDis	= CalcDistance(pMObjF->getPositionX(),pMObjF->getPositionY(),pMObjT->getPositionX(),pMObjT->getPositionY());
				float	fScale	= ((float)nDis) / nCLen;
				if(dwPassTick < 200)
					fScale	= fScale*dwPassTick/200;

				pSprEff->setPosition(pMObjF->getPosition());
				pSprEff->setScaleY(fScale);
				pSprEff->setRotation(nDir);
			}
		}
	}
}

void MapLayer::_updateControl()
{
	if(g_pMapCenter->m_pMObjLockView == NULL)
		return;
	if(m_pCtrlLayer->m_bClickHold && m_pCtrlLayer->m_nChgLen == 0)
	{
		m_pCtrlLayer->m_bClickHold	= false;
		m_pMapEvent->MapEventMove(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY);
	}
	else if(m_pCtrlLayer->m_nChgLen > 10/POINT_PIX_CNT)
	{
		
		CCPoint	ptClick;
		ptClick.x	= m_pCtrlLayer->m_ptChged.x*180/POINT_PIX_CNT/m_pCtrlLayer->m_nChgLen;
		ptClick.y	= m_pCtrlLayer->m_ptChged.y*180/POINT_PIX_CNT/m_pCtrlLayer->m_nChgLen;
		CCPoint ptCur	= PtMap2Cocos(g_pMapCenter->m_pMObjLockView->nPosX,g_pMapCenter->m_pMObjLockView->nPosY);
		CCPoint pt	= ccpAdd(ptClick,ptCur);

		m_pEffMove->setVisible(false);
		int nX, nY;
		PtCocos2Map(pt,nX,nY);
		m_pMapEvent->MapEventMove(nX,nY);
	}
	else if(m_bClickHold)
	{
		CCPoint pt	= ccpSub (m_ptClick,getPosition ());
		if(nowRateW&& nowRateH)
		{
			pt.x	/=	nowRateW;
			pt.y	/=	nowRateH;
		}

		m_pEffMove->setColor(ccc3(255,255,255));
		m_pEffMove->setPosition(pt);
		m_pEffMove->setVisible(true);
		
		m_dwMEffTick	= xnGetTickCount();

		int nX, nY;
		PtCocos2Map(pt,nX,nY);
		m_pMapEvent->MapEventMove(nX,nY);
	}
	if(g_pMapCommand->pGuideCmd)
		m_pEffMove->setVisible(false);
}

void MapLayer::InitMap()
{
	//清理旧光效
	while(g_pMapCenter->m_pLRunEffView->Count)
	{
		_MapEffectObj* pMEO	= (_MapEffectObj*)g_pMapCenter->m_pLRunEffView->Delete(0);
		if(pMEO->pRunEff)
			pMEO->pRunEff	= NULL;
	}
	m_pLSprEff->Clear();
	m_pLMObjOld->Clear();
	m_pLTileOld->Clear();
	g_pMapCenter->m_pMapActCtrl->ResetObj();

	m_pLayerTile->removeAllChildrenWithCleanup(false);
	m_pLayerMObj->removeAllChildrenWithCleanup(false);

	CCTextureCache::sharedTextureCache ()->removeUnusedTextures ();
	ResManager::GetInstance().ClearPList();
	gFun_ClearSpriteCache();
}
