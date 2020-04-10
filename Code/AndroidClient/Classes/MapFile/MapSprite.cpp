#include "MapSprite.h"
#include "MapControl.h"
#include "InterfaceLayer.h"
#include "EffectSprite.h"
#include "EffectObj.h"
#include "CtrlObj.h"
#include "MainMenu.h"
#include "MapMouse.h"
#include "EnemyConfig.h"
#include "MapStaticCfg.h"
#include "ResManage.h"
#include "MapTile.h"

using namespace cocos2d;

char*	s_szEnemyAction[]	= {"stand",	"run",	"attack",	"attack",	"hurt",		"hstand",	"hrun",  "dead",  "unkown"};

char*	s_szActionBody[]	= {"stand",	"run",	"atk1",		"atk2",		"hurt",		"hstand",	"hrun" , "dead",  "unkown"};
//char*	s_szActionBody[]	= {"stand",	"run",	"atk","atk1",		"atk2",	"atk3","atk4",	"hurt",		"fighting",	"floating" , "jump",  "relax"};

CCMObjSprite* CCMObjSprite::create( MapSprObj* pMObj )
{
	CCMObjSprite *pMOSprite = new CCMObjSprite();

	pMOSprite->m_pMSprObj	= pMObj;
	pMOSprite->init();
	pMOSprite->setContentSize(CCSize(1,1));
	pMOSprite->autorelease ();

	return pMOSprite;
}

CCMObjSprite::CCMObjSprite()
{
	m_pHpBar		= NULL;
	m_pHpBack		= NULL;
	m_pLbName		= NULL;
	m_pLbTitle		= NULL;
	m_pLbMTitle		= NULL;
	m_pSprCountry	= NULL;
	m_pSprVip		= NULL;
	//m_pSprSPEff	= NULL;
	m_pSprBody		= NULL;
	m_pSprWeopon	= NULL;
	m_pSprRide		= NULL;
	m_pSprRideEx	= NULL;
	m_pSprShadow	= NULL;
	m_pTipForm		= NULL;
	m_pSprMark		= NULL;
	m_pSprFstHome		= NULL;
	m_pCacheSkin		= NULL;
	m_pCacheSkinA		= NULL;
	m_pCacheSkinB		= NULL;
	//m_pEffShadow	= NULL;
	m_pLEffSpr		= xnList::Create();
	m_pLNumSpr		= xnList::Create();
	m_pLSprShadow	= xnList::Create();
}

CCMObjSprite::~CCMObjSprite()
{
	_clearSkinPList(m_pCacheSkin,1);
	_clearSkinPList(m_pCacheSkinA,2);
	_clearSkinPList(m_pCacheSkinB,3);
	m_pLSprShadow->Free();
	m_pLEffSpr->Free();
	m_pLNumSpr->Free();
}

bool CCMObjSprite::CheckUpdate()
{
	//if(m_pMSprObj->byDelState	== 1)
	//{
	//	m_pMSprObj->byDelState	= 2;
	//	return false;
	//}

	_checkHpBar();
	_checkMark();
	_checkName();
	_checkMyInfo();

	if(m_pMSprObj->byUpdateFlag)
	{
		if(m_pMSprObj->byUpdateFlag	== 2)
		{
			AddShadow(m_pSprBody,_checkBody());
			AddShadow(m_pSprWeopon,_checkWeopon());
			AddShadow(m_pSprRide,_checkRide());
		}
		else
		{
			_checkBody();
			_checkWeopon();
			_checkRide();
		}

		m_pMSprObj->byUpdateFlag	= 0;
	}

	_checkEff();
	_checkHitNum();
	_CheckToopTip();

	for (int i = 0; i < m_pLSprShadow->Count; i++)
	{
		CCSprite* pSpr	= (CCSprite*)m_pLSprShadow->Items[i];
		if(pSpr->getOpacity() < 10)
		{
			m_pLSprShadow->Delete(i--);
			getParent()->removeChild(pSpr,true);
		}
	}

	return true;
}

void CCMObjSprite::_checkHpBar()
{
	return;

	if(m_pHpBack	== NULL)
	{
		if(m_pMSprObj->byIsEnemy)
			m_pHpBack	= CCSprite::create ("ui/MB_HP_FTS2.png");
		else
			m_pHpBack	= CCSprite::create ("ui/MB_HP_FTS1.png");
		addChild(m_pHpBack, 9);
	}
	m_pHpBack->setPosition(ccp(0,m_pMSprObj->GetTitleH()/POINT_PIX_CNT));
	if(m_pHpBar	== NULL)
	{
		if(m_pMSprObj->byIsEnemy)
			m_pHpBar	= CCProgressTimer::create (CCSprite::create("ui/MB_HP_FT2.png"));
		else
			m_pHpBar	= CCProgressTimer::create (CCSprite::create("ui/MB_HP_FT1.png"));

		m_pHpBar->setType(kCCProgressTimerTypeBar);
		m_pHpBar->setBarChangeRate(ccp(1, 0));
		m_pHpBar->setMidpoint (ccp(0,0.5f));
		addChild(m_pHpBar, 10);
	}
	m_pHpBar->setPosition(ccp(0,m_pMSprObj->GetTitleH()/POINT_PIX_CNT));
	if(m_pMSprObj->nMaxHp)
		m_pHpBar->setPercentage (m_pMSprObj->nHp*100/m_pMSprObj->nMaxHp);
	
}

