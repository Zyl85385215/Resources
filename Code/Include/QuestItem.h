#ifndef QUESTITEM_H_
#define QUESTITEM_H_
#include <windows.h>
#include <stdio.h>
#include "xnlist.h"

#define WriteData(member)\
	fwrite(&(member),sizeof(member),1,pFile);

#define Writestring(member)\
	if (member)\
	{\
	fwrite(member,1,strlen(member)+1,pFile);\
	}\
	else\
	{\
	char aaaa=0;\
	fwrite(&aaaa,1,1,pFile);\
	}
#define LoadData(member)\
	memcpy(&member,Data+readpos,sizeof(member));\
	readpos+=sizeof(member)

#define LoadDataByType(t,member)\
	memcpy(&member,Data+readpos,sizeof(t));\
	readpos+=sizeof(t)

#define Loadstring(member)\
	if (member)\
	{\
	free(member);\
	}\
	member=strdup((LPCSTR)(Data+readpos));\
	readpos+=strlen(member)+1


enum{
	QUEST_FILE_VERS_ORI=0x11111111,
	QUEST_FILE_VERS_1,
	QUEST_FILE_VERS_2,
	QUEST_FILE_VERS_3,
	QUEST_FILE_VERS_4,
	QUEST_FILE_VERS_5,
	QUEST_FILE_VERS_6,
	QUEST_FILE_VERS_7,
	QUEST_FILE_VERS_8,
	QUEST_FILE_VERS_9,	
	QUEST_FILE_VERS_10,
	QUEST_FILE_VERS_11,
	QUEST_FILE_VERS_12,
	QUEST_FILE_VERS_13,
	QUEST_FILE_VERS_14,
	QUEST_FILE_VERS_MAX
};
enum
{
	quest_master,
	quest_lateral,
	quest_family,
	quest_guild,
	quest_country,
	quest_cycle,
	quest_convoy,
	quest_collect,
	quest_battle,
	quest_job,
	quest_skill,
	quest_special,
	quest_daily,
	quest_team,
	quest_marry,
	quest_achieve,//quest_phase,
	quest_guanxian,
	quest_huodong,
	quest_max,
};

#define  QUEST_FILE_VERS  QUEST_FILE_VERS_14


struct QuestConfig;
typedef QuestConfig * LPQUESTITEM;
typedef int (*PLOADQUESTFUNC)(BYTE * pData,int nSize,LPQUESTITEM*& pResult);
extern PLOADQUESTFUNC loadquestfromversion[QUEST_FILE_VERS_MAX-QUEST_FILE_VERS_ORI];

struct _Items
{
	DWORD ID;
	int Count;
};

struct _cDrop
{
	DWORD monsterid;
	DWORD itemid;
	DWORD maxcnt;
	DWORD chance;
};

struct QuestConfig
{
public:
	
	BYTE type;//0,this is a common quest,1 this is a Random quest
	DWORD RandomID;	//the random quest in random group
	
	BYTE minlevel;//the lowest level to accept quest,
	BYTE maxlevel;// 6-23 the highest level to accept quest,

	BYTE m_bySex;
	BYTE m_nBagNeed;//所需包裹个数

	BYTE jobflag;//the job require
	DWORD stateflag;//must in the state to accept quest,
	DWORD lpContry[10];//contry require
	DWORD dwCountDown;
	DWORD lpOptionQ[5];	//must complete one of 
	DWORD lpMustQ[5];		//must complete all 
	DWORD lpMustFrd[5];
	DWORD NextQID;
	int  bIsRandomID;
	int nKind;
	int QID;
	char* sNPC;	
	char* eNPC;	
	char* sNPCTalk;
	char* eNPCTalk;
	char* IncompleteTalk;
	char* QuestTitle;
	char* Questlog;
	char* szSimpleDesc;
	_Items lpQuestItems[5];	//the quest items give the player

	_Items lpItemRequire[5];
	_Items lpMonsterRequire[5];
	_Items pGuardInfo;		//护送任务怪物ID,目标区域ID

	_cDrop pChgDrop[15];			//change quest item drop when monster dies;

