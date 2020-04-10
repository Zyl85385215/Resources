#ifndef _EFFECT_FUNC_H_
#define _EFFECT_FUNC_H_

#include "xngrp16.h"
#include "xneffect.h"

struct xnEffectFunc {
	void (*Init)(xnEffect * p);
	void (*ShutDown)(xnEffect * p);
	void (*Update)(xnEffect * p);
	void (*Draw)(xnEffect * p);
};

extern xnEffectFunc xnEffectFade;
extern xnEffectFunc xnEffectSide;
extern xnEffectFunc xnEffectScroll;
extern xnEffectFunc xnEffectFire;
extern xnEffectFunc xnEffectRipple;
extern xnEffectFunc xnEffectRain;

extern xnEffectFunc xnEffectEllipse;
extern xnEffectFunc xnEffectEllipseSwap;
extern xnEffectFunc xnEffectStar;
extern xnEffectFunc xnEffectStarSwap;
extern xnEffectFunc xnEffectClockSwap;
extern xnEffectFunc xnEffectTransverseSwap;
extern xnEffectFunc xnEffectVerticalSwap;

#endif
