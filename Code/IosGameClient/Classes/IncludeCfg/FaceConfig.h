#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>


struct FaceObj
{
	int nID;
	int nTotalFrm;
	int nSpeed;
};

class FaceConfig : public CSingleton<FaceConfig>
{
public:
	FaceConfig();
	~FaceConfig();
	void LoadFile(char * pName);
	FaceObj* GetFace(int nId);

	xnList* m_pList;
};