	DWORD expAdd;
	DWORD moneyAdd;
	DWORD honourAdd;
	DWORD moneyBangAdd;
	_Items lpItemOpt[5];	//the items player could only select one
	_Items lpItemMust[5];	//the items will all give player 
	int bCanRepeat;
	int bsNPCNotShow;
	int beNPCNotShow;
	DWORD jobexpAdd;//IDC_EDIT_JOBEXP
	char* szScriptOnGiveUp;//IDC_EDIT_SCRIPT_GIVEUP
	char* szScriptOnAcceptTask;
	char* szScriptOnCompleteTask;
	BYTE completelevel;//完成等级
	WORD wGutType;
	int	nAcceptNeedGold;
	int sNPCID;	
	int eNPCID;	
	int	QuestEnemy;		//任务相关怪(如果接了此任务,则可以用吟唱杀死此怪)
	BYTE* pbScript;// 是否有脚本，这一位是编辑器里没有的，只服务器用到
	//begin add by hyq 2011.04.06 服务器新增加脚本文件
	BYTE* pbScriptQuestAdd_;
	BYTE* pbScriptQuestRemove_;
	char* pGetFormNpc;
	//end  add by hyq 2011.04.06 服务器新增加脚本文件
public:
	static int LoadAllQuestItem(char * filename,LPQUESTITEM*& pResult);	
	static BOOL SaveAllQuest(char * filename,LPQUESTITEM*& pResult,int count);
	static BOOL SaveAllQuest_T(char * filename,LPQUESTITEM*& pResult,int count);
	QuestConfig();
	~QuestConfig();
	QuestConfig & operator =(QuestConfig & pItem);

	void SaveToFile(FILE * hFile);
	int LoadFromFile(BYTE * Data);
	QuestConfig* DeepCopy();
	bool TransToT();
};

static void ClearQuestItemList(LPQUESTITEM* pResult,int count)
{
	for(int i =0;i<count;i++)
	{
		delete pResult[i];
	}
	delete [] pResult;
}

static int LOAD_ORIVERSION(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		pResult[index++]=pInfo;
	}
	return index;
}
static int LOAD_VER_1(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_2(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		pResult[index++]=pInfo;
	}
	return index;
}
static int LOAD_VER_3(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		pResult[index++]=pInfo;
	}
	return index;
}
static int LOAD_VER_4(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		pResult[index++]=pInfo;
	}
	return index;
}
static int LOAD_VER_5(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_6(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_7(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_8(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->completelevel);
		readpos+=sizeof(int);////addHonour
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_9(BYTE * Data,int nSize,LPQUESTITEM*& pResult)
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->bsNPCNotShow);
		LoadData(pInfo->beNPCNotShow);
		LoadData(pInfo->jobexpAdd);		
		Loadstring(pInfo->szScriptOnGiveUp);
		LoadData(pInfo->completelevel);
		LoadData(pInfo->wGutType);	
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->sNPCID);
		LoadData(pInfo->eNPCID);	
		LoadData(pInfo->QuestEnemy);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_10( BYTE * Data, int nSize, LPQUESTITEM *& pResult )
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->bsNPCNotShow);
		LoadData(pInfo->beNPCNotShow);
		LoadData(pInfo->jobexpAdd);		
		Loadstring(pInfo->szScriptOnGiveUp);
		LoadData(pInfo->completelevel);
		LoadData(pInfo->wGutType);	
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->sNPCID);
		LoadData(pInfo->eNPCID);	
		LoadData(pInfo->QuestEnemy);
		LoadData(pInfo->honourAdd);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_11( BYTE * Data, int nSize, LPQUESTITEM *& pResult )
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->bsNPCNotShow);
		LoadData(pInfo->beNPCNotShow);
		LoadData(pInfo->jobexpAdd);		
		Loadstring(pInfo->szScriptOnGiveUp);
		//begin add by hyq 2011.04.01
		Loadstring(pInfo->szScriptOnAcceptTask);
		Loadstring(pInfo->szScriptOnCompleteTask);
		//end add by hyq 2011.04.01
		LoadData(pInfo->completelevel);
		LoadData(pInfo->wGutType);	
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->sNPCID);
		LoadData(pInfo->eNPCID);	
		LoadData(pInfo->QuestEnemy);
		LoadData(pInfo->honourAdd);
		pResult[index++]=pInfo;
	}
	return index;
}
static int LOAD_VER_12( BYTE * Data, int nSize, LPQUESTITEM *& pResult )
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->bsNPCNotShow);
		LoadData(pInfo->beNPCNotShow);
		LoadData(pInfo->jobexpAdd);		
		Loadstring(pInfo->szScriptOnGiveUp);
		//begin add by hyq 2011.04.01
		Loadstring(pInfo->szScriptOnAcceptTask);
		Loadstring(pInfo->szScriptOnCompleteTask);
		//end add by hyq 2011.04.01
		LoadData(pInfo->completelevel);
		LoadData(pInfo->wGutType);	
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->sNPCID);
		LoadData(pInfo->eNPCID);	
		LoadData(pInfo->QuestEnemy);
		LoadData(pInfo->honourAdd);
		LoadData(pInfo->moneyBangAdd);
		pResult[index++]=pInfo;
	}
	return index;
}
static int LOAD_VER_13( BYTE * Data, int nSize, LPQUESTITEM *& pResult )
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->bsNPCNotShow);
		LoadData(pInfo->beNPCNotShow);
		LoadData(pInfo->jobexpAdd);		
		Loadstring(pInfo->szScriptOnGiveUp);
		//begin add by hyq 2011.04.01
		Loadstring(pInfo->szScriptOnAcceptTask);
		Loadstring(pInfo->szScriptOnCompleteTask);
		//end add by hyq 2011.04.01
		LoadData(pInfo->completelevel);
		LoadData(pInfo->wGutType);	
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->sNPCID);
		LoadData(pInfo->eNPCID);	
		LoadData(pInfo->QuestEnemy);
		LoadData(pInfo->honourAdd);
		LoadData(pInfo->moneyBangAdd);
		LoadData(pInfo->lpMustFrd);
		pResult[index++]=pInfo;
	}
	return index;
}

