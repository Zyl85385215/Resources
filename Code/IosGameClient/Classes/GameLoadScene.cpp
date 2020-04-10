#include "GameLoadScene.h"
#include "xnExtstr.h"
#include <pthread.h>
#include "LoginScene.h"
#ifdef USE_ANDROID
#include "JniUtil.h"
#endif
//-------------------------------------------
GameLoadScene::GameLoadScene()
{
	m_pHttpVManager		= NULL;
}

GameLoadScene::~GameLoadScene()
{
	if(m_pHttpVManager)
		delete m_pHttpVManager;
}

void GameLoadScene::StartGame()
{
	LoginScene*	pLoginScene	= LoginScene::create ();
	cocos2d::CCDirector::sharedDirector ()->replaceScene (pLoginScene);
}


void GameLoadScene::onEnter()
{
	CCScene::onEnter();
	scheduleUpdate ();
	_initBackSprite();
//	int	nUnZipV	= LoadIntegerFromXML("UnZipVersion",0);
//
//	int		nCurPos	= 0;
//#ifdef USE_ANDROID
//	JniUtil jni;
//	nCurPos	= jni.getUnZipCnt();
//#endif
//	if(nUnZipV < RESOURCE_VERSION || nCurPos >= 999999)
//	{
//		CCLog("11111UnZip");
//#ifdef USE_ANDROID
//		jni.initResToSDCard();
//#endif
//		//pthread_create(&s_unZipThread, NULL, UnZipThread, this);
//	}
//	else
//		m_bUnZipOk	= 2;
}


void GameLoadScene::onExit()
{
	CCScene::onExit();


}

void GameLoadScene::update( float dt )
{
	CCScene::update(dt);
	g_pTPObj->TP_Update();
	if(g_pTPObj->nLockInUpdate == 0)
	{//第三方更新完才开始初始化游戏相关
		g_pTPObj->nLockInUpdate	= 99;

		UrlUpdateCfg::GetInstance().Clear();

		char	szRead[10240];
		int		nSize	= 0;
		char*	szTmp	= NULL;
#ifdef USE_ANDROID
		std::string strResUrl	= LoadStringFromXML("ResUrl1","");
		char* szResUrl1		= strdup(strResUrl.c_str());
		if(strlen(szResUrl1)>4)
		{
			szTmp	= gfun_TPOpenVersionUrl(szResUrl1,szRead,nSize);
			if(szTmp && strlen(szTmp))
			{
				UrlUpdateCfg::GetInstance().LoadMemory(szTmp);
			}
		}

		strResUrl	= LoadStringFromXML("ResUrl2","");
		char* szResUrl2		= strdup(strResUrl.c_str());

		if(strlen(szResUrl2)>4)
		{
			szTmp	= gfun_TPOpenVersionUrl(szResUrl2,szRead,nSize);
			if(szTmp && strlen(szTmp))
			{
				UrlUpdateCfg::GetInstance().LoadMemory(szTmp);
			}
		}
#endif
		
		m_pHttpVManager	= new HttpVersionManager();
		int		nCurVersion	= LoadIntegerFromXML("DataVersion",0);
		int		nBaseVersion	= LoadIntegerFromXML("BaseVersion",0);
		if(nBaseVersion < BASERES_VERSION)
		{
			gfun_TPResetVersionFile();
			SaveIntegerToXML("BaseVersion",BASERES_VERSION);
			SaveToXMLOK();
			nCurVersion	= RESOURCE_VERSION;
		}
		else if(nCurVersion < RESOURCE_VERSION)
		{
			if(nCurVersion != 0)
				gfun_TPResetVersionFile();

			nCurVersion	= RESOURCE_VERSION;
		}
		
		szTmp	= gfun_TPOpenVersionUrl("http://www.mejoygame.com/"HTTP_UPDATE_FILE"/ViersionList.txt",szRead,nSize);
		if(szTmp && strlen(szTmp))
		{
			UrlUpdateCfg::GetInstance().LoadMemory(szTmp);
		}

		for(int i = 0; i < UrlUpdateCfg::GetInstance().m_pListUrls->Count;i++)
		{
			UrlTaskInfo*	pUTI	= (UrlTaskInfo*)UrlUpdateCfg::GetInstance().m_pListUrls->Items[i];
			if(pUTI->nVID > nCurVersion || pUTI->nVID == 0)
			{//提交下载请求
				m_pHttpVManager->RequestFile(pUTI);
			}
		}
		if(m_pHttpVManager->m_pListDU->Count)
		{
			
		}
		else
		{
			StartGame();
			return;
		}
	}
	if(!m_pHttpVManager)
		return;

	if(m_pHttpVManager->m_pListDU->Count == 0)
	{//没有需要更新的
		StartGame();
		return;
	}
#ifdef USE_ANDROID
	JniUtil jni;
	int nOkCnt	= jni.getUnZipCnt();
#else
	int nOkCnt	=	0;
	for (int i = 0; i < m_pHttpVManager->m_pListDU->Count; i++)
	{
		UrlTaskInfo*	pUTI	= (UrlTaskInfo*)m_pHttpVManager->m_pListDU->Items[i];
		if(pUTI->bDownOK)
		{
			nOkCnt++;
		}
	}
#endif
	
	if(nOkCnt == m_pHttpVManager->m_pListDU->Count)
		m_pHttpVManager->StartUnZip();
	if(m_pHttpVManager->GetProgress() >= 100)
	{//完成更新后刷新版本号
		if(m_pHttpVManager->GetEndVersion())
			SaveIntegerToXML("DataVersion",m_pHttpVManager->GetEndVersion());
#ifdef USE_ANDROID
		SaveStringToXML("ResUrl1","1");
		SaveStringToXML("ResUrl2","1");
#endif
		SaveToXMLOK();
		StartGame();
		return;
	}

	if(xnGetTickCount()/50%20==0)
	{//更新界面
		int nPercent	= m_pHttpVManager->GetProgress();

		if(nPercent == -1)
		{
			m_pProTime->setPercentage(0);

			//m_pSprLight->setPositionX(130/POINT_PIX_CNT + nPercent*766/POINT_PIX_CNT/100);
			char	szTmp[256];
			sprintf(szTmp,"更新失败, 请确认网络连接后重新启动程序");
			m_pLabTxt->setString(UTEXT_CHINE(szTmp));
			StartGame();
		}
		else
		{
			m_pProTime->setPercentage(nPercent);

			//m_pSprLight->setPositionX(130/POINT_PIX_CNT + nPercent*766/POINT_PIX_CNT/100);
			char	szTmp[256];
			int		nSizeKB	= m_pHttpVManager->m_dwTotalSize /1024;
			sprintf(szTmp,"正在更新资源包(%d.%03dMB),进度%d%%...",nSizeKB/1000,nSizeKB%1000,	nPercent);
			m_pLabTxt->setString(UTEXT_CHINE(szTmp));
		}
	}

}

