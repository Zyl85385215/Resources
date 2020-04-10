#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

class StateEffVal
{
public:
	BYTE	bType;	//�������Ե�ǰѪ��, ���Ѫ��,��ǰ����, �������, �﹥, ħ��, ���, ħ��, ����,����,����,�ƶ��ٶ�
	int		nPer;	//��ֱ�
	int		nSpVal;	//��ֵ  ��ֱȷ�0��ʱ���ʾ ��ǰѪ��, ���Ѫ��,��ǰ����, �������, �﹥, ħ��, ���, ħ��, ����,����,����
};

class StateBase
{
public:
	StateBase();
	~StateBase();
	
	WORD	wStateID;
	WORD	wWorkPos;
	BYTE	bWorkVal;

	//�ͻ��˰汾
	char*	szName;
	char*	szDesc;
	WORD	wIcon;
	WORD	wEff;
	WORD	wEffex;
	
	BYTE	bWorkTime;	//���ô���
	DWORD	dwPerTick;	//ÿ�γ���ʱ��
	
	xnList*	pListStateEff;
	BYTE	szbSpEff[8];	//1��ѣ, 2�޵�, 3�����ƶ�, 4 ���ܼ���, 5 ���ܹ���, 6 ������Ʒ��7����

	BYTE	bLayerStart;
	BYTE	bLayerMax;
		
	WORD	wChgSkin;
	WORD	wChgHorse;
	WORD	wChgWeapon;
	BYTE	bSkinPos;
		
	BYTE	bDisType;		//0��, 1 �ƶ�, 2����, 3����, 4������, 5ʹ����Ʒ
	BYTE	bOffLineDis;	//�����Ƿ���ʧ
	BYTE	bSyncClient;

	WORD	wRemoveMark;
};

class StateConfig	: public CSingleton<StateConfig>
{
public:
	StateConfig();
	~StateConfig();

	void	LoadFile(char* szFile);
	StateBase*	GetStateBase(WORD wStateID);
private:
	xnList*		m_pList;
};