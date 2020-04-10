#ifndef _XN_AVI_H_
#define _XN_AVI_H_

#include "xngrp16.h"

enum {
	AVI_STOP,
	AVI_PLAYING,
	AVI_LOOPING,
};

struct xnAvi
{
	BYTE	status;
	DWORD	fps;
	DWORD	totalFrames;

	static xnAvi * Open(char * AviFileName);
	void	Close(void);
	void	Play(int sndVolume, BOOLEAN loopFlag);
	void	Stop(void);
	void	Replay(void);
	void	NextFrame(void);
	int		GetFrame(void);
	void	SetFrame(int frameIndex);
	void	SetVol(int voil);
	xnBmp *	GetBmp(void);
};

//BOOLEAN xnAviInit(void);
//void xnAviShutDown(void);

#pragma comment(lib,"vfw32.lib")
#pragma comment(lib,"winmm.lib")

#endif	
