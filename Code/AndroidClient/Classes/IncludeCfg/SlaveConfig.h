#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif
#include <map>

static char* s_qualityName[] = {"������","������","������","������","������"};

enum _SLAVE_SKILL
{
	SLAVE_SKILL_ID1,
	SLAVE_SKILL_ID2,
	SLAVE_SKILL_ID3,
	SLAVE_SKILL_ID4,
	SLAVE_SKILL_ID5,
	SLAVE_SKILL_ID6,
	SLAVE_SKILL_MAX,
};

enum _SLAVE_STRENGTHEN
{
	SLAVE_STRENGTHEN_HP,
	SLAVE_STRENGTHEN_MP,
	SLAVE_STRENGTHEN_AD,
	SLAVE_STRENGTHEN_DD,
	SLAVE_STRENGTHEN_AP,
	SLAVE_STRENGTHEN_DP,
	SLAVE_STRENGTHEN_MAX,
};

enum 
{
	SLAVE_NUM_SKILL,
	SLAVE_NUM_STRENGTHEN,
	SLAVE_NUM_SKILLLV,
	SLAVE_NUM_MAX,
};

enum
{
	SLAVE_PARAM_HPMAX,
	SLAVE_PARAM_MPMAX,
	SLAVE_PARAM_AD,
	SLAVE_PARAM_DD,
	SLAVE_PARAM_AP,
	SLAVE_PARAM_DP,
	SLAVE_PARAM_MAX,
};

struct SlaveBasic
{
	char*	pName;								//����
	int		nID;								//ID
	int		szBaseParam[SLAVE_PARAM_MAX];		//��ʼ����
	int		szBaseUp[SLAVE_PARAM_MAX];			//�ɳ���
	BYTE	byQuality;							//Ʒ��
	int		nSkill;								//��ʼ����
	int		nSkin;								//Ƥ��
	BYTE	byLvLimit;							//�ȼ�����
	BYTE	byExchangLv;						//��ǰ�����ȼ�
	int		nExchangeID;						//����ID
	int		nExchangeGold;						//������������
	std::map<int,int> mapExchangeCost;			//�����������
	std::map<int,int> mapExchangeItem;			//����������Ʒ
	int		nAtkDis;							//���﹥������
};
class UserData;
class SlaveBaseCfg : public CSingleton<SlaveBaseCfg>
{
public:
	SlaveBaseCfg();
	~SlaveBaseCfg();
	void	LoadFile(char * pFileName);
	SlaveBasic* GetBaseInfo(int nID);

	void	LoadLvFile(char* pFileName);
	int		GetLvExp(int nLv);

	int		RandSlaveID(int nType,UserData*	pUser);

private:
	xnList* m_pList;
	xnList* m_pLvList;
#ifdef _ISSERVER
	xnList* m_pQWhiteList;
	xnList* m_pQGreenList;
	xnList* m_pQBlueList;
	xnList* m_pQPurpleList;
#endif
};

struct SlaveStreBasic
{
	int nVal;				//��ֵ�ӳ�
	int nSucPer;			//ǿ���ɹ���
	int nLvNeed;			//ǿ���������ȼ�
	int nMoneyCost;			//ǿ����������
	int nItemCost;			//ǿ������ǿ��������
};

class SlaveStrengthenCfg : public CSingleton<SlaveStrengthenCfg>
{
public:
	SlaveStrengthenCfg();
	~SlaveStrengthenCfg();
	void	LoadFile(char* pFileName);
	SlaveStreBasic* GetBase(BYTE byPos,int nLv);

private:
	//xnList* m_pList[SLAVE_PARAM_MAX];
	xnList* m_pList;
};

struct SlaveVariatBasic
{
	int nVal;		//���Լӳɣ���ֱȣ�
	int nMoney;		//��������
	int nSucPer[4]; //��ͬƷ���ṩ�ĳɹ���
};

class SlaveVariatCfg : public CSingleton<SlaveVariatCfg>
{
public:
	SlaveVariatCfg();
	~SlaveVariatCfg();
	void	LoadFile(char* pFileName);
	SlaveVariatBasic* GetBase(BYTE byValv);
	int		GetMaxLv() {return m_pList->Count;}

private:
	xnList* m_pList;
};