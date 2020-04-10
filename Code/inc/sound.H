#ifndef _DX_SOUND_H_
#define _DX_SOUND_H_

#include <windows.h>
#include <stdio.h>
#include <dsound.h>

//======================================================================
//
//      DIRECT SOUND
//
//======================================================================

// ** DEFINES **********************************************************
struct	__PCMWAVEFORMAT
{
  DWORD         riffID;
  DWORD         dwRiffLen;
  DWORD         waveID;
  DWORD         fmtID;
  DWORD         dwFmtLen;
  PCMWAVEFORMAT    pcm;
  DWORD         dataID;
  DWORD         dwDataLen;
};

BOOLEAN DxSndInit(void);
void DxSndShutDown(void);

LPDIRECTSOUNDBUFFER DxSndCreateBuf(int rate,
								   BOOLEAN stereo,
								   BOOLEAN bit16,
								   int bufSize);

LPDIRECTSOUNDBUFFER DxSndDuplicateBuf(LPDIRECTSOUNDBUFFER lpDSB);

void DxSndFreeBuf(LPDIRECTSOUNDBUFFER lpDSB);

BOOLEAN DxSndCopyBuf(LPDIRECTSOUNDBUFFER lpDSB,
					 int DSB_Pos,
					 BYTE * buf,
					 int copyLength);

BOOLEAN DxSndFillBuf(LPDIRECTSOUNDBUFFER lpDSB,
					 int DSB_Pos,
					 BYTE fillByte,
					 int fillLength);

BOOLEAN DxSndPlay(LPDIRECTSOUNDBUFFER lpDSB,
				  int pos,
				  int vol,
				  int pan,
				  BOOLEAN loopFlag);

BOOLEAN DxSndStop(LPDIRECTSOUNDBUFFER lpDSB);

BOOLEAN DxSndSetVol(LPDIRECTSOUNDBUFFER lpDSB, int vol);
BOOLEAN DxSndSetPan(LPDIRECTSOUNDBUFFER lpDSB, int pan);
BOOLEAN DxSndSetPos(LPDIRECTSOUNDBUFFER lpDSB, int pos);

int DxSndGetStatus(LPDIRECTSOUNDBUFFER lpDSB);
int DxSndGetPlayPos(LPDIRECTSOUNDBUFFER lpDSB);


#pragma comment(lib,"dsound.lib")


#endif	/* _DXSND_H_ */
