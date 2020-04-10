#pragma once
#include "cocos2d.h"
#include "GlobalDefine.h"
#include "singleton.h"
#include "xnlist.h"
#ifdef USE_ANDROID
#include "JniHelper.h"
#include <stdio.h>
#include <jni.h>
#endif
class JniUtil
{
public:
JniUtil();
virtual ~JniUtil();
#ifdef USE_ANDROID


char* getMacAddress();
int		getSystemVersion();
int		getTPlatID();
char* getAndDevId();
char* getAndIDFA();
char* getAndSDpath();
void	initResToSDCard();
int		getUnZipCnt();
int goLogin();
//void goBBS(char* uName,char * pwd);
void	goTpCenter();
void goPay(int nVal,int nSvrId,char* szAcc, char* szOrder);
void Update();
void OtherEvent(int nVal);
int		getLoginStep();
int		getLoginID();
char*	getLoginToken();
int		getLockInUpdate();
int		getCenterType();

//void	TPResetVersionFile();		//清理资源文件
char*	TPOpenVersionUrl(char*	szUrl,char* szOut);
int		TPDownZipFile(char*	szUrl,char*	szSaveName);
int		TPUnZipFile(char*	szName);
void	TPSubmitUser(int uin,int nSvr,int nLv, char* strName, char* szZone);

#endif
};

class _ResDownObj
{
public:
	int		nLvLimit;
	char*	szUrl;
};

class ResControl	: public CSingleton<ResControl>
{
public:
	ResControl();
	~ResControl();

	void	Load(char* szFile);
	_ResDownObj*	GetByLv(int nLv);
	xnList*		m_pList;
};