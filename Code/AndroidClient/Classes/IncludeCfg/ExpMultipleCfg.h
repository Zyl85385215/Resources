#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif


struct ExpMulObj
{
	int nID;
	char* szName;
	int nAddPer;			//�ӳɱ��ʣ��ٷֱȣ������
	bool ifDisOnline;		//�Ƿ�������ʧ
	bool ifDisDie;			//�Ƿ�������ʧ
	bool ifTimeAdd;			//�Ƿ�ʱ�����
	int nTime;				//����ʱ�䣬-1��ʾ����
	BYTE byTargent;			//�ӳɶ���0�ˣ�1����
	int nFamily;			//��Ⱥ
};

class ExpMultipleCfg	: public CSingleton<ExpMultipleCfg>
{
public:
	ExpMultipleCfg();
	~ExpMultipleCfg();
	void	LoadFile(char* pFile);
	ExpMulObj* GetObj(int nID);

	xnList	*m_pList;
};