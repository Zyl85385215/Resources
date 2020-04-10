#pragma once
#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

struct _simpleAward
{
	int nID;
	int nNum;
};

struct OnlineAward
{
	int nTime;
	_simpleAward xAward;
};

struct MounthAward
{
	int nCount;
	_simpleAward xAward;
};

struct LvupAward
{
	int nLv;
	xnList* pList;
};

struct RechargeAward
{
	int nDays;
	xnList* pList;
};

struct LoginAward
{
	int nDays;
	xnList* pList;
};

class GameAwardConfig	: public CSingleton<GameAwardConfig>
{
public:
	GameAwardConfig();
	~GameAwardConfig();
	void	LoadFile(char* pFile);
	OnlineAward* GetOnlineAward(BYTE byFlag);

	xnList* m_pOnlineList;		//���߽���
	xnList* m_pMounthList;		//ÿ��ǩ������
	xnList* m_pLvupList;		//��������
	xnList* m_pLoginList;		//�������뽱��
	xnList* m_pRechargeList;	//�׳佱��
	xnList* m_pInviteList;	    //�������뽱��
	xnList* m_pLoginNewList;	//���ۼƽ���
};