void GameLoadScene::_initBackSprite()
{
	//setScaleX(nowScreenWidth/1024.0);
	//setScaleY(nowScreenHeight/768.0);
	m_pSpriteBack[0]		= GFun_InitSpriteByJPG("other/SKY.jpg");
	m_pSpriteBack[0]->setPosition(ccp(nowScreenWidth/2/POINT_PIX_CNT,nowScreenHeight/2/POINT_PIX_CNT));
	m_pSpriteBack[0]->setScaleX(nowScreenWidth/1024.0);
	m_pSpriteBack[0]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSpriteBack[0],-2);
	m_pSpriteBack[1]		= CCSprite::create("other/TREE.png");
	m_pSpriteBack[1]->setPosition(ccp(nowScreenWidth/2/POINT_PIX_CNT,nowScreenHeight/2/POINT_PIX_CNT));
	m_pSpriteBack[1]->setScaleX(nowScreenWidth/1024.0);
	m_pSpriteBack[1]->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSpriteBack[1],0);

	m_pSpriteCloud[0]	= CCSprite::create("other/YUN.png");
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
	m_pSprBack->setAnchorPoint(ccp(0.5,0.5));
	m_pSprBack->setPosition(ccp(nowScreenWidth/2/POINT_PIX_CNT,150/POINT_PIX_CNT));
	m_pSprBack->setScaleX(nowScreenWidth/1024.0);
	m_pSprBack->setScaleY(nowScreenHeight/768.0);
	addChild(m_pSprBack,1);
	m_pProTime		= CCProgressTimer::create(CCSprite::create ("ui/LOADBAR.png"));
	m_pProTime->setType(kCCProgressTimerTypeBar);
	m_pProTime->setBarChangeRate(ccp(1, 0));
	m_pProTime->setMidpoint (ccp(0,0.5f));
	m_pProTime->setScaleX(nowScreenWidth/1024.0);
	m_pProTime->setScaleY(nowScreenHeight/768.0);
	//m_pProTime->retain();
	m_pProTime->setAnchorPoint(ccp(0.5,0.5));
	m_pProTime->setPosition(ccp(nowScreenWidth/2/POINT_PIX_CNT,150/POINT_PIX_CNT));
	m_pProTime->setPercentage(0);
	addChild(m_pProTime,2);
	//m_pSprLight	= CCSprite::create("ui/LOADLIGHT.png");
	//m_pSprLight->setAnchorPoint(ccp(0.8,0.5));
	//m_pSprLight->setPosition(ccp(512/POINT_PIX_CNT,150/POINT_PIX_CNT));
	//addChild(m_pSprLight,3);

	m_pLabTxt		= CCLabelTTF::create(UTEXT_CHINE("正在加载游戏..."),DEFAULT_FONT,24/POINT_PIX_CNT);
	m_pLabTxt->setAnchorPoint (ccp(0.5,0.5));
	m_pLabTxt->setPosition (ccp(nowScreenWidth/2/POINT_PIX_CNT,70/POINT_PIX_CNT));
	m_pLabTxt->setColor (ccc3(0,250,22));
	addChild(m_pLabTxt,4);
}

void GameLoadScene::_updateCheck()
{
	

}
