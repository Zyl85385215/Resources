#include "cocos2d.h"
#include "cocos-ext.h"
#include "CCEGLView.h"
#include "AppDelegate.h"
//#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
//#include "MapLayer.h"
//#include "InterfaceLayer.h"
#include "UpdateScene.h"
#include "GameScene.h"
#include "GameLoadScene.h"
#include "GlobalDefine.h"
//#ifndef WIN32
//#include "FMLayerWebView.h"
//#endif
#ifdef USE_ANDROID 
#include "JniUtil.h"
#endif

#ifdef WIN32
#include "debug_def.h"
//#include ".\..\vld.h"
#endif


using namespace CocosDenshion;

USING_NS_CC;

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
    SimpleAudioEngine::end();
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
	CCLog("1111111111APPStart111");
    // enable High Resource Mode(2x, such as iphone4) and maintains low resource on other devices.
   // pDirector->enableRetinaDisplay(true);
	TargetPlatform target = getTargetPlatform();
    
//#ifndef WIN32
//	FMLayerWebView::SFN_PlayerMove("1");
//    FMLayerWebView::SFN_InitOtherLib();
//#endif

	if (target == kTargetIpad)
	{
		// ipad

		g_nDFScreenWidth	= 1024;
		g_nDFScreenHeight	= 768;
		g_nDFScreenPix		= 1;

		//{
		//	// ipad hd
		//	CCFileUtils::sharedFileUtils()->setResourceDirectory("ipadhd");
		//}
		//else 
		//{
		//	CCFileUtils::sharedFileUtils()->setResourceDirectory("ipad");
		//}
	}
	else if (target == kTargetIphone)
	{
		// iphone
	}
	else
	{
		g_nDFScreenPix	= 1;
	}

#ifdef USE_ANDROID
	JniUtil jni;
	char* temp=jni.getAndSDpath();
	char path[1024];
	sprintf(path,"%s/rpgwar/res/",temp);
	//	std::string wPath=CCFileUtils::sharedFileUtils()->getWriteablePath();
	CCFileUtils::sharedFileUtils()->setResourceDirectory(path);
#endif

	CCEGLView* eglView=CCEGLView::sharedOpenGLView();
	nowScreenHeight=eglView->getFrameSize().height*POINT_PIX_CNT;
	nowScreenWidth=eglView->getFrameSize().width*POINT_PIX_CNT;
	g_nDFScreenWidth=eglView->getFrameSize().width;
	g_nDFScreenHeight=eglView->getFrameSize().height;
	if(nowScreenHeight > 900)
	{
		nowRateW=nowScreenWidth/1440.0f;
		nowRateH=nowScreenHeight/900.0f;
		g_nDFScreenWidth=1440.0f;
		g_nDFScreenHeight=900.0f;
	}
	else{
		if(nowScreenHeight<640){
			if((nowScreenHeight/640.0f-nowScreenWidth/960.0f)<0){
				g_nDFScreenWidth=nowScreenWidth/(nowScreenHeight/640.0f);
				g_nDFScreenHeight	= 640;
				nowRateH=nowScreenHeight/640.0f;//DJ
				nowRateW=nowRateH;

			}else{
				g_nDFScreenWidth	= 960;
				g_nDFScreenHeight=nowScreenHeight/(nowScreenWidth/960.0f);
				nowRateW=nowScreenWidth/960.0f;
				nowRateH=nowRateW;	
			}

		}
	}
	//nowScreenHeight	= 640;
	//nowScreenWidth	= 960;
//	eglView->setDesignResolutionSize(320, 480, kResolutionExactFit);
//#ifndef WIN32
//	FMLayerWebView::SFN_GetScreenSize(g_nDFScreenWidth,g_nDFScreenHeight,g_nDFScreenPix);
//#endif

    // turn on display FPS
#ifdef WIN32
	pDirector->setDisplayStats(true);		//显示帧数
	InstallMyExceptHandle();
#endif
	//pDirector->setProjection (kCCDirectorProjection2D);		//去掉边缘模糊效果,会引起抖动

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / RUN_FRAME);
	CCFileUtils::sharedFileUtils()->setPopupNotify (false);
	//CCTexture2D::PVRImagesHavePremultipliedAlpha (true);
	CCLog("1111111111APPStart");
	g_pTPObj	= new IosThirdPlat();
	// create a scene. it's an autorelease object
#ifdef USE_ANDROID
	PreLoginScene*	pRunScene	= PreLoginScene::create ();
#else
	GameLoadScene*	pRunScene	= GameLoadScene::create ();
#endif
	//pLoginScene->setPosition(ccp(0,100));
    pDirector->runWithScene(pRunScene);
    
    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();

    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();

    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
