#pragma once

#include "xnlist.h"
#include "singleton.h"
#include "GlobalDefine.h"
//�����ļ��趨�Ĺ�Ч��Ϣ

class EffectObj
{
public:
	EffectObj()	{m_szSound	= NULL;}
	~EffectObj();
	int		m_nEffID;			//����ID, �������ͬ��ͼƬ����ɫ����
	int		m_nRealSprID;		//ʵ��effect�е�ID
	int		m_nX,m_nY;			//���ĵ�
	BYTE	byR,byG,byB;
	int		m_nHeight;
	float	m_nTimeFrm;
	int		m_nRunType;
	int		m_nLoopTime;
	int		m_nZorder;
	char*	m_szSound;	
	int		m_nScale;
	BYTE	m_byScaleBySize;		//�������Ӱ�ӷŴ���
	BYTE	m_bPosMark;
};

class EffectCenter	: public CSingleton<EffectCenter>
{
public:
	EffectCenter();
	~EffectCenter();

	void		InitFile(char* szFile);

	EffectObj*	GetEffObj(int nID);

	xnList*		m_pList;
};