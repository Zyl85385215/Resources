#include "SoundCenter.h"
#include "SimpleAudioEngine.h"
#include <stdio.h>
#ifdef USE_ANDROID 
#include "JniUtil.h"
#endif
using namespace CocosDenshion;
CSoundCenter* g_pSoundCenter	= NULL;

CSoundCenter::CSoundCenter()
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(1);
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.4f);

	pListEffCenter	= xnList::Create ();

#ifdef USE_ANDROID
	JniUtil jni;
	char* temp=jni.getAndSDpath();
	sprintf(szAndroidPath,"%s/rpgwar/res/",temp);
#endif
}

CSoundCenter::~CSoundCenter()
{
	while(pListEffCenter->Count)
		delete	(SpecialSoundEff*)pListEffCenter->Delete (0);
	pListEffCenter->Free ();
}

int CSoundCenter::PlayerEffect(const char* szFile,bool bLoop )
{
	if(szFile)
	{
		char	szTmp[512];
		
#ifdef USE_ANDROID
		sprintf(szTmp,"%s%s%s",szAndroidPath,szFile,SOUND_FILENAME);
#else
		sprintf(szTmp,"%s%s",szFile,SOUND_FILENAME);
#endif
		return SimpleAudioEngine::sharedEngine ()->playEffect (szTmp,bLoop);
	}

	return 0;
}

void CSoundCenter::StopEffect( int nSID )
{
	SimpleAudioEngine::sharedEngine ()->stopEffect(nSID);
}


void CSoundCenter::PlayerBackMp3(const char* szFile,bool bLoop )
{
	SimpleAudioEngine::sharedEngine ()->stopBackgroundMusic (true);
	if(szFile)   
	{
#ifdef USE_ANDROID
		char szbuf[256];
		sprintf(szbuf,"%s%s",szAndroidPath,szFile);

		SimpleAudioEngine::sharedEngine ()->preloadBackgroundMusic(szbuf);
		SimpleAudioEngine::sharedEngine ()->playBackgroundMusic (szbuf,bLoop);
#else

		SimpleAudioEngine::sharedEngine ()->preloadBackgroundMusic(szFile);
		SimpleAudioEngine::sharedEngine ()->playBackgroundMusic (szFile,bLoop);
#endif
	}
}

void CSoundCenter::SetSoundVal( int nVal )
{
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(nVal/100.0f);
}

void CSoundCenter::SetMusicVal( int nVal )
{
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5f*nVal/100.0f);
}

void CSoundCenter::PauseBackMap3()
{
	SimpleAudioEngine::sharedEngine ()->pauseBackgroundMusic ();
}

void CSoundCenter::ResumeBackMap3()
{
	SimpleAudioEngine::sharedEngine ()->resumeBackgroundMusic ();
}
