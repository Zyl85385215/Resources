#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct TitleObj
{
	int nLvl;			//ͷ�εȼ�
	int nNeedVal;		//�������蹦ѫ
	int nLostVal;		//����ʱ�Լ���ʧ�Ĺ�ѫ
	int nSupportVal;	//����ʱĿ�����õĹ�ѫ
	int nHpAdd;			//Ѫ���ӳ�
	int nMpAdd;			//ħ���ӳ�
	int nAdAdd;			//�﹥�ӳ�
	int nDdAdd;			//����ӳ�
	int nApAdd;			//ħ���ӳ�
	int nDpAdd;			//ħ���ӳ�
	char* pName1;		//����ͷ��
	char* pName2;		//����ͷ��
};

class TitleConfig : public CSingleton<TitleConfig>
{
public:
	TitleConfig();
	~TitleConfig();
	void	LoadFile(char * pFile);
	TitleObj* GetObj(int nHonour);
	TitleObj* GetNextObj(int nHonour);
	void	GetPrograssVal(int nHonour,int& nCur,int& nMax);
	int		GetMaxHonour();


	xnList* m_pList;
};