CCSpriteFrame* CCMObjSprite::_checkBody()
{
	if(m_pSprBody == NULL)
	{
		m_pSprBody	= CCSprite::create();
		addChild(m_pSprBody,3);
	}

	if(m_pMSprObj->pSkinPB)
		m_pSprBody->setPosition(ccp(m_pMSprObj->pSkin->nXPer/POINT_PIX_CNT,(m_pMSprObj->pSkin->nYPer+m_pMSprObj->pSkinPB->nTitleH)/POINT_PIX_CNT));
	else
		m_pSprBody->setPosition(ccp(m_pMSprObj->pSkin->nXPer/POINT_PIX_CNT,m_pMSprObj->pSkin->nYPer/POINT_PIX_CNT));

	if(/*0 && */m_pMSprObj->byAction == OBJ_ACTION_DEAD)
	{
		if(m_pMSprObj->byType != MAPOBJ_PLAYER)
		{
			m_pSprBody->init ();
			if(m_pSprShadow)
				m_pSprShadow->setVisible(false);
		}
		else
		{
			m_pSprBody->initWithFile("ui/MUBEI.png");
			m_pSprBody->getTexture()->setAliasTexParameters();
			m_pSprBody->setPosition(ccp(0,0));
		}
		return NULL;
	}

	int nDir	= m_pMSprObj->byDirect;
	bool	bFlipX	= false;
	if(nDir == 6)
	{
		nDir	= 2;
		bFlipX	= true;
	}
	else if(nDir == 7)
	{
		nDir	= 1;
		bFlipX	= true;
	}
	else if(nDir == 5)
	{
		nDir	= 3;
		bFlipX	= true;
	}
	

	int	nAct	= m_pMSprObj->byAction;
	if(m_pMSprObj->pSkin->nId >= 8000 && m_pMSprObj->pSkin->nId < 9000)
	{
		nDir	= 4;
		m_pMSprObj->byFrame	= 0;
		bFlipX	= false;
	}
	else if(m_pMSprObj->pSkin->nId >= 9000 && m_pMSprObj->pSkin->nId < 9500)
	{
		nDir	= 4;
		nAct	= OBJ_ACTION_STAND;
		bFlipX	= false;
	}
	else if(m_pMSprObj->pSkin->nId >= 9500 && m_pMSprObj->pSkin->nId < 10000)
	{
		if(nAct != OBJ_ACTION_RUN)
			nAct	= OBJ_ACTION_STAND;
	}

	char	szTmp[256];

	if(m_pMSprObj->pSkin->nId < 2000)
		sprintf(szTmp,"skin/%d/%s/%d/%d.png",m_pMSprObj->pSkin->nRealID,s_szActionBody[nAct],nDir+1,m_pMSprObj->byFrame+1);
	else
		sprintf(szTmp,"skin/%d/%s/%d/%d.png",m_pMSprObj->pSkin->nRealID,s_szEnemyAction[nAct],nDir+1,m_pMSprObj->byFrame+1);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	CCSpriteFrame* frame = cache->spriteFrameByName( szTmp );
	if(m_pCacheSkin != m_pMSprObj->pSkin/* || frame == NULL*/)
	{
		char szList[256];
		if(m_pCacheSkin != m_pMSprObj->pSkin)
			_clearSkinPList(m_pCacheSkin,1);
		for (int i = 0; i < m_pMSprObj->pSkin->bPartCnt; i++)
		{
			sprintf(szList,"skin/%d/%d.plist",m_pMSprObj->pSkin->nRealID,i+1);
			bool	bIsFileExist	= gfun_CheckSprCache(szList,false);
			if(bIsFileExist && m_pCacheSkin != m_pMSprObj->pSkin)
			{
				ResManager::GetInstance().AddPList(szList,m_pMSprObj->pSkin->nRealID,1);
			}
			if(frame == NULL && bIsFileExist)
			{
				char szFileT[256];
				sprintf(szFileT,"skin/%d/%d.pvr.ccz",m_pMSprObj->pSkin->nRealID,i+1);
				ASyncPListCenter::GetInstance().RqLoadPlist(szList,szFileT);
				//cache->addSpriteFramesWithFile(szList);
			}
		}
		m_pCacheSkin	= m_pMSprObj->pSkin;
		if(frame == NULL)
			frame = cache->spriteFrameByName( szTmp );
	}
	if(frame)
	{
		//m_pSprBody->init ();	//特殊图片资源会高亮白底,加这个可以修正
		m_pSprBody->initWithSpriteFrame (frame);
		m_pSprBody->getTexture()->setAliasTexParameters();
	}
	//else
	//	m_pSprBody->init ();

	m_pSprBody->setFlipX(bFlipX);
	//if(m_pMSprObj->pSkin->nScale	!= 100)
	m_pSprBody->setScale(m_pMSprObj->pSkin->nScale/100.0f);
	if(m_pSprShadow)
	{
		if(m_pMSprObj->pSkinPB)
			m_pSprShadow->setScale(m_pMSprObj->pSkin->nShadowSize/50.0);
		else// if(m_pMSprObj->pSkin->nShadowSize != 100)
			m_pSprShadow->setScale(m_pMSprObj->pSkin->nShadowSize/100.0);
	}

	return frame;
}

