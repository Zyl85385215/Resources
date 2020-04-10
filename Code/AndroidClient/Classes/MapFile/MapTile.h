#pragma once

#include "cocos2d.h"
#include "xnlist.h"
#include "singleton.h"
#include <pthread.h>

class CMTileSprite : public cocos2d::CCSprite
{
public:
	static CMTileSprite* create(char* szFile);
	~CMTileSprite();
	void loadingTitlCallBack(cocos2d::CCObject *obj);

	bool	bIsLoaded;
};

class ASLoadObj	: public cocos2d::CCObject
{
public:
	ASLoadObj();
	virtual ~ASLoadObj();
	char*		m_szpList;
	char*		m_szpFile;
	cocos2d::CCTexturePVR*	m_pTexPvr;
	bool		m_bIsLoad;
};

class ASyncPListCenter	: public CSingleton<ASyncPListCenter>
{
public:
	ASyncPListCenter();
	~ASyncPListCenter();

	void		RqLoadPlist(char* szpList,char* szFile);
	void		Update();			//ͬһ�߳̾Ͳ���Ҫupdate������
	void	LoadPListThread();

	xnList*		m_pRQList;	//�����б�
	

	pthread_cond_t	loadPListSign;
	pthread_mutex_t loadPListLock;
};