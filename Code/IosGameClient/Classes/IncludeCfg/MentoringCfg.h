#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct _mbInfo
{
	int nID;
	int nNum;
	int nPer;
};

struct MentoringBase
{
	BYTE	byLv;
	char	szName[20];			//����
	int		nNeedval;			//��������
	int		nPuplecnt;			//ͽ������
	xnList* pList;
};

class MentoringCfg	: public CSingleton<MentoringCfg>
{
public:
	MentoringCfg();
	~MentoringCfg();
	void	LoadFile(char* pFile);
	MentoringBase*	GetCurBase(int nNum);
	MentoringBase*	GetNextBase(int nNum);

	xnList* m_pList;
	int	m_nExpaddid;			//��Ӿ���ӳ�ID
	int	m_nDaylyexpcoe;		//ÿ����ȡ����ϵ��
	int	m_nDaylygoldcoe;		//ÿ����ȡ���ϵͳ
	int	m_nDaylypower;		//ÿ����ȡ����
	int	m_nDaylyhonour;		//ÿ����ȡʦ��
	int	m_nGraduateexp;		//��ҵ��ȡ����
	int	m_nGraduategold;		//��ҵ��ȡ���
	int	m_nGraduatehonour;	//��ҵ��ȡʦ��
};

struct MarrySKObj
{
	char*	pName;
	char*	pIcon;
	BYTE	byType;
	xnList* pLvList;
};

struct MarryAward
{
	int nNeed;
	int nExp;
	int nGold;
	int nPoint;
};

class MarrySkillCfg	: public CSingleton<MarrySkillCfg>
{
public:
	MarrySkillCfg();
	~MarrySkillCfg();
	void	LoadFile(char* pFile);

	xnList* m_pList;
	xnList*	m_pAwardList;
};