CCSpriteFrame* CCMObjSprite::_checkWeopon()
{
	if(m_pMSprObj->pSkinPA == NULL)
	{
		if(m_pSprWeopon)
			removeChild(m_pSprWeopon,true);
		m_pSprWeopon	= NULL;
		return NULL;
	}
	if(m_pSprWeopon == NULL)
	{
		m_pSprWeopon	= CCSprite::create();
		addChild(m_pSprWeopon,2);
	}
	if(m_pMSprObj->pSkinPB)
		m_pSprWeopon->setPosition(ccp(m_pMSprObj->pSkinPA->nXPer/POINT_PIX_CNT,(m_pMSprObj->pSkinPA->nYPer+m_pMSprObj->pSkinPB->nTitleH)/POINT_PIX_CNT));
	else
		m_pSprWeopon->setPosition(ccp(m_pMSprObj->pSkinPA->nXPer/POINT_PIX_CNT,(m_pMSprObj->pSkinPA->nYPer)/POINT_PIX_CNT));

	if(m_pMSprObj->byAction == OBJ_ACTION_DEAD)
	{
		m_pSprWeopon->init();
		return NULL;
	}

	int nDir	= m_pMSprObj->byDirect;
	if(nDir == 6)
		nDir	= 2;

	switch((m_pMSprObj->pSkinPA->nId%10 - 1)/2)
	{//无职业,战士,法师,火枪手, 牧师
	case 0: //战士
		if(nDir == 0)
			reorderChild(m_pSprWeopon,2);
		else
			reorderChild(m_pSprWeopon,4);
		break;
	case 1:	//法师
		{
			BYTE bySex = 1;
			if(m_pMSprObj->byType == MAPOBJ_PLAYER)
			{
				bySex = ((MapPlayer*)m_pMSprObj->pMObj)->bySex;
			}
			if(m_pMSprObj->pSkinPB==NULL)
			{
				if(nDir == 4)
				{
					reorderChild(m_pSprWeopon,2);
				}
				else
					reorderChild(m_pSprWeopon,4);
			}
			else
			{
				if (bySex == 0)
				{
					if(nDir == 0)
					{
						reorderChild(m_pSprWeopon,2);
					}
					else
						reorderChild(m_pSprWeopon,4);
				}else
				{
					if(nDir == 0)
					{
						if (m_pMSprObj->byAction == OBJ_ACTION_HRUN)
							reorderChild(m_pSprWeopon,4);
						else
							reorderChild(m_pSprWeopon,2);
					}else if (nDir == 4)
					{
						if (m_pMSprObj->byAction == OBJ_ACTION_HRUN)
							reorderChild(m_pSprWeopon,2);
						else
							reorderChild(m_pSprWeopon,4);
					}else
						reorderChild(m_pSprWeopon,4);
				}
			}
		}
		
		break;
	case 2:	//火枪
		if(nDir == 0)
			reorderChild(m_pSprWeopon,2);
		else
			reorderChild(m_pSprWeopon,4);
		break;
	case 3:	//牧师
		if(nDir == 4)
			reorderChild(m_pSprWeopon,4);
		else
			reorderChild(m_pSprWeopon,2);
		break;
	}

	char	szTmp[256];
	sprintf(szTmp,"skin/%d/%s/%d/%d.png",m_pMSprObj->pSkinPA->nRealID,s_szActionBody[m_pMSprObj->byAction],nDir+1,m_pMSprObj->byFrame+1);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	CCSpriteFrame* frame = cache->spriteFrameByName( szTmp );
	if(m_pCacheSkinA != m_pMSprObj->pSkinPA/*|| frame == NULL*/)
	{
		char szList[256];
		if(m_pCacheSkinA != m_pMSprObj->pSkinPA)
			_clearSkinPList(m_pCacheSkinA,2);
		for (int i = 0; i < m_pMSprObj->pSkinPA->bPartCnt; i++)
		{
			sprintf(szList,"skin/%d/%d.plist",m_pMSprObj->pSkinPA->nRealID,i+1);
			bool	bIsFileExist	= gfun_CheckSprCache(szList,false);
			if(bIsFileExist && m_pCacheSkinA != m_pMSprObj->pSkinPA)
				ResManager::GetInstance().AddPList(szList,m_pMSprObj->pSkinPA->nRealID,2);
			if(frame == NULL && bIsFileExist)
			{
				char szFileT[256];
				sprintf(szFileT,"skin/%d/%d.pvr.ccz",m_pMSprObj->pSkinPA->nRealID,i+1);
				ASyncPListCenter::GetInstance().RqLoadPlist(szList,szFileT);
				//cache->addSpriteFramesWithFile(szList);
			}
		}
		m_pCacheSkinA	= m_pMSprObj->pSkinPA;
		if(frame == NULL)
			frame = cache->spriteFrameByName(szTmp);
	}

	if(frame != NULL)
	{
		//m_pSprWeopon->init ();	//特殊图片资源会高亮白底,加这个可以修正
		m_pSprWeopon->initWithSpriteFrame (frame);
		m_pSprWeopon->getTexture()->setAliasTexParameters();
	}

	if(nDir	!= m_pMSprObj->byDirect)
		m_pSprWeopon->setFlipX(true);
	else
		m_pSprWeopon->setFlipX(false);

	//if(m_pMSprObj->pSkinPA->nScale	!= 100)
	m_pSprWeopon->setScale(m_pMSprObj->pSkinPA->nScale/100.0f);

	return frame;
}

