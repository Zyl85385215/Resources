#pragma once

#include "singleton.h"
#include "xnlist.h"
#include <Windows.h>
#include <map.h>


struct GiveItemInfo
{
	DWORD dwItemID;
	int nPer;
};

class EndLessInfo
{
public:
	EndLessInfo();
	~EndLessInfo();
	int nLvlLow;
	int nLvlHigh;
	int nStep;
	int nGold;
	int nExp;
	int nPer;
	char *szText;
	xnList *m_pList;
};

class EndLessConfig : public CSingleton<EndLessConfig>
{
public:
	EndLessConfig();
	~EndLessConfig();
	void  LoadFile(char* szFile);
	EndLessInfo *GetInfo(int nLvl, int nStep);
	DWORD GetGiveItem(int nLvl, int nStep, int nRnd, int nRnd2);
private:
	xnList *m_pList;
};

struct BookRInfo
{
	int nLvlDetal;
	int nPer;
};

struct CollectInfo
{
	int nSkillID;
	int nPer;
	DWORD dwStateA;
	DWORD dwStateB;
	DWORD dwVal;
};

struct BookId
{
	int nSkillID;
};

struct PieceID
{
	int nPieceID;
};


class BookCollectInfo
{
public:
	BookCollectInfo();
	~BookCollectInfo();
	int     m_nBaseSkillID;
	xnList *m_pBookIDList;
	xnList *m_pbookSkillList;
};

//Œ‰¡÷≤–’¬≤ø∑÷
class NewBookCollectConfig : public CSingleton<NewBookCollectConfig>
{
public:
	NewBookCollectConfig();
	~NewBookCollectConfig();
	void  LoadFile(char* szFile);
	int   GetBookCollectID(BYTE bJob, int nRand);
	int   GetJobNum(BYTE bJob); 
	DWORD  GetMonDrop(int nLvl, int nRand);
	DWORD  GetBossDrop(int nLvl, int nRand);
	bool   GetThisJobCanGet(BYTE bJob, int j);
	int    GetDrawSkillOrder(BYTE bJob, int nPos);
	int    GetOrderBySkillID(int nSkillID);
	int    GetDrawSkillID(BYTE bJob, int nPos);
	bool   AnalySkill(DWORD dwSkillID, int nLevel, BYTE bJob);
	bool   AnalyPiece(DWORD dwSkillID, DWORD dwOrder, BYTE bJob);
	CollectInfo  *GetAddStateOnSkill(DWORD dwSkillID, int nRand, int nLvl);
	int	   GetAddBookLevelPer(int nSkillID, int nLevel);
	int  GetSkillIDByOrder(int nPos);
	BookCollectInfo *GetBookCollectInfo(int nSkillID);
	int    GetTotalNum(int nPos);
	bool bCanGetRobot(int nDelt, int nPer);
	char   *szsFlagName;
	char   *szrFlagName;
	int    m_nRChance;
	int    m_nSChance;
	int    m_nSPer;
	int    m_nOpenLevel;
private:
	xnList *m_pList;
	xnList *m_pJiangList;
	xnList *m_pXiaList;
	xnList *m_pDaoList;
	xnList *m_pYaoList;
	xnList *m_pCiList;
	xnList *m_pGongList;
	
	int    m_nMonPer;
	int    m_nBossPer;
	DWORD  m_dwBaodianID;
	xnList *m_pRList;


};