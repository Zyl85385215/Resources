#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

class SEffTrig
{
public:
	BYTE	bTrigType;		//1��������,2�ܻ�����, 3��������, 4���ܸ���, 5ŭ���ۼ� 6״̬�˺� 7����	9����
	BYTE	bTrigEffType;	//0���ּ���ͼ��, 1ֱ���ͷż���
	WORD	wTrigVal;		//��ֱȻ����
	WORD	wTrigID;		//��һ������ID
};

class SEffLvInfo
{
public:
	SEffLvInfo()	{szDesc = NULL;}
	~SEffLvInfo();
	BYTE	byUpLvNeed;		//��ǰ���ĵȼ�����
	WORD	wUpItemNeed;	//������Ʒ����
	int		nMpCost;

	int		nP_X;
	BYTE	nP_Y1Type;		//��ǰѪ��, ���Ѫ��,��ǰ����, �������, �﹥, ħ��, ���, ħ��, ����,����,����, �ƶ��ٶ�
	int		nP_Y1;			//��ֱ�
	BYTE	nP_Y2Type;
	int		nP_Y2;

	DWORD	dwCDTick;
	BYTE	bAddStateID;		//����״̬ID����
	BYTE	bAddSkillID;		//���ʴ��ļ���״̬ID����
		
	WORD	wRangeAdd;			//���ⷶΧ
	BYTE	bTargetAdd;
	WORD	wIgnoDef;			//��ֱȴ�͸����
	char*	szDesc;
};

class SkillBase
{
public:
	SkillBase();
	~SkillBase();

	SEffLvInfo*	GetSELv(BYTE bLv,bool bSafe=true);

	WORD	wSkillID;
	BYTE	bType;			//���ܷ���, 0����, 1����  2�⻷   3��������,4 ŭ������
	WORD	wCDPos;

	//�ͻ�����Ϣ
	char*	szName;
	//char*	szDesc;
	WORD	wIcon;		
	
	//ѧϰҪ��
	BYTE	byJob;			//0������  1-4����ְҵ	���������10 
	int		nGoldLvNeed;
	WORD	wItemLvNeed;

	WORD	wPreSkillID;	
	BYTE	bPreSkillLv;
	BYTE	byCostSPoint;

	//ʹ������
	BYTE	bCostNoPer;	//�ٷֱ�����
	int		nMpCost;
	int		nHpCost;
	int		nSpCost;
	WORD	wItemCost;	//ʹ��������Ʒ
	
	//��Ч
	WORD	wStartEff;
	WORD	wFlyEff;
	WORD	wEndEff;

	DWORD	dwEndDelay;
	BYTE	byStartEffDir;
	BYTE	byOnlyEnemy;

	//�������⼼��
	xnList*	pListTrigEff;
	
	//�˺�Ч��
	WORD	wAtkRange;
	BYTE	bAoeType;			//0�����ܱ߷�Χ, 1����, 2��Χ, 3ֱ��
	WORD	wAoeRange;			
	BYTE	bMaxTarget;			//������ø���
	BYTE	bActType;			//0��,1���  2��˸  3����
	BYTE	bDmgType;			//1�﹥, 2ħ��,0���� 3,�ٻ�
	
	xnList*	pListStateAdd;		//0����,1�Է� //״̬

	//���ȼ�������Ϣ
	xnList*	pListLvInfo;		
};

class SkillConfig	: public CSingleton<SkillConfig>
{
public:
	SkillConfig();
	~SkillConfig();

	void			LoadFile(char*	szFile);
	void			LoadFileJob(char*	szFile);

	SkillBase*		GetSkillBase(int nID);

	SkillBase*		GetJobSkill(BYTE bJob, BYTE bPos);

	xnList*		m_pLJobSkill[5];	//ְҵ����
private:
	xnList*		m_pList;
};