#include "GameScene.h"
#include "InterfaceLayer.h"
#include "SoundCenter.h"
#include "ReconnectGame.h"
#include "EffectObj.h"
#include "MainMenu.h"
//#include "SkillConfig.h"
//#include "ItemConfig.h"
//#include "StateConfig.h"
#include "ParticleLayer.h"
//#include "EquipConfig.h"
#include "GuideLayer.h"
#include "GuideCenter.h"
#include "MapLayer.h"
#include "MapControl.h"
#include "EffectSprite.h"
#include "ParticleCenter.h"
#include "TextConfig.h"
#include "MapTile.h"
//#include "HorseConfig.h"
//#include "SlaveConfig.h"
//#include "EnemyConfig.h"
//#include "MapStaticCfg.h"
//#include "UserConfig.h"
//#include "MallConfig.h"
//#include "QuestConfig.h"
//#include "FaceConfig.h"
//#include "MapCopyConfig.h"
//#include "VipConfig.h"
//#include "ActivedegreeCfg.h"
#include "ResManage.h"
#ifdef WIN32
#include "debug_def.h"
#endif

using namespace cocos2d;
extern void GFun_ChgSpriteByJPG(CCSprite* pSprTmp, char* szFile);
GameScene::GameScene()
{
	pSprUp		= NULL;
	pSprDown	= NULL;
	m_pLoadingTxt = NULL;
	m_bChg = true;
	m_bShowEff = false;
}

GameScene::~GameScene()
{

}

void EventTestEff(InterfaceEvent * pEvent,void * param)
{
	if(pEvent->event != INTERFACE_EVENT_CLICK)
		return;

	char* szTxt	= pEvent->pComponent->pParent->GetComponent ("num")->GetEdit ();
	int nEff	= atoi(szTxt);
	pEvent->pComponent->pParent->GetComponent ("eff")->SetEffect (nEff,-1);
}

char* GetLoadingSpr()
{
	int nNum = 1+ rand()%4;
	char szTxt[32];
	sprintf(szTxt,"ui/LOADING%d.jpg",nNum);
	char* pRlt = strdup(szTxt);
	return pRlt;
}

void GameScene::_CheckLoadingEff(char* szName)
{
	m_bShowEff = false;

	int nNum = 0;
	sscanf(szName,"ui/LOADING%d.jpg",&nNum);
	if (nNum == 1)
		m_bShowEff = true;
}