CCSpriteFrame* CCMObjSprite::_checkRide()
{
	if(m_pMSprObj->pSkinPB == NULL)
	{
		if(m_pSprRide)
			removeChild(m_pSprRide,true);
		m_pSprRide		= NULL;
		if(m_pSprRideEx)
			removeChild(m_pSprRideEx,true);
		m_pSprRideEx	= NULL;
		return NULL;
	}
	if(m_pSprRide == NULL)
	{
		m_pSprRide	= CCSprite::create();
		addChild(m_pSprRide,1);
	}
	if(m_pMSprObj->pSkinPB->bExPart)
	{
		if(m_pSprRideEx == NULL)
		{
			m_pSprRideEx	= CCSprite::create();
			addChild(m_pSprRideEx,5);
		}
	}
	else
	{
		if(m_pSprRideEx)
			removeChild(m_pSprRideEx,true);
		m_pSprRideEx	= NULL;
	}

	m_pSprRide->setPosition(ccp(m_pMSprObj->pSkinPB->nXPer/POINT_PIX_CNT,m_pMSprObj->pSkinPB->nYPer/POINT_PIX_CNT));
	if(m_pSprRideEx)
		m_pSprRideEx->setPosition(ccp(m_pMSprObj->pSkinPB->nXPer/POINT_PIX_CNT,m_pMSprObj->pSkinPB->nYPer/POINT_PIX_CNT));

	if(m_pMSprObj->byAction == OBJ_ACTION_DEAD)
	{
		m_pSprRide->init();
		if(m_pSprRideEx)
			m_pSprRideEx->init();
		return NULL;
	}

	int nDir	= m_pMSprObj->byDirect;
	if(nDir == 6)
		nDir	= 2;
	char	szTmp[256];
	sprintf(szTmp,"skin/%d/%s/%d/%d.png",m_pMSprObj->pSkinPB->nRealID,s_szActionBody[m_pMSprObj->byAction],nDir+1,m_pMSprObj->byFrame+1);
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();

	CCSpriteFrame* frame = cache->spriteFrameByName( szTmp );
	if(m_pCacheSkinB != m_pMSprObj->pSkinPB/*|| frame == NULL*/)
	{
		char szList[256];
		if(m_pCacheSkinB != m_pMSprObj->pSkinPB)
			_clearSkinPList(m_pCacheSkinB,3);

		for (int i = 0; i < m_pMSprObj->pSkinPB->bPartCnt; i++)
		{
			sprintf(szList,"skin/%d/%d.plist",m_pMSprObj->pSkinPB->nRealID,i+1);
			bool	bIsFileExist	= gfun_CheckSprCache(szList,false);
			if(bIsFileExist && m_pCacheSkinB != m_pMSprObj->pSkinPB)
				ResManager::GetInstance().AddPList(szList,m_pMSprObj->pSkinPB->nRealID,3);
			if(frame == NULL && bIsFileExist)
			{
				char szFileT[256];
				sprintf(szFileT,"skin/%d/%d.pvr.ccz",m_pMSprObj->pSkinPB->nRealID,i+1);
				ASyncPListCenter::GetInstance().RqLoadPlist(szList,szFileT);
				//cache->addSpriteFramesWithFile(szList);
			}
		}
		m_pCacheSkinB	= m_pMSprObj->pSkinPB;
		if(frame == NULL)
			frame = cache->spriteFrameByName(szTmp);
	}
	if(frame != NULL)
	{
		//m_pSprRide->init ();	//特殊图片资源会高亮白底,加这个可以修正
		m_pSprRide->initWithSpriteFrame (frame);
		m_pSprRide->getTexture()->setAliasTexParameters();

		if(m_pSprRideEx)
		{
			char	szTmpex[256];
			sprintf(szTmpex,"skin/%d/%sex/%d/%d.png",m_pMSprObj->pSkinPB->nRealID,s_szActionBody[m_pMSprObj->byAction],nDir+1,m_pMSprObj->byFrame+1);
			CCSpriteFrame* frameex = cache->spriteFrameByName(szTmpex);
			
			if(frameex)
			{
				m_pSprRideEx->initWithSpriteFrame (frameex);
				m_pSprRideEx->getTexture()->setAliasTexParameters();
			}
			else
				m_pSprRideEx->init ();	//特殊图片资源会高亮白底,加这个可以修正
		}

	}
	if(nDir	!= m_pMSprObj->byDirect)
	{
		m_pSprRide->setFlipX(true);
		if(m_pSprRideEx)
			m_pSprRideEx->setFlipX(true);
	}
	else
	{
		m_pSprRide->setFlipX(false);
		if(m_pSprRideEx)
			m_pSprRideEx->setFlipX(false);
	}

	//if(m_pMSprObj->pSkinPB->nScale	!= 100)
	{
		m_pSprRide->setScale(m_pMSprObj->pSkinPB->nScale/100.0f);
		if(m_pSprRideEx)
			m_pSprRideEx->setScale(m_pMSprObj->pSkinPB->nScale/100.0f);
	}

	return frame;
}

bool CCMObjSprite::NoNeedShadow()
{
	MapAIObj* pObj = (MapAIObj*)m_pMSprObj->pMObj;
	if (pObj&&(pObj->m_nID/100 > 49)&&(pObj->m_nID/100 < 52))
		return true;

	return false;
}

