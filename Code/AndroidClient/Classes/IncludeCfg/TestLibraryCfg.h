#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif



struct TestObj
{
	int nID;
	char* pQuestion;
	char* pAnswer;
	char* pChoosen[3];
	int nStar;
};

class TestLibraryCfg: public CSingleton<TestLibraryCfg>
{
public:
	TestLibraryCfg();
	~TestLibraryCfg();
	void LoadFile(char* pFile);
	TestObj* GetTest(int nID);
	TestObj* RandTest();

	xnList* m_pList;
};