#ifndef __XENON_H__
#define __XENON_H__

#include <windows.h>
#include "timer.h"
#include "xnmem.h"
#include "xnlink.h"
#include "xnfile.H"
#include "xnini.H"
#include "xnconsole.h"
#include "xnExtstr.h"

#include "xnmain.h"
#include "xnsystem.h"

#include "xngrp16.h"
#include "xnavi.h"
#include "xnflic.H"
#include "xneffect.h"

#include "xnsnd.H"
#include "xnmp3.h"

#include "xninput.h"
#include "xnkeyboard.h"
#include "xnjoystick.h"
#include "xnmouse.h"

#include "xnnet.h"

#ifdef _DEBUG
	#pragma comment(lib, "xenon_d.lib")
#else
	#pragma comment(lib, "xenon.lib")
#endif

#endif
