#ifndef _XN_TEXTURE_H_
#define _XN_TEXTURE_H_

#include <windows.h>

struct xnTexture {
	WORD Width;
	WORD Height;
	WORD * color;
	WORD ** colorLine;
	BYTE * alpha;
	BYTE ** alphaLine; 
	
	static xnTexture * LoadJpeg(char * jpegFileName);
	static xnTexture * LoadTexture(char * textureName);
	static xnTexture * Create(WORD w, WORD h, BOOLEAN alphaChannel = FALSE);
	
	void	Free(void);
};

#endif