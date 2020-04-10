#ifndef _XN_MPEG_H_
#define _XN_MPEG_H_

#include "windows.h"
#include "xngrp16.h"

enum {
	MPEG_STOP,
	MPEG_PLAYING,
	MPEG_LOOPING
};

struct xnMpeg
{
	BYTE	status;
	DWORD	fps;
	DWORD	totalFrames;
	int		width, height;

	static xnMpeg * Open(char * mpegFileName);
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


#endif	