static int LOAD_VER_14( BYTE * Data, int nSize, LPQUESTITEM *& pResult )
{
	int readpos=0,index=0,count=0;
	readpos+=4;
	LoadData(count);
	pResult=new QuestConfig *[count];
	while (readpos<nSize)
	{
		QuestConfig * pInfo=new QuestConfig();
		readpos+=pInfo->LoadFromFile(Data+readpos);
		LoadData(pInfo->NextQID);
		LoadData(pInfo->bIsRandomID);
		LoadData(pInfo->RandomID);
		Loadstring(pInfo->sNPCTalk);
		Loadstring(pInfo->eNPCTalk);
		LoadData(pInfo->lpMonsterRequire);
		LoadData(pInfo->pGuardInfo);
		LoadData(pInfo->pChgDrop);
		Loadstring(pInfo->IncompleteTalk);
		LoadData(pInfo->lpQuestItems);
		LoadData(pInfo->bCanRepeat);
		LoadData(pInfo->nKind);
		Loadstring(pInfo->szSimpleDesc);
		LoadData(pInfo->maxlevel);//the highest level to accept quest,
		LoadData(pInfo->m_bySex);
		LoadData(pInfo->m_nBagNeed);
		LoadData(pInfo->bsNPCNotShow);
		LoadData(pInfo->beNPCNotShow);
		LoadData(pInfo->jobexpAdd);		
		Loadstring(pInfo->szScriptOnGiveUp);
		//begin add by hyq 2011.04.01
		Loadstring(pInfo->szScriptOnAcceptTask);
		Loadstring(pInfo->szScriptOnCompleteTask);
		//end add by hyq 2011.04.01
		LoadData(pInfo->completelevel);
		LoadData(pInfo->wGutType);	
		LoadData(pInfo->nAcceptNeedGold);
		LoadData(pInfo->sNPCID);
		LoadData(pInfo->eNPCID);	
		LoadData(pInfo->QuestEnemy);
		LoadData(pInfo->honourAdd);
		LoadData(pInfo->moneyBangAdd);
		LoadData(pInfo->lpMustFrd);
		Loadstring(pInfo->pGetFormNpc);
		pResult[index++]=pInfo;
	}
	return index;
}
#endif