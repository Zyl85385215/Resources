#include "JniUtil.h"
#include "xnExtstr.h"

CreateSingleton(ResControl);
USING_NS_CC;
JniUtil::JniUtil()
{

}

JniUtil::~JniUtil()
{

}
#ifdef USE_ANDROID
int JniUtil::getUnZipCnt()
{
	int b;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getUnZipCnt","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}

int JniUtil::getLoginStep()
{
	int b;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getLoginStep","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}
int JniUtil::getLoginID()
{
	int b;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getLoginID","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}
int JniUtil::getCenterType()
{
	int b = -1;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getCenterType","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}

int JniUtil::getLockInUpdate()
{
	int b = 0;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getLockInUpdate","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}

int JniUtil::getTPlatID()
{
	int b;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getTPID","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}

char* JniUtil::getAndDevId()
{
	static char szRtl[256]={0};
	jstring jstr;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getAndDevId","()Ljava/lang/String;"); 
	if (isHave) { 
		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
		char * tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
		strcpy(szRtl,tchar);
		jmi.env->ReleaseStringUTFChars(jstr,tchar);
	}
	return szRtl;
}

char* JniUtil::getAndIDFA()
{
	static char szRtl[256]={0};
	jstring jstr;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getAndIDFA","()Ljava/lang/String;"); 
	if (isHave) { 
		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
		char * tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
		strcpy(szRtl,tchar);
		jmi.env->ReleaseStringUTFChars(jstr,tchar);
	}
	return szRtl;
}

char* JniUtil::getLoginToken()
{
	static char szRtl[256]={0};
	jstring jstr;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getLoginToken","()Ljava/lang/String;"); 
	if (isHave) { 
		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
		char * tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
		strcpy(szRtl,tchar);
		jmi.env->ReleaseStringUTFChars(jstr,tchar);
	}
	return szRtl;
}

//char* JniUtil::getLoginID()
//{
//	char *pdeviceName=new char[1024];
//	jstring jstr;
//	JniMethodInfo jmi;//定义Jni函数信息结构体 
//	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getLoginID","()Ljava/lang/String;"); 
//	if (isHave) { 
//		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
//		char * tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
//		strcpy(pdeviceName,tchar);
//		jmi.env->ReleaseStringUTFChars(jstr,tchar);
//	}
//	return pdeviceName;
//}
//char* JniUtil::getLoginKey()
//{
//	char *pdeviceName=new char[1024];
//	jstring jstr;
//	JniMethodInfo jmi;//定义Jni函数信息结构体 
//	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getLoginKey","()Ljava/lang/String;"); 
//	if (isHave) { 
//		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
//		char * tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
//		strcpy(pdeviceName,tchar);
//		jmi.env->ReleaseStringUTFChars(jstr,tchar);
//	}
//	return pdeviceName;
//}

char* JniUtil::getAndSDpath()
{
	static char szRtl[256]={0};
	jstring jstr;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getSDPath","()Ljava/lang/String;"); 
	if (isHave) { 
		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
		char* tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
		strcpy(szRtl,tchar);
		jmi.env->ReleaseStringUTFChars(jstr,tchar);
	}
	return szRtl;
}

void JniUtil::initResToSDCard()
{
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","initResToSDCard","()V"); 
	if (isHave) { 
		jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID);
	}
}

int JniUtil::goLogin()
{
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	int b = 0;
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","goLogin","()I"); 
	if (isHave) { 
		b	= jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}

//void JniUtil::goBBS( char* uName,char * pwd )
//{
//	JniMethodInfo jmi;//定义Jni函数信息结构体 
//	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","goBBS","(Ljava/lang/String;Ljava/lang/String;)V"); 
//	if (isHave) {
//		jstring user=jmi.env->NewStringUTF(uName);
//		jstring passwd=jmi.env->NewStringUTF(pwd);
//		jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID,user,passwd);
//		jmi.env->DeleteLocalRef(user);
//		jmi.env->DeleteLocalRef(passwd);
//		jmi.env->DeleteLocalRef(jmi.classID);
//	}
//}


void JniUtil::TPSubmitUser( int uin,int nSvr,int nLv, char* strName, char* szZoneName )
{
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","TPSubmitUser","(IIILjava/lang/String;Ljava/lang/String;)V"); 
	if (isHave) {
		jstring name=jmi.env->NewStringUTF(UTEXT_CHINE(strName));
		jstring zone=jmi.env->NewStringUTF(UTEXT_CHINE(szZoneName));
		/*nRtl	= */jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID,uin,nSvr,nLv,name,zone);
		jmi.env->DeleteLocalRef(name);
		jmi.env->DeleteLocalRef(zone);
		jmi.env->DeleteLocalRef(jmi.classID);
	}
}

