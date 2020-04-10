#ifndef __FLIC_H__
#define __FLIC_H__

struct  fli
{
	int frames;
	int width;
	int height;
	int speed;
	int current;
	int crntoff;
	int dataOfs;
	BYTE * fliData;
	BYTE * fliBmp;
	BYTE * pal256;
	WORD * pal;
	xnBmp * bmp;
	int  bmpFrame;
};


fli * flicOpen(char *filename);
void flicClose(fli *flicPtr);
void flicNextFrame(fli *flicPtr);

xnBmp * xnAviGetBmp(fli * flicPtr);

#endif // __FLIC_H__