void GameScene::onEnter()
{
	CCScene::onEnter ();
	srand(xnGetTickCount());
	scheduleUpdate();
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	cache->addSpriteFramesWithFile("icon/num/1.plist");
	cache->addSpriteFramesWithFile("icon/item/1.plist");

	if(gfun_CheckSprCache("icon/item/4.plist",false))
	{
		cache->addSpriteFramesWithFile("icon/item/2.plist");
		cache->addSpriteFramesWithFile("icon/item/3.plist");
		cache->addSpriteFramesWithFile("icon/item/4.plist");
	}


	cache->addSpriteFramesWithFile("icon/skill/1.plist");
	cache->addSpriteFramesWithFile("icon/fuckskill/1.plist");
	cache->addSpriteFramesWithFile("icon/map/1.plist");
	cache->addSpriteFramesWithFile("icon/head/1.plist");
	cache->addSpriteFramesWithFile("icon/activity/1.plist");
	cache->addSpriteFramesWithFile("icon/state/1.plist");

	//ReconnectModel::GetInstance ().SetModelWorked (true);

	//if(pMainInterface	== NULL)
	//	pMainInterface	= new DInterface("ui/Interface.IFP");

	if(g_pLayerIF == NULL)
	{
		g_pLayerIF	= InterfaceLayer::create();
		g_pLayerIF->retain();
	}
	addChild(g_pLayerIF,5,5);

	if(pChar == NULL)
		pChar	= new UserData();

	//if(pLuaStateManager == NULL)
	{
// 		pLuaStateManager = new CLuaStateManagerPlus();
// 		SetLuaStateManager(pLuaStateManager);
	}

	if(pMainMenu == NULL)
		pMainMenu	= new MainMenu();
	if(g_pSoundCenter == NULL)
		g_pSoundCenter	= new CSoundCenter();
	if(g_pMapCenter	== NULL)
		g_pMapCenter	= new MapControl();
	if(pGuideCenter)
		delete pGuideCenter;

	pGuideCenter	= new GuideCenter();
	if(pParticleCenter == NULL)
		pParticleCenter	= new ParticleCenter();

	pCtrlLayer	= ControlLayer::create();
	addChild(pCtrlLayer,2,2);
	pMapLayer	= MapLayer::create();
	addChild(pMapLayer,1,1);
	pMapLayer->m_pCtrlLayer	= pCtrlLayer;

	//EffectCenter::GetInstance ().InitFile ("cfg/effectcfg.ini");

	pMainInterface->GetDForm("map")->Open();
	pMainInterface->GetDForm("team")->Open();
	pMainInterface->GetDForm("main7")->Open(3);
	pMainInterface->GetDForm("main5")->Open(5);
	pMainInterface->GetDForm("liaotian2")->Open();
	pMainInterface->GetDForm("main4")->Open();
	pMainInterface->GetDForm("gonggao1")->Open();

	char* szTmp = GetLoadingSpr();
	_CheckLoadingEff(szTmp);
	m_pSprLoading	= GFun_InitSpriteByJPG(szTmp);
	free(szTmp);
	m_pSprLoading->retain();
	m_pSprLoading->setAnchorPoint(ccp(0.5,0.5));
	if(nowScreenWidth != 1024)
		m_pSprLoading->setScaleX(nowScreenWidth/1024.0);
	if(nowScreenHeight != 768)
		m_pSprLoading->setScaleY(nowScreenHeight/768.0);
	m_pSprLoading->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/2));
	addChild(m_pSprLoading,90,90);
	m_pSprBack	= CCSprite::create("ui/LOADBACK.png");
	m_pSprBack->retain();
	m_pSprBack->setAnchorPoint(ccp(0.5,0.5));
	m_pSprBack->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5));
	addChild(m_pSprBack,91,91);
	m_pProTime		= CCProgressTimer::create(CCSprite::create ("ui/LOADBAR.png"));
	m_pProTime->setType(kCCProgressTimerTypeBar);
	m_pProTime->setBarChangeRate(ccp(1, 0));
	m_pProTime->setMidpoint (ccp(0,0.5f));
	m_pProTime->retain();
	m_pProTime->setAnchorPoint(ccp(0.5,0.5));
	m_pProTime->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5));
	m_pProTime->setPercentage(50);
	m_pProTime->setPercentage(0);
	addChild(m_pProTime,92,92);
	m_pSprLight	= CCSprite::create("ui/LOADLIGHT.png");
	m_pSprLight->retain();
	m_pSprLight->setAnchorPoint(ccp(0.8,0.5));
	m_pSprLight->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,nowScreenHeight/POINT_PIX_CNT/5));
	m_pSprLight->setVisible(false);
	addChild(m_pSprLight,93,93);

	m_pSprEffBell	= CCEffectSprite::create(EffectCenter::GetInstance().GetEffObj(208));
	m_pSprEffBell->retain();
	m_pSprEffBell->setAnchorPoint(ccp(0.5,0.5));
	m_pSprEffBell->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT*99/200,nowScreenHeight/POINT_PIX_CNT*131/200));
	addChild(m_pSprEffBell,94,94);
	m_pSprEffBell->setVisible(m_bShowEff);

	ParticleLayer* pLayerParticle = ParticleLayer::create();
	addChild(pLayerParticle,3,3);
	GuideLayer*	pLayerGuide	= GuideLayer::create();
	addChild(pLayerGuide,10,4);

	m_pLoadingTxt		= CCLabelTTF::create(UTEXT_CHINE(""),DEFAULT_FONT,30/POINT_PIX_CNT);
	m_pLoadingTxt->setColor(ccc3(255,255,255));
	m_pLoadingTxt->setPosition(ccp(nowScreenWidth/POINT_PIX_CNT/2,35/POINT_PIX_CNT));
	addChild(m_pLoadingTxt,95,95);
	m_pLoadingTxt->setString(UTEXT_CHINE(LoadingTxtConfig::GetInstance().GetLoadingTxt()));

	pGuideCenter->SetGameSceneGuide();

	pMainMenu->CloseMessageBox();
}