void CCMObjSprite::_checkEff()
{
	//if(((MapAIObj*)(m_pMSprObj->pMObj))->m_bySPEff == 1)
	//{
	//	if(m_pSprSPEff	== NULL)
	//	{
	//		m_pSprSPEff	= CCSprite::create("ui/BOSS.png");
	//		m_pSprSPEff->setPosition(CCPointZero);
	//		addChild(m_pSprSPEff,-1);
	//	}
	//	int	nTmpScaleVal	= (((int)m_pMSprObj)+xnGetTickCount())/50%40;
	//	if(nTmpScaleVal > 20)
	//		nTmpScaleVal	= 40-nTmpScaleVal;
	//	m_pSprSPEff->setScale((90+nTmpScaleVal)/100.0);
	//}


	if(m_pSprShadow == NULL&&!NoNeedShadow())
	{
		m_pSprShadow		= CCSprite::create("ui/YINGZI.png");
		m_pSprShadow->setPosition(ccp(4/POINT_PIX_CNT,-8/POINT_PIX_CNT));
		addChild(m_pSprShadow,-2);
	}

	if (NoNeedShadow())//鱼群去掉影子
		m_pSprShadow == NULL;

	for (int i = m_pLEffSpr->Count-1; i >= 0; i--)
	{
		CCEffectSprite*	pEff	= (CCEffectSprite*)m_pLEffSpr->Items[i];
		if(pEff->CheckEnd())
		{
			DelSprEff(pEff);
		}
		else if(pEff->m_pEff->m_bPosMark == 1)
		{
			if(m_pMSprObj->pSkinPB)
				pEff->setPositionY((pEff->m_pEff->m_nHeight+m_pMSprObj->pSkinPB->nTitleH)/POINT_PIX_CNT);
			else
				pEff->setPositionY(pEff->m_pEff->m_nHeight+POINT_PIX_CNT);
		}
		else if(pEff->m_pEff->m_bPosMark == 2)
		{
			if(m_pSprCountry && m_pSprCountry->isVisible())
			{
				if(m_pMSprObj->pSkinPB)
					pEff->setPositionY((pEff->m_pEff->m_nHeight+m_pMSprObj->pSkinPB->nTitleH)/POINT_PIX_CNT);
				else
					pEff->setPositionY(pEff->m_pEff->m_nHeight/POINT_PIX_CNT);
			}
			else
			{
				if(m_pMSprObj->pSkinPB)
					pEff->setPositionY((pEff->m_pEff->m_nHeight+m_pMSprObj->pSkinPB->nTitleH-50)/POINT_PIX_CNT);
				else
					pEff->setPositionY((pEff->m_pEff->m_nHeight-50)/POINT_PIX_CNT);
			}

		}
	}
	while(m_pMSprObj->pListEffDel->Count)
	{
		int	nEffID	= (int)m_pMSprObj->pListEffDel->Delete (0);
		for (int i = m_pLEffSpr->Count-1; i >= 0; i--)
		{
			CCEffectSprite* pEffS	= (CCEffectSprite*)m_pLEffSpr->Items[i];
			if(pEffS->GetEffID ()	== nEffID)
			{
				DelSprEff(pEffS);
				break;
			}
		}
	}
	if(m_pMSprObj->byType == MAPOBJ_NPC && m_pLEffSpr->Count)
		return;

	DWORD	dwTickNow	= xnGetTickCount();
	for (int i = m_pMSprObj->pListEffAdd->Count-1; i >=0; i--)
	{
		MObjEff*	pMEff	= (MObjEff*)m_pMSprObj->pListEffAdd->Items[i];

		if(((int)(dwTickNow - pMEff->dwTickDelay)) < 0)
			continue;
		int nDir	= -1;

		EffectObj*	pEObj	= EffectCenter::GetInstance ().GetEffObj (pMEff->nEffID);
		if(pEObj == NULL)
		{
			delete pMEff;
			m_pMSprObj->pListEffAdd->Delete(i);
			continue;
		}
		if(pEObj->m_nRunType == 8)
			nDir	= m_pMSprObj->byDirect+1;
		else if(pMEff->nDifX != 0 || pMEff->nDifY != 0)
			nDir	= CalcDirection(pMEff->nDifX,pMEff->nDifY,0,0,100);
		
		CCEffectSprite* pEffSpr	= CCEffectSprite::create (pEObj,nDir);
		if(pEffSpr)
		{
			CCPoint	ptDefaul	= ccp(0,pEObj->m_nHeight/POINT_PIX_CNT);
			if(pEObj->m_bPosMark)
			{
				if(m_pMSprObj->pSkinPB)
					ptDefaul	= ccp(0,(pEObj->m_nHeight+m_pMSprObj->pSkinPB->nTitleH)/POINT_PIX_CNT);
			}
			pEffSpr->setPosition(ptDefaul);
			AddSprEff (pEffSpr,pEffSpr->GetEffZorder ());

			if(pEObj->m_byScaleBySize && m_pMSprObj->pSkin)
			{
				pEffSpr->setScale(pEObj->m_nScale*m_pMSprObj->pSkin->nShadowSize/10000.0);
			}

			if(pMEff->bFly && (pMEff->nDifX != 0 || pMEff->nDifY != 0))
			{//飞行动作
				CCPoint	ptCur	= pEffSpr->getPosition ();
				CCPoint	ptStart	= ccp(ptCur.x + pMEff->nDifX/POINT_PIX_CNT,ptCur.y-pMEff->nDifY/POINT_PIX_CNT);
				pEffSpr->setPosition (ccpAdd(ptDefaul,ptStart));
				CCMoveTo* pMove	= CCMoveTo::create (pEObj->m_nTimeFrm/1000.0,ccpAdd(ptDefaul,ptCur));
				pEffSpr->runAction (pMove);
				if(pMEff->nDifY < 0)
					pEffSpr->_setZOrder(-1);
			}
		}
		if (m_pMSprObj->byType == MAPOBJ_NPC)
			continue;
		delete pMEff;
		m_pMSprObj->pListEffAdd->Delete(i);
	}
}

void CCMObjSprite::_checkMyInfo()
{
	if(m_pMSprObj->pMObj == g_pMapCenter->m_pMObjLockView)
	{
		if(m_pLbName == NULL)
		{
			m_pLbName	=	CCLabelTTF::create("",DEFAULT_FONT,22/POINT_PIX_CNT);
		
			m_pLbName->setColor(ccc3(9,246,243));
			
			addChild(m_pLbName,5);
			
		}
		m_pLbName->setPosition(ccp(0,(m_pMSprObj->GetTitleH())/POINT_PIX_CNT));
		if(pMainMenu->m_pAutoMenu->m_bActive)
		{
			int nDotNum	= xnGetTickCount() /500%3;
			if(nDotNum == 1)
				m_pLbName->setString(UTEXT_CHINE("　自动战斗中．．"));
			else if(nDotNum == 2)
				m_pLbName->setString(UTEXT_CHINE("　　自动战斗中．．．"));
			else
				m_pLbName->setString(UTEXT_CHINE("自动战斗中．"));

			m_pLbName->setVisible(true);
		}
		else if(g_pMapCommand->pGuideCmd)
		{
			int nDotNum	= xnGetTickCount() /500%3;
			if(nDotNum == 1)
				m_pLbName->setString(UTEXT_CHINE("　自动寻路中．．"));
			else if(nDotNum == 2)
				m_pLbName->setString(UTEXT_CHINE("　　自动寻路中．．．"));
			else
				m_pLbName->setString(UTEXT_CHINE("自动寻路中．"));

			m_pLbName->setVisible(true);
		}
		else
			m_pLbName->setVisible(false);

	}
}

