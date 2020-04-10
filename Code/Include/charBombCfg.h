#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>
#include <map>

/*
enum{
	AUTO_MSG_TYPE_EQUIP_STAR,
	AUTO_MSG_TYPE_MAX,
};*/


class SimpleParam
{
public:
	SimpleParam();
	int		nszVal[10];

	SimpleParam		operator*(int nVal);
	void			operator+=(SimpleParam& spVal);
	SimpleParam		operator+(SimpleParam& spVal);
	//int		nHp;
	//int		nMp;
	//int		nAtkW;
	//int		nAtkN;
	//int		nDefW;
	//int		nDefN;
	//int		nApn;
	//int		nDpw;
	//int		nDunt;
	//int		nMdl;
};

class PBCfgParamLv
{
public:
	BYTE byFLv;
	BYTE byTLv;

	SimpleParam	spPreLv;
};

class PBQuaInfo
{
public:
	BYTE	byQua;
	int		nLvlUpCost;
	int		nLvlEffect;		//10000
	std::map<int,int>	mapQuaVal;		//10000
};


class PlayerBombConfig	: public	CSingleton<PlayerBombConfig>
{
public:
	PlayerBombConfig();
	~PlayerBombConfig();
	void	LoadInfo(char* szFile);
	void	LoadLvFile(char* szFile);
	void	LoadParamFile(char* szFile);
	void	LoadQualityFile(char* szFile);

	bool	DealSParamByLvl(OUT SimpleParam* pSPOut,BYTE bLv,BYTE bLvOld=0);
	bool	DealSParamWithQua(OUT SimpleParam* pSPOut,SimpleParam* pSPLvl,SimpleParam* pSPQua, BYTE bQua);
	
	DWORD	GetLevelUpCost(BYTE bCurLv);
	PBQuaInfo*		GetQuaInfo(BYTE bQua);

	int		zhizi[10];
	double	pingzhi[6];

private:
	int		nLvUpCost[100];

	xnList*	pListParams;
	xnList*	pListQuaInfo;
};

class PlayerAutoMsgCfg
{
public:
	PlayerAutoMsgCfg();
	~PlayerAutoMsgCfg();
	char sztxt[128];
	int per;
};

class PlayerAutoMsgConfig	: public	CSingleton<PlayerAutoMsgConfig>
{
public:
	PlayerAutoMsgConfig();
	~PlayerAutoMsgConfig();

	void	LoadInfo(char* szFile);
	DWORD	StateID;
	PlayerAutoMsgCfg* GetAutoMsgCfg(BYTE pos);
private:
	xnList * m_pList;
};