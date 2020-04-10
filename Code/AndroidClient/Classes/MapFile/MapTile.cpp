#include "MapTile.h"
#include "GlobalDefine.h"

CreateSingleton(ASyncPListCenter);
static pthread_t loadPListThread;
static pthread_t loadMTileThread;
using namespace cocos2d;

CMTileSprite* CMTileSprite::create( char* szFile)
{
	CMTileSprite *pMTitl = new CMTileSprite();

	//#ifndef WIN32
	//	pMTitl->initWithFile(szFile);//("CloseNormal.png");
	//CCTextureCache::sharedTextureCache()->removeTextureForKey(szFile);
	//#else
	pMTitl->init ();	//���߳���IOS�����ڴ�й¶
	//ASyncPListCenter::GetInstance().RqLoadMTile(pMTitl,szFile);
	CCTextureCache::sharedTextureCache()->addImageAsync(szFile, pMTitl, callfuncO_selector(CMTileSprite::loadingTitlCallBack));
	//#endif
	pMTitl->bIsLoaded	= false;
	pMTitl->autorelease ();
	return pMTitl;
}

void CMTileSprite::loadingTitlCallBack( cocos2d::CCObject *obj )
{
	initWithTexture((CCTexture2D*)obj);
	CCTextureCache::sharedTextureCache()->removeTexture ((CCTexture2D*)obj);
	setAnchorPoint (ccp(0,1));
	if(g_bySvrLockClient/10%10 == 0)
		this->getTexture()->setAliasTexParameters();
	//pSpr	= CCSprite::createWithTexture ((CCTexture2D*)obj);
	//pSpr->autorelease ();
	//addChild (pSpr);

	bIsLoaded	= true;
}

CMTileSprite::~CMTileSprite()
{

}

static void* loadPListDeal(void* data)
{
	ASyncPListCenter::GetInstance().LoadPListThread();
	return 0;
}

void ASyncPListCenter::LoadPListThread()
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	while(1)
	{
		pthread_mutex_lock(&loadPListLock);
		for (int i = 0; i < m_pRQList->Count; i++)
		{
			ASLoadObj * pAS = (ASLoadObj *)m_pRQList->Items[i];
			if (pAS && !pAS->m_bIsLoad)
			{
				pAS->m_pTexPvr	= new CCTexturePVR();
				if(pAS->m_pTexPvr->initWithContentsOfFile(pAS->m_szpFile,true) == false)
				{
					pAS->m_pTexPvr	= NULL;
				}

				//pAS->m_pTex	= CCTextureCache::sharedTextureCache()->addImage();	//�ж��߳�����,m_pTextures����߳�
				pAS->m_bIsLoad	= true;
			}
		}
		pthread_cond_wait(&loadPListSign,&loadPListLock);
		pthread_mutex_unlock(&loadPListLock);
	}
}

ASyncPListCenter::ASyncPListCenter()
{
	m_pRQList	= xnList::Create();

	pthread_mutex_init(&loadPListLock, NULL);
	pthread_cond_init(&loadPListSign,NULL);

	pthread_create(&loadPListThread, NULL, loadPListDeal, this);
}

ASyncPListCenter::~ASyncPListCenter()
{
	while(m_pRQList->Count)
		delete (ASLoadObj*)m_pRQList->Delete(0);
	m_pRQList->Free();
}

void ASyncPListCenter::RqLoadPlist( char* szpList,char* szFile )
{
	if(g_bySvrLockClient%10)
	{
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile(szpList);
		return;
	}

	pthread_mutex_lock(&loadPListLock);

	for (int i = 0; i < m_pRQList->Count; i++)
	{
		ASLoadObj*	pAS	= (ASLoadObj*)m_pRQList->Items[i];
		if(strcmp(pAS->m_szpList,szpList) == 0)
		{
			pthread_mutex_unlock(&loadPListLock);
			return;
		}
	}

	ASLoadObj*	pAS	= new ASLoadObj();
	pAS->m_szpFile	= strdup(CCFileUtils::sharedFileUtils()->fullPathFromRelativePath(szFile));
	pAS->m_szpList	= strdup(szpList);
	m_pRQList->Add(pAS);
	pthread_mutex_unlock(&loadPListLock);

	pthread_cond_signal(&loadPListSign);
}

void ASyncPListCenter::Update()
{
	CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
	pthread_mutex_lock(&loadPListLock);
	for (int i = m_pRQList->Count - 1; i >= 0; i--)
	{
		ASLoadObj* pAS	= (ASLoadObj*)m_pRQList->Items[i];
		if(pAS->m_bIsLoad)
		{
			if(pAS->m_pTexPvr)
			{
				CCTexture2D* texture	= new CCTexture2D();
				if(texture->InitMuliData(pAS->m_pTexPvr))
				{
					texture->autorelease();
					//pAS->m_pTex->InitMuliData();
					cache->addSpriteFramesWithFile(pAS->m_szpList,texture);
				}
				else
					delete texture;
			}
			m_pRQList->Delete(i);
			delete pAS;
		}
	}
	pthread_mutex_unlock(&loadPListLock);
}

//void ASLoadObj::loadPListCallBack( cocos2d::CCObject *obj )
//{
//	m_bIsLoad	= true;
//	CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile(m_szpList,(CCTexture2D*)obj);
//	CCTextureCache::sharedTextureCache()->removeTexture ((CCTexture2D*)obj);
//}

ASLoadObj::ASLoadObj(  )
{
	m_szpList	= NULL;
	m_szpFile	= NULL;
	m_pTexPvr	= NULL;
	m_bIsLoad	= false;
}

ASLoadObj::~ASLoadObj()
{
	if(m_szpList)
		free(m_szpList);
	if(m_szpFile)
		free(m_szpFile);
}