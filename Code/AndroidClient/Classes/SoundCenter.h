#pragma once

#include "GlobalDefine.h"
#include "xnlist.h"

class SpecialSoundEff
{
public:
	int		nSID;
	void*	pObj;
};
class CSoundCenter
{
public:
	CSoundCenter();
	~CSoundCenter();

	void	SetSoundVal(int nVal);
	void	SetMusicVal(int nVal);

	int		PlayerEffect(const char* szFile,bool bLoop=false);
	void	StopEffect(int nSID);
	void	PlayerBackMp3(const char* szFile,bool bLoop=false);

	void	PauseBackMap3();
	void	ResumeBackMap3();

	xnList*	pListEffCenter;

	char	szAndroidPath[256];
};

extern CSoundCenter* g_pSoundCenter;