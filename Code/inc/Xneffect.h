#ifndef _XN_EFFECT_H_
#define _XN_EFFECT_H_

#include "xngrp16.h"

enum {
	XN_EFFECT_FADE,
	XN_EFFECT_SIDE,
	XN_EFFECT_SCROLL,
	XN_EFFECT_FIRE,
	XN_EFFECT_RIPPLE,
	XN_EFFECT_RAIN,

	XN_EFFECT_ELLIPSE,
	XN_EFFECT_ELLIPSESWAP,
	XN_EFFECT_STAR,
	XN_EFFECT_STARSWAP,
	XN_EFFECT_CLOCKSWAP,
	XN_EFFECT_TRANSVERSESWAP,
	XN_EFFECT_VERTICALSWAP,
};

struct xnEffect {
	int time;
	xnBmp * dstBmp;
	xnBmp * srcBmp;
	BOOLEAN	active;
	int		index;
	void * data;

	static xnEffect * Open(xnBmp * dstBmp, xnBmp * srcBmp, int effectIndex, int time);
	void  Close(void);
	BOOLEAN Update(void);
	void Draw(void);
};

#endif
