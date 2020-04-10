#ifndef __DEBUG_H__
#define __DEBUG_H__

#ifdef	_DEBUG
	BOOLEAN debugInit(int w, int h);
	void debugShutDown(void);
	void debugOut(BOOLEAN a, int x, int y, char * format, ...);
#else
	#define debugInit
	#define debugShutDown()
	#define debugOut
#endif

#endif
