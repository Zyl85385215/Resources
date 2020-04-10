#ifndef _JCH_TIMER_FUNC
#define _JCH_TIMER_FUNC
#include <WINDOWS.H>
#include <TIME.H>
#include <sys/TIMEB.H>

#include "mhSingleton.hpp"

class ITimer
{
public:
	virtual ~ITimer(){}

	virtual DWORD  getTickCount()=0;
};

// 反加速计时
class CAntiAcceleratorTimer:public CMSingleton<CAntiAcceleratorTimer>
{
public:
	CAntiAcceleratorTimer();
	~CAntiAcceleratorTimer();

	DWORD		getTickCount();

private:
	ITimer*		m_pTimer;
};

extern "C" DWORD xnGetTickCount();

#endif