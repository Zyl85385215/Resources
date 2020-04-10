#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>


enum _HORSE_CULTIVATE
{
	HORSE_CULTIVATE_HP,
	HORSE_CULTIVATE_MP,
	HORSE_CULTIVATE_AD,
	HORSE_CULTIVATE_DD,
	HORSE_CULTIVATE_AP,
	HORSE_CULTIVATE_DP,
	HORSE_CULTIVATE_TOTAL,
	HORSE_CULTIVATE_MAX,
};


enum
{
	HORSE_PARAM_HPMAX,
	HORSE_PARAM_MPMAX,
	HORSE_PARAM_AD,
	HORSE_PARAM_DD,
	HORSE_PARAM_AP,
	HORSE_PARAM_DP,
	HORSE_PARAM_MAX,
};

enum 
{
	HORSE_NUM_CULTIVATE,
	HORSE_NUM_MAX,
};

struct HorseBasic
{
	char*	pName;								//����
	int		nID;								//ID
	int		szBaseParam[HORSE_PARAM_MAX];		//��ʼ����
	int		szBaseUp[HORSE_PARAM_MAX];			//�ɳ���
	BYTE	byQuality;							//Ʒ��
	int		nSkin;								//Ƥ��
	BYTE	byLvLimit;							//�ȼ�����
	int		nSpeed;								//�ٶ�
	int		nPotential;							//Ǳ��
	int		szUpSkin[3];						//���׺�Ƥ��
};

class HorseBaseCfg : public CSingleton<HorseBaseCfg>
{
public:
	HorseBaseCfg();
	~HorseBaseCfg();
	void	LoadFile(char * pFileName);
	void	LoadLvFile(char* pFileName);
	int		GetLvExp(int nLv);
	HorseBasic* GetBasic(int nID);
	int		GetFeedExp(int nLv,int nExp);

private:
	xnList* m_pList;
	xnList* m_pLvList;
};