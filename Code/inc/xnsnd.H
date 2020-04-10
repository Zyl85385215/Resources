#ifndef _XN_SND_H_
#define _XN_SND_H_

#include <windows.h>
#include <stdio.h>

#define SND_VOLUME_MIN	0
#define SND_VOLUME_MAX	255
#define SND_AT_LEFT 	0
#define SND_AT_CENTER	128
#define SND_AT_RIGHT	255

enum
{
	SND_ERR,
	SND_PLAYING,
	SND_LOOPING,
	SND_STOP
};

struct xnSndPlayID {
	int	volume;
	int	pan;

	void	Stop(void);
	void	SetVol(int vol);
	void	SetPan(int pan);
	int		GetStatus(void);
};

struct xnSnd {
	BOOLEAN stereo;
	int rate;
	int	bits;

	static xnSnd * Load(char * wavFileName, int channel);
	static xnSnd * SetSnd(void * wavBuf, int channel);
	void	Free(void);
	xnSndPlayID * Play(int val, int pan = SND_AT_CENTER, BOOLEAN loop = FALSE, BOOLEAN returnHandle = FALSE);
};

struct xnSndIn {
	static xnSndIn * Create(int bitsPerSample = 16, int samplesPerSecond = 22050, BOOLEAN stereo = FALSE);
	void Free(void);

	BOOLEAN Start(void);
	void Stop(void);
	void *GetInData(void);
	void SetVolume(int volume);
};

struct xnG729a {
	static void * Encode(void * src);
	static void * Decode(void * src);
};

extern	int xnSndMainVolume;

BOOLEAN xnSndInit(void);
void xnSndShutDown(void);
void xnSndFreeAll(void);
void xnSndStopAll(void);

int xnSndSetMainVol(int volume);
#define xnSndGetMainVol()	xnSndMainVolume


#pragma comment(lib,"G729a.lib")
/*
xnSnd * xnSndLoad(char * wavFileName, int channel);
//xnSnd * xnSndSet(__PCMWAVEFORMAT * wavData, int channel);
void xnSndFree(xnSnd * sndHandle);

xnSndPlayID * xnSndPlay(xnSnd * sndHandle, 
						  int vol, 
						  int pan = SND_AT_CENTER, 
						  BOOLEAN loop = FALSE, 
						  BOOLEAN returnHandle = FALSE);

void xnSndStop(xnSndPlayID * playHandle);

void xnSndSetVol(xnSndPlayID * playHandle, int vol);
void xnSndSetPan(xnSndPlayID * playHandle, int pan);

int xnSndGetStatus(xnSndPlayID * playHandle);
*/

#endif	/* _XN_SND_H_ */