void CCMObjSprite::_checkName()
{
	bool bHide	= false;
	if(pMainMenu->m_pSettingMenu->m_bHideTitle)
		bHide	= true;
	if(m_pMSprObj->pMObj == g_pMapCenter->m_pMObjLockView)
		bHide	= true;//自己不显示

	if(m_pMSprObj->byType	== MAPOBJ_ENEMY || m_pMSprObj->byType	== MAPOBJ_NULL)
	{
		if(g_pMapCenter->m_pMObjSel != m_pMSprObj)	//选中的目标显示名字
			bHide	= true;
	}

	if(bHide)
	{
		if(m_pLbName)
			m_pLbName->setVisible(false);

		if(m_pSprCountry)
			m_pSprCountry->setVisible(false);
		if(m_pSprFstHome)
			m_pSprFstHome->setVisible(false);
		if(m_pSprVip)
			m_pSprVip->setVisible(false);

		if(m_pLbTitle)
			m_pLbTitle->setVisible(false);

		if(m_pLbMTitle)
			m_pLbMTitle->setVisible(false);
		return;
	}

	if(m_pLbName == NULL)
	{
		m_pLbName	=	CCLabelTTF::create("",DEFAULT_FONT,22/POINT_PIX_CNT);
		m_pLbTitle	=	CCLabelTTF::create("",DEFAULT_FONT,18/POINT_PIX_CNT);
		m_pLbMTitle =	CCLabelTTF::create("",DEFAULT_FONT,18/POINT_PIX_CNT);
		if(m_pMSprObj->byType	== MAPOBJ_PLAYER)
		{
			m_pSprCountry	= CCSprite::create();
			m_pSprVip	= CCSprite::create();
		}
		if(m_pMSprObj->byIsEnemy)
		{
			m_pLbName->setColor(ccc3(200,0,0));
			m_pLbTitle->setColor(ccc3(200,0,0));
		}
		else
		{
			if(m_pMSprObj->byType	== MAPOBJ_SLAVE)
			{
				m_pLbName->setColor(ccc3(255,192,0));
				m_pLbTitle->setColor(ccc3(255,192,0));
			}
			else
			{
				m_pLbName->setColor(ccc3(0,255,0));
				m_pLbTitle->setColor(ccc3(147,205,221));
				m_pLbMTitle->setColor(ccc3(255,153,255));
				int nLen = strlen(m_pMSprObj->szMarryTitle);
				if(nLen >= 10)
				{
					char* pCheck = &m_pMSprObj->szMarryTitle[nLen-8];
					if(strncmp(pCheck,"模范",4) == 0)
						m_pLbMTitle->setColor(ccc3(204,51,255));
				}
			}

		}
		//m_pSprCountry	= CCSprite::create("");

		addChild(m_pLbName,5);
		addChild(m_pLbTitle,5);
		addChild(m_pLbMTitle,5);
		m_pLbName->setString(UTEXT_CHINE(m_pMSprObj->szName));
		m_pLbTitle->setString(UTEXT_CHINE(m_pMSprObj->szTitle));
		m_pLbMTitle->setString(UTEXT_CHINE(m_pMSprObj->szMarryTitle));
		if(m_pSprCountry)
		{
			//m_pSprCountry->setPosition(ccp(-40,(m_pMSprObj->GetTitleH()+30)/POINT_PIX_CNT));
			addChild(m_pSprCountry,5);
			if(*m_pMSprObj->pCountry == 1)
			{
				//m_pSprCountry->init();
				m_pSprCountry->initWithFile("ui/MB_GOU1.png");
			}
			else if(*m_pMSprObj->pCountry == 2)
			{
				//m_pSprCountry->init();
				m_pSprCountry->initWithFile("ui/MB_GOU2.png");
			}
		}
		if(m_pSprVip)
		{
			//m_pSprVip->setPosition(ccp(-40,(m_pMSprObj->GetTitleH())/POINT_PIX_CNT));
			addChild(m_pSprVip,5);
			if(*m_pMSprObj->pVipLevel > 0)
			{
				char szIcon[32];
				sprintf(szIcon,"ui/VIP%d.png",*m_pMSprObj->pVipLevel);
				//m_pSprVip->init();
				m_pSprVip->initWithFile(szIcon);
				m_pSprVip->setTag(5000+*m_pMSprObj->pVipLevel);
			}
			else
				m_pSprVip->setTag(0);
		}
	}
	else
	{
		if(m_pMSprObj->byIsEnemy)
		{
			m_pLbName->setColor(ccc3(200,0,0));
			m_pLbTitle->setColor(ccc3(255,192,0));
		}
		else
		{
			if(m_pMSprObj->byType	== MAPOBJ_SLAVE)
			{
				m_pLbName->setColor(ccc3(255,192,0));
				m_pLbTitle->setColor(ccc3(255,192,0));
			}
			else
			{
				m_pLbName->setColor(ccc3(0,255,0));
				m_pLbTitle->setColor(ccc3(147,205,221));
				m_pLbMTitle->setColor(ccc3(255,153,255));
				int nLen = strlen(m_pMSprObj->szMarryTitle);
				if(nLen >= 10)
				{
					char* pCheck = &m_pMSprObj->szMarryTitle[nLen-8];
					if(strncmp(pCheck,"模范",4) == 0)
						m_pLbMTitle->setColor(ccc3(204,51,255));
				}
			}

		}

		m_pLbName->setVisible(true);
		m_pLbTitle->setVisible(true);
		m_pLbName->setString(UTEXT_CHINE(m_pMSprObj->szName));
		m_pLbTitle->setString(UTEXT_CHINE(m_pMSprObj->szTitle));
		m_pLbMTitle->setString(UTEXT_CHINE(m_pMSprObj->szMarryTitle));

		//m_pSprCountry->init();
		//m_pSprCountry->initWithFile();
		if(m_pSprVip)
		{
			if(*m_pMSprObj->pVipLevel > 0 && (m_pSprVip->getTag()-5000) != *m_pMSprObj->pVipLevel)
			{
				char szIcon[32];
				sprintf(szIcon,"ui/VIP%d.png",*m_pMSprObj->pVipLevel);
				//m_pSprVip->init();
				m_pSprVip->initWithFile(szIcon);
				m_pSprVip->setTag(5000+*m_pMSprObj->pVipLevel);
			}
		}
	}
	if(m_pLbName)
	{
		m_pLbName->setPosition(ccp(0,(m_pMSprObj->GetTitleH())/POINT_PIX_CNT));
		m_pLbTitle->setPosition(ccp(0,(m_pMSprObj->GetTitleH()+25)/POINT_PIX_CNT));
		m_pLbMTitle->setPosition(ccp(0,(m_pMSprObj->GetTitleH()+50)/POINT_PIX_CNT));
		int nWidthN = m_pLbName->getContentSize().width*POINT_PIX_CNT;
		int nHeightN= m_pLbName->getContentSize().height*POINT_PIX_CNT;
		int nWidthT = m_pLbTitle->getContentSize().width*POINT_PIX_CNT;
		if(m_pSprCountry)
		{
			m_pSprCountry->setPosition(ccp(-nWidthT/2/POINT_PIX_CNT-20/POINT_PIX_CNT,(m_pMSprObj->GetTitleH()+nHeightN+4)/POINT_PIX_CNT));
		}
		if(m_pSprVip)
			m_pSprVip->setPosition(ccp(-nWidthN/2/POINT_PIX_CNT-20/POINT_PIX_CNT,(m_pMSprObj->GetTitleH())/POINT_PIX_CNT));

		if(m_pMSprObj->byType	== MAPOBJ_PLAYER)
		{
			bool	bIsFstH	= ((MapPlayer*)m_pMSprObj->pMObj)->pStateCenter->CheckState(5100);
			if(bIsFstH && m_pSprFstHome == NULL)
			{
				m_pSprFstHome	= CCSprite::create();
				addChild(m_pSprFstHome,5);
				m_pSprFstHome->initWithFile("ui/MB_JT_DIYI.png");
			}
			else if(!bIsFstH && m_pSprFstHome)
			{
				removeChild(m_pSprFstHome,false);
				m_pSprFstHome	= NULL;
			}
			if(m_pSprFstHome)
				m_pSprFstHome->setPosition(ccp(nWidthT/POINT_PIX_CNT+2/POINT_PIX_CNT,(m_pMSprObj->GetTitleH()+nHeightN+4)/POINT_PIX_CNT));
		}
	}
}

