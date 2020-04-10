#pragma once

#include "singleton.h"
#include "xnlist.h"
#ifndef _ISSERVER
#include "GlobalDefine.h"
#else
#include <Windows.h>
#endif

const char* const szLuaFunctionOnQuestGiveUp = "OnQuestGiveUp";
const char* const szLuaFunctionBeforeQuestGiveUp = "BeforeQuestGiveUp";
const char* const szLuaFunctionOnQuestComplete = "OnQuestComplete";
const char* const szLuaFunctionBeforeQuestComplete = "BeforeQuestComplete";
const char* const szLuaFunctionOnQuestAccept = "OnQuestAccept";
const char* const szLuaFunctionBeforeQuestAccept = "BeforeQuestAccept";

class _QConditon
{
public:
	BYTE	bValPos;
	BYTE	bDType;		//1ɱ�� 2ɱ�ֻ����Ʒ 3������ʽ�����Ʒ 8�����Ϊ 9����
	WORD	wDID;			
	WORD	wDNum;
	char*	szTitle;
};

class _QMonDrop
{
public:
	WORD	wMonID;
	WORD	wItemID;		//99999 ��Ϊ�ɼ� 99998���޵�
	WORD	wPerChance;
};

class QuestObj
{
public:
	QuestObj();
	~QuestObj();

	//������Ϣ
	WORD	wQID;
	WORD	wLine;	//����    FLAG����2��ֵ, 0 ��ʾ�������
	BYTE	bStep;	//���ߵ�һ�׶�, ͬʱ2��step��ͬ�ı�ʾ��ѡһ��,��˳����

	BYTE	bType;	//��ʾ��(0����,1֧��,2�, 3����, 4��λ)

	//��������
	BYTE	bAllDone;	//0�������ĳ��, 1�������
	xnList*	pLPreQ;		//�������ǰ��
	BYTE	bLvNeed;
	BYTE	bCtyNeed[3];	//0�޹���,1����,2����
	BYTE	bJobNeed[5];	//0��ְҵ,1սʿ, 2��ʦ,3ǹ��,4��ʦ
	
	WORD	wNpcSrc;		//��ʼNPC
	WORD	wNpcEnd;
		
	WORD	wItemNeed;		//��Ҫ����ʲô��Ʒ
	BYTE	bItemCost;		//��������

	WORD	wEnemyID;		//�������й�
	BYTE	bCallType;		//0,ɱ��, 1,2,3
	WORD	wStateID;
	
	BYTE	bDayTime;		//ÿ�մ�������
	//��������
	char*	szName;

	xnList*	pLTalkSrc;		//��ʼ�Ի���Ϣ
	xnList*	pLTalkEnd;
		
	char*	szDescSrc;
	char*	szDescEnd;
	
	//����Ҫ��
	xnList*	pLQCondtion;

	xnList*	pLMonDrop;		//�ı�������
	
	//������
	int		nAwardExp;
	int		nAwardGold;
	BYTE	bSelOne;		//��ѡ1����ȫ��
	xnList*	pLAwardItem;	//_muleparam		A:id, B: num

	BYTE	byValCnt;
};

class QuestLine
{
public:
	QuestLine();
	~QuestLine();

	WORD		wLine;
	xnList*		m_pListQStep;
};

class QuestCfg	: public CSingleton<QuestCfg>
{
public:
	QuestCfg();
	~QuestCfg();

	void		LoadFile(char*	szFile);
	QuestObj*		GetQuestByID(WORD wQId);
	
	xnList*		m_pListQLine;

	bool		GetFamilyQuest(int nLv,xnList* pList);
	bool		GetMarryQuest(int nLv,xnList* pList);
private:
	void		_addNewQuest(QuestObj* pObj,WORD wLine);
	QuestLine*	_getQLine(WORD	wLine,bool bCreate	= false);
};