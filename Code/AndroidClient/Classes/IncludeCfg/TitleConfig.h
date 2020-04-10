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
	int nLvl;			//头衔等级
	int nNeedVal;		//升级所需功勋
	int nLostVal;		//死亡时自己损失的功勋
	int nSupportVal;	//死亡时目标所得的功勋
	int nHpAdd;			//血量加成
	int nMpAdd;			//魔法加成
	int nAdAdd;			//物攻加成
	int nDdAdd;			//物防加成
	int nApAdd;			//魔攻加成
	int nDpAdd;			//魔防加成
	char* pName1;		//联盟头衔
	char* pName2;		//部落头衔
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