void CCMObjSprite::AddSprEff( CCEffectSprite* pEffSpr, int nZorder )
{
	m_pLEffSpr->Add (pEffSpr);
	addChild (pEffSpr,nZorder);
}

void CCMObjSprite::DelSprEff( CCEffectSprite* pEffSpr )
{
	m_pLEffSpr->Remove (pEffSpr);
	removeChild (pEffSpr,false);	//光效对象经过action->retain, 不能clearup
}

void CCMObjSprite::_checkHitNum()
{
	for (int i = m_pLNumSpr->Count-1; i >= 0; i--)
	{
		CCNumberSprite* pEff	= (CCNumberSprite*)m_pLNumSpr->Items[i];
		if(pEff->CheckEnd ())
		{
			removeChild (pEff,true);
			m_pLNumSpr->Delete (i);
		}
	}

	while(m_pMSprObj->pListFightNum->Count)
	{
		_MuliParamObj*	pNum	= (_MuliParamObj*)m_pMSprObj->pListFightNum->Delete (0);
		
		switch(pNum->nParam2)
		{
		case 0:
			if(pNum->nParam1 >0)
				pNum->nParam2	= 1;
			break;
		}
		CCNumberSprite* pNumSpr	= CCNumberSprite::create (pNum->nParam1,pNum->nParam2);
		
		if(pNumSpr)
		{
			int nStartY	= (m_pMSprObj->GetTitleH()+50)/2/POINT_PIX_CNT;
			
			addChild(pNumSpr,10);

			if(pNum->nParam2 == 2)
			{//暴击
				pNumSpr->setPosition(ccp(0,nStartY+30));
				CCActionInterval* pScale	= CCScaleTo::create (0.3,1.5f);
				CCActionInterval* pDelay		= CCDelayTime::create (0.7);
				CCActionInterval* pSeq		= (CCActionInterval*)CCSequence::create(CCEaseSineOut::create(pScale),pDelay,NULL);
				pNumSpr->runAction(pSeq);
			}
			else
			{
				pNumSpr->setPosition(ccp(0,nStartY));
				int nY	= (60+rand()%4*14)/POINT_PIX_CNT;
				CCActionInterval* pMove1		= CCMoveBy::create (0.3,ccp(0,nY));
				CCActionInterval* pMove2	= CCMoveTo::create (0.3,ccp((rand()%5*25-50)/POINT_PIX_CNT,nY+nStartY));
				CCActionInterval* pSpaw		= (CCActionInterval*)CCSpawn::create(pMove1,CCEaseOut::create(pMove2,0.5f),NULL);
				CCActionInterval* pDelay		= CCDelayTime::create (0.7);
				CCActionInterval* pSeq		= (CCActionInterval*)CCSequence::create(CCEaseSineOut::create(pSpaw),pDelay,NULL);
				pNumSpr->runAction(pSeq);
			}

			pNumSpr->SetAlphaAction(0.5,0.5);

			m_pLNumSpr->Add(pNumSpr);
		}
		delete pNum;
	}
}

