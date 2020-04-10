#ifndef __XNOGG_H__
#define __XNOGG_H__

#include <windows.h>

enum {
	OGG_STOP,
	OGG_END,
	OGG_PLAYING,
	OGG_LOOPING,
};

struct xnOgg {
	int	freq;
	int	stereo;
	int	bits;
	int	length;
	int	volume;
	int	status;

	static xnOgg * Open(char * oggFileName, BOOLEAN loadFlag = FALSE);
	static void * encode(void * pcmWav, int * encodeSize = NULL);
	static void * decode(void * oggData);
	void Close(void);
	void Play(int volume, BOOLEAN loopFlag = FALSE);
	void Stop(void);
	void Pause(void);
	void Replay(void);
	void SetVol(int volume);
};

#ifdef _DEBUG
	#pragma comment(lib,"ogg_d.lib")
	#pragma comment(lib,"vorbis_d.lib")
	#pragma comment(lib,"vorbisenc_d.lib")
	#pragma comment(lib,"vorbisfile_d.lib")
#else
	#pragma comment(lib,"ogg.lib")
	#pragma comment(lib,"vorbis.lib")
	#pragma comment(lib,"vorbisenc.lib")
	#pragma comment(lib,"vorbisfile.lib")
#endif

#endif
