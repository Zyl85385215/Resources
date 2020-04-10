#include "EffectSprite.h"
#include "SoundCenter.h"
#include "ResManage.h"
using namespace cocos2d;

CCEffectSprite* CCEffectSprite::create( EffectObj* pEff, int nDir /*= -1*/,bool bSound )
{
	if(pEff == NULL)
		return NULL;
	CCEffectSprite *pEffSprite = new CCEffectSprite();
	pEffSprite->m_szFrame	= NULL;
	pEffSprite->m_pEff	= pEff;
	pEffSprite->m_nSPID	= 0;
	pEffSprite->m_pSpVal	= NULL;
	pEffSprite->init ();
	pEffSprite->autorelease ();
	pEffSprite->m_pAnimate	= NULL;
	pEffSprite->m_dwEndTick	= 0;
	if(pEffSprite->InitSprAction(nDir) == false)
		return NULL;

	if(bSound)
	{
		//音效
		if(pEff->m_szSound && strlen (pEff->m_szSound))
		{
			char	szTmp[256];
			sprintf(szTmp,"Sound/%s",pEff->m_szSound);

			//if(pEff->m_nEffID == 63)
			//{
			//	g_pSoundCenter->PlayerSpEffect (szTmp,1500);
			//}
			//else
			g_pSoundCenter->PlayerEffect(szTmp);
		}
	}
	return pEffSprite;
}

CCEffectSprite::~CCEffectSprite()
{
	//if(m_pEff->m_szSound && strlen (m_pEff->m_szSound))
	//{
		//if(m_pEff->m_nEffID == 63 )
		//{
			//g_pSoundCenter->PlayerSpEffect ("",m_pEff->m_nEffID,0);
		//}
	//	//没循环播放会停止的
	//	char	szTmp[256];
	//	sprintf(szTmp,"sound/%s",m_pEff->m_szSound);
	//	CocosDenshion::SimpleAudioEngine::sharedEngine()->stopEffect(szTmp);
	//}

	if(m_szFrame)
	{
		//减少引用
		ResManager::GetInstance().SubPList(m_szFrame,m_pEff?m_pEff->m_nEffID:0,0);
		free(m_szFrame);
	}
	stopAllActions();
	if(m_pAnimate)
		m_pAnimate->release ();
}

bool CCEffectSprite::InitSprAction(int nDir)
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	CCAnimation* pAnimation	= CCAnimation::create();
	int nTotalFrm	= 0;
	bool	bFilpX	= false;
	while(1)
	{
		char	szTmp[256];
		switch(nDir)
		{
		case 7:
			nDir	= 3;
			bFilpX	= true;
			break;
		case 8:
			nDir	= 2;
			bFilpX	= true;
			break;
		case 6:
			nDir	= 4;
			bFilpX	= true;
			break;
		}
		if(m_pEff->m_nRunType	== 8)
			sprintf(szTmp,"effect/%d/%d/%d.png",m_pEff->m_nRealSprID,nDir,nTotalFrm+1);
		else if(m_pEff->m_nRunType == 6)
		{
			if(nDir == 1 || nDir == 5)
				sprintf(szTmp,"effect/%dx/%d.png",m_pEff->m_nRealSprID,nTotalFrm+1);
			else
				sprintf(szTmp,"effect/%d/%d.png",m_pEff->m_nRealSprID,nTotalFrm+1);
		}
		else
			sprintf(szTmp,"effect/%d/%d.png",m_pEff->m_nRealSprID,nTotalFrm+1);

		CCSpriteFrame* frame = cache->spriteFrameByName( szTmp );

		if(nTotalFrm == 0)
		{
			char	szFrame[64];
			sprintf(szFrame,"effect/%d.plist",m_pEff->m_nRealSprID);
			if(m_szFrame)
			{//减少引用
				ResManager::GetInstance().SubPList(m_szFrame,0,0);
				free(m_szFrame);
			}
			m_szFrame	= strdup(szFrame);

			if(frame == NULL)
			{//如果没图片就加载
				if(gfun_CheckSprCache(szFrame,false))
					cache->addSpriteFramesWithFile (szFrame);
				frame	= cache->spriteFrameByName (szTmp);
				if(frame == NULL)
					return false;
			}
			initWithSpriteFrame (frame);
		}
			
		if(frame == NULL)
			break;

		pAnimation->addSpriteFrame(frame);
		nTotalFrm++;
	}
	//cache->removeSpriteFramesFromFile(m_szFrame);
	if(gfun_CheckSprCache(m_szFrame,false))
		ResManager::GetInstance().AddPList(m_szFrame,m_pEff->m_nEffID,0);
	if(nTotalFrm)
	{
		pAnimation->setDelayPerUnit(1.0f*m_pEff->m_nTimeFrm/nTotalFrm/1000/g_byWarSpeed);
		CCAnimate* action = CCAnimate::create(pAnimation);

		//if(m_pEff->m_nLoopTime == 0)
		//	runAction(CCRepeatForever::create(action));
		//else
		{
			m_pAnimate	= action;
			m_pAnimate->retain ();
			m_nLoopTime	= m_pEff->m_nLoopTime;
			runAction(m_pAnimate);//CCRepeat::actionWithAction (action,m_pEff->m_nLoopTime));
		}
	}

	if(m_pEff->m_nX || m_pEff->m_nY)
		setAnchorPoint(ccp(0.5+m_pEff->m_nX/100.0,0.5-m_pEff->m_nY/100.0));

	if(nDir	!= -1 && m_pEff->m_nRunType	!= 8)
	{
		if(nDir > 180)
		{
			
			bFilpX	= true;
		}
		//else
			setRotation(nDir);

	}
	
	if(m_pEff->m_nScale != 100)
		setScale(m_pEff->m_nScale/100.0);

	if(m_pEff->byR != 255 || m_pEff->byG != 255 || m_pEff->byB != 255)
		setColor(ccc3(m_pEff->byR,m_pEff->byG,m_pEff->byB));

	if(bFilpX)
		setFlipX(true);

	return true;
}

