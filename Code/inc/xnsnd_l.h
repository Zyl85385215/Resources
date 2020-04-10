#ifndef _XN_SND_LOW_H_
#define _XN_SND_LOW_H_

#include <windows.h>
#include <stdio.h>
#include "sound.h"

struct XN_SND_DATA
{
	LPDIRECTSOUNDBUFFER lpDSB;
	BOOLEAN 	keepFlag;
};

struct XN_SND_INFO
{
	XN_SND_INFO * nextSnd;
    __PCMWAVEFORMAT * wavData;
	XN_SND_DATA * channelData;
	int 	channel;
	int 	channelPos;
	BOOLEAN loadFlag;
};

#endif	/* _XN_SND_LOW_H_ */
