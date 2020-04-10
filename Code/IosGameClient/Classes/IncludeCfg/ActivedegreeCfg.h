#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

#define MAX_DEGREE_LEVEL	4

struct ADAwardPart{
	int nID;
	int nCnt;
};

struct ADAward{
	int nDegree;
	xnList* pList;
};

struct ADPartChoose{
	int nCount;
	char * pName;
	char * pIocn;
	char * pForm;
	char * pComp;
	char * pDesc;
	BYTE byAction;
	BYTE byObject;
	int nID;
	int nVal;

};

struct ADDaylyPart{
	int nPoint;
	xnList* pList;
};

class ActiveDegree : public CSingleton<ActiveDegree>
{
public:
	ActiveDegree();
	~ActiveDegree();

	void LoadFile(char * pFile);
	void GetNums(char* pMsg,xnList* pList);
	int CheckLevel(int nPoint);

	ADAward szAward[MAX_DEGREE_LEVEL];
	xnList* m_pDaylyList;
};