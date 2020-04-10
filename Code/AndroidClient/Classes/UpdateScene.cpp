#include "UpdateScene.h"
#include "xnExtstr.h"
#include "InterfaceLayer.h"
#include "xnExtstr.h"
#include "GameLoadScene.h"

//#ifndef WIN32
//#include "FMLayerWebView.h"
//#endif
#ifdef USE_ANDROID 
#include "JniUtil.h"
#endif

static pthread_t s_unZipThread;

//-------------------------------------------
PreLoginScene::PreLoginScene()
{
	m_bUnZipOk	= 0;
}

PreLoginScene::~PreLoginScene()
{

}
static void* UnZipThread(void* data)
{
	//((PreLoginScene*)data)->m_bUnZipOk	= 1;
	return 0;
}

void PreLoginScene::onEnter()
{
	CCScene::onEnter();
	CCLog("1111111111PreLoginSceneONENter");
	scheduleUpdate ();
	
	int	nUnZipV	= LoadIntegerFromXML("UnZipVersion",0);
	int		nCurPos	= 0;
#ifdef USE_ANDROID
	JniUtil jni;
	nCurPos	= jni.getUnZipCnt();
#endif
	int		nBaseVersion	= LoadIntegerFromXML("BaseVersion",0);
	if(nBaseVersion < BASERES_VERSION)
	{
		_initBackSprite();
		CCLog("11111UnZip");
#ifdef USE_ANDROID
		jni.initResToSDCard();
#endif
		SaveIntegerToXML("BaseVersion",BASERES_VERSION);
		SaveToXMLOK();
	}
	else if(nUnZipV < RESOURCE_VERSION || nCurPos >= 999999)
	{
		_initBackSprite();
		CCLog("11111UnZip");
#ifdef USE_ANDROID
		jni.initResToSDCard();
#endif
		//pthread_create(&s_unZipThread, NULL, UnZipThread, this);
	}
	else
		m_bUnZipOk	= 2;
}


void PreLoginScene::onExit()
{
	CCScene::onExit();

	
}

void PreLoginScene::update( float dt )
{
	CCScene::update(dt);

	if(xnGetTickCount()/50%20==0)
		_updateBackView();
}