void GameScene::onExit()
{
	CCScene::onExit ();
	m_pSprLoading->release();
	m_pSprEffBell->release();
	m_pSprBack->release();
	m_pSprLight->release();
	m_pProTime->release();
	m_pLoadingTxt = NULL;
	pMapLayer	= NULL;
	//CCSpriteFrameCache::sharedSpriteFrameCache()->removeUnusedSpriteFrames ();
	//CCTextureCache::sharedTextureCache ()->removeUnusedTextures ();
	//CocosDenshion::SimpleAudioEngine::sharedEngine()->stopBackgroundMusic(true);
}
void GameScene::TryUpdate()
{
	g_packetCenter.Update ();
	ASyncPListCenter::GetInstance().Update();
	DWORD	dwTickPass	= xnGetTickCount() - g_pMapCenter->m_dwTickEnter;
	if(dwTickPass< 1500)
	{
		if(m_bChg == false)
		{
			m_bChg = true;
			char* szTmp = GetLoadingSpr();
			_CheckLoadingEff(szTmp);
			GFun_ChgSpriteByJPG(m_pSprLoading,szTmp);
			free(szTmp);
			m_pLoadingTxt->setString(UTEXT_CHINE(LoadingTxtConfig::GetInstance().GetLoadingTxt()));
		}
		m_pSprLoading->setOpacity(255);
		m_pSprLoading->setVisible(true);
		m_pSprBack->setVisible(true);
		m_pSprLight->setVisible(true);
		m_pProTime->setVisible(true);
		m_pSprEffBell->setVisible(m_bShowEff);
		m_pLoadingTxt->setVisible(true);
		m_pSprEffBell->CheckEnd();
		m_pProTime->setPercentage(dwTickPass/15);

		if(dwTickPass < 600)
			m_pSprLight->setScaleY((dwTickPass+200)/800.0);
		else
			m_pSprLight->setScaleY((1800-dwTickPass)/1200.0);

		m_pSprLight->setPositionX(nowScreenWidth/POINT_PIX_CNT/2-384/POINT_PIX_CNT + dwTickPass*766/POINT_PIX_CNT/1500);
	}
	else if(dwTickPass < 2500)
	{
		m_bChg = false;
		m_pSprLoading->setOpacity((2500-dwTickPass)/4);
		m_pSprBack->setVisible(false);
		m_pSprLight->setVisible(false);
		m_pProTime->setVisible(false);
		m_pSprEffBell->setVisible(false);
		m_pLoadingTxt->setVisible(false);
		switch(g_pMapCenter->m_nBaseMapIndex){
		case 27:
		case 50:
			pMainMenu->PlayEffect(219);
			break;
		}
	}
	else
	{
		m_bChg = false;
		m_pSprEffBell->setVisible(false);
		m_pSprLoading->setVisible(false);
		m_pSprBack->setVisible(false);
		m_pSprLight->setVisible(false);
		m_pProTime->setVisible(false);
		m_pLoadingTxt->setVisible(false);
	}
	
	pChar->Update ();		//暂时改为先数据UPDATE,后界面UPDATE
	pMainMenu->Update ();

	static DWORD	s_dwTickGSSec	= xnGetTickCount();
	if(xnGetTickCount() - s_dwTickGSSec > 1000)
	{
		s_dwTickGSSec	= xnGetTickCount();
		pChar->UpdateSec();
		pMainMenu->UpdateSec();
		ResManager::GetInstance().Update();
	}
	g_pTPObj->TP_Update();
	ReconnectModel::GetInstance().Update();

	if(g_pTPObj->nLoginStep == 0)
	{//被注销了
		pChar->ReturnLogin();
	}
}

void GameScene::update( float dt )
{
	CCScene::update (dt);

#ifdef WIN32

		DWORD	dwTick	= xnGetTickCount();
		TryUpdate();
		if(xnGetTickCount() - dwTick > 100)
		{//更新速度跟不上,连续40秒?开始执行策略
			g_pMapCenter->m_dwFrmBusyTime++;
		}
		else if(g_pMapCenter->m_dwFrmBusyTime < 20)
			g_pMapCenter->m_dwFrmBusyTime	= 0;
#else
	try
	{
		DWORD	dwTick	= xnGetTickCount();
		TryUpdate();
		if(xnGetTickCount() - dwTick > 100)
		{//更新速度跟不上,连续40秒?开始执行策略
			g_pMapCenter->m_dwFrmBusyTime++;
		}
		else if(g_pMapCenter->m_dwFrmBusyTime < 20)
			g_pMapCenter->m_dwFrmBusyTime	= 0;
	}
	catch (...)
	{

	}
#endif
}