void CCMObjSprite::ClearShadow()
{
	if(getParent() == NULL)
		return;
	while(m_pLSprShadow->Count)
	{
		CCSprite*	pSpr	= (CCSprite*)m_pLSprShadow->Delete(0);
		getParent()->removeChild(pSpr,true);
	}
}

void CCMObjSprite::AddShadow( cocos2d::CCSprite*	pSrcSpr,cocos2d::CCSpriteFrame* pSprFrm )
{
	if(pSprFrm == NULL || pSrcSpr == NULL)
		return;
	CCSprite* pSpr	= CCSprite::createWithSpriteFrame(pSprFrm);
	//pSpr->initWithTexture(pSprSrc->getTexture(),pSprSrc->getTextureRect(),pSprSrc->isTextureRectRotated());
	//pSpr->setColor(ccc3(0,0,100));
	pSpr->setPosition(ccpAdd(getPosition(),pSrcSpr->getPosition()));
	pSpr->setFlipX(m_pMSprObj->byDirect == 6);
	pSpr->runAction(CCFadeOut::create(1.5f));
	pSpr->setAnchorPoint(ccp(0.5,0.5));
	pSpr->setScale(pSrcSpr->getScale());
	getParent()->addChild(pSpr);
	m_pLSprShadow->Add(pSpr);
}


void CCMObjSprite::_CheckToopTip()
{
	if(m_pMSprObj->pSkin == NULL)
		return;
	if(m_pTipForm == NULL)
	{
		if(m_pMSprObj->pTalkInfo && m_pMSprObj->pTalkInfo->m_bIsDel == false && m_pMSprObj->pTalkInfo->m_pDataSTC->bNeedRefresh)
		{
			m_pTipForm	= new TipFormLayer(m_pMSprObj->pTalkInfo);
			m_pTipForm->m_pSpecialText->bLimitView	= false;
			CCSize	size	= getContentSize();
			m_pTipForm->setPosition(ccp(size.width/2-100/POINT_PIX_CNT,m_pMSprObj->GetTitleH()/POINT_PIX_CNT+50/POINT_PIX_CNT));
			addChild (m_pTipForm,99);
		}
	}
	else
	{
		if(m_pTipForm->CheckUpdate () == false)
		{
			removeChild (m_pTipForm,false);
			//delete m_pTipForm;
			m_pTipForm	= NULL;
		}
		else
		{
			m_pTipForm->setPositionY(m_pMSprObj->GetTitleH()/POINT_PIX_CNT+(m_pTipForm->m_pSpecialText->nMaxHeight+50)/POINT_PIX_CNT);
		}
	}
}

void CCMObjSprite::SetAlphaVal( int nAlpha )
{
	if(m_pSprBody && m_pSprBody->getOpacity() != nAlpha)
	{
		m_pSprBody->setOpacity (nAlpha);
		if(m_pSprWeopon)
			m_pSprWeopon->setOpacity (nAlpha);
		if(m_pSprRide)
			m_pSprRide->setOpacity (nAlpha);
		if(m_pSprRideEx)
			m_pSprRideEx->setOpacity (nAlpha);
	}
}

void CCMObjSprite::_checkMark()
{
	BYTE	byMark	= 0;
	switch(m_pMSprObj->byType)
	{
	case MAPOBJ_ENEMY:
		{
			//MapAIObj* pObj	= (MapAIObj*)m_pMSprObj->pMObj;
			//if(pObj->pBaseCfg && ((EnemyBase*)pObj->pBaseCfg)->pPath && pObj->nHP > 0)
				//byMark	= ((EnesmyBase*)pObj->pBaseCfg)->pPath->byMark;
		}
		break;
	case MAPOBJ_NPC:
		{
			MapAIObj* pObj	= (MapAIObj*)m_pMSprObj->pMObj;
			byMark	= pObj->pPathObj->byMark;
		}
		break;
	}
	char	szTmp[64];
	switch(byMark)
	{
	case 1:
		strcpy(szTmp,"ui/TB_MAP1.png");
		break;
	case 2:
		strcpy(szTmp,"ui/TB_MAP2.png");
		break;
	case 3:
		strcpy(szTmp,"ui/TB_MAP3.png");
		break;
	case 4:
		strcpy(szTmp,"ui/TB_MAP4.png");
		break;
	}
	if(byMark)
	{
		if(m_pSprMark	== NULL)
		{
			m_pSprMark	= CCSprite::create();
			addChild(m_pSprMark);
			m_pSprMark->setPosition(ccp(0,(m_pMSprObj->GetTitleH()+28)/POINT_PIX_CNT));
		}
		m_pSprMark->initWithFile(szTmp);
	}
	else
	{
		if(m_pSprMark)
		{
			removeChild(m_pSprMark,true);
			m_pSprMark	= NULL;
		}
	}

}

void CCMObjSprite::_clearSkinPList( ObjSkinInfo* pSkin,BYTE bType )
{
	if(pSkin == NULL)
		return;
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	if(pSkin)
	{
		for (int i = 0; i < pSkin->bPartCnt; i++)
		{
			char	szList[256];
			sprintf(szList,"skin/%d/%d.plist",pSkin->nRealID,i+1);
			ResManager::GetInstance().SubPList(szList,pSkin->nRealID,bType);
		}
	}
}