void PreLoginScene::_initBackSprite()
{
	m_pSpriteBack[0]		= GFun_InitSpriteByJPG("other/SKY.jpg");
	m_pSpriteBack[0]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/2));
	m_pSpriteBack[0]->setScaleX(nowScreenWidth/1024.0);
	m_pSpriteBack[0]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSpriteBack[0],-2);
	m_pSpriteBack[1]		= CCSprite::create("other/TREE.png");
	m_pSpriteBack[1]->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/2));
	m_pSpriteBack[1]->setScaleX(nowScreenWidth/1024.0);
	m_pSpriteBack[1]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSpriteBack[1],0);

	m_pSpriteCloud[0]	= CCSprite::create("other/YUN.png");
	m_pSpriteCloud[0]->setScaleY(nowScreenHeight/768.0);
	m_pSpriteCloud[0]->setAnchorPoint(ccp(0,1));
	m_pSpriteCloud[0]->setPosition(ccp(0,SCREEN_SIZE_H));
	addChild(m_pSpriteCloud[0],-1);

	{
		CCActionInterval* go = CCMoveTo::create (30,ccp(-1922/POINT_PIX_CNT,SCREEN_SIZE_H));
		CCActionInterval* hide	= CCMoveTo::create (0.1f,ccp(-1922/POINT_PIX_CNT,0));
		CCActionInterval* goback	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,0));
		CCActionInterval* show	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,SCREEN_SIZE_H));
		CCActionInterval* go1	= CCMoveTo::create (30,ccp(0,SCREEN_SIZE_H));

		CCFiniteTimeAction* seq = CCSequence::create(go, hide, goback, show, go1,NULL);

		m_pSpriteCloud[0]->runAction( (CCRepeatForever::create((CCActionInterval*) seq) ));
		m_pSpriteCloud[0]->getTexture ()->setAliasTexParameters ();
	}

	m_pSpriteCloud[1]	= CCSprite::create("other/YUN.png");
	m_pSpriteCloud[1]->setScaleY(nowScreenHeight/768.0);
	m_pSpriteCloud[1]->setAnchorPoint(ccp(0,1));
	m_pSpriteCloud[1]->setPosition(ccp(1922/POINT_PIX_CNT,SCREEN_SIZE_H));
	addChild(m_pSpriteCloud[1],-1);

	{
		CCActionInterval* go = CCMoveTo::create (60,ccp(-1922/POINT_PIX_CNT,SCREEN_SIZE_H));
		CCActionInterval* hide	= CCMoveTo::create (0.1f,ccp(-1922/POINT_PIX_CNT,0));
		CCActionInterval* goback	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,0));
		CCActionInterval* show	= CCMoveTo::create (0.1f,ccp(1922/POINT_PIX_CNT,SCREEN_SIZE_H));

		CCFiniteTimeAction* seq = CCSequence::create(go, hide, goback, show, NULL);

		m_pSpriteCloud[1]->runAction( (CCRepeatForever::create((CCActionInterval*) seq) ));
		m_pSpriteCloud[1]->getTexture ()->setAliasTexParameters ();
	}
	m_pSprBack	= CCSprite::create("ui/LOADBACK.png");
	m_pSprBack->retain();
	m_pSprBack->setAnchorPoint(ccp(0.5,0.5));
	m_pSprBack->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5));
	m_pSprBack->setScaleX(nowScreenWidth/1024.0);
	m_pSprBack->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSprBack,1);
	m_pProTime		= CCProgressTimer::create(CCSprite::create ("ui/LOADBAR.png"));
	m_pProTime->setType(kCCProgressTimerTypeBar);
	m_pProTime->setBarChangeRate(ccp(1, 0));
	m_pProTime->setMidpoint (ccp(0,0.5f));
	m_pProTime->retain();
	m_pProTime->setAnchorPoint(ccp(0.5,0.5));
	m_pProTime->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5));
	m_pProTime->setPercentage(0);
	m_pProTime->setScaleX(nowScreenWidth/1024.0);
	m_pProTime->setScaleY(nowScreenHeight/768.0);
	addChild(m_pProTime,2);
	m_pSprLight	= CCSprite::create("ui/LOADLIGHT.png");
	m_pSprLight->retain();
	m_pSprLight->setAnchorPoint(ccp(0.8,0.5));
	m_pSprLight->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5));
	m_pSprLight->setScaleX(nowScreenWidth/1024.0);
	m_pSprLight->setScaleY(nowScreenHeight/768.0);
	//m_pSprLight->setVisible(false);
	addChild(m_pSprLight,3);

	m_pLabTxt		= CCLabelTTF::create(UTEXT_CHINE("正在加载游戏..."),DEFAULT_FONT,24/POINT_PIX_CNT);
	m_pLabTxt->setAnchorPoint (ccp(0.5,0.5));
	m_pLabTxt->setPosition (ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5-80/POINT_PIX_CNT));
	m_pLabTxt->setColor (ccc3(0,250,22));
	addChild(m_pLabTxt,4);
}

void PreLoginScene::_updateBackView()
{
	if(m_bUnZipOk != 0)
	{
		CCLog("11111PreLoginSceneOK");
		if(m_bUnZipOk == 1)
		{//
			SaveIntegerToXML("UnZipVersion",RESOURCE_VERSION);
			SaveToXMLOK();
		}
		GameLoadScene*	pLogin	= GameLoadScene::create();
		cocos2d::CCDirector::sharedDirector()->replaceScene(pLogin);
		m_bUnZipOk	= 0;
	}
	else
	{
		int		nCurPos	=	1;
#ifdef USE_ANDROID
		JniUtil jni;
		nCurPos	= jni.getUnZipCnt();
#endif
		if(nCurPos	== -1)
		{
			m_bUnZipOk	= 1;
			return;
		}
		if(nCurPos >= 999999)
			nCurPos		= 1;
		int		nPercent	= nCurPos*100/4800;
		if(nPercent > 100)
			nPercent	= 100;
		m_pProTime->setPercentage(nPercent);

		if(nPercent < 40)
			m_pSprLight->setScaleY((nPercent+20)/60.0*nowScreenHeight/768.0);
		else
			m_pSprLight->setScaleY((120-nPercent)/80.0*nowScreenHeight/768.0);

		m_pSprLight->setPositionX((nowScreenWidth/POINT_PIX_CNT/2-384/POINT_PIX_CNT + nPercent*766/POINT_PIX_CNT/100));

		char	szTmp[256];
		sprintf(szTmp,"正在提取资源%d%%...",nPercent);
		m_pLabTxt->setString(UTEXT_CHINE(szTmp));
	}
}
