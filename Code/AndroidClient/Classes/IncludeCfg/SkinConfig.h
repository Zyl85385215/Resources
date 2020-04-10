#pragma once


#include "singleton.h"
#include "xnlist.h"
#include "GlobalDefine.h"

class ObjSkinInfo
{
public:
	int		nId;
	int		nRealID;
	int		nXPer;
	int		nYPer;
	int		nTitleH;
	int		nHead;
	BYTE	bPartCnt;		//��Ƶ��ļ�����
	BYTE	bRectCnt;
	BYTE	bTotalFrm;
	int*	pnVal;
	int		nScale;
	int		nShadowSize;
	BYTE	bExPart;
	char*	szSoundEx;
};

class	CfgObjSkin	: public	CSingleton<CfgObjSkin>
{
public:
	CfgObjSkin();
	~CfgObjSkin();

	void		InitFile(char* szFile);

	ObjSkinInfo*	GetObjSkin(int nID);

	xnList*		m_pList;
};