bool CCEffectSprite::CheckEnd(bool bSound,int nDis)
{
	if(m_dwEndTick && xnGetTickCount () >= m_dwEndTick)
		return true;

	if(numberOfRunningActions() == 0)
	{
		if(m_pAnimate == NULL)
			return true;
		if(m_nLoopTime != 0)
		{
			m_nLoopTime--;
			if(m_nLoopTime == 0)
				return true;
		}

		runAction (m_pAnimate);
		if(bSound && m_pEff->m_szSound && strlen (m_pEff->m_szSound))
		{
			char	szTmp[256];
			sprintf(szTmp,"Sound/%s",m_pEff->m_szSound);
			//if(m_pEff->m_nEffID == 63)
			//{
			//	g_pSoundCenter->PlayerSpEffect (szTmp,nDis);
			//}
			//else
			g_pSoundCenter->PlayerEffect(szTmp);
		}
	}

	return false;
}

//===============================================================

CCNumberSprite* CCNumberSprite::create(int nNum, BYTE bType)
{
	CCNumberSprite *pNumSprite = new CCNumberSprite();

	pNumSprite->init ();
	pNumSprite->autorelease ();
	if(pNumSprite->InitSprAction(nNum,bType) == false)
		return NULL;

	return pNumSprite;
}

CCNumberSprite::~CCNumberSprite()
{

}

bool CCNumberSprite::InitSprAction( int nNum, BYTE bType )
{
	m_nSizeW	= 0;

	char	szSprFrmBase[256];
	char	szSprFrm[256];

	if(bType == 3)
	{//躲避
		sprintf(szSprFrm,"ui/num/crit/r/sb.png");
		CCSprite* pSpr	= CCSprite::createWithSpriteFrameName (szSprFrm);
		if(pSpr)
		{
			addChild (pSpr,0,99);
			pSpr->setPosition (ccp(m_nSizeW,0));

			m_nSizeW	+= pSpr->getContentSize ().width;
		}
	}
	else
	{
		if(bType == 0)
			sprintf(szSprFrmBase,"ui/num/normal/r/");
		else if(bType == 1)
			sprintf(szSprFrmBase,"ui/num/normal/g/");
		else if( bType == 2)
			sprintf(szSprFrmBase,"ui/num/crit/r/");
		else if( bType == 5)
			sprintf(szSprFrmBase,"ui/num/hurt/");

		if(nNum != 0)
		{
			sprintf(szSprFrm,"%sfh.png",szSprFrmBase);
			nNum	= abs(nNum);
			CCSprite* pSpr	= CCSprite::createWithSpriteFrameName (szSprFrm);
			if(pSpr)
			{
				addChild (pSpr,0,99);
				pSpr->setPosition (ccp(m_nSizeW,0));

				m_nSizeW	+= pSpr->getContentSize ().width;
			}
		}

		char	szTmp[32];
		sprintf(szTmp,"%d", nNum);

		int nCnt	= strlen (szTmp);
		for (int i = 0; i < nCnt; i++)
		{

			if(bType == 0)
				sprintf(szSprFrm,"%s%d.png",szSprFrmBase,szTmp[i]-'0');
			else
				sprintf(szSprFrm,"%s%d.png",szSprFrmBase,szTmp[i]-'0');
			CCSprite* pSpr	= CCSprite::createWithSpriteFrameName (szSprFrm);
			if(pSpr)
			{
				addChild (pSpr,0,i);
				pSpr->setPosition (ccp(m_nSizeW,0));

				m_nSizeW	+= pSpr->getContentSize ().width;
			}
		}
	}
	setContentSize (CCSize(m_nSizeW,10));
	setAnchorPoint (ccp(0.4,0));
	return true;
}

bool CCNumberSprite::CheckEnd()
{
	if(numberOfRunningActions ())
		return false;

	return true;
}

void CCNumberSprite::SetAlphaAction( float fTime,float fDelay )
{
	
	CCObject* child;
	CCARRAY_FOREACH(getChildren(), child)
	{
		CCNode* pNode = (CCNode*) child;
		CCActionInstant* pFade	= (CCActionInstant*)CCFadeOut::create(fTime);
		CCActionInstant* pDelay	= (CCActionInstant*)CCDelayTime::create(fDelay);
		CCActionInstant* pSeq	= (CCActionInstant*)CCSequence::create(pDelay,pFade,NULL);
		pNode->runAction(pSeq);
	}
}
