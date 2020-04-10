#pragma once

#include "singleton.h"
#include "xnlist.h"
#include "ShipConfig.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

enum
{
	CAP_AD_UIN,
	CAP_AD_ID,			//Ψһ��ʾ
	CAP_AD_TYPE,		//��������PHASE
	CAP_AD_LV,			//�ȼ�
	CAP_AD_EXP,


	CAP_AD_MAX,
};

class CaptainBase
{
public:
	CaptainBase();
	~CaptainBase();
	
	char*	szName;
	char*	szDesc;
	int		nSpr;
	int		nStar;
	int		nTypeID;

	xnList*	pListEffect;
};

//��ֻ�ȼ��Ļ���Ч��
class CaptainLvCfgObj
{
public:
	BYTE	byLv;
	int		nExpCost;
	int		nGoldCost;
	int		nParams[WARS_PARAM_MAX];
};

class CaptainConfig	: public CSingleton<CaptainConfig>
{
public:
	CaptainConfig();
	~CaptainConfig();

	void		LoadCaptainFile(char*	szFile);
	void		LoadLvFile(char*	szFile);
	CaptainBase*	GetCaptainBase(int	nSType);
	CaptainLvCfgObj*		GetCaptainLv(int nLv);

private:
	xnList*		pListCaptain;
	xnList*		pListLv;
};