void JniUtil::goPay( int nVal,int nSvrId,char* szAcc, char* szOrder )
{
	int nRtl	= 0;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","goPay","(IILjava/lang/String;Ljava/lang/String;)V"); 
	if (isHave) {
		jstring acc=jmi.env->NewStringUTF(szAcc);
		jstring order=jmi.env->NewStringUTF(szOrder);
		/*nRtl	= */jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID,nVal,nSvrId,acc,order);
		jmi.env->DeleteLocalRef(acc);
		jmi.env->DeleteLocalRef(order);
		jmi.env->DeleteLocalRef(jmi.classID);
	}
	//return nRtl;
}
void JniUtil::goTpCenter()
{
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","goTpCenter","()V"); 
	if (isHave) { 
		jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID);
	}
}

void JniUtil::Update()
{
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","update","()V"); 
	if (isHave) { 
		jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID);
	}
}

void JniUtil::OtherEvent( int nVal )
{
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","OtherEvent","(I)V"); 
	if (isHave) { 
		jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID,nVal);
	}
}

//void JniUtil::TPResetVersionFile( int nVal )
//{
//	JniMethodInfo jmi;//定义Jni函数信息结构体 
//	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","TPResetVersionFile","()V"); 
//	if (isHave) { 
//		jmi.env->CallStaticVoidMethod(jmi.classID,jmi.methodID,nVal);
//	}
//}

char* JniUtil::TPOpenVersionUrl( char* szUrl,char* szOut )
{
	jstring jstr;

	int b = 0;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","TPOpenVersionUrl","(Ljava/lang/String;)Ljava/lang/String;"); 
	if (isHave) { 
		jstring url=jmi.env->NewStringUTF(szUrl);
		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID,url));
		jmi.env->DeleteLocalRef(url);
		jmi.env->DeleteLocalRef(jmi.classID);

		char* tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
		strcpy(szOut,tchar);
		jmi.env->ReleaseStringUTFChars(jstr,tchar);
	}
	return szOut;
}

int JniUtil::TPDownZipFile( char* szUrl,char* szSaveName )
{
	int b = 0;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","TPDownZipFile","(Ljava/lang/String;Ljava/lang/String;)I"); 
	CCLog("111122222AAAA333");
	if (isHave) { 
		CCLog("111122222AAAA4444");
		jstring url=jmi.env->NewStringUTF(szUrl);
		jstring sname=jmi.env->NewStringUTF(szSaveName);
		CCLog("111122222AAAA5555");
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID,url,sname);
		CCLog("111122222AAAA6666");
		jmi.env->DeleteLocalRef(url);
		jmi.env->DeleteLocalRef(sname);
		jmi.env->DeleteLocalRef(jmi.classID);
	}
	return b;
}

int JniUtil::TPUnZipFile( char* szName )
{
	int b = 0;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","TPUnZipFile","(Ljava/lang/String;)I"); 
	if (isHave) { 
		jstring sname=jmi.env->NewStringUTF(szName);
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID,sname);
		jmi.env->DeleteLocalRef(sname);
		jmi.env->DeleteLocalRef(jmi.classID);
	}
	return b;
}

int JniUtil::getSystemVersion()
{
	int b;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getSystemVersion","()I"); 
	if (isHave) { 
		b=jmi.env->CallStaticIntMethod(jmi.classID,jmi.methodID);
	}
	return b;
}

char* JniUtil::getMacAddress()
{
	static char szRtl[256]={0};
	jstring jstr;
	JniMethodInfo jmi;//定义Jni函数信息结构体 
	bool isHave = JniHelper::getStaticMethodInfo(jmi,"com/mejoygame/rpgwar/uc/RpgWar","getMacAddress","()Ljava/lang/String;"); 
	if (isHave) { 
		jstr=static_cast<jstring>(jmi.env->CallStaticObjectMethod(jmi.classID,jmi.methodID));
		char * tchar=(char*)jmi.env->GetStringUTFChars(jstr,0);
		strcpy(szRtl,tchar);
		jmi.env->ReleaseStringUTFChars(jstr,tchar);
	}
	return szRtl;
}

#endif

ResControl::ResControl()
{
	m_pList	= xnList::Create();
}

ResControl::~ResControl()
{
	while(m_pList->Count)
		delete (_ResDownObj*)m_pList->Delete(0);
	m_pList->Free();
}

void ResControl::Load( char* szFile )
{
	while(m_pList->Count)
		delete (_ResDownObj*)m_pList->Delete(0);
	xnExtstr * pStr = xnExtstr::Import(szFile);
	if( pStr == NULL )
		return;
	int index = 0;
	char	szTmp[256];
	while (pStr->SetSection(index++) != NULL)
	{
		_ResDownObj*	pRs	= new _ResDownObj();
		pRs->nLvLimit		= atoi (pStr->ReadString ("lv",0));
		pRs->szUrl			= strdup(pStr->ReadString ("res",""));
		m_pList->Add (pRs);
	}
	pStr->Free ();
}

_ResDownObj* ResControl::GetByLv( int nLv )
{
	for (int i = 0; i < m_pList->Count; i++)
	{
		_ResDownObj* pRs	= (_ResDownObj*)m_pList->Items[i];
		if(pRs->nLvLimit <= nLv)
			return pRs;
	}
	return NULL;
}
