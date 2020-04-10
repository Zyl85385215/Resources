#ifndef __MP3_H__
#define __MP3_H__

enum {
	MP3_STOP,
	MP3_END,
	MP3_PLAYING,
	MP3_LOOPING,
};

struct xnMp3 {
	int	freq;
	int	stereo;
	int	bits;
	int	length;
	int	volume;
	int	status;

	static xnMp3 * Open(char * mp3FileName, BOOLEAN loadFlag = FALSE);
	void Close(void);
	void Play(int volume, BOOLEAN loopFlag = FALSE);
	void Stop(void);
	void Pause(void);
	void Replay(void);
	void SetVol(int volume);
};

//BOOLEAN xnMp3Init(void);
//void xnMp3ShutDown(void);

/*
xnMp3 * xnMp3Open(char * xnMp3FileName);
void xnMp3Close(xnMp3 * handle);

int xnMp3GetFreq(xnMp3 * handle);
int xnMp3GetStereo(xnMp3 * handle);

void xnMp3Play(xnMp3 * handle, int volume, BOOLEAN loopflag);
void xnMp3Stop(xnMp3 * handle);
void xnMp3Pause(xnMp3 * handle);
void xnMp3Replay(xnMp3 * handle);

int xnMp3SetVol(xnMp3 * handle, int volume);
int xnMp3GetVol(xnMp3 * handle);
*/

#endif
