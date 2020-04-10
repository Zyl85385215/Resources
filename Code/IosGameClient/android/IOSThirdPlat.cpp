#include "IOSThirdPlat.h"
#include "stdio.h"
#include "stdlib.h"
#include "GlobalDefine.h"
#ifdef USE_ANDROID 
#include "JniUtil.h"
#endif

char*	g_szAPNToken	= NULL;
IosThirdPlat::IosThirdPlat()
{
	nEventMark	= 0;
	nLoginStep	= 0;
	nLoginMID	= 0;
	nLockInUpdate	= 0;
}

IosThirdPlat::~IosThirdPlat()
{

}


const char* IosThirdPlat::TP_GetMacAddress()
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.getAndDevId();
#endif
	return "PC";
}

const char* IosThirdPlat::TP_GetIDFA()
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.getAndIDFA();
#endif
	return "";
}

const char* IosThirdPlat::TP_GetDevID()
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.getAndDevId();
#else
	static char	szRtl[64];
	sprintf(szRtl,"sptest%08d",rand()%10000000);
	return szRtl;
#endif
}

int IosThirdPlat::TP_GetPlat()
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.getTPlatID();
#endif
	return 0;
}

bool IosThirdPlat::TP_OPenCenter()
{
#ifdef USE_ANDROID
	JniUtil jni;
	jni.goTpCenter();
#endif
	return false;
}

bool IosThirdPlat::TP_Login()
{
	nLoginStep	= 1;
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.goLogin();
#endif
	return false;
}

void IosThirdPlat::TP_RqPay( int nVal,int nSvrID, char* szAcc, char* szOrder )
{
#ifdef USE_ANDROID
	JniUtil jni;
	jni.goPay(nVal,nSvrID,szAcc,szOrder);
#endif
}

void IosThirdPlat::TP_Update()
{
#ifdef USE_ANDROID
	JniUtil jni;
	int nTmpStep	= jni.getLoginStep();
	if(nTmpStep != 2 || nLoginStep != 3)
		nLoginStep	= nTmpStep;
	
	nLoginMID	= jni.getLoginID();
	if(nLoginStep == 2)
		szToken	= jni.getLoginToken();
	if(nLockInUpdate != 99)
		nLockInUpdate	= jni.getLockInUpdate();
	jni.Update();
#endif
}

void IosThirdPlat::TP_OtherEvent( int nType,char* szUrl )
{
#ifdef USE_ANDROID
	JniUtil jni;
	jni.OtherEvent(nType);
#endif
}

int IosThirdPlat::TP_GetSystemVersion()
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.getSystemVersion();
#endif
	return 0;
}

int IosThirdPlat::TP_GetCenterType()
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.getCenterType();
#endif
	return -1;
}


void gfun_TPResetVersionFile()
{

}

char* gfun_TPOpenVersionUrl( char* szUrl,void* pData, int& nSize )
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.TPOpenVersionUrl(szUrl,(char*)pData);
#endif
	return "";
}

bool gfun_TPDownZipFile( char* szUrl,char* szSaveName )
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.TPDownZipFile(szUrl,szSaveName);
#endif
	return true;
}

bool gfun_TPUnZipFile( char* szName )
{
#ifdef USE_ANDROID
	JniUtil jni;
	return jni.TPUnZipFile(szName);
#endif
	return true;
}
