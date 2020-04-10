#pragma once
#include <windows.h>
#include "singleton.h"

class CSlaveAttribConfig : public CSingleton<CSlaveAttribConfig>
{
public:
	int m_nZizhi[10];
	double m_dK[5];
	double m_dU[5];
	int m_nJinhua[10];
	double m_dT[5];
	double m_dJinhuaCanshu;
	void Load( const char * filename );
};